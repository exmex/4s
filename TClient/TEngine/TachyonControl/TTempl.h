#if (!defined __TTEMPL_H)
#define __TTEMPL_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __TMEMORYDC_H)
	#include <TMemoryDC.h>
#endif

#if (!defined __TPAINTMANAGER_H)
	#include <TPaintManager.h>
#endif

#if (!defined __TPOPUPMENUWND_H)
	#include <TPopupMenuWnd.h>
#endif


template< class CTWFFBase = CWnd, BOOL _bExludeChildAreas = TRUE, BOOL _bWM_ERASEBKGND = TRUE>
class CTWFF : public CTWFFBase
{
protected:
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
	{
		switch(message)
		{
		case WM_ERASEBKGND	:
			if(_bWM_ERASEBKGND)
				return FALSE;

			break;

		case WM_PAINT		:
			{
				CPaintDC dcPaint(this);

				if(_bExludeChildAreas)
				{
					CTPaintManager::ExcludeChildAreas(
						dcPaint.GetSafeHdc(),
						GetSafeHwnd());
				}

				CRect rcClient;
				GetClientRect(&rcClient);

				CTMemoryDC dc(
					&dcPaint,
					&rcClient);

				DefWindowProc(
					WM_PAINT,
					WPARAM(dc.GetSafeHdc()),
					LPARAM(0));
			}

			return TRUE;
		};

		return CTWFFBase::WindowProc( message, wParam, lParam);
	};
}; // class CTWFF


template< class CTWRBBase = CWnd>
class CTWRB : public CTWRBBase
{
protected:
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
	{
		switch(message)
		{
		case WM_NCCALCSIZE	:
			{
				NCCALCSIZE_PARAMS *pNCCSP = reinterpret_cast <NCCALCSIZE_PARAMS *> (lParam);
				CRect rcInBarWnd(pNCCSP->rgrc[0]);

				rcInBarWnd.DeflateRect( 1, 1, 1, 1);
				::CopyRect( &pNCCSP->rgrc[0], rcInBarWnd);
			}

			return 0;

		case WM_NCPAINT		:
			{
				CRect rcInBarClient;
				CRect rcInBarWnd;

				GetWindowRect(&rcInBarWnd);
				GetClientRect(&rcInBarClient);
				ClientToScreen(&rcInBarClient);

				if( rcInBarWnd == rcInBarClient )
					return 0;

				CPoint ptDevOffset = -rcInBarWnd.TopLeft();
				rcInBarWnd.OffsetRect(ptDevOffset);
				rcInBarClient.OffsetRect(ptDevOffset);

				CWindowDC dc(this);
				dc.ExcludeClipRect(&rcInBarClient);

				g_PaintManager->PaintResizableBarChildNcAreaRect(
					dc,
					rcInBarWnd);
			}

			return 0;
		}

		return CTWRBBase::WindowProc( message, wParam, lParam);
	};
}; // class CTWRB


#ifdef GET_WM_CTLCOLOR_HDC
	#define __TGET_WM_CTLCOLOR_HDC( wp, lp, msg)		GET_WM_CTLCOLOR_HDC( wp, lp, msg)
#else
	#define __TGET_WM_CTLCOLOR_HDC( wp, lp, msg)		(HDC) (wp)
#endif


#ifdef GET_WM_CTLCOLOR_HWND
	#define __TGET_WM_CTLCOLOR_HWND( wp, lp, msg)		GET_WM_CTLCOLOR_HWND( wp, lp, msg)
#else
	#define __TGET_WM_CTLCOLOR_HWND( wp, lp, msg)		(HWND) (lp)
#endif

#ifdef GET_WM_CTLCOLOR_TYPE
	#define __TGET_WM_CTLCOLOR_TYPE( wp, lp, msg)		GET_WM_CTLCOLOR_TYPE( wp, lp, msg)
#else
	#define __TGET_WM_CTLCOLOR_TYPE( wp, lp, msg)		(WORD) (msg - WM_CTLCOLORMSGBOX)
#endif


