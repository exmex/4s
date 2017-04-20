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
#define EVENT_QUARTER_START				(20)
#define EVENT_QUARTER_END				(23)

class CTWorldSvrModule : public CAtlServiceModuleT< CTWorldSvrModule, IDS_SERVICENAME >
{
protected:
	QPACKETBUF m_qBATCHJOB;					// queue for batch job
	QPACKETBUF m_qDBJOB;					// queue for DB job
	QPACKETBUF m_qTIMERJOB;					// queue for TIMER job

	MAPTSERVER m_mapSESSION;				// all session pool
	MAPTSERVER m_mapSERVER;					// valid session pool

	CRITICAL_SECTION m_csDBQUEUE;			// sync object for DB queue
	CRITICAL_SECTION m_csQUEUE;				// sync object for batch queue
	CRITICAL_SECTION m_csBATCH;				// sync object for batch thread
	CRITICAL_SECTION m_csTIMERQUEUE;		// sync object for timer thread

protected:
	MAPTCHARACTERNAME m_mapTCHARNAME;		// Character pool
	MAPTCHARACTER m_mapTCHAR;				// Character pool
	VECTORDWORD m_vTOPERATOR;
	MAPDWORD	m_mapACTIVEUSER;

	MAPTGUILDLEVEL m_mapTGuildLevel;
	MAPTGUILD m_mapTGuild;

	MAPTCORPS m_mapTCorps;
	MAPTPARTY m_mapTParty;
	MAPTMS m_mapTMS;
	MAPTEVENTQUARTER m_mapEVQT;
	MAPTIMEQUARTER	 m_mapTimeEVQT;
	MAPTSTRING m_mapTSvrMsg;

	QUEUEWORD m_qGenPartyID;
	DWORD m_dwTMSIndex;
	DWORD m_dwTacticsIndex;
	DWORD m_dwGenRecallID;
	WORD m_wTournamentID;

	MAPDWORD m_mapApplicant;
	MAPDWORD m_mapTGuildEx;
	MAPDWORD m_mapWarCountry[TCONTRY_B][WARCOUNTRY_MAXGAP];

	TBATTLETIME m_battletime[BT_COUNT];

	CTServer * m_pRelay;
	CTServer * m_pCtrlSvr;

	MAPTGUILDWANTED m_mapTGuildWanted;
	MAPVTGUILDTACTICSWANTED m_mapTGuildTacticsWanted;
	MAPTGUILDWANTEDAPP m_mapTGuildWantedApp;
	MAPTGUILDTACTICSWANTEDAPP m_mapTGuildTacticsWantedApp;
	MAPBANCHAR m_mapBanChar;

	MAPDWORD m_mapCharGuild;
	MAPDWORD m_mapCharTactics;
	BYTE	m_bFirstGroupCount;

	VTEXPIREDBUF m_vExpired;
	 
	BYTE			m_bFameRankSave;	
	BYTE			m_bRankMonth; //랭킹이 업데이트 되야 하는 달
	MONTHRANKER		m_arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT]; //현재 달의 랭커 리스트
	MONTHRANKER		m_arLastFameRank[FAMERANKCOUNT]; //지난달의 명예의 전당 리스트
	MONTHRANKER		m_arFirstGradeGroup[COUNTRY_COUNT][FIRSTGRADEGROUPCOUNT]; //지난달의 1군 리스트

	MAPDWORD m_mapRank;
	MAPDWORD m_mapMonthRank;

	MAPTOURNAMENTSCHEDULE m_mapTournamentSchedule;	//Only use at Timer Thread
	MAPTOURNAMENTTIME m_mapTournamentTime;			//Only use at Timer Thread
	TOURNAMENTSCHEDULE m_TournamentSchedule;		//Only use at Timer Thread

	MAPTNMTPLAYER m_mapTNMTPlayer;
	MAPTOURNAMENT m_mapTournament;
	TOURNAMENTSTATUS m_tournament;

	MAPRPSGAME m_mapRPSGame;
	MAPCASTLEWARINFO m_mapCastleWarInfo;

	MAPCMGIFT m_mapCMGift;

protected:
	TCHAR m_szDBUserID[ONE_KBYTE];
	TCHAR m_szDBPasswd[ONE_KBYTE];
	TCHAR m_szDSN[ONE_KBYTE];

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;
	BYTE m_bBatchRun;
	BYTE m_bDBRun;
	BYTE m_bTimerRun;
	WORD m_wPort;

	BYTE m_bServerID;
	BYTE m_bGroupID;
	BYTE m_bNation;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;
	HANDLE m_hBatch;
	HANDLE m_hDB;
	HANDLE m_hTimer;

	HANDLE m_hBatchEvent;
	HANDLE m_hDBEvent;
	HANDLE m_hTimerEvent;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;

	BYTE m_bMaxLevel;

