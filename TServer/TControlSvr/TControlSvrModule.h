#pragma once

#define ON_RECEIVE(p)							case p : return On##p(pBUF);

////////////////////// 매우매우 중요 - 소켓 종료요령 /////////////////////////////////////
//
// 1. 자기자신을 종료 하려면 핸들러함수에서 EC_SESSION_INVALIDCHAR를 리턴한다.
//    (핸들러 함수의 파라메터로 넘어온 소켓 pBUF->m_pSESSION이 종료)
//
// 2. 다른 소켓을 종료 하려면 종료대상을 파라메터로 하여 CloseSession()을 호출한다.
//    (예 : CloseSession(pTarget); pTarget이 스스로 종료하도록 유도된다)
//
// 3. 이 이외의 방법으로 절대 소켓을 종료하면 안된다.
//
//////////////////////////////////////////////////////////////////////////////////////////

class CTControlSvrModule : public CAtlServiceModuleT< CTControlSvrModule, IDS_SERVICENAME >
{
protected:
	QPACKETBUF m_qBATCHJOB;					// queue for batch job
	QPACKETBUF m_qSMJOB;					// queue for SM job	

	MAPTSVRTEMP m_mapTSVRTEMP;				// Server template data
	MAPTMANAGER m_mapSESSION;				// all session pool
	MAPTMANAGER m_mapMANAGER;				// valid manager pool
	//MAPTMANAGERTEMP m_mapMANTEMP;			// 현승룡 매니저 권한
	MAPPLATFORM m_mapPLATFORM;				// 현승룡 Performance

	MAPTMACHINE m_mapTMachine;
	MAPTGROUP m_mapTGroup;
	MAPTSVRTYPE m_mapTSvrType;

	MAPBANINFO	m_mapBanInfo;
	MAPEVENTINFO m_mapEVENT;

	CRITICAL_SECTION m_csSMQUEUE;			// sync object for SM queue
	CRITICAL_SECTION m_csQUEUE;				// sync object for batch queue
	CRITICAL_SECTION m_csBATCH;				// sync object for batch thread

	CString m_strUploadFileName;
	CString m_strUploadFilePath;
	CString m_strMyAddr;
	FILE * m_fileTemp;

protected:
	TCHAR m_szDBUserID[ONE_KBYTE];
	TCHAR m_szDBPasswd[ONE_KBYTE];
	TCHAR m_szDSN[ONE_KBYTE];

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;
	BYTE m_bBatchRun;
	BYTE m_bSMRun;
	BYTE m_bTimerRun;

	WORD m_wPort;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;
	HANDLE m_hBatch;
	HANDLE m_hSM;
	HANDLE m_hTimer;

	HANDLE m_hBatchEvent;
	HANDLE m_hSMEvent;
	HANDLE m_hTimerEvent;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;

	BYTE m_bAutoStart; // 서비스 중지됐을 때 자동으로 다시 시작하기.
	DWORD m_dwManagerSeq;
	DWORD m_dwChatBanSeq;
	DWORD m_dwSendCount;
	BYTE m_bChatBanSuccess;
	DWORD m_dwEventIndex;

protected:
	void UploadEnd();	
	BYTE StartService(LPTSVRTEMP pService);
	BYTE StopService(LPTSVRTEMP pService);
	void QueryStatus();
	void QueryPlatform();	
	void OpenPlatform();

