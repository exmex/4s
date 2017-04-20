// DlgServiceLog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "MainFrm.h"
#include "DlgServiceLog.h"
#include ".\dlgservicelog.h"


// CDlgServiceLog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgServiceLog, CDialog)
CDlgServiceLog::CDlgServiceLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServiceLog::IDD, pParent)
{
	m_bStat = 0;
	m_bMin = 0;
}

CDlgServiceLog::~CDlgServiceLog()
{
	m_bStat = 0;
	m_bMin = 0;
}

void CDlgServiceLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboTime);
}


BEGIN_MESSAGE_MAP(CDlgServiceLog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgServiceLog 메시지 처리기입니다.

BOOL CDlgServiceLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bStat)
		SetDlgItemText(IDC_STATIC, _T("Log Recording..."));//로그를 기록 중 입니다.
	else
		SetDlgItemText(IDC_STATIC, _T(""));

	CString str = _T("now");
	m_comboTime.AddString(str);
	for(BYTE i=1; i<=60; i++)
	{
		str.Format(_T("%2dMin"), i);
		m_comboTime.AddString(str);
	}	

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(pDoc)
			m_bMin = pDoc->m_bLogTime;
	}

	m_comboTime.SetCurSel(m_bMin);
	if(m_bMin)
		m_comboTime.EnableWindow(FALSE);

	if(m_bStat)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);;
	}
	else
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgServiceLog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bMin = m_comboTime.GetCurSel();
	if(m_bMin == 0)
		m_bStat = 0;
	else
        m_bStat ? m_bStat = 3 : m_bStat = 1;	

	OnOK();
}

void CDlgServiceLog::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	!m_bStat ? m_bStat = 3 : m_bStat = 0;
	OnOK();
}
