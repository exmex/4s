#if (!defined __TCONTROLBAR_H)
#define __TCONTROLBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#if (!defined __AFXTEMPL_H__)
	#include<AfxTempl.h>
#endif

#include <afxpriv.h>

#define __TB_SEPARATOR_WIDTH__		6
#define __TB_TEXT_MARGINE__			5
#define __TB_BUTTON_MARGINE__		1
#define __TB_LINE_OFFSET			6

class CTBarContentExpandButton;
class CTDynamicControlBar;
class CTMiniDockFrameWnd;
class CTBarMdiDocButton;
class CTToolControlBar;
class CTMenuControlBar;
class CTDynamicDockBar;
class CTPopupMenuWnd;
class CTControlBar;
class CTBarButton;
class CTControlBar;
class CTDockBar;

typedef CArray< CTControlBar *, CTControlBar *>		ControlBarVector;
typedef CArray< CControlBar *, CControlBar *>		MFCControlBarVector;


/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButton

class __TUI_API CTBarNcAreaButton : public CObject
{
protected:
	CTControlBar *m_pBar;
	CRect m_rc;

	BOOL m_bPaintingCombinedArea:1;
	BOOL m_bDisabled:1;
	BOOL m_bPushed:1;
	BOOL m_bHover:1;

public:
	DECLARE_DYNAMIC(CTBarNcAreaButton)

	CTBarNcAreaButton( CTControlBar *pBar);
	~CTBarNcAreaButton();

	virtual void OnNcDestroy();
	operator CSize() const
	{
		return m_rc.Size();
	};

	operator const CRect &() const
	{
		return m_rc;
	};

	operator CRect &()
	{
		return m_rc;
	};

	BOOL IsBarFixedMode() const;
	BOOL IsBarVisible() const;

	BOOL IsBarFloated() const;
	BOOL IsBarDocked() const;

	BOOL IsBarDockedHorizontally() const;
	BOOL IsBarDockedVertically() const;
	BOOL IsBarDockedAtLeft() const;
	BOOL IsBarDockedAtRight() const;
	BOOL IsBarDockedAtTop() const;
	BOOL IsBarDockedAtBottom() const;

	BOOL GetBarNcAreaInfo(
		LPRECT pRectGripper = NULL,
		LPRECT pRectWnd = NULL,
		BOOL *pGripperAtTop = NULL,
		BOOL *pTextOnGripper = NULL) const;

	BOOL IsBarWndActive() const;
	BOOL IsBarSingleOnRow() const;
	BOOL IsBarMinimizedOnRow() const;
	BOOL IsBarMaximizedOnRow() const;
	BOOL IsBarFirstOnRow( BOOL bBar) const;
	BOOL IsBarLastOnRow( BOOL bBar) const;
	CFrameWnd *GetDockingFrame();

	CTControlBar *GetBar()
	{
		return m_pBar;
	}

	const CTControlBar *GetBar() const
	{
		return m_pBar;
	}

	virtual void OnNcAreaReposition( CTBarNcAreaButton *pPrevBtn);
	virtual BOOL OnQueryVisibility() const;

	void NcDrawDefault(
		CDC &dc,
		int nDockBtnGlyptT);

	virtual void OnNcAreaDraw( CDC &dc);
	virtual UINT OnNcAreaHitTest( CPoint point);
	virtual HCURSOR OnNcAreaQueryCursor( CPoint point);

	virtual BOOL OnNcAreaClicked( CPoint point);
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);

	int DoDefaultReflectForToolHitTest(
		TOOLINFO *pTI,
		LPCTSTR strTooltipText);

	friend class CTControlBar;
}; // class CTBarNcAreaButton


/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonExpand

class __TUI_API CTBarNcAreaButtonExpand : public CTBarNcAreaButton
{
public:
	DECLARE_DYNAMIC(CTBarNcAreaButtonExpand)
	CTBarNcAreaButtonExpand( CTControlBar *pBar);

	virtual BOOL OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC &dc);
	virtual UINT OnNcAreaHitTest( CPoint point);
	virtual BOOL OnNcAreaClicked( CPoint point);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarNcAreaButtonExpand


/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonMenu

class __TUI_API CTBarNcAreaButtonMenu : public CTBarNcAreaButton
{
public:
	DECLARE_DYNAMIC(CTBarNcAreaButtonMenu)
	CTBarNcAreaButtonMenu( CTControlBar *pBar);

	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC &dc,
		const CWnd &refWndMenu,
		const CRect &rcExcludeArea,
		int nCombineAlign);

	virtual BOOL OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC &dc);
	virtual UINT OnNcAreaHitTest( CPoint point);
	virtual BOOL OnNcAreaClicked( CPoint point);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarNcAreaButtonMenu


/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonClose

class __TUI_API CTBarNcAreaButtonClose : public CTBarNcAreaButton
{
public:
	DECLARE_DYNAMIC(CTBarNcAreaButtonClose)
	CTBarNcAreaButtonClose( CTControlBar *pBar);

	virtual BOOL OnQueryVisibility() const;
	virtual void OnNcAreaDraw( CDC &dc);
	virtual UINT OnNcAreaHitTest( CPoint point);
	virtual BOOL OnNcAreaClicked( CPoint point);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarNcAreaButtonClose


/////////////////////////////////////////////////////////////////////////////
// CTControlBar window

class __TUI_API CTControlBar : public CControlBar
{
	struct __TUI_API DockBarInfo
	{
		CMap< UINT, UINT, DWORD, DWORD> m_mapDockBarInfo;
		DockBarInfo();

		DWORD operator[]( UINT nDockBarID);
		BOOL ReplaceFrameDockBar(
			UINT nDockBarID,
			DWORD dwDockStyle,
			CFrameWnd * pFrame);
	};

	static DockBarInfo m_DockBarInfo;

public:
	class __TUI_API CDockStateBar : public CObject
	{
	public:
		UINT m_nDockBarID;
		UINT m_nCircleNo;
		UINT m_nMRUWidth;
		UINT m_nBarID;

		DWORD m_dwFloatFrameCreateStyle;
		DWORD m_dwDockStateFlags;
		DWORD m_dwMRUFloatStyle;
		DWORD m_dwBarFlags;

		CPoint m_ptMRUFloatPos;
		CPoint m_ptFloatPos;

		LONG m_nDelayedRowUpdateMetric;

		CRect m_rcMRUDockPos;
		CRect m_rcFloatFrame;
		CRect m_rcBar;

		CSize m_szDockedH;
		CSize m_szDockedV;
		CSize m_szFloated;

		CArray< CDockStateBar *, CDockStateBar *> m_arrChildStates;
		CTControlBar *m_pHelperBar;
		LONG m_nAciveTabChildIndex;

		// m_dwDockStateFlags
		enum
		{
			__DOCK_AT_NOWHERE				= 0x00000000,
			__DOCK_AT_SIDE					= 0x00000001,
			__DOCK_DYNAMIC_HALF_SPLIT		= 0x00000002,
			__DOCK_AT_MASK					= 0x0000000F,
			__STATE_VISIBLE					= 0x00000010,
			__STATE_FLOATING				= 0x00000020,
			__STATE_NEW_ROW					= 0x00000040,
			__STATE_DYNAMIC_CHILD			= 0x00000080
		};

		CDockStateBar();
		~CDockStateBar();

		void InitMembers();
		void Clear();

		void StateGet(
			CTControlBar *pBar,
			CDockStateBar *pParentState);

		void StateSet(
			CFrameWnd *pDockSite,
			CDockStateBar *pParentState);

		void PreSyncAttributes(
			CFrameWnd *pDockSite,
			CDockStateBar *pParentState);

		void PostSyncAttributes(
			CFrameWnd *pDockSite,
			CDockStateBar *pParentState);
	};

	struct ItemData
	{
		CPoint m_ptFloatHelper;
		CSize m_szDockedH;
		CSize m_szDockedV;
		CSize m_szFloated;
		UINT m_nBarID;

		ItemData() : m_nBarID(AFX_IDW_DOCKBAR_FLOAT),
			m_ptFloatHelper( 0, 0),
			m_szDockedH( 200, 200),
			m_szDockedV( 200, 200),
			m_szFloated( 200, 200)
		{
		}

		ItemData( const ItemData& other)
		{
			*this = other;
		}

		ItemData& operator=( const ItemData& other)
		{
			m_ptFloatHelper = other.m_ptFloatHelper;
			m_szDockedH = other.m_szDockedH;
			m_szDockedV = other.m_szDockedV;
			m_szFloated = other.m_szFloated;
			m_nBarID = other.m_nBarID;

			return *this;
		}
	};

	typedef CArray< ItemData, ItemData&> PropsArr;
	class __TUI_API CDockStateSite : public CObject
	{
	public:
		CArray< CDockStateBar *, CDockStateBar *> m_arrBarStates;
		PropsArr m_arrProps;

		CFrameWnd *m_pDockSite;
		CDockState *m_pState;
		WINDOWPLACEMENT m_wp;

		CDockStateSite( CFrameWnd *pDockSite);
		~CDockStateSite();

		void StateSet() const;
		void StateGet();
		void Clear();

		CDockStateBar* AppendFloatingLayout( CTMiniDockFrameWnd *pFrame);
		CDockStateBar* AppendDockBarAtSideLayout( CTDockBar *pDockBar);
	};

private:
	class __TUI_API CFriendlyFrameWnd : public CFrameWnd
	{
		void SetDockState(
			const CDockState& state,
			const PropsArr& arrProps);

