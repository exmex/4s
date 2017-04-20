#include "stdafx.h"
#include "TAuctionSearch.h"


//////////////////////////////////////////////////////////////////////////
// AUCTION SEARCH ITEM CLASS.
CTSearchItem::CTSearchItem()
{
	m_pEntity = NULL;
}

CTSearchItem::~CTSearchItem()
{
	m_pEntity = NULL;
}

void	CTSearchItem::ReflectSlot( CTCtrlListSlot* pSlot )
{
	if( !pSlot &&
		!m_pEntity )
		return ;

	CTSearchSlot* pSearchSlot = static_cast< CTSearchSlot* >( pSlot );

	DWORD dwTreeLevel = GetEntity()->GetLevel();

	// 확장버튼.
	if( m_pEntity->GetChildSize() )
	{
		BOOL bExpand = m_pEntity->IsExpand();
		pSearchSlot->m_pPlusBtn->ShowComponent( !bExpand );
		pSearchSlot->m_pPlusBtn->EnableComponent( !bExpand );
		pSearchSlot->m_pMinusBtn->ShowComponent( bExpand );
		pSearchSlot->m_pMinusBtn->EnableComponent( bExpand );
	}
	else
	{
		pSearchSlot->m_pPlusBtn->ShowComponent( FALSE );
		pSearchSlot->m_pPlusBtn->EnableComponent( FALSE );
		pSearchSlot->m_pMinusBtn->ShowComponent( FALSE );
		pSearchSlot->m_pMinusBtn->EnableComponent( FALSE );
	}

	CPoint ptBase;
	pSearchSlot->m_pBase->GetComponentPos( &ptBase );

	CPoint ptBtn_Offset = CTAuctionSearch::s_ptButton_Offset[ dwTreeLevel ];
	pSearchSlot->m_pPlusBtn->MoveComponent( ptBase + ptBtn_Offset );
	pSearchSlot->m_pMinusBtn->MoveComponent( ptBase + ptBtn_Offset );

	// 체크버튼.
	BOOL bCheck = m_pEntity->GetCheckState();
	pSearchSlot->m_pCheckBtn->Select( bCheck );
	pSearchSlot->m_pCheckBtn->ShowComponent( TRUE );
	pSearchSlot->m_pCheckBtn->EnableComponent( TRUE );

	// 이름.
	pSearchSlot->m_pText->m_strText = m_pEntity->GetName();
	pSearchSlot->m_pText->ShowComponent( TRUE );
	pSearchSlot->m_pText->EnableComponent( TRUE );

	CPoint ptString_Offset = CTAuctionSearch::s_ptString_Offset[ dwTreeLevel ];
	pSearchSlot->m_pText->MoveComponent( ptBase + ptString_Offset );
}

void	CTSearchItem::SetEntity( CTCategoryEntity* pEntity )
{
	m_pEntity = pEntity;
}

CTCategoryEntity*	CTSearchItem::GetEntity()
{
	return m_pEntity;
}
// END OF AUCTION SEARCH ITEM CLASS.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// SLOT CLASS.
CTSearchSlot::CTSearchSlot()
{
	m_pPlusBtn = NULL;
	m_pMinusBtn = NULL;
	m_pText = NULL;
	m_pCheckBtn = NULL;
}

CTSearchSlot::~CTSearchSlot()
{

}

void	CTSearchSlot::Select( BOOL bSel )
{
	static_cast< TButton* >( m_pBase )->Select( bSel );
}

void	CTSearchSlot::ShowComponent( BOOL bShow )
{
	m_bShow = bShow;

	if( m_bShow == FALSE )
	{
		m_pPlusBtn->ShowComponent( FALSE );
		m_pPlusBtn->EnableComponent( FALSE );
		m_pMinusBtn->ShowComponent( FALSE );
		m_pMinusBtn->EnableComponent( FALSE );
		m_pText->ShowComponent( FALSE );
		m_pText->EnableComponent( FALSE );
		m_pCheckBtn->ShowComponent( FALSE );
		m_pCheckBtn->ShowComponent( FALSE );
	}
}

