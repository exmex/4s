// MediaFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MediaFrame.h"
#include "MainFrm.h"


// CMediaFrame

IMPLEMENT_DYNCREATE(CMediaFrame, CMDIChildWnd)

CMediaFrame::CMediaFrame()
{
}

CMediaFrame::~CMediaFrame()
{
}


BEGIN_MESSAGE_MAP(CMediaFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CMediaFrame 메시지 처리기입니다.

BOOL CMediaFrame::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
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

int CMediaFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndMediaBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndMediaBar.GetPlayCtrl()->InitHandler(
		m_wndMediaBar.m_pContent,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);
	m_wndMediaBar.GetPlayCtrl()->SetRange( 0, 100);
	m_wndMediaBar.GetPlayCtrl()->SetPos(0);

	m_wndMediaBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMediaBar);

	return 0;
}

void CMediaFrame::OnSetFocus(CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_MAINFRAME);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}
