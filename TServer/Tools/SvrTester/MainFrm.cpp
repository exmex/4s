// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#include "ManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_MANAGER_CONNECT, OnManagerConnect)
	ON_MESSAGE( WM_SESSION_MSG, OnSessionMSG) // WM_USER_MSG
	ON_MESSAGE( WM_SESSION_CLOSE, OnSessionClose)
	ON_MESSAGE( WM_PUSH_LIST, OnPushList)
	ON_MESSAGE( WM_CHARINFO_VIEW, OnCharinfoView)
	ON_MESSAGE( WM_SESSIONLIST_VIEW, OnSessionlistView)
	ON_COMMAND(ID_MANAGER_DISCONNECT, OnManagerDisconnect)
	ON_COMMAND(ID_WINDOW_MACRO, OnWindowMacro)
	ON_COMMAND(ID_WINDOW_SESSION, OnWindowSession)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SESSION, OnUpdateWindowSession)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MACRO, OnUpdateWindowMacro)
	ON_UPDATE_COMMAND_UI(ID_MANAGER_CONNECT, OnUpdateManagerConnect)
	ON_UPDATE_COMMAND_UI(ID_MANAGER_DISCONNECT, OnUpdateManagerDisconnect)
	ON_COMMAND(ID_FILE_MACROSAVE, OnFileMacrosave)
	ON_COMMAND(ID_FILE_MACROLOAD, OnFileMacroload)
	ON_UPDATE_COMMAND_UI(ID_FILE_MACROSAVE, OnUpdateFileMacrosave)
	ON_UPDATE_COMMAND_UI(ID_FILE_MACROLOAD, OnUpdateFileMacroload)
	ON_COMMAND(ID_FILE_NEWMACRO, OnFileNewmacro)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWMACRO, OnUpdateFileNewmacro)
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
	m_mapBase.clear();
	m_mapProtocol.clear();
	m_mapType.clear();

	m_ConnectEnabel = TRUE;

	m_CheckSession = TRUE;
	m_CheckMacro = FALSE;

	TCHAR temp[200]={0};
	GetCurrentDirectory(200, temp);
	m_strCurDirectroy.Format(_T("%s"), temp);

	m_strMIP = _T("0.0.0.0");
	m_strMPort = _T("0");
	m_strMUser = _T(" ");
	m_strSIP = _T("0.0.0.0");
	m_strSPort = _T("0");

    CString path = m_strCurDirectroy + _T("\\") + FILE_NAME_CONNECT;
	CFile f;
	if(f.Open(path, CFile::modeRead))
	{
		CString strData;
		TCHAR id[50]={0};

		if(f.Read(id, 50))
			m_strMIP = id;

		if(f.Read(id, 50))
			m_strMPort = id;

		if(f.Read(id, 50))
			m_strMUser = id;

		if(f.Read(id, 50))
			m_strSIP = id;

		if(f.Read(id, 50))
			m_strSPort = id;

		f.Close();
	}
}

CMainFrame::~CMainFrame()
{
	m_mapBase.clear();
	m_mapProtocol.clear();
	m_mapType.clear();

	CString path = m_strCurDirectroy + _T("\\") + FILE_NAME_CONNECT;
	CFile f;
	if(f.Open(path,CFile::modeCreate | CFile::modeWrite))
	{
		f.Write(m_strMIP, 50);
		f.Write(m_strMPort, 50);
		f.Write(m_strMUser, 50);
		f.Write(m_strSIP, 50);
		f.Write(m_strSPort, 50);
		f.Close();
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0(_T("도구 모음을 만들지 못했습니다.\n"));
		return -1;      // 만들지 못했습니다.
	}
 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0(_T("상태 표시줄을 만들지 못했습니다.\n"));
		return -1;      // 만들지 못했습니다.
	}		

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if(!m_wndSideBar.Create(this, IDD_DIALOGBAR, WS_CHILD | WS_VISIBLE | CBRS_LEFT, IDD_DIALOGBAR))
	{
		TRACE0(_T("사이드바를 만들지 못했습니다.\n"));
		return -1;
	}
    
    m_wndSideBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);	

	// CTabGroup Dialogbar
	m_dlgSideBar.Create(IDD_TABGROUP, &m_wndSideBar);	
	m_dlgSideBar.ShowWindow(SW_SHOW);

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

