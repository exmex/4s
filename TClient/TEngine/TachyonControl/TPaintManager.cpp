#include "StdAfx.h"
#include "TPaintManager.h"
#include "TPopupMenuWnd.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC( CTPaintManagerXP, CTPaintManager)
IMPLEMENT_DYNAMIC( CTPaintManager, CObject)


#define __3D_COLOR_DEPTH_R		7
#define __3D_COLOR_DEPTH_G		6
#define __3D_COLOR_DEPTH_B		6

#define CXTEXTMARGIN			1


CTPaintManager::CTPaintManagerAutoPtr g_PaintManager;
CTPaintManager::CTPaintManagerAutoPtr::CTPaintManagerAutoPtr() : m_pPaintMenager(NULL),
	m_strOsVer(_T("Unknown")),
	m_bIsWin32s(FALSE),
	m_bIsWin9x(FALSE),
	m_bIsWin98(FALSE),
	m_bIsWin98orLater(FALSE),
	m_bIsWinNT(FALSE),
	m_bIsWinNT4(FALSE),
	m_bIsWin2000(FALSE),
	m_bIsWinXP(FALSE)
{
	memset( (char *) &m_osVerData, 0, sizeof(OSVERSIONINFO));
	m_osVerData.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	CString strTmp(_T(""));
	::GetVersionEx(&m_osVerData);

	switch(m_osVerData.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s			:
		{
			m_bIsWin32s = TRUE;
			strTmp = _T("Win32s");
		}

		break;

	case VER_PLATFORM_WIN32_WINDOWS		:
		{
			m_bIsWin9x = TRUE;
			m_bIsWin98orLater = m_osVerData.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && (
				m_osVerData.dwMajorVersion > 4 || (
				m_osVerData.dwMajorVersion == 4 && m_osVerData.dwMinorVersion > 0 ));

			strTmp = _T("Windows ");
			if(m_bIsWin98orLater)
			{
				if( m_osVerData.dwMajorVersion == 4 && m_osVerData.dwMinorVersion > 0 )
				{
					m_bIsWin98 = TRUE;
					strTmp += _T("98");
				}
				else
					strTmp += _T("98 or later");
			}
			else
				strTmp += _T("95");
		}

		break;

	case VER_PLATFORM_WIN32_NT			:
		{
			m_bIsWinNT = TRUE;

			if( m_osVerData.dwMajorVersion == 6 )
			{
				m_bIsWinXP = TRUE;
				strTmp = _T("Windows XP");
			}
			else if( m_osVerData.dwMajorVersion == 5 )
			{
				m_bIsWin2000 = TRUE;
				strTmp = _T("Windows 2000");
			}
			else if( m_osVerData.dwMajorVersion == 4 )
			{
				m_bIsWinNT4 = TRUE;
				strTmp = _T("Windows NT");
			}
			else
				strTmp = _T("Windows NT");
		}

		break;
	}

	m_strOsVer.Format(
		_T("%s v.%lu.%lu (build %lu) %s"),
		(LPCTSTR) strTmp,
		m_osVerData.dwMajorVersion,
		m_osVerData.dwMinorVersion,
		m_osVerData.dwBuildNumber,
		m_osVerData.szCSDVersion);

	InitPaintManagerInstance();
}

CTPaintManager::CTPaintManagerAutoPtr::~CTPaintManagerAutoPtr()
{
	if(m_pPaintMenager)
		delete m_pPaintMenager;

	m_pPaintMenager = NULL;
}

BOOL CTPaintManager::CTPaintManagerAutoPtr::InitPaintManagerInstance()
{
	if( m_pPaintMenager != NULL )
		return TRUE;

	return InstallPaintManager((CTPaintManager *) NULL);
}

CTPaintManager *CTPaintManager::CTPaintManagerAutoPtr::operator->()
{
	return m_pPaintMenager;
}

BOOL CTPaintManager::CTPaintManagerAutoPtr::InstallPaintManager( CTPaintManager *pPaintMenager)
{
	if(m_pPaintMenager)
	{
		if( pPaintMenager == m_pPaintMenager )
			return TRUE;

		if(m_pPaintMenager)
		{
			delete m_pPaintMenager;
			m_pPaintMenager = NULL;
		}
	}

	if(pPaintMenager)
		m_pPaintMenager = pPaintMenager;
	else
	{
		m_pPaintMenager = new __DEFAULT_PAINT_MANAGER_CLASS;

		if(!m_pPaintMenager->IsKindOf(RUNTIME_CLASS(CTPaintManager)))
			return FALSE;
	}

	m_pPaintMenager->SyncSysColors();
	m_pPaintMenager->InitTranslatedColors();

	return TRUE;
}

BOOL CTPaintManager::CTPaintManagerAutoPtr::InstallPaintManager( CRuntimeClass *pRtcPaintMenager)
{
	if(!pRtcPaintMenager)
		return InitPaintManagerInstance();

	CObject *pObj = pRtcPaintMenager->CreateObject();
	if(!pObj)
		return FALSE;

	CTPaintManager *pPaintManager = DYNAMIC_DOWNCAST( CTPaintManager, pObj);
	if(!pPaintManager)
	{
		delete pObj;
		return FALSE;
	}

	return InstallPaintManager(pPaintManager);
}

CTPaintManager::CTPaintManager()
{
	m_DSS_DISABLED_style = DSS_DISABLED;

	struct
	{
		LOGPALETTE LogPalette;
		PALETTEENTRY PalEntry[__3D_COLOR_DEPTH_R * __3D_COLOR_DEPTH_G * __3D_COLOR_DEPTH_B];
	} pal;

	LOGPALETTE *pLogPalette = (LOGPALETTE *) &pal;

	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = (WORD) __3D_COLOR_DEPTH_R * __3D_COLOR_DEPTH_G * __3D_COLOR_DEPTH_B;

	int i = 0;
	for( int nX=0; nX<__3D_COLOR_DEPTH_R; nX++)
	{
		for( int nY=0; nY<__3D_COLOR_DEPTH_G; nY++)
		{
			for( int nZ=0; nZ<__3D_COLOR_DEPTH_B; nZ++)
			{
				pLogPalette->palPalEntry[i].peRed = BYTE((nX * 255) / (__3D_COLOR_DEPTH_R - 1));
				pLogPalette->palPalEntry[i].peGreen = BYTE((nY * 255) / (__3D_COLOR_DEPTH_G - 1));
				pLogPalette->palPalEntry[i].peBlue = BYTE((nZ * 255) / (__3D_COLOR_DEPTH_B - 1));
				pLogPalette->palPalEntry[i].peFlags = 0;
				i++;
			}
		}
	}

	m_PaletteWide.CreatePalette(pLogPalette);
}

CTPaintManager::~CTPaintManager()
{
	m_colors.RemoveAll();
	m_brushes.RemoveAll();
}

COLORREF CTPaintManager::RGBtoCMY( COLORREF clr)
{
	int nR = GetRValue(clr);
	int nG = GetGValue(clr);
	int nB = GetBValue(clr);
	int nC = 255 - nR;
	int nM = 255 - nG;
	int nY = 255 - nB;

	COLORREF clrCMY = RGB( nC, nM, nY);
	return clrCMY;
}

COLORREF CTPaintManager::CMYtoRGB( COLORREF clr)
{
	int nC = GetRValue(clr);
	int nM = GetGValue(clr);
	int nY = GetBValue(clr);
	int nR = 255 - nC;
	int nG = 255 - nM;
	int nB = 255 - nY;

	COLORREF clrRGB = RGB( nR, nG, nB);
	return clrRGB;
}

double CTPaintManager::HuetoRGB( double m1, double m2, double h)
{
	if( h < 0 )
		h += 1.0f;

	if( h > 1 )
		h -= 1.0f;

	if( 6.0f * h < 1.0f )
		return m1 + (m2 - m1) * h * 6.0f;

	if( 2.0f * h < 1.0f )
		return m2;

	if( 3.0f * h < 2.0f )
		return m1 + (m2 - m1) * ((2.0f / 3.0f) - h) * 6.0f;

	return m1;
}

COLORREF CTPaintManager::HLStoRGB( double H, double L, double S)
{
	double r, g, b;
	double m1, m2;

	if( S == 0 )
	{
		r = g = b = L;
	}
	else
	{
		if( L <= 0.5f )
			m2 = L * (1.0f + S);
		else
			m2 = L + S - L * S;

		m1 = 2.0f * L - m2;
		r = HuetoRGB( m1, m2, H + 1.0f / 3.0f);
		g = HuetoRGB( m1, m2, H);
		b = HuetoRGB( m1, m2, H - 1.0f / 3.0f);
	}

	return RGB( (BYTE) (r * 255), (BYTE) (g * 255), (BYTE) (b * 255));
}

void CTPaintManager::RGBtoHSL( COLORREF rgb, double *H, double *S, double *L)
{
	double delta;
	double r = (double) GetRValue(rgb) / 255;
	double g = (double) GetGValue(rgb) / 255;
	double b = (double) GetBValue(rgb) / 255;
	double cmax = max( r, max( g, b));
	double cmin = min( r, min( g, b));

	*L = (cmax + cmin) / 2.0f;
	if( cmax == cmin )
	{
		*S = 0;
		*H = 0;
	}
	else
	{
		if( *L < 0.5f )
			*S = (cmax - cmin) / (cmax + cmin);
		else
			*S = (cmax - cmin) / (2.0f - cmax - cmin);

		delta = cmax - cmin;
		if( r == cmax )
			*H = (g - b) / delta;
		else if( g == cmax )
			*H = 2.0f + (b - r) / delta;
		else
			*H = 4.0f + (r - g) / delta;

		*H /= 6.0f;
		if( *H < 0.0f )
			*H += 1.0f;
	}
}

BYTE CTPaintManager::GetRGBFromHue( float rm1, float rm2, float rh)
{
	if( rh > 360.0f )
		rh -= 360.0f;
	else if( rh < 0.0f )
		rh += 360.0f;

	if( rh < 60.0f )
		rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;
	else if( rh < 180.0f )
		rm1 = rm2;
	else if( rh < 240.0f )
		rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;

	return static_cast<BYTE>(rm1 * 255);
}

COLORREF CTPaintManager::GetRGBFromHLSExtend( double H, double L, double S)
{
	WORD R, G, B;

	if( S == 0.0f )
	{
		R = G = B = unsigned char(L * 255.0f);
	}
	else
	{
		FLOAT rm1, rm2;

		if( L <= 0.5f )
			rm2 = (FLOAT) (L + L * S);
		else
			rm2 = (FLOAT) (L + S - L * S);

		rm1 = (FLOAT) (2.0f * L - rm2);
		R = GetRGBFromHue( rm1, rm2, (float) (H + 120.0f));
		G = GetRGBFromHue( rm1, rm2, (float) (H));
		B = GetRGBFromHue( rm1, rm2, (float) (H - 120.0f));
	}

	return RGB( R, G, B);
}

COLORREF CTPaintManager::HLS_Adjust( COLORREF clr,
									 double percentH,
									 double percentL,
									 double percentS)
{
	double H = 0.0f;
	double L = 0.0f;
	double S = 0.0f;

	RGBtoHSL( clr, &H, &S, &L);
	if( percentH > 0.0f )
		H = H + (1.0f - H) * percentH;
	else if( percentH < 0.0 )
		H = H * (1.0f + percentH);

	if( H < 0.0f )
		H = 0.0f;
	else if( H > 1.0f )
		H = 1.0f;

	if( percentL > 0.0f )
		L = L + (1.0f - L) * percentL;
	else if( percentL < 0.0f )
		L = L * (1.0f + percentL);

	if( L < 0.0f )
		L = 0.0f;
	else if( L > 1.0f )
		L = 1.0;

	if( percentS > 0.0f )
		S = S + (1.0f - S) * percentS;
	else if( percentS < 0.0f )
		S = S * (1.0f + percentS);

	if( S < 0.0f )
		S = 0.0f;
	else if( S > 1.0f )
		S = 1.0f;

	return HLStoRGB( H, L, S);
}

BOOL CTPaintManager::PaintParentBk( HWND hWnd, HDC hDC, LPCRECT pClip)
{
	if(pClip)
	{
		CRgn rgn;

		rgn.CreateRectRgnIndirect(pClip);
		::SelectClipRgn( hDC, (HRGN) rgn.GetSafeHandle());
	}

	HWND hWndParent = ::GetParent(hWnd);
	CPoint point( 0, 0);

	::MapWindowPoints( hWnd, hWndParent, &point, 1);
	::OffsetWindowOrgEx(
		hDC,
		point.x,
		point.y,
		&point);

	LRESULT lRes = ::SendMessage(
		hWndParent,
		WM_ERASEBKGND,
		(WPARAM)hDC, 0);

	::SetWindowOrgEx(
		hDC,
		point.x,
		point.y,
		&point);

	::SelectClipRgn( hDC, NULL);
	return (BOOL) lRes;
}

BOOL CTPaintManager::DefIsHwndNeedsDirectRepaint( HWND hWndChild)
{
	DWORD dwChildStyle = DWORD(GetWindowLong( hWndChild, GWL_STYLE));

	if( (dwChildStyle & WS_CHILD) == 0 )
		return TRUE;

	CString strClassName;
	::GetClassName(
		hWndChild,
		strClassName.GetBuffer(_MAX_PATH + 1),
		_MAX_PATH);

	strClassName.ReleaseBuffer();
	strClassName.MakeLower();

	if( strClassName == _T("systabcontrol32") )
		return TRUE;

	if( strClassName == _T("button") && (dwChildStyle&(BS_GROUPBOX|BS_CHECKBOX)) )
		return TRUE;

	return FALSE;
}

BOOL CTPaintManager::DefExcludeChildAreaCallback( HDC hDC,
													   HWND hWnd,
													   HWND hWndChild,
													   LPVOID pCookie)
{
	return DefIsHwndNeedsDirectRepaint(hWndChild);
}

int CTPaintManager::GetBPP()
{
	CWindowDC desktop(NULL);

	int nBitsPerPixel = desktop.GetDeviceCaps(BITSPIXEL);
	return nBitsPerPixel;
}

CSize CTPaintManager::GetTextSizes( BOOL bBold)
{
	CWindowDC dc(NULL);
	CFont *pOldFont = dc.SelectObject(bBold ? &m_FontBold : &m_FontNormal);
	TEXTMETRIC tm;

	dc.GetTextMetrics(&tm);
	int nTextHeightHorz = tm.tmHeight + 2;

	dc.SelectObject(&m_FontNormal);
	dc.GetTextMetrics(&tm);

	int nTextHeightVert = tm.tmHeight + 2;
	dc.SelectObject(pOldFont);

	return CSize( nTextHeightHorz, nTextHeightVert);
}

int CTPaintManager::GetTextHeight( BOOL bHorz)
{
	CSize size = GetTextSizes();
	return bHorz ? size.cx : size.cy;
}

static const UINT pixels_wingl_btn_expand_bottom[] = {
	1, 1, 1, 1, 1,
	0, 1, 1, 1, 0,
	0, 0, 1, 0, 0};

const CTPaintManager::wingl CTPaintManager::m_wingl_btn_expand_bottom(
	5, 3, 2,
	pixels_wingl_btn_expand_bottom);

static const UINT pixels_wingl_btn_expand_right2[] = {
	1, 1, 0, 0, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 0, 0, 1, 1,
	0, 1, 1, 0, 0, 1, 1, 0,
	1, 1, 0, 0, 1, 1, 0, 0};

const CTPaintManager::wingl CTPaintManager::m_wingl_btn_expand_right2(
	8, 5, 2,
	pixels_wingl_btn_expand_right2);

const CTPaintManager::wingl CTPaintManager::m_wingl_btn_expand_left(
	CTPaintManager::m_wingl_btn_expand_bottom,
	90);

const CTPaintManager::wingl CTPaintManager::m_wingl_btn_expand_right(
	CTPaintManager::m_wingl_btn_expand_bottom,
	270);

const CTPaintManager::wingl CTPaintManager::m_wingl_btn_expand_bottom2(
	CTPaintManager::m_wingl_btn_expand_right2,
	90);

void CTPaintManager::PaintWingl( CDC &dc,
								 POINT ptDest,
								 const wingl& _wingl,
								 COLORREF *pColorValues)
{
	CRect rcTestVisiblity( ptDest, _wingl.Size());

	if(!dc.RectVisible(&rcTestVisiblity))
		return;

	int x_dest = ptDest.x + _wingl.Size().cx;
	int y_dest = ptDest.y + _wingl.Size().cy;
	const UINT *pColorIndex = _wingl.GetSurface();

	for( int y=ptDest.y; y<y_dest; y++)
	{
		for( int x=ptDest.x; x<x_dest; x++, pColorIndex++)
		{
			UINT nColorIndex = *pColorIndex;
			if( nColorIndex == 0 )
				continue;

			COLORREF clr = pColorValues[nColorIndex];
			dc.SetPixel( x, y, clr);
		}
	}
}

void CTPaintManager::PaintWinglCentered( CDC& dc,
										 const RECT& rect,
										 const wingl& _wingl,
										 COLORREF *pColorValues)
{
	CRect _rect(rect);
	_rect.NormalizeRect();

	if(_rect.IsRectEmpty())
		return;

	CPoint ptDest = _rect.TopLeft();
	ptDest.x += (_rect.Width() - _wingl.Size().cx) / 2;
	ptDest.y += (_rect.Height() - _wingl.Size().cy) / 2;

	PaintWingl(
		dc,
		ptDest,
		_wingl,
		pColorValues);
}

