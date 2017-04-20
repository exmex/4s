#include "stdafx.h"
#include "TClientGame.h"
#include "TFameRank.h"

CTClientChar::CTClientChar()
{
	m_bCollisionType = TCOLLISION_CYLINDER;
	m_bType = OT_PC;
	m_bAfterMath = 0;
	m_bPrivateShop = FALSE;
	m_pRidingPet = NULL;
	m_pGuildMark = NULL;

	m_bEnterCastle = FALSE;
    m_wCastleID = 0;
	m_wBallID = 0;
	m_bCamp = 0;

	m_bUseFameTitle = FALSE;
	m_strFameTitle.Empty();

	for( BYTE i=0; i<TPOINT_COUNT; i++)
		m_vTSKILLPOINT[i] = 0;

	ZeroMemory( &m_StatInfo, sizeof(TSTATINFO) );	

	m_zEncryptKEY = rand();
}

CTClientChar::~CTClientChar()
{
	if( m_pGuildMark )
		delete m_pGuildMark;
}

void CTClientChar::ReleaseData()
{
	CTClientMoveObj::ReleaseData();

	for( BYTE i=0; i<TRECALL_MAX; i++)
		m_vTRECALL[i] = FALSE;

	for( i=0; i<TPOINT_COUNT; i++)
		m_vTSKILLPOINT[i] = 0;

	m_strGUILD.Empty();
	m_strNAME.Empty();

	m_dwGuildID = 0;
	m_dwTacticsID = 0;
	m_dwChiefID = 0;

	m_wCommanderID = 0;
	m_wPartyID = 0;

	m_dwRune = 0;
	m_dwLuna = 0;
	m_dwCron = 0;

	m_dwPrevEXP = 0;
	m_dwNextEXP = 0;
	//m_dwEXP = 0;
	m_dwEncryptEXP = 0;

	m_bRaceID_ = 0;
	m_bSex = 0;
	m_dwMantleCond = 0;
	m_bAfterMath = 0;

	m_bHair = 0;
	m_bFace = 0;
	m_bBody = 0;
	m_bPants = 0;
	m_bHand = 0;
	m_bFoot = 0;

	m_bPrevMainRecallAI = TRECALLAI_PASSIVE;
	ZeroMemory( &m_StatInfo, sizeof(TSTATINFO) );

	m_dwPvPTotalPoint = 0;
	m_dwPvPUseablePoint = 0;
}

CString CTClientChar::GetName()
{
	//!! 임시
//	if( m_pSiegeBall )
//		return m_strNAME + " 0";
//	else
//		return m_strNAME;

	return m_strNAME;
}

void CTClientChar::ResetDEFEQUIP( CD3DDevice *pDevice,
								  BYTE bEquipPOS)
{
	if(!m_pTRANS ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_HUMAN ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WHUMAN ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_BEAST ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WBEAST ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_FAIRY ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WFAIRY )
	{
		DWORD dwCLK = 0;
		DWORD dwCL = 0;
		DWORD dwMESH = 0;

		switch(bEquipPOS)
		{
		case ES_FACE	:
			{
				dwCLK = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCLK;
				dwCL = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCL;
				dwMESH = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwMESH;
			}

			break;

		case ES_BODY	:
			{
				dwCLK = CTChart::m_vTBODY[m_bBody].m_dwCLK;
				dwCL = CTChart::m_vTBODY[m_bBody].m_dwCL;
				dwMESH = CTChart::m_vTBODY[m_bBody].m_dwMESH;
			}

			break;

		case ES_PANTS	:
			{
				dwCLK = CTChart::m_vTPANTS[m_bPants].m_dwCLK;
				dwCL = CTChart::m_vTPANTS[m_bPants].m_dwCL;
				dwMESH = CTChart::m_vTPANTS[m_bPants].m_dwMESH;
			}

			break;

		case ES_FOOT	:
			{
				dwCLK = CTChart::m_vTFOOT[m_bFoot].m_dwCLK;
				dwCL = CTChart::m_vTFOOT[m_bFoot].m_dwCL;
				dwMESH = CTChart::m_vTFOOT[m_bFoot].m_dwMESH;
			}

			break;

		case ES_HAND	:
			{
				dwCLK = CTChart::m_vTHAND[m_bHand].m_dwCLK;
				dwCL = CTChart::m_vTHAND[m_bHand].m_dwCL;
				dwMESH = CTChart::m_vTHAND[m_bHand].m_dwMESH;
			}

			break;

		case ES_BACK:
			{
				dwCLK = ID_CLK_MANTLE;
				dwCL = 0;
				dwMESH = 0;
			}
			break;
		}

		if(dwCLK)
			SetCloth( pDevice, dwCLK, dwCL, dwMESH);
	}
	else
		CTClientMoveObj::ResetDEFEQUIP( pDevice, bEquipPOS);
}

