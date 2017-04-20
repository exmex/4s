#include "stdafx.h"
#include "TMiniDockFrameWnd.h"
#include "TDockBar.h"
#include "TMemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define __REMOVED_NONFIXMODE_STYLES			FWS_SNAPTOBARS

/////////////////////////////////////////////////////////////////////////////
// CTMiniDockFrameWnd

BOOL CTMiniDockFrameWnd::m_bAutoMakeTopmostOnMouseEvent = FALSE;
BOOL CTMiniDockFrameWnd::m_bOnWndPosChgProcessing = FALSE;
BOOL CTMiniDockFrameWnd::m_bInResizingUpdateState = FALSE;

IMPLEMENT_DYNCREATE(CTMiniDockFrameWnd, CMiniDockFrameWnd);

BEGIN_MESSAGE_MAP(CTMiniDockFrameWnd, CMiniDockFrameWnd)
    //{{AFX_MSG_MAP(CTMiniDockFrameWnd)
	ON_WM_NCMOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCRBUTTONDOWN()
    ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTMiniDockFrameWnd::Create( CWnd* pParent, DWORD dwBarStyle)
{
	if(!CMiniDockFrameWnd::Create( pParent, dwBarStyle))
		return FALSE;

	ModifyStyle( MFS_4THICKFRAME, 0);
	ModifyStyleEx( 0, WS_EX_TOOLWINDOW);
	ModifyStyle( WS_SYSMENU, 0);
	ModifyStyle( 0, WS_BORDER);
	SetTimer( 1, 1, NULL);

	EnableToolTips();
	m_wndToolTip.Create(this);
	m_wndToolTip.Activate(TRUE);

	return TRUE;
}

void CTMiniDockFrameWnd::OnNcLButtonUp( UINT nHitTest, CPoint point)
{
	if(_ResizingIsInProgress())
	{
		_ResizingEnd();
		return;
	}

	if(CTControlBar::_DraggingGetBar())
		return;

	CRect rcBarWnd;
	GetWindowRect(&rcBarWnd);

	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	CPoint ptWnd(point + ptDevOffset);

	CTControlBar *pBar = GetTControlBar();
	if( pBar && pBar->m_bNcAreaBtnTracking )
	{
		int nCountOfNcButtons = pBar->NcButtons_GetCount();
		for( int i=0; i<nCountOfNcButtons; i++)
		{
			CTBarNcAreaButton *pBtn = pBar->NcButtons_GetAt(i);

			if(!pBtn->OnQueryVisibility())
				continue;

			if(pBtn->OnNcAreaClicked(ptWnd))
				return;
		}

		return;
	}

	if(m_rcBtnHideBar.PtInRect(ptWnd))
	{
		CControlBar *pBar = GetControlBar();
		if( pBar == NULL )
			return;

		GetParentFrame()->ShowControlBar(
			pBar,
			FALSE,
			FALSE);

		return;
	}

	CMiniDockFrameWnd::OnNcLButtonUp( nHitTest, point);
}