void CTPaintManager::PaintMenuExpandButton( CDC& dc,
										    const CRect& rectButton,
											BOOL bPushed,
											BOOL bHover)
{
	CRect rcWingl(rectButton);

	if(bPushed)
	{
		dc.FillRect(
			&rcWingl,
			&m_brushLight);

		dc.Draw3dRect(
			&rcWingl,
			GetColor(CLR_3DHILIGHT_OUT),
			GetColor(CLR_3DSHADOW_OUT));
	}
	else
	{
		PaintMenuItem(
			dc,
			rectButton,
			_T(""),
			_T(""),
			NULL,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			TRUE,
			FALSE,
			FALSE,
			FALSE, 0);
	}

	COLORREF ColorValues[] = {
		RGB( 0, 0, 0),
		GetColor(CLR_TEXT_OUT)};

	PaintWinglCentered(
		dc,
		rcWingl,
		m_wingl_btn_expand_bottom2,
		ColorValues);
}

void CTPaintManagerXP::PaintMenuExpandButton( CDC& dc,
											  const CRect& rectButton,
											  BOOL bPushed,
											  BOOL bHover)
{
	if(!bPushed)
	{
		CRect rc(rectButton);
		rc.top--;

		PaintMenuItem(
			dc,
			rc,
			_T(""),
			_T(""),
			NULL,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			TRUE,
			FALSE,
			FALSE,
			FALSE, 0);
	}

	CRect rcWingl(rectButton);
	if(bPushed)
	{
		PaintPushButton(
			dc,
			TRUE,
			rectButton,
			_T(""),
			NULL,
			TRUE,
			FALSE,
			bPushed,
			FALSE,
			FALSE,
			TRUE,
			FALSE,
			FALSE,
			__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
			NULL,
			FALSE);

		rcWingl.OffsetRect(GetPushedOffset());
	}
	else
	{
		PaintMenuItem(
			dc,
			rectButton,
			_T(""),
			_T(""),
			NULL,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			FALSE,
			TRUE,
			FALSE,
			FALSE,
			FALSE, 0);
	}

	COLORREF ColorValues[] = {
		RGB( 0, 0, 0),
		GetColor(CLR_TEXT_OUT)};

	PaintWinglCentered(
		dc,
		rcWingl,
		m_wingl_btn_expand_bottom2,
		ColorValues);
}

void CTPaintManager::PaintToolbarExpandButton( CDC& dc,
											   const CRect& rcButton,
											   BOOL bHorz,
											   BOOL bBarIsCompletelyVisible,
											   BOOL bEnabled,
											   BOOL bPushed,
											   BOOL bHover,
											   BOOL bTransparentBackground)
{
	CRect rect(rcButton);
	rect.NormalizeRect();

	if(!dc.RectVisible(&rect))
		return;

	const wingl *pWingl1 = NULL;
	const wingl *pWingl2 = NULL;

	if(bHorz)
	{
		pWingl1 = &m_wingl_btn_expand_bottom;

		if(!bBarIsCompletelyVisible)
			pWingl2 = &m_wingl_btn_expand_right2;
	}
	else
	{
		pWingl1 = &m_wingl_btn_expand_left;

		if(!bBarIsCompletelyVisible)
			pWingl2 = &m_wingl_btn_expand_bottom2;
	}

	PaintPushButton(
		dc,
		TRUE,
		rect,
		_T(""),
		NULL,
		TRUE,
		bHover,
		bPushed,
		FALSE,
		bEnabled,
		TRUE,
		FALSE,
		FALSE,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,
		FALSE, 0,
		bTransparentBackground);

	COLORREF clr = GetColor(bEnabled ? CLR_TEXT_OUT : COLOR_3DHILIGHT);
	COLORREF ColorValues[] = { 0, clr};

	if(bPushed)
		rect.OffsetRect(GetPushedOffset());

	CRect rcWingl1( rcButton.TopLeft(), pWingl1->Size());
	CRect rcWingl2(rcWingl1);
	CSize szPushedOffset = GetPushedOffset();

	if(bPushed)
	{
		rcWingl1.OffsetRect(szPushedOffset);
		rcWingl2.OffsetRect(szPushedOffset);
	}

	if(bHorz)
	{
		int nGap = max( szPushedOffset.cy, 1) * 3;

		rcWingl1.OffsetRect(
			(rcButton.Size().cx - pWingl1->Size().cx) / 2,
			rcButton.Size().cy - pWingl1->Size().cy - nGap);

		if(!bBarIsCompletelyVisible)
			rcWingl2.OffsetRect( (rcButton.Size().cx - pWingl2->Size().cx) / 2, nGap);
	}
	else
	{
		int nGap = max( szPushedOffset.cx, 1) * 3;

		rcWingl1.OffsetRect(
			nGap,
			(rcButton.Size().cy - pWingl1->Size().cy) / 2);

		if(!bBarIsCompletelyVisible)
		{
			rcWingl2.OffsetRect(
				rcButton.Size().cx - pWingl2->Size().cx - nGap,
				(rcButton.Size().cy - pWingl2->Size().cy) / 2);
		}
	}

	if(bEnabled)
	{
		PaintWingl(
			dc,
			rcWingl1.TopLeft(),
			*pWingl1,
			ColorValues);

		if(!bBarIsCompletelyVisible)
		{
			PaintWingl(
				dc,
				rcWingl2.TopLeft(),
				*pWingl2,
				ColorValues);
		}
	}
	else
	{
		rcWingl1.OffsetRect( 1, 1);
		PaintWingl(
			dc,
			rcWingl1.TopLeft(),
			*pWingl1,
			ColorValues);

		rcWingl1.OffsetRect( -1, -1);
		ColorValues[1] = GetColor(COLOR_3DSHADOW);

		PaintWingl(
			dc,
			rcWingl1.TopLeft(),
			*pWingl1,
			ColorValues);
	}
}

void CTPaintManagerXP::PaintToolbarExpandButton( CDC& dc,
												 const CRect& rcButton,
												 BOOL bHorz,
												 BOOL bBarIsCompletelyVisible,
												 BOOL bEnabled,
												 BOOL bPushed,
												 BOOL bHover,
												 BOOL bTransparentBackground)
{
	CRect rect(rcButton);
	rect.NormalizeRect();

	if(!dc.RectVisible(&rect))
		return;

	const wingl *pWingl1 = NULL;
	const wingl *pWingl2 = NULL;

	if(bHorz)
	{
		pWingl1 = &m_wingl_btn_expand_bottom;

		if(!bBarIsCompletelyVisible)
			pWingl2 = &m_wingl_btn_expand_right2;
	}
	else
	{
		pWingl1 = &m_wingl_btn_expand_left;

		if(!bBarIsCompletelyVisible)
			pWingl2 = &m_wingl_btn_expand_bottom2;
	}

	PaintPushButton(
		dc,
		TRUE,
		rect,
		_T(""),
		NULL,
		TRUE,
		bHover,
		bPushed,
		FALSE,
		bEnabled,
		TRUE,
		FALSE,
		FALSE,
		__ALIGN_HORIZ_CENTER|__ALIGN_VERT,
		NULL,
		FALSE, 0,
		bTransparentBackground);

	COLORREF clr = GetColor(bEnabled ? CLR_TEXT_OUT : COLOR_3DHILIGHT);
	COLORREF ColorValues[] = { 0, clr};

	if(bPushed)
		rect.OffsetRect(GetPushedOffset());

	CRect rcWingl1( rcButton.TopLeft(), pWingl1->Size());
	CRect rcWingl2(rcWingl1);
	CSize szPushedOffset = GetPushedOffset();

	if(bPushed)
	{
		rcWingl1.OffsetRect(szPushedOffset);
		rcWingl2.OffsetRect(szPushedOffset);
	}

	if(bHorz)
	{
		int nGap = max( szPushedOffset.cy, 1) * 3;

		rcWingl1.OffsetRect(
			(rcButton.Size().cx - pWingl1->Size().cx) / 2 + 1,
			rcButton.Size().cy - pWingl1->Size().cy - nGap);

		if(!bBarIsCompletelyVisible)
			rcWingl2.OffsetRect( (rcButton.Size().cx - pWingl2->Size().cx) / 2 + 1, nGap);
	}
	else
	{
		int nGap = max( szPushedOffset.cx, 1) * 3;

		rcWingl1.OffsetRect(
			nGap,
			(rcButton.Size().cy - pWingl1->Size().cy) / 2 + 1);

		if(!bBarIsCompletelyVisible)
		{
			rcWingl2.OffsetRect(
				rcButton.Size().cx - pWingl2->Size().cx - nGap,
				(rcButton.Size().cy - pWingl2->Size().cy) / 2 + 1);
		}
	}

	if(bEnabled)
	{
		if( bHover && !bPushed )
		{
			COLORREF ColorValuesHover[] = { 0, GetColor(COLOR_3DDKSHADOW)};

			rcWingl1.OffsetRect( -1, -1);
			rcWingl2.OffsetRect( -1, -1);
			PaintWingl(
				dc,
				rcWingl1.TopLeft(),
				*pWingl1,
				ColorValuesHover);

			if(!bBarIsCompletelyVisible)
			{
				PaintWingl(
					dc,
					rcWingl2.TopLeft(),
					*pWingl2,
					ColorValuesHover);
			}

			rcWingl1.OffsetRect( 1, 1);
			rcWingl2.OffsetRect( 1, 1);
		}

		PaintWingl(
			dc,
			rcWingl1.TopLeft(),
			*pWingl1,
			ColorValues);

		if(!bBarIsCompletelyVisible)
		{
			PaintWingl(
				dc,
				rcWingl2.TopLeft(),
				*pWingl2,
				ColorValues);
		}
	}
	else
	{
		ColorValues[1] = GetColor(COLOR_3DSHADOW);
		PaintWingl(
			dc,
			rcWingl1.TopLeft(),
			*pWingl1,
			ColorValues);
	}
}

void CTPaintManager::ExcludeChildAreas( HDC hDC,
									    HWND hWnd,
										CTPaintManager::pfnExcludeChildAreaCallback pCallback,
										LPVOID pCallbackCookie)
{
	if( hDC == NULL || !IsWindow(hWnd) )
		return;

	HWND hWndChild = GetWindow(hWnd,GW_CHILD);
	for(; hWndChild != NULL; hWndChild = GetWindow( hWndChild, GW_HWNDNEXT))
	{
		if(!IsWindowVisible(hWndChild))
			continue;

		DWORD dwChildStyle = DWORD(GetWindowLong( hWndChild, GWL_STYLE));
		if( (dwChildStyle & WS_CHILD) == 0 )
			continue;

		RECT rect;
		if(!GetWindowRect( hWndChild, &rect))
			continue;

		if(!ScreenToClient( hWnd, (LPPOINT) &rect))
			continue;

		if(!ScreenToClient( hWnd, ((LPPOINT) &rect) + 1))
			continue;

		if( pCallback != NULL && pCallback( hDC, hWnd, hWndChild, pCallbackCookie) )
			continue;

		ExcludeClipRect(
			hDC,
			rect.left,
			rect.top,
			rect.right,
			rect.bottom);
	}
}

void CTPaintManager::TransparentBlt( HDC hDEST,
									 int nXOriginDest,
									 int nYOriginDest,
									 int nWidthDest,
									 int hHeightDest,
									 HDC hSRC,
									 int nXOriginSrc,
									 int nYOriginSrc,
									 int nWidthSrc,
									 int nHeightSrc,
									 UINT crTransparent)
{
	CDC dcDST;
	CDC dcSRC;

	dcDST.Attach(hDEST);
	dcSRC.Attach(hSRC);

	CTMemoryDC dcmm(&dcDST);
	CTMemoryDC dcmsk(
		&dcDST,
		NULL,
		CTMemoryDC::MDCOPT_TO_MEMORY|
		CTMemoryDC::MDCOPT_FORCE_BLACK);

	if( (nWidthDest != nWidthSrc || hHeightDest != nHeightSrc) &&
		nWidthDest >= 0 &&
		hHeightDest >= 0 )
	{
		dcmm.StretchBlt(
			0, 0,
			nWidthDest,
			hHeightDest,
			&dcSRC,
			nXOriginSrc,
			nYOriginSrc,
			nWidthSrc,
			nHeightSrc,
			SRCCOPY);
	}
	else
	{
		dcmm.BitBlt(
			0, 0,
			nWidthSrc,
			nHeightSrc,
			&dcSRC,
			nXOriginSrc,
			nYOriginSrc,
			SRCCOPY);
	}

	int nRealWidthDest = nWidthDest < 0 ? nWidthSrc : nWidthDest;
	int nRealHeightDest = hHeightDest < 0 ? nHeightSrc : hHeightDest;

	dcmm.SetBkColor(crTransparent);
	dcmsk.BitBlt(
		0, 0,
		nRealWidthDest,
		nRealHeightDest,
		&dcmm,
		0, 0,
		SRCCOPY);

	dcmm.SetBkColor(RGB( 0, 0, 0));
	dcmm.SetTextColor(RGB( 255, 255, 255));

	dcmm.BitBlt(
		0, 0,
		nRealWidthDest,
		nRealHeightDest,
		&dcmsk,
		0, 0,
		SRCAND);

	dcDST.SetBkColor(RGB( 255, 255, 255));
	dcDST.SetTextColor(RGB( 0, 0, 0));
	dcDST.BitBlt(
		nXOriginDest,
		nYOriginDest,
		nRealWidthDest,
		nRealHeightDest,
		&dcmsk,
		0, 0,
		SRCAND);

	dcDST.BitBlt(
		nXOriginDest,
		nYOriginDest,
		nRealWidthDest,
		nRealHeightDest,
		&dcmm,
		0, 0,
		SRCPAINT);

	dcmm.__Flush(FALSE);
	dcmsk.__Flush(FALSE);

	dcDST.Detach();
	dcSRC.Detach();
}

HBITMAP CTPaintManager::GenLBDIB()
{
	CWindowDC dcAlign(NULL);
	CBitmap bmp;
	CDC dc;

	if(!dc.CreateCompatibleDC(&dcAlign))
		return NULL;

	if(!bmp.CreateCompatibleBitmap( &dcAlign, 16, 16))
		return NULL;

	CBitmap *pOldBmp = dc.SelectObject(&bmp);
	COLORREF clr0 = ::GetSysColor(COLOR_BTNFACE);
	COLORREF clr1 = ::GetSysColor(COLOR_BTNHIGHLIGHT);

	for( int nY=0; nY<16; nY++)
	{
		for( int nX=0; nX<16; nX++)
		{
			COLORREF clr = nY&0x01 ? nX&0x01 ? clr0 : clr1 : nX&0x01 ? clr1 : clr0;
			dc.SetPixel( nX, nY, clr);
		}
	}

	dc.SelectObject(pOldBmp);
	return (HBITMAP) bmp.Detach();
}

void CTPaintManager::PaintGradientRect( CDC &dc,
											 const CRect &rcPaintGradient,
											 COLORREF clrLeft,
											 COLORREF clrRight,
											 BOOL bHorz,
											 UINT nCountOfSteps)
{
	if( rcPaintGradient.left >= rcPaintGradient.right ||
		rcPaintGradient.top >= rcPaintGradient.bottom)
		return;

	UINT nWidth = rcPaintGradient.Width();
	UINT nHeight = rcPaintGradient.Height();

	if(bHorz)
	{
		if( nCountOfSteps > nHeight )
			nCountOfSteps = nHeight;
	}
	else
	{
		if( nCountOfSteps > nWidth )
			nCountOfSteps = nWidth;
	}

	int nBitsPerPixel = GetBPP();
	if( nBitsPerPixel <= 8 )
	{
		UINT nCalcStepSize = bHorz ? nHeight / nCountOfSteps : nWidth  / nCountOfSteps;
		static const UINT nLowColorStepMinSize = 4;

		if( nCalcStepSize < nLowColorStepMinSize )
			nCountOfSteps = bHorz ? nHeight / nLowColorStepMinSize : nWidth  / nLowColorStepMinSize;
	}

	CRect rcItem(rcPaintGradient);
	for( UINT nStep=0, nBackStep=nCountOfSteps; nStep<nCountOfSteps; nStep++, nBackStep--)
	{
		COLORREF clrItem = RGB(
			(GetRValue(clrLeft) * nBackStep + GetRValue(clrRight) * nStep) / nCountOfSteps,
			(GetGValue(clrLeft) * nBackStep + GetGValue(clrRight) * nStep) / nCountOfSteps,
			(GetBValue(clrLeft) * nBackStep + GetBValue(clrRight) * nStep) / nCountOfSteps);

		if(bHorz)
		{
			rcItem.top = rcPaintGradient.bottom - MulDiv( nStep, nHeight, nCountOfSteps);
			rcItem.top--;

			if( nStep == nCountOfSteps && rcItem.top < rcPaintGradient.top )
			{
				rcItem.top = rcPaintGradient.top;
				if( rcItem.top >= rcItem.bottom )
					break;
			}

			if( nBitsPerPixel <= 8 )
			{
				CBrush br(clrItem);
				dc.FillRect( rcItem, &br);
			}
			else
				dc.FillSolidRect( rcItem, clrItem);

			rcItem.bottom = rcItem.top;
		}
		else
		{
			rcItem.right = MulDiv( nStep, nWidth, nCountOfSteps) + rcPaintGradient.left;
			rcItem.right++;

			if( nStep == nCountOfSteps && rcItem.right > rcPaintGradient.right )
			{
				rcItem.right = rcPaintGradient.right;
				if( rcItem.right <= rcItem.left )
					break;
			}

			if( nBitsPerPixel <= 8 )
			{
				CBrush br(clrItem);
				dc.FillRect( rcItem, &br);
			}
			else
				dc.FillSolidRect( rcItem, clrItem);

			rcItem.left = rcItem.right;
		}
	}
}

