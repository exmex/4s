#include "Stdafx.h"
#include "TGuildCommander.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TCommunityDlg.h"
#include "TGuildPvPRecordFrame.h"

// ====================================================================
const DWORD CTGuildCommander::CMD_AUTHORITY[] = 
{
	AUTHORITY_ALL,						///< 길드 모집창 띄우기
	AUTHORITY_SUBMASTER,				///< 길드 초대
	AUTHORITY_MEMBER,					///< 길드 탈퇴
	AUTHORITY_SUBMASTER,				///< 길드원 추방
	AUTHORITY_MASTER,					///< 길드 해체 신청
	AUTHORITY_MASTER,					///< 길드 해체 취소
	AUTHORITY_MASTER,					///< 길드장 위임
	AUTHORITY_MASTER,					///< 부길드장 위임
	AUTHORITY_MASTER,					///< 부길드장 해임
	AUTHORITY_MASTER,					///< 주어진 작위를 취소
	AUTHORITY_MASTER,					///< 남작에 임명
	AUTHORITY_MASTER,					///< 자작에 임명
	AUTHORITY_MASTER,					///< 백작에 임명
	AUTHORITY_MASTER,					///< 후작에 임명
	AUTHORITY_MASTER,					///< 공작에 임명
	AUTHORITY_SUBMASTER,					///< 용병 초대
};
// --------------------------------------------------------------------
const DWORD CTGuildCommander::CMD_STRING[] = 
{
	TSTR_CMD_SHOW_GUILDAPPDLG,			///< 길드 모집창 띄우기
	TSTR_CMD_GUILD_INVITE,				///< 길드 초대
	TSTR_CMD_GUILD_OUT,					///< 길드 탈퇴
	TSTR_CMD_GUILD_KICK,					///< 길드원 추방
	TSTR_CMD_GUILD_DISORG,				///< 길드 해체 신청
	TSTR_CMD_GUILD_DISORGCANCEL,			///< 길드 해체 취소
	TSTR_CMD_GUILD_MASTER,				///< 길드장 위임
	TSTR_CMD_GUILD_SUBMASTER,			///< 부길드장 위임
	TSTR_CMD_GUILD_DISUBMASTER,			///< 부길드장 해임
	TSTR_CMD_GUILD_PEER_CANCEL,			///< 주어진 작위를 취소
	TSTR_CMD_GUILD_BARON,				///< 남작에 임명
	TSTR_CMD_GUILD_VISCOUNT,				///< 자작에 임명
	TSTR_CMD_GUILD_COUNT,				///< 백작에 임명
	TSTR_CMD_GUILD_MARQUIS,				///< 후작에 임명
	TSTR_CMD_GUILD_DUKE,					///< 공작에 임명
	TSTR_CMD_GUILD_INVITE_TACTICS		///<  용병 초대
};
// --------------------------------------------------------------------
const DWORD CTGuildCommander::CMD_DESC_STRING[] = 
{
	TSTR_CMDESC_SHOW_GUILDAPPDLG,		///< 길드 모집창 띄우기
	TSTR_CMDESC_GUILD_INVITE,			///< 길드 초대
	TSTR_CMDESC_GUILD_OUT,				///< 길드 탈퇴
	TSTR_CMDESC_GUILD_KICK,				///< 길드원 추방
	TSTR_CMDESC_GUILD_DISORG,			///< 길드 해체 신청
	TSTR_CMDESC_GUILD_DISORGCANCEL,		///< 길드 해체 취소
	TSTR_CMDESC_GUILD_MASTER,			///< 길드장 위임
	TSTR_CMDESC_GUILD_SUBMASTER,			///< 부길드장 위임
	TSTR_CMDESC_GUILD_DISUBMASTER,		///< 부길드장 해임
	TSTR_CMDESC_GUILD_PEER_CANCEL,		///< 주어진 작위를 취소
	TSTR_CMDESC_GUILD_BARON,				///< 남작에 임명
	TSTR_CMDESC_GUILD_VISCOUNT,			///< 자작에 임명
	TSTR_CMDESC_GUILD_COUNT,				///< 백작에 임명
	TSTR_CMDESC_GUILD_MARQUIS,			///< 후작에 임명
	TSTR_CMDESC_GUILD_DUKE,				///< 공작에 임명
	TSTR_CMDESC_GUILD_INVITE_TACTICS		///< 용병 초대
};
// ====================================================================

// ====================================================================
CTGuildCommander::CTGuildCommander()
	: m_dwInviterID(0), m_bDisorgCancel(FALSE), m_eCurDonMode(DON_NONE), m_eCurVolunUpdate(VUT_NONE)
{
}
// --------------------------------------------------------------------
CTGuildCommander* CTGuildCommander::GetInstance()
{
	static CTGuildCommander guild;
	return &guild;
}
// ====================================================================

