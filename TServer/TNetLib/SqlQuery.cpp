// SqlQuery.cpp: implementation of the CSqlQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSqlQuery::CSqlQuery(CSqlDatabase *pdb, LPCTSTR lpszQuery)
:CSqlBase(pdb, lpszQuery)
{
	m_strKey = "";		
}

CSqlQuery::CSqlQuery(CSqlDatabase *pdb, string strKey, LPCTSTR lpszQuery)
:CSqlBase(pdb, lpszQuery)
{	
	m_strKey = strKey;	
	if( AllocStatement() &&
		pdb->AddQuery(this, strKey))
	{
		m_strKey = strKey;
	}
}

CSqlQuery::~CSqlQuery()
{
	FreeStatement();

	if(m_strKey != "")
		m_pdb->DelQuery(m_strKey);
}

SQLRETURN CSqlQuery::BindParameters(BOOL bRewind)
{	
	return SQL_SUCCESS;
}

SQLRETURN CSqlQuery::BindColumns()
{
	return SQL_SUCCESS;
}

BOOL CSqlQuery::AllocStatement()
{	
	if(!CSqlBase::AllocStatement()) return FALSE;

	if(TSQL_FAIL(BindParameters()))
	{
		int cntrec = 1;
		SQLDIAGREC diag;
		ZeroMemory(&diag, sizeof(SQLDIAGREC));

		while(SQL_NO_DATA != SQLGetDiagRec(
			SQL_HANDLE_STMT, m_hstmt, cntrec++, diag.state,
			&diag.err, diag.msg, MAX_SQLDIAGREC_MSG, &diag.cbmsg))
		{
			ATLTRACE(_T("[%s] %s\n"), diag.state, diag.msg);
		}

		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;
		return FALSE;
	}

	if(TSQL_FAIL(BindColumns()))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;		
		return FALSE;
	}
	return TRUE;
}

void CSqlQuery::ResetKey()
{
	m_strKey = "";
}