void CTPaintManager::SyncSysColors()
{
	HFONT hDefaultGuiFont = (HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;

	if( hDefaultGuiFont == NULL )
		hDefaultGuiFont = (HFONT) ::GetStockObject(SYSTEM_FONT);

	if(m_FontNormal.GetSafeHandle())
		m_FontNormal.Detach();

	m_FontNormal.Attach(hDefaultGuiFont);
	static LPCTSTR strVertFontFaceForNt4 = _T("Arial");

	m_FontNormal.GetLogFont(&lf);
	lf.lfWeight = 900;

	if(m_FontBold.GetSafeHandle())
		m_FontBold.DeleteObject();

	m_FontBold.CreateFontIndirect(&lf);
	m_FontNormal.GetLogFont(&lf);

	lf.lfCharSet = SYMBOL_CHARSET;
	lf.lfWeight = 0;
	lf.lfHeight = ::GetSystemMetrics(SM_CYMENUCHECK) - 1;

	_tcscpy( lf.lfFaceName, _T("Marlett"));
	if(m_FontMarlett.GetSafeHandle())
		m_FontMarlett.DeleteObject();

	m_FontMarlett.CreateFontIndirect(&lf);
	m_FontNormal.GetLogFont(&lf);

	lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
	if(m_FontNormalVert.GetSafeHandle())
		m_FontNormalVert.DeleteObject();

	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, strVertFontFaceForNt4);

	m_FontNormalVert.CreateFontIndirect(&lf);
	lf.lfEscapement = __VERT_FONT_ESCAPEMENT_X__;

	if(m_FontNormalVertX.GetSafeHandle())
		m_FontNormalVertX.DeleteObject();

	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, strVertFontFaceForNt4);

	m_FontNormalVertX.CreateFontIndirect(&lf);
	m_FontBold.GetLogFont(&lf);

	lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
	if(m_FontBoldVert.GetSafeHandle())
		m_FontBoldVert.DeleteObject();

	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, strVertFontFaceForNt4);

	m_FontBoldVert.CreateFontIndirect(&lf);
	lf.lfEscapement = __VERT_FONT_ESCAPEMENT_X__;

	if(m_FontBoldVertX.GetSafeHandle())
		m_FontBoldVertX.DeleteObject();

	if( g_PaintManager.m_bIsWinNT4 || g_PaintManager.m_bIsWin9x )
		_tcscpy( lf.lfFaceName, strVertFontFaceForNt4);
	m_FontBoldVertX.CreateFontIndirect(&lf);

	COLORREF clrDefaultExtendColor = ::GetSysColor(COLOR_3DFACE);
	for( int i = m_colors.GetSize(); i <= __TMFC_MAX_SYS_COLOR_VAL; i++)
		m_colors.Add(clrDefaultExtendColor);

	HBRUSH hDefaultExtendBrush = ::GetSysColorBrush(COLOR_3DFACE);
	for(int i = m_brushes.GetSize(); i <= __TMFC_MAX_SYS_COLOR_VAL; i++)
		m_brushes.Add(hDefaultExtendBrush);

	for(int i = __TMFC_MIN_SYS_COLOR_VAL; i <= __TMFC_MAX_SYS_COLOR_VAL; i++)
	{
		m_brushes[i] = ::GetSysColorBrush(i);
		m_colors[i] = ::GetSysColor(i);
	}

	if( m_brushLight.GetSafeHandle() != NULL )
		m_brushLight.DeleteObject();

	int nBitsPerPixel = GetBPP();
	if( nBitsPerPixel > 8 )
	{
		COLORREF clrBtnHilite = GetColor(COLOR_3DHILIGHT);
		COLORREF clrBtnFace = GetColor(COLOR_3DFACE);
		COLORREF clrLight = RGB(
			GetRValue(clrBtnFace) + ((GetRValue(clrBtnHilite) - GetRValue(clrBtnFace)) / 2),
			GetGValue(clrBtnFace) + ((GetGValue(clrBtnHilite) - GetGValue(clrBtnFace)) / 2),
			GetBValue(clrBtnFace) + ((GetBValue(clrBtnHilite) - GetBValue(clrBtnFace)) / 2));

		m_brushLight.CreateSolidBrush(clrLight);
	}
	else
	{
		HBITMAP hbmGray = GenLBDIB();
		CBitmap bmp;

		bmp.Attach(hbmGray);
		m_brushLight.CreatePatternBrush(&bmp);
	}
}

HBRUSH CTPaintManager::GetBrush( int nColorIndex)
{
	int nBrushesCount = m_brushes.GetSize();

	if( __TMFC_MIN_SYS_COLOR_VAL > nColorIndex && nColorIndex >= nBrushesCount )
		nColorIndex = COLOR_3DFACE;

	HBRUSH hBrush = m_brushes[nColorIndex];
	return hBrush;
}

COLORREF CTPaintManager::GetColor( int nColorIndex)
{
	if( nColorIndex >= __TMFC_COLOR_MAP_BASE )
	{
		int nColorIndex2 = 0;

		if(m_mapColorTranslate.Lookup( nColorIndex, nColorIndex2))
			nColorIndex = nColorIndex2;
		else
			nColorIndex = COLOR_3DFACE;
	}

	int nColorCount = m_colors.GetSize();
	if( __TMFC_MIN_SYS_COLOR_VAL > nColorIndex && nColorIndex >= nColorCount )
		nColorIndex = COLOR_3DFACE;

	COLORREF clr = m_colors[nColorIndex];
	return clr;
}

int CTPaintManager::InstallColor( COLORREF clr, int nColorIndex)
{
	int nColorCount = m_colors.GetSize();

	if( nColorIndex < 0 || nColorIndex >= nColorCount )
		nColorIndex = m_colors.Add(clr);
	else
		m_colors[nColorIndex] = clr;

	return nColorIndex;
}

void CTPaintManager::InitTranslatedColors()
{
	m_mapColorTranslate.RemoveAll();

	m_mapColorTranslate[CLR_3DFACE_OUT] = COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_IN] = COLOR_3DFACE;
	m_mapColorTranslate[CLR_3DFACE_DISABLED] = COLOR_3DFACE;

	m_mapColorTranslate[CLR_3DLIGHT_OUT] = COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_IN] = COLOR_3DLIGHT;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED] = COLOR_3DLIGHT;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT] = COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_IN] = COLOR_3DHILIGHT;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED] = COLOR_3DHILIGHT;

	m_mapColorTranslate[CLR_3DSHADOW_OUT] = COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_IN] = COLOR_3DSHADOW;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED] = COLOR_3DSHADOW;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT] = COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN] = COLOR_3DDKSHADOW;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED] = COLOR_3DDKSHADOW;

	m_mapColorTranslate[CLR_TEXT_OUT] = COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN] = COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_DISABLED] = COLOR_3DSHADOW;
}

