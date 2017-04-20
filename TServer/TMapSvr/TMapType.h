#pragma once

#include <math.h>

//#define __HACK_SHIELD
//#define __N_PROTECT

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr macro
#define DEFAULT_CHANNEL						(BYTE(1))

#define CASHITEM_CABINET		(16)
#define BASE_STAT				(1)

#define MAX_STR_PARAM			(1024)
#define MAX_MON_SKILL			(4)
#define MAX_SMS_MSG				(80)

#define MAKEINT64(a,b)						(__int64(a) << 32 | (b))
#define LODWORD(l)							((DWORD)((LONG64)(l) & 0xffffffff))
#define HIDWORD(l)							((DWORD)((LONG64)(l) >> 32))

#define INVALID_MAPID						((WORD)0xFFFF)
#define CHAR_SAVE_TICK						30*60*1000

#define DEFAULT_AI							((BYTE) 1)
#define MAP_TIMER							(100)
#define BITSHIFTID(a)						((DWORD)(0x00000001 << (a)))
#define RECOVER_INIT						5000
#define RECOVER_TIME						3000
#define MONKEEP_PER							10

#define ITEMGRADE_COUNT						126
#define ITEMLEVEL_COUNT						50
#define ITEMLEVEL_MAX						24
#define ITEMWEARLEVEL_MAX					34
#define MAX_RECALLMON						5
#define MAX_CHANNEL							((BYTE) 30)

#define AFTERMATH_ATONCE					20
#define AFTERMATH_HELP						12
#define AFTERMATH_GHOST						10
#define AFTERMATH_NONE						0

#define ITEMTRADE_DEAL						1
#define ITEMTRADE_SELL						2
#define ITEMTRADE_CABINET					4

#define OBJECT_SIZE							(1.0f)
#define MAX_ROAMRANGE						64
#define MAX_ROAMHEIGHT						12
#define MAX_ROAMBOUND						64
#define ZONE_HOMESIZE						3
#define ROAM_DELAY_BOUND					4
#define MAX_WALKBOUND						10

#define MAX_TARGET							16

#define PCBANG_TIME							(4 * 60 * 60)
#define PCBANG_PET							102
#define PREMIUM1_PET						101
#define PREMIUM2_PET						103
#define PCBANG_PETNAME						_T("PC방")

#define PCBANG_DROPMAGICPROB				300

#define TMAGIC_OPT_SPEED_RATE				100
#define MONSTER_BASE_SKILL_MIN				700				
#define MONSTER_BASE_SKILL_MAX				712
#define MONSTER_BASE_SKILL_KIND				101

#define RECALL_ITEMABILITY_RATE				0.55

#define RECENTRECORD						10
#define DEFAULT_WARPOWER					(1800)

#define	PORTALCONDITION_COUNT				3
#define ITEMAGIC_BASELEVEL					(70)
#define PVP_RECENTRECORDCOUNT				10
#define PVP_SAVETIME						600

#define AUCTIONSUCCESSFEE					0.03
#define AUCTIONBIDFEE						0.05
#define AUCTIONREGFEE						0.02
#define AUCTIONPAGECOUNT					5
#define AUCTIONTREE1(a)						BYTE(a >> 16)
#define AUCTIONTREE2(a)						BYTE(a >> 8)
#define AUCTIONTREE3(a)						BYTE(a)
#define MAKEAUCTIONCODE(a,b,c)				DWORD( DWORD(a) << 16 | DWORD(b) << 8 | DWORD(c) )
#define TCHAT_CHAR_MAX						(512)
#define TCHAT_DWORD_SIZE					(8)

#define TLOCALBONUS_POINT					(800)
#define MAPID_TOURNAMENT					(501)
#define ARENA_COUNT							(4)
#define TOURNAMENT_PLOUNGE_SPAWNID			(10011)
#define MEETINGROOM_OUTPOS					(15500)

inline FLOAT GetDistance(FLOAT fStartPosX, FLOAT fStartPosZ, FLOAT fEndPosX, FLOAT fEndPosZ)
{
	FLOAT fDistX = fStartPosX - fEndPosX;
	FLOAT fDistZ = fStartPosZ - fEndPosZ;
	return sqrtf(fDistX * fDistX + fDistZ * fDistZ);
}