/////////////////////////////////////////////////////////////////////////////////////
// WM_USER_MSG (TachyonSession)
LRESULT CMainFrame::OnSessionMSG( WPARAM wParam, LPARAM lParam)
{	
	CTachyonSession* pSession = CTachyonSession::GetSession(wParam);	

	int nError = WSAGETSELECTERROR(lParam);	

	if(pSession)
	{
		if( pSession->m_SessionID > 0 ) // Session ID
		{
			CSvrTesterApp* pApp = (CSvrTesterApp *)AfxGetApp();
			CTSessionView* pView = pApp->GetTSessionView();
			CTSessionDoc* pDoc = (CTSessionDoc *)pView->GetDocument();
			if( !pDoc->m_bRun )
				return 0;

			STRUCTTHREAD sThread;

			sThread.nError = nError;
			sThread.type = THREAD_TYPE_RECV;
			sThread.lParam = lParam;
			pSession->PushMessage(sThread); // Message Queue Push
		}
		else
		{
			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_CONNECT	: pSession->OnConnect(nError); break;
			case FD_READ	: pSession->OnReceive(nError); break;
			case FD_WRITE	: pSession->OnSend(nError); break;
			case FD_CLOSE	: pSession->OnClose(nError); break;
			}
		}
	}
	return 0;
}
LRESULT CMainFrame::OnSessionClose( WPARAM wParam, LPARAM lParam)
{
	CSvrTesterApp* pApp = (CSvrTesterApp *)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	CTSessionDoc* pDoc = (CTSessionDoc *)pView->GetDocument();

	int index = pView->GetIndexComboThread((DWORD)wParam);
	if(index >= 0)
	{		
		pView->m_comboThread.DeleteString(index);		
		pView->ClearList();

		if(pView->m_comboThread.GetCount())
		{
			pView->m_comboThread.SetCurSel(0);
			pDoc->ShowCurSessionList();			
		}

		pDoc->ShowCharInfo();
		pView->SetEditConnect();
	}

	pDoc->OnSessionClose(DWORD(wParam));

	return 0;
}
LRESULT CMainFrame::OnPushList( WPARAM wParam, LPARAM lParam)
{
	STRUCTLIST* s = (STRUCTLIST*)wParam;

	CSvrTesterApp* pApp = (CSvrTesterApp *)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	if( !pView->GetDocument()->m_bRun )
	{
		delete s;
		return 0;
	}	

	CTachyonSession* pSession = s->pSession;
	pSession->PushList(s->strP, s->data);
	delete s;

	return 0;
}
LRESULT CMainFrame::OnCharinfoView( WPARAM wParam, LPARAM lParam)
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	if( !pView->GetDocument()->m_bRun )
		return 0;
	
	pView->GetDocument()->ShowCharInfo();

	return 0;
}
LRESULT CMainFrame::OnSessionlistView( WPARAM wParam, LPARAM lParam)
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();	

	if( !pView->GetDocument()->m_bRun )
		return 0;
	
	pView->GetDocument()->ShowCurSessionList();

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Get CTabGroup
CTabGroup* CMainFrame::GetTabGroup()
{
	return &m_dlgSideBar;
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Manager Svr Connect
void CMainFrame::OnManagerConnect()
{	
	CManagerDlg dlg;

	dlg.m_strIP = m_strMIP;
	dlg.m_strPort = m_strMPort;
	dlg.m_strUser = m_strMUser;

	if( dlg.DoModal() == IDOK )
	{
		m_mapBase.clear();
		m_mapProtocol.clear();
		m_mapType.clear();

		// Protocol
		if( !LoadProtocolBase() )
		{
			AfxMessageBox(_T("베이스 파일을 열 수 없습니다."));
			m_mapBase.clear();
			m_mapProtocol.clear();
			m_mapType.clear();
			return;
		}

		if( !LoadProtocol(dlg.m_strFile) )
		{
			AfxMessageBox(_T("프로토콜 파일을 열 수 없습니다."));
			m_mapBase.clear();
			m_mapProtocol.clear();
			m_mapType.clear();
			return;
		}

		if( m_mapBase.empty() || m_mapProtocol.empty() )
		{
			AfxMessageBox(_T("프로토콜이 비어있습니다."));
			m_mapBase.clear();
			m_mapProtocol.clear();
			m_mapType.clear();
			return;
		}

		m_strMIP = dlg.m_strIP;
		m_strMPort = dlg.m_strPort;
		m_strMUser = dlg.m_strUser;

		SetEnableManagerConnect(FALSE);

		CSvrTesterApp *pApp = (CSvrTesterApp *)AfxGetApp();
		CTSessionView *pView = pApp->GetTSessionView();
		pView->ManagerConnect(dlg.m_strIP, dlg.m_strPort, dlg.m_strUser, dlg.m_strPass);		
	}
}
// Manager Svr DisConnect
void CMainFrame::OnManagerDisconnect()
{
	CSvrTesterApp *pApp = (CSvrTesterApp *)AfxGetApp();
	CTSessionView *pView = pApp->GetTSessionView();
		
	pView->ManagerDisConnect(); // Manager DisConnect	
		
	m_mapBase.clear();
	m_mapProtocol.clear();
	m_mapType.clear();
	
	m_dlgSideBar.TSessionListClear(); // Session Protocol List Clear
	// Macro Clear
	GetTabGroup()->GetTMacroTabView()->ClearMacroView();
	GetTabGroup()->GetTMacroTabView()->DeleteAllMacroMap();	
	pApp->GetTSessionFrm()->GetTSessionDlg()->ClearMacroTree();
	pApp->GetTSessionFrm()->GetTSessionDlg()->ClearDisregardList();

	SetEnableManagerConnect(TRUE);
}
// Manager Connect / DisConnect Enable
void CMainFrame::SetEnableManagerConnect(BOOL ConnectEnabel)
{
    m_ConnectEnabel = ConnectEnabel;
}
BOOL CMainFrame::GetEnableManagerConnect()
{
    return m_ConnectEnabel;
}
void CMainFrame::OnUpdateManagerConnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_ConnectEnabel);
}
void CMainFrame::OnUpdateManagerDisconnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_ConnectEnabel);
}

