// TControlSvr.cpp : WinMain의 구현입니다.

#include "stdafx.h"
#include "TControlSvr.h"
#include "TControlSvrModule.h"

CTControlSvrModule _AtlModule;


extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


CTControlSvrModule::CTControlSvrModule()
{
	memset( m_szDBPasswd, 0, ONE_KBYTE);
	memset( m_szDBUserID, 0, ONE_KBYTE);
	memset( m_szDSN, 0, ONE_KBYTE);

	m_wPort = DEF_CONTROLPORT;
	m_bNumWorker = 0;

	m_strUploadFileName.Empty();
	m_strUploadFilePath.Empty();
	m_fileTemp = NULL;

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;

	m_bThreadRun = FALSE;
	m_bBatchRun = TRUE;
	m_bSMRun = TRUE;
	m_bTimerRun = TRUE;
	
	m_hIocpControl = NULL;
	m_hIocpWork = NULL;

	m_hBatchEvent = NULL;
	m_hSMEvent = NULL;	
	m_hControl = NULL;
	m_hBatch = NULL;
	m_hSM = NULL;
	m_hTimer = NULL;
	m_hTimerEvent = NULL;
	
	
	m_mapTSVRTEMP.clear();
	m_mapSESSION.clear();
	m_mapMANAGER.clear();

	m_accept = INVALID_SOCKET;

	m_bAutoStart = FALSE;
	m_dwManagerSeq = 0;
	m_dwChatBanSeq = 0;
	m_dwSendCount = 0;
	m_bChatBanSuccess = FALSE;
}

CTControlSvrModule::~CTControlSvrModule()
{
}

void CTControlSvrModule::OnERROR( DWORD dwErrorCode)
{
}

DWORD CTControlSvrModule::_ControlThread( LPVOID lpParam)
{
	CTControlSvrModule *pModule = (CTControlSvrModule *) lpParam;
	return pModule->ControlThread();
}

DWORD CTControlSvrModule::_WorkThread( LPVOID lpParam)
{
	CTControlSvrModule *pModule = (CTControlSvrModule *) lpParam;
	return pModule->WorkThread();
}

DWORD CTControlSvrModule::_BatchThread( LPVOID lpParam)
{
	CTControlSvrModule *pModule = (CTControlSvrModule *) lpParam;
	return pModule->BatchThread();
}

DWORD CTControlSvrModule::_SMThread( LPVOID lpParam)
{
	CTControlSvrModule *pModule = (CTControlSvrModule *) lpParam;
	return pModule->SMThread();
}

DWORD CTControlSvrModule::_TimerThread(LPVOID lpParam)
{
	CTControlSvrModule *pModule = (CTControlSvrModule *) lpParam;
	return pModule->TimerThread();
}

DWORD CTControlSvrModule::OnEnter()
{
	DWORD dwResult = LoadConfig();
	if(dwResult)
		return dwResult;

	dwResult = InitDB();
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

	while(!m_qBATCHJOB.empty())
	{
		delete m_qBATCHJOB.front();
		m_qBATCHJOB.pop();
	}

	while(!m_qSMJOB.empty())
	{
		delete m_qSMJOB.front();
		m_qSMJOB.pop();
	}

	m_mapSESSION.clear();
	m_mapMANAGER.clear();

	if(!ResumeThreads())
		return EC_INITSERVICE_RESUMETHREAD;

	// Dump
	CTMiniDump::SetOption(MiniDumpWithFullMemory);

	return 0;
}

void CTControlSvrModule::OnExit()
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

	while(!m_qSMJOB.empty())
	{
		delete m_qSMJOB.front();
		m_qSMJOB.pop();
	}

	UpdateData();

	SetEvent(m_hBatchEvent);
	CloseHandle(m_hBatchEvent);

	SetEvent(m_hSMEvent);
	CloseHandle(m_hSMEvent);

	SetEvent(m_hTimerEvent);
	CloseHandle(m_hTimerEvent);

	DeleteCriticalSection(&m_csSMQUEUE);
	DeleteCriticalSection(&m_csQUEUE);
	DeleteCriticalSection(&m_csBATCH);

	m_db.Close();
	WSACleanup();
}

DWORD CTControlSvrModule::LoadConfig()
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

	// AutoStartEnable
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(
		hKey,
		_T("AutoStart"),
		NULL,
		NULL,
		NULL,
		NULL);
	if( nERROR == ERROR_SUCCESS )
		m_bAutoStart = TRUE;
	
	return EC_NOERROR;
}

DWORD CTControlSvrModule::CreateThreads()
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

	m_hSM = CreateThread(
		NULL, 0,
		_SMThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);
	m_bSMRun = TRUE;

	if(!m_hSM)
		return EC_INITSERVICE_CREATETHREAD;

	m_hTimer = CreateThread(
		NULL, 0,
		_TimerThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);
	m_bTimerRun = TRUE;

	if(!m_hTimer)
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

BYTE CTControlSvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	if( ResumeThread(m_hBatch) < 0 )
		return FALSE;

	if( ResumeThread(m_hSM) < 0 )
		return FALSE;

	if( ResumeThread(m_hTimer) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;

	m_bThreadRun = TRUE;

	return TRUE;
}

DWORD CTControlSvrModule::InitNetwork()
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

	if(!WaitForConnect())
		return EC_INITSERVICE_WAITFORCONNECT;

	InitializeCriticalSectionAndSpinCount(&m_csSMQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csBATCH, 4000);

	m_hBatchEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hSMEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hTimerEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	return EC_NOERROR;
}

BYTE CTControlSvrModule::WaitForConnect()
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

BYTE CTControlSvrModule::Accept()
{
	// 현승룡 CTManager
	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	CTManager *pMANAGER = new CTManager();

	pMANAGER->Open( m_accept, m_vAccept);
	pMANAGER->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	HANDLE hIocpWork;
	hIocpWork = CreateIoCompletionPort(
		(HANDLE) pMANAGER->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csBATCH);

	m_mapSESSION.insert( MAPTMANAGER::value_type( (DWORD_PTR) pMANAGER, pMANAGER));

	if( !hIocpWork || !pMANAGER->WaitForMessage() )
	{
		m_mapSESSION.erase((DWORD_PTR) pMANAGER);
		pMANAGER->Close();
		delete pMANAGER;
		return FALSE;
	}

	return TRUE;
}

void CTControlSvrModule::ClearThread()
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

	EnterCriticalSection(&m_csSMQUEUE);
	m_bSMRun = FALSE;
	LeaveCriticalSection(&m_csSMQUEUE);
	SetEvent(m_hSMEvent);
	WaitForSingleObject( m_hSM, INFINITE);

	m_bTimerRun = FALSE;
	SetEvent(m_hTimerEvent);
	WaitForSingleObject(m_hTimer,INFINITE);

	CloseHandle(m_hIocpControl);
	CloseHandle(m_hIocpWork);
}

