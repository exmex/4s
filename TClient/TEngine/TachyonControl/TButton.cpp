#include "stdafx.h"
#include "TButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTButton::CTButton()
{
	m_ToolTip.m_hWnd = NULL;
	m_hIconOut = NULL;
	m_hIconIn = NULL;
	m_hCursor = NULL;

	m_bDrawBorder = TRUE;
	m_bIsFlat = FALSE;

	m_bPushTracking = FALSE;
	m_bKeyTracking = FALSE;
	m_bMouseOver = FALSE;
	m_bPushed = FALSE;

	m_nAlign = CTPaintManager::e_align::__ALIGN_HORIZ_CENTER|CTPaintManager::e_align::__ALIGN_VERT;
	m_bIsDefault = FALSE;

	m_dwMenuOpt = TPMX_TOPALIGN;
	m_nMenuLeftAreaWidth = 0;
}

CTButton::~CTButton()
{
	_FreeWinObjects();
}

void CTButton::_FreeWinObjects()
{
	if(m_hIconIn)
		::DestroyIcon(m_hIconIn);

	if(m_hIconOut)
		::DestroyIcon(m_hIconOut);

	if(m_hCursor)
		::DestroyCursor(m_hCursor);
}


IMPLEMENT_DYNCREATE(CTButton, CButton);

BEGIN_MESSAGE_MAP(CTButton, CButton)
    //{{AFX_MSG_MAP(CTButton)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CTToolControlBar::m_nMsgPutToPopupMenu, _OnPutToPopupMenuImpl)
END_MESSAGE_MAP()


LRESULT CTButton::_OnPutToPopupMenuImpl( WPARAM wParam, LPARAM lParam)
{
	CTPopupMenuWnd *pPopup = reinterpret_cast <CTPopupMenuWnd *> (wParam);
	return OnPutToPopupMenu(pPopup);
}

BOOL CTButton::OnPutToPopupMenu( CTPopupMenuWnd *pPopup)
{
	if(m_menu.GetSafeHmenu())
	{
		CTPopupMenuWnd *pPopupIns = new CTPopupMenuWnd();

		pPopupIns->CreatePopupMenu(pPopup->GetCmdRecieverHWND());
		pPopupIns->UpdateFromMenu(
			pPopup->GetCmdRecieverHWND(),
			&m_menu);

		HICON hIcon = NULL;
		if(m_hIconIn)
		{
			CTCmdManager::icon _icon( m_hIconIn, TRUE);
			hIcon = _icon.Detach();
		}

		CString strPopupText(_T(""));
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
			GetDlgCtrlID());

		if(p_cmd)
			strPopupText = p_cmd->m_strMenuText;

		if(strPopupText.IsEmpty())
			GetWindowText(strPopupText);

		pPopup->ItemInsertSpecPopup(
			pPopupIns, -1,
			LPCTSTR(strPopupText),
			hIcon);

		return TRUE;
	}

	return FALSE;
}

void CTButton::SetIcon( int nIconInId, int nIconOutId)
{
	HICON hIconOut;
	HICON hIconIn;

	HINSTANCE hInstResource = AfxFindResourceHandle(
		MAKEINTRESOURCE(nIconInId),
		RT_GROUP_ICON);

	hIconIn = (HICON)::LoadImage(
		hInstResource,
		MAKEINTRESOURCE(nIconInId),
		IMAGE_ICON,
		0, 0, 0);

	hIconOut = (nIconOutId == NULL) ? NULL : (HICON)::LoadImage(
		hInstResource,
		MAKEINTRESOURCE(nIconOutId),
		IMAGE_ICON,
		0, 0, 0);

	SetIcon( hIconIn, hIconOut);
}

void CTButton::SetIcon( HICON hIconIn, HICON hIconOut)
{
	if(m_hIconIn)
		::DestroyIcon(m_hIconIn);

	if(m_hIconOut)
		::DestroyIcon(m_hIconOut);

	m_hIconIn = hIconIn;
	m_hIconOut = (hIconOut == NULL) ? m_hIconIn : hIconOut;

	Invalidate();
	UpdateWindow();
}

