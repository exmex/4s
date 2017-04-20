// TQuestPathView.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "TPathWndMAIN.h"
#include "TPathWndMAP.h"
#include "TPathWndMON.h"
#include "TPathWndNPC.h"
#include "TPathWndPOS.h"
#include "TQuestPathView.h"


// CTQuestPathView

IMPLEMENT_DYNCREATE(CTQuestPathView, CScrollView)

CTQuestPathView::CTQuestPathView()
{
	for( int i=0; i<TPATHWND_COUNT; i++)
	{
		m_vTWNDSIZE[i].SetSize( 0, 0);
		m_vTWND[i] = NULL;
	}
}

CTQuestPathView::~CTQuestPathView()
{
}

BOOL CTQuestPathView::Create( LPCTSTR lpszClassName,
							  LPCTSTR lpszWindowName,
							  DWORD dwStyle,
							  const RECT& rect,
							  CWnd* pParentWnd,
							  UINT nID,
							  CCreateContext* pContext)
{
	BOOL bResult = CScrollView::Create(
		lpszClassName,
		lpszWindowName,
		dwStyle,
		rect,
		pParentWnd,
		nID,
		pContext);

	if(bResult)
	{
		CTPathWndMAIN *pTMAIN = new CTPathWndMAIN();
		CTPathWndMAP *pTMAP = new CTPathWndMAP();
		CTPathWndMON *pTMON = new CTPathWndMON();
		CTPathWndNPC *pTNPC = new CTPathWndNPC();
		CTPathWndPOS *pTPOS = new CTPathWndPOS();

		m_vTWND[TPATHWND_MAIN] = pTMAIN;
		m_vTWND[TPATHWND_MAP] = pTMAP;
		m_vTWND[TPATHWND_MON] = pTMON;
		m_vTWND[TPATHWND_NPC] = pTNPC;
		m_vTWND[TPATHWND_POS] = pTPOS;

		pTMAIN->Create(this);
		pTMAP->Create(this);
		pTMON->Create(this);
		pTNPC->Create(this);
		pTPOS->Create(this);

		for( int i=0; i<TPATHWND_COUNT; i++)
		{
			CRect rect;

			m_vTWND[i]->GetWindowRect(&rect);
			m_vTWNDSIZE[i].SetSize(
				rect.Width(),
				rect.Height());
		}
	}

	return bResult;
}

void CTQuestPathView::EnableTPATHWND( int nIndex)
{
	for( int i=0; i<TPATHWND_COUNT; i++)
		if(m_vTWND[i])
			m_vTWND[i]->ShowWnd(i == nIndex ? TRUE : FALSE);

	SetScrollSizes( MM_TEXT, m_vTWNDSIZE[nIndex]);
	ScrollToPosition(CPoint( 0, 0));
}


BEGIN_MESSAGE_MAP(CTQuestPathView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTQuestPathView 그리기입니다.

void CTQuestPathView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	UpdateDATA( TPATHWND_MAIN, TUPDATE_RESET, 0);
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		pDOC->ResetMode();
}

void CTQuestPathView::OnDraw( CDC *pDC)
{
}


// CTQuestPathView 진단입니다.

#ifdef _DEBUG
void CTQuestPathView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTQuestPathView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTQuestPathDoc* CTQuestPathView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTQuestPathDoc)));
	return (CTQuestPathDoc *)m_pDocument;
}
#endif //_DEBUG


// CTQuestPathView 메시지 처리기입니다.

BOOL CTQuestPathView::OnEraseBkgnd( CDC *pDC)
{
	CRect rect;

	GetClientRect(&rect);
	pDC->FillSolidRect(
		&rect,
		GetSysColor(COLOR_BTNFACE));

	return TRUE;
}

void CTQuestPathView::OnDestroy()
{
	for( int i=0; i<TPATHWND_COUNT; i++)
		if(m_vTWND[i])
		{
			m_vTWND[i]->DestroyWindow();

			delete m_vTWND[i];
			m_vTWND[i] = NULL;
		}

	CScrollView::OnDestroy();
}

void CTQuestPathView::UpdateDATA( int nIndex,
								  int nType,
								  LPARAM lParam)
{
	if(m_vTWND[nIndex])
		m_vTWND[nIndex]->UpdateDATA( nType, lParam);
}
