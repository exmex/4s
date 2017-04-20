// GSDialogBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSDialogBar.h"


HCURSOR CGSDialogBar::m_hHand = NULL;

// CGSDialogBar

IMPLEMENT_DYNAMIC( CGSDialogBar, CControlBar)
CGSDialogBar::CGSDialogBar()
{
	m_szDefault = CSize( 0, 0);
	m_szContent = CSize( 0, 0);

	m_pContent = NULL;
	m_bScroll = FALSE;
}

CGSDialogBar::~CGSDialogBar()
{
}

BEGIN_MESSAGE_MAP( CGSDialogBar, CControlBar)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


void CGSDialogBar::ReleaseGSDialogBar()
{
	if(m_hHand)
	{
		DestroyCursor(m_hHand);
		m_hHand = NULL;
	}
}

void CGSDialogBar::InitGSDialogBar()
{
	ReleaseGSDialogBar();
	m_hHand = AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);
}


// CGSDialogBar 메시지 처리기입니다.

CSize CGSDialogBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz)
{
	static const DWORD dwDockBarID[2] = {
		AFX_IDW_DOCKBAR_TOP,
		AFX_IDW_DOCKBAR_BOTTOM};

	CFrameWnd *pFrame = GetParentFrame();
	CRect rect;

	pFrame->GetClientRect(&rect);
	for( int i=0; i<2; i++)
	{
		CControlBar *pDockBar = pFrame->GetControlBar(dwDockBarID[i]);
		CRect rcDock;

		if( pDockBar && (DWORD_PTR) m_pDockBar != (DWORD_PTR) pDockBar )
		{
			pDockBar->GetWindowRect(&rcDock);
			rect.top += rcDock.Height();
		}
	}

	if(!IsFloating())
		return CSize( bHorz ? rect.Width() : m_szDefault.cx, bHorz ? m_szDefault.cy : rect.Height());
	else
		return m_szDefault;
}

void CGSDialogBar::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

BOOL CGSDialogBar::Create( CWnd* pParentWnd, CDialog *pContent, DWORD dwStyle, UINT nTemplateID)
{
	m_dwStyle = dwStyle & CBRS_ALIGN_ANY;
	CRect rect;

	BOOL bResult = CControlBar::Create(
		NULL, NULL,
		WS_VISIBLE|WS_CHILD|dwStyle,
		CRect(
		0, 0,
		10, 10),
		pParentWnd,
		nTemplateID);

	pContent->Create( nTemplateID, this);
	pContent->UpdateWindow();
	pContent->ShowWindow(SW_SHOW);
	pContent->GetWindowRect(&rect);

	m_szDefault = rect.Size();
	m_szContent = rect.Size();
	m_pContent = pContent;

	return bResult;
}

void CGSDialogBar::OnSize( UINT nType, int cx, int cy)
{
	CControlBar::OnSize( nType, cx, cy);

	if(m_pContent)
	{
		CRect rect;

		m_pContent->GetWindowRect(&rect);
		ScreenToClient(&rect);

		if( cx > rect.right && rect.left < 0 )
			rect.OffsetRect( min( -rect.left, cx - rect.right), 0);

		if( cy > rect.bottom && rect.top < 0 )
			rect.OffsetRect( 0, min( -rect.top, cy - rect.bottom));

		rect.bottom = rect.top + m_szContent.cy;
		rect.right = rect.left + m_szContent.cx;

		if( cx > rect.right )
			rect.right = cx;

		if( cy > rect.bottom )
			rect.bottom = cy;

		m_pContent->MoveWindow(&rect);
	}
}

BOOL CGSDialogBar::CanScroll()
{
	CRect rcContent;
	CRect rcClient;

	if(!m_pContent)
		return FALSE;

	m_pContent->GetWindowRect(&rcContent);
	ScreenToClient(&rcContent);
	GetClientRect(&rcClient);
	rcClient.IntersectRect( &rcContent, &rcClient);

	return rcContent != rcClient;
}

void CGSDialogBar::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(CanScroll())
	{
		m_bScroll = TRUE;
		m_prev = point;
		SetCapture();
	}

	CWnd::OnLButtonDown( nFlags, point);
}

void CGSDialogBar::OnMouseMove( UINT nFlags, CPoint point)
{
	if(m_pContent)
	{
		CRect rcContent;
		CRect rcClient;
		CPoint scroll;

		m_pContent->GetWindowRect(&rcContent);
		ScreenToClient(&rcContent);
		GetClientRect(&rcClient);

		if( point.x > m_prev.x )
			scroll.x = min( point.x - m_prev.x, -rcContent.left);
		else
			scroll.x = max( point.x - m_prev.x, rcClient.right - rcContent.right);

		if( point.y > m_prev.y )
			scroll.y = min( point.y - m_prev.y, -rcContent.top);
		else
			scroll.y = max( point.y - m_prev.y, rcClient.bottom - rcContent.bottom);

		if( scroll != CPoint( 0, 0) )
			ScrollWindow( scroll.x, scroll.y);

		m_prev = point;
	}

	CWnd::OnMouseMove( nFlags, point);
}

void CGSDialogBar::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_bScroll)
	{
		m_bScroll = FALSE;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp( nFlags, point);
}

BOOL CGSDialogBar::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( m_bScroll || (
		pWnd == m_pContent &&
		nHitTest == HTCLIENT &&
		CanScroll()))
	{
		SetCursor(m_hHand);
		return TRUE;
	}

	return CControlBar::OnSetCursor( pWnd, nHitTest, message);
}
