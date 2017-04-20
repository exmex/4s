// SFXFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SFXFrame.h"


// CSFXFrame

IMPLEMENT_DYNCREATE(CSFXFrame, CGridFrame)

CSFXFrame::CSFXFrame()
{
}

CSFXFrame::~CSFXFrame()
{
}


BEGIN_MESSAGE_MAP(CSFXFrame, CGridFrame)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSFXFrame 메시지 처리기입니다.

int CSFXFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGridFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableDocking(CBRS_ALIGN_ANY);

	if(!m_wndParticleBar.Create( this, &m_wndPSFXBar, CBRS_RIGHT, IDD_PARTICLE))
		return -1;

	if(!m_wndControlBar.Create( this, &m_wndSFXBar, CBRS_RIGHT, IDD_SFX))
		return -1;

	if(!m_wndPlayCtrlBar.Create( this, CBRS_BOTTOM))
		return -1;

	m_wndPlayCtrlBar.GetPlayCtrl()->InitHandler(
		&m_wndSFXBar,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);
	m_wndParticleBar.ShowWindow(SW_HIDE);

	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_pHandler = &m_wndSFXBar;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwMoveID = WM_MARKER_MOVE;
	m_wndPlayCtrlBar.GetPlayCtrl()->m_cMarker.m_dwCopyID = WM_MARKER_COPY;
	m_wndPlayCtrlBar.GetPlayCtrl()->SetPos(0);

	m_wndPlayCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndParticleBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPlayCtrlBar);
	DockControlBar(&m_wndControlBar);

	return 0;
}
