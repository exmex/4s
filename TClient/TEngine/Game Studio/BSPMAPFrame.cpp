// BSPMAPFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BSPMAPFrame.h"


// CBSPMAPFrame

IMPLEMENT_DYNCREATE(CBSPMAPFrame, CMAPFrame)

CBSPMAPFrame::CBSPMAPFrame()
{
}

CBSPMAPFrame::~CBSPMAPFrame()
{
}


BEGIN_MESSAGE_MAP(CBSPMAPFrame, CMAPFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MAP_BACK, OnMapBack)
END_MESSAGE_MAP()


// CBSPMAPFrame 메시지 처리기입니다.

int CBSPMAPFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMAPFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndControlBar.Create( this, &m_wndBSPMAPBar, CBRS_RIGHT, IDD_BSPMAP))
		return -1;

	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndControlBar);

	return 0;
}

void CBSPMAPFrame::OnMapBack()
{
	m_wndMAPLIGHTBar.ShowWindow(SW_HIDE);
	m_wndMAPOBJBar.ShowWindow(SW_HIDE);
	m_wndMAPSFXBar.ShowWindow(SW_HIDE);
	m_wndMAPSNDBar.ShowWindow(SW_HIDE);
	m_wndControlBar.ShowWindow(SW_SHOW);
	DockControlBar(&m_wndControlBar);
	RepositionBars( 0, 0xFFFFFFFF, 0);
}
