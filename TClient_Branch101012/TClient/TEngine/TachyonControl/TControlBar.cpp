#include "StdAfx.h"
#include "TMiniDockFrameWnd.h"
#include "TControlbar.h"
#include "TDockBar.h"


#define __DOCKSITE_DCX_FLAGS			(DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE|DCX_CLIPSIBLINGS)

#define __BAR_NC_GAP_X					1
#define __BAR_NC_GAP_Y					1
#define __DOCKING_SIDE_GAP				10

#define ID_TIMER_WAIT					0xE000
#define ID_TIMER_CHECK					0xE001

static HCURSOR g_hCursorDrag = ::LoadCursor( NULL, IDC_SIZEALL);
static HCURSOR g_hCursorResizeV = ::LoadCursor( NULL, IDC_SIZENS);
static HCURSOR g_hCursorResizeH = ::LoadCursor( NULL, IDC_SIZEWE);
static HCURSOR g_hCursorArrow = ::LoadCursor( NULL, IDC_ARROW);


/////////////////////////////////////////////////////////////////////////////
// CTControlBar

IMPLEMENT_DYNAMIC( CTControlBar, CControlBar)

CTControlBar::DraggingState		CTControlBar::m_DragStateOld;
CTControlBar::DraggingState		CTControlBar::m_DragStateNew;

CTControlBar::DockBarInfo		CTControlBar::m_DockBarInfo;
ControlBarVector				CTControlBar::m_AllBars;

const UINT CTControlBar::m_nMsgQueryIcon = ::RegisterWindowMessage(_T("CTControlBar::m_nMsgQueryIcon"));
BOOL CTControlBar::m_bControlBarClassRegistered = FALSE;
BOOL CTControlBar::m_bEnableOnIdleCalls = FALSE;
BOOL CTControlBar::m_bUpdatingDragState = FALSE;


CTControlBar::DockBarInfo::DockBarInfo()
{
	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_TOP,
		CBRS_TOP);

	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_BOTTOM,
		CBRS_BOTTOM);

	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_LEFT,
		CBRS_LEFT);

	m_mapDockBarInfo.SetAt(
		AFX_IDW_DOCKBAR_RIGHT,
		CBRS_RIGHT);
}

DWORD CTControlBar::DockBarInfo::operator[]( UINT nDockBarID)
{
	DWORD dwStyle = 0;

	m_mapDockBarInfo.Lookup(
		nDockBarID,
		dwStyle);

	return dwStyle;
}

BOOL CTControlBar::DockBarInfo::ReplaceFrameDockBar( UINT nDockBarID,
													 DWORD dwDockStyle,
													 CFrameWnd *pFrame)
{
	DWORD dwDockBarStyle = (*this)[nDockBarID];

	if(!(dwDockBarStyle&dwDockStyle&CBRS_ALIGN_ANY))
		return FALSE;

	CDockBar *pDockBar = (CDockBar *) pFrame->GetControlBar(nDockBarID);
	if(pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		return TRUE;

	BOOL bExplicitDelete = !pDockBar->m_bAutoDelete;
	pDockBar->m_pDockSite->RemoveControlBar(pDockBar);
	pDockBar->m_pDockSite = NULL;
	pDockBar->DestroyWindow();

	if(bExplicitDelete)
		delete pDockBar;

	pDockBar = new CTDockBar(0);
	DWORD dwStyle = WS_CHILD|
		WS_VISIBLE|
		WS_CLIPSIBLINGS|
		WS_CLIPCHILDREN|
		dwDockBarStyle;

	if(!pDockBar->Create( pFrame, dwStyle, nDockBarID))
		return FALSE;

	return TRUE;
}

CTControlBar::CTControlBar() : m_bGripperStaticallyAtTop(TRUE),
	m_bAllowContractExpand(TRUE),
	m_bDoNotEraseClientBackground(FALSE),
	m_bPresubclassDialogMode(FALSE),
	m_bReflectParentSizing(TRUE),
	m_bInCustomModeUpdateCmdUI(FALSE),
	m_bFixedMode(FALSE),
	m_nGripHeightAtTop(::GetSystemMetrics(SM_CYSMCAPTION) + 1),
	m_nGripWidthAtLeft(::GetSystemMetrics(SM_CYSMCAPTION) + 1),
	m_nSeparatorHeight(min( 4, ::GetSystemMetrics(SM_CYSIZEFRAME))),
	m_nSeparatorWidth(min( 4, ::GetSystemMetrics(SM_CXSIZEFRAME))),
	m_nMinHW(::GetSystemMetrics(SM_CYSMCAPTION) + 1 + m_nSeparatorHeight),
	m_nMinVH(::GetSystemMetrics(SM_CYSMCAPTION) + 1 + m_nSeparatorHeight),
	m_nTrackerOffset(0),
	m_nMetricOffset(0),
	m_szDockedH( 200, 150),
	m_szDockedV( 150, 200),
	m_szFloated( 200, 200),
	m_bRowResizing(FALSE),
	m_bRowRecalcing(FALSE),
	m_bDragging(FALSE),
	m_bReposSingleChildMode(TRUE),
	m_ptHelperDragOffsetFloated( 0, 0),
	m_ptHelperDragOffsetDockedH( 0, 0),
	m_ptHelperDragOffsetDockedV( 0, 0),
	m_ptFloatHelper( 0, 0),
	m_bWindowActive(FALSE),
	m_bTopRecalcing(FALSE),
	m_bNcAreaBtnTracking(FALSE),
	m_ptDraggingLast( -1, -1),
	m_bUpdatingChain(FALSE),
	m_nDelayedRowUpdateMetric(0)
{
	RegisterControlBarClass();
	m_AllBars.Add(this);
}

CTControlBar::~CTControlBar()
{
	int nCount = m_AllBars.GetSize();

	for( int i=0; i<nCount; i++)
		if( m_AllBars[i] == this )
		{
			m_AllBars.RemoveAt(i);
			break;
		}

	NcButtons_RemoveAll();
}


BEGIN_MESSAGE_MAP( CTControlBar, CControlBar)
	//{{AFX_MSG_MAP(CTControlBar)
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
    ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SETTINGCHANGE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
    ON_MESSAGE( WM_SETTEXT, OnSetText)
	ON_MESSAGE( WM_SIZEPARENT, OnSizeParent)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CTControlBar::Create( LPCTSTR lpszWindowName,
						   CWnd *pParentWnd,
						   UINT nID,
						   DWORD dwStyle)
{
	if(!RegisterControlBarClass())
		return FALSE;

	CString strName( lpszWindowName != NULL ? lpszWindowName : _T(""));
	m_dwStyle = dwStyle & CBRS_ALL;

	if( lpszWindowName == NULL && nID != 0 && nID != UINT(-1) )
	{
		CTCmdManager::cmd *p_cmd = g_CmdManager->CmdGetPtr(
			g_CmdManager->ProfileNameFromWnd(pParentWnd->GetSafeHwnd()),
			nID);

		if( p_cmd != NULL )
		{
			strName = p_cmd->m_strMenuText;
			strName.Replace( _T("&"), _T(""));
		}
		else
		{
			if(!strName.LoadString(nID))
				strName = _T("");
		}
	}

	if(!strName.IsEmpty())
	{
		int nPos = strName.Find(_T('\n'));

		if( nPos >= 0 )
			strName = strName.Left(nPos);
	}

	dwStyle &= ~CBRS_ALL;
	dwStyle |= WS_CLIPCHILDREN;

	CString strClassName(
		::AfxRegisterWndClass(
		CS_DBLCLKS,
		g_hCursorArrow,
		::GetSysColorBrush(COLOR_BTNFACE), 0));

	if(!CWnd::Create(
		LPCTSTR(strClassName),
		strName.IsEmpty() ? _T("") : LPCTSTR(strName),
		dwStyle,
		CRect( 0, 0, 0, 0),
		pParentWnd,
		nID))
		return FALSE;

	return TRUE;
}

BOOL CTControlBar::_SetupFloatingFrameWndImpl( CFrameWnd *pFrame)
{
	return ((CTControlBar::CFriendlyFrameWnd *) pFrame)->SetupSmilyFrameWnd();
}

BOOL CTControlBar::_FrameEnableDockingImpl( CFrameWnd *pFrame,
										    DWORD dwDockStyle,
											BOOL bReplaceFloatingWnd)
{
	pFrame->EnableDocking(dwDockStyle);

	if(!m_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_TOP,
		dwDockStyle,
		pFrame))
		return FALSE;

	if(!m_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_BOTTOM,
		dwDockStyle,
		pFrame))
		return FALSE;

	if(!m_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_LEFT,
		dwDockStyle,
		pFrame))
		return FALSE;

	if(!m_DockBarInfo.ReplaceFrameDockBar(
		AFX_IDW_DOCKBAR_RIGHT,
		dwDockStyle,
		pFrame))
		return FALSE;

	if( bReplaceFloatingWnd && !_SetupFloatingFrameWndImpl(pFrame) )
		return FALSE;

	return TRUE;
}

BOOL CTControlBar::FrameEnableDocking( CFrameWnd *pFrame,
									   DWORD dwDockStyle,
									   BOOL bReplaceFloating)
{
	return _FrameEnableDockingImpl(
		pFrame,
		dwDockStyle,
		bReplaceFloating);
}

BOOL CTControlBar::IsFixedMode() const
{
	return m_bFixedMode;
}

BOOL CTControlBar::CFriendlyFrameWnd::SetupSmilyFrameWnd()
{
	m_pFloatingFrameClass = RUNTIME_CLASS(CTMiniDockFrameWnd);
	return TRUE;
}

