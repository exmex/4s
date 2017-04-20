#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////
// AUCTION LIST CONSTRUCTOR & DESTRUCTOR IMPLEMENTATAION.
CTAuctionCtrlList::CTAuctionCtrlList()
:	m_bNeedUpdate( FALSE ),
	m_nCurPage( 1 ),
	m_nLastPage( 1 ),
	m_nSelectedItemIdx( T_INVALID )
{
	ClearItems();
	ClearSlots();
}

CTAuctionCtrlList::~CTAuctionCtrlList()
{
	ClearItems();
	ClearSlots();
}
//	END OF AUCTION LIST CON.. & DEST...
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
CTCtrlListItem* CTAuctionCtrlList::AddItem( CTCtrlListItem* pItem )
{
	if( !pItem )
		return NULL;

	size_t index = m_vItems.size();
	pItem->m_nIndex = index;
	m_vItems.push_back( pItem );
	NotifyUpdate();

	return m_vItems[ index ];
}

CTCtrlListSlot* CTAuctionCtrlList::AddSlot( CTCtrlListSlot* pSlot )
{
	if( !pSlot )
		return NULL;

	size_t index = m_vSlots.size();
	pSlot->m_nSlotIndex = index;
	m_vSlots.push_back( pSlot );
	NotifyUpdate();

	return m_vSlots[ index ];
}

BOOL CTAuctionCtrlList::NextPage()
{
	Select( T_INVALID );

	INT nItemsPerPage = (INT)m_vSlots.size();
	if( m_nCurPage * nItemsPerPage < m_vItems.size() )
	{
		++m_nCurPage;
		NotifyUpdate();

		return TRUE;
	}

	m_nCurPage = 1;

	NotifyUpdate();

	return FALSE;
}

BOOL CTAuctionCtrlList::PrevPage()
{
	Select( T_INVALID );

	INT nItemsPerPage = (INT)m_vSlots.size();

	--m_nCurPage;

	if( m_nCurPage * nItemsPerPage > 0 )
	{
		NotifyUpdate();

		return TRUE;
	}

	m_nCurPage = TAUCTION_MAXPAGE_COUNT;

	NotifyUpdate();

	return FALSE;
}

BOOL CTAuctionCtrlList::MoveFirstPage()
{
	Select( T_INVALID );

	m_nCurPage = 1;

	NotifyUpdate();

	return FALSE;
}

BOOL CTAuctionCtrlList::MoveLastPage()
{
	Select( T_INVALID );

	m_nCurPage = m_nLastPage;

	NotifyUpdate();

	return FALSE;
}

void CTAuctionCtrlList::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}

void CTAuctionCtrlList::Update()
{
	if( !m_bNeedUpdate )
		return ;

	UINT nSlotSize = (INT)m_vSlots.size();
	UINT nFirstIndex = ( m_nCurPage - 1) * nSlotSize;

	UINT cnt = 0;
	UINT idx = nFirstIndex;
	for( ; idx < nFirstIndex + nSlotSize; ++idx, ++cnt )
	{
		CTCtrlListItem* pItem = NULL;

		if( idx < m_vItems.size() )
			pItem = m_vItems[ idx ];

		m_vSlots[ cnt ]->Select( cnt == m_nSelectedItemIdx );
		m_vSlots[ cnt ]->Update( pItem );
	}

	m_bNeedUpdate = FALSE;
}

void CTAuctionCtrlList::ShowComponent( BOOL bVisible )
{
	size_t size = m_vSlots.size();
	size_t i = 0;

	for( ; i < size; ++i )
		m_vSlots[i]->ShowComponent( bVisible );
}

void CTAuctionCtrlList::Render()
{
	Update();
}


void CTAuctionCtrlList::ClearItems()
{
	size_t size = m_vItems.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		if( m_vItems[i] )
		{
			delete m_vItems[i];
			m_vItems[i] = NULL;
		}
	}
	m_vItems.clear();

	m_nSelectedItemIdx = T_INVALID;

	NotifyUpdate();
}

void CTAuctionCtrlList::ClearSlots()
{
	size_t size = m_vSlots.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		if( m_vSlots[i] )
		{
			delete m_vSlots[i];
			m_vSlots[i] = NULL;
		}
	}
	m_vSlots.clear();

	m_nSelectedItemIdx = T_INVALID;

	NotifyUpdate();
}

CTCtrlListItem* CTAuctionCtrlList::GetHitItem( CPoint pt )
{
	size_t size = m_vSlots.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		if( m_vSlots[i]->m_pBase->HitTest( pt ) )
			return m_vSlots[i]->m_pItem;
	}

	return NULL;
}
//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
void CTAuctionCtrlList::Select( UINT idx )
{
	m_nSelectedItemIdx = idx;
	NotifyUpdate();
}

void CTAuctionCtrlList::SetSlot( CTCLSlotArray* pSlots )
{
	ClearSlots();

	if( pSlots )
		m_vSlots.swap( *pSlots );

	NotifyUpdate();
}

void CTAuctionCtrlList::SetCurPage( INT nPage )
{
	m_nCurPage = nPage;

	NotifyUpdate();
}

void CTAuctionCtrlList::SetLastPage( INT nLastPage )
{
	m_nLastPage = nLastPage;

	NotifyUpdate();
}

UINT	CTAuctionCtrlList::GetSelectedIndex()
{
	return m_nSelectedItemIdx;
}

CTCtrlListItem* CTAuctionCtrlList::GetSelectedItem()
{
	if( m_nSelectedItemIdx == T_INVALID )
		return NULL;

	return m_vItems[ (m_nCurPage-1) * m_vSlots.size() + m_nSelectedItemIdx ];
}

CTCtrlListItem*	CTAuctionCtrlList::GetItem( size_t index )
{
	return m_vItems[ index ];
}

CTCtrlListSlot*	CTAuctionCtrlList::GetSlot( size_t index )
{
	return m_vSlots[ index ];
}

size_t	CTAuctionCtrlList::GetSlotCount()
{
	return m_vSlots.size();
}

INT	CTAuctionCtrlList::GetCurPage()
{
	return m_nCurPage;
}

INT	CTAuctionCtrlList::GetLastPage()
{
	return m_nLastPage;
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
