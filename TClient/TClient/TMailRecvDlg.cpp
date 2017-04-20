#include "Stdafx.h"
#include "TMailRecvDlg.h"
#include "TMailSendDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// =======================================================
CTMailRecvDlg::CTMailRecvDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc), m_pMail(NULL), m_pMailSimple(NULL)
{
	static const DWORD ICONID[] = 
	{
		ID_CTRLINST_ICON1,
		ID_CTRLINST_ICON2,
		ID_CTRLINST_ICON3
	};

	for(INT i=0; i<TMAIL_ITEM_MAX; ++i)
	{
		m_vItemIcons[i] = static_cast<TImageList*>(FindKid(ICONID[i]));
		m_vItemIcons[i]->m_strText.Empty();
	}

	m_pSender	= FindKid(ID_CTRLINST_TXT_MAILTYPE);
	m_pTitle	= FindKid(ID_CTRLINST_TXT_MAILTITLE);
	
	m_pRune		= FindKid(ID_CTRLINST_TXT_RUNE);
	m_pLuna		= FindKid(ID_CTRLINST_TXT_LUNA);
	m_pCron		= FindKid(ID_CTRLINST_TXT_CRON);
	
	m_pReplyBtn	 = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_REPLY));
	m_pReplyBtn->m_menu[TNM_LCLICK] = GM_SHOW_MAILREPLY;

	m_pDeleteBtn = static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_DEL));
	m_pDeleteBtn->m_menu[TNM_LCLICK] = GM_DELETE_MAIL;

	m_pGetItemBtn= static_cast<TButton*>(FindKid(ID_CTRLINST_BTN_GETITEM));
	m_pGetItemBtn->m_menu[TNM_LCLICK] = GM_MAIL_GETITEM;

	TComponent* pCloseBtn = FindKid(ID_CTRLINST_CLOSE);
	pCloseBtn->m_menu[TNM_LCLICK] = GM_CLOSE_RECV_MAIL;
	pCloseBtn = FindKid(ID_CTRLINST_BTN_CANCEL);
	pCloseBtn->m_menu[TNM_LCLICK] = GM_CLOSE_RECV_MAIL;

	m_pMessage	 = static_cast<TList*>(FindKid(ID_CTRLINST_LIST));

	TComponent* pTEXT = FindKid(ID_CTRLINST_TXT_3);
	if(pTEXT)
		pTEXT->m_bWordBreak = FALSE;
}
// -------------------------------------------------------
CTMailRecvDlg::~CTMailRecvDlg()
{
	Clear();
}
// =======================================================

