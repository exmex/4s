#include "stdafx.h"
#include "TMiniDockFrameWnd.h"
#include "TMenuControlBar.h"
#include "TDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define __MENUBAR_RIGHT_ADJUST_GAP		1

const UINT CTMenuControlBar::m_nMsgTrackButtonMenu = ::RegisterWindowMessage(_T("CTMenuControlBar::m_nMsgTrackButtonMenu"));
const CSize CTMenuControlBar::m_szDefTBB( 23, 20);

BOOL CTMenuControlBar::m_bHideMenuBarExpandContentButton = FALSE;


/////////////////////////////////////////////////////////////////////////////
// CTMenuControlBar

BOOL CTMenuControlBar::OnHookWndMsg( LRESULT &lResult,
									 HWND hWndHooked,
									 UINT nMessage,
									 WPARAM &wParam,
									 LPARAM &lParam)
{
	if( !GetSafeHwnd() ||
		!::IsWindow(GetSafeHwnd()) ||
		_DraggingGetBar())
	{
		return CTHookSink::OnHookWndMsg(
			lResult,
			hWndHooked,
			nMessage,
			wParam,
			lParam);
	}

	if( (!m_bPresubclassDialogMode && hWndHooked == _GetHwndMainFrame()) ||
		(m_bPresubclassDialogMode && hWndHooked == _GetHwndPlacement()))
	{
		if( nMessage == WM_PARENTNOTIFY && CTPopupMenuWnd::IsMenuTracking() )
		{
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_SIZE ||
			nMessage == WM_ACTIVATEAPP ||
			nMessage == WM_COMMAND ||
			nMessage == WM_SYSCOMMAND)
		{
			HWND hWndCapture = ::GetCapture();

			if( hWndCapture == GetSafeHwnd() )
				ReleaseCapture();
			else if(hWndCapture)
			{
				CWnd *pWnd = FromHandlePermanent(hWndCapture);

				if( pWnd && pWnd->GetSafeHwnd() &&
					::IsWindow(pWnd->GetSafeHwnd()) &&
					pWnd->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					ReleaseCapture();
			}

			if(m_bFlatTracking)
			{
				m_bFlatTracking = FALSE;
				_UpdateFlatTracking();
			}

			if(CTMenuControlBar::m_bMenuTracking)
				CTMenuControlBar::_CloseTrackingMenus();

			return CTHookSink::OnHookWndMsg(
				lResult,
				hWndHooked,
				nMessage,
				wParam,
				lParam);
		}

		if(!m_bPresubclassDialogMode)
		{
			if( (nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU) ||
				(nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU)))
			{
				CTToolControlBar::m_bMenuTrackingExpanded = FALSE;

				TrackMainFrameSystemMenu(
					NULL,
					FALSE);

				lResult = 0;
				return TRUE;
			}

			if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN) && wParam == HTSYSMENU )
			{
				lResult = 0;
				return TRUE;
			}

			if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
			{
				CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
				CPoint point;

				GetCursorPos(&point);
				TrackMainFrameSystemMenu(
					&point,
					FALSE);

				lResult = 0;
				return TRUE;
			}

			if( nMessage == CTPopupMenuWnd::m_nMsgNotifyMenuClosed )
			{
				m_bSysMenuTracking = FALSE;
				return TRUE;
			}
		}

		if( nMessage == CTPopupMenuWnd::m_nMsgPopupNext || nMessage == CTPopupMenuWnd::m_nMsgPopupPrev )
		{
			BOOL bDummyMax = FALSE;
			HWND hWndMdiChild = _GetActiveMdiChildWnd(bDummyMax);
			int nTrackingIndex = GetMenuTrackingButton();

			if( nTrackingIndex < 0 )
			{
				if(m_bSysMenuTracking)
				{
					if( nMessage == CTPopupMenuWnd::m_nMsgPopupNext &&
						m_bMdiApp && hWndMdiChild &&
						IsDisplayMdiDocumentButtons())
						TrackChildFrameSystemMenu( NULL, TRUE);
					else
					{
						int nNewTrackIndex = GetVisibleButton(
							nTrackingIndex,
							nMessage == CTPopupMenuWnd::m_nMsgPopupNext);

						if( nNewTrackIndex == nTrackingIndex )
						{
							lResult = 0;
							return TRUE;
						}

						TrackButtonMenu(nNewTrackIndex);
					}
				}

				lResult = 0;
				return TRUE;
			}

			if( nMessage == CTPopupMenuWnd::m_nMsgPopupNext &&
				!m_bPresubclassDialogMode &&
				nTrackingIndex == GetVisibleButton( -1, FALSE))
			{
				TrackMainFrameSystemMenu( NULL, TRUE);

				lResult = 0;
				return TRUE;
			}

			if( nMessage == CTPopupMenuWnd::m_nMsgPopupPrev &&
				!m_bPresubclassDialogMode &&
				!nTrackingIndex &&
				IsDisplayMdiDocumentButtons())
			{
				if( !m_bMdiApp || !hWndMdiChild || !TrackChildFrameSystemMenu( NULL, TRUE))
					TrackMainFrameSystemMenu( NULL, TRUE);

				lResult = 0;
				return TRUE;
			}

			if(	nMessage == CTPopupMenuWnd::m_nMsgPopupNext &&
				!m_bPresubclassDialogMode &&
				m_bMdiApp && m_bSysMenuTracking &&
				nTrackingIndex < 0 &&
				hWndMdiChild &&
				IsDisplayMdiDocumentButtons() )
			{
				TrackChildFrameSystemMenu( NULL, TRUE);
				lResult = 0;
				return TRUE;
			}

			int nNewTrackIndex = GetVisibleButton(
				nTrackingIndex,
				nMessage == CTPopupMenuWnd::m_nMsgPopupNext);

			if( !IsDisplayMdiDocumentButtons() &&
				nMessage == CTPopupMenuWnd::m_nMsgPopupPrev &&
				!nTrackingIndex &&
				nNewTrackIndex > 0 )
			{
				TrackMainFrameSystemMenu( NULL, TRUE);
				lResult = 0;
				return TRUE;
			}

			if( nNewTrackIndex == nTrackingIndex )
			{
				lResult = 0;
				return TRUE;
			}

			TrackButtonMenu(nNewTrackIndex);
			lResult = 0;
			return TRUE;
		}

		if( nMessage == CTPopupMenuWnd::m_nMsgNotifyMenuExpanded )
		{
			CTToolControlBar::m_bMenuTrackingExpanded = TRUE;
			lResult = 0;
			return TRUE;
		}

		if(!m_bPresubclassDialogMode)
			_KillFrameMenu();
	}
	else if( hWndHooked == _GetHwndChildFrame() )
	{
		if( nMessage == WM_PARENTNOTIFY &&
			CTPopupMenuWnd::IsMenuTracking())
		{
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_SIZE ||
			nMessage == WM_WINDOWPOSCHANGED ||
			nMessage == WM_ACTIVATEAPP ||
			nMessage == WM_COMMAND ||
			nMessage == WM_SYSCOMMAND )
		{
			HWND hWndCapture = ::GetCapture();

			if( hWndCapture == GetSafeHwnd() )
				ReleaseCapture();
			else if(hWndCapture)
			{
				CWnd *pWnd = FromHandlePermanent(hWndCapture);

				if( pWnd && pWnd->GetSafeHwnd() &&
					::IsWindow(pWnd->GetSafeHwnd()) &&
					pWnd->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					ReleaseCapture();
			}

			if(m_bFlatTracking)
			{
				m_bFlatTracking = FALSE;
				_UpdateFlatTracking();
			}

			if(CTMenuControlBar::m_bMenuTracking)
				CTMenuControlBar::_CloseTrackingMenus();

			if(CTPopupMenuWnd::IsMenuTracking())
				CTPopupMenuWnd::CancelMenuTracking();

			_KillFrameMenu();
			if( nMessage == WM_SIZE || nMessage == WM_WINDOWPOSCHANGED )
				PostMessage( WM_TIMER, 14, 0);

			return CTHookSink::OnHookWndMsg(
				lResult,
				hWndHooked,
				nMessage,
				wParam,
				lParam);
		}

		if( (nMessage == WM_NCLBUTTONUP && wParam == HTSYSMENU) ||
			(nMessage == WM_NCRBUTTONUP && (wParam == HTCAPTION || wParam == HTSYSMENU)))
		{
			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;

			TrackChildFrameSystemMenu(
				NULL,
				FALSE);

			lResult = 0;
			return TRUE;
		}

		if( (nMessage == WM_NCLBUTTONDOWN || nMessage == WM_NCRBUTTONDOWN) && wParam == HTSYSMENU )
		{
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_NCRBUTTONDOWN && wParam == HTCAPTION )
		{
			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
			CPoint point;

			GetCursorPos(&point);
			TrackChildFrameSystemMenu(
				&point,
				FALSE);

			lResult = 0;
			return TRUE;
		}

		if( nMessage == CTPopupMenuWnd::m_nMsgNotifyMenuClosed )
		{
			m_bSysMenuTracking = FALSE;
			return TRUE;
		}

		if( nMessage == CTPopupMenuWnd::m_nMsgPopupNext ||
			nMessage == CTPopupMenuWnd::m_nMsgPopupPrev )
		{
			if( nMessage == CTPopupMenuWnd::m_nMsgPopupPrev )
			{
				TrackMainFrameSystemMenu( NULL, TRUE);
				lResult = 0;

				return TRUE;
			}

			int nTrackingIndex = GetMenuTrackingButton();
			if( nTrackingIndex < 0 )
			{
				if(m_bSysMenuTracking)
				{
					int nNewTrackIndex = GetVisibleButton(
						nTrackingIndex,
						nMessage == CTPopupMenuWnd::m_nMsgPopupNext);

					if( nNewTrackIndex == nTrackingIndex )
					{
						lResult = 0;
						return TRUE;
					}

					TrackButtonMenu(nNewTrackIndex);
				}

				lResult = 0;
				return TRUE;
			}

			int nNewTrackIndex = GetVisibleButton(
				nTrackingIndex,
				nMessage == CTPopupMenuWnd::m_nMsgPopupNext);

			if( nNewTrackIndex == nTrackingIndex )
			{
				lResult = 0;
				return TRUE;
			}

			TrackButtonMenu(nNewTrackIndex);
			lResult = 0;

			return TRUE;
		}
	}
	else if( hWndHooked == _GetHwndMdiArea() )
	{
		if( nMessage == WM_PARENTNOTIFY &&
			CTPopupMenuWnd::IsMenuTracking() )
		{
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_MDISETMENU )
		{
			CFrameWnd *pFrame = _GetDockingFrameImpl();
			HWND hWndFrame = pFrame->GetSafeHwnd();
			BOOL bOle = IsOleIpObjActive();

			HMENU hOldMdiMenu = m_menuDoc.Detach();
			HMENU hNewMdiMenu = (HMENU) wParam;
			m_menuDoc.Attach(hNewMdiMenu);

			if(bOle)
				::SetMenu( hWndFrame, hNewMdiMenu);
			else
			{
				HMENU hFrameRealMenu = ::GetMenu(hWndFrame);

				if(hFrameRealMenu)
					::SetMenu( hWndFrame, NULL);

				_UpdateMenuBar(TRUE);
			}

			lResult = (LRESULT) hOldMdiMenu;
			PostMessage( WM_TIMER, 13, 0);

			return TRUE;
		}

		if( nMessage == WM_MDIREFRESHMENU )
		{
			PostMessage( WM_TIMER, 13, 0);
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_CONTEXTMENU )
		{
			_ContextMenuTrack();
			lResult = 0;
			return TRUE;
		}

		if( nMessage == WM_MDIACTIVATE || nMessage == WM_MDIDESTROY )
			PostMessage( WM_TIMER, 13, 0);
	}

	MSG msg;
	::memset( &msg, 0, sizeof(MSG));
	msg.hwnd = hWndHooked;
	msg.message = nMessage;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if(TranslateMainFrameMessage(&msg))
		return TRUE;

	return CTHookSink::OnHookWndMsg(
		lResult,
		hWndHooked,
		nMessage,
		wParam,
		lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CTMenuControlBar

IMPLEMENT_DYNAMIC(CTMenuControlBar, CTToolControlBar)


CTMenuControlBar::CTMenuControlBar()
{
	m_bFlatTracking = FALSE;
	m_bSysMenuTracking = FALSE;
	m_nFlatTrackingIndex = -1;
	m_nOldTrackingIndex = -1;
	m_bMdiApp = FALSE;

	m_strMdiWindowPopupName = _T("");
	m_nMdiDocButtonCmdID = 0;
	m_hWndHelper = NULL;
}

CTMenuControlBar::~CTMenuControlBar()
{
	_RemoveAllButtonsImpl();

	int nCountOfBars = m_AllBars.GetSize();
	for( int i=0; i<nCountOfBars; i++)
	{
		CTControlBar *pBar = m_AllBars[i];

		if( pBar == this )
		{
			m_AllBars.RemoveAt(i);
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CTMenuControlBar, CTToolControlBar)
    //{{AFX_MSG_MAP(CTMenuControlBar)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CTMenuControlBar::m_nMsgTrackButtonMenu, OnTrackButtonMenu)
END_MESSAGE_MAP()


BOOL CTMenuControlBar::LoadMenuBar( UINT nResourceID)
{
	if(m_menuDoc.GetSafeHmenu())
		m_menuDoc.DestroyMenu();

	if(m_menuFrame.GetSafeHmenu())
		m_menuFrame.DestroyMenu();

	if(m_pDockSite)
	{
		if( !GetSafeHwnd() ||
			!::IsWindow(GetSafeHwnd()) ||
			m_menuFrame.GetSafeHmenu() ||
			!IsHookedWindow(_GetHwndMainFrame()))
			return FALSE;
	}

	if(!m_menuFrame.LoadMenu(nResourceID))
		return FALSE;

	if(!g_CmdManager->UpdateFromMenu(
		g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
		m_menuFrame.GetSafeHmenu()))
		return FALSE;

	return _UpdateMenuBar();
}

CMenu *CTMenuControlBar::GetMenu()
{
	if(m_menuDoc.GetSafeHmenu())
		return &m_menuDoc;

	if(m_menuFrame.GetSafeHmenu())
		return &m_menuFrame;

	return NULL;
}

BOOL CTMenuControlBar::UpdateMenuBar( BOOL bDoRecalcLayout)
{
	return _UpdateMenuBar(bDoRecalcLayout);
}

BOOL CTMenuControlBar::_UpdateMenuBar( BOOL bDoRecalcLayout)
{
	SetButtons();

	CMenu *pMenu = GetMenu();
	if( pMenu && pMenu->GetSafeHmenu() )
	{
		UINT nCount = pMenu->GetMenuItemCount();

		for( UINT i=0; i<nCount; i++)
		{
			MENUITEMINFO mii;

			::memset(&mii,0,sizeof(MENUITEMINFO));
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_CHECKMARKS|
				MIIM_DATA|
				MIIM_ID|
				MIIM_STATE|
				MIIM_SUBMENU|
				MIIM_TYPE;

			mii.cch = __MAX_UI_ITEM_TEXT;
			CString strText;
			mii.dwTypeData = strText.GetBuffer(__MAX_UI_ITEM_TEXT);

			if(!mii.dwTypeData)
				return FALSE;

			if(!pMenu->GetMenuItemInfo( i, &mii, TRUE))
			{
				strText.ReleaseBuffer();
				return FALSE;
			}
			strText.ReleaseBuffer();

			BOOL bAppendMdiWindowsMenu = FALSE;
			UINT nCmdID = 0;
			CTCmdManager::cmd* p_cmd = NULL;

			if(!mii.hSubMenu)
			{
				nCmdID = mii.wID;

				if( nCmdID == ID_SEPARATOR )
				{
					if(!InsertButton( i, nCmdID, FALSE))
						return FALSE;

					continue;
				}

				p_cmd = g_CmdManager->CmdGetPtr(
					g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
					nCmdID);
			}
			else
			{
				p_cmd = g_CmdManager->CmdAllocPtr(g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()));
				if(!p_cmd)
					return FALSE;

				nCmdID = p_cmd->m_nCmdID;
				if( m_bMdiApp && !m_strMdiWindowPopupName.IsEmpty() )
				{
					CString _strText(strText);

					_strText.TrimLeft();
					_strText.TrimRight();

					while( _strText.Replace( _T("&"), _T("")) > 0 )
					{
						_strText.TrimLeft();
						_strText.TrimRight();
					}

					if( _strText == m_strMdiWindowPopupName )
						bAppendMdiWindowsMenu = TRUE;
				}
			}

			if(p_cmd->m_strToolbarText.IsEmpty())
				p_cmd->m_strToolbarText = strText;

			if(p_cmd->m_strMenuText.IsEmpty())
				p_cmd->m_strMenuText = strText;

			if(!InsertButton( i, nCmdID, FALSE))
				return FALSE;

			if(mii.hSubMenu)
			{
				SetButtonMenu(
					i, mii.hSubMenu,
					FALSE,
					FALSE,
					FALSE);
			}

			if(bAppendMdiWindowsMenu)
				MarkButtonAsMdiWindowsMenu( i, TRUE);
		}

		m_pRightBtn = OnCreateBarRightBtn();
		if(m_pRightBtn)
			m_buttons.Add(m_pRightBtn);
	}

	if(m_bMdiApp)
	{
		if( !IsOleIpObjActive() && _InstallMdiDocButtons(FALSE) )
			bDoRecalcLayout = TRUE;

		_SyncActiveMdiChild();
	}

	if(bDoRecalcLayout)
		_RecalcLayoutImpl();

	return TRUE;
}

HWND CTMenuControlBar::_GetActiveMdiChildWnd( BOOL &bMaximized)
{
	bMaximized = FALSE;
	if(!m_bMdiApp)
		return NULL;

	CMDIFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CMDIFrameWnd,
		_GetDockingFrameImpl());

	HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	BOOL bMax = FALSE;
	HWND hWnd = (HWND) ::SendMessage(
		hWndMdiArea,
		WM_MDIGETACTIVE,
		0, (LPARAM) &bMax);

	bMaximized = bMax;
	return hWnd;
}

void CTMenuControlBar::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
	CTToolControlBar::OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHndler);
}