BOOL	CTSearchSlot::OnLButtonDown( CPoint pt )
{
	if( m_pBase &&
		m_pItem &&
		m_pBase->HitTest( pt ) )
	{
		CTSearchItem* pItem = static_cast< CTSearchItem* >( m_pItem );

		if( m_pPlusBtn &&
			m_pPlusBtn->HitTest( pt ) )
		{
			m_pPlusBtn->ShowComponent( FALSE );
			m_pPlusBtn->EnableComponent( FALSE );

			m_pMinusBtn->ShowComponent( TRUE );
			m_pMinusBtn->EnableComponent( TRUE );

			pItem->GetEntity()->OpenCategory();

			return TRUE;
		}
		else if( m_pMinusBtn &&
			m_pMinusBtn->HitTest( pt ) )
		{
			m_pMinusBtn->ShowComponent( FALSE );
			m_pMinusBtn->EnableComponent( FALSE );

			m_pPlusBtn->ShowComponent( TRUE );
			m_pPlusBtn->EnableComponent( TRUE );

			pItem->GetEntity()->CloseCategory();

			return TRUE;
		}
		else if( m_pCheckBtn &&
			m_pCheckBtn->HitTest( pt ) )
		{
			BOOL bCheck = m_pCheckBtn->IsStateDown();
			pItem->GetEntity()->SetCheckState( bCheck );
			pItem->GetEntity()->SetCheck_Child( bCheck );

			return TRUE;
		}
	}

	return FALSE;
}
// END OF SLOT CLASS.
//////////////////////////////////////////////////////////////////////////


// AUCTION SEARCH DEFINITION.
TButton*     CTAuctionSearch::s_pPlusBtn[SC_TREE_DEPTH_COUNT] = { NULL };
TButton*     CTAuctionSearch::s_pMinusBtn[SC_TREE_DEPTH_COUNT] = { NULL };
TComponent*  CTAuctionSearch::s_pString[SC_TREE_DEPTH_COUNT] = { NULL };
CPoint       CTAuctionSearch::s_ptButton_Offset[SC_TREE_DEPTH_COUNT];
CPoint       CTAuctionSearch::s_ptString_Offset[SC_TREE_DEPTH_COUNT];
TButton*     CTAuctionSearch::s_pCheck = NULL;
CPoint       CTAuctionSearch::s_ptCheck_Offset = CPoint( 0, 0 );


//////////////////////////////////////////////////////////////////////////
// AUCTION SEARCH CLASS.