template < class CTWSBase, UINT nIdTimerSysMenuTracking = UINT(301)>
class CTWS : public CTWSBase
{
protected:
	CRect m_rcResizingGripper;
	CString m_strSection;
	CString m_strEntry;

	BOOL m_bShowResizingGripper:TRUE;
	BOOL m_bStyledWndInitDone:TRUE;
	BOOL m_bEnableSaveRestore:TRUE;
	BOOL m_bSysMenuTracking:TRUE;

public:
	CTWS() : m_bStyledWndInitDone(FALSE),
		m_bSysMenuTracking(FALSE),
		m_bEnableSaveRestore(FALSE),
		m_bShowResizingGripper(FALSE),
		m_rcResizingGripper( 0, 0, 0, 0)
	{
	};

	CTWS( UINT nIDTemplate, CWnd *pParentWnd) : CTWSBase( nIDTemplate, pParentWnd)
	{
	};

	CTWS( LPCTSTR lpszTemplateName, CWnd *pParentWnd) : CTWSBase( lpszTemplateName, pParentWnd)
	{
	};

	CTWS( UINT nIDTemplate, UINT nIDCaption = 0) : CTWSBase( nIDTemplate, nIDCaption)
	{
	};

	CTWS( LPCTSTR lpszTemplateName, UINT nIDCaption = 0) : CTWSBase( lpszTemplateName, nIDCaption)
	{
	};

	CTWS( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage) : CTWSBase( nIDCaption, pParentWnd, nSelectPage)
	{
	};

	CTWS( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage) : CTWSBase( pszCaption, pParentWnd, nSelectPage)
	{
	};

	void ShowSizeGrip( BOOL bShow = TRUE)
	{
		BOOL _bShow = bShow;

		if( m_bShowResizingGripper != _bShow )
		{
			m_bShowResizingGripper = _bShow;
			InvalidateRect(&m_rcResizingGripper);
		}
	};

	void SaveWindowRect()
	{
		WINDOWPLACEMENT WindowPlacement;
		CString strWindowPlacement;

		ZeroMemory( &WindowPlacement, sizeof(WINDOWPLACEMENT));
		WindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&WindowPlacement);

		RECT &rc = WindowPlacement.rcNormalPosition;
		strWindowPlacement.Format(
			_T("%d,%d,%d,%d,%d,%d"),
			rc.left,
			rc.top,
			rc.right,
			rc.bottom,
			WindowPlacement.showCmd,
			WindowPlacement.flags);

