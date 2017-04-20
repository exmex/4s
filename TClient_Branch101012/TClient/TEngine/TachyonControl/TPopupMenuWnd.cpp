#include "stdafx.h"

#define _AFX_NO_OLE_SUPPORT
#include <math.h>
#include "TPopupMenuWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_TIMER_ANIMATION			666
#define ID_PERIOD_ANIMATION			30

#define ID_TIMER_ITEM_FOCUS_DELAY	667
#define ID_PERIOD_ITEM_FOCUS_DELAY	500

#define ID_TIMER_SCROLLING			668
#define ID_PERIOD_SCROLLING			15

#define ID_TIMER_DELAY_SHOW			669
#define ID_PERIOD_DELAY_SHOW		1000

#define ID_TIMER_DELAY_EXPAND		670
#define ID_FREQ_DELAY_EXPAND		50
#define ID_TOTAL_DELAY_EXPAND		1500

static const int g_nAnimStepMetric = 20;
static clock_t g_nLastAnimTime = 0;

#define __SCROLLING_PIXEL_STEP		5
#define __ANIM_CY					15

#define __EXCLUDE_AREA_GAP_DX		3
#define __EXCLUDE_AREA_GAP_DY		__EXCLUDE_AREA_GAP_DX
#define __MI_HORZ_GAP_TO_BORDER		1
#define __BOX_ANIM_METRIC			32
#define __TIP_OVER_METRIC			12

#define __NCLR_DX					8
#define __NCLR_DY					5
#define __NCLR_COUNT				(__NCLR_DX * __NCLR_DY)
#define __NCLR_BOX_DX_SIZE			16
#define __NCLR_BOX_DY_SIZE			16
#define __NCLR_BOX_DX_SPACE			2
#define __NCLR_BOX_DY_SPACE			2

#define CX_ROUNDED					12
#define CY_ROUNDED					10
#define CX_LEADER					25
#define CY_LEADER					25
#define CX_ICON_MARGIN				5

BOOL CTPopupMenuWnd::m_bUseStretchOnExpandAnimation = FALSE;
BOOL CTPopupMenuWnd::m_bMenuHighlightRarely = TRUE;
BOOL CTPopupMenuWnd::m_bMenuExpandAnimation = TRUE;
BOOL CTPopupBaseWnd::m_bUseDesktopWorkArea = TRUE;
BOOL CTPopupBaseWnd::m_bEnableOnIdleCalls = FALSE;
BOOL CTPopupMenuWnd::m_bMenuShowCoolTips = TRUE;
BOOL CTPopupMenuWnd::m_bMenuWithShadows = TRUE;
BOOL CTPopupMenuWnd::m_bMenuExpanding = TRUE;

CTPopupBaseWnd::e_animation_type CTPopupBaseWnd::m_DefAnimationType = CTPopupBaseWnd::__AT_FADE;
CTPopupMenuSite CTPopupMenuSite::m_DefPopupMenuSite;

static inline int _GetMenuShadowSize()
{
	if(!CTPopupMenuWnd::m_bMenuWithShadows)
		return 0;

	return g_PaintManager->GetMenuShadowSize();
};

UINT CTPopupBaseWnd::m_nMsgPrepareMenu = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgPrepareMenu"));
UINT CTPopupBaseWnd::m_nMsgPopupNext = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgPopupNext"));
UINT CTPopupBaseWnd::m_nMsgPopupPrev = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgPopupPrev"));
UINT CTPopupBaseWnd::m_nMsgNotifyMenuExpanded = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgNotifyMenuExpanded"));
UINT CTPopupBaseWnd::m_nMsgNotifyMenuClosed = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgNotifyMenuClosed"));
UINT CTPopupBaseWnd::m_nMsgPopupDrawItem = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgPopupDrawItem"));
UINT CTPopupBaseWnd::m_nMsgPopupDrawLeftArea = ::RegisterWindowMessage(_T("CTPopupMenuWnd::m_nMsgPopupDrawLeftArea"));

UINT CTPopupColorMenuWnd::m_nMsgNotifyColorChanged = ::RegisterWindowMessage(_T("CTPopupColorMenuWnd::m_nMsgNotifyColorChanged"));
UINT CTPopupColorMenuWnd::m_nMsgNotifyColorChangedFinally = ::RegisterWindowMessage(_T("CTPopupColorMenuWnd::m_nMsgNotifyColorChangedFinally"));
UINT CTPopupColorMenuWnd::m_nMsgNotifyCustColor = ::RegisterWindowMessage(_T("CTPopupColorMenuWnd::m_nMsgNotifyCustColor"));

CTPopupColorMenuWnd::COLORREF_TABLE_ENTRY CTPopupColorMenuWnd::m_colors[] = {
	{ RGB( 0x00, 0x00, 0x00), _T("Black")},
	{ RGB( 0xA5, 0x2A, 0x00), _T("Brown")},
	{ RGB( 0x00, 0x40, 0x40), _T("Dark Olive Green")},
	{ RGB( 0x00, 0x55, 0x00), _T("Dark Green")},
	{ RGB( 0x00, 0x00, 0x5E), _T("Dark Teal")},
	{ RGB( 0x00, 0x00, 0x8B), _T("Dark blue")},
	{ RGB( 0x4B, 0x00, 0x82), _T("Indigo")},
	{ RGB( 0x28, 0x28, 0x28), _T("Dark grey")},
	{ RGB( 0x8B, 0x00, 0x00), _T("Dark red")},
	{ RGB( 0xFF, 0x68, 0x20), _T("Orange")},
	{ RGB( 0x8B, 0x8B, 0x00), _T("Dark yellow")},
	{ RGB( 0x00, 0x93, 0x00), _T("Green")},
	{ RGB( 0x38, 0x8E, 0x8E), _T("Teal")},
	{ RGB( 0x00, 0x00, 0xFF), _T("Blue")},
	{ RGB( 0x7B, 0x7B, 0xC0), _T("Blue-grey")},
	{ RGB( 0x66, 0x66, 0x66), _T("Grey - 40")},
	{ RGB( 0xFF, 0x00, 0x00), _T("Red")},
	{ RGB( 0xFF, 0xAD, 0x5B), _T("Light orange")},
	{ RGB( 0x32, 0xCD, 0x32), _T("Lime")},
	{ RGB( 0x3C, 0xB3, 0x71), _T("Sea green")},
	{ RGB( 0x7F, 0xFF, 0xD4), _T("Aqua")},
	{ RGB( 0x7D, 0x9E, 0xC0), _T("Light blue")},
	{ RGB( 0x80, 0x00, 0x80), _T("Violet")},
	{ RGB( 0x7F, 0x7F, 0x7F), _T("Grey - 50")},
	{ RGB( 0xFF, 0xC0, 0xCB), _T("Pink")},
	{ RGB( 0xFF, 0xD7, 0x00), _T("Gold")},
	{ RGB( 0xFF, 0xFF, 0x00), _T("Yellow")},
	{ RGB( 0x00, 0xFF, 0x00), _T("Bright green")},
	{ RGB( 0x40, 0xE0, 0xD0), _T("Turquoise")},
	{ RGB( 0xC0, 0xFF, 0xFF), _T("Sky blue")},
	{ RGB( 0x48, 0x00, 0x48), _T("Plum")},
	{ RGB( 0xC0, 0xC0, 0xC0), _T("Light grey")},
	{ RGB( 0xFF, 0xE4, 0xE1), _T("Rose")},
	{ RGB( 0xD2, 0xB4, 0x8C), _T("Tan")},
	{ RGB( 0xFF, 0xFF, 0xE0), _T("Light yellow")},
	{ RGB( 0x98, 0xFB, 0x98), _T("Pale green")},
	{ RGB( 0xAF, 0xEE, 0xEE), _T("Pale turquoise ")},
	{ RGB( 0x68, 0x83, 0x8B), _T("Pale blue")},
	{ RGB( 0xE6, 0xE6, 0xFA), _T("Lavender")},
	{ RGB( 0xFF, 0xFF, 0xFF), _T("White")}};

#define __POPUP_WNDCLASS_STYLES__	(CS_SAVEBITS)


/////////////////////////////////////////////////////////////////////////////
// CTSoundPlayer

CTSoundPlayer::CTSoundPlayerAutoPtr g_SoundPlayer;
IMPLEMENT_DYNAMIC( CTSoundPlayer, CObject);

CEvent CTSoundPlayer::SoundThredadParms::m_EventPlaySnd( TRUE, FALSE, _T("CTSoundPlayer__SoundThredadParms"));
CCriticalSection CTSoundPlayer::SoundThredadParms::m_csPlaySnd;


DWORD CTSoundPlayer::SoundThredadParms::ParallelPlayerProc( CTSoundPlayer::SoundThredadParms *pParms)
{
	pParms->PlaySound();
	m_EventPlaySnd.SetEvent();

	return 0;
}

void CTSoundPlayer::SoundThredadParms::PlaySound()
{
	CString strSoundSpecBuffer(m_strSoundSpecBuffer);
	HANDLE hModule = m_hModule;
	DWORD dwPlayerFlags = m_dwPlayerFlags;

	if(m_pEventInitDone)
		m_pEventInitDone->SetInitDone();

	::PlaySound(
		LPCTSTR(strSoundSpecBuffer),
		(HMODULE) hModule,
		dwPlayerFlags);

	if(m_dwPlayerFlags&SND_SYNC)
		::PlaySound( NULL, NULL, SND_PURGE);
}

void CTSoundPlayer::SoundThredadParms::PlaySoundParallel()
{
	CSingleLock slCS(&m_csPlaySnd);
	slCS.Lock();

	HANDLE hEvent = (HANDLE) m_EventPlaySnd;
	if( ::WaitForSingleObject(hEvent,0) == WAIT_OBJECT_0 )
	{
		SoundEventInitDone EventInitDone;

		m_pEventInitDone = &EventInitDone;
		m_hThread = ::CreateThread(
			NULL, 0,
			*this,
			this,
			CREATE_SUSPENDED,
			&m_dwThreadID);

		if( m_hThread != NULL )
		{
			::SetThreadPriority(
				m_hThread,
				THREAD_PRIORITY_BELOW_NORMAL);

			::ResumeThread(m_hThread);
			EventInitDone.WaitInitDone();
		}
		else
			m_EventPlaySnd.SetEvent();
	}

	slCS.Unlock();
}

CTSoundPlayer::CTSoundPlayerAutoPtr::CTSoundPlayerAutoPtr()
{
	m_pPlayer = new CTSoundPlayer;
}

CTSoundPlayer::CTSoundPlayerAutoPtr::~CTSoundPlayerAutoPtr()
{
	if( m_pPlayer != NULL )
		delete m_pPlayer;
}

void CTSoundPlayer::CTSoundPlayerAutoPtr::InstallSoundPlayer( CTSoundPlayer *pPlayer)
{
	if( m_pPlayer != NULL )
		delete m_pPlayer;

	m_pPlayer = pPlayer;
}

CTSoundPlayer::CTSoundPlayer()
{
}

CTSoundPlayer::~CTSoundPlayer()
{
}

