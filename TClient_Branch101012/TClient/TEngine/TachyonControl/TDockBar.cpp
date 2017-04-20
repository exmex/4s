#include "stdafx.h"
#include "TDockBar.h"
#include "TMemoryDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////
// CTDockBar window

IMPLEMENT_DYNAMIC( CTDockBar, CDockBar);

BEGIN_MESSAGE_MAP(CTDockBar, CDockBar)
	//{{AFX_MSG_MAP(CTDockBar)
	ON_WM_CONTEXTMENU()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTDockBar::m_bControlBarFixSizePixel = TRUE;
BOOL CTDockBar::m_bExtendedRepositioning = FALSE;
BOOL CTDockBar::m_bDockBarClassRegistered = FALSE;

CTDockBar::CTDockBar( UINT nCircleNo) : m_nCircleNo(nCircleNo),
	m_bLockedOptimize(FALSE),
	m_pDockBarOuter(NULL),
	m_pDockBarInner(NULL),
	m_rcLastInvisiblePreCalc( 0, 0, 0, 0),
	m_bInDynamicLayoutUpdate(FALSE)
{
	RegisterDockBarClass();
}

BOOL CTDockBar::RegisterDockBarClass()
{
	if(m_bDockBarClassRegistered)
		return TRUE;

	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!::GetClassInfo(
		hInst,
		__TDOCKBAR_CLASS_NAME,
		&wndclass))
	{
		wndclass.style = 0;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = ::LoadCursor(
			NULL,
			IDC_ARROW);

		wndclass.hbrBackground = NULL;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = __TDOCKBAR_CLASS_NAME;

		if(!::AfxRegisterClass(&wndclass))
			return FALSE;
	}

	m_bDockBarClassRegistered = TRUE;
	return TRUE;
}

void CTDockBar::PreSubclassWindow()
{
	CDockBar::PreSubclassWindow();
}

BOOL CTDockBar::PreCreateWindow( CREATESTRUCT& cs)
{
	if( !RegisterDockBarClass() ||
		!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszClass = __TDOCKBAR_CLASS_NAME;
	return TRUE;
}

void CTDockBar::OnCancelMode()
{
	CDockBar::OnCancelMode();
	CTPopupMenuWnd::CancelMenuTracking();
}

CFrameWnd *CTDockBar::_GetDockingFrameImpl()
{
	return CTControlBar::_GetDockingFrameImpl((CControlBar *) this);
}

void CTDockBar::_ContextMenuBuild( CFrameWnd *pFrame,
								   CTPopupMenuWnd *pPopup)
{
	ControlBarVector vBars;

	CTControlBar::_GetFrameControlBars(
		pFrame,
		vBars);

	BOOL bOleInplaceItemActivated = CTControlBar::IsOleIpObjActive(pFrame);
	int nBarsCount = vBars.GetSize();
	CTControlBar *pPrevBar = NULL;

	for( int i=0; i<nBarsCount; i++)
	{
		CTControlBar *pBar = vBars[i];

		if( !pBar->GetSafeHwnd() || !::IsWindow(pBar->GetSafeHwnd()) )
			continue;

		if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			continue;

		if(pBar->m_nStateFlags&CControlBar::tempHide)
			continue;

		if( bOleInplaceItemActivated && (pBar->m_dwStyle&CBRS_HIDE_INPLACE) )
			continue;

		CString strBarText;
		pBar->GetWindowText(strBarText);

		if(strBarText.IsEmpty())
			continue;

		if( pPrevBar && pPrevBar->IsFixedMode() != pBar->IsFixedMode() )
			pPopup->ItemInsert(CTPopupMenuWnd::TYPE_SEPARATOR);
		pPrevBar = pBar;

		int nBarID = pBar->GetDlgCtrlID();
		CTCmdManager::cmd _cmd;

		_cmd.m_strMenuText = strBarText;
		_cmd.m_nCmdID = nBarID;
		_cmd.TipsLoad();

		g_CmdManager->CmdSetup(
			g_CmdManager->ProfileNameFromWnd(pFrame->GetSafeHwnd()),
			_cmd);

		pPopup->ItemInsert(nBarID);
	}
}

void CTDockBar::_ContextMenuTrack()
{
	CControlBar *pBar = NULL;

	for( int i=0; i<m_arrBars.GetSize(); i++)
	{
		pBar = GetDockedControlBar(i);

		if(pBar)
			break;
	}

	if(!pBar)
		return;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	CPoint point;

	::GetCursorPos(&point);
	HWND hWndTrack = pBar->GetOwner()->GetSafeHwnd();
	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();

	pPopup->CreatePopupMenu(hWndTrack);
	_ContextMenuBuild(
		pFrame,
		pPopup);

	::SetFocus(hWndTrack);
	pPopup->TrackPopupMenu(
		TPMX_OWNERDRAW_FIXED,
		point.x,
		point.y);
}

void CTDockBar::OnContextMenu( CWnd* pWnd, CPoint point)
{
	_ContextMenuTrack();
}

void CTDockBar::DoPaint( CDC* pDC)
{
	CTPaintManager::ExcludeChildAreas(
		pDC->GetSafeHdc(),
		GetSafeHwnd());

	CRect rcClient;
	GetClientRect(&rcClient);

	g_PaintManager->PaintDockBarClientArea(
		*pDC,
		rcClient);
}

BOOL CTDockBar::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTDockBar::OnDynamicLayoutUpdate()
{
	if(m_bLockedOptimize)
		return;

	if(m_bInDynamicLayoutUpdate)
		return;

	m_bInDynamicLayoutUpdate = TRUE;
	for( int i=1; i<m_arrBars.GetSize(); i++)
	{
		CTControlBar *pBar = (CTControlBar *) m_arrBars[i];

		if( !pBar || __PLACEHODLER_BAR_PTR(pBar) ||
			pBar->IsFixedMode() ||
			!pBar->IsVisible() ||
			!pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) )
			continue;

		((CTDynamicControlBar *) pBar)->m_pWndDynDocker->OnDynamicLayoutUpdate();
	}

	RedrawWindow(
		NULL,
		NULL,
		RDW_ERASE|
		RDW_INVALIDATE|
		RDW_UPDATENOW|
		RDW_ERASENOW|
		RDW_ALLCHILDREN|
		RDW_FRAME);

	m_bInDynamicLayoutUpdate = FALSE;
}

void CTDockBar::OnDynamicLayoutOptimize()
{
	if(m_bLockedOptimize)
		return;

	for( int i=1; i<m_arrBars.GetSize(); i++)
	{
		CTControlBar *pBar = (CTControlBar *) m_arrBars[i];

		if( !pBar || __PLACEHODLER_BAR_PTR(pBar) ||
			pBar->IsFixedMode() ||
			!pBar->IsVisible() ||
			!pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) )
			continue;

		((CTDynamicControlBar *) pBar)->m_pWndDynDocker->OnDynamicLayoutOptimize();
	}
}

