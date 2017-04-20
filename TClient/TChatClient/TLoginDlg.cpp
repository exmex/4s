// TLoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TLoginDlg.h"
#include ".\tlogindlg.h"

// CTLoginDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTLoginDlg, CDialog)
CTLoginDlg::CTLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTLoginDlg::IDD, pParent)
	, m_dwPort(0)
	, m_strID(_T(""))
	, m_strPW(_T(""))
	, m_bPWEncrypt(false)
{
}

CTLoginDlg::~CTLoginDlg()
{
}

void CTLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_LOGIN, m_cIPAddr);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_PW, m_strPW);	
	DDX_Control(pDX, IDC_CHK_PWENCRYPT, m_chkPWEncrypt);
}


BEGIN_MESSAGE_MAP(CTLoginDlg, CDialog)
END_MESSAGE_MAP()


// CTLoginDlg 메시지 처리기입니다.

BOOL CTLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_cIPAddr.SetAddress( 211, 43, 149, 165);	
	m_dwPort = 4815;
	m_bPWEncrypt = TRUE;
	((CButton*)GetDlgItem(IDC_CHK_PWENCRYPT))->SetCheck(TRUE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTLoginDlg::OnOK()
{
	UpdateData();
	m_bPWEncrypt = m_chkPWEncrypt.GetCheck();
	BYTE vIPAddr[4];

	m_cIPAddr.GetAddress(
		vIPAddr[0],
		vIPAddr[1],
		vIPAddr[2],
		vIPAddr[3]);

	m_strIPAddr.Format(
		IDS_FMT_IPADDR,
		vIPAddr[0],
		vIPAddr[1],
		vIPAddr[2],
		vIPAddr[3]);

	CDialog::OnOK();
}
