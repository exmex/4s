#include "stdafx.h"
#include "TTabControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_TAB_CONTROL					5007


/////////////////////////////////////////////////////////////////////////////
// CTTabControlBar

CTTabControlBar::CTTabControlBar()
{
	m_bToolTipsEnabled = TRUE;
	m_nNextToolTipId = 0;
	m_pActiveView = NULL;
	m_nActiveTab = -1;
}

CTTabControlBar::~CTTabControlBar()
{
	while(!m_viewList.IsEmpty())
	{
		TCB_ITEM *pMember = m_viewList.RemoveHead();

		if(pMember)
			delete pMember;
	}
}


IMPLEMENT_DYNAMIC( CTTabControlBar, CTControlBar)

BEGIN_MESSAGE_MAP( CTTabControlBar, CTControlBar)
	//{{AFX_MSG_MAP(CTTabControlBar)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY( TCN_SELCHANGE, IDC_TAB_CONTROL, OnTabSelChange)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTabControlBar message handlers

void CTTabControlBar::OnWindowPosChanged( WINDOWPOS FAR* lpwndpos)
{
	CTControlBar::OnWindowPosChanged(lpwndpos);
	CFrameWnd *pFrame = GetParentFrame();
	CRect rect;

	GetClientRect(&rect);
	if(IsFloating())
		rect.DeflateRect( 3, 3);
	else
		rect.DeflateRect( 0, 1, 0, 0);

	m_TabCtrl.MoveWindow(&rect);
	m_TabCtrl.SetWindowPos(
		NULL,
		0, 0,
		0, 0,
		SWP_NOSIZE|
		SWP_NOMOVE);

	POSITION pos = m_viewList.GetHeadPosition();
	while(pos)
	{
		TCB_ITEM *pItem = (TCB_ITEM *) m_viewList.GetNext(pos);

		if( pFrame->GetActiveView() == pItem->m_pWnd )
			pFrame->SetActiveView(NULL);

		SizeTabView(pItem->m_pWnd);
	}
}

int CTTabControlBar::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if( CTControlBar::OnCreate(lpCreateStruct) == -1 )
		return -1;

	if(!m_TabCtrl.Create( this, IDC_TAB_CONTROL))
		return -1;

	return 0;
}

BOOL CTTabControlBar::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN		:
			{
				m_pActiveView->SendMessage(
					pMsg->message,
					pMsg->wParam,
					pMsg->lParam);
			}

			return TRUE;
		}
	}

	return CTControlBar::PreTranslateMessage(pMsg);
}

void CTTabControlBar::OnTabSelChange( NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	SetActiveView(m_TabCtrl.GetCurSel());
}

BOOL CTTabControlBar::CreateTabView( CRuntimeClass *pViewClass, CDocument *pDocument, CCreateContext *pContext)
{
	CWnd* pWnd = NULL;

	if( pContext == NULL )
	{
		CCreateContext context;

		context.m_pNewViewClass = pViewClass;
		context.m_pCurrentDoc = pDocument;
		context.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
		context.m_pLastView = GetParentFrame()->GetActiveView();
		context.m_pCurrentFrame = GetParentFrame();
		pContext = &context;
	}

	TRY
	{
		pWnd = DYNAMIC_DOWNCAST(
			CWnd,
			pViewClass->CreateObject());

		if(!pWnd)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		return FALSE;
	}
	END_CATCH_ALL

	if(!pWnd->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 0, 0), this, 0, pContext))
		return FALSE;

	m_pActiveView = DYNAMIC_DOWNCAST( CView, pWnd);
	if(m_pActiveView)
		m_pActiveView->OnInitialUpdate();

	return TRUE;
}

TCB_ITEM *CTTabControlBar::CreateTabListItem( LPCTSTR lpszLabel)
{
	TCB_ITEM *pMember = new TCB_ITEM;

	pMember->m_pWnd = m_pActiveView;
	pMember->m_strTabLabel = lpszLabel;
	pMember->m_strToolTipLabel = lpszLabel;
	pMember->m_nToolTipId = m_nNextToolTipId++;

	m_viewList.AddTail(pMember);
	AddToolTip( pMember, lpszLabel);

	return pMember;
}

CToolTipCtrl *CTTabControlBar::GetToolTip()
{
	return m_TabCtrl.GetToolTips();
}

void CTTabControlBar::AddToolTip( TCB_ITEM *pMember, LPCTSTR lpszLabel)
{
	CToolTipCtrl *pToolTip = GetToolTip();

	if( pMember->m_nToolTipId == 0 )
		pToolTip->AddTool( &m_TabCtrl, lpszLabel, NULL, pMember->m_nToolTipId);
	else
		pToolTip->AddTool( &m_TabCtrl, lpszLabel, CRect( 0, 0, 0, 0), pMember->m_nToolTipId);
}

