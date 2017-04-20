#pragma once

#include <math.h>

///////////////////////////////////////////////////////////////////////////
// TWorldSvr macro

///////////////////////////////////////////////////////////////////////////
// TWorldSvr structure type
typedef struct tagTCHARACTER				TCHARACTER, *LPTCHARACTER;
typedef struct tagTCHARCON					TCHARCON, *LPTCHARCON;
typedef struct tagTGUILDMEMBER				TGUILDMEMBER, *LPTGUILDMEMBER;
typedef struct tagTTACTICSMEMBER			TTACTICSMEMBER, *LPTTACTICSMEMBER;
typedef struct tagTFRIEND					TFRIEND, *LPTFRIEND;
typedef struct tagTSOULMATE					TSOULMATE, *LPTSOULMATE;
typedef struct tagTCOMMAND					TCOMMAND, *LPTCOMMAND;
typedef struct tagTMS						TMS, *LPTMS;
typedef struct tagTITEM						TITEM, *LPTITEM;
typedef struct tagTGUILDLEVEL				TGUILDLEVEL, *LPTGUILDLEVEL;
typedef struct tagTGUILDARTICLE				TGUILDARTICLE, *LPTGUILDARTICLE;
typedef struct tagTGUILDWANTED				TGUILDWANTED, *LPTGUILDWANTED;
typedef struct tagTGUILDWANTEDAPP			TGUILDWANTEDAPP, *LPTGUILDWANTEDAPP;
typedef struct tagTGUILDTACTICSWANTED		TGUILDTACTICSWANTED, *LPTGUILDTACTICSWANTED;
typedef struct tagTGUILDTACTICSWANTEDAPP	TGUILDTACTICSWANTEDAPP, *LPTGUILDTACTICSWANTEDAPP;
typedef struct tagTEVENTQUARTER				TEVENTQUARTER, *LPTEVENTQUARTER;
typedef struct tagBATTLETIME				TBATTLETIME, *LPTBATTLETIME;
typedef struct tagTENTRYRECORD				TENTRYRECORD, *LPTENTRYRECORD;
typedef struct tagTGUILDPOINTREWARD			TGUILDPOINTREWARD, *LPTGUILDPOINTREWARD;
typedef struct tagTCASHITMSALE				TCASHITEMSALE, *LPTCASHITEMSALE;  
typedef struct tagTCASHITEMSALEEVENT		TCASHITEMSALEEVENT, *LPTCASHITEMSALEEVENT;
typedef struct tagMONEVENT					MONEVENT, *LPMONEVENT;
typedef struct tagEVENTINFO					EVENTINFO, *LPEVENTINFO;
typedef struct tagEXPIREDBUF				TEXPIREDBUF, *LPTEXPIREDBUF;
typedef struct tagMONTHRANKER				MONTHRANKER, *LPMONTHRANKER;  
typedef struct tagMONREGEN					MONREGEN,*LPMONREGEN;  
typedef struct tagLUCKYEVENT				LUCKYEVENT, *LPLUCKYEVENT;
typedef struct tagTNMTPLAYER				TNMTPLAYER, *LPTNMTPLAYER;
typedef struct tagTNMTREWARD				TNMTREWARD, *LPTNMTREWARD;
typedef struct tagTOURNAMENTENTRY			TOURNAMENTENTRY, *LPTOURNAMENTENTRY;
typedef struct tagTOURNAMENTSTEP			TOURNAMENTSTEP, *LPTOURNAMENTSTEP;
typedef struct tagTOURNAMENTSTATUS			TOURNAMENTSTATUS, *LPTOURNAMENTSTATUS;
typedef struct tagTOURNAMENTSCHEDULE		TOURNAMENTSCHEDULE, *LPTOURNAMENTSCHEDULE;
typedef struct tagLOTTERY					TLOTTERY,*LPLOTTERY;
typedef struct tagTRPSGAME					TRPSGAME, *LPRPSGAME;
typedef struct tagCASTLEWARINFO				TCASTLEWARINFO, *LPTCASTLEWARINFO;
typedef struct tagCASTLETOP3				TCASTLETOP3, *LPCASTLETOP3;
typedef struct tagCMGIFT					TCMGIFT, *LPCMGIFT;
///////////////////////////////////////////////////////////////////////////
// TWorldSvr class type

class CTWorldSession;
class CTServer;
class CTParty;
class CTGuild;
class CTCorps;

///////////////////////////////////////////////////////////////////////////
typedef vector<LPTCHARACTER>				VECTORTMEMBER, *LPVECTORTMEMBER;
typedef vector<DWORD>						VECTORDWORD, *LPVECTORDWORD;
typedef vector<WORD>						VWORD, *LPVWORD;
typedef vector<BYTE>						VECTORBYTE, *LPVECTORBYTE;
typedef vector<LPTITEM>						VTITEM, *LPVTITEM;
typedef vector<TENTRYRECORD>				VTENTRYRECORD, *LPVTENTRYRECORD;
typedef vector<TGUILDPOINTREWARD>			VTGUILDPOINTREWARD, *LPVTGUILDPOINTREWARD;
typedef vector<TGUILDTACTICSWANTED>			VTGUILDTACTICSWANTED, *LPVTGUILDTACTICSWANTED;
typedef vector<TCASHITEMSALE>				VTCASHITEMSALE, *LPVTCASHITEMSALE;  
typedef vector<TEXPIREDBUF>					VTEXPIREDBUF, *LPVTEXPIREDBUF;
typedef vector<MONTHRANKER>					VMONTHRANKER, *LPVMONTHRANKER;  
typedef vector<MONREGEN>					VMONREGEN, *LPVMONREGEN;  
typedef vector<LUCKYEVENT>					VLUCKYEVENT, *LPVLUCKYEVENT;
typedef vector<TNMTREWARD>					VTNMTREWARD, *LPVTNMTREWARD;
typedef vector<TLOTTERY>					VLOTTERY, *LPVLOTTERY;
typedef vector<INT64>						VINT64, *LPVINT64;

