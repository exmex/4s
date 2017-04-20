#include "Stdafx.h"
#include "TMessengerChat.h"
#include "TClientGame.h"
#include "TMessengerBase.h"
#include "TMsgGroupList.h"
#include "TClient.h"
#include "Resource.h"

// ===============================================================================
LP_FRAMEDESC					CTMessengerChat::ms_ChatFrameDesc = NULL;
VTMSGCHATARRAY		CTMessengerChat::ms_FreeChats;
TMAPMSGTOCHAT			CTMessengerChat::ms_TmsActChats;
BOOL							CTMessengerChat::ms_bShow = TRUE;
// ===============================================================================

// ===============================================================================
CTMessengerChat* CTMessengerChat::NewInstance(DWORD dwTmsID)
{
    CTMessengerChat* pChat;
	if( ms_FreeChats.empty() )
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		pChat = new CTMessengerChat(pGame, ms_ChatFrameDesc);
		pGame->AddKid(pChat);
	}
	else
	{
		pChat = ms_FreeChats.back();
		ms_FreeChats.pop_back();
	}

	pChat->m_dwTmsID = dwTmsID;
	ms_TmsActChats.insert( std::make_pair(dwTmsID,pChat) );

	pChat->EnableComponent(TRUE);
	pChat->ShowComponent(FALSE);

	return pChat;
}
// -------------------------------------------------------------------------------
BOOL CTMessengerChat::DeleteInstance(CTMessengerChat* pChat)
{
	if( !pChat->IsEnable() )
		return FALSE;
	
	TMAPMSGTOCHAT::iterator itr = ms_TmsActChats.find( pChat->m_dwTmsID );
	if( itr == ms_TmsActChats.end() )
		return FALSE;

	ms_TmsActChats.erase(itr);
	ms_FreeChats.push_back(pChat);

	pChat->ShowComponent(FALSE);
	pChat->EnableComponent(FALSE);

	return TRUE;
}
// -------------------------------------------------------------------------------
void CTMessengerChat::DeleteAllInstance()
{
	TMAPMSGTOCHAT::iterator itr = ms_TmsActChats.begin();
	TMAPMSGTOCHAT::iterator end = ms_TmsActChats.end();

	for(; itr!=end; ++itr)
	{
		CTMessengerChat* pChat = itr->second;
		ms_FreeChats.push_back(pChat);

		pChat->ShowComponent(FALSE);
		pChat->EnableComponent(FALSE);

		itr = ms_TmsActChats.erase(itr);
	}
	
	ms_bShow = TRUE;
}
// ===============================================================================
CTMessengerChat* CTMessengerChat::GetChat(DWORD dwTmsID)
{
	TMAPMSGTOCHAT::iterator itr = ms_TmsActChats.find(dwTmsID);
	if( itr == ms_TmsActChats.end() )
		return NULL;

	return itr->second;
}
// -------------------------------------------------------------------------------
BOOL CTMessengerChat::DoChat(CTMessengerChat* pChat, const CString& strMsg)
{
	if( !pChat->IsEnable() )
		return FALSE;

	CTMessengerCommander::GetInstance()
		->DoChat(pChat->m_dwTmsID, strMsg);
	
	return TRUE;
}
// -------------------------------------------------------------------------------
BOOL CTMessengerChat::OnOutChatTarget(CTMessengerChat* pChat, const CString& strOutTarget)
{
	if( pChat->GetTargetCount() > 1 )
	{
		if( !pChat->RemoveTarget(strOutTarget) )
			return FALSE;
	}

	return TRUE;
}
// ===============================================================================
void CTMessengerChat::ShowAll(BOOL bShow)
{
	ms_bShow = bShow;

	TMAPMSGTOCHAT::iterator itrtms = ms_TmsActChats.begin();
	TMAPMSGTOCHAT::iterator endtms = ms_TmsActChats.end();
	for(;itrtms!=endtms; ++itrtms)
		(itrtms->second)->ShowComponent(bShow);
}
// ===============================================================================
CTMessengerChat* CTMessengerChat::GetCurMsgChat()
{
	TMAPMSGTOCHAT::iterator itrtms = ms_TmsActChats.begin();
	TMAPMSGTOCHAT::iterator endtms = ms_TmsActChats.end();
	for(;itrtms!=endtms; ++itrtms)
	{
		if( (itrtms->second)->IsVisible() && (itrtms->second)->GetFocus() )
			return (itrtms->second);
	}

	return NULL;
}
// -------------------------------------------------------------------------------
TEdit* CTMessengerChat::GetCurMsgChatEdit()
{
	TEdit* pEdit;

	TMAPMSGTOCHAT::iterator itrtms = ms_TmsActChats.begin();
	TMAPMSGTOCHAT::iterator endtms = ms_TmsActChats.end();
	for(;itrtms!=endtms; ++itrtms)
	{
		pEdit = (itrtms->second)->GetCurEdit();
		if( pEdit )
			return pEdit;
	}

	return NULL;
}
// ===============================================================================
CTMessengerChat* CTMessengerChat::FindInP2PByTarget(const CString& strTarget)
{
	CTMessengerChat* pChat;

	TMAPMSGTOCHAT::iterator itrtms = ms_TmsActChats.begin();
	TMAPMSGTOCHAT::iterator endtms = ms_TmsActChats.end();
	for(;itrtms!=endtms; ++itrtms)
	{
		pChat = itrtms->second;
		
		if( pChat->GetTargetCount() == 1 && pChat->FindTargetByName(strTarget) )
			return pChat;
	}
	
	return NULL;
}
// ===============================================================================


