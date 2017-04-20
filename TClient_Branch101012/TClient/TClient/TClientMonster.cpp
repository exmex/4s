#include "StdAfx.h"
#include "TClientGame.h"

CTClientMonster::CTClientMonster()
{
	m_bCollisionType = TCOLLISION_CYLINDER;
	m_pTDEFSKILL = NULL;
	m_bType = OT_MON;
	m_fAB = 0.0f;

	m_fSpeedFactor = TMON_SPEED;
	m_fTDEFAULTSPEED = TMON_SPEED;

	m_vTGOAL = D3DXVECTOR3(
		0.0f,
		0.0f,
		0.0f);

	m_bGoHome = FALSE;
	m_bUPDATE = FALSE;
	m_bPATH = FALSE;
	m_bBACK = FALSE;
	m_bHostMainChar = FALSE;
}

CTClientMonster::~CTClientMonster()
{
}

void CTClientMonster::ReleaseData()
{
	CTClientMoveObj::ReleaseData();

	m_fSpeedFactor = TMON_SPEED;
	m_fTDEFAULTSPEED = TMON_SPEED;

	m_bContryID = TCONTRY_N;
	m_bTD = TD_NONE;
	m_fAB = 0.0f;
	m_pTDEFSKILL = NULL;

	m_vTGOAL = D3DXVECTOR3(
		0.0f,
		0.0f,
		0.0f);

	m_bGoHome = FALSE;
	m_bUPDATE = FALSE;
	m_bPATH = FALSE;

	m_bFollowType = OT_NONE;
	m_dwFollowID = 0;
	m_pMON = NULL;
	m_bBACK = FALSE;
}

CString CTClientMonster::GetTitle()
{
	CString strTITLE = m_pMON ? m_pMON->m_strTITLE : CString("");
	strTITLE.Remove('_');

	return strTITLE;
}

CString CTClientMonster::GetName()
{
	CString strNAME = m_pMON ? m_pMON->m_strNAME : CString("");
	strNAME.Remove('_');

	return strNAME;
}

void CTClientMonster::InitMonster( CD3DDevice *pDevice,
								   CTachyonRes *pRES,
								   WORD wTempID)
{
	m_pMON = CTChart::FindTMONTEMP( wTempID );

	if(m_pMON)
	{
		CTClientInven *pTEQUIP = new CTClientInven();

		InitOBJ(pRES->GetOBJ(m_pMON->m_dwOBJ));
		ClearInven();

		pTEQUIP->m_bInvenID = INVEN_EQUIP;
		pTEQUIP->m_wItemID = 0;
		m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

		for( BYTE i=0; i<ES_COUNT; i++)
			if(m_pMON->m_pTITEM[i])
			{
				CTClientItem *pTITEM = new CTClientItem();

				pTITEM->SetItemSlot(i);
				pTITEM->SetTITEM(m_pMON->m_pTITEM[i]);
				pTITEM->SetCount(1);

				pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );

			}

		ResetEQUIP( pDevice, pRES);

		m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT) * m_pMON->m_fScaleY;
		m_bCanSelected = m_pMON->m_bCanSelected;
		m_bDrawName = m_pMON->m_bDrawName;

		FLOAT fSize = m_pMON->m_fSize;

		InitSIZE(
			fSize,
			m_pMON->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
			fSize,
			0.0f, 0.0f);

		m_fAB = m_pMON->m_fAB;

		D3DXMatrixScaling(
			&m_vScale,
			m_pMON->m_fScaleX,
			m_pMON->m_fScaleY,
			m_pMON->m_fScaleZ);

		m_vScaleSFX = CTMath::Inverse(&m_vScale);
		m_vPosition = m_vScale * m_vWorld;

		m_vTGOAL.x = m_vPosition._41;
		m_vTGOAL.y = m_vPosition._42;
		m_vTGOAL.z = m_vPosition._43;

		// 은심감지 기능 On/Off (일단 PC만)
		if( m_pMON->m_bCanDetectHidingPC)
			m_wTDETECT |= (1 << OT_PC);
	}
}