// ====================================================================
CString CTGuildCommander::OnGuildCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CString strParam("");
	if( !vPARAM.empty() )
		strParam = vPARAM.front();

	GetInstance()->DoGuildCommand((CmdEnum)dwUSER, strParam);
	return CString("");
}
// --------------------------------------------------------------------
void CTGuildCommander::RegChatCmds()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrame = pGame->GetChatFrame();
	if( !pChatFrame )
		return;
	
	CString strCMD;
	for(DWORD i=0; i<CMD_COUNT; ++i)
	{
		strCMD = CTChart::LoadString( (TSTRING) CMD_STRING[i] );
		pChatFrame->AddChatCmdProc(strCMD, OnGuildCmdByChatMsg, i);
	}
}
// ====================================================================

// ====================================================================
CString CTGuildCommander::GetDutyStr(BYTE bDuty)
{
	CString ret;
	
	switch(bDuty)
	{
	case GUILD_DUTY_NONE:		ret = CTChart::LoadString( TSTR_GUILD_DUTY_NONE); break;
	case GUILD_DUTY_VICECHIEF:	ret = CTChart::LoadString( TSTR_GUILD_DUTY_VICECHIEF); break;
	case GUILD_DUTY_CHIEF:		ret = CTChart::LoadString( TSTR_GUILD_DUTY_CHIEF); break;
	}

	return ret;
}
// --------------------------------------------------------------------
CString CTGuildCommander::GetPeerageStr(BYTE bPeer)
{
	CString ret;
	
	switch(bPeer)
	{
	case GUILD_PEER_NONE:		ret = CTChart::LoadString( TSTR_GUILD_PEER_NONE); break;
	case GUILD_PEER_BARON:		ret = CTChart::LoadString( TSTR_GUILD_PEER_BARON); break;
	case GUILD_PEER_VISCOUNT:	ret = CTChart::LoadString( TSTR_GUILD_PEER_VISCOUNT); break;
	case GUILD_PEER_COUNT:		ret = CTChart::LoadString( TSTR_GUILD_PEER_COUNT); break;
	case GUILD_PEER_MARQUIS:	ret = CTChart::LoadString( TSTR_GUILD_PEER_MARQUIS); break;
	case GUILD_PEER_DUKE:		ret = CTChart::LoadString( TSTR_GUILD_PEER_DUKE); break;
	}

	return ret;
}
// ====================================================================

