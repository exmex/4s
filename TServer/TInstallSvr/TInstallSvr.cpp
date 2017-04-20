// TInstallSvr.cpp : WinMain의 구현입니다.

#include "stdafx.h"
#include "TInstallSvr.h"
#include "TInstallSvrModule.h"


CTInstallSvrModule _AtlModule;

extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

CTInstallSvrModule::CTInstallSvrModule()
{
	memset( m_szDBPasswd, 0, ONE_KBYTE);
	memset( m_szDBUserID, 0, ONE_KBYTE);
	memset( m_szDSN, 0, ONE_KBYTE);

	m_wPort = DEF_PATCHPORT;
	m_bNumWorker = 0;

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;
	m_hControl = NULL;
	m_hCheck = NULL;

	m_hIocpControl = NULL;
	m_hIocpCheck = NULL;
	m_hIocpWork = NULL;

	m_mapTSESSION.clear();

	m_accept = INVALID_SOCKET;
	m_bThreadRun = FALSE;
}

CTInstallSvrModule::~CTInstallSvrModule()
{
}

void CTInstallSvrModule::OnERROR( DWORD dwErrorCode)
{
}

DWORD CTInstallSvrModule::_ControlThread( LPVOID lpParam)
{
	CTInstallSvrModule *pModule = (CTInstallSvrModule *) lpParam;
	return pModule->ControlThread();
}

DWORD CTInstallSvrModule::_CheckThread( LPVOID lpParam)
{
	CTInstallSvrModule *pModule = (CTInstallSvrModule *) lpParam;
	return pModule->CheckThread();
}

DWORD CTInstallSvrModule::_WorkThread( LPVOID lpParam)
{
	CTInstallSvrModule *pModule = (CTInstallSvrModule *) lpParam;
	return pModule->WorkThread();
}

HRESULT CTInstallSvrModule::PreMessageLoop( int nShowCmd)
{
	DWORD dwResult = OnEnter();

	if(dwResult)
	{
		OnERROR(dwResult);
		return E_FAIL;
	}
	m_dwThreadID = GetCurrentThreadId();

	return CAtlServiceModuleT<CTInstallSvrModule,IDS_SERVICENAME>::PreMessageLoop(nShowCmd);
}

HRESULT CTInstallSvrModule::PostMessageLoop()
{
	OnExit();
	return CAtlServiceModuleT<CTInstallSvrModule,IDS_SERVICENAME>::PostMessageLoop();
}

DWORD CTInstallSvrModule::OnEnter()
{
	DWORD dwResult = LoadConfig();
	if(dwResult)
		return dwResult;

	dwResult = InitDB(&m_db);
	if(dwResult)
		return dwResult;

	dwResult = LoadData(&m_db);
	if(dwResult)
		return dwResult;

	dwResult = CreateThreads();
	if(dwResult)
		return dwResult;

	dwResult = InitNetwork();
	if(dwResult)
		return dwResult;

	if(!ResumeThreads())
		return EC_INITSERVICE_RESUMETHREAD;

	return 0;
}

