// TTermTextSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TTermTextSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTermTextSet

IMPLEMENT_DYNAMIC(CTTermTextSet, CRecordset)

CTTermTextSet::CTTermTextSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTTermTextSet)
	m_dwQuestID = 0;
	m_dwTermID = 0;
	m_szMessage = _T("");
	m_szObjective = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTTermTextSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTTermTextSet::GetDefaultSQL()
{
	return _T("[dbo].[TQTERMTEXTCHART]");
}

void CTTermTextSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTTermTextSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Long(pFX, _T("[dwTermID]"), m_dwTermID);
	RFX_Text(pFX, _T("[szMessage]"), m_szMessage, 1024);
	RFX_Text(pFX, _T("[szObjective]"), m_szObjective, 1024);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTTermTextSet diagnostics

#ifdef _DEBUG
void CTTermTextSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTTermTextSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
