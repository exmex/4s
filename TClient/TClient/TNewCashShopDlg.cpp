#include "stdafx.h"
#include "TNewCashShopDlg.h"
#include "resource.h"

CASHEXTENDPERIODINFO CTNewCashShopDlg::m_ExtendPeriodInfo;
CRect CTNewCashShopDlg::m_rtNewCategory;

TComponent* CTCashShopCategory::m_pTempCategoryBar = NULL;
TComponent* CTCashShopCategory::m_pTempCategoryName = NULL;
TComponent* CTCashShopCategory::m_pTempCategoryPlusBtn = NULL;
TComponent* CTCashShopCategory::m_pTempCategoryMinusBtn = NULL;

CPoint CTCashShopCategory::m_ptPlusBtnOffset;
CPoint CTCashShopCategory::m_ptMinusBtnOffset;
CPoint CTCashShopCategory::m_ptNameBtnOffset;



CTCashShopCategory::CTCashShopCategory()
{
}

CTCashShopCategory::~CTCashShopCategory()
{
	DeleteAllItem();
}

void CTCashShopCategory::DeleteAllItem()
{
	VTCASHSHOPITEM::iterator it, end;
	it = m_vCashShopItems.begin();
	end = m_vCashShopItems.end();

	for(; it != end ; ++it )
		delete (*it);

	m_vCashShopItems.clear();
}

void CTCashShopCategory::SetCategoryPos( CPoint ptBase )
{
	m_pCategoryBar->MoveComponent( ptBase );
	m_pPlusBtn->MoveComponent( ptBase + CTCashShopCategory::m_ptPlusBtnOffset );
	m_pMinusBtn->MoveComponent( ptBase + CTCashShopCategory::m_ptMinusBtnOffset );
	m_pName->MoveComponent( ptBase + CTCashShopCategory::m_ptNameBtnOffset );
}

void CTCashShopCategory::SetName( CString strName )
{
	if( m_pName )
		m_pName->m_strText = strName;
}

INT CTCashShopCategory::CheckExpandBtn( CPoint pt )
{
	if( m_pPlusBtn && m_pPlusBtn->IsVisible() )
	{
		if( m_pPlusBtn->HitTest( pt ) )
		{
			return 1;
		}
	}

	if( m_pMinusBtn && m_pMinusBtn->IsVisible() )
	{
		if( m_pMinusBtn->HitTest( pt ) )
		{
			return -1;
		}
	}

	return 0;
}

void CTCashShopCategory::ShowComponent( BOOL bVisible )
{
	if( bVisible )
	{
		m_pMinusBtn->ShowComponent( m_bOpened);
		m_pPlusBtn->ShowComponent( !m_bOpened);	
	}
	else
	{
		m_pMinusBtn->ShowComponent( FALSE);
		m_pPlusBtn->ShowComponent( FALSE);
	}
}

INT CTCashShopCategory::GetItemCount()
{
	return (INT) m_vCashShopItems.size();
}

// =================================================