// TWorldSvr map type

typedef map< CString, LPTCHARACTER>			MAPTCHARACTERNAME, *LPMAPTCHARACTERNAME;
typedef map< DWORD, LPTCHARACTER>			MAPTCHARACTER, *LPMAPTCHARACTER;
typedef map< BYTE, LPTCHARCON>				MAPTCHARCON, *LPMAPTCHARCON;

typedef map< DWORD_PTR, CTServer *>			MAPTSERVER, *LPMAPTSERVER;

typedef map< DWORD, DWORD>					MAPDWORD, *LPMAPDWORD;
typedef map< WORD, WORD>					MAPWORD, *LPMAPWORD;
typedef map< BYTE, BYTE>					MAPBYTE, *LPMAPBYTE;
typedef map< BYTE, DWORD>					MAPBDWDRD, *LPMAPBDWORD;

typedef map< DWORD, CString>				MAPTSTRING, *LPMAPTSTRING;
typedef map< BYTE, CString>					MAPBSTRING, *LPMAPBSTRING;

typedef map< DWORD, CTGuild *>				MAPTGUILD, *LPMAPTGUILD;
typedef map< DWORD, CTCorps *>				MAPTCORPS, *LPMAPTCORPS;
typedef map< WORD, CTParty *>				MAPTPARTY, *LPMAPTPARTY;

typedef map< DWORD, LPTGUILDMEMBER>			MAPTGUILDMEMBER, *LPMAPTGUILDMEMBER;
typedef map< DWORD, LPTTACTICSMEMBER>		MAPTTACTICSMEMBER, *LPMAPTTACTICSMEMBER;
typedef map< DWORD, LPTFRIEND>				MAPTFRIEND, *LPMAPTFRIEND;
typedef map< DWORD, TFRIEND>				MAPTFRIENDTEMP;
typedef map< DWORD, LPTSOULMATE>			MAPTSOULMATE, *LPMAPTSOULMATE;
typedef map< DWORD, LPTMS>					MAPTMS, *LPMAPTMS;
typedef map< BYTE, LPTGUILDLEVEL>			MAPTGUILDLEVEL, *LPMAPTGUILDLEVEL;
typedef map< DWORD, LPTGUILDARTICLE>		MAPTGUILDARTICLE, *LPMAPTGUILDARTICLE;
typedef map< DWORD, LPTITEM>				MAPTITEM, *LPMAPTITEM;
typedef map< DWORD, TGUILDWANTED>			MAPTGUILDWANTED, *LPMAPTGUILDWANTED;
typedef map< DWORD, LPTGUILDWANTEDAPP>		MAPTGUILDWANTEDAPP, *LPMAPTGUILDWANTEDAPP;
typedef map< DWORD, VTGUILDTACTICSWANTED>	MAPVTGUILDTACTICSWANTED, *LPMAPVTGUILDTACTICSWANTED;
typedef map< DWORD, LPTGUILDTACTICSWANTEDAPP>	MAPTGUILDTACTICSWANTEDAPP, *LPMAPTGUILDTACTICSWANTEDAPP;

typedef map< INT64, WORD>					MAPTIMEQUARTER, *LPMAPTIMEQUARTER;
typedef map< WORD, LPTEVENTQUARTER>			MAPTEVENTQUARTER, *LPMAPTEVENTQUARTER;
typedef map< DWORD, TCASHITEMSALEEVENT>		MAPTCASHITEMSALE, *LPMAPTCASHITEMSALE;  
typedef map< DWORD , EVENTINFO >			MAPEVENTINFO, *LPMAPEVENTINFO;
typedef map< CString, INT64>				MAPBANCHAR, *LPMAPBANCHAR;
typedef map< DWORD, LPTNMTPLAYER >			MAPTNMTPLAYER, *LPMAPTNMTPLAYER;
typedef map< BYTE, LPTOURNAMENTENTRY >		MAPTOURNAMENTENTRY, *LPMAPTOURNAMENTENTRY;
typedef map< WORD, MAPTOURNAMENTENTRY >		MAPTOURNAMENT, *LPMAPTOURNAMENT;
typedef map< WORD, TOURNAMENTSTEP >			MAPTOURNAMENTSTEP, *LPMAPTOURNAMENTSTEP;
typedef map< WORD, TOURNAMENTSCHEDULE>		MAPTOURNAMENTSCHEDULE, *LPMAPTOURNAMENTSCHEDULE;
typedef map< WORD, TBATTLETIME>				MAPTOURNAMENTTIME, *LPMAPTOURNAMENTTIME;
typedef map< WORD, TRPSGAME>				MAPRPSGAME, *LPMAPRPSGAME;
typedef map< WORD, TCASTLEWARINFO>			MAPCASTLEWARINFO, *LPMAPCASTLEWARINFO;
typedef map< DWORD, VWORD>					MAPARRAY, *LPMAPARRAY;
typedef map< BYTE, TCASTLETOP3>				MAPCASTLETOP3, *LPMAPCASTLETOP3;
typedef map< DWORD, INT64 >					MAPINT64, *LPMAPINT64;
typedef map< WORD, LPCMGIFT >				MAPCMGIFT, *LPMAPCMGIFT;