BOOL CTButton::SetBtnCursor( int nCursorId)
{
	HINSTANCE hInstResource;

	if(m_hCursor)
		::DestroyCursor(m_hCursor);

	m_hCursor = NULL;
	if( nCursorId != -1 )
	{
		hInstResource = AfxFindResourceHandle(
			MAKEINTRESOURCE(nCursorId),
			RT_GROUP_CURSOR);

		m_hCursor = (HCURSOR)::LoadImage(
			hInstResource,
			MAKEINTRESOURCE(nCursorId),
			IMAGE_CURSOR,
			0, 0, 0);

		if(!m_hCursor)
		{
			m_hCursor = ::LoadCursor(
				NULL,
				MAKEINTRESOURCE(nCursorId));

			if(!m_hCursor)
				return FALSE;
		}
	}

	return TRUE;
}

void CTButton::SetFlat( BOOL bState)
{
	m_bIsFlat = bState;
	Invalidate();
	UpdateWindow();
}

BOOL CTButton::GetFlat()
{
	return m_bIsFlat;
}

void CTButton::SetAlign( int nAlign)
{
	switch(nAlign)
	{
	case CTPaintManager::e_align::__ALIGN_HORIZ_CENTER	: m_nAlign = CTPaintManager::e_align::__ALIGN_HORIZ_CENTER; break;
	case CTPaintManager::e_align::__ALIGN_HORIZ_RIGHT	: m_nAlign = CTPaintManager::e_align::__ALIGN_HORIZ_RIGHT; break;
	case CTPaintManager::e_align::__ALIGN_VERT			: m_nAlign = CTPaintManager::e_align::__ALIGN_VERT; break;
	}

	Invalidate();
	UpdateWindow();
}

int CTButton::GetAlign()
{
	return m_nAlign;
}

void CTButton::SetDrawBorder( BOOL bEnable)
{
	m_bDrawBorder = bEnable;
	Invalidate();
	UpdateWindow();
}

BOOL CTButton::_IsMouseOnButton()
{
	POINT point;

	GetCursorPos(&point);
	CWnd *pWndUnderMouse = WindowFromPoint(point);

	if( pWndUnderMouse->GetSafeHwnd() == m_hWnd )
		return TRUE;

	return FALSE;
}

void CTButton::OnMouseMove( UINT nFlags, CPoint point)
{
	if(m_bKeyTracking)
		return;

	CButton::OnMouseMove( nFlags, point);
	if(_IsMouseOnButton())
	{
		if( GetCapture() != this )
			SetCapture();

		if(m_bMouseOver)
			return;

		m_bMouseOver = TRUE;
		m_bPushed = (nFlags & MK_LBUTTON) && m_bPushTracking;

		Invalidate();
		UpdateWindow();
	}
	else
	{
		if( GetCapture() == this && !m_bPushTracking )
			ReleaseCapture();

		if(!m_bMouseOver)
			return;

		m_bMouseOver = FALSE;
		m_bPushed = FALSE;

		Invalidate();
		UpdateWindow();
	}
}

void CTButton::OnKillFocus( CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	if( m_bMouseOver || m_bPushed )
	{
		if( GetCapture() == this )
			ReleaseCapture();

		m_bMouseOver = FALSE;
		m_bPushed = FALSE;

		Invalidate();
		UpdateWindow();
	}
}

void CTButton::OnCaptureChanged( CWnd *pWnd)
{
	if( (m_bMouseOver || m_bPushed) && pWnd != this )
	{
		m_bMouseOver = FALSE;
		m_bPushed = FALSE;

		Invalidate();
		UpdateWindow();
	}

	CButton::OnCaptureChanged(pWnd);
}

void CTButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	UINT nBS = GetButtonStyle();

	if(nBS&BS_DEFPUSHBUTTON)
		m_bIsDefault = TRUE;

	if(nBS&BS_OWNERDRAW)
		SetButtonStyle(nBS&~BS_OWNERDRAW);

	SetFont( m_bIsDefault ? &g_PaintManager->m_FontBold : &g_PaintManager->m_FontNormal);
}

