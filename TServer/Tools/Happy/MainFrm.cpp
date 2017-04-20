// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Happy.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "HappyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_SERVER_CONNECT, OnUpdateServerConnect)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_START, OnUpdateServiceStart)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_STOP, OnUpdateServiceStop)
	ON_UPDATE_COMMAND_UI(ID_SERVER_DISCONNECT, OnUpdateServerDisconnect)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_ALERT, OnUpdateServiceAlert)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_ALL_VIEW, OnUpdateServiceAllView)
	ON_UPDATE_COMMAND_UI(ID_SERVICE_UPLOAD, OnUpdateServiceUpload)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_COMPARE, OnUpdateButtonCompare)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PATCHUP, OnUpdateButtonPatchup)
	ON_UPDATE_COMMAND_UI(ID_CURUSER_GRAPH, OnUpdateCuruserGraph)
	ON_UPDATE_COMMAND_UI(ID_GMTOOL, OnUpdateGmtool)
	ON_UPDATE_COMMAND_UI(ID_SERVCIE_LOG, OnUpdateServcieLog)
	ON_UPDATE_COMMAND_UI(ID_USERWEB, OnUpdateUserweb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SERVICEAUTOSTART, OnUpdateServiceAutoStart)
	ON_UPDATE_COMMAND_UI(ID_SDATACLEAR, OnUpdateServiceDataClear)
	ON_UPDATE_COMMAND_UI(ID_USERADMIN_EXPORTHAPPYDATA, OnUpdateExportHappydata)
	ON_UPDATE_COMMAND_UI(ID_USERADMIN_CHATBANLIST, OnUpdateUseradminChatbanlist)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_BAR_CONTROL))
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

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndPatchBar.Create(this, IDD_DIALOG_PATCHBAR, CBRS_TOP, IDD_DIALOG_PATCHBAR);
	m_wndPatchBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPatchBar);

	ShowToolBar(0);

	// AutoStart ON 이미지를 툴바 이미지리스트에 등록
	m_dwAutoNormal = 7;
	CImageList* pImgList;
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP2);
	pImgList = m_wndToolBar.GetToolBarCtrl().GetImageList();
	m_dwAutoPush = pImgList->GetImageCount();
	pImgList->Add(&bmp,RGB(255,0,255) );
	m_wndToolBar.GetToolBarCtrl().SetImageList(pImgList);

	SetTimer(TIMER_CHECK_BEEP, 1000, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_wndSplitter.CreateStatic(this, 1, 2);

	m_nGroup = m_wndSplitter.AddView(0,0,RUNTIME_CLASS(CHappyGroup),pContext);
	m_nTree = m_wndSplitter.AddView(0,0,RUNTIME_CLASS(CServiceTree),pContext);
	m_nPatch = m_wndSplitter.AddView(0,0,RUNTIME_CLASS(CAutoPatch), pContext);
	m_nMachine = m_wndSplitter.AddView(0,0,RUNTIME_CLASS(CMachine), pContext); // 현승룡 Machine
	
	m_nMachineGraph = m_wndSplitter.AddView(0, 1, RUNTIME_CLASS(CMachineGraph), pContext);
	m_nListPatch = m_wndSplitter.AddView(0,1,RUNTIME_CLASS(CPatchList), pContext);
	m_nGraph = m_wndSplitter.AddView(0,1,RUNTIME_CLASS(CServiceGraph), pContext); // 현승룡 Graph
	m_nList = m_wndSplitter.AddView(0,1,RUNTIME_CLASS(CServiceList), pContext);
	
	CHappyGroup * pWnd = (CHappyGroup *)m_wndSplitter.GetView(m_nGroup);
	CServiceTree * pTree = (CServiceTree *)m_wndSplitter.GetView(m_nTree);
	CAutoPatch * pPatch = (CAutoPatch *)m_wndSplitter.GetView(m_nPatch);
	CMachine * pMachine = (CMachine *)m_wndSplitter.GetView(m_nMachine);
	pWnd->OnAddTab(_T("Service"), pTree);
	pWnd->OnAddTab(_T("Auto Patch"), pPatch);
	pWnd->OnAddTab(_T("Machine"), pMachine);

	m_wndSplitter.ShowView(m_nGroup);

	// 현승룡 버튼 활성화 / 비활성화
	SetAllControlEnable(FALSE,TRUE);
	
	return CMDIFrameWnd::OnCreateClient(lpcs, pContext);	
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == TIMER_CHECK_BEEP)
	{
		CHappyDoc * pHappy = (CHappyDoc *)GetActiveDocument();
		if(!pHappy)
			return;

		CHappyApp *pApp = (CHappyApp *)AfxGetApp();
		if(!pApp)
			return;

		if(pHappy->CheckBeep() && pHappy->m_bAlertOnFail)
			pApp->Beep();
	}

	if(nIDEvent == TIMER_CHECK_LOG)
	{
		CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
		if(!pDoc)
		{
			pDoc = (CHappyDoc *)GetFirstDocument();
			if(!pDoc)
				return;
		}

		pDoc->SaveLogData();
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

CDocument * CMainFrame::GetFirstDocument()
{
	CHappyApp *pApp = (CHappyApp *)AfxGetApp();
	if(pApp)
	{
		POSITION pos = pApp->GetFirstDocTemplatePosition();
		if(!pos) return NULL;
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(pos);
		if(pDocTemplate)
		{
			pos = pDocTemplate->GetFirstDocPosition();
			if(!pos)
				return NULL;
			else
				return pDocTemplate->GetNextDoc(pos);
		}
	}
	return NULL;
}
void CMainFrame::OnUpdateServerConnect(CCmdUI *pCmdUI)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}
	if(pDoc->m_session.IsValid())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}
