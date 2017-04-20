#pragma once

class CTCashCabinetDlg : public CTCabinetDlg
{
public:
	enum { MAX_ITEM_SLOT = 12 };
	enum { MAX_ITEM_COLUMN = 2 };
	
protected:
	TComponent* m_pExtensionPeriodBtn;
	TComponent*	m_pGetBtn;
	TComponent* m_pCancelBtn;
	TComponent* m_pRemainDayItemCountText;
	TComponent* m_pCashText;

	DWORD m_dwTotal;
	BOOL m_bChargeTwinkle;
public:
	void SetInfoCashCabinet( 
		CTime dEndTime,
		DWORD dwCount,
		DWORD dwCash,
		DWORD dwBonusCash );

	BOOL IsPeriodRental() { return m_dwRemainDays != 0; }

	void UpdateChargeBtnTwinkle(DWORD dwTick);

	virtual void SetupComponent();
	virtual void RequestShowComponent();

	virtual BOOL AddItem(UINT nCab, CTCabinetItem* pItem);
	void ReCountItem();

	virtual HRESULT Render(DWORD dwTickCount);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );

public:
	DWORD m_dwRemainDays;
	CTime m_dEndTime;
	DWORD m_dwItemCount;
	DWORD m_dwCash;
	DWORD m_dwBonusCash;
	BOOL m_bRequestShowCashCabinet;

public:
	CTCashCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCashCabinetDlg();
};