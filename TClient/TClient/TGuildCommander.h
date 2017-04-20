#pragma once

class CTGuildCommander
{
public:
	/// 길드 권한 열거
	enum AuthorityEnum
	{
		AUTHORITY_MASTER,					///< 길드장 이상
		AUTHORITY_SUBMASTER,				///< 부길드장 이상
		AUTHORITY_MEMBER,					///< 길드원 이상
		AUTHORITY_NOT_MEMBER,				///< 길드원을 제외한 모두
		AUTHORITY_ALL,						///< 제한 없음( 모두 )
		AUTHORITY_COUNT
	};

	/// 명령 열거
	enum CmdEnum
	{
		CMD_SHOW_GUILDAPPDLG,				///< 길드 모집창 띄우기
		CMD_INVITE,							///< 길드 초대
		CMD_LEAVE,							///< 길드 탈퇴
		CMD_KICK,							///< 길드원 추방
		CMD_DISORGONIZATION,				///< 길드 해체 신청
		CMD_DISORGONIZATION_CANCEL,			///< 길드 해체 취소
		CMD_APPOINTMENT_MASTER,				///< 길드장 위임
		CMD_APPOINTMENT_SUBMASTER,			///< 부길드장 위임
		CMD_DISMISS_SUBMASTER,				///< 부길드장 해임
		CMD_PEER_CANCEL,					///< 주어진 작위를 취소
		CMD_PEER_BARON,						///< 남작에 임명
		CMD_PEER_VISCOUNT,					///< 자작에 임명
		CMD_PEER_COUNT,						///< 백작에 임명
		CMD_PEER_MARQUIS,					///< 후작에 임명
		CMD_PEER_DUKE,						///< 공작에 임명
		CMD_INVITE_TACTICS,					///< 용병초대
		CMD_COUNT
	};

	/// 기부 종류
	enum DonateEnum
	{
		DON_NONE,							///< 기부가 아님
		DON_EXP,							///< 경험치 기부
		DON_MONEY,							///< 자금 기부
		DON_PVP,							///< 공적 기부
	};

	/// 지원자 갱신 타입
	enum VolunUpdateType
	{
		VUT_NONE,
		VUT_GUILD,
		VUT_TACTICS,
		VUT_COUNT
	};

	/// 길드에 대한 상세 정보
	struct GuildDetInfo
	{
		DWORD				m_dwGuildID;			///< 길드 아이디
		CString				m_strGName;				///< 길드명
		//CString				m_strGDate;				///< 길드 개설 일
		INT64					m_ldwGDate;
		CString				m_strGMaster;			///< 길드장
		BYTE				m_bGMasterPeer;			///< 길드장 작위
		CString				m_strGSubMaster1;		///< 부 길드장1
		CString				m_strGSubMaster2;		///< 부 길드장2
		WORD				m_nGMbCnt;				///< 길드원의 수 (현재 수)
		WORD				m_nGMbMax;				///< 길드원의 수 (최대 가능 수)
		BYTE				m_nGLev;				///< 길드 레벨
		DWORD				m_dwGExpCur;			///< 길드 경험치 (현재 수치)
		DWORD				m_dwGExpMax;			///< 길드 경험치 (다음 레벨업 수치)
		CString				m_strGKind;				///< 길드 종류
		BYTE				m_bMyDuty;				///< 나의 직위
		DWORD				m_nGRune;				///< 길드 자금 (룬)
		DWORD				m_nGLuna;				///< 길드 자금 (루나)
		DWORD				m_nGCron;				///< 길드 자금 (크론)
		BYTE				m_bMyPeerage;			///< 나의 작위
		BYTE				m_bMyService;			///< 나의 길드 공헌도
		BOOL				m_bShowMark;			///< 길드 마크 표시 여부
		BYTE				m_bMark;				///< 길드 마크
		BYTE				m_bMarkBack;			///< 길드 마크 배경
		BYTE				m_bMarkColor;			///< 길드 마크 색
		BYTE				m_bMarkBackColor1;		///< 길드 마크 배경 색
		BYTE				m_bMarkBackColor2;		///< 길드 마크 배경 색
		CString				m_strNotify;			///< 길드 공지사항
		DWORD			m_dwPvPTotalPoint;
		DWORD			m_dwPvPUseablePoint;
		DWORD			m_dwMonthPoint;
		DWORD			m_dwRankTotal;
		DWORD			m_dwRankMonth;