void CTInstallSvrModule::OnExit()
{
	if( m_accept != INVALID_SOCKET )
		closesocket(m_accept);

	m_listen.Close();
	if(m_bThreadRun)
		ClearThread();

	map<DWORD, CISession*>::iterator it;
	for(it = m_mapConnect.begin(); it != m_mapConnect.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
	m_mapConnect.clear();

	DeleteCriticalSection(&m_csPC);
	DeleteCriticalSection(&m_csCon);
	UpdateData();
	m_db.Close();
	WSACleanup();
}

DWORD CTInstallSvrModule::LoadConfig()
{
	//Load config from registry
	DWORD dwLength;
	DWORD dwValue;
	HKEY hKey;

	CString strRegKey;
	strRegKey.Empty();

#ifdef _DEBUG
	HKEY hOpenKey = HKEY_CURRENT_USER;
	strRegKey.Format(_T("software\\%s"), m_szServiceName);
#else
	HKEY hOpenKey = HKEY_LOCAL_MACHINE;
	strRegKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\%s\\Config"), m_szServiceName);
#endif

	int nERROR = RegCreateKey( hOpenKey, strRegKey, &hKey);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_OPENREG;

	// Load game database password
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("DBPasswd"),
		NULL,
		NULL,
		(LPBYTE) &m_szDBPasswd,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_PASSWDNOTASSIGNED;

	// Load DB user ID
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("DBUser"),
		NULL,
		NULL,
		(LPBYTE) &m_szDBUserID,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_PASSWDNOTASSIGNED;

	// Load account database DSN
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("DSN"),
		NULL,
		NULL,
		(LPBYTE) &m_szDSN,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_DSNNOTASSIGNED;

	// Load listen port
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("Port"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_PORTNOTASSIGNED;
	else
		m_wPort = (WORD) dwValue;

	return EC_NOERROR;
}

DWORD CTInstallSvrModule::InitDB( CSqlDatabase *pDB)
{
	if(!pDB->Open( m_szDSN, m_szDBUserID, m_szDBPasswd))
		return EC_INITSERVICE_DBOPENFAILED;

	if(!InitQueryTInstallSvr(pDB))
		return EC_INITSERVICE_PREPAREQUERY;

	return EC_NOERROR;
}

DWORD CTInstallSvrModule::LoadData( CSqlDatabase *pDB)
{
	DEFINE_QUERY(pDB, CSPLoadService)
	query->m_bWorld = SVRGRP_NULL;
	query->m_bServiceGroup = SVRGRP_INSTALLFTP;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;

	m_strFTP = query->m_szIP;
	UNDEFINE_QUERY();	

	return EC_NOERROR;
}

DWORD CTInstallSvrModule::CreateThreads()
{
	DWORD dwThread = 0;
	SYSTEM_INFO vINFO;

	m_hControl = CreateThread(
		NULL, 0,
		_ControlThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);

	if(!m_hControl)
		return EC_INITSERVICE_CREATETHREAD;

	m_hCheck = CreateThread(
		NULL, 0, _CheckThread,
		(LPVOID)this,
		CREATE_SUSPENDED,
		&dwThread);

	if(!m_hCheck)
		return EC_INITSERVICE_CREATETHREAD;

	GetSystemInfo(&vINFO);
	m_bNumWorker = (BYTE) (2 * vINFO.dwNumberOfProcessors);

	for( BYTE i=0; i<m_bNumWorker; i++)
	{
		m_hWorker[i] = CreateThread(
			NULL, 0,
			_WorkThread,
			(LPVOID) this,
			CREATE_SUSPENDED,
			&dwThread);

		if(!m_hWorker[i])
			return EC_INITSERVICE_CREATETHREAD;
	}

	return EC_NOERROR;
}

BYTE CTInstallSvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	if( ResumeThread(m_hCheck) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;
	m_bThreadRun = TRUE;

	return TRUE;
}

DWORD CTInstallSvrModule::InitNetwork()
{
	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD( 2, 2);

	int nERROR = WSAStartup( wVersionRequested, &wsaDATA);
	if(nERROR)
		return EC_INITSERVICE_SOCKLIBFAILED;

	if( LOBYTE(wsaDATA.wVersion) != 2 ||
		HIBYTE(wsaDATA.wVersion) != 2 )
		return EC_INITSERVICE_INVALIDSOCKLIB;

	m_hIocpControl = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, 0);

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	m_hIocpWork = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, 0);

	if(!m_hIocpWork)
		return EC_INITSERVICE_CREATEIOCP;

	if(!m_listen.Listen(m_wPort))
		return EC_INITSERVICE_LISTENFAILED;

	m_hIocpControl = CreateIoCompletionPort(
		(HANDLE) m_listen.m_sock,
		m_hIocpControl,
		COMP_ACCEPT, 0);

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	m_hIocpCheck = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL, 0, 0);

	if(!m_hIocpCheck)
		return EC_INITSERVICE_CREATEIOCP;

	if(!WaitForConnect())
		return EC_INITSERVICE_WAITFORCONNECT;

	InitializeCriticalSectionAndSpinCount(&m_csPC, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csCon, 4000);

	return EC_NOERROR;
}

BYTE CTInstallSvrModule::WaitForConnect()
{
	DWORD dwRead = 0;

	if(!CSession::CreateSocket(m_accept))
		return FALSE;

	if(!AcceptEx(
		m_listen.m_sock,
		m_accept,
		m_vAccept.GetBuffer(), 0,
		sizeof(SOCKADDR) + 16,
		sizeof(SOCKADDR) + 16,
		&dwRead,
		(LPOVERLAPPED) &m_ovAccept) &&
		WSAGetLastError() != ERROR_IO_PENDING )
		return FALSE;

	return TRUE;
}

