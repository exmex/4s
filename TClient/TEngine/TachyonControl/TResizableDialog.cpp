#include "stdafx.h"
#include "TResizableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTResizableDialog

IMPLEMENT_DYNAMIC( CTResizableDialog, CDialog)

CTResizableDialog::CTResizableDialog() : m_bEnabledControlBarUpdate(FALSE), m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = TRUE;
}

CTResizableDialog::CTResizableDialog( UINT nIDTemplate, CWnd *pParentWnd) : CTWA<CTWS<__DLGBASE> >( nIDTemplate, pParentWnd),
	m_bEnabledControlBarUpdate(FALSE),
	m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = TRUE;
}

CTResizableDialog::CTResizableDialog( LPCTSTR lpszTemplateName, CWnd *pParentWnd) : CTWA<CTWS<__DLGBASE> >( lpszTemplateName, pParentWnd),
	m_bEnabledControlBarUpdate(FALSE),
	m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = TRUE;
}

BEGIN_MESSAGE_MAP(CTResizableDialog, CDialog)
	//{{AFX_MSG_MAP(CTResizableDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTResizableDialog message handlers

void CTResizableDialog::OnOK()
{
	DWORD dwStyle = GetStyle();

	if( (dwStyle&WS_CHILD) == 0 )
		CTWA<CTWS<__DLGBASE> >::OnOK();
}

void CTResizableDialog::OnCancel()
{
	DWORD dwStyle = GetStyle();

	if( (dwStyle&WS_CHILD) == 0 )
		CTWA<CTWS<__DLGBASE> >::OnCancel();
}

LRESULT CTResizableDialog::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_DESTROY )
		m_bEnabledControlBarUpdate = FALSE;

	BOOL bUpdateBarsOnThisMsg = FALSE;
	if(m_bEnabledControlBarUpdate)
	{
		CWinApp *pApp = AfxGetApp();
		BOOL bIdleMsg = pApp->IsIdleMessage(
#if _MFC_VER < 0x700
			&pApp->m_msgCur
#else
			&::AfxGetThreadState()->m_msgCur
#endif
			);

		if(bIdleMsg)
			bUpdateBarsOnThisMsg = TRUE;
	}

	LRESULT lResult = CTWA<CTWS<__DLGBASE> >::WindowProc( message, wParam, lParam);
	if( bUpdateBarsOnThisMsg && !m_bInConrolBarUpdate )
	{
		m_bInConrolBarUpdate = TRUE;
		CTControlBar::DoCustomModeUpdateControlBars(this);
		m_bInConrolBarUpdate = FALSE;
	}

	return lResult;
}

void CTResizableDialog::PreSubclassWindow()
{
	CDialog::PreSubclassWindow();
	m_bEnabledControlBarUpdate = TRUE;
	EnableToolTips();
}

BOOL CTResizableDialog::PreTranslateMessage( MSG *pMsg)
{
	if(CTControlBar::DoCustomModePreTranslateMessage( this, pMsg))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// CTResizablePropertyPage

IMPLEMENT_DYNAMIC( CTResizablePropertyPage, CPropertyPage)

CTResizablePropertyPage::CTResizablePropertyPage() : m_bEnabledControlBarUpdate(FALSE), m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = FALSE;
}

CTResizablePropertyPage::CTResizablePropertyPage( UINT nIDTemplate, UINT nIDCaption) : CTWA<CTWS<__PAGEBASE> >( nIDTemplate, nIDCaption),
	m_bEnabledControlBarUpdate(FALSE),
	m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = FALSE;
}

CTResizablePropertyPage::CTResizablePropertyPage( LPCTSTR lpszTemplateName, UINT nIDCaption) : CTWA<CTWS<__PAGEBASE> >( lpszTemplateName, nIDCaption),
	m_bEnabledControlBarUpdate(FALSE),
	m_bInConrolBarUpdate(FALSE)
{
	m_bShowResizingGripper = FALSE;
}