void CTMiniDockFrameWnd::OnNcLButtonDown( UINT nHitTest, CPoint point)
{
	CTControlBar *pControlBar = GetTControlBar();
	CPoint pt(point);
	CRect rcWnd;

	GetWindowRect(&rcWnd);
	pt.Offset( -rcWnd.left, -rcWnd.top);
	if(m_rcBtnHideBar.PtInRect(pt))
		return;

	if( nHitTest == HTCAPTION )
	{
		ActivateTopParent();
		BringWindowToTop();

		if( (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0 )
		{
			CControlBar *pBar = NULL;
			int nPos = 1;

			while( !pBar && nPos < m_wndDockBar.m_arrBars.GetSize() )
				pBar = reinterpret_cast <CTDockBar &>(m_wndDockBar).GetDockedControlBar(nPos++);

			if( !pControlBar || (
				pControlBar &&
				pControlBar->IsFixedMode() &&
				!pControlBar->_IsShowContentWhenDragging()))
			{
				pBar->m_pDockContext->StartDrag(point);
				return;
			}

			int nCyGripper = min(
				pControlBar->m_rcGrip.Width(),
				pControlBar->m_rcGrip.Height());

			pControlBar->_DraggingStart(
				point,
				point - rcWnd.TopLeft() - CPoint( nCyGripper, nCyGripper / 2));

			return;
		}
	}
	else if( nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST )
	{
		ActivateTopParent();

		CControlBar *pBar = NULL;
		int nPos = 1;

		while( !pBar && nPos < m_wndDockBar.m_arrBars.GetSize() )
			pBar = reinterpret_cast <CTDockBar &>(m_wndDockBar).GetDockedControlBar(nPos++);

		if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			CMiniDockFrameWnd::OnNcLButtonDown( nHitTest, point);
		else
		{
			pControlBar = (CTControlBar *) pBar;

			if( !pControlBar->_IsShowContentWhenDragging() && pControlBar->IsFixedMode() )
			{
				pBar->m_pDockContext->StartResize( nHitTest, point);
				return;
			}

			ClientToScreen(&point);
			_ResizingStart( nHitTest, point);
		}

		return;
	}
	else if( nHitTest == HTMINBUTTON || nHitTest == HTMAXBUTTON || nHitTest == HTGROWBOX )
		return;

	CMiniFrameWnd::OnNcLButtonDown( nHitTest, point);
}

void CTMiniDockFrameWnd::OnNcLButtonDblClk( UINT nHitTest, CPoint point)
{
	if( nHitTest == HTCAPTION )
	{
		CRect rcWnd;
		GetWindowRect(&rcWnd);

		CPoint ptTestBtnHideBar(point - rcWnd.TopLeft());
		if(m_rcBtnHideBar.PtInRect(ptTestBtnHideBar))
			return;
	}

	CTControlBar *pControlBar = GetTControlBar();
	if(pControlBar)
	{
		pControlBar->ToggleDocking();
		return;
	}

	CMiniDockFrameWnd::OnNcLButtonDblClk( nHitTest, point);
}

void CTMiniDockFrameWnd::_ResizingStart( UINT nHitTest, const CPoint& point)
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	CTPopupMenuWnd::CancelMenuTracking();

	CTControlBar *pControlBar = GetTControlBar();
	if( !pControlBar->_IsShowContentWhenDragging() && pControlBar->IsFixedMode() )
	{
		m_nResizingMode = HTNOWHERE;
		pControlBar->m_pDockContext->StartResize( nHitTest, point);

		return;
	}

	switch(nHitTest)
	{
	case HTLEFT			:
	case HTRIGHT		:
	case HTTOP			:
	case HTTOPLEFT		:
	case HTTOPRIGHT		:
	case HTBOTTOM		:
	case HTBOTTOMLEFT	:
	case HTBOTTOMRIGHT	: m_nResizingMode = nHitTest; break;
	default				: return;
	}

	if(!pControlBar->IsFixedMode())
		ModifyStyle( __REMOVED_NONFIXMODE_STYLES, 0);

	BringWindowToTop();
	SetCapture();
	GetWindowRect(&m_rcWndResizingStart);

	while( m_nResizingMode != HTNOWHERE )
	{
		::WaitMessage();
		MSG msg;

		while(PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bStop = FALSE;

			switch(msg.message)
			{
			case WM_ACTIVATEAPP		:
			case WM_COMMAND			: bStop = TRUE; break;
			default					:
				if( WM_KEYFIRST <= msg.message && msg.message <= WM_KEYLAST )
					bStop = TRUE;

				break;
			}

			if( !bStop && GetCapture() != this )
				bStop = TRUE;

			if(bStop)
			{
				_ResizingEnd();
				return;
			}

			if(!AfxGetApp()->PumpMessage())
			{
				PostQuitMessage(0);
				return;
			}
		}

		if(CTControlBar::m_bEnableOnIdleCalls)
		{
			LONG lIdle = 0;
			while(AfxGetApp()->OnIdle(lIdle++));
		}
	}
}

