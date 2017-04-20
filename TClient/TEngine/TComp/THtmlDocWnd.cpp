// THtmlDocWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "THtmlDocWnd.h"
#include ".\thtmldocwnd.h"

#undef SubclassWindow

TASM_CODE THtmlDocWnd::m_vTAPI[TPAINT_API_COUNT];

// THtmlDocWnd

IMPLEMENT_DYNAMIC(THtmlDocWnd, CWnd)
THtmlDocWnd::THtmlDocWnd()
{
	m_pMAINPROC = NULL;
	m_pWINPROC = NULL;
	m_pMainWnd = NULL;
	m_pTHOST = NULL;

	m_bReady = FALSE;
}

THtmlDocWnd::~THtmlDocWnd()
{
}


BEGIN_MESSAGE_MAP(THtmlDocWnd, CWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

BYTE THtmlDocWnd::InitHook()
{
	HMODULE hMODULE = GetModuleHandle(TUSER32_DLL);
	ReleaseHook();

	if(!hMODULE)
		return FALSE;

	m_vTAPI[TPAINT_API_BEGINPAINT].m_pTAPI = (LPBYTE) GetProcAddress(
		hMODULE,
		TUSER32_BEGINPAINT);

	if(!m_vTAPI[TPAINT_API_BEGINPAINT].m_pTAPI)
	{
		ReleaseHook();
		return FALSE;
	}

	m_vTAPI[TPAINT_API_ENDPAIND].m_pTAPI = (LPBYTE) GetProcAddress(
		hMODULE,
		TUSER32_ENDPAINT);

	if(!m_vTAPI[TPAINT_API_ENDPAIND].m_pTAPI)
	{
		ReleaseHook();
		return FALSE;
	}

	return TRUE;
}

void THtmlDocWnd::ReleaseHook()
{
	for( int i=0; i<TPAINT_API_COUNT; i++)
	{
		UnhookTAPI(&m_vTAPI[i]);

		ZeroMemory( &m_vTAPI[i].m_vTBUF, TASM_BYTE);
		m_vTAPI[i].m_bTSET = FALSE;
		m_vTAPI[i].m_pTAPI = NULL;
	}
}

BYTE THtmlDocWnd::HookTAPI( LPTASM_CODE pTCODE,
						    LPVOID pTAPI)
{
	if( !pTCODE || pTCODE->m_bTSET || !pTCODE->m_pTAPI || !pTAPI )
		return FALSE;

	DWORD dwNEW = PAGE_EXECUTE_READWRITE;
	DWORD dwOLD = 0;

	VirtualProtect(
		pTCODE->m_pTAPI,
		TASM_BYTE,
		dwNEW,
		&dwOLD);
	memcpy( pTCODE->m_vTBUF, pTCODE->m_pTAPI, TASM_BYTE);

	*((DWORD_PTR *) &pTCODE->m_pTAPI[1]) = DWORD_PTR(pTAPI) - (DWORD_PTR(pTCODE->m_pTAPI) + TASM_BYTE);
	pTCODE->m_pTAPI[0] = TASM_JUMP;
	pTCODE->m_bTSET = TRUE;

	VirtualProtect(
		pTCODE->m_pTAPI,
		TASM_BYTE,
		dwOLD,
		&dwNEW);

	return TRUE;
}

void THtmlDocWnd::UnhookTAPI( LPTASM_CODE pTCODE)
{
	if( !pTCODE || !pTCODE->m_bTSET || !pTCODE->m_pTAPI )
		return;

	DWORD dwNEW = PAGE_EXECUTE_READWRITE;
	DWORD dwOLD = 0;

	VirtualProtect(
		pTCODE->m_pTAPI,
		TASM_BYTE,
		dwNEW,
		&dwOLD);

	memcpy( pTCODE->m_pTAPI, pTCODE->m_vTBUF, TASM_BYTE);
	pTCODE->m_bTSET = FALSE;

	VirtualProtect(
		pTCODE->m_pTAPI,
		TASM_BYTE,
		dwOLD,
		&dwNEW);
}

BOOL CALLBACK THtmlDocWnd::__HookDocWnd( HWND hWnd, LPARAM lParam)
{
	CWnd *pWND = FromHandlePermanent(hWnd);

	if( hWnd && !pWND )
	{
		THtmlDocWnd *pWnd = new THtmlDocWnd();

		pWnd->m_pMainWnd = AfxGetMainWnd();
		if(pWnd->m_pMainWnd)
			pWnd->m_pMAINPROC = (DWORD_PTR) GetWindowLongPtr( pWnd->m_pMainWnd->GetSafeHwnd(), GWL_WNDPROC);

		pWnd->m_pWINPROC = (DWORD_PTR) GetWindowLongPtr( hWnd, GWL_WNDPROC);
		pWnd->m_pTHOST = (THttpCtrl *) lParam;
		pWnd->m_bReady = TRUE;
		pWnd->SubclassWindow(hWnd);
	}
	else if( pWND && pWND->IsKindOf(RUNTIME_CLASS(THtmlDocWnd)) )
		((THtmlDocWnd *) pWND)->m_bReady = TRUE;

	return TRUE;
}

BOOL CALLBACK THtmlDocWnd::__PutReady( HWND hWnd, LPARAM lParam)
{
	CWnd *pWND = FromHandlePermanent(hWnd);

	if( pWND && pWND->IsKindOf(RUNTIME_CLASS(THtmlDocWnd)) )
	{
		((THtmlDocWnd *) pWND)->m_bReady = (BYTE) lParam;
		return TRUE;
	}

	return FALSE;
}

BOOL CALLBACK THtmlDocWnd::__RedrawWnd( HWND hWnd, LPARAM lParam)
{
	CWnd *pWND = FromHandlePermanent(hWnd);

	if( pWND && pWND->IsWindowVisible() &&
		!pWND->GetWindowedChildCount() )
	{
		pWND->InvalidateRect(NULL, FALSE);
	}

	return TRUE;
}

BYTE THtmlDocWnd::CanFocus()
{
	return !GetSafeHwnd() ||
		!IsWindowEnabled() ||
		!IsWindowVisible() ||
		!m_pWINPROC ||
		!m_pMainWnd ||
		!m_pMAINPROC ||
		!m_bReady ||
		!m_pTHOST ||
		!m_pTHOST->HaveCaret(NULL) ? FALSE : TRUE;
}

void THtmlDocWnd::OnDestroy()
{
	CWnd::OnDestroy();
	m_pTHOST = NULL;

	UnsubclassWindow();
	delete this;
}

void THtmlDocWnd::OnPaint()
{
	if( !GetWindowedChildCount() )
	{
		DestroyCaret();

		CallWindowProc(
			(WNDPROC) m_pWINPROC,
			GetSafeHwnd(),
			WM_PAINT,
			0, 0);

		THttpCtrl::UpdateTIMGBUF();
	}
	else
		ValidateRect(NULL);
}

void THtmlDocWnd::OnContextMenu( CWnd* /*pWnd*/, CPoint /*point*/)
{
}

BYTE THtmlDocWnd::OnMouseMSG( UINT nMSG,
							  UINT nFlags,
							  CPoint point)
{
	if( !m_pMainWnd || !m_pMAINPROC )
		return FALSE;

	if(m_bReady)
	{
		ClientToScreen(&point);
		m_pMainWnd->ScreenToClient(&point);

		CallWindowProc(
			(WNDPROC) m_pMAINPROC,
			m_pMainWnd->GetSafeHwnd(),
			nMSG,
			(WPARAM) nFlags,
			(LPARAM) MAKELONG( point.x, point.y));
	}

	return TRUE;
}

BOOL THtmlDocWnd::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( !m_bReady || !m_pMainWnd || !m_pMAINPROC )
		return TRUE;

	CallWindowProc(
		(WNDPROC) m_pMAINPROC,
		m_pMainWnd->GetSafeHwnd(),
		WM_SETCURSOR,
		(WPARAM) (pWnd ? pWnd->GetSafeHwnd() : NULL),
		(LPARAM) MAKELONG( nHitTest, message));

	return TRUE;
}

