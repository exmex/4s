#pragma once


///////////////////////////////////////////////////////////////////////////
// TControlSvr macro

#define	MAX_FILENAME			50
#define	MAX_PATHNAME			100


#define CTRL_TIMER							(1000)
#define GROUP_MANAGER						0xFF
#define IDS_FILENAME_UPLOADTEMP				_T("__ServiceTemp.hpy")
#define IDS_FOLDERNAME_INSTALLED			_T("Services")
///////////////////////////////////////////////////////////////////////////
// TControlSvr structure type
typedef struct tagTSVRTEMP					TSVRTEMP, *LPTSVRTEMP;
typedef struct tagTGROUP					TGROUP, *LPTGROUP;
typedef struct tagTSVRTYPE					TSVRTYPE, *LPTSVRTYPE;
typedef struct tagTMACHINE					TMACHINE, *LPTMACHINE;
typedef struct tagTMANAGER					TMANAGERTEMP, *LPTMANAGERTEMP;
typedef struct tagBANINFO					BANINFO, *LPBANINFO;
typedef struct tagEVENTINFO					EVENTINFO, *LPEVENTINFO;
typedef struct tagTCASHITEMSALE				TCASHITEMSALE, *LPTCASHITEMSALE;
typedef struct tagTCASHITEM					TCASHITEM, *LPTCASHITEM;
typedef struct tagMONEVENT					MONEVENT, *LPMONEVENT;
typedef struct tagMONREGEN					MONREGEN,*LPMONREGEN;  
typedef struct tagLOTTERY					TLOTTERY,*LPLOTTERY;
typedef struct tagPATCHFILE					PATCHFILE, *LPPATCHFILE;
typedef struct tagCMGIFT					TCMGIFT, *LPCMGIFT;
///////////////////////////////////////////////////////////////////////////
// TControlSvr class type

class CTControlSession;
class CTServer;
class CTManager;
class CPlatformUsage;


///////////////////////////////////////////////////////////////////////////
// TControlSvr map type
typedef map< DWORD_PTR, CTServer *>				MAPTSERVER, *LPMAPTSERVER;
typedef map< DWORD_PTR, CTManager *>			MAPTMANAGER, *LPMAPTMANAGER;
typedef map< DWORD, LPTSVRTEMP>					MAPTSVRTEMP, *LPMAPTSVRTEMP;
typedef map< BYTE, LPTGROUP>					MAPTGROUP, *LPMAPTGROUP;
typedef map< BYTE, LPTSVRTYPE>					MAPTSVRTYPE, *LPMAPTSVRTYPE;
typedef map< BYTE, LPTMACHINE>					MAPTMACHINE, *LPMAPTMACHINE;
typedef map< CString, LPTMANAGERTEMP>			MAPTMANAGERTEMP, *LPMAPTMANAGERTEMP;
typedef map< CString, CString>					MAPTSTRING, *LPMAPTSTRING;
typedef map< BYTE, CPlatformUsage *>			MAPPLATFORM, *LPMAPPLATFORM;
typedef map< DWORD, LPBANINFO >					MAPBANINFO, *LPMAPBANINFO;
typedef map< DWORD, EVENTINFO>					MAPEVENTINFO, *LPMAPEVENTINFO;

typedef vector<CString>							VSTRING, *LPVSTRING;
typedef vector<WORD>							VWORD, *LPVWORD;
typedef vector< __int64 >						VINT64, *LPVINT64;
typedef vector< BYTE >							VBYTE, *LPVBYTE;
typedef vector< TCASHITEMSALE >					VTCASHITEMSALE, *LPVTCASHITEMSALE;
typedef vector< TCASHITEM >						VTCASHITEM, *LPVTCASHITEM;
typedef vector<MONREGEN>						VMONREGEN, *LPVMONREGEN;  
typedef vector<TLOTTERY>						VLOTTERY, *LPVLOTTERY;
typedef vector<PATCHFILE>						VPATCHFILE, *LPVPATCHFILE;

///////////////////////////////////////////////////////////////////////////
// TControlSvr structure define