void CTMiniDockFrameWnd::_ResizingEnd()
{
	m_ptLastResizing.x = -1;
	m_ptLastResizing.y = -1;

	if( m_nResizingMode == HTNOWHERE )
		return;

	if( GetCapture() == this )
		ReleaseCapture();

	CTControlBar *pControlBar = GetTControlBar();
	if(!pControlBar->IsFixedMode())
		ModifyStyle( 0, __REMOVED_NONFIXMODE_STYLES);

	m_nResizingMode = HTNOWHERE;
}

void CTMiniDockFrameWnd::_ResizingUpdateState( const CPoint &point)
{
	if( m_bInResizingUpdateState || m_ptLastResizing == point )
		return;

	m_ptLastResizing = point;
	m_bInResizingUpdateState = TRUE;

	CPoint pt(point);
	CTControlBar *pControlBar = GetTControlBar();

	CRect rcClient;
	CRect rcClientOrg;
	CRect rcWnd;
	CRect rcWndOrg;

	GetClientRect(&rcClient);
	GetWindowRect(&rcWnd);
	rcClientOrg = rcClient;
	rcWndOrg = rcWnd;

	CSize szDiff = rcWnd.Size() - rcClient.Size();
	CSize szNcDiff(szDiff);

	int nCalcMinHW = pControlBar->_CalcDesiredMinHW();
	int nCalcMinVH = pControlBar->_CalcDesiredMinVH();

	if( szDiff.cx < nCalcMinHW )
		szDiff.cx = nCalcMinHW;

	if( szDiff.cy < nCalcMinVH )
		szDiff.cy = nCalcMinVH;

	UINT nHit = m_nResizingMode;
	BOOL bLeft = FALSE;
	BOOL bTop = FALSE;
	BOOL bHorz = FALSE;
	BOOL bInvTop = FALSE;
	BOOL bInvLeft = FALSE;

	CPoint ptOffsetWnd( 0, 0);
	if(pControlBar->IsFixedMode())
	{
		switch(nHit)
		{
		case HTTOPLEFT		:
		case HTTOPRIGHT		: nHit = HTTOP; break;
		case HTBOTTOMLEFT	:
		case HTBOTTOMRIGHT	: nHit = HTBOTTOM; break;
		}

		switch(nHit)
		{
		case HTTOP			:
			{
				pt.y = rcWndOrg.bottom + rcWndOrg.top - pt.y;
				nHit = HTBOTTOM;
				bInvLeft = TRUE;
				bInvTop = TRUE;
			}

			break;

		case HTLEFT			:
			{
				pt.x = rcWndOrg.right + rcWndOrg.left - pt.x;
				nHit = HTRIGHT;

				bInvLeft = TRUE;
				bInvTop = TRUE;
			}

			break;
		}
	}

	if( bInvLeft || bInvTop || !pControlBar->IsFixedMode() )
	{
		CRect rcWnd(rcWndOrg);
		ScreenToClient(&rcWnd);

		if( bInvLeft || !pControlBar->IsFixedMode() )
			ptOffsetWnd.x = abs(rcWnd.left - rcClientOrg.left);

		if( bInvTop || !pControlBar->IsFixedMode() )
			ptOffsetWnd.y = abs(rcWnd.top - rcClientOrg.top);
	}

	switch(nHit)
	{
	case HTLEFT			:
		{
			bHorz = TRUE;
			rcWnd.left = min( pt.x, rcWnd.right);
			bLeft = TRUE;
		}

		break;

	case HTRIGHT		:
		{
			bHorz = TRUE;
			rcWnd.right = max( pt.x, rcWnd.left);
		}

		break;

	case HTTOP			:
		{
			rcWnd.top = min( pt.y, rcWnd.bottom);
			bTop = TRUE;
		}

		break;

	case HTTOPLEFT		:
		{
			rcWnd.left = min( pt.x, rcWnd.right);
			rcWnd.top = min( pt.y, rcWnd.bottom);
			bLeft = TRUE;
			bTop = TRUE;
		}

		break;

	case HTTOPRIGHT		:
		{
			rcWnd.right = max( pt.x, rcWnd.left);
			rcWnd.top = min( pt.y, rcWnd.bottom);
			bTop = TRUE;
		}

		break;

	case HTBOTTOM		:
		{
			rcWnd.bottom = max( pt.y, rcWnd.top);
		}

		break;

	case HTBOTTOMLEFT	:
		{
			rcWnd.left = min( pt.x, rcWnd.right);
			rcWnd.bottom = max( pt.y, rcWnd.top);
			bLeft = TRUE;
		}

		break;

	case HTBOTTOMRIGHT	:
		{
			rcWnd.right = max( pt.x, rcWnd.left);
			rcWnd.bottom = max( pt.y, rcWnd.top);
		}

		break;

	default: m_bInResizingUpdateState = FALSE; return;
	}

	if( rcWnd.Width() < szDiff.cx )
		rcWnd.right = rcWnd.left + szDiff.cx;

	if( rcWnd.Height() < szDiff.cy )
		rcWnd.bottom = rcWnd.top + szDiff.cy;

	if( rcWnd.right > rcWndOrg.right && bLeft )
		rcWnd.OffsetRect( rcWndOrg.right - rcWnd.right, 0);

	if( rcWnd.bottom > rcWndOrg.bottom && bTop )
		rcWnd.OffsetRect( 0, rcWndOrg.bottom - rcWnd.bottom);

	rcClient.SetRect(
		rcWnd.left - (rcClient.left - rcWndOrg.left),
		rcWnd.top - (rcClient.top - rcWndOrg.top),
		rcWnd.right - (rcClient.right - rcWndOrg.right),
		rcWnd.bottom - (rcClient.bottom - rcWndOrg.bottom));

	if(!pControlBar->IsFixedMode())
	{
		CSize szMinFloated = pControlBar->_CalcDesiredMinFloatedSize();
		CSize size = rcWnd.Size();

		if( size.cx < szMinFloated.cx )
			size.cx = szMinFloated.cx;

		if( size.cy < szMinFloated.cy )
			size.cy = szMinFloated.cy;

		CRect rcValidated( rcWnd.TopLeft(), size);
		pControlBar->m_szFloated = size - ptOffsetWnd;
		MoveWindow(&rcValidated);
		SendMessage(WM_NCPAINT);
		DelayRecalcLayout();
		SetTimer( 3, 20, NULL);
		m_bInResizingUpdateState = FALSE;

		return;
	}

	DWORD dwMode = LM_HORZ|LM_COMMIT;
	LONG nLendth = 0;

	if(bHorz)
		nLendth = rcClient.Width();
	else
	{
		dwMode |= LM_LENGTHY;
		nLendth = rcClient.Height();
	}

	CSize szFloatedPrev = pControlBar->m_szFloated;
	CSize size = pControlBar->CalcDynamicLayout( nLendth, dwMode);

	if( szFloatedPrev == size )
	{
		m_bInResizingUpdateState = FALSE;
		return;
	}

	pControlBar->m_szFloated = size;
	if(bInvTop)
	{
		rcWnd.bottom = m_rcWndResizingStart.bottom;
		rcWnd.top = rcWnd.bottom - size.cy - ptOffsetWnd.y;
	}

	if(bInvLeft)
	{
		rcWnd.right = m_rcWndResizingStart.right;
		rcWnd.left = rcWnd.right - size.cx - ptOffsetWnd.x;
	}

	pControlBar->m_ptFloatHelper = rcWnd.TopLeft();
	size += szNcDiff;
	DWORD dwStyle = GetStyle();

	if(dwStyle & (MFS_4THICKFRAME|MFS_THICKFRAME|WS_THICKFRAME))
	{
		size.cx -= GetSystemMetrics(SM_CXFRAME);
		size.cy -= GetSystemMetrics(SM_CYFRAME);
	}
	else
	{
		size.cx -= GetSystemMetrics(SM_CXBORDER);
		size.cy -= GetSystemMetrics(SM_CYBORDER);
	}

	CRect rcToMoveMinFrame(
		pControlBar->m_ptFloatHelper,
		size);

	MoveWindow(&rcToMoveMinFrame);
	SendMessage(WM_NCPAINT);
	RecalcLayout();
	m_bInResizingUpdateState = FALSE;
}

