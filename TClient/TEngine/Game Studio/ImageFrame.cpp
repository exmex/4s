// ImageFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "ImageFrame.h"
#include "MainFrm.h"


// CImageFrame

IMPLEMENT_DYNCREATE(CImageFrame, CMDIChildWnd)

CImageFrame::CImageFrame()
{
}

CImageFrame::~CImageFrame()
{
}


BEGIN_MESSAGE_MAP(CImageFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CImageFrame 메시지 처리기입니다.

BOOL CImageFrame::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	BOOL bResult = CMDIChildWnd::Create(
		lpszClassName,
		lpszWindowName,
		dwStyle,
		rect,
		pParentWnd,
		pContext);
	ShowWindow(SW_SHOWMAXIMIZED);

	return bResult;
}

int CImageFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndControlBar.Create( this, &m_wndImageBar, CBRS_RIGHT, IDD_IMAGE))
		return -1;

	if(!m_wndPlayCtrlBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndPlayCtrlBar.GetPlayCtrl()->InitHandler(
		&m_wndImageBar,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);

	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_pHandler = &m_wndImageBar;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwMoveID = WM_MARKER_MOVE;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwCopyID = WM_MARKER_COPY;
	m_wndPlayCtrlBar.GetPlayCtrl()->SetPos(0);

	m_wndPlayCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPlayCtrlBar);
	DockControlBar(&m_wndControlBar);

	return 0;
}

void CImageFrame::OnSetFocus(CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_MAINFRAME);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}