		void GetDockState(
			CDockState& state,
			PropsArr& arrProps) const;

	public:
		BOOL SetupSmilyFrameWnd(void);

		void GetDockState( CDockStateSite& state) const;
		void SetDockState( const CDockStateSite& state);

		friend class CDockStateSite;
	};

	class __TUI_API CFriendlyDockBar : CDockBar
	{
	public:
		friend class DraggingState;
		friend class CTControlBar;

		int GetCalcExtentMax( BOOL bHorz)
		{
			int nCalcExtentMax = bHorz ? m_rectLayout.Width() : m_rectLayout.Height();

			if( nCalcExtentMax == 0 || m_rectLayout.IsRectEmpty() )
			{
				CRect rcDock;

				GetClientRect(&rcDock);
				nCalcExtentMax = bHorz ? rcDock.Width() : rcDock.Height();

				if( nCalcExtentMax <= 0 )
					return 3;
			}

			return nCalcExtentMax;
		}

		void _InsertBar( int nPos, CControlBar *pBar)
		{
			m_arrBars.InsertAt( nPos, pBar);
		}

		void _InsertRowSplitter( int nPos)
		{
			_InsertBar( nPos, NULL);
		}
	};

	struct __TUI_API BarRepositionData
	{
		CControlBar *m_pBar;
		CRect m_rcSrc;
		CRect m_rcDst;

		int m_nSpaceAvailBySideBefore;
		int m_nSpaceAvailBySideAfter;
		int	m_nSpaceAvailByBar;

		void SetBar(
			CControlBar *pBar,
			BOOL bSetAdvData = FALSE);

		BarRepositionData(
			CControlBar * pBar = NULL,
			BOOL bSetAdvData = FALSE) : m_pBar(NULL),
			m_rcSrc( 0, 0, 0, 0),
			m_rcDst( 0, 0, 0, 0),
			m_nSpaceAvailByBar(0),
			m_nSpaceAvailBySideBefore(0),
			m_nSpaceAvailBySideAfter(0)
		{
			if(pBar)
				SetBar( pBar, bSetAdvData);
		}

		BarRepositionData( const BarRepositionData& other)
		{
			*this = other;
		}

		BarRepositionData& operator=( const BarRepositionData& other)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
			m_pBar = other.m_pBar;

			m_nSpaceAvailBySideBefore = other.m_nSpaceAvailBySideBefore;
			m_nSpaceAvailBySideAfter = other.m_nSpaceAvailBySideAfter;
			m_nSpaceAvailByBar = other.m_nSpaceAvailByBar;

			return *this;
		}

		BOOL IsRectsEqual() const
		{
			return m_rcSrc == m_rcDst;
		}

		CTControlBar *GetBar();
		CTControlBar *GetBarFixed();
	}; // struct BarRepositionData

// Construction
public:
	DECLARE_DYNAMIC(CTControlBar)
	CTControlBar();

// Attributes
public:
	static const UINT m_nMsgPutToPopupMenu;
	static const UINT m_nMsgQueryIcon;
	static BOOL m_bEnableOnIdleCalls;

	BOOL m_bDoNotEraseClientBackground:TRUE;
	BOOL m_bInCustomModeUpdateCmdUI:TRUE;
	BOOL m_bGripperStaticallyAtTop:TRUE;
	BOOL m_bPresubclassDialogMode:TRUE;
	BOOL m_bAllowContractExpand:TRUE;
	BOOL m_bReflectParentSizing:TRUE;

protected:
	BOOL m_bFixedMode:TRUE;
	int m_nGripHeightAtTop;
	int m_nGripWidthAtLeft;
	int m_nSeparatorHeight;
	int m_nSeparatorWidth;
	int m_nTrackerOffset;
	int m_nMetricOffset;
	int m_nMinHW;
	int m_nMinVH;

protected:
	static ControlBarVector m_AllBars;

	CRect m_rcRowRecalcUp;
	CRect m_rcRowResize;
	CRect m_rcRowRecalc;
	CSize m_szDockedH;
	CSize m_szDockedV;
	CSize m_szFloated;
	CRect m_rcGrip;

	BOOL m_bReposSingleChildMode:TRUE;
	BOOL m_bRowRecalcing:TRUE;
	BOOL m_bRowResizing:TRUE;
	BOOL m_bDragging:TRUE;

protected:
	BOOL m_bUpdatingChain:TRUE;

protected:
	LONG m_nDelayedRowUpdateMetric;

