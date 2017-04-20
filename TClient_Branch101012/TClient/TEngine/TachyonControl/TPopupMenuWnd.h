#if (!defined __TPOPUPMENUWND_H)
#define __TPOPUPMENUWND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#if (!defined __AFXTEMPL_H__)
	#include <AfxTempl.h>
#endif

#if (!defined __TCMDMANAGER_H)
	#include <TCmdManager.h>
#endif


/////////////////////////////////////////////////////////////////////////////
// CTSoundPlayer

class __TUI_API CTSoundPlayer : public CObject
{
	struct __TUI_API SoundEventInitDone : public CEvent
	{
		SoundEventInitDone() : CEvent( FALSE, TRUE)
		{
		}

		void SetInitDone()
		{
			SetEvent();
		}

		void WaitInitDone()
		{
			HANDLE hEvent = *this;
			::WaitForSingleObject( hEvent, INFINITE);
		}
	};

	struct __TUI_API SoundThredadParms
	{
		SoundEventInitDone *m_pEventInitDone;
		CString m_strSoundSpecBuffer;

		DWORD m_dwPlayerFlags;
		DWORD m_dwThreadID;

		HANDLE m_hModule;
		HANDLE m_hThread;

		static CCriticalSection m_csPlaySnd;
		static CEvent m_EventPlaySnd;

		SoundThredadParms(
			LPCTSTR strSoundSpecBuffer,
			HANDLE hModule = NULL,
			DWORD dwPlayerFlags = SND_ALIAS|SND_NODEFAULT|SND_NOWAIT|SND_SYNC) : m_strSoundSpecBuffer(strSoundSpecBuffer),
			m_hModule(hModule),
			m_dwPlayerFlags(dwPlayerFlags),
			m_hThread(NULL),
			m_dwThreadID(0),
			m_pEventInitDone(NULL)
		{
		}

		static DWORD ParallelPlayerProc( SoundThredadParms *pParms);
		operator LPTHREAD_START_ROUTINE() const
		{
			return (LPTHREAD_START_ROUTINE) ParallelPlayerProc;
		};

		void PlaySound();
		void PlaySoundParallel();
	}; // struct SoundThredadParms

public:
	DECLARE_DYNAMIC(CTSoundPlayer);

	class __TUI_API CTSoundPlayerAutoPtr
	{
		CTSoundPlayer *m_pPlayer;

	public:
		CTSoundPlayerAutoPtr();
		~CTSoundPlayerAutoPtr();

		void InstallSoundPlayer( CTSoundPlayer *pPlayer);
		CTSoundPlayer *operator->()
		{
			return m_pPlayer;
		};
	}; // class CTSoundPlayerAutoPtr

	CTSoundPlayer();
	~CTSoundPlayer();

	enum e_ui_sounds
	{
		__NO_SOUND					= 0,
		__ON_MENU_EXPAND_CLICKED	= 1,
		__ON_MENU_POPUP_DISPLAYED	= 2,
		__ON_MENU_CMD_CLICKED		= 3,
	}; // enum e_ui_sounds

	virtual void PlaySound( CTSoundPlayer::e_ui_sounds nSoundID);
}; // class CTSoundPlayer


extern __TUI_API CTSoundPlayer::CTSoundPlayerAutoPtr		g_SoundPlayer;

/////////////////////////////////////////////////////////////////////////////
// CTWndShadow

class __TUI_API CTWndShadow
{
	COLORREF *m_pHelperDibSurface;

	CBitmap m_bmp0;
	CBitmap m_bmp1;

	CRect m_rcWndArea;
	CRect m_rc1stArea;
	CRect m_rc2ndArea;

	BOOL m_bEnablePhotos;
	UINT m_nShadowSize;
	UINT m_nBr0;
	UINT m_nBr1;

	void _FreeWinObjects();
	void _DoPixelOvershadow(
		int nMakeSpec,
		int nPosX,
		int nPosY);

	BOOL _MakePhotos(
		CDC &dc,
		CDC &dcmm);

	BOOL _PaintLo( CDC &dc);
	BOOL _PaintHi( CDC &dc);

public:
	enum eShadowOptions
	{
		DEF_BRIGHTNESS_MIN =  70UL,
		DEF_BRIGHTNESS_MAX = 100UL,
		DEF_SHADOW_SIZE    =   4UL,
	};

	CTWndShadow();
	~CTWndShadow();

	BOOL Restore( CDC &dc);
	BOOL Paint( CDC &dc);
	BOOL Paint(
		CDC &dc,
		const CRect &rcWndArea,
		const CRect &rc1stArea = CRect( 0, 0, 0, 0),
		const CRect &rc2ndArea = CRect( 0, 0, 0, 0),
		UINT nShadowSize = DEF_SHADOW_SIZE,
		UINT nBr0 = DEF_BRIGHTNESS_MIN,
		UINT nBr1 = DEF_BRIGHTNESS_MAX,
		BOOL bEnablePhotos = TRUE);

	void Destroy();
}; // class CTWndShadow


/////////////////////////////////////////////////////////////////////////////
// CTPopupBaseWnd

class __TUI_API CTPopupBaseWnd : public CWnd
{
public:
	DECLARE_DYNCREATE(CTPopupBaseWnd)

