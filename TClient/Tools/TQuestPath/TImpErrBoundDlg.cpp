// TImpErrBoundDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TImpErrBoundDlg.h"


// CTImpErrBoundDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTImpErrBoundDlg, CDialog)
CTImpErrBoundDlg::CTImpErrBoundDlg( CWnd *pParent /*=NULL*/)
: CDialog(CTImpErrBoundDlg::IDD, pParent)
, m_dwBound(0)
{
}

CTImpErrBoundDlg::~CTImpErrBoundDlg()
{
}

void CTImpErrBoundDlg::DoDataExchange( CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BOUND, m_dwBound);
}


BEGIN_MESSAGE_MAP(CTImpErrBoundDlg, CDialog)
END_MESSAGE_MAP()


// CTImpErrBoundDlg 메시지 처리기입니다.

BOOL CTImpErrBoundDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	UpdateData(FALSE);

	return TRUE;
}

void CTImpErrBoundDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}
