#pragma once

class CTBLMachine : public CSqlQuery
{
public :
	BYTE m_bMachineID;
	TCHAR m_szName[MAX_NAME+1];
	BYTE m_bRouteID;

DEF_QUERY( CTBLMachine, _T("SELECT bMachineID, szName, bRouteID FROM TMACHINE"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY(m_bMachineID)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_bRouteID)
END_COL()
};

class CTBLNetwork : public CSqlQuery
{
public :
	BYTE m_bMachineID;
	TCHAR m_szNetwork[MAX_NAME+1];

DEF_QUERY( CTBLNetwork, _T("SELECT bMachineID, szNetwork FROM TNETWORK"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bMachineID)
	COLUMN_ENTRY_STR(m_szNetwork)
END_COL()
};

class CTBLGroup : public CSqlQuery
{
public :
	BYTE m_bGroupID;
	TCHAR m_szName[MAX_NAME+1];

DEF_QUERY( CTBLGroup, _T("SELECT bGroupID, szName FROM TGROUP"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY_STR(m_szName)
END_COL()
};

class CTBLSvrType : public CSqlQuery
{
public :
	BYTE m_bType;
	TCHAR m_szName[MAX_NAME+1];

DEF_QUERY( CTBLSvrType, _T("SELECT bType, szName FROM TSVRTYPE WHERE bControl = 1"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY_STR(m_szName)
END_COL()
};

class CTBLServer : public CSqlQuery
{
public :
	BYTE m_bGroupID;
	BYTE m_bServerID;
	BYTE m_bType;
	BYTE m_bMachineID;
	WORD m_wPort;
	TCHAR m_szName[MAX_NAME+1];

DEF_QUERY( CTBLServer, _T("SELECT bGroupID, bServerID, bType, bMachineID, wPort, szName FROM TSERVER WHERE bType <> 6"));

BEGIN_COLUMN(6)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY(m_bServerID)
	COLUMN_ENTRY(m_bType)
	COLUMN_ENTRY(m_bMachineID)
	COLUMN_ENTRY(m_wPort)
	COLUMN_ENTRY_STR(m_szName)
END_COL()
};

class CTBLIPAddr : public CSqlQuery
{
public :
	BYTE m_bMachineID;
	TCHAR m_szIPAddr[MAX_NAME+1];
	TCHAR m_szPriAddr[MAX_NAME + 1];

DEF_QUERY( CTBLIPAddr, _T("SELECT szIPAddr, szPriAddr FROM TIPADDR WHERE bMachineID = ? AND bActive = 1"));

BEGIN_COLUMN(2)
	COLUMN_ENTRY_STR(m_szIPAddr)
	COLUMN_ENTRY_STR(m_szPriAddr)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bMachineID);
END_PARAM()
};

class CTBLManager : public CSqlQuery 
{
public:
	TCHAR m_szID[MAX_NAME+1];
	TCHAR m_szPasswd[MAX_NAME+1];
	BYTE m_bAuthority; // Çö½Â·æ ¸Å´ÏÀú ±ÇÇÑ

DEF_QUERY( CTBLManager, _T("SELECT szID, szPasswd, bAuthority FROM TMANAGER"));

BEGIN_COLUMN(3)
	COLUMN_ENTRY_STR(m_szID)
	COLUMN_ENTRY_STR(m_szPasswd)
	COLUMN_ENTRY(m_bAuthority) // Çö½Â·æ ¸Å´ÏÀú ±ÇÇÑ
END_COL()
};
 
class CTBLEvent : public CSqlQuery
{
public:
	DWORD m_dwIndex;
	BYTE m_bID;
	TCHAR m_szTitle[MAX_BOARD_TITLE +1];
	BYTE m_bGroupID;
	BYTE m_bSvrType;
	BYTE m_bSvrID; // 0 : ALL 
	TIMESTAMP_STRUCT m_dStartDate;
	TIMESTAMP_STRUCT m_dEndDate;
	WORD m_wValue;
	WORD m_wMapID;
	DWORD m_dwStartAlarm;
	DWORD m_dwEndAlarm;
	BYTE m_bStartAlarm;
	BYTE m_bEndAlarm;
	BYTE m_bPartTime;
	TCHAR m_szStartMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szMidMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szEndMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szValue[MAX_BOARD_TEXT + 1];

DEF_QUERY(CTBLEvent,_T("SELECT \
    dwIndex, \
	bID, \
	szTitle, \
	bGroupID, \
	bSvrType, \
	bSvrID, \
	dStartDate, \
	dEndDate, \
	wValue, \
	wMapID, \
	dwStartAlarm, \
	dwEndAlarm, \
	bPartTime, \
	szStartMsg, \
	szMidMsg, \
	szEndMsg, \
	szValue \
	FROM TEVENTCHART"))

BEGIN_COLUMN(16)
	COLUMN_ENTRY(m_dwIndex)
	COLUMN_ENTRY(m_bID)
	COLUMN_ENTRY_STR(m_szTitle)
	COLUMN_ENTRY(m_bGroupID)
	COLUMN_ENTRY(m_bSvrType)
	COLUMN_ENTRY(m_bSvrID)
	COLUMN_ENTRY(m_dStartDate)
	COLUMN_ENTRY(m_dEndDate)
	COLUMN_ENTRY(m_wValue)
	COLUMN_ENTRY(m_wMapID)
	COLUMN_ENTRY(m_dwStartAlarm)
	COLUMN_ENTRY(m_dwEndAlarm)
	COLUMN_ENTRY(m_bPartTime)
	COLUMN_ENTRY_STR(m_szStartMsg)
	COLUMN_ENTRY_STR(m_szMidMsg)
	COLUMN_ENTRY_STR(m_szEndMsg)
	COLUMN_ENTRY_STR(m_szValue)
END_COL()
};

class CTBLCashShopItem : public CSqlQuery
{
public:
	WORD m_wID;
	TCHAR m_szName[MAX_NAME + 1];

DEF_QUERY(CTBLCashShopItem,_T("SELECT \
	wID, \
	szName \
	FROM TCASHSHOPITEMCHART \
	WHERE bCanSell = 1"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_wID)
	COLUMN_ENTRY_STR(m_szName)
END_COL()
};

class CTBLPreVersion : public CSqlQuery
{
public:
	DWORD	m_dwBetaVer;
	TCHAR	m_szPath[MAX_PATHNAME];	
	TCHAR	m_szName[MAX_FILENAME+1];
	DWORD	m_dwSize;

DEF_QUERY(CTBLPreVersion, _T("SELECT dwBetaVer, szPath, szName, dwSize FROM TPREVERSION"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwBetaVer)
	COLUMN_ENTRY_STR(m_szPath)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwSize)
END_COL()
};

///////////////////////////////////////////////////////////////////
// Çö½Â·æ CT_UPDATEPATCH_REQ
class CSPUpdatePatch : public CSqlQuery
{
public:
	int		m_nRET;
	TCHAR	m_szName[MAX_FILENAME+1];
	TCHAR	m_szPath[MAX_PATHNAME];	
	DWORD	m_dwSize;
	DWORD	m_dwBetaVer;

DEF_QUERY(CSPUpdatePatch, _T("{ ? = CALL TUpdateVersion(?,?,?,?)}"))

BEGIN_PARAM(5)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szPath)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSize)	
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBetaVer)	
END_PARAM()
};

//	CT_UPDATEPREPATCH_REQ
class CSPUpdatePrePatch : public CSqlQuery
{
public:
	int		m_nRET;
	TCHAR	m_szName[MAX_FILENAME+1];
	TCHAR	m_szPath[MAX_PATHNAME];	
	DWORD	m_dwSize;

DEF_QUERY(CSPUpdatePrePatch, _T("{ ? = CALL TUpdatePreVersion(?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szPath)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szName)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwSize)	
END_PARAM()
};

class CSPBetaToVer : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwBetaVer;

DEF_QUERY(CSPBetaToVer, _T("{ ? = CALL TBetaToVersion(?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBetaVer)	
END_PARAM()
};

class CSPDeletePreVersion : public CSqlQuery
{
public:
	int		m_nRET;
	DWORD	m_dwBetaVer;

DEF_QUERY(CSPDeletePreVersion, _T("{ ? = CALL TDeletePreVersion(?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBetaVer)	
END_PARAM()
};

class CSPUserProtectedAdd : public CSqlQuery
{
public:
	int m_nRET;
	TCHAR m_szUserID[MAX_FILENAME+1];
	DWORD m_dwDuration;

DEF_QUERY(CSPUserProtectedAdd, _T("{ ? = CALL TUserProtectedAdd(?,?)}"))

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szUserID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwDuration)
END_PARAM()
};


class CSPSvrStatusSMS : public CSqlQuery
{
public:	
	BYTE m_bSvrType;
	DWORD m_dwSvrID;
	BYTE m_bSvrStatus;
	
DEF_QUERY(CSPSvrStatusSMS, _T("{ CALL OPTool_SMSEmergency(?,?,?) }") )

BEGIN_PARAM(3)	
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bSvrType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwSvrID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bSvrStatus)
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

class CSPOPLogin : public CSqlQuery
{
public:
	int		m_nRET;
	TCHAR	m_szID[MAX_NAME+1];
	TCHAR	m_szPW[MAX_NAME+1];

DEF_QUERY(CSPOPLogin, _T( "{? = CALL TOPLogin(?,?)}") )

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_nRET)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szID)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT, m_szPW)
END_PARAM()
};
 
class CSPEventUpdate : public CSqlQuery
{
public:
	int	m_nRET;
	DWORD m_dwIndex;
	BYTE m_bID;
	BYTE m_bType;
	TCHAR m_strTitle[MAX_NAME +1];
	BYTE m_bGroupID;
	BYTE m_bSvrType;
	BYTE m_bSvrID;
	TIMESTAMP_STRUCT m_dStartDate;
	TIMESTAMP_STRUCT m_dEndDate;
	WORD m_wValue;
	WORD m_wMapID;
	DWORD m_dwStartAlarm;
	DWORD m_dwEndAlarm;
	BYTE m_bPartTime;
	TCHAR m_szStartMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szMidMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szEndMsg[MAX_BOARD_TEXT +1];
	TCHAR m_szValue[MAX_BOARD_TEXT +1];

DEF_QUERY(CSPEventUpdate,_T("{? = CALL TEventUpdate(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"))

BEGIN_PARAM(19)
	PARAM_ENTRY(SQL_PARAM_OUTPUT,m_nRET)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwIndex)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bType)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_strTitle)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bGroupID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bSvrType)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bSvrID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dStartDate)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dEndDate)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wValue)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_wMapID)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwStartAlarm)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_dwEndAlarm)
	PARAM_ENTRY(SQL_PARAM_INPUT,m_bPartTime)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szStartMsg)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szMidMsg)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szEndMsg)
	PARAM_ENTRY_STR(SQL_PARAM_INPUT,m_szValue)
END_PARAM()
};

///////////////////////////////////////////////////////////////////
EXPORT_INIT_QUERY(TControlSvr)