void CTControlBar::CFriendlyFrameWnd::SetDockState( const CDockState& state,
												    const CTControlBar::PropsArr& arrProps)
{
	for( int i=0; i<state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo *pInfo = (CControlBarInfo *) state.m_arrBarInfo[i];

		if(pInfo->m_bFloating)
		{
			CMiniDockFrameWnd* pDockFrame = CreateFloatingFrame(pInfo->m_bHorz ? CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT);
			CRect rcWnd( pInfo->m_pointPos, CSize( 10, 10));

			pDockFrame->CalcWindowRect(&rcWnd);
			pDockFrame->SetWindowPos(
				NULL,
				rcWnd.left,
				rcWnd.top,
				0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);

			pInfo->m_pBar = (CDockBar *) pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
		}
		else
			pInfo->m_pBar = GetControlBar(pInfo->m_nBarID);

		if(pInfo->m_pBar)
			pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	}

	for( i=0; i<arrProps.GetSize(); i++)
	{
		ItemData item = arrProps[i];

		CTControlBar *pBar = STATIC_DOWNCAST(
			CTControlBar,
			GetControlBar(item.m_nBarID));

		pBar->m_ptFloatHelper = item.m_ptFloatHelper;
		pBar->m_szDockedH = item.m_szDockedH;
		pBar->m_szDockedV = item.m_szDockedV;
		pBar->m_szFloated = item.m_szFloated;
	}

	for( i=0; i<state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo *pInfo = (CControlBarInfo *) state.m_arrBarInfo[i];

		if(pInfo->m_pBar)
		{
			if( pInfo->m_pBar->m_pDockContext != NULL &&
				pInfo->m_pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
				((CTControlBar *) (pInfo->m_pBar))->m_ptFloatHelper = pInfo->m_ptMRUFloatPos;

			pInfo->m_pBar->SetBarInfo( pInfo, this);
		}
	}

	for( i=0; i<state.m_arrBarInfo.GetSize(); i++)
	{
		CControlBarInfo *pInfo = (CControlBarInfo *) state.m_arrBarInfo[i];

		if( !pInfo->m_pBar ||
			!pInfo->m_bFloating )
			continue;

		CFrameWnd *pFrameWnd = pInfo->m_pBar->GetParentFrame();
		CDockBar *pDockBar = (CDockBar *) pInfo->m_pBar;

		if( pDockBar->GetDockedVisibleCount() > 0 )
		{
			pFrameWnd->RecalcLayout();
			pFrameWnd->ShowWindow(SW_SHOWNA);

			for( int j=1; j<pDockBar->m_arrBars.GetSize(); j++)
			{
				CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[j];

				if( !pBar || __PLACEHODLER_BAR_PTR(pBar) )
					continue;

				CTControlBar *pTBar = DYNAMIC_DOWNCAST(
					CTControlBar,
					pBar);

				if( !pTBar || pTBar->IsFixedMode() )
					continue;

				pTBar->_RecalcNcArea();
			}
		}
	}
}

void CTControlBar::CFriendlyFrameWnd::GetDockState( CDockState& state,
												    CTControlBar::PropsArr& arrProps) const
{
	POSITION pos = m_listControlBars.GetHeadPosition();

	state.Clear();
	arrProps.RemoveAll();

	while(pos)
	{
		CControlBar *pBar = (CControlBar *) m_listControlBars.GetNext(pos);

		if( pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
			continue;

		if( pBar->m_pDockBar != NULL &&
			pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
			continue;

		if(pBar->IsDockBar())
		{
			UINT nID = pBar->GetDlgCtrlID();

			if( nID == AFX_IDW_DOCKBAR_FLOAT )
			{
				CControlBar *pBarChild = STATIC_DOWNCAST(
					CControlBar,
					pBar->GetWindow(GW_CHILD));

				if(pBarChild->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
					continue;
			}

			if( pBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) &&
				((CTDockBar *) pBar)->_GetCircleNo() != 0 )
				continue;
		}

		CSize szReset( -1, -1);

		if(pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
		{
			if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
				continue;

			if((!((CTControlBar *) pBar)->IsFixedMode()) &&
				!pBar->IsFloating())
			{
				if( pBar->m_pDockBar == NULL )
					continue;

				CTDockBar *pDockBar = STATIC_DOWNCAST(
					CTDockBar,
					pBar->m_pDockBar);

				if( pDockBar->_GetCircleNo() > 0 )
					continue;
			}

			ItemData item;

			item.m_nBarID = pBar->GetDlgCtrlID();
			item.m_ptFloatHelper = ((CTControlBar *) pBar)->m_ptFloatHelper;
			item.m_szDockedH = ((CTControlBar *) pBar)->m_szDockedH;
			item.m_szDockedV = ((CTControlBar *) pBar)->m_szDockedV;
			item.m_szFloated = ((CTControlBar *) pBar)->m_szFloated;

			if( pBar->IsFloating() &&
				((CTControlBar *) pBar)->IsFixedMode())
			{
				CRect rcFraweWnd;
				CRect rcBarWnd;

				pBar->GetWindowRect(&rcBarWnd);

				CTMiniDockFrameWnd *pMiniFrame = STATIC_DOWNCAST(
					CTMiniDockFrameWnd,
					pBar->GetParentFrame());
				pMiniFrame->GetWindowRect(&rcFraweWnd);

				CSize szFraweWnd = rcFraweWnd.Size();
				CSize szBarWnd = rcBarWnd.Size();
				CSize szDiff = szFraweWnd - szBarWnd;

				item.m_szFloated += szDiff;
				szReset = item.m_szFloated;
			}

			arrProps.Add(item);
		}

		CControlBarInfo *pInfo = new CControlBarInfo;
		pBar->GetBarInfo(pInfo);

		if(!pInfo->m_pBar)
			continue;

		if( szReset.cx > 0 && szReset.cy > 0 )
			pInfo->m_nMRUWidth = szReset.cx;

		state.m_arrBarInfo.Add(pInfo);
	}
}

void CTControlBar::CFriendlyFrameWnd::SetDockState( const CTControlBar::CDockStateSite& state)
{
	state.StateSet();
}

void CTControlBar::CFriendlyFrameWnd::GetDockState( CTControlBar::CDockStateSite& state) const
{
	state.StateGet();
}

CTControlBar::CDockStateSite::CDockStateSite( CFrameWnd *pDockSite) :
	m_pDockSite(pDockSite),
	m_pState(NULL)
{
	::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT));
	m_pState = new CDockState;
}

CTControlBar::CDockStateSite::~CDockStateSite()
{
	Clear();

	if(m_pState)
		delete m_pState;
}

void CTControlBar::CDockStateSite::Clear()
{
	::memset( &m_wp, 0, sizeof(WINDOWPLACEMENT));
	m_wp.length = sizeof(WINDOWPLACEMENT);

	int nCount = m_arrBarStates.GetSize();
	m_pState->Clear();

	for( int i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrBarStates[i];

		pState->Clear();
		delete pState;
	}

	m_arrBarStates.RemoveAll();
	m_arrProps.RemoveAll();
}

void CTControlBar::CDockStateSite::StateGet()
{
	Clear();
	CTDockBar::_OptimizeCircles(m_pDockSite);
	m_pDockSite->GetWindowPlacement(&m_wp);

	POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CControlBar *pBar = (CControlBar *) m_pDockSite->m_listControlBars.GetNext(pos);

		if(pBar->IsFloating())
		{
			if(pBar->IsDockBar())
				continue;

			if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			{
				CTMiniDockFrameWnd *pFrame = STATIC_DOWNCAST(
					CTMiniDockFrameWnd,
					pBar->GetParentFrame());

				AppendFloatingLayout(
					STATIC_DOWNCAST(
					CTMiniDockFrameWnd,
					pFrame));
			}

			continue;
		}

		if( !pBar->IsDockBar() ||
			!pBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			continue;

		CFrameWnd *pFrame = pBar->GetParentFrame();
		if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
			continue;

		CTDockBar *pDockBar = STATIC_DOWNCAST(
			CTDockBar,
			pBar);

		UINT nCircleNo = pDockBar->_GetCircleNo();
		if( nCircleNo == 0 )
			continue;

		AppendDockBarAtSideLayout(pDockBar);
	}

	((CFriendlyFrameWnd *) m_pDockSite)->GetDockState(
		*m_pState,
		m_arrProps);
}

CTControlBar::CDockStateBar* CTControlBar::CDockStateSite::AppendFloatingLayout( CTMiniDockFrameWnd *pMiniFrame)
{
	CDockBar *pFloatDocker = STATIC_DOWNCAST(
		CDockBar,
		pMiniFrame->GetWindow(GW_CHILD));

	CTDynamicControlBar *pDynDocker = STATIC_DOWNCAST(
		CTDynamicControlBar,
		pFloatDocker->GetWindow(GW_CHILD));

	CDockStateBar *pBar = new CDockStateBar;
	pBar->StateGet( pDynDocker, NULL);
	m_arrBarStates.Add(pBar);

	return pBar;
}

CTControlBar::CDockStateBar* CTControlBar::CDockStateSite::AppendDockBarAtSideLayout( CTDockBar *pDockBar)
{
	CDockStateBar *pLastState = NULL;
	BOOL bNewRow = FALSE;

	int nCount = pDockBar->m_arrBars.GetSize();

	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = (CTControlBar *) pDockBar->m_arrBars[i];

		if(!pBar)
		{
			bNewRow = TRUE;
			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		CDockStateBar *pState = new CDockStateBar;
		pState->StateGet( pBar, NULL);

		if(bNewRow)
			pState->m_dwDockStateFlags |= CDockStateBar::__STATE_NEW_ROW;

		m_arrBarStates.Add(pState);
		bNewRow = FALSE;
		pLastState = pState;
	}

	return pLastState;
}

void CTControlBar::CDockStateSite::StateSet() const
{
	ControlBarVector vRedockVars;

	for( POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition(); pos;)
	{
		CControlBar *pBar = (CControlBar *) m_pDockSite->m_listControlBars.GetNext(pos);

		if(pBar->GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		{
			if( pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) &&
				!pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
				vRedockVars.Add((CTControlBar *) pBar);

			continue;
		}

		if( pBar->IsDockBar() ||
			!pBar->m_pDockSite ||
			!pBar->m_pDockContext )
			continue;

		if(!pBar->m_pDockBar)
		{
			m_pDockSite->DockControlBar(pBar);
			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE);
			continue;
		}

		if( !pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) ||
			((CTControlBar *) pBar)->IsFixedMode())
			continue;

		if(((CTDockBar *) pBar->m_pDockBar)->_GetCircleNo() == 0 &&
			!pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
			continue;

		m_pDockSite->ShowControlBar( pBar, FALSE, TRUE);
	}

	int nCount = vRedockVars.GetSize();
	for( int i=0; i<nCount; i++)
	{
		UINT nBarDockerID = AFX_IDW_DOCKBAR_TOP;

		CTControlBar *pBar = vRedockVars[i];
		DWORD dwBarStyle = pBar->GetBarStyle();

		if( dwBarStyle & CBRS_ALIGN_BOTTOM )
			nBarDockerID = AFX_IDW_DOCKBAR_BOTTOM;
		else if( dwBarStyle & CBRS_ALIGN_LEFT )
			nBarDockerID = AFX_IDW_DOCKBAR_LEFT;
		else if( dwBarStyle & CBRS_ALIGN_RIGHT )
			nBarDockerID = AFX_IDW_DOCKBAR_RIGHT;

		if(pBar->IsFixedMode())
		{
			BOOL bFloating = pBar->IsFloating();

			if(bFloating)
				m_pDockSite->ShowControlBar( pBar, FALSE, FALSE);

			m_pDockSite->DockControlBar(
				pBar,
				nBarDockerID);

			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE);
		}
		else
		{
			((CTControlBar *) pBar)->DockControlBar(
				nBarDockerID, 1,
				m_pDockSite,
				FALSE);

			m_pDockSite->ShowControlBar( pBar, FALSE, TRUE);
		}
	}

	CTDockBar::_OptimizeCircles(m_pDockSite);
	m_pDockSite->RecalcLayout();

	nCount = m_arrBarStates.GetSize();
	for( i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrBarStates[i];
		pState->StateSet( m_pDockSite, NULL);
	}

	((CFriendlyFrameWnd *) m_pDockSite)->SetDockState(
		*m_pState,
		m_arrProps);

	nCount = m_arrBarStates.GetSize();
	for( i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrBarStates[i];
		pState->PreSyncAttributes( m_pDockSite, NULL);
	}

	m_pDockSite->RecalcLayout();
	nCount = m_arrBarStates.GetSize();

	for( i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrBarStates[i];

		pState->PostSyncAttributes( m_pDockSite, NULL);
		if( pState->m_dwDockStateFlags & CDockStateBar::__STATE_FLOATING )
		{
			CTMiniDockFrameWnd *pMiniFrame = STATIC_DOWNCAST(
				CTMiniDockFrameWnd,
				pState->m_pHelperBar->GetParentFrame());

			pState->m_pHelperBar->_RecalcNcArea();
			pState->m_pHelperBar->OnRepositionSingleChild();

			pMiniFrame->RecalcLayout();
		}
	}

	for( pos = m_pDockSite->m_listControlBars.GetHeadPosition(); pos;)
	{
		CControlBar *pBar = (CControlBar *) m_pDockSite->m_listControlBars.GetNext(pos);

		if( pBar->IsDockBar() ||
			!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			continue;

		if( !((CTControlBar *) pBar)->IsFixedMode() ||
			!pBar->IsFloating())
			continue;

		((CTControlBar *)pBar)->_RecalcNcArea();
		((CTControlBar *)pBar)->_RecalcLayoutImpl();
	}
}

CTControlBar::CDockStateBar::CDockStateBar()
{
	InitMembers();
}

CTControlBar::CDockStateBar::~CDockStateBar()
{
	Clear();
}

void CTControlBar::CDockStateBar::Clear()
{
	InitMembers();

	int nCount = m_arrChildStates.GetSize();
	for( int i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrChildStates[i];

		pState->Clear();
		delete pState;
	}

	m_arrChildStates.RemoveAll();
}

void CTControlBar::CDockStateBar::InitMembers()
{
	m_nDockBarID = AFX_IDW_DOCKBAR_FLOAT;
	m_nBarID = AFX_IDW_DOCKBAR_FLOAT;
	m_nMRUWidth = 50;
	m_nCircleNo = 0;

	m_rcMRUDockPos.SetRectEmpty();
	m_ptFloatPos = CPoint( 0, 0);

	m_rcFloatFrame.SetRect( 0, 0, 200, 200);
	m_dwFloatFrameCreateStyle = WS_CHILD;

	m_nDelayedRowUpdateMetric = 0;
	m_rcBar.SetRect( 0, 0, 200, 200);

	m_dwDockStateFlags = 0;
	m_dwMRUFloatStyle = 0;

	m_ptMRUFloatPos.x = m_ptMRUFloatPos.y;
	m_szDockedH = CSize( 200, 200);
	m_szDockedV = CSize( 200, 200);
	m_szFloated = CSize( 200, 200);

	m_nAciveTabChildIndex = -1;
	m_pHelperBar = NULL;
}

void CTControlBar::CDockStateBar::StateGet( CTControlBar *pBar,
										    CTControlBar::CDockStateBar *pParentState)
{
	Clear();

	m_nDockBarID = pBar->m_pDockBar->GetDlgCtrlID();
	m_nBarID = pBar->GetDlgCtrlID();
	m_nCircleNo = 0;

	if(!pBar->IsFloating())
		m_nCircleNo = ((CTDockBar *) pBar->m_pDockBar)->_GetCircleNo();

	if(pBar->IsVisible())
		m_dwDockStateFlags |= __STATE_VISIBLE;

	CRect m_rcBar;
	m_nMRUWidth = pBar->m_nMRUWidth;
	pBar->GetWindowRect(&m_rcBar);
	pBar->m_pDockBar->ScreenToClient(&m_rcBar);

	m_ptFloatPos = pBar->m_ptFloatHelper;
	m_szDockedH = pBar->m_szDockedH;
	m_szDockedV = pBar->m_szDockedV;
	m_szFloated = pBar->m_szFloated;

	m_nDelayedRowUpdateMetric = pBar->m_nDelayedRowUpdateMetric;

	m_dwMRUFloatStyle = pBar->m_pDockContext->m_dwMRUFloatStyle;
	m_ptMRUFloatPos = pBar->m_pDockContext->m_ptMRUFloatPos;
	m_rcMRUDockPos = pBar->m_pDockContext->m_rectMRUDockPos;

	m_dwBarFlags = pBar->m_dwStyle;
	if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
	{
		m_dwDockStateFlags |= CDockStateBar::__DOCK_DYNAMIC_HALF_SPLIT;
		CTDynamicDockBar *pDynDocker = STATIC_DOWNCAST(
			CTDynamicDockBar,
			pBar->GetWindow(GW_CHILD));

		int nCount = pDynDocker->m_arrBars.GetSize();
		BOOL bNewRow = FALSE;

		for( int i=1; i<nCount; i++)
		{
			CTControlBar *pChildBar = (CTControlBar *) pDynDocker->m_arrBars[i];

			if(!pChildBar)
			{
				bNewRow = TRUE;
				continue;
			}

			if(__PLACEHODLER_BAR_PTR(pChildBar))
				continue;

			CDockStateBar *pState = new CDockStateBar;
			pState->StateGet( pChildBar, this);

			if(bNewRow)
				pState->m_dwDockStateFlags |= CDockStateBar::__STATE_NEW_ROW;

			pState->m_dwDockStateFlags |= CDockStateBar::__STATE_DYNAMIC_CHILD;
			m_arrChildStates.Add(pState);

			bNewRow = FALSE;
		}

		if(pBar->IsFloating())
		{
			m_dwDockStateFlags |= CDockStateBar::__STATE_FLOATING;
			CTMiniDockFrameWnd *pMiniFrame = STATIC_DOWNCAST(
				CTMiniDockFrameWnd,
				pBar->GetParentFrame());

			pMiniFrame->GetWindowRect(&m_rcFloatFrame);
			m_dwFloatFrameCreateStyle = pMiniFrame->GetStyle();
		}
	}
	else
		m_dwDockStateFlags |= CDockStateBar::__DOCK_AT_SIDE;
}

void CTControlBar::CDockStateBar::StateSet( CFrameWnd *pDockSite,
										    CTControlBar::CDockStateBar *pParentState)
{
	m_pHelperBar = NULL;

	switch((m_dwDockStateFlags&__DOCK_AT_MASK))
	{
	case __DOCK_AT_NOWHERE			: break;
	case __DOCK_AT_SIDE				:
		{
			m_pHelperBar = (CTControlBar *) pDockSite->GetControlBar(m_nBarID);

			if(m_dwDockStateFlags&CDockStateBar::__STATE_DYNAMIC_CHILD)
			{
				CTDynamicDockBar *pDockBar = STATIC_DOWNCAST(
					CTDynamicDockBar, (STATIC_DOWNCAST(
					CTDynamicControlBar,
					pParentState->m_pHelperBar))->m_pWndDynDocker);

				pDockBar->_LockSequenceOptimization(TRUE);
				m_pHelperBar->m_bUpdatingChain = TRUE;
				pDockBar->DockControlBar(m_pHelperBar);

				m_pHelperBar->MoveWindow( &m_rcBar, FALSE);
				m_pHelperBar->m_bUpdatingChain = FALSE;
				pDockBar->_LockSequenceOptimization(FALSE);

				int nBarPos = pDockBar->FindBar(m_pHelperBar);
				if( nBarPos > 1 )
				{
					if(m_dwDockStateFlags&__STATE_NEW_ROW)
					{
						if(pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.InsertAt( nBarPos, (CControlBar *) NULL);
					}
					else
					{
						if(!pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.RemoveAt(nBarPos - 1);
					}
				}
			}
			else
			{
				CTDockBar *pDockBar = STATIC_DOWNCAST(
					CTDockBar,
					pDockSite->GetControlBar(m_nDockBarID));

				pDockBar = pDockBar->_GetBarByCircleNo(m_nCircleNo);
				pDockBar->_LockSequenceOptimization(TRUE);
				pDockBar->DockControlBar(m_pHelperBar);
				pDockBar->_LockSequenceOptimization(FALSE);

				int nBarPos = pDockBar->FindBar(m_pHelperBar);
				if( nBarPos > 1 )
				{
					if(m_dwDockStateFlags&__STATE_NEW_ROW)
					{
						if(pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.InsertAt( nBarPos, (CControlBar *) NULL);
					}
					else
					{
						if(!pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.RemoveAt(nBarPos - 1);
					}
				}
			}
		}

		break;

	case __DOCK_DYNAMIC_HALF_SPLIT	:
		{
			CFrameWnd *pParentFrame = pDockSite;

			if(m_dwDockStateFlags&CDockStateBar::__STATE_FLOATING)
			{
				CTMiniDockFrameWnd *pMiniFrame = new CTMiniDockFrameWnd();

				if(!pMiniFrame->Create(
					pDockSite,
					m_dwFloatFrameCreateStyle))
					AfxThrowResourceException();

				pMiniFrame->MoveWindow( &m_rcFloatFrame, FALSE);
				pParentFrame = pMiniFrame;
			}

			m_pHelperBar = new CTDynamicControlBar;
			DWORD dwDynStyle = WS_CHILD | (m_dwDockStateFlags&CBRS_ALIGN_ANY);

			dwDynStyle |= CBRS_BORDER_ANY;
			dwDynStyle &= ~(CBRS_FLOATING|CBRS_GRIPPER);
			dwDynStyle |= CBRS_TOOLTIPS;

			m_pHelperBar->SetInitDesiredSizeHorizontal(m_szDockedH);
			m_pHelperBar->SetInitDesiredSizeVertical(m_szDockedV);
			m_pHelperBar->SetInitDesiredSizeFloating(m_szFloated);
			if(!m_pHelperBar->Create(
				NULL,
				pDockSite,
				m_nBarID,
				dwDynStyle))
				return;

			m_pHelperBar->m_pDockSite = pDockSite;
			((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker->m_pDockSite = pDockSite;

			m_pHelperBar->m_hWndOwner = pDockSite->GetSafeHwnd();
			m_pHelperBar->EnableDocking(CBRS_ALIGN_ANY);

			if(!pDockSite->m_listControlBars.Find(m_pHelperBar))
				pDockSite->m_listControlBars.AddTail(m_pHelperBar);

			if(m_dwDockStateFlags&CDockStateBar::__STATE_FLOATING)
			{
				CTMiniDockFrameWnd *pMiniFrame = STATIC_DOWNCAST(
					CTMiniDockFrameWnd,
					pParentFrame);

				CDockBar *pFloatingDocker = STATIC_DOWNCAST(
					CDockBar,
					pMiniFrame->GetWindow(GW_CHILD));

				m_pHelperBar->SetParent(pFloatingDocker);
			}

			if(!((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified)
			{
				POSITION posToRemove = pDockSite->m_listControlBars.Find(((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker);

				if(posToRemove)
				{
					pDockSite->m_listControlBars.RemoveAt(posToRemove);
					((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = TRUE;
				}
			}

			if(m_dwDockStateFlags&CDockStateBar::__STATE_FLOATING)
			{
				CDockBar *pDockBar = STATIC_DOWNCAST(
					CDockBar,
					pParentFrame->GetWindow(GW_CHILD));

				((CTDockBar *) pDockBar)->DockControlBar(m_pHelperBar);
				m_pHelperBar->m_bUpdatingChain = TRUE;
				m_pHelperBar->MoveWindow( &m_rcBar, FALSE);
				m_pHelperBar->m_bUpdatingChain = FALSE;
			}
			else if(m_dwDockStateFlags&CDockStateBar::__STATE_DYNAMIC_CHILD)
			{
				CTDockBar *pDockBar = STATIC_DOWNCAST(
					CTDockBar,
					(STATIC_DOWNCAST(
					CTDynamicControlBar,
					pParentState->m_pHelperBar))->m_pWndDynDocker);

				pDockBar->_LockSequenceOptimization(TRUE);
				pDockBar->DockControlBar(m_pHelperBar);
				m_pHelperBar->m_bUpdatingChain = TRUE;
				m_pHelperBar->MoveWindow( &m_rcBar, FALSE);
				m_pHelperBar->m_bUpdatingChain = FALSE;
				pDockBar->_LockSequenceOptimization(FALSE);

				int nBarPos = pDockBar->FindBar(m_pHelperBar);
				if( nBarPos > 1 )
				{
					if(m_dwDockStateFlags&__STATE_NEW_ROW)
					{
						if(pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.InsertAt( nBarPos, (CControlBar *) NULL);
					}
					else
					{
						if(!pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.RemoveAt(nBarPos - 1);
					}
				}
			}
			else
			{
				CTDockBar *pDockBar = STATIC_DOWNCAST(
					CTDockBar,
					pDockSite->GetControlBar(m_nDockBarID));

				pDockBar = pDockBar->_GetBarByCircleNo(m_nCircleNo);
				pDockBar->_LockSequenceOptimization(TRUE);
				pDockBar->DockControlBar(m_pHelperBar);

				m_pHelperBar->m_bUpdatingChain = TRUE;
				m_pHelperBar->MoveWindow( &m_rcBar, FALSE);
				m_pHelperBar->m_bUpdatingChain = FALSE;
				pDockBar->_LockSequenceOptimization(FALSE);

				int nBarPos = pDockBar->FindBar(m_pHelperBar);
				if( nBarPos > 1 )
				{
					if(m_dwDockStateFlags&__STATE_NEW_ROW)
					{
						if(pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.InsertAt( nBarPos, (CControlBar *) NULL);
					}
					else
					{
						if(!pDockBar->m_arrBars[nBarPos - 1])
							pDockBar->m_arrBars.RemoveAt(nBarPos - 1);
					}
				}
			}

			int nCount = m_arrChildStates.GetSize();
			for( int i=0; i<nCount; i++)
			{
				CDockStateBar *pState = m_arrChildStates[i];
				pState->StateSet( pDockSite, this);
			}
		}

		break;
	}

	pDockSite->ShowControlBar(
		m_pHelperBar,
		(m_dwDockStateFlags&__STATE_VISIBLE) ? TRUE : FALSE,
		TRUE);

	CTMiniDockFrameWnd *pMiniFrame = DYNAMIC_DOWNCAST(
		CTMiniDockFrameWnd,
		m_pHelperBar->GetParentFrame());

	if(pMiniFrame)
	{
		int nCount = pMiniFrame->m_listControlBars.GetCount();

		if(nCount)
			pMiniFrame->m_listControlBars.RemoveAll();
	}

	m_pHelperBar->m_dwStyle = m_dwBarFlags;
	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;

	m_pHelperBar->m_szDockedH = m_szDockedH;
	m_pHelperBar->m_szDockedV = m_szDockedV;
	m_pHelperBar->m_szFloated = m_szFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;
	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;
}

void CTControlBar::CDockStateBar::PreSyncAttributes( CFrameWnd *pDockSite,
													 CTControlBar::CDockStateBar *pParentState)
{
	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;
	m_pHelperBar->m_szDockedH = m_szDockedH;
	m_pHelperBar->m_szDockedV = m_szDockedV;
	m_pHelperBar->m_szFloated = m_szFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;
	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;

	int nCount = m_arrChildStates.GetSize();
	for( int i=0; i<nCount; i++)
	{
		CDockStateBar *pState = m_arrChildStates[i];
		pState->PreSyncAttributes( pDockSite, this);
	}
}

void CTControlBar::CDockStateBar::PostSyncAttributes( CFrameWnd *pDockSite,
													  CTControlBar::CDockStateBar *pParentState)
{
	BOOL bWalkChild = FALSE;

	switch(m_dwDockStateFlags&__DOCK_AT_MASK)
	{
	case __DOCK_AT_NOWHERE			: break;
	case __DOCK_AT_SIDE				: break;
	case __DOCK_DYNAMIC_HALF_SPLIT	:
		{
			bWalkChild = TRUE;

			if(!((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified)
			{
				POSITION posToRemove = pDockSite->m_listControlBars.Find(((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker);

				if(posToRemove)
				{
					pDockSite->m_listControlBars.RemoveAt(posToRemove);
					((CTDynamicControlBar *) m_pHelperBar)->m_pWndDynDocker->m_bHelperDockSiteModified = TRUE;
				}
			}

			if(m_dwDockStateFlags&CDockStateBar::__STATE_FLOATING)
			{
				CTMiniDockFrameWnd *pParentFrame = STATIC_DOWNCAST(
					CTMiniDockFrameWnd,
					m_pHelperBar->GetParentFrame());

				pParentFrame->RecalcLayout();
			}
		}

		break;
	}

	if(bWalkChild)
	{
		int nCount = m_arrChildStates.GetSize();

		for( int i=0; i<nCount; i++)
		{
			CDockStateBar *pState = m_arrChildStates[i];
			pState->PostSyncAttributes( pDockSite, this);
		}
	}

	m_pHelperBar->m_ptFloatHelper = m_ptFloatPos;
	m_pHelperBar->m_szDockedH = m_szDockedH;
	m_pHelperBar->m_szDockedV = m_szDockedV;
	m_pHelperBar->m_szFloated = m_szFloated;

	m_pHelperBar->m_nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;
	m_pHelperBar->m_nMRUWidth = m_nMRUWidth;

	m_pHelperBar->m_pDockContext->m_dwMRUFloatStyle = m_dwMRUFloatStyle;
	m_pHelperBar->m_pDockContext->m_rectMRUDockPos = m_rcMRUDockPos;
	m_pHelperBar->m_pDockContext->m_ptMRUFloatPos = m_ptMRUFloatPos;
}

LRESULT CTControlBar::OnSetText( WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CWnd::Default();
	BOOL bTextOnGripper = FALSE;

	if(IsFloating())
	{
		CFrameWnd *pFrame = GetParentFrame();

		if( pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		{
			m_pDockBar->SetWindowText((LPCTSTR) lParam);
			pFrame->SetWindowText((LPCTSTR) lParam);
			pFrame->DelayRecalcLayout();
			pFrame->SendMessage(WM_NCPAINT);
		}
	}

	if(IsBarWithGripper( NULL, &bTextOnGripper))
	{
		if(bTextOnGripper)
			SendMessage(WM_NCPAINT);
	}

	return lResult;
}

static void AddUnique( ControlBarVector& vBars, CTControlBar *pBar)
{
	int nCount = vBars.GetSize();

	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pAddedBar = vBars[i];

		if( pAddedBar == pBar )
			return;
	}

	vBars.Add(pBar);
}

void CTControlBar::_GetFrameControlBars( CFrameWnd *pFrame,
										 ControlBarVector& vBars,
										 DWORD dwCallOpt,
										 CPoint ptRestrict)
{
	vBars.RemoveAll();

	if(!(dwCallOpt & (DWORD(__GFECB_FIXED)|DWORD(__GFECB_NONFIXED))))
		return;

	if(!(dwCallOpt & (DWORD(__GFECB_BAR_VISIBLE)|DWORD(__GFECB_BAR_INVISIBLE))))
		return;

	if(!(dwCallOpt & (DWORD(__GFECB_WND_VISIBLE)|DWORD(__GFECB_WND_INVISIBLE))))
		return;

	int nAllBarsCount = m_AllBars.GetSize();
	for( int i=0; i<nAllBarsCount; i++)
	{
		CTControlBar *pBar = m_AllBars[i];

		if(!pBar->GetSafeHwnd())
			continue;

		if(!(::IsWindow(pBar->GetSafeHwnd())))
			continue;

		if(pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
			continue;

		if(pBar->m_bPresubclassDialogMode)
			continue;

		CFrameWnd *pBarFrame = _GetDockingFrameImpl(pBar);
		if( pBarFrame != pFrame )
			continue;

		if( dwCallOpt == ((DWORD)__GFECB_ALL_NONRESTRICT) )
		{
			vBars.Add(pBar);
			continue;
		}

		if(!(dwCallOpt&((DWORD)__GFECB_FIXED)))
		{
			if(pBar->IsFixedMode())
				continue;
		}

		if(!pBar->IsFixedMode())
		{
			if(!(dwCallOpt&((DWORD)__GFECB_NONFIXED)))
				continue;

			BOOL bMinOnRow = pBar->IsMinimizedOnRow();
			if(!(dwCallOpt&((DWORD)__GFECB_NONFIXED_MIN)))
			{
				if(bMinOnRow)
					continue;
			}

			if(!(dwCallOpt&((DWORD)__GFECB_NONFIXED_NONMIN)))
			{
				if(!bMinOnRow)
					continue;
			}
		}

		if(pBar->IsVisible())
		{
			if(!(dwCallOpt&((DWORD)__GFECB_BAR_VISIBLE)))
				continue;
		}
		else
		{
			if(!(dwCallOpt&((DWORD)__GFECB_BAR_INVISIBLE)))
				continue;
		}

		if(pBar->IsWindowVisible())
		{
			if(!(dwCallOpt&((DWORD)__GFECB_WND_VISIBLE)))
				continue;
		}
		else
		{
			if(!(dwCallOpt&((DWORD)__GFECB_WND_INVISIBLE)))
				continue;
		}

		if(pBar->IsFloating())
		{
			if(!(dwCallOpt&((DWORD)__GFECB_FLOATED)))
				continue;
		}
		else
		{
			if(!(dwCallOpt&((DWORD)__GFECB_DOCKED)))
				continue;

			if( (dwCallOpt&((DWORD)__GFECB_DOCKED)) != ((DWORD)__GFECB_DOCKED) )
			{
				UINT nDockBarID = pBar->GetSafeDockBarDlgCtrlID();

				switch(nDockBarID)
				{
				case AFX_IDW_DOCKBAR_TOP		:
					if(!(dwCallOpt&((DWORD)__GFECB_DOCKED_TOP)))
						continue;

					break;

				case AFX_IDW_DOCKBAR_BOTTOM		:
					if(!(dwCallOpt&((DWORD)__GFECB_DOCKED_BOTTOM)))
						continue;

					break;

				case AFX_IDW_DOCKBAR_LEFT		:
					if(!(dwCallOpt&((DWORD)__GFECB_DOCKED_LEFT)))
						continue;

					break;

				case AFX_IDW_DOCKBAR_RIGHT		:
					if(!(dwCallOpt&((DWORD)__GFECB_DOCKED_RIGHT)))
						continue;

					break;
				}
			}
		}

		if(dwCallOpt&((DWORD)__GFECB_BYPT_ALL))
		{
			CWnd *pWndNcTest = pBar;
			CRect rcWnd;

			if(pBar->IsFloating())
				pWndNcTest = pBar->GetDockingFrame();
			pWndNcTest->GetWindowRect(&rcWnd);

			BOOL bInsideWndArea = rcWnd.PtInRect(ptRestrict);
			if(!bInsideWndArea)
				continue;

			if(dwCallOpt&((DWORD)__GFECB_BYPT_ONLY_WND))
			{
				AddUnique( vBars, pBar);
				return;
			}

			if(dwCallOpt&((DWORD)__GFECB_BYPT_NCGRIP))
			{
				if(pBar->m_rcGrip.IsRectEmpty())
					continue;

				CPoint ptWnd(ptRestrict);
				ptWnd -= rcWnd.TopLeft();

				if(!pBar->m_rcGrip.PtInRect(ptWnd))
					continue;
			}

			if(dwCallOpt&((DWORD)__GFECB_BYPT_CLIENT))
			{
				CRect rcClient;

				pBar->GetClientRect(&rcClient);
				pBar->ClientToScreen(&rcClient);

				if(!rcClient.PtInRect(ptRestrict))
					continue;
			}

			if(dwCallOpt&((DWORD)__GFECB_BYPT_NC))
			{
				UINT nHitTest = pWndNcTest->SendMessage(
					WM_NCHITTEST,
					WPARAM(0),
					MAKELPARAM( ptRestrict.x, ptRestrict.y));

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTLEFT))
				{
					if( nHitTest != HTLEFT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTRIGHT))
				{
					if( nHitTest != HTRIGHT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTTOP))
				{
					if( nHitTest != HTTOP )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOM))
				{
					if( nHitTest != HTBOTTOM )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTTOPLEFT))
				{
					if( nHitTest != HTTOPLEFT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTTOPRIGHT))
				{
					if( nHitTest != HTTOPRIGHT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOMLEFT))
				{
					if( nHitTest != HTBOTTOMLEFT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTBOTTOMRIGHT))
				{
					if( nHitTest != HTBOTTOMRIGHT )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTNOWHERE))
				{
					if( nHitTest != HTNOWHERE )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTSYSMENU))
				{
					if( nHitTest != HTSYSMENU )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTGROWBOX))
				{
					if( nHitTest != HTGROWBOX )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTMINBUTTON))
				{
					if( nHitTest != HTMINBUTTON )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTMAXBUTTON))
				{
					if( nHitTest != HTMAXBUTTON )
						continue;
				}

				if(dwCallOpt&((DWORD)__GFECB_BYPT_HTCLIENT))
				{
					if( nHitTest != HTCLIENT )
						continue;
				}
			}
		}

		AddUnique( vBars, pBar);
	}
}

void CTControlBar::_GetFrameControlBars( ControlBarVector& vBars,
										 DWORD dwCallOpt,
										 CPoint ptRestrict) const
{
	CFrameWnd *pFrame = _GetDockingFrameImpl();

	_GetFrameControlBars(
		pFrame,
		vBars,
		dwCallOpt,
		ptRestrict);
}

CControlBar *CTControlBar::_GetNextControlBarInRow( CControlBar *pBar,
												    BOOL bNext,
													BOOL bBar,
													BOOL bNonFixed)
{
	int nCountOfSideBars = pBar->m_pDockBar->m_arrBars.GetSize();
	int nPos = pBar->m_pDockBar->FindBar(pBar);

	if( nPos == 0 && !bNext )
		return NULL;

	if( nPos == (nCountOfSideBars - 1) && bNext )
		return NULL;

	if(bNext)
	{
		for( nPos++; nPos<nCountOfSideBars; nPos++)
		{
			CControlBar *pExamineBar = (CControlBar *) pBar->m_pDockBar->m_arrBars[nPos];

			if(!pExamineBar)
				return NULL;

			if(__PLACEHODLER_BAR_PTR(pExamineBar))
				continue;

			if(bBar)
			{
				if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					continue;

				if(bNonFixed)
				{
					if(((CTControlBar *) pBar)->IsFixedMode())
						continue;
				}
			}

			return pExamineBar;
		}
	}
	else
	{
		for( nPos--; nPos>=0; nPos--)
		{
			CControlBar *pExamineBar = (CControlBar *) pBar->m_pDockBar->m_arrBars[nPos];

			if(!pExamineBar)
				return NULL;

			if(__PLACEHODLER_BAR_PTR(pExamineBar))
				continue;

			if(bBar)
			{
				if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					continue;

				if(bNonFixed)
				{
					if(((CTControlBar *) pBar)->IsFixedMode())
						continue;
				}
			}

			return pExamineBar;
		}
	}

	return NULL;
}

CControlBar *CTControlBar::_GetFirstControlBarInRow( CControlBar *pBar,
													 BOOL bNext,
													 BOOL bBar,
													 BOOL bNonFixed)
{
	int nPos = pBar->m_pDockBar->FindBar(pBar);

	if( nPos == 1 && !bNext )
	{
		if(bBar)
		{
			if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			{
				CControlBar *pControlBar = _GetNextControlBarInRow(
					pBar,
					TRUE,
					bBar,
					bNonFixed);

				return pControlBar ? pControlBar : pBar;
			}

			if( bNonFixed && ((CTControlBar *) pBar)->IsFixedMode())
			{
				CControlBar *pControlBar = _GetNextControlBarInRow(
					pBar,
					TRUE,
					bBar,
					bNonFixed);

				return pControlBar ? pControlBar : pBar;
			}
		}

		return pBar;
	}

	int nCountOfSideBars = pBar->m_pDockBar->m_arrBars.GetSize();
	if( nPos == (nCountOfSideBars - 1) && bNext )
	{
		if(bBar)
		{
			if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			{
				CControlBar *pControlBar = _GetNextControlBarInRow(
					pBar,
					FALSE,
					bBar,
					bNonFixed);

				return pControlBar ? pControlBar : pBar;
			}

			if( bNonFixed && ((CTControlBar *) pBar)->IsFixedMode())
			{
				CControlBar *pControlBar = _GetNextControlBarInRow(
					pBar,
					FALSE,
					bBar,
					bNonFixed);

				return pControlBar ? pControlBar : pBar;
			}
		}

		return pBar;
	}

	CControlBar *pPrevBar = pBar;
	if(bNext)
	{
		for( nPos++; nPos<nCountOfSideBars; nPos++)
		{
			CControlBar *pExamineBar = (CControlBar *) pBar->m_pDockBar->m_arrBars[nPos];

			if(!pExamineBar)
				return pPrevBar;

			if(__PLACEHODLER_BAR_PTR(pExamineBar))
				continue;

			if(bBar)
			{
				if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					continue;

				if(bNonFixed)
				{
					if(((CTControlBar *) pBar)->IsFixedMode())
						continue;
				}
			}

			pPrevBar = pExamineBar;
		}
	}
	else
	{
		for( nPos--; nPos>=0; nPos--)
		{
			CControlBar *pExamineBar = (CControlBar *) pBar->m_pDockBar->m_arrBars[nPos];

			if(!pExamineBar)
				return pPrevBar;

			if(__PLACEHODLER_BAR_PTR(pExamineBar))
				continue;

			if(bBar)
			{
				if(!pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
					continue;

				if(bNonFixed)
				{
					if(((CTControlBar *) pBar)->IsFixedMode())
						continue;
				}
			}

			pPrevBar = pExamineBar;
		}
	}

	return pPrevBar;
}

void CTControlBar::_GetNextRowBars( BOOL bNextRow,
								    MFCControlBarVector& vBars,
									CRect *pRcAllBars,
									int *pOwnPos,
									BOOL *pRowMaxSized,
									BOOL *pRowFullSized) const
{
	if(pRcAllBars)
		pRcAllBars->SetRect( 0, 0, 0, 0);

	if(pOwnPos)
		*pOwnPos = -1;

	if(pRowMaxSized)
		*pRowMaxSized = FALSE;

	if(pRowFullSized)
		*pRowFullSized = FALSE;

	vBars.RemoveAll();
	if(!m_pDockBar)
		return;

	int nOwnIdx = m_pDockBar->FindBar((CControlBar *) this);
	if( nOwnIdx < 0 )
		return;

	if(pOwnPos)
		*pOwnPos = nOwnIdx;

	int nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	int nIncrement = bNextRow ? 1 : -1;
	int nFirstToReview = -1;

	for( int i = nOwnIdx + nIncrement; TRUE; i += nIncrement)
	{
		if( i == 0 || i == nTotalDockCount )
			return;

		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];
		if(!pBar)
		{
			nFirstToReview = i + nIncrement;

			if( nFirstToReview >= nTotalDockCount )
				return;

			break;
		}
	}

	if( nFirstToReview <= 0 )
		return;

	BOOL bHorz = IsDockedHorizontally();
	int nBarWidth = 0;

	for( i = nFirstToReview; TRUE; i += nIncrement )
	{
		if( i < 0 || i >= nTotalDockCount )
			break;

		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];
		if(!pBar)
		{
			if(!vBars.GetSize())
			{
				nBarWidth = 0;

				if(pRcAllBars)
					pRcAllBars->SetRect( 0, 0, 0, 0);

				if(pRowMaxSized)
					*pRowMaxSized = FALSE;

				if(pRowFullSized)
					*pRowFullSized = FALSE;

				continue;
			}

			break;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(!pBar->IsVisible())
			continue;

		if(pRcAllBars)
		{
			if(!vBars.GetSize())
				pBar->GetWindowRect(pRcAllBars);
			else
			{
				CRect rcWndBar;

				pBar->GetWindowRect(&rcWndBar);
				pRcAllBars->left = min( pRcAllBars->left, rcWndBar.left);
				pRcAllBars->top = min( pRcAllBars->top, rcWndBar.top);
				pRcAllBars->right = max( pRcAllBars->right, rcWndBar.right);
				pRcAllBars->bottom = max( pRcAllBars->bottom, rcWndBar.bottom);
			}
		}

		if(( pRowMaxSized || pRowFullSized ) &&
			pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
		{
			if( pRowMaxSized && !(*pRowMaxSized) &&
				!((CTControlBar *) pBar)->IsFixedMode())
				*pRowMaxSized = TRUE;

			if(	pRowFullSized && !(*pRowFullSized) &&
				pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
				*pRowFullSized = TRUE;

			if( pRowFullSized != NULL && !(*pRowFullSized) )
			{
				CRect rcBarWnd;

				pBar->GetWindowRect(&rcBarWnd);
				nBarWidth += bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
		}

		vBars.Add(pBar);
	}

	if( pRowFullSized && (!(*pRowFullSized)))
	{
		CRect rcDockBarWnd;

		m_pDockBar->GetWindowRect(&rcDockBarWnd);
		int nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();

		nDockBarWidth--;
		if( nBarWidth >= nDockBarWidth )
			*pRowFullSized = TRUE;
	}
}

void CTControlBar::_GetRowBars( MFCControlBarVector& vBars,
							    CRect *pRcAllBars,
								int *pOwnPos,
								BOOL *pRowMaxSized,
								BOOL *pRowFullSized) const
{
	if(!pRcAllBars)
		pRcAllBars->SetRect( 0, 0, 0, 0);

	if(pOwnPos)
		*pOwnPos = -1;

	if(pRowMaxSized)
		*pRowMaxSized = FALSE;

	if(pRowFullSized)
		*pRowFullSized = FALSE;

	vBars.RemoveAll();
	if(!m_pDockBar)
		return;

	int nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	int nBarWidth = 0;

	BOOL bHorz = IsDockedHorizontally();
	BOOL bThisFound = FALSE;

	for( int i=0; i<nTotalDockCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(!pBar)
		{
			if(bThisFound)
				break;

			vBars.RemoveAll();
			nBarWidth = 0;

			if(pRcAllBars)
				pRcAllBars->SetRect( 0, 0, 0, 0);

			if(pRowMaxSized)
				*pRowMaxSized = FALSE;

			if(pRowFullSized)
				*pRowFullSized = FALSE;

			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if( pBar == (CControlBar *) this )
		{
			bThisFound = TRUE;

			if(pOwnPos)
				*pOwnPos = vBars.GetSize();
		}

		if(!pBar->IsVisible())
			continue;

		if(pRcAllBars)
		{
			if(vBars.GetSize())
			{
				CRect rcWndBar;

				pBar->GetWindowRect(&rcWndBar);
				pRcAllBars->left = min( pRcAllBars->left, rcWndBar.left);
				pRcAllBars->top = min( pRcAllBars->top, rcWndBar.top);
				pRcAllBars->right = max( pRcAllBars->right, rcWndBar.right);
				pRcAllBars->bottom = max( pRcAllBars->bottom, rcWndBar.bottom);
			}
			else
				pBar->GetWindowRect(pRcAllBars);
		}

		if(( pRowMaxSized || pRowFullSized ) &&
			pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
		{
			if( pRowMaxSized && !(*pRowMaxSized) &&
				!((CTControlBar *) pBar)->IsFixedMode())
				*pRowMaxSized = TRUE;

			if( pRowFullSized && !(*pRowFullSized) &&
				pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
				*pRowFullSized = TRUE;

			if( pRowFullSized && !(*pRowFullSized) )
			{
				CRect rcBarWnd;

				pBar->GetWindowRect(&rcBarWnd);
				nBarWidth += bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
		}

		vBars.Add(pBar);
	}

	if( pRowFullSized && !(*pRowFullSized) )
	{
		CRect rcDockBarWnd;

		m_pDockBar->GetWindowRect(&rcDockBarWnd);
		int nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();

		nDockBarWidth--;
		if( nBarWidth >= nDockBarWidth )
			*pRowFullSized = TRUE;
	}
}

void CTControlBar::_GetRowBars( ControlBarVector& vBars,
							    CRect *pRcAllBars,
								int *pOwnPos,
								BOOL *pRowMaxSized,
								BOOL *pRowFullSized) const
{
	if(pRcAllBars)
		pRcAllBars->SetRect( 0, 0, 0, 0);

	if(pOwnPos)
		*pOwnPos = -1;

	if(pRowMaxSized)
		*pRowMaxSized = FALSE;

	if(pRowFullSized)
		*pRowFullSized = FALSE;

	vBars.RemoveAll();
	if(!m_pDockBar)
		return;

	BOOL bHorz = IsDockedHorizontally();
	BOOL bThisFound = FALSE;

	int nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	int nBarWidth = 0;

	for( int i=0; i<nTotalDockCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(!pBar)
		{
			if(bThisFound)
				break;

			vBars.RemoveAll();
			nBarWidth = 0;

			if(pRcAllBars)
				pRcAllBars->SetRect( 0, 0, 0, 0);

			if(pRowMaxSized)
				*pRowMaxSized = FALSE;

			if(pRowFullSized)
				*pRowFullSized = FALSE;

			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if( pBar == (CControlBar *) this )
		{
			bThisFound = TRUE;

			if(pOwnPos)
				*pOwnPos = vBars.GetSize();
		}

		if(pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
		{
			if(!((CTControlBar *) pBar)->IsFixedMode())
				vBars.Add((CTControlBar *) pBar);

			if( pRowMaxSized || pRowFullSized )
			{
				if( pRowMaxSized &&
					!(*pRowMaxSized) &&
					!((CTControlBar *) pBar)->IsFixedMode())
					*pRowMaxSized = TRUE;

				if( pRowFullSized &&
					!(*pRowFullSized) &&
					pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
					*pRowFullSized = TRUE;
			}
		}

		if( pRowFullSized != NULL && !(*pRowFullSized) )
		{
			CRect rcBarWnd;

			pBar->GetWindowRect(&rcBarWnd);
			nBarWidth += bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
		}

		if(pRcAllBars)
		{
			if(vBars.GetSize())
			{
				CRect rcWndBar;

				pBar->GetWindowRect(&rcWndBar);
				pRcAllBars->left = min( pRcAllBars->left, rcWndBar.left);
				pRcAllBars->top = min( pRcAllBars->top, rcWndBar.top);
				pRcAllBars->right = max( pRcAllBars->right, rcWndBar.right);
				pRcAllBars->bottom = max( pRcAllBars->bottom, rcWndBar.bottom);
			}
			else
				pBar->GetWindowRect(pRcAllBars);
		}
	}

	if( pRowFullSized && !(*pRowFullSized) )
	{
		CRect rcDockBarWnd;

		m_pDockBar->GetWindowRect(&rcDockBarWnd);
		int nDockBarWidth = bHorz ? rcDockBarWnd.Width() : rcDockBarWnd.Height();

		nDockBarWidth--;
		if( nBarWidth >= nDockBarWidth )
			*pRowFullSized = TRUE;
	}
}

BOOL CTControlBar::_IsFirstBarInRow( BOOL bBar) const
{
	if(!m_pDockBar)
		return FALSE;

	int nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	int nRowCount = 0;

	for( int i=0; i<nTotalDockCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(!pBar)
		{
			nRowCount = 0;
			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(bBar)
		{
			CTControlBar *pControlBar = DYNAMIC_DOWNCAST( CTControlBar, pBar);

			if(!pControlBar)
				return FALSE;

			if(pControlBar->IsFixedMode())
				return FALSE;
		}

		if( pBar == (CControlBar *) this )
			return nRowCount ? FALSE : TRUE;

		nRowCount++;
	}

	return FALSE;
}

BOOL CTControlBar::_IsLastBarInRow( BOOL bBar) const
{
	if(bBar)
	{
		ControlBarVector vBars;

		_GetRowBars(vBars);
		int nCountOfBars = vBars.GetSize();

		if( vBars[nCountOfBars - 1] == (CTControlBar *) this )
			return TRUE;

		return FALSE;
	}

	MFCControlBarVector vBars;
	_GetRowBars(vBars);

	int nCountOfBars = vBars.GetSize();
	if( vBars[nCountOfBars - 1] == (CControlBar *) this )
		return TRUE;

	return FALSE;
}

CFrameWnd *CTControlBar::_GetDockingFrameImpl( CControlBar *pBar)
{
	if(pBar->m_pDockSite)
		return pBar->m_pDockSite;

	CFrameWnd *pFrame = pBar->GetParentFrame();
	if(!pFrame)
		return NULL;

	if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		pFrame = pFrame->GetParentFrame();

	return pFrame;
}

CFrameWnd *CTControlBar::_GetDockingFrameImpl() const
{
	return _GetDockingFrameImpl((CControlBar *) this);
}

UINT CTControlBar::GetSafeDockBarDlgCtrlID() const
{
	CWnd *pParentWnd = m_pDockBar ? m_pDockBar : GetParent();

	if( pParentWnd->GetSafeHwnd() == NULL ||
		!::IsWindow(pParentWnd->GetSafeHwnd()) ||
		!pParentWnd->IsKindOf(RUNTIME_CLASS(CDockBar)))
		return AFX_IDW_DOCKBAR_FLOAT;

	return pParentWnd->GetDlgCtrlID();
}

BOOL CTControlBar::IsDockedAtTop() const
{
	if(!m_pDockSite)
		return m_dwStyle&CBRS_TOP;

	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_TOP;
}

BOOL CTControlBar::IsDockedAtBottom() const
{
	if(!m_pDockSite)
		return m_dwStyle&CBRS_BOTTOM;

	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_BOTTOM;
}

BOOL CTControlBar::IsDockedAtLeft() const
{
	if(!m_pDockSite)
		return m_dwStyle&CBRS_LEFT;

	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_LEFT;
}

BOOL CTControlBar::IsDockedAtRight() const
{
	if(!m_pDockSite)
		return m_dwStyle&CBRS_RIGHT;

	return GetSafeDockBarDlgCtrlID() == AFX_IDW_DOCKBAR_RIGHT;
}

BOOL CTControlBar::IsDockedHorizontally() const
{
	if(!m_pDockSite)
		return m_dwStyle&(CBRS_TOP|CBRS_BOTTOM);

	int nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP ||
		nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM;
}

BOOL CTControlBar::IsDockedVertically() const
{
	if(!m_pDockSite)
		return m_dwStyle&(CBRS_LEFT|CBRS_RIGHT);

	int nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT ||
		nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

BOOL CTControlBar::IsDocked() const
{
	if(!m_pDockSite)
		return TRUE;

	int nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();
	return nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP ||
		nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM ||
		nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT ||
		nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT;
}

BOOL CTControlBar::IsFloating() const
{
	return !IsDocked() && GetParentFrame();
}

BOOL CTControlBar::_IsSingleVisibleInFloatingPalette()
{
	if( !m_pDockSite ||
		!IsVisible() ||
		IsFixedMode())
		return FALSE;

	CFrameWnd *pFrame = GetParentFrame();
	if(!pFrame)
		return FALSE;

	if(!pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		return FALSE;

	POSITION pos = m_pDockSite->m_listControlBars.GetHeadPosition();
	while(pos)
	{
		CTControlBar *pBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			(CControlBar *) (m_pDockSite->m_listControlBars.GetNext(pos)));

		if(( !pBar || pBar == this ||
			!pBar->m_pDockSite ||
			!pBar->m_pDockBar ||
			pBar->IsFixedMode() ||
			pBar->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar))) ||
			pBar->GetParentFrame() != pFrame )
			continue;

		if(!pBar->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			continue;

		if(pBar->IsVisible())
			return FALSE;
	}

	return TRUE;
}

BOOL CTControlBar::IsBarWithGripper( BOOL *pGripperAtTop,
									 BOOL *pTextOnGripper) const
{
	if(pGripperAtTop)
		*pGripperAtTop = FALSE;

	if(pTextOnGripper)
		*pTextOnGripper = FALSE;

	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if(!(m_dwStyle&CBRS_GRIPPER) || IsFloating())
		return FALSE;

	CFrameWnd *pParentFrame = GetParentFrame();
	if( pParentFrame != NULL &&
		pParentFrame->IsKindOf(RUNTIME_CLASS(CTMiniDockFrameWnd)))
	{
		if(((CTControlBar *) this)->_IsSingleVisibleInFloatingPalette())
			return FALSE;
	}

	if( pGripperAtTop && (
		m_bGripperStaticallyAtTop ||
		IsDockedVertically()))
		*pGripperAtTop = TRUE;

	if(pTextOnGripper)
		*pTextOnGripper = TRUE;

	return TRUE;
}

CSize CTControlBar::CalcFixedLayout( BOOL bStretch,
									 BOOL bHorz)
{
	return _CalcDesiredLayout(bHorz);
}

CSize CTControlBar::CalcDynamicLayout( int nLength,
									   DWORD nMode)
{
	if(!(nMode & (LM_HORZDOCK | LM_VERTDOCK)) || IsFloating())
	{
		if( nLength > 0 )
		{
			if(nMode&LM_LENGTHY)
				m_szFloated.cy = nLength;
			else
				m_szFloated.cx = nLength;
		}

		if(nMode&(LM_MRUWIDTH|LM_COMMIT))
			return m_szFloated;

		return _CalcDesiredLayout(nMode&LM_HORZ);
	}

	if( nLength >= 0 )
		return _CalcDesiredLayout(nMode&LM_HORZ);

	if(((CTDockBar *) m_pDockBar)->m_bLayoutQuery)
		return _CalcDesiredLayout(nMode&LM_HORZ);

	int nCalcExtentMax = ((CFriendlyDockBar *) m_pDockBar)->GetCalcExtentMax(nMode&LM_HORZ);
	if(!nCalcExtentMax)
		return _CalcDesiredLayout(nMode&LM_HORZ);

	int nCalcExtentMin = (nMode&LM_HORZ) ? afxData.cyBorder2 : afxData.cxBorder2;
	int nCalcExtentDiff = 0;
	int nCalcExtentReal = 0;
	int nMaxWidth = 0;

	int nCount = m_pDockBar->m_arrBars.GetSize();
	int nReviewStart = -1;
	int nReviewStop = -1;
	int nThisIdx = -1;

	int nCountBarsInRowReal = 0;
	int nCountOfMinSizedBars = 0;
	int nCountInRowReal = 0;

	for( int i=0; i<nCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(pBar)
		{
			if( nThisIdx >= 0 )
			{
				nReviewStop = i - 1;
				break;
			}

			nCountBarsInRowReal = 0;
			nReviewStart = i + 1;

			nCountInRowReal = 0;
			nReviewStop = -1;
			nMaxWidth = 0;

			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(!pBar->IsVisible())
			continue;

		if( pBar == this )
		{
			if( nCountBarsInRowReal > 0 )
				return _CalcDesiredLayout(nMode&LM_HORZ);

			nThisIdx = i;
		}

		CRect rcBarWnd;
		pBar->GetWindowRect(&rcBarWnd);

		CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			pBar);

		if( pControlBar && pControlBar->IsFixedMode() )
			pControlBar = NULL;

		if( pControlBar != NULL )
		{
			nCountBarsInRowReal++;
			nMaxWidth = max( nMaxWidth, (nMode&LM_HORZ) ? pControlBar->m_szDockedH.cy : pControlBar->m_szDockedV.cx);
		}

		nCountInRowReal++;
	}

	if( nCountInRowReal == 1 )
	{
		if(nMode&LM_HORZ)
			m_szDockedH.cx = max( _CalcDesiredMinHW(), nCalcExtentMax);
		else
			m_szDockedV.cy = max( _CalcDesiredMinVH(), nCalcExtentMax);
	}
	else
	{
		for( i=nReviewStart; i<=nReviewStop; i++)
		{
			CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

			if(__PLACEHODLER_BAR_PTR(pBar))
				continue;

			if(!pBar->IsVisible())
				continue;

			CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
				CTControlBar,
				pBar);

			if( pControlBar && pControlBar->IsFixedMode() )
				pControlBar = NULL;

			int nCalcExtentBar = 0;
			if(!pControlBar)
			{
				CRect rcBar;

				pBar->GetWindowRect(&rcBar);
				nCalcExtentBar = (nMode&LM_HORZ) ? rcBar.Width() : rcBar.Height();
			}
			else
			{
				nCalcExtentBar = (nMode&LM_HORZ) ? pControlBar->_CalcDesiredMinHW() : pControlBar->_CalcDesiredMinVH();
				LONG &ref_nCalcExtentBar = (nMode&LM_HORZ) ? pControlBar->m_szDockedH.cx : pControlBar->m_szDockedV.cy;

				if( ref_nCalcExtentBar < nCalcExtentBar )
					ref_nCalcExtentBar = nCalcExtentBar;
			}

			nCalcExtentMin += nCalcExtentBar;
			if( nCalcExtentMin <= nCalcExtentMax )
			{
				if(!pControlBar)
				{
					nCalcExtentMax -= nCalcExtentBar;
					continue;
				}

				nCalcExtentReal += (nMode & LM_HORZ) ? pControlBar->m_szDockedH.cx : pControlBar->m_szDockedV.cy;
				continue;
			}

			if( i < nThisIdx )
			{
				((CFriendlyDockBar *) m_pDockBar)->_InsertRowSplitter(i + 1);
				return _CalcDesiredLayout(nMode&LM_HORZ);
			}

			if( i == nThisIdx )
			{
				if(nMode&LM_HORZ)
					m_szDockedH.cx = max( _CalcDesiredMinHW(), nCalcExtentMax);
				else
					m_szDockedV.cy = max( _CalcDesiredMinVH(), nCalcExtentMax);

				break;
			}

			((CFriendlyDockBar *) m_pDockBar)->_InsertRowSplitter(i);
			nReviewStop = i - 1;

			break;
		}

		nCountOfMinSizedBars = 0;
		for( i=nReviewStart; i<=nReviewStop; i++)
		{
			CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

			if(__PLACEHODLER_BAR_PTR(pBar))
				continue;

			if(!pBar->IsVisible())
				continue;

			CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
				CTControlBar,
				pBar);

			if( pControlBar && pControlBar->IsFixedMode() )
				pControlBar = NULL;

			if(!pControlBar)
				continue;

			LONG nMinMetric = (nMode&LM_HORZ) ? pControlBar->_CalcDesiredMinHW() : pControlBar->_CalcDesiredMinVH();
			LONG nCalcExtentBar = (nMode&LM_HORZ) ? pControlBar->m_szDockedH.cx : pControlBar->m_szDockedV.cy;

			if( nMinMetric == nCalcExtentBar )
				nCountOfMinSizedBars++;
		}

		nCalcExtentDiff = nCalcExtentMax - nCalcExtentReal;
		if( nCountOfMinSizedBars != nCountBarsInRowReal )
		{
			int nCalcStep = nCalcExtentDiff > 0 ? 1 : -1;
			int nPassMax = 2 * abs(nCalcExtentDiff);
			int nPass = 0;

			i = nReviewStart;
			while(nCalcExtentDiff != 0 && nCountOfMinSizedBars != nCountBarsInRowReal && nPass < nPassMax)
			{
				if( i == nReviewStop + 1 )
				{
					nPass++;
					i = nReviewStart;
				}

				CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];
				if(!pBar)
				{
					i++;
					continue;
				}

				if(__PLACEHODLER_BAR_PTR(pBar))
				{
					i++;
					continue;
				}

				if(!pBar->IsVisible())
				{
					i++;
					continue;
				}

				CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
					CTControlBar,
					pBar);

				if( !pControlBar || pControlBar->IsFixedMode() )
				{
					i++;
					continue;
				}

				if(nMode&LM_HORZ)
					pControlBar->m_szDockedH.cy = nMaxWidth;
				else
					pControlBar->m_szDockedV.cx = nMaxWidth;

				LONG nMinMetric = (nMode&LM_HORZ) ? pControlBar->_CalcDesiredMinHW() : pControlBar->_CalcDesiredMinVH();
				LONG &ref_nCalcExtentBar = (nMode&LM_HORZ) ? pControlBar->m_szDockedH.cx : pControlBar->m_szDockedV.cy;

				if( ref_nCalcExtentBar == nMinMetric )
				{
					i++;
					continue;
				}

				ref_nCalcExtentBar += nCalcStep;
				nCalcExtentDiff -= nCalcStep;

				if( ref_nCalcExtentBar == nMinMetric )
					nCountOfMinSizedBars++;

				i++;
			}
		}
		else
		{
			CTControlBar *pLastBar = (CTControlBar *) _GetFirstControlBarInRow(
				this,
				TRUE,
				TRUE,
				TRUE);

			while(!pLastBar->IsVisible())
			{
				CTControlBar *pTemp = (CTControlBar *) _GetNextControlBarInRow(
					pLastBar,
					FALSE,
					TRUE,
					TRUE);

				if(!pTemp)
					break;

				pLastBar = pTemp;
			}

			LONG &ref_nCalcExtentBar = (nMode&LM_HORZ) ? pLastBar->m_szDockedH.cx : pLastBar->m_szDockedV.cy;
			ref_nCalcExtentBar += nCalcExtentDiff;
			nCalcExtentReal = nCalcExtentMax;
		}
	}

	CRect rcWndDockBar;
	m_pDockBar->GetWindowRect(&rcWndDockBar);

	CPoint ptDevOffset = -rcWndDockBar.TopLeft();
	int nCalcBarOffset = 0;
	int nHdwpCount = nReviewStop - nReviewStart + 1;

	HANDLE hDWP = ::BeginDeferWindowPos(nHdwpCount);
	static const UINT nDwpFlags = SWP_NOZORDER|
		SWP_NOOWNERZORDER|
		SWP_NOACTIVATE;

	for( i = nReviewStart; i<=nReviewStop; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if( !pBar->IsVisible() )
			continue;

		CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			pBar);

		if( pControlBar && pControlBar->IsFixedMode() )
			pControlBar = NULL;

		CRect rcWndBarOrg;
		pBar->GetWindowRect(&rcWndBarOrg);

		CRect rcWndBar(rcWndBarOrg);
		rcWndBar.OffsetRect(ptDevOffset);

		if(pControlBar)
		{
			rcWndBar = CRect(
				rcWndBar.TopLeft(),
				nMode&LM_HORZ ? pControlBar->m_szDockedH : pControlBar->m_szDockedV);
		}

		int nBarRealOffset = nMode&LM_HORZ ? rcWndBar.left : rcWndBar.top;
		if( nBarRealOffset != nCalcBarOffset )
		{
			if(!(nMode&LM_HORZ))
				rcWndBar.OffsetRect( 0, nCalcBarOffset - rcWndBar.top);
			else
				rcWndBar.OffsetRect( nCalcBarOffset - rcWndBar.left, 0);
		}

		if( rcWndBar != rcWndBarOrg )
		{
			hDWP = ::DeferWindowPos(
				hDWP,
				pBar->GetSafeHwnd(),
				NULL,
				rcWndBar.left,
				rcWndBar.top,
				rcWndBar.Width(),
				rcWndBar.Height(),
				nDwpFlags);
		}

		nCalcBarOffset += nMode&LM_HORZ ? rcWndBar.Width() : rcWndBar.Height();
	}
	::EndDeferWindowPos(hDWP);

	return _CalcDesiredLayout(nMode&LM_HORZ);
}

int CTControlBar::_CalcDesiredMinHW() const
{
	int nCalcVal = m_nMinHW + m_rcRowRecalcUp.Width();

	if(m_bGripperStaticallyAtTop)
		nCalcVal = max( nCalcVal, 10);

	return nCalcVal;
}

int CTControlBar::_CalcDesiredMinVH() const
{
	int nCalcVal = m_nMinVH + m_rcRowRecalcUp.Height();
	return nCalcVal;
}

CSize CTControlBar::_CalcDesiredMinFloatedSize() const
{
	return CSize( 80, 60);
}

CSize CTControlBar::_CalcDesiredLayout( BOOL bHorz, BOOL bForceFullRow)
{
	if(IsFloating())
		return m_szFloated;

	if(bHorz)
	{
		if(bForceFullRow)
			return CSize( MAXSHORT, m_szDockedH.cy);

		if( m_szDockedH.cy < _CalcDesiredMinHW() )
			m_szDockedH.cy = _CalcDesiredMinHW();

		return m_szDockedH;
	}
	else
	{
		if(bForceFullRow)
			return CSize( m_szDockedH.cx, MAXSHORT);

		if( m_szDockedV.cx < _CalcDesiredMinVH() )
			m_szDockedV.cx = _CalcDesiredMinVH();

		return m_szDockedV;
	}
}

void CTControlBar::DoPaint( CDC *pDC)
{
	CRect rcClient;

	GetClientRect(&rcClient);
	if( rcClient.IsRectEmpty() || !pDC->RectVisible(&rcClient) )
		return;

	CTPaintManager::ExcludeChildAreas(
		*pDC,
		*this);

	COLORREF clrBack = g_PaintManager->GetColor(CTPaintManager::CLR_3DFACE_OUT);
	pDC->FillSolidRect(
		&rcClient,
		clrBack);
}

BOOL CTControlBar::DoCustomModePreTranslateMessage( CWnd *pWndPretranslateSrc, MSG *pMsg)
{
	HWND hWndPretranslateSrc = pWndPretranslateSrc->GetSafeHwnd();

	if(	hWndPretranslateSrc == NULL || !::IsWindow(hWndPretranslateSrc) )
		return FALSE;

	for( HWND hWndChild = ::GetWindow( hWndPretranslateSrc, GW_CHILD);
		hWndChild && ::IsWindow(hWndPretranslateSrc);
		hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT))
	{
		CWnd *pWndChild = CWnd::FromHandlePermanent(hWndChild);

		if(!pWndChild)
			continue;

		CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			pWndChild);

		if(!pControlBar)
			continue;

		if(pControlBar->PreTranslateMessage(pMsg))
			return TRUE;
	}

	return FALSE;
}

void CTControlBar::DoCustomModeUpdateControlBars( CWnd *pWndParentTarget, BOOL bDisableIfNoHndler)
{
	HWND hWndUpdateTarget = pWndParentTarget->GetSafeHwnd();

	for( HWND hWndChild = ::GetWindow( hWndUpdateTarget, GW_CHILD);
		hWndChild && ::IsWindow(hWndUpdateTarget);
		hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT))
	{
		CWnd *pWndChild = CWnd::FromHandlePermanent(hWndChild);

		if(!pWndChild)
			continue;

		CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
			CTControlBar,
			pWndChild);

		if(!pControlBar)
			continue;

		pControlBar->DoCustomModeUpdateCmdUI(bDisableIfNoHndler);
	}
}

BOOL CTControlBar::DoCustomModeUpdateCmdUI( BOOL bDisableIfNoHndler)
{
	if(m_bInCustomModeUpdateCmdUI)
		return TRUE;

	HWND hWndOwn = GetSafeHwnd();
	if( !hWndOwn || !::IsWindow(hWndOwn) )
		return FALSE;

	m_bInCustomModeUpdateCmdUI = TRUE;
	CFrameWnd *pWndUpdateTarget = NULL;

	if(!m_bPresubclassDialogMode)
		pWndUpdateTarget = _GetDockingFrameImpl();

	OnUpdateCmdUI(
		pWndUpdateTarget,
		bDisableIfNoHndler);

	m_bInCustomModeUpdateCmdUI = FALSE;
	return TRUE;
}

void CTControlBar::OnUpdateCmdUI( CFrameWnd *pTarget,
								  BOOL bDisableIfNoHndler)
{
	if( m_bPresubclassDialogMode ||
		m_bDragging ||
		m_bRowRecalcing ||
		m_bRowResizing ||
		CTControlBar::_DraggingGetBar())
		return;

	if(!(GetStyle()&WS_VISIBLE))
		return;

	BOOL bNeedRepaint = FALSE;
	CPoint ptScreen;
	::GetCursorPos(&ptScreen);

	UINT nHT = NcButtons_HitTest(ptScreen);
	if( nHT == HTCLIENT )
	{
		if(m_bNcAreaBtnTracking)
			bNeedRepaint = TRUE;

		m_bNcAreaBtnTracking = FALSE;
	}
	else
	{
		if(!m_bNcAreaBtnTracking)
			bNeedRepaint = TRUE;

		m_bNcAreaBtnTracking = TRUE;
	}

	BOOL bWindowActiveOldVal = m_bWindowActive;
	HWND hTmpWndFocus = ::GetFocus();

	m_bWindowActive = hTmpWndFocus && (
		GetSafeHwnd() == hTmpWndFocus ||
		::IsChild( GetSafeHwnd(), hTmpWndFocus));

	if( bWindowActiveOldVal != m_bWindowActive )
		bNeedRepaint = TRUE;

	if(bNeedRepaint)
	{
		SendMessage(WM_NCPAINT);
		GetParent()->SendMessage(WM_NCPAINT);
	}
}

void CTControlBar::_RecalcNcArea()
{
	if(!IsVisible())
		return;

	SetWindowPos(
		NULL, 0,
		0, 0, 0,
		SWP_NOSIZE|
		SWP_NOMOVE|
		SWP_NOZORDER|
		SWP_NOOWNERZORDER|
		SWP_NOREPOSITION|
		SWP_FRAMECHANGED);
}

void CTControlBar::_RecalcLayoutImpl()
{
	if(!m_bReflectParentSizing)
		return;

	CWnd *pWndParent = NULL;
	if(!m_bPresubclassDialogMode)
		pWndParent = GetParentFrame();
	else
		return;

	if( pWndParent && pWndParent->GetSafeHwnd() )
	{
		WINDOWPLACEMENT wp;

		ZeroMemory( &wp, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);

		pWndParent->GetWindowPlacement(&wp);
		if( wp.showCmd != SW_SHOWMINIMIZED )
		{
			if( IsFixedMode() && IsFloating() )
				((CFrameWnd *) pWndParent)->RecalcLayout();
			else
				((CFrameWnd *) pWndParent)->DelayRecalcLayout();
		}
	}
}

LRESULT CTControlBar::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE			: m_bInCustomModeUpdateCmdUI = TRUE; return 0;
	case WM_DESTROY			: m_bInCustomModeUpdateCmdUI = TRUE; break;
	case WM_NCDESTROY		:
		{
			m_bInCustomModeUpdateCmdUI = TRUE;
			NcButtons_RemoveAll();
		}

		break;

	case WM_SETFOCUS		:
		{
			LRESULT lResult = CControlBar::WindowProc( message, wParam, lParam);

			if(IsMinimizedOnRow())
				MaximizeOnRow();

			if(!IsFixedMode())
			{
				CWnd *pWnd = GetWindow(GW_CHILD);

				if(pWnd)
					pWnd->SetFocus();
			}

			return lResult;
		}

	case WM_ERASEBKGND		:
		{
			if(!m_bDoNotEraseClientBackground)
				return 1;

			CTPaintManager::ExcludeChildAreas(
				(HDC) wParam,
				*this);

			return CControlBar::WindowProc( message, wParam, lParam);
		}

	case WM_CREATE			:
		{
			if( !m_bPresubclassDialogMode && !IsFixedMode() )
			{
				HWND hWndParent = ::GetParent(GetSafeHwnd());
				CFrameWnd *pFrame = NULL;

				while(!pFrame)
				{
					CWnd *pWnd = CWnd::FromHandle(hWndParent);

					if(pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
					{
						pFrame = (CFrameWnd *) pWnd;
						break;
					}
				}

				if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
					pFrame = pFrame->GetParentFrame();

				_FrameEnableDockingImpl(pFrame);
			}
		}

		break;

	case WM_WINDOWPOSCHANGED	:
		{
			if( !m_pDockBar && !m_bPresubclassDialogMode )
				break;

			LPWINDOWPOS lpWindowPos = reinterpret_cast <LPWINDOWPOS> (lParam);
			if(!IsFixedMode())
				_UpdateVisibilityInChain();

			if(lpWindowPos->flags&SWP_FRAMECHANGED)
				OnNcAreaButtonsReposition();
			else
				_RecalcNcArea();
		}

		break;

	case WM_WINDOWPOSCHANGING	:
		{
			if( !m_pDockBar && !m_bPresubclassDialogMode )
				break;

			LPWINDOWPOS lpWindowPos = reinterpret_cast <LPWINDOWPOS> (lParam);
			lpWindowPos->flags |= SWP_FRAMECHANGED;
		}

		break;
	}

	return CControlBar::WindowProc( message, wParam, lParam);
}

void CTControlBar::NcButtons_Paint( CDC& dc)
{
	int nCount = NcButtons_GetCount();

	for( int i=0; i<nCount; i++)
	{
		CTBarNcAreaButton *pBtn = NcButtons_GetAt(i);

		if(!pBtn->OnQueryVisibility())
			continue;

		pBtn->OnNcAreaDraw(dc);
	}
}

CRect& CTControlBar::NcButtons_CalcGripperTextLocation( CRect& rcPreCalcText)
{
	int nCount = NcButtons_GetCount();

	if(!nCount)
		return rcPreCalcText;

	BOOL bHorz = FALSE;
	BOOL bGripperAtTop = TRUE;

	if(!IsFloating())
	{
		if(!IsBarWithGripper(&bGripperAtTop))
			return rcPreCalcText;

		bHorz = IsDockedHorizontally();
	}

	CTBarNcAreaButton *pBtn = NULL;
	for( int i=nCount; i>0; i--)
	{
		CTBarNcAreaButton *pBtnExamine = NcButtons_GetAt(i - 1);

		if(pBtnExamine->OnQueryVisibility())
		{
			pBtn = pBtnExamine;
			break;
		}
	}

	if(!pBtn)
		return rcPreCalcText;

	const CRect& rcBtn = *pBtn;
	if( bHorz && !bGripperAtTop )
	{
		rcPreCalcText.top = rcBtn.bottom + 1;

		if( rcPreCalcText.top > rcPreCalcText.bottom )
			rcPreCalcText.top = rcPreCalcText.bottom;
	}
	else
	{
		rcPreCalcText.right = rcBtn.left - 1;

		if( rcPreCalcText.left > rcPreCalcText.right )
			rcPreCalcText.left = rcPreCalcText.right;
	}

	return rcPreCalcText;
}

UINT CTControlBar::NcButtons_HitTest( CPoint point,
									  CTBarNcAreaButton **ppBtn)
{
	if(ppBtn)
		*ppBtn = NULL;

	CWnd *pWndNcRepaint = this;
	BOOL bNeedRepaint = FALSE;
	CRect rcBarWnd;

	if(IsFloating())
	{
		CTMiniDockFrameWnd *pFrame = DYNAMIC_DOWNCAST(
			CTMiniDockFrameWnd,
			GetParentFrame());

		if(!pFrame)
			return HTCLIENT;

		pFrame->GetWindowRect(&rcBarWnd);
		pWndNcRepaint = pFrame;
	}
	else
		GetWindowRect(&rcBarWnd);

	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	CPoint ptWnd(point + ptDevOffset);

	BOOL bDragging = m_bDragging;
	if(!bDragging)
		bDragging = (BOOL) _DraggingGetBar();

	int nCount = NcButtons_GetCount();
	UINT nFirstHT = HTCLIENT;

	for( int i=0; i<nCount; i++)
	{
		CTBarNcAreaButton *pBtn = NcButtons_GetAt(i);

		if(!pBtn->OnQueryVisibility())
			continue;

		if(( pBtn->m_bHover || pBtn->m_bPushed) && ppBtn == NULL )
		{
			bNeedRepaint = TRUE;

			if( bDragging || m_bRowRecalcing || m_bRowResizing )
			{
				pBtn->m_bPushed = FALSE;
				pBtn->m_bHover = FALSE;
				continue;
			}
		}

		UINT nHT = pBtn->OnNcAreaHitTest(ptWnd);
		if( nHT != HTCLIENT )
		{
			if(ppBtn)
			{
				*ppBtn = pBtn;
				return nHT;
			}

			nFirstHT = nHT;
			bNeedRepaint = TRUE;
		}
	}

	if(bNeedRepaint)
		pWndNcRepaint->SendMessage(WM_NCPAINT);

	return nFirstHT;
}

UINT CTControlBar::OnNcHitTest( CPoint point)
{
	UINT nHT = NcButtons_HitTest(point);

	if( nHT != HTCLIENT )
	{
		m_bNcAreaBtnTracking = TRUE;
		return nHT;
	}

	if( IsFixedMode() || IsFloating() )
		return HTCLIENT;

	CRect rcBarWnd;
	GetWindowRect(&rcBarWnd);

	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	CRect rcBarClient;

	GetClientRect(&rcBarClient);
	ClientToScreen(&rcBarClient);

	if( rcBarWnd == rcBarClient )
		return HTCLIENT;

	BOOL bHorz = IsDockedHorizontally();
	point += ptDevOffset;

	if( !m_rcRowResize.IsRectEmpty() && m_rcRowResize.PtInRect(point) )
		return bHorz ? HTBOTTOM : HTRIGHT;

	if( !m_rcRowRecalc.IsRectEmpty() && m_rcRowRecalc.PtInRect(point) )
	{
		if(_IsLastBarInRow(TRUE))
			return HTCLIENT;

		if(!_RowRecalcing_IsBottomEnabled())
			return HTCLIENT;

		return bHorz ? HTRIGHT : HTBOTTOM;
	}

	if( !m_rcRowRecalcUp.IsRectEmpty() && m_rcRowRecalcUp.PtInRect(point) )
	{
		if(_IsFirstBarInRow(TRUE))
			return HTCLIENT;

		if(!_RowRecalcing_IsTopEnabled())
			return HTCLIENT;

		return bHorz ? HTLEFT : HTTOP;
	}

	return HTCLIENT;
}

void CTControlBar::_GetDeepNcModeMetrics( BOOL& bEnableResizerLeft,
										  BOOL& bEnableResizerTop,
										  BOOL& bEnableResizerRight,
										  BOOL& bEnableResizerBottom)
{
	bEnableResizerLeft = TRUE;
	bEnableResizerTop = TRUE;
	bEnableResizerRight = TRUE;
	bEnableResizerBottom = TRUE;

	if(IsFixedMode())
		return;

	if(!IsVisible())
		return;

	if( m_pDockSite == NULL || m_pDockBar == NULL )
		return;

	if(!m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		return;

	CTDockBar *pDockBar = (CTDockBar *) m_pDockBar;
	UINT nDockBarID = pDockBar->GetDlgCtrlID();
	BOOL bHorz = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
	int nCount = pDockBar->m_arrBars.GetSize();

	if( nCount == 1 )
		return;

	int nOwnIdx = pDockBar->FindBar(this);
	if( nOwnIdx == -1 )
		return;

	BOOL bFirstVisibleOnRow = TRUE;
	BOOL bPrevRowExist = FALSE;

	for( int i = nOwnIdx - 1; i > 0; i--)
	{
		CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];

		if(!pBar)
		{
			if( i > 0 )
				bPrevRowExist = TRUE;

			break;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(pBar->IsVisible())
			bFirstVisibleOnRow = FALSE;
	}

	BOOL bPrevVisibleRowExist = FALSE;
	if(bPrevRowExist)
	{
		for( i--; i>0; i--)
		{
			CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];

			if( !pBar || __PLACEHODLER_BAR_PTR(pBar) )
				continue;

			if(pBar->IsVisible())
			{
				bPrevVisibleRowExist = TRUE;
				break;
			}
		}
	}

	BOOL bLastVisibleOnRow = TRUE;
	BOOL bNextRowExist = FALSE;

	for( i = nOwnIdx + 1; i<nCount; i++)
	{
		CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];

		if(!pBar)
		{
			if( i < nCount - 1 )
				bNextRowExist = TRUE;

			break;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if(pBar->IsVisible())
			bLastVisibleOnRow = FALSE;
	}

	BOOL bNextVisibleRowExist = FALSE;
	if(bNextRowExist)
	{
		for( i++; i<nCount; i++)
		{
			CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];

			if( !pBar || __PLACEHODLER_BAR_PTR(pBar) )
				continue;

			if(pBar->IsVisible())
			{
				bNextVisibleRowExist = TRUE;
				break;
			}
		}
	}

	if(m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
	{
		if(bHorz)
		{
			if(!bPrevVisibleRowExist)
				bEnableResizerTop = FALSE;

			if(!bNextVisibleRowExist)
				bEnableResizerBottom = FALSE;

			if(bFirstVisibleOnRow)
				bEnableResizerLeft = FALSE;

			if(bLastVisibleOnRow)
				bEnableResizerRight = FALSE;
		}
		else
		{
			if(!bPrevVisibleRowExist)
				bEnableResizerLeft = FALSE;

			if(!bNextVisibleRowExist)
				bEnableResizerRight = FALSE;

			if(bFirstVisibleOnRow)
				bEnableResizerTop = FALSE;

			if(bLastVisibleOnRow)
				bEnableResizerBottom = FALSE;
		}
	}
	else
	{
		int nOwnCircleNo = GetDockBarCircleNo();

		if( nOwnCircleNo > 0 )
		{
			switch(nDockBarID)
			{
			case AFX_IDW_DOCKBAR_TOP		:
				{
					if(!bPrevVisibleRowExist)
						bEnableResizerTop = FALSE;

					if(bFirstVisibleOnRow)
						bEnableResizerLeft = FALSE;

					if(bLastVisibleOnRow)
						bEnableResizerRight = FALSE;
				}

				break;

			case AFX_IDW_DOCKBAR_BOTTOM		:
				{
					if(!bNextVisibleRowExist)
						bEnableResizerBottom = FALSE;

					if(bFirstVisibleOnRow)
						bEnableResizerLeft = FALSE;

					if(bLastVisibleOnRow)
						bEnableResizerRight = FALSE;
				}

				break;

			case AFX_IDW_DOCKBAR_LEFT		:
				{
					if(!bPrevVisibleRowExist)
						bEnableResizerLeft = FALSE;

					if(bFirstVisibleOnRow)
						bEnableResizerTop = FALSE;

					if(bLastVisibleOnRow)
						bEnableResizerBottom = FALSE;
				}

				break;

			case AFX_IDW_DOCKBAR_RIGHT		:
				{
					if(!bNextVisibleRowExist)
						bEnableResizerRight = FALSE;

					if(bFirstVisibleOnRow)
						bEnableResizerTop = FALSE;

					if(bLastVisibleOnRow)
						bEnableResizerBottom = FALSE;
				}

				break;
			}
		}
	}
}

void CTControlBar::OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if( !m_pDockBar && !m_bPresubclassDialogMode )
		return;

	if(IsFloating())
		return;

	BOOL bEnableResizerLeft = TRUE;
	BOOL bEnableResizerTop = TRUE;
	BOOL bEnableResizerRight = TRUE;
	BOOL bEnableResizerBottom = TRUE;

	_GetDeepNcModeMetrics(
		bEnableResizerLeft,
		bEnableResizerTop,
		bEnableResizerRight,
		bEnableResizerBottom);

	CRect& rcClientLoc = reinterpret_cast < CRect &> (lpncsp->rgrc[0]);
	CRect rcDevOrg(rcClientLoc);

	m_rcGrip.SetRectEmpty();
	m_rcRowResize.SetRectEmpty();
	m_rcRowRecalc.SetRectEmpty();
	m_rcRowRecalcUp.SetRectEmpty();

	m_nMinHW = m_nSeparatorWidth;
	m_nMinVH = m_nSeparatorHeight;

	BOOL bGripperAtTop = FALSE;
	BOOL bGripperExists = IsBarWithGripper(&bGripperAtTop);

	CPoint ptGripOffset( 0, 0);
	UINT nDockBarDlgCtrlID = GetSafeDockBarDlgCtrlID();

	if(!IsFixedMode())
	{
		MFCControlBarVector vBars;

		_GetRowBars(vBars);
		int nCount = vBars.GetSize();
		int nLastFixedIdx = -1;

		CTControlBar *pLastBar = NULL;
		BOOL bAddTopRecalc = FALSE;

		for( int i=0; i<nCount; i++)
		{
			CControlBar *pBar = vBars[i];

			if(!pBar->IsVisible())
				continue;

			CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
				CTControlBar,
				pBar);

			if( pControlBar && pControlBar->IsFixedMode() )
				pControlBar = NULL;

			if(pControlBar)
			{
				if( pControlBar == this &&
					pLastBar &&
					nLastFixedIdx == i - 1 )
				{
					bAddTopRecalc = TRUE;
					break;
				}

				pLastBar = pControlBar;
			}
			else
				nLastFixedIdx = i;
		}

		switch(nDockBarDlgCtrlID)
		{
		case AFX_IDW_DOCKBAR_TOP		:
			{
				if(bEnableResizerRight)
				{
					m_rcRowRecalc = rcClientLoc;
					m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
					rcClientLoc.right -= m_nSeparatorWidth;
				}

				if(bEnableResizerBottom)
				{
					rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight);
					m_rcRowResize = rcClientLoc;
					m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowResize.top = m_rcRowResize.bottom;
					m_rcRowResize.bottom += m_nSeparatorHeight;
					m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();
				}

				if(bAddTopRecalc)
				{
					m_rcRowRecalcUp = rcClientLoc;
					m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;

					if(bGripperExists)
						ptGripOffset.x += m_nSeparatorWidth;

					rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0);
				}
			}

			break;

		case AFX_IDW_DOCKBAR_BOTTOM		:
			{
				if(bEnableResizerRight)
				{
					m_rcRowRecalc = rcClientLoc;
					m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalc.left = m_rcRowRecalc.right - m_nSeparatorWidth;
					rcClientLoc.right -= m_nSeparatorWidth;
				}

				if(bEnableResizerTop)
				{
					rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0);
					m_rcRowResize = rcClientLoc;
					m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowResize.bottom = m_rcRowResize.top + m_nSeparatorHeight;
					m_rcRowResize.right = m_rcRowResize.left + rcDevOrg.Width();

					if(bGripperExists)
						ptGripOffset.y += m_nSeparatorHeight;
				}

				if(bAddTopRecalc)
				{
					m_rcRowRecalcUp = rcClientLoc;
					m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalcUp.right = m_rcRowRecalcUp.left + m_nSeparatorWidth;

					if(bGripperExists)
						ptGripOffset.x += m_nSeparatorWidth;
					rcClientLoc.DeflateRect( m_nSeparatorWidth, 0, 0, 0);
				}
			}

			break;

		case AFX_IDW_DOCKBAR_LEFT		:
			{
				if(bEnableResizerBottom)
				{
					m_rcRowRecalc = rcClientLoc;
					m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;
					rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight);
				}

				if(bEnableResizerRight)
				{
					rcClientLoc.DeflateRect( 0, 0, m_nSeparatorHeight, 0);
					m_rcRowResize = rcClientLoc;
					m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowResize.left = m_rcRowResize.right;
					m_rcRowResize.right += m_nSeparatorWidth;
					m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();
				}

				if(bAddTopRecalc)
				{
					m_rcRowRecalcUp = rcClientLoc;
					m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;

					if(bGripperExists)
						ptGripOffset.y += m_nSeparatorHeight;
					rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0);
				}
			}

			break;

		case AFX_IDW_DOCKBAR_RIGHT		:
			{
				if(bEnableResizerBottom)
				{
					m_rcRowRecalc = rcClientLoc;
					m_rcRowRecalc.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalc.top = m_rcRowRecalc.bottom - m_nSeparatorHeight;
					rcClientLoc.DeflateRect( 0, 0, 0, m_nSeparatorHeight );
				}

				if(bEnableResizerLeft)
				{
					rcClientLoc.DeflateRect( m_nSeparatorHeight, 0, 0, 0);
					m_rcRowResize = rcClientLoc;
					m_rcRowResize.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowResize.right = m_rcRowResize.left + m_nSeparatorWidth;
					m_rcRowResize.bottom = m_rcRowResize.top + rcDevOrg.Height();

					if(bGripperExists)
						ptGripOffset.x += m_nSeparatorWidth;
				}

				if(bAddTopRecalc)
				{
					m_rcRowRecalcUp = rcClientLoc;
					m_rcRowRecalcUp.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);
					m_rcRowRecalcUp.bottom = m_rcRowRecalcUp.top + m_nSeparatorWidth;

					if(bGripperExists)
						ptGripOffset.y += m_nSeparatorHeight;
					rcClientLoc.DeflateRect( 0, m_nSeparatorHeight, 0, 0);
				}
			}

			break;
		}
	}
	else if(IsDockedVertically())
	{
		rcClientLoc.DeflateRect(
			__TB_LINE_OFFSET / 4,
			__TB_LINE_OFFSET / 2);
	}
	else
	{
		rcClientLoc.DeflateRect(
			__TB_LINE_OFFSET / 2,
			__TB_LINE_OFFSET / 4);
	}

	if(bGripperExists)
	{
		m_rcGrip = rcClientLoc;
		m_rcGrip.OffsetRect( -rcClientLoc.left, -rcClientLoc.top);

		if(bGripperAtTop)
		{
			m_rcGrip.bottom = m_rcGrip.top + m_nGripHeightAtTop;
			m_nMinVH += m_rcGrip.Height();

			if(IsFixedMode())
				m_rcGrip.DeflateRect( __BAR_NC_GAP_X, 0);
		}
		else
		{
			m_rcGrip.right = m_rcGrip.left + m_nGripHeightAtTop;
			m_nMinHW += m_rcGrip.Width();

			if(IsFixedMode())
				m_rcGrip.DeflateRect( 0, __BAR_NC_GAP_Y);
		}
		m_rcGrip.OffsetRect(ptGripOffset);

		if(IsFixedMode())
		{
			m_rcGrip.DeflateRect(
				nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_LEFT ? __BAR_NC_GAP_X : 0,
				nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_TOP ? __BAR_NC_GAP_X : 0,
				nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_RIGHT ? __BAR_NC_GAP_X : 0,
				nDockBarDlgCtrlID == AFX_IDW_DOCKBAR_BOTTOM ? __BAR_NC_GAP_X : 0);
		}
	}

	OnNcAreaButtonsReinitialize();
	OnNcAreaButtonsReposition();

	if(bGripperExists)
	{
		rcClientLoc.DeflateRect(
			bGripperAtTop ? 0 : m_nGripWidthAtLeft  + __BAR_NC_GAP_X,
			bGripperAtTop ? m_nGripHeightAtTop  + __BAR_NC_GAP_Y : 0,
			0, 0);
	}

	if( rcClientLoc.bottom < rcClientLoc.top )
		rcClientLoc.bottom = rcClientLoc.top;

	if( rcClientLoc.right < rcClientLoc.left )
		rcClientLoc.right = rcClientLoc.left;
}

void CTControlBar::OnNcPaint()
{
	if( !m_pDockBar && !m_bPresubclassDialogMode )
		return;

	CWindowDC dc(this);
	CRect rcClient;
	CRect rcBar;

	GetClientRect(rcClient);
	ClientToScreen(rcClient);
	GetWindowRect(rcBar);

	rcClient.OffsetRect(-rcBar.TopLeft());
	rcBar.OffsetRect(-rcBar.TopLeft());
	dc.ExcludeClipRect(rcClient);

	CRect rcDraw = rcBar;
	CTMemoryDC mdc( &dc, &rcBar);

	g_PaintManager->PaintDockingFrame(
		mdc,
		rcBar,
		rcClient,
		IsFloating() ? TRUE : FALSE,
		!IsFixedMode(),
		!IsFixedMode());

	BOOL bGripperAtTop = FALSE;
	if(IsBarWithGripper(&bGripperAtTop) && !IsFloating())
	{
		BOOL bHorz = IsDockedHorizontally();
		CRect rcGrip(m_rcGrip);
		CRect rcText(rcGrip);

		if( bHorz && !bGripperAtTop )
		{
			rcText.DeflateRect( 0, 2);
			rcText.OffsetRect( -2, 0);
		}
		else
			rcText.DeflateRect( 2, 0);

		NcButtons_CalcGripperTextLocation(rcText);
		CString strCaption;

		GetWindowText(strCaption);
		CRgn rgnExclude;

		if(rgnExclude.CreateRectRgnIndirect(&rcGrip))
			mdc.SelectClipRgn(&rgnExclude);

		g_PaintManager->PaintGripper(
			mdc,
			rcGrip,
			rcText,
			m_bWindowActive,
			FALSE,
			bHorz && !bGripperAtTop,
			!IsFixedMode(),
			strCaption.IsEmpty() ? NULL : (LPCTSTR) strCaption);

		NcButtons_Paint(mdc);
		mdc.SelectClipRgn(NULL);
	}

	mdc.__Flush();
	ReleaseDC(&dc);
}

void CTControlBar::OnSettingChange( UINT uFlags, LPCTSTR lpszSection)
{
	CControlBar::OnSettingChange( uFlags, lpszSection);
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CTControlBar::OnSysColorChange()
{
	CControlBar::OnSysColorChange();
	g_PaintManager->SyncSysColors();
	g_PaintManager->InitTranslatedColors();
}

void CTControlBar::OnCancelMode()
{
	if(m_bRowResizing)
		_RowResizingStop(TRUE);

	if(m_bRowRecalcing)
		_RowRecalcingStop(TRUE);

	if(m_bDragging)
		_DraggingStop(TRUE);

	CTPopupMenuWnd::CancelMenuTracking();
	CControlBar::OnCancelMode();
}

void CTControlBar::OnCaptureChanged( CWnd *pWnd)
{
	if(m_bRowResizing)
		_RowResizingStop(TRUE);

	if(m_bRowRecalcing)
		_RowRecalcingStop(TRUE);

	CControlBar::OnCaptureChanged(pWnd);
}

void CTControlBar::OnMouseMove( UINT nFlags, CPoint point)
{
	_SetCursor(point);

	if(m_bNcAreaBtnTracking)
		return;

	if(m_bRowResizing)
	{
		_RowResizingUpdateState();
		return;
	}

	if(m_bRowRecalcing)
	{
		_RowRecalcingUpdateState();
		return;
	}

	if(m_bDragging)
	{
		CPoint ptScreen(point);
		ClientToScreen(&ptScreen);
		_DraggingUpdateState(ptScreen);

		return;
	}

	if( IsFloating() && CTMiniDockFrameWnd::m_bAutoMakeTopmostOnMouseEvent &&
		!CTPopupMenuWnd::IsMenuTracking())
	{
		CFrameWnd *pParentFrame = GetParentFrame();

		if(pParentFrame)
			pParentFrame->BringWindowToTop();
	}

	CControlBar::OnMouseMove( nFlags, point);
}

BOOL CTControlBar::_RowRecalcing_IsBottomEnabled()
{
	ControlBarVector vBars;
	_GetRowBars(vBars);

	int nCount = vBars.GetSize();
	BOOL bThisPassed = FALSE;

	CTControlBar *pBarPrev = NULL;
	CTControlBar *pBarNext = NULL;

	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = vBars[i];

		if(!pBar->IsVisible())
			continue;

		if(bThisPassed)
		{
			pBarNext = pBar;
			break;
		}
		else
			pBarPrev = pBar;

		if( pBar == this )
		{
			bThisPassed = TRUE;
			continue;
		}
	}

	BOOL bMinimizedPrev = TRUE;
	BOOL bMinimizedNext = TRUE;

	if(pBarPrev)
		bMinimizedPrev = pBarPrev->IsMinimizedOnRow();

	if(pBarNext)
		bMinimizedNext = pBarNext->IsMinimizedOnRow();

	if( bMinimizedPrev && bMinimizedNext )
		return FALSE;

	return TRUE;
}

BOOL CTControlBar::_RowRecalcing_IsTopEnabled()
{
	ControlBarVector vBars;
	_GetRowBars(vBars);

	CTControlBar *pBarNext = NULL;
	int nCount = vBars.GetSize();

	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = vBars[nCount - i - 1];

		if(!pBar->IsVisible())
			continue;

		if( pBar == this )
			break;

		pBarNext = pBar;
	}

	BOOL bMinimizedNext = TRUE;
	if(pBarNext)
		bMinimizedNext = pBarNext->IsMinimizedOnRow();

	if( bMinimizedNext && IsMinimizedOnRow() )
		return FALSE;

	return TRUE;
}

void CTControlBar::OnNcLButtonDown( UINT nHitTest, CPoint point)
{
	if( IsFixedMode() || IsFloating() )
		return;

	switch(nHitTest)
	{
	case HTLEFT		:
	case HTBOTTOM	: break;
	default			:
		if(!IsFixedMode())
			SetFocus();

		break;
	}

	CRect rcBarClient;
	CRect rcBarWnd;

	GetWindowRect(&rcBarWnd);
	GetClientRect(&rcBarClient);
	ClientToScreen(&rcBarClient);

	if( rcBarWnd == rcBarClient )
		return;

	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	point += ptDevOffset;

	BOOL bHorz = IsDockedHorizontally();
	if( !m_rcRowResize.IsRectEmpty() && m_rcRowResize.PtInRect(point) )
	{
		_RowResizingStart();
		return;
	}

	if( !m_rcRowRecalc.IsRectEmpty() &&
		m_rcRowRecalc.PtInRect(point))
	{
		m_bTopRecalcing = FALSE;

		if(!_RowRecalcing_IsBottomEnabled())
			return;

		_RowRecalcingStart();
		return;
	}

	if( !m_rcRowRecalcUp.IsRectEmpty() &&
		m_rcRowRecalcUp.PtInRect(point))
	{
		if(!_RowRecalcing_IsTopEnabled())
			return;

		m_bTopRecalcing = TRUE;
		_RowRecalcingStart();
		return;
	}
}

void CTControlBar::OnNcLButtonUp( UINT nHitTest, CPoint point) 
{
	if(m_bNcAreaBtnTracking)
	{
		CRect rcBarWnd;
		GetWindowRect(&rcBarWnd);

		CPoint ptDevOffset = -rcBarWnd.TopLeft();
		CPoint ptWnd(point + ptDevOffset);

		int nCount = NcButtons_GetCount();
		for( int i=0; i<nCount; i++)
		{
			CTBarNcAreaButton *pBtn = NcButtons_GetAt(i);

			if(!pBtn->OnQueryVisibility())
				continue;

			if(pBtn->OnNcAreaClicked(ptWnd))
				return;
		}

		return;
	}

	if(m_bRowResizing)
	{
		_RowResizingStop(FALSE);
		return;
	}

	if(m_bRowRecalcing)
	{
		_RowRecalcingStop(FALSE);
		return;
	}

	if(m_bDragging)
		return;

	CControlBar::OnNcLButtonUp( nHitTest, point);
}

void CTControlBar::OnLButtonDown( UINT nFlags, CPoint point)
{
	if( m_pDockBar == NULL ||
		CTPopupMenuWnd::IsMenuTracking())
	{
		CWnd::OnLButtonDown( nFlags, point);
		return;
	}

	ClientToScreen(&point);
	BOOL bExtendedDragging = _IsShowContentWhenDragging();

	if( !bExtendedDragging && IsFixedMode() )
	{
		m_pDockContext->StartDrag(point);
		return;
	}

	if( m_pDockBar && !IsFloating() &&
		!m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		bExtendedDragging = FALSE;

	if(IsFloating())
	{
		CMiniDockFrameWnd *pMiniFrame = DYNAMIC_DOWNCAST(
			CMiniDockFrameWnd,
			GetDockingFrame());

		CTMiniDockFrameWnd *pTMiniFrame = DYNAMIC_DOWNCAST(
			CTMiniDockFrameWnd,
			GetDockingFrame());

		if(pTMiniFrame)
		{
			pTMiniFrame->OnNcLButtonDown(
				HTCAPTION,
				point);

			return;
		}
	}

	if(!IsFixedMode())
		SetFocus();

	CPoint ptOffset(point);
	CRect rcWnd;

	GetWindowRect(&rcWnd);
	ptOffset -= rcWnd.TopLeft();

	_DraggingStart( point, ptOffset);
}

void CTControlBar::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_bRowResizing)
	{
		_RowResizingStop(FALSE);
		return;
	}

	if(m_bRowRecalcing)
	{
		_RowRecalcingStop(FALSE);
		return;
	}

	if(m_bDragging)
	{
		_DraggingStop(FALSE);
		return;
	}

	CControlBar::OnLButtonUp( nFlags, point);
}

void CTControlBar::OnRButtonUp( UINT nFlags, CPoint point)
{
	if( m_bPresubclassDialogMode ||
		!_GetDockingFrameImpl())
	{
		CControlBar::OnRButtonUp( nFlags, point);
		return;
	}

	_ContextMenuTrack();
}

void CTControlBar::OnLButtonDblClk( UINT nFlags, CPoint point)
{
	if(!m_pDockBar)
	{
		CWnd::OnLButtonDblClk( nFlags, point);
		return;
	}

	ToggleDocking();
}

void CTControlBar::OnNcLButtonDblClk( UINT nHitTest, CPoint point)
{
	if( m_pDockBar == NULL || nHitTest != HTCAPTION )
	{
		CWnd::OnNcLButtonDblClk( nHitTest, point);
		return;
	}

	ToggleDocking();
}

void CTControlBar::_ContextMenuTrack()
{
	CFrameWnd *pFrame = _GetDockingFrameImpl();
	CTDockBar *pDockBar = NULL;

	if(m_pDockBar)
	{
		pDockBar = DYNAMIC_DOWNCAST(
			CTDockBar,
			m_pDockBar);
	}

	if(!pDockBar)
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

			if(pDockBar)
				break;
		}
	}

	if(!pDockBar)
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

BOOL CTControlBar::_IsShowContentWhenDragging() const
{
	return FALSE;
}

BOOL CTControlBar::_IsShowContentWhenRowResizing() const
{
	return _IsShowContentWhenDragging();
}

BOOL CTControlBar::_IsShowContentWhenRowRecalcing() const
{
	return _IsShowContentWhenDragging();
}

void CTControlBar::_RowResizingStart()
{
	m_bRowResizing = TRUE;
	SetCapture();

	m_nMetricOffset = IsDockedHorizontally() ? m_szDockedH.cy : m_szDockedV.cx;
	if(!_IsShowContentWhenRowResizing())
	{
		CPoint ptCursorScreen;

		::GetCursorPos(&ptCursorScreen);
		m_nTrackerOffset = IsDockedHorizontally() ? ptCursorScreen.y : ptCursorScreen.x;
		_DrawResizingTracker();
	}

	while(m_bRowResizing)
	{
		::WaitMessage();
		MSG msg;

		while(PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bStop = FALSE;

			switch(msg.message)
			{
			case WM_ACTIVATEAPP		:
			case WM_CANCELMODE		:
			case WM_RBUTTONDOWN		:
			case WM_NCLBUTTONUP		:
			case WM_NCRBUTTONUP		:
			case WM_NCRBUTTONDOWN	:
			case WM_MBUTTONDOWN		:
			case WM_MBUTTONUP		:
			case WM_NCMBUTTONDOWN	:
			case WM_NCMBUTTONUP		:
			case WM_CONTEXTMENU		: bStop = TRUE; break;
			default					:
				if( WM_KEYFIRST <= msg.message &&
					msg.message <= WM_KEYLAST )
					bStop = TRUE;

				break;
			}

			if(bStop)
			{
				_RowResizingStop(TRUE);
				break;
			}

			if(!AfxGetApp()->PumpMessage())
			{
				PostQuitMessage(0);
				break;
			}
		}

		if( m_bRowResizing && CTControlBar::m_bEnableOnIdleCalls )
		{
			LONG lIdle = 0;
			while(AfxGetApp()->OnIdle(lIdle++));
		}
	}
}

void CTControlBar::_RowResizingStop( BOOL bCancel)
{
	m_bRowResizing = FALSE;

	if( GetCapture() == this )
		ReleaseCapture();

	if(!_IsShowContentWhenRowResizing())
	{
		_DrawResizingTracker();

		if(!bCancel)
		{
			BOOL bHorz = IsDockedHorizontally();

			if( m_nMetricOffset != bHorz ? m_szDockedH.cy : m_szDockedV.cx )
			{
				ControlBarVector vBars;
				_GetRowBars(vBars);

				int nCount = vBars.GetSize();
				LONG nDiff = 0;

				for( int i=0; i<nCount; i++)
				{
					CTControlBar *pBar = vBars[i];

					if(pBar->IsFixedMode())
						continue;

					if(bHorz)
					{
						if(!nDiff)
							nDiff = pBar->m_szDockedH.cy - m_nMetricOffset;

						pBar->m_szDockedH.cy = m_nMetricOffset;
					}
					else
					{
						if(!nDiff)
							nDiff = pBar->m_szDockedV.cx - m_nMetricOffset;

						pBar->m_szDockedV.cx = m_nMetricOffset;
					}
				}

				if( nDiff != 0 )
				{
					UINT nDockBarID = GetSafeDockBarDlgCtrlID();
					MFCControlBarVector vNextRow;

					if( nDockBarID == AFX_IDW_DOCKBAR_TOP ||
						nDockBarID == AFX_IDW_DOCKBAR_LEFT )
						_GetNextRowBars( TRUE, vNextRow);
					else
						_GetNextRowBars( FALSE, vNextRow);

					nCount = vNextRow.GetSize();
					for( i=0; i<nCount; i++)
					{
						CTControlBar *pBar = (CTControlBar *) vNextRow[i];

						if( !pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) ||
							pBar->IsFixedMode())
							continue;

						if(bHorz)
							pBar->m_szDockedH.cy += nDiff;
						else
							pBar->m_szDockedV.cx += nDiff;
					}
				}
			}

			GetParentFrame()->RecalcLayout();
		}

		m_pDockBar->RedrawWindow(
			NULL, NULL,
			RDW_ERASE|
			RDW_INVALIDATE|
			RDW_UPDATENOW|
			RDW_ERASENOW|
			RDW_ALLCHILDREN|
			RDW_NOFRAME);
	}
}

void CTControlBar::_RowResizingUpdateState()
{
	CRect rcDockSite;

	m_pDockSite->GetWindowRect(&rcDockSite);
	CSize sizeDockMax = rcDockSite.Size() - CSize(
		_CalcDesiredMinHW(),
		_CalcDesiredMinVH());

	if( sizeDockMax.cx <= 0 ||
		sizeDockMax.cy <= 0 )
		return;

	CPoint ptCursorScreen;
	::GetCursorPos(&ptCursorScreen);

	CPoint ptCursor(ptCursorScreen);
	CRect rcBarWnd;

	GetWindowRect(&rcBarWnd);
	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

	BOOL bHorz = IsDockedHorizontally();
	if(bHorz)
	{
		BOOL bTop = IsDockedAtTop();

		m_nMetricOffset = max(
			2 * _CalcDesiredMinVH(),
			bTop ? ptCursor.y + m_rcRowResize.Height() / 2 : m_szDockedH.cy - ptCursor.y + m_rcRowResize.Height() / 2);

		if( m_nMetricOffset > sizeDockMax.cy )
			m_nMetricOffset = sizeDockMax.cy;

		if( m_nMetricOffset == m_szDockedH.cy )
			return;
	}
	else
	{
		BOOL bLeft = IsDockedAtLeft();

		m_nMetricOffset = max(
			2 * _CalcDesiredMinHW(),
			bLeft ? ptCursor.x + m_rcRowResize.Width() / 2 : m_szDockedV.cx - ptCursor.x + m_rcRowResize.Width() / 2);

		if( m_nMetricOffset > sizeDockMax.cx )
			m_nMetricOffset = sizeDockMax.cx;

		if( m_nMetricOffset == m_szDockedV.cx )
			return;
	}

	BOOL bShowContent = _IsShowContentWhenRowResizing();
	if(bShowContent)
	{
		ControlBarVector vBars;
		_GetRowBars(vBars);

		int nCount = vBars.GetSize();
		LONG nDiff = 0;

		for( int i=0; i<nCount; i++)
		{
			CTControlBar *pBar = vBars[i];

			if(pBar->IsFixedMode())
				continue;

			if(bHorz)
			{
				if(!nDiff)
					nDiff = pBar->m_szDockedH.cy - m_nMetricOffset;

				pBar->m_szDockedH.cy = m_nMetricOffset;
			}
			else
			{
				if(!nDiff)
					nDiff = pBar->m_szDockedV.cx - m_nMetricOffset;

				pBar->m_szDockedV.cx = m_nMetricOffset;
			}
		}

		m_pDockSite->RecalcLayout();
		if(m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			((CTDockBar *) m_pDockBar)->OnDynamicLayoutUpdate();

		return;
	}

	_DrawResizingTracker();
	m_nTrackerOffset = bHorz ? ptCursorScreen.y : ptCursorScreen.x;
	_DrawResizingTracker();
}

void CTControlBar::_RowRecalcingStart()
{
	m_bRowRecalcing = TRUE;
	SetCapture();

	if(!_IsShowContentWhenRowRecalcing())
	{
		CPoint ptCursorScreen;
		::GetCursorPos(&ptCursorScreen);

		m_nTrackerOffset = IsDockedHorizontally() ? ptCursorScreen.x : ptCursorScreen.y;
		m_nMetricOffset = 0;
		_DrawRecalcingTracker();
	}

	while(m_bRowRecalcing)
	{
		::WaitMessage();
		MSG msg;

		while(PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bStop = FALSE;

			switch(msg.message)
			{
			case WM_ACTIVATEAPP		:
			case WM_CANCELMODE		:
			case WM_RBUTTONDOWN		:
			case WM_NCLBUTTONDOWN	:
			case WM_NCRBUTTONUP		:
			case WM_NCRBUTTONDOWN	:
			case WM_MBUTTONDOWN		:
			case WM_MBUTTONUP		:
			case WM_NCMBUTTONDOWN	:
			case WM_NCMBUTTONUP		:
			case WM_CONTEXTMENU		: bStop = TRUE; break;
			default					:
				if(	WM_KEYFIRST <= msg.message &&
					msg.message <= WM_KEYLAST )
					bStop = TRUE;

				break;
			}

			if(bStop)
			{
				_RowRecalcingStop(TRUE);
				break;
			}

			if(!AfxGetApp()->PumpMessage())
			{
				PostQuitMessage(0);
				break;
			}
		}

		if( m_bRowRecalcing && CTControlBar::m_bEnableOnIdleCalls )
		{
			LONG lIdle = 0;
			while(AfxGetApp()->OnIdle(lIdle++));
		}
	}
}

void CTControlBar::_RowRecalcingStop( BOOL bCancel)
{
	m_bRowRecalcing = FALSE;

	if( GetCapture() == this )
		ReleaseCapture();

	if(!_IsShowContentWhenRowRecalcing())
	{
		_DrawRecalcingTracker();

		if(!bCancel)
		{
			if(m_nMetricOffset)
			{
				BOOL bHorz = IsDockedHorizontally();
				ControlBarVector vBars;
				_GetRowBars(vBars);

				int nCount = vBars.GetSize();
				BOOL bThisPassed = FALSE;

				for( int i=0; i<nCount; i++)
				{
					CTControlBar *pBar = vBars[m_bTopRecalcing ? nCount - i - 1 : i];

					if( pBar == this )
					{
						bThisPassed = TRUE;
						continue;
					}

					if(!bThisPassed)
						continue;

					if(bHorz)
					{
						pBar->m_szDockedH.cx += m_nMetricOffset;
						if( pBar->m_szDockedH.cx < pBar->_CalcDesiredMinHW() )
						{
							m_szDockedH.cx -= pBar->_CalcDesiredMinHW() - pBar->m_szDockedH.cx;
							pBar->m_szDockedH.cx = pBar->_CalcDesiredMinHW();
						}
					}
					else
					{
						pBar->m_szDockedV.cy += m_nMetricOffset;
						if( pBar->m_szDockedV.cy < pBar->_CalcDesiredMinVH() )
						{
							m_szDockedV.cy -= pBar->_CalcDesiredMinVH() - pBar->m_szDockedV.cy;
							pBar->m_szDockedV.cy = pBar->_CalcDesiredMinVH();
						}
					}

					break;
				}

				if(bHorz)
				{
					m_szDockedH.cx -= m_nMetricOffset;

					if( m_szDockedH.cx < _CalcDesiredMinHW() )
					{
						m_szDockedH.cx += _CalcDesiredMinHW() - m_szDockedH.cx;
						m_szDockedH.cx = _CalcDesiredMinHW();
					}
				}
				else
				{
					m_szDockedV.cy -= m_nMetricOffset;

					if( m_szDockedV.cy < _CalcDesiredMinVH() )
					{
						m_szDockedV.cy += _CalcDesiredMinVH() - m_szDockedV.cy;
						m_szDockedV.cy = _CalcDesiredMinVH();
					}
				}

				GetParentFrame()->RecalcLayout();
			}
		}

		m_pDockBar->RedrawWindow(
			NULL,
			NULL,
			RDW_ERASE|
			RDW_INVALIDATE|
			RDW_UPDATENOW|
			RDW_ERASENOW|
			RDW_ALLCHILDREN|
			RDW_NOFRAME);
	}
}

void CTControlBar::_RowRecalcingUpdateState()
{
	CPoint ptCursorScreen;
	::GetCursorPos(&ptCursorScreen);

	CPoint ptCursor(ptCursorScreen);
	CRect rcBarWnd;

	GetWindowRect(&rcBarWnd);
	CPoint ptDevOffset = -rcBarWnd.TopLeft();
	ptCursor += ptDevOffset;

	BOOL bSysDragShowContent = _IsShowContentWhenRowRecalcing();
	BOOL bHorz = IsDockedHorizontally();

	if(bHorz)
	{
		if(m_bTopRecalcing)
		{
			int nMetric = max(
				_CalcDesiredMinHW(),
				m_szDockedH.cx - ptCursor.x + m_rcRowRecalcUp.Width() / 2);

			m_nMetricOffset = m_szDockedH.cx - nMetric;
			if(!m_nMetricOffset)
				return;

			if(bSysDragShowContent)
				m_szDockedH.cx = nMetric;
		}
		else
		{
			int nMetric = max(
				_CalcDesiredMinHW(),
				ptCursor.x + m_rcRowRecalc.Width() / 2);

			m_nMetricOffset = m_szDockedH.cx - nMetric;
			if(!m_nMetricOffset)
				return;

			if(bSysDragShowContent)
				m_szDockedH.cx = nMetric;
		}
	}
	else
	{
		if(m_bTopRecalcing)
		{
			int nMetric = max(
				_CalcDesiredMinVH(),
				m_szDockedV.cy - ptCursor.y + m_rcRowRecalcUp.Height() / 2);

			m_nMetricOffset = m_szDockedV.cy - nMetric;
			if(!m_nMetricOffset)
				return;

			if(bSysDragShowContent)
				m_szDockedV.cy = nMetric;
		}
		else
		{
			int nMetric = max(
				_CalcDesiredMinVH(),
				ptCursor.y + m_rcRowRecalc.Height() / 2);

			m_nMetricOffset = m_szDockedV.cy - nMetric;
			if(!m_nMetricOffset)
				return;

			if(bSysDragShowContent)
				m_szDockedV.cy = nMetric;
		}
	}

	if(bSysDragShowContent)
	{
		ControlBarVector vBars;
		_GetRowBars(vBars);

		int nCount = vBars.GetSize();
		BOOL bThisPassed = FALSE;

		for( int i=0; i<nCount; i++)
		{
			CTControlBar *pBar = vBars[m_bTopRecalcing ? nCount - i - 1 : i];

			if( pBar == this )
			{
				bThisPassed = TRUE;
				continue;
			}

			if(!bThisPassed)
				continue;

			if(bHorz)
			{
				pBar->m_szDockedH.cx += m_nMetricOffset;
				if( pBar->m_szDockedH.cx < pBar->_CalcDesiredMinHW() )
				{
					m_szDockedH.cx -= pBar->_CalcDesiredMinHW() - pBar->m_szDockedH.cx;
					pBar->m_szDockedH.cx = pBar->_CalcDesiredMinHW();
				}
			}
			else
			{
				pBar->m_szDockedV.cy += m_nMetricOffset;
				if( pBar->m_szDockedV.cy < pBar->_CalcDesiredMinVH() )
				{
					m_szDockedV.cy -= pBar->_CalcDesiredMinVH() - pBar->m_szDockedV.cy;
					pBar->m_szDockedV.cy = pBar->_CalcDesiredMinVH();
				}
			}

			break;
		}

		m_pDockSite->RecalcLayout();
		if(m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			((CTDockBar *) m_pDockBar)->OnDynamicLayoutUpdate();

		return;
	}

	CRect rcDockBarWnd;
	m_pDockBar->GetWindowRect(&rcDockBarWnd);

	int nMinTrackerOffset = bHorz ? rcDockBarWnd.left : rcDockBarWnd.top;
	if(m_bTopRecalcing)
	{
		MFCControlBarVector vBars;
		_GetRowBars(vBars);

		int nCount = vBars.GetSize();
		for( int i=0; i<nCount; i++)
		{
			CControlBar *pBar = vBars[i];

			if(!pBar->IsVisible())
				continue;

			CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
				CTControlBar,
				pBar);

			if( pControlBar && pControlBar->IsFixedMode() )
				pControlBar = NULL;

			if(!pControlBar)
			{
				CRect rcBarWnd;

				pBar->GetWindowRect(&rcBarWnd);
				nMinTrackerOffset += bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
			else
				nMinTrackerOffset += bHorz ? pControlBar->_CalcDesiredMinHW() : pControlBar->_CalcDesiredMinVH();

			if( pBar == this )
				break;
		}
	}
	else
		nMinTrackerOffset = bHorz ? rcBarWnd.left + _CalcDesiredMinHW() : rcBarWnd.top  + _CalcDesiredMinVH();

	int nMaxTrackerOffset = 0;
	if(!m_bTopRecalcing)
	{
		MFCControlBarVector vBars;
		_GetRowBars(vBars);

		nMaxTrackerOffset = bHorz ? rcDockBarWnd.right : rcDockBarWnd.bottom;
		int nCount = vBars.GetSize();

		for( int i=0; i<nCount; i++)
		{
			CControlBar *pBar = vBars[nCount - i - 1];

			if(!pBar->IsVisible())
				continue;

			if( pBar == this )
				break;

			CTControlBar *pControlBar = DYNAMIC_DOWNCAST(
				CTControlBar,
				pBar);

			if( pControlBar && pControlBar->IsFixedMode() )
				pControlBar = NULL;

			if(!pControlBar)
			{
				CRect rcBarWnd;

				pBar->GetWindowRect(&rcBarWnd);
				nMaxTrackerOffset -= bHorz ? rcBarWnd.Width() : rcBarWnd.Height();
			}
			else
				nMaxTrackerOffset -= bHorz ? pControlBar->_CalcDesiredMinHW() : pControlBar->_CalcDesiredMinVH();
		}
	}
	else
		nMaxTrackerOffset = bHorz ? rcBarWnd.right  - _CalcDesiredMinHW() : rcBarWnd.bottom - _CalcDesiredMinVH();

	_DrawRecalcingTracker();
	m_nTrackerOffset = bHorz ? ptCursorScreen.x : ptCursorScreen.y;

	if( m_nTrackerOffset < nMinTrackerOffset )
		m_nTrackerOffset = nMinTrackerOffset;

	if( m_nTrackerOffset > nMaxTrackerOffset )
		m_nTrackerOffset = nMaxTrackerOffset;

	_DrawRecalcingTracker();
}

void CTControlBar::_DrawResizingTracker()
{
	CRect rcDockBar;
	m_pDockBar->GetWindowRect(&rcDockBar);

	if(rcDockBar.IsRectEmpty())
		return;

	CRect rcTracker(rcDockBar);
	if(IsDockedHorizontally())
	{
		rcTracker.bottom = m_nTrackerOffset + m_rcRowResize.Height() / 2;
		rcTracker.top = m_nTrackerOffset - m_rcRowResize.Height() / 2;
	}
	else
	{
		rcTracker.left = m_nTrackerOffset - m_rcRowResize.Width() / 2;
		rcTracker.right = m_nTrackerOffset + m_rcRowResize.Width() / 2;
	}

	CFrameWnd *pFrame = GetDockingFrame();
	CRect rcDockingFrame;

	pFrame->GetWindowRect(&rcDockingFrame);
	rcTracker.OffsetRect(-rcDockingFrame.TopLeft());

	CDC *pDC = pFrame->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS);
	CBrush *pBrushOld = pDC->SelectObject(CDC::GetHalftoneBrush());

	pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT);

	pDC->SelectObject(pBrushOld);
	pFrame->ReleaseDC(pDC);
}

void CTControlBar::_DrawRecalcingTracker()
{
	CRect rcTracker;
	GetWindowRect(&rcTracker);

	if(IsDockedHorizontally())
	{
		int nMetric = m_bTopRecalcing ? m_rcRowRecalcUp.Width() / 2 : m_rcRowRecalc.Width() / 2;

		rcTracker.left = m_nTrackerOffset - nMetric;
		rcTracker.right = m_nTrackerOffset + nMetric;
	}
	else
	{
		int nMetric = m_bTopRecalcing ? m_rcRowRecalcUp.Height() / 2 : m_rcRowRecalc.Height() / 2;

		rcTracker.top = m_nTrackerOffset - nMetric;
		rcTracker.bottom = m_nTrackerOffset + nMetric;
	}

	CFrameWnd *pFrame = GetDockingFrame();
	CRect rcDockingFrame;

	pFrame->GetWindowRect(&rcDockingFrame);
	rcTracker.OffsetRect(-rcDockingFrame.TopLeft());

	CDC *pDC = pFrame->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS);
	CBrush *pBrushOld = pDC->SelectObject(CDC::GetHalftoneBrush());

	pDC->PatBlt(
		rcTracker.left, rcTracker.top,
		rcTracker.Width(), rcTracker.Height(),
		PATINVERT);

	pDC->SelectObject(pBrushOld);
	pFrame->ReleaseDC(pDC);
}

BOOL CTControlBar::IsOleIpObjActive( CFrameWnd *pFrameSearch)
{
	if(!pFrameSearch)
	{
		pFrameSearch = DYNAMIC_DOWNCAST(
			CFrameWnd,
			AfxGetMainWnd());

		if(!pFrameSearch)
			return FALSE;
	}

	if(pFrameSearch->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		pFrameSearch = ((CMDIFrameWnd *) pFrameSearch)->GetActiveFrame();

		if(!pFrameSearch)
			return FALSE;
	}

	BOOL bOleInplaceItemActivated = FALSE;
	CView *pActiveView = pFrameSearch->GetActiveView();

	if(pActiveView)
	{
		CDocument *pActiveDoc = pActiveView->GetDocument();

		if(pActiveDoc)
		{
			COleDocument *pActiveOleDoc = DYNAMIC_DOWNCAST(
				COleDocument,
				pActiveDoc);

			if(pActiveOleDoc)
			{
				COleClientItem *pItem = pActiveOleDoc->GetInPlaceActiveItem(pActiveView);

				if(pItem)
					bOleInplaceItemActivated = TRUE;
			}
		}
	}

	return bOleInplaceItemActivated;
}

BOOL CTControlBar::SafeDisplayBar()
{
	if(m_bPresubclassDialogMode)
	{
		if(GetStyle()&WS_VISIBLE)
			return TRUE;

		return FALSE;
	}

	if(IsVisible())
		return TRUE;

	if(m_nStateFlags&CControlBar::tempHide)
		return FALSE;

	CFrameWnd *pFrame = _GetDockingFrameImpl();
	BOOL bOleInplaceItemActivated = CTControlBar::IsOleIpObjActive(pFrame);

	if( bOleInplaceItemActivated && (m_dwStyle&CBRS_HIDE_INPLACE) )
		return FALSE;

	pFrame->ShowControlBar( this, TRUE, FALSE);
	if(!IsVisible())
		return FALSE;

	return TRUE;
}

BOOL CTControlBar::IsSingleOnRow( BOOL bOnlyFixed, BOOL bOnlyNonFixed) const
{
	int nTotalDockCount = m_pDockBar->m_arrBars.GetSize();
	int nCount = 0;
	BOOL bThisFound = FALSE;

	for( int i=0; i<nTotalDockCount; i++)
	{
		CControlBar *pBar = (CControlBar *) m_pDockBar->m_arrBars[i];

		if(!pBar)
		{
			if(bThisFound)
				break;

			nCount = 0;
			continue;
		}

		if(__PLACEHODLER_BAR_PTR(pBar))
			continue;

		if( pBar == (CControlBar *) this )
			bThisFound = TRUE;

		if(!pBar->IsVisible())
			continue;

		if( bOnlyFixed || bOnlyNonFixed )
		{
			BOOL bFixed = TRUE;

			if( pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)) &&
				!((CTControlBar *) pBar)->IsFixedMode())
				bFixed = FALSE;

			if( bOnlyFixed && !bFixed )
				continue;

			if( !bOnlyFixed && bFixed )
				continue;
		}

		nCount++;
		if( bThisFound && nCount > 1 )
			return FALSE;
	}

	return nCount == 1;
}