		GuildDetInfo();
	};

	/// 길드원
	struct GuildMember
	{
		DWORD m_dwCharID;				///< 캐릭터 아이디
		CString m_strName;				///< 캐릭터 이름
		BYTE m_bLevel;				///< 레벨
		BYTE m_bClassID;				///< 직업
		BYTE m_bDuty;				///< 직위
		BYTE m_bPeer;				///< 작위
		BYTE m_bIsConnect;			///< 접속여부
		DWORD m_dwRegion;				///< 위치
		WORD m_wCastle;
		BYTE m_bCamp;
		DWORD m_dwTactics;		///< 다른 길드 용병인가?
		BYTE m_bWarCountry;
		INT64 m_dlConnectedDate;
	};
	typedef std::vector<GuildMember>	GuildMemberVec;

	/// 길드 용병
	struct GuildTactics
	{
		DWORD m_dwCharID;
		CString m_strName;
		BYTE m_bLevel;
		BYTE m_bClass;
		BYTE m_bDay;
		DWORD m_dwPoint;
		INT64 m_dlMoney;
		INT64 m_dlEndTime;
		DWORD m_dwGotPoint; // 계약기간동안 얻은 공적 포인트
		DWORD m_dwRegion;
		WORD m_wCastle;
		BYTE m_bCamp;
	};
	typedef std::vector<GuildTactics>	GuildTacticsVec;

	/// 길드 공지사항
	struct GuildNotify
	{
		DWORD				m_dwID;					///< 아이디	
		BYTE				m_bDuty;				///< 공지 작성자 권한
		CString				m_strTitle;				///< 공지 타이틀
		CString				m_strAuthor;			///< 공지 작성자
		CString				m_strText;				///< 공지 내용
		CString				m_strDate;				///< 작성일
	};
	typedef std::vector<GuildNotify>	GuildNotifyVec;

	/// 길드 지원자 정보
	struct GuildVolunteer
	{
		DWORD m_dwCharID;
		CString m_strName;
		BYTE m_bLevel;
		BYTE m_bClass;
		DWORD m_dwRegion; // 현재위치
	};
	typedef std::vector<GuildVolunteer>	GuildVolunteerVec;

	/// 용병 지원자 정보
	struct GuildTacticsVolunteer
	{
		DWORD m_dwCharID;
		CString m_strName;
		BYTE m_bLevel;
		BYTE m_bClass;
		DWORD m_dwRegion;
		BYTE m_bDay;
		DWORD m_dwPoint;
		DWORD m_dwGold;
		DWORD m_dwSilver;
		DWORD m_dwCooper;
	};
	typedef std::vector<GuildTacticsVolunteer> GuildTacticsVolunteerVec;

	/// 길드 구인 정보
	struct GuildOffer
	{
		//DWORD m_dwID;
		DWORD m_dwGuildID;
		CString m_strGuildName;
		CString m_strTitle;
		CString m_strText;
		BYTE m_bMinLevel;
		BYTE m_bMaxLevel;
		INT64 m_dlTime;
		BYTE m_bApplied;
	};
	typedef std::vector<GuildOffer>		GuildOfferVec;

