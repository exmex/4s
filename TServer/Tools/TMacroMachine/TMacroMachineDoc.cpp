// TMacroMachineDoc.cpp : CTMacroMachineDoc 클래스의 구현
//

#include "stdafx.h"
#include "TMacroMachine.h"

#include "TMacroMachineDoc.h"
#include "TMacroMachineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMacroMachineDoc

IMPLEMENT_DYNCREATE(CTMacroMachineDoc, CDocument)

BEGIN_MESSAGE_MAP(CTMacroMachineDoc, CDocument)
END_MESSAGE_MAP()


// CTMacroMachineDoc 생성/소멸
CTMacroMachineView * g_pView = NULL;

CTMacroMachineDoc::CTMacroMachineDoc()
{	
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	WSACleanup();

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		WSACleanup();
		return;	
	}

	m_bRun = FALSE;

	m_mapTVALUE.clear();

	while(m_qTMACRO.size())
		m_qTMACRO.pop();
	
	MAPTPROTOCOLTEMP::iterator it;
	for(it = m_mapTProtocol.begin(); it != m_mapTProtocol.end(); it++)
		delete (*it).second;
	m_mapTProtocol.clear();

	m_wThread = 0;
	m_wSessionIndex = 0;
	m_bRelayChatting = 0;
	InitializeCriticalSection(&m_cs);

}

CTMacroMachineDoc::~CTMacroMachineDoc()
{
	WSACleanup();
	m_mapProtocol.clear();
	m_mapType.clear();
	if(m_mapTSession.size())
		EndThread();

	MAPTPROTOCOLTEMP::iterator it;
	for(it = m_mapTProtocol.begin(); it != m_mapTProtocol.end(); it++)
		delete (*it).second;
	m_mapTProtocol.clear();

	DeleteCriticalSection(&m_cs);
}

BOOL CTMacroMachineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CTMacroMachineDoc 진단

#ifdef _DEBUG
void CTMacroMachineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTMacroMachineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTMacroMachineDoc 명령
HWND CTMacroMachineDoc::GetSafeHwnd() 
{
	return m_HWND;
}

CMainFrame * CTMacroMachineDoc::GetMain()
{
	CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
	return pMain;
}


BOOL CTMacroMachineDoc::OnSessionMsg(CTachyonSession * pSession, CPacket * pPacket)
{
	Receive(pSession, pPacket);
	return TRUE;
}


void CTMacroMachineDoc::OnConnect(CTachyonSession *pSession, int nErrorCode)
{
	if(nErrorCode)
	{
		TRACE(_T("OnConnect Error: %d, Session: %d %d\n"),nErrorCode, pSession->m_wID);

		GetMain()->PostMessage(WM_SESSION_CLOSE, pSession->m_sock, nErrorCode);
		if(g_pView)
			g_pView->ViewSessionCnt(FALSE);
		return;
	}

	TRACE(_T("SESSION CONNECT %d\n"), pSession->m_wID);
	if(!pSession->EmptyMacro())
	{
		pSession->PushTType(nErrorCode, TYPE_START);
		return;
	}

	CTMacroMachineView * pView = (CTMacroMachineView*)GetMain()->GetActiveView();
	g_pView = (CTMacroMachineView*)GetMain()->GetActiveView();
	pView->ViewSessionCnt(TRUE);
}

void CTMacroMachineDoc::OnClose(CTachyonSession *pSession, int nErrorCode)
{
	TRACE(_T("SESSION CLOSE. Sesson: %d. Error: %d\n"), pSession->m_wID,nErrorCode);

	MAPTSESSION::iterator find = m_mapTSession.find(pSession->m_wID);
	if(find!=m_mapTSession.end())
	{
		(*find).second->m_bRun = FALSE;
		SetEvent((*find).second->m_hEvent);
		SetEvent((*find).second->m_hSleep);
		if(WaitForSingleObject((*find).second->m_hThread, TIME_WAIT) == WAIT_TIMEOUT)
		{
			DWORD dwExitCode = 0;
			TerminateThread((*find).second->m_hThread, dwExitCode);
			TRACE(_T("TERMINATE THREAD\n"));
			CloseHandle((*find).second->m_hSleep);
			CloseHandle((*find).second->m_hEvent);
		}
		CloseHandle((*find).second->m_hThread);

		delete (*find).second;
		EnterCriticalSection(&m_cs);
		m_mapTSession.erase(find);
		LeaveCriticalSection(&m_cs);
	}

	m_wThread--;

	/*
	CTMacroMachineView * pView = (CTMacroMachineView*)GetMain()->GetActiveView();
	pView->ViewSessionCnt(FALSE);
	*/
	if(g_pView)
        g_pView->ViewSessionCnt(FALSE);

	if(!m_mapTSession.size())
		EndThread();
}

BOOL CTMacroMachineDoc::EmptyMacro()
{
	if(m_qTMACRO.empty())
		return TRUE;

	return FALSE;
}

void CTMacroMachineDoc::ExcuteMacro()
{
	MAPTSESSION::iterator it;
	for(it=m_mapTSession.begin(); it!=m_mapTSession.end(); it++)
		(*it).second->SetQueue(m_qTMACRO, m_mapTVALUE);

	for(it=m_mapTSession.begin(); it!=m_mapTSession.end(); it++)
	{
		(*it).second->m_bRelayEnable = m_bRelayChatting;
		(*it).second->ExcuteMacro();
		(*it).second->PushTType(0, TYPE_START);
	}
}

void CTMacroMachineDoc::UpdateSessionCnt()
{
	//CTMacroMachineView * pView = (CTMacroMachineView*)GetMain()->GetActiveView();	
	if(g_pView)	
		g_pView->ViewSessionCnt(FALSE);	
}