inline BYTE GetAttackCountry(BYTE bCountry, BYTE bAidCountry)
{
	return bAidCountry != TCONTRY_N ? bAidCountry : bCountry;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr enum

typedef enum LOCAL_TYPE
{
	LOCAL_NONE = 0,
	LOCAL_OCCUPATION,
	LOCAL_CASTLE,
	LOCAL_MISSION
};

typedef enum PROB_TYPE
{
	PROB_NONE = 0,
	PROB_MAGIC,
	PROB_REFINE,
	PROB_TRANS,
	PROB_GAMBLE,
	PROB_UPGRADE,
	PROB_ITEMGUARD,  
	PROB_RAREMAGIC
};

typedef enum LOCAL_LINE
{
	LOCALLINE_NORTH = 0,
	LOCALLINE_SOUTH,
	LOCALLINE_COUNT
};

typedef enum GODBALL_CMD
{
	GB_ENDWAR = 0,
	GB_STARTWAR,
	GB_MOUNTBALL,
	GB_DEMOUNTBALL,
	GB_DROPBALL,
	GB_TAKEBALL,
	GB_COUNT
};

typedef enum SDELAY_TYPE
{
	SDELAY_SKILL,
	SDELAY_LOOP,
	SDELAY_KIND
};

typedef enum GATE_TYPE
{
	GT_ONESWITCH = 0,
	GT_SELECTSWITCH,
	GT_MULTISWITCH
};

typedef enum LOG_ITEM
{
	LI_INS_MON = 0,
	LI_INS_BUY,
	LI_INS_NPC,
	LI_INS_QUEST,
	LI_INS_DEAL,
	LI_INS_POST,
	LI_INS_STORAGE,
	LI_DEL_DROP,
	LI_DEL_SELL,
	LI_DEL_QUEST,
	LI_DEL_DEAL,
	LI_DEL_POST,
	LI_DEL_UPGRADE,
	LI_DEL_STORAGE,
	LI_DEL_USE
};

typedef enum DEAL_STATUS
{
	DEAL_READY = 0,
	DEAL_WAIT,
	DEAL_START,
	DEAL_ADDITEM,
	DEAL_CONFORM
} *LPDEAL_STATUS;

typedef enum SWITCH_CONTROL
{
	SWC_TOGGLE = 0,
	SWC_OPEN,
	SWC_CLOSE
} *LPSWITCH_CONTROL;

typedef enum SPAWN_TYPE
{
	SPAWN_NORMAL = 0,
	SPAWN_ATTACK,
	SPAWN_DEFEND
} *LPSPAWN_TYPE;

typedef enum RV_TYPE
{
	RV_PHYSIC,
	RV_MAGIC,
	RV_PHYSICPROB,
	RV_MAGICPROB,
	RV_COUNT
} *LPRV_TYPE;

typedef enum MAP_TYPE
{
	MAP_WORLD = 0,
	MAP_INDUNTEMP,
	MAP_INDUN
} *LPMAP_TYPE;

typedef enum ITEM_SAVE
{
	ITEM_SAVE_NO = 1,
	ITEM_SAVE_UPDATE = 2,
	ITEM_SAVE_INSERT = 4,
	ITEM_SAVE_DELETE = 8,
	ITEM_SAVE_LOAD = 16
} *LPITEM_SAVE;

typedef enum POST_SAVE
{
	POST_SAVE_NO = 0,
	POST_SAVE_READ
} *LPPOST_SAVE;

typedef enum AI_TRIGGER
{
	AT_AICOMPLETE = 0,
	AT_ENTERLB,
	AT_LEAVELB,
	AT_ENTERAB,
	AT_LEAVEAB,
	AT_DEFEND,
	AT_DELETE,
	AT_ENTER,
	AT_LEAVE,
	AT_DEAD,
	AT_TIMEOUT,
	AT_ATHOME,
	AT_HELP,
	AT_LOTTERY,
	AT_COUNT
} *LPAI_TRIGGER;

typedef enum AI_COMMAND
{
	AC_REGEN = 0,
	AC_LEAVE,
	AC_SETHOST,
	AC_CHKHOST,
	AC_CHGHOST,
	AC_CHGMODE,
	AC_BEGINATK,
	AC_ATTACK,
	AC_FOLLOW,
	AC_GETAWAY,
	AC_REFILL,
	AC_ROAM,
	AC_REMOVE,
	AC_GOHOME,
	AC_LOTTERY,
	AC_COUNT
} *LPAI_COMMAND;

typedef enum AI_CONDITION
{
	AN_PROB = 0,
	AN_MODE,
	AN_CHGHOST,
	AN_COUNT
} *LPAI_CONDITION;

typedef enum UNIT_DIR
{
	UD_LEFT_TOP = 0,
	UD_TOP,
	UD_RIGHT_TOP,
	UD_LEFT,
	UD_RIGHT,
	UD_LEFT_BOTTOM,
	UD_BOTTOM,
	UD_RIGHT_BOTTOM,
	UD_COUNT
} *LPUNIT_DIR;

typedef enum OBJ_STATUS
{
	OS_DISAPPEAR = 0,
	OS_WAKEUP,
	OS_SLEEP,
	OS_DEAD,
	OS_COUNT
} *LPOBJ_STATUS;

typedef enum SKILL_ATTACK_TYPE
{
	SAT_NONE,
	SAT_PHYSIC,
	SAT_LONG,
	SAT_MAGIC,
	SAT_MAGICLONG
} *LPSKILL_ATTACK_TYPE;

typedef enum SKILL_MAINTAIN_TYPE
{
	SMT_PHYSIC = 1,
	SMT_MAGIC
} *LPSKILL_MAINTAIN_TYPE;

typedef enum SPAWN_EVENT
{
	SE_DEFAULT = 0,
	SE_LOCALOCCUPY,
	SE_QUEST,
	SE_QUESTDEL,
	SE_DYNAMIC
} *LPSPAWN_EVENT;

typedef enum REGEN_TYPE
{
	RT_ETERNAL = 0,
	RT_WAITSPAWN,
	RT_TIMELIMIT
} *LPREGEN_TYPE;

typedef enum MONSPAWN_STATUS
{
	MONSPAWN_READY = 0,
	MONSPAWN_TIMEOUT,
	MONSPAWN_SUSPEND,
	MONSPAWN_DELETE
} *LPSPAWN_STATUS;

typedef enum MONITEM_PROB
{
	MIP_NORMAL1,
	MIP_NORMAL2,
	MIP_NORMAL3,
	MIP_NORMAL4,
	MIP_MAGIC,
	MIP_SET,
	MIP_RARE,
	MIP_COUNT
} *LPMONITEM_PROB;

typedef enum SKILLPERFORM_TYPE
{
	PERFORM_SUCCESS = 0,
	PERFORM_MISS,
	PERFORM_FAIL
} *LPPERFORM_TYPE;

typedef enum DUEL_TYPE
{
	DUEL_NONE = 0,
	DUEL_STANDBY,
	DUEL_START,
	DUEL_END
} *LPDUEL_TYPE;

typedef enum CHAR_ABILITY
{
	ABILITY_STR = 0,	//힘
	ABILITY_DEX,		//민첩
	ABILITY_CON,		//체력
	ABILITY_INT,		//지능
	ABILITY_WIS,		//지혜
	ABILITY_MEN,		//정신
	ABILITY_MAXAP,		//최대근거리공격력
	ABILITY_MAXLAP,		//최대원거리공격력
	ABILITY_MINAP,		//최소근거리공격력
	ABILITY_MINLAP,		//최소원거리공격력
	ABILITY_MAXMAP,		//최대마법공격력
	ABILITY_MINMAP,		//최소마법공격력
	ABILITY_MAXHP,		//최대생명력
	ABILITY_MAXMP,		//최대정신력
	ABILITY_PDP,		//물리방어력
	ABILITY_MDP,		//마법방어력
	ABILITY_AL,			//공격등급
	ABILITY_DL,			//방어등급
	ABILITY_MAL,		//마법공격등급
	ABILITY_MDL,		//마법방어등급
	ABILITY_PCR,		//근거리치명확률
	ABILITY_MCR,		//마법치명확률
	ABILITY_CMP,		//시전유지확률
	ABILITY_SDR,		//방패방어확률
	ABILITY_SMDR,		//마법방패방어확률
	ABILITY_SDP,		//방패방어력
	ABILITY_SMDP,		//마법방패방어력
	ABILITY_MCS,		//시전속도
	ABILITY_HPR,		//생명력회복률
	ABILITY_MPR,		//정신력회복률
	ABILITY_AVP,		//회피확률
	ABILITY_ATKSPEED_S,	//근거리공격속도
	ABILITY_ATKSPEED_L,	//원거리공격속도
	ABILITY_ATKSPEED_SR,//근거리공격속도비율
	ABILITY_ATKSPEED_LR,//원거리공격속도비율
	ABILITY_ATKSPEED_MR,//마법공격속도비율,
	ABILITY_AFTERMATH	//부활후유증
} *LPCHAR_ABILITY;

typedef enum DISCOUNTCONDITION_TYPE 
{
	DCC_NONE = 0,
	DCC_GUILD,
	DCC_LOCALHERO,
	DCC_CASTLEHERO,
	DCC_ALLCOUNTRY
};

typedef enum PORTALCONDITION_TYPE
{
	PCT_NONE = 0,
	PCT_COUNTRY,
	PCT_HAVEITEM,
	PCT_ATTACKPOS,
	PCT_DEFENDPOS,
	PCT_DOWNLEVEL,
	PCT_UPLEVEL,
	PCT_UPDOWNLEVEL,
	PCT_GUILD,
	PCT_TOURNAMENTPLAYER,
	PCT_TOURNAMENTLOUNGE,
	PCT_TOURNAMENTGUEST,
	PCT_MEETING
};

typedef enum MONITEMCHART_TYPE
{
	MIC_QUESTITEMCHART = 0,
	MIC_ITEMCHART ,
	MIC_CRAFTITEM
};

typedef enum BUFFERASEACTION_TYPE
{
	BEA_NONE	= 0,
	BEA_ATTACK,
	BEA_DEFEND,
	BEA_RIDE
};

typedef enum ITEMMAKE_TYPE
{
	IMT_NONE = 0,
	IMT_SCROLL,
	IMT_REFINE,
	IMT_GAMBLE
};


typedef enum TTEXT_LINK_TYPE
{
	TTEXT_LINK_TYPE_ITEM_OLD,
	TTEXT_LINK_TYPE_QUEST,
	TTEXT_LINK_TYPE_ITEM,
	TTEXT_LINK_TYPE_COUNT
} *LPTTEXT_LINK_TYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr structure type

typedef struct tagTMAGICSKILL				TMAGICSKILL, *LPTMAGICSKILL;
typedef struct tagTAICONDITION				TAICONDITION, *LPTAICONDITION;
typedef struct tagTAICOMMAND				TAICOMMAND, *LPTAICOMMAND;
typedef struct tagTINVENDESC				TINVENDESC, *LPTINVENDESC;
typedef struct tagTMONSPAWN					TMONSPAWN, *LPTMONSPAWN;
typedef struct tagTMONSTER					TMONSTER, *LPTMONSTER;
typedef struct tagTMONATTR					TMONATTR, *LPTMONATTR;
typedef struct tagTMAPMON					TMAPMON, *LPTMAPMON;
typedef struct tagTAIBUF					TAIBUF, *LPTAIBUF;
typedef struct tagTMAGIC					TMAGIC, *LPTMAGIC;
typedef struct tagTLEVEL					TLEVEL, *LPTLEVEL;
typedef struct tagTSKILLDATA				TSKILLDATA, *LPTSKILLDATA;
typedef struct tagTSTAT						TSTAT, *LPTSTAT;
typedef struct tagTITEM						TITEM, *LPTITEM;
typedef struct tagTSPAWNPOS					TSPAWNPOS, *LPTSPAWNPOS;
typedef struct tagGODBALLBUF				TGBBUF, *LPTGBBUF;

typedef struct tagQUESTCONDITION			QUESTCONDITION, *LPQUESTCONDITION;
typedef struct tagQUESTREWARD				QUESTREWARD,	*LPQUESTREWARD;
typedef struct tagQUESTTERM					QUESTTERM,		*LPQUESTTERM;
typedef struct tagQUESTTEMP					QUESTTEMP,		*LPQUESTTEMP;

typedef struct tagPARTYEXP					PARTYEXP,	*LPPARTYEXP;
typedef struct tagTMONITEM					TMONITEM, *LPTMONITEM;
typedef struct tagPARTYMEMBER				PARTYMEMBER, *LPPARTYMEMBER;

typedef struct tagHOTKEY					THOTKEY, *LPTHOTKEY;
typedef struct tagHOTKEYINVEN				THOTKEYINVEN, *LPTHOTKEYINVEN;

typedef struct tagBATTLEZONE				TBATTLEZONE, *LPTBATTLEZONE;
typedef struct tagLOCAL						TLOCAL, *LPTLOCAL;
typedef struct tagITEMATTR					TITEMATTR, *LPTITEMATTR;
typedef struct tagITEMGRADE					TITEMGRADE, *LPTITEMGRADE;
typedef struct tagITEMMAGIC					TITEMMAGIC, *LPTITEMMAGIC;
typedef struct tagITEMSET					TITEMSET, *LPTITEMSET;
typedef struct tagPATHPOINT					TPATHPOINT, *LPTPATHPOINT;

typedef struct tagDESTINATION				TDESTINATION, *LPTDESTINATION;
typedef struct tagPORTAL					TPORTAL, *LPTPORTAL;
typedef struct tagAFTERMATH					TAFTERMATH, *LPTAFTERMATH;
typedef struct tagAGGRO						TAGGRO, *LPTAGGRO;

typedef struct tagSWITCH					TSWITCH, *LPTSWITCH;
typedef struct tagGATE						TGATE, *LPTGATE;
typedef struct tagSELFOBJ					TSELFOBJ, * LPTSELFOBJ;
typedef struct tagSWITCHOBJ					TSWITCHOBJ, *LPTSWITCHOBJ;

typedef struct tagDEALITEM					TDEALITEM, *LPTDEALITEM;
typedef struct tagPOST						TPOST, *LPTPOST;
typedef struct tagSTOREITEM					TSTOREITEM, *LPTSTOREITEM;
typedef struct tagCABINET					TCABINET, *LPTCABINET;

typedef struct tagPETTEMP					TPETTEMP, *LPTPETTEMP;
typedef struct tagPET						TPET, *LPTPET;

typedef struct tagDUEL						TDUEL, *LPTDUEL;

typedef struct tagFORMULA					TFORMULA, *LPTFORMULA;
typedef struct tagCASHITEM					TCASHITEM, *LPTCASHITEM;
typedef struct tagGAMBLE					TGAMBLE, *LPTGAMBLE;
typedef struct tagCASHCATEGORY				TCASHCATEGORY, *LPTCASHCATEGORY;

typedef struct tagGODTOWER					TGODTOWER, *LPTGODTOWER;
typedef struct tagGODBALL					TGODBALL, *LPTGODBALL;
typedef struct tagTDURINGITEM				TDURINGITEM, *LPTDURINGITEM;
typedef struct tagTINDURN					TINDURN, *LPTINDURN;
typedef struct tagTITEMQUERY				TITEMQUERY, *LPTITEMQUERY;
typedef struct tagSKILLPOINT				TSKILLPOINT, *LPTSKILLPOINT;
typedef struct tagONTIMEMON					TONTIMEMON, *LPTONTIMEMON;
typedef struct tagPVPOINT					TPVPOINT, *LPTPVPOINT;
typedef struct tagENTRYRECORD				TENTRYRECORD, *LPTENTRYRECORD;
typedef struct tagRECORDSET					TRECORDSET, *LPTRECORDSET;
typedef struct tagMONSTERSHOP				TMONSTERSHOP, *LPTMONSTERSHOP;
typedef struct tagPROTECTED					TPROTECTED, *LPTPROTECTED;
typedef struct tagCASHGAMBLE				TCASHGAMBLE, *LPTCASHGAMBLE;
typedef struct tagTCASHITMSALE				TCASHITEMSALE, *LPTCASHITEMSALE;  
typedef struct tagMONEVENT					MONEVENT, *LPMONEVENT;  
typedef struct tagEVENTINFO					EVENTINFO, *LPEVENTINFO;
typedef struct tagTAUCTION					TAUCTION, *LPTAUCTION;
typedef struct tagTAUCTIONINTEREST			TAUCTIONINTEREST, *LPTAUCTIONINTEREST;
typedef struct tagTAUCTIONBIDDER			TAUCTIONBIDDER, *LPTAUCTIONBIDDER;
typedef struct tagTAUCTIONBUF				TAUCTIONBUF, *LPTAUCTIONBUF; 
typedef struct tagMONTHRANKER				MONTHRANKER, *LPMONTHRANKER;
typedef struct tagMONREGEN					MONREGEN,*LPMONREGEN;
typedef struct tagTOURNAMENTPLAYER			TOURNAMENTPLAYER, *LPTOURNAMENTPLAYER;
typedef struct tagTOURNAMENT				TOURNAMENT, *LPTOURNAMENT;
typedef struct tagTNMTREWARD				TNMTREWARD, *LPTNMTREWARD;
typedef struct tagTOURNAMETTIME				TOURNAMETTIME, *LPTOURNAMETTIME;
typedef struct tagHELPMESSAGE				THELPMESSAGE, *LPHELPMESSAGE;
typedef struct tagLOTTERY					TLOTTERY,*LPLOTTERY;
typedef struct tagRPSGAME					TRPSGAME, *LPRPSGAME;
typedef struct tagLOCALSUMPOINT				TLOCALSUMPOINT, *LPLOCALSUMPOINT;
typedef struct tagARENA						TARENA, *LPTARENA;
typedef struct tagTCHATLOG					TCHATLOG, *LPTCHATLOG;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr class type

class CTMapSession;
class CTAICommand;
class CTAICmdBeginAtk;
class CTAICmdSetHost;
class CTAICmdChkHost;
class CTAICmdChgHost;
class CTAICmdChgMode;
class CTAICmdGetaway;
class CTAICmdAttack;
class CTAICmdFollow;
class CTAICmdRefill;
class CTAICmdLeave;
class CTAICmdRegen;
class CTAICmdRoam;
class CTAICmdRemove;
class CTAICmdGohome;
class CTAICmdLottery;
class CTMonsterAI;
class CTMonSpawn;
class CTMonster;
class CTChannel;
class CTObjBase;
class CTPlayer;
class CTSkillTemp;
class CTSkill;
class CTInven;
class CTItem;
class CTCell;
class CTMap;
class CQuest;
class CQuestComplete;
class CQuestDefaultTalk;
class CQuestDropItem;
class CQuestGiveItem;
class CQuestGiveSkill;
class CQuestMission;
class CQuestSpawnMon;
class CQuestTeleport;
class CQuestRouting;
class CQuestTalk;
class CQuestDrop;
class CQuestChapterMsg;
class CQuestSwitch;
class CQuestDieMon;
class CQuestDefendSkill;
class CQuestDeleteItem;
class CQuestSendPost;
class CQuestCraft;
class CQuestRegen;
class CTNpc;
class CTRecallMon;
class CTSelfObj;
class CTTextLinker;
class CTTextLinkData;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr vector type

typedef vector<LPTMAGICSKILL>				VTMAGICSKILL, *LPVTMAGICSKILL;
typedef vector<LPTAICONDITION>				VTAICONDITION, *LPVTAICONDITION;
typedef vector<CTAICommand *>				VTAICOMMAND, *LPVTAICOMMAND;
typedef vector<CTMonster *>					VTMONSTER, *LPVTMONSTER;
typedef vector<CTPlayer *>					VPLAYER, *LPVPLAYER;
typedef vector<CTObjBase *>					VOBJECT, *LPVOBJECT;
typedef vector<CTItem *>					VTITEM, *LPVTITEM;
typedef vector<CTSkill *>					VTSKILL, *LPVTSKILL;
typedef vector<LPTSKILLDATA>				VTSKILLDATA, *LPVTSKILLDATA;

typedef vector<LPTINVENDESC>				VTINVENDESC, *LPVTINVENDESC;
typedef vector<LPTMAPMON>					VTMAPMON, *LPVTMAPMON;
typedef vector<LPTAIBUF>					VTAIBUF, *LPVTAIBUF;
typedef vector<LPTGBBUF>					VTGBBUF, *LPVTGBBUF;

typedef vector<DWORD>						VDWORD, *LPVDWORD;
typedef vector<WORD>						VWORD, *LPVWORD;
typedef vector<BYTE>						VBYTE, *LPVBYTE;
typedef vector<CString>						VSTRING, *LPVSTRING;
typedef vector<INT64>						VINT64, *LPVINT64;

typedef vector<LPQUESTCONDITION>			VQUESTCONDITION, *LPVQUESTCONDITION;
typedef vector<LPQUESTREWARD>				VQUESTREWARD, *LPVQUESTREWARD;
typedef vector<LPQUESTTERM>					VQUESTTERM, *LPVQUESTTERM;
typedef vector<LPQUESTTEMP>					VQUESTTEMP, *LPVQUESTTEMP;
typedef vector<CQuest *>					VQUEST, *LPVQUEST;
typedef vector<LPTMONITEM>					VTMONITEM, *LPVTMONITEM;
typedef vector<LPPARTYMEMBER>				VPARTYMEMBER, *LPVPARTYMEMBER;
typedef vector<LPTITEMMAGIC>				VTITEMMAGIC, *LPVTITEMMAGIC;
typedef vector<LPTITEMSET>					VTITEMSET, *LPVTITEMSET;
typedef vector<LPTPATHPOINT>				VPATHPOINT, *LPVPATHPOINT;
typedef vector<LPTMONSPAWN>					VTMONSPAWN, *LPVTMONSPAWN;
typedef vector<LPTGATE>						VTGATE, *LPVTGATE;
typedef vector<LPTSWITCH>					VTSWITCH, *LPVTSWITCH;
typedef vector<TSELFOBJ>					VTSELFOBJ, *LPVTSELFOBJ;
typedef vector<TSWITCHOBJ>					VTSWITCHOBJ, *LPVTSWITCHOBJ;
typedef vector<LPTGAMBLE>					VTGAMBLE, *LPVTGAMBLE;
typedef vector<LPTMAGIC>					VTMAGIC, *LPVTMAGIC;
typedef vector<LPTITEM>						VTITEMTEMP, *LPVTITEMTEMP;
typedef vector<LPTCASHCATEGORY>				VTCASHCATEGORY, *LPVTCASHCATEGORY;
typedef vector<LPTCASHITEM>					VTCASHITEM, *LPVTCASHITEM;
typedef vector<TRECORDSET>					VTRECORDSET, *LPVTRECORDSET;
typedef vector<LPTCASHGAMBLE>				VTCASHGAMBLE, *LPVTCASHGAMBLE;
typedef vector<TCASHITEMSALE>				VTCASHITEMSALE, *LPVTCASHITEMSALE;  
typedef vector<LPTAUCTIONBUF>				VTAUCTIONBUF, *LPVTAUCTIONBUF;
typedef vector<LPTAUCTION>					VTAUCTION, *LPVTAUCTION;
typedef vector<LPTLOCAL>					VTLOCAL, *LPVTLOCAL;
typedef vector<MONTHRANKER>					VMONTHRANKER, *LPVMONTHRANKER;  
typedef vector<MONREGEN>					VMONREGEN, *LPVMONREGEN;  
typedef vector<TNMTREWARD>					VTNMTREWARD, *LPVTNMTREWARD;
typedef vector<CTMap *>						VTMAP, *LPVTMAP;
typedef vector<TLOTTERY>					VLOTTERY, *LPVLOTTERY;
typedef vector<TLOCALSUMPOINT>				VLOCALSUMPOINT, *LPVLOCALSUMPOINT;
typedef vector< TCHATLOG >					VCHATLOG, *LPVCHATLOG;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr map type

typedef map<WORD, VTMAGICSKILL>				MAPTMAGICSKILL, *LPMAPTMAGICSKILL;
typedef map< CString, CTPlayer *>			MAPPLAYERNAME, *LPMAPPLAYERNAME;
typedef map< DWORD_PTR, CTPlayer *>			MAPPLAYER, *LPMAPPLAYER;
typedef map< DWORD, CTCell *>				MAPTCELL, *LPMAPTCELL;

typedef map< BYTE, LPTMAGIC>				MAPTMAGIC, *LPMAPTMAGIC;
typedef map< BYTE, LPTLEVEL>				MAPTLEVEL, *LPMAPTLEVEL;
typedef map< BYTE, LPTSTAT>					MAPTSTAT, *LPMAPTSTAT;

typedef map< DWORD, CTMonster *>			MAPTMONSTER, *LPMAPTMONSTER;
typedef map< DWORD, CTRecallMon *>			MAPTRECALLMON, *LPMAPTRECALLMON;
typedef map< WORD, CTMonSpawn *>			MAPTMONSPAWN, *LPMAPTMONSPAWN;
typedef map< DWORD, LPTMONATTR >			MAPTMONATTR, *LPMAPTMONATTR;
typedef map< WORD, LPTMONSPAWN>				MAPTMONSPAWNTEMP, *LPMAPTMONSPAWNTEMP;
typedef map< WORD, LPTMONSTER>				MAPTMONSTERTEMP, *LPMAPTMONSTERTEMP;
typedef map< WORD, LPTITEM>					MAPTITEMTEMP, *LPMAPTITEMTEMP;

typedef map< BYTE, CTInven *>				MAPTINVEN, *LPMAPTINVEN;
typedef map< BYTE, CTItem *>				MAPTITEM, *LPMAPTITEM;
typedef map< WORD, CTSkillTemp *>			MAPTSKILLTEMP, *LPMAPTSKILLTEMP;
typedef map< WORD, CTSkill *>				MAPTSKILL, *LPMAPTSKILL;
typedef map< DWORD, CTItem *>				MAPTSTORAGEITEM, *LPMAPTSTORAGEITEM;
typedef map< WORD, CTNpc *>					MAPTNPC, *LPMAPTNPC;

typedef map< WORD, CTMap *>					MAPTMAP, *LPMAPTMAP;
typedef map< WORD, WORD>					MAPWORD, *LPMAPWORD;
typedef map< BYTE, BYTE>					MAPBYTE, *LPMAPBYTE;
typedef map< DWORD, DWORD>					MAPDWORD, *LPMAPDWORD;
typedef map< WORD, DWORD>					MAPWDWORD, *LPMAPWDWORD;
typedef map< WORD, LPVDWORD>				MAPVDWORD, *LPMAPVDWORD;
typedef map< __int64, __int64>				MAPINT64, *LPMAPINT64;
typedef map< DWORD, __int64>				MAPDWORD64, *LPMAPDWORD64;
typedef map< WORD, MAPDWORD >				MAPMAPDWORD, *LPMAPMAPDWORD;
typedef map< WORD, MAPWORD>					MAPMAPWORD, *LPMAPMAPWORD;
typedef map< DWORD, CTChannel *>			MAPTCHANNEL, *LPMAPTCHANNEL;

typedef map< DWORD, LPVTAICOMMAND>			MAPVTAICOMMAND, *LPMAPVTAICOMMAND;
typedef map< DWORD, LPTAICOMMAND>			MAPTAICMDTEMP, *LPMAPTAICMDTEMP;
typedef map< BYTE, CTMonsterAI *>			MAPTMONSTERAI, *LPMAPTMONSTERAI;

typedef map< DWORD, LPQUESTTEMP		>		MAPQUESTTEMP,		*LPMAPQUESTTEMP;
typedef map< DWORD, LPVQUESTTEMP	>		MAPVQUESTTEMP,		*LPMAPVQUESTTEMP;
typedef map< BYTE,	LPMAPVQUESTTEMP	>		MAPMAPVQUESTTEMP,	*LPMAPMAPVQUESTTEMP;
typedef map< DWORD, CQuest *>				MAPQUEST, *LPMAPQUEST;
typedef map< WORD, LPPARTYEXP>				MAPPARTYEXP, *LPMAPPARTYEXP;
typedef map< WORD, LPTSPAWNPOS>				MAPTSPAWNPOS, *LPMAPTSPAWNPOS;

typedef map< BYTE, LPTHOTKEYINVEN>			MAPTHOTKEYINVEN, *LPMAPTHOTKEYINVEN;
typedef map< WORD, LPTBATTLEZONE>			MAPTBATTLEZONE, *LPMAPTBATTLEZONE;
typedef map< WORD, LPTLOCAL>				MAPTLOCAL, *LPMAPTLOCAL;
typedef map< BYTE, LPTITEMMAGIC>			MAPTITEMMAGIC, *LPMAPTITEMMAGIC;
typedef map< WORD, LPTITEMATTR>				MAPTITEMATTR, *LPMAPTITEMATTR;
typedef map< WORD, LPVTMONSPAWN>			MAPVMONSPAWN, *LPMAPVMONSPAWN;

typedef map< WORD, LPTPORTAL>				MAPTPORTAL, *LPMAPTPORTAL;
typedef map< WORD, LPTDESTINATION>			MAPTDESTINATION, *LPMAPTDESTINATION;
typedef map< __int64, TAGGRO>				MAPTAGGRO, *LPMAPTAGGRO;

typedef map< DWORD, LPTSWITCH>				MAPTSWITCH, *LPMAPTSWITCH;
typedef map< DWORD, LPTGATE>				MAPTGATE, *LPMAPTGATE;
typedef map< DWORD, CTSelfObj *>			MAPTSELFOBJ, *LPTMAPTSELFOBJ;

typedef map< DWORD, CString>				MAPTSTRING, *LPMAPTSTRING;
typedef map< DWORD, LPTPOST>				MAPTPOST, *LPMAPTPOST;
typedef map< DWORD, MAPDWORD64>				MAPTPOSTBILLS, *LPMAPTPOSTBILLS;
typedef map< DWORD, VDWORD>					MAPDVDWORD, *LPMAPDVDWORD;

typedef map< BYTE, LPTCABINET>				MAPTCABINET, *LPMAPTCABINET;

typedef map< BYTE, LPTSTOREITEM>			MAPTSTOREITEM, *LPMAPTSTOREITEM;

typedef map< BYTE, WORD>					MAPBWORD, *LPMAPBWORD;
typedef map< WORD, LPTPETTEMP>				MAPTPETTEMP, *LPMAPTPETTEMP;
typedef map< WORD, LPTPET>					MAPTPET, *LPMAPTPET;

typedef map< DWORD, LPTDUEL>				MAPTDUEL, *LPMAPTDUEL;

typedef LPMAPTSTAT							LPMAPCLASS;

typedef map< BYTE, LPTFORMULA>				MAPTFORMULA, *LPMAPTFORMULA;
typedef map< WORD, LPTCASHITEM>				MAPTCASHITEM, *LPMAPTCASHITEM;
typedef map< WORD, VTGAMBLE>				MAPTGAMBLE, *LPMAPTGAMBLE;
typedef map< WORD, LPTDURINGITEM>			MAPTDURINGITEM, *LPMAPTDURINGITEM;
typedef map< WORD, LPTINDURN>				MAPTINDURN, *LPMAPTINDURN;

typedef map< WORD, LPTGODTOWER>				MAPTGODTOWER, *LPMAPTGODTOWER;
typedef map< WORD, LPTGODBALL>				MAPTGODBALL, *LPMAPTGODBALL;
typedef map< BYTE, LPTSKILLPOINT>			MAPTSKILLPOINT, *LPMAPTSKILLPOINT;
typedef map< BYTE, VTITEMTEMP>				MAPVTMONITEM, *LPMAPVTMONITEM;
typedef map< DWORD, TONTIMEMON>				MAPTONTIMEMON, *LPMAPTONTIMEMON;
typedef map< WORD, LPTPVPOINT>				MAPTPVPOINT, *LPMAPTPVPOINT;
typedef map< DWORD, TENTRYRECORD>			MAPTENTRYRECORD, *LPMAPTENTRYRECORD;
typedef map< DWORD, MAPTENTRYRECORD>		MAPTLOCALRECORD, *LPMAPTLOCALRECORD;
typedef map< WORD, LPTMONSTERSHOP>			MAPTMONSTERSHOP, *LPMAPTMONSTERSHOP;
typedef map< DWORD, LPTPROTECTED>			MAPTPROTECTED, *LPMAPTPROTECTED;

typedef map< DWORD, LPTAUCTION>				MAPTAUCTION, *LPMAPTAUCTION;
typedef map< DWORD, LPMAPTAUCTION >			MAPMAPTAUCTION, *LPMAPMAPTAUCTION;
typedef map< DWORD,TAUCTIONINTEREST>		MAPTAUCTIONINTEREST, *LPMAPTAUCTIONINTEREST;
typedef map< DWORD, TAUCTIONBIDDER >		MAPTAUCTIONBIDDER, *LPMAPTAUCTIONBIDDER;
typedef map< DWORD , EVENTINFO >			MAPEVENTINFO, *LPMAPEVENTINFO;  
typedef map< WORD, LPVTCASHGAMBLE >			MAPCASHGAMBLE, *LPMAPCASHGAMBLE;
typedef map< DWORD, LPTOURNAMENTPLAYER>		MAPTOURNAMENTPLAYER, *LPMAPTOURNAMENTPLAYER;
typedef map< BYTE, TOURNAMENT>				MAPTOURNAMENT, *LPMAPTOURNAMENT;
typedef map< BYTE, THELPMESSAGE>			MAPHELPMESSAGE, *LPMAPHELPMESSAGE;
typedef map< WORD, TRPSGAME>				MAPRPSGAME, *LPMAPRPSGAME;
typedef map< WORD, LPTARENA>				MAPARENA, *LPMAPARENA;

/////////////////////////////////////////////////////////////////////////////////////////////////
// TMapSvr structure define
struct tagTINDURN
{
	WORD m_wID;
	WORD m_wInSpawn;
	WORD m_wOutSpawn_D;
	WORD m_wOutSpawn_C;
};

struct tagGAMBLE
{
	BYTE m_bType;
	BYTE m_bKind;
	WORD m_wReplaceID;
	BYTE m_bCountMax;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	WORD m_wProb;
};

struct tagTMAGICSKILL
{
	BYTE m_bMagic;
	DWORD m_dwKind;
	WORD m_wSkillID;
	BYTE m_bIsMagic;
	BYTE m_bIsRare;
	BYTE m_bMinLevel;
};

struct tagSWITCHOBJ
{
	BYTE m_bChannel;
	WORD m_wPartyID;
	WORD m_wMapID;
	DWORD m_dwID;
	DWORD m_dwTick;
	DWORD m_dwDelay;
};
struct tagSELFOBJ
{
	BYTE m_bChannel;
	WORD m_wMapID;
	DWORD m_dwID;
	DWORD m_dwRecallTick;
	DWORD m_dwDurationTick;
};
struct tagGATE
{
	DWORD m_dwGateID;
	DWORD m_dwSwitchID;
	WORD m_wMapID;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	BYTE m_bType;
	VTSWITCH m_vSwitch;
	BYTE m_bOpened;
};

struct tagSWITCH
{
	DWORD m_dwSwitchID;
	WORD m_wMapID;
	CTMap * m_pMAP;

	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	BYTE m_bLockOnOpen;
	BYTE m_bLockOnClose;
	DWORD m_dwDuration;
	DWORD m_dwStartTime;
	BYTE m_bOpened;
	VTGATE m_vGate;
};

struct tagAGGRO
{
	BYTE m_bObjType;
	DWORD m_dwObjID;
	DWORD m_dwHostID;
	DWORD m_dwAggro;
	BYTE m_bCountry;
	tagAGGRO()
	{
		m_bObjType = 0;
		m_dwObjID = 0;
		m_dwHostID = 0;
		m_dwAggro = 0;
		m_bCountry = TCONTRY_N;
	};

	void Copy(tagAGGRO& aggro)
	{
		m_bObjType = aggro.m_bObjType;
		m_dwObjID = aggro.m_dwObjID;
		m_dwHostID = aggro.m_dwHostID;
		m_dwAggro = aggro.m_dwAggro;
		m_bCountry = aggro.m_bCountry;
	};
};
struct tagAFTERMATH
{
	BYTE m_bStep;
	DWORD m_dwTick;
	FLOAT m_fStatDec;
	FLOAT m_fReuseInc;
};
struct tagDESTINATION
{
	WORD m_wDestID;
	LPTPORTAL m_pPortal;
	BYTE m_bCondition[3];
	DWORD m_dwConditionID[3];
	DWORD m_dwPrice;
	BYTE m_bEnable;
};

struct tagPORTAL
{
	WORD m_wPortalID;
	WORD m_wLocalID;
	BYTE m_bCountry;
	WORD m_wSpawnID;
	BYTE m_bCondition;
	DWORD m_dwConditionID;
	MAPTDESTINATION m_mapDestination;
	LPTLOCAL m_pLocal;

	~tagPORTAL()
	{
		MAPTDESTINATION::iterator it;
		for(it=m_mapDestination.begin(); it!=m_mapDestination.end(); it++)
			delete (*it).second;
	};
};

struct tagPATHPOINT
{
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	BYTE m_bProb;
	FLOAT m_fRadius;
};

struct tagITEMATTR
{
	WORD m_wID;
	BYTE m_bKind;
	BYTE m_bGrade;
	WORD m_wMinAP;
	WORD m_wMaxAP;
	WORD m_wDP;
	WORD m_wMinMAP;
	WORD m_wMaxMAP;
	WORD m_wMDP;
	BYTE m_bBlockProb;
};
struct tagITEMGRADE
{
	BYTE m_bLevel;
	BYTE m_bGrade;
	BYTE m_bProb;
	DWORD m_dwMoney;
};
struct tagITEMMAGIC
{
	BYTE m_bMagic;
	DWORD m_dwKind;
	BYTE m_bRvType;
	WORD m_wMaxValue;
	BYTE m_bIsMagic;
	BYTE m_bIsRare;
	BYTE m_bMinLevel;
	BYTE m_bExclIndex;
	BYTE m_bOptionKind;
	WORD m_wAutoSkill;
	BYTE m_bRefine;
	WORD m_wMaxBound;
	WORD m_wRareBound;
};
struct tagITEMSET
{
	WORD m_wBaseID;
	WORD m_wSetID;
	BYTE m_bLevel;
	LPTITEMMAGIC m_pMagic[TMAGIC_MAX];
	BYTE m_bMin[TMAGIC_MAX];
	BYTE m_bValue[TMAGIC_MAX];
};
struct tagBATTLEZONE
{
	WORD m_wID;
	CString m_strName;
	WORD m_wMapID;
	WORD m_wCastle;
	WORD m_wBossSpawnID;
	WORD m_wLGateKeeperSpawnID;
	WORD m_wRGateKeeperSpawnID;
	WORD m_wCGateKeeperSpawnID;
	DWORD m_dwLSwitchID;
	DWORD m_dwRSwitchID;
	DWORD m_dwCSwitchID;
	BYTE m_bLine;
	WORD m_wSkill1;
	WORD m_wSkill2;
	BYTE m_bItemLevel;
	LPTITEM m_pNormalItem;
	LPTITEM m_pChiefItem;
	VWORD m_vSpawnID;
	MAPTPVPOINT m_mapTPvPoint;
};
struct tagLOCAL
{
	BYTE m_bType;
	WORD m_wLocal;
	BYTE m_bValid;
	LPTBATTLEZONE m_pZone;
	DWORD m_dwGuild;
	CString m_strGuild;
	BYTE m_bCountry;
	DWORD m_dwBattleStart;
	__time64_t m_timeOccupied;
	__time64_t m_timeNextDefend;
	DWORD m_dwAlarmTime;
	BYTE m_bStatus;
	BYTE m_bGateOpened;
	BYTE m_bCanBattle;
	DWORD m_occupyGuild[7];
	BYTE m_occupyType[7];
	BYTE m_bOccupied;
	BYTE m_bLastOccType;
	CString m_strHero;       
	__time64_t m_timeHero;
	MAPTLOCALRECORD m_mapTRecord;

	BYTE m_bDefCountry;
	WORD m_wDPoint;
	WORD m_wCPoint;
	BYTE m_bDefCount;
	BYTE m_bAtkCount;
	BYTE m_bDay;
	MAPTLOCAL m_mapLocal;
	DWORD m_dwAtkGuildID;
	DWORD m_dwDefGuildID;
	CString m_strAtkName;
	CString m_strDefName;

	VLOCALSUMPOINT m_vTop3[TCONTRY_COUNT];
	MAPDWORD m_mapTPoint;
};

struct tagLOCALSUMPOINT
{
	CString m_strName;
	WORD m_wPoint;
};

struct tagHOTKEY
{
	BYTE m_bPos;
	BYTE m_bType;
	WORD m_wID;
};

struct tagHOTKEYINVEN
{
	BYTE m_bSave;
	THOTKEY m_hotkey[MAX_HOTKEY_POS];
	tagHOTKEYINVEN()
	{
		memset(m_hotkey, 0, sizeof(THOTKEY)*MAX_HOTKEY_POS);
	};
};

struct tagPARTYMEMBER
{
	DWORD m_dwID;
	CString m_strName;
	CString m_strGuild;
	BYTE m_bLevel;
	DWORD m_dwMaxHP;
	DWORD m_dwHP;
	DWORD m_dwMaxMP;
	DWORD m_dwMP;
	BYTE m_bRace;
	BYTE m_bSex;
	BYTE m_bFace;
	BYTE m_bHair;
	tagPARTYMEMBER()
	{
		m_dwMaxHP = 0;
		m_dwHP = 0;
		m_dwMaxMP = 0;
		m_dwMP = 0;
		m_bRace = 0;
		m_bSex = 0;
		m_bFace = 0;
		m_bHair = 0;
	};
};
struct tagTMONITEM
{
	LPTITEM m_pTITEM;
	CTItem * m_pMagicItem;
	BYTE m_bChartType;
	WORD m_wItemIDMin;
	WORD m_wItemIDMax;
	WORD m_wWeight;
	BYTE m_bLevelMin;
	BYTE m_bLevelMax;
	BYTE m_bItemProb[MIP_COUNT];
	BYTE m_bItemMagicOpt;
	BYTE m_bItemRareOpt;
};
struct tagTSPAWNPOS
{
	WORD m_wID;
	WORD m_wMapID;
	BYTE m_bType;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
};
struct tagPARTYEXP
{
	VPLAYER m_vPlayer;
	DWORD m_dwTotalDamage;
	DWORD m_dwTotalLevel;
	DWORD m_dwTotalExp;
	tagPARTYEXP()
	{
		m_vPlayer.clear();
		m_dwTotalDamage = 0;
		m_dwTotalLevel = 0;
		m_dwTotalExp =0;
	}
};

struct tagTAICONDITION
{
	DWORD m_dwID;
	BYTE m_bType;
};

struct tagTAICOMMAND
{
	VTAICONDITION m_vCONDITION;

	DWORD m_dwID;
	BYTE m_bType;

	~tagTAICOMMAND()
	{
		while(!m_vCONDITION.empty())
		{
			delete m_vCONDITION.back();
			m_vCONDITION.pop_back();
		}
	};
};

struct tagTMONATTR
{
	WORD m_wID;
	BYTE m_bLevel;
	DWORD m_dwAttr;
	WORD m_wAP;
	WORD m_wLAP;
	DWORD m_dwAtkSpeed;
	WORD m_wAL;
	WORD m_wMAL;
	WORD m_wDL;
	WORD m_wMDL;
	BYTE m_bCriticalPP;
	DWORD m_dwMaxHP;
	BYTE m_bHPRecover;
	WORD m_wMAP;
	BYTE m_bCriticalMP;
	DWORD m_dwMaxMP;
	BYTE m_bMPRecover;
	WORD m_wDP;
	WORD m_wMDP;
	WORD m_wMinWAP;
	WORD m_wMaxWAP;
	WORD m_wWDP;
};

struct tagTMONSTER
{
	CTMonsterAI *m_pAI;
	VTMONITEM m_vMONITEM;
	VWORD m_vSKILL;

	WORD m_wID;

	BYTE m_bClass;
	WORD m_wKind;
	BYTE m_bLevel;
	BYTE m_bRace;

	BYTE m_bRoamProb;
	BYTE m_bRange;

	BYTE m_bMoneyProb;
	DWORD m_dwMinMoney;
	DWORD m_dwMaxMoney;

	BYTE m_bItemProb;
	BYTE m_bDropCount;
	BYTE m_bIsSelf;

	WORD m_wExp;
	WORD m_wChaseRange;
	BYTE m_bRecallType;
	BYTE m_bCanSelect;
	BYTE m_bCanAttack;
	BYTE m_bTame;
	WORD m_wTransSkillID;
	BYTE m_bCall;
	BYTE m_bRemove;
	FLOAT m_fSize;
	BYTE m_bIsSpecial;
	WORD m_wMonAttr;
	WORD m_wSummonAttr;
	DWORD m_dwMaxWeight;
	BYTE m_bCheckPartyQuest;

	tagTMONSTER()
	{
		m_pAI = NULL;
		m_vMONITEM.clear();
		m_dwMaxWeight = 0;
	};
	~tagTMONSTER()
	{
		while(!m_vMONITEM.empty())
		{
			delete m_vMONITEM.back();
			m_vMONITEM.pop_back();
		}
	};
};

struct tagTMAPMON
{
	WORD m_wSpawnID;
	WORD m_wMonID;
	BYTE m_bLeader;

	BYTE m_bEssential;
	BYTE m_bProb;
};

struct tagTMONSPAWN
{
	VTMAPMON m_vMAPMON;
	LPVTMONSPAWN m_lpvGroup;
	VPATHPOINT m_vPathPoint;
	VWORD m_vPARTY;

	WORD m_wID;

	DWORD m_dwRegion;
	WORD m_wMapID;
	WORD m_wLocalID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	WORD m_wDir;

	BYTE m_bCountry;
	BYTE m_bCount;
	BYTE m_bRange;
	BYTE m_bArea;
	BYTE m_bLink;
	BYTE m_bProb;
	BYTE m_bRoamType;
	WORD m_wPartyID;

	DWORD m_dwDelay;
	BYTE m_bEvent;
	WORD m_wGroup;

	tagTMONSPAWN()
	{
		m_vMAPMON.clear();
	};

	~tagTMONSPAWN()
	{
		while(!m_vMAPMON.empty())
		{
			delete m_vMAPMON.back();
			m_vMAPMON.pop_back();
		}
		while(!m_vPathPoint.empty())
		{
			delete m_vPathPoint.back();
			m_vPathPoint.pop_back();
		}
	};
};

struct tagTAIBUF
{
	DWORD m_dwCmdHandle;
	DWORD m_dwEventHost;
	DWORD m_dwRHId;
	BYTE m_bRHType;
	DWORD m_dwHostKEY;
	DWORD m_dwMonID;
	DWORD m_dwDelay;
	DWORD m_dwTick;

	BYTE m_bChannel;
	WORD m_wMapID;
	WORD m_wPartyID;
};

struct tagTSTAT
{
	WORD m_wSTR;
	WORD m_wDEX;
	WORD m_wCON;

	WORD m_wINT;
	WORD m_wWIS;
	WORD m_wMEN;
};

struct tagTLEVEL
{
	BYTE m_bLevel;
	DWORD m_dwEXP;
	DWORD m_dwHP;
	DWORD m_dwMP;
	BYTE m_bSkillPoint;
	DWORD m_dwMoney;
	DWORD m_dwRegCost;
	DWORD m_dwSearchCost;
	DWORD m_dwGambleCost;
	DWORD m_dwRepCost;
	DWORD m_dwRepairCost;
	DWORD m_dwRefineCost;
	WORD m_wPvPoint;
	DWORD m_dwPvPMoney;
};

struct tagTINVENDESC
{
	BYTE m_bInvenID;
	WORD m_wItemID;
};

struct tagTITEM
{
	WORD m_wItemID;
	BYTE m_bType;
	BYTE m_bKind;
	WORD m_wAttrID;
	WORD m_wUseValue;

	DWORD m_dwClassID;
	DWORD m_dwSlotID;

	FLOAT m_fPrice;
	FLOAT m_fPvPrice;
	FLOAT m_fRevision[RV_COUNT];

	BYTE m_bIsSell;
	BYTE m_bPrmSlotID;
	BYTE m_bSubSlotID;
	BYTE m_bSlotCount;
	BYTE m_bMinRange;
	BYTE m_bMaxRange;
	BYTE m_bDefaultLevel;
	BYTE m_bStack;
	BYTE m_bEquipSkill;
	BYTE m_bItemCountry;

	BYTE m_bUseItemKind;
	BYTE m_bUseItemCount;
	BYTE m_bGrade;
	WORD m_wUseTime;
	BYTE m_bUseType;
	BYTE m_bCanGrade;
	BYTE m_bCanMagic;
	BYTE m_bCanRare;
	BYTE m_bDropLevel;
	DWORD m_dwSpeedInc;
	BYTE m_bSpecial;
	DWORD m_dwDelay;
	BYTE m_bCanGamble;
	WORD m_wItemProb_G;
	BYTE m_bGambleProb;
	BYTE m_bDestroyProb;
	DWORD m_dwDuraMax;
	BYTE m_bRefineMax;
	BYTE m_bCanRepair;
	WORD m_wDelayGroupID;
	WORD m_wWeight;
	BYTE m_bGroup;
	BYTE m_bInitState;
	BYTE m_bCanWrap;
	DWORD m_dwCode;
	BYTE m_bCanColor;
	BYTE m_bConsumable;	//	소모타입
};

struct tagTDURINGITEM
{
	LPTITEM m_pTITEM;
	BYTE m_bType;
	DWORD m_dwRemainTime;
	__time64_t m_tEndTime;

	tagTDURINGITEM()
	{
		m_pTITEM = NULL;
		m_bType = 0;
		m_dwRemainTime = 0;
		m_tEndTime = 0;
	};
};

struct tagTMAGIC
{
	WORD m_wValue; // 최종옵션비율
	LPTITEMMAGIC m_pMagic;
};

struct tagTSKILLDATA
{
	BYTE m_bAction;
	BYTE m_bType;
	BYTE m_bAttr;
	BYTE m_bExec;
	BYTE m_bInc;
	WORD m_wValue;
	WORD m_wValueInc;
	BYTE m_bCalc;
};

struct tagPOST
{
	BYTE m_bSave;
	DWORD m_dwPostID;
	DWORD m_dwSendID;
	BYTE m_bRead;
	CString m_strSender;
	CString m_strTitle;
	CString m_strMessage;
	BYTE m_bType;
	__time64_t m_timeRecv;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	VTITEM m_vItem;
};

struct tagCABINET
{
	BYTE m_bCabinetID;
	BYTE m_bUse;
	MAPTSTORAGEITEM m_mapCabinetItem;
	
	tagCABINET()
	{
		m_mapCabinetItem.clear();
	};
};

struct tagQUESTCONDITION
{
	DWORD m_dwConditionID;
	BYTE m_bConditionType;
	BYTE m_bCount;
};

struct tagQUESTREWARD
{
	DWORD m_dwRewardID;
	BYTE m_bRewardType;
	BYTE m_bTakeMethod;
	BYTE m_bTakeData;
	BYTE m_bCount;
	CTItem * m_pMagicItem;

	tagQUESTREWARD()
	{
		m_pMagicItem = NULL;
	};
};

struct tagQUESTTERM
{
	DWORD m_dwTermID;
	BYTE m_bTermType;
	BYTE m_bCount;
};

struct tagQUESTTEMP
{
	VQUESTCONDITION m_vCondition;
	VQUESTREWARD m_vReward;
	VQUESTTERM m_vTerm;

	MAPQUESTTEMP m_mapCHILD;

	DWORD m_dwTriggerID;
	DWORD m_dwParentID;
	DWORD m_dwQuestID;

	BYTE m_bTriggerType;
	BYTE m_bForceRun;
	BYTE m_bCountMax;
	BYTE m_bType;
	BYTE m_bConditionCheck;

	tagQUESTTEMP()
	{
		m_vCondition.clear();
		m_vReward.clear();
		m_vTerm.clear();

		m_mapCHILD.clear();

		m_dwTriggerID = 0;
		m_dwParentID = 0;
		m_dwQuestID = 0;

		m_bTriggerType = 0;
		m_bForceRun = 0;
		m_bCountMax = 0;
		m_bType = 0;
		m_bConditionCheck = 0;
	};

	~tagQUESTTEMP()
	{
		MAPQUESTTEMP::iterator it;

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

		for( it = m_mapCHILD.begin(); it != m_mapCHILD.end(); it++)
		{
			tagQUESTTEMP *pTEMP = (*it).second;
			DWORD dwID = (*it).first;
			delete (*it).second;
		}
		m_mapCHILD.clear();
	};
};

struct tagDEALITEM
{
	BYTE m_bStatus;
	BYTE m_bDealing;
	CString m_strTarget;
	__int64 m_llSendMoney;
	__int64 m_llRecvMoney;
	VTITEM m_vSendItem;
	VTITEM m_vRecvItem;
};

struct tagSTOREITEM
{
	BYTE m_bID;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	BYTE m_bInvenID;
	BYTE m_bItemID;
	BYTE m_bItemCount;
};

struct tagPETTEMP
{
	WORD m_wID;
	BYTE m_bPetType;
	BYTE m_bRace;
	WORD m_wMonID;
	MAPBWORD m_mapKind;
	BYTE m_bConditionType;
	DWORD m_dwConditionValue;
};

struct tagPET
{
    WORD m_wPetID;
	CString m_strName;
	__int64 m_ldwTime;
	LPTPETTEMP m_pTPET;
};

struct tagDUEL
{
	DWORD m_dwDuelID;
	BYTE m_bType;
	DWORD m_dwInviter;
	CString m_strInviter;
	DWORD m_dwTarget;
	CString m_strTarget;
	DWORD m_dwDuelTick;
};

struct tagFORMULA
{
	BYTE m_bID;
	CString m_strName;
	DWORD m_dwInit;
	FLOAT m_fRateX;
	FLOAT m_fRateY;
};

struct tagCASHCATEGORY
{
	BYTE m_bID;
	CString m_strName;
	VTCASHITEM m_vCashItem;
};

struct tagCASHITEM
{
	WORD m_wID;
	WORD m_wInfoID;
	DWORD m_dwMoney;
	DWORD m_dwOriMoney;
	BYTE m_bKind;
	BYTE m_bCanSell;
	BYTE m_bSaleValue;  
	WORD m_wUseTime;
	CTItem * m_pItem;
};

struct tagCASHGAMBLE
{
	DWORD m_dwID;
	DWORD m_dwProb;
	WORD  m_wGroup;
	WORD m_wUseTime;
	CTItem * m_pItem;
};

struct tagGODTOWER
{
	WORD m_wID;
	LPTGODBALL m_pGodBall;
	CTMap * m_pMAP;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	VWORD m_vSpawnID;
};

struct tagGODBALL
{
	DWORD m_dwKEY;
	WORD m_wID;
	WORD m_wCastle;
	BYTE m_bCamp;
	DWORD m_dwGuildID;
	CTMap * m_pMAP;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	CString m_strOwner;
	__time64_t m_dlTime;
	BYTE m_bMove;
	WORD m_wTempID;
	BYTE m_bGround;
};

struct tagGODBALLBUF
{
	WORD m_wGodBall;
	DWORD m_dwKEY;
	BYTE m_bChannel;
	WORD m_wMapID;
	WORD m_wCastle;
	BYTE m_bCmd;
	DWORD m_dwDelay;
	DWORD m_dwTick;
	CString m_strCharName;
};

struct tagTITEMQUERY
{
	__int64 m_dlID;
	BYTE m_bItemID;
	WORD m_wItemID;
	BYTE m_bLevel;
	BYTE m_bCount;
	BYTE m_bGLevel;
	DWORD m_dwDuraMax;
	DWORD m_dwDuraCur;
	BYTE m_bRefineCur;
	TIMESTAMP_STRUCT m_dEndTime;
	BYTE m_bGradeEffect;
	BYTE m_bMagic[TMAGIC_MAX];
	WORD m_wValue[TMAGIC_MAX];
	DWORD m_dwExtValue[TMAGIC_MAX];

	void Reset()
	{
		m_dlID = 0;
		m_bItemID = 0;
		m_wItemID = 0;
		m_bLevel = 0;
		m_bCount = 0;
		m_bGLevel = 0;
		m_dwDuraMax = 0;
		m_dwDuraCur = 0;
		m_bRefineCur = 0;
		m_bGradeEffect = 0;
		memset(&m_dEndTime, 0, sizeof(TIMESTAMP_STRUCT));
		memset(m_bMagic, 0, sizeof(BYTE)*TMAGIC_MAX);
		memset(m_wValue, 0, sizeof(WORD)*TMAGIC_MAX);
		memset(m_dwExtValue, 0, sizeof(DWORD)*TMAGIC_MAX);
	};
};

struct tagSKILLPOINT
{
	WORD m_wSkillID;
	BYTE m_bLevel;
	BYTE m_bSkillPoint;
	BYTE m_bGroupPoint;
	BYTE m_bParentLevel;
	DWORD m_dwPayback;
};

struct tagONTIMEMON
{
	BYTE m_bChannel;
	WORD m_wPartyID;
	WORD m_wMapID;
	DWORD m_dwID;
};

struct tagPVPOINT
{
	WORD m_wID;
	BYTE m_bTarget;
	DWORD m_dwIncPoint;
	DWORD m_dwDecPoint;
};

struct tagENTRYRECORD
{
	DWORD m_dwCharID;
	CString m_strName;
	BYTE m_bRace;
	BYTE m_bClass;
	BYTE m_bLevel;
	WORD m_wKillCount;
	WORD m_wDieCount;
	DWORD m_aGainPoint[PVPE_COUNT];
};

struct tagRECORDSET
{
	CString	m_strName;
	BYTE	m_bClass;
	BYTE	m_bLevel;
	BYTE	m_bWin;
	DWORD	m_dwPoint;
	__time64_t	m_dTime;
};

struct tagMONSTERSHOP
{
	WORD m_wID;
	LPTMONSPAWN m_pSpawn;
	DWORD m_dwPrice;
	WORD m_wTowerID;
};

struct tagPROTECTED
{
	DWORD m_dwCharID;
	CString m_strName;
	BYTE m_bOption;
	BYTE m_bChanged;
};
 
struct tagTCASHITMSALE
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

struct tagTAUCTION
{
	DWORD	m_dwAuctionID;
	WORD	m_wNpcID;
	DWORD	m_dwSeller;
	BYTE	m_bBidCount;
	CString m_strSeller;
	CTItem*	m_pItem;
	__int64	m_ldwDirectPrice;
	__int64	m_ldwStartPrice;
	__int64 m_ldwBidPrice;
	__time64_t	m_dStart;
	__time64_t	m_dEnd;
	__time64_t  m_dBid;
	DWORD m_dwRemainTime;
	DWORD m_dwCode;
	DWORD m_dwBidder;

	tagTAUCTION()
	{
		m_pItem = NULL;
	}
	void WrapPacketAuction(CPacket* pPacket)
	{
		(*pPacket)
			<< m_dwAuctionID
			<< m_strSeller
			<< m_dEnd
			<< m_dwRemainTime
			<< m_ldwDirectPrice
			<< m_ldwStartPrice
			<< m_ldwBidPrice
			<< m_bBidCount
			<< m_dwCode;
	}
};

struct tagTAUCTIONINTEREST
{	
	DWORD m_dwAuctionID;
	BYTE m_bStatus; // 0: 경매중 1:낙찰 2:유찰
	TAUCTION m_TAuction;
};

struct tagTAUCTIONBIDDER
{
	DWORD	m_dwAuctionID;
	DWORD	m_dwCharID;
	__int64	m_ldwBidPrice;
	__time64_t	m_dBidDate;
};

struct tagTAUCTIONBUF
{
	WORD m_wNpcID;
	DWORD m_dwAuctionID;
	__time64_t	m_dEndTime;
};

struct AuctionIDASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dwAuctionID < p2->m_dwAuctionID ;		
	}
};

