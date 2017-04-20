#include "Stdafx.h"
#include "TMailListFrame.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TMailSendDlg.h"
#include "TMailRecvDlg.h"

#define READ_MAIL_COLOR			D3DCOLOR_ARGB(255, 120,120,120)
#define NEW_MAIL_COLOR			D3DCOLOR_ARGB(255, 225,225,225)

// =======================================================
CTMailListFrame::CTMailListFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent,pDesc,TMAIL_FRAME_MAILBOX), m_nPrvScrPos(0), 
	  m_bNeedUpdate(TRUE), m_nSelectIdx(T_INVALID)
{
	static const DWORD dwITEM[] = 
	{
		ID_CTRLINST_ITEM1,
		ID_CTRLINST_ITEM2,
		ID_CTRLINST_ITEM3,
		ID_CTRLINST_ITEM4,
		ID_CTRLINST_ITEM5,
		ID_CTRLINST_ITEM6,
		ID_CTRLINST_ITEM7
	};

	ZeroMemory(m_vItems, sizeof(m_vItems));

	for(INT i=0; i<MAX_LINE; ++i)
	{
		m_vItems[i].m_pButton = static_cast<TButton*>( FindKid(dwITEM[i]) );
		m_vItems[i].m_pButton->SetStateButton();
		m_vItems[i].m_pButton->m_menu[TNM_DBLCLICK] = GM_SHOW_RECVMAIL;
	}

	CPoint ptBase, ptTemp;
	m_vItems[0].m_pButton->GetComponentPos(&ptBase);

	m_vItems[0].m_pTypeTxt = FindKid(ID_CTRLINST_TYPE);
	m_vItems[0].m_pTypeTxt->GetComponentPos(&ptTemp);
	m_vItems[0].m_pTypeTxt->m_bNoHIT = TRUE;
	CPoint ptTypeOffset = ptTemp - ptBase;

	m_vItems[0].m_pSenderTxt = FindKid(ID_CTRLINST_TXT_TYPE);
	m_vItems[0].m_pSenderTxt->GetComponentPos(&ptTemp);
	m_vItems[0].m_pSenderTxt->m_bNoHIT = TRUE;
	CPoint ptSenderOffset = ptTemp - ptBase;

	m_vItems[0].m_pTitleTxt = FindKid(ID_CTRLINST_TXT_TITLE);
	m_vItems[0].m_pTitleTxt->GetComponentPos(&ptTemp);
	m_vItems[0].m_pTitleTxt->m_bNoHIT = TRUE;
	CPoint ptTitleOffset = ptTemp - ptBase;

	m_vItems[0].m_pTimeTxt = FindKid(ID_CTRLINST_TXT_DATA);
	m_vItems[0].m_pTimeTxt->GetComponentPos(&ptTemp);
	m_vItems[0].m_pTimeTxt->m_bNoHIT = TRUE;
	CPoint ptTimeOffset = ptTemp - ptBase;

	for(INT i=1; i<MAX_LINE; ++i)
	{
		m_vItems[i].m_pTypeTxt = new TComponent(this, m_vItems[0].m_pTypeTxt->m_pDESC);
		m_vItems[i].m_pSenderTxt = new TComponent(this, m_vItems[0].m_pSenderTxt->m_pDESC);
		m_vItems[i].m_pTitleTxt = new TComponent(this, m_vItems[0].m_pTitleTxt->m_pDESC);
		m_vItems[i].m_pTimeTxt = new TComponent(this, m_vItems[0].m_pTimeTxt->m_pDESC);
		
		m_vItems[i].m_pTypeTxt->m_id = GetUniqueID();
		m_vItems[i].m_pSenderTxt->m_id = GetUniqueID();
		m_vItems[i].m_pTitleTxt->m_id = GetUniqueID();
		m_vItems[i].m_pTimeTxt->m_id = GetUniqueID();

		m_vItems[i].m_pTypeTxt->m_bNoHIT = TRUE;
		m_vItems[i].m_pSenderTxt->m_bNoHIT = TRUE;
		m_vItems[i].m_pTitleTxt->m_bNoHIT = TRUE;
		m_vItems[i].m_pTimeTxt->m_bNoHIT = TRUE;

		AddKid( m_vItems[i].m_pTypeTxt );
		AddKid( m_vItems[i].m_pSenderTxt );
		AddKid( m_vItems[i].m_pTitleTxt );
		AddKid( m_vItems[i].m_pTimeTxt );
		
		m_vItems[i].m_pButton->GetComponentPos(&ptBase);

		m_vItems[i].m_pTypeTxt->MoveComponent(ptBase+ptTypeOffset);
		m_vItems[i].m_pSenderTxt->MoveComponent(ptBase+ptSenderOffset);
		m_vItems[i].m_pTitleTxt->MoveComponent(ptBase+ptTitleOffset);
		m_vItems[i].m_pTimeTxt->MoveComponent(ptBase+ptTimeOffset);
	}

	for(INT i=0; i<MAX_LINE; ++i)
		m_vItems[i].ShowComponent(FALSE);

	m_pScroll = NULL;
	
	m_pSendBtn = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_SEND));
	m_pSendBtn->m_menu[TNM_LCLICK] = GM_SHOW_NEWMAIL;

	TComponent* pBTN = FindKid(ID_CTRLINST_0);
	if( pBTN )
		pBTN->m_bWordBreak = FALSE;

	m_pPage[0] = FindKid(ID_CTRLINST_PAGE0);
	m_pPage[1] = FindKid(ID_CTRLINST_PAGE1);
	m_pPage[2] = FindKid(ID_CTRLINST_PAGE2);
	m_pPage[3] = FindKid(ID_CTRLINST_PAGE3);
	m_pPage[4] = FindKid(ID_CTRLINST_PAGE4);
	m_pPage[5] = FindKid(ID_CTRLINST_PAGE5);
	m_pPage[6] = FindKid(ID_CTRLINST_PAGE6);

	m_pPage[0]->m_strText.Empty();
	m_pPage[1]->m_strText.Empty();
	m_pPage[2]->m_strText.Empty();
	m_pPage[3]->m_strText.Empty();
	m_pPage[4]->m_strText.Empty();
	m_pPage[5]->m_strText.Empty();
	m_pPage[6]->m_strText.Empty();

	m_wTotalCount = 0;
	m_wCurPage = 1;
}
// -------------------------------------------------------
CTMailListFrame::~CTMailListFrame()
{
	for(size_t i=0; i<m_vMails.size(); ++i)
	{
		Mail& m = m_vMails[i];

		delete m.pSimple;

		if( m.pInfo )
		{
			for(INT j=0; j<TMAIL_ITEM_MAX; ++j)
			{
				if( m.pInfo->m_vItems[j] )
					delete m.pInfo->m_vItems[j];
			}

			delete m.pInfo;
		}
	}
}
// =======================================================