DWORD CTInstallSvrModule::ControlThread()
{
	while(TRUE)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		if(!GetQueuedCompletionStatus(
			m_hIocpControl,
			&dwIoBytes,
			&dwCompKey,
			&pOV, INFINITE) &&
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT	:
				if(!WaitForConnect())
					LogEvent(_T("WaitForConnect : Error"));

				break;
			}
		}
		else if(pOV)
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT	:
				if(!Accept())
					LogEvent(_T("Accept : Error"));

				if(!WaitForConnect())
					LogEvent(_T("WaitForConnect : Error"));

				break;

			case COMP_CLOSE		: OnCloseSession((CTInstallSession *) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}

DWORD CTInstallSvrModule::CheckThread()
{
	CSqlDatabase db;

	DWORD dwResult = InitDB(&db);
	if(dwResult)
	{
		OnERROR(dwResult);
		PostThreadMessage(
			m_dwThreadID,
			WM_QUIT, 0, 0);

		return 0;
	}

	while(TRUE)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		BYTE bRet = GetQueuedCompletionStatus(
			m_hIocpCheck,
			&dwIoBytes,
			&dwCompKey,
			&pOV,
			300000);
		if(!bRet &&	GetLastError() == WAIT_TIMEOUT )
		{
			//	목록 가져오기
			deque<DWORD> qdwIpAddr;
			DEFINE_QUERY(&db, CTBLCheckaddr);
			if(query->Open())
			{
				while(query->Fetch())
					qdwIpAddr.push_back((DWORD)query->m_dwIPaddr);
				query->Close();
			}
			UNDEFINE_QUERY();

			//	Connect 확인
			while(!qdwIpAddr.empty())
			{
				DWORD dwIPAddr = (DWORD)qdwIpAddr.front();
				qdwIpAddr.pop_front();
				ConnectCheck(dwIPAddr, CLIENT_PORT);
			}
		}
		else if(pOV)
		{
			if( COMP_ACCEPT == dwCompKey )
			{
				CISession* pSession = (CISession*)((CONNECTOVER*)pOV)->m_pOBJ;

				DWORD dwByteTransPer, dwFlag;
				BOOL bRet = WSAGetOverlappedResult(pSession->m_sock, &pSession->m_ovConnect, &dwByteTransPer, FALSE, &dwFlag);
				if(!bRet)
					DeleteIP(&db, pSession->GetIP());

				FindSession(pSession->GetIP(), TRUE);
				continue;
			}
		}
		if( COMP_EXIT == dwCompKey )
			return 0;
	}
	return 0;
}
BYTE CTInstallSvrModule::FindSession(DWORD dwIpAddr, BYTE bDel)
{
	SMART_LOCKCS(&m_csCon);
	map<DWORD, CISession*>::iterator it = m_mapConnect.find(dwIpAddr);
	if(it != m_mapConnect.end())
	{
		if(bDel)
		{
			delete it->second;
			m_mapConnect.erase(it);
		}
		return TRUE;
	}
	return FALSE;
}

BYTE CTInstallSvrModule::ConnectCheck(DWORD dwIpAddr, WORD wPort)
{
	if( !m_hIocpCheck )
		return FALSE;

	if(FindSession(dwIpAddr))
		return FALSE;

	CISession* pSession = new CISession;
	if(!pSession->Connect(dwIpAddr, wPort, m_hIocpCheck))
		return FALSE;

	SMART_LOCKCS(&m_csCon);
	m_mapConnect.insert(map<DWORD, CISession*>::value_type(dwIpAddr, pSession));

	return TRUE;	
}

void CTInstallSvrModule::OnCloseSession( CTInstallSession *pSession)
{
	SMART_LOCKCS(&m_csPC);
	MAPTSESSION::iterator itSESSION = m_mapTSESSION.find((DWORD_PTR) pSession);

	if( itSESSION == m_mapTSESSION.end() )
		return;

	m_mapTSESSION.erase(itSESSION);
	delete pSession;
}

BYTE CTInstallSvrModule::Accept()
{
	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	CTInstallSession *pSession = new CTInstallSession();

	pSession->Open( m_accept, m_vAccept);
	pSession->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	HANDLE hIocpWork = CreateIoCompletionPort(
		(HANDLE) pSession->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csPC);

	m_mapTSESSION.insert( MAPTSESSION::value_type( (DWORD_PTR) pSession, pSession));
	pSession->m_dwTick = GetTickCount();

	if( !hIocpWork || !pSession->WaitForMessage() )
	{
		m_mapTSESSION.erase((DWORD_PTR) pSession);
		pSession->Close();
		delete pSession;

		return FALSE;
	}
	
	return TRUE;
}