static const UINT pixels_dock_btn_empty[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_close[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0, 0, 1, 1,
	0, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 1, 1, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_close_dc2k[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 0, 1, 1, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 0, 1, 1, 0, 0,
	0, 1, 1, 0, 0, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_thin_close[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_arrow_up[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_hollow_up[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 0, 0, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_arrow_up_l[] = {
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_wnd_minimize[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_wnd_restore[] = {
	0, 0, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 1, 0, 0, 0, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 0, 1, 0,
	1, 0, 0, 0, 0, 1, 1, 1, 0,
	1, 0, 0, 0, 0, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_wnd_maximize[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1};

static const UINT pixels_dock_wnd_keep[] = {
	0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0};

static const UINT pixels_dock_btn_contexthelp[] = {
	0, 0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 1, 1, 0, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0};

static CTPaintManager::wingl wingl_dock_empty(
	9, 9, 2,
	pixels_dock_btn_empty);

static CTPaintManager::wingl wingl_dock_close(
	9, 9, 2,
	pixels_dock_btn_close);

static CTPaintManager::wingl wingl_dock_close_dc2k(
	9, 9, 2,
	pixels_dock_btn_close_dc2k);

static CTPaintManager::wingl wingl_dock_xp_thin_close(
	9, 9, 2,
	pixels_dock_btn_thin_close);

static CTPaintManager::wingl wingl_dock_arrow_up(
	9, 9, 2,
	pixels_dock_btn_arrow_up);

static CTPaintManager::wingl wingl_dock_arrow_down(
	wingl_dock_arrow_up,
	180);

static CTPaintManager::wingl wingl_dock_arrow_left(
	wingl_dock_arrow_up,
	270);

static CTPaintManager::wingl wingl_dock_arrow_right(
	wingl_dock_arrow_left,
	180);

static CTPaintManager::wingl wingl_dock_wnd_minimize(
	9, 9, 2,
	pixels_dock_wnd_minimize);

static CTPaintManager::wingl wingl_dock_wnd_restore(
	9, 9, 2,
	pixels_dock_wnd_restore);

static CTPaintManager::wingl wingl_dock_wnd_maximize(
	9, 9, 2,
	pixels_dock_wnd_maximize);

static CTPaintManager::wingl wingl_dock_wnd_keep(
	9, 9, 2,
	pixels_dock_wnd_keep);

static CTPaintManager::wingl wingl_dock_wnd_unkeep(
	wingl_dock_wnd_keep,
	90);

static CTPaintManager::wingl wingl_dock_wnd_contexthelp(
	9, 9, 2,
	pixels_dock_btn_contexthelp);

static CTPaintManager::wingl wingl_dock_arrow_up_l(
	9, 9, 2,
	pixels_dock_btn_arrow_up_l);

static CTPaintManager::wingl wingl_dock_arrow_down_l(
	wingl_dock_arrow_up_l,
	180);

static CTPaintManager::wingl wingl_dock_arrow_left_l(
	wingl_dock_arrow_up_l,
	270);

static CTPaintManager::wingl wingl_dock_arrow_right_l(
	wingl_dock_arrow_left_l,
	180);

static CTPaintManager::wingl wingl_dock_hollow_up(
	9, 9, 2,
	pixels_dock_btn_hollow_up);

static CTPaintManager::wingl wingl_dock_hollow_down(
	wingl_dock_hollow_up,
	180);

static CTPaintManager::wingl wingl_dock_hollow_left(
	wingl_dock_hollow_up,
	270);

static CTPaintManager::wingl wingl_dock_hollow_right(
	wingl_dock_hollow_left,
	180);

CTPaintManager::wingl *CTPaintManager::m_DockingCaptionWINGLs[__DCBT_VALUE_MAX + 1] = {
	&wingl_dock_empty,
	&wingl_dock_close,
	&wingl_dock_arrow_up,
	&wingl_dock_arrow_down,
	&wingl_dock_arrow_left,
	&wingl_dock_arrow_right,
	&wingl_dock_wnd_minimize,
	&wingl_dock_wnd_restore,
	&wingl_dock_wnd_maximize,
	&wingl_dock_wnd_contexthelp,
	&wingl_dock_wnd_keep,
	&wingl_dock_wnd_unkeep,
	&wingl_dock_arrow_up_l,
	&wingl_dock_arrow_down_l,
	&wingl_dock_arrow_left_l,
	&wingl_dock_arrow_right_l,
	&wingl_dock_xp_thin_close,
	&wingl_dock_hollow_up,
	&wingl_dock_hollow_down,
	&wingl_dock_hollow_left,
	&wingl_dock_hollow_right,
	&wingl_dock_close_dc2k};

void CTPaintManager::PaintDockingCaptionButton( CDC &dc,
											    const CRect& rcClient,
												e_docking_caption_button nType,
												BOOL bHover,
												BOOL bPushed,
												BOOL bEnabled,
												BOOL bSideBar,
												BOOL bBarWndActive,
												BOOL bFloating)
{
	if(rcClient.IsRectEmpty())
		return;

	if(!bEnabled)
		bHover = FALSE;

	COLORREF ColorValues[2] = {
		RGB( 0, 0, 0),
		RGB( 0, 0, 0)};

	COLORREF clr3dFace = GetColor(COLOR_3DFACE);
	COLORREF clr3dShadow = GetColor(COLOR_3DSHADOW);
	COLORREF clr3dDkShadow = GetColor(COLOR_3DDKSHADOW);
	COLORREF clr3dHiLight = GetColor(COLOR_3DHILIGHT);

	if(!bEnabled)
		ColorValues[1] = clr3dShadow;

	CRect rcWingl(rcClient);
	rcWingl.NormalizeRect();

	if( rcWingl.Width() & 1 )
		rcWingl.left++;

	if( rcWingl.Height() & 1 )
		rcWingl.top++;

	if( nType == __DCBT_CLOSE )
		nType = __DCBT_CLOSE_DC2K;

	wingl *pWingl = m_DockingCaptionWINGLs[nType];
	if(!bFloating)
		rcWingl.InflateRect( 1, 1);

	rcWingl.InflateRect( 1, 1, 0, 0);
	dc.FillSolidRect(
		&rcWingl,
		clr3dFace);

	CRect rcWinglShape(rcWingl);
	if(bPushed)
		rcWinglShape.OffsetRect( 1, 1);

	PaintWinglCentered(
		dc,
		rcWinglShape,
		*pWingl,
		ColorValues);

	if( bEnabled && bPushed )
	{
		dc.Draw3dRect(
			&rcWingl,
			clr3dShadow,
			clr3dHiLight);
	}
	else if( bEnabled && bHover )
	{
		dc.Draw3dRect(
			&rcWingl,
			clr3dFace,
			clr3dDkShadow);

		rcWingl.DeflateRect( 1, 1);
		dc.Draw3dRect(
			&rcWingl,
			clr3dHiLight,
			clr3dShadow);
	}
	else
	{
		dc.Draw3dRect(
			&rcWingl,
			clr3dHiLight,
			clr3dShadow);
	}
}

void CTPaintManagerXP::PaintDockingCaptionButton( CDC &dc,
												  const CRect &rcClient,
												  e_docking_caption_button nType,
												  BOOL bHover,
												  BOOL bPushed,
												  BOOL bEnabled,
												  BOOL bSideBar,
												  BOOL bBarWndActive,
												  BOOL bFloating)
{
	if(rcClient.IsRectEmpty())
		return;

	if(!bEnabled)
		bHover = FALSE;

	COLORREF ColorValues[2] = {
		RGB( 0, 0, 0),
		RGB( 0, 0, 0)};

	if(bEnabled)
	{
		if( !bHover && !bPushed )
			ColorValues[1] = GetColor(bSideBar ? bBarWndActive ? COLOR_HIGHLIGHTTEXT : COLOR_3DDKSHADOW : COLOR_3DHILIGHT);
	}
	else
		ColorValues[1] = GetColor(COLOR_3DSHADOW);

	CRect rcWingl(rcClient);
	rcWingl.NormalizeRect();

	if( bHover || bPushed )
	{
		dc.FillSolidRect(
			&rcWingl,
			GetColor( bPushed ? XPCLR_HILIGHT : CLR_3DFACE_IN));

		CRect rcBtnBorder(rcWingl);
		rcBtnBorder.InflateRect( 1, 1);

		COLORREF clrBtnBorder = GetColor( bBarWndActive ? XPCLR_HILIGHT : XPCLR_HILIGHT_BORDER);
		dc.Draw3dRect(
			rcBtnBorder,
			clrBtnBorder,
			clrBtnBorder);
	}

	if( rcWingl.Width() & 1 )
		rcWingl.left++;

	if( rcWingl.Height() & 1 )
		rcWingl.top++;

	if( bSideBar && !bFloating && nType == __DCBT_CLOSE )
		nType = __DCBT_XP_THIN_CLOSE;

	wingl *pWingl = m_DockingCaptionWINGLs[nType];
	PaintWinglCentered(
		dc,
		rcWingl,
		*pWingl,
		ColorValues);
}

void CTPaintManager::PaintPushButton( CDC &dc,
									  BOOL bHorz,
									  const CRect &rcClient,
									  LPCTSTR strText,
									  HICON hIcon,
									  BOOL bFlat,
									  BOOL bHover,
									  BOOL bPushed,
									  BOOL bIndeterminate,
									  BOOL bEnabled,
									  BOOL bDrawBorder,
									  BOOL bDrawFocusRect,
									  BOOL bDefaultPushButton,
									  int nAlign,
									  CFont *pFont,
									  BOOL bDropDown,
									  UINT nHelperSysCmdID,
									  BOOL bTransparentBackground)
{
	if(rcClient.IsRectEmpty())
		return;

	if(!bEnabled)
	{
		bHover = FALSE;
		bDrawFocusRect = FALSE;
		bIndeterminate = FALSE;
	}

	COLORREF clrOldText = dc.GetTextColor();
	COLORREF clrOldBk = dc.GetBkColor();
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	COLORREF bgColor;
	if( bHover || bPushed )
		bgColor = GetColor(CLR_3DFACE_IN);
	else
		bgColor = GetColor(CLR_3DFACE_OUT);

	if(!bTransparentBackground)
	{
		if( bEnabled && bPushed && bHover )
			dc.FillRect( &rcClient, &m_brushLight);
		else
		{
			CBrush br(bgColor);
			dc.FillRect( &rcClient, &br);
		}
	}

	if(bPushed)
	{
		if(bFlat)
		{
			if(bDrawBorder)
			{
				dc.Draw3dRect(
					rcClient,
					GetColor(CLR_3DSHADOW_IN),
					GetColor(CLR_3DHILIGHT_IN));
			}
		}
		else
		{
			dc.Draw3dRect(
				rcClient,
				GetColor(CLR_3DSHADOW_IN),
				GetColor(CLR_3DHILIGHT_IN));
		}
	}
	else if(bFlat)
	{
		if( bHover && bDrawBorder )
		{
			dc.Draw3dRect(
				rcClient,
				GetColor(CLR_3DHILIGHT_OUT),
				GetColor(CLR_3DSHADOW_OUT));
		}
	}
	else if(bHover)
	{
		CRect rect(rcClient);

		dc.Draw3dRect(
			rect,
			GetColor(CLR_3DSHADOW_OUT),
			GetColor(CLR_3DDKSHADOW_OUT));

		rect.DeflateRect( 1, 1);
		dc.Draw3dRect(
			rect,
			GetColor(CLR_3DHILIGHT_OUT),
			GetColor(CLR_3DSHADOW_OUT));
	}
	else
	{
		dc.Draw3dRect(
			rcClient,
			GetColor(CLR_3DHILIGHT_OUT),
			GetColor(CLR_3DSHADOW_OUT));
	}

	CString strTitle(_T(""));
	if( strText != NULL )
		strTitle = strText;

	CRect rcClient2(rcClient);
	if(bDropDown)
	{
		CRect rcDopArea(rcClient2);
		COLORREF ColorValues[2] = {
			RGB( 0, 0, 0),
			GetColor( bEnabled ? CLR_TEXT_OUT : CLR_3DSHADOW_OUT)};

		if(bHorz)
		{
			rcClient2.right -= __DROPDOWN_ARROW_GAP * 2 + m_wingl_btn_expand_bottom.Size().cx;
			rcDopArea.left = rcClient2.right;
			rcDopArea.DeflateRect( 1, 1);
			rcDopArea.OffsetRect( -1, 0);

			if(bPushed)
				rcDopArea.OffsetRect(GetPushedOffset());

			PaintWinglCentered(
				dc,
				rcDopArea,
				m_wingl_btn_expand_bottom,
				ColorValues);
		}
		else
		{
			rcClient2.top += __DROPDOWN_ARROW_GAP * 2 + m_wingl_btn_expand_right.Size().cy;
			rcDopArea.bottom = rcClient2.top;

			rcDopArea.DeflateRect( 1, 1);
			rcDopArea.OffsetRect( 0, 1);

			if(bPushed)
				rcDopArea.OffsetRect(GetPushedOffset());

			PaintWinglCentered(
				dc,
				rcDopArea,
				m_wingl_btn_expand_right,
				ColorValues);
		}
	}

	CRect rcCaption = rcClient2;
	if(	rcClient2.bottom > rcClient2.top && rcClient2.right > rcClient2.left )
	{
		if( hIcon != NULL )
		{
			PaintIcon(
				dc,
				bHorz,
				strTitle,
				hIcon,
				rcClient2,
				rcCaption,
				bPushed,
				bEnabled,
				bHover,
				nAlign);
		}
		else
		{
			PaintSysCmdWINGL(
				dc,
				nHelperSysCmdID,
				rcClient2,
				FALSE,
				bPushed,
				bEnabled);
		}
	}

	if( rcCaption.bottom > rcCaption.top &&
		rcCaption.right > rcCaption.left )
	{
		if(!strTitle.IsEmpty())
		{
			if(bPushed)
			{
				CSize szPushedOffset = GetPushedOffset();
				rcCaption.OffsetRect(szPushedOffset);
			}
			dc.SetBkColor(bgColor);

			if(bEnabled)
			{
				if( bHover || bPushed )
					dc.SetTextColor(GetColor(COLOR_3DDKSHADOW));
				else
					dc.SetTextColor(GetColor(COLOR_BTNTEXT));
			}
			else
				dc.SetTextColor(GetColor(COLOR_3DFACE));

			CFont *pOldBtnFont = NULL;
			CFont *pCurrFont = NULL;
			CFont fontDummy;

			if( pFont != NULL )
			{
				if(bHorz)
					pCurrFont = pFont;
				else
				{
					LOGFONT lf;

					::memset( &lf, 0, sizeof(LOGFONT));
					pFont->GetLogFont(&lf);

					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
						fontDummy.CreateFontIndirect(&lf);
						pCurrFont = &fontDummy;
					}
					else
						pCurrFont = pFont;
				}
			}
			else if(bHorz)
			{
				if(bDefaultPushButton)
					pCurrFont = &m_FontBold;
				else
					pCurrFont = &m_FontNormal;
			}
			else if(bDefaultPushButton)
				pCurrFont = &m_FontBoldVert;
			else
				pCurrFont = &m_FontNormalVert;

			pOldBtnFont = dc.SelectObject(pCurrFont);
			int nTextLength = strTitle.GetLength();
			int nAmpIndex = strTitle.Find(_T('&'));

			CString strBtn;
			if( nAmpIndex < 0 )
				strBtn = strTitle;
			else
				strBtn = strTitle.Left(nAmpIndex) + strTitle.Right(nTextLength - (nAmpIndex + 1));

			CRect rcText( 0, 0, 0, 0);
			dc.DrawText(
				strBtn, -1,
				rcText,
				DT_SINGLELINE|DT_CALCRECT);

			CRect rcCenteredCaption(rcCaption);
			rcCenteredCaption.OffsetRect(
				(rcCaption.Width() - rcText.Width()) / 2,
				(rcCaption.Height() - rcText.Height()) / 2);

			if(bHorz)
			{
				dc.DrawState(
					rcCenteredCaption.TopLeft(),
					rcCenteredCaption.Size(),
					(LPCTSTR) strTitle,
					bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
					TRUE, 0,
					(CBrush *) NULL);
			}
			else
			{
				LOGFONT lf;
				::memset( &lf, 0, sizeof(LOGFONT));

				pCurrFont->GetLogFont(&lf);
				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin = (rcCaption.Width() - _cyHorzFont) / 2;

				CPoint ptLineFrom( 0, 0);
				CPoint ptLineTo( 0, 0);

				if( nAmpIndex >= 0 )
				{
					ptLineFrom = CPoint( _cyTextMargin, CXTEXTMARGIN + CalcTextWidth( dc, bDefaultPushButton ? m_FontBold : m_FontNormal, strBtn.Left(nAmpIndex)));
					ptLineTo = CPoint( _cyTextMargin, CXTEXTMARGIN + CalcTextWidth( dc, bDefaultPushButton ? m_FontBold : m_FontNormal, strBtn.Left(nAmpIndex + 1)));
				}

				CRect rcString = CRect(
					CPoint( rcCaption.right - _cyTextMargin, rcCaption.top + CXTEXTMARGIN),
					rcCaption.Size());

				rcString.DeflateRect( 2, 2);
				CSize ptFix( -rcString.Width() - 1, 0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;

				if(bEnabled)
				{
					dc.DrawText(
						strBtn,
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX);

					if( nAmpIndex >= 0 )
					{
						CPen pen( PS_SOLID, 0, dc.GetTextColor());
						CPen *pOldPen = dc.SelectObject(&pen);

						dc.MoveTo(ptLineFrom);
						dc.LineTo(ptLineTo);
						dc.SelectObject(pOldPen);
					}
				}
				else
				{
					COLORREF clrShadow = GetColor(COLOR_3DHILIGHT);
					COLORREF clrText = GetColor(COLOR_3DSHADOW);

					CPoint ptOffsetPost( -1, -1);
					CPoint ptOffsetPre( 1, 1);

					rcString.OffsetRect( ptOffsetPre.x, ptOffsetPre.y);
					dc.SetTextColor(clrShadow);

					dc.DrawText(
						strBtn,
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX);

					rcString.OffsetRect( ptOffsetPost.x, ptOffsetPost.y);
					dc.SetTextColor(clrText);

					dc.DrawText(
						strBtn,
						rcString,
						DT_SINGLELINE|DT_NOCLIP|DT_NOPREFIX);

					if( nAmpIndex >= 0 )
					{
						CPen pen( PS_SOLID, 0, clrShadow);
						CPen *pOldPen = dc.SelectObject(&pen);

						ptLineFrom += ptOffsetPre;
						ptLineTo += ptOffsetPre;
						dc.MoveTo(ptLineFrom);
						dc.LineTo(ptLineTo);
						pen.DeleteObject();
						pen.CreatePen(
							PS_SOLID,
							0, clrText);

						dc.SelectObject(&pen);
						ptLineFrom += ptOffsetPost;
						ptLineTo += ptOffsetPost;
						dc.MoveTo(ptLineFrom);
						dc.LineTo(ptLineTo);
						dc.SelectObject(pOldPen);
					}
				}
			}

			dc.SelectObject(pOldBtnFont);
		}

		if(bDrawFocusRect)
		{
			CRect focusRect = rcCaption;

			focusRect.DeflateRect( 3, 3);
			dc.DrawFocusRect(&focusRect);
		}
	}

	dc.SetBkMode(nOldBkMode);
	dc.SetBkColor(clrOldBk);
	dc.SetTextColor(clrOldText);
}

CRect CTPaintManager::CalcTextDimension( CDC& dc,
										 CFont& font,
										 const CString& strText)
{
	CRect rcText( 0, 0, 0, 0);
	CFont *pOldFont = dc.SelectObject(&font);

	dc.DrawText(
		strText,
		&rcText,
		DT_SINGLELINE|DT_CALCRECT|DT_LEFT|DT_VCENTER);

	dc.SelectObject(pOldFont);
	rcText.OffsetRect(-rcText.TopLeft());

	return rcText;
}

int CTPaintManager::CalcTextWidth( CDC &dc,
								   CFont &font,
								   const CString &strText)
{
	return CTPaintManager::CalcTextDimension(
		dc,
		font,
		strText).Width();
}

void CTPaintManager::PaintMenuSeparator( CDC &dc,
										 CRect &rcItem,
										 BOOL bRarelyUsed)
{
	CRect rcItem2(rcItem);

	if(bRarelyUsed)
	{
		rcItem2.InflateRect( 3, 0, 4, 0);
		dc.FillRect(
			&rcItem2,
			&m_brushLight);

		rcItem2.DeflateRect( 3, 0, 4, 0);
	}
	else
		dc.FillSolidRect( &rcItem2, GetColor(CLR_3DFACE_OUT));

	int nFunnyDeflateMetric = ::GetSystemMetrics(SM_CXSMICON) / 2;
	rcItem2.DeflateRect( nFunnyDeflateMetric, 0);

	COLORREF clrBtnShadow = GetColor( bRarelyUsed ? CLR_3DFACE_OUT : CLR_3DSHADOW_OUT);
	COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);

	int y = (rcItem2.top + rcItem2.bottom) / 2;
	rcItem2.top = y - 1;
	rcItem2.bottom = y + 1;

	dc.Draw3dRect(
		rcItem2,
		clrBtnShadow,
		clrBtnHilight);
}

void CTPaintManagerXP::PaintMenuSeparator( CDC &dc,
										   CRect &rcItem,
										   BOOL bRarelyUsed)
{
	if(rcItem.IsRectEmpty())
		return;

	COLORREF clr3dFace = GetColor(bRarelyUsed ? XPCLR_RARELY_BORDER : CLR_3DFACE_OUT);
	CRect rc(rcItem);

	rc.right = rc.left + __DEF_MENU_ICON_AREA_DX;
	dc.FillSolidRect(
		rc,
		clr3dFace);

	rc = rcItem;
	rc.left += __DEF_MENU_ICON_AREA_DX;

	dc.FillSolidRect(
		rc,
		GetColor(XPCLR_3DFACE_NORMAL));

	rc.left += __DEF_MENU_GAP;
	rc.top += rc.Height() / 2;
	rc.bottom = rc.top + 1;
	dc.FillSolidRect(
		rc,
		clr3dFace);
}

void CTPaintManager::PaintSysCmdWINGL( CDC &dc,
									   UINT nHelperSysCmdID,
									   const CRect& rcItem,
									   BOOL bHover,
									   BOOL bPushed,
									   BOOL bEnabled)
{
	e_docking_caption_button nWingl = __DCBT_EMPTY;

	switch(nHelperSysCmdID)
	{
	case SC_CLOSE		: nWingl = __DCBT_CLOSE; break;
	case SC_RESTORE		: nWingl = __DCBT_WND_RESTORE; break;
	case SC_MINIMIZE	: nWingl = __DCBT_WND_MINIMIZE; break;
	case SC_MAXIMIZE	: nWingl = __DCBT_WND_MAXIMIZE; break;
	case SC_CONTEXTHELP	: nWingl = __DCBT_WND_CONTEXTHELP; break;
	}

	if( nWingl == __DCBT_EMPTY )
		return;

	COLORREF ColorValues[2] = {
		RGB( 0, 0, 0),
		RGB( 0, 0, 0)};

	CRect _rcItem(rcItem);
	if(bPushed)
		_rcItem.OffsetRect(GetPushedOffset());

	if(bEnabled)
		ColorValues[1] = GetColor( bHover ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_OUT);
	else
	{
		ColorValues[1] = GetColor(CLR_3DHILIGHT_OUT);
		_rcItem.OffsetRect( 1, 1);

		PaintWinglCentered(
			dc,
			_rcItem,
			*m_DockingCaptionWINGLs[nWingl],
			ColorValues);

		_rcItem.OffsetRect( -1, -1);
		ColorValues[1] = GetColor(CLR_3DSHADOW_OUT);
	}

	PaintWinglCentered(
		dc,
		_rcItem,
		*m_DockingCaptionWINGLs[nWingl],
		ColorValues);
}

void CTPaintManagerXP::PaintSysCmdWINGL( CDC &dc,
										 UINT nHelperSysCmdID,
										 const CRect& rcItem,
										 BOOL bHover,
										 BOOL bPushed,
										 BOOL bEnabled)
{
	e_docking_caption_button nWingl = __DCBT_EMPTY;

	switch(nHelperSysCmdID)
	{
	case SC_CLOSE		: nWingl = __DCBT_CLOSE; break;
	case SC_RESTORE		: nWingl = __DCBT_WND_RESTORE; break;
	case SC_MINIMIZE	: nWingl = __DCBT_WND_MINIMIZE; break;
	case SC_MAXIMIZE	: nWingl = __DCBT_WND_MAXIMIZE; break;
	case SC_CONTEXTHELP	: nWingl = __DCBT_WND_CONTEXTHELP; break;
	}

	if( nWingl == __DCBT_EMPTY )
		return;

	COLORREF ColorValues[2] = {
		RGB(0,0,0),
		RGB(0,0,0)};
	CRect _rcItem(rcItem);

	if(bPushed)
		_rcItem.OffsetRect(GetPushedOffset());

	if(bEnabled)
	{
		if( bHover && !bPushed )
		{
			ColorValues[1] = GetColor(COLOR_3DDKSHADOW);
			PaintWinglCentered(
				dc,
				_rcItem,
				*m_DockingCaptionWINGLs[nWingl],
				ColorValues);

			_rcItem.OffsetRect( -1, -1);
		}

		ColorValues[1] = GetColor(CLR_TEXT_OUT);
	}
	else
		ColorValues[1] = GetColor(COLOR_3DSHADOW);

	PaintWinglCentered(
		dc,
		_rcItem,
		*m_DockingCaptionWINGLs[nWingl],
		ColorValues);
}

void CTPaintManager::PaintMenuItem( CDC &dc,
								    const CRect &rcItem,
									LPCTSTR strText,
									LPCTSTR strAccelText,
									HICON hIcon,
									BOOL bPopup,
									BOOL bSelected,
									BOOL bPushedCheck,
									BOOL bPushedRadio,
									BOOL bIndeterminate,
									BOOL bEnabled,
									BOOL bRarelyUsed,
									BOOL bRarelyUsedPrev,
									BOOL bRarelyUsedNext,
									UINT nHelperSysCmdID)
{
	if(rcItem.IsRectEmpty())
		return;

	CString _strAccelText = !strAccelText ? _T("") : strAccelText;
	CString _strText = !strText ? _T("") : strText;

	COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
	COLORREF clrBk = clr3dFace;
	COLORREF clrText = GetColor(CLR_TEXT_OUT);
	CRect rcBk(rcItem);

	dc.FillSolidRect( rcBk, clrBk);
	if(bSelected)
	{
		clrText = GetColor(COLOR_HIGHLIGHTTEXT);
		clrBk = GetColor(COLOR_HIGHLIGHT);
	}

	if(bRarelyUsed)
	{
		rcBk.InflateRect( 3, 0, 4, 0);
		dc.FillRect(
			rcBk,
			&m_brushLight);

		if(!bRarelyUsedPrev)
		{
			CPen penShadow(
				PS_SOLID, 1,
				GetColor(COLOR_3DSHADOW));

			CPen *pOldPen = dc.SelectObject(&penShadow);
			dc.MoveTo( rcBk.left, rcBk.top);
			dc.LineTo( rcBk.right, rcBk.top);
			dc.SelectObject(pOldPen);
		}

		if(!bRarelyUsedNext)
		{
			CPen penLight(
				PS_SOLID, 1,
				GetColor(COLOR_3DHIGHLIGHT));

			CPen *pOldPen = dc.SelectObject(&penLight);
			dc.MoveTo( rcBk.left, rcBk.bottom - 1);
			dc.LineTo( rcBk.right, rcBk.bottom - 1);
			dc.SelectObject(pOldPen);
		}

		rcBk.DeflateRect( 3, 0, 4, 0);
	}

	BOOL bSmallerSelRect = hIcon != NULL || bPushedCheck || bPushedRadio;
	if(bSmallerSelRect)
		rcBk.left += __DEF_MENU_ICON_AREA_DX;

	if( bSelected || !bRarelyUsed )
		dc.FillSolidRect( rcBk, clrBk);

	if(!bSmallerSelRect)
		rcBk.left += __DEF_MENU_ICON_AREA_DX;

	rcBk.DeflateRect(
		__DEF_MENU_GAP,
		__DEF_MENU_GAP);

	COLORREF clrOldText = dc.SetTextColor(clrText);
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	CFont *pOldFont = dc.SelectObject(&m_FontNormal);
	dc.DrawState(
		rcBk.TopLeft(),
		rcBk.Size(),
		(LPCTSTR) _strText,
		bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
		TRUE, 0,
		(CBrush *) NULL);

	if(!_strAccelText.IsEmpty())
	{
		CSize size;
		CRect rect( 0, 0, 0, 0);

		size.cy = dc.DrawText(
			(LPCTSTR) _strAccelText,
			_strAccelText.GetLength(),
			&rect,
			DT_CALCRECT|
			DT_SINGLELINE|
			DT_LEFT|
			DT_VCENTER);

		size.cx = rect.Width();
		rcBk.left = rcBk.right - size.cx - __DEF_MENU_POPUP_ARROW_AREA_DX;

		dc.DrawState(
			rcBk.TopLeft(),
			rcBk.Size(),
			(LPCTSTR) _strAccelText,
			bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
			TRUE, 0,
			(CBrush *) NULL);
	}

	if(bPopup)
	{
		CRect rcPopupArrow(rcItem);

		rcPopupArrow.DeflateRect(
			__DEF_MENU_GAP,
			__DEF_MENU_GAP);

		rcPopupArrow.left = rcPopupArrow.right - __DEF_MENU_POPUP_ARROW_AREA_DX;
		dc.SelectObject(&m_FontMarlett);

		dc.DrawText(
			_T("8"),
			rcPopupArrow,
			DT_CENTER|
			DT_VCENTER|
			DT_SINGLELINE);
	}

	dc.SelectObject(&pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

	CRect rcIcon(rcItem);
	rcIcon.DeflateRect(
		__DEF_MENU_GAP,
		__DEF_MENU_GAP);

	rcIcon.right = rcIcon.left + __DEF_MENU_ICON_CY;
	if( rcIcon.Height() != __DEF_MENU_ICON_CY )
	{
		rcIcon.DeflateRect( 0, rcIcon.Height() / 2);
		if( rcIcon.Height() != __DEF_MENU_ICON_CY )
			rcIcon.bottom = rcIcon.top + __DEF_MENU_ICON_CY;
	}

	CRect rcIconFrame(rcIcon);
	rcIconFrame.InflateRect(
		__DEF_MENU_IMG_RECT_OUT_GAP,
		__DEF_MENU_IMG_RECT_OUT_GAP);

	if( bPushedCheck || bPushedRadio )
		dc.FillRect( rcIconFrame, &m_brushLight);

	if( hIcon != NULL )
	{
		dc.DrawState(
			rcIcon.TopLeft(),
			rcIcon.Size(),
			hIcon,
			bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
			(CBrush *) NULL);

		if( bSelected &&
			bEnabled &&
			!bPushedCheck &&
			!bPushedRadio)
		{
			dc.Draw3dRect(
				&rcIconFrame,
				GetColor(CLR_3DHILIGHT_OUT),
				GetColor(CLR_3DSHADOW_OUT));
		}
	}
	else
	{
		PaintSysCmdWINGL(
			dc,
			nHelperSysCmdID,
			rcIcon,
			bSelected,
			FALSE,
			bEnabled);
	}

	if( bPushedCheck || bPushedRadio )
	{
		if( hIcon == NULL )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width() / 2 - 3;
			int y = rcIcon.top + rcIcon.Height() / 2 - 3;

			if(bPushedCheck)
				PaintCheckMark( dc, x, y, clrCheck);
			else
				PaintRadioDot( dc, x, y, clrCheck);
		}

		dc.Draw3dRect(
			rcIconFrame,
			GetColor(CLR_3DSHADOW_OUT),
			GetColor(CLR_3DHILIGHT_OUT));
	}
}

void CTPaintManagerXP::PaintMenuItem( CDC &dc,
									  const CRect &rcItem,
									  LPCTSTR strText,
									  LPCTSTR strAccelText,
									  HICON hIcon,
									  BOOL bPopup,
									  BOOL bSelected,
									  BOOL bPushedCheck,
									  BOOL bPushedRadio,
									  BOOL bIndeterminate,
									  BOOL bEnabled,
									  BOOL bRarelyUsed,
									  BOOL bRarelyUsedPrev,
									  BOOL bRarelyUsedNext,
									  UINT nHelperSysCmdID)
{
	if(rcItem.IsRectEmpty())
		return;

	CString _strAccelText = !strAccelText ? _T("") : strAccelText;
	CString _strText = !strText ? _T("") : strText;

	COLORREF clr3dFace = GetColor(bRarelyUsed ? XPCLR_RARELY_BORDER : CLR_3DFACE_OUT);
	COLORREF clrDkBorder = GetColor(XPCLR_HILIGHT_BORDER);
	COLORREF clrBk = clr3dFace;
	COLORREF clrText = GetColor(CLR_TEXT_OUT);
	COLORREF clrBkHilight = GetColor(XPCLR_HILIGHT);
	COLORREF clrIconBkHilight = bSelected ? clrBkHilight : GetColor(CLR_3DFACE_IN);
	COLORREF clrRight = GetColor(XPCLR_3DFACE_NORMAL);
	CRect rcBk(rcItem);

	dc.FillSolidRect( rcBk, clrBk);
	if(bSelected)
		clrBk = GetColor(CLR_3DFACE_IN);
	dc.FillSolidRect( rcBk, clrBk);

	if(bSelected)
	{
		dc.Draw3dRect(
			rcBk,
			clrDkBorder,
			clrDkBorder);

		rcBk.left += __DEF_MENU_ICON_AREA_DX;
	}
	else
	{
		rcBk.left += __DEF_MENU_ICON_AREA_DX;
		dc.FillSolidRect( rcBk, clrRight);
	}

	rcBk.DeflateRect(
		__DEF_MENU_GAP,
		__DEF_MENU_GAP);

	COLORREF clrOldText = dc.SetTextColor(clrText);
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	CFont *pOldFont = dc.SelectObject(&m_FontNormal);
	dc.DrawState(
		rcBk.TopLeft(),
		rcBk.Size(),
		(LPCTSTR) _strText,
		bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
		TRUE, 0,
		bEnabled ? (CBrush *) NULL : CBrush::FromHandle(GetBrush(COLOR_3DSHADOW)));

	if(!_strAccelText.IsEmpty())
	{
		CSize size;
		CRect rect( 0, 0, 0, 0);

		size.cy = dc.DrawText(
			(LPCTSTR) _strAccelText,
			_strAccelText.GetLength(),
			&rect,
			DT_CALCRECT|
			DT_SINGLELINE|
			DT_LEFT|
			DT_VCENTER);

		size.cx = rect.Width();
		rcBk.left = rcBk.right - size.cx - __DEF_MENU_POPUP_ARROW_AREA_DX;

		dc.DrawState(
			rcBk.TopLeft(),
			rcBk.Size(),
			(LPCTSTR) _strAccelText,
			bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
			TRUE, 0,
			bEnabled ? (CBrush *) NULL : CBrush::FromHandle(GetBrush(COLOR_3DSHADOW)));
	}

	if(bPopup)
	{
		CRect rcPopupArrow(rcItem);

		rcPopupArrow.DeflateRect(
			__DEF_MENU_GAP,
			__DEF_MENU_GAP);

		rcPopupArrow.left = rcPopupArrow.right - __DEF_MENU_POPUP_ARROW_AREA_DX;
		dc.SelectObject(&m_FontMarlett);

		dc.DrawText(
			_T("8"),
			rcPopupArrow,
			DT_CENTER|
			DT_VCENTER|
			DT_SINGLELINE);
	}

	dc.SelectObject(&pOldFont);
	dc.SetBkMode(nOldBkMode);
	dc.SetTextColor(clrOldText);

	CRect rcIcon(rcItem);
	rcIcon.DeflateRect(
		__DEF_MENU_GAP,
		__DEF_MENU_GAP);

	rcIcon.right = rcIcon.left + __DEF_MENU_ICON_CY;
	if( rcIcon.Height() != __DEF_MENU_ICON_CY )
	{
		rcIcon.DeflateRect( 0, rcIcon.Height() / 2);
		if( rcIcon.Height() != __DEF_MENU_ICON_CY )
			rcIcon.bottom = rcIcon.top + __DEF_MENU_ICON_CY;
	}

	CRect rcIconFrame(rcIcon);
	rcIconFrame.InflateRect(
		__DEF_MENU_IMG_RECT_OUT_GAP,
		__DEF_MENU_IMG_RECT_OUT_GAP);

	if( bPushedCheck || bPushedRadio )
	{
		dc.FillSolidRect(
			rcIconFrame,
			clrIconBkHilight);
	}

	if( hIcon != NULL )
	{
		CString strTitle(_T(""));
		CRect rcDummy;

		PaintIcon(
			dc,
			TRUE,
			strTitle,
			hIcon,
			rcIcon,
			rcDummy,
			bPushedCheck || bPushedRadio,
			bEnabled,
			bSelected, 0);
	}
	else
	{
		PaintSysCmdWINGL(
			dc,
			nHelperSysCmdID,
			rcIcon,
			bSelected,
			FALSE,
			bEnabled);
	}

	if( bPushedCheck || bPushedRadio )
	{
		if( hIcon == NULL )
		{
			COLORREF clrCheck = GetColor(CLR_TEXT_OUT);
			int x = rcIcon.left + rcIcon.Width() / 2 - 3;
			int y = rcIcon.top + rcIcon.Height() / 2 - 3;

			if(bPushedCheck)
				PaintCheckMark( dc, x, y, clrCheck);
			else
				PaintRadioDot( dc, x, y, clrCheck);
		}

		dc.Draw3dRect(
			rcIconFrame,
			clrDkBorder,
			clrDkBorder);
	}
}

void CTPaintManager::PaintIcon( CDC &dc,
							    BOOL bHorz,
								CString &strTitle,
								HICON hIcon,
								const CRect &rcItem,
								CRect &rcCaption,
								BOOL bPushed,
								BOOL bEnabled,
								BOOL bHover,
								int nAlign)
{
	rcCaption = rcItem;

	if( hIcon == NULL )
		return;

	ICONINFO ii;
	ZeroMemory( &ii, sizeof(ICONINFO));
	::GetIconInfo( hIcon, &ii);

	BYTE cxIcon = (BYTE) ii.xHotspot * 2;
	BYTE cyIcon = (BYTE) ii.yHotspot * 2;
	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	CRect iconRect = rcItem;
	CRect btnRect;

	if(bHorz)
	{
		if(nAlign&__ALIGN_HORIZ_CENTER)
		{
			if(strTitle.IsEmpty())
				iconRect.left += (iconRect.Width() - cxIcon) / 2;
			else
			{
				iconRect.left += 3;
				rcCaption.left += cxIcon + 3;
			}
		}
		else if(nAlign&__ALIGN_HORIZ_RIGHT)
		{
			btnRect = rcCaption;

			if(strTitle.IsEmpty())
				iconRect.left += (iconRect.Width() - cxIcon) / 2;
			else
			{
				rcCaption.right = rcCaption.Width() - cxIcon - 3;
				rcCaption.left = 3;

				iconRect.left = btnRect.right - cxIcon - 3;
				iconRect.top += (iconRect.Height() - cyIcon) / 2;
			}
		}

		if(nAlign&__ALIGN_VERT)
			iconRect.top += (iconRect.Height() - cyIcon) / 2;
	}
	else
	{
		if(nAlign&__ALIGN_HORIZ_CENTER)
		{
			if(strTitle.IsEmpty())
				iconRect.top += (iconRect.Height() - cyIcon) / 2;
			else
			{
				iconRect.top += 3;
				rcCaption.top += cyIcon + 3;
			}
		}
		else
		{
			if(nAlign&__ALIGN_HORIZ_RIGHT)
			{
				btnRect = rcCaption;

				if(strTitle.IsEmpty())
					iconRect.top += (iconRect.Height() - cyIcon) / 2;
				else
				{
					rcCaption.top = rcCaption.Height() - cyIcon - 3;
					rcCaption.top = 3;
					iconRect.top = btnRect.top - cyIcon - 3;
					iconRect.left += (iconRect.Width() - cxIcon) / 2;
				}
			}
		}

		if(nAlign&__ALIGN_VERT)
			iconRect.left += (iconRect.Width() - cxIcon) / 2;
	}

	if(bPushed)
	{
		CSize szPushedOffset = GetPushedOffset();
		iconRect.OffsetRect( szPushedOffset.cx, szPushedOffset.cy);
	}

	dc.DrawState(
		iconRect.TopLeft(),
		iconRect.Size(),
		hIcon,
		bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
		(CBrush *) NULL);
}

CSize CTPaintManager::GetPushedOffset()
{
	return CSize( 1, 1);
}

void CTPaintManagerXP::PaintSeparator( CDC& dc,
									   CRect& rcItem,
									   BOOL bHorz)
{
	CRect rcItem2(rcItem);

	dc.FillSolidRect(
		rcItem2,
		GetColor(CLR_3DFACE_OUT));

	if(bHorz)
	{
		rcItem2.left = rcItem2.left + rcItem2.Width() / 2 - 1;
		rcItem2.right = rcItem2.left + 1;
		rcItem2.DeflateRect( 0, 1);
	}
	else
	{
		rcItem2.top = rcItem2.top + rcItem2.Height() / 2 - 1;
		rcItem2.bottom = rcItem2.top + 1;
		rcItem2.DeflateRect( 1, 0);
	}

	dc.FillSolidRect(
		&rcItem2,
		GetColor(XPCLR_SEPARATOR));
}

void CTPaintManager::PaintSeparator( CDC& dc,
									 CRect& rcItem,
									 BOOL bHorz)
{
	CRect rcItem2(rcItem);

	dc.FillSolidRect(
		rcItem2,
		GetColor(CLR_3DFACE_OUT));

	COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	COLORREF clrBtnShadow = GetColor(CLR_3DSHADOW_OUT);

	if(bHorz)
	{
		int x = (rcItem2.left + rcItem2.right) / 2;

		rcItem2.left = x - 1;
		rcItem2.right = x + 1;
		rcItem2.DeflateRect( 0, 0, 0, 1);

		dc.Draw3dRect(
			rcItem2,
			clrBtnShadow,
			clrBtnHilight);
	}
	else
	{
		int y = (rcItem2.top + rcItem2.bottom) / 2;

		rcItem2.top = y - 1;
		rcItem2.bottom = y + 1;

		rcItem2.DeflateRect( 0, 0, 1, 0);
		dc.Draw3dRect(
			rcItem2,
			clrBtnShadow,
			clrBtnHilight);
	}
}

void CTPaintManager::PaintControlFrame( CDC& dc,
									    CRect& rcClient,
										BOOL bFlat,
										BOOL bHover,
										BOOL bEnabled,
										BOOL bFocus)
{
	CRect rcItem(rcClient);
	COLORREF clrInner = GetColor(COLOR_WINDOW);
	COLORREF clrIn1;
	COLORREF clrIn2;
	COLORREF clrOut1;
	COLORREF clrOut2;

	clrIn1 = clrIn2 = clrOut1 = clrOut2 = GetColor(CLR_3DFACE_OUT);
	if(bEnabled)
	{
		if( bHover && bFocus )
		{
			clrIn1 = GetColor(COLOR_3DFACE);
			clrIn2 = GetColor(COLOR_3DDKSHADOW);
			clrOut1 = GetColor(COLOR_3DHILIGHT);
			clrOut2 = GetColor(COLOR_3DSHADOW);
		}
		else if( bHover || bFocus )
		{
			clrIn1 = GetColor(COLOR_3DHILIGHT);
			clrIn2 = GetColor(COLOR_3DSHADOW);
		}
	}

	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrIn2, clrIn1);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrOut2, clrOut1);
}

void CTPaintManagerXP::PaintControlFrame( CDC& dc,
										  CRect& rcClient,
										  BOOL bFlat,
										  BOOL bHover,
										  BOOL bEnabled,
										  BOOL bFocus)
{
	CRect rcItem(rcClient);
	COLORREF clrInner = GetColor(COLOR_WINDOW);
	COLORREF clrIn;
	COLORREF clrOut;

	clrIn = clrOut = GetColor(CLR_3DFACE_OUT);
	if(bEnabled)
	{
		if(bHover)
		{
			if(bFocus)
				clrIn = clrOut = GetColor(XPCLR_HILIGHT_BORDER);
			else
				clrIn = GetColor(XPCLR_HILIGHT_BORDER);
		}
		else if(bFocus)
			clrIn = GetColor(XPCLR_HILIGHT_BORDER);
	}

	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrInner, clrInner);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrIn, clrIn);
	rcItem.InflateRect( 1, 1);
	dc.Draw3dRect( rcItem, clrOut, clrOut);
}

void CTPaintManager::PaintComboFrame( CDC &dc,
									  CRect &rcClient,
									  BOOL bHover,
									  BOOL bPushed,
									  BOOL bEnabled)
{
	COLORREF clrBottomRight;
	COLORREF clrTopLeft;

	clrTopLeft = clrBottomRight = GetColor(CLR_3DFACE_OUT);
	if( bPushed || bHover )
	{
		clrBottomRight = GetColor(CLR_3DHILIGHT_IN);
		clrTopLeft = GetColor(CLR_3DSHADOW_IN);
	}

	COLORREF clrHilightOut = GetColor(CLR_3DHILIGHT_OUT);
	COLORREF clrFaceOut = GetColor(CLR_3DFACE_OUT);
	COLORREF clrWindow = GetColor(bEnabled ? COLOR_WINDOW : COLOR_3DFACE);

	dc.Draw3dRect(
		rcClient,
		clrTopLeft,
		clrBottomRight);
	rcClient.DeflateRect( 1, 1);

	dc.Draw3dRect(
		rcClient,
		bEnabled ? clrFaceOut : clrHilightOut,
		bEnabled ? clrFaceOut : clrHilightOut);

	rcClient.left = rcClient.right - ::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rcClient,
		clrFaceOut,
		clrFaceOut);

	rcClient.DeflateRect( 1, 1);
	dc.Draw3dRect(
		rcClient,
		clrHilightOut,
		clrHilightOut);

	dc.FillSolidRect(
		rcClient.left - 2,
		rcClient.top, 3,
		rcClient.Height(),
		bEnabled ? clrWindow : clrFaceOut);

	if( bHover || bPushed )
	{
		dc.FillSolidRect(
			rcClient,
			clrFaceOut);
	}

	rcClient.left++;
	CFont font;

	int ppi = dc.GetDeviceCaps(LOGPIXELSX);
	int pointsize = MulDiv( 60, 96, ppi);

	font.CreatePointFont( pointsize, _T("Marlett"));
	PaintPushButton(
		dc,
		TRUE,
		rcClient,
		_T("6"),
		NULL,
		TRUE,
		bHover && !bPushed,
		bPushed,
		FALSE,
		bEnabled,
		TRUE,
		FALSE,
		FALSE,
		CTPaintManager::__ALIGN_HORIZ_CENTER|CTPaintManager::__ALIGN_VERT,
		&font);

	if( !bHover && !bPushed )
	{
		if(!bHover)
		{
			dc.Draw3dRect(
				rcClient,
				clrHilightOut,
				clrHilightOut);
		}
		else
		{
			dc.Draw3dRect(
				rcClient,
				clrFaceOut,
				clrFaceOut);
		}
	}
}

void CTPaintManager::PaintDotNet3dRect( CDC& dc,
									    LPCRECT lpRect,
										COLORREF clrTopLeft,
										COLORREF clrBottomRight)
{
	PaintDotNet3dRect(
		dc,
		lpRect->left,
		lpRect->top,
		lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top,
		clrTopLeft,
		clrBottomRight);
}

void CTPaintManager::PaintDotNet3dRect( CDC& dc,
									    int x,
										int y,
										int cx,
										int cy,
										COLORREF clrTopLeft,
										COLORREF clrBottomRight)
{
	const int nPixelGap = 1;

	dc.FillSolidRect(
		x + nPixelGap, y,
		cx - 1 - nPixelGap, 1,
		clrTopLeft);

	dc.FillSolidRect(
		x, y + nPixelGap, 1,
		cy - 1 - nPixelGap,
		clrTopLeft);

	dc.FillSolidRect(
		x + cx,
		y + nPixelGap,
		-1, cy - nPixelGap * 2,
		clrBottomRight);

	dc.FillSolidRect(
		x + nPixelGap,
		y + cy,
		cx - nPixelGap * 2,
		-1, clrBottomRight);
}

void CTPaintManager::PaintRadioDot( CDC& dc,
								    int x,
									int y,
									COLORREF color)
{
	CBrush *pOldBrush;
	CBrush brush;

	CPen *pOldPen;
	CPen pen;
	CRect rcDot( x, y, x + 6, y + 6);

	COLORREF color2 = dc.GetNearestColor(RGB(
		255 - GetRValue(color),
		255 - GetGValue(color),
		255 - GetBValue(color)));

	rcDot.OffsetRect( 1, 1);
	brush.CreateSolidBrush(color2);
	pen.CreatePen(PS_SOLID,0,color2);
	pOldBrush = dc.SelectObject(&brush);
	pOldPen = dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	pen.DeleteObject();
	brush.DeleteObject();
	rcDot.OffsetRect( -1, -1);

	brush.CreateSolidBrush(color);
	pen.CreatePen( PS_SOLID, 0, color);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	dc.Ellipse(&rcDot);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
}

void CTPaintManager::PaintCheckMark( CDC& dc,
									 int x,
									 int y,
									 COLORREF color)
{
	COLORREF color2 = dc.GetNearestColor(RGB(
		255 - GetRValue(color),
		255 - GetGValue(color),
		255 - GetBValue(color)));

	dc.SetPixel( x, y + 2, color);
	dc.SetPixel( x, y + 3, color);
	dc.SetPixel( x, y + 4, color2);

	dc.SetPixel( x + 1, y + 3, color);
	dc.SetPixel( x + 1, y + 4, color);
	dc.SetPixel( x + 1, y + 5, color2);

	dc.SetPixel( x + 2, y + 4, color);
	dc.SetPixel( x + 2, y + 5, color);
	dc.SetPixel( x + 2, y + 6, color2);

	dc.SetPixel( x + 3, y + 3, color);
	dc.SetPixel( x + 3, y + 4, color);
	dc.SetPixel( x + 3, y + 5, color2);

	dc.SetPixel( x + 4, y + 2, color);
	dc.SetPixel( x + 4, y + 3, color);
	dc.SetPixel( x + 4, y + 4, color2);

	dc.SetPixel( x + 5, y + 1, color);
	dc.SetPixel( x + 5, y + 2, color);
	dc.SetPixel( x + 5, y + 3, color2);

	dc.SetPixel( x + 6, y, color);
	dc.SetPixel( x + 6, y + 1, color);
	dc.SetPixel( x + 6, y + 2, color2);
}

void CTPaintManager::PaintNcAreaRect( CDC &dc, const CRect &rc)
{
	COLORREF clrTL = GetColor(COLOR_3DSHADOW);
	COLORREF clrBR = GetColor(COLOR_3DHILIGHT);

	dc.Draw3dRect(
		&rc,
		clrTL,
		clrBR);
}

void CTPaintManagerXP::PaintNcAreaRect( CDC &dc, const CRect &rc)
{
	COLORREF clrTL = GetColor(COLOR_3DSHADOW);
	COLORREF clrBR = clrTL;

	dc.Draw3dRect(
		&rc,
		clrTL,
		clrBR);
}

void CTPaintManager::PaintDockBarClientArea( CDC &dc, const CRect &rcClient)
{
	if( rcClient.IsRectEmpty() || !dc.RectVisible(&rcClient) )
		return;

	dc.FillSolidRect(
		&rcClient,
		GetColor(COLOR_3DFACE));
}

void CTPaintManagerXP::PaintDockBarClientArea( CDC &dc, const CRect &rcClient)
{
	CTPaintManager::PaintDockBarClientArea(
		dc,
		rcClient);
}

void CTPaintManager::PaintDockingFrame( CDC &dc,
									    const CRect& rcWindow,
										const CRect& rcClient,
										BOOL bFloating,
										BOOL bTBar,
										BOOL bSideBar)
{
	if(rcWindow.IsRectEmpty())
		return;

	dc.ExcludeClipRect(rcClient);
	CRect rcWnd(rcWindow);

	COLORREF clr3dFace = GetColor(CLR_3DFACE_OUT);
	COLORREF clr3dHilight = GetColor(CLR_3DHILIGHT_OUT);
	COLORREF clr3dShadow = GetColor(CLR_3DSHADOW_OUT);
	COLORREF clr3dDkShadow = GetColor(CLR_3DDKSHADOW_OUT);

	dc.FillSolidRect(
		&rcWnd,
		clr3dFace);

	if(bFloating)
	{
		dc.Draw3dRect(
			&rcWnd,
			clr3dFace,
			clr3dDkShadow);

		rcWnd.DeflateRect( 1, 1);
		dc.Draw3dRect(
			&rcWnd,
			clr3dHilight,
			clr3dShadow);
	}
	else if(!bTBar)
	{
		dc.Draw3dRect(
			&rcWnd,
			clr3dHilight,
			clr3dShadow);
	}

	dc.IntersectClipRect(rcWindow);
}

void CTPaintManagerXP::PaintDockingFrame( CDC &dc,
										  const CRect &rcWindow,
										  const CRect &rcClient,
										  BOOL bFloating,
										  BOOL bTBar,
										  BOOL bSideBar)
{
	if(rcWindow.IsRectEmpty())
		return;

	dc.ExcludeClipRect(rcClient);
	CRect rcWnd(rcWindow);

	COLORREF clr3dFace = GetColor(COLOR_3DFACE);
	COLORREF clrDark = GetColor(COLOR_3DSHADOW);
	COLORREF clrBkOld = dc.GetBkColor();
	COLORREF clrBkNew = GetColor(bTBar && bFloating ? XPCLR_3DFACE_NORMAL : CLR_3DFACE_OUT);

	dc.FillSolidRect( &rcWnd, clrBkNew);
	if(bFloating)
	{
		dc.Draw3dRect(
			&rcWnd,
			clrDark,
			clrDark);

		rcWnd.DeflateRect( 1, 1);
		dc.Draw3dRect(
			&rcWnd,
			clrDark,
			clrDark);

		rcWnd.DeflateRect( 1, 1, 2, 2);
		dc.SetPixel( rcWnd.left, rcWnd.top, clrDark);
		dc.SetPixel( rcWnd.left, rcWnd.bottom, clrDark);
		dc.SetPixel( rcWnd.right, rcWnd.top, clrDark);
		dc.SetPixel( rcWnd.right, rcWnd.bottom, clrDark);
	}
	else
	{
		if(!bTBar)
		{
			dc.Draw3dRect(
				&rcWnd,
				clr3dFace,
				clr3dFace);
		}

		if( !bSideBar && GetBPP() <= 8 )
		{
			static WORD hatchBits[8] = {
				0xAA,
				0x55,
				0xAA,
				0x55,
				0xAA,
				0x55,
				0xAA,
				0x55};

			CBitmap bmp;
			CBrush br;

			bmp.CreateBitmap( 8, 8, 1, 1, hatchBits);
			br.CreatePatternBrush(&bmp);

			LOGBRUSH lbr;
			br.GetLogBrush(&lbr);

			CPen pen( PS_GEOMETRIC, 1, &lbr);
			CPen *pOldPen = dc.SelectObject(&pen);

			dc.MoveTo( rcWnd.left, rcWnd.top);
			dc.LineTo( rcWnd.right - 1, rcWnd.top);
			dc.LineTo( rcWnd.right - 1, rcWnd.bottom - 1);
			dc.LineTo( rcWnd.left, rcWnd.bottom - 1);
			dc.LineTo( rcWnd.left, rcWnd.top);
			dc.SelectObject(pOldPen);
		}
	}

	dc.SetBkColor(clrBkOld);
	dc.SelectClipRgn(NULL);
}

void CTPaintManager::PaintGripper( CDC &dc,
								   const CRect &rcGripper,
								   const CRect &rcText,
								   BOOL bActive,
								   BOOL bFloating,
								   BOOL bHorz,
								   BOOL bSideBar,
								   LPCTSTR strCaption)
{
	COLORREF clrBtnShadow = GetColor(CLR_3DSHADOW_OUT);
	COLORREF clrBtnHilight = GetColor(CLR_3DHILIGHT_OUT);
	CRect rcGripper2(rcGripper);

	if( bSideBar || bFloating )
	{
		if(!bFloating)
			rcGripper2.DeflateRect( 1, 1);

		int nTextColorIndex = COLOR_HIGHLIGHTTEXT;
		if(bSideBar)
		{
			if( bFloating || bActive )
			{
				nTextColorIndex = COLOR_CAPTIONTEXT;

				PaintGradientRect(
					dc,
					rcGripper2,
					GetColor(COLOR_ACTIVECAPTION),
					GetColor(27),
					bHorz);
			}
			else
			{
				nTextColorIndex = COLOR_INACTIVECAPTIONTEXT;

				PaintGradientRect(
					dc,
					rcGripper2,
					GetColor(COLOR_INACTIVECAPTION),
					GetColor(28),
					bHorz);
			}
		}
		else
		{
			dc.FillSolidRect(
				rcGripper2,
				GetColor(COLOR_HIGHLIGHT));
		}

		if( strCaption != NULL &&
			!rcText.IsRectEmpty() &&
			rcText.right > rcText.left &&
			rcText.bottom > rcText.top)
		{
			COLORREF clrText = GetColor(nTextColorIndex);
			COLORREF clrOldText = dc.SetTextColor(clrText);

			int nOldBkMode = dc.SetBkMode(TRANSPARENT);
			CFont *pCurrFont = !bHorz ? &m_FontBold : &m_FontBoldVertX;
			CFont *pOldFont = dc.SelectObject(pCurrFont);

			if(!bHorz)
			{
				dc.DrawText(
					strCaption,
					(LPRECT) &rcText,
					DT_SINGLELINE|
					DT_LEFT|
					DT_VCENTER|
					DT_END_ELLIPSIS);
			}
			else
			{
				LOGFONT lf;
				::memset( &lf, 0, sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);

				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin = (rcText.Width() - _cyHorzFont)  / 2;

				CRect rcString = CRect(CPoint(
					rcText.left + _cyTextMargin,
					rcText.bottom - CXTEXTMARGIN),
					rcText.Size());
				rcString.DeflateRect( 2, 2);

				CRect rcExclude;
				rcExclude.SetRect(
					rcGripper.left,
					rcGripper.top,
					rcGripper.right,
					rcText.top);
				dc.ExcludeClipRect(&rcExclude);

				dc.DrawText(
					strCaption,
					rcString,
					DT_SINGLELINE|
					DT_NOCLIP|
					DT_NOPREFIX);
				dc.SelectClipRgn(NULL);
			}

			dc.SelectObject(pOldFont);
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	}
	else if(bHorz)
	{
		rcGripper2.DeflateRect( 4, 1);
		rcGripper2.right = rcGripper2.left + 3;

		dc.Draw3dRect(
			rcGripper2,
			clrBtnHilight,
			clrBtnShadow);

		rcGripper2.OffsetRect( 3, 0);
		dc.Draw3dRect(
			rcGripper2,
			clrBtnHilight,
			clrBtnShadow);
	}
	else
	{
		rcGripper2.OffsetRect( 1, 0);
		rcGripper2.DeflateRect( 1, 4);

		rcGripper2.bottom = rcGripper2.top + 3;
		dc.Draw3dRect(
			rcGripper2,
			clrBtnHilight,
			clrBtnShadow);

		rcGripper2.OffsetRect( 0, 3);
		dc.Draw3dRect(
			rcGripper2,
			clrBtnHilight,
			clrBtnShadow);
	}
}

void CTPaintManagerXP::PaintGripper( CDC &dc,
									 const CRect &rcGripper,
									 const CRect &rcText,
									 BOOL bActive,
									 BOOL bFloating,
									 BOOL bHorz,
									 BOOL bSideBar,
									 LPCTSTR strCaption)
{
	CRect rcGripper2(rcGripper);

	if( bSideBar || bFloating )
	{
		COLORREF clrGrip = GetColor( bSideBar ? bActive ? COLOR_HIGHLIGHT : XPCLR_3DFACE_DARK : COLOR_3DSHADOW);
		dc.FillSolidRect( rcGripper2, clrGrip);

		if( bSideBar && !bFloating && !bActive )
		{
			COLORREF clrSideRect = GetColor(COLOR_3DSHADOW);

			PaintDotNet3dRect(
				dc,
				rcGripper2,
				clrSideRect,
				clrSideRect);
		}

		if( strCaption != NULL &&
			!rcText.IsRectEmpty() &&
			rcText.right > rcText.left &&
			rcText.bottom > rcText.top )
		{
			COLORREF clrText = GetColor( bSideBar ? bActive ? COLOR_HIGHLIGHTTEXT : COLOR_BTNTEXT : COLOR_HIGHLIGHTTEXT);
			COLORREF clrOldText = dc.SetTextColor(clrText);
			int nOldBkMode = dc.SetBkMode(TRANSPARENT);

			CFont *pCurrFont = !bHorz ? bSideBar ? &m_FontNormal : &m_FontBold : bSideBar ? &m_FontNormalVertX : &m_FontBoldVertX;
			CFont *pOldFont = dc.SelectObject(pCurrFont);

			if(!bHorz)
			{
				dc.DrawText(
					strCaption,
					(LPRECT) &rcText,
					DT_SINGLELINE|
					DT_LEFT|
					DT_VCENTER|
					DT_END_ELLIPSIS);
			}
			else
			{
				LOGFONT lf;
				::memset( &lf, 0, sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);

				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin = (rcText.Width() - _cyHorzFont) / 2;

				CRect rcString = CRect(CPoint(
					rcText.left + _cyTextMargin,
					rcText.bottom - CXTEXTMARGIN),
					rcText.Size());
				rcString.DeflateRect( 2, 2);

				CRect rcExclude;
				rcExclude.SetRect(
					rcGripper.left,
					rcGripper.top,
					rcGripper.right,
					rcText.top);
				dc.ExcludeClipRect(&rcExclude);

				dc.DrawText(
					strCaption,
					rcString,
					DT_SINGLELINE|
					DT_NOCLIP|
					DT_NOPREFIX);
				dc.SelectClipRgn(NULL);
			}

			dc.SelectObject(pOldFont);
			dc.SetBkMode(nOldBkMode);
			dc.SetTextColor(clrOldText);
		}
	}
	else
	{
		COLORREF clrGrip = GetColor(CLR_3DFACE_OUT);
		CRect rcGripToClear(rcGripper);

		rcGripToClear.DeflateRect( 1, 1);
		dc.FillSolidRect(
			rcGripToClear,
			clrGrip);

		int nBitsPerPixel = GetBPP();
		static WORD horzHatchBits[8] = {
			0xFF, 0x00, 0xFF, 0x00,
			0xFF, 0x00, 0xFF, 0x00};

		static WORD vertHatchBits[8] = {
			0xAA, 0xAA, 0xAA, 0xAA,
			0xAA, 0xAA, 0xAA, 0xAA};

		if(bHorz)
		{
			CBitmap bmpGripperHorz;
			CBrush brGripperHorz;

			if( nBitsPerPixel <= 8 )
				bmpGripperHorz.CreateBitmap( 8, 8, 1, 1, horzHatchBits);
			else
			{
				CDC mdc;

				mdc.CreateCompatibleDC(&dc);
				bmpGripperHorz.CreateCompatibleBitmap( &dc, 8, 8);

				CBitmap *pOldBmp = mdc.SelectObject(&bmpGripperHorz);
				mdc.FillSolidRect( 0, 0, 8, 8, clrGrip);

				COLORREF clr1 = GetColor(COLOR_3DFACE);
				COLORREF clr2 = GetColor(COLOR_3DSHADOW);
				COLORREF clrGripLines = RGB(
					(GetRValue(clr1) * 3 + GetRValue(clr2)) / 4,
					(GetGValue(clr1) * 3 + GetGValue(clr2)) / 4,
					(GetBValue(clr1) * 3 + GetBValue(clr2)) / 4);

				CPen pen( PS_SOLID, 1, clrGripLines);
				CPen *pOldPen = mdc.SelectObject(&pen);

				for( int i=0; i<8; i+=2)
				{
					mdc.MoveTo( 0, i);
					mdc.LineTo( 8, i);
				}

				mdc.SelectObject(pOldPen);
				mdc.SelectObject(pOldBmp);
			}
			brGripperHorz.CreatePatternBrush(&bmpGripperHorz);

			rcGripper2.DeflateRect( 5, 4);
			rcGripper2.right = rcGripper2.left + 5;
			rcGripper2.DeflateRect( 1, 0);

			COLORREF clrOldBk = dc.SetBkColor(GetColor(XPCLR_3DFACE_NORMAL));
			CBrush *pBrushOld = dc.SelectObject(&brGripperHorz);

			dc.PatBlt(
				rcGripper2.left, rcGripper2.top,
				rcGripper2.Width(), rcGripper2.Height(),
				0xA000C9);

			dc.SelectObject(pBrushOld);
			dc.SetBkColor(clrOldBk);
		}
		else
		{
			CBitmap bmpGripperVert;
			CBrush brGripperVert;

			if( nBitsPerPixel <= 8 )
				bmpGripperVert.CreateBitmap( 8, 8, 1, 1, vertHatchBits);
			else
			{
				CDC mdc;

				mdc.CreateCompatibleDC(&dc);
				bmpGripperVert.CreateCompatibleBitmap( &dc, 8, 8);

				CBitmap *pOldBmp = mdc.SelectObject(&bmpGripperVert);
				mdc.FillSolidRect( 0, 0, 8, 8, clrGrip);

				COLORREF clr1 = GetColor(COLOR_3DFACE);
				COLORREF clr2 = GetColor(COLOR_3DSHADOW);
				COLORREF clrGripLines = RGB(
					(GetRValue(clr1) * 3 + GetRValue(clr2)) / 4,
					(GetGValue(clr1) * 3 + GetGValue(clr2)) / 4,
					(GetBValue(clr1) * 3 + GetBValue(clr2)) / 4);

				CPen pen( PS_SOLID, 1, clrGripLines);
				CPen *pOldPen = mdc.SelectObject(&pen);

				for( int i=0; i<8; i+=2)
				{
					mdc.MoveTo( i, 0);
					mdc.LineTo( i, 8);
				}

				mdc.SelectObject(pOldPen);
				mdc.SelectObject(pOldBmp);
			}
			brGripperVert.CreatePatternBrush(&bmpGripperVert);

			rcGripper2.OffsetRect( 1, 0);
			rcGripper2.DeflateRect( 4, 5);
			rcGripper2.bottom = rcGripper2.top + 5;
			rcGripper2.DeflateRect( 0, 1);

			COLORREF clrOldBk = dc.SetBkColor(GetColor(XPCLR_3DFACE_NORMAL));
			CBrush *pBrushOld = dc.SelectObject(&brGripperVert);

			dc.PatBlt(
				rcGripper2.left,
				rcGripper2.top,
				rcGripper2.Width(),
				rcGripper2.Height(),
				0xA000C9);

			dc.SelectObject(pBrushOld);
			dc.SetBkColor(clrOldBk);
		}
	}
}

CSize CTPaintManagerXP::GetPushedOffset()
{
	return CSize( 0, 0);
}

#define __XP_DSS_MONO DSS_MONO
void CTPaintManagerXP::InitTranslatedColors()
{
	m_DSS_DISABLED_style = __XP_DSS_MONO;
	CTPaintManager::InitTranslatedColors();

	int nBitsPerPixel = CTPaintManager::GetBPP();
	COLORREF clrSysWindow = ::GetSysColor(COLOR_WINDOW);
	COLORREF clrSys3dFace = ::GetSysColor(COLOR_3DFACE);
	COLORREF clrSys3dHilight = ::GetSysColor(COLOR_3DHILIGHT);
	COLORREF clrSys3dShadow = ::GetSysColor(COLOR_3DSHADOW);
	COLORREF clrSys3dDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
	COLORREF clrSysBtnText = ::GetSysColor(COLOR_BTNTEXT);
	COLORREF clrSysItemHilight = ::GetSysColor(COLOR_HIGHLIGHT);

	COLORREF xpclr_MenuLight = clrSysWindow;
	COLORREF xpclr_ControlBarBk = clrSys3dFace;
	COLORREF xpclr_Highlight = clrSys3dFace;
	COLORREF xpclr_Separator = clrSys3dShadow;
	COLORREF xpclr_PanelBorder = clrSys3dShadow;
	COLORREF xpclr_RarelyUsedMenuLeft = clrSys3dShadow;
	COLORREF xpclr_HighlightDarked = clrSys3dShadow;
	COLORREF xpclr_HighlightBorder = clrSysBtnText;

	if( nBitsPerPixel > 8 )
	{
		xpclr_MenuLight = HLS_Adjust(
			clrSys3dFace,
			0.0f, 0.7f, -0.6f);

		COLORREF clrMixWindowFace = RGB(
			(750L * long(GetRValue(clrSys3dFace)) + 250L * long(GetRValue(clrSysWindow))) / 1000L,
			(750L * long(GetGValue(clrSys3dFace)) + 250L * long(GetGValue(clrSysWindow))) / 1000L,
			(750L * long(GetBValue(clrSys3dFace)) + 250L * long(GetBValue(clrSysWindow))) / 1000L);

		xpclr_RarelyUsedMenuLeft = RGB(
			(999L * long(GetRValue(clrSys3dFace))) / 1000L,
			(995L * long(GetGValue(clrSys3dFace))) / 1000L,
			(995L * long(GetBValue(clrSys3dFace))) / 1000L);

		xpclr_ControlBarBk = RGB(
			(970L * long(GetRValue(clrMixWindowFace))) / 1000L,
			(970L * long(GetGValue(clrMixWindowFace))) / 1000L,
			(970L * long(GetBValue(clrMixWindowFace))) / 1000L);

		xpclr_Highlight = HLS_Adjust(
			clrSysItemHilight,
			0.0f, 0.75f, -0.75f);

		xpclr_HighlightDarked = HLS_Adjust(
			xpclr_Highlight,
			0.0f, -0.25f, 0.1f);

		xpclr_HighlightBorder = HLS_Adjust(
			xpclr_HighlightDarked,
			0.0f, -0.5f, 0.0f);

		xpclr_Separator = RGB(
			(857L * long(GetRValue(clrSys3dFace))) / 1000L,
			(857L * long(GetGValue(clrSys3dFace))) / 1000L,
			(857L * long(GetBValue(clrSys3dFace))) / 1000L);

		xpclr_PanelBorder = RGB(
			(750L * long(GetRValue(clrSys3dShadow)) + 250L * long(GetRValue(clrSys3dDkShadow))) / 1000L,
			(750L * long(GetGValue(clrSys3dShadow)) + 250L * long(GetGValue(clrSys3dDkShadow))) / 1000L,
			(750L * long(GetBValue(clrSys3dShadow)) + 250L * long(GetBValue(clrSys3dDkShadow))) / 1000L);
	}

	m_mapColorTranslate[CLR_3DFACE_OUT] = InstallColor(xpclr_ControlBarBk);
	m_mapColorTranslate[CLR_3DFACE_IN] = InstallColor(xpclr_Highlight);
	m_mapColorTranslate[CLR_3DFACE_DISABLED] = COLOR_3DFACE;

	int idxClrDark = InstallColor(xpclr_PanelBorder);
	m_mapColorTranslate[CLR_3DLIGHT_OUT] = idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_IN] = idxClrDark;
	m_mapColorTranslate[CLR_3DLIGHT_DISABLED] = idxClrDark;

	m_mapColorTranslate[CLR_3DHILIGHT_OUT] = idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_IN] = idxClrDark;
	m_mapColorTranslate[CLR_3DHILIGHT_DISABLED] = idxClrDark;

	m_mapColorTranslate[CLR_3DSHADOW_OUT] = idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_IN] = idxClrDark;
	m_mapColorTranslate[CLR_3DSHADOW_DISABLED] = idxClrDark;

	m_mapColorTranslate[CLR_3DDKSHADOW_OUT] = idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_IN] = idxClrDark;
	m_mapColorTranslate[CLR_3DDKSHADOW_DISABLED] = idxClrDark;

	m_mapColorTranslate[CLR_TEXT_OUT] = COLOR_BTNTEXT;
	m_mapColorTranslate[CLR_TEXT_IN] = InstallColor( nBitsPerPixel > 8 ? RGB( 0, 0, 0) : clrSys3dHilight);
	m_mapColorTranslate[CLR_TEXT_DISABLED] = COLOR_3DSHADOW;

	m_mapColorTranslate[XPCLR_3DFACE_DARK] = InstallColor(xpclr_ControlBarBk);
	m_mapColorTranslate[XPCLR_3DFACE_NORMAL] = InstallColor(xpclr_MenuLight);
	m_mapColorTranslate[XPCLR_SEPARATOR] = InstallColor(xpclr_Separator);

	m_mapColorTranslate[XPCLR_HILIGHT] = InstallColor(xpclr_HighlightDarked);
	m_mapColorTranslate[XPCLR_HILIGHT_BORDER] = InstallColor(xpclr_HighlightBorder);
	m_mapColorTranslate[XPCLR_RARELY_BORDER] = InstallColor(xpclr_RarelyUsedMenuLeft);
}

