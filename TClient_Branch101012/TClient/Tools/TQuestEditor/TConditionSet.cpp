// TConditionSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TConditionSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTConditionSet

IMPLEMENT_DYNAMIC(CTConditionSet, CRecordset)

CTConditionSet::CTConditionSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTConditionSet)
	m_dwQuestID = 0;
	m_bConditionType = 0;
	m_dwConditionID = 0;
	m_bCount = 0;
	m_dwID = 0;
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTConditionSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTConditionSet::GetDefaultSQL()
{
	return _T("[dbo].[TQCONDITIONCHART]");
}

void CTConditionSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTConditionSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Byte(pFX, _T("[bConditionType]"), m_bConditionType);
	RFX_Long(pFX, _T("[dwConditionID]"), m_dwConditionID);
	RFX_Byte(pFX, _T("[bCount]"), m_bCount);
	RFX_Long(pFX, _T("[dwID]"), m_dwID);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTConditionSet diagnostics

#ifdef _DEBUG
void CTConditionSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTConditionSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
