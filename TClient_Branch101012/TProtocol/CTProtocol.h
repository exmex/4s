#pragma once

#include "ProtocolBase.h"

#define DCSVC_STAT_STOPPED				SERVICE_STOPPED
#define DCSVC_STAT_START_PENDING		SERVICE_START_PENDING
#define DCSVC_STAT_STOP_PENDING			SERVICE_STOP_PENDING
#define DCSVC_STAT_RUNNING				SERVICE_RUNNING
#define DCSVC_STAT_CONTINUE_PENDING		SERVICE_CONTINUE_PENDING
#define DCSVC_STAT_PENDING				SERVICE_PAUSE_PENDING
#define DCSVC_STAT_PAUSED				SERVICE_PAUSED
#define DCSVC_STAT_CANNOTCONTROL		((DWORD)-1)

#define ACK_SUCCESS			0
#define ACK_FAILED			1

#define WORLD_NULL			0
#define SVRGRP_NULL			0
#define SVRID_NULL			0

#define MAKESVRID(a,b,c)	DWORD(DWORD(a) << 16 | DWORD(b) << 8 | DWORD(c))
#define SVRGROUP(a)			BYTE(a >> 16)
#define SVRTYPE(a)			BYTE(a >> 8)
#define SVRID(a)			BYTE(a)

#define SVRGRP_NULL			0
#define SVRGRP_CTLSVR		1
#define SVRGRP_LOGINSVR		2
#define SVRGRP_WORLDSVR		3
#define SVRGRP_MAPSVR		4
#define SVRGRP_PATCHSVR		5
#define SVRGRP_FTPSVR		6
#define SVRGRP_LOG			7
#define SVRGRP_RLYSVR		8
#define SVRGRP_PREFTPSVR	9

#define DEFAULT_CTL_PORT	3615
#define DEFAULT_PATCH_PORT	3715
#define DEFAULT_GM_PORT		3815
#define DEFAULT_LOGIN_PORT	4815
#define DEFAULT_MAP_PORT	5815

typedef enum WORLDPOST_TYPE
{
	WPT_NONE = 0,
	WPT_TACTICSKICK,
	WPT_TACTICSEND,
	WPT_PAYBACK,
	WPT_LOTITEM
} *LPWORLDPOST_TYPE;