typedef queue<WORD>							QUEUEWORD, *LPQUEUEWORD;
typedef queue<LPPACKETBUF>					QCONCESS, *LPQCONCESS;

///////////////////////////////////////////////////////////////////////////
// TWorldSvr MAX define
#define MAX_FRIEND			64
#define MAX_FRIENDGROUP		5
#define WORLD_TIMER							(1000)
#define NAME_NULL							_T("")
#define SOULMATE_LEVEL						10
#define MAX_CORPS_PARTY						7
#define GUILDWANTED_PERIOD					(60*60*24*14)
#define MAX_TACTICSWANTED					(5)

//#define __TW_APEX

#define MSG_DATE							_T("%d/ %d/ %d")
#define TIME_FORMAT							_T("%Y/ %m/ %d")
#define AIDCOUNTRY_RESETDAY					1

///////////////////////////////////////////////////////////////////////////
// TWorldSvr structure define
typedef enum GUILDAPP_TYPE
{
	GUILDAPP_MEMBER = 1,
	GUILDAPP_TACTICS
};

typedef enum EXPIRED_TYPE
{
	EXPIRED_GMW = 1,//길드원 모집광고
	EXPIRED_GTW,	//용병 모집광고
	EXPIRED_GT		//용병 계약 기간
} *LPEXPIRED_TYPE;

typedef enum SAVE_TYPE
{
	SAVE_LOADED = 1,
	SAVE_UPDATE = 2,
	SAVE_INSERT = 4,
	SAVE_DELETE = 8
} *LPSAVE_TYPE;

typedef enum BATTLE_TYPE
{
	BT_LOCAL = 0,
	BT_CASTLE,
	BT_TOURNAMENT,
	BT_MISSION,
	BT_COUNT
} *LPBATTLE_TYPE;

typedef enum EVENT_KIND
{
	EK_DEL = 0,
	EK_ADD ,  
	EK_UPDATE 
};

struct tagBATTLETIME
{
	BYTE m_bType;
	BYTE m_bStatus;
	DWORD m_dwBattleDur;
	DWORD m_dwBattleStart;
	DWORD m_dwAlarmStart;
	DWORD m_dwAlarmEnd;
	DWORD m_dwPeaceDur;
	BYTE m_bDay;
	BYTE m_bWeek;
	DWORD m_dwLeftTime;
	BYTE m_bRun;

	tagBATTLETIME()
	{
		m_dwBattleStart = 0;
		m_bDay = 0;
		m_bWeek = 0;
		m_bRun = FALSE;
	};
};

struct tagTMS
{
	DWORD m_dwID;
	CString m_strLastMember;
	MAPTCHARACTER m_mapMember;
};

struct tagTCOMMAND
{
	BYTE	m_bCommand;
	BYTE	m_bTgType;
	DWORD	m_dwTgObjID;
	WORD	m_wTgPosX;
	WORD	m_wTgPosZ;

	tagTCOMMAND()
	{
		m_bCommand = 0;
		m_bTgType = 0;
		m_dwTgObjID = 0;
		m_wTgPosX = 0;
		m_wTgPosZ = 0;
	};
};

struct tagTENTRYRECORD
{
	DWORD m_dwDate;
	WORD m_wKillCount;
	WORD m_wDieCount;
	DWORD m_aGainPoint[PVPE_COUNT];
	BYTE m_bLoad;

	tagTENTRYRECORD()
	{
		m_dwDate = 0;
		m_wKillCount = 0;
		m_wDieCount = 0;
		m_bLoad = 0;
		memset(m_aGainPoint, 0, sizeof(m_aGainPoint));
	};
};

struct tagTGUILDPOINTREWARD
{
	CString m_strName;
	DWORD m_dwPoint;
	INT64 m_dlDate;
};

///////////////////////////////////////////////////////////////////////////
// 길드
struct tagTGUILDLEVEL
{
	BYTE	m_bLevel;
	DWORD	m_dwEXP;
	BYTE	m_bMaxCnt;
	BYTE	m_bMinCnt;
	BYTE	m_bCabinetCnt;
	BYTE	m_bTacticsCnt;
	BYTE	m_bBattleSetCnt;
	BYTE	m_bGuardCnt;
	BYTE	m_bRoyalGuardCnt;
	BYTE	m_bTurretCnt;
	BYTE	m_bPeer[MAX_GUILD_PEER_COUNT];
};

struct tagTGUILDMEMBER
{
	DWORD m_dwGuildID;
	LPTCHARACTER m_pChar;
	DWORD	m_dwID;
	CString m_strName;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bDuty;
	BYTE	m_bPeer;
	WORD	m_wCastle;
	BYTE	m_bCamp;
	DWORD	m_dwTactics;
	BYTE	m_bSaveRecord;
	VTENTRYRECORD	m_vRecord;
	TENTRYRECORD m_weekrecord;
	BYTE	m_bWarCountry;
	INT64	m_dlConnectedDate;

	tagTGUILDMEMBER()
	{
		m_dwGuildID = 0;
		m_pChar = NULL;
		m_dwID = 0;
		m_strName = NAME_NULL;
		m_bLevel = 0;
		m_bClass = 0;
		m_bDuty = 0;
		m_bPeer = 0;
		m_wCastle = 0;
		m_bCamp = CAMP_NONE;
		m_dwTactics = 0;
		m_bSaveRecord = FALSE;
		m_dlConnectedDate = 0;
		m_bWarCountry = TCONTRY_N;
		m_vRecord.clear();
	};
};

