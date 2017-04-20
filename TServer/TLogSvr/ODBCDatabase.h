// Module Made By Carlos Antollini 
//        Modified By Jason
//				- Add some GetFiledXXX() functions
//				- Add some Diagnostic Error Message
//				- Add BLOB
//				- Fix Minor things

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <time.h>

#define MAX_BUFFER			256
#define MAX_COL_NAME_LEN	32

class CODBCDatabase
{
public:
	enum drvCompletion
	{
		sqlNoPrompt = SQL_DRIVER_NOPROMPT,
		sqlPrompt   = SQL_DRIVER_PROMPT
	};
	
	CODBCDatabase()
	{
		m_hDbc					= NULL;
		m_hEnv					= NULL;
		m_lConnectionTimeout	= 0;
		m_lLoginTimeout			= 5;
		m_bIsConnected			= FALSE;
	
		memset(m_Error, 0x00, sizeof(m_Error));

		SQLAlloc();
	};

	~CODBCDatabase()
	{
		Close();
		m_lConnectionTimeout= 0;
		m_lLoginTimeout		= 0;
		m_bIsConnected		= FALSE;
	};

	void Init();
	BOOL Connect(TCHAR* lpstrDSN, TCHAR* lpstrUser = NULL, TCHAR* lpstrPass = NULL);
	BOOL DriverConnect(TCHAR* szConnStr, TCHAR* szConnStrOut = NULL, HWND hWnd = NULL, enum drvCompletion drvConn = sqlNoPrompt);
	BOOL Browse(TCHAR* lpstrConnStrIn, TCHAR* lpstrConnStrOut);
	void Close();

	void SetReadOnly(BOOL bReadOnly = TRUE);
	void SetConnectionTimeout(LONG nSeconds);
	LONG GetConnectionTimeout();
	void SetLoginTimeout(LONG nSeconds);
	LONG GetLoginTimeout();
	
	BOOL	IsConnected();	//	 {return m_bIsConnected;}
	void	Disconnectd() { m_bIsConnected = 0; }

	void	SetConnectionAs(BOOL bConnected = TRUE) {m_bIsConnected = bConnected;}
	SQLHDBC GetHDBC() {return m_hDbc;}

	LPTSTR	GetErrMsg() {return m_Error;}

protected:
	void SQLFree();
	void SQLAlloc();
	
protected:

	SQLHDBC m_hDbc;
	SQLHENV m_hEnv;
	BOOL	m_bIsConnected;
	LONG	m_lLoginTimeout;
	LONG	m_lConnectionTimeout;
	TCHAR	m_Error[MAX_PATH];
};

class CODBCRecordset
{
public:

	CODBCRecordset(CODBCDatabase* pDb)
	{
		m_pConnection	= pDb;
		m_hStmt			= NULL;
		m_bIsEOF		= FALSE;
		m_bIsBOF		= FALSE;
		m_lRowsAffected = 0;
		
		memset(m_Error, 0x00, sizeof(m_Error));

		AllocStmt();
	};

	~CODBCRecordset()
	{
		Close();
	
		m_bIsEOF = FALSE;
		m_bIsBOF = FALSE;
	};

	BOOL Open(CHAR* szSqlStr);
	void Close();
	void Clear();

	LONG GetFieldLength(int nField);
	int  GetFieldIndex(CHAR* szFieldName);
	BOOL GetFieldName(int nField, CHAR* szFieldName);
	BOOL GetFieldAttributes(int nField, CHAR* szFieldName, int& nType, int& nLength);
	int  GetFieldCount();
	
	CHAR GetFieldCHAR(int nField);
	BYTE GetFieldBYTE(int nField);

	BOOL GetFieldValue(int nField, WORD *Data);
	BOOL GetFieldValue(CHAR* szFieldName, WORD *Data);
	BOOL GetFieldValue(int nField, DWORD *Data);
	BOOL GetFieldValue(CHAR* szFieldName, DWORD *Data);
	BOOL GetFieldValue(int nField, CHAR* Data);
	BOOL GetFieldValue(CHAR* szFieldName, CHAR *Data);
	BOOL GetFieldValue(int nField, SHORT *Data);
	BOOL GetFieldValue(CHAR* szFieldName, SHORT *Data);
	BOOL GetFieldValue(int nField, INT *Data);
	BOOL GetFieldValue(CHAR* szFieldName, INT *Data);
	BOOL GetFieldValue(int nField, LONG *Data);
	BOOL GetFieldValue(CHAR* szFieldName, LONG *Data);
	BOOL GetFieldValue(int nField, FLOAT *Data);
	BOOL GetFieldValue(CHAR* szFieldName, FLOAT *Data);	
	BOOL GetFieldValue(int nField, DOUBLE *Data);
	BOOL GetFieldValue(CHAR* szFieldName, DOUBLE *Data);	
	BOOL GetFieldValue(int nField, struct tm* time);
	BOOL GetFieldValue(CHAR* szFieldName, struct tm* time);
	
	BOOL WriteBlob(LPCTSTR szSQL, LPVOID pBlob, int size);
	BOOL GetFieldBlob(int nField, LPVOID pBlob);
	BOOL GetFieldBlob(CHAR* szFieldName, LPVOID pBlob);

	BOOL MoveFirst();
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveLast();
	
	BOOL IsEof() {return m_bIsEOF;};
	BOOL IsBof() {return m_bIsBOF;};

	LONG GetRowsAffected() {SQLRowCount(m_hStmt, &m_lRowsAffected); return m_lRowsAffected;}
	LPTSTR GetErrMsg();

protected:
	void AllocStmt();

protected:

	CODBCDatabase*	m_pConnection;
	SQLHDBC			m_hDbc;
	SQLHSTMT		m_hStmt;	
	BOOL			m_bIsEOF;
	BOOL			m_bIsBOF;
	LONG			m_lRowsAffected;
	TCHAR			m_Error[MAX_PATH];
};