public:
	__time64_t m_timeCurrent;
	BYTE m_bReservedPostMinute;  
	DWORD m_dwRecentRecordDate;
	
	BYTE m_bCashItemSale;  	
	MAPTCASHITEMSALE m_mapTCashItemSale;	
	MAPEVENTINFO m_mapEVENT;

	LPTGUILDLEVEL FindGuildLevel(BYTE bLevel);
	void OnEventExpired(BYTE bInsert, BYTE bType, INT64 timeExpired, DWORD dwValue_1, DWORD dwValue_2);

protected:
	////////////////////////////////////////////////
	// Batch thread에서만 사용가능
	BYTE PushConCess(LPTCHARACTER pTCHAR, LPPACKETBUF pBUF);
	DWORD PopConCess(LPTCHARACTER pTCHAR);
	void OnBeginTeleport(LPTCHARACTER pTCHAR, LPPACKETBUF pBUF);
	void OnCheckConnect(LPTCHARACTER pTCHAR, LPPACKETBUF pBUF);

	CTServer *FindTServer( DWORD_PTR dwHandle);
	CTServer *FindMapSvr( BYTE bServerID);
	void ClearDeadCON( LPTCHARACTER pTCHAR);
	void CheckMainCON( LPTCHARACTER pTCHAR);

	LPTCHARACTER FindTChar(
		DWORD dwCharID,
		DWORD dwKEY);

	LPTCHARACTER FindTChar(
		CString strName);

	void BroadcastCorps(LPTCHARACTER pChar, CPacket * pPacket, WORD wMsgID);
	void BroadcastCorps(CTParty * pParty, CPacket * pPacket, WORD wMsgID);
	void NotifyDelCorpsUnit(CTCorps * pCorps, LPTCHARACTER pChar);
	void NotifyDelCorpsUnit(CTParty * pParty, LPTCHARACTER pChar);
	void NotifyAddCorpsUnit(CTCorps * pCorps, LPTCHARACTER pChar);
	void NotifyAddCorpsUnit(CTParty * pParty, LPTCHARACTER pChar);
	void NotifyCorpsJoin(CTCorps * pCorps, CTParty * pParty);
	CTCorps * NotifyCorpsLeave(CTCorps * pCorps, CTParty * pParty);
	void TransferCorpsCommand(
		LPTCHARACTER pChar,
		BYTE bCMD,
		BYTE bTgType,
		DWORD dwTgID,
		WORD wPosX,
		WORD wPosZ,
		WORD wSquad,
		DWORD dwCharID,
		WORD wMapID);

	void ChgSquadChief(CTCorps * pCorps, CTParty * pParty);
	void ChgSquadChief(CTParty * pParty, WORD wSquadID, DWORD dwChiefID);
	void ReportEnemyList(CTCorps * pCorps, WORD wPartyID, DWORD dwChiefID);

	void CorpsJoin(CTParty * pParty, WORD wCommanderID);
	void DelSquad(CTParty * pParty, WORD wDelParty);

	void AddPartyMember(CTParty * pParty, LPTCHARACTER pChar);
	void PartyAttr(LPTCHARACTER pChar);
	void CloseChar(LPTCHARACTER pChar);

	CTCorps *FindCorps( WORD wID);
	CTParty *FindParty( WORD wID);
	BYTE DeleteParty( WORD wID);
	WORD GenPartyID();
	DWORD GenRecallID();

	void LeaveParty( LPTCHARACTER, BYTE bKick, BYTE bDelete = TRUE);
	void JoinParty( CTParty * pParty, DWORD dwChiefID, LPTCHARACTER pTarget);
	void LeaveFriend( LPTCHARACTER);
	void LeaveSoulmate(LPTCHARACTER);
	void LeaveTMS( LPTCHARACTER);

	CTGuild *FindTGuild( DWORD dwGuildID);
	BYTE DeleteTGuild( DWORD dwGuildID);

	void SetCharLevel( LPTCHARACTER pCHAR, BYTE bLevel);
	void SetCharStatus( LPTCHARACTER pCHAR, DWORD dwMaxHP, DWORD dwHP, DWORD dwMaxMP, DWORD dwMP);
	void CheckTGuildExtinction( DWORD dwCurrentTime);
	
	void OnTimer();

	void CheckSoulmateEnd(LPTCHARACTER pTCHAR, LPTSOULMATE pTSOUL);
	void RegSoulmate(LPTCHARACTER pTCHAR, LPTCHARACTER pTarget, BYTE bSearch,BYTE bNpcInvenID, BYTE bNpcItemID);
	LPTITEM CreateItem(CPacket * pPacket);
	LPTITEM CreateItem(TITEM & item);
	void WrapItem(CPacket * pPacket, TITEM &item);
	void EraseFriend(LPTCHARACTER pTCHAR, DWORD dwTarget);
	void SoulmateEnd(LPTCHARACTER pTCHAR, DWORD dwTime);
	void SoulmateDel(LPTCHARACTER pTCHAR, DWORD dwSoul);
	CString GetSvrMsg(DWORD dwID);
	CString BuildNetString( const CString& strHeader, const CString& strBody);
	void SaveAllPvPRecord();
	void ResetCastleApply(CTGuild * pGuild, WORD wCastleID);
	BYTE CheckCorpsJoin(CTParty * pOrigin, CTParty * pTarget);

	BYTE AddGuildWanted(
		CTGuild * pGuild,
		BYTE bMinLevel,
		BYTE bMaxLevel,
		CString strTitle,
		CString strText,
		INT64 dlEndTime = 0);

	BYTE AddGuildTacticsWanted(
		CTGuild * pGuild,
		DWORD dwID,
		DWORD dwPoint,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper,
		BYTE bDay,
		BYTE bMinLevel,
		BYTE bMaxLevel,
		CString strTitle,
		CString strText,
		INT64 dlEndTime=0);

	BYTE DelGuildWanted(CTGuild * pGuild);
	BYTE DelGuildTacticsWanted(CTGuild * pGuild, DWORD dwID);
	LPTGUILDWANTED FindGuildWanted(DWORD dwGuildID);
	LPTGUILDTACTICSWANTED FindGuildTacticsWanted(DWORD dwGuildID, DWORD dwID);
	LPTGUILDTACTICSWANTED FindGuildTacticsWanted(DWORD dwID);
	LPTGUILDWANTEDAPP FindGuildWantedApp(DWORD dwCharID);
	LPTGUILDTACTICSWANTEDAPP FindGuildTacticsWantedApp(DWORD dwCharID);
	LPVTGUILDTACTICSWANTED FindGuildTacticsList(DWORD dwGuildID);
	BYTE AddGuildWantedApp(
		DWORD dwID,
		DWORD dwCharID,
		CString strName,
		BYTE bLevel,
		BYTE bClass,
		LPTCHARACTER pTCHAR = NULL);
	BYTE AddGuildTacticsWantedApp(
		DWORD dwID,
		DWORD dwCharID,
		CString strNAME,
		BYTE bLevel,
		BYTE bClass,
		DWORD dwGuildID,
		LPTCHARACTER pTCHAR = NULL);
	BYTE DelGuildWantedApp(DWORD dwCharID);
	BYTE DelGuildTacticsWantedApp(DWORD dwCharID);
	BYTE ApplyGuildApp(DWORD dwCharID);
	BYTE ApplyGuildTacticsApp(DWORD dwCharID);
	void NotifyAddGuildMember(LPTGUILDMEMBER pMember, CTGuild * pGuild);
	void NotifyGuildWantedList(DWORD dwCharID, DWORD dwKey, CTServer * pServer);
	void NotifyGuildTacticsWantedList(DWORD dwCharID, DWORD dwKey, CTServer * pServer);
	void NotifyGuildVolunteerList(DWORD dwCharID, DWORD dwKey, CTServer * pServer);
	void NotifyGuildTacticsVolunteerList(DWORD dwCharID, DWORD dwKey, CTServer * pServer);
	CTGuild * GetCurGuild(LPTCHARACTER pTCHAR);
	BYTE DeleteTactics(CTGuild * pGuild, DWORD dwTarget, BYTE bKick);
	BYTE AddTactics(CTGuild * pGuild, LPTTACTICSMEMBER pMember);
	DWORD FindCharTacticsID(DWORD dwCharID);
	void SendPost(BYTE bType, DWORD dwValue, DWORD dwSenderID, CString strSender, DWORD dwRecvID, CString strRecver, INT64 dlValue=0);
	void SendPost(BYTE bType, DWORD dwRecvID, CString strRecver, CString strTitle, CString strMessage, WORD wItemID, BYTE bItemNum, WORD wUseTime=0);
	void NotifyCastleApply(WORD wCastle, CTGuild * pGuild);
	void GuildMemberAdd(CTGuild * pGuild, LPTGUILDMEMBER pMember);
	void GuildMemberDel(CTGuild * pGuild, LPTGUILDMEMBER pMember, BYTE bReason);
	void GuildTacticsAdd(CTGuild * pGuild, LPTTACTICSMEMBER pMember);
	BYTE GuildTacticsDel(CTGuild * pGuild, LPTTACTICSMEMBER pMember, BYTE bKick);
	void MonthRankReset();  
	void AddChatBan(CString strName, INT64 dlEndTime);
	void CheckChatBan(LPTCHARACTER pChar, BYTE bRelay);

	BYTE IsFirstGroup(BYTE bCountry, DWORD dwCharID);

	WORD SetTournamentTime(LPMAPTOURNAMENTSTEP pTour, TBATTLETIME battletime, WORD wTourID, BYTE bMonthBase, BYTE bEnable, WORD wTnmtID=0, BYTE bTnmtGroup=0, BYTE bTnmtStep=TNMTSTEP_READY);
	void DelTournamentSchedule(WORD wTourID);
	BYTE CanDoTournament(BYTE bStep, BYTE bGroup=0);
	LPMAPTOURNAMENTENTRY GetTournament(WORD wTournamentID);
	LPTOURNAMENTENTRY GetCurrentTournamentEntry(BYTE bEntry);
	void GetRanking(DWORD dwCharID, DWORD & dwRank, DWORD & dwMonthRank);
	LPTNMTPLAYER FindTNMTPlayer(DWORD dwCharID);
	void AddTNMTPlayer(LPTOURNAMENTENTRY pEntry, LPTNMTPLAYER pPlayer, BYTE bStep, LPTNMTPLAYER pChief);
	void DelTNMTPlayer(LPTOURNAMENTENTRY pEntry, LPTNMTPLAYER pPlayer);
	LPTNMTPLAYER FindBatter(BYTE bEntry, LPTCHARACTER pTCHAR);
	void ResetBatting(LPTNMTPLAYER pTarget, LPTCHARACTER pTCHAR);
	void JoinBatting(LPTNMTPLAYER pTarget, LPTCHARACTER pTCHAR);
	void GetBattingAmount(LPTNMTPLAYER pTarget, DWORD dwCharID, FLOAT & fRate, DWORD & dwAmount);

	void TournamentSelectPlayer();
	void TournamentClear();

	void TournamentApply(LPTCHARACTER pChar, CTServer * pServer, BYTE bEntryID);
	void TournamentApplyInfo(LPTCHARACTER pChar, CTServer * pServer);
	void TournamentJoinList(LPTCHARACTER pChar, CTServer * pServer);
	void TournamentPartyAdd(
		LPTCHARACTER pChar,
		CTServer * pServer,
		DWORD dwTargetID,
        BYTE bCountry,
        CString strTarget,
        BYTE bLevel,
        BYTE bClass);
	void TournamentPartyDel(LPTCHARACTER pChar, CTServer * pServer, DWORD dwTargetID);
	void TournamentPartyList(LPTCHARACTER pChar, CTServer * pServer, DWORD dwChiefID);
	void TournamentEventJoin(LPTCHARACTER pChar, CTServer * pServer, BYTE bEntryID, DWORD dwTargetID);
	void TournamentEventList(LPTCHARACTER pChar, CTServer * pServer);
	void TournamentEventInfo(LPTCHARACTER pChar, CTServer * pServer, BYTE bEntryID);
	void TournamentMatchList(LPTCHARACTER pChar, CTServer * pServer);
	void TournamentInfo(CTServer * pServer = NULL);
	void TournamentMatch(CTServer * pServer = NULL);
	void TournamentSchedule(LPTCHARACTER pChar, CTServer * pServer);
	BYTE TournamentGetEntryCount();

	void TournamentUpdate(WORD wTourID=0);//Just run at Timer Thread
	void TNMTMatch(LPTOURNAMENTENTRY pEntry, MAPTNMTPLAYER map1st[TCONTRY_COUNT], MAPTNMTPLAYER mapNormal[TCONTRY_COUNT], BYTE bTotal);
	void TNMTEnterGate(LPTCHARACTER pChar, DWORD dwMoney, BYTE bEnter);
	void TNMTEntryDelete(WORD wTID, BYTE bEntryID);

	void LotteryItem(VLOTTERY& pLot, BYTE bMap, CString& szMsg, CString& szEventName);
	void GiftTime(LPEVENTINFO pEvent);//LPLOTTERY pLot, CString& szMsg);
	BYTE CheckMapID(WORD wMapType, WORD wMapID);
	CTGuild * SelectCastleWarGuild(BYTE bIsDef, LPTCASTLEWARINFO pCastle);
	void NotifyCastleWarInfo(CTServer * pServer = NULL);
	BYTE CompareGuildRank(CTGuild * pG1, CTGuild * pG2);
	DWORD CompareOccupation(LPTCASTLEWARINFO pCastle1, DWORD dwG1, LPTCASTLEWARINFO pCastle2, DWORD dwG2);
	void ChangeCountry(LPTCHARACTER pTCHAR, BYTE bType, BYTE bCountry);
	BYTE GetWarCountry(LPTCHARACTER pTCHAR);
	BYTE GetWarCountryGap(BYTE bLevel);
	BYTE IsMeetingRoom(WORD wMapID, BYTE bIsSmall);
	void CalcGuildRanking();
	
	__int64 GetNextEventTime(BYTE bWeek, BYTE bHour, BYTE bMin);
	void CheckEventQuarter();

	////////////////////////////////////////////////