	CPoint m_ptHelperDragOffsetFloated;
	CPoint m_ptHelperDragOffsetDockedH;
	CPoint m_ptHelperDragOffsetDockedV;
	CPoint m_ptDraggingLast;
	CPoint m_ptFloatHelper;

	BOOL m_bNcAreaBtnTracking:TRUE;
	BOOL m_bWindowActive:TRUE;
	BOOL m_bTopRecalcing:TRUE;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTControlBar)
	public:
	virtual BOOL PreTranslateMessage( MSG* pMsg);
	protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTControlBar();

	void SetInitDesiredSizeVertical( CSize size)
	{
		m_szDockedV = size;
	};

	void SetInitDesiredSizeHorizontal( CSize size)
	{
		m_szDockedH = size;
	};

	void SetInitDesiredSizeFloating( CSize size)
	{
		m_szFloated = size;
	};

	virtual BOOL Create(
		LPCTSTR lpszWindowName,
		CWnd *pParentWnd,
		UINT nID = AFX_IDW_DIALOGBAR,
		DWORD dwStyle = WS_CHILD|
		WS_VISIBLE|
		CBRS_TOP|
		CBRS_GRIPPER|
		CBRS_TOOLTIPS|
		CBRS_FLYBY|
		CBRS_SIZE_DYNAMIC|
		CBRS_HIDE_INPLACE);

	static BOOL IsOleIpObjActive( CFrameWnd *pFrameSearch = NULL);
	virtual BOOL SafeDisplayBar();

protected:
	static BOOL _SetupFloatingFrameWndImpl( CFrameWnd *pFrame);
	static BOOL _FrameEnableDockingImpl(
		CFrameWnd * pFrame,
		DWORD dwDockStyle = CBRS_ALIGN_ANY,
		BOOL bReplaceFloatingWnd = TRUE);

public:
	static BOOL FrameEnableDocking(
		CFrameWnd * pFrame,
		DWORD dwDockStyle = CBRS_ALIGN_ANY,
		BOOL bReplaceFloatingWnd = TRUE);

	BOOL IsSingleOnRow(
		BOOL bOnlyFixed = FALSE,
		BOOL bOnlyNonFixed = FALSE) const;

	BOOL IsMinimizedOnRow() const;
	BOOL IsMaximizedOnRow() const;
	void MakeSingleOnRow();
	void MaximizeOnRow();
	void MinimizeOnRow();

	virtual BOOL IsFixedMode() const;

	UINT GetSafeDockBarDlgCtrlID() const;
	BOOL IsDockedHorizontally() const;
	BOOL IsDockedVertically() const;
	BOOL IsDockedAtTop() const;
	BOOL IsDockedAtBottom() const;
	BOOL IsDockedAtLeft() const;
	BOOL IsDockedAtRight() const;
	BOOL IsDocked() const;
	BOOL IsFloating() const;

	virtual BOOL IsBarWithGripper(
		BOOL *pbGripperAtTop = NULL,
		BOOL *pbTextOnGripper = NULL) const;

	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz);

	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD nMode);

	virtual void DoPaint( CDC *pDC);
	virtual void OnUpdateCmdUI(
		CFrameWnd *pTarget,
		BOOL bDisableIfNoHndler);

	static BOOL DoCustomModePreTranslateMessage(
		CWnd *pWndPretranslateSrc,
		MSG *pMsg);

	static void DoCustomModeUpdateControlBars(
		CWnd *pWndParentTarget,
		BOOL bDisableIfNoHndler = TRUE);

	BOOL DoCustomModeUpdateCmdUI( BOOL bDisableIfNoHndler = TRUE);

protected:
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO * pTI) const;

	virtual CSize _CalcDesiredMinFloatedSize() const;
	virtual int _CalcDesiredMinHW() const;
	virtual int _CalcDesiredMinVH() const;

private:
	CSize _CalcDesiredLayout(
		BOOL bHorz,
		BOOL bForceFullRow = FALSE);

