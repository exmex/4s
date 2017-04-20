// OBJPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"
#include "PreviewDlg.h"
#include "OBJPage.h"


// COBJPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(COBJPage, CPropertyPage)
COBJPage::COBJPage()
: CPropertyPage(COBJPage::IDD)
{
	m_nCurACT = -1;
	m_nCurCLK = -1;
	m_nCurCL = -1;

	m_pItem = NULL;
	m_pOBJ = NULL;
	m_pSTR = NULL;
}

COBJPage::~COBJPage()
{
}

void COBJPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACTION, m_cListACT);
	DDX_Control(pDX, IDC_LIST_CLOTHKIND, m_cListCLK);
	DDX_Control(pDX, IDC_LIST_CLOTH, m_cListCL);
	DDX_Control(pDX, IDC_EDIT_ACTION, m_cACT);
	DDX_Control(pDX, IDC_EDIT_CLOTHKIND, m_cCLK);
	DDX_Control(pDX, IDC_EDIT_CLOTH, m_cCL);
}


BEGIN_MESSAGE_MAP(COBJPage, CPropertyPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLOTHKIND, OnLvnItemchangedListClothkind)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, OnHdnItemchangedList)
	ON_MESSAGE( WM_LISTCTRL_UPDATE, OnListUpdate)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COBJPage 메시지 처리기입니다.

BOOL COBJPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_cListCLK.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cListACT.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cListCL.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

	m_cListCLK.InsertColumn( 0, "Cloth Kind ID");
	m_cListACT.InsertColumn( 0, "Action ID");
	m_cListCL.InsertColumn( 0, "Cloth ID");

	m_cListACT.m_dwMessageID = WM_LISTCTRL_UPDATE;
	m_cListACT.m_pHost = this;

	m_cListCLK.m_dwMessageID = WM_LISTCTRL_UPDATE;
	m_cListCLK.m_pHost = this;

	m_cListCL.m_dwMessageID = WM_LISTCTRL_UPDATE;
	m_cListCL.m_pHost = this;

	m_cACT.SetParent(&m_cListACT);
	m_cCLK.SetParent(&m_cListCLK);
	m_cCL.SetParent(&m_cListCL);

	m_nCurACT = -1;
	m_nCurCLK = -1;
	m_nCurCL = -1;

	return TRUE;
}

BOOL COBJPage::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYFIRST &&
		pMsg->wParam == VK_TAB )
		return OnTabKey(pMsg->hwnd);

	if( pMsg->hwnd == m_cListACT.GetSafeHwnd() )
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN		: OnACTListLButtonDown();
		case WM_LBUTTONDBLCLK	:
		case WM_RBUTTONDOWN		:
		case WM_RBUTTONDBLCLK	: return TRUE;
		}
	}

	if( pMsg->hwnd == m_cListCLK.GetSafeHwnd() )
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN		: OnCLKListLButtonDown();
		case WM_LBUTTONDBLCLK	:
		case WM_RBUTTONDOWN		:
		case WM_RBUTTONDBLCLK	: return TRUE;
		}
	}

	if( pMsg->hwnd == m_cListCL.GetSafeHwnd() )
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN		: OnCLListLButtonDown();
		case WM_LBUTTONDBLCLK	:
		case WM_RBUTTONDOWN		:
		case WM_RBUTTONDBLCLK	: return TRUE;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL COBJPage::OnTabKey( HWND hWnd)
{
	if( hWnd == m_cACT.GetSafeHwnd() )
	{
		if(ApplyACTData())
		{
			if(!m_cListACT.GetItemText( m_nCurACT, 0).IsEmpty())
				m_nCurACT++;

			PrepareACTCtrl(TRUE);
		}

		EditACTData();
	}
	else if( hWnd == m_cCLK.GetSafeHwnd() )
	{
		if(ApplyCLKData())
		{
			if(!m_cListCLK.GetItemText( m_nCurCLK, 0).IsEmpty())
				m_nCurCLK++;

			PrepareCLKCtrl(TRUE);
		}

		EditCLKData();
	}
	else if( hWnd == m_cCL.GetSafeHwnd() )
	{
		if(ApplyCLData())
		{
			if(!m_cListCL.GetItemText( m_nCurCL, 0).IsEmpty())
				m_nCurCL++;

			PrepareCLCtrl(TRUE);
		}

		EditCLData();
	}

	return TRUE;
}

