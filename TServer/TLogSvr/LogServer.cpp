////////////////////////////////////////////////////////////////////////////////////////////////////////
//	File Name	:	LogServer.cpp
//	Copyright	:	(c) Zemi Interactive, Inc 2002-2009
////////////////////////////////////////////////////////////////////////////////////////////////////////


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f LogServerps.mk in the project directory.
#pragma once


#include "stdafx.h"
#include "resource.h"
#include "LogServer.h"

#include "LogServer_i.c"
#include "CUdpSocket.h"


#include "RegCrypt.h"
#include "XRegKey.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Debug Global Value & Function
////////////////////////////////////////////////////////////////////////////////////////////////////////
HWND		g_MainHwnd;
HINSTANCE	g_hInstance;

BOOL		CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
//	Regestry Global Value
/////////////////////////////////////////////////////////////////////////////

CUdpSocket* lpUdpSock ;


CServiceModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

/*
 *
 */
CServiceModule::CServiceModule()
{	

}	

CServiceModule::~CServiceModule()
{	

}	



//
//
//
BOOL CServiceModule::Run_Server()
{
	//
	//	Read Registry
	//
	if( !ConfigLoad() )
	{
		LogFileWrite("(X) *** LoadConfig Errorr ***");
		return FALSE;
	}

	LogFileWrite( "IP=%s", _Module.m_szLogSvrIPAddr );
	LogFileWrite( "Port=%d", _Module.m_nLogSvrPort );

	LogFileWrite( "DBIP=%s", _Module.m_szLogDBIPAddr );
	LogFileWrite( "DBName=%s", _Module.m_szLogDBName );

	LogFileWrite( "DBID=%s", _Module.m_szDBID );
	LogFileWrite( "DBPWD=%s", _Module.m_szDBPWD );

	//
	//	Log Server Initialization
	//
	lpUdpSock = new CUdpSocket;

	if( !lpUdpSock->Initialize(m_szLogSvrIPAddr, m_nLogSvrPort) )
	{
		LogFileWrite("(X) Log Server bInit Error");
		return FALSE;
	}

	//
	//	Log Server Starting
	//
	LogFileWrite("************************************************************");
	LogFileWrite("* 4S Log Server Starting                                   *");
	LogFileWrite("*                                                          *");
	LogFileWrite("*                                                          *");
	LogFileWrite("*                CopyRight (C) Zemi Interactive 2007-2008  *");
	LogFileWrite("************************************************************");

	// Dump
	CTMiniDump::SetOption(MiniDumpWithFullMemory);

	return TRUE;
}

/*
 *	MainDlgProc
 */
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	//
	//	Debug Window
	//
	switch(iMessage)
	{
	case WM_INITDIALOG:	
		{
			if( !_Module.Run_Server() )
			{
				MessageBox( NULL, "Log Server Server Run_Server error", "Log Server", MB_OK );
				
				EndDialog( hDlg, 0);
				return FALSE;		
			}
		
			SetDlgItemText( hDlg,  IDC_COUNT,_T("0") );
			g_MainHwnd	=	hDlg;

		}
		break;

	case WM_COMMAND:

		switch ( LOWORD(wParam) )
		{
		case ID_EXIT:	
			{
				EndDialog( hDlg, 0);
				return TRUE;
			}
			break;


		}
		return FALSE;


	//	Clean Memory
	case WM_DESTROY:
		{
			delete lpUdpSock;
						
			EndDialog( hDlg, 0);
			return TRUE;
		}

		break;
	}

	return FALSE;
}



// Although some of these functions are big they are declared inline since they are only used once

inline HRESULT CServiceModule::RegisterServer(BOOL bRegTypeLib, BOOL bService)
{
   HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove any previous service since it may point to
    // the incorrect file
    Uninstall();

    // Add service entries
     UpdateRegistryFromResource(IDR_LogServer, TRUE);
 
    if (bService)
    {
        // Create service
        Install();
    }

    // Add object entries
    hr = CComModule::RegisterServer(bRegTypeLib);

    CoUninitialize();
    return hr;
}

inline HRESULT CServiceModule::UnregisterServer()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove service entries
    UpdateRegistryFromResource(IDR_LogServer, FALSE);
    // Remove service
    Uninstall();
    // Remove object entries
    CComModule::UnregisterServer(TRUE);
    CoUninitialize();
    return S_OK;
}

inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid)
{
    CComModule::Init(p, h, plibid);

    m_bService = TRUE;

    LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
}

LONG CServiceModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0 && !m_bService)
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
    return l;
}

BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

