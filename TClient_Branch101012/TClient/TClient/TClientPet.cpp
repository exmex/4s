#include "Stdafx.h"
#include "TClientPet.h"
#include "TClientGame.h"

// =======================================================
CTClientPet::CTClientPet()
	: CTClientRecall(), m_wPetID(0), m_pPetTemp(NULL), m_pTakeUpChar(NULL)
{
	m_bCanSelected = FALSE;
}
// -------------------------------------------------------
CTClientPet::~CTClientPet()
{
	if( m_dwTakeUpPivot )
	{
		MAPOBJECT::iterator itr = m_mapEQUIP.find(m_dwTakeUpPivot);
		if( itr != m_mapEQUIP.end() )
			m_mapEQUIP.erase(itr);
	}

	if( m_pTakeUpChar )
		m_pTakeUpChar->SetRidingPet(NULL);
}
// =======================================================
void CTClientPet::SetPetInfo(const CString& strName, WORD wPetID, const CTime& tEndTime)
{
	m_strPetName = strName;

	m_wPetID = wPetID;
	m_pPetTemp = CTChart::FindTPETTEMP( wPetID );
	m_tPetEndTime = tEndTime;

	m_fBaseSpeedFactor = TDEF_SPEED;

	if( m_pPetTemp->m_bRecallKind1 == PETKIND_ONE ||
		m_pPetTemp->m_bRecallKind1 == PETKIND_TWO )
	{
		FLOAT fFac = 1.0f + ( FLOAT(m_pPetTemp->m_wRecallValue1) / 100.0f );
		m_fBaseSpeedFactor *= fFac;
	}

	if( m_pPetTemp->m_bRecallKind2 == PETKIND_ONE ||
		m_pPetTemp->m_bRecallKind2 == PETKIND_TWO )
	{
		FLOAT fFac = 1.0f + ( FLOAT(m_pPetTemp->m_wRecallValue2) / 100.0f );
		m_fBaseSpeedFactor *= fFac;
	}

	m_fTDEFAULTSPEED = m_fBaseSpeedFactor;
	m_pTEMP = CTChart::FindTMONTEMP( m_pPetTemp->m_wMonID );
}
// =======================================================
BOOL CTClientPet::TakeUp( CD3DDevice *pDevice,
						  CTachyonRes *pRES,
						  CTClientChar *pChar,
						  DWORD dwPivot)
{
	if( m_pTakeUpChar )
		return FALSE;

	MAPDWORD::iterator itr = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwPivot);
	if( itr == m_OBJ.m_pOBJ->m_mapPIVOT.end() )
		return FALSE;

	TACTION act = pChar->FindActionID(
		TA_RIDING,
		pChar->GetWeaponID(pChar->m_bMode));
	BYTE bHideOnCapeMode = CTChart::GetTACTION(TA_RIDING)->m_bHideOnCapeMode;

	if( pChar->m_bHideOnCapeMode != bHideOnCapeMode ||
		pChar->m_bEquipMode != act.m_bEquipMode )
	{
		pChar->m_bHideOnCapeMode = bHideOnCapeMode;
		pChar->m_bEquipMode = act.m_bEquipMode;

		pChar->ResetEQUIP(
			pDevice,
			pRES);
	}

	pChar->m_bAction = TA_RIDING;
	pChar->SetAction(
		act.m_dwActID,
		act.m_dwAniID);

	m_dwTakeUpPivot = itr->second + 1;
	pChar->SetRidingPet(this);
	m_nDIR = INT(pChar->m_wDIR) - INT(m_wDIR);

	m_mapEQUIP.insert( std::make_pair(m_dwTakeUpPivot,pChar) );
	D3DXMatrixIdentity(&pChar->m_vWorld);
	m_pTakeUpChar = pChar;

	m_bMouseDIR = m_pTakeUpChar->m_bMouseDIR;
	m_bKeyDIR = TKDIR_N; //m_pTakeUpChar->m_bKeyDIR;
	
	m_pTakeUpChar->m_vPushDIR = D3DXVECTOR2(0,0);
	m_pTakeUpChar->m_vJumpDIR = D3DXVECTOR2(0,0);
	m_pTakeUpChar->m_vFallDIR = D3DXVECTOR2(0,0);

	m_pTakeUpChar->m_dwPushTick = 0;
	m_pTakeUpChar->m_dwJumpTick = 0;
	m_pTakeUpChar->m_dwFallTick = 0;

	m_bCanSelected = TRUE;

	return TRUE;
}
// -------------------------------------------------------
CTClientChar* CTClientPet::TakeDown()
{
	if( !m_pTakeUpChar )
		return NULL;

	CTClientChar* pRet = m_pTakeUpChar;

	MAPOBJECT::iterator itr = m_mapEQUIP.find(m_dwTakeUpPivot);
	if( itr != m_mapEQUIP.end() )
		m_mapEQUIP.erase(itr);

	m_pTakeUpChar->m_wDIR = m_wDIR;
	m_pTakeUpChar->m_wMoveDIR = m_wMoveDIR;
	m_pTakeUpChar->m_wPITCH = m_wPITCH;
	m_pTakeUpChar->m_nDIR = m_nDIR;
	m_pTakeUpChar->m_nPITCH = m_nPITCH;
	m_pTakeUpChar->m_bMouseDIR = m_bMouseDIR;
	m_pTakeUpChar->m_bKeyDIR = m_bKeyDIR;
	m_pTakeUpChar->m_vWorld = m_vWorld;
	m_pTakeUpChar->SetPosition(m_vPosition);

	m_vPushDIR = D3DXVECTOR2(0,0);
	m_vJumpDIR = D3DXVECTOR2(0,0);
	m_vFallDIR = D3DXVECTOR2(0,0);
	m_dwPushTick = 0;
	m_dwJumpTick = 0;
	m_dwFallTick = 0;
	m_nPITCH = 0;
	m_nDIR = 0;
	m_bMouseDIR = TKDIR_N;
	m_bKeyDIR = TKDIR_N;

	m_dwTakeUpPivot = 0;
	m_pTakeUpChar->SetRidingPet(NULL);
	m_pTakeUpChar = NULL;
	m_bDrawTalk = FALSE;
	m_bCanSelected = FALSE;
	
	SetTAction(TA_STAND);

	return pRet;
}
// =======================================================

