#pragma once

class CTItemRegGuildDlg : public CTItemSlotBaseDlg
{
public:
	enum 
	{
		TARGET_SLOT,
		SLOT_COUNT
	};

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();
	void SetRegMode( int nRegMode );

	int m_nRegMode;

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;

public:
	CTItemRegGuildDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemRegGuildDlg();
};