protected:
	void CheckEventExpired();
	void ProcessSession( CTWorldSession *pSession, DWORD dwIoBytes);
	void OnSendComplete( CTWorldSession *pSession, DWORD dwIoBytes);

	void OnInvalidSession( CTWorldSession *pServer);			// 사용 금지
	void OnCloseSession( CTWorldSession *pServer);				// 사용 금지
	void ClosingSession( CTWorldSession *pServer);				// 사용 금지
	void CloseSession( CTWorldSession *pServer);				// 세션을 종료하려면 이 함수를 호출

	void SayToBATCH( LPPACKETBUF pBUF);
	void SayToDB( LPPACKETBUF pBUF);
	void SayToTIMER( LPPACKETBUF pBUF);

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
	DWORD DBThread();
	DWORD TimerThread();

#ifdef __TW_APEX
	static long ApexSendToMap(signed int nSendId,const char * pBuffer,int nLen);
	static long ApexKillUser(signed int nId, int nAction);
	_FUNC_S_REC	m_pApexRecv;
	void ApexNotifyUserEnter(DWORD dwCharID, CString strName, DWORD dwIP);
	void ApexNotifyUserLeave(DWORD dwCharID, CString strName);
	void ApexNotifyUserData(int nSendId,const char * pBuf,int nBufLen);
	void ApexNotifyUserReturn(int nSendId,const char * pRet);
