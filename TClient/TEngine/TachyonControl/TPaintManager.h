#if (!defined __TPAINT_MANAGER_H)
#define __TPAINT_MANAGER_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __DEFAULT_PAINT_MANAGER_CLASS)
	#define __DEFAULT_PAINT_MANAGER_CLASS	CTPaintManagerXP
#endif

#define __TMFC_MIN_SYS_COLOR_VAL			0L
#define __TMFC_MAX_SYS_COLOR_VAL			49L
#define __TMFC_COLOR_MAP_BASE				1000L
#define __TMFC_DISABLED_COLOR_SHIFT			1000L
#define __TMFC_XP_COLOR_SHIFT				(__TMFC_DISABLED_COLOR_SHIFT + 1000L)

#define __DEF_MENU_ICON_CX					16
#define __DEF_MENU_ICON_CY					__DEF_MENU_ICON_CX
#define __DEF_MENU_GAP						3
#define __DEF_MENU_HEIGHT					(__DEF_MENU_ICON_CY + __DEF_MENU_GAP * 2)
#define __DEF_MENU_SEPARATOR_HEIGHT			4
#define __DEF_MENU_ICON_AREA_DX				(__DEF_MENU_ICON_CX + __DEF_MENU_GAP * 2)
#define __DEF_MENU_POPUP_ARROW_AREA_DX		16
#define __DEF_MENU_IMG_RECT_OUT_GAP			2

#define __VERT_FONT_ESCAPEMENT_X__			(900)
#define __VERT_FONT_ESCAPEMENT__			(2700)
#define __DROPDOWN_ARROW_GAP				3


class __TUI_API CTPaintManager : public CObject
{
	DECLARE_DYNAMIC(CTPaintManager)

protected:
	typedef CArray< COLORREF, COLORREF&>	sys_colors_container;
	sys_colors_container m_colors;

	typedef CArray< HBRUSH, HBRUSH&>		sys_brashes_container;
	sys_brashes_container m_brushes;

	typedef CMap< int, int, int, int>		translate_color_map;
	translate_color_map m_mapColorTranslate;

public:
	CPalette m_PaletteWide;
	CBrush m_brushLight;

	CFont m_FontNormalVertX;
	CFont m_FontNormalVert;
	CFont m_FontNormal;

	CFont m_FontBoldVertX;
	CFont m_FontBoldVert;
	CFont m_FontMarlett;
	CFont m_FontBold;

	int m_DSS_DISABLED_style;

	class __TUI_API CTPaintManagerAutoPtr
	{
		CTPaintManager *m_pPaintMenager;
		BOOL InitPaintManagerInstance();

	public:
		OSVERSIONINFO m_osVerData;
		CString m_strOsVer;

		BOOL m_bIsWin32s:TRUE;
		BOOL m_bIsWin9x:TRUE;
		BOOL m_bIsWin98:TRUE;
		BOOL m_bIsWin98orLater:TRUE;
		BOOL m_bIsWinNT:TRUE;
		BOOL m_bIsWinNT4:TRUE;
		BOOL m_bIsWin2000:TRUE;
		BOOL m_bIsWinXP:TRUE;

		CTPaintManagerAutoPtr();
		~CTPaintManagerAutoPtr();

		CTPaintManager *operator->();

		BOOL InstallPaintManager( CTPaintManager *pPaintMenager);
		BOOL InstallPaintManager( CRuntimeClass *pRtcPaintMenager);
	}; // class CTPaintManagerAutoPtr

	CTPaintManager();
	~CTPaintManager();