DWORD CTInstallSvrModule::WorkThread()
{
	CSqlDatabase db;

	DWORD dwResult = InitDB(&db);
	BOOL bRun = TRUE;

	if(dwResult)
	{
		OnERROR(dwResult);
		PostThreadMessage(
			m_dwThreadID,
			WM_QUIT, 0, 0);

		return 0;
	}

	while(bRun)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		if(!GetQueuedCompletionStatus(
			m_hIocpWork,
			&dwIoBytes,
			&dwCompKey,
			&pOV, INFINITE) &&
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_SESSION	:
				if(pOV)
				{
					CTInstallSession *pUser = (CTInstallSession *) ((COverlappedEx *) pOV)->m_pOBJ;
					BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	: OnInvalidSession(pUser); break;
						// ***** IOCP 사용법 중 알아내기 힘든 첫번째 구문 (서버측 세션 종료) *****
						//
						// 서버가 먼저 closesocket()을 호출하여 세션을 종료한 경우이며
						// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
						// 모든 오버랩 오퍼래이션이 종료된 상태이기 때문에
						// 이 소켓 핸들과 관련된 데이타는 IOCP큐에 남아있지 않다.
						// 따라서 이 스레드에서는 해당 세션에 관련된 작업 명령을 더이상 수행하지 않기 때문에
						// 다른 스레드가 허락한다면 이 구문에서 세션 포인터를 삭제해도 무방하다.
						// 세션을 삭제 하는데 가장 좋은 지점이므로 전체 시스템 설계시
						// 정상적인 세션 종료는 서버측에서 먼저 세션을 종료시키도록 설계하는 것이 안전하다.
					case TOV_SSN_SEND:
						OnSendComplete(pUser, 0);
					}
				}

				break;
			}
		}
		else if(pOV)
		{
			BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

			switch(dwCompKey)
			{
			case COMP_SESSION	:
				{
					CTInstallSession *pUser = (CTInstallSession *) ((COverlappedEx *) pOV)->m_pOBJ;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	:
						if(pUser)
							ProcessSession(&db, pUser, dwIoBytes);

						break;

					case TOV_SSN_SEND	:
						if(pUser)
							OnSendComplete( pUser, dwIoBytes);

						break;
					}
				}

				break;
			}
		}
		else
		{
			switch(dwCompKey)
			{
			case COMP_EXIT		:
				{
					PostQueuedCompletionStatus(
						m_hIocpWork, 0,
						COMP_EXIT, NULL);

					bRun = FALSE;
				}

				break;
			}
		}
	}

	db.Close();

	return 0;
}

void CTInstallSvrModule::ClosingSession( CTInstallSession *pUser)
{
	// pUser에 대한 패킷처리가 완료되는 시점을 알림
	// pUser에 대한 오버랩 오퍼래이션이 완료된 것을 확인후 호출 하여야 함.

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pUser);
}

void CTInstallSvrModule::CloseSession( CTInstallSession *pUser)
{
	pUser->CloseSession();
}

void CTInstallSvrModule::OnSendComplete( CTInstallSession *pUser, DWORD dwIoBytes)
{
	if(pUser->SendComplete(dwIoBytes))
		ClosingSession(pUser);
}