void CTControlSvrModule::UpdateData()
{
	if(m_fileTemp != NULL)
		fclose(m_fileTemp);

	

	MAPTSVRTEMP::iterator itSVRTEMP;
	MAPTMANAGER::iterator itCMANAGER; // 현승룡 CTManager
	MAPTMACHINE::iterator itMACHINE;
	MAPTGROUP::iterator itGROUP;
	MAPTSVRTYPE::iterator itSVRTYPE;
	MAPPLATFORM::iterator itPLATFORM; // 현승룡 CPlatformUsage

	for( itSVRTEMP = m_mapTSVRTEMP.begin(); itSVRTEMP != m_mapTSVRTEMP.end(); itSVRTEMP++)
	{
		if((*itSVRTEMP).second->m_pConn)
			delete (*itSVRTEMP).second->m_pConn;
		delete (*itSVRTEMP).second;
	}
	m_mapTSVRTEMP.clear();

	for( itCMANAGER = m_mapSESSION.begin(); itCMANAGER != m_mapSESSION.end(); itCMANAGER++) // tmanager.h
		delete (*itCMANAGER).second;

	for( itMACHINE = m_mapTMachine.begin(); itMACHINE != m_mapTMachine.end(); itMACHINE++)
		delete (*itMACHINE).second;

	for( itGROUP = m_mapTGroup.begin(); itGROUP != m_mapTGroup.end(); itGROUP++)
		delete (*itGROUP).second;

	for( itSVRTYPE = m_mapTSvrType.begin(); itSVRTYPE != m_mapTSvrType.end(); itSVRTYPE++)
		delete (*itSVRTYPE).second;

	for( itPLATFORM = m_mapPLATFORM.begin(); itPLATFORM != m_mapPLATFORM.end(); itPLATFORM++)
		delete (*itPLATFORM).second;

	m_mapSESSION.clear();
	m_mapTMachine.clear();
	m_mapTGroup.clear();
	m_mapTSvrType.clear();
	m_mapMANAGER.clear();
	m_mapPLATFORM.clear();

	/////////////////////////////////////////////////////////////
	// 현승룡 매니저 권한
	//MAPTMANAGERTEMP::iterator itMANAGER;
	//for( itMANAGER = m_mapMANTEMP.begin(); itMANAGER != m_mapMANTEMP.end(); itMANAGER++)
	//	delete (*itMANAGER).second;
	//m_mapMANTEMP.clear();
	/////////////////////////////////////////////////////////////
}

void CTControlSvrModule::OnInvalidSession( CTControlSession *pSession)
{
	if(pSession->OnInvalidSession())
		ClosingSession(pSession);
}

void CTControlSvrModule::OnCloseSession( CTControlSession *pSession)
{
	BYTE bManager = pSession->m_bManager; // 현승룡 CTManager

	SMART_LOCKCS(&m_csBATCH);
	MAPTMANAGER::iterator finder;

	if(!bManager)
	{
		CTServer *pServer = (CTServer *) pSession;
		LPTSVRTEMP pSvrTemp = FindService(pServer);
		if(pSvrTemp)
		{
			LogEvent("Svr Close %d", pServer->m_dwID);
			pSvrTemp->m_pConn = NULL;
			pSvrTemp->m_flag = FALSE;
		}
	}
	else
	{
		CTManager *pMANAGER = (CTManager *) pSession;
		finder = m_mapMANAGER.find((DWORD_PTR)pMANAGER);

		if(pMANAGER->m_bUpload)
		{
			pMANAGER->m_bUpload = FALSE;
			UploadEnd();
		}

		if( finder != m_mapMANAGER.end() )
		{
			m_mapMANAGER.erase(finder);
		}
	}

	finder = m_mapSESSION.find((DWORD_PTR) pSession);
	if( finder != m_mapSESSION.end() )
		m_mapSESSION.erase(finder);

	delete pSession;
}