void CTClientChar::ClearDEFEQUIP( CD3DDevice *pDevice,
								  BYTE bEquipPOS)
{
	if(!m_pTRANS ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_HUMAN ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WHUMAN ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_BEAST ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WBEAST ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_FAIRY ||
		m_pTRANS->m_dwOBJ == ID_PLAYER_WFAIRY )
	{
		DWORD dwCLK = 0;

		switch(bEquipPOS)
		{
		case ES_FACE: dwCLK = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCLK; break;
		case ES_BODY: dwCLK = CTChart::m_vTBODY[m_bBody].m_dwCLK; break;
		case ES_PANTS: dwCLK = CTChart::m_vTPANTS[m_bPants].m_dwCLK; break;
		case ES_FOOT: dwCLK = CTChart::m_vTFOOT[m_bFoot].m_dwCLK; break;
		case ES_HAND: dwCLK = CTChart::m_vTHAND[m_bHand].m_dwCLK; break;
		case ES_BACK: dwCLK = ID_CLK_MANTLE; break;
		}

		if(dwCLK)
			SetCloth( pDevice, dwCLK, 0, 0);
	}
}

THIDEPART CTClientChar::FindHidePart( CTClientItem *pTITEM,
									  BYTE bSlotID)
{
	if( m_pTRANS || pTITEM )
		return CTClientMoveObj::FindHidePart( pTITEM, bSlotID);

	THIDEPART vRESULT = { 0xFF, 0xFF, 0};
	switch(bSlotID)
	{
	case ES_FACE	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHideRaceID;
		}

		break;

	case ES_BODY	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTBODY[m_bBody].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTBODY[m_bBody].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTBODY[m_bBody].m_bHideRaceID;
		}

		break;

	case ES_PANTS	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTPANTS[m_bPants].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTPANTS[m_bPants].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTPANTS[m_bPants].m_bHideRaceID;
		}

		break;

	case ES_FOOT	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTFOOT[m_bFoot].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTFOOT[m_bFoot].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTFOOT[m_bFoot].m_bHideRaceID;
		}

		break;

	case ES_HAND	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTHAND[m_bHand].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTHAND[m_bHand].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTHAND[m_bHand].m_bHideRaceID;
		}

		break;
	}

	return vRESULT;
}

BYTE CTClientChar::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	static BYTE bRaceID[TMAINRACE_COUNT][TSEX_COUNT] = {
		{  1,  2},
		{  4,  8},
		{ 16, 32}};

	if(m_pTRANS)
		return CTClientMoveObj::CheckOBJPart(pTHIDE);

	return CTClientMoveObj::CheckOBJPart(pTHIDE) && (bRaceID[GetRaceID()][m_bSex] & pTHIDE->m_bHideRaceID) ? TRUE : FALSE;
}

void CTClientChar::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
{
	if(!m_pTRANS)
	{
		DWORD dwCLK = 0;

		switch(pTHIDE->m_bHideSlotID)
		{
		case ES_FACE	: dwCLK = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCLK; break;
		case ES_BODY	: dwCLK = CTChart::m_vTBODY[m_bBody].m_dwCLK; break;
		case ES_PANTS	: dwCLK = CTChart::m_vTPANTS[m_bPants].m_dwCLK; break;
		case ES_FOOT	: dwCLK = CTChart::m_vTFOOT[m_bFoot].m_dwCLK; break;
		case ES_HAND	: dwCLK = CTChart::m_vTHAND[m_bHand].m_dwCLK; break;
		}

		MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(dwCLK);
		if( itCI != m_OBJ.m_mapCLK.end() )
			(*itCI).second->m_dwHideID = pTHIDE->m_bHidePartID;
	}
	else
		CTClientMoveObj::ResetDEFOBJPart(pTHIDE);
}