void CTTabControlBar::ResetToolTips( CToolTipCtrl *pToolTip)
{
	if(pToolTip)
	{
		int nIndex = 0;
		POSITION pos;

		for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
		{
			TCB_ITEM *pMember = m_viewList.GetAt(pos);

			pMember->m_nToolTipId = nIndex;
			pToolTip->UpdateTipText(
				pMember->m_strToolTipLabel,
				&m_TabCtrl,
				pMember->m_nToolTipId);

			nIndex++;
		}

		m_nNextToolTipId = nIndex == 0 ? 1 : nIndex;
	}
}

BOOL CTTabControlBar::AddView( LPCTSTR lpszLabel,
							   CRuntimeClass *pViewClass,
							   CDocument *pDoc,
							   UINT nIndex,
							   CCreateContext *pContext)
{
	if(!CreateTabView( pViewClass, pDoc, pContext))
		return FALSE;

	if(!CreateTabListItem(lpszLabel))
		return FALSE;

	ResetToolTips(GetToolTip());
	int nViews = nIndex == -1 ? m_viewList.GetCount() : nIndex;

	if( nViews != 1 )
	{
		m_pActiveView->EnableWindow(FALSE);
		m_pActiveView->ShowWindow(SW_HIDE);
	}

	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = (LPTSTR) lpszLabel;
	tci.iImage	= nViews - 1;
	m_TabCtrl.InsertItem( m_viewList.GetCount() - 1, &tci);
	SizeTabView(m_pActiveView);

	return TRUE;
}

void CTTabControlBar::SizeTabView( CWnd *pWnd)
{
	CRect rcChild;

	m_TabCtrl.GetWindowRect(&rcChild);
	m_TabCtrl.AdjustRect(
		FALSE,
		&rcChild);

	ScreenToClient(&rcChild);
	rcChild.bottom -= 3;

	pWnd->MoveWindow(rcChild);
	pWnd->SetWindowPos(
		NULL,
		0, 0,
		0, 0,
		SWP_NOSIZE|
		SWP_NOMOVE);
}

void CTTabControlBar::RemoveListItem( POSITION pos)
{
	if( pos != NULL )
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if( pMember->m_pWnd != NULL )
		{
			pMember->m_pWnd->DestroyWindow();
			pMember->m_pWnd = NULL;
		}

		m_viewList.RemoveAt(pos);
		if(pMember)
			delete pMember;
	}
}

void CTTabControlBar::DeleteView( int nView)
{
	POSITION pos = m_viewList.FindIndex(nView);

	if( pos != NULL )
	{
		int nActiveTab = m_nActiveTab;

		SetActiveView(-1);
		RemoveListItem(pos);
		m_TabCtrl.DeleteItem(nView);

		if( nActiveTab >= m_viewList.GetCount() )
			nActiveTab--;

		SetActiveView(nActiveTab);
		ResetToolTips(GetToolTip());
	}
}

void CTTabControlBar::DeleteView( CRuntimeClass *pViewClass)
{
	int nIndex = 0;
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if(pMember->m_pWnd->IsKindOf(pViewClass))
		{
			DeleteView(nIndex);
			return;
		}

		nIndex++;
	}
}

void CTTabControlBar::RemoveView( int nView)
{
	RemoveListItem(m_viewList.FindIndex(nView));
}

void CTTabControlBar::SetViewToolTip( int nView, LPCTSTR lpszLabel)
{
	POSITION pos = m_viewList.FindIndex(nView);

	if( pos != NULL )
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);
		pMember->m_strToolTipLabel = lpszLabel;
		GetToolTip()->UpdateTipText(
			pMember->m_strToolTipLabel,
			&m_TabCtrl,
			pMember->m_nToolTipId);
	}
}

void CTTabControlBar::SetViewToolTip( CRuntimeClass *pViewClass, LPCTSTR lpszLabel)
{
	int nIndex=0;
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if(pMember->m_pWnd->IsKindOf(pViewClass))
		{
			SetViewToolTip( nIndex, lpszLabel);
			break;
		}

		nIndex++;
	}
}

CView* CTTabControlBar::GetView( int nView)
{
	if( nView != -1 )
	{
		POSITION pos = m_viewList.FindIndex(nView);

		if( pos != NULL )
		{
			TCB_ITEM *pMember = m_viewList.GetAt(pos);
			return pMember ? (CView *) pMember->m_pWnd : NULL;
		}
	}

	return NULL;
}

CView* CTTabControlBar::GetView( CRuntimeClass *pViewClass)
{
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if(pMember->m_pWnd->IsKindOf(pViewClass))
			return (CView *) pMember->m_pWnd;
	}

	return NULL;
}

LPCTSTR CTTabControlBar::GetViewName( int nView)
{
	if( nView != -1 )
	{
		POSITION pos = m_viewList.FindIndex(nView);

		if( pos != NULL )
		{
			TCB_ITEM *pMember = m_viewList.GetAt(pos);
			return pMember ? (LPCTSTR) pMember->m_strTabLabel : NULL;
		}
	}

	return NULL;
}

LPCTSTR CTTabControlBar::GetViewName( CRuntimeClass *pViewClass)
{
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if(pMember->m_pWnd->IsKindOf(pViewClass))
			return (LPCTSTR) pMember->m_strTabLabel;
	}

	return NULL;
}

