#pragma once

class CTBLChannelList : public CSqlQuery
{
public:
	BYTE m_bChannel;

DEF_QUERY( CTBLChannelList, _T("SELECT \
	bChannel \
	FROM TCHANNEL"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_bChannel)
END_COL()
};

class CTBLChannelChart : public CSqlQuery
{
public:
	WORD m_wMapID;

	BYTE m_bLogChannel;
	BYTE m_bPhyChannel;
	BYTE m_bServerID;

DEF_QUERY( CTBLChannelChart, _T("SELECT \
	TCHANNELCHART.wMapID, \
	TCHANNELCHART.bLogChannel, \
	TCHANNELCHART.bPhyChannel \
	FROM TCHANNELCHART \
	INNER JOIN TSVRCHART ON \
	TCHANNELCHART.wMapID = TSVRCHART.wMapID AND \
	TCHANNELCHART.bGroupID = TSVRCHART.bGroup AND \
	TCHANNELCHART.wUnitID = TSVRCHART.wUnitID AND \
	TCHANNELCHART.bPhyChannel = TSVRCHART.bChannel \
	WHERE TSVRCHART.bServerID = ? \
	GROUP BY \
	TCHANNELCHART.wMapID, \
	TCHANNELCHART.bLogChannel, \
	TCHANNELCHART.bPhyChannel, \
	TSVRCHART.bServerID"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_bLogChannel)
	COLUMN_ENTRY(m_bPhyChannel)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bServerID)
END_PARAM()
};

class CTBLServerChart : public CSqlQuery
{
public:
	BYTE m_bServerID;
	BYTE m_bChannel;

	WORD m_wUnitID;
	WORD m_wMapID;

DEF_QUERY( CTBLServerChart, _T("SELECT \
	bChannel, \
	wMapID, \
	wUnitID \
	FROM TSVRCHART \
	WHERE bServerID = ?"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_bChannel)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wUnitID)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bServerID)
END_PARAM()
};