void CTPaintManagerXP::PaintPushButton( CDC &dc,
									    BOOL bHorz,
										const CRect &rcClient,
										LPCTSTR strText,
										HICON hIcon,
										BOOL bFlat,
										BOOL bHover,
										BOOL bPushed,
										BOOL bIndeterminate,
										BOOL bEnabled,
										BOOL bDrawBorder,
										BOOL bDrawFocusRect,
										BOOL bDefaultPushButton,
										int nAlign,
										CFont *pFont,
										BOOL bDropDown,
										UINT nHelperSysCmdID,
										BOOL bTransparentBackground)
{
	if(rcClient.IsRectEmpty())
		return;

	if(!bEnabled)
	{
		bHover = FALSE;
		bDrawFocusRect = FALSE;
		bIndeterminate = FALSE;
	}

	COLORREF clrOldText = dc.GetTextColor();
	COLORREF clrOldBk = dc.GetBkColor();
	int nOldBkMode = dc.SetBkMode(TRANSPARENT);

	COLORREF bgColor;
	if( bHover || bPushed )
	{
		if(bPushed)
			bgColor = GetColor( bHover ? XPCLR_HILIGHT : CLR_3DFACE_IN);
		else
			bgColor = GetColor(CLR_3DFACE_IN);
	}
	else
		bgColor = GetColor(CLR_3DFACE_OUT);

	if(!bTransparentBackground)
	{
		CBrush br(bgColor);
		dc.FillRect( &rcClient, &br);
	}

	COLORREF clrDkBorder = GetColor(XPCLR_HILIGHT_BORDER);
	if( bHover || bPushed )
	{
		dc.Draw3dRect(
			&rcClient,
			clrDkBorder,
			clrDkBorder);
	}

	CString strTitle(_T(""));
	if( strText != NULL )
		strTitle = strText;

	CRect rcClient2(rcClient);
	if(bDropDown)
	{
		CRect rcDopArea(rcClient2);
		COLORREF ColorValues[2] = {
			RGB( 0, 0, 0),
			GetColor( bEnabled ? bHover ? CLR_TEXT_IN : CLR_TEXT_OUT : CLR_3DSHADOW_OUT)};

		if(bHorz)
		{
			rcClient2.right -= __DROPDOWN_ARROW_GAP * 2 + m_wingl_btn_expand_bottom.Size().cx;
			rcDopArea.left = rcClient2.right;
			rcDopArea.DeflateRect( 1, 1);
			rcDopArea.OffsetRect( -1, 0);

			if(bPushed)
				rcDopArea.OffsetRect(GetPushedOffset());

			PaintWinglCentered(
				dc,
				rcDopArea,
				m_wingl_btn_expand_bottom,
				ColorValues);
		}
		else
		{
			rcClient2.top += __DROPDOWN_ARROW_GAP * 2 + m_wingl_btn_expand_right.Size().cy;
			rcDopArea.bottom = rcClient2.top;

			rcDopArea.DeflateRect( 1, 1);
			rcDopArea.OffsetRect( 0, 1);

			if(bPushed)
				rcDopArea.OffsetRect(GetPushedOffset());

			PaintWinglCentered(
				dc,
				rcDopArea,
				m_wingl_btn_expand_right,
				ColorValues);
		}
	}

	CRect rcCaption = rcClient2;
	if( rcClient2.bottom > rcClient2.top &&
		rcClient2.right > rcClient2.left)
	{
		if( hIcon != NULL )
		{
			PaintIcon(
				dc,
				bHorz,
				strTitle,
				hIcon,
				rcClient2,
				rcCaption,
				bPushed,
				bEnabled,
				bHover,
				nAlign);
		}
		else
		{
			PaintSysCmdWINGL(
				dc,
				nHelperSysCmdID,
				rcClient2,
				FALSE,
				bPushed,
				bEnabled);
		}
	}

	if( rcCaption.bottom > rcCaption.top &&
		rcCaption.right > rcCaption.left)
	{
		if(!strTitle.IsEmpty())
		{
			if(bPushed)
			{
				CSize szPushedOffset = GetPushedOffset();
				rcCaption.OffsetRect(szPushedOffset);
			}
			dc.SetBkColor(bgColor);

			if(bEnabled)
				dc.SetTextColor(GetColor( bHover || bPushed ? bHover && bPushed ? COLOR_HIGHLIGHTTEXT : CLR_TEXT_IN : CLR_TEXT_OUT));
			else
				dc.SetTextColor(GetColor(COLOR_3DSHADOW));

			CFont *pOldBtnFont = NULL;
			CFont *pCurrFont = NULL;
			CFont fontDummy;

			if( pFont != NULL )
			{
				if(bHorz)
					pCurrFont = pFont;
				else
				{
					LOGFONT lf;
					::memset( &lf, 0, sizeof(LOGFONT));
					pFont->GetLogFont(&lf);

					if( lf.lfEscapement == 0 )
					{
						lf.lfEscapement = __VERT_FONT_ESCAPEMENT__;
						fontDummy.CreateFontIndirect(&lf);
						pCurrFont = &fontDummy;
					}
					else
						pCurrFont = pFont;
				}
			}
			else if(bHorz)
			{
				if(bDefaultPushButton)
					pCurrFont = &m_FontBold;
				else
					pCurrFont = &m_FontNormal;
			}
			else if(bDefaultPushButton)
				pCurrFont = &m_FontBoldVert;
			else
				pCurrFont = &m_FontNormalVert;
			pOldBtnFont = dc.SelectObject(pCurrFont);

			int nTextLength = strTitle.GetLength();
			int nAmpIndex = strTitle.Find(_T('&'));

			CString strBtn;
			if( nAmpIndex < 0 )
				strBtn = strTitle;
			else
				strBtn = strTitle.Left(nAmpIndex) + strTitle.Right(nTextLength - (nAmpIndex + 1));

			CRect rcText( 0, 0, 0, 0);
			dc.DrawText(
				strBtn,
				-1, rcText,
				DT_SINGLELINE|
				DT_CALCRECT);

			CRect rcCenteredCaption(rcCaption);
			rcCenteredCaption.OffsetRect(
				(rcCaption.Width() - rcText.Width()) / 2,
				(rcCaption.Height() - rcText.Height()) / 2);

			if(bHorz)
			{
				dc.DrawText(
					strTitle, -1,
					rcCaption,
					DT_SINGLELINE|
					DT_CENTER|
					DT_VCENTER);
			}
			else
			{
				LOGFONT lf;
				::memset( &lf, 0, sizeof(LOGFONT));
				pCurrFont->GetLogFont(&lf);

				int _cyHorzFont = abs(lf.lfHeight);
				int _cyTextMargin = (rcCaption.Width() - _cyHorzFont) / 2;

				CPoint ptLineFrom( 0, 0);
				CPoint ptLineTo( 0, 0);

				if( nAmpIndex >= 0 )
				{
					ptLineFrom = CPoint(
						_cyTextMargin,
						CXTEXTMARGIN + CalcTextWidth(
						dc,
						bDefaultPushButton ? m_FontBold : m_FontNormal,
						strBtn.Left(nAmpIndex)));

					ptLineTo = CPoint(
						_cyTextMargin,
						CXTEXTMARGIN + CalcTextWidth(
						dc,
						bDefaultPushButton ? m_FontBold : m_FontNormal,
						strBtn.Left(nAmpIndex + 1)));
				}

				CRect rcString = CRect(CPoint(
					rcCaption.right - _cyTextMargin,
					rcCaption.top + CXTEXTMARGIN),
					rcCaption.Size());
				rcString.DeflateRect( 2, 2);

				CSize ptFix( -rcString.Width() - 1, 0);
				ptLineFrom += rcString.TopLeft() + ptFix;
				ptLineTo += rcString.TopLeft() + ptFix;

				if(bEnabled)
				{
					dc.DrawText(
						strBtn,
						rcString,
						DT_SINGLELINE|
						DT_NOCLIP|
						DT_NOPREFIX);

					if( nAmpIndex >= 0 )
					{
						CPen pen( PS_SOLID, 0, dc.GetTextColor());
						CPen *pOldPen = dc.SelectObject(&pen);

						dc.MoveTo(ptLineFrom);
						dc.LineTo(ptLineTo);
						dc.SelectObject(pOldPen);
					}
				}
				else
				{
					dc.DrawText(
						strBtn,
						rcString,
						DT_SINGLELINE|
						DT_NOCLIP|
						DT_NOPREFIX);

					if( nAmpIndex >= 0 )
					{
						CPen pen( PS_SOLID, 0, dc.GetTextColor());
						CPen *pOldPen = dc.SelectObject(&pen);

						dc.MoveTo(ptLineFrom);
						dc.LineTo(ptLineTo);
						dc.SelectObject(pOldPen);
					}
				}
			}

			dc.SelectObject(pOldBtnFont);
		}

		if(bDrawFocusRect)
		{
			CRect focusRect = rcCaption;
			focusRect.DeflateRect( 3, 3);
			dc.DrawFocusRect(&focusRect);
		}
	}

	dc.SetBkMode(nOldBkMode);
	dc.SetBkColor(clrOldBk);
	dc.SetTextColor(clrOldText);
}

