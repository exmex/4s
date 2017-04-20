// COMPFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "COMPFrame.h"


// CCOMPFrame

IMPLEMENT_DYNCREATE(CCOMPFrame, CGuideFrame)

CCOMPFrame::CCOMPFrame()
{
}

CCOMPFrame::~CCOMPFrame()
{
}


BEGIN_MESSAGE_MAP(CCOMPFrame, CGuideFrame)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CCOMPFrame 메시지 처리기입니다.

int CCOMPFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuideFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndControlBar.Create( this, &m_wndCOMPBar, CBRS_RIGHT, IDD_CONTROL))
		return -1;

	m_wndControlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndControlBar);

	return 0;
}