// =======================================================
void CTMailListFrame::AddMail(LPTMAIL_SIMPLE pMail)
{
	Mail m;
	m.pSimple = pMail;
	m.pInfo = NULL;

	m_vMails.push_back(m);
	NotifyUpdate();
}
// -------------------------------------------------------
void CTMailListFrame::SetMail(INT nIdx, LPTMAIL pMail)
{
	if( m_vMails[nIdx].pInfo )
	{
		for(INT j=0; j<TMAIL_ITEM_MAX; ++j)
		{
			if( m_vMails[nIdx].pInfo->m_vItems[j] )
				delete m_vMails[nIdx].pInfo->m_vItems[j];
		}

		delete m_vMails[nIdx].pInfo;
	}

	m_vMails[nIdx].pInfo = pMail;
}
// =======================================================
bool less_mail_by_time(const CTMailListFrame::Mail& l, const CTMailListFrame::Mail& r)
{
	return (l.pSimple->m_nTime > r.pSimple->m_nTime);
}
void CTMailListFrame::SortMail()
{
	std::sort(m_vMails.begin(), m_vMails.end(), less_mail_by_time);
	
	NotifyUpdate();
}
// =======================================================
void CTMailListFrame::RemoveMail(INT nIdx)
{
	MailArray::iterator itr = m_vMails.begin() + nIdx;
	
	Mail& m = *itr;
	delete m.pSimple;

	if( m.pInfo )
	{
		for(INT j=0; j<TMAIL_ITEM_MAX; ++j)
		{
			if( m.pInfo->m_vItems[j] )
				delete m.pInfo->m_vItems[j];
		}

		delete m.pInfo;
	}

	m_vMails.erase(itr);
	NotifyUpdate();
}
// -------------------------------------------------------
void CTMailListFrame::ClearMail()
{
	for(size_t i=0; i<m_vMails.size(); ++i)
	{
		Mail& m = m_vMails[i];

		delete m.pSimple;

		if( m.pInfo )
		{
			for(INT j=0; j<TMAIL_ITEM_MAX; ++j)
			{
				if( m.pInfo->m_vItems[j] )
					delete m.pInfo->m_vItems[j];
			}

			delete m.pInfo;
		}
	}

	m_vMails.clear();
	NotifyUpdate();
}
// =======================================================
LPTMAIL CTMailListFrame::GetMail(INT nIdx) const
{
	return const_cast<LPTMAIL>(m_vMails[nIdx].pInfo);
}
// -------------------------------------------------------
LPTMAIL_SIMPLE CTMailListFrame::GetMailSimple(INT nIdx) const
{
	if( 0 <= nIdx && nIdx < m_vMails.size() )
		return const_cast<LPTMAIL_SIMPLE>(m_vMails[nIdx].pSimple);
	return NULL;	
}
// =======================================================
void CTMailListFrame::SetSelectedIndex(INT nIdx)
{
	m_nSelectIdx = nIdx;
	NotifyUpdate();
}
// -------------------------------------------------------
INT CTMailListFrame::GetSelectedIndex() const
{
	return m_nSelectIdx;
}
// -------------------------------------------------------
INT CTMailListFrame::FindIndexByPostID(DWORD dwPostID) const
{
	for(size_t i=0; i<m_vMails.size(); ++i)
	{
		if( m_vMails[i].pSimple->m_dwPostID == dwPostID )
			return i;
	}

	return T_INVALID;
}
// =======================================================
BOOL CTMailListFrame::IsEmpty() const
{
	return m_vMails.empty();
}
// -------------------------------------------------------
UINT CTMailListFrame::GetCount() const
{
	return (UINT)m_vMails.size();
}
// =======================================================
void CTMailListFrame::ViewMail(INT nIdx)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_vMails[nIdx].pInfo && m_vMails[nIdx].pInfo->m_bRead )
	{
		CTMailRecvDlg* pDlg = static_cast<CTMailRecvDlg*>(
			pGame->GetFrame(TFRAME_MAIL_RECV));

		pDlg->SetInfo(m_vMails[nIdx].pInfo, m_vMails[nIdx].pSimple);
		pGame->EnableUI(TFRAME_MAIL_RECV);
	}
	else
	{
		CTClientSession* pSession = pGame->GetSession();
		if( !pSession )
			return;

		pSession->SendCS_POSTVIEW_REQ( m_vMails[nIdx].pSimple->m_dwPostID );
	}
}
// =======================================================
BOOL CTMailListFrame::IsNewMail() const
{
	for(size_t i=0; i<m_vMails.size(); ++i)
	{
		if( !m_vMails[i].pSimple->m_bRead )
			return TRUE;
	}

	return FALSE;
}
// =======================================================
void CTMailListFrame::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// -------------------------------------------------------
void CTMailListFrame::Update()
{
	if( !m_bNeedUpdate )
		return;

	SortMail();
	INT nMailCnt = GetCount();

	for(INT i=0; i<MAX_LINE; ++i)
	{
		INT nIndex = i;
		m_vItems[i].m_pButton->Select( nIndex == m_nSelectIdx );

		if( nMailCnt > nIndex )
		{
			m_vItems[i].ShowComponent(TRUE);
			m_vItems[i].SetMail( m_vMails[nIndex].pSimple );
		}
		else
		{
			m_vItems[i].ShowComponent(FALSE);
			m_vItems[i].SetMail(NULL);
		}
	}

	m_bNeedUpdate = FALSE;
}
// =======================================================
BOOL CTMailListFrame::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_pScroll) 
		return FALSE;

	if( !IsVisible() || !CanProcess() || !HitTest(pt) )
		return FALSE;

	int nRange;
	int nPos;

	m_pScroll->GetScrollPos( nRange, nPos);
	nPos += zDelta > 0 ? -1 : 1;
	nPos = min( max( nPos, 0), nRange);
	m_pScroll->SetScrollPos( nRange, nPos);

	return TRUE;
}
// =======================================================
void CTMailListFrame::ShowComponent(BOOL bVisible)
{
	ITInnerFrame::ShowComponent(bVisible);

	if( bVisible )
		NotifyUpdate();
}
// -------------------------------------------------------
HRESULT CTMailListFrame::Render(DWORD dwTickCount)
{
	Update();
	
	return ITInnerFrame::Render(dwTickCount);
}
// =======================================================
void CTMailListFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	for(INT i=0; i<MAX_LINE; ++i)
	{
		TButton* pFrame = m_vItems[i].m_pButton;
		if( pFrame->IsVisible() && pFrame->HitTest(pt) )
		{
			CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
			CTClientGame *pTGAME = CTClientGame::GetInstance();
			DWORD dwInfo;

			if( m_vItems[i].m_pTitleTxt->HitTestTextSetting(pt, &dwInfo) )
			{
				CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

				if( pLinkData && pLinkData->IsItemType() )
				{
					CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

					if(pItem)
						pTGAME->ShowChatDetailInfo(pItem);
				}
			}

			INT nSelIdx = i;
			SetSelectedIndex(nSelIdx);
			break;
		}
	}

	ITInnerFrame::OnLButtonDown(nFlags,pt);
}
// -------------------------------------------------------
void CTMailListFrame::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( HitTest(pt) )
	{
		if( m_pPage[0]->HitTest(pt) &&
			!m_pPage[0]->m_strText.IsEmpty() )
		{
			if( m_wCurPage > 0 )
			{
				WORD wCurPageGroup = (m_wCurPage-1) / 5;
				INT nFirstPage = wCurPageGroup * 5 + 1;
				nFirstPage -= 1;
				if( nFirstPage > 0 )
				{
					CTClientSession* pSession = CTClientGame::GetInstance()->GetSession();
					if( pSession )
						pSession->SendCS_POSTLIST_REQ( (WORD) nFirstPage );
				}
				return ;
			}
		}

		if( m_pPage[6]->HitTest(pt) &&
			!m_pPage[6]->m_strText.IsEmpty() )
		{
			if( m_wCurPage > 0 )
			{
				WORD wCurPageGroup = (m_wCurPage-1) / 5;
				INT nFirstPage = wCurPageGroup * 5 + 1;
				nFirstPage += 5;
				if( nFirstPage > 0 )
				{
					CTClientSession* pSession = CTClientGame::GetInstance()->GetSession();
					if( pSession )
						pSession->SendCS_POSTLIST_REQ( (WORD) nFirstPage );
				}
				return ;
			}
		}

		for( int i=1 ; i < 6 ; ++i )
		{
			if( !m_pPage[i]->m_strText.IsEmpty() &&
				m_pPage[i]->HitTest(pt) )
			{
				WORD wCurPageGroup = (m_wCurPage-1) / 5;
				INT nFirstPage = wCurPageGroup * 5 + i;
				if( nFirstPage > 0 )
				{
					CTClientSession* pSession = CTClientGame::GetInstance()->GetSession();
					if( pSession )
						pSession->SendCS_POSTLIST_REQ( (WORD) nFirstPage );
				}
				return ;
			}
		}
	}

	ITInnerFrame::OnLButtonUp(nFlags, pt);
}
// -------------------------------------------------------
void CTMailListFrame::OnRButtonDown(UINT nFlags, CPoint pt)
{
}
// -------------------------------------------------------
void CTMailListFrame::SetInfo(WORD wTotalCount, WORD wNotReadCount, WORD wCurPage)
{
	m_wTotalCount = wTotalCount;
	m_wCurPage = wCurPage;
	m_pMailDlgTitle->m_strText = CTChart::Format( TSTR_MAIL_TITLE, wNotReadCount, wTotalCount);

	if( m_wTotalCount != 0 && m_wCurPage != 0 )
	{
		INT nTotalPageCount = wTotalCount / 7 + (wTotalCount%7 != 0 ? 1 : 0);
		INT nTotalPageGroupCount = nTotalPageCount / 5;
		INT nCurPageGroup = (wCurPage-1) / 5;

		if( nCurPageGroup-1 >= 0 )
		{
			m_pPage[0]->m_strText = CTChart::LoadString( TSTR_MAIL_PREVPAGE );
			m_pPage[0]->SetTextClr( D3DCOLOR_XRGB(210, 210, 210) );
		}
		else
			m_pPage[0]->m_strText.Empty();

		if( nCurPageGroup+1 < nTotalPageGroupCount )
		{
			m_pPage[6]->m_strText = CTChart::LoadString( TSTR_MAIL_NEXTPAGE );
			m_pPage[6]->SetTextClr( D3DCOLOR_XRGB(210, 210, 210) );
		}
		else
			m_pPage[6]->m_strText.Empty();

		INT nPage = nCurPageGroup * 5 + 1; // first page in current page group.
		for( int i=1 ; i <= 5 ; ++i )
		{
			if( nPage <= nTotalPageCount )
			{
				m_pPage[i]->m_strText.Format("%d", nPage);

				if( nPage == wCurPage )
					m_pPage[i]->SetTextClr( D3DCOLOR_XRGB(254, 208, 0) );
				else
					m_pPage[i]->SetTextClr( D3DCOLOR_XRGB(210, 210, 210) );

				++nPage;
			}
			else
				m_pPage[i]->m_strText.Empty();
		}
	}
	else
	{
		for( int i=0 ; i < 7 ; ++i )
		{
			m_pPage[i]->m_strText.Empty();
		}
	}
}
// =======================================================

