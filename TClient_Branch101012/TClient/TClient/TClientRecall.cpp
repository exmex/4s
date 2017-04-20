#include "StdAfx.h"
#include "TClientGame.h"

D3DXVECTOR2 CTClientRecall::m_vTPOS[TRECALL_MAX] = {
	D3DXVECTOR2(  1.8f, 0.0f),
	D3DXVECTOR2( -1.8f, 0.0f),
	D3DXVECTOR2(  1.8f, 1.8f),
	D3DXVECTOR2(  0.0f, 1.8f),
	D3DXVECTOR2( -1.8f, 1.8f)};


CTClientRecall::CTClientRecall()
{
	m_bCollisionType = TCOLLISION_CYLINDER;
	m_bTPOS = TRECALL_MAX;
	m_bType = OT_RECALL;

	m_bDEAD = FALSE;
	m_bDIE = FALSE;

	m_bSubAI = TRECALLAI_COUNT;
	m_bAI = TRECALLAI_PASSIVE;

	m_pTDEFSKILL = NULL;
	m_pTCURSKILL = NULL;

	m_dwTargetID = 0;
	m_dwHostID = 0;
	
	m_dwEndLifeTick = 0;

	m_bRecallType = TRECALLTYPE_NONE;
	m_bTargetType = OT_NONE;

	m_nRecallRunAwayIndex = 0;
	m_vRecallRunAway.clear();
	m_vRecallRunAwayTarget = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

CTClientRecall::~CTClientRecall()
{
}

void CTClientRecall::ReleaseData()
{
	CTClientMoveObj::ReleaseData();

	m_bContryID = TCONTRY_N;
	m_bTPOS = TRECALL_MAX;
	m_pTEMP = NULL;

	m_bDEAD = FALSE;
	m_bDIE = FALSE;

	m_bSubAI = TRECALLAI_COUNT;
	m_bAI = TRECALLAI_PASSIVE;

	m_pTDEFSKILL = NULL;
	m_pTCURSKILL = NULL;

	m_dwTargetID = 0;
	m_dwHostID = 0;
	
	m_dwEndLifeTick = 0;

	m_bTargetType = OT_NONE;
	m_dwTargetID = 0;

	m_nRecallRunAwayIndex = 0;
	m_vRecallRunAway.clear();
	m_vRecallRunAwayTarget = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

DWORD CTClientRecall::CalcJumpDamage()
{
	if( m_pTEMP && m_pTEMP->m_bCanFly )
		return 0;

	return CTClientMoveObj::CalcJumpDamage();
}

DWORD CTClientRecall::CalcFallDamage()
{
	if( m_pTEMP && m_pTEMP->m_bCanFly )
		return 0;

	return CTClientMoveObj::CalcFallDamage();
}

BYTE CTClientRecall::IsAlliance( CTClientObjBase *pTARGET)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pGame->IsDuel() )
	{
		CTClientObjBase *pHostTarget = NULL;
		if( pTARGET->m_bType == OT_PC )
			pHostTarget = pTARGET;
		else if( pTARGET->m_bType == OT_RECALL || pTARGET->m_bType == OT_SELF )
			pHostTarget = pGame->FindPC(pTARGET->GetHostID());

		if( pHostTarget )
		{
			CTClientChar* pMyHost = pGame->FindPC(GetHostID());
			if( pMyHost )
			{
				CTClientChar* pMainChar = pGame->GetMainChar();
				CTClientChar* pDuelTarg = pGame->GetDuelTarget();
				if( (pMainChar == pMyHost && pDuelTarg == pHostTarget) ||
					(pDuelTarg == pMyHost && pMainChar == pHostTarget))
				{
					return FALSE;
				}
			}
		}
	}

	if( pGame->IsTournamentBattle() || pGame->IsTournamentWatching() )
	{
		CTClientObjBase *pHostTarget = NULL;
		if( pTARGET->m_bType == OT_PC )
			pHostTarget = pTARGET;
		else if( pTARGET->m_bType == OT_RECALL || pTARGET->m_bType == OT_SELF )
			pHostTarget = pGame->FindPC(pTARGET->GetHostID());

		CTClientChar* pMyHost = pGame->FindPC(GetHostID());
		if( pHostTarget && pMyHost )
		{
			if( pMyHost->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
				pHostTarget->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
				pHostTarget->m_bTournamentTeam != pMyHost->m_bTournamentTeam )
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	return CTClientMoveObj::IsAlliance(pTARGET);
}

CString CTClientRecall::GetTitle()
{
	CString strTITLE = m_pTEMP ? m_pTEMP->m_strTITLE : CString("");
	strTITLE.Remove('_');

	return strTITLE;
}

CString CTClientRecall::GetName()
{
	CString strNAME = m_pTEMP ? m_pTEMP->m_strNAME : CString("");
	strNAME.Remove('_');

	return strNAME;
}

void CTClientRecall::InitRecall( CD3DDevice *pDevice,
								 CTachyonRes *pRES,
								 WORD wTempID,
								 BYTE bLevel)
{
	CTClientInven *pTEQUIP = new CTClientInven();
	ClearInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

	LPTMONTEMP pTMON = CTChart::FindTMONTEMP( wTempID );
	if( pTMON )
		m_pTEMP = pTMON;

	if( m_pTEMP && m_pTEMP->m_dwOBJ )
	{
		InitOBJ(pRES->GetOBJ(m_pTEMP->m_dwOBJ));
		
		for( BYTE i=0; i<ES_COUNT; i++)
			if(m_pTEMP->m_pTITEM[i])
			{
				CTClientItem *pTITEM = new CTClientItem();

				pTITEM->SetItemSlot(i);
				pTITEM->SetTITEM(m_pTEMP->m_pTITEM[i]);
				pTITEM->SetCount(1);

				pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			}
		ResetEQUIP( pDevice, pRES);

		m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT) * m_pTEMP->m_fScaleY;
		m_bCanSelected = m_pTEMP->m_bCanSelected;
		m_bHide = !m_pTEMP->m_bVisible;
		m_bDrawName = m_pTEMP->m_bDrawName;

		InitSIZE(
			m_pTEMP->m_fScaleX * GetAttrFLOAT(ID_SIZE_X),
			m_pTEMP->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
			m_pTEMP->m_fScaleZ * GetAttrFLOAT(ID_SIZE_Z),
			0.0f, 0.0f);

		D3DXMatrixScaling(
			&m_vScale,
			m_pTEMP->m_fScaleX,
			m_pTEMP->m_fScaleY,
			m_pTEMP->m_fScaleZ);

		m_vScaleSFX = CTMath::Inverse(&m_vScale);
		m_vPosition = m_vScale * m_vWorld;

		for( i=0; i<TMONSKILL_COUNT; i++)
		{
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(m_pTEMP->m_wTSKILL[i]);

			if(pTSKILL)
			{
				CTClientSkill *pTSkill = new CTClientSkill();

				pTSkill->m_pTSKILL = pTSKILL;
				pTSkill->m_dwTick = 0;
				pTSkill->m_bLevel = min( bLevel, pTSKILL->m_bMaxLevel);

				m_mapTSKILL.insert( MAPTSKILL::value_type( m_pTEMP->m_wTSKILL[i], pTSkill));
			}
		}

		m_pTCURSKILL = FindTSkill(m_pTEMP->m_wTSKILL[TMONSKILL_DEFAULT]);
	}

	m_vTSKILLDATA.m_bAtkCountryID = m_bContryID;
	m_vTSKILLDATA.m_bAglow = m_bCanSelected;

	switch(m_bRecallType)
	{
	case TRECALLTYPE_MAIN		: m_bAI = TRECALLAI_PASSIVE; break;
	case TRECALLTYPE_MINE		:
	case TRECALLTYPE_AUTOAI		: m_bAI = TRECALLAI_ACTIVE; break;
	case TRECALLTYPE_PET		: m_bCanDefend = FALSE;
	case TRECALLTYPE_MAINTAIN	:
	case TRECALLTYPE_SKILL		:
	case TRECALLTYPE_SPY		: m_bAI = TRECALLAI_MANUAL; break;
	}
}

BYTE CTClientRecall::GetTAction()
{
	return CTClientMoveObj::GetTAction();
}

CTClientSkill *CTClientRecall::GetBestTSKILL( CTClientObjBase *pTARGET)
{
	FLOAT fDIST = pTARGET ? D3DXVec3LengthSq(&(pTARGET->GetPosition() - GetPosition())) : TCAM_LENGTH * TCAM_LENGTH;
	FLOAT fMAX = 0.0f;

	CTClientSkill *pTRESULT = NULL;
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_mapTSKILL.begin(); itTSKILL != m_mapTSKILL.end(); itTSKILL++)
		if((*itTSKILL).second->m_pTSKILL->m_bLoop)
		{
			FLOAT fLOCAL = GetMinRange(
				pTARGET,
				(*itTSKILL).second->m_pTSKILL);

			if( fLOCAL = 0.0f || fDIST > fLOCAL * fLOCAL )
			{
				fLOCAL = TMAXRANGE_RATIO * GetMaxRange(
					pTARGET,
					(*itTSKILL).second->m_pTSKILL);

				if( !pTRESULT || fMAX < fLOCAL )
				{
					pTRESULT = (*itTSKILL).second;
					fMAX = fLOCAL;
				}
			}
		}

	if(!pTRESULT)
		pTRESULT = m_pTCURSKILL;

	return pTRESULT;
}