	/// 길드 구직 정보
	struct GuildTacticsOffer
	{
		DWORD m_dwID;
		DWORD m_dwGuildID;
		CString m_strGuildName;
		CString m_strTitle;
		CString m_strText;
		BYTE m_bDay;
		BYTE m_bMinLevel;
		BYTE m_bMaxLevel	;
		DWORD m_dwPoint;
		DWORD m_dwGold;
		DWORD m_dwSilver;
		DWORD m_dwCooper;
		INT64 m_dlTime;
		BYTE m_bApplied;
	};
	typedef std::vector<GuildTacticsOffer>		GuildTacticsOfferVec;

	/// 지역 점령지 정보
	struct LocalTerritory
	{
		WORD				m_wCastleID;
		WORD				m_wLocalID;				///< 점령지아이디
		CString				m_strLocalName;			///< 점령지이름
		DWORD				m_dwGuildID;			///< 점령 길드
		CString				m_strGuildName;			///< 점령 길드 이름
		BYTE				m_bCountry;				///< 소속 국가
		__int64			m_dNextWar;
		CString			m_strHeroName;
	};
	typedef std::vector<LocalTerritory>	LocalTerritoryVec;

	/// 점령지 정보
	struct Territory
	{
		WORD m_wCastleID;			///< 성아이디
		CString m_strCastleName;		///< 성이름
		BYTE m_bCanApplyCastle;		///< 성 지원가능	//공성전
		DWORD m_dwGuildID;			///< 점령 길드
		CString m_strGuildName;			///< 점령 길드 이름
		BYTE m_bCountry;				///< 소속 국가
		INT64 m_dlNextWar;			///< 다음 공성 시간	//공성전
		CString m_strHero;				///< 공성 영웅		//공성전
		CString m_strAtkGuild;			///< 공격측 길드	//공성전
		CString m_strDefGuild;
		WORD m_wDefGuildPoint;
		WORD m_wDefCountryPoint;
		BYTE m_bDefCount;
		WORD m_wAtkGuildPoint;
		WORD m_wAtkCountryPoint;
		BYTE m_bAtkCount;
		WORD m_wMyGuildPoint;
		
		LocalTerritoryVec	m_vLocals;				///< 소속된 점령지 리스트
		VTOP3 m_vDTop3s; //드프겔 탑3
		VTOP3 m_vCTop3s; //크락시온 탑3
	};
	typedef std::vector<Territory>		TerritoryVec;

	struct Mission
	{
		WORD m_wMissionID;
		CString m_strMission;
		BYTE m_bCountry;				///< 소속 국가
		CTime m_NextWar;
		BYTE m_bStatus;
	};
	typedef std::vector<Mission>		MissionVec;
	Mission m_SkyGarden;

	/// 팝업 버튼 정보
	struct PopupBtnInfo
	{
		DWORD			m_dwTXT;
		DWORD			m_dwGM;
	};

	struct GuildLatestPvP
	{
		DWORD m_dwCharID;
		WORD m_wKillCount_D;
		WORD m_wDieCount_D;
		DWORD m_dwKillPoint_H_D;
		DWORD m_dwKillPoint_E_D;
		DWORD m_dwKillPoint_L_D;
		DWORD m_dwGodMonKillPoint_D;
		DWORD m_dwEntryPoint_D;
		DWORD m_dwResult;
	};
	typedef std::vector<GuildLatestPvP> GuildLatestPvPVec;

	struct GuildWeekPvP
	{
		DWORD m_dwCharID;
		WORD m_wKillCount_W;
		WORD m_wDieCount_W;
		DWORD m_dwKillPoint_H_W;
		DWORD m_dwKillPoint_E_W;
		DWORD m_dwKillPoint_L_W;
		DWORD m_dwGodMonKillPoint_W;
		DWORD m_dwEntryPoint_W;
		DWORD m_dwResult;
	};
	typedef std::vector<GuildWeekPvP> GuildWeekPvPVec;

	/// 명령에 따른 권한 배열
	static const DWORD CMD_AUTHORITY[];
	/// 명령에 따른 문자열 배열
	static const DWORD CMD_STRING[];
	/// 명령에 대한 상세 문자열 배열
	static const DWORD CMD_DESC_STRING[];

public:
	/// 길드 커맨드 인스턴스를 얻는다.
	static CTGuildCommander* GetInstance();