// ====================================================================
void CTGuildCommander::DoGuildCommand(CmdEnum eCmd, const CString& strParam)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	if( CMD_AUTHORITY[eCmd] == AUTHORITY_MASTER )
	{
		if( !pMainChar->m_dwGuildID || pMainChar->m_bGuildDuty < GUILD_DUTY_CHIEF )
			return;
	}
	else if( CMD_AUTHORITY[eCmd] == AUTHORITY_SUBMASTER )
	{
		if( !pMainChar->m_dwGuildID || pMainChar->m_bGuildDuty < GUILD_DUTY_VICECHIEF )
			return;
	}
	else if( CMD_AUTHORITY[eCmd] == AUTHORITY_MEMBER )
	{
		if( !pMainChar->m_dwGuildID )
			return;
	}
	else if( CMD_AUTHORITY[eCmd] == AUTHORITY_NOT_MEMBER )
	{
		if( pMainChar->m_dwGuildID )
			return;
	}

	switch(eCmd)
	{
	case CMD_SHOW_GUILDAPPDLG:
		{
			//pGame->EnableUI(TFRAME_GUILDAPP);
			static_cast<CTGuildApplicationDlg*>( pGame->GetFrame( TFRAME_GUILDAPP ) )->RequestInfo();
		}
		break;
	case CMD_INVITE:
		{
			if( !strParam.IsEmpty() )
				RequestGuildInvite(strParam);
		}
		break;
	case CMD_LEAVE: 
		{
			RequestGuildLeave();
		}
		break;
	case CMD_KICK: 
		{
			if( !strParam.IsEmpty() )
				RequestGuildKickOut(strParam,FALSE);
		}
		break;
	case CMD_DISORGONIZATION: 
		{
			RequestGuildDisorganization(FALSE);
		}
		break;
	case CMD_DISORGONIZATION_CANCEL: 
		{
			RequestGuildDisorganization(TRUE);
		}
		break;
	case CMD_APPOINTMENT_MASTER: 
		{
			if( !strParam.IsEmpty() )
				RequestGuildDuty(strParam, GUILD_DUTY_CHIEF);
		}
		break;
	case CMD_APPOINTMENT_SUBMASTER: 
		{
			if( !strParam.IsEmpty() )
				RequestGuildDuty(strParam, GUILD_DUTY_VICECHIEF);
		}
		break;
	case CMD_DISMISS_SUBMASTER:
		{
			if( !strParam.IsEmpty() )
				RequestGuildDuty(strParam, GUILD_DUTY_NONE);
		}
		break;
	case CMD_PEER_CANCEL:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_NONE);
		}
		break;
	case CMD_PEER_BARON:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_BARON);
		}
		break;
	case CMD_PEER_VISCOUNT:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_VISCOUNT);
		}
		break;
	case CMD_PEER_COUNT:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_COUNT);
		}
		break;
	case CMD_PEER_MARQUIS:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_MARQUIS);
		}
		break;
	case CMD_PEER_DUKE:
		{
			if( !strParam.IsEmpty() )
				RequestGuildPeer(strParam, GUILD_PEER_DUKE);
		}
		break;

	case CMD_INVITE_TACTICS:
		{
			if( !strParam.IsEmpty() )
			{
				pGame->m_strTacticsInvitationTarget = strParam;
				pGame->OnGM_SHOW_INVITE_TACTICS();
			}
		}
		break;
	}
}
// ====================================================================
CTGuildCommander::GuildMember* CTGuildCommander::FindGuildMemberByID( DWORD dwCharID )
{
	CTGuildCommander::GuildMemberVec::iterator it, end;
	it = m_GuildMemberVec.begin();
	end = m_GuildMemberVec.end();

	for(; it != end ; ++it )
	{
		if( it->m_dwCharID == dwCharID )
			return &(*it);
	}

	return NULL;
}
// ====================================================================
void CTGuildCommander::RecvGuildEstablish(BYTE bResult, DWORD dwGuildID, const CString& strGuild)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CString msgMSG;
	if( bResult == GUILD_SUCCESS )
	{
		CTClientChar* pMainChar = pGame->GetMainChar();
		if( pMainChar )
		{
			msgMSG = CTChart::Format( TSTR_MBOX_GUILD_BUILD_SUCC, (LPCSTR)strGuild);

			pMainChar->m_dwGuildID = dwGuildID;
			pMainChar->m_strGUILD = strGuild;
			pMainChar->m_bGuildDuty = GUILD_DUTY_CHIEF;
			pMainChar->m_bGuildPeer = GUILD_PEER_NONE;

			pGame->EnableGuildUI(TRUE);

			CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
			if( pComm->IsVisible() && 
				pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) ||
				pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER))
			{
				pComm->RequestInfo();
			}
		}
	}
	else if( bResult == GUILD_ALREADYGUILDNAME )
		msgMSG = CTChart::LoadString( TSTR_MBOX_GUILD_ALREADYGUILDNAME);
	else if( bResult == GUILD_HAVEGUILD )
		msgMSG = CTChart::LoadString( TSTR_MBOX_GUILD_ALREADYMEMBER);
	else if( bResult == GUILD_LEAVE_SELF )
		msgMSG = CTChart::LoadString( TSTR_MBOX_GUILD_LEAVE_SELF_ERR);
	else if( bResult == GUILD_LEAVE_DISORGANIZATION )
		msgMSG = CTChart::LoadString( TSTR_MBOX_GUILD_LEAVE_DISO_ERR);
	else
		msgMSG = CTChart::LoadString( TSTR_MBOX_GUILD_BUILD_FAIL);
	
	if( !msgMSG.IsEmpty() )
	{
		pWnd->MessageBoxOK(
			msgMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
}
// ====================================================================
void CTGuildCommander::RecvGuildDisorganization(BYTE bResult)
{
	if( bResult == GUILD_DISORGANIZATION_ERR )
		return;

	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	if( !pWnd )
		return;

	if( m_bDisorgCancel )
	{
		pWnd->MessageBoxOK(
			TSTR_MBOX_GUILD_DISORGCANCLE_SUCC,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
	else
	{
		pWnd->MessageBoxOK(
			TSTR_MBOX_GUILD_DISORG_SUCC,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}	
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDisorganization(BOOL bCancel)
{
	m_bDisorgCancel = bCancel;

	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame  )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDDISORGANIZATION_REQ(bCancel);
}
// ====================================================================
void CTGuildCommander::RecvGuildInvite(BYTE bResult, DWORD dwInviterID, const CString& strInviter, const CString& strGuild)
{	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	if( !pWnd )
		return;

	if( bResult == GUILD_SUCCESS )
	{
		m_dwInviterID = dwInviterID;

		if( CTClientGame::GetInstance()->IsBusy() )
		{
			RequestGuildInviteAnswer(ASK_BUSY);
		}
		else
		{
			CString msg;
			msg = CTChart::Format( TSTR_MBOX_GUILD_INVITE, (LPCSTR)strInviter, (LPCSTR)strGuild);

			pWnd->MessageBoxYesNo(
				msg,
				TSTR_YES,
				TSTR_NO,
				GM_GUILDINVITE_ACK_YES,
				GM_GUILDINVITE_ACK_NO,
				TCOMMAND( GM_GUILDINVITE_ACK_BUSY, dwInviterID ),
				FALSE);
		}
	}
	else
	{
		CString strMSG;
		switch( bResult )
		{
		case GUILD_HAVEGUILD:				strMSG = CTChart::LoadString( TSTR_MBOX_GUILD_ALREADYMEMBER); break;
		case GUILD_LEAVE_SELF:				strMSG = CTChart::LoadString( TSTR_MBOX_GUILD_LEAVE_SELF_ERR); break;
		case GUILD_LEAVE_DISORGANIZATION:	strMSG = CTChart::LoadString( TSTR_MBOX_GUILD_LEAVE_DISO_ERR); break;
		}

		if( !strMSG.IsEmpty() )
		{
			pWnd->MessageBoxOK(
				strMSG,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				FALSE);
		}
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildInviteAnswer(BYTE bReply)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDINVITEANSWER_REQ(bReply, m_dwInviterID);
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildInvite(const CString& strTarget)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDINVITE_REQ(strTarget);
}
// ====================================================================
void CTGuildCommander::RecvGuildJoin(BYTE bResult, DWORD dwGuildID, const CString& strGuild, 
									 DWORD dwNewMemberID, const CString& strNewMember, BYTE bMax)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	BOOL bInviter = (pMainChar->m_strNAME != strNewMember);

	CString msg;
	if( bResult == GUILD_JOIN_SUCCESS )
	{
		CTClientChar* pNewMember;
		if( bInviter )
		{
			msg = CTChart::Format( TSTR_MBOX_GUILD_NEWMEMBER, (LPCSTR)strNewMember);
			pNewMember = pGame->FindPC(dwNewMemberID);
		}
		else
		{
			pGame->EnableGuildUI(TRUE);

			msg = CTChart::Format( TSTR_MBOX_GUILD_JOIN_SUCC, (LPCSTR)strGuild);
			pNewMember = pMainChar;
		}

		if( pNewMember )
		{
			pNewMember->m_dwGuildID = dwGuildID;
			pNewMember->m_strGUILD = strGuild;
			pNewMember->m_bGuildDuty = GUILD_DUTY_NONE;
			pNewMember->m_bGuildPeer = GUILD_PEER_NONE;
		}

		CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
		if( pComm->IsVisible() && 
			pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) ||
			pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER))
		{
			pComm->RequestInfo();
		}
	}
	else 
	{
		switch(bResult)
		{
		case GUILD_JOIN_DENY:
			if(bInviter)
				msg = CTChart::Format( TSTR_MBOX_GUILD_JOIN_DENY, (LPCSTR)strNewMember);
			break;

		case GUILD_JOIN_BUSY:
			if(bInviter)
				msg = CTChart::Format( TSTR_ERR_TARGET_BUSY, (LPCSTR)strNewMember);
			break;
		
		case GUILD_MEMBER_FULL:
			msg = CTChart::Format( TSTR_MBOX_GUILD_MEMBER_FULL, bMax);
			break;

		case GUILD_FAIL:
			msg = CTChart::LoadString( TSTR_GUILD_FAILED_JOIN );
			break;
		}
	}

	if( !msg.IsEmpty() )
	{
		pWnd->MessageBoxOK(
			msg,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
}
// ====================================================================
void CTGuildCommander::RecvGuildLeave(BYTE bResult, const CString& strTarget, BYTE bReason)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CString strMSG;
	if( bResult == GUILD_SUCCESS )
	{
		if( bReason == GUILD_LEAVE_SELF ||
			bReason == GUILD_LEAVE_KICK ||
			bReason == GUILD_LEAVE_DISORGANIZATION )
		{
			CTClientChar* pLeaveMan;
			if( bReason == GUILD_LEAVE_SELF )
			{
				pLeaveMan = pMainChar;
				strMSG = CTChart::Format( TSTR_MBOX_GUILD_LEAVE_SELF);
			}
			else if( bReason == GUILD_LEAVE_DISORGANIZATION )
			{
				pLeaveMan = pMainChar;
				strMSG = CTChart::Format( TSTR_MBOX_GUILD_LEAVE_DISORG);
			}
			else
			{
				if( pMainChar->m_strNAME == strTarget )
				{
					pLeaveMan = pMainChar;
					strMSG = CTChart::Format( TSTR_MBOX_GUILD_LEAVE_KICK);
				}
				else
				{
					pLeaveMan = NULL;
					strMSG = CTChart::Format( TSTR_MBOX_GUILD_KICK_SUCC, (LPCSTR)strTarget);
				}
			}

			if( pMainChar->m_strNAME == strTarget )
			{
				m_GuildDetInfo = GuildDetInfo();
				m_GuildMemberVec.clear();

				pGame->EnableGuildUI(FALSE);
			}
			else
			{
				CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
				if( pComm->IsVisible() && 
					pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) ||
					pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
				{
					pComm->RequestInfo();
				}
			}

			if( pLeaveMan )
			{
				pLeaveMan->m_dwGuildID = 0;
				pLeaveMan->m_strGUILD = "";
			}
		}
	}
	else if( bResult == GUILD_NODUTY )
		strMSG = CTChart::LoadString( TSTR_GUILD_LEAVE_SELF_CHIEF_ERR);

	if( !strMSG.IsEmpty() )
	{
		pWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildKickOut(const CString& strTarget, BOOL bWarn)
{
	CString strKickTarget;
	if( strTarget.IsEmpty() )
	{
		if( bWarn == TRUE || m_strKickTarget.IsEmpty() )
			return;

		strKickTarget = m_strKickTarget;
		m_strKickTarget.Empty();
	}
	else
		strKickTarget = strTarget;

	if( bWarn )
	{
		m_strKickTarget = strKickTarget;

		CString strMSG;
		strMSG = CTChart::Format( TSTR_WARN_GUILDKICKOUT, strKickTarget);

		CTClientWnd::GetInstance()->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_GUILDKICKOUT,
			GM_CLOSE_MSGBOX );
	}
	else
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		if( !pGame )
			return;

		CTClientSession* pSession = pGame->GetSession();
		if( !pSession )
			return;

		pSession->SendCS_GUILDKICKOUT_REQ(strKickTarget);
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildTacticsKickOut(const CString& strTarget, DWORD dwTarget, BYTE bWarn)
{
	CString strKickTarget;
	DWORD dwKickTarget;

	if( strTarget.IsEmpty() )
	{
		if( bWarn == TRUE || m_strKickTarget.IsEmpty() )
			return;

		strKickTarget = m_strKickTarget;
		dwKickTarget = m_dwKickTarget;

		m_strKickTarget.Empty();
		m_dwKickTarget = 0;
	}
	else
	{
		strKickTarget = strTarget;
		dwKickTarget = dwTarget;
	}

	if( bWarn == 1 )
	{
		m_strKickTarget = strKickTarget;
		m_dwKickTarget = dwKickTarget;

		CString strMSG;
		strMSG = CTChart::Format( TSTR_WARN_GUILDKICKOUT, strKickTarget);

		CTClientWnd::GetInstance()->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_GUILDTACTICSKICKOUT,
			GM_CLOSE_MSGBOX,
			0 );
	}
	else if( bWarn == 2 )
	{
		m_strKickTarget = strKickTarget;
		m_dwKickTarget = dwKickTarget;

		CTClientWnd::GetInstance()->MessageBoxYesNo(
			TSTR_WARN_GUILDKICKOUT_SELF,
			TSTR_YES,
			TSTR_NO,
			GM_GUILDTACTICSKICKOUT,
			GM_CLOSE_MSGBOX,
			0 );
	}
	else
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		if( !pGame )
			return;

		CTClientSession* pSession = pGame->GetSession();
		if( !pSession )
			return;

		pSession->SendCS_GUILDTACTICSKICKOUT_REQ(dwKickTarget);
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildLeave()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar || !pMainChar->m_dwGuildID )
		return;

	pSession->SendCS_GUILDLEAVE_REQ();
}
// ====================================================================
void CTGuildCommander::RecvGuildDuty(BYTE bResult, const CString& strTarget, BYTE bDuty)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CString msgMSG;
	if( bResult == GUILD_SUCCESS )
	{
		BOOL bDutyChange = (strTarget == pMainChar->m_strNAME);
		CString strDuty = GetDutyStr(bDuty);

		if( bDutyChange )
		{
			pMainChar->m_bGuildDuty = bDuty;	
			msgMSG = CTChart::Format( TSTR_MBOX_GUILD_DUTY, (LPCSTR)strDuty);
		}
		else
			msgMSG = CTChart::Format( TSTR_MBOX_GUILD_DUTY_ACK, (LPCSTR)strTarget, (LPCSTR)strDuty);

		CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
		if( pComm->IsVisible() )
		{
			if( pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) )
				pComm->ResetInfo();
			else if( pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
				pComm->RequestInfo();
		}
	}

	if( !msgMSG.IsEmpty() )
	{
		pWnd->MessageBoxOK(
			msgMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDuty(const CString& strTarget, BYTE bDuty)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDDUTY_REQ(strTarget, bDuty);
}
// ====================================================================
void CTGuildCommander::RecvGuildPeer(BYTE bResult, const CString& strTarget, BYTE bPeer, BYTE bOldPeer)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CString msgMSG;
	if( bResult == GUILD_SUCCESS )
	{
		BOOL bDutyChange = (strTarget == pMainChar->m_strNAME);
		CString from = GetPeerageStr(bOldPeer);
		CString to = GetPeerageStr(bPeer);

		if( bDutyChange )
		{
			m_GuildDetInfo.m_bMyPeerage = bPeer;
			pMainChar->m_bGuildPeer = bPeer;
			msgMSG = CTChart::Format( TSTR_MBOX_GUILD_PEER, (LPCSTR)from, (LPCSTR)to);

			CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
			if( pComm->IsVisible() )
			{
				if( pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) )
					pComm->ResetInfo();
				else if( pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
					pComm->RequestInfo();
			}
		}
		else
			msgMSG = CTChart::Format( TSTR_MBOX_GUILD_PEER_ACK, (LPCSTR)strTarget, (LPCSTR)from, (LPCSTR)to);
	}
	else
	{
		CString to = GetPeerageStr(bPeer);
		msgMSG = CTChart::Format( TSTR_MBOX_GUILD_PEERMAX, to);
	}

	if( !msgMSG.IsEmpty() )
	{
		pWnd->MessageBoxOK(
			msgMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildPeer(const CString& strTarget, BYTE bPeer)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDPEER_REQ(strTarget, bPeer);
}
// ====================================================================
void CTGuildCommander::RecvGuildAttr(DWORD dwPlayerID, DWORD dwGuildID, const CString& strGuildName, 
									 BYTE bGuildPeer, DWORD dwTacticsID, const CString& strTacticsName,
									 CTClientGuildMark* pGuildMark)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	
	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CTClientChar* pChangeMan;
	if( pMainChar->m_dwID == dwPlayerID )
		pChangeMan = pMainChar;
	else
		pChangeMan = pGame->FindPC(dwPlayerID);

	if( pChangeMan )
	{
		pChangeMan->m_dwGuildID		= dwGuildID;
		pChangeMan->m_strGUILD		= strGuildName;
		pChangeMan->m_bGuildPeer	= bGuildPeer;
		pChangeMan->m_dwTacticsID	= dwTacticsID;
		pChangeMan->m_strTACTICS	= strTacticsName;
		
		if( pChangeMan->m_pGuildMark )
		{
			if( pGuildMark )
				*(pChangeMan->m_pGuildMark) = *pGuildMark;
			else
			{
				delete pChangeMan->m_pGuildMark;
				pChangeMan->m_pGuildMark = NULL;
			}
		}
		else if( pGuildMark )
			pChangeMan->m_pGuildMark = new CTClientGuildMark(*pGuildMark);

		if( pMainChar == pChangeMan )
		{
			if( pGuildMark )
			{
				m_GuildDetInfo.m_bMark = pGuildMark->GetFront();
				m_GuildDetInfo.m_bMarkBack = pGuildMark->GetBack();
				m_GuildDetInfo.m_bMarkColor = pGuildMark->GetFrontColorID();
				m_GuildDetInfo.m_bMarkBackColor1 = pGuildMark->GetBackColorID1();
				m_GuildDetInfo.m_bMarkBackColor2 = pGuildMark->GetBackColorID2();
			}
			else
			{
				m_GuildDetInfo.m_bMark = 0;
				m_GuildDetInfo.m_bMarkBack = 0;
				m_GuildDetInfo.m_bMarkColor = 0;
				m_GuildDetInfo.m_bMarkBackColor1 = 0;
				m_GuildDetInfo.m_bMarkBackColor2 = 0;
			}

			CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
			if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) )
				pComm->ResetInfo();
		}

		pChangeMan->ApplyMantleTexture();
	}
}
// ====================================================================
void CTGuildCommander::RecvGuildDetInfo(const GuildDetInfo& info)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	m_GuildDetInfo = info;
	m_GuildDetInfo.m_bMyPeerage = pMainChar->m_bGuildPeer;

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
	if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDNORMAL) )
		pComm->ResetInfo();
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDetInfo()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	if( pMainChar->m_dwGuildID ||
		pMainChar->m_dwTacticsID )
		pSession->SendCS_GUILDINFO_REQ();
}
// ====================================================================
void CTGuildCommander::RecvGuildMemberList(GuildMemberVec* pMembers)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	m_GuildMemberVec.swap(*pMembers);

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
	if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
		pComm->ResetInfo();
}
// ====================================================================
void CTGuildCommander::RecvGuildTacticsList(GuildTacticsVec* pMembers)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	
	m_GuildTacticsVec.swap(*pMembers);

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
	if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
		pComm->ResetInfo();
}
// ====================================================================
void CTGuildCommander::RecvGuildDonation(BYTE bResult)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CString strMSG;
	TUISND_TYPE eUITYPE = TUISND_TYPE_NONE;

	if( bResult == GUILD_SUCCESS )
	{
		strMSG = CTChart::LoadString( TSTR_DONATION_SUCCESS);
		eUITYPE = TUISND_TYPE_INFO;

		CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
		if( pComm->IsVisible() )
			pComm->RequestInfo();
	}
	else
	{
		eUITYPE = TUISND_TYPE_ERROR;

		switch(bResult)
		{
		case GUILD_CONTRIBUTION_NOTENOUGH:
			if( m_eCurDonMode == DON_EXP )
				strMSG = CTChart::LoadString( TSTR_DONERR_NOTENOUGH_EXP);
			else if( m_eCurDonMode == DON_MONEY )
				strMSG = CTChart::LoadString( TSTR_DONERR_NOTENOUGH_MONEY);
			break;

		case GUILD_CONTRIBUTION_MAXGUILDLEVEL:
			strMSG = CTChart::LoadString( TSTR_DONERR_MAXGUILDLEVEL);
			break;

		default:
			strMSG = CTChart::LoadString( TSTR_DONERR_UNKNOWN);
			break;
		}
	}

	if( !strMSG.IsEmpty() )
		pGame->ShowInfoChat(strMSG, eUITYPE);

	m_eCurDonMode = DON_NONE;
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDonationExp(DWORD dwEXP)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDCONTRIBUTION_REQ(dwEXP,0,0,0,0);
	m_eCurDonMode = DON_EXP;
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDonationMoney(DWORD dwRune,DWORD dwLuna,DWORD dwCron)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDCONTRIBUTION_REQ(0,dwRune,dwLuna,dwCron,0);
	m_eCurDonMode = DON_MONEY;
}

