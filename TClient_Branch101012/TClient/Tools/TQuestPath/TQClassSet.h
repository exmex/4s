#pragma once


class CTQClassSet : public CRecordset
{
public:
	CTQClassSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTQClassSet)

	CStringA m_szNAME;
	long m_dwClassID;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange *pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