CTNewCashShopDlg::CTNewCashShopDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
	CGDIFont *pGDIFONT = FindFont( ID_FONT_LOGIN );

	m_pCashText = FindKid( ID_CTRLINST_CURCASH);

	CPoint ptIconOffset, ptNameOffset, ptCashOffset, ptBestIconOffset, ptNewIconOffset;
	// NEW √ ±‚»≠
	{
		static DWORD dwBASE_NEW[ MAX_NEW_ITEM_SLOT] =
		{
			ID_CTRLINST_TOP_SLOT_1, //ID_CTRLINST_SLOT_NEW_1,
			ID_CTRLINST_TOP_SLOT_2, //ID_CTRLINST_SLOT_NEW_2,
			ID_CTRLINST_TOP_SLOT_3, //ID_CTRLINST_SLOT_NEW_3,
			ID_CTRLINST_TOP_SLOT_4, //ID_CTRLINST_SLOT_NEW_4
		};

		CTCLSlotArray vSlots;

		for(INT i=0; i<MAX_NEW_ITEM_SLOT; ++i)
		{
			CTCashShopSlot* pSlot = new CTCashShopSlot();
			pSlot->SetSlot( i, FindKid( dwBASE_NEW[ i ]) );
			vSlots.push_back(pSlot);
		}

		CTCashShopSlot* pSlot0 = NULL;

		pSlot0 = static_cast<CTCashShopSlot*>(vSlots[0]);
		pSlot0->m_pIcon = static_cast<TImageList*>( FindKid( ID_CTRLINST_ICON) );
		pSlot0->m_pIcon->SetFont( pGDIFONT, NULL );
		pSlot0->m_pIcon->m_flagAlign = ALIGN_CENTER;
		pSlot0->m_pName = FindKid( ID_CTRLINST_TXT_ITEM);
		pSlot0->m_pCash = FindKid( ID_CTRLINST_TXT_CASH);
		pSlot0->m_pBestIcon = FindKid( ID_CTRLINST_BEST );
		pSlot0->m_pNewIcon = FindKid( ID_CTRLINST_NEW );

		CPoint ptBase, ptIcon, ptName, ptCash, ptBestIcon, ptNewIcon;

		pSlot0->m_pBase->GetComponentPos(&ptBase);
		pSlot0->m_pIcon->GetComponentPos(&ptIcon);
		pSlot0->m_pName->GetComponentPos(&ptName);
		pSlot0->m_pCash->GetComponentPos(&ptCash);
		pSlot0->m_pBestIcon->GetComponentPos(&ptBestIcon);
		pSlot0->m_pNewIcon->GetComponentPos(&ptNewIcon);

		ptIconOffset = ptIcon - ptBase;
		ptNameOffset = ptName - ptBase;
		ptCashOffset = ptCash - ptBase;
		ptBestIconOffset = ptBestIcon - ptBase;
		ptNewIconOffset = ptNewIcon - ptBase;

		for(INT i=1; i<MAX_NEW_ITEM_SLOT; ++i)
		{
			CTCashShopSlot* pSlot = static_cast<CTCashShopSlot*>(vSlots[i]);

			pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
			pSlot->m_pIcon->SetFont( pGDIFONT, NULL );
			pSlot->m_pIcon->m_flagAlign = ALIGN_CENTER;
			pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);
			pSlot->m_pCash = new TComponent(this, pSlot0->m_pCash->m_pDESC);
			pSlot->m_pBestIcon = new TComponent(this, pSlot0->m_pBestIcon->m_pDESC);
			pSlot->m_pNewIcon = new TComponent(this, pSlot0->m_pNewIcon->m_pDESC);
			
			pSlot->m_pIcon->m_id = GetUniqueID();
			pSlot->m_pName->m_id = GetUniqueID();
			pSlot->m_pCash->m_id = GetUniqueID();
			pSlot->m_pBestIcon->m_id = GetUniqueID();
			pSlot->m_pNewIcon->m_id = GetUniqueID();

			AddKid( pSlot->m_pIcon );
			AddKid( pSlot->m_pName );
			AddKid( pSlot->m_pCash );
			AddKid( pSlot->m_pBestIcon );
			AddKid( pSlot->m_pNewIcon );

			pSlot->m_pBase->GetComponentPos(&ptBase);

			pSlot->m_pIcon->MoveComponent( ptBase + ptIconOffset);
			pSlot->m_pName->MoveComponent( ptBase + ptNameOffset);
			pSlot->m_pCash->MoveComponent( ptBase + ptCashOffset);
			pSlot->m_pBestIcon->MoveComponent( ptBase + ptBestIconOffset);
			pSlot->m_pNewIcon->MoveComponent( ptBase + ptNewIconOffset);
		}
	
		m_pListNEW = new CTCtrlList();
		m_pListNEW->SetSlot(&vSlots);
		m_pListNEW->SetupScroll( static_cast<TScroll*>( FindKid( ID_CTRLINST_TOP_SCROLL)), MAX_ITEM_COLUMN);
	}

	m_rtNewCategory = CRect( 19, 60, 411, 183 );

	// ID_CTRLINST_CATEGORY_BAR

	TComponent* pCategory = FindKid( ID_CTRLINST_CATEGORY_BAR );
	TComponent* pCategoryName = FindKid( ID_CTRLINST_CATEGORY_TITLE );
	TComponent* pCategoryPlusBtn = FindKid( ID_CTRLINST_BTN_PLUS);
	TComponent* pCategoryMinusBtn = FindKid( ID_CTRLINST_BTN_MINUS );

	RemoveKid( pCategory );
	RemoveKid( pCategoryName );
	RemoveKid( pCategoryPlusBtn );
	RemoveKid( pCategoryMinusBtn );

	CTCashShopCategory::m_pTempCategoryBar = pCategory;
	CTCashShopCategory::m_pTempCategoryName = pCategoryName;
	CTCashShopCategory::m_pTempCategoryPlusBtn = pCategoryPlusBtn;
	CTCashShopCategory::m_pTempCategoryMinusBtn = pCategoryMinusBtn;

	CPoint ptCategory, ptCategoryName, ptCategoryPlusBtn, ptCategoryMinusBtn;
	pCategory->GetComponentPos( &ptCategory );
	pCategoryName->GetComponentPos( &ptCategoryName );
	pCategoryPlusBtn->GetComponentPos( &ptCategoryPlusBtn );
	pCategoryMinusBtn->GetComponentPos( &ptCategoryMinusBtn );

	m_ptBasePointCategory = ptCategory;

	CTCashShopCategory::m_ptNameBtnOffset = ptCategoryName - ptCategory;
	CTCashShopCategory::m_ptPlusBtnOffset = ptCategoryPlusBtn - ptCategory;
	CTCashShopCategory::m_ptMinusBtnOffset = ptCategoryMinusBtn - ptCategory;

	SIZE sizeCategory;
	pCategory->GetComponentSize( &sizeCategory );
	m_nCategoryHeight = sizeCategory.cy;

	{
		static DWORD dwBASE[ MAX_ITEM_SLOT] =
		{
			ID_CTRLINST_SLOT_1,
			ID_CTRLINST_SLOT_2,
			ID_CTRLINST_SLOT_3,
			ID_CTRLINST_SLOT_4,
			ID_CTRLINST_SLOT_5,
			ID_CTRLINST_SLOT_6,
		};

		CTCLSlotArray vSlots;

		for(INT i=0; i<MAX_ITEM_SLOT; ++i)
		{
			CTCashShopSlot* pSlot = new CTCashShopSlot();
			pSlot->SetSlot( i, FindKid( dwBASE[ i ]) );
			vSlots.push_back(pSlot);
		}

		TImageList* pImageTemp = static_cast<TImageList*>( FindKid( ID_CTRLINST_ICON) );
		TComponent* pNameTemp = FindKid( ID_CTRLINST_TXT_ITEM);
		TComponent* pCashTemp = FindKid( ID_CTRLINST_TXT_CASH);
		TComponent* pBestIconTemp = FindKid( ID_CTRLINST_BEST );
		TComponent* pNewIconTemp = FindKid( ID_CTRLINST_NEW );

		for(INT i=0; i<MAX_ITEM_SLOT; ++i)
		{
			CTCashShopSlot* pSlot = static_cast<CTCashShopSlot*>( vSlots[i]);

			pSlot->m_pIcon = new TImageList(this, pImageTemp->m_pDESC);
			pSlot->m_pIcon->SetFont( pGDIFONT, NULL );
			pSlot->m_pIcon->m_flagAlign = ALIGN_CENTER;
			pSlot->m_pName = new TComponent(this, pNameTemp->m_pDESC);
			pSlot->m_pCash = new TComponent(this, pCashTemp->m_pDESC);
			pSlot->m_pBestIcon = new TComponent(this, pBestIconTemp->m_pDESC);
			pSlot->m_pNewIcon = new TComponent(this, pNewIconTemp->m_pDESC);

			pSlot->m_pIcon->m_id = GetUniqueID();
			pSlot->m_pName->m_id = GetUniqueID();
			pSlot->m_pCash->m_id = GetUniqueID();
			pSlot->m_pBestIcon->m_id = GetUniqueID();
			pSlot->m_pNewIcon->m_id = GetUniqueID();

			AddKid( pSlot->m_pIcon );
			AddKid( pSlot->m_pName );
			AddKid( pSlot->m_pCash );
			AddKid( pSlot->m_pBestIcon );
			AddKid( pSlot->m_pNewIcon );

			CPoint ptBase, ptIcon, ptName, ptCash, ptBestIcon, ptNewIcon;
			pSlot->m_pBase->GetComponentPos(&ptBase);
			pSlot->m_pIcon->MoveComponent(ptBase+ptIconOffset);
			pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
			pSlot->m_pCash->MoveComponent(ptBase+ptCashOffset);
			pSlot->m_pBestIcon->MoveComponent(ptBase+ptBestIconOffset);
			pSlot->m_pNewIcon->MoveComponent(ptBase+ptNewIconOffset);

			pSlot->m_pIcon->GetComponentPos( &ptIcon);
			pSlot->m_pName->GetComponentPos( &ptName);
			pSlot->m_pCash->GetComponentPos( &ptCash);
			pSlot->m_pBestIcon->GetComponentPos( &ptBestIcon);
			pSlot->m_pNewIcon->GetComponentPos( &ptNewIcon);

			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pBase->m_id, 0, ptBase - ptCategory ) );
			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pIcon->m_id, 0, ptIcon - ptCategory  ) );
			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pName->m_id, 0, ptName - ptCategory ) );
			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pCash->m_id, 0, ptCash - ptCategory ) );
			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pBestIcon->m_id, 0, ptBestIcon - ptCategory ) );
			m_vItemListOffset.push_back( TFRAMEOFFSET( pSlot->m_pNewIcon->m_id, 0, ptNewIcon - ptCategory ) );

			m_pListCompIMAGE[ i ] = pSlot->m_pIcon;
			m_pListCompNAME[ i ] = pSlot->m_pName;
			m_pListCompCASH[ i ] = pSlot->m_pCash;
			m_pListCompBEST[ i ] = pSlot->m_pBestIcon;
			m_pListCompNEW[ i ] = pSlot->m_pNewIcon;
		}

		CPoint ptScroll;
		m_pSCROLL = FindKid( ID_CTRLINST_SCROLL);
		m_pSCROLL->GetComponentPos( &ptScroll);
		m_vItemListOffset.push_back( TFRAMEOFFSET( m_pSCROLL->m_id, 0, ptScroll - ptCategory ) );

		//SIZE sizeSCROLL;
		//m_pSCROLL->GetComponentSize( &sizeSCROLL );
		//m_nListHeight = (ptScroll.y + sizeSCROLL.cy) - (ptCategory.y+sizeCategory.cy) + 10;
		m_nListHeight = 70;
		
		m_pList = new CTCtrlList();
		m_pList->SetSlot(&vSlots);
		m_pList->SetupScroll( static_cast<TScroll*>( m_pSCROLL), MAX_ITEM_COLUMN);	
	}

	m_pBuy = static_cast<TButton*>(FindKid(ID_CTRLINST_OK));
	m_pPresent = static_cast<TButton*>(FindKid(ID_CTRLINST_PRESENT));

	m_bShowScroll = TRUE;
	m_bNeedResetPos = TRUE;
	m_bPresent = FALSE;
}