void CTClientChar::ResetWeaponPOS(
	DWORD dwHideEquipSlot,
	CD3DDevice *pDevice,
	CTachyonRes *pRES)
{
	CTClientMoveObj::ResetWeaponPOS( dwHideEquipSlot, pDevice, pRES);
	ApplyMantleTexture();
}

void CTClientChar::SetRaceID(BYTE bRace)
{
	m_bRaceID_ = bRace;
}

BYTE CTClientChar::GetRaceID()
{
	BYTE bCostumeRaceID = GetCostumeRaceID();
	if( bCostumeRaceID != (BYTE)(-1) )
		return bCostumeRaceID;

	return m_bRaceID_;
}

BYTE CTClientChar::GetCostumeRaceID()
{
	MAPTINVEN::iterator itTINVEN = m_mapTINVEN.find(INVEN_EQUIP);
	if( itTINVEN != m_mapTINVEN.end() )
	{
		if( (*itTINVEN).second )
		{
			MAPTITEM::iterator it = (*itTINVEN).second->m_mapTITEM.find( ES_COSTUME_RACE );
			if( it != (*itTINVEN).second->m_mapTITEM.end() )
			{
				if( it->second && it->second->GetTITEM() )
					return (BYTE) it->second->GetTITEM()->m_wUseValue;
			}
		}
	}

	return (BYTE)(-1);
}

BYTE CTClientChar::GetFreeRecallPOS()
{
	for( BYTE i=0; i<TRECALL_MAX; i++)
		if(!m_vTRECALL[i])
			return i;

	return TRECALL_MAX;
}

CTClientPet* CTClientChar::GetRidingPet() const
{
	return m_pRidingPet;
}

void CTClientChar::SetRidingPet(CTClientPet* pTPET)
{
	m_pRidingPet = pTPET;
	m_bDrawTalk = FALSE;
}

void CTClientChar::DoTRANS(
	CD3DDevice *pDevice,
	CTachyonRes *pRES,
	WORD wMonID)
{
	if( m_bDoubleHead )
		SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

	CTClientMoveObj::DoTRANS(
		pDevice,
		pRES,
		wMonID);
}

void CTClientChar::DoRETRANS( CD3DDevice *pDevice,
							  CTachyonRes *pRES)
{
	CTClientMoveObj::DoRETRANS( pDevice, pRES);

	InitOBJ(pRES->GetOBJ( CTChart::m_vTRACE[GetRaceID()][m_bSex]));
	ResetEQUIP( pDevice, pRES);

	m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_bCanSelected = TRUE;

	InitSIZE(
		GetAttrFLOAT(ID_SIZE_X),
		GetAttrFLOAT(ID_SIZE_Y),
		GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	ResetRootID(ID_PIVOT_WAIST);

	if( m_bDoubleHead )
		SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f );
}

void CTClientChar::DoDISGUISE( CD3DDevice *pDevice,
							   CTachyonRes *pRES,
							   BYTE bCountry)
{
	m_bPants	= bCountry;
	m_bBody		= bCountry;
	m_bHand		= bCountry;
	m_bFoot		= bCountry;

	CTClientMoveObj::DoDISGUISE(pDevice, pRES, bCountry);
}
	
void CTClientChar::EndDISGUISE( CD3DDevice *pDevice,
							    CTachyonRes *pRES)
{
	m_bPants	= m_bContryID;
	m_bBody		= m_bContryID;
	m_bHand		= m_bContryID;
	m_bFoot		= m_bContryID;

	CTClientMoveObj::EndDISGUISE(pDevice, pRES);
}

BYTE CTClientChar::IsAlliance( CTClientObjBase *pTARGET)
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
			CTClientChar* pMainChar = pGame->GetMainChar();
			CTClientChar* pDuelTarg = pGame->GetDuelTarget();
			if( (pMainChar == this && pDuelTarg == pHostTarget) ||
				(pDuelTarg == this && pMainChar == pHostTarget))
			{
				return FALSE;
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

		if( pHostTarget )
		{
			if( m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
				pHostTarget->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
				pHostTarget->m_bTournamentTeam != m_bTournamentTeam  )
			{
				return FALSE;
			}
		}

		return TRUE;
	}
	
	return CTClientMoveObj::IsAlliance(pTARGET);
}

