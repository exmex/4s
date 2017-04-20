#pragma once


///////////////////////////////////////////////////////////////////////////
// TLoginSvr structure type

typedef struct tagTGROUP					TGROUP, *LPTGROUP;
typedef struct tagTCHAR						TCHARACTER, *LPTCHARACTER;
typedef struct tagTITEM						TITEM, *LPTITEM;
typedef struct tagTMAGIC					TMAGIC, *LPTMAGIC;
typedef struct tagEVENTINFO					EVENTINFO, *LPEVENTINFO;
typedef struct tagTCASHITEMSALE				TCASHITEMSALE, *LPTCASHITEMSALE;
typedef struct tagMONEVENT					MONEVENT, *LPMONEVENT;
typedef struct tagMONREGEN					MONREGEN,*LPMONREGEN;  
typedef struct tagLOTTERY					TLOTTERY,*LPLOTTERY;
///////////////////////////////////////////////////////////////////////////
// TLoginSvr class type

class CTLoginSession;
class CTUser;


///////////////////////////////////////////////////////////////////////////
// TLoginSvr map type

typedef map< BYTE, LPTGROUP>				MAPTGROUP, *LPMAPTGROUP;
typedef map< DWORD_PTR, CTUser *>			MAPTUSER, *LPMAPTUSER;
typedef map< DWORD,DWORD >					MAPDWORD, *LPMAPDWORD;
typedef map< DWORD , EVENTINFO >			MAPEVENTINFO, *LPMAPEVENTINFO;

typedef vector<LPTCHARACTER>				VTCHAR, *LPVTCHAR;
typedef vector<LPTITEM>						VTITEM, *LPVTITEM;
typedef vector<LPTMAGIC>					VTMAGIC, *LPVTMAGIC;
typedef vector<WORD>						VWORD, *LPVWORD;
typedef vector< TCASHITEMSALE >				VTCASHITEMSALE, *LPVTCASHITEMSALE;
typedef vector<MONREGEN>					VMONREGEN, *LPVMONREGEN;  
typedef vector<TLOTTERY>					VLOTTERY, *LPVLOTTERY;

typedef queue<DWORD>						QTDWORD, LPQTDWORD;

typedef enum TACCOUNT_CHECK
{
	ACHECK_NONE = 0,
	ACHECK_IP,
	ACHECK_BLOCK
};

#define MAX_CHECKKEY						8

///////////////////////////////////////////////////////////////////////////
// TLoginSvr structure define

struct tagTGROUP
{
	CSqlDatabase m_db;
	BYTE m_bID;
	BYTE m_bType;

	CString m_strNAME;
	CString m_strDSN;

	CString m_strDBUser;
	CString m_strPasswd;
	CRITICAL_SECTION m_csGR;

	tagTGROUP()
	{
		InitializeCriticalSectionAndSpinCount(&m_csGR, 4000);
	};

	~tagTGROUP()
	{
		m_db.Close();
		DeleteCriticalSection(&m_csGR);
	};
};

struct tagTCHAR
{
	DWORD m_dwCharID;
	CString m_strName;
	BYTE m_bStartAct;
	BYTE m_bSlot;
	BYTE m_bLevel;
	BYTE m_bClass;
	BYTE m_bRace;
	BYTE m_bCountry;
	BYTE m_bSex;
	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;
	DWORD m_dwRegion;
	CString m_strGuildName;
	DWORD m_dwFame;
	DWORD m_dwFameColor;
	VTITEM	m_vTItem;
};

struct tagTITEM
{
	BYTE	m_bItemID;
	WORD	m_wItemID;
	BYTE	m_bLevel;
	BYTE	m_bGradeEffect;
	WORD	m_wColor;
	BYTE	m_bRegGuild;
};

struct tagTMAGIC
{
	BYTE m_bMagic;
	WORD m_wValue;
	DWORD m_dwTime;
};

struct tagTCASHITEMSALE
{
	WORD m_wID;
	BYTE m_bSaleValue;
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

///////////////////////////////////////////////////////////////////////////
// TLoginSvr class define

#include "TLoginSession.h"
#include "TUser.h"