CTNewCashShopDlg::~CTNewCashShopDlg()
{
	delete m_pListNEW;
	delete m_pList;

	MAPTCASHSHOPCATEGORY::iterator it, end;
	it = m_mapCategory.begin();
	end = m_mapCategory.end();

	for(; it != end ; ++it )
		delete it->second;

	m_mapCategory.clear();

	if( CTCashShopCategory::m_pTempCategoryBar )
		delete CTCashShopCategory::m_pTempCategoryBar;

	if( CTCashShopCategory::m_pTempCategoryName )
		delete CTCashShopCategory::m_pTempCategoryName;

	if( CTCashShopCategory::m_pTempCategoryPlusBtn )
		delete CTCashShopCategory::m_pTempCategoryPlusBtn;

	if( CTCashShopCategory::m_pTempCategoryMinusBtn )
		delete CTCashShopCategory::m_pTempCategoryMinusBtn;
}

void CTNewCashShopDlg::SetCash( DWORD dwCash, DWORD dwBonusCash )
{
	m_dwCash = dwCash;
	m_dwCashBonus = dwBonusCash;

	if( CTNationOption::JAPAN )
		m_pCashText->m_strText = CTChart::Format( TSTR_FMT_CASH, dwCash);
	else if ( CTNationOption::RUSSIA )
	{
		FLOAT fCash = dwCash / 100.0f;
		FLOAT fBonusCash = dwBonusCash / 100.0f;

		m_pCashText->m_strText = CTChart::Format( TSTR_FMT_CASHNBONUS_FLOAT, fCash, fBonusCash );
	}
	else
		m_pCashText->m_strText = CTChart::Format( TSTR_FMT_CASHNBONUS, dwCash, dwBonusCash);
}

void CTNewCashShopDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )
		return ;

	CTClientUIBase::OnLButtonUp( nFlags, pt);

	MAPTCASHSHOPCATEGORY::iterator it, end;
	it = m_mapCategory.begin();
	end = m_mapCategory.end();
	for(; it != end ; ++it )
	{
		switch( (it->second)->CheckExpandBtn( pt ) )
		{
		case 1:
			OpenCategory( it->first );
			break;
		case -1:
			OpenCategory( -1 );
			break;
		}
	}
}

void CTNewCashShopDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent( bVisible );
	m_pList->NotifyUpdate();
	m_pListNEW->NotifyUpdate();

	VTCASHSHOPCATEGORY::iterator it, end;
	it = m_vCategoryOrders.begin();
	end = m_vCategoryOrders.end();
	for(; it != end ; ++it )
		(*it)->ShowComponent( bVisible );

	switch( m_nMode )
	{
	case CASHSHOPDLGMODE_NORMAL:

		{
			if( CTNationOption::POLAND ||
				CTNationOption::GERMANY ||
				CTNationOption::GREECE ||
//				CTNationOption::FRANCE ||
				CTNationOption::CZECH || 
//				CTNationOption::ITALY || 
				CTNationOption::SPAIN ||
				CTNationOption::UNITEDKINGDOM ||
				CTNationOption::ROMANIA ||
				CTNationOption::UNITEDSTATE ||
				CTNationOption::HUNGARY ||
				CTNationOption::TURKEY ||
				CTNationOption::PORTUGAL ||
				CTNationOption::ENGLISH )
			{
				m_pBuy->ShowComponent(FALSE);
			}
		}
		break;

	case CASHSHOPDLGMODE_PERIOD_EXTENSION:
		m_pBuy->ShowComponent(FALSE);
		break;
	}

	m_bNeedResetPos = TRUE;
}

