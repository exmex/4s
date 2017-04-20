// TChatShopDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChatShopDlg.h"
#include "TChatClientDoc.h"
#include "TChatClientView.h"
#include "TChatPriceDlg.h"
#include ".\tchatshopdlg.h"


// CTChatShopDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTChatShopDlg, CDialog)
CTChatShopDlg::CTChatShopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTChatShopDlg::IDD, pParent)
	, m_strTSHOPNAME(_T(""))
{
	m_pHost = NULL;
}

CTChatShopDlg::~CTChatShopDlg()
{
}

void CTChatShopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INVEN, m_cInvenList);
	DDX_Control(pDX, IDC_LIST_ITEM, m_cItemList);
	DDX_Control(pDX, IDC_LIST_SHOP, m_cShopList);
	DDX_Text(pDX, IDC_EDIT_SHOP_NAME, m_strTSHOPNAME);
}


BEGIN_MESSAGE_MAP(CTChatShopDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INVEN, OnLvnItemchangedListInven)
	ON_BN_CLICKED(IDC_BUTTON_TOSHOP, OnBnClickedButtonToshop)
	ON_BN_CLICKED(IDC_BUTTON_TOINVEN, OnBnClickedButtonToinven)
END_MESSAGE_MAP()


// CTChatShopDlg 메시지 처리기입니다.

BOOL CTChatShopDlg::OnInitDialog()
{
	MAPTINVEN::iterator itTINVEN;
	CRect rect;

	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	m_cInvenList.GetClientRect(&rect);
	rect.DeflateRect( 1, 1);

	m_cInvenList.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);

	m_cItemList.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);

	m_cShopList.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);
	m_cInvenList.InsertColumn( 0, _T("Inventory"), LVCFMT_CENTER, rect.Width());

	m_cItemList.GetClientRect(&rect);
	rect.DeflateRect( 1, 1);
	m_cItemList.InsertColumn( 0, _T("Name"), LVCFMT_CENTER, rect.Width() * 2 / 3);
	m_cItemList.InsertColumn( 1, _T("Cost"), LVCFMT_CENTER, rect.Width() / 3);
	m_cShopList.InsertColumn( 0, _T("Name"), LVCFMT_CENTER, rect.Width() * 2 / 3);
	m_cShopList.InsertColumn( 1, _T("Price"), LVCFMT_CENTER, rect.Width() / 3);

	m_cInvenList.DeleteAllItems();
	m_cItemList.DeleteAllItems();
	m_cShopList.DeleteAllItems();

	for( itTINVEN = m_pHost->m_mapTINVEN.begin(); itTINVEN != m_pHost->m_mapTINVEN.end(); itTINVEN++)
		if( (*itTINVEN).second->m_bInvenID != INVEN_EQUIP &&
			(*itTINVEN).second->m_bInvenID != INVEN_TRANS &&
			(*itTINVEN).second->m_bInvenID != INVEN_NULL )
		{
			int nIndex = m_cInvenList.InsertItem( m_cInvenList.GetItemCount(), (*itTINVEN).second->m_pTITEM->m_strNAME);
			m_cInvenList.SetItemData( nIndex, (DWORD_PTR) (*itTINVEN).second);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTChatShopDlg::OnLvnItemchangedListInven(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		CTChatInven *pTINVEN = (CTChatInven *) pNMLV->lParam;
		MAPTITEM::iterator itTITEM;
		m_cItemList.DeleteAllItems();

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
			if( (*itTITEM).second->m_pTITEM->m_bCanTrade & (1<<TTRADEOP_TRADE) )
				if( m_pHost->m_mapTSELLITEM.find((*itTITEM).second) == m_pHost->m_mapTSELLITEM.end() )
				{
					int nIndex = m_cItemList.InsertItem( m_cItemList.GetItemCount(), (*itTITEM).second->m_pTITEM->m_strNAME);
					CString strPRICE;

					strPRICE.Format(
						_T("%d, %d, %d"),
						(*itTITEM).second->m_pTITEM->m_dwPrice / 1000000,
						((*itTITEM).second->m_pTITEM->m_dwPrice % 1000000) / 1000,
						(*itTITEM).second->m_pTITEM->m_dwPrice % 1000);

					m_cItemList.SetItemText( nIndex, 1, strPRICE);
					m_cItemList.SetItemData( nIndex, (DWORD_PTR) (*itTITEM).second);
				}
	}

	*pResult = 0;
}