CSize CTDockBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz)
{
	CSize szFixed = CControlBar::CalcFixedLayout( bStretch, bHorz);
	CSize szMax;

	if(!m_rectLayout.IsRectEmpty())
		szMax = m_rectLayout.Size();
	else
	{
		CFrameWnd *pFrame = GetParentFrame();
		CRect rcFrameWindow;

		pFrame->GetClientRect(&rcFrameWindow);
		szMax = rcFrameWindow.Size();
	}

	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = m_bLayoutQuery ? NULL : ::BeginDeferWindowPos(m_arrBars.GetSize());

	CPoint point( 0, 0);

	BOOL bWrapped = FALSE;
	int nWidth = 0;

	for( int i=0; i<m_arrBars.GetSize(); i++)
	{
		CControlBar *pBar = GetDockedControlBar(i);
		void *pVoid = m_arrBars[i];

		if(pBar)
		{
			CRect rcBarClient;
			CRect rcBarWin;

			pBar->GetWindowRect(&rcBarWin);
			pBar->ScreenToClient(&rcBarWin);
			pBar->GetClientRect(&rcBarClient);

			CSize szBarMin = rcBarWin.Size() - rcBarClient.Size();
			if(pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			{
				if(!((CTControlBar *) pBar)->IsFixedMode())
				{
					szBarMin = CSize(
						((CTControlBar *) pBar)->_CalcDesiredMinHW(),
						((CTControlBar *) pBar)->_CalcDesiredMinVH());
				}
			}

			if(pBar->IsVisible())
			{
				DWORD dwMode = 0;

				if( (pBar->m_dwStyle&CBRS_SIZE_DYNAMIC) &&
					(pBar->m_dwStyle&CBRS_FLOATING))
					dwMode |= LM_HORZ|LM_MRUWIDTH;
				else if(pBar->m_dwStyle&CBRS_ORIENT_HORZ)
					dwMode |= LM_HORZ|LM_HORZDOCK;
				else
					dwMode |= LM_VERTDOCK;

				CSize szBar = pBar->CalcDynamicLayout( -1, dwMode);
				BOOL bIsMenuBar = FALSE;

				if(pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
				{
					bIsMenuBar = TRUE;

					if(dwMode&LM_HORZDOCK)
						szBar.cx = szMax.cx;
					else if(dwMode&LM_VERTDOCK)
						szBar.cy = szMax.cy;
				}

				CRect rect( point, szBar);
				CRect rcBar;

				pBar->GetWindowRect(&rcBar);
				ScreenToClient(&rcBar);
				BOOL bMenuIsCutted = FALSE;

				if(bHorz)
				{
					if( rcBar.left > rect.left && !m_bFloating )
						rect.OffsetRect( rcBar.left - rect.left, 0);

					if( rect.right > szMax.cx && !m_bFloating )
					{
						int x = rect.Width();

						x = max( szMax.cx - x, point.x);
						rect.OffsetRect( x - rect.left, 0);

						if(bIsMenuBar)
							bMenuIsCutted = TRUE;

						if( rect.right > szMax.cx )
							rect.right -= rect.right - szMax.cx;
					}

					if(bWrapped)
					{
						bWrapped = FALSE;
						rect.OffsetRect( -rect.left, 0);
					}
					else if( rect.Width() < szBarMin.cx || (
						rect.left >= szMax.cx ||
						bMenuIsCutted ) &&
						i > 0 &&
						m_arrBars[i - 1])
					{
						if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) || (
							pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) &&
							((CTControlBar *) pBar)->IsFixedMode()))
						{
							m_arrBars.InsertAt( i, (CObject *) NULL);

							pBar = NULL;
							pVoid = NULL;
							bWrapped = TRUE;
						}
					}

					if(!bWrapped)
					{
						if( rect != rcBar )
						{
							if( !m_bLayoutQuery && !(pBar->m_dwStyle&CBRS_FLOATING))
								pBar->m_pDockContext->m_rectMRUDockPos = rect;

							AfxRepositionWindow(
								&layout,
								pBar->m_hWnd,
								&rect);
						}

						point.x = rect.left + szBar.cx;
						nWidth = max( nWidth, szBar.cy);
					}
				}
				else
				{
					if( rcBar.top > rect.top && !m_bFloating )
						rect.OffsetRect( 0, rcBar.top - rect.top);

					if(	rect.bottom > szMax.cy && !m_bFloating )
					{
						int y = rect.Height();

						y = max( szMax.cy - y, point.y);
						rect.OffsetRect( 0, y - rect.top);

						if(bIsMenuBar)
							bMenuIsCutted = TRUE;

						if( rect.bottom > szMax.cy )
							rect.bottom -= rect.bottom - szMax.cy;
					}

					if(bWrapped)
					{
						bWrapped = FALSE;
						rect.OffsetRect( 0, -rect.top);
					}
					else if( rect.Height() < szBarMin.cy || (
						rect.top >= szMax.cy ||
						bMenuIsCutted ) &&
						i > 0 &&
						m_arrBars[i - 1])
					{
						if( !pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) || (
							pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) &&
							((CTControlBar *) pBar)->IsFixedMode()))
						{
							m_arrBars.InsertAt( i, (CObject *) NULL);
							pBar = NULL;
							pVoid = NULL;
							bWrapped = TRUE;
						}
					}

					if(!bWrapped)
					{
						if( rect != rcBar )
						{
							if( !m_bLayoutQuery && !(pBar->m_dwStyle&CBRS_FLOATING) && pBar->m_pDockContext )
								pBar->m_pDockContext->m_rectMRUDockPos = rect;

							AfxRepositionWindow(
								&layout,
								pBar->m_hWnd,
								&rect);
						}

						point.y = rect.top + szBar.cy;
						nWidth = max( nWidth, szBar.cx);
					}
				}
			}

			if(!bWrapped)
				pBar->RecalcDelayShow(&layout);
		}
		else
		{
			if(m_bExtendedRepositioning)
			{
				if( i > 0 && i != m_arrBars.GetSize() - 1 )
				{
					int nPosPrev = i - 1;

					CControlBar *pPrevBar = NULL;
					do
					{
						pPrevBar = GetDockedControlBar(nPosPrev--);

						if( pPrevBar && pPrevBar->IsVisible() && !pPrevBar->IsFloating() )
						{
							DWORD dwMode = 0;

							if( (pPrevBar->m_dwStyle&CBRS_SIZE_DYNAMIC) &&
								(pPrevBar->m_dwStyle&CBRS_FLOATING))
								dwMode |= LM_HORZ|LM_MRUWIDTH;
							else if(pPrevBar->m_dwStyle&CBRS_ORIENT_HORZ)
								dwMode |= LM_HORZ|LM_HORZDOCK;
							else
								dwMode |= LM_VERTDOCK;

							CSize szPrevBar = pPrevBar->CalcDynamicLayout( -1, dwMode);
							CRect rcPrev( point, szPrevBar);

							if(bHorz)
							{
								if( rcPrev.right > szMax.cx && !m_bFloating )
									pPrevBar = NULL;
							}
							else if( rcPrev.bottom > szMax.cy && !m_bFloating )
								pPrevBar = NULL;

							if(pPrevBar)
								break;
						}

						pPrevBar = NULL;
					} while( nPosPrev >= 0 );

					if(pPrevBar)
					{
						m_arrBars.RemoveAt(i);
						i--;

						continue;
					}
				}
			}
		}

		if( !pBar && !pVoid && nWidth != 0 )
		{
			if(bHorz)
			{
				point.y += nWidth;
				szFixed.cx = max( szFixed.cx, point.x);
				szFixed.cy = max( szFixed.cy, point.y);

				point.x = 0;
				szFixed.cy--;
			}
			else
			{
				point.x += nWidth;
				szFixed.cx = max( szFixed.cx, point.x);
				szFixed.cy = max( szFixed.cy, point.y);

				point.y = 0;
				szFixed.cx--;
			}

			nWidth = 0;
		}
	}

	if( !m_bLayoutQuery && layout.hDWP )
		::EndDeferWindowPos(layout.hDWP);

	CRect rect( 0, 0, 0, 0);
	CalcInsideRect( rect, bHorz);

	if( (!bStretch || !bHorz) && szFixed.cx != 0 )
		szFixed.cx += -rect.right + rect.left + m_bControlBarFixSizePixel ? 1 : 0;

	if( (!bStretch || bHorz) && szFixed.cy != 0 )
		szFixed.cy += -rect.bottom + rect.top + m_bControlBarFixSizePixel ? 1 : 0;

	if(!m_bLayoutQuery)
		OnDynamicLayoutUpdate();

	return szFixed;
}

void CTDockBar::RemoveAllPlaceHolders( BOOL bSearchPlaceHolder)
{
	int nCount = m_arrBars.GetSize();
	MFCControlBarVector vPlaceHolders;

	for( int i=1; i<nCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_arrBars[i];

		if(!pBar)
			continue;

		if(__PLACEHODLER_BAR_PTR(pBar))
		{
			vPlaceHolders.Add(pBar);
			continue;
		}
	}

	nCount = vPlaceHolders.GetSize();
	for( i=0; i<nCount; i++)
	{
		CControlBar *pBar = vPlaceHolders[i];
		RemovePlaceHolder( pBar, bSearchPlaceHolder);
	}
}

void CTDockBar::RemovePlaceHolder( CControlBar *pBar,
								   BOOL bSearchPlaceHolder,
								   BOOL *pRemoved)
{
	if(pRemoved)
		*pRemoved = FALSE;

	if(HIWORD(pBar))
		pBar = (CControlBar *) _AfxGetDlgCtrlID(pBar->m_hWnd);

	int nOldPos = FindBar(pBar);
	if( nOldPos > 0 )
	{
		m_arrBars.RemoveAt(nOldPos);

		if( !m_arrBars[nOldPos - 1] &&
			!m_arrBars[nOldPos])
			m_arrBars.RemoveAt(nOldPos);

		if(pRemoved)
			*pRemoved = TRUE;

		return;
	}

	if(!bSearchPlaceHolder)
		return;

	_RemovePlaceHolder(
		m_pDockSite,
		pBar,
		pRemoved);
}

