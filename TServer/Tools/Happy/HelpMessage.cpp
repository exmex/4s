// CHelpMessage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "MainFrm.h"
#include "HelpMessage.h"
#include ".\helpmessage.h"


// CHelpMessage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHelpMessage, CDialog)
CHelpMessage::CHelpMessage(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpMessage::IDD, pParent)
{
}

CHelpMessage::~CHelpMessage()
{
}

void CHelpMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_HWORLD, m_cbWorld);
	DDX_Control(pDX, IDC_CB_HTYPE, m_cbType);
	DDX_Control(pDX, IDC_DT_HSTART, m_dtStart);
	DDX_Control(pDX, IDC_DT_HEND, m_dtEnd);
}


BEGIN_MESSAGE_MAP(CHelpMessage, CDialog)
	ON_BN_CLICKED(IDC_BTN_HSEND, OnBnClickedBtnHsend)
	ON_CBN_SELCHANGE(IDC_CB_HTYPE, OnCbnSelchangeCbHtype)
END_MESSAGE_MAP()


// CHelpMessage 메시지 처리기입니다.

void CHelpMessage::Init()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	if(!m_cbWorld.GetCount())
	{
		m_cbWorld.AddString(_T("SELECT"));
		m_cbWorld.AddString( STR_WORLD_ALL );

		//	월드서버 리스트 구하기
		MAPGROUP::iterator itG;
		for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		{	
			if( (*itG).second->m_bID != 0 )
				m_cbWorld.AddString((*itG).second->m_strName);
		}
	}
	m_cbWorld.SetCurSel(0);
	
	if(!m_cbType.GetCount())
	{
		//	타입 리스트 초기화
		m_cbType.AddString(_T("SELECT"));
		m_cbType.AddString(_T("Tutorial"));
		m_cbType.AddString(_T("Normal"));
		m_cbType.AddString(_T("Event"));
	}
	m_cbType.SetCurSel(0);

	m_dtStart.SetTime(CTime::GetCurrentTime().GetTime());
	m_dtEnd.SetTime(CTime::GetCurrentTime().GetTime());

	//	텍스트 박스 초기화
	SetDlgItemText(IDC_EB_HTEXT, _T(""));
	SetDlgItemInt(IDC_EB_HSTART_HOUR, 0, FALSE);
	SetDlgItemInt(IDC_EB_HSTART_MIN, 0, FALSE);
	SetDlgItemInt(IDC_EB_HEND_HOUR, 0, FALSE);
	SetDlgItemInt(IDC_EB_HEND_MIN, 0, FALSE);

	SetShow(FALSE);


}

void CHelpMessage::SetShow(BYTE bShow)
{
	GetDlgItem(IDC_GROUP_PR)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_PR)->ShowWindow(bShow);

	GetDlgItem(IDC_EB_HSTART_HOUR)->ShowWindow(bShow);
	GetDlgItem(IDC_EB_HSTART_MIN)->ShowWindow(bShow);
	GetDlgItem(IDC_EB_HEND_HOUR)->ShowWindow(bShow);
	GetDlgItem(IDC_EB_HEND_MIN)->ShowWindow(bShow);
	
	m_dtStart.ShowWindow(bShow);
	m_dtEnd.ShowWindow(bShow);
}

BYTE CHelpMessage::GetWorld(BYTE& p_bOut)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return false;
	}

	CString str = _T("");
	m_cbWorld.GetLBText(m_cbWorld.GetCurSel(),str);

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
	{
		if((*itG).second->m_strName == str)
			break;
		if( str == STR_WORLD_ALL )
		{
			p_bOut = 0;
			return true;
		}
	}

	if( itG == pDoc->m_mapGroup.end() )
		return false;

	p_bOut = (*itG).second->m_bID;

	return true;
}

void CHelpMessage::OnBnClickedBtnHsend()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	CDialog::OnInitDialog();

	BYTE bGroupID = 0;
    HELPMSG stHELPMSG;
	stHELPMSG.Reset();
	
	if( !GetWorld(bGroupID) )
	{
		AfxMessageBox(_T("Not found server"));
		return;
	}

	stHELPMSG.m_bType = m_cbType.GetCurSel();

	//	타입 확인 ( 1:Tutorial 2:Normal 3:Event )
	if( !stHELPMSG.m_bType || 3 < stHELPMSG.m_bType )
	{
		AfxMessageBox(_T("Select Type"));
		return;
	}

    //	텍스트 얻기
	GetDlgItemText(IDC_EB_HTEXT, stHELPMSG.m_strMsg);

	//	Event만 날짜얻기
	if(stHELPMSG.m_bType == 3)
	{
		BYTE bStartHour, bStartMinute, bEndHour, bEndMinute;

		bStartHour = (BYTE)GetDlgItemInt(IDC_EB_HSTART_HOUR);
		bStartMinute = (BYTE)GetDlgItemInt(IDC_EB_HSTART_MIN);
		bEndHour = (BYTE)GetDlgItemInt(IDC_EB_HEND_HOUR);
		bEndMinute = (BYTE)GetDlgItemInt(IDC_EB_HEND_MIN);

		if(bStartHour > 23 || bStartMinute > 59 || bEndHour > 23 || bEndMinute > 59)
		{
			AfxMessageBox(_T("Enter the correct Time"));
			return;
		}

		CTime t;
		m_dtStart.GetTime(t);
		CTime ts(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute,0);
		stHELPMSG.m_dStartDate = ts.GetTime();

		m_dtEnd.GetTime(t);
		CTime te(t.GetYear(),t.GetMonth(),t.GetDay(),bEndHour,bEndMinute,0);
		stHELPMSG.m_dEndDate = te.GetTime();

		//if( (stHELPMSG.m_dEndDate < stHELPMSG.m_dStartDate) || (stHELPMSG.m_dEndDate <= CTime::GetCurrentTime().GetTime()))
		//{
		//	AfxMessageBox(_T("Enter the correct Date"));
		//	return;			
		//}
	}
	else
	{
		stHELPMSG.m_dStartDate = 0;
		stHELPMSG.m_dEndDate = 0;
	}

	//Send
	pDoc->SendCT_HELPMESSAGE_REQ(bGroupID, &stHELPMSG);

	SetDlgItemText(IDC_EB_HTEXT, _T(""));

	AfxMessageBox(_T("Help Message send success"));
}

void CHelpMessage::OnCbnSelchangeCbHtype()
{
	BYTE bType = m_cbType.GetCurSel();

	//	타입 확인 ( 1:Tutorial 2:Normal 3:Event )
	if(bType == 3)
		SetShow(TRUE);
	else
		SetShow(FALSE);
}
