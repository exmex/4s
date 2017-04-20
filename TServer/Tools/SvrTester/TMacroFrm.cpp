// TMacroFrm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TMacroFrm.h"
#include ".\tmacrofrm.h"


// CTMacroFrm

IMPLEMENT_DYNCREATE(CTMacroFrm, CMDIChildWnd)

CTMacroFrm::CTMacroFrm()
{
}

CTMacroFrm::~CTMacroFrm()
{
}


BEGIN_MESSAGE_MAP(CTMacroFrm, CMDIChildWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTMacroFrm 메시지 처리기입니다.

void CTMacroFrm::ActivateFrame(int nCmdShow)
{
	CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED);
}

int CTMacroFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndSideBar.Create(this, IDD_DIALOGBAR2, WS_CHILD | WS_VISIBLE | CBRS_LEFT, IDD_DIALOGBAR))
	{
		TRACE0("사이드바를 만들지 못했습니다.\n");
		return -1;
	}    

	// CTMacroDlg Dialogbar
	m_dlgSideBar.Create(IDD_TMACRODLG, &m_wndSideBar);	
	m_dlgSideBar.ShowWindow(SW_SHOW);

	// TMacroFrm의 클래스포인터를 넘겨준다
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->pTMacroFrm = this;

	return 0;
}

////////////////////////////////////////////////////////
// Get Macro Dlg
CTMacroDlg* CTMacroFrm::GetTMacroDlg()
{
	return &m_dlgSideBar;
}
////////////////////////////////////////////////////////