inline BOOL CServiceModule::Install()
{
    if (IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
        hSCM, m_szServiceName, m_szServiceName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		// 인스톨시 자동 시작으로 등록된다. SERVICE_DEMAND_START
        SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

inline BOOL CServiceModule::Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
void CServiceModule::LogEvent(LPCTSTR pFormat, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, pFormat);
    _vstprintf(chMsg, pFormat, pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;

    if (m_bService)
    {
        /* Get a handle to use with ReportEvent(). */
        hEventSource = RegisterEventSource(NULL, m_szServiceName);
        if (hEventSource != NULL)
        {
            /* Write to event log. */
            ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
            DeregisterEventSource(hEventSource);
        }
    }
    else
    {
        // As we are not running as a service, just write the error to the console.
        _putts(chMsg);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
    SERVICE_TABLE_ENTRY st[] =
    {
        { m_szServiceName, _ServiceMain },
        { NULL, NULL }
    };
    if (m_bService && !::StartServiceCtrlDispatcher(st))
    {
        m_bService = FALSE;
    }
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
    if (m_hServiceStatus == NULL)
    {
        LogEvent(_T("Handler not installed"));
        return;
    }
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);
    LogEvent(_T("Service stopped"));
}

inline void CServiceModule::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
        SetServiceStatus(SERVICE_STOP_PENDING);
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
        break;

    case SERVICE_CONTROL_PAUSE:
        break;

    case SERVICE_CONTROL_CONTINUE:
        break;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    case SERVICE_CONTROL_SHUTDOWN:
        break;

    default:
        LogEvent(_T("Bad service request"));
    }
}

void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    _Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
    _Module.Handler(dwOpcode); 
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Run()
{
    _Module.dwThreadID = GetCurrentThreadId();

    HRESULT hr = CoInitialize(NULL);
//  If you are running on NT 4.0 or higher you can use the following call
//  instead to make the EXE free threaded.
//  This means that calls come in on a random RPC thread
//  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _ASSERTE(SUCCEEDED(hr));

    // This provides a NULL DACL which will allow access to everyone.
    CSecurityDescriptor sd;
    sd.InitializeFromThreadToken();
    hr = CoInitializeSecurity(sd, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    _ASSERTE(SUCCEEDED(hr));

    hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE);
    _ASSERTE(SUCCEEDED(hr));

    LogEvent(_T("Log Server started"));
    if (m_bService)
        SetServiceStatus(SERVICE_RUNNING);


	if( !_Module.Run_Server()  )
	{
		LogEvent(_T("Log Server Start Error"));		
		return;
	}


    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
        DispatchMessage(&msg);

	delete lpUdpSock;

    _Module.RevokeClassObjects();

    CoUninitialize();
}



/*
 *	Winsocekt 사용 초기화 	
 */
BOOL bWinsockInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 ) 
		return FALSE;

	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return FALSE ; 
	}
	return TRUE ;
}

/*
 *	ReadRegistry	
 *
 */
BOOL CServiceModule::ConfigLoad()
{	
	//	Create Log Folder	
	CString strLogFolder;
	strLogFolder.Format("%s%s%s",_Module.m_szDrv, _Module.m_szPath, _Module.m_szFname);
	CreateDirectory(strLogFolder, NULL);

	DWORD dwLength;
	DWORD dwValue;
	HKEY hKey;

	CString strRegKey;
	strRegKey.Empty();

	HKEY hOpenKey = HKEY_LOCAL_MACHINE;
	strRegKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\%s\\Config"), m_szFname);


	int nERROR = RegCreateKey( hOpenKey, strRegKey, &hKey);
	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error");
		return FALSE;
	}

	//	Log Server IP
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(hKey,  _T("IP"),  NULL,  NULL,  (LPBYTE) &m_szLogSvrIPAddr, &dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error [IP]" );
		return FALSE;
	}

	//	Log Server Port
	dwLength = sizeof(DWORD);
	nERROR = RegQueryValueEx(hKey, _T("PORT"), NULL, NULL, (LPBYTE) &dwValue, &dwLength);
	
	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error[PORT]");
		return FALSE;
	}

	m_nLogSvrPort =  dwValue;



	//	Log Server DB IP Address
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(hKey,  _T("LOGDBIP"),  NULL,  NULL,  (LPBYTE) &m_szLogDBIPAddr, &dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error [LOGDBIP]" );
		return FALSE;
	}


	//	Log Server DB Name
	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(hKey,  _T("LOGDBNAME"),  NULL,  NULL,  (LPBYTE) &m_szLogDBName, &dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
			LogFileWrite( "(X) Log Server Registry Error [LOGDBNAME]");
		return FALSE;
	}

	//
	//	Get	Log DB ID * Password
	//
	char szRegName[ONE_KBYTE];
	char szBuffer[ONE_KBYTE];

	lstrcpy(szRegName, strRegKey );

