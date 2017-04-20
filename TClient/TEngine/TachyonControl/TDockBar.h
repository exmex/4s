#if (!defined __TDOCKBAR_H)
#define __TDOCKBAR_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#if (!defined __TCONTROLBAR_H)
	#include <TControlBar.h>
#endif

#if (!defined __TTOOLCONTROLBAR_H)
	#include <TToolControlBar.h>
#endif

#if (!defined __TMENUCONTROLBAR_H)
	#include <TMenuControlBar.h>
#endif

#if (!defined __TPOPUP_MENU_WND_H)
	#include <TPopupMenuWnd.h>
#endif

#if (!defined __TPAINT_MANAGER_H)
	#include <TPaintManager.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTDynamicControlBar;
class CTDynamicDockBar;
class CTDockBar;

/////////////////////////////////////////////////////////////////////////
// CTDockBar window

class __TUI_API CTDockBar : public CDockBar
{
	friend class CTBarButton;
	friend class CTBarContentExpandButton;
	friend class CTControlBar;
	friend class CTControlBar::CDockStateBar;
	friend class CTControlBar::CDockStateSite;
	friend class CTToolControlBar;
	friend class CTMiniDockFrameWnd;
	friend class CTDynamicControlBar;

	DECLARE_DYNAMIC(CTDockBar)

protected:
	static BOOL m_bControlBarFixSizePixel;
	BOOL m_bLockedOptimize:1;
	UINT m_nCircleNo;

	CTDockBar *m_pDockBarOuter;
	CTDockBar *m_pDockBarInner;
	CRect m_rcLastInvisiblePreCalc;

public:
	CTDockBar( UINT nCircleNo);

	UINT _GetCircleNo() const
	{
		return m_nCircleNo;
	}

	CTDockBar *_GetDockBarInner()
	{
		return m_pDockBarInner;
	}

	const CTDockBar *_GetDockBarInner() const
	{
		return m_pDockBarInner;
	}

	CTDockBar *_GetDockBarOuter()
	{
		return m_pDockBarOuter;
	}

	const CTDockBar *_GetDockBarOuter() const
	{
		return m_pDockBarOuter;
	}

	CTDockBar *_GetDockBarInnerTop()
	{
		if(!m_pDockBarInner)
			return this;

		CTDockBar *pBar = m_pDockBarInner;
		while(pBar->m_pDockBarInner)
			pBar = pBar->m_pDockBarInner;

		return pBar;
	}

	const CTDockBar *_GetDockBarInnerTop() const
	{
		if(!m_pDockBarInner)
			return this;

		CTDockBar *pBar = m_pDockBarInner;
		while(pBar->m_pDockBarInner)
			pBar = pBar->m_pDockBarInner;

		return pBar;
	}

	CTDockBar *_GetDockBarOuterTop()
	{
		if(!m_pDockBarOuter)
			return this;

		CTDockBar *pBar = m_pDockBarOuter;
		while(pBar->m_pDockBarOuter)
			pBar = pBar->m_pDockBarOuter;

		return pBar;
	}

	const CTDockBar *_GetDockBarOuterTop() const
	{
		if(!m_pDockBarOuter)
			return this;

		CTDockBar *pBar = m_pDockBarOuter;
		while(pBar->m_pDockBarOuter)
			pBar = pBar->m_pDockBarOuter;

		return pBar;
	}

	CTDockBar *_GetBarByCircleNo( UINT nCircleNo);
	const CRect &_GetLastInvisiblePreCalcRect() const
	{
		return m_rcLastInvisiblePreCalc;
	}

	void _GetPreCalcWindowRect( CRect& rcPreCalc)
	{
		rcPreCalc = _GetLastInvisiblePreCalcRect();
	}

	CTDockBar *_GetInCircle(
		UINT nDockBarID,
		BOOL bEnableCreateNew = TRUE);

	static UINT _CreateInnerCircle( CFrameWnd *pFrame);
	static void _OptimizeCircles( CFrameWnd *pFrame);

	static CTDockBar *_GetInCircle(
		CFrameWnd *pFrame,
		UINT nCircleNo,
		UINT nDockBarID,
		BOOL bEnableCreateNew = TRUE);

	static void _InjectCircle(
		CFrameWnd *pFrame,
		UINT nCircleNo);

	static void _RemovePlaceHolder(
		CFrameWnd *pFrame,
		CControlBar *pBar,
		BOOL *pRemoved = NULL);

	static void _ContextMenuBuild(
		CFrameWnd *pFrame,
		CTPopupMenuWnd *pPopup);

	void _InjectCircle( UINT nCircleNo);
	void _InjectInnerCircle();
	UINT _CreateInnerCircle();

	static BOOL m_bExtendedRepositioning;

	CControlBar *_GetDockedControlBar( int nPos) const
	{
		return CDockBar::GetDockedControlBar(nPos);
	}

	void RemoveAllPlaceHolders( BOOL bSearchPlaceHolder = TRUE);
	void RemovePlaceHolder(
		CControlBar *pBar,
		BOOL bSearchPlaceHolder = TRUE,
		BOOL *pRemoved = NULL);

	void _SlideDockControlBar(
		CControlBar* pBar,
		LPCRECT lpRect,
		BOOL bMovingEnabled,
		const POINT *ptDesiredMid = NULL);