BOOL CTMenuControlBar::_SyncActiveMdiChild()
{
	if(m_bPresubclassDialogMode)
		return TRUE;

	if(_DraggingGetBar())
		return TRUE;

	if(!(GetStyle()&WS_VISIBLE))
		return TRUE;

	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);

	if( m_hWndHelper == hWndActiveChild )
		return TRUE;

	if(m_hWndHelper)
		SetupHookWndSink( m_hWndHelper, TRUE);

	m_hWndHelper = hWndActiveChild;
	if(m_hWndHelper)
		SetupHookWndSink(m_hWndHelper);

	UpdateWindow();
	Invalidate();
	GetDockingFrame()->DelayRecalcLayout();

	return TRUE;
}

BOOL CTMenuControlBar::IsDisplayMdiDocumentButtons() const
{
	return TRUE;
}

BOOL CTMenuControlBar::_InstallMdiDocButtons( BOOL bDoRecalcLayout)
{
	if(!IsDisplayMdiDocumentButtons())
		return TRUE;

	if(_DraggingGetBar())
		return FALSE;

	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);
	int nCountOfButtons = GetButtonsCount();

	if( !nCountOfButtons && !hWndActiveChild )
		return FALSE;

	CTBarMdiDocButton *pDocTBB = DYNAMIC_DOWNCAST(
		CTBarMdiDocButton,
		_GetButtonPtr(0));

	int nRightIndex = nCountOfButtons - 1;
	CTBarMdiRightButton *pRightTBB = DYNAMIC_DOWNCAST(
		CTBarMdiRightButton,
		_GetButtonPtr(nRightIndex));

	if( !pRightTBB && nCountOfButtons > 1 )
	{
		nRightIndex--;
		pRightTBB = DYNAMIC_DOWNCAST(
			CTBarMdiRightButton,
			_GetButtonPtr(nRightIndex));
	}

	BOOL bRetVal = FALSE;
	if( !hWndActiveChild || !bMax )
	{
		if(pRightTBB)
		{
			RemoveButton( nRightIndex, FALSE);
			bRetVal = TRUE;
		}

		if(pDocTBB)
		{
			RemoveButton( 0, FALSE);
			bRetVal = TRUE;
		}
	}
	else
	{
		if(!m_nMdiDocButtonCmdID)
		{
			CTCmdManager::cmd *p_cmd = g_CmdManager->CmdAllocPtr(g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()));

			if(!p_cmd)
				return FALSE;

			m_nMdiDocButtonCmdID = p_cmd->m_nCmdID;
		}

		if(!pRightTBB)
		{
			pRightTBB = new CTBarMdiRightButton(
				this,
				m_nMdiDocButtonCmdID);

			m_buttons.InsertAt(
				nRightIndex + 1,
				pRightTBB);

			bRetVal = TRUE;
		}

		if(!pDocTBB)
		{
			pDocTBB = new CTBarMdiDocButton(
				this,
				m_nMdiDocButtonCmdID);

			pDocTBB->SetMenu(
				(HMENU) ::GetSystemMenu( hWndActiveChild, FALSE),
				FALSE,
				FALSE);

			m_buttons.InsertAt( 0, pDocTBB);
			bRetVal = TRUE;
		}
	}

	if(bDoRecalcLayout)
	{
		_RecalcLayoutImpl();
		bRetVal = FALSE;
	}

	return bRetVal;
}