#ifdef _REGCRYPT

	CRegCrypt crypt;

	memset(szBuffer, 0x00, sizeof(szBuffer ) );
	int nLen = sizeof(szBuffer);
	crypt.RegDecrypt(szRegName, "LOGDBID", szBuffer, &nLen, ENCDLG_KEY);

	if(szBuffer[0] != '\0')
	{
		strcpy(_Module.m_szDBID, szBuffer);
	}
	else
	{
		LogFileWrite( "(X) Log Server Registry Error [LOGDBID]");
		return FALSE;
	}


	memset(szBuffer, 0x00, sizeof(szBuffer));
	nLen = sizeof(szBuffer);
	crypt.RegDecrypt(szRegName, "LOGDBPWD", szBuffer, &nLen, ENCDLG_KEY);

	if(szBuffer[0] != '\0')
	{
		lstrcpy(_Module.m_szDBPWD, szBuffer);
	}
	else
	{
		LogFileWrite( "(X) Log Server Registry Error [LOGDBPWD]");
		return FALSE;
	}

#else

	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(hKey,  _T("LOGDBID"),  NULL,  NULL,  (LPBYTE) &m_szDBID, &dwLength);

	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error [LOGDBID]");
		return FALSE;
	}

	dwLength = ONE_KBYTE;
	nERROR = RegQueryValueEx(hKey,  _T("LOGDBPWD"),  NULL,  NULL,  (LPBYTE) &m_szDBPWD, &dwLength);
	if( nERROR != ERROR_SUCCESS )
	{
		LogFileWrite( "(X) Log Server Registry Error [LOGDBNAME]");
		return FALSE;
	}

#endif

	return TRUE ;
}


/*
 *	Write Registery
 *	----------------------------------------------------------------
 *		IP
 *		PORT
 *
 *		LOGDBIP
 *		LOGDBNAME
 *		LOGDB ID PASSWORD
 */
BOOL CServiceModule::ConfigWrite(LPCTSTR lpszToken)
{	
	char strRegName[512]="";

	sprintf( strRegName, "SYSTEM\\CurrentControlSet\\Services\\%s\\Config", m_szFname );

	char *delim	= " /";
	char *token	    = strtok((char *)(LPCTSTR)lpszToken, delim);

	if ( token )
	{
		//
		//	LOG DB ID / PWD
		//
		if ( !strcmpi( token, _T( "-LOGDB" ) ) )
		{

			token = strtok( NULL, delim );
			if ( token )
			{
#ifdef _REGCRYPT
				CRegCrypt crypt;

				char* next = strtok( NULL, delim );
				if ( next )
				{
					if(!crypt.RegEncrypt(strRegName, "LOGDBPWD", next, strlen(next), ENCDLG_KEY))
					{
						LogFileWrite("(X) Registry Write LOG DB PWD Error");
						return false;
					}					
				}				

				if(!crypt.RegEncrypt(strRegName, "LOGDBID", token, strlen(next) - strlen(token), ENCDLG_KEY))
				{
					LogFileWrite("(X) Registry Write LOG DB ID Error");
					return false;
				}
#else

				CXRegKey keySetting;
				LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);
				if (lRes != ERROR_SUCCESS)					
					LogFileWrite("(X) Registry Key Open Error");
				lRes = keySetting.SetValue( token, _T("LOGDBID") );
				if (lRes != ERROR_SUCCESS)					
					LogFileWrite("(X) Registry Write LOG DB ID Error");
				keySetting.Close();
				char* next = strtok( NULL, delim );
				if(next)
				{
					CXRegKey keySetting;
					LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);
					if (lRes != ERROR_SUCCESS)					
						LogFileWrite("(X) Registry Key Open Error");
					lRes = keySetting.SetValue( next, _T("LOGDBPWD") );
					if (lRes != ERROR_SUCCESS)					
						LogFileWrite("(X) Registry Write LOG DB PW Error");
					keySetting.Close();
				}
#endif						
			}

			return true;
		}


		//	IP 
		if ( !strcmpi( token, _T( "-IP" ) ) )
		{
			token = strtok( NULL, delim );
			if ( token )
			{
				CXRegKey keySetting;

				LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);

				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Key Open Error");
				}						

				lRes = keySetting.SetValue( token, _T("IP") );
				//	lRes = keySetting.SetMultiStringValue( token, _T("IP") );
				
				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Write Error - IP" );
				}

				keySetting.Close();
			}

			return true;
		}

		//	PORT
		if ( !strcmpi( token, _T( "-PORT" ) ) )
		{
			token = strtok( NULL, delim );
			if ( token )
			{
				CXRegKey keySetting;

				LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);

				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Key Open Error");
				}						

				lRes = keySetting.SetValue( atoi(token), _T("PORT") );
				//	lRes = keySetting.SetDWORDValue( _T("PORT"), atoi(token)  );
				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Write Error - PORT" );
					}

				keySetting.Close();
			}

			return true;
		}

		//
		//	LOGDBIP 
		//
		if ( !strcmpi( token, _T( "-LOGDBIP" ) ) )
		{
			token = strtok( NULL, delim );
			if ( token )
			{
				CXRegKey keySetting;

				LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);

				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Key Open Error");
				}						

				lRes = keySetting.SetValue( token, _T("LOGDBIP") );
				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Write Error - LOGDBIP" );
				}

				keySetting.Close();
			}

			return true;
		}
		
		//	LOGDBNAME 
		if ( !strcmpi( token, _T( "-LOGDBNAME" ) ) )
		{
			token = strtok( NULL, delim );
			if ( token )
			{
				CXRegKey keySetting;

				LONG lRes = keySetting.Open(HKEY_LOCAL_MACHINE, strRegName, KEY_READ | KEY_WRITE);

				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry key open error");				
				}						

				lRes = keySetting.SetValue( token, _T("LOGDBNAME") );
				if (lRes != ERROR_SUCCESS)
				{
					LogFileWrite("(X) Registry Write Error - LOGDBNAME");
				}

				keySetting.Close();
			}

			return true;
		}
	}

	return false;
}




