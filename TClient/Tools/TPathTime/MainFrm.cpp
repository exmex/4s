// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TPathTime.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


typedef enum TSTATUSBAR_NAME
{
	TSTATUSBAR_MENU_STATE = 0,
	TSTATUSBAR_UNIT_NAME,
	TSTATUSBAR_UNIT_ID,
	TSTATUSBAR_UNIT_X,
	TSTATUSBAR_UNIT_Z,
	TSTATUSBAR_MOVE_TIME,
	TSTATUSBAR_MOVE_DIST,
	TSTATUSBAR_CAPS_LOCK,
	TSTATUSBAR_NUM_LOCK,
	TSTATUSBAR_SCRL_LOCK,
	TSTATUSBAR_COUNT
} *LPTSTATUSBAR_NAME;

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
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

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_UNIT_NAME,
		ID_SEPARATOR,
		SBPS_NORMAL, 80);

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_UNIT_ID,
		ID_SEPARATOR,
		SBPS_NORMAL, 88);

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_UNIT_X,
		ID_SEPARATOR,
		SBPS_NORMAL, 55);

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_UNIT_Z,
		ID_SEPARATOR,
		SBPS_NORMAL, 55);

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_MOVE_TIME,
		ID_SEPARATOR,
		SBPS_NORMAL, 70);

	m_wndStatusBar.SetPaneInfo(
		TSTATUSBAR_MOVE_DIST,
		ID_SEPARATOR,
		SBPS_NORMAL, 70);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


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


// CMainFrame message handlers

void CMainFrame::UpdatePathTime( int nTIME, float fDIST)
{
	CString strDIST;
	CString strTIME;

	strTIME.Format( _T("%d  SEC"), nTIME);
	strDIST.Format( _T("%.2f  M"), fDIST);

	m_wndStatusBar.SetPaneText( TSTATUSBAR_MOVE_TIME, LPCSTR(strTIME));
	m_wndStatusBar.SetPaneText( TSTATUSBAR_MOVE_DIST, LPCSTR(strDIST));
}

void CMainFrame::UpdateUnitID( WORD wUnitID)
{
	static CString vNAME[TUNITCOUNT_X][TUNITCOUNT_Z] = {{
		CString(_T("UNIT_00")),
		CString(_T("UNIT_01")),
		CString(_T("UNIT_02")),
		CString(_T("UNIT_03")),
		CString(_T("UNIT_04")),
		CString(_T("UNIT_05")),
		CString(_T("UNIT_06")),
		CString(_T("UNIT_07"))}, {
		CString(_T("UNIT_10")),
		CString(_T("UNIT_11")),
		CString(_T("UNIT_12")),
		CString(_T("UNIT_13")),
		CString(_T("케테르")),
		CString(_T("코쿠마 해")),
		CString(_T("UNIT_16")),
		CString(_T("UNIT_17"))}, {
		CString(_T("UNIT_20")),
		CString(_T("UNIT_21")),
		CString(_T("UNIT_22")),
		CString(_T("비나")),
		CString(_T("호루스 랜드")),
		CString(_T("코쿠마")),
		CString(_T("블로네아")),
		CString(_T("UNIT_27"))}, {
		CString(_T("UNIT_30")),
		CString(_T("UNIT_31")),
		CString(_T("UNIT_32")),
		CString(_T("게브라")),
		CString(_T("타라트")),
		CString(_T("헤세드")),
		CString(_T("모스엘리엔")),
		CString(_T("UNIT_37"))}, {
		CString(_T("UNIT_40")),
		CString(_T("UNIT_41")),
		CString(_T("UNIT_42")),
		CString(_T("타이콘테로가")),
		CString(_T("티퍼레트")),
		CString(_T("아르드리")),
		CString(_T("하이랜드")),
		CString(_T("UNIT_47"))}, {
		CString(_T("UNIT_50")),
		CString(_T("UNIT_51")),
		CString(_T("UNIT_52")),
		CString(_T("호드")),
		CString(_T("예소드")),
		CString(_T("네자크")),
		CString(_T("UNIT_56")),
		CString(_T("UNIT_57"))}, {
		CString(_T("UNIT_60")),
		CString(_T("UNIT_61")),
		CString(_T("UNIT_62")),
		CString(_T("UNIT_63")),
		CString(_T("마르쿠트")),
		CString(_T("드래곤 로드")),
		CString(_T("UNIT_66")),
		CString(_T("UNIT_67"))}};

	CString strUnitID;
	CString strUnitX;
	CString strUnitZ;

	BYTE bUnitX = LOBYTE(wUnitID);
	BYTE bUnitZ = HIBYTE(wUnitID);

	strUnitID.Format( _T("Unit ID : 0x%04X"), wUnitID);
	strUnitX.Format( _T("Unit X : %d"), bUnitX);
	strUnitZ.Format( _T("Unit Z : %d"), bUnitZ);

	m_wndStatusBar.SetPaneText( TSTATUSBAR_UNIT_NAME, LPCSTR(vNAME[bUnitX][bUnitZ]));
	m_wndStatusBar.SetPaneText( TSTATUSBAR_UNIT_ID, LPCSTR(strUnitID));
	m_wndStatusBar.SetPaneText( TSTATUSBAR_UNIT_X, LPCSTR(strUnitX));
	m_wndStatusBar.SetPaneText( TSTATUSBAR_UNIT_Z, LPCSTR(strUnitZ));
}
