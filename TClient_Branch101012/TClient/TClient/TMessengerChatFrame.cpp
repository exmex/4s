#include "Stdafx.h"
#include "TMessengerChatFrame.h"
#include "TMessengerChat.h"
#include "TMessengerCommander.h"
#include "TClient.h"
#include "TClientGame.h"
#include "Resource.h"

// ===============================================================================
#define SENDERCOLOR_ME		D3DCOLOR_ARGB(255,150,230,150)
#define SENDERCOLOR			D3DCOLOR_ARGB(255,150,150,230)
// ===============================================================================

// ===============================================================================
CTMessengerChatFrame::CTMessengerChatFrame(CTMessengerChat* pHost, TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc), m_pHost(pHost), m_bMyFaceID(0), m_bTargetFaceID(0)
{
	m_style |= TFS_FLOAT;

	m_pTitle = FindKid(ID_CTRLINST_TITLE);

	m_pImeBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_LANG) );
	m_pSendBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_SEND) );
	m_pInviteBtn	= static_cast<TButton*>( FindKid(ID_CTRLINST_BTN_INVITE) );
	m_pChatMembers	= static_cast<TList*>( FindKid(ID_CTRLINST_LIST_MEMBER) );
	m_pMyFace		= static_cast<TImageList*>( FindKid(ID_CTRLINST_FACE_ME) );
	m_pTargetFace	= static_cast<TImageList*>( FindKid(ID_CTRLINST_FACE_TARGET) );
	
	m_pCloseBtn		= static_cast<TButton*>( FindKid(ID_CTRLINST_CLOSE) );
	m_pCloseBtn->m_menu[TNM_LCLICK] = GM_MESSENGERCHAT_CLOSE;

	m_pChatList		= static_cast<TList*>( FindKid(ID_CTRLINST_LIST_MTM) );
	if( !m_pChatList )
		m_pChatList = static_cast<TList*>( FindKid(ID_CTRLINST_LIST_MUTI) );

	m_pChatList->ApplyUserColor(TRUE);

	if( m_pMyFace )
		m_pMyFace->ShowComponent(FALSE);
	if( m_pTargetFace )
		m_pTargetFace->ShowComponent(FALSE);

	TEdit* pEdit = static_cast<TEdit*>( FindKid(ID_CTRLINST_EDITOR) );
	m_pChatEdit = m_pHost->GetChatEdit(this,pEdit->m_pDESC);

	RemoveKid(pEdit);
	delete pEdit;

	NotifyImeLocalModeChange();
}
// -------------------------------------------------------------------------------
CTMessengerChatFrame::~CTMessengerChatFrame()
{
}
// ===============================================================================