void CTControlSvrModule::ClosingSession( CTControlSession *pSession)
{
	// pSession에 대한 패킷처리가 완료되는 시점을 알림
	// pSession에 대한 오버랩 오퍼래이션이 완료된 것을 확인후 호출 하여야 함.
	EnterCriticalSection(&m_csBATCH);
	MAPTMANAGER::iterator finder = m_mapSESSION.find((DWORD_PTR) pSession); // 현승룡 CTManager
	pSession->m_dwCloseTick = GetTickCount();

	if( pSession->m_bManager && 
		finder == m_mapSESSION.end() ||
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

void CTControlSvrModule::CloseSession( CTControlSession *pSession)
{
	pSession->m_dwCloseTick = GetTickCount();
	pSession->CloseSession();
}

DWORD CTControlSvrModule::InitDB()
{
	if(!m_db.Open( m_szDSN, m_szDBUserID, m_szDBPasswd))
		return EC_INITSERVICE_DBOPENFAILED;

	if(!InitQueryTControlSvr(&m_db))
		return EC_INITSERVICE_PREPAREQUERY;

	return EC_NOERROR;
}

DWORD CTControlSvrModule::LoadData()
{
	m_mapTSVRTEMP.clear();

	DEFINE_QUERY(&m_db, CTBLMachine);
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTMACHINE pMachine = new TMACHINE();
			pMachine->m_strNetwork = _T("");
			pMachine->m_bMachineID = query->m_bMachineID;
			pMachine->m_strName = query->m_szName;
			pMachine->m_bRouteID = query->m_bRouteID;
			pMachine->m_bClose = TRUE;			
			m_mapTMachine.insert(MAPTMACHINE::value_type(pMachine->m_bMachineID, pMachine));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLNetwork);
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTMACHINE::iterator find = m_mapTMachine.find(query->m_bMachineID);
			if(find!=m_mapTMachine.end())
				(*find).second->m_strNetwork = query->m_szNetwork;
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLGroup)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTGROUP pGroup = new TGROUP();
			pGroup->m_bGroupID = query->m_bGroupID;
			pGroup->m_strName = query->m_szName;
			m_mapTGroup.insert(MAPTGROUP::value_type(pGroup->m_bGroupID, pGroup));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLSvrType)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTSVRTYPE pType = new TSVRTYPE();
			pType->m_bType = query->m_bType;
			pType->m_strName = query->m_szName;
			m_mapTSvrType.insert(MAPTSVRTYPE::value_type(pType->m_bType, pType));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLServer)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTime t(CTime::GetCurrentTime());			
			LPTSVRTEMP pSvr = new TSVRTEMP();
			pSvr->m_bServerID = query->m_bServerID;
			pSvr->m_wPort = query->m_wPort;
			pSvr->m_strName = query->m_szName;
			pSvr->m_dwStatus = DCSVC_STAT_STOPPED;
			pSvr->m_bManagerControl = FALSE;
			pSvr->m_dwAutoStartTick = 0;
			pSvr->m_nLatestStop = 0;

			MAPTGROUP::iterator itGp = m_mapTGroup.find(query->m_bGroupID);
			if(itGp != m_mapTGroup.end())
				pSvr->m_pGroup = (*itGp).second;
			MAPTMACHINE::iterator itMc = m_mapTMachine.find(query->m_bMachineID);
			if(itMc != m_mapTMachine.end())			
				pSvr->m_pMachine = (*itMc).second;
				
			MAPTSVRTYPE::iterator itTy = m_mapTSvrType.find(query->m_bType);
			if(itTy != m_mapTSvrType.end())
				pSvr->m_pSvrType = (*itTy).second;

			if(itGp == m_mapTGroup.end() ||
				itMc == m_mapTMachine.end() ||
				itTy == m_mapTSvrType.end())
			{
				delete pSvr;
				continue;
			}
		
			pSvr->m_dwID = MAKESVRID(query->m_bGroupID, query->m_bType, query->m_bServerID);
			m_mapTSVRTEMP.insert(MAPTSVRTEMP::value_type(pSvr->m_dwID, pSvr));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	MAPTMACHINE::iterator itMc;
	for(itMc = m_mapTMachine.begin(); itMc != m_mapTMachine.end(); itMc++)
	{
		DEFINE_QUERY(&m_db, CTBLIPAddr);
		query->m_bMachineID = (*itMc).first;
		if(query->Open())
		{
			while(query->Fetch())
			{
				if(lstrcmp(query->m_szIPAddr, _T("")) != 0)
					(*itMc).second->m_vIPAddr.push_back(query->m_szIPAddr);
				if(lstrcmp(query->m_szPriAddr, _T("")) != 0)
					(*itMc).second->m_vPriAddr.push_back(query->m_szPriAddr);
			}
			query->Close();
		}
		UNDEFINE_QUERY();
	}


	//DEFINE_QUERY(&m_db, CTBLManager)
	//if(query->Open())
	//{
	//	while(query->Fetch())
	//	{			
	//		LPTMANAGERTEMP pManager = new TMANAGERTEMP();
	//		pManager->m_strPasswd = query->m_szPasswd;
	//		pManager->m_bAuthority = query->m_bAuthority; 
	//		m_mapMANTEMP.insert(MAPTMANAGERTEMP::value_type(query->m_szID, pManager));
	//	}
	//	query->Close();
	//}
	//UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPLoadService)
	query->m_bWorld = SVRGRP_NULL;
	query->m_bServiceGroup = SVRGRP_CTLSVR;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;
	m_strMyAddr = query->m_szIP;
	UNDEFINE_QUERY();

	OpenPlatform();


	m_dwEventIndex = 0;
	CString strValue;
	DEFINE_QUERY(&m_db, CTBLEvent)
	if(query->Open())
	{
		while(query->Fetch())
		{
			EVENTINFO stEVENTINFO;
			stEVENTINFO.m_dwIndex = query->m_dwIndex;
			stEVENTINFO.m_bID = query->m_bID;
			stEVENTINFO.m_bGroupID = query->m_bGroupID;			
		    stEVENTINFO.m_bSvrID = query->m_bSvrID;
			stEVENTINFO.m_bSvrType = query->m_bSvrType;
			stEVENTINFO.m_dStartDate = __DBTOTIME(query->m_dStartDate);
			stEVENTINFO.m_dEndDate = __DBTOTIME(query->m_dEndDate);
			stEVENTINFO.m_wValue = query->m_wValue;
			stEVENTINFO.m_wMapID = query->m_wMapID;  
			stEVENTINFO.m_dwStartAlarm = query->m_dwStartAlarm;
			stEVENTINFO.m_dwEndAlarm = query->m_dwEndAlarm;
			stEVENTINFO.m_strStartMsg = query->m_szStartMsg;
			//stEVENTINFO.m_strMidMsg = query->m_szMidMsg;  
			stEVENTINFO.m_strEndMsg = query->m_szEndMsg;
			stEVENTINFO.m_strTitle = query->m_szTitle;
			strValue = query->m_szValue;
			stEVENTINFO.m_bStartAlarm = FALSE;
			stEVENTINFO.m_bEndAlarm = FALSE;
			stEVENTINFO.m_bState = FALSE;
			stEVENTINFO.m_bPartTime = query->m_bPartTime;
			ParseStrValue(stEVENTINFO,strValue,stEVENTINFO.m_bID);

			m_mapEVENT.insert(MAPEVENTINFO::value_type(stEVENTINFO.m_dwIndex,stEVENTINFO));

			if(m_dwEventIndex < stEVENTINFO.m_dwIndex)
				m_dwEventIndex = stEVENTINFO.m_dwIndex;
		}
	}
	UNDEFINE_QUERY()


	return EC_NOERROR;
}

void CTControlSvrModule::OpenPlatform()
{
	MAPTMACHINE::iterator it;
	for(it = m_mapTMachine.begin(); it != m_mapTMachine.end(); it++)
	{
		LPTMACHINE pMachine = (*it).second;
		CPlatformUsage * pPlatform = new CPlatformUsage(pMachine->m_bMachineID);

		CString str = _T("\\Network Interface(") + pMachine->m_strNetwork + _T(")\\Bytes Total/sec");
		pPlatform->SetNETREG(str);
		
		
		//SC_HANDLE hscm = OpenSCManager(pMachine->m_vPriAddr.front(), NULL, SC_MANAGER_ALL_ACCESS);
		//if(hscm)
		//	pPlatform->Open(pMachine->m_strName);
		//
		//CloseServiceHandle(hscm);

		m_mapPLATFORM.insert(MAPPLATFORM::value_type(pMachine->m_bMachineID, pPlatform));
	}
}

BYTE CTControlSvrModule::StartService(LPTSVRTEMP pService)
{
	SC_HANDLE hscm = OpenSCManager(pService->m_pMachine->m_vPriAddr.front(),NULL,SC_MANAGER_ALL_ACCESS);
	if(!hscm)
		return FALSE;

	BYTE bResult = TRUE;

	SC_HANDLE hsc = OpenService(hscm, pService->m_strName, SERVICE_ALL_ACCESS);
	if( !hsc || !::StartService(hsc, 0, NULL) )
		bResult = FALSE;
	
	CloseServiceHandle(hsc);
	CloseServiceHandle(hscm);

	return bResult;

}

BYTE CTControlSvrModule::StopService(LPTSVRTEMP pService)
{
	SC_HANDLE hscm = OpenSCManager(pService->m_pMachine->m_vPriAddr.front(),NULL,SC_MANAGER_ALL_ACCESS);
	if(!hscm)
		return FALSE;

	BYTE bResult = TRUE;
	SERVICE_STATUS ss;
	ZeroMemory(&ss, sizeof(SERVICE_STATUS) );

	SC_HANDLE hsc = OpenService( hscm, pService->m_strName, SERVICE_ALL_ACCESS );
	if( !hsc || !::ControlService(hsc, SERVICE_CONTROL_STOP, &ss) )
		bResult = FALSE;

	CloseServiceHandle(hsc);
	CloseServiceHandle(hscm);

	return bResult;

}