// =======================================================
void CTMailRecvDlg::SetInfo( LPTMAIL pMail, LPTMAIL_SIMPLE pMailSimple)
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
	Clear();

	m_pMail = pMail;
	m_pMailSimple = pMailSimple;

	m_pTitle->ResetTextSetting();
	m_pTitle->m_strText = pTextLinker->MakeNetToSimpleLinkText( m_pTitle, pMailSimple->m_strTitle);
	m_pSender->m_strText = pMailSimple->m_strSender;

	if(m_pTitle->m_strText.IsEmpty())
		m_pTitle->m_strText = CTChart::LoadString( TSTR_FMT_NOTITLE);

	if( pMailSimple->m_bType != POST_BILLS )
	{
		m_pRune->m_strText.Format("%u", pMail->m_dwRune);
		m_pLuna->m_strText.Format("%u", pMail->m_dwLuna);
		m_pCron->m_strText.Format("%u", pMail->m_dwCron);
	}
	else
	{
		m_pRune->m_strText = "0";
		m_pLuna->m_strText = "0";
		m_pCron->m_strText = "0";
	}

	TLLINESETTING_ARRAY2D vSETTINGS2D;
	TTLINKLINE_ARRAY vLINES;
	INT nPos = 0;

	CString strLINE = pTextLinker->SplitTextByCrLf( pMail->m_strMessage, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
	while(!strLINE.IsEmpty())
	{
		CString strPMSG = pTextLinker->MakeNetToLinkText( this, strLINE);
		strPMSG.Remove('\r');

		CSize szITEM = m_pMessage->GetItemSize(0);
		pTextLinker->SplitTextByComp( m_pMessage, szITEM.cx, strPMSG, vLINES);
		pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

		INT nLineSize = (INT) vLINES.size();
		for( INT i=0; i<nLineSize; i++)
		{
			const TTEXT_LINK_LINE& line = vLINES[i];
			const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

			INT iRow = m_pMessage->AddString(line.strMSG);
			for( INT k=0; k<INT(vLineSettings.size()); k++)
			{
				const TLINK_LINE_SETTING& s = vLineSettings[k];
				m_pMessage->AddTextSetting( iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
			}
		}

		vLINES.clear();
		vSETTINGS2D.clear();
		ResetTextSetting();

		strLINE = pTextLinker->SplitTextByCrLf( pMail->m_strMessage, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
	}

	for( INT i=0; i<TMAIL_ITEM_MAX; i++)
	{
		if( !pMail->m_vItems[i] )
			break;
		LPTITEM pItem = pMail->m_vItems[i]->GetTITEM();

		LPTITEMVISUAL pTVISUAL = pMail->m_vItems[i]->GetVisual();
		WORD wImg = pTVISUAL ? pTVISUAL->m_wIcon : 0;

		m_vItemIcons[i]->SetCurImage(wImg);
		m_vItemIcons[i]->ShowComponent(TRUE);

		if( pMail->m_vItems[i]->GetCount() > 1 )
			m_vItemIcons[i]->m_strText.Format("%u", pMail->m_vItems[i]->GetCount());
		else
			m_vItemIcons[i]->m_strText.Empty();
	}
}
// =======================================================
void CTMailRecvDlg::Clear()
{
	m_pMail = NULL;
	m_pMailSimple = NULL;

	m_pTitle->ResetTextSetting();
	m_pMessage->RemoveAll();

	for( INT i=0; i<TMAIL_ITEM_MAX; ++i)
		m_vItemIcons[i]->ShowComponent(FALSE);
}
// =======================================================
void CTMailRecvDlg::ClearAppendedItem()
{
	if( !m_pMail )
		return;

	for(INT i=0; i<TMAIL_ITEM_MAX; ++i)
	{
		if( m_pMail->m_vItems[i] )
		{
			delete m_pMail->m_vItems[i];
			m_pMail->m_vItems[i] = NULL;

			m_vItemIcons[i]->ShowComponent(FALSE);
		}
	}

	m_pMail->m_dwRune = 0;
	m_pMail->m_dwLuna = 0;
	m_pMail->m_dwCron = 0;

	m_pRune->m_strText = "0";
	m_pLuna->m_strText = "0";
	m_pCron->m_strText = "0";
}
// =======================================================
ITDetailInfoPtr CTMailRecvDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( !m_pMail )
		return pInfo;

	for( BYTE i=0; i<TMAIL_ITEM_MAX; ++i )
	{
		CTClientItem* pClientItem = m_pMail->m_vItems[i];
		if( !pClientItem )
			return pInfo;

		if( m_vItemIcons[i]->HitTest(point) ) 
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewItemInst(pClientItem, rc);
		}
	}

	return pInfo;
}
// -------------------------------------------------------
BOOL CTMailRecvDlg::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	if( !m_pMail )
		return FALSE;

	for( BYTE i=0; i<TMAIL_ITEM_MAX; ++i )
	{
		CTClientItem* pClientItem = m_pMail->m_vItems[i];
		if( !pClientItem )
			return FALSE;

		if( m_vItemIcons[i]->HitTest(point) ) 
		{
			outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
			outInfo.m_pItem = pClientItem->GetTITEM();
			outInfo.m_pClientItem = pClientItem;
			
			return TRUE;
		}
	}

	return FALSE;
}
// =======================================================
void CTMailRecvDlg::ShowComponent( BOOL bVisible )
{
	if( m_bVisible == bVisible )
		return;

	CTClientUIBase::ShowComponent(bVisible);

	if( m_pMail && bVisible )
	{
		for( INT i=0; i<TMAIL_ITEM_MAX; ++i)
			m_vItemIcons[i]->ShowComponent( m_pMail->m_vItems[i] != NULL );
	}
}
// ===============================================================================
void CTMailRecvDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(HitTest(pt))
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();
		DWORD dwInfo;

		if( m_pMessage->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					pTGAME->ShowChatDetailInfo(pItem);

				return;
			}
		}

		if( m_pTitle->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					pTGAME->ShowChatDetailInfo(pItem);

				return;
			}
		}

		SwitchFocus(m_pMessage);
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
