// AnnounceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "AnnounceDlg.h"
#include ".\announcedlg.h"

#include "GMToolDoc.h"

// CAnnounceDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnnounceDlg, CDialog)
CAnnounceDlg::CAnnounceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnounceDlg::IDD, pParent)
	, m_dwTimerMin(0)
{
}

CAnnounceDlg::~CAnnounceDlg()
{
}

void CAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_TIMER, m_chkTimer);
	DDX_Control(pDX, IDC_COMBO_NOTICEWORLD, m_comboWorld);
}


BEGIN_MESSAGE_MAP(CAnnounceDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_TIMESELECT, OnBnClickedBtnTimeSelect)
	ON_BN_CLICKED(IDC_BTN_TIMEREND, OnBnClickedBtnTimerEnd)	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAnnounceDlg 메시지 처리기입니다.


BOOL CAnnounceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_chkTimer.SetCheck(FALSE);
	m_strAnnounce = _T("");
	GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
	m_bTimer = 0;

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);// 모달리스 대화상자가 윈도우 태스크바에 안 뜨게 하기
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnnounceDlg::SetWorld()
{
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Total"));//전체
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);

    m_comboWorld.SetCurSel(0);
}


void CAnnounceDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	GetDlgItemText(IDC_EDIT_NOTICE, m_strAnnounce);	

	CString str = m_strAnnounce;
	str.Trim(_T(" \n\r"));
	if(str == _T(""))
	{
		MessageBox(_T("You should enter the notice"));
		GetDlgItem(IDC_EDIT_NOTICE)->SetFocus();
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

	if(m_chkTimer.GetCheck())
	{
		m_dwTimerMin = GetDlgItemInt(IDC_EDIT_TIME);
		DWORD dwTimer = m_bTimer ? m_dwTimerMin*1000 : m_dwTimerMin*60000;
		if(dwTimer > 3600000)
		{
			AfxMessageBox(_T("Control to less than 60 mins"));//60분이내로 조절해 주세요
			return;
		}

		GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow();

		SetTimer(1, dwTimer, NULL);
	}
	else
		SetDlgItemText(IDC_EDIT1, _T(""));

	pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);

	//OnOK();
}

void CAnnounceDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(GetDlgItem(IDC_BTN_TIMEREND)->IsWindowEnabled())
	{
		AfxMessageBox(_T("Close the timer"));//Timer를 종료하세요
		return;
	}
	m_chkTimer.SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_TIME, _T("0"));

	OnCancel();
}

void CAnnounceDlg::OnBnClickedBtnTimerEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(1);
	GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow();
	GetDlgItem(IDC_CHK_TIMER)->EnableWindow();
	GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow();
	GetDlgItem(IDC_EDIT_TIME)->EnableWindow();
	GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow();
	GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_NOTICE, _T(""));
}

void CAnnounceDlg::OnBnClickedBtnTimeSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_bTimer)
		SetDlgItemText(IDC_BTN_TIMESELECT, _T("Sec"));
	else
		SetDlgItemText(IDC_BTN_TIMESELECT, _T("Min"));

	m_bTimer = !m_bTimer;
}



void CAnnounceDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch(nIDEvent)
	{
	case 1:
		{
			CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
			if(!pDoc)
			{
				KillTimer(1);
					GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow();
					GetDlgItem(IDC_CHK_TIMER)->EnableWindow();
					GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow();
					GetDlgItem(IDC_EDIT_TIME)->EnableWindow();
					GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow();
					GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
					SetDlgItemText(IDC_EDIT_NOTICE, _T(""));
					break;
			}
			
			pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