		AfxGetApp()->WriteProfileString(
			m_strSection,
			m_strEntry,
			LPCTSTR(strWindowPlacement));
	};

	void LoadWindowRect( BOOL bForceHideWindow = FALSE)
	{
		WINDOWPLACEMENT WindowPlacement;
		CString strWindowPlacement;

		strWindowPlacement = AfxGetApp()->GetProfileString(
			m_strSection,
			m_strEntry);

		if(strWindowPlacement.IsEmpty())
			return;

		ZeroMemory( &WindowPlacement, sizeof(WINDOWPLACEMENT));
		WindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&WindowPlacement);

		RECT &rc = WindowPlacement.rcNormalPosition;
		if( _stscanf(
			LPCTSTR(strWindowPlacement),
			_T("%d,%d,%d,%d,%d,%d"),
			&rc.left,
			&rc.top,
			&rc.right,
			&rc.bottom,
			&WindowPlacement.showCmd,
			&WindowPlacement.flags) == 6 )
		{
			if(bForceHideWindow)
				WindowPlacement.showCmd = SW_HIDE;

			SetWindowPlacement(&WindowPlacement);
		}

		RedrawWindow(
			NULL,
			NULL,
			RDW_INVALIDATE|
			RDW_ERASE|
			RDW_UPDATENOW|
			RDW_ALLCHILDREN|
			RDW_FRAME);
	};

	void EnableSaveRestore(
		LPCTSTR pszSection,
		LPCTSTR pszEntry,
		BOOL bForceHideWindow = FALSE)
	{
		HWND hWndOwn = GetSafeHwnd();

		m_bEnableSaveRestore = TRUE;
		m_strSection = pszSection;
		m_strEntry = pszEntry;

		if( hWndOwn && ::IsWindow(hWndOwn) )
			LoadWindowRect(bForceHideWindow);
	};

	BOOL _TrackWndSystemPopupMenu(
		CPoint *pPoint,
		BOOL bSelectAny)
	{
		if(!(GetStyle()&WS_POPUP))
			return FALSE;

		if(CTPopupMenuWnd::IsMenuTracking())
			CTPopupMenuWnd::CancelMenuTracking();
		m_bSysMenuTracking = FALSE;

		CMenu *pSysMenu = GetSystemMenu(FALSE);
		if(!pSysMenu)
			return FALSE;

		CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();
		if(!pPopup->CreatePopupMenu(GetSafeHwnd()))
			return FALSE;

		if(!pPopup->UpdateFromMenu(
			GetSafeHwnd(),
			pSysMenu,
			FALSE))
			return FALSE;

		CPoint point;
		if(pPoint)
			point = *pPoint;
		else
		{
			CRect rcClient;
			CRect rcHelper;
			CRect rcWnd;

			GetWindowRect(&rcWnd);
			GetClientRect(&rcClient);
			rcHelper = rcWnd;

			ScreenToClient(&rcHelper);
			int nDiff = rcClient.top - rcHelper.top;

			point = rcWnd.TopLeft();
			point.x += GetSystemMetrics(SM_CXFRAME);
			point.y += nDiff;
		}

		DWORD dwTrackFlags = TPMX_BOTTOMALIGN;
		if(bSelectAny)
			dwTrackFlags |= TPMX_SELECT_ANY;

		SetFocus();
		m_bSysMenuTracking = pPopup->TrackPopupMenu(
			dwTrackFlags,
			point.x,
			point.y,
			NULL);

		if(m_bSysMenuTracking)
			SetTimer( nIdTimerSysMenuTracking, 20, NULL);

		return TRUE;
	};

	virtual BOOL PreTranslateMessage( MSG* pMsg)
	{
		WPARAM wParam = pMsg->wParam;
		LPARAM lParam = pMsg->lParam;
		UINT message = pMsg->message;

		if( message == WM_SYSKEYDOWN ||
			message == WM_SYSKEYUP ||
			message == WM_KEYDOWN )
		{
			BOOL bKeyUp = message == WM_SYSKEYUP;
			BOOL bAlt = HIWORD(lParam)&KF_ALTDOWN;
			TCHAR vkTCHAR = (TCHAR) wParam;

			if( bAlt && vkTCHAR == VK_SPACE )
			{
				if(!bKeyUp)
				{
					CWnd *pWnd = CWnd::GetActiveWindow();

					if( !pWnd || !::IsWindow(pWnd->GetSafeHwnd()) )
						return TRUE;

					if( pWnd->GetSafeHwnd() != GetSafeHwnd() &&
						pWnd->GetStyle() & WS_POPUP )
						return TRUE;

					_TrackWndSystemPopupMenu( NULL, TRUE);
				}

				return 1;
			}
		}

		return CTWSBase::PreTranslateMessage(pMsg);
	};

