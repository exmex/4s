// TSessionDoc.cpp : CTSessionDoc 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSessionDoc

IMPLEMENT_DYNCREATE(CTSessionDoc, CDocument)

BEGIN_MESSAGE_MAP(CTSessionDoc, CDocument)
END_MESSAGE_MAP()


// CTSessionDoc 생성/소멸

CTSessionDoc::CTSessionDoc()
{	
	WSACleanup();

	m_pSession = new CTachyonSession();
	m_pSession->SetOwner(this);

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		WSACleanup();
		return;	
	}

	CWnd *pWnd = AfxGetMainWnd();
	m_HWND =  pWnd->m_hWnd;	

	m_mapProtocol.clear(); // Protocol Map
	m_mapType.clear(); // Protocol Type Map

	m_bAuthority = 0; // Manager Authority
	m_bManagerConnect = FALSE; // Manager Connect Flag
	m_bRecvMessage = FALSE; // Receive Message Flag

	m_dwThreadNum = 0; // Thread Num	

	m_mapSession.clear(); // Session Map	
	m_mapThreadSession.clear(); // Thread Map

	m_bRun = FALSE; // Thread Run Flag	

	/////////////////////////////
	// Macro
	m_bMacro = FALSE; 
	m_mapSMacro.clear();
	ClearSMacroQueue();
	/////////////////////////////
}

CTSessionDoc::~CTSessionDoc()
{
	m_pSession->End();
	delete m_pSession;

	if( m_mapSession.size() > 0 )
		SessionClose(); // Session Close

	m_mapProtocol.clear(); // Protocol Map
	m_mapType.clear(); // Protocol Type Map

	WSACleanup();

	m_bAuthority = 0; // Manager Authority
	m_bManagerConnect = FALSE; // Manager Connect Flag
	m_bRecvMessage = FALSE; // Receive Message Flag

    /////////////////////////////
	// Macro
	m_bMacro = FALSE;
	if( m_mapSMacro.size() > 0 )
	{
		m_mapSMacro.clear();
		ClearSMacroQueue();
	}
	/////////////////////////////
}

BOOL CTSessionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CTSessionDoc serialization

void CTSessionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTSessionDoc 진단

#ifdef _DEBUG
void CTSessionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTSessionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTSessionDoc 명령

/////////////////////////////////////////////////////////////////////////
// HWND return (TachyonSession)
HWND CTSessionDoc::GetSafeHwnd() 
{
	return m_HWND;
}
// TachyonSession Message
BOOL CTSessionDoc::OnSessionMsg(CTachyonSession *pSession, CPacket *pPacket)
{
	if( !m_bRecvMessage )
	{
		switch(pPacket->GetID())
		{
		ON_RECEIVE(CT_STLOGIN_ACK); // Manager Receive
		}
		return TRUE;
	}
	else
	{
		Receive(pSession, pPacket->GetID(), pPacket); // Protocol Receive
		return TRUE;
	}

	return FALSE;
}
void CTSessionDoc::OnSessionClose(DWORD dwID)
{
	MAPPSESSION::iterator it = m_mapSession.find(dwID);
	if(it==m_mapSession.end())
		return;

	CTachyonSession * pSession = (*it).second;

	::WaitForSingleObject(pSession->m_hThread, INFINITE);
	CloseHandle(pSession->m_hThread);

	TRACE(_T("SESSIONCLOSE END dwID=%d\n"), dwID);

	pSession->ClearMessage();
	m_mapSession.erase(dwID);
	m_mapThreadSession.erase(pSession->m_dwThreadID);

	// Thread Combo box
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();	
	pView->m_comboThread.SetCurSel(0);

	delete pSession;

	if( m_mapSession.empty() )
	{
		CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
		CTSessionView* pView = pApp->GetTSessionView();
        pView->DisConnect();
	}
}