void CTChatShopDlg::OnBnClickedButtonToshop()
{
	POSITION pos = m_cInvenList.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	int nIndex = m_cInvenList.GetNextSelectedItem(pos);
	if( nIndex < 0 )
		return;

	CTChatInven *pTINVEN = (CTChatInven *) m_cInvenList.GetItemData(nIndex);
	if(!pTINVEN)
		return;

	pos = m_cItemList.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	nIndex = m_cItemList.GetNextSelectedItem(pos);
	if( nIndex < 0 )
		return;

	CTChatItem *pTITEM = (CTChatItem *) m_cItemList.GetItemData(nIndex);
	if(!pTITEM)
		return;

	CTChatPriceDlg dlg;
	dlg.m_pTITEM = pTITEM;

	if( dlg.DoModal() == IDOK )
	{
		CString strPRICE;

		m_pHost->m_mapTSELLITEM.insert( MAPTSELLITEM::value_type( pTITEM, MAKELONG( pTITEM->m_bItemID, pTINVEN->m_bInvenID)));
		m_cItemList.DeleteItem(nIndex);

		strPRICE.Format(
			_T("%d, %d, %d"),
			dlg.m_dwRune,
			dlg.m_dwLuna,
			dlg.m_dwCron);

		pTITEM->m_dwRune = dlg.m_dwRune;
		pTITEM->m_dwLuna = dlg.m_dwLuna;
		pTITEM->m_dwCron = dlg.m_dwCron;

		nIndex = m_cShopList.InsertItem( m_cShopList.GetItemCount(), pTITEM->m_pTITEM->m_strNAME);
		m_cShopList.SetItemText( nIndex, 1, strPRICE);
		m_cShopList.SetItemData( nIndex, (DWORD_PTR) pTITEM);
	}
}

void CTChatShopDlg::OnBnClickedButtonToinven()
{
	POSITION pos = m_cShopList.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	int nIndex = m_cShopList.GetNextSelectedItem(pos);
	if( nIndex < 0 )
		return;

	CTChatItem *pTITEM = (CTChatItem *) m_cShopList.GetItemData(nIndex);
	if(!pTITEM)
		return;

	MAPTSELLITEM::iterator finder = m_pHost->m_mapTSELLITEM.find(pTITEM);
	if( finder != m_pHost->m_mapTSELLITEM.end() )
		m_pHost->m_mapTSELLITEM.erase(finder);

	m_cShopList.DeleteItem(nIndex);
	m_cItemList.DeleteAllItems();

	pos = m_cInvenList.GetFirstSelectedItemPosition();
	if(!pos)
		return;

	nIndex = m_cInvenList.GetNextSelectedItem(pos);
	if( nIndex < 0 )
		return;

	CTChatInven *pTINVEN = (CTChatInven *) m_cInvenList.GetItemData(nIndex);
	if(!pTINVEN)
		return;
	MAPTITEM::iterator itTITEM;

	for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		if( m_pHost->m_mapTSELLITEM.find((*itTITEM).second) == m_pHost->m_mapTSELLITEM.end() )
		{
			int nIndex = m_cItemList.InsertItem( m_cItemList.GetItemCount(), (*itTITEM).second->m_pTITEM->m_strNAME);
			CString strPRICE;

			strPRICE.Format(
				_T("%d, %d, %d"),
				(*itTITEM).second->m_pTITEM->m_dwPrice / 1000000,
				((*itTITEM).second->m_pTITEM->m_dwPrice % 1000000) / 1000,
				(*itTITEM).second->m_pTITEM->m_dwPrice % 1000);

			m_cItemList.SetItemText( nIndex, 1, strPRICE);
			m_cItemList.SetItemData( nIndex, (DWORD_PTR) (*itTITEM).second);
		}
}

void CTChatShopDlg::OnOK()
{
	UpdateData();

	if( m_strTSHOPNAME.IsEmpty() )
	{
		MessageBox("Enter to Store Name");
		return;
	}

	CDialog::OnOK();
}