void CTControlSvrModule::QueryStatus()
{
	MAPTSVRTEMP::iterator it;
	for(it=m_mapTSVRTEMP.begin(); it!=m_mapTSVRTEMP.end(); it++)
	{
		LPTSVRTEMP pService = (*it).second;

#ifdef _DEBUG

		if( !pService->m_flag )
		{
			LPPACKETBUF pBUF = new PACKETBUF();
			pBUF->m_packet.SetID(CT_NEWCONNECT_REQ)
				<< pService->m_dwID;
			SayToBATCH(pBUF);
			pService->m_flag = TRUE;

			continue;
		}
		else
			continue;
#endif

		SERVICE_STATUS ss;

		ZeroMemory(&ss, sizeof(SERVICE_STATUS));
		ss.dwCurrentState = DCSVC_STAT_CANNOTCONTROL;
		
		SC_HANDLE hscm = 0;

		hscm = OpenSCManager(pService->m_pMachine->m_vPriAddr.front(), NULL,SC_MANAGER_ALL_ACCESS);

		
		if(hscm ) 
		{
			SC_HANDLE hsc = OpenService(hscm, pService->m_strName, SERVICE_ALL_ACCESS);

			if(hsc)
			{				
				QueryServiceStatus(hsc, &ss);

				if(ss.dwCurrentState == DCSVC_STAT_RUNNING &&
					pService->m_dwStatus != DCSVC_STAT_RUNNING)
				{
					//-------------------------------------------------------------------------------------
					// 서비스 변화 때 로그 남기기					
					switch(pService->m_dwStatus)
					{					
					case DCSVC_STAT_STOPPED :			LogEvent("%s Before : DCSVC_STAT_STOPPED", pService->m_strName);		 break;
					case DCSVC_STAT_START_PENDING:		LogEvent("%s Before : DCSVC_STAT_START_PENDING",pService->m_strName);	 break;
					case DCSVC_STAT_STOP_PENDING:		LogEvent("%s Before : DCSVC_STAT_STOP_PENDING",pService->m_strName);	 break;
					case DCSVC_STAT_RUNNING:			LogEvent("%s Before : DCSVC_STAT_RUNNING",pService->m_strName);			 break;
					case DCSVC_STAT_CONTINUE_PENDING:	LogEvent("%s Before : DCSVC_STAT_CONTINUE_PENDING",pService->m_strName); break;
					case DCSVC_STAT_PENDING:			LogEvent("%s Before : DCSVC_STAT_PENDING",pService->m_strName);			 break;
					case DCSVC_STAT_PAUSED:				LogEvent("%s Before : DCSVC_STAT_PAUSED",pService->m_strName);			 break;
					case DCSVC_STAT_CANNOTCONTROL:		LogEvent("%s Before : DCSVC_STAT_CANNOTCONTROL",pService->m_strName);	 break;
					default:							LogEvent("%s Before : Invalid Status",pService->m_strName);
					}					
					//-------------------------------------------------------------------------------------
					
					
					if(pService->m_dwStatus != DCSVC_STAT_CANNOTCONTROL)
					{							
						CTime t(CTime::GetCurrentTime());
						pService->m_nLatestStop = t.GetTime();
						LogEvent("Time Refresh");
					}


					if(pService->m_pSvrType->m_bType != SVRGRP_LOG )
					{
						LPPACKETBUF pBUF = new PACKETBUF();
						pBUF->m_packet.SetID(CT_NEWCONNECT_REQ)
							<< pService->m_dwID;
						SayToBATCH(pBUF);
					}
				
				}
				
				// 서비스가 Stop 일 때 자동으로 Start 시키기.				
				if(ss.dwCurrentState == DCSVC_STAT_STOPPED 
					&& pService->m_dwStatus ==  DCSVC_STAT_STOPPED
					&& m_bAutoStart == TRUE 
					&& pService->m_bManagerControl == TRUE)
				{
					DWORD dwCurTick = GetTickCount();
					if( pService->m_dwAutoStartTick + 60000 < dwCurTick) // 1분후에 시작
					{								
						StartService(pService);			
						pService->m_dwAutoStartTick = dwCurTick;
						LogEvent("QS_Auto Start : %s ", pService->m_strName);
					}
				}

				CloseServiceHandle( hsc );
			}
			
			CloseServiceHandle(hscm);
		}
		
		if(pService->m_dwStatus != ss.dwCurrentState)
		{			
			BYTE bSMS = 0;

			// 서비스가 Stop 됐을 때.
			if(ss.dwCurrentState == DCSVC_STAT_STOPPED)
			{
				LogEvent("QS_Stopped Server : %s", pService->m_strName);
				pService->m_dwStopCount += 1;
				pService->m_dwAutoStartTick = GetTickCount();

				if(pService->m_bManagerControl == TRUE)
					bSMS = 1;
			}
			else if(pService->m_dwStopCount != 0 &&
				pService->m_dwStatus != DCSVC_STAT_CANNOTCONTROL &&
				ss.dwCurrentState == DCSVC_STAT_RUNNING)
			{
				if(pService->m_bManagerControl == TRUE)
					bSMS = 2;				
			}

			if(ss.dwCurrentState == DCSVC_STAT_RUNNING)
				pService->m_bManagerControl = 1;

			pService->m_dwStatus = ss.dwCurrentState;

			LPPACKETBUF pMsg = new PACKETBUF();
			pMsg->m_packet.SetID(CT_SERVICECHANGE_REQ)
				<< pService->m_dwID
				<< pService->m_dwStatus
				<< bSMS
				<< pService->m_pSvrType->m_bType;

			SayToBATCH(pMsg);
		}
	}
}

void CTControlSvrModule::QueryPlatform()
{
	MAPPLATFORM::iterator it;
	for(it=m_mapPLATFORM.begin(); it!=m_mapPLATFORM.end(); it++)
	{
		MAPTMACHINE::iterator itMc = m_mapTMachine.find((*it).second->m_bMachineID);	
		if(itMc == m_mapTMachine.end() )
			continue;

		LPTMACHINE pMach = (*itMc).second;
		SC_HANDLE hscm = OpenSCManager( (*itMc).second->m_vPriAddr.front(), NULL, SC_MANAGER_ALL_ACCESS);
		if(  hscm == NULL) 
			continue;

		CPlatformUsage * pPlatform = (*it).second;
		if(pPlatform->IsOpen())
			pPlatform->QueryPlatformUsage();
		else
		{
			pPlatform->Open((*itMc).second->m_strName);
			continue;
		}

		CloseServiceHandle(hscm);

		// 100 mbps = 100.0
		// 1 gbps = 1000.0
		FLOAT bps = 1000.0;

		LPPACKETBUF pMsg = new PACKETBUF();
		pMsg->m_packet.SetID(CT_PLATFORM_REQ)
			<< pPlatform->m_bMachineID
			<< pPlatform->GetCPU()
			<< pPlatform->GetMEM()
			<< pPlatform->GetNET(bps);
		SayToBATCH(pMsg);
	}
}

