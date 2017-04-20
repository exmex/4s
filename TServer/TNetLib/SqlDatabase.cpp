// SqlDatabase.cpp: implementation of the CSqlDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSqlDatabase::CSqlDatabase()
{
	Clear();
}

CSqlDatabase::~CSqlDatabase()
{
	Close();
}

BOOL CSqlDatabase::Open(LPCTSTR lpszDsn,
						LPCTSTR lpszUser,
						LPCTSTR lpszPasswd)
{
	SQLRETURN sqlret;

	sqlret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);	
	if( TSQL_FAIL( sqlret ) )
		return FALSE;

	sqlret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
	if( TSQL_FAIL( sqlret ) )
	{	
		Close();
		return FALSE;
	}

	sqlret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if( TSQL_FAIL( sqlret ) )
	{		
		Close();
		return FALSE;
	}

	sqlret = SQLConnect( m_hdbc,
		(SQLCHAR*)lpszDsn, SQL_NTS,
		(SQLCHAR*)lpszUser, SQL_NTS, 
		(SQLCHAR*)lpszPasswd, SQL_NTS);
	if( TSQL_FAIL( sqlret ) )
	{		
		Close();
		return FALSE;
	}
	
	m_bOpen = TRUE;

	return TRUE;
}

void CSqlDatabase::Close()
{
	ClearQuery();

	if( IsOpen() )
		SQLDisconnect(m_hdbc );

	if( SQL_NULL_HANDLE != m_hdbc )
		SQLFreeHandle( SQL_HANDLE_DBC, m_hdbc);

	if( SQL_NULL_HANDLE != m_henv )
		SQLFreeHandle( SQL_HANDLE_ENV, m_henv);

	Clear();
}

void CSqlDatabase::Clear()
{
	m_bOpen = FALSE;
	m_hdbc = m_henv = SQL_NULL_HANDLE;
}

BOOL CSqlDatabase::IsOpen()
{
	return m_bOpen;
}


SQLHDBC CSqlDatabase::HDBC()
{
	return m_hdbc;
}

BOOL CSqlDatabase::AddQuery(CSqlQuery *pQuery, string strKey)
{
	QUERY_MAP::iterator it;

	it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end()) return FALSE;

	m_mapQuery.insert(QUERY_MAP::value_type(strKey, pQuery));
	return TRUE;		
}

CSqlQuery * CSqlDatabase::DelQuery(string strKey)
{
	CSqlQuery *pQuery = NULL;

	QUERY_MAP::iterator it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end())
	{
		pQuery =(*it).second;
		m_mapQuery.erase(it);
	}

	return pQuery;
}

CSqlQuery * CSqlDatabase::GetQuery(string strKey)
{
	CSqlQuery *pQuery = NULL;

	QUERY_MAP::iterator it = m_mapQuery.find(strKey);
	if( it != m_mapQuery.end()) pQuery =(*it).second;

	return pQuery;
}

void CSqlDatabase::ClearQuery()
{
	QUERY_MAP::iterator it;

	for( it = m_mapQuery.begin(); it != m_mapQuery.end(); it++)
	{
		(*it).second->ResetKey();
		delete (*it).second;
	}
	m_mapQuery.clear();
}
