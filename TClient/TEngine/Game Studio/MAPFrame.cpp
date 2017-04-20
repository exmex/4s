// MAPFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "MAPFrame.h"


// CMAPFrame

IMPLEMENT_DYNCREATE( CMAPFrame, CGridFrame)

CMAPFrame::CMAPFrame()
{
}

CMAPFrame::~CMAPFrame()
{
}


BEGIN_MESSAGE_MAP( CMAPFrame, CGridFrame)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMAPFrame 메시지 처리기입니다.

int CMAPFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndMAPLIGHTBar.Create( this, &m_wndMAPLIGHTDlg, CBRS_RIGHT, IDD_MAPLIGHT))
		return -1;

	if(!m_wndMAPOBJBar.Create( this, &m_wndMAPOBJDlg, CBRS_RIGHT, IDD_MAPOBJ))
		return -1;

	if(!m_wndMAPSFXBar.Create( this, &m_wndMAPSFXDlg, CBRS_RIGHT, IDD_MAPSFX))
		return -1;

	if(!m_wndMAPSNDBar.Create( this, &m_wndMAPSNDDlg, CBRS_RIGHT, IDD_MAPSND))
		return -1;

	if(!m_wndPlayCtrlBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndPlayCtrlBar.GetPlayCtrl()->InitHandler(
		&m_wndMAPOBJDlg,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);

	m_wndPlayCtrlBar.GetPlayCtrl()->SetRange( 0, 0);
	m_wndPlayCtrlBar.GetPlayCtrl()->SetPos(0);
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cPlayCtrl.EnableWindow(FALSE);
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.EnableWindow(FALSE);
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.Invalidate(FALSE);
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cLoop.EnableWindow(FALSE);

	m_wndMAPLIGHTBar.ShowWindow(SW_HIDE);
	m_wndMAPOBJBar.ShowWindow(SW_HIDE);
	m_wndMAPSFXBar.ShowWindow(SW_HIDE);
	m_wndMAPSNDBar.ShowWindow(SW_HIDE);

	m_wndPlayCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMAPLIGHTBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMAPOBJBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMAPSFXBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMAPSNDBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPlayCtrlBar);

	return 0;
}

void CMAPFrame::OnSetFocus( CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_MAPTYPE);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}
