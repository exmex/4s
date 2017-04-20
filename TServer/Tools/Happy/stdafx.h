// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0400		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션
#include <afxdisp.h>        // MFC 자동화 클래스

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <queue>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <algorithm>  // 알고리즘 함수 헤더
#include <functional> 

using namespace std;

#include <afxsock.h>
#include <afxdhtml.h>
#include <winsvc.h>

#include "Packet.h"
#include "CTProtocol.h"
#include "NetCode.h"
#include "resource.h"

#define TIMER_CHECK_BEEP				(WM_USER + 1)
#define TIMER_CHECK_SERVICEUPLOAD		(WM_USER + 2)
#define TIMER_CHECK_LOG					(WM_USER + 3)
#define WM_CMGIFTRET					(WM_USER + 4)

#define MAX_SERVICE_STAT_COLUMN			9
#define MAX_MONSPAWN_STAT_COLUMN		6
#define MAX_PATCH_FILE_COLUMN			3
#define FILE_NAME_CONNECT				_T("Happy.log")
#define SIZE_FILESEND					(DEF_PACKET_SIZE-PACKET_HEADER_SIZE)
#define WRITEBUFFERSIZE					(16384)
#define MAX_POSLIST_COLUMN				6
#define MAX_ITEMFIND_COLUMN				3
#define MAX_CHATBANLIST_COLUMN			5
#define MAX_CASTLELIST_COLUMN			6
#define MAX_EVENTLIST_COLUMN			5
#define MAX_LUCKYEVENT_COLUMN			11
#define MAX_TOURNAMENT_COLUMN			11
#define MAX_TOURNAMENTSTEP_COLUMN		4
#define MAX_TCHARINFO_COLUMN			2
#define MAX_TREWARD_COLUMN				6
#define MAX_TOURNAMENTSTEP				14
#define MAX_LOTTERY_COLUMN				4
#define MAX_CMGIFT_COLUMN				12

#define ACTION_AT_DEAD					9

#define PATCH_FOLDER					_T("GamePatch")

#define OPWEB_STR						_T("http://opdev.4story.com/") // KOREA

#define MSG_SUCCESS						_T("SUCCESS")
#define MSG_FAIL						_T("FAIL")

#define MAX_POINT_CNT					1000 // 좌표 배열 갯수
#define WIDTH_LENGTH					5 // 가로 간격
#define NAME_NULL						_T("")
#define SQL_GARBAGE						_T("!@#$");
#define SQL_LIKE(str)					_T("%") + str + _T("%")						

#define SVRGRP_NULL			0
#define SVRGRP_CTLSVR		1
#define SVRGRP_LOGINSVR		2
#define SVRGRP_WORLDSVR		3
#define SVRGRP_MAPSVR		4
#define SVRGRP_PATCHSVR		5
#define SVRGRP_FTPSVR		6
#define SVRGRP_LOG			7
#define SVRGRP_RLYSVR		8

#define STR_WORLD_ALL					_T("WORLD_ALL")
#define TICKCOUNT_MIN					60000

#define BITSHIFTID(a)					((DWORD)(0x00000001 << (a)))

typedef struct tagHAPPYSERVICE			HAPPYSERVICE, *LPHAPPYSERVICE;
typedef struct tagHAPPYMACHINE			HAPPYMACHINE, *LPHAPPYMACHINE;
typedef struct tagHAPPYGROUP			HAPPYGROUP, *LPHAPPYGROUP;
typedef struct tagHAPPYSVRTYPE			HAPPYSVRTYPE, *LPHAPPYSVRTYPE;
typedef struct tagPATCHFILE				PATCHFILE, *LPPATCHFILE;
typedef struct tagPATFILE				TPATFILE, *LPTPATFILE;
typedef struct tagMACHINEPLATFORM		PLATFORM, *LPPLATFORM;
typedef struct tagSERVICEGRAPH			SERVICEGRAPH, *LPSERVICEGRAPH;
typedef struct tagUSERPOS				USERPOS, *LPUSERPOS;
typedef struct tagITEMSTATE				ITEMSTATE, *LPITEMSTATE;
typedef struct tagBANLIST				BANLIST, *LPBANLIST;
typedef struct tagCASTLEINFO			CASTLEINFO, *LPCASTLEINFO;
typedef struct tagEVENTINFO				EVENTINFO, *LPEVENTINFO;
typedef struct tagTCASHITEMSALE			TCASHITEMSALE, *LPTCASHITEMSALE;
typedef struct tagTCASHITEM				TCASHITEM, *LPTCASHITEM;
typedef struct tagMONEVENT				MONEVENT, *LPMONEVENT;
typedef struct tagMONREGEN				MONREGEN,*LPMONREGEN;
typedef struct tagLUCKYEVENT			LUCKYEVENT, *LPLUCKYEVENT;
typedef struct tagTREWARD				TREWARD,*LPTREWARD;
typedef struct tagTSTEPS				TSTEPS, *LPTSTEPS;
typedef struct tagTPVP					TPVP, *LPTPVP;
typedef struct tagTOURNAMENT			TOURNAMENT, *LPTOURNAMENT;
typedef struct tagTNTCHAR				TNTCHAR, *LPTNTCHAR;
typedef struct tagHELPMSG				HELPMSG, *LPHELPMSG;
typedef struct tagLOTTERY				TLOTTERY,*LPLOTTERY;  
typedef struct tagRPSGameData			TRPSGameData, *LPRPSGameData;
typedef struct tagCMGIFT				TCMGIFT, *LPCMGIFT;

