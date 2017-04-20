#pragma once


class CTQClassSet : public CRecordset
{
public:
	CTQClassSet( CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTQClassSet)

	long m_dwClassID;
	CString m_szNAME;
	BYTE m_bClassMain;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange* pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