	CTPopupBaseWnd();
	virtual BOOL DestroyWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPopupBaseWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTPopupBaseWnd();

	static UINT m_nMsgPrepareMenu;
	static UINT m_nMsgPopupNext;
	static UINT m_nMsgPopupPrev;
	static UINT m_nMsgNotifyMenuExpanded;
	static UINT m_nMsgNotifyMenuClosed;
	static UINT m_nMsgPopupDrawItem;
	static UINT m_nMsgPopupDrawLeftArea;

	enum e_combine_align
	{
		__CMBA_NONE = 0,
		__CMBA_TOP = 1,
		__CMBA_BOTTOM = 2,
		__CMBA_RIGHT = 3,
		__CMBA_LEFT = 4,
	};

	typedef void (*pCbPaintCombinedContent)(
		LPVOID pCookie,
		CDC &dc,
		const CWnd &refWndMenu,
		const CRect &rcExcludeArea,
		int nCombineAlign);

	enum e_animation_type
	{
		__AT_NONE = 0,
		__AT_RANDOM = 1,
		__AT_ROLL = 2,
		__AT_SLIDE = 3,
		__AT_FADE = 4,
		__AT_ROLL_AND_STRETCH = 5,
		__AT_SLIDE_AND_STRETCH = 6,
		__AT_NOISE = 7,
		__AT_BOXES = 8,
		__AT_CIRCLES = 9,
		__AT_HOLES = 10,

		__AT_MAX_VALID_VALUE = 10,
		__AT_CONTENT_DISPLAY = -1,
		__AT_CONTENT_EXPAND = -2
	};

	static e_animation_type m_DefAnimationType;
	static BOOL m_bUseDesktopWorkArea;
	static BOOL m_bEnableOnIdleCalls;
	virtual CRect _CalcDesktopRect();

protected:
	CRect m_rcExcludeArea;
	CSize m_szFullItems;
	CPoint m_ptTrack;

	BOOL m_bExcludeAreaSpec:TRUE;
	BOOL m_bCombineWithEA:TRUE;
	BOOL m_bAnimFinished:TRUE;

	LPVOID m_pCbPaintCombinedCookie;
	pCbPaintCombinedContent m_pCbPaintCombinedContent;

	e_animation_type m_AnimationType;
	CTSoundPlayer::e_ui_sounds m_nPlaySoundOnAnimationFinished;

	COLORREF *m_clrFadeBitsSrc;
	COLORREF *m_clrFadeBitsDst;
	COLORREF *m_clrFadeBitsTmp;

	int m_nAnimPercentOld;
	int m_nAnimPercent;

	CTWndShadow m_ShadowMain;
	CTWndShadow m_ShadowCMBA;

	CBitmap m_bmpScreenSrc;
	CBitmap m_bmpScreenDst;
	CBitmap m_bmpScreenTmp;

	virtual void _StartAnimation();
	virtual void _InitAnimation();

	void _SurfacesDelete()
	{
		if(m_bmpScreenDst.GetSafeHandle())
			m_bmpScreenDst.DeleteObject();

		if(m_bmpScreenSrc.GetSafeHandle())
			m_bmpScreenSrc.DeleteObject();

		if(m_bmpScreenTmp.GetSafeHandle())
			m_bmpScreenTmp.DeleteObject();

		m_clrFadeBitsSrc = NULL;
		m_clrFadeBitsDst = NULL;
		m_clrFadeBitsTmp = NULL;
	}

	virtual void _EndAnimation()
	{
		_SurfacesDelete();
	}

	virtual void _DrawAnimatedState( CDC &dc);
	virtual void _DoPaint(
		CDC &dcPaint,
		BOOL bUseBackBuffer = TRUE);

	virtual void _GetClientRect( RECT *pRectClient)
	{
		GetClientRect(pRectClient);
	}

	void _GetClientRect( RECT &rcClient)
	{
		_GetClientRect(&rcClient);
	}

	// Generated message map functions
	//{{AFX_MSG(CTPopupBaseWnd)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CTPopupBaseWnd


/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuTipWnd

class __TUI_API CTPopupMenuTipWnd : public CTPopupBaseWnd
{
	BOOL m_bFlipHorz:TRUE;
	BOOL m_bFlipVert:TRUE;

	CString m_strText;
	CSize m_IconSize;

	HICON m_hIcon;
	HRGN m_hRgn;

public:
	DECLARE_DYNCREATE(CTPopupMenuTipWnd)
	CTPopupMenuTipWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPopupMenuTipWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTPopupMenuTipWnd();

	void SetText( LPCTSTR lpszText);
	void SetIcon( HICON hIcon);
	void Hide();

	BOOL Show(
		CWnd *pWndParent,
		const RECT &rcExcludeArea);

protected:
	BOOL GetWindowRegion(
		CDC *pDC,
		HRGN *hRegion,
		CSize *Size = NULL);

	void _DoPaint(
		CDC &dcPaint,
		BOOL bUseBackBuffer = TRUE);

	// Generated message map functions
	//{{AFX_MSG(CTPopupMenuTipWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
}; // class CTPopupMenuTipWnd


/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuSite

class CTPopupMenuWnd;
class CTPopupColorMenuWnd;

class __TUI_API CTPopupMenuSite
{
	BOOL m_bShutdownMode:TRUE;

