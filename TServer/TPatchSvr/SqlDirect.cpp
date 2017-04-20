// SqlDirect.cpp: implementation of the CSqlDirect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSqlDirect::CSqlDirect( CSqlDatabase *pdb, LPCTSTR lpszQuery)
:CSqlBase( pdb, lpszQuery)
{
}

CSqlDirect::~CSqlDirect()
{
	FreeStatement();

	while(!m_cols.empty())
	{
		delete m_cols.back();
		m_cols.pop_back();
	}
}

BOOL CSqlDirect::AllocStatement()
{
	if(!CSqlBase::AllocStatement())
		return FALSE;

	for( int i=0; i<INT(m_cols.size()); i++)
	{
		CBindDesc *pDesc = (CBindDesc *) m_cols[i];

		if(!pDesc)
			return FALSE;
		pDesc->MAlloc();

		SQLRETURN ret = SQLBindCol(
			m_hstmt,
			i + 1,
			pDesc->m_type,
			pDesc->m_ptr,
			pDesc->m_size,
			&m_liCOLS[i]);

		if(TSQL_FAIL(ret))
			return FALSE;
	}

	return TRUE;
}

void CSqlDirect::AddBindDesc( CBindDesc *pDesc)
{
	m_cols.push_back(pDesc);
}

BOOL CSqlDirect::Textize( int nCol, CString &text)
{
	if( nCol >= INT(m_cols.size()) )
		return FALSE;
	text.Empty();

	CBindDesc *pDesc = (CBindDesc *) m_cols[nCol];
	if( !pDesc || !pDesc->m_ptr )
		return FALSE;

	switch(pDesc->m_type)
	{
	case SQL_C_SBIGINT			: text.Format(_T("%d"), *((__int64 *) pDesc->m_ptr)); break;
	case SQL_C_LONG				: text.Format(_T("%d"), *((int *) pDesc->m_ptr)); break;	
	case SQL_C_SHORT			: text.Format(_T("%d"), *((short *) pDesc->m_ptr)); break;
	case SQL_TINYINT			: text.Format(_T("%d"), *((char *) pDesc->m_ptr)); break;
	case SQL_C_BIT				: text.Format(_T("%d"), *((char *) pDesc->m_ptr)); break;	
	case SQL_C_DOUBLE			: text.Format(_T("%f"), *((double *) pDesc->m_ptr)); break;
	case SQL_C_FLOAT			: text.Format(_T("%f"), *((float *) pDesc->m_ptr)); break;	
	case SQL_C_WCHAR			: text.Format(_T("%s"), (LPTSTR) pDesc->m_ptr); break;
	case SQL_C_CHAR				: text.Format(_T("%s"), (LPTSTR) pDesc->m_ptr); break;
	case SQL_C_TYPE_TIMESTAMP	:
		{
			TIMESTAMP_STRUCT *pTime = (TIMESTAMP_STRUCT *) pDesc->m_ptr;

			text.Format(
				_T("%d-%d-%d %d:%d:%d.%d"),
				pTime->year,
				pTime->month,
				pTime->day,
				pTime->hour,
				pTime->minute,
				pTime->second,
				pTime->fraction);
		}

		break;

	default						: return FALSE;
	}

	return TRUE;
}
