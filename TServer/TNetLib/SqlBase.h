// SqlBase.h: interface for the CSqlBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLBASE_H__EFA4A766_1BF9_48F2_8803_4E77F1D7E93F__INCLUDED_)
#define AFX_SQLBASE_H__EFA4A766_1BF9_48F2_8803_4E77F1D7E93F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*¡÷¿««“ SQL & C++ Data Types ----------------------------------------
	C Type					SQL Type

	float					REAL
	double					FLOAT

--------------------------------------------------------------------*/

class CSqlBase  
{
public:
	CSqlBase(CSqlDatabase *pdb, LPCTSTR lpszQuery);
	virtual ~CSqlBase();

	void Init(CSqlDatabase *pdb, LPCTSTR lpszQuery);

protected:
	CSqlDatabase *	m_pdb;

	SQLHDBC			m_hdbc;
	SQLHSTMT		m_hstmt;
	SQLCHAR			m_szQuery[MAX_QUERY_LEN];	
	
	SQLINTEGER		m_liCOLS[MAX_LEN_IND_SIZE];
	SQLINTEGER		m_liPARAMS[MAX_LEN_IND_SIZE];

public:
	BOOL Call();
	void Close();
	BOOL Fetch();	
	BOOL Open(BOOL bRetAtOnce= FALSE);

	virtual int GetNumCol();
	virtual int GetNumParam();	

	virtual BOOL AllocStatement();

	BOOL IsNull(int nCol);
	void FreeStatement();


	SQLRETURN BindCol( SQLSMALLINT ci, float *ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, double *ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, TIME_STRUCT *ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, DATE_STRUCT *ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, TIMESTAMP_STRUCT *ptr);

	SQLRETURN BindCol( SQLSMALLINT ci, int* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, long* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, char* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, short* ptr);	
	SQLRETURN BindCol( SQLSMALLINT ci, __int64* ptr);

	SQLRETURN BindCol( SQLSMALLINT ci, unsigned long* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, unsigned char* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, unsigned short* ptr);
	SQLRETURN BindCol( SQLSMALLINT ci, unsigned __int64* ptr);	
	
	SQLRETURN BindColStr( SQLSMALLINT ci, LPTSTR ptr, SQLINTEGER len);
	SQLRETURN BindCustomCol( SQLSMALLINT ci, SQLSMALLINT tt, LPVOID ptr, SQLINTEGER len);
	
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, float* ptr);
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, double* ptr);
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIME_STRUCT* ptr);
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, DATE_STRUCT* ptr);
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, TIMESTAMP_STRUCT* ptr);

	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, int* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, long* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, char* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, short* ptr );	
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, __int64* ptr);

	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned long* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned char* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned short* ptr );
	SQLRETURN BindParam( SQLSMALLINT pt, SQLSMALLINT pi, unsigned __int64* ptr);

	SQLRETURN BindParamStr( SQLSMALLINT pt, SQLSMALLINT pi, LPTSTR ptr, SQLINTEGER len );
	SQLRETURN BindCustomParam( SQLSMALLINT pt, SQLSMALLINT pi, SQLSMALLINT ct, SQLSMALLINT st, LPVOID ptr, SQLINTEGER len);
	
};

#endif // !defined(AFX_SQLBASE_H__EFA4A766_1BF9_48F2_8803_4E77F1D7E93F__INCLUDED_)
