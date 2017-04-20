#include "Stdafx.h"
#include "TMailSendDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// =======================================================
CTMailSendDlg::CTMailSendDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_nInvenID(INVEN_NULL), m_nSlotID(0), m_pItem(NULL), m_nCurTabIndex(0)
{
	m_pReceiver	= static_cast<TEdit*>(FindKid(ID_CTRLINST_EDIT_RECEIVER));
	m_pTitle	= static_cast<TEdit*>(FindKid(ID_CTRLINST_EDIT_MAIL_TITLE));
	
	m_pReceiver->ClearText();
	m_pTitle->ClearText();
	
	m_pCostTxt	= FindKid(ID_CTRLINST_TXT_COST);
	m_pMyRune	= FindKid(ID_CTRLINST_TXT_RUNE);
	m_pMyLuna	= FindKid(ID_CTRLINST_TXT_LUNA);
	m_pMyCron	= FindKid(ID_CTRLINST_TXT_CRON);

	m_pSndMoneyBtn = static_cast<TButton*>(FindKid(ID_CTRLINST_CHK_SEND_GOLD));
	m_pAskMoneyBtn = static_cast<TButton*>(FindKid(ID_CTRLINST_CHK_REQUEST_GOLD));

	m_pSendBtn	   = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_SEND));
	m_pSendBtn->m_menu[TNM_LCLICK] = GM_SEND_MAIL;

	TComponent* pSendRuneTemp = FindKid(ID_CTRLINST_REQ_RUNE);
	m_pSendRune = new CTNumberEdit(this, pSendRuneTemp->m_pDESC, TRUNE_LENGTH);
	
	TComponent* pSendLunaTemp = FindKid(ID_CTRLINST_REQ_LUNA);
	m_pSendLuna = new CTNumberEdit(this, pSendLunaTemp->m_pDESC, TLUNA_LENGTH);

	TComponent* pSendCronTemp = FindKid(ID_CTRLINST_REQ_CRON);
	m_pSendCron = new CTNumberEdit(this, pSendCronTemp->m_pDESC, TCRON_LENGTH);
	
	AddKid(m_pSendRune);
	AddKid(m_pSendLuna);
	AddKid(m_pSendCron);

	RemoveKid(pSendRuneTemp);
	RemoveKid(pSendLunaTemp);
	RemoveKid(pSendCronTemp);
	
	delete pSendRuneTemp;
	delete pSendLunaTemp;
	delete pSendCronTemp;

	m_pItemIcon	   = static_cast<TImageList*>(FindKid(ID_CTRLINST_ICON));
	m_pItemIcon->m_strText.Empty();

	TComponent* pMsgTplt = FindKid(ID_CTRLINST_EDIT);
	m_pMessage = new TMultiLineEdit(this,pMsgTplt->m_pDESC);

	RemoveKid(pMsgTplt);
	AddKid(m_pMessage);

	m_pMessage->m_id = pMsgTplt->m_id;
	delete pMsgTplt;

	m_vTabSortEdit[0] = m_pReceiver;
	m_vTabSortEdit[1] = m_pTitle;
	m_vTabSortEdit[2] = m_pMessage;
	m_vTabSortEdit[3] = m_pSendRune;
	m_vTabSortEdit[4] = m_pSendLuna;
	m_vTabSortEdit[5] = m_pSendCron;

	ClearUsingByCash();
}
// -------------------------------------------------------
CTMailSendDlg::~CTMailSendDlg()
{
	if( m_pItem )
		delete m_pItem;
}
// =======================================================