protected:
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
	{
		switch(message)
		{
		case WM_SIZE		:
			{
				if( m_bShowResizingGripper && !m_rcResizingGripper.IsRectEmpty() )
					InvalidateRect(&m_rcResizingGripper);

				LRESULT lResult = CTWSBase::WindowProc( message, wParam, lParam);
				if(m_bShowResizingGripper)
				{
					GetClientRect(&m_rcResizingGripper);

					m_rcResizingGripper.left = m_rcResizingGripper.right - ::GetSystemMetrics(SM_CXVSCROLL);
					m_rcResizingGripper.top = m_rcResizingGripper.bottom - ::GetSystemMetrics(SM_CYHSCROLL);
					InvalidateRect(&m_rcResizingGripper);
				}

				return lResult;
			}

			break;

		case WM_NCHITTEST	:
			{
				if(m_bShowResizingGripper)
				{
					CPoint ptClient( LOWORD(lParam), HIWORD(lParam));

					ScreenToClient(&ptClient);
					if(m_rcResizingGripper.PtInRect(ptClient))
						return HTBOTTOMRIGHT;
				}
			}

			return CTWSBase::WindowProc( message, wParam, lParam);

		case WM_PAINT		:
			{
				CPaintDC dc(this);

				if( !m_bShowResizingGripper || IsZoomed() )
					return 0;

				CFont *pOldFont = dc.SelectObject(&g_PaintManager->m_FontMarlett);
				COLORREF clrOld = dc.SetTextColor(g_PaintManager->GetColor(CTPaintManager::CLR_TEXT_OUT));

				int nOldBkMode = dc.SetBkMode(TRANSPARENT);
				dc.DrawText(
					_T("o"), 1,
					m_rcResizingGripper,
					DT_SINGLELINE|
					DT_RIGHT|
					DT_BOTTOM);

				dc.SetBkMode(nOldBkMode);
				dc.SetTextColor(clrOld);
				dc.SelectObject(pOldFont);
			}

			return 0;

		case WM_ERASEBKGND	:
			{
				HDC hDC = reinterpret_cast <HDC> (wParam);

				CTPaintManager::ExcludeChildAreas(
					hDC,
					GetSafeHwnd(),
					CTPaintManager::DefExcludeChildAreaCallback);
			}

			return CTWSBase::WindowProc( message, wParam, lParam);

		case WM_TIMER		:
			{
				if( wParam == nIdTimerSysMenuTracking )
				{
					if(!CTPopupMenuWnd::IsMenuTracking())
					{
						m_bSysMenuTracking = FALSE;
						KillTimer(nIdTimerSysMenuTracking);
					}

					return 0;
				}
			}

			return CTWSBase::WindowProc( message, wParam, lParam);

		case WM_DESTROY		:
			{
				if(m_bEnableSaveRestore)
					SaveWindowRect();
			}

			return CTWSBase::WindowProc( message, wParam, lParam);

		default				:
			{
				if( message >= WM_CTLCOLORMSGBOX && message <= WM_CTLCOLORSTATIC )
				{
					HBRUSH hBrush = (HBRUSH) CTWSBase::WindowProc( message, wParam, lParam);
					HDC hDC = __TGET_WM_CTLCOLOR_HDC( wParam, lParam, message);

					int nCtlColor = __TGET_WM_CTLCOLOR_TYPE( wParam, lParam, message);
					if( nCtlColor == CTLCOLOR_DLG ||
						nCtlColor == CTLCOLOR_STATIC ||
						nCtlColor == CTLCOLOR_MSGBOX)
					{
						if( nCtlColor == CTLCOLOR_STATIC )
							::SetBkMode( hDC, TRANSPARENT);

						static COLORREF clrLast = (COLORREF) -1L;
						static CBrush brBackground;

						COLORREF clrNew = g_PaintManager->GetColor(CTPaintManager::CLR_3DFACE_OUT);
						if( clrLast != clrNew && brBackground.GetSafeHandle() )
							brBackground.DeleteObject();

						if(!brBackground.GetSafeHandle())
						{
							clrLast = clrNew;
							brBackground.CreateSolidBrush(clrLast);
						}

						return (LRESULT) brBackground.GetSafeHandle();
					}

					return (LRESULT) hBrush;
				}

				if((message == WM_NCLBUTTONUP &&
					wParam == HTSYSMENU ) || (
					message == WM_NCRBUTTONUP && (
					wParam == HTCAPTION ||
					wParam == HTSYSMENU)))
				{
					_TrackWndSystemPopupMenu(
						NULL,
						FALSE);

					return 0;
				}

				if((message == WM_NCLBUTTONDOWN ||
					message == WM_NCRBUTTONDOWN) &&
					wParam == HTSYSMENU)
					return 0;

				if( message == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
				{
					CPoint point;
					GetCursorPos(&point);

					_TrackWndSystemPopupMenu(
						&point,
						FALSE);

					return 0;
				}
			}

			break;
		}

		return CTWSBase::WindowProc( message, wParam, lParam);
	};

	virtual void PostNcDestroy()
	{
		m_bStyledWndInitDone = FALSE;
		CTWSBase::PostNcDestroy();
	}
}; // class CTWS


