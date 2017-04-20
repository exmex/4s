#include "stdafx.h"
#include "TMiniDockFrameWnd.h"
#include "TMemoryDC.h"
#include "TDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CX_BORDER   1
#define CY_BORDER   1

const UINT	CTControlBar::m_nMsgPutToPopupMenu = ::RegisterWindowMessage(_T("CTToolControlBar::m_nMsgPutToPopupMenu"));
const CSize CTToolControlBar::m_szDefTBB( 23, 22);

BOOL CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
BOOL CTToolControlBar::m_bMenuTracking = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CTBarButton

IMPLEMENT_DYNAMIC( CTBarButton, CObject)


BOOL CTBarButton::IsAbleToTrackMenu()
{
	if( GetMenu() || IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
		return TRUE;

	return FALSE;
}

CString CTBarButton::GetText() const
{
	CString strText(_T(""));

	if(IsSeparator())
		return strText;

	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
		GetCmdID());

	if(!p_cmd->m_strToolbarText.IsEmpty())
		strText = p_cmd->m_strToolbarText;

	int nTabChrPos = strText.Find(_T('\t'));
	if( nTabChrPos < 0 )
		return strText;

	return strText.Left(nTabChrPos);
}

BOOL CTBarButton::PutToPopupMenu( CTPopupMenuWnd *pPopup)
{
	if(m_pCtrl)
	{
		if( m_pCtrl->SendMessage(
			CTToolControlBar::m_nMsgPutToPopupMenu,
			reinterpret_cast <WPARAM> (pPopup), 0))
			return TRUE;
	}

	HMENU hMenu = GetMenu();
	if(hMenu)
	{
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
			GetCmdID());

		CString strText = p_cmd->m_strMenuText;
		if(strText.IsEmpty())
			strText = p_cmd->m_strToolbarText;

		HICON hIcon = g_CmdManager->CmdGetHICON(
			g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
			GetCmdID());

		if(hIcon)
		{
			CTCmdManager::icon _icon(
				hIcon,
				TRUE);

			hIcon = _icon.Detach();
		}

		pPopup->ItemInsert(
			(UINT) CTPopupMenuWnd::TYPE_POPUP,
			-1, (LPCTSTR) strText, hIcon);

		CTPopupMenuWnd *pChildPopup = pPopup->ItemGetPopup(pPopup->ItemGetCount() - 1);
		CMenu _menu;

		_menu.Attach(hMenu);
		pChildPopup->UpdateFromMenu(
			GetCmdTargetWnd()->GetSafeHwnd(),
			&_menu,
			IsContainerOfPopupLikeMenu(),
			FALSE);

		_menu.Detach();
	}
	else if(!pPopup->ItemInsert(GetCmdID()))
		return FALSE;

	return TRUE;
}

CSize CTBarButton::CalculateLayout( CDC &dc,
								    CSize szPreCalc,
									BOOL bHorz)
{
	if( m_pCtrl && (!m_bVertDocked || GetCtrlVisibleVertically()) )
	{
		CRect rcCtrl;

		m_pCtrl->GetWindowRect(&rcCtrl);
		m_ActiveSize.cx = rcCtrl.Width();
		m_ActiveSize.cy = szPreCalc.cy;

		return m_ActiveSize;
	}
	m_ActiveSize = szPreCalc;

	if(IsSeparator())
	{
		if(bHorz)
			m_ActiveSize.cx = __TB_SEPARATOR_WIDTH__;
		else
			m_ActiveSize.cy = __TB_SEPARATOR_WIDTH__;
	}
	else
	{
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
			GetCmdID());

		CTCmdManager::icon *p_icon = g_CmdManager->CmdGetIconPtr(
			g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
			GetCmdID());

		CSize _size(0,0);
		if(p_icon)
			_size = *p_icon;

		CString strMeasureText = GetText();
		if(!strMeasureText.IsEmpty())
		{
			static TCHAR strDummyAmpSeq[] = _T("\001\001");

			strMeasureText.Replace( _T("&&"), strDummyAmpSeq);
			strMeasureText.Remove(_T('&'));
			strMeasureText.Replace( strDummyAmpSeq, _T("&"));

			int nPixTextWidth = dc.GetTextExtent((LPCTSTR) strMeasureText).cx + __TB_TEXT_MARGINE__ * 2;
			if(bHorz)
				_size.cx += nPixTextWidth;
			else
				_size.cy += nPixTextWidth;
		}

		if( m_ActiveSize.cx < _size.cx )
			m_ActiveSize.cx = _size.cx;

		if( m_ActiveSize.cy < _size.cy )
			m_ActiveSize.cy = _size.cy;
	}

	if( GetMenu() && !m_pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)) )
	{
		if(bHorz)
			m_ActiveSize.cx += __DROPDOWN_ARROW_GAP * 2 + CTPaintManager::m_wingl_btn_expand_bottom.Size().cx;
		else
			m_ActiveSize.cy += __DROPDOWN_ARROW_GAP * 2 + CTPaintManager::m_wingl_btn_expand_right.Size().cy;
	}

	m_ActiveSize.cx += __TB_BUTTON_MARGINE__ * 2;
	m_ActiveSize.cy += __TB_BUTTON_MARGINE__ * 2;

	return m_ActiveSize;
}

HICON CTBarButton::GetHICON()
{
	HICON hIcon = g_CmdManager->CmdGetHICON(
		g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
		GetCmdID());

	return hIcon;
}

void CTBarButton::Paint( CDC &dc, BOOL bHorz)
{
	if( m_pCtrl && (!m_bVertDocked || GetCtrlVisibleVertically()) )
		return;

	CRect rcArea(m_ActiveRect);
	rcArea.DeflateRect(
		__TB_BUTTON_MARGINE__,
		__TB_BUTTON_MARGINE__);

	if( rcArea.right <= rcArea.left ||
		rcArea.bottom <= rcArea.top )
		return;

	if(GetStyle()&TBBS_HIDDEN)
		return;

	if( !IsVisible() || !dc.RectVisible(&m_ActiveRect) )
		return;

	BOOL bPushed = IsPressed();
	BOOL bEnabled = !IsDisabled();
	BOOL bHover = IsHover() &&
		!CTToolControlBar::m_bMenuTracking &&
		!CTPopupMenuWnd::IsMenuTracking();

	BOOL bIndeterminate = IsIndeterminate();
	CString strText = GetText();

	g_PaintManager->PaintPushButton(
		dc, bHorz,
		rcArea,
		(LPCTSTR) strText,
		GetHICON(),
		TRUE,
		bHover,
		bPushed,
		bIndeterminate,
		bEnabled,
		TRUE,
		FALSE,
		FALSE,
		CTPaintManager::__ALIGN_HORIZ_CENTER|
		CTPaintManager::__ALIGN_VERT,
		NULL,
		GetMenu() && !m_pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)),
		0, bEnabled && !bHover && !bPushed);
}

void CTBarButton::SetMenu( HMENU hMenu,
						   BOOL bPopupMenu,
						   BOOL bAutoDestroyMenu)
{
	_DestroyMenu();
	m_hMenu = hMenu;

	if(m_hMenu)
	{
		g_CmdManager->UpdateFromMenu(
			g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
			hMenu);

		m_bPopupMenu = bPopupMenu;
		ModifyStyle( 0, TBBS_DISABLED);
		m_bAutoDestroyMenu = bAutoDestroyMenu;
	}
}

UINT CTBarButton::_GetTrackPopupFlags()
{
	if(m_pBar->m_bPresubclassDialogMode)
	{
		if(m_pBar->IsDockedAtRight())
			return TPMX_RIGHTALIGN;

		if(m_pBar->IsDockedAtLeft())
			return TPMX_LEFTALIGN;

		if(m_pBar->IsDockedAtBottom())
			return TPMX_BOTTOMALIGN;

		return TPMX_TOPALIGN;
	}

	switch(m_pBar->GetSafeDockBarDlgCtrlID())
	{
	case AFX_IDW_DOCKBAR_TOP		: return TPMX_TOPALIGN;
	case AFX_IDW_DOCKBAR_BOTTOM		: return TPMX_BOTTOMALIGN;
	case AFX_IDW_DOCKBAR_LEFT		: return TPMX_LEFTALIGN;
	case AFX_IDW_DOCKBAR_RIGHT		: return TPMX_RIGHTALIGN;
	default							: return TPMX_TOPALIGN;
	}
}

CWnd *CTBarButton::GetCmdTargetWnd()
{
	CWnd *pWnd = m_pBar->GetOwner();
	return pWnd;
}

