#pragma once


#define ON_RECEIVE(p)							case p : return On##p( pDB, pGROUP, pUser, packet);

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


class CTLoginSvrModule : public CAtlServiceModuleT< CTLoginSvrModule, IDS_SERVICENAME >
{
protected:
	MAPTUSER m_mapTSESSION;							// all session pool
	MAPTUSER m_mapTUSER;							// valid session pool
	MAPDWORD m_mapACTIVEUSER;						
	MAPDWORD m_mapCurrentUser;
	INT64 m_dlCheckFile;
	HANDLE m_hExecFile;

	CRITICAL_SECTION m_csLI;

protected:
	TCHAR m_szDBUserID[ONE_KBYTE];
	TCHAR m_szDBPasswd[ONE_KBYTE];
	TCHAR m_szDSN[ONE_KBYTE];

	BYTE  m_bServerID;
	WORD  m_wPort;
	BYTE  m_bNation;

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;

	SOCKADDR_IN m_addrCtrl;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;
	
	MAPEVENTINFO m_mapEVENT;

#ifdef DEF_UDPLOG
	TCHAR m_szLogServerIP[ONE_KBYTE];
	WORD  m_wLogServerPORT;

	CUdpSocket *m_pUdpSocket;
#endif

protected:

	LPTGROUP FindGroup(LPMAPTGROUP pGROUP, BYTE bGroupID);
	DWORD GetCurrentUser(BYTE bGroup);

	void OnSendComplete(CTUser *pUser, DWORD dwIoBytes);

	void ProcessSession(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, DWORD dwIoBytes);

	void OnInvalidSession( CTUser *pUser);				// 사용 금지
	void OnCloseSession( CTUser *pUser);				// 사용 금지
	void ClosingSession( CTUser *pUser);				// 사용 금지
	void CloseSession( CTUser *pUser);					// 세션을 종료하려면 이 함수를 호출

	void ClearThread();
	void UpdateData();

	DWORD InitDB(CSqlDatabase *pDB);
	DWORD LoadData(CSqlDatabase *pDB, LPMAPTGROUP pGROUP);
	DWORD LoadDataLogin();

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD InitNetwork();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

	DWORD ControlThread();
	DWORD WorkThread();

	DWORD GetCheckFilePoint(CTUser * pUser);
	BYTE CheckFile(CTUser * pUser, INT64 dlValue);
	void LogExecCheck(DWORD dwUserID, DWORD dwError, CString strAccount = NAME_NULL);

protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);

protected:
	// 시스템 메세지 핸들러
	DWORD OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

protected:
	// Control Server Message
	DWORD OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_SERVICEDATACLEAR_ACK(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_CTRLSVR_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_EVENTUPDATE_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCT_EVENTMSG_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

	// CS message handler
	DWORD OnCS_CHANNELLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_GROUPLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_CHARLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_LOGIN_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	// 현승룡 CS_TESTLOGIN_REQ
	DWORD OnCS_TESTLOGIN_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	// 현승룡 CS_TESTVERSION_REQ
	DWORD OnCS_TESTVERSION_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);

	DWORD OnCS_CREATECHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_DELCHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_START_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_AGREEMENT_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet);
	DWORD OnCS_TERMINATE_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet);
	DWORD OnCS_HOTSEND_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet);

public:
	CTLoginSvrModule();
	virtual ~CTLoginSvrModule();
	BYTE CheckCharName(LPCSTR str);
	void ClearLoginUser();

public:
	DECLARE_LIBID(LIBID_TLoginSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TLOGINSVR, "{A9C0CF53-7D54-43D7-B01C-D604FB9DF809}")
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


extern CTLoginSvrModule _AtlModule;
