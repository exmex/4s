#include "Stdafx.h"
#include "TMessengerCommander.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TMessengerBase.h"
#include "TMsgGroupList.h"
#include "Resource.h"
#include "TMessengerChat.h"
#include "TMsgChatMemberList.h"

// TEMP
INT tempMCNT = 0;

// ====================================================================
const DWORD CTMessengerCommander::CMD_STRING[] = 
{
	TSTR_CMD_MSG_ADD_FRIEND,			///< 친구 등록
	TSTR_CMD_MSG_DEL_FRIEND,			///< 친구 제거
	TSTR_CMD_MSG_MAKE_GROUP,			///< 친구 그룹 생성
	TSTR_CMD_MSG_DEL_GROUP,			///< 친구 그룹 제거
	TSTR_CMD_MSG_CHANGE_GROUP,		///< 친구 그룹을 변경한다.
	TSTR_CMD_MSG_NAME_GROUP,			///< 친구 그룹 이름을 변경한다.
	TSTR_CMD_MSG_ADD_BLOCK,			///< 유저를 차단목록에 등록한다.
	TSTR_CMD_MSG_DEL_BLOCK,			///< 유저를 차단목록에서 제거한다.
};
// ====================================================================

// ====================================================================
CTMessengerCommander::CTMessengerCommander()
{
}
// --------------------------------------------------------------------
CTMessengerCommander* CTMessengerCommander::GetInstance()
{
	static CTMessengerCommander mcmd;
	return &mcmd;
}
// ====================================================================
void SetSoulmate( CTMsgGroupList::CMember* m,
	DWORD dwSoulID,
	CString strSoulName,
	DWORD dwSoulRegion,
	BYTE bSoulLevel,
	BYTE bSoulClass,
	BYTE bSoulConnected )
{
	m->m_dwID = dwSoulID;
	m->m_vTxt[CTMsgGroupList::MTXT_NAME]->m_strText = strSoulName;

	if( bSoulLevel != 255 )
		m->m_vTxt[CTMsgGroupList::MTXT_LEVEL]->m_strText.Format("%u", bSoulLevel );
	else
		m->m_vTxt[CTMsgGroupList::MTXT_LEVEL]->m_strText.Empty();

	if( bSoulClass < TCLASS_COUNT )
		m->m_vTxt[CTMsgGroupList::MTXT_JOB]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ bSoulClass ] );
	else
		m->m_vTxt[CTMsgGroupList::MTXT_JOB]->m_strText.Empty();

	if( bSoulConnected == 255 )
		m->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText.Empty();
	else if( bSoulConnected )
		m->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_CONNECT);
	else
		m->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_DISCONNECT);

	CString strRegion;
	if( dwSoulRegion )
	{
		LPTREGIONINFO pInfo = CTChart::FindTREGIONINFO( dwSoulRegion );
		if( pInfo )
			strRegion = pInfo->m_strNAME;
	}

	m->m_vTxt[CTMsgGroupList::MTXT_POS]->m_strText = strRegion;
}
// ====================================================================
void SetFriend(CTMsgGroupList::CMember* m, CTMessengerCommander::FriendMember* f)
{
	m->m_dwID = f->dwCharID;
			
	m->m_vTxt[CTMsgGroupList::MTXT_NAME]->m_strText = f->strName;
	m->m_vTxt[CTMsgGroupList::MTXT_LEVEL]->m_strText.Format("%u", f->bLevel);

	if( f->bClass < TCLASS_COUNT )
		m->m_vTxt[CTMsgGroupList::MTXT_JOB]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[f->bClass] );
	else
		m->m_vTxt[CTMsgGroupList::MTXT_JOB]->m_strText.Empty();
	
	m->m_bOnLine = f->bConnected;

	if( f->bConnected )
		m->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_CONNECT);
	else
		m->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_DISCONNECT);

	CString strRegion;
	if( f->dwRegion )
	{
		LPTREGIONINFO pInfo = CTChart::FindTREGIONINFO(f->dwRegion);
		if( pInfo )
			strRegion = pInfo->m_strNAME;
	}

	m->m_vTxt[CTMsgGroupList::MTXT_POS]->m_strText = strRegion;
}
// --------------------------------------------------------------------
void CTMessengerCommander::ChatSysMSG( const CString& strMSG)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrm = pGame->GetChatFrame();
	CTClientChar* pMainChar = pGame->GetMainChar();

	CString strType;
	strType = CTChart::LoadString( TSTR_INFO_TITLE);

	pChatFrm->ChatSysMSG( strType, pMainChar->GetName(), strMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
}
// ====================================================================

