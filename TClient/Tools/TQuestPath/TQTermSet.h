#pragma once


class CTQTermSet : public CRecordset
{
public:
	CTQTermSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTQTermSet)

	CStringA m_szObjective;

	BYTE m_bTermType;
	long m_dwTermID;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange *pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