void CTDockBar::_RemovePlaceHolder( CFrameWnd *pFrame,
								    CControlBar *pBar,
									BOOL *pRemoved)
{
	POSITION pos = pFrame->m_listControlBars.GetHeadPosition();

	while(pos)
	{
		CControlBar *pTestBar = (CControlBar *) pFrame->m_listControlBars.GetNext(pos);

		if(!pTestBar->IsDockBar())
		{
			if(pTestBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			{
				CTDynamicDockBar *pDynDocker = ((CTDynamicControlBar *) pTestBar)->m_pWndDynDocker;
				BOOL bRemoved = FALSE;

				pDynDocker->RemovePlaceHolder(
					pBar,
					FALSE,
					&bRemoved);

				if(bRemoved)
				{
					if(pRemoved)
						*pRemoved = TRUE;

					return;
				}
			}

			continue;
		}

		BOOL bRemoved = FALSE;
		((CTDockBar *) pTestBar)->RemovePlaceHolder(
			pBar,
			FALSE,
			&bRemoved);

		if(bRemoved)
		{
			if(pRemoved)
				*pRemoved = TRUE;

			return;
		}
	}
}

void CTDockBar::_SlideDockControlBar( CControlBar *pBar,
									  LPCRECT lpRect,
									  BOOL bMovingEnabled,
									  const POINT *pDesiredMid)
{
	CRect rcBar;
	pBar->GetWindowRect(&rcBar);

	if(	pBar->m_pDockBar == this && (!lpRect || rcBar == *lpRect))
		return;

	if( m_bFloating && (pBar->m_dwDockStyle&CBRS_FLOAT_MULTI) )
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED|CBRS_SIZE_DYNAMIC);
	m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED|CBRS_SIZE_DYNAMIC);

	if(!(m_dwStyle&CBRS_FLOAT_MULTI))
	{
		TCHAR szTitle[_MAX_PATH];

		pBar->GetWindowText( szTitle, _countof(szTitle));
		AfxSetWindowText( m_hWnd, szTitle);
	}

	DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |=  (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if(m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle(dwStyle);
	BOOL bShow = FALSE;

	if( bMovingEnabled && pBar->m_pDockBar != this && pBar->IsWindowVisible() )
	{
		pBar->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_HIDEWINDOW);

		bShow = TRUE;
	}

	int nPos = -1;
	if(lpRect)
	{
		CRect rect(lpRect);
		ScreenToClient(&rect);

		CPoint ptMid(
			!pDesiredMid ? rect.left + rect.Width() / 2 : pDesiredMid->x,
			!pDesiredMid ? rect.top + rect.Height() / 2 : pDesiredMid->y);

		nPos = _InsertByPointImpl( pBar, rect, ptMid);
		if(bMovingEnabled)
		{
			pBar->SetWindowPos(
				NULL,
				rect.left,
				rect.top,
				rect.Width(),
				rect.Height(),
				SWP_NOREDRAW|
				SWP_NOSENDCHANGING|
				SWP_NOZORDER|
				SWP_NOOWNERZORDER|
				SWP_NOACTIVATE|
				SWP_NOCOPYBITS);
		}
	}
	else
	{
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		if(bMovingEnabled)
		{
			pBar->SetWindowPos(
				NULL,
				-afxData.cxBorder2,
				-afxData.cyBorder2,
				0, 0,
				SWP_NOREDRAW|
				SWP_NOSENDCHANGING|
				SWP_NOZORDER|
				SWP_NOOWNERZORDER|
				SWP_NOACTIVATE|
				SWP_NOCOPYBITS|
				SWP_NOSIZE);
		}
	}

	if( pBar->GetParent() != this )
		pBar->SetParent(this);

	if( pBar->m_pDockBar == this )
	{
		RemoveControlBar(
			pBar,
			nPos, 0,
			FALSE);
	}
	else if(pBar->m_pDockBar)
	{
		if(pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		{
			BOOL bOptimizeDockBarLayout = FALSE;

			if(!((CTDockBar *) pBar->m_pDockBar)->m_bFloating)
				bOptimizeDockBarLayout = TRUE;

			((CTDockBar *) pBar->m_pDockBar)->RemoveControlBar(
				pBar, -1,
				m_bFloating && !pBar->m_pDockBar->m_bFloating,
				FALSE);

			if(bOptimizeDockBarLayout)
				((CTDockBar *) pBar->m_pDockBar)->OnDynamicLayoutOptimize();
		}
		else
		{
			pBar->m_pDockBar->RemoveControlBar(
				pBar, 0,
				m_bFloating && !pBar->m_pDockBar->m_bFloating);
		}
	}

	pBar->m_pDockBar = this;
	pBar->m_pDockContext->m_uMRUDockID = GetDlgCtrlID();

	if(lpRect)
		pBar->m_pDockContext->m_rectMRUDockPos = *lpRect;

	if(bShow)
	{
		pBar->SetWindowPos(
			NULL,
			lpRect->left,
			lpRect->top,
			lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top,
			SWP_NOSIZE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_SHOWWINDOW);
	}

	RemovePlaceHolder( pBar, TRUE);
	_OptimizeCircles();
}

void CTDockBar::_NewRowDockControlBar( CControlBar *pBarDocked,
									   CControlBar *pBarNew,
									   BOOL bLessIndex)
{
	CFrameWnd *pNewDockingFrameWnd = pBarNew->GetDockingFrame();
	int nNewDockBarGetDockedVisibleCount = -1;
	BOOL bNewFloating = pBarNew->IsFloating();
	BOOL bNew = pBarNew->IsKindOf(RUNTIME_CLASS(CTControlBar));

	if( pBarNew->GetParent() != this )
	{
		if(bNew)
			((CTControlBar *) pBarNew)->m_bUpdatingChain = TRUE;

		pBarNew->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_HIDEWINDOW);

		pBarNew->SetParent(this);
		if(bNew)
			((CTControlBar *) pBarNew)->m_bUpdatingChain = FALSE;
	}

	DWORD dwStyle = pBarNew->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |=  (m_dwStyle&CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if(m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;
	pBarNew->SetBarStyle(dwStyle);

	int nCountOfSideBars = m_arrBars.GetSize();
	CControlBar *pRemoveBar = NULL;
	HWND hWndDockBarOld = NULL;

	if( pBarNew->m_pDockBar != this )
	{
		if(pBarNew->m_pDockBar)
		{
			nNewDockBarGetDockedVisibleCount = GetDockedVisibleCount();

			if(pBarNew->IsWindowVisible())
			{
				pBarNew->SetWindowPos(
					NULL,
					0, 0,
					0, 0,
					SWP_NOSIZE|
					SWP_NOMOVE|
					SWP_NOZORDER|
					SWP_NOACTIVATE|
					SWP_HIDEWINDOW);
			}

			if( !pBarNew->m_pDockBar->m_bFloating &&
				pBarNew->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			{
				hWndDockBarOld = pBarNew->m_pDockBar->GetSafeHwnd();
				((CTDockBar *) pBarNew->m_pDockBar)->RemoveControlBar(pBarNew);
			}
			else
				pBarNew->m_pDockBar->RemoveControlBar(pBarNew);
		}
	}
	else if( pBarDocked != pBarNew )
	{
		int nPos = FindBar(pBarNew);

		m_arrBars.RemoveAt(nPos);
		nCountOfSideBars--;

		if( (nCountOfSideBars == nPos && !m_arrBars[nPos - 1]) ||
			(nCountOfSideBars > nPos && !m_arrBars[nPos - 1] && !m_arrBars[nPos]))
		{
			m_arrBars.RemoveAt(nPos - 1);
			nCountOfSideBars--;
		}
	}
	else
		pRemoveBar = pBarNew;

	int nIdxDockedBar = FindBar(pBarDocked);
	int nIncrement = bLessIndex ? -1 : 1;

	for( int i = nIdxDockedBar; TRUE; i += nIncrement)
	{
		CControlBar *pBar = (CControlBar *) m_arrBars[i];

		if( i == 0 || i == (nCountOfSideBars - 1) || !pBar )
		{
			m_arrBars.InsertAt( i, pBarNew);
			m_arrBars.InsertAt( i, (CControlBar *) NULL);

			break;
		}
	}

	CDockBar *pOldDockBar = pBarNew->m_pDockBar;
	pBarNew->m_pDockBar = this;
	pBarNew->m_pDockContext->m_uMRUDockID = GetDlgCtrlID();

	if(pRemoveBar)
	{
		int nPos = FindBar( pRemoveBar, i + 1);

		nCountOfSideBars = m_arrBars.GetSize();
		m_arrBars.RemoveAt(nPos);
		nCountOfSideBars --;

		if( (nCountOfSideBars == nPos && !m_arrBars[nPos - 1]) ||
			(nCountOfSideBars > nPos && !m_arrBars[nPos - 1] && !m_arrBars[nPos]))
			m_arrBars.RemoveAt(nPos - 1);
	}

	if( bNewFloating && !nNewDockBarGetDockedVisibleCount )
	{
		if(!pOldDockBar->GetDockedCount())
			pNewDockingFrameWnd->DestroyWindow();
		else
			pNewDockingFrameWnd->ShowWindow(SW_HIDE);
	}

	pBarNew->SetWindowPos(
		NULL,
		0, 0,
		0, 0,
		SWP_NOSIZE|
		SWP_NOMOVE|
		SWP_NOZORDER|
		SWP_NOACTIVATE|
		SWP_SHOWWINDOW|
		SWP_FRAMECHANGED);

	OnDynamicLayoutUpdate();
	if( hWndDockBarOld &&
		hWndDockBarOld != GetSafeHwnd() &&
		::IsWindow(hWndDockBarOld))
	{
		CWnd *pWndDockBar = FromHandlePermanent(hWndDockBarOld);

		if( pWndDockBar && pWndDockBar->GetSafeHwnd() == hWndDockBarOld )
			((CTDockBar *) pWndDockBar)->OnDynamicLayoutUpdate();
	}

	CFrameWnd *pFrame = GetParentFrame();
	if(!pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		_OptimizeCircles();
}

void CTDockBar::_InnerOuterDockControlBar( CControlBar *pBar, BOOL bInner)
{
	UINT nOwnID = GetDlgCtrlID();
	BOOL bDockAtStartIdx = FALSE;

	if( nOwnID == AFX_IDW_DOCKBAR_RIGHT ||
		nOwnID == AFX_IDW_DOCKBAR_BOTTOM)
		bDockAtStartIdx = TRUE;

	if(!bInner)
		bDockAtStartIdx = FALSE;

	if( pBar->m_pDockBar == this )
	{
		int nCountOfBars = m_arrBars.GetSize();
		if( m_arrBars[1] == pBar )
			return;

		int nBarPos = FindBar(pBar);
		m_arrBars.RemoveAt(nBarPos);
		nCountOfBars --;

		if( !m_arrBars[nBarPos - 1] && !m_arrBars[nBarPos] )
		{
			m_arrBars.RemoveAt(nBarPos);
			nCountOfBars--;
		}

		if(bDockAtStartIdx)
		{
			m_arrBars.InsertAt( 1, pBar);
			nCountOfBars++;

			if( nCountOfBars < 3 || m_arrBars[2] )
				m_arrBars.InsertAt( 2, (CControlBar *) NULL);
		}
		else
		{
			m_arrBars.InsertAt( nCountOfBars, (CControlBar *) NULL);
			m_arrBars.InsertAt( nCountOfBars, pBar);
		}
	}
	else
	{
		CFrameWnd *pDockingFrameWnd = pBar->GetDockingFrame();
		int nDockBarGetDockedVisibleCount = -1;
		BOOL bFloating = pBar->IsFloating();

		pBar->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_HIDEWINDOW);
		pBar->SetParent(this);

		DWORD dwStyle = pBar->GetBarStyle();
		dwStyle &= ~(CBRS_ALIGN_ANY);
		dwStyle |=  (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

		if(m_bFloating)
			dwStyle |= CBRS_FLOATING;
		else
			dwStyle &= ~CBRS_FLOATING;

		pBar->SetBarStyle(dwStyle);
		if(pBar->IsWindowVisible())
		{
			pBar->SetWindowPos(
				NULL,
				0, 0,
				0, 0,
				SWP_NOSIZE|
				SWP_NOMOVE|
				SWP_NOZORDER|
				SWP_NOACTIVATE|
				SWP_HIDEWINDOW);
		}

		if(pBar->m_pDockBar)
		{
			if( !pBar->m_pDockBar->m_bFloating && pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) )
			{
				((CTDockBar *) pBar->m_pDockBar)->RemoveControlBar(pBar);
				((CTDockBar *) pBar->m_pDockBar)->OnDynamicLayoutOptimize();
			}
			else
				pBar->m_pDockBar->RemoveControlBar(pBar);
		}

		int nCountOfBars = m_arrBars.GetSize();
		if(bDockAtStartIdx)
		{
			m_arrBars.InsertAt( 1, pBar);
			nCountOfBars++;

			if( nCountOfBars < 3 || m_arrBars[2] )
				m_arrBars.InsertAt( 2, (CControlBar *) NULL);
		}
		else
		{
			m_arrBars.InsertAt( nCountOfBars, (CControlBar *) NULL);
			m_arrBars.InsertAt( nCountOfBars, pBar);
		}

		CDockBar *pOldDockBar = pBar->m_pDockBar;
		pBar->m_pDockBar = this;
		pBar->m_pDockContext->m_uMRUDockID = GetDlgCtrlID();

		if( bFloating && !nDockBarGetDockedVisibleCount )
		{
			if(!pOldDockBar->GetDockedCount())
				pDockingFrameWnd->DestroyWindow();
			else
				pDockingFrameWnd->ShowWindow(SW_HIDE);
		}

		pBar->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_SHOWWINDOW|
			SWP_FRAMECHANGED);
	}

	_OptimizeCircles();
}

void CTDockBar::_LockSequenceOptimization( BOOL bLock)
{
	m_bLockedOptimize = bLock;
}

void CTDynamicDockBar::_LockSequenceOptimization( BOOL bLock)
{
	m_bLockedOptimize = bLock;
	CTDynamicControlBar *pDynBar = STATIC_DOWNCAST(
		CTDynamicControlBar,
		GetParent());

	if(pDynBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		((CTDockBar *) pDynBar->m_pDockBar)->_LockSequenceOptimization(bLock);
}

void CTDockBar::DockControlBar( CControlBar *pBar, LPCRECT lpRect)
{
	CRect rcBar;

	pBar->GetWindowRect(&rcBar);
	if( pBar->m_pDockBar == this && (!lpRect || rcBar == *lpRect) )
		return;

	if( m_bFloating && (pBar->m_dwDockStyle&CBRS_FLOAT_MULTI))
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
	m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);

	if(!(m_dwStyle & CBRS_FLOAT_MULTI))
	{
		TCHAR szTitle[_MAX_PATH];

		pBar->GetWindowText( szTitle, _countof(szTitle));
		AfxSetWindowText( m_hWnd, szTitle);
	}

	DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |=  (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if (m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle(dwStyle);
	BOOL bShow = FALSE;

	if( pBar->m_pDockBar != this && pBar->IsWindowVisible() )
	{
		pBar->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_HIDEWINDOW);

		bShow = TRUE;
	}

	int nPos = -1;
	if(lpRect)
	{
		CRect rect(lpRect);

		ScreenToClient(&rect);
		CPoint ptMid(
			rect.left + rect.Width() / 2,
			rect.top + rect.Height()/2);

		nPos = CDockBar::Insert( pBar, rect, ptMid);
		pBar->SetWindowPos(
			NULL,
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_NOCOPYBITS);
	}
	else
	{
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		pBar->SetWindowPos(
			NULL,
			-afxData.cxBorder2,
			-afxData.cyBorder2,
			0, 0,
			SWP_NOSIZE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_NOCOPYBITS);
	}

	if( pBar->GetParent() != this )
		pBar->SetParent(this);

	if( pBar->m_pDockBar == this )
		RemoveControlBar( pBar, nPos);
	else if(pBar->m_pDockBar)
	{
		if(pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		{
			BOOL bOptimizeDockBarLayout = FALSE;

			if(!((CTDockBar *) pBar->m_pDockBar)->m_bFloating)
				bOptimizeDockBarLayout = TRUE;

			((CTDockBar *) pBar->m_pDockBar)->RemoveControlBar(
				pBar, -1,
				m_bFloating && !pBar->m_pDockBar->m_bFloating,
				FALSE);

			if( bOptimizeDockBarLayout && !m_bLockedOptimize )
				((CTDockBar *) pBar->m_pDockBar)->OnDynamicLayoutOptimize();
		}
		else
		{
			pBar->m_pDockBar->RemoveControlBar(
				pBar, 0,
				m_bFloating && !pBar->m_pDockBar->m_bFloating);
		}
	}
	pBar->m_pDockBar = this;

	if(bShow)
	{
		pBar->SetWindowPos(
			NULL,
			0, 0,
			0, 0,
			SWP_NOSIZE|
			SWP_NOMOVE|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_SHOWWINDOW);
	}

	RemovePlaceHolder( pBar, TRUE);
	CFrameWnd *pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
}

void CTDockBar::DrawBorders( CDC *pDC, CRect &rect)
{
	g_PaintManager->PaintControlBarBorders(
		CTPaintManager::__CB_OUTER_DOCKBAR,
		m_dwStyle,
		*pDC,
		rect);
}

void CTDockBar::DrawGripper( CDC *pDC, const CRect &rect)
{
}

LRESULT CTDockBar::OnSizeParent( WPARAM wParam, LPARAM lParam)
{
	if(m_bLockedOptimize)
		return 0;

	LRESULT lResult = CDockBar::OnSizeParent( wParam, lParam);
	if(!(m_dwStyle&CBRS_ALIGN_ANY))
		return lResult;

	UINT nOwnID = GetDlgCtrlID();
	if( nOwnID == AFX_IDW_DOCKBAR_FLOAT )
		return lResult;

	AFX_SIZEPARENTPARAMS *lpLayout = (AFX_SIZEPARENTPARAMS *) lParam;
	m_rcLastInvisiblePreCalc = lpLayout->rect;

	switch(nOwnID)
	{
	case AFX_IDW_DOCKBAR_TOP		: m_rcLastInvisiblePreCalc.bottom = m_rcLastInvisiblePreCalc.top; break;
	case AFX_IDW_DOCKBAR_BOTTOM		: m_rcLastInvisiblePreCalc.top = m_rcLastInvisiblePreCalc.bottom; break;
	case AFX_IDW_DOCKBAR_LEFT		: m_rcLastInvisiblePreCalc.right = m_rcLastInvisiblePreCalc.left; break;
	case AFX_IDW_DOCKBAR_RIGHT		: m_rcLastInvisiblePreCalc.left = m_rcLastInvisiblePreCalc.right; break;
	}

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	pFrame->ClientToScreen(&m_rcLastInvisiblePreCalc);

	return lResult;
}

BOOL CTDockBar::RemoveControlBar( CControlBar *pBar,
								  int nPosExclude,
								  int nAddPlaceHolder,
								  BOOL bEnableFrameDelayRecalcLayout)
{
	if( pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) &&
		!((CTControlBar *) pBar)->IsFixedMode())
		nAddPlaceHolder = 0;

	int nPos = FindBar( pBar, nPosExclude);
	if( nAddPlaceHolder == 1 )
	{
		m_arrBars[nPos] = (void *) _AfxGetDlgCtrlID(pBar->m_hWnd);
		int nPosOld = FindBar((CControlBar *) m_arrBars[nPos], nPos);

		if( nPosOld > 0 )
		{
			m_arrBars.RemoveAt(nPos);

			if( !m_arrBars[nPos - 1] && !m_arrBars[nPos] )
				m_arrBars.RemoveAt(nPos);
		}
	}
	else
	{
		m_arrBars.RemoveAt(nPos);

		if( !m_arrBars[nPos - 1] && !m_arrBars[nPos] )
			m_arrBars.RemoveAt(nPos);

		if( nAddPlaceHolder != -1 )
			RemovePlaceHolder( pBar, TRUE);
	}

	if(!pBar->m_pDockContext)
		return FALSE;

	CFrameWnd *pFrameWnd = GetDockingFrame();
	if( m_bFloating && !GetDockedVisibleCount() )
	{
		if(!GetDockedCount())
		{
			pFrameWnd->DestroyWindow();
			return TRUE;
		}
		else
			pFrameWnd->ShowWindow(SW_HIDE);
	}
	else if(bEnableFrameDelayRecalcLayout)
		pFrameWnd->DelayRecalcLayout();

	return TRUE;
}

int CTDockBar::Insert( CControlBar *pBarIns, CRect rect, CPoint ptMid)
{
	return _InsertByPointImpl( pBarIns, rect, ptMid);
}

int CTDockBar::_InsertByPointImpl( CControlBar *pBarIns, CRect rect, CPoint ptMid)
{
	BOOL bBar = pBarIns->IsKindOf(RUNTIME_CLASS(CTControlBar));
	BOOL bTFixedBar = FALSE;

	if(bBar)
		bTFixedBar = ((CTControlBar *) pBarIns)->IsFixedMode();

	BOOL bFixedBar = !bBar || bTFixedBar;
	int nPosInsAfter = -1;
	int nWidth = 0;
	int nTotalWidth = 0;
	int nCountOfSideBars = m_arrBars.GetSize();
	BOOL bHorz = m_dwStyle&CBRS_ORIENT_HORZ;

	CRect rcDockBarWnd;
	GetWindowRect(&rcDockBarWnd);

	if(rcDockBarWnd.PtInRect(ptMid))
	{
		for( int i=0; i<nCountOfSideBars; i++)
		{
			CControlBar *pBar = GetDockedControlBar(i);

			if( pBar && pBar->IsVisible() )
			{
				CRect rcBar;

				pBar->GetWindowRect(&rcBar);
				nWidth = max( nWidth, bHorz ? rcBar.Size().cy : rcBar.Size().cx - 1);

				if( bHorz ? rect.left > rcBar.left : rect.top > rcBar.top )
				{
					if(bFixedBar)
					{
						if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
							nPosInsAfter = i;
						else if(((CTControlBar *) pBar)->IsFixedMode())
							nPosInsAfter = i;
					}
					else
						nPosInsAfter = i;
				}

				if( bFixedBar && rcBar.PtInRect(ptMid) )
				{
					int nDistDown = 0;
					int nDistUp = 0;

					if(bHorz)
					{
						nDistDown = rcBar.bottom - ptMid.y;
						nDistUp = ptMid.y - rcBar.top;
					}
					else
					{
						nDistUp = ptMid.x - rcBar.left;
						nDistDown = rcBar.right - ptMid.x;
					}

					BOOL bNextRow = nDistUp >= nDistDown;
					CControlBar *pFirstBar = CTControlBar::_GetFirstControlBarInRow(
						pBar,
						bNextRow);

					if(bNextRow)
					{
						m_arrBars.InsertAt( nPosInsAfter + 1, pBarIns);
						m_arrBars.InsertAt( nPosInsAfter + 1, (CObject *) NULL);

						return nPosInsAfter + 2;
					}
					else
					{
						m_arrBars.InsertAt( nPosInsAfter, (CObject *) NULL);
						m_arrBars.InsertAt( nPosInsAfter, pBarIns);

						return nPosInsAfter + 1;
					}
				}
			}
			else
			{
				if(pBar)
					continue;

				nTotalWidth += nWidth - afxData.cyBorder2;
				nWidth = 0;

				if( (bHorz ? ptMid.y : ptMid.x) < nTotalWidth )
				{
					if( i == 0 )
					{
						m_arrBars.InsertAt( 1, (CObject *) NULL);
						m_arrBars.InsertAt( 1, pBarIns);

						return 1;
					}
					else
					{
						if(!bFixedBar)
							m_arrBars.InsertAt( i, pBarIns);

						if( nPosInsAfter < 0 )
							break;

						CControlBar *pBar2 = (CControlBar *) m_arrBars[nPosInsAfter];
						if( !pBar2 || __PLACEHODLER_BAR_PTR(pBar2) )
							break;

						CRect rcBar2;
						pBar2->GetWindowRect(&rcBar2);

						int nDistDown = 0;
						int nDistUp = 0;

						if(bHorz)
						{
							nDistDown = rcBar2.bottom - ptMid.y;
							nDistUp = ptMid.y - rcBar2.top;
						}
						else
						{
							nDistDown = rcBar2.right - ptMid.x;
							nDistUp = ptMid.x - rcBar2.left;
						}

						BOOL bNextRow = nDistUp >= nDistDown;
						CControlBar *pFirstBar = CTControlBar::_GetFirstControlBarInRow(
							pBar2,
							bNextRow);

						nPosInsAfter = FindBar(pFirstBar);
						if(bNextRow)
						{
							m_arrBars.InsertAt( nPosInsAfter + 1, pBarIns);
							m_arrBars.InsertAt( nPosInsAfter + 1, (CObject *) NULL);

							return nPosInsAfter + 2;
						}
						else
						{
							m_arrBars.InsertAt( nPosInsAfter, (CObject *) NULL);
							m_arrBars.InsertAt( nPosInsAfter, pBarIns);

							return nPosInsAfter + 1;
						}
					}
				}
			}
		}
	}

	if( nPosInsAfter < 0 )
	{
		int nDistDown = 0;
		int nDistUp = 0;

		if(bHorz)
		{
			nDistDown = rcDockBarWnd.bottom - ptMid.y;
			nDistUp = ptMid.y - rcDockBarWnd.top;
		}
		else
		{
			nDistUp = ptMid.x - rcDockBarWnd.left;
			nDistDown = rcDockBarWnd.right - ptMid.x;
		}

		BOOL bNextRow = abs(nDistUp) >= abs(nDistDown);
		nPosInsAfter = bNextRow ? nCountOfSideBars - 1 : 1;
	}

	if( nPosInsAfter == nCountOfSideBars - 1 )
		m_arrBars.InsertAt( nCountOfSideBars, (CObject *) NULL);

	if( m_arrBars[nPosInsAfter] &&
		nPosInsAfter + 1 < nCountOfSideBars &&
		m_arrBars[nPosInsAfter + 1] )
		m_arrBars.InsertAt( nPosInsAfter + 1, (CObject *) NULL);

	m_arrBars.InsertAt( nPosInsAfter + 1, pBarIns);
	for( int i=nPosInsAfter; i>0; i--)
	{
		CControlBar *pBar = (CControlBar *) m_arrBars[i];
		if(!pBar)
			break;

		if(__PLACEHODLER_BAR_PTR(pBar))
		{
			pBar = _GetDockingFrameImpl()->GetControlBar(LOWORD(DWORD(pBar)));

			if(!pBar)
				continue;
		}

		BOOL bPrevFixedCheck = TRUE;
		if( pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) && !((CTControlBar *) pBar)->IsFixedMode() )
			bPrevFixedCheck = FALSE;

		if( bFixedBar != bPrevFixedCheck )
		{
			m_arrBars.InsertAt( nPosInsAfter + 1, (CObject *) NULL);
			nPosInsAfter++;
		}

		break;
	}

	return nPosInsAfter + 1;
}

void CTDockBar::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp)
{
}

void CTDockBar::OnNcPaint()
{
}

CTDockBar *CTDockBar::_GetInCircle( UINT nDockBarID, BOOL bEnableCreateNew)
{
	UINT nOwnID = GetDlgCtrlID();

	if( nDockBarID == nOwnID )
		return this;

	CFrameWnd *pFrame = GetParentFrame();
	return _GetInCircle(
		pFrame,
		m_nCircleNo,
		nDockBarID,
		bEnableCreateNew);
}

CTDockBar *CTDockBar::_GetInCircle( CFrameWnd *pFrame,
								    UINT nCircleNo,
									UINT nDockBarID,
									BOOL bEnableCreateNew)
{
	POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CControlBar *pBar = (CControlBar *) pFrame->m_listControlBars.GetNext(pos);

		if( !pBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) )
			continue;

		UINT nID = pBar->GetDlgCtrlID();
		if( nID != nDockBarID )
			continue;

		UINT nCircleNo2 = ((CTDockBar *) pBar)->_GetCircleNo();
		if( nCircleNo == nCircleNo2 )
			return (CTDockBar *) pBar;
	}

	if(!bEnableCreateNew)
		return NULL;

	_CreateInnerCircle(pFrame);
	CTDockBar *pDockBar = _GetInCircle(
		pFrame,
		nCircleNo,
		nDockBarID,
		bEnableCreateNew);

	return pDockBar;
}

