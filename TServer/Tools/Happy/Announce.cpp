// 현승룡 Announce.cpp

// Announce.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "Announce.h"
#include ".\announce.h"
#include "MainFrm.h"
#include "HappyDoc.h"

// CAnnounce 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnnounce, CDialog)
CAnnounce::CAnnounce(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnounce::IDD, pParent)
	, m_dwTimerMin(0)
{
}

CAnnounce::~CAnnounce()
{
}

void CAnnounce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboWorld);
	DDX_Control(pDX, IDC_CHECK1, m_checkTimer);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_dwTimerMin);
}


BEGIN_MESSAGE_MAP(CAnnounce, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TIMEREND, OnBnClickedButtonTimerend)
	ON_BN_CLICKED(IDC_BUTTON_TIMER, OnBnClickedButtonTimer)
END_MESSAGE_MAP()


// CAnnounce 메시지 처리기입니다.

BOOL CAnnounce::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_checkTimer.SetCheck(FALSE);
	m_strAnnounce = _T("");
	GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
	m_bTimer = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnnounce::SetWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Total"));//전체
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);

    m_comboWorld.SetCurSel(0);
}

void CAnnounce::OnBnClickedOk()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	GetDlgItemText(IDC_EDIT1, m_strAnnounce);
	

	CString str = m_strAnnounce;
	str.Trim(_T(" \n\r"));
	if(str == _T(""))
	{
		AfxMessageBox(_T("You should enter the notice"));
		return;
	}

	if( !m_comboWorld.GetCurSel() )
		m_nID = 0;
	else
	{
		m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);

		MAPGROUP::iterator itG;
		for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
			if((*itG).second->m_strName == str)
				break;
			
		m_nID = (*itG).second->m_bID;
	}

	if(m_checkTimer.GetCheck())
	{
		UpdateData();
		DWORD dwTimer = m_bTimer ? m_dwTimerMin*1000 : m_dwTimerMin*60000;
		if(dwTimer > 3600000)
		{
			AfxMessageBox(_T("Control to less than 60 mins"));//60분이내로 조절해 주세요
			return;
		}

		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow();

		SetTimer(1, dwTimer, NULL);
	}
	else
		SetDlgItemText(IDC_EDIT1, _T(""));
	pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		
	//OnOK();
}

void CAnnounce::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(GetDlgItem(IDC_BUTTON_TIMEREND)->IsWindowEnabled())
	{
		AfxMessageBox(_T("Close the timer"));//Timer를 종료하세요
		return;
	}
	m_checkTimer.SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_TIMER, _T("0"));
	OnCancel();
}

void CAnnounce::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
			if(!pDoc)
			{
				pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
				if(!pDoc)
				{
					KillTimer(1);
					GetDlgItem(IDC_EDIT1)->EnableWindow();
					GetDlgItem(IDC_CHECK1)->EnableWindow();
					GetDlgItem(IDC_COMBO1)->EnableWindow();
					GetDlgItem(IDC_EDIT_TIMER)->EnableWindow();
					GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow();
					GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
					SetDlgItemText(IDC_EDIT1, _T(""));
					break;
				}
			}
			pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CAnnounce::OnBnClickedButtonTimerend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(1);
	GetDlgItem(IDC_EDIT1)->EnableWindow();
	GetDlgItem(IDC_CHECK1)->EnableWindow();
	GetDlgItem(IDC_COMBO1)->EnableWindow();
	GetDlgItem(IDC_EDIT_TIMER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT1, _T(""));
}

void CAnnounce::OnBnClickedButtonTimer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_bTimer)
		SetDlgItemText(IDC_BUTTON_TIMER, _T("Sec"));
	else
		SetDlgItemText(IDC_BUTTON_TIMER, _T("Min"));

	m_bTimer = !m_bTimer;
}