void CTGuildCommander::RequestGuildDonationPVP(DWORD dwPVP)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDCONTRIBUTION_REQ(0,0,0,0,dwPVP);
	m_eCurDonMode = DON_PVP;
}
// ====================================================================
void CTGuildCommander::RecvGuildNotifyList(GuildNotifyVec* pNotVec)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	
	m_GuildNotifyVec.clear();

	int nCnt = (int)pNotVec->size();
	for(int i=nCnt-1; i>=0; --i)
		m_GuildNotifyVec.push_back((*pNotVec)[i]);

	CTGuildNotifyDlg* pDlg = static_cast<CTGuildNotifyDlg*>( pGame->GetFrame(TFRAME_GUILDNOTIFY) );
	if( pDlg->IsVisible() )
		pDlg->UpdateInfo();
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildNotifyList()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDARTICLELIST_REQ();
}
// ====================================================================
void CTGuildCommander::RequestGuildNewNotify(const CString& strTitle, const CString& strArticle)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDARTICLEADD_REQ(strTitle,strArticle);
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildDelNotify(DWORD dwID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDARTICLEDEL_REQ(dwID);
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestGuildModNotify(DWORD dwID, const CString& strTitle, const CString& strArticle)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDARTICLEUPDATE_REQ(dwID,strTitle,strArticle);
}
// --------------------------------------------------------------------
void CTGuildCommander::RecvDeleteVolunteer(BYTE bResult)
{
	if( bResult == GUILD_SUCCESS )
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		if( !pGame )
			return;

		CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
		if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDVOLUNTEER) )
			pComm->RequestInfo();
	}
}
// --------------------------------------------------------------------
void CTGuildCommander::RequestDeleteVolunteer(DWORD dwCharID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	/*remove pSession->SendCS_GUILDVOLUNTEERDEL_REQ(dwCharID);*/
}
// ======================================================================
void CTGuildCommander::RequestGuildMarkReg(CTClientGuildMark* pMark)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDFAME_REQ(
		TGMARK(pMark->GetFront(), pMark->GetBack()),
		TGMCOLOR(pMark->GetFrontColorID(), pMark->GetBackColorID1(), pMark->GetBackColorID2()) );

	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	pMainChar->ApplyMantleTexture();