	CTPopupMenuWnd *m_pWndAnimation;
	CTPopupMenuWnd *m_pWndCapture;
	CTPopupMenuWnd *m_pTopMenu;

	UINT *m_lpnResultCmdID;
	HHOOK m_hKeyboardHook;
	HHOOK m_hMouseHook;

	void _Hook( BOOL bHook = TRUE);
	void _Done();

	static LRESULT CALLBACK _HookMouseProc(
		int nCode,
		WPARAM wParam,
		LPARAM lParam);

	static LRESULT CALLBACK _HookKeyboardProc(
		int nCode,
		WPARAM wParam,
		LPARAM lParam);

public:
	CTPopupMenuSite();
	~CTPopupMenuSite();

	BOOL IsEmpty() const
	{
		return m_pTopMenu == NULL;
	}

	BOOL IsShutdownMode() const
	{
		return m_bShutdownMode;
	}

	void SetTargetCmdIdPtr( UINT *lpnResultCmdID = NULL);
	UINT *GetTargetCmdIdPtr();

	CTPopupMenuWnd *GetCapture();
	void SetCapture( CTPopupMenuWnd *pNewPopup = NULL);

	CTPopupMenuWnd *GetAnimated();
	void SetAnimated( CTPopupMenuWnd *pNewPopup = NULL);

	void SetInstance( CTPopupMenuWnd *pTopMenu = NULL);
	CTPopupMenuWnd *GetInstance();

	void DoneInstance();
	void operator=( CTPopupMenuWnd *pTopMenu)
	{
		SetInstance(pTopMenu);
	}

	CTPopupMenuWnd *operator->()
	{
		return m_pTopMenu;
	}

	BOOL IsTopPupup( CTPopupMenuWnd *pTopMenu) const;
	static CTPopupMenuSite m_DefPopupMenuSite;

	friend class CTPopupBaseWnd;
	friend class CTPopupMenuWnd;
}; // class CTPopupMenuSite


/////////////////////////////////////////////////////////////////////////////
// CTPopupMenuWnd

#define TPMX_LEFTALIGN				0x00000001L
#define TPMX_CENTERALIGN			0x00000002L
#define TPMX_RIGHTALIGN				0x00000003L
#define TPMX_TOPALIGN				0x00000004L
#define TPMX_VCENTERALIGN			0x00000005L
#define TPMX_BOTTOMALIGN			0x00000006L
#define TPMX_ALIGN_MASK				0x0000000FL

#define TPMX_COMBINE_NONE			0x00000000L
#define TPMX_COMBINE_ANY_SUITABLE	0x00000010L
#define TPMX_COMBINE_DEFAULT		0x00000020L
#define TPMX_COMBINE_MASK			0x00000030L

#define TPMX_SELECT_ANY				0x00000040L
#define TPMX_DO_MESSAGE_LOOP		0x00000080L
#define TPMX_NO_HIDE_RARELY			0x00000100L
#define TPMX_NO_WM_COMMAND			0x00000200L
#define TPMX_NO_CMD_UI				0x00000400L
#define TPMX_OWNERDRAW_FIXED		0x00000800L


class __TUI_API CTPopupMenuWnd : public CTPopupBaseWnd
{
	friend class CTToolControlBar;
	friend class CTMenuControlBar;
	friend class CTControlBar;

	class __TUI_API CFriendlyWinApp : CWinApp
	{
	public:
		inline static CFriendlyWinApp *_GetFriendlyApp()
		{
			CWinApp *pApp = ::AfxGetApp();
			return (CFriendlyWinApp *) pApp;
		}

		CRecentFileList *_GetRecentFileList()
		{
			return m_pRecentFileList;
		}
	}; // class CFriendlyWinApp

public:
	static BOOL m_bAllowNonAccelPositioning;
	friend class CTPopupMenuSite;

public:
	DECLARE_DYNCREATE(CTPopupMenuWnd)
	CTPopupMenuWnd();

// Attributes
public:
	virtual int _GetCurIndex() const;

protected:
	CTPopupMenuWnd *m_pWndParentMenu;
	CTPopupMenuTipWnd m_wndToolTip;

	int m_nDelayedFocusItemIndex;
	int m_nScrollingDirection;
	int m_nDyScrollOffset;
	int m_nCurIndex;

	BOOL m_bScrollingAvailable:TRUE;
	BOOL m_bExpandWasPressed:TRUE;
	BOOL m_bExpandAvailable:TRUE;
	BOOL m_bTopLevel:TRUE;

	UINT m_nWaitingExpandTickCount;
	HWND m_hWndCmdReciever;
	DWORD m_dwTrackFlags;

	CRect m_rcScrollBottom;
	CRect m_rcScrollTop;
	CRect m_rcExpandBtn;
	UINT m_nLeftAreaWidth;

public:
	HWND GetCmdRecieverHWND()
	{
		return m_hWndCmdReciever;
	};

protected:
	enum spec_buttons_index
	{
		IDX_NOTHING = -1,
		IDX_SCROLL_TOP = -2,
		IDX_SCROLL_BOTTOM = -3,
		IDX_EXPAND = -4
	};

// Operations
public:
	virtual HMENU ExportToMenu( BOOL bDeep = TRUE) const;
	virtual BOOL LoadMenu(
		HWND hWndCmdRecv,
		UINT nIDResource,
		BOOL bPopupMenu = TRUE);