typedef set<LPUSERPOS>					SETUSERPOS, *LPSETUSERPOS;
typedef map<BYTE, LPHAPPYMACHINE>		MAPMACHINE, *LPMAPMACHINE;
typedef map<BYTE, LPHAPPYGROUP>			MAPGROUP, *LPMAPGROUP;
typedef map<BYTE, LPHAPPYSVRTYPE>		MAPSVRTYPE, *LPMAPSVRTYPE;
typedef map<DWORD, LPHAPPYSERVICE>		MAPSERVICE, *LPMAPSERVICE;
typedef vector<LPPATCHFILE>				VPATCHFILE, *LPVPATCHFILE;
typedef vector<TPATFILE>				VPATFILE, *LPVPATFILE;
typedef vector<PLATFORM>				VPLATFORM;
typedef map<BYTE, VPLATFORM>			MAPPLATFORM, *LPMAPPLATFORM;
typedef vector<DWORD>					VDWORD;
typedef vector<WORD>					VWORD, *LPVWORD;
typedef map<DWORD, SERVICEGRAPH>		MAPSERVICEGRAPH, *LPMAPSERVICEGRAPH;
typedef vector< ITEMSTATE >				VITEMSTATE, *LPVITEMSTATE;
typedef map<DWORD, LPBANLIST>			MAPBANLIST, *LPMAPBANLIST;
typedef map<WORD, CASTLEINFO >			MAPCASTLEINFO, *LPMAPCASTLEINFO;
typedef map<BYTE, CString >				MAPBYTESTRING, *LPMAPBYTESTRING;
typedef map<DWORD, CString>				MAPDWORDSTRING, *LPMAPDWORDSTRING;
typedef map<DWORD, EVENTINFO>			MAPEVENTINFO, *LPMAPEVENTINFO;
typedef map<WORD, TCASHITEM>			MAPTCASHITEM, *LPMAPTCASHITEM;
typedef vector<TCASHITEMSALE>			VTCASHITEMSALE, *LPVTCASHITEMSALE;
typedef vector<MONREGEN>				VMONREGEN, *LPVMONREGEN;
typedef map<CString,BYTE>				MAPSTRINGBYTE, *LPMAPSTRINGBYTE;
typedef map<WORD, LUCKYEVENT>			MAPLUCKYEVENT, *LPMAPLUCKYEVENT;
typedef vector<CString>					VSTRING, *LPVSTRING;
typedef vector<TREWARD>					VTREWARD, *LPVTREWARD;
typedef vector<TPVP>					VTPVP, *LPVTPVP;
typedef vector<TNTCHAR>					VTNTCHAR, *LPVTNTCHAR;
typedef map<WORD, LPTOURNAMENT>			MAPTOURNAMENT, *LPMAPTOURNAMENT;
typedef map<BYTE , DWORD>				MAPBYTEDWORD, *LPMAPBYTEDWORD;
typedef vector<TLOTTERY>				VLOTTERY, *LPVLOTTERY;
typedef vector<TRPSGameData>			VRPSGameData, *LPVRPSGameData;
typedef map<WORD, TCMGIFT>				MAPCMGIFT, *LPMAPCMGIFT;
typedef deque<TCMGIFT>					QCMGIFT, *LPQCMGIFT;

struct tagHAPPYSERVICE
{
	DWORD m_dwID;
	BYTE m_bID;
	CString m_strName;
	LPHAPPYGROUP m_pGroup;
	LPHAPPYSVRTYPE m_pSvrType;
	LPHAPPYMACHINE m_pMachine;

