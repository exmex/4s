#pragma once

class CSPLogin : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szUserID[MAX_NAME + 1];
	TCHAR m_szPasswd[MAX_NAME + 1];
	TCHAR m_szIPAddr[MAX_NAME + 1];
	TCHAR m_szLoginIP[MAX_NAME + 1];

	BYTE m_bIPCheck;

	DWORD m_dwCharID;
	DWORD m_dwID;
	DWORD m_dwKEY;

	WORD m_wPort;
	BYTE m_bCreateCnt;
	BYTE m_bInPcBang;
	DWORD m_dwPremium;

DEF_QUERY( CSPLogin, _T("{ ? = CALL TLogin(?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(13)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szUserID)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szPasswd)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szLoginIP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bIPCheck)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwKEY)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwID)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szIPAddr)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_wPort)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCreateCnt)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bInPcBang)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwPremium)
END_PARAM()
};

class CSPLoginJP : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szUserID[MAX_NAME + 1];
	TCHAR m_szPasswd[MAX_NAME + 1];
	TCHAR m_szIPAddr[MAX_NAME + 1];
	TCHAR m_szLoginIP[MAX_NAME + 1];

	BYTE m_bIPCheck;

	DWORD m_dwCharID;
	DWORD m_dwID;
	DWORD m_dwKEY;

	WORD m_wPort;
	BYTE m_bCreateCnt;
	BYTE m_bInPcBang;
	DWORD m_dwPremium;
	BYTE m_bChanneling;

DEF_QUERY( CSPLoginJP, _T("{ ? = CALL TLogin(?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(14)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szUserID)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szPasswd)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szLoginIP)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bIPCheck)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bChanneling)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwKEY)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwID)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szIPAddr)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_wPort)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCreateCnt)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bInPcBang)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwPremium)
END_PARAM()
};

class CSPCheckIP : public CSqlQuery
{
public:
	int m_nRET;
	TCHAR m_szIPAddr[MAX_NAME + 1];

DEF_QUERY(CSPCheckIP, _T("{ ? = CALL TCheckIP(?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szIPAddr)
END_PARAM()
};

class CSPLogout : public CSqlQuery
{
public:
	int m_nRET;
	DWORD m_dwID;
	DWORD m_dwCharID;
	BYTE m_bLevel;
	DWORD m_dwExp;

DEF_QUERY( CSPLogout, _T("{ ? = CALL TLogout(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bLevel)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwExp)
END_PARAM()
};


class CSPCheckPasswd : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szPasswd[MAX_NAME + 1];
	DWORD m_dwID;

DEF_QUERY( CSPCheckPasswd, _T("{ ? = CALL TCheckPasswd(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwID)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szPasswd)
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


class CTBLGroup : public CSqlQuery
{
public:
	BYTE m_bGroupID;
	BYTE m_bType;

	TCHAR m_szNAME[MAX_NAME + 1];
	TCHAR m_szDSN[MAX_NAME + 1];
	TCHAR m_szDBUser[MAX_NAME + 1];
	TCHAR m_szPasswd[MAX_NAME + 1];

DEF_QUERY( CTBLGroup, _T("SELECT \
	bGroupID, \
	bType, \
	szNAME, \
	szDSN, \
	szUserID, \
	szPasswd \
	FROM TGROUP WHERE bGroupID <> 0"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY_STR(m_szNAME)
	COLUMN_ENTRY_STR(m_szDSN)
	COLUMN_ENTRY_STR(m_szDBUser)
	COLUMN_ENTRY_STR(m_szPasswd)
END_COL()
};


class CTBLChannel : public CSqlQuery
{
public:
	TCHAR m_szNAME[MAX_NAME + 1];
	DWORD m_dwCount;

	BYTE m_bGroupID;
	BYTE m_bChannel;
	BYTE m_bStatus;

	WORD m_wFull;
	WORD m_wBusy;

DEF_QUERY( CTBLChannel, _T("SELECT \
	TCHANNEL.bChannel, \
	TCHANNEL.szNAME, \
	TCHANNEL.bStatus, \
	TCHANNEL.wFull, \
	TCHANNEL.wBusy, \
	COUNT(TCURRENTUSER.dwKEY) FROM TCHANNEL \
	LEFT OUTER JOIN TCURRENTUSER ON \
	TCHANNEL.bGroupID = TCURRENTUSER.bGroupID AND \
	TCHANNEL.bChannel = TCURRENTUSER.bChannel \
	WHERE TCHANNEL.bStatus <> 0 AND TCHANNEL.bGroupID = ? \
	GROUP BY \
	TCHANNEL.bChannel, \
	TCHANNEL.szNAME, \
	TCHANNEL.bStatus, \
	TCHANNEL.wFull, \
	TCHANNEL.wBusy"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bChannel)
	COLUMN_ENTRY_STR(m_szNAME)
	COLUMN_ENTRY(m_bStatus)
	COLUMN_ENTRY(m_wFull)
	COLUMN_ENTRY(m_wBusy)
	COLUMN_ENTRY(m_dwCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGroupID)
END_PARAM()
};

class CTBLGroupList : public CSqlQuery
{
public:
	TCHAR m_szNAME[MAX_NAME + 1];
	DWORD m_dwCount;
	DWORD m_dwUserID;

	BYTE m_bGroupID;
	BYTE m_bStatus;
	BYTE m_bType;
	BYTE m_bCount;

	WORD m_wFull;
	WORD m_wBusy;
	DWORD m_dwMaxUser;

DEF_QUERY( CTBLGroupList, _T("SELECT \
	TGROUP.bGroupID, \
	TGROUP.bType, \
	TGROUP.szNAME, \
	TGROUP.bStatus, \
	TGROUP.wFull, \
	TGROUP.wBusy, \
	TGROUP.dwMaxUser, \
	COUNT(DISTINCT TCURRENTUSER.dwKEY), \
	COUNT(DISTINCT TALLCHARTABLE.dwCharID) \
	FROM TGROUP LEFT OUTER JOIN TCURRENTUSER ON \
	TGROUP.bGroupID = TCURRENTUSER.bGroupID \
	LEFT OUTER JOIN TALLCHARTABLE ON \
	TGROUP.bGroupID = TALLCHARTABLE.bWorldID AND \
	TALLCHARTABLE.dwUserID = ? AND TALLCHARTABLE.bDelete = 0 \
	WHERE TGROUP.bStatus <> 0 \
	GROUP BY TGROUP.bGroupID, \
	TGROUP.bType, \
	TGROUP.szNAME, \
	TGROUP.bStatus, \
	TGROUP.wFull, \
	TGROUP.wBusy, \
	TGROUP.dwMaxUser, \
	TALLCHARTABLE.dwUserID"))

BEGIN_COLUMN(9)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY_STR(m_szNAME)
	COLUMN_ENTRY(m_bStatus)
	COLUMN_ENTRY(m_wFull)
	COLUMN_ENTRY(m_wBusy)
	COLUMN_ENTRY(m_dwMaxUser)
	COLUMN_ENTRY(m_dwCount)
	COLUMN_ENTRY(m_bCount)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};

class CTBLUserCount : public CSqlQuery
{
public:
	BYTE m_bGroup;
	DWORD m_dwCount;

DEF_QUERY( CTBLUserCount, _T("SELECT \
	bWorldID, \
	COUNT(DISTINCT dwUserID) \
	FROM TALLCHARTABLE \
	GROUP BY bWorldID"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bGroup)
	COLUMN_ENTRY(m_dwCount)
END_COL()
};

// Çö½Â·æ CS_TESTLOGIN_REQ 
class CSPTestLogin : public CSqlQuery
{
public:	
	int m_nRET;

	TCHAR m_szUserID[MAX_NAME + 1];
	TCHAR m_szPasswd[MAX_NAME + 1];

	TCHAR m_szIPAddr[MAX_NAME + 1];

	DWORD m_dwCharID;
	DWORD m_dwID;
	DWORD m_dwKEY;

	WORD m_wPort;

DEF_QUERY( CSPTestLogin, _T("{ ? = CALL TTestLogin(?,?,?,?,?,?,?)}"))

BEGIN_PARAM(8)	
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szUserID)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szPasswd)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwKEY)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwID)
	PARAM_ENTRY_STR( SQL_PARAM_OUTPUT, m_szIPAddr)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_wPort)
END_PARAM()
};

EXPORT_INIT_QUERY(TLoginSvr)


class CSPCreateChar : public CSqlQuery
{
public:
	int m_nRET;

	TCHAR m_szNAME[MAX_NAME + 1];
	DWORD m_dwCharID;
	DWORD m_dwUserID;

	BYTE m_bGroup;
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
	BYTE m_bSlot;
	BYTE m_bCreateCnt;

DEF_QUERY( CSPCreateChar, _T("{ ? = CALL TCreateChar(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(17)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCreateCnt)
	PARAM_ENTRY_STR( SQL_PARAM_INPUT, m_szNAME)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGroup)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSlot)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bClass)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bRace)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bCountry)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bSex)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bHair)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bFace)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bBody)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bPants)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bHand)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bFoot)