struct AuctionIDDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dwAuctionID > p2->m_dwAuctionID;			
	}
};

struct EndTimeASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dEnd < p2->m_dEnd;			
	}	
};

struct EndTimeDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dEnd > p2->m_dEnd;			
	}	
};

struct RemainTimeASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dwRemainTime < p2->m_dwRemainTime;			
	}	
};

struct RemainTimeDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_dwRemainTime > p2->m_dwRemainTime;			
	}	
};

struct BidPriceASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_ldwBidPrice < p2->m_ldwBidPrice;			
	}
};

struct BidPriceDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_ldwBidPrice > p2->m_ldwBidPrice;			
	}
};

struct DirectPriceASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_ldwDirectPrice < p2->m_ldwDirectPrice;			
	}
};

struct DirectPriceDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_ldwDirectPrice > p2->m_ldwDirectPrice;			
	}
};

struct BidCountASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_bBidCount < p2->m_bBidCount;			
	}
};

struct BidCountDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_bBidCount > p2->m_bBidCount;			
	}
};

/*
struct SellerNameASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		int a =  strcmp(p1->m_strSeller, p2->m_strSeller);
		if(a <= 0 )
			return true;
		return false;
	}
};

struct SellerNameDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		int a =  strcmp(p1->m_strSeller, p2->m_strSeller);
		if(a <= 0 )
			return false;
		return true;
	}
};
*/

 
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