HRESULT CTNewCashShopDlg::Render(DWORD dwTickCount)
{
	if( m_bVisible )
	{
		if( m_bNeedResetPos )
		{
			m_pListNEW->Update();
			m_pList->Update();

			BOOL bShowList = ResetPos();

			m_pSCROLL->ShowComponent( bShowList && m_bShowScroll );

			if( !bShowList )
				m_pList->ShowComponent( bShowList );
		}

		return CTClientUIBase::Render(dwTickCount);
	}
	return S_OK;
}

BOOL CTNewCashShopDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	CRect rt = m_rtNewCategory;
	rt.left += m_rc.left;
	rt.top += m_rc.top;
	rt.right += m_rc.left;
	rt.bottom += m_rc.top;

	if( rt.PtInRect( pt ) )
		return m_pListNEW->DoMouseWheel( nFlags, zDelta, pt);
	else
		return m_pList->DoMouseWheel( nFlags, zDelta, pt);

	return FALSE;
}

void CTNewCashShopDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )
		return ;

	{
		CTCtrlListItem* pItem = m_pList->GetHitItem(pt);
		if( pItem )
		{
			m_pList->Select(pItem->m_nIndex);
			m_pListNEW->Select(-1);
		}
	}

	{
		CTCtrlListItem* pItem = m_pListNEW->GetHitItem(pt);
		if( pItem )
		{
			m_pListNEW->Select( pItem->m_nIndex );
			m_pList->Select(-1);
		}
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}