/*#ifdef ADD_TEXTOOL
	GuildDetInfo& info = CTGuildCommander::GetInstance()->m_GuildDetInfo;
	info.m_bShowMark = TRUE;
	info.m_bMark = pMark->GetFront();
	info.m_bMarkBack = pMark->GetBack();
	info.m_bMarkColor = pMark->GetFrontColorID();
	info.m_bMarkBackColor1 = pMark->GetBackColorID1();
	info.m_bMarkBackColor2 = pMark->GetBackColorID2();
	CTGuildCommander::GetInstance()->RecvGuildDetInfo( info );

	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();

	CTClientGuildMark* pMARK = pMainChar->m_pGuildMark;
	if( pMARK )
	{
		pMARK->SetBack( info.m_bMarkBack );
		pMARK->SetFront( info.m_bMark );
		pMARK->SetFrontColorID( info.m_bMarkColor );
		pMARK->SetBackColorID1( info.m_bMarkBackColor1 );
		pMARK->SetBackColorID2( info.m_bMarkBackColor2 );
	}
	pMainChar->ApplyMantleTexture();
#endif*/
}
// ======================================================================
void CTGuildCommander::RecvTerritoryList(TerritoryVec* pTerr)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	m_TerritoryVec.swap(*pTerr);

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( pGame->GetFrame(TFRAME_COMMUNITY) );
	if( pComm->IsVisible() && pComm->IsSelect(TCOMMUNITY_FRAME_GUILDTERRITORY) )
		pComm->ResetInfo();
}
// ----------------------------------------------------------------------
void CTGuildCommander::RequestTerritoryList()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_GUILDLOCALLIST_REQ();
}
// ----------------------------------------------------------------------
void CTGuildCommander::RequestTacticsSeekInvite(DWORD dwID, DWORD dwRune, DWORD dwLuna, DWORD dwCron)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	/*remove pSession->SendCS_GUILDTACTICSAPPINVITE_REQ(dwID, dwRune, dwLuna, dwCron);*/
}
// ======================================================================
void CTGuildCommander::RecvTacticsLeave(BYTE bResult)
{
}
// ----------------------------------------------------------------------
void CTGuildCommander::RequestTacticsLeave()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	/*remove pSession->SendCS_GUILDTACTICSLEAVE_REQ();*/
}
// ======================================================================

