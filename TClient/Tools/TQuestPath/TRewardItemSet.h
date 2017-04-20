#pragma once


class CTRewardItemSet : public CRecordset
{
public:
	CTRewardItemSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTRewardItemSet)

	long m_dwTriggerID;
	long m_dwRewardID;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange *pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
