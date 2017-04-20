// TSessionFrm.cpp : CTSessionFrm 클래스의 구현
//
#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionFrm.h"
#include ".\tsessionfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSessionFrm

IMPLEMENT_DYNCREATE(CTSessionFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CTSessionFrm, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTSessionFrm 생성/소멸

CTSessionFrm::CTSessionFrm()
{
	
}

CTSessionFrm::~CTSessionFrm()
{	
}


BOOL CTSessionFrm::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CTSessionFrm 진단

#ifdef _DEBUG
void CTSessionFrm::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CTSessionFrm::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CTSessionFrm 메시지 처리기

void CTSessionFrm::ActivateFrame(int nCmdShow)
{
	CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

int CTSessionFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndSideBar.Create(this, IDD_DIALOGBAR1, WS_CHILD | WS_VISIBLE | CBRS_LEFT, IDD_DIALOGBAR))
	{
		TRACE0("사이드바를 만들지 못했습니다.\n");
		return -1;
	}    

	// CTSessionDlg Dialogbar
	m_dlgSideBar.Create(IDD_TSESSIONDLG, &m_wndSideBar);	
	m_dlgSideBar.ShowWindow(SW_SHOW);

	// TSessionFrm의 클래스포인터를 넘겨준다
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->pTSessionFrm = this;

	return 0;
}

////////////////////////////////////////////////////////
// Get Session Dlg
CTSessionDlg* CTSessionFrm::GetTSessionDlg()
{
	return &m_dlgSideBar;
}
////////////////////////////////////////////////////////