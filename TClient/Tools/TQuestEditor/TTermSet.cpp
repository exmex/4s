// TTermSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TTermSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTermSet

IMPLEMENT_DYNAMIC(CTTermSet, CRecordset)

CTTermSet::CTTermSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTTermSet)
	m_bCount = 0;
	m_bTermType = 0;
	m_dwID = 0;
	m_dwQuestID = 0;
	m_dwTermID = 0;
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTTermSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTTermSet::GetDefaultSQL()
{
	return _T("[dbo].[TQUESTTERMCHART]");
}

void CTTermSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTTermSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[bCount]"), m_bCount);
	RFX_Byte(pFX, _T("[bTermType]"), m_bTermType);
	RFX_Long(pFX, _T("[dwID]"), m_dwID);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Long(pFX, _T("[dwTermID]"), m_dwTermID);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTTermSet diagnostics

#ifdef _DEBUG
void CTTermSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTTermSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