/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
  	g_hInstance = hInstance;

	if (bWinsockInit() == FALSE) return 0;

	//
	//	Get Execute Infomation
	//
	GetModuleFileName(NULL, _Module.m_szFilename, sizeof(_Module.m_szFilename) );
     _splitpath( _Module.m_szFilename, _Module.m_szDrv, _Module.m_szPath, _Module.m_szFname, _Module.m_szExt );

	 
	//
	//	Run Parameter Operation
	//
    lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT


	//	파라미터로 입력된 값을 얻음
    TCHAR szTokens[] = _T(" ");
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);

	//	Debug Mode
#if _DEBUG	

	if( strlen(lpszToken) != 0  )
	 {
		_Module.ConfigWrite(lpszToken);
		return 0;
	 }

	_Module.m_bService = TRUE;

	DialogBox( hInstance , MAKEINTRESOURCE( IDD_DIALOG_CONTROL ), HWND_DESKTOP, MainDlgProc);

	return 0;

	//
	//	Release Mode
	//
#else

    _Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, &LIBID_LogServerLib);
    _Module.m_bService = TRUE;
	

	//	파라미터 입력이 된경우만 처리 
	if( lpszToken != NULL  )
	{

		//	Service Install
		if(	!strcmpi( lpszToken , _T( "-I") ) )
		{
			strcpy( _Module.m_szServiceName, _Module.m_szFname );

			// Register as Service
			return _Module.RegisterServer(TRUE, TRUE);
		}

		//	Service UnInstall
		else if(	!strcmpi( lpszToken , _T( "-U") ) )
		{
			strcpy( _Module.m_szServiceName, _Module.m_szFname );

			return _Module.UnregisterServer();
		}
		else
		{
			_Module.ConfigWrite( lpszToken  );

			return 0;
		}
    } 

    _Module.Start();

    // When we get here, the service has been stopped
    return _Module.m_status.dwWin32ExitCode;
#endif
}

/*
 *	LogFileWrite
 *
 */
void LogFileWrite(const char *pszParam, ...)
{
	char buf[ONE_KBYTE];

	va_list va; 
	va_start(va, pszParam); 
		vsprintf(buf, pszParam, (va_list)va); 
	va_end(va);


#ifdef _DEBUG	
	MessagePrint(buf) ;
#endif


	CTime nowTime = CTime::GetCurrentTime(); 

	char strFile[MAX_PATH];

	sprintf( strFile, "%s%s%s%\\%04d%02d%02d.log", _Module.m_szDrv, _Module.m_szPath, _Module.m_szFname, nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay() );

	FILE* f = fopen( strFile ,"a+t");
	if( !f ) 	return;

		fprintf(f,"%02d:%02d:%02d %s\r\n",	nowTime.GetHour(), nowTime.GetMinute(), nowTime.GetSecond(), buf);

	fclose(f);
}


void MessagePrint(const char *pszParam, ...)
{

#ifdef _DEBUG 
	char buf[1024];

	va_list va; 
	va_start(va, pszParam); 
	vsprintf(buf, pszParam, (va_list)va); 
	va_end(va);

	SendDlgItemMessage( g_MainHwnd,  IDC_MSG, LB_INSERTSTRING, 0,  (LPARAM)buf );
#endif

}

/*
 *	Connected Count
 */
void MessageCount( CString pMessage )
{
	SetDlgItemText( g_MainHwnd,  IDC_COUNT,pMessage );
}