CTAuctionSearch::CTAuctionSearch( TComponent *pParent, LP_FRAMEDESC pDesc )
:	CTAuctionInnerFrame( pParent, pDesc, TAUCTION_REGIST ),
	m_pList( NULL )
{
	// Edit ID, Text ID, is NumberEdit?.
	DWORD dwEditID[ TEDIT_COUNT ][ 2 ] =
	{
		{ ID_CTRLINST_EDIT_SEARCH, FALSE },
		{ ID_CTRLINST_EDIT_NEEDLEVEL_MIN, TRUE },
		{ ID_CTRLINST_EDIT_NEEDLEVEL_MAX, TRUE },
		{ ID_CTRLINST_EDIT_UPGRADE_MIN, TRUE },
		{ ID_CTRLINST_EDIT_UPGRADE_MAX, TRUE }
	};

	size_t i = 0;
	for( ; i < TEDIT_COUNT; ++i )
	{
		if( dwEditID[i][1] ) // if NumberEdit.
		{
			TComponent* pComp = FindKid( dwEditID[i][0] );
			m_pEdit[i] = new CTNumberEdit( this, pComp->m_pDESC, TLEVEL_LENGTH );
			m_pEdit[i]->m_strText = "0";

			AddKid( m_pEdit[i] );
			RemoveKid( pComp );
			delete pComp;
		}
		else
		{
			m_pEdit[i] = static_cast< TEdit* >( FindKid( dwEditID[i][0] ) );
			m_pEdit[i]->m_strText = "0";
		}
	}

	m_pSearchBtn = static_cast< TButton* >( FindKid( ID_CTRLINST_BUTTON_SEARCH ) );
	m_pResetBtn = static_cast< TButton* >( FindKid( ID_CTRLINST_BUTTON_RESET ) );

	static DWORD dwCategoryBase[ CTAuctionSearch::MAX_ITEM_LIST ] =
	{
		ID_CTRLINST_SLOT1,
			ID_CTRLINST_SLOT2,
			ID_CTRLINST_SLOT3,
			ID_CTRLINST_SLOT4,
			ID_CTRLINST_SLOT5,
			ID_CTRLINST_SLOT6,
			ID_CTRLINST_SLOT7,
			ID_CTRLINST_SLOT8,
			ID_CTRLINST_SLOT9
	};

	static DWORD dwCategoryItem[ CTAuctionSearch::SC_TREE_DEPTH_COUNT ][ 3 ] =
	{
		{ ID_CTRLINST_TREE1_PLUS, ID_CTRLINST_TREE1_MINUS, ID_CTRLINST_TREE1_TEXT },
		{ ID_CTRLINST_TREE2_PLUS, ID_CTRLINST_TREE2_MINUS, ID_CTRLINST_TREE2_TEXT },
		{ ID_CTRLINST_TREE3_PLUS, ID_CTRLINST_TREE3_MINUS, ID_CTRLINST_TREE3_TEXT }
	};

	CTSearchSlot* pSlot;
	CTCLSlotArray vSlots;
	for( i=0; i<CTAuctionSearch::MAX_ITEM_LIST; ++i )
	{
		pSlot = new CTSearchSlot;
		pSlot->m_pBase = FindKid( dwCategoryBase[i] );
		vSlots.push_back( pSlot );
	}

	CTSearchSlot* pSlot0 = static_cast< CTSearchSlot* >( vSlots[0] );

	CPoint ptBase;
	pSlot0->m_pBase->GetComponentPos( &ptBase );
	CPoint ptTemp = CPoint( 0, 0 );

	for( i=0; i<CTAuctionSearch::SC_TREE_DEPTH_COUNT; ++i )
	{
		CTAuctionSearch::s_pPlusBtn[i] = static_cast< TButton* >( FindKid( dwCategoryItem[i][0] ) );
		CTAuctionSearch::s_pMinusBtn[i] = static_cast< TButton* >( FindKid( dwCategoryItem[i][1] ) );
		CTAuctionSearch::s_pString[i] = FindKid( dwCategoryItem[i][2] );

		CTAuctionSearch::s_pPlusBtn[i]->GetComponentPos( &ptTemp );
		CTAuctionSearch::s_ptButton_Offset[i] = ptTemp - ptBase;
		CTAuctionSearch::s_pString[i]->GetComponentPos( &ptTemp );
		CTAuctionSearch::s_ptString_Offset[i] = ptTemp - ptBase;

		RemoveKid( CTAuctionSearch::s_pPlusBtn[i] );
		RemoveKid( CTAuctionSearch::s_pMinusBtn[i] );
		RemoveKid( CTAuctionSearch::s_pString[i] );
	}

	CTAuctionSearch::s_pCheck = static_cast< TButton* >( FindKid( ID_CTRLINST_CHECKBOX ) );
	CTAuctionSearch::s_pCheck->GetComponentPos( &ptTemp );
	CTAuctionSearch::s_ptCheck_Offset = ptTemp - ptBase;
	RemoveKid( CTAuctionSearch::s_pCheck );

	for( i=0; i<CTAuctionSearch::MAX_ITEM_LIST; ++i )
	{
		pSlot = static_cast< CTSearchSlot* >( vSlots[i] );
		pSlot->m_pPlusBtn = new TButton( this, CTAuctionSearch::s_pPlusBtn[0]->m_pDESC );
		pSlot->m_pMinusBtn = new TButton( this, CTAuctionSearch::s_pMinusBtn[0]->m_pDESC );
		pSlot->m_pText = new TComponent( this, CTAuctionSearch::s_pString[0]->m_pDESC );
		pSlot->m_pCheckBtn = new TButton( this, CTAuctionSearch::s_pCheck->m_pDESC );

		pSlot->m_pPlusBtn->m_id = GetUniqueID();
		pSlot->m_pMinusBtn->m_id = GetUniqueID();
		pSlot->m_pText->m_id = GetUniqueID();
		pSlot->m_pCheckBtn->m_id = GetUniqueID();

		AddKid( pSlot->m_pPlusBtn );
		AddKid( pSlot->m_pMinusBtn );
		AddKid( pSlot->m_pText );
		AddKid( pSlot->m_pCheckBtn );

		pSlot->m_pBase->GetComponentPos( &ptBase );
		pSlot->m_pPlusBtn->MoveComponent( ptBase + CTAuctionSearch::s_ptButton_Offset[0] );
		pSlot->m_pMinusBtn->MoveComponent( ptBase + CTAuctionSearch::s_ptButton_Offset[0] );
		pSlot->m_pText->MoveComponent( ptBase + CTAuctionSearch::s_ptString_Offset[0] );
		pSlot->m_pCheckBtn->MoveComponent( ptBase + CTAuctionSearch::s_ptCheck_Offset );
	}

	m_pList = new CTCtrlList;
	m_pList->SetSlot( &vSlots );
	m_pList->SetupScroll( static_cast< TScroll* >( FindKid( ID_CTRLINST_SCROLL ) ), 1 );


	CTChart::InitTAUCTIONTREE( ".\\Tcd\\TAuctionTree.tcd" );
	m_pCategory = new CTSearchCategory;
	m_pCategory->BuildCategory();

	GetFindInfo()->Clear();
	ResetComp();
	UpdateByComp();
}