	/// 채팅 메시지에 의한 길드 명령 이벤트를 처리한다.
	static CString OnGuildCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER);
	/// 채팅 메시지 커맨드를 등록한다.
	static void RegChatCmds();

	/// 지위에 따른 문자열을 얻는다.
	static CString GetDutyStr(BYTE bDuty);
	/// 작위에 따른 문자열을 얻는다.
	static CString GetPeerageStr(BYTE bPeer);
	
public:
	DWORD				m_dwInviterID;			///< 나를 초대한 사람의 아이디
	BOOL				m_bDisorgCancel;		///< 길드 해체 취소인지 신청인지 여부
	DonateEnum			m_eCurDonMode;			///< 현재 기부 상태
	VolunUpdateType		m_eCurVolunUpdate;		///< 지원자 갱신 타입
	CString				m_strKickTarget;		///< 길드 추방 타겟
	DWORD				m_dwKickTarget;		///< 길드 추방 타겟 아이디
	CString				m_strTacticsInviter;	///< 나를 용병으로 초대한 사람

	GuildDetInfo		m_GuildDetInfo;			///< 길드 상세 정보
	GuildMemberVec		m_GuildMemberVec;		///< 길드원 리스트
	GuildTacticsVec		m_GuildTacticsVec;		///< 용병 리스트
	GuildNotifyVec		m_GuildNotifyVec;		///< 공지사항 리스트
	GuildLatestPvPVec	m_GuildLatestPvPVec;	///< 최근전투기여도
	GuildWeekPvPVec	m_GuildWeekPvPVec; ///< 주간전투기여도

	GuildVolunteerVec	m_GuildVolunteerVec;	///< 길드지원병 리스트
	GuildTacticsVolunteerVec	m_TacticsVolunteerVec;	///< 용병지원자 리스트

	GuildOfferVec		m_GuildOfferVec;		///< 길드 구인 리스트
	GuildTacticsOfferVec		m_GuildTacticsOfferVec;			///< 길드 구직 리스트

	TerritoryVec		m_TerritoryVec;			///< 점령지역 정보
	MissionVec		m_MissionVec;

public:
	/// 길드 관련 명령을 내린다.
	void DoGuildCommand(CmdEnum eCmd, const CString& strParam="");

	GuildMember* FindGuildMemberByID( DWORD dwCharID );