void CTBarButton::OnTrackPopup( CPoint point)
{
	if(!IsEnabled())
		return;

	HMENU hMenu = GetMenu();
	if(!hMenu)
		return;

	BOOL bPrevTBMT = CTToolControlBar::m_bMenuTracking;
	if( CTToolControlBar::m_bMenuTracking &&
		m_pBar->_GetIndexOf(this) == m_pBar->m_nBtnIdxMenuTracking )
		return;

	CTToolControlBar::_CloseTrackingMenus();
	if(m_pBar->IsFloating())
	{
		m_pBar->ActivateTopParent();
		CFrameWnd *pFrame = m_pBar->GetDockingFrame();

		pFrame->BringWindowToTop();
	}

	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();
	CWnd *pWndCmdTarger = GetCmdTargetWnd();
	CMenu _menu;

	_menu.Attach(hMenu);
	pPopup->UpdateFromMenu(
		pWndCmdTarger->GetSafeHwnd(),
		&_menu,
		m_bPopupMenu);
	_menu.Detach();

	if(IsAppendMdiWindowsMenu())
		pPopup->UpdateMdiWindowsMenu(pWndCmdTarger);

	CTToolControlBar::m_bMenuTracking = bPrevTBMT;
	CRect rcBtn = Rect();

	m_pBar->ClientToScreen(&rcBtn);
	m_pBar->ClientToScreen(&point);

	DWORD dwTrackFlags = _GetTrackPopupFlags()|
		TPMX_COMBINE_DEFAULT|
		TPMX_OWNERDRAW_FIXED;

	if(m_pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
		dwTrackFlags |= TPMX_SELECT_ANY;

	if(CTToolControlBar::m_bMenuTrackingExpanded)
		dwTrackFlags |= TPMX_NO_HIDE_RARELY;

	m_pBar->_SwitchMenuTrackingIndex(m_pBar->_GetIndexOf(this));
	if(!pPopup->TrackPopupMenu(
		dwTrackFlags,
		point.x,
		point.y,
		&rcBtn,
		m_pBar,
		CTToolControlBar::_CbPaintCombinedContent))
	{
		delete pPopup;

		CTToolControlBar::_CloseTrackingMenus();
		return;
	}

	CTToolControlBar::m_bMenuTracking = TRUE;
}

void CTBarButton::OnHover( CPoint point, BOOL bOn)
{
	if(bOn)
	{
		if(CTToolControlBar::m_bMenuTracking)
			OnTrackPopup(point);
		else
		{
			m_pBar->GetOwner()->SendMessage(
				WM_SETMESSAGESTRING,
				(WPARAM) CTCmdManager::IsCommand(GetCmdID() ? GetCmdID() : AFX_IDS_IDLEMESSAGE));
		}

		CWnd *pCtrl = CtrlGet();
		if( pCtrl == NULL || !(pCtrl->GetStyle()&WS_VISIBLE) )
			m_pBar->SetCapture();
	}
	else
		ReleaseCapture();
}

void CTBarButton::OnClick( CPoint point, BOOL bDown)
{
	if(bDown)
	{
		CTToolControlBar::_CloseTrackingMenus();

		if(GetMenu())
		{
			CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
			OnTrackPopup(point);

			return;
		}

		m_pBar->GetOwner()->SendMessage(
			WM_SETMESSAGESTRING,
			(WPARAM) GetCmdID());

		return;
	}

	m_pBar->GetOwner()->SendMessage(
		WM_SETMESSAGESTRING,
		AFX_IDS_IDLEMESSAGE);

	if(GetMenu())
		return;

	CWnd *pCtrl = CtrlGet();
	if(	pCtrl && pCtrl->GetStyle()&WS_VISIBLE )
	{
		CTToolControlBar *pBar = GetBar();

		if( CWnd::GetCapture() == pBar )
			pBar->SendMessage(WM_CANCELMODE);

		return;
	}

	if(!CTCmdManager::IsCommand(GetCmdID()))
		return;

	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(m_pBar->GetSafeHwnd()),
		GetCmdID());

	if(!p_cmd)
		return;

	p_cmd->Deliver(
		m_pBar,
		TRUE);
}

int CTBarButton::OnToolHitTest( CPoint point, TOOLINFO *pTI)
{
	if(IsSeparator())
		return -1;

	int nCmdID = (int) GetCmdID();
	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(GetBar()->GetSafeHwnd()),
		nCmdID);

	if( !p_cmd || p_cmd->m_strTipTool.IsEmpty() )
		return FALSE;

	if(pTI)
	{
		CRect rcArea = Rect();

		::CopyRect( &pTI->rect, &rcArea);
		pTI->uId = (UINT) nCmdID;
		pTI->hwnd = GetBar()->GetSafeHwnd();
		pTI->lpszText = (LPTSTR) ::calloc(
			p_cmd->m_strTipTool.GetLength() + 1,
			sizeof(TCHAR));

		if(pTI->lpszText)
			_tcscpy( pTI->lpszText, (LPCTSTR) p_cmd->m_strTipTool);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;
	}

	return nCmdID;
}


IMPLEMENT_DYNAMIC( CTBarContentExpandButton, CTBarButton)

CSize CTBarContentExpandButton::CalculateLayout( CDC &dc,
												 CSize szPreCalc,
												 BOOL bHorz)
{
	CSize _size( bHorz ? __RIGHT_BUTTON_HORZ_DX__ : szPreCalc.cx, bHorz ? szPreCalc.cy : __RIGHT_BUTTON_VERT_DY__);

	m_ActiveSize = _size;
	m_ActiveSize.cx += __TB_BUTTON_MARGINE__ * 2;
	m_ActiveSize.cy += __TB_BUTTON_MARGINE__ * 2;

	return m_ActiveSize;
}

void CTBarContentExpandButton::Paint( CDC &dc, BOOL bHorz)
{
	CRect rcArea(m_ActiveRect);

	rcArea.DeflateRect(
		__TB_BUTTON_MARGINE__,
		__TB_BUTTON_MARGINE__);

	if( rcArea.right <= rcArea.left ||
		rcArea.bottom <= rcArea.top)
		return;

	if(!dc.RectVisible(&m_ActiveRect))
		return;

	BOOL bEnabled = !IsDisabled();
	BOOL bBarIsCompletelyVisible = !GetButtons().GetSize();
	BOOL bEmpty = !m_pBar->m_bRightButtonDisplayBarsList && bBarIsCompletelyVisible;
	BOOL bPushed = !bEmpty && bEnabled && IsPressed();
	BOOL bHover = !bEmpty && bEnabled && IsHover() && !CTToolControlBar::m_bMenuTracking && !CTPopupMenuWnd::IsMenuTracking();

	g_PaintManager->PaintToolbarExpandButton(
		dc,
		rcArea,
		bHorz,
		bBarIsCompletelyVisible,
		bEnabled && !bEmpty,
		bPushed,
		bHover,
		!bHover && !bPushed);
}

BOOL CTBarContentExpandButton::PutToPopupMenu( CTPopupMenuWnd *pPopup)
{
	return FALSE;
}