void CMainFrame::OnUpdateServerDisconnect(CCmdUI *pCmdUI)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_session.IsValid())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
////////////////////////////////////////////////////
// 현승룡 서비스 시작/정지 버튼 활성화/비활성화
void CMainFrame::SetServiceButton(BYTE bService)
{
    m_bService = bService;
}
////////////////////////////////////////////////////
void CMainFrame::OnUpdateServiceStart(CCmdUI *pCmdUI)
{
	if(m_bService == FALSE) // 현승룡 활성화/비활성화
	{
		pCmdUI->Enable(m_bService);
		return;
	}

	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	LPHAPPYSERVICE pService = pDoc->FindService(pDoc->m_dwSelectedService);
	if(!pService)
	{
		if(pDoc->IsGroup())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
		return;
	}

	if(pService->m_dwStatus == DCSVC_STAT_STOPPED)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateServiceStop(CCmdUI *pCmdUI)
{
	if(m_bService == FALSE) // 현승룡 활성화/비활성화
	{
		pCmdUI->Enable(m_bService);
		return;
	}

	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	LPHAPPYSERVICE pService = pDoc->FindService(pDoc->m_dwSelectedService);
	if(!pService)
	{
		if(pDoc->IsGroup())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
		return;
	}

	if(pService->m_dwStatus == DCSVC_STAT_RUNNING)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateServiceAlert(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bAlert); // 현승룡 버튼 활성화/비활성화

	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_bAlertOnFail)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateServiceAllView(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bAllView); // 현승룡 버튼 활성화/비활성화

	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_bViewAllService)
	{
		pCmdUI->SetCheck(TRUE);		
	}
	else
	{
		pCmdUI->SetCheck(FALSE);        
	}
}

////////////////////////////////////////////////////////////
// 현승룡 서비스전체보기/경보음 버튼 활성화/비활성화
void CMainFrame::SetAllView(BYTE bAllView)
{
	m_bAllView = bAllView;
}

void CMainFrame::SetAlertButton(BYTE bAlert)
{
	m_bAlert = bAlert;
}
////////////////////////////////////////////////////////////

void CMainFrame::ShowToolBar(int nIndex)
{
	switch(nIndex)
	{
	case 0:
		ShowControlBar(&m_wndToolBar, TRUE, 0);
		ShowControlBar(&m_wndPatchBar, FALSE, 0);
		break;
	case 1:
		ShowControlBar(&m_wndToolBar, FALSE, 0);
		ShowControlBar(&m_wndPatchBar, TRUE, 0);
		break;
	}
}
void CMainFrame::GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort)
{
	m_wndPatchBar.GetFtp(strAddr, strUser, strPasswd, wPort);
}
void CMainFrame::GetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort)
{
	*dwAddr = m_wndPatchBar.m_dwIP;
	*dwPort = m_wndPatchBar.m_dwPort;
	*strUser = m_wndPatchBar.m_strUserID;
}
void CMainFrame::SetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort)
{
	in_addr addr;
	addr.S_un.S_addr = *dwAddr;
	m_wndPatchBar.SetFtp( &CString(inet_ntoa(addr)), strUser, dwPort);
}

// PatchBar의 Compare 버튼 Visible Setting
void CMainFrame::SetPatchBarCompareVisible(BYTE bCompare)
{
	m_wndPatchBar.m_bEnableCompare = bCompare;
}

// PatchBar의 PatchUpdate 버튼 Visible Setting
void CMainFrame::SetPatchBarPatchUpdateVisible(BYTE bPatchUpdate)
{
	m_wndPatchBar.m_bEnablePatchUpdate = bPatchUpdate;
}

