// AddMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AddMapDlg.h"


// CAddMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddMapDlg, CDialog)
CAddMapDlg::CAddMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMapDlg::IDD, pParent)
	, m_nType(0)
{
}

CAddMapDlg::~CAddMapDlg()
{
}

void CAddMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_EXTMAP, m_nType);
}


BEGIN_MESSAGE_MAP(CAddMapDlg, CDialog)
END_MESSAGE_MAP()


// CAddMapDlg 메시지 처리기입니다.

BOOL CAddMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	return TRUE;
}
