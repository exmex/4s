// OBJFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "OBJFrame.h"
#include "MainFrm.h"


// COBJFrame

IMPLEMENT_DYNCREATE(COBJFrame, CGridFrame)

COBJFrame::COBJFrame()
{
}

COBJFrame::~COBJFrame()
{
}


BEGIN_MESSAGE_MAP(COBJFrame, CGridFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// COBJFrame 메시지 처리기입니다.

int COBJFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndControlBar.Create( this, &m_wndOBJBar, CBRS_RIGHT, IDD_OBJECT))
		return -1;

	if(!m_wndPlayCtrlBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndPlayCtrlBar.GetPlayCtrl()->InitHandler(
		&m_wndOBJBar,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);

	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_pHandler = &m_wndOBJBar;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwMoveID = WM_MARKER_MOVE;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwCopyID = WM_MARKER_COPY;
	m_wndPlayCtrlBar.GetPlayCtrl()->SetPos(0);

	m_wndPlayCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPlayCtrlBar);
	DockControlBar(&m_wndControlBar);

	return 0;
}

void COBJFrame::OnSetFocus( CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_OBJECT);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}