END_PARAM()
};


class CSPDeleteChar : public CSqlQuery
{
public:
	int m_nRET;
	BYTE m_bGroup;
	DWORD m_dwUserID;
	DWORD m_dwCharID;
	BYTE m_bCreateCnt;

DEF_QUERY( CSPDeleteChar, _T("{ ? = CALL TDeleteChar(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bCreateCnt)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bGroup)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
END_PARAM()
};


class CSPFindServerID : public CSqlQuery
{
public:
	int m_nRET;

	DWORD m_dwCharID;

	BYTE m_bServerID;
	BYTE m_bChannel;

DEF_QUERY( CSPFindServerID, _T("{ ? = CALL TFindServerID(?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bChannel)
	PARAM_ENTRY( SQL_PARAM_OUTPUT, m_bServerID)
END_PARAM()
};

class CTBLChar : public CSqlQuery
{
public:
	TCHAR m_szNAME[MAX_NAME + 1];

	DWORD m_dwUserID;
	DWORD m_dwCharID;
	DWORD m_dwRegion;

	BYTE m_bStartAct;
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
	BYTE m_bSlot;
	BYTE m_bLevel;

DEF_QUERY( CTBLChar, _T("SELECT TOP 6 \
	szNAME, \
	dwCharID, \
	bStartAct, \
	bClass, \
	bRace, \
	bCountry, \
	bSex, \
	bHair, \
	bFace, \
	bBody, \
	bPants, \
	bHand, \
	bFoot, \
	bSlot, \
	bLevel, \
	dwRegion \
	FROM TCHARTABLE \
	WHERE dwUserID = ? AND bDelete = 0 ORDER BY dLogoutDate DESC"))

BEGIN_COLUMN(16)
	COLUMN_ENTRY_STR(m_szNAME)
	COLUMN_ENTRY(m_dwCharID)
	COLUMN_ENTRY(m_bStartAct)
	COLUMN_ENTRY(m_bClass)
	COLUMN_ENTRY(m_bRace)
	COLUMN_ENTRY(m_bCountry)
	COLUMN_ENTRY(m_bSex)
	COLUMN_ENTRY(m_bHair)
	COLUMN_ENTRY(m_bFace)
	COLUMN_ENTRY(m_bBody)
	COLUMN_ENTRY(m_bPants)
	COLUMN_ENTRY(m_bHand)
	COLUMN_ENTRY(m_bFoot)
	COLUMN_ENTRY(m_bSlot)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_dwRegion)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};

class CTBLItem : public CSqlQuery
{
public:
	BYTE m_bStorageType;
	DWORD m_dwStorageID;
	BYTE m_bOwnerType;
	DWORD m_dwOwnerID;
	BYTE m_bInvenID;
	BYTE m_bItemID;

	WORD m_wItemID;
	BYTE m_bLevel;
	BYTE m_bGradeEffect;
	
	DWORD m_dwColor;
	DWORD m_dwRegGuild;

DEF_QUERY( CTBLItem, _T("SELECT \
	bItemID, \
	wItemID, \
	bLevel, \
	bGradeEffect, \
	dwTime3, \
	dwTime4 \
	FROM TITEMTABLE \
	WHERE dwOwnerID =? AND bOwnerType=? AND bStorageType=? AND dwStorageID = ?"))

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bItemID)
	COLUMN_ENTRY(m_wItemID)
	COLUMN_ENTRY(m_bLevel)
	COLUMN_ENTRY(m_bGradeEffect)
	COLUMN_ENTRY(m_dwColor)
	COLUMN_ENTRY(m_dwRegGuild)
END_COL()

BEGIN_PARAM(4)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwOwnerID)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bOwnerType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_bStorageType)
	PARAM_ENTRY( SQL_PARAM_INPUT, m_dwStorageID)
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

class CSPAgreement : public CSqlQuery
{
public:
	DWORD m_dwUserID;

	DEF_QUERY(CSPAgreement, _T("{CALL TAgreement(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwUserID)
END_PARAM()
};

class CSPClearLoginUser : public CSqlQuery
{
public:
	DEF_QUERY(CSPClearLoginUser, _T("{CALL TClearLoginCurrentUser}"))
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

class CSPGetGuildInfo : public CSqlQuery
{
public:
	DWORD m_dwCharID;
	TCHAR m_szName[MAX_NAME + 1];
	DWORD m_dwFame;
	DWORD m_dwFameColor;

	DEF_QUERY(CSPGetGuildInfo, _T("{CALL TGetGuildInfo(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwCharID)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwFame)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwFameColor)
END_PARAM()
};

EXPORT_INIT_QUERY(TGameSvr)
