#if (!defined __TTOOLCONTROLBAR_H)
#define __TTOOLCONTROLBAR_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __TCONTROLBAR_H)
	#include <TControlBar.h>
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTBarButton

class __TUI_API CTBarButton : public CObject
{
protected:
	UINT _GetTrackPopupFlags();

public:
	DECLARE_DYNAMIC(CTBarButton)

protected:
	CTToolControlBar *m_pBar;
	UINT m_nCmdID;
	UINT m_nStyle;

	CSize m_ActiveSize;
	CRect m_ActiveRect;

	BOOL m_bAppendMdiWindowsMenu;
	BOOL m_bCtrlAutoDestroyed;
	BOOL m_bAutoDestroyMenu;
	BOOL m_bVertDocked;
	BOOL m_bPopupMenu;
	BOOL m_bVisible;
	BOOL m_bHover;
	BOOL m_bWrap;

	BOOL m_bCtrlVisibleVert:1;
	CWnd *m_pCtrl;
	HMENU m_hMenu;

	void _InitMembers()
	{
		m_nCmdID = ID_SEPARATOR;
		m_hMenu = NULL;
		m_pCtrl = NULL;
		m_pBar = NULL;

		m_ActiveSize.cx = 0;
		m_ActiveSize.cy = 0;
		m_nStyle = 0;

		m_bAppendMdiWindowsMenu = FALSE;
		m_bCtrlAutoDestroyed = FALSE;
		m_bAutoDestroyMenu = FALSE;
		m_bCtrlVisibleVert = FALSE;
		m_bVertDocked = FALSE;
		m_bPopupMenu = FALSE;
		m_bHover = FALSE;
		m_bWrap = FALSE;

		m_ActiveRect.SetRectEmpty();
		m_bVisible = TRUE;
	};

	void _UpdateCtrl()
	{
		if(!m_pCtrl)
			return;

		m_pCtrl->ShowWindow( m_bVisible && (!m_bVertDocked || GetCtrlVisibleVertically()) && (m_nStyle & TBBS_HIDDEN) == 0 ? SW_SHOW : SW_HIDE);
		m_pCtrl->EnableWindow(IsEnabled());

		if(m_pCtrl->IsKindOf(RUNTIME_CLASS(CButton)))
		{
			int nCheck = 0;

			if(IsPressed())
				nCheck = 1;
			else if(IsIndeterminate())
				nCheck = 2;

			((CButton *) m_pCtrl)->SetCheck(nCheck);
		}
	};

	void _UpdateCmdIdFromStyle()
	{
		if(m_nStyle&TBBS_SEPARATOR)
		{
			m_nStyle &= ~(TBBS_BUTTON);
			m_nCmdID = ID_SEPARATOR;
		}
		else
			m_nStyle |= TBBS_BUTTON;
	};

	void _DestroyMenu()
	{
		if(!m_hMenu)
			return;

		if(m_bAutoDestroyMenu)
			::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	};

public:
	CTBarButton(
		CTToolControlBar *pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0)
	{
		_InitMembers();

		m_nStyle = nStyle;
		m_pBar = pBar;
		SetCmdID(nCmdID);
	};

	~CTBarButton()
	{
		_DestroyMenu();
	};

	CTToolControlBar *GetBar()
	{
		return m_pBar;
	};

	const CTToolControlBar *GetBar() const
	{
		return m_pBar;
	};

	virtual UINT GetCmdID() const
	{
		return m_nCmdID;
	};

	virtual void SetCmdID( UINT nCmdID)
	{
		m_nCmdID = nCmdID;

		if( m_nCmdID == ID_SEPARATOR )
		{
			m_nStyle |= TBBS_SEPARATOR;
			m_nStyle &= ~(TBBS_BUTTON);
		}
		else
		{
			m_nStyle |= TBBS_BUTTON;
			m_nStyle &= ~(TBBS_SEPARATOR);
		}
	}

	virtual void SetCtrlVisibleVertically( BOOL bVisible)
	{
		m_bCtrlVisibleVert = bVisible;
		_UpdateCtrl();
	}

	virtual BOOL GetCtrlVisibleVertically() const
	{
		return m_bCtrlVisibleVert;
	}

	virtual CWnd *CtrlGet()
	{
		return m_pCtrl;
	}