BOOL CTButton::PreTranslateMessage( MSG* pMsg)
{
	if(!CTPopupMenuWnd::IsMenuTracking())
	{
		InitToolTip();
		m_ToolTip.RelayEvent(pMsg);
	}

	if( (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) &&
		(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		if( IsWindowEnabled() && IsWindowVisible() && !m_bPushTracking )
		{
			if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
				(pMsg->message == WM_KEYUP && pMsg->wParam == VK_SPACE))
			{
				if( GetCapture() == this )
					ReleaseCapture();

				m_bPushTracking = FALSE;
				m_bKeyTracking = FALSE;
				m_bMouseOver = FALSE;
				m_bPushed = FALSE;

				Invalidate();
				UpdateWindow();
				_OnClick();

				return TRUE;
			}

			if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
			{
				m_bMouseOver = TRUE;
				m_bPushed = TRUE;
				m_bKeyTracking = TRUE;

				SetCapture();
				Invalidate();
				UpdateWindow();

				return TRUE;
			}
		}

		return TRUE;
	}

	return CButton::PreTranslateMessage(pMsg);
}

LRESULT CTButton::DefWindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bInvalidate = FALSE;

	switch(message)
	{
	case WM_LBUTTONDBLCLK		: message = WM_LBUTTONDOWN; break;
	case BM_SETCHECK			:
	case BM_SETSTATE			:
	case BM_SETSTYLE			:
#if (WINVER >= 0x0400)
	case BM_CLICK				:
	case BM_SETIMAGE			:
#endif
	case WM_SETTEXT				:
	case WM_SIZE				:
	case WM_WINDOWPOSCHANGED	: bInvalidate = TRUE; break;
	}

	LRESULT lResult = CButton::DefWindowProc( message, wParam, lParam);
	if(bInvalidate)
	{
		Invalidate();
		UpdateWindow();
	}

	return lResult;
}

BOOL CTButton::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CButton::OnSetCursor( pWnd, nHitTest, message);
}

