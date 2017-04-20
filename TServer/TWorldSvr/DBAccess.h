#pragma once

#define MAX_STR_GUILD			(1024)
#define MAX_GROUPNAME			(20)

class CTBLGuildChart : public CSqlQuery
{
public:
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

DEF_QUERY( CTBLGuildChart, _T("SELECT bLevel, dwEXP, bMaxCnt, bMinCnt, bCabinetCnt, bTacticsCnt, bBattleSetCnt, bGuardCnt, bRoyalGuardCnt, bTurretCnt, bPeer1, bPeer2, bPeer3, bPeer4, bPeer5 FROM TGUILDCHART"));

BEGIN_COLUMN(15)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwEXP)
	COLUMN_ENTRY(m_bMaxCnt)
	COLUMN_ENTRY(m_bMinCnt)
	COLUMN_ENTRY(m_bCabinetCnt)
	COLUMN_ENTRY(m_bTacticsCnt)
	COLUMN_ENTRY(m_bBattleSetCnt)
	COLUMN_ENTRY(m_bGuardCnt)
	COLUMN_ENTRY(m_bRoyalGuardCnt)
	COLUMN_ENTRY(m_bTurretCnt)
	for(int i=0; i<MAX_GUILD_PEER_COUNT; i++)
	{
		COLUMN_ENTRY(m_bPeer[i])
	}
END_COL()
};

class CTBLGuild : public CSqlQuery
{
public:
	DWORD	m_dwID;
	TCHAR	m_szName[MAX_NAME+1];
	DWORD	m_dwChief;
	BYTE	m_bLevel;
	DWORD	m_dwFame;
	DWORD	m_dwFameColor;
	BYTE	m_bMaxCabinet;
	DWORD	m_dwGI;
	DWORD	m_dwExp;
	BYTE	m_bGPoint;
	BYTE	m_bStatus;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;
	BYTE	m_bDisorg;
	DWORD	m_dwTime;
	TIMESTAMP_STRUCT m_timeEstablish;
	DWORD	m_dwPvPTotalPoint;
	DWORD	m_dwPvPUseablePoint;
	DWORD	m_dwPvPMonthPoint;

DEF_QUERY( CTBLGuild, _T("SELECT \
	dwID, \
	szName, \
	dwChief, \
	bLevel, \
	dwFame, \
	dwFameColor, \
	bMaxCabinet, \
	dwGI, \
	dwExp, \
	bGPoint, \
	bStatus, \
	dwGold, \
	dwSilver, \
	dwCooper, \
	bDisorg, \
	dwTime, \
	timeEstablish, \
	dwPvPTotalPoint, \
	dwPvPUseablePoint, \
	dwPvPMonthPoint \
	FROM TGUILDTABLE"));

BEGIN_COLUMN(20)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwChief)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwFame)
	COLUMN_ENTRY(m_dwFameColor)
	COLUMN_ENTRY(m_bMaxCabinet)
	COLUMN_ENTRY(m_dwGI)
	COLUMN_ENTRY(m_dwExp)
	COLUMN_ENTRY(m_bGPoint)
	COLUMN_ENTRY(m_bStatus)
	COLUMN_ENTRY(m_dwGold)
	COLUMN_ENTRY(m_dwSilver)
	COLUMN_ENTRY(m_dwCooper)
	COLUMN_ENTRY(m_bDisorg)
	COLUMN_ENTRY(m_dwTime)
	COLUMN_ENTRY(m_timeEstablish)
	COLUMN_ENTRY(m_dwPvPTotalPoint)
	COLUMN_ENTRY(m_dwPvPUseablePoint)
	COLUMN_ENTRY(m_dwPvPMonthPoint)
END_COL()
};

class CTBLGuildLoad : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;
	TCHAR	m_szName[MAX_NAME+1];
	DWORD	m_dwChief;
	BYTE	m_bLevel;
	DWORD	m_dwFame;
	DWORD	m_dwFameColor;
	BYTE	m_bMaxCabinet;
	DWORD	m_dwGI;
	DWORD	m_dwExp;
	BYTE	m_bGPoint;
	BYTE	m_bStatus;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;
	BYTE	m_bDisorg;
	DWORD	m_dwTime;
	TIMESTAMP_STRUCT m_timeEstablish;
	DWORD	m_dwPvPTotalPoint;
	DWORD	m_dwPvPUseablePoint;

DEF_QUERY( CTBLGuildLoad, _T("SELECT \
		  dwID, \
		  szName, \
		  dwChief, \
		  bLevel, \
		  dwFame, \
		  dwFameColor, \
		  bMaxCabinet, \
		  dwGI, \
		  dwExp, \
		  bGPoint, \
		  bStatus, \
		  dwGold, \
		  dwSilver, \
		  dwCooper, \
		  bDisorg, \
		  dwTime, \
		  timeEstablish, \
		  dwPvPTotalPoint, \
		  dwPvPUseablePoint \
		  FROM TGUILDTABLE WHERE dwID = ?"));

BEGIN_COLUMN(19)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwChief)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwFame)
	COLUMN_ENTRY(m_dwFameColor)
	COLUMN_ENTRY(m_bMaxCabinet)
	COLUMN_ENTRY(m_dwGI)
	COLUMN_ENTRY(m_dwExp)
	COLUMN_ENTRY(m_bGPoint)
	COLUMN_ENTRY(m_bStatus)
	COLUMN_ENTRY(m_dwGold)
	COLUMN_ENTRY(m_dwSilver)
	COLUMN_ENTRY(m_dwCooper)
	COLUMN_ENTRY(m_bDisorg)
	COLUMN_ENTRY(m_dwTime)
	COLUMN_ENTRY(m_timeEstablish)
	COLUMN_ENTRY(m_dwPvPTotalPoint)
	COLUMN_ENTRY(m_dwPvPUseablePoint)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLGuildArticle : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;
	BYTE	m_bDuty;
	TCHAR	m_szWritter[MAX_NAME+1];
	TCHAR	m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR	m_szArticle[MAX_BOARD_TEXT+1];
	DWORD	m_dwTime;