void CTClientChar::OnDIE(CD3DDevice *pDevice, CTachyonRes *pRES)
{
	if( m_pRidingPet )
		m_pRidingPet->TakeDown();
	
	CTClientMoveObj::OnDIE(pDevice, pRES);
}

void CTClientChar::ActivateTALK(LPDIRECT3DDEVICE9 pDevice, CString strTALK, DWORD dwCOLOR)
{
	if( m_pRidingPet )
		m_pRidingPet->ActivateTALK(pDevice, strTALK, dwCOLOR);
	else
		CTClientMoveObj::ActivateTALK(pDevice, strTALK, dwCOLOR);
}

BYTE CTClientChar::GetDrawName()
{
	if( CTClientObjBase::GetDrawName() &&
		CTClientGame::GetPcNAMEOption() &&
        !m_pRidingPet )
	{
		return TRUE;
	}
	else
		return FALSE;
}

D3DXMATRIX& CTClientChar::GetPositionMat()
{
	if(m_pRidingPet)
		return m_pRidingPet->GetPositionMat();

	return CTClientMoveObj::GetPositionMat();
}

void CTClientChar::DoResetObj(
	CD3DDevice *pDevice,
	CTachyonRes *pRES)
{
	DWORD dwActionID = m_dwActionID;
	DWORD dwAniID = m_dwAniID;

	D3DXMATRIX vWorld = m_vWorld;
	InitOBJ(pRES->GetOBJ( CTChart::m_vTRACE[GetRaceID()][m_bSex]));
	
	ResetEQUIP( pDevice, pRES);

	m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_fJumpSpeed = GetAttrFLOAT(ID_JUMP_SPEED);

	InitSIZE( GetAttrFLOAT(ID_SIZE_X),
		GetAttrFLOAT(ID_SIZE_Y),
		GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	ResetRootID(ID_PIVOT_WAIST);
	SetPosition(vWorld);

	m_dwActionID = 0;
	m_dwAniID = 0;

	OBJMSG msg;

	msg.m_dwMSG = OM_ACTENDING;
	msg.m_pOBJ = (DWORD_PTR) this;
	m_dwActionID = 0;
	m_dwAniID = 0;

	m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

	if( !m_pRidingPet )
	{
		switch( GetTAction() )
		{
			case TA_WALK		:
			case TA_RUN			:
			case TA_BACK		:
			case TA_TURN_LEFT	:
			case TA_TURN_RIGHT	: break;
			default				: ApplyTAction( pDevice, pRES, TA_STAND); break;
		}
	}
	else
	{
		ClearAnimationID();
		SetAction(dwActionID, dwAniID );
	}
}

BOOL CTClientChar::CanTalkWithNPC(
					CTClientNpc* pTARGET )
{
	// 변장에 관계 없이 상대방이 중립지역이면 무조건 대화 가능
	if( pTARGET->m_bContryID == TCONTRY_N )
		return TRUE;

	if( IsDisguise() ) // 변장 중
	{
		if( pTARGET->m_bOriginalContryID == TCONTRY_N ) // 원래 중립지역NPC 인 경우
		{
			if( m_bDisguise == pTARGET->m_bContryID )
				return TRUE;
		}
		else // 원래 특정국가소속NPC 인 경우
		{
			if( m_bDisguise == pTARGET->m_bContryID &&
				m_bContryID == pTARGET->m_bOriginalContryID )
				return TRUE;
		}
	}
	else // 변장 아닌 중
	{
		if( m_bContryID == pTARGET->m_bContryID)
			return TRUE;
	}

	return FALSE;
}

void CTClientChar::ResetEQUIP(
	CD3DDevice *pDevice,
	CTachyonRes *pRES)
{
	CTClientMoveObj::ResetEQUIP( pDevice, pRES );
}


///////////////////////////////////////////////////////////
//	성물쟁탈전 Function Implementation.

void	CTClientChar::PlayGodBallSFX( BOOL bShow )
{
	if( GetGodBallID() == 0 )
		return ;

	CTGodBall* pGodBall = SiegeMgr->FindGodBall( this->GetGodBallID() );
	if( pGodBall == NULL )
		return ;

	LPTGODBALL pBallInfo = pGodBall->GetBallInfo();
	if( pBallInfo == NULL )
		return ;

	VECTOROBJSFX::iterator itor = m_vTGARBAGESFX.begin();
	while( itor != m_vTGARBAGESFX.end() )
	{
		if( (*itor)->m_pSFX->m_dwFuncID = pBallInfo->m_dwSfxID )
			return ;

		++itor;
	}

	PlayTActionSFX( &m_vTGARBAGESFX, pBallInfo->m_dwSfxID, bShow );
}

void	CTClientChar::ClearGodBallSFX()
{
	if( GetGodBallID() == 0 )
		return ;

	CTGodBall* pGodBall = SiegeMgr->FindGodBall( this->GetGodBallID() );
	if( pGodBall == NULL )
		return ;

	LPTGODBALL pBallInfo = pGodBall->GetBallInfo();
	if( pBallInfo == NULL )
		return ;

	ClearSFX( &m_vTGARBAGESFX, pBallInfo->m_dwSfxID );
}

void	CTClientChar::EnterCastle()
{
	m_bEnterCastle = TRUE;
}

void	CTClientChar::LeaveCastle()
{
	m_bEnterCastle = FALSE;
}

BOOL	CTClientChar::InCastle() const
{
	return m_bEnterCastle;
}

WORD	CTClientChar::GetCastleID() const
{
	return m_wCastleID;
}

WORD	CTClientChar::GetGodBallID() const
{
	return m_wBallID;
}

BYTE	CTClientChar::GetCamp() const
{
	return m_bCamp;
}

void	CTClientChar::SetCastleID( WORD wCastleID )
{
	m_wCastleID = wCastleID;
}

void	CTClientChar::SetGodBallID( WORD wBallID )
{
	m_wBallID = wBallID;
}

void	CTClientChar::SetCamp( BYTE bCamp )
{
	m_bCamp = bCamp;
}

//	End of 성물쟁탈전 Function.
///////////////////////////////////////////////////////////
void CTClientChar::ApplyMantleTexture()
{
	MAPTINVEN::iterator itTINVEN = m_mapTINVEN.find(INVEN_EQUIP);
	if( itTINVEN != m_mapTINVEN.end() )
	{
		CTClientInven *pTEQUIP = (*itTINVEN).second;
		CTClientItem* pTITEM = pTEQUIP->FindTItem( ES_BACK );
		if( pTITEM )
		{
			BOOL bRegGuild = pTITEM->GetRegGuild();

#ifdef ADD_TEXTOOL
			bRegGuild = TRUE;
#endif

			if( bRegGuild && m_pGuildMark )
			{
				CTClientGuildMark::SetMantleTexture(
					this,
					m_pGuildMark );
			}
			else if( pTITEM->GetTITEM() &&
				!pTITEM->GetTITEM()->m_bCanColor )
			{
			}
			else if( TCOLOR_NONE != pTITEM->GetColor() )
			{
				CTClientGuildMark::SetMantleColor(
					this,
					(TCOLOR) pTITEM->GetColor() );
			}
			else
			{
				CTClientGuildMark::SetMantleColor(
					this,
					TCOLOR_MANTLE_DEFAULT );
			}
		}
	}	
}

BYTE CTClientChar::GetAftermathStep()
{
	return m_bAfterMath;
}

bool CTClientChar::GetHasShield()
{
	MAPTINVEN::iterator finder = m_mapTINVEN.find(INVEN_EQUIP);
	if( finder != m_mapTINVEN.end() )
	{
		CTClientInven* pTINVEN = finder->second;

		MAPTITEM::iterator itITEM, endITEM;
		itITEM = pTINVEN->m_mapTITEM.begin();
		endITEM = pTINVEN->m_mapTITEM.end();
		for(; itITEM != endITEM ; ++itITEM )
		{
			if( itITEM->second->GetTITEM()->m_bType == 6 )
				return true;
		}
	}

	return false;
}

DWORD CTClientChar::GetEXP()
{
	return m_dwEncryptEXP ^ m_zEncryptKEY;
}

void CTClientChar::SetEXP(DWORD dwEXP)
{
	m_dwEncryptEXP = dwEXP ^ m_zEncryptKEY;
}
