// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"

#include "TQuestPathView.h"
#include "TQuestView.h"
#include "TMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	m_bReady = FALSE;
	m_bInit = FALSE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndStatusBar.SetPaneInfo(
		TPANE_POS_X,
		ID_SEPARATOR,
		SBPS_NORMAL, 80);

	m_wndStatusBar.SetPaneInfo(
		TPANE_POS_Z,
		ID_SEPARATOR,
		SBPS_NORMAL, 80);

	m_wndStatusBar.SetPaneInfo(
		TPANE_SCALE,
		ID_SEPARATOR,
		SBPS_NORMAL, 50);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	BOOL bResult = m_wndSplitter[0].CreateStatic( this, 1, 2);

	if(bResult)
	{
		bResult = m_wndSplitter[1].CreateStatic(
			&m_wndSplitter[0],
			1, 2,
			WS_CHILD|WS_VISIBLE|WS_BORDER,
			m_wndSplitter[0].IdFromRowCol( 0, 0));

		if(bResult)
		{
			m_wndSplitter[0].CreateView( 0, 1, RUNTIME_CLASS(CTQuestPathView), CSize( 0, 0), pContext);
			m_wndSplitter[1].CreateView( 0, 1, RUNTIME_CLASS(CTQuestView), CSize( 0, 0), pContext);
			m_wndSplitter[1].CreateView( 0, 0, RUNTIME_CLASS(CTMapView), CSize( 0, 0), pContext);

			m_bReady = TRUE;
		}
	}

	return bResult;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기
void CMainFrame::OnSize( UINT nType, int cx, int cy)
{
	int nPathWidth = TPATHVIEW_X;
	int nMapWidth = TQUESTVIEW_X;

	if( m_bReady && nType != SIZE_MINIMIZED )
	{
		if(m_bInit)
		{
			int nMIN = 0;

			m_wndSplitter[0].GetColumnInfo( 1, nPathWidth, nMIN);
			m_wndSplitter[1].GetColumnInfo( 1, nMapWidth, nMIN);
		}

		nPathWidth = cx - nPathWidth - 11;
		nPathWidth = max( 0, nPathWidth);

		nMapWidth = nPathWidth - nMapWidth - 7;
		nMapWidth = max( 0, nMapWidth);
	}

	CFrameWnd::OnSize( nType, cx, cy);
	if( m_bReady && nType != SIZE_MINIMIZED )
	{
		m_wndSplitter[0].SetColumnInfo( 0, nPathWidth, 0);
		m_wndSplitter[1].SetColumnInfo( 0, nMapWidth, 0);

		m_wndSplitter[0].RecalcLayout();
		m_wndSplitter[1].RecalcLayout();
		m_bInit = TRUE;
	}
}

CStatusBar* CMainFrame::GetStatusBar()
{
	return &m_wndStatusBar;
}