typedef enum SERVER_MESSAGE
{
	MSG_POSTINVENITEM = 1,
	MSG_LOCAL_REWARD,
	MSG_CASTLE_REWARD,
	PREMIUM_PETNAME,
	TMS_NORECEIVER,
	NAME_OPERATOR,
	MSG_CHAR_LOGOUT,
	MSG_GUILDPOINT_TAKE,
	MSG_PRESENT_TITLE,
	MSG_PRESENT_MESSAGE,
	MSG_ITEMERROR_TITLE,
	MSG_ITEMERROR_MESSAGE,
	MSG_AUCTION_OPERATOR,	 // 경매 중개인
	MSG_AUCTION_FAIL,		 // 거래가 이루어지지 않았습니다.
	MSG_AUCTION_BIDSUCCESS,  // 입찰하신 물품이 도착했습니다.
	MSG_AUCTION_PRICE,		 // 거래가 성공적으로 이루어졌습니다.
	MSG_AUCTION_REGCANCEL,	 // 등록이 취소된 물품입니다.
	MSG_AUCTION_BIDFAIL,	 // 입찰에 실패하였습니다.
	MSG_AUCTION_OTHERBIDDER, // 상위 입찰자가 나타났습니다.
	MSG_AUCTION_OTHERWIN,	 // 물품이 다른 분에게 낙찰되었습니다
	MSG_AUCTION_FAIL_BODY,			// 정해진 기간 내에 %s에 대한 입찰자가 나타나지 않아 거래가 이루어지지 못했습니다.반송된 물품을 확인해주시기 바랍니다. 
	MSG_AUCTION_BIDSUCCESS_BODY,	// %s에 낙찰되셨습니다. 편지에 동봉되어 있는 물품을 확인해주시기 바랍니다.
	MSG_AUCTION_PRICE_BODY,			// %s 물품이 성공적으로 판매되었습니다. 편지에 동봉되어 있는 금액을 확인해주시기 바랍니다.(경매장 이용 수수료는 차감됩니다)
	MSG_AUCTION_REGCANCEL_BODY,		// 경매장에서 등록이 취소된 %s입니다. 편지에 동봉되어 있는 물품을 확인해주시기 바랍니다.
	MSG_AUCTION_BIDFAIL_BODY,		// 경매장 이용에 불편을 드려 죄송합니다. %s에 입찰하셨던 금액은 편지에 동봉되어 있으니 확인해주시기 바랍니다.
	MSG_AUCTION_OTHERBIDDER_BODY,	// 입찰하신 %s에 상위 입찰자가 나타났습니다.원하실 경우 경매 담당자를 통해 상위 금액으로 다시 입찰하실 수 있습니다.	
	MSG_AUCTION_OTHERBUY_BODY,		// %s 물품이 다른 분에게 낙찰되었습니다.
	MSG_AUCTION_DROP_BODY,			// %s 물품의 등록이 취소되었습니다. 입찰하셨던 금액은 편지에 동봉되어 있으니 확인해주시기 바랍니다.
	MSG_AUCTION_REGCANCEL_DROP,		// 입찰하신 물품의 등록이 취소되었습니다.
	MSG_TACTICS_TITLE,				// 용병 계약금이 지급되었습니다.
	MSG_TACTICS_MESSAGE,			// [%s]길드의 용병으로 고용되셨습니다. 계약 만료전에 해고 당할 경우 공적을 받을 수 없습니다. 많은 활동 부탁드립니다.
	MSG_TACTICSKICK_TITLE,			// 용병 해고
	MSG_TACTICSKICK_MESSAGE,		// 용병으로써의 자격 상실로 해고하였습니다. 계약 공적은 지급할 수 없습니다.
	MSG_TACTICSEND_TITLE,			// 용병 계약 공적이 지급되었습니다.
	MSG_TACTICSEND_MESSAGE,			// [%d] 공적이 지급되었습니다. 적극적인 활동에 감사드리며 앞으로도 많은 성원 부탁드립니다.
	MSG_HALLOFHERO_TITLE,		
	MSG_HALLOFHERO_MESSAGE,		
	TNMT_PAYBACK_TITLE,				// 토너먼트 선발 탈락.
	TNMT_PAYBACK_MESSAGE,			// 토너먼트 선발에서 탈락되어 환불금이 지급되었습니다. 다음기회를 이용해 주시기 바랍니다.
	TNMT_WIN_TITLE,
	TNMT_WIN_MESSAGE,
	TNMT_BATTING_TITLE,
	TNMT_BATTING_MESSAGE,
	MSG_MISSION_REWARD,
	RPS_REWARD_TITLE,
	RPS_REWARD_MESSAGE
};

#define CT_OPLOGIN_REQ						(CT_CONTROL + 0x0001)
#define CT_OPLOGIN_ACK						(CT_CONTROL + 0x0002)
#define CT_SVRTYPELIST_ACK					(CT_CONTROL + 0x0003)
#define CT_MACHINELIST_ACK					(CT_CONTROL + 0x0004)
#define CT_GROUPLIST_ACK					(CT_CONTROL + 0x0005)
#define CT_SERVICESTAT_REQ					(CT_CONTROL + 0x0006)
#define CT_SERVICESTAT_ACK					(CT_CONTROL + 0x0007)
#define CT_SERVICECONTROL_REQ				(CT_CONTROL + 0x0008)
#define CT_SERVICECONTROL_ACK				(CT_CONTROL + 0x0009)
#define CT_NEWCONNECT_REQ					(CT_CONTROL + 0x001A)
#define CT_SERVICECHANGE_REQ				(CT_CONTROL + 0x001B)
#define CT_SERVICECHANGE_ACK				(CT_CONTROL + 0x001C)
#define CT_SERVICEMONITOR_REQ				(CT_CONTROL + 0x001D)
#define CT_SERVICEMONITOR_ACK				(CT_CONTROL + 0x001E)
#define CT_TIMER_REQ						(CT_CONTROL + 0x001F)
#define CT_SERVICEDATA_ACK					(CT_CONTROL + 0x0020)
#define CT_SERVICEUPLOADSTART_REQ			(CT_CONTROL + 0x0021)
#define CT_SERVICEUPLOADSTART_ACK			(CT_CONTROL + 0x0022)
#define CT_SERVICEUPLOAD_REQ				(CT_CONTROL + 0x0023)
#define CT_SERVICEUPLOAD_ACK				(CT_CONTROL + 0x0024)
#define CT_SERVICEUPLOADEND_REQ				(CT_CONTROL + 0x0025)
#define CT_SERVICEUPLOADEND_ACK				(CT_CONTROL + 0x0026)
#define CT_UPDATEPATCH_REQ					(CT_CONTROL + 0x0027)
#define CT_UPDATEPATCH_ACK					(CT_CONTROL + 0x0028)