	virtual void CtrlSet(
		CWnd *pCtrl,
		BOOL bCtrlAutoDestroyed)
	{
		if(m_pCtrl)
		{
			if(::IsWindow(m_pCtrl->GetSafeHwnd()))
				m_pCtrl->DestroyWindow();

			if(!m_bCtrlAutoDestroyed)
				delete m_pCtrl;
		}

		m_bCtrlAutoDestroyed = bCtrlAutoDestroyed;
		m_pCtrl = pCtrl;
	}

	virtual UINT GetStyle() const
	{
		return m_nStyle;
	};

	virtual void SetStyle( UINT nStyle)
	{
		m_nStyle = nStyle;

		_UpdateCmdIdFromStyle();
		_UpdateCtrl();
	};

	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0)
	{
		m_nStyle |= nAdd;
		m_nStyle &= ~nRemove;

		_UpdateCmdIdFromStyle();
		_UpdateCtrl();
	};

	virtual void SetVertDocked( BOOL bVertDocked)
	{
		m_bVertDocked = bVertDocked;
	}

	virtual BOOL IsWrap() const
	{
		return m_bWrap;
	};

	virtual void SetWrap( BOOL bWrap = TRUE)
	{
		m_bWrap = bWrap;
	};

	virtual BOOL IsSeparator() const
	{
		BOOL bSeparator = GetStyle() & TBBS_SEPARATOR;
		return bSeparator;
	};

	virtual BOOL IsDisabled() const
	{
		BOOL bDisabled = GetStyle() & TBBS_DISABLED;
		return bDisabled;
	};

	virtual BOOL IsEnabled() const
	{
		return !IsDisabled();
	};

	virtual BOOL IsIndeterminate() const
	{
		BOOL bIndeterminate = GetStyle()&TBBS_INDETERMINATE;
		return bIndeterminate;
	};

	virtual BOOL IsPressed() const
	{
		BOOL bPressed = GetStyle() & (TBBS_PRESSED|TBBS_CHECKED);
		return bPressed;
	};

	virtual void Show( BOOL bShow = TRUE)
	{
		m_bVisible = bShow;
		_UpdateCtrl();
	};

	virtual BOOL IsVisible() const
	{
		return m_bVisible;
	};

	virtual void SetHover( BOOL bHover = TRUE)
	{
		m_bHover = bHover;
		_UpdateCtrl();
	};

	virtual BOOL IsHover() const
	{
		return m_bHover;
	};

	virtual CString GetText() const;
	virtual void SetRect( const RECT& rectButton)
	{
		m_ActiveRect = rectButton;

		if( m_pCtrl != NULL && (
			!m_bVertDocked ||
			GetCtrlVisibleVertically()))
		{
			CRect rcCtrl;

			m_pCtrl->GetWindowRect(&rcCtrl);
			CRect rcMove( m_ActiveRect.TopLeft(), rcCtrl.Size());
			m_pCtrl->MoveWindow( &rcMove, FALSE);
		}

		_UpdateCtrl();
	};

	virtual CRect Rect() const
	{
		return m_ActiveRect;
	};

	operator CRect() const
	{
		return Rect();
	};

	virtual CSize Size() const
	{
		return m_ActiveSize;
	};

	operator CSize() const
	{
		return m_ActiveSize;
	};

	virtual BOOL IsContainerOfPopupLikeMenu()
	{
		return m_bPopupMenu;
	}

	virtual void SetMenu(
		HMENU hMenu,
		BOOL bPopupMenu,
		BOOL bAutoDestroyMenu);

	const HMENU GetMenu() const
	{
		return m_hMenu;
	};

	operator const HMENU() const
	{
		return GetMenu();
	};

	virtual HMENU GetMenu()
	{
		return m_hMenu;
	};

	operator HMENU()
	{
		return GetMenu();
	};

	virtual void SetAppendMdiWindowsMenu( BOOL bAppendMdiWindowsMenu)
	{
		m_bAppendMdiWindowsMenu = bAppendMdiWindowsMenu;
	};

	virtual BOOL IsAppendMdiWindowsMenu()
	{
		return m_bAppendMdiWindowsMenu;
	};

	virtual BOOL IsAbleToTrackMenu();
	virtual CSize CalculateLayout(
		CDC & dc,
		CSize sizePreCalc,
		BOOL bHorz);

	virtual BOOL PutToPopupMenu( CTPopupMenuWnd * pPopup);
	virtual HICON GetHICON();
	virtual CWnd *GetCmdTargetWnd();

	virtual void Paint(
		CDC &dc,
		BOOL bHorz);

	virtual void OnTrackPopup( CPoint point);
	virtual void OnHover(
		CPoint point,
		BOOL bOn);

	virtual void OnClick(
		CPoint point,
		BOOL bDown);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarButton


/////////////////////////////////////////////////////////////////////////////
// CTBarContentExpandButton

typedef CArray< CTBarButton *, CTBarButton *>	_tb_buttons_container;

#define __ID_TOOLBAR_RIGHT_BUTTON__		((UINT) -66)
#define __RIGHT_BUTTON_HORZ_DX__		11
#define __RIGHT_BUTTON_VERT_DY__		11

class CTPopupBaseWnd;
class CTPopupMenuWnd;
class CTPopupMenuSite;
class CTMiniDockFrameWnd;

class __TUI_API CTBarContentExpandButton : public CTBarButton
{
public:
	DECLARE_DYNAMIC(CTBarContentExpandButton)

protected:
	_tb_buttons_container m_buttons;
	BOOL m_bClearContent;

public:
	CTBarContentExpandButton( CTToolControlBar *pBar) : CTBarButton(pBar)
	{
		SetCmdID(__ID_TOOLBAR_RIGHT_BUTTON__);
	};

	~CTBarContentExpandButton()
	{
		CTBarButton::~CTBarButton();
	}

	BOOL IsContentClear()
	{
		return m_bClearContent;
	};

	void ClearContent( BOOL bClearContent = TRUE)
	{
		if(!this)
			return;

		m_bClearContent = bClearContent;
	};

	const _tb_buttons_container& GetButtons() const
	{
		return m_buttons;
	};

	_tb_buttons_container& GetButtons()
	{
		return m_buttons;
	};

	virtual CSize CalculateLayout(
		CDC &dc,
		CSize sizePreCalc,
		BOOL bHorz);

	virtual BOOL PutToPopupMenu( CTPopupMenuWnd *pPopup);
	virtual void Paint(
		CDC &dc,
		BOOL bHorz);

	virtual void OnTrackPopup( CPoint point);
	virtual void OnHover(
		CPoint point,
		BOOL bOn);

	virtual void OnClick(
		CPoint point,
		BOOL bDown);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarContentExpandButton

/////////////////////////////////////////////////////////////////////////////
// CTToolControlBar

class __TUI_API CTToolControlBar : public CTControlBar
{
public:
	DECLARE_DYNAMIC(CTToolControlBar)

private:
	class CTToolControlBarCmdUI;

	friend class CTBarContentExpandButton;
	friend class CTMiniDockFrameWnd;
	friend class CTBarMdiDocButton;
	friend class CTPopupMenuSite;
	friend class CTPopupBaseWnd;
	friend class CTPopupMenuWnd;
	friend class CTBarButton;

public:
	BOOL IsRightExpandButton( int nBtnIdx);

protected:
	CTBarContentExpandButton *m_pRightBtn;
	_tb_buttons_container m_buttons;

	void _RemoveAllButtonsImpl();
	int _GetButtonsCountImpl() const;

// Construction
public:
	CTToolControlBar();
	void SetSizes(
		SIZE sizeTBB,
		SIZE sizeImage = CSize( 0, 0));

	BOOL LoadToolBar( LPCTSTR lpszResourceName);
	BOOL LoadToolBar( UINT nIDResource)
	{
		return LoadToolBar(MAKEINTRESOURCE(nIDResource));
	};

	BOOL InsertButton(
		int nPos = -1,
		UINT nCmdID = ID_SEPARATOR,
		BOOL bDoRecalcLayout = TRUE);

	BOOL InsertSpecButton(
		int nPos,
		CTBarButton *pButton,
		BOOL bDoRecalcLayout = TRUE);

	BOOL RemoveButton(
		int nPos,
		BOOL bDoRecalcLayout = TRUE);

	BOOL SetButtons(
		const UINT *lpIDArray = NULL,
		int nIDCount = 0);

// Attributes
public:
	BOOL m_bRightButtonDisplayBarsList:1;

	int CommandToIndex( UINT nIDFind) const;
	UINT GetButtonID( int nIndex) const;

	virtual void GetButtonRect( int nIndex, LPRECT lpRect) const;
	void SetButtonStyle( int nIndex, UINT nStyle);
	UINT GetButtonStyle( int nIndex) const;

protected:
	virtual BOOL _IsShowContentWhenDragging() const;

public:
	virtual CTBarContentExpandButton *OnCreateBarRightBtn();
	BOOL InitContentExpandButton();

	void GetButtonInfo(
		int nIndex,
		UINT &nID,
		UINT &nStyle) const;

