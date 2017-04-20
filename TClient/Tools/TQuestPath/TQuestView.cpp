// TQuestView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TQuestView.h"


// CTQuestView

IMPLEMENT_DYNCREATE(CTQuestView, CTreeView)

CTQuestView::CTQuestView()
{
	m_listTreeImage.Create( IDB_BITMAP_TREEIMG, 16, 0, RGB( 255, 255, 255));
}

CTQuestView::~CTQuestView()
{
}

BEGIN_MESSAGE_MAP(CTQuestView, CTreeView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()


// CTQuestView 진단입니다.

#ifdef _DEBUG
void CTQuestView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTQuestView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CTQuestPathDoc* CTQuestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTQuestPathDoc)));
	return (CTQuestPathDoc*)m_pDocument;
}
#endif //_DEBUG


// CTQuestView 메시지 처리기입니다.

BOOL CTQuestView::Create( LPCTSTR lpszClassName,
						  LPCTSTR lpszWindowName,
						  DWORD dwStyle,
						  const RECT& rect,
						  CWnd* pParentWnd,
						  UINT nID,
						  CCreateContext* pContext)
{
	BOOL bResult = CTreeView::Create(
		lpszClassName,
		lpszWindowName,
		dwStyle,
		rect,
		pParentWnd,
		nID,
		pContext);

	if(bResult)
	{
		CTreeCtrl& cTree = GetTreeCtrl();

		LONG nStyle = GetWindowLong(
			cTree.GetSafeHwnd(),
			GWL_STYLE);

		SetWindowLong(
			cTree.GetSafeHwnd(),
			GWL_STYLE,
			nStyle
			|TVS_HASLINES
			|TVS_LINESATROOT
			|TVS_HASBUTTONS
			|TVS_SHOWSELALWAYS);

		cTree.SetImageList( &m_listTreeImage, TVSIL_NORMAL);
	}

	return bResult;
}

void CTQuestView::OnDestroy()
{
	CTreeCtrl& cTree = GetTreeCtrl();

	cTree.DeleteAllItems();
	m_listTreeImage.DeleteImageList();

	CTreeView::OnDestroy();
}

void CTQuestView::OnTvnDeleteitem( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	LPTQITEM pTQITEM = (LPTQITEM) pNMTreeView->itemOld.lParam;
	CTreeCtrl& cTree = GetTreeCtrl();

	if(pTQITEM)
	{
		CTQuestPathDoc *pDOC = GetDocument();

		if(pDOC)
		{
			LPMAPTQITEM pTBUF = pTQITEM->m_bType == TQITEM_CLASS ? &pDOC->m_mapTQCLASS : &pDOC->m_mapTMISSION;
			MAPTQITEM::iterator finder = pTBUF->find(pTQITEM->m_dwID);

			if( finder != pTBUF->end() )
				pTBUF->erase(finder);
		}

		delete pTQITEM;
	}

	cTree.Invalidate();
	*pResult = 0;
}

void CTQuestView::OnTvnSelchanged( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	LPTQITEM pTQITEM = (LPTQITEM) pNMTreeView->itemNew.lParam;

	if(pTQITEM)
	{
		CTQuestPathDoc *pDOC = GetDocument();

		if(pDOC)
			pDOC->UpdateDATA(pTQITEM);
	}

	*pResult = 0;
}