CControlBar *CTMiniDockFrameWnd::GetControlBar()
{
	CWnd *pTempWnd = GetWindow(GW_CHILD);

	if( pTempWnd == NULL )
		return NULL;

	pTempWnd = pTempWnd->GetWindow(GW_CHILD);
	if( pTempWnd == NULL )
		return NULL;

	if(!pTempWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))
		return NULL;

	return reinterpret_cast < CControlBar *> (pTempWnd);
}

const CControlBar * CTMiniDockFrameWnd::GetControlBar() const
{
	CControlBar *pBar = (const_cast <CTMiniDockFrameWnd *> (this))->GetControlBar();
	return pBar;
}

CTControlBar *CTMiniDockFrameWnd::GetTControlBar()
{
	CControlBar *pTempWnd = GetControlBar();

	if( pTempWnd == NULL )
		return NULL;

	if(!pTempWnd->IsKindOf(RUNTIME_CLASS(CTControlBar)))
		return NULL;

	return reinterpret_cast <CTControlBar *>(pTempWnd);
}

const CTControlBar *CTMiniDockFrameWnd::GetTControlBar() const
{
	return (const_cast <CTMiniDockFrameWnd *>(this))->GetTControlBar();
}

void CTMiniDockFrameWnd::OnNcRButtonDown( UINT nHitTest, CPoint point)
{
	if(_ResizingIsInProgress())
	{
		_ResizingEnd();
		return;
	}

	if( nHitTest == HTCAPTION )
	{
		CTControlBar *pControlBar = GetTControlBar();

		if( pControlBar != NULL )
		{
			pControlBar->_ContextMenuTrack();
			return;
		}
	}

	CMiniDockFrameWnd::OnNcRButtonDown( nHitTest, point);
}