void CTSoundPlayer::PlaySound( CTSoundPlayer::e_ui_sounds nSoundID)
{
	switch(nSoundID)
	{
	case __NO_SOUND					: return;
	case __ON_MENU_EXPAND_CLICKED	:
	case __ON_MENU_POPUP_DISPLAYED	:
		{
			SoundThredadParms _istp(_T("MenuPopup"));
			_istp.PlaySoundParallel();

			return;
		}

		break;

	case __ON_MENU_CMD_CLICKED		:
		{
			SoundThredadParms _istp(_T("MenuCommand"));
			_istp.PlaySoundParallel();

			return;
		}

		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuSite

CTPopupMenuSite::CTPopupMenuSite() : m_pTopMenu(NULL),
	m_pWndCapture(NULL),
	m_pWndAnimation(NULL),
	m_bShutdownMode(FALSE),
	m_hMouseHook(NULL),
	m_hKeyboardHook(NULL),
	m_lpnResultCmdID(NULL)
{
}

CTPopupMenuSite::~CTPopupMenuSite()
{
	_Done();
}

CTPopupMenuWnd *CTPopupMenuSite::GetCapture()
{
	return m_pWndCapture;
}

void CTPopupMenuSite::SetCapture( CTPopupMenuWnd *pNewPopup)
{
	m_pWndCapture = pNewPopup;
}

void CTPopupMenuSite::SetInstance( CTPopupMenuWnd *pTopMenu)
{
	if(!IsEmpty())
	{
		if( m_pTopMenu == pTopMenu )
			return;

		if( m_pTopMenu != pTopMenu )
			_Done();
	}

	if( pTopMenu != NULL )
	{
		m_pTopMenu = pTopMenu;
		_Hook(TRUE);
	}
}

CTPopupMenuWnd *CTPopupMenuSite::GetAnimated()
{
	return m_pWndAnimation;
}

void CTPopupMenuSite::SetAnimated( CTPopupMenuWnd *pNewPopup)
{
	m_pWndAnimation = pNewPopup;
}

CTPopupMenuWnd *CTPopupMenuSite::GetInstance()
{
	return m_pTopMenu;
}

void CTPopupMenuSite::SetTargetCmdIdPtr( UINT *lpnResultCmdID)
{
	m_lpnResultCmdID = lpnResultCmdID;
}

UINT *CTPopupMenuSite::GetTargetCmdIdPtr()
{
	return m_lpnResultCmdID;
}

void CTPopupMenuSite::DoneInstance()
{
	_Done();
}

BOOL CTPopupMenuSite::IsTopPupup( CTPopupMenuWnd *pTopMenu) const
{
	return pTopMenu == m_pTopMenu;
}

void CTPopupMenuSite::_Done()
{
	m_bShutdownMode = TRUE;
	m_pWndCapture = NULL;
	m_pWndAnimation = NULL;
	m_lpnResultCmdID = NULL;

	if( m_pTopMenu != NULL )
	{
		m_pTopMenu->_BuildItems( NULL, TRUE);

		if( m_pTopMenu->GetSafeHwnd() != NULL &&
			::IsWindow(m_pTopMenu->GetSafeHwnd()))
		{
			m_pTopMenu->DestroyWindow();
			CTPopupMenuWnd::PassMsgLoop(CTPopupBaseWnd::m_bEnableOnIdleCalls);
		}

		delete m_pTopMenu;
		m_pTopMenu = NULL;
	}

	_Hook(FALSE);
	m_bShutdownMode = FALSE;
}

LRESULT CALLBACK CTPopupMenuSite::_HookMouseProc( int nCode,
												  WPARAM wParam,
												  LPARAM lParam)
{
	MOUSEHOOKSTRUCT* lpMS = (MOUSEHOOKSTRUCT *) lParam;

	if( !m_DefPopupMenuSite.IsEmpty() &&
		!m_DefPopupMenuSite.IsShutdownMode())
	{
		CTPopupMenuWnd *pWndCapture = m_DefPopupMenuSite.GetCapture();

		if( pWndCapture != NULL && ::IsWindow(pWndCapture->GetSafeHwnd()) )
		{
			switch(wParam)
			{
			case WM_MOUSEMOVE			:
				{
					CPoint pt(lpMS->pt);
					pWndCapture->ScreenToClient(&pt);

					if(pWndCapture->_OnMouseMove( wParam, pt))
						return 1;
				}

				break;

			case WM_MOUSEWHEEL			: return 1;
			case WM_NCLBUTTONDOWN		:
			case WM_NCRBUTTONDOWN		:
			case WM_NCMBUTTONDOWN		:
			case WM_LBUTTONDOWN			:
			case WM_RBUTTONDOWN			:
			case WM_MBUTTONDOWN			:
			case WM_NCMBUTTONUP			:
			case WM_MBUTTONUP			:
			case WM_NCRBUTTONUP			:
			case WM_RBUTTONUP			:
				{
					CPoint pt(lpMS->pt);
					CTPopupMenuWnd *pPopup = pWndCapture;

					for(; pPopup; pPopup = pPopup->m_pWndParentMenu)
					{
						CRect rcMenuFrame;

						pPopup->GetWindowRect(&rcMenuFrame);
						if(rcMenuFrame.PtInRect(pt))
							return 1;
					}

					pPopup = CTPopupMenuSite::m_DefPopupMenuSite.GetInstance();
					pPopup->_OnCancelMode();

					if( wParam != WM_LBUTTONDOWN &&
						wParam != WM_RBUTTONDOWN &&
						wParam != WM_MBUTTONDOWN)
						return 1;
				}

				break;

			case WM_NCLBUTTONUP			:
			case WM_LBUTTONUP			:
				{
					CPoint pt(lpMS->pt);
					pWndCapture->ScreenToClient(&pt);

					if(pWndCapture->_OnMouseClick( wParam, pt))
						return 1;
				}

				break;
			}
		}
		else
			return 1;
	}

	return ::CallNextHookEx(
		m_DefPopupMenuSite.m_hMouseHook,
		nCode,
		wParam,
		lParam);
}

LRESULT CALLBACK CTPopupMenuSite::_HookKeyboardProc( int nCode,
													 WPARAM wParam,
													 LPARAM lParam)
{
	if( !m_DefPopupMenuSite.IsEmpty() && !m_DefPopupMenuSite.IsShutdownMode() )
	{
		CTPopupMenuWnd *pWndCapture = m_DefPopupMenuSite.GetCapture();

		if( pWndCapture->GetSafeHwnd() != NULL &&
			::IsWindow(pWndCapture->GetSafeHwnd()))
		{
			if( (lParam & (1 << 31)) == 0 )
			{
				static clock_t _clock_step = CLOCKS_PER_SEC / 50;
				static clock_t _clock_last = 0;

				clock_t _clock_curr = clock();
				clock_t _clock_diff = abs(_clock_curr - _clock_last);

				if( _clock_diff >= _clock_step )
				{
					_clock_last = _clock_curr;
					if(pWndCapture->_OnKeyDown( wParam, LOWORD(lParam), HIWORD(lParam)))
						return 1;

					if(pWndCapture)
						return 1;
				}
			}
		}
	}

	if( !CTToolControlBar::m_bMenuTracking &&
		CTPopupMenuWnd::IsCmdKeyActivation(lParam) &&
		(lParam & (1 << 31)) != 0 )
	{
		CTPopupMenuWnd::CancelMenuTracking();

		return ::CallNextHookEx(
			m_DefPopupMenuSite.m_hKeyboardHook,
			nCode,
			wParam,
			lParam);
	}

	return ::CallNextHookEx(
		m_DefPopupMenuSite.m_hKeyboardHook,
		nCode,
		wParam,
		lParam);
}

void CTPopupMenuSite::_Hook( BOOL bHook)
{
	if(bHook)
	{
		if( m_hMouseHook == NULL )
		{
			m_hMouseHook = ::SetWindowsHookEx(
				WH_MOUSE,
				_HookMouseProc, 0,
				::GetCurrentThreadId());
		}

		if( m_hKeyboardHook == NULL )
		{
			m_hKeyboardHook = ::SetWindowsHookEx(
				WH_KEYBOARD,
				_HookKeyboardProc, 0,
				::GetCurrentThreadId());
		}
	}
	else
	{
		if( m_hMouseHook != NULL )
		{
			::UnhookWindowsHookEx(m_hMouseHook);
			m_hMouseHook = NULL;
		}

		if( m_hKeyboardHook != NULL )
		{
			::UnhookWindowsHookEx(m_hKeyboardHook);
			m_hKeyboardHook = NULL;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTWndShadow

CTWndShadow::CTWndShadow()
{
	Destroy();
}

CTWndShadow::~CTWndShadow()
{
	Destroy();
}

void CTWndShadow::_DoPixelOvershadow( int nMakeSpec, int nPosX, int nPosY)
{
	int nTotalHeight = m_rcWndArea.Height() + m_nShadowSize;
	int nTotalWidth = m_rcWndArea.Width() + m_nShadowSize;

	COLORREF *ptr = m_pHelperDibSurface + nPosX + (nTotalHeight - nPosY) * nTotalWidth;
	*ptr = RGB(
		nMakeSpec * int(GetRValue(*ptr)) / 100,
		nMakeSpec * int(GetGValue(*ptr)) / 100,
		nMakeSpec * int(GetBValue(*ptr)) / 100);
}

BOOL CTWndShadow::Restore( CDC &dc)
{
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty() )
		return TRUE;

	if( m_bmp0.GetSafeHandle() == NULL ||
		m_bmp1.GetSafeHandle() == NULL )
		return FALSE;

	int nWndAreaDX = m_rcWndArea.Width();
	int nWndAreaDY = m_rcWndArea.Height();
	CDC dcmm;

	if(!dcmm.CreateCompatibleDC(&dc))
		return FALSE;

	CBitmap *pbmpold = dcmm.SelectObject(&m_bmp0);
	dc.BitBlt(
		m_rcWndArea.right,
		m_rcWndArea.top,
		m_nShadowSize,
		nWndAreaDY + m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY);

	dcmm.SelectObject(&m_bmp1);
	dc.BitBlt(
		m_rcWndArea.left,
		m_rcWndArea.bottom,
		nWndAreaDX + m_nShadowSize,
		m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY);

	dcmm.SelectObject(pbmpold);
	return TRUE;
}

BOOL CTWndShadow::Paint( CDC &dc,
						 const CRect &rcWndArea,
						 const CRect &rc1stArea,
						 const CRect &rc2ndArea,
						 UINT nShadowSize,
						 UINT nBr0,
						 UINT nBr1,
						 BOOL bEnablePhotos)
{
	m_rcWndArea = rcWndArea;
	m_rc1stArea = rc1stArea;
	m_rc2ndArea = rc2ndArea;
	m_nShadowSize = nShadowSize;
	m_nBr0 = nBr0;
	m_nBr1 = nBr1;
	m_bEnablePhotos = bEnablePhotos;

	return Paint(dc);
}

BOOL CTWndShadow::Paint( CDC &dc)
{
	if( m_nShadowSize == 0 || m_rcWndArea.IsRectEmpty() )
		return TRUE;

	if(Restore(dc))
		return TRUE;

	if( CTPaintManager::GetBPP() > 8 )
		return _PaintHi(dc);

	return _PaintLo(dc);
}

BOOL CTWndShadow::_PaintLo( CDC &dc)
{
	int nWndAreaDY = m_rcWndArea.Height();
	int nWndAreaDX = m_rcWndArea.Width();

	static int _Pattern[] = {
		~0xAA,
		~0x55,
		~0xAA,
		~0x55,
		~0xAA,
		~0x55,
		~0xAA,
		~0x55};

	CBitmap bmpsh;
	CBrush brsh;

	if( !bmpsh.CreateBitmap( 8, 8, 1, 1, _Pattern) ||
		!brsh.CreatePatternBrush(&bmpsh))
		return FALSE;

	CTMemoryDC dcmm(
		&dc,
		NULL,
		CTMemoryDC::MDCOPT_TO_MEMORY);

	if( dcmm.GetSafeHdc() == NULL )
		return FALSE;

	CPalette *pOldPalette = NULL;
	if(dcmm.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)
	{
		pOldPalette = dcmm.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		dcmm.RealizePalette();
	}

	dcmm.BitBlt(
		m_rcWndArea.left,
		m_rcWndArea.top,
		nWndAreaDX + m_nShadowSize,
		nWndAreaDY + m_nShadowSize,
		&dc,
		m_rcWndArea.left,
		m_rcWndArea.top,
		SRCCOPY);

	UINT nPaintShadowSize = m_nShadowSize > 4 ? 4 : m_nShadowSize;
	CRect rcV(
		m_rcWndArea.right,
		m_rcWndArea.top + nPaintShadowSize,
		m_rcWndArea.right + nPaintShadowSize,
		m_rcWndArea.bottom);

	CRect rcH(
		m_rcWndArea.left + nPaintShadowSize,
		m_rcWndArea.bottom,
		m_rcWndArea.right + nPaintShadowSize,
		m_rcWndArea.bottom + nPaintShadowSize);

	CBrush *pbrold = dcmm.SelectObject(&brsh);
	dcmm.PatBlt( rcH.left, rcH.top, rcH.Width(), rcH.Height(), 0xA000C9);
	dcmm.PatBlt( rcV.left, rcV.top, rcV.Width(), rcV.Height(), 0xA000C9);
	dcmm.SelectObject(pbrold);

	dc.BitBlt(
		m_rcWndArea.left,
		m_rcWndArea.top,
		nWndAreaDX + m_nShadowSize,
		nWndAreaDY + m_nShadowSize,
		&dcmm,
		m_rcWndArea.left,
		m_rcWndArea.top,
		SRCCOPY);

	if( m_bEnablePhotos && !_MakePhotos( dc, dcmm) )
		return FALSE;

	if( pOldPalette != NULL )
		dcmm.SelectPalette( pOldPalette, FALSE);

	dcmm.__Flush(FALSE);
	return TRUE;
}

BOOL CTWndShadow::_PaintHi( CDC &dc)
{
	int nWndAreaDY = m_rcWndArea.Height();
	int nWndAreaDX = m_rcWndArea.Width();

	CDC dcmm;
	if(!dcmm.CreateCompatibleDC(&dc))
		return FALSE;

	BITMAPINFOHEADER bih;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = nWndAreaDX+m_nShadowSize;
	bih.biHeight = nWndAreaDY+m_nShadowSize;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = (nWndAreaDX+m_nShadowSize) * (nWndAreaDY+m_nShadowSize);
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	HBITMAP hDIB = ::CreateDIBSection(
		dcmm.GetSafeHdc(),
		(LPBITMAPINFO) &bih,
		DIB_RGB_COLORS,
		(void **) &m_pHelperDibSurface,
		NULL,
		NULL);

	if( hDIB == NULL || m_pHelperDibSurface == NULL )
		return FALSE;

	dcmm.SelectObject(hDIB);
	dcmm.BitBlt(
		0, 0,
		nWndAreaDX + m_nShadowSize,
		nWndAreaDY + m_nShadowSize,
		&dc,
		m_rcWndArea.left,
		m_rcWndArea.top,
		SRCCOPY);

	UINT nBrDiff = m_nBr1 - m_nBr0;
	LONG n2nd;
	LONG nStep;
	LONG nDist;
	LONG nDist1;
	LONG nDist2 = m_nShadowSize * m_nShadowSize;
	int nMakeSpec;

	BOOL bCmbaV = FALSE;
	BOOL bCmbaH = FALSE;
	BOOL bCmbaVrt = FALSE;
	BOOL bCmbaVrb = FALSE;
	BOOL bCmbaHrb = FALSE;
	BOOL bCmbaHlb = FALSE;
	BOOL bCmbaLA = FALSE;

	int nX0 = 0;
	int nX1 = nWndAreaDX;
	int nY0 = 0;
	int nY1 = nWndAreaDY;
	int nLa0 = 0;
	int nLa1 = 0;

	if( !m_rc1stArea.IsRectEmpty() && !m_rc2ndArea.IsRectEmpty() )
	{
		BOOL bCmbaDetected = FALSE;

		if( m_rc1stArea.right == m_rc2ndArea.right )
		{
			bCmbaDetected = TRUE;

			if( m_rc1stArea.top < m_rc2ndArea.top )
				bCmbaVrb = TRUE;
			else
				bCmbaVrt = TRUE;
		}

		if( !bCmbaDetected && m_rc1stArea.left == m_rc2ndArea.left )
		{
			if( m_rc2ndArea.bottom >= m_rc1stArea.top - 1 &&
				m_rc2ndArea.top < m_rc1stArea.top &&
				m_rc2ndArea.right >= m_rc1stArea.right )
			{
				bCmbaDetected = TRUE;
				bCmbaV = TRUE;
			}

			if( !bCmbaDetected &&
				m_rc1stArea.bottom >= m_rc2ndArea.top - 1 &&
				m_rc1stArea.top < m_rc2ndArea.top &&
				m_rc1stArea.right >= m_rc2ndArea.right )
			{
				bCmbaDetected = TRUE;
				bCmbaH = TRUE;
				nX0 = m_rcWndArea.right - (m_rc1stArea.right - m_rc2ndArea.right);
			}
		}

		if( !bCmbaDetected && m_rc1stArea.bottom == m_rc2ndArea.bottom )
		{
			bCmbaDetected = TRUE;

			if( m_rc1stArea.left < m_rc2ndArea.left )
				bCmbaHrb = TRUE;
			else
				bCmbaHlb = TRUE;
		}

		if( !bCmbaDetected && m_rc1stArea.right == m_rc2ndArea.left + 1 )
		{
			bCmbaDetected = TRUE;
			bCmbaLA = TRUE;

			nLa0 = m_rcWndArea.top + m_rc2ndArea.bottom - m_rc1stArea.top;
			nLa1 = nLa0 + m_nShadowSize;
		}

		if( !bCmbaDetected && m_rc1stArea.left == m_rc2ndArea.right - 1 )
		{
			bCmbaDetected = TRUE;
			bCmbaH = TRUE;
		}
	}

	for( nStep = 0; ULONG(nStep) < m_nShadowSize; nStep++)
	{
		nMakeSpec = m_nBr0 + (nStep * nBrDiff) / m_nShadowSize;

		for( n2nd = nX0 + m_nShadowSize * 2 + 1 - (bCmbaH ? m_nShadowSize * 2 - nStep : 0) - (bCmbaHlb ? m_nShadowSize : 0); n2nd < LONG(nX1) + (bCmbaHrb ? LONG(m_nShadowSize) : 0); n2nd++)
		{
			_DoPixelOvershadow(
				nMakeSpec,
				n2nd,
				nY1 + nStep);
		}

		for( n2nd = nY0 + m_nShadowSize * 2 + 1 - (bCmbaV ? m_nShadowSize * 2 - nStep : 0) - (bCmbaVrt ? m_nShadowSize : 0); n2nd < LONG(nY1) + (bCmbaVrb ? LONG(m_nShadowSize) : 0); n2nd++)
		{
			if( bCmbaLA && n2nd >= nLa0 && n2nd < LONG(nLa1) - (LONG(m_nShadowSize) - nStep) )
				continue;

			_DoPixelOvershadow(
				nMakeSpec,
				nX1 + nStep,
				n2nd);
		}

		nDist1 = nStep * nStep;
		for( n2nd = 0; ULONG(n2nd) < m_nShadowSize; n2nd++)
		{
			nDist = nDist1 + n2nd * n2nd;
			if( nDist > nDist2 )
				continue;

			nDist = (LONG)::sqrt(double(nDist));
			nMakeSpec = m_nBr0 + (nDist * nBrDiff) / m_nShadowSize;

			if( !bCmbaVrb && !bCmbaHrb )
			{
				_DoPixelOvershadow(
					nMakeSpec,
					nX1 + nStep,
					nY1 + n2nd);
			}

			if( !bCmbaH && !bCmbaHlb )
			{
				_DoPixelOvershadow(
					nMakeSpec,
					m_nShadowSize + (m_nShadowSize - nStep),
					nY1 + n2nd);
			}

			if( !bCmbaV && !bCmbaVrt )
			{
				_DoPixelOvershadow(
					nMakeSpec,
					nX1 + nStep,
					m_nShadowSize + (m_nShadowSize - n2nd));
			}
		}
	}

	dc.BitBlt(
		m_rcWndArea.left,
		m_rcWndArea.top,
		nWndAreaDX + m_nShadowSize,
		nWndAreaDY + m_nShadowSize,
		&dcmm,
		0, 0,
		SRCCOPY);

	if( m_bEnablePhotos && !_MakePhotos( dc, dcmm) )
		return FALSE;

	::DeleteObject(hDIB);
	m_pHelperDibSurface = NULL;

	return TRUE;
}

BOOL CTWndShadow::_MakePhotos( CDC &dc, CDC &dcmm)
{
	int nWndAreaDY = m_rcWndArea.Height();
	int nWndAreaDX = m_rcWndArea.Width();

	if(!m_bmp0.CreateCompatibleBitmap( &dc, m_nShadowSize, nWndAreaDY + m_nShadowSize))
		return FALSE;
	dcmm.SelectObject(&m_bmp0);

	if(!dcmm.BitBlt( 0, 0, m_nShadowSize, nWndAreaDY + m_nShadowSize, &dc, m_rcWndArea.right, m_rcWndArea.top, SRCCOPY))
		return FALSE;

	if(!m_bmp1.CreateCompatibleBitmap( &dc, nWndAreaDX + m_nShadowSize, m_nShadowSize))
		return FALSE;

	dcmm.SelectObject(&m_bmp1);
	if(!dcmm.BitBlt( 0, 0, nWndAreaDX + m_nShadowSize, m_nShadowSize, &dc, m_rcWndArea.left, m_rcWndArea.bottom, SRCCOPY))
		return FALSE;

	return TRUE;
}

void CTWndShadow::_FreeWinObjects()
{
	if( m_bmp0.GetSafeHandle() != NULL )
		m_bmp0.DeleteObject();

	if( m_bmp1.GetSafeHandle() != NULL )
		m_bmp1.DeleteObject();

	m_pHelperDibSurface = NULL;
}

void CTWndShadow::Destroy()
{
	_FreeWinObjects();
	m_rcWndArea.SetRectEmpty();

	m_bEnablePhotos = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuWnd::MENUITEMDATA

CTPopupMenuWnd::MENUITEMDATA::MENUITEMDATA()
{
	m_pWndChild = NULL;
	m_nItemIndex = -100;
	m_nCmdID = TYPE_SEPARATOR;
	m_strPopupText = _T("");
	m_strPopupAccelText = _T("");
	m_hIconPopup = NULL;
	m_size.cx = 0;
	m_size.cy = 0;

	m_bSelected = FALSE;
	m_bDisplayed = FALSE;
	m_bForceDisplayed = FALSE;
	m_cAccelChar = 0;
	m_hWndSpecCmdReciever = NULL;
}

CTPopupMenuWnd::MENUITEMDATA::~MENUITEMDATA()
{
}

void CTPopupMenuWnd::MENUITEMDATA::AccelCharInit()
{
	m_cAccelChar = 0;
	CString strDisplayText(GetText());

	if(strDisplayText.IsEmpty())
		return;

	int nIndex = strDisplayText.Find(_T('&'));
	if( nIndex + 1 == strDisplayText.GetLength() )
		return;

	if( nIndex < 0 )
	{
		if(!CTPopupMenuWnd::m_bAllowNonAccelPositioning)
			return;

		m_cAccelChar = strDisplayText[0];
	}
	else
		m_cAccelChar = strDisplayText[nIndex + 1];

	TCHAR szChar[2] = { m_cAccelChar, _T('\0')};
	::CharUpper(szChar);
	m_cAccelChar = szChar[0];
}

TCHAR CTPopupMenuWnd::MENUITEMDATA::AccelCharGet() const
{
	return m_cAccelChar;
}

BOOL CTPopupMenuWnd::MENUITEMDATA::AccelCharIsSet() const
{
	return m_cAccelChar != 0;
}

BOOL CTPopupMenuWnd::MENUITEMDATA::UpdateCmdManagerCommand( CTCmdManager::cmd *p_cmd, int nItemIndex)
{
	m_strPopupText.Empty();
	m_nCmdID = p_cmd->m_nCmdID;

	if(!CTCmdManager::IsCommand(m_nCmdID))
	{
		m_nCmdID = TYPE_SEPARATOR;
		m_nItemIndex = -100;

		return FALSE;
	}

	if( nItemIndex >= 0 )
		m_nItemIndex = nItemIndex;

	MeasureItem();
	return TRUE;
}

CTCmdManager::cmd *CTPopupMenuWnd::MENUITEMDATA::GetCmd() const
{
	if( IsPopup() || IsSeparator() )
		return NULL;

	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(GetCmdReciever()),
		m_nCmdID);

	return p_cmd;
}

void CTPopupMenuWnd::MENUITEMDATA::MeasureItem()
{
	AccelCharInit();

	if(IsSeparator())
	{
		m_size.cy = __DEF_MENU_SEPARATOR_HEIGHT;
		m_size.cx = 0;

		return;
	}

	CString strMeasureText(GetText());
	strMeasureText += GetAccelText();
	strMeasureText.Replace( _T("&"), _T(""));

	CWindowDC dc(NULL);
	CFont *pOldFont = dc.SelectObject(&g_PaintManager->m_FontNormal);
	CRect rect( 0, 0, 0, 0);

	m_size.cy = DrawText(
		dc.GetSafeHdc(),
		(LPCTSTR) strMeasureText,
		strMeasureText.GetLength(),
		&rect,
		DT_CALCRECT|
		DT_SINGLELINE|
		DT_LEFT|
		DT_VCENTER);
	m_size.cx = rect.Width();
	dc.SelectObject(pOldFont);

	m_size.cx += __DEF_MENU_ICON_CX + __DEF_MENU_GAP * 6 + __DEF_MENU_POPUP_ARROW_AREA_DX;
	if( m_size.cy < __DEF_MENU_HEIGHT )
		m_size.cy = __DEF_MENU_HEIGHT;
}

BOOL CTPopupMenuWnd::MENUITEMDATA::UpdateFromMenu( HWND hWndCmdProfileInfo,
												   CMenu *pTrackMenu,
												   int nItemIndex)
{
	MENUITEMINFO mii;

	::memset( &mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_CHECKMARKS|MIIM_DATA|MIIM_ID|MIIM_STATE|MIIM_SUBMENU|MIIM_TYPE;
	mii.cch = __MAX_UI_ITEM_TEXT;
	mii.dwTypeData = m_strPopupText.GetBuffer(__MAX_UI_ITEM_TEXT);

	if( mii.dwTypeData == NULL )
		return FALSE;

	if(!pTrackMenu->GetMenuItemInfo( nItemIndex, &mii, TRUE))
	{
		m_strPopupText.ReleaseBuffer();
		return FALSE;
	}
	m_strPopupText.ReleaseBuffer();

	m_nItemIndex = nItemIndex;
	if(mii.fType&MFT_SEPARATOR)
	{
		m_strPopupText.Empty();
		m_nCmdID = TYPE_SEPARATOR;
		MeasureItem();

		return TRUE;
	}

	if( mii.hSubMenu != NULL )
	{
		if(!ConstructPopup())
			return FALSE;

		m_nCmdID = (UINT) TYPE_POPUP;
		MeasureItem();

		return TRUE;
	}

	m_strPopupText.Empty();
	if(!CTCmdManager::IsCommand(mii.wID))
		return FALSE;

	m_nCmdID = mii.wID;
	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(hWndCmdProfileInfo),
		m_nCmdID);

	if( p_cmd == NULL )
		return FALSE;
	MeasureItem();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTPopupBaseWnd

IMPLEMENT_DYNCREATE( CTPopupBaseWnd, CWnd)

CTPopupBaseWnd::CTPopupBaseWnd()
{
	m_clrFadeBitsSrc = NULL;
	m_clrFadeBitsDst = NULL;
	m_clrFadeBitsTmp = NULL;

	m_AnimationType = m_DefAnimationType;
	_InitAnimation();

	m_nPlaySoundOnAnimationFinished = CTSoundPlayer::__NO_SOUND;
	m_szFullItems.cx = 1;
	m_szFullItems.cy = 1;

	m_bExcludeAreaSpec = FALSE;
	m_bCombineWithEA = FALSE;

	m_rcExcludeArea.SetRectEmpty();
	m_ptTrack.x = m_ptTrack.y;
}

CTPopupBaseWnd::~CTPopupBaseWnd()
{
}

void CTPopupBaseWnd::_InitAnimation()
{
	m_nAnimPercentOld = 0;
	m_nAnimPercent = 0;

	if((m_AnimationType < 0 ||
		m_AnimationType > __AT_MAX_VALID_VALUE) &&
		m_AnimationType != __AT_CONTENT_DISPLAY &&
		m_AnimationType != __AT_CONTENT_EXPAND )
		m_AnimationType = __AT_NONE;

	if( m_AnimationType == __AT_RANDOM )
	{
		int nRand = rand() % (__AT_MAX_VALID_VALUE + 1);

		if( nRand == __AT_NONE || nRand == __AT_RANDOM )
			nRand = __AT_FADE;

		m_AnimationType = (e_animation_type) nRand;
	}

	int nBitsPerPixel = CTPaintManager::GetBPP();
	if( nBitsPerPixel <= 8 &&
		m_AnimationType != __AT_CONTENT_EXPAND &&
		m_AnimationType != __AT_CONTENT_DISPLAY )
	{
		m_AnimationType = __AT_NONE;
		m_bAnimFinished = TRUE;
	}
	else
		m_bAnimFinished = m_AnimationType == __AT_NONE;
}

void CTPopupBaseWnd::_StartAnimation()
{
	_InitAnimation();

	if( m_AnimationType != __AT_NONE )
	{
		SetTimer( ID_TIMER_ANIMATION, ID_PERIOD_ANIMATION, NULL);
		g_nLastAnimTime = clock();
	}
}

void CTPopupMenuWnd::_StartAnimation()
{
	_ItemFocusDelay();
	_CoolTipHide(FALSE);

	CTPopupBaseWnd::_StartAnimation();
	GetSite().SetAnimated( m_bAnimFinished ? NULL : this);
}

BEGIN_MESSAGE_MAP( CTPopupBaseWnd, CWnd)
	//{{AFX_MSG_MAP(CTPopupBaseWnd)
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_ACTIVATEAPP()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTPopupBaseWnd::OnPaint()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	if(!::IsWindowVisible(GetSafeHwnd()))
		return;

	CPaintDC dc(this);
	if(!m_bAnimFinished)
		_DrawAnimatedState(dc);
	else
		_DoPaint(dc);
}

void CTPopupBaseWnd::_DrawAnimatedState( CDC &dc)
{
	_DoPaint(dc);
}

void CTPopupMenuWnd::_DrawAnimatedState( CDC &dc)
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	CPalette *pOldPalette = NULL;
	CDC *pPaintDC = &dc;

	if( pPaintDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		pOldPalette = pPaintDC->SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		pPaintDC->RealizePalette();
	}

	BOOL bFail = FALSE;
	CDC dcmm;

	dcmm.CreateCompatibleDC(pPaintDC);
	if(!m_bmpScreenDst.GetSafeHandle())
	{
		int nBitsPerPixel = CTPaintManager::GetBPP();
		CBitmap *pBmpOld = NULL;

		if((m_AnimationType == __AT_FADE &&
			nBitsPerPixel > 8 ) ||
			m_AnimationType == __AT_CONTENT_EXPAND ||
			m_AnimationType == __AT_CONTENT_DISPLAY ||
			m_AnimationType == __AT_ROLL ||
			m_AnimationType == __AT_SLIDE ||
			m_AnimationType == __AT_ROLL_AND_STRETCH ||
			m_AnimationType == __AT_SLIDE_AND_STRETCH ||
			m_AnimationType == __AT_NOISE ||
			m_AnimationType == __AT_BOXES ||
			m_AnimationType == __AT_CIRCLES ||
			m_AnimationType == __AT_HOLES )
		{
			BITMAPINFOHEADER bih;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biWidth = cx;
			bih.biHeight = cy;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = cx * cy;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;

			HBITMAP hDIB = ::CreateDIBSection(
				dcmm.GetSafeHdc(),
				(LPBITMAPINFO) &bih,
				DIB_RGB_COLORS,
				(void **) &m_clrFadeBitsSrc,
				NULL,
				NULL);

			if( hDIB != NULL && m_clrFadeBitsSrc != NULL )
			{
				m_bmpScreenSrc.Attach(hDIB);

				hDIB = ::CreateDIBSection(
					dcmm.GetSafeHdc(),
					(LPBITMAPINFO) &bih,
					DIB_RGB_COLORS,
					(void **) &m_clrFadeBitsDst,
					NULL,
					NULL);

				if( hDIB != NULL && m_clrFadeBitsDst != NULL )
				{
					m_bmpScreenDst.Attach(hDIB);

					hDIB = ::CreateDIBSection(
						dcmm.GetSafeHdc(),
						(LPBITMAPINFO) &bih,
						DIB_RGB_COLORS,
						(void **) &m_clrFadeBitsTmp,
						NULL,
						NULL);

					if( hDIB != NULL && m_clrFadeBitsTmp != NULL )
					{
						m_bmpScreenTmp.Attach(hDIB);

						pBmpOld = dcmm.SelectObject(&m_bmpScreenSrc);
						dcmm.BitBlt(
							0, 0,
							cx, cy,
							pPaintDC,
							rcClient.left,
							rcClient.top,
							SRCCOPY);

						::memcpy( m_clrFadeBitsDst, m_clrFadeBitsSrc, sizeof(COLORREF) * cx * cy);
						dcmm.SelectObject(&m_bmpScreenDst);
					}
					else
					{
						m_bmpScreenSrc.DeleteObject();
						m_bmpScreenDst.DeleteObject();
						bFail = TRUE;
					}
				}
				else
				{
					m_bmpScreenSrc.DeleteObject();
					bFail = TRUE;
				}
			}
		}
		else
		{
			m_bmpScreenDst.CreateCompatibleBitmap(
				pPaintDC,
				cx, cy);

			pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
		}

		if(!bFail)
			_DoPaint( dcmm, FALSE);

		dcmm.SetViewportOrg(CPoint( 0, 0));
		dcmm.SelectObject(pBmpOld);
	}

	CBitmap *pBmpOld = NULL;
	if(!bFail)
	{
		COLORREF *src = m_clrFadeBitsSrc;
		COLORREF *dst = m_clrFadeBitsDst;
		COLORREF *tmp = m_clrFadeBitsTmp;

		switch(m_AnimationType)
		{
		case __AT_BOXES					:
		case __AT_CIRCLES				:
		case __AT_HOLES					:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				if( m_nAnimPercentOld != m_nAnimPercent )
				{
					pBmpOld = dcmm.SelectObject(&m_bmpScreenTmp);

					if( m_AnimationType == __AT_BOXES )
					{
						for( int y=0; y<cy; y++)
						{
							int _y0 = y % __BOX_ANIM_METRIC;
							int _yPercent = 100 - (_y0 * 100) / __BOX_ANIM_METRIC;

							if( _yPercent < m_nAnimPercent )
							{
								::memcpy(
									tmp,
									src,
									cx * sizeof(COLORREF));

								tmp += cx;
								src += cx;
								dst += cx;
								continue;
							}

							for( int x=0; x<cx; x++)
							{
								int _x0 = x % __BOX_ANIM_METRIC;
								int _xPercent = (_x0 * 100) / __BOX_ANIM_METRIC;

								if( _xPercent < m_nAnimPercent )
									*tmp = *dst;
								else
									*tmp = *src;

								tmp++;
								src++;
								dst++;
							}
						}
					}
					else
					{
						double radius = 0;

						if( m_AnimationType == __AT_HOLES )
							radius = ((((double) __BOX_ANIM_METRIC) / 2.0f) * (100 - m_nAnimPercent)) / 100.0f;
						else
							radius = ((((double) __BOX_ANIM_METRIC) / 2.0f) * m_nAnimPercent) / 100.0f;

						if( radius > 0 )
						{
							for( int y=0; y<cy; y++)
							{
								int _y0 = y % __BOX_ANIM_METRIC;
								int _y1 = y - _y0;
								int _yCenter = _y1 + __BOX_ANIM_METRIC / 2;
								int _yDist = abs(y - _yCenter);
								double _yDist2 = (double) _yDist * _yDist;

								for( int x=0; x<cx; x++)
								{
									int _x0 = x % __BOX_ANIM_METRIC;
									int _x1 = x - _x0;
									int _xCenter = _x1 + __BOX_ANIM_METRIC / 2;
									int _xDist = abs(x - _xCenter);
									double _xDist2 = (double) _xDist * _xDist;
									double nDispance = ::sqrt(_yDist2 + _xDist2);

									if( m_AnimationType == __AT_HOLES )
									{
										if( nDispance >= radius )
											*tmp = *dst;
										else
											*tmp = *src;
									}
									else if( nDispance < radius )
										*tmp = *dst;
									else
										*tmp = *src;

									tmp++;
									src++;
									dst++;
								}
							}
						}
					}

					int nMenuShadowSize = _GetMenuShadowSize();
					if(m_bCombineWithEA)
					{
						CRect rcExcludeArea(m_rcExcludeArea);

						ScreenToClient(&rcExcludeArea);
						pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
						pPaintDC->BitBlt(
							rcExcludeArea.left,
							rcExcludeArea.top,
							rcExcludeArea.Width(),
							rcExcludeArea.Height(),
							&dcmm,
							rcExcludeArea.left,
							rcExcludeArea.top,
							SRCCOPY);

						dcmm.SelectObject(pBmpOld);
						pBmpOld = NULL;

						if( m_nCombineAlign != __CMBA_NONE )
						{
							rcExcludeArea.InflateRect(
								m_nCombineAlign == __CMBA_RIGHT ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_BOTTOM ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_LEFT ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_TOP ? 0 : nMenuShadowSize);
						}

						pPaintDC->ExcludeClipRect(&rcExcludeArea);
					}

					if( nMenuShadowSize > 0 )
					{
						CRect rcExcludeArea(rcClient);

						rcExcludeArea.left = rcExcludeArea.right - nMenuShadowSize;
						pPaintDC->ExcludeClipRect(&rcExcludeArea);

						rcExcludeArea = rcClient;
						rcExcludeArea.top = rcExcludeArea.bottom - nMenuShadowSize;
						pPaintDC->ExcludeClipRect(&rcExcludeArea);
					}

					pPaintDC->BitBlt(
						rcClient.left,
						rcClient.top,
						cx, cy,
						&dcmm,
						0, 0,
						SRCCOPY);

					if( nMenuShadowSize > 0 || m_bCombineWithEA )
						pPaintDC->SelectClipRgn(NULL);
				}
			}

			break;

		case __AT_NOISE					:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				if( m_nAnimPercentOld != m_nAnimPercent )
				{
					int nDiff = m_nAnimPercent - m_nAnimPercentOld;
					int nRandHalf = nDiff / 2 + 1;

					pBmpOld = dcmm.SelectObject(&m_bmpScreenTmp);
					for( int pixel = 0; pixel < cx * cy; pixel++)
					{
						int nRandValue = rand() % nDiff;
						if( nRandValue > nRandHalf )
							*tmp = *dst;

						tmp++;
						src++;
						dst++;
					}

					int nMenuShadowSize = _GetMenuShadowSize();
					if(m_bCombineWithEA)
					{
						CRect rcExcludeArea(m_rcExcludeArea);

						ScreenToClient(&rcExcludeArea);
						pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);

						pPaintDC->BitBlt(
							rcExcludeArea.left,
							rcExcludeArea.top,
							rcExcludeArea.Width(),
							rcExcludeArea.Height(),
							&dcmm,
							rcExcludeArea.left,
							rcExcludeArea.top,
							SRCCOPY);

						dcmm.SelectObject(pBmpOld);
						pBmpOld = NULL;

						if( m_nCombineAlign != __CMBA_NONE )
						{
							rcExcludeArea.InflateRect(
								m_nCombineAlign == __CMBA_RIGHT ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_BOTTOM ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_LEFT ? 0 : nMenuShadowSize,
								m_nCombineAlign == __CMBA_TOP ? 0 : nMenuShadowSize);
						}

						pPaintDC->ExcludeClipRect(&rcExcludeArea);
					}

					if( nMenuShadowSize > 0 )
					{
						CRect rcExcludeArea(rcClient);

						rcExcludeArea.left = rcExcludeArea.right - nMenuShadowSize;
						pPaintDC->ExcludeClipRect(&rcExcludeArea);

						rcExcludeArea = rcClient;
						rcExcludeArea.top = rcExcludeArea.bottom - nMenuShadowSize;
						pPaintDC->ExcludeClipRect(&rcExcludeArea);
					}

					pPaintDC->BitBlt(
						rcClient.left,
						rcClient.top,
						cx, cy,
						&dcmm,
						0, 0,
						SRCCOPY);

					if( nMenuShadowSize > 0 || m_bCombineWithEA )
						pPaintDC->SelectClipRgn(NULL);
				}
			}

			break;

		case __AT_SLIDE					:
		case __AT_ROLL					:
		case __AT_ROLL_AND_STRETCH		:
		case __AT_SLIDE_AND_STRETCH		:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				CRect _rcClient;

				_GetClientRect(&_rcClient);
				if(m_bCombineWithEA)
				{
					CRect rcExcludeArea(m_rcExcludeArea);

					ScreenToClient(&rcExcludeArea);
					pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);

					pPaintDC->BitBlt(
						rcExcludeArea.left,
						rcExcludeArea.top,
						rcExcludeArea.Width(),
						rcExcludeArea.Height(),
						&dcmm,
						rcExcludeArea.left,
						rcExcludeArea.top,
						SRCCOPY);

					dcmm.SelectObject(pBmpOld);
					pBmpOld = NULL;

					if( m_nCombineAlign != __CMBA_NONE )
					{
						int nMenuShadowSize = _GetMenuShadowSize();

						rcExcludeArea.InflateRect(
							m_nCombineAlign == __CMBA_RIGHT ? 0 : nMenuShadowSize,
							m_nCombineAlign == __CMBA_BOTTOM ? 0 : nMenuShadowSize,
							m_nCombineAlign == __CMBA_LEFT ? 0 : nMenuShadowSize,
							m_nCombineAlign == __CMBA_TOP ? 0 : nMenuShadowSize);
					}

					pPaintDC->ExcludeClipRect(&rcExcludeArea);
				}

				pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
				int cx = _rcClient.Width();
				int cy = _rcClient.Height();

				if( m_AnimationType == __AT_ROLL ||
					m_AnimationType == __AT_SLIDE )
				{
					int _cx = 0;
					if( m_AnimationType == __AT_ROLL )
						_cx = cx * (100 - m_nAnimPercent) / 100;

					int _cy = cy * (100 - m_nAnimPercent) / 100;
					pPaintDC->BitBlt(
						_rcClient.left - _cx,
						_rcClient.top - _cy,
						cx, cy,
						&dcmm,
						_rcClient.left,
						_rcClient.top,
						SRCCOPY);
				}
				else
				{
					int _cx = cx;
					if( m_AnimationType == __AT_ROLL_AND_STRETCH )
						_cx = (cx * m_nAnimPercent) / 100;

					int _cy = (cy * m_nAnimPercent) / 100;
					pPaintDC->StretchBlt(
						_rcClient.left,
						_rcClient.top,
						_cx, _cy,
						&dcmm,
						_rcClient.left,
						_rcClient.top,
						cx, cy,
						SRCCOPY);
				}

				if(m_bCombineWithEA)
					pPaintDC->SelectClipRgn(NULL);
			}

			break;

		case __AT_CONTENT_EXPAND		:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				CRect _rcClient;

				_GetClientRect(&_rcClient);
				if(m_bCombineWithEA)
				{
					CRect rcExcludeArea(m_rcExcludeArea);

					ScreenToClient(&rcExcludeArea);
					pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
					pPaintDC->BitBlt(
						rcExcludeArea.left,
						rcExcludeArea.top,
						rcExcludeArea.Width(),
						rcExcludeArea.Height(),
						&dcmm,
						rcExcludeArea.left,
						rcExcludeArea.top,
						SRCCOPY);

					dcmm.SelectObject(pBmpOld);
					pBmpOld = NULL;
				}

				pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
				int cx = _rcClient.Width();
				int cy = _rcClient.Height();
				int _cx = cx;
				int _cy = (cy * m_nAnimPercent) / 100;
				visible_items v;

				_GetVisibleItems( dcmm, v);
				if( v.GetSize() == 0 )
				{
					pPaintDC->StretchBlt(
						_rcClient.left,
						_rcClient.top,
						_cx, _cy,
						&dcmm,
						_rcClient.left,
						_rcClient.top,
						cx, cy,
						SRCCOPY);
				}
				else
				{
					int y_pos_src = _rcClient.top;
					int y_pos_dst = _rcClient.top;
					int vis_it = 0;
					int nItemIndex = 0;
					int nItemCount = v.GetSize();
					expand_effect_rects_container vRects;

					for(; vis_it < v.GetSize(); vis_it++, nItemIndex++)
					{
						VisibleItemDefinion &vi = v[vis_it];
						CRect rcItemSrc(vi.m_rcItem);

						rcItemSrc.left = _rcClient.left;
						rcItemSrc.right = _rcClient.right;

						if( nItemIndex == 0 )
							rcItemSrc.top = _rcClient.top;
						else if( nItemIndex == nItemCount - 1 )
							rcItemSrc.bottom = _rcClient.bottom;

						int y_pos_src_next = y_pos_src + rcItemSrc.Height();
						CRect rcItemDst(rcItemSrc);

						rcItemDst.OffsetRect( 0, y_pos_dst - rcItemDst.top);
						if(vi.m_bRarelyUsed)
							rcItemDst.bottom = rcItemDst.top + rcItemDst.Height() * m_nAnimPercent / 100;

						int y_pos_dst_next = y_pos_dst + rcItemDst.Height();
						ExpandEffectRects _eert(
							rcItemSrc,
							rcItemDst);

						vRects.Add(_eert);
						y_pos_dst = y_pos_dst_next;
						y_pos_src = y_pos_src_next;
					}

					if( m_bCombineWithEA &&
						m_nCombineAlign == __CMBA_BOTTOM &&
						y_pos_dst != y_pos_src )
					{
						int it_rects = 0;
						int nDstOffset = y_pos_src - y_pos_dst;

						for(; it_rects < vRects.GetSize(); it_rects++)
						{
							ExpandEffectRects &eerc = vRects[it_rects];
							eerc.m_rcDst.OffsetRect( 0, nDstOffset);
						}
					}

					int it_rects = 0;
					for(; it_rects < vRects.GetSize(); it_rects++)
					{
						ExpandEffectRects &eerc = vRects[it_rects];
						if(eerc.m_rcDst.IsRectEmpty())
							continue;

						if(m_bUseStretchOnExpandAnimation)
						{
							pPaintDC->StretchBlt(
								eerc.m_rcDst.left,
								eerc.m_rcDst.top,
								eerc.m_rcDst.Width(),
								eerc.m_rcDst.Height(),
								&dcmm,
								eerc.m_rcSrc.left,
								eerc.m_rcSrc.top,
								eerc.m_rcSrc.Width(),
								eerc.m_rcSrc.Height(),
								SRCCOPY);
						}
						else
						{
							pPaintDC->BitBlt(
								eerc.m_rcDst.left,
								eerc.m_rcDst.top,
								eerc.m_rcDst.Width(),
								eerc.m_rcDst.Height(),
								&dcmm,
								eerc.m_rcSrc.left,
								eerc.m_rcSrc.top,
								SRCCOPY);
						}
					}
				}
			}

			break;

		case __AT_CONTENT_DISPLAY		:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				pBmpOld = dcmm.SelectObject(&m_bmpScreenDst);
				pPaintDC->BitBlt(
					rcClient.left,
					rcClient.top,
					cx, cy,
					&dcmm,
					0, 0,
					SRCCOPY);
			}

			break;

		case __AT_FADE					:
			if( tmp != NULL && src != NULL && dst != NULL )
			{
				pBmpOld = dcmm.SelectObject(&m_bmpScreenTmp);

				for( int pixel = 0; pixel < cx * cy; pixel++)
				{
					COLORREF c0 = *dst++;
					COLORREF c1 = *src++;

					*tmp++ = RGB(
						(m_nAnimPercent * long(GetRValue(c0)) + (100L - m_nAnimPercent) * long(GetRValue(c1))) / 100L,
						(m_nAnimPercent * long(GetGValue(c0)) + (100L - m_nAnimPercent) * long(GetGValue(c1))) / 100L,
						(m_nAnimPercent * long(GetBValue(c0)) + (100L - m_nAnimPercent) * long(GetBValue(c1))) / 100L);
				}

				pPaintDC->BitBlt(
					rcClient.left,
					rcClient.top,
					cx, cy,
					&dcmm,
					0, 0,
					SRCCOPY);
			}

			break;
		}
	}

	if( pBmpOld != NULL )
		dcmm.SelectObject(pBmpOld);

	if( pOldPalette != NULL )
		pPaintDC->SelectPalette( pOldPalette, FALSE);
}

