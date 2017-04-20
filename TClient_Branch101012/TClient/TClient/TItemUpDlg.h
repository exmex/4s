#pragma once

class CTItemUpDlg : public CTItemSlotBaseDlg
{
public:
	enum 
	{
		TARGET_SLOT,
		SCROLL_SLOT,
		SLOT_COUNT
	};

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

public:
	virtual BOOL SetItem(BYTE bSlotID, LPTITEM pItem, BYTE bInven=T_INVALID, BYTE bInvenSlot=T_INVALID);
	BOOL CheckItem(CTClientItem* pTargetItem, CTClientItem* pScrollItem) const;
	BOOL CanUpgrade(LPTITEM pScrollItemTemp, LPTITEM pTargetItemTemp) const;
	virtual void ShowComponent( BOOL bVisible = 1 );

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;
	
public:
	CTItemUpDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemUpDlg();
};