class CTBLFormulaChart : public CSqlQuery
{
public:
	BYTE	m_bID;
	TCHAR	m_szName[MAX_NAME+1];
	DWORD	m_dwInit;
	FLOAT	m_fRateX;
	FLOAT	m_fRateY;

DEF_QUERY( CTBLFormulaChart, _T("SELECT \
	bID, \
	szName, \
	dwInit, \
	fRateX, \
	fRateY \
	FROM TFORMULACHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_bID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwInit)
	COLUMN_ENTRY(m_fRateX)
	COLUMN_ENTRY(m_fRateY)
END_COL()
};

class CTBLChar : public CSqlQuery
{
public:
	TCHAR m_szNAME[MAX_NAME + 1];
	DWORD m_dwCharID;
	BYTE m_bStartAct;
	BYTE m_bRealSex;

	BYTE m_bClass;
	BYTE m_bLevel;
	BYTE m_bRace;
	BYTE m_bCountry;
	BYTE m_bOriCountry;
	BYTE m_bSex;

	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;

	BYTE m_bHelmetHide;

	DWORD m_dwCooper;
	DWORD m_dwSilver;
	DWORD m_dwGold;

	DWORD m_dwEXP;
	DWORD m_dwHP;
	DWORD m_dwMP;
	WORD m_wSkillPoint;
	DWORD m_dwRegion;

	BYTE m_bGuildLeave;
	DWORD m_dwGuildLeaveTime;

	WORD m_wTemptedMon;
	BYTE m_bAftermath;

	WORD m_wSpawnID;
	WORD m_wLastSpawnID;
	DWORD m_dwLastDestination;
	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	WORD m_wDIR;

DEF_QUERY( CTBLChar, _T("SELECT \
	szNAME, \
	bStartAct, \
	bRealSex, \
	bClass, \
	bLevel, \
	bRace, \
	bCountry, \
	bOriCountry, \
	bSex, \
	bHair, \
	bFace, \
	bBody, \
	bPants, \
	bHand, \
	bFoot, \
	bHelmetHide, \
	dwCooper, \
	dwSilver, \
	dwGold, \
	dwEXP, \
	dwHP, \
	dwMP, \
	wSkillPoint, \
	dwRegion, \
	bGuildLeave, \
	dwGuildLeaveTime, \
	wMapID, \
	wSpawnID, \
	wLastSpawnID, \
	dwLastDestination, \
	wTemptedMon, \
	bAftermath, \
	fPosX, \
	fPosY, \
	fPosZ, \
	wDIR \
	FROM TCHARTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(35)
	COLUMN_ENTRY_STR(m_szNAME)
	COLUMN_ENTRY(m_bStartAct)
	COLUMN_ENTRY(m_bRealSex)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bOriCountry)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY(m_bBody)
	COLUMN_ENTRY(m_bPants)
	COLUMN_ENTRY(m_bHand)
	COLUMN_ENTRY(m_bFoot)
	COLUMN_ENTRY(m_bHelmetHide)
	COLUMN_ENTRY(m_dwCooper)
	COLUMN_ENTRY(m_dwSilver)
	COLUMN_ENTRY(m_dwGold)
	COLUMN_ENTRY(m_dwEXP)
	COLUMN_ENTRY(m_dwHP)
	COLUMN_ENTRY(m_dwMP)
	COLUMN_ENTRY(m_wSkillPoint)
	COLUMN_ENTRY(m_dwRegion)
	COLUMN_ENTRY(m_bGuildLeave)
	COLUMN_ENTRY(m_dwGuildLeaveTime)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wSpawnID)
	COLUMN_ENTRY(m_wLastSpawnID)
	COLUMN_ENTRY(m_dwLastDestination)
	COLUMN_ENTRY(m_wTemptedMon)
	COLUMN_ENTRY(m_bAftermath)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
	COLUMN_ENTRY(m_wDIR)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLSoulmateTable : public CSqlQuery
{
public:
	DWORD	m_dwCharID;

	DWORD	m_dwSoulmate;
	TCHAR	m_szName[MAX_NAME+1];
	DWORD	m_dwTime;

DEF_QUERY(CTBLSoulmateTable, _T("SELECT dwTarget, szNAME, dwTime FROM TVIEW_SOULMATE WHERE dwCharID = ?"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwSoulmate)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLProtectedTable : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwProtected;
	TCHAR	m_szName[MAX_NAME+1];
	BYTE	m_bOption;

DEF_QUERY(CTBLProtectedTable, _T("SELECT dwProtected, szName, bOption FROM TPROTECTEDTABLE WHERE dwCharID = ?"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwProtected)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bOption)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLCharGuild : public CSqlQuery
{
public:
	DWORD m_dwCharID;

	DWORD m_dwGuildID;
	BYTE m_bDuty;
	BYTE m_bPeer;

DEF_QUERY( CTBLCharGuild, _T("SELECT \
	dwGuildID, \
	bDuty, \
	bPeer \
	FROM TGUILDMEMBERTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwGuildID)
	COLUMN_ENTRY(m_bDuty)
	COLUMN_ENTRY(m_bPeer)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLCharTactics : public CSqlQuery
{
public:
	DWORD m_dwCharID;

	DWORD m_dwGuildID;

DEF_QUERY( CTBLCharTactics, _T("SELECT \
	dwGuildID \
	FROM TGUILDTACTICSTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwGuildID)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLCharGuildInfo : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwFame;
	DWORD m_dwFameColor;

DEF_QUERY( CTBLCharGuildInfo, _T("SELECT \
	dwFame, \
	dwFameColor \
	FROM TGUILDTABLE \
	WHERE dwID = ?"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwFame)
	COLUMN_ENTRY(m_dwFameColor)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildID)
END_PARAM()
};

class CTBLAIChart : public CSqlQuery
{
public:
	DWORD m_dwTriggerID;
	DWORD m_dwCmdID;
	DWORD m_dwDelay;

	BYTE m_bTriggerType;
	BYTE m_bAIType;
	BYTE m_bLoop;

DEF_QUERY( CTBLAIChart, _T("SELECT \
	dwTriggerID, \
	dwCmdID, \
	dwDelay, \
	bTriggerType, \
	bAIType, \
	bLoop \
	FROM TAICHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwTriggerID)
	COLUMN_ENTRY(m_dwCmdID)
	COLUMN_ENTRY(m_dwDelay)
	COLUMN_ENTRY(m_bTriggerType)
	COLUMN_ENTRY(m_bAIType)
	COLUMN_ENTRY(m_bLoop)
END_COL()
};


class CTBLAICommand : public CSqlQuery
{
public:
	DWORD m_dwID;
	BYTE m_bType;

DEF_QUERY( CTBLAICommand, _T("SELECT \
	dwCmdID, \
	bCmdType \
	FROM TAICMDCHART"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_bType)
END_COL()
};


class CTBLAICondition : public CSqlQuery
{
public:
	DWORD m_dwCmdID;
	DWORD m_dwID;
	BYTE m_bType;

DEF_QUERY( CTBLAICondition, _T("SELECT \
	dwConditionID, \
	bConditionType \
	FROM TAICONCHART \
	WHERE dwCmdID = ?"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_bType)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCmdID)
END_PARAM()
};


class CTBLMonster : public CSqlQuery
{
public:
	WORD m_wID;

	BYTE m_bClass;
	WORD m_wKind;
	BYTE m_bLevel;
	BYTE m_bRace;

	BYTE m_bRoamProb;
	BYTE m_bAIType;
	BYTE m_bRange;
	BYTE m_bIsSelf;
	WORD m_wChaseRange;

	BYTE m_bMoneyProb;
	DWORD m_dwMinMoney;
	DWORD m_dwMaxMoney;

	BYTE m_bItemProb;
	BYTE m_bDropCount;

	WORD m_wExp;
	BYTE m_bRecallType;
	BYTE m_bCanSelect;
	BYTE m_bCanAttack;
	BYTE m_bTame;
	BYTE m_bCall;
	BYTE m_bRemove;
	WORD m_wTransSkillID;
	FLOAT m_fSize;
	BYTE m_bIsSpecial;
	WORD m_wMonAttr;
	WORD m_wSummonAttr;
	WORD m_wSkill[4];

DEF_QUERY( CTBLMonster, _T("SELECT \
	wID, \
	bClass, \
	wKind, \
	bLevel, \
	bRace, \
	bRoamProb, \
	bAIType, \
	bRange, \
	wChaseRange, \
	bMoneyProb, \
	dwMinMoney, \
	dwMaxMoney, \
	bItemProb, \
	bDropCount, \
	wExp, \
	bRecallType, \
	bCanSelect, \
	bIsSelf, \
	bCanAttack, \
	bTame, \
	bCall, \
	bRemove, \
	wTransSkillID, \
	fSize, \
	bIsSpecial, \
	wMonAttr, \
	wSummonAttr, \
	wSkill1, \
	wSkill2, \
	wSkill3, \
	wSkill4 \
	FROM TMONSTERCHART"))

BEGIN_COLUMN(31)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_wKind)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bRoamProb)
	COLUMN_ENTRY(m_bAIType)
	COLUMN_ENTRY(m_bRange)
	COLUMN_ENTRY(m_wChaseRange)
	COLUMN_ENTRY(m_bMoneyProb)
	COLUMN_ENTRY(m_dwMinMoney)
	COLUMN_ENTRY(m_dwMaxMoney)
	COLUMN_ENTRY(m_bItemProb)
	COLUMN_ENTRY(m_bDropCount)
	COLUMN_ENTRY(m_wExp)
	COLUMN_ENTRY(m_bRecallType)
	COLUMN_ENTRY(m_bCanSelect)
	COLUMN_ENTRY(m_bIsSelf)
	COLUMN_ENTRY(m_bCanAttack)
	COLUMN_ENTRY(m_bTame)
	COLUMN_ENTRY(m_bCall)
	COLUMN_ENTRY(m_bRemove)
	COLUMN_ENTRY(m_wTransSkillID)
	COLUMN_ENTRY(m_fSize);
	COLUMN_ENTRY(m_bIsSpecial);
	COLUMN_ENTRY(m_wMonAttr);
	COLUMN_ENTRY(m_wSummonAttr);
	for(int i=0; i<4; i++)
	{
		COLUMN_ENTRY(m_wSkill[i]);
	}
END_COL()
};

class CTBLPet : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bPetType;
	BYTE m_bRace;
	WORD m_wMonID;
	BYTE m_bRecallKind1;
	BYTE m_bRecallKind2;
	WORD m_wRecallValue1;
	WORD m_wRecallValue2;
	BYTE m_bConditionType;
	DWORD m_dwConditionValue;

DEF_QUERY( CTBLPet, _T("SELECT \
		  wID, \
		  bPetType, \
		  bRace, \
		  wMonID, \
		  bRecallKind1, \
		  bRecallKind2, \
		  wRecallValue1, \
		  wRecallValue2, \
		  bConditionType, \
		  dwConditionValue \
		  FROM TPETCHART"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bPetType)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_wMonID)
	COLUMN_ENTRY(m_bRecallKind1)
	COLUMN_ENTRY(m_bRecallKind2)
	COLUMN_ENTRY(m_wRecallValue1)
	COLUMN_ENTRY(m_wRecallValue2)
	COLUMN_ENTRY(m_bConditionType)
	COLUMN_ENTRY(m_dwConditionValue)
END_COL()
};

class CTBLMonParty : public CSqlQuery
{
public:
	WORD m_wSpawnID;
	WORD m_wPartyID;

DEF_QUERY( CTBLMonParty, _T("SELECT wID FROM TMONSPAWNCHART WHERE wPartyID = ?"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_wSpawnID)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPartyID)
END_PARAM()
};

class CTBLMonSpawn : public CSqlQuery
{
public:
	WORD m_wID;
	WORD m_wGroup;
	WORD m_wLocalID;

	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	WORD m_wDir;

	BYTE m_bServerID;
	BYTE m_bCount;
	BYTE m_bRange;
	BYTE m_bArea;
	BYTE m_bLink;
	BYTE m_bProb;
	BYTE m_bRoamType;
	BYTE m_bCountry;
	WORD m_wPartyID;

	DWORD m_dwRegion;
	DWORD m_dwDelay;
	BYTE m_bEvent;

DEF_QUERY( CTBLMonSpawn, _T("SELECT \
	wID, \
	wGroup, \
	wMapID, \
	wLocalID, \
	fPosX, \
	fPosY, \
	fPosZ, \
	wDir, \
	bCountry, \
	bCount, \
	bRange, \
	bArea, \
	bLink, \
	bProb, \
	bRoamType, \
	dwRegion, \
	dwDelay, \
	bEvent, \
	wPartyID \
	FROM TMONSPAWNCHART \
	WHERE wMapID IN (\
	SELECT wMapID \
	FROM TSVRCHART \
	WHERE bServerID = ?) AND \
	CAST(fPosZ / 1024 AS SMALLINT) * 256 + CAST(fPosX / 1024 AS SMALLINT) IN (\
	SELECT wUnitID \
	FROM TSVRCHART \
	WHERE bServerID = ?)"))

BEGIN_COLUMN(19)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wGroup)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wLocalID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
	COLUMN_ENTRY(m_wDir)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bCount)
	COLUMN_ENTRY(m_bRange)
	COLUMN_ENTRY(m_bArea)
	COLUMN_ENTRY(m_bLink)
	COLUMN_ENTRY(m_bProb)
	COLUMN_ENTRY(m_bRoamType)
	COLUMN_ENTRY(m_dwRegion)
	COLUMN_ENTRY(m_dwDelay)
	COLUMN_ENTRY(m_bEvent)
	COLUMN_ENTRY(m_wPartyID)
END_COL()

BEGIN_PARAM(2)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bServerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bServerID)
END_PARAM()
};

class CTBLCastleTable : public CSqlQuery
{
public:
	WORD m_wCastle;
	BYTE m_bCountry;
	DWORD m_dwGuild;
	TCHAR m_szGuildName[MAX_NAME + 1];
	TIMESTAMP_STRUCT m_dateWarTime;
	TCHAR m_szHero[MAX_NAME + 1];             
	TIMESTAMP_STRUCT m_dateHero;

DEF_QUERY(CTBLCastleTable, _T("SELECT \
	TCASTLETABLE.wCastle, \
	TCASTLETABLE.bCountry, \
	TCASTLETABLE.dwGuildID, \
	TGUILDTABLE.szName, \
	TCASTLETABLE.dateWarTime, \
	TCASTLETABLE.szHero, \
	TCASTLETABLE.dateHero \
	FROM TCASTLETABLE LEFT OUTER JOIN TGUILDTABLE \
	ON TCASTLETABLE.dwGuildID = TGUILDTABLE.dwID"))

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_wCastle)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_dwGuild)
	COLUMN_ENTRY_STR(m_szGuildName)
	COLUMN_ENTRY(m_dateWarTime)
	COLUMN_ENTRY_STR(m_szHero)
	COLUMN_ENTRY(m_dateHero)
END_COL()
};

class CTBLBattleZoneChart : public CSqlQuery
{
public:
	WORD m_wID;
	TCHAR m_szName[MAX_NAME + 1];
	WORD m_wMapID;
	WORD m_wCastle;
	WORD m_wBossSpawnID;
	WORD m_wLGateKeeperSpawnID;
	WORD m_wRGateKeeperSpawnID;
	WORD m_wCGateKeeperSpawnID;
	DWORD m_dwLSwitchID;
	DWORD m_dwRSwitchID;
	DWORD m_dwCSwitchID;
	WORD m_wNormalItem;
	WORD m_wChiefItem;
	BYTE m_bLine;
	WORD m_wSkill1;
	WORD m_wSkill2;
	BYTE m_bItemLevel;

DEF_QUERY(CTBLBattleZoneChart, _T("SELECT \
	wID, \
	szName, \
	wMapID, \
	wCastle, \
	wBossSpawnID, \
	wLGateKeeperSpawnID, \
	wRGateKeeperSpawnID, \
	wCGateKeeperSpawnID, \
	dwLSwitchID, \
	dwRSwitchID, \
	dwCSwitchID, \
	wNormalItem, \
	wChiefItem, \
	bLine, \
	wSkill1, \
	wSkill2, \
	bItemLevel \
	FROM TBATTLEZONECHART"))

BEGIN_COLUMN(17)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wCastle)
	COLUMN_ENTRY(m_wBossSpawnID)
	COLUMN_ENTRY(m_wLGateKeeperSpawnID)
	COLUMN_ENTRY(m_wRGateKeeperSpawnID)
	COLUMN_ENTRY(m_wCGateKeeperSpawnID)
	COLUMN_ENTRY(m_dwLSwitchID)
	COLUMN_ENTRY(m_dwRSwitchID)
	COLUMN_ENTRY(m_dwCSwitchID)
	COLUMN_ENTRY(m_wNormalItem)
	COLUMN_ENTRY(m_wChiefItem)
	COLUMN_ENTRY(m_bLine)
	COLUMN_ENTRY(m_wSkill1)
	COLUMN_ENTRY(m_wSkill2)
	COLUMN_ENTRY(m_bItemLevel)
END_COL()
};

class CTBLLocalTable : public CSqlQuery
{
public:
	WORD m_wLocal;
	BYTE m_bCountry;
	DWORD m_dwGuild;
	TCHAR m_szGuildName[MAX_NAME + 1];
	TIMESTAMP_STRUCT m_dateOccupy;
	TIMESTAMP_STRUCT m_dateDefend;
	TCHAR m_szHero[MAX_NAME + 1];  
	TIMESTAMP_STRUCT m_dateHero; 

DEF_QUERY(CTBLLocalTable, _T("SELECT \
	TLOCALTABLE.wLocalID, \
	TLOCALTABLE.bCountry, \
	TLOCALTABLE.dwGuild, \
	TGUILDTABLE.szName, \
	TLOCALTABLE.dateOccupy, \
	TLOCALTABLE.dateDefend, \
	TLOCALTABLE.szHero, \
	TLOCALTABLE.dateHero \
	FROM TLOCALTABLE LEFT OUTER JOIN TGUILDTABLE \
	ON TLOCALTABLE.dwGuild = TGUILDTABLE.dwID"))

BEGIN_COLUMN(8)
	COLUMN_ENTRY(m_wLocal)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_dwGuild)
	COLUMN_ENTRY_STR(m_szGuildName)		//IsNull Check...nCol=3
	COLUMN_ENTRY(m_dateOccupy)
	COLUMN_ENTRY(m_dateDefend)
	COLUMN_ENTRY_STR(m_szHero)
	COLUMN_ENTRY(m_dateHero)
END_COL()
};

class CTBLMapMon : public CSqlQuery
{
public:
	WORD m_wSpawnID;
	WORD m_wMonID;
	BYTE m_bLeader;

	BYTE m_bEssential;
	BYTE m_bProb;

DEF_QUERY( CTBLMapMon, _T("SELECT \
	wMonID, \
	bLeader, \
	bEssential, \
	bProb \
	FROM TMAPMONCHART \
	WHERE wSpawnID = ?"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wMonID)
	COLUMN_ENTRY(m_bLeader)
	COLUMN_ENTRY(m_bEssential)
	COLUMN_ENTRY(m_bProb)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSpawnID)
END_PARAM()
};

class CTBLSpawnPath : public CSqlQuery
{
public:
	WORD m_wSpawnID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	BYTE m_bProb;
	FLOAT m_fRadius;

DEF_QUERY(CTBLSpawnPath, _T("SELECT wSpawnID, fPosX, fPosY, fPosZ, bProb, fRadius FROM TSPAWNPATHCHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_wSpawnID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
	COLUMN_ENTRY(m_bProb)
	COLUMN_ENTRY(m_fRadius)
END_COL()
};
class CTBLClassChart : public CSqlQuery
{
public:
	BYTE m_bClassID;

	WORD m_wSTR;
	WORD m_wDEX;
	WORD m_wCON;

	WORD m_wINT;
	WORD m_wWIS;
	WORD m_wMEN;

DEF_QUERY( CTBLClassChart, _T("SELECT \
	bClassID, \
	wSTR, \
	wDEX, \
	wCON, \
	wINT, \
	wWIS, \
	wMEN \
	FROM TCLASSCHART"))

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_bClassID)
	COLUMN_ENTRY(m_wSTR)
	COLUMN_ENTRY(m_wDEX)
	COLUMN_ENTRY(m_wCON)
	COLUMN_ENTRY(m_wINT)
	COLUMN_ENTRY(m_wWIS)
	COLUMN_ENTRY(m_wMEN)
END_COL()
};


class CTBLRaceChart : public CSqlQuery
{
public:
	BYTE m_bRaceID;

	WORD m_wSTR;
	WORD m_wDEX;
	WORD m_wCON;

	WORD m_wINT;
	WORD m_wWIS;
	WORD m_wMEN;

DEF_QUERY( CTBLRaceChart, _T("SELECT \
	bRaceID, \
	wSTR, \
	wDEX, \
	wCON, \
	wINT, \
	wWIS, \
	wMEN \
	FROM TRACECHART"))

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_bRaceID)
	COLUMN_ENTRY(m_wSTR)
	COLUMN_ENTRY(m_wDEX)
	COLUMN_ENTRY(m_wCON)
	COLUMN_ENTRY(m_wINT)
	COLUMN_ENTRY(m_wWIS)
	COLUMN_ENTRY(m_wMEN)
END_COL()
};


class CTBLLevelChart : public CSqlQuery
{
public:
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

DEF_QUERY( CTBLLevelChart, _T("SELECT \
	bLevel, \
	dwEXP, \
	dwHP, \
	dwMP, \
	bSkillPoint, \
	dwMoney, \
	dwRegCost, \
	dwSearchCost, \
	dwGambleCost, \
	dwRepCost, \
	dwRepairCost, \
	dwRefineCost, \
	wPvPoint, \
	dwPvPMoney \
	FROM TLEVELCHART"))

BEGIN_COLUMN(14)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwEXP)
	COLUMN_ENTRY(m_dwHP)
	COLUMN_ENTRY(m_dwMP)
	COLUMN_ENTRY(m_bSkillPoint)
	COLUMN_ENTRY(m_dwMoney)
	COLUMN_ENTRY(m_dwRegCost)
	COLUMN_ENTRY(m_dwSearchCost)
	COLUMN_ENTRY(m_dwGambleCost)
	COLUMN_ENTRY(m_dwRepCost)
	COLUMN_ENTRY(m_dwRepairCost)
	COLUMN_ENTRY(m_dwRefineCost)
	COLUMN_ENTRY(m_wPvPoint)
	COLUMN_ENTRY(m_dwPvPMoney)
END_COL()
};


class CTBLItemChart : public CSqlQuery
{
public:
	WORD m_wItemID;
	BYTE m_bType;
	BYTE m_bKind;
	WORD m_wAttrID;
	WORD m_wUseValue;

	DWORD m_dwClassID;
	DWORD m_dwSlotID;
	FLOAT m_fPrice;
	FLOAT m_fPvPrice;

	BYTE m_bPrmSlotID;
	BYTE m_bSubSlotID;
	BYTE m_bSlotCount;
	BYTE m_bMinRange;
	BYTE m_bMaxRange;
	BYTE m_bDefaultLevel;
	BYTE m_bStack;
	BYTE m_bEquipSkill;
	BYTE m_bIsSell;
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
	BYTE m_bItemCountry;
	BYTE m_bIsSpecial;
	BYTE m_bCanGamble;
	WORD m_wItemProb_G;
	BYTE m_bGambleProb;
	BYTE m_bDestroyProb;
	DWORD m_dwDelay;
	DWORD m_dwDuraMax;
	BYTE m_bRefineMax;
	BYTE m_bCanRepair;
	WORD m_wDelayGroupID;
	WORD m_wWeight;
	BYTE m_bGroupID;
	BYTE m_bInitState;
	BYTE m_bCanWrap;
	DWORD m_dwCode;
	BYTE m_bCanColor;
	FLOAT m_fRevision[4];
	BYTE m_bConsumable;

DEF_QUERY( CTBLItemChart, _T("SELECT \
	wItemID, \
	bType, \
	bKind, \
	wAttrID, \
	wUseValue, \
	dwClassID, \
	dwSlotID, \
	bPrmSlotID, \
	bSubSlotID, \
	bSlotCount, \
	fPrice, \
	fPvPrice, \
	bIsSell, \
	bMinRange, \
	bMaxRange, \
	bLevel, \
	bStack, \
	bEquipSkill, \
	bUseItemKind, \
	bUseItemCount, \
	bGrade, \
	wUseTime, \
	bUseType, \
	bCanGrade, \
	bCanMagic, \
	bCanRare, \
	bDropLevel, \
	dwSpeedInc, \
	bItemCountry, \
	bIsSpecial, \
	dwDelay, \
	bCanGamble, \
	wItemProb_G, \
	bGambleProb, \
	bDestroyProb, \
	dwDuraMax, \
	bRefineMax, \
	bCanRepair, \
	wDelayGroupID, \
	wWeight, \
	bGroupID, \
	bInitState, \
	bCanWrap, \
	dwCode, \
	bCanColor, \
	fRevision, \
	fMRevision, \
	fAtRate, \
	fMAtRate, \
	bConsumable \
	FROM TITEMCHART"))

BEGIN_COLUMN(50)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bKind)
	COLUMN_ENTRY(m_wAttrID)
	COLUMN_ENTRY(m_wUseValue)
	COLUMN_ENTRY(m_dwClassID)
	COLUMN_ENTRY(m_dwSlotID)
	COLUMN_ENTRY(m_bPrmSlotID)
	COLUMN_ENTRY(m_bSubSlotID)
	COLUMN_ENTRY(m_bSlotCount)
	COLUMN_ENTRY(m_fPrice)
	COLUMN_ENTRY(m_fPvPrice)
	COLUMN_ENTRY(m_bIsSell)
	COLUMN_ENTRY(m_bMinRange)
	COLUMN_ENTRY(m_bMaxRange)
	COLUMN_ENTRY(m_bDefaultLevel)
	COLUMN_ENTRY(m_bStack)
	COLUMN_ENTRY(m_bEquipSkill)
	COLUMN_ENTRY(m_bUseItemKind)
	COLUMN_ENTRY(m_bUseItemCount)
	COLUMN_ENTRY(m_bGrade)
	COLUMN_ENTRY(m_wUseTime)
	COLUMN_ENTRY(m_bUseType)
	COLUMN_ENTRY(m_bCanGrade)
	COLUMN_ENTRY(m_bCanMagic)
	COLUMN_ENTRY(m_bCanRare)
	COLUMN_ENTRY(m_bDropLevel)
	COLUMN_ENTRY(m_dwSpeedInc)
	COLUMN_ENTRY(m_bItemCountry)
	COLUMN_ENTRY(m_bIsSpecial)
	COLUMN_ENTRY(m_dwDelay)
	COLUMN_ENTRY(m_bCanGamble)
	COLUMN_ENTRY(m_wItemProb_G)
	COLUMN_ENTRY(m_bGambleProb)
	COLUMN_ENTRY(m_bDestroyProb)
	COLUMN_ENTRY(m_dwDuraMax)
	COLUMN_ENTRY(m_bRefineMax)
	COLUMN_ENTRY(m_bCanRepair)
	COLUMN_ENTRY(m_wDelayGroupID)
	COLUMN_ENTRY(m_wWeight)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY(m_bInitState)
	COLUMN_ENTRY(m_bCanWrap)
	COLUMN_ENTRY(m_dwCode)
	COLUMN_ENTRY(m_bCanColor)
	for(int i=0; i<4; i++)
	{
		COLUMN_ENTRY(m_fRevision[i])
	}
	COLUMN_ENTRY(m_bConsumable)
END_COL()
};

class CTBLItemAttrChart : public CSqlQuery
{
public:
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

DEF_QUERY( CTBLItemAttrChart, _T("SELECT \
	wID, \
	bKind, \
	bGrade, \
	wMinAP, \
	wMaxAP, \
	wDP, \
	wMinMAP, \
	wMaxMAP, \
	wMDP, \
	bBlockProb \
	FROM TITEMATTRCHART"))

BEGIN_COLUMN(10)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bKind)
	COLUMN_ENTRY(m_bGrade)
	COLUMN_ENTRY(m_wMinAP)
	COLUMN_ENTRY(m_wMaxAP)
	COLUMN_ENTRY(m_wDP)
	COLUMN_ENTRY(m_wMinMAP)
	COLUMN_ENTRY(m_wMaxMAP)
	COLUMN_ENTRY(m_wMDP)
	COLUMN_ENTRY(m_bBlockProb)
END_COL()
};

class CTBLItemGradeChart : public CSqlQuery
{
public:
	BYTE m_bLevel;
	BYTE m_bGrade;
	BYTE m_bProb;
	DWORD m_dwMoney;

DEF_QUERY(CTBLItemGradeChart, _T("SELECT \
	bLevel, \
	bGrade, \
	bProb, \
	dwMoney \
	FROM TITEMGRADECHART"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bGrade)
	COLUMN_ENTRY(m_bProb)
	COLUMN_ENTRY(m_dwMoney)
END_COL()
};

class CTBLInven : public CSqlQuery
{
public:
	DWORD m_dwCharID;

