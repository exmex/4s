#include "stdafx.h"
#include "TQClassSet.h"


IMPLEMENT_DYNAMIC(CTQClassSet, CRecordset)

CTQClassSet::CTQClassSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_dwClassID = 0;
	m_szNAME = "";

	m_nDefaultType = snapshot;
	m_nFields = 2;
}

CString CTQClassSet::GetDefaultConnect()
{
	return _T("DSN=TQUEST");
}

CString CTQClassSet::GetDefaultSQL()
{
	return _T("[dbo].[TQCLASSCHART]");
}

void CTQClassSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Long(pFX, _T("[dwClassID]"), m_dwClassID);
	RFX_Text(pFX, _T("[szNAME]"), m_szNAME);
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