void CTNewCashShopDlg::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )
		return ;

	if( m_pList->GetHitItem(pt) || m_pListNEW->GetHitItem(pt) )
	{
		switch( m_nMode )
		{
		case CASHSHOPDLGMODE_NORMAL:
			m_pCommandHandler->m_vCOMMAND.push_back( GM_WARN_BUY_CASHITEM);
			break;
		case CASHSHOPDLGMODE_PERIOD_EXTENSION:
			m_pCommandHandler->m_vCOMMAND.push_back( GM_WARN_EXTEND_PERIOD);
			break;
		}		
	}
}

void CTNewCashShopDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )
		return ;

	if( !m_pList->GetHitItem(pt) && !m_pListNEW->GetHitItem(pt) )
		CTClientUIBase::OnRButtonDown(nFlags,pt);
}

BYTE CTNewCashShopDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	return FALSE;
}

ITDetailInfoPtr CTNewCashShopDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
	{
		pHitItem = m_pListNEW->GetHitItem(point);

		if( !pHitItem )
			return pInfo;
	}

	if( pHitItem )
	{
		CTCashShopItem* pShopItem = static_cast<CTCashShopItem*>(pHitItem);
		if( pShopItem->m_pClientItem )
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewItemInst(pShopItem->m_pClientItem, rc);
			pInfo->SetDir(TRUE, FALSE, TRUE);
		}
	}

	return pInfo;
}

BOOL CTNewCashShopDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);

	if( !pHitItem )
	{
		pHitItem = m_pListNEW->GetHitItem(point);

		if( !pHitItem )
			return FALSE;
	}

	CTCashShopItem* pShopItem = static_cast<CTCashShopItem*>(pHitItem);
	
	if( pShopItem->m_pClientItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pShopItem->m_pClientItem->GetTITEM();
		outInfo.m_pClientItem = pShopItem->m_pClientItem;
		
		return TRUE;
	}

	return FALSE;
}

void CTNewCashShopDlg::ClearAllCategory()
{
	m_pListNEW->ClearItem();
	m_pList->ClearItem();

	{
		VTCASHSHOPCATEGORY::iterator it, end;
		it = m_vCategoryOrders.begin();
		end = m_vCategoryOrders.end();

		for(; it != end ;++it )
		{
			(*it)->DeleteAllItem();

			RemoveKid( (*it)->m_pCategoryBar );
			delete (*it)->m_pCategoryBar;

			RemoveKid( (*it)->m_pName );
			delete (*it)->m_pName;

			RemoveKid( (*it)->m_pPlusBtn );
			delete (*it)->m_pPlusBtn;
			
			RemoveKid( (*it)->m_pMinusBtn );
			delete (*it)->m_pMinusBtn;

			delete (*it);
		}

		m_vCategoryOrders.clear();
		m_mapCategory.clear();
	}

	m_bNeedResetPos = TRUE;
}

void CTNewCashShopDlg::AddItemToNewCategory( CTCashShopItem* pCashShopItem )
{
	m_pListNEW->AddItem( pCashShopItem );

	m_bNeedResetPos = TRUE;
}

void CTNewCashShopDlg::AddCategory( DWORD dwCategoryID, CString strName )
{
	MAPTCASHSHOPCATEGORY::iterator it  = m_mapCategory.find( dwCategoryID );
	if( it ==  m_mapCategory.end() )
	{
		CTCashShopCategory* pCategory = new CTCashShopCategory;

		pCategory->m_dwID = dwCategoryID;

		pCategory->m_pCategoryBar = new TComponent( this, CTCashShopCategory::m_pTempCategoryBar->m_pDESC );
		pCategory->m_pName = new TComponent( this, CTCashShopCategory::m_pTempCategoryName->m_pDESC );
		pCategory->m_pPlusBtn = new TButton( this, CTCashShopCategory::m_pTempCategoryPlusBtn->m_pDESC );
		pCategory->m_pMinusBtn = new TButton( this, CTCashShopCategory::m_pTempCategoryMinusBtn->m_pDESC );

		pCategory->m_pCategoryBar->m_id = GetUniqueID();
		pCategory->m_pName->m_id = GetUniqueID();
		pCategory->m_pPlusBtn->m_id = GetUniqueID();
		pCategory->m_pMinusBtn->m_id = GetUniqueID();

		AddKid( pCategory->m_pCategoryBar );
		AddKid( pCategory->m_pName );
		AddKid( pCategory->m_pPlusBtn );
		AddKid( pCategory->m_pMinusBtn );

		pCategory->SetName( strName );
		m_mapCategory.insert( make_pair( dwCategoryID, pCategory ));
		m_vCategoryOrders.push_back( pCategory );
	}
	else
	{
		CTCashShopCategory* pCategory = it->second;
		pCategory->SetName( strName );
	}

	m_bNeedResetPos = TRUE;
}

