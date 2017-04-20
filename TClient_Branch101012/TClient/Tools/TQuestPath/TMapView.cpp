// TMapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TQuestPath.h"
#include "TMapView.h"


// CTMapView

IMPLEMENT_DYNCREATE(CTMapView, CView)

CTMapView::CTMapView()
{
}

CTMapView::~CTMapView()
{
}

BEGIN_MESSAGE_MAP(CTMapView, CView)
END_MESSAGE_MAP()


// CTMapView 그리기입니다.

void CTMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CTMapView 진단입니다.

#ifdef _DEBUG
void CTMapView::AssertValid() const
{
	CView::AssertValid();
}

void CTMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CTMapView 메시지 처리기입니다.