void CTTabControlBar::SetActiveView( int nView)
{
	if( nView < 0 )
	{
		m_TabCtrl.ShowWindow(SW_HIDE);
		m_pActiveView = NULL;
		m_nActiveTab = nView;
	}
	else if( nView != m_nActiveTab )
	{
		POSITION pos = m_viewList.FindIndex(nView);

		if( pos != NULL )
		{
			POSITION oldPos = NULL;

			if(!m_TabCtrl.IsWindowVisible())
				m_TabCtrl.ShowWindow(SW_SHOW);

			if( m_nActiveTab >= 0 )
				oldPos = m_viewList.FindIndex(m_nActiveTab);

			if(oldPos)
			{
				TCB_ITEM *pOldItem = m_viewList.GetAt(oldPos);

				pOldItem->m_pWnd->EnableWindow(FALSE);
				pOldItem->m_pWnd->ShowWindow(SW_HIDE);
			}

			TCB_ITEM *pMember = m_viewList.GetAt(pos);

			pMember->m_pWnd->EnableWindow(TRUE);
			pMember->m_pWnd->ShowWindow(SW_SHOW);
			pMember->m_pWnd->SetFocus();

			m_nActiveTab = nView;
			m_TabCtrl.SetCurSel(m_nActiveTab);

			m_pActiveView = DYNAMIC_DOWNCAST( CView, pMember->m_pWnd);
			GetParentFrame()->SetActiveView(m_pActiveView);
			SizeTabView(m_pActiveView);
		}
	}
}

void CTTabControlBar::SetActiveView( CRuntimeClass *pViewClass)
{
	int nView = 0;
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		TCB_ITEM *pMember = m_viewList.GetAt(pos);

		if(pMember->m_pWnd->IsKindOf(pViewClass))
		{
			m_pActiveView->EnableWindow(FALSE);
			m_pActiveView->ShowWindow(SW_HIDE);
			m_pActiveView = (CView *) pMember->m_pWnd;

			m_pActiveView->EnableWindow(TRUE);
			m_pActiveView->ShowWindow(SW_SHOW);
			m_pActiveView->SetFocus();
			m_nActiveTab = nView;

			m_TabCtrl.SetCurSel(m_nActiveTab);
			break;
		}

		nView++;
	}
}

void CTTabControlBar::SetActiveView( CView *pUserView)
{
	POSITION pos;

	for( pos = m_viewList.GetHeadPosition(); pos; m_viewList.GetNext(pos))
	{
		CView *pTabView = (CView *) m_viewList.GetAt(pos)->m_pWnd;

		if( pTabView == pUserView )
		{
			m_pActiveView = pTabView;
			((CFrameWnd *) GetParent())->SetActiveView((CView *) m_pActiveView);
		}
	}
}

void CTTabControlBar::AssociateImageIndexWithRuntimeClass( int nIndex, CRuntimeClass* pRuntimeClass)
{
	CString strClassName(pRuntimeClass->m_lpszClassName);
	m_ClassNameToImageIndexMap[strClassName] = (short *) nIndex;
}

void CTTabControlBar::AssociateImageIndexWithRuntimeClass( int nIndex, LPCTSTR lpszRuntimeClassName)
{
	CString strClassName(lpszRuntimeClassName);
	m_ClassNameToImageIndexMap[strClassName] = (short *) nIndex;
}

short CTTabControlBar::GetImageIndexFromRuntimeClass( CRuntimeClass *pRuntimeClass)
{
	CString strClassName(pRuntimeClass->m_lpszClassName);
	short *pValue;

	if(!m_ClassNameToImageIndexMap.Lookup( strClassName, (void *&) pValue))
		return -1;

	return *pValue;
}

BOOL CTTabControlBar::RemoveAllTabs()
{
	while(!m_viewList.IsEmpty())
	{
		RemoveView(0);
		m_TabCtrl.DeleteItem(0);
	}
	m_pActiveView = NULL;

	((CFrameWnd *) GetParent())->SetActiveView(NULL);
	Invalidate();

	return TRUE;
}

int CTTabControlBar::GetTabFromPoint( CPoint point)
{
	TCHITTESTINFO tch;
	memset( &tch, 0, sizeof(TCHITTESTINFO));

	if( point == CPoint( 0, 0) )
	{
		CPoint pt;

		GetCursorPos(&pt);
		ScreenToClient(&pt);
		tch.pt = pt;
	}
	else
		tch.pt = point;

	int nTab = m_TabCtrl.HitTest(&tch);
	if( nTab != -1 )
		m_nPointTab = nTab;

	return nTab;
}

BOOL CTTabControlBar::EnableToolTips( BOOL bEnable)
{
	BOOL bPrev = m_bToolTipsEnabled;

	m_bToolTipsEnabled = bEnable;
	CToolTipCtrl *pToolTip = GetToolTip();

	if(pToolTip)
		pToolTip->Activate(m_bToolTipsEnabled);
	else
		m_bToolTipsEnabled = bPrev;

	return bPrev;
}

BOOL CTTabControlBar::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}