D3DXVECTOR3 CTClientRecall::GetRoamTarget( LPD3DXMATRIX pDIR,
										   FLOAT fPosX,
										   FLOAT fPosY,
										   FLOAT fPosZ)
{
	D3DXVECTOR3 vRESULT;

	if( m_bType != OT_SELF )
	{
		vRESULT = m_bTPOS < TRECALL_MAX ? D3DXVECTOR3(
			m_vTPOS[m_bTPOS].x,
			0.0f,
			m_vTPOS[m_bTPOS].y) : D3DXVECTOR3(
			0.0f,
			0.0f,
			0.0f);

		CTMath::Transform(
			pDIR,
			&vRESULT);

		vRESULT.x += fPosX;
		vRESULT.y += fPosY;
		vRESULT.z += fPosZ;
	}
	else
	{
		vRESULT.x = m_vPosition._41;
		vRESULT.y = m_vPosition._42;
		vRESULT.z = m_vPosition._43;
	}

	return vRESULT;
}

BYTE CTClientRecall::GetRoamACT( LPD3DXVECTOR3 pTARGET)
{
	if( m_bType != OT_SELF )
	{
		FLOAT fDIST = D3DXVec2LengthSq(&D3DXVECTOR2(
			pTARGET->x - m_vPosition._41,
			pTARGET->z - m_vPosition._43));
		FLOAT fSB = TROAM_BOUND;

		if( m_bSubAI == TRECALLAI_STAY || fDIST < fSB * fSB )
			return TA_STAND;

		if( fDIST < TRECALL_WALK_BOUND * TRECALL_WALK_BOUND )
			return m_bAction == TA_STAND || m_bAction == TA_WALK ? TA_WALK : TA_RUN;
	}
	else
		return TA_STAND;

	return TA_RUN;
}