LPTSVRTEMP CTControlSvrModule::FindService(BYTE bGroup, BYTE bType, BYTE bID)
{
	MAPTSVRTEMP::iterator it = m_mapTSVRTEMP.find(MAKESVRID(bGroup, bType, bID));
	if(it!=m_mapTSVRTEMP.end())
		return (*it).second;
	else
		return NULL;
}
LPTSVRTEMP CTControlSvrModule::FindService(DWORD dwID)
{
	MAPTSVRTEMP::iterator it = m_mapTSVRTEMP.find(dwID);
	if(it!=m_mapTSVRTEMP.end())
		return (*it).second;
	else
		return NULL;
}

LPTSVRTEMP CTControlSvrModule::FindService(CTServer * pServer)
{
	MAPTSVRTEMP::iterator it;
	for(it=m_mapTSVRTEMP.begin(); it!=m_mapTSVRTEMP.end(); it++)
	{
		if((*it).second->m_pConn == pServer)
			return (*it).second;
	}
	return NULL;
}
void CTControlSvrModule::UploadEnd()
{
	m_strUploadFileName.Empty();
	m_strUploadFilePath.Empty();
	if(m_fileTemp)
	{
		fclose(m_fileTemp);
		m_fileTemp = NULL;
	}
}

DWORD CTControlSvrModule::ControlThread()
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

			case COMP_CLOSE		: OnCloseSession((CTControlSession *) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}

DWORD CTControlSvrModule::WorkThread()
{
	BOOL bRun = TRUE;

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
					CTControlSession *pSession = (CTControlSession *) ((COverlappedEx *) pOV)->m_pOBJ;
					BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	: 
						if(pSession->m_bSessionType == SESSION_SERVER)
						{
							LogEvent("WorkThread CompletionStatus %d %d\n", GetLastError(), BYTE(((CTServer *)pSession)->m_dwID));
						}
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
					CTControlSession *pSession = (CTControlSession *) ((COverlappedEx *) pOV)->m_pOBJ;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	:
						if(pSession)
							ProcessSession( pSession, dwIoBytes);

						break;

					case TOV_SSN_SEND	:
						if(pSession)
							OnSendComplete( pSession, dwIoBytes);

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

	return 0;
}