void CTNewCashShopDlg::DeleteCategory( DWORD dwCategoryID )
{
	MAPTCASHSHOPCATEGORY::iterator it  = m_mapCategory.find( dwCategoryID );
	if( it !=  m_mapCategory.end() )
	{
		CTCashShopCategory* pCategory = it->second;

		{
			VTCASHSHOPCATEGORY::iterator itOrder, endOrder;
			itOrder = m_vCategoryOrders.begin();
			endOrder = m_vCategoryOrders.end();

			for(; itOrder != endOrder ; ++itOrder )
			{
				if( (*itOrder) == pCategory )
				{
					m_vCategoryOrders.erase( itOrder );
					break;
				}
			}
		}

		pCategory->DeleteAllItem();

		RemoveKid( pCategory->m_pCategoryBar );
		delete pCategory->m_pCategoryBar;

		RemoveKid( pCategory->m_pName );
		delete pCategory->m_pName;

		RemoveKid( pCategory->m_pPlusBtn );
		delete pCategory->m_pPlusBtn;
		
		RemoveKid( pCategory->m_pMinusBtn );
		delete pCategory->m_pMinusBtn;

		delete pCategory;

		m_mapCategory.erase( it );
		m_bNeedResetPos = TRUE;
	}
}

CTCashShopCategory* CTNewCashShopDlg::GetCategory( DWORD dwCategoryID )
{
	MAPTCASHSHOPCATEGORY::iterator it  = m_mapCategory.find( dwCategoryID );
	if( it !=  m_mapCategory.end() )
		return it->second;

	return NULL;
}

void CTNewCashShopDlg::AddItemToCategory( DWORD dwCategoryID, CTCashShopItem* pCashShopItem )
{
	MAPTCASHSHOPCATEGORY::iterator it  = m_mapCategory.find( dwCategoryID );
	if( it != m_mapCategory.end() )
	{
		CTCashShopCategory* pCategory = (CTCashShopCategory*) it->second;
		pCategory->m_vCashShopItems.push_back( pCashShopItem );	
	}

	m_bNeedResetPos = TRUE;
}

void CTNewCashShopDlg::OpenCategory( DWORD dwCategoryID )
{
	MAPTCASHSHOPCATEGORY::iterator it, end;
	it = m_mapCategory.begin();
	end = m_mapCategory.end();

	m_pList->ClearItem();
	ClearListComponent();

	for(; it != end ; ++it )
	{
		if( it->first == dwCategoryID )
		{
			(it->second)->m_bOpened = TRUE;

			VTCASHSHOPITEM& vCashShopItem = (it->second)->m_vCashShopItems;

			VTCASHSHOPITEM::iterator itITEM, endITEM;
			itITEM = vCashShopItem.begin();
			endITEM = vCashShopItem.end();

			for(; itITEM != endITEM ; ++itITEM )
				m_pList->AddItem( (*itITEM)->Clone() );
		}
		else
			(it->second)->m_bOpened = FALSE;
	}

	m_pList->NotifyUpdate();
	m_bNeedResetPos = TRUE;
}

void CTNewCashShopDlg::OpenTopCategory()
{
	if( !m_vCategoryOrders.empty() )
		OpenCategory( m_vCategoryOrders[ 0 ]->m_dwID );

	m_bNeedResetPos = TRUE;
}