void CTPaintManagerXP::PaintIcon( CDC &dc,
								  BOOL bHorz,
								  CString &strTitle,
								  HICON hIcon,
								  const CRect &rcItem,
								  CRect &rcCaption,
								  BOOL bPushed,
								  BOOL bEnabled,
								  BOOL bHover,
								  int nAlign)
{
	rcCaption = rcItem;

	if( hIcon == NULL )
		return;

	ICONINFO ii;
	ZeroMemory( &ii, sizeof(ICONINFO));
	::GetIconInfo( hIcon, &ii);

	BYTE cxIcon = (BYTE) ii.xHotspot * 2;
	BYTE cyIcon = (BYTE) ii.yHotspot * 2;

	::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	CRect iconRect = rcItem;
	CRect btnRect;

	if(bHorz)
	{
		if(nAlign&__ALIGN_HORIZ_CENTER)
		{
			if(strTitle.IsEmpty())
				iconRect.left += (iconRect.Width() - cxIcon) / 2;
			else
			{
				iconRect.left += 3;
				rcCaption.left += cxIcon + 3;
			}
		}
		else if(nAlign&__ALIGN_HORIZ_RIGHT)
		{
			btnRect = rcCaption;

			if(strTitle.IsEmpty())
				iconRect.left += (iconRect.Width() - cxIcon) / 2;
			else
			{
				rcCaption.right = rcCaption.Width() - cxIcon - 3;
				rcCaption.left = 3;
				iconRect.left = btnRect.right - cxIcon - 3;
				iconRect.top += (iconRect.Height() - cyIcon) / 2;
			}
		}

		if(nAlign&__ALIGN_VERT)
			iconRect.top += (iconRect.Height() - cyIcon) / 2;
	}
	else
	{
		if(nAlign&__ALIGN_HORIZ_CENTER)
		{
			if(strTitle.IsEmpty())
				iconRect.top += (iconRect.Height() - cyIcon) / 2;
			else
			{
				iconRect.top += 3;
				rcCaption.top += cyIcon + 3;
			}
		}
		else
		{
			if(nAlign&__ALIGN_HORIZ_RIGHT)
			{
				btnRect = rcCaption;

				if(strTitle.IsEmpty())
					iconRect.top += (iconRect.Height() - cyIcon) / 2;
				else
				{
					rcCaption.top = rcCaption.Height() - cyIcon - 3;
					rcCaption.top = 3;
					iconRect.top = btnRect.top - cyIcon - 3;
					iconRect.left += (iconRect.Width() - cxIcon) / 2;
				}
			}
		}

		if(nAlign&__ALIGN_VERT)
			iconRect.left += (iconRect.Width() - cxIcon) / 2;
	}

	if(bPushed)
	{
		CSize szPushedOffset = GetPushedOffset();
		iconRect.OffsetRect( szPushedOffset.cx, szPushedOffset.cy);
	}

	CPoint ptTopLeft = iconRect.TopLeft() + CSize( 1, 1);
	if(bEnabled)
	{
		CPoint ptOffs( 0, 0);

		if( bHover && !bPushed )
		{
			CBrush brBack;

			if(!brBack.CreateSolidBrush(GetColor(COLOR_3DDKSHADOW)))
				return;

			dc.DrawState(
				ptTopLeft,
				iconRect.Size(),
				hIcon,
				__XP_DSS_MONO,
				&brBack);

			ptOffs.x = ptOffs.y = -1;
		}

		dc.DrawState(
			ptTopLeft + ptOffs,
			iconRect.Size(),
			hIcon,
			bEnabled ? DSS_NORMAL : m_DSS_DISABLED_style,
			(CBrush *) NULL);
	}
	else
	{
		CBrush brDisabled;

		if(!brDisabled.CreateSolidBrush(GetColor(XPCLR_SEPARATOR)))
			return;

		dc.DrawState(
			ptTopLeft,
			iconRect.Size(),
			hIcon,
			__XP_DSS_MONO,
			&brDisabled);
	}
}