	void _NewRowDockControlBar(
		CControlBar *pBarDocked,
		CControlBar *pBarNew,
		BOOL bLessIndex);

	void _InnerOuterDockControlBar(
		CControlBar *pBar,
		BOOL bInner);

	virtual void _LockSequenceOptimization( BOOL bLock);
	void DockControlBar(
		CControlBar *pBar,
		LPCRECT lpRect = NULL);

	BOOL _CanBeSafeOptimized();

	void _SafeOptimizeInnerOuterChain();
	void _OptimizeCircles();

	CFrameWnd *_GetDockingFrameImpl();

	BOOL RemoveControlBar(
		CControlBar *pBar,
		int nPosExclude = -1,
		int nAddPlaceHolder = 0,
		BOOL bEnableFrameDelayRecalcLayout = TRUE);

protected:
	void _ContextMenuTrack();

	int _InsertByPointImpl(
		CControlBar *pBar,
		CRect rect,
		CPoint ptMid);

	int Insert(
		CControlBar *pBar,
		CRect rect,
		CPoint ptMid);

protected:
	static BOOL m_bDockBarClassRegistered;
	static BOOL RegisterDockBarClass();

	BOOL m_bInDynamicLayoutUpdate:1;
	void _HandleDestruction();

public:
	virtual BOOL DestroyWindow();

	virtual void OnDynamicLayoutOptimize();
	virtual void OnDynamicLayoutUpdate();

	// this is the one and only method of interest
	virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz);
	virtual void DoPaint(CDC* pDC);

	void DrawBorders( CDC *pDC, CRect& rect);
	void DrawGripper( CDC *pDC, const CRect& rect);
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDockBar)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs);
	virtual void PreSubclassWindow();

	//{{AFX_MSG(CTDockBar)
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC);
	afx_msg LRESULT OnSizeParent( WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

}; // class CTDockBar

#define __TDOCKBAR_CLASS_NAME _T("Tachyon-DockBar")

/////////////////////////////////////////////////////////////////////////
// CTDynamicDockBar window

class __TUI_API CTDynamicDockBar : public CTDockBar
{
public:
	struct __TUI_API VisibleLayoutItem
	{
		CArray< CTControlBar *, CTControlBar *&> m_vRow;
		LONG m_nRowMetric;
		LONG m_nRowMinMetric;
		LONG m_nRowExtent;
		LONG m_nRowMinExtent;

		void _AssignFromOther( const VisibleLayoutItem &other);
		VisibleLayoutItem();
		VisibleLayoutItem( const VisibleLayoutItem &other);

		~VisibleLayoutItem();
		VisibleLayoutItem& operator=( const VisibleLayoutItem &other);

		BOOL IsEmpty() const;
		BOOL IsRowMinSized() const;
	}; // struct VisibleLayoutItem

	struct __TUI_API VisibleLayout
	{
		CArray< VisibleLayoutItem *, VisibleLayoutItem *&> m_vRows;
		LONG m_nTotalMetric;
		LONG m_nTotalMinMetric;
		LONG m_nTotalMinExtent;
		LONG m_nTotalBarsCount;

		void _Clean();
		VisibleLayout();
		~VisibleLayout();

		void AddBarPointer( CTControlBar *pBar);
		void MakeNewRow();

		BOOL IsEmpty() const;
	}; // struct VisibleLayout

	void _VisibleLayoutBuild( VisibleLayout& _vl);
	void _VisibleLayoutAlign(
		VisibleLayout& _vl,
		CSize _size);

public:
	CTDynamicDockBar( UINT nCircleNo);

	DECLARE_DYNAMIC(CTDynamicDockBar)
	virtual void OnDynamicLayoutUpdate();
	virtual void OnDynamicLayoutOptimize();

	// this is the one and only method of interest
	virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz);
	virtual void CalcOrderedVector( ControlBarVector& vBars);

private:
	BOOL m_bHelperDockSiteModified:1;

public:
	virtual void _LockSequenceOptimization( BOOL bLock);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDynamicDockBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTDynamicDockBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CTControlBar::CDockStateBar;
}; // class CTDynamicDockBar

/////////////////////////////////////////////////////////////////////////
// CTDynamicControlBar window

class __TUI_API CTDynamicControlBar : public CTControlBar
{
public:
	CTDynamicDockBar *m_pWndDynDocker;
	CTDynamicControlBar();

	DECLARE_DYNAMIC(CTDynamicControlBar)

public:
	virtual BOOL IsBarWithGripper(
		BOOL *pGripperAtTop = NULL,
		BOOL *pTextOnGripper = NULL) const;

protected:
	virtual CTDynamicDockBar *OnCreateDynamicDockBarObject();
	virtual CSize _CalcLayoutMinSize() const;

	virtual int _CalcDesiredMinHW() const;
	virtual int _CalcDesiredMinVH() const;
	virtual CSize _CalcDesiredMinFloatedSize() const;

	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD nMode);

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDynamicControlBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTDynamicControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CTDynamicControlBar


#define __PLACEHODLER_BAR_PTR(x)	( HIWORD(x) == 0 )

#endif // #if (!defined __TDOCKBAR_H)