protected:
	enum eGFECB_t
	{
		__GFECB_FIXED				= 0x00000001L, // fixed sized
		__GFECB_NONFIXED_MIN		= 0x00000002L, // resizable and minimized on row
		__GFECB_NONFIXED_NONMIN		= 0x00000004L, // resizable and not minimized on row
		__GFECB_NONFIXED			= (__GFECB_NONFIXED_MIN|__GFECB_NONFIXED_NONMIN),

		__GFECB_DOCKED_TOP			= 0x00000008L,
		__GFECB_DOCKED_BOTTOM		= 0x00000010L,
		__GFECB_DOCKED_LEFT			= 0x00000020L,
		__GFECB_DOCKED_RIGHT		= 0x00000040L,

		__GFECB_DOCKED_HORZ			= (__GFECB_DOCKED_TOP|__GFECB_DOCKED_BOTTOM),
		__GFECB_DOCKED_VERT			= (__GFECB_DOCKED_LEFT|__GFECB_DOCKED_RIGHT),
		__GFECB_DOCKED				= (__GFECB_DOCKED_HORZ|__GFECB_DOCKED_VERT),

		__GFECB_FLOATED				= 0x00000080L,

		__GFECB_BAR_VISIBLE			= 0x00000100L,
		__GFECB_BAR_INVISIBLE		= 0x00000200L,
		__GFECB_WND_VISIBLE			= 0x00000400L,
		__GFECB_WND_INVISIBLE		= 0x00000800L,

		__GFECB_VIS_BAR_ALL			= (__GFECB_BAR_VISIBLE|__GFECB_BAR_INVISIBLE),
		__GFECB_VIS_WND_ALL			= (__GFECB_WND_VISIBLE|__GFECB_WND_INVISIBLE),

		__GFECB_VIS_ALL				= (__GFECB_BAR_VISIBLE|__GFECB_WND_VISIBLE),
		__GFECB_INVIS_ALL			= (__GFECB_BAR_INVISIBLE|__GFECB_WND_INVISIBLE),
		__GFECB_ALL_NONRESTRICT		= (__GFECB_FIXED|__GFECB_NONFIXED|__GFECB_DOCKED|__GFECB_FLOATED|__GFECB_VIS_ALL|__GFECB_INVIS_ALL),

		// restriction by point
		__GFECB_BYPT_ONLY_WND		= 0x00008000L, // abondones all other __GFECB_BYPT_ flags
		__GFECB_BYPT_CLIENT			= 0x00010000L,
		__GFECB_BYPT_NCGRIP			= 0x00020000L,

		__GFECB_BYPT_HTLEFT			= 0x00040000L,
		__GFECB_BYPT_HTRIGHT		= 0x00080000L,
		__GFECB_BYPT_HTTOP			= 0x00100000L,
		__GFECB_BYPT_HTBOTTOM		= 0x00200000L,
		__GFECB_BYPT_HTTOPLEFT		= 0x00400000L,
		__GFECB_BYPT_HTTOPRIGHT		= 0x00800000L,
		__GFECB_BYPT_HTBOTTOMLEFT	= 0x01000000L,
		__GFECB_BYPT_HTBOTTOMRIGHT	= 0x02000000L,

		__GFECB_BYPT_HT_CORNER		= (__GFECB_BYPT_HTLEFT|__GFECB_BYPT_HTRIGHT|__GFECB_BYPT_HTTOP|__GFECB_BYPT_HTBOTTOM),
		__GFECB_BYPT_HT_NONCORNER	= (__GFECB_BYPT_HTTOPLEFT|__GFECB_BYPT_HTTOPRIGHT|__GFECB_BYPT_HTBOTTOMLEFT|__GFECB_BYPT_HTBOTTOMRIGHT),

		__GFECB_BYPT_HTNOWHERE		= 0x04000000L,
		__GFECB_BYPT_HTSYSMENU		= 0x06000000L,

		__GFECB_BYPT_HTGROWBOX		= 0x10000000L,
		__GFECB_BYPT_HTMINBUTTON	= 0x20000000L,
		__GFECB_BYPT_HTMAXBUTTON	= 0x40000000L,
		__GFECB_BYPT_HT_BUTTONS		= (__GFECB_BYPT_HTGROWBOX|__GFECB_BYPT_HTMINBUTTON|__GFECB_BYPT_HTMAXBUTTON),

		__GFECB_BYPT_HT_BORDER		= (__GFECB_BYPT_HT_CORNER|__GFECB_BYPT_HT_NONCORNER),
		__GFECB_BYPT_HT_NONBORDER	= (__GFECB_BYPT_HT_BUTTONS|__GFECB_BYPT_HTNOWHERE|__GFECB_BYPT_HTSYSMENU),
		__GFECB_BYPT_HTNONCLIENT	= (__GFECB_BYPT_HT_BORDER|__GFECB_BYPT_HT_NONBORDER),

		__GFECB_BYPT_HTCLIENT		= 0x80000000L,

		__GFECB_BYPT_NC				= (__GFECB_BYPT_NCGRIP|__GFECB_BYPT_HTNONCLIENT|__GFECB_BYPT_HTCLIENT),
		__GFECB_BYPT_ALL			= (__GFECB_BYPT_ONLY_WND|__GFECB_BYPT_NC|__GFECB_BYPT_CLIENT),
	}; // enum eGFECB_t

	static void _GetFrameControlBars(
		CFrameWnd *pFrame,
		ControlBarVector& vBars,
		DWORD dwCallOpt = ((DWORD)__GFECB_ALL_NONRESTRICT),
		CPoint ptRestrict = CPoint( -1000, -1000));

	void _GetFrameControlBars(
		ControlBarVector& vBars,
		DWORD dwCallOpt = ((DWORD)__GFECB_ALL_NONRESTRICT),
		CPoint ptRestrict = CPoint( -1000, -1000)) const;

	static CControlBar* _GetNextControlBarInRow(
		CControlBar *pBar,
		BOOL bNext = TRUE,
		BOOL bBar = FALSE,
		BOOL bNonFixed = FALSE);

	static CControlBar* _GetFirstControlBarInRow(
		CControlBar *pBar,
		BOOL bNext = FALSE,
		BOOL bBar = FALSE,
		BOOL bNonFixed = FALSE);

	void _GetRowBars(
		MFCControlBarVector& vBars,
		CRect *pRcAllBars = NULL,
		int *pOwnPos = NULL,
		BOOL *pRowMaxSized = NULL,
		BOOL *pRowFullSized = NULL) const;

	void _GetRowBars(
		ControlBarVector& vBars,
		CRect *pRcAllBars = NULL,
		int *pOwnPos = NULL,
		BOOL *pRowMaxSized = NULL,
		BOOL *pRowFullSized = NULL) const;

	void _GetNextRowBars(
		BOOL bNextRow,
		MFCControlBarVector& vBars,
		CRect *pAllBars = NULL,
		int *pOwnPos = NULL,
		BOOL *pRowMaxSized = NULL,
		BOOL *pRowFullSized = NULL) const;

	BOOL _IsFirstBarInRow( BOOL bBar) const;
	BOOL _IsLastBarInRow( BOOL bBar) const;

	static CFrameWnd* _GetDockingFrameImpl( CControlBar *pBar);
	CFrameWnd* _GetDockingFrameImpl() const;

	virtual void _RecalcLayoutImpl();
	virtual void _RecalcNcArea();

