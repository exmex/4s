// ATTRPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"
#include "PreviewDlg.h"
#include "ATTRPage.h"


// CATTRPage 대화 상자입니다.

IMPLEMENT_DYNAMIC( CATTRPage, CPropertyPage)

CATTRPage::CATTRPage()
: CPropertyPage(CATTRPage::IDD)
{
	m_nCurColumn = -1;
	m_nCurRow = -1;

	m_pItem = NULL;
	m_pATTR = NULL;
	m_pSTR = NULL;
}

CATTRPage::~CATTRPage()
{
}

void CATTRPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ATTR, m_cListCtrl);
	DDX_Control(pDX, IDC_EDIT_ID, m_cEditID);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cType);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_cValue);
}


BEGIN_MESSAGE_MAP( CATTRPage, CPropertyPage)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, OnHdnItemchangedListAttr)
	ON_MESSAGE( WM_LISTCTRL_UPDATE, OnListUpdate)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CATTRPage 메시지 처리기입니다.

BOOL CATTRPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_cListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cListCtrl.InsertColumn( 0, "ID", LVCFMT_LEFT, 200);
	m_cListCtrl.InsertColumn( 1, "Type", LVCFMT_LEFT, 80);
	m_cListCtrl.InsertColumn( 2, "Value");

	m_cListCtrl.m_dwMessageID = WM_LISTCTRL_UPDATE;
	m_cListCtrl.m_pHost = this;

	m_cEditID.SetParent(&m_cListCtrl);
	m_cType.SetParent(&m_cListCtrl);
	m_cValue.SetParent(&m_cListCtrl);

	m_cType.InsertString( 0, "Integer");
	m_cType.InsertString( 1, "Float");
	m_cType.InsertString( 2, "String");

	m_nCurColumn = -1;
	m_nCurRow = -1;

	return TRUE;
}

void CATTRPage::OnSize( UINT nType, int cx, int cy)
{
	if(m_cListCtrl.GetSafeHwnd())
	{
		CRect rect( 0, 0, cx, cy);

		m_cListCtrl.MoveWindow(&rect);
		m_cListCtrl.SetColumnWidth( 0, 200);
		m_cListCtrl.SetColumnWidth( 1, 80);
		m_cListCtrl.SetColumnWidth( 2, -1);

		int nWidth = m_cListCtrl.GetColumnWidth(2);
		m_cListCtrl.GetClientRect(&rect);

		if( nWidth < rect.Width() - 280 )
			m_cListCtrl.SetColumnWidth( 2, rect.Width() - 280);

		PrepareCtrl(FALSE);
		EditData();
	}

	CPropertyPage::OnSize( nType, cx, cy);
}

BOOL CATTRPage::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYFIRST &&
		pMsg->wParam == VK_TAB )
		return OnTabKey(pMsg->hwnd);

	if( pMsg->hwnd == m_cListCtrl.GetSafeHwnd() )
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN		: OnListLButtonDown();
		case WM_LBUTTONDBLCLK	:
		case WM_RBUTTONDOWN		:
		case WM_RBUTTONDBLCLK	: return TRUE;
		}
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CATTRPage::OnTabKey( HWND hWnd)
{
	if( hWnd == m_cEditID.GetSafeHwnd() ||
		hWnd == m_cType.GetSafeHwnd())
	{
		if(ApplyData())
		{
			if(m_cListCtrl.GetItemText( m_nCurRow, 0).IsEmpty())
				m_nCurColumn = 0;
			else
				m_nCurColumn++;

			PrepareCtrl(TRUE);
		}

		EditData();
	}
	else if( hWnd == m_cValue.GetSafeHwnd() )
	{
		if(ApplyData())
		{
			m_nCurColumn = 0;
			m_nCurRow++;
			PrepareCtrl(TRUE);
		}

		EditData();
	}

	return TRUE;
}

BOOL CATTRPage::OnListLButtonDown()
{
	BOOL bResult = TRUE;

	LVHITTESTINFO info;
	info.flags = 0;

	GetCursorPos(&info.pt);
	m_cListCtrl.ScreenToClient(&info.pt);

	if( m_cListCtrl.SubItemHitTest(&info) != -1 && ApplyData() )
	{
		if(m_cListCtrl.GetItemText( info.iItem, 0).IsEmpty())
			info.iSubItem = 0;

		if( m_nCurRow != info.iItem )
			m_nCurRow = info.iItem;
		else
			bResult = FALSE;

		if( bResult || m_nCurColumn != info.iSubItem )
		{
			m_nCurColumn = info.iSubItem;
			PrepareCtrl(TRUE);
		}
	}
	else
		bResult = FALSE;

	EditData();
	return bResult;
}

