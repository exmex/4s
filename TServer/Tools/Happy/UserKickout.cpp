// 현승룡 UserKickout.cpp

// UserKickout.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "UserKickout.h"
#include ".\userkickout.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CUserKickout 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserKickout, CDialog)
CUserKickout::CUserKickout(CWnd* pParent /*=NULL*/)
	: CDialog(CUserKickout::IDD, pParent)
{
}

CUserKickout::~CUserKickout()
{
}

void CUserKickout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserKickout, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserKickout 메시지 처리기입니다.

void CUserKickout::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	GetDlgItemText(IDC_EDIT1, m_strUser);
	m_strUser.Trim(_T(" "));

	if(m_strUser == _T(""))
		return;

	pDoc->SendCT_USERKICKOUT_REQ(m_strUser);
	//OnOK();
}

void CUserKickout::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
