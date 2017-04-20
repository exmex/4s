// TQuestSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TQuestSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTQuestSet

IMPLEMENT_DYNAMIC(CTQuestSet, CRecordset)

CTQuestSet::CTQuestSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTQuestSet)
	m_dwQuestID = 0;
	m_dwParentID = 0;
	m_bType = 0;
	m_bTriggerType = 0;
	m_dwTriggerID = 0;
	m_bCountMax = 0;
	m_bForceRun = 0;
	m_bLevel = 1;
	m_bMain = 0;
	m_bConditionCheck = 0;
	m_nFields = 10;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTQuestSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTQuestSet::GetDefaultSQL()
{
	return _T("[dbo].[TQUESTCHART]");
}

void CTQuestSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTQuestSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Long(pFX, _T("[dwParentID]"), m_dwParentID);
	RFX_Byte(pFX, _T("[bType]"), m_bType);
	RFX_Byte(pFX, _T("[bTriggerType]"), m_bTriggerType);
	RFX_Long(pFX, _T("[dwTriggerID]"), m_dwTriggerID);
	RFX_Byte(pFX, _T("[bCountMax]"), m_bCountMax);
	RFX_Byte(pFX, _T("[bForceRun]"), m_bForceRun);
	RFX_Byte(pFX, _T("[bLevel]"), m_bLevel);
	RFX_Byte(pFX, _T("[bMain]"), m_bMain);
	RFX_Byte(pFX, _T("[bConditionCheck]"), m_bConditionCheck);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestSet diagnostics

#ifdef _DEBUG
void CTQuestSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTQuestSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