BOOL CTNewCashShopDlg::ResetPos()
{
	BOOL bShowList = FALSE;
	CPoint ptCurrent = m_ptBasePointCategory;

	VTCASHSHOPCATEGORY::iterator it, end;
	it = m_vCategoryOrders.begin();
	end = m_vCategoryOrders.end();

	for(; it != end ; ++it )
	{
		(*it)->SetCategoryPos( ptCurrent );

		if( (*it)->m_bOpened )
		{
			size_t nItemCount = (*it)->m_vCashShopItems.size();
			m_bShowScroll = nItemCount > MAX_ITEM_SLOT;

			VTFRAMEOFFSET::iterator it, end;
			it = m_vItemListOffset.begin();
			end = m_vItemListOffset.end();

			for(; it != end ; ++it )
			{
				TComponent* pCOMP = FindKid( it->m_dwFRAME );
				if( pCOMP )
					pCOMP->MoveComponent( ptCurrent + it->m_vOffset );
			}

			INT n = 1;
			if( nItemCount >= MAX_ITEM_SLOT-1 )
				n = 3;
			else if( nItemCount >= MAX_ITEM_SLOT / 2 )
				n = 2;
			else
				n = 1;

			ptCurrent.y += m_nListHeight * n;
			bShowList = TRUE;
		}

		(*it)->m_pPlusBtn->ShowComponent( !((*it)->m_bOpened) );
		(*it)->m_pMinusBtn->ShowComponent( (*it)->m_bOpened );

		ptCurrent.y += m_nCategoryHeight;
	}

	m_bNeedResetPos = FALSE;

	return bShowList;
}

void CTNewCashShopDlg::ClearListComponent()
{
	for( INT i=0 ; i < MAX_ITEM_SLOT ; ++i )
	{
		m_pListCompIMAGE[ i ]->SetCurImage( 0 );
		m_pListCompNAME[ i ]->m_strText.Empty();
		m_pListCompCASH[ i ]->m_strText.Empty();
	}

	m_bNeedResetPos = TRUE;
}

CTCashShopItem* CTNewCashShopDlg::GetSelectItem()
{
	CTCtrlListItem* pItem = m_pList->GetSelectItem();
	if( !pItem )
		pItem = m_pListNEW->GetSelectItem();

	return (CTCashShopItem*) pItem;
}

void CTNewCashShopDlg::SetCashShopDlgMode( INT n )
{
	m_nMode = n;

	switch( m_nMode )
	{
	case CASHSHOPDLGMODE_NORMAL:
		{
			if( CTNationOption::POLAND ||
				CTNationOption::GERMANY ||
				CTNationOption::GREECE ||
//				CTNationOption::FRANCE ||
				CTNationOption::CZECH ||
//				CTNationOption::ITALY ||
				CTNationOption::SPAIN ||
				CTNationOption::UNITEDKINGDOM ||
				CTNationOption::ROMANIA ||
				CTNationOption::UNITEDSTATE ||
				CTNationOption::HUNGARY ||
				CTNationOption::TURKEY ||
				CTNationOption::PORTUGAL ||
				CTNationOption::ENGLISH )
			{
				m_pBuy->ShowComponent(FALSE);
				m_pPresent->ShowComponent(TRUE);

				m_pPresent->m_menu[TNM_LCLICK] = GM_WARN_BUY_CASHITEM;
				m_pPresent->m_strText = CTChart::LoadString( TSTR_BUY);
			}
			else
			{
				m_pBuy->ShowComponent(TRUE);

				m_pBuy->m_menu[TNM_LCLICK] = GM_WARN_BUY_CASHITEM;
				m_pPresent->m_menu[TNM_LCLICK] = GM_WARN_CASH_PRESENT;

				m_pBuy->m_strText = CTChart::LoadString( TSTR_BUY);
				m_pPresent->m_strText = CTChart::LoadString( TSTR_PRESENT);
			}
		}
		break;

	case CASHSHOPDLGMODE_PERIOD_EXTENSION:
		{
			m_pBuy->ShowComponent(FALSE);
			m_pPresent->m_menu[TNM_LCLICK] = GM_WARN_EXTEND_PERIOD;
			m_pPresent->m_strText = CTChart::LoadString( TSTR_PERIOD_EXTENSION);
		}
		break;
	}
}


void CTNewCashShopDlg::SetPresent( BOOL bPresent )
{
	m_bPresent = bPresent;
}