BEGIN_MESSAGE_MAP(CTResizablePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTResizablePropertyPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTResizablePropertyPage message handlers

void CTResizablePropertyPage::OnOK()
{
	DWORD dwStyle = GetStyle();

	if( (dwStyle&WS_CHILD) == 0 )
		CTWA<CTWS<__PAGEBASE> >::OnOK();
}

void CTResizablePropertyPage::OnCancel()
{
	DWORD dwStyle = GetStyle();

	if( (dwStyle & WS_CHILD) == 0 )
		CTWA<CTWS<__PAGEBASE> >::OnCancel();
}

LRESULT CTResizablePropertyPage::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_DESTROY )
		m_bEnabledControlBarUpdate = FALSE;

	BOOL bUpdateBarsOnThisMsg = FALSE;
	if(m_bEnabledControlBarUpdate)
	{
		CWinApp *pApp = AfxGetApp();
		BOOL bIdleMsg = pApp->IsIdleMessage(
#if _MFC_VER < 0x700
			&pApp->m_msgCur
#else
			&::AfxGetThreadState()->m_msgCur
#endif
			);

		if(bIdleMsg)
			bUpdateBarsOnThisMsg = TRUE;
	}

	LRESULT lResult = CTWA<CTWS<__PAGEBASE> >::WindowProc( message, wParam, lParam);
	switch(message)
	{
	case WM_SHOWWINDOW			:
	case WM_WINDOWPOSCHANGED	:
		if(GetStyle()&WS_VISIBLE)
			ArrangeLayout();

		break;
	}

	if( bUpdateBarsOnThisMsg && !m_bInConrolBarUpdate )
	{
		m_bInConrolBarUpdate = TRUE;
		CTControlBar::DoCustomModeUpdateControlBars(this);
		m_bInConrolBarUpdate = FALSE;
	}

	return lResult;
}

void CTResizablePropertyPage::PreSubclassWindow()
{
	CPropertyPage::PreSubclassWindow();
	m_bEnabledControlBarUpdate = TRUE;
	EnableToolTips();
}

