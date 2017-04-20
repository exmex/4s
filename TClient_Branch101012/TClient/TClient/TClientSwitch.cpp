#include "stdafx.h"

CTClientSwitch::CTClientSwitch()
{
	m_pTSWITCH = NULL;

	m_bCanSelected = TRUE;
	m_bDynamicOBJ = FALSE;
	m_bDrawName = FALSE;
	m_bOPEN = FALSE;

	m_bType = OT_SWITCH;
}

CTClientSwitch::~CTClientSwitch()
{
}

void CTClientSwitch::Release()
{
	CTClientObjBase::Release();
	m_pTSWITCH = NULL;
	m_bOPEN = FALSE;
}

void CTClientSwitch::ReleaseData()
{
	CTClientObjBase::ReleaseData();
	m_pTSWITCH = NULL;
	m_bOPEN = FALSE;
}

void CTClientSwitch::InitTSWITCH( CD3DDevice *pDevice, CTachyonRes *pRES)
{
	MAPRES::iterator itOBJ = pRES->m_mapOBJ.find(m_pTSWITCH->m_dwOBJ);
	CTClientInven *pTEQUIP = new CTClientInven();
	ClearInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

	InitOBJ((LPOBJECT) (*itOBJ).second);
	
	for( BYTE i=0; i<ES_COUNT; i++)
		if(m_pTSWITCH->m_pTITEM[i])
		{
			CTClientItem *pTITEM = new CTClientItem();

			pTITEM->SetItemSlot(i);
			pTITEM->SetTITEM(m_pTSWITCH->m_pTITEM[i]);
			pTITEM->SetCount(1);

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}

	ResetEQUIP( pDevice, pRES);

	TACTION vActionID = FindActionID(
		m_bOPEN ? m_pTSWITCH->m_bOpenID : m_pTSWITCH->m_bCloseID,
		GetWeaponID(m_bMode));
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

	m_bCollisionType = GetAttrINT(ID_COLLISION_TYPE);
	InitSIZE(
		m_pTSWITCH->m_fScaleX * GetAttrFLOAT(ID_SIZE_X),
		m_pTSWITCH->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
		m_pTSWITCH->m_fScaleZ * GetAttrFLOAT(ID_SIZE_Z),
		m_pTSWITCH->m_fScaleY * GetAttrFLOAT(ID_DEPTH),
		max( m_pTSWITCH->m_fScaleX, max(
		m_pTSWITCH->m_fScaleY,
		m_pTSWITCH->m_fScaleZ)) * GetAttrFLOAT(ID_SIGHT));

	D3DXMatrixRotationY(
		&m_vWorld,
		m_pTSWITCH->m_fDIR);

	D3DXMatrixScaling(
		&m_vScale,
		m_pTSWITCH->m_fScaleX,
		m_pTSWITCH->m_fScaleY,
		m_pTSWITCH->m_fScaleZ);

	m_vWorld._41 = m_pTSWITCH->m_fPosX;
	m_vWorld._42 = m_pTSWITCH->m_fPosY;
	m_vWorld._43 = m_pTSWITCH->m_fPosZ;
	m_vPosition = m_vScale * m_vWorld;

	m_bHouseMesh = m_pTSWITCH->m_bHouseMesh;
	m_dwHouseID = m_pTSWITCH->m_dwHouseID;
	m_dwID = m_pTSWITCH->m_dwID;
}

void CTClientSwitch::TurnON( BYTE bOPEN)
{
	TACTION vActionID = FindActionID(
		bOPEN ? m_pTSWITCH->m_bOpenID : m_pTSWITCH->m_bCloseID,
		GetWeaponID(m_bMode));

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	m_bOPEN = bOPEN;
}
