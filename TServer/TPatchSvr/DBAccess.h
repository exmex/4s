#pragma once

#define		MAX_IP					30
#define		MAX_FILENAME			50
#define		MAX_PATHNAME			100

class CSPLoadService : public CSqlQuery
{
public:
	BYTE	m_bWorld;
	BYTE	m_bServiceGroup;
	TCHAR	m_szIP[MAX_PATHNAME+1];
	WORD	m_wPort;

DEF_QUERY(CSPLoadService, _T("{CALL TLoadService(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bWorld)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_bServiceGroup)
	PARAM_ENTRY_STR(SQL_PARAM_OUTPUT, m_szIP)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wPort)
END_PARAM()
};

class CSPMinBetaVer : public CSqlQuery
{
public:
	DWORD m_dwMinVer;

DEF_QUERY(CSPMinBetaVer, _T("{? = CALL TMinBetaVer}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_dwMinVer)
END_PARAM()
};

class CSPPreComplete : public CSqlQuery
{
public:
	DWORD m_dwBetaVer;

DEF_QUERY(CSPPreComplete, _T("{CALL TPreCompleteAdd(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBetaVer)
END_PARAM()
};

class CTBLVersion : public CSqlQuery
{
public:
	TCHAR	m_szName[MAX_FILENAME+1];
	TCHAR	m_szPath[MAX_PATHNAME];
	DWORD	m_dwVersion;
	DWORD	m_dwBetaVer;
	DWORD	m_dwSize;

DEF_QUERY(CTBLVersion, _T("SELECT dwVersion, szPath, szName, dwSize, dwBetaVer FROM TVERSION WHERE dwVersion > ? ORDER BY dwVersion"))

BEGIN_COLUMN(5)
	COLUMN_ENTRY(m_dwVersion)
	COLUMN_ENTRY_STR(m_szPath)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwSize)
	COLUMN_ENTRY(m_dwBetaVer)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwVersion)
END_PARAM()
};

class CTBLPreVersion : public CSqlQuery
{
public:
	TCHAR	m_szName[MAX_FILENAME+1];
	TCHAR	m_szPath[MAX_PATHNAME];
	DWORD	m_dwBetaVer;
	DWORD	m_dwSize;

DEF_QUERY(CTBLPreVersion, _T("SELECT dwBetaVer, szPath, szName, dwSize FROM TPREVERSION WHERE dwBetaVer > ? ORDER BY dwBetaVer"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwBetaVer)
	COLUMN_ENTRY_STR(m_szPath)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwSize)
END_COL()

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwBetaVer)
END_PARAM()

};
EXPORT_INIT_QUERY(TPatchSvr)