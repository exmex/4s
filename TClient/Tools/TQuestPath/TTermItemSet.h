#pragma once


class CTTermItemSet : public CRecordset
{
public:
	CTTermItemSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTTermItemSet)

	long m_dwTriggerID;
	long m_dwQuestID;
	long m_dwTermID;

	BYTE m_bTriggerType;
	BYTE m_bTermType;
	BYTE m_bType;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange *pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