void COBJPage::OnSize( UINT nType, int cx, int cy)
{
	if(m_cListACT.GetSafeHwnd())
	{
		CRect rect( 0, 0, cx / 3, cy);

		m_cListACT.MoveWindow(&rect);
		rect.OffsetRect( cx / 3, 0);
		m_cListCLK.MoveWindow(&rect);
		rect.OffsetRect( cx / 3, 0);
		m_cListCL.MoveWindow(&rect);

		UpdateColumnWidth(&m_cListACT);
		UpdateColumnWidth(&m_cListCLK);
		UpdateColumnWidth(&m_cListCL);

		if(m_cACT.IsWindowVisible())
		{
			PrepareACTCtrl(FALSE);
			EditACTData();
		}
		else if(m_cCLK.IsWindowVisible())
		{
			PrepareCLKCtrl(FALSE);
			EditCLKData();
		}
		else if(m_cCL.IsWindowVisible())
		{
			PrepareCLCtrl(FALSE);
			EditCLData();
		}
	}

	CPropertyPage::OnSize( nType, cx, cy);
}

void COBJPage::UpdateColumnWidth( CListCtrl *pListCtrl)
{
	CRect rect;

	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetColumnWidth( 0, -1);
	int nWidth = pListCtrl->GetColumnWidth(0);

	if( nWidth < rect.Width() )
		pListCtrl->SetColumnWidth( 0, rect.Width());
}