//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.
CTAuctionSearch::~CTAuctionSearch()
{
	if( m_pList )
	{
		delete m_pList;
		m_pList = NULL;
	}

	if( m_pCategory )
	{
		delete m_pCategory;
		m_pCategory = NULL;
	}

	size_t i = 0;
	for( ; i<CTAuctionSearch::SC_TREE_DEPTH_COUNT; ++i )
	{
		if( CTAuctionSearch::s_pPlusBtn[i] )
		{
			delete CTAuctionSearch::s_pPlusBtn[i];
			CTAuctionSearch::s_pPlusBtn[i] = NULL;
		}
		
		if( CTAuctionSearch::s_pMinusBtn[i] )
		{
			delete CTAuctionSearch::s_pMinusBtn[i];
			CTAuctionSearch::s_pMinusBtn[i] = NULL;
		}

		if( CTAuctionSearch::s_pString[i] )
		{
			delete CTAuctionSearch::s_pString[i];
			CTAuctionSearch::s_pString[i] = NULL;
		}
	}

	if( CTAuctionSearch::s_pCheck )
	{
		delete CTAuctionSearch::s_pCheck;
		CTAuctionSearch::s_pCheck = NULL;
	}
}

void CTAuctionSearch::RequestInfo()
{
	CTAuctionInnerFrame::RequestInfo();

	CTAuctionCommander::GetInstance()
		->RequestSearchList( GetFindInfo() );
}

void	CTAuctionSearch::RequestUpdatePage()
{
	CTAuctionInnerFrame::RequestUpdatePage();

	CTAuctionCommander::GetInstance()
		->RequestSearchList( GetFindInfo() );
}

void CTAuctionSearch::ResetInfo()
{
	m_pCategory->NotifyUpdate();
}

void CTAuctionSearch::UpdateEditable( CPoint pt )
{
	size_t i = 0;
	for( ; i < TEDIT_COUNT; ++i )
	{
		BOOL bHit = m_pEdit[i]->HitTest( pt );
		m_pEdit[i]->EnableComponent( bHit );
		m_pEdit[i]->SetFocus( bHit );
	}
}