void CTPopupBaseWnd::_DoPaint( CDC &dcPaint, BOOL bUseBackBuffer)
{
}

void CTPopupBaseWnd::OnTimer( UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_DELAY_SHOW		:
		{
			KillTimer(ID_TIMER_DELAY_SHOW);
			ShowWindow(SW_SHOWNA);
			Invalidate(FALSE);
			UpdateWindow();

			return;
		}

		break;

	case ID_TIMER_ANIMATION			:
		if(!m_bAnimFinished)
		{
			clock_t nCurrAnimTime = clock();
			int nDuration = nCurrAnimTime - g_nLastAnimTime;
			int nSteps = (int) (0.5f + (FLOAT) nDuration / ID_PERIOD_ANIMATION);

			switch(m_AnimationType)
			{
			case __AT_CONTENT_DISPLAY		: m_bAnimFinished = TRUE; break;
			case __AT_FADE					:
			case __AT_CONTENT_EXPAND		:
			case __AT_ROLL					:
			case __AT_SLIDE					:
			case __AT_ROLL_AND_STRETCH		:
			case __AT_SLIDE_AND_STRETCH		:
			case __AT_NOISE					:
			case __AT_BOXES					:
			case __AT_CIRCLES				:
			case __AT_HOLES					:
				{
					m_nAnimPercentOld = m_nAnimPercent;
					m_nAnimPercent += g_nAnimStepMetric;

					if( m_nAnimPercent > 100 + nSteps * g_nAnimStepMetric )
						m_nAnimPercent = 101;

					if( m_nAnimPercent > 100 )
						m_bAnimFinished = TRUE;
				}

				break;
			}

			if(m_bAnimFinished)
			{
				g_SoundPlayer->PlaySound(m_nPlaySoundOnAnimationFinished);
				m_nPlaySoundOnAnimationFinished = CTSoundPlayer::__NO_SOUND;
				KillTimer(ID_TIMER_ANIMATION);
				m_AnimationType = m_DefAnimationType;
			}

			if( GetSafeHwnd() != NULL )
			{
				if(m_bAnimFinished)
					_EndAnimation();
				else
					Invalidate(FALSE);

				g_nLastAnimTime = nCurrAnimTime;
			}
		}

		if( m_bAnimFinished && IsKindOf(RUNTIME_CLASS(CTPopupMenuWnd)) )
			((CTPopupMenuWnd *) this)->GetSite().SetAnimated(NULL);

		return;

	default							: CWnd::OnTimer(nIDEvent); break;
	}
}

int CTPopupBaseWnd::OnMouseActivate( CWnd *pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;
}

BOOL CTPopupBaseWnd::OnEraseBkgnd( CDC *pDC)
{
	return TRUE;
}

void CTPopupBaseWnd::OnNcPaint()
{
}

void CTPopupBaseWnd::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR *lpncsp)
{
}

void CTPopupBaseWnd::OnSize( UINT nType, int cx, int cy)
{
	CWnd::OnSize( nType, cx, cy);
	Invalidate(FALSE);
}

BOOL CTPopupBaseWnd::DestroyWindow()
{
	if(!::IsWindow(GetSafeHwnd()))
		return TRUE;

	return CWnd::DestroyWindow();
}

void CTPopupBaseWnd::PostNcDestroy()
{
}

void CTPopupMenuWnd::PostNcDestroy()
{
	if(m_bTopLevel)
	{
		if(::IsWindow(m_hWndCmdReciever))
			::SendMessage( m_hWndCmdReciever, m_nMsgNotifyMenuClosed, 0, LPARAM(this));

		CTPopupMenuSite &_site = GetSite();
		if(!_site.IsShutdownMode())
			_site.DoneInstance();
	}
}

BOOL CTPopupBaseWnd::OnSetCursor( CWnd *pWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnSetCursor( pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuTipWnd

IMPLEMENT_DYNCREATE( CTPopupMenuTipWnd, CTPopupBaseWnd)

CTPopupMenuTipWnd::CTPopupMenuTipWnd()
{
	m_hIcon = NULL;
	m_IconSize.cx = 0;
	m_IconSize.cy = 0;
	m_strText = _T("");
	m_bFlipHorz = FALSE;
	m_bFlipVert = FALSE;

	m_hIcon = ::LoadIcon( NULL, IDI_INFORMATION);
	m_IconSize.cx = 16;
	m_IconSize.cy = 16;

	m_bAnimFinished = TRUE;
	m_AnimationType = __AT_NONE;
}

CTPopupMenuTipWnd::~CTPopupMenuTipWnd()
{
}

BEGIN_MESSAGE_MAP(CTPopupMenuTipWnd, CTPopupBaseWnd)
	//{{AFX_MSG_MAP(CTPopupMenuTipWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTPopupMenuTipWnd::GetWindowRegion( CDC *pDC, HRGN *pRegion, CSize *pSize)
{
	CRect rcWnd( 0, 0, 0, 0);
	POINT ptLeader[3];
	CRgn rgnLeader;
	CRgn rgnCaption;
	CFont *pSysFont;

	pSysFont = (CFont *) pDC->SelectObject(&g_PaintManager->m_FontNormal);
	pDC->DrawText( m_strText, &rcWnd, DT_CALCRECT);
	pDC->SelectObject(pSysFont);

	rcWnd.InflateRect( CX_ROUNDED, CY_ROUNDED);
	if( m_hIcon != NULL )
		rcWnd.right = rcWnd.right + m_IconSize.cx + CX_ICON_MARGIN;

	if( rcWnd.Height() < m_IconSize.cy )
		rcWnd.bottom = rcWnd.top + m_IconSize.cy;

	ptLeader[0].x = rcWnd.Width() - CX_ROUNDED;
	ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;

	ptLeader[1].x = ptLeader[0].x;
	ptLeader[1].y = ptLeader[0].y + CY_LEADER;

	ptLeader[2].x = ptLeader[0].x - CX_LEADER;
	ptLeader[2].y = rcWnd.Height() - CY_ROUNDED;

	CRect rcRoundRectRgn( CPoint( 0, 0), rcWnd.Size());
	CRect rcRectRgn( 0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER);

	m_bFlipHorz = FALSE;
	m_bFlipVert = FALSE;

	CRect rcDesktop = _CalcDesktopRect();
	if( m_ptTrack.x - rcWnd.Width() < rcDesktop.left )
	{
		m_bFlipHorz = TRUE;
		ptLeader[0].x = CX_ROUNDED;
		ptLeader[0].y = rcWnd.Height() - CY_ROUNDED;
		ptLeader[1].x = ptLeader[0].x;
		ptLeader[1].y = ptLeader[0].y + CY_LEADER;
		ptLeader[2].x = ptLeader[0].x + CX_LEADER;
		ptLeader[2].y = ptLeader[0].y;
	}

	if( m_ptTrack.y - rcWnd.Height() - CY_ROUNDED * 2 < rcDesktop.top )
	{
		m_bFlipVert = TRUE;
		ptLeader[0].y -= rcWnd.Height() - CY_ROUNDED * 2;
		ptLeader[1].y = ptLeader[0].y - CY_LEADER;
		ptLeader[2].y -= ptLeader[0].y;
	}

	rgnCaption.CreateRoundRectRgn(
		rcRoundRectRgn.left,
		rcRoundRectRgn.top,
		rcRoundRectRgn.right,
		rcRoundRectRgn.bottom,
		CX_ROUNDED,
		CY_ROUNDED);

	rgnLeader.CreatePolygonRgn( ptLeader, 3, ALTERNATE);
	*pRegion = ::CreateRectRgn(
		rcRectRgn.left,
		rcRectRgn.top,
		rcRectRgn.right,
		rcRectRgn.bottom);

	CombineRgn(
		*pRegion,
		HRGN(rgnCaption),
		HRGN(rgnLeader),
		RGN_OR);

	if(m_bFlipVert)
		OffsetRgn( *pRegion, 0, CY_LEADER);

	if( pSize != NULL )
	{
		pSize->cx = rcWnd.Width();
		pSize->cy = rcWnd.Height() + CY_LEADER;
	}

	return TRUE;
}

void CTPopupMenuTipWnd::_DoPaint( CDC &dcPaint, BOOL bUseBackBuffer)
{
	CDC &dc = dcPaint;
	CRect rect;
	CBrush brWindow;
	CBrush brFrame;
	CBrush brInnerFrame;
	HRGN hRegion;
	CRgn *pRegion;
	CFont *pSysFont;

	GetClientRect(rect);
	brInnerFrame.CreateSolidBrush(g_PaintManager->GetColor(COLOR_3DFACE));
	brFrame.CreateSolidBrush(g_PaintManager->GetColor(COLOR_3DDKSHADOW));
	brWindow.CreateSolidBrush(g_PaintManager->GetColor(COLOR_INFOBK));

	hRegion = CreateRectRgnIndirect(&rect);
	GetWindowRgn(hRegion);
	pRegion = CRgn::FromHandle(hRegion);

	dc.FillRgn( pRegion, &brWindow);
	dc.FrameRgn( pRegion, &brInnerFrame, 3, 3);
	dc.FrameRgn( pRegion, &brFrame, 1, 1);

	rect.DeflateRect( CX_ROUNDED, CY_ROUNDED, 0, 0);
	if( m_hIcon != NULL )
		rect.left += m_IconSize.cx + CX_ICON_MARGIN;

	pSysFont = (CFont *) dc.SelectObject(&g_PaintManager->m_FontNormal);
	dc.SetBkMode(TRANSPARENT);

	int nIconVertPos = CY_ROUNDED;
	if(m_bFlipVert)
	{
		rect.OffsetRect( 0, CY_LEADER);
		nIconVertPos += CY_LEADER;
	}

	COLORREF clrOltTextColor = dc.SetTextColor(g_PaintManager->GetColor(COLOR_INFOTEXT));
	dc.DrawText( m_strText, &rect, DT_TOP|DT_LEFT);
	dc.SetTextColor(clrOltTextColor);

	if( m_hIcon != NULL )
	{
		DrawIconEx(
			dc.GetSafeHdc(),
			CX_ROUNDED,
			nIconVertPos,
			m_hIcon,
			m_IconSize.cx,
			m_IconSize.cy,
			0, NULL,
			DI_NORMAL);
	}

	::DeleteObject(hRegion);
	dc.SelectObject(pSysFont);
}

void CTPopupMenuTipWnd::SetText( LPCTSTR lpszText)
{
	m_strText = lpszText == NULL ? _T("") : lpszText;
	m_strText.TrimLeft();
	m_strText.TrimRight();
}

void CTPopupMenuTipWnd::SetIcon( HICON hIcon)
{
	ICONINFO IconInfo;
	m_hIcon = hIcon;

	if( hIcon == NULL )
	{
		m_IconSize = CSize( 0, 0);
		return;
	}

	ZeroMemory( &IconInfo, sizeof(ICONINFO));
	::GetIconInfo( m_hIcon, &IconInfo);

	m_IconSize.cx = (BYTE) (IconInfo.xHotspot * 2);
	m_IconSize.cy = (BYTE) (IconInfo.yHotspot * 2);

	::DeleteObject(IconInfo.hbmMask);
	::DeleteObject(IconInfo.hbmColor);

	if(::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CTPopupMenuTipWnd::Show( CWnd *pWndParent, const RECT &rcExcludeArea)
{
	HCURSOR hCursor = ::LoadCursor( NULL, IDC_ARROW);

	if( GetSafeHwnd() == NULL )
	{
		CString strClassName = ::AfxRegisterWndClass(
			__POPUP_WNDCLASS_STYLES__,
			hCursor,
			(HBRUSH) (COLOR_BTNFACE + 1),
			NULL);

		BOOL bCreateResult = CWnd::CreateEx(
			WS_EX_TOPMOST,
			(LPCTSTR) strClassName,
			NULL,
			WS_POPUP,
			0, 0, 0, 0,
			pWndParent->GetSafeHwnd(),
			NULL,
			NULL);

		if(!bCreateResult)
			return FALSE;
	}

	m_rcExcludeArea = rcExcludeArea;
	m_rcExcludeArea.NormalizeRect();
	CPoint ptTipTarget(m_rcExcludeArea.TopLeft());

	ptTipTarget += CSize( __TIP_OVER_METRIC / 2, __TIP_OVER_METRIC / 2);
	m_ptTrack = ptTipTarget;

	CDC *pDC = GetDC();
	HRGN hRegion;
	CSize szWindow;

	GetWindowRegion( pDC, &hRegion, &szWindow);
	ReleaseDC(pDC);
	::SetWindowRgn(
		m_hWnd,
		hRegion,
		TRUE);

	if(m_bFlipHorz)
		m_ptTrack.x += m_rcExcludeArea.Width() - __TIP_OVER_METRIC + szWindow.cx - CX_ROUNDED * 2;

	if(m_bFlipVert)
		m_ptTrack.y += m_rcExcludeArea.Height() - __TIP_OVER_METRIC + szWindow.cy - CX_ROUNDED * 2;

	CPoint ptWndPos(
		m_ptTrack.x - szWindow.cx + CX_ROUNDED,
		m_ptTrack.y - szWindow.cy + CY_ROUNDED);

	SetWindowPos(
		&wndTop,
		ptWndPos.x,
		ptWndPos.y,
		szWindow.cx,
		szWindow.cy,
		SWP_NOACTIVATE);

	CRect rcWnd( ptWndPos, szWindow);
	MoveWindow(&rcWnd);
	::SetCursor(hCursor);

	SetTimer( ID_TIMER_DELAY_SHOW, ID_PERIOD_DELAY_SHOW, NULL);
	return TRUE;
}

void CTPopupMenuTipWnd::Hide()
{
	if( GetSafeHwnd() != NULL )
	{
		KillTimer(ID_TIMER_DELAY_SHOW);
		ShowWindow(SW_HIDE);
	}

	m_rcExcludeArea.SetRectEmpty();
}


/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuWnd

BOOL CTPopupMenuWnd::m_bAllowNonAccelPositioning = FALSE;
IMPLEMENT_DYNCREATE( CTPopupMenuWnd, CTPopupBaseWnd)

CTPopupMenuWnd::CTPopupMenuWnd() : m_pWndParentMenu(NULL),
	m_hWndCmdReciever(m_hWndCmdReciever),
	m_nLeftAreaWidth(0)
{
	_Init();
}

CTPopupMenuWnd::~CTPopupMenuWnd()
{
	_OnCancelMode(TRUE);
	if(m_bTopLevel)
		_BuildItems( NULL, TRUE);
}

void CTPopupMenuWnd::_Init()
{
	_InitAnimation();
	m_nDyScrollOffset = 0;
	m_bTopLevel = FALSE;
	m_bScrollingAvailable = FALSE;
	m_bExpandAvailable = FALSE;
	m_bExpandWasPressed = FALSE;
	m_bCombineWithEA = FALSE;
	m_dwTrackFlags = 0;
	m_nWaitingExpandTickCount = 0;
	m_nCurIndex = IDX_NOTHING;
	m_nDelayedFocusItemIndex = IDX_NOTHING;

	m_nScrollingDirection = 0;
	m_rcScrollTop.SetRectEmpty();
	m_rcScrollBottom.SetRectEmpty();
	m_rcExpandBtn.SetRectEmpty();
	m_nCombineAlign = __CMBA_NONE;

	m_pCbPaintCombinedCookie = NULL;
	m_pCbPaintCombinedContent = NULL;
}

int CTPopupMenuWnd::_GetCurIndex() const
{
	return m_nCurIndex;
}

int CTPopupMenuWnd::_GetSpecBtnHeight()
{
	int nHeight = ::GetSystemMetrics(SM_CYMENUCHECK);
	return nHeight;
}

void CTPopupMenuWnd::_RecalcLayoutImpl()
{
	CRect rcClient;
	_GetClientRect(&rcClient);

	int nScrollBtnDy = _GetSpecBtnHeight();
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nMenuShadowSize = 0;
	int nScrollBottomOffsetDy = 0;

	if(m_bExpandAvailable)
	{
		m_rcExpandBtn.left = rcClient.left + nMenuBorderSize;
		m_rcExpandBtn.right = rcClient.right - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.bottom = rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcExpandBtn.top = m_rcExpandBtn.bottom - nScrollBtnDy;
		nScrollBottomOffsetDy = -m_rcExpandBtn.Height();
		m_rcExpandBtn.DeflateRect( __MI_HORZ_GAP_TO_BORDER, 0);
		m_rcExpandBtn.left += m_nLeftAreaWidth;
	}

	if(m_bScrollingAvailable)
	{
		m_rcScrollTop.left = m_rcScrollBottom.left = rcClient.left + nMenuBorderSize;
		m_rcScrollTop.right = m_rcScrollBottom.right = rcClient.right - (nMenuBorderSize + nMenuShadowSize);
		m_rcScrollTop.top = rcClient.top + nMenuBorderSize;
		m_rcScrollTop.bottom = m_rcScrollTop.top + nScrollBtnDy;
		m_rcScrollBottom.bottom = rcClient.bottom - (nMenuBorderSize + nMenuShadowSize);
		m_rcScrollBottom.top = m_rcScrollBottom.bottom - nScrollBtnDy;

		m_rcScrollBottom.OffsetRect( 0, nScrollBottomOffsetDy);
		m_rcScrollTop.DeflateRect( __MI_HORZ_GAP_TO_BORDER, 0);
		m_rcScrollBottom.DeflateRect( __MI_HORZ_GAP_TO_BORDER, 0);
		m_rcScrollTop.left += m_nLeftAreaWidth;
		m_rcScrollBottom.left += m_nLeftAreaWidth;
	}
}

class CTPopupMenuWnd::CTPopupMenuCmdUI : public CCmdUI
{
public:
	CTPopupMenuWnd *m_pPopup;

	virtual void Enable( BOOL bOn)
	{
		m_bEnableChanged = TRUE;
		CTPopupMenuWnd::MENUITEMDATA &mi = m_pPopup->_GetItemRef(m_nIndex);

		if(mi.IsPopup())
			return;

		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(mi.GetCmdReciever()),
			mi.GetCmdID());

		if( p_cmd == NULL )
			return;

		p_cmd->StateEnable(bOn);
	};

	virtual void SetRadio( BOOL bOn)
	{
		CTPopupMenuWnd::MENUITEMDATA &mi = m_pPopup->_GetItemRef(m_nIndex);

		if(mi.IsPopup())
			return;

		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(mi.GetCmdReciever()),
			mi.GetCmdID());

		if( p_cmd == NULL )
			return;

		p_cmd->StateSetRadio(bOn);
	};

	virtual void SetCheck( int nCheck)
	{
		CTPopupMenuWnd::MENUITEMDATA &mi = m_pPopup->_GetItemRef(m_nIndex);

		if(mi.IsPopup())
			return;

		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(mi.GetCmdReciever()),
			mi.GetCmdID());

		if( p_cmd == NULL )
			return;

		p_cmd->StateSetCheck(nCheck);
	};

	virtual void SetText( LPCTSTR lpszText)
	{
		if( lpszText == NULL )
			return;

		CTPopupMenuWnd::MENUITEMDATA &mi = m_pPopup->_GetItemRef(m_nIndex);
		if(mi.IsPopup())
			return;

		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(mi.GetCmdReciever()),
			mi.GetCmdID());

		if( p_cmd == NULL )
			return;

		p_cmd->m_strMenuText = lpszText;
	};
}; // class CTPopupMenuWnd::CTPopupMenuCmdUI

