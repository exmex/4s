#include "stdafx.h"

CTClientGate::CTClientGate()
{
	m_pTGATE = NULL;

	m_bCanSelected = FALSE;
	m_bDynamicOBJ = FALSE;
	m_bDrawName = FALSE;

	m_bResetGATE = FALSE;
	m_bOPEN = FALSE;

	m_bType = OT_GATE;
}

CTClientGate::~CTClientGate()
{
}

void CTClientGate::Release()
{
	CTClientObjBase::Release();
	m_bResetGATE = FALSE;
	m_bOPEN = FALSE;
	m_pTGATE = NULL;
}

void CTClientGate::ReleaseData()
{
	CTClientObjBase::ReleaseData();
	m_bResetGATE = FALSE;
	m_bOPEN = FALSE;
	m_pTGATE = NULL;
}

void CTClientGate::InitTGate( CD3DDevice *pDevice, CTachyonRes *pRES)
{
	MAPRES::iterator itOBJ = pRES->m_mapOBJ.find(m_pTGATE->m_dwOBJ);
	CTClientInven *pTEQUIP = new CTClientInven();
	ClearInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

	InitOBJ((LPOBJECT) (*itOBJ).second);

	for( BYTE i=0; i<ES_COUNT; i++)
	{	
		if(m_pTGATE->m_pTITEM[i])
		{
			CTClientItem *pTITEM = new CTClientItem();

			pTITEM->SetItemSlot(i);
			pTITEM->SetTITEM(m_pTGATE->m_pTITEM[i]);
			pTITEM->SetCount(1);

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}
	}

	m_bCollisionType = GetAttrINT(ID_COLLISION_TYPE);
	m_bCamCollision = GetAttrINT(ID_CAM_COLLISION);
	InitSIZE(
		m_pTGATE->m_fScaleX * GetAttrFLOAT(ID_SIZE_X),
		m_pTGATE->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
		m_pTGATE->m_fScaleZ * GetAttrFLOAT(ID_SIZE_Z),
		m_pTGATE->m_fScaleY * GetAttrFLOAT(ID_DEPTH),
		max( m_pTGATE->m_fScaleX, max(
		m_pTGATE->m_fScaleY,
		m_pTGATE->m_fScaleZ)) * GetAttrFLOAT(ID_SIGHT));

	ResetEQUIP( pDevice, pRES);
	ResetTGate();

	m_bHouseMesh = m_pTGATE->m_bHouseMesh;
	m_dwHouseID = m_pTGATE->m_dwHouseID;
	m_dwID = m_pTGATE->m_dwID;
}

BYTE CTClientGate::OpenTGate( BYTE bOPEN)
{
	if( m_bOPEN != bOPEN )
	{
		m_bAction = bOPEN ? m_pTGATE->m_bOpenActID : m_pTGATE->m_bCloseActID;
		m_bOPEN = bOPEN;

		TACTION vActionID = FindActionID(
			m_bAction,
			GetWeaponID(m_bMode));
		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

		return TRUE;
	}

	return FALSE;
}

void CTClientGate::ResetTGate()
{
	D3DXVECTOR3 vTPOS(
		0.0f,
		0.0f,
		0.0f);

	D3DXVECTOR3 vTROT(
		0.0f,
		0.0f,
		0.0f);

	D3DXMATRIX vRotX;
	D3DXMATRIX vRotY;
	D3DXMATRIX vRotZ;
	D3DXMATRIX vPos;

	m_bAction = m_bOPEN ? m_pTGATE->m_bOpenID : m_pTGATE->m_bCloseID;
	m_bResetGATE = FALSE;

	if(m_bOPEN)
	{
		TACTION vActionID = FindActionID(
			m_pTGATE->m_bOpenActID,
			GetWeaponID(m_bMode));
		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

		LPANI pANI = GetCurANI();
		if( pANI && pANI->m_pANI )
		{
			pANI->m_pANI->m_fCurTime = pANI->m_pANI->GetTotalTime();
			ANIKEY vKEY = pANI->m_pANI->GetAniKey();

			vTPOS = D3DXVECTOR3(
				vKEY.m_fPosX,
				vKEY.m_fPosY,
				vKEY.m_fPosZ);

			vTROT = D3DXVECTOR3(
				vKEY.m_fRotX,
				vKEY.m_fRotY,
				vKEY.m_fRotZ);
		}
	}

	D3DXMatrixRotationX(
		&vRotX,
		vTROT.x);

	D3DXMatrixRotationY(
		&vRotY,
		vTROT.y + m_pTGATE->m_fDIR);

	D3DXMatrixRotationZ(
		&vRotZ,
		vTROT.z);

	D3DXMatrixTranslation(
		&vPos,
		vTPOS.x + m_pTGATE->m_fPosX,
		vTPOS.y + m_pTGATE->m_fPosY,
		vTPOS.z + m_pTGATE->m_fPosZ);

	D3DXMatrixScaling(
		&m_vScale,
		m_pTGATE->m_fScaleX,
		m_pTGATE->m_fScaleY,
		m_pTGATE->m_fScaleZ);

	m_vWorld = vRotX * vRotY * vRotZ * vPos;
	m_vPosition = m_vScale * m_vWorld;

	TACTION vActionID = FindActionID(
		m_bAction,
		GetWeaponID(m_bMode));
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

	if(!IsActiveTGate())
	{
		ClearOBJSFX();
		ClearOBJSND();
	}
}

LRESULT CTClientGate::OnActEndMsg()
{
	if( m_bAction != m_pTGATE->m_bOpenID && m_bAction != m_pTGATE->m_bCloseID )
	{
		m_bResetGATE = TRUE;
		return 1;
	}

	return 0;
}

BYTE CTClientGate::IsActiveTGate()
{
	return (m_pTGATE->m_bDeleteOnClose && m_bAction == m_pTGATE->m_bCloseID) ||
		(m_pTGATE->m_bDeleteOnOpen && m_bAction == m_pTGATE->m_bOpenID) ? FALSE : TRUE;
}

void CTClientGate::CalcTick( LPDIRECT3DDEVICE9 pDevice,
							 DWORD dwTick)
{
	if(m_bResetGATE)
		ResetTGate();

	CTClientObjBase::CalcTick(
		pDevice,
		dwTick);
}

void CTClientGate::SetAction( DWORD dwActionID,
							  DWORD dwAniID)
{
	CTClientObjBase::SetAction(
		dwActionID,
		dwAniID);
	m_dwBlendTick = m_dwBlend;
}