	BYTE m_bInvenID;
	WORD m_wItemID;
	TIMESTAMP_STRUCT m_dEndTime;
	BYTE m_bELD;

DEF_QUERY( CTBLInven, _T("SELECT \
	bInvenID, \
	wItemID, \
	dEndTime, \
	bELD \
	FROM TINVENTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_bInvenID)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_dEndTime)
	COLUMN_ENTRY(m_bELD)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};


class CTBLItem : public CSqlQuery
{
public:
	BYTE m_bStorageType;
	DWORD m_dwStorageID;
	BYTE m_bOwnerType;
	DWORD m_dwOwnerID;
	TITEMQUERY m_item;

DEF_QUERY( CTBLItem, _T("SELECT \
	dlID, \
	bStorageType, \
	dwStorageID, \
	bOwnerType, \
	dwOwnerID, \
	bItemID, \
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
	WHERE dwOwnerID = ? AND bOwnerType=? AND bStorageType<>2"))

BEGIN_COLUMN(33)
	COLUMN_ENTRY(m_item.m_dlID)
	COLUMN_ENTRY(m_bStorageType)
	COLUMN_ENTRY(m_dwStorageID)
	COLUMN_ENTRY(m_bOwnerType)
	COLUMN_ENTRY(m_dwOwnerID)
	COLUMN_ENTRY(m_item.m_bItemID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_item.m_dEndTime)
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

BEGIN_PARAM(2)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
END_PARAM()
};

class CTBLPostItem : public CSqlQuery
{
public:
	BYTE m_bStorageType;
	DWORD m_dwStorageID;
	BYTE m_bOwnerType;
	DWORD m_dwOwnerID;
	TITEMQUERY m_item;

DEF_QUERY( CTBLPostItem, _T("SELECT \
	dlID, \
	bItemID, \
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
	WHERE dwOwnerID = ? AND bOwnerType=? AND bStorageType=? AND dwStorageID=? "))

BEGIN_COLUMN(29)
	COLUMN_ENTRY(m_item.m_dlID)
	COLUMN_ENTRY(m_item.m_bItemID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_item.m_dEndTime)
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

BEGIN_PARAM(4)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStorageType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwStorageID)
END_PARAM()
};

class CTBLSkillChart : public CSqlQuery
{
public:
	WORD m_wID;

	WORD m_wPrevActiveID;
	WORD m_wParentSkillID;

	WORD m_wMaxRange;
	WORD m_wMinRange;
	WORD m_wPosture;
	WORD m_wTargetActiveID;
	WORD m_wUseItem;

	DWORD m_dwConditionID;
	DWORD m_dwWeaponID;
	DWORD m_dwClassID;
	BYTE m_bKind;

	DWORD m_dwActionTime;
	DWORD m_dwDuration;
	DWORD m_dwDurationInc;
	DWORD m_dwReuseDelay;
	DWORD m_dwKindDelay;
	DWORD m_dwLoopDelay;
	DWORD m_dwUseMP;
	DWORD m_dwUseHP;
	BYTE m_bUseHPType;
	BYTE m_bUseMPType;
	BYTE m_bHitTest;
	BYTE m_bHitInit;
	BYTE m_bHitInc;
	DWORD m_dwAggro;
	int m_nReuseDelayInc;

	FLOAT m_fPrice;
	BYTE m_bLevel;
	BYTE m_bMaxLevel;
	BYTE m_bNextLevel;
	BYTE m_bPositive;
	BYTE m_bPriority;

	BYTE m_bTarget;
	BYTE m_bTargetRange;
	BYTE m_bIsuse;
	BYTE m_bTargetHit;
	BYTE m_bSpeedApply;
	BYTE m_bMaintainType;

	BYTE m_bCanLearn;
	BYTE m_bORadius;

	BYTE m_bIsRide;
	BYTE m_bIsDismount;
	BYTE m_bDuraSlot;
	BYTE m_bCanCancel;
	BYTE m_bGlobal;
	BYTE m_bRadius;
	BYTE m_bStatic;
	BYTE m_bEraseAct;
	BYTE m_bEraseHide;
	BYTE m_bIsHideSkill;
	BYTE m_bRunFromServer;
	BYTE m_bCheckAttacker;
	WORD m_wTriggerID;
	WORD m_wMapID;

DEF_QUERY( CTBLSkillChart, _T("SELECT \
	wID, \
	wPrevActiveID, \
	wParentSkillID, \
	wItemID, \
	wMaxRange, \
	wMinRange, \
	wPosture, \
	dwConditionID, \
	dwWeaponID, \
	dwClassID, \
	bKind, \
	fPrice, \
	dwUseMP, \
	bUseMPType, \
	dwUseHP, \
	bUseHPType, \
	dwActionTime, \
	dwDuration, \
	dwDurationInc, \
	dwReuseDelay, \
	nReuseDelayInc, \
	dwLoopDelay, \
	dwKindDelay, \
	dwAggro, \
	bLevel, \
	bMaxLevel, \
	bNextLevel, \
	bTarget, \
	bTargetRange, \
	bIsuse, \
	bTargetHit, \
	bPositive, \
	bPriority, \
	bSpeedApply, \
	bCanLearn, \
	bORadius, \
	bIsRide, \
	bIsDismount, \
	wTargetActiveID, \
	bMaintainType, \
	bDuraSlot, \
	bCanCancel, \
	bHitTest, \
	bHitInit, \
	bHitInc, \
	bGlobal, \
	bRadius, \
	bStatic, \
	bEraseAct, \
	bEraseHide, \
	bIsHideSkill, \
	bRunFromServer, \
	bCheckAttacker, \
	wTriggerID, \
	wMapID \
	FROM TSKILLCHART"))

BEGIN_COLUMN(58)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wPrevActiveID)
	COLUMN_ENTRY(m_wParentSkillID)
	COLUMN_ENTRY(m_wUseItem)
	COLUMN_ENTRY(m_wMaxRange)
	COLUMN_ENTRY(m_wMinRange)
	COLUMN_ENTRY(m_wPosture)
	COLUMN_ENTRY(m_dwConditionID)
	COLUMN_ENTRY(m_dwWeaponID)
	COLUMN_ENTRY(m_dwClassID)
	COLUMN_ENTRY(m_bKind)
	COLUMN_ENTRY(m_fPrice)
	COLUMN_ENTRY(m_dwUseMP)
	COLUMN_ENTRY(m_bUseMPType)
	COLUMN_ENTRY(m_dwUseHP)
	COLUMN_ENTRY(m_bUseHPType)
	COLUMN_ENTRY(m_dwActionTime)
	COLUMN_ENTRY(m_dwDuration)
	COLUMN_ENTRY(m_dwDurationInc)
	COLUMN_ENTRY(m_dwReuseDelay)
	COLUMN_ENTRY(m_nReuseDelayInc)
	COLUMN_ENTRY(m_dwLoopDelay)
	COLUMN_ENTRY(m_dwKindDelay)
	COLUMN_ENTRY(m_dwAggro)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bMaxLevel)
	COLUMN_ENTRY(m_bNextLevel)
	COLUMN_ENTRY(m_bTarget)
	COLUMN_ENTRY(m_bTargetRange)
	COLUMN_ENTRY(m_bIsuse)
	COLUMN_ENTRY(m_bTargetHit)
	COLUMN_ENTRY(m_bPositive)
	COLUMN_ENTRY(m_bPriority)
	COLUMN_ENTRY(m_bSpeedApply)
	COLUMN_ENTRY(m_bCanLearn)
	COLUMN_ENTRY(m_bORadius)
	COLUMN_ENTRY(m_bIsRide)
	COLUMN_ENTRY(m_bIsDismount)
	COLUMN_ENTRY(m_wTargetActiveID)
	COLUMN_ENTRY(m_bMaintainType)
	COLUMN_ENTRY(m_bDuraSlot)
	COLUMN_ENTRY(m_bCanCancel)
	COLUMN_ENTRY(m_bHitTest)
	COLUMN_ENTRY(m_bHitInit)
	COLUMN_ENTRY(m_bHitInc)
	COLUMN_ENTRY(m_bGlobal)
	COLUMN_ENTRY(m_bRadius)
	COLUMN_ENTRY(m_bStatic)
	COLUMN_ENTRY(m_bEraseAct)
	COLUMN_ENTRY(m_bEraseHide)
	COLUMN_ENTRY(m_bIsHideSkill)
	COLUMN_ENTRY(m_bRunFromServer)
	COLUMN_ENTRY(m_bCheckAttacker)
	COLUMN_ENTRY(m_wTriggerID)
	COLUMN_ENTRY(m_wMapID)
END_COL()
};

class CTBLSkillData : public CSqlQuery
{
public:
	WORD	m_wSkillID;
	BYTE	m_bAction;
	BYTE	m_bType;
	BYTE	m_bAttr;
	BYTE	m_bExec;
	BYTE	m_bInc;
	WORD	m_wValue;
	WORD	m_wValueInc;
	BYTE	m_bCalc;

DEF_QUERY( CTBLSkillData, _T("SELECT bAction, bType, bAttr, bExec, bInc, wValue, wValueInc, bCalc FROM TSKILLDATA WHERE wSkillID = ?"))

BEGIN_COLUMN(8)
	COLUMN_ENTRY(m_bAction)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bAttr)
	COLUMN_ENTRY(m_bExec)
	COLUMN_ENTRY(m_bInc)
	COLUMN_ENTRY(m_wValue)
	COLUMN_ENTRY(m_wValueInc)
	COLUMN_ENTRY(m_bCalc)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkillID)
END_PARAM()
};

class CTBLSkill : public CSqlQuery
{
public:
	DWORD m_dwCharID;

	WORD m_wSkillID;
	BYTE m_bLevel;
	DWORD m_dwRemainTick;

DEF_QUERY( CTBLSkill, _T("SELECT \
	wSkillID, \
	bLevel, \
	dwRemainTick \
	FROM TSKILLTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wSkillID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwRemainTick)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLSkillMaintain : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wSkillID;
	BYTE m_bLevel;
	DWORD m_dwRemainTick;
	BYTE m_bAttackType;
	DWORD m_dwAttackID;
	BYTE m_bHostType;
	DWORD m_dwHostID;
	BYTE m_bAttackCountry;

DEF_QUERY( CTBLSkillMaintain, _T("SELECT \
	wSkillID, \
	bLevel, \
	dwRemainTick, \
	bAttackType, \
	dwAttackID, \
	bHostType, \
	dwHostID, \
	bAttackCountry \
	FROM TSKILLMAINTAINTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(8)
	COLUMN_ENTRY(m_wSkillID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwRemainTick)
	COLUMN_ENTRY(m_bAttackType)
	COLUMN_ENTRY(m_dwAttackID)
	COLUMN_ENTRY(m_bHostType)
	COLUMN_ENTRY(m_dwHostID)
	COLUMN_ENTRY(m_bAttackCountry)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLQuestChart : public CSqlQuery
{
public:
	DWORD m_dwQuestID;

	DWORD m_dwTriggerID;
	DWORD m_dwParentID;

	BYTE m_bTriggerType;
	BYTE m_bForceRun;
	BYTE m_bCountMax;
	BYTE m_bType;
	BYTE m_bConditionCheck;

DEF_QUERY( CTBLQuestChart, _T("SELECT \
	dwQuestID, \
	dwTriggerID, \
	bTriggerType, \
	bForceRun, \
	bCountMax, \
	bType, \
	bConditionCheck \
	FROM TQUESTCHART \
	WHERE dwParentID = ? \
	ORDER BY bMain DESC"));

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_dwQuestID)
	COLUMN_ENTRY(m_dwTriggerID)
	COLUMN_ENTRY(m_bTriggerType)
	COLUMN_ENTRY(m_bForceRun)
	COLUMN_ENTRY(m_bCountMax)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bConditionCheck)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwParentID)
END_PARAM()
};

class CTBLQuestConditionChart : public CSqlQuery
{
public:
	DWORD m_dwQuestID;

	DWORD m_dwConditionID;
	BYTE m_bConditionType;
	BYTE m_bCount;

DEF_QUERY( CTBLQuestConditionChart, _T("SELECT \
	dwConditionID, \
	bConditionType, \
	bCount \
	FROM TQCONDITIONCHART \
	WHERE dwQuestID = ? \
	ORDER BY bConditionType DESC"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwConditionID)
	COLUMN_ENTRY(m_bConditionType)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwQuestID)
END_PARAM()
};

class CTBLQuestRewardChart : public CSqlQuery
{
public:
	DWORD m_dwQuestID;

	DWORD m_dwRewardID;
	BYTE m_bRewardType;
	BYTE m_bTakeMethod;
	BYTE m_bTakeData;
	BYTE m_bCount;

DEF_QUERY( CTBLQuestRewardChart, _T("SELECT \
	dwRewardID, \
	bRewardType, \
	bTakeMethod, \
	bTakeData, \
	bCount \
	FROM TQREWARDCHART \
	WITH (NOLOCK) \
	WHERE dwQuestID = ?"));

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_dwRewardID)
	COLUMN_ENTRY(m_bRewardType)
	COLUMN_ENTRY(m_bTakeMethod)
	COLUMN_ENTRY(m_bTakeData)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwQuestID)
END_PARAM()
};

class CTBLQuestMagicItemChart : public CSqlQuery
{
public:
	DWORD m_dwRewardID;
	BYTE m_bDropLevel;
	WORD m_wUseTime;
	TITEMQUERY m_item;
	DWORD m_dwMoney;

DEF_QUERY( CTBLQuestMagicItemChart, _T("SELECT \
    dwID, \
	wItemID, \
	bLevel, \
	bGLevel, \
	bDropLevel, \
	dwDuraMax, \
	dwDuraCur, \
	bRefineCur, \
	wUseTime, \
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
	dwTime6, \
	dwMoney \
	FROM TQUESTITEMCHART"));

BEGIN_COLUMN(29)
	COLUMN_ENTRY(m_dwRewardID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_bDropLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_wUseTime)
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
	COLUMN_ENTRY(m_dwMoney)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwRewardID)
END_PARAM()
};

class CTBLQuestTermChart : public CSqlQuery
{
public:
	DWORD m_dwQuestID;

	DWORD m_dwTermID;
	BYTE m_bTermType;
	BYTE m_bCount;

DEF_QUERY( CTBLQuestTermChart, _T("SELECT \
	dwTermID, \
	bTermType, \
	bCount \
	FROM TQUESTTERMCHART \
	WITH (NOLOCK) \
	WHERE dwQuestID = ? ORDER BY dwID"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwTermID)
	COLUMN_ENTRY(m_bTermType)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwQuestID)
END_PARAM()
};

class CTBLQuestTable : public CSqlQuery
{
public:
	DWORD m_dwQuestID;
	DWORD m_dwCharID;
	DWORD m_dwTick;

