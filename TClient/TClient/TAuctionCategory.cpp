#include "stdafx.h"


CTCategoryEntity::CTCategoryEntity()
:	m_bExpand( FALSE ),
	m_bCheck( FALSE ),
	m_pItem( NULL )
{
	m_vChild.clear();
}

CTCategoryEntity::~CTCategoryEntity()
{
	size_t i = 0;
	size_t size = m_vChild.size();

	for( ; i < size; ++i )
	{
		delete m_vChild[i];
		m_vChild[i] = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// MEMBER FUNCTION IMPLEMENTATION.
BOOL	CTCategoryEntity::FindAdd( LPTAUCTIONTREE pTreeInfo )
{
	if( !pTreeInfo )
		return FALSE;

	if( m_TreeInfo.m_ID.dwValue == pTreeInfo->m_ID.dwValue )
		return TRUE;

	if( pTreeInfo->m_ID.Depth &&
		!pTreeInfo->m_ID.Class &&
		!pTreeInfo->m_ID.KindID )
	{
		CTCategoryEntity* pEntity = new CTCategoryEntity;
		pEntity->m_TreeInfo = *pTreeInfo;
		pEntity->SetLevel( CTAuctionSearch::SC_TREE_DEPTH1 );
		m_vChild.push_back( pEntity );

		return TRUE;
	}
	else if( (pTreeInfo->m_ID.Depth &&
		pTreeInfo->m_ID.Class &&
		!pTreeInfo->m_ID.KindID) &&
		m_dwTreeLevel == CTAuctionSearch::SC_TREE_DEPTH1 )
	{
		if( m_TreeInfo.m_ID.Depth &
			pTreeInfo->m_ID.Depth )
		{
			CTCategoryEntity* pEntity = new CTCategoryEntity;
			pEntity->m_TreeInfo = *pTreeInfo;
			pEntity->m_TreeInfo.m_ID.Depth = m_TreeInfo.m_ID.Depth;
			pEntity->SetLevel( CTAuctionSearch::SC_TREE_DEPTH2 );
			m_vChild.push_back( pEntity );

			return TRUE;
		}
	}
	else if( (pTreeInfo->m_ID.Depth &&
		pTreeInfo->m_ID.Class &&
		pTreeInfo->m_ID.KindID) )
	{
		if( (m_TreeInfo.m_ID.Depth &
			pTreeInfo->m_ID.Depth) &&
			(m_TreeInfo.m_ID.Class &
			pTreeInfo->m_ID.Class) )
		{
			CTCategoryEntity* pEntity = new CTCategoryEntity;
			pEntity->m_TreeInfo = *pTreeInfo;
			pEntity->m_TreeInfo.m_ID.Class = m_TreeInfo.m_ID.Class;
			pEntity->SetLevel( CTAuctionSearch::SC_TREE_DEPTH3 );
			m_vChild.push_back( pEntity );

			return TRUE;
		}
	}
	else if( pTreeInfo->m_ID.Depth &&
		!pTreeInfo->m_ID.Class &&
		pTreeInfo->m_ID.KindID )
	{
		if( m_TreeInfo.m_ID.Depth == pTreeInfo->m_ID.Depth )
		{
			CTCategoryEntity* pEntity = new CTCategoryEntity;
			pEntity->m_TreeInfo = *pTreeInfo;
			pEntity->SetLevel( CTAuctionSearch::SC_TREE_DEPTH2 );
			m_vChild.push_back( pEntity );

			return TRUE;
		}
	}


	size_t size = m_vChild.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		m_vChild[i]->FindAdd( pTreeInfo );
	}

	return FALSE;
}

void	CTCategoryEntity::Attach( CTCategoryEntity* pEntity )
{
	if( pEntity )
	{
		m_vChild.push_back( pEntity );
	}
}

void	CTCategoryEntity::AllAddItem( CTCtrlList* pList )
{
	if( pList )
	{
		if( m_TreeInfo.m_ID.dwValue )
		{
			CTSearchItem* pItem = new CTSearchItem;
			pItem->SetEntity( this );
			pList->AddItem( pItem );
		}

		if( m_bExpand )
		{
			size_t i = 0;
			size_t size = m_vChild.size();

			for( ; i < size; ++i )
				m_vChild[i]->AllAddItem( pList );
		}
	}
}

void	CTCategoryEntity::SetCheck_Child( BOOL bCheck )
{
	size_t i = 0;
	size_t size = m_vChild.size();

	for( ; i < size; ++i )
	{
		m_vChild[i]->SetCheckState( bCheck );
		m_vChild[i]->SetCheck_Child( bCheck );
	}
}

void	CTCategoryEntity::SetExpand_Child( BOOL bExpand )
{
	size_t i = 0;
	size_t size = m_vChild.size();

	for( ; i < size; ++i )
	{
		m_vChild[i]->SetExpand( bExpand );
		m_vChild[i]->SetExpand_Child( bExpand );
	}
}

LPVTDWORD	CTCategoryEntity::GetCheckList_Child( LPVTDWORD pVTDWORD )
{
	if( m_bCheck && 
		m_TreeInfo.m_ID.Depth &&
		m_TreeInfo.m_ID.KindID )
	{
		pVTDWORD->push_back( m_TreeInfo.m_ID.dwValue );
	}

	size_t i = 0;
	size_t size = m_vChild.size();
	for( ; i < size; ++i )
	{
		m_vChild[i]->GetCheckList_Child( pVTDWORD );
	}

	return pVTDWORD;
}
// END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION.
void	CTCategoryEntity::SetItem( CTSearchItem* pItem )
{
	m_pItem = pItem;
}

void	CTCategoryEntity::SetID( DWORD dwID )
{
	m_TreeInfo.m_ID.dwValue = dwID;
}

void	CTCategoryEntity::SetName( CString strName )
{
	m_TreeInfo.m_strName = strName;
}

void	CTCategoryEntity::SetCheckState( BOOL bCheck )
{
	m_bCheck = bCheck;
}

void	CTCategoryEntity::SetLevel( DWORD dwLevel )
{
	m_dwTreeLevel = dwLevel;
}

void	CTCategoryEntity::SetExpand( BOOL bExpand )
{
	m_bExpand = bExpand;
}

CTSearchItem*	CTCategoryEntity::GetItem()
{
	return m_pItem;
}

DWORD	CTCategoryEntity::GetID()
{
	return m_TreeInfo.m_ID.dwValue;
}

CString	CTCategoryEntity::GetName()
{
	return m_TreeInfo.m_strName;
}
BOOL	CTCategoryEntity::GetCheckState()
{
	return m_bCheck;
}

DWORD	CTCategoryEntity::GetLevel()
{
	return m_dwTreeLevel;
}

size_t	CTCategoryEntity::GetChildSize()
{
	return m_vChild.size();
}

void	CTCategoryEntity::OpenCategory()
{
	m_bExpand = TRUE;
}

void	CTCategoryEntity::CloseCategory()
{
	m_bExpand = FALSE;
}

BOOL	CTCategoryEntity::IsExpand()
{
	return m_bExpand;
}
// END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



CTSearchCategory::CTSearchCategory()
:	m_bNeedUpdate( TRUE )
{
	m_pRootEntity = new CTCategoryEntity;
	m_pRootEntity->OpenCategory();
	m_pRootEntity->SetID( 0x00000000 );
}

CTSearchCategory::~CTSearchCategory()
{
	size_t i = 0;
	size_t size = m_vEntity.size();

	for( ; i<size; ++i )
	{
		delete m_vEntity[i];
		m_vEntity[i] = NULL;
	}

	if( m_pRootEntity )
	{
		delete m_pRootEntity;
		m_pRootEntity = NULL;
	}
}

void	CTSearchCategory::BuildCategory()
{
	size_t size = CTChart::m_vTAUCTIONTREE.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		m_pRootEntity->FindAdd( CTChart::m_vTAUCTIONTREE[i] );
	}

	NotifyUpdate();
}

void	CTSearchCategory::AddEntity( CTCategoryEntity* pEntity )
{
	if( pEntity )
	{
		m_vEntity.push_back( pEntity );

		NotifyUpdate();
	}
}

void	CTSearchCategory::Update( CTCtrlList* pList )
{
	if( m_bNeedUpdate )
	{
		INT nScrollPos = pList->GetScrollPos();
		pList->ClearItem();

		size_t i = 0;
		size_t size = m_vEntity.size();
		
//		for( ; i < size; ++i )
//			m_vEntity[i]->AllAddItem( pList );

		m_pRootEntity->AllAddItem( pList );

		pList->SetScroll( 400, nScrollPos );
	}

	m_bNeedUpdate = FALSE;
}

////////////////////////////////////////////////////////////////
// INLINE FUNCTION.
CTCategoryEntity*	CTSearchCategory::GetRootEntity()
{
	return m_pRootEntity;
}

void	CTSearchCategory::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// END OF INLINE FUNCTION.
////////////////////////////////////////////////////////////////

