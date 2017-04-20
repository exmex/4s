// EditObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditObjectDlg.h"


// CEditObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditObjectDlg, CDialog)
CEditObjectDlg::CEditObjectDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEditObjectDlg::IDD, pParent)
{
	m_pItem = NULL;
}

CEditObjectDlg::~CEditObjectDlg()
{
}

void CEditObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditObjectDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CEditObjectDlg 메시지 처리기입니다.

BOOL CEditObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_wndATTR.SetData(
		m_pItem,
		&m_pItem->m_mapATTR,
		&m_pItem->m_mapNAME);

	m_wndOBJ.SetData(
		m_pItem,
		&m_pItem->m_OBJ,
		&m_pItem->m_mapNAME);

	return TRUE;
}

void CEditObjectDlg::OnDestroy()
{
	m_listTabImages.DeleteImageList();
	CDialog::OnDestroy();
}

void CEditObjectDlg::OnSize( UINT nType, int cx, int cy)
{
	if(m_wndSHEET.GetSafeHwnd())
	{
		CTabCtrl *pCtrl = m_wndSHEET.GetTabControl();
		CRect rect( 0, 0, cx, cy);

		m_wndSHEET.MoveWindow(rect);
		pCtrl->MoveWindow(rect);
		pCtrl->AdjustRect(
			FALSE,
			&rect);

		m_wndATTR.MoveWindow(rect);
		if(m_wndOBJ.GetSafeHwnd())
			m_wndOBJ.MoveWindow(rect);
	}

	CDialog::OnSize(nType, cx, cy);
}

int CEditObjectDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndSHEET.AddPage(&m_wndATTR);
	m_wndSHEET.AddPage(&m_wndOBJ);
	m_wndSHEET.Create(
		this,
		WS_CHILD|WS_VISIBLE);

	m_wndSHEET.SetActivePage(&m_wndOBJ);
	m_wndSHEET.SetActivePage(&m_wndATTR);

	CTabCtrl *pCtrl = m_wndSHEET.GetTabControl();
	TCITEM item;

	m_listTabImages.Create(
		IDB_OBJTYPE_TAB,
		17, 1,
		RGB( 255, 255, 255));
	pCtrl->SetImageList(&m_listTabImages);

	item.mask = TCIF_IMAGE;
	item.iImage = 0;
	pCtrl->SetItem( 0, &item);

	item.iImage = 1;
	pCtrl->SetItem( 1, &item);

	return 0;
}