void CTPopupMenuWnd::_UpdateCmdUI()
{
	if( m_bTopLevel && (m_dwTrackFlags&TPMX_NO_CMD_UI) )
		return;

	int it = 0;
	BOOL bDisableIfNoHndler = TRUE;
	CTPopupMenuCmdUI state;

	state.m_pPopup = this;
	state.m_pOther = NULL;
	state.m_nIndexMax = (UINT) ItemGetCount();
	state.m_nIndex = 0;

	for(; it < m_items_all.GetSize(); it++, state.m_nIndex++)
	{
		MENUITEMDATA &mi = m_items_all[it];

		if(mi.IsSeparator())
			continue;

		if(mi.IsPopup())
		{
			mi.GetPopup()->_UpdateCmdUI();
			continue;
		}

		if(CTCmdManager::IsCommandNeedsSpecUpdate(mi.GetCmdID()))
			continue;

		HWND hWndCmdReciever = mi.GetCmdReciever();
		CWnd *pWndCmdReciever = CWnd::FromHandle(hWndCmdReciever);

		state.m_nID = mi.GetCmdID();
		state.DoUpdate( pWndCmdReciever, bDisableIfNoHndler);
	}
}

BOOL CTPopupMenuWnd::MENUITEMDATA::ConstructPopup()
{
	m_pWndChild = new CTPopupMenuWnd;
	m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;

	return TRUE;
}

void CTPopupMenuWnd::MENUITEMDATA::DestroyPopup()
{
	SetPopupIcon(NULL);

	if( m_pWndChild == NULL )
		return;

	if( m_pWndChild->GetSafeHwnd() != NULL && ::IsWindow(m_pWndChild->GetSafeHwnd()) )
		m_pWndChild->DestroyWindow();

	delete m_pWndChild;
	m_pWndChild = NULL;
}

BOOL CTPopupMenuWnd::_BuildItems( CMenu *pBuildMenu, BOOL bTopLevel)
{
	for( int i=0; i<m_items_all.GetSize(); i++)
	{
		MENUITEMDATA &mi = m_items_all[i];

		if(mi.IsPopup())
		{
			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->_OnCancelMode();
			mi.GetPopup()->_BuildItems( NULL, FALSE);
			mi.DestroyPopup();
		}
	}
	m_items_all.RemoveAll();

	if( pBuildMenu == NULL )
		return TRUE;

	m_bTopLevel = bTopLevel;
	if(m_bTopLevel)
		m_pWndParentMenu = NULL;

	BOOL bPrevWasSeparator = FALSE;
	int nMruUpdateIndex = -1;
	int nInsertedIndex = 0;
	int nMenuItemCount = pBuildMenu->GetMenuItemCount();

	m_items_all.RemoveAll();
	for( int nItemIndex=0; nItemIndex<nMenuItemCount; nItemIndex++)
	{
		UINT nMenuItemID = pBuildMenu->GetMenuItemID(nItemIndex);
		MENUITEMDATA mi;

		if( ID_FILE_MRU_FIRST <= nMenuItemID && nMenuItemID <= ID_FILE_MRU_LAST )
		{
			if( nMruUpdateIndex < 0 )
				nMruUpdateIndex = nInsertedIndex;

			continue;
		}

		mi.SetCmdReciever(m_hWndCmdReciever);
		mi.UpdateFromMenu(
			m_hWndCmdReciever,
			pBuildMenu,
			nItemIndex);

		BOOL bIsSeparator = mi.IsSeparator();
		if( bPrevWasSeparator && bIsSeparator )
			continue;

		bPrevWasSeparator = bIsSeparator;
		if(mi.IsPopup())
		{
			CMenu *pSubMenu = pBuildMenu->GetSubMenu(nItemIndex);

			mi.GetPopup()->m_hWndCmdReciever = m_hWndCmdReciever;
			mi.GetPopup()->_BuildItems( pSubMenu, FALSE);
			mi.GetPopup()->m_pWndParentMenu = this;
		}

		_InsertItem( -1, mi);
		nInsertedIndex++;
	}

	int nMruInsertCount = 0;
	if( nMruUpdateIndex >= 0 )
	{
		CRecentFileList *pRecentFileList = CFriendlyWinApp::_GetFriendlyApp()->_GetRecentFileList();

		if( pRecentFileList != NULL )
		{
			int nRecentCount = pRecentFileList->GetSize();
			TCHAR szCurrDir[_MAX_PATH + 1];

			::memset( szCurrDir, 0, sizeof(szCurrDir));
			::GetCurrentDirectory( _MAX_PATH, szCurrDir);

			int nLenCurDir = _tcslen(szCurrDir);
			for( nItemIndex=0; nItemIndex<nRecentCount; nItemIndex++)
			{
				CString strDisplayName(_T(""));

				if(!pRecentFileList->GetDisplayName(
					strDisplayName,
					nItemIndex,
					szCurrDir,
					nLenCurDir,
					TRUE))
					continue;

				UINT nCmdID = ID_FILE_MRU_FIRST + nItemIndex;
				CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever),
					nCmdID);

				if( p_cmd == NULL )
					p_cmd = g_CmdManager->CmdAllocPtr( g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever), nCmdID);

				if( p_cmd == NULL )
					return FALSE;

				int nDisplayIndex = nItemIndex + 1;
				if( nDisplayIndex < 10 )
					p_cmd->m_strMenuText.Format( _T("&%d %s"), nDisplayIndex, (LPCTSTR) strDisplayName);
				else
					p_cmd->m_strMenuText.Format( _T("%d&%d %s"), nDisplayIndex / 10, nDisplayIndex % 10, (LPCTSTR) strDisplayName);

				CString strRecentFileFmt1(_T("Recent file %d"));
				CString strRecentFileFmt2(_T("Recent file %d (\"%s\")"));

				p_cmd->m_strToolbarText.Format(
					strRecentFileFmt1,
					nItemIndex + 1);

				p_cmd->m_strTipTool.Format(
					strRecentFileFmt2,
					nItemIndex + 1,
					(LPCTSTR) strDisplayName);

				p_cmd->m_strTipStatus = p_cmd->m_strTipTool;
				BOOL bInsRetVal = ItemInsert(
					nCmdID,
					nMruUpdateIndex + nItemIndex);

				if(!bInsRetVal)
					return FALSE;

				nMruInsertCount++;
			}
		}

		if( nMruInsertCount > 0 )
		{
			if(!ItemInsert( ID_SEPARATOR, nMruUpdateIndex + nMruInsertCount))
				return FALSE;

			nMruInsertCount++;
		}
	}

	_SyncItems();
	return TRUE;
}

void CTPopupMenuWnd::_GetClientRect( RECT *pRectClient)
{
	*pRectClient = m_rcClient;
}

int CTPopupMenuWnd::_HitTest( const CPoint &point)
{
	if( GetSafeHwnd() == NULL )
		return IDX_NOTHING;

	CRect rcClient;
	_GetClientRect(&rcClient);

	if(!rcClient.PtInRect(point))
		return IDX_NOTHING;

	if(m_bScrollingAvailable)
	{
		CRect rcItem;

		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect( IDX_SCROLL_TOP, rcItem);

			if(rcItem.PtInRect(point))
				return IDX_SCROLL_TOP;
		}

		int nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect( IDX_SCROLL_BOTTOM, rcItem);

			if(rcItem.PtInRect(point))
				return IDX_SCROLL_BOTTOM;
		}
	}

	if(m_bExpandAvailable)
	{
		CRect rcExpand;

		_GetItemRect( IDX_EXPAND, rcExpand);
		if(rcExpand.PtInRect(point))
			return IDX_EXPAND;
	}

	for( int i=0, nIndex = 0; i<m_items_all.GetSize(); i++, nIndex++)
	{
		MENUITEMDATA &mi = m_items_all[i];
		if(!mi.IsDisplayed())
			continue;

		CRect rcItem;
		_GetItemRect( nIndex, rcItem);

		if(rcItem.PtInRect(point))
		{
			int nRetIndex = mi.GetIndex();
			return nRetIndex;
		}
	}

	return IDX_NOTHING;
};


BEGIN_MESSAGE_MAP(CTPopupMenuWnd, CTPopupBaseWnd)
	//{{AFX_MSG_MAP(CTPopupMenuWnd)
	ON_WM_ACTIVATEAPP()
	ON_WM_CANCELMODE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSDEADCHAR()
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuWnd message handlers

BOOL CTPopupMenuWnd::LoadMenu( HWND hWndCmdRecv,
							   UINT nIDResource,
							   BOOL bPopupMenu)
{
	CMenu menu;

	if(!menu.LoadMenu(nIDResource))
		return FALSE;

	return UpdateFromMenu(
		hWndCmdRecv,
		&menu,
		bPopupMenu);
}

BOOL CTPopupMenuWnd::UpdateMdiWindowsMenu( CWnd *pWndStartSearchMdiFrameWnd)
{
	CMDIFrameWnd *pFrame = NULL;
	CWnd *pWnd = pWndStartSearchMdiFrameWnd ? pWndStartSearchMdiFrameWnd : FromHandle(m_hWndCmdReciever);

	while(TRUE)
	{
		pFrame = DYNAMIC_DOWNCAST(
			CMDIFrameWnd,
			pWnd);

		if( pFrame != NULL )
			break;

		pWnd = pWnd->GetParentFrame();
		if( pWnd == NULL )
			return FALSE;
	}

	if( pFrame == NULL )
		return FALSE;

	int nItemIndex = ItemGetCount();
	if( nItemIndex > 0 )
	{
		MENUITEMDATA &mi = _GetItemRef(nItemIndex - 1);

		if(!mi.IsSeparator())
		{
			ItemInsert(ID_SEPARATOR);
			nItemIndex++;
		}
	}

	HWND hWndMDIClient = pFrame->m_hWndMDIClient;
	HWND hWndActiveChildFrame = (HWND)::SendMessage(
		hWndMDIClient,
		WM_MDIGETACTIVE,
		0, NULL);

	int nCmdID = __ID_MDIWNDLIST_FIRST;
	for( int i=1; TRUE; i++, nCmdID++)
	{
		HWND hWndChildFrame = ::GetDlgItem( hWndMDIClient, nCmdID);
		if( hWndChildFrame == NULL )
			break;

		if( i <= __ID_MDIWNDLIST_COUNT )
		{
			CString strWinName(_T(""));

			CWnd::FromHandle(hWndChildFrame)->GetWindowText(strWinName);
			CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd(pWndStartSearchMdiFrameWnd->GetSafeHwnd()),
				nCmdID);

			if( p_cmd == NULL )
				p_cmd = g_CmdManager->CmdAllocPtr( g_CmdManager->ProfileNameFromWnd(pWndStartSearchMdiFrameWnd->GetSafeHwnd()), nCmdID);

			if( p_cmd == NULL )
				return FALSE;

			p_cmd->m_strMenuText.Format(
				_T("&%d %s"),
				i, (LPCTSTR) strWinName);
			p_cmd->m_strToolbarText = p_cmd->m_strMenuText;

			CString strDocumentNameFmt(_T("Document \"%s\""));
			p_cmd->m_strTipTool.Format(
				strDocumentNameFmt,
				(LPCTSTR) strWinName);

			p_cmd->m_strTipStatus = p_cmd->m_strTipTool;
			p_cmd->StateSetRadio(hWndChildFrame == hWndActiveChildFrame);

			if(!ItemInsert(nCmdID))
				return FALSE;
		}
		else
		{
			if(!ItemInsert(TYPE_SEPARATOR))
				return FALSE;

			HWND hWndSearch = pWndStartSearchMdiFrameWnd->GetSafeHwnd();
			CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd(hWndSearch),
				__ID_MDIWND_DLGWINDOWS);

			if( p_cmd == NULL )
				p_cmd = g_CmdManager->CmdAllocPtr( g_CmdManager->ProfileNameFromWnd(hWndSearch), __ID_MDIWND_DLGWINDOWS);

			if( p_cmd == NULL )
				return FALSE;

			CString strMoreWindows(_T("More Windows"));
			CString strManageWindows(_T("Manage opened document windows"));

			p_cmd->m_strMenuText = strMoreWindows;
			p_cmd->m_strToolbarText = p_cmd->m_strMenuText;
			p_cmd->m_strTipTool = strManageWindows;
			p_cmd->m_strTipStatus = p_cmd->m_strTipTool;

			if(!ItemInsert(nCmdID))
				return FALSE;

			break;
		}
	}

	_SyncItems();
	return TRUE;
}

HMENU CTPopupMenuWnd::ExportToMenu( BOOL bDeep) const
{
	CMenu menu;

	if(!menu.CreatePopupMenu())
		return NULL;

	OSVERSIONINFO ov;
	ov.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&ov);

	BOOL bStupidNT4XX = ov.dwPlatformId == VER_PLATFORM_WIN32_NT && ov.dwMajorVersion < 5;
	int nCount = ItemGetCount();

	for( int nItem = 0; nItem < nCount; nItem++)
	{
		const MENUITEMDATA &mi = _GetItemRef(nItem);

		if(mi.IsPopup())
		{
			CString strMiAccelText(mi.GetAccelText());
			CString strMiText(mi.GetText());

			if(!strMiAccelText.IsEmpty())
			{
				strMiText += _T("\t");
				strMiText += strMiAccelText;
			}

			const CTPopupMenuWnd *pPopup = mi.GetPopup();
			HMENU hPopupData;

			if(bDeep)
				hPopupData = pPopup->ExportToMenu();
			else
				hPopupData = ::CreatePopupMenu();

			if(bStupidNT4XX)
				::DestroyMenu(hPopupData);

			continue;
		}

		if(mi.IsSeparator())
		{
			menu.AppendMenu(MF_SEPARATOR);
			continue;
		}

		CString strMiText(mi.GetText());
		CString strMiAccelText(mi.GetAccelText());

		if(!strMiAccelText.IsEmpty())
		{
			strMiText += _T("\t");
			strMiText += strMiAccelText;
		}

		menu.AppendMenu(
			MF_STRING,
			mi.GetCmdID(),
			strMiText);
	}

	return menu.Detach();
}

BOOL CTPopupMenuWnd::UpdateFromMenu( HWND hWndCmdRecv,
									 CMenu *pBuildMenu,
									 BOOL bPopupMenu,
									 BOOL bTopLevel)
{
	if( hWndCmdRecv != NULL )
		m_hWndCmdReciever = hWndCmdRecv;

	if( pBuildMenu == NULL || !::IsMenu(pBuildMenu->GetSafeHmenu()) )
		return FALSE;

	if(bPopupMenu)
	{
		pBuildMenu = pBuildMenu->GetSubMenu(0);

		if( pBuildMenu == NULL || !::IsMenu(pBuildMenu->GetSafeHmenu()) )
			return FALSE;
	}

	if( !g_CmdManager->UpdateFromMenu(
		g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever),
		pBuildMenu->GetSafeHmenu()))
		return FALSE;

	if(!_BuildItems( pBuildMenu, bTopLevel))
		return FALSE;

	return TRUE;
}

CTPopupMenuSite &CTPopupMenuWnd::GetSite() const
{
	return CTPopupMenuSite::m_DefPopupMenuSite;
}

CTPopupMenuWnd *CTPopupMenuWnd::GetTrackingMenu()
{
	CTPopupMenuWnd *pPopup = CTPopupMenuSite::m_DefPopupMenuSite.GetInstance();

	if(!::IsWindow(pPopup->GetSafeHwnd()))
		return NULL;

	return pPopup;
}

void CTPopupMenuWnd::CancelMenuTracking()
{
	if( !CTPopupMenuSite::m_DefPopupMenuSite.IsEmpty() &&
		!CTPopupMenuSite::m_DefPopupMenuSite.IsShutdownMode())
		CTPopupMenuSite::m_DefPopupMenuSite.DoneInstance();
}

BOOL CTPopupMenuWnd::IsMenuTracking()
{
	if(CTPopupMenuSite::m_DefPopupMenuSite.IsEmpty())
		return FALSE;

	return TRUE;
}

BOOL CTPopupMenuWnd::TrackPopupMenu( DWORD dwTrackFlags,
									 int x,
									 int y,
									 LPCRECT lpRect,
									 LPVOID pCbPaintCombinedCookie,
									 pCbPaintCombinedContent pCbContent,
									 UINT *lpnResultCmdID)
{
	CTPopupMenuSite &_site = GetSite();
	BOOL bSiteWasEmpty = _site.IsEmpty();

	if(!bSiteWasEmpty)
	{
		if(_site.IsShutdownMode())
			return FALSE;

		_site.DoneInstance();
	}

	HWND hWndCapture = ::GetCapture();
	CWnd *pWndCapture = FromHandlePermanent(hWndCapture);

	if( pWndCapture != NULL )
	{
		ReleaseCapture();

		if(::IsWindow(hWndCapture))
			pWndCapture->SendMessage(WM_CANCELMODE);
	}

	_site.SetInstance(this);
	if( lpnResultCmdID != NULL )
		_site.SetTargetCmdIdPtr(lpnResultCmdID);

	m_dwTrackFlags = dwTrackFlags;
	_UpdateCmdUI();

	if(!_TrackPopupMenu(
		dwTrackFlags,
		x, y,
		lpRect,
		pCbPaintCombinedCookie,
		pCbContent))
	{
		_site.DoneInstance();
		return FALSE;
	}

	if( (dwTrackFlags&TPMX_DO_MESSAGE_LOOP) == 0 )
		return TRUE;

	while(IsMenuTracking())
	{
		WaitMessage();
		HWND hWndThis = GetSafeHwnd();

		CTPopupMenuWnd::PassMsgLoop(CTPopupBaseWnd::m_bEnableOnIdleCalls);
		if(!::IsWindow(hWndThis))
			break;
	}

	_site.DoneInstance();
	return TRUE;
}

void CTPopupMenuWnd::PassMsgLoop( BOOL bEnableOnIdleCalls)
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	MSG msg;
	while(PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if(!AfxGetApp()->PumpMessage())
		{
			PostQuitMessage(0);
			return;
		}
	}

	if(bEnableOnIdleCalls)
	{
		LONG lIdle = 0;
		while(AfxGetApp()->OnIdle(lIdle++));
	}
}

CSize CTPopupMenuWnd::_CalcTrackSize()
{
	int nMenuShadowSize = _GetMenuShadowSize();
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nXtraSz = nMenuBorderSize * 2 + nMenuShadowSize;
	CSize _size(m_szFullItems);

	if(m_bExpandAvailable)
		_size.cy += _GetSpecBtnHeight();

	_size += CSize(
		nXtraSz + m_nLeftAreaWidth,
		nXtraSz);

	return _size;
}

