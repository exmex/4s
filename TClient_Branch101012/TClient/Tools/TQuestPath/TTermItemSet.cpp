#include "stdafx.h"
#include "TTermItemSet.h"


IMPLEMENT_DYNAMIC(CTTermItemSet, CRecordset)

CTTermItemSet::CTTermItemSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_dwTriggerID = 0;
	m_dwQuestID = 0;
	m_dwTermID = 0;

	m_bTriggerType = 0;
	m_bTermType = 0;
	m_bType = 0;

	m_nDefaultType = snapshot;
	m_nFields = 6;
}

CString CTTermItemSet::GetDefaultConnect()
{
	return _T("DSN=TQUEST");
}

CString CTTermItemSet::GetDefaultSQL()
{
	return _T("[dbo].[TQUESTTERMCHART] INNER JOIN [dbo].[TQUESTCHART] ON [TQUESTTERMCHART].[dwQuestID] = [TQUESTCHART].[dwQuestID] AND [TQUESTTERMCHART].[bTermType] = 5 AND [TQUESTCHART].[bType] IN ( 3, 4, 9)");
}

void CTTermItemSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Byte(pFX, _T("[TQUESTCHART].[bTriggerType]"), m_bTriggerType);
	RFX_Long(pFX, _T("[TQUESTCHART].[dwTriggerID]"), m_dwTriggerID);
	RFX_Byte(pFX, _T("[TQUESTCHART].[bType]"), m_bType);
	RFX_Long(pFX, _T("[TQUESTTERMCHART].[dwQuestID]"), m_dwQuestID);
	RFX_Byte(pFX, _T("[TQUESTTERMCHART].[bTermType]"), m_bTermType);
	RFX_Long(pFX, _T("[TQUESTTERMCHART].[dwTermID]"), m_dwTermID);
}

/////////////////////////////////////////////////////////////////////////////
// CTTermItemSet Áø´Ü

#ifdef _DEBUG
void CTTermItemSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTTermItemSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