BOOL CTResizablePropertyPage::PreTranslateMessage( MSG* pMsg)
{
	if(CTControlBar::DoCustomModePreTranslateMessage( this, pMsg))
		return TRUE;

	return CPropertyPage::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CTResizablePropertySheet

IMPLEMENT_DYNAMIC( CTResizablePropertySheet, CPropertySheet)

CTResizablePropertySheet::CTResizablePropertySheet() : m_bSheetInitialized(FALSE)
{
	m_bShowResizingGripper = TRUE;
}

CTResizablePropertySheet::CTResizablePropertySheet( UINT nIDCaption,
												    CWnd *pParentWnd,
													UINT nSelectPage) : CTWA<CTWS<__SHEETBASE> >( nIDCaption, pParentWnd, nSelectPage),
													m_bSheetInitialized(FALSE)
{
	m_bShowResizingGripper = TRUE;
}

CTResizablePropertySheet::CTResizablePropertySheet( LPCTSTR pszCaption,
												    CWnd *pParentWnd,
													UINT nSelectPage) : CTWA<CTWS<__SHEETBASE> >( pszCaption, pParentWnd, nSelectPage),
													m_bSheetInitialized(FALSE)
{
	m_bShowResizingGripper = TRUE;
}


BEGIN_MESSAGE_MAP(CTResizablePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CTResizablePropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTResizablePropertySheet message handlers

void CTResizablePropertySheet::PreSubclassWindow()
{
	CPropertySheet::PreSubclassWindow();
	EnableToolTips();
}

void CTResizablePropertySheet::_SyncActivePage()
{
	CPropertyPage *pWnd = CPropertySheet::GetActivePage();

	if( pWnd != NULL &&
		pWnd->GetSafeHwnd() != NULL &&
		::IsWindow(pWnd->GetSafeHwnd()))
	{
		RDI _val;

		if(!m_mapRDI.Lookup( pWnd->GetSafeHwnd(), _val))
			AddAnchor( pWnd->GetSafeHwnd(), __RDA_LT, __RDA_RB);
	}
}

LRESULT CTResizablePropertySheet::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	CPropertyPage *pWndPrevActivePage = NULL;

	switch(message)
	{
	case WM_CLOSE			:
		if(m_bEnableSaveRestore)
			SaveWindowRect();

		break;

	case WM_SYSCOMMAND		:
		if( wParam == SC_CLOSE && m_bEnableSaveRestore )
			SaveWindowRect();

		break;

	case WM_COMMAND			:
		if( wParam == IDOK || wParam == IDCANCEL )
		{
			if(m_bEnableSaveRestore)
				SaveWindowRect();
		}
		else if( wParam == ID_WIZBACK || wParam == ID_WIZNEXT )
			pWndPrevActivePage = CPropertySheet::GetActivePage();

		break;

	case WM_DESTROY			:
	case WM_NCDESTROY		: m_bSheetInitialized = FALSE; break;
	case WM_SIZE			: _SyncActivePage(); break;
	}

	LRESULT lResult = m_bSheetInitialized ? CTWA<CTWS<__SHEETBASE> >::WindowProc( message, wParam, lParam) : CPropertySheet::WindowProc( message, wParam, lParam);
	switch(message)
	{
	case WM_CREATE		:
		{
			ModifyStyle( 0, WS_THICKFRAME|WS_CLIPCHILDREN);
			::PostMessage( m_hWnd, WM_USER + 10, 0, 0);
		}

		break;

	case WM_USER + 10	:
		{
			if(m_bSheetInitialized)
				break;

			m_bSheetInitialized = TRUE;
			CRect rcWnd;
			GetWindowRect(&rcWnd);
			CSize szWnd = rcWnd.Size();

			SetMinTrackSize(szWnd);
			::SetWindowPos(
				m_hWnd,
				NULL,
				0, 0,
				0, 0,
				SWP_NOSIZE|
				SWP_NOMOVE|
				SWP_NOZORDER|
				SWP_NOOWNERZORDER|
				SWP_NOREPOSITION|
				SWP_FRAMECHANGED);

			CTabCtrl *pTabCtrl = CPropertySheet::GetTabControl();
			CWnd *pWnd = GetWindow(GW_CHILD);

			for(; pWnd != NULL; pWnd = pWnd->GetWindow(GW_HWNDNEXT))
			{
				if( pTabCtrl == pWnd )
				{
					pTabCtrl->ModifyStyle( 0, WS_CLIPSIBLINGS);
					AddAnchor( pTabCtrl->GetSafeHwnd(), __RDA_LT, __RDA_RB);

					continue;
				}

				CString strClassName;
				::GetClassName(
					pWnd->GetSafeHwnd(),
					strClassName.GetBuffer(_MAX_PATH + 1),
					_MAX_PATH);

				strClassName.ReleaseBuffer();
				strClassName.MakeLower();

				if( strClassName == _T("static") )
				{
					pWnd->ModifyStyle( 0, WS_CLIPSIBLINGS);
					AddAnchor( pWnd->GetSafeHwnd(), __RDA_LB, __RDA_RB);

					continue;
				}

				if( strClassName == _T("button") )
				{
					CRect rcWndButton;
					pWnd->GetWindowRect(&rcWndButton);
					ScreenToClient(&rcWndButton);

					rcWndButton.OffsetRect( -10, 0);
					pWnd->MoveWindow(&rcWndButton);

					pWnd->ModifyStyle( 0, WS_CLIPSIBLINGS);
					AddAnchor( pWnd->GetSafeHwnd(), __RDA_RB);

					CButtonInSheetHook *pBtn = new CButtonInSheetHook();
					pBtn->SubclassWindow(pWnd->GetSafeHwnd());

					continue;
				}
			}
			_SyncActivePage();

			CRect rcDesktop( 0, 0, 0, 0);
			::SystemParametersInfo(
				SPI_GETWORKAREA, 0,
				&rcDesktop, 0);

			SetMaximizedRect(&rcDesktop);
			CSize szDesktop = rcDesktop.Size();
			SetMaxTrackSize(szDesktop);

			CRect _rcWnd;
			GetWindowRect(&_rcWnd);

			CSize _size = _rcWnd.Size();
			::SendMessage(
				m_hWnd,
				WM_SIZE,
				WPARAM(SIZE_RESTORED),
				MAKELPARAM( _size.cx, _size.cy));

			if(GetStyle()&WS_VISIBLE)
				RedrawWindow();

			OnResizablePropertySheetInitialized();
		}

		break;

	case WM_COMMAND		:
		if( wParam == ID_WIZBACK || wParam == ID_WIZNEXT )
		{
			if(!m_bSheetInitialized)
				break;

			if( pWndPrevActivePage != NULL )
			{
				CPropertyPage *pWndNewActivePage = CPropertySheet::GetActivePage();

				if( pWndPrevActivePage != pWndNewActivePage )
				{
					CRect rcWndPage;

					pWndPrevActivePage->GetWindowRect(&rcWndPage);
					ScreenToClient(&rcWndPage);
					pWndNewActivePage->MoveWindow(&rcWndPage);
				}
			}

			_SyncActivePage();
			ArrangeLayout();
		}

		break;
	}

	return lResult;
}

BOOL CTResizablePropertySheet::PreCreateWindow( CREATESTRUCT& cs)
{
	return CTWA<CTWS<__SHEETBASE> >::PreCreateWindow(cs);
}

void CTResizablePropertySheet::OnResizablePropertySheetInitialized()
{
	if(m_bEnableSaveRestore)
		LoadWindowRect();
}