CRect CTPopupMenuWnd::_CalcTrackRect()
{
	m_nCombineAlign = __CMBA_NONE;
	CRect rcDesktop = _CalcDesktopRect();
	CSize _size = _CalcTrackSize();
	int nMenuShadowSize = _GetMenuShadowSize();
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nGapShift = 0;

	if(!m_bCombineWithEA)
		nGapShift = nMenuBorderSize + nMenuShadowSize;

	CRect rect( m_ptTrack, _size);
	int nCombineOffset = 0;

	if(m_bCombineWithEA)
	{
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_TOPALIGN )
		{
			nCombineOffset = 1;
			rect.OffsetRect( 0, -nCombineOffset);
		}

		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_LEFTALIGN )
		{
			nCombineOffset = 1;
			rect.OffsetRect( -nCombineOffset, 0);
		}

		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_BOTTOMALIGN )
		{
			nCombineOffset = -nMenuShadowSize - 1;
			rect.OffsetRect( 0, -nCombineOffset);
		}

		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_RIGHTALIGN )
		{
			nCombineOffset = -nMenuShadowSize - 1;
			rect.OffsetRect( -nCombineOffset, 0);
		}
	}

	if( rect.bottom > rcDesktop.bottom )
	{
		if(m_bExcludeAreaSpec)
			rect.OffsetRect( 0, -rect.Height() + m_rcExcludeArea.Height() + nGapShift + (m_bCombineWithEA ? nMenuShadowSize : 0));

		if( rect.bottom > rcDesktop.bottom )
			rect.OffsetRect( 0, m_bExcludeAreaSpec ? rcDesktop.bottom - rect.bottom : -rect.Height());
	}

	if( rect.top < rcDesktop.top )
		rect.OffsetRect( 0, rcDesktop.top - rect.top);

	if( rect.right > rcDesktop.right )
	{
		if( !m_bTopLevel && m_bExcludeAreaSpec )
			rect.OffsetRect( -rect.Width() - m_rcExcludeArea.Width(), 0);

		if( rect.right > rcDesktop.right )
			rect.OffsetRect( m_bExcludeAreaSpec ? rcDesktop.right - rect.right : -rect.Width(), 0);
	}

	if( rect.left < rcDesktop.left )
	{
		if( !m_bTopLevel && m_bExcludeAreaSpec )
			rect.OffsetRect( rect.Width() + m_rcExcludeArea.Width(), 0);

		if( rect.left < rcDesktop.left )
			rect.OffsetRect( rcDesktop.left - rect.left, 0);
	}

	if( m_bExcludeAreaSpec || m_bCombineWithEA )
	{
		CRect rcIntersection;

		if( rcIntersection.IntersectRect( &rect, &m_rcExcludeArea) || m_bCombineWithEA )
		{
			CRect rcFree( 0, 0, 0, 0);

			if( m_rcExcludeArea.left > rcDesktop.left )
				rcFree.left = m_rcExcludeArea.left - rcDesktop.left;

			if( m_rcExcludeArea.right < rcDesktop.right )
				rcFree.right = rcDesktop.right - m_rcExcludeArea.right;

			if( m_rcExcludeArea.top > rcDesktop.top )
				rcFree.top = m_rcExcludeArea.top - rcDesktop.top;

			if( m_rcExcludeArea.bottom < rcDesktop.bottom )
				rcFree.bottom = rcDesktop.bottom - m_rcExcludeArea.bottom;

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_LEFTALIGN && rcFree.right > rect.Width() )
				rect.OffsetRect( rect.left - m_rcExcludeArea.right + (m_bCombineWithEA ? nCombineOffset : 0), 0);

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_RIGHTALIGN && rcFree.left > rect.Width() )
				rect.OffsetRect( m_rcExcludeArea.left - rect.right - (m_bCombineWithEA ? nCombineOffset : 0), 0);

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_TOPALIGN )
			{
				if( rcFree.bottom > rect.Height() )
					rect.OffsetRect( 0, rect.top - m_rcExcludeArea.bottom + (m_bCombineWithEA ? nCombineOffset : 0));
				else
				{
					if(::IsWindow(GetSafeHwnd()))
					{
						CRect rcWndOld;

						GetWindowRect(&rcWndOld);
						ShowWindow(SW_HIDE);
						_FreeWinObjects();

						HWND hWndThis = GetSafeHwnd();
						BOOL bWasCreated = hWndThis != NULL && ::IsWindow(hWndThis);

						CTPopupMenuWnd::PassMsgLoop(CTPopupBaseWnd::m_bEnableOnIdleCalls);
						if( bWasCreated && !::IsWindow(hWndThis) )
							return CRect( 0, 0, 0, 0);

						if(!rcWndOld.IsRectEmpty())
						{
							CWnd *pWndTLP = GetTopLevelParent();

							if( pWndTLP != NULL )
							{
								if(pWndTLP->IsWindowVisible())
								{
									CRect rcTLP;

									pWndTLP->GetWindowRect(&rcTLP);
									if(!rcTLP.IsRectEmpty())
									{
										CRect rcIntersection;

										if(rcIntersection.IntersectRect( &rcTLP, &rcWndOld))
											pWndTLP->UpdateWindow();
									}
								}
							}
						}
					}

					m_dwTrackFlags &= ~TPMX_ALIGN_MASK;
					m_dwTrackFlags |= TPMX_BOTTOMALIGN;

					return _CalcTrackRect();
				}
			}

			if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_BOTTOMALIGN && rcFree.top > rect.Height() )
				rect.OffsetRect( 0, m_rcExcludeArea.top - rect.bottom - (m_bCombineWithEA ? nCombineOffset : 0));
		}
	}

	if( rect.right > rcDesktop.right )
	{
		rect.OffsetRect( rcDesktop.right - rect.right, 0);

		if(m_bTopLevel)
		{
			m_bExcludeAreaSpec = FALSE;
			m_bCombineWithEA = FALSE;
		}
	}

	if( rect.left < rcDesktop.left )
	{
		rect.OffsetRect( rcDesktop.left - rect.left, 0);

		if(m_bTopLevel)
		{
			m_bExcludeAreaSpec = FALSE;
			m_bCombineWithEA = FALSE;
		}
	}

	if( rect.bottom > rcDesktop.bottom )
	{
		rect.OffsetRect( 0, rcDesktop.bottom - rect.bottom);

		if(m_bTopLevel)
		{
			m_bExcludeAreaSpec = FALSE;
			m_bCombineWithEA = FALSE;
		}
	}

	if( rect.top < rcDesktop.top )
	{
		rect.OffsetRect( 0, rcDesktop.top - rect.top);

		if(m_bTopLevel)
		{
			m_bExcludeAreaSpec = FALSE;
			m_bCombineWithEA = FALSE;
		}
	}

	if( rect.bottom > rcDesktop.bottom )
	{
		rect.bottom = rcDesktop.bottom;
		m_bScrollingAvailable = TRUE;
	}

	if(m_bCombineWithEA)
	{
		if( rect.bottom == m_rcExcludeArea.top - nCombineOffset )
			m_nCombineAlign = __CMBA_BOTTOM;

		if( rect.top == m_rcExcludeArea.bottom - nCombineOffset )
			m_nCombineAlign = __CMBA_TOP;

		if( rect.left == m_rcExcludeArea.right - nCombineOffset )
			m_nCombineAlign = __CMBA_LEFT;

		if( rect.right == m_rcExcludeArea.left - nCombineOffset )
			m_nCombineAlign = __CMBA_RIGHT;

		if( m_nCombineAlign == __CMBA_NONE )
		{
			m_bCombineWithEA = FALSE;
			return _CalcTrackRect();
		}

		if( m_rgnWnd.GetSafeHandle() != NULL )
			m_rgnWnd.DeleteObject();

		CRect rcExcludeArea(m_rcExcludeArea);
		rcExcludeArea.InflateRect(
			0, 0,
			nMenuShadowSize,
			nMenuShadowSize);

		CRect rcFullWnd(
			min( rect.left, rcExcludeArea.left),
			min( rect.top, rcExcludeArea.top),
			max( rect.right, rcExcludeArea.right),
			max( rect.bottom, rcExcludeArea.bottom));

		CRgn rgnAreaWnd;
		CRgn rgnAreaExclude;

		if( !m_rgnWnd.CreateRectRgnIndirect(&rcFullWnd) ||
			!rgnAreaExclude.CreateRectRgnIndirect(&rcExcludeArea) ||
			!rgnAreaWnd.CreateRectRgnIndirect(&rect) ||
			m_rgnWnd.CombineRgn( &rgnAreaWnd, &rgnAreaExclude, RGN_OR) == ERROR ||
			m_rgnWnd.OffsetRgn(-rcFullWnd.TopLeft()) == ERROR )
		{
			if( m_rgnWnd.GetSafeHandle() != NULL )
				m_rgnWnd.DeleteObject();

			m_bCombineWithEA = FALSE;
			return _CalcTrackRect();
		}

		m_rcClient = rect;
		m_rcClient.OffsetRect(-rcFullWnd.TopLeft());
		m_rcClient.DeflateRect(
			0, 0,
			nMenuShadowSize,
			nMenuShadowSize);

		return rcFullWnd;
	}

	m_rcClient = rect;
	m_rcClient.OffsetRect(-rect.TopLeft());
	m_rcClient.DeflateRect(
		0, 0,
		nMenuShadowSize,
		nMenuShadowSize);

	return rect;
}

CRect CTPopupBaseWnd::_CalcDesktopRect()
{
	CRect rcDesktop;

	if(m_bUseDesktopWorkArea)
	{
		::SystemParametersInfo(
			SPI_GETWORKAREA, 0,
			PVOID(&rcDesktop), 0);

		return rcDesktop;
	}

	rcDesktop.SetRect( 0, 0, 800, 600);
	return rcDesktop;
}

BOOL CTPopupMenuWnd::_TrackPopupMenu( DWORD dwTrackFlags,
									  int x,
									  int y,
									  LPCRECT lpRect,
									  LPVOID pCbPaintCombinedCookie,
									  pCbPaintCombinedContent pCbContent)
{
	BOOL bForceExpandRarelyUsed = dwTrackFlags&TPMX_NO_HIDE_RARELY;

	if(!m_bMenuExpanding)
		bForceExpandRarelyUsed = TRUE;

	CTPopupMenuSite &_site = GetSite();
	if(_site.IsTopPupup(this))
	{
		MsgPrepareMenuData _mpmd(this);
		_mpmd.SendMessage(m_hWndCmdReciever);

		if(_mpmd.m_bMenuCanceled)
			return FALSE;

		if(_mpmd.m_bMenuChanged)
			_UpdateCmdUI();
	}

	for( int i=0; i<m_items_all.GetSize(); i++)
	{
		MENUITEMDATA &mi = m_items_all[i];

		if(mi.IsSeparator())
			continue;

		UINT nCmdID = mi.GetCmdID();
		if(!CTCmdManager::IsSystemCommand(nCmdID))
			continue;

		WINDOWPLACEMENT wndpl;
		::memset( &wndpl, 0, sizeof(WINDOWPLACEMENT));
		wndpl.length = sizeof(WINDOWPLACEMENT);
		::GetWindowPlacement(
			mi.GetCmdReciever(),
			&wndpl);

		BOOL bSysCmdEnabled = FALSE;
		switch(nCmdID)
		{
		case SC_CLOSE			: bSysCmdEnabled = TRUE; break;
		case SC_SIZE			:
		case SC_MOVE			:
			if( wndpl.showCmd != SW_SHOWMINIMIZED &&
				wndpl.showCmd != SW_SHOWMAXIMIZED)
				bSysCmdEnabled = TRUE;

			break;

		case SC_MINIMIZE		:
			if( wndpl.showCmd != SW_SHOWMINIMIZED )
				bSysCmdEnabled = TRUE;

			break;

		case SC_MAXIMIZE		:
			if( wndpl.showCmd != SW_SHOWMAXIMIZED )
				bSysCmdEnabled = TRUE;

			break;

		case SC_RESTORE			:
			if( wndpl.showCmd == SW_SHOWMAXIMIZED ||
				wndpl.showCmd == SW_SHOWMINIMIZED)
				bSysCmdEnabled = TRUE;

			break;

		case SC_CONTEXTHELP		:
			if(GetWindowLong( mi.GetCmdReciever(), GWL_EXSTYLE)&WS_EX_CONTEXTHELP)
				bSysCmdEnabled = TRUE;

			break;

		default					: continue;
		}

		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever),
			nCmdID);
		p_cmd->StateEnable(bSysCmdEnabled);
	}

	CWnd *pWndCmdReciever = CWnd::FromHandle(m_hWndCmdReciever);
	BOOL bOldTopLevel = m_bTopLevel;
	BOOL bOldExpandAvailable = m_bExpandAvailable;

	_Init();
	m_bTopLevel = bOldTopLevel;
	m_bExpandAvailable = bOldExpandAvailable;
	m_dwTrackFlags = dwTrackFlags;
	m_pCbPaintCombinedCookie = pCbPaintCombinedCookie;
	m_pCbPaintCombinedContent = pCbContent;

	if(!m_bTopLevel)
	{
		if(m_pWndParentMenu->m_bExpandWasPressed)
		{
			if(m_bExpandAvailable)
			{
				m_bExpandAvailable = FALSE;
				m_bExpandWasPressed = TRUE;
				_SyncItems();
			}
			else
				m_bExpandWasPressed = TRUE;
		}
	}
	else if(bForceExpandRarelyUsed)
	{
		if(m_bExpandAvailable)
		{
			m_bExpandAvailable = FALSE;
			m_bExpandWasPressed = TRUE;
			_SyncItems();
		}
		else
			m_bExpandWasPressed = TRUE;
	}

	m_ptTrack.x = x;
	m_ptTrack.y = y;

	if( m_ptTrack.x < 0 || m_ptTrack.y < 0 )
		::GetCursorPos(&m_ptTrack);

	if( lpRect != NULL )
	{
		m_rcExcludeArea = *lpRect;
		m_bExcludeAreaSpec = TRUE;
	}
	else
	{
		m_bExcludeAreaSpec = FALSE;
		m_rcExcludeArea.left = m_ptTrack.x - __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.right = m_ptTrack.x + __EXCLUDE_AREA_GAP_DX;
		m_rcExcludeArea.top = m_ptTrack.y - __EXCLUDE_AREA_GAP_DY;
		m_rcExcludeArea.bottom = m_ptTrack.y + __EXCLUDE_AREA_GAP_DY;
	}

	m_bCombineWithEA = FALSE;
	if( m_bTopLevel && m_bExcludeAreaSpec )
	{
		switch(dwTrackFlags&TPMX_COMBINE_MASK)
		{
		case TPMX_COMBINE_ANY_SUITABLE	: m_bCombineWithEA = TRUE; break;
		case TPMX_COMBINE_DEFAULT		: m_bCombineWithEA = g_PaintManager->IsMenuMustCombineExcludeArea(); break;
		}
	}

	CSize _size = _CalcTrackSize();
	BOOL bPointAdjusted = TRUE;

	if(m_bExcludeAreaSpec)
	{
		switch(m_dwTrackFlags&TPMX_ALIGN_MASK)
		{
		case TPMX_LEFTALIGN		:
			{
				m_ptTrack.x = m_rcExcludeArea.right;
				m_ptTrack.y = m_rcExcludeArea.top;
			}

			break;

		case TPMX_RIGHTALIGN	:
			{
				m_ptTrack.x = m_rcExcludeArea.left - _size.cx;
				m_ptTrack.y = m_rcExcludeArea.top;
			}

			break;

		case TPMX_TOPALIGN		:
			{
				m_ptTrack.x = m_rcExcludeArea.left;
				m_ptTrack.y = m_rcExcludeArea.bottom;
			}

			break;

		case TPMX_BOTTOMALIGN	:
			{
				m_ptTrack.x = m_rcExcludeArea.left;
				m_ptTrack.y = m_rcExcludeArea.top - _size.cy;
			}

			break;

		default					: bPointAdjusted = FALSE; break;
		}
	}

	if(!bPointAdjusted)
	{
		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_RIGHTALIGN )
			m_ptTrack.x -= _size.cx;
		else if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_CENTERALIGN )
			m_ptTrack.x -= _size.cx / 2;

		if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_BOTTOMALIGN )
			m_ptTrack.y -= _size.cy;
		else if( (m_dwTrackFlags&TPMX_ALIGN_MASK) == TPMX_VCENTERALIGN )
			m_ptTrack.y -= _size.cy / 2;
	}

	if(!_CreateHelper(pWndCmdReciever))
		return FALSE;

	if(dwTrackFlags&TPMX_SELECT_ANY)
	{
		int nItem = _GetNextItem(__NI_ANY);

		if( nItem >= 0 )
			_ItemFocusSet( nItem, FALSE, FALSE);
	}

	m_AnimationType = m_DefAnimationType;
	if(CTToolControlBar::m_bMenuTracking)
		m_AnimationType = __AT_NONE;

	m_nPlaySoundOnAnimationFinished = CTSoundPlayer::__ON_MENU_POPUP_DISPLAYED;
	_StartAnimation();

	if( m_AnimationType == __AT_NONE )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
	}

	if( m_rgnWnd.GetSafeHandle() != NULL )
	{
		CRgn rgnTmp;

		rgnTmp.CreateRectRgn( 0, 0, 0, 0);
		rgnTmp.CopyRgn(&m_rgnWnd);

		SetWindowRgn(
			(HRGN) rgnTmp.Detach(),
			FALSE);
	}

	SetWindowPos(
		&wndTop,
		-1, -1,
		-1, -1,
		SWP_NOACTIVATE|
		SWP_NOMOVE|
		SWP_NOSIZE|
		SWP_SHOWWINDOW|
		SWP_NOZORDER);

	_SetCapture();
	return TRUE;
}

BOOL CTPopupMenuWnd::_CreateHelper( CWnd *pWndCmdReciever)
{
	CRect rect = _CalcTrackRect();

	if(rect.IsRectEmpty())
		return FALSE;

	HCURSOR hCursor = ::LoadCursor( NULL, IDC_ARROW);
	CString strMenuClassName = ::AfxRegisterWndClass(
		__POPUP_WNDCLASS_STYLES__,
		hCursor,
		(HBRUSH) (COLOR_BTNFACE + 1),
		NULL);

	if(!CWnd::CreateEx(
		WS_EX_TOPMOST,
		strMenuClassName,
		NULL,
		WS_POPUP,
		rect,
		pWndCmdReciever, 0))
		return FALSE;

	_RecalcLayoutImpl();
	::SetCursor(hCursor);

	return TRUE;
}

void CTPopupMenuWnd::_GetItemRect( int nIndex, RECT &rcItem)
{
	CRect rcClient;
	_GetClientRect(&rcClient);

	rcClient.left += m_nLeftAreaWidth;
	if(m_bScrollingAvailable)
	{
		if( nIndex == IDX_SCROLL_TOP )
		{
			rcItem = m_rcScrollTop;
			return;
		}

		if( nIndex == IDX_SCROLL_BOTTOM )
		{
			rcItem = m_rcScrollBottom;
			return;
		}
	}

	if( m_bExpandAvailable && nIndex == IDX_EXPAND )
	{
		rcItem = m_rcExpandBtn;
		return;
	}

	int nCountOfItems = m_items_all.GetSize();
	if( nCountOfItems == 0 )
	{
		((CRect *) &rcItem)->SetRectEmpty();
		return;
	}

	MENUITEMDATA &mi = m_items_all[nIndex];
	if(!mi.IsDisplayed())
	{
		((CRect *) &rcItem)->SetRectEmpty();
		return;
	}

	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int y = m_nDyScrollOffset + nMenuBorderSize;
	CRect rect;

	rect.SetRectEmpty();
	rect.left = nMenuBorderSize + 1;
	rect.right = m_szFullItems.cx + nMenuBorderSize - 1;

	for( int i=0, it=0; i<nIndex && it != m_items_all.GetSize(); i++, it++)
	{
		MENUITEMDATA &mi = m_items_all[it];

		if(!mi.IsDisplayed())
			continue;

		const RECT &rcBase = mi.GetBaseRect();
		y +=  rcBase.bottom - rcBase.top;
	}

	rect.OffsetRect( 0, y);
	mi = m_items_all[it];

	const RECT &rcBase = mi.GetBaseRect();
	rect.bottom = rect.top + rcBase.bottom - rcBase.top;
	rect.OffsetRect(rcClient.TopLeft());
	rcItem = rect;
}

void CTPopupMenuWnd::_GetVisibleItems( CDC &dc, visible_items &v)
{
	v.RemoveAll();

	for( int nIndex=0, it=0; it != m_items_all.GetSize(); it++, nIndex++)
	{
		MENUITEMDATA &mi = m_items_all[it];

		if(!mi.IsDisplayed())
			continue;

		CRect rcItem;
		_GetItemRect( nIndex, rcItem);

		if(rcItem.IsRectEmpty())
			continue;

		if(!dc.RectVisible(&rcItem))
			continue;

		VisibleItemDefinion vi;
		vi.m_nIndex = nIndex;
		vi.m_nHelperCmdID = mi.GetCmdID();
		vi.m_rcItem = rcItem;

		if(mi.IsSeparator())
		{
			vi.m_bHelperIsSeparator = TRUE;
			v.Add(vi);

			continue;
		}

		if(mi.IsPopup())
		{
			vi.m_bHelperIsPopup = TRUE;
			vi.m_bRarelyUsed = mi.IsAllItemsRarelyUsed() && !mi.IsForceDisplayed();
			v.Add(vi);

			continue;
		}

		CTCmdManager::cmd *p_cmd = mi.GetCmd();
		vi.m_bRarelyUsed = p_cmd->StateIsRarelyUsed();
		v.Add(vi);
	}

	for( int vis_it = 0; vis_it < v.GetSize(); vis_it++)
	{
		VisibleItemDefinion &vi = v[vis_it];
		int it_prev = vis_it;
		int it_next = vis_it;

		if( vis_it != 0 )
		{
			it_prev--;

			if(v[it_prev].m_bRarelyUsed)
				vi.m_bRarelyUsedPrev = TRUE;
		}

		it_next++;
		if( it_next != v.GetSize() )
		{
			if(v[it_next].m_bRarelyUsed)
				vi.m_bRarelyUsedNext = TRUE;
		}

		if(vi.m_bHelperIsSeparator)
		{
			if( vi.m_bRarelyUsedPrev && vi.m_bRarelyUsedNext )
			{
				vi.m_bRarelyUsed = TRUE;
				v[it_prev].m_bRarelyUsedNext = TRUE;
				v[it_next].m_bRarelyUsedPrev = TRUE;
			}
		}
	}
}

void CTPopupMenuWnd::DRAWITEMDATA::PaintDefault( BOOL bForceNoIcon,
												 BOOL bForceNoText,
												 BOOL bForceNoCheck,
												 BOOL bForceEnabled,
												 BOOL bForceUnselected)
{
	g_PaintManager->PaintMenuItem(
		*m_pDC,
		CRect(*m_pRectItem),
		bForceNoText ? _T("") : m_pItemData->GetText(),
		bForceNoText ? _T("") : m_pItemData->GetAccelText(),
		bForceNoIcon ? NULL : HICON(*this),
		m_bPopupItem,
		bForceUnselected ? FALSE : m_pItemData->IsSelected(),
		bForceNoCheck ? FALSE : m_pItemData->GetCheck(),
		bForceNoCheck ? FALSE : m_pItemData->GetRadio(),
		m_pItemData->GetIndeterminate(),
		bForceEnabled || m_pItemData->IsEnabled(),
		m_bRarelyUsed,
		m_bRarelyUsedPreviouse,
		m_bRarelyUsedNext,
		m_pItemData->GetCmdID());
}

BOOL CTPopupMenuWnd::DRAWITEMDATA::DoOwnerDrawPainting()
{
	return ::SendMessage(
		m_pPopup->m_hWndCmdReciever,
		CTPopupMenuWnd::m_nMsgPopupDrawItem,
		0, LPARAM(*this));
}

BOOL CTPopupMenuWnd::DRAWLEFTAREADATA::DoOwnerDrawPainting()
{
	return ::SendMessage(
		m_pPopup->m_hWndCmdReciever,
		CTPopupMenuWnd::m_nMsgPopupDrawLeftArea,
		0, LPARAM(*this));
}