void CTMenuControlBar::OnDestroy()
{
	RemoveAllWndHooks();
	CTToolControlBar::OnDestroy();
}

BOOL CTMenuControlBar::TranslateMainFrameMessage( MSG* pMsg)
{
	if( !GetSafeHwnd() || !::IsWindow(GetSafeHwnd()) )
		return  FALSE;

	UINT nMsg = pMsg->message;
	if( nMsg == WM_SYSKEYDOWN ||
		nMsg == WM_SYSKEYUP ||
		nMsg == WM_KEYDOWN ||
		nMsg == WM_KEYUP )
	{
		BOOL bKeyUp = nMsg == WM_KEYUP || nMsg == WM_SYSKEYUP;
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x80000000;
		BOOL bShift = GetKeyState(VK_SHIFT) & 0x80000000;
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;
		TCHAR vkTCHAR = (TCHAR) pMsg->wParam;

		if( vkTCHAR == VK_MENU && !bCtrl && !bShift )
		{
			if(!bKeyUp)
				return TRUE;

			if(m_bMenuTracking)
				return TRUE;

			if( GetButtonsCount() == 0 )
				return TRUE;

			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
			TrackButtonMenu(0);

			return TRUE;
		}

		if( !m_bPresubclassDialogMode && bAlt && vkTCHAR == VK_SPACE )
		{
			if(bKeyUp)
				return TRUE;

			CWnd *pWnd = CWnd::GetActiveWindow();
			if( !pWnd || !::IsWindow(pWnd->GetSafeHwnd()) )
				return TRUE;

			if( pWnd->GetSafeHwnd() != _GetDockingFrameImpl()->GetSafeHwnd() && (pWnd->GetStyle()&WS_POPUP) )
				return TRUE;

			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
			TrackMainFrameSystemMenu( NULL, TRUE);

			return TRUE;
		}

		if(bAlt)
		{
			if(bKeyUp)
				return TRUE;

			if(m_bFlatTracking)
			{
				m_bFlatTracking = FALSE;
				_UpdateFlatTracking();
			}

			BYTE lpKeyState[256];
			::GetKeyboardState(lpKeyState);
			UINT wScanCode = ::MapVirtualKey( vkTCHAR, 0);

			HKL hKeyboardLayout = ::GetKeyboardLayout(::AfxGetThread()->m_nThreadID);
#if (defined _UNICODE)
			TCHAR szChar[2] = { _T('\0'), _T('\0')};

			::ToUnicodeEx(
				vkTCHAR,
				wScanCode,
				lpKeyState,
				szChar,
				1, 1,
				hKeyboardLayout);
#else
			WORD nMapped = 0;

			::ToAsciiEx(
				vkTCHAR,
				wScanCode,
				lpKeyState,
				&nMapped, 1,
				hKeyboardLayout);

			TCHAR szChar[2] = { (TCHAR)nMapped, _T('\0')};
#endif
			::CharUpper(szChar);
			int nBtnIdx = -1;

			if(szChar[0])
				nBtnIdx = GetButtonByAccessKey(szChar[0]);

			if( nBtnIdx < 0 )
				return FALSE;

			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
			CTToolControlBar::TrackButtonMenu(nBtnIdx);

			return TRUE;
		}

		if(m_bFlatTracking)
		{
			if(bKeyUp)
				return TRUE;

			BOOL bUpdateIndex = FALSE;
			switch(vkTCHAR)
			{
			case VK_LEFT		:
				{
					bUpdateIndex = TRUE;

					m_nFlatTrackingIndex = GetVisibleButton(
						m_nFlatTrackingIndex,
						FALSE);
				}

				break;

			case VK_RIGHT		:
				{
					bUpdateIndex = TRUE;

					m_nFlatTrackingIndex = GetVisibleButton(
						m_nFlatTrackingIndex,
						TRUE);
				}

				break;

			case VK_RETURN		:
			case VK_UP			:
			case VK_DOWN		:
				{
					if( CTPopupMenuWnd::IsMenuTracking() || m_nFlatTrackingIndex < 0 )
						return TRUE;

					CTBarButton *pTBB = _GetButtonPtr(m_nFlatTrackingIndex);
					if(pTBB->IsAbleToTrackMenu())
					{
						int nIndex = m_nFlatTrackingIndex;

						m_bFlatTracking = FALSE;
						_UpdateFlatTracking(FALSE);

						CTToolControlBar::TrackButtonMenu(nIndex);
					}
					else if( vkTCHAR == VK_RETURN && CTCmdManager::IsCommand(pTBB->GetCmdID()))
					{
						pTBB->OnClick(
							CPoint(0,0),
							FALSE);
					}

					return TRUE;
				}

				break;

			case VK_ESCAPE		:
				{
					m_bFlatTracking = FALSE;
					_UpdateFlatTracking();
				}

				return TRUE;
			}

			if(bUpdateIndex)
			{
				int nCountOfButtons = GetButtonsCount();

				if( m_nFlatTrackingIndex < 0 )
					m_nFlatTrackingIndex = nCountOfButtons - 1;
				else if( m_nFlatTrackingIndex >= nCountOfButtons )
					m_nFlatTrackingIndex = 0;
			}

			_UpdateFlatTracking();
			return TRUE;
		}
	}

	if( nMsg == WM_CONTEXTMENU )
	{
		if( m_bMenuTracking || m_bFlatTracking )
			return TRUE;
	}

	if( m_bFlatTracking &&
		nMsg >= WM_MOUSEFIRST &&
		nMsg <= WM_MOUSELAST &&
		nMsg != WM_MOUSEMOVE )
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking();
	}

	return FALSE;
}

