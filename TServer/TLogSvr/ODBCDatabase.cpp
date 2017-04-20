
#include "StdAfx.h"
#include "ODBCDatabase.h"

#define BLOBBATCH 12000

/////////////////////////////////////////////////////////////
//
// CODBCDatabase Class
//

void CODBCDatabase::SQLAlloc()
{
	Init();
}

void CODBCDatabase::SQLFree()
{
	SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	m_hDbc = NULL; //SQL_NULL_HDBC, SQL_NULL_HSTMT, or SQL_NULL_HDESC,
	m_hEnv = NULL;
}


BOOL CODBCDatabase::IsConnected()
{
	LONG lConnection = SQL_CD_TRUE;

	//supported by odbc 3.5 이 함수는 테스트가 필요함
	SQLRETURN ret = SQLGetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_DEAD, (SQLPOINTER)&lConnection, sizeof(lConnection), 0);
	
	if(ret == SQL_SUCCESS || ret ==SQL_SUCCESS_WITH_INFO)
		m_bIsConnected = (lConnection == SQL_CD_FALSE ? 1 : 0);
	else
		m_bIsConnected = FALSE;
	
	return m_bIsConnected;
}


void CODBCDatabase::Close()
{
	m_bIsConnected = FALSE;
	if(m_hDbc == NULL)
		return;
	SQLDisconnect(m_hDbc);
	SQLFree();
}
	
void CODBCDatabase::Init()
{
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
	SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
}

BOOL CODBCDatabase::Connect(TCHAR* szDSN, TCHAR* szUser, TCHAR* szPass)
{
	SQLRETURN ret;
	
	ret = SQLConnect(m_hDbc, 
					(SQLCHAR*)szDSN, 
					SQL_NTS,
					(SQLCHAR*)szUser, 
					SQL_NTS,
					(SQLCHAR*)szPass, 
					SQL_NTS);

	m_bIsConnected = ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
	
	if(!m_bIsConnected)
	{
		SQLINTEGER NativeError;
		SQLCHAR SqlState[6], Msg[255];
		SQLSMALLINT MsgLen;

		if(SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, 1, SqlState, &NativeError, 
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) 
		{
			wsprintf(m_Error, "SQLSTATE:%s, MESSAGE:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
		}
	}

	return m_bIsConnected;
}

BOOL CODBCDatabase::Browse(TCHAR* szConnStrIn, TCHAR* szConnStrOut)
{
	SQLRETURN ret;
	SWORD swLenOut = 0;

	ret = SQLBrowseConnect(m_hDbc, 
							(SQLCHAR*)szConnStrIn, 
							sizeof(szConnStrIn), 
							(SQLCHAR*)szConnStrOut, 
							MAX_BUFFER, 
							&swLenOut);

	m_bIsConnected = ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;

	if(!m_bIsConnected)
	{
		SQLINTEGER NativeError;
		SQLCHAR SqlState[6], Msg[255];
		SQLSMALLINT MsgLen;

		if(SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, 1, SqlState, &NativeError, 
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) 
		{
			wsprintf(m_Error, "SQLSTATE:%s, MESSAGE:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
		}
	}

	return m_bIsConnected;
}

void CODBCDatabase::SetConnectionTimeout(LONG nSeconds)
{
	if(m_hDbc)
	{
		SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)nSeconds, 0);
	}

	m_lConnectionTimeout = nSeconds;
}

void CODBCDatabase::SetLoginTimeout(LONG nSeconds)
{
	if(m_hDbc)
	{
		SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)nSeconds, 0);
	}

	m_lLoginTimeout = nSeconds;
}

BOOL CODBCDatabase::DriverConnect(TCHAR* szConnStr, TCHAR* szConnStrOut, HWND hWnd, enum drvCompletion drvConn)
{
	SQLRETURN ret;
	SQLSMALLINT pcbConnStrOut;

	if(drvConn == sqlPrompt && hWnd == NULL)
		return FALSE;

	ret = SQLDriverConnect(m_hDbc, 
							hWnd, 
							(SQLCHAR*)szConnStr, 
							SQL_NTS, 
							(SQLCHAR*)szConnStrOut,
							sizeof(szConnStrOut), 
							&pcbConnStrOut, 
							(SQLUSMALLINT)drvConn);
	
	m_bIsConnected = ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;

	if(!m_bIsConnected)
	{
		SQLINTEGER NativeError;
		SQLCHAR SqlState[6], Msg[255];
		SQLSMALLINT MsgLen;

		if(SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, 1, SqlState, &NativeError, 
			Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) 
		{
			wsprintf(m_Error, "SQLSTATE:%s, MESSAGE:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
		}
	}

	return m_bIsConnected;
}

void CODBCDatabase::SetReadOnly(BOOL bReadOnly)
{
	SQLSetConnectAttr(m_hDbc, SQL_ATTR_ACCESS_MODE, (SQLPOINTER)(bReadOnly? SQL_MODE_READ_ONLY : SQL_MODE_READ_WRITE), 0);
}

LONG CODBCDatabase::GetConnectionTimeout()
{
	LONG nSeconds;

	SQLGetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, &nSeconds, NULL, 0);

	return nSeconds;
}

