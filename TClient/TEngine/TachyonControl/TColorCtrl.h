#if (!defined __TCOLORCTRL_H)
#define __TCOLORCTRL_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

#if (!defined __AFXPLEX_H__)
	#include <AfxTempl.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl window

class __TUI_API CTColorCtrl : public CWnd
{
public:
	enum e_mode
	{
		MODE_HEXAGON = 0,
		MODE_HSL_PANEL = 1,
		MODE_HSL_BASIC_HUE = 2,
		MODE_HSL_ROLLER = 3,
		MODE_RGB_CUBE = 4,
		MODE_RGB_MIX = 5,
		MODE_GRAY_SCALE_MIX = 6,
		MODE_CMY_CUBE = 7,
		MODE_CMY_MIX = 8,
		MODE_XYZ_R = 9,
		MODE_XYZ_G = 10,
		MODE_XYZ_B = 11,
		MODE_XYZ_C = 12,
		MODE_XYZ_M = 13,
		MODE_XYZ_Y = 14,
		MODE_DIFF = 15,
		MODE_VAL_MIN = 0,
		MODE_VAL_MAX = 15
	};

	CTColorCtrl( e_mode nMode = MODE_HEXAGON);
	struct __TUI_API selection_colors
	{
		COLORREF m_clrCurrent;
		COLORREF m_clrNew;
		COLORREF m_clrPrev;

		selection_colors(
			COLORREF clrCurrent = RGB( 255, 255, 255),
			COLORREF clrNew = RGB( 255, 255, 255),
			COLORREF clrPrev = RGB( 255, 255, 255)) : m_clrCurrent(clrCurrent),
			m_clrNew(clrNew),
			m_clrPrev(clrPrev)
		{
		};

		selection_colors( const selection_colors& other) : m_clrCurrent(RGB( 255, 255, 255)),
			m_clrNew(RGB( 255, 255, 255)),
			m_clrPrev(RGB( 255, 255, 255))
		{
			AssignFromOther(other);
		};

		~selection_colors()
		{
		};

		selection_colors& operator=( const selection_colors& other)
		{
			AssignFromOther(other);
			return *this;
		};

	private:
		void AssignFromOther( const selection_colors& other)
		{
			m_clrCurrent = other.m_clrCurrent;
			m_clrPrev = other.m_clrPrev;
			m_clrNew = other.m_clrNew;
		};
	}; // struct selection_colors

// Attributes
public:
private:
	BOOL m_bMouseTracking;
	e_mode m_nMode;

	struct __TUI_API any_picker_data
	{
		enum eMoveMode
		{
			_MM_NONE = 0,
			_MM_RED = 1,
			_MM_GREEN = 2,
			_MM_BLUE = 3,
			_MM_SLIDER = 4,
			_MM_PICKER = 5,
			_MM_CELL_SCAN = 6,
		};
		eMoveMode m_nMoveMode;

		CSize m_szTotal;
		COLORREF m_clrOld;
		COLORREF m_clr;

		double m_fCurrSaturation;
		double m_fCurrLuminance;
		double m_fCurrHue;