// =======================================================
CString CTMailSendDlg::GetReceiver() const
{
	return m_pReceiver->m_strText;
}
// -------------------------------------------------------
CString CTMailSendDlg::GetTitle() const
{
	if( m_pTitle->m_strText.IsEmpty() )
	{
		CString strTitle;
		strTitle = CTChart::LoadString( TSTR_MAIL_TITLE_BLANK);

		return strTitle;
	}
	else
		return m_pTitle->m_strText;
}
// -------------------------------------------------------
CString CTMailSendDlg::GetMessage() const
{
	return m_pMessage->m_strText;
}
// =======================================================
DWORD CTMailSendDlg::GetRune() const
{
	return ::atoi( (LPCSTR)m_pSendRune->m_strText );
}
// -------------------------------------------------------
DWORD CTMailSendDlg::GetLuna() const
{
	return ::atoi( (LPCSTR)m_pSendLuna->m_strText );
}
// -------------------------------------------------------
DWORD CTMailSendDlg::GetCron() const
{
	return ::atoi( (LPCSTR)m_pSendCron->m_strText );
}
// =======================================================
BYTE CTMailSendDlg::GetInvenID() const
{
	return m_nInvenID;
}
// -------------------------------------------------------
BYTE CTMailSendDlg::GetInvenSlot() const
{
	return m_nSlotID;
}
// =======================================================
CString CTMailSendDlg::GetCostTxt(BYTE bMailType)
{
	CString strCost;

	switch(bMailType)
	{
	case POST_NORMAL:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_NORMAL); break;
	case POST_PACKATE:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_PACKAGE); break;
	case POST_BILLS:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_BILLS); break;
	case POST_RETURN:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_RETURN); break;
	case POST_PAYMENT:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_PAYMENT); break;
	case POST_NPC:		strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_NPC); break;
	case POST_OPERATOR:	strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_OPERATOR); break;
	case POST_CASH:		strCost = CTChart::Format( TSTR_FMT_POST_COST, POST_COST_CASH); break;
	}

	return strCost;
}
// =======================================================
BYTE CTMailSendDlg::GetCurrentMailType()
{
	if( m_pAskMoneyBtn->IsStateDown() )
		return POST_BILLS;
	
	if( m_pItem )
		return POST_PACKATE;

	if( GetRune() || GetLuna() || GetCron() )
		return POST_PACKATE;

	return POST_NORMAL;
}
// =======================================================
TEdit* CTMailSendDlg::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	if( m_pSendRune->IsVisible() && m_pSendRune->GetFocus() )
		return m_pSendRune;
	if( m_pSendLuna->IsVisible() && m_pSendLuna->GetFocus() )
		return m_pSendLuna;
	if( m_pSendCron->IsVisible() && m_pSendCron->GetFocus() )
		return m_pSendCron;

	if( m_pMessage->IsVisible() && m_pMessage->GetFocus() )
		return m_pMessage;

	if( m_pReceiver->IsVisible() && m_pReceiver->GetFocus() )
		return m_pReceiver;
	if( m_pTitle->IsVisible() && m_pTitle->GetFocus() )
		return m_pTitle;

	return NULL;
}
// =======================================================
void CTMailSendDlg::SetReceiver(const CString& strReceiver)
{
	m_pReceiver->SetText(strReceiver);
}
// -------------------------------------------------------
void CTMailSendDlg::SetTitle(const CString& strTitle)
{
	m_pTitle->SetText(strTitle);
}
// =======================================================
void CTMailSendDlg::SetItem(CTClientItem* pItem, BYTE nInvenID, BYTE nInvenSlot)
{
	if( m_pItem )
	{
		delete m_pItem;
		m_pItem = NULL;
	}

	if( pItem )
	{
		m_pItem = new CTClientItem;
		
		*m_pItem = *pItem;

		LPTITEMVISUAL pTVISUAL = pItem->GetVisual();
		WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;
		m_pItemIcon->SetCurImage( wImg );

		if( pItem->GetCount() > 1 )
			m_pItemIcon->m_strText.Format("%u", pItem->GetCount());
		else
			m_pItemIcon->m_strText.Empty();
	}

	m_nInvenID = nInvenID;
	m_nSlotID = nInvenSlot;

	m_pItemIcon->ShowComponent( pItem != NULL );
}
// -------------------------------------------------------
void CTMailSendDlg::ChangeItemPos(BYTE nInvenID, BYTE nInvenSlot)
{
	m_nInvenID = nInvenID;
	m_nSlotID = nInvenSlot;
}
void CTMailSendDlg::SetUsingByCash( BYTE bInvenID, BYTE bSlotID)
{
	m_bInvenIDByCashItem = bInvenID;
	m_bSlotIDByCashItem = bSlotID;
	m_bSendByCashItem = 1; // 0:none 1:will send 2:sended
}
void CTMailSendDlg::ClearUsingByCash()
{
	m_bInvenIDByCashItem = 0;
	m_bSlotIDByCashItem = 0;
	m_bSendByCashItem = 0; // 0:none 1:will send 2:sended
}
// =======================================================