DEF_QUERY( CTBLGuildArticle, _T("SELECT dwID, bDuty, szWritter, szTitle, szArticle, dwTime FROM TGUILDARTICLETABLE WHERE dwGuildID = ?"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_bDuty)
	COLUMN_ENTRY_STR(m_szWritter)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY_STR(m_szArticle)
	COLUMN_ENTRY(m_dwTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLGuildItem : public CSqlQuery
{
public:
	DWORD m_dwOwnerID;
	BYTE m_bOwnerType;
	BYTE m_bStorageType;
	TIMESTAMP_STRUCT m_dEndTime;
	TITEM m_item;

DEF_QUERY( CTBLGuildItem, _T("SELECT \
	dlID, \
	dwStorageID, \
	wItemID, \
	bLevel, \
	bCount, \
	bGLevel, \
	dwDuraMax, \
	dwDuraCur, \
	bRefineCur, \
	dEndTime, \
	bGradeEffect, \
	bMagic1, \
	bMagic2, \
	bMagic3, \
	bMagic4, \
	bMagic5, \
	bMagic6, \
	wValue1, \
	wValue2, \
	wValue3, \
	wValue4, \
	wValue5, \
	wValue6, \
	dwTime1, \
	dwTime2, \
	dwTime3, \
	dwTime4, \
	dwTime5, \
	dwTime6 \
	FROM TITEMTABLE \
	WHERE dwOwnerID = ? AND bOwnerType=? AND bStorageType=?"))

BEGIN_COLUMN(29)
	COLUMN_ENTRY(m_item.m_dlID)
	COLUMN_ENTRY(m_item.m_dwItemID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_dEndTime)
	COLUMN_ENTRY(m_item.m_bGradeEffect)
	COLUMN_ENTRY(m_item.m_bMagic[0])
	COLUMN_ENTRY(m_item.m_bMagic[1])
	COLUMN_ENTRY(m_item.m_bMagic[2])
	COLUMN_ENTRY(m_item.m_bMagic[3])
	COLUMN_ENTRY(m_item.m_bMagic[4])
	COLUMN_ENTRY(m_item.m_bMagic[5])
	COLUMN_ENTRY(m_item.m_wValue[0])
	COLUMN_ENTRY(m_item.m_wValue[1])
	COLUMN_ENTRY(m_item.m_wValue[2])
	COLUMN_ENTRY(m_item.m_wValue[3])
	COLUMN_ENTRY(m_item.m_wValue[4])
	COLUMN_ENTRY(m_item.m_wValue[5])
	COLUMN_ENTRY(m_item.m_dwExtValue[0])
	COLUMN_ENTRY(m_item.m_dwExtValue[1])
	COLUMN_ENTRY(m_item.m_dwExtValue[2])
	COLUMN_ENTRY(m_item.m_dwExtValue[3])
	COLUMN_ENTRY(m_item.m_dwExtValue[4])
	COLUMN_ENTRY(m_item.m_dwExtValue[5])
END_COL()

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStorageType)
END_PARAM()
};

class CTBLGuildVolunteer : public CSqlQuery
{
public:
	BYTE	m_bType;
	DWORD	m_dwID;
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bLevel;
	BYTE	m_bClass;

DEF_QUERY(CTBLGuildVolunteer, _T("SELECT bType, dwID, dwCharID, szName, bLevel, bClass FROM TVIEW_GUILDVOLUNTEERTABLE"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
END_COL()
};

class CTBLGuildMember : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwMemberID;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bDuty;
	BYTE	m_bPeer;
	BYTE	m_bCountry;
	BYTE	m_bWarCountry;
	TIMESTAMP_STRUCT	m_dConnectedDate;
	
DEF_QUERY(CTBLGuildMember, _T("SELECT dwCharID, szName, bCountry, bWarCountry, bLevel, bClass, bDuty, bPeer, dLogoutDate FROM TGUILDMEMBER WHERE dwGuildID = ?"));

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_dwMemberID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bWarCountry)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bDuty)
	COLUMN_ENTRY(m_bPeer)
	COLUMN_ENTRY(m_dConnectedDate);
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLGuildTactics : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwMemberID;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bLevel;
	BYTE	m_bClass;
	DWORD	m_dwRewardPoint;
	INT64	m_dlRewardMoney;
	DWORD	m_dwGainPoint;
	BYTE	m_bDay;
	TIMESTAMP_STRUCT m_timeEnd;

DEF_QUERY(CTBLGuildTactics, _T("SELECT dwCharID, szName, bLevel, bClass, dwRewardPoint, dlRewardMoney, dwGainPoint, bDay, dEndTime FROM TVIEW_GUILDTACTICSTABLE WHERE dwGuildID = ?"));

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_dwMemberID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_dwRewardPoint)
	COLUMN_ENTRY(m_dlRewardMoney)
	COLUMN_ENTRY(m_dwGainPoint)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_timeEnd)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLGuildWanted : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	BYTE	m_bMinLevel;
	BYTE	m_bMaxLevel;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR	m_szTitle[MAX_BOARD_TITLE + 1];
	TCHAR	m_szText[MAX_BOARD_TEXT + 1];

DEF_QUERY(CTBLGuildWanted, _T("SELECT dwGuildID, bMinLevel, bMaxLevel, dEndTime, szTitle, szText FROM TGUILDWANTEDTABLE"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwGuildID)
	COLUMN_ENTRY(m_bMinLevel)
	COLUMN_ENTRY(m_bMaxLevel)
	COLUMN_ENTRY(m_timeEnd)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY_STR(m_szText)
END_COL()
};

class CTBLGuildTacticsWanted : public CSqlQuery
{
public:
	DWORD	m_dwID;
	DWORD	m_dwGuildID;
	BYTE	m_bMinLevel;
	BYTE	m_bMaxLevel;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR	m_szTitle[MAX_BOARD_TITLE + 1];
	TCHAR	m_szText[MAX_BOARD_TEXT + 1];
	BYTE	m_bDay;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;
	DWORD	m_dwPvPoint;

DEF_QUERY(CTBLGuildTacticsWanted, _T("SELECT \
									 dwID, \
									 dwGuildID, \
									 bMinLevel, \
									 bMaxLevel, \
									 dEndTime, \
									 szTitle, \
									 szText, \
									 bDay, \
									 dwGold, \
									 dwSilver, \
									 dwCooper, \
									 dwPvPoint \
									 FROM TGUILDTACTICSWANTEDTABLE ORDER BY dwID"));

BEGIN_COLUMN(12)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_dwGuildID)
	COLUMN_ENTRY(m_bMinLevel)
	COLUMN_ENTRY(m_bMaxLevel)
	COLUMN_ENTRY(m_timeEnd)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY_STR(m_szText)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_dwGold)
	COLUMN_ENTRY(m_dwSilver)
	COLUMN_ENTRY(m_dwCooper)
	COLUMN_ENTRY(m_dwPvPoint)
END_COL()
};

class CTBLGuildRelation : public CSqlQuery
{
public:
	BYTE	m_bType;
	DWORD	m_dwGuildOne;
	DWORD	m_dwGuildTwo;

DEF_QUERY(CTBLGuildRelation, _T("SELECT bType, dwGuildOne, dwGuildTwo FROM TGUILDRELATION"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwGuildOne)
	COLUMN_ENTRY(m_dwGuildTwo)
END_COL()
};

class CTBLFriendGroupTable : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	BYTE	m_bGroup;
	TCHAR	m_szName[MAX_GROUPNAME+1];

DEF_QUERY(CTBLFriendGroupTable, _T("SELECT bGroup, szName FROM TFRIENDGROUPTABLE WHERE dwCharID = ?"));
BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY_STR(m_szName)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLFriend : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwFriendID;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bGroup;
	BYTE	m_bClass;
	BYTE	m_bLevel;

DEF_QUERY(CTBLFriend, _T("SELECT F.dwFriendID, C.szName, F.bGroup, C.bClass, C.bLevel \
						 FROM TFRIENDTABLE AS F INNER JOIN TCHARTABLE AS C ON F.dwFriendID=C.dwCharID \
						 WHERE F.dwCharID = ? AND F.dwFriendID = C.dwCharID AND C.bDelete = 0"));

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_dwFriendID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bLevel)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLFriendTarget : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwFriendID;
	TCHAR	m_szName[MAX_NAME+1];