void CTSessionDoc::OnConnect(CTachyonSession *pSession, int nErrorCode)
{
	if( !m_bManagerConnect ) // Manager Connect
	{
		if(nErrorCode)
		{
			AfxMessageBox(_T("서버 접속에 실패했습니다"));

            CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->SetEnableManagerConnect(TRUE);
			pFrm->m_mapBase.clear();
			pFrm->m_mapProtocol.clear();
			pFrm->m_mapType.clear();
			return;
		}	
		SendCT_STLOGIN_REQ(m_strUser, m_strPass);		
	}
	else
	{
		TRACE("OnConnect %d, %d\n", pSession->m_SessionID, nErrorCode);

		if( pSession->m_bLogin )
		{		
			pSession->m_bLogin = FALSE;			
			if(nErrorCode)
			{
				PostSessionClose(pSession);
				return;
			}			

			if( GetMacroFlag() )			
				MacroSend(pSession);

			return;
		}		

		BYTE bComplete = FALSE;

		CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
		CTSessionView* pView = pApp->GetTSessionView();
		
		if(nErrorCode)
			PostSessionClose(pSession);
		else
		{
			if(pView->GetIndexComboThread(pSession->m_SessionID) < 0)
			{
				pView->SetComboThread(pSession->m_SessionID);
				pView->SetEditConnect();
			}
			else
				TRACE("Reconnect Session %d\n",pSession->m_SessionID);
		}		

		// Macro Protocol Send
		if( GetMacroFlag() )
		{		
			pSession->SetMacroParam();
			MacroSend(pSession);
		}		
	}
}
void CTSessionDoc::OnSend(CTachyonSession *pSession, int nErrorCode)
{
}

