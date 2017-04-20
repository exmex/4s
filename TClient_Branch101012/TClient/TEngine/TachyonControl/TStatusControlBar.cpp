#include "stdafx.h"
#include "TStatusControlBar.h"
#include "TDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTStatusControlBar

IMPLEMENT_DYNAMIC( CTStatusControlBar, CStatusBar)

CTStatusControlBar::CTStatusControlBar() : m_bOuterRectInFirstBand(FALSE)
{
	m_clrStatusBk = (COLORREF) -1;
}

CTStatusControlBar::~CTStatusControlBar()
{
}

BEGIN_MESSAGE_MAP(CTStatusControlBar, CStatusBar)
	//{{AFX_MSG_MAP(CTStatusControlBar)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTStatusControlBar message handlers

void CTStatusControlBar::OnPaint()
{
	_SyncStatusBarColors();
	Default();

	CClientDC dc(this);
	dc.SelectStockObject(NULL_BRUSH);
	int nItem = m_bOuterRectInFirstBand ? 0 : 1;

	for(; nItem < m_nCount; nItem++)
	{
		CRect rect;

		GetItemRect( nItem, rect);
		g_PaintManager->PaintControlBarBorders(
			CTPaintManager::__CB_INNER_STATUSBAR_ITEM,
			0, dc,
			rect);
	}
}

void CTStatusControlBar::OnNcPaint()
{
	_SyncStatusBarColors();
	CWindowDC dc(this);
	CRect rect;

	GetWindowRect(&rect);
	ScreenToClient(&rect);

	g_PaintManager->PaintControlBarBorders(
		CTPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		dc, rect);
}

void CTStatusControlBar::DrawBorders( CDC *pDC, CRect &rect)
{
	g_PaintManager->PaintControlBarBorders(
		CTPaintManager::__CB_OUTER_STATUSBAR,
		m_dwStyle,
		*pDC,
		rect);
}

void CTStatusControlBar::_SyncStatusBarColors()
{
	COLORREF clrStatusBkNeeded = g_PaintManager->GetColor(CTPaintManager::CLR_3DFACE_OUT);

	if( m_clrStatusBk != clrStatusBkNeeded )
	{
		m_clrStatusBk = clrStatusBkNeeded;
		GetStatusBarCtrl().SetBkColor(m_clrStatusBk);
	}
}

void CTStatusControlBar::OnSysColorChange()
{
	CStatusBar::OnSysColorChange();

	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
	_SyncStatusBarColors();
}

void CTStatusControlBar::OnSettingChange( UINT uFlags, LPCTSTR lpszSection)
{
	CStatusBar::OnSettingChange( uFlags, lpszSection);

	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
	_SyncStatusBarColors();
}

int CTStatusControlBar::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if( CStatusBar::OnCreate(lpCreateStruct) == -1 )
		return -1;

	HWND hWndParent = ::GetParent(GetSafeHwnd());
	::SetWindowPos(
		GetSafeHwnd(),
		HWND_TOP,
		0, 0,
		0, 0,
		SWP_NOSIZE|
		SWP_NOMOVE|
		SWP_NOREDRAW|
		SWP_NOACTIVATE|
		SWP_NOCOPYBITS|
		SWP_NOSENDCHANGING);
	_SyncStatusBarColors();

	return 0;
}

void CTStatusControlBar::OnContextMenu( CWnd *pWnd, CPoint point)
{
	if( _GetDockingFrameImpl() == NULL )
	{
		CStatusBar::OnContextMenu( pWnd, point);
		return;
	}

	_ContextMenuTrack();
}

CFrameWnd *CTStatusControlBar::_GetDockingFrameImpl()
{
	CFrameWnd *pFrame = GetParentFrame();

	if( pFrame == NULL )
		return NULL;

	if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		pFrame = pFrame->GetParentFrame();

	return pFrame;
}

void CTStatusControlBar::_ContextMenuTrack()
{
	CFrameWnd *pFrame = _GetDockingFrameImpl();
	CTDockBar *pDockBar = NULL;

	if( m_pDockBar != NULL )
		pDockBar = DYNAMIC_DOWNCAST( CTDockBar, m_pDockBar);

	if( pDockBar == NULL )
	{
		POSITION pos = pFrame->m_listControlBars.GetHeadPosition();

		while(pos)
		{
			CDockBar *pTempDockBar = (CDockBar *) pFrame->m_listControlBars.GetNext(pos);

			if(!pTempDockBar->IsDockBar())
				continue;

			pDockBar = DYNAMIC_DOWNCAST(
				CTDockBar,
				pTempDockBar);

			if( pDockBar != NULL )
				break;
		}
	}

	if( pDockBar == NULL )
		return;

	CPoint point;
	::GetCursorPos(&point);

	HWND hWndTrack = GetOwner()->GetSafeHwnd();
	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();

	pPopup->CreatePopupMenu(hWndTrack);
	pDockBar->_ContextMenuBuild(
		pFrame,
		pPopup);

	::SetFocus(hWndTrack);
	pPopup->TrackPopupMenu(
		TPMX_OWNERDRAW_FIXED,
		point.x,
		point.y);
}