	COLORREF GetColor( int nColorIndex);
	HBRUSH GetBrush( int nColorIndex);
	void SyncSysColors();

	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(CLR_3DFACE_OUT);
	};

	int InstallColor(
		COLORREF clr,
		int nColorIndex = -1);

	virtual void InitTranslatedColors();
	enum e_translated_colors
	{
		CLR_3DFACE_OUT = __TMFC_COLOR_MAP_BASE + 0,
		CLR_3DFACE_IN = __TMFC_COLOR_MAP_BASE + 1,
		CLR_3DFACE_DISABLED = CLR_3DFACE_OUT + __TMFC_DISABLED_COLOR_SHIFT,

		CLR_3DLIGHT_OUT = __TMFC_COLOR_MAP_BASE + 2,
		CLR_3DLIGHT_IN = __TMFC_COLOR_MAP_BASE + 3,
		CLR_3DLIGHT_DISABLED = CLR_3DLIGHT_OUT + __TMFC_DISABLED_COLOR_SHIFT,

		CLR_3DHILIGHT_OUT = __TMFC_COLOR_MAP_BASE + 4,
		CLR_3DHILIGHT_IN = __TMFC_COLOR_MAP_BASE + 5,
		CLR_3DHILIGHT_DISABLED = CLR_3DHILIGHT_OUT + __TMFC_DISABLED_COLOR_SHIFT,

		CLR_3DSHADOW_OUT = __TMFC_COLOR_MAP_BASE + 6,
		CLR_3DSHADOW_IN = __TMFC_COLOR_MAP_BASE + 7,
		CLR_3DSHADOW_DISABLED = CLR_3DSHADOW_OUT + __TMFC_DISABLED_COLOR_SHIFT,

		CLR_3DDKSHADOW_OUT = __TMFC_COLOR_MAP_BASE + 8,
		CLR_3DDKSHADOW_IN = __TMFC_COLOR_MAP_BASE + 9,
		CLR_3DDKSHADOW_DISABLED = CLR_3DDKSHADOW_OUT + __TMFC_DISABLED_COLOR_SHIFT,

		CLR_TEXT_OUT = __TMFC_COLOR_MAP_BASE + 10,
		CLR_TEXT_IN = __TMFC_COLOR_MAP_BASE + 11,
		CLR_TEXT_DISABLED = CLR_TEXT_OUT + __TMFC_DISABLED_COLOR_SHIFT,
	};

	enum e_align
	{
		__ALIGN_HORIZ_CENTER = 1,
		__ALIGN_HORIZ_RIGHT = 2,
		__ALIGN_VERT = 4
	};
	virtual CSize GetPushedOffset();

	class __TUI_API wingl
	{
		const UINT *m_pColorIndexes;
		BOOL m_bDynamicData;

		UINT m_nColors;
		CSize m_size;

	public:
		wingl()
		{
			_Init();
		};

		wingl( const SIZE &_size, UINT nColors, const UINT *pColorIndexes)
		{
			m_bDynamicData = FALSE;

			m_size.cx = _size.cx;
			m_size.cy = _size.cy;
			m_nColors = nColors;
			m_pColorIndexes = pColorIndexes;
		};

		wingl( int cx, int cy, UINT nColors, const UINT *pColorIndexes)
		{
			m_bDynamicData = FALSE;

			m_size.cx = cx;
			m_size.cy = cy;
			m_nColors = nColors;
			m_pColorIndexes = pColorIndexes;
		};

		wingl( const wingl& other, UINT nRotateAngle = 0)
		{
			_Init();
			_CopyFromOther(other);

			Rotate(nRotateAngle);
		};

		~wingl()
		{
			_Done();
		};

		const wingl& operator=( const wingl& other)
		{
			_CopyFromOther(other);
			return *this;
		};

		UINT GetColorPixel( int x, int y)
		{
			int nOffset = y * m_size.cx + x;
			UINT nValue = *(m_pColorIndexes + nOffset);

			return nValue;
		};

		void Rotate( UINT nRotateAngle)
		{
			if(!nRotateAngle)
				return;

			int nBufferSize = m_size.cx * m_size.cy;
			UINT *pNew = new UINT[nBufferSize];

			if(!pNew)
				return;

			UINT *pOld = (UINT *) m_pColorIndexes;
			int x, y;

			switch(nRotateAngle)
			{
			case 90		:
				{
					for( y=0; y<m_size.cy; y++)
						for( x=0; x<m_size.cx; x++, pOld++)
						{
							int nNewOffset = x * m_size.cy + (m_size.cy - y - 1);
							UINT *p = pNew + nNewOffset;

							*p = *pOld;
						}

					m_size = CSize( m_size.cy, m_size.cx);
				}

				break;

			case 180	:
				{
					for( y=0; y<m_size.cy; y++)
						for( x=0; x<m_size.cx; x++, pOld++)
						{
							int nNewOffset = (m_size.cy - y - 1) * m_size.cx + (m_size.cx - x - 1);
							UINT *p = pNew + nNewOffset;

							*p = *pOld;
						}
				}

				break;

			case 270	:
				{
					for( y=0; y<m_size.cy; y++)
						for( x=0; x<m_size.cx; x++, pOld++)
						{
							int nNewOffset = (m_size.cx - x - 1) * m_size.cy + y;
							UINT *p = pNew + nNewOffset;

							*p = *pOld;
						}

					m_size = CSize( m_size.cy, m_size.cx);
				}

				break;
			}

			::memcpy( (UINT *) m_pColorIndexes, pNew, nBufferSize * sizeof(UINT));
			delete[] pNew;
		};

		CSize Size() const
		{
			return m_size;
		};

		operator CSize() const
		{
			return Size();
		};

		const UINT *GetSurface() const
		{
			return m_pColorIndexes;
		};

		UINT GetColorsCount() const
		{
			return m_nColors;
		};

	private:
		void _Init()
		{
			m_size.cx = 0;
			m_size.cy = 0;
			m_nColors = 1;

			m_pColorIndexes = NULL;
			m_bDynamicData = FALSE;
		};

		void _Done()
		{
			if(m_bDynamicData)
			{
				delete[] ((UINT *) m_pColorIndexes);
				m_pColorIndexes = NULL;
			}

			_Init();
		};

		BOOL _CopyFromOther( const wingl& other)
		{
			_Done();

			if(other.m_pColorIndexes)
			{
				int nBufferSize = other.m_size.cx * other.m_size.cy;
				m_pColorIndexes = new UINT[nBufferSize];

				if(!m_pColorIndexes)
					return FALSE;

				::memcpy( (void *) m_pColorIndexes, other.m_pColorIndexes, nBufferSize * sizeof(UINT));
				m_bDynamicData = TRUE;
			}

			m_size.cx = other.m_size.cx;
			m_size.cy = other.m_size.cy;
			m_nColors = other.m_nColors;

			return TRUE;
		};

		void _SetColorPixel( int x, int y, UINT nValue)
		{
			int nOffset = y * m_size.cx + x;
			UINT *ptr = (UINT *) m_pColorIndexes + nOffset;

			*ptr = nValue;
		};
	}; // class wingl

	static const wingl m_wingl_btn_expand_left;
	static const wingl m_wingl_btn_expand_right;
	static const wingl m_wingl_btn_expand_right2;
	static const wingl m_wingl_btn_expand_bottom;
	static const wingl m_wingl_btn_expand_bottom2;

	virtual void PaintWingl(
		CDC &dc,
		POINT ptDest,
		const wingl& _wingl,
		COLORREF *pColorValues);

	virtual void PaintWinglCentered(
		CDC &dc,
		const RECT &rect,
		const wingl &_wingl,
		COLORREF *pColorValues);

	virtual void PaintToolbarExpandButton(
		CDC &dc,
		const CRect &rectButton,
		BOOL bHorz,
		BOOL bBarIsCompletelyVisible,
		BOOL bEnabled,
		BOOL bPushed,
		BOOL bHover,
		BOOL bTransparentBackground = FALSE);

	virtual void PaintMenuExpandButton(
		CDC &dc,
		const CRect &rectButton,
		BOOL bPushed,
		BOOL bHover);

	static COLORREF RGBtoCMY( COLORREF clr);
	static COLORREF CMYtoRGB( COLORREF clr);

	static double HuetoRGB( double m1, double m2, double h);
	static COLORREF HLStoRGB( double H, double L, double S);
	static void RGBtoHSL( COLORREF rgb, double *H, double *S, double *L);

	static BYTE GetRGBFromHue( float rm1, float rm2, float rh);
	static COLORREF GetRGBFromHLSExtend( double H, double L, double S);

	static COLORREF HLS_Adjust(
		COLORREF clr,
		double percentH = 0.0,
		double percentL = 0.0,
		double percentS = 0.0);

	static BOOL PaintParentBk(
		HWND hWnd,
		HDC hDC,
		LPCRECT rectClip = NULL);

	typedef BOOL (*pfnExcludeChildAreaCallback)(
		HDC hDC,
		HWND hWnd,
		HWND hWndChild,
		LPVOID pCookie);

	static void ExcludeChildAreas(
		HDC hDC,
		HWND hWnd,
		pfnExcludeChildAreaCallback pCallback = NULL,
		LPVOID pCallbackCookie = NULL);

	static void TransparentBlt(
		HDC hdcDest,
		int nXOriginDest,
		int nYOriginDest,
		int nWidthDest,
		int hHeightDest,
		HDC hdcSrc,
		int nXOriginSrc,
		int nYOriginSrc,
		int nWidthSrc,
		int nHeightSrc,
		UINT crTransparent);

	static HBITMAP GenLBDIB();
	static void PaintGradientRect(
		CDC &dc,
		const CRect &rcPaintGradient,
		COLORREF clrLeft,
		COLORREF clrRight,
		BOOL bHorz = FALSE,
		UINT nCountOfSteps = 256);

	static void PaintDotNet3dRect(
		CDC &dc,
		LPCRECT lpRect,
		COLORREF clrTopLeft,
		COLORREF clrBottomRight);

	static void PaintDotNet3dRect(
		CDC &dc,
		int x,
		int y,
		int cx,
		int cy,
		COLORREF clrTopLeft,
		COLORREF clrBottomRight);

	static void PaintRadioDot(
		CDC &dc,
		int x,
		int y,
		COLORREF color);

	static void PaintCheckMark(
		CDC &dc,
		int x,
		int y,
		COLORREF color);

	virtual void PaintNcAreaRect(
		CDC &dc,
		const CRect &rc);

	virtual void PaintResizableBarChildNcAreaRect(
		CDC &dc,
		const CRect &rc);

	static CRect CalcTextDimension(
		CDC &dc,
		CFont &font,
		const CString &strText);

	static int CalcTextWidth(
		CDC &dc,
		CFont &font,
		const CString &strText);

	virtual void PaintIcon(
		CDC &dc,
		BOOL bHorz,
		CString &strTitle,
		HICON hIcon,
		const CRect &rcItem,
		CRect &rcCaption,
		BOOL bPushed,
		BOOL bEnabled,
		BOOL bHover,
		int nAlign);

	enum e_docking_caption_button
	{
		__DCBT_EMPTY			= 0,
		__DCBT_CLOSE			= 1,
		__DCBT_ARROW_UP			= 2,
		__DCBT_ARROW_DOWN		= 3,
		__DCBT_ARROW_LEFT		= 4,
		__DCBT_ARROW_RIGHT		= 5,
		__DCBT_WND_MINIMIZE		= 6,
		__DCBT_WND_RESTORE		= 7,
		__DCBT_WND_MAXIMIZE		= 8,
		__DCBT_WND_CONTEXTHELP	= 9,
		__DCBT_WND_KEEP			= 10,
		__DCBT_WND_UNKEEP		= 11,
		__DCBT_ARROW_UP_L		= 12,
		__DCBT_ARROW_DOWN_L		= 13,
		__DCBT_ARROW_LEFT_L		= 14,
		__DCBT_ARROW_RIGHT_L	= 15,
		__DCBT_XP_THIN_CLOSE	= 16,
		__DCBT_HOLLOW_UP		= 17,
		__DCBT_HOLLOW_DOWN		= 18,
		__DCBT_HOLLOW_LEFT		= 19,
		__DCBT_HOLLOW_RIGHT		= 20,
		__DCBT_CLOSE_DC2K		= 21,

		__DCBT_VALUE_MIN		= 0,
		__DCBT_VALUE_MAX		= 21,
	};

	static wingl *m_DockingCaptionWINGLs[__DCBT_VALUE_MAX + 1];
	virtual void PaintDockingCaptionButton(
		CDC &dc,
		const CRect &rcClient,
		e_docking_caption_button nType,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled,
		BOOL bSideBar,
		BOOL bBarWndActive,
		BOOL bFloating);

	virtual void PaintSysCmdWINGL(
		CDC &dc,
		UINT nHelperSysCmdID,
		const CRect &rcItem,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled);

	virtual void PaintPushButton(
		CDC &dc,
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
		CFont *pFont = NULL,
		BOOL bDropDown = FALSE,
		UINT nHelperSysCmdID = 0,
		BOOL bTransparentBackground = FALSE);

	virtual void PaintMenuItem(
		CDC &dc,
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
		UINT nHelperSysCmdID);

	virtual void PaintMenuSeparator(
		CDC &dc,
		CRect &rcItem,
		BOOL bRarelyUsed);

	virtual void PaintSeparator(
		CDC &dc,
		CRect &rcItem,
		BOOL bHorz);

	virtual void PaintGripper(
		CDC &dc,
		const CRect &rcGripper,
		const CRect &rcText,
		BOOL bActive,
		BOOL bFloating,
		BOOL bHorz,
		BOOL bSideBar = FALSE,
		LPCTSTR strCaption = NULL);

	virtual void PaintDockingFrame(
		CDC &dc,
		const CRect &rcWindow,
		const CRect &rcClient,
		BOOL bFloating,
		BOOL bTBar,
		BOOL bSideBar = FALSE);

	virtual void PaintControlFrame(
		CDC &dc,
		CRect &rcClient,
		BOOL bFlat,
		BOOL bHover,
		BOOL bEnabled,
		BOOL bFocus);

	virtual void PaintComboFrame(
		CDC &dc,
		CRect &rcClient,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled);

	static BOOL DefIsHwndNeedsDirectRepaint( HWND hWndChild);
	static BOOL DefExcludeChildAreaCallback(
		HDC hDC,
		HWND hWnd,
		HWND hWndChild,
		LPVOID pCookie);

	static int GetBPP();

	CSize GetTextSizes( BOOL bBold = FALSE);
	virtual int GetTextHeight( BOOL bHorz);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();
	virtual void PaintMenuBorder(
		CDC &dc,
		const CRect &rcClient);

	virtual BOOL IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC &dc,
		const CRect &rcArea,
		const CRect &rcClient,
		int nCombineAlign);

	enum e_control_bar_border_type
	{
		__CB_OUTER_DOCKBAR = 0,
		__CB_OUTER_STATUSBAR = 1,
		__CB_INNER_STATUSBAR_ITEM = 2,
	};

	void PaintControlBarBordersDefImpl(
		e_control_bar_border_type nCBBT,
		DWORD dwStyle,
		CDC &dc,
		CRect &rc,
		COLORREF clrHi,
		COLORREF clrLo);

	virtual void PaintControlBarBorders(
		e_control_bar_border_type nCBBT,
		DWORD dwStyle,
		CDC &dc,
		CRect &rc);

	virtual void PaintDockBarClientArea(
		CDC &dc,
		const CRect &rcClient);
}; // class CTPaintManager