protected:
	virtual BOOL _IsShowContentWhenRowRecalcing() const;
	virtual BOOL _IsShowContentWhenRowResizing() const;
	virtual BOOL _IsShowContentWhenDragging() const;

public:
	virtual void OnRepositionSingleChild(
		int cx = -1,
		int cy = -1,
		BOOL bRedraw = TRUE);

protected:
	void _RowResizingStop( BOOL bCancel);
	void _RowResizingUpdateState();
	void _RowResizingStart();

	void _RowRecalcingStop( BOOL bCancel);
	void _RowRecalcingUpdateState();
	void _RowRecalcingStart();

	BOOL _RowRecalcing_IsBottomEnabled();
	BOOL _RowRecalcing_IsTopEnabled();
	void _DrawRecalcingTracker();
	void _DrawResizingTracker();

	enum e_calc_dock_type
	{
		__ECDT_FLOATED				=  0,
		__ECDT_INNER_TOP			=  1,
		__ECDT_INNER_BOTTOM			=  2,
		__ECDT_INNER_LEFT			=  3,
		__ECDT_INNER_RIGHT			=  4,
		__ECDT_DOCKBAR_INNER_TOP	=  5,
		__ECDT_DOCKBAR_INNER_BOTTOM	=  6,
		__ECDT_DOCKBAR_INNER_LEFT	=  7,
		__ECDT_DOCKBAR_INNER_RIGHT	=  8,
		__ECDT_CIRCLE1_OUTER_TOP	=  9,
		__ECDT_CIRCLE1_OUTER_BOTTOM	= 10,
		__ECDT_CIRCLE1_OUTER_LEFT	= 11,
		__ECDT_CIRCLE1_OUTER_RIGHT	= 12,
		__ECDT_OWN_AREA				= 13
	};

	class __TUI_API DraggingState : public CObject
	{
		BOOL m_bDeepHalfSplit:TRUE;
		BOOL m_bEmpty:TRUE;
		UINT m_nCircleNo;

		e_calc_dock_type m_nCDT;
		CRect m_rcTrack;

		CTControlBar *m_pBarSrc;
		CTControlBar *m_pBarDst;

		void _AssignFromOther( const DraggingState& other);
		BOOL _IsEqualToOther( const DraggingState& other) const
		{
			return m_bEmpty == other.m_bEmpty &&
				m_nCDT == other.m_nCDT &&
				m_nCircleNo == other.m_nCircleNo &&
				m_rcTrack == other.m_rcTrack &&
				m_pBarSrc == other.m_pBarSrc &&
				m_pBarDst == other.m_pBarDst &&
				m_bDeepHalfSplit == other.m_bDeepHalfSplit;
		}

	public:
		DraggingState( CTControlBar *pBarSrc = NULL)
			: m_bEmpty(TRUE),
			m_bDeepHalfSplit(FALSE),
			m_nCDT(__ECDT_FLOATED),
			m_nCircleNo(0),
			m_rcTrack( 0, 0, 0, 0),
			m_pBarSrc(pBarSrc),
			m_pBarDst(NULL)
		{
		}

		DraggingState( const DraggingState& other)
		{
			_AssignFromOther(other);
		}

		DraggingState& operator=( const DraggingState& other)
		{
			_AssignFromOther(other);
			return *this;
		}

		BOOL operator==( const DraggingState& other) const
		{
			return _IsEqualToOther(other);
		}

		BOOL operator!=( const DraggingState& other) const
		{
			return !_IsEqualToOther(other);
		}

		e_calc_dock_type GetCDT() const
		{
			return m_nCDT;
		};

		e_calc_dock_type SetCDT( e_calc_dock_type nCDT)
		{
			e_calc_dock_type nOldCDT = m_nCDT;
			m_nCDT = nCDT;

			return nOldCDT;
		};

		UINT GetCircleNo() const
		{
			return m_nCircleNo;
		};

		UINT SetCircleNo( UINT nCircleNo)
		{
			UINT nCircleNoOld = m_nCircleNo;

			m_nCircleNo = nCircleNo;
			return nCircleNoOld;
		};

		CRect& TrackRectGet()
		{
			return m_rcTrack;
		};

		const CRect& TrackRectGet() const
		{
			return m_rcTrack;
		};

		void TrackRectGet( CRect& rcTrack) const
		{
			rcTrack = m_rcTrack;
		};

		void TrackRectSet( const CRect& rcTrack)
		{
			m_rcTrack = rcTrack;
		};

		void TrackRectSetEmpty()
		{
			m_rcTrack.SetRectEmpty();
		};

		BOOL IsEmpty() const
		{
			return m_bEmpty;
		}

		void SetEmpty( BOOL bEmpty = TRUE);
		BOOL IsDeepHalfSplit() const
		{
			return m_bDeepHalfSplit;
		}

		void SetDeepHalfSplit( BOOL bDeepHalfSplit = TRUE)
		{
			m_bDeepHalfSplit = bDeepHalfSplit;
		}

		CTControlBar* BarSrcGet()
		{
			return m_pBarSrc;
		}

		CTControlBar* BarDstGet()
		{
			return m_pBarDst;
		}

		CTControlBar* BarSrcSet( CTControlBar *pBarSrc)
		{
			CTControlBar *pBarSrcOld = m_pBarSrc;

			m_pBarSrc = pBarSrc;
			return pBarSrcOld;
		}

		CTControlBar* BarDstSet(
			CTControlBar *pBarDst = NULL,
			BOOL bNoCheckOnShutdown = FALSE);

		static void AlignFixedRow( MFCControlBarVector& vRowBars);
		void CalcStateDynamic( CPoint ptTrack);
		void CalcStateFixed( CPoint ptTrack);
		void DrawState();
	}; // class DraggingState

	friend class DraggingState;

	static DraggingState m_DragStateOld;
	static DraggingState m_DragStateNew;
	static BOOL m_bUpdatingDragState;

	virtual BOOL _DraggingApplyState( CTControlBar::DraggingState& _ds);
	virtual void _DraggingDoDeepHalfSplit(
		CTControlBar *pBar,
		UINT nHalfSplitID);

	static CTControlBar* _DraggingGetBar();
	static BOOL _DraggingCancel();

	virtual void _DraggingStart(
		const CPoint& point,
		const CPoint& pointOffset = CPoint( 0, 0));

	virtual void _DraggingUpdateState( const CPoint& point);
	virtual void _DraggingStop( BOOL bCancel);

	void _CompressRowSpace(
		int nSpaceNeeded,
		MFCControlBarVector& arrPrevBars);

	virtual void _Dragging_OnStart();
	virtual void _Dragging_OnStop(
		DraggingState& _dsOld,
		DraggingState& _dsNew,
		BOOL bCancel);

	void _SetCursor( const CPoint& point);
	void _ContextMenuTrack();

	BOOL _IsSingleVisibleInFloatingPalette();
	void _GetDeepNcModeMetrics(
		BOOL& bEnableResizerLeft,
		BOOL& bEnableResizerTop,
		BOOL& bEnableResizerRight,
		BOOL& bEnableResizerBottom);

	void _UpdateVisibilityInChain();
	void _UpdateVisibilityInRow();
	void _ActivateOnClick();