struct tagTTACTICSMEMBER
{
	LPTCHARACTER m_pChar;
	DWORD	m_dwID;
	CString m_strName;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	DWORD	m_dwRewardPoint;
	DWORD	m_dwGainPoint;
	BYTE	m_bDay;
	INT64	m_dlEndTime;
	WORD	m_wCastle;
	BYTE	m_bCamp;
	INT64	m_dlRewardMoney;

	tagTTACTICSMEMBER()
	{
		m_pChar = NULL;
		m_dwID = 0;
		m_strName = NAME_NULL;
		m_bLevel = 0;
		m_bClass = 0;
		m_wCastle = 0;
		m_bCamp = 0;
		m_dlRewardMoney = 0;
	};
};

struct tagTGUILDARTICLE
{
	DWORD m_dwID;
	BYTE m_bDuty;
	CString m_strWritter;
	CString m_strTitle;
	CString m_strArticle;
	CString m_strDate;
};

struct tagTGUILDWANTED
{
	DWORD m_dwGuildID;
	BYTE m_bCountry;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	INT64 m_dlEndTime;
	CString m_strName;
	CString m_strTitle;
	CString m_strText;
	MAPTGUILDWANTEDAPP m_mapTWantedApp;
};

struct tagTGUILDTACTICSWANTED
{
	DWORD m_dwID;
	DWORD m_dwGuildID;
	DWORD m_dwPoint;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	BYTE m_bDay;
	BYTE m_bCountry;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	INT64 m_dlEndTime;
	CString m_strName;
	CString m_strTitle;
	CString m_strText;
	MAPTGUILDTACTICSWANTEDAPP m_mapTWantedApp;
};

struct tagTGUILDWANTEDAPP
{
	DWORD m_dwCharID;
	DWORD m_dwWantedID;
	DWORD m_dwRegion;
	BYTE m_bClass;
	BYTE m_bLevel;
	CString m_strName;
};

struct tagTGUILDTACTICSWANTEDAPP
{
	DWORD m_dwCharID;
	DWORD m_dwWantedGuildID;
	DWORD m_dwWantedID;
	DWORD m_dwRegion;
	DWORD m_dwPoint;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	BYTE m_bDay;
	BYTE m_bClass;
	BYTE m_bLevel;
	CString m_strName;
};
///////////////////////////////////////////////////////////////////////////

struct tagTCHARCON
{
	DWORD m_dwIPAddr;
	WORD m_wPort;

	BYTE m_bValid;
	BYTE m_bReady;
};

struct tagTFRIEND
{
	BYTE m_bConnected;
	CString m_strName;
	DWORD m_dwID;
	DWORD m_dwRegion;

	BYTE m_bLevel;
	BYTE m_bGroup;
	BYTE m_bClass;
	BYTE m_bType;
};

struct tagTSOULMATE
{
	DWORD m_dwCharID;
	DWORD m_dwTarget;
	CString m_strName;
	BYTE m_bLevel;
	BYTE m_bClass;
	BYTE m_bConnected;
	DWORD m_dwRegion;
	DWORD m_dwTime;
};

struct tagTCHARACTER
{
	MAPTCHARCON m_mapTCHARCON;
	MAPTFRIEND m_mapTFRIEND;
	MAPTSOULMATE m_mapTSOULMATE;
	MAPTNMTPLAYER m_mapBatting;
	QCONCESS m_qConCess;

	MAPBSTRING m_mapFRIENDGROUP;
	MAPDWORD m_mapTMS;

	VECTORBYTE m_vTDEADCON;
	CString m_strNAME;

	DWORD m_dwCharID;
    DWORD m_dwKEY;

	BYTE m_bDBLoading;
	BYTE m_bCHGMainID;
	BYTE m_bMainID;
	BYTE m_bLogout;
	BYTE m_bSave;

	BYTE m_bChannel;
	WORD m_wMapID;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	TCOMMAND m_command;
	CTParty *m_pParty;
	CTGuild *m_pGuild;
	CTGuild *m_pTactics;

	DWORD m_dwRiding;
	DWORD m_dwRegion;
	DWORD m_dwMaxHP;
	DWORD m_dwHP;
	DWORD m_dwMaxMP;
	DWORD m_dwMP;
	DWORD m_dwUserID;
	DWORD m_dwSoulSilence;
	DWORD m_dwTicket;

	BYTE m_bRealSex;
	BYTE m_bPartyWaiter;
	BYTE m_bCountry;
	BYTE m_bAidCountry;
	BYTE m_bClass;
	BYTE m_bLevel;
	BYTE m_bRace;
	BYTE m_bFace;
	BYTE m_bHair;
	BYTE m_bSex;
	BYTE m_bHelmetHide;
	INT64 m_nChatBanTime;  
	INT64 m_timeRelay;

	~tagTCHARACTER()
	{
		MAPTFRIEND::iterator itFRIEND;
		MAPTSOULMATE::iterator itSOULMATE;
		MAPTCHARCON::iterator itCON;
		m_vTDEADCON.clear();

		for( itFRIEND = m_mapTFRIEND.begin(); itFRIEND!=m_mapTFRIEND.end(); itFRIEND++)
			delete (*itFRIEND).second;
		m_mapTFRIEND.clear();

		for( itSOULMATE = m_mapTSOULMATE.begin(); itSOULMATE!=m_mapTSOULMATE.end(); itSOULMATE++)
			delete (*itSOULMATE).second;
		m_mapTSOULMATE.clear();

		for( itCON = m_mapTCHARCON.begin(); itCON != m_mapTCHARCON.end(); itCON++)
			delete (*itCON).second;
		m_mapTCHARCON.clear();

		while(!m_qConCess.empty())
		{
			delete m_qConCess.front();
			m_qConCess.pop();
		}
		m_mapBatting.clear();
	};
};