CRect CTMenuControlBar::_GetChildFrameSysIconRect()
{
	CRect rc;
	rc.SetRectEmpty();

	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);

	if(!hWndActiveChild)
		return rc;

	if(bMax)
		return rc;

	CWnd *pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if(!pWnd)
		return rc;

	CFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CFrameWnd,
		pWnd);

	if(!pFrame)
		return rc;

	CRect rcHelper;
	CRect rcClient;
	CRect rcWnd;

	pFrame->GetWindowRect(&rcWnd);
	pFrame->GetClientRect(&rcClient);
	rcHelper = rcWnd;
	pFrame->ScreenToClient(&rcHelper);

	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff);

	return rc;
}

CRect CTMenuControlBar::_GetMainFrameSysIconRect()
{
	CRect rcHelper;
	CRect rcClient;
	CRect rcWnd;
	CRect rc;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	pFrame->GetWindowRect(&rcWnd);
	pFrame->GetClientRect(&rcClient);
	rcHelper = rcWnd;
	pFrame->ScreenToClient(&rcHelper);

	int yDiff = rcClient.top - rcHelper.top;
	rc.SetRect(
		rcWnd.left + GetSystemMetrics(SM_CYFRAME),
		rcWnd.top + GetSystemMetrics(SM_CXFRAME),
		rcWnd.left + yDiff,
		rcWnd.top + yDiff);

	return rc;
}

BOOL CTMenuControlBar::_TrackFrameSystemMenu( CFrameWnd *pFrame,
											  CPoint *pPoint,
											  BOOL bSelectAny,
											  LPCRECT rcExcludeArea,
											  UINT nTrackFlags,
											  BOOL bCombinedMode)
{
	if(m_bFlatTracking)
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking();
	}

	if( nTrackFlags == (UINT) (-1) )
		nTrackFlags = TPMX_BOTTOMALIGN;

	BOOL bPrevTBMT = CTToolControlBar::m_bMenuTracking;
	int nTrackingIdxOld = m_nBtnIdxMenuTracking;

	if(m_bMenuTracking)
		_CloseTrackingMenus();

	if(CTPopupMenuWnd::IsMenuTracking())
		CTPopupMenuWnd::CancelMenuTracking();

	CMenu *pSysMenu = pFrame->GetSystemMenu(FALSE);
	if(!pSysMenu)
		return FALSE;

	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();
	if(!pPopup->CreatePopupMenu(pFrame->GetSafeHwnd()))
		return FALSE;

	if(!pPopup->UpdateFromMenu(
		pFrame->GetSafeHwnd(),
		pSysMenu,
		FALSE))
		return FALSE;

	CPoint point;
	if(pPoint)
		point = *pPoint;
	else
	{
		CRect rcHelper;
		CRect rcClient;
		CRect rcWnd;

		pFrame->GetWindowRect(&rcWnd);
		pFrame->GetClientRect(&rcClient);
		rcHelper = rcWnd;
		pFrame->ScreenToClient(&rcHelper);

		int yDiff = rcClient.top - rcHelper.top;
		point = rcWnd.TopLeft();
		point.x += GetSystemMetrics(SM_CXFRAME);
		point.y += yDiff;
	}

	if(bSelectAny)
		nTrackFlags |= TPMX_SELECT_ANY;

	if(bPrevTBMT)
		nTrackFlags |= TPMX_NO_HIDE_RARELY;

	pFrame->SetFocus();
	CTToolControlBar::m_bMenuTracking = bPrevTBMT;

	if( nTrackingIdxOld >= 0 )
		_SwitchMenuTrackingIndex(nTrackingIdxOld);

	if(!pPopup->TrackPopupMenu(
		nTrackFlags|
		TPMX_COMBINE_DEFAULT|
		TPMX_OWNERDRAW_FIXED,
		point.x,
		point.y,
		rcExcludeArea,
		bCombinedMode ? this : NULL,
		bCombinedMode ? CTToolControlBar::_CbPaintCombinedContent : NULL))
	{
		delete pPopup;

		CTToolControlBar::_CloseTrackingMenus();
		return FALSE;
	}

	m_bMenuTracking = TRUE;
	return TRUE;
}