UINT CTDockBar::_CreateInnerCircle()
{
	if(_GetDockBarInner())
		return _GetCircleNo() + 1;

	CFrameWnd *pFrame = GetParentFrame();
	return _CreateInnerCircle(pFrame);
}

UINT CTDockBar::_CreateInnerCircle( CFrameWnd *pFrame)
{
	CTDockBar *vCreatingCircle[4] = { NULL, NULL, NULL, NULL};
	CTDockBar *vInnerCircle[4] = { NULL, NULL, NULL, NULL};
	UINT nCircleNo = 0;

	POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CControlBar *pBar = (CControlBar *) pFrame->m_listControlBars.GetNext(pos);
		if( !pBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
			continue;

		UINT nCircleNo2 = ((CTDockBar *) pBar)->_GetCircleNo();
		if( nCircleNo2 < nCircleNo )
			continue;

		nCircleNo = nCircleNo2;
		UINT nID = pBar->GetDlgCtrlID();

		switch(nID)
		{
		case AFX_IDW_DOCKBAR_TOP		: vInnerCircle[0] = (CTDockBar *) pBar; break;
		case AFX_IDW_DOCKBAR_BOTTOM		: vInnerCircle[1] = (CTDockBar *) pBar; break;
		case AFX_IDW_DOCKBAR_LEFT		: vInnerCircle[2] = (CTDockBar *) pBar; break;
		case AFX_IDW_DOCKBAR_RIGHT		: vInnerCircle[3] = (CTDockBar *) pBar; break;
		}
	}

	nCircleNo++;
	for( int i=0; i<4; i++)
	{
		CTDockBar *pBar = vInnerCircle[i];
		UINT nID = pBar->GetDlgCtrlID();

		vCreatingCircle[i] = new CTDockBar(nCircleNo);
		static const DWORD dwAlignFlags[4] = {
			CBRS_ALIGN_TOP,
			CBRS_ALIGN_BOTTOM,
			CBRS_ALIGN_LEFT,
			CBRS_ALIGN_RIGHT};

		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|dwAlignFlags[i];
		vCreatingCircle[i]->Create( pFrame, dwStyle, nID);

		pBar->m_pDockBarInner = vCreatingCircle[i];
		vCreatingCircle[i]->m_pDockBarOuter = pBar;
		vCreatingCircle[i]->SetBarStyle(pBar->GetBarStyle());

		if( (pBar->GetStyle() & WS_VISIBLE) && pBar->GetDockedVisibleCount() )
		{
			pBar->GetWindowRect(&vCreatingCircle[i]->m_rcLastInvisiblePreCalc);

			switch(nID)
			{
			case AFX_IDW_DOCKBAR_TOP		: vCreatingCircle[i]->m_rcLastInvisiblePreCalc.bottom = vCreatingCircle[i]->m_rcLastInvisiblePreCalc.top; break;
			case AFX_IDW_DOCKBAR_BOTTOM		: vCreatingCircle[i]->m_rcLastInvisiblePreCalc.top = vCreatingCircle[i]->m_rcLastInvisiblePreCalc.bottom; break;
			case AFX_IDW_DOCKBAR_LEFT		: vCreatingCircle[i]->m_rcLastInvisiblePreCalc.right = vCreatingCircle[i]->m_rcLastInvisiblePreCalc.left; break;
			case AFX_IDW_DOCKBAR_RIGHT		: vCreatingCircle[i]->m_rcLastInvisiblePreCalc.left = vCreatingCircle[i]->m_rcLastInvisiblePreCalc.right; break;
			}
		}
		else
			vCreatingCircle[i]->m_rcLastInvisiblePreCalc = pBar->m_rcLastInvisiblePreCalc;

		switch(nID)
		{
		case AFX_IDW_DOCKBAR_TOP		:
			::SetWindowPos(
				vCreatingCircle[i]->GetSafeHwnd(),
				vInnerCircle[3]->GetSafeHwnd(),
				0, 0,
				0, 0,
				SWP_NOSIZE|
				SWP_NOMOVE|
				SWP_NOREDRAW|
				SWP_NOACTIVATE|
				SWP_NOCOPYBITS|
				SWP_NOSENDCHANGING);

			break;

		case AFX_IDW_DOCKBAR_BOTTOM		:
		case AFX_IDW_DOCKBAR_LEFT		:
		case AFX_IDW_DOCKBAR_RIGHT		:
			::SetWindowPos(
				vCreatingCircle[i]->GetSafeHwnd(),
				vCreatingCircle[i - 1]->GetSafeHwnd(),
				0, 0,
				0, 0,
				SWP_NOSIZE|
				SWP_NOMOVE|
				SWP_NOREDRAW|
				SWP_NOACTIVATE|
				SWP_NOCOPYBITS|
				SWP_NOSENDCHANGING);

			break;
		}
	}

	return nCircleNo;
}

