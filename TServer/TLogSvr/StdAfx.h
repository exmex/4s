// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently
#pragma once


#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#define _ATL_APARTMENT_THREADED

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT

#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <winbase.h>

#include <process.h>
#include <winsock2.h>
#include <afxdb.h>			// MFC ODBC database classes
#include <atlbase.h>
#include <assert.h>
#include <math.h>
#include <Mmsystem.h>

#include "TMiniDump.h"
//	STL	Include Header 
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <ctime>
#include <string>
#include <map>	

using namespace std;
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

// #define _REGCRYPT

#include "../../TProtocol/LogPacket.h"


#define ONE_KBYTE			(1024)
#define	ENCDLG_KEY			"zemi_story"


#define	DEF_LISTHENTHREAD	1
// #define	MAX_IO_CONTEXT		1000000
 #define	MAX_IO_CONTEXT		1000

#define	SQLQUERY_DGINSERT		"INSERT INTO ITEMLOGTL%04d%02d%02d( LT_LOGDATE, LT_SERVERID, LT_CLIENTIP, LT_ACTION, LT_MAPID, LT_X, LT_Y, LT_Z, LT_DWKEY1, LT_DWKEY2, LT_DWKEY3, LT_DWKEY4, LT_DWKEY5, LT_DWKEY6, LT_DWKEY7, LT_DWKEY8, LT_DWKEY9, LT_DWKEY10, LT_DWKEY11,LT_KEY1, LT_KEY2, LT_KEY3, LT_KEY4, LT_KEY5, LT_KEY6, LT_KEY7, LT_FMT, LT_LOG) VALUES('%s', %u,'%s', %u, %d, %d, %d, %d, %I64d, %I64d, %I64d, %I64d, %I64d, %I64d, %I64d,%I64d,%I64d,%I64d,%I64d,'%s','%s','%s','%s','%s','%s','%s', %d, ?)"
#define DATETIME				"%d-%d-%d %d:%d:%d"

void  LogFileWrite(const char *pszParam, ...);

void  MessagePrint(const char *pszParam, ...);
void  MessageCount( CString pMessage );


class CServiceModule : public CComModule
{
public:
	CServiceModule();
	~CServiceModule();
	
// data members
public:
    TCHAR m_szServiceName[256];
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;
	DWORD dwThreadID;
	BOOL m_bService;

	char				m_szLogSvrIPAddr[20];		//	Log Server
	unsigned long	m_nLogSvrPort;
	
	char				m_szLogDBIPAddr[20];			//	Log DB
	char				m_szLogDBName[100];

	char				m_szDBID[64];	
	char				m_szDBPWD[128];
	
	
	//
	//	Server File Information
	//	
	TCHAR	m_szFilename[MAX_PATH];
	TCHAR	m_szDrv		[MAX_PATH];
	TCHAR	m_szPath	[MAX_PATH];
	TCHAR	m_szFname	[MAX_PATH];
	TCHAR	m_szExt		[MAX_PATH];

//Implementation
private:
	static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);

public:
	HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService);
	HRESULT UnregisterServer();

	void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, const GUID* plibid = NULL);
    void Start();
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void Handler(DWORD dwOpcode);
    void Run();

    BOOL IsInstalled();

    BOOL Install();
    BOOL Uninstall();
	LONG Unlock();

	void LogEvent(LPCTSTR pszFormat, ...);
    void SetServiceStatus(DWORD dwState);
    void SetupAsLocalServer();

	BOOL ConfigWrite(LPCTSTR lpszToken);
	BOOL ConfigLoad();

	BOOL Run_Server() ;
};

extern CServiceModule _Module;

#include <atlcom.h>