	virtual BOOL UpdateFromMenu(
		HWND hWndCmdRecv,
		CMenu *pBuildMenu,
		BOOL bPopupMenu = TRUE,
		BOOL bTopLevel = TRUE);

	virtual BOOL UpdateMdiWindowsMenu( CWnd *pWndStartSearchMdiFrameWnd = NULL);
	virtual void SetCmdTargetToAllItems(
		HWND hWndSpecCmdReciever = NULL,
		BOOL bOnlyThisLevel = FALSE);

	enum menu_item_type
	{
		TYPE_POPUP		= 0xFFFFFFFF,
		TYPE_SEPARATOR	= 0x00000000
	};

	virtual HICON ItemGetIcon( int nPos) const;
	virtual UINT ItemGetCmdID( int nPos) const;
	virtual int ItemGetCount() const;

	virtual CString ItemGetAccelText( int nPos) const;
	virtual CString ItemGetText( int nPos) const;
	virtual int ItemFindPosForCmdID(
		UINT nCmdID,
		int nPosStart = -1) const;

	virtual BOOL ItemSetPopupIcon(
		int nPos,
		HICON hIcon = NULL);

	virtual BOOL ItemSetPopupText(
		int nPos,
		LPCTSTR strText);

	virtual BOOL ItemSetPopupAccelText(
		int nPos,
		LPCTSTR strText);

	virtual const CTPopupMenuWnd *ItemGetPopup( int nPos) const;
	virtual CTPopupMenuWnd *ItemGetPopup( int nPos);
	virtual BOOL ItemIsDisplayed( int nPos) const;
	virtual void ItemSetDisplayed(
		int nPos,
		BOOL bForceDisplayed = TRUE);

	virtual BOOL ItemRemove( int nPos = -1);
	virtual BOOL ItemInsert(
		UINT nCmdID = TYPE_SEPARATOR,
		int nPos = -1,
		LPCTSTR strText = NULL,
		HICON hIcon = NULL,
		HWND hWndSpecCmdReciever = NULL);

	virtual BOOL ItemInsertSpecPopup(
		CTPopupMenuWnd *pSpecPopup,
		int nPos = -1,
		LPCTSTR strText = NULL,
		HICON hIcon = NULL);

	virtual int ItemFindByAccessChar(
		TCHAR chrAccess,
		int nStartIdx = -1,
		BOOL bRestartAt0 = TRUE) const;

	virtual int ItemFindByText(
		LPCTSTR strText,
		int nStartIdx = -1,
		BOOL bRestartAt0 = TRUE) const;

	virtual BOOL IsAllItemsRarelyUsed() const;
	virtual BOOL CreatePopupMenu( HWND hWndCmdRecv);

	void SetLeftAreaWidth( UINT nWidth = 0);
	UINT GetLeftAreaWidth() const;

	static CTPopupMenuWnd *GetTrackingMenu();
	static void CancelMenuTracking();

	inline static BOOL IsKeyPressed( int nVirtKey)
	{
		SHORT nKeyState = ::GetKeyState(nVirtKey);
		return nKeyState & (1 << (sizeof(SHORT) * 8 - 1));
	}

	static BOOL IsCmdKeyActivation( LPARAM lParam = 0);
	static BOOL IsMenuTracking();

	virtual BOOL TrackPopupMenu(
		DWORD dwTrackFlags,
		int x,
		int y,
		LPCRECT lpRect = NULL,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbPaintCombinedContent = NULL,
		UINT *lpnResultCmdID = NULL);

	struct __TUI_API MsgPrepareMenuData
	{
		BOOL m_bMenuCanceled:TRUE;
		BOOL m_bMenuChanged:TRUE;
		CTPopupMenuWnd *m_pPopup;

		MsgPrepareMenuData( CTPopupMenuWnd *pPopup) : m_bMenuChanged(FALSE),
			m_bMenuCanceled(FALSE),
			m_pPopup(pPopup)
		{
		}

		~MsgPrepareMenuData()
		{
		}

		operator WPARAM() const
		{
			return WPARAM(this);
		}

		LRESULT SendMessage( HWND hWnd)
		{
			return ::SendMessage(
				hWnd,
				CTPopupMenuWnd::m_nMsgPrepareMenu,
				WPARAM(*this),
				LPARAM(m_pPopup));
		}

		LRESULT SendMessage( CWnd *pWnd)
		{
			return SendMessage(pWnd->GetSafeHwnd());
		}
	}; // struct MsgPrepareMenuData

	static BOOL m_bUseStretchOnExpandAnimation;
	static BOOL m_bMenuExpandAnimation;
	static BOOL m_bMenuHighlightRarely;
	static BOOL m_bMenuShowCoolTips;
	static BOOL m_bMenuWithShadows;
	static BOOL m_bMenuExpanding;

protected:
	static int _GetSpecBtnHeight();

	BOOL _BuildItems(
		CMenu *pBuildMenu,
		BOOL bTopLevel);

	void _RecalcLayoutImpl();
	void _UpdateCmdUI();
	void _Init();