// =======================================================
BYTE CTClientPet::GetDrawName()
{
	if( !CTClientObjBase::GetDrawName() ||
		!CTClientGame::GetPcNAMEOption() /*||
		m_pTakeUpChar == CTClientGame::GetInstance()->GetMainChar()*/ )
	{
		return FALSE;
	}

	return TRUE;
}
// -------------------------------------------------------
CString CTClientPet::GetTitle()
{
	if( m_pTakeUpChar )
		return m_pTakeUpChar->GetTitle();
	else
		return CString("");
}
// -------------------------------------------------------
CString CTClientPet::GetName()
{
	if( m_pTakeUpChar )
		return m_pTakeUpChar->GetName();
	else
		return m_strPetName;
}
CString CTClientPet::GetUserTitle()
{
	if( m_pTakeUpChar )
		return m_pTakeUpChar->GetUserTitle();
	else
		return CString("");
}
void CTClientPet::CalcFrame(BOOL bUpdate)
{
    if( m_pTakeUpChar ) 
		m_pTakeUpChar->CalcFrame( bUpdate );

	CTClientRecall::CalcFrame( bUpdate );
}
// =======================================================
void CTClientPet::CalcHeight(LPD3DXVECTOR3 pPREV, CTClientMAP *pMAP, DWORD dwTick)
{
	CTClientRecall::CalcHeight(pPREV,pMAP,dwTick);

	if( m_pTakeUpChar )
	{
		m_pTakeUpChar->m_dwDropDamage = m_dwDropDamage;
	}
	else
	{
		if( m_bAction == TA_DEAD )
		{
			m_bAction = TA_DIE;
			m_bACTLevel = CTChart::GetTACTION( TA_DIE )->m_bLevel;
		}
	}

	m_dwDropDamage = 0;
}
// -------------------------------------------------------
void CTClientPet::Render(CD3DDevice *pDevice, CD3DCamera *pCamera)
{
	if( m_pTakeUpChar )
		 m_fSpeedFactor = GetSpeedWhenRiding();
	else
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		CTClientChar* pMyHost = pGame->FindPC(GetHostID());
		if( !pMyHost )
			m_fSpeedFactor = TDEF_SPEED;
		else
			m_fSpeedFactor = pMyHost->m_fSpeedFactor;
	}

	CTClientRecall::Render(pDevice,pCamera);
}
// =======================================================
FLOAT CTClientPet::GetSpeedWhenRiding()
{
	if( m_fBaseSpeedFactor < m_pTakeUpChar->m_fSpeedFactor )
		return m_pTakeUpChar->m_fSpeedFactor;
	else
		return m_fBaseSpeedFactor;

	// 기존코드 ( 캐릭터속도 + 말속도 )
	return m_fBaseSpeedFactor * (m_pTakeUpChar->m_fSpeedFactor / m_pTakeUpChar->m_fTDEFAULTSPEED);
}

void CTClientPet::ShowSFX()
{
	if( m_pTakeUpChar )
		m_pTakeUpChar->ShowSFX();
	CTClientRecall::ShowSFX();
}

void CTClientPet::HideSFX()
{
	if( m_pTakeUpChar )
		m_pTakeUpChar->HideSFX();
	CTClientRecall::HideSFX();
}

BYTE CTClientPet::CheckFall( CTClientMAP *pMAP,
							 LPD3DXVECTOR2 pFallDIR)
{
	return m_pTakeUpChar ? CTClientMoveObj::CheckFall( pMAP, pFallDIR) : CTClientRecall::CheckFall( pMAP, pFallDIR);
}
