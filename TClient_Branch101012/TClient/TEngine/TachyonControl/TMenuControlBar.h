#if (!defined __TMENUCONTROLBAR_H)
#define __TMENUCONTROLBAR_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __TTOOLCONTROLBAR_H)
	#include <TToolControlBar.h>
#endif

#if (!defined __THOOKSINK_H)
	#include "THookSink.h"
#endif

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////////
// CTBarMdiDocButton

class __TUI_API CTBarMdiDocButton : public CTBarButton
{
public:
	DECLARE_DYNAMIC(CTBarMdiDocButton)

protected:
	HWND _GetActiveMdiChildWnd( BOOL &bMaximized);

public:
	CTBarMdiDocButton(
		CTToolControlBar *pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0) : CTBarButton(
		pBar,
		nCmdID,
		nStyle)
	{
	};

	~CTBarMdiDocButton()
	{
		CTBarButton::~CTBarButton();
	};

	virtual UINT GetStyle() const
	{
		return CTBarButton::GetStyle()&(~TBBS_DISABLED);
	};

	virtual void SetStyle( UINT nStyle)
	{
		nStyle &= ~TBBS_DISABLED;
		CTBarButton::SetStyle(nStyle);
	};

	virtual void ModifyStyle(
		UINT nAdd,
		UINT nRemove = 0)
	{
		nAdd &= ~TBBS_DISABLED;
		nRemove |= TBBS_DISABLED;

		CTBarButton::ModifyStyle(
			nAdd,
			nRemove);
	};

	virtual BOOL PutToPopupMenu( CTPopupMenuWnd *pPopup);
	virtual void OnTrackPopup( CPoint point);
	virtual CWnd *GetCmdTargetWnd();
	virtual HICON GetHICON();
}; // class CTBarMdiDocButton


/////////////////////////////////////////////////////////////////////////////
// CTBarMdiRightButton

#define __GAP_BETWEEN_WND_RIGHT_BUTTONS		2

class CTMenuControlBar;
class __TUI_API CTBarMdiRightButton : public CTBarMdiDocButton
{
	struct MdiMenuBarRightButtonsInfo
	{
		CWnd *m_pWndMdiChildFrame;
		CTMenuControlBar *m_pBar;

		CSize m_calcSize;
		int m_cxIcon;
		int m_cyIcon;

		BOOL m_bInitialized;
		BOOL m_bBtnClose;
		BOOL m_bBtnMaximize;
		BOOL m_bBtnMinimize;
		BOOL m_bBtnHelp;

		CRect m_rcBtnClose;
		CRect m_rcBtnMaximize;
		CRect m_rcBtnRestore;
		CRect m_rcBtnMinimize;
		CRect m_rcBtnHelp;

		MdiMenuBarRightButtonsInfo()
		{
			m_bInitialized = FALSE;
			m_bBtnMaximize = FALSE;
			m_bBtnMinimize = FALSE;
			m_bBtnClose = FALSE;
			m_bBtnHelp = FALSE;

			m_pWndMdiChildFrame = NULL;
			m_pBar = NULL;

			m_cxIcon = ::GetSystemMetrics(SM_CXSIZE);
			m_cyIcon = ::GetSystemMetrics(SM_CYSIZE);
			m_cxIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
			m_cyIcon -= __GAP_BETWEEN_WND_RIGHT_BUTTONS;
			m_calcSize.cx = 0;
			m_calcSize.cy = 0;
			EmptyLocations();
		};

		void EmptyLocations()
		{
			m_rcBtnMaximize.SetRectEmpty();
			m_rcBtnMinimize.SetRectEmpty();
			m_rcBtnRestore.SetRectEmpty();
			m_rcBtnClose.SetRectEmpty();
			m_rcBtnHelp.SetRectEmpty();
		};

		void InitLocations(
			const CRect &rcBtn,
			BOOL bHorz);
	};
	friend class CTMenuControlBar;

public:
	DECLARE_DYNAMIC(CTBarMdiRightButton)

public:
	CTBarMdiRightButton(
		CTToolControlBar *pBar,
		UINT nCmdID = ID_SEPARATOR,
		UINT nStyle = 0) : CTBarMdiDocButton(
		pBar,
		nCmdID,
		nStyle)
	{
	};

	~CTBarMdiRightButton()
	{
		CTBarMdiDocButton::~CTBarMdiDocButton();
	};

	virtual HICON GetHICON();
	virtual CSize CalculateLayout(
		CDC &dc,
		CSize szPreCalc,
		BOOL bHorz);

	virtual void Paint(
		CDC &dc,
		BOOL bHorz);

	virtual BOOL PutToPopupMenu( CTPopupMenuWnd *pPopup);
	virtual void OnTrackPopup( CPoint point);
	virtual void OnClick(
		CPoint point,
		BOOL bDown);