void CTBarContentExpandButton::OnTrackPopup( CPoint point)
{
	if(!IsEnabled())
		return;

	BOOL bPrevTBMT = CTToolControlBar::m_bMenuTracking;
	if( CTToolControlBar::m_bMenuTracking &&
		m_pBar->_GetIndexOf(this) == m_pBar->m_nBtnIdxMenuTracking)
		return;

	BOOL bBarIsCompletelyVisible = !GetButtons().GetSize();
	BOOL bEmpty = !m_pBar->m_bRightButtonDisplayBarsList && bBarIsCompletelyVisible;

	if(bEmpty)
		return;

	CTToolControlBar::_CloseTrackingMenus();
	CTToolControlBar::m_bMenuTracking = bPrevTBMT;

	if(m_pBar->IsFloating())
	{
		m_pBar->ActivateTopParent();
		CFrameWnd *pFrame = m_pBar->GetParentFrame();

		if(pFrame)
			pFrame->BringWindowToTop();
	}

	HWND hWndTrack = GetCmdTargetWnd()->GetSafeHwnd();
	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();

	pPopup->CreatePopupMenu(hWndTrack);
	int nCount = GetButtons().GetSize();

	if( nCount != 0 )
	{
		int nCountBefore = pPopup->ItemGetCount();
		for( int i=0; i<nCount; i++)
		{
			CTBarButton *pTBB = GetButtons()[i];

			if( i==0 && pTBB->IsSeparator() )
				continue;
			pTBB->PutToPopupMenu(pPopup);
		}

		int nCountAfter = pPopup->ItemGetCount();
		if( nCountAfter != nCountBefore &&
			!m_pBar->m_bPresubclassDialogMode &&
			m_pBar->m_bRightButtonDisplayBarsList )
			pPopup->ItemInsert(CTPopupMenuWnd::TYPE_SEPARATOR);
	}

	if( !m_pBar->m_bPresubclassDialogMode && m_pBar->m_bRightButtonDisplayBarsList )
	{
		CFrameWnd *pFrame = m_pBar->_GetDockingFrameImpl();
		if(pFrame)
		{
			CString strShowHidePanels(_T("&Show/hide panels"));

			pPopup->ItemInsert(
				(UINT) CTPopupMenuWnd::TYPE_POPUP,
				-1, strShowHidePanels);

			CTPopupMenuWnd *pPopupBars = pPopup->ItemGetPopup(pPopup->ItemGetCount() - 1);
			CTDockBar::_ContextMenuBuild(
				pFrame,
				pPopupBars);
		}
	}

	CRect rcBtn = Rect();
	m_pBar->ClientToScreen(&rcBtn);
	m_pBar->ClientToScreen(&point);

	DWORD dwTrackFlags = _GetTrackPopupFlags()|
		TPMX_COMBINE_DEFAULT|
		TPMX_OWNERDRAW_FIXED;

	if(m_pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
		dwTrackFlags |= TPMX_SELECT_ANY;

	if(CTToolControlBar::m_bMenuTrackingExpanded)
		dwTrackFlags |= TPMX_NO_HIDE_RARELY;

	m_pBar->_SwitchMenuTrackingIndex(m_pBar->_GetIndexOf(this));
	if(!pPopup->TrackPopupMenu(
		dwTrackFlags,
		point.x,
		point.y,
		&rcBtn,
		m_pBar,
		CTToolControlBar::_CbPaintCombinedContent))
	{
		delete pPopup;
		CTToolControlBar::_CloseTrackingMenus();

		return;
	}

	CTToolControlBar::m_bMenuTracking = TRUE;
}

void CTBarContentExpandButton::OnHover( CPoint point,
									    BOOL bOn)
{
	CTBarButton::OnHover(
		point,
		bOn);
}

void CTBarContentExpandButton::OnClick( CPoint point,
									    BOOL bDown)
{
	if(!bDown)
		return;

	CTToolControlBar::m_bMenuTrackingExpanded = FALSE;
	OnTrackPopup(point);
}

int CTBarContentExpandButton::OnToolHitTest( CPoint point,
											 TOOLINFO *pTI)
{
	if(pTI)
	{
		CRect rcArea = Rect();
		::CopyRect(
			&(pTI->rect),
			&rcArea);

		int nCmdID = (int) GetBar()->GetDlgCtrlID();
		CString strExpandTip(_T("No buttons"));

		pTI->uId = (UINT) nCmdID;
		pTI->hwnd = GetBar()->GetSafeHwnd();

		pTI->lpszText = (LPTSTR) ::calloc(
			strExpandTip.GetLength() + 1,
			sizeof(TCHAR));

		if(pTI->lpszText)
			_tcscpy( pTI->lpszText, (LPCTSTR) strExpandTip);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;

		return nCmdID;
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CTToolControlBar

IMPLEMENT_DYNAMIC( CTToolControlBar, CTControlBar)

CTBarButton * CTToolControlBar::_GetButtonPtr( int nIndex) const
{
	int nButtonsCount = m_buttons.GetSize();

	if( nIndex < 0 || nIndex >= nButtonsCount )
		return NULL;

	CTBarButton *pTBB = m_buttons[nIndex];
	return pTBB;
}

void CTToolControlBar::_RemoveAllButtonsImpl()
{
	for( int i=0; i<m_buttons.GetSize(); i++)
	{
		CTBarButton *pTBB = m_buttons[i];
		delete pTBB;
	}

	m_buttons.RemoveAll();
	m_pRightBtn = NULL;
	m_nBtnIdxCapture = -1;
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;
}

int CTToolControlBar::_GetButtonsCountImpl() const
{
	return m_buttons.GetSize();
}

CTToolControlBar::CTToolControlBar()
{
	m_bRightButtonDisplayBarsList = TRUE;
	m_bGripperStaticallyAtTop = FALSE;
	m_bReposSingleChildMode = FALSE;
	m_nBtnIdxMenuTracking = -1;
	m_nBtnIdxCapture = -1;
	m_nBtnIdxHover = -1;
	m_bFixedMode = TRUE;
	m_pRightBtn = NULL;

	m_cxLeftBorder = 1;
	m_cxRightBorder = 1;
	m_cyTopBorder = 1;
	m_cyBottomBorder = 1;

	m_nGripWidthAtLeft = 8;
	m_nGripHeightAtTop = 10;
}

CTToolControlBar::~CTToolControlBar()
{
	_RemoveAllButtonsImpl();

	int nCount = m_AllBars.GetSize();
	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = m_AllBars[i];

		if( pBar == this )
		{
			m_AllBars.RemoveAt(i);
			break;
		}
	}
}

void CTToolControlBar::SetSizes( SIZE szTBB, SIZE szImage)
{
}

BOOL CTToolControlBar::LoadToolBar( LPCTSTR lpszResourceName)
{
	CToolBar _bar;

	if(!_bar.Create(
		this,
		WS_CHILD|
		CBRS_TOP,
		AFX_IDW_TOOLBAR + 1000))
		return FALSE;

	if(!_bar.LoadToolBar(lpszResourceName))
		return FALSE;

	if(!g_CmdManager->UpdateFromToolBar( g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()), _bar))
		return FALSE;

	int nIDCount = _bar.GetToolBarCtrl().GetButtonCount();
	if( nIDCount <= 0 )
		return TRUE;

	UINT *lpIDArray = new UINT[nIDCount];
	if(!lpIDArray)
		return FALSE;

	for( int i=0; i<nIDCount; i++)
	{
		UINT nStyle;
		UINT nID;
		int nImage;

		_bar.GetButtonInfo( i, nID, nStyle, nImage);
		lpIDArray[i] = nID;
	}

	BOOL bRetVal = SetButtons(
		lpIDArray,
		nIDCount);

	delete[] lpIDArray;
	return bRetVal;
}

BOOL CTToolControlBar::_IsShowContentWhenDragging() const
{
	return TRUE;
}

BOOL CTToolControlBar::RemoveButton( int nPos, BOOL bDoRecalcLayout)
{
	if( nPos < 0 )
		return FALSE;

	int nCount = _GetButtonsCountImpl();
	if( nPos >= nCount )
		return FALSE;

	CTBarButton *pTBB = _GetButtonPtr(nPos);
	if(pTBB->IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
		return FALSE;

	delete pTBB;
	m_buttons.RemoveAt(nPos);
	m_nBtnIdxCapture = -1;
	m_nBtnIdxHover = -1;
	m_nBtnIdxMenuTracking = -1;

	if(bDoRecalcLayout)
		_RecalcLayoutImpl();

	return TRUE;
}

BOOL CTToolControlBar::InsertSpecButton( int nPos,
										 CTBarButton *pButton,
										 BOOL bDoRecalcLayout)
{
	int nCount = _GetButtonsCountImpl();
	if( nPos < 0 )
		nPos = nCount;

	if( nPos > nCount )
		return FALSE;

	if( nCount > 0 && nPos == nCount )
	{
		CTBarButton *pTBB = _GetButtonPtr( nCount - 1 );

		if(pTBB->IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
			nPos--;
	}

	if( _GetIndexOf(pButton) >= 0 )
		return FALSE;

	m_buttons.InsertAt( nPos, pButton);
	if(bDoRecalcLayout)
		_RecalcLayoutImpl();

	return TRUE;
}

BOOL CTToolControlBar::InsertButton( int nPos,
									 UINT nCmdID,
									 BOOL bDoRecalcLayout)
{
	int nCount = _GetButtonsCountImpl();

	if( nPos < 0 )
		nPos = nCount;

	if( nPos > nCount )
		return FALSE;

	if( nCount > 0 && nPos == nCount )
	{
		CTBarButton *pTBB = _GetButtonPtr(nCount - 1);

		if(pTBB->IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
			nPos--;
	}

	try
	{
		CTBarButton *pTBB = new CTBarButton(
			this,
			nCmdID);

		m_buttons.InsertAt(
			nPos,
			pTBB);

		if(bDoRecalcLayout)
			_RecalcLayoutImpl();
	}
	catch( CException *pExcept)
	{
		pExcept->Delete();
		return FALSE;
	}
	catch( ... )
	{
		return FALSE;
	}

	return TRUE;
}

CTBarContentExpandButton *CTToolControlBar::OnCreateBarRightBtn()
{
	CTBarContentExpandButton *pRightBtn = new CTBarContentExpandButton(this);
	return pRightBtn;
}

BOOL CTToolControlBar::SetButtons( const UINT *lpIDArray, int nIDCount)
{
	_RemoveAllButtonsImpl();
	if( lpIDArray == NULL ||
		nIDCount == 0 )
		return TRUE;

	try
	{
		for( int i=0; i<nIDCount; i++)
		{
			CTBarButton *pTBB = new CTBarButton(
				this,
				*lpIDArray++);

			m_buttons.Add(pTBB);
		}

		m_pRightBtn = OnCreateBarRightBtn();
		if(m_pRightBtn)
			m_buttons.Add(m_pRightBtn);
	}
	catch( CException *pExcept)
	{
		pExcept->Delete();
		return FALSE;
	}
	catch( ... )
	{
		return FALSE;
	}

	return TRUE;
}

int CTToolControlBar::CommandToIndex( UINT nIDFind) const
{
	int nCount = _GetButtonsCountImpl();

	for( int i=0; i<nCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);

		if( pTBB->GetCmdID() == nIDFind )
			return i;
	}

	return -1;
}

UINT CTToolControlBar::GetButtonID( int nIndex) const
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return ID_SEPARATOR;

	return pTBB->GetCmdID();
}

void CTToolControlBar::GetButtonRect( int nIndex, LPRECT lpRect) const
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
	{
		::memset( lpRect, 0, sizeof(RECT));
		return;
	}

	*lpRect = *pTBB;
}

UINT CTToolControlBar::GetButtonStyle( int nIndex) const
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return 0;

	return pTBB->GetStyle();
}

void CTToolControlBar::SetButtonStyle( int nIndex, UINT nStyle)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return;

	UINT nOldStyle = pTBB->GetStyle();
	if( nOldStyle != nStyle )
	{
		pTBB->SetStyle(nStyle);
		_InvalidateButton(nIndex);

		UpdateWindow();
	}
}

CWnd *CTToolControlBar::GetButtonCtrl( int nIndex)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return NULL;

	CWnd *pCtrl = pTBB->CtrlGet();
	return pCtrl;
}

void CTToolControlBar::SetButtonCtrlVisibleVertically( int nIndex, BOOL bVisible)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return;

	pTBB->SetCtrlVisibleVertically(bVisible);
}

BOOL CTToolControlBar::GetButtonCtrlVisibleVertically( int nIndex) const
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return FALSE;

	return pTBB->GetCtrlVisibleVertically();
}

BOOL CTToolControlBar::SetButtonCtrl( int nIndex,
									  CWnd *pCtrl,
									  BOOL bCtrlAutoDestroyed)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return FALSE;

	if(pCtrl)
	{
		DWORD dwCtrlStyle = pCtrl->GetStyle();

		if(!(dwCtrlStyle&WS_CHILD))
			return FALSE;

		if(!IsChild(pCtrl))
			return FALSE;

		UINT nDlgCtrlID = (UINT)pCtrl->GetDlgCtrlID();
		if( nDlgCtrlID != pTBB->GetCmdID() )
			return FALSE;
	}

	pTBB->CtrlSet(
		pCtrl,
		bCtrlAutoDestroyed);

	_RecalcLayoutImpl();
	return TRUE;
}

int CTToolControlBar::GetButtonByAccessKey( TCHAR vkTCHAR)
{
	int nCount = _GetButtonsCountImpl();

	if(!nCount)
		return -1;

	for( int i=0; i<nCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);

		if(	pTBB->IsSeparator() ||
			pTBB->GetStyle()&TBBS_HIDDEN ||
			pTBB->IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
			continue;

		UINT nCmdID = pTBB->GetCmdID();
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(GetSafeHwnd()),
			nCmdID);

		if(p_cmd->m_strToolbarText.IsEmpty())
			continue;

		int nTextLen = p_cmd->m_strToolbarText.GetLength();
		int nAmdIndex = p_cmd->m_strToolbarText.Find(_T('&'));

		if( nAmdIndex == nTextLen - 1 )
			continue;

		TCHAR vk = p_cmd->m_strToolbarText[nAmdIndex + 1];
		TCHAR strChar[2] = { vk, _T('\0')};

		::CharUpper(strChar);
		vk = strChar[0];

		if( vkTCHAR == vk )
		{
			if(!pTBB->IsVisible())
			{
				if(m_pRightBtn)
					return _GetIndexOf(m_pRightBtn);

				continue;
			}

			return i;
		}
	}

	return -1;
}