		any_picker_data();
		~any_picker_data();

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDone() = 0;
		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette) = 0;

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point) = 0;

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point) = 0;

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point) = 0;

		virtual void OnSetOldRGB( COLORREF clr);
		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);
	}; // struct any_picker_data

	struct __TUI_API cell
	{
		CPoint m_vPoints[6];
		CRect m_rcPosition;

		BOOL m_bAtFirstType;
		int m_nBorderSize;
		COLORREF m_clr;

		cell( COLORREF clr = RGB( 255, 255, 255)) : m_clr(clr),
			m_nBorderSize(1),
			m_bAtFirstType(TRUE)
		{
			for( int i=0; i<6; i++)
				m_vPoints[i] = CPoint(0,0);
		};

		cell( const cell& other) : m_clr( RGB( 255, 255, 255)),
			m_nBorderSize(1),
			m_bAtFirstType(TRUE)
		{
			for( int i=0; i<6; i++)
				m_vPoints[i] = CPoint(0,0);

			AssignFromOther(other);
		};

		~cell()
		{
		};

		cell& operator=( const cell& other)
		{
			AssignFromOther(other);
			return *this;
		};

		cell& operator=( COLORREF clr)
		{
			m_clr = clr;
			return *this;
		};

		void AdjustCellPosition(
			int x,
			int y,
			int nWidth)
		{
			FLOAT fSideLength = (FLOAT) ((FLOAT) nWidth * 0.57735026918962f);

			m_vPoints[0] = CPoint( MoreNear(x - (FLOAT) (nWidth / 2)), MoreNear(y - (FLOAT) (fSideLength / 2.0f)));
			m_vPoints[1] = CPoint( MoreNear((FLOAT) x), MoreNear(y - (FLOAT) (nWidth / 2)));
			m_vPoints[2] = CPoint( MoreNear(x + (FLOAT) (nWidth / 2)), MoreNear(y - (FLOAT) (fSideLength / 2.0f)));
			m_vPoints[3] = CPoint( MoreNear(x + (FLOAT) (nWidth / 2)), MoreNear(y + (FLOAT) (fSideLength / 2.0f)) + 1);
			m_vPoints[4] = CPoint( MoreNear((FLOAT) x), MoreNear(y + (FLOAT) (nWidth / 2)) + 1);
			m_vPoints[5] = CPoint( MoreNear(x - (FLOAT) (nWidth / 2)), MoreNear(y + (FLOAT) (fSideLength / 2.0f)) + 1);
		};

		static int MoreNear( FLOAT fNear)
		{
			int nRound = (int) fabs(fNear);

			nRound = (fabs(fNear) - (FLOAT) nRound >= 0.5f) ? nRound + 1 : nRound;
			return fNear < 0.0f ? -nRound : nRound;
		};

		void SetRect( const CRect &rcPos)
		{
			m_rcPosition = rcPos;
		};

		CRect GetRect() const
		{
			if(!m_bAtFirstType)
			{
				return m_rcPosition;
			}
			CRect rcBounds;
			rcBounds.left = rcBounds.right  = m_vPoints[0].x;
			rcBounds.top  = rcBounds.bottom = m_vPoints[0].y;

			for( int i=1; i<6; i++)
			{
				rcBounds.left = min( rcBounds.left, m_vPoints[i].x);
				rcBounds.top = min( rcBounds.top, m_vPoints[i].y);
				rcBounds.right = max( rcBounds.right, m_vPoints[i].x);
				rcBounds.bottom = max(rcBounds.bottom, m_vPoints[i].y);
			}

			return rcBounds;
		};

		void SetCellColor( const COLORREF &cr)
		{
			m_clr = cr;
		};

		void OnDraw( CDC &dc, CPalette *pPalette, BOOL bFocused)
		{
			dc.SaveDC();
			dc.SetBkMode(TRANSPARENT);

			CBrush _brush(m_clr);
			dc.SelectObject(&_brush);

			CPen _pen( PS_SOLID, 0, m_clr);
			dc.SelectObject(&_pen);

			CBrush *pBrWhite = NULL;
			CBrush *pBrBlack = NULL;

			if(bFocused)
			{
				pBrWhite = CBrush::FromHandle((HBRUSH) ::GetStockObject(WHITE_BRUSH));
				pBrBlack = CBrush::FromHandle((HBRUSH) ::GetStockObject(BLACK_BRUSH));
			}

			if(!m_bAtFirstType)
			{
				if(bFocused)
				{
					CRect rcPos(m_rcPosition);
					rcPos.InflateRect( 3, 3);

					dc.FillSolidRect( rcPos, RGB( 0, 0, 0));
					rcPos.DeflateRect( 1, 1);

					dc.FillSolidRect( rcPos, RGB( 255, 255, 255));
					rcPos.DeflateRect( 1, 1);

					dc.FillSolidRect( rcPos, RGB( 0, 0, 0));
					dc.FillSolidRect( m_rcPosition, m_clr);
				}
				else if(pPalette)
				{
					CBrush brush(m_clr);

					dc.FillRect(
						m_rcPosition,
						&brush);
				}
				else
					dc.FillSolidRect(m_rcPosition,m_clr);
			}
			else
			{
				if(bFocused)
				{
					CPoint vPoints[6];
					CRgn rgnOne;
					CRgn rgnTwo;

					for( int i=0; i<6; i++)
						vPoints[i] = m_vPoints[i];

					vPoints[0].x -= 2;
					vPoints[0].y -= 1;

					vPoints[5].x -= 2;
					vPoints[5].y += 1;

					vPoints[2].x += 2;
					vPoints[2].y -= 1;
					vPoints[3].x += 2;
					vPoints[3].y += 1;

					vPoints[1].y -= 2;
					vPoints[4].y += 2;

					rgnOne.CreatePolygonRgn( vPoints, 6, ALTERNATE);
					rgnTwo.CreatePolygonRgn( m_vPoints, 6, ALTERNATE);

					dc.SelectObject(pBrWhite);
					dc.Polygon( vPoints, 6);
					dc.SelectObject(&_brush);
					dc.Polygon( m_vPoints, 6);

					dc.FrameRgn( &rgnOne, pBrWhite, 3, 3);
					dc.FrameRgn( &rgnOne, pBrBlack, 1, 1);
					dc.FrameRgn( &rgnTwo, pBrBlack, 1, 1);
				}
				else
					dc.Polygon( m_vPoints, 6);
			}

			dc.RestoreDC(-1);
		};

		BOOL HitTest( CPoint point)
		{
			if(m_bAtFirstType)
			{
				CRgn rgn;

				rgn.CreatePolygonRgn( m_vPoints, 6, ALTERNATE);
				return rgn.PtInRegion(point);
			}
			else
			{
				CRgn rgn;

				rgn.CreateRectRgn( m_rcPosition.left, m_rcPosition.top, m_rcPosition.right, m_rcPosition.bottom);
				return rgn.PtInRegion(point);
			}
		};

	private:
		void AssignFromOther( const cell& other)
		{
			m_nBorderSize = other.m_nBorderSize;
			m_rcPosition = other.m_rcPosition;
			m_clr = other.m_clr;

			for( int i=0; i<6; i++)
				m_vPoints[i] = other.m_vPoints[i];

			m_bAtFirstType = other.m_bAtFirstType;
		};
	}; // struct cell

	typedef CArray< cell, cell&>	vCells;
	struct __TUI_API cell_based_data : public any_picker_data
	{
		vCells m_vCells;

		cell_based_data()
		{
		};

		~cell_based_data()
		{
			ReleaseAll();
		};

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDone();
		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point);

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

	private:
		void Generate( BOOL bOnlySizeChanged);
		void ReleaseAll();
	}; // struct cell_based_data

	struct __TUI_API grayscale_mixer_data : public cell_based_data
	{
		grayscale_mixer_data()
		{
		};

		~grayscale_mixer_data()
		{
		};

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);
	}; // struct grayscale_mixer_data

	struct __TUI_API rgb_mixer_data : public cell_based_data
	{
		rgb_mixer_data()
		{
		};

		~rgb_mixer_data()
		{
		};

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);
	}; // struct rgb_mixer_data

	struct __TUI_API rgb_cube_data : public any_picker_data
	{
		CSize m_szHalfSelBox;
		CRect m_rcCube;
		CDC m_dcCube;

		CPoint m_ptClrCurr;
		CPoint m_ptPlainRG;
		CPoint m_ptPlainRB;
		CPoint m_ptPlainGB;

		CPoint m_ptCubeR;
		CPoint m_ptCubeG;
		CPoint m_ptCubeB;
		CPoint m_ptCubeC;

		CPoint m_ptClrR;
		CPoint m_ptClrG;
		CPoint m_ptClrB;
		int m_nSideGap;

		rgb_cube_data();
		~rgb_cube_data();

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDone();
		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point);

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	private:
		void RecalcLayout();
		void ReleaseAll();

		void Generate(
			CDC &dc,
			CPalette *pPalette);
	}; // struct rgb_cube_data

	struct __TUI_API cmy_mixer_data : public cell_based_data
	{
		cmy_mixer_data()
		{
		};

		~cmy_mixer_data()
		{
		};

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);
	}; // struct cmy_mixer_data

	struct __TUI_API cmy_cube_data : public any_picker_data
	{
		CDC m_dcCube;
		CSize m_szHalfSelBox;
		CRect m_rcCube;

		CPoint m_ptClrCurr;
		CPoint m_ptPlainRG;
		CPoint m_ptPlainRB;
		CPoint m_ptPlainGB;

		CPoint m_ptCubeR;
		CPoint m_ptCubeG;
		CPoint m_ptCubeB;
		CPoint m_ptCubeC;

		CPoint m_ptClrR;
		CPoint m_ptClrG;
		CPoint m_ptClrB;
		int m_nSideGap;

		cmy_cube_data();
		~cmy_cube_data();

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDone();
		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point);

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	private:
		void RecalcLayout();
		void ReleaseAll();

		void Generate(
			CDC &dc,
			CPalette *pPalette);
	}; // struct cmy_cube_data

	struct __TUI_API hsl_panel_data : public any_picker_data
	{
		CDC m_dcPicker;
		CDC m_dcSlider;

		BOOL m_bBmpValidPicker;
		BOOL m_bBmpValidSlider;

		CPoint m_ptMousePos;

		SIZE m_szColorPicker;
		SIZE m_szColorSlider;

		int m_nLumBarDy;
		int m_nAreaGap;

		hsl_panel_data();
		~hsl_panel_data();

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDone();
		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point);

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	private:
		void ReleaseAll();

	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette *pPalette);

		virtual void GeneratePicker(
			CDC &dc,
			CPalette *pPalette);

		void DrawCrossAt(
			CPoint &point,
			CDC &dc,
			SIZE &sz);
	}; // struct hsl_panel_data

	struct __TUI_API hsl_basic_hue_data : public hsl_panel_data
	{
		hsl_basic_hue_data()
		{
		};

		~hsl_basic_hue_data()
		{
		};

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette *pPalette);

		virtual void GeneratePicker(
			CDC &dc,
			CPalette *pPalette);
	}; // struct hsl_basic_hue_data

	struct __TUI_API hsl_roller_data : public hsl_panel_data
	{
		hsl_roller_data()
		{
			m_ptCenterSpaceHueSat.x = 0;
			m_ptCenterSpaceHueSat.y = 0;
			m_nSatTranslation = 1;
		};

		~hsl_roller_data()
		{
		};

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnLButtonDown(
			UINT nFlags,
			CPoint point);

		virtual void OnLButtonUp(
			UINT nFlags,
			CPoint point);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	protected:
		virtual void GeneratePicker(
			CDC &dc,
			CPalette *pPalette);

		CPoint m_ptCenterSpaceHueSat;
		CRgn m_rgnSpaceHueSat;
		CRgn m_rgnMouseSelHueSat;
		int m_nSatTranslation;
	}; // struct hsl_roller_data

	struct __TUI_API xyz_plain_data : public hsl_panel_data
	{
		enum eXYZmode
		{
			MXYZ_R = 0,
			MXYZ_G = 1,
			MXYZ_B = 2,
			MXYZ_C = 3,
			MXYZ_M = 4,
			MXYZ_Y = 5,
			MXYZ_MIN = 0,
			MXYZ_MAX = 5
		};

		eXYZmode m_nXYZmode;
		xyz_plain_data( eXYZmode nXYZmode) : m_nXYZmode(nXYZmode)
		{
		};

		~xyz_plain_data()
		{
		};

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnMouseMove(
			UINT nFlags,
			CPoint point);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	protected:
		virtual void GenerateSlider(
			CDC &dc,
			CPalette *pPalette);

		virtual void GeneratePicker(
			CDC &dc,
			CPalette *pPalette);

		COLORREF CalcColorPicker( int nX, int nY);
		COLORREF CalcColorSlider( int nZ);

		void CalcPoints(
			int &nX,
			int &nY,
			int &nZ,
			BOOL bDcScale);

		void RecalcByZ( int nZ);
	}; // struct xyz_plain_data

	struct __TUI_API diff_data : public grayscale_mixer_data
	{
		diff_data()
		{
		};

		~diff_data()
		{
		};

		virtual void OnInit(
			CDC &dc,
			CPalette *pPalette,
			CSize szTotal,
			BOOL bOnlySizeChanged);

		virtual void OnDraw(
			CDC &dc,
			CPalette *pPalette);

		virtual void OnSetRGB( COLORREF clr);
		virtual void OnSetHLS(
			double hue,
			double luminance,
			double saturation);

	private:
		void Generate( BOOL bOnlySizeChanged);
	}; // struct diff_data