	virtual BOOL _CreateHelper( CWnd *pWndCmdReciever);
	virtual int _HitTest( const CPoint &point);
	virtual void _DrawAnimatedState( CDC &dc);
	virtual void _DoPaint(
		CDC &dcPaint,
		BOOL bUseBackBuffer = TRUE);

	virtual void _GetClientRect( RECT *pRectClient);
	void _EndSequence(
		UINT nCmdID = 0,
		HWND hWndDeliver = NULL);

public:
	static void PassMsgLoop( BOOL bEnableOnIdleCalls);

protected:
	struct __TUI_API ExpandEffectRects
	{
		CRect m_rcSrc;
		CRect m_rcDst;

		ExpandEffectRects()
		{
			m_rcSrc.SetRectEmpty();
			m_rcDst.SetRectEmpty();
		};

		ExpandEffectRects(
			const RECT &rcSrc,
			const RECT &rcDst)
		{
			m_rcSrc = rcSrc;
			m_rcDst = rcDst;
		}

		ExpandEffectRects( const ExpandEffectRects& other)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;
		}

		ExpandEffectRects& operator=( const ExpandEffectRects& other)
		{
			m_rcSrc = other.m_rcSrc;
			m_rcDst = other.m_rcDst;

			return *this;
		}
	}; // struct ExpandEffectRects

	typedef CArray< ExpandEffectRects, ExpandEffectRects&>	expand_effect_rects_container;
	struct __TUI_API VisibleItemDefinion
	{
		BOOL m_bHelperIsSeparator:TRUE;
		BOOL m_bRarelyUsedPrev:TRUE;
		BOOL m_bRarelyUsedNext:TRUE;
		BOOL m_bHelperIsPopup:TRUE;
		BOOL m_bRarelyUsed:TRUE;

		UINT m_nHelperCmdID;
		CRect m_rcItem;
		int m_nIndex;

		VisibleItemDefinion()
		{
			m_bHelperIsSeparator = FALSE;
			m_bRarelyUsedPrev = FALSE;
			m_bRarelyUsedNext = FALSE;
			m_bHelperIsPopup = FALSE;
			m_bRarelyUsed = FALSE;

			m_nHelperCmdID = ID_SEPARATOR;
			m_rcItem.SetRectEmpty();
			m_nIndex = -1;
		}

		VisibleItemDefinion( const VisibleItemDefinion& other)
		{
			AssignFromOther(other);
		}

		~VisibleItemDefinion()
		{
		}

		VisibleItemDefinion& operator=( const VisibleItemDefinion& other)
		{
			AssignFromOther(other);
			return *this;
		}

		void AssignFromOther( const VisibleItemDefinion& other)
		{
			m_bHelperIsSeparator = other.m_bHelperIsSeparator;
			m_bRarelyUsedPrev = other.m_bRarelyUsedPrev;
			m_bRarelyUsedNext = other.m_bRarelyUsedNext;
			m_bHelperIsPopup = other.m_bHelperIsPopup;
			m_bRarelyUsed = other.m_bRarelyUsed;

			m_nHelperCmdID = other.m_nHelperCmdID;
			m_nIndex = other.m_nIndex;
			m_rcItem = other.m_rcItem;
		}

		operator int() const
		{
			return m_nIndex;
		}
	};

	typedef CArray< VisibleItemDefinion, VisibleItemDefinion&>	visible_items;
	void _GetVisibleItems(
		CDC &dc,
		visible_items& v);

	BOOL _TrackPopupMenu(
		DWORD dwTrackFlags,
		int x,
		int y,
		LPCRECT lpRect,
		LPVOID pCbPaintCombinedCookie = NULL,
		pCbPaintCombinedContent pCbContent = NULL);

public:
	void _ItemFocusDelay( int nItemIndex = IDX_NOTHING);
	void _ItemFocusCancel( BOOL bRepaint);
	void _ItemFocusSet(
		int nCurIndex,
		BOOL bEnableDropChild,
		BOOL bRepaint);

	void _GetItemRect(
		int nIndex,
		RECT &rcItem);

public:
	virtual BOOL _OnMouseMove( UINT nFlags, CPoint point);
	virtual BOOL _OnMouseClick( UINT nFlags, CPoint point);
	virtual BOOL _OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	virtual void _OnCancelMode( BOOL bNcDestroy = FALSE);

	virtual void _FreeWinObjects();
	virtual void _SetCapture();

	virtual CTPopupMenuWnd *_GetCapture();
	virtual void _ReleaseCapture();
	virtual void _SyncItems();

	void _CoolTipHide( BOOL bAdvOperation = TRUE);
	void _DoExpand();
	int _GetMaxScrollPos();

	BOOL _StartScrolling( int nButtonIndex);
	BOOL _CoolTipIsVisible();

	e_combine_align m_nCombineAlign;
	CRect m_rcClient;
	CRgn m_rgnWnd;

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();

	enum next_item
	{
		__NI_NOTHING,
		__NI_ANY,
		__NI_NEXT,
		__NI_PREV,
		__NI_PAGE_UP,
		__NI_PAGE_DOWN
	};

	virtual int _GetNextItem( next_item ni);

public:
	/////////////////////////////////////////////////////////////////////////////
	// CTPopupMenuWnd::MENUITEMDATA

	class MENUITEMDATA;
	friend class MENUITEMDATA;

