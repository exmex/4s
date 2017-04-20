// ShopNameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "ShopNameDlg.h"
#include ".\shopnamedlg.h"


// CShopNameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CShopNameDlg, CDialog)
CShopNameDlg::CShopNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopNameDlg::IDD, pParent)
	, m_strNAME(_T(""))
{
}

CShopNameDlg::~CShopNameDlg()
{
}

void CShopNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOP_NAME, m_strNAME);
}


BEGIN_MESSAGE_MAP(CShopNameDlg, CDialog)
END_MESSAGE_MAP()


// CShopNameDlg 메시지 처리기입니다.

BOOL CShopNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CShopNameDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}
