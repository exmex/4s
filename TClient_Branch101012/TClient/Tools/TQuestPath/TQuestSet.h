#pragma once


class CTQuestSet : public CRecordset
{
public:
	CTQuestSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTQuestSet)

	CStringA m_szComplete;
	CStringA m_szMessage;
	CStringA m_szTitle;

	long m_dwTriggerID;
	long m_dwQuestID;
	long m_dwClassID;

	BYTE m_bTriggerType;
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
