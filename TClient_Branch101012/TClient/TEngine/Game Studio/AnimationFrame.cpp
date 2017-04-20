// AnimationFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AnimationFrame.h"


// CAnimationFrame

IMPLEMENT_DYNCREATE(CAnimationFrame, CGridFrame)

CAnimationFrame::CAnimationFrame()
{
}

CAnimationFrame::~CAnimationFrame()
{
}


BEGIN_MESSAGE_MAP(CAnimationFrame, CGridFrame)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CAnimationFrame 메시지 처리기입니다.

int CAnimationFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndControlBar.Create( this, &m_wndAnimationBar, CBRS_RIGHT, IDD_ANIMATION))
		return -1;

	if(!m_wndPlayCtrlBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndPlayCtrlBar.GetPlayCtrl()->InitHandler(
		&m_wndAnimationBar,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);

	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_pHandler = &m_wndAnimationBar;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwMoveID = WM_MARKER_MOVE;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwCopyID = WM_MARKER_COPY;
	m_wndPlayCtrlBar.GetPlayCtrl()->SetPos(0);

	m_wndPlayCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPlayCtrlBar);
	DockControlBar(&m_wndControlBar);

	return 0;
}