BOOL THtmlDocWnd::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !m_bReady || !m_pMainWnd || !m_pMAINPROC )
		return TRUE;

	CPoint point = pt;
	ScreenToClient(&point);

	CWnd *pWND = ChildWindowFromPoint(
		point,
		CWP_SKIPDISABLED|
		CWP_SKIPINVISIBLE|
		CWP_SKIPTRANSPARENT);

	if( !pWND || pWND == this )
	{
		CallWindowProc(
			(WNDPROC) m_pMAINPROC,
			m_pMainWnd->GetSafeHwnd(),
			WM_MOUSEWHEEL,
			(WPARAM) MAKELONG( nFlags, zDelta),
			(LPARAM) MAKELONG( pt.x, pt.y));
	}

	return TRUE;
}

void THtmlDocWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_LBUTTONDBLCLK, nFlags, point))
		CWnd::OnLButtonDblClk( nFlags, point);
}

void THtmlDocWnd::OnMButtonDblClk( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_MBUTTONDBLCLK, nFlags, point))
		CWnd::OnMButtonDblClk( nFlags, point);
}

void THtmlDocWnd::OnRButtonDblClk( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_RBUTTONDBLCLK, nFlags, point))
		CWnd::OnRButtonDblClk( nFlags, point);
}

void THtmlDocWnd::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_LBUTTONDOWN, nFlags, point))
		CWnd::OnLButtonDown( nFlags, point);
}

void THtmlDocWnd::OnMButtonDown( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_MBUTTONDOWN, nFlags, point))
		CWnd::OnMButtonDown( nFlags, point);
}

void THtmlDocWnd::OnRButtonDown( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_RBUTTONDOWN, nFlags, point))
		CWnd::OnRButtonDown( nFlags, point);
}

void THtmlDocWnd::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_LBUTTONUP, nFlags, point))
		CWnd::OnLButtonUp( nFlags, point);
}

void THtmlDocWnd::OnMButtonUp( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_MBUTTONUP, nFlags, point))
		CWnd::OnMButtonUp( nFlags, point);
}

void THtmlDocWnd::OnRButtonUp( UINT nFlags, CPoint point)
{
	if(!OnMouseMSG( WM_RBUTTONUP, nFlags, point))
		CWnd::OnRButtonUp( nFlags, point);
}

void THtmlDocWnd::OnMouseMove( UINT nFlags, CPoint point)
{
}

void THtmlDocWnd::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	WORD wTCODE = LOWORD(nChar);
	BYTE bHI = HIBYTE(wTCODE);
	BYTE bLO = LOBYTE(wTCODE);

	if( !bHI && bLO < 0x80 )
		CWnd::OnChar( nChar, nRepCnt, nFlags);
	else if(m_pTHOST)
		m_pTHOST->OnChar( nChar, nRepCnt, nFlags);
}