////////////////////////////////////////////////////////////////
////// 현승룡 CT_ANNOUNCEMENT /////////////////////////////////
#define CT_ANNOUNCEMENT_REQ					(CT_CONTROL + 0x0029)
#define CT_ANNOUNCEMENT_ACK					(CT_CONTROL + 0x002A)
////// 현승룡 CT_USERKICKOUT //////////////////////////////////
#define CT_USERKICKOUT_REQ					(CT_CONTROL + 0x002B)
#define CT_USERKICKOUT_ACK					(CT_CONTROL + 0x002C)
////// 현승룡 CT_USERMOVE /////////////////////////////////////
#define CT_USERMOVE_REQ						(CT_CONTROL + 0x002D)
#define CT_USERMOVE_ACK						(CT_CONTROL + 0x002E)
////// 현승룡 CT_AUTHORITY ////////////////////////////////////
#define CT_AUTHORITY_ACK					(CT_CONTROL + 0x002F)
////// 현승룡 CT_STLOGIN ////////////////////////////////////
#define CT_STLOGIN_REQ						(CT_CONTROL + 0x0030)
#define CT_STLOGIN_ACK						(CT_CONTROL + 0x0031)
////// 현승룡 CT_ACCOUNTINPUT ////////////////////////////////
#define CT_ACCOUNTINPUT_REQ					(CT_CONTROL + 0x0032)
#define CT_ACCOUNTINPUT_ACK					(CT_CONTROL + 0x0033)
////// 현승룡 CT_PLATFORM     ////////////////////////////////
#define CT_PLATFORM_REQ						(CT_CONTROL + 0x0034)
#define CT_PLATFORM_ACK						(CT_CONTROL + 0x0035)
////// 현승룡 CT_MONSPAWNFIND ////////////////////////////////
#define CT_MONSPAWNFIND_REQ					(CT_CONTROL + 0x0036)
#define CT_MONSPAWNFIND_ACK					(CT_CONTROL + 0x0037)
////// 현승룡 CT_MONACTION    ////////////////////////////////
#define CT_MONACTION_REQ					(CT_CONTROL + 0x0038)
#define CT_MONACTION_ACK					(CT_CONTROL + 0x0039)
////// 현승룡 CT_USERPROTECTED  //////////////////////////////
#define CT_USERPROTECTED_REQ				(CT_CONTROL + 0x003A)
#define CT_USERPROTECTED_ACK				(CT_CONTROL + 0x003B)
////// 현승룡 CT_CHARMSG    ////////////////////////////////
#define CT_CHARMSG_REQ						(CT_CONTROL + 0x003C)
#define CT_CHARMSG_ACK						(CT_CONTROL + 0x003D)
////// 현승룡 CT_LOCALGUILDCHANGE    ////////////////////////////////
#define CT_LOCALGUILDCHANGE_REQ				(CT_CONTROL + 0x003E)
#define CT_LOCALGUILDCHANGE_ACK				(CT_CONTROL + 0x003F)
////// 현승룡 CT_LOCALINIT    ////////////////////////////////
#define CT_LOCALINIT_REQ					(CT_CONTROL + 0x0040)
#define CT_LOCALINIT_ACK					(CT_CONTROL + 0x0041)

#define CT_USERPOSITION_REQ					(CT_CONTROL + 0x0043)
// BYTE		bWorld
// STRING	strTargetName
// STRING	strGMName

#define CT_USERPOSITION_ACK					(CT_CONTROL + 0x0044)
// STRING	strTargetName
// STRING	strGMName

#define CT_SERVICECLOSE_REQ					(CT_CONTROL + 0x0045)