void CTDockBar::_InjectInnerCircle()
{
	_InjectCircle(m_nCircleNo + 1);
}

void CTDockBar::_InjectCircle( UINT nCircleNo)
{
	CFrameWnd *pFrame = GetParentFrame();
	_InjectCircle( pFrame, nCircleNo);
}

void CTDockBar::_InjectCircle( CFrameWnd *pFrame, UINT nCircleNo)
{
	CTDockBar *vCircleOld[4];

	vCircleOld[0] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_TOP);
	vCircleOld[1] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);
	vCircleOld[2] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_LEFT);
	vCircleOld[3] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_RIGHT);

	BOOL bEmptyCircleExist = TRUE;
	for( int nDockSide = 0; nDockSide < 4; nDockSide++)
	{
		vCircleOld[nDockSide] = vCircleOld[nDockSide]->_GetBarByCircleNo(nCircleNo);

		if(!vCircleOld[nDockSide]->_CanBeSafeOptimized())
			bEmptyCircleExist = FALSE;
	}

	if(bEmptyCircleExist)
		return;

	CTDockBar *vCircleNew[4] = { NULL, NULL, NULL, NULL};
	for( nDockSide = 0; nDockSide < 4; nDockSide++)
	{
		UINT nID = vCircleOld[nDockSide]->GetDlgCtrlID();
		vCircleNew[nDockSide] = new CTDockBar(nCircleNo);

		static const DWORD dwAlignFlags[4] = {
			CBRS_ALIGN_TOP,
			CBRS_ALIGN_BOTTOM,
			CBRS_ALIGN_LEFT,
			CBRS_ALIGN_RIGHT};

		DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|dwAlignFlags[nDockSide];
		vCircleNew[nDockSide]->Create( pFrame, dwStyle, nID);
		vCircleOld[nDockSide]->m_pDockBarOuter->m_pDockBarInner = vCircleNew[nDockSide];
		vCircleNew[nDockSide]->m_pDockBarOuter = vCircleOld[nDockSide]->m_pDockBarOuter;
		vCircleOld[nDockSide]->m_pDockBarOuter = vCircleNew[nDockSide];
		vCircleNew[nDockSide]->m_pDockBarInner = vCircleOld[nDockSide];
		vCircleNew[nDockSide]->SetBarStyle(vCircleOld[nDockSide]->GetBarStyle());

		if((vCircleOld[nDockSide]->GetStyle() & WS_VISIBLE) && vCircleOld[nDockSide]->GetDockedVisibleCount())
		{
			vCircleOld[nDockSide]->GetWindowRect(&vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc);

			switch(nID)
			{
			case AFX_IDW_DOCKBAR_TOP		: vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.bottom = vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.top; break;
			case AFX_IDW_DOCKBAR_BOTTOM		: vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.top = vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.bottom; break;
			case AFX_IDW_DOCKBAR_LEFT		: vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.right = vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.left; break;
			case AFX_IDW_DOCKBAR_RIGHT		: vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.left = vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc.right; break;
			}
		}
		else
			vCircleNew[nDockSide]->m_rcLastInvisiblePreCalc = vCircleOld[nDockSide]->m_rcLastInvisiblePreCalc;

		for( CTDockBar *pResetNoBar = vCircleOld[nDockSide]; pResetNoBar; pResetNoBar = pResetNoBar->m_pDockBarInner)
			pResetNoBar->m_nCircleNo++;

		switch(nID)
		{
		case AFX_IDW_DOCKBAR_TOP		:
			{
				::SetWindowPos(
					vCircleNew[nDockSide]->GetSafeHwnd(),
					vCircleOld[3]->m_pDockBarOuter->GetSafeHwnd(),
					0, 0,
					0, 0,
					SWP_NOSIZE|
					SWP_NOMOVE|
					SWP_NOREDRAW|
					SWP_NOACTIVATE|
					SWP_NOCOPYBITS|
					SWP_NOSENDCHANGING);
			}

			break;

		case AFX_IDW_DOCKBAR_BOTTOM		:
		case AFX_IDW_DOCKBAR_LEFT		:
		case AFX_IDW_DOCKBAR_RIGHT		:
			{
				::SetWindowPos(
					vCircleNew[nDockSide]->GetSafeHwnd(),
					vCircleNew[nDockSide-1]->GetSafeHwnd(),
					0, 0,
					0, 0,
					SWP_NOSIZE|
					SWP_NOMOVE|
					SWP_NOREDRAW|
					SWP_NOACTIVATE|
					SWP_NOCOPYBITS|
					SWP_NOSENDCHANGING);
			}

			break;
		}
	}
}

