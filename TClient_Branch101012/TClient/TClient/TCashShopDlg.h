#pragma once

class CTCashShopSlot;
class CTCashShopItem;

class CTCashShopSlot : public CTCtrlListSlot
{
public:
	TImageList*		m_pIcon;
	TComponent*		m_pName;
	TComponent*		m_pCash;

public:
	BYTE					m_bKind;
	TComponent*		m_pBestIcon;
	TComponent*		m_pNewIcon;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);
	
public:
	CTCashShopSlot()
		: m_pIcon(NULL), m_pName(NULL), m_pCash(NULL),
		m_bKind(0), m_pBestIcon(NULL), m_pNewIcon(NULL) {}
	virtual ~CTCashShopSlot() {}
};

class CTCashShopItem : public CTCtrlListItem
{
public:
	CTClientItem*	m_pClientItem;
	BYTE			m_bItemCnt;
	WORD			m_wCashItemID;
	WORD			m_wItemID;
	DWORD			m_dwPrice;
	WORD			m_wUseDay;
	BYTE				m_bKind;

public:
	CTCashShopItem* Clone();

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);
	
public:
	CTCashShopItem() : m_pClientItem(NULL), m_wUseDay(0) {}
	virtual ~CTCashShopItem();
};

typedef std::vector< CTCashShopItem* > VTCASHSHOPITEM;

struct CASHEXTENDPERIODINFO
{
	WORD wItemID;
	BYTE bInvenID;
	BYTE bSlotID;
};