HMENU CTToolControlBar::GetButtonMenu( int nIndex)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return NULL;

	return ((HMENU) (*pTBB));
}

BOOL CTToolControlBar::MarkButtonAsMdiWindowsMenu( int nIndex,
												   BOOL bAppendMdiWindowsMenu)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return FALSE;

	pTBB->SetAppendMdiWindowsMenu(bAppendMdiWindowsMenu);
	return TRUE;
}

BOOL CTToolControlBar::SetButtonMenu( int nIndex,
									  HMENU hMenu,
									  BOOL bPopupMenu,
									  BOOL bAutoDestroyMenu,
									  BOOL bDoRecalcLayout)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return FALSE;

	pTBB->SetMenu(
		hMenu,
		bPopupMenu,
		bAutoDestroyMenu);

	if(bDoRecalcLayout)
		_RecalcLayoutImpl();

	return TRUE;
}

int CTToolControlBar::GetVisibleButton( int nBtnIdx,
									    BOOL bNext,
										BOOL bPassDisabled)
{
	int nCount = _GetButtonsCountImpl();

	if(!nCount)
		return -1;

	if( nCount == 1 )
		return 0;

	if(bNext)
	{
		int nNewButton = nBtnIdx + 1;

		if( nNewButton >= nCount )
			nNewButton = 0;

		while( nNewButton != nBtnIdx )
		{
			CTBarButton *pTBB = _GetButtonPtr(nNewButton);

			if( pTBB->IsVisible() &&
				!(pTBB->GetStyle()&TBBS_HIDDEN) &&
				!pTBB->IsSeparator() &&
				!pTBB->IsKindOf(RUNTIME_CLASS(CTBarMdiRightButton)) && (
				!bPassDisabled || (
				bPassDisabled &&
				!pTBB->IsDisabled())))
				return nNewButton;

			nNewButton++;
			if( nNewButton >= nCount )
				nNewButton = 0;
		}

		return nNewButton;
	}
	else
	{
		int nNewButton = nBtnIdx - 1;

		if( nNewButton < 0 )
			nNewButton = nCount - 1;

		while( nNewButton != nBtnIdx )
		{
			CTBarButton *pTBB = _GetButtonPtr(nNewButton);

			if( pTBB->IsVisible() &&
				!(pTBB->GetStyle()&TBBS_HIDDEN) &&
				!pTBB->IsSeparator() &&
				!pTBB->IsKindOf(RUNTIME_CLASS(CTBarMdiRightButton)) && (
				!bPassDisabled || (
				bPassDisabled &&
				!pTBB->IsDisabled())))
				return nNewButton;

			nNewButton--;
			if( nNewButton < 0 )
				nNewButton = nCount - 1;
		}

		return nNewButton;
	}
}

BOOL CTToolControlBar::TrackButtonMenu( int nIndex)
{
	if( m_bMenuTracking && m_nBtnIdxMenuTracking == nIndex )
		return TRUE;

	if(!SafeDisplayBar())
		return FALSE;

	CTBarButton *pTBB = _GetButtonPtr(nIndex);
	if(!pTBB)
		return FALSE;

	if(pTBB->GetStyle()&(TBBS_HIDDEN|TBBS_DISABLED))
		return FALSE;

	if(!pTBB->IsAbleToTrackMenu())
		return FALSE;

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	pTBB->OnTrackPopup(point);

	return TRUE;
}

CSize CTToolControlBar::CalcDynamicLayout( int nLength, DWORD dwMode)
{
	if( nLength == -1 &&
		!(dwMode&(LM_MRUWIDTH|LM_COMMIT)) &&
		dwMode&(LM_HORZDOCK|LM_VERTDOCK))
		return CalcFixedLayout( dwMode&LM_STRETCH, dwMode&LM_HORZDOCK);

	return _CalcLayout( dwMode, nLength);
}

CSize CTToolControlBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return _CalcLayout(dwMode);
}

CSize CTToolControlBar::_CalcLayout( DWORD dwMode, int nLength)
{
	CSize szCalcLayout = _GetDefButtonSize();

	if( _GetButtonsCountImpl() > 0 )
	{
		if(m_pRightBtn)
			m_pRightBtn->ClearContent(FALSE);

		BOOL bDynamic = m_dwStyle&CBRS_SIZE_DYNAMIC;
		int nLengthToSizeTB = (dwMode&LM_HORZ) ? 32767 : 0;
		BOOL bVertSizeTB = FALSE;

		if(bDynamic)
		{
			if(dwMode&LM_MRUWIDTH)
				nLengthToSizeTB = m_nMRUWidth;
			else if(dwMode&LM_HORZDOCK)
				nLengthToSizeTB = 32767;
			else if(dwMode&LM_VERTDOCK)
				nLengthToSizeTB = 0;
			else if( nLength != -1 )
			{
				CRect rcInside( 0, 0, 0, 0);
				_CalcInsideRect(
					rcInside,
					dwMode&LM_HORZ);

				bVertSizeTB = dwMode&LM_LENGTHY;
				nLengthToSizeTB = nLength + (bVertSizeTB ? rcInside.Height() : rcInside.Width());
			}
			else if(m_dwStyle&CBRS_FLOATING)
				nLengthToSizeTB = m_nMRUWidth;
		}

		_SizeToolBar(
			nLengthToSizeTB,
			bVertSizeTB);

		szCalcLayout = _CalcSize(!(dwMode&LM_HORZ));
		if( m_pRightBtn && !m_pRightBtn->GetButtons().GetSize() )
		{
			m_pRightBtn->ClearContent();
			szCalcLayout = _CalcSize(!(dwMode&LM_HORZ));
		}

		if( dwMode&LM_COMMIT &&
			m_dwStyle&(CBRS_FLOATING|CBRS_SIZE_DYNAMIC) &&
			dwMode&LM_HORZ )
			m_nMRUWidth = szCalcLayout.cx;

		CRect rcInside( 0, 0, 0, 0);
		_CalcInsideRect(
			rcInside,
			dwMode&LM_HORZ);

		szCalcLayout.cy -= rcInside.Height();
		szCalcLayout.cx -= rcInside.Width();

		CSize szCalcFixedLayout = CControlBar::CalcFixedLayout(
			dwMode&LM_STRETCH,
			dwMode&LM_HORZ);

		szCalcLayout.cx = max( szCalcLayout.cx, szCalcFixedLayout.cx);
		szCalcLayout.cy = max( szCalcLayout.cy, szCalcFixedLayout.cy);

		if( IsFloating() && IsBarWithGripper() )
		{
			int nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height());
			szCalcLayout.cy += nCyGripper;
		}
	}

	if(IsDockedHorizontally())
	{
		m_szDockedH = szCalcLayout;
		m_nMinHW = szCalcLayout.cy;
	}
	else if(IsDockedVertically())
	{
		m_szDockedV = szCalcLayout;
		m_nMinVH = szCalcLayout.cx;
	}
	else if(IsFloating())
		m_szFloated = szCalcLayout;

	if(!m_pDockSite)
	{
		if(IsDockedHorizontally())
			szCalcLayout.cy += 4;
		else
			szCalcLayout.cx += 4;
	}

	return szCalcLayout;
}

CSize CTToolControlBar::_CalcSize( BOOL bVerticallyDocked)
{
	int nCount = _GetButtonsCountImpl();

	if( nCount == 0 )
		return _GetDefButtonSize();

	CClientDC dc(this);
	CFont *pOldFont = (CFont *) dc.SelectObject( bVerticallyDocked ? &g_PaintManager->m_FontNormalVert : &g_PaintManager->m_FontNormal);
	CSize szCalc(_GetDefButtonSize());
	CPoint ptCurrent(
		__TB_BUTTON_MARGINE__,
		__TB_BUTTON_MARGINE__);

	for( int i=0; i<nCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);

		if(pTBB->GetStyle()&TBBS_HIDDEN)
			continue;

		if( i == nCount - 1 &&
			m_pRightBtn &&
			IsFloating())
			break;

		CSize szTBB( pTBB->CalculateLayout(
			dc, _GetDefButtonSize(),
			!bVerticallyDocked));

		if( !bVerticallyDocked && (
			ptCurrent.x == __TB_BUTTON_MARGINE__ ||
			pTBB->IsWrap()) &&
			pTBB->IsSeparator())
			szTBB = CSize( 0, 0);

		szCalc.cx = max(
			ptCurrent.x + szTBB.cx,
			szCalc.cx);

		szCalc.cy = max(
			ptCurrent.y + szTBB.cy,
			szCalc.cy);

		if(bVerticallyDocked)
		{
			ptCurrent.x = __TB_BUTTON_MARGINE__;
			ptCurrent.y += szTBB.cy;
		}
		else
		{
			ptCurrent.x += szTBB.cx;

			if(pTBB->IsWrap())
			{
				ptCurrent.x = __TB_BUTTON_MARGINE__;
				ptCurrent.y += _GetDefButtonSize().cy + __TB_LINE_OFFSET;
			}
		}
	}

	dc.SelectObject(pOldFont);
	if( bVerticallyDocked && szCalc.cx > __TB_LINE_OFFSET / 2 )
		szCalc.cx -= __TB_LINE_OFFSET / 2;

	if( !bVerticallyDocked && szCalc.cy > __TB_LINE_OFFSET / 2 )
		szCalc.cy -= __TB_LINE_OFFSET / 2;

	if(!bVerticallyDocked)
	{
		szCalc.cx += m_cxLeftBorder + m_cxRightBorder;
		szCalc.cx += __TB_LINE_OFFSET / 2;
	}
	else
	{
		szCalc.cy += m_cyTopBorder + m_cyBottomBorder;
		szCalc.cy += __TB_LINE_OFFSET;
	}

	return szCalc;
}