struct tagTOURNAMENTPLAYER
{
	DWORD m_dwCharID;
	BYTE m_bCountry;
	CString m_strName;
	BYTE m_bLevel;
	BYTE m_bClass;
	DWORD m_dwChiefID;
	BYTE m_bEntryID;
	BYTE m_bSlot;
	WORD m_wMapID;
	BYTE m_bResult[MATCH_COUNT];
};

struct tagTNMTREWARD
{
	BYTE m_bChartType;
	WORD m_wItemID;
	BYTE m_bCount;
};

struct tagTOURNAMENT
{
	BYTE m_bGroup;
	BYTE m_bEntryID;
	CString m_strName;
	BYTE m_bType;
	DWORD m_dwClass;
	DWORD m_dwFee;
	DWORD m_dwBackFee;
	WORD m_wPermitItem;
	BYTE m_bPermitCount;	
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	VTNMTREWARD m_vReward;
};

struct tagTOURNAMETTIME
{
	BYTE m_bGroup;
	BYTE m_bStep;
	INT64 m_dlEndTime;
	BYTE m_bResult;
};

struct tagHELPMESSAGE
{
	BYTE m_bID;
	INT64 m_dlStartTime;
	INT64 m_dlEndTime;
	CString m_strMessage;
	BYTE m_bNotice;
};