	class __TUI_API MENUITEMDATA
	{
		CTPopupMenuWnd *m_pWndChild;
		int m_nItemIndex;

		CString m_strPopupAccelText;
		CString m_strPopupText;
		CSize m_size;

		BOOL m_bForceDisplayed:TRUE;
		BOOL m_bDisplayed:TRUE;
		BOOL m_bSelected:TRUE;

		HWND m_hWndSpecCmdReciever;
		HICON m_hIconPopup;
		TCHAR m_cAccelChar;
		UINT m_nCmdID;

	public:
		MENUITEMDATA();
		~MENUITEMDATA();

	protected:
		BOOL AccelCharIsSet() const;
		BOOL UpdateFromMenu(
			HWND hWndCmdRecv,
			CMenu *pTrackMenu,
			int nItemIndex);

		BOOL UpdateCmdManagerCommand(
			CTCmdManager::cmd* p_cmd,
			int nItemIndex);

		TCHAR AccelCharGet() const;
		void AccelCharInit();
		void MeasureItem();

	public:
		CTCmdManager::cmd* GetCmd() const;
		HWND GetCmdReciever() const
		{
			return m_hWndSpecCmdReciever;
		};

	protected:
		void SetCmdReciever( HWND hWndSpecCmdReciever)
		{
			m_hWndSpecCmdReciever = hWndSpecCmdReciever;
		};

		void SetDisplayed( BOOL bDisplayed)
		{
			m_bDisplayed = bDisplayed;
		};

		void SetForceDisplayed( BOOL bForceDisplayed)
		{
			m_bForceDisplayed = bForceDisplayed;
		};

	public:
		BOOL IsDisplayed() const
		{
			return m_bDisplayed || m_bForceDisplayed;
		};

		BOOL IsForceDisplayed()
		{
			return m_bForceDisplayed;
		};

		BOOL IsAllItemsRarelyUsed() const
		{
			return m_pWndChild->IsAllItemsRarelyUsed();
		};

		BOOL IsEnabled() const
		{
			if(IsPopup())
				return TRUE;

			if(IsSeparator())
				return FALSE;

			return GetCmd()->StateIsEnable();
		};

		BOOL IsSelected() const
		{
			if(IsSeparator())
				return FALSE;

			return m_bSelected;
		};

	protected:
		void SetSelected( BOOL bOn = TRUE)
		{
			m_bSelected = bOn;
		};

	public:
		BOOL GetRadio() const
		{
			if( IsPopup() || IsSeparator() )
				return FALSE;

			return GetCmd()->StateGetRadio();
		};

		BOOL GetCheck() const
		{
			if( IsPopup() || IsSeparator() )
				return FALSE;

			return GetCmd()->StateGetCheck();
		};

		BOOL GetIndeterminate() const
		{
			if( IsPopup() || IsSeparator() )
				return FALSE;

			return GetCmd()->StateGetIndeterminate();
		};

		LPCTSTR GetText() const
		{
			if(IsSeparator())
				return _T("");

			if(IsPopup())
				return (LPCTSTR) m_strPopupText;

			return GetCmd()->m_strMenuText;
		};

		LPCTSTR GetAccelText() const
		{
			if(IsSeparator())
				return _T("");

			if(IsPopup())
				return (LPCTSTR) m_strPopupAccelText;

			return GetCmd()->m_strAccelText;
		};

		HICON GetIcon()
		{
			if(IsSeparator())
				return NULL;

			if(IsPopup())
				return m_hIconPopup;

			return g_CmdManager->CmdGetHICON( g_CmdManager->ProfileNameFromWnd(GetCmdReciever()), m_nCmdID);
		};

		LPCTSTR GetTip() const
		{
			if( IsPopup() || IsSeparator() )
				return _T("");

			return GetCmd()->m_strTipStatus;
		};

		BOOL IsSeparator() const
		{
			return m_nCmdID == TYPE_SEPARATOR;
		};

	protected:
		void SetSeparator( int nItemIndex)
		{
			if(!IsSeparator())
			{
				if(m_pWndChild)
					DestroyPopup();

				m_nItemIndex = nItemIndex;
				m_nCmdID = TYPE_SEPARATOR;
				m_strPopupText.Empty();
			}

			MeasureItem();
		};

		void SetPopup(
			int nItemIndex,
			LPCTSTR strText = NULL,
			HICON hIcon = NULL,
			CTPopupMenuWnd *pSpecPopup = NULL)
		{
			if(!m_pWndChild)
			{
				if(!pSpecPopup)
					ConstructPopup();
				else
				{
					m_pWndChild = pSpecPopup;
					m_pWndChild->m_hWndCmdReciever = m_hWndSpecCmdReciever;
				}
			}

			m_nCmdID = (UINT) TYPE_POPUP;
			m_nItemIndex = nItemIndex;

			m_strPopupAccelText = _T("");
			m_strPopupText = _T("");

			SetPopupText(strText);
			SetPopupIcon(hIcon);
			MeasureItem();
		};

		BOOL SetPopupIcon( HICON hIcon)
		{
			if(!IsPopup())
				return FALSE;

			if( m_hIconPopup && m_hIconPopup != hIcon )
			{
				DestroyIcon(m_hIconPopup);
			}

			if(!hIcon)
			{
				m_hIconPopup = NULL;
				return TRUE;
			}

			CTCmdManager::icon _icon( hIcon, TRUE);
			m_hIconPopup = _icon.Detach();

			return TRUE;
		};