void CTControlBar::MakeSingleOnRow()
{
	CFrameWnd *pFrame = GetDockingFrame();

	pFrame->RecalcLayout();
	if(IsDockedHorizontally())
		m_szDockedH = CSize( MAXSHORT, m_szDockedH.cy);
	else
		m_szDockedV = CSize( m_szDockedV.cx, MAXSHORT);

	pFrame->RecalcLayout();
}

BOOL CTControlBar::IsMinimizedOnRow() const
{
	if( IsDockedVertically() &&
		m_szDockedV.cy == _CalcDesiredMinVH())
		return TRUE;

	if( IsDockedHorizontally() &&
		m_szDockedH.cx == _CalcDesiredMinHW())
		return TRUE;

	return FALSE;
}

BOOL CTControlBar::IsMaximizedOnRow() const
{
	if(IsFloating())
		return FALSE;

	if(IsMinimizedOnRow())
		return FALSE;

	ControlBarVector vBars;
	_GetRowBars(vBars);

	int nCount = vBars.GetSize();
	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = vBars[i];

		if( pBar == this )
			continue;

		if(!pBar->IsMinimizedOnRow())
			return FALSE;
	}

	return TRUE;
}

void CTControlBar::MinimizeOnRow()
{
	if( IsFloating () || IsSingleOnRow() )
		return;

	ControlBarVector vBars;
	_GetRowBars(vBars);

	int nCount = vBars.GetSize();
	if( nCount <= 1 )
		return;

	CTDockBar *pDockBar = (CTDockBar *) m_pDockBar;
	BOOL bHorz = IsDockedHorizontally();
	int nCalcExtentMax = 0;

	if(bHorz)
	{
		int nMin = _CalcDesiredMinHW();

		nCalcExtentMax = m_szDockedH.cx - nMin;
		m_szDockedH.cx = nMin;
	}
	else
	{
		int nMin = _CalcDesiredMinVH();

		nCalcExtentMax = m_szDockedV.cy - nMin;
		m_szDockedV.cy = nMin;
	}

	for( int i=0; nCalcExtentMax > 0; i++)
	{
		if( i == nCount )
			i = 0;

		CTControlBar *pBar = vBars[i];

		if( pBar != this )
		{
			if(bHorz)
				pBar->m_szDockedH.cx++;
			else
				pBar->m_szDockedV.cy++;

			nCalcExtentMax--;
		}
	}

	GetParentFrame()->RecalcLayout();
	_GetDockingFrameImpl()->SetFocus();
}