void CTPopupMenuWnd::_DoPaint( CDC &dcPaint, BOOL bUseBackBuffer)
{
	CRect rcRealClient;
	GetClientRect(&rcRealClient);

	CRect rcClient;
	_GetClientRect(&rcClient);

	CPalette *pOldPalette = NULL;
	CPalette *pOldPalette2 = NULL;

	if(dcPaint.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)
	{
		pOldPalette = dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		dcPaint.RealizePalette();
	}

	CTMemoryDC mdc;
	if(bUseBackBuffer)
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient);

		if( pOldPalette != NULL )
		{
			pOldPalette2 = mdc.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
			mdc.RealizePalette();
		}
	}

	CDC &dcDummyRef = mdc;
	CDC &dc = bUseBackBuffer ? dcDummyRef : dcPaint;
	CFont *pOldFont = (CFont *) dc.SelectObject(&g_PaintManager->m_FontNormal);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor());

	g_PaintManager->PaintMenuBorder(
		dc,
		&rcClient);

	CRect rcExpand;
	if(m_bExpandAvailable)
	{
		_GetItemRect(
			IDX_EXPAND,
			rcExpand);

		CRect rcExcludePart(rcExpand);
		rcExcludePart.InflateRect( 0, 1);
		rcExcludePart.left = rcRealClient.left;
		rcExcludePart.right = rcRealClient.right;
		dc.ExcludeClipRect(rcExcludePart);
	}

	CRect rcScrollBottom;
	CRect rcScrollTop;
	int nMaxScrollPos = 0;

	if(m_bScrollingAvailable)
	{
		if( m_nDyScrollOffset != 0 )
		{
			_GetItemRect(
				IDX_SCROLL_TOP,
				rcScrollTop);

			CRect rcExcludePart(rcScrollTop);
			rcExcludePart.InflateRect( 0, 1);
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect(rcExcludePart);
		}

		nMaxScrollPos = _GetMaxScrollPos();
		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			_GetItemRect(
				IDX_SCROLL_BOTTOM,
				rcScrollBottom);

			CRect rcExcludePart(rcScrollBottom);
			rcExcludePart.InflateRect( 0, 1);
			rcExcludePart.left = rcRealClient.left;
			rcExcludePart.right = rcRealClient.right;
			dc.ExcludeClipRect(rcExcludePart);
		}
	}

	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nMenuShadowSize = _GetMenuShadowSize();

	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);

		rcExcludePart.bottom = rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect(rcExcludePart);
	}

	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);

		rcExcludePart.top = rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect(rcExcludePart);
	}

	visible_items v;
	_GetVisibleItems( dc, v);

	for( int vis_it = 0; vis_it < v.GetSize(); vis_it++)
	{
		VisibleItemDefinion &vi = v[vis_it];
		MENUITEMDATA &mi = m_items_all[vi.m_nIndex];

		if(vi.m_bHelperIsSeparator)
		{
			g_PaintManager->PaintMenuSeparator(
				dc,
				vi.m_rcItem,
				m_bMenuHighlightRarely && vi.m_bRarelyUsed);

			continue;
		}

		DRAWITEMDATA _DrawItemData(
			&dc,
			&vi.m_rcItem,
			&mi,
			this,
			GetSite()->m_dwTrackFlags&TPMX_OWNERDRAW_FIXED,
			vi.m_bHelperIsPopup,
			m_bMenuHighlightRarely && vi.m_bRarelyUsed,
			m_bMenuHighlightRarely && vi.m_bRarelyUsedPrev,
			m_bMenuHighlightRarely && vi.m_bRarelyUsedNext);

		BOOL bPainted = _DrawItemData.m_bMenuOwnderDrawFixed ? _DrawItemData.DoOwnerDrawPainting() : FALSE;
		if(!bPainted)
			_DrawItemData.PaintDefault();
	}

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	dc.SelectClipRgn(NULL);

	if(m_bExpandAvailable)
	{
		BOOL bHover = rcExpand.PtInRect(point);
		BOOL bPushed = bHover;

		g_PaintManager->PaintMenuExpandButton(
			dc,
			rcExpand,
			bHover,
			bPushed);
	}

	if(m_bScrollingAvailable)
	{
		if( m_nDyScrollOffset != 0 )
		{
			BOOL bHover = rcScrollTop.PtInRect(point);
			BOOL bPushed = m_nScrollingDirection > 0;

			bPushed = bPushed || bHover;
			g_PaintManager->PaintPushButton(
				dc,
				TRUE,
				rcScrollTop,
				_T("t"),
				NULL,
				TRUE,
				FALSE,
				bPushed,
				FALSE,
				TRUE,
				TRUE,
				FALSE,
				FALSE,
				CTPaintManager::__ALIGN_HORIZ_CENTER|CTPaintManager::__ALIGN_VERT,
				&g_PaintManager->m_FontMarlett);
		}

		if( m_nDyScrollOffset != nMaxScrollPos )
		{
			BOOL bHover = rcScrollBottom.PtInRect(point);
			BOOL bPushed = m_nScrollingDirection < 0;

			bPushed = bPushed || bHover;
			g_PaintManager->PaintPushButton(
				dc,
				TRUE,
				rcScrollBottom,
				_T("u"),
				NULL,
				TRUE,
				FALSE,
				bPushed,
				FALSE,
				TRUE,
				TRUE,
				FALSE,
				FALSE,
				CTPaintManager::__ALIGN_HORIZ_CENTER|CTPaintManager::__ALIGN_VERT,
				&g_PaintManager->m_FontMarlett);
		}
	}

	if( m_nLeftAreaWidth > 0 )
	{
		CRect rcLeftArea(rcClient);

		rcLeftArea.right = rcLeftArea.left + m_nLeftAreaWidth;
		rcLeftArea.OffsetRect( nMenuBorderSize, 0);
		rcLeftArea.DeflateRect( 0, nMenuBorderSize);

		DRAWLEFTAREADATA _DrawLeftAreaData( &dc, &rcLeftArea, this);
		_DrawLeftAreaData.DoOwnerDrawPainting();
	}

	if(m_bCombineWithEA)
	{
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient(m_rcExcludeArea);
		ScreenToClient(&rcExcludeClient);

		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_nCombineAlign);

		if( m_pCbPaintCombinedContent != NULL )
		{
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc, *this,
				m_rcExcludeArea,
				m_nCombineAlign);
		}
	}

	dc.SelectObject(pOldFont);
	if(bUseBackBuffer)
	{
		if( pOldPalette2 != NULL )
			mdc.SelectPalette( pOldPalette2, FALSE);

		mdc.__Flush();
	}

	if( CTPopupMenuWnd::m_bMenuWithShadows && m_bAnimFinished && nMenuShadowSize > 0 )
	{
		dcPaint.SelectClipRgn(NULL);
		CRect rcExcludeClient( 0, 0, 0, 0);
		CRect rcExcludeScreen( 0, 0, 0, 0);
		CRect rcBaseScreen( 0, 0, 0, 0);

		if(m_bCombineWithEA)
		{
			if( CTPopupMenuWnd::m_bMenuWithShadows && m_bAnimFinished && nMenuShadowSize > 0 )
			{
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient(&rcExcludeClient);
				dcPaint.ExcludeClipRect(&rcClient);
				rcBaseScreen = rcClient;
				ClientToScreen(&rcBaseScreen);

				m_ShadowCMBA.Paint(
					dcPaint,
					rcExcludeClient,
					rcExcludeScreen,
					rcBaseScreen,
					nMenuShadowSize);

				dcPaint.SelectClipRgn(NULL);
			}

			CRect rcExcludePart(m_rcExcludeArea);
			ScreenToClient(&rcExcludePart);
			dcPaint.ExcludeClipRect(&rcExcludePart);
		}

		m_ShadowMain.Paint(
			dcPaint,
			rcClient,
			rcBaseScreen,
			rcExcludeScreen,
			nMenuShadowSize);
	}

	if( pOldPalette != NULL )
		dcPaint.SelectPalette( pOldPalette, FALSE);
}

#if _MFC_VER < 0x700
void CTPopupMenuWnd::OnActivateApp( BOOL bActive, HTASK hTask)
#else
void CTPopupMenuWnd::OnActivateApp( BOOL bActive, DWORD hTask)
#endif
{
	_OnCancelMode();
}

BOOL CTPopupMenuWnd::_StartScrolling( int nButtonIndex)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if( nButtonIndex == IDX_SCROLL_TOP ||
		nButtonIndex == IDX_SCROLL_BOTTOM)
	{
		_ItemFocusDelay();
		_ItemFocusCancel(FALSE);

		m_nCurIndex = nButtonIndex;
		Invalidate(FALSE);

		SetTimer(
			ID_TIMER_SCROLLING,
			ID_PERIOD_SCROLLING,
			NULL);

		_SetCapture();
		return TRUE;
	}

	return FALSE;
}

BOOL CTPopupMenuWnd::_OnMouseClick( UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if(!m_bAnimFinished)
		return TRUE;

	CTPopupMenuSite &_site = GetSite();
	if( _site.IsShutdownMode() ||
		_site.IsEmpty() ||
		_site.GetAnimated() != NULL )
		return TRUE;

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	if(!rcWindow.PtInRect(point))
	{
		if( m_pWndParentMenu != NULL &&
			m_pWndParentMenu->GetSafeHwnd() != NULL)
		{
			ClientToScreen(&point);
			m_pWndParentMenu->ScreenToClient(&point);

			HWND hWndOwn = GetSafeHwnd();
			CTPopupMenuWnd *pWndParentMenu = m_pWndParentMenu;

			if(pWndParentMenu->_OnMouseClick( nFlags, point))
			{
				if(::IsWindow(hWndOwn))
					_OnCancelMode();

				return TRUE;
			}

			return FALSE;
		}

		if( nFlags == WM_RBUTTONUP ||
			nFlags == WM_LBUTTONUP)
			return TRUE;

		_OnCancelMode();
		return FALSE;
	}

	int nHitTest = _HitTest(point);
	if(_StartScrolling(nHitTest))
		return TRUE;

	if( nHitTest < 0 )
	{
		if( nHitTest == IDX_SCROLL_TOP ||
			nHitTest == IDX_SCROLL_BOTTOM)
			return TRUE;

		if( nHitTest == IDX_EXPAND )
		{
			if( !m_bExpandWasPressed && m_bExpandAvailable )
				_DoExpand();

			return TRUE;
		}

		if( nFlags != WM_RBUTTONUP && nFlags != WM_LBUTTONUP )
		{
			_OnCancelMode();
			return TRUE;
		}

		return FALSE;
	}

	MENUITEMDATA &mi = _GetItemRef(nHitTest);
	if( !mi.IsEnabled() || mi.IsSeparator() )
		return TRUE;

	if(mi.IsPopup())
	{
		if( mi.GetPopup()->GetSafeHwnd() != NULL )
			return FALSE;

		_ItemFocusSet(
			nHitTest,
			TRUE,
			TRUE);

		return TRUE;
	}

	if(mi.IsExecutableCmdID())
		_EndSequence( mi.GetCmdID(), mi.GetCmdReciever());
	else
		_OnCancelMode();

	return TRUE;
}

void CTPopupMenuWnd::_EndSequence( UINT nCmdID, HWND hWndDeliver)
{
	CTPopupMenuSite &_site = GetSite();
	UINT *lpnResultCmdID = _site.GetTargetCmdIdPtr();

	if( lpnResultCmdID != NULL )
		*lpnResultCmdID = nCmdID;

	CTCmdManager::cmd *p_cmd = NULL;
	if( nCmdID > 0 && (m_dwTrackFlags&TPMX_NO_WM_COMMAND) == 0 )
		p_cmd = g_CmdManager->CmdGetPtr( g_CmdManager->ProfileNameFromWnd(m_hWndCmdReciever), nCmdID);

	CTPopupMenuWnd *pTopPopup = _site.GetInstance();
	HWND hWndTopPopup = pTopPopup->GetSafeHwnd();

	if( hWndDeliver == NULL )
		hWndDeliver = m_hWndCmdReciever;

	CWnd *pTopLevelParent = GetTopLevelParent();
	HWND hWndTopLevelParent = pTopLevelParent->GetSafeHwnd();

	CRect rcBkUpdate( 0, 0, 0, 0);
	CTPopupMenuWnd *pPopup = this;

	for(; pPopup; pPopup = pPopup->m_pWndParentMenu)
	{
		CRect rcPopup;
		pPopup->GetWindowRect(&rcPopup);

		if(rcBkUpdate.IsRectEmpty())
			rcBkUpdate = rcPopup;
		else
		{
			CRect rcPrev(rcBkUpdate);
			rcBkUpdate.UnionRect( &rcPrev, &rcPopup);
		}

		if( pPopup->m_wndToolTip.GetSafeHwnd() != NULL &&
			::IsWindow(pPopup->m_wndToolTip.GetSafeHwnd()) &&
			pPopup->m_wndToolTip.IsWindowVisible())
		{
			CRect rcToolTip;
			pPopup->m_wndToolTip.GetWindowRect(&rcToolTip);

			CRect rcPrev(rcBkUpdate);
			rcBkUpdate.UnionRect( &rcPrev, &rcToolTip);
		}

		pPopup->ShowWindow(SW_HIDE);
	}

	if( !rcBkUpdate.IsRectEmpty() &&
		hWndTopLevelParent != NULL &&
		::IsWindow(hWndTopLevelParent))
	{
		::ScreenToClient( hWndTopLevelParent, (LPPOINT) &rcBkUpdate);
		::ScreenToClient( hWndTopLevelParent, ((LPPOINT) &rcBkUpdate) + 1);

		if(((DWORD) ::GetWindowLong( hWndTopLevelParent, GWL_EXSTYLE))&WS_EX_LAYOUTRTL)
			CRect::SwapLeftRight(&rcBkUpdate);

		::RedrawWindow(
			hWndTopLevelParent,
			&rcBkUpdate,
			NULL,
			RDW_INVALIDATE|
			RDW_ERASE|
			RDW_UPDATENOW|
			RDW_ERASENOW|
			RDW_VALIDATE|
			RDW_FRAME|
			RDW_ALLCHILDREN);
	}

	if(::IsWindow(hWndTopPopup))
		::PostMessage( hWndTopPopup, WM_CLOSE, 0, 0);

	if( p_cmd != NULL && ::IsWindow(hWndDeliver) )
	{
		p_cmd->Deliver(hWndDeliver);
		g_SoundPlayer->PlaySound(CTSoundPlayer::__ON_MENU_CMD_CLICKED);
	}

	if(CTToolControlBar::m_bMenuTracking)
		CTToolControlBar::_CloseTrackingMenus();
}

void CTPopupMenuWnd::OnCancelMode()
{
	CTPopupMenuSite &_site = GetSite();
	_site.DoneInstance();
}

void CTPopupMenuWnd::_FreeWinObjects()
{
	if( m_rgnWnd.GetSafeHandle() != NULL )
		m_rgnWnd.DeleteObject();

	m_ShadowMain.Destroy();
	m_ShadowCMBA.Destroy();
	_SurfacesDelete();

	if(!::IsWindow(GetSafeHwnd()))
		return;

	KillTimer(ID_TIMER_ANIMATION);
}

void CTPopupMenuWnd::_OnCancelMode( BOOL bNcDestroy)
{
	_FreeWinObjects();
	_ReleaseCapture();

	for( int i=0; i<m_items_all.GetSize(); i++)
	{
		MENUITEMDATA &mi = m_items_all[i];
		mi.SetSelected(FALSE);

		if( mi.IsPopup() && mi.GetPopup()->GetSafeHwnd() != NULL )
			mi.GetPopup()->_OnCancelMode();
	}
	m_nCurIndex = IDX_NOTHING;

	if( bNcDestroy || !::IsWindow(GetSafeHwnd()) )
		return;

	ShowWindow(SW_HIDE);
	PostMessage(WM_CLOSE);

	if( CTToolControlBar::m_bMenuTracking && m_bTopLevel )
		CTToolControlBar::_CloseTrackingMenus();
}

void CTPopupMenuWnd::_ItemFocusSet( int nCurIndex,
								    BOOL bEnableDropChild,
									BOOL bRepaint)
{
	if( m_nCurIndex != nCurIndex )
	{
		_ItemFocusCancel(FALSE);
		m_nCurIndex = nCurIndex;

		if( m_nCurIndex < 0 )
			return;
	}

	MENUITEMDATA &mi = _GetItemRef(m_nCurIndex);
	BOOL bWasSet = FALSE;

	if( mi.IsEnabled() && mi.IsDisplayed() )
	{
		mi.SetSelected(TRUE);
		bWasSet = TRUE;

		int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
		if(m_bScrollingAvailable)
		{
			int nMaxScrollPos = _GetMaxScrollPos();
			int nMetric;

			CRect rcScrollBtnDown;
			CRect rcScrollBtnUp;
			CRect rcClient;
			CRect rcItem;

			_GetItemRect( IDX_SCROLL_TOP, rcScrollBtnUp);
			_GetItemRect( IDX_SCROLL_BOTTOM, rcScrollBtnDown);
			_GetClientRect(rcClient);
			_GetItemRect( m_nCurIndex, rcItem);
			nMetric = rcClient.bottom;

			if( m_nDyScrollOffset != nMaxScrollPos )
			{
				nMetric -= rcScrollBtnDown.Height();

				if(m_bExpandAvailable)
				{
					CRect rcExpand;

					_GetItemRect( IDX_EXPAND, rcExpand);
					nMetric -= rcExpand.Height();
				}
			}

			if( rcItem.bottom >= nMetric )
				m_nDyScrollOffset -= rcItem.bottom - nMetric + nMenuBorderSize * 2;

			nMetric = rcClient.top;
			if( m_nDyScrollOffset != 0 )
				nMetric += rcScrollBtnUp.Height();

			if( rcItem.top <= nMetric )
				m_nDyScrollOffset += nMetric - rcItem.top + nMenuBorderSize * 2;

			if( m_nDyScrollOffset > 0 )
				m_nDyScrollOffset = 0;
			else if( m_nDyScrollOffset < nMaxScrollPos )
				m_nDyScrollOffset = nMaxScrollPos;
		}
	}

	if(bWasSet)
	{
		CWnd *pWndOwner = GetOwner();

		if( pWndOwner != NULL )
		{
			WPARAM nStatusUpdateValue = (WPARAM) AFX_IDS_IDLEMESSAGE;
			UINT nCmdID = mi.GetCmdID();

			if(CTCmdManager::IsCommand(nCmdID))
				nStatusUpdateValue = (WPARAM) nCmdID;

			pWndOwner->SendMessage(
				WM_SETMESSAGESTRING,
				nStatusUpdateValue);
		}
	}

	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
		CClientDC dc(this);
		_DoPaint(dc);
	}

	if( bWasSet &&
		bEnableDropChild &&
		mi.IsPopup() &&
		mi.GetPopup()->GetSafeHwnd() == NULL )
	{
		_ReleaseCapture();
		CPoint point;
		CRect rcItem;

		_GetItemRect(
			m_nCurIndex,
			rcItem);
		rcItem.left -= m_nLeftAreaWidth;

		point.x = rcItem.right;
		point.y = rcItem.top;
		ClientToScreen(&point);
		ClientToScreen(&rcItem);

		if(mi.GetPopup()->_TrackPopupMenu( 0, point.x, point.y, &rcItem))
		{
			int nCountOfPopupItems = mi.GetPopup()->m_items_all.GetSize();

			if( nCountOfPopupItems > 0 )
			{
				int nIndex = mi.GetPopup()->_GetNextItem(__NI_ANY);
				if( nIndex >= 0 )
					mi.GetPopup()->_ItemFocusSet( nIndex, FALSE, GetSite().GetAnimated() == NULL);
			}
		}
	}
}

BOOL CTPopupMenuWnd::_CoolTipIsVisible()
{
	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if( CTPopupMenuSite::m_DefPopupMenuSite.GetCapture() != this )
		return FALSE;

	HWND hWndToolTip = m_wndToolTip.GetSafeHwnd();
	if( hWndToolTip != NULL &&
		::IsWindow(hWndToolTip) &&
		::IsWindowVisible(hWndToolTip))
		return TRUE;

	return FALSE;
}

void CTPopupMenuWnd::_CoolTipHide( BOOL bAdvOperation)
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	HWND hWndToolTip = m_wndToolTip.GetSafeHwnd();
	if( hWndToolTip == NULL || !::IsWindow(hWndToolTip) )
		return;

	if( bAdvOperation && GetSite().GetAnimated() == NULL )
	{
		m_AnimationType = __AT_CONTENT_DISPLAY;
		_StartAnimation();
	}

	m_wndToolTip.Hide();
}

void CTPopupMenuWnd::_ItemFocusCancel( BOOL bRepaint)
{
	_CoolTipHide();

	if( m_nCurIndex < 0 )
		m_nCurIndex = IDX_NOTHING;
	else
	{
		MENUITEMDATA &mi = _GetItemRef(m_nCurIndex);
		mi.SetSelected(FALSE);
		m_nCurIndex = IDX_NOTHING;

		if(mi.IsPopup())
			mi.GetPopup()->_OnCancelMode();
	}

	if( bRepaint && ::IsWindowVisible(GetSafeHwnd()) )
	{
		CClientDC dc(this);
		_DoPaint(dc);
	}
}

BOOL CTPopupMenuWnd::_OnMouseMove( UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if(!m_bAnimFinished)
		return TRUE;

	CTPopupMenuSite &_site = GetSite();
	if( _site.IsShutdownMode() ||
		_site.IsEmpty() ||
		_site.GetAnimated() != NULL )
		return TRUE;

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	if(!rcWindow.PtInRect(point))
	{
		if( m_pWndParentMenu != NULL &&
			m_pWndParentMenu->GetSafeHwnd() != NULL )
		{
			ClientToScreen(&point);
			m_pWndParentMenu->ScreenToClient(&point);

			if(m_pWndParentMenu->_OnMouseMove( nFlags, point))
			{
				_OnCancelMode();
				return TRUE;
			}
		}

		return FALSE;
	}

	int nCurIndex = _HitTest(point);
	if( nCurIndex == IDX_EXPAND )
	{
		_ItemFocusCancel(TRUE);
		_SetCapture();

		if( m_nWaitingExpandTickCount == 0 )
		{
			SetTimer(
				ID_TIMER_DELAY_EXPAND,
				ID_TOTAL_DELAY_EXPAND / ID_FREQ_DELAY_EXPAND,
				NULL);

			m_nWaitingExpandTickCount = 1;
		}

		return TRUE;
	}

	if(_StartScrolling(nCurIndex))
		return TRUE;

	if( nCurIndex >= 0 )
	{
		MENUITEMDATA &mi = _GetItemRef(nCurIndex);

		if( !mi.IsEnabled() || mi.IsSeparator() )
			return FALSE;

		int nOldCurIndex = m_nCurIndex;
		if( nOldCurIndex != nCurIndex )
		{
			_ItemFocusDelay();
			_SetCapture();

			_ItemFocusSet(
				nCurIndex,
				FALSE,
				TRUE);

			MENUITEMDATA &mi = _GetItemRef(nCurIndex);
			if(mi.IsPopup())
			{
				m_wndToolTip.Hide();
				if( mi.GetPopup()->GetSafeHwnd() == NULL )
					_ItemFocusDelay(nCurIndex);

				return TRUE;
			}

			if( m_bMenuShowCoolTips &&
				nCurIndex != nOldCurIndex &&
				GetSite().GetCapture() == this )
			{
				CString strTipText = mi.GetTip();

				if(!strTipText.IsEmpty())
				{
					CRect rcItem;

					_GetItemRect( nCurIndex, rcItem);
					ClientToScreen(&rcItem);

					m_wndToolTip.SetText((LPCTSTR) strTipText);
					m_wndToolTip.Show( this, rcItem);
				}
			}
		}

		return nOldCurIndex != nCurIndex;
	}

	return FALSE;
}

void CTPopupMenuWnd::_ItemFocusDelay( int nItemIndex)
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	if( m_nDelayedFocusItemIndex != nItemIndex &&
		m_nDelayedFocusItemIndex >= 0 )
		KillTimer(ID_TIMER_ITEM_FOCUS_DELAY);

	if( nItemIndex < 0 )
	{
		m_nDelayedFocusItemIndex = IDX_NOTHING;
		return;
	}

	if( m_nDelayedFocusItemIndex == nItemIndex )
		return;

	m_nDelayedFocusItemIndex = nItemIndex;
	SetTimer(
		ID_TIMER_ITEM_FOCUS_DELAY,
		ID_PERIOD_ITEM_FOCUS_DELAY,
		NULL);
}

int CTPopupMenuWnd::_GetMaxScrollPos()
{
	CRect rcClient;
	_GetClientRect(&rcClient);

	int nClientHeight = rcClient.Height();
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nMenuShadowSize = _GetMenuShadowSize();
	int nMaxScrollPos = m_szFullItems.cy;

	nMaxScrollPos += nMenuBorderSize * 2;
	nMaxScrollPos += nMenuShadowSize;
	nMaxScrollPos -= nClientHeight;

	if(m_bExpandAvailable)
		nMaxScrollPos += _GetSpecBtnHeight();

	return -nMaxScrollPos;
}

void CTPopupMenuWnd::OnTimer( UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_TIMER_DELAY_EXPAND			:
		{
			if( m_nWaitingExpandTickCount < ID_TOTAL_DELAY_EXPAND )
			{
				if( m_nWaitingExpandTickCount <= 1 )
					m_nWaitingExpandTickCount = 0;

				m_nWaitingExpandTickCount += ID_FREQ_DELAY_EXPAND;
				return;
			}

			KillTimer(ID_TIMER_DELAY_EXPAND);
			m_nWaitingExpandTickCount = 0;

			if( m_bExpandAvailable &&
				!m_bExpandWasPressed &&
				GetSite().GetAnimated() == NULL )
			{
				CPoint point;

				::GetCursorPos(&point);
				ScreenToClient(&point);

				CRect rcExpand;
				_GetItemRect( IDX_EXPAND, rcExpand);

				if(rcExpand.PtInRect(point))
					_DoExpand();
			}
		}

		return;

	case ID_TIMER_SCROLLING				:
		{
			_ItemFocusDelay();

			if( m_nCurIndex >= 0 )
				_ItemFocusCancel(TRUE);

			CPoint point;
			::GetCursorPos(&point);
			ScreenToClient(&point);

			m_nScrollingDirection = 0;
			CRect rcItem;

			_GetItemRect( IDX_SCROLL_TOP, rcItem);
			if(rcItem.PtInRect(point))
				m_nScrollingDirection = __SCROLLING_PIXEL_STEP;
			else
			{
				_GetItemRect( IDX_SCROLL_BOTTOM, rcItem);

				if(rcItem.PtInRect(point))
					m_nScrollingDirection = -__SCROLLING_PIXEL_STEP;
			}

			m_nDyScrollOffset += m_nScrollingDirection;
			BOOL bKillThisTimer = m_nScrollingDirection == 0;

			if( m_nDyScrollOffset > 0 )
			{
				m_nDyScrollOffset = 0;
				bKillThisTimer = TRUE;
			}
			else
			{
				int nMaxScrollPos = _GetMaxScrollPos();

				if( m_nDyScrollOffset < nMaxScrollPos )
				{
					m_nDyScrollOffset = nMaxScrollPos;
					bKillThisTimer = TRUE;
				}
			}

			if(bKillThisTimer)
				KillTimer(ID_TIMER_SCROLLING);

			Invalidate(FALSE);
		}

		return;

	case ID_TIMER_ITEM_FOCUS_DELAY		:
		{
			if( GetSite().GetAnimated() != NULL )
				return;

			KillTimer(ID_TIMER_ITEM_FOCUS_DELAY);
			if( m_nDelayedFocusItemIndex >= 0 )
			{
				CRect rcItem;
				_GetItemRect( m_nDelayedFocusItemIndex, rcItem);

				CPoint point;
				::GetCursorPos(&point);
				ScreenToClient(&point);

				if(rcItem.PtInRect(point))
				{
					_ItemFocusSet(
						m_nDelayedFocusItemIndex,
						TRUE,
						TRUE);
				}
			}

			m_nDelayedFocusItemIndex = IDX_NOTHING;
		}

		return;

	default								: CTPopupBaseWnd::OnTimer(nIDEvent); break;
	}
}

