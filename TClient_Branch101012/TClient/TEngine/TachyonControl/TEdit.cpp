#include "stdafx.h"
#include "TEdit.h"
#include "TPaintManager.h"
#include "TMemoryDC.h"
#include "TPopupMenuWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CWinApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CTEdit

CTEdit::CTEdit()
{
	m_bMouseOver = FALSE;
}

CTEdit::~CTEdit()
{
}

BEGIN_MESSAGE_MAP(CTEdit, CEdit)
	//{{AFX_MSG_MAP(CTEdit)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTEdit message handlers

void CTEdit::OnMouseMove( UINT nFlags, CPoint point)
{
	SetTimer( 1, 50, NULL);
	CEdit::OnMouseMove( nFlags, point);
}

void CTEdit::OnTimer( UINT nIDEvent)
{
	if( nIDEvent != 1 )
	{
		CEdit::OnTimer(nIDEvent);
		return;
	}

	POINT point;
	::GetCursorPos(&point);

	CRect rcItem;
	GetWindowRect(&rcItem);

	BOOL bOldMouseOver = m_bMouseOver;
	if(!rcItem.PtInRect(point))
	{
		KillTimer(1);
		m_bMouseOver = FALSE;
	}
	else
		m_bMouseOver = TRUE;

	if( bOldMouseOver != m_bMouseOver )
		_PostRedraw();
}

void CTEdit::OnPaint()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CPaintDC dcPaint(this);
	CTMemoryDC dcmm( &dcPaint, &rcClient);
	DefWindowProc( WM_PAINT, (WPARAM) dcmm.GetSafeHdc(), (LPARAM) 0);
}

void CTEdit::_PostRedraw()
{
	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE|
		RDW_UPDATENOW|
		RDW_ERASE|
		RDW_ERASENOW|
		RDW_FRAME|
		RDW_ALLCHILDREN);
}

void CTEdit::_DrawEditImpl( CRect rcClient, CDC *pDC)
{
	BOOL bNeedReleaseDC = FALSE;

	if( pDC == NULL )
	{
		pDC = GetDC();
		bNeedReleaseDC = TRUE;
	}

	rcClient.DeflateRect( 1, 1);
	BOOL bFocus = FALSE;

	if(	GetFocus()->GetSafeHwnd() == m_hWnd && !CTPopupMenuWnd::IsMenuTracking() )
		bFocus = TRUE;

	g_PaintManager->PaintControlFrame(
		*pDC,
		rcClient,
		TRUE,
		m_bMouseOver,
		IsWindowEnabled(),
		bFocus);

	if(bNeedReleaseDC)
		ReleaseDC(pDC);
}

void CTEdit::SetIndent( int margin)
{
	SetMargins( margin, 1);
}

void CTEdit::OnSetFocus( CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	m_bMouseOver = TRUE;
	_PostRedraw();
}

void CTEdit::OnKillFocus( CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	m_bMouseOver = FALSE;
	_PostRedraw();
}

BOOL CTEdit::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTEdit::OnNcPaint()
{
	CWindowDC dc(this);
	CRect rcClient;
	CRect rcBar;

	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);

	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());
	dc.ExcludeClipRect(rcClient);

	CTMemoryDC dcmm( &dc, &rcBar);
	rcBar.DeflateRect( 3, 3);
	_DrawEditImpl( rcBar, &dcmm);
}

void CTEdit::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	CEdit::OnNcCalcSize( bCalcValidRects, lpncsp);
	CRect rect(lpncsp->rgrc[0]);

	rect.DeflateRect( 4, 4);
	lpncsp->rgrc[0] = rect;
}