public:
	UINT GetDockBarCircleNo() const;

protected:
	virtual BOOL _CanDockToTabbedContainers() const;
	virtual BOOL _CanDockToInnerCircles() const;
	void _PreSyncRowMetrics();

	virtual void OnFrameBarCheckCmd( BOOL bResizableCheckmarks = FALSE);
	virtual void OnFrameBarCheckUpdate(
		CCmdUI* pCmdUI,
		BOOL bResizableCheckmarks = FALSE);

public:
	static BOOL DoFrameBarCheckCmd(
		CFrameWnd *pFrame,
		UINT nBarID,
		BOOL bResizableCheckmarks = FALSE);

	static void DoFrameBarCheckUpdate(
		CFrameWnd *pFrame,
		CCmdUI *pCmdUI,
		BOOL bResizableCheckmarks = FALSE);

	virtual CTMiniDockFrameWnd* OnCreateFloatingFrame( DWORD dwStyle);
	virtual void ToggleDocking();

	void FloatControlBar( DWORD dwStyle = CBRS_ALIGN_TOP);
	virtual void FloatControlBar(
		CPoint ptFloat,
		DWORD dwStyle = CBRS_ALIGN_TOP);

	virtual BOOL DockControlBar(
		UINT nDockBarID,
		UINT nCircleNo,
		CFrameWnd *pDockSite = NULL,
		BOOL bRecalcLayout = TRUE);

	virtual BOOL DockControlBar(
		CTControlBar* pBar,
		BOOL bHorzBarHalfSplit,
		BOOL bInnerHalfSplit = FALSE,
		CFrameWnd* pDockSite = NULL,
		BOOL bRecalcLayout = TRUE);