CTPopupMenuWnd *CTPopupMenuWnd::_GetCapture()
{
	return GetSite().GetCapture();
}

void CTPopupMenuWnd::_SetCapture()
{
	CTPopupMenuSite &_site = GetSite();

	if( GetSafeHwnd() != NULL )
		_site.SetCapture(this);
	else
		_site.SetCapture(NULL);
}

void CTPopupMenuWnd::_ReleaseCapture()
{
	CTPopupMenuSite &_site = GetSite();

	if( _site.IsEmpty() || _site.IsShutdownMode() )
		return;

	if( _site.GetCapture() == this )
		_site.SetCapture(NULL);
}

int CTPopupMenuWnd::_GetNextItem( next_item next)
{
	int nCountOfItems = m_items_all.GetSize();
	BOOL bCanExpand = FALSE;

	if( m_bExpandAvailable && !m_bExpandWasPressed )
	{
		if(IsAllItemsRarelyUsed())
			return IDX_EXPAND;

		bCanExpand = TRUE;
	}

	if( next == __NI_NOTHING ||  nCountOfItems == 0 )
	{
		if(bCanExpand)
			return IDX_EXPAND;

		return IDX_NOTHING;
	}

	if( next == __NI_ANY && m_nCurIndex >= 0 )
		return m_nCurIndex;

	if( m_nCurIndex < 0 )
	{
		for( int it = 0; it < m_items_all.GetSize(); it++)
		{
			MENUITEMDATA &mi = m_items_all[it];

			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}

		if(bCanExpand)
			return IDX_EXPAND;

		return IDX_NOTHING;
	}

	if( next == __NI_NEXT )
	{
		if( nCountOfItems < 2 )
		{
			if(bCanExpand)
				return IDX_EXPAND;

			return IDX_NOTHING;
		}

		for( int it = m_nCurIndex + 1; it < m_items_all.GetSize(); it++)
		{
			MENUITEMDATA &mi = m_items_all[it];

			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();
		}

		if(bCanExpand)
			return IDX_EXPAND;

		if( m_nCurIndex == 0 )
			return IDX_NOTHING;

		int it2 = m_nCurIndex - 1;
		it = 0;

		do
		{
			MENUITEMDATA &mi = m_items_all[it];

			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();

			it++;
		} while( it != it2 );

		return IDX_NOTHING;
	}

	if( next == __NI_PREV )
	{
		if( nCountOfItems < 2 )
			return IDX_NOTHING;

		if( m_nCurIndex != 0 )
		{
			int nIndex = m_nCurIndex - 1;

			for( int it = nIndex; TRUE; it--, nIndex--)
			{
				MENUITEMDATA &mi = m_items_all[it];

				if( mi.IsEnabled() && mi.IsDisplayed() )
					return mi.GetIndex();

				if( nIndex == 0 )
					break;
			}
		}

		int nIndex = m_nCurIndex + 1;
		int it2 = nIndex;
		int it = m_items_all.GetSize() - 1;

		do
		{
			MENUITEMDATA &mi = m_items_all[it];

			if( mi.IsEnabled() && mi.IsDisplayed() )
				return mi.GetIndex();

			it--;
		} while( it != it2 );

		return IDX_NOTHING;
	}

	return IDX_NOTHING;
}

BOOL CTPopupMenuWnd::_OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( GetSafeHwnd() == NULL )
		return TRUE;

	if(!m_bAnimFinished)
		return TRUE;

	CTPopupMenuSite &_site = GetSite();
	if( _site.IsShutdownMode() ||
		_site.IsEmpty() ||
		_site.GetAnimated() != NULL )
		return TRUE;

	next_item next = __NI_NOTHING;
	switch(nChar)
	{
	case VK_MENU	: _EndSequence(); return TRUE;
	case VK_RETURN	:
	case VK_SPACE	:
		{
			if( m_nCurIndex < 0 )
				return TRUE;

			MENUITEMDATA &mi = _GetItemRef(m_nCurIndex);
			if(mi.IsExecutableCmdID())
			{
				_EndSequence( mi.GetCmdID(), mi.GetCmdReciever());
				return TRUE;
			}
		}

	case VK_RIGHT	:
		{
			BOOL bSendChangeMenu = FALSE;

			if( m_nCurIndex < 0 )
			{
				if( m_items_all.GetSize() == 0 )
					return TRUE;

				bSendChangeMenu = TRUE;
			}
			else
			{
				MENUITEMDATA &mi = _GetItemRef(m_nCurIndex);

				if(!mi.IsPopup())
					bSendChangeMenu = TRUE;
				else if( mi.GetPopup()->GetSafeHwnd() == NULL )
				{
					_ItemFocusDelay();
					_ItemFocusSet(
						m_nCurIndex,
						TRUE,
						TRUE);
				}
			}

			if( bSendChangeMenu && nChar == VK_RIGHT )
			{
				if(CTToolControlBar::m_bMenuTracking)
				{
					::PostMessage(
						m_hWndCmdReciever,
						CTPopupMenuWnd::m_nMsgPopupNext,
						0, LPARAM(this));
				}

				return TRUE;
			}
		}

		break;

	case VK_LEFT	:
		if( m_pWndParentMenu == NULL )
		{
			if(CTToolControlBar::m_bMenuTracking)
			{
				::PostMessage(
					m_hWndCmdReciever,
					CTPopupMenuWnd::m_nMsgPopupPrev,
					0, LPARAM(this));
			}

			return TRUE;
		}
		else
		{
			int nParentCurIndex = m_pWndParentMenu->m_nCurIndex;

			m_pWndParentMenu->_ItemFocusCancel(FALSE);
			m_pWndParentMenu->_ItemFocusSet(
				nParentCurIndex,
				FALSE,
				TRUE);
			m_pWndParentMenu->_SetCapture();

			return TRUE;
		}

	case VK_ESCAPE	: _EndSequence(); return TRUE;
	case VK_UP		: next = __NI_PREV; break;
	case VK_DOWN	:
		if(IsKeyPressed(VK_CONTROL))
		{
			if(m_bExpandAvailable)
				_DoExpand();

			return TRUE;
		}
		next = __NI_NEXT;

		break;

	case VK_NEXT	: next = __NI_PAGE_DOWN; break;
	case VK_PRIOR	: next = __NI_PAGE_UP; break;
	default			:
		{
			if( IsKeyPressed(VK_SHIFT) ||
				IsKeyPressed(VK_CONTROL) ||
				IsKeyPressed(VK_MENU) ||
				(nFlags & (1 << 29)) != 0 )
				return FALSE;

			BYTE lpKeyState[256];
			::GetKeyboardState(lpKeyState);

			UINT wScanCode = ::MapVirtualKey( nChar, 0);
			HKL hKeyboardLayout = ::GetKeyboardLayout(::AfxGetThread()->m_nThreadID);

#if (defined _UNICODE)
			TCHAR szChar[2] = { _T('\0'), _T('\0')};

			::ToUnicodeEx(
				nChar,
				wScanCode,
				lpKeyState,
				szChar,
				1, 1,
				hKeyboardLayout);

			WORD nMapped = WORD(szChar[0]);
#else
			WORD nMapped = 0;
			::ToAsciiEx(
				nChar,
				wScanCode,
				lpKeyState,
				&nMapped,
				1, hKeyboardLayout);
#endif
			int nNextIdx = ItemFindByAccessChar(
				(TCHAR) nMapped,
				m_nCurIndex);

			if( nNextIdx < 0 && m_nCurIndex >= 0 && ItemFindByAccessChar( (TCHAR) nMapped, -1) == m_nCurIndex )
			{
				MENUITEMDATA &mi = _GetItemRef(m_nCurIndex);

				if(mi.IsExecutableCmdID())
				{
					_EndSequence( mi.GetCmdID(), mi.GetCmdReciever());
					return TRUE;
				}
			}

			if( m_nCurIndex != nNextIdx && nNextIdx >= 0 )
			{
				_ItemFocusSet( nNextIdx, TRUE, TRUE);

				if( ItemFindByAccessChar( (TCHAR) nMapped, nNextIdx) < 0 )
				{
					MENUITEMDATA &mi = _GetItemRef(nNextIdx);

					if(mi.IsExecutableCmdID())
					{
						_EndSequence( mi.GetCmdID(), mi.GetCmdReciever());
						return TRUE;
					}
				}

				return TRUE;
			}
		}

		break;
	}

	if( next != __NI_NOTHING )
	{
		int nItemIndex = _GetNextItem(next);

		if( m_items_all.GetSize() > 0 &&
			nItemIndex >= 0 &&
			nItemIndex != m_nCurIndex )
		{
			_ItemFocusSet(
				nItemIndex,
				FALSE,
				TRUE);

			return TRUE;
		}

		if( nItemIndex == IDX_EXPAND )
		{
			int nItemIndex = m_nCurIndex;
			_DoExpand();

			if( nItemIndex >= 0 )
			{
				_ItemFocusSet(
					nItemIndex,
					FALSE,
					GetSite().GetAnimated() == NULL);

				int item = _GetNextItem(__NI_NEXT);
				_ItemFocusSet(
					item > 0 ? item : nItemIndex,
					FALSE,
					GetSite().GetAnimated() == NULL);
			}

			return TRUE;
		}
	}

	return FALSE;
}

int CTPopupMenuWnd::ItemFindByText( LPCTSTR strText,
								    int nStartIdx,
									BOOL bRestartAt0) const
{
	if( strText == NULL || strText[0] == _T('\0') )
		return -1;

	int nCount = m_items_all.GetSize();
	int nIdx = nStartIdx >= 0 ? nStartIdx + 1 : 0;

	for(; nIdx < nCount; nIdx++)
	{
		const MENUITEMDATA &mi = _GetItemRef(nIdx);

		if( _tcscmp( strText, mi.GetText()) == 0 && nStartIdx != nIdx )
			return nIdx;
	}

	if( nStartIdx == 0 )
		return -1;

	for( nIdx = 0; nIdx < nStartIdx; nIdx++)
	{
		const MENUITEMDATA &mi = _GetItemRef(nIdx);

		if( _tcscmp( strText, mi.GetText()) == 0 && nStartIdx != nIdx )
			return nIdx;
	}

	return -1;
}

int CTPopupMenuWnd::ItemFindByAccessChar( TCHAR nAccess,
										  int nStartIdx,
										  BOOL bRestartAt0) const
{
	if( nAccess == 0 )
		return -1;

	TCHAR szChar[2] = { nAccess, _T('\0')};
	::CharUpper(szChar);

	int nAccelSearch = szChar[0];
	if( nAccelSearch == 0 )
		return -1;

	int nCount = m_items_all.GetSize();
	int nIdx = nStartIdx >= 0 ? nStartIdx + 1 : 0;

	for(; nIdx < nCount; nIdx++)
	{
		const MENUITEMDATA &mi = _GetItemRef(nIdx);

		if( !mi.AccelCharIsSet() ||
			!mi.IsDisplayed() ||
			!mi.IsEnabled() )
			continue;

		TCHAR nAccel = mi.AccelCharGet();
		if( nAccelSearch == nAccel && nStartIdx != nIdx )
			return nIdx;
	}

	if( nStartIdx == 0 )
		return -1;

	for( nIdx = 0; nIdx < nStartIdx; nIdx++)
	{
		const MENUITEMDATA &mi = _GetItemRef(nIdx);

		if( !mi.AccelCharIsSet() ||
			!mi.IsDisplayed() ||
			!mi.IsEnabled() )
			continue;

		TCHAR nAccel = mi.AccelCharGet();
		if( nAccelSearch == nAccel && nStartIdx != nIdx )
			return nIdx;
	}

	return -1;
}

BOOL CTPopupMenuWnd::IsCmdKeyActivation( LPARAM lParam)
{
	if( (lParam & (1 << 29)) != 0 )
		return TRUE;

	static int nTestKeys[] = {
		VK_MENU,
		VK_RMENU,
		VK_LMENU,
		VK_CONTROL,
		VK_RCONTROL,
		VK_LCONTROL,
		VK_SHIFT,
		VK_RSHIFT,
		VK_LSHIFT};

	for( int i=0; i<sizeof(nTestKeys)/sizeof(int); i++)
	{
		if(IsKeyPressed(nTestKeys[i]))
			return TRUE;
	}

	return FALSE;
}

int CTPopupMenuWnd::ItemGetCount() const
{
	return m_items_all.GetSize();
}

int CTPopupMenuWnd::ItemFindPosForCmdID( UINT nCmdID, int nPosStart) const
{
	int nPos = nPosStart < 0 ? 0 : nPosStart + 1;
	int nCount = ItemGetCount();

	for(; nPos < nCount; nPos++)
	{
		UINT nCmdID2 = ItemGetCmdID(nPos);

		if( nCmdID == nCmdID2 )
			return nPos;
	}

	return -1;
}

UINT CTPopupMenuWnd::ItemGetCmdID( int nPos) const
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return TYPE_SEPARATOR;

	const MENUITEMDATA &mi = _GetItemRef(nPos);
	UINT nCmdID = mi.GetCmdID();

	return nCmdID;
}

BOOL CTPopupMenuWnd::ItemSetPopupIcon( int nPos, HICON hIcon)
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return FALSE;

	MENUITEMDATA &mi = _GetItemRef(nPos);
	return mi.SetPopupIcon(hIcon);
}

BOOL CTPopupMenuWnd::ItemSetPopupText( int nPos, LPCTSTR strText)
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return FALSE;

	MENUITEMDATA &mi = _GetItemRef(nPos);
	return mi.SetPopupText(strText);
}

BOOL CTPopupMenuWnd::ItemSetPopupAccelText( int nPos, LPCTSTR strText)
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return FALSE;

	MENUITEMDATA &mi = _GetItemRef(nPos);
	return mi.SetPopupAccelText(strText);
}

HICON CTPopupMenuWnd::ItemGetIcon( int nPos) const
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return NULL;

	MENUITEMDATA &mi = (MENUITEMDATA&) _GetItemRef(nPos);
	return mi.GetIcon();
}

CString CTPopupMenuWnd::ItemGetText( int nPos) const
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return CString(_T(""));

	const MENUITEMDATA &mi = _GetItemRef(nPos);
	return CString(mi.GetText());
}

CString CTPopupMenuWnd::ItemGetAccelText( int nPos) const
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return CString(_T(""));

	const MENUITEMDATA &mi = _GetItemRef(nPos);
	return CString(mi.GetAccelText());
}

BOOL CTPopupMenuWnd::ItemIsDisplayed( int nPos) const
{
	const MENUITEMDATA &mi = _GetItemRef(nPos);
	return mi.IsDisplayed();
}

void CTPopupMenuWnd::ItemSetDisplayed( int nPos, BOOL bForceDisplayed)
{
	MENUITEMDATA &mi = _GetItemRef(nPos);

	mi.SetForceDisplayed(bForceDisplayed);
	_SyncItems();
}

CTPopupMenuWnd *CTPopupMenuWnd::ItemGetPopup( int nPos)
{
	const CTPopupMenuWnd *pChild = ((const CTPopupMenuWnd *) this)->ItemGetPopup(nPos);

	if( pChild == NULL )
		return NULL;

	return const_cast<CTPopupMenuWnd *> (pChild);
}

const CTPopupMenuWnd *CTPopupMenuWnd::ItemGetPopup( int nPos) const
{
	int nCountOfItems = ItemGetCount();

	if( nPos < 0 || nPos >= nCountOfItems )
		return NULL;

	const MENUITEMDATA &mi = _GetItemRef(nPos);
	UINT nCmdID = mi.GetCmdID();

	if( nCmdID != TYPE_POPUP )
		return NULL;

	return mi.GetPopup();
}

void CTPopupMenuWnd::_DoExpand()
{
	_ItemFocusDelay();
	_CoolTipHide(FALSE);

	if( GetSite().GetAnimated() != NULL ||
		_CoolTipIsVisible() )
		return;

	m_bExpandWasPressed = TRUE;
	CSize szFullItems = m_szFullItems;

	_SyncItems();
	_RecalcLayoutImpl();

	if( m_szFullItems != szFullItems )
	{
		CRect rcWndOld;

		GetWindowRect(&rcWndOld);
		SetWindowRgn( NULL, FALSE);
		ShowWindow(SW_HIDE);

		if(!rcWndOld.IsRectEmpty())
		{
			CWnd *pWndTLP = GetTopLevelParent();

			if( pWndTLP != NULL && pWndTLP->IsWindowVisible() )
			{
				CRect rcTLP;

				pWndTLP->GetWindowRect(&rcTLP);
				if(!rcTLP.IsRectEmpty())
				{
					CRect rcIntersection;

					if(rcIntersection.IntersectRect( &rcTLP, &rcWndOld))
						pWndTLP->UpdateWindow();
				}
			}
		}
		_FreeWinObjects();

		m_bAnimFinished = FALSE;
		m_AnimationType = __AT_CONTENT_DISPLAY;

		CRect rcWnd = _CalcTrackRect();
		if( m_rgnWnd.GetSafeHandle() != NULL )
		{
			CRgn rgnTmp;

			rgnTmp.CreateRectRgn( 0, 0, 0, 0);
			rgnTmp.CopyRgn(&m_rgnWnd);
			SetWindowRgn( (HRGN) rgnTmp.Detach(), FALSE);
		}

		m_bAnimFinished = TRUE;
		m_nPlaySoundOnAnimationFinished = CTSoundPlayer::__ON_MENU_EXPAND_CLICKED;
		m_AnimationType = m_bMenuExpandAnimation ? __AT_CONTENT_EXPAND : __AT_CONTENT_DISPLAY;

		_StartAnimation();
		if( m_AnimationType == __AT_NONE )
		{
			m_AnimationType = __AT_CONTENT_DISPLAY;
			_StartAnimation();
		}

		if(m_bScrollingAvailable)
			_RecalcLayoutImpl();

		SetWindowPos(
			NULL,
			rcWnd.left,
			rcWnd.top,
			rcWnd.Width(),
			rcWnd.Height(),
			SWP_NOACTIVATE|
			SWP_SHOWWINDOW|
			SWP_NOZORDER);
	}
	else
		UpdateWindow();

	::SendMessage(
		m_hWndCmdReciever,
		m_nMsgNotifyMenuExpanded,
		0, LPARAM(this));
}

BOOL CTPopupMenuWnd::IsAllItemsRarelyUsed() const
{
	for( int it = 0; it < m_items_all.GetSize(); it++)
	{
		const MENUITEMDATA &mi = m_items_all[it];

		if(mi.IsDisplayed())
			return FALSE;
	}

	return TRUE;
}

void CTPopupMenuWnd::_SyncItems()
{
	m_bExpandAvailable = FALSE;
	m_szFullItems.cx = 1;
	m_szFullItems.cy = 1;

	int nItemIndex = 0;
	int itLast = m_items_all.GetSize();
	int itLastVisSep = itLast;
	BOOL bAtLeastOneDisplayed = FALSE;

	for( int it = 0; it < m_items_all.GetSize(); it++)
	{
		MENUITEMDATA &mi = m_items_all[it];
		mi.SetIndex(nItemIndex++);

		if(m_bExpandWasPressed)
			mi.SetDisplayed(TRUE);
		else if(mi.IsPopup())
		{
			mi.GetPopup()->_SyncItems();

			if(mi.IsAllItemsRarelyUsed())
			{
				itLast = m_items_all.GetSize();
				mi.SetDisplayed(FALSE);
			}
			else
			{
				itLast = it;
				itLastVisSep = m_items_all.GetSize();
				mi.SetDisplayed(TRUE);
			}
		}
		else if(mi.IsSeparator())
		{
			if( itLast != m_items_all.GetSize() || (
				itLast == m_items_all.GetSize() &&
				itLastVisSep == m_items_all.GetSize() &&
				bAtLeastOneDisplayed))
			{
				itLastVisSep = it;
				mi.SetDisplayed(TRUE);
			}
			else
			{
				itLast = m_items_all.GetSize();
				mi.SetDisplayed(FALSE);
			}
		}
		else
		{
			CTCmdManager::cmd *p_cmd = mi.GetCmd();

			if(p_cmd->StateIsRarelyUsed())
			{
				itLast = m_items_all.GetSize();
				mi.SetDisplayed(FALSE);
			}
			else
			{
				itLast = it;
				itLastVisSep = m_items_all.GetSize();
				mi.SetDisplayed(TRUE);
			}
		}

		if(mi.IsDisplayed())
		{
			bAtLeastOneDisplayed = TRUE;
			m_szFullItems.cy += mi.GetMeasuredHeight();

			if( m_szFullItems.cx < mi.GetMeasuredWidth() )
				m_szFullItems.cx = mi.GetMeasuredWidth();
		}
		else
			m_bExpandAvailable = TRUE;
	}

	if( itLastVisSep != m_items_all.GetSize() && (
		itLastVisSep >= itLast ||
		itLast == m_items_all.GetSize()))
	{
		m_items_all[itLastVisSep].SetDisplayed(FALSE);
		m_szFullItems.cy -= m_items_all[itLastVisSep].GetMeasuredHeight();
	}

	int nMixDx = 80;
	if( m_szFullItems.cx < nMixDx )
		m_szFullItems.cx = nMixDx;
}

BOOL CTPopupMenuWnd::ItemRemove( int nPos)
{
	if( nPos < 0 )
	{
		if(!_BuildItems( NULL, FALSE))
			return FALSE;

		_SyncItems();
		return TRUE;
	}

	int nCountOfItems = ItemGetCount();
	if( nPos >= nCountOfItems )
		return FALSE;

	MENUITEMDATA &mi = _GetItemRef(nPos);
	if(mi.IsPopup())
	{
		mi.GetPopup()->_OnCancelMode();
		mi.GetPopup()->_BuildItems( NULL, FALSE);
		mi.DestroyPopup();
	}

	m_items_all.RemoveAt(nPos);
	_SyncItems();

	return TRUE;
}

BOOL CTPopupMenuWnd::ItemInsertSpecPopup( CTPopupMenuWnd *pSpecPopup,
										  int nPos,
										  LPCTSTR strText,
										  HICON hIcon)
{
	int nCountCountOfItems = ItemGetCount();

	if( nPos < 0 )
		nPos = nCountCountOfItems;

	if( nPos > nCountCountOfItems )
		return FALSE;

	MENUITEMDATA mi;
	mi.SetCmdReciever(m_hWndCmdReciever);

	pSpecPopup->m_bTopLevel = FALSE;
	mi.SetPopup( nPos, strText, hIcon, pSpecPopup);
	mi.GetPopup()->m_pWndParentMenu = this;
	mi.SetDisplayed(TRUE);

	_InsertItem( nPos, mi);
	if(m_bTopLevel)
		_SyncItems();
	else
		m_pWndParentMenu->_SyncItems();

	return TRUE;
}

BOOL CTPopupMenuWnd::ItemInsert( UINT nCmdID,
								 int nPos,
								 LPCTSTR strText,
								 HICON hIcon,
								 HWND hWndSpecCmdReciever)
{
	int nCountCountOfItems = ItemGetCount();

	if( nPos < 0 )
		nPos = nCountCountOfItems;

	if( nPos > nCountCountOfItems )
		return FALSE;

	MENUITEMDATA mi;
	mi.SetCmdReciever(hWndSpecCmdReciever != NULL ? hWndSpecCmdReciever : m_hWndCmdReciever);

	switch(nCmdID)
	{
	case TYPE_POPUP			: mi.SetPopup( nPos, strText, hIcon); break;
	case TYPE_SEPARATOR		: mi.SetSeparator(nPos); break;
	default					:
		{
			CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
				g_CmdManager->ProfileNameFromWnd(
				hWndSpecCmdReciever != NULL ? hWndSpecCmdReciever : m_hWndCmdReciever),
				nCmdID);

			if( p_cmd == NULL )
				return FALSE;

			mi.UpdateCmdManagerCommand( p_cmd, nPos);
		}

		break;
	}

	if(mi.IsPopup())
		mi.GetPopup()->m_pWndParentMenu = this;

	_InsertItem( nPos, mi);
	if(m_bTopLevel)
		_SyncItems();
	else
		m_pWndParentMenu->_SyncItems();

	return TRUE;
}