// ===============================================================================
void CTMessengerChatFrame::Clear()
{
	ClearTargetList();
	ClearChatList();
	ClearEditTxt();

	SetTargetFace(0);
}
// ===============================================================================
void CTMessengerChatFrame::SetTitle(const CString& strTarget, UINT nCnt)
{
	if( nCnt <= 1 )
		m_pTitle->m_strText = strTarget;
	else
		m_pTitle->m_strText.Format(IDS_FMT_MSGHAT_TITLE , strTarget,nCnt);
}
// ===============================================================================
void CTMessengerChatFrame::SetMyFace(BYTE bFaceID)
{
	if( m_pMyFace )
	{
		m_bMyFaceID = bFaceID;

		if( bFaceID == 0 )
			m_pMyFace->ShowComponent(FALSE);
		else
		{
			m_pMyFace->ShowComponent(TRUE);
			m_pMyFace->SetCurImage(bFaceID);
		}
	}
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::SetTargetFace(BYTE bFaceID)
{
	if( m_pTargetFace )
	{
		m_bTargetFaceID = bFaceID;

		if( bFaceID == 0 )
			m_pTargetFace->ShowComponent(FALSE);
		else
		{
			m_pTargetFace->ShowComponent(TRUE);
			m_pTargetFace->SetCurImage(bFaceID);
		}
	}
}
// ===============================================================================
void CTMessengerChatFrame::ClearTargetList()
{
	if( m_pChatMembers )
		m_pChatMembers->RemoveAll();
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::AddTargetToList(const CString& strTargetName)
{
	if( m_pChatMembers )
		m_pChatMembers->AddString(strTargetName);
}
// ===============================================================================
void CTMessengerChatFrame::ClearEditTxt()
{
	m_pChatEdit->ClearText();
}
// -------------------------------------------------------------------------------
CString CTMessengerChatFrame::GetEditTxt() const
{
	return m_pChatEdit->m_strText;
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::SetEditTxt(const CString& strTxt)
{
	m_pChatEdit->SetText(strTxt);
}
// ===============================================================================
void CTMessengerChatFrame::ClearChatList()
{
	m_pChatList->RemoveAll();
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::AddChatMsgToList(const CString& strSender, const CString& strMsg)
{
	CString strFMTSND;
	DWORD dwCOLOR;
	
	if( strSender == m_pHost->GetMyName() )
	{
		strFMTSND.Format(IDS_FMT_MSGSENDER_ME);
		dwCOLOR = SENDERCOLOR_ME;
	}
	else
	{
		strFMTSND.Format(IDS_FMT_MSGSENDER, strSender);
		dwCOLOR = SENDERCOLOR;
	}

	INT nLine = m_pChatList->AddString(strFMTSND);
	m_pChatList->SetUserColor(nLine, 0, dwCOLOR);

	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	TTLINKLINE_ARRAY vLINES;
	TLLINESETTING_ARRAY2D vSETTINGS2D;

	INT nPos = 0;
	CString strLINE = pTextLinker->SplitTextByCrLf(strMsg, nPos);
	while( !strLINE.IsEmpty() )
	{
		CString strPMSG = pTextLinker
			->MakeNetToLinkText(this, strLINE);

		CSize szITEM = m_pChatList->GetItemSize(0);
		szITEM.cx -= TCHAT_MARGINE;
		pTextLinker->SplitTextByComp(m_pChatList, szITEM.cx, strPMSG, vLINES);
		pTextLinker->GetTextSettings(this, vLINES, vSETTINGS2D);
		
		INT nLineSize = (INT)vLINES.size();
		for( INT i=0; i<nLineSize; ++i)
		{
			const TTEXT_LINK_LINE& line =  vLINES[i];
			const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

			INT iLoopCnt = m_pChatList->GetItemCount() + nLineSize - TCHAT_SIZE;
			for( INT k=0; k<iLoopCnt; ++k )
				m_pChatList->DelItem(0);

			INT iRow = m_pChatList->AddString(line.strMSG);
			m_pChatList->SetUserColor(iRow, 0, 0xFFFFFFFF);

			for( INT k=0; k < (INT)vLineSettings.size(); ++k )
			{
				const TLINK_LINE_SETTING& s = vLineSettings[k];
				m_pChatList->AddTextSetting(iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
			}
		}
		
		vLINES.clear();
		vSETTINGS2D.clear();

		ResetTextSetting();

		strLINE = pTextLinker->SplitTextByCrLf(strMsg, nPos);
	}

	INT nItemCnt = m_pChatList->GetItemCount();
	if( nItemCnt )
		m_pChatList->SetCurSelItem(nItemCnt-1);
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::SetChatLines(const VECTORSTRING& inLINES, 
										const VECTORDWORD& inCOLORS, 
										const TLLINESETTING_ARRAY2D& inSETTINGS)
{
	m_pChatList->RemoveAll();

	INT nCnt = (INT)inLINES.size();
	for(INT i=0; i<nCnt; ++i)
	{
		m_pChatList->AddString( inLINES[i] );
		m_pChatList->SetUserColor( i, 0, inCOLORS[i] );

		const TLLINESETTING_ARRAY& vLineSettings = inSETTINGS[i];
		for( INT k=0; k < (INT)vLineSettings.size(); ++k )
		{
			const TLINK_LINE_SETTING& s = vLineSettings[k];
			m_pChatList->AddTextSetting(i, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
		}
	}

	if( nCnt > 0 )
		m_pChatList->SetCurSelItem(nCnt-1);
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::GetChatLines(VECTORSTRING& outLINES, 
										VECTORDWORD& outCOLORS, 
										TLLINESETTING_ARRAY2D& outSETTINGS)
{
	INT nItemCnt = m_pChatList->GetItemCount();

	outSETTINGS.clear();
	outSETTINGS.resize(nItemCnt);
	
	for(INT i=0; i<nItemCnt; ++i)
	{
		MAP_LISTITEM* pItemList = m_pChatList->GetItem(i);
		MAP_LISTITEM::iterator itr = pItemList->find(0);
		if( itr != pItemList->end() )
		{
			TListItem* pItem = itr->second;
			outLINES.push_back(pItem->m_strText);
			outCOLORS.push_back(pItem->m_dwTextColor);

			TComponent::TextSettingSet::iterator itr,end;
			itr = pItem->m_TextSettingSet.begin();
			end = pItem->m_TextSettingSet.end();

			for(;itr!=end; ++itr)
			{
				outSETTINGS[i].push_back( 
					TLINK_LINE_SETTING(
					itr->iStart,
					itr->iEnd,
					itr->dwColor,
					itr->dwUser));
			}
		}	
	}
}
// ===============================================================================


// ===============================================================================
HRESULT CTMessengerChatFrame::Render(DWORD dwTickCount)
{
	if( IsVisible() )
		NotifyImeLocalModeChange();

	return CTClientUIBase::Render(dwTickCount);
}
// ===============================================================================
void CTMessengerChatFrame::ShowComponent(BOOL bVisible)
{
	if( !CTMessengerChat::IsAllVisible() )
		bVisible = FALSE;

	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible )
	{
		m_pParent->RemoveKid(this);
		m_pParent->AddKid(this);

		if( m_pMyFace && !m_bMyFaceID )
			m_pMyFace->ShowComponent(FALSE);
		
		if( m_pTargetFace && !m_bTargetFaceID )
			m_pTargetFace->ShowComponent(FALSE);
	}
}
// ===============================================================================
void CTMessengerChatFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( HitTest(pt) )
	{
		DWORD dwInfo;
		if( m_pChatList->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData* pLinkData = CTTextLinker::GetInstance()->GetTextLinkData(dwInfo);
			if( pLinkData && pLinkData->GetType() == TTEXT_LINK_TYPE_ITEM )
			{
				CTClientItem* pItem = pLinkData->GetLastUnkpackItem();
				CTClientGame::GetInstance()->ShowChatDetailInfo(pItem);
				return;
			}
		}
	
		if( m_pChatEdit->GetParent() == this )
			SwitchFocus(m_pChatEdit);
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// -------------------------------------------------------------------------------
void CTMessengerChatFrame::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pImeBtn->HitTest(pt) )
	{
		CTClientApp::ToggleImeLocalMode();
		NotifyImeLocalModeChange();
	}
	else if( m_pInviteBtn->HitTest(pt) )
		m_pHost->InviteSelectedFriendInList();
	else if( m_pSendBtn->HitTest(pt) )
		m_pHost->TryChatMsgInEdit();

	CTClientUIBase::OnLButtonUp(nFlags,pt);
}
// ===============================================================================
void CTMessengerChatFrame::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	if( nChar == VK_RETURN )
		m_pHost->TryChatMsgInEdit();

	CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ===============================================================================


// ===============================================================================
void CTMessengerChatFrame::NotifyImeLocalModeChange()
{
	m_pImeBtn->Select( !CTClientApp::IsImeLocalMode() );
}
// ===============================================================================



