struct tagRPSGAME
{
	BYTE m_bType;
	BYTE m_bWinCount;
	WORD m_wItemID;
	DWORD m_dwRewardMoney;
	LPTITEM m_pRewardItem[2];
	BYTE m_bItemCount[2];
	BYTE m_bProb[3];
	WORD m_wWinKeep;
	WORD m_wWinPeriod;
};

struct tagARENA
{
	WORD m_wID;
	BYTE m_bType;
	LPTSPAWNPOS m_pInPos;
	LPTSPAWNPOS m_pOutPos;
	DWORD m_dwFee;
	MAPPLAYER m_mapFighter[2];
	BYTE m_bStatus;
	DWORD m_dwTick;
	BYTE m_bWinCount;
	BYTE m_bDrawCount;
	WORD m_wWinPartyID;
};

struct tagTCHATLOG
{
	DWORD m_dwRecent;
	DWORD m_dwSize;

	tagTCHATLOG()
	{
		m_dwRecent = 0;
		m_dwSize = 0;
	};

	void reset()
	{
		m_dwRecent = 0;
		m_dwSize = 0;
	};
};

///////////////////////////////////////////////////////////////////////////
// TMapSvr class define
#ifdef __HACK_SHIELD
#pragma comment (lib, "AntiCpXSvr.lib")
#include "AntiCpXSvr.h"
#define HACKSHIELD_SENDTICK		120000
#define HACKSHIELD_RECVTICK		20000
#endif