// ===============================================================================
CTMessengerChat::CTMessengerChat(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc), m_dwTmsID(0)
{
	m_style |= TFS_FLOAT;

	m_pGame = CTClientGame::GetInstance();
	m_pCmd = CTMessengerCommander::GetInstance();

	m_pChatTitle	= FindKid(ID_CTRLINST_CHAT_TITLE);
	//m_pTargetName	= FindKid(ID_CTRLINST_TXT_TARGET);

	m_pImeBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_LANG) );
	//m_pSendBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_SEND) );
	m_pInviteBtn	= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_INVITE) );
	m_pChatMembers	= static_cast<TList*>( FindKid(ID_CTRLINST_LIST_MEMBER) );
	m_pMyFace		= static_cast<TImageList*>( FindKid(ID_CTRLINST_FACE_ME) );
	//m_pTargetFace	= static_cast<TImageList*>( FindKid(ID_CTRLINST_FACE_TARGET) );
	
	m_pCloseBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_CLOSE) );
	m_pCloseBtn->m_menu[TNM_LCLICK] = GM_MESSENGERCHAT_CLOSE;

	m_pChatList		= static_cast<TList*>( FindKid(ID_CTRLINST_LIST_MUTI) );
	m_pChatList->ApplyUserColor(TRUE);

	TEdit* pEdit	= static_cast<TEdit*>( FindKid(ID_CTRLINST_EDITOR) );
	m_pChatEdit		= new TMultiLineEdit(this, pEdit->m_pDESC);
	
	AddKid(m_pChatEdit);
	RemoveKid(pEdit);
	delete pEdit;

	CTClientChar* pMChar = m_pGame->GetMainChar();
	m_strMyName = pMChar->m_strNAME;
	m_pMyFace->SetCurImage( 
		m_pGame->GetFACE(
		pMChar->GetRaceID(), 
		pMChar->m_bSex, 
		pMChar->m_bFace,
		pMChar->m_bHair));

	NotifyImeLocalModeChange();
}
// -------------------------------------------------------------------------------
CTMessengerChat::~CTMessengerChat()
{

}
// ===============================================================================

