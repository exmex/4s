#include "stdafx.h"
#include "TQClassSet.h"


IMPLEMENT_DYNAMIC( CTQClassSet, CRecordset)

CTQClassSet::CTQClassSet(CDatabase* pdb)
: CRecordset(pdb)
{
	m_szNAME.Empty();
	m_dwClassID = 0;
	m_bClassMain = 0;

	m_nFields = 3;
	m_nDefaultType = snapshot;
}

CString CTQClassSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTQClassSet::GetDefaultSQL()
{
	return _T("[dbo].[TQCLASSCHART]");
}

void CTQClassSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTQClassSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[dwClassID]"), m_dwClassID);
	RFX_Text(pFX, _T("[szNAME]"), m_szNAME);
	RFX_Byte(pFX, _T("[bClassMain]"), m_bClassMain);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTQClassSet Áø´Ü

#ifdef _DEBUG
void CTQClassSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTQClassSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
