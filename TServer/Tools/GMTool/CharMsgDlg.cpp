// CharMsgDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "CharMsgDlg.h"
#include ".\charmsgdlg.h"
#include "GMToolDoc.h"

// CCharMsgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCharMsgDlg, CDialog)
CCharMsgDlg::CCharMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharMsgDlg::IDD, pParent)
{
}

CCharMsgDlg::~CCharMsgDlg()
{
}

void CCharMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharMsgDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCharMsgDlg 메시지 처리기입니다.

// Send 버튼 눌렀을 때
void CCharMsgDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strName;
	CString strMsg;

	GetDlgItemText(IDC_EDIT_CHARNAME, strName);
	GetDlgItemText(IDC_EDIT_CHARMSG, strMsg);

	strName.Trim(_T(" "));
	if(strName.IsEmpty() || strMsg.IsEmpty())
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		SetDlgItemText(IDC_EDIT_CHARNAME, _T(""));
		return;
	}

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_CHARMSG_REQ(strName, strMsg);	
}

BOOL CCharMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0); // 모달리스 대화상자가 윈도우 태스크바에 안 뜨게 하기
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