public:
	/// 길드 해체/창설 관련
	// -----------------------------------------------------------------------------------------
	/// 길드 창설 응답 메시지를 처리한다.
	void RecvGuildEstablish(BYTE bResult, DWORD dwGuildID, const CString& strGuild);
	
	/// 길드 해체 신청/취소 메시지를 처리한다.
	void RecvGuildDisorganization(BYTE bResult);
	/// 길드 해체 신청/취소 를 보낸다.
	void RequestGuildDisorganization(BOOL bCancel);

	/// 길드 초대/가입/탈퇴 관련
	// -----------------------------------------------------------------------------------------
	/// 길드 초대 메시지를 처리한다.
	void RecvGuildInvite(BYTE bResult, DWORD dwInviterID, const CString& strInviter, const CString& strGuild);
    /// 길드 초대에 대한 응답을 보낸다.
	void RequestGuildInviteAnswer(BYTE bReply);
	/// 주어진 플레이어를 길드로 초대한다.
	void RequestGuildInvite(const CString& strTarget);

	/// 길드 가입 메시지를 처리한다.
	void RecvGuildJoin(BYTE bResult, DWORD dwGuildID, const CString& strGuild, DWORD dwNewMemberID, const CString& strNewMember, BYTE bMax);
	
	/// 길드 탈퇴 메시지를 처리한다.
	void RecvGuildLeave(BYTE bResult, const CString& strTarget, BYTE bReason);
	/**
		특정 길드원 추방을 요청한다.

		@param strTarget	추방하고자 하는 길드원의 이름.
		@param bWarn		추방전에 경고 메시지를 띄울지 여부.
	*/
	void RequestGuildKickOut(const CString& strTarget, BOOL bWarn);
	void RequestGuildTacticsKickOut(const CString& strTarget, DWORD dwTarget, BYTE bWarn);
	/// 현재 길드에서 나간다,
	void RequestGuildLeave();

	/// 길드 권한 관련
	// -----------------------------------------------------------------------------------------
	/// 길드장 임명 메시지를 처리한다.
	void RecvGuildDuty(BYTE bResult, const CString& strTarget, BYTE bDuty);
	/// 주어진 길드원의 직위를 변경한다.
	void RequestGuildDuty(const CString& strTarget, BYTE bDuty);

	/// 길드 작위 변경 메시지를 처리한다.
	void RecvGuildPeer(BYTE bResult, const CString& strTarget, BYTE bPeer, BYTE bOldPeer);
	/// 주어진 길드원의 작위를 변경한다.
	void RequestGuildPeer(const CString& strTarget, BYTE bPeer);

	/// 길드 정보 관련
	// -----------------------------------------------------------------------------------------
	/// 길드관련 상태 변경 메시지를 처리한다.
	void RecvGuildAttr( DWORD dwPlayerID, DWORD dwGuildID, const CString& strGuildName, 
						BYTE bGuildPeer,  DWORD dwTacticsID, const CString& strTacticsName,
						CTClientGuildMark* pGuildMark );

	/// 길드 상세 정보를 얻는다.
	GuildDetInfo& GetGuildDetInfo() { return m_GuildDetInfo; }

	/// 길드 상세 정보 변경 메시지를 처리한다.
	void RecvGuildDetInfo(const GuildDetInfo& info);
	/// 길드 상세 정보를 요청한다.
	void RequestGuildDetInfo();
	/// 길드 멤버 리스트 변경 메시지를 처리한다.
	void RecvGuildMemberList(GuildMemberVec* pMembers);
	/// 길드 용병 리스트 변경 메시지를 처리한다.
	void RecvGuildTacticsList(GuildTacticsVec* pMembers);

	/// 길드 기부 관련
	// -----------------------------------------------------------------------------------------
	/// 길드 기부 메시지를 처리한다.
	void RecvGuildDonation(BYTE bResult);
	/// 길드 경험치 기부를 요청한다.
	void RequestGuildDonationExp(DWORD dwEXP);
	/// 길드 자금 기부를 요청한다.
	void RequestGuildDonationMoney(DWORD dwRune,DWORD dwLuna,DWORD dwCron);
	/// 길드 공적 기부를 요청한다.
	void RequestGuildDonationPVP(DWORD dwPVP);

	/// 공지사항 관련
	// -----------------------------------------------------------------------------------------
	/// 길드 공시사항 리스트 변경 메시지를 처리한다.
	void RecvGuildNotifyList(GuildNotifyVec* pNotVec);
	/// 길드 공지사항 정보를 요청한다.
	void RequestGuildNotifyList();
	
	/// 길드 공지사항의 추가를 요청한다.
	void RequestGuildNewNotify(const CString& strTitle, const CString& strArticle);
	/// 길드 공지사항의 삭제를 요청한다.
	void RequestGuildDelNotify(DWORD dwID);
	/// 길드 공지사항의 수정을 요청한다.
	void RequestGuildModNotify(DWORD dwID, const CString& strTitle, const CString& strArticle);

	/// 길드 지원자의 리스트 갱신 메시지를 처리한다.
	void RecvTacticsVolunteerList(GuildVolunteerVec* pVol);

	/// 지원자 삭제 메시지를 처리한다.
	void RecvDeleteVolunteer(BYTE bResult);
	/// 주어진 캐릭터를 지원자 리스트에서 삭제한다.
	void RequestDeleteVolunteer(DWORD dwCharID);

	/// 길드 마크 관련
	// -----------------------------------------------------------------------------------------
	/// 새로운 길드 마크 등록을 요청한다.
	void RequestGuildMarkReg(CTClientGuildMark* pMark);

	/// 점령지 관련
	// -----------------------------------------------------------------------------------------
	/// 점령지 리스트 메시지를 처리한다.
	void RecvTerritoryList(TerritoryVec* pTerr);
	/// 점령지 관련 리스트를 서버에 요청한다.
	void RequestTerritoryList();

	/// 용병 초대 관련 (길드구인보고 신청한 사람)
	// -----------------------------------------------------------------------------------------
	/// 주어진 플레이어를 용병으로 초대한다.
	void RequestTacticsOfferInvite(const CString& strName);
	/// 용병 초대 관련 (길드구직을 통한 금액제시)
	// -----------------------------------------------------------------------------------------
	/// 용병 초대 메시지를 처리한다.
	void RecvTacticsSeekInvite( const CString& strGuild, const CString& strInviter,
								DWORD dwRune, DWORD dwLuna, DWORD dwCron);
	/// 주어진 플레이어를 용병으로 초대한다.
	void RequestTacticsSeekInvite(DWORD dwID, DWORD dwRune, DWORD dwLuna, DWORD dwCron);

	/// 용병 관련
	// -----------------------------------------------------------------------------------------
	/// 용병 탈퇴 메시지를 처리한다.
	void RecvTacticsLeave(BYTE bResult);
	/// 용병 탈퇴를 요청한다.
	void RequestTacticsLeave();

