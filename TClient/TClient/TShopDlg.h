#pragma once

class CTShopDlg : public CTShopBaseDlg
{
protected:
	TSHOP_TYPE m_eShopType;
	TSHOPPHURCHASETYPE m_ePhurchaseType;

public:
	LPTOPENBYCASH m_pOpenByCash;
	void SetOpenByCash( LPTOPENBYCASH );
	void ClearOpenByCash();

public:
	TSHOP_TYPE GetType() const { return m_eShopType; }
	TSHOPPHURCHASETYPE GetPhurchaseType() { return m_ePhurchaseType; }
	void Clear();

	void SetPhurchaseType( TSHOPPHURCHASETYPE );

public:
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );
	
	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc, TSHOP_TYPE eShopType);
	virtual ~CTShopDlg();
};