		BOOL SetPopupText( LPCTSTR strText)
		{
			if(!IsPopup())
				return FALSE;

			m_strPopupText = !strText ? _T("") : strText;

			int nPos = m_strPopupText.Find(_T('\t'));
			if( nPos >= 0 )
			{
				CString strLeft = m_strPopupText.Left(nPos);
				CString strRight = m_strPopupText.Right(m_strPopupText.GetLength() - nPos - 1);

				m_strPopupAccelText = strRight;
				m_strPopupText = strLeft;
			}

			return TRUE;
		};

		BOOL SetPopupAccelText( LPCTSTR strText)
		{
			if(!IsPopup())
				return FALSE;

			m_strPopupAccelText = !strText ? _T("") : strText;
			return TRUE;
		};

		BOOL ConstructPopup();
		void DestroyPopup();

	public:
		CTPopupMenuWnd *GetPopup()
		{
			return m_pWndChild;
		};

		const CTPopupMenuWnd *GetPopup() const
		{
			return m_pWndChild;
		};

		BOOL IsPopup() const
		{
			return m_nCmdID == TYPE_POPUP;
		};

		UINT GetCmdID() const
		{
			if( IsSeparator() || IsPopup() )
				return (UINT) IDC_STATIC;

			return m_nCmdID;
		};

		BOOL IsExecutableCmdID() const
		{
			if( IsSeparator() || IsPopup() )
				return FALSE;

			return TRUE;
		};

		int GetMeasuredHeight() const
		{
			return m_size.cy;
		};

		int GetMeasuredWidth() const
		{
			return m_size.cx;
		};

		CRect GetBaseRect() const
		{
			return CRect(
				CPoint( 0, 0),
				m_size);
		};

		int GetIndex() const
		{
			return m_nItemIndex;
		};

	protected:
		void SetIndex( int nItemIndex)
		{
			m_nItemIndex = nItemIndex;
		};

		friend class CTPopupMenuWnd;
	}; // class MENUITEMDATA

	/////////////////////////////////////////////////////////////////////////////
	// CTPopupMenuWnd::DRAWITEMDATA
	struct DRAWITEMDATA;
	friend struct DRAWITEMDATA;

	struct __TUI_API DRAWITEMDATA
	{
		CTPopupMenuWnd *m_pPopup;
		CDC *m_pDC;

		LPCRECT m_pRectItem;

		const BOOL m_bMenuOwnderDrawFixed:TRUE;
		const BOOL m_bRarelyUsedPreviouse:TRUE;
		const BOOL m_bRarelyUsedNext:TRUE;
		const BOOL m_bRarelyUsed:TRUE;
		const BOOL m_bPopupItem:TRUE;
		const MENUITEMDATA *m_pItemData;

	protected:
		DRAWITEMDATA(
			CDC *pDC,
			LPCRECT pRectItem,
			const MENUITEMDATA *pItemData,
			CTPopupMenuWnd *pPopup,
			BOOL bMenuOwnderDrawFixed,
			BOOL bPopupItem,
			BOOL bRarelyUsed,
			BOOL bRarelyUsedPreviouse,
			BOOL bRarelyUsedNext) : m_pDC(pDC),
			m_pRectItem(pRectItem),
			m_pItemData(pItemData),
			m_pPopup(pPopup),
			m_bMenuOwnderDrawFixed(bMenuOwnderDrawFixed),
			m_bPopupItem(bPopupItem),
			m_bRarelyUsed(bRarelyUsed),
			m_bRarelyUsedPreviouse(bRarelyUsedPreviouse),
			m_bRarelyUsedNext(bRarelyUsedNext)
		{
		};

	public:
		operator CDC *()
		{
			return m_pDC;
		};

		operator LPCRECT()
		{
			return m_pRectItem;
		};

		operator const MENUITEMDATA *()
		{
			return m_pItemData;
		};

		operator HICON()
		{
			return (const_cast <MENUITEMDATA *> (m_pItemData))->GetIcon();
		};

		operator LPCTSTR()
		{
			return m_pItemData->GetText();
		};

		operator CSize()
		{
			return CSize(
				m_pRectItem->right - m_pRectItem->left,
				m_pRectItem->bottom - m_pRectItem->top);
		};

		operator CTPopupMenuWnd *()
		{
			return m_pPopup;
		};

		CTPopupMenuWnd* operator ->()
		{
			return m_pPopup;
		};

		operator LPARAM()
		{
			return reinterpret_cast <LPARAM> (this);
		};

		UINT GetCmdID()
		{
			return m_pItemData->GetCmdID();
		};

		void PaintDefault(
			BOOL bForceNoIcon = FALSE,
			BOOL bForceNoText = FALSE,
			BOOL bForceNoCheck = FALSE,
			BOOL bForceEnabled = FALSE,
			BOOL bForceUnselected = FALSE);

		void PaintDefaultBk()
		{
			PaintDefault(
				TRUE,
				TRUE,
				TRUE,
				TRUE,
				FALSE);
		};