#endif

protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _BatchThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);
	static DWORD WINAPI _DBThread( LPVOID lpParam);
	static DWORD WINAPI _TimerThread( LPVOID lpParam);

protected:
	// Control Server Message
	DWORD OnCT_SERVICEMONITOR_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_USERMOVE_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_USERPOSITION_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CHATBAN_REQ(LPPACKETBUF pBUF);  
	DWORD OnCT_CHARMSG_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEDATACLEAR_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMFIND_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMSTATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CTRLSVR_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTUPDATE_REQ(LPPACKETBUF pBUF);  
	DWORD OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMSALE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF);  
	DWORD OnCT_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_HELPMESSAGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMEDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CMGIFT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CMGIFTLIST_REQ(LPPACKETBUF pBUF);

	// System message handler
	DWORD OnSM_QUITSERVICE_REQ( LPPACKETBUF pBUF);
	DWORD OnSM_DELSESSION_REQ( LPPACKETBUF pBUF);
	DWORD OnSM_EVENTQUARTER_REQ( LPPACKETBUF pBUF);
	DWORD OnSM_EVENTQUARTERNOTIFY_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_BATTLESTATUS_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_EVENTEXPIRED_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_MONTHRANKSAVE_REQ(LPPACKETBUF pBUF);  
	DWORD OnSM_CHANGEDAY_REQ(LPPACKETBUF pBUF);

	// Timer message handler
	DWORD OnSM_GUILDDISORGANIZATION_REQ( LPPACKETBUF pBUF);
	DWORD OnSM_EVENTEXPIRED_ACK(LPPACKETBUF pBUF);
	DWORD OnSM_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_TOURNAMENTEVENT_ACK(LPPACKETBUF pBUF);
	DWORD OnSM_TOURNAMENT_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_TOURNAMENTUPDATE_REQ(LPPACKETBUF pBUF);