	DWORD m_dwStatus;
	DWORD m_dwCurSession;
	DWORD m_dwCurUser;
	DWORD m_dwMaxUser;
	DWORD m_dwPing;
	DWORD m_dwStopCount;
	DWORD m_dwActiveUser;
	__int64 m_nStopTime;
	__int64 m_nPickTime;
};

struct tagHAPPYMACHINE
{
	BYTE m_bID;
	CString m_strName;
};

struct tagHAPPYGROUP
{
	BYTE m_bID;
	CString m_strName;
};

struct tagHAPPYSVRTYPE
{
	BYTE m_bType;
	CString m_strName;
};

struct tagPATCHFILE
{
	CString m_strFullFile;
	CString m_strRelFile;
	CString m_strFileName;
	CString m_strPath;
	CString m_strZipFile;
	CString m_strOriFileName; // 확장자를 뺀 파일이름.
	DWORD m_dwSize;
	DWORD m_dwZipSize;
	FILETIME m_time;
};

struct tagPATFILE
{
	DWORD	m_dwVersion;
	CString m_strPath;
	CString m_strName;
	DWORD	m_dwSize;
	BYTE	m_bState;
};

enum {
	_FTP_CONNECT = 4,
	_ERR_TEMPDIR,
	_ERR_CURRENTDIR,
	_ERR_ZIP,
	_ERR_EMPTY,
	_ERR_ADDRORACCOUNT,
	_ERR_THREADFAIL,
	_ERR_DBWRITE
};


typedef enum MANAGER_CLASS
{	
	MANAGER_ALL = 1, // 모두 사용
	MANAGER_CONTROL , // 패치/업로드
	MANAGER_USER , // 공지사항/강제퇴장/위치이동
	MANAGER_SERVICE , // 서비스 ON/OFF	
	MANAGER_GMLEVEL1 , // GMTool 의 모든 기능(캐릭터 메세지,이동,채팅금지,강퇴)
	MANAGER_GMLEVEL2 ,  // GMTool 의 캐릭터 메세지, 이동 기능만.	
	MANAGER_GMLEVEL3 ,
	MANAGER_COUNT

} *LPMANAGER_CLASS;

typedef enum EVENT_RESULT
{
	ER_SUCCESS = 0,
	ER_FAIL,
	ER_NOTFOUNDEVENT,
	ER_RUN
};

typedef enum EVENT_KIND
{
	EK_DEL = 0,
	EK_ADD,
	EK_UPDATE
};

//////////////////////////////////////////////////////////////////////////

struct tagMACHINEPLATFORM
{
    DWORD m_dwCPU;
	DWORD m_dwMEM;
	float m_fNET;
};

struct tagSERVICEGRAPH
{
	DWORD m_dwID;
    CString m_strWorld;
	CString m_strGroup;
	CString m_strService;
	VDWORD m_vCurUser;
	DWORD m_dwPeekUser;
	VDWORD m_vPing;
	BYTE m_bColor;
	LPHAPPYSVRTYPE m_pSvrType;
};

struct tagUSERPOS
{
	CString strName;
	BYTE bChannelNum;
	WORD wMapID;
	float PosX;
	float PosY;
	float PosZ;
};

struct tagITEMSTATE
{
	WORD m_wItemID;
	BYTE m_bInitState;
	CString m_strName;
};

struct tagBANLIST
{
	DWORD m_dwID;
	CString m_strOPName;
	CString m_strBanName;
	__time64_t m_tStartTime;
	WORD	m_wBanTime;
	CString m_strReason;
};

struct tagCASTLEINFO
{
	WORD	m_wCastleID;	
	CString m_strName;
	BYTE	m_bState;
	BYTE	m_bCountry;
	DWORD	m_dwDefGuildID;
	DWORD	m_dwAtkGuildID;
	CString	m_strDefGuild;
	CString m_strAtkGuild;
	__time64_t	m_tNextWar;
};

struct tagTCASHITEMSALE
{
	WORD m_wID;
	BYTE m_bSaleValue;
};

struct tagTCASHITEM
{
	WORD m_wID;
	CString m_strName;
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

struct tagTNTCHAR
{
	BYTE	m_bEventID;
	DWORD	m_dwCharID;
	CString m_strName;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bCountry;
};

struct tagTREWARD
{
	BYTE	m_bChartType;
	WORD	m_wItemID;
	BYTE	m_bCount;
	DWORD	m_dwClass;
	BYTE	m_bShield;

