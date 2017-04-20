#include "stdafx.h"
#include "TQTermSet.h"


IMPLEMENT_DYNAMIC(CTQTermSet, CRecordset)

CTQTermSet::CTQTermSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_szObjective = "";

	m_bTermType = 0;
	m_dwTermID = 0;

	m_nDefaultType = snapshot;
	m_nFields = 3;
}

CString CTQTermSet::GetDefaultConnect()
{
	return _T("DSN=TQUEST");
}

CString CTQTermSet::GetDefaultSQL()
{
	return _T("[dbo].[TQUESTTERMCHART] INNER JOIN [dbo].[TQTERMTEXTCHART] ON [TQUESTTERMCHART].[dwID] = [TQTERMTEXTCHART].[dwTermID]");
}

void CTQTermSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX, _T("[TQTERMTEXTCHART].[szObjective]"), m_szObjective, 1024);
	RFX_Byte(pFX, _T("[TQUESTTERMCHART].[bTermType]"), m_bTermType);
	RFX_Long(pFX, _T("[TQUESTTERMCHART].[dwTermID]"), m_dwTermID);
}

/////////////////////////////////////////////////////////////////////////////
// CTQTermSet Áø´Ü

#ifdef _DEBUG
void CTQTermSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTQTermSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