DEF_QUERY(CTBLFriendTarget, _T("SELECT F.dwCharID, C.szName \
							   FROM TFRIENDTABLE AS F INNER JOIN TCHARTABLE AS C ON F.dwCharID=C.dwCharID \
							   WHERE F.dwFriendID = ? AND  F.dwCharID = C.dwCharID AND C.bDelete = 0"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwFriendID)
	COLUMN_ENTRY_STR(m_szName)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLSoulmate : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwTarget;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bLevel;
	BYTE	m_bClass;
	DWORD	m_dwTime;

DEF_QUERY(CTBLSoulmate, _T("SELECT dwCharID, dwTarget, szNAME, bLevel, bClass, dwTime \
						   FROM TVIEW_SOULMATE \
						   WHERE dwTarget = ?"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_dwTarget)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_dwTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLMySoulmate : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwTarget;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bLevel;
	BYTE	m_bClass;
	DWORD	m_dwTime;

DEF_QUERY(CTBLMySoulmate, _T("SELECT dwCharID, dwTarget, szNAME, bLevel, bClass, dwTime \
						   FROM TVIEW_SOULMATE \
						   WHERE dwCharID = ?"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_dwTarget)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_dwTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLOperatorChart : public CSqlQuery
{
public:
	DWORD m_dwOperator;

DEF_QUERY(CTBLOperatorChart, _T("SELECT dwOperatorID FROM TOPERATORTABLE"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwOperator)
END_COL()
};

class CTBLEventQuarterChart : public CSqlQuery
{
public:
	BYTE m_bDay;
	BYTE m_bHour;
	BYTE m_bMinute;
	WORD m_wID;
	TCHAR m_szPresent[MAX_NAME + 1];
	TCHAR m_szAnnounce[MAX_STR_GUILD + 1];

DEF_QUERY(CTBLEventQuarterChart, _T("SELECT wID, bDay, bHour, bMinute, szPresent, szAnnounce FROM TEVENTQUARTERCHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_bHour)
	COLUMN_ENTRY(m_bMinute)
	COLUMN_ENTRY_STR(m_szPresent)
	COLUMN_ENTRY_STR(m_szAnnounce)
END_COL()
};

class CTBLBattleTimeChart : public CSqlQuery
{
public:
	BYTE m_bType;
	DWORD m_dwBattleDur;
	DWORD m_dwBattleStart;
	DWORD m_dwAlarmStart;
	DWORD m_dwAlarmEnd;
	DWORD m_dwPeaceDur;
	BYTE m_bDay;
	BYTE m_bWeek;

DEF_QUERY(CTBLBattleTimeChart, _T("SELECT bType, dwBattleDur, dwBattleStart, dwAlarmStart, dwAlarmEnd, dwPeaceDur, bDay, bWeek FROM TBATTLETIMECHART"))

BEGIN_COLUMN(8)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwBattleDur)
	COLUMN_ENTRY(m_dwBattleStart)
	COLUMN_ENTRY(m_dwAlarmStart)
	COLUMN_ENTRY(m_dwAlarmEnd)
	COLUMN_ENTRY(m_dwPeaceDur)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_bWeek)
END_COL()
};

class CTBLSvrMsg : public CSqlQuery
{
public:
	DWORD m_dwID;
	TCHAR m_szMessage[MAX_BOARD_TITLE + 1];

DEF_QUERY(CTBLSvrMsg, _T("SELECT dwID, szMessage FROM TSVRMSGCHART"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY_STR(m_szMessage)
END_COL()
};

class CTBLItemFind : public CSqlQuery
{
public:
	WORD	m_wFindID;
	WORD	m_wItemID;
	BYTE	m_bInitState;
	TCHAR	m_szName[MAX_NAME+1];
	TCHAR	m_szFindName[MAX_NAME+1];
	
DEF_QUERY(CTBLItemFind, _T("SELECT wItemID,bInitState,szName FROM TITEMCHART WHERE szName LIKE ? OR wItemID = ? "))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bInitState)
	COLUMN_ENTRY_STR(m_szName)
END_COL()

BEGIN_PARAM(2)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szFindName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wFindID)	
END_PARAM()
};

class CTBLGuildPvPointReward : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	TCHAR m_szName[MAX_NAME];
	DWORD m_dwPoint;
	TIMESTAMP_STRUCT m_dlDate;

DEF_QUERY(CTBLGuildPvPointReward, _T("SELECT TOP 50 szName, dwPoint, dlDate FROM TGUILDPVPOINTREWARDTABLE WHERE dwGuildID = ? ORDER BY dlDate DESC"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwPoint)
	COLUMN_ENTRY(m_dlDate)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLGuildPvPRecord : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwCharID;
	DWORD m_dwDate;
	WORD m_wKillCount;
	WORD m_wDieCount;
	DWORD m_aPoint[PVPE_COUNT];

DEF_QUERY(CTBLGuildPvPRecord, _T("SELECT \
	dwCharID, \
	dwDate, \
	wKillCount, \
	wDieCount, \
	dwPoint_1, \
	dwPoint_2, \
	dwPoint_3, \
	dwPoint_4, \
	dwPoint_5, \
	dwPoint_6, \
	dwPoint_7, \
	dwPoint_8 \
	FROM TGUILDPVPRECORDTABLE WHERE dwGuildID = ? "))

BEGIN_COLUMN(12)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_dwDate)
	COLUMN_ENTRY(m_wKillCount)
	COLUMN_ENTRY(m_wDieCount)
	for(BYTE i=0; i<PVPE_COUNT; i++)
	{
		COLUMN_ENTRY(m_aPoint[i]);
	}
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLCastleApplicant : public CSqlQuery
{
public:
	WORD m_wCastle;
	DWORD m_dwCharID;
	BYTE m_bCamp;

DEF_QUERY(CTBLCastleApplicant, _T("SELECT wCastleID, dwCharID, bCamp FROM TCASTLEAPPLICANTTABLE"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wCastle)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_bCamp)
END_COL()
};
 
class CTBLMonthPvPointTable : public CSqlQuery
{
public:	
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME +1];
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
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];
	TCHAR	m_szGuild[MAX_NAME +1];

DEF_QUERY(CTBLMonthPvPointTable, _T("SELECT TOP 16 \
	M.dwCharID,  \
	C.szName, \
	P.dwTotalPoint,  \
	M.dwPoint,  \
	M.wWin,  \
	M.wLose, \
	R.dwWarrior_win + R.dwRanger_win + R.dwArcher_win + R.dwWizard_win + R.dwPriest_win + R.dwSorcerer_win, \
	R.dwWarrior_lose + R.dwRanger_lose + R.dwArcher_lose + R.dwWizard_lose + R.dwPriest_lose + R.dwSorcerer_lose, \
	C.bLevel,  \
	C.bClass,  \
	C.bRace,  \
	C.bSex,  \
	C.bHair,  \
	C.bFace,  \
	M.szSay, \
	U.szName \
	FROM TMONTHPVPOINTTABLE AS M  \
	INNER JOIN TCHARTABLE AS C ON C.dwCharID = M.dwCharID  \
	INNER JOIN TPVPOINTTABLE AS P ON  P.dwCharID = M.dwCharID \
	INNER JOIN TPVPRECORDTABLE AS R ON R.dwCharID = M.dwCharID  \
	LEFT JOIN TGUILDMEMBERTABLE AS G ON M.dwCharID = G.dwCharID  \
	LEFT JOIN TGUILDTABLE AS U ON G.dwGuildID = U.dwID \
	WHERE C.bCountry = ? AND M.dwPoint > 0 \
	ORDER BY dwPoint DESC, wWin DESC, wLose DESC, C.dwCharID ASC"))

BEGIN_COLUMN(16)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwTotalPoint)
	COLUMN_ENTRY(m_dwMonthPoint)
	COLUMN_ENTRY(m_wMonthWin)
	COLUMN_ENTRY(m_wMonthLose)
	COLUMN_ENTRY(m_dwTotalWin)
	COLUMN_ENTRY(m_dwTotalLose)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY_STR(m_szSay)
	COLUMN_ENTRY_STR(m_szGuild)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCountry)
END_PARAM()
};

class CTBLMonthPvPointChar : public CSqlQuery
{
public:	
	DWORD	m_dwCharID;	
	DWORD	m_dwMonthPoint;
	WORD	m_wMonthWin;
	WORD	m_wMonthLose;	
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];

DEF_QUERY(CTBLMonthPvPointChar, _T("SELECT \
	dwPoint, \
	wWin, \
	wLose, \
	szSay \
	FROM TMONTHPVPOINTTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwMonthPoint)
	COLUMN_ENTRY(m_wMonthWin)
	COLUMN_ENTRY(m_wMonthLose)
	COLUMN_ENTRY_STR(m_szSay)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
END_PARAM()
};


class CTBLPvPointTable : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME +1];
	DWORD	m_dwTotalPoint;
	BYTE	m_bCountry;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bRace;
	BYTE	m_bSex;
	BYTE	m_bHair;
	BYTE	m_bFace;
	DWORD	m_dwTotalWin;
	DWORD	m_dwTotalLose;
	TCHAR	m_szGuild[MAX_NAME +1];

DEF_QUERY(CTBLPvPointTable,_T("SELECT TOP 1 \
	P.dwCharID, \
	C.szName, \
	P.dwTotalPoint, \
	C.bLevel, \
	C.bClass, \
	C.bRace, \
	C.bSex, \
	C.bHair, \
	C.bFace, \
	R.dwWarrior_win + R.dwRanger_win + R.dwArcher_win + R.dwWizard_win + R.dwPriest_win + R.dwSorcerer_win, \
	R.dwWarrior_lose + R.dwRanger_lose + R.dwArcher_lose + R.dwWizard_lose + R.dwPriest_lose + R.dwSorcerer_lose, \
	U.szName \
	FROM TPVPOINTTABLE AS P \
	INNER JOIN TCHARTABLE AS C ON C.dwCharID = P.dwCharID \
	INNER JOIN TPVPRECORDTABLE AS R ON P.dwCharID = R.dwCharID \
	LEFT JOIN TGUILDMEMBERTABLE AS G ON P.dwCharID = G.dwCharID \
	LEFT JOIN TGUILDTABLE AS U ON G.dwGuildID = U.dwID \
	WHERE C.bCountry = ? \
	ORDER BY P.dwTotalPoint DESC, P.dwCharID ASC"))

