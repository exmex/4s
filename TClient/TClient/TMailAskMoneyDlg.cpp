#include "Stdafx.h"
#include "TMailAskMoneyDlg.h"
#include "TMailListFrame.h"
#include "TClientGame.h"

// =======================================================
CTMailAskMoneyDlg::CTMailAskMoneyDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc), m_pMail(NULL), m_pMailSimple(NULL), m_bWaitPayAck(FALSE)
{
	m_pRune = FindKid(ID_CTRLINST_TXT_RUNE);
	m_pLuna = FindKid(ID_CTRLINST_TXT_LUNA);
	m_pCron = FindKid(ID_CTRLINST_TXT_CRON);
	
	m_pItemIcon = static_cast<TImageList*>(FindKid(ID_CTRLINST_ICON));

	m_pPayBtn	 = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_PAY));
	m_pPayBtn->m_menu[TNM_LCLICK] = GM_MAILBILL_PAY;

	m_pReturnBtn = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_BACK));
	m_pReturnBtn->m_menu[TNM_LCLICK] = GM_MAILBILL_RETURN;
}
// -------------------------------------------------------
CTMailAskMoneyDlg::~CTMailAskMoneyDlg()
{
	Clear();
}
// =======================================================

// =======================================================
void CTMailAskMoneyDlg::SetInfo(LPTMAIL pMail, LPTMAIL_SIMPLE pMailSimple)
{
	Clear();

	m_pMail = pMail;
	m_pMailSimple = pMailSimple;

	m_pRune->m_strText.Format("%u", pMail->m_dwRune);
	m_pLuna->m_strText.Format("%u", pMail->m_dwLuna);
	m_pCron->m_strText.Format("%u", pMail->m_dwCron);

	if( pMail->m_vItems[0] )
	{
		LPTITEM pItem = pMail->m_vItems[0]->GetTITEM();

		WORD wImg = 0;
		if( pItem )
		{
			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pItem);
			wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;
		}
		m_pItemIcon->SetCurImage(wImg);
		m_pItemIcon->ShowComponent(TRUE);

		if( pMail->m_vItems[0]->GetCount() > 1 )
			m_pItemIcon->m_strText.Format("%u", pMail->m_vItems[0]->GetCount());
		else
			m_pItemIcon->m_strText.Empty();
	}
	else
		m_pItemIcon->ShowComponent(FALSE);
}
// -------------------------------------------------------
void CTMailAskMoneyDlg::Clear()
{
	m_pMail = NULL;
	m_pMailSimple = NULL;
	m_bWaitPayAck = FALSE;
}
// =======================================================
void CTMailAskMoneyDlg::AnswerPay(BOOL bPay)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	DWORD dwPostID = m_pMail->m_dwPostID;
	BYTE bReturnType;

	if( bPay )
	{
		m_bWaitPayAck = TRUE;
		bReturnType = PTURN_PAYMENT;	
	}
	else
	{
		m_bWaitPayAck = FALSE;
		bReturnType = PTURN_RETURN;

		CTMailListFrame* pList = pGame->GetMailList();
		INT nIndex = pList->FindIndexByPostID(dwPostID);
		if( nIndex != T_INVALID )
			pList->RemoveMail(nIndex);
	}

	pSession->SendCS_POSTRETURN_REQ(dwPostID, bReturnType);
}
// =======================================================
ITDetailInfoPtr CTMailAskMoneyDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( !m_pMail )
		return pInfo;

	CTClientItem* pClientItem = m_pMail->m_vItems[0];
	if( !pClientItem )
		return pInfo;

	if( m_pItemIcon->HitTest(point) ) 
	{
		CRect rc;
		GetComponentRect(&rc);

		pInfo = CTDetailInfoManager::NewItemInst(pClientItem, rc);
	}

	return pInfo;
}
// -------------------------------------------------------
BOOL CTMailAskMoneyDlg::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	if( !m_pMail )
		return FALSE;

	CTClientItem* pClientItem = m_pMail->m_vItems[0];
	if( !pClientItem )
		return FALSE;

	if( m_pItemIcon->HitTest(point) ) 
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pClientItem->GetTITEM();
		outInfo.m_pClientItem = pClientItem;
		
		return TRUE;
	}

	return FALSE;
}
// =======================================================










