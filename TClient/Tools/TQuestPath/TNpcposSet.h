#pragma once


class CTNpcposSet : public CRecordset
{
public:
	CTNpcposSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTNpcposSet)

	CStringA m_szName2;
	CStringA m_szName;

	int m_wMapID;
	int m_wID;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;

public:
	virtual CString GetDefaultConnect();

	virtual CString GetDefaultSQL();
	virtual void DoFieldExchange( CFieldExchange *pFX);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