const SIZE __RDA_NONE	= {	 -1,	 -1};
const SIZE __RDA_LT		= {	  0,	  0};
const SIZE __RDA_RT		= {	100,	  0};
const SIZE __RDA_LB		= {	  0,	100};
const SIZE __RDA_RB		= {	100,	100};

const SIZE __RDA_KEEP	= __RDA_LT;
const SIZE __RDA_X		= __RDA_RT;
const SIZE __RDA_Y		= __RDA_LB;
const SIZE __RDA_XY		= __RDA_RB;
const SIZE __RDA_BOTH	= __RDA_RB;


template< class CTWABase>
class CTWA : public CTWABase
{
protected:
	BOOL m_bAnchoringInitDone:TRUE;
	BOOL m_bUseMaxTrack:TRUE;
	BOOL m_bUseMinTrack:TRUE;
	BOOL m_bUseMaxRect:TRUE;

	CPoint m_ptMinTrackSize;
	CPoint m_ptMaxTrackSize;
	CPoint m_ptMaxPos;
	CPoint m_ptMaxSize;

	struct RDI
	{
		SIZE m_AnchLT;
		SIZE m_MargLT;
		SIZE m_AnchRB;
		SIZE m_MargRB;

		RDI(SIZE AnchLT = __RDA_LT,
			SIZE MargLT = CSize( 0, 0),
			SIZE AnchRB = __RDA_LT,
			SIZE MargRB = CSize( 0, 0)) : m_AnchLT(AnchLT),
			m_MargLT(MargLT),
			m_AnchRB(AnchRB),
			m_MargRB(MargRB)
		{
		};

		RDI(const RDI& other) : m_AnchLT(other.m_AnchLT),
			m_MargLT(other.m_MargLT),
			m_AnchRB(other.m_AnchRB),
			m_MargRB(other.m_MargRB)
		{
		};

		RDI& operator=( const RDI& other)
		{
			m_AnchLT = other.m_AnchLT;
			m_MargLT = other.m_MargLT;
			m_AnchRB = other.m_AnchRB;
			m_MargRB = other.m_MargRB;

			return *this;
		};
	};

	CMap< HWND, HWND, RDI, RDI>	m_mapRDI;

public:
	CTWA() : m_bAnchoringInitDone(FALSE),
		m_bUseMinTrack(TRUE),
		m_bUseMaxTrack(FALSE),
		m_bUseMaxRect(FALSE),
		m_ptMinTrackSize( 0, 0),
		m_ptMaxTrackSize( 0, 0),
		m_ptMaxPos( 0, 0),
		m_ptMaxSize( 0, 0)
	{
	};

	CTWA( UINT nIDTemplate, CWnd *pParentWnd) : CTWABase( nIDTemplate, pParentWnd),
		m_bAnchoringInitDone(FALSE),
		m_bUseMinTrack(TRUE),
		m_bUseMaxTrack(FALSE),
		m_bUseMaxRect(FALSE),
		m_ptMinTrackSize( 0, 0),
		m_ptMaxTrackSize( 0, 0),
		m_ptMaxPos( 0, 0),
		m_ptMaxSize( 0, 0)
	{
	};

	CTWA( LPCTSTR lpszTemplateName, CWnd *pParentWnd) : CTWABase( lpszTemplateName, pParentWnd),
		m_bAnchoringInitDone(FALSE),
		m_bUseMinTrack(TRUE),
		m_bUseMaxTrack(FALSE),
		m_bUseMaxRect(FALSE),
		m_ptMinTrackSize( 0, 0),
		m_ptMaxTrackSize( 0, 0),
		m_ptMaxPos( 0, 0),
		m_ptMaxSize( 0, 0)
	{
	};