DWORD CTControlSvrModule::BatchThread()
{
	while(TRUE)
	{
		EnterCriticalSection(&m_csQUEUE);
		if(!m_bBatchRun)
		{
			LeaveCriticalSection(&m_csQUEUE);
			break;
		}

		if(m_qBATCHJOB.empty())
		{
			LeaveCriticalSection(&m_csQUEUE);
			WaitForSingleObject( m_hBatchEvent, INFINITE);
		}
		else
		{
			LPPACKETBUF pBUF = m_qBATCHJOB.front();
			m_qBATCHJOB.pop();
			LeaveCriticalSection(&m_csQUEUE);

			EnterCriticalSection(&m_csBATCH);
			DWORD dwResult = OnReceive(pBUF);
			LeaveCriticalSection(&m_csBATCH);

			if(dwResult)
			{
				CloseSession((CTControlSession *) pBUF->m_pSESSION);
				OnERROR(dwResult);
			}

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTControlSvrModule::SMThread()
{
	while(TRUE)
	{
		EnterCriticalSection(&m_csSMQUEUE);
		if(!m_bSMRun)
		{
			LeaveCriticalSection(&m_csSMQUEUE);
			break;
		}

		if(m_qSMJOB.empty())
		{
			LeaveCriticalSection(&m_csSMQUEUE);
			WaitForSingleObject( m_hSMEvent, CTRL_TIMER);
			QueryStatus();
			//QueryPlatform();
		}
		else
		{
			LPPACKETBUF pBUF = m_qSMJOB.front();
			m_qSMJOB.pop();
			LeaveCriticalSection(&m_csSMQUEUE);

			DWORD dwResult = OnReceive(pBUF);
			if(dwResult)
				OnERROR(dwResult);

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTControlSvrModule::TimerThread()
{
	while(TRUE)
	{
		if(!m_bTimerRun)
			break;
		
		WaitForSingleObject(m_hTimerEvent, CTRL_TIMER);

		LPPACKETBUF pMsgTime = new PACKETBUF();
		pMsgTime->m_packet.SetID(CT_TIMER_REQ);
		SayToBATCH(pMsgTime);		
	}

	return 0;
}

void CTControlSvrModule::OnSendComplete( CTControlSession *pSession, DWORD dwIoBytes)
{
	if(pSession->SendComplete(dwIoBytes))
	{
		if(pSession->m_bSessionType == SESSION_SERVER)
			LogEvent("OnSendComplete Valid %d, %d\n", pSession->m_bValid, dwIoBytes);

		ClosingSession(pSession);
	}
}

void CTControlSvrModule::ProcessSession( CTControlSession *pSession, DWORD dwIoBytes)
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
		LogEvent("Connect Error %d, %d", GetLastError(), dwIoBytes);
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
			LogEvent("2 %d",GetLastError());
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
		LogEvent("3 %d",GetLastError());
		OnInvalidSession(pSession);
	}
}

DWORD CTControlSvrModule::OnReceive( LPPACKETBUF pBUF)
{
	switch(pBUF->m_packet.GetID())
	{
		ON_RECEIVE(SM_DELSESSION_REQ)
		ON_RECEIVE(CT_OPLOGIN_REQ)
		ON_RECEIVE(CT_SERVICESTAT_REQ)
		ON_RECEIVE(CT_SERVICECONTROL_REQ)
		ON_RECEIVE(CT_NEWCONNECT_REQ)
		ON_RECEIVE(CT_SERVICECHANGE_REQ)
		ON_RECEIVE(CT_TIMER_REQ)		
		ON_RECEIVE(CT_SERVICEMONITOR_REQ)
		ON_RECEIVE(CT_SERVICEUPLOADSTART_REQ)
		ON_RECEIVE(CT_SERVICEUPLOAD_REQ)
		ON_RECEIVE(CT_SERVICEUPLOADEND_REQ)
        ON_RECEIVE(CT_UPDATEPATCH_REQ) // 현승룡 CT_UPDATEPATCH_REQ
		ON_RECEIVE(CT_ANNOUNCEMENT_REQ) // 현승룡 CT_ANNOUNCEMENT_REQ
		ON_RECEIVE(CT_USERKICKOUT_REQ) // 현승룡 CT_USERKICKOUT_REQ
		ON_RECEIVE(CT_USERMOVE_REQ) // 현승룡 CT_USERMOVE_REQ
		ON_RECEIVE(CT_USERPOSITION_REQ) // 현승룡 CT_USERPOSITION_REQ
		ON_RECEIVE(CT_MONSPAWNFIND_REQ) // 현승룡 CT_MONSPAWNFIND_REQ
		ON_RECEIVE(CT_MONSPAWNFIND_ACK) // 현승룡 CT_MONSPAWNFIND_ACK
		ON_RECEIVE(CT_MONACTION_REQ) // 현승룡 CT_MONACTION_REQ
		ON_RECEIVE(CT_STLOGIN_REQ) // 현승룡 CT_STLOGIN_REQ
		ON_RECEIVE(CT_PLATFORM_REQ) // 현승룡 CT_PLATFORM_REQ
		ON_RECEIVE(CT_USERPROTECTED_REQ) // 현승룡 CT_USERPROTECTED_REQ
		ON_RECEIVE(CT_CHARMSG_REQ) // 현승룡 CT_CHARMSG_REQ
		ON_RECEIVE(CT_RECONNECT_REQ)
		ON_RECEIVE(CT_SERVICEAUTOSTART_REQ)
		ON_RECEIVE(CT_CHATBAN_REQ)
		ON_RECEIVE(CT_CHATBAN_ACK)
		ON_RECEIVE(CT_SERVICEDATACLEAR_REQ)
		ON_RECEIVE(CT_ITEMFIND_REQ)
		ON_RECEIVE(CT_ITEMSTATE_REQ)
		ON_RECEIVE(CT_ITEMFIND_ACK)
		ON_RECEIVE(CT_ITEMSTATE_ACK)
		ON_RECEIVE(CT_CHATBANLIST_REQ)
		ON_RECEIVE(CT_CHATBANLISTDEL_REQ)
		ON_RECEIVE(CT_CASTLEINFO_REQ)
		ON_RECEIVE(CT_CASTLEINFO_ACK)
		ON_RECEIVE(CT_CASTLEGUILDCHG_REQ)
		ON_RECEIVE(CT_CASTLEGUILDCHG_ACK)
		ON_RECEIVE(CT_CASTLEENABLE_REQ)
		ON_RECEIVE(CT_EVENTUPDATE_REQ)
		ON_RECEIVE(CT_EVENTCHANGE_REQ)
		ON_RECEIVE(CT_EVENTMSG_REQ)
		ON_RECEIVE(CT_EVENTLIST_REQ)
		ON_RECEIVE(CT_EVENTDEL_REQ)
		ON_RECEIVE(CT_CASHSHOPSTOP_REQ)
		ON_RECEIVE(CT_CASHITEMSALE_REQ)
		ON_RECEIVE(CT_CASHITEMLIST_REQ)
		ON_RECEIVE(CT_CASHITEMSALE_ACK)
		ON_RECEIVE(CT_EVENTQUARTERLIST_REQ)  
		ON_RECEIVE(CT_EVENTQUARTERLIST_ACK)
		ON_RECEIVE(CT_EVENTQUARTERUPDATE_REQ)
		ON_RECEIVE(CT_EVENTQUARTERUPDATE_ACK)
		ON_RECEIVE(CT_TOURNAMENTEVENT_REQ)
		ON_RECEIVE(CT_TOURNAMENTEVENT_ACK)
		ON_RECEIVE(CT_HELPMESSAGE_REQ)
		ON_RECEIVE(CT_RPSGAMEDATA_REQ)
		ON_RECEIVE(CT_RPSGAMECHANGE_REQ)
		ON_RECEIVE(CT_RPSGAMEDATA_ACK)
		ON_RECEIVE(CT_PREVERSIONTABLE_REQ)
		ON_RECEIVE(CT_PREVERSIONUPDATE_REQ)

		ON_RECEIVE(CT_CMGIFT_REQ)
		ON_RECEIVE(CT_CMGIFT_ACK)
		ON_RECEIVE(CT_CMGIFTCHARTUPDATE_REQ)
		ON_RECEIVE(CT_CMGIFTLIST_REQ)
		ON_RECEIVE(CT_CMGIFTLIST_ACK)
	}

	LogEvent("Msg Invalid %d",pBUF->m_packet.GetID());
	return EC_SESSION_INVALIDMSG;
}

void CTControlSvrModule::SayToBATCH( LPPACKETBUF pBUF)
{
	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csQUEUE);
	m_qBATCHJOB.push(pBUF);
	LeaveCriticalSection(&m_csQUEUE);
	SetEvent(m_hBatchEvent);
}

void CTControlSvrModule::SayToSM( LPPACKETBUF pBUF)
{
	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csSMQUEUE);
	m_qSMJOB.push(pBUF);
	LeaveCriticalSection(&m_csSMQUEUE);
	SetEvent(m_hSMEvent);
}

HRESULT CTControlSvrModule::PreMessageLoop( int nShowCmd)
{
	DWORD dwResult = OnEnter();

	if(dwResult)
	{
		OnERROR(dwResult);
		return E_FAIL;
	}

	return CAtlServiceModuleT<CTControlSvrModule,IDS_SERVICENAME>::PreMessageLoop(nShowCmd);
}

HRESULT CTControlSvrModule::PostMessageLoop()
{
	OnExit();
	return CAtlServiceModuleT<CTControlSvrModule,IDS_SERVICENAME>::PostMessageLoop();
}

void CTControlSvrModule::SendSvrStatusSMS(BYTE _bSvrType, DWORD _dwSvrID, BYTE _bSvrStatus)
{
	DEFINE_QUERY(&m_db, CSPSvrStatusSMS)
	query->m_bSvrType = _bSvrType;
	query->m_dwSvrID = BYTE(_dwSvrID);
	query->m_bSvrStatus = _bSvrStatus;						
	if(!query->Call())	
		LogEvent("TSvrStatusSMS Error");	
	UNDEFINE_QUERY();
}

CTManager* CTControlSvrModule::FindManager(DWORD dwID)
{
	MAPTMANAGER::iterator itM;
	for(itM = m_mapMANAGER.begin(); itM != m_mapMANAGER.end(); itM++)
	{
		if( (*itM).second->m_dwID == dwID )		
			return (*itM).second;
	}

	return NULL;
}

CTManager* CTControlSvrModule::FindManager(CString strID)
{
	MAPTMANAGER::iterator itM;
	for(itM = m_mapMANAGER.begin(); itM != m_mapMANAGER.end(); itM++)
	{
		if( (*itM).second->m_strID == strID )		
			return (*itM).second;
	}

	return NULL;
}

void CTControlSvrModule::CheckEvent()
{
	CTime curtime(CTime::GetCurrentTime().GetTime());

	MAPEVENTINFO::iterator it;
	for( it = m_mapEVENT.begin(); it != m_mapEVENT.end(); it++)
	{
		if( (*it).second.m_dEndDate < curtime.GetTime() && !(*it).second.m_bState )
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(CT_EVENTDEL_REQ)
				<< (*it).second.m_dwIndex;
			SayToBATCH(pMSG);
			continue;
		}

		BYTE bTmp = 0;	
		BYTE bPartTime = (*it).second.m_bPartTime;

		DWORD dwCurTime = (curtime.GetHour() * 60) + curtime.GetMinute() + 1440; // 1440 = 24 * 60
		CTime tStart((*it).second.m_dStartDate);
		CTime tEnd( (*it).second.m_dEndDate);	
		CTime tEventStart	= tStart -  CTimeSpan(0,0,(*it).second.m_dwStartAlarm,0);
		CTime tEventEnd		= tEnd	 -  CTimeSpan(0,0,(*it).second.m_dwEndAlarm,0);	
		
		WORD wValue;

		if( curtime.GetTime() < tEventStart.GetTime() )
			continue;

		if(!bPartTime)	//	0:Dayily Event, 1:Term Event
		{			
			DWORD dwStartTime		= ( tStart.GetHour() * 60 ) + tStart.GetMinute() + 1440;
			DWORD dwEndTime			= ( tEnd.GetHour()   * 60 ) + tEnd.GetMinute()   + 1440;
			DWORD dwStartAlarmTime	= dwStartTime - (*it).second.m_dwStartAlarm;			
			DWORD dwEndAlarmTIme	= dwEndTime	 - (*it).second.m_dwEndAlarm;				

			if( dwCurTime < dwStartAlarmTime )
				continue;
			
			if( !(*it).second.m_bState )
			{
				if( !(*it).second.m_bStartAlarm && dwStartAlarmTime <= dwCurTime && dwCurTime < dwEndTime )
				{
					bTmp = 1;
					(*it).second.m_bStartAlarm = TRUE;
				}
				else if( dwStartTime <= dwCurTime && dwCurTime < dwEndTime )				
				{
					bTmp = 3;
					wValue = (*it).second.m_wValue;
					(*it).second.m_bState = TRUE;
				}
			}
			else if( (*it).second.m_bState == 1)
			{
				if( !(*it).second.m_bEndAlarm && dwEndAlarmTIme <= dwCurTime && dwCurTime <= dwEndTime )
				{
					bTmp = 2;
					(*it).second.m_bEndAlarm = TRUE;
				}
				else if ( dwEndTime <= dwCurTime)				
				{
					bTmp = 4;				
					wValue = 0;
					(*it).second.m_bState = FALSE;
					(*it).second.m_bStartAlarm = FALSE;
					(*it).second.m_bEndAlarm = FALSE;
				}
			}
		}
		else
		{
			if( !(*it).second.m_bState)
			{
				if( !(*it).second.m_bStartAlarm && tEventStart.GetTime() <= curtime.GetTime() && curtime.GetTime() < tEnd.GetTime() )
				{
					bTmp = 1;
					(*it).second.m_bStartAlarm = TRUE;
				}
				else if( tStart.GetTime() <= curtime.GetTime() && curtime.GetTime() < tEnd.GetTime() )
				{
					bTmp = 3;
					wValue = (*it).second.m_wValue;
					(*it).second.m_bState = TRUE;
				}
			}
			else if( (*it).second.m_bState == 1)
			{
				if( !(*it).second.m_bEndAlarm && tEventEnd.GetTime() <= curtime.GetTime() && curtime.GetTime() <= tEnd.GetTime() )
				{
					bTmp = 2;
					(*it).second.m_bEndAlarm = TRUE;
				}
				else if ( tEnd.GetTime() <= curtime.GetTime())
				{
					bTmp = 4;				
					wValue = 0;
					(*it).second.m_bState = FALSE;
					(*it).second.m_bStartAlarm = FALSE;
					(*it).second.m_bEndAlarm = FALSE;
				}
			}
		}

		switch(bTmp)
		{
		case 1:
		case 2:
			{
				LPPACKETBUF pMSG = new PACKETBUF();
				pMSG->m_packet.SetID(CT_EVENTMSG_REQ)
					<< (*it).second.m_dwIndex
					<< BYTE(bTmp - 1);
				SayToBATCH(pMSG);

				if( (*it).second.m_bID == EVENT_CASHSALE)
				{
					LPPACKETBUF pMSG2 = new PACKETBUF();
					pMSG2->m_packet.SetID(CT_CASHSHOPSTOP_REQ)
						<< (*it).second.m_dwIndex
						<< BYTE(bTmp - 1);

					SayToBATCH(pMSG2);
				}
				break;
			}

		case 3:
		case 4:
			{
				LPPACKETBUF pMSG = new PACKETBUF();

				if( (*it).second.m_bID == EVENT_CASHSALE)
					pMSG->m_packet.SetID(CT_CASHITEMSALE_REQ);
				else
					pMSG->m_packet.SetID(CT_EVENTUPDATE_REQ);

				pMSG->m_packet
						<< (*it).second.m_dwIndex
						<< wValue;

				SayToBATCH(pMSG);

				if( (*it).second.m_bID == EVENT_LOTTERY || (*it).second.m_bID == EVENT_GIFTTIME)
				{
					LPPACKETBUF pMSG2 = new PACKETBUF();
					pMSG2->m_packet.SetID(CT_EVENTDEL_REQ)
						<< (*it).second.m_dwIndex;

					SayToBATCH(pMSG2);
				}
				break;
			}
		}		
	}
}


void CTControlSvrModule::SendEventToNewConnect(LPTSVRTEMP pService)
{
	BYTE bSvrType = pService->m_pSvrType->m_bType;
	if( !pService || 
		!pService->m_pConn || 
		(bSvrType != SVRGRP_LOGINSVR && bSvrType != SVRGRP_MAPSVR && bSvrType != SVRGRP_WORLDSVR) )
		return;

	MAPEVENTINFO::iterator it;
	for( it = m_mapEVENT.begin(); it != m_mapEVENT.end(); it++)
	{	
		if(!(*it).second.m_bState )
			continue;

		if( pService->m_pConn &&
				pService->m_pSvrType->m_bType == (*it).second.m_bSvrType && 
				(!(*it).second.m_bGroupID || pService->m_pGroup->m_bGroupID == (*it).second.m_bGroupID) &&
				(!(*it).second.m_bSvrID || pService->m_bServerID == (*it).second.m_bSvrID) )
			{
				if( (*it).second.m_bID != EVENT_CASHSALE)
					pService->m_pConn->SendCT_EVENTUPDATE_REQ( (*it).second.m_bID, (*it).second.m_wValue,&((*it).second));
				else
					pService->m_pConn->SendCT_CASHITEMSALE_REQ( (*it).second.m_dwIndex, (*it).second.m_wValue, &((*it).second.m_vCashItem) );				
			}
	}
}

void CTControlSvrModule::MakeStrValue(EVENTINFO& stEVENTINFO,CString& str,BYTE bEventID)
{
	 
	WORD i = 0;
	CString strVal1 =_T("");
	CString strVal2 =_T("");
	CString strVal3 =_T("");
	CString strVal4 =_T("");
	CString strVal5 =_T("");
	CString strVal6 =_T("");
	CString strResult =_T("");

	switch(bEventID)
	{
	case EVENT_CASHSALE:
		for(i = 0; i < (WORD)stEVENTINFO.m_vCashItem.size(); i++)
		{
			strVal1.Format("%d", stEVENTINFO.m_vCashItem[i].m_wID);
			strVal2.Format("%d", stEVENTINFO.m_vCashItem[i].m_bSaleValue);

			strVal3 = strVal1 + _T("-") + strVal2 + _T(";");

			str = str + strVal3;		
		}
		break;

	case EVENT_MONSPAWN:
		for(i = 0; i < (WORD)stEVENTINFO.m_stMONEVENT.m_vSpawnID.size(); i++)
		{
			strVal1.Format("%d",stEVENTINFO.m_stMONEVENT.m_vSpawnID[i]);
			strResult = strVal1 + _T(";");
			str = str +strResult;
		}
		break;

	case EVENT_MONREGEN:
		for( i = 0; i < (WORD)stEVENTINFO.m_vMONREGEN.size(); i++)
		{
			strVal1.Format("%d",stEVENTINFO.m_vMONREGEN[i].m_wMonID);
			strVal2.Format("%d",stEVENTINFO.m_vMONREGEN[i].m_dwDelay);
			strVal3.Format("%d",stEVENTINFO.m_vMONREGEN[i].m_wMapID);
			strVal4.Format("%.2f",stEVENTINFO.m_vMONREGEN[i].m_fPosX);
			strVal5.Format("%.2f",stEVENTINFO.m_vMONREGEN[i].m_fPosY);
			strVal6.Format("%.2f",stEVENTINFO.m_vMONREGEN[i].m_fPosZ);

			strResult = strVal1 + _T("-") + strVal2 + _T("-") + strVal3 + _T("-") + strVal4 + _T("-") +strVal5 + _T("-") + strVal6 + _T(";");
			str = str + strResult;
		}
		break;

	case EVENT_LOTTERY:	
		for( i = 0; i < (WORD)stEVENTINFO.m_vLOTTERY.size(); i++)
		{
			strVal2.Format("%d", stEVENTINFO.m_vLOTTERY[i].m_wItemID);
			strVal3.Format("%d", stEVENTINFO.m_vLOTTERY[i].m_bNum);
			strVal4.Format("%d", stEVENTINFO.m_vLOTTERY[i].m_wWinner);

			str = str + strVal2 + _T("x") + strVal3 + _T("-") + strVal4 + _T(";");
		}
		str = str + _T("|") + stEVENTINFO.m_strLotMsg;
		break;
	case EVENT_GIFTTIME:
		str = stEVENTINFO.m_strLotMsg;
		break;
	}
}

void CTControlSvrModule::ParseStrValue(EVENTINFO& stEVENTINFO,CString str,BYTE bEventID)
{
	int nPos = 0;
	int nPos2 = 0;
	int nV1,nV2,nLength;

	BYTE bCount = 0;

	CString strTok;  
	CString strTok2;
	CString str1;
	CString str2;
	CString str3;

	if(bEventID == EVENT_CASHSALE)
	{
		strTok = str.Tokenize(_T(";"),nPos);
		while(strTok != _T(""))
		{
			nLength = strTok.GetLength();
			nV1 = strTok.Find(_T("-"));
			str1 = strTok.Left(nV1);
			str2 = strTok.Right(nLength - nV1-1);

			TCASHITEMSALE stSALE;
			stSALE.m_wID = atoi(str1);
			stSALE.m_bSaleValue = atoi(str2);

			stEVENTINFO.m_vCashItem.push_back(stSALE);

			strTok = str.Tokenize(_T(";"),nPos);
		}
	}
	else if( bEventID == EVENT_MONSPAWN)
	{
		strTok = str.Tokenize(_T(";"),nPos);
		WORD wSpawnID;
		while(strTok != _T(""))
		{
			wSpawnID = atoi(strTok);

			stEVENTINFO.m_stMONEVENT.m_vSpawnID.push_back(wSpawnID);

			strTok = str.Tokenize(_T(";"),nPos);			
		}
	}
	else if( bEventID == EVENT_MONREGEN)  
	{		
		strTok = str.Tokenize(_T(";"),nPos);
		while(strTok != _T("") )
		{
			bCount = 0;
			nPos2 = 0;
			MONREGEN stMONREGEN;

			strTok2 = strTok.Tokenize(_T("-"),nPos2);
			while(strTok2 != _T(""))
			{
				switch(bCount)
				{
				case 0:	stMONREGEN.m_wMonID = atoi(strTok2);	break;
				case 1: stMONREGEN.m_dwDelay = atoi(strTok2);	break;
				case 2: stMONREGEN.m_wMapID = atoi(strTok2);	break;
				case 3:	stMONREGEN.m_fPosX = (FLOAT)atof(strTok2);		break;
				case 4: stMONREGEN.m_fPosY = (FLOAT)atof(strTok2);		break;
				case 5: stMONREGEN.m_fPosZ = (FLOAT)atof(strTok2);		break;
				}

				bCount++;
				strTok2 = strTok.Tokenize(_T("-"),nPos2);
			}
			stEVENTINFO.m_vMONREGEN.push_back(stMONREGEN);

			strTok = str.Tokenize(_T(";"),nPos);
		}
	}
	else if( bEventID == EVENT_LOTTERY)
	{
		//	reword(IDxNUM-win;)'|'title'|'message
		//	ex) 1203x1-10;55x1-1;|당첨|축하드립니다!

		//	보상
		nPos2 = 0;
		strTok = str.Tokenize(_T("|"), nPos);
		strTok2 = strTok.Tokenize(_T(";"), nPos2);
		while(strTok2 != _T(""))
		{
			TLOTTERY tLot;
			nV1 = 0;
			nV2 = 0;

			nLength = strTok2.GetLength();
			nV1 = strTok.Find(_T("x"));
			str1 = strTok.Left(nV1);
			nV2 = strTok.Find(_T("-"));
			str2 = strTok.Mid(nV1+1, nV2-nV1-1);
			str3 = strTok.Mid(nV2+1, nLength-nV2-1);
			
			tLot.m_wItemID	= atoi(str1);
			tLot.m_bNum		= atoi(str2);
			tLot.m_wWinner	= atoi(str3);

			stEVENTINFO.m_vLOTTERY.push_back(tLot);

			strTok2 = strTok.Tokenize(_T(";"), nPos2);
		}
		stEVENTINFO.m_strLotMsg = str.Right(str.GetLength() - nPos);
	}
}