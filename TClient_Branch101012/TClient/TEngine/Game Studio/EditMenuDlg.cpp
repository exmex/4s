// EditMenuDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditMenuDlg.h"
#include "PreviewDlg.h"
#include "ItemBase.h"


// CEditMenuDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditMenuDlg, CDialog)
CEditMenuDlg::CEditMenuDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEditMenuDlg::IDD, pParent)
{
	m_strSEL.Empty();
}

CEditMenuDlg::~CEditMenuDlg()
{
}

void CEditMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MENU, m_cMENU);
}


BEGIN_MESSAGE_MAP(CEditMenuDlg, CDialog)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_MENU, OnLvnItemActivateListMenu)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_MENU, OnLvnEndlabeleditListMenu)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_MENU, OnLvnBeginlabeleditListMenu)
	ON_BN_CLICKED(ID_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(ID_BUTTON_DEL, OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CEditMenuDlg 메시지 처리기입니다.

BOOL CEditMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	CRect rect;

	CListCtrl *pListCtrl = m_cMENU.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	MAPSTR::iterator it;
	for( it = CItemBase::m_mapMenuID.begin(); it != CItemBase::m_mapMenuID.end(); it++)
	{
		int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*it).second);

		if( m_strSEL == (*it).second )
			m_cMENU.SetCurSel(nIndex);
	}

	return TRUE;
}

void CEditMenuDlg::DeleteMENU( int nIndex)
{
	CListCtrl *pListCtrl = m_cMENU.GetListCtrl();
	CString strName = pListCtrl->GetItemText( nIndex, 0);
	MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strName);

	if( finder != CItemBase::m_mapGlobalID.end() )
	{
		MAPSTR::iterator itID = CItemBase::m_mapMenuID.find((*finder).second);

		if( itID != CItemBase::m_mapMenuID.end() )
			CItemBase::m_mapMenuID.erase(itID);

		CItemBase::m_mapGlobalID.erase(finder);
		pListCtrl->DeleteItem(nIndex);

		CTachyonDoc::UpdateAll(NULL);
	}
}

void CEditMenuDlg::OnLvnItemActivateListMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteMENU(pNMIA->iItem);
	*pResult = 0;
}

void CEditMenuDlg::OnLvnEndlabeleditListMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl *pListCtrl = m_cMENU.GetListCtrl();
	CString strOLD = pListCtrl->GetItemText( pDispInfo->item.iItem, 0);
	CString strNEW(pDispInfo->item.pszText);

	if( strNEW != strOLD )
		if( strNEW.IsEmpty() || !CItemBase::CheckID(strNEW) )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else if(!CItemBase::CheckDupID( strOLD, strNEW))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else
		{
			MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strOLD);
			DWORD dwID = 1;

			if( finder != CItemBase::m_mapGlobalID.end() )
			{
				dwID = (*finder).second;
				CItemBase::m_mapGlobalID.erase(finder);
			}
			else
			{
				while(CItemBase::m_mapMenuID.find(dwID) != CItemBase::m_mapMenuID.end())
					dwID++;
			}
			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strNEW, dwID));

			MAPSTR::iterator it = CItemBase::m_mapMenuID.find(dwID);
			if( it != CItemBase::m_mapMenuID.end() )
				(*it).second = strNEW;
			else
				CItemBase::m_mapMenuID.insert( MAPSTR::value_type( dwID, strNEW));
			pListCtrl->SetItemText( pDispInfo->item.iItem, 0, strNEW);
		}

	*pResult = 0;
}

void CEditMenuDlg::OnLvnBeginlabeleditListMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CEdit *pEdit = m_cMENU.GetListCtrl()->GetEditControl();
	pEdit->ModifyStyle( 0, ES_UPPERCASE);
	*pResult = 0;
}

void CEditMenuDlg::OnBnClickedButtonAdd()
{
	CString strName = CItemBase::GetNewID(IDS_FMT_NEWMENU);
	CListCtrl *pListCtrl = m_cMENU.GetListCtrl();
	DWORD dwID = 1;

	while(CItemBase::m_mapMenuID.find(dwID) != CItemBase::m_mapMenuID.end())
		dwID++;

	CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strName, dwID));
	CItemBase::m_mapMenuID.insert( MAPSTR::value_type( dwID, strName));
	pListCtrl->InsertItem( pListCtrl->GetItemCount(), strName);
}

void CEditMenuDlg::OnBnClickedButtonDel()
{
	int nIndex = m_cMENU.GetCurSel();

	if( nIndex != -1 )
		DeleteMENU(nIndex);
}

void CEditMenuDlg::OnOK()
{
	int nIndex = m_cMENU.GetCurSel();

	if( nIndex != -1 )
		m_strSEL = m_cMENU.GetListCtrl()->GetItemText( nIndex, 0);
	else
		m_strSEL.Empty();

	CDialog::OnOK();
}
