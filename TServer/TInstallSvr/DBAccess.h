#pragma once

#define		MAX_FILENAME			50
#define		MAX_PATHNAME			100
#define		MAX_CHECKSUM			32

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

//	IP, 세그먼트 등록
class CSPRegistSegment : public CSqlQuery
{
public:
	INT64	m_dwIPaddr;
	DWORD	m_dwFileID;
	WORD	m_wSegment;
	WORD	m_wContry;

DEF_QUERY(CSPRegistSegment, _T("{CALL TRegistSegment(?,?,?,?)}"))

BEGIN_PARAM(4)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwIPaddr)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFileID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wSegment)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wContry)
END_PARAM()
};

// IP 제거
class CSPTDeleteIP : public CSqlQuery
{
public:
	INT64	m_dwIPaddr;

DEF_QUERY(CSPTDeleteIP, _T("{CALL TDeleteInstallIP(?)}"))

BEGIN_PARAM(1)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwIPaddr)
END_PARAM()
};


// IP 국가검색
class CSPTGetContry : public CSqlQuery
{
public:
	INT64	m_dwIPaddr;
	WORD	m_wContry;

DEF_QUERY(CSPTGetContry, _T("{CALL TGetContryIP(?,?)}"))

BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwIPaddr)
	PARAM_ENTRY(SQL_PARAM_OUTPUT, m_wContry)
END_PARAM()
};


//	IPaddr 얻기 (Check)
class CTBLCheckaddr : public CSqlQuery
{
public:
	INT64	m_dwIPaddr;

DEF_QUERY(CTBLCheckaddr, _T("SELECT DISTINCT dwIPaddr FROM TINSTALLIPTABLE"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwIPaddr)
END_COL()

};

//	Segment IPaddr 얻기
class CTBLInstallIPaddr : public CSqlQuery
{
public:
	INT64	m_dwIPaddr;
	DWORD	m_dwFileID;
	WORD	m_wSegment;
	WORD	m_wContry;

DEF_QUERY(CTBLInstallIPaddr, _T("SELECT dwIPaddr FROM TINSTALLIPTABLE WHERE wContry = ? AND dwFileID = ? AND wSegment = ?"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY(m_dwIPaddr)
END_COL()

BEGIN_PARAM(3)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wContry)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFileID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wSegment)
END_PARAM()
};

//	인스톨 버전
class CTBLInstallVer : public CSqlQuery
{
public:
	DWORD	m_dwVersion;
	DWORD	m_dwSegmentSize;

DEF_QUERY(CTBLInstallVer, _T("SELECT dwVersion, dwSegmentSize FROM TINSTALLVERSION"))

BEGIN_COLUMN(2)
	COLUMN_ENTRY(m_dwVersion)
	COLUMN_ENTRY(m_dwSegmentSize)
END_COL()
};

//	세그먼트 CheckSum얻기
class CTBLSegmentCheck : public CSqlQuery
{
public:
	DWORD	m_dwFileID;
	WORD	m_wSegment;
	TCHAR	m_szCheckSum[MAX_CHECKSUM+1];

DEF_QUERY(CTBLSegmentCheck, _T("SELECT szCheck FROM TINSTALLSEGMENTTABLE WHERE dwFileID = ? AND wSegmentID = ?"))

BEGIN_COLUMN(1)
	COLUMN_ENTRY_STR(m_szCheckSum)
END_COL()
BEGIN_PARAM(2)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_dwFileID)
	PARAM_ENTRY(SQL_PARAM_INPUT, m_wSegment)
END_PARAM()
};

//	Install 파일 목록얻기
class CTBLInstallFile : public CSqlQuery
{
public:
	DWORD	m_dwVersion;
	DWORD	m_dwSize;
	TCHAR	m_szName[MAX_FILENAME+1];
	TCHAR	m_szCheckSum[MAX_CHECKSUM+1];

DEF_QUERY(CTBLInstallFile, _T("SELECT dwFileID, szName, dwSize, szCheck FROM TINSTALLFILETABLE"))

BEGIN_COLUMN(4)
	COLUMN_ENTRY(m_dwVersion)
	COLUMN_ENTRY_STR(m_szName)
	COLUMN_ENTRY(m_dwSize)
	COLUMN_ENTRY_STR(m_szCheckSum)
END_COL()
};

EXPORT_INIT_QUERY(TInstallSvr)