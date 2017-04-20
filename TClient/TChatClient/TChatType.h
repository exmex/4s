#pragma once

static const CString	LINK_DEF_TOK;

static const CHAR		LINK_STR_START;
static const CHAR		LINK_STR_END;
static const DWORD		LINK_DATA_POOL_SIZE;
static const DWORD		LINK_QUEST_COLOR;

#define LINK_DEF_TOK						_T("\n\r");
#define LINK_STR_START					'['
#define LINK_STR_END					']'
#define LINK_DATA_POOL_SIZE			100
#define LINK_QUEST_COLOR				0xFFFFFF7F

#define TCHAT_DWORD_SIZE			(8)

#define MBCS_LEAD_TEST(c)				(BYTE(c) & 0x80 ? TRUE : FALSE)

#define ON_TRECEIVE(x)							case x	: nERROR = On##x( pTSocket, pPacket); break;

#define WM_SESSION_MSG							(WM_USER + 0x3000)
#define MAX_CON									(9)
#define KEY_COUNT								((BYTE) 7)

#define T_INVALID						(-1)
#define TTRADEOP_TRADE					0

#define TCHAT_FLAG_WHISPER				((DWORD)(1))
#define TCHAT_FLAG_NEAR					((DWORD)(1 << 1))
#define TCHAT_FLAG_MAP					((DWORD)(1 << 2))
#define TCHAT_FLAG_WORLD				((DWORD)(1 << 3))
#define TCHAT_FLAG_PARTY				((DWORD)(1 << 4))
#define TCHAT_FLAG_GUILD				((DWORD)(1 << 5))
#define TCHAT_FLAG_INFO					((DWORD)(1 << 6))
#define TCHAT_FLAG_FORCE				((DWORD)(1 << 7))
#define TCHAT_FLAG_ALL_CHAT				((DWORD)(TCHAT_FLAG_WHISPER|TCHAT_FLAG_NEAR|TCHAT_FLAG_MAP|TCHAT_FLAG_PARTY|TCHAT_FLAG_GUILD|TCHAT_FLAG_FORCE))
#define TCHAT_FLAG_ALL_CHAT_WITH_INFO	((DWORD)(TCHAT_FLAG_ALL_CHAT | TCHAT_FLAG_INFO))
#define TCHAT_FLAG_COUNT				8
#define TCHAT_MSG_COUNT					80

typedef struct tagTGROUP				TGROUP, *LPTGROUP;
typedef struct tagTITEM					TITEM, *LPTITEM;
typedef struct tagTCHATMSG			TCHATMSG, *LPTCHATMSG;
typedef struct tagTCHANNEL			TCHANNEL, *LPTCHANNEL;
typedef struct tagTCHARACTER			TCHARACTER, *LPTCHARACTER;
typedef struct tagTQUEST				TQUEST, *LPTQUEST;
typedef struct tagTREWARD				TREWARD, *LPTREWARD;
typedef struct tagTTERM					TTERM, *LPTTERM;


class CTChatItem;
class CTChatSocket;
class CTChatInven;

typedef vector<LPTREWARD>								VTREWARDTEMP, *LPVTREWARDTEMP;
typedef vector<LPTTERM>									VTTERMTEMP, *LPVTTERMTEMP;
typedef vector<LPTCHARACTER>				VTCHARACTER, *LPVTCHARACTER;
typedef vector<LPTCHANNEL>					VTCHANNEL, *LPVTCHANNEL;
typedef map< CTChatItem *, DWORD>		MAPTSELLITEM, *LPMAPTSELLITEM;
typedef map< BYTE, CTChatInven *>			MAPTINVEN, *LPMAPTINVEN;
typedef vector<LPTCHATMSG>					VTCHATMSG, *LPVTCHATMSG;
typedef map< SOCKET, CTChatSocket *>		MAPTCHATSOCKET, *LPMAPTCHATSOCKET;
typedef map< WORD, LPTITEM>				MAPTITEMTEMP, *LPMAPTITEMTEMP;
typedef map<DWORD, BYTE>					MAPCHAR, *LPMAPCHAR;
typedef map< BYTE, CTChatItem *>			MAPTITEM, *LPMAPTITEM;
typedef vector<LPTGROUP>						VTGROUP, *LPVTGROUP;

// enum

typedef enum TGRADE_LEVEL
{
	TGRADE_LEVEL_0		= 0,		// Grade 0 ~ 4
	TGRADE_LEVEL_1		= 5,		// Grade 5 ~ 9
	TGRADE_LEVEL_2		= 10,		// Grade 10 ~ 14
	TGRADE_LEVEL_3		= 15,		// Grade 15 ~ 19
	TGRADE_LEVEL_4		= 20,		// Grade 20 ~
	TGRADE_LEVEL_COUNT	= 5
} *LPTGRADE_LEVEL;

typedef enum TMAGIC_LEVEL
{
	TMAGIC_LEVEL_NOR,
	TMAGIC_LEVEL_MAGIC,
	TMAGIC_LEVEL_RARE,
	TMAGIC_LEVEL_COUNT
} *LPTMAGIC_LEVEL;

typedef enum TSKILL_POINT
{
	TPOINT_TAB1 = 0,
	TPOINT_TAB2,
	TPOINT_TAB3,
	TPOINT_TAB4,
	TPOINT_MINE,
	TPOINT_COUNT
} *LPTSKILL_POINT;

typedef enum TTEXT_LINK_TYPE
{
	TTEXT_LINK_TYPE_ITEM,
	TTEXT_LINK_TYPE_QUEST,
	TTEXT_LINK_TYPE_COUNT
} *LPTTEXT_LINK_TYPE;