	CTWA( UINT nIDTemplate, UINT nIDCaption = 0) : CTWABase( nIDTemplate, nIDCaption),
		m_bAnchoringInitDone(FALSE),
		m_bUseMinTrack(TRUE),
		m_bUseMaxTrack(FALSE),
		m_bUseMaxRect(FALSE),
		m_ptMinTrackSize( 0, 0),
		m_ptMaxTrackSize( 0, 0),
		m_ptMaxPos( 0, 0),
		m_ptMaxSize( 0, 0)
	{
	};

	CTWA( LPCTSTR lpszTemplateName, UINT nIDCaption = 0) : CTWABase( lpszTemplateName, nIDCaption),
		m_bAnchoringInitDone(FALSE),
		m_bUseMinTrack(TRUE),
		m_bUseMaxTrack(FALSE),
		m_bUseMaxRect(FALSE),
		m_ptMinTrackSize( 0, 0),
		m_ptMaxTrackSize( 0, 0),
		m_ptMaxPos( 0, 0),
		m_ptMaxSize( 0, 0)
	{
	};

	CTWA( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage) : CTWABase( nIDCaption, pParentWnd, nSelectPage)
	{
	};

	CTWA( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage) : CTWABase( pszCaption, pParentWnd, nSelectPage)
	{
	};

	BOOL AddAnchor(
		UINT nDlgCtrlID,
		const CRect &rcAnch)
	{
		if( !GetSafeHwnd() || !::IsWindow(GetSafeHwnd()) )
			return FALSE;

		HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID);
		if( hWnd == NULL || !::IsWindow(hWnd) )
			return FALSE;