#define CT_RECONNECT_REQ					(CT_CONTROL + 0x0046)
#define CT_DISCONNECT_REQ					(CT_CONTROL + 0x0048)
#define CT_SERVICEAUTOSTART_REQ				(CT_CONTROL + 0x004A)
#define CT_SERVICEAUTOSTART_ACK				(CT_CONTROL + 0x004B)
#define CT_CHATBAN_REQ						(CT_CONTROL + 0x004C)
#define CT_CHATBAN_ACK						(CT_CONTROL + 0x004D)
#define CT_SERVICEDATACLEAR_REQ				(CT_CONTROL + 0x004E)
#define CT_SERVICEDATACLEAR_ACK				(CT_CONTROL + 0x004F)
#define CT_ITEMFIND_REQ						(CT_CONTROL + 0x0050)
#define CT_ITEMFIND_ACK						(CT_CONTROL + 0x0051)
#define CT_ITEMSTATE_REQ					(CT_CONTROL + 0x0052)
#define CT_ITEMSTATE_ACK					(CT_CONTROL + 0x0053)
#define CT_CHATBANLIST_REQ					(CT_CONTROL + 0x0054)
#define CT_CHATBANLIST_ACK					(CT_CONTROL + 0x0055)
#define CT_CHATBANLISTDEL_REQ				(CT_CONTROL + 0x0056)
#define CT_CHATBANLISTDEL_ACK				(CT_CONTROL + 0x0057)
#define CT_CTRLSVR_REQ						(CT_CONTROL + 0x0058)
#define CT_CASTLEINFO_REQ					(CT_CONTROL + 0x0059)
#define CT_CASTLEINFO_ACK					(CT_CONTROL + 0x005A)
#define CT_CASTLEGUILDCHG_REQ				(CT_CONTROL + 0x005B)
#define CT_CASTLEGUILDCHG_ACK				(CT_CONTROL + 0x005C)
#define CT_CASTLEENABLE_REQ					(CT_CONTROL + 0x005D)
#define CT_CASTLEENABLE_ACK					(CT_CONTROL + 0x005E)
#define CT_EVENTUPDATE_REQ					(CT_CONTROL + 0x005F)
#define CT_EVENTUPDATE_ACK					(CT_CONTROL + 0x0060)
#define CT_EVENTCHANGE_REQ					(CT_CONTROL + 0x0061)
#define CT_EVENTCHANGE_ACK					(CT_CONTROL + 0x0062)
#define CT_EVENTLIST_REQ					(CT_CONTROL + 0x0063)
#define CT_EVENTLIST_ACK					(CT_CONTROL + 0x0064)
#define CT_EVENTMSG_REQ						(CT_CONTROL + 0x0065)
#define CT_EVENTMSG_ACK						(CT_CONTROL + 0x0066)
#define CT_EVENTDEL_REQ						(CT_CONTROL + 0x0067)
#define CT_CASHSHOPSTOP_REQ					(CT_CONTROL + 0x0068)
#define CT_CASHITEMSALE_REQ					(CT_CONTROL + 0x0069)
#define CT_CASHITEMSALE_ACK					(CT_CONTROL + 0x006A)
#define CT_CASHITEMLIST_REQ					(CT_CONTROL + 0x006B)
#define CT_CASHITEMLIST_ACK					(CT_CONTROL + 0x006C)
#define CT_EVENTQUARTERUPDATE_REQ			(CT_CONTROL + 0x006D)  
#define CT_EVENTQUARTERUPDATE_ACK			(CT_CONTROL + 0x006E)
#define CT_EVENTQUARTERLIST_REQ				(CT_CONTROL + 0x006F)
#define CT_EVENTQUARTERLIST_ACK				(CT_CONTROL + 0x0070)

#define CT_TOURNAMENTEVENT_REQ				(CT_CONTROL + 0x0071)
#define CT_TOURNAMENTEVENT_ACK				(CT_CONTROL + 0x0072)
#define CT_HELPMESSAGE_REQ					(CT_CONTROL + 0x0073)
#define CT_RPSGAMEDATA_REQ					(CT_CONTROL + 0x0074)
#define CT_RPSGAMEDATA_ACK					(CT_CONTROL + 0x0075)
// WORD wCount
// {
//		BYTE bType
//		BYTE bWinCount
//		BYTE bWinProb
//		BYTE bDrawProb
//		BYTE bLoseProb
//		WORD wWinKeep
//		WORD wWinPeriod
// }
#define CT_RPSGAMECHANGE_REQ				(CT_CONTROL + 0x0076)
// WORD wCount
// {
//		BYTE bType
//		BYTE bWinCount
//		BYTE bWinProb
//		BYTE bDrawProb
//		BYTE bLoseProb
//		WORD wWinKeep
//		WORD wWinPeriod
// }
#define CT_PREVERSIONTABLE_REQ				(CT_CONTROL + 0x0077)
#define CT_PREVERSIONTABLE_ACK				(CT_CONTROL + 0x0078)
#define CT_PREVERSIONUPDATE_REQ				(CT_CONTROL + 0x0079)