	protected:
		BOOL DoOwnerDrawPainting();
		friend class CTPopupMenuWnd;
	}; // struct DRAWITEMDATA

	/////////////////////////////////////////////////////////////////////////////
	// CTPopupMenuWnd::DRAWLEFTAREADATA
	struct DRAWLEFTAREADATA;
	friend struct DRAWLEFTAREADATA;

	struct __TUI_API DRAWLEFTAREADATA
	{
		CTPopupMenuWnd *m_pPopup;
		CDC *m_pDC;

		LPCRECT m_pRectLeftArea;

	protected:
		DRAWLEFTAREADATA(
			CDC *pDC,
			LPCRECT pRectLeftArea,
			CTPopupMenuWnd *pPopup) : m_pDC(pDC),
			m_pRectLeftArea(pRectLeftArea),
			m_pPopup(pPopup)
		{
		};

	public:
		operator CDC *()
		{
			return m_pDC;
		};

		operator LPCRECT()
		{
			return m_pRectLeftArea;
		};

		operator CSize()
		{
			return CSize(
				m_pRectLeftArea->right - m_pRectLeftArea->left,
				m_pRectLeftArea->bottom - m_pRectLeftArea->top);
		};

		operator CTPopupMenuWnd *()
		{
			return m_pPopup;
		};

		CTPopupMenuWnd *operator ->()
		{
			return m_pPopup;
		};

		operator LPARAM()
		{
			return reinterpret_cast <LPARAM> (this);
		};

	protected:
		BOOL DoOwnerDrawPainting();

		friend class CTPopupMenuWnd;
		friend class CTPopupColorMenuWnd;
	}; // struct DRAWLEFTAREADATA

protected:
	typedef CArray< MENUITEMDATA, MENUITEMDATA&>	items_container;
	typedef const items_container	const_items_container;
	items_container	m_items_all;

	MENUITEMDATA &_GetItemRef( int nIndex);
	const MENUITEMDATA &_GetItemRef( int nIndex) const;
	void _InsertItem(
		int nInsertBefore,
		MENUITEMDATA &mi);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPopupMenuWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTPopupMenuWnd();
	virtual CTPopupMenuSite &GetSite() const;

protected:
	virtual void _StartAnimation();
	virtual void _EndAnimation()
	{
		CTPopupBaseWnd::_EndAnimation();

		Invalidate(FALSE);
		_SetCapture();
	};

protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CTPopupMenuWnd)
	afx_msg void OnCancelMode();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

#if _MFC_VER < 0x700
	afx_msg void OnActivateApp( BOOL bActive, HTASK hTask);
#else
	afx_msg void OnActivateApp( BOOL bActive, DWORD hTask);
#endif
	DECLARE_MESSAGE_MAP()

	class CTPopupMenuCmdUI;
	friend class CTPopupMenuCmdUI;
}; // class CTPopupMenuWnd


class __TUI_API CTPopupColorMenuWnd : public CTPopupMenuWnd
{
public:
	DECLARE_DYNCREATE(CTPopupColorMenuWnd)
	CTPopupColorMenuWnd();

// Attributes
public:
	static UINT m_nMsgNotifyColorChangedFinally;
	static UINT m_nMsgNotifyColorChanged;
	static UINT m_nMsgNotifyCustColor;

	HWND m_hWndNotifyColorChanged;
	LPARAM m_lParamCookie;

	COLORREF m_clrDefault;
	COLORREF m_clrInitial;

	BOOL m_bEnableBtnColorDefault:TRUE;
	BOOL m_bEnableBtnColorCustom:TRUE;

	CString m_strBtnTextColorDefault;
	CString m_strBtnTextColorCustom;

protected:
	CRect m_rcCustColorText;
	CRect m_rcDefColorText;

	enum
	{
		IDX_DEFAULT_COLOR_BTN = -4,
		IDX_CUSTOM_COLOR_BTN = -5
	};

// Implementation
protected:
	int m_nInitialColorIdx;
	int m_nColorIdxCurr;

	int _FindCellByColorRef( COLORREF clr);
	HWND _GetWndNotifyColorChanged();

	virtual BOOL _CreateHelper( CWnd *pWndCmdReciever);
	struct COLORREF_TABLE_ENTRY
	{
		COLORREF m_clr;
		LPCTSTR m_szName;
	};

	static COLORREF_TABLE_ENTRY m_colors[];
	virtual int _HitTest( const CPoint& point)
	{
		return IDX_NOTHING;
	};

	void _NotifyColorChanged( BOOL bFinal = FALSE);
	int _ColorItemHitTest( const CPoint& point);

	CPoint _GetColorItemCoord( int nIdx);
	CRect _GetColorItemRect( int nIdx);

	virtual CRect _CalcTrackRect();
	virtual CSize _CalcTrackSize();
	virtual void _DoPaint(
		CDC &dcPaint,
		BOOL bUseBackBuffer = TRUE);

public:
	virtual BOOL _OnMouseMove( UINT nFlags, CPoint point);
	virtual BOOL _OnMouseClick( UINT nFlags, CPoint point);
	virtual BOOL _OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	// Generated message map functions
	//{{AFX_MSG(CTPopupColorMenuWnd)
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
}; // class CTPopupColorMenuWnd

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if (!defined __TPOPUPMENUWND_H)