#include "stdafx.h"
#include "TMonposSet.h"


IMPLEMENT_DYNAMIC(CTMonposSet, CRecordset)

CTMonposSet::CTMonposSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_szName2 = "";
	m_szName = "";

	m_wMonID = 0;
	m_wMapID = 0;
	m_wKind = 0;

	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;

	m_nDefaultType = snapshot;
	m_nFields = 8;
}

CString CTMonposSet::GetDefaultConnect()
{
	return _T("DSN=TGAME");
}

CString CTMonposSet::GetDefaultSQL()
{
	return _T("[dbo].[TMAPMONCHART] INNER JOIN [dbo].[TMONSPAWNCHART] ON [TMAPMONCHART].[wSpawnID] = [TMONSPAWNCHART].[wID] INNER JOIN [dbo].[TMONSTERCHART] ON [TMAPMONCHART].[wMonID] = [TMONSTERCHART].[wID]");
}

void CTMonposSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX, _T("[TMONSTERCHART].[szName2]"), m_szName2, 50);
	RFX_Text(pFX, _T("[TMONSTERCHART].[szName]"), m_szName, 50);
	RFX_Int(pFX, _T("[TMONSTERCHART].[wKind]"), m_wKind);
	RFX_Int(pFX, _T("[TMAPMONCHART].[wMonID]"), m_wMonID);
	RFX_Int(pFX, _T("[TMONSPAWNCHART].[wMapID]"), m_wMapID);
	RFX_Single(pFX, _T("[TMONSPAWNCHART].[fPosX]"), m_fPosX);
	RFX_Single(pFX, _T("[TMONSPAWNCHART].[fPosY]"), m_fPosY);
	RFX_Single(pFX, _T("[TMONSPAWNCHART].[fPosZ]"), m_fPosZ);
}

/////////////////////////////////////////////////////////////////////////////
// CTMonposSet Áø´Ü

#ifdef _DEBUG
void CTMonposSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTMonposSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