struct tagTITEM
{
	__int64 m_dlID;
	DWORD m_dwItemID;
	BYTE m_bItemID;
	WORD m_wItemID;
	BYTE m_bType;
	BYTE m_bKind;
	BYTE m_bLevel;
	BYTE m_bCount;
	BYTE m_bGLevel;
	DWORD m_dwDuraMax;
	DWORD m_dwDuraCur;
	BYTE m_bRefineCur;
	__time64_t m_dEndTime;
	BYTE m_bGradeEffect;
	BYTE m_bMagicCount;
	BYTE m_bMagic[TMAGIC_MAX];
	WORD m_wValue[TMAGIC_MAX];
	DWORD m_dwExtValue[TMAGIC_MAX];

	tagTITEM()
	{
		m_dlID = 0;
		m_dwItemID = 0;
		m_bType = 0;
		m_bKind = 0;
		m_dEndTime = 0;
		m_bMagicCount = 0;
		m_bGradeEffect = 0;

		for(BYTE i=0; i<TMAGIC_MAX; i++)
		{
			m_bMagic[i] = 0;
			m_wValue[i] = 0;
			m_dwExtValue[i] = 0;
		}
	};
};

struct tagTEVENTQUARTER
{
	BYTE m_bDay;
	BYTE m_bHour;
	BYTE m_bMinute;
	BYTE m_bNotice;
	WORD m_wID;
	INT64 m_nTime;
	CString m_strPresent;
	CString m_strAnnounce;
};

 
struct tagTCASHITMSALE
{	
	WORD m_wID;
	BYTE m_bSaleValue;
};

struct tagTCASHITEMSALEEVENT
{
	DWORD m_dwIndex;
	WORD m_wValue;
	VTCASHITEMSALE m_vCashItemSale;
};

struct tagMONEVENT
{
	BYTE m_bStartAction;
	BYTE m_bEndAction;
	VWORD m_vSpawnID;
};
 

struct tagMONREGEN
{
	WORD	m_wMonID;
	DWORD	m_dwDelay;
	WORD	m_wMapID;
	FLOAT	m_fPosX;
	FLOAT	m_fPosY;
	FLOAT	m_fPosZ;
};

struct LOTPACKET
{
	LPLOTTERY				m_pLot;
	vector<LPTCHARACTER>	m_pChar;
};

struct tagLOTTERY
{
	WORD	m_wItemID;
	BYTE	m_bNum;
	WORD	m_wWinner;
};

struct tagEVENTINFO
{
	DWORD	m_dwIndex;
    BYTE	m_bID;			// 이벤트 종류
	CString m_strTitle;		// 이벤트 이름
	BYTE	m_bGroupID;		// 서버 그룹
	BYTE	m_bSvrType;
	BYTE	m_bSvrID;		// 0 : ALL 
	__time64_t m_dStartDate;// 시작날짜시간
	__time64_t m_dEndDate;	// 종료날짜시간
	WORD	m_wValue;		// 이벤트값
	WORD	m_wMapID;		// 0xFF:ALL 맵아이디
	DWORD	m_dwStartAlarm; 
	DWORD	m_dwEndAlarm;
	BYTE	m_bStartAlarm;
	BYTE	m_bEndAlarm;
	CString m_strStartMsg;
	CString m_strEndMsg;
	BYTE	m_bState;		// 이벤트 상태
	BYTE	m_bPartTime;
	VTCASHITEMSALE m_vCashItem;
	MONEVENT m_stMONEVENT;	
	VMONREGEN m_vMONREGEN;
	VLOTTERY m_vLOTTERY;	// 경품아이템
	CString m_strLotMsg;