public:
	/// 길드 멤버를 정렬한다.
	void SortGuildMember();
	/// 용병을 정렬한다.
	void SortTactics();
	/// 최근전투기여도
	void SortLatestPVP();
	/// 주간전투기여도
	void SortWeekPVP();

public:
	/// 주어진 인덱스의 길드원 정보를 얻는다.
	const GuildMember& GetGuildMember(size_t nIndex) { return m_GuildMemberVec[nIndex]; }
	/// 주어진 인덱스의 길드원 정보를 얻는다.
	GuildMember* GetGuildMemberPtr(size_t nIndex)	{ return &m_GuildMemberVec[nIndex]; }
	/// 전체 길드원 수를 얻는다.
	size_t GetCountGuildMember() { return m_GuildMemberVec.size(); }

	/// 주어진 인덱스의 용병 정보를 얻는다.
	GuildTactics* GetTacticsPtr(size_t nIndex) { return &m_GuildTacticsVec[nIndex]; }
	/// 전체 용병의 수를 얻는다.
	size_t GetCountTactics() { return m_GuildTacticsVec.size(); }

	///< 최근전투기여도
	const GuildLatestPvP& GetGuildLatestPvP(size_t nIndex) const	{ return m_GuildLatestPvPVec[nIndex]; }

	///< 주간전투기여도
	const GuildWeekPvP& GetGuildWeekPvP(size_t nIndex) const	{ return m_GuildWeekPvPVec[nIndex]; }

	/// 주어진 인덱스의 공지정보를 얻는다.
	const GuildNotify& GetGuildNotify(size_t nIndex) const	{ return m_GuildNotifyVec[nIndex]; }
	/// 전체 공지사항의 수를 얻는다.
	size_t GetCountGuildNotify() const { return m_GuildNotifyVec.size(); }

	/// 현재 지원자 갱신 상태를 얻는다.
	VolunUpdateType GetCurVolunUpdate() const { return m_eCurVolunUpdate; }

	/// 주어진 인덱스의 점령지정보를 얻는다.
	Territory& GetTerritory(size_t nIndex) { return m_TerritoryVec[nIndex]; }
	/// 전체 점령지의 수를 얻는다.
	size_t GetCountTerritory() const	{ return m_TerritoryVec.size(); }

protected:
	CTGuildCommander();
};




