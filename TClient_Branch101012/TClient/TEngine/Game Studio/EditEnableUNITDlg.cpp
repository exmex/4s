// EditTargetUNITDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditEnableUNITDlg.h"


// EditTargetUNITDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditEnableUNITDlg, CDialog)
CEditEnableUNITDlg::CEditEnableUNITDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditEnableUNITDlg::IDD, pParent),
	  m_pHugeMAP(NULL)
{
}

CEditEnableUNITDlg::~CEditEnableUNITDlg()
{
}

void CEditEnableUNITDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BUTTON_DISABLE, m_cDisable);
	DDX_Control(pDX, ID_BUTTON_ENABLE, m_cEnable);
	DDX_Control(pDX, IDC_LIST_ENABLE_EXPORT, m_cListEnableUnit);
	DDX_Control(pDX, IDC_LIST_DISABLE_EXPORT, m_cListDisableUnit);
}


BEGIN_MESSAGE_MAP(CEditEnableUNITDlg, CDialog)
	ON_BN_CLICKED(ID_BUTTON_DISABLE, OnBnClickedButtonDisable)
	ON_BN_CLICKED(ID_BUTTON_ENABLE, OnBnClickedButtonEnable)
	ON_BN_CLICKED(IDC_BUTTON_EANBLE_UNIT_LIST, OnBnClickedButtonEanbleUnitList)
END_MESSAGE_MAP()

BOOL CEditEnableUNITDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);		
	SetIcon(hIcon, FALSE);		

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVELEFT);
	m_cEnable.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVERIGHT);
	m_cDisable.SetIcon(hIcon);

	SetWindowText("Edit Enable UNIT");
	CRect rect;

	CListCtrl *pEnableListCtrl = m_cListEnableUnit.GetListCtrl();
	pEnableListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pEnableListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP );
	pEnableListCtrl->InsertColumn( 0, NULL, LVCFMT_CENTER, rect.Width());

	CListCtrl *pDisableListCtrl = m_cListDisableUnit.GetListCtrl();

	pDisableListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pDisableListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pDisableListCtrl->InsertColumn( 0, NULL, LVCFMT_CENTER, rect.Width());

	DWORD dwCount = m_pHugeMAP->m_nUnitCountX * m_pHugeMAP->m_nUnitCountZ;
	CString strUNIT;

	for( DWORD i=0; i<dwCount; i++)
	{
		int nUnitX = (i % m_pHugeMAP->m_nUnitCountX) * m_pHugeMAP->m_nUnitLength;
		int nUnitZ = (i / m_pHugeMAP->m_nUnitCountX) * m_pHugeMAP->m_nUnitLength;

		strUNIT.Format( _T("%d : %d"), nUnitX, nUnitZ);
		if(m_pHugeMAP->m_pENABLE[i])
		{
			int nIndex = pEnableListCtrl->InsertItem( pEnableListCtrl->GetItemCount(), strUNIT);

			if( nIndex != -1 )
				pEnableListCtrl->SetItemData( nIndex, i);
		}
		else
		{
			int nIndex = pDisableListCtrl->InsertItem( pDisableListCtrl->GetItemCount(), strUNIT);

			if( nIndex != -1 )
				pDisableListCtrl->SetItemData( nIndex, i);
		}
	}

	return TRUE;
}

void CEditEnableUNITDlg::InitData( CTachyonHUGEMAP* pHugeMap)
{
	m_pHugeMAP = pHugeMap;
}

void CEditEnableUNITDlg::OnBnClickedButtonDisable()
{
	CListCtrl *pDisableListCtrl = m_cListDisableUnit.GetListCtrl();
	CListCtrl *pEnableListCtrl = m_cListEnableUnit.GetListCtrl();

	POSITION pos = pEnableListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pEnableListCtrl->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			int nNEW = pDisableListCtrl->InsertItem( pDisableListCtrl->GetItemCount(), pEnableListCtrl->GetItemText( nIndex, 0));

			if( nNEW >= 0 )
				pDisableListCtrl->SetItemData( nNEW, pEnableListCtrl->GetItemData(nIndex));

			pEnableListCtrl->DeleteItem(nIndex);
		}

		pos = pEnableListCtrl->GetFirstSelectedItemPosition();
	}
}

void CEditEnableUNITDlg::OnBnClickedButtonEnable()
{
	CListCtrl *pDisableListCtrl = m_cListDisableUnit.GetListCtrl();
	CListCtrl *pEnableListCtrl = m_cListEnableUnit.GetListCtrl();

	POSITION pos = pDisableListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pDisableListCtrl->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			int nNEW = pEnableListCtrl->InsertItem( pEnableListCtrl->GetItemCount(), pDisableListCtrl->GetItemText( nIndex, 0));

			if( nNEW >= 0 )
				pEnableListCtrl->SetItemData( nNEW, pDisableListCtrl->GetItemData(nIndex));

			pDisableListCtrl->DeleteItem(nIndex);
		}

		pos = pDisableListCtrl->GetFirstSelectedItemPosition();
	}
}

void CEditEnableUNITDlg::OnOK()
{
	CListCtrl *pEnableListCtrl = m_cListEnableUnit.GetListCtrl();
	int nCount = pEnableListCtrl->GetItemCount();

	ZeroMemory( m_pHugeMAP->m_pENABLE, m_pHugeMAP->m_nUnitCountX * m_pHugeMAP->m_nUnitCountZ * sizeof(BYTE));
	for( int i=0; i<nCount; i++)
		m_pHugeMAP->m_pENABLE[pEnableListCtrl->GetItemData(i)] = TRUE;

	CDialog::OnOK();
}

void CEditEnableUNITDlg::OnBnClickedButtonEanbleUnitList()
{
	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL, 0);

	if( dlg.DoModal() == IDOK )
	{
		CStdioFile vFILE( dlg.GetPathName(), CFile::modeRead);
		CString strREAD;
		DWORD dwCount = m_pHugeMAP->m_nUnitCountX * m_pHugeMAP->m_nUnitCountZ;

		MAPDWORD mapUNIT;
		mapUNIT.clear();

		while(vFILE.ReadString(strREAD))
		{
			int nUnitX;
			int nUnitZ;

			sscanf( strREAD, _T("%d %d"), &nUnitX, &nUnitZ);
			mapUNIT.insert( MAPDWORD::value_type( DWORD(nUnitZ * m_pHugeMAP->m_nUnitCountX + nUnitX), 0));
		}

		CListCtrl *pDisableListCtrl = m_cListDisableUnit.GetListCtrl();
		CListCtrl *pEnableListCtrl = m_cListEnableUnit.GetListCtrl();

		pDisableListCtrl->DeleteAllItems();
		pEnableListCtrl->DeleteAllItems();

		for( DWORD i=0; i<dwCount; i++)
		{
			CListCtrl *pLIST = mapUNIT.find(i) == mapUNIT.end() ? pDisableListCtrl : pEnableListCtrl;
			int nUnitX = (i % m_pHugeMAP->m_nUnitCountX) * m_pHugeMAP->m_nUnitLength;
			int nUnitZ = (i / m_pHugeMAP->m_nUnitCountX) * m_pHugeMAP->m_nUnitLength;

			strREAD.Format( _T("%d : %d"), nUnitX, nUnitZ);
			int nIndex = pLIST->InsertItem( pLIST->GetItemCount(), strREAD);

			if( nIndex != -1 )
				pLIST->SetItemData( nIndex, i);
		}

		mapUNIT.clear();
	}
}