void CTControlBar::MaximizeOnRow()
{
	if( IsFloating () || IsSingleOnRow() )
		return;

	ControlBarVector vBars;
	_GetRowBars(vBars);

	int nCount = vBars.GetSize();
	if( nCount <= 1 )
		return;

	CTDockBar *pDockBar = (CTDockBar *) m_pDockBar;
	BOOL bHorz = IsDockedHorizontally();
	int nMetric = 0;

	for( int i=0; i<nCount; i++)
	{
		CTControlBar *pBar = vBars[i];

		if( pBar == this )
			continue;

		if(bHorz)
		{
			int nMin = pBar->_CalcDesiredMinHW();

			nMetric += pBar->m_szDockedH.cx - nMin;
			pBar->m_szDockedH.cx = nMin;
		}
		else
		{
			int nMin = pBar->_CalcDesiredMinVH();

			nMetric += pBar->m_szDockedV.cy - nMin;
			pBar->m_szDockedV.cy = nMin;
		}
	}

	if(bHorz)
		m_szDockedH.cx += nMetric;
	else
		m_szDockedV.cy += nMetric;

	GetParentFrame()->RecalcLayout();
}

void CTControlBar::OnRepositionSingleChild( int cx, int cy, BOOL bRedraw)
{
	if(!m_bReposSingleChildMode)
		return;

	CWnd *pWnd = GetWindow(GW_CHILD);
	if( pWnd && ::IsWindow(pWnd->GetSafeHwnd()) )
	{
		CRect rcClient;

		if( cx < 0 || cy < 0 )
			GetClientRect(&rcClient);
		else
			rcClient.SetRect( 0, 0, cx, cy);

		if(IsFloating())
		{
			CSize sizeFrame(
				::GetSystemMetrics(SM_CXSIZEFRAME),
				::GetSystemMetrics(SM_CYSIZEFRAME));

			rcClient.DeflateRect(sizeFrame);
		}

		pWnd->MoveWindow( &rcClient, FALSE);
	}

	if(!bRedraw)
		return;

	RedrawWindow(
		NULL,
		NULL,
		RDW_ERASE|
		RDW_INVALIDATE|
		RDW_INTERNALPAINT|
		RDW_UPDATENOW|
		RDW_ALLCHILDREN|
		RDW_NOFRAME);
}

void CTControlBar::OnSize( UINT nType, int cx, int cy)
{
	if(m_bUpdatingChain)
		return;

	OnRepositionSingleChild( cx, cy, TRUE);
	CFrameWnd *pFrame = GetParentFrame();

	if( pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
	{
		OnNcAreaButtonsReposition();
		pFrame->SendMessage(WM_NCPAINT);
	}
}

void CTControlBar::_Dragging_OnStart()
{
	CTPopupMenuWnd::CancelMenuTracking();
}

void CTControlBar::_Dragging_OnStop( DraggingState& _dsOld,
									 DraggingState& _dsNew,
									 BOOL bCancel)
{
	if(IsFixedMode())
		return;
}

void CTControlBar::_SetCursor( const CPoint& point)
{
	HCURSOR hCursor = NULL;

	if(!m_bPresubclassDialogMode)
	{
		if( m_bDragging || m_bRowResizing || m_bRowRecalcing )
		{
			if(m_bDragging)
			{
				if(IsFixedMode())
					hCursor = g_hCursorDrag;
			}
			else if(IsDockedVertically())
			{
				if(m_bRowRecalcing)
					hCursor = g_hCursorResizeV;
				else
					hCursor = g_hCursorResizeH;
			}
			else if(m_bRowRecalcing)
				hCursor = g_hCursorResizeH;
			else
				hCursor = g_hCursorResizeV;
		}
		else
		{
			CRect rcBarWnd;
			GetWindowRect(&rcBarWnd);

			CPoint ptDevOffset = -rcBarWnd.TopLeft();
			CPoint ptWnd(point);

			ClientToScreen(&ptWnd);
			ptWnd += ptDevOffset;

			if(m_rcGrip.PtInRect(ptWnd))
			{
				int nCount = NcButtons_GetCount();

				for( int i=0; i<nCount; i++)
				{
					CTBarNcAreaButton *pBtn = NcButtons_GetAt(i);

					hCursor = pBtn->OnNcAreaQueryCursor(ptWnd);
					if(hCursor)
						break;
				}

				if(!hCursor)
					hCursor = g_hCursorDrag;
			}
		}
	}

	if(!hCursor)
		hCursor = (HCURSOR) ::GetClassLong( m_hWnd, GCL_HCURSOR);

	if( GetCursor() != hCursor )
		::SetCursor(hCursor);
}

CTControlBar *CTControlBar::_DraggingGetBar()
{
	if(m_DragStateOld.IsEmpty())
		return NULL;

	return m_DragStateOld.BarSrcGet();
}

BOOL CTControlBar::_DraggingCancel()
{
	CTControlBar *pBar = _DraggingGetBar();

	if(!pBar)
		return FALSE;

	pBar->_DraggingStop(TRUE);
	return TRUE;
}

void CTControlBar::_DraggingStart( const CPoint& point,
								   const CPoint& pointOffset)
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif

	m_DragStateOld.BarSrcSet(this);
	m_DragStateNew.BarSrcSet(this);
	m_DragStateNew.SetEmpty(FALSE);

	_Dragging_OnStart();
	SetCapture();
	m_bDragging = TRUE;

	int nCyGripper = min( m_rcGrip.Width(), m_rcGrip.Height());
	CPoint ptHelperDragOffsetDefault(
		nCyGripper > 2 ? nCyGripper : ::GetSystemMetrics(SM_CYSMCAPTION) / 2,
		nCyGripper > 2 ? nCyGripper / 2 : ::GetSystemMetrics(SM_CYSMCAPTION) / 2);

	m_ptHelperDragOffsetFloated = ptHelperDragOffsetDefault;
	m_ptHelperDragOffsetDockedH = ptHelperDragOffsetDefault;
	m_ptHelperDragOffsetDockedV = ptHelperDragOffsetDefault;

	if( pointOffset.x != 0 || pointOffset.y != 0 )
	{
		if(IsFloating())
			m_ptHelperDragOffsetFloated = pointOffset;
		else if(IsDockedHorizontally())
			m_ptHelperDragOffsetDockedH = pointOffset;
		else
			m_ptHelperDragOffsetDockedV = pointOffset;
	}

	_DraggingUpdateState(point);
	while(m_bDragging)
	{
		::WaitMessage();

		HWND hWndOwn = GetSafeHwnd();
		MSG msg;

		while(PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bStop = FALSE;

			switch(msg.message)
			{
			case WM_ACTIVATEAPP		:
			case WM_CANCELMODE		:
			case WM_RBUTTONDOWN		:
			case WM_NCLBUTTONUP		:
			case WM_NCRBUTTONUP		:
			case WM_NCRBUTTONDOWN	:
			case WM_MBUTTONDOWN		:
			case WM_MBUTTONUP		:
			case WM_NCMBUTTONDOWN	:
			case WM_NCMBUTTONUP		:
			case WM_CONTEXTMENU		: bStop = TRUE; break;
			default					:
				if(	WM_KEYFIRST <= msg.message &&
					msg.message <= WM_KEYLAST )
				{
					if( msg.message == WM_KEYDOWN ||
						msg.message == WM_KEYUP )
						if( msg.wParam == VK_CONTROL )
							break;

					bStop = TRUE;
				}
				else if( GetCapture() == this )
				{
					if(!CTPopupMenuWnd::IsKeyPressed(VK_LBUTTON) ||
						CTPopupMenuWnd::IsKeyPressed(VK_RBUTTON) ||
						CTPopupMenuWnd::IsKeyPressed(VK_MBUTTON))
						bStop = TRUE;
				}
				else
					bStop = TRUE;

				break;
			}

			if(!::IsWindow(hWndOwn))
				return;

			if(bStop)
			{
				_DraggingStop(TRUE);
				break;
			}

			if(!AfxGetApp()->PumpMessage())
			{
				PostQuitMessage(0);
				break;
			}
		}

		if( m_bDragging && m_bEnableOnIdleCalls )
		{
			LONG lIdle = 0;
			while(AfxGetApp()->OnIdle(lIdle++));
		}
	}
}