class __TUI_API CTPaintManagerXP : public CTPaintManager
{
public:
	DECLARE_DYNAMIC(CTPaintManagerXP)

private:
	virtual void InitTranslatedColors();
	virtual CSize GetPushedOffset();

	virtual void PaintPushButton(
		CDC &dc,
		BOOL bHorz,
		const CRect& rcClient,
		LPCTSTR strText,
		HICON hIcon,
		BOOL bFlat,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled,
		BOOL bIndeterminate,
		BOOL bDrawBorder,
		BOOL bDrawFocusRect,
		BOOL bDefaultPushButton,
		int nAlign,
		CFont *pFont = NULL,
		BOOL bDropDown = FALSE,
		UINT nHelperSysCmdID = 0,
		BOOL bTransparentBackground = FALSE);

	virtual void PaintGripper(
		CDC &dc,
		const CRect &rcGripper,
		const CRect &rcText,
		BOOL bActive,
		BOOL bFloating,
		BOOL bHorz,
		BOOL bSideBar = FALSE,
		LPCTSTR sCaption = NULL);

	virtual void PaintSeparator(
		CDC &dc,
		CRect &rcItem,
		BOOL bHorz);

	virtual void PaintIcon(
		CDC &dc,
		BOOL bHorz,
		CString &strTitle,
		HICON hIcon,
		const CRect &rcItem,
		CRect &rcCaption,
		BOOL bPushed,
		BOOL bEnabled,
		BOOL bHover,
		int nAlign);

