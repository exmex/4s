// TQuest.h : header file
//

typedef struct tagTQUESTREWARD			TQUESTREWARD, *LPTQUESTREWARD;
typedef struct tagTQUESTTERM			TQUESTTERM, *LPTQUESTTERM;
typedef struct tagTCONDITION			TCONDITION, *LPTCONDITION;
typedef struct tagTQUEST				TQUEST, *LPTQUEST;
typedef struct tagTQCLASS				TQCLASS, *LPTQCLASS;

typedef vector<LPTQUESTREWARD>			VTQUESTREWARD, *LPVTQUESTREWARD;
typedef vector<LPTQUESTTERM>			VTQUESTTERM, *LPVTQUESTTERM;
typedef vector<LPTCONDITION>			VTCONDITION, *LPVTCONDITION;

typedef map< DWORD, LPTQCLASS>			MAPTQCLASS, *LPMAPTQCLASS;

//
//	Quest type
//
typedef enum __QUESTTYPE
{
	QT_NONE = 0,
	QT_DEFTALK = 1,
	QT_GIVESKILL,
	QT_GIVEITEM,
	QT_DROPITEM,
	QT_SPAWNMON,
	QT_TELEPORT,
	QT_COMPLETE,
	QT_MISSION,
	QT_ROUTING,
	QT_NPCTALK,
	QT_DROPQUEST,
	QT_CHAPTERMSG,
	QT_SWITCH,
	QT_DIEMON,
	QT_DEFENDSKILL,
	QT_COUNT
} QUESTTYPE, *LPQUESTTYPE;

//
//	Trigger type
//
typedef enum __TRIGGERTYPE
{
	TT_EXECQUEST = 1,
	TT_POSITION,
	TT_TALKNPC,
	TT_GETITEM,
	TT_KILLMON
} TRIGGERTYPE, *LPTRIGGERTYPE;

//
// Condition type
//
typedef enum __QUEST_CONDITION_TYPE
{
	QCT_NONE = 0,
	QCT_UPPERLEVEL,
	QCT_LOWERLEVEL,
	QCT_HAVEQUEST,
	QCT_HAVEITEM,
	QCT_CLASS,
	QCT_MONID,
	QCT_MAPID,
	QCT_LEFT,
	QCT_TOP,
	QCT_RIGHT,
	QCT_BOTTOM,
	QCT_PROB,
	QCT_HAVENOITEM,
	QCT_COUNTRY,
	QCT_AFTERQUESTCOMPLETE,
	QCT_SAMELEVEL,
	QCT_SEX,
	QCT_BEFOREQUESTCOMPLETE,
	QCT_COUNTMAX,
	QCT_NOPARENT,
	QCT_MAINTAINSKILL
} QUEST_CONDITION_TYPE, *LPQUEST_CONDITION_TYPE;

typedef enum EQUIP_SLOT
{
	ES_PRMWEAPON = 0,
	ES_SNDWEAPON,
	ES_LONGWEAPON,
	ES_HEAD,
	ES_BACK,
	ES_BODY,
	ES_PANTS,
	ES_FOOT,
	ES_HAND,
	ES_NECK,
	ES_LFINGER,
	ES_RFINGER,
	ES_LEAR,
	ES_REAR,
	ES_FACE,
	ES_COUNT
} *LPEQUIP_SLOT;

//
// Struct
//

struct tagTQCLASS
{
	DWORD m_dwClassID;
	CString m_strNAME;
	BYTE m_bClassMain;
};

struct tagTQUESTREWARD
{
	DWORD m_dwID;

	DWORD m_dwRewardID;
	BYTE m_bRewardType;
	BYTE m_bTakeMethod;
	BYTE m_bTakeData;
	BYTE m_bCount;
	DWORD m_dwQuestMob;
	DWORD m_dwQuestTime;
	DWORD m_dwQuestPathMob;
	DWORD m_dwTicketID;
	BYTE m_bSendQ;
};

struct tagTQUESTTERM
{
	DWORD m_dwID;

	CString m_strObjective;
	CString m_strMessage;

	DWORD m_dwTermID;
	BYTE m_bTermType;
	BYTE m_bCount;
};

struct tagTCONDITION
{
	DWORD m_dwID;

	DWORD m_dwConditionID;
	BYTE m_bConditionType;
	BYTE m_bCount;
};

struct tagTQUEST
{
	CString m_strComplete;
	CString m_strMessage;
	CString m_strAccept;
	CString m_strReject;
	CString m_strSummary;
	CString m_strTitle;			// 01/19 박상연 - 퀘스트 타이틀 옆에 퀘스트 아이디를 붙이기 위해
	CString m_strNPCName;
	CString m_strReply;

	DWORD m_dwTriggerID;
	DWORD m_dwParentID;
	DWORD m_dwQuestID;
	DWORD m_dwClassID;

	BYTE m_bTriggerType;
	BYTE m_bForceRun;
	BYTE m_bCountMax;
	BYTE m_bLevel;
	BYTE m_bType;
	BYTE m_bClassMain;
	BYTE m_bMain;
	BYTE m_bConditionCheck;
	
	VTQUESTREWARD m_vReward;
	VTCONDITION m_vCondition;
	VTQUESTTERM m_vTerm;

	tagTQUEST()
	{
		m_strComplete.Empty();
		m_strMessage.Empty();
		m_strAccept.Empty();
		m_strReject.Empty();
		m_strSummary.Empty();
		m_strNPCName.Empty();
		m_strReply.Empty();

		m_dwTriggerID = 0;
		m_dwParentID = 0;
		m_dwQuestID = 0;

		m_bTriggerType = 0;
		m_bForceRun = 0;
		m_bMain = 0;
		m_bClassMain = 0;
		m_bCountMax = 0;
		m_bLevel = 0;
		m_bType = 0;
		m_bConditionCheck = 0;

		m_vCondition.clear();
		m_vReward.clear();
		m_vTerm.clear();
	};

	~tagTQUEST()
	{
		while(!m_vCondition.empty())
		{
			delete m_vCondition.back();
			m_vCondition.pop_back();
		}

		while(!m_vReward.empty())
		{
			delete m_vReward.back();
			m_vReward.pop_back();
		}

		while(!m_vTerm.empty())
		{
			delete m_vTerm.back();
			m_vTerm.pop_back();
		}
	};
};