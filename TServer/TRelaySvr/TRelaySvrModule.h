#pragma once

#define ON_RECEIVE(p)							case p : return On##p(pBUF);

class CTRelaySvrModule : public CAtlServiceModuleT< CTRelaySvrModule, IDS_SERVICENAME >
{

protected:
	virtual void OnERROR(DWORD dwErrorCode);
	virtual DWORD OnEnter();
	virtual void OnExit();

protected:
	QPACKETBUF m_qBATCHJOB;
	
	MAPPLAYER		m_mapSESSION;					// all session pool
	MAPPLAYER		m_mapPLAYER;					// valid session pool
	MAPPLAYERNAME	m_mapPLAYERNAME;

	MAPTSTRING		m_mapTSvrMsg;
	MAPTGUILD		m_mapTGUILD;
	MAPTPARTY		m_mapTPARTY;
	MAPTCORPS		m_mapTCORPS;
	MAPMEETING		m_mapMeeting;

	VECDWORD		m_vTOPERATOR;

	CRITICAL_SECTION	m_csQUEUE;					// sync object for batch queue
	CRITICAL_SECTION	m_csBATCH;					// sync object for batch thread
	
	
	TCHAR m_szWorldIP[ONE_KBYTE];
	TCHAR m_szControlIP[ONE_KBYTE];
	TCHAR m_szLogServerIP[ONE_KBYTE];

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;
	BYTE m_bBatchRun;

	WORD m_wRelayPort;
	WORD m_wWorldPort;
	WORD m_wLogServerPORT;

	BYTE m_bServerID;
	BYTE m_bGroupID;
	BYTE m_bNation;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;
	HANDLE m_hBatch;

	HANDLE m_hBatchEvent;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CTRelaySession m_listen;
	CTRelaySession m_world;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;
	SOCKADDR_IN m_addrCtrl;


	//
	//	Log Server Value
	//
#ifdef DEF_UDPLOG

public:
	CUdpSocket *m_pUdpSocket;

	BYTE GetServerID() { return m_bServerID; };

#endif	//	DEF_UDPLOG

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam );
	static DWORD WINAPI _BatchThread( LPVOID lpParam );
	static DWORD WINAPI _WorkThread( LPVOID lpParam );

protected:
	void ProcessSession( CTRelaySession *pSession, DWORD dwIoBytes);
	void OnSendComplete( CTRelaySession *pSession, DWORD dwIoBytes);
	
	void OnInvalidSession( CTRelaySession *pSession);		
	void OnCloseSession( CTRelaySession *pSession);			
	void ClosingSession( CTRelaySession *pSession);			
	void CloseSession( CTRelaySession *pSession);			// 세션을 종료하려면 이 함수를 호출

	void SayToBATCH( LPPACKETBUF pBUF);
	
	void ClearThread();
	void UpdateData();	

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD LoadData();

	DWORD InitNetwork();
	//DWORD InitDB();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD ControlThread();
	DWORD BatchThread();
	DWORD WorkThread();

	DWORD OnReceive( LPPACKETBUF pBUF);
	void SetEventCloseSession(CTRelaySession * pSession);

public:
	void ClearCharData(CTPlayer * pPlayer);
	BYTE IsOperator(DWORD dwID);
	CTPlayer* FindPlayer(DWORD dwID);
	CTPlayer* FindPlayer(CString strName);
	CTGuild* FindGuild(DWORD dwID);
	CTParty* FindParty(WORD wID);
	CTCorps* FindCorps(WORD wID);
	CTCorps* FindCorps(CTPlayer* pPlayer);
	
	void AddParty(CTParty* pPARTY);
	void AddGuild(CTGuild* pGUILD);
	void AddCorps(CTCorps* pCORPS);
	void DelParty(WORD wID);	
	void DelGuild(DWORD dwID);	
	void DelCorps(WORD wID);

	void BroadcastCorps(CTPlayer* pPlayer,CPacket* pPacket, WORD wMsgID);
	void BroadcastCorps(WORD wPartyID, CPacket* pPacket, WORD wMsgID);

	void ChgGuildMaster(CTGuild* pGuild,DWORD dwGuildMaster);
	void ChgPartyChief(CTParty* pParty,DWORD dwChiefID);	
	void TransferCorpsCommand(
		CTParty* pParty,
		WORD wSquadID,
		DWORD dwCharID,
		WORD wMapID,
		BYTE bCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wPosX,
		WORD wPosZ);
	
	CString GetSvrMsg(DWORD dwID);
	CString BuildNetString( const CString& strHeader, const CString& strBody);
	BYTE IsMeetingRoom(WORD wMapID, BYTE bIsSmall);