	void Reset()
	{
		m_dwIndex = 0;
		m_bID = 0;
		m_strTitle = _T("");
		m_bGroupID = 0;
		m_bSvrType = 0;
		m_bSvrID = 0;
		m_dStartDate = 0;
		m_dEndDate = 0;
		m_wValue = 0;
		m_wMapID = 0xFF;
		m_dwStartAlarm = 0;
		m_dwEndAlarm = 0;
		m_bStartAlarm = 0;
		m_bEndAlarm = 0;
		m_strStartMsg = _T("");
		m_strEndMsg = _T("");
		m_bState = 0;
		m_vCashItem.clear();
		m_stMONEVENT.m_bEndAction = 0;
		m_stMONEVENT.m_bEndAction = 0;
		m_stMONEVENT.m_vSpawnID.clear();
		m_vMONREGEN.clear();		
		m_bPartTime = 0;
		m_vLOTTERY.clear();
		m_strLotMsg = _T("");
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		WORD wCount,i,wTemp;
		
		(*pMsg)
			>> m_dwIndex
			>> m_bID
			>> m_bState
			>> m_bGroupID
			>> m_bSvrType
			>> m_bSvrID
			>> m_dStartDate
			>> m_dEndDate	
			>> m_wValue
			>> m_wMapID
			>> m_dwStartAlarm
			>> m_dwEndAlarm
			>> m_strStartMsg
			>> m_strEndMsg
			>> m_strTitle
			>> m_bPartTime
			>> m_strLotMsg
			>> wCount;

		for(i = 0; i < wCount; i++)
		{
			TCASHITEMSALE stSALEITEM;

			(*pMsg)
				>> stSALEITEM.m_wID
				>> stSALEITEM.m_bSaleValue;
			
			m_vCashItem.push_back(stSALEITEM);
		}
		
		(*pMsg)
			>> m_stMONEVENT.m_bStartAction
			>> m_stMONEVENT.m_bEndAction
			>> wCount;

		for(i = 0; i < wCount ; i++)
		{
			(*pMsg)
				>> wTemp;

			m_stMONEVENT.m_vSpawnID.push_back(wTemp);
		}

		(*pMsg)
			>> wCount;

		for (i = 0; i < wCount; i++)
		{
			MONREGEN stMONREGEN;

			(*pMsg)
				>> stMONREGEN.m_wMonID
				>> stMONREGEN.m_dwDelay
				>> stMONREGEN.m_wMapID
				>> stMONREGEN.m_fPosX
				>> stMONREGEN.m_fPosY
				>> stMONREGEN.m_fPosZ;

			m_vMONREGEN.push_back(stMONREGEN);
		}			
		(*pMsg)
			>> wCount;

		for (i = 0; i < wCount; i++)
		{
			TLOTTERY stLOTTERY;

			(*pMsg)
				>> stLOTTERY.m_wItemID
				>> stLOTTERY.m_bNum
				>> stLOTTERY.m_wWinner;

			m_vLOTTERY.push_back(stLOTTERY);
		}
	}

	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_dwIndex
			<< m_bID
			<< m_bState
			<< m_bGroupID
			<< m_bSvrType
			<< m_bSvrID
			<< m_dStartDate
			<< m_dEndDate	
			<< m_wValue
			<< m_wMapID
			<< m_dwStartAlarm
			<< m_dwEndAlarm
			<< m_strStartMsg
			<< m_strEndMsg
			<< m_strTitle
			<< m_bPartTime
			<< m_strLotMsg
			<< (WORD)m_vCashItem.size();

		WORD i;
		for(i = 0; i < (WORD)m_vCashItem.size(); i++)
		{
			(*pMsg)
				<< m_vCashItem[i].m_wID
				<< m_vCashItem[i].m_bSaleValue;
		}

		(*pMsg)
			<< m_stMONEVENT.m_bStartAction
			<< m_stMONEVENT.m_bEndAction
			<< (WORD)m_stMONEVENT.m_vSpawnID.size();

		for(i = 0; i < (WORD)m_stMONEVENT.m_vSpawnID.size(); i++)
		{
			(*pMsg)
				<< m_stMONEVENT.m_vSpawnID[i];
		}

		(*pMsg)
			<< (WORD)m_vMONREGEN.size();
		for(i = 0; i < (WORD)m_vMONREGEN.size(); i++)
		{
			(*pMsg)
				<< m_vMONREGEN[i].m_wMonID
				<< m_vMONREGEN[i].m_dwDelay
				<< m_vMONREGEN[i].m_wMapID
				<< m_vMONREGEN[i].m_fPosX
				<< m_vMONREGEN[i].m_fPosY
				<< m_vMONREGEN[i].m_fPosZ;
		}

		(*pMsg)
			<< (WORD)m_vLOTTERY.size();
		for(i = 0; i < (WORD)m_vLOTTERY.size(); i++)
		{
			(*pMsg)
				<< m_vLOTTERY[i].m_wItemID
				<< m_vLOTTERY[i].m_bNum
				<< m_vLOTTERY[i].m_wWinner;
		}
	}
};

struct tagEXPIREDBUF
{
	BYTE m_bType;
	INT64 m_timeExpired;
	DWORD m_dwValue_1;
	DWORD m_dwValue_2;
};

 
struct tagMONTHRANKER
{
	DWORD	m_dwCharID;
	CString m_strName;
	DWORD	m_dwTotalRank;
	DWORD	m_dwMonthRank;
	DWORD	m_dwTotalPoint;
	DWORD	m_dwMonthPoint;
	WORD	m_wMonthWin;
	WORD	m_wMonthLose;
	DWORD	m_dwTotalWin;
	DWORD	m_dwTotalLose;
	BYTE	m_bCountry;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bRace;
	BYTE	m_bSex;
	BYTE	m_bHair;
	BYTE	m_bFace;
	CString	m_strSay;
	CString m_strGuild;

