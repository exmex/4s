// TabCtrlST.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "TabCtrlST.h"
#include ".\tabctrlst.h"

#include "SvrTester.h"
#include "MainFrm.h"

// CTabCtrlST

IMPLEMENT_DYNAMIC(CTabCtrlST, CTabCtrl)
CTabCtrlST::CTabCtrlST()
{
}

CTabCtrlST::~CTabCtrlST()
{
	m_mapView.clear();
}


BEGIN_MESSAGE_MAP(CTabCtrlST, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
END_MESSAGE_MAP()

// CTabCtrlST 메시지 처리기입니다.
// View Add
BOOL CTabCtrlST::AddView(int nPageID, CRuntimeClass *pViewClass, CCreateContext *pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif
	
	CCreateContext context;
	if (pContext == NULL)
	{	
		context.m_pCurrentDoc = NULL;
		context.m_pCurrentFrame = GetParentFrame();
		context.m_pLastView = NULL;
		context.m_pNewDocTemplate = NULL;
		context.m_pNewViewClass = pViewClass;
		pContext = &context;
	}
	
	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0(_T("Out of memory creating a view.\n"));		
		return FALSE;
	}
	END_CATCH_ALL
		
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect;	

	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0(_T("Warning: couldn't create client pane for view.\n"));
		// pWnd will be cleaned up by PostNcDestroy		
		return FALSE;
	}
	m_pActiveView = (CView*) pWnd;

	pWnd->EnableWindow(FALSE);
	pWnd->ShowWindow(SW_HIDE);	

	CRect rcChild;	
	GetClientRect(rcChild);
	rcChild.DeflateRect(3,3);
	//rcChild.top += 30; // 탭의 위치 top
	rcChild.bottom -= 30; // 탭의 위치 bottom

	pWnd->MoveWindow(rcChild);

    m_mapView.insert(MAPVIEW::value_type(nPageID, m_pActiveView));    
	
	return TRUE;
}
// Active View Set
void CTabCtrlST::SetActiveView(int nPageID)
{	
	m_pActiveView->EnableWindow(FALSE);
	m_pActiveView->ShowWindow(SW_HIDE);

	MAPVIEW::iterator finder = m_mapView.find(nPageID);

	(*finder).second->EnableWindow(TRUE);
	(*finder).second->ShowWindow(SW_SHOW);
	(*finder).second->SetFocus();

	m_pActiveView = (CView *)(*finder).second;
}
// View Get
CView* CTabCtrlST::GetView(int nPageID)
{
	MAPVIEW::iterator finder = m_mapView.find(nPageID);
	return (*finder).second;
}
// Active View Get
CView* CTabCtrlST::GetActiveView()
{	
	return m_pActiveView;
}

void CTabCtrlST::Resize(int cx, int cy)
{
	SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOZORDER);
	CRect rcChild;	
	GetClientRect(rcChild);
	rcChild.DeflateRect(3,3);
	//rcChild.top += 30; // 탭의 위치 top
	rcChild.bottom -= 30; // 탭의 위치 bottom	

	MAPVIEW::iterator itV;
	for(itV=m_mapView.begin(); itV!=m_mapView.end(); itV++)
    {
        (*itV).second->MoveWindow(rcChild);
	}

	RedrawWindow();
}
// Tab Sel Change
void CTabCtrlST::TabSelchange()
{	
	SetActiveView(GetCurSel());
	
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition(); 
	
	CMultiDocTemplate* pDocTemplate; 
	for(int i = 0; i < GetCurSel() + 1; i++)
		pDocTemplate = (CMultiDocTemplate*)AfxGetApp()->GetNextDocTemplate(pos); 

	POSITION docpos = pDocTemplate->GetFirstDocPosition();
	CDocument *pDoc = pDocTemplate->GetNextDoc(docpos);

	docpos = pDoc->GetFirstViewPosition();
	CView *pView = pDoc->GetNextView(docpos);

	CMDIChildWnd *pChildFrame = (CMDIChildWnd*)pView->GetParentFrame();
	pChildFrame->MDIActivate();	
}
// Tab Sel Change
void CTabCtrlST::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{	
	TabSelchange();
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();	
	switch(GetCurSel())
	{
	case TSESSION:
		pFrm->SetCheckWindow(TSESSION);
		break;
	case TMACRO:
		pFrm->SetCheckWindow(TMACRO);
		break;
	}
}