private:
	typedef CArray< CTBarNcAreaButton *, CTBarNcAreaButton *> NcButtonsArr;
	NcButtonsArr m_vNcButtons;

protected:
	virtual CRect& NcButtons_CalcGripperTextLocation( CRect& rcPreCalcText);
	UINT NcButtons_HitTest(
		CPoint point,
		CTBarNcAreaButton **ppBtn = NULL);

	void NcButtons_Paint( CDC& dc);

public:
	int NcButtons_GetCount() const
	{
		return (int) m_vNcButtons.GetSize();
	}

	CTBarNcAreaButton* NcButtons_GetAt( int nIndex)
	{
		return m_vNcButtons.GetAt(nIndex);
	}

	void NcButtons_SetAt( int nIndex, CTBarNcAreaButton *pBtn)
	{
		if( nIndex < 0 )
		{
			m_vNcButtons.Add(pBtn);
			return;
		}

		if( nIndex != m_vNcButtons.GetSize() )
		{
			CTBarNcAreaButton *pBtnOld = m_vNcButtons.GetAt(nIndex);

			if( pBtnOld != pBtn )
			{
				pBtnOld->OnNcDestroy();

				m_vNcButtons.SetAt(
					nIndex,
					pBtn);
			}

			return;
		}

		m_vNcButtons.Add(pBtn);
	}

	void NcButtons_Add( CTBarNcAreaButton *pBtn)
	{
		NcButtons_SetAt( -1, pBtn);
	}

	void NcButtons_RemoveAt( int nIndex)
	{
		CTBarNcAreaButton *pBtnOld = m_vNcButtons.GetAt(nIndex);
		pBtnOld->OnNcDestroy();
	}

	void NcButtons_RemoveAll()
	{
		int nCount = (int) m_vNcButtons.GetSize();

		for( int i=0; i<nCount; i++)
		{
			CTBarNcAreaButton *pBtnOld = m_vNcButtons.GetAt(i);
			pBtnOld->OnNcDestroy();
		}

		m_vNcButtons.RemoveAll();
		m_vNcButtons.FreeExtra();
	}

	virtual void OnNcAreaButtonsReinitialize();
	virtual void OnNcAreaButtonsReposition();

	static BOOL m_bControlBarClassRegistered;
	static BOOL RegisterControlBarClass();

	static void RepositionBarsEx(
		CWnd *pWndToReposChilds,
		UINT nIDFirst,
		UINT nIDLast,
		UINT nIDLeftOver,
		UINT nFlag = 0,
		LPRECT lpRectParam = NULL,
		LPCRECT lpRectClient = NULL,
		BOOL bStretch = TRUE);

protected:
	//{{AFX_MSG(CTControlBar)
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	friend class CTBarButton;
	friend class CTBarContentExpandButton;
	friend class CTBarMdiDocButton;
	friend class CTDockBar;
	friend class CTToolControlBar;
	friend class CTMenuControlBar;
	friend class CTMiniDockFrameWnd;
	friend class CTBarNcAreaButton;
	friend struct BarRepositionData;
	friend class CTDynamicControlBar;
	friend class CTDynamicDockBar;

	friend class DockStateBar;
	friend class DockStateSite;
	friend class FriendlyFrameWnd;
};

#define __TCONTROLBAR_CLASS_NAME  _T("TUI-ControlBar")


#endif // #if (!defined __TCONTROLBAR_H)