// =======================================================
HRESULT CTMailSendDlg::Render(DWORD dwTickCount)
{
	BYTE bMailType = GetCurrentMailType();
	m_pCostTxt->m_strText = GetCostTxt(bMailType);

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	
	m_pMyRune->m_strText.Format("%u", pMainChar->m_dwRune);
	m_pMyLuna->m_strText.Format("%u", pMainChar->m_dwLuna);
	m_pMyCron->m_strText.Format("%u", pMainChar->m_dwCron);

	return CTClientUIBase::Render(dwTickCount);
}
// -------------------------------------------------------
void CTMailSendDlg::ShowComponent(BOOL bVisible)
{
	if( m_bVisible == bVisible )
		return;

	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible )
	{
		m_pSendRune->ClearText();
		m_pSendLuna->ClearText();
		m_pSendCron->ClearText();
		m_pMessage->ClearText();

		m_pSndMoneyBtn->Select(TRUE);
		m_pAskMoneyBtn->Select(FALSE);

		m_nInvenID = INVEN_NULL;
		m_nSlotID = 0;
		
		if( m_pItem )
		{
			delete m_pItem;
			m_pItem = NULL;
		}

		m_pItemIcon->ShowComponent(FALSE);

		BYTE bMailType = GetCurrentMailType();
		m_pCostTxt->m_strText = GetCostTxt(bMailType);

		SwitchFocus(m_pReceiver);
	}
}
// -------------------------------------------------------
void CTMailSendDlg::SwitchFocus(TComponent* pCandidate)
{
	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if( pCandidate )
	{
		for(INT i=0; i<MAX_TAB_IDX; ++i)
		{
			if( m_vTabSortEdit[i] == pCandidate )
			{
				m_nCurTabIndex = i;
				break;
			}
		}

		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}
// -------------------------------------------------------
void CTMailSendDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags,pt);

	if( m_pSndMoneyBtn->HitTest(pt) )
	{
		m_pSndMoneyBtn->Select(TRUE);
		m_pAskMoneyBtn->Select(FALSE);
	}
	else if( m_pAskMoneyBtn->HitTest(pt) )
	{
		m_pSndMoneyBtn->Select(FALSE);
		m_pAskMoneyBtn->Select(TRUE);
	}
}
// -------------------------------------------------------
void CTMailSendDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_TAB )
	{
		++m_nCurTabIndex;

		if( m_nCurTabIndex >= MAX_TAB_IDX )
			m_nCurTabIndex = 0;

		SwitchFocus( m_vTabSortEdit[m_nCurTabIndex] );
	}
	else if( nChar == VK_RETURN && m_pMessage->GetFocus() )
	{
		m_pMessage->InsertReturnChar();
		m_pMessage->MoveScrollToCaret(FALSE);
	}
	else
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}
// =======================================================
ITDetailInfoPtr CTMailSendDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( !m_pItem )
		return pInfo;

	if( m_pItemIcon->HitTest(point) ) 
	{
		CRect rc;
		GetComponentRect(&rc);

		pInfo = CTDetailInfoManager::NewItemInst(m_pItem, rc);
	}

	return pInfo;
}
// -------------------------------------------------------
BOOL CTMailSendDlg::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	if( !m_pItem )
		return FALSE;

	if( m_pItemIcon->HitTest(point) ) 
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = m_pItem->GetTITEM();
		outInfo.m_pClientItem = m_pItem;
		
		return TRUE;
	}

	return FALSE;
}
// =======================================================
TDROPINFO CTMailSendDlg::OnDrop( CPoint point )
{
	TDROPINFO vResult;

	if( HitTest(point) )
		vResult.m_bDrop = TRUE;
	
	return vResult;
}
// -------------------------------------------------------
BYTE CTMailSendDlg::OnBeginDrag( LPTDRAG pDRAG, CPoint point )
{
	if( m_pItem && m_pItemIcon->HitTest(point) )
	{
		if( pDRAG )
		{
			pDRAG->m_pIMAGE = new TImageList(NULL, m_pItemIcon->m_pDESC);
			pDRAG->m_pIMAGE->m_strText = m_pItemIcon->m_strText;
			
			LPTITEM pItemTplt = m_pItem->GetTITEM();

			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pItemTplt);
			WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

			pDRAG->m_pIMAGE->SetCurImage(wImg);
			pDRAG->m_pIMAGE->ShowComponent(TRUE);
			pDRAG->m_pIMAGE->MoveComponent(point);

			SetItem(NULL);
		}

		return TRUE;
	}

	return FALSE;
}
// =======================================================