void CTToolControlBar::_SizeToolBar( int nLength, BOOL bVert)
{
	int nCount = _GetButtonsCountImpl();

	if(!nCount)
		return;

	if(!IsFloating())
	{
		_WrapToolBar(
			bVert ? 32767 : nLength,
			bVert ? nLength : 32767);

		return;
	}

	int nMinExtent = _GetDefButtonSize().cx / 2;
	if(bVert)
	{
		CSize size( 32767, 32767);

		for( int i = nMinExtent; nLength < size.cy; i += nMinExtent)
		{
			_WrapToolBar(i);
			size = _CalcSize(FALSE);
		}

		_WrapToolBar(size.cx);
		return;
	}

	int nCountOfRowsDesired = _WrapToolBar(nLength);
	int nCountOfRowsCurrent = _WrapToolBar(nMinExtent);

	if( nCountOfRowsCurrent == nCountOfRowsDesired || nMinExtent >= nLength )
	{
		_WrapToolBar(_CalcSize(FALSE).cx);
		return;
	}

	while( nMinExtent < nLength )
	{
		int nCurrentExtent = (nMinExtent + nLength) / 2;

		nCountOfRowsCurrent = _WrapToolBar(nCurrentExtent);
		if( nCountOfRowsCurrent == nCountOfRowsDesired )
		{
			nLength = nCurrentExtent;
			continue;
		}

		if( nMinExtent == nCurrentExtent )
		{
			_WrapToolBar(nLength);
			break;
		}

		nMinExtent = nCurrentExtent;
	}

	_WrapToolBar(_CalcSize(FALSE).cx);
}

int CTToolControlBar::_WrapToolBar( int nWidth, int nHeight)
{
	BOOL bVerticallyDocked = !(m_dwStyle&CBRS_ORIENT_HORZ);
	BOOL bFloating = IsFloating();
	CClientDC dc(this);

	CFont *pOldFont = (CFont *) dc.SelectObject( bVerticallyDocked ? &g_PaintManager->m_FontNormalVert : &g_PaintManager->m_FontNormal);
	CRect rcClient;

	GetClientRect(&rcClient);
	CPoint ptCurrent( 0, rcClient.top);

	if( !bFloating &&
		!bVerticallyDocked &&
		m_pRightBtn )
		nWidth -= m_pRightBtn->CalculateLayout( dc, _GetDefButtonSize(), TRUE).cx;

	int nCountOfRows = 1;
	int nCountOfButtons = _GetButtonsCountImpl();
	CTBarButton *pPrevButton = NULL;

	for( int i=0; i<nCountOfButtons; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);
		pTBB->SetWrap(FALSE);

		if( i == nCountOfButtons - 1 && m_pRightBtn )
			break;

		if(pTBB->GetStyle()&TBBS_HIDDEN)
			continue;

		CSize szTBB = pTBB->CalculateLayout(
			dc,
			_GetDefButtonSize(),
			!bVerticallyDocked);

		if( ptCurrent.x == 0 &&
			pTBB->IsSeparator())
			szTBB = CSize( 0, 0);

		if( pPrevButton && (
			bFloating ||
			ptCurrent.y + szTBB.cy < nHeight) &&
			ptCurrent.x + szTBB.cx > nWidth &&
			!pTBB->IsSeparator())
		{
			pPrevButton->SetWrap();
			ptCurrent.x = 0;
			ptCurrent.y += szTBB.cy + __TB_LINE_OFFSET;
			nCountOfRows++;
		}

		ptCurrent.x += szTBB.cx;
		pPrevButton = pTBB;
	}

	dc.SelectObject(pOldFont);
	return nCountOfRows;
}

void CTToolControlBar::GetButtonInfo( int nIndex, UINT& nID, UINT& nStyle) const
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
	{
		nStyle = 0;
		nID = 0;

		return;
	}

	nStyle = pTBB->GetStyle();
	nID = pTBB->GetCmdID();
}

void CTToolControlBar::SetButtonInfo( int nIndex, UINT nID, UINT nStyle)
{
	CTBarButton *pTBB = _GetButtonPtr(nIndex);

	if(!pTBB)
		return;

	pTBB->SetCmdID(nID);
	pTBB->SetStyle(nStyle);

	_InvalidateButton(nIndex);
	UpdateWindow();
}

void CTToolControlBar::DoPaint( CDC* pDC)
{
	CTPaintManager::ExcludeChildAreas(
		*pDC,
		*this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CTMemoryDC dc( pDC, &rcClient);
	if(dc.GetSafeHdc())
		pDC = &dc;

	pDC->FillSolidRect(
		&rcClient,
		g_PaintManager->GetColor(CTPaintManager::CLR_3DFACE_OUT));

	pDC->SetTextColor(g_PaintManager->GetColor(CTPaintManager::CLR_TEXT_OUT));
	pDC->SetBkMode(TRANSPARENT);

	CFont *pOldFont = pDC->SelectObject( m_dwStyle&CBRS_ORIENT_HORZ ? &g_PaintManager->m_FontNormal : &g_PaintManager->m_FontNormalVert);
	int nCountOfButtons = _GetButtonsCountImpl();

	for( int i=0; i<nCountOfButtons; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);

		if(pTBB->GetStyle()&TBBS_HIDDEN)
			continue;

		if(!pTBB->IsVisible())
			continue;

		CRect rcTBB = *pTBB;

		if(!pDC->RectVisible(&rcTBB))
			continue;

		if(!pTBB->IsSeparator())
		{
			pTBB->Paint(
				*pDC,
				m_dwStyle&CBRS_ORIENT_HORZ);

			continue;
		}

		BOOL bHorzSeparator = m_dwStyle&CBRS_ORIENT_HORZ;
		CRect rcSeparator(rcTBB);

		if( pTBB->IsWrap() && m_dwStyle&CBRS_ORIENT_HORZ )
		{
			rcSeparator.left = rcClient.left;
			rcSeparator.right = rcClient.right;
			rcSeparator.top = rcTBB.bottom;
			rcSeparator.bottom = rcSeparator.top + __TB_LINE_OFFSET;
			bHorzSeparator = FALSE;
		}

		rcSeparator.DeflateRect(
			bHorzSeparator ? 0 : 1,
			bHorzSeparator ? 1 : 0);

		g_PaintManager->PaintSeparator(
			*pDC,
			rcSeparator,
			bHorzSeparator);
	}

	pDC->SelectObject(pOldFont);
}

void CTToolControlBar::_InvalidateButton( int nIndex)
{
	CRect rect;

	GetButtonRect( nIndex, &rect);
	if(rect.IsRectEmpty())
		return;

	InvalidateRect(&rect);
}

int CTToolControlBar::OnToolHitTest( CPoint point, TOOLINFO *pTI) const
{
	if( CTToolControlBar::m_bMenuTracking ||
		CTPopupMenuWnd::IsMenuTracking())
		return -1;

	int nToolTipHit = CTControlBar::OnToolHitTest(
		point,
		pTI);

	if( nToolTipHit != -1 )
		return nToolTipHit;

	nToolTipHit = ((CTToolControlBar *) this)->HitTest(point);
	if( nToolTipHit < 0 )
		return -1;

	CTBarButton *pTBB = _GetButtonPtr(nToolTipHit);
	if(!pTBB)
		return -1;

	nToolTipHit = pTBB->OnToolHitTest( point, pTI);
	return nToolTipHit;
}

int CTToolControlBar::_HitTestImpl( CPoint point,
								    UINT nButtonStyleInclude,
									UINT nButtonStyleExclude) const
{
	int nCount = _GetButtonsCountImpl();

	for( int i=0; i<nCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);

		if(pTBB->GetStyle()&TBBS_HIDDEN)
			continue;

		if(!pTBB->IsVisible())
			continue;

		if( (pTBB->GetStyle()&nButtonStyleInclude) != nButtonStyleInclude )
			continue;

		if(pTBB->GetStyle()&nButtonStyleExclude)
			continue;

		CRect rect = *pTBB;
		if(rect.PtInRect(point))
			return i;
	}

	return -1;
}

int CTToolControlBar::HitTest( CPoint point) const
{
	return _HitTestImpl(
		point, 0,
		TBBS_SEPARATOR);
}


/////////////////////////////////////////////////////////////////////////////
// CTToolControlBar message handlers

BEGIN_MESSAGE_MAP( CTToolControlBar, CTControlBar)
	//{{AFX_MSG_MAP(CTToolControlBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTToolControlBar::OnLButtonDown( UINT nFlags, CPoint point)
{
	if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CTControlBar::OnLButtonDown( nFlags, point);
		return;
	}

	_ActivateOnClick();
	if( (m_nBtnIdxCapture = HitTest(point)) < 0 )
	{
		CTControlBar::OnLButtonDown( nFlags, point);
		return;
	}

	CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxCapture);
	_UpdateButton(m_nBtnIdxCapture);

	if( pTBB->IsDisabled() ||
		m_nBtnIdxMenuTracking == m_nBtnIdxCapture )
	{
		m_nBtnIdxCapture = -1;
		return;
	}

	pTBB->ModifyStyle(TBBS_PRESSED);
	_InvalidateButton(m_nBtnIdxCapture);
	UpdateWindow();

	CWnd *pCtrlTBB = pTBB->CtrlGet();
	pTBB->OnClick( point, TRUE);

	if(!m_bMenuTracking)
	{
		if( pCtrlTBB == NULL || !(pCtrlTBB->GetStyle()&WS_VISIBLE) )
			SetCapture();
		else
			m_nBtnIdxCapture = -1;
	}
	else
		m_nBtnIdxCapture = -1;
}