protected:
	// DB message handler

	/////////////////////////////////////////////////////////////////////
	// 길드
	DWORD OnDM_GUILDUPDATE_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDESTABLISH_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDESTABLISH_ACK( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDDISORGANIZATION_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDDISORGANIZATION_ACK( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDMEMBERADD_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDKICKOUT_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDEXTINCTION_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDEXTINCTION_ACK( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDLEAVE_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDDUTY_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDPEER_REQ( LPPACKETBUF pBUF);

	DWORD OnDM_GUILDLEVEL_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDCABINETMAX_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDARTICLEADD_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDARTICLEDEL_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDARTICLEUPDATE_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDFAME_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDCONTRIBUTION_REQ( LPPACKETBUF pBUF);
	DWORD OnDM_GUILDLOAD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDLOAD_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDWANTEDADD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDWANTEDDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDVOLUNTEERING_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDVOLUNTEERINGDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSWANTEDADD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSWANTEDDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSADD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSADD_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDTACTICSDEL_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_TACTICSPOINT_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTSTATUS_REQ(LPPACKETBUF pBUF);
	/////////////////////////////////////////////////////////////////////

	DWORD OnDM_FRIENDLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDLIST_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATELIST_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATELIST_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEREG_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEEND_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDINSERT_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDERASE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDERASE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDGROUPMAKE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDGROUPDELETE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDGROUPCHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_FRIENDGROUPNAME_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_RESERVEDPOSTSEND_REQ(LPPACKETBUF pBUF);  
	DWORD OnDM_RESERVEDPOSTRECV_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_CLEARMAPCURRENTUSER_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEREG_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEEND_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_SOULMATEDEL_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_ITEMFIND_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_ITEMSTATE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_ITEMSTATE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDPVPOINT_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GUILDPOINTREWARD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_PVPRECORD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_CASTLEAPPLY_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_CASHITEMSALE_REQ(LPPACKETBUF pBUF);  
	DWORD OnDM_CASHITEMSALE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_MONTHRANKSAVE_REQ(LPPACKETBUF pBUF);  
	DWORD OnDM_MONTHRANKSAVE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_CLEARDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_CLEARDATA_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF); 
	DWORD OnDM_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_EVENTQUARTERUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_GETCHARINFO_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_GETCHARINFO_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTPAYBACK_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTPAYBACK_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTRESULT_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTAPPLY_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTCLEAR_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TOURNAMENTEVENTCHARINFO_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TNMTEVENTSCHEDULEADD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TNMTEVENTSCHEDULEDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_TNMTEVENTENTRYADD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_HELPMESSAGE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_RPSGAMERECORD_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_ACTIVECHARUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_ACTIVECHARUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_CMGIFT_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_CMGIFT_ACK(LPPACKETBUF pBUF);
	DWORD OnDM_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnDM_CMGIFTCHARTUPDATE_ACK(LPPACKETBUF pBUF);


protected:
	// Server message handler
	DWORD OnMW_ARENAJOIN_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MONTHRANKRESETCHAR_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CHECKCONNECT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_RELEASEMAIN_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_MAPSVRLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_ENTERCHAR_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CLOSECHAR_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CHECKMAIN_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_ENTERSVR_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CHARDATA_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_ADDCHAR_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CONNECT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CONLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_ROUTE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CHGPARTYCHIEF_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_CHGPARTYTYPE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_PARTYADD_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_PARTYJOIN_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_PARTYDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_PARTYMANSTAT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_LEVELUP_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_PARTYMEMBERRECALLANS_ACK(LPPACKETBUF pBUF);
	////////////////////////////////////////////////////////////////////////////////////
	// 길드
	DWORD OnMW_GUILDESTABLISH_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GUILDDISORGANIZATION_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDINVITE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDINVITEANSWER_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDLEAVE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDDUTY_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDPEER_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDKICKOUT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDMEMBERLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDINFO_ACK( LPPACKETBUF pBUF);

	DWORD OnMW_GUILDCABINETLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDCABINETPUTIN_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDCABINETTAKEOUT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDCONTRIBUTION_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDARTICLELIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDARTICLEADD_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDARTICLEDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDARTICLEUPDATE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDFAME_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDWANTEDADD_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDWANTEDDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDWANTEDLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDVOLUNTEERING_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDVOLUNTEERINGDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDVOLUNTEERLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDVOLUNTEERREPLY_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSWANTEDADD_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSWANTEDDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSWANTEDLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSVOLUNTEERING_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSVOLUNTEERINGDEL_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSVOLUNTEERLIST_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSREPLY_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSKICKOUT_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSINVITE_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSANSWER_ACK( LPPACKETBUF pBUF);
	DWORD OnMW_GUILDTACTICSLIST_ACK( LPPACKETBUF pBUF);
	/////////////////////////////////////////////////////////////////////////////////////
	DWORD OnMW_CHAT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TAKEMONMONEY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MONSTERDIE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_ADDITEM_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_PARTYORDERTAKEITEM_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDASK_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDREPLY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDERASE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_ADDITEMRESULT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_ENTERSOLOMAP_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_LEAVESOLOMAP_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CHARSTATINFO_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CHARSTATINFOANS_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CHGCORPSCOMMANDER_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSLEAVE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSASK_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSREPLY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSCMD_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSENEMYLIST_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MOVECORPSENEMY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MOVECORPSUNIT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_ADDCORPSENEMY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_DELCORPSENEMY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CORPSHP_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_PARTYMOVE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TMSSEND_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TMSINVITEASK_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TMSINVITE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TMSOUT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_POSTRECV_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_LOCALOCCUPY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CASTLEOCCUPY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MISSIONOCCUPY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CASTLEAPPLY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MONTEMPT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GETBLOOD_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_FRIENDGROUPMAKE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDGROUPDELETE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDGROUPCHANGE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDGROUPNAME_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_PROTECTEDCHECK_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDPROTECTEDASK_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_DEALITEMERROR_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_MAGICMIRROR_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CREATERECALLMON_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_RECALLMONDEL_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_TELEPORT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_REGION_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_BEGINTELEPORT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_PETRIDING_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_HELMETHIDE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_PARTYMEMBERRECALL_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_SOULMATESEARCH_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_SOULMATEREG_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_SOULMATEEND_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CASTLEWARINFO_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_ENDWAR_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_RECALLMONDATA_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CHANGECHARBASE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_HEROSELECT_ACK(LPPACKETBUF pBUF);  
	DWORD OnMW_GAINPVPPOINT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_LOCALRECORD_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GUILDPOINTLOG_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GUILDPOINTREWARD_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GUILDPVPRECORD_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MONSTERBUY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_GUILDMONEYRECOVER_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FRIENDLIST_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CASHITEMSALE_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_MONTHRANKUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_FAMERANKUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_WARLORDSAY_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TERMINATE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CHATBAN_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_TOURNAMENT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TOURNAMENTENTERGATE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_TOURNAMENTRESULT_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_RPSGAME_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_WARCOUNTRYBALANCE_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_MEETINGROOM_ACK(LPPACKETBUF pBUF);

	DWORD OnMW_CMGIFT_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_CMGIFTRESULT_ACK(LPPACKETBUF pBUF);

// Relay Server Protocol
	DWORD OnRW_RELAYSVR_REQ(LPPACKETBUF pBUF);
	DWORD OnRW_ENTERCHAR_REQ(LPPACKETBUF pBUF);
	DWORD OnRW_RELAYCONNECT_REQ(LPPACKETBUF pBUF);
	

// APEX
	DWORD OnMW_APEXDATA_ACK(LPPACKETBUF pBUF);
	DWORD OnMW_APEXSTART_ACK(LPPACKETBUF pBUF);
	DWORD OnSM_APEXDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnSM_APEXKILLUSER_REQ(LPPACKETBUF pBUF);

public:
	// DB request message sender (Call on BATCH thread)

	void SendDM_ACTIVECHARUPDATE_REQ();

	void SendDM_FRIENDLIST_REQ(
		DWORD dwCharID,
		DWORD dwKEY);

	void SendDM_GUILDLOAD_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwGuildID);

	void SendDM_SOULMATELIST_REQ(
		DWORD dwCharID,
		DWORD dwKEY);

	void SendDM_SOULMATEREG_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwTarget,
		BYTE bSearch,
		BYTE bNpcInvenID,  
		BYTE bNpcItemID);

	void SendDM_SOULMATEEND_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		DWORD dwTime);

	void SendDM_SOULMATEDEL_REQ(
		DWORD dwCharID,
		DWORD dwSoul);

	void SendDM_FRIENDERASE_REQ(
		DWORD dwCharID,
		DWORD dwTarget);

	void SendDM_CASTLEAPPLY_REQ(
		WORD wCastle,
		DWORD dwTarget,
		BYTE bCamp);

	void SendDM_TOURNAMENTCLEAR_REQ();

	void SendDM_HELPMESSAGE_REQ(
		BYTE bID,
		INT64 dlStart,
		INT64 dlEnd,
		CString strMessage);

	void SendDM_TOURNAMENTSTATUS_REQ(
		WORD wID,
		BYTE bGroup,
		BYTE bStep);
	void SendDM_CLEARDATA_REQ(DWORD dwKey);

	void SendDM_GETCHARINFO_REQ(
		DWORD dwCharID,
		DWORD dwKey,
		WORD wProtocol,
		CString strTarget);

	void SendDM_TOURNAMENTPAYBACK_REQ(DWORD dwCharID, DWORD dwBack);
	void SendDM_TOURNAMENTRESULT_REQ(BYTE bStep, BYTE bRet, DWORD dwWin, DWORD dwLose);
	void SendDM_TOURNAMENTAPPLY_REQ(BYTE bAdd, DWORD dwCharID, BYTE bEntry=0, DWORD dwChiefID = 0);
	void SendDM_TNMTEVENTSCHEDULEADD_REQ(CPacket * pPacket, WORD wTourID);
	void SendDM_TNMTEVENTSCHEDULEDEL_REQ(WORD wTourID);
	void SendDM_TNMTEVENTENTRYADD_REQ(CPacket * pPacket);
	void SendDM_RPSGAMERECORD_REQ(BYTE bRecord, DWORD dwCharID, BYTE bType, BYTE bWinCount, INT64 dDate);

	void SendDM_CMGIFT_REQ(
		CString strTarget,
		WORD wGiftID,
		BYTE bTool,
		DWORD dwGmID);

	void SendDM_CMGIFT_ACK(BYTE bRet,
							CString strTarget,
							WORD wGiftID,
							BYTE bTool,
							DWORD dwGMID);

	void SendDM_CMGIFTCHARTUPDATE_REQ(CPacket * pPacket);
	////////////////////////////////////////////////////////////////
	// 길드
	void SendDM_GUILDDUTY_REQ(
		DWORD dwCharID,
		DWORD dwGuildID,
		BYTE bDuty);

	void SendDM_GUILDPEER_REQ(
		DWORD dwCharID,
		DWORD dwGuildID,
		BYTE bPeer);

	void SendDM_GUILDMEMBERADD_REQ(
		DWORD dwGuildID,
		DWORD dwCharID,
		BYTE bLevel,
		BYTE bDuty);

	void SendDM_GUILDARTICLEADD_REQ(
		DWORD dwGuildID,
		DWORD dwID,
		BYTE bDuty,
		CString strWritter,
		CString strTitle,
		CString strArticle,
		DWORD dwTime);

	void SendDM_GUILDARTICLEDEL_REQ(
		DWORD dwGuildID,
		DWORD dwID);

	void SendDM_GUILDARTICLEUPDATE_REQ(
		DWORD dwGuildID,
		DWORD dwID,
		CString strTitle,
		CString strArticle);

	void SendDM_GUILDFAME_REQ(
		DWORD dwGuildID,
		DWORD dwFame,
		DWORD dwFameColor);

	void SendDM_GUILDLEVEL_REQ(
		DWORD dwGuildID,
		BYTE bLevel);

	void SendDM_GUILDCABINETMAX_REQ(
		DWORD dwGuildID, 
		BYTE bMaxCabinet);

	void SendDM_GUILDCONTRIBUTION_REQ(
		DWORD dwGuildID,
		DWORD dwCharID,
		DWORD dwExp,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper);

	void SendDM_GUILDPVPOINT_REQ(
		DWORD dwGuildID,
		DWORD dwTotalPoint,
		DWORD dwUseablePoint,
		DWORD dwMonthPoint);

	void SendDM_GUILDPOINTREWARD_REQ(
		DWORD dwGuildID,
		DWORD dwPoint,
		CString strName,
		DWORD dwTotalPoint,
		DWORD dwUseablePoint);

	void SendDM_PVPRECORD_REQ(DWORD dwGuildID, LPTGUILDMEMBER pMember);

	void SendDM_CASHITEMSALE_REQ(DWORD dwIndex, WORD wValue, LPVTCASHITEMSALE vSALEITEM); 

	void SendDM_GUILDWANTEDADD_REQ(DWORD dwGuildID, BYTE bMinLevel, BYTE bMaxLevel, CString strTitle, CString strText);
	void SendDM_GUILDWANTEDDEL_REQ(DWORD dwGuildID);
	void SendDM_GUILDVOLUNTEERING_REQ(BYTE bType, DWORD dwCharID, DWORD dwID);
	void SendDM_GUILDVOLUNTEERINGDEL_REQ(BYTE bType, DWORD dwCharID);
	void SendDM_GUILDTACTICSWANTEDADD_REQ(
		DWORD dwID,
		DWORD dwGuildID,
		DWORD dwPoint, DWORD dwGold, DWORD dwSilver, DWORD dwCooper,
		BYTE bDay, BYTE bMinLevel, BYTE bMaxLevel,
		CString strTitle, CString strText);
	void SendDM_GUILDTACTICSWANTEDDEL_REQ(DWORD dwID);
	void SendDM_GUILDTACTICSADD_REQ(
		DWORD dwGuildID,
		DWORD dwTactics,
		DWORD dwPoint,
		INT64 dlMoney,
		BYTE bDay,
		INT64 dlEnd);
	void SendDM_GUILDTACTICSDEL_REQ(DWORD dwCharID);
	void SendDM_TACTICSPOINT_REQ(LPTTACTICSMEMBER pTactics);
	void SendDM_GUILDLEAVE_REQ(
		DWORD dwGuildID,
		DWORD dwCharID,
		BYTE bReason,
		DWORD dwTime);

	//////////////////////////////////////////////////////////////////
public:
	CTWorldSvrModule();
	virtual ~CTWorldSvrModule();

public:
	DECLARE_LIBID(LIBID_TWorldSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TWORLDSVR, "{2DD07566-9248-4798-A96F-259263CDA0E5}")
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


extern CTWorldSvrModule _AtlModule;
