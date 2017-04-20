// OBJTypeView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "OBJTypeView.h"


// COBJTypeView

IMPLEMENT_DYNCREATE(COBJTypeView, CView)

COBJTypeView::COBJTypeView()
{
}

COBJTypeView::~COBJTypeView()
{
}

BEGIN_MESSAGE_MAP(COBJTypeView, CView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// COBJTypeView 그리기입니다.

void COBJTypeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// COBJTypeView 진단입니다.

#ifdef _DEBUG
void COBJTypeView::AssertValid() const
{
	CView::AssertValid();
}

void COBJTypeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// COBJTypeView 메시지 처리기입니다.

void COBJTypeView::OnSize( UINT nType, int cx, int cy)
{
	if(m_wndSHEET.GetSafeHwnd())
	{
		CTabCtrl *pCtrl = m_wndSHEET.GetTabControl();
		CRect rect( 0, 0, cx, cy);

		m_wndSHEET.MoveWindow(rect);
		pCtrl->MoveWindow(rect);
		pCtrl->AdjustRect(
			FALSE,
			&rect);

		m_wndATTR.MoveWindow(rect);
		if(m_wndOBJ.GetSafeHwnd())
			m_wndOBJ.MoveWindow(rect);
	}

	CView::OnSize(nType, cx, cy);
}

void COBJTypeView::OnDestroy()
{
	m_listTabImages.DeleteImageList();
	CView::OnDestroy();
}

void COBJTypeView::OnInitialUpdate()
{
	COBJTypeDoc *pDoc = GetDocument();

	m_wndATTR.SetData(
		NULL,
		&pDoc->m_pItem->m_mapATTR,
		&pDoc->m_pItem->m_mapNAME);

	m_wndOBJ.SetData(
		NULL,
		&pDoc->m_pItem->m_OBJ,
		&pDoc->m_pItem->m_mapNAME);

	CView::OnInitialUpdate();
}

int COBJTypeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndSHEET.AddPage(&m_wndATTR);
	m_wndSHEET.AddPage(&m_wndOBJ);
	m_wndSHEET.Create(
		this,
		WS_CHILD|WS_VISIBLE);

	m_wndSHEET.SetActivePage(&m_wndOBJ);
	m_wndSHEET.SetActivePage(&m_wndATTR);

	CTabCtrl *pCtrl = m_wndSHEET.GetTabControl();
	TCITEM item;

	m_listTabImages.Create(
		IDB_OBJTYPE_TAB,
		17, 1,
		RGB( 255, 255, 255));
	pCtrl->SetImageList(&m_listTabImages);

	item.mask = TCIF_IMAGE;
	item.iImage = 0;
	pCtrl->SetItem( 0, &item);

	item.iImage = 1;
	pCtrl->SetItem( 1, &item);

	return 0;
}