	LPTSVRTEMP FindService(BYTE bGroup, BYTE bType, BYTE bID);
	LPTSVRTEMP FindService(CTServer * pServer);
	LPTSVRTEMP FindService(DWORD dwID);
protected:
	DWORD OnSM_DELSESSION_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_OPLOGIN_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICESTAT_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICECONTROL_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_NEWCONNECT_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICECHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_TIMER_REQ(LPPACKETBUF pBUF);	
	DWORD OnCT_SERVICEMONITOR_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOADSTART_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOAD_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOADEND_REQ(LPPACKETBUF	pBUF);
	DWORD OnCT_UPDATEPATCH_REQ(LPPACKETBUF pBUF); // 현승룡 CT_UPDATEPATCH_REQ
	DWORD OnCT_ANNOUNCEMENT_REQ(LPPACKETBUF pBUF); // 현승룡 CT_ANNOUNCEMENT_REQ
	DWORD OnCT_USERKICKOUT_REQ(LPPACKETBUF pBUF); // 현승룡 CT_USERKICKOUT_REQ
	DWORD OnCT_USERMOVE_REQ(LPPACKETBUF pBUF); // 현승룡 CT_USERMOVE_REQ
	DWORD OnCT_USERPOSITION_REQ(LPPACKETBUF pBUF); // 현승룡 CT_USERPOSITION_REQ
	DWORD OnCT_MONSPAWNFIND_REQ(LPPACKETBUF pBUF); // 현승룡 CT_MONSPAWNFIND_REQ
	DWORD OnCT_MONSPAWNFIND_ACK(LPPACKETBUF pBUF); // 현승룡 CT_MONSPAWNFIND_ACK
	DWORD OnCT_MONACTION_REQ(LPPACKETBUF pBUF); // 현승룡 CT_MONACTION_REQ
	DWORD OnCT_STLOGIN_REQ(LPPACKETBUF pBUF); // 현승룡 CT_STLOGIN_REQ
	DWORD OnCT_PLATFORM_REQ(LPPACKETBUF pBUF); // 현승룡 CT_PLATFORM_REQ
	DWORD OnCT_USERPROTECTED_REQ(LPPACKETBUF pBUF); // 현승룡 CT_USERPROTECTED_REQ
	DWORD OnCT_CHARMSG_REQ(LPPACKETBUF pBUF); // 현승룡 CT_CHARMSG_REQ
	DWORD OnCT_RECONNECT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEAUTOSTART_REQ(LPPACKETBUF pBUF); //서비스 자동시작 기능 요청.
	DWORD OnCT_CHATBAN_REQ(LPPACKETBUF pBUF); //채팅금지요청.
	DWORD OnCT_CHATBAN_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEDATACLEAR_REQ(LPPACKETBUF pBUF); 
	DWORD OnCT_ITEMFIND_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMSTATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMFIND_ACK(LPPACKETBUF pBUF); // World -> Control
	DWORD OnCT_ITEMSTATE_ACK(LPPACKETBUF pBUF); // World -> Control
	DWORD OnCT_CHATBANLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CHATBANLISTDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEINFO_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEINFO_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEGUILDCHG_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEENABLE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTCHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMSALE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMSALE_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF);  
	DWORD OnCT_EVENTQUARTERLIST_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_TOURNAMENTEVENT_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_HELPMESSAGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_PREVERSIONUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_PREVERSIONTABLE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMEDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMEDATA_ACK(LPPACKETBUF pBUF);	
	DWORD OnCT_CMGIFT_REQ(LPPACKETBUF pBUF);	// 월드로 전송
	DWORD OnCT_CMGIFT_ACK(LPPACKETBUF pBUF);	// 해피로 리턴
	DWORD OnCT_CMGIFTLIST_REQ(LPPACKETBUF pBUF);	// 해피로 리스트 전송
	DWORD OnCT_CMGIFTLIST_ACK(LPPACKETBUF pBUF);	// 해피로 리스트 전송
	DWORD OnCT_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF);	// db업데이트후, 월드데이터 업데이트

protected:
	void ProcessSession( CTControlSession *pSession, DWORD dwIoBytes);
	void OnSendComplete( CTControlSession *pSession, DWORD dwIoBytes);

	void OnInvalidSession( CTControlSession *pServer);			// 사용 금지
	void OnCloseSession( CTControlSession *pServer);			// 사용 금지
	void ClosingSession( CTControlSession *pServer);			// 사용 금지
	void CloseSession( CTControlSession *pServer);				// 세션을 종료하려면 이 함수를 호출

	void SayToBATCH( LPPACKETBUF pBUF);
	void SayToSM( LPPACKETBUF pBUF);
	

	void ClearThread();
	void UpdateData();

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD LoadData();

	DWORD InitNetwork();
	DWORD InitDB();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive( LPPACKETBUF pBUF);

	DWORD ControlThread();
	DWORD BatchThread();
	DWORD WorkThread();
	DWORD SMThread();
	DWORD TimerThread();
	
	void SendSvrStatusSMS(BYTE _bSvrType, DWORD _dwSvrID, BYTE _bSvrStatus);
	CTManager* FindManager(DWORD dwID);
	CTManager* FindManager(CString strID);
	void CheckEvent();
	void SendEventToNewConnect(LPTSVRTEMP pService);
	void ParseStrValue(EVENTINFO& stEVENTINFO,CString str,BYTE bEventID);
	void MakeStrValue(EVENTINFO& stEVENTINFO,CString& str,BYTE bEventID);
	
protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _BatchThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);
	static DWORD WINAPI _SMThread( LPVOID lpParam);
	static DWORD WINAPI _TimerThread(LPVOID lpParam);

public:
	CTControlSvrModule();
	virtual ~CTControlSvrModule();

public:
	DECLARE_LIBID(LIBID_TControlSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TCONTROLSVR, "{A666C778-2308-47B0-A6F4-AAE1B0BB50D8}")
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


extern CTControlSvrModule _AtlModule;