	BYTE m_bCompleteCount;
	BYTE m_bTriggerCount;

DEF_QUERY( CTBLQuestTable, _T("SELECT \
	dwQuestID, \
	dwTick, \
	bCompleteCount, \
	bTriggerCount \
	FROM TQUESTTABLE \
	WHERE dwCharID = ?"));

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwQuestID)
	COLUMN_ENTRY(m_dwTick)
	COLUMN_ENTRY(m_bCompleteCount)
	COLUMN_ENTRY(m_bTriggerCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLQuestTermTable : public CSqlQuery
{
public:
	DWORD m_dwQuestID;
	DWORD m_dwCharID;
	DWORD m_dwTermID;

	BYTE m_bTermType;
	BYTE m_bCount;

DEF_QUERY( CTBLQuestTermTable, _T("SELECT \
	dwQuestID, \
	dwTermID, \
	bTermType, \
	bCount \
	FROM TQUESTTERMTABLE \
	WHERE dwCharID = ?"));

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwQuestID)
	COLUMN_ENTRY(m_dwTermID)
	COLUMN_ENTRY(m_bTermType)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};
class CTBLSpawnPos : public CSqlQuery
{
public:
	WORD m_wMapID;
	WORD m_wID;
	BYTE m_bType;

	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

DEF_QUERY( CTBLSpawnPos, _T("SELECT \
	wID, \
	wMapID, \
	fPosX, \
	fPosY, \
	fPosZ, \
	bType \
	FROM TSPAWNPOSCHART"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
	COLUMN_ENTRY(m_bType)
END_COL()
};
class CTBLIndunChart : public CSqlQuery
{
public:
	WORD m_wID;
	WORD m_wInSpawn;
	WORD m_wOutSpawn_D;
	WORD m_wOutSpawn_C;

DEF_QUERY( CTBLIndunChart, _T("SELECT wMapID, wInSpawn, wOutSpawn_D, wOutSpawn_C FROM TINDUNCHART"));

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wInSpawn)
	COLUMN_ENTRY(m_wOutSpawn_D)
	COLUMN_ENTRY(m_wOutSpawn_C)
END_COL()
};
class CTBLNpc : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bType;
	BYTE m_bCountry;
	WORD m_wLocalID;
	BYTE m_bDiscountCondition; 
	BYTE m_bDiscountRate;  
	BYTE m_bAddProb;	
	WORD m_wItemID;
	WORD m_wMapID; 
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

DEF_QUERY( CTBLNpc, _T("SELECT \
	wID, \
	bType, \
	bCountryID, \
	wLocalID, \
	bCondition, \
	bDiscountRate, \
	bAddProb, \
	wItemID, \
	wMapID, \
	fPosX, \
	fPosY, \
	fPosZ \
	FROM TNPCCHART"));

BEGIN_COLUMN(12)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_wLocalID)
	COLUMN_ENTRY(m_bDiscountCondition)
	COLUMN_ENTRY(m_bDiscountRate)
	COLUMN_ENTRY(m_bAddProb)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
END_COL()
};
class CTBLNpcItem : public CSqlQuery
{
public:
	WORD m_wNpcID;
	DWORD m_dwItemID;

DEF_QUERY( CTBLNpcItem, _T("SELECT \
	dwItemID \
	FROM TNPCITEMCHART \
	WHERE wNpcID=?"));

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwItemID)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wNpcID)
END_PARAM()
};
class CTBLMonItem : public CSqlQuery
{
public:
	WORD m_wMonID;
	BYTE m_bChartType;
	WORD m_wItemID;
	WORD m_wItemIDMin;
	WORD m_wItemIDMax;
	WORD m_wWeight;
	BYTE m_bLevelMin;
	BYTE m_bLevelMax;
	BYTE m_bItemProb[MIP_COUNT];
	BYTE m_bItemMagicOpt;
	BYTE m_bItemRareOpt;

DEF_QUERY( CTBLMonItem, _T("SELECT \
	bChartType, \
	wItemID, \
	wItemIDMin, \
	wItemIDMax, \
	wWeight, \
	bLevelMin, \
	bLevelMax, \
	bItemProb_N1, \
	bItemProb_N2, \
	bItemProb_N3, \
	bItemProb_N4, \
	bItemProb_M, \
	bItemProb_S, \
	bItemProb_R, \
	bItemMagicOpt, \
	bItemRareOpt \
	FROM TMONITEMCHART \
	WHERE wMonID = ?"))

BEGIN_COLUMN(16)
	COLUMN_ENTRY(m_bChartType)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_wItemIDMin)
	COLUMN_ENTRY(m_wItemIDMax)
	COLUMN_ENTRY(m_wWeight)
	COLUMN_ENTRY(m_bLevelMin)
	COLUMN_ENTRY(m_bLevelMax)
	for(int i=0; i<MIP_COUNT; i++)
	{
		COLUMN_ENTRY(m_bItemProb[i])
	}
	COLUMN_ENTRY(m_bItemMagicOpt)
	COLUMN_ENTRY(m_bItemRareOpt)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wMonID)
END_PARAM()
};

class CTBLItemMagicChart : public CSqlQuery
{
public:
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

DEF_QUERY(CTBLItemMagicChart, _T("SELECT \
	bMagic, \
	dwKind, \
	bRvType, \
	wMaxValue, \
	bIsMagic, \
	bIsRare, \
	bMinLevel, \
	bExclIndex, \
	bOptionKind, \
	wAutoSkill, \
	bRefine, \
	wMaxBound, \
	wRareBound \
	FROM TITEMMAGICCHART"))

BEGIN_COLUMN(13)
	COLUMN_ENTRY(m_bMagic)
	COLUMN_ENTRY(m_dwKind)
	COLUMN_ENTRY(m_bRvType)
	COLUMN_ENTRY(m_wMaxValue)
	COLUMN_ENTRY(m_bIsMagic)
	COLUMN_ENTRY(m_bIsRare)
	COLUMN_ENTRY(m_bMinLevel)
	COLUMN_ENTRY(m_bExclIndex)
	COLUMN_ENTRY(m_bOptionKind)
	COLUMN_ENTRY(m_wAutoSkill)
	COLUMN_ENTRY(m_bRefine)
	COLUMN_ENTRY(m_wMaxBound)
	COLUMN_ENTRY(m_wRareBound)
END_COL()
};

class CTBLItemSetChart : public CSqlQuery
{
public:
	WORD m_wBaseID;
	WORD m_wSetID;
	BYTE m_bLevel;
	BYTE m_bMagic[TMAGIC_MAX];
	BYTE m_bMin[TMAGIC_MAX];
	BYTE m_bValue[TMAGIC_MAX];

DEF_QUERY(CTBLItemSetChart, _T("SELECT \
	wBaseID, \
	wSetID, \
	bLevel, \
	bMagic1, bMin1, bValue1, \
	bMagic2, bMin2, bValue2, \
	bMagic3, bMin3, bValue3, \
	bMagic4, bMin4, bValue4, \
	bMagic5, bMin5, bValue5, \
	bMagic6, bMin6, bValue6 \
	FROM TITEMSETCHART"))

BEGIN_COLUMN(21)
	COLUMN_ENTRY(m_wBaseID)
	COLUMN_ENTRY(m_wSetID)
	COLUMN_ENTRY(m_bLevel)
	for(BYTE i=0; i<TMAGIC_MAX; i++)
	{
		COLUMN_ENTRY(m_bMagic[i])
		COLUMN_ENTRY(m_bMin[i])
		COLUMN_ENTRY(m_bValue[i])
	}
END_COL()
};

class CTBLPostTable : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bRead;
	DWORD m_dwPostID;
	BYTE m_bType;
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];
	TIMESTAMP_STRUCT m_timeRecv;
	TCHAR m_szSender[MAX_NAME+1];
	DWORD m_dwBeginID;

DEF_QUERY(CTBLPostTable, _T("SELECT TOP 7 dwPostID, bRead, timeRecv, szSender, bType, szTitle FROM TPOSTTABLE WHERE dwCharID = ? AND dwPostID <= ? ORDER BY dwPostID DESC"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_dwPostID)
	COLUMN_ENTRY(m_bRead)
	COLUMN_ENTRY(m_timeRecv)
	COLUMN_ENTRY_STR(m_szSender)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY_STR(m_szTitle)
END_COL();

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBeginID)
END_PARAM()
};

class CTBLPostBill : public CSqlQuery
{
public:
	BYTE m_bType;
	DWORD m_dwSendID;
	DWORD m_dwPostID;
	TIMESTAMP_STRUCT m_timeRecv;

DEF_QUERY(CTBLPostBill, _T("SELECT dwPostID, timeRecv FROM TPOSTTABLE WHERE dwSendID=? AND bType=? AND bRead=0"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwPostID)
	COLUMN_ENTRY(m_timeRecv)
END_COL()

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSendID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
END_COL()
};

class CTBLCabinetTable : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bCabinetID;
	BYTE m_bUse;

DEF_QUERY(CTBLCabinetTable, _T("SELECT bCabinetID, bUse FROM TCABINETTABLE WHERE dwCharID = ? ORDER BY bCabinetID"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bCabinetID)
	COLUMN_ENTRY(m_bUse)
END_COL();

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLHotKey : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bInven;
	BYTE m_bType[MAX_HOTKEY_POS];
	WORD m_wHotID[MAX_HOTKEY_POS];

DEF_QUERY(CTBLHotKey, _T("SELECT * FROM THOTKEYTABLE WHERE dwCharID = ?"));
BEGIN_COLUMN(MAX_HOTKEY_POS*2 + 2)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_bInven)
	for(int i=0; i<MAX_HOTKEY_POS; i++)
	{
		COLUMN_ENTRY(m_bType[i])
		COLUMN_ENTRY(m_wHotID[i])
	}
END_COL();

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};
class CTBLFindChar : public CSqlQuery
{
public:
	TCHAR m_szName[MAX_NAME +1];
	DWORD m_dwID;

	DEF_QUERY(CTBLFindChar, _T("SELECT dwID FROM TCHARTABLE WHERE szName = ?"));
BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwID)
END_COL();

BEGIN_PARAM(1)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
END_PARAM()
};
class CTBLLocalInfo : public CSqlQuery
{
public:
	WORD m_wCastle;
	WORD m_wLocal;
	BYTE m_bCountry;
	DWORD m_dwGuild;

	DEF_QUERY(CTBLLocalInfo, _T("SELECT wLocalID, bCountry, dwGuild FROM TLOCALTABLE WHERE wLocalID IN(SELECT wID FROM TBATTLEZONECHART WHERE wCastle = ?)"));
BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wLocal)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_dwGuild)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wCastle)
END_PARAM()
};

class CTBLItemUsed : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wDelayGroupID;
	DWORD m_dwTick;

	DEF_QUERY(CTBLItemUsed, _T("SELECT wDelayGroupID, dwTick FROM TITEMUSEDTABLE WHERE dwCharID = ?"))
BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_wDelayGroupID)
	COLUMN_ENTRY(m_dwTick)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};
class CTBLRecallMon : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwID;
	WORD m_wMonID;
	WORD m_wPetID;
	DWORD m_dwATTR;
	BYTE m_bLevel;
	DWORD m_dwHP;
	DWORD m_dwMP;
	BYTE m_bSkillLevel;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	DWORD m_dwTime;

DEF_QUERY(CTBLRecallMon, _T("SELECT dwID, wMonID, wPetID, dwATTR, bLevel, dwHP, dwMP, bSkillLevel, wPosX, wPosY, wPosZ, dwTime FROM TRECALLMONTABLE WHERE dwOwnerID=?"))

BEGIN_COLUMN(13)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_wMonID)
	COLUMN_ENTRY(m_wPetID)
	COLUMN_ENTRY(m_dwATTR)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwHP)
	COLUMN_ENTRY(m_dwMP)
	COLUMN_ENTRY(m_bSkillLevel)
	COLUMN_ENTRY(m_wPosX)
	COLUMN_ENTRY(m_wPosY)
	COLUMN_ENTRY(m_wPosZ)
	COLUMN_ENTRY(m_dwTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};
class CTBLPortalChart : public CSqlQuery
{
public:
	WORD m_wPortalID;
	BYTE m_bCountry;
	WORD m_wLocalID;
	WORD m_wSpawnID;
	BYTE m_bCondition;

DEF_QUERY( CTBLPortalChart, _T("SELECT \
	wPortalID, \
	bCountry, \
	wLocalID, \
	wSpawnID, \
	bCondition \
	FROM TPORTALCHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_wPortalID)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_wLocalID)
	COLUMN_ENTRY(m_wSpawnID)
	COLUMN_ENTRY(m_bCondition)
END_COL()
};
class CTBLDestinationChart : public CSqlQuery
{
public:
	WORD m_wPortalID;
	WORD m_wDestID;
	DWORD m_dwPrice;
	BYTE m_bEnable;
	BYTE m_bCondition[3];
	DWORD m_dwConditionID[3];

DEF_QUERY( CTBLDestinationChart, _T("SELECT \
	wDestID, \
	dwPrice, \
	bEnable, \
	bCondition1, \
	dwConditionID1, \
	bCondition2, \
	dwConditionID2, \
	bCondition3, \
	dwConditionID3 \
	FROM TDESTINATIONCHART \
	WHERE wPortalID = ?"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_wDestID)	
	COLUMN_ENTRY(m_dwPrice)
	COLUMN_ENTRY(m_bEnable)
	for(BYTE i = 0; i < PORTALCONDITION_COUNT; i++)
	{
		COLUMN_ENTRY(m_bCondition[i])
		COLUMN_ENTRY(m_dwConditionID[i])
	}
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wPortalID)
END_PARAM()
};
class CTBLGateChart : public CSqlQuery
{
public:
	DWORD m_dwGateID;
	DWORD m_dwSwitchID;
	WORD m_wMapID;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	BYTE m_bType;

DEF_QUERY(CTBLGateChart, _T("SELECT dwGateID, dwSwitchID, bType, wMapID, wPosX, wPosY, wPosZ FROM TGATECHART"))
BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_dwGateID)
	COLUMN_ENTRY(m_dwSwitchID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wPosX)
	COLUMN_ENTRY(m_wPosY)
	COLUMN_ENTRY(m_wPosZ)
END_COL()
};

class CTBLSwitchChart : public CSqlQuery
{
public:
	DWORD m_dwSwitchID;
	WORD m_wMapID;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	BYTE m_bOpen;
	BYTE m_bLockOnOpen;
	BYTE m_bLockOnClose;
	DWORD m_dwDuration;

DEF_QUERY(CTBLSwitchChart, _T("SELECT dwSwitchID, wMapID, wPosX, wPosY, wPosZ, bStart, bLockOnOpen, bLockOnClose, dwDuration FROM TSWITCHCHART"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_dwSwitchID)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_wPosX)
	COLUMN_ENTRY(m_wPosY)
	COLUMN_ENTRY(m_wPosZ)
	COLUMN_ENTRY(m_bOpen)
	COLUMN_ENTRY(m_bLockOnOpen)
	COLUMN_ENTRY(m_bLockOnClose)
	COLUMN_ENTRY(m_dwDuration)
END_COL()
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

class CTBLPetTable : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	WORD m_wPetID;
	TCHAR m_szName[MAX_NAME+1];
	TIMESTAMP_STRUCT m_timeUse;

DEF_QUERY(CTBLPetTable, _T("SELECT wPetID, szName, timeUse FROM TPETTABLE WHERE dwUserID = ?"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_wPetID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_timeUse)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};
class CTBLMonAttr : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bLevel;

