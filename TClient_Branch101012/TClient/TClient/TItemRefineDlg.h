#pragma once

class CTItemRefineDlg : public CTItemSlotBaseDlg
{
public:
	enum 
	{
		TARGET_SLOT,
		RESOURCE_SLOT1,
		RESOURCE_SLOT2,
		RESOURCE_SLOT3,
		SLOT_COUNT
	};

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

private :

	virtual void	UpdateState_ButtonOK();

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;

public:
	CTItemRefineDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemRefineDlg();
};