BOOL CTMenuControlBar::TrackChildFrameSystemMenu( CPoint *pPoint, BOOL bSelectAny)
{
	if(!m_bMdiApp)
		return FALSE;

	if( GetButtonsCount() > 0 )
	{
		CTBarButton *pTBB = _GetButtonPtr(0);

		if(pTBB->IsKindOf(RUNTIME_CLASS(CTBarMdiDocButton)))
		{
			if(!TrackButtonMenu(0))
				return FALSE;

			return TRUE;
		}
	}

	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);

	if(!hWndActiveChild)
		return FALSE;

	CWnd *pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if(!pWnd)
		return FALSE;

	CFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CFrameWnd,
		pWnd);

	if(!pFrame)
		return FALSE;

	m_bMenuTracking = TRUE;
	_SwitchMenuTrackingIndex();

	m_bSysMenuTracking = _TrackFrameSystemMenu(
		pFrame,
		pPoint,
		bSelectAny);

	return m_bSysMenuTracking;
}

BOOL CTMenuControlBar::TrackMainFrameSystemMenu( CPoint *pPoint, BOOL bSelectAny)
{
	CFrameWnd *pFrame = _GetDockingFrameImpl();

	m_bMenuTracking = TRUE;
	_SwitchMenuTrackingIndex();

	m_bSysMenuTracking = _TrackFrameSystemMenu(
		pFrame,
		pPoint,
		bSelectAny);

	return m_bSysMenuTracking;
}

BOOL CTMenuControlBar::TrackButtonMenu( int nIndex)
{
	if( m_bMenuTracking && m_nBtnIdxMenuTracking == nIndex )
		return TRUE;

	if(!SafeDisplayBar())
		return FALSE;

	if(m_bFlatTracking)
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking();
	}

	PostMessage(
		m_nMsgTrackButtonMenu,
		(WPARAM) nIndex);

	return TRUE;
}

BOOL CTMenuControlBar::_InvokeParentTrackButtonMenu( int nIndex)
{
	return CTToolControlBar::TrackButtonMenu(nIndex);
}

LRESULT CTMenuControlBar::OnTrackButtonMenu( WPARAM wParam, LPARAM lParam)
{
	if(!::IsWindowEnabled(m_bPresubclassDialogMode ? GetOwner()->GetSafeHwnd() : _GetDockingFrameImpl()->GetSafeHwnd()))
		return 0;

	int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons == 0 )
		return 0;

	int nIndex = int(wParam);
	if( nIndex < 0 || nIndex >= nCountOfButtons )
	{
		if(m_bFlatTracking)
		{
			m_bFlatTracking = FALSE;
			_UpdateFlatTracking();
		}

		_CloseTrackingMenus();
		return 0;
	}

	CTBarButton *pTBB = _GetButtonPtr(nIndex);
	BOOL bDroppedMode = CTPopupMenuWnd::IsMenuTracking();

	if(!pTBB->IsAbleToTrackMenu())
		bDroppedMode = FALSE;

	if(bDroppedMode)
	{
		CTToolControlBar::TrackButtonMenu(nIndex);
		return 0;
	}

	_CloseTrackingMenus();
	if( GetCapture() == this )
		ReleaseCapture();

	m_nFlatTrackingIndex = nIndex;
	m_bFlatTracking = TRUE;
	_UpdateFlatTracking();

	return 0;
}

void CTMenuControlBar::_UpdateFlatTracking( BOOL bRepaint)
{
	if( m_nFlatTrackingIndex < 0 && m_nOldTrackingIndex < 0 )
	{
		m_bFlatTracking = FALSE;
		return;
	}

	int nCountOfButtons = GetButtonsCount();
	if(!m_bFlatTracking)
	{
		if( m_nFlatTrackingIndex >= 0 )
		{
			CTBarButton *pTBB = _GetButtonPtr(m_nFlatTrackingIndex);
			pTBB->ModifyStyle( 0, TBBS_PRESSED);

			if(bRepaint)
			{
				_InvalidateButton(m_nFlatTrackingIndex);
				UpdateWindow();
			}

			m_nFlatTrackingIndex = -1;
			m_nOldTrackingIndex = -1;
		}

		return;
	}

	if( m_nFlatTrackingIndex != m_nOldTrackingIndex )
	{
		if( m_nOldTrackingIndex >= 0 )
		{
			CTBarButton *pTBB = _GetButtonPtr(m_nOldTrackingIndex);
			pTBB->ModifyStyle(0,TBBS_PRESSED|TBBS_CHECKED);

			if(bRepaint)
				_InvalidateButton(m_nOldTrackingIndex);
		}

		if( m_nFlatTrackingIndex >= 0 )
		{
			CTBarButton *pTBB = _GetButtonPtr(m_nFlatTrackingIndex);
			pTBB->ModifyStyle(TBBS_PRESSED);

			if(bRepaint)
				_InvalidateButton(m_nFlatTrackingIndex);
		}

		m_nOldTrackingIndex = m_nFlatTrackingIndex;
		UpdateWindow();
	}
}

void CTMenuControlBar::OnTimer( UINT nIDEvent)
{
	if( nIDEvent == 13 )
	{
		_SyncActiveMdiChild();
		return;
	}

	if( nIDEvent == 14 )
	{
		UpdateMenuBar(TRUE);
		return;
	}

	CTToolControlBar::OnTimer(nIDEvent);
}

void CTMenuControlBar::OnMouseMove( UINT nFlags, CPoint point)
{
	if(!m_bFlatTracking)
	{
		CTToolControlBar::OnMouseMove( nFlags, point);
		return;
	}

	int nIndex = HitTest(point);
	if( nIndex == m_nFlatTrackingIndex || nIndex < 0 )
		return;

	TrackButtonMenu(nIndex);
}

void CTMenuControlBar::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(!m_bFlatTracking)
	{
		int nIndex = HitTest(point);
		if( nIndex >= 0 )
		{
			int nMenuIndex = GetMenuTrackingButton();
			if( nIndex == nMenuIndex )
				return;
		}
	}

	CTToolControlBar::OnLButtonDown( nFlags, point);
}

void CTMenuControlBar::OnLButtonUp( UINT nFlags, CPoint point)
{
	BOOL bRepaint = FALSE;

	if(m_bFlatTracking)
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking(FALSE);

		bRepaint = TRUE;
	}

	CTToolControlBar::OnLButtonUp( nFlags, point);
	if(bRepaint)
		Invalidate();
}

void CTMenuControlBar::_RecalcLayoutImpl()
{
	CTToolControlBar::_RecalcLayoutImpl();
}

