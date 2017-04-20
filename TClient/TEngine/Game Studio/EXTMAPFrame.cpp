// EXTMAPFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "EXTMAPFrame.h"


// CEXTMAPFrame

IMPLEMENT_DYNCREATE(CEXTMAPFrame, CMAPFrame)

CEXTMAPFrame::CEXTMAPFrame()
{
}

CEXTMAPFrame::~CEXTMAPFrame()
{
}


BEGIN_MESSAGE_MAP(CEXTMAPFrame, CMAPFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MAP_BACK, OnMapBack)
END_MESSAGE_MAP()


// CEXTMAPFrame 메시지 처리기입니다.

int CEXTMAPFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMAPFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndControlBar.Create( this, &m_wndEXTMAPBar, CBRS_RIGHT, IDD_EXTMAP))
		return -1;

	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndControlBar);

	return 0;
}

void CEXTMAPFrame::OnMapBack()
{
	m_wndMAPLIGHTBar.ShowWindow(SW_HIDE);
	m_wndMAPOBJBar.ShowWindow(SW_HIDE);
	m_wndMAPSFXBar.ShowWindow(SW_HIDE);
	m_wndMAPSNDBar.ShowWindow(SW_HIDE);
	m_wndControlBar.ShowWindow(SW_SHOW);
	DockControlBar(&m_wndControlBar);
	RepositionBars( 0, 0xFFFFFFFF, 0);
}