void CMainFrame::OnUpdateServiceUpload(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable( m_wndPatchBar.m_bEnableCompare );
}

void CMainFrame::OnUpdateButtonCompare(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable( m_wndPatchBar.m_bEnableCompare );
}

void CMainFrame::OnUpdateButtonPatchup(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable( m_wndPatchBar.m_bEnablePatchUpdate );
}

///////////////////////////////////////////////////////////////////
// 현승룡 Graph
void CMainFrame::SetGraphButton(BYTE bGraph)
{
	m_bGraph = bGraph;
}
void CMainFrame::OnUpdateCuruserGraph(CCmdUI *pCmdUI)
{	
	pCmdUI->Enable(m_bGraph);
	
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_bGraphCheck)
	{
		pCmdUI->SetCheck(TRUE);		
	}
	else
	{
		pCmdUI->SetCheck(FALSE);		
	}
}

BYTE CMainFrame::GetGraphCheck()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return 0;
	}

	return pDoc->m_bGraphCheck;
}

void CMainFrame::SetGraphCheck()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->m_bGraphCheck = !pDoc->m_bGraphCheck;
}

BYTE CMainFrame::GetCurView()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return 0;
	}
	return pDoc->GetCurView();
}
void CMainFrame::SetCurView(BYTE bCur)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}
	pDoc->SetCurView(bCur);
}
// 현승룡 로그 기록 활성화/비활성화
void CMainFrame::SetServiceLog(BYTE bLog)
{
	m_bLog = bLog;
}
void CMainFrame::OnUpdateServcieLog(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bLog);
}
// 현승룡 로그 기록
void CMainFrame::SetLogTimer(BYTE bResult, DWORD dwTime)
{
	if(bResult)
		SetTimer(TIMER_CHECK_LOG, dwTime, NULL);
	else
		KillTimer(TIMER_CHECK_LOG);
}
// 현승룡 유저 웹 활성화/비활성화
void CMainFrame::SetUserWeb(BYTE bWeb)
{
	m_bUserWeb = bWeb;
}
void CMainFrame::OnUpdateUserweb(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bUserWeb);
}
// 현승룡 운영자 툴 활성화/비활성화
void CMainFrame::SetGMTool(BYTE bGMTool)
{
	m_bGMTool = bGMTool;
}
void CMainFrame::OnUpdateGmtool(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bGMTool);
}
///////////////////////////////////////////////////////////////////
void CMainFrame::SetAutoStart(BYTE _bAutoStart)
{
	m_bAutoStart = _bAutoStart;
}

void CMainFrame::OnUpdateServiceAutoStart(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	pCmdUI->Enable(m_bAutoStart);

	CHappyDoc * pDoc = (CHappyDoc *)GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(pDoc->m_bServiceAutoStart)
	{
		pCmdUI->SetCheck(TRUE);
		
		if( !pDoc->m_bBtnAutoStart )
		{
			pDoc->m_bBtnAutoStart = TRUE;
			m_wndToolBar.SetButtonInfo(m_dwAutoNormal,ID_VIEW_SERVICEAUTOSTART,TBBS_CHECKED,m_dwAutoPush);// 푸쉬상태. 푸쉬이미지로 변경.
         
		}
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		if(pDoc->m_bBtnAutoStart)
		{
			pDoc->m_bBtnAutoStart = FALSE;
			m_wndToolBar.SetButtonInfo(m_dwAutoNormal,ID_VIEW_SERVICEAUTOSTART,NULL,m_dwAutoNormal);//노멀상태. 노멀이미지로 변경
		}
	}
}

void CMainFrame::SetAllControlEnable(BYTE bEnable, BYTE bServiceChange)
{
	m_bGraph = bEnable; 
	m_bAllView = bEnable;
	m_bAlert = bEnable;	
	m_bLog = bEnable;
	m_bUserWeb = bEnable;
	m_bGMTool = bEnable;
	m_bAutoStart = bEnable;
	m_bServiceDataClear = bEnable;
	m_bExportHappyData = bEnable;
	m_bChatBanList = bEnable;

	if(bServiceChange )
        m_bService = bEnable;
}

void CMainFrame::SetServiceDataClear(BYTE bView)
{
	m_bServiceDataClear = bView;
}
void CMainFrame::OnUpdateServiceDataClear(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_bServiceDataClear);
}

void CMainFrame::OnUpdateExportHappydata(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_bExportHappyData);
}

void CMainFrame::SetExportHappyData(BYTE bView)
{
	m_bExportHappyData = bView;
}

void CMainFrame::SetChatBanList(BYTE bView)
{
	m_bChatBanList = bView;
}

void CMainFrame::OnUpdateUseradminChatbanlist(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_bChatBanList);
}
