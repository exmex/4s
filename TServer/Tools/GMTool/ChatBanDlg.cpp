// ChatBanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "ChatBanDlg.h"
#include ".\chatbandlg.h"

#include "GMToolDoc.h"

// CChatBanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChatBanDlg, CDialog)
CChatBanDlg::CChatBanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBanDlg::IDD, pParent)
	, m_strChatBanName(_T(""))
{
	m_pdlgRet = new CRetDlg();
	m_pdlgRet->Create(IDD_RET);
}

CChatBanDlg::~CChatBanDlg()
{
	if(m_pdlgRet)
	{
		m_pdlgRet->DestroyWindow();
		delete m_pdlgRet;
		m_pdlgRet = NULL;
	}
}

void CChatBanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATBAN_CHAR, m_editChatBanName);
	DDX_Control(pDX, IDC_EB_BANREASON, m_ebBanReason);
	DDX_Control(pDX, IDC_CHATBAN_MIN, m_ebBanTime);
}


BEGIN_MESSAGE_MAP(CChatBanDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CChatBanDlg 메시지 처리기입니다.

void CChatBanDlg::Init()
{
	//m_editChatBanName.SetFocus();
	m_strChatBanName = _T("");
	m_iChatBanMin = 0;
	
	m_ebBanTime.SetWindowText(_T(""));
	m_editChatBanName.SetWindowText(_T(""));
	m_ebBanReason.SetWindowText(_T(""));
	m_ebBanReason.LimitText(200);	
}

void CChatBanDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	CString strMin;
	CString strReason;
	GetDlgItemText(IDC_EB_BANREASON,strReason);
	GetDlgItemText(IDC_CHATBAN_CHAR,m_strChatBanName);
	GetDlgItemText(IDC_CHATBAN_MIN,strMin);
	
	if( m_strChatBanName == _T(""))
	{
		MessageBox(_T("Enter the name"), MB_OK);// 이름을 입력해야 합니다
		return;
	}		
	if( strMin == _T("") )
	{
		MessageBox(_T("Enter the minutes"), MB_OK);// 시간을 입력해야 합니다
		return;
	}
	if( strReason == _T(""))
	{
		AfxMessageBox(_T("Enter the reason"), MB_OK); // 사유를 입력해야 합니다
		return;
	}

	m_iChatBanMin = (INT)atoi(strMin);

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_CHATBAN_REQ(m_strChatBanName,(WORD)m_iChatBanMin,strReason);


	//OnOK();
}

BOOL CChatBanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CChatBanDlg::ShowDlgRet(BYTE bShow, BYTE bRet)
{
	if(m_pdlgRet)
	{
		CString strRet,strName,strMin;
		if(bRet)
		{
			GetDlgItemText(IDC_CHATBAN_CHAR,strName);
			GetDlgItemText(IDC_CHATBAN_MIN,strMin);
			strRet.Format(_T("%s was banned for %s minutes"),strName,strMin);
		}
		else
			strRet.Format(_T("Enter the correct name"));	
		
		m_pdlgRet->SetMsg(strRet);
		m_pdlgRet->ShowWindow(bShow);
	}
}