// ===============================================================================
TEdit* CTMessengerChat::GetCurEdit()
{
	if( !IsVisible() || !GetFocus() )
		return NULL;

	return m_pChatEdit;
}
// ===============================================================================
void CTMessengerChat::SetTargets(LPMCTARGET_ARRAY pTargets)
{
	m_vChatTargets.swap(*pTargets);

	UpdateTarget();
}
// -------------------------------------------------------------------------------
void CTMessengerChat::SetTarget(LPMSGCHAT_TARGET pTarget)
{
	m_vChatTargets.clear();
	m_vChatTargets.push_back(*pTarget);

	UpdateTarget();
}
// -------------------------------------------------------------------------------
void CTMessengerChat::AddTarget(LPMSGCHAT_TARGET pTarget)
{
	m_vChatTargets.push_back(*pTarget);

	UpdateTarget();
}
// -------------------------------------------------------------------------------
BOOL CTMessengerChat::RemoveTarget(const CString& strTargetName)
{
	MCTARGET_ARRAY::iterator itr,end;
	itr = m_vChatTargets.begin();
	end = m_vChatTargets.end();

	BOOL bFind = FALSE;
	for(; itr!=end; ++itr)
	{
		if(itr->strName == strTargetName)
		{
			bFind = TRUE;
			m_vChatTargets.erase(itr);
			break;
		}
	}
	
	if( !bFind )
		return FALSE;

	UpdateTarget();
	
	return TRUE;
}
// -------------------------------------------------------------------------------
void CTMessengerChat::RemoveAllTargets()
{
	m_vChatTargets.clear();
	
	UpdateTarget();
}
// ===============================================================================
void CTMessengerChat::UpdateTarget()
{
	size_t cnt = m_vChatTargets.size();
	if( cnt > 0 )
	{
		const MSGCHAT_TARGET& t = m_vChatTargets.front();
		SetChatTitle(t.strName, cnt);
	}

	m_pChatMembers->RemoveAll();

	CString str;
	for( size_t i=0; i<cnt; ++i )
	{
		INT nLine = m_pChatMembers->AddString(m_vChatTargets[i].strName);
		
		str = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[m_vChatTargets[i].bClass]);
		m_pChatMembers->SetItemString(nLine, 1, str);

		m_pChatMembers->SetItemInt(nLine, 2, m_vChatTargets[i].bLevel);
	}
}
// ===============================================================================
LPMSGCHAT_TARGET CTMessengerChat::FindTargetByName(const CString& strName)
{
	MCTARGET_ARRAY::iterator itr,end;
	itr = m_vChatTargets.begin();
	end = m_vChatTargets.end();

	for(; itr!=end; ++itr)
	{
		if(itr->strName == strName)
			return &(*itr);
	}
	
	return NULL;
}
// ===============================================================================
void CTMessengerChat::SetChatTitle(const CString& strTarget, UINT nCnt)
{
	if( nCnt <= 1 )
		m_pChatTitle->m_strText = strTarget;
	else
		m_pChatTitle->m_strText = CTChart::Format( TSTR_FMT_MSGHAT_TITLE , strTarget,nCnt);
}
// ===============================================================================
void CTMessengerChat::TryChatMsgInEdit()
{
	CString strMSG = CTTextLinker::GetInstance()
		->MakeLinkToNetText(m_pChatEdit);

	if( strMSG.GetLength() <= TCHAT_DWORD_SIZE )
		return;

	m_pChatEdit->ClearText();
	m_pChatEdit->ResetTextSetting();

	//!! 메신저 작업중 (X)
	// -------------------------------
	DoChat(this, strMSG);
	// -------------------------------
	//OnChatMsg(m_strMyName, strMSG);
	// -------------------------------
}
// -------------------------------------------------------------------------------
void CTMessengerChat::OnChatMsg(const CString& strSender, const CString& strMessage)
{
	if(!IsEnable())
		return;

	CString strFMTSND;
	DWORD dwCOLOR;

	if( strSender == m_strMyName )
	{
		strFMTSND = CTChart::Format( TSTR_FMT_MSGSENDER_ME);
		dwCOLOR = TCOLOR_SENDER_ME;
	}
	else
	{
		strFMTSND = CTChart::Format( TSTR_FMT_MSGSENDER, strSender);
		dwCOLOR = TCOLOR_SENDER;
	}

	INT nLine = m_pChatList->AddString(strFMTSND);
	m_pChatList->SetUserColor(nLine, 0, dwCOLOR);

	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
	TTLINKLINE_ARRAY vLINES;
	TLLINESETTING_ARRAY2D vSETTINGS2D;

	INT nPos = 0;
	CString strLINE = pTextLinker->SplitTextByCrLf( strMessage, CTTextLinker::LINK_DEF_TOK, nPos);

	while(!strLINE.IsEmpty())
	{
		CString strPMSG = pTextLinker->MakeNetToLinkText( this, strLINE);

		CSize szITEM = m_pChatList->GetItemSize(0);
		szITEM.cx -= TCHAT_MARGINE;
		pTextLinker->SplitTextByComp( m_pChatList, szITEM.cx, strPMSG, vLINES);
		pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

		INT nLineSize = (INT) vLINES.size();
		for( INT i=0; i<nLineSize; i++)
		{
			const TTEXT_LINK_LINE& line = vLINES[i];
			const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

			INT iLoopCnt = m_pChatList->GetItemCount() + nLineSize - TCHAT_SIZE;
			for( INT k=0; k<iLoopCnt; k++)
				m_pChatList->DelItem(0);

			INT iRow = m_pChatList->AddString(line.strMSG);
			m_pChatList->SetUserColor( iRow, 0, TCOLOR_MSG_CHATLIST);

			for( k=0; k<INT(vLineSettings.size()); k++)
			{
				const TLINK_LINE_SETTING& s = vLineSettings[k];
				m_pChatList->AddTextSetting( iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
			}
		}

		vLINES.clear();
		vSETTINGS2D.clear();
		ResetTextSetting();

		strLINE = pTextLinker->SplitTextByCrLf( strMessage, CTTextLinker::LINK_DEF_TOK, nPos);
	}

	INT nItemCnt = m_pChatList->GetItemCount();
	if(nItemCnt)
		m_pChatList->SetCurSelItem(nItemCnt - 1);
}
// ===============================================================================
BOOL CTMessengerChat::InviteSelectedFriendInList()
{
	if(!m_bEnable)
		return FALSE;

	CTMessengerBase* pBase = m_pGame->GetMessengerBase();
	CTMsgGroupList* pList = pBase->GetMsgGroupList();
	CTMsgGroupList::CItem* pItem = pList->GetSelectItem();
	if( !pItem )
		return FALSE;

	const CString& name = pItem->GetNameComp()->m_strText;
	for(size_t i=0; i<m_vChatTargets.size(); ++i)
	{
		if( m_vChatTargets[i].strName == name )
			return FALSE;
	}

	VECTORDWORD vTARGET;
	vTARGET.push_back(pItem->m_dwID);

	m_pCmd->DoInviteChatTarget(m_dwTmsID, vTARGET);

	return TRUE;
}
// ===============================================================================

// ===============================================================================
HRESULT CTMessengerChat::Render(DWORD dwTickCount)
{
	if( IsVisible() )
		NotifyImeLocalModeChange();

	return CTClientUIBase::Render(dwTickCount);
}
// ===============================================================================
void CTMessengerChat::EnableComponent(BOOL bEnable)
{
	if( m_bEnable == bEnable )
		return;

	m_bEnable = bEnable;

	if( !m_bEnable )
	{
		m_dwTmsID = 0;

		RemoveAllTargets();

		m_pChatEdit->ClearText();
		m_pChatList->RemoveAll();
	}
}
// ===============================================================================
void CTMessengerChat::ShowComponent(BOOL bVisible)
{
	if( !IsAllVisible() )
		bVisible = FALSE;

	CTClientUIBase::ShowComponent(bVisible);

	//if( bVisible )
	//{
	//	m_pTargetFace->ShowComponent(FALSE);
	//	m_pTargetName->ShowComponent(FALSE);
	//}
}
// ===============================================================================
void CTMessengerChat::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(HitTest(pt))
	{
		DWORD dwInfo;

		if( m_pChatList->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = CTTextLinker::GetInstance()->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					m_pGame->ShowChatDetailInfo(pItem);

				return;
			}
		}

		SwitchFocus(m_pChatEdit);
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// -------------------------------------------------------------------------------
void CTMessengerChat::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pInviteBtn->HitTest(pt) )
		InviteSelectedFriendInList();
	//else if( m_pSendBtn->HitTest(pt) )
	//	TryChatMsgInEdit();

	CTClientUIBase::OnLButtonUp(nFlags,pt);
}
// ===============================================================================
void CTMessengerChat::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	if( nChar == VK_RETURN )
	{
		if( CTClientKEY::IsKeyDown(VK_CONTROL) )
			m_pChatEdit->InsertReturnChar();
		else
			TryChatMsgInEdit();
	}

	CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ===============================================================================


// ===============================================================================
void CTMessengerChat::NotifyImeLocalModeChange()
{
	m_pImeBtn->Select( !CTClientApp::IsImeLocalMode() );
}
// ===============================================================================