BEGIN_COLUMN(12)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwTotalPoint)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY(m_dwTotalWin)
	COLUMN_ENTRY(m_dwTotalLose)
	COLUMN_ENTRY_STR(m_szGuild)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCountry)
END_PARAM()
};

class CTBLStopMap : public CSqlQuery
{
public:
DEF_QUERY(CTBLStopMap, _T("DELETE TDBITEMINDEXTABLE"))
};

class CTBLProtectChar : public CSqlQuery
{
public:
DEF_QUERY(CTBLProtectChar, _T("UPDATE TCHARTABLE SET wMapID = 2048, wSpawnID = 30000"))
};

class CTBLDeleteLevel : public CSqlQuery
{
public:
DEF_QUERY(CTBLDeleteLevel, _T("DELETE TLEVELCHART"))
};
 
class CTBLEventQuarterList : public CSqlQuery
{
public:
	BYTE	m_bFindDay;
	BYTE	m_bDay;
	BYTE	m_bHour;
	BYTE	m_bMinute;
	WORD	m_wID;
	WORD	m_wItemID1;
	WORD	m_wItemID2;
	WORD	m_wItemID3;
	WORD	m_wItemID4;
	WORD	m_wItemID5;
	BYTE	m_bCount;
	TCHAR	m_szPresent[MAX_NAME +1];
	TCHAR	m_szAnnounce[MAX_STR_GUILD +1];
	TCHAR	m_szTitle[MAX_NAME +1];
	TCHAR	m_szMessage[501];
	TCHAR	m_szItem1[MAX_NAME +1];
	TCHAR	m_szItem2[MAX_NAME +1];
	TCHAR	m_szItem3[MAX_NAME +1];
	TCHAR	m_szItem4[MAX_NAME +1];
	TCHAR	m_szItem5[MAX_NAME +1];

DEF_QUERY(CTBLEventQuarterList,_T("SELECT \
	wID, \
	bDay, \
	bHour,\
	bMinute, \
	wItemID1, \
	wItemID2, \
	wItemID3, \
	wItemID4, \
	wItemID5, \
	bCount, \
	szPresent, \
	szAnnounce, \
	szTitle, \
	szMessage \
	FROM TEVENTQUARTERCHART WHERE bDay = ? \
	ORDER BY bHour, bMinute"))

BEGIN_COLUMN(14)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_bHour)
	COLUMN_ENTRY(m_bMinute)
	COLUMN_ENTRY(m_wItemID1)
	COLUMN_ENTRY(m_wItemID2)
	COLUMN_ENTRY(m_wItemID3)
	COLUMN_ENTRY(m_wItemID4)
	COLUMN_ENTRY(m_wItemID5)
	COLUMN_ENTRY(m_bCount)
	COLUMN_ENTRY_STR(m_szPresent)
	COLUMN_ENTRY_STR(m_szAnnounce)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY_STR(m_szMessage)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bFindDay)
END_PARAM()
};
class CTBLTournamentCurrentStep : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bGroup;
	BYTE m_bStep;

DEF_QUERY(CTBLTournamentCurrentStep, _T("SELECT wID, bGroup, bStep FROM TTOURNAMENTSTATUSTABLE"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY(m_bStep)
END_COL()
};

class CTBLTournamentSchedule : public CSqlQuery
{
public:
	BYTE m_bGroup;
	BYTE m_bStep;
	DWORD m_dwPeriod;

DEF_QUERY(CTBLTournamentSchedule, _T("SELECT \
	bGroup, \
	bStep, \
	dwPeriod \
	FROM TTOURNAMENTSCHEDULECHART \
	WHERE bGroup = 0 OR bGroup IN(SELECT bGroup FROM TTOURNAMENTCHART WHERE bEnable=1)"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY(m_bStep)
	COLUMN_ENTRY(m_dwPeriod)
END_COL()
};

class CTBLTournament : public CSqlQuery
{
public:
	BYTE m_bGroup;
	BYTE m_bEntryID;
	TCHAR m_szName[MAX_NAME+1];
	BYTE m_bType;
	DWORD m_dwClass;
	DWORD m_dwFee;
	DWORD m_dwFeeBack;
	WORD m_wPermitItemID;
	BYTE m_bPermitCount;

DEF_QUERY(CTBLTournament, _T("SELECT \
	bGroup, \
	bEntryID, \
	szName, \
	bType, \
	dwClass, \
	dwFee, \
	dwFeeBack, \
	wItemID, \
	bItemCount \
	FROM TTOURNAMENTCHART WHERE bEnable = 1"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY(m_bEntryID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwClass)
	COLUMN_ENTRY(m_dwFee)
	COLUMN_ENTRY(m_dwFeeBack)
	COLUMN_ENTRY(m_wPermitItemID)
	COLUMN_ENTRY(m_bPermitCount)
END_COL()
};

class CTBLTournamentReward : public CSqlQuery
{
public:
	BYTE m_bEntryID;
	DWORD m_dwClass;
	BYTE m_bCheckShield;
	BYTE m_bChartType;
	WORD m_wItemID;
	BYTE m_bCount;

DEF_QUERY(CTBLTournamentReward, _T("SELECT \
	bEntryID, \
	dwClass, \
	bCheckShield, \
	bChartType, \
	wItemID, \
	bItemCount \
	FROM TTOURNAMENTREWARDCHART WHERE bItemCount > 0"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bEntryID)
	COLUMN_ENTRY(m_dwClass)
	COLUMN_ENTRY(m_bCheckShield)
	COLUMN_ENTRY(m_bChartType)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bCount)
END_COL()
};

class CTBLTnmtEventTime : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bWeek;
	BYTE m_bDay;
	DWORD m_dwStart;

DEF_QUERY(CTBLTnmtEventTime, _T("SELECT \
	wTournamentID, \
	bWeek, \
	bDay, \
	dwStart \
	FROM TTNMTEVENTTIMETABLE"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wTourID)
	COLUMN_ENTRY(m_bWeek)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_dwStart)
END_COL()
};

class CTBLTnmtEventSchedule : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bStep;
	DWORD m_dwPeriod;

DEF_QUERY(CTBLTnmtEventSchedule, _T("SELECT \
	bStep, \
	dwPeriod \
	FROM TTNMTEVENTSCHEDULETABLE \
	WHERE wTournamentID = ?"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bStep)
	COLUMN_ENTRY(m_dwPeriod)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wTourID)
END_PARAM()
};

class CTBLTournamentEvent : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bEntryID;
	TCHAR m_szName[MAX_NAME+1];
	BYTE m_bType;
	DWORD m_dwClass;
	DWORD m_dwFee;
	DWORD m_dwFeeBack;
	WORD m_wPermitItemID;
	BYTE m_bPermitCount;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;

DEF_QUERY(CTBLTournamentEvent, _T("SELECT \
	bEntryID, \
	szName, \
	bType, \
	dwClass, \
	dwFee, \
	dwFeeBack, \
	wItemID, \
	bItemCount, \
	bMinLevel,  \
	bMaxLevel  \
	FROM TTNMTEVENTTABLE \
	WHERE wTournamentID = ?"))

BEGIN_COLUMN(10)
	COLUMN_ENTRY(m_bEntryID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwClass)
	COLUMN_ENTRY(m_dwFee)
	COLUMN_ENTRY(m_dwFeeBack)
	COLUMN_ENTRY(m_wPermitItemID)
	COLUMN_ENTRY(m_bPermitCount)
	COLUMN_ENTRY(m_bMinLevel)
	COLUMN_ENTRY(m_bMaxLevel)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wTourID)
END_PARAM()
};

class CTBLTnmtEventReward : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bEntryID;
	DWORD m_dwClass;
	BYTE m_bCheckShield;
	BYTE m_bChartType;
	WORD m_wItemID;
	BYTE m_bCount;

DEF_QUERY(CTBLTnmtEventReward, _T("SELECT \
	bEntryID, \
	dwClass, \
	bCheckShield, \
	bChartType, \
	wItemID, \
	bItemCount \
	FROM TTNMTEVENTREWARDTABLE \
	WHERE wTournamentID = ?"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bEntryID)
	COLUMN_ENTRY(m_dwClass)
	COLUMN_ENTRY(m_bCheckShield)
	COLUMN_ENTRY(m_bChartType)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wTourID)
END_PARAM()
};

