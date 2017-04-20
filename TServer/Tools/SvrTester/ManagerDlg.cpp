// ManagerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "ManagerDlg.h"
#include ".\managerdlg.h"


// CManagerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManagerDlg, CDialog)
CManagerDlg::CManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManagerDlg::IDD, pParent)
{
}

CManagerDlg::~CManagerDlg()
{
}

void CManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_SERCH_PROTOCOL, OnBnClickedSerchProtocol)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CManagerDlg 메시지 처리기입니다.

void CManagerDlg::OnBnClickedSerchProtocol()
{
	//파일 다이얼로그를 띄어서 프로토콜을 찾는다.
	CFileDialog dlg(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT);

	if(dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();

		SetDlgItemText(IDC_EDIT2, str);	
	}	
}

void CManagerDlg::OnBnClickedOk()
{	
	GetDlgItemText(IDC_IPADDRESS1, m_strIP);
	GetDlgItemText(IDC_EDIT1, m_strPort);
	GetDlgItemText(IDC_EDIT2, m_strFile);
	GetDlgItemText(IDC_EDIT3, m_strUser);
	GetDlgItemText(IDC_EDIT4, m_strPass);

	OnOK();
}

BOOL CManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_IPADDRESS1, m_strIP);
	SetDlgItemText(IDC_EDIT1, m_strPort);
	SetDlgItemText(IDC_EDIT3, m_strUser);

	/*SetDlgItemText(IDC_IPADDRESS1, _T("192.168.1.50"));
	SetDlgItemText(IDC_EDIT1, _T("3615"));
	SetDlgItemText(IDC_EDIT3, _T("kant"));*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
