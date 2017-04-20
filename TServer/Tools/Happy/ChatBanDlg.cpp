// ChatBanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "ChatBanDlg.h"
#include ".\chatbandlg.h"

#include "MainFrm.h"
#include "HappyDoc.h"

// CChatBanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChatBanDlg, CDialog)
CChatBanDlg::CChatBanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBanDlg::IDD, pParent)
	, m_strChatBanName(_T(""))
	, m_iChatBanMin(0)
{
}

CChatBanDlg::~CChatBanDlg()
{
}

void CChatBanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATBAN_CHAR, m_editChatBanName);
	DDX_Text(pDX, IDC_CHATBAN_CHAR, m_strChatBanName);
	DDX_Control(pDX, IDC_CHATBAN_MIN, m_editChatBanMin);
	DDX_Text(pDX, IDC_CHATBAN_MIN, m_iChatBanMin);
	DDX_Control(pDX, IDC_EB_BANREASON, m_ebBanReason);
}


BEGIN_MESSAGE_MAP(CChatBanDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CChatBanDlg 메시지 처리기입니다.

void CChatBanDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	CString strMin=_T("");
	CString strReason=_T("");
	GetDlgItemText(IDC_CHATBAN_MIN,strMin);
	GetDlgItemText(IDC_EB_BANREASON,strReason);
	
	if( m_strChatBanName == _T(""))
	{
		AfxMessageBox(_T("You should enter the name"), MB_OK); // 이름을 입력해야 합니다
		return;
	}

	if( strMin == _T("") )
	{
		AfxMessageBox(_T("You should enter the minute"), MB_OK); // 시간(분)을 입력해야 합니다
		return;
	}

	if( strReason == _T(""))
	{
		AfxMessageBox(_T("You should enter the reason"), MB_OK); // 사유를 입력해야 합니다
		return;
	}

	m_iChatBanMin = (INT)atoi(strMin);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	pDoc->SendCT_CHATBAN_REQ(m_strChatBanName,(WORD)m_iChatBanMin,strReason);
	
	//OnOK();
}

void CChatBanDlg::Init()
{
	//m_editChatBanName.SetFocus();
	m_strChatBanName = _T("");
	m_iChatBanMin = 10;
	SetDlgItemText(IDC_EB_BANREASON,_T(""));
	m_ebBanReason.LimitText(200);

	UpdateData(0);
}