void CTPopupMenuWnd::SetLeftAreaWidth( UINT nWidth)
{
	m_nLeftAreaWidth = nWidth;
}

UINT CTPopupMenuWnd::GetLeftAreaWidth() const
{
	return m_nLeftAreaWidth;
}

BOOL CTPopupMenuWnd::CreatePopupMenu( HWND hWndCmdRecv)
{
	if( m_bTopLevel ||
		m_pWndParentMenu != NULL ||
		ItemGetCount() != 0 )
		return FALSE;

	m_szFullItems.cx = 1;
	m_szFullItems.cy = 1;
	m_bTopLevel = TRUE;
	m_pWndParentMenu = NULL;
	m_hWndCmdReciever = hWndCmdRecv;

	return TRUE;
}

void CTPopupMenuWnd::SetCmdTargetToAllItems( HWND hWndSpecCmdReciever, BOOL bOnlyThisLevel)
{
	for( int it = 0; it < m_items_all.GetSize(); it++)
	{
		MENUITEMDATA &mi = m_items_all[it];

		if(mi.IsSeparator())
			continue;

		if(mi.IsPopup())
		{
			if(!bOnlyThisLevel)
				mi.GetPopup()->SetCmdTargetToAllItems( hWndSpecCmdReciever, TRUE);
		}
		else
			mi.SetCmdReciever(hWndSpecCmdReciever);
	}
}

CTPopupMenuWnd::MENUITEMDATA &CTPopupMenuWnd::_GetItemRef( int nIndex)
{
	return m_items_all[nIndex];
}

const CTPopupMenuWnd::MENUITEMDATA &CTPopupMenuWnd::_GetItemRef(int nIndex) const
{
	return const_cast<CTPopupMenuWnd *> (this)->_GetItemRef(nIndex);
}

void CTPopupMenuWnd::_InsertItem( int nInsertBefore, MENUITEMDATA& mi)
{
	m_items_all.InsertAt( nInsertBefore < 0 ? m_items_all.GetSize() : nInsertBefore, mi);
}


/////////////////////////////////////////////////////////////////////////////
// CTPopupColorMenuWnd

IMPLEMENT_DYNCREATE(CTPopupColorMenuWnd, CTPopupMenuWnd)

BEGIN_MESSAGE_MAP(CTPopupColorMenuWnd, CTPopupMenuWnd)
	//{{AFX_MSG_MAP(CTPopupColorMenuWnd)
	//}}AFX_MSG_MAP
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
END_MESSAGE_MAP()


CTPopupColorMenuWnd::CTPopupColorMenuWnd()
{
	m_nColorIdxCurr = -1;
	m_nInitialColorIdx = -1;

	m_hWndNotifyColorChanged = NULL;
	m_clrInitial = (COLORREF) -1;
	m_clrDefault = RGB( 0, 0, 0);
	m_bEnableBtnColorDefault = TRUE;
	m_bEnableBtnColorCustom = TRUE;

	m_rcDefColorText.SetRectEmpty();
	m_rcCustColorText.SetRectEmpty();
	m_lParamCookie = 0;

	m_strBtnTextColorDefault = _T("Default Color");
	m_strBtnTextColorCustom = _T("Custom Color...");
}

BOOL CTPopupColorMenuWnd::OnQueryNewPalette()
{
	Invalidate();
	return CWnd::OnQueryNewPalette();
}

void CTPopupColorMenuWnd::OnPaletteChanged( CWnd* pFocusWnd)
{
	CWnd::OnPaletteChanged(pFocusWnd);

	if( pFocusWnd->GetSafeHwnd() != GetSafeHwnd() )
		Invalidate();
}

CSize CTPopupColorMenuWnd::_CalcTrackSize()
{
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nMenuShadowSize = _GetMenuShadowSize();

	CSize _size(
		(__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE * 2) * __NCLR_DX + nMenuBorderSize * 2 + nMenuShadowSize,
		(__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE * 2) * __NCLR_DY + nMenuBorderSize * 2 + nMenuShadowSize);

	if(m_bEnableBtnColorDefault)
	{
		_size.cy += __DEF_MENU_SEPARATOR_HEIGHT + __DEF_MENU_GAP * 2;
		CString strMeasureText(m_strBtnTextColorDefault);

		strMeasureText.Replace( _T("&"), _T(""));
		CWindowDC dc(NULL);
		CFont *pOldFont = dc.SelectObject(&g_PaintManager->m_FontNormal);
		CRect rect( 0, 0, 0, 0);
		CSize szDefColorText;

		szDefColorText.cy = DrawText(
			dc.GetSafeHdc(),
			(LPCTSTR) strMeasureText,
			strMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|
			DT_SINGLELINE|
			DT_LEFT|
			DT_VCENTER);

		szDefColorText.cx = rect.Width();
		dc.SelectObject(pOldFont);
		szDefColorText.cx += __DEF_MENU_GAP * 2;

		if( szDefColorText.cy < __DEF_MENU_HEIGHT )
			szDefColorText.cy = __DEF_MENU_HEIGHT;
		_size.cy += szDefColorText.cy;

		if( _size.cx < szDefColorText.cx )
			_size.cx = szDefColorText.cx;

		CPoint point(
			nMenuBorderSize + __DEF_MENU_GAP,
			nMenuBorderSize + __DEF_MENU_GAP);

		m_rcDefColorText.SetRect(
			point,
			point + CSize(
			max( _size.cx, szDefColorText.cx) - __DEF_MENU_GAP * 2 - nMenuBorderSize * 2 - nMenuShadowSize,
			szDefColorText.cy));
	}

	if(m_bEnableBtnColorCustom)
	{
		_size.cy += __DEF_MENU_SEPARATOR_HEIGHT + __DEF_MENU_GAP * 2;
		CString strMeasureText(m_strBtnTextColorCustom);

		strMeasureText.Replace( _T("&"), _T(""));
		CWindowDC dc(NULL);
		CFont *pOldFont = dc.SelectObject(&g_PaintManager->m_FontNormal);
		CRect rect( 0, 0, 0, 0);
		CSize szCustColorText;

		szCustColorText.cy = DrawText(
			dc.GetSafeHdc(),
			(LPCTSTR) strMeasureText,
			strMeasureText.GetLength(),
			&rect,
			DT_CALCRECT|
			DT_SINGLELINE|
			DT_LEFT|
			DT_VCENTER);

		szCustColorText.cx = rect.Width();
		dc.SelectObject(pOldFont);
		szCustColorText.cx += __DEF_MENU_GAP * 2;

		if( szCustColorText.cy < __DEF_MENU_HEIGHT )
			szCustColorText.cy = __DEF_MENU_HEIGHT;
		_size.cy += szCustColorText.cy;

		if( _size.cx < szCustColorText.cx )
			_size.cx = szCustColorText.cx;

		CPoint point(
			nMenuBorderSize + __DEF_MENU_GAP,
			nMenuBorderSize + __DEF_MENU_GAP);

		m_rcCustColorText.SetRect(
			point,
			point + CSize(
			max( _size.cx, szCustColorText.cx) - __DEF_MENU_GAP * 2 - nMenuBorderSize * 2 - nMenuShadowSize,
			szCustColorText.cy));

		if(m_bEnableBtnColorDefault)
			m_rcCustColorText.OffsetRect( 0, m_rcCustColorText.Height() + __DEF_MENU_GAP * 2 + __DEF_MENU_SEPARATOR_HEIGHT);

		m_rcCustColorText.OffsetRect( 0, nMenuBorderSize + (__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE * 2) * __NCLR_DY + __NCLR_BOX_DY_SPACE);
	}
	_size.cx += m_nLeftAreaWidth;

	return _size;
}

CPoint CTPopupColorMenuWnd::_GetColorItemCoord( int nIdx)
{
	return CPoint( nIdx % __NCLR_DX, nIdx / __NCLR_DX);
}

CRect CTPopupColorMenuWnd::_GetColorItemRect( int nIdx)
{
	CPoint ptCoord = _GetColorItemCoord(nIdx);
	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	CRect rcItem(
		CPoint(
		nMenuBorderSize + (__NCLR_BOX_DX_SIZE + __NCLR_BOX_DX_SPACE * 2) * ptCoord.x + __NCLR_BOX_DX_SPACE,
		nMenuBorderSize + (__NCLR_BOX_DY_SIZE + __NCLR_BOX_DY_SPACE * 2) * ptCoord.y + __NCLR_BOX_DY_SPACE),
		CSize( __NCLR_BOX_DX_SIZE, __NCLR_BOX_DY_SIZE));

	if(m_bEnableBtnColorDefault)
		rcItem.OffsetRect( 0, m_rcDefColorText.Height() + __DEF_MENU_SEPARATOR_HEIGHT + __DEF_MENU_GAP * 2);

	CRect rcClient;
	_GetClientRect(&rcClient);
	rcItem.OffsetRect(rcClient.TopLeft());
	rcItem.OffsetRect( m_nLeftAreaWidth, 0);

	return rcItem;
}

CRect CTPopupColorMenuWnd::_CalcTrackRect()
{
	return CTPopupMenuWnd::_CalcTrackRect();
}

BOOL CTPopupColorMenuWnd::_CreateHelper( CWnd *pWndCmdReciever)
{
	if(!CTPopupMenuWnd::_CreateHelper(pWndCmdReciever))
		return FALSE;

	int nIdx = _FindCellByColorRef(m_clrInitial);
	if( nIdx >= 0 )
	{
		m_nInitialColorIdx = nIdx;
		Invalidate();
	}

	CRect rcClient;
	_GetClientRect(&rcClient);

	if(m_bEnableBtnColorDefault)
	{
		m_rcDefColorText.OffsetRect(rcClient.TopLeft());
		m_rcDefColorText.OffsetRect( m_nLeftAreaWidth, 0);
	}

	if(m_bEnableBtnColorCustom)
	{
		m_rcCustColorText.OffsetRect(rcClient.TopLeft());
		m_rcCustColorText.OffsetRect( m_nLeftAreaWidth, 0);
	}

	return TRUE;
}

void CTPopupColorMenuWnd::_DoPaint( CDC &dcPaint, BOOL bUseBackBuffer)
{
	CRect rcRealClient;
	GetClientRect(&rcRealClient);

	CRect rcClient;
	_GetClientRect(&rcClient);

	CPalette *pOldPalette = NULL;
	if(dcPaint.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)
	{
		pOldPalette = dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		dcPaint.RealizePalette();
	}

	CTMemoryDC mdc;
	if(bUseBackBuffer)
	{
		mdc.__InitMemoryDC(
			&dcPaint,
			&rcClient,
			CTMemoryDC::MDCOPT_TO_MEMORY|
			CTMemoryDC::MDCOPT_FILL_SURFACE|
			CTMemoryDC::MDCOPT_FORCE_DIB);
	}

	CDC &dcDummyRef = mdc;
	CDC &dc = bUseBackBuffer ? dcDummyRef : dcPaint;
	CFont *pOldFont = (CFont *) dc.SelectObject(&g_PaintManager->m_FontNormal);

	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetMenuFrameFillColor());

	g_PaintManager->PaintMenuBorder(
		dc,
		&rcClient);

	int nMenuBorderSize = g_PaintManager->GetMenuBorderSize();
	int nMenuShadowSize = _GetMenuShadowSize();

	if( rcClient.top >= rcRealClient.top )
	{
		CRect rcExcludePart(rcRealClient);

		rcExcludePart.bottom = rcClient.top + nMenuBorderSize;
		dc.ExcludeClipRect(rcExcludePart);
	}

	if( rcClient.bottom <= rcRealClient.bottom )
	{
		CRect rcExcludePart(rcRealClient);

		rcExcludePart.top = rcClient.bottom - nMenuBorderSize;
		dc.ExcludeClipRect(rcExcludePart);
	}

	if(m_bEnableBtnColorDefault)
	{
		BOOL bSelected = m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN;

		g_PaintManager->PaintPushButton(
			dc, TRUE,
			m_rcDefColorText,
			(LPCTSTR) m_strBtnTextColorDefault,
			NULL,
			TRUE,
			bSelected,
			FALSE,
			FALSE,
			TRUE,
			bSelected,
			FALSE,
			FALSE,
			CTPaintManager::__ALIGN_HORIZ_CENTER|
			CTPaintManager::__ALIGN_VERT,
			NULL,
			FALSE,
			0, !bSelected);

		CRect rcItem(m_rcDefColorText);
		rcItem.OffsetRect( 0, rcItem.Height() + __DEF_MENU_GAP);
		rcItem.bottom = rcItem.top + __DEF_MENU_SEPARATOR_HEIGHT;
		rcItem.top += rcItem.Height() / 2;
		rcItem.bottom = rcItem.top + 1;

		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			FALSE);
	}

	if(m_bEnableBtnColorCustom)
	{
		BOOL bSelected = m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN;

		g_PaintManager->PaintPushButton(
			dc, TRUE,
			m_rcCustColorText,
			(LPCTSTR) m_strBtnTextColorCustom,
			NULL,
			TRUE,
			bSelected,
			FALSE,
			FALSE,
			TRUE,
			bSelected,
			FALSE,
			FALSE,
			CTPaintManager::__ALIGN_HORIZ_CENTER|
			CTPaintManager::__ALIGN_VERT,
			NULL,
			FALSE,
			0, !bSelected);

		CRect rcItem(m_rcCustColorText);
		rcItem.OffsetRect( 0, -__DEF_MENU_GAP);
		rcItem.bottom = rcItem.top;
		rcItem.top -= __DEF_MENU_SEPARATOR_HEIGHT;
		rcItem.top += rcItem.Height() / 2;
		rcItem.bottom = rcItem.top + 1;

		g_PaintManager->PaintSeparator(
			dc,
			rcItem,
			FALSE);
	}

	COLORREF clrColorBorder = g_PaintManager->GetColor(COLOR_3DDKSHADOW);
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++)
	{
		BOOL bSelected = m_nColorIdxCurr == nIdx || m_nInitialColorIdx == nIdx;
		CRect rcItem = _GetColorItemRect(nIdx);

		g_PaintManager->PaintPushButton(
			dc, TRUE,
			rcItem,
			_T(""),
			NULL,
			TRUE,
			bSelected,
			bSelected,
			FALSE,
			TRUE,
			bSelected,
			FALSE,
			FALSE,
			CTPaintManager::__ALIGN_HORIZ_CENTER|
			CTPaintManager::__ALIGN_VERT,
			NULL,
			FALSE,
			0, !bSelected);

		rcItem.DeflateRect(
			__NCLR_BOX_DX_SPACE,
			__NCLR_BOX_DY_SPACE);

		dc.FillSolidRect(
			&rcItem,
			m_colors[nIdx].m_clr);

		dc.Draw3dRect(
			&rcItem,
			clrColorBorder,
			clrColorBorder);
	}

	if( m_nLeftAreaWidth > 0 )
	{
		CRect rcLeftArea(rcClient);

		rcLeftArea.right = rcLeftArea.left + m_nLeftAreaWidth;
		rcLeftArea.OffsetRect( nMenuBorderSize, 0);
		rcLeftArea.DeflateRect( 0, nMenuBorderSize);

		DRAWLEFTAREADATA _DrawLeftAreaData( &dc, &rcLeftArea, this);
		_DrawLeftAreaData.DoOwnerDrawPainting();
	}

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	dc.SelectClipRgn(NULL);

	if(m_bCombineWithEA)
	{
		dc.SelectClipRgn(NULL);
		CRect rcExcludeClient(m_rcExcludeArea);
		ScreenToClient(&rcExcludeClient);

		g_PaintManager->PaintMenuCombinedArea(
			dc,
			rcExcludeClient,
			rcClient,
			m_nCombineAlign);

		if( m_pCbPaintCombinedContent != NULL )
		{
			m_pCbPaintCombinedContent(
				m_pCbPaintCombinedCookie,
				dc, *this,
				m_rcExcludeArea,
				m_nCombineAlign);
		}
	}
	dc.SelectObject(pOldFont);

	if(bUseBackBuffer)
		mdc.__Flush();

	if( CTPopupMenuWnd::m_bMenuWithShadows &&
		m_bAnimFinished &&
		nMenuShadowSize > 0 )
	{
		dcPaint.SelectClipRgn(NULL);

		CRect rcExcludeClient( 0, 0, 0, 0);
		CRect rcExcludeScreen( 0, 0, 0, 0);
		CRect rcBaseScreen( 0, 0, 0, 0);

		if(m_bCombineWithEA)
		{
			if( CTPopupMenuWnd::m_bMenuWithShadows &&
				m_bAnimFinished &&
				nMenuShadowSize > 0 )
			{
				rcExcludeScreen = m_rcExcludeArea;
				rcExcludeClient = rcExcludeScreen;
				ScreenToClient(&rcExcludeClient);

				dcPaint.ExcludeClipRect(&rcClient);
				rcBaseScreen = rcClient;
				ClientToScreen(&rcBaseScreen);

				m_ShadowCMBA.Paint(
					dcPaint,
					rcExcludeClient,
					rcExcludeScreen,
					rcBaseScreen,
					nMenuShadowSize);

				dcPaint.SelectClipRgn(NULL);
			}

			CRect rcExcludePart(m_rcExcludeArea);
			ScreenToClient(&rcExcludePart);
			dcPaint.ExcludeClipRect(&rcExcludePart);
		}

		m_ShadowMain.Paint(
			dcPaint,
			rcClient,
			rcBaseScreen,
			rcExcludeScreen,
			nMenuShadowSize);
	}

	if( pOldPalette != NULL )
		dcPaint.SelectPalette( pOldPalette, FALSE);
}

int CTPopupColorMenuWnd::_ColorItemHitTest( const CPoint& point)
{
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++)
	{
		CRect rcItem = _GetColorItemRect(nIdx);

		if(rcItem.PtInRect(point))
			return nIdx;
	}

	if(m_rcDefColorText.PtInRect(point))
		return IDX_DEFAULT_COLOR_BTN;

	if(m_rcCustColorText.PtInRect(point))
		return IDX_CUSTOM_COLOR_BTN;

	return -1;
}

BOOL CTPopupColorMenuWnd::_OnMouseMove( UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if( GetSite().GetAnimated() != NULL )
		return TRUE;

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	if(!rcWindow.PtInRect(point))
	{
		if( m_pWndParentMenu != NULL && m_pWndParentMenu->GetSafeHwnd() != NULL )
		{
			ClientToScreen(&point);
			m_pWndParentMenu->ScreenToClient(&point);

			if(m_pWndParentMenu->_OnMouseMove( nFlags, point))
			{
				_OnCancelMode();
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL bHoverChanged = FALSE;
	int nColorIdxCurr = _ColorItemHitTest(point);

	if( m_nColorIdxCurr != nColorIdxCurr )
	{
		m_nColorIdxCurr = nColorIdxCurr;
		bHoverChanged = TRUE;
		Invalidate();

		if( m_nColorIdxCurr != IDX_CUSTOM_COLOR_BTN )
			_NotifyColorChanged();
	}

	if( m_nColorIdxCurr >= 0 )
	{
		if( m_bMenuShowCoolTips && bHoverChanged )
		{
			CString strTipText = m_colors[m_nColorIdxCurr].m_szName;

			if( !strTipText.IsEmpty() &&
				GetSite().GetCapture() == this &&
				GetSite().GetAnimated() == NULL )
			{
				CRect rcItem = _GetColorItemRect(m_nColorIdxCurr);

				ClientToScreen(&rcItem);
				m_wndToolTip.SetText((LPCTSTR) strTipText);
				m_wndToolTip.Show( this, rcItem);
			}
		}
	}
	else
		_CoolTipHide();

	return TRUE;
}

BOOL CTPopupColorMenuWnd::_OnMouseClick( UINT nFlags, CPoint point)
{
	if( GetSafeHwnd() == NULL )
		return FALSE;

	if( GetSite().GetAnimated() != NULL )
		return TRUE;

	int m_nColorIdxCurr = _ColorItemHitTest(point);
	if( m_nColorIdxCurr >= 0 ||
		m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN ||
		m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN )
	{
		_NotifyColorChanged(TRUE);
		g_SoundPlayer->PlaySound(CTSoundPlayer::__ON_MENU_CMD_CLICKED);
	}

	_EndSequence();
	return TRUE;
}

BOOL CTPopupColorMenuWnd::_OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( GetSafeHwnd() == NULL )
		return TRUE;

	if( GetSite().GetAnimated() != NULL )
		return TRUE;

	BOOL bEat = FALSE;
	switch(nChar)
	{
	case VK_RETURN		:
		{
			if( m_nColorIdxCurr >= 0 ||
				m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN ||
				m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN )
			{
				_NotifyColorChanged(TRUE);
				g_SoundPlayer->PlaySound(CTSoundPlayer::__ON_MENU_CMD_CLICKED);
			}

			_EndSequence();
		}

		return TRUE;

	case VK_ESCAPE		: _EndSequence(); return TRUE;
	case VK_RIGHT		:
		{
			bEat = TRUE;

			if( m_nColorIdxCurr < 0 )
				m_nColorIdxCurr = 0;
			else
			{
				CPoint ptCoord = _GetColorItemCoord(m_nColorIdxCurr);
				ptCoord.x++;

				if( ptCoord.x >= __NCLR_DX )
					ptCoord.x = 0;

				m_nColorIdxCurr = ptCoord.x + ptCoord.y * __NCLR_DX;
			}
		}

		break;

	case VK_LEFT		:
		{
			bEat = TRUE;

			if( m_nColorIdxCurr < 0 )
				m_nColorIdxCurr = 0;
			else
			{
				CPoint ptCoord = _GetColorItemCoord(m_nColorIdxCurr);
				ptCoord.x--;

				if( ptCoord.x < 0 )
					ptCoord.x = __NCLR_DX - 1;

				m_nColorIdxCurr = ptCoord.x + ptCoord.y * __NCLR_DX;
			}
		}

		break;

	case VK_DOWN		:
		{
			bEat = TRUE;

			if( m_nColorIdxCurr < 0 )
				m_nColorIdxCurr = 0;
			else
			{
				CPoint ptCoord = _GetColorItemCoord(m_nColorIdxCurr);
				ptCoord.y++;

				if( ptCoord.y >= __NCLR_DY )
					ptCoord.y = 0;

				m_nColorIdxCurr = ptCoord.x + ptCoord.y * __NCLR_DX;
			}
		}

		break;

	case VK_UP			:
		{
			bEat = TRUE;

			if( m_nColorIdxCurr < 0 )
				m_nColorIdxCurr = 0;
			else
			{
				CPoint ptCoord = _GetColorItemCoord(m_nColorIdxCurr);
				ptCoord.y--;

				if( ptCoord.y < 0 )
					ptCoord.y = __NCLR_DY - 1;

				m_nColorIdxCurr = ptCoord.x + ptCoord.y * __NCLR_DX;
			}
		}

		break;
	}

	if(bEat)
	{
		_CoolTipHide();
		Invalidate();
		_NotifyColorChanged();
	}

	return bEat;
}

HWND CTPopupColorMenuWnd::_GetWndNotifyColorChanged()
{
	if( m_hWndNotifyColorChanged != NULL )
		return m_hWndNotifyColorChanged;

	return m_hWndCmdReciever;
}

void CTPopupColorMenuWnd::_NotifyColorChanged( BOOL bFinal)
{
	HWND hWndNotify = _GetWndNotifyColorChanged();

	if( m_nColorIdxCurr == IDX_CUSTOM_COLOR_BTN )
	{
		::PostMessage(
			hWndNotify,
			m_nMsgNotifyCustColor,
			0, m_lParamCookie);

		return;
	}

	COLORREF clr = RGB( 0, 0, 0);
	if( m_nColorIdxCurr == IDX_DEFAULT_COLOR_BTN )
		clr = m_clrDefault;
	else
	{
		if( m_nColorIdxCurr < 0 )
			return;

		clr = m_colors[m_nColorIdxCurr].m_clr;
	}

	::PostMessage(
		hWndNotify,
		bFinal ? m_nMsgNotifyColorChangedFinally : m_nMsgNotifyColorChanged,
		(WPARAM) clr,
		m_lParamCookie);
}

int CTPopupColorMenuWnd::_FindCellByColorRef( COLORREF clr)
{
	for( int nIdx = 0; nIdx < __NCLR_COUNT; nIdx++)
	{
		if( m_colors[nIdx].m_clr == clr )
			return nIdx;
	}

	return -1;
}