void CTControlBar::_DraggingStop( BOOL bCancel)
{
	if(!m_bDragging)
		return;

	m_bDragging = FALSE;
	m_ptDraggingLast.x = -1;
	m_ptDraggingLast.y = -1;

	BOOL bEmptyOld = m_DragStateOld.IsEmpty();
	BOOL bOldEqualNew = m_DragStateOld == m_DragStateNew;

	if(!m_DragStateNew.IsEmpty())
	{
		m_DragStateNew.DrawState();

		_Dragging_OnStop(
			m_DragStateOld,
			m_DragStateNew,
			bCancel);

		if(!bCancel)
		{
			if(_IsShowContentWhenDragging())
			{
				if( !bEmptyOld && !bOldEqualNew )
					_DraggingApplyState(m_DragStateNew);
			}
			else
				_DraggingApplyState(m_DragStateNew);
		}
	}

	m_DragStateOld.BarDstSet( NULL, TRUE);
	m_DragStateNew.BarDstSet( NULL, TRUE);

	m_DragStateOld.SetEmpty();
	m_DragStateNew.SetEmpty();

	if( GetCapture() == this )
		ReleaseCapture();

	if(bCancel)
		m_pDockSite->DelayRecalcLayout();
}

void CTControlBar::_DraggingUpdateState( const CPoint& point)
{
	if( m_bUpdatingDragState || m_ptDraggingLast == point )
		return;

	m_ptDraggingLast = point;
	m_bUpdatingDragState = TRUE;

	m_DragStateNew.CalcStateDynamic(point);
	if( m_DragStateNew != m_DragStateOld )
	{
		BOOL bShowContent = _IsShowContentWhenDragging();
		BOOL bEmptyOld = m_DragStateOld.IsEmpty();
		BOOL bEmptyNew = m_DragStateNew.IsEmpty();

		if(!bEmptyOld)
			m_DragStateOld.DrawState();

		if(!bEmptyNew)
			m_DragStateNew.DrawState();

		if( bShowContent &&
			!m_DragStateOld.IsEmpty() &&
			m_DragStateOld != m_DragStateNew )
			_DraggingApplyState(m_DragStateNew);

		m_DragStateOld = m_DragStateNew;
	}

	if( GetCapture() != this )
		SetCapture();
	m_bUpdatingDragState = FALSE;
}

void CTControlBar::_CompressRowSpace( int nSpaceNeeded,
									  MFCControlBarVector& arrPrevBars)
{
	typedef CArray< CRect, CRect&> Rects;
	typedef CArray< int, int&> Ints;

	int nSpaceAvail = 0;
	CRect rcDockBar;

	m_pDockBar->GetClientRect(&rcDockBar);
	m_pDockBar->ClientToScreen(&rcDockBar);

	int nCount = arrPrevBars.GetSize();
	CRect rcBarPrev;

	Rects vBarRects;
	Ints vOffsets;

	for( int i=0; i<nCount; i++)
	{
		CControlBar *pBar = arrPrevBars[i];
		CRect rcBar;

		pBar->GetWindowRect(&rcBar);
		if( i == 0 )
		{
			int nSpaceBeforeFirst = IsDockedHorizontally() ? rcBar.left - rcDockBar.left : rcBar.top - rcDockBar.top;

			nSpaceAvail = nSpaceBeforeFirst;
			vOffsets.Add(nSpaceBeforeFirst);
		}
		else
		{
			int nSpaceBetweenBars = IsDockedHorizontally() ? rcBar.left - rcBarPrev.right : rcBar.top - rcBarPrev.top;

			if( nSpaceBetweenBars < 0 )
				nSpaceBetweenBars = 0;

			nSpaceAvail += nSpaceBetweenBars;
			vOffsets.Add(nSpaceBetweenBars);
		}

		vBarRects.Add(rcBar);
		rcBarPrev = rcBar;
	}

	if( nSpaceAvail == 0 )
		return;

	if( nSpaceAvail > nSpaceNeeded )
		nSpaceAvail = nSpaceNeeded;

	for( i=nCount; i>0; i--)
	{
		CControlBar *pBar = arrPrevBars[i - 1];
		CRect &rcBar = vBarRects[i - 1];

		int nShift = vOffsets[i - 1];
		if( nShift > nSpaceAvail )
			nShift = nSpaceAvail;

		if(IsDockedHorizontally())
			rcBar.OffsetRect( -nSpaceAvail, 0);
		else
			rcBar.OffsetRect( 0, -nSpaceAvail);

		nSpaceAvail -= nShift;
		if( nSpaceAvail == 0 )
			break;
	}

	for( i=0; i<nCount; i++)
	{
		CControlBar *pBar = arrPrevBars[i];
		CRect& rcBar = vBarRects[i];

		((CTDockBar *) m_pDockBar)->_SlideDockControlBar(
			pBar,
			&rcBar,
			FALSE);
	}
}

void CTControlBar::OnNcAreaButtonsReinitialize()
{
	int nCount = NcButtons_GetCount();

	if( nCount > 0 )
		return;

	NcButtons_Add(new CTBarNcAreaButtonClose(this));
	NcButtons_Add(new CTBarNcAreaButtonMenu(this));
}