	WORD	m_wAP;
	WORD	m_wLAP;
	DWORD	m_dwAtkSpeed;
	WORD	m_wAL;
	WORD	m_wMAL;
	WORD	m_wDL;
	WORD	m_wMDL;
	BYTE	m_bCriticalPP;
	DWORD	m_dwMaxHP;
	BYTE	m_bHPRecover;
	WORD	m_wMAP;
	BYTE	m_bCriticalMP;
	DWORD	m_dwMaxMP;
	BYTE	m_bMPRecover;
	WORD	m_wDP;
	WORD	m_wMDP;
	WORD	m_wMinWAP;
	WORD	m_wMaxWAP;
	WORD	m_wWDP;

DEF_QUERY( CTBLMonAttr, _T("SELECT \
	wID, \
	bLevel, \
	wAP, \
	wLAP, \
	dwAtkSpeed, \
	wAL, \
	wMAL, \
	wDL, \
	wMDL, \
	bCriticalPP, \
	dwMaxHP, \
	bHPRecover, \
	wMAP, \
	bCriticalMP, \
	dwMaxMP, \
	bMPRecover, \
	wDP, \
	wMDP, \
	wMinWAP, \
	wMaxWAP, \
	wWDP \
	FROM TMONATTRCHART"))

BEGIN_COLUMN(21)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_wAP)
	COLUMN_ENTRY(m_wLAP)
	COLUMN_ENTRY(m_dwAtkSpeed)
	COLUMN_ENTRY(m_wAL)
	COLUMN_ENTRY(m_wMAL)
	COLUMN_ENTRY(m_wDL)
	COLUMN_ENTRY(m_wMDL)
	COLUMN_ENTRY(m_bCriticalPP)
	COLUMN_ENTRY(m_dwMaxHP)
	COLUMN_ENTRY(m_bHPRecover)
	COLUMN_ENTRY(m_wMAP)
	COLUMN_ENTRY(m_bCriticalMP)
	COLUMN_ENTRY(m_dwMaxMP)
	COLUMN_ENTRY(m_bMPRecover)
	COLUMN_ENTRY(m_wDP)
	COLUMN_ENTRY(m_wMDP)
	COLUMN_ENTRY(m_wMinWAP)
	COLUMN_ENTRY(m_wMaxWAP)
	COLUMN_ENTRY(m_wWDP)
END_COL()
};

