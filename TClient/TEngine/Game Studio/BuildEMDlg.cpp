// BuildEMDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BuildEMDlg.h"


// CBuildEMDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBuildEMDlg, CDialog)
CBuildEMDlg::CBuildEMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildEMDlg::IDD, pParent)
	, m_nType(FALSE)
{
	m_strTitle.Empty();
}

CBuildEMDlg::~CBuildEMDlg()
{
}

void CBuildEMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SEL_UNIT, m_nType);
}


BEGIN_MESSAGE_MAP(CBuildEMDlg, CDialog)
END_MESSAGE_MAP()


// CBuildEMDlg 메시지 처리기입니다.

BOOL CBuildEMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	SetWindowText(m_strTitle);

	return TRUE;
}