#ifdef __N_PROTECT
#define __N_PROTECT_DEBUG
#pragma comment (lib, "ggsrvlib25.lib")
#include "ggsrv25.h"

#ifdef __N_PROTECT_DEBUG
	#define NPROTECT_CS_UPDATETICK	180000
	#define NPROTECT_SENDTICK		60000
	#define NPROTECT_RECVTICK		10000
#else
	#define NPROTECT_CS_UPDATETICK	10800000
	#define NPROTECT_SENDTICK		120000
	#define NPROTECT_RECVTICK		10000
#endif
#endif

#include "TMapSession.h"
#include "TMonsterAI.h"
#include "TAICommand.h"
#include "TAICmdBeginAtk.h"
#include "TAICmdSetHost.h"
#include "TAICmdChkHost.h"
#include "TAICmdChgHost.h"
#include "TAICmdChgMode.h"
#include "TAICmdGetaway.h"
#include "TAICmdAttack.h"
#include "TAICmdFollow.h"
#include "TAICmdRefill.h"
#include "TAICmdLeave.h"
#include "TAICmdRegen.h"
#include "TAICmdRoam.h"
#include "TAICmdRemove.h"
#include "TAICmdGohome.h"
#include "TAICmdLottery.h"
#include "TMonSpawn.h"
#include "TChannel.h"
#include "TObjBase.h"
#include "TMonster.h"
#include "TPlayer.h"
#include "TSkillTemp.h"
#include "TSkill.h"
#include "TInven.h"
#include "TItem.h"
#include "TCell.h"
#include "TMap.h"
#include "Quest.h"
#include "QuestComplete.h"
#include "QuestDefaultTalk.h"
#include "QuestDropItem.h"
#include "QuestGiveItem.h"
#include "QuestGiveSkill.h"
#include "QuestMission.h"
#include "QuestSpawnMon.h"
#include "QuestTeleport.h"
#include "QuestRouting.h"
#include "QuestTalk.h"
#include "QuestDrop.h"
#include "QuestChapterMsg.h"
#include "QuestSwitch.h"
#include "QuestDieMon.h"
#include "QuestDefendSkill.h"
#include "QuestDeleteItem.h"
#include "QuestSendPost.h"
#include "QuestCraft.h"
#include "QuestRegen.h"
#include "TNpc.h"
#include "TRecallMon.h"
#include "TSelfObj.h"
#include "TMiniDump.h"
#include "TTextLinker.h"
#include "TTextLinkData.h"


struct ClassASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_pTITEM->m_dwClassID < p2->m_pItem->m_pTITEM->m_dwClassID;			
	}
};

struct ClassDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_pTITEM->m_dwClassID > p2->m_pItem->m_pTITEM->m_dwClassID;			
	}
};

struct SellerNameASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		int a =  strcmp(p1->m_strSeller, p2->m_strSeller);
		if(a <= 0 )
			return TRUE;
		return false;
	}
};

struct SellerNameDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		int a =  strcmp(p1->m_strSeller, p2->m_strSeller);
		if(a > 0 )
			return TRUE;
		return false;
	}
};

struct WearLevelASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_pTITEM->m_bDefaultLevel < p2->m_pItem->m_pTITEM->m_bDefaultLevel;
	}
};

struct WearLevelDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_pTITEM->m_bDefaultLevel > p2->m_pItem->m_pTITEM->m_bDefaultLevel;
	}
};

struct ItemLevelASC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_bLevel < p2->m_pItem->m_bLevel;
	}
};

struct ItemLevelDESC
{
	bool operator()(LPTAUCTION p1, LPTAUCTION p2) const
	{
		return p1->m_pItem->m_bLevel > p2->m_pItem->m_bLevel;
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