// ======================================================================
bool _compare_guildmember(const CTGuildCommander::GuildMember& l, const CTGuildCommander::GuildMember& r)
{
	if( l.m_bIsConnect != r.m_bIsConnect )
		return l.m_bIsConnect > r.m_bIsConnect;

	if( l.m_bLevel != r.m_bLevel )
		return l.m_bLevel > r.m_bLevel;

	if( l.m_bWarCountry != r.m_bWarCountry )
		return l.m_bWarCountry > r.m_bWarCountry;

	if( l.m_bIsConnect != r.m_bIsConnect )
		return (l.m_bIsConnect == TRUE);

	return l.m_bDuty > r.m_bDuty;
}

void CTGuildCommander::SortGuildMember()
{
	std::sort( m_GuildMemberVec.begin(), m_GuildMemberVec.end(), _compare_guildmember );
}
// ----------------------------------------------------------------------
bool _compare_tactics(const CTGuildCommander::GuildTactics& l, const CTGuildCommander::GuildTactics& r)
{
	if( l.m_bLevel != r.m_bLevel )
		return l.m_bLevel > r.m_bLevel;

	//if( l.m_bClass != r.m_bClass )
		return l.m_bClass > r.m_bClass;

	//return l.m_bJoinKind > r.m_bJoinKind;
}
void CTGuildCommander::SortTactics()
{
	std::sort( m_GuildTacticsVec.begin(), m_GuildTacticsVec.end(), _compare_tactics );
}
// ======================================================================
bool _compare_latest_pvp(const CTGuildCommander::GuildLatestPvP& l, const CTGuildCommander::GuildLatestPvP& r)
{
	return l.m_dwResult > r.m_dwResult;
}
void CTGuildCommander::SortLatestPVP()
{
	std::sort( m_GuildLatestPvPVec.begin(), m_GuildLatestPvPVec.end(), _compare_latest_pvp );
}
// ======================================================================
bool _compare_week(const CTGuildCommander::GuildWeekPvP& l, const CTGuildCommander::GuildWeekPvP& r)
{
	return l.m_dwResult > r.m_dwResult;
}
void CTGuildCommander::SortWeekPVP()
{
	std::sort( m_GuildWeekPvPVec.begin(), m_GuildWeekPvPVec.end(), _compare_week );
}
// ======================================================================
CTGuildCommander::GuildDetInfo::GuildDetInfo()
  :	m_dwGuildID(0),
	m_strGName(""),
	m_ldwGDate(0),
	m_strGMaster(""),	
	m_strGSubMaster1(""),
	m_strGSubMaster2(""),
	m_nGMbCnt(0),
	m_nGMbMax(0),	
	m_nGLev(0),	
	m_dwGExpCur(0),
	m_dwGExpMax(0),
	m_strGKind(""),
	m_nGRune(0),
	m_nGLuna(0),
	m_nGCron(0),
	m_bMyPeerage(0),
	m_bMyService(0),
	m_bMark(0),		
	m_bMarkBack(0),
	m_bMarkColor(0),		
	m_bMarkBackColor1(0),
	m_bMarkBackColor2(0),
	m_dwPvPTotalPoint(0),
	m_dwPvPUseablePoint(0),
	m_dwMonthPoint(0),
	m_dwRankTotal(0),
	m_dwRankMonth(0)
{
}
// ======================================================================























