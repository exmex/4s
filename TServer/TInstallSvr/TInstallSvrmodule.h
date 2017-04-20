#pragma once

#define ON_RECEIVE(p)							case p : return On##p( pDB, pSession);
#define CT_INSATLLEND_REQ						CT_INSATLLSTART_REQ
////////////////////// 매우매우 중요 - 소켓 종료요령 /////////////////////////////////////
//
// 1. 자기자신을 종료 하려면 핸들러함수에서 EC_SESSION_INVALIDCHAR를 리턴한다.
//    (핸들러 함수의 파라메터로 넘어온 소켓 pUser가 종료)
//
// 2. 다른 소켓을 종료 하려면 종료대상을 파라메터로 하여 CloseSession()을 호출한다.
//    (예 : CloseSession(pTarget); pTarget이 스스로 종료하도록 유도된다)
//
// 3. 이 이외의 방법으로 절대 소켓을 종료하면 안된다.
//
//////////////////////////////////////////////////////////////////////////////////////////

class CTInstallSvrModule : public CAtlServiceModuleT< CTInstallSvrModule, IDS_SERVICENAME >
{
protected:
	MAPTSESSION m_mapTSESSION;							// all session pool
	CRITICAL_SECTION m_csPC;
	CRITICAL_SECTION m_csCon;

protected:
	TCHAR m_szDBUserID[ONE_KBYTE];
	TCHAR m_szDBPasswd[ONE_KBYTE];
	TCHAR m_szDSN[ONE_KBYTE];

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;
	WORD m_wPort;

	CString m_strFTP;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;
	HANDLE m_hCheck;

	HANDLE m_hIocpCheck;
	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	COverlappedEx m_ovConnect;
	CPacket m_vAccept;

	map<DWORD, CISession*> m_mapConnect;

protected:
	void OnSendComplete( CTInstallSession *pSession, DWORD dwIoBytes);
	void ProcessSession( CSqlDatabase *pDB, CTInstallSession * pSession, DWORD dwIoBytes);

	void OnInvalidSession( CTInstallSession * pSession);			// 사용 금지
	void OnCloseSession( CTInstallSession * pSession);				// 사용 금지
	void ClosingSession( CTInstallSession * pSession);				// 사용 금지
	void CloseSession( CTInstallSession * pSession);				// 세션을 종료하려면 이 함수를 호출

	void DeleteIP( CSqlDatabase *pDB,  DWORD dwIPAddr);

	void ClearThread();
	void UpdateData();

	DWORD InitDB( CSqlDatabase *pDB);
	DWORD LoadData(CSqlDatabase *pDB);

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD InitNetwork();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive( CSqlDatabase *pDB, CTInstallSession * pSession);

	DWORD ControlThread();
	DWORD CheckThread();
	DWORD WorkThread();

	BYTE ConnectCheck(DWORD dwIpAddr, WORD wPort);
	BYTE FindSession(DWORD dwIpAddr, BYTE bDel = FALSE);

protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);
	static DWORD WINAPI _CheckThread( LPVOID lpParam);

protected:
	// 시스템 메세지 핸들러
	DWORD OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, CTInstallSession * pSession);

	DWORD OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, CTInstallSession * pSession );
	DWORD OnCT_SERVICEDATACLEAR_ACK( CSqlDatabase *pDB, CTInstallSession * pSession){return EC_NOERROR;}
	DWORD OnCT_CTRLSVR_REQ( CSqlDatabase *pDB, CTInstallSession * pSession){return EC_NOERROR;}
	DWORD OnCT_INSTALL_REQ( CSqlDatabase *pDB, CTInstallSession * pSession);
	DWORD OnCT_INSTALLEND_REQ( CSqlDatabase *pDB, CTInstallSession * pSession);
	DWORD OnCT_FILEADDR_REQ( CSqlDatabase *pDB, CTInstallSession * pSession);

public:
	CTInstallSvrModule();
	virtual ~CTInstallSvrModule();