LONG CODBCDatabase::GetLoginTimeout()
{
	LONG nSeconds;

	SQLGetConnectAttr(m_hDbc, SQL_ATTR_LOGIN_TIMEOUT, &nSeconds, NULL, 0);

	return nSeconds;
}

/////////////////////////////////////////////////////////////
//
// CODBCRecordset Class
//

void CODBCRecordset::AllocStmt()
{
	SQLAllocHandle(SQL_HANDLE_STMT, m_pConnection->GetHDBC(), &m_hStmt);
}

BOOL CODBCRecordset::Open(CHAR *szSqlStr)
{
	SQLRETURN nResult;

	nResult = SQLExecDirect(m_hStmt, (SQLCHAR*)szSqlStr, SQL_NTS);

	if(!SQL_SUCCEEDED(nResult) && nResult != SQL_NO_DATA )
	{
	
		SQLINTEGER	nErrCode;
		SQLSMALLINT sLen;
		SQLCHAR SqlState[6], Msg[MAX_PATH];
		
		if(SQLGetDiagRec
		(
			SQL_HANDLE_STMT	,
			m_hStmt			,
			1				,
			SqlState		,
			&nErrCode		,
			Msg				,
			MAX_PATH		,
			&sLen
		) != SQL_NO_DATA)
		{
			wsprintf(m_Error, "SQLSTATE:%s, MESSAGE:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
		}

		//connection error
		if(_tcscmp((char *)SqlState, "08S01" ) != 0)
		{
			m_pConnection->SetConnectionAs(FALSE);		
		}

		return FALSE;
	}

	SQLFetch(m_hStmt);
	
	return TRUE;
}

CHAR CODBCRecordset::GetFieldCHAR(int nField)
{
	SQLINTEGER cbValue;
	CHAR c;
	int nLength = GetFieldLength(nField) + 1;

	SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_STINYINT, &c, nLength, &cbValue);

	return c;
}

BYTE CODBCRecordset::GetFieldBYTE(int nField)
{
	SQLINTEGER cbValue;
	BYTE c;
	int nLength = GetFieldLength(nField) + 1;

	SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_UTINYINT, &c, nLength, &cbValue);

	return c;
}

BOOL CODBCRecordset::GetFieldValue(int nField, WORD *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_USHORT, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR* szFieldName, WORD *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, DWORD *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_ULONG, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR* szFieldName, DWORD *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, CHAR *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_CHAR, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR *szFieldName, CHAR *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, SHORT *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_SSHORT, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR* szFieldName, SHORT *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, INT *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_SLONG, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR* szFieldName, INT *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);
}

