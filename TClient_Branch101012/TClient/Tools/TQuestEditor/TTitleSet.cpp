// TTitleSet.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TTitleSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTitleSet

IMPLEMENT_DYNAMIC(CTTitleSet, CRecordset)

CTTitleSet::CTTitleSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CTTitleSet)
	m_dwQuestID = 0;
	m_dwClassID = 0;
	m_szTitle = _T("");
	m_szMessage = _T("");
	m_szComplete = _T("");
	m_szAccept = _T("");
	m_szReject = _T("");
	m_szSummary = _T("");
	m_szNPCName = _T("");
	m_szReply = _T("");
	m_nFields = 10;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CTTitleSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTTitleSet::GetDefaultSQL()
{
	return _T("[dbo].[TQTITLECHART]");
}

void CTTitleSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CTTitleSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[dwQuestID]"), m_dwQuestID);
	RFX_Long(pFX, _T("[dwClassID]"), m_dwClassID);
	RFX_Text(pFX, _T("[szTitle]"), m_szTitle, 255);
	RFX_Text(pFX, _T("[szMessage]"), m_szMessage, 1024);
	RFX_Text(pFX, _T("[szComplete]"), m_szComplete, 1024);
	RFX_Text(pFX, _T("[szAccept]"), m_szAccept, 1024);
	RFX_Text(pFX, _T("[szReject]"), m_szReject, 1024);
	RFX_Text(pFX, _T("[szSummary]"), m_szSummary, 1024);
	RFX_Text(pFX, _T("[szNPCName]"), m_szNPCName, 1024);
	RFX_Text(pFX, _T("[szReply]"), m_szReply, 1024);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTTitleSet diagnostics

#ifdef _DEBUG
void CTTitleSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTTitleSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
