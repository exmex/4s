// TRelaySvr.cpp : WinMain의 구현입니다.

#include "stdafx.h"
#include "resource.h"
#include "TRelaySvr.h"
#include "TRelaySvrModule.h"


CTRelaySvrModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

CTRelaySvrModule::CTRelaySvrModule()
{
	memset(m_szWorldIP,0,ONE_KBYTE);
	memset(m_szControlIP,0,ONE_KBYTE);	

	m_wWorldPort = DEF_WORLDPORT;
	m_wRelayPort = 0;
	
	m_bNumWorker = 0;
	m_bServerID = 0;
	m_bGroupID = 0;

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;

	m_bThreadRun = FALSE;
	m_bBatchRun = TRUE;

	m_hIocpControl = NULL;
	m_hIocpWork = NULL;

	m_hBatchEvent = NULL;
	m_hControl = NULL;
	m_hBatch = NULL;

	m_accept = INVALID_SOCKET;


	m_mapSESSION.clear();					// all session pool
	m_mapPLAYER.clear();					// valid session pool
	m_mapPLAYERNAME.clear();

	m_mapTGUILD.clear();
	m_mapTPARTY.clear();
	m_vTOPERATOR.clear();
	m_mapTSvrMsg.clear();

#ifdef DEF_UDPLOG
	memset( m_szLogServerIP, 0, ONE_KBYTE);
	m_wLogServerPORT = 0;

	m_pUdpSocket = new CUdpSocket;
#endif

}

CTRelaySvrModule::~CTRelaySvrModule()
{
	#ifdef DEF_UDPLOG
		delete	m_pUdpSocket;
		m_pUdpSocket = NULL;
	#endif
}

void CTRelaySvrModule::OnERROR(DWORD dwErrorCode)
{
}

DWORD CTRelaySvrModule::OnEnter()
{
	while(!m_qBATCHJOB.empty() )
	{
		delete m_qBATCHJOB.front();
		m_qBATCHJOB.pop();
	}

	InitializeCriticalSectionAndSpinCount(&m_csQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csBATCH, 4000);

	m_hBatchEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL );

	DWORD dwResult = LoadConfig();
	if(dwResult)
		return dwResult;

	dwResult = LoadData();
	if(dwResult)
		return dwResult;

	dwResult = CreateThreads();
	if(dwResult)
		return dwResult;

	dwResult = InitNetwork();
	if(dwResult)
		return dwResult;

	if( !ResumeThreads() )
		return EC_INITSERVICE_RESUMETHREAD;

	// Dump
	CTMiniDump::SetOption(MiniDumpWithFullMemory);

	return EC_NOERROR;
}

void CTRelaySvrModule::OnExit()
{
	if( m_accept != INVALID_SOCKET )
		closesocket(m_accept);

	m_listen.Close();

	if(m_bThreadRun)
		ClearThread();

	while(!m_qBATCHJOB.empty())
	{
		delete m_qBATCHJOB.front();
		m_qBATCHJOB.pop();
	}

	UpdateData();

	SetEvent(m_hBatchEvent);
	CloseHandle(m_hBatchEvent);

	DeleteCriticalSection(&m_csQUEUE);
	DeleteCriticalSection(&m_csBATCH);

	m_world.Close();	
	WSACleanup();

}

HRESULT CTRelaySvrModule::PreMessageLoop(int nShowCmd)
{
	DWORD dwResult = OnEnter();

	if(dwResult)
	{
		OnERROR(dwResult);
		return E_FAIL;
	}
	m_dwThreadID = GetCurrentThreadId();

	return CAtlServiceModuleT< CTRelaySvrModule, IDS_SERVICENAME >::PreMessageLoop(nShowCmd);
}
HRESULT CTRelaySvrModule::PostMessageLoop()
{
	OnExit();
	return CAtlServiceModuleT< CTRelaySvrModule, IDS_SERVICENAME >::PostMessageLoop();
}

DWORD CTRelaySvrModule::_ControlThread(LPVOID lpParam)
{
	CTRelaySvrModule* pModule = (CTRelaySvrModule*)lpParam;
	return pModule->ControlThread();
}

DWORD CTRelaySvrModule::_BatchThread(LPVOID lpParam)
{
	CTRelaySvrModule* pModule = (CTRelaySvrModule*)lpParam;
	return pModule->BatchThread();
}

DWORD CTRelaySvrModule::_WorkThread(LPVOID lpParam)
{
	CTRelaySvrModule* pModule = (CTRelaySvrModule*)lpParam;
	return pModule->WorkThread();
}

