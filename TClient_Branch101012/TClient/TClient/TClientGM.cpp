#include "stdafx.h"
#include "Resource.h"
#include "TClientWnd.h"
#include "TClient.h"
#include "TItemUpDlg.h"
#include "TItemUpGaugeDlg.h"
#include "TItemRefineDlg.h"
#include "TItemRepairDlg.h"
#include "TItemGambleDlg.h"
#include "TItemExtPeriodDlg.h"
#include "TCommunityDlg.h"
#include "TPrivateShopDlg.h"
#include "TMailAskMoneyDlg.h"
#include "TMailSendDlg.h"
#include "TMailRecvDlg.h"
#include "TMailListFrame.h"
#include "TMessengerBase.h"
#include "TMsgGroupList.h"
#include "TMessengerChat.h"
#include "TPetMakeDlg.h"
#include "TPetManageDlg.h"
#include "TGuildMemberFrame.h"
#include "TGuildVolunteerFrame.h"
#include "TSoulmateFrame.h"
#include "Curse.h"
#include "THelpFrame.h"
#include "TNewCashShopDlg.h"
#include "TCraftQuestDlg.h"
#include "TChatFrame.h"
#include "TGuildPvPRecordFrame.h"
#include "TGuildPointRewardFrame.h"
#include "TUpItemSellDlg.h"
#include "TItemUpCashDlg.h"
#include "TPortalShopDlgNew.h"
#include "TGuildRegularOffer.h"
#include "TGuildTacticsOffer.h"
#include "TTacticsInviteDlg.h"
#include "TItemColorDlg.h"
#include "TItemRegGuildDlg.h"
#include "TColorPicker.h"
#include "TFameRank.h"
#include "TFameBaseDlg.h"
#include "TFameListDlg.h"
#include "TCharRankInfoDlg.h"
#include "TSettingFrame.h"
#include "TWebCashShopDlg.h"
#include "TTournament.h"
#include "TTournamentEvent.h"
#include "TTournamentGage.h"
#include "TTournamentSelectTarget.h"
#include "TTournamentMode.h"
#include "TRpcDlg.h"
#include "TRpcRewardDlg.h"
#include "TCharPvPInfoDlg.h"
#include "TCharRankInfoDlg.h"

int CTClientGame::OnGM_MAIN_MENU()
{
	ToggleUI(TFRAME_MAIN_MENU);
	return TERR_NONE;
}

int CTClientGame::OnGM_SYSTEM_SETTING()
{
	EnableUI(TFRAME_SETTING);
	static_cast<CTFrameGroupBase*>(m_vTFRAME[TFRAME_SETTING])->SetSelect(TSETTING_FRAME_SYSTEM);

	return TERR_NONE;
}

int CTClientGame::OnGM_CONTROL_SETTING()
{
	EnableUI(TFRAME_KEYSETTING);
	return TERR_NONE;
}

int CTClientGame::OnGM_GAME_SETTING()
{
	EnableUI(TFRAME_SETTING);
	static_cast<CTFrameGroupBase*>(m_vTFRAME[TFRAME_SETTING])->SetSelect(TSETTING_FRAME_GAME);
	return TERR_NONE;
}