void CTToolControlBar::OnMouseMove( UINT nFlags, CPoint point)
{
	if( CTPopupMenuWnd::IsMenuTracking() &&
		!CTToolControlBar::m_bMenuTracking )
		return;

	if( IsFloating() && CTMiniDockFrameWnd::m_bAutoMakeTopmostOnMouseEvent )
	{
		CFrameWnd *pParentFrame = GetParentFrame();

		if(pParentFrame)
			pParentFrame->BringWindowToTop();
	}

	if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CTControlBar::OnMouseMove( nFlags, point);
		return;
	}

	if( m_nBtnIdxCapture >= 0 )
	{
		CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxCapture);
		UINT nNewStyle = pTBB->GetStyle()&~TBBS_PRESSED;
		int nBtnIdxCapture = m_nBtnIdxCapture;

		if( GetCapture() != this )
			m_nBtnIdxCapture = -1;
		else if( HitTest(point) == m_nBtnIdxCapture )
			nNewStyle |= TBBS_PRESSED;

		SetButtonStyle( nBtnIdxCapture, nNewStyle);
		UpdateWindow();

		return;
	}

	BOOL bHoverChanged = _UpdateHoverButton(point);
	if( GetCapture() == this &&
		m_nBtnIdxHover < 0 &&
		m_nBtnIdxMenuTracking < 0 &&
		m_nBtnIdxCapture < 0 )
	{
		CPoint ptScreen;
		::GetCursorPos(&ptScreen);

		HWND hWnd = ::WindowFromPoint(ptScreen);
		if( hWnd != m_hWnd )
			ReleaseCapture();
	}

	if(bHoverChanged)
		return;

	CTControlBar::OnMouseMove( nFlags, point);
}

BOOL CTToolControlBar::_UpdateHoverButton( CPoint point)
{
	if( point.x < 0 || point.y < 0 )
	{
		::GetCursorPos(&point);
		ScreenToClient(&point);
	}

	BOOL bHoverChanged = FALSE;
	int nBtnIdxHoverNew = HitTest(point);
	int nBtnIdxHoverOld = m_nBtnIdxHover;

	if( nBtnIdxHoverOld != nBtnIdxHoverNew )
	{
		bHoverChanged = TRUE;
		m_nBtnIdxHover = -1;

		if( nBtnIdxHoverOld >= 0 )
		{
			CTBarButton *pTBB = _GetButtonPtr(nBtnIdxHoverOld);

			pTBB->SetHover(FALSE);
			_InvalidateButton(nBtnIdxHoverOld);
			UpdateWindow();

			pTBB->OnHover( point, FALSE);
		}

		m_nBtnIdxHover = nBtnIdxHoverNew;
		if( m_nBtnIdxHover >= 0 )
		{
			CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxHover);
			BOOL bEnableHoverOnNewBtn = TRUE;

			if( pTBB->IsKindOf(RUNTIME_CLASS(CTBarMdiRightButton)) || pTBB->IsDisabled() )
			{
				if(m_bMenuTracking)
					bEnableHoverOnNewBtn = FALSE;
			}

			if(bEnableHoverOnNewBtn)
			{
				pTBB->SetHover();
				_InvalidateButton(m_nBtnIdxHover);
				UpdateWindow();

				pTBB->OnHover( point, TRUE);
			}
		}
	}

	return bHoverChanged;
}

void CTToolControlBar::OnLButtonUp( UINT nFlags, CPoint point)
{
	if((m_nBtnIdxCapture < 0 &&
		m_nBtnIdxHover < 0 ) ||
		m_bRowResizing ||
		m_bRowRecalcing ||
		m_bDragging )
	{
		CTControlBar::OnLButtonUp( nFlags, point);
		return;
	}

	int nBtnIdxHover = m_nBtnIdxHover;
	int nBtnIdxCapture = m_nBtnIdxCapture;

	CTBarButton *pHoverTBB = NULL;
	if( m_nBtnIdxHover >= 0 )
	{
		pHoverTBB = _GetButtonPtr(m_nBtnIdxHover);
		m_nBtnIdxHover = -1;
	}

	if( m_nBtnIdxCapture >= 0 )
	{
		m_nBtnIdxCapture = -1;

		CTBarButton *pCaptureTBB = NULL;
		UINT nNewStyle = 0;

		if( nBtnIdxCapture == HitTest(point) )
		{
			pCaptureTBB = _GetButtonPtr(nBtnIdxCapture);
			nNewStyle = pCaptureTBB->GetStyle()&~TBBS_PRESSED;
			_UpdateButton(nBtnIdxCapture);

			if(!(pCaptureTBB->IsDisabled()))
			{
				if(pCaptureTBB->GetStyle()&TBBS_CHECKBOX)
				{
					if(nNewStyle&TBBS_INDETERMINATE)
						nNewStyle &= ~TBBS_INDETERMINATE;

					nNewStyle ^= TBBS_CHECKED;
				}
			}
		}

		ReleaseCapture();
		if(pCaptureTBB)
			pCaptureTBB->OnClick( point, FALSE);

		if(!::IsWindow(GetSafeHwnd()))
			return;

		if(!m_bMenuTracking)
		{
			SetButtonStyle( nBtnIdxCapture, nNewStyle);
			_UpdateButton(nBtnIdxCapture);
		}
	}

	if(pHoverTBB)
	{
		pHoverTBB->SetHover(FALSE);
		pHoverTBB->OnHover( point, FALSE);
		_InvalidateButton(nBtnIdxHover);
	}

	UpdateWindow();
}

void CTToolControlBar::OnLButtonDblClk( UINT nFlags, CPoint point)
{
	if( m_bRowResizing || m_bRowRecalcing || m_bDragging )
	{
		CTControlBar::OnLButtonDblClk( nFlags, point);
		return;
	}

	int nBtnIdx = _HitTestImpl(point);
	if( nBtnIdx >= 0 )
		return;

	CTControlBar::OnLButtonDblClk( nFlags, point);
}

void CTToolControlBar::OnCancelMode()
{
	CTControlBar::OnCancelMode();

	BOOL bUpdateState = FALSE;
	if( m_nBtnIdxCapture >= 0 )
	{
		CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxCapture);

		if( m_nBtnIdxMenuTracking != m_nBtnIdxCapture )
		{
			UINT nNewStyle = pTBB->GetStyle()&~TBBS_PRESSED;
			SetButtonStyle( m_nBtnIdxCapture, nNewStyle);
		}

		m_nBtnIdxCapture = -1;
		bUpdateState = TRUE;
	}

	if( m_nBtnIdxHover >= 0 )
	{
		CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxHover);

		pTBB->SetHover(FALSE);
		m_nBtnIdxHover = -1;
		bUpdateState = TRUE;

		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);
		pTBB->OnHover( point, FALSE);
	}

	if(bUpdateState)
	{
		if( GetCapture() == this )
			ReleaseCapture();

		Invalidate();
		UpdateWindow();
	}
}

void CTToolControlBar::OnSettingChange( UINT uFlags, LPCTSTR lpszSection)
{
	CTControlBar::OnSettingChange( uFlags, lpszSection);

	if(m_bPresubclassDialogMode)
	{
		_RecalcLayoutImpl();
		return;
	}

	CFrameWnd *pFrame = GetParentFrame();
	pFrame->DelayRecalcLayout();
}

BOOL CTToolControlBar::IsRightExpandButton( int nBtnIdx)
{
	CTBarButton *pTBB = _GetButtonPtr(nBtnIdx);

	if(!pTBB)
		return FALSE;

	if(pTBB->IsKindOf(RUNTIME_CLASS(CTBarContentExpandButton)))
		return TRUE;

	return FALSE;
}

class CTToolControlBar::CTToolControlBarCmdUI : public CCmdUI
{
	void _SetCheckImpl(
		int nCheck,
		BOOL bUpdaeteInCmdManager);

public:
	virtual void SetRadio( BOOL bOn);
	virtual void Enable( BOOL bOn);

	virtual void SetCheck( int nCheck)
	{
		_SetCheckImpl( nCheck, TRUE);
	};

	virtual void SetText( LPCTSTR lpszText)
	{
	};
};

void CTToolControlBar::CTToolControlBarCmdUI::_SetCheckImpl( int nCheck,
															 BOOL bUpdaeteInCmdManager)
{
	CTToolControlBar *pToolBar = (CTToolControlBar *) m_pOther;

	if(pToolBar->IsRightExpandButton(m_nIndex))
		return;

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex)&~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if( nCheck == 1 )
		nNewStyle |= TBBS_CHECKED;
	else if( nCheck == 2 )
		nNewStyle |= TBBS_INDETERMINATE;

	pToolBar->SetButtonStyle( m_nIndex, nNewStyle|TBBS_CHECKBOX);
	if(bUpdaeteInCmdManager)
	{
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(pToolBar->GetSafeHwnd()),
			pToolBar->GetButtonID(m_nIndex));

		if(p_cmd)
			p_cmd->StateSetCheck(nNewStyle&(TBBS_CHECKED|TBBS_INDETERMINATE));
	}
}

void CTToolControlBar::CTToolControlBarCmdUI::Enable( BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CTToolControlBar *pToolBar = (CTToolControlBar *) m_pOther;

	if(pToolBar->IsRightExpandButton(m_nIndex))
		return;

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex)&~TBBS_DISABLED;
	if(!bOn)
		nNewStyle |= TBBS_DISABLED;

	pToolBar->SetButtonStyle( m_nIndex, nNewStyle);
	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(pToolBar->GetSafeHwnd()),
		pToolBar->GetButtonID(m_nIndex));

	if(p_cmd)
		p_cmd->StateEnable(bOn);
}