	void SetButtonInfo(
		int nIndex,
		UINT nID,
		UINT nStyle);

	CTBarButton *GetButton( int nIndex);
	CTBarContentExpandButton *GetRightButton();

	CWnd *GetButtonCtrl( int nIndex);
	BOOL SetButtonCtrl(
		int nIndex,
		CWnd * pCtrl = NULL,
		BOOL bCtrlAutoDestroyed = TRUE);

	void SetButtonCtrlVisibleVertically(
		int nIndex,
		BOOL bVisible = TRUE);

	BOOL GetButtonCtrlVisibleVertically( int nIndex) const;
	int GetButtonByAccessKey( TCHAR vkTCHAR);

	HMENU GetButtonMenu( int nIndex);
	BOOL SetButtonMenu(
		int nIndex,
		HMENU hMenu = NULL,
		BOOL bPopupMenu = TRUE,
		BOOL bAutoDestroyMenu = TRUE,
		BOOL bDoRecalcLayout = TRUE);

	BOOL MarkButtonAsMdiWindowsMenu(
		int nIndex,
		BOOL bAppendMdiWindowsMenu = TRUE);

	virtual BOOL TrackButtonMenu( int nIndex);
	int GetVisibleButton(
		int nBtnIdx,
		BOOL bNext,
		BOOL bPassDisabled = TRUE);

	int GetButtonsCount() const
	{
		return _GetButtonsCountImpl();
	};

	int GetMenuTrackingButton() const
	{
		return m_nBtnIdxMenuTracking;
	};

// Implementation
public:
	virtual ~CTToolControlBar();

	virtual void ToggleDocking();
	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz);

	virtual CSize CalcDynamicLayout(
		int nLength,
		DWORD dwMode);

protected:
	CTBarButton *_GetButtonPtr( int nIndex) const;
	void _InvalidateButton( int nIndex);
	void _UpdateButton( int nIndex);

protected:
	BOOL _UpdateHoverButton( CPoint point = CPoint( -1, -1));

	virtual DWORD RecalcDelayShow( AFX_SIZEPARENTPARAMS *lpLayout);
	virtual void _RecalcPositionsImpl();
	virtual void _RecalcLayoutImpl();

	// input CRect should be client rectangle size
	void _CalcInsideRect(
		CRect &rect,
		BOOL bHorz) const;

	CSize _CalcLayout(
		DWORD dwMode,
		int nLength = -1);

	CSize _CalcSize( BOOL bVerticallyDocked);
	void _SizeToolBar(
		int nLength,
		BOOL bVert = FALSE);

	int _WrapToolBar(
		int nWidth,
		int nHeight = 32767);

public:
	virtual CSize _GetDefButtonSize()
	{
		return m_szDefTBB;
	};

	int _GetIndexOf( CTBarButton *pTBB)
	{
		if(!pTBB)
			return -1;

		int nCount = (int) m_buttons.GetSize();
		for( int i=0; i<nCount; i++)
		{
			if( m_buttons[i] == pTBB )
				return i;
		}

		return -1;
	};

protected:
	void _SwitchMenuTrackingIndex( int nNewMenuTrackingIndex = -1);
	static CTToolControlBar *_GetMenuTrackingBar();

public:
	static void _CloseTrackingMenus();

protected:
	static void _CbPaintCombinedContent(
		LPVOID pCookie,
		CDC &dc,
		const CWnd &refWndMenu,
		const CRect &rcExcludeArea,
		int eCombineAlign);

public:
	static BOOL m_bMenuTrackingExpanded;
	static BOOL m_bMenuTracking;

protected:
	static const CSize m_szDefTBB;

	int m_nBtnIdxMenuTracking;
	int m_nBtnIdxCapture;
	int m_nBtnIdxHover;

public:
	int _GetTrackingButtonIndex() const
	{
		return m_nBtnIdxMenuTracking;
	}

	virtual void OnUpdateCmdUI(
		CFrameWnd* pTarget,
		BOOL bDisableIfNoHndler);

protected:
	virtual BOOL _CanDockToTabbedContainers() const;
	virtual BOOL _CanDockToInnerCircles() const;

	virtual void DoPaint( CDC *pDC);
	int _HitTestImpl(
		CPoint point,
		UINT nButtonStyleInclude = 0,
		UINT nButtonStyleExclude = 0) const;

	virtual int HitTest( CPoint point) const;
	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI) const;

	//{{AFX_MSG(CTToolControlBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // #if (!defined __TTOOLCONTROLBAR_H)