class CTBLGetCharInfo : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bCountry;
	BYTE m_bLevel;
	BYTE m_bClass;
	TCHAR m_szFindName[MAX_NAME +1];
	TCHAR m_szName[MAX_NAME +1];

DEF_QUERY(CTBLGetCharInfo, _T("SELECT \
	dwCharID, \
	szName, \
	bCountry, \
	bLevel, \
	bClass \
	FROM TCHARTABLE WHERE szName = ?"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szFindName)
END_PARAM()
};

class CTBLTournamentPlayer : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	TCHAR m_szName[MAX_NAME + 1];
	DWORD m_dwChiefID;
	BYTE m_bClass;
	BYTE m_bCountry;
	BYTE m_bLevel;
	BYTE m_bEntry;
	BYTE m_bStep;
	BYTE m_bResult;

DEF_QUERY(CTBLTournamentPlayer, _T("SELECT \
	dwCharID, \
	szName, \
	dwChiefID, \
	bClass, \
	bCountry, \
	bLevel, \
	bEntry, \
	bStep, \
	bResult \
	FROM TVIEW_TOURNAMENTPLAYER"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwChiefID)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bEntry)
	COLUMN_ENTRY(m_bStep)
	COLUMN_ENTRY(m_bResult)
END_COL()
};

class CTBLFirstGradeGroup : public CSqlQuery
{
public:
	BYTE	m_bMonth;
	BYTE	m_bRank;	
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME +1];
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
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];
	TCHAR	m_szGuild[MAX_NAME +1];

DEF_QUERY(CTBLFirstGradeGroup, _T("SELECT \
	bRank, \
	dwCharID, \
	szName, \
	dwTotalPoint, \
	dwMonthPoint, \
	wMonthWin, \
	wMonthLose, \
	dwTotalWin, \
	dwTotalLose, \
	bCountry, \
	bLevel, \
	bClass, \
	bRace, \
	bSex, \
	bHair, \
	bFace, \
	szSay, \
	szGuild \
	FROM TMONTHRANKTABLE WHERE bMonth = ? ORDER BY bCountry,bRank"))

BEGIN_COLUMN(18)
	COLUMN_ENTRY(m_bRank)	
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwTotalPoint)
	COLUMN_ENTRY(m_dwMonthPoint)
	COLUMN_ENTRY(m_wMonthWin)
	COLUMN_ENTRY(m_wMonthLose)
	COLUMN_ENTRY(m_dwTotalWin)
	COLUMN_ENTRY(m_dwTotalLose)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY_STR(m_szSay)
	COLUMN_ENTRY_STR(m_szGuild)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonth)
END_PARAM()
};

class CTBLTotalRank : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwRank;

DEF_QUERY(CTBLTotalRank, _T("SELECT dwCharID, dwRank FROM TLASTTOTALPOINTTABLE"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwCharID)	
	COLUMN_ENTRY(m_dwRank)
END_COL()
};

class CTBLMonthRank : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwRank;

DEF_QUERY(CTBLMonthRank, _T("SELECT dwCharID, dwRank FROM TLASTMONTHPOINTTABLE"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwCharID)	
	COLUMN_ENTRY(m_dwRank)
END_COL()
};

class CTBLRPSGame : public CSqlQuery
{
public:
	BYTE m_bType;
	BYTE m_bWinCount;
	WORD m_wWinKeep;
	WORD m_wWinPeriod;
	BYTE m_bProb[3];

DEF_QUERY(CTBLRPSGame, _T("SELECT \
	bType, \
	bWinCount, \
	bProb_Win, \
	bProb_Draw, \
	bProb_Lose, \
	wWinKeep, \
	wWinPeriod \
	FROM TRPSGAMECHART"))

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bWinCount)
	COLUMN_ENTRY(m_bProb[0])
	COLUMN_ENTRY(m_bProb[1])
	COLUMN_ENTRY(m_bProb[2])
	COLUMN_ENTRY(m_wWinKeep)
	COLUMN_ENTRY(m_wWinPeriod)
END_COL()
};

class CTBLRPSGameRecord : public CSqlQuery
{
public:
	BYTE m_bType;
	BYTE m_bWinCount;
	TIMESTAMP_STRUCT m_dWinDate;
	DWORD m_dwCharID;

DEF_QUERY(CTBLRPSGameRecord, _T("SELECT \
	bType, \
	bWinCount, \
	dWinDate, \
	dwCharID \
	FROM TRPSGAMERECORDTABLE"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bWinCount)
	COLUMN_ENTRY(m_dWinDate)
	COLUMN_ENTRY(m_dwCharID)
END_COL()
};

class CTBLActiveCharTable : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bCountry;
	BYTE m_bAidCountry;
	BYTE m_bLevel;

DEF_QUERY(CTBLActiveCharTable, _T("SELECT \
	AC.dwCharID, \
	CT.bCountry, \
	AT.bCountry, \
	CT.bLevel \
	FROM TACTIVECHARTABLE AS AC INNER JOIN TCHARTABLE AS CT ON AC.dwCharID=CT.dwCharID \
	LEFT OUTER JOIN TAIDTABLE AS AT ON AC.dwCharID = AT.dwCharID"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bAidCountry)
	COLUMN_ENTRY(m_bLevel)
END_COL()
};

class CTBLActiveCharDel : public CSqlQuery
{
public:
	TIMESTAMP_STRUCT m_dDate;

DEF_QUERY(CTBLActiveCharDel, _T("DELETE TACTIVECHARTABLE WHERE dateEnter < ?"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dDate)
END_PARAM()
};

class CTBLCMGiftChart : public CSqlQuery
{
public:
	WORD	m_wGiftID;
	BYTE	m_bGiftType;
	DWORD	m_dwValue;
	BYTE	m_bCount;
	BYTE	m_bTakeType;
	BYTE	m_bMaxTakeCount;
	BYTE	m_bToolOnly;
	WORD	m_wErrGiftID;
	TCHAR	m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR	m_szMsg[MAX_BOARD_MSG+1];

DEF_QUERY(CTBLCMGiftChart, _T("SELECT \
	wGiftID, \
	bGiftType, \
	dwValue, \
	bCount, \
	bTakeType, \
	bMaxTakeCount, \
	bToolOnly, \
	wErrGiftID, \
	szTitle, \
	szMsg \
	FROM TCMGIFTCHART"))

BEGIN_COLUMN(10)
	COLUMN_ENTRY(m_wGiftID)
	COLUMN_ENTRY(m_bGiftType)
	COLUMN_ENTRY(m_dwValue)
	COLUMN_ENTRY(m_bCount)
	COLUMN_ENTRY(m_bTakeType)
	COLUMN_ENTRY(m_bMaxTakeCount)
	COLUMN_ENTRY(m_bToolOnly)
	COLUMN_ENTRY(m_wErrGiftID)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY_STR(m_szMsg)
END_PARAM()
};
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////   SP   ///////////////////////////////////////////////////////
class CSPGuildUpdate : public CSqlQuery
{
public:
	DWORD	m_dwID;
	BYTE	m_bFame;
	BYTE	m_bGPoint;
	BYTE	m_bLevel;
	BYTE	m_bStatus;
	DWORD	m_dwChief;
	DWORD	m_dwGI;
	DWORD	m_dwExp;
	DWORD	m_dwTime;
	TCHAR	m_szAllience[MAX_STR_GUILD];
	TCHAR	m_szEnemy[MAX_STR_GUILD];

DEF_QUERY(CSPGuildUpdate, _T("{ Call TGuildUpdate(?,?,?,?,?,?,?,?,?,?,?) }"))

BEGIN_PARAM(11)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bFame)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bStatus)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwChief)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGI)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwExp)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTime)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szAllience)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szEnemy)
END_PARAM()
};

