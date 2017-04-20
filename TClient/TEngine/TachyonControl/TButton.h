#if (!defined __TBUTTON_H)
#define __TBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTPopupMenuWnd;


/////////////////////////////////////////////////////////////////////////////
// CTButton window

class __TUI_API CTButton : public CButton
{
	DECLARE_DYNCREATE(CTButton);

// Construction
public:
    CTButton();
	~CTButton();

// Attributes
public:

// Operations
public:
	CMenu m_menu;
	DWORD m_dwMenuOpt;
	UINT m_nMenuLeftAreaWidth;

	virtual BOOL OnPutToPopupMenu( CTPopupMenuWnd *pPopup);
	virtual BOOL _OnTrackPopup();
	virtual BOOL _IsMenuAvail();
	virtual void _OnClick();

	BOOL _IsMouseOnButton();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTButton)
	public:
	virtual BOOL PreTranslateMessage( MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL GetDefault();

	void SetTooltipText( int nId, BOOL bActivate = TRUE);
	void SetTooltipText( CString *pText, BOOL bActivate = TRUE);
	void SetTooltipText( CString &strText, BOOL bActivate = TRUE);
	void SetTooltipText( LPCTSTR strText, BOOL bActivate = TRUE);
	void ActivateTooltip( BOOL bEnable = TRUE);

	BOOL SetBtnCursor( int nCursorId = -1);

	void SetAlign( int nAlign);
	int GetAlign();

	void SetFlat( BOOL bState = TRUE);
	BOOL GetFlat();

	void SetDrawBorder( BOOL bEnable = TRUE);
	void SetIcon( int nIconInId, int nIconOutId = NULL);
	void SetIcon( HICON hIconIn, HICON hIconOut = NULL);

protected:
	void _FreeWinObjects();

    //{{AFX_MSG(CTButton)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg LRESULT _OnPutToPopupMenuImpl( WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC &dc,
		const CWnd &refWndMenu,
		const CRect &rcExcludeArea,
		int nCombineAlign);

protected:
	void InitToolTip();
	void _DrawImpl(
		CDC &dc,
		BOOL bTransparent = FALSE);

	virtual void _RenderImpl(
		CDC &dc,
		BOOL bTransparent = FALSE);

	BOOL m_bPushTracking;
	BOOL m_bKeyTracking;
	BOOL m_bDrawBorder;
	BOOL m_bMouseOver;
	BOOL m_bIsFlat;
	BOOL m_bPushed;
	int m_nAlign;

	CToolTipCtrl m_ToolTip;
	HCURSOR m_hCursor;

	HICON m_hIconIn;
	HICON m_hIconOut;

	BOOL m_bIsDefault;
}; // class CTButton


/////////////////////////////////////////////////////////////////////////////

class __TUI_API CTColorButton : public CTButton
{
	DECLARE_DYNCREATE(CTColorButton);

// Construction
public:
	CTColorButton();
	~CTColorButton();

	CString m_strBtnTextColorDefault;
	CString m_strBtnTextColorCustom;

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTColorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bEnableBtnColorDefault;
	BOOL m_bEnableBtnColorCustom;
	BOOL m_bUseStdColorDlg;

	COLORREF m_clrDefault;
	COLORREF m_clrSelected;
	LPARAM m_lParamCookie;

	static HICON GenerateColorIcon( COLORREF clrIcon);
	virtual void OnGenerateColorBtnIcon(
		HICON *pIcon,
		BOOL *pDestroyIcon);

protected:
	virtual BOOL _OnTrackPopup();
	virtual BOOL _IsMenuAvail();
	virtual void _RenderImpl(
		CDC &dc,
		BOOL bTransparent = FALSE);

	//{{AFX_MSG(CTColorButton)
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CTColorButton

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif /// #if (!defined __TBUTTON_H)