void CTToolControlBar::CTToolControlBarCmdUI::SetRadio( BOOL bOn)
{
	_SetCheckImpl( bOn ? 1 : 0, FALSE);

	CTToolControlBar *pToolBar = (CTToolControlBar *) m_pOther;
	if(pToolBar->IsRightExpandButton(m_nIndex))
		return;

	CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
		g_CmdManager->ProfileNameFromWnd(pToolBar->GetSafeHwnd()),
		pToolBar->GetButtonID(m_nIndex));

	if(p_cmd)
		p_cmd->StateSetRadio(bOn);
}

void CTToolControlBar::OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
	CWnd *pTargetUpdateWnd = pTarget;

	if(!m_bPresubclassDialogMode)
	{
		CTControlBar::OnUpdateCmdUI( pTarget, bDisableIfNoHndler);

		if(_DraggingGetBar())
			return;
	}
	else
	{
		if( pTargetUpdateWnd && pTargetUpdateWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
			return;

		pTargetUpdateWnd = GetOwner();
		if(!pTargetUpdateWnd)
			pTargetUpdateWnd = GetParent();
	}

	if(!pTargetUpdateWnd)
		return;

	if(!(GetStyle()&WS_VISIBLE))
		return;

	CTToolControlBarCmdUI state;
	state.m_pOther = this;

	int nCountOfButtons = _GetButtonsCountImpl();
	state.m_nIndexMax = (UINT) nCountOfButtons;

	for( state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		CTBarButton *pTBB = _GetButtonPtr(state.m_nIndex);

		if(pTBB->IsAbleToTrackMenu())
			continue;

		state.m_nID = pTBB->GetCmdID();
		if(pTBB->IsSeparator())
			continue;

		state.DoUpdate( pTargetUpdateWnd, bDisableIfNoHndler);
	}

	UpdateDialogControls( pTargetUpdateWnd, bDisableIfNoHndler);
}

void CTToolControlBar::_UpdateButton( int nIndex)
{
	CWnd *pTarget = GetOwner();

	if( !pTarget && !m_bPresubclassDialogMode )
		pTarget = GetParentFrame();

	if(!pTarget)
		return;

	BOOL bDisableIfNoHandler = TRUE;
	if(pTarget->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		bDisableIfNoHandler = ((CFrameWnd *) pTarget)->m_bAutoMenuEnable;

	CTBarButton *pTBB = _GetButtonPtr(nIndex);
	if(pTBB->IsSeparator())
		return;

	if(pTBB->IsAbleToTrackMenu())
		return;

	CTToolControlBarCmdUI state;
	state.m_pOther = this;
	state.m_nIndex = nIndex;

	int nCountOfButtons = _GetButtonsCountImpl();
	state.m_nIndexMax = (UINT) nCountOfButtons;
	state.m_nID = pTBB->GetCmdID();
	state.DoUpdate( pTarget, bDisableIfNoHandler);
}


/////////////////////////////////////////////////////////////////////////////
// CTToolControlBar diagnostics

void CTToolControlBar::_CalcInsideRect( CRect& rect, BOOL bHorz) const
{
	CControlBar::CalcInsideRect( rect,bHorz);
}

void CTToolControlBar::_RecalcPositionsImpl()
{
	if( GetSafeHwnd() == NULL ||
		!::IsWindow(GetSafeHwnd()))
		return;

	int nCountOfButtons = _GetButtonsCountImpl();
	if( nCountOfButtons == 0 )
		return;

	BOOL bHorz = m_dwStyle&CBRS_ORIENT_HORZ;
	BOOL bFloating = IsFloating();
	CRect rcInner;

	GetClientRect(&rcInner);
	rcInner.DeflateRect(
		m_cxLeftBorder,
		m_cyTopBorder,
		m_cxRightBorder,
		m_cyBottomBorder);

	CPoint ptLimitTL = rcInner.TopLeft();
	CPoint ptLimitBR = rcInner.BottomRight();

	CClientDC dc(this);
	CFont *pOldFont = (CFont *) dc.SelectObject(bHorz ? &g_PaintManager->m_FontNormal : &g_PaintManager->m_FontNormalVert);
	int nReviewCount = nCountOfButtons;

	CSize szTBBRight( 0, 0);
	if(m_pRightBtn)
	{
		nReviewCount--;
		m_pRightBtn->GetButtons().RemoveAll();
		m_pRightBtn->ClearContent();

		szTBBRight = m_pRightBtn->CalculateLayout( dc, _GetDefButtonSize(), bHorz);
		if(bFloating)
		{
			m_pRightBtn->Show(FALSE);
			m_pRightBtn->SetRect(CRect( ptLimitTL, szTBBRight));

			if(!nReviewCount)
				return;
		}
		else
		{
			m_pRightBtn->SetRect(CRect( ptLimitBR - szTBBRight, szTBBRight));
			m_pRightBtn->Show(TRUE);

			if(bHorz)
				rcInner.right -= szTBBRight.cx + __TB_BUTTON_MARGINE__;
			else
				rcInner.bottom -= szTBBRight.cy + __TB_BUTTON_MARGINE__;
		}
	}

	CArray< CRect, CRect> arrBtnRects;
	CArray< BOOL, BOOL> arrBtnVisibility;
	CArray< BOOL, BOOL> arrBtnSeparators;

	arrBtnRects.SetSize(nReviewCount);
	arrBtnVisibility.SetSize(nReviewCount);
	arrBtnSeparators.SetSize(nReviewCount);

	CSize szLastWrappedRow( 0, 0);
	CPoint ptBtnPosCurr(ptLimitTL);
	BOOL bSeparatorPrev = FALSE;

	for( int i=0; i<nReviewCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);
		BOOL bVisibleTBB = !(pTBB->GetStyle()&TBBS_HIDDEN);

		arrBtnVisibility.SetAt( i, bVisibleTBB);
		if(!bVisibleTBB)
			continue;

		pTBB->SetVertDocked(!bHorz);
		BOOL bSeparator = pTBB->IsSeparator();

		arrBtnSeparators.SetAt( i, bSeparator);
		if( (bSeparatorPrev && bSeparator) || (bSeparator && i == 0) )
		{
			arrBtnVisibility.SetAt( i, FALSE);
			continue;
		}

		CSize szTBB = pTBB->CalculateLayout( dc, _GetDefButtonSize(), bHorz);
		CRect rcTBB( ptBtnPosCurr, szTBB);

		if( bFloating && pTBB->IsWrap() )
		{
			ptBtnPosCurr.x = ptLimitTL.x;
			ptBtnPosCurr.y += _GetDefButtonSize().cy + __TB_LINE_OFFSET;
			szLastWrappedRow = CSize( 0, 0);
		}
		else
		{
			BOOL bResyncVisibleTBB = FALSE;

			if(bHorz)
			{
				ptBtnPosCurr.x += szTBB.cx;

				if( ptBtnPosCurr.x > rcInner.right )
					bResyncVisibleTBB = TRUE;
			}
			else
			{
				ptBtnPosCurr.y += szTBB.cy;

				if( ptBtnPosCurr.y > rcInner.bottom )
					bResyncVisibleTBB = TRUE;
			}

			if(bResyncVisibleTBB)
			{
				int nSpaceToFind = bHorz ? ptBtnPosCurr.x - rcInner.right : ptBtnPosCurr.y - rcInner.bottom;
				int nBtnWithNearestWidth = i;
				int nNearestMetric = bHorz ? rcTBB.Width() : rcTBB.Height();
				int nNearestDiff = nSpaceToFind - nNearestMetric;

				for( int j = i - 1; j>=0; j--)
				{
					if( !arrBtnVisibility[j] ||
						arrBtnSeparators[j])
						continue;

					int nMetric = bHorz ? arrBtnRects[j].Width() : arrBtnRects[j].Height();
					if( nMetric > nSpaceToFind )
						continue;

					int nDiff = nSpaceToFind - nMetric;
					if( nDiff >= nNearestDiff )
						continue;

					nNearestDiff = nDiff;
					nNearestMetric = nMetric;
					nBtnWithNearestWidth = j;
				}

				if( nBtnWithNearestWidth < i )
				{
					arrBtnVisibility.SetAt( nBtnWithNearestWidth, FALSE);

					if( m_pRightBtn && !arrBtnSeparators[nBtnWithNearestWidth] )
					{
						CTBarButton *pTBB2 = _GetButtonPtr(nBtnWithNearestWidth);
						m_pRightBtn->GetButtons().Add(pTBB2);
					}

					if( nBtnWithNearestWidth > 0 && nBtnWithNearestWidth < nReviewCount - 1 )
					{
						if( arrBtnSeparators[nBtnWithNearestWidth - 1] && arrBtnSeparators[nBtnWithNearestWidth + 1] )
						{
							arrBtnVisibility.SetAt( nBtnWithNearestWidth - 1, FALSE);
							nNearestMetric += bHorz ? arrBtnRects[nBtnWithNearestWidth - 1].Width() : arrBtnRects[nBtnWithNearestWidth - 1].Height();
						}
					}

					rcTBB.OffsetRect(
						bHorz ? -nNearestMetric : 0,
						bHorz ? 0 : -nNearestMetric);

					(bHorz ? ptBtnPosCurr.x : ptBtnPosCurr.y) -= nNearestMetric;
					for( j = nBtnWithNearestWidth + 1; j<i; j++)
					{
						if(!arrBtnVisibility[j])
							continue;

						CRect rcTBB2 = arrBtnRects[j];
						rcTBB2.OffsetRect(
							bHorz ? -nNearestMetric : 0,
							bHorz ? 0 : -nNearestMetric);

						arrBtnRects.SetAt( j, rcTBB2);
					}
				}
				else
				{
					bVisibleTBB = FALSE;
					(bHorz ? ptBtnPosCurr.x : ptBtnPosCurr.y) -= (bHorz ? szTBB.cx : szTBB.cy);
				}
			}
		}

		arrBtnVisibility.SetAt( i, bVisibleTBB);
		arrBtnRects.SetAt( i, rcTBB);

		if(bVisibleTBB)
			bSeparatorPrev = bSeparator;

		if( m_pRightBtn && !bVisibleTBB && !bSeparator )
			m_pRightBtn->GetButtons().Add(pTBB);
	}

	dc.SelectObject(pOldFont);
	int nRowStart = 0;
	int nSizeRow = 0;

	for( i=0; i<nReviewCount; i++)
	{
		CTBarButton *pTBB = _GetButtonPtr(i);
		CSize szTBB = *pTBB;

		int nSizeBtn = bHorz || bFloating ? szTBB.cy : szTBB.cx;
		nSizeRow = max( nSizeRow, nSizeBtn);

		if( (bFloating && pTBB->IsWrap()) || i == nReviewCount - 1 )
		{
			for( int j = nRowStart; j<=i; j++)
			{
				CTBarButton *pTBB = _GetButtonPtr(j);
				BOOL bVis = arrBtnVisibility[j];

				pTBB->Show(bVis);
				if(!bVis)
					continue;

				CRect rcBtn = arrBtnRects[j];
				if(pTBB->IsSeparator())
				{
					if( bHorz || bFloating )
						rcBtn.bottom = rcBtn.top + nSizeRow;
					else
						rcBtn.right = rcBtn.left + nSizeRow;
				}
				else
				{
					rcBtn.OffsetRect(
						bHorz || bFloating ? 0 : (nSizeRow - rcBtn.Width()) / 2,
						bHorz || bFloating ? (nSizeRow - rcBtn.Height()) / 2 : 0);
				}

				pTBB->SetRect(rcBtn);
			}

			nRowStart = i + 1;
			nSizeRow = 0;
		}
	}

	if(	m_pRightBtn && m_bPresubclassDialogMode )
	{
		if(!m_pRightBtn->GetButtons().GetSize())
			m_pRightBtn->ModifyStyle( TBBS_DISABLED, 0);
		else
			m_pRightBtn->ModifyStyle( 0, TBBS_DISABLED);
	}

	if( bFloating || !m_pRightBtn || (
		m_pRightBtn && !m_pRightBtn->GetButtons().GetSize()))
		return;

	int nHiddenCount = m_pRightBtn->GetButtons().GetSize();
	if( nHiddenCount < 2 )
		return;

	int nEndMeasure = nHiddenCount - 1;
	for( int nHidden = 0; nHidden < nEndMeasure; nHidden++)
	{
		CTBarButton *pTbbHidden0 = m_pRightBtn->GetButtons().GetAt(nHidden);
		CTBarButton *pTbbHidden1 = m_pRightBtn->GetButtons().GetAt(nHidden + 1);
		CTBarButton *pTbbSeparatorToInsert = NULL;
		int nIdx0 = -1;
		int nIdx1 = -1;

		for( i=0; i<nCountOfButtons; i++)
		{
			CTBarButton *pTBB = _GetButtonPtr(i);
			if( pTBB == pTbbHidden0 )
			{
				nIdx0 = i;
				continue;
			}

			if( nIdx0 >= 0 && pTBB->IsSeparator() && !pTbbSeparatorToInsert )
				pTbbSeparatorToInsert = pTBB;

			if( pTBB == pTbbHidden1 )
			{
				nIdx1 = i;
				break;
			}
		}

		if( nIdx0 + 1 == nIdx1 )
			continue;

		if(pTbbSeparatorToInsert)
		{
			nEndMeasure++;
			nHidden++;

			m_pRightBtn->GetButtons().InsertAt(
				nHidden,
				pTbbSeparatorToInsert);
		}
	}
}

