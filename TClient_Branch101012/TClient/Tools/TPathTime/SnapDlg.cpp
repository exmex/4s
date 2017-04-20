// SnapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathTime.h"
#include "SnapDlg.h"


// CSnapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSnapDlg, CDialog)
CSnapDlg::CSnapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnapDlg::IDD, pParent)
	, m_nSNAP(0)
{
}

CSnapDlg::~CSnapDlg()
{
}

void CSnapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SNAP, m_nSNAP);
}


BEGIN_MESSAGE_MAP(CSnapDlg, CDialog)
END_MESSAGE_MAP()


// CSnapDlg 메시지 처리기입니다.

BOOL CSnapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_nSNAP = theApp.m_nSNAP;
	UpdateData(FALSE);

	return TRUE;
}
