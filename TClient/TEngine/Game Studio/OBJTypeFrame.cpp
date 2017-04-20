// OBJTypeFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "OBJTypeFrame.h"
#include "MainFrm.h"


// COBJTypeFrame

IMPLEMENT_DYNCREATE(COBJTypeFrame, CMDIChildWnd)

COBJTypeFrame::COBJTypeFrame()
{
}

COBJTypeFrame::~COBJTypeFrame()
{
}


BEGIN_MESSAGE_MAP(COBJTypeFrame, CMDIChildWnd)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// COBJTypeFrame 메시지 처리기입니다.

BOOL COBJTypeFrame::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
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

void COBJTypeFrame::OnSetFocus( CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_MAINFRAME);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}