void CTMiniDockFrameWnd::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR *lpncsp)
{
	CMiniDockFrameWnd::OnNcCalcSize( bCalcValidRects, lpncsp);
}

void CTMiniDockFrameWnd::OnNcPaint()
{
	CTControlBar *pControlBar = GetTControlBar();
	BOOL bBar = FALSE;

	if( pControlBar != NULL && !pControlBar->IsFixedMode() )
		bBar = TRUE;

	CRect rcClient;
	CRect rcBar;

	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

	CWindowDC dc(this);
	dc.ExcludeClipRect(rcClient);
	CTMemoryDC dcWin(&dc);

	g_PaintManager->PaintDockingFrame(
		dcWin,
		rcBar,
		rcClient,
		TRUE,
		bBar);

	BOOL bEnableCaptionText = TRUE;
	CString strCaption;

	if(pControlBar)
	{
		if(pControlBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			bEnableCaptionText = FALSE;
		else
			pControlBar->GetWindowText(strCaption);
	}

	if(!bEnableCaptionText)
	{
		if( pControlBar && pControlBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) )
			bEnableCaptionText = _GetSingleVisibleCaptionText(strCaption);
	}
	else if( bEnableCaptionText && strCaption.IsEmpty() )
		GetWindowText(strCaption);

	CRect rcCapt(m_rcFrameCaption);
	CRect rcText(m_rcFrameCaption);
	rcText.right = m_rcBtnHideBar.left;

	if(pControlBar)
		pControlBar->NcButtons_CalcGripperTextLocation(rcText);

	BOOL bActive = FALSE;
	HWND hTmpWndFocus = ::GetFocus();

	if( hTmpWndFocus && (
		GetSafeHwnd() == hTmpWndFocus ||
		::IsChild( GetSafeHwnd(), hTmpWndFocus)))
		bActive = TRUE;

	rcText.DeflateRect( 2, 0);
	g_PaintManager->PaintGripper(
		dcWin,
		rcCapt,
		rcText,
		bActive,
		TRUE,
		FALSE,
		bBar,
		strCaption.IsEmpty() ? NULL : (LPCTSTR) strCaption);

	if(pControlBar)
		pControlBar->NcButtons_Paint(dcWin);
	else
	{
		BOOL bPushed = FALSE;
		BOOL bEnabled = TRUE;

		g_PaintManager->PaintDockingCaptionButton(
			dcWin,
			m_rcBtnHideBar,
			CTPaintManager::__DCBT_CLOSE,
			m_bBtnHideBar,
			bPushed,
			bEnabled,
			bBar,
			bActive,
			TRUE);
	}
}