	void Reset()
	{
		m_bChartType = 0;
		m_wItemID = 0;
		m_bCount = 0;
		m_dwClass = 1;
		m_bShield = 0;
	}
	void Copy(LPTREWARD pRE)
	{
		m_bChartType = pRE->m_bChartType;
		m_wItemID = pRE->m_wItemID;
		m_bCount = pRE->m_bCount;
		m_dwClass = pRE->m_dwClass;
		m_bShield = pRE->m_bShield;
	}
};

struct tagTSTEPS
{
	BYTE m_bWeek;
	BYTE m_bDay;
	DWORD m_dwStartTime;
	DWORD m_arStep[MAX_TOURNAMENTSTEP];
	__int64 m_arStart[MAX_TOURNAMENTSTEP];

	void Copy(LPTSTEPS pTS)
	{
		m_bWeek = pTS->m_bWeek;
		m_bDay = pTS->m_bDay;
		m_dwStartTime = pTS->m_dwStartTime;
		memcpy( m_arStep, pTS->m_arStep, sizeof(DWORD) * (int)MAX_TOURNAMENTSTEP );
		memcpy( m_arStart, pTS->m_arStart, sizeof(__int64) * (int)MAX_TOURNAMENTSTEP );
	}

	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_bWeek
			<< m_bDay
			<< m_dwStartTime
			<< (BYTE)(MAX_TOURNAMENTSTEP - 1);

		
		for(BYTE i = 1; i < (BYTE)MAX_TOURNAMENTSTEP; i++)
		{
			(*pMsg)
				<< i
				<< m_arStep[i];		
		}
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		BYTE bCount;
		BYTE bStep;
		DWORD dwTime;
		__int64 dStart;

		(*pMsg)
			>> m_bWeek
			>> m_bDay
			>> m_dwStartTime
			>> bCount;

		for(BYTE i = 1; i <= bCount; i++)
		{
			(*pMsg)
				>> bStep
				>> dwTime
				>> dStart;
            
			m_arStep[bStep] = dwTime;
			m_arStart[bStep] = dStart;
		}
	}
};

struct tagTPVP
{
	BYTE m_bEvent; //종류
	CString m_strName;
	BYTE m_bType; //개인전,파티전
	DWORD m_dwClass;	
	DWORD m_dwFee;
	DWORD m_dwRefund;
	WORD  m_wNeedItem;
	BYTE  m_bNeedCount;
	BYTE  m_bMinLevel;
	BYTE  m_bMaxLevel;
	VTREWARD m_vTReward;
	VTNTCHAR m_vTChar;


	void Reset()
	{
		m_bEvent = 0;
		m_strName = _T("Event");
		m_bType = 1;
		m_dwClass = 1;
		m_dwFee = 0;
		m_dwRefund = 0;
		m_wNeedItem = 0;
		m_bNeedCount = 0;
		m_bMinLevel = 0;
		m_bMaxLevel = 0;
		m_vTReward.clear();
	}

	void Copy(LPTPVP pTPVP)
	{
		m_bEvent = pTPVP->m_bEvent;
		m_strName = pTPVP->m_strName;
		m_bType = pTPVP->m_bType;
		m_dwClass = pTPVP->m_dwClass;
		m_dwFee = pTPVP->m_dwFee;
		m_dwRefund = pTPVP->m_dwRefund;
		m_wNeedItem = pTPVP->m_wNeedItem;
		m_bNeedCount = pTPVP->m_bNeedCount;
		m_bMinLevel = pTPVP->m_bMinLevel;
		m_bMaxLevel = pTPVP->m_bMaxLevel;
		
		for(BYTE i = 0; i < (BYTE)pTPVP->m_vTReward.size(); i++)
		{
			TREWARD stSOURCE = pTPVP->m_vTReward[i];
			TREWARD stDEST;
			stDEST.Copy(&stSOURCE);
			m_vTReward.push_back(stDEST);			
		}

		for(BYTE j = 0; j < (BYTE)pTPVP->m_vTChar.size(); j++ )
		{
			TNTCHAR stTNTCHAR = pTPVP->m_vTChar[j];			
			m_vTChar.push_back(stTNTCHAR);
		}

	}
	
	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_bEvent
			<< m_strName
			<< m_bType
			<< m_dwClass
			<< m_dwFee
			<< m_dwRefund
			<< m_wNeedItem
			<< m_bNeedCount
			<< m_bMinLevel
			<< m_bMaxLevel
			<< (BYTE)m_vTReward.size();


		BYTE n = 0;
		for(n = 0; n < (BYTE)m_vTReward.size(); n++)
		{
			(*pMsg)
				<< m_vTReward[n].m_bChartType
				<< m_vTReward[n].m_wItemID
				<< m_vTReward[n].m_bCount
				<< m_vTReward[n].m_dwClass
				<< m_vTReward[n].m_bShield;
		}
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		BYTE bCount;