void CTMenuControlBar::_RecalcPositionsImpl()
{
	CTToolControlBar::_RecalcPositionsImpl();

	if( m_bHideMenuBarExpandContentButton && m_pRightBtn &&
		!m_pRightBtn->GetButtons().GetSize())
		m_pRightBtn->Show(FALSE);

	if(IsFloating())
		return;

	int nCountOfButtons = GetButtonsCount();
	if( nCountOfButtons <= 1 )
		return;
	int nRightIndex = nCountOfButtons - 1;

	CTBarMdiRightButton *pRightTBB = DYNAMIC_DOWNCAST(
		CTBarMdiRightButton,
		_GetButtonPtr(nRightIndex));

	if( !pRightTBB && nCountOfButtons > 1 )
	{
		nRightIndex--;

		pRightTBB = DYNAMIC_DOWNCAST(
			CTBarMdiRightButton,
			_GetButtonPtr(nRightIndex));
	}

	if(!pRightTBB)
		return;

	if(!pRightTBB->IsVisible())
		return;

	CTBarButton *pExpandTBB = NULL;
	CRect rcBtnExpand( 0, 0, 0, 0);
	int nAlignMargin = 0;

	BOOL bHorz = IsDockedHorizontally();
	CRect rcClient;
	GetClientRect(&rcClient);

	if( nRightIndex < nCountOfButtons - 1 )
	{
		pExpandTBB = _GetButtonPtr(nRightIndex + 1);
		rcBtnExpand = pExpandTBB->Rect();

		nAlignMargin = bHorz ? rcBtnExpand.left : rcBtnExpand.top;
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	}
	else
	{
		nAlignMargin = bHorz ? rcClient.right : rcClient.bottom;
		nAlignMargin -= __MENUBAR_RIGHT_ADJUST_GAP;
	}

	CRect rcBtnRight = pRightTBB->Rect();
	if( bHorz && rcBtnRight.right >= nAlignMargin )
		return;

	if( !bHorz && rcBtnRight.bottom >= nAlignMargin )
		return;

	if(bHorz)
		rcBtnRight.OffsetRect( nAlignMargin - rcBtnRight.right, 0);
	else
		rcBtnRight.OffsetRect( 0, nAlignMargin - rcBtnRight.bottom);

	if(!pExpandTBB)
	{
		pRightTBB->SetRect(rcBtnRight);
		return;
	}

	CRect rcBtnExpand2(
		bHorz ? rcBtnRight.left : rcBtnExpand.left,
		bHorz ? rcBtnExpand.top : rcBtnRight.top,
		bHorz ? rcBtnRight.left + rcBtnExpand.Width() : rcBtnExpand.left + rcBtnExpand.Width(),
		bHorz ? rcBtnExpand.top + rcBtnExpand.Height() : rcBtnRight.top + rcBtnExpand.Height());

	rcBtnRight.OffsetRect(
		bHorz ? rcBtnExpand.right - rcBtnRight.right : 0,
		bHorz ? 0 : rcBtnExpand.bottom - rcBtnRight.bottom);

	pRightTBB->SetRect(rcBtnRight);
	pExpandTBB->SetRect(rcBtnExpand2);

	if(m_bHideMenuBarExpandContentButton)
		pExpandTBB->Show(FALSE);
}

void CTMenuControlBar::_Dragging_OnStart()
{
	if(m_bFlatTracking)
	{
		m_bFlatTracking = FALSE;
		_UpdateFlatTracking();
	}

	if(m_bMenuTracking)
		_CloseTrackingMenus();

	CTToolControlBar::_Dragging_OnStart();
}

HWND CTMenuControlBar::_GetHwndMainFrame()
{
	if(m_bPresubclassDialogMode)
		return NULL;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	return pFrame->GetSafeHwnd();
}

HWND CTMenuControlBar::_GetHwndPlacement()
{
	if(!m_bPresubclassDialogMode)
		return NULL;

	return m_hWndHelper;
}

HWND CTMenuControlBar::_GetHwndChildFrame()
{
	if(m_bPresubclassDialogMode)
		return NULL;

	return m_hWndHelper;
}

HWND CTMenuControlBar::_GetHwndMdiArea()
{
	if(m_bPresubclassDialogMode)
		return NULL;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	HWND hWndMdiArea = ((CMDIFrameWnd *) pFrame)->m_hWndMDIClient;

	return hWndMdiArea;
}

int CTMenuControlBar::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if( CTToolControlBar::OnCreate(lpCreateStruct) == -1 )
		return -1;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	if(!pFrame)
	{
		m_bPresubclassDialogMode = TRUE;

		if(!m_hWndHelper)
		{
			m_hWndHelper = ::GetParent(m_hWnd);

			if(!IsHookedWindow(m_hWndHelper))
				SetupHookWndSink(m_hWndHelper);
		}

		return 0;
	}

	HMENU hFrameMenu = pFrame->GetMenu()->GetSafeHmenu();
	if(hFrameMenu)
	{
		m_menuFrame.Attach(hFrameMenu);
		_UpdateMenuBar(FALSE);
		pFrame->SetMenu(NULL);
	}

	if(!SetupHookWndSink(_GetHwndMainFrame()))
		return FALSE;

	if(pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		m_bMdiApp = TRUE;

		if(!SetupHookWndSink(_GetHwndMdiArea()))
			return FALSE;

		::SetWindowPos(
			_GetHwndMdiArea(),
			NULL,
			0, 0,
			0, 0,
			SWP_FRAMECHANGED|
			SWP_NOOWNERZORDER|
			SWP_NOCOPYBITS|
			SWP_NOREPOSITION|
			SWP_NOZORDER|
			SWP_NOACTIVATE|
			SWP_NOMOVE|
			SWP_NOSIZE);

		_SyncActiveMdiChild();
	}

	CTPopupMenuWnd::PassMsgLoop(CTControlBar::m_bEnableOnIdleCalls);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CTBarMdiDocButton

IMPLEMENT_DYNAMIC( CTBarMdiDocButton, CTBarButton)

BOOL CTBarMdiDocButton::PutToPopupMenu( CTPopupMenuWnd *pPopup)
{
	BOOL bDummyMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bDummyMax);

	CFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CFrameWnd,
		CWnd::FromHandle(hWndActiveChild));

	CMenu *pSysMenu = pFrame->GetSystemMenu(FALSE);
	if(!pSysMenu)
		return FALSE;

	HICON hIcon = GetHICON();
	if(hIcon)
	{
		CTCmdManager::icon _icon( hIcon, TRUE);
		hIcon = _icon.Detach();
	}

	CString strActiveDocument(_T("Active document"));
	if(!pPopup->ItemInsert(
		(UINT) CTPopupMenuWnd::TYPE_POPUP,
		-1, strActiveDocument, hIcon))
		return FALSE;

	CTPopupMenuWnd *pChildPopup = pPopup->ItemGetPopup(pPopup->ItemGetCount() - 1);
	if(!pChildPopup->UpdateFromMenu(
		m_pBar->GetSafeHwnd(),
		pSysMenu,
		FALSE,
		FALSE))
		return FALSE;

	pChildPopup->SetCmdTargetToAllItems(hWndActiveChild);
	pPopup->ItemInsert( CTPopupMenuWnd::TYPE_SEPARATOR, -1);

	return TRUE;
}

HWND CTBarMdiDocButton::_GetActiveMdiChildWnd( BOOL &bMaximized)
{
	bMaximized = FALSE;

	CMDIFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CMDIFrameWnd,
		m_pBar->_GetDockingFrameImpl());

	if(!pFrame)
		return NULL;

	HWND hWndMdiArea = pFrame->m_hWndMDIClient;
	HWND hWndActiveChild = (HWND) ::SendMessage(
		hWndMdiArea,
		WM_MDIGETACTIVE,
		0, (LPARAM) &bMaximized);

	return hWndActiveChild;
}

HICON CTBarMdiDocButton::GetHICON()
{
	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);

	if(!hWndActiveChild)
		return NULL;

	HICON hIcon = (HICON) ::SendMessage(
		hWndActiveChild,
		CTControlBar::m_nMsgQueryIcon,
		0, 0);

	if(!hIcon)
	{
		hIcon = (HICON) ::SendMessage(
			hWndActiveChild,
			WM_GETICON,
			ICON_SMALL, 0);
	}

	if(!hIcon)
	{
		hIcon = (HICON) ::SendMessage(
			hWndActiveChild,
			WM_GETICON,
			ICON_BIG, 0);
	}

	if(!hIcon)
	{
		hIcon = (HICON) ::GetClassLong(
			hWndActiveChild,
			GCL_HICONSM);
	}

	return hIcon;
}

CWnd *CTBarMdiDocButton::GetCmdTargetWnd()
{
	BOOL bMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bMax);

	if(!hWndActiveChild)
		return NULL;

	CWnd *pWnd = CWnd::FromHandlePermanent(hWndActiveChild);
	if(!pWnd)
		return NULL;

	return pWnd;
}

