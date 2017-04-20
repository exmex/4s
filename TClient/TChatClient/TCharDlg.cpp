// TCharDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TCharDlg.h"


// CTCharDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTCharDlg, CDialog)
CTCharDlg::CTCharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCharDlg::IDD, pParent)
{
	m_pTCHAR = NULL;
	ClearTCHAR();
}

CTCharDlg::~CTCharDlg()
{
	ClearTCHAR();
}

void CTCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHAR, m_cTCHAR);
}


BEGIN_MESSAGE_MAP(CTCharDlg, CDialog)
END_MESSAGE_MAP()


// CTCharDlg 메시지 처리기입니다.

void CTCharDlg::ClearTCHAR()
{
	while(!m_vTCHAR.empty())
	{
		delete m_vTCHAR.back();
		m_vTCHAR.pop_back();
	}
}

BOOL CTCharDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	m_cTCHAR.DeleteAllItems();

	m_cTCHAR.GetClientRect(&rect);
	rect.DeflateRect( 1, 1);
	m_cTCHAR.InsertColumn( 0, _T("Character"), LVCFMT_CENTER, rect.Width());

	m_cTCHAR.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);

	for( int i=0; i<INT(m_vTCHAR.size()); i++)
	{
		int nIndex = m_cTCHAR.InsertItem( i, m_vTCHAR[i]->m_strNAME);
		m_cTCHAR.SetItemData( nIndex, (DWORD_PTR) m_vTCHAR[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTCharDlg::OnOK()
{
	POSITION pos = m_cTCHAR.GetFirstSelectedItemPosition();

	if(pos)
	{
		int nIndex = m_cTCHAR.GetNextSelectedItem(pos);

		if( nIndex >= 0 && nIndex < INT(m_vTCHAR.size()) )
			m_pTCHAR = m_vTCHAR[nIndex];
	}

	CDialog::OnOK();
}
