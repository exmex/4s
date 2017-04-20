#pragma once

class CTItemRepairDlg : public CTItemSlotBaseDlg
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

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;

public:
	CTItemRepairDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemRepairDlg();
};