public:
	DECLARE_LIBID(LIBID_TInstallSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TINSTALLSVR, "{A06F49B5-675E-4824-9653-688BC0B00293}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : CoInitializeSecurity를 호출하고 서비스에 
		// 올바른 보안 설정을
		// 적용하십시오. PKT 수준 인증, 
		// RPC_C_IMP_LEVEL_IDENTIFY 가장 수준 인증 
		// 및 Null이 아닌 적절한 보안 설명자 등을 적용하면 됩니다.

		return S_OK;
	}
	HRESULT RegisterAppId( bool bService = false) throw()
	{
		if(!Uninstall())
			return E_FAIL;

		HRESULT hr = UpdateRegistryAppId(TRUE);
		if(FAILED(hr))
			return hr;

		CRegKey keyAppID;
		LONG lRes = keyAppID.Open( HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
		if( lRes != ERROR_SUCCESS )
			return AtlHresultFromWin32(lRes);

		CRegKey key;
		lRes = key.Create( keyAppID, GetAppIdT());

		if( lRes != ERROR_SUCCESS )
			return AtlHresultFromWin32(lRes);

		key.DeleteValue(_T("LocalService"));
		if(!bService)
			return S_OK;
		key.SetStringValue(_T("LocalService"), m_szServiceName);

		// Create service
		if(!Install())
			return E_FAIL;

		return S_OK;
	}
	bool ParseCommandLine( LPCTSTR lpCmdLine, HRESULT* pnRetCode) throw()
	{
		TCHAR szTokens[] = _T("-/");
		*pnRetCode = S_OK;

		LPCTSTR lpszToken = FindOneOf( lpCmdLine, szTokens);
		while(lpszToken)
		{
			if( WordCmpI( lpszToken, _T("UnregServer")) == 0 )
			{
				*pnRetCode = UnregisterServer(TRUE);

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = UnregisterAppId();

				return false;
			}

			// Register as Local Server
			if( WordCmpI( lpszToken, _T("RegServer")) == 0 )
			{
				*pnRetCode = RegisterAppId();

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = RegisterServer(TRUE);

				return false;
			}

			if( WordCmpI( lpszToken, _T("Service")) == 0 ||
				WordCmpI( lpszToken, _T("Install")) == 0 )
			{
				*pnRetCode = RegisterAppId(true);

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = RegisterServer(TRUE);

				return false;
			}

			if( WordCmpI( lpszToken, _T("UnInstall")) == 0 )
			{
				*pnRetCode = S_OK;
				if(!Uninstall())
					*pnRetCode = E_FAIL;

				return false;
			}

			if( WordCmpI( lpszToken, _T("Name")) == 0 )
			{
				CString strNAME(FindOneOf( lpszToken, _T(" ")));
				int nPOS = 0;

				strNAME = strNAME.Tokenize( _T(" "), nPOS);
				strcpy( m_szServiceName, LPCTSTR(strNAME));
			}

			lpszToken = FindOneOf( lpszToken, szTokens);
		}

		return true;
	}
	BOOL Install() throw()
	{
		if(IsInstalled())
			return TRUE;

		// Get the executable file path
		TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
		DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 1, MAX_PATH);
		if( dwFLen == 0 || dwFLen == MAX_PATH )
			return FALSE;

		// Quote the FilePath before calling CreateService
		szFilePath[0] = _T('\"');
		szFilePath[dwFLen + 1] = _T('\"');
		szFilePath[dwFLen + 2] = 0;

		CString strFilePath;
		strFilePath.Format( "%s -Name %s", szFilePath, m_szServiceName);

		SC_HANDLE hSCM = ::OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if( hSCM == NULL )
		{
			TCHAR szBuf[1024];

			if( AtlLoadString( ATL_SERVICE_MANAGER_OPEN_ERROR, szBuf, 1024) == 0 )
				lstrcpy( szBuf,  _T("Could not open Service Manager"));
			MessageBox( NULL, szBuf, m_szServiceName, MB_OK);

			return FALSE;
		}

		SC_HANDLE hService = ::CreateService(
			hSCM,
			m_szServiceName,
			m_szServiceName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			LPCSTR(strFilePath),
			NULL, NULL,
			_T("RPCSS\0"),
			NULL, NULL);

		if( hService == NULL )
		{
			::CloseServiceHandle(hSCM);
			TCHAR szBuf[1024];

			if( AtlLoadString( ATL_SERVICE_START_ERROR, szBuf, 1024) == 0 )
				lstrcpy(szBuf,  _T("Could not start service"));
			MessageBox( NULL, szBuf, m_szServiceName, MB_OK);

			return FALSE;
		}

		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);

		return TRUE;
	}
	HRESULT PreMessageLoop( int nShowCmd);
	HRESULT PostMessageLoop();
};

extern CTInstallSvrModule _AtlModule;