int CTClientGame::OnGM_SYSTEM()
{
	m_pMainWnd->MessageBoxYesNo(
		TSTR_EXITGAME_MSG,
		TSTR_YES,
		TSTR_NO,
		GM_GOBACK_CHARSEL,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnGM_QUIT()
{
	m_pMainWnd->MessageBoxYesNo(
		TSTR_QUIT_MSG,
		TSTR_YES,
		TSTR_NO,
		GM_BEGIN_EXIT,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnGM_GOBACK_CHARSEL()
{
	m_pMainWnd->CloseMessageBox(TRUE);
	DoSelectCharacter();
	return TERR_NONE;
}

int CTClientGame::OnGM_BEGIN_EXIT()
{
	m_pMainWnd->CloseMessageBox(TRUE);
	m_pMainWnd->ExitGame();

	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_CANCELQUIT()
{
	m_pMainWnd->CloseMessageBox();

	m_bDoChangeCH = FALSE;
	m_bDoSelectCHAR = FALSE;
	m_bDoEXIT = FALSE;

	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_WORLDMAP()
{
	if( m_bTournamentBattle || m_bTournamentWatching )
	{
		m_pChatFrame->ChatSysMSG(
			CTChart::LoadString(TSTR_INFO_TITLE),
			m_pMainChar->GetName(),
			CTChart::LoadString(TSTR_TOURNAMENT_CANT_WORLDMAP),
			TCOLOR_ERROR,
			TCHAT_FLAG_INFO,
			ID_SND_ERROR,
			MEDIA_TSOUND);

		return TERR_NONE;
	}

	CTWorldmapDlg *pTMAP = (CTWorldmapDlg *) m_vTFRAME[TFRAME_WORLDMAP];
	
	FLOAT fPOSX = m_pMainChar->GetPositionX(); 
	FLOAT fPOSZ = m_pMainChar->GetPositionZ();

	if( pTMAP->GetWorldmapLevel(fPOSX,fPOSZ) == TWORLDMAP_LEVEL_NONE )
	{
		CString strMSG = CTChart::LoadString( TSTR_ERROR_CANNOT_OPEN_WORLDMAP);
		CString strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		m_pChatFrame->ChatSysMSG(
			strTYPE,
			m_pMainChar->GetName(),
			strMSG, 
			TCOLOR_ERROR,
			TCHAT_FLAG_INFO,
			ID_SND_ERROR,
			MEDIA_TSOUND);

		return TERR_NONE;
	}

	pTMAP->m_bTALLWORLDMODE = FALSE;
	pTMAP->m_vTCENTER.x = fPOSX;
	pTMAP->m_vTCENTER.y = fPOSZ;

	EnableUI(TFRAME_WORLDMAP);

	return TERR_NONE;
}

int CTClientGame::OnGM_BASIC_INFO()
{
	CTCharInfoDlg *pTCharInfoDlg = (CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO];

	if( !pTCharInfoDlg->IsVisible() )
		pTCharInfoDlg->ShowComponent( TRUE );

	CTCharInfoDlg::m_bTabIndex = TINFO_BASIC;

	pTCharInfoDlg->ResetData(
		m_pMainChar,
		m_pRES,
		TINFO_BASIC);

	return TERR_NONE;
}

int CTClientGame::OnGM_MORE_INFO()
{
	CTCharInfoDlg *pTCharInfoDlg = (CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO];
	
	if( !pTCharInfoDlg->IsVisible() )
		pTCharInfoDlg->ShowComponent( TRUE );

	CTCharInfoDlg::m_bTabIndex = TINFO_MORE;

	pTCharInfoDlg->ResetData(
		m_pMainChar,
		m_pRES,
		TINFO_MORE);

	return TERR_NONE;
}

int CTClientGame::OnGM_BLOCK_REQ()
{
	CTMessengerCommander *pDlg = CTMessengerCommander::GetInstance();

	CString strTarget;

	if( m_pTARGET && m_pTARGET->m_bType == OT_PC )
	{
		strTarget = m_pTARGET->GetName();
	}
	else if ( m_pChatFrame->GetCandiWhisperTarget() != "" )
	{
		strTarget = m_pChatFrame->GetCandiWhisperTarget();
		m_pChatFrame->SetCandiWhisperTarget("");
	}
	else
		return TERR_NONE;

	pDlg->DoAddBlock( strTarget );

	return TERR_NONE;
}

int CTClientGame::OnGM_UNBLOCK_REQ()
{
	CTMessengerCommander *pDlg = CTMessengerCommander::GetInstance();

	if( !m_pTARGET || m_pTARGET->m_bType != OT_PC )
		return TERR_NONE;

	pDlg->DoDeleteBlock( m_pTARGET->GetName() );

	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_UI()
{
	for( BYTE i=0; i<TFRAME_COUNT; i++)
		if( m_pTCURFRAME && m_pTCURFRAME == m_vTFRAME[i] )
		{
			switch(i)
			{
			case TFRAME_MAIN :
			case TFRAME_GAUGE : 
				break;

			case TFRAME_TRADE : 
				CancelTrade(); 
				break;

			case TFRAME_PRIVATE_SELL : 
				CancelPrivateSell();
				break;

			case TFRAME_MAIL_SEND:
				static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND])->ClearUsingByCash();
				DisableUI(TFRAME_MAIL_SEND);
				break;

			default :
				DisableUI(i); 
				break;
			}

			break;
		}

	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_POPUP()
{
#ifdef MODIFY_CNP_NAME
	m_pChatFrame->HideAllPopup();
#endif

	m_pTPOPUP->ShowComponent(FALSE);
	m_pTPOPUP->ClearMENU();
	m_pChannelPOPUP->ShowComponent(FALSE);
	m_pChannelPOPUP->ClearMENU();
	m_bRSCSMENU = FALSE;

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTYMEMBERRECALL()
{
	BYTE bInven, bSlot;

	if( m_pSESSION && 
		m_pTPARTY && 
		!m_pMainChar->GetRidingPet() && 
		FindInven(IT_USE, IK_MEMBERRECALL, bInven, bSlot) )
	{
		m_pSESSION->SendCS_PARTYMEMBERRECALL_REQ(
			bInven, 
			bSlot, 
			m_pTPARTY->m_strNAME);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_USEITEM_PARTYMEMBERRECALL()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && m_pTPARTY )
	{
		m_pSESSION->SendCS_PARTYMEMBERRECALL_REQ(
			m_bPortalItemInven, 
			m_bPortalItemSlot, 
			m_pTPARTY->m_strNAME);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_USEITEM_MOVETOCHAR()
{
	CString strNAME = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( strNAME.IsEmpty() )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_PARTYMEMBERRECALL_REQ(
			m_bPortalItemInven, 
			m_bPortalItemSlot, 
			strNAME);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTYMEMBERRECALLANS_OK()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_PARTYMEMBERRECALLANS_REQ(
			ASK_YES, 
			m_bPartyRecallName,	
			m_bPartyRecallAnsInven,
			m_bPartyRecallAnsSlot);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTYMEMBERRECALLANS_NO()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_PARTYMEMBERRECALLANS_REQ(
			ASK_NO, 
			m_bPartyRecallName,	
			m_bPartyRecallAnsInven,
			m_bPartyRecallAnsSlot);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_REQ()
{
	if( m_pTARGET &&
		m_pTARGET->m_bType == OT_PC &&
		!(m_pTARGET->IsDead() || m_pTARGET->m_bGhost) &&
		m_pSESSION )
	{
		BYTE bObtainType;
		if( GetPartyID(m_pMainChar) )
			bObtainType = m_bPartyItemType;
		else
			bObtainType = PT_ORDER;

		m_pSESSION->SendCS_PARTYADD_REQ(m_pTARGET->GetName(), bObtainType);
	}
	else if ( m_pChatFrame->GetCandiWhisperTarget() != "" )
	{
		BYTE bObtainType;
		CString strTarget = m_pChatFrame->GetCandiWhisperTarget();
		m_pChatFrame->SetCandiWhisperTarget("");

		if( GetPartyID(m_pMainChar) )
			bObtainType = GetPartyItemType();
		else
			bObtainType = PT_ORDER;

		m_pSESSION->SendCS_PARTYADD_REQ(strTarget,bObtainType);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_OK()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_PARTYJOIN_REQ(
			m_strReqNAME,
			m_bObtainType,
			ASK_YES);
	}
	
	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_NO()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_PARTYJOIN_REQ(
			m_strReqNAME,
			m_bObtainType,
			ASK_NO);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_INVITE_FRIEND_TO_PARTY()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem )
	{
		BYTE bObtainType;
		if( GetPartyID(m_pMainChar) )
			bObtainType = m_bPartyItemType;
		else
			bObtainType = PT_ORDER;

		const CString& strTarget = pItem->GetNameComp()->m_strText;
		m_pSESSION->SendCS_PARTYADD_REQ(strTarget,bObtainType);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CHANGE_PARTY_ITEM_TYPE()		
{
	static const BYTE vNEXTID[] = 
	{
		PT_LOTTERY, //PT_FREE
		PT_FREE,	//PT_SOLO,
		PT_CHIEF,	//PT_HUNTER,
		PT_HUNTER,	//PT_LOTTERY,
		PT_ORDER,	//PT_CHIEF
		PT_FREE		//PT_ORDER
	};

	if( m_pSESSION )
		m_pSESSION->SendCS_CHGPARTYTYPE_REQ( vNEXTID[m_bPartyItemType] );

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_SETCHIEF()
{
	WORD wSquadID = GetPartyID(m_pMainChar);
	WORD wPartyID = GetPartyID(m_pTARGET);

	if( wSquadID && wSquadID == wPartyID && m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
	{
		if( m_pMainChar->m_wCommanderID != wSquadID )
			ExecTSQUADCMD( wSquadID, TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF);
		else
			ExecTCMD( TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF, TRUE);

		RelaySession()->SendCS_CORPSCMD_REQ(
			wSquadID,
			m_pMainChar->m_dwID,
			0xFFFF,
			TRSCSCMD_NONE,
			0, 0, 0, 0);

		m_pSESSION->SendCS_CHGPARTYCHIEF_REQ(m_pTARGET->m_dwID);
		m_pMainChar->m_dwChiefID = 0;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_FARPARTY_SETCHIEF()
{
	WORD wPartyID = GetPartyID(m_pMainChar);

	if( m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
	{
		if( m_pMainChar->m_wCommanderID != wPartyID )
			ExecTSQUADCMD( wPartyID, TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF);
		else
			ExecTCMD( TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF, TRUE);

		RelaySession()->SendCS_CORPSCMD_REQ(
			wPartyID,
			m_pMainChar->m_dwID,
			0xFFFF,
			TRSCSCMD_NONE,
			0, 0, 0, 0);

		m_pSESSION->SendCS_CHGPARTYCHIEF_REQ(m_pTPARTY->m_dwCharID);
		m_pMainChar->m_dwChiefID = 0;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_SETCOMMANDER()
{
	WORD wSquadID = GetPartyID(m_pMainChar);
	WORD wPartyID = 0;

	if(m_bRSCSMENU)
		wPartyID = m_pTRSCS->m_pTCURSQUAD->m_wPartyID;
	else
		wPartyID = GetPartyID(m_pTARGET);

	if( wPartyID && wSquadID && wPartyID != wSquadID &&
		m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID &&
		m_pTRSCS->m_vTCORPS.m_wID &&
		m_pTRSCS->FindTSQUAD(wPartyID) &&
		m_pMainChar->m_wCommanderID == wSquadID )
	{
		for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
			if( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID != wSquadID )
				ExecTCORPSCMD( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i], TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF);

		RelaySession()->SendCS_CORPSCMD_REQ(
			wSquadID,
			m_pMainChar->m_dwID,
			0xFFFF,
			TRSCSCMD_NONE,
			0, 0, 0, 0);

		m_pSESSION->SendCS_CHGCORPSCOMMANDER_REQ(wPartyID);
		m_pMainChar->m_dwChiefID = 0;
	}

	m_bRSCSMENU = FALSE;

	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_LEAVE()
{
	m_pSESSION->SendCS_CORPSLEAVE_REQ( (WORD) m_dwCmdParam );
	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_REQ()
{
	WORD wPartyID = GetPartyID(m_pMainChar);

	if(!wPartyID)
	{
		CorpsMSG( m_pMainChar, CORPS_NO_PARTY);
		return TERR_NONE;
	}

	if( m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID || (
		m_pTRSCS->m_vTCORPS.m_wID &&
		m_pMainChar->m_wCommanderID != wPartyID) )
	{
		CorpsMSG( m_pMainChar, CORPS_NOT_COMMANDER);
		return TERR_NONE;
	}

	if( !m_pTARGET || m_pTARGET->m_bType != OT_PC )
	{
		CorpsMSG( m_pMainChar, CORPS_WRONG_TARGET);
		return TERR_NONE;
	}

	if( !GetPartyID(m_pTARGET) || ((CTClientChar *) m_pTARGET)->m_dwChiefID != m_pTARGET->m_dwID )
	{
		CorpsMSG( m_pMainChar, CORPS_TARGET_NO_PARTY);
		return TERR_NONE;
	}

	if(((CTClientChar *) m_pTARGET)->m_wCommanderID)
	{
		CorpsMSG( m_pMainChar, CORPS_ALREADY);
		return TERR_NONE;
	}
	m_pSESSION->SendCS_CORPSASK_REQ(m_pTARGET->GetName());

	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_OK()
{
	m_pMainWnd->CloseMessageBox();
	
	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_CORPSREPLY_REQ(
			m_strReqNAME,
			ASK_YES);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_NO()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_CORPSREPLY_REQ(
			m_strReqNAME,
			ASK_NO);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ATTACK()
{
	UseTSkill( m_wSkillID, FALSE, FALSE);
	return TERR_NONE;
}

int CTClientGame::OnGM_REVIVAL_YES()
{
	m_pMainWnd->CloseMessageBox( TRUE );

	if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
	{
		SendCS_REVIVALASK_REQ(
			ASK_YES,
			m_pMainChar->m_vTSKILLDATA.m_dwMaxAP,
			m_pMainChar->m_vTSKILLDATA.m_bAtkLevel,
			m_pMainChar->m_vTSKILLDATA.m_wID,
			m_pMainChar->m_vTSKILLDATA.m_bLevel);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_REVIVAL_NO()
{
	if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
	{
		SendCS_REVIVALASK_REQ(
			ASK_NO,
			m_pMainChar->m_vTSKILLDATA.m_dwMaxAP,
			m_pMainChar->m_vTSKILLDATA.m_bAtkLevel,
			m_pMainChar->m_vTSKILLDATA.m_wID,
			m_pMainChar->m_vTSKILLDATA.m_bLevel);
	}

	if(!m_pMainChar->m_bGhost)
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_GOTO_THUMB_MSG,
			TSTR_OK,
			GM_GOTO_THUMB,
			0,
			TRUE,
			ID_SND_INFO,
			MEDIA_TSOUND,
			TMSGBOXOPEN_CLOSEALL_ALREADY);

		EnableChat(FALSE);
	}
	else
		m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_REVIVAL()
{
	if( CheckRevival(m_pTARGET ? &(m_pTARGET->GetPosition()) : &(m_pMainChar->m_vTDEAD) ) )
	{
		if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
		{
			SendCS_REVIVAL_REQ(
				m_pMainChar->GetPositionX(),
				m_pMainChar->GetPositionY(),
				m_pMainChar->GetPositionZ(),
				REVIVAL_NPC);

			m_bAutoRun = FALSE;
		}

		m_pMainWnd->CloseMessageBox();
	}
	else
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::LoadString( TSTR_CANNOT_REVIVALPOS);
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
			TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_REVIVAL()
{
	if(CheckRevival( &(m_pMainChar->m_vTDEAD) ) )
	{
		if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
		{
			SendCS_REVIVAL_REQ(
				m_pMainChar->GetPositionX(),
				m_pMainChar->GetPositionY(),
				m_pMainChar->GetPositionZ(),
				REVIVAL_GHOST);

			m_bAutoRun = FALSE;
		}

		m_pMainWnd->CloseMessageBox();
	}
	else
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::LoadString( TSTR_CANNOT_REVIVALPOS);
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
			TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_EXIT()
{
	if(m_pSESSION)
		m_pSESSION->SendCS_PARTYDEL_REQ(m_pMainChar->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_KICK()
{
	if( m_pSESSION && m_pTPARTY )
		m_pSESSION->SendCS_PARTYDEL_REQ(m_pTPARTY->m_dwCharID);

	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_SKILLINFO()
{
	if(m_pSESSION)
		m_pSESSION->SendCS_NPCITEMLIST_REQ(TDEF_SKILL_NPC);

	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_PARTYINFO()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_GUILDINFO()
{
	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	if( !pComm->IsVisible() )
		EnableUI(TFRAME_COMMUNITY);

	pComm->SetSelect(TCOMMUNITY_FRAME_GUILDNORMAL);
	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_CHARINFO()
{
	EnableUI(TFRAME_CHAR_INFO);
	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_YES()
{
	m_pMainWnd->CloseMessageBox();

	m_pSESSION->SendCS_DEALITEMRLY_REQ(
		ASK_YES,
		m_strReqNAME);
	
	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_NO()
{
	m_pMainWnd->CloseMessageBox();

	m_pSESSION->SendCS_DEALITEMRLY_REQ(
		ASK_NO,
		m_strReqNAME);
	
	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_LOCK()
{
	CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];
	CPacket vMSG;
	BYTE bCount = (BYTE) pTTradeDlg->GetItemCount(FALSE);

	DWORD dwRune = pTTradeDlg->GetMyRune();
	DWORD dwLuna = pTTradeDlg->GetMyLuna();
	DWORD dwCron = pTTradeDlg->GetMyCron();

	vMSG.SetID(CS_DEALITEMADD_REQ)
		<< dwRune
		<< dwLuna
		<< dwCron
		<< bCount;

	for( BYTE i=0; i<bCount; ++i )
	{
		CTTradeItem* pInfo = pTTradeDlg->GetItem(FALSE, i);

		vMSG << pInfo->m_bInven
			 << pInfo->m_bInvenSlot;
	}

	m_pSESSION->Say(&vMSG);
	pTTradeDlg->SetLock(TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_OK()
{
	((CTTradeDlg *) m_vTFRAME[TFRAME_TRADE])->SetTradeEnable(FALSE);
	m_pSESSION->SendCS_DEALITEM_REQ(TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_REQ()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_PC )
	{
		TradeMSG( CString(_T("")), DEALITEM_NOTARGET);
		return TERR_NONE;
	}

	if(!m_pMainChar->IsAlliance(m_pTARGET))
	{
		TradeMSG( CString(_T("")), DEALITEM_ENEMY);
		return TERR_NONE;
	}
	m_pSESSION->SendCS_DEALITEMASK_REQ(m_pTARGET->GetName());

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_TRADE()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		return TERR_NONE;
	}

	if( ((CTClientNpc*)m_pTARGET)->m_bCamp != m_pMainChar->m_bCamp )
		return TERR_NONE;

	BOOL bAlliance;
	if( m_pTARGET->m_bContryID != TCONTRY_N )
	{
		if( m_pMainChar->IsDisguise() )
			bAlliance = !m_pMainChar->IsAlliance(m_pTARGET);
		else
			bAlliance = m_pMainChar->IsAlliance(m_pTARGET);
	}
	else
		bAlliance = TRUE;

	if( !bAlliance )
		return TERR_NONE;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_TRADE;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		switch(m_pTARGET->m_bNPCType)
		{
		case TNPC_MAKE			:
		case TNPC_BOX			:
		case TNPC_NONE			: return TERR_NONE;
		
		case TNPC_UPGRADE		: EnableUI(TFRAME_ITEM_UP); break;
		case TNPC_GAMBLE		: EnableUI(TFRAME_ITEM_GAMBLE); break;

		case TNPC_REVIVAL		:
			{
				m_pMainWnd->MessageBoxYesNo(
					TSTR_NPC_REVIVAL,
					TSTR_YES,
					TSTR_NO,
					GM_NPC_REVIVAL,
					GM_CLOSE_MSGBOX );

				EnableChat(FALSE);
			}
			break;

		case TNPC_RETURN		:
			{
				m_pMainWnd->MessageBoxYesNo(
					TSTR_SET_HOME,
					TSTR_YES,
					TSTR_NO,
					GM_SET_RETURNPOS,
					GM_CLOSE_MSGBOX );

				EnableChat(FALSE);
			}

			break;

		case TNPC_WAREHOUSE		:
			{
				CTCabinetDlg* pCabDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
				pCabDlg->RequestShowComponent();
			}

			break;

		case TNPC_GUILD			:
			{
				CTCabinetDlg* pCabDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_GUILDCABINET]);
				pCabDlg->RequestShowComponent();
			}

			break;

		case TNPC_DIVINER		:
			{
				m_pMainWnd->MessageBoxYesNo(
					TSTR_SOULMATEBOX_SELREG,
					TSTR_YES,
					TSTR_NO,
					GM_SOULMATE_REGBOX,
					GM_SOULMATE_SEARCH );

				EnableChat(FALSE);
			}
			break;

		case TNPC_AUCTION		:
			{
				CTAuctionCommander::GetInstance()
					->SetNpcID( (WORD)m_pTARGET->m_dwID );

				EnableUI( TFRAME_AUCTION );

				CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
				pFrame->ResetInfo();
			}
			break;

		default					:
			if( m_pSESSION && m_pTARGET->m_bClassID & (1 << m_pMainChar->m_bClassID) )
				m_pSESSION->SendCS_NPCITEMLIST_REQ(WORD(m_pTARGET->m_dwID));

			break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_GUILD_APP()
{
	EnableUI(TFRAME_GUILDAPP);
	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_GUILD_NEW()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_GUILD_NEW;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		if( m_pMainChar->m_bLevel < TGUILD_ESTABLISH_LEVEL)
		{
			CString msg;
			msg = CTChart::Format( TSTR_MBOX_GUILD_BUILD_LOWLEVEL, TGUILD_ESTABLISH_LEVEL);
			m_pMainWnd->MessageBoxOK(
				msg,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
		}
		else
		{
			if( CTNationOption::RUSSIA )
			{
				TFrame *pFRAME = m_pMainWnd->m_MessageBox.FindFrame(ID_FRAME_2BTNMSGBOX);
				TComponent *pEDIT = pFRAME->FindKid(ID_CTRLINST_EDIT);
				pEDIT->m_style |= TES_NOCONMODE;
			}

			m_pMainWnd->MessageBox(
				TSTR_MBOX_GUILD_BUILD,
				0, 0,
				TSTR_YES,
				TSTR_NO,
				ID_FRAME_2BTNMSGBOX,
				GM_ESTABLISH_GUILD,
				GM_CLOSE_MSGBOX_WITH_RESETCONMODE,
				GM_CLOSE_MSGBOX_WITH_RESETCONMODE,
				TRUE,
				TRUE);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_GUILD_MARK()
{
	EnableUI(TFRAME_GUILDMARK);
	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_GUILD_DEL()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_GUILD_DEL;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		CTGuildCommander::GetInstance()
			->RequestGuildDisorganization(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_GUILD_DEL_CANCEL()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_GUILD_DEL_CANCEL;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		CTGuildCommander::GetInstance()
			->RequestGuildDisorganization(TRUE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PORTAL_REQ()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC )
		return TERR_NONE;

	CTPortalShopDlg *pPORTALSHOP = (CTPortalShopDlg *) GetFrame(TFRAME_PORTAL_SHOP_NEW);
	CTCtrlList *pList = pPORTALSHOP->GetList();
	CTPortalShopItem *pInfo = static_cast<CTPortalShopItem *>(pList->GetSelectItem());

	if(pInfo)
	{
		LPTPORTAL pTPORTAL = pInfo->m_pTPortal;

		if( !m_bTELEPORT && pTPORTAL )
		{
			DisableUI(TFRAME_PORTAL_SHOP_NEW);

			m_pSESSION->SendCS_TELEPORT_REQ(
				pTPORTAL->m_wPortalID,
				(WORD) m_pTARGET->m_dwID);
			m_bTELEPORT = TRUE;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_BUY()
{
	CTShopDlg* pDLG = (CTShopDlg*) (m_vTFRAME[ TFRAME_ITEM_SHOP] );

	CTCtrlList* pList = pDLG->GetList();
	CTShopNpcItem* pInfo = static_cast<CTShopNpcItem*>(pList->GetSelectItem());

	if( pInfo )
	{
		LPTITEM pTITEM = pInfo->m_pTItem;

		CString strEDITTITLE;
		CString strEDIT;
		CString strMSG;

		strEDITTITLE = CTChart::LoadString( TSTR_EA);
		strEDIT = CTChart::Format( TSTR_FMT_NUMBER, 1);
		
		CString strPRICE = CTClientGame::MakeMoneyStr(pInfo->m_dwRealPrice);

		switch( pDLG->GetPhurchaseType() )
		{
		default:
		case TSHOP_PHURCHASE_TYPE_MONEY:
			{
				strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PRICE,
					pTITEM->m_strNAME,
					strPRICE);
			}
			break;
		case TSHOP_PHURCHASE_TYPE_POINT:
			{
				strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PVPPOINT,
					pTITEM->m_strNAME,
					pInfo->m_dwRealPrice );
			}
			break;
		}

		if( pTITEM->m_bStack > 1 )
		{
			CString strStack = CTChart::Format( TSTR_FMT_BUYITEM_STACK, pTITEM->m_bStack );
			strMSG += strStack;
		}

		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ITEM_DO_BUY,
			GM_CLOSE_MSGBOX,
			0,
			pTITEM->m_bStack > 1 ? TRUE : FALSE,
			TRUE);
	
		EnableChat(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_MONSTER_BUY()
{
	CTShopDlg* pDLG = (CTShopDlg*) (m_vTFRAME[ TFRAME_MONSHOP] );

	CTCtrlList* pList = pDLG->GetList();
	CTMonsterShopItem* pInfo = static_cast<CTMonsterShopItem*>(pList->GetSelectItem());

	if( pInfo )
	{
		LPTMONSHOP pTMON = pInfo->m_pTMon;

		CString strMSG;
		CString strPRICE = CTClientGame::MakeMoneyStr(pInfo->m_dwRealPrice);

		switch( pDLG->GetPhurchaseType() )
		{
		default:
		case TSHOP_PHURCHASE_TYPE_MONEY:
			strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PRICE,
				pTMON->m_strName,
				strPRICE);
			break;
		case TSHOP_PHURCHASE_TYPE_POINT:
			{
				strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PVPPOINT,
					pTMON->m_strName,
					pInfo->m_dwRealPrice );
			}
			break;
		}

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_MONSTER_DO_BUY,
			GM_CLOSE_MSGBOX );
	
		EnableChat(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_SELL()
{
	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return TERR_NONE;

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM)
		return TERR_NONE;

	if( !(pTITEM->GetTITEM()->m_bCanTrade & (1 << TTRADEOP_SELL)) )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_ITEM_CANNOT_SELL,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );
		
		return TERR_NONE;
	}

	CString strMSG;

	if( pTITEM->GetCount() > 1 )
	{
		CString strEDITTITLE;
		CString strEDIT;
		
		strEDITTITLE = CTChart::LoadString( TSTR_EA);
		strEDIT = CTChart::Format( TSTR_FMT_NUMBER,
			pTITEM->GetCount());

		strMSG = CTChart::Format( TSTR_FMT_SELLITEMS,
			pTITEM->GetTITEM()->m_strNAME);

		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_CONFIRM_ITEM_SELL,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);

		m_nSellCount = 0;
	}
	else
	{
		m_nSellCount = 1;
		
		if ( pTITEM->GetGrade() > 0 )
			OnGM_WARN_GRADE_ITEM_SELL();
		else
			OnGM_CONFIRM_ITEM_SELL();
	}

	EnableChat(FALSE);
	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_GRADE_ITEM_SELL()
{
	CTUpItemSellDlg *pDlg = static_cast<CTUpItemSellDlg*>(m_vTFRAME[ TFRAME_WARN_UPITEM_SELL ] );

	if( !m_nSellCount )
	{
		CString strCNT = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
		m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

		m_nSellCount = ::atoi(strCNT);
	}

	m_pMainWnd->CloseMessageBox();

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return TERR_NONE;

	CTClientItem *pClientITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pClientITEM)
		return TERR_NONE;

	LPTITEM pTITEM = pClientITEM->GetTITEM();
	if(!pTITEM)
		return TERR_NONE;

	if ( CTNationOption::JAPAN )
	{
		pDlg->m_strSecurityCode.Format("%04d", rand() % 9999);
		pDlg->m_pMSG->m_strText = CTChart::Format( TSTR_WARN_UPITEM_SELL_SECURITYCODE, pDlg->m_strSecurityCode );
	}
	else
		pDlg->m_pMSG->m_strText = CTChart::Format( TSTR_WARN_UPITEM_SELL );

	pDlg->SetClientItem( pClientITEM );
	pDlg->SetMode( CTUpItemSellDlg::MODE_SELL );

	// m_pMainWnd->DoModalFrame( TFRAME_WARN_UPITEM_SELL );
	pDlg->ShowComponent();
	SwitchFocus( pDlg );

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_UPITEM_SELL_OK()
{
	CTUpItemSellDlg *pDlg = static_cast<CTUpItemSellDlg*>(m_vTFRAME[ TFRAME_WARN_UPITEM_SELL ] );

	//m_pMainWnd->CloseModalFrame();

	TFrame *pFRAME = m_pMainWnd->m_TNet.FindFrame(ID_FRAME_LOGIN);
	TEdit* pEditID = (TEdit*) pFRAME->FindKid(ID_CTRLINST_ID);
	DWORD dwDifferentMsg;
	BOOL bOK = FALSE;

	CString strID;
	strID = pEditID->m_strText;
	strID.MakeUpper();

	if ( CTNationOption::JAPAN )
	{
		dwDifferentMsg = TSTR_WARN_UPITEM_SELL_DIFFERENT_SECURITYCODE;
		if ( pDlg->m_pID->m_strText == pDlg->m_strSecurityCode )
			bOK = TRUE;
	}
	else
	{
		CString strCompereID = pDlg->m_pID->m_strText;
		strCompereID.MakeUpper();

		dwDifferentMsg = TSTR_WARN_UPITEM_SELL_DIFFERENT_ID;
		if ( strID == strCompereID )
			bOK = TRUE;
	}

	if ( bOK )
	{
		if ( pDlg->GetMode() == CTUpItemSellDlg::MODE_SELL )
		{
			DisableUI( TFRAME_WARN_UPITEM_SELL );
			OnGM_CONFIRM_ITEM_SELL();
		}
		else if ( pDlg->GetMode() == CTUpItemSellDlg::MODE_DROP )
		{
			DisableUI( TFRAME_WARN_UPITEM_SELL );
			OnGM_DROP_ITEM_YES();
		}
	}
	else
	{
		m_pMainWnd->MessageBoxOK(
			dwDifferentMsg,
			TSTR_OK,
			GM_WARN_UPITEM_SELL_DIFFERENT_ID,
			0,
			TRUE,
			ID_SND_ERROR );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_UPITEM_SELL_DIFFERENT_ID()
{
	CTUpItemSellDlg *pDlg = static_cast<CTUpItemSellDlg*>(m_vTFRAME[ TFRAME_WARN_UPITEM_SELL ] );

	m_pMainWnd->CloseMessageBox();

	SwitchFocus( pDlg );
	pDlg->m_pID->ClearText();
	pDlg->m_pID->SetFocus();

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_UPITEM_SELL_CANCEL()
{
	m_pMainWnd->CloseMessageBox();

	DisableUI( TFRAME_WARN_UPITEM_SELL );

	return TERR_NONE;
}

int CTClientGame::OnGM_CONFIRM_ITEM_SELL()
{
	if( !m_nSellCount )
	{
		CString strCNT = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
		m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

		m_nSellCount = ::atoi(strCNT);
	}

	m_pMainWnd->CloseMessageBox();

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return TERR_NONE;

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM)
		return TERR_NONE;

	if( !m_nSellCount || m_nSellCount > pTITEM->GetCount() )
	{
		OnGM_ITEM_SELL();
		return TERR_NONE;
	}

	DWORD dwPrice = pTITEM->GetSellPrice() * m_nSellCount;
	INT nPriceUp = 0;
	if( m_pMainChar->CountPriceUpMaintain( &nPriceUp ) > 0 )
		dwPrice += (dwPrice*nPriceUp) / 100;

	CString strPrice = MakeMoneyStr(dwPrice);

	CString strMSG;
	strMSG = CTChart::Format( TSTR_FMT_SELLITEM,
		pTITEM->GetTITEM()->m_strNAME,
		m_nSellCount,
		strPrice);

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_YES,
		TSTR_NO,
		GM_ITEM_DO_SELL,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_DO_BUY()
{
	CTShopDlg* pSHOPDLG = (CTShopDlg*) GetFrame( TFRAME_ITEM_SHOP );

	if( m_pMainWnd->m_MessageBox.GetParent() != this )
		return TERR_NONE;

	TFrame *pTMSGBOX = (TFrame *) m_pMainWnd->m_MessageBox.FindKid(ID_FRAME_2BTNMSGBOX);
	if( !pTMSGBOX || !pTMSGBOX->CanProcess() )
		return TERR_NONE;
	TEdit *pTEDIT = (TEdit *) pTMSGBOX->FindKid(ID_CTRLINST_EDIT);

	if( !pSHOPDLG->m_pOpenByCash && !m_pTARGET )
		return TERR_NONE;

	CTCtrlList* pList = pSHOPDLG->GetList();
	CTShopNpcItem* pInfo = static_cast<CTShopNpcItem*>(pList->GetSelectItem());
	if( !pInfo )
		return TERR_NONE;

	LPTITEM pTITEM = pInfo->m_pTItem;
	INT nCount = 1;

	if( pTEDIT && pTEDIT->CanProcess() )
		nCount = (INT) atoi(pTEDIT->m_strText);

	if( nCount > pTITEM->m_bStack )
	{
		CString strEDITTITLE;
		CString strEDIT;
		CString strMSG;

		strEDITTITLE = CTChart::LoadString( TSTR_EA);
		strEDIT = CTChart::Format( TSTR_FMT_NUMBER,
			pTITEM->m_bStack);

		strMSG = CTChart::Format( TSTR_FMT_BUYSTACK,
			pTITEM->m_strNAME,
			pTITEM->m_bStack);

		m_pMainWnd->CloseMessageBox();
		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ITEM_DO_BUY,
			GM_CLOSE_MSGBOX,
			0,
			pTITEM->m_bStack > 1 ? TRUE : FALSE,
			TRUE);
		EnableChat(FALSE);

		return TERR_NONE;
	}

	BYTE bCount = (BYTE) nCount;

	if( !bCount )
	{
		CString strEDITTITLE;
		CString strEDIT;
		CString strMSG;

		strEDIT = CTChart::Format( TSTR_FMT_NUMBER, 1);
		strEDITTITLE = CTChart::LoadString( TSTR_EA);

		strMSG = CTChart::Format( TSTR_FMT_BUYONE,
			pTITEM->m_strNAME);

		m_pMainWnd->CloseMessageBox();
		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ITEM_DO_BUY,
			GM_CLOSE_MSGBOX,
			0,
			pTITEM->m_bStack > 1 ? TRUE : FALSE,
			TRUE);
		EnableChat(FALSE);

		return TERR_NONE;
	}

	if(m_pSESSION)
	{
		if( !pSHOPDLG->m_pOpenByCash )
			m_pSESSION->SendCS_ITEMBUY_REQ(
				WORD(m_pTARGET->m_dwID),
				0,
				pTITEM->m_wItemID,
				bCount,
				INVEN_NULL,
				INVALID_SLOT);
		else
		{
			m_pSESSION->SendCS_ITEMBUY_REQ(
				pSHOPDLG->m_pOpenByCash->m_wNPCID,
				0,
				pTITEM->m_wItemID,
				bCount,
				pSHOPDLG->m_pOpenByCash->m_bInvenID,
				pSHOPDLG->m_pOpenByCash->m_bSlotID);
		}
	}
	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_MONSTER_DO_BUY()
{
	CTShopDlg* pSHOPDLG = (CTShopDlg*) GetFrame( TFRAME_MONSHOP );

	if( m_pMainWnd->m_MessageBox.GetParent() != this )
		return TERR_NONE;

	TFrame *pTMSGBOX = (TFrame *) m_pMainWnd->m_MessageBox.FindKid(ID_FRAME_2BTNMSGBOX);
	if( !pTMSGBOX || !pTMSGBOX->CanProcess() )
		return TERR_NONE;

	if( !pSHOPDLG->m_pOpenByCash && !m_pTARGET )
		return TERR_NONE;

	CTCtrlList* pList = pSHOPDLG->GetList();
	CTMonsterShopItem* pInfo = static_cast<CTMonsterShopItem*>(pList->GetSelectItem());
	if( !pInfo )
		return TERR_NONE;

	LPTMONSHOP pTITEM = pInfo->m_pTMon;

	if(m_pSESSION)
	{
		if( !pSHOPDLG->m_pOpenByCash )
			m_pSESSION->SendCS_MONSTERBUY_REQ(
				WORD(m_pTARGET->m_dwID),
				(WORD)pTITEM->m_dwID );
	}
	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_DO_SELL()
{
	if( m_pMainWnd->m_MessageBox.GetParent() != this )
		return TERR_NONE;

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return TERR_NONE;

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM)
		return TERR_NONE;

	BYTE bCount = (BYTE) m_nSellCount;
	if( bCount > pTITEM->GetCount() )
	{
		CString strEDITTITLE;
		CString strEDIT;
		CString strMSG;

		strEDITTITLE = CTChart::LoadString( TSTR_EA);
		strEDIT = CTChart::Format( TSTR_FMT_NUMBER,
			pTITEM->GetCount());

		strMSG = CTChart::Format( TSTR_FMT_SELLALL,
			pTITEM->GetTITEM()->m_strNAME,
			pTITEM->GetCount());

		m_pMainWnd->CloseMessageBox();
		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ITEM_DO_SELL,
			GM_CLOSE_MSGBOX,
			0,
			pTITEM->GetCount() > 1 ? TRUE : FALSE,
			TRUE);
		EnableChat(FALSE);

		return TERR_NONE;
	}

	if( !bCount )
	{
		CString strEDITTITLE;
		CString strEDIT;
		CString strMSG;

		strEDIT = CTChart::Format( TSTR_FMT_NUMBER, 1);
		strEDITTITLE = CTChart::LoadString( TSTR_EA);

		strMSG = CTChart::Format( TSTR_FMT_SELLONE,
			pTITEM->GetTITEM()->m_strNAME);

		m_pMainWnd->CloseMessageBox();
		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			strEDIT,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_ITEM_DO_SELL,
			GM_CLOSE_MSGBOX,
			0,
			pTITEM->GetCount() > 1 ? TRUE : FALSE,
			TRUE);
		EnableChat(FALSE);

		return TERR_NONE;
	}

	CTShopDlg* pSHOPDLG = (CTShopDlg*) GetFrame( TFRAME_ITEM_SHOP );
	

	if(m_pSESSION)
	{
		if( !pSHOPDLG->m_pOpenByCash )
			m_pSESSION->SendCS_ITEMSELL_REQ(
				m_bInvenID,
				m_bSlotID,
				bCount,
				INVEN_NULL,
				INVALID_SLOT );
		else
		{
			m_pSESSION->SendCS_ITEMSELL_REQ(
				m_bInvenID,
				m_bSlotID,
				bCount,
				pSHOPDLG->m_pOpenByCash->m_bInvenID,
				pSHOPDLG->m_pOpenByCash->m_bSlotID );
		}
	}
	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_1()
{
	BuySKILL(0);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_2()
{
	BuySKILL(1);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_3()
{
	BuySKILL(2);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_4()
{
	BuySKILL(3);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_5()
{
	BuySKILL(4);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_6()
{
	BuySKILL(5);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_7()
{
	BuySKILL(6);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_8()
{
	BuySKILL(7);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_9()
{
	BuySKILL(8);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_10()
{
	BuySKILL(9);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_11()
{
	BuySKILL(10);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_12()
{
	BuySKILL(11);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_13()
{
	BuySKILL(12);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_14()
{
	BuySKILL(13);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_15()
{
	BuySKILL(14);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_16()
{
	BuySKILL(15);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_17()
{
	BuySKILL(16);
	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLLEVEL_18()
{
	BuySKILL(17);
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_INVEN_SLOT()
{
	if( !m_pMainChar->m_bPrivateShop )
	{
		OnGM_TOGGLE_DEFINVEN();
		EnableAllSubInven( m_vTFRAME[TFRAME_DEFINVEN]->IsVisible() );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_MAIN()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_SKILLINFO()
{
	if(m_vTFRAME[TFRAME_SKILL])
		if(m_vTFRAME[TFRAME_SKILL]->IsVisible())
			DisableUI(TFRAME_SKILL);
		else if(m_pSESSION)
			m_pSESSION->SendCS_NPCITEMLIST_REQ(TDEF_SKILL_NPC);

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_QUESTINFO()
{
	CTNewQuestDlg *pTQuestDlg = static_cast<CTNewQuestDlg*>(m_vTFRAME[TFRAME_QUEST]);

	ToggleUI(TFRAME_QUEST);
	pTQuestDlg->ResetTree();

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CHARINFO()
{
	switch( CTCharInfoDlg::m_bTabIndex )
	{
	case TINFO_BASIC:
	case TINFO_MORE:
		ToggleUI(TFRAME_CHAR_INFO);
		break;
	case TINFO_WAR:
	case TINFO_PVP:
		{
			if( m_vTFRAME[ TFRAME_CHAR_PVP_INFO ]->IsVisible() )
				ToggleUI(TFRAME_CHAR_PVP_INFO);
			else
			{
				if( CTCharInfoDlg::m_bTabIndex == TINFO_WAR )
					OnGM_WAR_INFO();
				else
					OnGM_PVP_INFO();
			}
		}
		break;
	case TINFO_RANK:
		{
			if( m_vTFRAME[ TFRAME_RANK_INFO ]->IsVisible() )
				ToggleUI( TFRAME_RANK_INFO );
			else
			{
				if ( CTCharInfoDlg::m_bTabIndex == TINFO_RANK )
					OnGM_RANK_INFO();
			}
		}
		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_COMMUNITY()
{
	ToggleUI(TFRAME_COMMUNITY);
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_DEFINVEN()
{
	ToggleUI(TFRAME_DEFINVEN);
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CMDMAP()
{
	if( m_bTournamentBattle || m_bTournamentWatching )
		return TERR_NONE;

	if(!ResetRSCS(!m_bRSCS))
		CorpsMSG( m_pMainChar, CORPS_NO_PARTY);
	else
		m_pTMinimap->ShowComponent( IsShowMinimap() );

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_HIDE_SQUAD()
{
	if( m_pTRSCS )
	{
		m_pTRSCS->m_bHideSquad = !m_pTRSCS->m_bHideSquad;
		m_pTRSCS->ResetVisible();
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_PETMANAGE()
{
	ToggleUI(TFRAME_PET_MANAGE);

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_HELP()
{
	ToggleUI(TFRAME_HELP);
	return TERR_NONE;
}

int CTClientGame::OnGM_HOTKEY_DOWN()
{
	CTGaugePannel* pDlg = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	CPoint point;
	GetCursorPos(&point);
	m_pMainWnd->ScreenToClient(&point);

	THOTKEY_BASE eHotKeyType;
	if( !pDlg->CheckHotkeyBase(point,&eHotKeyType) )
	{
		if( CTClientKEY::IsKeyDown(VK_LMENU) )
			eHotKeyType = THOTKEYBASE_SUB1;
		else
			eHotKeyType = THOTKEYBASE_DEF;
	}
	
	pDlg->HotkeyPrev(eHotKeyType);

	return TERR_NONE;
}

int CTClientGame::OnGM_HOTKEY_UP()
{
	CTGaugePannel* pDlg = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	CPoint point;
	GetCursorPos(&point);
	m_pMainWnd->ScreenToClient(&point);

	THOTKEY_BASE eHotKeyType;
	if( !pDlg->CheckHotkeyBase(point,&eHotKeyType) )
	{
		if( CTClientKEY::IsKeyDown(VK_LMENU) )
			eHotKeyType = THOTKEYBASE_SUB1;
		else
			eHotKeyType = THOTKEYBASE_DEF;
	}

	pDlg->HotkeyNext(eHotKeyType);

	return TERR_NONE;
}

int CTClientGame::OnGM_ACTIVATE_SWITCH()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_SWITCH ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() ||
		(((CTClientSwitch *) m_pTARGET)->m_pTSWITCH->m_bLockOnClose && !((CTClientSwitch *) m_pTARGET)->m_bOPEN) ||
		(((CTClientSwitch *) m_pTARGET)->m_pTSWITCH->m_bLockOnOpen && ((CTClientSwitch *) m_pTARGET)->m_bOPEN) )
		return TERR_NONE;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TSWITCH_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_ACTIVATE_SWITCH;
		m_bAutoRun = FALSE;
		m_fBOUND = TSWITCH_DIST;
	}
	else
	{
		ResetActOBJ(NULL);

		BeginCharge(
			TBOX_CHARGE_TICK,
			TCHARGETYPE_OPEN);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_TALK()
{
	if( !m_pTARGET ||
		m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() ||
		m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() ||
		m_pMainChar->IsDown() ||
		!m_pMainChar->CanTalkWithNPC( (CTClientNpc*)m_pTARGET ) )
		return TERR_NONE;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_TALK;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else if( !m_pMainChar->IsJump() && !m_pMainChar->IsPush() && !m_pMainChar->IsFall() )
		m_pSESSION->SendCS_NPCTALK_REQ(WORD(m_pTARGET->m_dwID));

	return TERR_NONE;
}

int CTClientGame::OnGM_CRAFT_OK()
{
	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_CRAFT_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_CRAFT_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TCRAFT_PERIOD,
		GM_CRAFT);

	return TERR_NONE;
}

int CTClientGame::OnGM_CRAFT()
{
	CTCraftQuestDlg* pCraft = static_cast< CTCraftQuestDlg* >(m_vTFRAME[TFRAME_CRAFT]);

	m_pSESSION->SendCS_QUESTEXEC_REQ( pCraft->GetQuestID(), 0, 0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_QUEST_ACCEPT()
{
	CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];

	LPTQUEST pTQUEST = NULL;
	if(m_pSESSION)
	{
		LPTREWARD pTREWARD = pTNPCTalkDlg->m_pTSELREWARD;
		pTQUEST = pTNPCTalkDlg->GetCurTQuest();

		if(pTQUEST)
		{
			m_pSESSION->SendCS_QUESTEXEC_REQ(
				pTQUEST->m_dwID,
				pTREWARD ? pTREWARD->m_bType : 0,
				pTREWARD ? pTREWARD->m_dwID : 0);

			if( pTQUEST->m_bType == QT_MISSION )
			{
				MAPOBJECT::iterator itTNPC;

				m_vMAP.m_vNEWQUESTOBJ.clear();
				for( itTNPC = m_vMAP.m_mapTNpc.begin(); itTNPC != m_vMAP.m_mapTNpc.end(); itTNPC++)
					m_vMAP.m_vNEWQUESTOBJ.push_back(((CTClientObjBase *) (*itTNPC).second)->m_dwID);

				CheckQuestOBJ();
			}
		}
	}

	if( pTQUEST )
	{
		CTHelpFrame* pHELPFRAME = (CTHelpFrame*) m_vTFRAME[ TFRAME_HELP ];
		pHELPFRAME->OnQuestEvent( THELPLINK_TRIGGER_ACCEPT, pTQUEST->m_dwID );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_QUEST_GIVEUP()
{
	CTNewQuestDlg *pTQuestDlg = (CTNewQuestDlg *) m_vTFRAME[TFRAME_QUEST];

	if( m_pSESSION && pTQuestDlg->m_pTQUEST )
		m_pSESSION->SendCS_QUESTDROP_REQ(pTQuestDlg->m_pTQUEST->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_EXPAND_QUEST()
{
	return TERR_NONE;
}
	
int CTClientGame::OnGM_EXPAND_NPCQUEST()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_QUEST_CRAFT()
{
	if( !m_pTARGET ||
		m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() ||
		m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() ||
		m_pMainChar->IsDown() ||
		!m_pMainChar->CanTalkWithNPC( (CTClientNpc*)m_pTARGET ) )
		return TERR_NONE;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_QUEST_CRAFT;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else if( !m_pMainChar->IsJump() && !m_pMainChar->IsPush() && !m_pMainChar->IsFall() )
	{
		EnableUI( TFRAME_CRAFT );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_MINIMAP()
{
	if( m_pTRSCS->IsVisible() )
	{
		m_pTRSCS->m_bMinimize = !m_pTRSCS->m_bMinimize;
		m_pTRSCS->ResetVisible();
	}
	else
	{
		m_pTMinimap->m_bMinimize = !m_pTMinimap->m_bMinimize;
		m_pTMinimap->ResetVisible();
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_SIEGE()
{
	CTSiegeUI* pSiege = static_cast< CTSiegeUI* >( m_vTFRAME[TFRAME_SIEGEUI] );

	if( !pSiege )
		return TERR_NONE;

	pSiege->ShowSubComponent( pSiege->GetSubwinHide() );

	return TERR_NONE;
}

int CTClientGame::OnGM_OPEN_MONINVEN()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_MON ||
		!m_pTARGET->IsDead() || !m_pSESSION ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() ||
		m_pMainChar->m_bPrivateShop || m_pMainChar->GetRidingPet() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_OPEN_MONINVEN;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else if( !m_pMainChar->IsJump() && !m_pMainChar->IsPush() && !m_pMainChar->IsFall() )
		SendCS_MONITEMLIST_REQ( TRUE, m_pTARGET->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_JOIN_ITEM_LOTTERY()
{
	CTPartyItemLottery* pLottery = CTPartyItemLottery::GetGMTarget();
	if( pLottery && pLottery->IsActive() && pLottery->GetItem() )
	{
		CTClientItem* pItem = pLottery->GetItem();
		SendCS_MONITEMLOTTERY_REQ(TRUE, pLottery->GetMonID(), pItem->GetItemID());

		CTPartyItemLottery::DeleteInstance(pLottery);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_GIVEUP_ITEM_LOTTERY()
{
	CTPartyItemLottery* pLottery = CTPartyItemLottery::GetGMTarget();
	if( pLottery && pLottery->IsActive() && pLottery->GetItem() )
	{
		CTClientItem* pItem = pLottery->GetItem();
		SendCS_MONITEMLOTTERY_REQ(FALSE, pLottery->GetMonID(), pItem->GetItemID());

		CTPartyItemLottery::DeleteInstance(pLottery);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_GETMONEY()
{
	SendCS_MONMONEYTAKE_REQ(m_pTARGET);
	return TERR_NONE;
}

int CTClientGame::OnGM_GETALL()
{
	if( !m_pTARGET || !m_pSESSION )
		return TERR_NONE;

	if( !m_vTFRAME[TFRAME_MONINVEN]->IsVisible() )
	{
		FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
			m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
			m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

		if( fDist > TARGETUI_DIST )
		{
			m_bMoveType = TMAINMOVE_TARGET;
			m_dwMoveGM = GM_GETALL;
			m_bAutoRun = FALSE;
			m_fBOUND = POPUP_DIST;
		}
		else
			SendCS_MONITEMTAKEALL_REQ(m_pTARGET);
	}
	else if( !m_pMainChar->m_bGhost &&!m_pMainChar->IsDead() &&
			 !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() )
	{
		switch(m_pTARGET->m_bType)
		{
		case OT_NPC	:
			if(!m_vTMONINVEN.m_mapTITEM.empty())
			{
				MAPTITEM::iterator itTITEM = m_vTMONINVEN.m_mapTITEM.begin();

				m_bMonSlotID = (*itTITEM).second->GetItemSlot();
				m_bInvenID = INVALID_SLOT;
				m_bSlotID = INVALID_SLOT;

				OnGM_ITEM_TAKE();
			}

			break;

		case OT_MON	:
			{
				MAPTITEM::iterator itTITEM;

				for( itTITEM = m_vTMONINVEN.m_mapTITEM.begin(); itTITEM != m_vTMONINVEN.m_mapTITEM.end(); itTITEM++)
				{
					m_bMonSlotID = (*itTITEM).second->GetItemSlot();
					m_bInvenID = INVALID_SLOT;
					m_bSlotID = INVALID_SLOT;

					OnGM_ITEM_TAKE();
				}

				OnGM_GETMONEY();
			}

			break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_TAKE()
{
	if( m_pTARGET && m_pSESSION && !m_pMainChar->IsDead() && !m_pMainChar->m_bGhost && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() )
		switch(m_pTARGET->m_bType)
		{
		case OT_NPC	:
			{
				CTClientItem *pTITEM = m_vTMONINVEN.FindTItem(m_bMonSlotID);

				if(pTITEM)
				{
					MAPTQUESTTEMP::iterator itTQUEST;

					for( itTQUEST = m_pTARGET->m_mapTQUESTTEMP.begin(); itTQUEST != m_pTARGET->m_mapTQUESTTEMP.end(); itTQUEST++)
						if( (*itTQUEST).second->m_bType == QT_ROUTING )
						{
							m_pSESSION->SendCS_ITEMBUY_REQ(
								WORD(m_pTARGET->m_dwID),
								(*itTQUEST).second->m_dwID,
								pTITEM->GetTITEM()->m_wItemID,
								1,
								INVEN_NULL,
								INVALID_SLOT);

							m_vMAP.m_vNEWQUESTOBJ.push_back(m_pTARGET->m_dwID);
							CheckQuestOBJ();

							break;
						}
				}

				DisableUI(TFRAME_MONINVEN);
			}

			break;

		case OT_MON	:
			if(m_pTARGET->IsDead())
			{
				SendCS_MONITEMTAKE_REQ(
					m_pTARGET->m_dwID,
					m_bMonSlotID,
					m_bInvenID,
					m_bSlotID);
			}

			break;

		default		: ResetTMonInvenUI(); break;
		}
	else
		ResetTMonInvenUI();

	return TERR_NONE;
}

int CTClientGame::OnGM_MINIMAP_ZOOMOUT()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMIN = 1.0f;

	if(m_vTFRAME[TFRAME_WORLDMAP]->IsVisible())
	{
		CTWorldmapDlg* pWorldDlg = (CTWorldmapDlg*) m_vTFRAME[TFRAME_WORLDMAP];

		if( pWorldDlg->m_fTSCALE == TWORLDMAP_MINSCALE )
		{
			FLOAT fPOSX = m_pMainChar->GetPositionX();
			FLOAT fPOSZ = m_pMainChar->GetPositionZ();

			if( pWorldDlg->GetWorldmapLevel(fPOSX,fPOSZ) == TWORLDMAP_LEVEL_ALL )
				pWorldDlg->m_bTALLWORLDMODE = TRUE;
		}
		else
		{
			pTSCALE = &(pWorldDlg->m_fTSCALE);
			fMIN = TWORLDMAP_MINSCALE;
		}
	}
	//else if(m_bRSCS)
	//{
	//	pTSCALE = &m_pTRSCS->m_fTSCALE;
	//	fMIN = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	//}
	//else
	//{
	//	pTSCALE = &m_vMAP.m_fTSCALE;
	//	fMIN = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	//}

	if( pTSCALE )
		(*pTSCALE) = max( (*pTSCALE) / TMINIMAP_SCALE_FACTOR, fMIN);

	return TERR_NONE;
}

int CTClientGame::OnGM_MINIMAP_ZOOMIN()
{
	FLOAT *pTSCALE = NULL;
	FLOAT fMAX = 1.0f;

	if(m_vTFRAME[TFRAME_WORLDMAP]->IsVisible())
	{
		CTWorldmapDlg* pWorldDlg = (CTWorldmapDlg*) m_vTFRAME[TFRAME_WORLDMAP];

		if( !pWorldDlg->m_bTALLWORLDMODE )
		{
			pTSCALE = &(pWorldDlg->m_fTSCALE);
			fMAX = powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
		}
		else
			pWorldDlg->m_bTALLWORLDMODE = FALSE;
	}
	//else if(m_bRSCS)
	//{
	//	pTSCALE = &m_pTRSCS->m_fTSCALE;
	//	fMAX = TMINIMAP_INIT_SCALE;
	//}
	//else
	//{
	//	pTSCALE = &m_vMAP.m_fTSCALE;
	//	fMAX = TMINIMAP_INIT_SCALE;
	//}

	if( pTSCALE )
		(*pTSCALE) = min( (*pTSCALE) * TMINIMAP_SCALE_FACTOR, fMAX);

	return TERR_NONE;
}

int CTClientGame::OnGM_SELECT_WORLDMAP()
{
	CTWorldmapDlg* pWorldDlg = (CTWorldmapDlg*) m_vTFRAME[TFRAME_WORLDMAP];
	if( pWorldDlg->m_pTLASTWBTN )
	{
		MAPDWORD::iterator itr = CTChart::m_mapTBTNID2UNIT.find(pWorldDlg->m_pTLASTWBTN->m_id);
		if( itr != CTChart::m_mapTBTNID2UNIT.end() )
		{
			WORD wID = LOWORD(itr->second);
			BYTE bUX = LOBYTE(wID);
			BYTE bUZ = HIBYTE(wID);

			FLOAT fLEN = (FLOAT)(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
			FLOAT fHLEN = fLEN * 0.5f;

			pWorldDlg->m_vTCENTER.x = FLOAT(bUX) * fLEN + fHLEN;
			pWorldDlg->m_vTCENTER.y = FLOAT(bUZ) * fLEN + fHLEN;

			pWorldDlg->m_bTALLWORLDMODE = FALSE;
			pWorldDlg->m_pTLASTWBTN = NULL;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_RECALL_PASSIVE_AI()
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
		pTRECALL->m_bAI = TRECALLAI_PASSIVE;

	return TERR_NONE;
}

int CTClientGame::OnGM_RECALL_ACTIVE_AI()
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
		pTRECALL->m_bAI = TRECALLAI_ACTIVE;

	return TERR_NONE;
}

int CTClientGame::OnGM_RECALL_MANUAL_AI()
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
		pTRECALL->m_bAI = TRECALLAI_MANUAL;

	return TERR_NONE;
}

int CTClientGame::OnGM_RECALL_BACK_AI()
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
		pTRECALL->m_bSubAI = TRECALLAI_BACK;

	return TERR_NONE;
}

int CTClientGame::OnGM_RECALL_STAY_AI()
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
		pTRECALL->m_bSubAI = pTRECALL->m_bSubAI == TRECALLAI_STAY ? TRECALLAI_COUNT : TRECALLAI_STAY;

	return TERR_NONE;
}

int CTClientGame::OnGM_DEL_RECALL()
{
	if( m_pTARGET && (m_pTARGET->m_bType == OT_RECALL || m_pTARGET->m_bType == OT_SELF) &&
		IsHostingOBJ( m_pTARGET->m_dwID, m_pTARGET->m_bType) )
	{
		m_pSESSION->SendCS_DELRECALLMON_REQ(m_pTARGET->m_dwID, m_pTARGET->m_bType);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_GOTO_THUMB()
{
	m_bShowingRevivalItemMessageBox = FALSE;
	m_pMainWnd->CloseMessageBox();

	m_vMainDead.m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, m_pMainChar->FindTInven(INVEN_EQUIP)));
	m_vMainDead.SetPosition(m_pMainChar->GetPositionMat());
	m_vMainDead.m_vWorld = m_pMainChar->m_vWorld;

	m_vMainDead.m_strGUILD = m_pMainChar->m_strGUILD;
	m_vMainDead.m_bShoot = m_pMainChar->m_bShoot;
	m_vMainDead.m_bMode = MT_NORMAL;

	m_vMainDead.m_bACTLevel = CTChart::GetTACTION(TA_DEAD)->m_bLevel;
	m_vMainDead.m_bAction = TA_DEAD;

	TACTION vActionID = m_vMainDead.FindActionID(
		m_vMainDead.m_bAction,
		m_vMainDead.GetWeaponID(m_vMainDead.m_bMode));

	m_vMainDead.m_bHideOnCapeMode = CTChart::GetTACTION(TA_DEAD)->m_bHideOnCapeMode;
	m_vMainDead.m_bEquipMode = vActionID.m_bEquipMode;

	m_vMainDead.ResetEQUIP( m_pDevice, m_pRES);
	m_vMainDead.m_mapTINVEN.clear();

	m_pMainChar->m_bACTLevel = CTChart::GetTACTION(TA_STAND)->m_bLevel;
	m_pMainChar->m_bAction = TA_STAND;
	m_pMainChar->m_bMode = MT_NORMAL;
	m_pMainChar->m_bGhost = TRUE;
	m_pMainChar->m_fSpeedFactor = TDEF_SPEED * 1.5f;

	m_pMainChar->m_vTDEAD = D3DXVECTOR3(
		m_pMainChar->GetPositionX(),
		m_pMainChar->GetPositionY(),
		m_pMainChar->GetPositionZ());

	LPTREGIONINFO pTREGION = m_vMAP.GetRegionINFO(
		m_pMainChar->GetPositionX(),
		m_pMainChar->GetPositionZ());

	if( !pTREGION && m_pMainChar->m_pTREGION )
		pTREGION = CTChart::FindTREGIONINFO(m_pMainChar->m_pTREGION->m_dwID);

	if(pTREGION)
	{
		if( m_pMainChar->m_bEnterCastle && m_pMainChar->m_bCamp )
		{
			m_pMainChar->m_vWorld._41 = pTREGION->m_vTHUMB[m_pMainChar->m_bCamp - 1].x;
			m_pMainChar->m_vWorld._42 = pTREGION->m_vTHUMB[m_pMainChar->m_bCamp - 1].y;
			m_pMainChar->m_vWorld._43 = pTREGION->m_vTHUMB[m_pMainChar->m_bCamp - 1].z;
		}
		else
		{
			m_pMainChar->m_vWorld._41 = pTREGION->m_vTHUMB[m_pMainChar->m_bContryID].x;
			m_pMainChar->m_vWorld._42 = pTREGION->m_vTHUMB[m_pMainChar->m_bContryID].y;
			m_pMainChar->m_vWorld._43 = pTREGION->m_vTHUMB[m_pMainChar->m_bContryID].z;
		}
	}
	else
	{
		m_pMainChar->m_vWorld._41 = m_pMainChar->GetPositionX();
		m_pMainChar->m_vWorld._42 = m_pMainChar->GetPositionY();
		m_pMainChar->m_vWorld._43 = m_pMainChar->GetPositionZ();
	}

	SendCS_MOVE_REQ(
		m_vMAP.m_wMapID,
		m_pMainChar->m_vWorld._41,
		m_pMainChar->m_vWorld._42,
		m_pMainChar->m_vWorld._43,
		m_pMainChar->m_wPITCH,
		m_pMainChar->m_wDIR,
		m_pMainChar->m_bMouseDIR,
		m_pMainChar->m_bKeyDIR,
		m_pMainChar->m_bAction,
		m_pMainChar->m_bGhost);

	ReleaseForTELEPORT();
	m_vMAP.ResetTMAP(
		m_pDevice,
		m_pCamera,
		0, TRUE);

	m_vMAP.FixTOBJHeight(m_pMainChar);
	ResetTSKY();
	ResetSkillICON();

	return TERR_NONE;
}

int CTClientGame::OnGM_USEITE_REVIVAL()
{
	m_bShowingRevivalItemMessageBox = FALSE;
	m_dwWaitingRevivalItem = 1000 * 60 * 2; // 2min

	m_pMainWnd->CloseMessageBox();

	BYTE bInven, bSlot;
	if( !FindInven(IT_USE, IK_REVIVAL, bInven, bSlot) )
	{
		OnGM_GOTO_THUMB();
		return TERR_NONE;
	}

	UseTItem(bInven, bSlot);
	return TERR_NONE;
}

int CTClientGame::OnGM_DIVIDE_ITEM()
{
	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return OnGM_DROP_ITEM_NO();

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM || pTITEM->GetWrap() )
		return OnGM_DROP_ITEM_NO();

	TFrame *pTFRAME = (TFrame *) m_pMainWnd->m_MessageBox.FindKid(ID_FRAME_2BTNMSGBOX);
	if( !pTFRAME || !pTFRAME->CanProcess() )
		return OnGM_DROP_ITEM_NO();

	TEdit *pTEDIT = (TEdit *) pTFRAME->FindKid(ID_CTRLINST_EDIT);
	if( !pTEDIT || !pTEDIT->CanProcess() )
		return OnGM_DROP_ITEM_NO();
	int nCount = (BYTE) atoi(pTEDIT->m_strText);

	if( !nCount || nCount < 0 ||
		nCount > pTITEM->GetCount() )
		return OnGM_DROP_ITEM_NO();

	m_pSESSION->SendCS_MOVEITEM_REQ(
		m_bInvenID,
		m_bSlotID,
		m_bTargetInvenID,
		m_bTargetSlotID,
		BYTE(nCount));
	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}


int CTClientGame::OnGM_WARN_GRADE_ITEM_DROP()
{
	CTUpItemSellDlg *pDlg = static_cast<CTUpItemSellDlg*>(m_vTFRAME[ TFRAME_WARN_UPITEM_SELL ] );

	if( !m_nSellCount )
	{
		CString strCNT = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
		m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

		m_nSellCount = ::atoi(strCNT);
	}

	m_pMainWnd->CloseMessageBox();

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return TERR_NONE;

	CTClientItem *pClientITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pClientITEM)
		return TERR_NONE;

	LPTITEM pTITEM = pClientITEM->GetTITEM();
	if(!pTITEM)
		return TERR_NONE;

	if ( CTNationOption::JAPAN )
	{
		pDlg->m_strSecurityCode.Format( "%04d", rand() % 9999 );
		pDlg->m_pMSG->m_strText = CTChart::Format( TSTR_WARN_UPITEM_DROP_SECURITYCODE, pDlg->m_strSecurityCode );
	}
	else
		pDlg->m_pMSG->m_strText = CTChart::Format( TSTR_WARN_UPITEM_DROP );

	pDlg->SetClientItem( pClientITEM );
	pDlg->SetMode( CTUpItemSellDlg::MODE_DROP );

	// m_pMainWnd->DoModalFrame( TFRAME_WARN_UPITEM_SELL );
	pDlg->ShowComponent();
	SwitchFocus( pDlg );

	return TERR_NONE;
}

int CTClientGame::OnGM_DROP_ITEM_YES()
{
	m_pMainWnd->CloseMessageBox();

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return OnGM_DROP_ITEM_NO();

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM)
		return OnGM_DROP_ITEM_NO();

	m_bTargetInvenID = INVEN_NULL;
	m_bTargetSlotID = INVEN_NULL;

	m_pSESSION->SendCS_MOVEITEM_REQ(
		m_bInvenID,
		m_bSlotID,
		INVEN_NULL, 0,
		pTITEM->GetCount());

	PlayUISound(ID_SND_ITEM_DESTROY);

	MAPOBJECT::iterator itTNPC;
	m_vMAP.m_vNEWQUESTOBJ.clear();

	for( itTNPC = m_vMAP.m_mapTNpc.begin(); itTNPC != m_vMAP.m_mapTNpc.end(); itTNPC++)
		m_vMAP.m_vNEWQUESTOBJ.push_back(((CTClientObjBase *) (*itTNPC).second)->m_dwID);
	CheckQuestOBJ();

	return TERR_NONE;
}

int CTClientGame::OnGM_DROP_ITEM_NO()
{
	m_pMainWnd->CloseMessageBox();
	ResetInvenUI();

	return TERR_NONE;
}

int CTClientGame::OnGM_SET_RETURNPOS()
{
	m_pMainWnd->CloseMessageBox();
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
		return TERR_NONE;
	m_pSESSION->SendCS_SETRETURNPOS_REQ(WORD(m_pTARGET->m_dwID));

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CHAT_LOCK()
{
	CTClientUIBase::m_bDragLock = !CTClientUIBase::m_bDragLock;

	m_pChatFrame->SetLock(CTClientUIBase::m_bDragLock);
	m_pChatFrame->ResetTick();

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CHAT_LANG()
{
	CTClientApp::ToggleImeLocalMode();
	m_pChatFrame->NotifyImeLocalModeChange();
	m_pChatFrame->ResetTick();

	return TERR_NONE;
}
 
int CTClientGame::OnGM_TOGGLE_ACTLIST()
{
	m_pActListPopup->ShowComponent( !m_pActListPopup->IsVisible() );

	if( m_pActListPopup->IsVisible() )
	{
		RemoveKid(m_pActListPopup);
		AddKid(m_pActListPopup);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_MESSENGER()
{
	CTFrameGroupBase* pMsgFrame = static_cast<CTFrameGroupBase*>(m_vTFRAME[TFRAME_MESSENGER]);

	if( pMsgFrame->IsVisible() && pMsgFrame->IsSelect(TMESSENGER_FRAME_MESSENGER) )
		DisableUI(TFRAME_MESSENGER);
	else
	{
		if( m_pSESSION )
			m_pSESSION->SendCS_FRIENDLIST_REQ();
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_MAIL()
{
	if( m_vMAP.m_wMapID == TUTORIAL_MAPID )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_TUTORIAL_ERROR_MSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX, 0);

		return TERR_NONE;
	}

	if( m_vTFRAME[TFRAME_MAIL]->IsVisible() )
	{
		DisableUI( TFRAME_MAIL );
	}
	else
	{
		if( m_pSESSION && m_pMailList->m_wCurPage != 0 )
			m_pSESSION->SendCS_POSTLIST_REQ( m_pMailList->m_wCurPage );
		else
		{
			m_pMailList->ClearMail();
			EnableUI( TFRAME_MAIL );
		}
	}
    return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_CHATINFO()
{
	m_vTFRAME[TFRAME_CHAT_DETAIL_INFO]->ShowComponent(FALSE);
	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMUP_READY()
{
	BOOL bUseCash = FALSE;
    CTItemUpDlg* pDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );

	LPTITEM pUpItem = pDlg->GetItem( CTItemUpDlg::TARGET_SLOT );
	LPTITEM pMatItem = pDlg->GetItem( CTItemUpDlg::SCROLL_SLOT );

	if( !pUpItem || !pMatItem )
	{
		if( (pUpItem == NULL) )
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEMUP_WARN_NOITEM,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEMUP_WARN_NOMAT,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
		}
	}
	else
	{
		CString strMSG;
		
		switch( pMatItem->m_bKind )
		{
		case IK_UPGRADE		:
			{
				CTClientItem* pCTItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
				if( !pCTItem )
					return TERR_NONE;

				DWORD dwCost = CTChart::m_vTITEMGRADE[pCTItem->GetGrade()].m_dwCost;
				CString strCost = MakeMoneyStr(dwCost);
				strMSG = CTChart::Format( TSTR_ITEMUP_WARN_UP, strCost);

				if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_PROTECTIONOFUPGRADING ) ||
					(!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE ) &&
					!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE150 ) &&
					!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE200 ) ) )
					bUseCash = TRUE;
			}
			break;

		case IK_DOWNGRADE	:	
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_DOWN); 
			break;
		
		case IK_MAGICGRADE	:	
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_MAGIC); 
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDENCHARTBOOK ) )
				bUseCash = TRUE;
			break;
		
		case IK_RAREGRADE	:	
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_RARE);
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDENCHARTBOOK ) )
				bUseCash = TRUE;
			break;

		case IK_CLEARMAGIC:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_CLEARMAGIC );
			break;

		case IK_WRAP:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_WRAP );
			break;

		case IK_ELD:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_ELD );
			break;

		case IK_CLEARREFINE: //제련초기화
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_CLEARREFINE );
			break;

		case IK_CHGGRADEEFFECT: //강화이펙트변경
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_CHANGEEFFECT );
			break;

		case IK_COLOR:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_WARN_CHANGECOLOR );
			break;
		}

		if ( !bUseCash || m_vMAP.m_wMapID == TUTORIAL_MAPID )
		{
			m_pMainWnd->MessageBoxYesNo(
				strMSG,
				TSTR_YES,
				TSTR_NO,
				GM_ITEMUP_START,
				GM_CLOSE_MSGBOX );
		}
		else
		{
			CheckItemupUseCashItem();
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMUP_CANCEL()
{
	DisableUI(TFRAME_ITEM_UP);
	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMUP_START()
{
	CTItemUpDlg* pItemUp = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );
	CTItemUpGaugeDlg* pUpGauge = static_cast<CTItemUpGaugeDlg*>( m_vTFRAME[TFRAME_ITEM_UP_GAUGE] );

	LPTITEM pTITEM = pItemUp->GetItem( CTItemUpDlg::TARGET_SLOT );
	LPTITEM pSITEM = pItemUp->GetItem( CTItemUpDlg::SCROLL_SLOT );
	
	CTClientItem* pTCLIENTITEM = pItemUp->GetLastClientItem( CTItemUpDlg::TARGET_SLOT );

	if( pTITEM && pSITEM )
	{
		m_pMainWnd->CloseMessageBox();
		pItemUp->ShowComponent(FALSE);

		CPoint pt;
		pItemUp->GetComponentPos(&pt);
		pUpGauge->MoveComponent(pt);
		pUpGauge->m_bShowCancel = TRUE;

		WORD wImgT(0), wImgS(0);
		if( pTITEM )
		{
			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
			wImgT = pTVISUAL? pTVISUAL->m_wIcon: 0;
			
			DWORD qt = pTCLIENTITEM->GetQuality();

			if( pTCLIENTITEM->GetWrap() )
				pUpGauge->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
			else if ( qt & TITEM_QUALITY_GRADE )
				pUpGauge->SetSkinImage( TITEM_INDEX_GRADE_SKIN + pTCLIENTITEM->GetGrade() );
			else if ( qt & TITEM_QUALITY_RARE )
				pUpGauge->SetSkinImage( TITEM_INDEX_RARE_SKIN );
			else if ( qt & TITEM_QUALITY_MAGIC )
				pUpGauge->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
			else 
				pUpGauge->SetSkinImageEmpty();
			
		}
		else 
		{
			pUpGauge->SetSkinImageEmpty();
		}

		if( pSITEM )
		{
			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pSITEM);
			wImgS = pTVISUAL? pTVISUAL->m_wIcon: 0;

			if( pSITEM->m_bKind == IK_COLOR )
			{
				CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( GetFrame( TFRAME_COLOR_PICKER ) );
				pColorPicker->ShowComponent( TRUE );
				pColorPicker->SetBindTarget( pUpGauge );
				pColorPicker->StartProgress();

				pUpGauge->m_bShowCancel = FALSE;
			}
		}

		pUpGauge->SetImage(wImgT,wImgS);
		m_pMainWnd->DoModalFrame(TFRAME_ITEM_UP_GAUGE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_NOTUSE_CASHITEM_CANCEL()
{
	DisableUI( TFRAME_ITEM_UP );
	DisableUI( TFRAME_WARN_NOTUSE_CASHITEM );
	DisableUI( TFRAME_ITEM_REFINE );

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_NOTUSE_CASHITEM_CASH()
{
	DisableUI( TFRAME_ITEM_UP );
	DisableUI( TFRAME_WARN_NOTUSE_CASHITEM );

	if( CTNationOption::USE_WEB_CASHSHOP )
	{
		EnableUI(TFRAME_WEB_CASHSHOP);
	}
	else
	{
		m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ();
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_NOTUSE_CASHITEM_OK()
{
	CTItemUpCashDlg* pCashDlg = static_cast<CTItemUpCashDlg *>( m_vTFRAME[TFRAME_WARN_NOTUSE_CASHITEM] );

	DisableUI( TFRAME_WARN_NOTUSE_CASHITEM );

	if ( pCashDlg->m_nMode == CTItemUpCashDlg::MODE_ITEMUP )
	{
		OnGM_ITEMUP_START();
	}
	else if ( pCashDlg->m_nMode == CTItemUpCashDlg::MODE_REFINE )
	{
		CTItemRefineDlg *pRefineDlg = static_cast<CTItemRefineDlg *>( m_vTFRAME[ TFRAME_ITEM_REFINE ]);

		if ( !pRefineDlg->GetItem(CTItemRefineDlg::TARGET_SLOT) )
			return TERR_NONE;

		if ( !pRefineDlg->GetItem(CTItemRefineDlg::RESOURCE_SLOT1) && !pRefineDlg->GetItem(CTItemRefineDlg::RESOURCE_SLOT2) && !pRefineDlg->GetItem(CTItemRefineDlg::RESOURCE_SLOT3) )
			return TERR_NONE;

		OnGM_REFINE_PROGRESS();
	}

	return TERR_NONE;
}
// =============================================================================================================
int CTClientGame::OnGM_GUILD_REQ()
{
	CString strTarget;

	if( m_pTARGET && m_pTARGET->m_bType == OT_PC && m_pSESSION )
	{
		CTClientChar* pTargetChar = static_cast<CTClientChar*>(m_pTARGET);
		if( !pTargetChar->m_dwGuildID )
			strTarget = m_pTARGET->GetName();

	}
	else if ( m_pChatFrame->GetCandiWhisperTarget() != "" )
	{
		strTarget = m_pChatFrame->GetCandiWhisperTarget();
		m_pChatFrame->SetCandiWhisperTarget("");
	}
	else
		return TERR_NONE;

	m_pSESSION->SendCS_GUILDINVITE_REQ( strTarget );

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_INVITE_TACTICS()
{
	CString strTarget;

	if( !m_strTacticsInvitationTarget.IsEmpty() )
	{
		strTarget = m_strTacticsInvitationTarget;
		m_strTacticsInvitationTarget.Empty();
	}
	else if( m_pTARGET && m_pTARGET->m_bType == OT_PC )
	{
		CTClientChar* pTargetChar = static_cast<CTClientChar*>(m_pTARGET);
		if( !pTargetChar->m_dwTacticsID )
			strTarget = m_pTARGET->GetName();
	}
	else if ( m_pChatFrame->GetCandiWhisperTarget() != "" )
	{
		strTarget = m_pChatFrame->GetCandiWhisperTarget();
		m_pChatFrame->SetCandiWhisperTarget("");
	}
	else
		return TERR_NONE;

	if( !strTarget.IsEmpty() )
	{
		CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );
		pDLG->SetInviteCond( strTarget );
		EnableUI( TFRAME_INVITE_TACTICS );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TACTICS_REQ()
{
	CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );

	pDLG->m_bCloseUI = TRUE;
	DisableUI( TFRAME_INVITE_TACTICS );

	m_pSESSION->SendCS_GUILDTACTICSINVITE_REQ(
		pDLG->m_strTarget,
		(BYTE)pDLG->m_pPeriodEdit->GetValue(),
		pDLG->m_pPointEdit->GetValue(),
		pDLG->m_pRuneEdit->GetValue(),
		pDLG->m_pLunaEdit->GetValue(),
		pDLG->m_pCronEdit->GetValue() );

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_EXIT_WARN()
{
	CString strWARN;
	DWORD dwDAY = GUILD_LEAVE_DURATION / 86400;
	strWARN = CTChart::Format( TSTR_GUILD_EXIT_WARN, dwDAY);

	m_pMainWnd->MessageBoxYesNo(
		strWARN, 
		TSTR_YES,
		TSTR_NO,
		GM_GUILD_EXIT,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}


int CTClientGame::OnGM_GUILD_EXIT()
{
	CTGuildCommander::GetInstance()
		->RequestGuildLeave();

	m_pMainWnd->CloseMessageBox();
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_NORMAL()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDNORMAL);

	return TERR_NONE;
}
int CTClientGame::OnGM_GUILD_MEMBER()
{
	static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY])
		->SetSelect(TCOMMUNITY_FRAME_GUILDMEMBER);
	
	return TERR_NONE;
}
int CTClientGame::OnGM_GUILD_TERRITORY()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDTERRITORY);
	
	return TERR_NONE;
}
int CTClientGame::OnGM_GUILD_MERCENARY()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDVOLUNTEER);
	
	return TERR_NONE;
}

int CTClientGame::OnGM_ESTABLISH_GUILD()
{
	if( !m_pSESSION || m_pMainWnd->m_MessageBox.GetParent() != this )
		return TERR_NONE;

	CString strGUILD = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( strGUILD.IsEmpty() )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();

//	if( CCurse::IsCurse(strGUILD) )
	if( CTStrFilter::CheckFobiddenWords( strGUILD ) )
		return TERR_CURSE_STR;

	m_pSESSION->SendCS_GUILDESTABLISH_REQ(strGUILD);
	
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDINVITE_ACK_YES()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildCommander::GetInstance()
		->RequestGuildInviteAnswer(ASK_YES);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDINVITE_ACK_NO()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildCommander::GetInstance()
		->RequestGuildInviteAnswer(ASK_NO);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDTACTICSINVITE_ACK_YES()
{
	CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_GUILDTACTICSANSWER_REQ(
			ASK_YES,
			pDLG->m_strInviter,
			atoi( (LPCTSTR) pDLG->m_pPeriod->m_strText),
			atoi( (LPCTSTR) pDLG->m_pHornor->m_strText),
			atoi( (LPCTSTR) pDLG->m_pRune->m_strText),
			atoi( (LPCTSTR) pDLG->m_pLuna->m_strText),
			atoi( (LPCTSTR) pDLG->m_pCron->m_strText) );
	}

	pDLG->m_bCloseUI = TRUE;
	DisableUI( TFRAME_INVITE_TACTICS );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDTACTICSINVITE_ACK_NO()
{
	CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_GUILDTACTICSANSWER_REQ(
			ASK_NO,
			pDLG->m_strInviter,
			atoi( (LPCTSTR) pDLG->m_pPeriod->m_strText),
			atoi( (LPCTSTR) pDLG->m_pHornor->m_strText),
			atoi( (LPCTSTR) pDLG->m_pRune->m_strText),
			atoi( (LPCTSTR) pDLG->m_pLuna->m_strText),
			atoi( (LPCTSTR) pDLG->m_pCron->m_strText) );
	}

	pDLG->m_bCloseUI = TRUE;
	DisableUI( TFRAME_INVITE_TACTICS );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDTACTICSINVITE_ACK_BUSY()
{
	CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );
	if( m_pSESSION )
	{
		m_pSESSION->SendCS_GUILDTACTICSANSWER_REQ(
			ASK_BUSY,
			pDLG->m_strInviter,
			atoi( (LPCTSTR) pDLG->m_pPeriod->m_strText),
			atoi( (LPCTSTR) pDLG->m_pHornor->m_strText),
			atoi( (LPCTSTR) pDLG->m_pRune->m_strText),
			atoi( (LPCTSTR) pDLG->m_pLuna->m_strText),
			atoi( (LPCTSTR) pDLG->m_pCron->m_strText) );
	}

	pDLG->m_bCloseUI = TRUE;
	DisableUI( TFRAME_INVITE_TACTICS );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_DONATE_EXP()
{
	CTGuildExpDonDlg* pDlg = static_cast<CTGuildExpDonDlg*>( m_vTFRAME[TFRAME_DONATE_EXP] );
	
	if( pDlg->IsVisible() )
		DisableUI(TFRAME_DONATE_EXP);		
	else if( pDlg->InitValue() )
		EnableUI(TFRAME_DONATE_EXP);

	return TERR_NONE;
}
int CTClientGame::OnGM_GUILD_DONATE_MONEY()
{
	CTGuildMoneyDonDlg* pDlg = static_cast<CTGuildMoneyDonDlg*>( m_vTFRAME[TFRAME_DONATE_MONEY] );
	
	if( pDlg->IsVisible() )
		DisableUI(TFRAME_DONATE_MONEY);		
	else if( pDlg->InitValue() )
		EnableUI(TFRAME_DONATE_MONEY);

	return TERR_NONE;
}
int CTClientGame::OnGM_WRAN_DONATE_EXP()
{
	CTGuildExpDonDlg* pDlg = static_cast<CTGuildExpDonDlg*>( m_vTFRAME[TFRAME_DONATE_EXP] );

	if( pDlg->IsVisible() )
	{
		DWORD dwEXP = pDlg->GetDonationExp();
		
		CString strMSG;
		strMSG = CTChart::Format( TSTR_WRAN_DONATE_EXP, dwEXP);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_OK,
			TSTR_CANCEL,
			GM_DO_DONATE_EXP,
			GM_CLOSE_MSGBOX );
	}

	return TERR_NONE;
}
int CTClientGame::OnGM_DO_DONATE_EXP()
{
	if( !m_pSESSION )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();
	
	CTGuildExpDonDlg* pDlg = static_cast<CTGuildExpDonDlg*>( m_vTFRAME[TFRAME_DONATE_EXP] );
	DWORD dwEXP = pDlg->GetDonationExp();

	CTGuildCommander::GetInstance()
		->RequestGuildDonationExp(dwEXP);

	DisableUI(TFRAME_DONATE_EXP);

	return TERR_NONE;
}
int CTClientGame::OnGM_WRAN_DONATE_MONEY()
{
	CTGuildMoneyDonDlg* pDlg = static_cast<CTGuildMoneyDonDlg*>( m_vTFRAME[TFRAME_DONATE_MONEY] );

	if( pDlg->IsVisible() )
	{
		DWORD dwRune,dwLuna,dwCron;
		pDlg->GetDonationMoney(dwRune,dwLuna,dwCron);
		
		CString strMSG;
		strMSG = CTChart::Format( TSTR_WRAN_DONATE_MONEY, dwRune,dwLuna,dwCron);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_OK,
			TSTR_CANCEL,
			GM_DO_DONATE_MONEY,
			GM_CLOSE_MSGBOX );
	}

	return TERR_NONE;
}
int CTClientGame::OnGM_DO_DONATE_MONEY()
{
	if( !m_pSESSION )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();
	
	CTGuildMoneyDonDlg* pDlg = static_cast<CTGuildMoneyDonDlg*>( m_vTFRAME[TFRAME_DONATE_MONEY] );
	
	DWORD dwRune,dwLuna,dwCron;
	pDlg->GetDonationMoney(dwRune,dwLuna,dwCron);
	
	CTGuildCommander::GetInstance()
		->RequestGuildDonationMoney(dwRune,dwLuna,dwCron);

	DisableUI(TFRAME_DONATE_MONEY);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_NOTIFY()
{
	EnableUI(TFRAME_GUILDNOTIFY);
	return TERR_NONE;
}
	
int CTClientGame::OnGM_GUILD_COMMAND()
{
	EnableUI(TFRAME_GUILDCMD);
	return TERR_NONE;
}

int CTClientGame::OnGM_REG_GUILDMARK()
{
	CTGuildMarkMakeDlg* pDlg = static_cast<CTGuildMarkMakeDlg*>( m_vTFRAME[TFRAME_GUILDMARK] );
	CTClientGuildMark* pGuildMark = pDlg->GetGuildMark();
	if( !pGuildMark )
		return NULL;

	DisableUI(TFRAME_GUILDMARK);

	CTGuildCommander::GetInstance()
		->RequestGuildMarkReg(pGuildMark);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDKICKOUT()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildCommander::GetInstance()
		->RequestGuildKickOut("",FALSE);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDTACTICSKICKOUT()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildCommander::GetInstance()
		->RequestGuildTacticsKickOut("", 0, FALSE);

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDCAB_GETITEM()
{
	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_GUILDCABINET]);
	CTCabinetItem* pCabItem = pDlg->GetSelectCabItem();

	if( pCabItem && m_pSESSION )
	{
		m_pSESSION->SendCS_GUILDCABINETTAKEOUT_REQ(
			pCabItem->m_dwStItemID,
			pCabItem->m_bItemCnt,
			INVEN_DEFAULT,
			0);
	}
	
	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_NOTIFY()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->StartNew();

	return TERR_NONE;
}

int CTClientGame::OnGM_MOD_NOTIFY()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->StartModify();

	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_NOTIFY_OK()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->EndNew(TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_NOTIFY_CANCEL()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->EndNew(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnGM_MOD_NOTIFY_OK()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->EndModify(TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_MOD_NOTIFY_CANCEL()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->EndModify(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnGM_DELETE_NOTIFY()
{
	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] );
	pDlg->Delete();

	return TERR_NONE;
}
// =============================================================================================================
int CTClientGame::OnGM_GINV_APP()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->Application();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_EDIT()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->StartMod();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_NEW()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->StartNew();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_DEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->Delete();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_MOD_OK()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->EndMod(TRUE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_NEW_OK()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->EndNew(TRUE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_MOD_CANCEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->EndMod(FALSE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GINV_NEW_CANCEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->EndNew(FALSE);*/

	return TERR_NONE;
}
// =============================================================================================================
int CTClientGame::OnGM_GSEEK_INV()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->Invite();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_EDIT()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->StartMod();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_NEW()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->StartNew();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_DEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->Delete();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_MOD_OK()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->EndMod(TRUE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_NEW_OK()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->EndNew(TRUE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_MOD_CANCEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->EndMod(FALSE);*/

	return TERR_NONE;
}

int CTClientGame::OnGM_GSEEK_NEW_CANCEL()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->EndNew(FALSE);*/

	return TERR_NONE;
}
// =============================================================================================================
int CTClientGame::OnGM_FORCE_NEW_GUILDOFFER()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildOfferFrame* pFrame = static_cast<CTGuildOfferFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_OFFER) );
	pFrame->ForceEndNew();

	m_pMainWnd->CloseMessageBox();*/

	return TERR_NONE;
}

int CTClientGame::OnGM_FORCE_NEW_GUILDSEEK()
{
/*	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[TFRAME_GUILDAPP] );
	CTGuildSeekFrame* pFrame = static_cast<CTGuildSeekFrame*>( pDlg->GetInnerFrame(TGUILDAPP_FRAME_SEEK) );
	pFrame->ForceEndNew();

	m_pMainWnd->CloseMessageBox();*/

	return TERR_NONE;
}
// =============================================================================================================
int CTClientGame::OnGM_GMP_WHISPER()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_WHISPER();
}

int CTClientGame::OnGM_GMP_MAIL()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_MAIL();
}

int CTClientGame::OnGM_GMP_PARTY()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PARTY();
}

int CTClientGame::OnGM_GMP_FRIEND()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_FRIEND();
}

int CTClientGame::OnGM_GMP_KICK()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_KICK();
}

int CTClientGame::OnGM_GMP_DISORG_TACTICS()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_DISORG_TACTICS();
}

int CTClientGame::OnGM_GMP_ET_GM()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_ET_GM();
}
	
int CTClientGame::OnGM_GMP_SET_GSM()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_SET_GSM();
}
	
int CTClientGame::OnGM_GMP_CANCEL_GSM()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_CANCEL_GSM();
}
	
int CTClientGame::OnGM_GMP_PEER_BARON()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PEER_BARON();
}
	
int CTClientGame::OnGM_GMP_PEER_VISCOUNT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PEER_VISCOUNT();
}

int CTClientGame::OnGM_GMP_PEER_COUNT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PEER_COUNT();
}
	
int CTClientGame::OnGM_GMP_PEER_MARQUIS()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PEER_MARQUIS();
}

int CTClientGame::OnGM_GMP_PEER_DUKE()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_PEER_DUKE();
}

int CTClientGame::OnGM_GMP_CANCEL_PEER()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_CANCEL_PEER();
}

int CTClientGame::OnGM_GMP_POINT_REWARD()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[ TFRAME_COMMUNITY ] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_POINT_REWARD();
}

int CTClientGame::OnGM_GMP_CANCEL()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_CANCEL();
}

int CTClientGame::OnGM_GMP_VOLUNTEER_HESED()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_VOLUNTEER_HESED();
}
int CTClientGame::OnGM_GMP_VOLUNTEER_ARDRI()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_VOLUNTEER_ARDRI();
}
int CTClientGame::OnGM_GMP_VOLUNTEER_TYCON()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_VOLUNTEER_TYCON();
}
int CTClientGame::OnGM_GMP_VOLUNTEER_GEHBRA()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_VOLUNTEER_GEHBRA();
}
int CTClientGame::OnGM_GMP_VOLUNTEER_CANCEL()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_VOLUNTEER_CANCEL();
}
// =============================================================================================================
int CTClientGame::OnGM_GVP_WHISPER()
{
//	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
//	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
//	return pFrame->OnGM_GVP_WHISPER();
	return TERR_NONE;
}

int CTClientGame::OnGM_GVP_MAIL()
{
//	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
//	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
//	return pFrame->OnGM_GVP_MAIL();
	return TERR_NONE;
}

int CTClientGame::OnGM_GVP_INVITE_YES()
{
//	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
//	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
//	return pFrame->OnGM_GVP_INVITE_YES();
	return TERR_NONE;
}

int CTClientGame::OnGM_GVP_INVITE_NO()
{
//	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
//	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
//	return pFrame->OnGM_GVP_INVITE_NO();
	return TERR_NONE;
}

int CTClientGame::OnGM_GVP_CANCEL()
{
//	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
//	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
//	return pFrame->OnGM_GVP_CANCEL();
	return TERR_NONE;
}
// =============================================================================================================

int CTClientGame::OnGM_OPEN_PRIVSHOP_FOR_BUY()
{
	if( !m_pTARGET ||
		m_pTARGET->m_bType != OT_PC ||
		m_pMainChar->IsDead() ||
		m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() ||
		m_pMainChar->IsDown() )
		return TERR_NONE;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_OPEN_PRIVSHOP_FOR_BUY;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else if( !m_pMainChar->IsJump() && !m_pMainChar->IsPush() && !m_pMainChar->IsFall() )
		if( m_pTARGET && m_pTARGET->m_bType == OT_PC && m_pSESSION )
			m_pSESSION->SendCS_STOREITEMLIST_REQ(m_pTARGET->GetName());

	return TERR_NONE;
}
int CTClientGame::OnGM_OPEN_PRIVSHOP_FOR_SELL()
{
	EnableUI(TFRAME_PRIVATE_SELL);
	return TERR_NONE;
}
int CTClientGame::OnGM_PRIVATE_SHOP_START()
{
	CTPrivateShopDlgForSell* pShopDlg = static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);
	
	if( pShopDlg->IsSelling() )
		return TERR_NONE;

	CString strShopName;
	CString strText = pShopDlg->GetShopName();
	CString info = BuildMBCSInfo( strText);

	INT nMaxBytes = 60;
	for( INT i=0 ; i < info.GetLength() ; ++i )
	{
		CHAR c = info.GetAt( i );
		if( IS_MBCS_LEAD(c) )
		{
			strShopName.AppendChar( strText.GetAt( i ) );
			strShopName.AppendChar( strText.GetAt( i+1) );
			++i;
			nMaxBytes -= 2;
		}
		else
		{
			strShopName.AppendChar( strText.GetAt( i ) );
			nMaxBytes -= 1;
		}

		if( nMaxBytes <= 0 )
			break;
	}

	if( strShopName.IsEmpty() )
	{
		CTClientWnd* pWnd = CTClientWnd::GetInstance();
		pWnd->MessageBoxOK(
			TSTR_PRVSHOP_SELL_NOSHOPNAME, 
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );

		return TERR_NONE;
	}

//	if( CCurse::IsCurse(strShopName) )
	if( CTStrFilter::CheckFobiddenWords( strShopName ) )
		return TERR_CURSE_STR;

	BYTE bCount = (BYTE) pShopDlg->GetItemCount();
	LPPRVSHOP_ITEM item = new PRVSHOP_ITEM[bCount];

	for(BYTE i=0; i<bCount; ++i)
	{
		CTPrivateShopItem* pInfo = pShopDlg->GetItem(i);
		item[i].bItem = pInfo->m_bNetID;
		item[i].dwRune = pInfo->m_dwRune;
		item[i].dwLuna = pInfo->m_dwLuna = min( pInfo->m_dwLuna, 999 );
		item[i].dwCron = pInfo->m_dwCron = min( pInfo->m_dwCron, 999 );
		item[i].bInvenID = pInfo->m_bInven;
		item[i].bItemID = pInfo->m_bInvenSlot;
		item[i].bItemCount = pInfo->m_bItemCnt;
	}

	m_pSESSION->SendCS_STOREOPEN_REQ( strShopName, bCount, item );

	delete [] item;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_bAutoRun = FALSE;
	m_dwMoveGM = 0;

	return TERR_NONE;
}
int CTClientGame::OnGM_PRIVATE_SHOP_END()
{
	m_pSESSION->SendCS_STORECLOSE_REQ();

	return TERR_NONE;
}
int CTClientGame::OnGM_PRIVATE_SHOP_BUY()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_PC && !m_pSESSION )
		return TERR_NONE;

	CTClientChar* pTargetChar = static_cast<CTClientChar*>( m_pTARGET );
	if( !pTargetChar->m_bPrivateShop )
		return TERR_NONE;

	CTPrivateShopDlg* pShopDlg = static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_BUY]);
	CTPrivateShopItem* pItemInfo = pShopDlg->GetCurSelItem();
	if( pItemInfo && pItemInfo->m_bItemCnt == 1 )
	{
		m_pSESSION->SendCS_STOREITEMBUY_REQ(pTargetChar->GetName(), pItemInfo->m_bNetID, 1);
	}
	else if( pItemInfo )
	{
		CString strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PRICE,
			pItemInfo->m_pClientItem->GetTITEM()->m_strNAME,
			MakeMoneyStr( pItemInfo->m_dwRune, pItemInfo->m_dwLuna, pItemInfo->m_dwCron ) );
		strMSG += CTChart::Format( TSTR_FMT_BUYITEM_STACK, pItemInfo->m_bItemCnt );

		CString strEDITTITLE = CTChart::LoadString( TSTR_EA);

		m_pMainWnd->CloseMessageBox();
		m_pMainWnd->MessageBox(
			strMSG,
			strEDITTITLE,
			CString("1"),
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_PRIVATE_SHOP_BUY_ITEMS,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);

		EnableChat(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PRIVATE_SHOP_BUY_ITEMS()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_PC && !m_pSESSION )
		return TERR_NONE;

	CTClientChar* pTargetChar = static_cast<CTClientChar*>( m_pTARGET );
	if( !pTargetChar->m_bPrivateShop )
		return TERR_NONE;

	CString strCNT = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	DWORD nCnt = ::atol((LPCSTR)strCNT);
	if( !nCnt )
		return TERR_NONE;

	CTPrivateShopDlg* pShopDlg = static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_BUY]);
	CTPrivateShopItem* pItemInfo = pShopDlg->GetCurSelItem();
	if( pItemInfo )
		m_pSESSION->SendCS_STOREITEMBUY_REQ(pTargetChar->GetName(), pItemInfo->m_bNetID, (BYTE)nCnt);

	m_pMainWnd->CloseMessageBox();
	return TERR_NONE;
}

int CTClientGame::OnGM_REG_ITEM_PRVSHOP()
{
	CTItemRegBox* pBox = static_cast<CTItemRegBox*>(m_vTFRAME[TFRAME_ITEM_REGBOX]);
	if( pBox->GetCount() < 0 )
		return TERR_NONE;

	CTPrivateShopDlgForSell* pPrvShop = (CTPrivateShopDlgForSell*) m_vTFRAME[TFRAME_PRIVATE_SELL];

	INT nIdx = pPrvShop->AddItem( pBox->GetItem(), pBox->GetInven(), pBox->GetInvenSlot() );
	CTPrivateShopItem* pInfo = pPrvShop->GetItem(nIdx);

	pInfo->m_bItemCnt = (BYTE) min( pBox->GetCount(), pBox->GetItem()->GetCount() );
	pInfo->m_dwRune = pBox->GetRune();
	pInfo->m_dwLuna = pBox->GetLuna();
	pInfo->m_dwCron = pBox->GetCron();

	pPrvShop->NotifyUpdate();

	DisableUI(TFRAME_ITEM_REGBOX);
	return TERR_NONE;
}

int CTClientGame::OnGM_FRIEND_REQ()
{
	CString strTarget;

	if( m_pTARGET && m_pTARGET->m_bType == OT_PC && m_pSESSION )
	{
		CTClientChar* pTargetChar = static_cast<CTClientChar*>( m_pTARGET );
		strTarget = pTargetChar->m_strNAME;
	}
	else if ( m_pChatFrame->GetCandiWhisperTarget() != "" )
	{
		strTarget = m_pChatFrame->GetCandiWhisperTarget();
		m_pChatFrame->SetCandiWhisperTarget("");
	}
	else
		return TERR_NONE;


	CTMessengerCommander::GetInstance()
		->DoAddFriend( strTarget );

	return TERR_NONE;
}

int CTClientGame::OnGM_FARFRIEND_REQ()
{
	if( !m_pTPARTY || !m_pSESSION )
		return TERR_NONE;

	CTMessengerCommander::GetInstance()
		->DoAddFriend(m_pTPARTY->m_strNAME);

	return TERR_NONE;
}

int CTClientGame::OnGM_ADD_FRIEND()
{
	CString strCharName = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->CloseMessageBox();
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX );

	if( strCharName.IsEmpty() )
	{
		CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
		pList->HideAllPopup();

		m_pMainWnd->MessageBox(
			TSTR_MBOX_ADD_FRIEND,
			TSTR_NAME,
			0,
			TSTR_OK,TSTR_CANCEL,
			ID_FRAME_2BTNMSGBOX,
			GM_ADD_FRIEND,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);
	}
	else
	{
		CTMessengerCommander::GetInstance()
			->DoAddFriend(strCharName);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_FRIENDINVITE_ACK_YES()
{
	m_pMainWnd->CloseMessageBox();

	CTMessengerCommander::GetInstance()
		->DoFriendInviteAnswer(ASK_YES);

	return TERR_NONE;
}
int CTClientGame::OnGM_FRIENDINVITE_ACK_NO()
{
	m_pMainWnd->CloseMessageBox();

	CTMessengerCommander::GetInstance()
		->DoFriendInviteAnswer(ASK_NO);

	return TERR_NONE;
}

int CTClientGame::OnGM_ADDTOBLOCKLIST()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CString strName = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( strName.IsEmpty() )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();
	CTMessengerCommander::GetInstance()->DoAddBlock(strName);

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_TARGETMAIL()
{
	if( !CanUseMail() )
		return TERR_MAIL_REGION;

	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem )
	{
		CTMailSendDlg* pSndDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);
		pSndDlg->SetReceiver(pItem->GetNameComp()->m_strText);
		pSndDlg->SetTitle("");

		EnableUI(TFRAME_MAIL_SEND);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_GROUPLIST_POPUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->ShowPopup(CTMsgGroupList::POPUPTYPE_GROUPLIST);

	return TERR_NONE;
}

int CTClientGame::OnGM_ADDTOBLOCKLIST_TARGET()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem )
	{
		CTMessengerCommander::GetInstance()->DoAddBlock(
			pItem->GetNameComp()->m_strText);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_DELETE_FRIENDITEM()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem )
	{
		CTMessengerCommander* pCmd = CTMessengerCommander::GetInstance();
		if( pItem->GetType() == CTMsgGroupList::ITYPE_MEMBER )
			pCmd->DoEraseFriend(pItem->m_dwID);
		else
			pCmd->DoEraseFriendGroup((BYTE)pItem->m_dwID);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_MESSENGER_GROUPCHAT()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem && pItem->GetType() == CTMsgGroupList::ITYPE_GROUP )
		CTMessengerCommander::GetInstance()->DoInviteChatGroup(0, (BYTE)pItem->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_ASK_NEW_FRIEND_GROUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	m_pMainWnd->MessageBox(
		TSTR_MBOX_NEWFRIENDGROUP,
		TSTR_NAME,
		0,
		TSTR_YES,
		TSTR_NO,
		ID_FRAME_2BTNMSGBOX,
		GM_NEW_FRIEND_GROUP,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_FRIEND_GROUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CString strName = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->CloseMessageBox();
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( strName == CTChart::LoadString(TSTR_GROUP_ETC) ||
		strName == CTChart::LoadString(TSTR_GROUP_SOULMATE) )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_GROUP_NONAME,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );
	}
	else
	{
		CTMessengerCommander::GetInstance()->DoMakeFriendGroup(strName);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ASK_CHANGE_FRIEND_GROUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	m_pMainWnd->MessageBox(
		TSTR_MBOX_CHANGEFRIENDGROUP,
		0,0,
		TSTR_YES,
		TSTR_NO,
		ID_FRAME_2BTNMSGBOX,
		GM_CHANGE_FRIEND_GROUP,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_CHANGE_FRIEND_GROUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem && pItem->GetType() == CTMsgGroupList::ITYPE_GROUP )
	{
		CString strName = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
		m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

		CTMessengerCommander::GetInstance()->DoFriendGroupName((BYTE)pItem->m_dwID, strName);
	}

	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_MOVE_FRIEND_GROUP()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem && pItem->GetType() == CTMsgGroupList::ITYPE_MEMBER )
	{
		CTMiniPopupDlg* pPopup = pList->GetPopup(CTMsgGroupList::POPUPTYPE_GROUPLIST);
		INT nGrpNum = pPopup->GetLastSelIndex();
		INT nItemCnt = pList->GetItemCount();
		for(INT i(0),g(0); i<nItemCnt; ++i)
		{
			CTMsgGroupList::CItem* pCurItem = pList->GetItem(i);
			if( pCurItem->GetType() == CTMsgGroupList::ITYPE_GROUP &&
				pCurItem->m_bSoulmate == FALSE &&
				pCurItem->m_bEtcGroup == FALSE )
			{
				if( g == nGrpNum )
				{
					CTMessengerCommander::GetInstance()
						->DoChangeFriendGroup(pItem->m_dwID, (BYTE)pCurItem->m_dwID);
					
					break;
				}
				else
					++g;
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SEND_MAIL()
{
	if( !m_pSESSION )
		return TERR_NONE;

	if( m_vTFRAME[TFRAME_TRADE]->IsVisible() )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_MIALERR_TRADING,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );

		return TERR_NONE;
	}

	CTMailSendDlg* pDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);

	CString strReceiver = pDlg->GetReceiver();
	if( strReceiver.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_MIALERR_NORECEIVER,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );

		return TERR_NONE;
	}
	CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();

	m_pSESSION->SendCS_POSTSEND_REQ(
		strReceiver,
		pTTextLinker->MakeLinkToNetText( pDlg->GetTitleEdit(), TRUE, MAX_BOARD_TITLE),
		pTTextLinker->MakeLinkToNetText( pDlg->GetMessageEdit(), TRUE, MAX_BOARD_TEXT),
		pDlg->GetCurrentMailType(),
		pDlg->GetRune(),
		pDlg->GetLuna(),
		pDlg->GetCron(),
		pDlg->GetInvenID(),
		pDlg->GetInvenSlot());

	// 두가지 경로로 GM_SEND_MAIL을 사용한다.
	// 1. 마을지역(PC방 사용자인 경우 어느지역에서든)에서 하단의 편지보내기 버튼을 눌러서 보낼때
	// 2. 편지보내기 캐쉬아이템을 사용해서 어느지역에서든 보낼때.
	// 2번의 경우 m_bSendByCashItem이 1로 설정되어 있다.

	if( pDlg->m_bSendByCashItem)
		pDlg->m_bSendByCashItem = 2; // 0:none 1:will send 2:sended

	DisableUI(TFRAME_MAIL_SEND);
	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_MAILREPLY()
{
	if( !CanUseMail() )
		return TERR_MAIL_REGION;

	CTMailRecvDlg* pRecvDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
	LPTMAIL_SIMPLE pMail = pRecvDlg->GetMailSimple();
	if( !pMail )
		return TERR_NONE;

	CString strSender = pMail->m_strSender;
	CString strSenderTitle = pMail->m_strTitle;

	DisableUI(TFRAME_MAIL_RECV);
	EnableUI(TFRAME_MAIL_SEND);

	CTMailSendDlg* pSndDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);
	pSndDlg->SetReceiver(strSender);

	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
	CString strTitle;

	strTitle = CTChart::Format( TSTR_FMT_MAIL_REPLY, _T(""));
	strTitle = pTextLinker->BuildNetString( _T(""), strTitle);
	strTitle = pTextLinker->AppendNetString( strTitle, strSenderTitle);
	pSndDlg->GetTitleEdit()->ResetTextSetting();
	strTitle = pTextLinker->MakeNetToSimpleLinkText( pSndDlg->GetTitleEdit(), strTitle);
	pSndDlg->SetTitle(strTitle);

	return TERR_NONE;
}

int CTClientGame::OnGM_DELETE_MAIL()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTMailRecvDlg* pRecvDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
	LPTMAIL_SIMPLE pMail = pRecvDlg->GetMailSimple();

	if( pMail )
		m_pSESSION->SendCS_POSTDEL_REQ( pMail->m_dwPostID );

	return TERR_NONE;
}

int CTClientGame::OnGM_MESSENGER_DELETE_MAIL()
{
	if( !m_pSESSION )
		return TERR_NONE;

	INT nSelIdx = m_pMailList->GetSelectedIndex();
	if( nSelIdx != T_INVALID )
	{
		LPTMAIL_SIMPLE pMail = m_pMailList->GetMailSimple(nSelIdx);
		if( pMail )
			m_pSESSION->SendCS_POSTDEL_REQ( pMail->m_dwPostID );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_MAILBILL_PAY()
{
	static_cast<CTMailAskMoneyDlg*>(m_vTFRAME[TFRAME_MAIL_ASK_MONEY])->AnswerPay(TRUE);
	DisableUI(TFRAME_MAIL_ASK_MONEY);

	return TERR_NONE;
}

int CTClientGame::OnGM_MAILBILL_RETURN()
{
	static_cast<CTMailAskMoneyDlg*>(m_vTFRAME[TFRAME_MAIL_ASK_MONEY])->AnswerPay(FALSE);
	DisableUI(TFRAME_MAIL_ASK_MONEY);

	return TERR_NONE;
}

int CTClientGame::OnGM_MAIL_GETITEM()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTMailRecvDlg* pRecvDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
	LPTMAIL_SIMPLE pMail = pRecvDlg->GetMailSimple();
	if( pMail )
		m_pSESSION->SendCS_POSTGETITEM_REQ( pMail->m_dwPostID );

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_RECVMAIL()
{
	if( CanUseMail() )
	{
		INT nSelIdx = m_pMailList->GetSelectedIndex();
		if( nSelIdx != T_INVALID )
			m_pMailList->ViewMail(nSelIdx);
	}
	else
		return TERR_MAIL_REGION;

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_NEWMAIL()
{
	if( CanUseMail() )
		EnableUI(TFRAME_MAIL_SEND);
	else
		return TERR_MAIL_REGION;

	return TERR_NONE;
}

int CTClientGame::OnGM_NEW_MESSENGER_CHAT()
{
	CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( pItem && pItem->GetType() == CTMsgGroupList::ITYPE_MEMBER &&
		static_cast<CTMsgGroupList::CMember*>(pItem)->m_bOnLine )
	{
		const CString& strTarget = pItem->GetNameComp()->m_strText;
		CTMessengerChat* pChat = CTMessengerChat::FindInP2PByTarget(strTarget);
		if( pChat )
		{
			pChat->ShowComponent(TRUE);
			TComponent* pParent = pChat->GetParent();
			if( pParent )
			{
				pParent->RemoveKid(pChat);
				pParent->AddKid(pChat);
			}
		}
		else
		{
			VECTORDWORD vIDS;
			vIDS.push_back(pItem->m_dwID);

			CTMessengerCommander::GetInstance()
				->DoInviteChatTarget(0, vIDS);
		}
	}
	
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_BLOCKLIST()
{
	/*CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
	pList->HideAllPopup();

    ToggleUI(TFRAME_BLOCKLIST);*/
	return TERR_NONE;
}

int CTClientGame::OnGM_MESSENGERCHAT_CLOSE()
{
	CTMessengerChat* pChat = CTMessengerChat::GetCurMsgChat();
	if( pChat )
		CTMessengerCommander::GetInstance()->DoChatOut(pChat);

	return TERR_NONE;
}

int CTClientGame::OnGM_PET_NEWMAKE()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CString strPetName = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( !strPetName.IsEmpty() )
	{
//		if( CCurse::IsCurse(strPetName) )
		if( CTStrFilter::CheckFobiddenWords( strPetName ) )
			return TERR_CURSE_STR;

		CTPetMakeDlg* pDlg = static_cast<CTPetMakeDlg*>(m_vTFRAME[TFRAME_PET_MAKE]);

		m_pSESSION->SendCS_PETMAKE_REQ(
			pDlg->GetInvenID(),
			pDlg->GetInvenSlot(),
			strPetName);

		m_pMainWnd->CloseMessageBox();
		DisableUI(TFRAME_PET_MAKE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PET_NAMEBOX()
{
	m_pMainWnd->MessageBox(
		TSTR_PET_NAMEBOX_MSG,
		TSTR_PET_NAMEBOX_EDIT,
		0,
		TSTR_YES,
		TSTR_NO,
		ID_FRAME_2BTNMSGBOX,
		GM_PET_NEWMAKE,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_PET_RECALL()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTPetManageDlg* pDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
	CTClientPet* pPet = pDlg->GetSelectPet();
	if( pPet )
	{
		CTClientPet* pMainPet = GetMainPet();
		if( pMainPet && pMainPet->GetPetID() == pPet->GetPetID() )
		{
			m_pSESSION->SendCS_PETCANCEL_REQ();
			//m_pSESSION->SendCS_DELRECALLMON_REQ(pMainPet->m_dwID,pMainPet->m_bType);
		}
		else if( pPet->GetPetEndTime() != CTime(0) &&
				CTime( pPet->GetPetEndTime()) < CTClientApp::m_dCurDate )
		{
			m_pSESSION->SendCS_PETDEL_REQ( pPet->GetPetID() );
		}
		else if( pDlg->IsRecalling() )
		{
			m_pSESSION->SendCS_PETCANCEL_REQ();
			//m_pSESSION->SendCS_DELRECALLMON_REQ(pPet->m_dwID,pPet->m_bType);
		}
		else
		{
			if( !pDlg->IsRecalling() ) // SetRecalling(), IsRecalling() 함수는 CS_PETRECALL_REQ패킷의 연이은 중복전송을 막는 역활.
			{
				m_pSESSION->SendCS_PETRECALL_REQ(pPet->GetPetID());
				pDlg->SetRecalling(TRUE);
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_MENU_RIDEPET()
{
	if( !m_pSESSION || m_pMainChar->IsDead() )
		return TERR_NONE;

	CTClientPet* pPet = GetMainPet();
	if( !pPet )
		return TERR_NONE;

	BOOL bRide = pPet->GetTakeUpChar()? FALSE: TRUE;
	if( bRide )
	{
		if( m_pMainChar->IsJump() || m_pMainChar->IsDown() || m_pMainChar->IsFall() )
			return TERR_NONE;
		
		if( m_pMainChar->m_bMode != MT_NORMAL )
			m_pSESSION->SendCS_CHGMODE_REQ(MT_NORMAL);
	}
	else
	{
		if( pPet->IsJump() || pPet->IsDown() || pPet->IsFall() )
			return TERR_NONE;		
	}

	if( m_pSESSION && pPet->m_dwID )
		m_pSESSION->SendCS_PETRIDING_REQ(pPet->m_dwID, bRide);

	return TERR_NONE;
}

int CTClientGame::OnGM_MENU_PETMANAGE()
{
	EnableUI(TFRAME_PET_MANAGE);
	return TERR_NONE;
}

int CTClientGame::OnGM_SELECT_TEXTURE_DETAIL()
{
	BOOL bChange = FALSE;

	for( BYTE i=0; i<TEXTURE_DETAIL_COUNT; ++i )
	{
		if( m_pDevice->m_option.m_nTextureDetail != i && m_pSystemSetting->m_pTTEXDETAIL[i]->IsStateDown() )
		{
			bChange = TRUE;
			m_pDevice->m_option.m_nTextureDetail = (TEXTURE_DETAIL_LEVEL)i;
			break;
		}
	}

	for( i=0; i<TEXTURE_DETAIL_COUNT; ++i )
		m_pSystemSetting->m_pTTEXDETAIL[i]->Select( i == m_pDevice->m_option.m_nTextureDetail );

	if( bChange )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_NEED_RESTART_OPTION,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CHANGE_CHANNEL()
{
	if( m_pChannelPOPUP->IsVisible() )
	{
		INT nChNum = m_pChannelPOPUP->m_nSELECT;
		if( nChNum == T_INVALID )
			return TERR_NONE;

		if( nChNum >= m_pMainWnd->m_bChannelNumber )
			++nChNum;

		if( nChNum < (INT)m_pMainWnd->m_vTCHANNELS.size() )
			DoChangeChannel(m_pMainWnd->m_vTCHANNELS[nChNum].bID);

		m_pChannelPOPUP->EnableComponent(FALSE);
		m_pChannelPOPUP->ShowComponent(FALSE);
	}	
	else
	{
		DisableUI(TFRAME_MAIN_MENU);

		m_pChannelPOPUP->ClearMENU();

		for(size_t i=0; i<m_pMainWnd->m_vTCHANNELS.size(); ++i)
		{
			if( m_pMainWnd->m_bChannelNumber == (BYTE)i )
				continue;

			m_pChannelPOPUP->AddMENU(
				m_pMainWnd->m_vTCHANNELS[i].strNAME,
				GM_CHANGE_CHANNEL);
		}

		m_pChannelPOPUP->AddMENU( TSTR_CANCEL, GM_CHANGE_CHANNEL);
		m_pChannelPOPUP->EnableComponent(TRUE);
		m_pChannelPOPUP->ShowComponent(TRUE);

		RemoveKid(m_pChannelPOPUP);
		AddKid(m_pChannelPOPUP);

		CSize sz;
		m_pChannelPOPUP->GetComponentSize(&sz);
		
		CRect rect;
		INT n = (m_pMainWnd->m_Device.m_option.m_dwScreenX - sz.cx);
		rect.left = (n >= 0 ? n/2 : 0 );
		rect.right = rect.left + sz.cx;
		n = (m_pMainWnd->m_Device.m_option.m_dwScreenY - sz.cy - TSCREENHEIGHT_OFFSET);
		rect.top = (n >= 0 ? n/2 : 0 );
		rect.bottom = rect.top + sz.cy;

		m_pChannelPOPUP->SetComponentRect(rect);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CABINET_SELECT()
{
	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
	pDlg->SelectCab(pDlg->GetLastHitBtn());

	return TERR_NONE;
}

int CTClientGame::OnGM_CABINET_OPEN_WARN()
{
	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
	
	INT64 dwPrice = pDlg->GetCabOpenCost( pDlg->GetLastHitBtn() );
	CString strPRICE = MakeMoneyStr(dwPrice);

	CString strMSG;
	strMSG = CTChart::Format( TSTR_CABINET_OPEN_WARN, strPRICE);

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_OK,
		TSTR_CANCEL,
		GM_CABINET_OPEN,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnGM_CABINET_OPEN()
{
	m_pMainWnd->CloseMessageBox();

	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
	m_pSESSION->SendCS_CABINETOPEN_REQ(pDlg->GetLastHitBtn());

	return TERR_NONE;
}

int CTClientGame::OnGM_FOLLOW_TARGET()
{
	if(!m_pMainChar->CountMaintainFunc( SDT_AI, SDT_RUNAWAY, 1))
	{
		m_bMoveType = TMAINMOVE_FOLLOW;
		m_bAutoRun = FALSE;
		m_dwMoveGM = 0;
		m_fBOUND = POPUP_DIST;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_KEYSET_DEFAULT()
{
	CTKeySettingDlg* pKeyDlg = static_cast<CTKeySettingDlg*>(m_vTFRAME[TFRAME_KEYSETTING]);
	pKeyDlg->SetToDefault();

	return TERR_NONE;
}

int CTClientGame::OnGM_KEYSET_APPLY()
{
	CTKeySetting::GetInstance()->SaveRegistry(TREGISTRY_KEY);
	DisableUI(TFRAME_KEYSETTING);

	return TERR_NONE;
}

int CTClientGame::OnGM_KEYSET_CANCEL()
{
	CTKeySetting::GetInstance()->LoadRegistry(TREGISTRY_KEY);
	static_cast<CTKeySettingDlg*>(m_vTFRAME[TFRAME_KEYSETTING])->NotifyUpdate();
	DisableUI(TFRAME_KEYSETTING);

	return TERR_NONE;
}

int CTClientGame::OnGM_MENU_DUEL()
{
	if( m_pSESSION && 
		m_pTARGET && 
		m_pTARGET->m_bType == OT_PC && 
		CanDuel(static_cast<CTClientChar*>(m_pTARGET)) )
	{
		m_pDuelTarget = static_cast<CTClientChar*>(m_pTARGET);
		m_pSESSION->SendCS_DUELINVITE_REQ(m_pTARGET->m_dwID);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_DUELINVITE_YES()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && m_pDuelTarget )
		m_pSESSION->SendCS_DUELINVITEREPLY_REQ(ASK_YES, m_pDuelTarget->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_DUELINVITE_NO()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && m_pDuelTarget )
		m_pSESSION->SendCS_DUELINVITEREPLY_REQ(ASK_NO, m_pDuelTarget->m_dwID);

	return TERR_NONE;
}

int CTClientGame::OnGM_SMS_SEND()
{
	CString strMSG;
	strMSG = CTChart::Format( TSTR_SMS_SEND);

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_OK,
		TSTR_CANCEL,
		GM_SMS_SEND_OK,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}
	
int CTClientGame::OnGM_SMS_SEND_OK()
{
	m_pMainWnd->CloseMessageBox();

	CTSMSDlg* pDlg = static_cast<CTSMSDlg*>(m_vTFRAME[TFRAME_SMS]);
	pDlg->SendSMS();

	DisableUI(TFRAME_SMS);
	return TERR_NONE;
}

int CTClientGame::OnGM_WHISPER_REPLY()
{
	if( !m_pChatFrame->IsChatEditON() )
	{
		CString strWhisper = m_pChatFrame->GetLastWhisperTarget();
		if( !strWhisper.IsEmpty() )
		{
			EnableChat(TRUE);
			m_pChatFrame->SetChatMode(CHAT_WHISPER, strWhisper);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLINIT_ALL()
{
	if( !m_pSESSION )
		return TERR_NONE;

	m_pSESSION->SendCS_SKILLINIT_REQ(0, m_bSkillInitInven, m_bSkillInitSlot);
	m_pMainWnd->CloseMessageBox();

	return TERR_NONE;
}

int CTClientGame::OnGM_SKILLINIT_ONE()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTSkillDlg* pDlg = static_cast<CTSkillDlg*>(m_vTFRAME[TFRAME_SKILL]);
	if( !pDlg->m_bSKILLINIT || !pDlg->m_bSiSelectID )
		return TERR_NONE;

	m_pSESSION->SendCS_SKILLINIT_REQ(
		pDlg->m_bSiSelectID,
		pDlg->m_bSiInvenID,
		pDlg->m_bSiSlotID);

	m_pMainWnd->CloseMessageBox();
	pDlg->EndSkillInitMode();

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_HELMETHIDE()
{
	if( m_pSESSION && m_pMainChar )
		m_pSESSION->SendCS_HELMETHIDE_REQ( !m_pMainChar->m_bHelmetHide );

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_SUBCASHMENU()
{
	CTMainUI* pDlg = static_cast<CTMainUI*>(m_vTFRAME[TFRAME_MAIN]);

	pDlg->m_bCashMenu = !pDlg->m_bCashMenu;
	pDlg->ResetCashMenu();

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CASH_SHOP()
{
	if( !m_pSESSION )
		return TERR_NONE;

	if( m_vMAP.m_wMapID == TUTORIAL_MAPID )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_TUTORIAL_ERROR_MSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX, 0);

		return TERR_NONE;
	}

//#ifdef TEST_MODE
//	if( m_vTFRAME[TFRAME_CASHSHOP_NEW]->IsVisible() )
//		DisableUI(TFRAME_CASHSHOP_NEW);
//	else
//		m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ();
//#else
	CTWebCashShopDlg* pWebShop = (CTWebCashShopDlg*) m_vTFRAME[TFRAME_WEB_CASHSHOP];

	if ( CTNationOption::USE_WEB_CASHSHOP )
	{
		if ( pWebShop->IsWebShop() )
		{
			if ( pWebShop->IsVisible() )
				DisableUI(TFRAME_WEB_CASHSHOP);
			else
				EnableUI(TFRAME_WEB_CASHSHOP);
		}
	}
	else
	{
		if( m_vTFRAME[TFRAME_CASHSHOP_NEW]->IsVisible() )
			DisableUI(TFRAME_CASHSHOP_NEW);
		else
			m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ();
	}
//#endif

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CASH_CAB()
{
	if( m_vMAP.m_wMapID == TUTORIAL_MAPID )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_TUTORIAL_ERROR_MSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX, 0);

		return TERR_NONE;
	}

	if( m_vTFRAME[TFRAME_CASHCABINET]->IsVisible() )
		DisableUI(TFRAME_CASHCABINET);
	else
	{
		CTCabinetDlg* pCabDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);
		pCabDlg->RequestShowComponent();
	}

	static_cast<CTMainUI*>( m_vTFRAME[ TFRAME_MAIN ] )->m_pCabinetButton->m_bTwinkle = FALSE;
	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CASH_CHARGE()
{
	CString strURL;

	if(!m_pMainWnd)
		return TERR_NONE;

	if( CTNationOption::JAPAN )
	{
		switch( m_pMainWnd->m_bSiteCode )
		{
		case 0: //losteden.jp로 로그인 한 유저 : 채널 0 strURL = "http://losteden.jp/point/choice/";
			strURL = CTChart::LoadString( TSTR_URL_CASHCHARGE);
			break;

		case 1: //daletto.com으로 로그인 한 유저 : 채널 1 strURL = "http://losteden.daletto.com/point/choice/";
			strURL = CTChart::LoadString( TSTR_URL_CASHCHARGE_CH1);
			break;

		default:
			return TERR_NONE;
		}
	}
	else
	{
		strURL = CTChart::LoadString( TSTR_URL_CASHCHARGE);
	}

	if( CTNationOption::POLAND ||
		CTNationOption::FRANCE ||
		CTNationOption::GERMANY ||
		// Arab Ver.
		CTNationOption::EGYPT ||
		CTNationOption::GREECE || 
		CTNationOption::CZECH ||
//		CTNationOption::ITALY ||
		CTNationOption::SPAIN ||
		CTNationOption::UNITEDKINGDOM ||
		CTNationOption::TAIWAN ||
		CTNationOption::ENGLISH ||
		CTNationOption::RUSSIA ||
		CTNationOption::ROMANIA ||
		CTNationOption::UNITEDSTATE ||
		CTNationOption::HUNGARY ||
		CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL ||
		CTNationOption::JAPAN )
	{
		m_pMainWnd->m_vWebCtrl.Navigate(
			strURL,
			0, 0,
			0, 0,
			TRUE,
			TRUE,
			TRUE,
			TRUE,
			TRUE,
			HWND_TOPMOST);
	}
	else
	{
		m_pMainWnd->m_vWebCtrl.Navigate(
			strURL,
			100,
			100,
			530,
			690,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			TRUE,
			HWND_TOPMOST);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_ACCOUNT()
{
	if(!m_pMainWnd)
		return TERR_NONE;

	if( !m_pMainWnd->m_TNet.m_strCAURL.IsEmpty() )
	{
		m_pMainWnd->m_vWebCtrl.Navigate(
			m_pMainWnd->m_TNet.m_strCAURL,
			0, 0,
			0, 0,
			TRUE,
			TRUE,
			TRUE,
			TRUE,
			TRUE,
			HWND_TOPMOST);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_CASHCHARGE()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_CASHCAB_EXTENSIONPERIOD()
{
	if( !m_pSESSION )
		return TERR_NONE;

	if( CTNationOption::RUSSIA )
	{
		BYTE bNeedInven = 0;
		BYTE bNeedItem = 0;

		CString strMsg;
		if( FindInven( IT_STORAGE, IK_NONE, bNeedInven, bNeedItem ) )
		{
			CTClientItem* pItem = FindMainItemByInven( bNeedInven, bNeedItem );
			if( pItem )
			{
				strMsg.Format( CTChart::Format(
					TSTR_FMT_ASK_EXTEND_PERIOD,
					pItem->GetTITEM()->m_wUseValue ) );

				m_pMainWnd->MessageBoxYesNo(
					strMsg,
					TSTR_OK,
					TSTR_CANCEL,
					TCOMMAND( GM_CASHCAB_BUYCABINET_YES ),
					TCOMMAND( GM_CLOSE_MSGBOX ) );
			}
		}
		else
		{
			strMsg.Format( CTChart::LoadString( TSTR_RPSGAME_NEED_A_ITEM ) );

			m_pMainWnd->MessageBoxOK(
				strMsg,
				TSTR_OK,
				TCOMMAND( GM_CLOSE_MSGBOX ),
				0 );
		}
	}
	else
	{
		m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ(
			CASHCABINET_ID, // bItemID
			0, // bInvenID
			INVALID_SLOT);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CASHCAB_GETITEM()
{
	if( !FindEmptyInven(m_bInvenID,m_bSlotID) )
		return TERR_ITEM_NO_SLOT;

	if( m_bLockGM_CASHCAB_GETITEM )
		return TERR_NONE;

	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);
	CTCabinetItem* pCabItem = pDlg->GetSelectCabItem();

	if( pCabItem && m_pSESSION )
	{
		m_bLockGM_CASHCAB_GETITEM = TRUE;

		m_pSESSION->SendCS_CASHITEMGET_REQ(
			pCabItem->m_dwStItemID,
			m_bInvenID,
			m_bSlotID);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CASHCAB_BUYCABINET_YES()
{
	if( CTNationOption::RUSSIA )
	{
		m_pMainWnd->CloseMessageBox();

		if( !m_pSESSION )
			return TERR_NONE;

		BYTE bNeedInven = 0;
		BYTE bNeedItem = 0;

		if( FindInven( IT_STORAGE, IK_NONE, bNeedInven, bNeedItem ) )
			m_pSESSION->SendCS_CASHCABINETBUY_REQ( bNeedInven, bNeedItem );

		return TERR_NONE;
	}
	else
	{
		if( m_pSESSION )
		{
			m_pSESSION->SendCS_CASHCABINETBUY_REQ(0);
		}

		m_pMainWnd->CloseMessageBox();
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_BUY_CASHITEM()
{
	CTNewCashShopDlg* pDLG = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
	CTCashShopItem* pInfo = static_cast<CTCashShopItem*>( pDLG->GetSelectItem() );

	if( pInfo )
	{
		LPTITEM pTITEM = pInfo->m_pClientItem->GetTITEM();

		CString strMSG;
		strMSG = CTChart::Format( TSTR_FMT_BUYITEM,
			pTITEM->m_strNAME);

		pDLG->SetPresent( FALSE );

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_BUY_CASHITEM,
			GM_CLOSE_MSGBOX );
	
		EnableChat(FALSE);
	}

	return TERR_NONE;
}
	
int CTClientGame::OnGM_BUY_CASHITEM()
{
	if( !m_pSESSION )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();

	CTNewCashShopDlg* pDLG = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
	CTCashShopItem* pInfo = static_cast<CTCashShopItem*>( pDLG->GetSelectItem());

	if( pInfo )
		m_pSESSION->SendCS_CASHITEMBUY_REQ(pInfo->m_wCashItemID);

	return TERR_NONE; 
}

int CTClientGame::OnGM_WARN_CASH_PRESENT()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTNewCashShopDlg* pDLG = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
	CTCashShopItem* pInfo = static_cast<CTCashShopItem*>( pDLG->GetSelectItem());

	if( pInfo )
	{
		LPTITEM pTITEM = pInfo->m_pClientItem->GetTITEM();

		CString strMSG;
		strMSG = CTChart::Format( TSTR_FMT_PRESENTITEM, pTITEM->m_strNAME);

		CString strNAME;
		strNAME = CTChart::LoadString( TSTR_NAME);
		pDLG->SetPresent( FALSE );

		m_pMainWnd->MessageBox(
			strMSG,
			strNAME,
			0,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_CHECK_CASH_PRESENT,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);
		
		EnableChat(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CHECK_CASH_PRESENT()
{
	CTNewCashShopDlg* pDLG = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
	CTCashShopItem* pInfo = static_cast<CTCashShopItem*>( pDLG->GetSelectItem());

	m_pMainWnd->CloseMessageBox();
	CString strTarget = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);

	if ( pInfo )
	{

		LPTITEM pTITEM = pInfo->m_pClientItem->GetTITEM();

		CString strMSG;

		strMSG = CTChart::Format( TSTR_FMT_CHECK_PRESENTCASHITEM, pTITEM->m_strNAME, strTarget );

		m_pMainWnd->MessageBox(
			strMSG,
			0,
			strTarget,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_CASH_PRESENT,
			GM_CLOSE_MSGBOX,
			0,
			FALSE,
			TRUE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CASH_PRESENT()
{
	if( !m_pSESSION )
		return TERR_NONE;

	m_pMainWnd->CloseMessageBox();

	CString strTarget = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( !strTarget.IsEmpty() )
	{
		CTNewCashShopDlg* pDLG = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
		CTCashShopItem* pInfo = static_cast<CTCashShopItem*>( pDLG->GetSelectItem());

		if( pInfo )
		{
			m_pSESSION->SendCS_CASHITEMPRESENT_REQ(strTarget, pInfo->m_wCashItemID);
			pDLG->SetPresent( TRUE );
		}

		/*CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();
		CString strTitle = CTChart::FindTITEMTEMP( pInfo->m_wItemID )->m_strNAME;
		CString strMSG = CTChart::Format( TSTR_MAILMSG_PRESENTCASHITEM, m_pMainChar->m_strNAME, strTitle );

		strTitle = CTTextLinker::GetInstance()->BuildNetString( _T(""), strTitle);
		strMSG = CTTextLinker::GetInstance()->BuildNetString( _T(""), strMSG);

		CTMailSendDlg* pDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);

		m_pSESSION->SendCS_POSTSEND_REQ(
			strTarget,
			strTitle,
			strMSG,
			POST_NORMAL,
			0,
			0,
			0,
			INVEN_NULL,
			0);*/
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_WARN_EXTEND_PERIOD()
{
	CTNewCashShopDlg* pNewCashShopDlg = (CTNewCashShopDlg*) m_vTFRAME[TFRAME_CASHSHOP_NEW];
	CTCashShopItem* pInfo = pNewCashShopDlg->GetSelectItem();

	if( pInfo &&
		pInfo->m_pClientItem &&
		pInfo->m_wItemID == CTNewCashShopDlg::m_ExtendPeriodInfo.wItemID )
	{
		LPTITEM pTITEM = pInfo->m_pClientItem->GetTITEM();

		if( !pTITEM )
			return TERR_NONE;

		BYTE bInvenID = CTNewCashShopDlg::m_ExtendPeriodInfo.bInvenID;
		BYTE bSlotID = CTNewCashShopDlg::m_ExtendPeriodInfo.bSlotID;

		CString strMSG;
		strMSG = CTChart::Format( TSTR_FMT_EXTEND_PERIOD,
			pTITEM->m_strNAME,
			pInfo->m_wUseDay);

		DWORD dwPARAM = MAKELONG(
			(WORD) bInvenID,
			(WORD) bSlotID);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			TCOMMAND(GM_EXTEND_PERIOD, dwPARAM),
			GM_CLOSE_MSGBOX );

		EnableChat(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_EXTEND_PERIOD()
{
	if( CTNationOption::RUSSIA )
	{
		if( !m_pSESSION )
			return TERR_NONE;

		m_pMainWnd->CloseMessageBox();
		return TERR_NONE;
	}
	else
	{
		BOOL bCASH_CABINET = (CTNewCashShopDlg::m_ExtendPeriodInfo.wItemID == CASHCABINET_ID);
		ZeroMemory( &CTNewCashShopDlg::m_ExtendPeriodInfo, sizeof(CASHEXTENDPERIODINFO));

		if( !m_pSESSION )
			return TERR_NONE;

		m_pMainWnd->CloseMessageBox();
		DisableUI(TFRAME_CASHSHOP_NEW);

		CTNewCashShopDlg* pNewCashShopDlg = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
		CTCashShopItem* pInfo = pNewCashShopDlg->GetSelectItem();

		if( pInfo )
		{
			BYTE bInvenID = (BYTE) LOWORD(m_dwCmdParam);
			BYTE bSlotID = (BYTE) HIWORD(m_dwCmdParam);

			if( bCASH_CABINET )
			{
				m_pSESSION->SendCS_CASHCABINETBUY_REQ(
					pInfo->m_wCashItemID);
			}
			else
			{
				m_pSESSION->SendCS_STOPTHECLOCK_REQ(
					bInvenID,
					bSlotID,
					pInfo->m_wCashItemID);
			}
		}
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_CASHCABINET_PUTIN()
{
	m_pMainWnd->CloseMessageBox();

	if( !m_pSESSION )
		return TERR_NONE;

	BYTE bInvenID = (BYTE) LOWORD( m_dwCmdParam );
	BYTE bSlotID = (BYTE) HIWORD( m_dwCmdParam );

	m_pSESSION->SendCS_CASHITEMPUTIN_REQ(
		bInvenID,
		bSlotID);

	return TERR_NONE;
}

int CTClientGame::OnGM_END_MAINTAIN()
{
	if( !m_pSelectSkill || !m_pMainChar )
		return TERR_NONE;

	CTClientMaintain* pMaintain = m_pMainChar->FindMaintain( m_pSelectSkill->m_wSkillID );
	if( pMaintain != NULL )
	{
		SendCS_SKILLEND_REQ(
			m_pMainChar->m_dwID,
			m_pMainChar->m_bType,
			m_pMainChar->GetHostID(),
			pMaintain->m_dwAttackID,
			pMaintain->m_bAttackType,
			m_pSelectSkill->m_wSkillID,
			m_vMAP.m_wMapID,
			m_pMainWnd->m_bChannel);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_REGBOX()
{
	m_pMainWnd->CloseMessageBox();

	m_pMainWnd->MessageBox(
		TSTR_SOULMATEBOX_NAME,
		TSTR_NAME, 
		0,
		TSTR_OK,
		TSTR_CANCEL,
		ID_FRAME_2BTNMSGBOX,
		TCOMMAND( GM_SOULMATE_REGREADY, m_dwCmdParam ),
		GM_SOULMATE_NO,
		0,
		TRUE,
		TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_SEARCH()
{
	m_pMainWnd->CloseMessageBox();

	UINT64 nCOST = CTChart::GetSoulmateRegPrice(m_pMainChar->m_bLevel);
	CString strCOST = MakeMoneyStr(nCOST);

	CString strMSG;
	strMSG = CTChart::Format( TSTR_SOULMATEBOX_SEARCH, strCOST);

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_OK,
		TSTR_CANCEL,
		TCOMMAND( GM_SOULMATE_SEARCHOK, m_dwCmdParam ),
		GM_SOULMATE_NO );

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_REGREADY()
{
	CString strNAME = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->ClearMsgBoxEditor( ID_FRAME_2BTNMSGBOX);

	if( !strNAME.IsEmpty() )
	{
		m_pMainWnd->CloseMessageBox();

		if( m_pSESSION )
		{
			if( m_dwCmdParam == 0 )
			{
				m_pSESSION->SendCS_SOULMATEREGREADY_REQ(
					strNAME,
					INVEN_NULL,
					INVALID_SLOT );
			}
			else
			{
				WORD wNPCID = HIWORD( m_dwCmdParam );

				LPTNPC pTNPC = CTChart::FindTNPCGlobal( wNPCID );

				if( !pTNPC || pTNPC->m_bNPCType != TNPC_DIVINER )
					return TERR_NONE;

				WORD wItemID = LOWORD( m_dwCmdParam );
				BYTE bInvenID = HIBYTE( wItemID );
				BYTE bSlotID = LOBYTE( wItemID );

				m_pSESSION->SendCS_SOULMATEREGREADY_REQ(
					strNAME,
					bInvenID,
					bSlotID );
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_REGOK()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strSoulRelay.IsEmpty() )
	{
		if( m_dwCmdParam == 0 )
			m_pSESSION->SendCS_SOULMATEREG_REQ(
				m_strSoulRelay,
				INVEN_NULL,
				INVALID_SLOT );
		else
		{
			WORD wNPCID = HIWORD( m_dwCmdParam );
			WORD wItemID = LOWORD( m_dwCmdParam );
			BYTE bInvenID = HIBYTE( wItemID );
			BYTE bSlotID = LOBYTE( wItemID );

			m_pSESSION->SendCS_SOULMATEREG_REQ(
				m_strSoulRelay,
				bInvenID,
				bSlotID );
		}
	}
	
	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_SEARCHOK()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		if( m_dwCmdParam == 0 )
			m_pSESSION->SendCS_SOULMATESEARCH_REQ(
				INVEN_NULL, INVALID_SLOT );
		else
		{
			WORD wNPCID = HIWORD( m_dwCmdParam );

			LPTNPC pTNPC = CTChart::FindTNPCGlobal( wNPCID );

			if( !pTNPC || pTNPC->m_bNPCType != TNPC_DIVINER )
				return TERR_NONE;

			WORD wItemID = LOWORD( m_dwCmdParam );
			BYTE bInvenID = HIBYTE( wItemID );
			BYTE bSlotID = LOBYTE( wItemID );

			m_pSESSION->SendCS_SOULMATESEARCH_REQ(
				bInvenID,
				bSlotID );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_NO()
{
	m_pMainWnd->CloseMessageBox();

	m_pMainWnd->MessageBoxOK(
		TSTR_SOULMATEBOX_NO,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_END()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
		m_pSESSION->SendCS_SOULMATEEND_REQ();

	m_pMessengerBase->GetMsgGroupList()->HideAllPopup();
	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_SUCCESS()
{
	LPTREGIONINFO pTREGION = CTChart::FindTREGIONINFO( m_dwSoulRegion);
	m_dwSoulRegion = 0;
	CString strREGION;
	if( pTREGION )
		strREGION = pTREGION->m_strNAME;

	CString strMSG;
	strMSG = CTChart::Format( TSTR_SOULMATE_SEARCH_SUCCESS, strREGION, m_strSoulName); 

	ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_INFO);

	m_pMessengerBase->GetMsgGroupList()->UpdateSoulmateInfo(
		m_dwSoulID,
		m_strSoulName,
		m_dwSoulRegion,
		255, 255, 255 );

	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_NOTFOUND()
{
	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_ERR_SOULMATE_NOTFOUND); 

	ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
	return TERR_NONE;
}

int CTClientGame::OnGM_SOULMATE_FAIL()
{
	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_ERR_SOULMATE_FAIL);

	ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
	return TERR_NONE;
}

int CTClientGame::OnGM_GAMBLE_CHECK()
{
	CTItemGambleDlg* pDlg = static_cast<CTItemGambleDlg*>(m_vTFRAME[TFRAME_ITEM_GAMBLE]);

	if( !m_pSESSION ||
		!pDlg->GetItem(CTItemGambleDlg::TARGET_SLOT) )
		return TERR_NONE;

	if( !pDlg->m_pOpenByCash )
	{
		if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC )
			return TERR_NONE;

		BYTE bInven = pDlg->GetLastInvenID(CTItemGambleDlg::TARGET_SLOT);
		BYTE bSlot = pDlg->GetLastInvenSlot(CTItemGambleDlg::TARGET_SLOT);
		WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

		m_pSESSION->SendCS_GAMBLECHECK_REQ(
			bInven,
			bSlot,
			wNpcID );	
	}
	else
	{
		BYTE bInven = pDlg->GetLastInvenID(CTItemGambleDlg::TARGET_SLOT);
		BYTE bSlot = pDlg->GetLastInvenSlot(CTItemGambleDlg::TARGET_SLOT);

		m_pSESSION->SendCS_GAMBLECHECK_REQ(
			bInven,
			bSlot,
			pDlg->m_pOpenByCash->m_wNPCID );	
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_GAMBLE_PROGRESS()
{
	m_pMainWnd->CloseMessageBox();

	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_GAMBLING_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_GAMBLING_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TGAMBLE_PERIOD,
		GM_GAMBLE_OK);

	return TERR_NONE;
}

int CTClientGame::OnGM_GAMBLE_OK()
{
	m_pMainWnd->CloseMessageBox();

	CTItemGambleDlg* pDlg = static_cast<CTItemGambleDlg*>(m_vTFRAME[TFRAME_ITEM_GAMBLE]);

	if( !m_pSESSION || !pDlg->GetItem(CTItemGambleDlg::TARGET_SLOT) )
		return TERR_NONE;

	if( !pDlg->m_pOpenByCash )
	{
		if( !(m_pTARGET && m_pTARGET->m_bType == OT_NPC) )
			return TERR_NONE;

		BYTE bInven = pDlg->GetLastInvenID(CTItemGambleDlg::TARGET_SLOT);
		BYTE bSlot = pDlg->GetLastInvenSlot(CTItemGambleDlg::TARGET_SLOT);
		WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

		m_pSESSION->SendCS_GAMBLEOPEN_REQ(
			bInven,
			bSlot,
			wNpcID,
			INVEN_NULL,
			INVALID_SLOT );
	}
	else
	{
		BYTE bInven = pDlg->GetLastInvenID(CTItemGambleDlg::TARGET_SLOT);
		BYTE bSlot = pDlg->GetLastInvenSlot(CTItemGambleDlg::TARGET_SLOT);

		m_pSESSION->SendCS_GAMBLEOPEN_REQ(
			bInven,
			bSlot,
			pDlg->m_pOpenByCash->m_wNPCID,
			pDlg->m_pOpenByCash->m_bInvenID,
			pDlg->m_pOpenByCash->m_bSlotID );
	}

	DisableUI(TFRAME_ITEM_GAMBLE, TRUE);

	return TERR_NONE;
}

int CTClientGame::OnGM_GAMBLE_NO()
{
	m_pMainWnd->CloseMessageBox();

	m_pMainWnd->MessageBoxOK(
		TSTR_GAMBLE_NO,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_DURATIONREP_NOR()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_DURATIONREP_NOR;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

        EnableUI(TFRAME_ITEM_REPAIR);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_DURATIONREP_NOR()
{
	CTItemRepairDlg* pDlg = static_cast<CTItemRepairDlg*>(m_vTFRAME[TFRAME_ITEM_REPAIR]);

	if( !pDlg->m_pOpenByCash )
	{
		if( !pDlg->GetItem(CTItemRepairDlg::TARGET_SLOT) ||
			!(m_pTARGET && m_pTARGET->m_bType == OT_NPC) )
			return TERR_NONE;

		m_bDurationRepInven = pDlg->GetLastInvenID(CTItemRepairDlg::TARGET_SLOT);
		m_bDurationRepSlot = pDlg->GetLastInvenSlot(CTItemRepairDlg::TARGET_SLOT);
		m_bDurationRepType = RPT_NORMAL; //일반수리

		if( m_pSESSION )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_DURATIONREP_REQ(
				TRUE,	// 비용요청
				m_bDurationRepType,
				m_bDurationRepInven,
				m_bDurationRepSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT );
		}
	}
	else
	{
		if( !pDlg->GetItem(CTItemRepairDlg::TARGET_SLOT) )
			return TERR_NONE;

		m_bDurationRepInven = pDlg->GetLastInvenID(CTItemRepairDlg::TARGET_SLOT);
		m_bDurationRepSlot = pDlg->GetLastInvenSlot(CTItemRepairDlg::TARGET_SLOT);
		m_bDurationRepType = RPT_NORMAL; //일반수리

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_DURATIONREP_REQ(
				TRUE,	// 비용요청
				m_bDurationRepType,
				m_bDurationRepInven,
				m_bDurationRepSlot,
				pDlg->m_pOpenByCash->m_wNPCID,
				pDlg->m_pOpenByCash->m_bInvenID,
				pDlg->m_pOpenByCash->m_bSlotID);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_DURATIONREP_ALL()
{
	m_pMainWnd->CloseMessageBox();

	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_DURATIONREP_ALL;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		m_bDurationRepInven = 0;
		m_bDurationRepSlot = 0;
		m_bDurationRepType = RPT_ALL; //전부수리

		if( m_pSESSION )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_DURATIONREP_REQ(
				TRUE,	// 비용요청
				m_bDurationRepType,
				m_bDurationRepInven,
				m_bDurationRepSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_DURATIONREP_EQUIP()
{
	m_pMainWnd->CloseMessageBox();

	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_DURATIONREP_EQUIP;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		m_bDurationRepInven = 0;
		m_bDurationRepSlot = 0;
		m_bDurationRepType = RPT_EQUIP; //장비수리

		if( m_pSESSION )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_DURATIONREP_REQ(
				TRUE,	// 비용요청
				m_bDurationRepType,
				m_bDurationRepInven,
				m_bDurationRepSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_DURATIONREP_PROGRESS()
{
	m_pMainWnd->CloseMessageBox();

	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_DURATIONREPAIRING_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_DURATIONREPAIRING_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TDURATIONREP_PERIOD,
		GM_DURATIONREP_REQ);

	return TERR_NONE;
}

int CTClientGame::OnGM_DURATIONREP_REQ()
{
	if( !m_pSESSION )
		return TERR_NONE;

	CTItemRepairDlg* pITEMREPAIRDLG = (CTItemRepairDlg*) GetFrame( TFRAME_ITEM_REPAIR );

	if( !pITEMREPAIRDLG->m_pOpenByCash )
	{
		if( m_pTARGET &&
			(m_pTARGET->m_bType == OT_NPC))
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_DURATIONREP_REQ(
				FALSE,	// 수리요청
				m_bDurationRepType,
				m_bDurationRepInven,
				m_bDurationRepSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT);
		}
	}
	else
	{
		m_pSESSION->SendCS_DURATIONREP_REQ(
			FALSE,	// 수리요청
			m_bDurationRepType,
			m_bDurationRepInven,
			m_bDurationRepSlot,
			pITEMREPAIRDLG->m_pOpenByCash->m_wNPCID,
			pITEMREPAIRDLG->m_pOpenByCash->m_bInvenID,
			pITEMREPAIRDLG->m_pOpenByCash->m_bSlotID );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_REFINE()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_REFINE;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		EnableUI(TFRAME_ITEM_REFINE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_REFINE_COST()
{
	CTItemRefineDlg* pDlg = static_cast<CTItemRefineDlg*>(m_vTFRAME[TFRAME_ITEM_REFINE]);

	if( !pDlg->GetItem(CTItemRefineDlg::TARGET_SLOT) )
		return TERR_NONE;

	TINVENITEM_ARRAY vAdds;
	BYTE bSlot = CTItemRefineDlg::RESOURCE_SLOT1;
	for(; bSlot<CTItemRefineDlg::SLOT_COUNT; ++bSlot)
	{
		if( pDlg->GetItem(bSlot) )
		{
			TINVENITEM item;
			item.m_bInven = pDlg->GetLastInvenID(bSlot);
			item.m_bItem = pDlg->GetLastInvenSlot(bSlot);

			vAdds.push_back(item);
		}	
	}

	if( vAdds.empty() )
		return TERR_NONE;

	if( !pDlg->m_pOpenByCash )
	{
		if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC)
			return TERR_NONE;
		
		m_bRefineInven = pDlg->GetLastInvenID(CTItemRefineDlg::TARGET_SLOT);
		m_bRefineSlot = pDlg->GetLastInvenSlot(CTItemRefineDlg::TARGET_SLOT);

		if( m_pSESSION )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_REFINE_REQ(
				TRUE,	// 비용요청
				m_bRefineInven,
				m_bRefineSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT,
				&vAdds);
		}
	}
	else
	{
		m_bRefineInven = pDlg->GetLastInvenID(CTItemRefineDlg::TARGET_SLOT);
		m_bRefineSlot = pDlg->GetLastInvenSlot(CTItemRefineDlg::TARGET_SLOT);

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_REFINE_REQ(
				TRUE,	// 비용요청
				m_bRefineInven,
				m_bRefineSlot,
				pDlg->m_pOpenByCash->m_wNPCID,
				pDlg->m_pOpenByCash->m_bInvenID,
				pDlg->m_pOpenByCash->m_bSlotID,
				&vAdds);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_REFINE_PROGRESS()
{
	m_pMainWnd->CloseMessageBox();

	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_REFINING_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_REFINING_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TREFINE_PERIOD,
		GM_REFINE);

	return TERR_NONE;
}

int CTClientGame::OnGM_REFINE()
{
	CTItemRefineDlg* pDlg = static_cast<CTItemRefineDlg*>(m_vTFRAME[TFRAME_ITEM_REFINE]);

	if( !pDlg->GetItem(CTItemRefineDlg::TARGET_SLOT) )
		return TERR_NONE;

	TINVENITEM_ARRAY vAdds;
	BYTE bSlot = CTItemRefineDlg::RESOURCE_SLOT1;
	for(; bSlot<CTItemRefineDlg::SLOT_COUNT; ++bSlot)
	{
		if( pDlg->GetItem(bSlot) )
		{
			TINVENITEM item;
			item.m_bInven = pDlg->GetLastInvenID(bSlot);
			item.m_bItem = pDlg->GetLastInvenSlot(bSlot);

			vAdds.push_back(item);
		}	
	}

	if( vAdds.empty() )
		return TERR_NONE;

	if( !pDlg->m_pOpenByCash )
	{
		if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC)
			return TERR_NONE;

		m_bRefineInven = pDlg->GetLastInvenID(CTItemRefineDlg::TARGET_SLOT);
		m_bRefineSlot = pDlg->GetLastInvenSlot(CTItemRefineDlg::TARGET_SLOT);

		if( m_pSESSION )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_REFINE_REQ(
				FALSE,	// 제련요청
				m_bRefineInven,
				m_bRefineSlot,
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT,
				&vAdds);
		}
	}
	else
	{
		m_bRefineInven = pDlg->GetLastInvenID(CTItemRefineDlg::TARGET_SLOT);
		m_bRefineSlot = pDlg->GetLastInvenSlot(CTItemRefineDlg::TARGET_SLOT);

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_REFINE_REQ(
				FALSE,	// 제련요청
				m_bRefineInven,
				m_bRefineSlot,
				pDlg->m_pOpenByCash->m_wNPCID,
				pDlg->m_pOpenByCash->m_bInvenID,
				pDlg->m_pOpenByCash->m_bSlotID,
				&vAdds);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CHANGE_NAME()
{
	BYTE bInvenID = (BYTE) LOWORD(m_dwCmdParam);
	BYTE bItemID = (BYTE) HIWORD(m_dwCmdParam);

	TFrame *pFRAME = m_pMainWnd->m_MessageBox.FindFrame(ID_FRAME_2BTNMSGBOX);
	if( !pFRAME )
		return TERR_NONE;

	TComponent *pEDIT = pFRAME->FindKid(ID_CTRLINST_EDIT);
	if( !pEDIT )
		return TERR_NONE;

	if( CTClientWnd::CheckCharID(pEDIT->m_strText) < 0 )
		return TERR_INVALID_NAME;

	if( m_pSESSION)
	{
		m_pSESSION->SendCS_CHANGENAME_REQ(
			bInvenID,
			bItemID,
			pEDIT->m_strText);

		m_pMainWnd->CloseMessageBox();
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_EXTEND_INVEN_PERIOD()
{
	if( CTNationOption::RUSSIA )
	{
		if( m_pSESSION )
		{
			BYTE bNeedInven = 0;
			BYTE bNeedItem = 0;

			CString strMsg;
			if( FindInven( IT_STURDYSACK, IK_NONE, bNeedInven, bNeedItem ) )
			{
				CTClientItem* pItem = FindMainItemByInven( bNeedInven, bNeedItem );
				if( pItem )
				{
					strMsg.Format( CTChart::Format(
						TSTR_FMT_ASK_EXTEND_PERIOD,
						pItem->GetTITEM()->m_wUseValue ) );

					m_pMainWnd->MessageBoxYesNo(
						strMsg,
						TSTR_OK,
						TSTR_CANCEL,
						TCOMMAND( GM_STOPTHECLOCK, m_dwCmdParam ),
						TCOMMAND( GM_CLOSE_MSGBOX ) );
				}
			}
			else
			{
				strMsg.Format( CTChart::LoadString( TSTR_RPSGAME_NEED_A_ITEM ) );

				m_pMainWnd->MessageBoxOK(
					strMsg,
					TSTR_OK,
					TCOMMAND( GM_CLOSE_MSGBOX ),
					0 );
			}
		}

		return TERR_NONE;
	}
	else
	{
		if( m_pSESSION )
		{
			BYTE bInvenID = (BYTE) LOWORD(m_dwCmdParam);
			WORD wItemID = HIWORD(m_dwCmdParam);

			m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ(
				wItemID, // bItemID
				bInvenID, // bInvenID
				INVALID_SLOT);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_EXTEND_ITEM_PERIOD()
{
	CTItemExtPeriodDlg* pDlg = static_cast<CTItemExtPeriodDlg*>( m_vTFRAME[TFRAME_ITEM_EXTPERIOD]);

	if( pDlg && m_pSESSION)
	{
		LPTITEM pTITEM = pDlg->GetItem(0);
		BYTE bInvenID = pDlg->GetLastInvenID(0);
		BYTE bSlotID = pDlg->GetLastInvenSlot(0);

		m_pSESSION->SendCS_CASHSHOPITEMLIST_REQ(
			pTITEM->m_wItemID,
			bInvenID,
			bSlotID);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_STOPTHECLOCK()
{
	m_pMainWnd->CloseMessageBox();

	BYTE bInven = (BYTE) LOWORD(m_dwCmdParam);
	BYTE bItem = (BYTE) HIWORD(m_dwCmdParam);
	BYTE bNeedInven = 0;
	BYTE bNeedItem = 0;

	if( FindInven( IT_STURDYSACK, IK_NONE, bNeedInven, bNeedItem ) )
	{
		m_pSESSION->SendCS_STOPTHECLOCK_REQ(
			bInven,
			bItem,
			bNeedInven,
			bNeedItem );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_EXTEND_ITEM_PERIOD()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (
		m_pTARGET->m_bContryID != TCONTRY_N &&
		!m_pMainChar->IsAlliance(m_pTARGET)) )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_NPC_EXTEND_ITEM_PERIOD;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		m_pTARGET->PlayRandomSND(
			&m_pTARGET->m_mapOBJSNDINST,
			&m_pTARGET->m_mapOBJSND,
			((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
			FALSE);

		EnableUI(TFRAME_ITEM_EXTPERIOD);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_MSGBOX_RESETINVEN()
{
	m_pMainWnd->CloseMessageBox();
	ResetInvenUI();
	return TERR_NONE;
}

int CTClientGame::OnGM_INIT_UI()
{
	m_pMainWnd->ResetTOption();
	m_pMainWnd->InitUI(TRUE);

	m_pSystemSetting->UpdateUI(m_pDevice);
	m_pGameSetting->UpdateUI();
	static_cast<CTSettingFrame*>( m_vTFRAME[TFRAME_SETTING] )->UpdateUI();

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_RESOLUTION()
{
	if( m_pSystemSetting && m_pSystemSetting->m_pResolutionListPopup )
		if( m_pSystemSetting->m_pResolutionListPopup->IsVisible() )
		{
			m_pSystemSetting->m_pResolutionListPopup->ShowComponent( !m_pSystemSetting->m_pResolutionListPopup->IsVisible() );
		}
		else
		{
			CRect rcBtn,rcPopup;
			m_pSystemSetting->m_pResolutionListPopup->GetComponentRect(&rcPopup);

			TComponent *pComp = m_pSystemSetting->FindKid(ID_CTRLINST_BTN_RESOLUTION);
			pComp->GetComponentRect(&rcBtn);

			m_pSystemSetting->m_pResolutionListPopup->MoveComponent( CPoint(rcBtn.left, rcBtn.top-rcPopup.Height()) );
			m_pSystemSetting->m_pResolutionListPopup->ShowComponent(!m_pSystemSetting->m_pResolutionListPopup->IsVisible());
		}

	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_NEXT()
{
	CTHelpFrame* pHELPFRAME = (CTHelpFrame*) GetFrame( TFRAME_HELP );
	pHELPFRAME->NextHelp();
	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_PREV()
{
	CTHelpFrame* pHELPFRAME = (CTHelpFrame*) GetFrame( TFRAME_HELP );
	pHELPFRAME->PrevHelp();
	return TERR_NONE;
}

int CTClientGame::OnGM_SET_WHISPER_TARGET()
{
	EnableChat(TRUE);

	m_pChatFrame->SetChatMode(
		CHAT_WHISPER,
		m_pChatFrame->GetCandiWhisperTarget() );

	m_pChatFrame->SetCandiWhisperTarget( "" );

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_WHISPER_TARGET()
{
	m_pChatFrame->ShowPopup(
		CTChatFrame::POPUP_LATEST_WHISPER,
		FALSE == m_pChatFrame->IsVisiblePopup( CTChatFrame::POPUP_LATEST_WHISPER) );

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_CHAT_TYPE()
{
	m_pChatFrame->ShowPopup(
		CTChatFrame::POPUP_CHATTYPE,
		FALSE == m_pChatFrame->IsVisiblePopup( CTChatFrame::POPUP_CHATTYPE) );

	return TERR_NONE;
}

#ifdef MODIFY_CNP_NAME
int CTClientGame::OnGM_COPY_NAME()
{
	if( m_pTARGET &&
		m_pTARGET->m_bType == OT_PC &&
		!(m_pTARGET->IsDead() || m_pTARGET->m_bGhost) )
	{
		m_strCopyName = m_pTARGET->GetName();
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PASTE_NAME()
{
	TEdit* pEDIT = reinterpret_cast<TEdit*>(m_dwCmdParam);

	if( pEDIT )
		pEDIT->InsertString( m_strCopyName );

	return TERR_NONE;
}
#endif

int CTClientGame::OnGM_DAY_GUILDPVPRECORD()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDMEMBER);
	EnableUI( TFRAME_COMMUNITY );
	return TERR_NONE;
}

int CTClientGame::OnGM_WEEKEN_GUILDPVPRECORD()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDMEMBER);
	EnableUI( TFRAME_COMMUNITY );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDREWARD()
{
	CTGuildPointRewardFrame* pDLG = (CTGuildPointRewardFrame*) m_vTFRAME[ TFRAME_GUILDPOINTREWARD ];

	if( pDLG->m_pTargetEdit->m_strText.IsEmpty() ||
		pDLG->m_pPointEdit->m_strText.IsEmpty() )
		return TERR_GUILD_REWARD_INVALID;

	INT nPoint = atoi( pDLG->m_pPointEdit->m_strText );
	CString strFMT;
	strFMT.Format( "%d", nPoint );
	if( strFMT != pDLG->m_pPointEdit->m_strText )
		return TERR_GUILD_REWARD_INVALID;

	m_pSESSION->SendCS_GUILDPOINTREWARD_REQ(
		pDLG->m_pTargetEdit->m_strText,
		nPoint,
		pDLG->m_pMessage->m_strText );

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_PVPRECORD()
{
	CTCommunityDlg* pDLG = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pDLG->SetSelect(TCOMMUNITY_FRAME_GUILDMEMBER);
	EnableUI( TFRAME_COMMUNITY );
	return TERR_NONE;
}

int CTClientGame::OnGM_POINTREWARD()
{
	if( m_pSESSION )
		m_pSESSION->SendCS_GUILDPOINTLOG_REQ();

	return TERR_NONE;
}

int CTClientGame::OnGM_PVP_INFO()
{
	CTCharPvPInfoDlg *pDlg = static_cast<CTCharPvPInfoDlg*>( m_vTFRAME[TFRAME_CHAR_PVP_INFO] );

	pDlg->ResetTabButton( TINFO_PVP );


	if( m_pSESSION )
		m_pSESSION->SendCS_PVPRECORD_REQ( 1 );

	return TERR_NONE;
}

int CTClientGame::OnGM_WAR_INFO()
{
	CTCharPvPInfoDlg *pDlg = static_cast<CTCharPvPInfoDlg*>( m_vTFRAME[TFRAME_CHAR_PVP_INFO] );
	
	pDlg->ResetTabButton( TINFO_WAR );
	
	if( m_pSESSION )
		m_pSESSION->SendCS_PVPRECORD_REQ( 0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_LIST_TAB1()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	pFrame->ResetTabButton( TPVPM_REGULAR );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_LIST_TAB2()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	pFrame->ResetTabButton( TPVPM_TACTICS );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_LIST_TAB3()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	pFrame->ResetTabButton( TPVPM_LATEST_PVPR );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_LIST_TAB4()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	pFrame->ResetTabButton( TPVPM_WEEK_PVPR );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_REWARD_CONTINUE()
{
	CTGuildPointRewardFrame* pDLG = (CTGuildPointRewardFrame*)
		CTClientGame::GetInstance()->GetFrame( TFRAME_GUILDPOINTREWARD );

	pDLG->m_pGuildPoint->m_strText.Format( "%d",
		CTGuildCommander::GetInstance()->GetGuildDetInfo().m_dwPvPUseablePoint );

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	pDLG->m_pTargetEdit->ClearText();
	pDLG->m_pPointEdit->ClearText();
	pDLG->m_pMessage->ClearText();

	m_pMainWnd->CloseMessageBox();

	pDLG->SwitchFocus( pDLG->m_pTargetEdit );
	pDLG->m_pTargetEdit->SetFocus(TRUE);

	pDLG->ShowComponent(FALSE);
	EnableUI( TFRAME_GUILDPOINTREWARD );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_REWARD_STOP()
{
	DisableUI( TFRAME_GUILDPOINTREWARD );
	m_pMainWnd->CloseMessageBox();
	return TERR_NONE;
}

int CTClientGame::OnGM_PICKUP_BALL()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_GODBALL ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() ||
		m_pMainChar->GetGodBallID() ||
		m_pMainChar->GetCamp() != static_cast< CTGodBall* >( m_pTARGET )->GetCamp() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > GODBALL_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_PICKUP_BALL;
		m_bAutoRun = FALSE;
		m_fBOUND = 1.0f;
	}
	else
	{
		CTGodBall* pGodBall = static_cast< CTGodBall* >( m_pTARGET );
		m_pSESSION->SendCS_TAKEGODBALL_REQ( pGodBall->GetBallID() );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_MOUNT_BALL2_TOWER()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_GODTOWER ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() ||
		!m_pMainChar->GetGodBallID() ||
		m_pMainChar->GetCamp() == static_cast< CTGodTower* >( m_pTARGET )->GetCamp() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > GODTOWER_DIST * 2.0f )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_MOUNT_BALL2_TOWER;
		m_bAutoRun = FALSE;
		m_fBOUND = GODTOWER_DIST + 2.0f;
	}
	else
	{
		if( m_pMainChar->GetGodBallID() )
		{
			ResetActOBJ(NULL);
			BeginCharge( 10000, TCHARGETYPE_OPEN );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEM_UP_PROGRESS_CANCEL()
{
	CTItemUpDlg* pDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );
	pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);
	pDlg->SetItem(CTItemUpDlg::TARGET_SLOT, NULL );

	//CTachyonRes::m_MEDIA.Stop( MEDIA_TSOUND, ID_SND_ITEM_START, 0);
	CD3DSound::ClearGARBAGE();
	m_pMainWnd->CloseModalFrame();
	return TERR_NONE;
}

int CTClientGame::OnGM_CHANGE_REFINE()
{
	BYTE bInvenID = (BYTE) LOWORD(m_dwCmdParam);
	BYTE bItemID = (BYTE) HIWORD(m_dwCmdParam);

	if( m_pSESSION )
		m_pSESSION->SendCS_WASTEREFINE_REQ( bInvenID, bItemID );

	m_pMainWnd->CloseMessageBox();
	return TERR_NONE;
}

int CTClientGame::OnGM_CORPS_BUSY()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_CORPSREPLY_REQ(
			m_strReqNAME,
			ASK_BUSY);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_PARTY_BUSY()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_PARTYJOIN_REQ(
			m_strReqNAME,
			m_bObtainType,
			ASK_BUSY);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TRADE_BUSY()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION && !m_strReqNAME.IsEmpty() )
	{
		m_pSESSION->SendCS_DEALITEMRLY_REQ(
			ASK_BUSY,
			m_strReqNAME);
	}
	
	return TERR_NONE;
}

int CTClientGame::OnGM_DUELINVITE_BUSY()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_DUELINVITEREPLY_REQ(
			ASK_BUSY,
			m_dwCmdParam /*Inviter*/ );
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDINVITE_ACK_BUSY()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_GUILDINVITEANSWER_REQ(
			ASK_BUSY,
			m_dwCmdParam /*Inviter*/ );
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_SEARCH_TAB()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->SetSearchFrame();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_REGIST_TAB()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->SetRegistFrame();

	pFrame->SetCurPage( 1 );
	pFrame->UpdateByListComp();
	pFrame->GetList()->MoveFirstPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_BIDDING_TAB()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->SetMyBidFrame();

	pFrame->SetCurPage( 1 );
	pFrame->UpdateByListComp();
	pFrame->GetList()->MoveFirstPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_BASKET_TAB()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->SetBasketFrame();

	pFrame->SetCurPage( 1 );
	pFrame->UpdateByListComp();
	pFrame->GetList()->MoveFirstPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MOVEPAGE_RIGHT()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->MoveNextPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MOVEPAGE_LEFT()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->MovePrevPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MOVEPAGE_RIGHTEND()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->MoveLastPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MOVEPAGE_LEFTEND()
{
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	pFrame->MoveFirstPage();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_BID()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_BidButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_BUY()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_BuyButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_DO_BUY()
{
	CTAuctionCommander::GetInstance()
		->RequestBuy();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_CANCEL_REG()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_RegistCancelButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_DO_CANCEL_REG()
{
	m_pMainWnd->CloseMessageBox();

	CTAuctionCommander::GetInstance()
		->RequestItemRegCancel();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_INSERT_BASKET()
{
	CTAuctionCommander::GetInstance()
		->RequestInsertBasket();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ERASE_BASKET()
{
	CTAuctionCommander::GetInstance()
		->RequestEraseBasket();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MOVE_BASKET_TAB()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();

	pFrame->SetSelect( TAUCTION_BASKET );
	pFrame->SetBasketFrame();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_MAKEABIDFORITEM()
{
	CTAuctionCommander::GetInstance()
		->RequestBid();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_REGIST_MANYITEMS()
{
	CTItemRegBox* pBox = static_cast<CTItemRegBox*>(m_vTFRAME[TFRAME_ITEM_REGBOX]);
	if( pBox->GetCount() < 0 )
		return TERR_NONE;

	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();

	if( pFrame->IsSelect( TAUCTION_REGIST ) )
	{
		CTAuctionRegist* pRegistFrame = static_cast< CTAuctionRegist* >( pFrame->GetInnerFrame( TAUCTION_REGIST ) );
		pRegistFrame->SetItem( pBox->GetItem() );
		BYTE bCount = (BYTE) min( pBox->GetCount(), pRegistFrame->GetItem()->GetCount() );
		pRegistFrame->SetItemCount( bCount );

		DWORD dwRune = pBox->GetRune();
		DWORD dwLuna = pBox->GetLuna();
		DWORD dwCron = pBox->GetCron();

		INT64 ldwPrice = 0;

		CTClientGame::AddUpMoney(
			&ldwPrice,
			dwRune,
			dwLuna,
			dwCron );

		pRegistFrame->SetUnitPrice( ldwPrice );
		pRegistFrame->SetTotalPrice( ldwPrice * bCount );
		pRegistFrame->SetEditEnable( FALSE );
		pRegistFrame->UpdateByComp();
	}

	DisableUI(TFRAME_ITEM_REGBOX);

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_SEARCH()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_SearchButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_SEARCH_RESET()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_SearchResetButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_REGIST()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_RegistButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_DO_REGIST()
{
	m_pMainWnd->CloseMessageBox(TRUE);

	CTAuctionCommander::GetInstance()
		->RequestItemRegist();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_REGIST_RESET()
{
	CTAuctionCommander::GetInstance()
		->OnClicked_RegistResetButton();

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_NAME()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_NAME,
		AAK_ITEMLEVELASC,
		AAK_ITEMLEVELDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_CLASS()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_CLASS,
		AAK_CLASSASC,
		AAK_CLASSDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_LEVEL()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_LEVEL,
		AAK_WEARLEVELASC,
		AAK_WEARLEVELDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_PRICE()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_PRICE,
		AAK_BIDPRICEASC,
		AAK_DIRECTPRICEDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_SELLER()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_SELLER,
		AAK_SELLERNAMEASC,
		AAK_SELLERNAMEDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_LEFTTIME()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_LEFTTIME,
		AAK_REMAINTIMEASC,
		AAK_REMAINTIMEDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_AUCTION_ALIGN_BIDCOUNT()
{
	CTAuctionCommander::GetInstance()->OnClicked_AlignKind(
		CTAuctionMainFrame::TCOLUMN_HEAD_BIDCOUNT,
		AAK_BIDCOUNTASC,
		AAK_BIDCOUNTDESC );

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_WRITE()
{
	if( m_pMainChar->m_dwGuildID == 0 )
		return TERR_GUILD_ONLY_MASTER;

	if( m_pMainChar->m_dwGuildID != 0 &&
		(m_pMainChar->m_bGuildDuty != GUILD_DUTY_VICECHIEF && m_pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF) )
	{
		return TERR_GUILD_NOTDUTY_WANTED_ADD_DEL;
	}

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			static_cast<CTGuildRegularOffer*>( GetFrame( TFRAME_GUILD_REGULAR_OFFER ) )->ResetNewEdit();
			EnableUI( TFRAME_GUILD_REGULAR_OFFER );			
		}
		break;
	case TGM_TACTICS:
		{
			static_cast<CTGuildTacticsOffer*>( GetFrame( TFRAME_GUILD_TACTICS_OFFER ) )->ResetNewEdit();
			EnableUI( TFRAME_GUILD_TACTICS_OFFER );
		}
		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_MODIFY()
{
	if( m_pMainChar->m_dwGuildID == 0 )
		return TERR_GUILD_ONLY_MASTER;

	if( m_pMainChar->m_dwGuildID != 0 &&
		(m_pMainChar->m_bGuildDuty != GUILD_DUTY_VICECHIEF && m_pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF) )
	{
		return TERR_GUILD_NOTDUTY_WANTED_ADD_DEL;
	}

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			if( pCmd->m_GuildOfferVec[ nLastIdx ].m_dwGuildID == m_pMainChar->m_dwGuildID )
			{
				static_cast<CTGuildRegularOffer*>( GetFrame( TFRAME_GUILD_REGULAR_OFFER ) )
					->ResetModifyInfo( pCmd->m_GuildOfferVec[ nLastIdx ] );

				EnableUI( TFRAME_GUILD_REGULAR_OFFER );
			}
			else
				return TERR_GUILD_NOT_OWN_WANTED;
		}
		break;

	case TGM_TACTICS:
		{
			if( pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwGuildID == m_pMainChar->m_dwGuildID )
			{
				static_cast<CTGuildTacticsOffer*>( GetFrame( TFRAME_GUILD_TACTICS_OFFER ) )
					->ResetModifyInfo( pCmd->m_GuildTacticsOfferVec[ nLastIdx ] );

				EnableUI( TFRAME_GUILD_TACTICS_OFFER );
			}
			else
				return TERR_GUILD_NOT_OWN_WANTED;
		}
		break;
	}

	pDlg->RequestInfo();

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_DELETE()
{
	if( m_pMainChar->m_dwGuildID == 0 )
		return TERR_GUILD_ONLY_MASTER;

	if( m_pMainChar->m_dwGuildID != 0 &&
		(m_pMainChar->m_bGuildDuty != GUILD_DUTY_VICECHIEF && m_pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF) )
	{
		return TERR_GUILD_NOTDUTY_WANTED_ADD_DEL;
	}

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			if( pCmd->m_GuildOfferVec[ nLastIdx ].m_dwGuildID != m_pMainChar->m_dwGuildID )
				return TERR_GUILD_NOT_OWN_WANTED;
		}
		break;

	case TGM_TACTICS:
		{
			if( pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwGuildID != m_pMainChar->m_dwGuildID )
				return TERR_GUILD_NOT_OWN_WANTED;
		}
		break;
	}

	m_pMainWnd->MessageBoxYesNo(
		TSTR_GUILD_OFFER_BBS_DELETE_WARN,
		TSTR_YES,
		TSTR_NO,
		GM_GUILD_OFFER_BBS_DO_DELETE,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		ID_SND_ERROR );

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_DO_DELETE()
{
	m_pMainWnd->CloseMessageBox();

	if( m_pMainChar->m_dwGuildID == 0 )
		return TERR_NONE;

	if( m_pMainChar->m_bGuildDuty != GUILD_DUTY_VICECHIEF &&
		m_pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF )
		return TERR_NONE;

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			if( pCmd->m_GuildOfferVec[ nLastIdx ].m_dwGuildID == m_pMainChar->m_dwGuildID )
			{
				if( m_pSESSION )
					m_pSESSION->SendCS_GUILDWANTEDDEL_REQ( pCmd->m_GuildOfferVec[ nLastIdx ].m_dwGuildID );
			}
		}
		break;

	case TGM_TACTICS:
		{
			if( pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwGuildID == m_pMainChar->m_dwGuildID )
			{
				if( m_pSESSION )
					m_pSESSION->SendCS_GUILDTACTICSWANTEDDEL_REQ( 
						//pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwGuildID,
						pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwID );
			}
		}
		break;
	}

	pDlg->RequestInfo();
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_MERCENARY_TAB1()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	pFrame->ResetTabButton( TGM_REGULAR );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_MERCENARY_TAB2()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	pFrame->ResetTabButton( TGM_TACTICS );
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_GUILDMEMBER()
{
	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	pDlg->ChangePage( TGM_REGULAR );
	pDlg->RequestInfo();
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_TACTICS()
{
	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	pDlg->ChangePage( TGM_TACTICS );
	pDlg->RequestInfo();
	return TERR_NONE;
}

int CTClientGame::OnGM_GVFPM_MAIL()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	return pFrame->OnGM_GVFPM_MAIL();
}

int CTClientGame::OnGM_GVFPM_REGULAR_ACCEPT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	return pFrame->OnGM_GVFPM_REGULAR_ACCEPT();
}

int CTClientGame::OnGM_GVFPM_REGULAR_REJECT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	return pFrame->OnGM_GVFPM_REGULAR_REJECT();
}

int CTClientGame::OnGM_GVFPM_TACTICS_ACCEPT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	return pFrame->OnGM_GVFPM_TACTICS_ACCEPT();
}

int CTClientGame::OnGM_GVFPM_TACTICS_REJECT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	return pFrame->OnGM_GVFPM_TACTICS_REJECT();
}

int CTClientGame::OnGM_GVFPM_CLOSE()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	pFrame->CancelPopup();
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_INVITATION_REGULAR_OK()
{
	CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();
	CTGuildRegularOffer* pDlg = static_cast<CTGuildRegularOffer*>( m_vTFRAME[ TFRAME_GUILD_REGULAR_OFFER ] );

	if( pDlg->GetMode() == 1 )  // Edit
	{
		if( pDlg->m_pEditArticleTitle->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_TITLE; // 제목을 입력하세요.

		if( pDlg->m_pEditLevelMin->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MINLEVEL; // 최소레벨을 입력하세요.

		if( pDlg->m_pEditLevelMax->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MAXLEVEL;  // 최대레벨을 입력하세요.

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_GUILDWANTEDADD_REQ(
				pDlg->m_dwArticleID,
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditArticleTitle, TRUE, MAX_BOARD_TITLE),
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditContent, TRUE, MAX_BOARD_TEXT),
				atoi( (LPCTSTR) pDlg->m_pEditLevelMin->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditLevelMax->m_strText ) );
		}
	}
	else if( pDlg->GetMode() == 2 ) // Modify
	{
		if( pDlg->m_pEditArticleTitle->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_TITLE; // 제목을 입력하세요.

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_GUILDWANTEDADD_REQ(
				pDlg->m_dwArticleID,
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditArticleTitle, TRUE, MAX_BOARD_TITLE),
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditContent, TRUE, MAX_BOARD_TEXT),
				atoi( (LPCTSTR) pDlg->m_pViewLevelMin->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewLevelMax->m_strText ) );
		}
	}

	return OnGM_CLOSE_UI();
}

int CTClientGame::OnGM_GUILD_INVITATION_TACTICS_OK()
{
	CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();
	CTGuildTacticsOffer* pDlg = static_cast<CTGuildTacticsOffer*>( m_vTFRAME[ TFRAME_GUILD_TACTICS_OFFER ] );

	if( pDlg->GetMode() == 1 ) // Edit
	{
		if( pDlg->m_pEditArticleTitle->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_TITLE;

		if( pDlg->m_pEditPeriod->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_DAY;

		if( pDlg->m_pEditLevelMin->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MINLEVEL;

		if( pDlg->m_pEditLevelMax->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MAXLEVEL;

		if( pDlg->m_pEditHornor->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_POINT;

		if( pDlg->m_pEditRune->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MONEY;

		if( pDlg->m_pEditLuna->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MONEY;

		if( pDlg->m_pEditCron->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_MONEY;
				
		if( m_pSESSION )
		{
			m_pSESSION->SendCS_GUILDTACTICSWANTEDADD_REQ(
				pDlg->m_dwArticleID,
				//pDlg->m_pEditArticleTitle->m_strText,
				//pDlg->m_pEditContent->m_strText,
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditArticleTitle, TRUE, MAX_BOARD_TITLE),
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditContent, TRUE, MAX_BOARD_TEXT),
				atoi( (LPCTSTR) pDlg->m_pEditPeriod->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditLevelMin->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditLevelMax->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditHornor->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditRune->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditLuna->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pEditCron->m_strText ) );
		}
	}
	if( pDlg->GetMode() == 2 ) // Modify
	{
		if( pDlg->m_pEditArticleTitle->m_strText.IsEmpty() )
			return TERR_GUILD_NOINPUT_ARTICLE_TITLE;
		
		if( m_pSESSION )
		{
			m_pSESSION->SendCS_GUILDTACTICSWANTEDADD_REQ(
				pDlg->m_dwArticleID,
				//pDlg->m_pEditArticleTitle->m_strText,
				//pDlg->m_pEditContent->m_strText,
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditArticleTitle, TRUE, MAX_BOARD_TITLE),
				pTTextLinker->MakeLinkToNetText( pDlg->m_pEditContent, TRUE, MAX_BOARD_TEXT),
				atoi( (LPCTSTR) pDlg->m_pViewPeriod->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewLevelMin->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewLevelMax->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewHornor->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewRune->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewLuna->m_strText ),
				atoi( (LPCTSTR) pDlg->m_pViewCron->m_strText ) );
		}
	}

	return OnGM_CLOSE_UI();
}

int CTClientGame::OnGM_GMP_TACTICS_RE_CONTRACT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_TACTICS_RE_CONTRACT();
}

int CTClientGame::OnGM_GMP_TACTICS_KICK()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_TACTICS_KICK();
}

int CTClientGame::OnGM_GMP_TACTICS_REWARD_POINT()
{
	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	return pFrame->OnGM_GMP_TACTICS_REWARD_POINT();
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_JOIN()
{
	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			m_pMainWnd->MessageBoxYesNo(
				TSTR_GUILD_DO_VOLUNTEERING, 
				TSTR_YES,
				TSTR_NO,
				GM_GUILD_OFFER_BBS_JOIN_DO,
				GM_CLOSE_MSGBOX,
				0,
				FALSE);
		}
		break;
	case TGM_TACTICS:
		{
			m_pMainWnd->MessageBoxYesNo(
				TSTR_GUILD_DO_TACTICS_VOLUNTEERING, 
				TSTR_YES,
				TSTR_NO,
				GM_GUILD_OFFER_BBS_JOIN_DO,
				GM_CLOSE_MSGBOX,
				0,
				FALSE);
		}
		break;
	};

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_JOIN_DO()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			if( m_pSESSION )
				m_pSESSION->SendCS_GUILDVOLUNTEERING_REQ( pCmd->m_GuildOfferVec[ nLastIdx ].m_dwGuildID );
		}
		break;

	case TGM_TACTICS:
		{
			if( m_pSESSION )
				m_pSESSION->SendCS_GUILDTACTICSVOLUNTEERING_REQ(
					pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwGuildID,
					pCmd->m_GuildTacticsOfferVec[ nLastIdx ].m_dwID );
		}
		break;
	}

	pDlg->RequestInfo();

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_JOINCANCEL()
{
	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			m_pMainWnd->MessageBoxYesNo(
				TSTR_GUILD_DO_VOLUNTEERING_CANCEL, 
				TSTR_YES,
				TSTR_NO,
				GM_GUILD_OFFER_BBS_JOINCANCEL_DO,
				GM_CLOSE_MSGBOX,
				0,
				FALSE);
		}
		break;
	case TGM_TACTICS:
		{
			m_pMainWnd->MessageBoxYesNo(
				TSTR_GUILD_DO_TACTICS_VOLUNTEERING_CANCEL, 
				TSTR_YES,
				TSTR_NO,
				GM_GUILD_OFFER_BBS_JOINCANCEL_DO,
				GM_CLOSE_MSGBOX,
				0,
				FALSE);
		}
		break;
	};

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_JOINCANCEL_DO()
{
	m_pMainWnd->CloseMessageBox();

	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	int nLastIdx = pDlg->m_pList->GetSel();
	if( nLastIdx < 0 )
		return TERR_GUILD_SELECT_WANTED;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	switch( pDlg->m_nCurPage )
	{
	case TGM_REGULAR:
		{
			if( m_pSESSION )
				m_pSESSION->SendCS_GUILDVOLUNTEERINGDEL_REQ();
		}
		break;

	case TGM_TACTICS:
		{
			if( m_pSESSION )
				m_pSESSION->SendCS_GUILDTACTICSVOLUNTEERINGDEL_REQ();
		}
		break;
	}

	pDlg->RequestInfo();

	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_OFFER_BBS_REFRESH()
{
	CTGuildApplicationDlg* pDlg = static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] );
	pDlg->RequestInfo();
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILD_SHOW_MARKDLG()
{
	EnableUI( TFRAME_GUILDMARK );
	return TERR_NONE;
}

#ifdef ADD_TEXTOOL
int CTClientGame::OnGM_TEXTOOL_BACK()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->m_bShowBACK1 = !pTT->m_bShowBACK1;
	pTT->GetCursor( 0 ).m_bVisible = pTT->m_bShowBACK1;
	pTT->GetCursor( 1 ).m_bVisible = pTT->m_bShowBACK1;
	pTT->GetCursor( 2 ).m_bVisible = pTT->m_bShowBACK1;
	pTT->GetCursor( 3 ).m_bVisible = pTT->m_bShowBACK1;
	pTT->m_pBtnUV1->SetTextClr( pTT->m_bShowBACK1 ? D3DCOLOR_XRGB(255,0,0) : D3DCOLOR_XRGB(210,210,210) );

	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_BACK2()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->m_bShowBACK2 = !pTT->m_bShowBACK2;
	pTT->GetCursor( 4 ).m_bVisible = pTT->m_bShowBACK2;
	pTT->GetCursor( 5 ).m_bVisible = pTT->m_bShowBACK2;
	pTT->GetCursor( 6 ).m_bVisible = pTT->m_bShowBACK2;
	pTT->GetCursor( 7 ).m_bVisible = pTT->m_bShowBACK2;
	pTT->m_pBtnUV2->SetTextClr( pTT->m_bShowBACK2 ? D3DCOLOR_XRGB(0,0,255) : D3DCOLOR_XRGB(210,210,210) );
	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_CENTERBOTTOM()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	
	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 128, -255 );
	
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_CENTERCENTER()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 128, -128 );
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_CENTERTOP()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 128, 0 );
	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_EXT1()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->ClearCursorSelected();
	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_DOWN()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->SetSelectedCursorPosY( -255 );

	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT2() //수직정렬
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->AlignV();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT4() //수평정렬
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->AlignH();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_LEFT()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->SetSelectedCursorPosX( 0 );
	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_LEFTBOTTOM()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 0, -255 );

	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_LEFTCENTER()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	
	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 0, -128 );

	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_LEFTTOP()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 0, 0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_MARK()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->m_bShowMARK = !pTT->m_bShowMARK;
	pTT->GetCursor( 8 ).m_bVisible = pTT->m_bShowMARK;
	pTT->GetCursor( 9 ).m_bVisible = pTT->m_bShowMARK;
	pTT->GetCursor( 10 ).m_bVisible = pTT->m_bShowMARK;
	pTT->GetCursor( 11 ).m_bVisible = pTT->m_bShowMARK;
	pTT->m_pBtnUV3->SetTextClr( pTT->m_bShowMARK ? D3DCOLOR_XRGB(0,255,0) : D3DCOLOR_XRGB(210,210,210) );
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT10()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	if( m_pMainChar->m_pGuildMark )
		pTT->ApplyToCharacter();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT3()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->DefaultCursor();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT5()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->Undo();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT6()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->Copy();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT8()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->Paste();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT7()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->Redo();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_RIGHT()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->SetSelectedCursorPosX( 255 );

	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_RIGHTBOTTOM()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 255, -255 );

	return TERR_NONE;
}
int CTClientGame::OnGM_TEXTOOL_RIGHTCENTER()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 255, -128 );

	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_RIGHTTOP()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	if( pTT->GetSelectedCursorSize() == 1 )
		pTT->SetSelectedCursorPosXY( 255, 0 );

	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_EXT9()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
	pTT->Save();
	return TERR_NONE;
}

int CTClientGame::OnGM_TEXTOOL_UP()
{
	CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );

	pTT->SetSelectedCursorPosY( 0 );

	return TERR_NONE;
}
#endif

int CTClientGame::OnGM_CLOSE_MSGBOX_WITH_RESETCONMODE()
{
	m_pMainWnd->CloseMessageBox();
	
	if( CTNationOption::RUSSIA )
	{
		TFrame *pFRAME = m_pMainWnd->m_MessageBox.FindFrame(ID_FRAME_2BTNMSGBOX);
		TComponent *pEDIT = pFRAME->FindKid(ID_CTRLINST_EDIT);
		pEDIT->m_style &= ~(TES_NOCONMODE);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_COLOR_MANTLE()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_MAKE_GUILDMARK()
{
	if( m_pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
		EnableUI( TFRAME_GUILDMARK );
	else
		m_pMainWnd->MessageBoxOK(
			TSTR_ONLY_USE_GUILDMASTER,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			ID_SND_ITEM_FAILED,
			MEDIA_TSOUND );

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_UNREG_GUILDMARK()
{
	if( m_pMainChar->m_dwGuildID != 0 )
	{
		CTItemRegGuildDlg* pDlg = static_cast<CTItemRegGuildDlg*>( m_vTFRAME[TFRAME_ITEM_REGGM] );
		pDlg->SetRegMode( 0 );
		EnableUI( TFRAME_ITEM_REGGM );
	}
	else
	{
		return TERR_NEED_JOIN_GUILD;
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_NPC_REG_GUILDMARK()
{
	if( m_pMainChar->m_dwGuildID != 0 )
	{
		CTItemRegGuildDlg* pDlg = static_cast<CTItemRegGuildDlg*>( m_vTFRAME[TFRAME_ITEM_REGGM] );
		pDlg->SetRegMode( 1 );
		EnableUI( TFRAME_ITEM_REGGM );
	}
	else
		return TERR_NEED_JOIN_GUILD;

	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMCOLOR_READY()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMCOLOR_SELECT()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_ITEMCOLOR_DO()
{
	/*OnGM_CLOSE_UI();

	CTItemColorDlg* pDlg = static_cast<CTItemColorDlg*>( m_vTFRAME[TFRAME_ITEM_COLOR] );
	WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;
	CTClientItem* pScroll = pDlg->GetLastClientItem(CTItemColorDlg::SCROLL_SLOT);
	LPTITEM pTITEM = pScroll->GetTITEM();

	if( pTITEM && pTITEM->m_wUseValue != 0 )
	{
		CTColorPicker* Picker = static_cast<CTColorPicker*>( m_vTFRAME[ TFRAME_COLOR_PICKER ] );

		m_pSESSION->SendCS_ITEMUPGRADE_REQ(
			pDlg->GetLastInvenID(CTItemColorDlg::TARGET_SLOT), pDlg->GetLastInvenSlot(CTItemColorDlg::TARGET_SLOT),
			pDlg->GetLastInvenID(CTItemColorDlg::SCROLL_SLOT), pDlg->GetLastInvenSlot(CTItemColorDlg::SCROLL_SLOT),
			wNpcID,
			INVEN_NULL,
			INVALID_SLOT,
			Picker->m_wSelectedColor );
	}*/

	return TERR_NONE;
}

int CTClientGame::OnGM_REGGM_CHECK()
{
	CTItemRegGuildDlg* pDlg = static_cast<CTItemRegGuildDlg*>( m_vTFRAME[TFRAME_ITEM_REGGM] );
	LPTITEM pUpItem = pDlg->GetItem( CTItemUpDlg::TARGET_SLOT );

	if( !pUpItem )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_REGGUILDDLG_NO_TARGET_ITEM,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );

		return TERR_NONE;
	}

	DisableUI( TFRAME_ITEM_REGGM );
	pDlg->SetItem(CTItemRegGuildDlg::TARGET_SLOT, NULL);

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_REGGUILDCLOAK_REQ(
			pDlg->GetLastInvenID(CTItemRegGuildDlg::TARGET_SLOT),
			pDlg->GetLastInvenSlot(CTItemRegGuildDlg::TARGET_SLOT),
			TRUE );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_UNREGGM_CHECK()
{
	CTItemRegGuildDlg* pDlg = static_cast<CTItemRegGuildDlg*>( m_vTFRAME[TFRAME_ITEM_REGGM] );
	LPTITEM pUpItem = pDlg->GetItem( CTItemUpDlg::TARGET_SLOT );

	if( !pUpItem )
	{
		m_pMainWnd->MessageBoxOK(
			TSTR_REGGUILDDLG_NO_UNREG_TARGET_ITEM,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );

		return TERR_NONE;
	}

	DisableUI( TFRAME_ITEM_REGGM );
	pDlg->SetItem(CTItemRegGuildDlg::TARGET_SLOT, NULL);

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_REGGUILDCLOAK_REQ(
			pDlg->GetLastInvenID(CTItemRegGuildDlg::TARGET_SLOT),
			pDlg->GetLastInvenSlot(CTItemRegGuildDlg::TARGET_SLOT),
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_APPLY_GUILDMARK()
{
	CTGuildMarkMakeDlg* pDlg = static_cast<CTGuildMarkMakeDlg*>( m_vTFRAME[TFRAME_GUILDMARK] );

	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(INVEN_EQUIP);
	if( itTINVEN != m_pMainChar->m_mapTINVEN.end() )
	{
		CTClientInven *pTEQUIP = (*itTINVEN).second;
		CTClientItem* pTITEM = pTEQUIP->FindTItem( ES_BACK );
		if( pTITEM )
		{
			CTClientGuildMark* pGuildMark = pDlg->GetGuildMark();

			if( pGuildMark )
			{
				CTClientGuildMark::SetMantleTexture(
					m_pMainChar,
					pGuildMark );
			}
		}
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_GUILDMARK_CANCEL()
{
	OnGM_CLOSE_UI();
	//m_pMainChar->ApplyMantleTexture();
	return TERR_NONE;
}

int CTClientGame::OnGM_FAMERANK_MONTH_LEFT()
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[ TFRAME_FAME_BASE ] );

	if ( m_pSESSION )
	{
		int n = pDlg->m_bMonth;
		--n;
		if( n <= 0 )
			n = 12;
		pDlg->m_bMonth = n;

		m_pSESSION->SendCS_FAMERANKLIST_REQ( pDlg->m_bType, pDlg->m_bMonth );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_FAMERANK_MONTH_RIGHT()
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[ TFRAME_FAME_BASE ] );

	if ( m_pSESSION )
	{
		++pDlg->m_bMonth;
		if( pDlg->m_bMonth > 12 )
			pDlg->m_bMonth = 1;

		m_pSESSION->SendCS_FAMERANKLIST_REQ( pDlg->m_bType, pDlg->m_bMonth );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOGGLE_FAMERANK()
{
	ToggleUI( TFRAME_FAME_BASE );
	return TERR_NONE;
}

int CTClientGame::OnGM_HERO_FAME()
{
	CTFameBaseDlg *pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );
	pDlg->m_bType = FRT_HERO;
	pDlg->OnEnable();
	EnableUI( TFRAME_FAME_BASE );
    return TERR_NONE;
}

int CTClientGame::OnGM_KNIGHT_FAME()
{
	CTFameBaseDlg *pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );
	pDlg->m_bType = FRT_GODDESS;
	pDlg->OnEnable();
	EnableUI( TFRAME_FAME_BASE );
    return TERR_NONE;
}

int CTClientGame::OnGM_GOD_FAME()
{
	CTFameBaseDlg *pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );
	pDlg->m_bType = FRT_GOD;
	pDlg->OnEnable();
	EnableUI( TFRAME_FAME_BASE );
    return TERR_NONE;
}

int CTClientGame::OnGM_FIRSTGRADEGROUP_FAME()
{
	CTFameBaseDlg *pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );
	pDlg->m_bType = FRT_1ST;
	pDlg->OnEnable();
	EnableUI( TFRAME_FAME_BASE );
    return TERR_NONE;
}

int CTClientGame::OnGM_MONTH_FAME()
{
	if ( m_pSESSION )
		m_pSESSION->SendCS_MONTHRANKLIST_REQ();
	return TERR_NONE;
}

int CTClientGame::OnGM_WARLORD_SAY_CHANGE()
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[ TFRAME_FAME_BASE ] );
	if ( pDlg->m_pWarLord->dwCharID == m_pMainChar->m_dwID )
	{
		m_pMainWnd->MessageBox(
			TSTR_WARLORD_SAY_MSG,
			0, 0,
			TSTR_YES,
			TSTR_NO,
			ID_FRAME_2BTNMSGBOX,
			GM_HERO_SAY_CHANGE,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_HERO_SAY_CHANGE()
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );

	CString strSAY = m_pMainWnd->GetMsgBoxStrInEditor(ID_FRAME_2BTNMSGBOX);
	m_pMainWnd->CloseMessageBox();

	if ( strSAY.GetLength() > 255 )
	{
		m_pMainWnd->MessageBox( 
			TSTR_ERR_SAY_SIZE,
			0, 0,
			TSTR_OK,
			TSTR_OK,
			ID_FRAME_1BTNMSGBOX,
			GM_CLOSE_MSGBOX,
			GM_CLOSE_MSGBOX,
			0,
			FALSE,
			TRUE);
	}
	else
	{
//		strSAY = CCurse::ConvertString( strSAY, FALSE );
		if( !CTStrFilter::ConvertString( strSAY, false ) )
			return TERR_NONE;

		if( m_pSESSION )
		{
			m_pSESSION->SendCS_WARLORDSAY_REQ( pDlg->m_bType, strSAY );
			m_pSESSION->SendCS_FAMERANKLIST_REQ( pDlg->m_bType, pDlg->m_bMonth );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_KNIGHT_SAY_CHANGE()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_GOD_SAY_CHANGE()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_RANK_INFO()
{
	CTCharRankInfoDlg *pDlg = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[TFRAME_RANK_INFO] );

	pDlg->ResetTabIndex( TINFO_RANK );

	if( m_pSESSION )
		m_pSESSION->SendCS_MONTHRANKLIST_REQ();

	if( m_pSESSION )
		m_pSESSION->SendCS_FIRSTGRADEGROUP_REQ();

	return TERR_NONE;
}

int CTClientGame::OnGM_RANK_INFO_TOTAL_TAB()
{
	CTCharRankInfoDlg* pDlg = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[TFRAME_RANK_INFO] );

	CTCharRankInfoDlg::m_bTabIndex = CTCharRankInfoDlg::TRANKINFO_TAB_TOTAL;
	pDlg->ResetData( CTCharRankInfoDlg::m_bTabIndex, CTCharInfoDlg::m_bTabIndex );
	
	return TERR_NONE;
}

int CTClientGame::OnGM_RANK_INFO_COUNTRY_TAB()
{
	CTCharRankInfoDlg* pDlg = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[TFRAME_RANK_INFO] );
	
	CTCharRankInfoDlg::m_bTabIndex = CTCharRankInfoDlg::TRANKINFO_TAB_COUNTRY;
	pDlg->ResetData( CTCharRankInfoDlg::m_bTabIndex, CTCharInfoDlg::m_bTabIndex );
	
	return TERR_NONE;
}

int CTClientGame::OnGM_RANK_INFO_TOURNAMENT_TAB()
{
	CTCharRankInfoDlg* pDlg = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[TFRAME_RANK_INFO] );

	CTCharRankInfoDlg::m_bTabIndex = CTCharRankInfoDlg::TRANKINFO_TAB_TOURNAMENT;
	pDlg->ResetData( CTCharRankInfoDlg::m_bTabIndex, CTCharInfoDlg::m_bTabIndex );

	return TERR_NONE;
}

int CTClientGame::OnGM_CAUTION15_CLICKED()
{
	CTClientGame::m_vTOPTION.m_bShowCaution15 = !CTClientGame::m_vTOPTION.m_bShowCaution15;
	static_cast<CTGaugePannel*>( m_vTFRAME[ TFRAME_GAUGE ] )->DettachCaution15();
	static_cast<CTSettingFrame*>( m_vTFRAME[TFRAME_SETTING] )->UpdateUI();
	return TERR_NONE;
}

int CTClientGame::OnGM_CLOSE_RECV_MAIL()
{
	OnGM_CLOSE_UI();
	return OnGM_TOGGLE_MAIL();
}

int CTClientGame::OnGM_TOURNAMENT_ENTRY() // 참가신청
{
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_ENTRY_DO()
{
	m_pMainWnd->CloseMessageBox();

	BYTE bEntryID = (BYTE) m_dwCmdParam;
	if( m_pSESSION )
	{
		m_pSESSION->SendCS_TOURNAMENTAPPLY_REQ( bEntryID );
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_GIVEUP() // 참가포기
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	INT nSel = pDLG->m_pMATCHLIST->GetCurSelIndex();
	if( nSel != -1 )
	{
		MAP_LISTITEM* pItem = pDLG->m_pMATCHLIST->GetCurSelItem();
		MAP_LISTITEM::iterator it = pItem->find( 0 );
		if( it != pItem->end() )
		{
			TMATCH* pMATCH = (TMATCH*) it->second->m_param;
			DWORD dwParam = (DWORD)( pMATCH->m_bEntryID );

			m_pMainWnd->MessageBoxYesNo(
				CTChart::Format(TSTR_TOURNAMENT_GIVE_UP, it->second->m_strText),
				TSTR_YES,
				TSTR_NO,
				TCOMMAND( GM_TOURNAMENT_GIVEUP_DO, dwParam ),
				GM_CLOSE_MSGBOX,
				GM_CLOSE_MSGBOX,
				FALSE );
		}
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_GIVEUP_DO()
{
	m_pMainWnd->CloseMessageBox();

	BYTE bEntryID = (BYTE) m_dwCmdParam;
	if( m_pSESSION )
	{
		m_pSESSION->SendCS_TOURNAMENTAPPLYCANCEL_REQ();
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_EVENT_SELECT_PLAYER() // 이벤트결정
{
	CTTournamentEvent* pDLG = static_cast<CTTournamentEvent*>( m_vTFRAME[ TFRAME_TOURNAMENT_EVENT ]);
	switch( pDLG->m_nMode )
	{
	case TTEVENT_SELECT_EVENT:
		{
			int nEventSel = pDLG->m_pEVENTLIST->GetCurSelIndex();
			if( nEventSel == -1 )
				return TERR_NONE;

			LPTTEVENTMATCH pEVENT = (LPTTEVENTMATCH) pDLG->m_pEVENTLIST->GetItemData( nEventSel, 0 );
			if( pEVENT )
				m_pSESSION->SendCS_TOURNAMENTEVENTINFO_REQ(pEVENT->m_bEntryID);
		}
		break;

	case TTEVENT_SELECT_ENTRY:
		{
			int nEventSel = pDLG->m_pEVENTLIST->GetCurSelIndex();
			if( nEventSel == -1 )
			{
				return TERR_NONE;
			}

			int nEntrySel = pDLG->m_pENTRYLIST->GetCurSelIndex();
			if( nEntrySel == -1 )
			{
				return TERR_NONE;
			}

			LPTTEVENTMATCH pEVENT = (LPTTEVENTMATCH) pDLG->m_pEVENTLIST->GetItemData( nEventSel, 0 );
			LPTTEVENTTARGET pENTRY = (LPTTEVENTTARGET) pDLG->m_pENTRYLIST->GetItemData( nEntrySel, 0 );

			if( pEVENT && pENTRY )
			{
				DWORD dwParam = MAKELONG( (WORD)(nEventSel), (WORD)(nEntrySel) );

				m_pMainWnd->MessageBoxYesNo(
					CTChart::Format( TSTR_TOURNAMENT_BATTING, pEVENT->m_strEntry, pENTRY->m_strName),
					TSTR_YES,
					TSTR_NO,
					TCOMMAND( GM_TOURNAMENT_EVENT_SELECT_PLAYER_DO, dwParam ),
					GM_CLOSE_MSGBOX,
					GM_CLOSE_MSGBOX,
					FALSE );
			}
		}
		break;
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_EVENT_SELECT_PLAYER_DO()
{
	m_pMainWnd->CloseMessageBox();

	CTTournamentEvent* pDLG = static_cast<CTTournamentEvent*>( m_vTFRAME[TFRAME_TOURNAMENT_EVENT] );
	WORD wEventSel = LOWORD(m_dwCmdParam);
	WORD wEntrySel = HIWORD(m_dwCmdParam);

	LPTTEVENTMATCH pEVENT = (LPTTEVENTMATCH) pDLG->m_pEVENTLIST->GetItemData( wEventSel, 0 );
	LPTTEVENTTARGET pENTRY = (LPTTEVENTTARGET) pDLG->m_pENTRYLIST->GetItemData( wEntrySel, 0 );

	if( pEVENT && pENTRY )
	{
		if( m_pSESSION )
			m_pSESSION->SendCS_TOURNAMENTEVENTJOIN_REQ( pEVENT->m_bEntryID, pENTRY->m_dwCharID );
	}

	DisableUI(TFRAME_TOURNAMENT_EVENT);
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_EVENT_PREV()
{
	CTTournamentEvent* pDLG = static_cast<CTTournamentEvent*>( m_vTFRAME[ TFRAME_TOURNAMENT_EVENT ] );
	pDLG->SetMode( TTEVENT_SELECT_EVENT );
	pDLG->m_pEVENTLIST->RemoveAll();
	m_pSESSION->SendCS_TOURNAMENTEVENTLIST_REQ();
	pDLG->ResetShow();
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_ADD_PARTY() // 파티원 추가
{
	CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] );
	pDLG->SetAddPartyMode();

	if( pDLG->IsVisible() )
		SwitchFocus( m_vTFRAME[TFRAME_TOURNAMENT_SELECT_TARGET] );
	else
		EnableUI( TFRAME_TOURNAMENT_SELECT_TARGET );

	CTTournament* pTDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pTDLG->m_pENTRY_PLAYER_LIST->SetCurSelItem(0);

	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_REMOVE_PARTY() // 파티원 삭제
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );

	int nSel = pDLG->m_pENTRY_PLAYER_LIST->GetCurSelIndex();
	if( nSel == -1 )
		return TERR_NONE;

	if( m_dwCmdParam >= pDLG->m_pENTRY_PARTY_LIST->GetItemCount() )
		return TERR_NONE;
	
	CString strPartyLeaderName = pDLG->m_pENTRY_PLAYER_LIST->GetItemString(nSel, 0);
	if( strPartyLeaderName.IsEmpty() )
		return TERR_NONE;
	
	LPTMATCHPARTY pPARTY = (LPTMATCHPARTY) pDLG->m_pENTRY_PARTY_LIST->GetItemData( m_dwCmdParam, 0 );
	if( !pPARTY )
		return TERR_NONE;

	if( strPartyLeaderName == m_pMainChar->m_strNAME )
	{
		m_pMainWnd->MessageBoxYesNo(
			CTChart::Format( TSTR_TOURNAMENT_UNJOIN_PARTY, pPARTY->m_strName ),
			TSTR_YES,
			TSTR_NO,
			TCOMMAND( GM_TOURNAMENT_REMOVE_PARTY_DO, pPARTY->m_dwCharID ),
			GM_CLOSE_MSGBOX,
			GM_CLOSE_MSGBOX,
			FALSE );
	}
	else if( pPARTY->m_strName == m_pMainChar->m_strNAME )
	{
		m_pMainWnd->MessageBoxYesNo(
			CTChart::LoadString(TSTR_TOURNAMENT_UNJOIN_PARTY_SELF),
			TSTR_YES,
			TSTR_NO,
			TCOMMAND( GM_TOURNAMENT_REMOVE_PARTY_DO, pPARTY->m_dwCharID ),
			GM_CLOSE_MSGBOX,
			GM_CLOSE_MSGBOX,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_REMOVE_PARTY_DO() // 파티원 삭제
{
	m_pMainWnd->CloseMessageBox();

	if( m_pSESSION )
	{
		m_pSESSION->SendCS_TOURNAMENTPARTYDEL_REQ(
			m_dwCmdParam );
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_SELECT_TARGET() // 대상선택
{
	CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[TFRAME_TOURNAMENT_SELECT_TARGET] );
	pDLG->OnGM_TOURNAMENT_SELECT_TARGET();
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_SHOW_MATCH() // 대진표 보여주기
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_TOURNAMENT_SHOW_MATCH;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		EnableUI( TFRAME_TOURNAMENT );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_SHOW_EVENT()
{
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		m_bMoveType = TMAINMOVE_TARGET;
		m_dwMoveGM = GM_TOURNAMENT_SHOW_EVENT;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		EnableUI( TFRAME_TOURNAMENT_EVENT );
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_SELECT_TARGET_GUILD()
{
	CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] );
	pDLG->ChangeTab(TTST_TAB_GUILD);
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_SELECT_TARGET_FRIEND()
{
	CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] );
	pDLG->ChangeTab(TTST_TAB_FRIEND);
	return TERR_NONE;
}

// 아래 쓰레기일듯
int CTClientGame::OnGM_TOURNAMENT_SELECT_WINNER()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_GOTO_MATCH()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_OK()
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	return pDLG->OnGM_TOURNAMENT_OK();
}

int CTClientGame::OnGM_TOURNAMENT_CANCEL()
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	return pDLG->OnGM_TOURNAMENT_CANCEL();
}

int CTClientGame::OnGM_TOURNAMENT_SCHEDULE_LEFT()
{
#ifdef MODIFY_TT
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	if( pDLG->m_pCurrentGroup )
	{
		pDLG->m_pCurrentGroup->m_nScheduleTop -= 1;
		if( pDLG->m_pCurrentGroup->m_nScheduleTop < 0 )
			pDLG->m_pCurrentGroup->m_nScheduleTop = 0;
		pDLG->UpdateSchedule();
	}
	return TERR_NONE;
#else
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->m_nScheduleTop -= 1;
	if( pDLG->m_nScheduleTop < 0 )
		pDLG->m_nScheduleTop = 0;
	pDLG->UpdateSchedule();
	return TERR_NONE;
#endif
}

int CTClientGame::OnGM_TOURNAMENT_SCHEDULE_RIGHT()
{
#ifdef MODIFY_TT
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	if( pDLG->m_pCurrentGroup )
	{
		pDLG->m_pCurrentGroup->m_nScheduleTop += 1;
		if( pDLG->m_pCurrentGroup->m_nScheduleTop > pDLG->m_pCurrentGroup->m_vSchedule.size()-6 )
			pDLG->m_pCurrentGroup->m_nScheduleTop = pDLG->m_pCurrentGroup->m_vSchedule.size()-6;
		
		pDLG->UpdateSchedule();
	}
	return TERR_NONE;
#else
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->m_nScheduleTop += 1;
	if( pDLG->m_nScheduleTop > pDLG->m_vSchedule.size()-6 )
		pDLG->m_nScheduleTop = pDLG->m_vSchedule.size()-6;
	pDLG->UpdateSchedule();
	return TERR_NONE;
#endif
}

int CTClientGame::OnGM_TOURNAMENT_TOGGLE_SHOW_PLAYER_INFO()
{
	CTTournamentGage* pDLG = static_cast<CTTournamentGage*>( m_vTFRAME[TFRAME_TOURNAMENT_GAGE] );
	pDLG->m_bShowPlayerInfo = !pDLG->m_bShowPlayerInfo;
	pDLG->m_pTournamentInfo->m_pBtnShowPlayerInfo->Select( (BOOL)pDLG->m_bShowPlayerInfo );
	pDLG->ResetTournamentINFO();
	return TERR_NONE;
}

int CTClientGame::OnGM_FIRSTRANK_MONTH_LEFT()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_FIRSTRANK_MONTH_RIGHT()
{
	return TERR_NONE;
}

int CTClientGame::OnGM_TOURNAMENT_USE_CHEERITEM()
{
	if( m_pSESSION )
	{
		BYTE bInven = 0;
		BYTE bSlot = 0;
		if( FindInven(IT_USE, IK_CHEER, bInven, bSlot) )
		{
			m_pSESSION->SendCS_TOURNAMENTCHEER_REQ(
				m_dwCmdParam,
				bInven,
				bSlot );
		}
		else
		{
			CString strMSG = CTChart::LoadString(TSTR_TOURNAMENT_DONT_HAVE_CHEER_ITEM);
			CString strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

			m_pChatFrame->ChatSysMSG(
				strTYPE,
				m_pMainChar->GetName(),
				strMSG, 
				TCOLOR_ERROR,
				TCHAT_FLAG_INFO,
				ID_SND_ERROR,
				MEDIA_TSOUND);
		}
	}
	return TERR_NONE;
}

int CTClientGame::OnGM_CLICKED_SIEGE_ALARM()
{
	if( m_vMAP.m_wMapID != TUTORIAL_MAPID )
	{
		m_pMainWnd->MessageBoxYesNo(
			TSTR_GOTO_CAPITAL,
			TSTR_YES,
			TSTR_NO,
			GM_CLICKED_SIEGE_ALARM_DO,
			GM_CLOSE_MSGBOX);
	}
	else
	{
		m_pMainWnd->MessageBoxYesNo(
			TSTR_ARE_YOU_SURE_SKIP_TUTORIAL,
			TSTR_YES,
			TSTR_NO,
			GM_CLICKED_STOP_TUTORIAL,
			GM_CLOSE_MSGBOX);
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_CLICKED_STOP_TUTORIAL()
{
	m_pMainWnd->CloseMessageBox(TRUE);
	m_vTSCENE_PLAYER.StartScene(
		m_pDevice,
		m_pRES,
		TSCENE_SCRIPT_LEAVE_TUTORIAL);
	((CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE])->DettachSiegeAlarm();

	return TERR_NONE;
}

int CTClientGame::OnGM_CLICKED_SIEGE_ALARM_DO()
{
	if(m_pSESSION)
		switch(m_pMainChar->m_bContryID)
		{
		case TCONTRY_D	: m_pSESSION->SendCS_TELEPORT_REQ(TSIEGE_D_PORTAL_ID); break;
		case TCONTRY_C	: m_pSESSION->SendCS_TELEPORT_REQ(TSIEGE_C_PORTAL_ID); break;
		}
	((CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE])->DettachSiegeAlarm();

	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_TOURNAMENT()
{
	ToggleUI( TFRAME_TOURNAMENT );
	return TERR_NONE;
}

int CTClientGame::OnGM_SHOW_TOURNAMENT_EVENT()
{
	ToggleUI( TFRAME_TOURNAMENT_EVENT );
	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_LEFT()
{
	static_cast<CTHelpFrame*>(m_vTFRAME[TFRAME_HELP])->LeftPage();
	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_RIGHT()
{
	static_cast<CTHelpFrame*>(m_vTFRAME[TFRAME_HELP])->RightPage();
	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_UP()
{
	static_cast<CTHelpFrame*>(m_vTFRAME[TFRAME_HELP])->PrevHelp();
	return TERR_NONE;
}

int CTClientGame::OnGM_HELP_DOWN()
{
	static_cast<CTHelpFrame*>(m_vTFRAME[TFRAME_HELP])->NextHelp();
	return TERR_NONE;
}

int CTClientGame::OnGM_SKIP_SCENE()
{
	CTCinematicDlg *pTCINEMATIC = (CTCinematicDlg *) m_vTFRAME[TFRAME_CINEMATIC];

	if( pTCINEMATIC && pTCINEMATIC->IsVisible() && pTCINEMATIC->m_bTSKIP )
	{
		m_vTSCENE_PLAYER.StartScene(
			m_pDevice,
			m_pRES,
			TSCENE_SCRIPT_SKIP);
	}

	return TERR_NONE;
}

#ifdef ADD_RPS
int CTClientGame::OnGM_RPS_1()
{
	if( m_bRPSGaming )
		return TERR_NONE;

	m_dwCmdParam = 1;
	OnGM_RPS_ASK_COST();
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_10()
{
	if( m_bRPSGaming )
		return TERR_NONE;

	m_dwCmdParam = 2;
	OnGM_RPS_ASK_COST();
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_50()
{
	if( m_bRPSGaming )
		return TERR_NONE;

	m_dwCmdParam = 3;
	OnGM_RPS_ASK_COST();
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_GIVEUP()
{
	DisableUI( TFRAME_RPS );
	DisableUI( TFRAME_RPS_REWARD );

	m_bRPSGaming = FALSE;
	GetSession()->SendCS_RPSGAME_REQ( RPSCARD_COUNT ); // 포기
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_TRY()
{
	m_pMainWnd->CloseMessageBox(TRUE);

	DisableUI( TFRAME_RPS_REWARD );

	CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[ TFRAME_RPS ] );
	pDLG->m_bSelectMode = TRUE;

	EnableUI(TFRAME_RPS);
	m_bRPSKeyEnable = TRUE;
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_SELECT_1()
{
	m_bRPSKeyEnable = FALSE;
	CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[ TFRAME_RPS ] );
	GetSession()->SendCS_RPSGAME_REQ( RPSCARD_SCISSORS );
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_SELECT_2()
{
	m_bRPSKeyEnable = FALSE;
	CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[ TFRAME_RPS ] );
	GetSession()->SendCS_RPSGAME_REQ( RPSCARD_ROCK );
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_SELECT_3()
{
	m_bRPSKeyEnable = FALSE;
	CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[ TFRAME_RPS ] );
	GetSession()->SendCS_RPSGAME_REQ( RPSCARD_PAPER );
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_START()
{
	m_pMainWnd->CloseMessageBox(FALSE);
	
	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		return TERR_NONE;
	}

	m_dwRPSGameType = (BYTE) m_dwCmdParam;
	m_bRPSWinCount = 0;
	m_bRPSKeyEnable = FALSE;
	m_bRPSGaming = FALSE;

	LPTRPS pRps = CTChart::FindTRPS( (BYTE) m_dwRPSGameType, 0);
	if( pRps )
	{
        BYTE bInvenID = 0;
		BYTE bItemID = 0;
		if( CheckInvenItem( pRps->wNeedItem, &bInvenID, &bItemID) )
		{
			GetSession()->SendCS_RPSSTART_REQ(
				(BYTE)m_dwRPSGameType,
				bInvenID,
				bItemID );
		}
		else
		{
			LPTITEM pTItem = CTChart::FindTITEMTEMP( pRps->wNeedItem );
			if( pTItem )
			{
				LPTITEMVISUAL pTItemVisual = CTChart::FindTITEMVISUAL( pTItem->m_wVisual[0] );
				if( pTItem && pTItemVisual )
				{
					m_pMainWnd->MessageBoxItemOK(
						CTChart::LoadString(TSTR_RPSGAME_NEED_A_ITEM),
						TSTR_OK,
						GM_CLOSE_MSGBOX,
						GM_CLOSE_MSGBOX,
						pTItemVisual->m_wIcon,
						pTItem->m_strNAME );
				}
			}

			DisableUI( TFRAME_RPS );
			m_bRPSWinCount = 0;
			m_bRPSKeyEnable = FALSE;
			m_bRPSGaming = FALSE;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_END()
{
	m_pMainWnd->CloseMessageBox(TRUE);
	DisableUI( TFRAME_RPS );
	m_bRPSWinCount = 0;
	m_bRPSKeyEnable = FALSE;
	m_bRPSGaming = FALSE;
	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_ASK_COST()
{
	DWORD dwType = m_dwCmdParam;

	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || (
		!m_pTARGET->m_bDrawGhost && (
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		return TERR_NONE;
	}

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ()));

	if( fDist > TARGETUI_DIST )
	{
		switch(dwType)
		{
		case 1: m_dwMoveGM = GM_RPS_1; break;
		case 2: m_dwMoveGM = GM_RPS_10; break;
		case 3: m_dwMoveGM = GM_RPS_50; break;
		default:
			return TERR_NONE;
		}

		m_bMoveType = TMAINMOVE_TARGET;
		m_bAutoRun = FALSE;
		m_fBOUND = POPUP_DIST;
	}
	else
	{
		LPTRPS pRps = CTChart::FindTRPS( (BYTE) dwType, 0);
		if( !pRps )
			return TERR_NONE;

		LPTITEM pTItem = CTChart::FindTITEMTEMP( pRps->wNeedItem );
		if( !pTItem )
			return TERR_NONE;

		LPTITEMVISUAL pTItemVisual = CTChart::FindTITEMVISUAL( pTItem->m_wVisual[0] );
		if( !pTItemVisual )
			return TERR_NONE;

		BYTE bInvenID = 0;
		BYTE bItemID = 0;
		if( CheckInvenItem( pRps->wNeedItem, &bInvenID, &bItemID) )
		{
			m_pMainWnd->MessageBoxItemYesNo(
				CTChart::LoadString(TSTR_RPSGAME_ITEM_TRY),
				TSTR_YES,
				TSTR_NO,
				TCOMMAND( GM_RPS_START, dwType ),
				GM_CLOSE_MSGBOX,
				GM_CLOSE_MSGBOX,
				pTItemVisual->m_wIcon,
				pTItem->m_strNAME,
				TRUE,
				ID_SND_INFO,
				MEDIA_TSOUND,
				TMSGBOXOPEN_NONE	);
		}
		else
		{
			m_pMainWnd->MessageBoxItemOK(
				CTChart::LoadString(TSTR_RPSGAME_NEED_A_ITEM),
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				GM_CLOSE_MSGBOX,
				pTItemVisual->m_wIcon,
				pTItem->m_strNAME );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnGM_RPS_SHOW_REWARD()
{
#ifdef ADD_RPS_RL
	EnableUI(TFRAME_RPS_REWARD_LIST);
#endif
	return TERR_NONE;
}
#endif