DWORD CTToolControlBar::RecalcDelayShow( AFX_SIZEPARENTPARAMS *lpLayout)
{
	DWORD dwRes = CControlBar::RecalcDelayShow(lpLayout);

	if(!IsFloating())
		_RecalcPositionsImpl();

	return dwRes;
}

void CTToolControlBar::OnSize( UINT nType, int cx, int cy)
{
	CTControlBar::OnSize( nType, cx, cy);
	_RecalcLayoutImpl();
}

void CTToolControlBar::_RecalcLayoutImpl()
{
	if( !GetSafeHwnd() || !::IsWindow(GetSafeHwnd()) )
		return;

	CTControlBar::_RecalcLayoutImpl();
	_RecalcPositionsImpl();

	RedrawWindow(
		NULL,
		NULL,
		RDW_INVALIDATE|
		RDW_UPDATENOW|
		RDW_ERASE|
		RDW_ALLCHILDREN);
}

void CTToolControlBar::OnWindowPosChanging( WINDOWPOS FAR *lpwndpos)
{
	CTControlBar::OnWindowPosChanging(lpwndpos);
}

void CTToolControlBar::OnTimer( UINT nIDEvent)
{
	CTControlBar::OnTimer(nIDEvent);
}

void CTToolControlBar::_SwitchMenuTrackingIndex( int nNewMenuTrackingIndex)
{
	if( m_nBtnIdxMenuTracking >= 0 )
	{
		CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxMenuTracking);
		UINT nNewStyle = pTBB->GetStyle()&~(TBBS_PRESSED|TBBS_CHECKED|TBBS_INDETERMINATE);

		pTBB->SetStyle(nNewStyle);
		_InvalidateButton(m_nBtnIdxMenuTracking);
		m_nBtnIdxMenuTracking = -1;
	}

	if( nNewMenuTrackingIndex >= 0 )
	{
		m_nBtnIdxMenuTracking = nNewMenuTrackingIndex;
		CTBarButton *pTBB = _GetButtonPtr(m_nBtnIdxMenuTracking);
		UINT nNewStyle = pTBB->GetStyle()&~(TBBS_CHECKED|TBBS_INDETERMINATE);

		nNewStyle |= TBBS_PRESSED;
		pTBB->SetStyle(nNewStyle);
		_InvalidateButton(m_nBtnIdxMenuTracking);
	}

	UpdateWindow();
}

CTToolControlBar *CTToolControlBar::_GetMenuTrackingBar()
{
	for( int i=0; i<m_AllBars.GetSize(); i++)
	{
		CTControlBar *pBar = m_AllBars[i];
		CTToolControlBar *pToolControlBar = DYNAMIC_DOWNCAST(
			CTToolControlBar,
			pBar);

		if(!pToolControlBar)
			continue;

		if( pToolControlBar->m_nBtnIdxMenuTracking >= 0 )
			return pToolControlBar;
	}

	return NULL;
}

void CTToolControlBar::_CloseTrackingMenus()
{
	m_bMenuTracking = FALSE;

	for( int i=0; i<m_AllBars.GetSize(); i++)
	{
		CTControlBar *pBar = m_AllBars[i];
		CTToolControlBar *pToolControlBar = DYNAMIC_DOWNCAST(
			CTToolControlBar,
			pBar);

		if(!pToolControlBar)
			continue;

		pToolControlBar->_SwitchMenuTrackingIndex();
	}

	CTPopupMenuWnd::CancelMenuTracking();
}

void CTToolControlBar::_CbPaintCombinedContent( LPVOID pCookie,
											    CDC &dc,
												const CWnd &refWndMenu,
												const CRect &rcExcludeArea,
												int nCombineAlign)
{
	CTToolControlBar *pBar = (CTToolControlBar *) pCookie;

	if(rcExcludeArea.IsRectEmpty())
		return;

	if( pBar->m_nBtnIdxMenuTracking < 0 )
		return;

	CTBarButton *pTBB = pBar->_GetButtonPtr(pBar->m_nBtnIdxMenuTracking);
	UINT nStyle = pTBB->GetStyle();
	BOOL bHover = pTBB->IsHover();

	pTBB->SetHover(FALSE);
	pTBB->ModifyStyle( 0, TBBS_PRESSED|TBBS_CHECKED|TBBS_CHECKBOX);

	CRect rcClientExcludeArea;
	CRect rcClientBar;

	pBar->GetClientRect(&rcClientBar);
	pBar->ClientToScreen(&rcClientBar);

	refWndMenu.GetClientRect(&rcClientExcludeArea);
	refWndMenu.ClientToScreen(&rcClientExcludeArea);

	CPoint ptOffset = rcClientBar.TopLeft() - rcClientExcludeArea.TopLeft();
	CPoint ptViewportOrg = dc.GetViewportOrg();
	dc.SetViewportOrg(ptOffset);

	BOOL bHorz = !pBar->IsDockedVertically();
	pTBB->Paint( dc, bHorz);
	dc.SetViewportOrg(ptViewportOrg);
	pTBB->SetHover(bHover);
	pTBB->SetStyle(nStyle);
}

CTBarButton *CTToolControlBar::GetButton( int nIndex)
{
	return _GetButtonPtr(nIndex);
}

CTBarContentExpandButton *CTToolControlBar::GetRightButton()
{
	return m_pRightBtn;
}

void CTToolControlBar::OnCaptureChanged( CWnd *pWnd)
{
	if( pWnd != this && ( m_nBtnIdxCapture >= 0 || m_nBtnIdxHover >= 0 ))
		SendMessage(WM_CANCELMODE);

	CTControlBar::OnCaptureChanged(pWnd);
}

BOOL CTToolControlBar::InitContentExpandButton()
{
	if(m_pRightBtn)
		return TRUE;

	m_pRightBtn = OnCreateBarRightBtn();
	if(!m_pRightBtn)
		return FALSE;

	m_buttons.Add(m_pRightBtn);
	return TRUE;
}

BOOL CTToolControlBar::_CanDockToInnerCircles() const
{
	return FALSE;
}

BOOL CTToolControlBar::_CanDockToTabbedContainers() const
{
	return FALSE;
}

void CTToolControlBar::ToggleDocking()
{
	m_pDockContext->ToggleDocking();
	CFrameWnd *pFrame = _GetDockingFrameImpl();

	pFrame->DelayRecalcLayout();
	_RecalcNcArea();
}
