#pragma once

class CTPortalShopSlot;
class CTPortalShopItem;

typedef vector<CTPortalShopItem*>	VTPORTALSHOPITEM,	*LPVTPORTALSHOPITEM;
typedef map< WORD, LPVTPORTALSHOPITEM >	MAPLPVTPORTALSHOPITEM,	*LPMAPVTPORTALSHOPITEM;

class CTPortalShopSlot : public CTCtrlListSlot
{
public:
		TComponent*		m_pTitleImage;
		TComponent*		m_pTitleText;

		TComponent*		m_pPortalText;
		TComponent*		m_pGoldImage;
		TComponent*		m_pRune;
		TComponent*		m_pLuna;
		TComponent*		m_pCron;
		TButton*		m_pButton;

		BYTE			m_bItemType;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);

public:
	CTPortalShopSlot()
		: m_pTitleImage(NULL), m_pTitleText(NULL), m_pPortalText(NULL), m_pGoldImage(NULL),
		  m_pRune(NULL), m_pLuna(NULL), m_pCron(NULL), m_pButton(NULL) {}
		virtual ~CTPortalShopSlot() {}
};

// Portal Item
class CTPortalShopItem : public CTCtrlListItem
{
public:
	enum
	{
		TPORTALSHOPITEM_TYPE_NONE = 0 ,
		TPORTALSHOPITEM_TYPE_TITLE,
		TPORTALSHOPITEM_TYPE_PORTAL,
		TPORTALSHOPITEM_TYPE_COUNT
	};

	BYTE		m_bItemType;

	LPTPORTAL	m_pTPortal;
	DWORD		m_dwRealPrice;

	CString		m_strRegion;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public:
	CTPortalShopItem() : m_bItemType(TPORTALSHOPITEM_TYPE_NONE), m_pTPortal(NULL), m_dwRealPrice(0) {}
	virtual ~CTPortalShopItem();
};


class CTPortalShopDlg : public CTClientUIBase
{
public:
	enum { MAX_ITEM_SLOT = 15 };
	enum { MAX_ITEM_COLUMN = 1 };

public:

	TButton*		m_pOk;
	TButton*		m_pCancel;
	
	TComponent*		m_pSCROLL;
	TComponent*		m_pRegion;

	TButton*		m_pSlotBtn;
	TComponent*		m_pPotralName;
	TComponent*		m_pLuna;
	TComponent*		m_pRune;
	TComponent*		m_pCron;

	CTCtrlList*		m_pList;

	MAPLPVTPORTALSHOPITEM	m_mapVTPORTALSHOPITEM;

public: 
	void AddPortal( WORD wPortalID, DWORD dwPrice, DWORD dwRealPrice );
	void AlignPortalList();
	void ClearList();

	CTCtrlList* GetList() const		{ return m_pList; }

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	
public:
	CTPortalShopDlg( TComponent *pParent, LP_FRAMEDESC pDesc );
	~CTPortalShopDlg(void);
};