BOOL COBJPage::OnACTListLButtonDown()
{
	BOOL bResult = FALSE;
	CWnd *pFocus = NULL;

	LVHITTESTINFO info;
	info.flags = 0;

	GetCursorPos(&info.pt);
	m_cListACT.ScreenToClient(&info.pt);

	if( m_cListACT.SubItemHitTest(&info) != -1 )
	{
		if(m_cACT.IsWindowVisible())
		{
			bResult = ApplyACTData();
			pFocus = &m_cACT;
		}
		else if(m_cCLK.IsWindowVisible())
		{
			bResult = ApplyCLKData();
			pFocus = &m_cCLK;
		}
		else if(m_cCL.IsWindowVisible())
		{
			bResult = ApplyCLData();
			pFocus = &m_cCL;
		}

		if( bResult && ( pFocus != &m_cACT || m_nCurACT != info.iItem ))
		{
			m_nCurACT = info.iItem;
			PrepareACTCtrl(TRUE);
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;

	if(bResult)
		EditACTData();
	else if(pFocus)
		pFocus->SetFocus();

	return bResult;
}

BOOL COBJPage::OnCLKListLButtonDown()
{
	BOOL bResult = FALSE;
	CWnd *pFocus = NULL;

	LVHITTESTINFO info;
	info.flags = 0;

	GetCursorPos(&info.pt);
	m_cListCLK.ScreenToClient(&info.pt);

	if( m_cListCLK.SubItemHitTest(&info) != -1 )
	{
		if(m_cACT.IsWindowVisible())
		{
			bResult = ApplyACTData();
			pFocus = &m_cACT;
		}
		else if(m_cCLK.IsWindowVisible())
		{
			bResult = ApplyCLKData();
			pFocus = &m_cCLK;
		}
		else if(m_cCL.IsWindowVisible())
		{
			bResult = ApplyCLData();
			pFocus = &m_cCL;
		}

		if( bResult && ( pFocus != &m_cCLK || m_nCurCLK != info.iItem ))
		{
			m_nCurCLK = info.iItem;
			PrepareCLKCtrl(TRUE);
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;

	if(bResult)
		EditCLKData();
	else if(pFocus)
		pFocus->SetFocus();

	return bResult;
}

BOOL COBJPage::OnCLListLButtonDown()
{
	BOOL bResult = FALSE;
	CWnd *pFocus = NULL;

	LVHITTESTINFO info;
	info.flags = 0;

	GetCursorPos(&info.pt);
	m_cListCL.ScreenToClient(&info.pt);

	if( m_cListCL.SubItemHitTest(&info) != -1 )
	{
		if(m_cACT.IsWindowVisible())
		{
			bResult = ApplyACTData();
			pFocus = &m_cACT;
		}
		else if(m_cCLK.IsWindowVisible())
		{
			bResult = ApplyCLKData();
			pFocus = &m_cCLK;
		}
		else if(m_cCL.IsWindowVisible())
		{
			bResult = ApplyCLData();
			pFocus = &m_cCL;
		}

		if( bResult && ( pFocus != &m_cCL || m_nCurCL != info.iItem ))
		{
			m_nCurCL = info.iItem;
			PrepareCLCtrl(TRUE);
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;

	if(bResult)
		EditCLData();
	else if(pFocus)
		pFocus->SetFocus();

	return bResult;
}

BOOL COBJPage::CheckACTID( CString strID)
{
	int nCount = m_cListACT.GetItemCount();
	MAPSTR::iterator it;

	if( m_cListACT.GetItemText( m_nCurACT, 0) == strID )
		return TRUE;

	if(!CItemBase::CheckID(strID))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	if( CItemBase::m_mapGlobalID.find(strID) != CItemBase::m_mapGlobalID.end() &&
		CItemBase::m_mapLocalID.find(strID) == CItemBase::m_mapLocalID.end() )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_DUP_ATTRID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	for( it = m_pSTR->begin(); it != m_pSTR->end(); it++)
		if( strID == (*it).second )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);
			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

			return FALSE;
		}

	if(m_pItem)
	{
		for( it = m_pItem->m_mapPIVOT.begin(); it != m_pItem->m_mapPIVOT.end(); it++)
			if( strID == (*it).second )
			{
				CString strMessage;
				CString strTitle;

				strMessage.LoadString(IDS_ERR_DUP_ATTRID);
				strTitle.LoadString(IDR_MAINFRAME);
				MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

				return FALSE;
			}
	}

	return TRUE;
}

BOOL COBJPage::CheckCLKID( CString strID)
{
	int nCount = m_cListCLK.GetItemCount();
	MAPSTR::iterator it;

	if( m_cListCLK.GetItemText( m_nCurCLK, 0) == strID )
		return TRUE;

	if(!CItemBase::CheckID(strID))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	if( CItemBase::m_mapGlobalID.find(strID) != CItemBase::m_mapGlobalID.end() &&
		CItemBase::m_mapLocalID.find(strID) == CItemBase::m_mapLocalID.end() )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_DUP_ATTRID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	for( it = m_pSTR->begin(); it != m_pSTR->end(); it++)
		if( strID == (*it).second )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);
			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

			return FALSE;
		}

	if(m_pItem)
	{
		for( it = m_pItem->m_mapPIVOT.begin(); it != m_pItem->m_mapPIVOT.end(); it++)
			if( strID == (*it).second )
			{
				CString strMessage;
				CString strTitle;

				strMessage.LoadString(IDS_ERR_DUP_ATTRID);
				strTitle.LoadString(IDR_MAINFRAME);
				MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

				return FALSE;
			}
	}

	return TRUE;
}

BOOL COBJPage::CheckCLID( CString strID)
{
	int nCount = m_cListCL.GetItemCount();
	MAPSTR::iterator it;

	if( m_cListCL.GetItemText( m_nCurCL, 0) == strID )
		return TRUE;

	if(!CItemBase::CheckID(strID))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	if( CItemBase::m_mapGlobalID.find(strID) != CItemBase::m_mapGlobalID.end() &&
		CItemBase::m_mapLocalID.find(strID) == CItemBase::m_mapLocalID.end() )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_DUP_ATTRID);
		strTitle.LoadString(IDR_MAINFRAME);
		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

		return FALSE;
	}

	for( it = m_pSTR->begin(); it != m_pSTR->end(); it++)
		if( strID == (*it).second )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);
			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

			return FALSE;
		}

	if(m_pItem)
	{
		for( it = m_pItem->m_mapPIVOT.begin(); it != m_pItem->m_mapPIVOT.end(); it++)
			if( strID == (*it).second )
			{
				CString strMessage;
				CString strTitle;

				strMessage.LoadString(IDS_ERR_DUP_ATTRID);
				strTitle.LoadString(IDR_MAINFRAME);
				MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

				return FALSE;
			}
	}

	return TRUE;
}