void CTPaintManagerXP::PaintComboFrame( CDC &dc,
									    CRect &rcClient,
										BOOL bHover,
										BOOL bPushed,
										BOOL bEnabled)
{
	CRect rcClient2(rcClient);
	int nSavedDC = dc.SaveDC();

	COLORREF clrWindow = GetColor(bEnabled ? COLOR_WINDOW : COLOR_3DFACE);
	COLORREF clrFaceOut = GetColor(CLR_3DFACE_OUT);
	COLORREF clrHilight = GetColor(XPCLR_3DFACE_NORMAL);
	COLORREF clrShadow = GetColor(CLR_3DFACE_OUT);

	dc.Draw3dRect(
		rcClient,
		clrFaceOut,
		clrFaceOut);

	rcClient.DeflateRect( 1, 1);
	dc.Draw3dRect(
		rcClient,
		clrHilight,
		clrHilight);

	rcClient.left = rcClient.right - ::GetSystemMetrics(SM_CXHTHUMB);
	dc.Draw3dRect(
		rcClient,
		clrHilight,
		clrHilight);

	rcClient.DeflateRect( 1, 1);
	dc.Draw3dRect(
		rcClient,
		clrHilight,
		clrHilight);

	dc.FillSolidRect(
		rcClient.left - 2,
		rcClient.top, 3,
		rcClient.Height(),
		clrWindow);

	if( bHover || bPushed )
	{
		dc.FillSolidRect(
			rcClient,
			clrHilight);
	}

	rcClient.InflateRect( 1, 1);
	CFont font;

	int ppi = dc.GetDeviceCaps(LOGPIXELSX);
	int pointsize = MulDiv( 60, 96, ppi);
	font.CreatePointFont( pointsize, _T("Marlett"));

	PaintPushButton(
		dc,
		TRUE,
		rcClient,
		_T("6"),
		NULL,
		TRUE,
		bHover || bPushed,
		bPushed,
		FALSE,
		bEnabled,
		TRUE,
		FALSE,
		FALSE,
		CTPaintManager::__ALIGN_HORIZ_CENTER|
		CTPaintManager::__ALIGN_VERT,
		&font);

	if( !bHover && !bPushed )
	{
		dc.Draw3dRect(
			rcClient,
			clrHilight,
			clrHilight);
	}

	if( bHover || bPushed )
	{
		rcClient2.DeflateRect( 1, 1);
		HBRUSH hOldBrush = (HBRUSH) ::SelectObject(
			dc.GetSafeHdc(),
			::GetStockObject(NULL_BRUSH));

		COLORREF clrOld = dc.SetTextColor(clrShadow);
		dc.Rectangle(&rcClient2);
		dc.SetTextColor(clrOld);

		if(hOldBrush)
			::SelectObject( dc.GetSafeHdc(), hOldBrush);
	}

	dc.RestoreDC(nSavedDC);
}