public:
	selection_colors m_selClrs;

private:
	grayscale_mixer_data m_grayscale_mixer_data;
	rgb_mixer_data m_rgb_mixer_data;
	cell_based_data m_hexagon_data;
	rgb_cube_data m_rgb_cube_data;

	cmy_mixer_data m_cmy_mixer_data;
	cmy_cube_data m_cmy_cube_data;

	hsl_basic_hue_data m_hsl_basic_hue_data;
	hsl_roller_data m_hsl_roller_data;
	hsl_panel_data m_hsl_panel_data;

	xyz_plain_data m_xyz_r;
	xyz_plain_data m_xyz_g;
	xyz_plain_data m_xyz_b;
	xyz_plain_data m_xyz_c;
	xyz_plain_data m_xyz_m;
	xyz_plain_data m_xyz_y;
	diff_data m_diff_data;
	any_picker_data *m_algorithms[MODE_VAL_MAX + 1];

// Operations
public:
	void SetupColor(
		COLORREF clr,
		BOOL bNotifyChanged);

	operator e_mode() const
	{
		return m_nMode;
	};

	CTColorCtrl& operator=( e_mode nMode)
	{
		_Cleanup();

		m_nMode = nMode;
		_Init(FALSE);
		Invalidate();
		UpdateWindow();

		return *this;
	};

private:
	void _MouseTracking( BOOL bStart = TRUE);
	void _Init( BOOL bOnlySizeChanged);
	void _Cleanup();

public:
	static UINT m_nMsgColorChanged;

protected:
	virtual void NotifyColorChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTColorCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTColorCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTColorCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TCOLORCTRL_H)