	virtual int OnToolHitTest(
		CPoint point,
		TOOLINFO *pTI);
}; // class CTBarMdiRightButton


/////////////////////////////////////////////////////////////////////////////
// CTMenuControlBar

class __TUI_API CTMenuControlBar : public CTToolControlBar, public CTHookSink
{
	friend class CTBarMdiDocButton;
	friend class CTBarMdiRightButton;
	friend struct CTBarMdiRightButton::MdiMenuBarRightButtonsInfo;

	void _GetMdiMenuBarRightButtonsInfo(
		CTBarMdiRightButton::MdiMenuBarRightButtonsInfo& _info,
		CTBarMdiRightButton *pTBB,
		BOOL bHorz);

	CString m_strMdiWindowPopupName;
	UINT m_nMdiDocButtonCmdID;
	HWND m_hWndHelper;

	BOOL m_bSysMenuTracking:1;
	BOOL m_bFlatTracking:1;

	int m_nFlatTrackingIndex;
	int m_nOldTrackingIndex;

	CMenu m_menuFrame;
	CMenu m_menuDoc;

	void _UpdateFlatTracking( BOOL bRepaint = TRUE);
	CRect _GetChildFrameSysIconRect();
	CRect _GetMainFrameSysIconRect();

	virtual BOOL OnHookWndMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM &wParam,
		LPARAM &lParam);

	HWND _GetHwndPlacement();
	HWND _GetHwndMainFrame();
	HWND _GetHwndChildFrame();
	HWND _GetHwndMdiArea();

	BOOL m_bMdiApp:1;
	BOOL _UpdateMenuBar( BOOL bDoRecalcLayout = TRUE);
	HWND _GetActiveMdiChildWnd( BOOL &bMaximized);
	BOOL _InvokeParentTrackButtonMenu( int nIndex);
	BOOL _InstallMdiDocButtons( BOOL bDoRecalcLayout = TRUE);
	BOOL _SyncActiveMdiChild();
	BOOL _TrackFrameSystemMenu(
		CFrameWnd *pFrame,
		CPoint *pPoint = NULL,
		BOOL bSelectAny = FALSE,
		LPCRECT rcExcludeArea = NULL,
		UINT nTrackFlags = (UINT) -1,
		BOOL bCombinedMode = FALSE);

public:
	DECLARE_DYNAMIC(CTMenuControlBar);

// Construction
public:
	CTMenuControlBar();

// Attributes
public:
	static BOOL m_bHideMenuBarExpandContentButton;
	virtual CSize _GetDefButtonSize()
	{
		return m_szDefTBB;
	};

protected:
	static const CSize m_szDefTBB;
	static const UINT CTMenuControlBar::m_nMsgTrackButtonMenu;

// Operations
public:
	BOOL UpdateMenuBar( BOOL bDoRecalcLayout = TRUE);
	CMenu *GetMenu();

	void SetMdiWindowPopupName( LPCTSTR strMdiWindowPopupName = NULL)
	{
		m_strMdiWindowPopupName = strMdiWindowPopupName ? strMdiWindowPopupName : _T("");
		m_strMdiWindowPopupName.TrimLeft();
		m_strMdiWindowPopupName.TrimRight();

		while( m_strMdiWindowPopupName.Replace( _T("&"), _T("")) > 0 )
		{
			m_strMdiWindowPopupName.TrimLeft();
			m_strMdiWindowPopupName.TrimRight();
		}
	};

	BOOL LoadMenuBar( UINT nResourceID);
	BOOL TranslateMainFrameMessage( MSG *pMsg);
	BOOL TrackMainFrameSystemMenu(
		CPoint *pPoint = NULL,
		BOOL bSelectAny = FALSE);

	BOOL TrackChildFrameSystemMenu(
		CPoint *pPoint = NULL,
		BOOL bSelectAny = FALSE);

// Overridables
public:
	virtual void OnUpdateCmdUI(
		CFrameWnd *pTarget,
		BOOL bDisableIfNoHndler);

protected:
	virtual BOOL TrackButtonMenu( int nIndex);

	virtual void _RecalcPositionsImpl();
	virtual void _Dragging_OnStart();
	virtual void _RecalcLayoutImpl();
	void _KillFrameMenu();

// Overrides
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTMenuControlBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL IsDisplayMdiDocumentButtons() const;
	virtual ~CTMenuControlBar();

	BOOL IsMenuBarTracking()
	{
		if( m_bFlatTracking || m_bSysMenuTracking )
			return TRUE;

		return FALSE;
	};

// Generated message map functions
protected:
    //{{AFX_MSG(CTMenuControlBar)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTrackButtonMenu(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
}; // class CTMenuControlBar

#endif // #if (!defined __TMENUCONTROLBAR_H)