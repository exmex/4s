// UserKickOutDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "UserKickOutDlg.h"
#include ".\userkickoutdlg.h"
#include "GMToolDoc.h"

// CUserKickOutDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserKickOutDlg, CDialog)
CUserKickOutDlg::CUserKickOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserKickOutDlg::IDD, pParent)
	,m_strUser		(_T(""))
{
}

CUserKickOutDlg::~CUserKickOutDlg()
{
}

void CUserKickOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserKickOutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUserKickOutDlg 메시지 처리기입니다.

void CUserKickOutDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	GetDlgItemText(IDC_EDIT1, m_strUser);
	m_strUser.Trim(_T(" "));

	if(m_strUser == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		return;
	}

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_USERKICKOUT_REQ(m_strUser);
	OnOK();
}

BOOL CUserKickOutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