BOOL CTMiniDockFrameWnd::_GetSingleVisibleCaptionText( CString &strCaption)
{
	CFrameWnd *pDockSite = GetParentFrame();
	strCaption = _T("");

	if(!pDockSite)
		return FALSE;

	POSITION pos = pDockSite->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CTControlBar *pBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			(CControlBar *) pDockSite->m_listControlBars.GetNext(pos));

		if( !pBar || !pBar->m_pDockSite ||
			pBar->IsFixedMode() ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) ||
			pBar->GetParentFrame() != this ||
			pBar->IsFloating() ||
			!pBar->IsVisible())
			continue;

		if(!pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			continue;

		if(!pBar->_IsSingleVisibleInFloatingPalette())
			return FALSE;

		pBar->GetWindowText(strCaption);
		return !strCaption.IsEmpty();
	}

	return FALSE;
}

BOOL CTMiniDockFrameWnd::OnNcActivate( BOOL bActive)
{
	BOOL bResult = CMiniDockFrameWnd::OnNcActivate(bActive);
	SendMessage(WM_NCPAINT);

	if(bActive)
	{
		CTControlBar *pControlBar = GetTControlBar();

		if( pControlBar != NULL )
		{
			pControlBar->RedrawWindow(
				NULL,
				NULL,
				RDW_INVALIDATE|
				RDW_UPDATENOW|
				RDW_ERASE|
				RDW_ALLCHILDREN);
		}
	}

	return bResult;
}

void CTMiniDockFrameWnd::OnSize( UINT nType, int cx, int cy)
{
	CMiniDockFrameWnd::OnSize( nType, cx, cy);
	DWORD dwStyle = GetStyle();
	CSize szFrame = dwStyle & WS_THICKFRAME ? CSize(
		GetSystemMetrics(SM_CXSIZEFRAME),
		GetSystemMetrics(SM_CYSIZEFRAME)) : CSize(
		GetSystemMetrics(SM_CXFIXEDFRAME),
		GetSystemMetrics(SM_CYFIXEDFRAME));

	CRect rcFrameCaption;
	GetWindowRect(&rcFrameCaption);

	rcFrameCaption -= rcFrameCaption.TopLeft();
	rcFrameCaption.left += szFrame.cx;
	rcFrameCaption.right -= szFrame.cx;
	rcFrameCaption.top += szFrame.cy;
	rcFrameCaption.bottom = rcFrameCaption.top + ::GetSystemMetrics(SM_CYSMSIZE);
	m_rcFrameCaption = rcFrameCaption;

	int nDummyMetric = ::GetSystemMetrics(SM_CXSMSIZE) - 1;
	int nRight = m_rcFrameCaption.right - nDummyMetric - 1;

	m_rcBtnHideBar.SetRect(
		nRight,
		m_rcFrameCaption.top + 2,
		nRight + nDummyMetric - 2,
		m_rcFrameCaption.bottom - 2);

	CTControlBar *pControlBar = GetTControlBar();
	if(pControlBar)
	{
		pControlBar->OnNcAreaButtonsReinitialize();
		pControlBar->OnNcAreaButtonsReposition();
	}

	if(!_ResizingIsInProgress())
	{
		RedrawWindow(
			NULL,
			NULL,
			RDW_FRAME|
			RDW_INVALIDATE|
			RDW_ERASE|
			RDW_UPDATENOW|
			RDW_ALLCHILDREN);
	}
}