void CTControlBar::OnNcAreaButtonsReposition()
{
	CTBarNcAreaButton *pPrevBtn = NULL;
	int nCount = NcButtons_GetCount();

	for( int i=0; i<nCount; i++)
	{
		CTBarNcAreaButton *pBtn = NcButtons_GetAt(i);

		pBtn->OnNcAreaReposition(pPrevBtn);
		if(!pBtn->OnQueryVisibility())
			continue;

		pPrevBtn = pBtn;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButton

IMPLEMENT_DYNAMIC( CTBarNcAreaButton, CObject)

CTBarNcAreaButton::CTBarNcAreaButton( CTControlBar *pBar) :
	m_pBar(pBar),
	m_rc( 0, 0, 0, 0),
	m_bHover(FALSE),
	m_bPushed(FALSE),
	m_bDisabled(FALSE),
	m_bPaintingCombinedArea(FALSE)
{
}

CTBarNcAreaButton::~CTBarNcAreaButton()
{
}

void CTBarNcAreaButton::OnNcDestroy()
{
	delete this;
}

BOOL CTBarNcAreaButton::IsBarFixedMode() const
{
	return m_pBar->IsFixedMode();
}

BOOL CTBarNcAreaButton::IsBarVisible() const
{
	return m_pBar->IsVisible();
}

BOOL CTBarNcAreaButton::IsBarFloated() const
{
	return m_pBar->IsFloating();
}

BOOL CTBarNcAreaButton::IsBarDocked() const
{
	return m_pBar->IsDocked();
}

BOOL CTBarNcAreaButton::IsBarDockedHorizontally() const
{
	return m_pBar->IsDockedHorizontally();
}

BOOL CTBarNcAreaButton::IsBarDockedVertically() const
{
	return m_pBar->IsDockedVertically();
}

BOOL CTBarNcAreaButton::IsBarDockedAtLeft() const
{
	return m_pBar->IsDockedAtLeft();
}

BOOL CTBarNcAreaButton::IsBarDockedAtRight() const
{
	return m_pBar->IsDockedAtRight();
}

BOOL CTBarNcAreaButton::IsBarDockedAtTop() const
{
	return m_pBar->IsDockedAtTop();
}

BOOL CTBarNcAreaButton::IsBarDockedAtBottom() const
{
	return m_pBar->IsDockedAtBottom();
}

BOOL CTBarNcAreaButton::GetBarNcAreaInfo( LPRECT pRectGripper,
										  LPRECT pRectWnd,
										  BOOL *pGripperAtTop,
										  BOOL *pTextOnGripper) const
{
	if(pRectGripper)
		::SetRectEmpty(pRectGripper);

	if(pRectWnd)
		::SetRectEmpty(pRectWnd);

	if(IsBarFloated())
	{
		if(pGripperAtTop)
			*pGripperAtTop = FALSE;

		if(pTextOnGripper)
			*pTextOnGripper = FALSE;

		CFrameWnd *pMiniFrame = m_pBar->GetDockingFrame();
		if(pRectWnd)
			pMiniFrame->GetWindowRect(pRectWnd);

		CTMiniDockFrameWnd *pTMiniFrame = DYNAMIC_DOWNCAST(
			CTMiniDockFrameWnd,
			pMiniFrame);

		if(!pTMiniFrame)
			return FALSE;

		if(pRectGripper)
		{
			CRect rcGrip(pTMiniFrame->m_rcFrameCaption);

			rcGrip.InflateRect( 2, 2, 1, 2);
			*pRectGripper = rcGrip;
		}

		if(pGripperAtTop)
			*pGripperAtTop = TRUE;

		if(pTextOnGripper)
			*pTextOnGripper = TRUE;

		return TRUE;
	}

	if(!m_pBar->IsFixedMode())
	{
		CFrameWnd *pParentFrame = m_pBar->GetParentFrame();

		if( pParentFrame && pParentFrame->IsKindOf(RUNTIME_CLASS(CTMiniDockFrameWnd)) &&
			m_pBar->_IsSingleVisibleInFloatingPalette())
		{
			if(pGripperAtTop)
				*pGripperAtTop = FALSE;

			if(pTextOnGripper)
				*pTextOnGripper = FALSE;

			if(pRectWnd)
				pParentFrame->GetWindowRect(pRectWnd);

			CTMiniDockFrameWnd *pTMiniFrame = STATIC_DOWNCAST(
				CTMiniDockFrameWnd,
				pParentFrame);

			if(pRectGripper)
			{
				CRect rcGrip(pTMiniFrame->m_rcFrameCaption);

				rcGrip.InflateRect( 2, 2, 1, 2);
				*pRectGripper = rcGrip;
			}

			if(pGripperAtTop)
				*pGripperAtTop = TRUE;

			if(pTextOnGripper)
				*pTextOnGripper = TRUE;

			return TRUE;
		}
	}

	if(pRectWnd)
		m_pBar->GetWindowRect(pRectWnd);

	BOOL bGrip = m_pBar->IsBarWithGripper(
		pGripperAtTop,
		pTextOnGripper);

	if(!bGrip)
		return FALSE;

	if(pRectGripper)
		*pRectGripper = m_pBar->m_rcGrip;

	return TRUE;
}

BOOL CTBarNcAreaButton::IsBarWndActive() const
{
	if(IsBarFloated())
	{
		CFrameWnd *pMiniFrame = m_pBar->GetDockingFrame();
		CTMiniDockFrameWnd *pTMiniFrame = DYNAMIC_DOWNCAST(
			CTMiniDockFrameWnd,
			pMiniFrame);

		if(!pTMiniFrame)
			return FALSE;

		HWND hTmpWndFocus = ::GetFocus();
		if( hTmpWndFocus && (
			pTMiniFrame->GetSafeHwnd() == hTmpWndFocus ||
			::IsChild( pTMiniFrame->GetSafeHwnd(), hTmpWndFocus)))
			return TRUE;

		return FALSE;
	}

	return m_pBar->m_bWindowActive;
}

BOOL CTBarNcAreaButton::IsBarSingleOnRow() const
{
	return m_pBar->IsSingleOnRow();
}

BOOL CTBarNcAreaButton::IsBarMinimizedOnRow() const
{
	return m_pBar->IsMinimizedOnRow();
}

BOOL CTBarNcAreaButton::IsBarMaximizedOnRow() const
{
	return m_pBar->IsMaximizedOnRow();
}

BOOL CTBarNcAreaButton::IsBarFirstOnRow( BOOL bBar) const
{
	return m_pBar->_IsFirstBarInRow(bBar);
}

BOOL CTBarNcAreaButton::IsBarLastOnRow( BOOL bBar) const
{
	return m_pBar->_IsLastBarInRow(bBar);
}

CFrameWnd *CTBarNcAreaButton::GetDockingFrame()
{
	return m_pBar->_GetDockingFrameImpl();
}

void CTBarNcAreaButton::OnNcAreaReposition( CTBarNcAreaButton *pPrevBtn)
{
	BOOL bGripperAtTop = FALSE;

	if(!GetBarNcAreaInfo( &m_rc, NULL, &bGripperAtTop))
		return;

	const int _nGapBetweenButtons = 3;
	const int _nGapToBordersH = 2;
	const int _nGapToBordersV = 3;

	if(bGripperAtTop)
	{
		m_rc.DeflateRect(
			_nGapToBordersH,
			_nGapToBordersV);

		m_rc.left = m_rc.right - m_rc.Height();
		m_rc.OffsetRect( -1, 0);

		if(pPrevBtn)
		{
			const CRect& rcPrev = *pPrevBtn;
			m_rc.OffsetRect(rcPrev.left - m_rc.right - _nGapBetweenButtons, 0);
		}
	}
	else
	{
		m_rc.DeflateRect(
			_nGapToBordersV,
			_nGapToBordersH);

		m_rc.bottom = m_rc.top + m_rc.Width();
		m_rc.OffsetRect( 0, 1);

		if(pPrevBtn)
		{
			const CRect& rcPrev = *pPrevBtn;
			m_rc.OffsetRect( 0, -rcPrev.top + m_rc.bottom + _nGapBetweenButtons);
		}
	}
}

BOOL CTBarNcAreaButton::OnQueryVisibility() const
{
	if(!IsBarVisible())
		return FALSE;

	CRect rcGrip;
	if( !GetBarNcAreaInfo(&rcGrip) ||
		rcGrip.IsRectEmpty() )
		return FALSE;

	return TRUE;
}

void CTBarNcAreaButton::NcDrawDefault( CDC& dc, int nDockBtnGlyptT)
{
	if(m_rc.IsRectEmpty())
		return;

	BOOL bMenuTracking = CTPopupMenuWnd::IsMenuTracking();
	BOOL bDrawActive = FALSE;

	bDrawActive = IsBarWndActive();
	g_PaintManager->PaintDockingCaptionButton(
		dc, m_rc,
		(CTPaintManager::e_docking_caption_button) nDockBtnGlyptT,
		!m_bDisabled && m_bHover && !bMenuTracking && !m_bPaintingCombinedArea,
		!m_bDisabled && m_bPushed && !bMenuTracking && !m_bPaintingCombinedArea,
		!m_bDisabled,
		!IsBarFixedMode() || m_bPaintingCombinedArea,
		bDrawActive,
		IsBarFloated());
}

void CTBarNcAreaButton::OnNcAreaDraw( CDC& dc)
{
}

UINT CTBarNcAreaButton::OnNcAreaHitTest( CPoint point)
{
	if(m_rc.PtInRect(point))
	{
		m_bPushed = CTPopupMenuWnd::IsKeyPressed(VK_LBUTTON) &&
			!CTPopupMenuWnd::IsKeyPressed(VK_MBUTTON) &&
			!CTPopupMenuWnd::IsKeyPressed(VK_RBUTTON);

		m_bHover = TRUE;
		return HTNOWHERE;
	}

	m_bPushed = FALSE;
	m_bHover = FALSE;

	return HTCLIENT;
}

HCURSOR CTBarNcAreaButton::OnNcAreaQueryCursor( CPoint point)
{
	if(m_rc.PtInRect(point))
		return g_hCursorArrow;

	return NULL;
}

BOOL CTBarNcAreaButton::OnNcAreaClicked( CPoint point)
{
	if(m_rc.PtInRect(point))
		return TRUE;

	return FALSE;
}

int CTBarNcAreaButton::OnToolHitTest( CPoint point,
									  TOOLINFO * pTI)
{
	return -1;
}

int CTBarNcAreaButton::DoDefaultReflectForToolHitTest( TOOLINFO *pTI,
													   LPCTSTR strTooltipText)
{
	if(pTI)
	{
		CTControlBar *pBar = GetBar();
		CPoint ptDevOffset( 0, 0);

		if(pBar->IsFloating())
		{
			CFrameWnd *pFrame = pBar->GetParentFrame();
			CRect rcBarClient;
			CRect rcBarWnd;

			pFrame->GetWindowRect(&rcBarWnd);
			pFrame->GetClientRect(&rcBarClient);
			pFrame->ClientToScreen(&rcBarClient);

			ptDevOffset = rcBarClient.TopLeft() - rcBarWnd.TopLeft();
		}
		else
		{
			CRect rcBarWnd;
			CRect rcBarClient;

			pBar->GetWindowRect(&rcBarWnd);
			pBar->GetClientRect(&rcBarClient);
			pBar->ClientToScreen(&rcBarClient);

			ptDevOffset = rcBarClient.TopLeft() - rcBarWnd.TopLeft();
		}

		CRect rcArea = *this;
		rcArea.OffsetRect(-ptDevOffset);

		::CopyRect(
			&(pTI->rect),
			&rcArea);

		int nCmdID = (int) pBar->GetDlgCtrlID();
		pTI->uId = (UINT) nCmdID;
		pTI->hwnd = pBar->GetSafeHwnd();
		pTI->lpszText = (LPTSTR) ::calloc(
			_tcslen(strTooltipText) + 1,
			sizeof(TCHAR));

		if(pTI->lpszText)
			_tcscpy( pTI->lpszText, strTooltipText);
		else
			pTI->lpszText = LPSTR_TEXTCALLBACK;

		return nCmdID;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonExpand

IMPLEMENT_DYNAMIC( CTBarNcAreaButtonExpand, CTBarNcAreaButton)


CTBarNcAreaButtonExpand::CTBarNcAreaButtonExpand( CTControlBar *pBar) : CTBarNcAreaButton(pBar)
{
}

BOOL CTBarNcAreaButtonExpand::OnQueryVisibility() const
{
	if(!CTBarNcAreaButton::OnQueryVisibility())
		return FALSE;

	if( IsBarFixedMode() || IsBarFloated() )
		return FALSE;

	return TRUE;
}

void CTBarNcAreaButtonExpand::OnNcAreaDraw( CDC& dc)
{
	if(m_rc.IsRectEmpty())
		return;
	m_bDisabled = IsBarSingleOnRow();

	BOOL bMax = IsBarMaximizedOnRow();
	BOOL bHorz = IsBarDockedHorizontally();

	NcDrawDefault(
		dc, bHorz ? bMax ?
		CTPaintManager::__DCBT_ARROW_RIGHT : CTPaintManager::__DCBT_ARROW_LEFT :
		bMax ? CTPaintManager::__DCBT_ARROW_DOWN : CTPaintManager::__DCBT_ARROW_UP);
}

UINT CTBarNcAreaButtonExpand::OnNcAreaHitTest( CPoint point)
{
	UINT nHT = CTBarNcAreaButton::OnNcAreaHitTest(point);

	if( nHT == HTCLIENT )
		return HTCLIENT;

	if(IsBarSingleOnRow())
		return HTCLIENT;

	return HTMINBUTTON;
}

BOOL CTBarNcAreaButtonExpand::OnNcAreaClicked( CPoint point)
{
	if(!m_rc.PtInRect(point))
		return FALSE;

	if(IsBarSingleOnRow())
	{
		if(!IsBarWndActive())
			m_pBar->SetFocus();

		return TRUE;
	}

	if(IsBarMaximizedOnRow())
		m_pBar->MinimizeOnRow();
	else
	{
		m_pBar->MaximizeOnRow();
		m_pBar->SetFocus();
	}

	return TRUE;
}

int CTBarNcAreaButtonExpand::OnToolHitTest( CPoint point, TOOLINFO *pTI)
{
	BOOL bBarMaximizedOnRow = IsBarMaximizedOnRow();
	return DoDefaultReflectForToolHitTest(
		pTI,
		bBarMaximizedOnRow ? _T("Contract") : _T("Expand"));
}

/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonClose

IMPLEMENT_DYNAMIC( CTBarNcAreaButtonClose, CTBarNcAreaButton)


CTBarNcAreaButtonClose::CTBarNcAreaButtonClose( CTControlBar *pBar) : CTBarNcAreaButton(pBar)
{
}

BOOL CTBarNcAreaButtonClose::OnQueryVisibility() const
{
	if(!CTBarNcAreaButton::OnQueryVisibility())
		return FALSE;

	if(!IsBarFixedMode())
		return TRUE;

	return IsBarFloated();
}

void CTBarNcAreaButtonClose::OnNcAreaDraw( CDC& dc)
{
	OnQueryVisibility();

	if(m_rc.IsRectEmpty())
		return;

	NcDrawDefault( dc, CTPaintManager::__DCBT_CLOSE);
}

UINT CTBarNcAreaButtonClose::OnNcAreaHitTest( CPoint point)
{
	UINT nHT = CTBarNcAreaButton::OnNcAreaHitTest(point);

	if( nHT == HTCLIENT )
		return HTCLIENT;

	return HTMINBUTTON;
}

BOOL CTBarNcAreaButtonClose::OnNcAreaClicked( CPoint point)
{
	if(m_rc.PtInRect(point))
	{
		CFrameWnd *pFrame = GetDockingFrame();

		pFrame->ShowControlBar(
			m_pBar,
			!IsBarVisible(),
			FALSE);

		return TRUE;
	}

	return FALSE;
}

int CTBarNcAreaButtonClose::OnToolHitTest( CPoint point, TOOLINFO *pTI)
{
	return DoDefaultReflectForToolHitTest(
		pTI,
		_T("Close"));
}

/////////////////////////////////////////////////////////////////////////////
// CTBarNcAreaButtonMenu

IMPLEMENT_DYNAMIC( CTBarNcAreaButtonMenu, CTBarNcAreaButton)


CTBarNcAreaButtonMenu::CTBarNcAreaButtonMenu( CTControlBar *pBar) : CTBarNcAreaButton(pBar)
{
}

BOOL CTBarNcAreaButtonMenu::OnQueryVisibility() const
{
	if(!CTBarNcAreaButton::OnQueryVisibility())
		return FALSE;

	if( !IsBarFixedMode() || !IsBarFloated() )
		return FALSE;

	return TRUE;
}

void CTBarNcAreaButtonMenu::OnNcAreaDraw( CDC& dc)
{
	OnQueryVisibility();
	if(m_rc.IsRectEmpty())
		return;

	NcDrawDefault(
		dc,
		CTPaintManager::__DCBT_ARROW_DOWN);
}

UINT CTBarNcAreaButtonMenu::OnNcAreaHitTest( CPoint point)
{
	UINT nHT = CTBarNcAreaButton::OnNcAreaHitTest(point);

	if( nHT == HTCLIENT )
		return HTCLIENT;

	return HTMINBUTTON;
}

BOOL CTBarNcAreaButtonMenu::OnNcAreaClicked( CPoint point)
{
	if(!m_rc.PtInRect(point))
		return FALSE;

	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();
	CFrameWnd *pFrame = GetDockingFrame();
	HWND hWndTrack = pFrame->GetSafeHwnd();

	if(!pPopup->CreatePopupMenu(hWndTrack))
		return FALSE;

	CString strShowHidePanels(_T("&Show/hide panels"));
	pPopup->ItemInsert(
		(UINT) CTPopupMenuWnd::TYPE_POPUP,
		-1, strShowHidePanels);

	CTPopupMenuWnd *pPopupBars = pPopup->ItemGetPopup(pPopup->ItemGetCount() - 1);
	CTDockBar::_ContextMenuBuild(
		pFrame,
		pPopupBars);

	CRect rcBtnScreen(m_rc);
	CRect rcBarWnd;

	GetBarNcAreaInfo( NULL, &rcBarWnd);
	rcBtnScreen.OffsetRect(rcBarWnd.TopLeft());
	rcBtnScreen.InflateRect( 1, 1);

	CPoint ptCursor = rcBtnScreen.CenterPoint();
	::SetFocus(hWndTrack);

	pPopup->TrackPopupMenu(
		TPMX_TOPALIGN|
		TPMX_COMBINE_DEFAULT|
		TPMX_OWNERDRAW_FIXED,
		ptCursor.x,
		ptCursor.y,
		&rcBtnScreen,
		this,
		_CbPaintCombinedContent);

	return TRUE;
}

int CTBarNcAreaButtonMenu::OnToolHitTest( CPoint point,
										  TOOLINFO *pTI)
{
	return DoDefaultReflectForToolHitTest(
		pTI,
		_T("Options"));
}

void CTBarNcAreaButtonMenu::_CbPaintCombinedContent( LPVOID pCookie,
													 CDC& dc,
													 const CWnd& refWndMenu,
													 const CRect& rcExcludeArea,
													 int nCombineAlign)
{
	CTBarNcAreaButtonMenu *pBtn = (CTBarNcAreaButtonMenu *) pCookie;

	if(rcExcludeArea.IsRectEmpty())
		return;

	CRect rcBarWnd;
	pBtn->GetBarNcAreaInfo( NULL, &rcBarWnd);

	CRect rcExcludeAreaX;
	refWndMenu.GetClientRect(&rcExcludeAreaX);
	refWndMenu.ClientToScreen(&rcExcludeAreaX);

	CPoint ptOffset = rcBarWnd.TopLeft() - rcExcludeAreaX.TopLeft();
	CPoint ptViewportOrg = dc.GetViewportOrg();

	dc.SetViewportOrg(ptOffset);
	pBtn->m_bPaintingCombinedArea = TRUE;
	pBtn->OnNcAreaDraw(dc);
	pBtn->m_bPaintingCombinedArea = FALSE;
	dc.SetViewportOrg(ptViewportOrg);
}

LRESULT CTControlBar::OnSizeParent( WPARAM wParam, LPARAM lParam)
{
	if(!m_bReflectParentSizing)
		return (LRESULT) 0;

	LRESULT lResult = CControlBar::OnSizeParent( wParam, lParam);
	return lResult;
}

CTControlBar *CTControlBar::DraggingState::BarDstSet( CTControlBar *pBarDst,
													  BOOL bNoCheckOnShutdown)
{
	CTControlBar *pBarDstOld = m_pBarDst;
	m_pBarDst = pBarDst;

	return pBarDstOld;
}

void CTControlBar::DraggingState::_AssignFromOther( const CTControlBar::DraggingState& other)
{
	if( m_pBarDst && m_pBarDst != other.m_pBarDst )
		BarDstSet(other.m_pBarDst);

	m_bDeepHalfSplit = other.m_bDeepHalfSplit;
	m_nCircleNo = other.m_nCircleNo;
	m_rcTrack = other.m_rcTrack;
	m_pBarSrc = other.m_pBarSrc;
	m_pBarDst = other.m_pBarDst;
	m_bEmpty = other.m_bEmpty;
	m_nCDT = other.m_nCDT;
}

void CTControlBar::DraggingState::SetEmpty( BOOL bEmpty)
{
	if(bEmpty)
		BarDstSet(NULL);

	m_bEmpty = bEmpty;
	if(m_bEmpty)
		m_rcTrack.SetRectEmpty();
}

void CTControlBar::DraggingState::DrawState()
{
	if( m_nCDT == __ECDT_OWN_AREA )
		return;

	if(m_pBarSrc->IsFixedMode())
		return;

	if(m_pBarSrc->_IsShowContentWhenDragging())
		return;

	if(m_rcTrack.IsRectEmpty())
		return;

	CRect rcTrackerOuter(m_rcTrack);
	CRect rcDockingFrame;
	CRect rcDockBarWnd;

	m_pBarSrc->m_pDockSite->GetWindowRect(&rcDockingFrame);
	if(m_pBarDst)
	{
		m_pBarDst->m_pDockBar->GetWindowRect(&rcDockBarWnd);

		if(rcDockBarWnd.IsRectEmpty())
			return;

		if( rcTrackerOuter.left < rcDockBarWnd.left )
			rcTrackerOuter.left = rcDockBarWnd.left;

		if( rcTrackerOuter.top < rcDockBarWnd.top )
			rcTrackerOuter.top = rcDockBarWnd.top;

		if( rcTrackerOuter.right > rcDockBarWnd.right )
			rcTrackerOuter.right = rcDockBarWnd.right;

		if( rcTrackerOuter.bottom > rcDockBarWnd.bottom )
			rcTrackerOuter.bottom = rcDockBarWnd.bottom;

		if(rcTrackerOuter.IsRectEmpty())
			return;
	}
	rcTrackerOuter.OffsetRect(-rcDockingFrame.TopLeft());

	int nTrackerBorderH = ::GetSystemMetrics(SM_CXSIZEFRAME);
	int nTrackerMinWidth = nTrackerBorderH * 2 + 1;

	if( rcTrackerOuter.Width() < nTrackerMinWidth )
		return;

	int nTrackerBorderV = ::GetSystemMetrics(SM_CYSIZEFRAME);
	int nTrackerMinHeight = nTrackerBorderV * 2 + 1;

	if( rcTrackerOuter.Height() < nTrackerMinHeight )
		return;

	CWnd *pWndDrawTracker = NULL;
	if( m_pBarDst != NULL ||
		m_nCDT == __ECDT_DOCKBAR_INNER_TOP ||
		m_nCDT == __ECDT_DOCKBAR_INNER_BOTTOM ||
		m_nCDT == __ECDT_DOCKBAR_INNER_LEFT ||
		m_nCDT == __ECDT_DOCKBAR_INNER_RIGHT ||
		m_nCDT == __ECDT_CIRCLE1_OUTER_TOP ||
		m_nCDT == __ECDT_CIRCLE1_OUTER_BOTTOM ||
		m_nCDT == __ECDT_CIRCLE1_OUTER_LEFT ||
		m_nCDT == __ECDT_CIRCLE1_OUTER_RIGHT )
	{
		if(m_pBarDst)
		{
			pWndDrawTracker = m_pBarDst->GetDockingFrame();

			if( m_pBarSrc->m_pDockSite != pWndDrawTracker )
			{
				m_pBarSrc->m_pDockSite->ClientToScreen(&rcTrackerOuter);
				pWndDrawTracker->ScreenToClient(&rcTrackerOuter);

				rcTrackerOuter.OffsetRect( -1, -1);
			}
		}
		else
			pWndDrawTracker = m_pBarSrc->m_pDockSite;
	}
	else
		pWndDrawTracker = CWnd::GetDesktopWindow();

	CDC *pDcDrawTracker = pWndDrawTracker->GetDCEx( NULL, __DOCKSITE_DCX_FLAGS);
	if(pDcDrawTracker->RectVisible(&rcTrackerOuter))
	{
		CRect rcTrackerInner(rcTrackerOuter);

		rcTrackerInner.DeflateRect(
			nTrackerBorderH,
			nTrackerBorderV);

		BOOL bRgnCreated = FALSE;
		CRgn rgnPaint;
		CRgn rgnInner;
		CRgn rgnOuter;
		CRgn rgnTabMainArea;
		CRgn rgnTabBottomLeftArea;
		CRgn rgnTabBottomMiddleArea;
		CRgn rgnTabBottomRightArea;

		if(!bRgnCreated)
		{
			if( rgnOuter.CreateRectRgnIndirect(&rcTrackerOuter) &&
				rgnInner.CreateRectRgnIndirect(&rcTrackerInner) &&
				rgnPaint.CreateRectRgn( 0, 0, 0, 0))
			{
				int nRgnPaintCombineResult = rgnPaint.CombineRgn(
					&rgnOuter,
					&rgnInner,
					RGN_DIFF);

				if( nRgnPaintCombineResult != ERROR &&
					nRgnPaintCombineResult != NULLREGION )
					bRgnCreated = TRUE;
			}
		}

		if(bRgnCreated)
		{
			CBrush *pBrushOld = pDcDrawTracker->SelectObject(CDC::GetHalftoneBrush());
			pDcDrawTracker->SelectClipRgn(&rgnPaint);

			pDcDrawTracker->PatBlt(
				rcTrackerOuter.left,
				rcTrackerOuter.top,
				rcTrackerOuter.Width(),
				rcTrackerOuter.Height(),
				PATINVERT);

			pDcDrawTracker->SelectObject(pBrushOld);
			pDcDrawTracker->SelectClipRgn(NULL);
		}
	}

	pWndDrawTracker->ReleaseDC(pDcDrawTracker);
}

void CTControlBar::BarRepositionData::SetBar( CControlBar *pBar,
											  BOOL bSetAdvData)
{
	m_pBar = pBar;
	m_pBar->GetWindowRect(m_rcSrc);

	m_nSpaceAvailBySideBefore = 0;
	m_nSpaceAvailBySideAfter = 0;
	m_nSpaceAvailByBar = 0;
	m_rcDst = m_rcSrc;

	if(!bSetAdvData)
		return;

	CTControlBar *pControlBar = GetBarFixed();
	if(!pControlBar)
		return;

	if(pControlBar->IsFloating())
		return;

	BOOL bHorz = pControlBar->IsDockedHorizontally();
	CRect rcClient;

	pControlBar->GetClientRect(&rcClient);
	m_nSpaceAvailByBar = bHorz ? m_rcSrc.Width() - rcClient.Width() : m_rcSrc.Height() - rcClient.Height();

	CRect rcDockBar;
	pControlBar->m_pDockBar->GetWindowRect(&rcDockBar);

	CControlBar *pBarPrev = pControlBar->_GetNextControlBarInRow( pControlBar, FALSE);
	while(!pBarPrev->IsVisible())
	{
		CControlBar *pLastBar2 = _GetNextControlBarInRow( pBarPrev, FALSE);

		if(!pLastBar2)
			break;

		pBarPrev = pLastBar2;
	}

	if(pBarPrev)
	{
		CRect rcPrevBar;

		pBarPrev->GetWindowRect(&rcPrevBar);
		m_nSpaceAvailBySideBefore = bHorz ? m_rcSrc.left - rcPrevBar.right : m_rcSrc.top - rcPrevBar.bottom;
	}
	else
		m_nSpaceAvailBySideBefore = bHorz ? m_rcSrc.left - rcDockBar.left : m_rcSrc.top - rcDockBar.top;

	CControlBar *pBarNext = pControlBar->_GetNextControlBarInRow( pControlBar, TRUE);
	while(!pBarNext->IsVisible())
	{
		CControlBar *pLastBar2 = _GetNextControlBarInRow( pBarNext, TRUE);

		if(!pLastBar2)
			break;

		pBarNext = pLastBar2;
	}

	if(pBarNext)
	{
		CRect rcNextBar;

		pBarNext->GetWindowRect(&rcNextBar);
		m_nSpaceAvailBySideAfter = bHorz ? m_rcSrc.left - rcNextBar.right : m_rcSrc.top - rcNextBar.bottom;
	}
	else
		m_nSpaceAvailBySideBefore = bHorz ? rcDockBar.right - m_rcSrc.right : rcDockBar.bottom - m_rcSrc.bottom;
}

CTControlBar *CTControlBar::BarRepositionData::GetBar()
{
	if(!m_pBar)
		return NULL;

	CTControlBar *pBar = DYNAMIC_DOWNCAST(
		CTControlBar,
		m_pBar);

	return pBar;
}

CTControlBar *CTControlBar::BarRepositionData::GetBarFixed()
{
	CTControlBar *pBar = GetBar();

	if(!pBar)
		return NULL;

	if(pBar->IsFixedMode())
		return NULL;

	return pBar;
}

void CTControlBar::DraggingState::AlignFixedRow( MFCControlBarVector& vRowBars)
{
}

void CTControlBar::DraggingState::CalcStateFixed( CPoint ptTrack)
{
	TrackRectSetEmpty();

	BOOL bForceFloat = CTPopupMenuWnd::IsKeyPressed(VK_CONTROL);
	CFrameWnd *pFrame = m_pBarSrc->_GetDockingFrameImpl();

	CRect rcFrame;
	pFrame->GetWindowRect(&rcFrame);

	CRect rcBarWnd;
	m_pBarSrc->GetWindowRect(&rcBarWnd);

	CRect rcFloatingBar(rcBarWnd);
	static const int nRdpInner = 4;
	static const int nRdpOuter = 12;
	static const int nRowInner = 2;

	int nCyGripper = min(
		m_pBarSrc->m_rcGrip.Width(),
		m_pBarSrc->m_rcGrip.Height());
	CSize sizeOffset( nCyGripper, nCyGripper / 2);

	BOOL bAlreadyFloat = m_pBarSrc->IsFloating();
	if( !bAlreadyFloat && !bForceFloat )
	{
		CTDockBar *pDockBar = (CTDockBar *) m_pBarSrc->m_pDockBar;
		CRect rcDockBar;
		CRect rcRowBars;

		BOOL bRowFullSizedThis = FALSE;
		BOOL bRowMaxSizedThis = FALSE;
		int nOwnIdx = -1;

		MFCControlBarVector vRowBars;
		m_pBarSrc->_GetRowBars(
			vRowBars,
			&rcRowBars,
			&nOwnIdx,
			&bRowMaxSizedThis,
			&bRowFullSizedThis);

		int nCount = vRowBars.GetSize();
		int nCountInRowBefore = nOwnIdx;
		int nCountInRowAfter = nCount - nOwnIdx - 1;
		BOOL bHorzSliding = m_pBarSrc->IsDockedHorizontally();

		pDockBar->GetClientRect(&rcDockBar);
		pDockBar->ClientToScreen(&rcDockBar);

		UINT nDockBarID = pDockBar->GetDlgCtrlID();
		if( nDockBarID == AFX_IDW_DOCKBAR_TOP )
			rcDockBar.top = rcFrame.top - nRowInner;
		else if( nDockBarID == AFX_IDW_DOCKBAR_BOTTOM )
			rcDockBar.bottom = rcFrame.bottom + nRowInner;
		else if( nDockBarID == AFX_IDW_DOCKBAR_LEFT )
			rcDockBar.left = rcFrame.left - nRowInner;
		else if( nDockBarID == AFX_IDW_DOCKBAR_RIGHT )
			rcDockBar.right = rcFrame.right + nRowInner;

		int nSpaceTotalBefore = 0;
		int nSpaceTotalAfter = 0;
		CRect rcRow(rcRowBars);

		if(bHorzSliding)
		{
			rcRow.left = min( rcRow.left, rcDockBar.left);
			rcRow.right = max( rcRow.right, rcDockBar.right);
			nSpaceTotalBefore = rcBarWnd.left - rcDockBar.left;
			nSpaceTotalAfter = rcDockBar.right - rcBarWnd.right;
		}
		else
		{
			rcRow.top = min( rcRow.top, rcDockBar.top);
			rcRow.bottom = max( rcRow.bottom, rcDockBar.bottom);
			nSpaceTotalBefore = rcBarWnd.top - rcDockBar.top;
			nSpaceTotalAfter = rcDockBar.bottom - rcBarWnd.bottom;
		}

		if( bRowFullSizedThis &&
			rcRow.PtInRect(ptTrack))
			return;

		BOOL bMoveInsideRow = FALSE;
		int nRowOffset = 0;
		CRect rcRowTest(rcRow);

		MFCControlBarVector vTestOuterBars;
		m_pBarSrc->_GetNextRowBars(
			nDockBarID == AFX_IDW_DOCKBAR_LEFT || nDockBarID == AFX_IDW_DOCKBAR_TOP ? FALSE : TRUE,
			vTestOuterBars);

		if(!vTestOuterBars.GetSize())
		{
			rcRowTest.DeflateRect(
				nDockBarID == AFX_IDW_DOCKBAR_LEFT ? nRowInner : 0,
				nDockBarID == AFX_IDW_DOCKBAR_TOP ? nRowInner : 0,
				nDockBarID == AFX_IDW_DOCKBAR_RIGHT ? nRowInner : 0,
				nDockBarID == AFX_IDW_DOCKBAR_BOTTOM ? nRowInner : 0);
		}

		BOOL bAnalyzeOverResizableBar = FALSE;
		if(!bRowMaxSizedThis)
		{
			CRect rcRowAnalyze(rcRowTest);

			bMoveInsideRow = !bRowFullSizedThis && rcRowAnalyze.PtInRect(ptTrack);
			if( !bMoveInsideRow || bRowFullSizedThis )
			{
				rcRowAnalyze.InflateRect(
					bHorzSliding ? 0 : nRdpOuter,
					bHorzSliding ? nRdpOuter : 0,
					0, 0);

				if(bHorzSliding)
					rcRowAnalyze.bottom = rcRowTest.top - 1;
				else
					rcRowAnalyze.right = rcRowTest.left - 1;

				if(!rcRowAnalyze.PtInRect(ptTrack))
				{
					rcRowAnalyze = rcRowTest;
					rcRowAnalyze.InflateRect(
						0, 0,
						bHorzSliding ? 0 : nRdpOuter,
						bHorzSliding ? nRdpOuter : 0);

					if(bHorzSliding)
						rcRowAnalyze.top = rcRowTest.bottom + 1;
					else
						rcRowAnalyze.left = rcRowTest.right + 1;

					if(rcRowAnalyze.PtInRect(ptTrack))
						nRowOffset = 1;
				}
				else
					nRowOffset = -1;

				if( nRowOffset == 0 &&
					rcDockBar.PtInRect(ptTrack))
				{
					CPoint ptCenterBar = rcBarWnd.CenterPoint();
					nRowOffset = bHorzSliding ? ptTrack.y - ptCenterBar.y : ptTrack.x - ptCenterBar.x;

					if( nRowOffset < 0 )
						nRowOffset = -1;
					else if( nRowOffset > 0 )
						nRowOffset = 1;

					if( nRowOffset != 0 )
					{
						bAnalyzeOverResizableBar = TRUE;
						bMoveInsideRow = FALSE;
					}
				}
			}

			BOOL bTryMoveInRow = FALSE;
			if( !bRowFullSizedThis && !nRowOffset )
				bTryMoveInRow = TRUE;

			if(bTryMoveInRow)
			{
				rcRowAnalyze = rcRowTest;
				rcRowAnalyze.InflateRect(
					bHorzSliding ? 0 : nRdpInner,
					bHorzSliding ? nRdpInner : 0,
					0, 0);

				if(rcRowAnalyze.PtInRect(ptTrack))
				{
					bMoveInsideRow = TRUE;
					nRowOffset = 0;
				}
				else
				{
					rcRowAnalyze = rcRowTest;
					rcRowAnalyze.InflateRect(
						0, 0,
						bHorzSliding ? 0 : nRdpInner,
						bHorzSliding ? nRdpInner : 0);

					if(rcRowAnalyze.PtInRect(ptTrack))
					{
						bMoveInsideRow = TRUE;
						nRowOffset = 0;
					}
				}
			}
		}

		if((!bRowFullSizedThis &&
			!bMoveInsideRow &&
			!nRowOffset &&
			rcDockBar.PtInRect(ptTrack)) ||
			bAnalyzeOverResizableBar)
		{
			ControlBarVector vResizableBars;

			_GetFrameControlBars(
				pFrame,
				vResizableBars,
				__GFECB_NONFIXED|
				__GFECB_DOCKED|
				__GFECB_VIS_ALL|
				__GFECB_BYPT_ONLY_WND,
				ptTrack);

			int nResizableCountByPoint = vResizableBars.GetSize();
			if(!nResizableCountByPoint)
				bMoveInsideRow = !nRowOffset;
			else if(bAnalyzeOverResizableBar)
			{
				nRowOffset = 0;
				bMoveInsideRow = FALSE;
			}
		}

		if(bHorzSliding)
		{
			rcFloatingBar.OffsetRect( ptTrack.x - rcFloatingBar.TopLeft().x - m_pBarSrc->m_ptHelperDragOffsetDockedH.x, 0);

			if( rcFloatingBar.right > rcDockBar.right )
				rcFloatingBar.OffsetRect( rcDockBar.right - rcFloatingBar.right, 0);

			if( rcFloatingBar.left < rcDockBar.left )
				rcFloatingBar.OffsetRect( rcDockBar.left - rcFloatingBar.left, 0);
		}
		else
		{
			rcFloatingBar.OffsetRect( 0, ptTrack.y - rcFloatingBar.TopLeft().y - m_pBarSrc->m_ptHelperDragOffsetDockedV.y);

			if( rcFloatingBar.bottom > rcDockBar.bottom )
				rcFloatingBar.OffsetRect( 0, rcDockBar.bottom - rcFloatingBar.bottom - 1);

			if( rcFloatingBar.top < rcDockBar.top )
				rcFloatingBar.OffsetRect( 0, rcDockBar.top - rcFloatingBar.top);
		}

		if(nRowOffset)
		{
			if(!m_pBarSrc->IsSingleOnRow())
			{
				pDockBar->_NewRowDockControlBar(
					m_pBarSrc,
					m_pBarSrc,
					nRowOffset < 0);

				pFrame->RecalcLayout();
				return;
			}

			MFCControlBarVector vNextRowBars;
			BOOL bRowFullSizedNext = FALSE;
			BOOL bRowMaxSizedNext = FALSE;
			int nOwnPosInDockBar = -1;
			CRect rcRow;

			m_pBarSrc->_GetNextRowBars(
				nRowOffset > 0,
				vNextRowBars,
				&rcRow,
				&nOwnPosInDockBar,
				&bRowMaxSizedNext,
				&bRowFullSizedNext);

			int nCountOfNextRowBars = vNextRowBars.GetSize();
			if( !bRowMaxSizedNext && nCountOfNextRowBars > 0 )
			{
				if(bRowFullSizedNext)
				{
					int nCountOfSideBars = pDockBar->m_arrBars.GetSize();
					CControlBar *pFullRowBar = vNextRowBars[0];
					int nFullRowBarPos = pDockBar->FindBar(pFullRowBar);

					pDockBar->m_arrBars.RemoveAt(nFullRowBarPos);
					nCountOfSideBars--;
					if((nCountOfSideBars == nFullRowBarPos &&
						!pDockBar->m_arrBars[nFullRowBarPos - 1]) || (
						nCountOfSideBars > nFullRowBarPos &&
						!pDockBar->m_arrBars[nFullRowBarPos - 1] &&
						!pDockBar->m_arrBars[nFullRowBarPos]))
					{
						pDockBar->m_arrBars.RemoveAt(nFullRowBarPos - 1);
						nCountOfSideBars--;
					}

					nOwnIdx = pDockBar->FindBar(m_pBarSrc);
					for( int i = nOwnIdx - nRowOffset; TRUE; i -= nRowOffset )
					{
						CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];

						if( i == 0 || i == nCountOfSideBars || pBar == NULL )
						{
							pDockBar->m_arrBars.InsertAt( i, pFullRowBar);
							pDockBar->m_arrBars.InsertAt( i, (CControlBar *) NULL);

							break;
						}
					}

					pFrame->RecalcLayout();
					return;
				}

				if(m_pBarSrc->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
				{
					int nCountOfSideBars = pDockBar->m_arrBars.GetSize();
					int nFullRowBarPos = pDockBar->FindBar(m_pBarSrc);

					pDockBar->m_arrBars.RemoveAt(nFullRowBarPos);
					nCountOfSideBars--;

					if((nCountOfSideBars == nFullRowBarPos &&
						!pDockBar->m_arrBars[nFullRowBarPos - 1]) || (
						nCountOfSideBars > nFullRowBarPos &&
						!pDockBar->m_arrBars[nFullRowBarPos - 1] &&
						!pDockBar->m_arrBars[nFullRowBarPos]))
					{
						pDockBar->m_arrBars.RemoveAt(nFullRowBarPos - 1);
						nCountOfSideBars--;
					}

					CControlBar *pSomeBar = vNextRowBars[nRowOffset < 0 ? 0 : nCountOfNextRowBars - 1];
					int nSomeBarIdx = pDockBar->FindBar(pSomeBar);

					for( int i = nSomeBarIdx + nRowOffset; TRUE; i += nRowOffset )
					{
						CControlBar *pBar = (CControlBar *) pDockBar->m_arrBars[i];
						if( i == 0 || i == nCountOfSideBars || pBar == NULL )
						{
							pDockBar->m_arrBars.InsertAt( i, m_pBarSrc);
							pDockBar->m_arrBars.InsertAt( i, (CControlBar *) NULL);

							break;
						}
					}

					pFrame->RecalcLayout();
					return;
				}

				int nMinReal = bHorzSliding ? rcFloatingBar.left : rcFloatingBar.top;
				int nMaxReal = bHorzSliding ? rcFloatingBar.right : rcFloatingBar.bottom;
				int nDockPos = -1;

				for( int i=0; i<nCountOfNextRowBars; i++)
				{
					CControlBar *pBar = vNextRowBars[i];
					CRect rcIntersectBar;

					pBar->GetWindowRect(&rcIntersectBar);
					int nMinTest = bHorzSliding ? rcIntersectBar.left : rcIntersectBar.top;
					int nMaxTest = bHorzSliding ? rcIntersectBar.right : rcIntersectBar.bottom;

					if( nMinReal < nMinTest )
					{
						nDockPos = pDockBar->FindBar(pBar);
						if( nMaxReal < nMinTest  )
							continue;

						if( nMaxReal < nMaxTest  )
							break;

						int nDiffUp = nMinTest - nMinReal;
						int nDiffDn = nMaxReal - nMaxTest;

						if( nDiffDn > nDiffUp )
							nDockPos++;

						break;
					}

					if( i == nCountOfNextRowBars - 1 )
					{
						nDockPos = pDockBar->FindBar(pBar);
						nDockPos++;
						break;
					}
				}

				if( nDockPos > nOwnPosInDockBar )
					nDockPos--;

				pDockBar->m_arrBars.RemoveAt(nOwnPosInDockBar);
				if( !pDockBar->m_arrBars[nOwnPosInDockBar] &&
					pDockBar->m_arrBars.GetSize() > 1 &&
					!pDockBar->m_arrBars[nOwnPosInDockBar - 1])
				{
					pDockBar->m_arrBars.RemoveAt(nOwnPosInDockBar);

					if( nDockPos > nOwnPosInDockBar )
						nDockPos--;
				}

				pDockBar->m_arrBars.InsertAt( nDockPos, m_pBarSrc);
				pFrame->RecalcLayout();
				return;
			}

			if(bRowFullSizedThis)
				return;

			bMoveInsideRow = TRUE;
		}

		if(bMoveInsideRow)
		{
			CArray< BarRepositionData, BarRepositionData&> vRepositionPrev, vRepositionNext;
			int nOffsetBarDesired = bHorzSliding ? rcFloatingBar.left : rcFloatingBar.top;
			int nSpaceNeededBefore = ( bHorzSliding ? rcBarWnd.left : rcBarWnd.top ) - nOffsetBarDesired;
			int nRedrawBarsBefore = 0;
			int nRedrawBarsAfter = 0;

			if( nCountInRowBefore > 0 && nSpaceNeededBefore > 0 )
			{
				CControlBar *pPrevBar = vRowBars[nOwnIdx - 1];
				CRect rcPrevBar;

				pPrevBar->GetWindowRect(&rcPrevBar);
				int nSpaceToCompress = 0;

				if(bHorzSliding)
				{
					if( rcPrevBar.right >= rcFloatingBar.left )
						nSpaceToCompress = rcPrevBar.right - rcFloatingBar.left;
				}
				else if( rcPrevBar.bottom >= rcFloatingBar.top )
					nSpaceToCompress = rcPrevBar.bottom - rcFloatingBar.top;

				if( nSpaceToCompress > 0 )
				{
					int nBarWidths = 0;

					for( int i=0; i<nCountInRowBefore; i++)
					{
						CControlBar *pBar = vRowBars[i];
						BarRepositionData brd(pBar);

						vRepositionPrev.Add(brd);
						nBarWidths += bHorzSliding ? brd.m_rcSrc.Width() : brd.m_rcSrc.Height();
					}

					int nSpaceAvail = nSpaceTotalBefore - nBarWidths;
					if( nSpaceAvail > 0 )
					{
						if( nSpaceNeededBefore > nSpaceAvail )
						{
							int nDist = nSpaceNeededBefore - nSpaceAvail;

							rcFloatingBar.OffsetRect(
								bHorzSliding ? nDist : 0,
								bHorzSliding ? 0 : nDist);
						}

						int nOffsetBarPrev = bHorzSliding ? rcFloatingBar.left : rcFloatingBar.top;
						int nOffsetBarPrevMax = bHorzSliding ? rcDockBar.left + nBarWidths : rcDockBar.top + nBarWidths;

						if( nOffsetBarPrev < nOffsetBarPrevMax )
							nOffsetBarPrev = nOffsetBarPrevMax;

						for( i = nCountInRowBefore; i>0; i--)
						{
							BarRepositionData& brd = vRepositionPrev[i - 1];
							int nMetric = bHorzSliding ? brd.m_rcDst.right : brd.m_rcDst.bottom;

							if( nOffsetBarPrev < nMetric )
							{
								int nDist = nOffsetBarPrev - nMetric;

								brd.m_rcDst.OffsetRect(
									bHorzSliding ? nDist : 0,
									bHorzSliding ? 0 : nDist);

								nRedrawBarsBefore++;
							}

							nOffsetBarPrev = bHorzSliding ? brd.m_rcDst.left : brd.m_rcDst.top;
						}
					}
					else
						rcFloatingBar = rcBarWnd;
				}
			}

			nOffsetBarDesired = bHorzSliding ? rcFloatingBar.right : rcFloatingBar.bottom;
			int nSpaceNeededAfter = nOffsetBarDesired - ( bHorzSliding ? rcBarWnd.right : rcBarWnd.bottom );

			if( nCountInRowAfter > 0 && nSpaceNeededAfter > 0 )
			{
				CControlBar *pNextBar = vRowBars[nOwnIdx + 1];
				CRect rcNextBar;

				pNextBar->GetWindowRect(&rcNextBar);
				int nSpaceToCompress = 0;

				if(bHorzSliding)
				{
					if( rcNextBar.left <= rcFloatingBar.right )
						nSpaceToCompress = rcFloatingBar.right - rcNextBar.left;
				}
				else if( rcNextBar.top <= rcFloatingBar.bottom )
					nSpaceToCompress = rcFloatingBar.bottom - rcNextBar.top;

				if( nSpaceToCompress > 0 )
				{
					int nBarWidths = 0;

					for( int i = nOwnIdx + 1; i < nCount; i++)
					{
						CControlBar *pBar = vRowBars[i];
						BarRepositionData brd(pBar);

						vRepositionNext.Add(brd);
						nBarWidths += bHorzSliding ? brd.m_rcSrc.Width() : brd.m_rcSrc.Height();
					}

					int nSpaceAvail = nSpaceTotalAfter - nBarWidths;
					if( nSpaceAvail > 0 )
					{
						int nOffsetBarNext = bHorzSliding ? rcFloatingBar.right : rcFloatingBar.bottom;
						int nOffsetBarNextMax = bHorzSliding ? rcDockBar.right - nBarWidths : rcDockBar.bottom - nBarWidths;

						if( nOffsetBarNext > nOffsetBarNextMax)
							nOffsetBarNext = nOffsetBarNextMax;

						for( i=0; i<nCountInRowAfter; i++)
						{
							BarRepositionData& brd = vRepositionNext[i];
							int nMetric = bHorzSliding ? brd.m_rcDst.left : brd.m_rcDst.top;

							if( nOffsetBarNext > nMetric )
							{
								int nDist = nOffsetBarNext - nMetric;

								brd.m_rcDst.OffsetRect(
									bHorzSliding ? nDist : 0,
									bHorzSliding ? 0 : nDist);

								nRedrawBarsAfter++;
							}

							if( i == 0 )
								rcNextBar = brd.m_rcDst;

							nOffsetBarNext = bHorzSliding ? brd.m_rcDst.right : brd.m_rcDst.bottom;
						}
					}

					rcFloatingBar.OffsetRect(
						bHorzSliding ? rcNextBar.left - rcFloatingBar.right : 0,
						bHorzSliding ? 0 : rcNextBar.top - rcFloatingBar.bottom);
				}
			}

			int nHdwpCount = nRedrawBarsBefore + nRedrawBarsAfter;
			if( rcFloatingBar != rcBarWnd )
				nHdwpCount++;

			nHdwpCount += 10;
			HANDLE hDWP = ::BeginDeferWindowPos(nHdwpCount);

			static const UINT nDwpFlags = SWP_NOZORDER|
				SWP_NOOWNERZORDER|
				SWP_NOACTIVATE;

			BOOL bRredrawDockBar = FALSE;
			if( nRedrawBarsBefore > 0 )
			{
				int nCount = vRepositionPrev.GetSize();
				for( int i=0; i<nCount; i++)
				{
					BarRepositionData& brd = vRepositionPrev[i];

					if(!brd.IsRectsEqual())
					{
						CRect rcMove(brd.m_rcDst);

						pDockBar->ScreenToClient(&rcMove);
						if(hDWP)
						{
							hDWP = ::DeferWindowPos(
								hDWP,
								brd.m_pBar->GetSafeHwnd(),
								NULL,
								rcMove.left,
								rcMove.top,
								rcMove.Width(),
								rcMove.Height(),
								nDwpFlags);
						}
						else
						{
							if(!bRredrawDockBar)
								pDockBar->LockWindowUpdate();

							bRredrawDockBar = TRUE;
							brd.m_pBar->MoveWindow( &rcMove,FALSE);
						}
					}
				}
			}

			if( nRedrawBarsAfter > 0 )
			{
				int nCount = vRepositionNext.GetSize();

				for( int i=0; i<nCount; i++)
				{
					BarRepositionData& brd = vRepositionNext[i];

					if(!brd.IsRectsEqual())
					{
						CRect rcMove(brd.m_rcDst);

						pDockBar->ScreenToClient(&rcMove);
						if(hDWP)
						{
							hDWP = ::DeferWindowPos(
								hDWP,
								brd.m_pBar->GetSafeHwnd(),
								NULL,
								rcMove.left,
								rcMove.top,
								rcMove.Width(),
								rcMove.Height(),
								nDwpFlags);
						}
						else
						{
							if(!bRredrawDockBar)
								pDockBar->LockWindowUpdate();

							bRredrawDockBar = TRUE;
							brd.m_pBar->MoveWindow( &rcMove, FALSE);
						}
					}
				}
			}

			if( rcFloatingBar != rcBarWnd )
			{
				CRect rcMove(rcFloatingBar);

				pDockBar->ScreenToClient(&rcMove);
				if(hDWP)
				{
					hDWP = ::DeferWindowPos(
						hDWP,
						m_pBarSrc->GetSafeHwnd(),
						NULL,
						rcMove.left,
						rcMove.top,
						rcMove.Width(),
						rcMove.Height(),
						nDwpFlags);
				}
				else
				{
					if(!bRredrawDockBar)
						pDockBar->LockWindowUpdate();

					bRredrawDockBar = TRUE;
					m_pBarSrc->MoveWindow( &rcMove, FALSE);
				}
			}

			if(hDWP)
				::EndDeferWindowPos(hDWP);
			else if(bRredrawDockBar)
			{
				pDockBar->UnlockWindowUpdate();
				CRect rcRowRedraw(rcRow);

				pDockBar->ScreenToClient(&rcRowRedraw);
				pDockBar->RedrawWindow(
					&rcRowRedraw,
					NULL,
					RDW_ERASE|
					RDW_INVALIDATE|
					RDW_UPDATENOW|
					RDW_ALLCHILDREN|
					RDW_NOFRAME);
			}

			CControlBar *pLastBar = m_pBarSrc->_GetFirstControlBarInRow(
				m_pBarSrc,
				TRUE,
				TRUE);

			if(pLastBar)
			{
				CTToolControlBar *pTB = DYNAMIC_DOWNCAST(
					CTToolControlBar,
					pLastBar);

				if(pTB)
				{
					CTBarContentExpandButton *pTBB = pTB->GetRightButton();

					if( pTBB && pTBB->GetButtons().GetSize() > 0 )
						pFrame->DelayRecalcLayout();
				}
			}

			return;
		}

		bForceFloat = TRUE;
	}

	if( bForceFloat && !bAlreadyFloat )
	{
		m_pBarSrc->m_ptFloatHelper = ptTrack - sizeOffset;

		if( m_pBarSrc->GetCapture() != m_pBarSrc )
			m_pBarSrc->SetCapture();

		m_pBarSrc->FloatControlBar();
		m_pBarSrc->m_pDockContext->m_ptMRUFloatPos = m_pBarSrc->m_ptFloatHelper;

		return;
	}

	BOOL bCanDockToInnerCircles = m_pBarSrc->_CanDockToInnerCircles();
	if(!bForceFloat)
	{
		CTDockBar *pPossibleDockBar = NULL;
		CRect rcDockBar;

		POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
		while(pos)
		{
			CTDockBar *pTempDockBar = (CTDockBar *) pFrame->m_listControlBars.GetNext(pos);

			if( pTempDockBar->IsDockBar() &&
				pTempDockBar->IsWindowVisible() &&
				(pTempDockBar->m_dwStyle&m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) && (
				!pTempDockBar->m_bFloating ||
				(m_pBarSrc->m_dwDockStyle&pTempDockBar->m_dwStyle&CBRS_FLOAT_MULTI)))
			{
				if( !bCanDockToInnerCircles &&
					pTempDockBar->_GetCircleNo() > 0 )
					continue;

				if( pTempDockBar->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)) ||
					pTempDockBar->GetDlgCtrlID() == AFX_IDW_DOCKBAR_FLOAT)
					continue;

				CFrameWnd *pTestFloatFrame = pTempDockBar->GetParentFrame();
				if( pTestFloatFrame && pTestFloatFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
					continue;

				pTempDockBar->GetWindowRect(&rcDockBar);
				int nDockedVisCount = pTempDockBar->GetDockedVisibleCount();

				if(!nDockedVisCount)
					rcDockBar.InflateRect( __DOCKING_SIDE_GAP, __DOCKING_SIDE_GAP);

				UINT nDockBarID = pTempDockBar->GetDlgCtrlID();
				BOOL bHorzDockBar = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
				CRect rcTest(rcDockBar);

				rcTest.InflateRect(
					bHorzDockBar ? 0 : nRdpInner,
					bHorzDockBar ? nRdpInner : 0);

				if(rcDockBar.PtInRect(ptTrack))
				{
					pPossibleDockBar = pTempDockBar;
					break;
				}
			}
		}

		BOOL bEnableDockToNewSide = TRUE;
		if( pPossibleDockBar != NULL &&
			pPossibleDockBar != m_pBarSrc->m_pDockBar)
		{
			UINT nDockBarID = pPossibleDockBar->GetDlgCtrlID();
			BOOL bHorzDockBar = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
			ControlBarVector vBars;

			CTControlBar::_GetFrameControlBars(
				pFrame,
				vBars,
				__GFECB_NONFIXED|
				__GFECB_FIXED|
				__GFECB_DOCKED|
				__GFECB_VIS_ALL|
				__GFECB_BYPT_ONLY_WND,
				ptTrack);

			int nFoundBars = vBars.GetSize();
			for( int i=0; i<nFoundBars; i++)
			{
				CTControlBar *pBar = vBars[i];

				if( pBar->m_pDockBar != pPossibleDockBar )
					continue;

				if( !pBar->IsFixedMode() || pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
				{
					CRect rcBar;

					int nDistDown = 0;
					int nDistUp = 0;

					pBar->GetWindowRect(&rcBar);
					if(bHorzDockBar)
					{
						nDistUp = ptTrack.y - rcBar.top;
						nDistDown = rcBar.bottom - ptTrack.y;
					}
					else
					{
						nDistUp = ptTrack.x - rcBar.left;
						nDistDown = rcBar.right - ptTrack.x;
					}

					if( abs(nDistUp) > nRowInner &&
						abs(nDistDown) > nRowInner )
					{
						bEnableDockToNewSide = FALSE;
						break;
					}
				}

				if(pBar->IsKindOf(RUNTIME_CLASS(CTMenuControlBar)))
					break;

				DWORD dwMode = 0;
				if(bHorzDockBar)
					dwMode |= LM_HORZ|LM_HORZDOCK;
				else
					dwMode |=  LM_VERTDOCK;

				CSize sizeDocked = m_pBarSrc->CalcFixedLayout(
					FALSE,
					bHorzDockBar);

				CRect rcInsert( ptTrack - sizeOffset, sizeDocked);
				pPossibleDockBar->_SlideDockControlBar(
					m_pBarSrc,
					&rcInsert,
					TRUE,
					&ptTrack);

				CControlBar *pNextBar = m_pBarSrc->_GetNextControlBarInRow(
					m_pBarSrc,
					TRUE,
					TRUE);

				if(pNextBar)
				{
					int nPos = pPossibleDockBar->FindBar(pNextBar);

					if(pPossibleDockBar->m_arrBars[nPos - 1])
						pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
				}

				pNextBar = m_pBarSrc->_GetNextControlBarInRow(
					m_pBarSrc,
					FALSE,
					TRUE);

				if(pNextBar)
				{
					int nPos = pPossibleDockBar->FindBar(pNextBar);

					if(pPossibleDockBar->m_arrBars[nPos - 1])
						pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
				}

				pFrame->RecalcLayout();
				return;
			}
		}
		else
			bEnableDockToNewSide = FALSE;

		if( !bEnableDockToNewSide && pPossibleDockBar )
		{
			UINT nDockBarID = pPossibleDockBar->GetDlgCtrlID();
			BOOL bHorzDockBar = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;

			int nDistDown = 0;
			int nDistUp = 0;
			CRect rcBar;

			pPossibleDockBar->GetWindowRect(&rcBar);
			if(bHorzDockBar)
			{
				nDistUp = ptTrack.y - rcBar.top;
				nDistDown = rcBar.bottom - ptTrack.y;
			}
			else
			{
				nDistUp = ptTrack.x - rcBar.left;
				nDistDown = rcBar.right - ptTrack.x;
			}

			if((nDistUp >= 0 && nDistUp < nRowInner) || (
				nDistDown >= 0 && nDistDown < nRowInner))
			{
				CSize sizeDocked = m_pBarSrc->CalcFixedLayout(
					FALSE,
					bHorzDockBar);

				CRect rcInsert( ptTrack - sizeOffset, sizeDocked);
				pPossibleDockBar->_SlideDockControlBar(
					m_pBarSrc,
					&rcInsert,
					TRUE,
					&ptTrack);

				CControlBar *pNextBar = m_pBarSrc->_GetNextControlBarInRow(
					m_pBarSrc,
					TRUE,
					TRUE);

				if(pNextBar)
				{
					int nPos = pPossibleDockBar->FindBar(pNextBar);

					if(pPossibleDockBar->m_arrBars[nPos - 1])
						pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
				}

				pNextBar = m_pBarSrc->_GetNextControlBarInRow(
					m_pBarSrc,
					FALSE,
					TRUE);

				if(pNextBar)
				{
					int nPos = pPossibleDockBar->FindBar(pNextBar);

					if(pPossibleDockBar->m_arrBars[nPos - 1])
						pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
				}

				pFrame->RecalcLayout();
				return;
			}
		}

		if(bEnableDockToNewSide)
		{
			UINT nDockBarID = pPossibleDockBar->GetDlgCtrlID();
			BOOL bHorzDockBar = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
			CSize sizeDocked = m_pBarSrc->CalcFixedLayout( FALSE, bHorzDockBar);
			CRect rcInsert( ptTrack - sizeOffset, sizeDocked);

			pPossibleDockBar->_SlideDockControlBar(
				m_pBarSrc,
				&rcInsert,
				TRUE,
				&ptTrack);

			CControlBar *pNextBar = m_pBarSrc->_GetNextControlBarInRow(
				m_pBarSrc,
				TRUE,
				TRUE);

			if(pNextBar)
			{
				int nPos = pPossibleDockBar->FindBar(pNextBar);

				if(pPossibleDockBar->m_arrBars[nPos - 1])
					pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
			}

			pNextBar = m_pBarSrc->_GetNextControlBarInRow(
				m_pBarSrc,
				FALSE,
				TRUE);

			if(pNextBar)
			{
				int nPos = pPossibleDockBar->FindBar(pNextBar);

				if(pPossibleDockBar->m_arrBars[nPos - 1])
					pPossibleDockBar->m_arrBars.InsertAt( nPos, (CControlBar *) NULL);
			}

			if( m_pBarSrc->GetCapture() != m_pBarSrc )
				m_pBarSrc->SetCapture();

			pFrame->RecalcLayout();
			return;
		}
	}

	CFrameWnd *pFloatingFrame = m_pBarSrc->GetParentFrame();
	CRect rcFloatingFrame;

	pFloatingFrame->GetWindowRect(&rcFloatingFrame);
	rcFloatingFrame.OffsetRect( -rcFloatingFrame.TopLeft() + (ptTrack - sizeOffset));

	CRect rcDesktop;
	::SystemParametersInfo(
		SPI_GETWORKAREA, 0,
		(PVOID) &rcDesktop, 0);

	if( rcFloatingFrame.top < rcDesktop.top )
		rcFloatingFrame.OffsetRect( 0, rcDesktop.top - rcFloatingFrame.top);

	if( rcFloatingFrame.left < rcDesktop.left )
		rcFloatingFrame.OffsetRect( rcDesktop.left - rcFloatingFrame.left, 0);

	if( rcFloatingFrame.top + 2 * afxData.cyBorder2 + nCyGripper > rcDesktop.bottom )
		rcFloatingFrame.OffsetRect( 0, rcDesktop.bottom - (rcFloatingFrame.top + 2 * afxData.cyBorder2 + nCyGripper));

	if( ptTrack.x > rcDesktop.right )
		rcFloatingFrame.OffsetRect( rcDesktop.right - ptTrack.x, 0);

	rcFloatingFrame.OffsetRect(-m_pBarSrc->m_ptHelperDragOffsetFloated);
	pFloatingFrame->MoveWindow(&rcFloatingFrame);
	pFloatingFrame->BringWindowToTop();
	pFloatingFrame->DelayRecalcLayout();
	m_pBarSrc->m_pDockContext->m_ptMRUFloatPos = rcFloatingFrame.TopLeft();

	if( m_pBarSrc->GetCapture() != m_pBarSrc )
		m_pBarSrc->SetCapture();
}

void CTControlBar::DraggingState::CalcStateDynamic( CPoint ptTrack)
{
	if(m_pBarSrc->IsFixedMode())
	{
		CalcStateFixed(ptTrack);
		return;
	}

	BOOL bShowContent = m_pBarSrc->_IsShowContentWhenDragging();
	if( bShowContent && m_pBarSrc->IsDocked() )
	{
		CRect rcDockBar;

		m_pBarSrc->m_pDockBar->GetWindowRect(&rcDockBar);
		if(rcDockBar.PtInRect(ptTrack))
			return;
	}

	TrackRectSetEmpty();
	static const DWORD _dwDockBarMap[4][2] = {
		{ AFX_IDW_DOCKBAR_TOP, CBRS_TOP},
		{ AFX_IDW_DOCKBAR_BOTTOM, CBRS_BOTTOM},
		{ AFX_IDW_DOCKBAR_LEFT, CBRS_LEFT},
		{ AFX_IDW_DOCKBAR_RIGHT, CBRS_RIGHT}};

	static const int nRdpOuter = 16;
	static const int nRdpInner = 2 * nRdpOuter;
	static const int nRdpMostInner = 12;
	static const int nRcpAnyHalfMin = 8;

	m_nCDT = __ECDT_FLOATED;
	m_pBarDst = NULL;
	CRect rcBarWnd;

	int nSrcMinHW = m_pBarSrc->_CalcDesiredMinHW();
	int nSrcMinVH = m_pBarSrc->_CalcDesiredMinVH();

	if( nSrcMinHW < nRcpAnyHalfMin )
		nSrcMinHW = nRcpAnyHalfMin;

	if( nSrcMinVH < nRcpAnyHalfMin )
		nSrcMinVH = nRcpAnyHalfMin;

	int nDstMinHW = nRcpAnyHalfMin;
	int nDstMinVH = nRcpAnyHalfMin;

	CFrameWnd *pFrame = m_pBarSrc->_GetDockingFrameImpl();
	CRect rcFrameWnd;

	pFrame->GetWindowRect(&rcFrameWnd);
	BOOL bForceFloat = FALSE;

	if( m_pBarSrc->IsFloating() && (
		m_pBarSrc->IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)) ||
		!bShowContent))
	{
		CFrameWnd *pFrameExcludeTest = m_pBarSrc->GetParentFrame();

		if(pFrameExcludeTest->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		{
			CRect rcFrameExcludeText;

			pFrameExcludeTest->GetWindowRect(&rcFrameExcludeText);
			if(rcFrameExcludeText.PtInRect(ptTrack))
				bForceFloat = TRUE;
		}
	}

	if( !CTPopupMenuWnd::IsKeyPressed(VK_CONTROL) && !bForceFloat )
	{
		BOOL bCanDockToInnerCircles = m_pBarSrc->_CanDockToInnerCircles();

		if( bCanDockToInnerCircles && !bShowContent )
		{
			ControlBarVector vBars;

			m_pBarSrc->_GetFrameControlBars(
				vBars,
				(DWORD) __GFECB_NONFIXED_NONMIN|
				__GFECB_FLOATED|
				__GFECB_VIS_ALL|
				__GFECB_BYPT_ONLY_WND,
				ptTrack);

			CTControlBar *pDesiredBar = NULL;
			int nCount = vBars.GetSize();

			for( int i=0; i<nCount; i++)
			{
				CTControlBar *pTestBar = vBars[i];

				if( pTestBar == m_pBarSrc )
					continue;

				pDesiredBar = pTestBar;
				break;
			}

			if(pDesiredBar)
			{
				BarDstSet(pDesiredBar);
				m_pBarDst->GetParentFrame()->GetWindowRect(&rcBarWnd);

				nDstMinHW = m_pBarDst->_CalcDesiredMinHW();
				nDstMinVH = m_pBarDst->_CalcDesiredMinVH();

				int nHeight3  = nRdpInner;
				int nWidth3 = nRdpInner;

				CDockBar *pDockBar = STATIC_DOWNCAST(
					CDockBar,
					m_pBarDst->m_pDockBar);

				BOOL bEnableInnerDock = TRUE;
				if( bEnableInnerDock &&
					(m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY))
				{
					m_bDeepHalfSplit = FALSE;

					if( !m_pBarDst->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) ||
						m_pBarDst->IsFloating())
						m_bDeepHalfSplit = TRUE;

					CRect rcWndDockBar;
					pDockBar->GetWindowRect(&rcWndDockBar);

					CRect rcPart(rcBarWnd);
					while(TRUE)
					{
						int nHalfHeight = rcBarWnd.Height() / 2;

						if(	rcBarWnd.Width() >= nSrcMinHW &&
							nHalfHeight >= nSrcMinVH &&
							nHalfHeight >= nDstMinVH )
						{
							rcPart = rcBarWnd;
							rcPart.bottom = rcPart.top + nHeight3;

							if(rcPart.PtInRect(ptTrack))
							{
								m_bDeepHalfSplit = TRUE;
								m_nCDT = __ECDT_INNER_TOP;

								rcPart.bottom = rcPart.top + rcBarWnd.Height() / 2;
								rcPart.left = min( rcPart.left, rcWndDockBar.left);
								rcPart.right = max( rcPart.right, rcWndDockBar.right);
								break;
							}

							rcPart = rcBarWnd;
							rcPart.top = rcPart.bottom - nHeight3;

							if(rcPart.PtInRect(ptTrack))
							{
								m_bDeepHalfSplit = TRUE;
								m_nCDT = __ECDT_INNER_BOTTOM;
								rcPart.top = rcPart.bottom - rcBarWnd.Height() / 2;
								rcPart.left = min( rcPart.left, rcWndDockBar.left);
								rcPart.right = max( rcPart.right, rcWndDockBar.right);

								break;
							}
						}

						int nHalfWidth = rcBarWnd.Width()/2;
						if( rcBarWnd.Height() >= nSrcMinVH &&
							nHalfWidth >= nSrcMinHW &&
							nHalfWidth >= nDstMinHW )
						{
							rcPart = rcBarWnd;
							rcPart.right = rcPart.left + nWidth3;

							if(rcPart.PtInRect(ptTrack))
							{
								m_bDeepHalfSplit = TRUE;
								m_nCDT = __ECDT_INNER_LEFT;
								rcPart.right = rcPart.left + rcBarWnd.Width() / 2;
								rcPart.top = min( rcPart.top, rcWndDockBar.top);
								rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom);

								break;
							}

							rcPart = rcBarWnd;
							rcPart.left = rcPart.right - nWidth3;

							if(rcPart.PtInRect(ptTrack))
							{
								m_bDeepHalfSplit = TRUE;
								m_nCDT = __ECDT_INNER_RIGHT;

								rcPart.left = rcPart.right - rcBarWnd.Width() / 2;
								rcPart.top = min( rcPart.top, rcWndDockBar.top);
								rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom);

								break;
							}
						}

						break;
					}

					if( m_nCDT != __ECDT_FLOATED )
					{
						if( rcPart.Width() < nSrcMinHW )
							m_nCDT = __ECDT_FLOATED;

						if( rcPart.Height() < nSrcMinVH )
							m_nCDT = __ECDT_FLOATED;

						if( m_nCDT != __ECDT_FLOATED )
							rcBarWnd = rcPart;
					}
				}
			}
		}

		if( bCanDockToInnerCircles && m_nCDT == __ECDT_FLOATED )
		{
			CRect rcCircle0( 0, 0, 0, 0);
			CTDockBar *vDockBars0[4];

			for( int i=0; i<4; i++)
			{
				UINT nDockBarID = _dwDockBarMap[i][0];
				vDockBars0[i] = (CTDockBar *) pFrame->GetControlBar(nDockBarID);

				CRect rcDockBar;
				vDockBars0[i]->_GetPreCalcWindowRect(rcDockBar);

				switch(nDockBarID)
				{
				case AFX_IDW_DOCKBAR_TOP		: rcCircle0.top = rcDockBar.bottom; break;
				case AFX_IDW_DOCKBAR_BOTTOM		: rcCircle0.bottom = rcDockBar.top; break;
				case AFX_IDW_DOCKBAR_LEFT		: rcCircle0.left = rcDockBar.right; break;
				case AFX_IDW_DOCKBAR_RIGHT		: rcCircle0.right = rcDockBar.left; break;
				}
			}

			CRect rcTestCircle0(rcCircle0);
			rcTestCircle0.InflateRect( nRdpMostInner, nRdpMostInner);

			if( rcCircle0.left < rcCircle0.right &&
				rcCircle0.top < rcCircle0.bottom &&
				rcTestCircle0.PtInRect(ptTrack))
			{
				for( i=0; i<4; i++)
				{
					DWORD dwDockStyle = _dwDockBarMap[i][1];

					if(!(m_pBarSrc->m_dwDockStyle&dwDockStyle))
						continue;

					UINT nDockBarID = _dwDockBarMap[i][0];
					CRect rcFrameAnalyze(rcTestCircle0);

					switch(nDockBarID)
					{
					case AFX_IDW_DOCKBAR_TOP	:
						{
							rcFrameAnalyze.bottom = rcFrameAnalyze.top + nRdpOuter;

							if(rcFrameAnalyze.PtInRect(ptTrack))
							{
								m_nCDT = __ECDT_CIRCLE1_OUTER_TOP;
								m_nCircleNo = 1;

								rcBarWnd.top = rcCircle0.top;
								rcBarWnd.bottom = rcBarWnd.top + m_pBarSrc->m_szDockedH.cy;
								rcBarWnd.left = rcCircle0.left;
								rcBarWnd.right = rcCircle0.right;
							}
						}

						break;

					case AFX_IDW_DOCKBAR_BOTTOM	:
						{
							rcFrameAnalyze.top = rcFrameAnalyze.bottom - nRdpOuter;

							if(rcFrameAnalyze.PtInRect(ptTrack))
							{
								m_nCDT = __ECDT_CIRCLE1_OUTER_BOTTOM;
								m_nCircleNo = 1;

								rcBarWnd.bottom = rcCircle0.bottom;
								rcBarWnd.top = rcBarWnd.bottom - m_pBarSrc->m_szDockedH.cy;
								rcBarWnd.left = rcCircle0.left;
								rcBarWnd.right = rcCircle0.right;
							}
						}

						break;

					case AFX_IDW_DOCKBAR_LEFT	:
						{
							rcFrameAnalyze.right = rcFrameAnalyze.left + nRdpOuter;

							if(rcFrameAnalyze.PtInRect(ptTrack))
							{
								m_nCDT = __ECDT_CIRCLE1_OUTER_LEFT;
								m_nCircleNo = 1;

								rcBarWnd.left = rcCircle0.left;
								rcBarWnd.right = rcBarWnd.left + m_pBarSrc->m_szDockedV.cx;
								rcBarWnd.top = rcCircle0.top;
								rcBarWnd.bottom = rcCircle0.bottom;
							}
						}

						break;

					case AFX_IDW_DOCKBAR_RIGHT	:
						{
							rcFrameAnalyze.left = rcFrameAnalyze.right - nRdpOuter;

							if(rcFrameAnalyze.PtInRect(ptTrack))
							{
								m_nCDT = __ECDT_CIRCLE1_OUTER_RIGHT;
								m_nCircleNo = 1;

								rcBarWnd.right = rcCircle0.right;
								rcBarWnd.left = rcBarWnd.right - m_pBarSrc->m_szDockedV.cx;
								rcBarWnd.top = rcCircle0.top;
								rcBarWnd.bottom = rcCircle0.bottom;
							}
						}

						break;
					}

					if( m_nCDT != __ECDT_FLOATED )
						break;
				}
			}

			if( m_nCDT == __ECDT_FLOATED )
			{
				CRect rcFrameTest(rcFrameWnd);

				rcFrameTest.InflateRect( nRdpInner, nRdpInner);
				if(rcFrameTest.PtInRect(ptTrack))
				{
					CRect rcFrameInner;

					pFrame->GetClientRect(&rcFrameInner);
					RepositionBarsEx(
						pFrame,
						0, 0xFFFFFFFF,
						AFX_IDW_PANE_FIRST,
						CWnd::reposQuery,
						rcFrameInner,
						rcFrameInner);
					pFrame->ClientToScreen(&rcFrameInner);

					if( rcFrameInner.left < rcFrameInner.right &&
						rcFrameInner.top < rcFrameInner.bottom &&
						rcFrameInner.PtInRect(ptTrack))
					{
						CSize _sizeMaxInner = rcFrameInner.Size();

						_sizeMaxInner.cx = ::MulDiv( _sizeMaxInner.cx, 3, 4);
						_sizeMaxInner.cy = ::MulDiv( _sizeMaxInner.cy, 3, 4);

						UINT nSrcBarCircleNo = ((CTDockBar *) m_pBarSrc->m_pDockBar)->_GetCircleNo();
						for( i=0; i<4; i++)
						{
							DWORD dwDockStyle = _dwDockBarMap[i][1];

							if(!(m_pBarSrc->m_dwDockStyle&dwDockStyle))
								continue;

							UINT nDockBarID = _dwDockBarMap[i][0];
							CTDockBar *pDockBar = vDockBars0[i];

							if( bShowContent && m_pBarSrc->IsDocked() &&
								pDockBar == m_pBarSrc->m_pDockBar )
							{
								CRect rcDockBar;

								pDockBar->GetWindowRect(&rcDockBar);
								if(rcDockBar.PtInRect(ptTrack))
								{
									m_nCDT = __ECDT_OWN_AREA;
									nDstMinHW = nSrcMinHW;
									nDstMinVH = nSrcMinVH;

									return;
								}

								continue;
							}

							pDockBar = pDockBar->_GetDockBarInnerTop();
							CRect rcFrameAnalyze(rcFrameInner);

							switch(nDockBarID)
							{
							case AFX_IDW_DOCKBAR_TOP		:
								{
									rcFrameAnalyze.bottom = rcFrameAnalyze.top + nRdpMostInner;

									if(rcFrameAnalyze.PtInRect(ptTrack))
									{
										m_nCDT = __ECDT_DOCKBAR_INNER_TOP;
										m_nCircleNo = pDockBar->_GetCircleNo();

										BOOL bDeepCircleTarget = FALSE;
										if(	m_nCircleNo <= nSrcBarCircleNo + 2 )
											bDeepCircleTarget = TRUE;

										if(!bDeepCircleTarget)
										{
											CTDockBar *pBarLeft = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_LEFT);
											CTDockBar *pBarRight = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_RIGHT);

											if( !pBarLeft->_CanBeSafeOptimized() ||
												!pBarLeft->_CanBeSafeOptimized())
												bDeepCircleTarget = TRUE;
										}

										if(bDeepCircleTarget)
											m_nCircleNo++;

										rcBarWnd.top = rcFrameInner.top;
										rcBarWnd.bottom = rcBarWnd.top + min( m_pBarSrc->m_szDockedH.cy, _sizeMaxInner.cy);
										rcBarWnd.left = rcFrameInner.left;
										rcBarWnd.right = rcFrameInner.right;
									}
								}

								break;

							case AFX_IDW_DOCKBAR_BOTTOM		:
								{
									rcFrameAnalyze.top = rcFrameAnalyze.bottom - nRdpMostInner;

									if(rcFrameAnalyze.PtInRect(ptTrack))
									{
										m_nCDT = __ECDT_DOCKBAR_INNER_BOTTOM;
										m_nCircleNo = pDockBar->_GetCircleNo();

										BOOL bDeepCircleTarget = FALSE;
										if(	m_nCircleNo <= nSrcBarCircleNo + 2 )
											bDeepCircleTarget = TRUE;

										if(!bDeepCircleTarget)
										{
											CTDockBar *pBarLeft = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_LEFT);
											CTDockBar *pBarRight = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_RIGHT);

											if( !pBarLeft->_CanBeSafeOptimized() ||
												!pBarLeft->_CanBeSafeOptimized())
												bDeepCircleTarget = TRUE;
										}

										if(bDeepCircleTarget)
											m_nCircleNo++;

										rcBarWnd.bottom = rcFrameInner.bottom;
										rcBarWnd.top = rcBarWnd.bottom - min(
											m_pBarSrc->m_szDockedH.cy,
											_sizeMaxInner.cy);

										rcBarWnd.left = rcFrameInner.left;
										rcBarWnd.right = rcFrameInner.right;
									}
								}

								break;

							case AFX_IDW_DOCKBAR_LEFT		:
								{
									rcFrameAnalyze.right = rcFrameAnalyze.left + nRdpMostInner;

									if(rcFrameAnalyze.PtInRect(ptTrack))
									{
										m_nCDT = __ECDT_DOCKBAR_INNER_LEFT;
										m_nCircleNo = pDockBar->_GetCircleNo();

										BOOL bDeepCircleTarget = FALSE;
										if(	m_nCircleNo <= nSrcBarCircleNo + 2 )
											bDeepCircleTarget = TRUE;

										if(!bDeepCircleTarget)
										{
											CTDockBar *pBarTop = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_TOP);
											CTDockBar *pBarBottom = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_BOTTOM);

											if( !pBarTop->_CanBeSafeOptimized() ||
												!pBarBottom->_CanBeSafeOptimized())
												bDeepCircleTarget = TRUE;
										}

										if(bDeepCircleTarget)
											m_nCircleNo++;

										rcBarWnd.left = rcFrameInner.left;
										rcBarWnd.right = rcBarWnd.left + min(
											m_pBarSrc->m_szDockedV.cx,
											_sizeMaxInner.cx);

										rcBarWnd.top = rcFrameInner.top;
										rcBarWnd.bottom = rcFrameInner.bottom;
									}
								}

								break;

							case AFX_IDW_DOCKBAR_RIGHT		:
								{
									rcFrameAnalyze.left = rcFrameAnalyze.right - nRdpMostInner;

									if(rcFrameAnalyze.PtInRect(ptTrack))
									{
										m_nCDT = __ECDT_DOCKBAR_INNER_RIGHT;
										m_nCircleNo = pDockBar->_GetCircleNo();

										BOOL bDeepCircleTarget = FALSE;
										if(	m_nCircleNo <= nSrcBarCircleNo + 2 )
											bDeepCircleTarget = TRUE;

										if(!bDeepCircleTarget)
										{
											CTDockBar *pBarTop = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_TOP);
											CTDockBar *pBarBottom = pDockBar->_GetInCircle(AFX_IDW_DOCKBAR_BOTTOM);

											if( !pBarTop->_CanBeSafeOptimized() ||
												!pBarBottom->_CanBeSafeOptimized())
												bDeepCircleTarget = TRUE;
										}

										if(bDeepCircleTarget)
											m_nCircleNo++;

										rcBarWnd.right = rcFrameInner.right;
										rcBarWnd.left = rcBarWnd.right - min(
											m_pBarSrc->m_szDockedV.cx,
											_sizeMaxInner.cx);

										rcBarWnd.top = rcFrameInner.top;
										rcBarWnd.bottom = rcFrameInner.bottom;
									}
								}

								break;
							}

							if( m_nCDT != __ECDT_FLOATED )
								break;
						}
					}
				}
			}
		}

		if( m_nCDT == __ECDT_FLOATED )
		{
			ControlBarVector vBars;

			m_pBarSrc->_GetFrameControlBars(
				vBars,
				(DWORD) __GFECB_NONFIXED_NONMIN|
				__GFECB_DOCKED|
				__GFECB_VIS_ALL|
				__GFECB_BYPT_ONLY_WND,
				ptTrack);

			int nCount = vBars.GetSize();
			if( nCount == 1 )
			{
				m_pBarDst = vBars[0];
				m_pBarDst->GetWindowRect(&rcBarWnd);

				if( m_pBarDst == m_pBarSrc )
				{
					m_nCDT = __ECDT_OWN_AREA;

					nDstMinHW = nSrcMinHW;
					nDstMinVH = nSrcMinVH;
				}
				else if(!bShowContent)
				{
					m_nCDT = __ECDT_FLOATED;

					BOOL bCanAnalyzeInnerParts = TRUE;
					if(!bCanDockToInnerCircles)
					{
						CTDockBar *pDockBarDst = STATIC_DOWNCAST(
							CTDockBar,
							m_pBarDst->m_pDockBar);

						if( pDockBarDst->_GetCircleNo() > 0 )
							bCanAnalyzeInnerParts = FALSE;
					}

					if( bCanAnalyzeInnerParts && m_pBarDst )
					{
						nDstMinHW = m_pBarDst->_CalcDesiredMinHW();
						nDstMinVH = m_pBarDst->_CalcDesiredMinVH();
						int nWidth3 = nRdpInner;
						int nHeight3 = nRdpInner;
						UINT nDockBarID = m_pBarDst->GetSafeDockBarDlgCtrlID();

						if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
							nDockBarID = AFX_IDW_DOCKBAR_TOP;

						BOOL bHorz = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
						CDockBar *pDockBar = m_pBarDst->m_pDockBar;
						BOOL bEnableInnerDock = TRUE;

						if( nDockBarID == AFX_IDW_DOCKBAR_LEFT &&
							!(m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_LEFT))
							bEnableInnerDock = FALSE;

						if( nDockBarID == AFX_IDW_DOCKBAR_RIGHT &&
							!(m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_RIGHT))
							bEnableInnerDock = FALSE;

						if( nDockBarID == AFX_IDW_DOCKBAR_TOP &&
							!(m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_TOP))
							bEnableInnerDock = FALSE;

						if( nDockBarID == AFX_IDW_DOCKBAR_BOTTOM &&
							!(m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_BOTTOM))
							bEnableInnerDock = FALSE;

						if( bEnableInnerDock && (m_pBarSrc->m_dwDockStyle&CBRS_ALIGN_ANY) )
						{
							m_bDeepHalfSplit = FALSE;

							if( !m_pBarDst->m_pDockBar->IsKindOf(RUNTIME_CLASS(CTDockBar)) ||
								m_pBarDst->IsFloating())
								m_bDeepHalfSplit = TRUE;

							CRect rcWndDockBar;
							pDockBar->GetWindowRect(&rcWndDockBar);

							CRect rcPart(rcBarWnd);
							while(TRUE)
							{
								int nHalfHeight = 2 * nRdpInner;

								if(	rcBarWnd.Width() >= nSrcMinHW &&
									nHalfHeight >= nSrcMinVH &&
									nHalfHeight >= nDstMinVH)
								{
									rcPart = rcBarWnd;
									rcPart.bottom = rcPart.top + nHeight3;

									if(rcPart.PtInRect(ptTrack))
									{
										if( !bShowContent || !bHorz )
										{
											CRect rcDeep(
												rcPart.left,
												rcPart.top+rcPart.Height() / 2,
												rcPart.right,
												rcPart.bottom);

											m_bDeepHalfSplit = rcDeep.PtInRect(ptTrack);
											m_nCDT = __ECDT_INNER_TOP;
											rcPart.bottom = rcPart.top + rcBarWnd.Height() / 2;

											if( bHorz && !m_bDeepHalfSplit )
											{
												rcPart.left = min( rcPart.left, rcWndDockBar.left);
												rcPart.right = max( rcPart.right, rcWndDockBar.right);
											}

											break;
										}
									}

									rcPart = rcBarWnd;
									rcPart.top = rcPart.bottom - nHeight3;

									if(rcPart.PtInRect(ptTrack))
									{
										if( !bShowContent || !bHorz )
										{
											CRect rcDeep(
												rcPart.left,
												rcPart.top,
												rcPart.right,
												rcPart.bottom-rcPart.Height() / 2);

											m_bDeepHalfSplit = rcDeep.PtInRect(ptTrack);
											m_nCDT = __ECDT_INNER_BOTTOM;

											rcPart.top = rcPart.bottom - rcBarWnd.Height() / 2;
											if( bHorz && !m_bDeepHalfSplit )
											{
												rcPart.left = min( rcPart.left, rcWndDockBar.left);
												rcPart.right = max( rcPart.right, rcWndDockBar.right);
											}

											break;
										}
									}
								}

								int nHalfWidth = 2 * nRdpInner;
								if( rcBarWnd.Height() >= nSrcMinVH &&
									nHalfWidth >= nSrcMinHW &&
									nHalfWidth >= nDstMinHW )
								{
									rcPart = rcBarWnd;
									rcPart.right = rcPart.left + nWidth3;

									if(rcPart.PtInRect(ptTrack))
									{
										if( !bShowContent || bHorz )
										{
											CRect rcDeep(
												rcPart.left+rcPart.Width() / 2,
												rcPart.top,
												rcPart.right,
												rcPart.bottom);

											m_bDeepHalfSplit = rcDeep.PtInRect(ptTrack);
											m_nCDT = __ECDT_INNER_LEFT;
											rcPart.right = rcPart.left + rcBarWnd.Width() / 2;

											if( !bHorz && !m_bDeepHalfSplit )
											{
												rcPart.top = min( rcPart.top, rcWndDockBar.top);
												rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom);
											}

											break;
										}
									}

									rcPart = rcBarWnd;
									rcPart.left = rcPart.right - nWidth3;

									if(rcPart.PtInRect(ptTrack))
									{
										if( !bShowContent || bHorz )
										{
											CRect rcDeep(
												rcPart.left,
												rcPart.top,
												rcPart.right-rcPart.Width() / 2,
												rcPart.bottom);

											m_bDeepHalfSplit = rcDeep.PtInRect(ptTrack);
											m_nCDT = __ECDT_INNER_RIGHT;
											rcPart.left = rcPart.right - rcBarWnd.Width() / 2;

											if( !bHorz && !m_bDeepHalfSplit )
											{
												rcPart.top = min( rcPart.top, rcWndDockBar.top);
												rcPart.bottom = max( rcPart.bottom, rcWndDockBar.bottom);
											}

											break;
										}
									}
								}

								break;
							}

							if( m_bDeepHalfSplit &&
								!m_pBarDst->IsFloating() &&
								m_pBarDst->IsSingleOnRow())
								m_bDeepHalfSplit = FALSE;

							if( m_nCDT != __ECDT_FLOATED )
							{
								if(bHorz)
								{
									if( rcPart.Width() < nSrcMinHW )
										m_nCDT = __ECDT_FLOATED;
								}
								else
								{
									if( rcPart.Height() < nSrcMinVH )
										m_nCDT = __ECDT_FLOATED;
								}

								if( m_nCDT != __ECDT_FLOATED )
									rcBarWnd = rcPart;
							}
						}
					}
				}
			}
		}
	}

	if( m_nCDT == __ECDT_FLOATED )
	{
		BarDstSet(NULL);

		if(m_pBarSrc->IsFloating())
		{
			CMiniDockFrameWnd *pMiniFrame = DYNAMIC_DOWNCAST(
				CMiniDockFrameWnd,
				m_pBarSrc->GetDockingFrame());

			pMiniFrame->GetWindowRect(&rcBarWnd);
			rcBarWnd.OffsetRect(-rcBarWnd.TopLeft());
			rcBarWnd.OffsetRect(ptTrack - m_pBarSrc->m_ptHelperDragOffsetFloated);

			int nCyGripper = min(
				m_pBarSrc->m_rcGrip.Width(),
				m_pBarSrc->m_rcGrip.Height());

			rcBarWnd.OffsetRect( -nCyGripper, -nCyGripper / 2);
		}
		else
		{
			rcBarWnd.SetRect(
				ptTrack,
				ptTrack + m_pBarSrc->m_szFloated);

			rcBarWnd.OffsetRect(-m_pBarSrc->m_ptHelperDragOffsetFloated);
		}

		rcBarWnd.OffsetRect(rcFrameWnd.TopLeft());
	}

	TrackRectSet(&rcBarWnd);
}