// ====================================================================
CString CTMessengerCommander::OnMessengerCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CString strParam1(""), strParam2("");
	if( !vPARAM.empty() )
	{
		strParam1 = vPARAM[0];

		if( vPARAM.size() > 1 )
			strParam2 = vPARAM[1];
	}

	GetInstance()->DoMessengerCommand((CmdEnum)dwUSER, strParam1,strParam2);
	return CString("");
}
// --------------------------------------------------------------------
void CTMessengerCommander::RegChatCmds()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrame = pGame->GetChatFrame();
	if( !pChatFrame )
		return;
	
	CString strCMD;
	for(DWORD i=0; i<CMD_MSG_COUNT; ++i)
	{
		strCMD = CTChart::LoadString( (TSTRING) CMD_STRING[i] );
		pChatFrame->AddChatCmdProc(strCMD, OnMessengerCmdByChatMsg, i);
	}
}
// ====================================================================


// ====================================================================
void CTMessengerCommander::DoMessengerCommand(CmdEnum eCmd, const CString& strParam1, const CString& strParam2)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();

	switch(eCmd)
	{
	case CMD_MSG_ADD_FRIEND: 
		{
			if( !strParam1.IsEmpty() )
				DoAddFriend(strParam1);
		}
		break;
	case CMD_MSG_DEL_FRIEND:
		{
			if( !strParam1.IsEmpty() )
			{
				CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
				CTMsgGroupList::CMember* pMember = pList->FindMemberByName(strParam1);
				if( pMember )
					DoEraseFriend( pMember->m_dwID );
			}
		}
		break;
	case CMD_MSG_MAKE_GROUP:
		{
			if( !strParam1.IsEmpty() )
				DoMakeFriendGroup(strParam1);
		}
		break;
	case CMD_MSG_DEL_GROUP:
		{
			if( !strParam1.IsEmpty() )
			{
				CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
				CTMsgGroupList::CGroup* pGroup = pList->FindGroupByName(strParam1);
				if( pGroup )
					DoEraseFriendGroup( (BYTE)pGroup->m_dwID );
			}
		}
		break;
	case CMD_MSG_CHANGE_GROUP:
		{
			if( !strParam1.IsEmpty() )
			{
				CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
				CTMsgGroupList::CMember* pMember = pList->FindMemberByName(strParam1);
				if( !pMember )
					return;

				BYTE bGrpID = 0;
				if( !strParam2.IsEmpty() )
				{
					CTMsgGroupList::CGroup* pGroup = pList->FindGroupByName(strParam2);
					if( !pGroup )
						return;

					bGrpID = (BYTE)pGroup->m_dwID;
				}

				DoChangeFriendGroup(pMember->m_dwID, bGrpID);
			}
		}
		break;
	case CMD_MSG_NAME_GROUP:
		{
			if( strParam1.IsEmpty() || strParam2.IsEmpty() )
				return;

			CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
			CTMsgGroupList::CGroup* pGroup = pList->FindGroupByName(strParam1);
			if( !pGroup )
				return;

			DoFriendGroupName((BYTE)pGroup->m_dwID, strParam2);
		}
		break;
	case CMD_MSG_ADD_BLOCK:
		{
			if( strParam1.IsEmpty() )
				return;

			DoAddBlock(strParam1);
		}
		break;

	case CMD_MSG_DEL_BLOCK:
		{
			if( strParam1.IsEmpty() )
				return;

			DoDeleteBlock(strParam1);
		}
		break;
	}
}
// ====================================================================
struct TSortFriend
{
	bool operator () ( CTMessengerCommander::FriendMember* _Left,
		CTMessengerCommander::FriendMember* _Right)
	{
		return _Left->bConnected > _Right->bConnected;
	};
};
// ====================================================================
void CTMessengerCommander::OnFriendlist(
	BYTE bGroupCount,
	FriendGroup* pGroups,
	BYTE bFriendCount,
	FriendMember* pFriends,
	CTMessengerCommander::TMAPLINKGROUPMEMBER& vGROUPS )
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	std::map< DWORD, BOOL > vTWINKLE;
	pList->CollectTwinkle( vTWINKLE );
	pList->Clear();
	
	CTMsgGroupList::CGroup* pEtcGroup = pList->GetEtcGroup();
	{
		TMAPLINKGROUPMEMBER::iterator it;
		it = vGROUPS.find( 0 );
		if( it != vGROUPS.end() )
		{
			CTMessengerCommander::VTFRIENDMEMBER& vMEMBERS = it->second;
			std::sort( vMEMBERS.begin(), vMEMBERS.end(), TSortFriend() );

			for( INT i=0 ; i < vMEMBERS.size() ; ++i )
			{
				CTMsgGroupList::CMember* m = pList->AddMember( pEtcGroup );
				SetFriend(m, vMEMBERS[ i ] );

				std::map< DWORD, BOOL >::iterator itT = vTWINKLE.find( vMEMBERS[ i ]->dwCharID );
				if( itT != vTWINKLE.end() )
				{
					m->m_bTwinkle = itT->second;
					if( m->m_pParentGrp )
						m->m_pParentGrp->Spread( TRUE );
				}
			}
		}
	}

	/*for(BYTE i=0; i<bFriendCount; ++i)
	{
		FriendMember* f = pFriends+i;
		if( !f->bGroup )
		{
			CTMsgGroupList::CMember* m = pList->AddMember( pEtcGroup );
			SetFriend(m,f);
		}
	}*/

	for(BYTE i=0; i<bGroupCount; ++i)
	{
		CTMsgGroupList::CGroup* g = pList->AddGroup();
		g->m_dwID = pGroups[i].bGroupID;
		g->m_pGroupName->m_strText = pGroups[i].strName;

		TMAPLINKGROUPMEMBER::iterator it;
		it = vGROUPS.find( pGroups[i].bGroupID );
		if( it != vGROUPS.end() )
		{
			CTMessengerCommander::VTFRIENDMEMBER& vMEMBERS = it->second;
			std::sort( vMEMBERS.begin(), vMEMBERS.end(), TSortFriend() );

			for( INT i=0 ; i < vMEMBERS.size() ; ++i )
			{
				CTMsgGroupList::CMember* m = pList->AddMember(g);
				SetFriend(m, vMEMBERS[ i ] );

				std::map< DWORD, BOOL >::iterator itT = vTWINKLE.find( vMEMBERS[ i ]->dwCharID );
				if( itT != vTWINKLE.end() )
				{
					m->m_bTwinkle = itT->second;
					if( m->m_pParentGrp )
						m->m_pParentGrp->Spread( TRUE );
				}
			}
		}
	}
}
// ====================================================================
void CTMessengerCommander::DoAddFriend(const CString& strFriend)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	if( !pList->FindMemberByName(strFriend) )
	{
		// -------------------------------------------
		pSession->SendCS_FRIENDASK_REQ(strFriend);
		// -------------------------------------------
		//FriendMember fm;
		//fm.bClass = 0;
		//fm.bConnected = 1;
		//fm.bGroup = 0;
		//fm.bLevel = 1;
		//fm.dwCharID = ++tempMCNT;
		//fm.dwRegion = 1;
		//fm.strName = strFriend;
		//OnAddFriend(FRIEND_SUCCESS, &fm);
		// -------------------------------------------
	}
	else
	{
		CString strMsg;
		strMsg = CTChart::Format( TSTR_CMSG_ALEADYFRIEND, (LPCSTR)strFriend);

		ChatSysMSG(strMsg);
	}
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnFriendInvite(const CString& strInviter)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	m_strReqTarget = strInviter;

	if( pGame->IsBusy() )
		DoFriendInviteAnswer(ASK_BUSY);
	else
	{
		CString msg;
		msg = CTChart::Format( TSTR_MBOX_FRIEND_INVITE, (LPCSTR)strInviter);
		
		pWnd->MessageBoxYesNo(
			msg,
			TSTR_YES,
			TSTR_NO,
			GM_FRIENDINVITE_ACK_YES,
			GM_FRIENDINVITE_ACK_NO,
			0,
			FALSE);
	}
}
// --------------------------------------------------------------------
void CTMessengerCommander::DoFriendInviteAnswer(BYTE bReply)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_FRIENDREPLY_REQ(m_strReqTarget, bReply);
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnAddFriend(BYTE bResult, FriendMember* pFriend)
{
	CTClientWnd* pWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pWnd || !pGame )
		return;

	CString msg;
	if( bResult == FRIEND_SUCCESS )
	{
		msg = CTChart::Format( TSTR_MBOX_FRIENDREPLY_YES, pFriend->strName);

		CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
		CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

		SetFriend( pList->AddMember( pList->GetEtcGroup() ), pFriend);
	}
	else if( bResult == FRIEND_REFUSE )
		msg = CTChart::Format( TSTR_MBOX_FRIENDREPLY_NO, pFriend->strName);
	else if( bResult == FRINED_BUSY )
		msg = CTChart::Format( TSTR_ERR_TARGET_BUSY, pFriend->strName);

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
void CTMessengerCommander::DoEraseFriend(DWORD dwCharID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	// -------------------------------------------
	pSession->SendCS_FRIENDERASE_REQ(dwCharID);
	// -------------------------------------------
	//OnEraseFriend(dwCharID);
	// -------------------------------------------
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnEraseFriend(DWORD dwCharID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	CTMsgGroupList::CMember* pMember = pList->FindMemberByID(dwCharID);
	if( pMember )
	{
		CString strMsg;
		strMsg = CTChart::Format( TSTR_CMSG_ERASEFRIEND, (LPCSTR)(pMember->GetNameComp()->m_strText));
		ChatSysMSG(strMsg);

		pList->RemoveItem(pMember->m_nIndex, pMember->m_pParentGrp);
	}
}
// ====================================================================
void CTMessengerCommander::OnConnectFriend(BYTE bResult, const CString& strName, DWORD dwRegion)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	CTMsgGroupList::CMember* pFMem = pList->FindMemberByName(strName);
	if( pFMem )
	{
		CString strChatMsg;

		if( bResult == FRIEND_CONNECTION )
		{
			pFMem->m_bOnLine = TRUE;
			pFMem->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_CONNECT);

			LPTREGIONINFO pInfo = CTChart::FindTREGIONINFO(dwRegion);
			if( pInfo )
				pFMem->m_vTxt[CTMsgGroupList::MTXT_POS]->m_strText = pInfo->m_strNAME;

			strChatMsg = CTChart::Format( TSTR_CMSG_FRIEND_CONNECT, strName);
		}
		else
		{
			pFMem->m_bOnLine = FALSE;
			pFMem->m_vTxt[CTMsgGroupList::MTXT_STATE]->m_strText = CTChart::LoadString( TSTR_STATE_DISCONNECT);
			pFMem->m_vTxt[CTMsgGroupList::MTXT_POS]->m_strText.Empty();

			strChatMsg = CTChart::Format( TSTR_CMSG_FRIEND_DISCONNECT, strName);
		}

		ChatSysMSG( strChatMsg );
	}
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnChangeFriendRegion(const CString& strName, DWORD dwRegion)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	CTMsgGroupList::CMember* pFMem = pList->FindMemberByName(strName);
	if( pFMem )
	{
		LPTREGIONINFO pInfo = CTChart::FindTREGIONINFO(dwRegion);
		if( pInfo )
			pFMem->m_vTxt[CTMsgGroupList::MTXT_POS]->m_strText = pInfo->m_strNAME;
	}
}
// ====================================================================
void CTMessengerCommander::DoMakeFriendGroup(const CString& strName)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
	
	if( !pList->FindGroupByName(strName) )
	{
		UINT nGrpIdx = 1;
		UINT nCnt = pList->GetItemCount();
		for( UINT i=0; i<nCnt; ++i )
		{
			CTMsgGroupList::CItem* pItem = pList->GetItem(i); 
			if( pItem->GetType() == CTMsgGroupList::ITYPE_GROUP &&
				pItem->m_bSoulmate == FALSE &&
				pItem->m_bEtcGroup == FALSE )
			{
				DWORD dwCurID = pItem->m_dwID;
				if( dwCurID >= nGrpIdx )
					nGrpIdx = dwCurID+1;
			}
		}

		// -------------------------------------------
		pSession->SendCS_FRIENDGROUPMAKE_REQ(nGrpIdx,strName);
		// -------------------------------------------
		//OnMakeFriendGroup(FRIEND_SUCCESS,nGrpIdx,strName);
		// -------------------------------------------
	}
	else
	{
		CString strMsg;
		strMsg = CTChart::Format( TSTR_CMSG_ALEADYGROUP, (LPCSTR)strName);
		
		ChatSysMSG(strMsg);
	}
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnMakeFriendGroup(BYTE bResult, BYTE bGroupID, const CString& strName)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	CString msgChat;
	if( bResult == FRIEND_SUCCESS )
	{
		CTMsgGroupList::CGroup* pGrp = pList->AddGroup();
		pGrp->m_dwID = bGroupID;
		pGrp->m_pGroupName->m_strText = strName;

		msgChat = CTChart::Format( TSTR_CMSG_MAKEGROUP_SUCC, (LPCSTR)strName);
	}
	else if( bResult == FRIEND_MAX )
		msgChat = CTChart::LoadString( TSTR_CMSG_MAKEGROUP_MAX);

	if( !msgChat.IsEmpty() )
		ChatSysMSG( msgChat );
}
// ====================================================================
void CTMessengerCommander::DoEraseFriendGroup(BYTE bGroupID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	// -------------------------------------------
	pSession->SendCS_FRIENDGROUPDELETE_REQ(bGroupID);
	// -------------------------------------------
	//OnEraseFriendGroup(FRIEND_SUCCESS,bGroupID);
	// -------------------------------------------
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnEraseFriendGroup(BYTE bResult, BYTE bGroupID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CString msgChat;
	if( bResult == FRIEND_SUCCESS )
	{
		CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
		CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
		
		CTMsgGroupList::CGroup* pFindGrp = pList->FindGroupByID(bGroupID);
		if( pFindGrp )
		{
			msgChat = CTChart::Format( TSTR_CMSG_ERASEFRIENDGROUP, (LPCSTR)(pFindGrp->GetNameComp()->m_strText));
			pList->RemoveItem(pFindGrp->m_nIndex);
		}
	}

	if( !msgChat.IsEmpty() )
		ChatSysMSG( msgChat );
}
// ====================================================================
void CTMessengerCommander::DoChangeFriendGroup(DWORD dwCharID, BYTE bWantedGroup)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	// -------------------------------------------
	pSession->SendCS_FRIENDGROUPCHANGE_REQ(dwCharID,bWantedGroup);
	// -------------------------------------------
	//OnChangeFriendGroup(FRIEND_SUCCESS,bWantedGroup,dwCharID);
	// -------------------------------------------
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnChangeFriendGroup(BYTE bResult, BYTE bGroup, DWORD dwCharID)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	if( bResult == FRIEND_SUCCESS )
	{
		CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
		CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
		
		CTMsgGroupList::CGroup* pGrp = NULL;
		if( bGroup )
			pGrp = pList->FindGroupByID(bGroup);

		CTMsgGroupList::CMember* pMember = pList->FindMemberByID(dwCharID);
		if( !pMember )
			return;

		pList->ChangerGroup(pMember,pGrp);
	}
}
// ====================================================================
void CTMessengerCommander::DoFriendGroupName(BYTE bGroup, const CString& strName)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_FRIENDGROUPNAME_REQ(bGroup,strName);
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnFriendGroupName(BYTE bResult, BYTE bGroup, const CString& strName)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	if( bResult == FRIEND_SUCCESS )
	{
		CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
		CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

		CTMsgGroupList::CGroup* pGroup = pList->FindGroupByID(bGroup);
		if( pGroup )
			pGroup->m_pGroupName->m_strText = strName;
	}
}
// ====================================================================
void CTMessengerCommander::DoInviteChatGroup(DWORD dwTMS, BYTE bGroup)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

	CTMsgGroupList::CGroup* pGrp = pList->FindGroupByID(bGroup);
	if( !pGrp )
		return;
	
	VECTORDWORD vTARGET;

	CTMsgGroupList::CItemArray::iterator itr,end;
	itr = pGrp->m_ItemArray.begin();
	end = pGrp->m_ItemArray.end();

	for(; itr!=end; ++itr)
	{
		if( static_cast<CTMsgGroupList::CMember*>(*itr)->m_bOnLine )
			vTARGET.push_back( (*itr)->m_dwID );
	}

	if( !vTARGET.empty() )
		DoInviteChatTarget(dwTMS, vTARGET);
}
// --------------------------------------------------------------------
void CTMessengerCommander::DoInviteChatTarget(DWORD dwTMS, const VECTORDWORD& vTARGET)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_TMSINVITE_REQ(dwTMS, vTARGET);
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnInviteChatTarget(DWORD dwTMS, const CString& strInviter, LPMCTARGET_ARRAY pTargets)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMessengerChat* pChat = CTMessengerChat::GetChat(dwTMS);
	if( !pChat )
	{
		BOOL bShow = (strInviter == pGame->GetMainChar()->m_strNAME);
		
		pChat = CTMessengerChat::NewInstance(dwTMS);
		pChat->ShowComponent(bShow);

		if( !bShow )
			pGame->NotifyNewMsgChat(pChat);
	}
	else
		pGame->NotifyNewMsgChat(pChat);

	pChat->SetTargets(pTargets);
}
// ====================================================================
void CTMessengerCommander::DoChat(DWORD dwTMS, const CString& strMessage)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_TMSSEND_REQ(dwTMS,strMessage);
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnChat(DWORD dwTMS, const CString& strSender, const CString& strMessage)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTMessengerChat* pChat = CTMessengerChat::GetChat(dwTMS);
	if( !pChat )
		return;

	pGame->NotifyNewMsgChat(pChat);
	pChat->OnChatMsg(strSender, strMessage);
}
// ====================================================================
void CTMessengerCommander::DoChatOut(CTMessengerChat* pChat)
{
	if( !pChat->IsEnable() )
		return;
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_TMSOUT_REQ(pChat->GetTmsID());
	
	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();

	// 메신저
	/*CTMsgChatMemberList* pList = pMsgBase->GetMsgChatMemberList();
	INT nIdx = pList->FindByChat(pChat);
	if( nIdx != T_INVALID )
		pList->Remove(nIdx);*/

	LPMCTARGET_ARRAY pArray = pChat->GetTargets();
	if( !pArray->empty() )
	{
		CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
		CTMsgGroupList::CMember* pMember = pList->FindMemberByID( (*pArray)[ 0 ].dwCharID );

		if( pMember )
		{
			pMember->m_bTwinkle = FALSE;
			if( pMember->m_pParent )
				((CTMsgGroupList::CGroup*)(pMember->m_pParentGrp))->Spread(TRUE);
		}
	}

	CTMessengerChat::DeleteInstance(pChat);
}
// --------------------------------------------------------------------
void CTMessengerCommander::OnChatOut(DWORD dwTMS, const CString& strTarget)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	if( pGame->GetMainChar()->m_strNAME == strTarget )
		return;

	CTMessengerChat* pChat = CTMessengerChat::GetChat(dwTMS);
	if( !pChat )
		return;

	CTMessengerChat::OnOutChatTarget(pChat, strTarget);
}
// ====================================================================
void CTMessengerCommander::DoAddBlock(const CString& strTarget)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
	CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();
	CTMsgGroupList::CMember* pMember = pList->FindMemberByName(strTarget);
	if( pMember )
		DoEraseFriend( (BYTE) pMember->m_dwID );

	pSession->SendCS_PROTECTEDADD_REQ(strTarget);
}
// --------------------------------------------------------------------
void CTMessengerCommander::DoDeleteBlock(const CString& strTarget)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	pSession->SendCS_PROTECTEDERASE_REQ(strTarget);
}
// ====================================================================



