void CTAuctionSearch::OnLButtonDown( UINT nFlags, CPoint pt )
{
	UpdateEditable( pt );

	if( !CanProcess() )
		return ;

	if( !HitTest( pt ) )
		return ;

	ITInnerFrame::OnLButtonDown( nFlags, pt );

	if( m_pSearchBtn->HitTest( pt ) )
	{
		DoSearch();
	}
	else if( m_pResetBtn->HitTest( pt ) )
	{
		GetFindInfo()->Clear();
		m_strSearch.Empty();
		ResetComp();
		UpdateByComp();
	}
	else if( m_pList )
	{
		size_t i = 0;
		for( ; i < m_pList->GetSlotCnt(); ++i )
		{
			CTSearchSlot* pSlot = static_cast< CTSearchSlot* >( m_pList->GetSlot( i ) );
			if( pSlot->OnLButtonDown( pt ) )
			{
				m_pCategory->NotifyUpdate();
				break;
			}
		}
	}
}

void	CTAuctionSearch::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags )
{
	if( !CanProcess() )
		return ;

	if( nChar == VK_RETURN && IsEditMode() )
		DoSearch();
	else
		ITInnerFrame::OnKeyDown( nChar, nRepCnt, nFlags );
}

BOOL	CTAuctionSearch::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	m_pList->DoMouseWheel( nFlags, zDelta, pt );
	m_pList->NotifyUpdate();

	return TRUE;
}

void CTAuctionSearch::SwitchFocus( TComponent *pCandidate )
{
	if( m_pFocus == pCandidate )
		return;

	if( m_pFocus )
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if( pCandidate )
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}

void	CTAuctionSearch::ShowComponent( BOOL bVisible )
{
	ITInnerFrame::ShowComponent( bVisible );

	if( !bVisible )
	{
		ResetComp();
		UpdateByComp();
	}

	m_pCategory->NotifyUpdate();
}

HRESULT	CTAuctionSearch::Render( DWORD dwTickCount )
{
	if( m_pCategory )
		m_pCategory->Update( m_pList );

	if( m_pList )
		m_pList->Update();

	ITInnerFrame::Render( dwTickCount );

	return S_OK;
}
//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
void	CTAuctionSearch::DoSearch()
{
	CTAuctionMainFrame::GetInstance()
		->GetList()->MoveFirstPage();

	CTAuctionMainFrame::GetInstance()
		->GetList()->ClearItems();

	SetCurPage(1);

	UpdateByFindInfo();

	LPTAUCTIONFINDINFO pFindInfo = GetFindInfo();
	pFindInfo->ClearCodeList();

	BOOL bStringSearch = FALSE;
	if( m_strSearch.GetLength() - 1 > 0 )
	{
		bStringSearch = TRUE;

		MAPTITEMTEMP::iterator itor = CTChart::m_mapTCANTRADETEMP.begin();
		MAPTITEMTEMP::iterator end = CTChart::m_mapTCANTRADETEMP.end();

		while( itor != end )
		{
			CString strItemName = (*itor).second->m_strNAME;

			strItemName.MakeUpper();
			m_strSearch.MakeUpper();

			if( strItemName.Find( m_strSearch ) != -1 )
			{
				size_t i = 0;
				for( ; i < TCLASS_COUNT; ++i )
				{
					TAUCTIONTREE _AuctionCode;
					_AuctionCode.m_ID.dwValue = (*itor).second->m_dwAuctionCode;
					DWORD dwClass = (1<<i);
					if( (_AuctionCode.m_ID.KindID &&
						_AuctionCode.m_ID.Class == 0) ||
						_AuctionCode.m_ID.Class & dwClass )
					{
						TAUCTIONTREE _Code;
						_Code.m_ID.dwValue = _AuctionCode.m_ID.dwValue;
						_Code.m_ID.Class = _Code.m_ID.Class ? dwClass : 0;

						pFindInfo->PushCode( _Code.m_ID.dwValue, (DWORD)(*itor).second->m_wItemID );
					}
				}
			}

			++itor;
		}

		if( pFindInfo->vAuctionCode.size() )
		{
			CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
			pFindInfo->wPageNum = GetPageNum();
			pComm->RequestSearchList( pFindInfo );
		}
		else
		{
			CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
			pComm->SimpleMsgBox( CTChart::Format( TSTR_AUCTIONERROR_NAMESEARCH ) );
		}
	}
	else
	{
		VTDWORD vTemp;
		m_pCategory->GetRootEntity()->GetCheckList_Child( &vTemp );

		size_t i = 0;
		size_t size = vTemp.size();
		for( ; i < size; ++i )
			pFindInfo->PushCode( vTemp[i] );

		CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
		pFindInfo->wPageNum = GetPageNum();
		pComm->RequestSearchList( pFindInfo );
	}
}