BOOL COBJPage::ApplyACTData()
{
	CString strID;
	m_cACT.GetWindowText(strID);

	if(!strID.IsEmpty())
		if(CheckACTID(strID))
		{
			if( m_nCurACT + 1 == m_cListACT.GetItemCount() )
			{
				m_cListACT.InsertItem( m_nCurACT + 1, NULL);
				m_cListACT.SetItemData( m_nCurACT + 1, 0);
			}

			UpdateACT(strID);
		}
		else
			return FALSE;
	else if(!m_cListACT.GetItemText( m_nCurACT, 0).IsEmpty())
		DeleteACTRow();

	return TRUE;
}

BOOL COBJPage::ApplyCLKData()
{
	CString strID;
	m_cCLK.GetWindowText(strID);

	if(!strID.IsEmpty())
		if(CheckCLKID(strID))
		{
			if( m_nCurCLK + 1 == m_cListCLK.GetItemCount() )
			{
				m_cListCLK.InsertItem( m_nCurCLK + 1, NULL);
				m_cListCLK.SetItemData( m_nCurCLK + 1, 0);
			}

			UpdateCLK(strID);
		}
		else
			return FALSE;
	else if(!m_cListCLK.GetItemText( m_nCurCLK, 0).IsEmpty())
		DeleteCLKRow();

	return TRUE;
}

BOOL COBJPage::ApplyCLData()
{
	CString strID;
	m_cCL.GetWindowText(strID);

	if(!strID.IsEmpty())
		if(CheckCLID(strID))
		{
			if( m_nCurCL + 1 == m_cListCL.GetItemCount() )
			{
				m_cListCL.InsertItem( m_nCurCL + 1, NULL);
				m_cListCL.SetItemData( m_nCurCL + 1, 0);
			}

			UpdateCL(strID);
		}
		else
			return FALSE;
	else if(!m_cListCL.GetItemText( m_nCurCL, 0).IsEmpty())
		DeleteCLRow();

	return TRUE;
}

void COBJPage::PrepareACTCtrl( BOOL bLoad)
{
	CRect rect;

	m_cListACT.GetItemRect(
		m_nCurACT,
		&rect,
		LVIR_BOUNDS);

	rect.InflateRect( 0, 2);
	rect.OffsetRect( 0, -1);

	if(!bLoad)
	{
		CString strText;

		m_cACT.GetWindowText(strText);
		m_cACT.SetWindowText(strText);
	}
	else
		m_cACT.SetWindowText(m_cListACT.GetItemText( m_nCurACT, 0));
	m_cACT.MoveWindow(&rect);
}

void COBJPage::PrepareCLKCtrl( BOOL bLoad)
{
	CRect rect;

	m_cListCLK.GetItemRect(
		m_nCurCLK,
		&rect,
		LVIR_BOUNDS);

	rect.InflateRect( 0, 2);
	rect.OffsetRect( 0, -1);

	if(!bLoad)
	{
		CString strText;

		m_cCLK.GetWindowText(strText);
		m_cCLK.SetWindowText(strText);
	}
	else
		m_cCLK.SetWindowText(m_cListCLK.GetItemText( m_nCurCLK, 0));
	m_cCLK.MoveWindow(&rect);
}

void COBJPage::PrepareCLCtrl( BOOL bLoad)
{
	CRect rect;

	m_cListCL.GetItemRect(
		m_nCurCL,
		&rect,
		LVIR_BOUNDS);

	rect.InflateRect( 0, 2);
	rect.OffsetRect( 0, -1);

	if(!bLoad)
	{
		CString strText;

		m_cCL.GetWindowText(strText);
		m_cCL.SetWindowText(strText);
	}
	else
		m_cCL.SetWindowText(m_cListCL.GetItemText( m_nCurCL, 0));
	m_cCL.MoveWindow(&rect);
}