FLOAT CTClientMonster::GetLOST( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_pMON ? m_pMON->m_fLOST + m_fRadius : 0.0f;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

FLOAT CTClientMonster::GetAB( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_fAB + m_fRadius;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

FLOAT CTClientMonster::GetLB( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_pMON ? m_pMON->m_fLB + m_fRadius : 0.0f;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

BYTE CTClientMonster::GetTAction()
{
	return !IsDead() && m_bFollowType != OT_NONE && !m_bIsHolded && !m_bIsStuned ? TA_FOLLOW : CTClientMoveObj::GetTAction();
}

BYTE CTClientMonster::IsInBattle()
{
	return TRUE;
}

void CTClientMonster::DoTRANS(
					 CD3DDevice *pDevice,
					 CTachyonRes *pRES,
					 WORD wMonID)
{
	CTClientMoveObj::DoTRANS(
		pDevice,
		pRES,
		wMonID);

	m_strPreTITLE = m_pMON->m_strTITLE;
	m_strPreNAME = m_pMON->m_strNAME;

	if( m_pTRANS )
	{
		m_pMON->m_strTITLE = m_pTRANS->m_strTITLE;
		m_pMON->m_strNAME = m_pTRANS->m_strNAME;
	}
}

void CTClientMonster::DoRETRANS( CD3DDevice *pDevice,
								 CTachyonRes *pRES)
{
	CTClientMoveObj::DoRETRANS( pDevice, pRES);

	InitOBJ(pRES->GetOBJ(m_pMON->m_dwOBJ));
	ResetEQUIP( pDevice, pRES);

	m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_bCanSelected = m_pMON->m_bCanSelected;

	InitSIZE(
		m_pMON->m_fScaleX * GetAttrFLOAT(ID_SIZE_X),
		m_pMON->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
		m_pMON->m_fScaleZ * GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	if( m_pMON )
	{
		m_pMON->m_strTITLE = m_strPreTITLE;
		m_pMON->m_strNAME = m_strPreNAME;
	}

	ResetRootID(ID_PIVOT_WAIST);
}

BYTE CTClientMonster::GetDrawName()
{
	return CTClientObjBase::GetDrawName() && CTClientGame::GetMonNAMEOption() ? TRUE : FALSE;
}

void CTClientMonster::OnMaintainEnd( CD3DDevice *pDevice,
									 CTachyonRes *pRES,
									 CTClientSkill *pTSKILL)
{
	BYTE bBLOCK = IsBlock();

	CTClientMoveObj::OnMaintainEnd(
		pDevice,
		pRES,
		pTSKILL);

	if( bBLOCK && !IsBlock() )
		m_bTD = TD_NONE;
}

BYTE CTClientMonster::CanDIVE()
{
	return m_pMON && m_pMON->m_bCanFly ? FALSE : TRUE;
}

BYTE CTClientMonster::CheckFall( CTClientMAP *pMAP,
								 LPD3DXVECTOR2 pFallDIR)
{
	return FALSE;
}

BYTE CTClientMonster::Fall( LPD3DXVECTOR2 pFallDIR)
{
	if( IsRide() || IsFall() )
		return FALSE;

	if( CTChart::GetTACTION(TA_FALL)->m_bLevel >= m_bACTLevel )
	{
		TACTION vActionID = FindActionID(
			TA_STAND,
			GetWeaponID(m_bMode));
		SetTAction(TA_FALL);

		m_bACTLevel = CTChart::GetTACTION( TA_FALL )->m_bLevel;
		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	}

	if( pFallDIR->x != 0.0f || pFallDIR->y != 0.0f )
		(*pFallDIR) /= D3DXVec2Length(pFallDIR);

	m_fFallHeight = m_vPosition._42;
	m_dwFallTick = 0;
	m_vFallDIR = max( TMINFALL_SPEED, m_fMoveSpeed) * (*pFallDIR);

	return TRUE;
}

BYTE CTClientMonster::IsDrawNameWhenDead()
{
	return m_pMON->m_bDrawNameWhenDie;
}

BYTE CTClientMonster::IsSlidingWhenLastHit()
{
	return m_pMON->m_bSlidingWhenDie;
}