		(*pMsg)
			>> m_bEvent
			>> m_strName
			>> m_bType
			>> m_dwClass
			>> m_dwFee
			>> m_dwRefund
			>> m_wNeedItem
			>> m_bNeedCount
			>> m_bMinLevel
			>> m_bMaxLevel
			>> bCount;
		
		for(BYTE n = 0; n < bCount; n++)
		{
			TREWARD stTREWARD;
			(*pMsg)
				>> stTREWARD.m_bChartType
				>> stTREWARD.m_wItemID
				>> stTREWARD.m_bCount
				>> stTREWARD.m_dwClass
				>> stTREWARD.m_bShield;

			m_vTReward.push_back(stTREWARD);
		}

		
		(*pMsg)
			>> bCount;

		for(BYTE m = 0; m < bCount; m++)
		{
			TNTCHAR stTNTCHAR;

			(*pMsg)								
				>> stTNTCHAR.m_dwCharID
				>> stTNTCHAR.m_strName
				>> stTNTCHAR.m_bLevel
				>> stTNTCHAR.m_bClass
				>> stTNTCHAR.m_bCountry;

			stTNTCHAR.m_bEventID = m_bEvent;

			m_vTChar.push_back(stTNTCHAR);
		}		
	}
};

struct tagTOURNAMENT
{
	WORD	m_wID;
	CString m_strTitle;
	TSTEPS	m_stTSTEP;
	VTPVP	m_vTPVP;

	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_wID;			

		m_stTSTEP.WrapPacketIn(pMsg);

		(*pMsg)
			<< (BYTE)m_vTPVP.size();
		for(int i = 0; i < (int)m_vTPVP.size(); i++)
		{
			m_vTPVP[i].WrapPacketIn(pMsg);
		}
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		BYTE bCount = 0;
		BYTE i = 0;

		(*pMsg)
			>> m_wID;
			
		m_stTSTEP.WrapPacketOut(pMsg);

		(*pMsg)
            >> bCount;		
		for( i = 0; i < bCount; i++)
		{			
			TPVP stTPVP;
			stTPVP.WrapPacketOut(pMsg);
			m_vTPVP.push_back(stTPVP);
		}	
	}

	void Copy(LPTOURNAMENT pTNT)
	{
		m_wID = pTNT->m_wID;
		m_strTitle = pTNT->m_strTitle;
		m_stTSTEP.Copy(&(pTNT->m_stTSTEP));
		
		for(BYTE i = 0; i < (BYTE)pTNT->m_vTPVP.size(); i++)
		{
			TPVP stSPVP = pTNT->m_vTPVP[i];
			TPVP stDPVP;
			stDPVP.Copy(&stSPVP);

			m_vTPVP.push_back(stDPVP);
		}
	}
};

struct tagHELPMSG
{
	BYTE		m_bType;
	__time64_t	m_dStartDate;
	__time64_t	m_dEndDate;
	CString		m_strMsg;

	void Reset()
	{
		m_bType		= 0;
		m_dStartDate= 0;
		m_dEndDate	= 0;
		m_strMsg	= _T("");
	}

	void Copy(tagHELPMSG& pHM)
	{
		m_bType		= pHM.m_bType;
		m_dStartDate= pHM.m_dStartDate;
		m_dEndDate	= pHM.m_dEndDate;
		m_strMsg	= pHM.m_strMsg;
	}

	void WrapPacketIn(CPacket* pMsg)
	{
		(*pMsg)
			<< m_bType
			<< m_dStartDate
			<< m_dEndDate
			<< m_strMsg;
	}

	void WrapPacketOut(CPacket* pMsg)
	{
		(*pMsg)
			>> m_bType
			>> m_dStartDate
			>> m_dEndDate
			>> m_strMsg;
	}
};

struct tagRPSGameData
{
	BYTE m_bType;
	BYTE m_bWinCount;
	BYTE m_bProbWin;
	BYTE m_bProbDraw;
	BYTE m_bProbLose;
	WORD m_wWinKeep;
	WORD m_wWinPeriod;
};

struct tagCMGIFT
{
	WORD	m_wGiftID;
	BYTE	m_bGiftType;
	DWORD	m_dwValue;
	BYTE	m_bCount;
	BYTE	m_bTakeType;
	BYTE	m_bMaxTakeCount;
	BYTE	m_bLevel;
	BYTE	m_bToolOnly;
	WORD	m_wErrGiftID;
	CString m_strTitle;
	CString m_strMsg;

	BYTE	m_bUpdate;
};

////////////////////////////////////////////////////////////////////