void CTMiniDockFrameWnd::OnWindowPosChanged( WINDOWPOS FAR* lpwndpos)
{
	CMiniDockFrameWnd::OnWindowPosChanged(lpwndpos);

	if( _ResizingIsInProgress() ||
		m_bOnWndPosChgProcessing ||
		lpwndpos->flags&SWP_NOMOVE )
		return;

	CTControlBar *pControlBar = GetTControlBar();
	if( !pControlBar ||
		!pControlBar->IsFloating())
		return;

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	CRect rcDesktop;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &rcDesktop, 0);

	if( rcWnd.top < rcDesktop.top )
	{
		rcWnd.top = rcDesktop.top;
		m_bOnWndPosChgProcessing = TRUE;
	}
	else if( rcWnd.top + ::GetSystemMetrics(SM_CYSMCAPTION) > rcDesktop.bottom )
	{
		rcWnd.top = max(
			rcDesktop.top,
			rcDesktop.bottom - ::GetSystemMetrics(SM_CYSMCAPTION));

		m_bOnWndPosChgProcessing = TRUE;
	}

	if(!m_bOnWndPosChgProcessing)
		return;

	SetWindowPos(
		NULL,
		rcWnd.left,
		rcWnd.top,
		-1, -1,
		SWP_NOZORDER|
		SWP_NOACTIVATE|
		SWP_NOSIZE);

	pControlBar->OnNcAreaButtonsReposition();
	PostMessage(WM_NCPAINT);

	m_bOnWndPosChgProcessing = FALSE;
}

void CTMiniDockFrameWnd::OnWindowPosChanging( WINDOWPOS* lpwndpos)
{
	CMiniDockFrameWnd::OnWindowPosChanging(lpwndpos);
	ModifyStyle( WS_SYSMENU, 0);
}

void CTMiniDockFrameWnd::_AnalyzeButtonsHover( CPoint point)
{
	CTControlBar *pControlBar = GetTControlBar();

	if(pControlBar)
	{
		pControlBar->NcButtons_HitTest(point);
		return;
	}

	CRect rcClient;
	CRect rcBar;

	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);

	GetWindowRect(&rcBar);
	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());

	ScreenToClient(&point);
	point.y -= rcBar.top - rcClient.top;

	BOOL bBtnHideBarHover = FALSE;
	if(m_rcBtnHideBar.PtInRect(point))
		bBtnHideBarHover = TRUE;

	if(	m_bBtnHideBar != bBtnHideBarHover )
	{
		m_bBtnHideBar = bBtnHideBarHover;
		SendMessage(WM_NCPAINT);
	}

	if( m_bBtnHideBar || bBtnHideBarHover )
		SetTimer( 2, 20, NULL);
	else
		KillTimer(2);
}

void CTMiniDockFrameWnd::OnNcMouseMove( UINT nHitTest, CPoint point)
{
	if( m_bAutoMakeTopmostOnMouseEvent &&
		!CTPopupMenuWnd::IsMenuTracking() &&
		!CTControlBar::_DraggingGetBar())
		BringWindowToTop();

	if(_ResizingIsInProgress())
		return;

	CMiniDockFrameWnd::OnNcMouseMove( nHitTest, point);
	_AnalyzeButtonsHover(point);
}

void CTMiniDockFrameWnd::OnTimer( UINT nIDEvent)
{
	CTControlBar *pControlBar = GetTControlBar();

	if( nIDEvent == 1 )
	{
		if(!pControlBar)
			return;
		KillTimer(1);

		pControlBar->OnNcAreaButtonsReposition();
		SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOMOVE|
			SWP_NOACTIVATE|
			SWP_NOZORDER|
			SWP_NOOWNERZORDER|
			SWP_FRAMECHANGED);

		return;
	}

	if( nIDEvent == 2 )
	{
		_AnalyzeButtonsHover();
		return;
	}

	if( nIDEvent == 3 )
	{
		if( !pControlBar || _ResizingIsInProgress() )
			return;
		KillTimer(3);
		RecalcLayout();

		pControlBar->OnNcAreaButtonsReposition();
		SendMessage(WM_NCPAINT);

		return;
	}

	if(!pControlBar)
		return;

	if( nIDEvent == (UINT) pControlBar->GetDlgCtrlID() )
	{
		if(_ResizingIsInProgress())
			return;

		CPoint point;
		GetCursorPos(&point);

		CWnd *pTempWnd = WindowFromPoint(point);
		int IsMouseInsideTheWindow = IsChild(pTempWnd) || pTempWnd->m_hWnd == m_hWnd;

		if( IsMouseInsideTheWindow ||
			IsChild(GetFocus()) || (
			GetCapture() &&
			pControlBar->m_hWnd == GetCapture()->m_hWnd) ||
			CTPopupMenuWnd::IsMenuTracking())
		{
			if( IsMouseInsideTheWindow &&
				!CTPopupMenuWnd::IsMenuTracking() &&
				IsWindowEnabled() &&
				GetFocus())
				BringWindowToTop();
		}
	}
}