#define CT_INSTALLVERSION_REQ				(CT_CONTROL + 0x007A)
#define CT_INSTALLVERSION_ACK				(CT_CONTROL + 0x007B)
#define CT_INSTALLVERSIONUPDATE_REQ			(CT_CONTROL + 0x007C)


////////////////////////////////////////////////////////////////
////// CT_PATCH ///////////////////////////////////////////////
///////////////////////////////////////////////////////////////
#define CT_PATCH_REQ						(CT_PATCH + 0x0001)
#define CT_PATCH_ACK						(CT_PATCH + 0x0002)
#define CT_PATCHEND_REQ						(CT_PATCH + 0x0003)
#define CT_PATCHEND_ACK						(CT_PATCH + 0x0004)
#define CT_PATCHSTART_REQ					(CT_PATCH + 0x0005)
#define CT_PREPATCH_REQ						(CT_PATCH + 0x0006)
#define CT_PREPATCH_ACK						(CT_PATCH + 0x0007)
#define CT_NEWPATCH_REQ						(CT_PATCH + 0x000A)
#define CT_NEWPATCH_ACK						(CT_PATCH + 0x000B)
#define CT_PREPATCHCOMPLETE_REQ				(CT_PATCH + 0x000C)

//	INSTALL SVR
#define CT_INSTALL_REQ						(CT_PATCH + 0x0F01)
#define CT_INSTALL_ACK						(CT_PATCH + 0x0F02)
#define CT_INSATLLSTART_REQ					(CT_PATCH + 0x0F03)
//////////////////////////////////////////////////////////////////////
// TRelaySvr/TWorldSvr Protocol
#define RW_RELAYSVR_REQ						(RW_RELAY + 0x0001)
// WORD wSvrID
#define RW_RELAYSVR_ACK						(RW_RELAY + 0x0002)
// WORD wCount
// {
//		DWORD dwOperatorID
// }
#define RW_ENTERCHAR_REQ					(RW_RELAY + 0x0003)
// DWORD dwCharID
// STRING strName
#define RW_ENTERCHAR_ACK					(RW_RELAY + 0x0004)
// DWORD dwCharID
// STRING strName
// BYTE bResult
// BYTE bCountry
// DWORD dwGuildID
// BYTE bGuildDuty
// WORD wPartyID
// DWORD dwPartyChiefID
// WORD wCorpsID
// DWORD m_dwGeneralID
#define RW_PARTYADD_ACK						(RW_RELAY + 0x0005)
// DWORD dwCharID
// WORD wPartyID
// DWORD dwChiefID
#define RW_PARTYDEL_ACK						(RW_RELAY + 0x0006)
// DWORD dwCharID
// WORD wPartyID
// DWORD dwChief
#define RW_PARTYCHGCHIEF_ACK				(RW_RELAY + 0x0007)
// WORD wPartyID
// DWORD dwChief
#define RW_GUILDADD_ACK						(RW_RELAY + 0x0008)
// DWORD dwCharID
// DWORD dwGuildID
// DWORD dwMasterID
#define RW_GUILDDEL_ACK						(RW_RELAY + 0x0009)
// DWORD dwCharID
// DWORD dwGuildID
#define RW_GUILDCHGMASTER_ACK				(RW_RELAY + 0x000A)
// DWORD dwGuildID
// DWORD dwMasterID
#define RW_CORPSJOIN_ACK					(RW_RELAY + 0x000B)
// WORD wPartyID
// WORD wCorpsID
// WORD wCommander
#define RW_RELAYCONNECT_REQ					(RW_RELAY + 0x000C)
// DWORD dwCharID
#define RW_CHANGENAME_ACK					(RW_RELAY + 0x000D)
// DWORD dwCharID
// STRING strName
#define RW_TACTICSADD_ACK					(RW_RELAY + 0x000E)
#define RW_TACTICSDEL_ACK					(RW_RELAY + 0x000F)
#define RW_CHATBAN_ACK						(RW_RELAY + 0x0010)
// CString strName
// WORD wMinute