class CTBLItemMagicSkill : public CSqlQuery
{
public:
	BYTE	m_bMagic;
	DWORD	m_dwKind;
	WORD	m_wSkill;
	BYTE	m_bIsMagic;
	BYTE	m_bIsRare;
	BYTE	m_bMinLevel;

DEF_QUERY( CTBLItemMagicSkill, _T("SELECT \
	bMagic, \
	dwKind, \
	wSkillID, \
	bIsMagic, \
	bIsRare, \
	bMinLevel \
	FROM TITEMMAGICSKILLCHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bMagic)
	COLUMN_ENTRY(m_dwKind)
	COLUMN_ENTRY(m_wSkill)
	COLUMN_ENTRY(m_bIsMagic)
	COLUMN_ENTRY(m_bIsRare)
	COLUMN_ENTRY(m_bMinLevel)
END_COL()
};

class CTBLCashCategory : public CSqlQuery
{
public:
	BYTE m_bID;
	TCHAR m_szName[MAX_NAME + 1];

DEF_QUERY( CTBLCashCategory, _T("SELECT bID, szName FROM TVIEW_CASHCATEGORYCHART ORDER BY wOrder"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bID)
	COLUMN_ENTRY_STR(m_szName)
END_COL()
};

class CTBLCashShopItem : public CSqlQuery
{
public:
	WORD m_wID;
	DWORD m_dwMoney;
	BYTE m_bCategory;
	BYTE m_bKind;
	BYTE m_bCanSell;
	WORD m_wUseTime;
	DWORD m_wInfoID;
	TITEMQUERY m_item;
	BYTE m_bSaleValue;  

DEF_QUERY( CTBLCashShopItem, _T("SELECT \
	wID, \
	wInfoID, \
	dwMoney, \
	bCategory, \
	bKind, \
	wItemID, \
	bLevel, \
	bCount, \
	bGLevel, \
	dwDuraMax, \
	dwDuraCur, \
	bRefineCur, \
	wUseTime, \
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
	dwTime6, \
	bCanSell, \
	bSaleValue \
	FROM TVIEW_CASHSHOPITEMCHART \
	ORDER BY bCategory, wOrder"))

BEGIN_COLUMN(34)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wInfoID)
	COLUMN_ENTRY(m_dwMoney)
	COLUMN_ENTRY(m_bCategory)
	COLUMN_ENTRY(m_bKind)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_wUseTime)
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
	COLUMN_ENTRY(m_bCanSell)
	COLUMN_ENTRY(m_bSaleValue)
END_COL()
};

class CTBLCashItemCabinet : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	DWORD m_dwID;
	TITEMQUERY m_item;	

DEF_QUERY( CTBLCashItemCabinet, _T("SELECT \
    TOP 500 \
	dwID, \
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
	FROM TVIEW_CASHITEMCABINET \
	WHERE dwUserID = ?"))

BEGIN_COLUMN(28)
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_item.m_dEndTime)
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

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};

class CTBLDuringItem : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	WORD m_wItemID;
	BYTE m_bType;
	DWORD m_dwRemainTime;
	TIMESTAMP_STRUCT m_tEndTime;

DEF_QUERY(CTBLDuringItem, _T("SELECT \
	wItemID, \
	bType, \
	dwRemainTime, \
	dEndTime \
	FROM TVIEW_DURINGITEMTABLE \
	WHERE dwUserID = ?"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwRemainTime)
	COLUMN_ENTRY(m_tEndTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};

class CTBLExpItem : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wItemID;
	BYTE m_bType;
	DWORD m_dwRemainTime;
	TIMESTAMP_STRUCT m_tEndTime;

DEF_QUERY(CTBLExpItem, _T("SELECT \
	wItemID, \
	bType, \
	dwRemainTime, \
	dEndTime \
	FROM TEXPITEMTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_dwRemainTime)
	COLUMN_ENTRY(m_tEndTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLGamble : public CSqlQuery
{
public:
	BYTE m_bType;
	BYTE m_bKind;
	WORD m_wReplaceID;
	BYTE m_bCountMax;
	BYTE m_bMinLevel;
	BYTE m_bMaxLevel;
	WORD m_wProb;

DEF_QUERY( CTBLGamble, _T("SELECT \
	bType, \
	bKind, \
	wReplaceID, \
	bCountMax, \
	bMinLevel, \
	bMaxLevel, \
	wProb \
	FROM TGAMBLECHART"))

BEGIN_COLUMN(7)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bKind)
	COLUMN_ENTRY(m_wReplaceID)
	COLUMN_ENTRY(m_bCountMax)
	COLUMN_ENTRY(m_bMinLevel)
	COLUMN_ENTRY(m_bMaxLevel)
	COLUMN_ENTRY(m_wProb)
END_COL()
};

class CTBLGodTower : public CSqlQuery
{
public:
	WORD m_wID;
	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

DEF_QUERY( CTBLGodTower, _T("SELECT \
	wID, \
	wMapID, \
	fPosX, \
	fPosY, \
	fPosZ \
	FROM TGODTOWERCHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
END_COL()
};

class CTBLGodBall : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bCamp;
	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

DEF_QUERY( CTBLGodBall, _T("SELECT \
	wID, \
	bCamp, \
	wMapID, \
	fPosX, \
	fPosY, \
	fPosZ \
	FROM TGODBALLCHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bCamp)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_fPosX)
	COLUMN_ENTRY(m_fPosY)
	COLUMN_ENTRY(m_fPosZ)
END_COL()
};

class CTBLLocalOccupy : public CSqlQuery
{
public:
	WORD m_wLocalID;
	BYTE m_bDay;
	DWORD m_dwGuildID;
	BYTE m_bType;

DEF_QUERY( CTBLLocalOccupy, _T("SELECT \
	wLocalID, \
	bDay, \
	dwGuildID, \
	bType \
	FROM TLOCALOCCUPYTABLE"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_wLocalID)
	COLUMN_ENTRY(m_bDay)
	COLUMN_ENTRY(m_dwGuildID)
	COLUMN_ENTRY(m_bType)
END_COL()
};

class CTBLSkillPoint : public CSqlQuery
{
public:
	WORD m_wSkillID;
	BYTE m_bLevel;
	BYTE m_bSkillPoint;
	BYTE m_bGroupPoint;
	BYTE m_bParentLevel;
	DWORD m_dwPayback;

DEF_QUERY( CTBLSkillPoint, _T("SELECT \
	wID, \
	bLevel, \
	bSkillPoint, \
	bGroupPoint, \
	bPrevSkillLevel, \
	dwPayback \
	FROM TSKILLPOINTCHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_wSkillID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bSkillPoint)
	COLUMN_ENTRY(m_bGroupPoint)
	COLUMN_ENTRY(m_bParentLevel)
	COLUMN_ENTRY(m_dwPayback)
END_COL()
};

class CTBLRecallMaintain : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwRecallID;
	WORD m_wSkillID;
	BYTE m_bLevel;
	DWORD m_dwRemainTick;
	BYTE m_bAttackType;
	DWORD m_dwAttackID;
	BYTE m_bHostType;
	DWORD m_dwHostID;
	BYTE m_bAttackCountry;

DEF_QUERY( CTBLRecallMaintain, _T("SELECT \
	dwRecallID, \
	wSkillID, \
	bLevel, \
	dwRemainTick, \
	bAttackType, \
	dwAttackID, \
	bHostType, \
	dwHostID, \
	bAttackCountry \
	FROM TRECALLMAINTAINTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_dwRecallID)
	COLUMN_ENTRY(m_wSkillID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwRemainTick)
	COLUMN_ENTRY(m_bAttackType)
	COLUMN_ENTRY(m_dwAttackID)
	COLUMN_ENTRY(m_bHostType)
	COLUMN_ENTRY(m_dwHostID)
	COLUMN_ENTRY(m_bAttackCountry)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
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

class CTBLPvPointChart : public CSqlQuery
{
public:
	WORD m_wLocalID;
	BYTE m_bStatus;
	BYTE m_bEvent;
	BYTE m_bTarget;
	DWORD m_dwIncPoint;
	DWORD m_dwDecPoint;

DEF_QUERY(CTBLPvPointChart, _T("SELECT wLocalID, bStatus, bEvent, bTarget, dwIncPoint, dwDecPoint FROM TPVPOINTCHART"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_wLocalID)
	COLUMN_ENTRY(m_bStatus)
	COLUMN_ENTRY(m_bEvent)
	COLUMN_ENTRY(m_bTarget)
	COLUMN_ENTRY(m_dwIncPoint)
	COLUMN_ENTRY(m_dwDecPoint)
END_COL()
};

class CTBLPvPRecent : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	TCHAR	m_szName[MAX_NAME + 1];
	BYTE	m_bClass;
	BYTE	m_bLevel;
	BYTE	m_bWin;
	DWORD	m_dwPoint;
	TIMESTAMP_STRUCT m_dlDate;

DEF_QUERY(CTBLPvPRecent, _T("SELECT szName, bClass, bLevel, bWin, dwPoint, dlDate FROM TPVPRECENTTABLE WHERE dwCharID=?"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bWin)
	COLUMN_ENTRY(m_dwPoint)
	COLUMN_ENTRY(m_dlDate)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLDuelCharScore : public CSqlQuery
{
public:	
	DWORD m_aDuelScore[TCLASS_COUNT][2];
	DWORD m_dwCharID;

DEF_QUERY(CTBLDuelCharScore, _T("SELECT \
		dwWarriorLose, \
		dwWarriorWin, \
		dwRangerLose, \
		dwRangerWin, \
		dwArcherLose, \
		dwArcherWin, \
		dwWizardLose, \
		dwWizardWin, \
		dwPriestLose, \
		dwPriestWin, \
		dwSorcererLose, \
		dwSorcererWin \
		FROM TDUELSCORETABLE \
		WHERE dwCharID = ?"))

BEGIN_COLUMN(12)	
	for(BYTE i = 0; i < TCLASS_COUNT; i++)
	{
		COLUMN_ENTRY(m_aDuelScore[i][0])
		COLUMN_ENTRY(m_aDuelScore[i][1])
	}
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLDuelCharList : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME + 1];
	BYTE	m_bClass;
	BYTE	m_bLevel;
	BYTE	m_bWin;
	DWORD	m_dwPoint;
	TIMESTAMP_STRUCT	m_dTime;

DEF_QUERY(CTBLDuelCharList, _T("SELECT TOP 10 \
	szName, \
	bClass, \
	bLevel, \
	bWin, \
	dwPoint, \
	dTime \
	FROM TDUELCHARTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(6)	
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bWin)
	COLUMN_ENTRY(m_dwPoint)
	COLUMN_ENTRY(m_dTime)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLMonsterShop : public CSqlQuery
{
public:
	WORD m_wID;
	WORD m_wNpcID;
	WORD m_wSpawnID;
	DWORD m_dwPrice;
	WORD m_wTowerID;

DEF_QUERY(CTBLMonsterShop, _T("SELECT wID, wNpcID, wSpawnID, dwPrice, wTowerID FROM TMONSTERSHOPCHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_wNpcID)
	COLUMN_ENTRY(m_wSpawnID)
	COLUMN_ENTRY(m_dwPrice)
	COLUMN_ENTRY(m_wTowerID)
END_COL()
};

class CTBLCashGambleChart : public CSqlQuery
{
public:
	DWORD m_dwID;
	DWORD m_dwProb;
	WORD m_wGroup;
	WORD m_wUseTime;
	TITEMQUERY m_item;

DEF_QUERY( CTBLCashGambleChart, _T("SELECT \
	dwID, \
	dwProb, \
	wGroup, \
	wUseTime, \
	wItemID, \
	bLevel, \
	bCount, \
	bGLevel, \
	dwDuraMax, \
	dwDuraCur, \
	bRefineCur, \
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
	FROM TVIEW_CASHGAMBLECHART"))

BEGIN_COLUMN(32) 
	COLUMN_ENTRY(m_dwID)
	COLUMN_ENTRY(m_dwProb)
	COLUMN_ENTRY(m_wGroup)
	COLUMN_ENTRY(m_wUseTime)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
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
};


class CTBLAuctionTable : public CSqlQuery
{
public:
	DWORD	m_dwAuctionID;
	WORD	m_wNpcID;  
	DWORD	m_dwSeller;
	BYTE	m_bBidCount;	
	TCHAR	m_szSeller[MAX_NAME+1];
	__int64	m_ldwDirectPrice;
	__int64	m_ldwStartPrice;
	TIMESTAMP_STRUCT	m_dStart;
	TIMESTAMP_STRUCT	m_dEnd;	
	TITEMQUERY m_item;

DEF_QUERY(CTBLAuctionTable, _T("SELECT \
	A.dwAuctionID, \
	A.dwCharID, \
	A.bBidCount, \
	A.dlDirectPrice, \
	A.dlStartPrice, \
	A.DateStart, \
	A.DateEnd, \
	C.szName, \
	T.dlID, \
	T.bItemID, \
	T.wItemID, \
	T.bLevel, \
	T.bCount, \
	T.bGLevel, \
	T.dwDuraMax, \
	T.dwDuraCur, \
	T.bRefineCur, \
	T.dEndTime, \
	T.bGradeEffect, \
	T.bMagic1, \
	T.bMagic2, \
	T.bMagic3, \
	T.bMagic4, \
	T.bMagic5, \
	T.bMagic6, \
	T.wValue1, \
	T.wValue2, \
	T.wValue3, \
	T.wValue4, \
	T.wValue5, \
	T.wValue6, \
	T.dwTime1, \
	T.dwTime2, \
	T.dwTime3, \
	T.dwTime4, \
	T.dwTime5, \
	T.dwTime6 \
	FROM TAUCTIONTABLE AS A\
	INNER JOIN TITEMTABLE AS T ON \
	A.dlItemID = T.dlID \
	INNER JOIN TCHARTABLE AS C ON \
	A.dwCharID = C.dwCharID \
	WHERE wNpcID = ?"))

BEGIN_COLUMN(37)
	COLUMN_ENTRY(m_dwAuctionID)	
	COLUMN_ENTRY(m_dwSeller)
	COLUMN_ENTRY(m_bBidCount)
	COLUMN_ENTRY(m_ldwDirectPrice)
	COLUMN_ENTRY(m_ldwStartPrice)
	COLUMN_ENTRY(m_dStart)
	COLUMN_ENTRY(m_dEnd)	
	COLUMN_ENTRY_STR(m_szSeller)
	COLUMN_ENTRY(m_item.m_dlID)
	COLUMN_ENTRY(m_item.m_bItemID)
	COLUMN_ENTRY(m_item.m_wItemID)
	COLUMN_ENTRY(m_item.m_bLevel)
	COLUMN_ENTRY(m_item.m_bCount)
	COLUMN_ENTRY(m_item.m_bGLevel)
	COLUMN_ENTRY(m_item.m_dwDuraMax)
	COLUMN_ENTRY(m_item.m_dwDuraCur)
	COLUMN_ENTRY(m_item.m_bRefineCur)
	COLUMN_ENTRY(m_item.m_dEndTime)
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

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wNpcID)
END_PARAM()
};

class CTBLAuctionInterest : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwAuctionID;	

DEF_QUERY(CTBLAuctionInterest,_T("SELECT \
	dwAuctionID \
	FROM TAUCTIONINTEREST \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(1)	
	COLUMN_ENTRY(m_dwAuctionID)	
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CTBLAuctionBidList : public CSqlQuery
{
public:
	DWORD m_dwAuctionID;
	DWORD m_dwCharID;
	__int64 m_ldwBidPrice;
	TIMESTAMP_STRUCT m_dBid;

DEF_QUERY(CTBLAuctionBidList, _T("SELECT \
	dwAuctionID, \
	dlBidPrice, \
	DateBid \
	FROM TAUCTIONBIDDER \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_dwAuctionID)
	COLUMN_ENTRY(m_ldwBidPrice)
	COLUMN_ENTRY(m_dBid)	
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
END_PARAM()
};

class CTBLAuctionRegList : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwAuctionID;	

DEF_QUERY(CTBLAuctionRegList, _T("SELECT \
	dwAuctionID \
	FROM TAUCTIONTABLE \
	WHERE dwCharID = ?"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwAuctionID)	
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
END_PARAM()
};

class CTBLALLAuctionBidder : public CSqlQuery
{
public:	
	DWORD m_dwAuctionID;
	DWORD m_dwCharID;
	__int64 m_ldwBidPrice;
	TIMESTAMP_STRUCT m_dBid;

DEF_QUERY(CTBLALLAuctionBidder, _T("SELECT \
	dwAuctionID, \
	dwCharID, \
	dlBidPrice, \
	DateBid \
	FROM TAUCTIONBIDDER"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwAuctionID)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_ldwBidPrice)
	COLUMN_ENTRY(m_dBid)
END_COL()
};
 
class CTBLMonthRankTable : public CSqlQuery
{
public:
	BYTE	m_bMonth;
	BYTE	m_bRank;
	BYTE	m_bMonthRank;
	DWORD	m_dwTotalRank;
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

DEF_QUERY(CTBLMonthRankTable, _T("SELECT \
	bMonth, \
	bRank, \
	bMonthRank, \
	dwTotalRank, \
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
	FROM TMONTHRANKTABLE WHERE bMonthRank < 9 ORDER BY bMonth,bMonthRank"))
	
BEGIN_COLUMN(21)
	COLUMN_ENTRY(m_bMonth)
	COLUMN_ENTRY(m_bRank)
	COLUMN_ENTRY(m_bMonthRank)
	COLUMN_ENTRY(m_dwTotalRank)
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
};

class CTBLHeroTable : public CSqlQuery
{
public:
	BYTE	m_bMonth;
	BYTE	m_bType;
	BYTE	m_bOrder;
	DWORD	m_dwMonthRank;
	DWORD	m_dwTotalRank;
	DWORD	m_dwTotalPoint;
	DWORD	m_dwMonthPoint;
	WORD	m_wMonthWin;
	WORD	m_wMonthLose;
	DWORD	m_dwTotalWin;
	DWORD	m_dwTotalLose;
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME +1];
	BYTE	m_bCountry;
	BYTE	m_bLevel;
	BYTE	m_bClass;
	BYTE	m_bRace;
	BYTE	m_bSex;
	BYTE	m_bHair;
	BYTE	m_bFace;
	TCHAR	m_szGuild[MAX_NAME +1];
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];

DEF_QUERY(CTBLHeroTable, _T("SELECT \
	bMonth, \
	bType, \
	bOrder, \
	dwMonthRank, \
	dwTotalRank, \
	dwTotalPoint, \
	dwMonthPoint, \
	wMonthWin, \
	wMonthLose, \
	dwTotalWin, \
	dwTotalLose, \
	dwCharID, \
	szName, \
	bCountry, \
	bLevel, \
	bClass, \
	bRace, \
	bSex, \
	bHair, \
	bFace, \
	szGuild, \
	szSay \
	FROM THEROTABLE"))
	
BEGIN_COLUMN(22)
	COLUMN_ENTRY(m_bMonth)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bOrder)
	COLUMN_ENTRY(m_dwMonthRank)
	COLUMN_ENTRY(m_dwTotalRank)
	COLUMN_ENTRY(m_dwTotalPoint)
	COLUMN_ENTRY(m_dwMonthPoint)
	COLUMN_ENTRY(m_wMonthWin)
	COLUMN_ENTRY(m_wMonthLose)
	COLUMN_ENTRY(m_dwTotalWin)
	COLUMN_ENTRY(m_dwTotalLose)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY_STR(m_szGuild)
	COLUMN_ENTRY_STR(m_szSay)
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

class CTBLTutorialCharItem : public CSqlQuery
{
public:

	BYTE	m_bCountry;
	BYTE	m_bClass;
	BYTE	m_bSex;

	WORD	m_wPrmWeapon;
	WORD	m_wSndWeapon;
	WORD	m_wLongWeapon;
	WORD	m_wHead;
	WORD	m_wBody;
	WORD	m_wPants;
	WORD	m_wHand;
	WORD	m_wFoot;
	WORD	m_wBack;

DEF_QUERY(CTBLTutorialCharItem, _T("SELECT \
	bCountry, \
	bClass, \
	bSex, \
	wPrmWeapon, \
	wSndWeapon, \
	wLongWeapon, \
	wHead, \
	wBody, \
	wPants, \
	wHand, \
	wFoot, \
	wBack \
	FROM TEQUIPCREATECHARCHART"))

BEGIN_COLUMN(12)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_wPrmWeapon)
	COLUMN_ENTRY(m_wSndWeapon)
	COLUMN_ENTRY(m_wLongWeapon)
	COLUMN_ENTRY(m_wHead)
	COLUMN_ENTRY(m_wBody)
	COLUMN_ENTRY(m_wPants)
	COLUMN_ENTRY(m_wHand)
	COLUMN_ENTRY(m_wFoot)
	COLUMN_ENTRY(m_wBack)
END_COL()
};

class CTBLHelpMessage : public CSqlQuery
{
public:
	BYTE m_bID;
	TIMESTAMP_STRUCT m_timeStart;
	TIMESTAMP_STRUCT m_timeEnd;
	TCHAR m_szMessage[MAX_BOARD_TEXT + 1];

DEF_QUERY(CTBLHelpMessage, _T("SELECT bID, dStart, dEnd, szMessage FROM THELPMESSAGETABLE"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_bID)
	COLUMN_ENTRY(m_timeStart)
	COLUMN_ENTRY(m_timeEnd)
	COLUMN_ENTRY_STR(m_szMessage)
END_COL()
};

class CTBLMissionTable : public CSqlQuery
{
public:
	WORD m_wMissionID;
	BYTE m_bCountry;

DEF_QUERY(CTBLMissionTable, _T("SELECT \
	wMissionID, \
	bCountry \
	FROM TMISSIONTABLE"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_wMissionID)
	COLUMN_ENTRY(m_bCountry)
END_COL()
};

class CTBLRPSGame : public CSqlQuery
{
public:
	BYTE m_bType;
	BYTE m_bWinCount;
	WORD m_wItemID;
	DWORD m_dwRewardMoney;
	WORD m_wRewardItem[2];
	BYTE m_bItemCount[2];
	BYTE m_bProb[3];
	WORD m_wWinKeep;
	WORD m_wWinPeriod;

DEF_QUERY(CTBLRPSGame, _T("SELECT \
	bType, \
	bWinCount, \
	wItemID, \
	dwRewardMoney, \
	wRewardItem_1, \
	wRewardItem_2, \
	bItemCount_1, \
	bItemCount_2, \
	bProb_Win, \
	bProb_Draw, \
	bProb_Lose, \
	wWinKeep, \
	wWinPeriod \
	FROM TRPSGAMECHART"))

BEGIN_COLUMN(13)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bWinCount)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_dwRewardMoney)
	COLUMN_ENTRY(m_wRewardItem[0])
	COLUMN_ENTRY(m_wRewardItem[1])
	COLUMN_ENTRY(m_bItemCount[0])
	COLUMN_ENTRY(m_bItemCount[1])
	COLUMN_ENTRY(m_bProb[0])
	COLUMN_ENTRY(m_bProb[1])
	COLUMN_ENTRY(m_bProb[2])
	COLUMN_ENTRY(m_wWinKeep)
	COLUMN_ENTRY(m_wWinPeriod)
END_COL()
};

class CTBLArena : public CSqlQuery
{
public:
	WORD m_wID;
	BYTE m_bType;
	WORD m_wInPos;
	WORD m_wOutPos;
	DWORD m_dwFee;

DEF_QUERY(CTBLArena, _T("SELECT wID, bType, wInPos, wOutPos, dwFee FROM TARENACHART"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_wInPos)
	COLUMN_ENTRY(m_wOutPos)
	COLUMN_ENTRY(m_dwFee)
END_COL()
};

class CTBLAidTable : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bCountry;
	TIMESTAMP_STRUCT m_dDate;

DEF_QUERY(CTBLAidTable, _T("SELECT \
	bCountry, \
	dDate \
	FROM TAIDTABLE WHERE dwCharID = ?"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_dDate)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class CSPCashItemBuy : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwUserID;
	DWORD m_dwCharID;
	WORD m_wCashItemID;
	DWORD m_dwCash;
	DWORD m_dwCashBonus;
	__int64 m_dlID;
	BYTE m_bBuyType;
	TCHAR m_szTarget[MAX_NAME + 1];
	DWORD m_dwTargetID;
	TCHAR m_szPresent[MAX_NAME + 1];

DEF_QUERY(CSPCashItemBuy, _T("{? = CALL TCashItemBuy(?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(11)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCash)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCashBonus)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dlID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwTargetID)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szPresent)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wCashItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bBuyType)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szTarget)
END_PARAM()
};

class CSPGetCash : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	DWORD m_dwCash;
	DWORD m_dwCashBonus;

DEF_QUERY(CSPGetCash, _T("{CALL TCashGet(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCash)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCashBonus)
END_PARAM()
};

class CSPCashItemGet : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwUserID;
	DWORD m_dwCharID;
	DWORD m_dwCashItemID;
	BYTE m_bInven;
	BYTE m_bSlot;
	TITEMQUERY m_item;

DEF_QUERY(CSPCashItemGet, _T("{? = CALL TCashItemGet(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(34)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCashItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bInven)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSlot)
END_PARAM()
};

class CSPSaveRecallMon : public CSqlQuery
{
public:
	int m_nRET;

	BYTE m_bDelete;
	DWORD m_dwCharID;
	DWORD m_dwMonID;
	WORD m_wTempID;
	WORD m_wPetID;
	DWORD m_dwAttr;
	BYTE m_bLevel;
	DWORD m_dwHP;
	DWORD m_dwMP;
	BYTE m_bSkillLevel;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	DWORD m_dwTick;

DEF_QUERY(CSPSaveRecallMon, _T("{? = CALL TSaveRecallMon(?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(14)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bDelete)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwMonID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wTempID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPetID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwAttr)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwHP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwMP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSkillLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosX)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosY)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosZ)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTick)
END_PARAM()
};

class CSPCreateRecallMon : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwMonID;
	DWORD m_dwCharID;
	WORD m_wTempID;
	WORD m_wPetID;
	DWORD m_dwATTR;
	BYTE m_bLevel;
	DWORD m_dwHP;
	DWORD m_dwMP;
	BYTE m_bSkillLevel;
	WORD m_wPosX;
	WORD m_wPosY;
	WORD m_wPosZ;
	DWORD m_dwTime;

DEF_QUERY(CSPCreateRecallMon, _T("{ ? = CALL TCreateRecallMon(?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(13)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwMonID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wTempID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPetID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwATTR)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwHP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwMP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSkillLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosX)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosY)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPosZ)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTime)
END_PARAM()
};

class CSPGetServerID : public CSqlQuery
{
public:
	int m_nRET;

	BYTE m_bServerID;
	BYTE m_bChannel;

	WORD m_wUnitID;
	WORD m_wMapID;

DEF_QUERY( CSPGetServerID, _T("{ ? = CALL TGetServerID(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wMapID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wUnitID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bChannel)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bServerID)
END_PARAM()
};

class CSPEnterServer : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szIPAddr[MAX_NAME + 1];
	WORD m_wPort;

	DWORD m_dwUserID;
	DWORD m_dwCharID;
	DWORD m_dwKEY;

	BYTE m_bGroupID;
	BYTE m_bChannel;

DEF_QUERY( CSPEnterServer, _T("{ ? = CALL TEnterServer(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(8)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwKEY)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGroupID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bChannel)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szIPAddr)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wPort)
END_PARAM()
};


class CSPLogout : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwID;
	DWORD m_dwCharID;

DEF_QUERY( CSPLogout, _T("{ ? = CALL TLogout(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPRoute : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szIPAddr[MAX_NAME + 1];

	BYTE m_bServerID;
	BYTE m_bGroupID;
	BYTE m_bType;

	WORD m_wPort;

DEF_QUERY( CSPRoute, _T("{ ? = CALL TRoute(?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGroupID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bServerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szIPAddr)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_wPort)
END_PARAM()
};

class CSPSaveChar : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwCharID;

	BYTE m_bStartAct; 
	BYTE m_bHelmetHide;

	DWORD m_dwCooper;
	DWORD m_dwSilver;
	DWORD m_dwGold;

	BYTE m_bGuildLeave;
	DWORD m_dwGuildLeaveTime;

	DWORD m_dwEXP;
	DWORD m_dwHP;
	DWORD m_dwMP;
	WORD m_wSkillPoint;
	DWORD m_dwRegion;

	BYTE m_bLevel;
	WORD m_wSpawnID;
	WORD m_wLastSpawnID;
	WORD m_wTemptedMon;
	BYTE m_bAftermath;

	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	WORD m_wDIR;

	DWORD m_dwPcBangTime;
	BYTE m_bPcBangItemCnt;
	DWORD m_dwLastDestination;

DEF_QUERY( CSPSaveChar, _T("{ ? = CALL TSaveChar(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(27)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStartAct)  
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bHelmetHide)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCooper)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGuildLeave)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildLeaveTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwEXP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwHP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwMP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkillPoint)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwRegion)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wMapID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSpawnID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wLastSpawnID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wTemptedMon)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bAftermath)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_fPosX)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_fPosY)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_fPosZ)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wDIR)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwPcBangTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bPcBangItemCnt)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwLastDestination)
END_PARAM()
};

class CSPSaveInven : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwCharID;
	BYTE m_bInvenID;
	WORD m_wItemID;
	TIMESTAMP_STRUCT m_dEndTime;
	BYTE m_bELD;

DEF_QUERY( CSPSaveInven, _T("{ ? = CALL TSaveInven(?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bInvenID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bELD)
END_PARAM()
};

class CSPLogSkill : public CSqlQuery
{
public:

	DWORD m_dwCharID;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	WORD m_wSkill;
	BYTE m_bLevel;

DEF_QUERY( CSPLogSkill, _T("{ CALL TLogSkill(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCooper)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkill)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
END_PARAM()
};

class CSPSaveItem : public CSqlQuery
{
public:
	int m_nRET;

	BYTE m_bStorageType;
	DWORD m_dwStorageID;
	BYTE m_bOwnerType;
	DWORD m_dwOwnerID;
	TITEMQUERY m_item;

DEF_QUERY( CSPSaveItem, _T("{ ? = CALL TSaveItem(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(34)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStorageType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwStorageID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPSaveCabinet : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwCharID;
	BYTE m_bCabinetID;
	BYTE m_bUse;

DEF_QUERY( CSPSaveCabinet, _T("{ ? = CALL TSaveCabinet(?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bCabinetID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bUse)
END_PARAM()
};

class CSPSaveSkill : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwCharID;
	WORD m_wSkill;
	BYTE m_bLevel;
	DWORD m_dwTick;
DEF_QUERY( CSPSaveSkill, _T("{? = CALL TSaveSkill(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkill)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTick)
END_PARAM()
};

class CSPSaveSkillMaintain : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwCharID;
	WORD m_wSkillID;
	BYTE m_bLevel;
	DWORD m_dwTick;
	BYTE m_bAttackType;
	DWORD m_dwAttackID;
	BYTE m_bHostType;
	DWORD m_dwHostID;
	BYTE m_bAttackCountry;

DEF_QUERY( CSPSaveSkillMaintain, _T("{? = CALL TSaveSkillMaintain(?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(10)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkillID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTick)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bAttackType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwAttackID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bHostType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwHostID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bAttackCountry)
END_PARAM()
};

class CSPSaveQuest : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwQuestID;
	DWORD m_dwCharID;
	DWORD m_dwTick;

	BYTE m_bCompleteCount;
	BYTE m_bTriggerCount;

DEF_QUERY( CSPSaveQuest, _T("{ ? = CALL TSaveQuest(?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwQuestID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTick)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bCompleteCount)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bTriggerCount)
END_PARAM()
};

class CSPSaveQuestTerm : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwQuestID;
	DWORD m_dwCharID;
	DWORD m_dwTermID;

	BYTE m_bTermType;
	BYTE m_bCount;

DEF_QUERY( CSPSaveQuestTerm, _T("{ ? = CALL TSaveQuestTerm(?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwQuestID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTermID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bTermType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bCount)
END_PARAM()
};

class CSPSaveHotkey : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bSave;
	BYTE m_bInvenID;
	BYTE m_bType[MAX_HOTKEY_POS];
	WORD m_wID[MAX_HOTKEY_POS];

DEF_QUERY( CSPSaveHotkey, _T("{ CALL TSaveHotkey(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(27)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSave)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bInvenID)
	for(BYTE i=0; i<MAX_HOTKEY_POS; i++)
	{
		PARAM_ENTRY( SQL_PARAM_INPUT, m_bType[i])
		PARAM_ENTRY( SQL_PARAM_INPUT, m_wID[i])
	}
END_PARAM()
};

class CSPSaveCastleOccupy : public CSqlQuery
{
public:
	int m_nRET;
	WORD m_wCastleID;
	BYTE m_bType;
	DWORD m_dwGuildID;
	BYTE m_bCountry;

DEF_QUERY(CSPSaveCastleOccupy, _T("{ ? = CALL TSaveCastleOccupy(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCountry)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wCastleID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bType)
END_PARAM()
};

class CSPSaveLocalOccupy : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwOccupiedGuild;  
	DWORD m_dwOccupiedChar;
	WORD m_wLocalID;
	BYTE m_bType;
	BYTE m_bCountry;	
	DWORD m_dwGuild;

	DEF_QUERY(CSPSaveLocalOccupy, _T("{ ? = CALL TSaveLocalOccupy(?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wLocalID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOccupiedGuild)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOccupiedChar)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCountry)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwGuild)
END_PARAM()
};

class CSPSaveItemUsed : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wDelayGroupID;
	DWORD m_dwTick;

	DEF_QUERY(CSPSaveItemUsed, _T("{CALL TSaveItemUsed(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wDelayGroupID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTick)
END_PARAM()
};

class CSPPostCanSend : public CSqlQuery
{
public:
	int m_nRet;
	TCHAR m_szName[MAX_NAME+1];
	TCHAR m_szTarget[MAX_NAME+1];
	DWORD m_dwCharID;
	BYTE m_bType;

DEF_QUERY(CSPPostCanSend, _T("{ ? = CALL TPostCanSend(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTarget)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
END_PARAM()
};

class CSPPostDelete : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	DWORD m_dwPostID;

DEF_QUERY(CSPPostDelete, _T("{ ? = CALL TPostDelete(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPostID)
END_PARAM()
};

class CSPPostGetItem : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwPostID;

DEF_QUERY(CSPPostGetItem, _T("{ CALL TPostGetItem(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPostID)
END_PARAM()
};

class CSPSavePost : public CSqlQuery
{
public:
	int m_nRet;
	DWORD m_dwMakeID;
	DWORD m_dwCharID;
	DWORD m_dwRecvID;
	DWORD m_dwSendID;
	TCHAR m_szTarget[MAX_NAME + 1];
	TCHAR m_szSender[MAX_NAME + 1];
	TCHAR m_szTitle[MAX_BOARD_TITLE + 1];
	TCHAR m_szMessage[MAX_BOARD_TEXT + 1];
	BYTE m_bRead;
	BYTE m_bType;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	TIMESTAMP_STRUCT m_timeRecv;

DEF_QUERY(CSPSavePost, _T("{ ? = CALL TSavePost(?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(15)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwMakeID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwRecvID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSendID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTarget)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szSender)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szMessage)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bRead)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCooper)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeRecv)
END_PARAM()
};

class CSPSaveItemDirect : public CSqlQuery
{
public:
	BYTE m_bStorageType;
	DWORD m_dwStorageID;
	BYTE m_bOwnerType;
	DWORD m_dwOwnerID;
	TITEMQUERY m_item;

DEF_QUERY( CSPSaveItemDirect, _T("{ CALL TSaveItemDirect(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(33)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStorageType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwStorageID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPPostRead : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwPostID;
	BYTE m_bRead;

DEF_QUERY(CSPPostRead, _T("{ CALL TPostRead(?,?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPostID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bRead)
END_PARAM()
};

class CSPRecallmonDel : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwMonID;

DEF_QUERY(CSPRecallmonDel, _T("{ CALL TRecallMonDel(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwMonID)
END_PARAM()
};

class CSPClearCurrentUser : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPClearCurrentUser, _T("{ CALL TClearCurrentUser(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPProtectedInsert : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwCharID;
    DWORD m_dwProtected;	
	TCHAR m_szName[MAX_NAME+1];
	BYTE m_bOption;

DEF_QUERY(CSPProtectedInsert, _T("{? = CALL TProtectedInsert(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwProtected)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bOption)
END_PARAM()
};
class CSPProtectedDelete : public CSqlQuery
{
public:
	DWORD m_dwCharID;
    DWORD m_dwProtected;	

DEF_QUERY(CSPProtectedDelete, _T("{CALL TProtectedDelete(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwProtected)	
END_PARAM()
};

class CSPPetDelete : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	WORD m_wPetID;

DEF_QUERY(CSPPetDelete, _T("{CALL TPetDelete(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wPetID)
END_PARAM()
};

class CSPSavePet : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wPetID;
	TCHAR m_szName[MAX_NAME+1];
	TIMESTAMP_STRUCT m_timeUse;

DEF_QUERY(CSPSavePet, _T("{CALL TSavePet(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wPetID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_timeUse)
END_PARAM()
};

class CSPSaveCharDataStart : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPSaveCharDataStart, _T("{CALL TSaveCharDataStart(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPSaveCharDataEnd : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPSaveCharDataEnd, _T("{CALL TSaveCharDataEnd(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPSaveItemDataStart : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPSaveItemDataStart, _T("{CALL TSaveItemDataStart(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPSaveItemDataEnd : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPSaveItemDataEnd, _T("{CALL TSaveItemDataEnd(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
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

class CSPSaveSMS : public CSqlQuery
{
public:
	int m_nRET;
	BYTE m_bWorld;
	BYTE m_bType;
	DWORD m_dwSenderID;
	DWORD m_dwGuildID;
	TCHAR m_szTarget[MAX_NAME+1];
	TCHAR m_szMsg[MAX_SMS_MSG+1];

DEF_QUERY(CSPSaveSMS, _T("{ ? = CALL TSaveSMS(?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bWorld)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSenderID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szTarget)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szMsg)
END_PARAM()
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ±æµå
class CSPGuildItemPutIn : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwGuildID;
	DWORD m_dwItemID;
	TITEMQUERY m_item;

DEF_QUERY(CSPGuildItemPutIn, _T("{ ? = CALL TGuildItemPutIn(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(31)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPGuildItemTakeOut : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwGuildID;
	DWORD m_dwItemID;
	TITEMQUERY m_item;
	
DEF_QUERY(CSPGuildItemTakeOut, _T("{ ? = CALL TGuildItemTakeOut(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(31)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwItemID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPGuildItemRollback : public CSqlQuery
{
public:
	DWORD m_dwGuildID;
	DWORD m_dwItemID;
	TITEMQUERY m_item;

DEF_QUERY(CSPGuildItemRollback, _T("{CALL TGuildItemRollback(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(30)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dlID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGuildID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wItemID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bCount)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraMax)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwDuraCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bRefineCur)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dEndTime)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bGradeEffect)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_bMagic[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_wValue[5])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[0])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[1])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[2])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[3])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[4])
	PARAM_ENTRY( SQL_PARAM_INPUT, m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPGetPcBangData : public CSqlQuery
{
public:
	DWORD m_dwUserID;
	DWORD m_dwEnterTime;
	BYTE m_bInPcBang;
	DWORD m_dwPcBangTime;
	BYTE m_bItemCnt;
	BYTE m_bLuckyNumber;

DEF_QUERY(CSPGetPcBangData, _T("{ CALL TGetPcBangData(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwEnterTime)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bInPcBang)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwPcBangTime)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bItemCnt)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bLuckyNumber)
END_PARAM()
};

class CSPEventQuarter : public CSqlQuery
{
public:
	TCHAR m_szName[MAX_NAME + 1];
	BYTE m_bDay;
	BYTE m_bHour;
	BYTE m_bMinute;

DEF_QUERY(CSPEventQuarter, _T("{ CALL TEventQuarter(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bDay)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bHour)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMinute)
END_PARAM()
};

class CSPSaveEXP : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwEXP;

DEF_QUERY(CSPSaveEXP, _T("{ CALL TSaveEXP(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwEXP)
END_PARAM()
};

class CSPSaveMoney : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;

DEF_QUERY(CSPSaveMoney, _T("{ CALL TSaveMoney(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwGold)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCooper)
END_PARAM()
};

class CSPSaveDuringItem : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wItemID;
	BYTE m_bType;
	DWORD m_dwRemainTime;
	TIMESTAMP_STRUCT m_tEndTime;

DEF_QUERY(CSPSaveDuringItem, _T("{ CALL TSaveDuringItem(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwRemainTime)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_tEndTime)
END_PARAM()
};

class CSPSaveExpItem : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	WORD m_wItemID;
	BYTE m_bType;
	DWORD m_dwRemainTime;
	TIMESTAMP_STRUCT m_tEndTime;

DEF_QUERY(CSPSaveExpItem, _T("{ CALL TSaveExpItem(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwRemainTime)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_tEndTime)
END_PARAM()
};

class CSPSaveActEnd : public CSqlQuery
{
public:
	DWORD m_dwCharID;

DEF_QUERY(CSPSaveActEnd, _T("{ CALL TSaveActEnd(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPCashCabinetBuy : public CSqlQuery
{
public:
	int	m_nRet;
	DWORD m_dwCharID;
	WORD m_wCashItemID;
	DWORD m_dwCash;
	DWORD m_dwBonus;
	TIMESTAMP_STRUCT m_dateTime;

DEF_QUERY(CSPCashCabinetBuy, _T("{ ? = CALL TCashCabinetBuy(?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet);
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dateTime)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCash)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwBonus)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wCashItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPCashItemPutin : public CSqlQuery
{
public:
	int	m_nRet;
	DWORD m_dwNewID;
	DWORD m_dwCash;
	DWORD m_dwBonus;
	DWORD m_dwCharID;
	BYTE m_bInven;
	BYTE m_bItemID;

DEF_QUERY(CSPCashItemPutin, _T("{ ? = CALL TCashItemPutin(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet);
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwNewID);
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCash);
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwBonus);
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bInven)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bItemID)
END_PARAM()
};

class CSPGetCashCabinetUseTime : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	TIMESTAMP_STRUCT m_dateTime;
	DWORD m_dwCash;
	DWORD m_dwBonus;

DEF_QUERY(CSPGetCashCabinetUseTime, _T("{ ? = CALL TGetCashCabinetUseTime(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dateTime)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwCash)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwBonus)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};

class CSPInitGenItemID : public CSqlQuery
{
public:
	BYTE m_bServerID;
	__int64 m_dlGenItemID;

DEF_QUERY(CSPInitGenItemID, _T("{ CALL TInitGenItemID(?,?)}"))
BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dlGenItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bServerID)
END_PARAM()
};

class CSPDeleteDealItem : public CSqlQuery
{
public:
	__int64 m_dlID;

DEF_QUERY(CSPDeleteDealItem, _T("{ CALL TDeleteDealItem(?)}"))
BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dlID)
END_PARAM()
};

class CSPSaveRecallMaintain : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwCharID;
	DWORD m_dwRecallID;
	WORD m_wSkillID;
	BYTE m_bLevel;
	DWORD m_dwTick;
	BYTE m_bAttackType;
	DWORD m_dwAttackID;
	BYTE m_bHostType;
	DWORD m_dwHostID;
	BYTE m_bAttackCountry;

DEF_QUERY( CSPSaveRecallMaintain, _T("{? = CALL TSaveRecallMaintain(?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(11)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwRecallID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wSkillID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwTick)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bAttackType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwAttackID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bHostType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwHostID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bAttackCountry)
END_PARAM()
};

class CSPSaveCharBase : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	BYTE m_bType;
	BYTE m_bValue;
	TCHAR m_szName[MAX_NAME+1];

DEF_QUERY( CSPSaveCharBase, _T("{ CALL TSaveCharBase(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bValue)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szName)
END_PARAM()
};

class CSPCheckDuplicateName : public CSqlQuery
{
public:
	int m_nRet;
	DWORD m_dwCharID;
	TCHAR m_szName[MAX_NAME+1];

DEF_QUERY( CSPCheckDuplicateName, _T("{ ? = CALL TCheckDuplicateName(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szName)
END_PARAM()
};

class CSPStopTheClock : public CSqlQuery
{
public:
	int m_nRet;
	DWORD m_dwCharID;
	WORD m_wShopItemID;

DEF_QUERY( CSPStopTheClock, _T("{ ? = CALL TStopTheClock(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wShopItemID)
END_PARAM()
};

class CSPHeroSelect : public CSqlQuery  
{
public:
	int m_nRET;
	DWORD m_dwCharID;
	DWORD m_dwGuildID;
	WORD m_wBattleZoneID;
	TIMESTAMP_STRUCT m_timeHero;
	TCHAR m_szHero[MAX_NAME + 1];
	
DEF_QUERY(CSPHeroSelect, _T( "{ ? = CALL THeroSelect(?,?,?,?,?) }" ))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwGuildID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wBattleZoneID)	
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szHero)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_timeHero)
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

class CSPGetNation : public CSqlQuery
{
public:
	BYTE m_bNation;

DEF_QUERY(CSPGetNation, _T("{CALL TGetNation(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bNation)
END_PARAM()
};

class CSPQuestSendPost : public CSqlQuery
{
public:
	int   m_nRET;
	DWORD m_dwQuestID;
	DWORD m_dwCharID;	
	TITEMQUERY m_item;

DEF_QUERY(CSPQuestSendPost, _T("{ ? = CALL TQuestSendPost(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?) }")) 

BEGIN_PARAM(30)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwQuestID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)		
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wItemID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bCount)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bGLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwDuraMax)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwDuraCur)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bRefineCur)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dEndTime)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bGradeEffect)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[0])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[1])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[2])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[3])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[4])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_bMagic[5])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[0])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[1])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[2])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[3])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[4])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_wValue[5])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[0])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[1])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[2])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[3])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[4])
	PARAM_ENTRY(SQL_PARAM_INPUT,m_item.m_dwExtValue[5])
END_PARAM()
};

class CSPGetPvPRecord : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwUseablePoint;
	DWORD m_dwTotalPoint;
	DWORD m_dwRankOrder;
	BYTE m_bRankPercent;

	DWORD m_aRecord[TCLASS_COUNT][2];

DEF_QUERY(CSPGetPvPRecord, _T("{CALL TGetPvPRecord(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(17)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwUseablePoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwRankOrder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bRankPercent)
	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_aRecord[cr][0])
		PARAM_ENTRY(SQL_PARAM_OUTPUT, m_aRecord[cr][1])
	}
END_PARAM()
};

class CSPSavePvPRecord : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwUseablePoint;
	DWORD m_dwTotalPoint;

	DWORD m_aRecord[TCLASS_COUNT][2];

DEF_QUERY(CSPSavePvPRecord, _T("{CALL TSavePvPRecord(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(15)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUseablePoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwTotalPoint)
	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		PARAM_ENTRY(SQL_PARAM_INPUT, m_aRecord[cr][0])
		PARAM_ENTRY(SQL_PARAM_INPUT, m_aRecord[cr][1])
	}
END_PARAM()
};

class CSPSavePvPRecent : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	TCHAR	m_szName[MAX_NAME + 1];
	BYTE	m_bClass;
	BYTE	m_bLevel;
	BYTE	m_bWin;
	DWORD	m_dwPoint;
	TIMESTAMP_STRUCT m_dlDate;

DEF_QUERY(CSPSavePvPRecent, _T("{CALL TSavePvPRecent(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bClass)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bWin)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dlDate)
END_PARAM()
};

class CSPDuelScore : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwCharID;	
	DWORD	m_aDuelScore[TCLASS_COUNT][2];

DEF_QUERY(CSPDuelScore, _T("{? = CALL TDuelScore(?,?,?,?,?,?,?,?,?,?,?,?,?)}"))
BEGIN_PARAM(15)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)	
	for(BYTE i = 0; i < TCLASS_COUNT; i++)
	{
		PARAM_ENTRY(SQL_PARAM_INPUT,m_aDuelScore[i][0])
		PARAM_ENTRY(SQL_PARAM_INPUT,m_aDuelScore[i][1])
	}
END_PARAM()
};

class CSPDuelCharAdd : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwCharID;
	TCHAR	m_szName[MAX_NAME + 1];
	BYTE	m_bClass;
	BYTE	m_bLevel;
	BYTE	m_bWin;
	DWORD	m_dwPoint;
	TIMESTAMP_STRUCT	m_timeDuel;

DEF_QUERY(CSPDuelCharAdd, _T("{? = CALL TDuelCharAdd(?,?,?,?,?,?,?)}"))
BEGIN_PARAM(8)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bClass)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bLevel)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bWin)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_timeDuel)
END_PARAM()
};

class CSPSaveProtectedOption : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwTarget;
	BYTE m_bOption;

DEF_QUERY(CSPSaveProtectedOption, _T("{CALL TSaveProtectedOption(?,?,?)}"))
BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwTarget)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bOption)
END_PARAM()
};


class CSPAuctionReg : public CSqlQuery
{
public:
	int		m_nRET;
	WORD	m_wNpcID;
	WORD	m_wHour;
	DWORD	m_dwSeller;
	__int64	m_dlDirectPrice;
	__int64	m_dlStartPrice;	
	__int64 m_dlID;
	DWORD	m_dwAuctionID;	
	TIMESTAMP_STRUCT m_DateStart;
	TIMESTAMP_STRUCT m_DateEnd;	
	
DEF_QUERY(CSPAuctionReg, _T("{ ? = CALL TAuctionReg(?,?,?,?,?,?,?,?,?)}"))
BEGIN_PARAM(10)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wNpcID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wHour)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwSeller)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dlDirectPrice)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dlStartPrice)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dlID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwAuctionID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_DateStart)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_DateEnd)	
	
END_PARAM()	
};

class CSPAuctionRegCancel : public CSqlQuery
{
public:
	int		m_nRET;
	WORD	m_wNpcID;
	DWORD	m_dwAuctionID;
	DWORD	m_dwBidder;
	__int64 m_ldwBidPrice;	
	TCHAR	m_szSeller[MAX_NAME+1];
	TCHAR	m_szBidder[MAX_NAME+1];	

DEF_QUERY(CSPAuctionRegCancel, _T("{ ? = CALL TAuctionRegCancel(?,?,?,?,?,?) }"))
BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wNpcID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwAuctionID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSeller)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szBidder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwBidder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_ldwBidPrice)	
END_PARAM()
};

class CSPAuctionBid : public CSqlQuery
{
public:
	int		m_nRET;	
	WORD	m_wNpcID;
	DWORD	m_dwAuctionID;
	DWORD	m_dwCharID;
	__int64	m_ldwBidPrice;
	DWORD	m_dwDropBidder;
	__int64 m_ldwDropBidPrice;	
	TCHAR	m_szDropBidder[MAX_NAME+1];
	TIMESTAMP_STRUCT m_dBid;

DEF_QUERY(CSPAuctionBid, _T("{ ? = CALL TAuctionBid(?,?,?,?,?,?,?,?)}"))
BEGIN_PARAM(9)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wNpcID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwAuctionID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_ldwBidPrice)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_ldwDropBidPrice)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwDropBidder)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szDropBidder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dBid)
END_PARAM()

};

class CSPAuctionEnd : public CSqlQuery
{
public:
	int		m_nRET;
	WORD	m_wNpcID;
	DWORD	m_dwAuctionID;	
	DWORD	m_dwBuyer;
	DWORD	m_dwSeller;
	TCHAR	m_szBuyer[MAX_NAME+1];
	TCHAR	m_szSeller[MAX_NAME+1];
	__int64	m_ldwPrice;
	

DEF_QUERY(CSPAuctionEnd, _T("{ ? = CALL TAuctionEnd(?,?,?,?,?,?,?)}"))
BEGIN_PARAM(8)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wNpcID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwAuctionID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwBuyer)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwSeller)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szBuyer)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSeller)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_ldwPrice)	
END_PARAM()
};

class CSPAuctionBuyDirect : public CSqlQuery
{
public:
	int		m_nRET;	
	WORD	m_wNpcID;
	DWORD	m_dwAuctionID;
	DWORD	m_dwCharID;
	BYTE	m_bBuyCount;
	__int64	m_ldwBidPrice;
	DWORD	m_dwSeller;
	TCHAR	m_szSeller[MAX_NAME+1];
	DWORD	m_dwDropBidder;
	TCHAR	m_szDropBidder[MAX_NAME +1];
	__int64 m_ldwDropBidPrice;


DEF_QUERY(CSPAuctionBuyDirect, _T("{ ? = CALL TAuctionBuyDirect(?,?,?,?,?,?,?,?,?,?)}"))
BEGIN_PARAM(11)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wNpcID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwAuctionID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bBuyCount)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_ldwBidPrice)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwSeller)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSeller)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwDropBidder)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szDropBidder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_ldwDropBidPrice)	
END_PARAM()
};

class CSPSaveAuctionInterest : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	DWORD m_dwAuctionID;
	
DEF_QUERY(CSPSaveAuctionInterest, _T("{CALL TSaveAuctionInterest(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwAuctionID)
END_PARAM()
};

class CSPGainCashBonus : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	BYTE m_bInven;
	BYTE m_bItem;
	DWORD m_dwBonus;
	
DEF_QUERY(CSPGainCashBonus, _T("{ ? = CALL TGainCashBonus(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRet)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bInven)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bItem)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwBonus)
END_PARAM()
};
 
class CSPGetMonthPvPoint : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwMonthPvPoint;
	WORD	m_wMonthWin;
	WORD	m_wMonthLose;
	DWORD	m_dwMonthRankOrder;
	BYTE	m_bMonthRankPercent;

DEF_QUERY(CSPGetMonthPvPoint, _T("{CALL TGetMonthPvPoint(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwMonthPvPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wMonthWin)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wMonthLose)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwMonthRankOrder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bMonthRankPercent)
END_PARAM()
};

class CSPSaveMonthPvPoint : public CSqlQuery
{
public:
	DWORD	m_dwCharID;
	DWORD	m_dwPoint;
	WORD	m_wWin;
	WORD	m_wLose;
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];
	BYTE	m_bCountry;

DEF_QUERY(CSPSaveMonthPvPoint,_T("{CALL TSaveMonthPvPoint(?,?,?,?,?,?)}"))

BEGIN_PARAM(6)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwPoint)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wWin)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wLose)	
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szSay)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bCountry)
END_PARAM()
};

class CSPSaveWarlordSay : public CSqlQuery
{
public:
	BYTE	m_nRET;
	BYTE	m_bType;
	BYTE	m_bRankMonth;
	DWORD	m_dwCharID;
	TCHAR	m_szSay[MAX_BOARD_TITLE +1];

DEF_QUERY(CSPSaveWarlordSay,_T("{? = CALL TSaveWarlordSay(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bRankMonth)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szSay)
END_PARAM()	
};

class CSPTournamentReward : public CSqlQuery
{
public:
	WORD m_wTournamentID;
	BYTE m_bEntryID;
	DWORD m_dwChiefID;
	DWORD m_dwWinner;
	BYTE m_bIsEquipShield;
	BYTE m_bMonth;
	BYTE m_bType;
	BYTE m_bOrder;
	DWORD m_dwMonthRank;
	DWORD m_dwTotalRank;
	DWORD m_dwMonthPoint;
	DWORD m_dwTotalPoint;
	WORD m_wMonthWin;
	WORD m_wMonthLose;
	DWORD m_dwTotalWin;
	DWORD m_dwTotalLose;
	BYTE m_bCountry;
	BYTE m_bLevel;
	BYTE m_bClass;
	BYTE m_bRace;
	BYTE m_bSex;
	BYTE m_bFace;
	BYTE m_bHair;
	TCHAR m_szName[MAX_NAME + 1];
	TCHAR m_szGuild[MAX_NAME + 1];

DEF_QUERY(CSPTournamentReward,_T("{CALL TTournamentReward(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(20)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bType)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bOrder)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwMonthRank)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwTotalRank)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwMonthPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwTotalPoint)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wMonthWin)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wMonthLose)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwTotalWin)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwTotalLose)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bCountry)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bLevel)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bClass)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bRace)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bSex)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bFace)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bHair)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szGuild)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bMonth)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wTournamentID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bEntryID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwChiefID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwWinner)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bIsEquipShield)
END_PARAM()
};

class CSPGetPostInfo : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	WORD m_wPage;
	WORD m_wTotalCount;
	WORD m_wNotReadCount;
	DWORD m_dwBeginID;

DEF_QUERY(CSPGetPostInfo, _T("{CALL TGetPostInfo(?,?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wTotalCount)	
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_wNotReadCount)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwBeginID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wPage)
END_PARAM()	

};

class CSPPostView : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	DWORD m_dwPostID;
	DWORD m_dwSendID;
	BYTE m_bItemCount;
	BYTE m_bType;
	BYTE m_bRead;
	DWORD m_dwGold;
	DWORD m_dwSilver;
	DWORD m_dwCooper;
	TIMESTAMP_STRUCT m_timeRecv;
	TCHAR m_szSender[MAX_NAME + 1];
	TCHAR m_szTitle[MAX_BOARD_TITLE + 1];
	TCHAR m_szMessage[MAX_BOARD_TEXT + 1];

DEF_QUERY(CSPPostView, _T("{? = CALL TPostView(?,?,?,?,?,?,?,?,?,?,?,?,?)}"));

BEGIN_PARAM(14)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_bItemCount)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwSendID)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bType)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_bRead)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwGold)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwSilver)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_dwCooper)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_timeRecv)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSender)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szTitle)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szMessage)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPostID)
END_PARAM()
};

class CSPPostBillsUpdate : public CSqlQuery
{
public:
	INT m_nRet;
	TCHAR m_szCharName[MAX_NAME + 1];
	DWORD m_dwPostID;
	DWORD m_dwNewID;
	BYTE m_bType;
	TCHAR m_szSender[MAX_NAME+1];
	TCHAR m_szRecvName[MAX_NAME +1];
	TCHAR m_szTitle[MAX_BOARD_TITLE+1];

DEF_QUERY(CSPPostBillsUpdate, _T("{? = CALL TPostBillsUpdate(?,?,?,?,?,?,?)}"));

BEGIN_PARAM(8)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwNewID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szSender)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szRecvName)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT,m_szTitle)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwPostID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szCharName)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bType)
END_PARAM()
};

class CSPSaveCharPosition : public CSqlQuery
{
public:
	INT m_nRet;
	DWORD m_dwCharID;
	WORD m_wMapID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	WORD m_wDir;

DEF_QUERY(CSPSaveCharPosition, _T("{? = CALL TSaveCharPosition(?,?,?,?,?,?)}"));

BEGIN_PARAM(7)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRet)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wMapID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_fPosX)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_fPosY)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_fPosZ)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wDir)
END_PARAM()
};

class CSPSaveMissionOccupy : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwOccupiedChar;
	WORD m_wLocalID;
	BYTE m_bType;
	BYTE m_bCountry;

DEF_QUERY(CSPSaveMissionOccupy, _T("{ ? = CALL TSaveMissionOccupy(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wLocalID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOccupiedChar)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bCountry)
END_PARAM()
};

class CSPMonthPvPointClear : public CSqlQuery
{
public:
	DEF_QUERY(CSPMonthPvPointClear, _T("{ CALL TMonthPvPointClear }"))
};

class CSPTCMGiftLog : public CSqlQuery
{
public:
	DWORD	m_dwUserID;
	DWORD	m_dwCharID;
	WORD	m_wGiftID;
	DWORD	m_dwGMCharID;
	WORD	m_wErrID;
public:
	DEF_QUERY(CSPTCMGiftLog, _T("{ CALL TCMGiftLog(?,?,?,?,?) }"))
		
BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wGiftID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwGMCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_wErrID)
END_PARAM()
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT_INIT_QUERY(TMapSvr)