BOOL CATTRPage::CheckID( CString strID)
{
	int nCount = m_cListCtrl.GetItemCount();
	MAPSTR::iterator it;

	if( m_cListCtrl.GetItemText( m_nCurRow, 0) == strID )
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

BYTE CATTRPage::GetType( CString strType)
{
	static CString strTYPE[3] = {
		CString("Integer"),
		CString("Float"),
		CString("String")};

	for( BYTE i=0; i<3; i++)
		if( strType == strTYPE[i] )
			return i;

	return ((BYTE) 2);
}

void CATTRPage::SetData( COBJItem *pItem,
						 MAPATTRITEM *pATTR,
						 MAPSTR *pSTR)
{
	static CString strTYPE[3] = {
		CString("Integer"),
		CString("Float"),
		CString("String")};

	MAPATTRITEM::iterator it;

	m_pItem = pItem;
	m_pATTR = pATTR;
	m_pSTR = pSTR;

	for( it = m_pATTR->begin(); it != m_pATTR->end(); it++)
	{
		MAPSTR::iterator finder = m_pSTR->find((*it).first);
		LPATTRITEM pITEM = (*it).second;

		int nIndex = m_cListCtrl.InsertItem( m_cListCtrl.GetItemCount(), (*finder).second);

		m_cListCtrl.SetItemText( nIndex, 1, strTYPE[pITEM->m_bType]);
		m_cListCtrl.SetItemText( nIndex, 2, pITEM->m_strValue);
		m_cListCtrl.SetItemData( nIndex, (*it).first);
	}

	int nIndex = m_cListCtrl.InsertItem( m_cListCtrl.GetItemCount(), NULL);
	m_cListCtrl.SetItemData( nIndex, 0);

	m_nCurColumn = 0;
	m_nCurRow = 0;

	PrepareCtrl(TRUE);
	EditData();
}

void CATTRPage::UpdateType( BYTE bType)
{
	static CString strTYPE[3] = {
		CString("Integer"),
		CString("Float"),
		CString("String")};
	MAPATTRITEM::iterator finder = m_pATTR->find((DWORD) m_cListCtrl.GetItemData(m_nCurRow));

	if( finder != m_pATTR->end() )
	{
		(*finder).second->m_bType = bType;
		m_cListCtrl.SetItemText( m_nCurRow, 1, strTYPE[bType]);
	}
}

void CATTRPage::UpdateValue( CString strValue)
{
	MAPATTRITEM::iterator finder = m_pATTR->find((DWORD) m_cListCtrl.GetItemData(m_nCurRow));

	if( finder != m_pATTR->end() )
	{
		(*finder).second->m_strValue = strValue;
		m_cListCtrl.SetItemText( m_nCurRow, 2, strValue);
	}
}

void CATTRPage::UpdateID( CString strID)
{
	DWORD dwID = (DWORD) m_cListCtrl.GetItemData(m_nCurRow);

	if( dwID == 0 )
	{
		LPATTRITEM pATTR = new ATTRITEM();

		if(!m_pATTR->empty())
		{
			MAPATTRITEM::iterator it = m_pATTR->end();
			it--;

			dwID = (*it).first + 1;
		}
		else
			dwID = ROOT_ATTRID;

		m_pATTR->insert( MAPATTRITEM::value_type( dwID, pATTR));
		m_pSTR->insert( MAPSTR::value_type( dwID, strID));
		m_cListCtrl.SetItemData( m_nCurRow, dwID);

		if(m_pItem)
			CItemBase::RegLocalID( strID, m_pItem);
	}
	else
	{
		MAPSTR::iterator finder = m_pSTR->find(dwID);

		if( finder != m_pSTR->end() && (*finder).second != strID )
		{
			if(m_pItem)
			{
				CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
				CItemBase::RegLocalID( strID, m_pItem);
			}

			(*finder).second = strID;
		}
	}

	m_cListCtrl.SetItemText( m_nCurRow, 0, strID);
}

void CATTRPage::DeleteRow()
{
	DWORD dwID = (DWORD) m_cListCtrl.GetItemData(m_nCurRow);

	if( dwID != 0 )
	{
		MAPATTRITEM::iterator itATTR = m_pATTR->find(dwID);
		MAPSTR::iterator itSTR = m_pSTR->find(dwID);

		if(m_pItem)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
		delete (*itATTR).second;

		m_pATTR->erase(itATTR);
		m_pSTR->erase(itSTR);

		m_cListCtrl.DeleteItem(m_nCurRow);
	}
}

BOOL CATTRPage::ApplyData()
{
	switch(m_nCurColumn)
	{
	case 0	:
		{
			CString strID;
			m_cEditID.GetWindowText(strID);

			if(!strID.IsEmpty())
				if(CheckID(strID))
				{
					if( m_nCurRow + 1 == m_cListCtrl.GetItemCount() )
					{
						m_cListCtrl.SetItemText( m_nCurRow, 1, "Integer");

						m_cListCtrl.InsertItem( m_nCurRow + 1, NULL);
						m_cListCtrl.SetItemData( m_nCurRow + 1, 0);
					}

					UpdateID(strID);
				}
				else
					return FALSE;
			else if(!m_cListCtrl.GetItemText( m_nCurRow, 0).IsEmpty())
				DeleteRow();
		}

		break;

	case 1	: UpdateType(m_cType.GetCurSel()); break;
	case 2	:
		{
			CString strValue;

			m_cValue.GetWindowText(strValue);
			UpdateValue(strValue);
		}

		break;
	}

	return TRUE;
}

void CATTRPage::PrepareCtrl( BOOL bLoad)
{
	CRect rect;

	m_cListCtrl.GetSubItemRect(
		m_nCurRow,
		m_nCurColumn,
		LVIR_BOUNDS,
		rect);

	switch(m_nCurColumn)
	{
	case 0	:
		{
			rect.right = rect.left + m_cListCtrl.GetColumnWidth(0);
			rect.InflateRect( 0, 2);
			rect.OffsetRect( 0, -1);

			if(!bLoad)
			{
				CString strText;

				m_cEditID.GetWindowText(strText);
				m_cEditID.SetWindowText(strText);
			}
			else
				m_cEditID.SetWindowText(m_cListCtrl.GetItemText( m_nCurRow, m_nCurColumn));
			m_cEditID.MoveWindow(&rect);
		}

		break;

	case 1	:
		{
			rect.OffsetRect( 0, -3);

			if(!bLoad)
				m_cType.SetCurSel(m_cType.GetCurSel());
			else
				m_cType.SetCurSel(GetType(m_cListCtrl.GetItemText( m_nCurRow, m_nCurColumn)));

			m_cType.MoveWindow(&rect);
		}

		break;

	case 2	:
		{
			rect.InflateRect( 0, 2);
			rect.OffsetRect( 0, -1);

			if(!bLoad)
			{
				CString strText;

				m_cValue.GetWindowText(strText);
				m_cValue.SetWindowText(strText);
			}
			else
				m_cValue.SetWindowText(m_cListCtrl.GetItemText( m_nCurRow, m_nCurColumn));
			m_cValue.MoveWindow(&rect);
		}

		break;
	}
}

void CATTRPage::EditData()
{
	m_cListCtrl.SetItemState(
		m_nCurRow,
		LVIS_SELECTED|LVIS_FOCUSED,
		LVIS_SELECTED|LVIS_FOCUSED);

	switch(m_nCurColumn)
	{
	case 0	:
		{
			m_cEditID.ShowWindow(SW_SHOW);
			m_cType.ShowWindow(SW_HIDE);
			m_cValue.ShowWindow(SW_HIDE);

			m_cEditID.SetFocus();
		}

		break;

	case 1	:
		{
			m_cEditID.ShowWindow(SW_HIDE);
			m_cType.ShowWindow(SW_SHOW);
			m_cValue.ShowWindow(SW_HIDE);

			m_cType.SetFocus();
		}

		break;

	case 2	:
		{
			m_cEditID.ShowWindow(SW_HIDE);
			m_cType.ShowWindow(SW_HIDE);
			m_cValue.ShowWindow(SW_SHOW);

			m_cValue.SetFocus();
		}

		break;
	}
}

void CATTRPage::OnHdnItemchangedListAttr( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	PrepareCtrl(FALSE);
	EditData();

	*pResult = 0;
}

LRESULT CATTRPage::OnListUpdate( WPARAM wParam, LPARAM lParam)
{
	PrepareCtrl(FALSE);
	EditData();

	return 0;
}