protected:

	///////////////////////////////////////////////////
	// Control server Message Handler

	DWORD OnCT_SERVICEMONITOR_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_ANNOUNCEMENT_ACK(LPPACKETBUF pBUF );
	DWORD OnCT_CHATBAN_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CHARMSG_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEDATACLEAR_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CTRLSVR_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF);

	
	///////////////////////////////////////////////////
	// System message Handler

	DWORD OnSM_QUITSERVICE_REQ( LPPACKETBUF pBUF);
	DWORD OnSM_DELSESSION_REQ( LPPACKETBUF pBUF);

	///////////////////////////////////////////////////
	// World message handler

	DWORD OnRW_CHATBAN_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_ENTERCHAR_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_RELAYSVR_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_CHANGENAME_ACK(LPPACKETBUF pBUF);
	
	// world party message
	DWORD OnRW_PARTYADD_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_PARTYDEL_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_PARTYCHGCHIEF_ACK(LPPACKETBUF pBUF);
	
	// world guild message
	DWORD OnRW_GUILDADD_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_GUILDDEL_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_GUILDCHGMASTER_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_TACTICSADD_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_TACTICSDEL_ACK(LPPACKETBUF pBUF);
	DWORD OnRW_CHANGEMAP_ACK(LPPACKETBUF pBUF);

	// world corps message
	DWORD OnRW_CORPSJOIN_ACK(LPPACKETBUF pBUF);


	///////////////////////////////////////////////////
	// Client message handler

	DWORD OnCS_RELAYCHARDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_CHAT_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_CORPSCMD_REQ(LPPACKETBUF pBUF);

	DWORD OnCS_CORPSENEMYLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_MOVECORPSENEMY_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_MOVECORPSUNIT_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_ADDCORPSENEMY_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_DELCORPSENEMY_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_CORPSHP_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_RELAYPROTECTEDLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_RELAYPROTECTEDADD_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_RELAYPROTECTEDDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_CHECKRELAY_REQ(LPPACKETBUF pBUF);
	DWORD OnCS_RELAYPROTECTEDOPTION_REQ(LPPACKETBUF pBUF);

#ifdef	DEF_UDPLOG
	DWORD OnDM_UDPLOG_REQ(LPPACKETBUF pBUF);
public:

	void SendDM_UDPLOG_REQ(_LOG_DATA_ pLogData);
#endif
    
public:
	///////////////////////////////////////////////////
	// world message sender
	void SendRW_ENTERCHAR_REQ( DWORD dwCharID , CString strName);
	void SendRW_RELAYSVR_REQ();
	void SendRW_RELAYCONNECT_REQ(DWORD dwCharID);
	void SendRW_CHATBAN_ACK(CString strTarget, WORD wMin);

public:
	CTRelaySvrModule();
	virtual ~CTRelaySvrModule();

public :
	DECLARE_LIBID(LIBID_TRelaySvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TRELAYSVR, "{391A2583-6A42-4915-BE88-0CCE50BA382F}")
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
	HRESULT PreMessageLoop(int nShowCmd);
	HRESULT PostMessageLoop();
};

extern CTRelaySvrModule _AtlModule;