// =======================================================
void CTMailListFrame::Item::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;

	m_pButton->ShowComponent(bShow);
	m_pTypeTxt->ShowComponent(bShow);
	m_pSenderTxt->ShowComponent(bShow);
	m_pTitleTxt->ShowComponent(bShow);
	m_pTimeTxt->ShowComponent(bShow);
}
// -------------------------------------------------------
void CTMailListFrame::Item::SetMail(LPTMAIL_SIMPLE pMail)
{
	static const DWORD MAIL_TYPE_STR[] = 
	{
		TSTR_MAILTYPE_NORMAL,	//POST_NORMAL
		TSTR_MAILTYPE_PACKATE,	//POST_PACKATE
		TSTR_MAILTYPE_BILLS,		//POST_BILLS
		TSTR_MAILTYPE_RETURN,	//POST_RETURN
		TSTR_MAILTYPE_PAYMENT,	//POST_PAYMENT
		TSTR_MAILTYPE_NPC,		//POST_NPC
		TSTR_MAILTYPE_OPERATOR,	//POST_OPERATOR
		TSTR_MAILTYPE_CASH		//POST_CASH
	};
	m_pMail = pMail;

	if( pMail )
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

		m_pTypeTxt->m_strText = CTChart::LoadString( (TSTRING) MAIL_TYPE_STR[pMail->m_bType] );
		m_pSenderTxt->m_strText = pMail->m_strSender;

		m_pTitleTxt->ResetTextSetting();
		m_pTitleTxt->m_strText = pTextLinker->MakeNetToSimpleLinkText( m_pTitleTxt, pMail->m_strTitle);

		if(m_pTitleTxt->m_strText.IsEmpty())
			m_pTitleTxt->m_strText = CTChart::LoadString( TSTR_FMT_NOTITLE);

		CTime time(pMail->m_nTime);
		m_pTimeTxt->m_strText = CTChart::Format( TSTR_FMT_DATE, time.GetMonth(),time.GetDay());

		DWORD dwColor = pMail->m_bRead? READ_MAIL_COLOR: NEW_MAIL_COLOR;
		m_pTypeTxt->SetTextClr(dwColor);
		m_pSenderTxt->SetTextClr(dwColor);
		m_pTitleTxt->SetTextClr(dwColor);
		m_pTimeTxt->SetTextClr(dwColor);
	}
}
// =======================================================