		return AddAnchor( hWnd, rcAnch);
	};

	BOOL AddAnchor(
		HWND hWnd,
		const CRect &rcAnch)
	{
		if( !GetSafeHwnd() || !::IsWindow(GetSafeHwnd()) )
			return FALSE;

		return AddAnchor(
			hWnd,
			CSize( rcAnch.left, rcAnch.top),
			CSize( rcAnch.right, rcAnch.bottom));
	};

	BOOL AddAnchor(
		UINT nDlgCtrlID,
		CSize AnchLT,
		CSize AnchRB = __RDA_NONE)
	{
		if( AnchLT == __RDA_NONE ||
			!GetSafeHwnd() ||
			!::IsWindow(GetSafeHwnd()))
			return FALSE;

		HWND hWnd = ::GetDlgItem( GetSafeHwnd(), nDlgCtrlID);
		if( hWnd == NULL || !::IsWindow(hWnd) )
			return FALSE;

		return AddAnchor( hWnd, AnchLT, AnchRB);
	};

	virtual BOOL AddAnchor(
		HWND hWnd,
		CSize szAnchLT,
		CSize szAnchRB = __RDA_NONE)
	{
		if( szAnchLT == __RDA_NONE ||
			!GetSafeHwnd() ||
			!::IsWindow(GetSafeHwnd()) ||
			!::IsChild( GetSafeHwnd(), hWnd))
			return FALSE;

		CRect rcClient;
		CRect rcItem;

		GetClientRect(&rcClient);

		int cx = rcClient.Width();
		int cy = rcClient.Height();

		::GetWindowRect( hWnd, &rcItem);
		ScreenToClient(&rcItem);

		CSize szMargLT;
		CSize szMargRB;

		if( szAnchRB == __RDA_NONE )
			szAnchRB = szAnchLT;

		szMargLT.cx = rcItem.left - cx * szAnchLT.cx / 100;
		szMargLT.cy = rcItem.top - cy * szAnchLT.cy / 100;

		szMargRB.cx = rcItem.right - cx * szAnchRB.cx / 100;
		szMargRB.cy = rcItem.bottom - cy * szAnchRB.cy / 100;

		m_mapRDI.SetAt(
			hWnd,
			RDI( szAnchLT, szMargLT, szAnchRB, szMargRB));

		return TRUE;
	};

	virtual void ArrangeLayout(
		int cx = -1,
		int cy = -1)
	{
		if( cx < 0 || cy < 0 )
		{
			CRect rcClient;
			GetClientRect(&rcClient);

			cx = rcClient.Width();
			cy = rcClient.Height();
		}

		HDWP _hDWP = ::BeginDeferWindowPos((int) m_mapRDI.GetCount());
		CList< HWND, HWND> listInvalidHWNDs;
		POSITION pos = m_mapRDI.GetStartPosition();

		while(pos)
		{
			HWND _hWnd;
			RDI _rdi;

			m_mapRDI.GetNextAssoc( pos, _hWnd, _rdi);
			if(!::IsWindow(_hWnd))
			{
				listInvalidHWNDs.AddTail(_hWnd);
				continue;
			}

			int x = _rdi.m_MargLT.cx + ::MulDiv( cx, _rdi.m_AnchLT.cx, 100);
			int y = _rdi.m_MargLT.cy + ::MulDiv( cy, _rdi.m_AnchLT.cy, 100);
			int dx = _rdi.m_MargRB.cx + ::MulDiv( cx, _rdi.m_AnchRB.cx, 100) - x;
			int dy = _rdi.m_MargRB.cy + ::MulDiv( cy, _rdi.m_AnchRB.cy, 100) - y;

			if(CTPaintManager::DefIsHwndNeedsDirectRepaint(_hWnd))
			{
				::MoveWindow(
					_hWnd,
					x, y,
					dx, dy,
					TRUE);

				CRect rcChild(
					0, 0,
					dx, dy);

				::RedrawWindow(
					_hWnd,
					&rcChild,
					NULL,
					RDW_INVALIDATE|
					RDW_UPDATENOW|
					RDW_ERASE|
					RDW_ERASENOW|
					RDW_ALLCHILDREN|
					RDW_NOFRAME|
					RDW_VALIDATE);

				continue;
			}

			_hDWP = ::DeferWindowPos(
				_hDWP,
				_hWnd,
				NULL,
				x, y,
				dx, dy,
				SWP_NOZORDER|
				SWP_NOACTIVATE);
		}

		::EndDeferWindowPos(_hDWP);
		if( listInvalidHWNDs.GetCount() > 0 )
		{
			pos = listInvalidHWNDs.GetHeadPosition();

			while(pos)
			{
				HWND hWnd = listInvalidHWNDs.GetNext(pos);
				m_mapRDI.RemoveKey(hWnd);
			}
		}
	};

	void SetMaximizedRect( const CRect& rc)
	{
		m_bUseMaxRect = TRUE;
		m_ptMaxPos = rc.TopLeft();
		m_ptMaxSize.x = rc.Width();
		m_ptMaxSize.y = rc.Height();
	};

	void ResetMaximizedRect()
	{
		m_bUseMaxRect = FALSE;
	};

	void SetMinTrackSize( const CSize& size)
	{
		m_bUseMinTrack = TRUE;
		m_ptMinTrackSize.x = size.cx;
		m_ptMinTrackSize.y = size.cy;
	};

	void ResetMinTrackSize()
	{
		m_bUseMinTrack = FALSE;
	};

	void SetMaxTrackSize( const CSize& size)
	{
		m_bUseMaxTrack = TRUE;
		m_ptMaxTrackSize.x = size.cx;
		m_ptMaxTrackSize.y = size.cy;
	};

	void ResetMaxTrackSize()
	{
		m_bUseMaxTrack = FALSE;
	};