	virtual void PaintMenuItem(
		CDC &dc,
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
		UINT nHelperSysCmdID);

	virtual void PaintMenuSeparator(
		CDC &dc,
		CRect &rcItem,
		BOOL bRarelyUsed);

	virtual void PaintControlFrame(
		CDC &dc,
		CRect &rcClient,
		BOOL bFlat,
		BOOL bHover,
		BOOL bEnabled,
		BOOL bFocus);

	virtual void PaintComboFrame(
		CDC &dc,
		CRect &rectClient,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled);

	virtual void PaintSysCmdWINGL(
		CDC &dc,
		UINT nHelperSysCmdID,
		const CRect &rcItem,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled);

	virtual void PaintToolbarExpandButton(
		CDC &dc,
		const CRect &rcButton,
		BOOL bHorz,
		BOOL bBarIsCompletelyVisible,
		BOOL bEnabled,
		BOOL bPushed,
		BOOL bHover,
		BOOL bTransparentBackground = FALSE);

	virtual void PaintMenuExpandButton(
		CDC &dc,
		const CRect &rcButton,
		BOOL bPushed,
		BOOL bHover);

	virtual void PaintDockingFrame(
		CDC &dc,
		const CRect &rcWindow,
		const CRect &rcClient,
		BOOL bFloating,
		BOOL bTBar,
		BOOL bSideBar = FALSE);