struct tagTSVRTEMP
{
	DWORD m_dwID;
	CString m_strBinPath;
	CString m_strBinName;
	CString m_strName;

	CTServer * m_pConn;
	CString m_strIPAddr;
	WORD m_wPort;

	BYTE m_bServerID;

	LPTGROUP m_pGroup;
	LPTSVRTYPE m_pSvrType;
	LPTMACHINE m_pMachine;
	
	DWORD m_dwStatus;
	DWORD m_dwMaxUser;
	DWORD m_dwActiveUser;
	DWORD m_dwStopCount;
	DWORD m_dwAutoStartTick; 
	__int64 m_nLatestStop;
	__int64 m_nPickTime;
	
	BYTE  m_bManagerControl; //매니저가 Start 시켰는지, Stop 시켰는지를 저장.

	BYTE m_flag; // debug test

	tagTSVRTEMP()
	{
		m_dwID = 0;
		m_dwMaxUser = 0;
		m_dwActiveUser = 0;
		m_nPickTime = 0;
		m_dwStopCount = 0;
		m_nLatestStop = 0;
		m_pGroup = NULL;
		m_pConn = NULL;
		m_pMachine = NULL;
		m_pSvrType = NULL;
		m_flag = FALSE;
	};	
};

struct tagTWORLD
{
	BYTE m_bWorldID;
	CString m_strName;
};

struct tagTGROUP
{
	BYTE m_bGroupID;
	CString m_strName;
};

struct tagTSVRTYPE
{
	BYTE m_bType;
	BYTE m_bGlobal;
	CString m_strName;
};

struct tagTMACHINE
{
	BYTE m_bMachineID;
	CString m_strName;
	BYTE m_bRouteID;
	VSTRING m_vIPAddr;
	VSTRING m_vPriAddr;	
	CString m_strNetwork;
	BYTE m_bClose;	
};


struct tagTMANAGER
{	
	CString m_strPasswd;
	BYTE m_bAuthority;
};

struct tagBANINFO
{
	CString m_strOPName;
	CString m_strBanName;
	CString m_strReason;
	DWORD	m_dwID;	
	WORD	m_wMin;	
	__time64_t m_Time;
};

struct tagTCASHITEM
{
	WORD m_wID;
	CString m_strName;
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

struct tagPATCHFILE
{
	CString m_strPath;
	CString m_strName;
	DWORD m_dwSize;
	DWORD m_dwBetaVer;
};

typedef enum MANAGER_CLASS
{
	MANAGER_ALL = 1,	// 모두 사용
	MANAGER_CONTROL ,	// 패치/업로드
	MANAGER_USER ,		// 공지사항/강제퇴장/위치이동
	MANAGER_SERVICE ,	// 서비스 ON/OFF	
	MANAGER_GMLEVEL1 ,	// GMTool 의 모든 기능
	MANAGER_GMLEVEL2 ,  // GMTool 의 하위기능
	MANAGER_GMLEVEL3 ,
	MANAGER_COUNT

} *LPMANAGER_CLASS;

typedef enum BATTLE_TYPE
{
	BT_LOCAL = 0,
	BT_CASTLE
} *LPBATTLE_TYPE;

typedef enum EVENT_KIND
{
	EK_DEL = 0,
	EK_ADD ,  
	EK_UPDATE 
};

typedef enum EVENT_RESULT
{
	ER_SUCCESS = 0,
	ER_FAIL,
	ER_NOTFOUNDEVENT,
	ER_RUN,
	ER_INVALIDTIME,  
	ER_MAXCOUNT
};

struct tagCMGIFT
{
	WORD m_wGiftID;
	BYTE m_bGiftType;
	DWORD m_dwValue;
	BYTE m_bCount;
	BYTE m_bTakeType;
	BYTE m_bMaxTakeCount;
	BYTE m_bToolOnly;
	WORD m_wErrGiftID;
	CString m_strTitle;
	CString m_strMsg;
};
///////////////////////////////////////////////////////////////////////////
// TControlSvr class define

#include "TControlSession.h"
#include "TServer.h"
#include "TManager.h"
#include "PlatformUsage.h"
#include "TMiniDump.h"