void CTBarMdiDocButton::OnTrackPopup( CPoint point)
{
	CTMenuControlBar *pBar = DYNAMIC_DOWNCAST(
		CTMenuControlBar,
		m_pBar);

	CWnd *pWnd = CTBarMdiDocButton::GetCmdTargetWnd();
	CFrameWnd *pFrame = DYNAMIC_DOWNCAST(
		CFrameWnd,
		pWnd);

	if( !pBar || !pFrame )
	{
		CTBarButton::OnTrackPopup(point);
		return;
	}

	BOOL bPrevTBMT = CTToolControlBar::m_bMenuTracking;
	if( CTToolControlBar::m_bMenuTracking &&
		pBar->_GetIndexOf(this) == pBar->m_nBtnIdxMenuTracking)
		return;

	CTToolControlBar::_CloseTrackingMenus();
	if(pBar->IsFloating())
	{
		pBar->ActivateTopParent();
		CFrameWnd *pFrame = pBar->GetDockingFrame();
		pFrame->BringWindowToTop();
	}

	UINT nTrackFlags = TPMX_TOPALIGN;
	CRect rcBtn = Rect();

	switch(m_pBar->GetSafeDockBarDlgCtrlID())
	{
	case AFX_IDW_DOCKBAR_BOTTOM		: nTrackFlags = TPMX_BOTTOMALIGN; break;
	case AFX_IDW_DOCKBAR_LEFT		: nTrackFlags = TPMX_LEFTALIGN; break;
	case AFX_IDW_DOCKBAR_RIGHT		: nTrackFlags = TPMX_RIGHTALIGN; break;
	}

	pBar->ClientToScreen(&rcBtn);
	point = rcBtn.CenterPoint();
	CTToolControlBar::m_bMenuTracking = bPrevTBMT;

	m_pBar->_SwitchMenuTrackingIndex(m_pBar->_GetIndexOf(this));
	pBar->m_bSysMenuTracking = pBar->_TrackFrameSystemMenu(
		pFrame,
		&point,
		TRUE,
		&rcBtn,
		nTrackFlags,
		TRUE);

	CTToolControlBar::m_bMenuTracking = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTBarMdiRightButton

IMPLEMENT_DYNAMIC( CTBarMdiRightButton, CTBarMdiDocButton)

BOOL CTBarMdiRightButton::PutToPopupMenu( CTPopupMenuWnd *pPopup)
{
	CTMenuControlBar *pBar = DYNAMIC_DOWNCAST(
		CTMenuControlBar,
		m_pBar);

	BOOL bHorz = !pBar->IsDockedVertically();
	MdiMenuBarRightButtonsInfo _info;

	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz);
	if(!_info.m_bInitialized)
		return FALSE;

	BOOL bDummyMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bDummyMax);

	int nCount = pPopup->ItemGetCount();
	if( nCount > 0 && pPopup->ItemGetCmdID(nCount - 1) != CTPopupMenuWnd::TYPE_SEPARATOR )
		pPopup->ItemInsert( CTPopupMenuWnd::TYPE_SEPARATOR, -1);

	if(_info.m_bBtnHelp)
		pPopup->ItemInsert( SC_CONTEXTHELP, -1, NULL, NULL, hWndActiveChild);

	if(_info.m_bBtnMinimize)
	{
		pPopup->ItemInsert(
			SC_MINIMIZE,
			-1, NULL, NULL,
			hWndActiveChild);

		pPopup->ItemInsert(
			SC_RESTORE,
			-1, NULL, NULL,
			hWndActiveChild);
	}

	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
	{
		pPopup->ItemInsert(
			SC_MAXIMIZE,
			-1, NULL, NULL,
			hWndActiveChild);
	}

	if(_info.m_bBtnClose)
	{
		pPopup->ItemInsert(
			SC_CLOSE,
			-1, NULL, NULL,
			hWndActiveChild);
	}

	return TRUE;
}

HICON CTBarMdiRightButton::GetHICON()
{
	return NULL;
}

void CTMenuControlBar::_GetMdiMenuBarRightButtonsInfo( CTBarMdiRightButton::MdiMenuBarRightButtonsInfo& _info,
													   CTBarMdiRightButton *pTBB,
													   BOOL bHorz)
{
	_info.m_pWndMdiChildFrame = pTBB->GetCmdTargetWnd();
	if(!_info.m_pWndMdiChildFrame)
		return;

	_info.m_pBar = this;
	DWORD dwFrameStyle = _info.m_pWndMdiChildFrame->GetStyle();

	_info.m_bBtnClose = TRUE;
	_info.m_bBtnMaximize = dwFrameStyle&WS_MAXIMIZEBOX;
	_info.m_bBtnMinimize = dwFrameStyle&WS_MINIMIZEBOX;
	_info.m_bBtnHelp = _info.m_bBtnMaximize && (_info.m_pWndMdiChildFrame->GetExStyle()&WS_EX_CONTEXTHELP);

	if(bHorz)
	{
		_info.m_calcSize.cx = __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = _info.m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnMinimize)
			_info.m_calcSize.cx += (_info.m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS) * 2;

		if(_info.m_bBtnMaximize && !_info.m_bBtnMinimize)
			_info.m_calcSize.cx += _info.m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnHelp)
			_info.m_calcSize.cx += _info.m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnClose)
			_info.m_calcSize.cx += _info.m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	}
	else
	{
		_info.m_calcSize.cx = _info.m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;
		_info.m_calcSize.cy = __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnMinimize)
			_info.m_calcSize.cy += (_info.m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS) * 2;

		if(_info.m_bBtnMaximize && !_info.m_bBtnMinimize)
			_info.m_calcSize.cy += _info.m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnHelp)
			_info.m_calcSize.cy += _info.m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;

		if(_info.m_bBtnClose)
			_info.m_calcSize.cy +=_info.m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	}

	_info.m_bInitialized = TRUE;
}

void CTBarMdiRightButton::MdiMenuBarRightButtonsInfo::InitLocations( const CRect& rcBtn, BOOL bHorz)
{
	EmptyLocations();

	if(rcBtn.IsRectEmpty())
		return;

	CRect rcStart( 0, 0, 0, 0);
	CSize szOffset( 0, 0);

	if(bHorz)
	{
		int nOffset = (rcBtn.Height() - m_cyIcon) / 2;

		rcStart.SetRect(
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.top + nOffset,
			rcBtn.left + __GAP_BETWEEN_WND_RIGHT_BUTTONS + m_cxIcon,
			rcBtn.top + nOffset + m_cyIcon);
		szOffset.cx = m_cxIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	}
	else
	{
		int nOffset = (rcBtn.Width() - m_cxIcon) / 2;

		rcStart.SetRect(
			rcBtn.left + nOffset,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS,
			rcBtn.left + nOffset + m_cxIcon,
			rcBtn.top + __GAP_BETWEEN_WND_RIGHT_BUTTONS + m_cxIcon);
		szOffset.cy = m_cyIcon + __GAP_BETWEEN_WND_RIGHT_BUTTONS;
	}

	if(m_bBtnHelp)
	{
		m_rcBtnHelp = rcStart;
		rcStart.OffsetRect(szOffset);
	}

	if(m_bBtnMinimize)
	{
		m_rcBtnMinimize = rcStart;
		rcStart.OffsetRect(szOffset);

		m_rcBtnRestore = rcStart;
		rcStart.OffsetRect(szOffset);
	}

	if( m_bBtnMaximize && !m_bBtnMinimize )
	{
		m_rcBtnMaximize = rcStart;
		rcStart.OffsetRect(szOffset);
	}

	if(m_bBtnClose)
		m_rcBtnClose = rcStart;
}

