#pragma once

class CTGuildCabinetDlg : public CTCabinetDlg
{
public:
	enum { MAX_ITEM_SLOT = 12 };
	enum { MAX_ITEM_COLUMN = 2 };
	
protected:
	TComponent*	m_pGetBtn;
	TComponent* m_pCancelBtn;

public:
	virtual void SetupComponent();
	virtual void RequestShowComponent();

public:
	CTGuildCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildCabinetDlg();
};