void CTAuctionSearch::ClearText()
{
	size_t i = 0;
	for( ; i < TEDIT_COUNT; ++i )
	{
		m_pEdit[i]->ClearText();
	}
}

void CTAuctionSearch::ResetComp()
{
	CTCategoryEntity* pEntity = m_pCategory->GetRootEntity();

	pEntity->SetExpand_Child( FALSE );
	pEntity->SetCheck_Child( FALSE );

	m_pCategory->NotifyUpdate();
	m_pList->NotifyUpdate();

	size_t i = 0;
	for( ; i < TEDIT_COUNT; ++i )
	{
		m_pEdit[i]->m_strText.Empty();
		m_pEdit[i]->SetFocus( FALSE );
		m_pEdit[i]->ShowComponent( TRUE );
		m_pEdit[i]->EnableComponent( TRUE );
	}
}

void	CTAuctionSearch::UpdateByFindInfo()
{
	LPTAUCTIONFINDINFO pFindInfo = GetFindInfo();
	pFindInfo->bMinWearLv	= (BYTE)atoi( m_pEdit[ TEDIT_NEEDLEVEL_MIN ]->m_strText );
	pFindInfo->bMaxWearLv	= (BYTE)atoi( m_pEdit[ TEDIT_NEEDLEVEL_MAX ]->m_strText );
	pFindInfo->bMinPowerLv	= (BYTE)atoi( m_pEdit[ TEDIT_UPGRADE_MIN ]->m_strText );
	pFindInfo->bMaxPowerLv	= (BYTE)atoi( m_pEdit[ TEDIT_UPGRADE_MAX ]->m_strText );
	m_strSearch				= m_pEdit[ TEDIT_SEARCH ]->m_strText;

	if( pFindInfo->bMinWearLv > pFindInfo->bMaxWearLv )
	{
		BYTE bTemp = pFindInfo->bMaxWearLv;
		pFindInfo->bMaxWearLv = pFindInfo->bMinWearLv;
		pFindInfo->bMinWearLv = bTemp;
	}

	if( pFindInfo->bMinPowerLv > pFindInfo->bMaxPowerLv )
	{
		BYTE bTemp = pFindInfo->bMaxPowerLv;
		pFindInfo->bMaxPowerLv = pFindInfo->bMinPowerLv;
		pFindInfo->bMinPowerLv = bTemp;
	}
}

void	CTAuctionSearch::UpdateByComp()
{
	LPTAUCTIONFINDINFO pFindInfo = GetFindInfo();
	m_pEdit[ TEDIT_NEEDLEVEL_MIN ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFindInfo->bMinWearLv );
	m_pEdit[ TEDIT_NEEDLEVEL_MAX ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFindInfo->bMaxWearLv );
	m_pEdit[ TEDIT_UPGRADE_MIN ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFindInfo->bMinPowerLv );
	m_pEdit[ TEDIT_UPGRADE_MAX ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFindInfo->bMaxPowerLv );
	m_pEdit[ TEDIT_SEARCH ]->m_strText = CTChart::Format( TSTR_FMT_NAME, m_strSearch );
}

TEdit*	CTAuctionSearch::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	size_t i = 0;
	for( ; i < TEDIT_COUNT; ++i )
	{
		if( m_pEdit[i] &&
			m_pEdit[i]->CanProcess() &&
			m_pEdit[i] == m_pFocus )
		{
			return m_pEdit[i];
		}
	}

	return NULL;
}
//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATAION.
BOOL CTAuctionSearch::IsEditMode()
{
	return ( m_pEdit[ TEDIT_SEARCH ] &&
		m_pEdit[ TEDIT_SEARCH ]->CanProcess() &&
		m_pEdit[ TEDIT_SEARCH ] == m_pFocus );

//	return (BOOL)m_bCurMode == TMODE_EDITING;
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////