void CTButton::SetTooltipText( CString *pText, BOOL bActivate)
{
	if(!pText)
		return;

	InitToolTip();
	if(!m_ToolTip.GetToolCount())
	{
		CRect rcBtn;

		GetClientRect(rcBtn);
		m_ToolTip.AddTool(
			this,
			(LPCTSTR) *pText,
			rcBtn, 1);
	}

	m_ToolTip.UpdateTipText( (LPCTSTR) *pText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CTButton::SetTooltipText( CString &strText, BOOL bActivate)
{
	if(strText.IsEmpty())
		return;

	InitToolTip();
	if(!m_ToolTip.GetToolCount())
	{
		CRect rcBtn;

		GetClientRect(rcBtn);
		m_ToolTip.AddTool( this, (LPCTSTR) strText, rcBtn, 1);
	}

	m_ToolTip.UpdateTipText( (LPCTSTR) strText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CTButton::SetTooltipText( LPCTSTR strText, BOOL bActivate)
{
	if( !strText || !_tcslen(strText) )
		return;

	InitToolTip();
	if(!m_ToolTip.GetToolCount())
	{
		CRect rcBtn;

		GetClientRect(rcBtn);
		m_ToolTip.AddTool( this, strText, rcBtn, 1);
	}

	m_ToolTip.UpdateTipText( strText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CTButton::SetTooltipText( int nId, BOOL bActivate)
{
	CString strText;

	strText.LoadString(nId);
	if(!strText.IsEmpty())
		SetTooltipText( &strText, bActivate);
}

void CTButton::ActivateTooltip( BOOL bActivate)
{
	if(!m_ToolTip.GetToolCount())
		return;

	m_ToolTip.Activate(bActivate);
}

BOOL CTButton::GetDefault()
{
	return m_bIsDefault;
}

void CTButton::InitToolTip()
{
	if(!m_ToolTip.m_hWnd)
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);
	}
}

void CTButton::OnSysColorChange()
{
	CButton::OnSysColorChange();

	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CTButton::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(m_bKeyTracking)
		return;

	CButton::OnLButtonDown( nFlags, point);
	if(!_IsMouseOnButton())
		return;

	m_bMouseOver = TRUE;
	m_bPushed = TRUE;
	m_bPushTracking = TRUE;

	SetCapture();
	Invalidate();
	UpdateWindow();
}

void CTButton::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_bKeyTracking)
		return;

	BOOL bWasPushed = m_bPushed && m_bPushTracking;
	m_bMouseOver = FALSE;
	m_bPushed = FALSE;
	m_bPushTracking = FALSE;

	if( GetCapture() == this )
		ReleaseCapture();

	Invalidate();
	UpdateWindow();

	if(!bWasPushed)
		return;

	if( IsWindowEnabled() && IsWindowVisible() )
		_OnClick();
}

void CTButton::_OnClick()
{
	if(_OnTrackPopup())
		return;

	CWnd *pWnd = GetParent();
	if(!m_menu.GetSafeHmenu())
	{
		HWND hWndThis = GetSafeHwnd();

		pWnd->SendMessage(
			WM_COMMAND,
			MAKEWPARAM( GetDlgCtrlID(), BN_CLICKED),
			(LPARAM) m_hWnd);

		if(::IsWindow(hWndThis))
		{
			Invalidate();
			UpdateWindow();
		}

		return;
	}
}

BOOL CTButton::_IsMenuAvail()
{
	if(!m_menu.GetSafeHmenu())
		return FALSE;

	return TRUE;
}

BOOL CTButton::_OnTrackPopup()
{
	if(!m_menu.GetSafeHmenu())
		return FALSE;

	g_CmdManager->UpdateFromMenu(
		g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
		m_menu.GetSafeHmenu());

	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();
	pPopup->SetLeftAreaWidth(m_nMenuLeftAreaWidth);

	CWnd *pWnd = GetParent();
	DWORD dwAlignOpt = 0;
	DWORD dwMenuOpt = m_dwMenuOpt;

	while(pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		if( dwAlignOpt == 0 && ((CControlBar *) pWnd)->m_pDockBar )
		{
			UINT nDockBarID = ((CControlBar *) pWnd)->m_pDockBar->GetDlgCtrlID();
			dwMenuOpt &= ~TPMX_ALIGN_MASK;

			switch(nDockBarID)
			{
			case AFX_IDW_DOCKBAR_TOP		: dwAlignOpt = TPMX_TOPALIGN; break;
			case AFX_IDW_DOCKBAR_BOTTOM		: dwAlignOpt = TPMX_BOTTOMALIGN; break;
			case AFX_IDW_DOCKBAR_LEFT		: dwAlignOpt = TPMX_LEFTALIGN; break;
			case AFX_IDW_DOCKBAR_RIGHT		: dwAlignOpt = TPMX_RIGHTALIGN; break;
			case AFX_IDW_DOCKBAR_FLOAT		: dwAlignOpt = TPMX_TOPALIGN; break;
			}
		}
		pWnd = pWnd->GetParent();

		if(pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		{
			pWnd = pWnd->GetParentFrame();
			break;
		}
	}

	pPopup->UpdateFromMenu( pWnd->GetSafeHwnd(), &m_menu);
	CRect rect;

	GetWindowRect(&rect);
	CPoint point = rect.CenterPoint();

	SetFocus();
	pPopup->TrackPopupMenu(
		dwMenuOpt|
		dwAlignOpt|
		TPMX_SELECT_ANY|
		TPMX_COMBINE_DEFAULT,
		point.x,
		point.y,
		&rect,
		this,
		_CbPaintCombinedContent);

	return TRUE;
}

void CTButton::_CbPaintCombinedContent( LPVOID pCookie,
									    CDC& dc,
										const CWnd& refWndMenu,
										const CRect& rcExcludeArea,
										int nCombineAlign)
{
	CTButton *pBtn = (CTButton *) pCookie;

	if(rcExcludeArea.IsRectEmpty())
		return;

	CRect rcClientExcludeArea;
	CRect rcClientBar;

	pBtn->GetClientRect(&rcClientBar);
	pBtn->ClientToScreen(&rcClientBar);

	refWndMenu.GetClientRect(&rcClientExcludeArea);
	refWndMenu.ClientToScreen(&rcClientExcludeArea);

	CPoint ptOffset = rcClientBar.TopLeft() - rcClientExcludeArea.TopLeft();
	CPoint ptViewportOrg = dc.GetViewportOrg();

	dc.SetViewportOrg(ptOffset);
	pBtn->_RenderImpl(dc,true);
	dc.SetViewportOrg( ptViewportOrg );
}

void CTButton::_RenderImpl( CDC &dc, BOOL bTransparent)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CString strWindowText(_T(""));
	GetWindowText(strWindowText);

	BOOL bDrawFocusRect = GetFocus() == this;
	BOOL bDefault = GetDefault();

	BOOL bPushed = m_bPushed || GetCheck();
	BOOL bEnabled = IsWindowEnabled();
	BOOL bFlat = GetFlat();
	BOOL bHover = m_bMouseOver && !CTPopupMenuWnd::IsMenuTracking();

	HICON hIcon = bHover ? m_hIconIn : m_hIconOut;
	if(GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		bDrawFocusRect = FALSE;
		bFlat = TRUE;
	}

	g_PaintManager->PaintPushButton(
		dc, TRUE,
		rcClient,
		(LPCTSTR) strWindowText,
		hIcon,
		bFlat,
		bHover,
		bPushed,
		FALSE,
		bEnabled,
		m_bDrawBorder,
		bDrawFocusRect,
		bDefault,
		m_nAlign,
		NULL,
		_IsMenuAvail(), 0,
		bTransparent);
}

void CTButton::_DrawImpl( CDC &dc, BOOL bTransparent)
{
	CRect rcClient;

	GetClientRect(&rcClient);
	CTMemoryDC dcmm( &dc, &rcClient);
	_RenderImpl( dcmm, bTransparent);
}

void CTButton::OnPaint()
{
	CPaintDC dcPaint(this);
	_DrawImpl(dcPaint);
}

BOOL CTButton::OnEraseBkgnd( CDC *pDC)
{
	return TRUE;
}

void CTButton::OnEnable( BOOL bEnable)
{
	CButton::OnEnable(bEnable);

	CClientDC dc(this);
	_DrawImpl(dc);
}

CTColorButton::CTColorButton()
{
#if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)
	m_bUseStdColorDlg = FALSE;
#else
	m_bUseStdColorDlg = TRUE;
#endif // #if (defined _AFXDLL && !defined __TUI_WITH_DLLMFC__)
	m_bEnableBtnColorDefault = TRUE;
	m_bEnableBtnColorCustom = TRUE;
	m_clrDefault = (COLORREF) (-1);
	m_clrSelected = RGB( 0, 0, 0);
	m_lParamCookie = 0;

	m_strBtnTextColorDefault = _T("");
	m_strBtnTextColorCustom = _T("");
}

CTColorButton::~CTColorButton()
{
	_FreeWinObjects();
}

IMPLEMENT_DYNCREATE(CTColorButton, CTButton);

BEGIN_MESSAGE_MAP(CTColorButton, CTButton)
    //{{AFX_MSG_MAP(CTColorButton)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( CTPopupColorMenuWnd::m_nMsgNotifyColorChanged, OnColorChanged)
	ON_REGISTERED_MESSAGE( CTPopupColorMenuWnd::m_nMsgNotifyColorChangedFinally, OnColorChangedFinally)
	ON_REGISTERED_MESSAGE( CTPopupColorMenuWnd::m_nMsgNotifyCustColor, OnColorSelectCustom)
END_MESSAGE_MAP()

BOOL CTColorButton::_IsMenuAvail()
{
	return TRUE;
}

BOOL CTColorButton::_OnTrackPopup()
{
	BOOL bInBar = FALSE;
	CWnd *pWnd = GetParent();
	DWORD dwAlignOpt = 0;
	DWORD dwMenuOpt = m_dwMenuOpt;

	while(pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		bInBar = TRUE;

		if( !dwAlignOpt && ((CControlBar *) pWnd)->m_pDockBar )
		{
			dwMenuOpt &= ~TPMX_ALIGN_MASK;
			UINT nDockBarID = ((CControlBar *) pWnd)->m_pDockBar->GetDlgCtrlID();

			switch(nDockBarID)
			{
			case AFX_IDW_DOCKBAR_TOP		: dwAlignOpt = TPMX_TOPALIGN; break;
			case AFX_IDW_DOCKBAR_BOTTOM		: dwAlignOpt = TPMX_BOTTOMALIGN; break;
			case AFX_IDW_DOCKBAR_LEFT		: dwAlignOpt = TPMX_LEFTALIGN; break;
			case AFX_IDW_DOCKBAR_RIGHT		: dwAlignOpt = TPMX_RIGHTALIGN; break;
			case AFX_IDW_DOCKBAR_FLOAT		: dwAlignOpt = TPMX_TOPALIGN; break;
			}
		}
		pWnd = pWnd->GetParent();

		if(pWnd->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		{
			pWnd = pWnd->GetParentFrame();
			break;
		}
	}

	CTPopupColorMenuWnd *pPopup = new CTPopupColorMenuWnd;
	pPopup->SetLeftAreaWidth(m_nMenuLeftAreaWidth);
	pPopup->m_lParamCookie = m_lParamCookie;

	pPopup->CreatePopupMenu( bInBar ? pWnd->GetSafeHwnd() : GetSafeHwnd());
	pPopup->m_bEnableBtnColorDefault = m_bEnableBtnColorDefault;
	pPopup->m_bEnableBtnColorCustom = m_bEnableBtnColorCustom;
	pPopup->m_clrDefault = m_clrDefault;
	pPopup->m_clrInitial = m_clrSelected;

	if(!m_strBtnTextColorDefault.IsEmpty())
		pPopup->m_strBtnTextColorDefault = m_strBtnTextColorDefault;

	if(!m_strBtnTextColorCustom.IsEmpty())
		pPopup->m_strBtnTextColorCustom = m_strBtnTextColorCustom;

	CRect rect;
	GetWindowRect(&rect);

	CPoint point = rect.CenterPoint();
	if(bInBar)
		pWnd->SetFocus();
	else
		SetFocus();

	pPopup->TrackPopupMenu(
		dwMenuOpt|
		dwAlignOpt|
		TPMX_SELECT_ANY|
		TPMX_COMBINE_DEFAULT,
		point.x,
		point.y,
		&rect,
		this,
		_CbPaintCombinedContent);

	return TRUE;
}

HICON CTColorButton::GenerateColorIcon( COLORREF clrIcon)
{
	CWindowDC dc(NULL);
	ICONINFO iiNewIcon;

	iiNewIcon.fIcon = TRUE;
	HDC hIconDC = ::CreateCompatibleDC(dc.GetSafeHdc());
	HDC hMaskDC = ::CreateCompatibleDC(dc.GetSafeHdc());

	iiNewIcon.hbmColor = ::CreateCompatibleBitmap(
		dc.GetSafeHdc(),
		16, 16);

	iiNewIcon.hbmMask = ::CreateCompatibleBitmap(
		hMaskDC,
		16, 16);

	HGDIOBJ hOldIconDC = ::SelectObject(
		hIconDC,
		iiNewIcon.hbmColor);

	HGDIOBJ hOldMaskDC = ::SelectObject(
		hMaskDC,
		iiNewIcon.hbmMask);

	CBrush brColorItem(clrIcon);
	CRect rcColorItem( 0, 0, 16, 16);

	::FillRect(
		hIconDC,
		&rcColorItem,
		(HBRUSH) brColorItem.GetSafeHandle());

	::FillRect(
		hMaskDC,
		&rcColorItem,
		(HBRUSH) ::GetStockObject(BLACK_BRUSH));

	::SelectObject( hIconDC, hOldIconDC);
	::SelectObject( hMaskDC, hOldMaskDC);

	HICON hIcon = CreateIconIndirect(&iiNewIcon);
	DeleteObject(iiNewIcon.hbmColor);
	DeleteObject(iiNewIcon.hbmMask);
	DeleteDC(hMaskDC);
	DeleteDC(hIconDC);

	return hIcon;
}

void CTColorButton::OnGenerateColorBtnIcon( HICON *p_hIcon, BOOL *pDestroyIcon)
{
	*p_hIcon = GenerateColorIcon(m_clrSelected);
	*pDestroyIcon = FALSE;
}

void CTColorButton::_RenderImpl( CDC &dc, BOOL bTransparent)
{
	HICON hIcon = NULL;
	BOOL bDestroyIcon = FALSE;

	OnGenerateColorBtnIcon( &hIcon, &bDestroyIcon);
	CRect rcClient;

	GetClientRect(&rcClient);
	CString strWindowText(_T(""));
	GetWindowText(strWindowText);

	BOOL bDrawFocusRect = GetFocus() == this;
	BOOL bDefault = GetDefault();
	BOOL bPushed = m_bPushed || GetCheck();
	BOOL bEnabled = IsWindowEnabled();
	BOOL bFlat = GetFlat();
	BOOL bHover = m_bMouseOver && !CTPopupMenuWnd::IsMenuTracking();

	if(GetParent()->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		bDrawFocusRect = FALSE;
		bFlat = TRUE;
	}

	g_PaintManager->PaintPushButton(
		dc, TRUE,
		rcClient,
		(LPCTSTR) strWindowText,
		hIcon,
		bFlat,
		bHover,
		bPushed,
		FALSE,
		bEnabled,
		m_bDrawBorder && !CTPopupMenuWnd::IsMenuTracking(),
		bDrawFocusRect && !CTPopupMenuWnd::IsMenuTracking() && !strWindowText.IsEmpty(),
		bDefault,
		m_nAlign,
		NULL,
		_IsMenuAvail(), 0,
		bTransparent);

	if( hIcon && bDestroyIcon )
		::DestroyIcon(hIcon);
}

LRESULT CTColorButton::OnColorChanged( WPARAM wParam, LPARAM lParam)
{
	return GetParent()->SendMessage(
		CTPopupColorMenuWnd::m_nMsgNotifyColorChanged,
		wParam,
		lParam);
}

LRESULT CTColorButton::OnColorChangedFinally( WPARAM wParam, LPARAM lParam)
{
	COLORREF clr = (COLORREF) wParam;

	m_clrSelected = clr == ((COLORREF) (-1)) ? RGB( 0, 0, 0) : clr;
	Invalidate();
	UpdateWindow();

	return GetParent()->SendMessage(
		CTPopupColorMenuWnd::m_nMsgNotifyColorChangedFinally,
		wParam,
		lParam);
}

LRESULT CTColorButton::OnColorSelectCustom( WPARAM wParam, LPARAM lParam)
{
	if(m_bUseStdColorDlg)
	{
		CColorDialog dlg(
			m_clrSelected,
			CC_FULLOPEN,
			this);

		if( dlg.DoModal() != IDOK )
			return 0;

		m_clrSelected = dlg.m_cc.rgbResult;
	}
	else
	{
		CTColorDlg dlg(
			m_clrSelected,
			m_clrSelected,
			this);

		if( dlg.DoModal() != IDOK )
			return 0;

		m_clrSelected = dlg.m_clrNew;
	}

	GetParent()->SendMessage(
		CTPopupColorMenuWnd::m_nMsgNotifyColorChangedFinally,
		WPARAM(m_clrSelected),
		lParam);

	Invalidate();
	UpdateWindow();

	return 0;
}

LRESULT CTButton::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == CTPopupMenuWnd::m_nMsgPrepareMenu ||
		message == CTPopupMenuWnd::m_nMsgPopupNext ||
		message == CTPopupMenuWnd::m_nMsgPopupPrev ||
		message == CTPopupMenuWnd::m_nMsgNotifyMenuExpanded ||
		message == CTPopupMenuWnd::m_nMsgNotifyMenuClosed ||
		message == CTPopupMenuWnd::m_nMsgPopupDrawItem ||
		message == CTPopupMenuWnd::m_nMsgPopupDrawLeftArea)
	{
		CWnd *pParent = GetParent();

		if(pParent)
			return pParent->SendMessage( message, wParam, lParam);
		else
			return 0;
	}

	return CButton::WindowProc( message, wParam, lParam);
}