void CTSessionDoc::OnClose(CTachyonSession *pSession, int nErrorCode)
{
	PostSessionClose(pSession);
}
void CTSessionDoc::PostSessionClose(CTachyonSession * pSession)
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CMainFrame * pMain = (CMainFrame *)pApp->GetMainWnd();

	TRACE(_T("POST CLOSE %d\n"),pSession->m_SessionID);

	pMain->PostMessage(WM_SESSION_CLOSE, pSession->m_SessionID);
}
// Push Receive Data List
void CTSessionDoc::PostPushList(CTachyonSession* pSession, CString strP, CString data)
{
	if( !m_bRun )
		return;

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CMainFrame * pMain = (CMainFrame *)pApp->GetMainWnd();
	
	STRUCTLIST* s = new STRUCTLIST;
	s->pSession = pSession;
	s->strP = strP;
	s->data = data;

	pMain->PostMessage(WM_PUSH_LIST, (WPARAM)s);
}
// CharInfo Show
void CTSessionDoc::PostCharinfoView()
{
	if( !m_bRun )
		return;

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CMainFrame * pMain = (CMainFrame *)pApp->GetMainWnd();
	
	pMain->PostMessage(WM_CHARINFO_VIEW);
}
// Current Session List Show
void CTSessionDoc::PostSessionlistView()
{
	if( !m_bRun )
		return;

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CMainFrame * pMain = (CMainFrame *)pApp->GetMainWnd();
	
	pMain->PostMessage(WM_SESSIONLIST_VIEW);
}
// Manager Say
BYTE CTSessionDoc::Say(CPacket * pPacket) 
{
	return m_pSession->Say(pPacket);
}
// Protocol One Say
void CTSessionDoc::SayToOne(CPacket * pPacket, int nID, DWORD pID, CString strParam) 
{
	MAPPSESSION::iterator finder = m_mapSession.find(nID);
	if(finder==m_mapSession.end())
	{
		delete pPacket;
		return;
	}

	STRUCTTHREAD sThread;
	sThread.type = THREAD_TYPE_SEND;
	sThread.packet = new CPacket();
	sThread.packet->Copy(pPacket);

	CTachyonSession* pSession = (*finder).second;

	MAPPROTOCOL::iterator Pfinder = m_mapProtocol.find(pID);
	if( (*Pfinder).second == _T("CS_CHANNELLIST_REQ") ||
		(*Pfinder).second == _T("CS_CHARLIST_REQ") )
		pSession->SetCharInfo(1, strParam);
	if( (*Pfinder).second == _T("CS_START_REQ") )
		pSession->SetCharInfo(2, strParam);
	
	pSession->PushMessage(sThread);
	
	delete pPacket;
}
// Protocol All Say
void CTSessionDoc::SayToAll(CPacket * pPacket, DWORD pID, CString strParam) 
{
	MAPPSESSION::iterator it;
	for(it = m_mapSession.begin(); it != m_mapSession.end(); it++)
	{
		STRUCTTHREAD sThread;		
		sThread.type = THREAD_TYPE_SEND;
		sThread.packet = new CPacket();
		sThread.packet->Copy(pPacket);		

		CTachyonSession* pSession = (*it).second;		

		MAPPROTOCOL::iterator Pfinder = m_mapProtocol.find(pID);
		if( (*Pfinder).second == _T("CS_CHANNELLIST_REQ") ||
			(*Pfinder).second == _T("CS_CHARLIST_REQ") )
			pSession->SetCharInfo(1, strParam);
		if( (*Pfinder).second == _T("CS_START_REQ") )
			pSession->SetCharInfo(2, strParam);

		pSession->PushMessage(sThread);		
	}
	delete pPacket;
}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// TControlSvr Connect / DisConnect
void CTSessionDoc::CTSvrConnect(CString strIP, DWORD wPort, CString strUser, CString strPass)
{
	m_strUser = strUser;
	m_strPass = strPass;

	m_pSession->Start(strIP, wPort);
}
void CTSessionDoc::CTSvrDisConnect()
{
	m_pSession->End();
	
	DisConnect();

	m_mapProtocol.clear(); // Protocol Map
	m_mapType.clear(); // Protocol Type Map	

	m_bAuthority = 0; // Manager Authority
	m_bManagerConnect = FALSE; // Manager Connect Flag
	m_bRecvMessage = FALSE; // Receive Message Flag
}
// Manager Svr Login Message
void CTSessionDoc::SendCT_STLOGIN_REQ(CString strUser, CString strPass)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_STLOGIN_REQ)
		<< strUser
		<< strPass;

	Say(pMsg);
}
void CTSessionDoc::OnCT_STLOGIN_ACK(CPacket * pPacket)
{
	BYTE bRet;
	BYTE bAuthority;

	(*pPacket) >> bRet
		>> bAuthority;

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();

	if(bRet != ACK_SUCCESS)
	{
		m_bManagerConnect = FALSE;

		if(bRet == ACK_FAILED)		
		{
			AfxMessageBox(_T("서버 접속에 실패했습니다."));

			pFrm->SetEnableManagerConnect(TRUE);

			pFrm->m_mapBase.clear();
			pFrm->m_mapProtocol.clear();
			pFrm->m_mapType.clear();

			m_pSession->End();			
		}		
		return;
	}
	
	AfxMessageBox(_T("로그인했습니다."));

	m_bManagerConnect = TRUE;
	m_bAuthority = bAuthority; // Manager Authority	
	m_bRecvMessage = TRUE; // Receive Message Flag	
	
	pFrm->GetTabGroup()->TSessionTabLoadList();

	m_mapProtocol = pFrm->m_mapProtocol;
	m_mapType = pFrm->m_mapType;

	// Macro View
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->GetTMacroView()->m_mapProtocol = m_mapProtocol;
	pApp->GetTMacroView()->m_mapType = m_mapType;

	m_pSession->End();
}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Sever Connect / DisConnect
void CTSessionDoc::Connect(CString strIP, CString strPort, CString strThread)
{
	if(m_mapSession.size())
	{
		AfxMessageBox(_T("현재 세션이 열려있습니다."));
		return;
	}

	m_bRecvCnt = 0;

	DWORD wPort = atoi(strPort);
	m_dwThreadNum = atoi(strThread);

	SessionCreate();		
	SessionStart(strIP, wPort);	
}
void CTSessionDoc::DisConnect()
{	
	if(m_mapSession.empty())
		return;

	m_bRecvCnt = 0;

	SessionClose();	
	m_dwThreadNum = 0;	

	AfxMessageBox(_T("모든 세션이 닫혔습니다."));
}
/////////////////////////////////////////////////////////////////////////