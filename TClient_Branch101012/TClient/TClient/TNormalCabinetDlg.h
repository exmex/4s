#pragma once

class CTNormalCabinetDlg : public CTCabinetDlg
{
public:
	enum { MAX_ITEM_SLOT = 12 };
	enum { MAX_ITEM_COLUMN = 2 };

	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

public:
	virtual void SetupComponent();
	virtual void RequestShowComponent();
	virtual void SetCabinetInfo(UINT nCab, BOOL bOpen);
	virtual BOOL SelectCab(UINT nCab);
	virtual INT64 GetCabOpenCost(UINT nCab);
	virtual INT64 GetCabUseCost(UINT nCab);

public:
	CTNormalCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTNormalCabinetDlg();
};