class CSPGuildEstablish : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwGuildID;
	TCHAR	m_szName[MAX_NAME+1];
	DWORD	m_dwChiefID;
	TIMESTAMP_STRUCT m_timeEstablish;

DEF_QUERY(CSPGuildEstablish, _T("{ ? = Call TGuildEstablish(?,?,?,?) }"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwGuildID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwChiefID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeEstablish)
END_PARAM();
};

class CSPGuildDisorg : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	BYTE	m_bDisorg;
	DWORD	m_dwTime;

DEF_QUERY(CSPGuildDisorg, _T("{ Call TGuildDisorg(?,?,?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDisorg)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTime)
END_PARAM();
};

class CSPGuildDelete : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwGuildID;

DEF_QUERY(CSPGuildDelete, _T("{ ? = Call TGuildDelete(?) }"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM();
};

class CSPGuildMemberAdd : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwCharID;
	BYTE	m_bLevel;
	BYTE	m_bDuty;

DEF_QUERY(CSPGuildMemberAdd, _T("{ Call TGuildMemberAdd(?,?,?,?) }"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDuty)
END_PARAM();
};

class CSPGuildLeave : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwCharID;
	BYTE 	m_bLeave;
	DWORD	m_dwLeaveTime;

DEF_QUERY(CSPGuildLeave, _T("{ Call TGuildLeave(?,?,?,?) }"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bLeave)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwLeaveTime)
END_PARAM();
};

class CSPGuildDuty : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD 	m_dwGuildID;
	BYTE	m_bDuty;

DEF_QUERY(CSPGuildDuty, _T("{ Call TGuildDuty(?,?,?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDuty)
END_PARAM();
};

class CSPGuildPeer : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD 	m_dwGuildID;
	BYTE	m_bPeer;

DEF_QUERY(CSPGuildPeer, _T("{ Call TGuildPeer(?,?,?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bPeer)
END_PARAM();
};


class CSPGuildKickout : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwCharID;

DEF_QUERY(CSPGuildKickout, _T("{ Call TGuildKickout(?,?) }"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM();
};

class CSPFriendInsert : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwFriendID;

DEF_QUERY(CSPFriendInsert, _T("{ Call TFriendInsert(?,?) }"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFriendID)
END_PARAM();
};

class CSPFriendErase : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwFriendID;

DEF_QUERY(CSPFriendErase, _T("{ Call TFriendDelete(?,?) }"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFriendID)
END_PARAM();
};

class CSPFriendGroupMake : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bGroup;
	TCHAR m_szName[MAX_GROUPNAME + 1];

DEF_QUERY(CSPFriendGroupMake, _T("{ Call TFriendGroupMake(?, ?, ?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGroup)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
END_PARAM();
};

class CSPFriendGroupDelete : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bGroup;

DEF_QUERY(CSPFriendGroupDelete, _T("{ Call TFriendGroupDelete(?, ?) }"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGroup)
END_PARAM();
};

class CSPFriendGroupChange : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bGroup;
	DWORD m_dwFriendID;

DEF_QUERY(CSPFriendGroupChange, _T("{ Call TFriendGroupChange(?, ?, ?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGroup)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFriendID)
END_PARAM();
};

class CSPFriendGroupName : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bGroup;
	TCHAR m_szName[MAX_GROUPNAME+1];

DEF_QUERY(CSPFriendGroupName, _T("{ Call TFriendGroupName(?, ?, ?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGroup)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
END_PARAM();
};

class CSPProtectedSearch : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwCharID;
	DWORD m_dwProtected;

DEF_QUERY(CSPProtectedSearch, _T("{ ? = Call TProtectedSearch(?, ?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwProtected)
END_PARAM();
};

class CSPLoadService : public CSqlQuery
{
public:
	BYTE	m_bWorld;
	BYTE	m_bServiceGroup;
	TCHAR	m_szIP[MAX_NAME+1];
	WORD	m_wPort;

DEF_QUERY(CSPLoadService, _T("{CALL TLoadService(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bWorld)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bServiceGroup)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szIP)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wPort)
END_PARAM()
};

class CSPGuildArticleAdd : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;
	BYTE	m_bDuty;
	TCHAR	m_szWritter[MAX_NAME+1];
	TCHAR	m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR	m_szArticle[MAX_BOARD_TEXT+1];
	DWORD	m_dwTime;

DEF_QUERY(CSPGuildArticleAdd, _T("{CALL TGuildArticleAdd(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDuty)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szWritter)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szArticle)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTime)
END_PARAM()
};

class CSPGuildArticleDel : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;

DEF_QUERY(CSPGuildArticleDel, _T("{CALL TGuildArticleDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
END_PARAM()
};

class CSPGuildArticleUpdate : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;
	TCHAR	m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR	m_szArticle[MAX_BOARD_TEXT+1];

DEF_QUERY(CSPGuildArticleUpdate, _T("{CALL TGuildArticleUpdate(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szArticle)
END_PARAM()
};

class CSPGuildFame : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwFame;
	DWORD	m_dwFameColor;

DEF_QUERY(CSPGuildFame, _T("{CALL TGuildFame(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFame)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFameColor)
END_PARAM()
};

class CSPGuildLevel : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	BYTE	m_bLevel;

DEF_QUERY(CSPGuildLevel, _T("{CALL TGuildLevel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bLevel)
END_PARAM()
};

class CSPGuildMaxCabinet : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	BYTE	m_bMaxCabinet;

DEF_QUERY(CSPGuildMaxCabinet, _T("{CALL TGuildMaxCabinet(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMaxCabinet)
END_PARAM()
};

class CSPGuildContribution : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwCharID;
	DWORD	m_dwExp;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;

DEF_QUERY(CSPGuildContribution, _T("{CALL TGuildContribution(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwExp)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCooper)
END_PARAM()
};

class CSPGuildVolunteerAdd : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwCharID;
	TCHAR	m_szTime[MAX_NAME+1];
	BYTE	m_bType;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;

DEF_QUERY(CSPGuildVolunteerAdd, _T("{CALL TGuildVolunteerAdd(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTime)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCooper)
END_PARAM()
};

class CSPGuildVolunteerDel : public CSqlQuery
{
public:
	DWORD	m_dwGuildID;
	DWORD	m_dwID;

DEF_QUERY(CSPGuildVolunteerDel, _T("{CALL TGuildVolunteerDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
END_PARAM()
};

class CSPSoulmateReg : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwTarget;

DEF_QUERY(CSPSoulmateReg, _T("{CALL TSoulmateReg(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTarget)
END_PARAM()
};

class CSPSoulmateEnd : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwTime;

DEF_QUERY(CSPSoulmateEnd, _T("{CALL TSoulmateEnd(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTime)
END_PARAM()
};

class CSPSoulmateDel : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwSoul;

DEF_QUERY(CSPSoulmateDel, _T("{CALL TSoulmateDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSoul)
END_PARAM()
};

class CSPGetReservedPost : public CSqlQuery  
{
public:
	int   m_nRET;
	DWORD m_dwPostID;
	DWORD m_dwSenderID;
	DWORD m_dwRecverID;
	TCHAR m_szSender[MAX_NAME + 1];
	TCHAR m_szRecver[MAX_NAME + 1];
	TCHAR m_szTitle[MAX_BOARD_TITLE + 1];
	TCHAR m_szMessage[MAX_BOARD_TEXT + 1];	
	TIMESTAMP_STRUCT m_dEndTime;
	TITEM m_item;

DEF_QUERY(CSPGetReservedPost, _T("{ ? = CALL TGetReservedPost(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(37)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwPostID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwSenderID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwRecverID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSender)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szRecver)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szMessage)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dlID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bItemID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wItemID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bLevel)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bCount)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bGLevel)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwDuraMax)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwDuraCur)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bRefineCur)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dEndTime)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bGradeEffect)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[0])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[1])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[2])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[3])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[4])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_bMagic[5])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[0])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[1])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[2])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[3])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[4])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_wValue[5])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[0])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[1])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[2])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[3])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[4])
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPClearMapCurrentUser : public CSqlQuery
{
public:
	int m_nRET;
	BYTE m_bGroupID;
	BYTE m_bServerID;
	BYTE m_bSvrType;

DEF_QUERY( CSPClearMapCurrentUser, _T( " { CALL TClearMapCurrentUser(?,?,?) }" ) )

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGroupID )
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bServerID )
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bSvrType)
END_PARAM()
};

class CSPGetNation : public CSqlQuery
{
public:
	BYTE m_bNation;

DEF_QUERY(CSPGetNation, _T("{CALL TGetNation(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bNation)
END_PARAM()
};

class CSPItemStateChange : public CSqlQuery
{
public:
	int m_nRET;
	WORD m_wItemID;
	BYTE m_bInitState;

DEF_QUERY(CSPItemStateChange, _T("{ ? = CALL TItemStateChange(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bInitState)
END_PARAM()
};

class CSPSaveGuildPvPoint : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwTotalPoint;
	DWORD m_dwUseablePoint;
	DWORD m_dwMonthPoint;

DEF_QUERY(CSPSaveGuildPvPoint, _T("{ CALL TSaveGuildPvPoint(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUseablePoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwMonthPoint)
END_PARAM()
};

class CSPSaveGuildPointReward : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwPoint;
	TCHAR m_szName[MAX_NAME];
	DWORD m_dwTotalPoint;
	DWORD m_dwUseablePoint;

DEF_QUERY(CSPSaveGuildPointReward, _T("{ CALL TSaveGuildPointReward(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPoint)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUseablePoint)
END_PARAM()
};

class CSPSaveGuildPvPRecord : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwMemberID;
	DWORD m_dwDate;
	WORD m_wKillCount;
	WORD m_wDieCount;
	DWORD m_aPoint[PVPE_COUNT];

DEF_QUERY(CSPSaveGuildPvPRecord, _T("{ CALL TSaveGuildPvPRecord(?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(13)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwMemberID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwDate)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wKillCount)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wDieCount)
	for(BYTE i=0; i<PVPE_COUNT; i++)
	{
		PARAM_ENTRY(SQL_PARAM_INPUT, m_aPoint[i])
	}
END_PARAM()
};

class CSPSaveCastleApplicant : public CSqlQuery
{
public:
	WORD m_wCastle;
	DWORD m_dwCharID;
	BYTE m_bCamp;

DEF_QUERY( CSPSaveCastleApplicant, _T(" { CALL TSaveCastleApplicant(?,?,?) }"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wCastle)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bCamp)
END_PARAM()
};

class CSPGetRecallID : public CSqlQuery
{
public:
	DWORD m_dwGenID;

DEF_QUERY(CSPGetRecallID, _T(" { CALL TGetRecallID(?) }"))
BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwGenID)
END_PARAM()
};
 
class CSPCashItemSale : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bValue;

DEF_QUERY(CSPCashItemSale,_T("{ CALL TCashItemSale(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bValue)
END_PARAM()
};

class CSPGuildWantedAdd : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR m_szText[MAX_BOARD_TEXT+1];

DEF_QUERY(CSPGuildWantedAdd,_T("{ CALL TGuildWantedAdd(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMinLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMaxLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeEnd)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szText)
END_PARAM()
};

class CSPGuildWantedDel : public CSqlQuery
{
public:
	DWORD m_dwGuildID;

DEF_QUERY(CSPGuildWantedDel, _T("{ CALL TGuildWantedDel(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CSPGuildVolunteering : public CSqlQuery
{
public:
	BYTE m_bType;
	DWORD m_dwCharID;
	DWORD m_dwID;

DEF_QUERY(CSPGuildVolunteering, _T("{ CALL TGuildVolunteering(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
END_PARAM()
};

class CSPGuildVolunteeringDel : public CSqlQuery
{
public:
	BYTE m_bType;
	DWORD m_dwCharID;

DEF_QUERY(CSPGuildVolunteeringDel, _T("{ CALL TGuildVolunteeringDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPGuildTacticsWantedAdd : public CSqlQuery
{
public:
	DWORD m_dwID;
	DWORD m_dwGuildID;
	DWORD m_dwPoint;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	BYTE m_bDay;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR m_szText[MAX_BOARD_TEXT+1];

DEF_QUERY(CSPGuildTacticsWantedAdd,_T("{ CALL TGuildTacticsWantedAdd(?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(12)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCooper)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDay)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMinLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMaxLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeEnd)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szText)
END_PARAM()
};

class CSPGuildTacticsWantedDel : public CSqlQuery
{
public:
	DWORD m_dwID;

DEF_QUERY(CSPGuildTacticsWantedDel, _T("{ CALL TGuildTacticsWantedDel(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwID)
END_PARAM()
};

class CSPGuildTacticsAdd : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwCharID;
	DWORD m_dwPoint;
	INT64 m_dlMoney;
	BYTE m_bDay;
	TIMESTAMP_STRUCT m_timeEnd;
	DWORD m_dwCharGuildID;

DEF_QUERY(CSPGuildTacticsAdd, _T("{ CALL TGuildTacticsAdd(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dlMoney)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDay)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeEnd)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCharGuildID)
END_PARAM()
};

class CSPGuildTacticsDel : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwCharGuildID;

DEF_QUERY(CSPGuildTacticsDel, _T("{ CALL TGuildTacticsDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCharGuildID)
END_PARAM()
};

class CSPSaveTacticsGainPoint : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwGainPoint;

DEF_QUERY(CSPSaveTacticsGainPoint, _T("{ CALL TSaveTacticsGainPoint(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGainPoint)
END_PARAM()
};
 
class CSPSaveMonthRank : public CSqlQuery
{
public:
	BYTE	m_bMonth;
	BYTE	m_bRank;
	BYTE	m_bMonthRank;
	DWORD	m_dwChaID;
	TCHAR	m_szName[MAX_NAME +1];
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
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];
	TCHAR	m_szGuild[MAX_NAME +1];

DEF_QUERY(CSPSaveMonthRank,_T("{ CALL TSaveMonthRank(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(20)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonth)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bRank)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonthRank)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwChaID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwMonthPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wMonthWin)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wMonthLose)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwTotalWin)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwTotalLose)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCountry)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bClass)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bRace)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bSex)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bHair)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bFace)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szSay)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szGuild)
END_PARAM()
};

class CSPInitMonthPvPoint : public CSqlQuery
{
public:
	INT		m_nRET;
	BYTE	m_bMonth;
	DWORD	m_dwMTotalPoint;
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME +1];
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
	TCHAR	m_szGuild[MAX_NAME+1];

DEF_QUERY(CSPInitMonthPvPoint, _T("{? = CALL TInitMonthPvPoint(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }"))

BEGIN_PARAM(19)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonth)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwMTotalPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwMonthPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wMonthWin)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wMonthLose)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwTotalWin)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwTotalLose)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bCountry)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bLevel)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bClass)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bRace)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bSex)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bHair)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bFace)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szGuild)
END_PARAM()
};

class CSPInitMonthRank	: public CSqlQuery
{
public:
	BYTE	m_bMonth;

DEF_QUERY(CSPInitMonthRank, _T("{CALL TInitMonthRank(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonth)
END_PARAM()
};
 
class CSPEventQuarterUpdate : public CSqlQuery
{
public:
	int		m_nRET;
	BYTE	m_bType;
	BYTE	m_bDay;
	BYTE	m_bHour;
	BYTE	m_bMinute;
	WORD	m_wID;
	WORD	m_wOutID;
	WORD	m_wItemID1;
	WORD	m_wItemID2;
	WORD	m_wItemID3;
	WORD	m_wItemID4;
	WORD	m_wItemID5;
	BYTE	m_bCount;
	TCHAR	m_szPresent[MAX_NAME +1];
	TCHAR	m_szAnnounce[MAX_STR_GUILD +1];
	TCHAR	m_szTitle[MAX_NAME+1];
	TCHAR	m_szMessage[501];
	TCHAR	m_szName1[MAX_NAME +1];
	TCHAR	m_szName2[MAX_NAME +1];
	TCHAR	m_szName3[MAX_NAME +1];
	TCHAR	m_szName4[MAX_NAME +1];
	TCHAR	m_szName5[MAX_NAME +1];

DEF_QUERY(CSPEventQuarterUpdate,_T("{? = CALL TEventQuarterUpdate(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(22)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDay)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bHour)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMinute)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID5)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bCount)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szPresent)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szAnnounce)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szMessage)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wOutID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName1)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName2)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName3)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName4)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName5)
END_PARAM()
};

class CSPGetItemName : public CSqlQuery
{
public:
	WORD	m_wItemID1;
	WORD	m_wItemID2;
	WORD	m_wItemID3;
	WORD	m_wItemID4;
	WORD	m_wItemID5;
	TCHAR	m_szName1[MAX_NAME +1];
	TCHAR	m_szName2[MAX_NAME +1];
	TCHAR	m_szName3[MAX_NAME +1];
	TCHAR	m_szName4[MAX_NAME +1];
	TCHAR	m_szName5[MAX_NAME +1];

DEF_QUERY(CSPGetItemName,_T("{CALL TGetItemName(?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(10)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID2)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID3)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID4)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID5)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName1)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName2)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName3)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName4)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szName5)
END_PARAM()
};

class CSPTournamentPayback : public CSqlQuery
{
public:
	DWORD m_dwPostID;
	DWORD m_dwCharID;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;

DEF_QUERY(CSPTournamentPayback, _T("{CALL TTournamentPayback(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwPostID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCooper)
END_PARAM()
};

class CSPTournamentResult : public CSqlQuery
{
public:
	BYTE m_bStep;
	BYTE m_bRet;
	DWORD m_dwWin;
	DWORD m_dwLose;

DEF_QUERY(CSPTournamentResult, _T("{CALL TTournamentResult(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bStep)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bRet)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwWin)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwLose)
END_PARAM()
};

class CSPTournamentApply : public CSqlQuery
{
public:
	BYTE m_bAdd;
	DWORD m_dwCharID;
	BYTE m_bEntry;
	DWORD m_dwChiefID;

DEF_QUERY(CSPTournamentApply, _T("{CALL TTournamentApply(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bAdd)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bEntry)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwChiefID)
END_PARAM()
};

class CSPTournamentClear : public CSqlQuery
{
public:
	BYTE m_bClear;

DEF_QUERY(CSPTournamentClear, _T("{CALL TTournamentClear(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bClear)
END_PARAM()
};

class CSPTournamentStatus : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bGroup;
	BYTE m_bStep;

DEF_QUERY(CSPTournamentStatus, _T("{CALL TTournamentStatus(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bGroup)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bStep)
END_PARAM()
};

class CSPTnmtEventTime : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bWeek;
	BYTE m_bDay;
	DWORD m_dwStart;

DEF_QUERY(CSPTnmtEventTime, _T("{CALL TTnmtEventTime(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTourID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bWeek)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bDay)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwStart)
END_PARAM()
};

class CSPTnmtEventSchedule : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bStep;
	DWORD m_dwPeriod;

DEF_QUERY(CSPTnmtEventSchedule, _T("{CALL TTnmtEventSchedule(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTourID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bStep)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwPeriod)
END_PARAM()
};

class CSPTnmtEventDel : public CSqlQuery
{
public:
	WORD m_wTourID;

DEF_QUERY(CSPTnmtEventDel, _T("{CALL TTnmtEventDel(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTourID)
END_PARAM()
};

class CSPTnmtEventEntry : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bEntryID;
	TCHAR m_szName[MAX_NAME];
	BYTE m_bType;
	DWORD m_dwClass;
	DWORD m_dwFee;
	DWORD m_dwFeeBack;
	WORD m_wPermitItemID;
	BYTE m_bPermitCount;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;

DEF_QUERY(CSPTnmtEventEntry, _T("{CALL TTnmtEventEntry(?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(11)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTourID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bEntryID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwClass)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwFee)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwFeeBack)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wPermitItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bPermitCount)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMinLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMaxLevel)
END_PARAM()
};

class CSPTnmtEventReward : public CSqlQuery
{
public:
	WORD m_wTourID;
	BYTE m_bEntryID;
	BYTE m_bChartType;
	WORD m_wItemID;
	BYTE m_bCount;
	DWORD m_dwClass;
	BYTE m_bCheckShield;

DEF_QUERY(CSPTnmtEventReward, _T("{CALL TTnmtEventReward(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTourID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bEntryID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bChartType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCount)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwClass)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCheckShield)
END_PARAM()
};

class CSPHelpMessage : public CSqlQuery
{
public:
	BYTE m_bID;
	TIMESTAMP_STRUCT m_timeStart;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR m_szMessage[MAX_BOARD_TEXT + 1];

DEF_QUERY(CSPHelpMessage, _T("{CALL THelpMessage(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_timeStart)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_timeEnd)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szMessage)
END_PARAM()
};

class CSPRPSGameRecord : public CSqlQuery
{
public:
	BYTE m_bRecord;
	DWORD m_dwCharID;
	BYTE m_bType;
	BYTE m_bWinCount;
	TIMESTAMP_STRUCT m_dWinDate;

DEF_QUERY(CSPRPSGameRecord, _T("{CALL TRPSGameRecord(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bRecord)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bWinCount)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dWinDate)
END_PARAM()
};

class CSPGetLimitedLevel : public CSqlQuery
{
public:
	BYTE m_bMaxLevel;

DEF_QUERY(CSPGetLimitedLevel, _T("{ CALL TGetLimitedLevel(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bMaxLevel)
END_PARAM()
};

class CSPCMGiftCanTake : public CSqlQuery
{
public:
	BYTE m_bRET;
	TCHAR m_szName[MAX_NAME+1];
	WORD m_wGiftID;
    
DEF_QUERY(CSPCMGiftCanTake, _T("{ ? = CALL TCMGiftCanTake(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bRET)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wGiftID)
END_PARAM()
};

class CSPCMGiftSet : public CSqlQuery
{
public:
	WORD m_wGiftID;
	BYTE m_bGiftType;
	DWORD m_dwValue;
	BYTE m_bCount;
	BYTE m_bTakeType;
	BYTE m_bMaxTakeCount;
	BYTE m_bToolOnly;
	WORD m_wErrGiftID;
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR m_szMsg[MAX_BOARD_MSG+1];

DEF_QUERY(CSPCMGiftSet, _T("{ CALL TCMGiftSet(?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(10)	
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wGiftID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGiftType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwValue)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bCount)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bTakeType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMaxTakeCount)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bToolOnly)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wErrGiftID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szMsg)
END_PARAM()

};

class CSPCMGiftAdd : public CSqlQuery
{
public:
	WORD m_wGiftID;
	BYTE m_bGiftType;
	DWORD m_dwValue;
	BYTE m_bCount;
	BYTE m_bTakeType;
	BYTE m_bMaxTakeCount;
	BYTE m_bToolOnly;
	WORD m_wErrGiftID;
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];
	TCHAR m_szMsg[MAX_BOARD_MSG+1];

DEF_QUERY(CSPCMGiftAdd, _T("{ CALL TCMGiftAdd(?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(10)	
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wGiftID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bGiftType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwValue)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bCount)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bTakeType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMaxTakeCount)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bToolOnly)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wErrGiftID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szMsg)
END_PARAM()

};

class CSPCMGiftDel : public CSqlQuery
{
public:
	WORD m_wGiftID;

DEF_QUERY(CSPCMGiftDel, _T("{ CALL TCMGiftDel(?)}"))

BEGIN_PARAM(1)	
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wGiftID)
END_PARAM()

};

/////////////////////////////////////////////////////////
EXPORT_INIT_QUERY(TWorldSvr)
