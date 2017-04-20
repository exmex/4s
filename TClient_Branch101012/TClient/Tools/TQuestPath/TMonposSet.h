#pragma once


class CTMonposSet : public CRecordset
{
public:
	CTMonposSet( CDatabase *pDatabase = NULL);
	DECLARE_DYNAMIC(CTMonposSet)

	CStringA m_szName2;
	CStringA m_szName;

	int m_wMonID;
	int m_wMapID;
	int m_wKind;

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