// Session / Macro Window View
void CMainFrame::OnWindowSession()
{	
	if(m_dlgSideBar.GetCurTab() == TSESSION)
		return;
	// Session Tab / Session View 표시	
	m_dlgSideBar.TSessionActive();
	SetCheckWindow(TSESSION);
}
void CMainFrame::OnWindowMacro()
{
	if(m_dlgSideBar.GetCurTab() == TMACRO)
		return;
	// Macro Tab / Macro View 표시
	m_dlgSideBar.TMacroActive();	
	SetCheckWindow(TMACRO);
}
// Set Check Window
void CMainFrame::SetCheckWindow(int nID)
{
	switch(nID)
	{
	case TSESSION:
		m_CheckSession = TRUE;
		m_CheckMacro = FALSE;
		break;
	case TMACRO:
        m_CheckSession = FALSE;
		m_CheckMacro = TRUE;
		break;
	}
}
// Check Session / Macro Window View
void CMainFrame::OnUpdateWindowSession(CCmdUI *pCmdUI)
{	
	pCmdUI->SetCheck(m_CheckSession);
}
void CMainFrame::OnUpdateWindowMacro(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_CheckMacro);
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Mecro File New / Save / Load
void CMainFrame::OnFileNewmacro()
{
	GetTabGroup()->GetTMacroTabView()->ClearMacroView();
	GetTabGroup()->GetTMacroTabView()->DeleteAllMacroMap();		
}
void CMainFrame::OnFileMacrosave()
{
	MAPMACRO mMacro = GetTabGroup()->GetTMacroTabView()->m_mapMacro;
	if(mMacro.size() == 0)
	{
		AfxMessageBox(_T("매크로가 비어있습니다."));
		return;
	}

	LPCTSTR lpszFilter = _T("mac파일(*.mac)|*.mac|모든파일|*.*||");

	CFileDialog dlg(FALSE, _T(".mac"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter);
	if(dlg.DoModal() == IDOK)
	{
		CFile file(dlg.GetFileName(), CFile::modeWrite | CFile::modeCreate);
		CArchive ar(&file, CArchive::store);
		Serialize(ar);	
		ar.Close();
		file.Close();	
	}
}
void CMainFrame::OnFileMacroload()
{	
	LPCTSTR lpszFilter = _T("mac파일(*.mac)|*.mac|모든파일|*.*||");
	
	CFileDialog dlg(TRUE, _T(".mac"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter);
	
	if(dlg.DoModal() == IDOK)
	{	        
		CFile file(dlg.GetFileName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
		ar.Close();	
		file.Close();
	}
}
void CMainFrame::OnUpdateFileNewmacro(CCmdUI *pCmdUI)
{	
	pCmdUI->Enable(!m_ConnectEnabel && m_CheckMacro);
}
void CMainFrame::OnUpdateFileMacrosave(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!m_ConnectEnabel && m_CheckMacro);
}
void CMainFrame::OnUpdateFileMacroload(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_ConnectEnabel && m_CheckMacro);	
}
void CMainFrame::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		TRACE(_T("File Save\n"));

		CSvrTesterApp *pApp = (CSvrTesterApp *)AfxGetApp();
		pApp->GetTMacroView()->AllProtocolAnalysis();

		MAPMACRO mMacro = GetTabGroup()->GetTMacroTabView()->m_mapMacro;		

		// Macro Map
        ar << (int)mMacro.size();
		MAPMACRO::iterator it;
		for(it = mMacro.begin(); it != mMacro.end(); it++)
		{
			// Macro
			DWORD mID = (*it).first;
			CTMacro* pMacro = (*it).second;
			ar << mID << pMacro->GetMacroEnable() << pMacro->GetMacroLoop() << pMacro->GetMacroName() << pMacro->GetMacroParent();

			// Macro Paramater
			VECTORPARAM vParam = pMacro->GetMacroParam();
			ar << (int)vParam.size();			
			for(int i = 0; i < (int)vParam.size(); i++)				
				ar << vParam[i].strName << vParam[i].strType << vParam[i].strData;			

			// Macro Protocol Info
			VECTORPINFO vPInfo = pMacro->GetMacroPInfo();
			ar << (int)vPInfo.size();
			for(i = 0; i < (int)vPInfo.size(); i++)
			{
				ar << vPInfo[i].strName << vPInfo[i].strParent << vPInfo[i].sleep << vPInfo[i].bSelf;

				VECTORPARAM vPInfoParam = vPInfo[i].vParam;
				ar << (int)vPInfoParam.size();				 
				for(int j = 0; j < (int)vPInfoParam.size(); j++)				
					ar << vPInfoParam[j].strName << vPInfoParam[j].strType;
			}			
		}
		mMacro.clear();
	}
	else
	{	// loading code
		TRACE(_T("File Load\n"));

		CSvrTesterApp *pApp = (CSvrTesterApp *)AfxGetApp();
		GetTabGroup()->GetTMacroTabView()->ClearMacroView();
		GetTabGroup()->GetTMacroTabView()->DeleteAllMacroMap();		

		// Macro Map
		int MacroCnt;
		ar >> MacroCnt;
		while(MacroCnt--)
		{
			// Macro
			DWORD mID;
			BOOL bEnable;
			BOOL bLoop;
			CString sName, sParent;
			ar >> mID >> bEnable >> bLoop >> sName >> sParent;

			// Macro Paramater
			int ParamCnt;
			ar >> ParamCnt;
			VECTORPARAM vParam;
			while(ParamCnt--)
			{
				STRUCTPARAM sParam;
				ar >> sParam.strName >> sParam.strType >> sParam.strData;
				vParam.push_back(sParam);
			}

			// Macro Protocol Info
			int pInfoMapCnt;
			ar >> pInfoMapCnt;
			VECTORPINFO vPInfo;
			while(pInfoMapCnt--)
			{
				STRUCTPINFO sPInfo;

				ar >> sPInfo.strName >> sPInfo.strParent >> sPInfo.sleep >> sPInfo.bSelf;
				if( sPInfo.sleep )
					sPInfo.bSleep = TRUE;
				else
					sPInfo.bSleep = FALSE;

				sPInfo.dwPID = 0;
				MAPPROTOCOL::iterator it;
				for(it = m_mapProtocol.begin(); it != m_mapProtocol.end(); it++)
				{
					DWORD pID = (*it).first;
					CString strProtocol = (*it).second;
					if( strProtocol == sPInfo.strName )
					{
						sPInfo.dwPID = pID;
						break;
					}
				}

				int pInfoParamCnt;
				ar >> pInfoParamCnt;
				VECTORPARAM vPInfoParam;
				while(pInfoParamCnt--)
				{
					STRUCTPARAM sPInfoParam;
					ar >> sPInfoParam.strName >> sPInfoParam.strType;
					vPInfoParam.push_back(sPInfoParam);
				}
				sPInfo.vParam = vPInfoParam;

				vPInfo.push_back(sPInfo);
			}

			// File Load Macro Map Insert
			CTMacro* pMacro = new CTMacro(bEnable, bLoop, sName, sParent, vParam, vPInfo);
            GetTabGroup()->GetTMacroTabView()->FLoadInsertMacroMap(mID, pMacro);
		}

		// Macro View
		GetTabGroup()->GetTMacroTabView()->MacroView();
	}
}
/////////////////////////////////////////////////////////////////////////////////////