void COBJPage::SetData( COBJItem *pItem,
					    LPOBJECT pOBJ,
						MAPSTR *pSTR)
{
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;

	m_pItem = pItem;
	m_pOBJ = pOBJ;
	m_pSTR = pSTR;

	for( itACT = m_pOBJ->m_mapACTION.begin(); itACT != m_pOBJ->m_mapACTION.end(); itACT++)
	{
		MAPSTR::iterator finder = m_pSTR->find((*itACT).first);

		int nIndex = m_cListACT.InsertItem( m_cListACT.GetItemCount(), (*finder).second);
		m_cListACT.SetItemData( nIndex, (*itACT).first);
	}

	for( itCLK = m_pOBJ->m_mapCLKIND.begin(); itCLK != m_pOBJ->m_mapCLKIND.end(); itCLK++)
	{
		MAPSTR::iterator finder = m_pSTR->find((*itCLK).first);

		int nIndex = m_cListCLK.InsertItem( m_cListCLK.GetItemCount(), (*finder).second);
		m_cListCLK.SetItemData( nIndex, (*itCLK).first);
	}

	int nIndex = m_cListACT.InsertItem( m_cListACT.GetItemCount(), NULL);
	m_cListACT.SetItemData( nIndex, 0);

	nIndex = m_cListCLK.InsertItem( m_cListCLK.GetItemCount(), NULL);
	m_cListCLK.SetItemData( nIndex, 0);
	m_nCurACT = 0;

	PrepareACTCtrl(TRUE);
	EditACTData();
}

void COBJPage::UpdateACT( CString strACT)
{
	DWORD dwID = (DWORD) m_cListACT.GetItemData(m_nCurACT);

	if( dwID == 0 )
	{
		LPACTION pACTION = new ACTION();

		if(!m_pOBJ->m_mapACTION.empty())
		{
			MAPACTION::iterator it = m_pOBJ->m_mapACTION.end();
			it--;

			dwID = (*it).first + 1;
		}
		else
			dwID = ROOT_ACTIONID;

		m_pOBJ->m_mapACTION.insert( MAPACTION::value_type( dwID, pACTION));
		m_pSTR->insert( MAPSTR::value_type( dwID, strACT));
		m_cListACT.SetItemData( m_nCurACT, dwID);

		if(m_pItem)
			CItemBase::RegLocalID( strACT, m_pItem);
	}
	else
	{
		MAPSTR::iterator finder = m_pSTR->find(dwID);

		if( finder != m_pSTR->end() && (*finder).second != strACT )
		{
			if(m_pItem)
			{
				CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
				CItemBase::RegLocalID( strACT, m_pItem);
			}

			(*finder).second = strACT;
		}
	}

	if(m_pItem)
		m_pItem->GetMainDocument()->UpdateAll();

	m_cListACT.SetItemText( m_nCurACT, 0, strACT);
}

void COBJPage::UpdateCLK( CString strCLK)
{
	DWORD dwID = (DWORD) m_cListCLK.GetItemData(m_nCurCLK);

	if( dwID == 0 )
	{
		LPCLKIND pCLKIND = new CLKIND();

		if(!m_pOBJ->m_mapCLKIND.empty())
		{
			MAPCLKIND::iterator it = m_pOBJ->m_mapCLKIND.end();
			it--;

			dwID = (*it).first + 1;
		}
		else
			dwID = ROOT_CLKINDID;

		m_pOBJ->m_mapCLKIND.insert( MAPCLKIND::value_type( dwID, pCLKIND));
		m_pSTR->insert( MAPSTR::value_type( dwID, strCLK));
		m_cListCLK.SetItemData( m_nCurCLK, dwID);

		if(m_pItem)
			CItemBase::RegLocalID( strCLK, m_pItem);
	}
	else
	{
		MAPSTR::iterator finder = m_pSTR->find(dwID);

		if( finder != m_pSTR->end() && (*finder).second != strCLK )
		{
			if(m_pItem)
			{
				CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
				CItemBase::RegLocalID( strCLK, m_pItem);
			}

			(*finder).second = strCLK;
		}
	}

	if(m_pItem)
		m_pItem->GetMainDocument()->UpdateAll();

	m_cListCLK.SetItemText( m_nCurCLK, 0, strCLK);
}

