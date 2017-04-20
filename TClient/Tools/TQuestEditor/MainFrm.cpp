// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TQuestEditorDoc.h"
#include "TQuestEditorView.h"
#include "TQuestView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_INDICATOR_PROGRESS_CAP		2000
#define ID_INDICATOR_PROGRESS			1000


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // progress indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// Pane Index
#define PANE_STATUS					0
#define PANE_PROGRESS_BAR			1

// Progress Bar ID
#define ID_PRGBAR					101
#define PROGRESS_BAR_MAX_SIZE		300

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
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
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo( PANE_PROGRESS_BAR, ID_INDICATOR_PROGRESS, SBPS_NOBORDERS | SBPS_DISABLED, 0 );

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	BOOL bResult = m_wndSplitter.CreateStatic( this, 1, 2);

	if(bResult)
	{
		m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS(CTQuestEditorView), CSize( 250, 0), pContext);
		m_wndSplitter.CreateView( 0, 1, RUNTIME_CLASS(CTQuestView), CSize( 0, 0), pContext);
	}

	return bResult;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::InitProgressBar(int nMax)
{
	CString strCaption( "Loading Data..." );

	// 프로그레스바 생성
	m_ctrProgress.Create( WS_CHILD|WS_VISIBLE, CRect( 0,0,0,0 ), &m_wndStatusBar, ID_PRGBAR );

	// DC를 구하여 한 문자의 평균폭을 구한다.
	CDC* pDC = GetDC();
	CSize sizeOfpane = pDC->GetTextExtent( strCaption );
	ReleaseDC( pDC );

	// 첫번째 페인 설정.
	m_wndStatusBar.SetPaneInfo( PANE_STATUS, ID_INDICATOR_PROGRESS_CAP, SBPS_NOBORDERS, sizeOfpane.cx );

	// 첫번째 패널에 문자열 표시
	m_wndStatusBar.SetPaneText( PANE_STATUS, strCaption );

	// 두번째 페인 설정.
	CRect rect;
	m_wndStatusBar.GetItemRect( PANE_PROGRESS_BAR, rect );
	m_wndStatusBar.SetPaneInfo( PANE_PROGRESS_BAR, ID_INDICATOR_PROGRESS, SBPS_STRETCH, rect.Width() );
	SetProgressBarPane();

	// 프로그레스바 설정
	m_ctrProgress.SetRange( 0, nMax );      // 범위
	m_ctrProgress.SetPos( 0 );              // 초기위치
	m_ctrProgress.SetStep( 1 );             // 증가치
}

void CMainFrame::RemoveProgressBar()
{
	// 프로그레스바 콘트롤 제거
	m_ctrProgress.DestroyWindow();

	// 두번째 패널이 보이지 않도록 한다.
	CRect rect;
	m_wndStatusBar.SetPaneInfo( PANE_PROGRESS_BAR, ID_INDICATOR_PROGRESS, SBPS_NOBORDERS | SBPS_DISABLED, 0 );

	// 첫번째 패널의 용도를 원래대로 복원하기 위해 변경
	m_wndStatusBar.GetItemRect( PANE_STATUS, rect );
	m_wndStatusBar.SetPaneInfo( PANE_STATUS, ID_SEPARATOR, SBPS_NOBORDERS | SBPS_STRETCH, rect.Width() );

	// 아이들(idle) 상태일때의 상태바 메시지 표시
	CString str;
	str.LoadString( AFX_IDS_IDLEMESSAGE );
	m_wndStatusBar.SetPaneText( PANE_STATUS, str );
}

void CMainFrame::SetProgressBar(BOOL bFlag)
{
	if( bFlag )
		m_ctrProgress.StepIt();
	else
		m_ctrProgress.SetPos( 0 );
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	if ( m_ctrProgress.m_hWnd )
		SetProgressBarPane();
}

void CMainFrame::SetProgressBarPane()
{
	CRect rect;
	m_wndStatusBar.GetItemRect( PANE_PROGRESS_BAR, rect );

	int nWidth = rect.Width() / 2;
	m_ctrProgress.SetWindowPos( NULL, rect.left, rect.top,
		( nWidth < PROGRESS_BAR_MAX_SIZE ? nWidth : PROGRESS_BAR_MAX_SIZE ),
		rect.Height(), SWP_SHOWWINDOW );
}