CTDockBar *CTDockBar::_GetBarByCircleNo( UINT nCircleNo)
{
	if( nCircleNo == m_nCircleNo )
		return this;

	if( nCircleNo < m_nCircleNo )
		return m_pDockBarOuter->_GetBarByCircleNo(nCircleNo);

	if(!m_pDockBarInner)
		_CreateInnerCircle();

	return m_pDockBarInner->_GetBarByCircleNo(nCircleNo);
}

LRESULT CTDockBar::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE		: EnableToolTips(); break;
	case WM_DESTROY		:
	case WM_NCDESTROY	:
		if(m_pDockBarInner)
		{
			m_pDockBarInner->m_pDockBarOuter = NULL;
			m_pDockBarInner = NULL;
		}

		if(m_pDockBarOuter)
		{
			m_pDockBarOuter->m_pDockBarInner = NULL;
			m_pDockBarOuter = NULL;
		}

		break;
	}

	return CControlBar::WindowProc( message, wParam, lParam);
}

void CTDockBar::_OptimizeCircles()
{
	CFrameWnd *pFrame = GetParentFrame();

	if(!m_bLockedOptimize)
		_OptimizeCircles(pFrame);
}

void CTDockBar::_OptimizeCircles( CFrameWnd *pFrame)
{
	CTDockBar *vCircle[4];

	vCircle[0] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_TOP);
	vCircle[1] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);
	vCircle[2] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_LEFT);
	vCircle[3] = (CTDockBar *) pFrame->GetControlBar(AFX_IDW_DOCKBAR_RIGHT);

	if(!vCircle[0]->_GetDockBarInner())
		return;

	vCircle[0] = vCircle[0]->_GetDockBarInner();
	vCircle[1] = vCircle[1]->_GetDockBarInner();
	vCircle[2] = vCircle[2]->_GetDockBarInner();
	vCircle[3] = vCircle[3]->_GetDockBarInner();

	UINT nCircleShift = 0;
	while(vCircle[0])
	{
		if( nCircleShift != 0 )
		{
			vCircle[0]->m_nCircleNo -= nCircleShift;
			vCircle[1]->m_nCircleNo -= nCircleShift;
			vCircle[2]->m_nCircleNo -= nCircleShift;
			vCircle[3]->m_nCircleNo -= nCircleShift;
		}

		CTDockBar *vCircleOptimize[4];
		vCircleOptimize[0] = vCircle[0];
		vCircleOptimize[1] = vCircle[1];
		vCircleOptimize[2] = vCircle[2];
		vCircleOptimize[3] = vCircle[3];

		vCircle[0] = vCircle[0]->_GetDockBarInner();
		vCircle[1] = vCircle[1]->_GetDockBarInner();
		vCircle[2] = vCircle[2]->_GetDockBarInner();
		vCircle[3] = vCircle[3]->_GetDockBarInner();

		if( vCircleOptimize[0]->_CanBeSafeOptimized() &&
			vCircleOptimize[1]->_CanBeSafeOptimized() &&
			vCircleOptimize[2]->_CanBeSafeOptimized() &&
			vCircleOptimize[3]->_CanBeSafeOptimized())
		{
			vCircleOptimize[0]->_SafeOptimizeInnerOuterChain();
			vCircleOptimize[1]->_SafeOptimizeInnerOuterChain();
			vCircleOptimize[2]->_SafeOptimizeInnerOuterChain();
			vCircleOptimize[3]->_SafeOptimizeInnerOuterChain();
			nCircleShift++;
		}
		else
		{
			vCircleOptimize[0]->OnDynamicLayoutOptimize();
			vCircleOptimize[1]->OnDynamicLayoutOptimize();
			vCircleOptimize[2]->OnDynamicLayoutOptimize();
			vCircleOptimize[3]->OnDynamicLayoutOptimize();
		}
	}

	CArray < HWND, HWND>	arrHwndsToDynOptimize;
	BOOL bDeepOptimize = FALSE;

	POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CTDynamicControlBar *pBar = DYNAMIC_DOWNCAST(
			CTDynamicControlBar,
			(CObject *) pFrame->m_listControlBars.GetNext(pos));

		if(!pBar)
			continue;

		HWND hWnd = pBar->m_pWndDynDocker->GetSafeHwnd();
		if( !hWnd || !::IsWindow(hWnd) )
			continue;

		int nDockedCount = pBar->m_pWndDynDocker->GetDockedCount();
		if( nDockedCount > 0 )
			continue;

		bDeepOptimize = TRUE;
		arrHwndsToDynOptimize.Add(hWnd);
	}

	for( int nHwndIdx = 0; nHwndIdx < arrHwndsToDynOptimize.GetSize(); nHwndIdx++)
	{
		HWND hWnd = arrHwndsToDynOptimize[nHwndIdx];

		if(!::IsWindow(hWnd))
			continue;

		CWnd *pWndChild = CWnd::FromHandlePermanent(hWnd);
		if(!pWndChild)
			continue;

		CTDynamicDockBar *pDockBar = DYNAMIC_DOWNCAST(
			CTDynamicDockBar,
			pWndChild);

		if(!pDockBar)
			continue;

		pDockBar->OnDynamicLayoutOptimize();
	}

	if(bDeepOptimize)
		_OptimizeCircles(pFrame);
}

void CTDockBar::_SafeOptimizeInnerOuterChain()
{
	if(m_pDockBarInner)
		m_pDockBarInner->m_pDockBarOuter = m_pDockBarOuter;

	if(m_pDockBarOuter)
		m_pDockBarOuter->m_pDockBarInner = m_pDockBarInner;

	m_pDockBarInner = NULL;
	m_pDockBarOuter = NULL;

	CFrameWnd *pFrame = GetParentFrame();
	POSITION pos = pFrame->m_listControlBars.Find(this);

	pFrame->m_listControlBars.RemoveAt(pos);
	BOOL bAutoDelete = m_bAutoDelete;
	DestroyWindow();

	if(!bAutoDelete)
		delete this;
}

BOOL CTDockBar::_CanBeSafeOptimized()
{
	for( int i=0; i<m_arrBars.GetSize(); i++)
	{
		void *pBar = m_arrBars[i];

		if(pBar)
			return FALSE;
	}

	return TRUE;
}

void CTDockBar::_HandleDestruction()
{
	m_bLockedOptimize = TRUE;
	BOOL bFirstDestroyed = TRUE;

	for( int i=m_arrBars.GetSize() - 1; i>0; i--)
	{
		CTControlBar *pBar = (CTControlBar *) m_arrBars[i];

		if(bFirstDestroyed)
		{
			bFirstDestroyed = FALSE;
			continue;
		}

		if( !pBar || __PLACEHODLER_BAR_PTR(pBar) )
		{
			m_arrBars.RemoveAt(i);
			continue;
		}

		pBar->m_bUpdatingChain = TRUE;
		pBar->m_pDockBar = NULL;

		if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
		{
			CTDynamicDockBar *pDynDocker = ((CTDynamicControlBar *) pBar)->m_pWndDynDocker;
			pDynDocker->_HandleDestruction();
		}

		if( pBar->GetParent() != pBar->m_pDockSite )
			pBar->SetParent(pBar->m_pDockSite);

		pBar->m_bUpdatingChain = FALSE;
		m_arrBars.RemoveAt(i);
	}

	m_bLockedOptimize = FALSE;
}

void CTDockBar::OnDestroy()
{
	_HandleDestruction();
	CDockBar::OnDestroy();
}