protected:
	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam)
	{
		switch(message)
		{
		case WM_GETMINMAXINFO	:
			{
				if(!m_bAnchoringInitDone)
					return 0;

				LPMINMAXINFO lpMMI = reinterpret_cast <LPMINMAXINFO> (lParam);
				if(m_bUseMinTrack)
					lpMMI->ptMinTrackSize = m_ptMinTrackSize;

				if(m_bUseMaxTrack)
					lpMMI->ptMaxTrackSize = m_ptMaxTrackSize;

				if(m_bUseMaxRect)
				{
					lpMMI->ptMaxPosition = m_ptMaxPos;
					lpMMI->ptMaxSize = m_ptMaxSize;
				}
			}

			return 0;

		case WM_SIZE			:
			{
				LRESULT lResult = CTWABase::WindowProc( message, wParam, lParam);

				if(!m_bAnchoringInitDone)
				{
					CRect rc;

					GetWindowRect(&rc);
					m_ptMinTrackSize.x = rc.Width();
					m_ptMinTrackSize.y = rc.Height();
					m_bAnchoringInitDone = TRUE;

					ArrangeLayout();
					return lResult;
				}

				if( wParam == SIZE_MAXHIDE || wParam == SIZE_MAXSHOW )
					return lResult;

				if(m_bAnchoringInitDone)
					ArrangeLayout( LOWORD(lParam), HIWORD(lParam));

				return lResult;
			}

			break;
		}

		return CTWABase::WindowProc( message, wParam, lParam);
	};
}; // class CTWA


template< class CTADLGBase>
class CTADLG : public CTADLGBase
{
public:
	CTADLG()
	{
	};

	CTADLG( UINT nIDTemplate, CWnd *pParentWnd) : CTADLGBase( nIDTemplate, pParentWnd)
	{
	};

	CTADLG( LPCTSTR lpszTemplateName, CWnd *pParentWnd) : CTADLGBase( lpszTemplateName, pParentWnd)
	{
	};

	CTADLG( UINT nIDTemplate, UINT nIDCaption = 0)
	{
	};

	CTADLG( LPCTSTR lpszTemplateName, UINT nIDCaption = 0)
	{
	};

	CTADLG( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};

	CTADLG( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};
}; // class CTADLG


template< class CTAPSHBase>
class CTAPSH : public CTAPSHBase
{
public:
	CTAPSH()
	{
	};

	CTAPSH( UINT nIDTemplate, CWnd *pParentWnd)
	{
	};

	CTAPSH( LPCTSTR lpszTemplateName, CWnd *pParentWnd)
	{
	};

	CTAPSH( UINT nIDTemplate, UINT nIDCaption = 0)
	{
	};

	CTAPSH( LPCTSTR lpszTemplateName, UINT nIDCaption = 0)
	{
	};

	CTAPSH( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage) : CTAPSHBase( nIDCaption, pParentWnd, nSelectPage)
	{
	};

	CTAPSH( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage) : CTAPSHBase( pszCaption, pParentWnd, nSelectPage)
	{
	};
}; // class CTAPSH


template< class CTAPPGBase>
class CTAPPG : public CTAPPGBase
{
public:
	CTAPPG()
	{
	};

	CTAPPG( UINT nIDTemplate, CWnd *pParentWnd)
	{
	};

	CTAPPG( LPCTSTR lpszTemplateName, CWnd *pParentWnd)
	{
	};

	CTAPPG( UINT nIDTemplate, UINT nIDCaption = 0) : CTAPPGBase( nIDTemplate, nIDCaption)
	{
	};

	CTAPPG( LPCTSTR lpszTemplateName, UINT nIDCaption = 0) : CTAPPGBase( lpszTemplateName, nIDCaption)
	{
	};

	CTAPPG( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};

	CTAPPG( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};
}; // class CTAPPG


template< class CTAGWBase>
class CTAGW : public CTAGWBase
{
public:
	CTAGW()
	{
	};

	CTAGW( UINT nIDTemplate, CWnd *pParentWnd)
	{
	};

	CTAGW( LPCTSTR lpszTemplateName, CWnd *pParentWnd)
	{
	};

	CTAGW( UINT nIDTemplate, UINT nIDCaption = 0) : CTAGWBase( nIDTemplate, nIDCaption)
	{
	};

	CTAGW( LPCTSTR lpszTemplateName, UINT nIDCaption = 0) : CTAGWBase( lpszTemplateName, nIDCaption)
	{
	};

	CTAGW( UINT nIDCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};

	CTAGW( LPCTSTR pszCaption, CWnd *pParentWnd, UINT nSelectPage)
	{
	};
}; // class CTAGW


#endif // #if (!defined __TTEMPL_H)