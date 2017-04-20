#pragma once

class CTCtrlListItem;
class CTCtrlListSlot;
class CTCtrlList;

typedef std::vector<CTCtrlListItem*> CTCLItemArray;
typedef std::vector<CTCtrlListSlot*> CTCLSlotArray;

class CTCtrlListItem
{
public:
	UINT			m_nIndex;
	BOOL			m_bMark;

public:
	virtual void ReflectSlot(CTCtrlListSlot* pSlot) = 0;

public:
	UINT GetIndex() const { return m_nIndex; }

public:
	CTCtrlListItem() : m_nIndex(T_INVALID), m_bMark(FALSE) {}
	virtual ~CTCtrlListItem() {}
};

class CTCtrlListSlot
{
public:
	UINT			m_nSlotIndex;
	BOOL			m_bShow;
	CTCtrlListItem*	m_pItem;
	TComponent*		m_pBase;
	DWORD m_dwUserData;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel) = 0;
	virtual void Update(CTCtrlListItem* pItem);
	
public:
	void SetSlot(INT nSlot, TComponent* pBase)
	{
		m_nSlotIndex = nSlot; 
		m_pBase = pBase;
	}

	UINT GetSlotIndex() const			{ return m_nSlotIndex; }
	BOOL IsVisible() const				{ return m_bShow; }
	CTCtrlListItem* GetItem() const		{ return m_pItem; }

public:
	CTCtrlListSlot() : m_nSlotIndex(0), m_bShow(TRUE), m_pItem(NULL), m_pBase(NULL), m_dwUserData(0) {}
	virtual ~CTCtrlListSlot() {}
};

class CTCtrlList
{
protected:
	CTCLItemArray	m_vItems;
	CTCLSlotArray	m_vSlots;

	TScroll*		m_pScroll;
	UINT			m_nSlotPerScroll;
	BOOL			m_bNeedUpdate;
	INT				m_nPrvScrPos;

	INT				m_nSelectItemIdx;
	INT				m_nWheelMoveValue;
	
public:
	virtual void NotifyUpdate();
	virtual void SetupScroll(TScroll* pScroll, UINT nSlotPerScroll);

	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(INT nIdx);
	virtual void Update();

public:
	void SetSlot(CTCLSlotArray* pSlots);
	void ClearSlot();
	INT GetSlotCnt() const;
	CTCtrlListSlot* GetSlot(INT nSlot) const;
	CTCtrlListSlot* GetSlotNotUpdate( INT nSlot ) const;
	
	INT AddItem(CTCtrlListItem* pItem);
	void SetItem(INT nIdx, CTCtrlListItem* pItem);
	void ClearItem();
	void RemoveItem(INT nIdx);
	INT GetItemCnt() const;
	BOOL IsItemEmpty() const;

	CTCtrlListItem* GetItem(INT nIdx) const;
	CTCtrlListItem* GetSelectItem() const;
	INT GetSelectItemIndex() const { return m_nSelectItemIdx; }
	CTCtrlListItem* GetHitItem(const CPoint& pt);
	
	void SetAllSlotUserData( DWORD dwUserData );

public:
	INT GetScrollPos();
	INT GetScrollRng();

	void SetScroll(INT nRng, INT nPos);

public:
	CTCtrlList();
	virtual ~CTCtrlList();
};






















