#include "stdafx.h"
#include "TNpcposSet.h"


IMPLEMENT_DYNAMIC(CTNpcposSet, CRecordset)

CTNpcposSet::CTNpcposSet( CDatabase *pdb)
:CRecordset(pdb)
{
	m_szName2 = "";
	m_szName = "";

	m_wMapID = 0;
	m_wID = 0;

	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;

	m_nDefaultType = snapshot;
	m_nFields = 7;
}

CString CTNpcposSet::GetDefaultConnect()
{
	return _T("DSN=TGAME");
}

CString CTNpcposSet::GetDefaultSQL()
{
	return _T("[dbo].[TNPCCHART]");
}

void CTNpcposSet::DoFieldExchange( CFieldExchange *pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX, _T("[NC_szName2]"), m_szName2, 50);
	RFX_Text(pFX, _T("[szName]"), m_szName, 50);
	RFX_Int(pFX, _T("[wMapID]"), m_wMapID);
	RFX_Int(pFX, _T("[wID]"), m_wID);
	RFX_Single(pFX, _T("[fPosX]"), m_fPosX);
	RFX_Single(pFX, _T("[fPosY]"), m_fPosY);
	RFX_Single(pFX, _T("[fPosZ]"), m_fPosZ);
}

/////////////////////////////////////////////////////////////////////////////
// CTNpcposSet Áø´Ü

#ifdef _DEBUG
void CTNpcposSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTNpcposSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
