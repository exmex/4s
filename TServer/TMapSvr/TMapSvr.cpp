// TMapSvr.cpp : WinMain의 구현입니다.
#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CTMapSvrModule _AtlModule;


extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

CTMapSvrModule::CTMapSvrModule()
{
	memset( m_szGamePasswd, 0, ONE_KBYTE);
	memset( m_szDBUserID, 0, ONE_KBYTE);
	memset( m_szWorldIP, 0, ONE_KBYTE);
	memset( m_szGameDSN, 0, ONE_KBYTE);
	memset( m_dwWarCountryBalance, 0, sizeof(m_dwWarCountryBalance));

	m_wWorldPort = DEF_WORLDPORT;
	m_wGamePort = DEF_MAPPORT;

	m_bNumWorker = 0;
	m_bServerID = 0;
	m_bGroupID = 0;
	m_bRelayOn = FALSE;

	for( int i=0; i<MAX_THREAD; i++)
		m_hWorker[i] = NULL;

	m_bThreadRun = FALSE;
	m_bBatchRun = TRUE;
	m_bDBRun = TRUE;
	m_bAIRun = TRUE;
	m_bLogRun = TRUE;

	m_hIocpControl = NULL;
	m_hIocpWork = NULL;

	m_hBatchEvent = NULL;
	m_hDBEvent = NULL;
	m_hAIEvent = NULL;
	m_hLogEvent = NULL;
	m_hControl = NULL;
	m_hBatch = NULL;
	m_hDB = NULL;
	m_hAI = NULL;
	m_hLog = NULL;

	memset(m_itemgrade,0,sizeof(TITEMGRADE)*ITEMLEVEL_COUNT);
	memset(m_arTutorialItem, 0, sizeof(m_arTutorialItem));

	m_mapTMONSPAWN.clear();
	m_mapTMONSTER.clear();
	m_mapTPET.clear();

	m_mapTLOGCHANNEL.clear();
	m_mapTCHANNEL.clear();

	m_mapTCMDTEMP.clear();
	m_mapTMONAI.clear();
	m_mapTLEVEL.clear();
	m_mapTCLASS.clear();
	m_mapTSKILL.clear();
	m_mapTRACE.clear();
	m_mapTITEM.clear();
	m_mapTINDUN.clear();
	m_mapTItemAttr.clear();
	m_mapTPostBills.clear();
	m_mapCharPostBills.clear();
	m_mapTMAGICSKILL.clear();
	m_mapTCashItem.clear();
	m_mapTPvPointKill.clear();
	m_vOccupation.clear();
	m_vCastle.clear();
	m_vMission.clear();
	m_vTNMTRESULTMAP.clear();
	m_mapHelpMessage.clear();
	m_mapRPSGame.clear();
	m_mapMeetingRoomTime.clear();

	for(BYTE i = 0; i < ITEMGROUP_COUNT; i++)
		m_mapTMONITEM[i].clear();

	m_vTMagicSkill.clear();
	for(BYTE ik=0; ik<IK_COUNT; ik++)
		m_mapTITEMKINDATTR[ik].clear();

	m_bFirstGroupCount = FIRSTGRADEGROUPCOUNT;
	m_dwDuelID = 0;
	m_mapTDuel.clear();

	m_mapTFORMULA.clear();
	m_mapTQuestItem.clear();

	m_vAIBUF.clear();
	m_vGBBUF.clear();
	m_mapTOPERATOR.clear();
	m_mapACTIVEUSER.clear();


	for(BYTE i = 0; i < EVENT_COUNT; i++)
		m_wEventValue[i] = 0;

	m_bCashShopStop = FALSE;

	for(BYTE m = 0; m < COUNTRY_COUNT; m++)
		for(BYTE n = 0; n < MONTHRANKCOUNT; n++)
			m_arMonthRank[m][n].Reset();

	for(BYTE m = 0; m < COUNTRY_COUNT; m++)
		for(BYTE n = 0; n < FIRSTGRADEGROUPCOUNT; n++)
			m_arFirstGradeGroup[m][n].Reset();
	
	m_accept = INVALID_SOCKET;

#ifdef __HACK_SHIELD
	m_hHackShield = NULL;
#endif

#ifdef DEF_UDPLOG
	memset( m_szLogServerIP, 0, ONE_KBYTE);
	m_wLogServerPORT = 0;

	m_pUdpSocket = new CUdpSocket;
#endif
}

CTMapSvrModule::~CTMapSvrModule()
{
#ifdef DEF_UDPLOG
	delete	m_pUdpSocket;
	m_pUdpSocket = NULL;
#endif
}

void CTMapSvrModule::OnERROR( DWORD dwErrorCode)
{
	static CString strErrorMsg[30] = {
		"Can't open Registry",
		"Port is not assigned",
		"Password is not assigned",
		"DSN is not assigned",
		"Server ID is not assigned",
		"DB open error",
		"Query preparation error",
		"Thread resume error",
		"Thread Creation error",
		"Socket library error",
		"Socket library initalization error",
		"IOCP creation error",
		"Listen error",
		"WaitForConnect error",
		"World IP is not assigned",
		"World Port is not assigned",
		"Can't connect to WORLD Server",
		"WaitForMessage error",
		"UDP Socket error",
		"No Control Server data"
		"No Relay Server data",
		"No Limited Level data",
		"No Nation data",
		"nProtect Create error",	//__N_PROTECT
		"Invalid Message",	// 24
		"Invalid Channel",
		"Invalid Character",
		"Invalid Map",
		"Duplication Server ID",
		"Session Exit"};

	DWORD dwError = 0;
	if(dwErrorCode >= EC_INITSERVICEBASE && dwErrorCode < EC_SESSIONBASE)
		dwError = dwErrorCode - EC_INITSERVICEBASE;
	else if(dwErrorCode >= EC_SESSIONBASE)
		dwError = dwErrorCode - EC_SESSIONBASE + 24;

	if(dwErrorCode && dwError < 30)
		LogEvent("%s", strErrorMsg[dwError]);
}

DWORD CTMapSvrModule::_ControlThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->ControlThread();
}

DWORD CTMapSvrModule::_WorkThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->WorkThread();
}

DWORD CTMapSvrModule::_BatchThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->BatchThread();
}

DWORD CTMapSvrModule::_DBThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->DBThread();
}

DWORD CTMapSvrModule::_AIThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->AIThread();
}

DWORD CTMapSvrModule::_LogThread( LPVOID lpParam)
{
	CTMapSvrModule *pModule = (CTMapSvrModule *) lpParam;
	return pModule->LogThread();
}

DWORD CTMapSvrModule::OnEnter()
{
	srand( (unsigned)CTime::GetCurrentTime().GetTime() );

	while(!m_qBATCHJOB.empty())
	{
		delete m_qBATCHJOB.front();
		m_qBATCHJOB.pop();
	}

	while(!m_qDBJOB.empty())
	{
		delete m_qDBJOB.front();
		m_qDBJOB.pop();
	}

	while(!m_qLOGJOB.empty())
	{
		delete m_qLOGJOB.front();
		m_qLOGJOB.pop();
	}

	while(!m_qAIJOB.empty())
	{
		delete m_qAIJOB.front();
		m_qAIJOB.pop();
	}

	while(!m_vAIBUF.empty())
	{
		delete m_vAIBUF.back();
		m_vAIBUF.pop_back();
	}	

	while(!m_vGBBUF.empty())
	{
		delete m_vGBBUF.back();
		m_vGBBUF.pop_back();
	}

	InitializeCriticalSectionAndSpinCount(&m_csAIQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csDBQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csQUEUE, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csBATCH, 4000);
	InitializeCriticalSectionAndSpinCount(&m_csLOGQUEUE, 4000);

	m_hBatchEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hDBEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hAIEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hLogEvent = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_mapTPostBills.clear();
	m_mapCharPostBills.clear();
	m_mapTDuel.clear();
	m_mapTSELFMONID.clear();
	m_mapPLAYERNAME.clear();
	m_mapSESSION.clear();
	m_mapPLAYER.clear();
	m_mapSUSPENDER.clear();

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

#ifdef __HACK_SHIELD
	if(m_bNation == NATION_GERMAN)
	{
		GetModuleFileName(NULL, m_szHsbFilePath, ONE_KBYTE); 
		char *pSlash = ::strrchr(m_szHsbFilePath, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "HS4Story.hsb");

		m_hHackShield = _AhnHS_CreateServerObject(m_szHsbFilePath);
		if(m_hHackShield == ANTICPX_INVALID_HANDLE_VALUE)
		{
			LogHackShield(0, 0, _T("_AhnHS_CreateServerObject"));
			return EC_INITSERVICE_OPENREG;
		}
	}
#endif

#ifdef __N_PROTECT
	m_bEnableNP = FALSE;
	if(m_bNation == NATION_RUSSIA)
	{
		TCHAR szFN[ONE_KBYTE];
		GetModuleFileName(NULL, szFN, ONE_KBYTE); 
		char *pSlash = ::strrchr(szFN, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "nProtect");

#ifdef __N_PROTECT_DEBUG	
		if( ERROR_SUCCESS != InitGameguardAuth(szFN, 1, true, NPLOG_DEBUG | NPLOG_ERROR))
			return EC_INITSERVICE_NPROTECT;
		SetUpdateCondition(5, 50);
#else
		if( ERROR_SUCCESS != InitGameguardAuth(szFN, 50, true, NPLOG_ERROR))
			return EC_INITSERVICE_NPROTECT;
		SetUpdateCondition(30, 50);
#endif
		m_bEnableNP = TRUE;
	}

#endif

	InitEnvironment();

	if(!ResumeThreads())
		return EC_INITSERVICE_RESUMETHREAD;

	// Dump
	CTMiniDump::SetOption(MiniDumpWithFullMemory);

	return 0;
}

void CTMapSvrModule::InitEnvironment()
{
	for(DWORD ll=0; ll<m_vCastle.size(); ll++)
	{
		LPTLOCAL pCastle = m_vCastle[ll];

		CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pCastle->m_pZone->m_wMapID);
		if(pMap)
		{
			pMap->SetCastle(pCastle);
			pCastle->m_bValid = TRUE;
		}
		else
			pCastle->m_bValid = FALSE;
	}

	for(DWORD ms=0; ms<m_vMission.size(); ms++)
	{
		LPTLOCAL pMission = m_vMission[ms];

		CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pMission->m_pZone->m_wMapID);
		if(pMap)
			pMission->m_bValid = TRUE;
		else
			pMission->m_bValid = FALSE;
	}
}

void CTMapSvrModule::OnExit()
{
	if( m_accept != INVALID_SOCKET )
		closesocket(m_accept);

	m_listen.Close();

	SaveAllCharData();

	if(m_bThreadRun)
		ClearThread();

	while(!m_qBATCHJOB.empty())
	{
		delete m_qBATCHJOB.front();
		m_qBATCHJOB.pop();
	}

	while(!m_qAIJOB.empty())
	{
		delete m_qAIJOB.front();
		m_qAIJOB.pop();
	}

	while(!m_qDBJOB.empty())
	{
		delete m_qDBJOB.front();
		m_qDBJOB.pop();
	}

	while(!m_qLOGJOB.empty())
	{
		delete m_qLOGJOB.front();
		m_qLOGJOB.pop();
	}

	while(!m_vAIBUF.empty())
	{
		delete m_vAIBUF.back();
		m_vAIBUF.pop_back();
	}
	
	while(!m_vGBBUF.empty())
	{
		delete m_vGBBUF.back();
		m_vGBBUF.pop_back();
	}

	UpdateData();

	SetEvent(m_hBatchEvent);
	CloseHandle(m_hBatchEvent);

	SetEvent(m_hDBEvent);
	CloseHandle(m_hDBEvent);

	SetEvent(m_hAIEvent);
	CloseHandle(m_hAIEvent);

	SetEvent(m_hLogEvent);
	CloseHandle(m_hLogEvent);

	DeleteCriticalSection(&m_csAIQUEUE);
	DeleteCriticalSection(&m_csDBQUEUE);
	DeleteCriticalSection(&m_csQUEUE);
	DeleteCriticalSection(&m_csBATCH);
	DeleteCriticalSection(&m_csLOGQUEUE);

	m_world.Close();
	m_db.Close();

#ifdef __HACK_SHIELD
	if(m_bNation == NATION_GERMAN)
		_AhnHS_CloseServerHandle(m_hHackShield);
#endif

#ifdef __N_PROTECT
	if(m_bEnableNP)
		CleanupGameguardAuth();
#endif

	WSACleanup();
}

void CTMapSvrModule::SaveAllCharData()
{
	MAPPLAYERNAME::iterator it;
	for(it=m_mapPLAYERNAME.begin(); it!=m_mapPLAYERNAME.end(); it++)
	{
		if((*it).second->m_bMain)
			SendDM_SAVECHAR_REQ((*it).second, 0);

		SendDM_LOGOUT_REQ((*it).second, 0);
	}
}

DWORD CTMapSvrModule::LoadConfig()
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
		_T("GamePasswd"),
		NULL,
		NULL,
		(LPBYTE) &m_szGamePasswd,
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

	// Load game database DSN
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(
		hKey,
		_T("GameDSN"),
		NULL,
		NULL,
		(LPBYTE) &m_szGameDSN,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_DSNNOTASSIGNED;

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
		_T("GamePort"),
		NULL,
		NULL,
		(LPBYTE) &dwValue,
		&dwLength);
	if( nERROR != ERROR_SUCCESS )
		return EC_INITSERVICE_PORTNOTASSIGNED;
	else
		m_wGamePort = (WORD) dwValue;

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

DWORD CTMapSvrModule::CreateThreads()
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

	m_hDB = CreateThread(
		NULL, 0,
		_DBThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);
	m_bDBRun = TRUE;

	if(!m_hDB)
		return EC_INITSERVICE_CREATETHREAD;

	m_hAI = CreateThread(
		NULL, 0,
		_AIThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);
	m_bAIRun = TRUE;

	if(!m_hAI)
		return EC_INITSERVICE_CREATETHREAD;

	m_hLog = CreateThread(
		NULL, 0,
		_LogThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThread);

	m_bLogRun = TRUE;

	if(!m_hLog)
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

BYTE CTMapSvrModule::ResumeThreads()
{
	if( ResumeThread(m_hControl) < 0 )
		return FALSE;

	if( ResumeThread(m_hBatch) < 0 )
		return FALSE;

	if( ResumeThread(m_hDB) < 0 )
		return FALSE;

	if( ResumeThread(m_hAI) < 0 )
		return FALSE;

	if( ResumeThread(m_hLog) < 0 )
		return FALSE;

	for( BYTE i=0; i<m_bNumWorker; i++)
		if( ResumeThread(m_hWorker[i]) < 0 )
			return FALSE;
	
	m_bThreadRun = TRUE;

	return TRUE;
}

DWORD CTMapSvrModule::InitNetwork()
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

	if(!m_listen.Listen(m_wGamePort))
		return EC_INITSERVICE_LISTENFAILED;

	m_hIocpControl = CreateIoCompletionPort(
		(HANDLE) m_listen.m_sock,
		m_hIocpControl,
		COMP_ACCEPT, 0);

	if(!m_hIocpControl)
		return EC_INITSERVICE_CREATEIOCP;

	if(!WaitForConnect())
		return EC_INITSERVICE_WAITFORCONNECT;

	m_world.m_Recv.ExpandIoBuffer(RECV_SVR_SIZE);
	m_world.m_bUseCrypt = FALSE;

	if(!m_world.Connect( m_szWorldIP, m_wWorldPort))
		return EC_INITSERVICE_CONNECTWORLD;

	m_hIocpWork = CreateIoCompletionPort(
		(HANDLE) m_world.m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	if(!m_hIocpWork)
		return EC_INITSERVICE_CREATEIOCP;

	if(!m_world.WaitForMessage())
		return EC_INITSERVICE_WAITFORMSG;
	SendMW_CONNECT_ACK();

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

BYTE CTMapSvrModule::WaitForConnect()
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

BYTE CTMapSvrModule::Accept()
{
	if(m_accept == INVALID_SOCKET)
	{
		m_vAccept.Clear();
		return FALSE;
	}

	CTPlayer *pPlayer = new CTPlayer();

	pPlayer->Open( m_accept, m_vAccept);
	pPlayer->m_Recv.ExpandIoBuffer(RECV_CLI_SIZE);

	if(pPlayer->m_addr.sin_addr.s_addr == m_addrCtrl.sin_addr.s_addr)
		pPlayer->m_bSessionType = SESSION_SERVER;
	else
		pPlayer->m_bUseCrypt = TRUE;

	m_accept = INVALID_SOCKET;
	m_vAccept.Clear();

	HANDLE hIocpWork = CreateIoCompletionPort(
		(HANDLE) pPlayer->m_sock,
		m_hIocpWork,
		COMP_SESSION, 0);

	SMART_LOCKCS(&m_csBATCH);

#ifdef __HACK_SHIELD
	if(m_bNation == NATION_GERMAN)
	{
		pPlayer->m_hHackShield = _AhnHS_CreateClientObject(m_hHackShield);
		if(pPlayer->m_hHackShield == NULL)
		{
			pPlayer->Close();
			delete pPlayer;
			return FALSE;
		}
		pPlayer->m_dwHSSendTick = pPlayer->m_dwHSRecvTick = 0;
	}
#endif

#ifdef __N_PROTECT
	pPlayer->m_csNProtect = NULL;

	if(m_bEnableNP)
	{
		pPlayer->m_csNProtect = new CCSAuth2();

		if(pPlayer->m_csNProtect)
			pPlayer->m_csNProtect->Init();
		else
		{
			pPlayer->Close();
			delete pPlayer;
			return FALSE;
		}
		
		pPlayer->m_dwNPSendTime = pPlayer->m_bFirst = pPlayer->m_bSend = 0;
	}
#endif

	m_mapSESSION.insert( MAPPLAYER::value_type( (DWORD_PTR) pPlayer, pPlayer));

	if( !hIocpWork || !pPlayer->WaitForMessage() )
	{
		m_mapSESSION.erase((DWORD_PTR) pPlayer);
		pPlayer->Close();
		delete pPlayer;

		return FALSE;
	}

	pPlayer->m_dwAcceptTick = GetTickCount();
	ATLTRACE2("SESSION INSERT %s, %d\n", inet_ntoa(pPlayer->m_addr.sin_addr), m_mapSESSION.size());
	return TRUE;
}

void CTMapSvrModule::ClearThread()
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

	EnterCriticalSection(&m_csDBQUEUE);
	m_bDBRun = FALSE;
	LeaveCriticalSection(&m_csDBQUEUE);
	SetEvent(m_hDBEvent);
	WaitForSingleObject( m_hDB, INFINITE);

	EnterCriticalSection(&m_csAIQUEUE);
	m_bAIRun = FALSE;
	LeaveCriticalSection(&m_csAIQUEUE);
	SetEvent(m_hAIEvent);
	WaitForSingleObject( m_hAI, INFINITE);

	EnterCriticalSection(&m_csLOGQUEUE);
	m_bLogRun = FALSE;
	LeaveCriticalSection(&m_csLOGQUEUE);
	SetEvent(m_hLogEvent);
	WaitForSingleObject( m_hLog, INFINITE);

	CloseHandle(m_hIocpControl);
	CloseHandle(m_hIocpWork);
}

void CTMapSvrModule::UpdateData()
{
	MAPTCHANNEL::iterator itCH;

	MAPTMONSPAWNTEMP::iterator itSPAWN;
	MAPTMONSTERTEMP::iterator itMON;
	MAPTPETTEMP::iterator itPET;
	MAPTMONATTR::iterator itMONATTR;
	MAPTSKILLTEMP::iterator itTSKILL;
	MAPTITEMTEMP::iterator itITEM;

	MAPTAICMDTEMP::iterator itCMD;
	MAPTMONSTERAI::iterator itAI;

	MAPPLAYER::iterator itPLAYER;
	MAPTLEVEL::iterator itLEVEL;
	MAPTSTAT::iterator itTSTAT;

	MAPMAPVQUESTTEMP::iterator itTRIGGER;
	MAPQUESTTEMP::iterator itQUESTTEMP;
	MAPTSPAWNPOS::iterator itCHARSPAWN;
	MAPTNPC::iterator itNPC;
	MAPTITEMMAGIC::iterator itIMagic;
	MAPTITEMATTR::iterator itIA;

	MAPTLOCAL::iterator itLc;
	MAPTBATTLEZONE::iterator itBz;

	MAPVMONSPAWN::iterator itMS;
	MAPTPORTAL::iterator itPt;
	MAPTDUEL::iterator itDuel;

	MAPTFORMULA::iterator itFORMULA;
	MAPTCASHITEM::iterator itCASHITEM;
	MAPTINDURN::iterator itINDURN;
	MAPTGAMBLE::iterator itGAM;
	MAPTSTORAGEITEM::iterator itQI;
	MAPTPVPOINT::iterator itPvP;
	MAPTOURNAMENTPLAYER::iterator itTP;
	MAPARENA::iterator itArn;

	for( itPLAYER = m_mapSESSION.begin(); itPLAYER != m_mapSESSION.end(); itPLAYER++)
	{
		ExitMAP((*itPLAYER).second);
		delete (*itPLAYER).second;
	}

	m_mapTPostBills.clear();
	m_mapCharPostBills.clear();
	m_mapTSELFMONID.clear();
	m_mapPLAYERNAME.clear();
	m_mapSESSION.clear();
	m_mapPLAYER.clear();
	m_mapTOnTimeMon.clear();
	m_mapTSvrMsg.clear();
	m_mapACTIVEUSER.clear();
	m_mapSUSPENDER.clear();
	m_vOccupation.clear();
	m_vCastle.clear();
	m_vMission.clear();
	m_mapTournament.clear();
	m_vTNMTRESULTMAP.clear();

	while(!m_qDBJOB.empty())
	{
		LPPACKETBUF pBUF = m_qDBJOB.front();

		m_qDBJOB.pop();
		OnReceive(pBUF);

		delete pBUF;
	}

	while(!m_qLOGJOB.empty())
	{
		LPPACKETBUF pBUF = m_qLOGJOB.front();
		m_qLOGJOB.pop();
		OnReceive(pBUF);

		delete pBUF;
	}

	for( itTRIGGER = m_mapTRIGGER.begin(); itTRIGGER != m_mapTRIGGER.end(); itTRIGGER++)
	{
		MAPVQUESTTEMP::iterator itVQUEST;

		for( itVQUEST = (*itTRIGGER).second->begin(); itVQUEST != (*itTRIGGER).second->end(); itVQUEST++)
		{
			(*itVQUEST).second->clear();
			delete (*itVQUEST).second;
		}

		(*itTRIGGER).second->clear();
		delete (*itTRIGGER).second;
	}

	m_mapQUESTTEMP.clear();
	m_mapTRIGGER.clear();

	while(!m_vTMagicSkill.empty())
	{
		delete m_vTMagicSkill.back();
		m_vTMagicSkill.pop_back();
	}

	while(!m_vQUESTTEMP.empty())
	{
		delete m_vQUESTTEMP.back();
		m_vQUESTTEMP.pop_back();
	}

	while(!m_vItemSet.empty())
	{
		delete m_vItemSet.back();
		m_vItemSet.pop_back();
	}
	while(!m_vCashCategory.empty())
	{
		delete m_vCashCategory.back();
		m_vCashCategory.pop_back();
	}

	
	MAPCASHGAMBLE::iterator itCG;
	for(itCG = m_mapCashGameble.begin(); itCG != m_mapCashGameble.end(); itCG++)
	{
		while(!(*itCG).second->empty())
		{
			delete (*itCG).second->back()->m_pItem;
			delete (*itCG).second->back();
			(*itCG).second->pop_back();
		}

	}

	for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
		delete (*itCH).second;

	CTMap::m_vTCHANNEL.clear();
	m_mapTLOGCHANNEL.clear();
	m_mapTCHANNEL.clear();
	m_mapTMAGICSKILL.clear();
	for(BYTE ik=0; ik<IK_COUNT; ik++)
		m_mapTITEMKINDATTR[ik].clear();

	for( itCASHITEM = m_mapTCashItem.begin(); itCASHITEM != m_mapTCashItem.end(); itCASHITEM++)
	{
		delete (*itCASHITEM).second->m_pItem;
		delete (*itCASHITEM).second;
	}
	m_mapTCashItem.clear();

	for( itSPAWN = m_mapTMONSPAWN.begin(); itSPAWN != m_mapTMONSPAWN.end(); itSPAWN++)
		delete (*itSPAWN).second;
	m_mapTMONSPAWN.clear();

	for( itMS = m_mvSpawnGroup.begin(); itMS != m_mvSpawnGroup.end(); itMS++)
		delete (*itMS).second;
	m_mvSpawnGroup.clear();

	for( itMON = m_mapTMONSTER.begin(); itMON != m_mapTMONSTER.end(); itMON++)
		delete (*itMON).second;
	m_mapTMONSTER.clear();

	for( itMONATTR = m_mapTMONATTR.begin(); itMONATTR != m_mapTMONATTR.end(); itMONATTR++)
		delete (*itMONATTR).second;
	m_mapTMONATTR.clear();

	for( itPET = m_mapTPET.begin(); itPET != m_mapTPET.end(); itPET++)
		delete (*itPET).second;
	m_mapTPET.clear();

	for( itCMD = m_mapTCMDTEMP.begin(); itCMD != m_mapTCMDTEMP.end(); itCMD++)
		delete (*itCMD).second;
	m_mapTCMDTEMP.clear();

	for( itAI = m_mapTMONAI.begin(); itAI != m_mapTMONAI.end(); itAI++)
		delete (*itAI).second;
	m_mapTMONAI.clear();

	for( itLEVEL = m_mapTLEVEL.begin(); itLEVEL != m_mapTLEVEL.end(); itLEVEL++)
		delete (*itLEVEL).second;
	m_mapTLEVEL.clear();

	for( itTSTAT = m_mapTCLASS.begin(); itTSTAT != m_mapTCLASS.end(); itTSTAT++)
		delete (*itTSTAT).second;
	m_mapTCLASS.clear();

	for( itTSTAT = m_mapTRACE.begin(); itTSTAT != m_mapTRACE.end(); itTSTAT++)
		delete (*itTSTAT).second;
	m_mapTRACE.clear();

	for( itITEM = m_mapTITEM.begin(); itITEM != m_mapTITEM.end(); itITEM++)
		delete (*itITEM).second;
	m_mapTITEM.clear();

	for( itTSKILL = m_mapTSKILL.begin(); itTSKILL != m_mapTSKILL.end(); itTSKILL++)
		delete (*itTSKILL).second;
	m_mapTSKILL.clear();

	for( itCHARSPAWN = m_mapTSPAWNPOS.begin(); itCHARSPAWN != m_mapTSPAWNPOS.end(); itCHARSPAWN++)
		delete (*itCHARSPAWN).second;
	m_mapTSPAWNPOS.clear();

	for( itNPC = m_mapTNpc.begin(); itNPC != m_mapTNpc.end(); itNPC++)
		delete (*itNPC).second;
	m_mapTNpc.clear();

	for( itIMagic=m_mapTItemMagic.begin(); itIMagic!=m_mapTItemMagic.end(); itIMagic++)
		delete (*itIMagic).second;
	m_mapTItemMagic.clear();

	for(itIA=m_mapTItemAttr.begin(); itIA!=m_mapTItemAttr.end(); itIA++)
		delete (*itIA).second;
	m_mapTItemAttr.clear();

	for(itLc=m_mapTLOCAL.begin(); itLc!=m_mapTLOCAL.end(); itLc++)
		delete (*itLc).second;
	m_mapTLOCAL.clear();

	for(itBz=m_mapTBATTLEZONE.begin(); itBz!=m_mapTBATTLEZONE.end(); itBz++)
	{
		for( itPvP = (*itBz).second->m_mapTPvPoint.begin(); itPvP != (*itBz).second->m_mapTPvPoint.end(); itPvP++)
			delete (*itPvP).second;
		(*itBz).second->m_mapTPvPoint.clear();

		delete (*itBz).second;
	}
	m_mapTBATTLEZONE.clear();

	for(itPt=m_mapTPortal.begin(); itPt!=m_mapTPortal.end(); itPt++)
		delete (*itPt).second;
	m_mapTPortal.clear();

	m_mapTOPERATOR.clear();

	for(itDuel=m_mapTDuel.begin(); itDuel!=m_mapTDuel.end(); itDuel++)
		delete (*itDuel).second;
	m_mapTDuel.clear();

	for(itFORMULA = m_mapTFORMULA.begin(); itFORMULA!=m_mapTFORMULA.end(); itFORMULA++)
		delete (*itFORMULA).second;
	m_mapTFORMULA.clear();

	for(itINDURN = m_mapTINDUN.begin(); itINDURN != m_mapTINDUN.end(); itINDURN++)
		delete (*itINDURN).second;
	m_mapTINDUN.clear();

	for(itQI = m_mapTQuestItem.begin(); itQI != m_mapTQuestItem.end(); itQI++)
		delete (*itQI).second;
	m_mapTQuestItem.clear();

	for(itGAM = m_mapTGAMBLE.begin(); itGAM != m_mapTGAMBLE.end(); itGAM++)
	{
		while(!(*itGAM).second.empty())
		{
			delete (*itGAM).second.back();
			(*itGAM).second.pop_back();
		}
	}
	m_mapTGAMBLE.clear();
	for(BYTE i = 0; i < ITEMGROUP_COUNT; i++)
		m_mapTMONITEM[i].clear();

	for( itPvP = m_mapTPvPointKill.begin(); itPvP != m_mapTPvPointKill.end(); itPvP++)
		delete (*itPvP).second;
	m_mapTPvPointKill.clear();

	for( itTP= m_mapTNMTPlayer.begin(); itTP != m_mapTNMTPlayer.end(); itTP++)
		delete (*itTP).second;
	m_mapTNMTPlayer.clear();

	for(itArn=m_mapArena.begin(); itArn != m_mapArena.end(); itArn++)
		delete itArn->second;
	m_mapArena.clear();
}

void CTMapSvrModule::OnInvalidSession( CTMapSession *pSession)
{
	if(pSession->OnInvalidSession())
		ClosingSession(pSession);
}

void CTMapSvrModule::OnCloseSession( CTMapSession *pSession)
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

		MAPPLAYER::iterator itSus = m_mapSUSPENDER.find(pPlayer->m_dwID);
		if(itSus == m_mapSUSPENDER.end() ||	pPlayer != (*itSus).second )
		{
			CString strFindName = pPlayer->m_strNAME;
			strFindName.MakeUpper();
			MAPPLAYERNAME::iterator itNAME = m_mapPLAYERNAME.find(strFindName);
			if( itNAME != m_mapPLAYERNAME.end() )
				m_mapPLAYERNAME.erase(itNAME);

			finder = m_mapPLAYER.find(pPlayer->m_dwID);
			if( finder != m_mapPLAYER.end() )
			{
				m_mapPLAYER.erase(finder);

				if(itSus != m_mapSUSPENDER.end())
				{
					CTPlayer * pNew = (*itSus).second;
					LPPACKETBUF pBUF = new PACKETBUF();
					pBUF->m_packet.SetID(CS_CONNECT_REQ)
						<< TVERSION
						<< pNew->m_bChannel
						<< pNew->m_dwUserID
						<< pNew->m_dwID
						<< pNew->m_dwKEY
						<< pNew->m_dwIPAddr
						<< pNew->m_wPort;

					pBUF->m_pSESSION = pNew;
					SayToBATCH(pBUF);
				}
			}
		}

		ExitMAP(pPlayer);

		m_mapSUSPENDER.erase(pPlayer->m_dwID);

#ifdef __N_PROTECT
		if(pPlayer->m_csNProtect && m_bEnableNP)
		{
			delete pPlayer->m_csNProtect;
			pPlayer->m_csNProtect = NULL;
	#ifdef __N_PROTECT_DEBUG
			LogNProtect(pPlayer->m_dwID, 0, _T("User Disconect") );
	#endif
		}
#endif

#ifdef __HACK_SHIELD
		if(m_bNation == NATION_GERMAN)
			_AhnHS_CloseClientHandle(pPlayer->m_hHackShield);
#endif
		delete pPlayer;
		LeaveCriticalSection(&m_csBATCH);
	}
	else
	{
		LPPACKETBUF pBUF = new PACKETBUF();

		pBUF->m_packet.SetID(SM_QUITSERVICE_REQ);
		SayToBATCH(pBUF);
	}
}

void CTMapSvrModule::SetEventCloseSession(CTMapSession * pSession, BYTE bSave)
{
	if(pSession->m_bSessionType == SESSION_CLIENT)
	{
		CTPlayer *pPlayer = (CTPlayer *)pSession;

		if(pPlayer->m_dwID)
		{
			pPlayer->StoreClose();
			CheckMeetingRoom(pPlayer, TRUE);

			SendSM_POSTBILLERASE_REQ(
				pPlayer->m_dwID,
				0);

			if(bSave)
			{
				if(!pPlayer->m_bGraceExit)
				{
					if(pPlayer->m_dwLastAttackCharID && m_dwTick - pPlayer->m_dwLastAttackTick < 60 * 1000)
						pPlayer->OnDie(pPlayer->m_dwLastAttackCharID, OT_PC, 0);
				}

				SendDM_SAVECHAR_REQ( pPlayer, 0);
			}

			if( pPlayer->m_bLogout )
				SendDM_LOGOUT_REQ( pPlayer, 0);

			if(pPlayer->m_pMAP)
			{
				if(pPlayer->m_dwLockedMonID)
				{
					MAPTMONSTER::iterator finder = pPlayer->m_pMAP->m_mapTMONSTER.find(pPlayer->m_dwLockedMonID);
					if( finder != pPlayer->m_pMAP->m_mapTMONSTER.end())
					{
						pPlayer->m_dwLockedMonID = 0;
						(*finder).second->m_dwInvenLock = 0;
					}
				}

				ExitMAP(pPlayer);
			}

			pPlayer->ClearRecallMon(FALSE);
			pPlayer->ClearSelfMon(FALSE);
			pPlayer->ClearMain();

			pPlayer->m_bMain = FALSE;
		}
	}

	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pSession);
}

void CTMapSvrModule::ClosingSession( CTMapSession *pSession)
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

void CTMapSvrModule::CloseSession( CTMapSession *pSession)
{
	pSession->m_dwCloseTick = GetTickCount();
	pSession->CloseSession();
}

DWORD CTMapSvrModule::InitDB()
{
	if(!m_db.Open( m_szGameDSN, m_szDBUserID, m_szGamePasswd))
		return EC_INITSERVICE_DBOPENFAILED;

	if(!InitQueryTMapSvr(&m_db))
		return EC_INITSERVICE_PREPAREQUERY;

	return EC_NOERROR;
}

DWORD CTMapSvrModule::LoadData()
{
	CTMap::m_vTCHANNEL.clear();
	m_mapTLOGCHANNEL.clear();
	m_mapTCHANNEL.clear();

	m_mapTMONSPAWN.clear();
	m_mapTMONSTER.clear();
	m_mapTMONATTR.clear();
	m_mapTPET.clear();
	m_mapTCMDTEMP.clear();
	m_mapTMONAI.clear();
	m_mapTLEVEL.clear();
	m_mapTCLASS.clear();
	m_mapTSKILL.clear();
	m_mapTRACE.clear();
	m_mapTItemAttr.clear();
	m_mapTItemMagic.clear();
	m_vItemSet.clear();
	m_mapTITEM.clear();
	m_mapTINDUN.clear();
	m_mapTNpc.clear();
	m_mapTLOCAL.clear();
	m_mapTPortal.clear();
	m_mapTOPERATOR.clear();
	m_mapTPostBills.clear();
	m_mapCharPostBills.clear();
	m_mapTMAGICSKILL.clear();
	m_mapTCashItem.clear();
	m_mapTGAMBLE.clear();
	m_mapTPvPointKill.clear();	
	m_mapCashGameble.clear();
	m_mapHelpMessage.clear();
	m_mapRPSGame.clear();
	m_mapArena.clear();

	DWORD dwGBCmd[GB_COUNT] = {0,1000,15000,15000,20000,300000};
	for(BYTE gb=0; gb<GB_COUNT; gb++)
		m_dwGBDelay[gb] = dwGBCmd[gb];

	for(BYTE i = 0; i < ITEMGROUP_COUNT; i++)
		m_mapTMONITEM[i].clear();

	m_mapTSvrMsg.clear();
	m_vCashCategory.clear();

	DEFINE_QUERY(&m_db,CSPQuestSendPost)
	UNDEFINE_QUERY()

	for(BYTE ik=0; ik<IK_COUNT; ik++)
		m_mapTITEMKINDATTR[ik].clear();

	DEFINE_QUERY(&m_db, CSPLoadService)
	query->m_bWorld = SVRGRP_NULL;
	query->m_bServiceGroup = SVRGRP_CTLSVR;
	if(!query->Call())
		return EC_INITSERVICE_CTRLSVRDATA;

	m_addrCtrl.sin_addr.s_addr = inet_addr(query->m_szIP);
	m_addrCtrl.sin_port = query->m_wPort;
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPLoadService)
	query->m_bWorld = m_bGroupID;
	query->m_bServiceGroup = SVRGRP_RLYSVR;
	if(!query->Call() || !query->m_wPort)
		return EC_INITSERVICE_RELAYSVRDATA;

	m_addrRelay.sin_addr.s_addr = inet_addr(query->m_szIP);
	m_addrRelay.sin_port = query->m_wPort;
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CSPInitGenItemID)
	query->m_bServerID = m_bServerID;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;
	m_dlGenItemID = query->m_dlGenItemID;
	if(m_dlGenItemID <= 0)
		return EC_INITSERVICE_DBOPENFAILED;
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPGetLimitedLevel)
	if(!query->Call())
		return EC_INITSERVICE_LIMITEDLEVEL;
	m_bMaxLevel = query->m_bMaxLevel;
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CSPGetNation)
	if(!query->Call())
		return EC_INITSERVICE_NATION;
	m_bNation = query->m_bNation;
	if(!m_bNation)
		return EC_INITSERVICE_NATION;
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLHelpMessage)
	if(query->Open())
	{
		while(query->Fetch())
		{
			THELPMESSAGE help;
			help.m_bID = query->m_bID;
			help.m_dlStartTime = __DBTOTIME(query->m_timeStart);
			help.m_dlEndTime = __DBTOTIME(query->m_timeEnd);
			help.m_strMessage = query->m_szMessage;
			help.m_bNotice = FALSE;
			if(!help.m_dlEndTime || help.m_dlEndTime > CTime::GetCurrentTime().GetTime())
				m_mapHelpMessage.insert(MAPHELPMESSAGE::value_type(help.m_bID, help));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLTutorialCharItem)
	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_bCountry >= TCONTRY_COUNT || query->m_bClass >= TCLASS_COUNT || query->m_bSex >= TSEX_COUNT)
				continue;

			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_PRMWEAPON] = query->m_wPrmWeapon;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_SNDWEAPON] = query->m_wSndWeapon;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_LONGWEAPON] = query->m_wLongWeapon;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_HEAD] = query->m_wHead;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_BACK] = query->m_wBack;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_BODY] = query->m_wBody;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_PANTS] = query->m_wPants;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_FOOT] = query->m_wFoot;
			m_arTutorialItem[query->m_bCountry][query->m_bClass][query->m_bSex][ES_HAND] = query->m_wHand;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLSvrMsg)
	if(query->Open())
	{
		while(query->Fetch())
			m_mapTSvrMsg.insert(MAPTSTRING::value_type(query->m_dwID, query->m_szMessage));
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLItemAttrChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTITEMATTR pAttr = new TITEMATTR();
			pAttr->m_wID = query->m_wID;
			pAttr->m_bKind = query->m_bKind;
			pAttr->m_bGrade = query->m_bGrade;
			pAttr->m_bBlockProb = query->m_bBlockProb;
			pAttr->m_wDP = query->m_wDP;
			pAttr->m_wMDP = query->m_wMDP;
			pAttr->m_wMinAP = query->m_wMinAP;
			pAttr->m_wMaxAP = query->m_wMaxAP;
			pAttr->m_wMinMAP = query->m_wMinMAP;
			pAttr->m_wMaxMAP = query->m_wMaxMAP;
			m_mapTItemAttr.insert(MAPTITEMATTR::value_type(pAttr->m_wID, pAttr));
			m_mapTITEMKINDATTR[pAttr->m_bKind].insert(MAPTITEMATTR::value_type(pAttr->m_wID, pAttr));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLItemGradeChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			m_itemgrade[query->m_bLevel].m_bLevel = query->m_bLevel;
			m_itemgrade[query->m_bLevel].m_bGrade = query->m_bGrade;
			m_itemgrade[query->m_bLevel].m_bProb = query->m_bProb;
			m_itemgrade[query->m_bLevel].m_dwMoney = query->m_dwMoney;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLItemMagicChart)

	for(BYTE i=0; i<IK_COUNT; i++)
		m_vItemMagic[i].clear();

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTITEMMAGIC pMagic = new TITEMMAGIC();
			pMagic->m_bMagic = query->m_bMagic;
			pMagic->m_dwKind = query->m_dwKind;
			pMagic->m_wMaxValue = query->m_wMaxValue;
			pMagic->m_bRvType = query->m_bRvType;
			pMagic->m_bIsMagic = query->m_bIsMagic;
			pMagic->m_bIsRare = query->m_bIsRare;
			pMagic->m_bExclIndex = query->m_bExclIndex;
			pMagic->m_bOptionKind = query->m_bOptionKind;
			pMagic->m_wAutoSkill = query->m_wAutoSkill;
			pMagic->m_bRefine = query->m_bRefine;
			pMagic->m_wMaxBound = query->m_wMaxBound;
			pMagic->m_wRareBound = query->m_wRareBound;
			m_mapTItemMagic.insert(MAPTITEMMAGIC::value_type(pMagic->m_bMagic, pMagic));

			for(BYTE i=1; i<IK_HP; i++)
			{
				if(pMagic->m_dwKind & (1<<(i-1)))
					m_vItemMagic[i].push_back(pMagic);
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLItemSetChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTITEMSET pSet = new TITEMSET();
			pSet->m_bLevel = query->m_bLevel;
			for(BYTE i=0; i<TMAGIC_MAX; i++)
			{
				pSet->m_pMagic[i] = GetItemMagic(query->m_bMagic[i]);
				pSet->m_bMin[i] = query->m_bMin[i];
				pSet->m_bValue[i] = query->m_bValue[i];
			}
			pSet->m_wBaseID = query->m_wBaseID;
			pSet->m_wSetID = query->m_wSetID;

			m_vItemSet.push_back(pSet);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLItemChart)
	if(query->Open())
	{
		MAPVTMONITEM::iterator itMI;

		while(query->Fetch())
		{
			LPTITEM pITEM = new TITEM();

			pITEM->m_wItemID = query->m_wItemID;
			pITEM->m_bType = query->m_bType;
			pITEM->m_bKind = query->m_bKind;
			pITEM->m_wAttrID = query->m_wAttrID;
			pITEM->m_wUseValue = query->m_wUseValue;

			pITEM->m_dwClassID = query->m_dwClassID;
			pITEM->m_dwSlotID = query->m_dwSlotID;
			pITEM->m_fPrice = query->m_fPrice;
			pITEM->m_fPvPrice = query->m_fPvPrice;
			pITEM->m_bIsSell = query->m_bIsSell;

			pITEM->m_bPrmSlotID = query->m_bPrmSlotID;
			pITEM->m_bSubSlotID = query->m_bSubSlotID;
			pITEM->m_bSlotCount = query->m_bSlotCount;
			pITEM->m_bMinRange = query->m_bMinRange;
			pITEM->m_bMaxRange = query->m_bMaxRange;
			pITEM->m_bDefaultLevel = query->m_bDefaultLevel;
			pITEM->m_bStack = query->m_bStack;
			pITEM->m_bEquipSkill = query->m_bEquipSkill;

			pITEM->m_bUseItemKind = query->m_bUseItemKind;
			pITEM->m_bUseItemCount = query->m_bUseItemCount;
			pITEM->m_bGrade = query->m_bGrade;
			pITEM->m_wUseTime = query->m_wUseTime;
			pITEM->m_bUseType = query->m_bUseType;
            pITEM->m_bCanGrade = query->m_bCanGrade;
			pITEM->m_bCanMagic = query->m_bCanMagic;
			pITEM->m_bCanRare = query->m_bCanRare;
			pITEM->m_bDropLevel = query->m_bDropLevel;
			pITEM->m_dwSpeedInc = query->m_dwSpeedInc;
			pITEM->m_bItemCountry = query->m_bItemCountry;
			pITEM->m_bSpecial = query->m_bIsSpecial;
			pITEM->m_dwDelay = query->m_dwDelay;
			pITEM->m_bGambleProb = query->m_bGambleProb;
			pITEM->m_wItemProb_G = query->m_wItemProb_G;
			pITEM->m_bCanGamble = query->m_bCanGamble;
			pITEM->m_bDestroyProb = query->m_bDestroyProb;
			pITEM->m_dwDuraMax = query->m_dwDuraMax;
			pITEM->m_bRefineMax = query->m_bRefineMax;
			pITEM->m_bCanRepair = query->m_bCanRepair;
			pITEM->m_wDelayGroupID = query->m_wDelayGroupID;
			pITEM->m_wWeight = query->m_wWeight;
			pITEM->m_bGroup = query->m_bGroupID;
			pITEM->m_bInitState = query->m_bInitState;
			pITEM->m_bCanWrap = query->m_bCanWrap;
			pITEM->m_dwCode = query->m_dwCode;
			pITEM->m_bCanColor = query->m_bCanColor;
			memcpy(pITEM->m_fRevision, query->m_fRevision, sizeof(FLOAT)*4);
			pITEM->m_bConsumable = query->m_bConsumable;
			m_mapTITEM.insert( MAPTITEMTEMP::value_type( pITEM->m_wItemID, pITEM));

			if(pITEM->m_bDropLevel)
			{
				itMI =  m_mapTMONITEM[pITEM->m_bGroup].find(pITEM->m_bDropLevel);
				if( itMI == m_mapTMONITEM[pITEM->m_bGroup].end())
				{
					VTITEMTEMP vMONITEM;
					vMONITEM.clear();
					vMONITEM.push_back(pITEM);
					m_mapTMONITEM[pITEM->m_bGroup].insert(MAPVTMONITEM::value_type(pITEM->m_bDropLevel, vMONITEM));
				}
				else
					(*itMI).second.push_back(pITEM);
			}
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLRPSGame)
	if(query->Open())
	{
		while(query->Fetch())
		{
			TRPSGAME rps;

			for(BYTE ri=0; ri<2; ri++)
			{
				rps.m_pRewardItem[ri] = NULL;
				rps.m_bItemCount[ri] = 0;

				if(query->m_wRewardItem[ri])
				{
					LPTITEM pItem = FindTItem(query->m_wRewardItem[ri]);
					if(!pItem || !query->m_bItemCount[ri])
						continue;

					rps.m_pRewardItem[ri] = pItem;
					rps.m_bItemCount[ri] = query->m_bItemCount[ri];
				}
			}

			rps.m_bType = query->m_bType;
			rps.m_bWinCount = query->m_bWinCount;
			rps.m_wItemID = query->m_wItemID;
			rps.m_wWinKeep = query->m_wWinKeep;
			rps.m_wWinPeriod = query->m_wWinPeriod;
			rps.m_dwRewardMoney = query->m_dwRewardMoney;
			memcpy(rps.m_bProb, query->m_bProb, sizeof(rps.m_bProb));
			m_mapRPSGame.insert(MAPRPSGAME::value_type(MAKEWORD(rps.m_bType, rps.m_bWinCount), rps));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	MAPCASHGAMBLE::iterator itCG;
	MAPWDWORD::iterator itCD;
	DEFINE_QUERY(&m_db, CTBLCashGambleChart)
	if(query->Open())
	{	
		while(query->Fetch())
		{
			CTItem * pItem = CreateItem(query->m_item);
			if(!pItem)
				continue;

			LPTCASHGAMBLE pCg = new TCASHGAMBLE();
			pCg->m_dwID = query->m_dwID;
			pCg->m_dwProb = query->m_dwProb;
			pCg->m_pItem = pItem;
			pCg->m_wGroup = query->m_wGroup;
			pCg->m_wUseTime = query->m_wUseTime;
			LPVTCASHGAMBLE pVTCASHGAMBLE = NULL;

			itCG = m_mapCashGameble.find(pCg->m_wGroup);
			if(itCG == m_mapCashGameble.end())
			{
				pVTCASHGAMBLE = new VTCASHGAMBLE();
				pVTCASHGAMBLE->push_back(pCg);

				m_mapCashGameble.insert(MAPCASHGAMBLE::value_type(pCg->m_wGroup,pVTCASHGAMBLE));
			}
			else
			{
				pVTCASHGAMBLE = (*itCG).second;
				pVTCASHGAMBLE->push_back(pCg);
			}

			itCD = m_mapMaxCashGambleProb.find(pCg->m_wGroup);
			if( itCD != m_mapMaxCashGambleProb.end())
				(*itCD).second += pCg->m_dwProb;
			else			
				m_mapMaxCashGambleProb.insert(MAPWDWORD::value_type(pCg->m_wGroup,pCg->m_dwProb));			
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLItemMagicSkill)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTMAGICSKILL pMSkill = new TMAGICSKILL();
			pMSkill->m_bMagic = query->m_bMagic;
			pMSkill->m_dwKind = query->m_dwKind;
			pMSkill->m_wSkillID = query->m_wSkill;
			pMSkill->m_bIsMagic = query->m_bIsMagic;
			pMSkill->m_bIsRare = query->m_bIsRare;
			pMSkill->m_bMinLevel = query->m_bMinLevel;

			m_vTMagicSkill.push_back(pMSkill);
			for(DWORD i=1 ; i<IK_COUNT; i++)
			{
				BYTE bKind = BYTE((pMSkill->m_dwKind & (1<<(i-1))) ? i : 0);
				if(bKind)
				{
					WORD wKey = MAKEWORD(pMSkill->m_bMagic, bKind);
					MAPTMAGICSKILL::iterator itMS = m_mapTMAGICSKILL.find(wKey);
					if(itMS != m_mapTMAGICSKILL.end())
						(*itMS).second.push_back(pMSkill);
					else
					{
						VTMAGICSKILL vMSkill;
						vMSkill.clear();
						vMSkill.push_back(pMSkill);
						m_mapTMAGICSKILL.insert(MAPTMAGICSKILL::value_type(wKey, vMSkill));
					}
				}
			}
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLLevelChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTLEVEL pTLEVEL = new TLEVEL();

			pTLEVEL->m_bLevel = query->m_bLevel;
			pTLEVEL->m_dwEXP = query->m_dwEXP;
			pTLEVEL->m_dwHP = query->m_dwHP;
			pTLEVEL->m_dwMP = query->m_dwMP;
			pTLEVEL->m_bSkillPoint = query->m_bSkillPoint;
			pTLEVEL->m_dwMoney = query->m_dwMoney;
			pTLEVEL->m_dwRegCost = query->m_dwRegCost;
			pTLEVEL->m_dwSearchCost = query->m_dwSearchCost;
			pTLEVEL->m_dwGambleCost = query->m_dwGambleCost;
			pTLEVEL->m_dwRepCost = query->m_dwRepCost;
			pTLEVEL->m_dwRepairCost = query->m_dwRepairCost;
			pTLEVEL->m_dwRefineCost = query->m_dwRefineCost;
			pTLEVEL->m_wPvPoint = query->m_wPvPoint;
			pTLEVEL->m_dwPvPMoney = query->m_dwPvPMoney;
			m_mapTLEVEL.insert( MAPTLEVEL::value_type( query->m_bLevel, pTLEVEL));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLCashCategory)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTCASHCATEGORY pCategory = new TCASHCATEGORY();
			pCategory->m_bID = query->m_bID;
			pCategory->m_strName = query->m_szName;
			pCategory->m_vCashItem.clear();
			m_vCashCategory.push_back(pCategory);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLCashShopItem)
	BYTE bCategory = 0;
	query->m_item.Reset();
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTItem *pTITEM = CreateItem(query->m_item);
			if(query->m_item.m_wItemID && !pTITEM)
				continue;

			LPTCASHITEM pCashItem = new TCASHITEM();
			pCashItem->m_wID = query->m_wID;
			pCashItem->m_wInfoID = (WORD)query->m_wInfoID;
			pCashItem->m_dwMoney = query->m_dwMoney;
			pCashItem->m_bKind = query->m_bKind;
			pCashItem->m_bCanSell = query->m_bCanSell;
			pCashItem->m_bSaleValue = query->m_bSaleValue;
			pCashItem->m_wUseTime = query->m_wUseTime;
			pCashItem->m_pItem = pTITEM;
			pCashItem->m_dwOriMoney = query->m_dwMoney;

			LPTCASHCATEGORY pCategory = FindCashCategory(query->m_bCategory);
			if(pCategory)
				pCategory->m_vCashItem.push_back(pCashItem);

			m_mapTCashItem.insert(MAPTCASHITEM::value_type(pCashItem->m_wID, pCashItem));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLGamble)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTGAMBLE pGamble = new TGAMBLE();
			pGamble->m_bType = query->m_bType;
			pGamble->m_bKind = query->m_bKind;
			pGamble->m_wReplaceID = query->m_wReplaceID;
			pGamble->m_bCountMax = query->m_bCountMax;
			pGamble->m_bMinLevel = query->m_bMinLevel;
			pGamble->m_bMaxLevel = query->m_bMaxLevel;
			pGamble->m_wProb = query->m_wProb;

			WORD wGamKey = MAKEWORD(pGamble->m_bType, pGamble->m_bKind);
			MAPTGAMBLE::iterator it = m_mapTGAMBLE.find(wGamKey);
			if(it == m_mapTGAMBLE.end())
			{
				VTGAMBLE vGAM;
				vGAM.clear();
				vGAM.push_back(pGamble);
				m_mapTGAMBLE.insert(MAPTGAMBLE::value_type(wGamKey, vGAM));
			}
			else
			{
				(*it).second.push_back(pGamble);
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLClassChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTSTAT pTCLASS = new TSTAT();

			pTCLASS->m_wSTR = query->m_wSTR;
			pTCLASS->m_wDEX = query->m_wDEX;
			pTCLASS->m_wCON = query->m_wCON;

			pTCLASS->m_wINT = query->m_wINT;
			pTCLASS->m_wWIS = query->m_wWIS;
			pTCLASS->m_wMEN = query->m_wMEN;

			m_mapTCLASS.insert( MAPTSTAT::value_type( query->m_bClassID, pTCLASS));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLRaceChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTSTAT pTRACE = new TSTAT();

			pTRACE->m_wSTR = query->m_wSTR;
			pTRACE->m_wDEX = query->m_wDEX;
			pTRACE->m_wCON = query->m_wCON;

			pTRACE->m_wINT = query->m_wINT;
			pTRACE->m_wWIS = query->m_wWIS;
			pTRACE->m_wMEN = query->m_wMEN;

			m_mapTRACE.insert( MAPTSTAT::value_type( query->m_bRaceID, pTRACE));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLAICommand)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTAICOMMAND pCMD = new TAICOMMAND();

			pCMD->m_bType = query->m_bType;
			pCMD->m_dwID = query->m_dwID;
			pCMD->m_vCONDITION.clear();

			m_mapTCMDTEMP.insert( MAPTAICMDTEMP::value_type( pCMD->m_dwID, pCMD));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTAICMDTEMP::iterator itCMD;
	for( itCMD = m_mapTCMDTEMP.begin(); itCMD != m_mapTCMDTEMP.end(); itCMD++)
	{
		LPTAICOMMAND pCMD = (*itCMD).second;

		DEFINE_QUERY( &m_db, CTBLAICondition)
		query->m_dwCmdID = pCMD->m_dwID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				LPTAICONDITION pCON = new TAICONDITION();

				pCON->m_bType = query->m_bType;
				pCON->m_dwID = query->m_dwID;

				pCMD->m_vCONDITION.push_back(pCON);
			}

			query->Close();
		}
		UNDEFINE_QUERY()
	}

	DEFINE_QUERY( &m_db, CTBLAIChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTMonsterAI *pAI = FindTMonsterAI(query->m_bAIType);

			if(!pAI)
			{
				pAI = new CTMonsterAI();
				pAI->m_bAIType = query->m_bAIType;
				m_mapTMONAI.insert( MAPTMONSTERAI::value_type( pAI->m_bAIType, pAI));
			}

			MAPVTAICOMMAND::iterator itVCMD = pAI->m_mapVCOMMAND[query->m_bTriggerType].find(query->m_dwTriggerID);
			LPVTAICOMMAND pVCMD = NULL;

			if( itVCMD == pAI->m_mapVCOMMAND[query->m_bTriggerType].end() )
			{
				pVCMD = new VTAICOMMAND();
				pAI->m_mapVCOMMAND[query->m_bTriggerType].insert( MAPVTAICOMMAND::value_type( query->m_dwTriggerID, pVCMD));
			}
			else
				pVCMD = (*itVCMD).second;

			CTAICommand *pCMD = CTAICommand::CreateCMD(FindTCmdTEMP(query->m_dwCmdID));
			pVCMD->push_back(pCMD);

			pCMD->m_dwDelay = query->m_dwDelay;
			pCMD->m_bLoop = query->m_bLoop;
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLIndunChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTINDURN pIndurn = new TINDURN();
			pIndurn->m_wID = query->m_wID;
			pIndurn->m_wInSpawn = query->m_wInSpawn;
			pIndurn->m_wOutSpawn_D = query->m_wOutSpawn_D;
			pIndurn->m_wOutSpawn_C = query->m_wOutSpawn_C;
            m_mapTINDUN.insert(MAPTINDURN::value_type(query->m_wID, pIndurn));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLChannelList)
	if(query->Open())
	{
		while(query->Fetch())
			CTMap::m_vTCHANNEL.push_back(query->m_bChannel);

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLServerChart)
	query->m_bServerID = m_bServerID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(query->m_bChannel);
			CTChannel *pCH = NULL;

			if( itCH == m_mapTCHANNEL.end() )
			{
				pCH = new CTChannel();
				pCH->m_bChannel = query->m_bChannel;

				m_mapTCHANNEL.insert( MAPTCHANNEL::value_type( pCH->m_bChannel, pCH));
			}
			else
				pCH = (*itCH).second;

			MAPTMAP::iterator itMAP = pCH->m_mapTMAP.find(query->m_wMapID);
			CTMap *pMAP = NULL;

			if( itMAP == pCH->m_mapTMAP.end() )
			{
				pMAP = new CTMap();

				pMAP->m_bChannel = pCH->m_bChannel;
				pMAP->m_wMapID = query->m_wMapID;

				if(IsIndunMap(query->m_wMapID))
					pMAP->m_bType = MAP_INDUNTEMP;

				pCH->m_mapTMAP.insert( MAPTMAP::value_type( pMAP->m_wMapID, pMAP));
			}
			else
				pMAP = (*itMAP).second;

			pMAP->m_mapTUNIT.insert( MAPWORD::value_type( query->m_wUnitID, query->m_wUnitID));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTCHANNEL::iterator itCH;
	for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
		(*itCH).second->InitChannel( &m_db, m_bServerID);

	DEFINE_QUERY( &m_db, CTBLChannelChart)
	query->m_bServerID = m_bServerID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(query->m_bPhyChannel);

			if( itCH != m_mapTCHANNEL.end() )
				m_mapTLOGCHANNEL.insert( MAPTCHANNEL::value_type( MAKELONG( query->m_wMapID, WORD(query->m_bLogChannel)), (*itCH).second));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLSwitchChart)
	if(query->Open())
	{
		while(query->Fetch())
			for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
			{
				MAPTMAP::iterator itMAP = (*itCH).second->m_mapTMAP.find(query->m_wMapID);

				if( itMAP != (*itCH).second->m_mapTMAP.end() )
				{
					LPTSWITCH pTSWITCH = new TSWITCH();

					pTSWITCH->m_bLockOnClose = query->m_bLockOnClose;
					pTSWITCH->m_bLockOnOpen = query->m_bLockOnOpen;
					pTSWITCH->m_dwSwitchID = query->m_dwSwitchID;
					pTSWITCH->m_dwDuration = query->m_dwDuration;
					pTSWITCH->m_bOpened = query->m_bOpen;
					pTSWITCH->m_wMapID = query->m_wMapID;
					pTSWITCH->m_wPosX = query->m_wPosX;
					pTSWITCH->m_wPosY = query->m_wPosY;
					pTSWITCH->m_wPosZ = query->m_wPosZ;
					pTSWITCH->m_dwStartTime = 0;

					if(!(*itMAP).second->EnterMAP(pTSWITCH))
						delete pTSWITCH;
				}
			}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLGateChart)
	if(query->Open())
	{
		while(query->Fetch())
			for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
			{
				MAPTMAP::iterator itMAP = (*itCH).second->m_mapTMAP.find(query->m_wMapID);

				if( itMAP != (*itCH).second->m_mapTMAP.end() )
				{
					LPTSWITCH pTSWITCH = (*itMAP).second->FindSwitch(query->m_dwSwitchID);

					if(pTSWITCH)
					{
						MAPTGATE::iterator itGATE = (*itMAP).second->m_mapTGATE.find(query->m_dwGateID);

						if( itGATE == (*itMAP).second->m_mapTGATE.end() )
						{
							LPTGATE pTGATE = new TGATE();

							pTGATE->m_dwSwitchID = query->m_dwSwitchID;
							pTGATE->m_dwGateID = query->m_dwGateID;
							pTGATE->m_bOpened = pTSWITCH->m_bOpened;
							pTGATE->m_wMapID = query->m_wMapID;
							pTGATE->m_wPosX = query->m_wPosX;
							pTGATE->m_wPosY = query->m_wPosY;
							pTGATE->m_wPosZ = query->m_wPosZ;
							pTGATE->m_bType = query->m_bType;

							if(!(*itMAP).second->EnterMAP(pTGATE))
								delete pTGATE;
							else
							{
								pTGATE->m_vSwitch.push_back(pTSWITCH);
								pTSWITCH->m_vGate.push_back(pTGATE);
							}
						}
						else
						{
							(*itGATE).second->m_vSwitch.push_back(pTSWITCH);
							pTSWITCH->m_vGate.push_back((*itGATE).second);
						}
					}
				}
			}

		query->Close();
	}
	UNDEFINE_QUERY()

	FLOAT f1stRateX = 0;
	m_mapTFORMULA.clear();
	DEFINE_QUERY( &m_db, CTBLFormulaChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTFORMULA pTFORMULA = new TFORMULA();
			pTFORMULA->m_bID = query->m_bID;
			pTFORMULA->m_strName = query->m_szName;
			pTFORMULA->m_dwInit = query->m_dwInit;
			pTFORMULA->m_fRateX = query->m_fRateX;
			pTFORMULA->m_fRateY = query->m_fRateY;

			m_mapTFORMULA.insert(MAPTFORMULA::value_type(pTFORMULA->m_bID, pTFORMULA));

			if(pTFORMULA->m_bID == FTYPE_1ST)
				f1stRateX = pTFORMULA->m_fRateX;
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLSkillChart);
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTSkillTemp * pSkill = new CTSkillTemp();
			pSkill->m_wID = query->m_wID;
			pSkill->m_wPrevActiveID = query->m_wPrevActiveID;
			pSkill->m_wParentSkillID = query->m_wParentSkillID;
			pSkill->m_wMaxRange = query->m_wMaxRange;
			pSkill->m_wMinRange = query->m_wMinRange;
			pSkill->m_wPosture = query->m_wPosture;
			pSkill->m_dwConditionID = query->m_dwConditionID;
			pSkill->m_dwClassID = query->m_dwClassID;
			pSkill->m_bKind = query->m_bKind;
			pSkill->m_dwActionTime = query->m_dwActionTime;
			pSkill->m_dwDuration = query->m_dwDuration;
			pSkill->m_dwDurationInc = query->m_dwDurationInc;
			pSkill->m_dwReuseDelay = query->m_dwReuseDelay;
			pSkill->m_nReuseDelayInc = query->m_nReuseDelayInc;
			pSkill->m_dwLoopDelay = query->m_dwLoopDelay;
			pSkill->m_dwKindDelay = query->m_dwKindDelay;
			pSkill->m_dwAggro = query->m_dwAggro;
			pSkill->m_dwUseMP = query->m_dwUseMP;
			pSkill->m_dwUseHP = query->m_dwUseHP;
			pSkill->m_bStartLevel = query->m_bLevel;
			pSkill->m_bMaxLevel = query->m_bMaxLevel;
			pSkill->m_bNextLevel = query->m_bNextLevel;
			pSkill->m_bTarget = query->m_bTarget;
			pSkill->m_bTargetRange = query->m_bTargetRange;
			pSkill->m_bIsuse = query->m_bIsuse;
			pSkill->m_bTargetHit = query->m_bTargetHit;
			pSkill->m_fPrice = query->m_fPrice;
			pSkill->m_bPositive = query->m_bPositive;
			pSkill->m_bPriority = query->m_bPriority;
			pSkill->m_bSpeedApply = query->m_bSpeedApply;
			pSkill->m_dwWeapon = query->m_dwWeaponID;
			pSkill->m_bUseMPType = query->m_bUseMPType;
			pSkill->m_bUseHPType = query->m_bUseHPType;
			pSkill->m_bCanLearn = query->m_bCanLearn;
			pSkill->m_bORadius = query->m_bORadius;
			pSkill->m_bIsRide = query->m_bIsRide;
			pSkill->m_bIsDismount = query->m_bIsDismount;
			pSkill->m_wTargetActiveID = query->m_wTargetActiveID;
			pSkill->m_bMaintainType = query->m_bMaintainType;
			pSkill->m_bDuraSlot = query->m_bDuraSlot;
			pSkill->m_bCanCancel = query->m_bCanCancel;
			pSkill->m_bHitInit = query->m_bHitInit;
			pSkill->m_bHitInc = query->m_bHitInc;
			pSkill->m_bHitTest = query->m_bHitTest;
			pSkill->m_f1stRateX = f1stRateX;
			pSkill->m_wUseItem = query->m_wUseItem;
			pSkill->m_bGlobal = query->m_bGlobal;
			pSkill->m_bRadius = query->m_bRadius;
			pSkill->m_bStatic = query->m_bStatic;
			pSkill->m_bEraseAct = query->m_bEraseAct;
			pSkill->m_bEraseHide = query->m_bEraseHide;
			pSkill->m_bIsHideSkill = query->m_bIsHideSkill;
			pSkill->m_bRunFromServer = query->m_bRunFromServer;
			pSkill->m_bCheckAttacker = query->m_bCheckAttacker;
			pSkill->m_wTriggerID = query->m_wTriggerID;
			pSkill->m_wMapID = query->m_wMapID;

			m_mapTSKILL.insert(MAPTSKILLTEMP::value_type(pSkill->m_wID, pSkill));
			if(pSkill->m_bKind == NORMAL_EQUIP)
				m_mapTEQUIPSKILL.insert(MAPTSKILLTEMP::value_type(pSkill->m_wID, pSkill));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLSkillPoint)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTSKILLTEMP::iterator itST = m_mapTSKILL.find(query->m_wSkillID);
			if(itST == m_mapTSKILL.end())
				continue;

			LPTSKILLPOINT pSPoint = new TSKILLPOINT();
			pSPoint->m_wSkillID = query->m_wSkillID;
			pSPoint->m_bLevel = query->m_bLevel;
			pSPoint->m_bSkillPoint = query->m_bSkillPoint;
			pSPoint->m_bGroupPoint = query->m_bGroupPoint;
			pSPoint->m_bParentLevel = query->m_bParentLevel;
			pSPoint->m_dwPayback = query->m_dwPayback;
			(*itST).second->m_mapTSkillPoint.insert(MAPTSKILLPOINT::value_type(pSPoint->m_bLevel, pSPoint));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTSKILLTEMP::iterator itSkill;
	for(itSkill = m_mapTSKILL.begin(); itSkill != m_mapTSKILL.end(); itSkill++)
	{
		CTSkillTemp * pSkill = (*itSkill).second;
		DEFINE_QUERY(&m_db, CTBLSkillData);
		query->m_wSkillID = pSkill->m_wID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				LPTSKILLDATA pData = new TSKILLDATA();
				pData->m_bAction = query->m_bAction;
				pData->m_bType = query->m_bType;
				pData->m_bAttr = query->m_bAttr;
				pData->m_bExec = query->m_bExec;
				pData->m_bInc = query->m_bInc;
				pData->m_wValue = query->m_wValue;
				pData->m_wValueInc = query->m_wValueInc;
				pData->m_bCalc = query->m_bCalc;
				pSkill->m_vData.push_back(pData);
			}
			query->Close();
		}
		UNDEFINE_QUERY();
	}

	DEFINE_QUERY( &m_db, CTBLMonster)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTMONSTER pMON = new TMONSTER();

			pMON->m_pAI = FindTMonsterAI(query->m_bAIType);
			pMON->m_wID = query->m_wID;

			pMON->m_wKind = query->m_wKind;
			pMON->m_bRoamProb = query->m_bRoamProb;
			pMON->m_bRange = query->m_bRange;
			pMON->m_wChaseRange = query->m_wChaseRange;
			pMON->m_bClass = query->m_bClass;
			pMON->m_bLevel = query->m_bLevel;
			pMON->m_bRace = query->m_bRace;
			pMON->m_bMoneyProb = query->m_bMoneyProb;
			pMON->m_dwMinMoney = query->m_dwMinMoney;
			pMON->m_dwMaxMoney = query->m_dwMaxMoney;
			pMON->m_bItemProb = query->m_bItemProb;
			pMON->m_bDropCount = query->m_bDropCount;
			pMON->m_wExp = query->m_wExp;
			pMON->m_bRecallType = query->m_bRecallType;
			pMON->m_bIsSelf = query->m_bIsSelf;
			pMON->m_bCanSelect = query->m_bCanSelect;
			pMON->m_bCanAttack = query->m_bCanAttack;
			pMON->m_bTame = query->m_bTame;
			pMON->m_bCall = query->m_bCall;
			pMON->m_bRemove = query->m_bRemove;
			pMON->m_wTransSkillID = query->m_wTransSkillID;
			pMON->m_fSize = query->m_fSize;
			pMON->m_bIsSpecial = query->m_bIsSpecial;
			pMON->m_wMonAttr = query->m_wMonAttr;
			pMON->m_wSummonAttr = query->m_wSummonAttr;
			pMON->m_bCheckPartyQuest = FALSE;

			for(int i=0; i<MAX_MON_SKILL; i++)
				pMON->m_vSKILL.push_back(query->m_wSkill[i]);

			m_mapTMONSTER.insert( MAPTMONSTERTEMP::value_type( pMON->m_wID, pMON));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLMonAttr)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTMONATTR pMonAttr = new TMONATTR();
			pMonAttr->m_wID = query->m_wID;
			pMonAttr->m_bLevel = query->m_bLevel;
			pMonAttr->m_dwAttr = MAKELONG(pMonAttr->m_wID, pMonAttr->m_bLevel);
			pMonAttr->m_wAP = query->m_wAP;
			pMonAttr->m_wLAP = query->m_wLAP;
			pMonAttr->m_dwAtkSpeed = query->m_dwAtkSpeed;
			pMonAttr->m_wAL = query->m_wAL;
			pMonAttr->m_wMAL = query->m_wMAL;
			pMonAttr->m_wDL = query->m_wDL;
			pMonAttr->m_wMDL = query->m_wMDL;
			pMonAttr->m_bCriticalPP = query->m_bCriticalPP;
			pMonAttr->m_dwMaxHP = query->m_dwMaxHP;
			pMonAttr->m_bHPRecover = query->m_bHPRecover;
			pMonAttr->m_wMAP = query->m_wMAP;
			pMonAttr->m_bCriticalMP = query->m_bCriticalMP;
			pMonAttr->m_dwMaxMP = query->m_dwMaxMP;
			pMonAttr->m_bMPRecover = query->m_bMPRecover;
			pMonAttr->m_wDP = query->m_wDP;
			pMonAttr->m_wMDP = query->m_wMDP;
			pMonAttr->m_wMinWAP = query->m_wMinWAP;
			pMonAttr->m_wMaxWAP = query->m_wMaxWAP;
			pMonAttr->m_wWDP = query->m_wWDP;

			m_mapTMONATTR.insert(MAPTMONATTR::value_type(pMonAttr->m_dwAttr, pMonAttr));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLPet)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTMONSTERTEMP::iterator find = m_mapTMONSTER.find(query->m_wMonID);
			if(find==m_mapTMONSTER.end())
				continue;

			LPTPETTEMP pPET = new TPETTEMP();
			pPET->m_wID = query->m_wID;
			pPET->m_bPetType = query->m_bPetType;
			pPET->m_bRace = query->m_bRace;
			pPET->m_wMonID = query->m_wMonID;
			pPET->m_bConditionType = query->m_bConditionType;
			pPET->m_dwConditionValue = query->m_dwConditionValue;
			if(query->m_bRecallKind1)
				pPET->m_mapKind.insert(MAPBWORD::value_type(query->m_bRecallKind1, query->m_wRecallValue1));
			if(query->m_bRecallKind2)
				pPET->m_mapKind.insert(MAPBWORD::value_type(query->m_bRecallKind2, query->m_wRecallValue2));

			m_mapTPET.insert( MAPTPETTEMP::value_type(pPET->m_wID, pPET));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLQuestMagicItemChart)
	query->m_item.Reset();
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTItem * pItem = CreateItem(query->m_item);
			if(!pItem)
				continue;

			pItem->m_bCount = 1;
			pItem->m_dEndTime = query->m_wUseTime;
			pItem->m_bMagicDropLevel = query->m_bDropLevel;
			pItem->m_dwMoney = query->m_dwMoney;
            m_mapTQuestItem.insert(MAPTSTORAGEITEM::value_type(query->m_dwRewardID, pItem));			
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTMONSTERTEMP::iterator itMon;
	for(itMon = m_mapTMONSTER.begin(); itMon != m_mapTMONSTER.end(); itMon++)
	{
		DEFINE_QUERY(&m_db, CTBLMonItem)
		query->m_wMonID = (*itMon).second->m_wID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				LPTMONITEM pMonItem = new TMONITEM();
				pMonItem->m_bChartType = query->m_bChartType;

				if(!pMonItem->m_bChartType)
				{
					MAPTSTORAGEITEM::iterator itMI = m_mapTQuestItem.find(query->m_wItemID);
					if(itMI != m_mapTQuestItem.end())
						pMonItem->m_pMagicItem = (*itMI).second;
					else
						pMonItem->m_pMagicItem = NULL;
				}
				else
				{
					LPTITEM pTITEM = FindTItem(query->m_wItemID);
					if(pTITEM)
						pMonItem->m_pTITEM = pTITEM;
					else
						pMonItem->m_pTITEM = NULL;
				}

				pMonItem->m_wItemIDMin = query->m_wItemIDMin;
				pMonItem->m_wItemIDMax = query->m_wItemIDMax;
				pMonItem->m_wWeight	= query->m_wWeight;
				pMonItem->m_bLevelMin = query->m_bLevelMin;
				pMonItem->m_bLevelMax = query->m_bLevelMax;

				memcpy(pMonItem->m_bItemProb, query->m_bItemProb, sizeof(BYTE)*MIP_COUNT);
				pMonItem->m_bItemMagicOpt = query->m_bItemMagicOpt;
				pMonItem->m_bItemRareOpt = query->m_bItemRareOpt;
				(*itMon).second->m_dwMaxWeight += query->m_wWeight;
				(*itMon).second->m_vMONITEM.push_back(pMonItem);
			}
			query->Close();
		}
		UNDEFINE_QUERY();
	}

	m_mapExtraSpawnID.clear();
	for(WORD i = 0x8000; i < 0xFFFF; i++)
		m_mapExtraSpawnID.insert(MAPWORD::value_type(i,i));

	DEFINE_QUERY( &m_db, CTBLMonSpawn)
	MAPVMONSPAWN::iterator itSG;
	query->m_bServerID = m_bServerID;
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTMONSPAWN pSPAWN = new TMONSPAWN();

			pSPAWN->m_wID = query->m_wID;
			pSPAWN->m_lpvGroup = NULL;			
			pSPAWN->m_wGroup = 0;
			if(query->m_wGroup)
			{
				pSPAWN->m_wGroup = query->m_wGroup;
				itSG = m_mvSpawnGroup.find(query->m_wGroup);
				if(itSG!=m_mvSpawnGroup.end())
				{
					pSPAWN->m_lpvGroup = (*itSG).second;
					(*itSG).second->push_back(pSPAWN);
				}
				else
				{
					LPVTMONSPAWN pvGroup = new VTMONSPAWN();
					pvGroup->push_back(pSPAWN);
					pSPAWN->m_lpvGroup = pvGroup;
					m_mvSpawnGroup.insert(MAPVMONSPAWN::value_type(query->m_wGroup, pvGroup));
				}
			}

			pSPAWN->m_vMAPMON.clear();

			pSPAWN->m_wMapID = query->m_wMapID;
			pSPAWN->m_fPosX = query->m_fPosX;
			pSPAWN->m_fPosY = query->m_fPosY;
			pSPAWN->m_fPosZ = query->m_fPosZ;
			pSPAWN->m_wDir = query->m_wDir;
			pSPAWN->m_bCount = query->m_bCount;
			pSPAWN->m_bRange = query->m_bRange;
			pSPAWN->m_bArea = query->m_bArea;
			pSPAWN->m_bLink = query->m_bLink;
			pSPAWN->m_bProb = query->m_bProb;
			pSPAWN->m_bRoamType = query->m_bRoamType;
			pSPAWN->m_dwRegion = query->m_dwRegion;
			pSPAWN->m_dwDelay = query->m_dwDelay;
			pSPAWN->m_bEvent = query->m_bEvent;
			pSPAWN->m_wLocalID = query->m_wLocalID;
			pSPAWN->m_bCountry = query->m_bCountry;
			pSPAWN->m_wPartyID = query->m_wPartyID;

			m_mapTMONSPAWN.insert( MAPTMONSPAWNTEMP::value_type( pSPAWN->m_wID, pSPAWN));
			 
			MAPWORD::iterator itW = m_mapExtraSpawnID.find(pSPAWN->m_wID);
			if( itW != m_mapExtraSpawnID.end())
				m_mapExtraSpawnID.erase(itW);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLSpawnPath)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTMONSPAWNTEMP::iterator it = m_mapTMONSPAWN.find(query->m_wSpawnID);
			if(it!=m_mapTMONSPAWN.end())
			{
				LPTMONSPAWN pSpawn = (*it).second;
				if(pSpawn->m_vPathPoint.size() == 0)
				{
					LPTPATHPOINT pOriPath = new TPATHPOINT();
					pOriPath->m_fPosX = pSpawn->m_fPosX;
					pOriPath->m_fPosY = pSpawn->m_fPosY + 0.7f;
					pOriPath->m_fPosZ = pSpawn->m_fPosZ;
					pOriPath->m_bProb = 0;
					pOriPath->m_fRadius = query->m_fRadius;
					pSpawn->m_vPathPoint.push_back(pOriPath);
				}

				DWORD dwIndex = DWORD(pSpawn->m_vPathPoint.size()-1);
				DWORD dwDistX = DWORD(abs(pSpawn->m_vPathPoint[dwIndex]->m_fPosX - query->m_fPosX));
				DWORD dwDistZ = DWORD(abs(pSpawn->m_vPathPoint[dwIndex]->m_fPosZ - query->m_fPosZ));
				if(sqrt(double(dwDistX*dwDistX + dwDistZ*dwDistZ)) > MAX_ROAMRANGE-ZONE_HOMESIZE)
				{
					LPTPATHPOINT pPath = new TPATHPOINT();
					pPath->m_fPosX = min(pSpawn->m_vPathPoint[dwIndex]->m_fPosX, query->m_fPosX) + dwDistX/2;
					pPath->m_fPosY = query->m_fPosY;
					pPath->m_fPosZ = min(pSpawn->m_vPathPoint[dwIndex]->m_fPosZ, query->m_fPosZ) + dwDistZ/2;
					pPath->m_bProb = query->m_bProb;
					pPath->m_fRadius = query->m_fRadius;
					pSpawn->m_vPathPoint.push_back(pPath);
				}

				LPTPATHPOINT pPath = new TPATHPOINT();
				pPath->m_fPosX = query->m_fPosX;
				pPath->m_fPosY = query->m_fPosY;
				pPath->m_fPosZ = query->m_fPosZ;
				pPath->m_bProb = query->m_bProb;
				pPath->m_fRadius = query->m_fRadius;
				pSpawn->m_vPathPoint.push_back(pPath);
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLBattleZoneChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTBATTLEZONE pZone = new TBATTLEZONE();
			pZone->m_wID = query->m_wID;
			pZone->m_strName = query->m_szName;
			pZone->m_wMapID = query->m_wMapID;
			pZone->m_wCastle = query->m_wCastle;
			pZone->m_wBossSpawnID = query->m_wBossSpawnID;
			pZone->m_wLGateKeeperSpawnID = query->m_wLGateKeeperSpawnID;
			pZone->m_wRGateKeeperSpawnID = query->m_wRGateKeeperSpawnID;
			pZone->m_wCGateKeeperSpawnID = query->m_wCGateKeeperSpawnID;
			pZone->m_dwLSwitchID = query->m_dwLSwitchID;
			pZone->m_dwRSwitchID = query->m_dwRSwitchID;
			pZone->m_dwCSwitchID = query->m_dwCSwitchID;
			pZone->m_bLine = query->m_bLine;
			pZone->m_wSkill1 = query->m_wSkill1;
			pZone->m_wSkill2 = query->m_wSkill2;
			pZone->m_bItemLevel = query->m_bItemLevel;

			MAPTITEMTEMP::iterator itI = m_mapTITEM.find(query->m_wNormalItem);
			if(itI != m_mapTITEM.end())
				pZone->m_pNormalItem = (*itI).second;
			else
				pZone->m_pNormalItem = NULL;

			itI = m_mapTITEM.find(query->m_wChiefItem);
			if(itI != m_mapTITEM.end())
				pZone->m_pChiefItem = (*itI).second;
			else
				pZone->m_pChiefItem = NULL;

			pZone->m_vSpawnID.clear();
			
			m_mapTBATTLEZONE.insert(MAPTBATTLEZONE::value_type(pZone->m_wID, pZone));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLMissionTable)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTBATTLEZONE::iterator itBz = m_mapTBATTLEZONE.find(query->m_wMissionID);
			if(itBz == m_mapTBATTLEZONE.end())
				continue;

			LPTLOCAL pLocal = new TLOCAL();
			pLocal->m_bType = LOCAL_MISSION;
			pLocal->m_wLocal = query->m_wMissionID;
			pLocal->m_bStatus = BS_NORMAL;
			pLocal->m_bCountry = query->m_bCountry;
			pLocal->m_dwGuild = 0;
			pLocal->m_wDPoint = 0;
			pLocal->m_dwAtkGuildID = 0;
			pLocal->m_dwDefGuildID = 0;
			pLocal->m_wCPoint = 0;
			pLocal->m_bDefCount = 0;
			pLocal->m_bAtkCount = 0;
			pLocal->m_bDefCountry = TCONTRY_N;
			pLocal->m_strAtkName = NAME_NULL;
			pLocal->m_strDefName = NAME_NULL;
			pLocal->m_bCanBattle = TRUE;
			pLocal->m_bOccupied = FALSE;
			pLocal->m_mapTPoint.clear();

			pLocal->m_pZone = (*itBz).second;
			pLocal->m_bValid = FALSE;

			pLocal->m_timeNextDefend = 0;
			pLocal->m_strHero = NAME_NULL;  
			pLocal->m_timeHero = 0;

			m_vMission.push_back(pLocal);
			m_mapTLOCAL.insert(MAPTLOCAL::value_type(pLocal->m_wLocal, pLocal));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLCastleTable)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTBATTLEZONE::iterator itBz = m_mapTBATTLEZONE.find(query->m_wCastle);
			if(itBz == m_mapTBATTLEZONE.end())
				continue;

			LPTLOCAL pLocal = new TLOCAL();
			pLocal->m_bType = LOCAL_CASTLE;
			pLocal->m_wLocal = query->m_wCastle;
			pLocal->m_bStatus = BS_NORMAL;
			pLocal->m_bCountry = query->m_bCountry;
			pLocal->m_dwGuild = query->m_dwGuild;
			pLocal->m_wDPoint = 0;
			pLocal->m_dwAtkGuildID = 0;
			pLocal->m_dwDefGuildID = 0;
			pLocal->m_wCPoint = 0;
			pLocal->m_bDefCount = 0;
			pLocal->m_bAtkCount = 0;
			pLocal->m_strAtkName = NAME_NULL;
			pLocal->m_strDefName = NAME_NULL;
			pLocal->m_bCanBattle = FALSE;
			pLocal->m_bDefCountry = TCONTRY_N;
			pLocal->m_bLastOccType = OCCUPY_DEFEND;
			pLocal->m_mapTPoint.clear();

			if(query->IsNull(3))
				pLocal->m_strGuild = NAME_NULL;
			else
				pLocal->m_strGuild = query->m_szGuildName;

			pLocal->m_pZone = (*itBz).second;
			pLocal->m_bValid = FALSE;

			pLocal->m_timeNextDefend = __DBTOTIME(query->m_dateWarTime);
			pLocal->m_strHero = query->m_szHero;  
			pLocal->m_timeHero = __DBTOTIME(query->m_dateHero);

			m_vCastle.push_back(pLocal);
			m_mapTLOCAL.insert(MAPTLOCAL::value_type(pLocal->m_wLocal, pLocal));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLGodTower)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, query->m_wMapID);
			if(pMap)
			{
				LPTGODTOWER pTower = new TGODTOWER();
				pTower->m_wID = query->m_wID;
				pTower->m_fPosX = query->m_fPosX;
				pTower->m_fPosY = query->m_fPosY;
				pTower->m_fPosZ = query->m_fPosZ;
				pTower->m_vSpawnID.clear();
				pTower->m_pGodBall = NULL;
				pTower->m_pMAP = pMap;
				pMap->EnterMAP(pTower);
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLGodBall)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, query->m_wMapID);
			if(pMap)
			{
				LPTGODBALL pBall = new TGODBALL();
				pBall->m_wID = query->m_wID;
				pBall->m_bCamp = query->m_bCamp;
				pBall->m_dwGuildID = 0;
				pBall->m_dwKEY = 0;
				pBall->m_fPosX = query->m_fPosX;
				pBall->m_fPosY = query->m_fPosY;
				pBall->m_fPosZ = query->m_fPosZ;
				pBall->m_pMAP = pMap;
				pBall->m_bMove = TRUE;
				pMap->m_mapTTEMPGODBALL.insert(MAPTGODBALL::value_type(pBall->m_wID, pBall));
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLLocalTable)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTBATTLEZONE::iterator itBz = m_mapTBATTLEZONE.find(query->m_wLocal);
			if(itBz == m_mapTBATTLEZONE.end())
				continue;

			LPTLOCAL pLocal = new TLOCAL();
			pLocal->m_bType = LOCAL_OCCUPATION;
			pLocal->m_wLocal = query->m_wLocal;
			pLocal->m_bCountry = query->m_bCountry;
			pLocal->m_dwGuild = query->m_dwGuild;
			pLocal->m_mapTPoint.clear();

			if(query->IsNull(3))
				pLocal->m_strGuild = NAME_NULL;
			else
				pLocal->m_strGuild = query->m_szGuildName;

			pLocal->m_pZone = (*itBz).second;

			MAPTMONSPAWNTEMP::iterator itSp = m_mapTMONSPAWN.find(pLocal->m_pZone->m_wBossSpawnID);
			if(itSp==m_mapTMONSPAWN.end())
				pLocal->m_bValid = FALSE;
			else
				pLocal->m_bValid = TRUE;

			pLocal->m_bLastOccType = OCCUPY_DEFEND;
			pLocal->m_bOccupied = FALSE;
			pLocal->m_bGateOpened = FALSE;
			pLocal->m_bCanBattle = TRUE;
			pLocal->m_bStatus = BS_NORMAL;

			pLocal->m_timeOccupied = __DBTOTIME(query->m_dateOccupy);
			pLocal->m_timeNextDefend = __DBTOTIME(query->m_dateDefend);

			pLocal->m_strHero = query->m_szHero;  
			pLocal->m_timeHero = __DBTOTIME(query->m_dateHero);

			memset(pLocal->m_occupyGuild, 0, sizeof(pLocal->m_occupyGuild));
			memset(pLocal->m_occupyType, 0, sizeof(pLocal->m_occupyType));

			m_vOccupation.push_back(pLocal);
			m_mapTLOCAL.insert(MAPTLOCAL::value_type(pLocal->m_wLocal, pLocal));
			LPTLOCAL pCastle = FindCastle(pLocal->m_pZone->m_wCastle);
			if(pCastle)
				pCastle->m_mapLocal.insert(MAPTLOCAL::value_type(pLocal->m_wLocal, pLocal));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db, CTBLLocalOccupy)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTLOCAL pLocal = FindLocal(query->m_wLocalID);
			if(pLocal && query->m_bDay > 0)
			{
				pLocal->m_occupyGuild[query->m_bDay-1] = query->m_dwGuildID;
				pLocal->m_occupyType[query->m_bDay-1] = query->m_bType;
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	MAPTMONSPAWNTEMP::iterator itSPAWN;
	for( itSPAWN = m_mapTMONSPAWN.begin(); itSPAWN != m_mapTMONSPAWN.end(); itSPAWN++)
	{
		LPTMONSPAWN pSPAWN = (*itSPAWN).second;

		MAPTBATTLEZONE::iterator itBz = m_mapTBATTLEZONE.find(pSPAWN->m_wLocalID);
		if(itBz != m_mapTBATTLEZONE.end())
			(*itBz).second->m_vSpawnID.push_back(pSPAWN->m_wID);

		if(pSPAWN->m_wPartyID)
		{
			DEFINE_QUERY( &m_db, CTBLMonParty)
			query->m_wPartyID = pSPAWN->m_wPartyID;
			if(query->Open())
			{
				while(query->Fetch())
					pSPAWN->m_vPARTY.push_back(query->m_wSpawnID);
				query->Close();
			}
			UNDEFINE_QUERY();
		}

		DEFINE_QUERY( &m_db, CTBLMapMon)
		query->m_wSpawnID = pSPAWN->m_wID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				LPTMAPMON pMON = new TMAPMON();

				pMON->m_wSpawnID = query->m_wSpawnID;
				pMON->m_wMonID = query->m_wMonID;
				pMON->m_bLeader = query->m_bLeader;

				pMON->m_bEssential = query->m_bEssential;
				pMON->m_bProb = query->m_bProb;
				pSPAWN->m_vMAPMON.push_back(pMON);
			}

			query->Close();
		}
		UNDEFINE_QUERY()

		if(pSPAWN->m_bEvent == SE_DEFAULT)
			for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
				(*itCH).second->AddMonSpawn( pSPAWN, pSPAWN->m_bCountry);
	}

	DEFINE_QUERY( &m_db, CTBLQuestChart)
	query->m_dwParentID = 0;

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPQUESTTEMP pQUEST = new QUESTTEMP();

			pQUEST->m_dwTriggerID	= query->m_dwTriggerID;
			pQUEST->m_dwParentID	= query->m_dwParentID;
			pQUEST->m_dwQuestID		= query->m_dwQuestID;

			pQUEST->m_bTriggerType	= query->m_bTriggerType;
			pQUEST->m_bForceRun		= query->m_bForceRun;
			pQUEST->m_bCountMax		= query->m_bCountMax;
			pQUEST->m_bType			= query->m_bType;
			pQUEST->m_bConditionCheck = query->m_bConditionCheck;

			m_vQUESTTEMP.push_back(pQUEST);
			AddQuestTemp(pQUEST);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	for( int i=0; i<INT(m_vQUESTTEMP.size()); i++)
		LoadQuestTemp(m_vQUESTTEMP[i]);

	DEFINE_QUERY(&m_db, CTBLSpawnPos);
	if(query->Open())
	{
		MAPTCHANNEL::iterator itCH;

		while(query->Fetch())
		{
			LPTSPAWNPOS pPOS = new TSPAWNPOS();

			pPOS->m_wMapID = query->m_wMapID;
			pPOS->m_fPosX = query->m_fPosX;
			pPOS->m_fPosY = query->m_fPosY;
			pPOS->m_fPosZ = query->m_fPosZ;
			pPOS->m_bType = query->m_bType;
			pPOS->m_wID = query->m_wID;

			for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
			{
				MAPTMAP::iterator itMAP = (*itCH).second->m_mapTMAP.find(pPOS->m_wMapID);

				if( itMAP != (*itCH).second->m_mapTMAP.end() )
					(*itMAP).second->m_mapTSPAWNPOS.insert( MAPTSPAWNPOS::value_type( pPOS->m_wMapID, pPOS));
			}

			m_mapTSPAWNPOS.insert( MAPTSPAWNPOS::value_type( pPOS->m_wID, pPOS));
		}

		query->Close();
	}
	UNDEFINE_QUERY();

	DEFINE_QUERY(&m_db, CTBLArena)
	if(query->Open())
	{
		while(query->Fetch())
		{
			MAPTSPAWNPOS::iterator itPos = m_mapTSPAWNPOS.find(query->m_wInPos);
			if(itPos!=m_mapTSPAWNPOS.end())
			{
				if(!IsMainCell(DEFAULT_CHANNEL, itPos->second->m_wMapID, itPos->second->m_fPosX, itPos->second->m_fPosZ))
					continue;
			}
			else
				continue;

			LPTARENA pArena = new TARENA();
			pArena->m_wID = query->m_wID;
			pArena->m_bType = query->m_bType;
			pArena->m_dwFee = query->m_dwFee;
			pArena->m_bStatus = BS_NORMAL;
			pArena->m_dwTick = 0;
			pArena->m_bWinCount = 0;
			pArena->m_bDrawCount = 0;
			pArena->m_wWinPartyID = 0;

			for(BYTE i=0; i<2; i++)
				pArena->m_mapFighter[i].clear();

			pArena->m_pInPos = itPos->second;
			itPos = m_mapTSPAWNPOS.find(query->m_wOutPos);
			if(itPos != m_mapTSPAWNPOS.end())
				pArena->m_pOutPos = itPos->second;

			m_mapArena.insert(MAPARENA::value_type(pArena->m_wID, pArena));
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	VWORD vAuctionNpc;  
	vAuctionNpc.clear();

	DEFINE_QUERY( &m_db, CTBLNpc)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTNpc *pNPC = new CTNpc();

			pNPC->m_wID		 = query->m_wID;
			pNPC->m_bType	 = query->m_bType;
			pNPC->m_bCountry = query->m_bCountry;
			pNPC->m_pLocal   = query->m_wLocalID ? FindOccupationZone(query->m_wLocalID) : NULL;
			pNPC->m_bDiscountCondition = query->m_bDiscountCondition;
			pNPC->m_bDiscountRate = query->m_bDiscountRate;  
			pNPC->m_bAddProb      = query->m_bAddProb;			
			pNPC->m_wItemID = query->m_wItemID;
			pNPC->m_wMapID = query->m_wMapID;  
			pNPC->m_fPosX = query->m_fPosX;
			pNPC->m_fPosY = query->m_fPosY;
			pNPC->m_fPosZ = query->m_fPosZ;

			m_mapTNpc.insert( MAPTNPC::value_type( pNPC->m_wID, pNPC));

			if(pNPC->m_bType == TNPC_AUCTION)
			{				
				WORD wUnitID =  MAKEWORD( BYTE(INT(pNPC->m_fPosX) / UNIT_SIZE), BYTE(INT(pNPC->m_fPosZ) / UNIT_SIZE));
				CTMap * pAuMap = FindTMap(DEFAULT_CHANNEL, 0, pNPC->m_wMapID);
				if(pAuMap)
				{
					MAPWORD::iterator itUT = pAuMap->m_mapTUNIT.find(wUnitID);
					if(itUT != pAuMap->m_mapTUNIT.end())
						vAuctionNpc.push_back(pNPC->m_wID);
				}
			}
		}

		query->Close();
	}

	UNDEFINE_QUERY()
	DEFINE_QUERY(&m_db, CTBLPortalChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTPORTAL pPortal = new TPORTAL();
			pPortal->m_wPortalID = query->m_wPortalID;
			pPortal->m_wLocalID = query->m_wLocalID;
			pPortal->m_wSpawnID = query->m_wSpawnID;
			pPortal->m_bCondition = query->m_bCondition;
			pPortal->m_dwConditionID = 0;
			pPortal->m_bCountry = query->m_bCountry;
			MAPTLOCAL::iterator it = m_mapTLOCAL.find(query->m_wLocalID);
			if(it!=m_mapTLOCAL.end())
				pPortal->m_pLocal = (*it).second;
			else
				pPortal->m_pLocal = NULL;

			m_mapTPortal.insert(MAPTPORTAL::value_type(pPortal->m_wPortalID, pPortal));
		}
		query->Close();
	}
	UNDEFINE_QUERY();

	MAPTPORTAL::iterator itPt, itDest;
	for(itPt=m_mapTPortal.begin(); itPt!=m_mapTPortal.end(); itPt++)
	{
		DEFINE_QUERY(&m_db, CTBLDestinationChart)
		query->m_wPortalID = (*itPt).second->m_wPortalID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				itDest = m_mapTPortal.find(query->m_wDestID);
				if(itDest != m_mapTPortal.end())
				{
					LPTDESTINATION pDes = new TDESTINATION();
					pDes->m_wDestID = query->m_wDestID;					
					pDes->m_dwPrice = query->m_dwPrice;
					pDes->m_bEnable = query->m_bEnable;					
					memcpy(pDes->m_bCondition, query->m_bCondition, sizeof(BYTE)*PORTALCONDITION_COUNT);
					memcpy(pDes->m_dwConditionID, query->m_dwConditionID, sizeof(DWORD)*PORTALCONDITION_COUNT);
					pDes->m_pPortal = (*itDest).second;
					(*itPt).second->m_mapDestination.insert(MAPTDESTINATION::value_type(pDes->m_wDestID, pDes));
				}
			}
			query->Close();
		}
		UNDEFINE_QUERY()
	}

	MAPTNPC::iterator itNpc;
	for(itNpc=m_mapTNpc.begin(); itNpc!=m_mapTNpc.end(); itNpc++)
	{
		DEFINE_QUERY(&m_db, CTBLNpcItem)
		query->m_wNpcID = (*itNpc).second->m_wID;
		if(query->Open())
		{
			while(query->Fetch())
			{
				CTNpc * pNpc = (*itNpc).second;
				switch(pNpc->m_bType)
				{
				case TNPC_SKILL_RENT:
				case TNPC_SKILL_MASTER:
					{
						MAPTSKILLTEMP::iterator itSkillTemp = m_mapTSKILL.find((WORD)query->m_dwItemID);
						if(itSkillTemp != m_mapTSKILL.end())
							pNpc->m_mapSkill.insert(MAPTSKILLTEMP::value_type((WORD)query->m_dwItemID, (*itSkillTemp).second));
					}
					break;
				case TNPC_PVPOINT:
				case TNPC_ITEM:
					{
						MAPTITEMTEMP::iterator itItemTemp = m_mapTITEM.find((WORD)query->m_dwItemID);
						if(itItemTemp != m_mapTITEM.end())
							pNpc->m_mapItem.insert(MAPTITEMTEMP::value_type((WORD)query->m_dwItemID, (*itItemTemp).second));
					}
					break;
				case TNPC_PORTAL:
					{
						MAPTPORTAL::iterator itPt = m_mapTPortal.find(WORD(query->m_dwItemID));
						if(itPt != m_mapTPortal.end())
							pNpc->m_pPortal = (*itPt).second;
					}
					break;
				case TNPC_RETURN:
					{
						pNpc->m_wSpawnPosID = WORD(query->m_dwItemID);
					}
					break;
				case TNPC_ARENA:
					{
						WORD wUnitID =  MAKEWORD( BYTE(INT(pNpc->m_fPosX) / UNIT_SIZE), BYTE(INT(pNpc->m_fPosZ) / UNIT_SIZE));
						CTMap * pArenaMap = FindTMap(DEFAULT_CHANNEL, 0, pNpc->m_wMapID);
						if(pArenaMap)
						{
							MAPWORD::iterator itUT = pArenaMap->m_mapTUNIT.find(wUnitID);
							if(itUT != pArenaMap->m_mapTUNIT.end())
							{
								MAPARENA::iterator itArn = m_mapArena.find(WORD(query->m_dwItemID));
								if(itArn != m_mapArena.end())
									pNpc->m_pArena = itArn->second;
							}
						}
					}
					break;
				case TNPC_MAGICITEM:
					{
						MAPTSTORAGEITEM::iterator itItemTemp = m_mapTQuestItem.find(query->m_dwItemID);
						if(itItemTemp != m_mapTQuestItem.end())
						{
							CTItem* pMagicItem = (*itItemTemp).second;
							pNpc->m_mapMagicItem.insert(MAPTSTORAGEITEM::value_type(query->m_dwItemID, pMagicItem));
						}
					}
					break;
				}
			}
			query->Close();
		}
		UNDEFINE_QUERY();
	}

	DEFINE_QUERY(&m_db, CTBLMonsterShop)
	if(query->Open())
	{
		while(query->Fetch())
		{
			CTNpc * pNpc = FindTNpc(query->m_wNpcID);
			if(pNpc)
			{
				MAPTMONSTERSHOP::iterator itMS = pNpc->m_mapMon.find(query->m_wID);
				if(itMS == pNpc->m_mapMon.end())
				{
					MAPTMONSPAWNTEMP::iterator itSp = m_mapTMONSPAWN.find(query->m_wSpawnID);
					if(itSp!=m_mapTMONSPAWN.end())
					{
						BYTE bMonLevel = 0;
						if(!(*itSp).second->m_vMAPMON.empty())
						{
							LPTMONSTER pMapMon = FindTMonster((*itSp).second->m_vMAPMON[0]->m_wMonID);
							if(pMapMon)
								bMonLevel = pMapMon->m_bLevel;
						}

						if(query->m_wTowerID && bMonLevel > m_bMaxLevel)
							continue;

						LPTMONSTERSHOP pMon = new TMONSTERSHOP();
						pMon->m_wID = query->m_wID;
						pMon->m_dwPrice = query->m_dwPrice;
						pMon->m_wTowerID = query->m_wTowerID;
						pMon->m_pSpawn = (*itSp).second;
						pNpc->m_mapMon.insert(MAPTMONSTERSHOP::value_type(pMon->m_wID, pMon));

						if(pMon->m_wTowerID)
						{
							CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pMon->m_pSpawn->m_wMapID);
							if(pMap)
							{
								LPTGODTOWER pTower = pMap->FindGodTower(pMon->m_wTowerID);
								if(pTower)
									pTower->m_vSpawnID.push_back(query->m_wSpawnID);
							}
						}
					}
				}
			}
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	MAPQUESTTEMP::iterator itQu;
	for(itQu=m_mapQUESTTEMP.begin(); itQu!=m_mapQUESTTEMP.end(); itQu++)
	{
		LPQUESTTEMP pQuest = (*itQu).second;
		if(pQuest->m_bTriggerType == TT_TALKNPC)
		{
			MAPTNPC::iterator itNpc = m_mapTNpc.find((WORD)pQuest->m_dwTriggerID);
			if(itNpc != m_mapTNpc.end())
				(*itNpc).second->m_mapQuest.insert(MAPQUESTTEMP::value_type(pQuest->m_dwQuestID, pQuest));
		}
	}

	DEFINE_QUERY( &m_db, CTBLOperatorChart)
	if(query->Open())
	{
		while(query->Fetch())
            m_mapTOPERATOR.insert(MAPDWORD::value_type(query->m_dwOperator, query->m_dwOperator));

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLPvPointChart)
	if(query->Open())
	{
		while(query->Fetch())
		{
			LPTPVPOINT pPvP = new TPVPOINT();
			pPvP->m_wID = MAKEWORD(query->m_bStatus, query->m_bEvent);
			pPvP->m_bTarget = query->m_bTarget;
			pPvP->m_dwIncPoint = query->m_dwIncPoint;
			pPvP->m_dwDecPoint = query->m_dwDecPoint;
			WORD wLocalID = query->m_wLocalID;

			if(wLocalID)
			{
				LPTBATTLEZONE pBattle = FindBattleZone(query->m_wLocalID);
				if(pBattle)
					pBattle->m_mapTPvPoint.insert(MAPTPVPOINT::value_type(pPvP->m_wID, pPvP));
				else
					delete pPvP;
			}
			else
				m_mapTPvPointKill.insert(MAPTPVPOINT::value_type(pPvP->m_wID, pPvP));
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	
	MAPTAUCTIONBIDDER mapTAUCTIONBIDDER;
	mapTAUCTIONBIDDER.clear();

	DEFINE_QUERY(&m_db, CTBLALLAuctionBidder)
	if(query->Open())
	{
		while(query->Fetch())
		{
			TAUCTIONBIDDER stTAUCTIONBIDDER;
			stTAUCTIONBIDDER.m_dwAuctionID = query->m_dwAuctionID;
			stTAUCTIONBIDDER.m_dwCharID = query->m_dwCharID;
			stTAUCTIONBIDDER.m_ldwBidPrice = query->m_ldwBidPrice;
			stTAUCTIONBIDDER.m_dBidDate = __DBTOTIME(query->m_dBid);

			mapTAUCTIONBIDDER.insert(MAPTAUCTIONBIDDER::value_type(stTAUCTIONBIDDER.m_dwAuctionID, stTAUCTIONBIDDER));			
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	for(WORD w = 0; w < (WORD)vAuctionNpc.size(); w++)  
	{	
		__time64_t CurTime = CTime::GetCurrentTime().GetTime();
		DEFINE_QUERY(&m_db, CTBLAuctionTable)
		query->m_wNpcID = vAuctionNpc[w];
		if(query->Open())
		{
			while(query->Fetch())
			{			
				CTNpc * pNpc = FindTNpc(query->m_wNpcID);
				if(pNpc)
				{
					LPTAUCTION pAuction = new TAUCTION();
					pAuction->m_wNpcID = query->m_wNpcID;
					pAuction->m_bBidCount = query->m_bBidCount;
					pAuction->m_dwAuctionID = query->m_dwAuctionID;
					pAuction->m_dwSeller = query->m_dwSeller;
					pAuction->m_strSeller = query->m_szSeller;
					pAuction->m_ldwDirectPrice = query->m_ldwDirectPrice;
					pAuction->m_ldwStartPrice =	query->m_ldwStartPrice;
					pAuction->m_ldwBidPrice = 0;
					pAuction->m_dStart = __DBTOTIME(query->m_dStart);
					pAuction->m_dEnd = __DBTOTIME(query->m_dEnd);				

					if( pAuction->m_dEnd > CurTime)
						pAuction->m_dwRemainTime = (DWORD)(pAuction->m_dEnd - CurTime);
					else
						pAuction->m_dwRemainTime = 0;

					MAPTAUCTIONBIDDER::iterator itB = mapTAUCTIONBIDDER.find(query->m_dwAuctionID);
					if(itB != mapTAUCTIONBIDDER.end())
					{
						pAuction->m_ldwBidPrice = (*itB).second.m_ldwBidPrice;
						pAuction->m_dBid = (*itB).second.m_dBidDate;
						pAuction->m_dwBidder = (*itB).second.m_dwCharID;
					}
					else
					{
						pAuction->m_ldwBidPrice = pAuction->m_ldwStartPrice;
						pAuction->m_dwBidder = 0;
					}


					CTItem *pTITEM = CreateItem(query->m_item);				
					if(!pTITEM)
					{
						delete pAuction;
						LogEvent("Auction Item Error: %d",pAuction->m_dwAuctionID);
						continue;
					}

					pAuction->m_dwCode = pTITEM->m_pTITEM->m_dwCode;
					pTITEM->m_dlID = query->m_item.m_dlID;
					pAuction->m_pItem = pTITEM;

					if(pNpc->AddAuction(pAuction->m_dwAuctionID,pAuction))
						SendSM_AUCTIONCMD_REQ(pAuction->m_wNpcID,pAuction->m_dwAuctionID,pAuction->m_dEnd);
				}
				else			
					LogEvent(_T("Invalid Auction NPC"));			
			}
			query->Close();
		}
		UNDEFINE_QUERY()
	}
	vAuctionNpc.clear();

	BYTE bMonthRank = TOTALMONTHRANKCOUNT;
	DEFINE_QUERY(&m_db,CTBLMonthRankTable)	
	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_bMonthRank >= FAMERANKCOUNT || query->m_bMonthRank == bMonthRank)
				continue;

			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwCharID = query->m_dwCharID;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwMonthRank = query->m_bMonthRank;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwTotalRank = query->m_dwTotalRank;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwTotalPoint = query->m_dwTotalPoint;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwMonthPoint = query->m_dwMonthPoint;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_wMonthWin = query->m_wMonthWin;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_wMonthLose = query->m_wMonthLose;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwTotalWin = query->m_dwTotalWin;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_dwTotalLose = query->m_dwTotalLose;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bCountry = query->m_bCountry;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bLevel = query->m_bLevel;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bClass = query->m_bClass;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bRace = query->m_bRace;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bSex = query->m_bSex;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bFace = query->m_bFace;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_bHair = query->m_bHair;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_strName = query->m_szName;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_strSay = query->m_szSay;
			m_arFameRank[FRT_HERO][query->m_bMonth][query->m_bMonthRank].m_strGuild = query->m_szGuild;

			bMonthRank = query->m_bMonthRank;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CTBLHeroTable)	
	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_bType >= FRT_COUNT || query->m_bMonth >= MONTHCOUNT || query->m_bOrder >= FAMERANKCOUNT)
				continue;

			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwCharID = query->m_dwCharID;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwMonthRank = query->m_dwMonthRank;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwTotalRank = query->m_dwTotalRank;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwTotalPoint = query->m_dwTotalPoint;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwMonthPoint = query->m_dwMonthPoint;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_wMonthWin = query->m_wMonthWin;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_wMonthLose = query->m_wMonthLose;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwTotalWin = query->m_dwTotalWin;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_dwTotalLose = query->m_dwTotalLose;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bCountry = query->m_bCountry;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bLevel = query->m_bLevel;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bClass = query->m_bClass;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bRace = query->m_bRace;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bSex = query->m_bSex;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bFace = query->m_bFace;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_bHair = query->m_bHair;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_strName = query->m_szName;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_strSay = query->m_szSay;
			m_arFameRank[query->m_bType][query->m_bMonth][query->m_bOrder].m_strGuild = query->m_szGuild;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY(&m_db,CTBLFirstGradeGroup)
	CTime t = CTime::GetCurrentTime().GetTime();
	BYTE bRankMonth = t.GetMonth();
	bRankMonth--;
	if(bRankMonth == 0)
		bRankMonth = 12;

	query->m_bMonth = bRankMonth;
	
	if(query->Open())
	{
		while(query->Fetch())
		{
			if(query->m_bCountry >= COUNTRY_COUNT || query->m_bRank >= FIRSTGRADEGROUPCOUNT)
				continue;

			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwMonthRank = query->m_bRank;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwCharID = query->m_dwCharID;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_strName = query->m_szName;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwTotalPoint = query->m_dwTotalPoint;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwMonthPoint = query->m_dwMonthPoint;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_wMonthWin = query->m_wMonthWin;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_wMonthLose = query->m_wMonthLose;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwTotalWin = query->m_dwTotalWin;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_dwTotalLose = query->m_dwTotalLose;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bCountry = query->m_bCountry;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bLevel = query->m_bLevel;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bClass = query->m_bClass;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bRace = query->m_bRace;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bSex = query->m_bSex;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bFace = query->m_bFace;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_bHair = query->m_bHair;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_strSay = query->m_szSay;
			m_arFirstGradeGroup[query->m_bCountry][query->m_bRank].m_strGuild = query->m_szGuild;
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

void CTMapSvrModule::LoadQuestTemp( LPQUESTTEMP pQUEST)
{
	DEFINE_QUERY( &m_db, CTBLQuestChart)
	query->m_dwParentID = pQUEST->m_dwQuestID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPQUESTTEMP pCHILD = new QUESTTEMP();

			pCHILD->m_dwTriggerID	= query->m_dwTriggerID;
			pCHILD->m_dwParentID	= query->m_dwParentID;
			pCHILD->m_dwQuestID		= query->m_dwQuestID;

			pCHILD->m_bTriggerType	= query->m_bTriggerType;
			pCHILD->m_bForceRun		= query->m_bForceRun;
			pCHILD->m_bCountMax		= query->m_bCountMax;
			pCHILD->m_bType			= query->m_bType;
			pCHILD->m_bConditionCheck = query->m_bConditionCheck;

			pQUEST->m_mapCHILD.insert( MAPQUESTTEMP::value_type( pCHILD->m_dwQuestID, pCHILD));
			AddQuestTemp(pCHILD);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLQuestConditionChart)
	query->m_dwQuestID = pQUEST->m_dwQuestID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPQUESTCONDITION pCONDITION = new QUESTCONDITION();

			pCONDITION->m_dwConditionID	= query->m_dwConditionID;
			pCONDITION->m_bConditionType= query->m_bConditionType;
			pCONDITION->m_bCount		= query->m_bCount;

			pQUEST->m_vCondition.push_back(pCONDITION);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	DEFINE_QUERY( &m_db, CTBLQuestRewardChart)
	query->m_dwQuestID = pQUEST->m_dwQuestID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPQUESTREWARD pREWARD = new QUESTREWARD();

			pREWARD->m_dwRewardID = query->m_dwRewardID;
			pREWARD->m_bRewardType = query->m_bRewardType;
			pREWARD->m_bTakeMethod = query->m_bTakeMethod;
			pREWARD->m_bTakeData = query->m_bTakeData;
			pREWARD->m_bCount = query->m_bCount;

			pQUEST->m_vReward.push_back(pREWARD);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	for(DWORD i=0; i<pQUEST->m_vReward.size(); i++)
	{
		LPQUESTREWARD pREWARD = pQUEST->m_vReward[i];

		if(pREWARD->m_bRewardType == RT_MAGICITEM)
		{
			MAPTSTORAGEITEM::iterator itQI = m_mapTQuestItem.find(pREWARD->m_dwRewardID);
			if(itQI != m_mapTQuestItem.end())
				pREWARD->m_pMagicItem = (*itQI).second;
		}
	}

	DEFINE_QUERY( &m_db, CTBLQuestTermChart)
	query->m_dwQuestID = pQUEST->m_dwQuestID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			LPQUESTTERM pTERM = new QUESTTERM();

			pTERM->m_dwTermID = query->m_dwTermID;
			pTERM->m_bTermType = query->m_bTermType;
			pTERM->m_bCount = query->m_bCount;

			pQUEST->m_vTerm.push_back(pTERM);

			if(pQUEST->m_bType == QT_REGEN && pQUEST->m_bTriggerType == TT_KILLMON && pTERM->m_bTermType == QTT_MONID)
			{
				LPTMONSTER pMon = FindTMonster(WORD(pQUEST->m_dwTriggerID));
				if(pMon)
					pMon->m_bCheckPartyQuest = TRUE;
			}
		}

		query->Close();
	}
	UNDEFINE_QUERY()
	MAPQUESTTEMP::iterator it;

	for( it = pQUEST->m_mapCHILD.begin(); it != pQUEST->m_mapCHILD.end(); it++)
		LoadQuestTemp((*it).second);
}

void CTMapSvrModule::AddQuestTemp( LPQUESTTEMP pQUEST)
{
	m_mapQUESTTEMP.insert( MAPQUESTTEMP::value_type( pQUEST->m_dwQuestID, pQUEST));

	MAPMAPVQUESTTEMP::iterator itTYPE = m_mapTRIGGER.find(pQUEST->m_bTriggerType);
	LPMAPVQUESTTEMP pMAPVQUEST = NULL;

	if( itTYPE == m_mapTRIGGER.end() )
	{
		pMAPVQUEST = new MAPVQUESTTEMP();
		pMAPVQUEST->clear();

		m_mapTRIGGER.insert( MAPMAPVQUESTTEMP::value_type( pQUEST->m_bTriggerType, pMAPVQUEST));
	}
	else
		pMAPVQUEST = (*itTYPE).second;

	MAPVQUESTTEMP::iterator itTRIGGER = pMAPVQUEST->find(pQUEST->m_dwTriggerID);
	LPVQUESTTEMP pVQUEST = NULL;

	if( itTRIGGER == pMAPVQUEST->end() )
	{
		pVQUEST = new VQUESTTEMP();
		pVQUEST->clear();

		pMAPVQUEST->insert( MAPVQUESTTEMP::value_type( pQUEST->m_dwTriggerID, pVQUEST));
	}
	else
		pVQUEST = (*itTRIGGER).second;
	pVQUEST->push_back(pQUEST);
}

DWORD CTMapSvrModule::ControlThread()
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

			case COMP_CLOSE		: OnCloseSession((CTMapSession *) pOV); break;
			}
		}
		else if( dwCompKey == COMP_EXIT )
			return 0;
	}

	return 0;
}

DWORD CTMapSvrModule::WorkThread()
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
					CTMapSession *pSession = (CTMapSession *) ((COverlappedEx *) pOV)->m_pOBJ;
					BYTE bTYPE = ((COverlappedEx *) pOV)->m_bTYPE;

					switch(bTYPE)
					{
					case TOV_SSN_RECV	:
						if(pSession->m_bSessionType == SESSION_SERVER)
						{
							LogEvent("WorkThread CompletionStatus %d\n", GetLastError());
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
						break;
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
					CTMapSession *pSession = (CTMapSession *) ((COverlappedEx *) pOV)->m_pOBJ;

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

DWORD CTMapSvrModule::BatchThread()
{
	srand( (unsigned)CTime::GetCurrentTime().GetTime() );

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

			if(dwResult && pBUF->m_pSESSION)
			{
//				LogEvent("Batch Close CloseSession %d, %d\n", dwResult, pBUF->m_packet.GetID());
				CloseSession((CTMapSession *) pBUF->m_pSESSION);
			}
			LeaveCriticalSection(&m_csBATCH);

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTMapSvrModule::DBThread()
{
	while(TRUE)
	{
		EnterCriticalSection(&m_csDBQUEUE);
		if(!m_bDBRun && m_qDBJOB.empty())
		{
			LeaveCriticalSection(&m_csDBQUEUE);
			break;
		}

		if(m_qDBJOB.empty())
		{
			LeaveCriticalSection(&m_csDBQUEUE);
			WaitForSingleObject( m_hDBEvent, INFINITE);
		}
		else
		{
			LPPACKETBUF pBUF = m_qDBJOB.front();
			m_qDBJOB.pop();
			LeaveCriticalSection(&m_csDBQUEUE);

			DWORD dwResult = OnReceive(pBUF);
			if(dwResult)
				OnERROR(dwResult);

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTMapSvrModule::AIThread()
{
	DWORD dwPrevTick = GetTickCount();
	srand( (unsigned)CTime::GetCurrentTime().GetTime() );

	while(TRUE)
	{
		m_dwTick = GetTickCount();
		m_timeCurrent = CTime::GetCurrentTime().GetTime();		

		if( dwPrevTick != m_dwTick )
		{
			OnTimer(m_dwTick - dwPrevTick);
			dwPrevTick = m_dwTick;
		}

		EnterCriticalSection(&m_csAIQUEUE);
		if(!m_bAIRun)
		{
			LeaveCriticalSection(&m_csAIQUEUE);
			break;
		}

		if(m_qAIJOB.empty())
		{
			LeaveCriticalSection(&m_csAIQUEUE);
			WaitForSingleObject( m_hAIEvent, MAP_TIMER);
		}
		else
		{
			LPPACKETBUF pBUF = m_qAIJOB.front();
			m_qAIJOB.pop();
			LeaveCriticalSection(&m_csAIQUEUE);

			DWORD dwResult = OnReceive(pBUF);
			if(dwResult)
				OnERROR(dwResult);

			delete pBUF;
		}
	}

	return 0;
}

DWORD CTMapSvrModule::LogThread()
{
	__int64 ldwPrevTime = m_timeCurrent;

	while(TRUE)
	{
		if(ldwPrevTime != m_timeCurrent )
		{
			AuctionTimeCheck();
			ldwPrevTime = m_timeCurrent;
		}		
		
		EnterCriticalSection(&m_csLOGQUEUE);
		if(!m_bLogRun && m_qLOGJOB.empty())
		{
			LeaveCriticalSection(&m_csLOGQUEUE);
			break;
		}

		if(m_qLOGJOB.empty())
		{
			LeaveCriticalSection(&m_csLOGQUEUE);
			WaitForSingleObject( m_hLogEvent, MAP_TIMER);
		}
		else
		{
			LPPACKETBUF pBUF = m_qLOGJOB.front();
			m_qLOGJOB.pop();
			LeaveCriticalSection(&m_csLOGQUEUE);

			DWORD dwResult = OnReceive(pBUF);
			if(dwResult)
				OnERROR(dwResult);

			delete pBUF;
		}
	}

	return 0;
}

void CTMapSvrModule::OnSendComplete( CTMapSession *pSession, DWORD dwIoBytes)
{
	if(pSession->SendComplete(dwIoBytes))
	{
		if(pSession->m_bSessionType == SESSION_SERVER)
			LogEvent("OnSendComplete Valid %d, %d\n", pSession->m_bValid, dwIoBytes);

		ClosingSession(pSession);
	}
}

void CTMapSvrModule::ProcessSession( CTMapSession *pSession, DWORD dwIoBytes)
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
		if(pSession->m_bSessionType == SESSION_SERVER)
			LogEvent("Process Read %d, %d\n", dwIoBytes, pSession->m_Recv.m_dwReadBytes);
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
			if(pSession->m_bSessionType == SESSION_SERVER)
			{
				LogEvent("Process Packet Invalid %d, %d, %d, %d\n", dwResult, pSession->m_Recv.GetID(), pSession->m_Recv.GetReadBytes(), pSession->m_Recv.GetSize());
				pSession->Flush();
				break;
			}
			else
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
		if(pSession->m_bSessionType == SESSION_SERVER)
			LogEvent("WaitForMessage %d, %d, %d, %d\n", pSession->m_sock, pSession->m_bValid, pSession->m_bCanRecv, WSAGetLastError());
		OnInvalidSession(pSession);
	}
}

DWORD CTMapSvrModule::OnReceive( LPPACKETBUF pBUF)
{
	if(pBUF->m_packet.GetSize() == MAX_PACKET_SIZE)
	{
		LogEvent("Overflow Message %d",pBUF->m_packet.GetID());
		return EC_SESSION_INVALIDMSG;
	}

	WORD wMsgID = pBUF->m_packet.GetID();

	switch(wMsgID)
	{
	// Control Server Message
	ON_RECEIVE(CT_SERVICEMONITOR_ACK)
	ON_RECEIVE(CT_ANNOUNCEMENT_ACK) // 현승룡 공지사항
	ON_RECEIVE(CT_USERKICKOUT_ACK) // 현승룡 유저 강제퇴장
	ON_RECEIVE(CT_USERMOVE_ACK) // 현승룡 유저 위치이동
	ON_RECEIVE(CT_MONSPAWNFIND_ACK) // 현승룡 몬스터 관리
	ON_RECEIVE(CT_MONACTION_ACK)
	ON_RECEIVE(CT_SERVICEDATACLEAR_ACK)
	ON_RECEIVE(CT_CTRLSVR_REQ)
	ON_RECEIVE(CT_CASTLEINFO_REQ)

	// AI request message handler (Execute on AI thread) - Implemented on SSHandler.cpp
	ON_RECEIVE(SM_AICMD_REQ)
	ON_RECEIVE(SM_SWITCHSTART_REQ)
	ON_RECEIVE(SM_POSTBILL_REQ)
	ON_RECEIVE(SM_POSTBILLERASE_REQ)
	ON_RECEIVE(SM_DUELSTART_REQ)
	ON_RECEIVE(SM_DUELEND_REQ)
	ON_RECEIVE(SM_RESETHOST_REQ)
	ON_RECEIVE(SM_GODBALLCMD_REQ)
	ON_RECEIVE(SM_AUCTIONCMD_REQ) 
	ON_RECEIVE(SM_AUCTIONCMDDEL_REQ)
	ON_RECEIVE(SM_MEETINGROOM_REQ)

	// AI acknowledge message handler (Execute on BATCH thread) - Implemented on SSHandler.cpp
	ON_RECEIVE(SM_AICMD_ACK)
	ON_RECEIVE(SM_POSTBILL_ACK)
	ON_RECEIVE(SM_SWITCHCHANGE_REQ)
	ON_RECEIVE(SM_DUELSTART_ACK)
	ON_RECEIVE(SM_DUELEND_ACK)
	ON_RECEIVE(SM_DUELCOMPLETE_ACK)
	ON_RECEIVE(SM_RESETHOST_ACK)
	ON_RECEIVE(SM_GODBALLCMD_ACK)
	ON_RECEIVE(SM_MEETINGROOM_ACK)

	// System message handler - Implemented on SSHandler.cpp
	ON_RECEIVE(SM_QUITSERVICE_REQ)
	ON_RECEIVE(SM_DELSESSION_REQ)
	ON_RECEIVE(SM_TIMER_REQ)

	// DB request message handler (Execute on DB thread) - Implemented on SSHandler.cpp
	ON_RECEIVE(DM_ENTERMAPSVR_REQ)
	ON_RECEIVE(DM_LOADCHAR_REQ)
	ON_RECEIVE(DM_LOADPOSTMESSAGE_REQ)
	ON_RECEIVE(DM_SAVECHAR_REQ)
	ON_RECEIVE(DM_SAVEITEM_REQ)
	ON_RECEIVE(DM_LOGOUT_REQ)
	ON_RECEIVE(DM_ROUTE_REQ)
	ON_RECEIVE(DM_CABINETITEM_REQ)
	ON_RECEIVE(DM_POSTRECV_REQ)
	ON_RECEIVE(DM_POSTCANSEND_REQ)
	ON_RECEIVE(DM_POSTDEL_REQ)
	ON_RECEIVE(DM_POSTLIST_REQ)
	ON_RECEIVE(DM_POSTGETITEM_REQ)
	ON_RECEIVE(DM_LOCALOCCUPY_REQ)
	ON_RECEIVE(DM_CASTLEOCCUPY_REQ)
	ON_RECEIVE(DM_MISSIONOCCUPY_REQ)
	ON_RECEIVE(DM_CLEARCURRENTUSER_REQ)
	ON_RECEIVE(DM_CREATERECALLMON_REQ)
	ON_RECEIVE(DM_RECALLMONDEL_REQ)
	ON_RECEIVE(DM_PROTECTEDADD_REQ)
	ON_RECEIVE(DM_PROTECTEDERASE_REQ)
	ON_RECEIVE(DM_LOGSKILL_REQ)
	ON_RECEIVE(DM_PETDEL_REQ)
	ON_RECEIVE(DM_TELEPORT_REQ)
	ON_RECEIVE(DM_SMSSEND_REQ)
	ON_RECEIVE(DM_EVENTQUARTER_REQ)
	ON_RECEIVE(DM_CASHITEMCABINET_REQ)
	ON_RECEIVE(DM_CASHITEMGET_REQ)
	ON_RECEIVE(DM_CASHSHOPITEMLIST_REQ)
	ON_RECEIVE(DM_CASHITEMBUY_REQ)
	ON_RECEIVE(DM_ACTEND_REQ)
	ON_RECEIVE(DM_CASHCABINETBUY_REQ)
	ON_RECEIVE(DM_CASHITEMPUTIN_REQ)
	ON_RECEIVE(DM_DELETEDEALITEM_REQ)
	ON_RECEIVE(DM_SAVECHARBASE_REQ)
	ON_RECEIVE(DM_CHECKCHANGENAME_REQ)
	ON_RECEIVE(DM_HEROSELECT_REQ)  
	ON_RECEIVE(DM_STOPTHECLOCK_REQ)
	ON_RECEIVE(DM_QUESTSENDPOST_REQ)
	ON_RECEIVE(DM_GAINCASHBONUS_REQ)
	ON_RECEIVE(DM_MONTHPVPOINTRESET_REQ)
	ON_RECEIVE(DM_POSTVIEW_REQ)
	ON_RECEIVE(DM_POSTBILL_REQ)
	ON_RECEIVE(DM_POSTBILLUPDATE_REQ)
	ON_RECEIVE(DM_TOURNAMENTREWARD_REQ)
	ON_RECEIVE(DM_SAVECHARPOSITION_REQ)

	//////////////////////////////////////////
	//길드
	ON_RECEIVE(DM_GUILDCABINETPUTIN_REQ)
	ON_RECEIVE(DM_GUILDCABINETTAKEOUT_REQ)
	ON_RECEIVE(DM_GUILDCABINETROLLBACK_REQ)
	//////////////////////////////////////////
	ON_RECEIVE(DM_SAVEEXP_REQ)
	ON_RECEIVE(DM_SAVEMONEY_REQ)
	ON_RECEIVE(DM_POSTINVENITEM_REQ)
	ON_RECEIVE(DM_AUCTIONREG_REQ) 
	ON_RECEIVE(DM_AUCTIONEND_REQ) 
	ON_RECEIVE(DM_AUCTIONREGCANCEL_REQ)
	ON_RECEIVE(DM_AUCTIONBID_REQ)
	ON_RECEIVE(DM_AUCTIONBUYDIRECT_REQ)
	ON_RECEIVE(DM_WARLORDSAY_REQ)  

#ifdef	DEF_UDPLOG
	ON_RECEIVE(DM_UDPLOG_REQ)	
#endif

	// DB acknowledge message handler (Execute on BATCH thread) - Implemented on SSHandler.cpp
	ON_RECEIVE(DM_DORELEASEMAIN_ACK)
	ON_RECEIVE(DM_ENTERMAPSVR_ACK)
	ON_RECEIVE(DM_LOADCHAR_ACK)
	ON_RECEIVE(DM_LOADPOSTMESSAGE_ACK)
	ON_RECEIVE(DM_ROUTE_ACK)
	ON_RECEIVE(DM_CABINETITEM_ACK)
	ON_RECEIVE(DM_POSTCANSEND_ACK)
	ON_RECEIVE(DM_POSTLIST_ACK)
	ON_RECEIVE(DM_LOCALOCCUPY_ACK)
	ON_RECEIVE(DM_MISSIONOCCUPY_ACK)
	ON_RECEIVE(DM_CREATERECALLMON_ACK)
	ON_RECEIVE(DM_TELEPORT_ACK)
	ON_RECEIVE(DM_CLEARCURRENTUSER_ACK)
	ON_RECEIVE(DM_PROTECTEDADD_ACK)
	ON_RECEIVE(DM_POSTRECV_ACK)
	ON_RECEIVE(DM_SMSSEND_ACK)
	ON_RECEIVE(DM_CASHITEMCABINET_ACK)
	ON_RECEIVE(DM_CASHITEMGET_ACK)
	ON_RECEIVE(DM_CASHSHOPITEMLIST_ACK)
	ON_RECEIVE(DM_CASHITEMBUY_ACK)
	ON_RECEIVE(DM_CASHCABINETBUY_ACK)
	ON_RECEIVE(DM_CASHITEMPUTIN_ACK)
	ON_RECEIVE(DM_POSTINVENITEM_ACK)
	ON_RECEIVE(DM_CHECKCHANGENAME_ACK)
	ON_RECEIVE(DM_HEROSELECT_ACK)  
	ON_RECEIVE(DM_STOPTHECLOCK_ACK)
	ON_RECEIVE(DM_CASTLEOCCUPY_ACK)
	ON_RECEIVE(DM_AUCTIONREG_ACK) 
	ON_RECEIVE(DM_AUCTIONREGCANCEL_ACK)
	ON_RECEIVE(DM_AUCTIONBID_ACK)
	ON_RECEIVE(DM_AUCTIONEND_ACK)
	ON_RECEIVE(DM_AUCTIONBUYDIRECT_ACK)	
	ON_RECEIVE(DM_GAINCASHBONUS_ACK)
	ON_RECEIVE(DM_WARLORDSAY_ACK)  
	ON_RECEIVE(DM_POSTVIEW_ACK)
	ON_RECEIVE(DM_POSTDEL_ACK)
	ON_RECEIVE(DM_POSTBILLUPDATE_ACK)
	ON_RECEIVE(DM_TOURNAMENTREWARD_ACK)
	ON_RECEIVE(DM_CMGIFTLOG_REQ)

	//////////////////////////////////////////
	//길드
	ON_RECEIVE(DM_GUILDCABINETPUTIN_ACK)
	ON_RECEIVE(DM_GUILDCABINETTAKEOUT_ACK)
	//////////////////////////////////////////

	// World server message handler
	ON_RECEIVE(MW_MONTHRANKRESETCHAR_REQ)
	ON_RECEIVE(MW_ENTERSOLOMAP_REQ)
	ON_RECEIVE(MW_INVALIDCHAR_REQ)
	ON_RECEIVE(MW_RELEASEMAIN_REQ)
	ON_RECEIVE(MW_MAPSVRLIST_REQ)
	ON_RECEIVE(MW_ADDCONNECT_REQ)
	ON_RECEIVE(MW_CHECKMAIN_REQ)
	ON_RECEIVE(MW_ENTERCHAR_REQ)
	ON_RECEIVE(MW_CONRESULT_REQ)
	ON_RECEIVE(MW_CLOSECHAR_REQ)
	ON_RECEIVE(MW_ROUTELIST_REQ)
	ON_RECEIVE(MW_ENTERSVR_REQ)
	ON_RECEIVE(MW_CHARDATA_REQ)
	ON_RECEIVE(MW_CHARINFO_REQ)
	ON_RECEIVE(MW_TELEPORT_REQ)
	ON_RECEIVE(MW_CONLIST_REQ)
	ON_RECEIVE(MW_DELCHAR_REQ)
	ON_RECEIVE(MW_ROUTE_REQ)
	ON_RECEIVE(MW_CHGPARTYTYPE_REQ)
	ON_RECEIVE(MW_PARTYADD_REQ)
	ON_RECEIVE(MW_PARTYJOIN_REQ)
	ON_RECEIVE(MW_PARTYDEL_REQ)
	ON_RECEIVE(MW_PARTYMANSTAT_REQ)
	ON_RECEIVE(MW_LEVELUP_REQ)
	ON_RECEIVE(MW_EVENTQUARTER_REQ)
	ON_RECEIVE(MW_HELMETHIDE_REQ)
	ON_RECEIVE(MW_PARTYMEMBERRECALL_REQ)
	ON_RECEIVE(MW_PARTYMEMBERRECALLANS_REQ)
	ON_RECEIVE(MW_CHANGECHARBASE_REQ)
	ON_RECEIVE(MW_RELAYCONNECT_REQ)
	ON_RECEIVE(MW_RPSGAMECHANGE_REQ)

	//////////////////////////////////////////////////////////////////////////
	// 길드
	ON_RECEIVE(MW_GUILDESTABLISH_REQ)
	ON_RECEIVE(MW_GUILDDISORGANIZATION_REQ)
	ON_RECEIVE(MW_GUILDLEAVE_REQ)
	ON_RECEIVE(MW_GUILDDUTY_REQ)
	ON_RECEIVE(MW_GUILDPEER_REQ)
	ON_RECEIVE(MW_GUILDJOIN_REQ)
	ON_RECEIVE(MW_GUILDINVITE_REQ)
	ON_RECEIVE(MW_GUILDMEMBERLIST_REQ)
	ON_RECEIVE(MW_GUILDINFO_REQ)
	ON_RECEIVE(MW_GUILDCABINETLIST_REQ)
	ON_RECEIVE(MW_GUILDCONTRIBUTION_REQ)
	ON_RECEIVE(MW_GUILDARTICLELIST_REQ)
	ON_RECEIVE(MW_GUILDARTICLEADD_REQ)
	ON_RECEIVE(MW_GUILDARTICLEDEL_REQ)
	ON_RECEIVE(MW_GUILDARTICLEUPDATE_REQ)
	ON_RECEIVE(MW_GUILDFAME_REQ)
	ON_RECEIVE(MW_GUILDWANTEDADD_REQ)
	ON_RECEIVE(MW_GUILDWANTEDDEL_REQ)
	ON_RECEIVE(MW_GUILDWANTEDLIST_REQ)
	ON_RECEIVE(MW_GUILDVOLUNTEERING_REQ)
	ON_RECEIVE(MW_GUILDVOLUNTEERINGDEL_REQ)
	ON_RECEIVE(MW_GUILDVOLUNTEERLIST_REQ)
	ON_RECEIVE(MW_GUILDVOLUNTEERREPLY_REQ)
	ON_RECEIVE(MW_GUILDTACTICSWANTEDADD_REQ)
	ON_RECEIVE(MW_GUILDTACTICSWANTEDDEL_REQ)
	ON_RECEIVE(MW_GUILDTACTICSWANTEDLIST_REQ)
	ON_RECEIVE(MW_GUILDTACTICSVOLUNTEERING_REQ)
	ON_RECEIVE(MW_GUILDTACTICSVOLUNTEERINGDEL_REQ)
	ON_RECEIVE(MW_GUILDTACTICSVOLUNTEERLIST_REQ)
	ON_RECEIVE(MW_GUILDTACTICSREPLY_REQ)
	ON_RECEIVE(MW_GUILDTACTICSKICKOUT_REQ)
	ON_RECEIVE(MW_GUILDTACTICSINVITE_REQ)
	ON_RECEIVE(MW_GUILDTACTICSANSWER_REQ)
	ON_RECEIVE(MW_GUILDTACTICSLIST_REQ)

	//////////////////////////////////////////////////////////////////////////
	ON_RECEIVE(MW_CHAT_REQ)
	ON_RECEIVE(MW_ADDITEM_REQ)
	ON_RECEIVE(MW_ADDITEMRESULT_REQ)
	ON_RECEIVE(MW_PARTYORDERTAKEITEM_REQ)
	ON_RECEIVE(MW_FRIENDLIST_REQ)
	ON_RECEIVE(MW_FRIENDCONNECTION_REQ)
	ON_RECEIVE(MW_FRIENDREGION_REQ)
	ON_RECEIVE(MW_FRIENDADD_REQ)
	ON_RECEIVE(MW_FRIENDASK_REQ)
	ON_RECEIVE(MW_FRIENDERASE_REQ)
	ON_RECEIVE(MW_FRIENDGROUPMAKE_REQ)
	ON_RECEIVE(MW_FRIENDGROUPDELETE_REQ)
	ON_RECEIVE(MW_FRIENDGROUPCHANGE_REQ)
	ON_RECEIVE(MW_FRIENDGROUPNAME_REQ)
	ON_RECEIVE(MW_CHGPARTYCHIEF_REQ)
	ON_RECEIVE(MW_PARTYATTR_REQ)
	ON_RECEIVE(MW_CHGSQUADCHIEF_REQ)
	ON_RECEIVE(MW_CHARSTATINFO_REQ)
	ON_RECEIVE(MW_CHARSTATINFOANS_REQ)
	ON_RECEIVE(MW_MONSTERDIE_REQ)
	ON_RECEIVE(MW_TAKEMONMONEY_REQ)
	ON_RECEIVE(MW_LOCALOCCUPY_REQ)
	ON_RECEIVE(MW_MISSIONOCCUPY_REQ)
	ON_RECEIVE(MW_LOCALENABLE_REQ)
	ON_RECEIVE(MW_CHGCORPSCOMMANDER_REQ)
	ON_RECEIVE(MW_CORPSASK_REQ)
	ON_RECEIVE(MW_CORPSREPLY_REQ)
	ON_RECEIVE(MW_CORPSJOIN_REQ)
	ON_RECEIVE(MW_ADDSQUAD_REQ)
	ON_RECEIVE(MW_DELSQUAD_REQ)
	ON_RECEIVE(MW_DELCORPSUNIT_REQ)
	ON_RECEIVE(MW_ADDCORPSUNIT_REQ)
	ON_RECEIVE(MW_CORPSCMD_REQ)
	ON_RECEIVE(MW_CORPSENEMYLIST_REQ)
	ON_RECEIVE(MW_MOVECORPSENEMY_REQ)
	ON_RECEIVE(MW_MOVECORPSUNIT_REQ)
	ON_RECEIVE(MW_ADDCORPSENEMY_REQ)
	ON_RECEIVE(MW_DELCORPSENEMY_REQ)
	ON_RECEIVE(MW_CORPSHP_REQ)
	ON_RECEIVE(MW_REPORTENEMYLIST_REQ)
	ON_RECEIVE(MW_PARTYMOVE_REQ)
	ON_RECEIVE(MW_TMSRECV_REQ)
	ON_RECEIVE(MW_TMSINVITEASK_REQ)
	ON_RECEIVE(MW_TMSINVITE_REQ)
	ON_RECEIVE(MW_TMSOUT_REQ)
	ON_RECEIVE(MW_POSTRECV_REQ)
	ON_RECEIVE(MW_CASTLEENABLE_REQ)
	ON_RECEIVE(MW_CASTLEAPPLY_REQ)
	ON_RECEIVE(MW_MONTEMPT_REQ)
	ON_RECEIVE(MW_GETBLOOD_REQ)
	ON_RECEIVE(MW_DEALITEMERROR_REQ)
	ON_RECEIVE(MW_MAGICMIRROR_REQ)
	ON_RECEIVE(MW_CASTLEOCCUPY_REQ)
	ON_RECEIVE(MW_CREATERECALLMON_REQ)
	ON_RECEIVE(MW_RECALLMONDEL_REQ)
	ON_RECEIVE(MW_USERPOSITION_REQ)
	ON_RECEIVE(MW_STARTTELEPORT_REQ)
	ON_RECEIVE(MW_PETRIDING_REQ)
	ON_RECEIVE(MW_SOULMATE_REQ)
	ON_RECEIVE(MW_SOULMATESEARCH_REQ)
	ON_RECEIVE(MW_SOULMATEREG_REQ)
	ON_RECEIVE(MW_SOULMATEEND_REQ)
	ON_RECEIVE(MW_CASTLEWARINFO_REQ)
	ON_RECEIVE(MW_ENDWAR_REQ)
	ON_RECEIVE(MW_CHATBAN_REQ)  
	ON_RECEIVE(MW_RECALLMONDATA_REQ)
	ON_RECEIVE(MW_HEROSELECT_REQ)
	ON_RECEIVE(MW_CHARMSG_REQ)
	ON_RECEIVE(MW_ITEMSTATE_REQ)
	ON_RECEIVE(MW_GAINPVPPOINT_REQ)
	ON_RECEIVE(MW_GUILDPOINTLOG_REQ)
	ON_RECEIVE(MW_GUILDPOINTREWARD_REQ)
	ON_RECEIVE(MW_GUILDPVPRECORD_REQ)
	ON_RECEIVE(MW_MONSTERBUY_REQ)
	ON_RECEIVE(MW_CASTLEGUILDCHG_REQ)
	ON_RECEIVE(MW_EVENTUPDATE_REQ)  
	ON_RECEIVE(MW_EVENTMSG_REQ)
	ON_RECEIVE(MW_CASHSHOPSTOP_REQ)
	ON_RECEIVE(MW_CASHITEMSALE_REQ)
	ON_RECEIVE(MW_WORLDPOSTSEND_REQ)
	ON_RECEIVE(MW_CASTLEAPPLICANTCOUNT_REQ)
	ON_RECEIVE(MW_MONTHRANKLIST_REQ)  
	ON_RECEIVE(MW_MONTHRANKRESET_REQ)
	ON_RECEIVE(MW_MONTHRANKUPDATE_REQ)
	ON_RECEIVE(MW_FAMERANKUPDATE_REQ)
	ON_RECEIVE(MW_WARLORDSAY_REQ)
	ON_RECEIVE(MW_FIRSTGRADEGROUP_REQ)
	ON_RECEIVE(MW_TERMINATE_REQ)
	ON_RECEIVE(MW_TOURNAMENT_REQ)
	ON_RECEIVE(MW_TOURNAMENTENABLE_REQ)
	ON_RECEIVE(MW_TOURNAMENTINFO_REQ)
	ON_RECEIVE(MW_TOURNAMENTMATCH_REQ)
	ON_RECEIVE(MW_TOURNAMENTRESULT_REQ)
	ON_RECEIVE(MW_TOURNAMENTBATPOINT_REQ)
	ON_RECEIVE(MW_HELPMESSAGE_REQ)
	ON_RECEIVE(MW_MISSIONENABLE_REQ)
	ON_RECEIVE(MW_EVENTMSGLOTTERY_REQ)
	ON_RECEIVE(MW_RPSGAME_REQ)
	ON_RECEIVE(MW_WARCOUNTRYBALANCE_REQ)
	ON_RECEIVE(MW_MEETINGROOM_REQ)
	ON_RECEIVE(MW_CMGIFT_REQ)
	ON_RECEIVE(MW_CMGIFTRESULT_REQ)

	//Apex
	ON_RECEIVE(MW_APEXDATA_REQ)

	// CS message handler - Implemented on CSHandler.cpp
#ifdef __HACK_SHIELD
	ON_RECEIVE(CS_HACKSHIELD_REQ)
#endif
#ifdef __N_PROTECT
	ON_RECEIVE(CS_NPROTECT_REQ)
#endif

	ON_RECEIVE(CS_CONREADY_REQ)
	ON_RECEIVE(CS_CONNECT_REQ)
	ON_RECEIVE(CS_KICKOUT_REQ)
	ON_RECEIVE(CS_BLOCK_REQ)
	ON_RECEIVE(CS_MOVE_REQ)
	ON_RECEIVE(CS_JUMP_REQ)
	ON_RECEIVE(CS_MONMOVE_REQ)
	ON_RECEIVE(CS_REVIVAL_REQ)
	ON_RECEIVE(CS_ENTERLB_REQ)
	ON_RECEIVE(CS_LEAVELB_REQ)
	ON_RECEIVE(CS_ENTERAB_REQ)
	ON_RECEIVE(CS_LEAVEAB_REQ)
	ON_RECEIVE(CS_CHGMODE_REQ)
	ON_RECEIVE(CS_ACTION_REQ)
	ON_RECEIVE(CS_DEFEND_REQ)
	ON_RECEIVE(CS_MOVEITEM_REQ)
	ON_RECEIVE(CS_SKILLBUY_REQ)
	ON_RECEIVE(CS_SKILLUSE_REQ)
	ON_RECEIVE(CS_LOOPSKILL_REQ)
	ON_RECEIVE(CS_CHGPARTYCHIEF_REQ)
	ON_RECEIVE(CS_CHGPARTYTYPE_REQ)
    ON_RECEIVE(CS_PARTYADD_REQ)
	ON_RECEIVE(CS_PARTYJOIN_REQ)
	ON_RECEIVE(CS_PARTYDEL_REQ)
	ON_RECEIVE(CS_QUESTEXEC_REQ)
	ON_RECEIVE(CS_QUESTDROP_REQ)
	ON_RECEIVE(CS_QUESTENDTIMER_REQ)
	ON_RECEIVE(CS_QUESTLIST_POSSIBLE_REQ)
	/////////////////////////////////////////////////////////
	// 길드
	ON_RECEIVE(CS_GUILDESTABLISH_REQ)
	ON_RECEIVE(CS_GUILDDISORGANIZATION_REQ)
	ON_RECEIVE(CS_GUILDINVITE_REQ)
	ON_RECEIVE(CS_GUILDINVITEANSWER_REQ)
	ON_RECEIVE(CS_GUILDKICKOUT_REQ)
	ON_RECEIVE(CS_GUILDLEAVE_REQ)
	ON_RECEIVE(CS_GUILDDUTY_REQ)
	ON_RECEIVE(CS_GUILDPEER_REQ)
	ON_RECEIVE(CS_GUILDMEMBERLIST_REQ)
	ON_RECEIVE(CS_GUILDINFO_REQ)
	ON_RECEIVE(CS_GUILDLOCALLIST_REQ)
	ON_RECEIVE(CS_GUILDLOCALRETURN_REQ)
	ON_RECEIVE(CS_GUILDCABINETLIST_REQ)
	ON_RECEIVE(CS_GUILDCABINETPUTIN_REQ)
	ON_RECEIVE(CS_GUILDCABINETTAKEOUT_REQ)
	ON_RECEIVE(CS_GUILDCONTRIBUTION_REQ)
	ON_RECEIVE(CS_GUILDARTICLELIST_REQ)
	ON_RECEIVE(CS_GUILDARTICLEADD_REQ)
	ON_RECEIVE(CS_GUILDARTICLEDEL_REQ)
	ON_RECEIVE(CS_GUILDARTICLEUPDATE_REQ)
	ON_RECEIVE(CS_GUILDFAME_REQ)
	ON_RECEIVE(CS_GUILDWANTEDADD_REQ)
	ON_RECEIVE(CS_GUILDWANTEDDEL_REQ)
	ON_RECEIVE(CS_GUILDWANTEDLIST_REQ)
	ON_RECEIVE(CS_GUILDVOLUNTEERING_REQ)
	ON_RECEIVE(CS_GUILDVOLUNTEERINGDEL_REQ)
	ON_RECEIVE(CS_GUILDVOLUNTEERLIST_REQ)
	ON_RECEIVE(CS_GUILDVOLUNTEERREPLY_REQ)
	ON_RECEIVE(CS_GUILDTACTICSWANTEDADD_REQ)
	ON_RECEIVE(CS_GUILDTACTICSWANTEDDEL_REQ)
	ON_RECEIVE(CS_GUILDTACTICSWANTEDLIST_REQ)
	ON_RECEIVE(CS_GUILDTACTICSVOLUNTEERING_REQ)
	ON_RECEIVE(CS_GUILDTACTICSVOLUNTEERINGDEL_REQ)
	ON_RECEIVE(CS_GUILDTACTICSVOLUNTEERLIST_REQ)
	ON_RECEIVE(CS_GUILDTACTICSREPLY_REQ)
	ON_RECEIVE(CS_GUILDTACTICSKICKOUT_REQ)
	ON_RECEIVE(CS_GUILDTACTICSINVITE_REQ)
	ON_RECEIVE(CS_GUILDTACTICSANSWER_REQ)
	ON_RECEIVE(CS_GUILDTACTICSLIST_REQ)

	/////////////////////////////////////////////////////////
	ON_RECEIVE(CS_CHAT_REQ)
	ON_RECEIVE(CS_CABINETPUTIN_REQ)
	ON_RECEIVE(CS_CABINETTAKEOUT_REQ)
	ON_RECEIVE(CS_CABINETLIST_REQ)
	ON_RECEIVE(CS_CABINETITEMLIST_REQ)
	ON_RECEIVE(CS_CABINETOPEN_REQ)
	ON_RECEIVE(CS_TELEPORT_REQ)
	ON_RECEIVE(CS_NPCITEMLIST_REQ)
	ON_RECEIVE(CS_ITEMBUY_REQ)
	ON_RECEIVE(CS_MAGICITEMBUY_REQ)
	ON_RECEIVE(CS_ITEMSELL_REQ)
	ON_RECEIVE(CS_MONITEMLIST_REQ)
	ON_RECEIVE(CS_MONITEMTAKE_REQ)
	ON_RECEIVE(CS_MONMONEYTAKE_REQ)
	ON_RECEIVE(CS_MONITEMLOTTERY_REQ)
	ON_RECEIVE(CS_PROTECTEDADD_REQ)
	ON_RECEIVE(CS_PROTECTEDERASE_REQ)
	ON_RECEIVE(CS_FRIENDASK_REQ)
	ON_RECEIVE(CS_FRIENDREPLY_REQ)
	ON_RECEIVE(CS_FRIENDERASE_REQ)
	ON_RECEIVE(CS_HOTKEYADD_REQ)
	ON_RECEIVE(CS_HOTKEYDEL_REQ)
	ON_RECEIVE(CS_CHARSTATINFO_REQ)
	ON_RECEIVE(CS_CANCELACTION_REQ)
	ON_RECEIVE(CS_ITEMUPGRADE_REQ)
	ON_RECEIVE(CS_CHGCORPSCOMMANDER_REQ)
	ON_RECEIVE(CS_CORPSLEAVE_REQ)
	ON_RECEIVE(CS_CORPSASK_REQ)
	ON_RECEIVE(CS_CORPSREPLY_REQ)
	ON_RECEIVE(CS_CORPSCMD_REQ)
	ON_RECEIVE(CS_CORPSENEMYLIST_REQ)
	ON_RECEIVE(CS_MOVECORPSENEMY_REQ)
	ON_RECEIVE(CS_MOVECORPSUNIT_REQ)
	ON_RECEIVE(CS_ADDCORPSENEMY_REQ)
	ON_RECEIVE(CS_DELCORPSENEMY_REQ)
	ON_RECEIVE(CS_CORPSHP_REQ)
	ON_RECEIVE(CS_PARTYMOVE_REQ)
	ON_RECEIVE(CS_FRIENDGROUPMAKE_REQ)
	ON_RECEIVE(CS_FRIENDGROUPDELETE_REQ)
	ON_RECEIVE(CS_FRIENDGROUPCHANGE_REQ)
	ON_RECEIVE(CS_FRIENDGROUPNAME_REQ)
	ON_RECEIVE(CS_TMSSEND_REQ)
	ON_RECEIVE(CS_TMSINVITE_REQ)
	ON_RECEIVE(CS_TMSOUT_REQ)
	ON_RECEIVE(CS_POSTSEND_REQ)
	ON_RECEIVE(CS_POSTVIEW_REQ)
	ON_RECEIVE(CS_POSTDEL_REQ)
	ON_RECEIVE(CS_POSTGETITEM_REQ)
	ON_RECEIVE(CS_POSTRETURN_REQ)
	ON_RECEIVE(CS_CASTLEAPPLY_REQ)
	ON_RECEIVE(CS_ITEMUSE_REQ)
	ON_RECEIVE(CS_NPCTALK_REQ)
	ON_RECEIVE(CS_DELRECALLMON_REQ)
	ON_RECEIVE(CS_CHGMODERECALLMON_REQ)
	ON_RECEIVE(CS_INVENADD_REQ)
	ON_RECEIVE(CS_INVENDEL_REQ)
	ON_RECEIVE(CS_INVENMOVE_REQ)
	ON_RECEIVE(CS_RESETHOST_REQ)
	ON_RECEIVE(CS_SWITCHCHANGE_REQ)
	ON_RECEIVE(CS_REGION_REQ)
	ON_RECEIVE(CS_SKILLEND_REQ)
	ON_RECEIVE(CS_REVIVALASK_REQ)
	ON_RECEIVE(CS_DROPDAMAGE_REQ)
	ON_RECEIVE(CS_SETRETURNPOS_REQ)
	ON_RECEIVE(CS_DEALITEMASK_REQ)
	ON_RECEIVE(CS_DEALITEMRLY_REQ)
	ON_RECEIVE(CS_DEALITEMADD_REQ)
	ON_RECEIVE(CS_DEALITEM_REQ)
	ON_RECEIVE(CS_STOREOPEN_REQ)
	ON_RECEIVE(CS_STORECLOSE_REQ)
	ON_RECEIVE(CS_STOREITEMLIST_REQ)
	ON_RECEIVE(CS_STOREITEMBUY_REQ)
	ON_RECEIVE(CS_PETMAKE_REQ)
	ON_RECEIVE(CS_PETDEL_REQ)
	ON_RECEIVE(CS_PETRECALL_REQ)
	ON_RECEIVE(CS_PETRIDING_REQ)
	ON_RECEIVE(CS_OTHERSELF_REQ)
	ON_RECEIVE(CS_DISCONNECT_REQ)
	ON_RECEIVE(CS_CHGCHANNEL_REQ)
	ON_RECEIVE(CS_CANCELSKILL_REQ)
	ON_RECEIVE(CS_DUELINVITE_REQ)
	ON_RECEIVE(CS_DUELINVITEREPLY_REQ)
	ON_RECEIVE(CS_DUELEND_REQ)
	ON_RECEIVE(CS_GETTARGET_REQ)
	ON_RECEIVE(CS_GETTARGETANS_REQ)
	ON_RECEIVE(CS_MONITEMTAKEALL_REQ)
	ON_RECEIVE(CS_SMSSEND_REQ)
	ON_RECEIVE(CS_SKILLINIT_REQ)
	ON_RECEIVE(CS_SKILLINITPOSSIBLE_REQ)
	ON_RECEIVE(CS_HELMETHIDE_REQ)
	ON_RECEIVE(CS_PARTYMEMBERRECALL_REQ)
	ON_RECEIVE(CS_PARTYMEMBERRECALLANS_REQ)
	ON_RECEIVE(CS_CASHITEMCABINET_REQ)
	ON_RECEIVE(CS_CASHITEMGET_REQ)
	ON_RECEIVE(CS_CASHSHOPITEMLIST_REQ)
	ON_RECEIVE(CS_CASHITEMBUY_REQ)
	ON_RECEIVE(CS_CASHITEMPRESENT_REQ)
	ON_RECEIVE(CS_SOULMATESEARCH_REQ)
	ON_RECEIVE(CS_SOULMATEREGREADY_REQ)
	ON_RECEIVE(CS_SOULMATEREG_REQ)
	ON_RECEIVE(CS_SOULMATEEND_REQ)
	ON_RECEIVE(CS_GAMBLECHECK_REQ)
	ON_RECEIVE(CS_GAMBLEOPEN_REQ)
	ON_RECEIVE(CS_TAKEGODBALL_REQ)
	ON_RECEIVE(CS_MOUNTGODBALL_REQ)
	ON_RECEIVE(CS_DEMOUNTGODBALL_REQ)
	ON_RECEIVE(CS_DURATIONREP_REQ)
	ON_RECEIVE(CS_REFINE_REQ)
	ON_RECEIVE(CS_ACTEND_REQ)
	ON_RECEIVE(CS_CASHCABINETBUY_REQ)
	ON_RECEIVE(CS_CASHITEMPUTIN_REQ)
	ON_RECEIVE(CS_CHANGENAME_REQ)
	ON_RECEIVE(CS_STOPTHECLOCK_REQ)
	ON_RECEIVE(CS_HEROSELECT_REQ)  
	ON_RECEIVE(CS_HEROLIST_REQ)
	ON_RECEIVE(CS_TERMINATE_REQ)
	ON_RECEIVE(CS_CHECKRELAY_REQ)
	ON_RECEIVE(CS_COMMENT_REQ)
	ON_RECEIVE(CS_GUILDPOINTLOG_REQ)
	ON_RECEIVE(CS_GUILDPOINTREWARD_REQ)
	ON_RECEIVE(CS_GUILDPVPRECORD_REQ)
	ON_RECEIVE(CS_PVPRECORD_REQ)
	ON_RECEIVE(CS_MONSTERBUY_REQ)
	ON_RECEIVE(CS_PROTECTEDOPTION_REQ)
	ON_RECEIVE(CS_FRIENDLIST_REQ)
	ON_RECEIVE(CS_ITEMCHANGE_REQ)
	ON_RECEIVE(CS_COUNTDOWN_REQ)
	ON_RECEIVE(CS_WASTEREFINE_REQ)
	ON_RECEIVE(CS_PETCANCEL_REQ)
	ON_RECEIVE(CS_AUCTIONREG_REQ) 
	ON_RECEIVE(CS_AUCTIONINTEREST_REQ)
	ON_RECEIVE(CS_AUCTIONINTERESTLIST_REQ)
	ON_RECEIVE(CS_AUCTIONBIDLIST_REQ)
	ON_RECEIVE(CS_AUCTIONREGCANCEL_REQ)
	ON_RECEIVE(CS_AUCTIONBID_REQ)
	ON_RECEIVE(CS_AUCTIONBUYDIRECT_REQ)
	ON_RECEIVE(CS_AUCTIONFIND_REQ)
	ON_RECEIVE(CS_AUCTIONREGLIST_REQ)
	ON_RECEIVE(CS_KICKOUTMAP_REQ)
	ON_RECEIVE(CS_REGGUILDCLOAK_REQ)
	ON_RECEIVE(CS_FAMERANKLIST_REQ)  
	ON_RECEIVE(CS_MONTHRANKLIST_REQ)
	ON_RECEIVE(CS_WARLORDSAY_REQ)
	ON_RECEIVE(CS_FIRSTGRADEGROUP_REQ)
	ON_RECEIVE(CS_CHATBAN_REQ)
	ON_RECEIVE(CS_POSTLIST_REQ)
	ON_RECEIVE(CS_TOURNAMENTAPPLYINFO_REQ)
	ON_RECEIVE(CS_TOURNAMENTAPPLY_REQ)
	ON_RECEIVE(CS_TOURNAMENTJOINLIST_REQ)
	ON_RECEIVE(CS_TOURNAMENTPARTYLIST_REQ)
	ON_RECEIVE(CS_TOURNAMENTPARTYADD_REQ)
	ON_RECEIVE(CS_TOURNAMEMTMATCHLIST_REQ)
	ON_RECEIVE(CS_TOURNAMENTEVENTLIST_REQ)
	ON_RECEIVE(CS_TOURNAMENTEVENTINFO_REQ)
	ON_RECEIVE(CS_TOURNAMENTEVENTJOIN_REQ)
	ON_RECEIVE(CS_TOURNAMENTCHEER_REQ)
	ON_RECEIVE(CS_TOURNAMENTSCHEDULE_REQ)
	ON_RECEIVE(CS_TOURNAMENTPARTYDEL_REQ)
	ON_RECEIVE(CS_RPSSTART_REQ)
	ON_RECEIVE(CS_RPSGAME_REQ)
	ON_RECEIVE(CS_ACDCLOSE_REQ)
	ON_RECEIVE(CS_CHANGECOUNTRY_REQ)
	ON_RECEIVE(CS_WARCOUNTRYBALANCE_REQ)
	ON_RECEIVE(CS_MEETINGROOM_REQ)
	ON_RECEIVE(CS_ARENA_REQ)
	ON_RECEIVE(CS_CMMOVE_REQ)
	ON_RECEIVE(CS_CMMOVETOUSER_REQ)
	ON_RECEIVE(CS_CMGIFT_REQ)	

	//apex
	ON_RECEIVE(CS_APEXDATA_REQ)
	ON_RECEIVE(CS_APEXSTART_REQ)
	}

	if(wMsgID != CS_KICKOUT_REQ && wMsgID != CS_CONNECT_REQ)
		LogEvent("Invalid Message %d",pBUF->m_packet.GetID());

	return EC_SESSION_INVALIDMSG;
}

void CTMapSvrModule::OnTimer( DWORD dwTick)
{
	static DWORD dwPassTick = 0;
	dwPassTick += dwTick;
	if(dwPassTick >= 1000)
	{
		LPPACKETBUF pBUF = new PACKETBUF();
		pBUF->m_packet.SetID(SM_TIMER_REQ);
		SayToBATCH(pBUF);
		dwPassTick = 0;
	}

	VTAIBUF::iterator it = m_vAIBUF.begin();

	while(it != m_vAIBUF.end())
	{
		LPTAIBUF pBUF = (*it);

		pBUF->m_dwTick += dwTick;
		if( pBUF->m_dwTick >= pBUF->m_dwDelay )
		{
			SendSM_AICMD_ACK(
				pBUF->m_dwCmdHandle,
				pBUF->m_dwHostKEY,
				pBUF->m_dwEventHost,
				pBUF->m_dwRHId,
				pBUF->m_bRHType,
				pBUF->m_dwMonID,
				pBUF->m_bChannel,
				pBUF->m_wMapID,
				pBUF->m_wPartyID);

			m_vAIBUF.erase(it);
			delete pBUF;
		}
		else
			it++;
	}

	VTGBBUF::iterator itGB = m_vGBBUF.begin();
	while( itGB != m_vGBBUF.end())
	{
		LPTGBBUF pGB = (*itGB);

		pGB->m_dwTick += dwTick;
		if(pGB->m_dwTick >= pGB->m_dwDelay )
		{
			SendSM_GODBALLCMD_ACK(
				pGB->m_wGodBall,
				pGB->m_bCmd,
				pGB->m_dwKEY,
				pGB->m_bChannel,
				pGB->m_wMapID,
				pGB->m_wCastle,
				pGB->m_strCharName);

			m_vGBBUF.erase(itGB);
			delete pGB;
		}
		else
			itGB++;
	}

	for( DWORD so = 0; so < m_vTSWITCHOBJ.size();)
	{
		m_vTSWITCHOBJ[so].m_dwTick += dwTick;
		if(m_vTSWITCHOBJ[so].m_dwTick >= m_vTSWITCHOBJ[so].m_dwDelay)
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_SWITCHCHANGE_REQ)
				<< m_vTSWITCHOBJ[so].m_bChannel
				<< m_vTSWITCHOBJ[so].m_wPartyID
				<< m_vTSWITCHOBJ[so].m_wMapID
				<< m_vTSWITCHOBJ[so].m_dwID;

			SayToBATCH(pMSG);
			m_vTSWITCHOBJ.erase(m_vTSWITCHOBJ.begin() + so);
		}
		else
			so++;
	}

	MAPDWORD64::iterator itPost;
	for(itPost=m_mapTPostBills.begin(); itPost != m_mapTPostBills.end();)
	{
		if((*itPost).second <= m_timeCurrent)
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_POSTBILL_ACK)
				<< (*itPost).first;

			SayToBATCH(pMSG);
			itPost = m_mapTPostBills.erase(itPost);
		}
		else
			break;
	}

	VDWORD vDuel;
	vDuel.clear();
	
	MAPTDUEL::iterator itDuel;
	for(itDuel=m_mapTDuel.begin(); itDuel!=m_mapTDuel.end(); itDuel++)
	{
		if((*itDuel).second->m_dwDuelTick <= DWORD(m_timeCurrent))
		{
			if((*itDuel).second->m_bType == DUEL_STANDBY)
			{
				LPPACKETBUF pMSG = new PACKETBUF();
				pMSG->m_packet.SetID(SM_DUELSTART_ACK)
					<< (*itDuel).second->m_dwDuelID
					<< (*itDuel).second->m_dwInviter
					<< (*itDuel).second->m_dwTarget;

				SayToBATCH(pMSG);

				(*itDuel).second->m_bType = DUEL_START;
				(*itDuel).second->m_dwDuelTick = (DWORD)m_timeCurrent + DUEL_TIME;
			}
			else if((*itDuel).second->m_bType == DUEL_START)
				SendSM_DUELEND_REQ((*itDuel).second->m_dwDuelID, 0);
			else
			{
				vDuel.push_back((*itDuel).second->m_dwDuelID);

				LPPACKETBUF pMSG = new PACKETBUF();
				pMSG->m_packet.SetID(SM_DUELCOMPLETE_ACK)
					<< (*itDuel).second->m_dwInviter
					<< (*itDuel).second->m_dwTarget;

				SayToBATCH(pMSG);
			}
		}
	}

	for(DWORD du=0; du<vDuel.size(); du++)
	{
		itDuel = m_mapTDuel.find(vDuel[du]);
		if(itDuel!=m_mapTDuel.end())
		{
			delete (*itDuel).second;
			m_mapTDuel.erase(vDuel[du]);
		}
	}
	vDuel.clear();

	MAPDWORD::iterator itMRT;
	for(itMRT=m_mapMeetingRoomTime.begin(); itMRT != m_mapMeetingRoomTime.end(); itMRT++)
	{
		if(m_dwTick - itMRT->second > 1000 * HOUR_ONE)
		{
			LPPACKETBUF pMSG = new PACKETBUF();
			pMSG->m_packet.SetID(SM_MEETINGROOM_ACK)
				<< itMRT->first;
			SayToBATCH(pMSG);

			m_mapMeetingRoomTime.erase(itMRT);
			break;
		}
	}
}

void CTMapSvrModule::AuctionTimeCheck()
{
	VTAUCTIONBUF::iterator itAU = m_vTAUCTIONBUF.begin();
	while(itAU != m_vTAUCTIONBUF.end())
	{
		if((*itAU)->m_dEndTime <= m_timeCurrent )
		{			
			SendDM_AUCTIONEND_REQ(
				(*itAU)->m_wNpcID,
				(*itAU)->m_dwAuctionID,
				(*itAU)->m_dEndTime);

			delete (*itAU);
			itAU = m_vTAUCTIONBUF.erase(itAU);			
		}
		else
			break;
	}
}

void CTMapSvrModule::SayToBATCH( LPPACKETBUF pBUF)
{
	if(!pBUF->m_packet.IsValid())
	{
		delete pBUF;
		return;
	}

	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csQUEUE);
	m_qBATCHJOB.push(pBUF);
	LeaveCriticalSection(&m_csQUEUE);
	SetEvent(m_hBatchEvent);
}

void CTMapSvrModule::SayToDB( LPPACKETBUF pBUF)
{
	if(!pBUF->m_packet.IsValid())
	{
		delete pBUF;
		return;
	}

	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csDBQUEUE);
	m_qDBJOB.push(pBUF);
	LeaveCriticalSection(&m_csDBQUEUE);
	SetEvent(m_hDBEvent);
}

void CTMapSvrModule::SayToAI( LPPACKETBUF pBUF)
{
	if(!pBUF->m_packet.IsValid())
	{
		delete pBUF;
		return;
	}

	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csAIQUEUE);
	m_qAIJOB.push(pBUF);
	LeaveCriticalSection(&m_csAIQUEUE);
	SetEvent(m_hAIEvent);
}

void CTMapSvrModule::SayToLOG( LPPACKETBUF pBUF)
{
	if( !pBUF->m_packet.IsValid() || !m_bLogRun)
	{
		delete pBUF;
		return;
	}

	pBUF->m_packet.Rewind(FALSE);

	EnterCriticalSection(&m_csLOGQUEUE);
	m_qLOGJOB.push(pBUF);
	LeaveCriticalSection(&m_csLOGQUEUE);
	SetEvent(m_hLogEvent);
}

HRESULT CTMapSvrModule::PreMessageLoop( int nShowCmd)
{
	DWORD dwResult = OnEnter();

	if(dwResult)
	{
		OnERROR(dwResult);
		return E_FAIL;
	}
	m_dwThreadID = GetCurrentThreadId();

	return CAtlServiceModuleT<CTMapSvrModule,IDS_SERVICENAME>::PreMessageLoop(nShowCmd);
}

HRESULT CTMapSvrModule::PostMessageLoop()
{
	OnExit();
	return CAtlServiceModuleT<CTMapSvrModule,IDS_SERVICENAME>::PostMessageLoop();
}

CTObjBase * CTMapSvrModule::FindTarget(CTPlayer * pHost,
									  BYTE bTargetType,
									  DWORD dwTargetID)
{
	if(!pHost || !pHost->m_pMAP)
		return NULL;

	CTObjBase *pTARGET = NULL;
	switch(bTargetType)
	{
	case OT_MON	:
		pTARGET = pHost->m_pMAP->FindMonster(dwTargetID);
		break;

	case OT_RECALL:
		{
			MAPTRECALLMON::iterator itRe = pHost->m_mapRecallMon.find(dwTargetID);
			if(itRe != pHost->m_mapRecallMon.end() &&
				(*itRe).second->m_bMain)
				pTARGET = (*itRe).second;
		}
		break;
	case OT_PC	:
		{
			if(pHost->m_bMain && dwTargetID == pHost->m_dwID)
				pTARGET = pHost;
		}
		break;
	case OT_SELF:
		pTARGET = pHost->FindSelfObj(dwTargetID);
		break;
	}

	if(pTARGET && !pTARGET->m_pMAP)
		return NULL;

	return pTARGET;
}

CTPlayer * CTMapSvrModule::FindChar(DWORD dwHostID)
{
	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwHostID);

	if( finder == m_mapPLAYER.end())
		return NULL;

	return (*finder).second;
}

CTPlayer *CTMapSvrModule::FindPlayer( DWORD dwCharID,
									  DWORD dwKEY)
{
	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwCharID);

	if( finder == m_mapPLAYER.end() || dwKEY != (*finder).second->m_dwKEY )
		return NULL;

	return (*finder).second;
}

CTPlayer * CTMapSvrModule::FindPlayer(CString strName)
{
	strName.MakeUpper();
	MAPPLAYERNAME::iterator it = m_mapPLAYERNAME.find(strName);
	if(it!=m_mapPLAYERNAME.end())
		return (*it).second;
	else
		return NULL;
}

CTMonsterAI *CTMapSvrModule::FindTMonsterAI( BYTE bAIType)
{
	MAPTMONSTERAI::iterator finder = m_mapTMONAI.find(bAIType);

	if( finder != m_mapTMONAI.end() )
		return (*finder).second;

	return NULL;
}

LPTMONATTR CTMapSvrModule::FindMonAttr(DWORD dwID)
{
	MAPTMONATTR::iterator it = m_mapTMONATTR.find(dwID);
	if(it != m_mapTMONATTR.end())
		return (*it).second;
	
	return NULL;
}

LPTLEVEL CTMapSvrModule::FindTLevel( BYTE bLevel)
{
	MAPTLEVEL::iterator finder = m_mapTLEVEL.find(bLevel);

	if( finder != m_mapTLEVEL.end() )
		return (*finder).second;

	return (*m_mapTLEVEL.begin()).second;
}

LPTSTAT CTMapSvrModule::FindTRace( BYTE bRace)
{
	MAPTSTAT::iterator finder = m_mapTRACE.find(bRace);

	if( finder != m_mapTRACE.end() )
		return (*finder).second;

	return NULL;
}

LPTSTAT CTMapSvrModule::FindTClass( BYTE bClass)
{
	MAPTSTAT::iterator finder = m_mapTCLASS.find(bClass);

	if( finder != m_mapTCLASS.end() )
		return (*finder).second;

	return NULL;
}

LPTITEM CTMapSvrModule::FindTItem( WORD wItemID)
{
	MAPTITEMTEMP::iterator finder = m_mapTITEM.find(wItemID);

	if( finder != m_mapTITEM.end() )
		return (*finder).second;

	return NULL;
}

CTItem * CTMapSvrModule::FindTQusetItem( DWORD dwQuestItemID)
{
	MAPTSTORAGEITEM::iterator finder = m_mapTQuestItem.find(dwQuestItemID);

	if(finder != m_mapTQuestItem.end())
		return (*finder).second;

	return NULL;
}

CTSkillTemp * CTMapSvrModule::FindTSkill(WORD wSkillID)
{
	MAPTSKILLTEMP::iterator finder = m_mapTSKILL.find(wSkillID);

	if( finder != m_mapTSKILL.end())
		return (*finder).second;

	return NULL;
}

LPTAICOMMAND CTMapSvrModule::FindTCmdTEMP( DWORD dwCmdID)
{
	MAPTAICMDTEMP::iterator finder = m_mapTCMDTEMP.find(dwCmdID);

	if( finder != m_mapTCMDTEMP.end() )
		return (*finder).second;

	return NULL;
}

LPTMONSPAWN CTMapSvrModule::FindTMonSpawn( WORD wSpawnID)
{
	MAPTMONSPAWNTEMP::iterator finder = m_mapTMONSPAWN.find(wSpawnID);

	if( finder != m_mapTMONSPAWN.end() )
		return (*finder).second;

	return NULL;
}

LPTMONSTER CTMapSvrModule::FindTMonster( WORD wMonID)
{
	MAPTMONSTERTEMP::iterator finder = m_mapTMONSTER.find(wMonID);

	if( finder != m_mapTMONSTER.end() )
		return (*finder).second;

	return NULL;
}

CTChannel *CTMapSvrModule::FindTChannel( BYTE bChannel,
										 WORD wMapID)
{
	MAPTCHANNEL::iterator finder = m_mapTLOGCHANNEL.find( MAKELONG( wMapID, WORD(bChannel)));

	if( finder != m_mapTLOGCHANNEL.end() )
		return (*finder).second;

	return NULL;
}

CTMap *CTMapSvrModule::FindTMap( BYTE bChannel,
								 WORD wPartyID,
								 WORD wMapID)
{
	CTChannel *pTCH = FindTChannel(
		bChannel,
		wMapID);

	if(pTCH)
		return pTCH->FindTMap( wPartyID, wMapID);

	return NULL;
}

CTMap *CTMapSvrModule::CreateInstanceMAP( BYTE bChannel,
										  WORD wPartyID,
										  WORD wMapID,
										  BYTE bCountryID)
{
	CTChannel *pTCH = FindTChannel(
		bChannel,
		wMapID);

	if(pTCH)
		return pTCH->CreateInstanceMAP( wPartyID, wMapID, bCountryID);

	return NULL;
}

void CTMapSvrModule::DeleteInstanceMAP( CTMap *pTMAP)
{
	MAPTCHANNEL::iterator finder = m_mapTCHANNEL.find(pTMAP->m_bChannel);

	if( finder != m_mapTCHANNEL.end() )
		return (*finder).second->DeleteInstanceMAP(pTMAP);
}

void CTMapSvrModule::DoAICMD( CTAICommand *pCMD,
							  CTMonster *pMON,
							  DWORD dwEventHost,
							  DWORD dwRHId,
							  BYTE bRHType,
							  DWORD dwTriggerID)
{
	DWORD dwDelay = pCMD->GetDelay( pMON, dwEventHost, dwRHId, bRHType);
	if(pMON->m_pMON &&
		pMON->m_pMON->m_bRemove &&
		pCMD->m_pCOMMAND->m_bType == AC_LEAVE)
		dwDelay = 1;

	if(dwDelay)
	{
		SendSM_AICMD_REQ(
			(DWORD) DWORD_PTR(pCMD),
			pMON->m_dwHostKEY,
			dwEventHost,
			dwRHId,
			bRHType,
			dwDelay,
			pMON->m_dwID,
			pMON->m_pMAP->m_bChannel,
			pMON->m_pMAP->m_wMapID,
			pMON->m_pMAP->m_wPartyID);
	}
	else if(pCMD->CanRun( pMON, dwEventHost, dwRHId, bRHType))
		pCMD->ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}

void CTMapSvrModule::DoGBCMD(BYTE bCmd,
							 LPTGODBALL pBall,
							 CString strCharName)
{
	SendSM_GODBALLCMD_REQ(
		pBall->m_wID,
		bCmd,
		pBall->m_pMAP->m_bChannel,
		pBall->m_pMAP->m_wMapID,
		pBall->m_wCastle,
		m_dwGBDelay[bCmd],
		++pBall->m_dwKEY,
		strCharName);
}

LPQUESTTEMP CTMapSvrModule::FindQuestTemplate( DWORD dwID)
{
	MAPQUESTTEMP::iterator finder = m_mapQUESTTEMP.find(dwID);

	if( finder != m_mapQUESTTEMP.end() )
		return (*finder).second;

	return NULL;
}

BYTE CTMapSvrModule::IsOperator(DWORD dwID)
{
	MAPDWORD::iterator finder = m_mapTOPERATOR.find(dwID);

	if( finder != m_mapTOPERATOR.end() )
		return TRUE;

	return FALSE;
}

BYTE CTMapSvrModule::IsIndunMap( WORD wMapID)
{
	MAPTINDURN::iterator it = m_mapTINDUN.find(wMapID);

	if( it != m_mapTINDUN.end() )
		return TRUE;

	return FALSE;
}

WORD CTMapSvrModule::GetIndunInPos( WORD wMapID)
{
	MAPTINDURN::iterator it = m_mapTINDUN.find(wMapID);

	if( it != m_mapTINDUN.end() )
		return (*it).second->m_wInSpawn;

	return 0;
}

WORD CTMapSvrModule::GetIndunOutPos( WORD wMapID, BYTE bCountry)
{
	MAPTINDURN::iterator it = m_mapTINDUN.find(wMapID);

	if( it != m_mapTINDUN.end() )
		return bCountry == TCONTRY_D ? (*it).second->m_wOutSpawn_D : (*it).second->m_wOutSpawn_C;

	return 0;
}

CTNpc * CTMapSvrModule::FindTNpc(WORD wNpcID)
{
	MAPTNPC::iterator it = m_mapTNpc.find(wNpcID);
	if(it==m_mapTNpc.end())
		return NULL;
	else
		return (*it).second;
}

void CTMapSvrModule::CheckQuest(CTPlayer * pPlayer,
								DWORD dwTargetID,
								FLOAT fPosX,
								FLOAT fPosY,
								FLOAT fPosZ,
								DWORD dwTermID,
								BYTE bTermType,
								BYTE bTriggerType,
								BYTE bCount)
{
	pPlayer->CheckQuest(m_dwTick, dwTermID, bTermType, bCount);

	VQUESTTEMP *pVQuest = NULL;

	MAPMAPVQUESTTEMP::iterator itTrigger = m_mapTRIGGER.find(bTriggerType);
	if(itTrigger != m_mapTRIGGER.end())
	{
		MAPVQUESTTEMP::iterator itvQuest = ((*itTrigger).second)->find(dwTermID);
		if(itvQuest != (*itTrigger).second->end())
			pVQuest = (*itvQuest).second;
	}

	if(pVQuest)
	{
		BYTE bItemDropped = FALSE;
		for(int i=0; i<(int)pVQuest->size(); i++)
		{
			BYTE bLevel;
			if( !pPlayer->CanRunQuest( (*pVQuest)[i], m_dwTick, bLevel) )
			{
				CQuest *pQuest = CQuest::CreateQuest((*pVQuest)[i]);

				pQuest->m_dwMonID = dwTargetID;
				pQuest->m_fPosX = fPosX;
				pQuest->m_fPosY = fPosY;
				pQuest->m_fPosZ = fPosZ;
				pQuest->m_pMap = pPlayer->m_pMAP;

				pQuest->ExecQuest(
					pPlayer,
					m_dwTick,
					&m_mapTRIGGER,
					&m_mapQUESTTEMP,
					&m_mapTITEM,
					&m_mapTSKILL,
					&m_mapTMONSTER,
					&m_mapTCLASS);

				delete pQuest;
			}
		}
	}
}

void CTMapSvrModule::QuestDropItem(VQUESTTEMP vQuestDropItem,
								   VPLAYER vGetChar,
								   DWORD dwMonID,
								   CTMap * pMAP)
{
	if(vQuestDropItem.empty() || !pMAP)
		return;

	DWORD dwSelected = rand() % DWORD(vQuestDropItem.size());

	CQuest *pQuest = CQuest::CreateQuest(vQuestDropItem[dwSelected]);

	pQuest->m_dwMonID = dwMonID;
	pQuest->m_fPosX = 0;
	pQuest->m_fPosY = 0;
	pQuest->m_fPosZ = 0;
	pQuest->m_pMap = pMAP;

	pQuest->ExecQuest(
		vGetChar[dwSelected],
		m_dwTick,
		&m_mapTRIGGER,
		&m_mapQUESTTEMP,
		&m_mapTITEM,
		&m_mapTSKILL,
		&m_mapTMONSTER,
		&m_mapTCLASS);

	delete pQuest;
}

BYTE CTMapSvrModule::GetSvrID()
{
	return m_bServerID;
}

CTItem * CTMapSvrModule::MonChoiceMagicItem(LPTMONITEM pMonItem, BYTE bItemCountry)
{
	BYTE bSel = 0;
	MAPTITEM mapItem;
	mapItem.clear();

	MAPTSTORAGEITEM::iterator it;
	for(it=m_mapTQuestItem.begin(); it!=m_mapTQuestItem.end(); it++)
	{
		CTItem * pItem = (*it).second;
		if(pItem->m_bMagicDropLevel >= pMonItem->m_bLevelMin &&
			pItem->m_bMagicDropLevel <= pMonItem->m_bLevelMax &&
			(pItem->m_pTITEM->m_bItemCountry == bItemCountry || pItem->m_pTITEM->m_bItemCountry == TCONTRY_N) &&
			(*it).first >= pMonItem->m_wItemIDMin &&
			(*it).first <= pMonItem->m_wItemIDMax)
		{
			mapItem.insert(MAPTITEM::value_type(bSel++, pItem));
		}
	}

	if(mapItem.empty())
		return NULL;

	BYTE bChoice = BYTE(rand() % mapItem.size());
	CTItem * pSelItem = mapItem.find(bChoice)->second;

	mapItem.clear();

	return pSelItem;
}

LPTITEM CTMapSvrModule::MonChoiceItem(LPTMONITEM pMonItem, BYTE bItemCountry)
{
	MAPTITEMTEMP mapItem;
	mapItem.clear();

	MAPVTMONITEM::iterator itMI;
	MAPTITEMTEMP::iterator it;
	WORD wSel = 0;
	WORD wMaxWeight = 0;

	for(itMI=m_mapTMONITEM[pMonItem->m_bChartType].begin(); itMI!=m_mapTMONITEM[pMonItem->m_bChartType].end(); itMI++)
	{
		if((*itMI).first >= pMonItem->m_bLevelMin &&
			(*itMI).first <= pMonItem->m_bLevelMax)
		{
			for(WORD i=0; i<(*itMI).second.size(); i++)
			{
                LPTITEM pItem = (*itMI).second[i];
				if(	pItem->m_bInitState &&
					(bItemCountry == TCONTRY_PEACE || bItemCountry == TCONTRY_B || pItem->m_bItemCountry == bItemCountry || pItem->m_bItemCountry == TCONTRY_N) &&
					pItem->m_wItemID >= pMonItem->m_wItemIDMin &&
					pItem->m_wItemID <= pMonItem->m_wItemIDMax)					
				{
					mapItem.insert(MAPTITEMTEMP::value_type(wSel++, pItem));
					wMaxWeight += pItem->m_wWeight;
				}
			}
		}
	}

	if(mapItem.empty() || wMaxWeight == 0)
		return NULL;

	LPTITEM pTITEM = NULL;
	WORD wChoice = WORD(rand() % wMaxWeight);
	WORD wCur = 0;

	for(it = mapItem.begin(); it != mapItem.end(); it++)
	{
		wCur += (*it).second->m_wWeight;
		if( wChoice < wCur )
			break;
	}	
	pTITEM = (*it).second;
	mapItem.clear();

	return pTITEM;
}

void CTMapSvrModule::SetItemAttr(CTItem * pItem, BYTE bLevel)
{
	MAPTITEMATTR::iterator it = m_mapTItemAttr.find(pItem->m_pTITEM->m_wAttrID+m_itemgrade[bLevel].m_bGrade);
	if(it!=m_mapTItemAttr.end())
		pItem->m_pTITEMATTR = (*it).second;
	else
		pItem->m_pTITEMATTR = (*(m_mapTItemAttr.begin())).second;

	SetAlarmTime(pItem);
}

void CTMapSvrModule::SetAlarmTime(CTItem * pItem)
{
	if(pItem->m_dEndTime &&
		pItem->m_dEndTime > m_timeCurrent)
	{
		if(pItem->m_dEndTime - m_timeCurrent > HOUR_ONE/2)
			pItem->m_dAlarmTime = pItem->m_dEndTime - HOUR_ONE/2;
		else if(pItem->m_dEndTime - m_timeCurrent > HOUR_ONE/3)
			pItem->m_dAlarmTime = pItem->m_dEndTime - HOUR_ONE/3;
		else if(pItem->m_dEndTime - m_timeCurrent > HOUR_ONE/6)
			pItem->m_dAlarmTime = pItem->m_dEndTime - HOUR_ONE/6;
		else if(pItem->m_dEndTime - m_timeCurrent > HOUR_ONE/60)
			pItem->m_dAlarmTime = pItem->m_dEndTime - HOUR_ONE/60;
		else
			pItem->m_dAlarmTime = pItem->m_dEndTime;
	}
}

BYTE CTMapSvrModule::MakeSpecialItem(CTPlayer * pPlayer, CTItem * pItem, BYTE bKind, WORD wMagicBuff, BYTE bMakeType)
{
	if(!pItem || pItem->m_bLevel)
		return FALSE;

	if(bKind == IK_MAGICGRADE)
	{
		if(pItem->m_mapTMAGIC.size())
			return FALSE;

		BYTE bCnt = min(max(BYTE(pPlayer->m_bLevel*0.1), 1), 2);
		bCnt = max(BYTE(rand() % bCnt)+1, 1);
		for(BYTE i=0; i<bCnt; i++)
			SetMagicOpt(pPlayer, pItem, MIP_MAGIC, bMakeType);
	}
	else if(bKind == IK_RAREGRADE)
	{
		BYTE bMaxMagic = 5;
		BYTE bMagicCnt = BYTE(pItem->m_mapTMAGIC.size());
		VBYTE vAddMagic;
		vAddMagic.clear();

		if(!bMagicCnt || bMagicCnt > 2)
			return FALSE;

		BYTE bCnt = min(max(BYTE(pPlayer->m_bLevel*0.1), 3), bMaxMagic);
		bCnt = max(BYTE(rand() % bCnt)+1, 3);
		if(wMagicBuff)
			bCnt = bMaxMagic - bMagicCnt;
		else
			bCnt = bCnt > bMagicCnt ? bCnt - bMagicCnt : 0;

		for(BYTE i=0; i<bCnt; i++)
		{
			BYTE bAddMagic = SetMagicOpt(pPlayer, pItem, MIP_RARE, bMakeType);
			if(bAddMagic)
				vAddMagic.push_back(bAddMagic);
		}

		if(pItem->m_mapTMAGIC.size() == bMagicCnt)
		{
			while(!vAddMagic.empty())
			{
				BYTE bAddMagic = vAddMagic.back();
				vAddMagic.pop_back();
				MAPTMAGIC::iterator itMg = pItem->m_mapTMAGIC.find(bAddMagic);
				if(itMg != pItem->m_mapTMAGIC.end())
				{
					delete (*itMg).second;
					pItem->m_mapTMAGIC.erase(itMg);
				}
			}

			return FALSE;
		}
	}

	if(!pItem->m_mapTMAGIC.size())
		return FALSE;

	return TRUE;
}

BYTE CTMapSvrModule::SetMagicOpt(CTPlayer * pPlayer, CTItem * pItem, BYTE bOptType, BYTE bMakeType)
{
	BYTE bKind = pItem->m_pTITEM->m_bKind;

	if(!pItem->m_pTITEM->m_bCanMagic || m_vItemMagic[bKind].empty())
		return 0;
    
	if(bOptType == MIP_RARE && !pItem->m_pTITEM->m_bCanRare)
		return 0;

	MAPTMAGIC::iterator it;
	VTITEMMAGIC vItemMagic;
	vItemMagic.clear();
	
	for(BYTE i=0; i<m_vItemMagic[bKind].size(); i++)
	{
		it = pItem->m_mapTMAGIC.find(m_vItemMagic[bKind][i]->m_bMagic);
		if(it==pItem->m_mapTMAGIC.end())
		{
			if(m_vItemMagic[bKind][i]->m_bMinLevel > pPlayer->m_bLevel)
				continue;
			else if( bOptType==MIP_MAGIC && !m_vItemMagic[bKind][i]->m_bIsMagic)
				continue;
			else if( bOptType==MIP_RARE && !m_vItemMagic[bKind][i]->m_bIsRare)
				continue;

			vItemMagic.push_back(m_vItemMagic[bKind][i]);
		}
	}

	if(!vItemMagic.size())
		return 0;

	BYTE sel = 0;
	BYTE bSelected = 0;

	while(!vItemMagic.empty())
	{
		sel = BYTE(rand() % vItemMagic.size());
		bSelected = 1;

		if(vItemMagic[sel]->m_bExclIndex)
		{
			for(it=pItem->m_mapTMAGIC.begin(); it!=pItem->m_mapTMAGIC.end(); it++)
			{
				if((*it).second->m_pMagic->m_bExclIndex==vItemMagic[sel]->m_bExclIndex)
				{
					vItemMagic.erase(vItemMagic.begin() + sel);
					bSelected = 0;
					break;
				}
			}
		}

		if(bSelected)
			break;
	}

	if(!bSelected)
	{
		vItemMagic.clear();
		return 0;
	}

	// m_wValue 최종옵션비율
	INT nBaseLevel = 0;
	if(bMakeType == IMT_SCROLL)
		nBaseLevel = INT(min(ITEMAGIC_BASELEVEL,pPlayer->m_bLevel)) - max(0, 34 - max(pItem->GetEquipLevel(), pItem->GetPowerLevel()));
	else if(bMakeType == IMT_GAMBLE)
		nBaseLevel = min(ITEMAGIC_BASELEVEL, pItem->m_bGLevel);
	else if(bMakeType == IMT_REFINE)
		nBaseLevel = ITEMAGIC_BASELEVEL;

	LPTMAGIC pTMAGIC = new TMAGIC();
	pTMAGIC->m_pMagic = vItemMagic[sel];
	pTMAGIC->m_wValue = pPlayer->MakeItemMagicValue(nBaseLevel, pTMAGIC->m_pMagic->m_wRareBound);

	if(pTMAGIC->m_wValue)
		pItem->m_mapTMAGIC.insert(MAPTMAGIC::value_type(pTMAGIC->m_pMagic->m_bMagic, pTMAGIC));
	else
	{
		delete pTMAGIC;
		pTMAGIC = NULL;
	}

	vItemMagic.clear();

	return pTMAGIC ? pTMAGIC->m_pMagic->m_bMagic : 0;
}

LPTITEMMAGIC CTMapSvrModule::GetItemMagic(BYTE bKind, BYTE bMagic)
{
	for(DWORD i=0; i<m_vItemMagic[bKind].size(); i++)
	{
		if(m_vItemMagic[bKind][i]->m_bMagic == bMagic)
			return m_vItemMagic[bKind][i];
	}

	return NULL;
}

LPTITEMMAGIC CTMapSvrModule::GetItemMagic( BYTE bMagic)
{
	MAPTITEMMAGIC::iterator finder = m_mapTItemMagic.find(bMagic);

	if( finder != m_mapTItemMagic.end() )
		return (*finder).second;

	return NULL;
}

BYTE CTMapSvrModule::Teleport( CTPlayer *pPlayer,
							   WORD wSpawnID)
{
	MAPTSPAWNPOS::iterator finder = m_mapTSPAWNPOS.find(wSpawnID);

	if( finder != m_mapTSPAWNPOS.end() )
	{
		LPTSPAWNPOS pPOS = (*finder).second;

		if(pPOS)
		{
			return Teleport(
				pPlayer,
				pPlayer->m_bChannel,
				pPOS->m_wMapID,
				pPOS->m_fPosX,
				pPOS->m_fPosY,
				pPOS->m_fPosZ);
		}
	}

	return FALSE;
}

BYTE CTMapSvrModule::Teleport( CTPlayer *pPlayer,
							   BYTE bChannel,
							   WORD wMapID,
							   FLOAT fPosX,
							   FLOAT fPosY,
							   FLOAT fPosZ)
{
	pPlayer->PetRiding(0);

	if( bChannel == pPlayer->m_bChannel &&
		wMapID == pPlayer->m_wMapID &&
		abs(INT(fPosX) - INT(pPlayer->m_fPosX)) < CELL_SIZE &&
		abs(INT(fPosZ) - INT(pPlayer->m_fPosZ)) < CELL_SIZE )
	{
		pPlayer->SendCS_TELEPORT_ACK(
			TPR_SUCCESS,
			0,
			pPlayer->m_dwID,
			pPlayer->m_bType,
			pPlayer->m_wMapID,
			fPosX,
			fPosY,
			fPosZ);
	}
	else
	{
		if(pPlayer->m_dwDuelID)
			SendSM_DUELEND_REQ(pPlayer->m_dwDuelID, pPlayer->m_dwID);

		SendMW_BEGINTELEPORT_ACK(
			pPlayer->m_dwID,
			pPlayer->m_dwKEY,
			FALSE,
			bChannel,
			wMapID,
			fPosX,
			fPosY,
			fPosZ);

		pPlayer->SendCS_BEGINTELEPORT_ACK(bChannel, wMapID);
	}

	return TRUE;
}

void CTMapSvrModule::ExitMAP( CTPlayer *pPlayer, BYTE bTeleport, WORD wGoMapID)
{
	if( pPlayer->m_dealItem.m_bStatus != DEAL_READY )
	{
		pPlayer->SendCS_DEALITEMEND_ACK(
			DEALITEM_NOTARGET,
			pPlayer->m_strNAME);

		SendMW_DEALITEMERROR_ACK(
			pPlayer->m_dealItem.m_strTarget,
			pPlayer->m_strNAME,
			DEALITEM_NOTARGET);

		pPlayer->ClearDealItem();
	}

	if(pPlayer->m_bRPSType)
		RPSReward(pPlayer);

	if(!pPlayer->m_pMAP)
		return;

	CTMap *pTMAP = pPlayer->m_pMAP;
	WORD wMapID = pTMAP->m_wMapID;

	pPlayer->DropGodBall();
	if(pPlayer->IsInCastle())
		pPlayer->SendCS_LEAVECASTLE_ACK();

	if(IsTournamentMap(pPlayer->m_wMapID))
	{
		LPTOURNAMENTPLAYER pTP = FindTournamentPlayer(pPlayer->m_dwID, TRUE);
		if(pTP)
		{
			pTP->m_wMapID = 0;
			if((m_tournament.m_bStep == TNMTSTEP_QFINAL ||
				m_tournament.m_bStep == TNMTSTEP_SFINAL ||
				m_tournament.m_bStep == TNMTSTEP_FINAL) &&
				pPlayer->m_pMAP->m_dwEndTick &&
				GetTournamentMap(pTP->m_dwCharID, m_tournament.m_bGroup) == wMapID &&
				!pPlayer->m_pMAP->CanDoTournament())
			{
				BYTE bRet = 0;
				DWORD dwWinner = 0;
				DWORD dwLoser = 0;

				pPlayer->m_pMAP->EndTournament(bRet, dwWinner, dwLoser);

#ifdef DEF_UDPLOG
				TournamentResultLog(dwWinner, dwLoser, m_tournament.m_bStep);
#endif	//	DEF_UDPLOG
				SendMW_TOURNAMENTRESULT_ACK(
					m_tournament.m_bStep,
					bRet,
					dwWinner,
					dwLoser);
			}
		}
	}

	if(!bTeleport || wMapID != wGoMapID)
		EraseMissionSkill(pPlayer);

	CheckMeetingRoom(pPlayer, FALSE);
	ArenaLeave(pPlayer);

	pPlayer->PetRiding(0);
	pPlayer->ClearSelfMon(FALSE);
	pPlayer->LeaveAllRecallMon();
	pTMAP->LeaveMAP(pPlayer);

	if(pTMAP->m_bType == MAP_INDUN && (!bTeleport || wMapID != wGoMapID))
	{
		MAPTCELL::iterator itTCELL;
		BYTE bDELETE = TRUE;

		for( itTCELL = pTMAP->m_mapTCELL.begin(); itTCELL != pTMAP->m_mapTCELL.end(); itTCELL++)
			if(!(*itTCELL).second->m_mapPLAYER.empty())
			{
				bDELETE = FALSE;
				break;
			}

		if(bDELETE)
		{
			if( pPlayer->m_bMain && !pPlayer->GetPartyID() )
				SendMW_LEAVESOLOMAP_ACK( pPlayer->m_dwID, pPlayer->m_dwKEY);

			DeleteInstanceMAP(pTMAP);
		}
	}

	CheckQuest(
		pPlayer,
		0,
		pPlayer->m_fPosX,
		pPlayer->m_fPosY,
		pPlayer->m_fPosZ,
		wMapID,
		QTT_MAPID,
		TT_LEAVEMAP,
		1);
}

CTRecallMon* CTMapSvrModule::CreateRecallMon( CTPlayer *pPlayer,
                                              LPTMONSTER pTemp,
											  DWORD dwATTR,
                                              DWORD dwID,
											  WORD wPetID,
											  CString strName,
                                              BYTE bLevel,
											  BYTE bClass,
											  BYTE bRace,
											  BYTE bAction,
											  BYTE bStatus,
											  BYTE bMode,
											  DWORD dwMaxHP,
											  DWORD dwMaxMP,
                                              DWORD dwHP,
                                              DWORD dwMP,
											  BYTE bHit,
                                              DWORD dwPysMinPower,
                                              DWORD dwPysMaxPower,
                                              DWORD dwMgMinPower,
                                              DWORD dwMgMaxPower,
                                              WORD wAttackLevel,
											  BYTE bSkillLevel,
                                              FLOAT fPosX,
                                              FLOAT fPosY,
                                              FLOAT fPosZ,
                                              WORD wDir,
                                              DWORD dwTime,
											  VWORD vSkill)
{
	CTRecallMon *pMon = pPlayer->FindRecallMon(dwID);
	if(pMon)
		return pMon;

	if(pPlayer->m_mapRecallMon.size() >= MAX_RECALLMON )
		return NULL;

	LPTLEVEL pLevel = FindTLevel(bLevel);
	LPTSTAT pClass = FindTClass(bClass);
	LPTSTAT pRace = FindTRace(bRace);
	LPTMONATTR pATTR = FindMonAttr(dwATTR);

	if(!pLevel || !pClass || !pRace || !pATTR)
		return NULL;

	if(pTemp->m_bIsSelf)
	{
		// 여기서 LockSelfMonID()로 생성된 ID는 CTSelfObj클래스의 소멸자인 ~CTSelfObj()에서 해제되므로
		// CTSelfObj클래스 인스턴스의 ID를 임의로 수정하면 안됨. 만일 부득이하게 수정이 필요한 경우
		// ReleaseSelfMonID(dwID)를 사용하여 해제한 후 다시 LockSelfMonID()로 재 할당받아 사용하여야만 한다.

		pMon = new CTSelfObj();
		dwID = LockSelfMonID();
		((CTSelfObj *)pMon)->m_dwPysMinPower = dwPysMinPower;
		((CTSelfObj *)pMon)->m_dwPysMaxPower = dwPysMaxPower;
		((CTSelfObj *)pMon)->m_dwMgMinPower = dwMgMinPower;
		((CTSelfObj *)pMon)->m_dwMgMaxPower = dwMgMaxPower;
		((CTSelfObj *)pMon)->m_wAtkAL = wAttackLevel;

		pPlayer->m_mapSelfMon.insert(MAPTSELFOBJ::value_type( dwID, (CTSelfObj *) pMon));
	}
	else
	{
		pMon = new CTRecallMon();
		pPlayer->m_mapRecallMon.insert(MAPTRECALLMON::value_type(dwID, pMon));
	}

	pMon->m_bAtkLevel = pPlayer->m_bLevel;
	pMon->m_bAtkSkillLevel = bSkillLevel;
	pMon->m_wPetID = wPetID;
	pMon->m_strName = strName;

	pMon->m_pMON = pTemp;
	pMon->m_dwID = dwID;
	pMon->m_bLevel = bLevel;
	pMon->m_bIsSpecial = FALSE;

	pMon->m_pTLEVEL = pLevel;
	pMon->m_pTCLASS = pClass;
	pMon->m_pTRACE = pRace;
	pMon->m_pATTR = pATTR;
	pMon->m_bAction = bAction;
	pMon->m_bStatus = bStatus;
	pMon->m_bMode = bMode;

	if(!dwMaxHP)
	{
		pMon->m_dwMaxHP = pATTR->m_dwMaxHP;
		pMon->m_dwMaxMP = pATTR->m_dwMaxMP;
	}
	else
	{
		pMon->m_dwMaxHP = dwMaxHP;
		pMon->m_dwMaxMP = dwMaxMP;
	}

	if(pMon->m_dwHP)
	{
		pMon->m_dwMP = dwMP;
		pMon->m_dwHP = dwHP;
	}
	else
	{
		pMon->m_dwHP = pATTR->m_dwMaxHP;
		pMon->m_dwMP = pATTR->m_dwMaxMP;
	}

	if(pMon->m_mapTSKILL.empty())
		for(DWORD ms=0; ms<vSkill.size(); ms++)
		{
			CTSkillTemp * pTempSkill = FindTSkill(vSkill[ms]);
			if(pTempSkill)
			{
				CTSkill * pSkill = new CTSkill();
				pSkill->m_pTSKILL = pTempSkill;
				pSkill->m_bLevel = min(bSkillLevel, pSkill->m_pTSKILL->m_bMaxLevel);
				pMon->m_mapTSKILL.insert(MAPTSKILL::value_type(vSkill[ms], pSkill));
			}
		}

	pMon->m_bEssential = FALSE;
	pMon->m_bChannel = pPlayer->m_bChannel;
	pMon->m_pSPAWN = NULL;
	pMon->m_pMAP = NULL;

	pMon->m_dwHostID = pPlayer->m_dwID;
	pMon->m_wMapID = pPlayer->m_wMapID;
	pMon->m_bCountry = pPlayer->m_bCountry;
	pMon->m_bAidCountry = pPlayer->m_bAidCountry;
	pMon->m_bHit = bHit;
	pMon->m_fPosX = fPosX;
	pMon->m_fPosY = fPosY;
	pMon->m_fPosZ = fPosZ;
	pMon->m_wDIR = wDir;

	if(dwTime)
	{
		pMon->m_dwRecallTick = m_dwTick;
		pMon->m_dwDurationTick = dwTime;
	}

	if(pPlayer->m_pMAP && pPlayer->m_bMain &&
		!pMon->m_pMON->m_wID)//분신
	{
		VTMONSTER vMONS;
		vMONS.clear();

		pPlayer->m_pMAP->GetNeighbor(&vMONS, pPlayer->m_fPosX, pPlayer->m_fPosZ);

		while(!vMONS.empty())
		{
			CTMonster * pAGGROMON = vMONS.back();
			vMONS.pop_back();
			DWORD dwAggro = pAGGROMON->FindAggro(pPlayer->m_dwID, pPlayer->m_bType);
			if(dwAggro)
				pAGGROMON->AddAggro(
					pPlayer->m_dwID,
					pMon->m_dwID,
					pMon->m_bType,
					pMon->GetWarCountry(),
					dwAggro);
		}
	}

	if(pPlayer->FindMaintainSkill(TREVIVAL_SKILL))
	{
		CTSkillTemp * pTemp = FindTSkill(TREVIVAL_SKILL);
		if(pTemp)
		{
			CTSkill * pSkill = new CTSkill();
			pSkill->m_bLevel = 1;
			pSkill->m_pTSKILL = pTemp;

			CTSkill * pMaintain = pMon->MaintainSkill(
				pSkill,
				0,
				pPlayer->m_dwID,
				pPlayer->m_bType,
				pMon->m_dwID,
				pMon->m_bType,
				0,0,0,0,0,
				TRUE,
				pMon->GetWarCountry(),
				0,0,0,
				pMon->m_fPosX,
				pMon->m_fPosY,
				pMon->m_fPosZ);

			if(pMaintain)
				pMon->PushMaintainSkill(pMaintain);

			delete pSkill;
		}
	}

	return pMon;
}

LPTBATTLEZONE CTMapSvrModule::FindBattleZone(WORD wLocalID)
{
	MAPTBATTLEZONE::iterator it = m_mapTBATTLEZONE.find(wLocalID);
	if(it!=m_mapTBATTLEZONE.end())
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindOccupationZone(WORD wLocalID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wLocalID);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_OCCUPATION &&
		(*it).second->m_bValid)
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindCastleZone(WORD wLocalID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wLocalID);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_CASTLE &&
		(*it).second->m_bValid)
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindMissionZone(WORD wLocalID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wLocalID);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_MISSION &&
		(*it).second->m_bValid)
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindLocal(WORD wLocalID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wLocalID);
	if( it != m_mapTLOCAL.end() )
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindOccupation(WORD wLocal)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wLocal);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_OCCUPATION)
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindCastle(WORD wCastleID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wCastleID);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_CASTLE)
		return (*it).second;
	else
		return NULL;
}

LPTLOCAL CTMapSvrModule::FindMission(WORD wMissionID)
{
	MAPTLOCAL::iterator it = m_mapTLOCAL.find(wMissionID);
	if( it != m_mapTLOCAL.end() &&
		(*it).second->m_bType == LOCAL_MISSION)
		return (*it).second;
	else
		return NULL;
}

BYTE CTMapSvrModule::ChangeSwitch( CTMap *pMap,
								   DWORD dwSwitchID,
								   BYTE bOpen)
{
	if(!pMap)
		return FALSE;

	LPTSWITCH pSwitch = pMap->FindSwitch(dwSwitchID);
	if(!pSwitch)
		return FALSE;

	switch(bOpen)
	{
	case SWC_TOGGLE:
		pSwitch->m_bOpened = !pSwitch->m_bOpened;
		break;
	case SWC_OPEN:
		if(pSwitch->m_bOpened == TRUE)
			return FALSE;
		pSwitch->m_bOpened = TRUE;
		break;
	case SWC_CLOSE:
		if(pSwitch->m_bOpened == FALSE)
			return FALSE;
		pSwitch->m_bOpened = FALSE;
		break;
	}

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pMap->GetNeighbor(
		&vPLAYERS,
		pSwitch->m_wPosX,
		pSwitch->m_wPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_SWITCHCHANGE_ACK(SWITCH_SUCCESS, dwSwitchID, pSwitch->m_bOpened);
		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();

	for(DWORD i=0; i<pSwitch->m_vGate.size(); i++)
	{
		LPTGATE pGate = pSwitch->m_vGate[i];
		BYTE bGateChange = TRUE;
		if(pGate->m_bType == GT_MULTISWITCH)
		{
			for(DWORD k=0; k<pGate->m_vSwitch.size(); k++)
			{
				if(pGate->m_vSwitch[k]->m_bOpened != pSwitch->m_bOpened)
				{
					bGateChange = FALSE;
					break;
				}
			}
		}

		if(!bGateChange)
			continue;

		pGate->m_bOpened = pSwitch->m_bOpened;

		vPLAYERS.clear();

		pMap->GetNeighbor(
			&vPLAYERS,
			pGate->m_wPosX,
			pGate->m_wPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();

			pChar->SendCS_GATECHANGE_ACK(pGate->m_dwGateID, pSwitch->m_bOpened);
			vPLAYERS.pop_back();
		}

		vPLAYERS.clear();
	}

	return TRUE;
}

void CTMapSvrModule::ResetLocal( LPTLOCAL pLocal,
								 BYTE bCountry,
								 DWORD dwGuildID,
								 CString strGuildName,
								 BYTE bType)
{
	if(pLocal->m_dwGuild != dwGuildID)
	{
		pLocal->m_strHero = NAME_NULL;
		pLocal->m_timeHero = 0;
	}

	DWORD dwPrevGuild = pLocal->m_dwGuild;
	pLocal->m_bCountry = bCountry;
	pLocal->m_dwGuild = dwGuildID;
	pLocal->m_strGuild = strGuildName;
	pLocal->m_timeNextDefend += DAY_ONE;
	pLocal->m_bLastOccType = bType;
	CTime curtime(m_timeCurrent);
	int nDay = curtime.GetDayOfWeek();
	pLocal->m_occupyGuild[nDay-1] = dwGuildID;
	pLocal->m_occupyType[nDay-1] = bType;
	LPTLOCAL pCastle = FindCastle(pLocal->m_pZone->m_wCastle);
	if(pCastle && pCastle->m_bDay == nDay+1)
		pLocal->m_timeNextDefend += DAY_ONE;

	if(!pLocal->m_bValid)
		return;

	if(pCastle)
		SendMW_CASTLEWARINFO_ACK(pCastle);

	PvPEvent(PVPE_ENTRY, pLocal, 0);

	if(dwPrevGuild != dwGuildID)
		PvPEvent(PVPE_WIN, pLocal, dwPrevGuild);
	else
		PvPEvent(PVPE_DEFEND, pLocal, dwPrevGuild);
}

void CTMapSvrModule::ResetMission( LPTLOCAL pLocal, BYTE bCountry, BYTE bType)
{
	pLocal->m_bCountry = bCountry;
	if(pLocal->m_bStatus != BS_NORMAL)
		pLocal->m_bStatus = BS_PEACE;
}

void CTMapSvrModule::NotifyLocalInfo(BYTE bType, WORD wID, DWORD dwSecond, CString strLocalName, CString strGuildName)
{
	MAPPLAYER::iterator it;
	for(it=m_mapPLAYER.begin(); it!=m_mapPLAYER.end(); it++)
	{
		CTPlayer * pPlayer = (*it).second;
		if(pPlayer->m_bMain && pPlayer->m_pMAP)
			pPlayer->SendCS_SYSTEMMSG_ACK(bType, wID, dwSecond, strLocalName, strGuildName);
	}
}

void CTMapSvrModule::CheckLocalEnv(CTPlayer * pPlayer)
{
	if(!pPlayer->m_pLocal)
		return;
	LPTLOCAL pLocal = pPlayer->m_pLocal;

	if( pLocal->m_bStatus == BS_BATTLE && pPlayer->GetWarCountry() != pLocal->m_bCountry && pLocal->m_bGateOpened )
	{
		MAPTMONSPAWNTEMP::iterator itMS = m_mapTMONSPAWN.find(pLocal->m_pZone->m_wBossSpawnID);

		if( itMS != _AtlModule.m_mapTMONSPAWN.end() )
		{
			if( GetDistance( pPlayer->m_fPosX, pPlayer->m_fPosZ, (*itMS).second->m_fPosX, (*itMS).second->m_fPosZ) < 30 )
			{
				MAPTMONSPAWN::iterator itCM = pPlayer->m_pMAP->m_mapTMONSPAWN.find(pLocal->m_pZone->m_wBossSpawnID);

				if( itCM == pPlayer->m_pMAP->m_mapTMONSPAWN.end() || (*itCM).second->m_vTMON.size() == 0 )
				{
					pPlayer->m_pMAP->AddMonSpawn(
						(*itMS).second,
						pPlayer->m_pMAP->m_bChannel,
						pLocal->m_bCountry);

					itCM = pPlayer->m_pMAP->m_mapTMONSPAWN.find(pLocal->m_pZone->m_wBossSpawnID);
					if( itCM != pPlayer->m_pMAP->m_mapTMONSPAWN.end() )
						(*itCM).second->m_bStatus = MONSPAWN_SUSPEND;
				}
			}
		}
	}
}

DWORD CTMapSvrModule::GetPostMoney(BYTE bType)
{
	DWORD dwMoney = 0;
	switch(bType)
	{
	case POST_NORMAL:
		dwMoney = POST_COST_NORMAL;
		break;
	case POST_PACKATE:
		dwMoney = POST_COST_PACKAGE;
		break;
	case POST_BILLS:
		dwMoney = POST_COST_BILLS;
		break;
	case POST_RETURN:
		dwMoney = POST_COST_RETURN;
		break;
	case POST_PAYMENT:
		dwMoney = POST_COST_PAYMENT;
		break;
	case POST_NPC:
		dwMoney = POST_COST_NPC;
		break;
	case POST_OPERATOR:
		dwMoney = POST_COST_OPERATOR;
		break;
	case POST_CASH:
		dwMoney = POST_COST_CASH;
		break;
	}

	return dwMoney;
}

BYTE CTMapSvrModule::PostReturn( CString strChar, 
								 DWORD dwPostID,
								 DWORD dwNewID,
								 CString strRecvName,
								 BYTE bType,
								 CString strSender,
								 CString strTitle)
{
	CTPlayer * pPlayer = FindPlayer(strRecvName);
	if(pPlayer)
	{
		pPlayer->SendCS_POSTRECV_ACK(dwNewID, FALSE, strSender, strTitle, bType, m_timeCurrent);
	}
	else
		SendMW_POSTRECV_ACK(dwNewID, strSender, strRecvName, strTitle, bType);

	CTPlayer * pChar = FindPlayer(strChar);
	if(!pChar)
		return 0;

	if(!pChar->m_pPost || pChar->m_pPost->m_dwPostID != dwPostID)
		return 0;

	pChar->m_pPost->m_dwGold = 0;
	pChar->m_pPost->m_dwSilver = 0;
	pChar->m_pPost->m_dwCooper = 0;
	pChar->m_pPost->m_bRead = TRUE;

	SendSM_POSTBILLERASE_REQ(pChar->m_dwID, dwPostID);

	switch(bType)
	{
	case POST_PAYMENT:
		pChar->SendCS_POSTRETURN_ACK(POST_SUCCESS);
		break;
	case POST_RETURN:
		pChar->SendCS_POSTDEL_ACK(dwPostID);
		break;
	}

	return 0;
}

void CTMapSvrModule::InitMap( CTPlayer *pPlayer)
{
	CTMap *pTMAP = FindTMap(
		pPlayer->m_bChannel,
		pPlayer->m_wPartyID,
		pPlayer->m_wMapID);

	if(!pTMAP)
	{
		if(!IsIndunMap(pPlayer->m_wMapID))
			return;
		else if(pPlayer->m_wPartyID)
		{
			pTMAP = CreateInstanceMAP(
				pPlayer->m_bChannel,
				pPlayer->m_wPartyID,
				pPlayer->m_wMapID,
				TCONTRY_N);

			if(!pTMAP)
				return;
		}
		else
		{
			if(pPlayer->m_bMain)
				SendMW_ENTERSOLOMAP_ACK( pPlayer->m_dwID, pPlayer->m_dwKEY);

			return;
		}
	}

	if(IsCastleMap(pTMAP))
	{
		LPTLOCAL pCastle = pTMAP->m_pCastle;
		if(!pCastle)
			return;

		if(IsOperator(pPlayer->m_dwID) ||
			(CanEnterCastle(pCastle) &&
			(pPlayer->GetGuild() == pCastle->m_dwDefGuildID || pPlayer->GetGuild() == pCastle->m_dwAtkGuildID) &&
			pPlayer->m_wCastle == pCastle->m_wLocal && pPlayer->m_bCamp))
			pPlayer->SendCS_ENTERCASTLE_ACK(pPlayer->m_wCastle, pPlayer->m_bCamp, pCastle->m_strAtkName, pCastle->m_strDefName);
		else
		{
			Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			return;
		}
	}
	else if(GetLoungeMapID() == pPlayer->m_wMapID)
	{
		if(!CanDoTournament(TNMTSTEP_ENTER))
		{
			Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			return;
		}
	}
	else if(IsTournamentMap(pPlayer->m_wMapID))
	{
		if(!CanDoTournament(TNMTSTEP_ENTER))
		{
			Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			return;
		}

		LPTOURNAMENTPLAYER pTP = FindTournamentPlayer(pPlayer->m_dwID);
		if(pTP)
		{
			WORD wTNMTMap = GetTournamentMap(pPlayer->m_dwID, m_tournament.m_bGroup);

			if((!wTNMTMap && pPlayer->m_fPosY < 7) || (wTNMTMap && wTNMTMap != pPlayer->m_wMapID))
			{
				Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
				return;
			}

			if(wTNMTMap)
			{
				pPlayer->m_aftermath.m_bStep = 0;
				pPlayer->m_aftermath.m_fReuseInc = 0;
				pPlayer->m_aftermath.m_fStatDec = 0;
				pPlayer->ResetCoolTime(0);
				pPlayer->ReleaseMaintain();
				pTP->m_wMapID = pPlayer->m_wMapID;

				pPlayer->SendCS_CHARSTATINFO_ACK(pPlayer);
			}
		}
		else if(pPlayer->m_fPosY < 7)
		{
			Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			return;
		}

		CTRecallMon * pRecall = pPlayer->FindRecallPet();
		if(pRecall)
			SendMW_RECALLMONDEL_ACK(pPlayer->m_dwID, pPlayer->m_dwKEY, pRecall->m_dwID);

		pPlayer->SendCS_TOURNAMENTENTER_ACK(pTMAP);
	}

	if(pPlayer->m_wMapID && pPlayer->m_dwLastDestination)
	{
		if(CheckMapLevel(pPlayer) || CheckMapGuild(pPlayer))
		{
			Teleport(pPlayer, pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID);
			return;
		}
	}

	MAPTRECALLMON::iterator itTMON;

	for( itTMON = pPlayer->m_mapRecallMon.begin(); itTMON != pPlayer->m_mapRecallMon.end(); itTMON++)
	{
		(*itTMON).second->m_bAction = TA_STAND;
		(*itTMON).second->m_bStatus = OS_WAKEUP;
		(*itTMON).second->m_bMode = pPlayer->m_bMode;
		(*itTMON).second->m_fPosX = pPlayer->m_fPosX - 2.0f * sinf(FLOAT(pPlayer->m_wDIR) * FLOAT(M_PI) / 900.0f);
		(*itTMON).second->m_fPosY = pPlayer->m_fPosY;
		(*itTMON).second->m_fPosZ = pPlayer->m_fPosZ - 2.0f * cosf(FLOAT(pPlayer->m_wDIR) * FLOAT(M_PI) / 900.0f);
		(*itTMON).second->m_wMapID = pPlayer->m_wMapID;
		(*itTMON).second->m_bChannel = pPlayer->m_bChannel;

		pTMAP->EnterMAP((*itTMON).second);
	}

	pTMAP->EnterMAP( pPlayer, TRUE);

	CString strFindName = pPlayer->m_strNAME;
	strFindName.MakeUpper();
	MAPPLAYERNAME::iterator it = m_mapPLAYERNAME.find(strFindName);
	if(it != m_mapPLAYERNAME.end())
		m_mapPLAYERNAME.erase(it);

	m_mapPLAYERNAME.insert( MAPPLAYERNAME::value_type( strFindName, pPlayer));

	if(!pPlayer->m_bMain)
		return;

	if(pPlayer->m_questlist_possible.GetID())
	{
		pPlayer->SendQuestListPossible(&pPlayer->m_questlist_possible);
		pPlayer->m_questlist_possible.Clear();
	}

	WORD wBonus = 0;
	BYTE bExp = pPlayer->IsExpBenefit(wBonus);

	if(!bExp)
	{
		for(DWORD i=0; i<pPlayer->m_vMaintainSkill.size(); i++)
		{
			if(pPlayer->m_vMaintainSkill[i]->m_pTSKILL->m_wID == TPCBANG_SKILL)
			{
				pPlayer->EraseMaintainSkill(pPlayer->m_vMaintainSkill[i], i);
				break;
			}
		}
	}
	
	if(pPlayer->HangPremiumItem())
	{
		VPLAYER vPLAYERS;
		vPLAYERS.clear();

		pPlayer->m_pMAP->GetNeighbor(
			&vPLAYERS,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		while(!vPLAYERS.empty())
		{
			CTPlayer *pChar = vPLAYERS.back();
			pChar->SendCS_RESETPCBANG_ACK(pPlayer->m_dwID, pPlayer->m_bInPcBang);
			vPLAYERS.pop_back();
		}
	}

	if(!pPlayer->m_bStartAct)
	{
		pPlayer->ForceMaintain(TREVIVAL_SKILL, pPlayer->m_dwID, pPlayer->m_bType, pPlayer->m_dwID, pPlayer->m_bType, 0);
		pPlayer->RecallRevival();
	}
}

void CTMapSvrModule::ReleaseSelfMonID( DWORD dwID)
{
	MAPDWORD::iterator finder = m_mapTSELFMONID.find(dwID);

	if( finder != m_mapTSELFMONID.end() )
		m_mapTSELFMONID.erase(finder);
}

DWORD CTMapSvrModule::LockSelfMonID()
{
	static WORD wSelfMonID = 0;
	DWORD dwID = MAKELONG( wSelfMonID, WORD(m_bServerID));

	while( !dwID || m_mapTSELFMONID.find(dwID) != m_mapTSELFMONID.end() )
	{
		wSelfMonID++;
		dwID = MAKELONG( wSelfMonID, WORD(m_bServerID));
	}
	m_mapTSELFMONID.insert( MAPDWORD::value_type( dwID, dwID));

	return dwID;
}

void CTMapSvrModule::ReviewMonItemList(DWORD dwID, CTMonster * pMON)
{
	MAPPLAYER::iterator find = _AtlModule.m_mapPLAYER.find(dwID);
	if( find!=_AtlModule.m_mapPLAYER.end() )
	{
		CTInven * pInven = pMON->FindTInven(INVEN_DEFAULT);
		if(pInven)
			(*find).second->SendCS_MONITEMLIST_ACK(MIL_SUCCESS, pMON->m_dwID, pMON->m_dwMoney, pInven, TRUE);
	}
}

void CTMapSvrModule::PartyMonItemTake(CTPlayer * pPlayer, CTItem *pItem)
{
	if(!pPlayer->m_pMAP)
		return;

	VPLAYER vPlayer;
	vPlayer.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPlayer,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	while(vPlayer.size())
	{
		CTPlayer *pChar = vPlayer.back();
		vPlayer.pop_back();

		if( pChar->m_dwID != pPlayer->m_dwID &&
			pChar->GetPartyID() == pPlayer->GetPartyID() )
		{
			pChar->SendCS_PARTYITEMTAKE_ACK(
				pPlayer->m_dwID,
				pItem);
		}
	}

	vPlayer.clear();
}

void CTMapSvrModule::PartyChiefItemTake(CTPlayer * pChief, CTPlayer * pTarget, CTMonster * pMon, CTInven *pInven, CTItem *pItem)
{
	if(pChief->m_bMain)
	{
		VTITEM vItem;
		vItem.clear();

		vItem.push_back(pItem);

		BYTE bEraseItemID = pItem->m_bItemID;
		WORD wItemID = pItem->m_wItemID;
		BYTE bCount = pItem->m_bCount;
		CTItem logItem;
		logItem.Copy(pItem, FALSE);

		if(pChief->CanPush(&vItem, 0))
		{
			pChief->PushTItem(&vItem);

#ifdef	DEF_UDPLOG
			m_pUdpSocket->LogItemByMonster(LOGMAP_ITEMBYMONPARTY, pChief, pMon, &logItem);
#endif	//	DEF_UDPLOG

			CheckQuest(
				pChief,
				0,
				pChief->m_fPosX,
				pChief->m_fPosY,
				pChief->m_fPosZ,
				wItemID,
				QTT_GETITEM,
				TT_GETITEM,
				bCount);

			pMon->EraseItem(bEraseItemID);
			pChief->SendCS_MONITEMTAKE_ACK(MIT_SUCCESS);
			pChief->SendCS_GETITEM_ACK(&logItem);

			if( pMon->m_bKeeperType == OWNER_PARTY )
				PartyMonItemTake(pChief, &logItem);
		}
		else
			pChief->SendCS_MONITEMTAKE_ACK(MIT_FULLINVEN);

		vItem.clear();
	}
	else
	{
		SendMW_ADDITEM_ACK(
			pChief->m_dwID,
			pChief->m_dwKEY,
			GetSvrID(),
			pChief->m_bChannel,
			pChief->m_pMAP->m_wMapID,
			pMon->m_dwID,
			0, INVALID_SLOT,
			pItem);

		pMon->EraseItem(pItem->m_bItemID);
		pMon->AddItemList(pItem);
	}

	pTarget->SendCS_MONITEMLIST_ACK(MIL_SUCCESS, pMon->m_dwID, pMon->m_dwMoney, pInven, TRUE);
}

// 이벤트 변신물약
CTSkillTemp * CTMapSvrModule::RandTransSkill(CTSkillTemp * pTemp)
{
	LPTSKILLDATA pData = NULL;
	for(DWORD i=0; i<pTemp->m_vData.size(); i++)
	{
		if(	pTemp->m_vData[i]->m_bType == SDT_TRANS &&
			pTemp->m_vData[i]->m_bExec == SDT_TRANS_RANDOM)
		{
			pData = pTemp->m_vData[i];
			break;
		}
	}

	if(!pData || !pData->m_wValue || !pData->m_wValueInc)
		return NULL;

	pTemp = FindTSkill(pData->m_wValue + rand() % pData->m_wValueInc);
	if(!pTemp || !pTemp->IsTrans())
		return NULL;

	return pTemp;
}

CTSkillTemp * CTMapSvrModule::RandBuffSkill(CTSkillTemp* pTemp)  
{
	LPTSKILLDATA pData = NULL;

	for(DWORD i=0; i<pTemp->m_vData.size(); i++)
	{
		if(	pTemp->m_vData[i]->m_bType == SDT_STATUS &&
			pTemp->m_vData[i]->m_bExec == SDT_STATUS_RANDOM)
		{
			pData = pTemp->m_vData[i];
			break;
		}
	}

	if(!pData || !pData->m_wValue || !pData->m_wValueInc)
		return NULL;

	pTemp = FindTSkill(pData->m_wValue + rand() % pData->m_wValueInc);
	if(!pTemp || !pTemp->IsBuffType())
		return NULL;

	return pTemp;
}

DWORD CTMapSvrModule::GetDuelID()
{
	m_dwDuelID++;
	return m_dwDuelID;
}

BYTE CTMapSvrModule::MonMoneyTake(CTPlayer * pPlayer, DWORD dwMonID)
{
	if(!pPlayer->m_pMAP)
		return FALSE;

	if( pPlayer->m_dealItem.m_bStatus >= DEAL_START )
	{
		pPlayer->SendCS_MONITEMTAKE_ACK(MIT_DEALING);
		return FALSE;
	}

	CTMonster *pMon = pPlayer->m_pMAP->FindMonster(dwMonID);
	if( !pMon )
		return FALSE;

	if( (pMon->m_bKeeperType == OWNER_PARTY && pMon->m_dwKeeperID != pPlayer->GetPartyID()) ||
		(pMon->m_bKeeperType == OWNER_PRIVATE && pMon->m_dwKeeperID != pPlayer->m_dwID) )
	{
		pPlayer->SendCS_MONITEMLIST_ACK(
			MIL_CANTACCESS,
			dwMonID, 0,
			NULL,
			FALSE);

		return FALSE;
	}

	if(pMon->m_dwInvenLock &&
		pMon->m_dwInvenLock != pPlayer->m_dwID)
	{
		pPlayer->SendCS_MONITEMLIST_ACK(
			MIL_CANTACCESS,
			dwMonID, 0,
			NULL,
			FALSE);

		return FALSE;
	}

	if(pMon->m_bKeeperType == OWNER_PARTY)
	{
		if(pPlayer->m_bPartyType == PT_HUNTER &&
			pPlayer->m_dwID != pMon->GetHunter())
		{
			pPlayer->SendCS_MONITEMLIST_ACK(
				MIL_CANTACCESS,
				dwMonID, 0,
				NULL,
				FALSE);

			return FALSE;
		}
	}

	CTInven *pInven = pMon->FindTInven(INVEN_DEFAULT);
	if(!pInven)
		return FALSE;

	if( !pMon->m_dwMoney )
		return TRUE;

	switch(pMon->m_bKeeperType)
	{
	case OWNER_PARTY	:
		if( pMon->m_dwKeeperID == pPlayer->GetPartyID() )
		{
			WORD wTotalLevel = 0;

			VPLAYER vPlayer;
			VPLAYER vParty;

			vPlayer.clear();
			vParty.clear();

			pPlayer->m_pMAP->GetNeighbor(
				&vPlayer,
				pMon->m_fPosX,
				pMon->m_fPosZ);

			for( DWORD i=0; i<DWORD(vPlayer.size()); i++)
				if( pMon->m_dwKeeperID == vPlayer[i]->GetPartyID() )
				{
					wTotalLevel += vPlayer[i]->m_bLevel;

					if( pPlayer != vPlayer[i] )
						vParty.push_back(vPlayer[i]);
				}

			DWORD dwMonMoney = pMon->m_dwMoney;
			for(int i=0; i<DWORD(vParty.size()); i++)
			{
				if(wTotalLevel == 0) wTotalLevel = 1;
				DWORD dwMoney = pMon->m_dwMoney * vParty[i]->m_bLevel / wTotalLevel;

				if(dwMoney <= dwMonMoney)
				{
					if(vParty[i]->m_bMain)
					{
						vParty[i]->EarnMoney(dwMoney);
#ifdef DEF_UDPLOG
						m_pUdpSocket->LogMoney(LOGMAP_MONMONEYPARTY, vParty[i], pMon,  dwMoney );
#endif

						vParty[i]->SendCS_MONEY_ACK();
					}
					else
					{
						SendMW_TAKEMONMONEY_ACK(
							vParty[i]->m_dwID,
							vParty[i]->m_dwKEY,
							dwMoney);
#ifdef DEF_UDPLOG
						m_pUdpSocket->LogMoney(LOGMAP_MONMONEYPARTY, vParty[i], pMon,  dwMoney, NULL, TRUE );
#endif
					}

					dwMonMoney -= dwMoney;
				}
			}
			
			pMon->m_dwMoney = dwMonMoney;

			if(pMon->m_dwMoney)
				if(pPlayer->m_bMain)
				{
					pPlayer->EarnMoney(pMon->m_dwMoney);
#ifdef DEF_UDPLOG
						m_pUdpSocket->LogMoney(LOGMAP_MONMONEYPARTY, pPlayer, pMon,  pMon->m_dwMoney );
#endif
					pPlayer->SendCS_MONEY_ACK();
					pMon->m_dwMoney = 0;

					pPlayer->SendCS_MONITEMLIST_ACK(
						MIL_SUCCESS,
						pMon->m_dwID,
						pMon->m_dwMoney,
						pInven,
						TRUE);
				}
				else
				{
					SendMW_TAKEMONMONEY_ACK(
						pPlayer->m_dwID,
						pPlayer->m_dwKEY,
						pMon->m_dwMoney);
#ifdef DEF_UDPLOG
						m_pUdpSocket->LogMoney(LOGMAP_MONMONEYPARTY, pPlayer, pMon,  pMon->m_dwMoney, NULL, TRUE );
#endif
					pMon->m_dwMoney = 0;

					pPlayer->SendCS_MONITEMLIST_ACK(
						MIL_SUCCESS,
						pMon->m_dwID,
						pMon->m_dwMoney,
						pInven,
						TRUE);
				}

			vPlayer.clear();
			vParty.clear();
		}

		break;

	case OWNER_PRIVATE	:
		if( pMon->m_dwKeeperID == pPlayer->m_dwID )
			if(pPlayer->m_bMain)
			{
				pPlayer->EarnMoney(pMon->m_dwMoney);

#ifdef DEF_UDPLOG
				m_pUdpSocket->LogMoney(LOGMAP_MONMONEYSOLOE, pPlayer, pMon, pMon->m_dwMoney);
#endif 

				pPlayer->SendCS_MONEY_ACK();
				pMon->m_dwMoney = 0;

				pPlayer->SendCS_MONITEMLIST_ACK(
					MIL_SUCCESS,
					pMon->m_dwID,
					pMon->m_dwMoney,
					pInven,
					TRUE);
			}
			else
			{
				SendMW_TAKEMONMONEY_ACK(
					pPlayer->m_dwID,
					pPlayer->m_dwKEY,
					pMon->m_dwMoney);

#ifdef DEF_UDPLOG
				m_pUdpSocket->LogMoney(LOGMAP_MONMONEYSOLOE, pPlayer, pMon, pMon->m_dwMoney, NULL, TRUE);
#endif

				pMon->m_dwMoney = 0;

				pPlayer->SendCS_MONITEMLIST_ACK(
					MIL_SUCCESS,
					pMon->m_dwID,
					pMon->m_dwMoney,
					pInven,
					TRUE);
			}

		break;
	}

	return TRUE;
}

BYTE CTMapSvrModule::MonItemTake(CTPlayer * pPlayer,
                                  DWORD dwMonID,
                                  BYTE bItemID,
                                  BYTE bInvenID,
                                  BYTE bSlotID)
{
	if(pPlayer->m_dealItem.m_bStatus >= DEAL_START)
	{
		pPlayer->SendCS_MONITEMTAKE_ACK(MIT_DEALING);
		return FALSE;
	}

	if(!pPlayer->m_pMAP)
		return FALSE;

	CTMonster *pMon = pPlayer->m_pMAP->FindMonster(dwMonID);
	if(!pMon)
		return FALSE;

	CTInven *pInven = pMon->FindTInven(INVEN_DEFAULT);
	if(!pInven)
	{
		pPlayer->SendCS_MONITEMTAKE_ACK(MIT_NOTFOUND);
		return FALSE;
	}

	VPLAYER vPlayer;
	vPlayer.clear();

	pPlayer->m_pMAP->GetNeighbor(
		&vPlayer,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	MAPTITEM::iterator it, itDel;

	for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end();)
	{
		itDel = it;
		it++;
		CTItem *pItem = NULL;
		if(bSlotID != INVALID_SLOT)
			pItem = pInven->FindTItem(bItemID);
		else
		{
			pItem = (*itDel).second;
			bItemID = pItem->m_bItemID;
		}

		if(!pItem)
		{
			pPlayer->SendCS_MONITEMTAKE_ACK(MIT_NOTFOUND);
			return FALSE;
		}

		if(pItem->m_bMaxRouting)
		{
			if(bSlotID != INVALID_SLOT)
				return FALSE;
			else
				continue;
		}

		if(pItem->m_dwOwnerID != 0 && pItem->m_dwOwnerID != pPlayer->m_dwID)
		{
			if(bSlotID != INVALID_SLOT)
			{
				pPlayer->SendCS_MONITEMTAKE_ACK(MIT_NOTFOUND);
				return FALSE;
			}
			else
				continue;
		}

		if( pMon->m_bKeeperType == OWNER_PARTY && pItem->m_dwOwnerID == 0)
		{
			if(pPlayer->m_bPartyType == PT_LOTTERY)
			{
				pItem->m_bMaxRouting++;
				pItem->m_mapRoutingJoiner.insert(MAPDWORD::value_type(pPlayer->m_dwID, pPlayer->m_dwKEY));

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					CTPlayer *pChar = vPlayer[i];

					if( pChar->m_dwID != pPlayer->m_dwID &&
						pChar->GetPartyID() == pMon->m_dwKeeperID )
					{
						pItem->m_bMaxRouting++;
						pChar->SendCS_MONITEMLOTTERY_ACK(
							dwMonID,
							pItem);
					}
				}

				if(pItem->m_bMaxRouting == 1)
					pMon->Lottery(pItem);
				else
					pMon->OnEvent(AT_LOTTERY, 0, 0, bItemID, 0);

				if(bSlotID != INVALID_SLOT)
					break;
				else
					continue;
			}
			else if(pPlayer->m_bPartyType == PT_CHIEF)
			{
				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					CTPlayer *pChar = vPlayer[i];

					if( pChar->GetPartyID() == pPlayer->GetPartyID() &&
						pChar->m_dwID == pPlayer->GetPartyChiefID() )
					{
						PartyChiefItemTake(pChar, pPlayer, pMon, pInven, pItem);
						break;
					}
				}

				if(bSlotID != INVALID_SLOT)
					break;
				else
					continue;
			}
			else if(pPlayer->m_bPartyType == PT_HUNTER &&
				pPlayer->m_dwID != pMon->GetHunter())
			{
				pPlayer->SendCS_MONITEMTAKE_ACK(MIT_AUTHORITY);
				if(bSlotID != INVALID_SLOT)
					break;
				else
					continue;
			}
			else if(pPlayer->m_bPartyType == PT_ORDER)
			{
				VDWORD vMember;
				vMember.clear();

				vMember.push_back(pPlayer->m_dwID);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					CTPlayer *pChar = vPlayer[i];

					if( pChar->GetPartyID() == pPlayer->GetPartyID() &&
						pChar->m_dwID != pPlayer->m_dwID )
						vMember.push_back(pChar->m_dwID);
				}

				if(vMember.size() > 1)
				{
					SendMW_PARTYORDERTAKEITEM_ACK(
						pPlayer->m_dwID,
						pPlayer->m_dwKEY,
						pPlayer->GetPartyID(),
						GetSvrID(),
						pPlayer->m_bChannel,
						pPlayer->m_pMAP->m_wMapID,
						pMon->m_dwID,
						pMon->m_pMON->m_wID,  
						vMember,
						pItem);

					pMon->EraseItem(bItemID);
					pMon->AddItemList(pItem);

					vMember.clear();

					if(bSlotID != INVALID_SLOT)
						break;
					else
						continue;
				}
			}
		}

		if(pPlayer->m_bMain)
		{
			VTITEM vItem;
			vItem.clear();
			vItem.push_back(pItem);

			BYTE bEraseItemID = pItem->m_bItemID;
			WORD wItemID = pItem->m_wItemID;
			BYTE bCount = pItem->m_bCount;
			DWORD dwOwnerID = pItem->m_dwOwnerID;
			CTItem logItem;
			logItem.Copy(pItem, FALSE);

			if((bSlotID != INVALID_SLOT && pPlayer->PushTItem(pItem, bInvenID, bSlotID)) || 
				bSlotID == INVALID_SLOT && pPlayer->CanPush(&vItem, 0))
			{
				if(bSlotID == INVALID_SLOT)
					pPlayer->PushTItem(&vItem);

	#ifdef DEF_UDPLOG
				if( pMon->m_bKeeperType == OWNER_PARTY && dwOwnerID == 0 )
					m_pUdpSocket->LogItemByMonster(LOGMAP_ITEMBYMONPARTY, pPlayer, pMon, &logItem);
				else
					m_pUdpSocket->LogItemByMonster(LOGMAP_ITEMBYMONSOLO, pPlayer, pMon, &logItem);
	#endif	//	DEF_UDPLOG

				CheckQuest(
					pPlayer,
					0,
					pPlayer->m_fPosX,
					pPlayer->m_fPosY,
					pPlayer->m_fPosZ,
					wItemID,
					QTT_GETITEM,
					TT_GETITEM,
					bCount);

				pMon->EraseItem(bEraseItemID);
				pPlayer->SendCS_GETITEM_ACK(&logItem);

				if( pMon->m_bKeeperType == OWNER_PARTY )
					PartyMonItemTake(pPlayer, &logItem);
			}
			else
			{
				pPlayer->SendCS_MONITEMTAKE_ACK(MIT_FULLINVEN);
				pPlayer->SendCS_MONITEMLIST_ACK(
					MIL_SUCCESS,
					dwMonID,
					pMon->m_dwMoney,
					pInven,
					TRUE);

				return FALSE;
			}
		}
		else
		{
			SendMW_ADDITEM_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				GetSvrID(),
				pPlayer->m_bChannel,
				pPlayer->m_pMAP->m_wMapID,
				pMon->m_dwID,
				bInvenID,
				bSlotID,
				pItem);

			pMon->EraseItem(bItemID);
			pMon->AddItemList(pItem);
		}

		if(bSlotID != INVALID_SLOT) break;
	}

	pPlayer->SendCS_MONITEMLIST_ACK(
		MIL_SUCCESS,
		dwMonID,
		pMon->m_dwMoney,
		pInven,
		TRUE);

	pPlayer->SendCS_MONITEMTAKE_ACK(MIT_SUCCESS);

	return TRUE;
}

void CTMapSvrModule::WrapItem(CPacket * pPacket,
							  TITEMQUERY & query)
{
	(*pPacket)
		<< query.m_dlID
		<< query.m_bItemID
		<< query.m_wItemID
		<< query.m_bLevel
		<< query.m_bCount
		<< query.m_bGLevel
		<< query.m_dwDuraMax
		<< query.m_dwDuraCur
		<< query.m_bRefineCur
		<< __DBTOTIME(query.m_dEndTime)
		<< query.m_bGradeEffect
		<< query.m_dwExtValue[IEV_ELD]
		<< query.m_dwExtValue[IEV_WRAP]
		<< query.m_dwExtValue[IEV_COLOR]
		<< query.m_dwExtValue[IEV_GUILD]
		<< BYTE(TMAGIC_MAX);

	for(BYTE i=0; i<TMAGIC_MAX; i++)
	{
		(*pPacket)
			<< query.m_bMagic[i]
			<< query.m_wValue[i];
	}
}
void CTMapSvrModule::WrapItemQuery(CPacket * pPacket,
								   TITEMQUERY & item)
{
	BYTE bMagicCount;
	item.Reset();
	__time64_t dEndTime;

	(*pPacket)
		>> item.m_dlID
		>> item.m_bItemID
		>> item.m_wItemID
		>> item.m_bLevel
		>> item.m_bCount
		>> item.m_bGLevel
		>> item.m_dwDuraMax
		>> item.m_dwDuraCur
		>> item.m_bRefineCur
		>> dEndTime
		>> item.m_bGradeEffect
		>> item.m_dwExtValue[IEV_ELD]
		>> item.m_dwExtValue[IEV_WRAP]
		>> item.m_dwExtValue[IEV_COLOR]
		>> item.m_dwExtValue[IEV_GUILD]
		>> bMagicCount;

	__TIMETODB(dEndTime, item.m_dEndTime);

	for(BYTE i=0; i<bMagicCount; i++)
	{
		(*pPacket)
			>> item.m_bMagic[i]
			>> item.m_wValue[i];
	}
}

CTItem * CTMapSvrModule::CreateItem(CPacket * pPacket)
{
	CTItem *pTITEM = new CTItem();
	BYTE bMagicCount;

	(*pPacket)
		>> pTITEM->m_dlID
        >> pTITEM->m_bItemID
        >> pTITEM->m_wItemID
        >> pTITEM->m_bLevel
        >> pTITEM->m_bCount
        >> pTITEM->m_bGLevel
		>> pTITEM->m_dwDuraMax
		>> pTITEM->m_dwDuraCur
		>> pTITEM->m_bRefineCur
		>> pTITEM->m_dEndTime
		>> pTITEM->m_bGradeEffect
		>> pTITEM->m_dwExtValue[IEV_ELD]
		>> pTITEM->m_dwExtValue[IEV_WRAP]
		>> pTITEM->m_dwExtValue[IEV_COLOR]
		>> pTITEM->m_dwExtValue[IEV_GUILD]
		>> bMagicCount;

	if(!pTITEM->m_dlID)
		pTITEM->m_dlID = GenItemID();

	pTITEM->m_pTITEM = FindTItem(pTITEM->m_wItemID);
	if(pTITEM->m_pTITEM)
	{
		for( BYTE k=0; k<bMagicCount; k++)
		{
			WORD wValue;
			BYTE bMagic;

			(*pPacket)
                >> bMagic
                >> wValue;

			if(bMagic && wValue)
			{
				LPTITEMMAGIC pMagic = GetItemMagic(bMagic);
				if(pMagic)
				{
					MAPTMAGIC::iterator itTMAGIC = pTITEM->m_mapTMAGIC.find(bMagic);
					LPTMAGIC pTMAGIC = new TMAGIC();

					pTMAGIC->m_wValue = wValue;
					pTMAGIC->m_pMagic = pMagic;

					if( itTMAGIC != pTITEM->m_mapTMAGIC.end() )
					{
						delete (*itTMAGIC).second;
						pTITEM->m_mapTMAGIC.erase(itTMAGIC);
					}

					pTITEM->m_mapTMAGIC.insert( MAPTMAGIC::value_type( bMagic, pTMAGIC));
				}
			}
		}

		SetItemAttr( pTITEM, pTITEM->m_bLevel);
	}
	else
	{
		for( BYTE k=0; k<bMagicCount; k++)
		{
			WORD wValue;
			BYTE bMagic;

			(*pPacket)
				>> bMagic
				>> wValue;
		}
		delete pTITEM;
		pTITEM = NULL;
	}

	return pTITEM;
}

CTItem * CTMapSvrModule::CreateItem(TITEMQUERY &item)
{
	CTItem *pTITEM = new CTItem();
	pTITEM->m_pTITEM = FindTItem(item.m_wItemID);
	if(pTITEM->m_pTITEM)
	{
		pTITEM->m_dlID = 0;
		pTITEM->m_wItemID = item.m_wItemID;
		pTITEM->m_bLevel = item.m_bLevel;
		pTITEM->m_bCount = item.m_bCount;
		pTITEM->m_bGLevel = item.m_bGLevel;
		pTITEM->m_dwDuraMax = item.m_dwDuraMax;
		pTITEM->m_dwDuraCur = item.m_dwDuraCur;
		pTITEM->m_bRefineCur = item.m_bRefineCur;
		pTITEM->m_bGradeEffect = item.m_bGradeEffect;
		pTITEM->m_dwExtValue[IEV_ELD] = item.m_dwExtValue[IEV_ELD];
		pTITEM->m_dwExtValue[IEV_WRAP] = item.m_dwExtValue[IEV_WRAP];
		pTITEM->m_dwExtValue[IEV_COLOR] = item.m_dwExtValue[IEV_COLOR];
		pTITEM->m_dwExtValue[IEV_GUILD] = item.m_dwExtValue[IEV_GUILD];

		pTITEM->m_dEndTime = __DBTOTIME(item.m_dEndTime);

		for( BYTE k=0; k<TMAGIC_MAX; k++)
		{
			if(item.m_bMagic[k] && item.m_wValue[k])
			{
				LPTITEMMAGIC pMagic = GetItemMagic(item.m_bMagic[k]);
				if(pMagic)
				{
					MAPTMAGIC::iterator itTMAGIC = pTITEM->m_mapTMAGIC.find(item.m_bMagic[k]);
					LPTMAGIC pTMAGIC = new TMAGIC();

					pTMAGIC->m_wValue = item.m_wValue[k];
					pTMAGIC->m_pMagic = pMagic;

					if( itTMAGIC != pTITEM->m_mapTMAGIC.end() )
					{
						delete (*itTMAGIC).second;
						pTITEM->m_mapTMAGIC.erase(itTMAGIC);
					}

					if(pTMAGIC->m_wValue)
						pTITEM->m_mapTMAGIC.insert( MAPTMAGIC::value_type( item.m_bMagic[k], pTMAGIC));
					else
						delete pTMAGIC;
				}
			}
		}
	}
	else
	{
		delete pTITEM;
		pTITEM = NULL;
	}

	return pTITEM;
}

DWORD CTMapSvrModule::GetFormulaInit(BYTE bFormula)
{
	MAPTFORMULA::iterator find = m_mapTFORMULA.find(bFormula);
	if(find!=m_mapTFORMULA.end())
		return (*find).second->m_dwInit;

	return 0;
}

FLOAT CTMapSvrModule::GetFormulaRateX(BYTE bFormula)
{
	MAPTFORMULA::iterator find = m_mapTFORMULA.find(bFormula);
	if(find!=m_mapTFORMULA.end())
		return (*find).second->m_fRateX;

	return 0.0;
}

FLOAT CTMapSvrModule::GetFormulaRateY(BYTE bFormula)
{
	MAPTFORMULA::iterator find = m_mapTFORMULA.find(bFormula);
	if(find!=m_mapTFORMULA.end())
		return (*find).second->m_fRateY;

	return 0.0;
}

DWORD CTMapSvrModule::GetItemPrice(LPTITEM pItem)
{
	BYTE bGrade = 0;
	if(pItem->m_wAttrID)
	{
		MAPTITEMATTR::iterator itAttr = _AtlModule.m_mapTItemAttr.find(pItem->m_wAttrID);
        if(itAttr != _AtlModule.m_mapTItemAttr.end())
			bGrade = (*itAttr).second->m_bGrade;
	}
	else
		bGrade = pItem->m_bDefaultLevel;

	LPTLEVEL pLevel = FindTLevel(bGrade);
	if(pLevel)
		return DWORD(pLevel->m_dwMoney * pItem->m_fPrice + 0.99);
	else
		return 0;
}

DWORD CTMapSvrModule::GetItemPvPrice(LPTITEM pItem)
{
	BYTE bGrade = 0;
	if(pItem->m_wAttrID)
	{
		MAPTITEMATTR::iterator itAttr = _AtlModule.m_mapTItemAttr.find(pItem->m_wAttrID);
        if(itAttr != _AtlModule.m_mapTItemAttr.end())
			bGrade = (*itAttr).second->m_bGrade;
	}
	else
		bGrade = pItem->m_bDefaultLevel;

	LPTLEVEL pLevel = FindTLevel(bGrade);
	if(pLevel)
		return DWORD(pLevel->m_dwPvPMoney * pItem->m_fPvPrice + 0.99);
	else
		return 0;
}

__int64 CTMapSvrModule::GenItemID()
{
	m_dlGenItemID++;
	return m_dlGenItemID;
}

BYTE CTMapSvrModule::UseItem(CTPlayer * pPlayer,
							 CTInven * pInven,
							 CTItem * pItem,
							 BYTE bCount)
{
	if(pItem->m_bCount < bCount)
		return FALSE;

	pItem->m_bCount -= bCount;
	if(!pItem->m_bCount)
	{
#ifdef	DEF_UDPLOG
		if(!(pItem->m_pTITEM->m_bKind >= IK_ARROW && pItem->m_pTITEM->m_bKind <= IK_MP))
			m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMUSE, pPlayer,pItem);
#endif	//	DEF_UDPLOG

		pPlayer->SendCS_DELITEM_ACK(
			pInven->m_bInvenID,
			pItem->m_bItemID);

		pInven->m_mapTITEM.erase(pItem->m_bItemID);
		delete pItem;
	}
	else
	{
#ifdef	DEF_UDPLOG
		if(!(pItem->m_pTITEM->m_bKind >= IK_ARROW && pItem->m_pTITEM->m_bKind <= IK_MP))
		{
			BYTE nTempCount = pItem->m_bCount;

			pItem->m_bCount = 1;
			m_pUdpSocket->LogItemUpgrade(LOGMAP_ITEMUSE, pPlayer, pItem);

			pItem->m_bCount	= nTempCount;
		}

#endif	//	DEF_UDPLOG
		pPlayer->SendCS_UPDATEITEM_ACK(
			pItem,
			pInven->m_bInvenID);
	}

	return TRUE;
}

BYTE CTMapSvrModule::GetDiscountRate(CTPlayer* pPlayer, CTNpc* pNpc)  
{
	if(!pPlayer || !pNpc)
		return 0;
	
	BYTE bDiscountRate = 0;	

	switch(pNpc->m_bDiscountCondition )
	{
	case DCC_GUILD:
		{
			if(pNpc->m_pLocal && pNpc->m_pLocal->m_dwGuild)
				if(pNpc->m_pLocal->m_dwGuild == pPlayer->m_dwGuildID)
					bDiscountRate =  pNpc->m_bDiscountRate;
		}
		break;

	case DCC_LOCALHERO:
		{
			MAPTLOCAL::iterator itL;
			for(itL = m_mapTLOCAL.begin(); itL != m_mapTLOCAL.end(); itL++)	
				if( (*itL).second->m_bType == LOCAL_OCCUPATION &&
					lstrcmp( (*itL).second->m_strHero , pPlayer->m_strNAME ) == 0 && (*itL).second->m_timeHero < (*itL).second->m_timeNextDefend )		
				{
					bDiscountRate = pNpc->m_bDiscountRate;
					break;
				}
		}
		break;

	case DCC_CASTLEHERO:
		{
			MAPTLOCAL::iterator itC;
			for(itC = m_mapTLOCAL.begin(); itC != m_mapTLOCAL.end(); itC++)
				if( (*itC).second->m_bType == LOCAL_CASTLE &&
					lstrcmp( (*itC).second->m_strHero, pPlayer->m_strNAME) == 0 && (*itC).second->m_timeHero < (*itC).second->m_timeNextDefend )		
				{
					bDiscountRate = pNpc->m_bDiscountRate;
					break;
				}
		}
		break;

	default:
		break;
	}

	return bDiscountRate;
}

BYTE CTMapSvrModule::CalcProb(CTPlayer* pPlayer, CTNpc* pNpc , BYTE bType, BYTE bBaseProb)  
{
	if( !pPlayer || !pNpc )
		return 0;

	WORD wAddProb = bBaseProb;

	switch(pNpc->m_bDiscountCondition)
	{
	case DCC_GUILD:
		{
			if( pNpc->m_pLocal && pNpc->m_pLocal->m_dwGuild )
				if(pNpc->m_pLocal->m_dwGuild == pPlayer->m_dwGuildID)
					wAddProb += pNpc->m_bAddProb;
		}
		break;

	case DCC_LOCALHERO:
		{
			MAPTLOCAL::iterator itL;
			for(itL = m_mapTLOCAL.begin(); itL != m_mapTLOCAL.end(); itL++)	
				if( (*itL).second->m_bType == LOCAL_OCCUPATION &&
					lstrcmp( (*itL).second->m_strHero , pPlayer->m_strNAME ) == 0 )		
				{
					wAddProb += pNpc->m_bAddProb;
					break;
				}
		}
		break;

	case DCC_CASTLEHERO:
		{
			MAPTLOCAL::iterator itC;
			for(itC = m_mapTLOCAL.begin(); itC != m_mapTLOCAL.end(); itC++)	
				if( (*itC).second->m_bType == LOCAL_CASTLE &&
					lstrcmp( (*itC).second->m_strHero, pPlayer->m_strNAME) == 0 )		
				{
					wAddProb += pNpc->m_bAddProb;
					break;
				}
		}
		break;

	default:
		break;
	}

	switch(bType)
	{
	case PROB_MAGIC:
		{
			wAddProb += bBaseProb * m_wEventValue[EVENT_MAGICUPGRADE] / 100;  
			WORD wAdd = pPlayer->HaveItemProbBuff(SDT_STATUS_MAGICPROB);
			if(wAdd)
			{
				wAddProb += bBaseProb * wAdd / 100;
				pPlayer->EraseItemProbBuff(SDT_STATUS_MAGICPROB); 
			}
		}
		break;
	case PROB_REFINE:
		{
			wAddProb += bBaseProb * m_wEventValue[EVENT_REFINE] / 100;
			WORD wAdd = pPlayer->HaveItemProbBuff(SDT_STATUS_REFINEPROB);
			if(wAdd)
			{
				wAddProb = 100;
				pPlayer->EraseItemProbBuff(SDT_STATUS_REFINEPROB); 
			}
		}
		break;
	case PROB_TRANS:
		{
			wAddProb += bBaseProb * m_wEventValue[EVENT_TRANS] / 100;
			WORD wAdd = pPlayer->HaveItemProbBuff(SDT_STATUS_TRANSPROB);
			if(wAdd)
			{
				wAddProb += bBaseProb * wAdd / 100;
				pPlayer->EraseItemProbBuff(SDT_STATUS_TRANSPROB); 
			}			
		}
		break;
	case PROB_GAMBLE:
		wAddProb += bBaseProb * m_wEventValue[EVENT_GAMBLEOPTION] / 100;
		break;
	case PROB_UPGRADE:
		{
			wAddProb += bBaseProb * m_wEventValue[EVENT_ITEMUPGRADE] / 100;
			WORD wAdd = pPlayer->HaveItemProbBuff(SDT_STATUS_ITEMUPGRADE);
			if(wAdd)
			{
				wAddProb += bBaseProb * wAdd / 100;
				pPlayer->EraseItemProbBuff(SDT_STATUS_ITEMUPGRADE); 
			}

			if(pPlayer->m_bInPcBang & PCBANG_REAL)
				wAddProb += bBaseProb;
		}
		break;
	case PROB_ITEMGUARD:
		{
			wAddProb = pPlayer->HaveItemProbBuff(SDT_STATUS_ITEMGUARD);
			pPlayer->EraseItemProbBuff(SDT_STATUS_ITEMGUARD); 
		}
		break;
	case PROB_RAREMAGIC:  
		{
			wAddProb += bBaseProb * m_wEventValue[EVENT_RAREMAGICUPGRADE] / 100;
			WORD wAdd = pPlayer->HaveItemProbBuff(SDT_STATUS_MAGICPROB);
			if(wAdd)
			{
				wAddProb += bBaseProb * wAdd / 100;
				pPlayer->EraseItemProbBuff(SDT_STATUS_MAGICPROB); 
			}
		}
		break;
	default:
		break;
	}

	return min(wAddProb, 100);
}

void CTMapSvrModule::LocalReward()
{
	MAPPLAYER mapPlayer[LOCALLINE_COUNT];
	for(BYTE i=0; i<LOCALLINE_COUNT; i++)
		mapPlayer[i].clear();

	for(DWORD ll=0; ll<m_vOccupation.size(); ll++)
	{
		LPTLOCAL pLocal = m_vOccupation[ll];
		CTMap *pTMAP = FindTMap(DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);
		if(pTMAP)
		{
			MAPTMONSPAWNTEMP::iterator itSP = m_mapTMONSPAWN.find(pLocal->m_pZone->m_wBossSpawnID);
			if(itSP == m_mapTMONSPAWN.end())
				continue;

			pTMAP->GetUnitPlayer(&mapPlayer[pLocal->m_pZone->m_bLine], (*itSP).second->m_fPosX, (*itSP).second->m_fPosZ);
		}
	}

	MAPPLAYER::iterator itChar;
	for(size_t ll=0; ll<m_vOccupation.size(); ll++)
	{
		LPTLOCAL pLocal = m_vOccupation[ll];
		if(!pLocal->m_dwGuild || !pLocal->m_pZone->m_pNormalItem)
			continue;

		CTItem * pReward = new CTItem();
		pReward->m_pTITEM = pLocal->m_pZone->m_pNormalItem;
		pReward->SetDuration(FALSE);
		SetItemAttr(pReward, 0);
		pReward->m_wItemID = pReward->m_pTITEM->m_wItemID;
		pReward->m_bCount = 1;

		for(itChar=mapPlayer[pLocal->m_pZone->m_bLine].begin(); itChar!=mapPlayer[pLocal->m_pZone->m_bLine].end(); itChar++)
		{
			CTPlayer * pChar = (*itChar).second;
			if(!pChar->m_bMain ||
				!pChar->m_pMAP ||
				pChar->GetGuild() != pLocal->m_dwGuild)
				continue;

			if(pChar->m_bGuildDuty == GUILD_DUTY_CHIEF &&
				pLocal->m_pZone->m_pChiefItem)
			{
					CTItem * pChiefReward = new CTItem();
					pChiefReward->m_dlID = GenItemID();
					pChiefReward->m_pTITEM = pLocal->m_pZone->m_pChiefItem;
					pChiefReward->SetDuration(FALSE);
					SetItemAttr(pChiefReward, 0);
					pChiefReward->m_wItemID = pChiefReward->m_pTITEM->m_wItemID;
					pChiefReward->m_bCount = 1;

					SendDM_POSTRECV_REQ(
						0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
						BuildNetString(NAME_NULL, GetSvrMsg(MSG_LOCAL_REWARD)), NAME_NULL, POST_PACKATE,
						0,0,0, pChiefReward);

					delete pChiefReward;
			}
			else
			{
				pReward->m_dlID = GenItemID();
				SendDM_POSTRECV_REQ(
					0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
					BuildNetString(NAME_NULL, GetSvrMsg(MSG_LOCAL_REWARD)), NAME_NULL, POST_PACKATE,
					0,0,0, pReward);
			}
		}

		delete pReward;
	}
}

void CTMapSvrModule::CastleReward(LPTLOCAL pCastle, DWORD dwGuild)
{
	if(!dwGuild)
		return;

	CTMap * pTMAP = FindTMap(DEFAULT_CHANNEL, 0, pCastle->m_pZone->m_wMapID);
	if(pTMAP)
	{
		CTItem * pReward = NULL;
		if(pCastle->m_pZone->m_pNormalItem)
		{
			pReward = new CTItem();
			pReward->m_pTITEM = pCastle->m_pZone->m_pNormalItem;
			pReward->SetDuration(FALSE);
			SetItemAttr(pReward, 0);
			pReward->m_wItemID = pReward->m_pTITEM->m_wItemID;
			pReward->m_bCount = 1;
		}

		MAPTCELL::iterator finder;
		for(finder = pTMAP->m_mapTCELL.begin(); finder != pTMAP->m_mapTCELL.end(); finder++)
		{
			CTCell *pCell = (*finder).second;
			MAPPLAYER::iterator it;

			for( it = pCell->m_mapPLAYER.begin(); it != pCell->m_mapPLAYER.end(); it++)
			{
				CTPlayer * pChar = (*it).second;
				if(!pChar->m_bMain ||
					!pChar->m_pMAP ||
					pChar->GetGuild() != dwGuild)
					continue;

				if(pChar->m_bGuildDuty == GUILD_DUTY_CHIEF &&
					pChar->m_dwGuildID == dwGuild &&
					pCastle->m_pZone->m_pChiefItem)
				{
						CTItem * pChiefReward = new CTItem();
						pChiefReward->m_dlID = GenItemID();
						pChiefReward->m_pTITEM = pCastle->m_pZone->m_pChiefItem;
						pChiefReward->SetDuration(FALSE);
						SetItemAttr(pChiefReward, 0);
						pChiefReward->m_wItemID = pChiefReward->m_pTITEM->m_wItemID;
						pChiefReward->m_bCount = 1;

						SendDM_POSTRECV_REQ(
							0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
							BuildNetString(NAME_NULL, GetSvrMsg(MSG_CASTLE_REWARD)), NAME_NULL, POST_PACKATE,
							0,0,0, pChiefReward);

						delete pChiefReward;
				}
				else if(pReward && pChar->m_dwGuildID == dwGuild)
				{
					pReward->m_dlID = GenItemID();
					SendDM_POSTRECV_REQ(
						0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
						BuildNetString(NAME_NULL, GetSvrMsg(MSG_CASTLE_REWARD)), NAME_NULL, POST_PACKATE,
						0,0,0, pReward);
				}
				else
				{
					WORD wNewItemID = 18044;
					BYTE bNewCount = 5;
					DWORD dwSilver = 500;

					LPTITEM pTITEM = FindTItem(wNewItemID);
					if(pTITEM)
					{
						CTItem * pNew = new CTItem();
						pNew->m_dlID = GenItemID();
						pNew->m_bCount = bNewCount;
						pNew->m_wItemID = wNewItemID;
						pNew->m_pTITEM = pTITEM;
						pNew->SetDuration(FALSE);
						SetItemAttr(pNew, 0);

						SendDM_POSTRECV_REQ(
							0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
							BuildNetString(NAME_NULL, GetSvrMsg(MSG_CASTLE_REWARD)), NAME_NULL, POST_PACKATE,
							0,dwSilver,0, pNew);

						delete pNew;
					}
				}
			}
		}

		if(pReward)
			delete pReward;
	}
}

void CTMapSvrModule::MissionReward(LPTLOCAL pLocal, BYTE bType, BYTE bCountry)
{
	CTMap *pTMAP = FindTMap(DEFAULT_CHANNEL, 0, pLocal->m_pZone->m_wMapID);

	if(!pTMAP)
		return;

	VPLAYER vPlayers;
	vPlayers.clear();

	pTMAP->GetMapPlayers(&vPlayers);

	if(pLocal->m_bCountry != bCountry)
	{
		for(DWORD i=0; i<vPlayers.size(); i++)
		{
			CTPlayer * pChar = vPlayers[i];
			if(pChar->GetWarCountry() != bCountry)
			{
				pChar->EraseMaintainSkill(pLocal->m_pZone->m_wSkill1);
				pChar->EraseMaintainSkill(pLocal->m_pZone->m_wSkill2);
			}
			else
			{
				pChar->ForceMaintain(pLocal->m_pZone->m_wSkill1, pChar->m_dwID, OT_PC, pChar->m_dwID, OT_PC, 0);
				pChar->ForceMaintain(pLocal->m_pZone->m_wSkill2, pChar->m_dwID, OT_PC, pChar->m_dwID, OT_PC, 0);
			}
		}
	}

	if(bType == OCCUPY_ACCEPT)
	{
		DWORD dwPvPoint = 0;
		MAPTPVPOINT::iterator itF = pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, PVPE_WIN));
		if(itF != pLocal->m_pZone->m_mapTPvPoint.end())
			dwPvPoint = (*itF).second->m_dwIncPoint;

		CTItem * pReward = NULL;
		if(pLocal->m_pZone->m_pNormalItem)
		{
			pReward = new CTItem();
			pReward->m_pTITEM = pLocal->m_pZone->m_pNormalItem;
			pReward->SetDuration(FALSE);
			SetItemAttr(pReward, 0);
			pReward->m_wItemID = pReward->m_pTITEM->m_wItemID;
			pReward->m_bCount = 1;
		}

		for(DWORD i=0; i<vPlayers.size(); i++)
		{
			CTPlayer * pChar = vPlayers[i];
			if(pChar->GetWarCountry() == bCountry)
			{
				if(pReward)
				{
					pReward->m_dlID = GenItemID();
					SendDM_POSTRECV_REQ(
						0, pChar->m_dwID, GetSvrMsg(NAME_OPERATOR), pChar->m_strNAME,
						BuildNetString(NAME_NULL, GetSvrMsg(MSG_MISSION_REWARD)), NAME_NULL, POST_PACKATE,
						0,0,0, pReward);
				}

				if(dwPvPoint)
					pChar->GainPvPoint(dwPvPoint, PVPE_WIN, PVP_USEABLE);

				CheckQuest(
					pChar,
					0,
					pChar->m_fPosX,
					pChar->m_fPosY,
					pChar->m_fPosZ,
					pChar->m_dwRegion,
					0,
					TT_WARREGION,
					1);
			}
		}

		if(pReward)
			delete pReward;
	}

	vPlayers.clear();
}

CString CTMapSvrModule::GetSvrMsg(DWORD dwID)
{
	MAPTSTRING::iterator it = m_mapTSvrMsg.find(dwID);
	if(it!=m_mapTSvrMsg.end())
		return (*it).second;
	else
		return NAME_NULL;
}

CString CTMapSvrModule::BuildNetString( const CString& strHeader, const CString& strBody)
{
	CString strSIZE;

	strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), strBody.GetLength());
	return strSIZE + strHeader + strBody;
}

BYTE CTMapSvrModule::GetNation()
{
	return m_bNation;
}

LPTCASHCATEGORY CTMapSvrModule::FindCashCategory(BYTE bCategory)
{
	for(BYTE i=0; i<m_vCashCategory.size(); i++)
	{
		if(m_vCashCategory[i]->m_bID == bCategory)
			return m_vCashCategory[i];
	}

	return NULL;
}

void CTMapSvrModule::PvPEvent(BYTE bEvent, void * pObject, DWORD dwTargetID)
{
	switch(bEvent)
	{
	case PVPE_KILL_H:
	case PVPE_KILL_E:
	case PVPE_KILL_L:
		{
			if(!pObject) return;

			CTPlayer * pChar = (CTPlayer *)pObject;
			MAPPLAYER::iterator itP = m_mapPLAYER.find(dwTargetID);
			if(itP == m_mapPLAYER.end())
				return;

			if(IsTournamentMap(pChar->m_wMapID))
				return;

			if(pChar->m_wArenaID)
				return;

			CTPlayer * pAttack = (*itP).second;
			if(!pAttack->m_pMAP)
				return;

			TRECORDSET setChar, setAtk;
			setChar.m_strName = pChar->m_strNAME;
			setChar.m_bClass = pChar->m_bClass;
			setChar.m_bLevel = pChar->m_bLevel;
			setAtk.m_strName = pAttack->m_strNAME;
			setAtk.m_bClass = pAttack->m_bClass;
			setAtk.m_bLevel = pAttack->m_bLevel;

			BYTE bAtkMaxLevel = pAttack->m_bLevel;
			BYTE bDefMaxLevel = pChar->m_bLevel;

			WORD wTotalLevel = 0;
			VPLAYER vParty;
			vParty.clear();

			if(pAttack->GetPartyID())
			{
				VPLAYER vPlayer;
				vPlayer.clear();

				pAttack->m_pMAP->GetNeighbor(
					&vPlayer,
					pAttack->m_fPosX,
					pAttack->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					if(vPlayer[i]->GetPartyID() == pAttack->GetPartyID())
					{
						wTotalLevel += vPlayer[i]->m_bLevel;
						vParty.push_back(vPlayer[i]);

						if(bAtkMaxLevel < vPlayer[i]->m_bLevel)
							bAtkMaxLevel = vPlayer[i]->m_bLevel;
					}
				}
			}

			if(pChar->GetPartyID())
			{
				VPLAYER vPlayer;
				vPlayer.clear();

				pChar->m_pMAP->GetNeighbor(
					&vPlayer,
					pChar->m_fPosX,
					pChar->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					if(vPlayer[i]->GetPartyID() == pChar->GetPartyID())
					{
						if(bDefMaxLevel < vPlayer[i]->m_bLevel)
							bDefMaxLevel = vPlayer[i]->m_bLevel;
					}
				}
			}

			BYTE bGap = 5;
			bEvent = bAtkMaxLevel > bDefMaxLevel + bGap ? PVPE_KILL_L : (bAtkMaxLevel + bGap < bDefMaxLevel ? PVPE_KILL_H : PVPE_KILL_E);
			MAPTPVPOINT::iterator itF = m_mapTPvPointKill.find(MAKEWORD(pChar->GetPvPStatus(), bEvent));
			if(itF == m_mapTPvPointKill.end())
				return;

			LPTPVPOINT pPvP = (*itF).second;
			DWORD dwDecPoint = pChar->m_pTLEVEL->m_wPvPoint * pPvP->m_dwDecPoint / 100;
			DWORD dwIncPoint = pChar->m_pTLEVEL->m_wPvPoint * pPvP->m_dwIncPoint / 100;

			LPTLOCAL pLocal = pChar->m_pLocal && pChar->m_pLocal->m_bStatus == BS_BATTLE ? pChar->m_pLocal : NULL;
			if(pLocal)
				LocalRecord(pLocal, pChar, bEvent, dwDecPoint, FALSE);
			else
				dwIncPoint = dwIncPoint * (100 - pChar->m_aftermath.m_bStep / 2) / 100;

			pChar->UsePvPoint(dwDecPoint, bEvent, PVP_TOTAL, &setAtk);

			if(vParty.empty())
			{
				if(pLocal)
					LocalRecord(pLocal, pAttack, bEvent, dwIncPoint, TRUE);

				if(pAttack->m_bMain)
					pAttack->GainPvPoint(dwIncPoint, bEvent, PVP_TOTAL | PVP_USEABLE, &setChar);
				else
					SendMW_GAINPVPPOINT_ACK(
						TOWNER_CHAR,
						dwTargetID,
						dwIncPoint,
						bEvent,
						PVP_TOTAL | PVP_USEABLE,
						TRUE,
						setChar.m_strName,
						setChar.m_bClass,
						setChar.m_bLevel);
			}
			else
			{
				if(wTotalLevel == 0)
					wTotalLevel = 100;

				while(!vParty.empty())
				{
					CTPlayer * pParty = vParty.back();
					vParty.pop_back();

					DWORD dwPoint = dwIncPoint * pParty->m_bLevel / wTotalLevel;

					if(pLocal)
						LocalRecord(pLocal, pParty, bEvent, dwPoint, TRUE);

					if(pParty->m_bMain)
						pParty->GainPvPoint(dwPoint, bEvent, PVP_TOTAL | PVP_USEABLE, &setChar);
					else
						SendMW_GAINPVPPOINT_ACK(
							TOWNER_CHAR,
							pParty->m_dwID,
							dwPoint,
							bEvent,
							PVP_TOTAL | PVP_USEABLE,
							TRUE,
							setChar.m_strName,
                            setChar.m_bClass,
                            setChar.m_bLevel);
				}
			}
		}
		break;
	case PVPE_GODMONKILL:
		{
			MAPPLAYER::iterator itP = m_mapPLAYER.find(dwTargetID);
			if(itP == m_mapPLAYER.end())
				return;

			CTPlayer * pAttack = (*itP).second;
			if(!pAttack->m_pMAP || !pAttack->m_pLocal)
				return;

			MAPTPVPOINT::iterator itF = pAttack->m_pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, PVPE_GODMONKILL));
			if(itF == pAttack->m_pLocal->m_pZone->m_mapTPvPoint.end())
				return;

			LPTPVPOINT pPvP = (*itF).second;

			if(pPvP->m_dwIncPoint == 0)
				return;

			VPLAYER vParty;
			vParty.clear();

			if(pAttack->GetPartyID())
			{
				VPLAYER vPlayer;
				vPlayer.clear();

				pAttack->m_pMAP->GetNeighbor(
					&vPlayer,
					pAttack->m_fPosX,
					pAttack->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
				{
					if(vPlayer[i]->GetPartyID() == pAttack->GetPartyID())
						vParty.push_back(vPlayer[i]);
				}
			}

			if(vParty.empty())
			{
				LocalRecord(pAttack->m_pLocal, pAttack, bEvent, pPvP->m_dwIncPoint, TRUE);

				if(pAttack->m_bMain)
					pAttack->GainPvPoint(pPvP->m_dwIncPoint, bEvent, PVP_USEABLE);
				else
					SendMW_GAINPVPPOINT_ACK(TOWNER_CHAR, dwTargetID, pPvP->m_dwIncPoint, bEvent, PVP_USEABLE, TRUE);
			}
			else
			{
				DWORD dwPoint  = pPvP->m_dwIncPoint / vParty.size();
				while(!vParty.empty())
				{
					CTPlayer * pParty = vParty.back();
					vParty.pop_back();
					
					LocalRecord(pAttack->m_pLocal, pParty, bEvent, pPvP->m_dwIncPoint, TRUE);

					if(pParty->m_bMain)
						pParty->GainPvPoint(dwPoint, bEvent, PVP_USEABLE);
					else
						SendMW_GAINPVPPOINT_ACK(TOWNER_CHAR, pParty->m_dwID, dwPoint, bEvent, PVP_USEABLE, TRUE);
				}
			}
		}
		break;
	case PVPE_ENTRY:
		{
			if(!pObject)
				return;

			LPTLOCAL pLocal = (LPTLOCAL)pObject;

			MAPTPVPOINT::iterator itF = pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, PVPE_ENTRY));
			if(itF == pLocal->m_pZone->m_mapTPvPoint.end() ||
				(*itF).second->m_dwIncPoint == 0 ||
				pLocal->m_mapTRecord.empty())
				return;

			DWORD dwGuildPoint = 0;
			BYTE bGEv = PVPE_COUNT;
			if(pLocal->m_bLastOccType == OCCUPY_DEFEND)
				bGEv = PVPE_DEFEND;
			else
				bGEv = PVPE_WIN;

			MAPTPVPOINT::iterator itGP = pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, bGEv));
			if(itGP != pLocal->m_pZone->m_mapTPvPoint.end())
				dwGuildPoint = (*itGP).second->m_dwIncPoint;

			LPTPVPOINT pPvP = (*itF).second;

			CPacket * pMsg = new CPacket();
			(*pMsg)
				<< pLocal->m_dwGuild
				<< dwGuildPoint
				<< (WORD)pLocal->m_mapTRecord.size();

			MAPTLOCALRECORD::iterator itL;
			for(itL=pLocal->m_mapTRecord.begin(); itL!=pLocal->m_mapTRecord.end(); itL++)
			{
				(*pMsg)
					<< (*itL).first
					<< (WORD)(*itL).second.size();

				MAPTENTRYRECORD::iterator itE;
				for(itE = (*itL).second.begin(); itE!=(*itL).second.end(); itE++)
				{
					LPTENTRYRECORD pEntry = &((*itE).second);
					pEntry->m_aGainPoint[bEvent] = pPvP->m_dwIncPoint;
					(*pMsg)
						<< pEntry->m_dwCharID
						<< pEntry->m_wKillCount
						<< pEntry->m_wDieCount;

					for(BYTE i=0; i<PVPE_COUNT; i++)
						(*pMsg)
							<< pEntry->m_aGainPoint[i];

					MAPPLAYER::iterator itP = m_mapPLAYER.find(pEntry->m_dwCharID);
					if(itP != m_mapPLAYER.end() && (*itP).second->m_bMain)
						(*itP).second->GainPvPoint(pPvP->m_dwIncPoint, bEvent, PVP_USEABLE);
					else
						SendMW_GAINPVPPOINT_ACK(TOWNER_CHAR, pEntry->m_dwCharID, pPvP->m_dwIncPoint, bEvent, PVP_USEABLE, TRUE);
				}
			}

			SendMW_LOCALRECORD_ACK(pMsg);
			pLocal->m_mapTRecord.clear();
		}
		break;
	case PVPE_WIN:
		{
			if(!pObject)
				return;

			LPTLOCAL pLocal = (LPTLOCAL)pObject;

			MAPTPVPOINT::iterator itF = pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, PVPE_WIN));
			if(itF == pLocal->m_pZone->m_mapTPvPoint.end())
				return;

			LPTPVPOINT pPvP = (*itF).second;

			if(pPvP->m_dwDecPoint && dwTargetID)
			{
				SendMW_GAINPVPPOINT_ACK(TOWNER_GUILD, dwTargetID, pPvP->m_dwDecPoint, bEvent, PVP_TOTAL, FALSE);
#ifdef DEF_UDPLOG				
				_AtlModule.m_pUdpSocket->LogPvPointGuild(LOGMAP_PVPOINTUSEGUILD,pPvP->m_dwDecPoint,pLocal ? pLocal->m_wLocal : 0, pLocal ? pLocal->m_bCountry : TCONTRY_N,dwTargetID,bEvent,PVP_TOTAL);
#endif
			}

			if(pPvP->m_dwIncPoint && pLocal->m_dwGuild)
			{
				SendMW_GAINPVPPOINT_ACK(TOWNER_GUILD, pLocal->m_dwGuild, pPvP->m_dwIncPoint, bEvent, PVP_TOTAL | PVP_USEABLE, TRUE);
#ifdef DEF_UDPLOG
				_AtlModule.m_pUdpSocket->LogPvPointGuild(LOGMAP_PVPOINTGAINGUILD,pPvP->m_dwIncPoint,pLocal->m_wLocal, pLocal->m_bCountry ,pLocal->m_dwGuild,bEvent,PVP_TOTAL | PVP_USEABLE);
#endif
			}
		}
		break;
	case PVPE_DEFEND:
		{
			if(!pObject)
				return;

			LPTLOCAL pLocal = (LPTLOCAL)pObject;

			MAPTPVPOINT::iterator itF = pLocal->m_pZone->m_mapTPvPoint.find(MAKEWORD(PVPS_LOCAL, PVPE_DEFEND));
			if(itF == pLocal->m_pZone->m_mapTPvPoint.end())
				return;

			LPTPVPOINT pPvP = (*itF).second;

			if(pPvP->m_dwIncPoint && pLocal->m_dwGuild)
			{
				SendMW_GAINPVPPOINT_ACK(TOWNER_GUILD, pLocal->m_dwGuild, pPvP->m_dwIncPoint, bEvent, PVP_TOTAL | PVP_USEABLE, TRUE);
#ifdef DEF_UDPLOG
				_AtlModule.m_pUdpSocket->LogPvPointGuild(LOGMAP_PVPOINTGAINGUILD,pPvP->m_dwIncPoint,pLocal->m_wLocal , pLocal->m_bCountry ,pLocal->m_dwGuild,bEvent,PVP_TOTAL | PVP_USEABLE);
#endif
			}
		}
		break;
	default:
		break;
	}
}

void CTMapSvrModule::LocalRecord(LPTLOCAL pLocal, CTPlayer * pPlayer, BYTE bEvent, DWORD dwPoint, BYTE bGain)
{
	TENTRYRECORD rec;
	DWORD dwGuildID = pPlayer->GetGuild();

	MAPTLOCALRECORD::iterator itL = pLocal->m_mapTRecord.find(dwGuildID);
	if(itL != pLocal->m_mapTRecord.end())
	{
		MAPTENTRYRECORD::iterator itE = (*itL).second.find(pPlayer->m_dwID);
		if(itE != (*itL).second.end())
		{
			SetEntryRecord((*itE).second, bGain, bEvent, dwPoint);
		}
		else
		{
			rec.m_dwCharID = pPlayer->m_dwID;
			rec.m_strName = pPlayer->m_strNAME;
			rec.m_bRace = pPlayer->m_bRace;
			rec.m_bClass = pPlayer->m_bClass;
			rec.m_bLevel = pPlayer->m_bLevel;
			rec.m_wKillCount = 0;
			rec.m_wDieCount = 0;
			memset(&(rec.m_aGainPoint), 0, sizeof(rec.m_aGainPoint));

			SetEntryRecord(rec, bGain, bEvent, dwPoint);
			(*itL).second.insert(MAPTENTRYRECORD::value_type(pPlayer->m_dwID, rec));
		}
	}
	else
	{
		MAPTENTRYRECORD mapRec;
		mapRec.clear();

		rec.m_dwCharID = pPlayer->m_dwID;
		rec.m_strName = pPlayer->m_strNAME;
		rec.m_bRace = pPlayer->m_bRace;
		rec.m_bClass = pPlayer->m_bClass;
		rec.m_bLevel = pPlayer->m_bLevel;
		rec.m_wKillCount = 0;
		rec.m_wDieCount = 0;
		memset(&(rec.m_aGainPoint), 0, sizeof(rec.m_aGainPoint));

		SetEntryRecord(rec, bGain, bEvent, dwPoint);
		mapRec.insert(MAPTENTRYRECORD::value_type(pPlayer->m_dwID, rec));
		pLocal->m_mapTRecord.insert(MAPTLOCALRECORD::value_type(dwGuildID, mapRec));
	}
}

void CTMapSvrModule::SetEntryRecord(TENTRYRECORD & rec, BYTE bGain, BYTE bEvent, DWORD dwPoint)
{
	if(bGain)
		rec.m_aGainPoint[bEvent] += dwPoint;

	if(bEvent >= PVPE_KILL_H && bEvent <= PVPE_KILL_L)
	{
		if(bGain)
			rec.m_wKillCount++;
		else
			rec.m_wDieCount++;
	}
}

DWORD CTMapSvrModule::CalcCastlePoint(DWORD dwWarPoint, WORD wLocalPoint, WORD wKillPoint)
{
	return dwWarPoint + DWORD(wLocalPoint) * 5 + wKillPoint;
}

void CTMapSvrModule::EndWar(LPTLOCAL pCastle, CTMap * pMap, BYTE bType)
{
	pCastle->m_bCanBattle = FALSE;

	if(!pMap->m_dwEndTick)
		return;

	DWORD dwWinGuild = pCastle->m_dwGuild;
	if(pCastle->m_dwAtkGuildID && !pCastle->m_dwDefGuildID)
		dwWinGuild = pCastle->m_dwAtkGuildID;
	else if(!pCastle->m_dwAtkGuildID && pCastle->m_dwDefGuildID)
		dwWinGuild = pCastle->m_dwDefGuildID;
	else
		dwWinGuild = pMap->m_dwAtkPower > pMap->m_dwDefPower ? pCastle->m_dwAtkGuildID : pCastle->m_dwDefGuildID;

	MAPTCELL::iterator finder;
	WORD wLBonus = pMap->GetLocalBonus(pCastle->m_bDefCountry, pCastle->m_wDPoint, pCastle->m_wCPoint);

	for(finder = pMap->m_mapTCELL.begin(); finder!=pMap->m_mapTCELL.end(); finder++)
	{
		CTCell *pCell = (*finder).second;
		MAPPLAYER::iterator it;
		for( it = pCell->m_mapPLAYER.begin(); it != pCell->m_mapPLAYER.end(); it++)
		{
			if((*it).second->m_bMain)
				(*it).second->SendCS_ENDWAR_ACK(
					bType,
					dwWinGuild,
					pMap->m_dwDefPower,
					pMap->m_dwAtkPower,
					pCastle->m_strDefName,
					pMap->m_dwDefPower,
					wLBonus,
					pMap->m_wDefKillPoint,
					pCastle->m_strAtkName,
					pMap->m_dwAtkPower,
					TLOCALBONUS_POINT - wLBonus,
					pMap->m_wAtkKillPoint);
		}
	}

	SendDM_CASTLEOCCUPY_REQ(
		pCastle->m_pZone->m_wID,
		dwWinGuild == pCastle->m_dwGuild ? OCCUPY_DEFEND : OCCUPY_ACCEPT,
		dwWinGuild,
		dwWinGuild == pCastle->m_dwDefGuildID ? pCastle->m_dwAtkGuildID : pCastle->m_dwDefGuildID);

	pMap->EndWar();

	for(DWORD i=0; i<pCastle->m_pZone->m_vSpawnID.size(); i++)
	{
		MAPTMONSPAWN::iterator it = pMap->m_mapTMONSPAWN.find(pCastle->m_pZone->m_vSpawnID[i]);

		if(it!=pMap->m_mapTMONSPAWN.end())
		{
			(*it).second->m_bStatus = MONSPAWN_READY;
			for(DWORD j=0; j<(*it).second->m_vTMON.size(); j++)
			{
				(*it).second->m_vTMON[j]->OnEvent( AT_DEAD, 0, 0, 0, 0);
			}
		}
	}

	pCastle->m_bStatus = BS_PEACE;
	CastleReward(pCastle, dwWinGuild);
}

BYTE CTMapSvrModule::CanApplyCastle(LPTLOCAL pCastle)
{
	if(m_timeCurrent >= pCastle->m_timeNextDefend)
		return CBS_NOTREADY;

	if(!pCastle->m_mapLocal.empty())
	{
		if((*(pCastle->m_mapLocal.begin())).second->m_timeNextDefend < pCastle->m_timeNextDefend)
			return CBS_NOTREADY;
	}

	return EC_NOERROR;
}

BYTE CTMapSvrModule::CanEnterCastle(LPTLOCAL pCastle)
{
	if(!CanApplyCastle(pCastle) || pCastle->m_bStatus == BS_BATTLE)
		return TRUE;

	return FALSE;
}

BYTE CTMapSvrModule::IsCastleMap(CTMap * pMap)
{
	if(!pMap || !pMap->m_pCastle)
		return FALSE;

	return TRUE;
}

void CTMapSvrModule::LeaveCastleMap(CTMap * pMap)
{
	MAPTCELL::iterator itCell;
	for(itCell=pMap->m_mapTCELL.begin(); itCell!=pMap->m_mapTCELL.end(); itCell++)
	{
		CTCell * pCell = (*itCell).second;
		MAPPLAYER::iterator itChar;
		for(itChar=pCell->m_mapPLAYER.begin(); itChar != pCell->m_mapPLAYER.end(); itChar++)
		{
			if((*itChar).second->m_bMain)
			{
				CTPlayer * pChar = (*itChar).second;
				Teleport(pChar, pChar->m_wLastSpawnID ? pChar->m_wLastSpawnID : pChar->m_wSpawnID);
			}
		}
	}
}

void CTMapSvrModule::AddMonSpawn(CTMap * pMap, WORD wSpawnID, BYTE bStatus)
{
	MAPTMONSPAWNTEMP::iterator it = m_mapTMONSPAWN.find(wSpawnID);
	if(it!=m_mapTMONSPAWN.end())
	{
		CTMonSpawn * pSp = pMap->AddTimelimitedMon( (*it).second, pMap->m_bChannel, (*it).second->m_bCountry, RT_ETERNAL);
		if(pSp)
			pSp->m_bStatus = bStatus;
	}
}

void CTMapSvrModule::DelMonSpawn(CTMap * pMap, WORD wSpawnID, BYTE bErase)
{
	MAPTMONSPAWNTEMP::iterator itMS = m_mapTMONSPAWN.find(wSpawnID);
	if(itMS!=m_mapTMONSPAWN.end())
	{
		pMap->DelMonSpawn((*itMS).second);

		if(bErase)
		{
			MAPTCHANNEL::iterator itCH;
			for( itCH = m_mapTCHANNEL.begin(); itCH != m_mapTCHANNEL.end(); itCH++)
			{
				MAPTMAP::iterator itMAP;
				for(itMAP=(*itCH).second->m_mapTMAP.begin(); itMAP != (*itCH).second->m_mapTMAP.end(); itMAP++)
				{
					MAPTMONSPAWN::iterator finder = itMAP->second->m_mapTMONSPAWN.find(wSpawnID);
					if( finder != itMAP->second->m_mapTMONSPAWN.end() )
						return;
				}
			}

			m_mapTMONSPAWN.erase(itMS);
			m_mapExtraSpawnID.insert(MAPWORD::value_type(wSpawnID,wSpawnID));
		}
	}
}

BYTE CTMapSvrModule::CheckMapLevel(CTPlayer * pPlayer)
{
	if(!pPlayer->m_wMapID || !pPlayer->m_dwLastDestination)
		return FALSE;

	MAPTPORTAL::iterator itPt = m_mapTPortal.find(LOWORD(pPlayer->m_dwLastDestination));
	if(itPt != m_mapTPortal.end())
	{
		MAPTDESTINATION::iterator itDEST = (*itPt).second->m_mapDestination.find(HIWORD(pPlayer->m_dwLastDestination));
		if(itDEST != (*itPt).second->m_mapDestination.end())
		{
			LPTDESTINATION pDest = (*itDEST).second;
			for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
			{
				if(pDest->m_bCondition[i] == PCT_UPLEVEL && pDest->m_dwConditionID[i] < pPlayer->m_bLevel)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BYTE CTMapSvrModule::CheckMapGuild(CTPlayer * pPlayer)
{
	if(!pPlayer->m_wMapID || !pPlayer->m_dwLastDestination)
		return FALSE;

	MAPTPORTAL::iterator itPt = m_mapTPortal.find(LOWORD(pPlayer->m_dwLastDestination));
	if(itPt != m_mapTPortal.end())
	{
		MAPTDESTINATION::iterator itDEST = (*itPt).second->m_mapDestination.find(HIWORD(pPlayer->m_dwLastDestination));
		if(itDEST != (*itPt).second->m_mapDestination.end())
		{
			LPTDESTINATION pDest = (*itDEST).second;
			for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
			{
				if(pDest->m_bCondition[i] == PCT_GUILD &&
					pDest->m_pPortal &&
					pDest->m_pPortal->m_pLocal &&
					pDest->m_pPortal->m_pLocal->m_dwGuild != pPlayer->m_dwGuildID)
					return TRUE;
			}
		}
	}

	return FALSE;
}

void CTMapSvrModule::GetLocalList(BYTE bType, LPVTLOCAL pLocal)
{
	MAPTLOCAL::iterator it;
	for(it=m_mapTLOCAL.begin(); it!=m_mapTLOCAL.end(); it++)
		if( bType == LOCAL_NONE || (*it).second->m_bType == bType)
			pLocal->push_back((*it).second);
}

BYTE CTMapSvrModule::CheckCharName(LPCSTR str)
{
	int l = int(strlen(str));
//	if(0 > l || l > 16) return FALSE;

	unsigned char* p = (unsigned char *)str;
	unsigned char* e = (unsigned char *)(str+l);

	while(p < e)
	{
		switch(m_bNation)
		{
		case NATION_GERMAN:
			{
				if('0' <= *p && *p <= '9') 
					++p;				
				else if('a' <= *p && *p <= 'z') 
					++p;	
				else if('A' <= *p && *p <= 'Z') 
					++p;
				else if( (*p == 220) ||
						(*p == 214) ||
						(*p == 196) ||
						(*p == 252) ||
						(*p == 246) ||
						(*p == 228) ||
						(*p == 223) )
					++p;
				else 
					return FALSE;
			}
			break;
		case NATION_TAIWAN:
			{
				while(p < e)
				{
					if( (0xA1 <= *p) && (*p <= 0xF9) && (p+1 < e)  )
					{
						if ( (0xA1 <= *p && *p <= 0xC5) || (0xC9 <= *p && *p <= 0xF8) )
						{
							if ( (0x40 <= *(p+1) && *(p+1) <= 0x7E) || (0xA1 <= *(p+1) && *(p+1) <= 0xFE) )
								p += 2;
							else
								return FALSE;
						}
						else if ( *p == 0xC6 )
						{
							if ( 0x40 <= *(p+1) && *(p+1) <= 0x7E )
								p += 2;
							else 
								return FALSE;
						}
						else if ( *p == 0xF9 )
						{
							if ( (0x40 <= *(p+1) && *(p+1) <= 0x7E) || (0xA1 <= *(p+1) && *(p+1) <= 0xDC) )
								p += 2;
							else
								return FALSE;
						}

					}
					else
					{
						if('0' <= *p && *p <= '9') 
							++p;				
						else if('a' <= *p && *p <= 'z') 
							++p;	
						else if('A' <= *p && *p <= 'Z') 
							++p;	
						else 
							return FALSE;
					}
				}
			}
			break;
		case NATION_US:
			{
				if('0' <= *p && *p <= '9') 
					++p;				
				else if('a' <= *p && *p <= 'z') 
					++p;	
				else if('A' <= *p && *p <= 'Z') 
					++p;
				else
					return FALSE;
			}
			break;
		case NATION_JAPAN:
			{
				if( 0xA6 <= *p && *p <= 0xDD )
				{
					p += 1;
					continue;
				}
				else if(p+1 < e && 0x80 <= *p)
				{
					if( *p == 0x81 && *(p+1) == 0x5B )
					{
						p += 2;
					}
					else if( *p == 0x82 )
					{
						p += 2;
					}
					else if( *p == 0x83 )
					{
						if( 0x40 <= *(p+1) && *(p+1) <= 0x96 )
							p += 2;
						else
							return FALSE;
					}
					else if( 0x88 <= *p && *p <= 0xFC )
					{
						p += 2;
					}
					else
						return FALSE;
				}
				else
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else 
						return FALSE;
				}
			}
			break;
		case NATION_KOREA:
			{
				if(p+1 < e && 0x80 <= *p)
				{
					if(0xB0 <= *p && *p <= 0xC8 && 0xA1 <= *(p+1) && *(p+1) <= 0xFE)
						p += 2;	
					else
						return FALSE;
				}
				else
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else 
						return FALSE;
				} 
			}
			break;
		case NATION_RUSSIA:
			{
				while(p < e)
				{
					if('0' <= *p && *p <= '9') 
						++p;				
					else if('a' <= *p && *p <= 'z') 
						++p;	
					else if('A' <= *p && *p <= 'Z') 
						++p;	
					else if ( 0xC0 <= *p && *p <= 0xFF )
						++p;
					else if ( 0xA8 == *p || 0xB8 == *p )
						++p;
					else 
						return FALSE;
				}
			}
			break;
		default:
			return FALSE;
		}
	}

	return TRUE;
}
 
BYTE CTMapSvrModule::CheckMonthRank(CTPlayer * pPlayer, BYTE bCountry, DWORD dwMonthPoint, DWORD dwTotalPoint)
{
	if(bCountry > TCONTRY_B)
		return FALSE;

	BYTE bUpdate = FALSE;

	for(BYTE i = 0; i < MONTHRANKCOUNT; i++)
		if( m_arMonthRank[bCountry][i].m_dwCharID == pPlayer->m_dwID || m_arMonthRank[bCountry][i].m_dwMonthPoint < dwMonthPoint)
		{
			bUpdate = TRUE;
			break;
		}

	if(m_arMonthRank[bCountry][0].m_dwTotalPoint < dwTotalPoint)
		bUpdate = TRUE;

	if(bUpdate)
	{
		MONTHRANKER stMONTHRANKER;
		stMONTHRANKER.m_dwCharID = pPlayer->m_dwID;
		stMONTHRANKER.m_strName = pPlayer->m_strNAME;
		stMONTHRANKER.m_dwTotalPoint = dwTotalPoint;
		stMONTHRANKER.m_dwMonthPoint = dwMonthPoint;
		stMONTHRANKER.m_wMonthWin = pPlayer->m_wMonthWin;
		stMONTHRANKER.m_wMonthLose = pPlayer->m_wMonthLose;
		stMONTHRANKER.m_dwTotalWin = pPlayer->m_dwTotalWin;
		stMONTHRANKER.m_dwTotalLose = pPlayer->m_dwTotalLose;
		stMONTHRANKER.m_bCountry = bCountry;
		stMONTHRANKER.m_bLevel = pPlayer->m_bLevel;
		stMONTHRANKER.m_bClass = pPlayer->m_bClass;
		stMONTHRANKER.m_bRace = pPlayer->m_bRace;
		stMONTHRANKER.m_bSex = pPlayer->m_bSex;
		stMONTHRANKER.m_bHair = pPlayer->m_bHair;
		stMONTHRANKER.m_bFace = pPlayer->m_bFace;
		stMONTHRANKER.m_strSay = NAME_NULL;
		stMONTHRANKER.m_strGuild = pPlayer->m_strGuildName;

		SendMW_MONTHRANKUPDATE_ACK(stMONTHRANKER);
	}

	return bUpdate;
}
 
void CTMapSvrModule::AddEvent(EVENTINFO &stEVENT)
{
	if(stEVENT.m_bID != EVENT_MONSPAWN || stEVENT.m_bID != EVENT_MONREGEN)
		DelEvent(stEVENT.m_bID);

	m_mapEVENT.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT) );
}

void CTMapSvrModule::DelEvent(DWORD dwIndex)
{
	MAPEVENTINFO::iterator itE = m_mapEVENT.find(dwIndex);
	if( itE != m_mapEVENT.end())
		m_mapEVENT.erase(itE);
}

void CTMapSvrModule::DelEvent(BYTE bEventID)
{
	MAPEVENTINFO::iterator itE;
	for(itE = m_mapEVENT.begin(); itE != m_mapEVENT.end(); itE++)
	{
		if( (*itE).second.m_bID == bEventID)
		{
			m_mapEVENT.erase(itE);
			break;
		}
	}
}

LPEVENTINFO CTMapSvrModule::GetEventInfo(BYTE bEventID)
{
	MAPEVENTINFO::iterator itE;
	for(itE = m_mapEVENT.begin(); itE != m_mapEVENT.end(); itE++)
	{
		if( (*itE).second.m_bID == bEventID)		
			return &( (*itE).second);		
	}
	
	return NULL;
}

LPEVENTINFO CTMapSvrModule::GetEventInfo(DWORD dwIndex)
{
	MAPEVENTINFO::iterator itE = m_mapEVENT.find(dwIndex);
	if( itE != m_mapEVENT.end() )
		return &( (*itE).second );

	return NULL;
}

LPTOURNAMENT CTMapSvrModule::FindTournament(BYTE bEntryID)
{
	MAPTOURNAMENT::iterator it = m_mapTournament.find(bEntryID);
	if(it!=m_mapTournament.end())
		return &((*it).second);
	
	return NULL;
}

void CTMapSvrModule::ClearTournamentPlayer()
{
	MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(DEFAULT_CHANNEL);
	if(itCH != m_mapTCHANNEL.end())
	{
		MAPTMAP::iterator itMAP;
		for(itMAP=(*itCH).second->m_mapTMAP.begin(); itMAP!=(*itCH).second->m_mapTMAP.end(); itMAP++)
		{
			if(IsTournamentMap((*itMAP).second->m_wMapID))
				(*itMAP).second->ResetTournament();
		}
	}

	MAPTOURNAMENTPLAYER::iterator itTM;
	for(itTM=m_mapTNMTPlayer.begin(); itTM!=m_mapTNMTPlayer.end(); itTM++)
		delete (*itTM).second;

	m_mapTNMTPlayer.clear();
}

WORD CTMapSvrModule::GetTournamentMap(DWORD dwCharID, BYTE bGroup)
{
	MAPTOURNAMENTPLAYER::iterator it = m_mapTNMTPlayer.find(dwCharID);
	if(it!=m_mapTNMTPlayer.end())
	{
		LPTOURNAMENTPLAYER pPlayer = (*it).second;
		if(bGroup)
		{	
			LPTOURNAMENT pEntry = FindTournament(pPlayer->m_bEntryID);
			if(!pEntry || pEntry->m_bGroup != bGroup)
				return 0;
		}

		if(pPlayer->m_bResult[MATCH_QFINAL] == TNMTWIN_LOSE ||
			pPlayer->m_bResult[MATCH_SFINAL] == TNMTWIN_LOSE ||
			pPlayer->m_bResult[MATCH_FINAL] == TNMTWIN_LOSE)
			return 0;

		if(pPlayer->m_bResult[MATCH_SFINAL] == TNMTWIN_WIN && pPlayer->m_bResult[MATCH_FINAL] == TNMTWIN_NONE && m_tournament.m_bStep >= TNMTSTEP_FENTER)
			return MAPID_TOURNAMENT + (pPlayer->m_bEntryID-1) * ARENA_COUNT;
		else if(pPlayer->m_bResult[MATCH_QFINAL] == TNMTWIN_WIN && pPlayer->m_bResult[MATCH_SFINAL] == TNMTWIN_NONE && m_tournament.m_bStep >= TNMTSTEP_SFENTER)
			return MAPID_TOURNAMENT + (pPlayer->m_bEntryID-1) * ARENA_COUNT + (pPlayer->m_bSlot/4)*2;
		else if(pPlayer->m_bResult[MATCH_QFINAL] == TNMTWIN_NONE && m_tournament.m_bStep < TNMTSTEP_SFENTER)
			return MAPID_TOURNAMENT + (pPlayer->m_bEntryID-1) * ARENA_COUNT + pPlayer->m_bSlot/2;
	}

	return 0;
}

BYTE CTMapSvrModule::IsTournamentMap(WORD wMapID)
{
	if(wMapID >= MAPID_TOURNAMENT && wMapID < MAPID_TOURNAMENT + ARENA_COUNT * 8)
		return TRUE;

	return FALSE;
}

WORD CTMapSvrModule::GetLoungeMapID()
{
	return MAPID_TOURNAMENT-1;
}

BYTE CTMapSvrModule::GetTournamentStep()
{
	if(m_mapTournament.empty())
		return TNMTSTEP_READY;

	return m_tournament.m_bStep;
}

BYTE CTMapSvrModule::CanDoTournament(BYTE bStep)
{
	if(	bStep == TNMTSTEP_MATCH ||
		bStep == TNMTSTEP_ENTER)
		return (GetTournamentStep() >= bStep) ? TRUE : FALSE;

	return (GetTournamentStep() == bStep) ? TRUE : FALSE;
}

LPTOURNAMENTPLAYER CTMapSvrModule::FindTournamentPlayer(DWORD dwCharID, BYTE bActive)
{
	MAPTOURNAMENTPLAYER::iterator it = m_mapTNMTPlayer.find(dwCharID);
	if(it!=m_mapTNMTPlayer.end())
	{
		LPTOURNAMENT pEntry = FindTournament((*it).second->m_bEntryID);
		if(!pEntry || pEntry->m_bGroup != m_tournament.m_bGroup)
			return NULL;

		if(bActive)
		{
			if((*it).second->m_bResult[MATCH_QFINAL] != TNMTWIN_LOSE &&
				(*it).second->m_bResult[MATCH_SFINAL] != TNMTWIN_LOSE &&
				(*it).second->m_bResult[MATCH_FINAL] != TNMTWIN_LOSE)
				return (*it).second;
		}
		else
			return (*it).second;
	}

	return NULL;
}

void CTMapSvrModule::SetTournamentResult(BYTE bStep, LPTOURNAMENTPLAYER pPlayer, BYTE bResult)
{
	BYTE bID;

	if(bStep == TNMTSTEP_FINAL)	bID = MATCH_FINAL;
	else if(bStep == TNMTSTEP_SFINAL) bID = MATCH_SFINAL;
	else bID = MATCH_QFINAL;

	pPlayer->m_bResult[bID] = bResult;
}

BYTE CTMapSvrModule::IsActiveTournamentMap(WORD wMapID)
{
	BYTE bStep = GetTournamentStep();

	if(bStep == TNMTSTEP_READY)
		return FALSE;
	else if(bStep < TNMTSTEP_SFENTER)
	{
		MAPTOURNAMENT::iterator it;
		for(it = m_mapTournament.begin(); it!=m_mapTournament.end(); it++)
		{
			BYTE bGroup = (*it).second.m_bGroup;
			if(bGroup != m_tournament.m_bGroup)
				continue;

			for(BYTE s=0; s<TOURNAMENT_SLOT; s++)
				if(wMapID >= MAPID_TOURNAMENT + WORD((*it).second.m_bEntryID-1) * ARENA_COUNT &&
					wMapID < MAPID_TOURNAMENT + WORD((*it).second.m_bEntryID) * ARENA_COUNT)
					return TRUE;
		}
	}
	else if(bStep < TNMTSTEP_FENTER)
	{
		MAPTOURNAMENT::iterator it;
		for(it = m_mapTournament.begin(); it!=m_mapTournament.end(); it++)
		{
			BYTE bGroup = (*it).second.m_bGroup;
			if(bGroup != m_tournament.m_bGroup)
				continue;

			for(BYTE s=0; s<TOURNAMENT_SLOT; s++)
				if(wMapID == MAPID_TOURNAMENT + WORD((*it).second.m_bEntryID-1) * ARENA_COUNT + (s/4)*2)
					return TRUE;
		}
	}
	else
	{
		MAPTOURNAMENT::iterator it;
		for(it = m_mapTournament.begin(); it!=m_mapTournament.end(); it++)
		{
			BYTE bGroup = (*it).second.m_bGroup;
			if(bGroup != m_tournament.m_bGroup)
				continue;

			if(wMapID == MAPID_TOURNAMENT + WORD((*it).second.m_bEntryID-1) * ARENA_COUNT)
				return TRUE;
		}
	}

	return FALSE;
}

void CTMapSvrModule::GetTournamentMapList(LPVTMAP vMAP)
{
	MAPTCHANNEL::iterator itCH = m_mapTCHANNEL.find(DEFAULT_CHANNEL);
	if(itCH != m_mapTCHANNEL.end())
	{
		MAPTMAP::iterator itMAP;
		for(itMAP=(*itCH).second->m_mapTMAP.begin(); itMAP!=(*itCH).second->m_mapTMAP.end(); itMAP++)
			if(IsActiveTournamentMap((*itMAP).second->m_wMapID))
				vMAP->push_back((*itMAP).second);
	}
}

BYTE CTMapSvrModule::CanApplyTournament(CTPlayer * pPlayer, BYTE bEntryID)
{
	LPTOURNAMENT pTour = FindTournament(bEntryID);
	if(!pTour)
		return TOURNAMENT_FAIL;

	if(!(pTour->m_dwClass & BITSHIFTID(pPlayer->m_bClass)))
		return TOURNAMENT_CLASS;

	if(!pPlayer->UseMoney(pTour->m_dwFee, FALSE))
		return TOURNAMENT_MONEY;

	if(pTour->m_wPermitItem && !pPlayer->HaveItem(pTour->m_wPermitItem, pTour->m_bPermitCount))
		return TOURNAMENT_ITEM;

	if(pTour->m_bMaxLevel < pPlayer->GetLevel())
		return TOURNAMENT_DISQUALIFY;

	return TOURNAMENT_SUCCESS;
}

BYTE CTMapSvrModule::ApplyTournament(LPTOURNAMENT pTour, CTPlayer * pPlayer, BYTE bEntryID)
{
	if(!pTour)
		return FALSE;

	if(pTour->m_wPermitItem && !pPlayer->HaveItem(pTour->m_wPermitItem, pTour->m_bPermitCount))
		return FALSE;

	if(!pPlayer->UseMoney(pTour->m_dwFee, TRUE))
		return FALSE;

	if(pTour->m_wPermitItem)
		pPlayer->UseItem(pTour->m_wPermitItem, pTour->m_bPermitCount);

	return TRUE;
}

WORD CTMapSvrModule::GetTNMTPlayerLounge(CTPlayer * pPlayer)
{
	MAPTSPAWNPOS::iterator finder = m_mapTSPAWNPOS.find(TOURNAMENT_PLOUNGE_SPAWNID);
	if(finder != m_mapTSPAWNPOS.end())
		return TOURNAMENT_PLOUNGE_SPAWNID;

	return pPlayer->m_wLastSpawnID ? pPlayer->m_wLastSpawnID : pPlayer->m_wSpawnID;
}

BYTE CTMapSvrModule::SetTutorialCharBase(CTPlayer * pPlayer)
{
	if(pPlayer->m_bStartAct != 1 && pPlayer->m_wMapID == TUTORIAL_MAPID)
		return FALSE;

	if(!pPlayer->IsTutorial())
		return TRUE;

	pPlayer->m_dwGold = 1;
	pPlayer->m_bLevel = 80;
	pPlayer->m_wSpawnID = 15000;
	pPlayer->m_wPostTotal = 0;
	pPlayer->m_wPostRead = 0;

	MAPTSKILLTEMP::iterator it;
	for(it=m_mapTSKILL.begin(); it!=m_mapTSKILL.end(); it++)
	{
		CTSkillTemp * pTemp = (*it).second;
		if(pTemp->m_bCanLearn && (pTemp->m_dwClassID & BITSHIFTID(pPlayer->m_bClass)))
		{
			CTSkill * pSkill = new CTSkill();
			pSkill->m_pTSKILL = pTemp;
			pSkill->m_bLevel = pTemp->m_bMaxLevel;

			//스킬 버릴때 m_vRemainSkill에서 해당 스킬 꼭 뺄것
			pPlayer->m_mapTSKILL.insert(MAPTSKILL::value_type(pTemp->m_wID, pSkill));
			pPlayer->RemainSkill( pSkill, 0);
		}
	}

	return TRUE;
}

void CTMapSvrModule::SetTutorialCharItem(CTPlayer * pPlayer)
{
	if(!pPlayer->IsTutorial())
		return;

	pPlayer->m_dwHP = pPlayer->GetMaxHP();
	pPlayer->m_dwMP = pPlayer->GetMaxMP();

	for(BYTE i=0; i<ES_COUNT; i++)
	{
		WORD wItemID = m_arTutorialItem[pPlayer->m_bCountry][pPlayer->m_bClass][pPlayer->m_bSex][i];
		if(wItemID)
		{
			LPTITEM pItem = FindTItem(wItemID);
			if(!pItem)
				continue;

			CTItem * pNew = new CTItem();
			pNew->m_bItemID = i;
			pNew->m_bCount = 1;
			pNew->m_wItemID = wItemID;
			pNew->m_pTITEM = pItem;
			pNew->SetDuration(FALSE);
			SetItemAttr(pNew, 0);

			CTInven *pTINVEN = pPlayer->FindTInven(INVEN_EQUIP);
			if(!pTINVEN)
				delete pNew;
			else
				pTINVEN->m_mapTITEM.insert( MAPTITEM::value_type( pNew->m_bItemID, pNew));
		}
	}
}

BYTE CTMapSvrModule::GetHelpMessage(CTPlayer * pPlayer, CString & strMessage)
{
	if(pPlayer->IsTutorial())
	{
		MAPHELPMESSAGE::iterator itHm = m_mapHelpMessage.find(HELPMESSAGE_TUTORIAL);
		if(itHm != m_mapHelpMessage.end())
		{
			strMessage = (*itHm).second.m_strMessage;
			return BYTE((*itHm).first);
		}
	}
	else
	{
		MAPHELPMESSAGE::iterator itHm = m_mapHelpMessage.find(HELPMESSAGE_EVENT);
		if(itHm != m_mapHelpMessage.end() && (*itHm).second.m_bNotice)
		{
			strMessage = (*itHm).second.m_strMessage;
			return BYTE((*itHm).first);
		}
		else
		{
			itHm = m_mapHelpMessage.find(HELPMESSAGE_NORMAL);
			if(itHm != m_mapHelpMessage.end())
			{
				strMessage = (*itHm).second.m_strMessage;
				return BYTE((*itHm).first);
			}
		}
	}

	return HELPMESSAGE_NONE;
}

BYTE CTMapSvrModule::CheckPeaceZone(CTPlayer * pPlayer)
{
	if(	pPlayer->m_pLocal && 
		!pPlayer->m_pLocal->m_bCanBattle )
		return TRUE;

	if(pPlayer->m_wMapID == GetLoungeMapID())
		return TRUE;

	if( IsTournamentMap(pPlayer->m_wMapID) &&
		m_tournament.m_bStep != TNMTSTEP_QFINAL &&
		m_tournament.m_bStep != TNMTSTEP_SFINAL &&
		m_tournament.m_bStep != TNMTSTEP_FINAL)
		return TRUE;

	if(pPlayer->m_wArenaID)
	{
		MAPARENA::iterator itAn = m_mapArena.find(pPlayer->m_wArenaID);
		if(itAn != m_mapArena.end() && itAn->second->m_bStatus != BS_BATTLE)
			return TRUE;
	}

	if(IsMeetingRoom(pPlayer->m_wMapID, FALSE))
		return TRUE;

	return FALSE;
}

BYTE CTMapSvrModule::EquipItemRevision(CTPlayer * pPlayer, BYTE bRevision, BYTE bLevel)
{
	if(!bLevel)
		return FALSE;

	CTInven *pTINVEN = pPlayer->FindTInven(INVEN_EQUIP);
	if(pTINVEN)
	{
		MAPTITEM::iterator itTITEM;
		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;

			if(bRevision)
				SetItemAttr(pTITEM, min(bLevel, pTITEM->m_bLevel));
			else
				SetItemAttr(pTITEM, pTITEM->m_bLevel);
		}
	}

	pPlayer->SendCS_CHARSTATINFO_ACK(pPlayer);

	return TRUE;
}

void CTMapSvrModule::RPSReward(CTPlayer * pPlayer, BYTE bGiveup)
{
	if(pPlayer->m_bRPSWin)
	{
		MAPRPSGAME::iterator it = m_mapRPSGame.find(MAKEWORD(pPlayer->m_bRPSType, pPlayer->m_bRPSWin));
		if(it == m_mapRPSGame.end())
		{
			pPlayer->ResetRPS();
			return;
		}

		TRPSGAME rps = (*it).second;

		VTITEM vItem;
		vItem.clear();

		for(BYTE i=0; i<2; i++)
		{
			if(rps.m_pRewardItem[i])
			{
				CTItem * pNew = new CTItem();
				pNew->m_dlID = GenItemID();
				pNew->m_bCount = rps.m_bItemCount[i];
				pNew->m_wItemID = rps.m_pRewardItem[i]->m_wItemID;
				pNew->m_pTITEM = rps.m_pRewardItem[i];
				pNew->SetDuration(FALSE);
				SetItemAttr(pNew, 0);
				vItem.push_back(pNew);
			}
		}

		DWORD dwGold;
		DWORD dwSilver;
		DWORD dwCooper;
		CalcMoney(rps.m_dwRewardMoney, dwGold, dwSilver, dwCooper);

		SendDM_POSTRECVFROMSVR_REQ(
			pPlayer->m_dwID, pPlayer->m_strNAME,
			BuildNetString(NAME_NULL, GetSvrMsg(RPS_REWARD_TITLE)),
			BuildNetString(NAME_NULL, GetSvrMsg(RPS_REWARD_MESSAGE)),
			POST_PACKATE, dwGold, dwSilver, dwCooper, &vItem);

#ifdef DEF_UDPLOG
		m_pUdpSocket->LogRPSGame(LOGMAP_RPSGAME, pPlayer, pPlayer->m_bRPSType, pPlayer->m_bRPSWin, rps.m_dwRewardMoney);
#endif
	}

	pPlayer->ResetRPS();
}

BYTE CTMapSvrModule::SelectNpcRPS(BYTE bResult, BYTE bPlayerRPS)
{
	if(bResult == TNMTWIN_NONE)
		return bPlayerRPS;

	BYTE bNpcRPS = RPSCARD_ROCK;

	switch(bPlayerRPS)
	{
	case RPSCARD_ROCK:
		bNpcRPS = (bResult == TNMTWIN_WIN) ? RPSCARD_SCISSORS : RPSCARD_PAPER;
		break;
	case RPSCARD_PAPER:
		bNpcRPS = (bResult == TNMTWIN_WIN) ? RPSCARD_ROCK : RPSCARD_SCISSORS;
		break;
	case RPSCARD_SCISSORS:
		bNpcRPS = (bResult == TNMTWIN_WIN) ? RPSCARD_PAPER : RPSCARD_ROCK;
		break;
	}

	return bNpcRPS;
}

void CTMapSvrModule::EraseMissionSkill(CTPlayer * pPlayer)
{
	if(pPlayer->m_pLocal)
	{
		pPlayer->EraseMaintainSkill(pPlayer->m_pLocal->m_pZone->m_wSkill1);
		pPlayer->EraseMaintainSkill(pPlayer->m_pLocal->m_pZone->m_wSkill2);
		if(EquipItemRevision(pPlayer, FALSE, pPlayer->m_pLocal->m_pZone->m_bItemLevel))
			pPlayer->SendCS_ITEMLEVELREVISION_ACK(0);
	}
}

void CTMapSvrModule::LogMapEvent(DWORD dwV1, DWORD dwV2, CString strV3)
{
	static CString strFileName = NAME_NULL;
	if(strFileName.IsEmpty())
	{
		TCHAR szFN[ONE_KBYTE];
		GetModuleFileName(NULL, szFN, ONE_KBYTE); 
		char *pSlash = ::strrchr(szFN, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "TMapLog_");

		strFileName.Format("%s%d%s",szFN,m_bServerID,_T(".txt"));
	}

	FILE * file;
	file = fopen(strFileName, "a+");
	if(!file)
		file = fopen(strFileName, "w+");

	if(!file)
		return;

	CString log;
	CTime time(m_timeCurrent);

	log.Format("%s	SERVER_ID=%d	dwV1=%d	dwV2=%x	strV3=%s\n",time.Format("%c"), m_bServerID, dwV1, dwV2, strV3);
	fwrite(log, sizeof(char), lstrlen(log), file);
	fclose(file);
}

void CTMapSvrModule::CheckMeetingRoom(CTPlayer * pPlayer, BYTE bForce)
{
	if(!pPlayer->m_dwLastDestination)
		return;

	if(!bForce && !IsMeetingRoom(pPlayer->m_wMapID, TRUE))
		return;

	MAPTPORTAL::iterator itPt = m_mapTPortal.find(LOWORD(pPlayer->m_dwLastDestination));
	if(itPt != m_mapTPortal.end())
	{
		MAPTDESTINATION::iterator itDEST = (*itPt).second->m_mapDestination.find(HIWORD(pPlayer->m_dwLastDestination));
		if(itDEST != (*itPt).second->m_mapDestination.end())
		{
			LPTDESTINATION pDest = (*itDEST).second;
			for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
			{
				if(pDest->m_bCondition[i] == PCT_MEETING && pDest->m_dwConditionID[i] == pPlayer->m_dwID)
				{
					if(pPlayer->m_pMAP)
					{
						VPLAYER vPlayers;
						pPlayer->m_pMAP->GetMapPlayers(&vPlayers);
						while(!vPlayers.empty())
						{
							CTPlayer * pChar = vPlayers.back();
							vPlayers.pop_back();

							Teleport(pChar, MEETINGROOM_OUTPOS);
						}
					}

					pDest->m_dwConditionID[i] = 0;
					SendSM_MEETINGROOM_REQ(FALSE, pPlayer->m_dwID);

					return;
				}
			}
		}
	}
}

BYTE CTMapSvrModule::IsChiefMeetingRoom(CTPlayer * pPlayer)
{
	if(!IsMeetingRoom(pPlayer->m_wMapID, TRUE) || !pPlayer->m_dwLastDestination)
		return FALSE;

	if(!pPlayer->m_pMAP)
		return FALSE;

	MAPTPORTAL::iterator itPt = m_mapTPortal.find(LOWORD(pPlayer->m_dwLastDestination));
	if(itPt != m_mapTPortal.end())
	{
		MAPTDESTINATION::iterator itDEST = (*itPt).second->m_mapDestination.find(HIWORD(pPlayer->m_dwLastDestination));
		if(itDEST != (*itPt).second->m_mapDestination.end())
		{
			LPTDESTINATION pDest = (*itDEST).second;
			for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
			{
				if(pDest->m_bCondition[i] == PCT_MEETING && pDest->m_dwConditionID[i] == pPlayer->m_dwID)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BYTE CTMapSvrModule::IsMeetingRoom(WORD wMapID, BYTE bIsSmall)
{
	BYTE bIn = wMapID >= MEETING_MAPID && wMapID <= MEETING_MAPID + MEETING_SROOM_COUNT;

	if(bIsSmall)
		bIn = bIn && wMapID != MEETING_MAPID;

	return bIn;
}

WORD CTMapSvrModule::RegenDynamicMonster(WORD wMapID,
										 WORD wTempMonID,
										 FLOAT fPosX,
										 FLOAT fPosY,
										 FLOAT fPosZ,
										 DWORD dwDelay,
										 BYTE bRoamType)
{
	if(m_mapExtraSpawnID.empty())
		return 0;

	LPTMONSTER pTemp = FindTMonster(wTempMonID);
	if(!pTemp)
		return 0;

	MAPWORD::iterator itW = m_mapExtraSpawnID.begin();
	WORD wNewSpawnID = (*itW).second;
	m_mapExtraSpawnID.erase(itW);

	LPTMONSPAWN pTSPAWN = new TMONSPAWN();
	pTSPAWN->m_wID = wNewSpawnID;
	pTSPAWN->m_fPosX = fPosX;
	pTSPAWN->m_fPosY = fPosY;
	pTSPAWN->m_fPosZ = fPosZ;
	pTSPAWN->m_wDir = 0;

	pTSPAWN->m_dwDelay = dwDelay;
	pTSPAWN->m_bRoamType = bRoamType;

	pTSPAWN->m_bRange = 0;
	pTSPAWN->m_bProb = 100;
	pTSPAWN->m_bCount = 1;
	pTSPAWN->m_bCountry = TCONTRY_N;
	pTSPAWN->m_bEvent = SE_DYNAMIC;
	pTSPAWN->m_bLink = 0;
	pTSPAWN->m_dwRegion = 0;
	pTSPAWN->m_lpvGroup = NULL;
	pTSPAWN->m_vPARTY.clear();
	pTSPAWN->m_vPathPoint.clear();
	pTSPAWN->m_wGroup = 0;
	pTSPAWN->m_wLocalID = 0;			
	pTSPAWN->m_wMapID = wMapID;
	pTSPAWN->m_wPartyID = 0;
	pTSPAWN->m_bArea = 3;

	LPTMAPMON pTEMPMAPMON = new TMAPMON();
	pTEMPMAPMON->m_bProb = 100;
	pTEMPMAPMON->m_bEssential = 0;
	pTEMPMAPMON->m_bLeader = 0;
	pTEMPMAPMON->m_wMonID = wTempMonID;
	pTEMPMAPMON->m_wSpawnID = wNewSpawnID;

	pTSPAWN->m_vMAPMON.push_back(pTEMPMAPMON);

	m_mapTMONSPAWN.insert( MAPTMONSPAWNTEMP::value_type( pTSPAWN->m_wID, pTSPAWN));

	return wNewSpawnID;
}

void CTMapSvrModule::ArenaLeave(CTPlayer * pPlayer)
{
	if(!pPlayer->m_wArenaID || !pPlayer->m_bArenaTeam)
		return;

	MAPARENA::iterator it = m_mapArena.find(pPlayer->m_wArenaID);
	if(it == m_mapArena.end())
		return;

	if(it->second->m_mapFighter[pPlayer->m_bArenaTeam-1].size() <= 2)
	{
		ArenaEnd(it->second, pPlayer);
		return;
	}

	it->second->m_mapFighter[pPlayer->m_bArenaTeam-1].erase(pPlayer->m_dwID);

	pPlayer->m_wArenaID = 0;
	pPlayer->m_bArenaTeam = 0;

	if(pPlayer->m_pMAP)
	{
		VPLAYER vPlayer;
		VDWORD vParty;

		vParty.push_back(pPlayer->m_dwID);
		pPlayer->m_pMAP->GetNeerPlayer(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);
		while(!vPlayer.empty())
		{
			CTPlayer * pChar = vPlayer.back();
			vPlayer.pop_back();
			pChar->SendCS_ARENATEAM_ACK(it->second->m_wID, 0, vParty);
		}

		pPlayer->m_pMAP->OnMove(
			pPlayer,
			it->second->m_pOutPos->m_fPosX,
			it->second->m_pOutPos->m_fPosZ);
		pPlayer->m_fPosY = it->second->m_pOutPos->m_fPosY;

		Teleport(pPlayer, it->second->m_pOutPos->m_wID);
	}
}

void CTMapSvrModule::ArenaEnd(LPTARENA pArena, CTPlayer * pPlayer)
{
	CTPlayer * pWin = NULL;
	CTPlayer * pLose = NULL;

	CTMap * pMap = FindTMap(DEFAULT_CHANNEL, 0, pArena->m_pInPos->m_wMapID);
	if(!pMap)
		return;

	MAPPLAYER::iterator itP;
	BYTE bF0, bF1;
	bF0 = bF1 = 0;
	CTPlayer * pF0 = NULL;
	CTPlayer * pF1 = NULL;
	BYTE bOutID = 0;

	for(itP=pArena->m_mapFighter[0].begin(); itP != pArena->m_mapFighter[0].end(); itP++)
	{
		if(itP->second->m_bStatus != OS_DEAD)
			bF0++;
		else
			itP->second->Revival(AFTERMATH_NONE, NULL, 0);

		if(!pF0 || itP->second->GetPartyChiefID() == itP->second->m_dwID)
			pF0 = itP->second;

		itP->second->ResetCoolTime(0);
		itP->second->ReleaseMaintain(TRUE);
		itP->second->DeleteAllRecallMon();
		itP->second->DeleteAllSelfMon();

		if(pPlayer && pPlayer->m_dwID == itP->second->m_dwID)
			bOutID = 0;
	}
	for(itP=pArena->m_mapFighter[1].begin(); itP != pArena->m_mapFighter[1].end(); itP++)
	{
		if(itP->second->m_bStatus != OS_DEAD)
			bF1++;
		else
			itP->second->Revival(AFTERMATH_NONE, NULL, 0);

		if(!pF1 || itP->second->GetPartyChiefID() == itP->second->m_dwID)
			pF1 = itP->second;

		itP->second->ResetCoolTime(0);
		itP->second->ReleaseMaintain(TRUE);
		itP->second->DeleteAllRecallMon();
		itP->second->DeleteAllSelfMon();

		if(pPlayer && pPlayer->m_dwID == itP->second->m_dwID)
			bOutID = 1;
	}

	VPLAYER vPlayer;
	pMap->GetNeerPlayer(&vPlayer, pArena->m_pInPos->m_fPosX, pArena->m_pInPos->m_fPosZ);

	if(pPlayer)
	{
		if(bOutID)
		{
			if(pArena->m_bStatus == BS_NORMAL)
				pWin = pF1;
			else
				pWin = pF0;

			pLose = pF1;
		}
		else
		{
			if(pArena->m_bStatus == BS_NORMAL)
				pWin = pF0;
			else
				pWin = pF1;

			pLose = pF0;
		}
	}
	else
	{
		if(bF0 > bF1)
		{
			pWin = pF0;
			pLose = pF1;
		}
		else if(bF0 < bF1)
		{
			pWin = pF1;
			pLose = pF0;
		}
		else if(pArena->m_bDrawCount)
		{
			if(!pArena->m_bWinCount)
			{
				pWin = pF0;
				pLose = pF1;
			}
			else
			{
				pWin = pF0 && pF0->GetPartyID() == pArena->m_wWinPartyID ? pF0 : pF1;
				pLose = pF0 == pWin ? pF1 : pF0;
			}
		}
	}

	if(pLose)
	{
		BYTE bArenaTeam = pLose->m_bArenaTeam;

		if(!bArenaTeam)
			return;

		VDWORD vParty;
		MAPPLAYER::iterator itP;
		for(itP=pArena->m_mapFighter[bArenaTeam-1].begin(); itP != pArena->m_mapFighter[bArenaTeam-1].end(); itP++)
		{
			itP->second->m_wArenaID = 0;
			itP->second->m_bArenaTeam = 0;
			vParty.push_back(itP->second->m_dwID);

			itP->second->m_pMAP->OnMove(itP->second, pArena->m_pOutPos->m_fPosX, pArena->m_pOutPos->m_fPosZ);
			itP->second->m_fPosY = pArena->m_pOutPos->m_fPosY;

			Teleport(itP->second, pArena->m_pOutPos->m_wID);
		}

		pArena->m_mapFighter[bArenaTeam-1].clear();

		for(DWORD i=0; i<(DWORD)vPlayer.size(); i++)
		{
			CTPlayer * pChar = vPlayer[i];
			pChar->SendCS_ARENATEAM_ACK(pArena->m_wID, 0, vParty);
		}

		SendMW_ARENAJOIN_ACK(pLose->m_dwID, pLose->m_dwKEY, FALSE, vParty);
	}

	if(pWin)
	{
		DWORD dwGold, dwSilver, dwCooper;
		CalcMoney(pArena->m_dwFee, dwGold, dwSilver, dwCooper);

		SendDM_POSTRECV_REQ(
			0, pWin->m_dwID, GetSvrMsg(NAME_OPERATOR), pWin->m_strNAME,
			BuildNetString(NAME_NULL, GetSvrMsg(ARENA_WIN_TITLE)),
			BuildNetString(NAME_NULL, GetSvrMsg(ARENA_WIN_MESSAGE)),
			POST_PACKATE,
			dwGold,dwSilver,dwCooper, NULL);
	}

	if(!pWin && !pLose) 
	{
		pArena->m_bStatus = BS_READY;
		pArena->m_dwTick = m_dwTick;
		pArena->m_bDrawCount++;
	}
	else if(pWin && pWin != pLose)
	{
		pArena->m_bStatus = BS_NORMAL;
		pArena->m_dwTick = 0;

		if(pWin->GetPartyID() == pArena->m_wWinPartyID)
			pArena->m_bWinCount++;
		else
			pArena->m_bWinCount = 1;

		pArena->m_wWinPartyID = pWin->GetPartyID();
		pArena->m_bDrawCount = 0;
	}
	else
	{
		pArena->m_bStatus = BS_NORMAL;
		pArena->m_dwTick = 0;
		pArena->m_bWinCount = 0;
		pArena->m_wWinPartyID = 0;
		pArena->m_bDrawCount = 0;
	}

	for(DWORD i=0; i<(DWORD)vPlayer.size(); i++)
	{
		CTPlayer * pChar = vPlayer[i];
		pChar->SendCS_SYSTEMMSG_ACK(
			SM_ARENA_COUNTDOWN,
			pArena->m_wID,
			pArena->m_bStatus == BS_READY ? 60 : 0,
			NAME_NULL,
			NAME_NULL,
			pArena->m_bStatus,
			pArena->m_bWinCount);
	}
}

BYTE CTMapSvrModule::IsMainCell(BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosZ)
{
	CTChannel *pCH = FindTChannel(
		bChannel,
		wMapID);

	if(pCH)
	{
		MAPTMAP::iterator itMAP = pCH->m_mapTMAP.find(wMapID);
		if( itMAP != pCH->m_mapTMAP.end() )
		{
			CTMap *pMAP = (*itMAP).second;

			DWORD dwCell = MAKELONG(
				WORD(fPosX / CELL_SIZE),
				WORD(fPosZ / CELL_SIZE));

			MAPTCELL::iterator itCELL = pMAP->m_mapTCELL.find(dwCell);
			if( itCELL != pMAP->m_mapTCELL.end() && (*itCELL).second->IsMainCell(bChannel) )
				return TRUE;
		}
	}

	return FALSE;
}

#ifdef __HACK_SHIELD
void CTMapSvrModule::CheckHackShield(CTPlayer * pPlayer)
{
	if(!pPlayer->m_pMAP)
	{
		pPlayer->m_dwHSRecvTick = 0;
		return;
	}

	if(pPlayer->m_dwHSRecvTick &&
		m_dwTick - pPlayer->m_dwHSRecvTick > HACKSHIELD_RECVTICK)
	{
		LogHackShield(pPlayer->m_dwID, m_dwTick - pPlayer->m_dwHSRecvTick, _T("Timeout"));
		CloseSession(pPlayer);
		return;
	}

	if(m_dwTick - pPlayer->m_dwHSSendTick > HACKSHIELD_SENDTICK)
	{
		AHNHS_TRANS_BUFFER stRequestBuf;
		DWORD dwRet = _AhnHS_MakeRequest(pPlayer->m_hHackShield, &stRequestBuf);
		if(dwRet != ERROR_SUCCESS)
		{
			LogHackShield(pPlayer->m_dwID, dwRet, _T("_AhnHS_MakeRequest"));
			CloseSession(pPlayer);
			return;
		}

		pPlayer->m_dwHSSendTick = pPlayer->m_dwHSRecvTick = m_dwTick;
//		LogHackShield(pPlayer->m_dwID, m_bServerID, _T("Send ESSD"));

		CPacket * pMsg = new CPacket();
		pMsg->SetID(CS_HACKSHIELD_ACK)
			<< m_bServerID;

		pMsg->AttachBinary(stRequestBuf.byBuffer, stRequestBuf.nLength);
		pPlayer->Say(pMsg);
	}
}
void CTMapSvrModule::LogHackShield(DWORD dwCharID, DWORD dwError, CString strMessage)
{
	static CString strFileName = NAME_NULL;
	if(strFileName.IsEmpty())
	{
		TCHAR szFN[ONE_KBYTE];
		::lstrcpy(szFN, m_szHsbFilePath);
		char *pSlash = ::strrchr(szFN, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "HackShield.log");

		strFileName = szFN;
	}

	FILE * file;
	file = fopen(strFileName, "a+");
	if(!file)
		file = fopen(strFileName, "w+");

	if(!file)
		return;

	CString log;
	CTime time(m_timeCurrent);

	log.Format("%s	SERVER_ID=%d	CHAR_ID=%d	CODE=%x	WHERE=%s\n",time.Format("%c"), m_bServerID, dwCharID, dwError, strMessage);
	fwrite(log, sizeof(char), lstrlen(log), file);
	fclose(file);
}
#endif

#ifdef __N_PROTECT

void CTMapSvrModule::CheckCSNProtect(CTPlayer * pPlayer)
{
	if(!pPlayer)
		return;
	if(!pPlayer->m_csNProtect)
		return;
	if(!pPlayer->m_pMAP)
	{
		pPlayer->m_bSend = FALSE;
		return;
	}

	DWORD dwSendTick = m_dwTick - pPlayer->m_dwNPSendTime;

	if( pPlayer->m_bSend )
	{
		if(	dwSendTick > NPROTECT_RECVTICK)
		{
			CString strBuf;
			strBuf.Format("TimeOut	[Query:%08X %08X %08X %08X]", 
				pPlayer->m_csNProtect->m_AuthQuery.dwIndex, pPlayer->m_csNProtect->m_AuthQuery.dwValue1, pPlayer->m_csNProtect->m_AuthQuery.dwValue2, pPlayer->m_csNProtect->m_AuthQuery.dwValue3);			

			LogNProtect(pPlayer->m_dwID, dwSendTick, strBuf);
			CloseSession(pPlayer);
			return;
		}
	}
	else
	{
		if(dwSendTick > NPROTECT_SENDTICK)
		{
			DWORD dwRet = pPlayer->m_csNProtect->GetAuthQuery();
			if(dwRet != ERROR_SUCCESS)	
			{
				CString strBuf;
				strBuf.Format("Invalid Request	[Query:%08X %08X %08X %08X] [Answer:%08X %08X %08X %08X]", 
					pPlayer->m_csNProtect->m_AuthQuery.dwIndex, pPlayer->m_csNProtect->m_AuthQuery.dwValue1, pPlayer->m_csNProtect->m_AuthQuery.dwValue2, pPlayer->m_csNProtect->m_AuthQuery.dwValue3, 
					pPlayer->m_csNProtect->m_AuthAnswer.dwIndex, pPlayer->m_csNProtect->m_AuthAnswer.dwValue1, pPlayer->m_csNProtect->m_AuthAnswer.dwValue2, pPlayer->m_csNProtect->m_AuthAnswer.dwValue3);
				
				LogNProtect(pPlayer->m_dwID, dwRet, strBuf);
				CloseSession(pPlayer);
				return;
			}

			pPlayer->SendCS_NPROTECT_ACK(m_bServerID, m_dwTick);

#ifdef __N_PROTECT_DEBUG
			TCHAR msgBuf[ONE_KBYTE];
			DWORD dwret = ModuleInfo(msgBuf, ONE_KBYTE);

			if(NPGG_INFO_SUCCESS != dwret)
				LogNProtect(0, dwret, "ModuleInfo error");
			else
			{
				CString strBuf = _T("CS MODULE INFO ");
				strBuf += msgBuf;
				LogNProtect(0, dwret, strBuf );
			}
#endif
		}
	}
}
void CTMapSvrModule::LogNProtect(DWORD dwCharID, DWORD dwError, CString strMessage)
{
	static CString strFileName = NAME_NULL;
	if(strFileName.IsEmpty())
	{
		TCHAR szFN[ONE_KBYTE];

		GetModuleFileName(NULL, szFN, ONE_KBYTE);
		char *pSlash = ::strrchr(szFN, '\\');
		if( pSlash )
			::lstrcpy(pSlash + 1, "\\nProtect\\LognProtect.log");

		strFileName = szFN;
	}

	FILE * file;
	file = fopen(strFileName, "a+");
	if(!file)
		file = fopen(strFileName, "w+");

	if(!file)
		return;

	CString log;
	CTime time(m_timeCurrent);

	if(dwCharID)
		log.Format("[%s][SERVER_ID=%2d][CODE=%4x][CHAR_ID=%8d] %s\n", time.Format("%c"), m_bServerID, dwError, dwCharID, strMessage);	
	else
		log.Format("[%s][SERVER_ID=%2d][CODE=%4x] %s\n", time.Format("%c"), m_bServerID, dwError, strMessage);	

	fwrite(log, sizeof(char), lstrlen(log), file);
	fclose(file);
}
void CTMapSvrModule::UpdateCSModule()
{	
	if(!m_bEnableNP)
		return;

	if(m_dwTick - m_dwNPCSModuleTick > NPROTECT_CS_UPDATETICK)	//	CS모듈 업데이트 시간
	{
		GGAuthUpdateTimer();
		m_dwNPCSModuleTick = m_dwTick;
	}
}
#endif

#ifdef DEF_UDPLOG
void CTMapSvrModule::TournamentResultLog(DWORD dwWinner, DWORD dwLoser, BYTE bStep)
{
	LPTOURNAMENTPLAYER pWin = FindTournamentPlayer(dwWinner);
	LPTOURNAMENTPLAYER pLos = FindTournamentPlayer(dwLoser);
	if(pWin)
	{
		LPTOURNAMENT tEntry = FindTournament(pWin->m_bEntryID);
		CString strEntryName = _T("");
		if(tEntry)
			strEntryName = tEntry->m_strName ;

		if(bStep == 1)
			m_pUdpSocket->LogTournamentWin(LOGMAP_TOURNAMENTWIN, pWin, pLos, strEntryName, pWin->m_dwChiefID, bStep);
		else
			m_pUdpSocket->LogTournamentWin(LOGMAP_TOURNAMENTWIN, pWin, pLos, strEntryName, NULL, bStep);
	}
}
#endif	//	DEF_UDPLOG