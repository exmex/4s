// TRewardSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TRewardSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTRewardSet

IMPLEMENT_DYNAMIC(CTRewardSet, CRecordset)

CTRewardSet::CTRewardSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTRewardSet)
	m_bCount = 0;
	m_bRewardType = 0;
	m_dwID = 0;
	m_dwQuestID = 0;
	m_dwRewardID = 0;
	m_bTakeData = 0;
	m_bTakeMethod = 0;
	m_dwQuestMob = 0;
	m_dwQuestTime = 0;
	m_dwQuestPathMob = 0;
	m_dwTicketID = 0;
	m_bSendQ = 0;
	m_nFields = 12;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTRewardSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTRewardSet::GetDefaultSQL()
{
	return _T("[dbo].[TQREWARDCHART]");
}

void CTRewardSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTRewardSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[bCount]"), m_bCount);
	RFX_Byte(pFX, _T("[bRewardType]"), m_bRewardType);
	RFX_Long(pFX, _T("[dwID]"), m_dwID);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Long(pFX, _T("[dwRewardID]"), m_dwRewardID);
	RFX_Byte(pFX, _T("[bTakeData]"), m_bTakeData);
	RFX_Byte(pFX, _T("[bTakeMethod]"), m_bTakeMethod);
	RFX_Long(pFX, _T("[dwQuestMob]"), m_dwQuestMob);
	RFX_Long(pFX, _T("[dwQuestTime]"), m_dwQuestTime);
	RFX_Long(pFX, _T("[dwQuestPathMob]"), m_dwQuestPathMob);
	RFX_Long(pFX, _T("[dwTicketID]"), m_dwTicketID);
	RFX_Byte(pFX, _T("[bSendQ]"), m_bSendQ);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTRewardSet diagnostics

#ifdef _DEBUG
void CTRewardSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTRewardSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