	tagMONTHRANKER&	operator = (const tagMONTHRANKER& rhs)
	{
		m_dwCharID = rhs.m_dwCharID;
		m_strName = rhs.m_strName;
		m_dwTotalRank = rhs.m_dwTotalRank;
		m_dwMonthRank = rhs.m_dwTotalRank;
		m_dwTotalPoint = rhs.m_dwTotalPoint;
		m_dwMonthPoint = rhs.m_dwMonthPoint;
		m_wMonthWin = rhs.m_wMonthWin;
		m_wMonthLose = rhs.m_wMonthLose;
		m_dwTotalWin = rhs.m_dwTotalWin;
		m_dwTotalLose = rhs.m_dwTotalLose;
		m_bCountry = rhs.m_bCountry;
		m_bLevel = rhs.m_bLevel;
		m_bClass = rhs.m_bClass;
		m_bRace = rhs.m_bRace;
		m_bSex = rhs.m_bSex;
		m_bHair = rhs.m_bHair;
		m_bFace = rhs.m_bFace;
		m_strSay = rhs.m_strSay;
		m_strGuild = rhs.m_strGuild;

		return (*this);
	}
	void Reset()
	{
		m_dwCharID = 0;
		m_strName = NAME_NULL;
		m_dwTotalRank = 0;
		m_dwMonthRank = 0;
		m_dwTotalPoint = 0;
		m_dwMonthPoint = 0;
		m_wMonthWin = 0;
		m_wMonthLose = 0;
		m_dwTotalWin = 0;
		m_dwTotalLose = 0;
		m_bCountry = 0;
		m_bLevel = 0;
		m_bClass = 0;
		m_bRace = 0;
		m_bSex = 0;
		m_bHair = 0;
		m_bFace = 0;
		m_strSay = NAME_NULL;
		m_strGuild = NAME_NULL;
	}
	void WrapPacketIn(CPacket* pMSG)
	{
		(*pMSG)
			<< m_dwTotalRank
			<< m_dwMonthRank
			<< m_dwCharID
			<< m_strName
			<< m_dwTotalPoint
			<< m_dwMonthPoint
			<< m_wMonthWin
			<< m_wMonthLose
			<< m_dwTotalWin
			<< m_dwTotalLose
			<< m_bCountry
			<< m_bLevel
			<< m_bClass
			<< m_bRace
			<< m_bSex
			<< m_bHair
			<< m_bFace
			<< m_strSay
			<< m_strGuild;		
	}
	void WrapPacketOut(CPacket* pMSG)
	{
		(*pMSG)
			>> m_dwTotalRank
			>> m_dwMonthRank
			>> m_dwCharID
			>> m_strName
			>> m_dwTotalPoint
			>> m_dwMonthPoint
			>> m_wMonthWin
			>> m_wMonthLose
			>> m_dwTotalWin
			>> m_dwTotalLose
			>> m_bCountry
			>> m_bLevel
			>> m_bClass
			>> m_bRace
			>> m_bSex
			>> m_bHair
			>> m_bFace
			>> m_strSay
			>> m_strGuild;
	}
};

struct MonthRankDesc
{
	bool operator()(MONTHRANKER& p1, MONTHRANKER& p2) const
	{
		if(p1.m_dwMonthPoint > p2.m_dwMonthPoint)
			return TRUE;
		else if( p1.m_dwMonthPoint == p2.m_dwMonthPoint)
		{
			if(p1.m_wMonthWin > p2.m_wMonthWin)
				return TRUE;
			else if( p1.m_wMonthWin == p2.m_wMonthWin)
			{
				if(p1.m_wMonthLose > p2.m_wMonthLose)
					return TRUE;
				else if(p1.m_wMonthLose == p2.m_wMonthLose)				
					return p1.m_dwCharID < p2.m_dwCharID ;
			}
			return FALSE;
		}		
		return FALSE;		
	}
};

struct tagLUCKYEVENT
{
	WORD	m_wID;
	BYTE	m_bDay;
	BYTE	m_bHour;
	BYTE	m_bMin;
	WORD	m_wItemID1;
	WORD	m_wItemID2;
	WORD	m_wItemID3;
	WORD	m_wItemID4;
	WORD	m_wItemID5;
	BYTE	m_bCount;
	CString	m_strItem1;
	CString	m_strItem2;
	CString m_strItem3;
	CString m_strItem4;
	CString m_strItem5;
	CString m_strPresent;
	CString	m_strAnnounce;
	CString m_strTitle;
	CString	m_strMessage;

	void Reset()
	{
		m_wID = 0;
		m_bDay = 0;
		m_bHour = 0;
		m_bMin = 0;
		m_wItemID1 = 0;
		m_wItemID2 = 0;
		m_wItemID3 = 0;
		m_wItemID4 = 0;
		m_wItemID5 = 0;
		m_bCount = 0;
		m_strItem1 = _T("");
		m_strItem2 = _T("");
		m_strItem3 = _T("");
		m_strItem4 = _T("");
		m_strItem5 = _T("");
		m_strPresent = _T("");
		m_strAnnounce = _T("");
		m_strTitle = _T("");
		m_strMessage = _T("");
	}

	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_wID
			<< m_bDay
			<< m_bHour
			<< m_bMin
			<< m_wItemID1
			<< m_wItemID2
			<< m_wItemID3
			<< m_wItemID4
			<< m_wItemID5
			<< m_bCount
			<< m_strPresent
			<< m_strAnnounce
			<< m_strTitle
			<< m_strMessage
			<< m_strItem1
			<< m_strItem2
			<< m_strItem3
			<< m_strItem4
			<< m_strItem5;
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		(*pMsg)
			>> m_wID
			>> m_bDay
			>> m_bHour
			>> m_bMin
			>> m_wItemID1
			>> m_wItemID2
			>> m_wItemID3
			>> m_wItemID4
			>> m_wItemID5
			>> m_bCount
			>> m_strPresent
			>> m_strAnnounce
			>> m_strTitle
			>> m_strMessage
			>> m_strItem1
			>> m_strItem2
			>> m_strItem3
			>> m_strItem4
			>> m_strItem5;			
	}

	void Copy(tagLUCKYEVENT* pLUCKY)
	{
		m_wID			= pLUCKY->m_wID;
		m_bDay			= pLUCKY->m_bDay;
		m_bHour			= pLUCKY->m_bHour;
		m_bMin			= pLUCKY->m_bMin;
		m_wItemID1		= pLUCKY->m_wItemID1;
		m_wItemID2		= pLUCKY->m_wItemID2;
		m_wItemID3		= pLUCKY->m_wItemID3;
		m_wItemID4		= pLUCKY->m_wItemID4;
		m_wItemID5		= pLUCKY->m_wItemID5;
		m_bCount		= pLUCKY->m_bCount;
		m_strItem1		= pLUCKY->m_strItem1;
		m_strItem2		= pLUCKY->m_strItem2;
		m_strItem3		= pLUCKY->m_strItem3;
		m_strItem4		= pLUCKY->m_strItem4;
		m_strItem5		= pLUCKY->m_strItem5;
		m_strPresent	= pLUCKY->m_strPresent;
		m_strAnnounce	= pLUCKY->m_strAnnounce;
		m_strTitle		= pLUCKY->m_strTitle;
		m_strMessage	= pLUCKY->m_strMessage;
	}
};