BOOL CTDockBar::DestroyWindow()
{
	_HandleDestruction();
	return  CDockBar::DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////
// CTDynamicDockBar window

IMPLEMENT_DYNAMIC( CTDynamicDockBar, CTDockBar);

BEGIN_MESSAGE_MAP(CTDynamicDockBar, CTDockBar)
	//{{AFX_MSG_MAP(CTDynamicDockBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTDynamicDockBar::CTDynamicDockBar( UINT nCircleNo) : CTDockBar(nCircleNo),
	m_bHelperDockSiteModified(FALSE)
{
	m_bAutoDelete = TRUE;
}


/////////////////////////////////////////////////////////////////////////
// CTDynamicControlBar window

void CTDynamicDockBar::VisibleLayoutItem::_AssignFromOther( const CTDynamicDockBar::VisibleLayoutItem& other)
{
	m_vRow.RemoveAll();
	int nSize = other.m_vRow.GetSize();

	if( nSize == 0 )
	{
		m_vRow.FreeExtra();
		return;
	}
	m_vRow.SetSize(nSize);

	for( int i=0; i<nSize; i++)
	{
		CTControlBar *pBar = other.m_vRow[i];
		m_vRow.SetAt( i, pBar);
	}
}

CTDynamicDockBar::VisibleLayoutItem::VisibleLayoutItem() : m_nRowMetric(0),
	m_nRowMinMetric(0),
	m_nRowExtent(0),
	m_nRowMinExtent(0)
{
}

CTDynamicDockBar::VisibleLayoutItem::VisibleLayoutItem( const CTDynamicDockBar::VisibleLayoutItem& other)
{
	_AssignFromOther(other);
}

CTDynamicDockBar::VisibleLayoutItem::~VisibleLayoutItem()
{
}

CTDynamicDockBar::VisibleLayoutItem& CTDynamicDockBar::VisibleLayoutItem::operator=( const CTDynamicDockBar::VisibleLayoutItem& other)
{
	_AssignFromOther(other);
	return *this;
}

BOOL CTDynamicDockBar::VisibleLayoutItem::IsEmpty() const
{
	return m_vRow.GetSize() == 0;
}

BOOL CTDynamicDockBar::VisibleLayoutItem::IsRowMinSized() const
{
	return m_nRowMetric == m_nRowMinMetric;
}

void CTDynamicDockBar::VisibleLayout::_Clean()
{
	int nCount = m_vRows.GetSize();

	for( int i=0; i<nCount; i++)
	{
		VisibleLayoutItem *pVLI = m_vRows.GetAt(i);
		delete pVLI;
	}

	m_vRows.RemoveAll();
	m_vRows.FreeExtra();
}

CTDynamicDockBar::VisibleLayout::VisibleLayout() : m_nTotalMetric(0),
	m_nTotalMinMetric(0),
	m_nTotalMinExtent(0),
	m_nTotalBarsCount(0)
{
}

CTDynamicDockBar::VisibleLayout::~VisibleLayout()
{
	_Clean();
}

void CTDynamicDockBar::VisibleLayout::AddBarPointer( CTControlBar *pBar)
{
	if(!pBar->IsVisible())
		return;

	VisibleLayoutItem *pVLI = NULL;
	int nRowCount = m_vRows.GetSize();

	if( nRowCount == 0 )
	{
		pVLI = new VisibleLayoutItem;
		m_vRows.Add(pVLI);
	}
	else
		pVLI = m_vRows[nRowCount - 1];

	pVLI->m_vRow.Add(pBar);
	m_nTotalBarsCount++;
}

void CTDynamicDockBar::VisibleLayout::MakeNewRow()
{
	int nRowCount = m_vRows.GetSize();
	if( nRowCount == 0 )
	{
		VisibleLayoutItem *pVLI = new VisibleLayoutItem;
		m_vRows.Add(pVLI);

		return;
	}

	VisibleLayoutItem *pVLI = m_vRows.GetAt(nRowCount - 1);
	if(pVLI->IsEmpty())
		return;

	pVLI = new VisibleLayoutItem;
	m_vRows.Add(pVLI);
}

BOOL CTDynamicDockBar::VisibleLayout::IsEmpty() const
{
	int nRowCount = m_vRows.GetSize();
	if( nRowCount == 0 )
		return TRUE;

	if( nRowCount == 1 )
	{
		VisibleLayoutItem *pVLI = m_vRows.GetAt(0);

		if(pVLI->IsEmpty())
			return TRUE;
	}

	return FALSE;
}

void CTDynamicDockBar::_VisibleLayoutBuild( VisibleLayout& _vl)
{
	int nCount = m_arrBars.GetSize();
	UINT nOwnID = GetDlgCtrlID();
	BOOL bHorz = nOwnID == AFX_IDW_DOCKBAR_TOP || nOwnID == AFX_IDW_DOCKBAR_BOTTOM;

	for( int i=1; i<nCount; i++)
	{
		CTControlBar *pBar = (CTControlBar *) m_arrBars[i];

		if(!pBar)
		{
			if( i == nCount - 1 )
				break;

			_vl.MakeNewRow();
			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(!pBar->IsVisible())
			continue;

		_vl.AddBarPointer(pBar);
	}

	int nRowsCount = _vl.m_vRows.GetSize();
	if( nRowsCount == 0 )
		return;

	for( int nRow = 0; nRow < nRowsCount; nRow++)
	{
		VisibleLayoutItem *pVLI = _vl.m_vRows[nRow];
		int nRowSize = pVLI->m_vRow.GetSize();

		if( nRowSize == 0 )
			continue;

		CTControlBar *pBar = pVLI->m_vRow[0];
		LONG nMinHW = pBar->_CalcDesiredMinHW();
		LONG nMinVH = pBar->_CalcDesiredMinVH();

		pVLI->m_nRowMinMetric = bHorz ? nMinVH : nMinHW;
		pVLI->m_nRowMetric = bHorz ? pBar->m_szDockedH.cy : pBar->m_szDockedV.cx;
		_vl.m_nTotalMetric += pVLI->m_nRowMetric;

		int nRowMinMetric = bHorz ? nMinVH : nMinHW;
		int nRowMinExtent = bHorz ? nMinHW : nMinVH;
		int nRowExtent = bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;

		for( i=1; i<nRowSize; i++)
		{
			pBar = pVLI->m_vRow[i];

			LONG nMinHW = pBar->_CalcDesiredMinHW();
			LONG nMinVH = pBar->_CalcDesiredMinVH();
			int nBarMinMetric = bHorz ? nMinVH : nMinHW;

			nRowMinMetric = min( nRowMinMetric, nBarMinMetric);
			nRowMinExtent += bHorz ? nMinHW : nMinVH;
			nRowExtent += bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;
		}

		_vl.m_nTotalMinMetric += nRowMinMetric;
		_vl.m_nTotalMinExtent = max( nRowMinExtent, _vl.m_nTotalMinExtent);
		pVLI->m_nRowExtent = nRowExtent;
	}
}

void CTDynamicDockBar::_VisibleLayoutAlign( VisibleLayout& _vl, CSize _size)
{
	CRect rcDockBarReal;

	GetClientRect(&rcDockBarReal);
	_size = rcDockBarReal.Size();

	if( _size.cx <= 0 || _size.cy <= 0 )
		return;

	UINT nOwnID = GetDlgCtrlID();
	BOOL bHorz = nOwnID == AFX_IDW_DOCKBAR_TOP || nOwnID == AFX_IDW_DOCKBAR_BOTTOM;

	int nRowsCount = _vl.m_vRows.GetSize();
	int nDesiredMetric = bHorz ? _size.cy : _size.cx;
	int nIncrement = _vl.m_nTotalMetric < nDesiredMetric ? 1 : -1;
	int nDiff = abs(_vl.m_nTotalMetric - nDesiredMetric);
	BOOL bResetRowMetrics = TRUE;

	while(nDiff)
	{
		bResetRowMetrics = FALSE;
		int nRowsMinSized = 0;

		for( int nRow=0; nRow<nRowsCount; nRow++)
		{
			VisibleLayoutItem *pVLI = _vl.m_vRows[nRow];
			int nRowSize = pVLI->m_vRow.GetSize();

			if( nRowSize == 0 )
				continue;

			if( nIncrement < 0 && pVLI->IsRowMinSized() )
			{
				nRowsMinSized++;
				if( nRowsCount == nRowsMinSized )
					break;

				continue;
			}

			pVLI->m_nRowMetric += nIncrement;
			nDiff--;

			for( int nBar=0; nBar<nRowSize; nBar++)
			{
				CTControlBar *pBar = pVLI->m_vRow[nBar];
				LONG &nRowMetricRef = bHorz ? pBar->m_szDockedH.cy : pBar->m_szDockedV.cx;

				nRowMetricRef += nIncrement;
			}

			if( nDiff == 0 )
				break;
		}

		if( nRowsCount == nRowsMinSized )
			break;
	}

	if(bResetRowMetrics)
	{
		for( int nRow=0; nRow<nRowsCount; nRow++)
		{
			VisibleLayoutItem *pVLI = _vl.m_vRows[nRow];
			int nRowSize = pVLI->m_vRow.GetSize();

			if( nRowSize == 0 )
				continue;

			for( int nBar=0; nBar<nRowSize; nBar++)
			{
				CTControlBar *pBar = pVLI->m_vRow[nBar];
				LONG &nRowMetricRef = bHorz ? pBar->m_szDockedH.cy : pBar->m_szDockedV.cx;

				nRowMetricRef = pVLI->m_nRowMetric;
			}
		}
	}

	nDesiredMetric = bHorz ? _size.cx : _size.cy;
	for( int nRow=0; nRow<nRowsCount; nRow++)
	{
		VisibleLayoutItem *pVLI = _vl.m_vRows[nRow];

		if(pVLI->IsEmpty())
			continue;

		int nIncrement = pVLI->m_nRowExtent < nDesiredMetric ? 1 : -1;
		int nDiff = abs(pVLI->m_nRowExtent - nDesiredMetric);

		while( nDiff != 0 )
		{
			int nRowSize = pVLI->m_vRow.GetSize();
			if( nRowSize == 0 )
				continue;
			int nBarsMinSized = 0;

			for( int nBar=0; nBar<nRowSize; nBar++)
			{
				CTControlBar *pBar = pVLI->m_vRow[nBar];
				LONG &nBarMetricRef = bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;

				if( nIncrement < 0 )
				{
					LONG nBarMinMetric = bHorz ? pBar->_CalcDesiredMinHW() : pBar->_CalcDesiredMinVH();

					if( nBarMetricRef <= nBarMinMetric )
					{
						nBarsMinSized++;

						if( nBarsMinSized == nRowSize )
							break;

						continue;
					}
				}

				nBarMetricRef += nIncrement;
				nDiff--;

				if( nDiff == 0 )
					break;
			}

			if( nBarsMinSized == nRowSize )
				break;
		}
	}

	HANDLE hDWP = ::BeginDeferWindowPos(_vl.m_nTotalBarsCount);
	CPoint ptOffs( 0, 0);

	for( nRow=0; nRow<nRowsCount; nRow++)
	{
		VisibleLayoutItem *pVLI = _vl.m_vRows[nRow];
		int nRowSize = pVLI->m_vRow.GetSize();
		if( nRowSize == 0 )
			continue;
		int nRowOffset = 0;

		for( int nBar=0; nBar<nRowSize; nBar++)
		{
			CTControlBar *pBar = pVLI->m_vRow[nBar];

			if( nBar == 0 )
				nRowOffset = bHorz ? pBar->m_szDockedH.cy : pBar->m_szDockedV.cx;

			CRect rcBar(
				ptOffs,
				bHorz ? pBar->m_szDockedH : pBar->m_szDockedV);

			if( hDWP != NULL )
			{
				hDWP = ::DeferWindowPos(
					hDWP,
					pBar->GetSafeHwnd(),
					NULL,
					rcBar.left,
					rcBar.top,
					rcBar.Width(),
					rcBar.Height(),
					SWP_NOZORDER|SWP_NOACTIVATE);
			}

			if( hDWP == NULL )
				pBar->MoveWindow(&rcBar);

			if(bHorz)
				ptOffs.x += pBar->m_szDockedH.cx;
			else
				ptOffs.y += pBar->m_szDockedV.cy;
		}

		if(bHorz)
		{
			ptOffs.x = 0;
			ptOffs.y += nRowOffset;
		}
		else
		{
			ptOffs.x += nRowOffset;
			ptOffs.y = 0;
		}
	}

	if( hDWP != NULL )
		::EndDeferWindowPos(hDWP);
}


IMPLEMENT_DYNAMIC(CTDynamicControlBar, CTControlBar);

BEGIN_MESSAGE_MAP(CTDynamicControlBar, CTControlBar)
	//{{AFX_MSG_MAP(CTDynamicControlBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CTDynamicControlBar::CTDynamicControlBar() : m_pWndDynDocker(NULL)
{
	m_bAutoDelete = TRUE;
}

CTDynamicDockBar *CTDynamicControlBar::OnCreateDynamicDockBarObject()
{
	return new CTDynamicDockBar(0);
}

int CTDynamicControlBar::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if( CTControlBar::OnCreate(lpCreateStruct) == -1 )
		return -1;
	UINT nOwnID = GetDlgCtrlID();

	m_pWndDynDocker = OnCreateDynamicDockBarObject();
	CFrameWnd *pParentFrame = STATIC_DOWNCAST( CFrameWnd, GetParent());

	if(!m_pWndDynDocker->Create(
		pParentFrame,
		WS_CHILD|
		WS_VISIBLE|
		WS_CLIPSIBLINGS|
		WS_CLIPCHILDREN|
		CBRS_ALIGN_TOP|
		CBRS_BORDER_ANY,
		nOwnID))
		return -1;

	m_pWndDynDocker->SetParent(this);
	EnableToolTips();

	return 0;
}

BOOL CTDynamicControlBar::IsBarWithGripper( BOOL *pGripperAtTop,
										    BOOL *pTextOnGripper) const
{
	if(pGripperAtTop)
		*pGripperAtTop = FALSE;

	if(pTextOnGripper)
		*pTextOnGripper = FALSE;

	return FALSE;
}

CSize CTDynamicControlBar::_CalcLayoutMinSize() const
{
	CTDynamicDockBar *pDockBar = STATIC_DOWNCAST( CTDynamicDockBar, GetWindow(GW_CHILD));
	CTDynamicDockBar::VisibleLayout _vl;

	pDockBar->_VisibleLayoutBuild(_vl);
	UINT nDockBarID = pDockBar->GetDlgCtrlID();
	BOOL bHorz = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;

	CSize _size(
		bHorz ? _vl.m_nTotalMinMetric : _vl.m_nTotalMinExtent,
		bHorz ? _vl.m_nTotalMinExtent : _vl.m_nTotalMinExtent);

	return _size;
}

int CTDynamicControlBar::_CalcDesiredMinHW() const
{
	return _CalcLayoutMinSize().cx;
}

int CTDynamicControlBar::_CalcDesiredMinVH() const
{
	return _CalcLayoutMinSize().cy;
}

CSize CTDynamicControlBar::_CalcDesiredMinFloatedSize() const
{
	return _CalcLayoutMinSize();
}

CSize CTDynamicControlBar::CalcDynamicLayout( int nLength, DWORD nMode)
{
	CSize _size = CTControlBar::CalcDynamicLayout( nLength, nMode);

	if(((CTDockBar *) m_pDockBar)->m_bLayoutQuery)
		return _size;

	if( m_pWndDynDocker->GetSafeHwnd() == NULL )
		return _size;

	int nDockedVisibleCount = m_pWndDynDocker->GetDockedVisibleCount();
	if( nDockedVisibleCount == 0 )
	{
		m_pDockSite->ShowControlBar( this, FALSE, FALSE);
		_size.cx = 0;
		_size.cy = 0;
	}

	CFrameWnd *pParentFrame = GetParentFrame();
	if( pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)) &&
		!m_pWndDynDocker->m_bLockedOptimize &&
		pParentFrame == m_pDockBar->GetParent() )
		m_pWndDynDocker->OnDynamicLayoutUpdate();

	return _size;
}