CSize CTBarMdiRightButton::CalculateLayout( CDC &dc, CSize szPreCalc, BOOL bHorz)
{
	CTMenuControlBar *pBar = DYNAMIC_DOWNCAST(
		CTMenuControlBar,
		m_pBar);

	MdiMenuBarRightButtonsInfo _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz);

	if(!_info.m_bInitialized)
		return CSize( 1, 1);

	CSize _size(_info.m_calcSize);
	if(bHorz)
	{
		if( _size.cy < szPreCalc.cy )
			_size.cy = szPreCalc.cy;
	}
	else
	{
		if( _size.cx < szPreCalc.cx )
			_size.cx = szPreCalc.cx;
	}

	m_ActiveSize = _size;
	return _size;
}

void CTBarMdiRightButton::Paint( CDC &dc, BOOL bHorz)
{
	CTMenuControlBar *pBar = DYNAMIC_DOWNCAST(
		CTMenuControlBar,
		m_pBar);

	MdiMenuBarRightButtonsInfo _info;
	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz);

	if(!_info.m_bInitialized)
		return;

	_info.InitLocations( Rect(), bHorz);
	CPoint ptCursor;

	::GetCursorPos(&ptCursor);
	pBar->ScreenToClient(&ptCursor);

	BOOL bDrawBorder = TRUE;
	BOOL bFlat = TRUE;

	int nAlign = CTPaintManager::__ALIGN_HORIZ_CENTER|CTPaintManager::__ALIGN_VERT;
	if(_info.m_bBtnHelp)
	{
		g_PaintManager->PaintPushButton(
			dc, bHorz,
			_info.m_rcBtnHelp,
			_T(""),
			NULL,
			bFlat,
			IsHover() && _info.m_rcBtnHelp.PtInRect(ptCursor),
			IsPressed() && _info.m_rcBtnHelp.PtInRect(ptCursor),
			FALSE,
			IsEnabled(),
			bDrawBorder,
			FALSE,
			FALSE,
			nAlign,
			NULL,
			FALSE,
			SC_CONTEXTHELP);
	}

	if(_info.m_bBtnMinimize)
	{
		g_PaintManager->PaintPushButton(
			dc, bHorz,
			_info.m_rcBtnRestore,
			_T(""),
			NULL,
			bFlat,
			IsHover() && _info.m_rcBtnRestore.PtInRect(ptCursor),
			IsPressed() && _info.m_rcBtnRestore.PtInRect(ptCursor),
			FALSE,
			IsEnabled(),
			bDrawBorder,
			FALSE,
			FALSE,
			nAlign,
			NULL,
			FALSE,
			SC_RESTORE);

		g_PaintManager->PaintPushButton(
			dc, bHorz,
			_info.m_rcBtnMinimize,
			_T(""),
			NULL,
			bFlat,
			IsHover() && _info.m_rcBtnMinimize.PtInRect(ptCursor),
			IsPressed() && _info.m_rcBtnMinimize.PtInRect(ptCursor),
			FALSE,
			IsEnabled(),
			bDrawBorder,
			FALSE,
			FALSE,
			nAlign,
			NULL,
			FALSE,
			SC_MINIMIZE);
	}

	if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize )
	{
		g_PaintManager->PaintPushButton(
			dc, bHorz,
			_info.m_rcBtnMaximize,
			_T(""),
			NULL,
			bFlat,
			IsHover() && _info.m_rcBtnMaximize.PtInRect(ptCursor),
			IsPressed() && _info.m_rcBtnMaximize.PtInRect(ptCursor),
			FALSE,
			IsEnabled(),
			bDrawBorder,
			FALSE,
			FALSE,
			nAlign,
			NULL,
			FALSE,
			SC_MAXIMIZE);
	}

	if(_info.m_bBtnClose)
	{
		g_PaintManager->PaintPushButton(
			dc, bHorz,
			_info.m_rcBtnClose,
			_T(""),
			NULL,
			bFlat,
			IsHover() && _info.m_rcBtnClose.PtInRect(ptCursor),
			IsPressed() && _info.m_rcBtnClose.PtInRect(ptCursor),
			FALSE,
			IsEnabled(),
			bDrawBorder,
			FALSE,
			FALSE,
			nAlign,
			NULL,
			FALSE,
			SC_CLOSE);
	}
}

void CTBarMdiRightButton::OnTrackPopup( CPoint point)
{
}

void CTBarMdiRightButton::OnClick( CPoint point, BOOL bDown)
{
	if(bDown)
		return;

	CTMenuControlBar *pBar = DYNAMIC_DOWNCAST(
		CTMenuControlBar,
		m_pBar);

	BOOL bHorz = !pBar->IsDockedVertically();
	MdiMenuBarRightButtonsInfo _info;

	pBar->_GetMdiMenuBarRightButtonsInfo( _info, this, bHorz);
	if(!_info.m_bInitialized)
		return;

	_info.InitLocations( Rect(), bHorz);
	BOOL bInstallHideFlag = FALSE;
	UINT nHelperSysCmdID = 0;

	if( _info.m_bBtnHelp && _info.m_rcBtnHelp.PtInRect(point) )
		nHelperSysCmdID = SC_CONTEXTHELP;
	else if( _info.m_bBtnMinimize && _info.m_rcBtnRestore.PtInRect(point) )
	{
		nHelperSysCmdID = SC_RESTORE;
		bInstallHideFlag = TRUE;
	}
	else if( _info.m_bBtnMinimize && _info.m_rcBtnMinimize.PtInRect(point) )
	{
		nHelperSysCmdID = SC_MINIMIZE;
		bInstallHideFlag = TRUE;
	}
	else if( _info.m_bBtnMaximize && !_info.m_bBtnMinimize && _info.m_rcBtnMaximize.PtInRect(point))
		nHelperSysCmdID = SC_MAXIMIZE;
	else if( _info.m_bBtnClose && _info.m_rcBtnClose.PtInRect(point) )
		nHelperSysCmdID = SC_CLOSE;

	if(!nHelperSysCmdID)
		return;

	BOOL bDummyMax = FALSE;
	HWND hWndActiveChild = _GetActiveMdiChildWnd(bDummyMax);

	if(!hWndActiveChild)
		return;

	pBar->ClientToScreen(&point);
	::PostMessage(
		hWndActiveChild,
		WM_SYSCOMMAND,
		nHelperSysCmdID,
		MAKELONG( point.x, point.y));

	if(bInstallHideFlag)
	{
		Show(FALSE);
		CTBarButton *pTBB = m_pBar->GetButton(0);

		if( pTBB->IsVisible() && pTBB->IsKindOf(RUNTIME_CLASS(CTBarMdiDocButton)) )
			pTBB->Show(FALSE);

		m_pBar->Invalidate();
		m_pBar->UpdateWindow();
	}
}

int CTBarMdiRightButton::OnToolHitTest( CPoint point, TOOLINFO *pTI)
{
	return -1;
}

void CTMenuControlBar::_KillFrameMenu()
{
	if(m_bPresubclassDialogMode)
		return;

	if(IsOleIpObjActive())
		return;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	HWND hWndFrame = pFrame->GetSafeHwnd();
	HMENU hFrameRealMenu = ::GetMenu(hWndFrame);

	if(!hFrameRealMenu)
		return;

	::SetMenu( hWndFrame, NULL);
	_UpdateMenuBar();
	GetDockingFrame()->DelayRecalcLayout();
}

void CTMenuControlBar::PreSubclassWindow()
{
	CTToolControlBar::PreSubclassWindow();

	if( m_bPresubclassDialogMode && !m_hWndHelper )
	{
		m_hWndHelper = ::GetParent(m_hWnd);

		if(!IsHookedWindow(m_hWndHelper))
			SetupHookWndSink(m_hWndHelper);
	}
}

BOOL CTMenuControlBar::PreTranslateMessage( MSG* pMsg)
{
	if(m_bPresubclassDialogMode)
	{
		if(TranslateMainFrameMessage(pMsg))
			return TRUE;
	}

	return CTToolControlBar::PreTranslateMessage(pMsg);
}