void COBJPage::UpdateCL( CString strCL)
{
	DWORD dwCLK = (DWORD) m_cListCLK.GetItemData(m_nCurCLK);

	if( dwCLK != 0 )
	{
		DWORD dwID = (DWORD) m_cListCL.GetItemData(m_nCurCL);

		if( dwID == 0 )
		{
			MAPCLKIND::iterator itCLK = m_pOBJ->m_mapCLKIND.find(dwCLK);
			LPCLKIND pCLK = (*itCLK).second;
			LPCLOTH pCL = new CLOTH();

			dwID = ROOT_CLOTHID;
			while(m_pSTR->find(dwID) != m_pSTR->end())
				dwID++;

			pCLK->m_mapCLOTH.insert( MAPCLOTH::value_type( dwID, pCL));
			m_pSTR->insert( MAPSTR::value_type( dwID, strCL));
			m_cListCL.SetItemData( m_nCurCL, dwID);

			if(m_pItem)
				CItemBase::RegLocalID( strCL, m_pItem);
		}
		else
		{
			MAPSTR::iterator finder = m_pSTR->find(dwID);

			if( finder != m_pSTR->end() && (*finder).second != strCL )
			{
				if(m_pItem)
				{
					CItemBase::ReleaseLocalID( strCL, m_pItem);
					CItemBase::RegLocalID( strCL, m_pItem);
				}

				(*finder).second = strCL;
			}
		}

		m_cListCL.SetItemText( m_nCurCL, 0, strCL);
	}

	if(m_pItem)
		m_pItem->GetMainDocument()->UpdateAll();
}

void COBJPage::DeleteACTRow()
{
	DWORD dwID = (DWORD) m_cListACT.GetItemData(m_nCurACT);

	if( dwID != 0 )
	{
		MAPACTION::iterator itACT = m_pOBJ->m_mapACTION.find(dwID);
		MAPSTR::iterator itSTR = m_pSTR->find(dwID);
		LPACTION pACT = (*itACT).second;

		if(m_pItem)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);

		m_pOBJ->m_mapACTION.erase(itACT);
		m_pSTR->erase(itSTR);

		if(m_pItem)
		{
			MAPSFXINST::iterator itSFX;
			MAPANI::iterator itANI;

			for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
			{
				MAPDWORD::iterator finder = m_pItem->m_mapSFX.find((*itSFX).first);
				if( finder != m_pItem->m_mapSFX.end() )
					m_pItem->m_mapSFX.erase(finder);
			}

			for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
			{
				MAPDWORD::iterator finder = m_pItem->m_mapANI.find((*itANI).first);

				if( finder != m_pItem->m_mapANI.end() )
				{
					itSTR = m_pItem->m_mapNAME.find((*itANI).first);

					CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
					m_pItem->m_mapNAME.erase(itSTR);
					m_pItem->m_mapANI.erase(finder);
				}

				for( itSFX = (*itANI).second->m_mapSFX.begin(); itSFX != (*itANI).second->m_mapSFX.end(); itSFX++)
				{
					finder = m_pItem->m_mapSFX.find((*itSFX).first);
					if( finder != m_pItem->m_mapSFX.end() )
						m_pItem->m_mapSFX.erase(finder);
				}
			}

			m_pItem->Regulate();
			m_pItem->GetMainDocument()->UpdateAll();
		}
		delete pACT;

		m_cListACT.DeleteItem(m_nCurACT);
	}
}