struct tagTNMTPLAYER
{
	DWORD m_dwCharID;
	BYTE m_bCountry;
	CString m_strName;
	BYTE m_bLevel;
	BYTE m_bClass;
	DWORD m_dwRank;
	DWORD m_dwMonthRank;
	BYTE m_bEntryID;
	DWORD m_dwChiefID;

	CString m_strGuildName;
	BYTE m_bSlotID;
	BYTE m_bResult[MATCH_COUNT];

	MAPTNMTPLAYER m_mapParty;
	MAPDWORD m_mapBatting;
	DWORD m_dwSum;

	tagTNMTPLAYER()
	{
		m_strGuildName = NAME_NULL;
		m_dwChiefID = 0;
		m_bSlotID = TOURNAMENT_SLOT;
		m_bResult[MATCH_QFINAL] = 0;
		m_bResult[MATCH_SFINAL] = 0;
		m_bResult[MATCH_FINAL] = 0;

		m_dwSum = 0;
		m_mapParty.clear();
		m_mapBatting.clear();
	};
};

struct tagTNMTREWARD
{
	BYTE m_bChartType;
	WORD m_wItemID;
	BYTE m_bCount;
	DWORD m_dwClass;
	BYTE m_bCheckShield;
};

struct tagTOURNAMENTENTRY
{
	BYTE m_bEntryID;
	BYTE m_bGroup;
	CString m_strName;
	BYTE m_bType;
	DWORD m_dwClass;
	DWORD m_dwFee;
	DWORD m_dwFeeBack;
	WORD m_wPermitItemID;
	BYTE m_bPermitCount;	
	VTNMTREWARD m_vReward;
	MAPTNMTPLAYER m_map1st;
	MAPTNMTPLAYER m_mapNormal;
	MAPTNMTPLAYER m_mapPlayer;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;

	tagTOURNAMENTENTRY()
	{
		m_bGroup = 0;
	};
};

struct tagTOURNAMENTSTEP
{
	BYTE m_bGroup;
	BYTE m_bStep;
	DWORD m_dwPeriod;

	INT64 m_dStart;
	INT64 m_dEnd;

	tagTOURNAMENTSTEP()
	{
		m_bGroup = 0;
	};
};

struct tagTOURNAMENTSTATUS
{
	WORD m_wID;
	BYTE m_bGroup;
	BYTE m_bStep;
	BYTE m_bSelected;

	BYTE m_bBase;
	DWORD m_dwSum;

	LPMAPTOURNAMENTENTRY m_mapEntry;
	MAPTOURNAMENTSTEP m_mapStep;

	tagTOURNAMENTSTATUS()
	{
		m_wID = 0;
		m_bGroup = 0;
		m_bStep = 0;
		m_bSelected = FALSE;
		m_bBase = 0;
		m_dwSum = 0;

		m_mapEntry = NULL;
		m_mapStep.clear();
	};
};

struct tagTOURNAMENTSCHEDULE
{
	WORD m_wID;
	BYTE m_bEnable;
	LPMAPTOURNAMENTSTEP m_mapStep;
};

struct tagTRPSGAME
{
	BYTE m_bType;
	BYTE m_bWinCount;
	BYTE m_bProb[3];
	WORD m_wWinKeep;
	WORD m_wWinPeriod;
	VINT64 m_vWinDate;
};

struct tagCASTLEWARINFO
{
	WORD m_wID;
	MAPDWORD m_mapGuild;
	MAPARRAY m_mapOccupy;
	MAPDWORD m_mapEnableGuild;
	MAPCASTLETOP3 m_mapTop3[TCONTRY_COUNT];

	DWORD m_dwAtkGuild;
	DWORD m_dwDefGuild;
	BYTE m_bDefCountry;
	WORD m_wCountryPoint[TCONTRY_COUNT];

	tagCASTLEWARINFO()
	{
		m_wID = 0;
		m_mapGuild.clear();
		m_mapOccupy.clear();
		m_mapEnableGuild.clear();
		m_mapTop3[TCONTRY_D].clear();
		m_mapTop3[TCONTRY_C].clear();
		m_dwAtkGuild=0;
		m_dwDefGuild=0;
	};
};

struct tagCASTLETOP3
{
	DWORD m_dwID;
	CString m_strName;
	WORD m_wPoint;
};


struct tagCMGIFT
{
	WORD	m_wGiftID;
	BYTE	m_bGiftType;
	DWORD	m_dwValue;
	BYTE	m_bCount;
	BYTE	m_bTakeType;
	BYTE	m_bMaxTakeCount;
	BYTE	m_bToolOnly;
	WORD	m_wErrGiftID;
	CString m_strTitle;
	CString m_strMsg;
};

///////////////////////////////////////////////////////////////////////////
// TWorldSvr class define

#ifdef __TW_APEX
#include "ApexProxy.h"
#endif

#include "TWorldSession.h"
#include "TServer.h"
#include "TParty.h"
#include "TGuild.h"
#include "TCorps.h"
#include "TMiniDump.h"