#define __RECALC_FRAME_STEPS(__FRAME_PTR__) \
	{\
	(__FRAME_PTR__)->RecalcLayout(); \
	(__FRAME_PTR__)->RedrawWindow( \
	NULL, NULL, \
	RDW_VALIDATE \
	|RDW_INVALIDATE|RDW_UPDATENOW \
	|RDW_ERASE|RDW_ERASENOW \
	|RDW_FRAME \
	|RDW_ALLCHILDREN);\
	}

void CTControlBar::_DraggingDoDeepHalfSplit( CTControlBar *pBar, UINT nHalfSplitID)
{
	CDockBar *pDockBarIn = m_pDockBar;
	CDockBar *pDockBarOut = pBar->m_pDockBar;
	CRect rcDyn;

	GetWindowRect(&rcDyn);
	CRect rcIn(rcDyn);
	CRect rcOut(rcDyn);

	pDockBarIn->ScreenToClient(&rcDyn);
	UINT nNewID = AFX_IDW_DOCKBAR_TOP;
	DWORD nBarStyle = CBRS_ALIGN_TOP;

	switch(nHalfSplitID)
	{
	case AFX_IDW_DOCKBAR_TOP		:
		{
			nNewID = AFX_IDW_DOCKBAR_LEFT;
			nBarStyle = CBRS_ALIGN_LEFT;
			rcIn.top += rcIn.Height() / 2;
			rcOut.bottom = rcIn.top;
		}

		break;

	case AFX_IDW_DOCKBAR_BOTTOM		:
		{
			nNewID = AFX_IDW_DOCKBAR_LEFT;
			nBarStyle = CBRS_ALIGN_LEFT;
			rcIn.bottom -= rcIn.Height() / 2;
			rcOut.top = rcIn.bottom;
		}

		break;

	case AFX_IDW_DOCKBAR_LEFT		:
		{
			rcIn.left += rcIn.Width() / 2;
			rcOut.right = rcIn.left;
		}

		break;

	case AFX_IDW_DOCKBAR_RIGHT		:
		{
			rcIn.right -= rcIn.Width() / 2;
			rcOut.left = rcIn.right;
		}

		break;
	}

	DWORD dwDynStyle = GetStyle()|(GetBarStyle()&CBRS_ALIGN_ANY);
	dwDynStyle |= CBRS_BORDER_ANY;
	dwDynStyle &= ~(CBRS_FLOATING|CBRS_GRIPPER);
	dwDynStyle |= CBRS_TOOLTIPS;

	CTDynamicControlBar *pDynBar = new CTDynamicControlBar();

	pDynBar->SetInitDesiredSizeHorizontal(m_szDockedH);
	pDynBar->SetInitDesiredSizeVertical(m_szDockedV);

	if(!pDynBar->Create(
		NULL,
		m_pDockSite,
		nNewID,
		dwDynStyle))
		return;

	pDynBar->m_pDockSite = m_pDockSite;
	pDynBar->m_pDockBar = m_pDockBar;
	pDynBar->EnableDocking(CBRS_ALIGN_ANY);

	int nPosIn = pDockBarIn->FindBar(this);
	pDockBarIn->m_arrBars[nPosIn] = pDynBar;

	pDynBar->m_pDockBar->m_bFloating = m_pDockBar->m_bFloating;
	m_pDockBar = pDynBar->m_pWndDynDocker;

	pBar->m_pDockBar = pDynBar->m_pWndDynDocker;
	m_pDockBar->m_bFloating = FALSE;

	if(pDynBar->m_pDockBar->m_bFloating)
		pDynBar->m_pDockBar->SetBarStyle(pDynBar->m_pDockBar->GetBarStyle()|CBRS_FLOATING);

	m_pDockBar->SetBarStyle(m_pDockBar->GetBarStyle()&(~CBRS_FLOATING));
	m_bUpdatingChain = TRUE;

	((CTControlBar *) pDynBar)->m_bUpdatingChain = TRUE;
	pBar->m_bUpdatingChain = TRUE;

	SetBarStyle(GetBarStyle()&(~CBRS_FLOATING));
	pBar->SetBarStyle(pBar->GetBarStyle()&(~CBRS_FLOATING));

	pDynBar->SetParent(pDockBarIn);
	SetParent(pDynBar->m_pWndDynDocker);
	pBar->SetParent(pDynBar->m_pWndDynDocker);

	if(pDockBarOut)
		if( !pDockBarOut->m_bFloating &&
			pDockBarOut->IsKindOf(RUNTIME_CLASS(CTDockBar)))
			((CTDockBar *) pDockBarOut)->RemoveControlBar( pBar, -1, 0, FALSE);
		else
			pDockBarOut->RemoveControlBar(pBar);

	if( nHalfSplitID == AFX_IDW_DOCKBAR_TOP ||
		nHalfSplitID == AFX_IDW_DOCKBAR_LEFT )
	{
		pDynBar->m_pWndDynDocker->m_arrBars.Add(pBar);
		pDynBar->m_pWndDynDocker->m_arrBars.Add(this);
	}
	else
	{
		pDynBar->m_pWndDynDocker->m_arrBars.Add(this);
		pDynBar->m_pWndDynDocker->m_arrBars.Add(pBar);
	}

	pDynBar->m_pWndDynDocker->m_arrBars.Add(NULL);
	SetInitDesiredSizeHorizontal(rcIn.Size());
	SetInitDesiredSizeVertical(rcIn.Size());

	pBar->SetInitDesiredSizeHorizontal(rcOut.Size());
	pBar->SetInitDesiredSizeVertical(rcOut.Size());

	pDynBar->ScreenToClient(&rcIn);
	pDynBar->ScreenToClient(&rcOut);
	MoveWindow( &rcIn, FALSE);
	pBar->MoveWindow( &rcOut, FALSE);

	pDynBar->SetInitDesiredSizeFloating(rcDyn.Size());
	pDynBar->MoveWindow( &rcDyn, FALSE);

	m_bUpdatingChain = FALSE;
	((CTControlBar *) pDynBar)->m_bUpdatingChain = FALSE;
	pBar->m_bUpdatingChain = FALSE;
	CTDockBar::_OptimizeCircles(m_pDockSite);

	CFrameWnd *pFrame = pDynBar->GetParentFrame();
	if(pFrame)
	{
		__RECALC_FRAME_STEPS(pFrame);

		pDynBar->OnRepositionSingleChild();
		pDynBar->OnNcAreaButtonsReinitialize();
		pDynBar->OnNcAreaButtonsReposition();

		if(pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
			pFrame->DelayRecalcLayout();
	}
}

BOOL CTControlBar::_DraggingApplyState( CTControlBar::DraggingState& _ds)
{
	if(_ds.IsEmpty())
		return FALSE;

	CTControlBar *pBarSrc = _ds.BarSrcGet();
	if(pBarSrc->IsFixedMode())
		return FALSE;

	CFrameWnd *pFrame = pBarSrc->_GetDockingFrameImpl();
	CFrameWnd *pOldFrame = pBarSrc->GetParentFrame();
	HWND hWndOldFrame = pOldFrame->GetSafeHwnd();

	CTControlBar *pBarDst = _ds.BarDstGet();
	ControlBarVector vBarsDstRow;
	int nCountOfDstRowBars = 0;
	int nDstBarIdx = -1;

	UINT nDstDockBarID = AFX_IDW_DOCKBAR_FLOAT;
	CDockBar *pDstDockBar = NULL;
	BOOL bDstHorzDockBar = TRUE;
	BOOL bDstFloatBar = FALSE;

	if(pBarDst)
	{
		pBarDst->_GetRowBars(vBarsDstRow);
		nCountOfDstRowBars = vBarsDstRow.GetSize();

		nDstDockBarID = pBarDst->GetSafeDockBarDlgCtrlID();
		pDstDockBar = pBarDst->m_pDockBar;

		if( nDstDockBarID == AFX_IDW_DOCKBAR_LEFT ||
			nDstDockBarID == AFX_IDW_DOCKBAR_RIGHT )
			bDstHorzDockBar = FALSE;

		if( nDstDockBarID == AFX_IDW_DOCKBAR_FLOAT )
			bDstFloatBar = TRUE;

		nDstBarIdx = pDstDockBar->FindBar(pBarDst);
	}

	BOOL bInitialMode = FALSE;
	if(!pBarSrc->m_pDockBar)
		bInitialMode = TRUE;

	CRect rcTrack(_ds.TrackRectGet());
	e_calc_dock_type nCDT = _ds.GetCDT();

	switch(nCDT)
	{
	case __ECDT_OWN_AREA			: m_pDockSite->DelayRecalcLayout(); return TRUE;
	case __ECDT_FLOATED				:
		{
			BOOL bWasFloating = TRUE;

			if(!pBarSrc->IsFloating())
			{
				bWasFloating = FALSE;
				CTControlBar *pUpSizeBar = (CTControlBar *) pBarSrc->_GetNextControlBarInRow(
					pBarSrc,
					TRUE,
					TRUE,
					TRUE);

				if(pUpSizeBar)
				{
					while(!pUpSizeBar->IsVisible())
					{
						CTControlBar *pUpSizeBar2 = (CTControlBar *) _GetNextControlBarInRow(
							pUpSizeBar,
							TRUE,
							TRUE,
							TRUE);

						if(!pUpSizeBar2)
							break;

						pUpSizeBar = pUpSizeBar2;
					}
				}

				if(!pUpSizeBar)
				{
					pUpSizeBar = (CTControlBar *) pBarSrc->_GetNextControlBarInRow(
						pBarSrc,
						FALSE,
						TRUE,
						TRUE);

					if(pUpSizeBar)
					{
						while(!pUpSizeBar->IsVisible())
						{
							CTControlBar *pUpSizeBar2 = (CTControlBar *) _GetNextControlBarInRow(
								pUpSizeBar,
								FALSE,
								TRUE,
								TRUE);

							if(!pUpSizeBar2)
								break;

							pUpSizeBar = pUpSizeBar2;
						}
					}
				}

				if(pUpSizeBar)
				{
					CRect rcBarWnd;
					pBarSrc->GetWindowRect(&rcBarWnd);

					BOOL bHorzUpsizing = pBarSrc->IsDockedHorizontally();
					LONG nUpSize = bHorzUpsizing ? rcBarWnd.Width() : rcBarWnd.Height();
					LONG &ref_nMetric = bHorzUpsizing ? pUpSizeBar->m_szDockedH.cx : pUpSizeBar->m_szDockedV.cy;

					ref_nMetric += nUpSize;
				}
			}

			CRect rcFrame;
			pFrame->GetWindowRect(&rcFrame);

			CPoint ptFloat(-rcFrame.TopLeft());
			ptFloat += rcTrack.TopLeft();
			pBarSrc->FloatControlBar(ptFloat);
			m_pDockContext->m_ptMRUFloatPos = ptFloat;

			if(pBarSrc->_IsShowContentWhenDragging())
			{
				pBarSrc->RedrawWindow(
					NULL,
					NULL,
					RDW_INVALIDATE|
					RDW_UPDATENOW|
					RDW_ERASE|
					RDW_ERASENOW|
					RDW_ALLCHILDREN);
			}

			if(!pBarSrc->_IsShowContentWhenDragging())
				pBarSrc->m_pDockSite->RecalcLayout();
		}

		return TRUE;

	case __ECDT_INNER_TOP			:
		{
			if(!pBarDst)
				return TRUE;

			if( bDstHorzDockBar ||
				bDstFloatBar ||
				_ds.IsDeepHalfSplit())
			{
				if(_ds.IsDeepHalfSplit())
				{
					pBarDst->_DraggingDoDeepHalfSplit(
						pBarSrc,
						AFX_IDW_DOCKBAR_TOP);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
				else
				{
					for( int i=0; i<nCountOfDstRowBars; i++)
					{
						CTControlBar *pBar = vBarsDstRow[i];

						if( pBar == pBarSrc ||
							pBar->IsFixedMode())
							continue;

						pBar->m_szDockedH.cy = rcTrack.Height();
					}

					pBarSrc->m_szDockedH.cx = rcTrack.Width();
					pBarSrc->m_szDockedH.cy = rcTrack.Height();

					((CTDockBar *) pDstDockBar)->_NewRowDockControlBar(
						pBarDst,
						pBarSrc,
						TRUE);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
			}
			else
			{
				pBarDst->m_szDockedV.cy = rcTrack.Height();
				pBarSrc->m_szDockedV.cy = rcTrack.Height();
				pBarSrc->m_szDockedV.cx = pBarDst->m_szDockedV.cx;

				((CTDockBar *) pDstDockBar)->DockControlBar( pBarSrc, &rcTrack);
				if(!bInitialMode)
				{
					pFrame->RecalcLayout();

					if( pOldFrame != pFrame &&
						::IsWindow(hWndOldFrame))
						__RECALC_FRAME_STEPS(pOldFrame);

					pBarSrc->SetWindowPos(
						NULL, 0,
						0, 0, 0,
						SWP_NOSIZE|
						SWP_NOMOVE|
						SWP_NOZORDER|
						SWP_NOACTIVATE|
						SWP_FRAMECHANGED);
				}
			}
		}

		return TRUE;

	case __ECDT_INNER_BOTTOM		:
		{
			if(!pBarDst)
				return TRUE;

			if( bDstHorzDockBar ||
				bDstFloatBar ||
				_ds.IsDeepHalfSplit())
			{
				if(_ds.IsDeepHalfSplit())
				{
					pBarDst->_DraggingDoDeepHalfSplit(
						pBarSrc,
						AFX_IDW_DOCKBAR_BOTTOM);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();
						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
				else
				{
					for( int i=0; i<nCountOfDstRowBars; i++)
					{
						CTControlBar *pBar = vBarsDstRow[i];

						if( pBar == pBarSrc ||
							pBar->IsFixedMode())
							continue;

						pBar->m_szDockedH.cy = rcTrack.Height();
					}

					pBarSrc->m_szDockedH.cx = rcTrack.Width();
					pBarSrc->m_szDockedH.cy = rcTrack.Height();
					((CTDockBar *) pDstDockBar)->_NewRowDockControlBar(
						pBarDst,
						pBarSrc,
						FALSE);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
			}
			else
			{
				pBarDst->m_szDockedV.cy = rcTrack.Height();
				pBarSrc->m_szDockedV.cy = rcTrack.Height();
				pBarSrc->m_szDockedV.cx = pBarDst->m_szDockedV.cx;

				((CTDockBar *) pDstDockBar)->DockControlBar( pBarSrc, &rcTrack);
				if(!bInitialMode)
				{
					pFrame->RecalcLayout();

					if( pOldFrame != pFrame &&
						::IsWindow(hWndOldFrame))
						__RECALC_FRAME_STEPS(pOldFrame);

					pBarSrc->SetWindowPos(
						NULL, 0,
						0, 0, 0,
						SWP_NOSIZE|
						SWP_NOMOVE|
						SWP_NOZORDER|
						SWP_NOACTIVATE|
						SWP_FRAMECHANGED);
				}
			}
		}

		return TRUE;

	case __ECDT_INNER_LEFT			:
		{
			if(!pBarDst)
				return TRUE;

			if( !bDstHorzDockBar ||
				bDstFloatBar ||
				_ds.IsDeepHalfSplit())
			{
				if(_ds.IsDeepHalfSplit())
				{
					pBarDst->_DraggingDoDeepHalfSplit(
						pBarSrc,
						AFX_IDW_DOCKBAR_LEFT);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
				else
				{
					for( int i=0; i<nCountOfDstRowBars; i++)
					{
						CTControlBar *pBar = vBarsDstRow[i];

						if( pBar == pBarSrc ||
							pBar->IsFixedMode())
							continue;

						pBar->m_szDockedV.cx = rcTrack.Width();
					}

					pBarSrc->m_szDockedV.cx = rcTrack.Width();
					pBarSrc->m_szDockedV.cy = rcTrack.Height();
					((CTDockBar *) pDstDockBar)->_NewRowDockControlBar(
						pBarDst,
						pBarSrc,
						TRUE);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
			}
			else
			{
				pBarDst->m_szDockedH.cx = rcTrack.Width();
				pBarSrc->m_szDockedH.cx = rcTrack.Width();
				pBarSrc->m_szDockedH.cy = pBarDst->m_szDockedH.cy;
				((CTDockBar *) pDstDockBar)->DockControlBar( pBarSrc, &rcTrack);

				if(!bInitialMode)
				{
					pFrame->RecalcLayout();

					if( pOldFrame != pFrame &&
						::IsWindow(hWndOldFrame))
						__RECALC_FRAME_STEPS(pOldFrame);

					pBarSrc->SetWindowPos(
						NULL, 0,
						0, 0, 0,
						SWP_NOSIZE|
						SWP_NOMOVE|
						SWP_NOZORDER|
						SWP_NOACTIVATE|
						SWP_FRAMECHANGED);
				}
			}
		}

		return TRUE;

	case __ECDT_INNER_RIGHT			:
		{
			if(!pBarDst)
				return TRUE;

			if( !bDstHorzDockBar ||
				bDstFloatBar ||
				_ds.IsDeepHalfSplit())
			{
				if(_ds.IsDeepHalfSplit())
				{
					pBarDst->_DraggingDoDeepHalfSplit(
						pBarSrc,
						AFX_IDW_DOCKBAR_RIGHT);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
				else
				{
					for( int i=0; i<nCountOfDstRowBars; i++)
					{
						CTControlBar *pBar = vBarsDstRow[i];

						if( pBar == pBarSrc ||
							pBar->IsFixedMode())
							continue;

						pBar->m_szDockedV.cx = rcTrack.Width();
					}

					pBarSrc->m_szDockedV.cx = rcTrack.Width();
					pBarSrc->m_szDockedV.cy = rcTrack.Height();
					((CTDockBar *) pDstDockBar)->_NewRowDockControlBar(
						pBarDst,
						pBarSrc,
						FALSE);

					if(!bInitialMode)
					{
						pFrame->RecalcLayout();

						if( pOldFrame != pFrame &&
							::IsWindow(hWndOldFrame))
							__RECALC_FRAME_STEPS(pOldFrame);
					}
				}
			}
			else
			{
				pBarDst->m_szDockedH.cx = rcTrack.Width();
				pBarSrc->m_szDockedH.cx = rcTrack.Width();
				pBarSrc->m_szDockedH.cy = pBarDst->m_szDockedH.cy;
				((CTDockBar *) pDstDockBar)->DockControlBar(
					pBarSrc,
					&rcTrack);

				if(!bInitialMode)
				{
					pFrame->RecalcLayout();

					if( pOldFrame != pFrame &&
						::IsWindow(hWndOldFrame))
						__RECALC_FRAME_STEPS(pOldFrame);

					pBarSrc->SetWindowPos(
						NULL, 0,
						0, 0, 0,
						SWP_NOSIZE|
						SWP_NOMOVE|
						SWP_NOZORDER|
						SWP_NOACTIVATE|
						SWP_FRAMECHANGED);
				}
			}
		}

		return TRUE;

	case __ECDT_DOCKBAR_INNER_TOP			:
	case __ECDT_DOCKBAR_INNER_BOTTOM		:
	case __ECDT_DOCKBAR_INNER_LEFT			:
	case __ECDT_DOCKBAR_INNER_RIGHT			:
		{
			if( nCDT == __ECDT_DOCKBAR_INNER_TOP ||
				nCDT == __ECDT_DOCKBAR_INNER_BOTTOM)
			{
				pBarSrc->m_szDockedH.cy = rcTrack.Height();
				pBarSrc->m_szDockedH.cy = rcTrack.Height();
			}
			else
			{
				pBarSrc->m_szDockedV.cx = rcTrack.Width();
				pBarSrc->m_szDockedV.cx = rcTrack.Width();
			}

			UINT nDockBarID = AFX_IDW_DOCKBAR_TOP;

			if( nCDT == __ECDT_DOCKBAR_INNER_BOTTOM )
				nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
			else if( nCDT == __ECDT_DOCKBAR_INNER_LEFT )
				nDockBarID = AFX_IDW_DOCKBAR_LEFT;
			else if( nCDT == __ECDT_DOCKBAR_INNER_RIGHT )
				nDockBarID = AFX_IDW_DOCKBAR_RIGHT;

			CTDockBar *pDockBar = (CTDockBar *) pFrame->GetControlBar(nDockBarID);
			UINT nCircleNoRequired = _ds.GetCircleNo();

			if( nCircleNoRequired > 0 )
				pDockBar = pDockBar->_GetBarByCircleNo(nCircleNoRequired);
			HWND hWndSrc = pBarSrc->GetSafeHwnd();

			pDockBar->_InnerOuterDockControlBar( pBarSrc, TRUE);
			if(!bInitialMode)
			{
				pFrame->RecalcLayout();

				if( pOldFrame != pFrame &&
					::IsWindow(hWndOldFrame))
					__RECALC_FRAME_STEPS(pOldFrame);
			}

			if(::IsWindow(hWndSrc))
			{
				pBarSrc->SetWindowPos(
					NULL, 0,
					0, 0, 0,
					SWP_NOSIZE|
					SWP_NOMOVE|
					SWP_NOZORDER|
					SWP_NOACTIVATE|
					SWP_FRAMECHANGED);
			}
		}

		return TRUE;

	case __ECDT_CIRCLE1_OUTER_TOP			:
	case __ECDT_CIRCLE1_OUTER_BOTTOM		:
	case __ECDT_CIRCLE1_OUTER_LEFT			:
	case __ECDT_CIRCLE1_OUTER_RIGHT			:
		{
			BOOL bInjectCircle = FALSE;
			UINT nDockBarID = 0;

			switch(nCDT)
			{
			case __ECDT_CIRCLE1_OUTER_TOP		: nDockBarID = AFX_IDW_DOCKBAR_TOP; break;
			case __ECDT_CIRCLE1_OUTER_BOTTOM	: nDockBarID = AFX_IDW_DOCKBAR_BOTTOM; break;
			case __ECDT_CIRCLE1_OUTER_LEFT		: nDockBarID = AFX_IDW_DOCKBAR_LEFT; bInjectCircle = TRUE; break;
			case __ECDT_CIRCLE1_OUTER_RIGHT		: nDockBarID = AFX_IDW_DOCKBAR_RIGHT; bInjectCircle = TRUE; break;
			}

			CTDockBar *pDockBar = (CTDockBar *) pFrame->GetControlBar(nDockBarID);
			if(bInjectCircle)
				pDockBar->_InjectInnerCircle();

			pDockBar = pDockBar->_GetBarByCircleNo(1);
			HWND hWndSrc = pBarSrc->GetSafeHwnd();

			pDockBar->_InnerOuterDockControlBar( pBarSrc, FALSE);
			if(!bInitialMode)
			{
				pFrame->RecalcLayout();

				if( pOldFrame != pFrame &&
					::IsWindow(hWndOldFrame))
					__RECALC_FRAME_STEPS(pOldFrame);
			}

			if(::IsWindow(hWndSrc))
			{
				pBarSrc->SetWindowPos(
					NULL, 0,
					0, 0, 0,
					SWP_NOSIZE|
					SWP_NOMOVE|
					SWP_NOZORDER|
					SWP_NOACTIVATE|
					SWP_FRAMECHANGED);
			}
		}

		return TRUE;
	}

	return FALSE;
}

UINT CTControlBar::GetDockBarCircleNo() const
{
	return ((CTDockBar *) m_pDockBar)->_GetCircleNo();
}

BOOL CTControlBar::_CanDockToInnerCircles() const
{
	return TRUE;
}

BOOL CTControlBar::_CanDockToTabbedContainers() const
{
	return FALSE;
}

void CTControlBar::ToggleDocking()
{
	UINT nDockBarID = GetSafeDockBarDlgCtrlID();

	if( nDockBarID != AFX_IDW_DOCKBAR_FLOAT &&
		!GetParentFrame()->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		FloatControlBar();
}

CTMiniDockFrameWnd *CTControlBar::OnCreateFloatingFrame( DWORD dwStyle)
{
	CTMiniDockFrameWnd *pMiniFrame = new CTMiniDockFrameWnd;

	if(!pMiniFrame->Create( m_pDockSite, dwStyle))
		AfxThrowResourceException();

	return pMiniFrame;
}

void CTControlBar::FloatControlBar( DWORD dwStyle)
{
	FloatControlBar( m_ptFloatHelper, dwStyle);
}

void CTControlBar::FloatControlBar( CPoint ptFloat, DWORD dwStyle)
{
	if(m_pDockBar)
	{
		CDockBar *pDockBar = m_pDockBar;

		if( pDockBar->m_bFloating &&
			pDockBar->GetDockedCount() == 1 )
		{
			CTMiniDockFrameWnd *pDockFrame = (CTMiniDockFrameWnd *) pDockBar->GetParent();

			pDockFrame->SetWindowPos(
				NULL,
				ptFloat.x,
				ptFloat.y,
				0, 0,
				SWP_NOSIZE|
				SWP_NOZORDER|
				SWP_NOACTIVATE);
			pDockFrame->RecalcLayout(TRUE);
			pDockFrame->UpdateWindow();

			m_ptFloatHelper = ptFloat;
			return;
		}
	}

	BOOL bShowMiniFrame = GetStyle()&WS_VISIBLE;
	if(m_dwStyle&CBRS_SIZE_DYNAMIC)
	{
		dwStyle |= CBRS_SIZE_DYNAMIC;

		if(dwStyle&CBRS_ORIENT_VERT)
		{
			dwStyle &= ~CBRS_ALIGN_ANY;
			dwStyle |= CBRS_ALIGN_TOP;
		}
	}

	CTDockBar *pDockBarOut = (CTDockBar *) m_pDockBar;
	BOOL bOptimizeDockBarLayout = FALSE;

	if( pDockBarOut && !pDockBarOut->m_bFloating &&
		pDockBarOut->IsKindOf(RUNTIME_CLASS(CTDockBar)))
		bOptimizeDockBarLayout = TRUE;

	HWND hWndDockBarOuter = pDockBarOut ? pDockBarOut->GetSafeHwnd() : NULL;
	HWND hWndOldFrame = NULL;

	CFrameWnd *pOldFrame = GetParentFrame();
	if(pOldFrame)
		hWndOldFrame = pOldFrame->GetSafeHwnd();

	CTMiniDockFrameWnd *pDockFrame = OnCreateFloatingFrame(dwStyle);
	pDockFrame->SetWindowPos(
		NULL,
		ptFloat.x,
		ptFloat.y,
		0, 0,
		SWP_NOSIZE|
		SWP_NOZORDER|
		SWP_NOACTIVATE);

	if(!pDockFrame->m_hWndOwner)
		pDockFrame->m_hWndOwner = m_pDockSite->m_hWnd;

	CDockBar *pDockBar = (CDockBar *) pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	m_bUpdatingChain = TRUE;
	((CTDockBar *) pDockBar)->DockControlBar(this);
	m_bUpdatingChain = FALSE;
	pDockFrame->RecalcLayout(TRUE);

	if(bShowMiniFrame)
	{
		pDockFrame->ShowWindow(SW_SHOWNA);
		pDockFrame->UpdateWindow();
	}
	m_ptFloatHelper = ptFloat;

	if(	bOptimizeDockBarLayout &&
		::IsWindow(hWndDockBarOuter))
		((CTDockBar *) pDockBarOut)->OnDynamicLayoutOptimize();

	if( pOldFrame != m_pDockSite &&
		::IsWindow(hWndOldFrame))
		__RECALC_FRAME_STEPS(pOldFrame);

	CTDockBar::_RemovePlaceHolder( m_pDockSite, this);
	CTDockBar::_OptimizeCircles(m_pDockSite);

	m_pDockSite->RecalcLayout();
	OnRepositionSingleChild();
}

BOOL CTControlBar::DockControlBar( UINT nDockBarID,
								   UINT nCircleNo,
								   CFrameWnd *pDockSite,
								   BOOL bRecalcLayout)
{
	if(!pDockSite)
		pDockSite = m_pDockSite;

	CTDockBar *pDockBar = STATIC_DOWNCAST(
		CTDockBar,
		pDockSite->GetControlBar(nDockBarID));

	if(!nCircleNo)
	{
		pDockBar->_InnerOuterDockControlBar(
			this,
			TRUE);

		if(bRecalcLayout)
			pDockSite->RecalcLayout();

		return TRUE;
	}

	pDockBar = pDockBar->_GetBarByCircleNo(nCircleNo);
	pDockBar->_InnerOuterDockControlBar(
		this,
		TRUE);

	if(bRecalcLayout)
		pDockSite->RecalcLayout();

	return TRUE;
}

BOOL CTControlBar::DockControlBar( CTControlBar *pBar,
								   BOOL bHorzBarHalfSplit,
								   BOOL bInnerHalfSplit,
								   CFrameWnd *pDockSite,
								   BOOL bRecalcLayout)
{
	if(!pDockSite)
		pDockSite = m_pDockSite;

	DraggingState _ds;
	CRect rc;

	GetWindowRect(&rc);
	_ds.BarSrcSet(pBar);
	_ds.BarDstSet(this);
	_ds.SetEmpty(FALSE);
	_ds.TrackRectSet(rc);

	UINT nOwnDockBarID = GetSafeDockBarDlgCtrlID();
	switch(nOwnDockBarID)
	{
	case AFX_IDW_DOCKBAR_FLOAT		:
		{
			_ds.SetCDT( bHorzBarHalfSplit ? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM) : (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT));
			_ds.SetDeepHalfSplit(TRUE);
		}

		break;

	case AFX_IDW_DOCKBAR_LEFT		:
		{
			_ds.SetCDT( bHorzBarHalfSplit ? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM) : (bInnerHalfSplit ? __ECDT_INNER_RIGHT : __ECDT_INNER_LEFT));
			_ds.SetDeepHalfSplit(!bHorzBarHalfSplit);
		}

		break;

	case AFX_IDW_DOCKBAR_RIGHT		:
		{
			_ds.SetCDT( bHorzBarHalfSplit ? (bInnerHalfSplit ? __ECDT_INNER_TOP : __ECDT_INNER_BOTTOM) : (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT));
			_ds.SetDeepHalfSplit(!bHorzBarHalfSplit);
		}

		break;

	case AFX_IDW_DOCKBAR_TOP		:
		{
			_ds.SetCDT( bHorzBarHalfSplit ? (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT) : (bInnerHalfSplit ? __ECDT_INNER_BOTTOM : __ECDT_INNER_TOP));
			_ds.SetDeepHalfSplit(bHorzBarHalfSplit);
		}

		break;

	case AFX_IDW_DOCKBAR_BOTTOM		:
		{
			_ds.SetCDT( bHorzBarHalfSplit ? (bInnerHalfSplit ? __ECDT_INNER_LEFT : __ECDT_INNER_RIGHT) : (bInnerHalfSplit ? __ECDT_INNER_TOP: __ECDT_INNER_BOTTOM));
			_ds.SetDeepHalfSplit(bHorzBarHalfSplit);
		}

		break;
	}

	BOOL bRetVal = _DraggingApplyState(_ds);
	if(bRecalcLayout)
		pDockSite->RecalcLayout();
	else
		pDockSite->DelayRecalcLayout();

	return bRetVal;
}

void CTControlBar::OnShowWindow( BOOL bShow, UINT nStatus)
{
	CControlBar::OnShowWindow( bShow, nStatus);
	if( nStatus != 0 )
		return;

	_UpdateVisibilityInChain();
}

void CTControlBar::_UpdateVisibilityInChain()
{
	if( m_pDockBar == NULL && !m_bPresubclassDialogMode )
		return;

	if( _DraggingGetBar() ||
		IsFixedMode() ||
		!m_pDockSite ||
		!m_pDockBar ||
		m_bUpdatingChain )
		return;

	m_bUpdatingChain = TRUE;
	_UpdateVisibilityInRow();

	BOOL bVisible = IsVisible();
	CTDynamicDockBar *pDynDocker = (CTDynamicDockBar *) m_pDockBar;

	if( pDynDocker != NULL &&
		pDynDocker->IsKindOf(RUNTIME_CLASS(CTDynamicDockBar)))
	{
		CTDynamicControlBar *pDynBar = STATIC_DOWNCAST(
			CTDynamicControlBar,
			pDynDocker->GetParent());

		if(bVisible)
			m_pDockSite->ShowControlBar( pDynBar, TRUE, TRUE);
		else
		{
			int nDockedVisibleCount = pDynDocker->GetDockedVisibleCount();
			if( nDockedVisibleCount == 0 )
				m_pDockSite->ShowControlBar( pDynBar, FALSE, TRUE);
		}
	}

	if(!bVisible)
	{
		if(IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
		{
			CTDynamicDockBar *pInnerDynDocker = ((CTDynamicControlBar *) this)->m_pWndDynDocker;

			if(pInnerDynDocker)
			{
				for( int i=0; i<pInnerDynDocker->m_arrBars.GetSize(); i++)
				{
					CControlBar *pBar = pInnerDynDocker->GetDockedControlBar(i);

					if(!pBar)
						continue;

					pBar->DelayShow(FALSE);
					pBar->SetWindowPos(
						NULL, 0,
						0, 0, 0,
						SWP_NOZORDER|
						SWP_NOMOVE|
						SWP_NOSIZE|
						SWP_NOACTIVATE|
						SWP_HIDEWINDOW|
						SWP_NOSENDCHANGING);
				}
			}
		}
	}

	CFrameWnd *pParentFrame = GetParentFrame();
	if(pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		pParentFrame->RecalcLayout();

	m_bUpdatingChain = FALSE;
}

void CTControlBar::_UpdateVisibilityInRow()
{
	if( !m_pDockBar && !m_bPresubclassDialogMode )
		return;

	if( IsFixedMode() ||
		!m_pDockSite ||
		!m_pDockBar ||
		IsKindOf(RUNTIME_CLASS(CTDynamicControlBar)))
		return;

	BOOL bVisible = IsVisible();
	UINT nDockBarID = GetSafeDockBarDlgCtrlID();

	if( nDockBarID == AFX_IDW_DOCKBAR_FLOAT )
		return;

	BOOL bHorz = nDockBarID == AFX_IDW_DOCKBAR_TOP || nDockBarID == AFX_IDW_DOCKBAR_BOTTOM;
	if(!bVisible)
	{
		m_nDelayedRowUpdateMetric = bHorz ? m_szDockedH.cx : m_szDockedV.cy;
		return;
	}

	if( m_nDelayedRowUpdateMetric == 0 )
		return;

	LONG nDelayedRowUpdateMetric = m_nDelayedRowUpdateMetric;
	m_nDelayedRowUpdateMetric = 0;

	ControlBarVector vRow;
	_GetRowBars(vRow);

	int nCountInRow = vRow.GetSize();
	if( nCountInRow == 1 )
		return;

	for( int i=0; i<nCountInRow;)
	{
		CTControlBar *pBar = vRow[i];
		BOOL bBarIsVisible = pBar->IsVisible();
		BOOL bFixedBar = pBar->IsFixedMode();

		if( pBar == this ||
			!bBarIsVisible ||
			bFixedBar)
		{
			nCountInRow--;
			vRow.RemoveAt(i);

			continue;
		}

		LONG nRealBarExtent = bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;
		LONG nMinBarExtent = bHorz ? pBar->_CalcDesiredMinHW() : pBar->_CalcDesiredMinVH();

		if( nRealBarExtent <= nMinBarExtent )
		{
			nCountInRow--;
			vRow.RemoveAt(i);

			continue;
		}

		i++;
	}

	if( nCountInRow == 0 )
		return;

	LONG nExtentDesizred = nDelayedRowUpdateMetric;
	while( nExtentDesizred>0 && nCountInRow > 0 )
	{
		for( i=0; i<nCountInRow;)
		{
			CTControlBar *pBar = vRow[i];
			LONG &nRealBarExtentRef = bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;
			LONG nMinBarExtent = bHorz ? pBar->_CalcDesiredMinHW() : pBar->_CalcDesiredMinVH();

			nRealBarExtentRef--;
			nExtentDesizred--;

			if( nExtentDesizred == 0 )
				break;

			if( nRealBarExtentRef == nMinBarExtent )
			{
				nCountInRow--;
				vRow.RemoveAt(i);

				continue;
			}

			i++;
		}
	}
}

BOOL CTControlBar::RegisterControlBarClass()
{
	if(m_bControlBarClassRegistered)
		return TRUE;

	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!::GetClassInfo(
		hInst,
		__TCONTROLBAR_CLASS_NAME,
		&wndclass))
	{
		wndclass.style = CS_DBLCLKS;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = ::LoadCursor(
			NULL,
			IDC_ARROW);
		wndclass.hbrBackground = NULL;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = __TCONTROLBAR_CLASS_NAME;

		if(!::AfxRegisterClass(&wndclass))
			return FALSE;
	}

	m_bControlBarClassRegistered = TRUE;
	return TRUE;
}

BOOL CTControlBar::PreCreateWindow( CREATESTRUCT& cs)
{
	if( !RegisterControlBarClass() || !CControlBar::PreCreateWindow(cs) )
		return FALSE;
	cs.lpszClass = __TCONTROLBAR_CLASS_NAME;

	return TRUE;
}

void CTControlBar::PreSubclassWindow()
{
	CControlBar::PreSubclassWindow();
	m_bInCustomModeUpdateCmdUI = FALSE;
	m_bPresubclassDialogMode = FALSE;

	HWND hWndParent = ::GetParent(m_hWnd);
	if(hWndParent)
	{
		CWnd *pWndParent = CWnd::FromHandlePermanent(hWndParent);

		if( pWndParent &&
			!pWndParent->IsKindOf(RUNTIME_CLASS(CControlBar)) &&
			!pWndParent->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
		{
			DWORD dwWndStyle = ::GetWindowLong( m_hWnd, GWL_STYLE);

			m_dwStyle = dwWndStyle&CBRS_ALL;
			m_bPresubclassDialogMode = TRUE;
		}
	}

	if(m_dwStyle&CBRS_TOOLTIPS)
		EnableToolTips();
}

BOOL CTControlBar::PreTranslateMessage( MSG *pMsg)
{
	if(CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	CWnd *pOwner = GetOwner();
#if _MFC_VER < 0x700
	UINT message = pMsg->message;

	if(((m_dwStyle&CBRS_FLYBY) ||
		message == WM_LBUTTONDOWN ||
		message == WM_LBUTTONUP ) && ((
		message >= WM_MOUSEFIRST &&
		message <= WM_MOUSELAST ) || (
		message >= WM_NCMOUSEFIRST &&
		message <= WM_NCMOUSELAST )) && ((
		!CTToolControlBar::m_bMenuTracking ||
		CTPopupMenuWnd::IsMenuTracking())))
	{
		_AFX_THREAD_STATE *pThreadState = ::AfxGetThreadState();
		CPoint point = pMsg->pt;
		ScreenToClient(&point);

		TOOLINFO ti;
		memset( &ti, 0, sizeof(TOOLINFO));
		ti.cbSize = sizeof(AFX_OLDTOOLINFO);

		int nHit = OnToolHitTest( point, &ti);
		if( ti.lpszText != LPSTR_TEXTCALLBACK )
			free(ti.lpszText);

		BOOL bNotButton = message == WM_LBUTTONDOWN && (ti.uFlags&TTF_NOTBUTTON);
		if( message != WM_LBUTTONDOWN &&
			GetKeyState(VK_LBUTTON) < 0 )
			nHit = pThreadState->m_nLastStatus;

		if( nHit < 0 || bNotButton )
		{
			if( GetKeyState(VK_LBUTTON) >= 0 || bNotButton )
			{
				SetStatusText(-1);
				KillTimer(ID_TIMER_CHECK);
			}
		}
		else if( message == WM_LBUTTONUP )
		{
			SetStatusText(-1);
			ResetTimer(ID_TIMER_CHECK, 200);
		}
		else if( (m_nStateFlags&statusSet) || GetKeyState(VK_LBUTTON) < 0 )
			SetStatusText(nHit);
		else if( nHit != pThreadState->m_nLastStatus )
			ResetTimer( ID_TIMER_WAIT, 300);

		pThreadState->m_nLastStatus = nHit;
	}
#endif

	CFrameWnd *pFrameWnd = GetTopLevelFrame();
	if( pFrameWnd && pFrameWnd->m_bHelpMode )
		return FALSE;

	if(!m_bPresubclassDialogMode)
	{
		while(pOwner)
		{
			HWND hWndOwner = pOwner->GetSafeHwnd();

			if( !::IsWindow(hWndOwner) ||
				!CWnd::FromHandlePermanent(hWndOwner))
				break;

			if(pOwner->PreTranslateMessage(pMsg))
				return TRUE;

			pOwner = pOwner->GetParentFrame();
		}
	}

	return PreTranslateInput(pMsg);
}

int CTControlBar::OnToolHitTest( CPoint point, TOOLINFO * pTI) const
{
	if( CTToolControlBar::m_bMenuTracking ||
		CTPopupMenuWnd::IsMenuTracking())
		return -1;

	CPoint ptScreen(point);
	ClientToScreen(&ptScreen);

	CTBarNcAreaButton *pBtn = NULL;
	(const_cast <CTControlBar *> (this))->NcButtons_HitTest( ptScreen, &pBtn);

	if(pBtn)
		return pBtn->OnToolHitTest( point, pTI);

	return CControlBar::OnToolHitTest( point, pTI);
}

void CTControlBar::_ActivateOnClick()
{
	HWND hWndOwn = GetSafeHwnd();

	if( hWndOwn == NULL || !::IsWindow(hWndOwn) )
		return;

	HWND hWndActivate = NULL;
	if(m_bPresubclassDialogMode)
		hWndActivate = ::GetParent(hWndOwn);
	else
		hWndActivate = _GetDockingFrameImpl()->GetSafeHwnd();

	if(hWndActivate)
	{
		HWND hWndFocus = ::GetFocus();

		if( !hWndFocus ||
			hWndActivate != hWndFocus ||
			!::IsChild( hWndActivate, hWndFocus))
			::SetFocus(hWndActivate);
	}
	else
		::SetFocus(hWndOwn);
}

void CTControlBar::RepositionBarsEx( CWnd *pWndToReposChilds,
									 UINT nIDFirst,
									 UINT nIDLast,
									 UINT nIDLeftOver,
									 UINT nFlag,
									 LPRECT lpRectParam,
									 LPCRECT lpRectClient,
									 BOOL bStretch)
{
	AFX_SIZEPARENTPARAMS layout;
	HWND hWndLeftOver = NULL;

	layout.bStretch = bStretch;
	layout.sizeTotal.cx = 0;
	layout.sizeTotal.cy = 0;

	if(lpRectClient)
		layout.rect = *lpRectClient;
	else
		pWndToReposChilds->GetClientRect(&layout.rect);

	if( nFlag != 1 )
		layout.hDWP = ::BeginDeferWindowPos(8);
	else
		layout.hDWP = NULL;

	for( HWND hWndChild = ::GetTopWindow(pWndToReposChilds->m_hWnd);
		hWndChild;
		hWndChild = ::GetWindow( hWndChild, GW_HWNDNEXT))
	{
		CWnd *pWnd = CWnd::FromHandlePermanent(hWndChild);
		UINT nIDC = _AfxGetDlgCtrlID(hWndChild);

		if( nIDC == nIDLeftOver )
			hWndLeftOver = hWndChild;
		else if( pWnd && nIDC >= nIDFirst && nIDC <= nIDLast )
		{
			::SendMessage(
				hWndChild,
				WM_SIZEPARENT,
				0, (LPARAM) &layout);
		}
	}

	if( nFlag == 1 )
	{
		if(bStretch)
			::CopyRect( lpRectParam, &layout.rect);
		else
		{
			lpRectParam->left = lpRectParam->top = 0;
			lpRectParam->right = layout.sizeTotal.cx;
			lpRectParam->bottom = layout.sizeTotal.cy;
		}

		return;
	}

	if( nIDLeftOver && hWndLeftOver )
	{
		CWnd *pLeftOver = CWnd::FromHandle(hWndLeftOver);

		if( nFlag == 2 )
		{
			layout.rect.left += lpRectParam->left;
			layout.rect.top += lpRectParam->top;
			layout.rect.right -= lpRectParam->right;
			layout.rect.bottom -= lpRectParam->bottom;
		}

		pLeftOver->CalcWindowRect(&layout.rect);
		AfxRepositionWindow( &layout, hWndLeftOver, &layout.rect);
	}

	::EndDeferWindowPos(layout.hDWP);
}

void CTControlBar::_PreSyncRowMetrics()
{
	if( IsVisible() || IsFloating() )
		return;

	ControlBarVector vRow;
	_GetRowBars(vRow);

	int nCount = vRow.GetSize();
	for( int i=0; i<nCount;)
	{
		CTControlBar *pBar = vRow[i];
		if( !pBar->IsVisible() ||
			pBar->IsFixedMode() ||
			pBar == this )
		{
			vRow.RemoveAt(i);
			nCount--;
		}
		else
			i++;
	}

	UINT nDockbarID = GetSafeDockBarDlgCtrlID();
	BOOL bHorz = nDockbarID == AFX_IDW_DOCKBAR_TOP || nDockbarID == AFX_IDW_DOCKBAR_BOTTOM;
	LONG nOwnMetric = bHorz ? m_szDockedH.cx : m_szDockedV.cy;

	if(!nOwnMetric)
	{
		nOwnMetric = bHorz ? _CalcDesiredMinHW() : _CalcDesiredMinVH();
		bHorz ? m_szDockedH.cx : m_szDockedV.cy= nOwnMetric;
	}

	LONG &nCrossMetricOwnRef = bHorz ? m_szDockedH.cy : m_szDockedV.cx;
	LONG nCrossMetricOther = -1;

	for( i=0; i<nCount && nOwnMetric>0;)
	{
		CTControlBar *pBar = vRow[i];
		LONG &nRefBarMetric = bHorz ? pBar->m_szDockedH.cx : pBar->m_szDockedV.cy;
		LONG nBarMinMetric = bHorz ? pBar->_CalcDesiredMinHW() : pBar->_CalcDesiredMinVH();

		if( nCrossMetricOther <= 0 )
			nCrossMetricOther = bHorz ? pBar->m_szDockedH.cy : pBar->m_szDockedV.cx;

		if( nRefBarMetric <= nBarMinMetric )
		{
			vRow.RemoveAt(i);
			nCount--;

			if( nCount == 0 )
				break;

			continue;
		}

		nRefBarMetric--;
		nOwnMetric--;

		if(!nOwnMetric)
			break;

		i++;
		if( i == nCount )
			i = 0;
	}

	if( nCrossMetricOther > 0 )
		nCrossMetricOwnRef = nCrossMetricOther;
}

void CTControlBar::OnFrameBarCheckCmd( BOOL bResizableCheckmarks)
{
	if( bResizableCheckmarks || IsFixedMode() )
	{
		m_pDockSite->ShowControlBar(
			this,
			!(GetStyle()&WS_VISIBLE),
			FALSE);

		return;
	}

	if(!(GetStyle()&WS_VISIBLE))
	{
		m_pDockSite->ShowControlBar( this, TRUE, FALSE);
		CTMiniDockFrameWnd *pMiniFrame = DYNAMIC_DOWNCAST(
			CTMiniDockFrameWnd,
			GetParentFrame());

		if(pMiniFrame)
		{
			_UpdateVisibilityInChain();
			pMiniFrame->RecalcLayout();
		}
	}

	CFrameWnd *pParentFrame = GetParentFrame();
	if(pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		pParentFrame->BringWindowToTop();

	SetFocus();
}

void CTControlBar::OnFrameBarCheckUpdate( CCmdUI *pCmdUI,
										  BOOL bResizableCheckmarks)
{
	pCmdUI->Enable();

	if( bResizableCheckmarks || IsFixedMode() )
	{
		pCmdUI->SetCheck(GetStyle()&WS_VISIBLE);
		return;
	}
}

BOOL CTControlBar::DoFrameBarCheckCmd( CFrameWnd *pFrame,
									   UINT nBarID,
									   BOOL bResizableCheckmarks)
{
	CControlBar *pBar = pFrame->GetControlBar(nBarID);

	if(pBar)
	{
		if(pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			((CTControlBar *) pBar)->OnFrameBarCheckCmd(bResizableCheckmarks);
		else
			pFrame->ShowControlBar( pBar, !(pBar->GetStyle()&WS_VISIBLE), FALSE);

		return TRUE;
	}

	return FALSE;
}

void CTControlBar::DoFrameBarCheckUpdate( CFrameWnd *pFrame,
										  CCmdUI *pCmdUI,
										  BOOL bResizableCheckmarks)
{
	CControlBar *pBar = pFrame->GetControlBar(pCmdUI->m_nID);

	if(pBar)
	{
		if(pBar->IsKindOf(RUNTIME_CLASS(CTControlBar)))
			((CTControlBar*)pBar)->OnFrameBarCheckUpdate( pCmdUI, bResizableCheckmarks);
		else
			pCmdUI->SetCheck(pBar->GetStyle()&WS_VISIBLE);

		return;
	}

	pCmdUI->ContinueRouting();
}

void CTControlBar::OnSetFocus( CWnd *pOldWnd)
{
	CControlBar::OnSetFocus(pOldWnd);

	if(IsFixedMode())
		return;

	HWND hWndChild = ::GetWindow( GetSafeHwnd(), GW_CHILD);
	if(hWndChild)
		::SetFocus(hWndChild);
}

BOOL CTControlBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