DWORD CTRelaySvrModule::LoadConfig()
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

	// Load world server IP
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("WorldIP"),
		NULL,
		NULL,
		(LPBYTE) &m_szWorldIP,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_WORLDIPNOTASSIGNED;

	// Load control server IP
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("ControlIP"),
		NULL,
		NULL,
		(LPBYTE) &m_szControlIP,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_WORLDIPNOTASSIGNED;

	// Load group ID
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("GroupID"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_SVRIDNOTASSIGNED;
	else
		m_bGroupID = (BYTE) dwValue;

	// Load server ID
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("ServerID"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_SVRIDNOTASSIGNED;
	else
		m_bServerID = (BYTE) dwValue;

	// Load listen port
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("RelayPort"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_PORTNOTASSIGNED;
	else
		m_wRelayPort = (WORD) dwValue;

	// Load world server port
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("WorldPort"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_WORLDPORTNOTASSIGNED;
	else
		m_wWorldPort = (WORD) dwValue;

#ifdef	DEF_UDPLOG
	
    //	Load LogServer UDP IP
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx( 
		hKey, 
		_T("LogIP"), 
		NULL, 
		NULL, 
		(LPBYTE) &m_szLogServerIP, 
		&dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
		LogEvent(_T("Can't Find LogServer IP"));

		return EC_INITSERVICE_DSNNOTASSIGNED;
	}


	//	Load LogServer UDP PORT
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("LogPort"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
		LogEvent(_T("Can't Find LogServer PORT"));

		return EC_INITSERVICE_PORTNOTASSIGNED;
	}
	else
	{
		m_wLogServerPORT = (WORD) dwValue;
	}   

#endif	DEF_UDPLOG

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::LoadData()
{
	m_addrCtrl.sin_addr.s_addr = inet_addr(m_szControlIP);

	return EC_NOERROR;
}


DWORD CTRelaySvrModule::CreateThreads()
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

	m_hBatch = CreateThread(
		NULL, 0,
		_BatchThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);
	m_bBatchRun = TRUE;

	if(!m_hBatch)
		return EC_INITSERVICE_CREATETHREAD;

	GetSystemInfo(&vINFO);
	m_bNumWorker = (BYTE)(2 * vINFO.dwNumberOfProcessors );

	for( BYTE i = 0; i < m_bNumWorker; i++)
	{
		m_hWorker[i] = CreateThread(
			NULL,0,
			_WorkThread,
			(LPVOID) this,
			CREATE_SUSPENDED,
			&dwThread);

		if( !m_hWorker[i] )
			return EC_INITSERVICE_CREATETHREAD;
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::InitNetwork()
{
	WSADATA wsaDATA;
	WORD wVersionRequested = MAKEWORD(2,2);

	int nERROR = WSAStartup(wVersionRequested,&wsaDATA);
	if(nERROR)
		return EC_INITSERVICE_SOCKLIBFAILED;

	if(LOBYTE(wsaDATA.wVersion) != 2 ||
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

	if(!m_listen.Listen(m_wRelayPort) )
		return EC_INITSERVICE_LISTENFAILED;

	m_hIocpControl = CreateIoCompletionPort(
		(HANDLE)m_listen.m_sock,
		m_hIocpControl,
		COMP_ACCEPT, 0 );

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	if(!WaitForConnect() )
		return EC_INITSERVICE_WAITFORCONNECT;

	m_world.m_Recv.ExpandIoBuffer(RECV_SVR_SIZE);
	m_world.m_bUseCrypt = FALSE;

	if(!m_world.ConnectEx(m_szWorldIP,m_wWorldPort,m_hIocpWork,COMP_SESSION,0 ) )
		return EC_INITSERVICE_CONNECTWORLD;
/*
	m_hIocpWork = CreateIoCompletionPort(
		(HANDLE)m_world.m_sock,
		m_hIocpWork,
		COMP_CONNECT, 0 );

	if(!m_hIocpWork)
		return EC_INITSERVICE_CREATEIOCP;

	if(!m_world.WaitForMessage() )
		return EC_INITSERVICE_WAITFORMSG;

	SendRW_RELAYSVR_REQ();
*/

	//	Initialize UDP
#ifdef DEF_UDPLOG

	if( !m_pUdpSocket->Initialize( m_szLogServerIP, m_wLogServerPORT) )
	{
		LogEvent("Fail then Initialize UDP ");

		return EC_INITSERVICE_UDPSOCKETFAILED;
	}

#endif

	return EC_NOERROR;
}

BYTE CTRelaySvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	if( ResumeThread(m_hBatch) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;
	
	m_bThreadRun = TRUE;

	return TRUE;
}

DWORD CTRelaySvrModule::ControlThread()
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
			&pOV,INFINITE) &&
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT:
				if(!WaitForConnect() )
					LogEvent(_T("WaitForConnect : Error ") );
				break;
			}
		}
		else if(pOV)
		{
			switch(dwCompKey)
			{
			case COMP_ACCEPT :
				if( !Accept() )
					LogEvent( _T("Accept : Error ") );

				if( !WaitForConnect() )
					LogEvent( _T("WaitForConnect : Error ") );
				break;

			case COMP_CLOSE : OnCloseSession( (CTRelaySession*) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}


DWORD CTRelaySvrModule::BatchThread()
{
	while(TRUE)
	{
		EnterCriticalSection(&m_csQUEUE);
		
		if(!m_bBatchRun)
		{
			LeaveCriticalSection(&m_csQUEUE);
			break;
		}

		if( m_qBATCHJOB.empty() )
		{
			LeaveCriticalSection(&m_csQUEUE);
			WaitForSingleObject(m_hBatchEvent,INFINITE);
		}
		else
		{
			LPPACKETBUF pBUF = m_qBATCHJOB.front();
			m_qBATCHJOB.pop();
			LeaveCriticalSection(&m_csQUEUE);

			
			EnterCriticalSection(&m_csBATCH);
			
			DWORD dwResult = OnReceive(pBUF);

			if(dwResult && pBUF->m_pSESSION )
			{
				LogEvent("Batch Close CloseSession %d, %d\n", dwResult, pBUF->m_packet.GetID());
				CloseSession( (CTRelaySession*)pBUF->m_pSESSION );
			}

			LeaveCriticalSection(&m_csBATCH);

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTRelaySvrModule::WorkThread()
{
	BOOL bRun = TRUE;

	while(bRun)
	{
		DWORD dwCompKey = COMP_NULL;
		DWORD dwIoBytes = 0;

		LPOVERLAPPED pOV = NULL;

		BYTE bRet = GetQueuedCompletionStatus(
			m_hIocpWork,
			&dwIoBytes,
			&dwCompKey,
			&pOV,INFINITE);

		if(!bRet && 
			GetLastError() != WAIT_TIMEOUT )
		{
			switch(dwCompKey)
			{
			case COMP_SESSION :
				if(pOV)
				{
					CTRelaySession* pSession = (CTRelaySession*)((COverlappedEx*)pOV)->m_pOBJ;
					BYTE bType = ((COverlappedEx*)pOV)->m_bTYPE;

					switch(bType)
					{
					case TOV_SSN_RECV :
						OnInvalidSession(pSession);
						break;

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
						OnSendComplete(pSession, 0);
						break;
					case TOV_SSN_CONN:
						OnInvalidSession(pSession);
						break;
					}
				}
				break;
			}
		}
		else if(pOV)
		{
			BYTE bTYPE = ((COverlappedEx*)pOV)->m_bTYPE;

			switch(dwCompKey)
			{
			case COMP_SESSION :
				{
					CTRelaySession* pSession = (CTRelaySession*)((COverlappedEx*)pOV)->m_pOBJ;

					switch(bTYPE)
					{
					case TOV_SSN_RECV :
						if(pSession)
							ProcessSession(pSession,dwIoBytes);
						break;
					case TOV_SSN_SEND :
						if(pSession)
							OnSendComplete(pSession,dwIoBytes);
						break;
					case TOV_SSN_CONN:
						{
							if(!pSession->OnConnect())
								OnInvalidSession(pSession);
							else
							{
								if(!pSession->WaitForMessage())
									return EC_INITSERVICE_WAITFORMSG;

								SendRW_RELAYSVR_REQ();
							}
						}
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
			case COMP_EXIT :
				{
					PostQueuedCompletionStatus(
						m_hIocpWork,0,
						COMP_EXIT,NULL);

					bRun = FALSE;
				}
				break;
			}
		}
	}

	return 0;
}

BYTE CTRelaySvrModule::WaitForConnect()
{
	DWORD dwRead = 0;

	if(!CSession::CreateSocket(m_accept) )
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

BYTE CTRelaySvrModule::Accept()
{
	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	CTPlayer* pPlayer = new CTPlayer();

	pPlayer->Open(m_accept, m_vAccept );
	pPlayer->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	if(pPlayer->m_addr.sin_addr.s_addr == m_addrCtrl.sin_addr.s_addr)
		pPlayer->m_bSessionType = SESSION_SERVER;
	else
		pPlayer->m_bUseCrypt = TRUE;

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	HANDLE hIocpWork = CreateIoCompletionPort(
		(HANDLE)pPlayer->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csBATCH);

	m_mapSESSION.insert(MAPPLAYER::value_type( (DWORD_PTR)pPlayer, pPlayer) );

	if( !hIocpWork || !pPlayer->WaitForMessage() )
	{
		m_mapSESSION.erase( (DWORD_PTR)pPlayer);
		pPlayer->Close();
		delete pPlayer;

		return FALSE;
	}

	pPlayer->m_dwAcceptTick = GetTickCount();
	ATLTRACE("SESSION INSERT %s, %d\n", inet_ntoa(pPlayer->m_addr.sin_addr), m_mapSESSION.size() );

	return TRUE;
}


void CTRelaySvrModule::OnInvalidSession(CTRelaySession* pSession)
{
	if(pSession->OnInvalidSession())
		ClosingSession(pSession);
}

void CTRelaySvrModule::OnCloseSession(CTRelaySession* pSession)
{
	if( pSession != &m_world )
	{
		EnterCriticalSection(&m_csBATCH);

		CTPlayer *pPlayer = (CTPlayer *) pSession;
		MAPPLAYER::iterator finder = m_mapSESSION.find((DWORD_PTR) pPlayer);
		if( finder == m_mapSESSION.end() )
		{
			LeaveCriticalSection(&m_csBATCH);
			LogEvent("Session Not Found \n");
			return;
		}

		m_mapSESSION.erase(finder);
		ATLTRACE2("SESSION DELETE %d, %d\n", pPlayer->m_dwID, m_mapSESSION.size());

		CString strFindName = pPlayer->m_strNAME;
		strFindName.MakeUpper();
		MAPPLAYERNAME::iterator itNAME = m_mapPLAYERNAME.find(strFindName);
		if( itNAME != m_mapPLAYERNAME.end() )
			m_mapPLAYERNAME.erase(itNAME);

		finder = m_mapPLAYER.find(pPlayer->m_dwID);
		if( finder != m_mapPLAYER.end() )
		{
			if(pPlayer->m_bReconnect)
				SendRW_RELAYCONNECT_REQ(pPlayer->m_dwID);

			m_mapPLAYER.erase(finder);
		}

		LeaveCriticalSection(&m_csBATCH);

		delete pPlayer;
	}
	else
	{
		LPPACKETBUF pBUF = new PACKETBUF();

		pBUF->m_packet.SetID(SM_QUITSERVICE_REQ);
		SayToBATCH(pBUF);
	}
}

void CTRelaySvrModule::ClosingSession(CTRelaySession* pSession)
{
		// pSession에 대한 패킷처리가 완료되는 시점을 알림
	// pSession에 대한 오버랩 오퍼래이션이 완료된 것을 확인후 호출 하여야 함.

	EnterCriticalSection(&m_csBATCH);

	MAPPLAYER::iterator finder = m_mapSESSION.find((DWORD_PTR) pSession);
	pSession->m_dwCloseTick = GetTickCount();

	if((pSession != &m_world &&
		finder == m_mapSESSION.end()) ||
		pSession->m_bClosing )
	{
		LeaveCriticalSection(&m_csBATCH);
		return;
	}
	
	pSession->m_bClosing = TRUE;
	LeaveCriticalSection(&m_csBATCH);

	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(SM_DELSESSION_REQ);
	pBUF->m_pSESSION = pSession;

	EnterCriticalSection(&m_csQUEUE);
	m_qBATCHJOB.push(pBUF);
	LeaveCriticalSection(&m_csQUEUE);
	SetEvent(m_hBatchEvent);
}

void CTRelaySvrModule::CloseSession(CTRelaySession* pSession)
{
	pSession->m_dwCloseTick = GetTickCount();
	pSession->CloseSession();
}

void CTRelaySvrModule::SayToBATCH(LPPACKETBUF pBUF)
{
	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csQUEUE);
	m_qBATCHJOB.push(pBUF);
	LeaveCriticalSection(&m_csQUEUE);
	SetEvent(m_hBatchEvent);
}

void CTRelaySvrModule::ProcessSession(CTRelaySession* pSession, DWORD dwIoBytes)
{
	BOOL bContinue = TRUE;

	if(!pSession->Read(dwIoBytes))
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
		((CTPlayer *)pSession)->m_bReconnect = FALSE;
		OnInvalidSession(pSession);

		return;
	}

	while(bContinue)
	{
		DWORD dwResult = pSession->CheckMessage();

		switch(dwResult)
		{
		case PACKET_INCOMPLETE	: bContinue = FALSE; break;
		case PACKET_COMPLETE	:
			{
				LPPACKETBUF pBUF = new PACKETBUF();

				pBUF->m_packet.Copy(&pSession->m_Recv);
				pBUF->m_pSESSION = pSession;

				SayToBATCH(pBUF);
				pSession->Flush();
			}

			break;

		default					:
			OnInvalidSession(pSession);

			return;
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

	if(!pSession->WaitForMessage())
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
		if(pSession->m_bSessionType == SESSION_CLIENT)
			LogEvent("WaitForMessage %d, %d", ((CTPlayer *)pSession)->m_dwID, WSAGetLastError());
		OnInvalidSession(pSession);
	}
}

void CTRelaySvrModule::OnSendComplete(CTRelaySession* pSession, DWORD dwIoBytes)
{
	if(pSession->SendComplete(dwIoBytes))
	{
		ClosingSession(pSession);
	}
}

void CTRelaySvrModule::SetEventCloseSession(CTRelaySession* pSession)
{
	if(pSession->m_bSessionType == SESSION_CLIENT)
	{
		CTPlayer *pPlayer = (CTPlayer *)pSession;

		if(pPlayer)
		{			
			CTParty* pParty = FindParty(pPlayer->m_wPartyID);
			if( pParty)
			{
				pParty->DelMember(pPlayer->m_dwID);
				
				CTCorps* pCorps = FindCorps(pParty->m_wCorpsID);
				if(pCorps)
				{
					CTParty* pMemParty = pCorps->FindMember(pPlayer->m_wPartyID);
					if(pMemParty && !pMemParty->GetMemberCount() )
						pCorps->DelMember(pPlayer->m_wPartyID);

					if(!pCorps->GetMemberCount() )
						DelCorps(pCorps->m_wID);
				}

				if(!pParty->GetMemberCount() )
					DelParty(pParty->m_wID);
			}			

			CTGuild* pGuild = FindGuild(pPlayer->m_dwGuildID);
			if(pGuild)
			{
				pGuild->DelMember(pPlayer->m_dwID);
			
				if(pGuild->IsEmpty() )
					DelGuild(pGuild->m_dwID);
			}

			CTGuild* pTactics = FindGuild(pPlayer->m_dwTacticsID);
			if(pTactics)
			{
				pTactics->DelTactics(pPlayer->m_dwID);
			
				if(pTactics->IsEmpty() )
					DelGuild(pTactics->m_dwID);
			}

			if(IsMeetingRoom(pPlayer->m_wMapID, FALSE))
			{
				MAPMEETING::iterator itMt = m_mapMeeting.find(pPlayer->m_wMapID);
				if(itMt!=m_mapMeeting.end())
					itMt->second.erase(pPlayer->m_dwID);
			}

			pPlayer->m_mapTPROTECTED.clear();
		}
	}

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pSession);
}


void CTRelaySvrModule::ClearThread()
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

	WaitForSingleObject( m_hControl, INFINITE);
	CloseHandle(m_hControl);

	EnterCriticalSection(&m_csQUEUE);
	m_bBatchRun = FALSE;
	LeaveCriticalSection(&m_csQUEUE);
	SetEvent(m_hBatchEvent);
	WaitForSingleObject( m_hBatch, INFINITE);

	CloseHandle(m_hIocpControl);
	CloseHandle(m_hIocpWork);
}

void CTRelaySvrModule::UpdateData()
{
	MAPPLAYER::iterator itP;
	MAPTGUILD::iterator itG;
	MAPTPARTY::iterator itR;
	MAPMEMBER::iterator itM;
	MAPPLAYERNAME::iterator itN;
	

	for(itP = m_mapSESSION.begin(); itP != m_mapSESSION.end(); itP++)	
		delete (*itP).second;	
	m_mapSESSION.clear();

	m_mapPLAYER.clear();
	m_mapPLAYERNAME.clear();
	m_mapTSvrMsg.clear();

	for(itG = m_mapTGUILD.begin(); itG != m_mapTGUILD.end(); )
	{		
		for(itM = (*itG).second->m_mapMember.begin(); itM != (*itG).second->m_mapMember.end(); )
			(*itG).second->m_mapMember.erase(itM++);
		m_mapTGUILD.erase(itG++);
	}
	m_mapTGUILD.clear();

	for(itR = m_mapTPARTY.begin(); itR != m_mapTPARTY.end(); )
	{
		for(itM = (*itR).second->m_mapMember.begin(); itM != (*itR).second->m_mapMember.end(); )
			(*itR).second->m_mapMember.erase(itM++);

		m_mapTPARTY.erase(itR++);
	}
	m_mapTPARTY.clear();
}

DWORD CTRelaySvrModule::OnReceive(LPPACKETBUF pBUF)
{
	if(pBUF->m_packet.GetSize() == MAX_PACKET_SIZE)
	{
		LogEvent("Overflow Message %d",pBUF->m_packet.GetID());
		return EC_SESSION_INVALIDMSG;
	}

	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( pPlayer && pPlayer->m_bSessionType == SESSION_CLIENT)
		pPlayer->m_dwCheckTick = GetTickCount();

	WORD wMsgID = pBUF->m_packet.GetID();

	switch(wMsgID)
	{
	// control message
	ON_RECEIVE(CT_SERVICEMONITOR_ACK)
	ON_RECEIVE(CT_ANNOUNCEMENT_ACK)
	ON_RECEIVE(CT_CHATBAN_REQ)
	ON_RECEIVE(CT_CHARMSG_ACK)
	ON_RECEIVE(CT_SERVICEDATACLEAR_ACK)
	ON_RECEIVE(CT_CTRLSVR_REQ)
	ON_RECEIVE(CT_EVENTMSG_REQ)


	// system message 
	ON_RECEIVE(SM_DELSESSION_REQ)
	ON_RECEIVE(SM_QUITSERVICE_REQ)

	// world message
	ON_RECEIVE(RW_ENTERCHAR_ACK)
	ON_RECEIVE(RW_PARTYADD_ACK)
	ON_RECEIVE(RW_PARTYDEL_ACK)
	ON_RECEIVE(RW_PARTYCHGCHIEF_ACK)
	ON_RECEIVE(RW_GUILDADD_ACK)
	ON_RECEIVE(RW_GUILDDEL_ACK)
	ON_RECEIVE(RW_GUILDCHGMASTER_ACK)
	ON_RECEIVE(RW_CORPSJOIN_ACK)
	ON_RECEIVE(RW_RELAYSVR_ACK)
	ON_RECEIVE(RW_CHANGENAME_ACK)
	ON_RECEIVE(RW_TACTICSADD_ACK)
	ON_RECEIVE(RW_TACTICSDEL_ACK)
	ON_RECEIVE(RW_CHATBAN_ACK)
	ON_RECEIVE(RW_CHANGEMAP_ACK)

	// client message
	ON_RECEIVE(CS_RELAYCHARDATA_REQ)
	ON_RECEIVE(CS_CHAT_REQ)	
	ON_RECEIVE(CS_CORPSENEMYLIST_REQ)
	ON_RECEIVE(CS_MOVECORPSENEMY_REQ)
	ON_RECEIVE(CS_MOVECORPSUNIT_REQ)
	ON_RECEIVE(CS_ADDCORPSENEMY_REQ)
	ON_RECEIVE(CS_DELCORPSENEMY_REQ)
	ON_RECEIVE(CS_CORPSHP_REQ)
	ON_RECEIVE(CS_CORPSCMD_REQ)
	ON_RECEIVE(CS_RELAYPROTECTEDLIST_REQ)
	ON_RECEIVE(CS_RELAYPROTECTEDADD_REQ)
	ON_RECEIVE(CS_RELAYPROTECTEDDEL_REQ)
	ON_RECEIVE(CS_CHECKRELAY_REQ)
	ON_RECEIVE(CS_RELAYPROTECTEDOPTION_REQ)

#ifdef	DEF_UDPLOG
	ON_RECEIVE(DM_UDPLOG_REQ)	
#endif
	}
	LogEvent("InValid Msg : %d " ,wMsgID);

	return EC_SESSION_INVALIDMSG;	
}

BYTE CTRelaySvrModule::IsOperator(DWORD dwID)
{
	for(DWORD i = 0; i < m_vTOPERATOR.size(); i++ )
		if( dwID == m_vTOPERATOR[i] )
			return TRUE;

	return FALSE;
}

CTPlayer* CTRelaySvrModule::FindPlayer(DWORD dwID)
{
	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwID);
	if( finder != m_mapPLAYER.end() )
		return (*finder).second;

	return NULL;
}

CTPlayer* CTRelaySvrModule::FindPlayer(CString strName)
{
	strName.MakeUpper();
	MAPPLAYERNAME::iterator finder = m_mapPLAYERNAME.find(strName);
	if( finder != m_mapPLAYERNAME.end() )
		return (*finder).second;

	return NULL;
}

CTGuild* CTRelaySvrModule::FindGuild(DWORD dwID)
{
	MAPTGUILD::iterator itG = m_mapTGUILD.find(dwID);
	if( itG != m_mapTGUILD.end() )
		return (*itG).second;

	return NULL;
}

CTParty* CTRelaySvrModule::FindParty(WORD wID)
{
	MAPTPARTY::iterator itR = m_mapTPARTY.find(wID);
	if( itR != m_mapTPARTY.end() )
		return (*itR).second;

	return NULL;
}

CTCorps* CTRelaySvrModule::FindCorps(WORD wID)
{
	MAPTCORPS::iterator itC = m_mapTCORPS.find(wID);
	if( itC != m_mapTCORPS.end() )
		return (*itC).second;

	return NULL;
}

CTCorps* CTRelaySvrModule::FindCorps(CTPlayer* pPlayer)
{
	CTCorps* pCorps = NULL;
	CTParty* pParty = FindParty(pPlayer->m_wPartyID);
	if(pParty)
	{
		if( !pParty->FindMember(pPlayer->m_dwID) )
			LogEvent("Not Party Member");
		pCorps = FindCorps(pParty->m_wCorpsID);
	}

	return pCorps;
}

void CTRelaySvrModule::AddParty(CTParty* pPARTY)
{
	if(pPARTY)
		m_mapTPARTY.insert(MAPTPARTY::value_type(pPARTY->m_wID, pPARTY) );
}

void CTRelaySvrModule::AddGuild(CTGuild* pGUILD)
{
	if(pGUILD)
		m_mapTGUILD.insert(MAPTGUILD::value_type(pGUILD->m_dwID,pGUILD) );
}

void CTRelaySvrModule::AddCorps(CTCorps* pCORPS)
{
	if(pCORPS)
		m_mapTCORPS.insert(MAPTCORPS::value_type(pCORPS->m_wID,pCORPS) );
}

void CTRelaySvrModule::DelParty(WORD wID)
{
	MAPTPARTY::iterator itR = m_mapTPARTY.find(wID);
	if(itR != m_mapTPARTY.end() )
	{
		CTCorps* pTCORPS = FindCorps((*itR).second->m_wCorpsID);
		if(pTCORPS)
			pTCORPS->DelMember(wID);

		(*itR).second->ResetCorpsID(0);
		delete (*itR).second;
		m_mapTPARTY.erase(itR);
	}
}

void CTRelaySvrModule::DelGuild(DWORD dwID)
{
	MAPTGUILD::iterator itG = m_mapTGUILD.find(dwID);
	if( itG != m_mapTGUILD.end() )
	{
		delete (*itG).second;
		m_mapTGUILD.erase(itG);
	}
}

void CTRelaySvrModule::DelCorps(WORD wID)
{
	MAPTCORPS::iterator itC = m_mapTCORPS.find(wID);
	if( itC != m_mapTCORPS.end() )
	{	
		delete (*itC).second;
		m_mapTCORPS.erase(itC);
	}
}

void CTRelaySvrModule::BroadcastCorps(CTPlayer* pPlayer,CPacket* pPacket, WORD wMsgID)
{
	if( pPlayer->m_wCorpsID &&
		pPlayer->m_dwPartyChiefID == pPlayer->m_dwID)
	{
		CTCorps* pTCORPS = FindCorps(pPlayer);
		if(pTCORPS)
		{
			MAPTPARTY::iterator itParty;
			for(itParty=pTCORPS->m_mapTParty.begin(); itParty!=pTCORPS->m_mapTParty.end(); itParty++)
			{
				if(pPlayer->m_wPartyID == (*itParty).second->m_wID )
					continue;

				CTPlayer* pChief  = FindPlayer( (*itParty).second->m_dwChiefID );
                if(pChief)
				{
					pChief->RelayCorpsMsg(pPacket,wMsgID );
				}
			}
		}
	}
	
	BroadcastCorps(pPlayer->m_wPartyID, pPacket, wMsgID);
}

void CTRelaySvrModule::BroadcastCorps(WORD wPartyID, CPacket* pPacket, WORD wMsgID)
{
	CTParty* pTPARTY = FindParty(wPartyID);
	if(!pTPARTY)
		return;

	MAPMEMBER::iterator itM;
	for(itM = pTPARTY->m_mapMember.begin(); itM != pTPARTY->m_mapMember.end(); itM++)
	{
		CTPlayer* pMember = (*itM).second;
		if(pMember)
			pMember->RelayCorpsMsg(pPacket, wMsgID);
	}
}

void CTRelaySvrModule::ChgGuildMaster(CTGuild* pGuild, DWORD dwGuildMaster)
{
	if(!pGuild || !dwGuildMaster)
		return;

	pGuild->m_dwChiefID = dwGuildMaster;
}


void CTRelaySvrModule::ChgPartyChief(CTParty* pParty, DWORD dwChiefID)
{
	if(!pParty || !dwChiefID )
		return;

	pParty->m_dwChiefID = dwChiefID;

	MAPMEMBER::iterator itM;
	for(itM = pParty->m_mapMember.begin(); itM != pParty->m_mapMember.end(); itM++)
	{
		CTPlayer* pMember = (*itM).second;
		if(pMember)
			pMember->m_dwPartyChiefID = dwChiefID;
	}
}

CString CTRelaySvrModule::GetSvrMsg(DWORD dwID)
{
	MAPTSTRING::iterator it = m_mapTSvrMsg.find(dwID);
	if(it!=m_mapTSvrMsg.end())
		return (*it).second;
	else
		return NAME_NULL;
}

CString CTRelaySvrModule::BuildNetString( const CString& strHeader, const CString& strBody)
{
	CString strSIZE;

	strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), strBody.GetLength());
	return strSIZE + strHeader + strBody;
}

void CTRelaySvrModule::TransferCorpsCommand(CTParty* pParty,
                                            WORD wSquadID,
                                            DWORD dwCharID,
                                            WORD wMapID,
                                            BYTE bCMD,
                                            DWORD dwTargetID,
                                            BYTE bTargetType,
                                            WORD wPosX,
                                            WORD wPosZ)

{
	if(!pParty)
		return;

	MAPMEMBER::iterator itM;
	for(itM = pParty->m_mapMember.begin(); itM != pParty->m_mapMember.end(); itM++)
	{
		CTPlayer* pMember = (*itM).second;
		if(pMember)
			pMember->SendCS_CORPSCMD_ACK(wSquadID,dwCharID,wMapID,bCMD,dwTargetID,bTargetType,wPosX,wPosZ);
	}
}

void CTRelaySvrModule::ClearCharData(CTPlayer * pPlayer)
{
	CTParty* pParty = FindParty(pPlayer->m_wPartyID);
	if( pParty)
	{
		pParty->DelMember(pPlayer->m_dwID);
		
		CTCorps* pCorps = FindCorps(pParty->m_wCorpsID);
		if(pCorps)
		{
			CTParty* pMemParty = pCorps->FindMember(pPlayer->m_wPartyID);
			if(pMemParty && !pMemParty->GetMemberCount())
				pCorps->DelMember(pPlayer->m_wPartyID);

			if(pCorps->m_mapTParty.size() == 0 )
				DelCorps(pCorps->m_wID);
		}

		if(!pParty->GetMemberCount() )
			DelParty(pParty->m_wID);
	}			

	CTGuild* pGuild = FindGuild(pPlayer->m_dwGuildID);
	if(pGuild)
	{
		pGuild->DelMember(pPlayer->m_dwID);
	
		if(pGuild->IsEmpty() )
			DelGuild(pGuild->m_dwID);
	}

	CTGuild * pTactics = FindGuild(pPlayer->m_dwTacticsID);
	if(pTactics)
	{
		pTactics->DelTactics(pPlayer->m_dwID);
	
		if(pTactics->IsEmpty() )
			DelGuild(pTactics->m_dwID);
	}

	if(IsMeetingRoom(pPlayer->m_wMapID, FALSE))
	{
		MAPMEETING::iterator it = m_mapMeeting.find(pPlayer->m_wMapID);
		if(it!=m_mapMeeting.end())
			it->second.erase(pPlayer->m_dwID);
	}

	pPlayer->m_bReconnect = FALSE;
	pPlayer->m_bAuthorized = FALSE;
	pPlayer->m_mapTPROTECTED.clear();
	m_mapPLAYER.erase(pPlayer->m_dwID);

	CString strFindName = pPlayer->m_strNAME;
	strFindName.MakeUpper();
	m_mapPLAYERNAME.erase(strFindName);

	pPlayer->m_dwID = 0;
	pPlayer->m_strNAME = NAME_NULL;
}

BYTE CTRelaySvrModule::IsMeetingRoom(WORD wMapID, BYTE bIsSmall)
{
	BYTE bIn = wMapID >= MEETING_MAPID && wMapID <= MEETING_MAPID + MEETING_SROOM_COUNT;

	if(bIsSmall)
		bIn = bIn && wMapID != MEETING_MAPID;

	return bIn;
}

#ifdef	DEF_UDPLOG

void CTRelaySvrModule::SendDM_UDPLOG_REQ(_LOG_DATA_ pLogData)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_UDPLOG_REQ);


	pBUF->m_packet.Write((LPVOID)&pLogData, sizeof(_LOG_DATA_) ); 
	
	SayToBATCH(pBUF);
}

#endif

