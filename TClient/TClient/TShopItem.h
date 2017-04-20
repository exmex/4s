#pragma once

class CTTradeItem : public CTCtrlListItem
{
public:
	CTClientItem*	m_pClientItem;
	BYTE			m_bInven;
	BYTE			m_bInvenSlot;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);
	
public:
	CTTradeItem() : m_pClientItem(NULL) {}
	virtual ~CTTradeItem();
};

class CTShopNpcItem : public CTCtrlListItem
{
public:
	LPTITEM			m_pTItem;
	DWORD			m_dwRealPrice;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public:
	CTShopNpcItem() : m_pTItem(NULL), m_dwRealPrice(0) {}
};

class CTShopPortal : public CTCtrlListItem
{
public:
	LPTPORTAL		m_pTPortal;
	DWORD			m_dwRealPrice;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public:
	CTShopPortal() : m_pTPortal(NULL), m_dwRealPrice(0) {}
};

class CTPrivateShopItem : public CTCtrlListItem
{
public:
	BYTE			m_bNetID;
	CTClientItem*	m_pClientItem;
	BYTE			m_bItemCnt;
	DWORD			m_dwRune;
	DWORD			m_dwLuna;
	DWORD			m_dwCron;
	BYTE			m_bInven;
	BYTE			m_bInvenSlot;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);
	
public:
	CTPrivateShopItem() : m_bNetID(T_INVALID), m_pClientItem(NULL) {}
	virtual ~CTPrivateShopItem();
};

class CTRentNpcItem : public CTCtrlListItem
{
public :
	LPTSKILL	m_pSkill;
	DWORD		m_dwRealPrice;

public :
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public :
	CTRentNpcItem() : m_pSkill(NULL), m_dwRealPrice(0) {}
};

class CTMonsterShopItem : public CTCtrlListItem
{
public :

	LPTMONSHOP	m_pTMon;
	DWORD		m_dwRealPrice;

public :
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public :
	CTMonsterShopItem() : m_pTMon( NULL ), m_dwRealPrice( 0 ) {}
	virtual ~CTMonsterShopItem() {}
};

#ifdef MAGICITEMSHOP
class CTMagicShopItem : public CTCtrlListItem
{
public :

	CTClientItem*	m_pClientItem;
	DWORD			m_dwRealPrice;
	DWORD			m_dwMagicItemID;

public :
	virtual void ReflectSlot(CTCtrlListSlot* pSlot);

public :
	CTMagicShopItem() : m_pClientItem( NULL ), m_dwRealPrice( 0 ), m_dwMagicItemID( 0 ) {}
	virtual ~CTMagicShopItem();
};
#endif