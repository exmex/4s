#include "stdafx.h"
#include "TRewardItemSet.h"


IMPLEMENT_DYNAMIC(CTRewardItemSet, CRecordset)

CTRewardItemSet::CTRewardItemSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_dwTriggerID = 0;
	m_dwRewardID = 0;

	m_nDefaultType = snapshot;
	m_nFields = 2;
}

CString CTRewardItemSet::GetDefaultConnect()
{
	return _T("DSN=TQUEST");
}

CString CTRewardItemSet::GetDefaultSQL()
{
	return _T("[dbo].[TQREWARDCHART] INNER JOIN [dbo].[TQUESTCHART] ON [TQREWARDCHART].[dwQuestID] = [TQUESTCHART].[dwQuestID] AND [TQREWARDCHART].[bRewardType] = 2 AND [TQUESTCHART].[bTriggerType] = 3 AND [TQUESTCHART].[bType] IN ( 0, 8, 18)");
}

void CTRewardItemSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Long(pFX, _T("[TQREWARDCHART].[dwRewardID]"), m_dwRewardID);
	RFX_Long(pFX, _T("[TQUESTCHART].[dwTriggerID]"), m_dwTriggerID);
}

/////////////////////////////////////////////////////////////////////////////
// CTRewardItemSet Áø´Ü

#ifdef _DEBUG
void CTRewardItemSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTRewardItemSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
