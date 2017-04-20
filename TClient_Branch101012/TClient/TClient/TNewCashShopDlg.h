#pragma once

#include "TCashShopDlg.h"

class CTCashShopCategory
{
public:
	static TComponent* m_pTempCategoryBar;
	static TComponent* m_pTempCategoryName;
	static TComponent* m_pTempCategoryPlusBtn;
	static TComponent* m_pTempCategoryMinusBtn;
	static CPoint m_ptPlusBtnOffset;
	static CPoint m_ptMinusBtnOffset;
	static CPoint m_ptNameBtnOffset;

public:
	CTCashShopCategory();
	~CTCashShopCategory();
	
	void DeleteAllItem();
	void SetCategoryPos( CPoint ptBase );
	void SetName( CString strName );
	INT CheckExpandBtn( CPoint pt );
	void ShowComponent( BOOL bVisible );
	INT GetItemCount();

public:
	DWORD m_dwID;

	VTCASHSHOPITEM m_vCashShopItems;

	TComponent* m_pPlusBtn;
	TComponent* m_pMinusBtn;
	TComponent* m_pName;
	TComponent* m_pCategoryBar;

	BOOL m_bOpened;
};

class CTNewCashShopDlg : public CTClientUIBase
{
public:
	static CASHEXTENDPERIODINFO m_ExtendPeriodInfo;
	static CRect m_rtNewCategory;

public:
	enum { MAX_NEW_ITEM_SLOT = 4 };
	enum { MAX_ITEM_SLOT = 6 };
	enum { MAX_ITEM_COLUMN = 2 };

	CTNewCashShopDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTNewCashShopDlg();

	void SetCash( DWORD dwCash, DWORD dwBonusCash );

	void ClearAllCategory();
	void AddCategory( DWORD dwCategoryID, CString strName );
	void DeleteCategory( DWORD dwCategoryID );
	CTCashShopCategory* GetCategory( DWORD dwCategoryID );
	
	void AddItemToNewCategory( CTCashShopItem* );
	void AddItemToCategory( DWORD dwCategoryID, CTCashShopItem* );
	
	void OpenCategory( DWORD dwCategoryID );
	void OpenTopCategory();
	
	BOOL ResetPos();
	void ClearListComponent();

	CTCashShopItem* GetSelectItem();

public:
	INT m_nMode;
	void SetCashShopDlgMode( INT );

	void SetPresent( BOOL bPresent );
	BOOL GetPresent() { return m_bPresent;	}


public:
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );

protected:
	MAPTCASHSHOPCATEGORY		m_mapCategory;
	VTCASHSHOPCATEGORY			m_vCategoryOrders;

	CTCtrlList*		m_pListNEW;
	CTCtrlList*		m_pList;

	TComponent*	m_pCashText;
	TButton*		m_pBuy;
	TButton*		m_pPresent;

	DWORD		m_dwCash;
	DWORD		m_dwCashBonus;

	VTFRAMEOFFSET		m_vItemListOffset;
	INT						m_nListHeight;
	TComponent*			m_pSCROLL;
	BOOL						m_bShowScroll;

	CPoint					m_ptBasePointCategory;
	INT						m_nCategoryHeight;
	BOOL						m_bNeedResetPos;
	BOOL					m_bPresent;

	// List Component
	TImageList*				m_pListCompIMAGE[ MAX_ITEM_SLOT ];
	TComponent*			m_pListCompNAME[ MAX_ITEM_SLOT ];
	TComponent*			m_pListCompCASH[ MAX_ITEM_SLOT ];
	TComponent*			m_pListCompBEST[ MAX_ITEM_SLOT ];
	TComponent*			m_pListCompNEW[ MAX_ITEM_SLOT ];

};