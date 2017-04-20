#include "Stdafx.h"
#include "TPortalShopDlgNew.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CTPortalShopSlot
// ====================================================================================================
void CTPortalShopSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	m_pBase->ShowComponent( bShow );

	if ( bShow )
	{
		if ( m_bItemType == CTPortalShopItem::TPORTALSHOPITEM_TYPE_TITLE )
		{
			m_pGoldImage->ShowComponent( FALSE );
			m_pPortalText->ShowComponent( FALSE );
			m_pRune->ShowComponent( FALSE );
			m_pLuna->ShowComponent( FALSE );
			m_pCron->ShowComponent( FALSE );
			m_pButton->ShowComponent( FALSE );

			m_pTitleImage->ShowComponent( TRUE );
			m_pTitleText->ShowComponent( TRUE );
		}
		else if ( m_bItemType == CTPortalShopItem::TPORTALSHOPITEM_TYPE_PORTAL )
		{
			m_pGoldImage->ShowComponent( TRUE );
			m_pPortalText->ShowComponent( TRUE );
			m_pRune->ShowComponent( TRUE );
			m_pLuna->ShowComponent( TRUE );
			m_pCron->ShowComponent( TRUE );
			m_pButton->ShowComponent( TRUE );

			m_pTitleImage->ShowComponent( FALSE );
			m_pTitleText->ShowComponent( FALSE );
		}
	}
	else
	{
		m_pGoldImage->ShowComponent( FALSE );
		m_pPortalText->ShowComponent( FALSE );
		m_pRune->ShowComponent( FALSE );
		m_pLuna->ShowComponent( FALSE );
		m_pCron->ShowComponent( FALSE );
		m_pButton->ShowComponent( FALSE );

		m_pTitleImage->ShowComponent( FALSE );
		m_pTitleText->ShowComponent( FALSE );
	}
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pButton)->Select(bSel);
}
// ====================================================================================================

// CTPortalShopItem
// ====================================================================================================
void CTPortalShopItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	CTPortalShopSlot* pPortalSlot = static_cast<CTPortalShopSlot*>(pSlot);

	if ( m_bItemType == TPORTALSHOPITEM_TYPE_TITLE )
	{
		pPortalSlot->m_pTitleText->m_strText = m_strRegion;
		pPortalSlot->m_bItemType = m_bItemType;

		pPortalSlot->m_pGoldImage->ShowComponent( FALSE );
		pPortalSlot->m_pPortalText->ShowComponent( FALSE );
		pPortalSlot->m_pRune->ShowComponent( FALSE );
		pPortalSlot->m_pLuna->ShowComponent( FALSE );
		pPortalSlot->m_pCron->ShowComponent( FALSE );
		pPortalSlot->m_pButton->ShowComponent( FALSE );

		pPortalSlot->m_pTitleImage->ShowComponent( TRUE );
		pPortalSlot->m_pTitleText->ShowComponent( TRUE );
	}
	else if ( m_bItemType == TPORTALSHOPITEM_TYPE_PORTAL )
	{
		pPortalSlot->m_bItemType = m_bItemType;
		pPortalSlot->m_pPortalText->m_strText = m_pTPortal->m_strNAME;

		DWORD dwPRICE = m_dwRealPrice;	//m_pTPortal->m_dwPrice;
		pPortalSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			dwPRICE / TRUNE_DIVIDER);

		pPortalSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			(dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER);

		pPortalSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			dwPRICE % TLUNA_DIVIDER);

		pPortalSlot->m_pGoldImage->ShowComponent( TRUE );
		pPortalSlot->m_pPortalText->ShowComponent( TRUE );
		pPortalSlot->m_pRune->ShowComponent( TRUE );
		pPortalSlot->m_pLuna->ShowComponent( TRUE );
		pPortalSlot->m_pCron->ShowComponent( TRUE );
		pPortalSlot->m_pButton->ShowComponent( TRUE );

		pPortalSlot->m_pTitleImage->ShowComponent( FALSE );
		pPortalSlot->m_pTitleText->ShowComponent( FALSE );
	}
}
// ----------------------------------------------------------------------------------------------------
CTPortalShopItem::~CTPortalShopItem()
{
}