void CTInstallSvrModule::ProcessSession( CSqlDatabase *pDB, CTInstallSession *pUser, DWORD dwIoBytes)
{
	BOOL bContinue = TRUE;

	if(!pUser->Read(dwIoBytes))
	{
		// ***** IOCP 사용법 중 알아내기 힘든 두번째 구문 (클라이언트측 세션 종료) *****
		//
		// 클라이언트가 먼저 closesocket()을 호출하여 세션을 종료한 경우이며
		// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
		// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
		// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
		// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
		// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
		// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
		// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
		OnInvalidSession(pUser);
		return;
	}

	while(bContinue)
	{
		DWORD dwResult = pUser->CheckMessage();

		switch(dwResult)
		{
		case PACKET_INCOMPLETE	: bContinue = FALSE; break;
		case PACKET_COMPLETE	:
			{
				DWORD dwResult = OnReceive(pDB, pUser);

				if(dwResult)
				{
					OnInvalidSession(pUser);
					OnERROR(dwResult);

					return;
				}

				pUser->Flush();
			}

			break;

		default					: OnInvalidSession(pUser); return;
			// ***** IOCP 사용법 중 알아내기 힘든 세번째 구문 (비 정상적인 세션 종료) *****
			//
			// 클라이언트가 패킷을 변조해서 보낸다거나 네트웤 오류로 인해 세션이 비 정상적인 상태가 된 경우이며
			// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
			// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
			// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
			// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
			// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
			// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
			// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
			//
			// *** 권장하지 않는 편법 ***
			// 혹시나 여기서 closesocket()을 호출 하여
			// 서버측 세션종료 프로세스로 들어가려는 시도는 하지 않는 것이 좋다.
			// closesocket()을 호출해도 WSARecv()가 호출되지 않은 상태이기 때문에
			// 서버측 세션종료 프로세스로 들어가지 못한다. 만약 WSARecv()를 먼저
			// 호출하고 바로 closesocket()을 호출하면 프로세스로의 진입은 가능 할 수도 있으나
			// 비 정상적인 세션을 대상으로 그런 액션을 하는 것은 위험하다.
		}
	}

	if(!pUser->WaitForMessage())
	{
		// ***** IOCP 사용법 중 알아내기 힘든 네번째 구문 (비 정상적인 세션 종료) *****
		//
		// 네트웤 오류로 인해 세션이 비 정상적인 상태에서 WSARecv()함수 호출이 실패한 경우이며
		// WSARecv()가 호출된 상태에서만 이 코드로 들어오며
		// 모든 오버랩 오퍼래이션이 종료 되었다고 볼 수 없다.
		// 따라서 이후에 이 스레드에서 이 세션과 관련된 작업명령이 실행 될 수 있으므로
		// 여기서 세션 포인터를 삭제하면 서버가 다운될 수 있다.
		// Receive와 관련된 오버랩 오퍼레이션은 확실히 종료 되었으므로
		// Send와 관련된 오버랩 오퍼레이션이 종료되었는지를
		// 확인한 후 다른 스레드의 세션 삭제 수락과정을 거치고 세션을 삭제 해야 한다.
		OnInvalidSession(pUser);
	}
}

void CTInstallSvrModule::OnInvalidSession( CTInstallSession *pUser)
{
	if(pUser->OnInvalidSession())
		ClosingSession(pUser);
}

void CTInstallSvrModule::ClearThread()
{
	PostQueuedCompletionStatus(
		m_hIocpWork, 0,
		COMP_EXIT, NULL);

	WaitForMultipleObjects(
		m_bNumWorker,
		m_hWorker,
		TRUE, INFINITE);

	for( BYTE i=0; i<m_bNumWorker; i++)
		CloseHandle(m_hWorker[i]);

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_EXIT, NULL);

	PostQueuedCompletionStatus(
		m_hIocpCheck, 0,
		COMP_EXIT, NULL);

	WaitForSingleObject( m_hControl, INFINITE);
	CloseHandle(m_hControl);

	WaitForSingleObject( m_hCheck, INFINITE);
	CloseHandle(m_hCheck);

	CloseHandle(m_hIocpControl);
	CloseHandle(m_hIocpWork);
}

void CTInstallSvrModule::UpdateData()
{
	m_mapTSESSION.clear();	
}

DWORD CTInstallSvrModule::OnReceive( CSqlDatabase *pDB, CTInstallSession *pSession)
{
	switch(pSession->m_Recv.GetID())
	{
		ON_RECEIVE(CT_SERVICEMONITOR_ACK)
		ON_RECEIVE(CT_SERVICEDATACLEAR_ACK)
		ON_RECEIVE(CT_CTRLSVR_REQ)
		ON_RECEIVE(CT_INSTALL_REQ)
		ON_RECEIVE(CT_FILEADDR_REQ)
		ON_RECEIVE(CT_INSTALLEND_REQ)
	}

	return EC_SESSION_INVALIDMSG;
}

void CTInstallSvrModule::DeleteIP( CSqlDatabase *pDB,  DWORD dwIPAddr)
{
	DEFINE_QUERY(pDB, CSPTDeleteIP)
		query->m_dwIPaddr = dwIPAddr;
	query->Call();
	UNDEFINE_QUERY()
}