int CTPaintManager::GetMenuBorderSize()
{
	return 4;
}

void CTPaintManager::PaintMenuBorder( CDC &dc, const CRect &rcClient)
{
	if(rcClient.IsRectEmpty())
		return;

	CRect rect(rcClient);
	dc.Draw3dRect(
		&rect,
		GetColor(CLR_3DFACE_OUT),
		GetColor(CLR_3DDKSHADOW_OUT));

	rect.DeflateRect( 1, 1);
	dc.Draw3dRect(
		&rect,
		GetColor(CLR_3DHILIGHT_OUT),
		GetColor(CLR_3DSHADOW_OUT));

	COLORREF clrFace = GetColor(CLR_3DFACE_OUT);
	rect.DeflateRect( 1, 1);

	dc.Draw3dRect( &rect, clrFace, clrFace);
	rect.DeflateRect( 1, 1);
	dc.Draw3dRect( &rect, clrFace, clrFace);
}

int CTPaintManager::GetMenuShadowSize()
{
	return 5;
}

int CTPaintManagerXP::GetMenuShadowSize()
{
	return 5;
}

int CTPaintManagerXP::GetMenuBorderSize()
{
	return 2;
}

void CTPaintManagerXP::PaintMenuBorder( CDC &dc, const CRect &rcClient)
{
	if(rcClient.IsRectEmpty())
		return;

	CRect rect(rcClient);
	COLORREF clr = GetColor(COLOR_3DDKSHADOW);
	dc.Draw3dRect( &rect, clr, clr);

	COLORREF clrFace = GetColor(XPCLR_3DFACE_NORMAL);
	rect.DeflateRect( 1, 1);
	dc.Draw3dRect( &rect, clrFace, clrFace);

	rect.right--;
	rect.bottom--;
	dc.Draw3dRect( &rect, clrFace, clrFace);
}

BOOL CTPaintManager::IsMenuMustCombineExcludeArea()
{
	return FALSE;
}

BOOL CTPaintManagerXP::IsMenuMustCombineExcludeArea()
{
	return TRUE;
}

void CTPaintManager::PaintMenuCombinedArea( CDC &dc,
										    const CRect &rcArea,
											const CRect &rcClient,
											int nCombineAlign)
{
	if(rcArea.IsRectEmpty())
		return;
	dc.SelectClipRgn(NULL);

	int nExpandSize = GetMenuBorderSize() - 1;
	CRect _rcClient(rcClient);
	CRect _rcArea(rcArea);

	_rcArea.InflateRect(
		nCombineAlign == CTPopupMenuWnd::__CMBA_RIGHT ? nExpandSize : 0,
		nCombineAlign == CTPopupMenuWnd::__CMBA_BOTTOM ? nExpandSize : 0,
		nCombineAlign == CTPopupMenuWnd::__CMBA_LEFT ? nExpandSize : 0,
		nCombineAlign == CTPopupMenuWnd::__CMBA_TOP ? nExpandSize : 0);

	dc.FillSolidRect(
		&_rcArea,
		GetColor(CLR_3DFACE_OUT));

	COLORREF clrMenuBorderLTo = GetColor(CLR_3DFACE_OUT);
	COLORREF clrMenuBorderLTi = GetColor(CLR_3DHILIGHT_OUT);
	COLORREF clrMenuBorderRBo = GetColor(CLR_3DSHADOW_OUT);
	COLORREF clrMenuBorderRBi = GetColor(CLR_3DDKSHADOW_OUT);

	switch(nCombineAlign)
	{
	case CTPopupMenuWnd::__CMBA_TOP			:
		{
			_rcArea.InflateRect( 0, 0, 0, nExpandSize);
			_rcClient.InflateRect( -nExpandSize, 0);
		}

		break;

	case CTPopupMenuWnd::__CMBA_BOTTOM		:
		{
			_rcArea.InflateRect( 0, nExpandSize, 0, 0);
			_rcClient.InflateRect( -nExpandSize, 0);
		}

		break;

	case CTPopupMenuWnd::__CMBA_LEFT		:
		{
			_rcArea.InflateRect( 0, 0, nExpandSize, 0);
			_rcClient.InflateRect( 0, -nExpandSize);
		}

		break;

	case CTPopupMenuWnd::__CMBA_RIGHT		:
		{
			_rcArea.InflateRect( nExpandSize, 0, 0, 0);
			_rcClient.InflateRect( 0, -nExpandSize);
		}

		break;
	}
	dc.ExcludeClipRect(&_rcClient);

	int cx = _rcArea.Width();
	int cy = _rcArea.Height();

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect( _rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTo);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect( _rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTo);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect( _rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBo);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect( _rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBo);

	_rcArea.DeflateRect( 1, 1, 1, 1);
	cx = _rcArea.Width();
	cy = _rcArea.Height();

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_BOTTOM )
		dc.FillSolidRect( _rcArea.left, _rcArea.top, cx - 1, 1, clrMenuBorderLTi);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_RIGHT )
		dc.FillSolidRect( _rcArea.left, _rcArea.top, 1, cy - 1, clrMenuBorderLTi);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_LEFT )
		dc.FillSolidRect( _rcArea.left + cx, _rcArea.top, -1, cy, clrMenuBorderRBi);

	if( nCombineAlign != CTPopupMenuWnd::__CMBA_TOP )
		dc.FillSolidRect(_rcArea.left, _rcArea.top + cy, cx, -1, clrMenuBorderRBi);

	dc.SelectClipRgn(NULL);
}

void CTPaintManagerXP::PaintMenuCombinedArea( CDC &dc,
											  const CRect &rcArea,
											  const CRect &rcClient,
											  int nCombineAlign)
{
	if(rcArea.IsRectEmpty())
		return;

	CRect _rcArea(rcArea);
	CRect _rcClient(rcClient);

	dc.FillSolidRect(
		&_rcArea,
		GetColor(XPCLR_3DFACE_DARK));

	COLORREF clrMenuBorder = GetColor(COLOR_3DDKSHADOW);
	switch(nCombineAlign)
	{
	case CTPopupMenuWnd::__CMBA_TOP		:
	case CTPopupMenuWnd::__CMBA_BOTTOM	: _rcClient.InflateRect( -1, 0); break;
	case CTPopupMenuWnd::__CMBA_LEFT	:
	case CTPopupMenuWnd::__CMBA_RIGHT	: _rcClient.InflateRect( 0, -1); break;
	}

	dc.ExcludeClipRect(&_rcClient);
	dc.Draw3dRect(
		&_rcArea,
		clrMenuBorder,
		clrMenuBorder);
	dc.SelectClipRgn(NULL);
}

void CTPaintManager::PaintControlBarBordersDefImpl( e_control_bar_border_type nCBBT,
												    DWORD dwStyle,
													CDC &dc,
													CRect &rect,
													COLORREF clrHi,
													COLORREF clrLo)
{
	if(!(dwStyle&CBRS_BORDER_ANY))
		return;

	CRect rect1(rect);
	CRect rect2(rect);
	COLORREF clr = clrLo;

	if(dwStyle&CBRS_BORDER_3D)
	{
		rect1.right -= CX_BORDER;
		rect1.bottom -= CY_BORDER;
	}

	if(dwStyle&CBRS_BORDER_TOP)
		rect2.top += afxData.cyBorder2;

	if(dwStyle&CBRS_BORDER_BOTTOM)
		rect2.bottom -= afxData.cyBorder2;

	if(dwStyle&CBRS_BORDER_LEFT)
		dc.FillSolidRect( 0, rect2.top, CX_BORDER, rect2.Height(), clr);

	if(dwStyle&CBRS_BORDER_TOP)
		dc.FillSolidRect( 0, 0, rect.right, CY_BORDER, clr);

	if(dwStyle&CBRS_BORDER_RIGHT)
		dc.FillSolidRect( rect1.right, rect2.top, -CX_BORDER, rect2.Height(), clr);

	if(dwStyle&CBRS_BORDER_BOTTOM)
		dc.FillSolidRect( 0, rect1.bottom, rect.right, -CY_BORDER, clr);

	if(dwStyle&CBRS_BORDER_3D)
	{
		clr = clrHi;

		if(dwStyle&CBRS_BORDER_LEFT)
			dc.FillSolidRect( 1, rect2.top, CX_BORDER, rect2.Height(), clr);

		if(dwStyle&CBRS_BORDER_TOP)
			dc.FillSolidRect( 0, 1, rect.right, CY_BORDER, clr);

		if(dwStyle&CBRS_BORDER_RIGHT)
			dc.FillSolidRect( rect.right, rect2.top, -CX_BORDER, rect2.Height(), clr);

		if(dwStyle&CBRS_BORDER_BOTTOM)
			dc.FillSolidRect( 0, rect.bottom, rect.right, -CY_BORDER, clr);
	}

	if(dwStyle&CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;

	if(dwStyle&CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;

	if(dwStyle&CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;

	if(dwStyle&CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;
}

void CTPaintManager::PaintControlBarBorders( e_control_bar_border_type nCBBT,
											 DWORD dwStyle,
											 CDC &dc,
											 CRect &rect)
{
	if( nCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		dc.Draw3dRect(
			&rect,
			GetColor(CLR_3DSHADOW_OUT),
			GetColor(CLR_3DHILIGHT_OUT));

		return;
	}

	COLORREF clr = GetColor(CLR_3DFACE_OUT);
	PaintControlBarBordersDefImpl( nCBBT, dwStyle, dc, rect, clr, clr);
}

void CTPaintManagerXP::PaintControlBarBorders( e_control_bar_border_type nCBBT,
											   DWORD dwStyle,
											   CDC &dc,
											   CRect &rect)
{
	if( nCBBT == __CB_INNER_STATUSBAR_ITEM )
	{
		COLORREF clr = GetColor(COLOR_3DSHADOW);
		dc.Draw3dRect( &rect, clr, clr);

		return;
	}

	COLORREF clr = GetColor(CLR_3DFACE_OUT);
	PaintControlBarBordersDefImpl( nCBBT, dwStyle, dc, rect, clr, clr);
}

void CTPaintManager::PaintResizableBarChildNcAreaRect( CDC &dc, const CRect &rect)
{
	PaintNcAreaRect( dc, rect);
}

void CTPaintManagerXP::PaintResizableBarChildNcAreaRect( CDC &dc, const CRect &rect)
{
	CTPaintManager::PaintResizableBarChildNcAreaRect( dc, rect);
}
