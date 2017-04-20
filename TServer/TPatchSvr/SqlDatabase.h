// SqlDatabase.h: interface for the CSqlDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLDATABASE_H__C74E2089_D0D9_4BB2_BD0A_42585A285ED2__INCLUDED_)
#define AFX_SQLDATABASE_H__C74E2089_D0D9_4BB2_BD0A_42585A285ED2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define TSQL_NODATA(sqlret)						( SQL_NO_DATA == sqlret )
#define TSQL_REC(sqlret)						( SQL_SUCCESS_WITH_INFO == sqlret || SQL_ERROR == sqlret )
#define TSQL_FAIL(sqlret)						( SQL_SUCCESS != sqlret && SQL_SUCCESS_WITH_INFO != sqlret )

#define TSQL_NULL_INDEX							-1


#define MAX_PP_STMT								256
#define MAX_QUERY_LEN							1024
#define MAX_SQL_DATA_TYPE						9

#define MAX_SQLDIAGREC_MSG						512
#define MAX_SQLDIAGREC_STATE					6

#define MAX_LEN_IND_SIZE						256

typedef struct _tagDiagRec
{
	SQLINTEGER	err;
	SQLSMALLINT cbmsg;
	SQLTCHAR	msg[MAX_SQLDIAGREC_MSG];
	SQLTCHAR	state[MAX_SQLDIAGREC_STATE];	
}SQLDIAGREC, *LPSQLDIAGREC;

class CSqlQuery;
typedef map<string, CSqlQuery *> QUERY_MAP;

class CSqlDatabase
{
protected:	
	//Attributes
	BOOL		m_bOpen;

	SQLHENV		m_henv;
	SQLHDBC		m_hdbc;

	QUERY_MAP	m_mapQuery;

public:			
	//Predefined query management
	void ClearQuery();
	CSqlQuery * GetQuery(string strKey);
	CSqlQuery * DelQuery(string strKey);
	BOOL AddQuery(CSqlQuery *pQuery, string strKey);

	//Operations
	void Clear();
	void Close();	
	BOOL IsOpen();		

	BOOL Open(LPCTSTR lpszDsn, LPCTSTR lpszUser, LPCTSTR lpszPasswd);		
	SQLHDBC HDBC();

	CSqlDatabase();
	virtual ~CSqlDatabase();
};

#endif // !defined(AFX_SQLDATABASE_H__C74E2089_D0D9_4BB2_BD0A_42585A285ED2__INCLUDED_)