	virtual int GetMenuShadowSize();
	virtual int GetMenuBorderSize();

	virtual void PaintMenuBorder(
		CDC &dc,
		const CRect &rcClient);

public:
	enum e_xp_colors
	{
		XPCLR_3DFACE_DARK		= __TMFC_XP_COLOR_SHIFT + 0,
		XPCLR_3DFACE_NORMAL		= __TMFC_XP_COLOR_SHIFT + 1,
		XPCLR_SEPARATOR			= __TMFC_XP_COLOR_SHIFT + 2,
		XPCLR_HILIGHT			= __TMFC_XP_COLOR_SHIFT + 3,
		XPCLR_HILIGHT_BORDER	= __TMFC_XP_COLOR_SHIFT + 4,
		XPCLR_RARELY_BORDER		= __TMFC_XP_COLOR_SHIFT + 5,
	};

	virtual BOOL IsMenuMustCombineExcludeArea();
	virtual void PaintMenuCombinedArea(
		CDC &dc,
		const CRect &rcArea,
		const CRect &rcClient,
		int nCombineAlign);

	virtual void PaintDockingCaptionButton(
		CDC &dc,
		const CRect &rcClient,
		e_docking_caption_button nType,
		BOOL bHover,
		BOOL bPushed,
		BOOL bEnabled,
		BOOL bSideBar,
		BOOL bBarWndActive,
		BOOL bFloating);

	virtual COLORREF GetMenuFrameFillColor()
	{
		return GetColor(XPCLR_3DFACE_NORMAL);
	};

	virtual void PaintControlBarBorders(
		e_control_bar_border_type nCBBT,
		DWORD dwStyle,
		CDC &dc,
		CRect &rc);

	virtual void PaintNcAreaRect(
		CDC &dc,
		const CRect &rc);

	virtual void PaintResizableBarChildNcAreaRect(
		CDC &dc,
		const CRect &rc);

	virtual void PaintDockBarClientArea(
		CDC &dc,
		const CRect &rcClient);
};


extern __TUI_API CTPaintManager::CTPaintManagerAutoPtr	g_PaintManager;

#endif // #if (!defined __TPAINT_MANAGER_H)