BOOL CODBCRecordset::GetFieldValue(int nField, LONG *Data)
{
	SQLRETURN ret;
	SQLINTEGER cbValue;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_LONG, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR *szFieldName, LONG *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, FLOAT *Data)
{
	SQLINTEGER cbValue;
	SQLRETURN ret;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_DOUBLE, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR* szFieldName, FLOAT *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, DOUBLE *Data)
{	
	SQLINTEGER cbValue;
	SQLRETURN ret;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_DOUBLE, Data, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldValue(CHAR *szFieldName, DOUBLE *Data)
{
	return GetFieldValue(GetFieldIndex(szFieldName), Data);	
}

BOOL CODBCRecordset::GetFieldValue(int nField, struct tm* time)
{
	
	SQLINTEGER cbValue;
	SQLRETURN ret;
	int nLength = GetFieldLength(nField) + 1;
	SQL_TIMESTAMP_STRUCT* sqltm = new SQL_TIMESTAMP_STRUCT;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_TYPE_TIMESTAMP, sqltm, nLength, &cbValue) == SQL_SUCCESS;
	if(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{	
		time->tm_year = sqltm->year;
		time->tm_mon = sqltm->month - 1; //January must be = 0		
		time->tm_mday = sqltm->day;
		time->tm_hour = sqltm->hour;
		time->tm_min = sqltm->minute;
		time->tm_sec = sqltm->second;
		return TRUE;
	}
	return FALSE;
}

BOOL CODBCRecordset::GetFieldValue(CHAR *szFieldName, struct tm* time)
{
	return GetFieldValue(GetFieldIndex(szFieldName), time);	
}

BOOL CODBCRecordset::GetFieldBlob(int nField, LPVOID pBlob)
{
	SQLINTEGER cbValue;
	SQLRETURN ret;
	int nLength = GetFieldLength(nField) + 1;
	
	ret = SQLGetData(m_hStmt, (SQLUSMALLINT)nField + 1, SQL_C_BINARY, pBlob, nLength, &cbValue) == SQL_SUCCESS;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::GetFieldBlob(CHAR* szFieldName, LPVOID pBlob)
{
	return GetFieldBlob(GetFieldIndex(szFieldName), pBlob);	
}

BOOL CODBCRecordset::GetFieldName(int nField, CHAR *szFieldName)
{
	int nType, nLength;

	return GetFieldAttributes(nField, szFieldName, nType, nLength);
}

int CODBCRecordset::GetFieldIndex(CHAR *szFieldName)
{
	SQLSMALLINT nCols;
	int nCol = 1;
	CHAR szColName[MAX_COL_NAME_LEN];
	SQLSMALLINT cbColNameLen, fSqlType, ibScale, fNullable;
	SQLUINTEGER cbColDef;


	SQLNumResultCols(m_hStmt, &nCols);
	while(nCol < nCols)
	{
		memset(szColName, 0, 32 * sizeof(CHAR));
		SQLDescribeCol(m_hStmt, nCol, (SQLCHAR*)szColName, MAX_COL_NAME_LEN, &cbColNameLen, &fSqlType, &cbColDef, &ibScale, &fNullable);

		if(_stricmp(szColName, szFieldName) == 0)
			return nCol - 1;
		nCol++;
	}
	return -1;
}

BOOL CODBCRecordset::MoveFirst()
{
	SQLRETURN ret;

	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_FIRST, 0);
	
	m_bIsBOF = ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
	if(m_bIsBOF)
		m_bIsEOF = FALSE;

	return m_bIsBOF;
}

BOOL CODBCRecordset::MoveNext()
{
	SQLRETURN ret;
	
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_NEXT, 0);
	
	m_bIsEOF = ret == SQL_NO_DATA;
	m_bIsBOF = FALSE;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::MovePrevious()
{
	SQLRETURN ret;
	
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_PRIOR, 0);
	m_bIsBOF = ret == SQL_NO_DATA;
	m_bIsEOF = FALSE;
	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

BOOL CODBCRecordset::MoveLast()
{
	SQLRETURN ret;
	
	ret = SQLFetchScroll(m_hStmt, SQL_FETCH_LAST, 0);

	m_bIsEOF = ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
	if(m_bIsEOF)
		m_bIsBOF = FALSE;

	return m_bIsEOF;
}

LONG CODBCRecordset::GetFieldLength(int nField)
{
	SQLSMALLINT fSqlType, ibScale, fNullable;
	SQLUINTEGER cbColDef;
	
	SQLDescribeCol(m_hStmt, nField + 1, NULL, 0, 0, &fSqlType, &cbColDef, &ibScale, &fNullable);

	return cbColDef;	
}

BOOL CODBCRecordset::GetFieldAttributes(int nField, CHAR* szFieldName, int& nType, int& nLength)
{
	SQLRETURN ret;
	SQLSMALLINT cbColNameLen, fSqlType, ibScale, fNullable;
	SQLUINTEGER cbColDef;
	
	ret = SQLDescribeCol(m_hStmt, nField + 1, (SQLCHAR*)szFieldName, MAX_COL_NAME_LEN, &cbColNameLen, &fSqlType, &cbColDef, &ibScale, &fNullable);
	
	nType = fSqlType;
	nLength = cbColDef;

	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;	
}

int CODBCRecordset::GetFieldCount()
{

	SQLSMALLINT nFieldCount = 0;
	SQLNumResultCols(m_hStmt, &nFieldCount);

	return nFieldCount;
}

void CODBCRecordset::Close()
{
	if(m_hStmt != NULL)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = NULL;
	}
}

//
//	버퍼의 BLOB 데이터를 저장한다. szSQL은 하나의 BLOB 데이터를 저장하는 UPDATE, INSERT SQL문이어야 한다.
//
BOOL CODBCRecordset::WriteBlob(LPCTSTR szSQL, LPVOID pBlob, int size)
{
	SQLRETURN ret;
	SQLINTEGER cbBlob;
	BYTE tmp[BLOBBATCH],*p;
	SQLPOINTER pToken;
	int nPut;

	cbBlob = SQL_LEN_DATA_AT_EXEC(size);
	
	SQLBindParameter(
				m_hStmt, 
				1, 
				SQL_PARAM_INPUT, 
				SQL_C_BINARY, 
				SQL_LONGVARBINARY, 
				size,
				0,
				(SQLPOINTER)1, 
				0, 
				&cbBlob);

	SQLExecDirect(m_hStmt, (SQLCHAR *)szSQL, SQL_NTS);

	ret = SQLParamData(m_hStmt, &pToken);

	while (ret == SQL_NEED_DATA) 
	{
		if ((int)pToken == 1) 
		{
			for (p=(BYTE *)pBlob; p<(BYTE *)pBlob+size; p+=BLOBBATCH) 
			{
				nPut=min(BLOBBATCH, (BYTE *)pBlob+size-p);
				memcpy(tmp,p,nPut);
				
				ret = SQLPutData(m_hStmt, (PTR)tmp, nPut);
				if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
					return FALSE;
			}
		}

		ret = SQLParamData(m_hStmt, &pToken);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
			return FALSE;
	}

	return TRUE;
}

void CODBCRecordset::Clear()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

char* CODBCRecordset::GetErrMsg()
{
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;

	if(SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, 1, SqlState, &NativeError, 
		Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA) 
	{
		wsprintf(m_Error, "SQLSTATE:%s, MESSAGE:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
	}

	return m_Error;
}