// ====================================================================================================
CTPortalShopDlg::CTPortalShopDlg( TComponent *pParent, LP_FRAMEDESC pDesc )
: CTClientUIBase(pParent, pDesc)
{
	static const DWORD dwSLOTID[] = 
	{
		ID_CTRLINST_SLOT1,
		ID_CTRLINST_SLOT2,
		ID_CTRLINST_SLOT3,
		ID_CTRLINST_SLOT4,
		ID_CTRLINST_SLOT5,
		ID_CTRLINST_SLOT6,
		ID_CTRLINST_SLOT7,
		ID_CTRLINST_SLOT8,
		ID_CTRLINST_SLOT9,
		ID_CTRLINST_SLOT10,
		ID_CTRLINST_SLOT11,
		ID_CTRLINST_SLOT12,
		ID_CTRLINST_SLOT13,
		ID_CTRLINST_SLOT14,
		ID_CTRLINST_SLOT15,
	};

	CTCLSlotArray vSlots;

	m_pPotralName = FindKid( ID_CTRLINST_TITLE );

	for ( INT i=0; i < MAX_ITEM_SLOT; ++i )
	{
		CTPortalShopSlot* pSlot = new CTPortalShopSlot();
		pSlot->SetSlot(i, FindKid(dwSLOTID[i]));
		vSlots.push_back(pSlot);
	}

	CTPortalShopSlot* pSlot0 = NULL;

	pSlot0 = static_cast<CTPortalShopSlot*>(vSlots[0]);
	pSlot0->m_pTitleImage = FindKid( ID_CTRLINST_REGION_BASE );
	pSlot0->m_pTitleText = FindKid( ID_CTRLINST_REGION_NAME );
	
	pSlot0->m_pPortalText = FindKid( ID_CTRLINST_PORTAL_NAME );
	pSlot0->m_pGoldImage = FindKid( ID_CTRLINST_GOLD_BASE );
	pSlot0->m_pRune = FindKid( ID_CTRLINST_RUNE );
	pSlot0->m_pLuna = FindKid( ID_CTRLINST_LUNA );
	pSlot0->m_pCron = FindKid( ID_CTRLINST_CRON );
	pSlot0->m_pButton = static_cast<TButton*>(FindKid( ID_CTRLINST_BTN_LIST ));

	CPoint ptBase, ptTitleImage, ptTitleText, ptPortalText, ptGoldImage, ptRune, ptLuna, ptCron, ptButton;
	CPoint ptTitleImageOffset, ptTitleTextOffset, ptPortalTextOffset, ptGoldImageOffset, ptRuneOffset, ptLunaOffset, ptCronOffset, ptButtonOffset;

	pSlot0->m_pBase->GetComponentPos(&ptBase);
	pSlot0->m_pTitleImage->GetComponentPos(&ptTitleImage);
	pSlot0->m_pTitleText->GetComponentPos(&ptTitleText);
	pSlot0->m_pPortalText->GetComponentPos(&ptPortalText);
	pSlot0->m_pGoldImage->GetComponentPos(&ptGoldImage);
	pSlot0->m_pRune->GetComponentPos(&ptRune);
	pSlot0->m_pLuna->GetComponentPos(&ptLuna);
	pSlot0->m_pCron->GetComponentPos(&ptCron);
	pSlot0->m_pButton->GetComponentPos(&ptButton);

	ptTitleImageOffset = ptTitleImage - ptBase;
	ptTitleTextOffset = ptTitleText - ptBase;
	ptPortalTextOffset = ptPortalText - ptBase;
	ptGoldImageOffset = ptGoldImage - ptBase;
	ptRuneOffset = ptRune - ptBase;
	ptLunaOffset = ptLuna - ptBase;
	ptCronOffset = ptCron - ptBase;
	ptButtonOffset = ptButton - ptBase;

	for(INT i=1; i<MAX_ITEM_SLOT; ++i)
	{
		CTPortalShopSlot* pSlot = static_cast<CTPortalShopSlot*>( vSlots[i]);

		pSlot->m_pTitleImage = new TComponent(this, pSlot0->m_pTitleImage->m_pDESC );
		pSlot->m_pTitleText = new TComponent(this, pSlot0->m_pTitleText->m_pDESC );
		pSlot->m_pPortalText = new TComponent(this, pSlot0->m_pPortalText->m_pDESC );
		pSlot->m_pGoldImage = new TComponent(this, pSlot0->m_pGoldImage->m_pDESC );
		pSlot->m_pRune = new TComponent(this, pSlot0->m_pRune->m_pDESC );
		pSlot->m_pLuna = new TComponent(this, pSlot0->m_pLuna->m_pDESC );
		pSlot->m_pCron = new TComponent(this, pSlot0->m_pCron->m_pDESC );
		pSlot->m_pButton = new TButton(this, pSlot0->m_pButton->m_pDESC );

		pSlot->m_pTitleImage->m_id = GetUniqueID();
		pSlot->m_pTitleText->m_id = GetUniqueID();
		pSlot->m_pPortalText->m_id = GetUniqueID();
		pSlot->m_pGoldImage->m_id = GetUniqueID();
		pSlot->m_pRune->m_id = GetUniqueID();
		pSlot->m_pLuna->m_id = GetUniqueID();
		pSlot->m_pCron->m_id = GetUniqueID();
		pSlot->m_pButton->m_id = GetUniqueID();

		AddKid( pSlot->m_pTitleImage );
		AddKid( pSlot->m_pTitleText );
		AddKid( pSlot->m_pPortalText );
		AddKid( pSlot->m_pGoldImage );
		AddKid( pSlot->m_pRune );
		AddKid( pSlot->m_pLuna );
		AddKid( pSlot->m_pCron );
		AddKid( pSlot->m_pButton );

		pSlot->m_pBase->GetComponentPos(&ptBase);
		pSlot->m_pTitleImage->MoveComponent(ptBase+ptTitleImageOffset);
		pSlot->m_pTitleText->MoveComponent(ptBase+ptTitleTextOffset);
		pSlot->m_pPortalText->MoveComponent(ptBase+ptPortalTextOffset);
		pSlot->m_pGoldImage->MoveComponent(ptBase+ptGoldImageOffset);
		pSlot->m_pRune->MoveComponent(ptBase+ptRuneOffset);
		pSlot->m_pLuna->MoveComponent(ptBase+ptLunaOffset);
		pSlot->m_pCron->MoveComponent(ptBase+ptCronOffset);
		pSlot->m_pButton->MoveComponent(ptBase+ptButtonOffset);
	}

	CPoint ptScroll;
	m_pSCROLL = FindKid( ID_CTRLINST_SCROLL );
	m_pSCROLL->GetComponentPos( &ptScroll);

	m_pList = new CTCtrlList();
	m_pList->SetSlot(&vSlots);
	m_pList->SetupScroll( static_cast<TScroll*>( m_pSCROLL), MAX_ITEM_COLUMN);	
}
// ----------------------------------------------------------------------------------------------------
CTPortalShopDlg::~CTPortalShopDlg(void)
{
	ClearList();
	delete m_pList;
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopDlg::AddPortal( WORD wPortalID, DWORD dwPrice, DWORD dwRealPrice )
{
	LPTPORTAL pTPortal = CTChart::GetTPORTAL(wPortalID);
	if( !pTPortal )
		return ;

	CTPortalShopItem* pItem = new CTPortalShopItem;
	pItem->m_bItemType = CTPortalShopItem::TPORTALSHOPITEM_TYPE_PORTAL;
	pItem->m_pTPortal = pTPortal;
	pItem->m_pTPortal->m_dwPrice = dwPrice;
	pItem->m_dwRealPrice = dwRealPrice;

	MAPLPVTPORTALSHOPITEM::iterator itor = m_mapVTPORTALSHOPITEM.find( pItem->m_pTPortal->m_wPortalRegionID );

	// 추가되는 포탈의 지역이 있을 때
	if ( itor != m_mapVTPORTALSHOPITEM.end() )
	{
		m_mapVTPORTALSHOPITEM[  pItem->m_pTPortal->m_wPortalRegionID ]->push_back( pItem );
	}
	// 새로이 추가되는 지역 일 때
	else
	{
		LPVTPORTALSHOPITEM pVTPORTAL = new VTPORTALSHOPITEM();
		pVTPORTAL->push_back( pItem );
		m_mapVTPORTALSHOPITEM[  pItem->m_pTPortal->m_wPortalRegionID ] = pVTPORTAL;
	}
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopDlg::AlignPortalList()
{
	m_pList->ClearItem();
	for( MAPLPVTPORTALSHOPITEM::iterator itMAPVTPORTAL = m_mapVTPORTALSHOPITEM.begin(); itMAPVTPORTAL != m_mapVTPORTALSHOPITEM.end(); itMAPVTPORTAL++ )
	{
		LPTPORTALREGION pRegion = CTChart::FindTPORTALREGION( (*itMAPVTPORTAL).first );

		CTPortalShopItem* pItem = new CTPortalShopItem;
		pItem->m_bItemType = CTPortalShopItem::TPORTALSHOPITEM_TYPE_TITLE;
		pItem->m_strRegion = pRegion->m_strName;

		m_pList->AddItem( pItem );

		LPVTPORTALSHOPITEM	pVTPORTAL = (*itMAPVTPORTAL).second;
		for( VTPORTALSHOPITEM::iterator itVTPORTAL = pVTPORTAL->begin(); itVTPORTAL != pVTPORTAL->end(); itVTPORTAL++ )
		{
			m_pList->AddItem( (*itVTPORTAL) );
		}
	}
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);
	m_pList->ShowComponent(bVisible);
}
// ----------------------------------------------------------------------------------------------------
BOOL CTPortalShopDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	return m_pList->DoMouseWheel(nFlags,zDelta,pt);
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTCtrlListItem* pItem = m_pList->GetHitItem(pt);
	if( pItem )
		m_pList->Select(pItem->m_nIndex);
	else
		CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------------------------------------
HRESULT CTPortalShopDlg::Render(DWORD dwTickCount)
{
	m_pList->Update();
	return CTClientUIBase::Render(dwTickCount);
}
// ----------------------------------------------------------------------------------------------------
void CTPortalShopDlg::ClearList()
{
	m_pList->ClearItem();
	for ( MAPLPVTPORTALSHOPITEM::iterator itor = m_mapVTPORTALSHOPITEM.begin(); itor != m_mapVTPORTALSHOPITEM.end(); itor++ )
	{
		if ( (*itor).second )
		{
			(*itor).second->clear();
            delete (*itor).second;
		}
	}
	m_mapVTPORTALSHOPITEM.clear();
}