void COBJPage::DeleteCLKRow()
{
	DWORD dwID = (DWORD) m_cListCLK.GetItemData(m_nCurCLK);

	if( dwID != 0 )
	{
		MAPCLKIND::iterator itCLK = m_pOBJ->m_mapCLKIND.find(dwID);
		MAPSTR::iterator itSTR = m_pSTR->find(dwID);
		LPCLKIND pCLK = (*itCLK).second;

		if(m_pItem)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);

		m_pOBJ->m_mapCLKIND.erase(itCLK);
		m_pSTR->erase(itSTR);

		if(m_pItem)
		{
			MAPCLOTH::iterator itCL;

			for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
			{
				LPCLOTH pCL = (*itCL).second;
				MAPMESH::iterator itMESH;

				itSTR = m_pItem->m_mapNAME.find((*itCL).first);
				CItemBase::ReleaseLocalID(
					(*itSTR).second,
					m_pItem);
				m_pItem->m_mapNAME.erase(itSTR);

				for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
				{
					MAPTEXINDEX::iterator itTI = m_pItem->m_mapTI.begin();

					while( itTI != m_pItem->m_mapTI.end() )
					{
						LPTEXINDEX pTI = (*itTI).second;

						if( pTI->m_dwMeshID == (*itMESH).first )
						{
							MAPDWORD::iterator finder = m_pItem->m_mapTEX.find((*itTI).first);
							MAPTEXINDEX::iterator itNEXT = itTI;
							itNEXT++;

							m_pItem->m_mapTEX.erase(finder);
							m_pItem->m_mapTI.erase(itTI);
							delete pTI;

							itTI = itNEXT;
						}
						else
							itTI++;
					}

					itTI = m_pItem->m_mapCOLOR.begin();
					while(itTI != m_pItem->m_mapCOLOR.end())
					{
						LPTEXINDEX pTI = (*itTI).second;

						if( pTI->m_dwMeshID == (*itMESH).first )
						{
							MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*itTI).first);
							MAPTEXINDEX::iterator itNEXT = itTI;
							itNEXT++;

							CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
							m_pItem->m_mapNAME.erase(finder);
							m_pItem->m_mapCOLOR.erase(itTI);
							delete pTI;

							itTI = itNEXT;
						}
						else
							itTI++;
					}

					MAPDWORD::iterator finder = m_pItem->m_mapMESH.find((*itMESH).first);
					if( finder != m_pItem->m_mapMESH.end() )
					{
						itSTR = m_pItem->m_mapNAME.find((*itMESH).first);

						CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
						m_pItem->m_mapNAME.erase(itSTR);
						m_pItem->m_mapMESH.erase(finder);
					}
				}
			}

			m_pItem->GetMainDocument()->Update();
		}
		delete pCLK;

		m_cListCLK.DeleteItem(m_nCurCLK);
	}
}

void COBJPage::DeleteCLRow()
{
	DWORD dwCLK = (DWORD) m_cListCLK.GetItemData(m_nCurCLK);
	DWORD dwID = (DWORD) m_cListCL.GetItemData(m_nCurCL);

	if( dwCLK != 0 && dwID != 0 )
	{
		MAPCLKIND::iterator itCLK = m_pOBJ->m_mapCLKIND.find(dwCLK);
		LPCLKIND pCLK = (*itCLK).second;

		MAPCLOTH::iterator itCL = pCLK->m_mapCLOTH.find(dwID);
		MAPSTR::iterator itSTR = m_pSTR->find(dwID);
		LPCLOTH pCL = (*itCL).second;

		if(m_pItem)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);

		pCLK->m_mapCLOTH.erase(itCL);
		m_pSTR->erase(itSTR);

		if(m_pItem)
		{
			MAPMESH::iterator itMESH;

			for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
			{
				MAPTEXINDEX::iterator itTI = m_pItem->m_mapTI.begin();

				while( itTI != m_pItem->m_mapTI.end() )
				{
					LPTEXINDEX pTI = (*itTI).second;

					if( pTI->m_dwMeshID == (*itMESH).first )
					{
						MAPDWORD::iterator finder = m_pItem->m_mapTEX.find((*itTI).first);
						MAPTEXINDEX::iterator itNEXT = itTI;
						itNEXT++;

						m_pItem->m_mapTEX.erase(finder);
						m_pItem->m_mapTI.erase(itTI);
						delete pTI;

						itTI = itNEXT;
					}
					else
						itTI++;
				}

				itTI = m_pItem->m_mapCOLOR.begin();
				while(itTI != m_pItem->m_mapCOLOR.end())
				{
					LPTEXINDEX pTI = (*itTI).second;

					if( pTI->m_dwMeshID == (*itMESH).first )
					{
						MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*itTI).first);
						MAPTEXINDEX::iterator itNEXT = itTI;
						itNEXT++;

						CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
						m_pItem->m_mapNAME.erase(finder);
						m_pItem->m_mapCOLOR.erase(itTI);
						delete pTI;

						itTI = itNEXT;
					}
					else
						itTI++;
				}

				MAPDWORD::iterator finder = m_pItem->m_mapMESH.find((*itMESH).first);
				if( finder != m_pItem->m_mapMESH.end() )
				{
					itSTR = m_pItem->m_mapNAME.find((*itMESH).first);

					CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
					m_pItem->m_mapNAME.erase(itSTR);
					m_pItem->m_mapMESH.erase(finder);
				}
			}

			m_pItem->Regulate();
			m_pItem->GetMainDocument()->UpdateAll();
		}
		delete pCL;

		m_cListCL.DeleteItem(m_nCurCL);
	}
}