void CTDynamicDockBar::OnDynamicLayoutUpdate()
{
	if(m_bLockedOptimize)
		return;

	if(m_bInDynamicLayoutUpdate)
		return;

	m_bInDynamicLayoutUpdate = TRUE;
	UINT nDockBarID = GetDlgCtrlID();
	BOOL bHorz = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;

	CalcFixedLayout( TRUE, bHorz);
	m_bInDynamicLayoutUpdate = FALSE;

	CTDockBar::OnDynamicLayoutUpdate();
}

void CTDynamicDockBar::OnDynamicLayoutOptimize()
{
	if(m_bLockedOptimize)
		return;

	BOOL bOptimizeChilds = FALSE;
	BOOL bPlaceHolder = FALSE;

	int nDockedCount = GetDockedCount();
	int nSinglePos = -1;

	if( nDockedCount != 0 )
	{
		for( int nBar=1; nBar<m_arrBars.GetSize(); nBar++)
		{
			CControlBar *pBar = (CControlBar *) m_arrBars[nBar];

			if( pBar == NULL )
				continue;

			if(__PLACEHODLER_BAR_PTR(pBar))
			{
				if( nSinglePos >= 0 && !bPlaceHolder )
				{
					bOptimizeChilds = TRUE;
					break;
				}

				nSinglePos = nBar;
				bPlaceHolder = TRUE;

				continue;
			}
			else
			{
				if( nSinglePos >= 0 )
				{
					bOptimizeChilds = TRUE;
					break;
				}

				nSinglePos = nBar;
			}
		}
	}

	if(bOptimizeChilds)
	{
		CTDockBar::OnDynamicLayoutOptimize();
		return;
	}

	CFrameWnd *pDockSite = m_pDockSite;
	CMiniFrameWnd *pMiniFrame = DYNAMIC_DOWNCAST(
		CMiniFrameWnd,
		GetParentFrame());

	if( pMiniFrame != NULL )
		pMiniFrame->DelayRecalcLayout();

	CTDynamicControlBar *pDynBar = STATIC_DOWNCAST( CTDynamicControlBar, GetParent());
	CDockBar *pDockBarParent = STATIC_DOWNCAST( CDockBar, pDynBar->GetParent());
	HWND hWndOwn = GetSafeHwnd();

	if( nSinglePos == -1 )
	{
		pDynBar->m_pDockBar = NULL;
		pDockSite->RemoveControlBar(this);

		HWND hWndDynBar = pDynBar->GetSafeHwnd();
		pDockBarParent->RemoveControlBar(pDynBar);

		if(::IsWindow(hWndDynBar))
			pDynBar->DestroyWindow();

		if( pMiniFrame == NULL )
			pDockSite->DelayRecalcLayout();

		if(!::IsWindow(hWndOwn))
			return;

		if(pDockBarParent->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			((CTDockBar *) pDockBarParent)->OnDynamicLayoutUpdate();

		return;
	}

	if(bPlaceHolder)
		return;

	CTControlBar *pSingleBar = (CTControlBar *) m_arrBars[nSinglePos];
	CRect rcSingleBar;

	pDynBar->GetWindowRect(&rcSingleBar);
	CSize szSingleBar = rcSingleBar.Size();

	int nDynPos = pDockBarParent->FindBar(pDynBar);
	pDockBarParent->m_arrBars[nDynPos] = pSingleBar;
	pSingleBar->m_pDockBar = pDockBarParent;

	if( pSingleBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) && !((CTControlBar *) pSingleBar)->IsFixedMode() )
	{
		if( pDockBarParent->GetBarStyle() & CBRS_ORIENT_HORZ )
			((CTControlBar *) pSingleBar)->SetInitDesiredSizeHorizontal(szSingleBar);
		else
			((CTControlBar *) pSingleBar)->SetInitDesiredSizeVertical(szSingleBar);

		DWORD dwStyle = pSingleBar->GetBarStyle();
		dwStyle &= ~(CBRS_ALIGN_ANY);
		dwStyle |=  (pDynBar->m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;
		pSingleBar->SetBarStyle(dwStyle);
	}

	m_arrBars.RemoveAll();
	m_arrBars.Add((CControlBar *) NULL);

	pSingleBar->SetParent(pDockBarParent);
	pSingleBar->m_pDockBar = pDockBarParent;

	pDynBar->m_pDockBar = NULL;
	pDockSite->RemoveControlBar(this);
	pDynBar->DestroyWindow();

	if( pMiniFrame == NULL )
		pDockSite->DelayRecalcLayout();

	if(pDockBarParent->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		((CTDockBar *) pDockBarParent)->OnDynamicLayoutUpdate();
}

CSize CTDynamicDockBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz)
{
	VisibleLayout _vl;
	_VisibleLayoutBuild(_vl);

	CRect rect;
	GetWindowRect(&rect);
	CSize _size = rect.Size();

	if( !_vl.IsEmpty() && bStretch && _size.cx > 0 && _size.cy > 0 )
		_VisibleLayoutAlign( _vl, _size);

	return _size;
}

void CTDynamicDockBar::CalcOrderedVector( ControlBarVector& vBars)
{
	int nCount = m_arrBars.GetSize();

	for( int i=1; i<nCount; i++)
	{
		CTControlBar *pBar = (CTControlBar *) m_arrBars[i];

		if( !pBar || __PLACEHODLER_BAR_PTR(pBar) )
			continue;

		vBars.Add(pBar);
	}
}

void CTDynamicDockBar::OnSize( UINT nType, int cx, int cy)
{
	CTDockBar::OnSize( nType, cx, cy);

	if( !m_bHelperDockSiteModified && m_pDockSite )
	{
		POSITION pos = m_pDockSite->m_listControlBars.Find(this);

		if(pos)
		{
			m_pDockSite->m_listControlBars.RemoveAt(pos);
			m_bHelperDockSiteModified = TRUE;
		}
	}
}
