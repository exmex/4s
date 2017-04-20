#if (!defined __TMINIDOCKFRAMEWND_H)
#define __TMINIDOCKFRAMEWND_H

#if (!defined __AFXPRIV_H__)
	#include <AfxPriv.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTBarNcAreaButton;
class CTControlBar;

/////////////////////////////////////////////////////////////////////////
// CTMiniDockFrameWnd window

class __TUI_API CTMiniDockFrameWnd : public CMiniDockFrameWnd
{
    DECLARE_DYNCREATE(CTMiniDockFrameWnd)

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTMiniDockFrameWnd)
    public:
	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
	//}}AFX_VIRTUAL

	BOOL PreTranslateMessage( MSG* pMsg);
	CToolTipCtrl m_wndToolTip;

// Implementation
public:
	CTMiniDockFrameWnd()
	{
		m_rcBtnHideBar.SetRectEmpty();
		m_rcFrameCaption.SetRectEmpty();

		m_bBtnHideBar = FALSE;
		m_nResizingMode = HTNOWHERE;
		m_ptLastResizing.x = -1;
		m_ptLastResizing.y = -1;
	};

	~CTMiniDockFrameWnd()
	{
	};

	CControlBar *GetControlBar();
	const CControlBar *GetControlBar() const;

	CTControlBar *GetTControlBar();
	const CTControlBar *GetTControlBar() const;

protected:
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI) const;

public:
	//{{AFX_MSG(CTMiniDockFrameWnd)
	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point);
	afx_msg void OnTimer( UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	static BOOL m_bOnWndPosChgProcessing;

public:
	static BOOL m_bAutoMakeTopmostOnMouseEvent;

protected:
	CPoint m_ptLastResizing;
	CRect m_rcFrameCaption;
	CRect m_rcBtnHideBar;
	CSize m_LastSize;
	BOOL m_bBtnHideBar;

	void _AnalyzeButtonsHover( CPoint point);
	void _AnalyzeButtonsHover()
	{
		CPoint point;

		if(::GetCursorPos(&point))
			_AnalyzeButtonsHover(point);
	};

private:
	static BOOL m_bInResizingUpdateState;
	UINT m_nResizingMode;
	CRect m_rcWndResizingStart;

	BOOL _GetSingleVisibleCaptionText( CString& strCaption);
	BOOL _ResizingIsInProgress()
	{
		return m_nResizingMode != HTNOWHERE;
	};

	void _ResizingStart(
		UINT nHitTest,
		const CPoint &point);

	void _ResizingUpdateState( const CPoint &point);
	void _ResizingEnd();

	friend class CTBarNcAreaButton;
}; // class CTMiniDockFrameWnd

#endif // #if (!defined __TMINIDOCKFRAMEWND_H)