void COBJPage::EditACTData()
{
	m_cListACT.SetItemState(
		m_nCurACT,
		LVIS_SELECTED|LVIS_FOCUSED,
		LVIS_SELECTED|LVIS_FOCUSED);

	m_cACT.ShowWindow(SW_SHOW);
	m_cCLK.ShowWindow(SW_HIDE);
	m_cCL.ShowWindow(SW_HIDE);

	m_cACT.SetFocus();
}

void COBJPage::EditCLKData()
{
	m_cListCLK.SetItemState(
		m_nCurCLK,
		LVIS_SELECTED|LVIS_FOCUSED,
		LVIS_SELECTED|LVIS_FOCUSED);

	m_cACT.ShowWindow(SW_HIDE);
	m_cCLK.ShowWindow(SW_SHOW);
	m_cCL.ShowWindow(SW_HIDE);

	m_cCLK.SetFocus();
}

void COBJPage::EditCLData()
{
	m_cListCL.SetItemState(
		m_nCurCL,
		LVIS_SELECTED|LVIS_FOCUSED,
		LVIS_SELECTED|LVIS_FOCUSED);

	m_cACT.ShowWindow(SW_HIDE);
	m_cCLK.ShowWindow(SW_HIDE);
	m_cCL.ShowWindow(SW_SHOW);

	m_cCL.SetFocus();
}

LRESULT COBJPage::OnListUpdate( WPARAM wParam, LPARAM lParam)
{
	if( m_cACT.IsWindowVisible() && ((CWnd *) wParam) == &m_cListACT )
	{
		PrepareACTCtrl(FALSE);
		EditACTData();
	}
	else if( m_cCLK.IsWindowVisible() && ((CWnd *) wParam) == &m_cListCLK )
	{
		PrepareCLKCtrl(FALSE);
		EditCLKData();
	}
	else if( m_cCL.IsWindowVisible() && ((CWnd *) wParam) == &m_cListCL )
	{
		PrepareCLCtrl(FALSE);
		EditCLData();
	}

	return 0;
}

void COBJPage::OnHdnItemchangedList( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if(m_cACT.IsWindowVisible())
	{
		PrepareACTCtrl(FALSE);
		EditACTData();
	}
	else if(m_cCLK.IsWindowVisible())
	{
		PrepareCLKCtrl(FALSE);
		EditCLKData();
	}
	else if(m_cCL.IsWindowVisible())
	{
		PrepareCLCtrl(FALSE);
		EditCLData();
	}

	*pResult = 0;
}

void COBJPage::OnLvnItemchangedListClothkind( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		DWORD dwCLK = (DWORD) m_cListCLK.GetItemData(pNMLV->iItem);
		m_cListCL.DeleteAllItems();

		if( dwCLK != 0 )
		{
			MAPCLKIND::iterator finder = m_pOBJ->m_mapCLKIND.find(dwCLK);
			MAPCLOTH::iterator it;
			LPCLKIND pCLK = (*finder).second;

			for( it = pCLK->m_mapCLOTH.begin(); it != pCLK->m_mapCLOTH.end(); it++)
			{
				MAPSTR::iterator itSTR = m_pSTR->find((*it).first);

				int nIndex = m_cListCL.InsertItem( m_cListCL.GetItemCount(), (*itSTR).second);
				m_cListCL.SetItemData( nIndex, (*it).first);
			}

			int nIndex = m_cListCL.InsertItem( m_cListCL.GetItemCount(), NULL);
			m_cListCL.SetItemData( nIndex, 0);

			m_cListCL.SetItemState(
				0, LVIS_SELECTED|LVIS_FOCUSED,
				LVIS_SELECTED|LVIS_FOCUSED);

			m_nCurCL = 0;
		}
	}

	*pResult = 0;
}
