#pragma once



//!	Auction CtrlList.
class CTAuctionCtrlList
{
public :

	CTAuctionCtrlList();
	~CTAuctionCtrlList();

private :	// MEMBER DATA.

	CTCLItemArray			m_vItems;
	CTCLSlotArray			m_vSlots;

	BOOL					m_bNeedUpdate;
	INT						m_nCurPage;
	INT						m_nLastPage;
	UINT					m_nSelectedItemIdx;

public :	// MEMBER FUNCTION.

	CTCtrlListItem*			AddItem( CTCtrlListItem* pItem );
	CTCtrlListSlot*			AddSlot( CTCtrlListSlot* pSlot );

	BOOL					NextPage();
	BOOL					PrevPage();
	BOOL					MoveFirstPage();
	BOOL					MoveLastPage();

	void					NotifyUpdate();
	void					Update();
	void					ShowComponent( BOOL bVisible );
	void					Render();

	void					ClearItems();
	void					ClearSlots();
	CTCtrlListItem*			GetHitItem( CPoint pt );

public :	// INLINE FUNCTION.

	void					Select( UINT idx );
	void					SetSlot( CTCLSlotArray* pSlots );
	void					SetCurPage( INT nPage );
	void					SetLastPage( INT nLastPage );
	UINT					GetSelectedIndex();
	CTCtrlListItem*			GetSelectedItem();
	CTCtrlListItem*			GetItem( size_t index );
	CTCtrlListSlot*			GetSlot( size_t index );
	size_t					GetSlotCount();
	INT						GetCurPage();
	INT						GetLastPage();
};