FLOAT CTClientRecall::GetLOST( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_pTEMP ? m_pTEMP->m_fLOST + m_fRadius : 0.0f;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

FLOAT CTClientRecall::GetAB( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_pTEMP ? m_pTEMP->m_fAB + m_fRadius : 0.0f;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

FLOAT CTClientRecall::GetLB( CTClientObjBase *pTARGET)
{
	FLOAT fRESULT = m_pTEMP ? m_pTEMP->m_fLB + m_fRadius : 0.0f;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

DWORD CTClientRecall::GetHostID()
{
	return m_dwHostID;
}

DWORD CTClientRecall::GetLeftLifeTick()
{
	DWORD dwCurTick = ::GetTickCount();
	if( m_dwEndLifeTick <= dwCurTick )
		return 0;

	return m_dwEndLifeTick - dwCurTick;
}

void CTClientRecall::DoRETRANS( CD3DDevice *pDevice,
							    CTachyonRes *pRES)
{
	CTClientMoveObj::DoRETRANS( pDevice, pRES);

	InitOBJ(pRES->GetOBJ(m_pTEMP->m_dwOBJ));
	ResetEQUIP( pDevice, pRES);

	m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_bCanSelected = m_pTEMP->m_bCanSelected;

	InitSIZE(
		m_pTEMP->m_fScaleX * GetAttrFLOAT(ID_SIZE_X),
		m_pTEMP->m_fScaleY * GetAttrFLOAT(ID_SIZE_Y),
		m_pTEMP->m_fScaleZ * GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	ResetRootID(ID_PIVOT_WAIST);
}

LRESULT CTClientRecall::OnActEndMsg()
{
	if(IsDead())
		m_bDEAD = TRUE;

	return CTClientMoveObj::OnActEndMsg();
}

void CTClientRecall::OnShotSkill()
{
	if( m_bRecallType == TRECALLTYPE_MINE )
		m_bDIE = TRUE;
}

BYTE CTClientRecall::GetDrawName()
{
	return CTClientObjBase::GetDrawName() && CTClientGame::GetMonNAMEOption() ? TRUE : FALSE;
}

BYTE CTClientRecall::CanDIVE()
{
	return m_pTEMP && m_pTEMP->m_bCanFly ? FALSE : TRUE;
}

BYTE CTClientRecall::CheckFall( CTClientMAP *pMAP,
							    LPD3DXVECTOR2 pFallDIR)
{
	return FALSE;
}

BYTE CTClientRecall::Fall( LPD3DXVECTOR2 pFallDIR)
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
