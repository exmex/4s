#include "stdafx.h"
#include "TQuestSet.h"


IMPLEMENT_DYNAMIC(CTQuestSet, CRecordset)

CTQuestSet::CTQuestSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_szComplete = "";
	m_szMessage = "";
	m_szTitle = "";

	m_bTriggerType = 0;
	m_dwTriggerID = 0;
	m_dwQuestID = 0;
	m_dwClassID = 0;
	m_bType = 0;

	m_nDefaultType = snapshot;
	m_nFields = 8;
}

CString CTQuestSet::GetDefaultConnect()
{
	return _T("DSN=TQUEST");
}

CString CTQuestSet::GetDefaultSQL()
{
	return _T("[dbo].[TQUESTCHART] INNER JOIN [dbo].[TQTITLECHART] ON [TQUESTCHART].[dwQuestID] = [TQTITLECHART].[dwQuestID]");
}

void CTQuestSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX, _T("[TQTITLECHART].[szComplete]"), m_szComplete, 1024);
	RFX_Text(pFX, _T("[TQTITLECHART].[szMessage]"), m_szMessage, 1024);
	RFX_Text(pFX, _T("[TQTITLECHART].[szTitle]"), m_szTitle, 255);
	RFX_Long(pFX, _T("[TQTITLECHART].[dwClassID]"), m_dwClassID);
	RFX_Long(pFX, _T("[TQUESTCHART].[dwTriggerID]"), m_dwTriggerID);
	RFX_Long(pFX, _T("[TQUESTCHART].[dwQuestID]"), m_dwQuestID);
	RFX_Byte(pFX, _T("[TQUESTCHART].[bTriggerType]"), m_bTriggerType);
	RFX_Byte(pFX, _T("[TQUESTCHART].[bType]"), m_bType);
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestSet Áø´Ü

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