void CTMiniDockFrameWnd::OnClose()
{
	CMiniDockFrameWnd::OnClose();
}

void CTMiniDockFrameWnd::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI)
{
	CMiniDockFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CTMiniDockFrameWnd::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_bAutoMakeTopmostOnMouseEvent && !CTPopupMenuWnd::IsMenuTracking() )
		BringWindowToTop();

	if(_ResizingIsInProgress())
	{
		ClientToScreen(&point);
		_ResizingUpdateState(point);

		return;
	}

	CMiniDockFrameWnd::OnMouseMove( nFlags, point);
}

void CTMiniDockFrameWnd::OnCaptureChanged( CWnd *pWnd)
{
	if( _ResizingIsInProgress() && pWnd != this )
	{
		_ResizingEnd();
		return;
	}

	CMiniDockFrameWnd::OnCaptureChanged(pWnd);
}

void CTMiniDockFrameWnd::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(_ResizingIsInProgress())
	{
		_ResizingEnd();
		return;
	}

	CMiniDockFrameWnd::OnLButtonUp( nFlags, point);
}

void CTMiniDockFrameWnd::OnRButtonDown( UINT nFlags, CPoint point)
{
	if(_ResizingIsInProgress())
	{
		_ResizingEnd();
		return;
	}

	CMiniDockFrameWnd::OnRButtonDown( nFlags, point);
}

BOOL CTMiniDockFrameWnd::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

LRESULT CTMiniDockFrameWnd::OnNcHitTest( CPoint point)
{
	if( GetStyle() & MFS_MOVEFRAME )
		ModifyStyle( MFS_MOVEFRAME, 0);

	UINT nHit = CMiniDockFrameWnd::OnNcHitTest(point);
	CTControlBar *pControlBar = GetTControlBar();

	if(pControlBar)
	{
		UINT nHT = pControlBar->NcButtons_HitTest(point);

		if( nHT != HTCLIENT )
		{
			pControlBar->m_bNcAreaBtnTracking = TRUE;
			return nHT;
		}

		if(pControlBar->IsFixedMode())
		{
			switch(nHit)
			{
			case HTTOPLEFT		:
			case HTTOPRIGHT		: nHit = HTTOP; break;
			case HTBOTTOMLEFT	:
			case HTBOTTOMRIGHT	: nHit = HTBOTTOM; break;
			}
		}
	}

	return nHit;
}

void CTMiniDockFrameWnd::OnCancelMode()
{
	CMiniDockFrameWnd::OnCancelMode();
	CTPopupMenuWnd::CancelMenuTracking();

	if(_ResizingIsInProgress())
		_ResizingEnd();
}

void CTMiniDockFrameWnd::OnPaint()
{
	CPaintDC dcPaint(this);
	return;
}

int CTMiniDockFrameWnd::OnToolHitTest( CPoint point, TOOLINFO *pTI) const
{
	if( !pTI || !m_wndToolTip.GetSafeHwnd() )
		return -1;

	const CControlBar *pBar = GetControlBar();
	if(!pBar)
		return -1;

	if(!(pBar->m_dwStyle & CBRS_TOOLTIPS))
		return -1;

	int nRet = pBar->OnToolHitTest( point, pTI);
	if( nRet < 0 )
		return -1;

	if(pTI->lpszText)
	{
		CTMiniDockFrameWnd *pNCT = (CTMiniDockFrameWnd *) this;

		pNCT->m_wndToolTip.AddTool(
			pNCT,
			pTI->lpszText,
			&pTI->rect, 1);
	}

	return -1;
}

BOOL CTMiniDockFrameWnd::PreTranslateMessage( MSG *pMsg)
{
	if(m_wndToolTip.GetSafeHwnd())
		m_wndToolTip.RelayEvent(pMsg);

	return CMiniDockFrameWnd::PreTranslateMessage(pMsg);
}