// struct

struct tagTITEM
{
	WORD	m_wItemID;							///일련번호
	BYTE	m_bType;							///종류
	BYTE	m_bKind;							///구분
	WORD	m_wAttrID;							///성능일련번호
	CString	m_strNAME;							///이름
	WORD	m_wUseValue;						///사용효과값
	DWORD	m_dwSlotID;							///장착위치
	DWORD	m_dwClassID;						///사용직업
	BYTE	m_bPrmSlotID;						///주무기장착위치
	BYTE	m_bSubSlotID;						///보조무기장착위치
	BYTE	m_bLevel;							///필요레벨
	BYTE	m_bCanRepair;						///수리여부(추가)
	DWORD	m_dwDuraMax;						///최대내구(추가)
	BYTE	m_bRefineMax;						///제련횟수(추가)
	FLOAT	m_fPriceRate;						///가격비율
	DWORD	m_dwPrice;							///기준가격
	BYTE	m_bMinRange;						///최소사정거리
	BYTE	m_bMaxRange;						///최대사정거리
	BYTE	m_bStack;							///최대수량
	BYTE	m_bSlotCount;						///슬롯갯수
	BYTE	m_bCanGamble;						///봉인 생성여부
	BYTE	m_bGambleProb;						///치환여부(추가)
	BYTE	m_bDestoryProb;						///소멸여부(추가)
	BYTE	m_bCanGrade;						///등급 가능여부
	BYTE	m_bCanMagic;						///마법 생성여부
	BYTE	m_bCanRare;							///희귀 생성여부
	WORD	m_wDelayGroupID;					///딜레이 그룹 ID
	DWORD	m_dwDelay;							///재사용대기시간
	BYTE	m_bCanTrade;						///거래,판매 가능 여부
	BYTE	m_bIsSpecial;						///캐쉬 아이템 여부
	WORD	m_wUseTime;							///사용 기간(일/시간)
	BYTE m_bUseType;									///사용 타입
	BYTE	m_bWeaponID;						///WEAPON ID
	FLOAT	m_fShotSpeed;						///SHOT SPEED
	FLOAT	m_fGravity;							///GRAVITY
	DWORD	m_dwInfoID;							///INFO ID
	BYTE	m_bSkillItemType;					///발사타입
	WORD	m_wVisual[TGRADE_LEVEL_COUNT];		///단계별 비주얼(추가)
	WORD	m_wGradeSFX;						///등급 이펙트(추가)
	WORD	m_wOptionSFX[TMAGIC_LEVEL_COUNT];	///단계별 옵션 이펙트(추가)
	BYTE	m_bCanWrap; ///밀납가능
	DWORD	m_dwAuctionCode;
	BYTE m_bCanColor;

	tagTITEM()
	{}
};

struct tagTGROUP
{
	CString m_strNAME;

	BYTE m_bGroupID;
	BYTE m_bStatus;
	BYTE m_bCount;
	BYTE m_bType;
};

struct tagTCHATMSG
{
	CString m_strNAME;
	CString m_strMSG;

	DWORD m_dwFlags;
	BYTE m_bType;
};

struct tagTCHANNEL
{
	CString m_strNAME;

	BYTE m_bChannel;
	BYTE m_bStatus;
};

struct tagTCHARACTER
{
	CString m_strNAME;
	DWORD m_dwID;

	BYTE m_bContry;
	BYTE m_bSlotID;
	BYTE m_bLevel;
	BYTE m_bClass;
	BYTE m_bPants;
	BYTE m_bRace;
	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bHand;
	BYTE m_bFoot;
	BYTE m_bSex;
};

struct tagTREWARD
{
	BYTE m_bType;
	DWORD m_dwID;

	BYTE m_bMethod;
	BYTE m_bCount;
	BYTE m_bProb;
};

struct tagTTERM
{
	CString m_strTermMSG;
	CString m_strFailMSG;

	BYTE m_bType;
	DWORD m_dwID;

	BYTE m_bCount;
};

struct tagTQUEST
{
	CString m_strCompleteMSG;
	CString m_strTriggerMSG;
	CString m_strSummaryMSG;
	CString m_strAcceptMSG;
	CString m_strRejectMSG;
	CString m_strNPCName;
	CString m_strReply;
	CString m_strTITLE;

	VTREWARDTEMP m_vTREWARD;
	VTTERMTEMP m_vTTERM;

	DWORD m_dwClassID;
	DWORD m_dwID;

	BYTE m_bLevel;
	BYTE m_bType;
	BYTE m_bMAIN;

	DWORD m_dwMinLevel;
	DWORD m_dwMaxLevel;

	tagTQUEST()
	{
		m_strCompleteMSG.Empty();
		m_strTriggerMSG.Empty();
		m_strSummaryMSG.Empty();
		m_strAcceptMSG.Empty();
		m_strRejectMSG.Empty();
		m_strNPCName.Empty();
		m_strReply.Empty();
		m_strTITLE.Empty();

		m_vTREWARD.clear();
		m_vTTERM.clear();

		m_dwClassID = 0;
		m_dwID = 0;

		m_bLevel = 1;
		m_bType = 0;

		m_bMAIN = FALSE;
		
		m_dwMinLevel = 0;
		m_dwMaxLevel = 0;
	};

	~tagTQUEST()
	{
		while(!m_vTREWARD.empty())
		{
			delete m_vTREWARD.back();
			m_vTREWARD.pop_back();
		}

		while(!m_vTTERM.empty())
		{
			delete m_vTTERM.back();
			m_vTTERM.pop_back();
		}
	};
};


#include "TChatItem.h"
#include "TTextLinkData.h"