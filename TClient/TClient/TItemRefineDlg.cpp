#include "stdafx.h"
#include "TItemRefineDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemRefineDlg::CTItemRefineDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM_TARGET) );
	m_pSlotCtrl[RESOURCE_SLOT1] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM_MAT1) );
	m_pSlotCtrl[RESOURCE_SLOT2] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM_MAT2) );
	m_pSlotCtrl[RESOURCE_SLOT3] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM_MAT3) );

	m_pSlotImage[TARGET_SLOT] = FindKid( ID_CTRLINST_SLOT1_IMAGE );
	m_pSlotImage[RESOURCE_SLOT1] = FindKid( ID_CTRLINST_SLOT2_IMAGE );
	m_pSlotImage[RESOURCE_SLOT2] = FindKid( ID_CTRLINST_SLOT3_IMAGE );
	m_pSlotImage[RESOURCE_SLOT3] = FindKid( ID_CTRLINST_SLOT4_IMAGE );

	m_pTitles[TARGET_SLOT] = FindKid( ID_CTRLINST_TXT1 )->m_strText;
	m_pTitles[RESOURCE_SLOT1] = FindKid( ID_CTRLINST_TXT2 )->m_strText;
	m_pTitles[RESOURCE_SLOT2] = FindKid( ID_CTRLINST_TXT3 )->m_strText;
	m_pTitles[RESOURCE_SLOT3] = FindKid( ID_CTRLINST_TXT4 )->m_strText;
	m_pTitles[SLOT_COUNT] = FindKid( ID_CTRLINST_TITLE )->m_strText;

	m_pToolTips[TARGET_SLOT] = CTChart::LoadString( TSTR_REFINE_TARGET_TOOLTIP );
	m_pToolTips[RESOURCE_SLOT1] = CTChart::LoadString( TSTR_REFINE_OFFERING_TOOLTIP );
	m_pToolTips[RESOURCE_SLOT2] = CTChart::LoadString( TSTR_REFINE_OFFERING_TOOLTIP );
	m_pToolTips[RESOURCE_SLOT3] = CTChart::LoadString( TSTR_REFINE_OFFERING_TOOLTIP );
	m_pToolTips[SLOT_COUNT] = CTChart::LoadString( TSTR_REFINE_FRAME_TOOLTIP );

	ClearItem();
	ShowComponent(FALSE);
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTItemRefineDlg::~CTItemRefineDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ----------------------------------------------------------------------------------------------------
void CTItemRefineDlg::UpdateState_ButtonOK()
{
	m_pButtonOK->EnableComponent( FALSE );
	m_pButtonOK->SetTextClr( 0xFF888888 );

	if( m_ItemSlot[TARGET_SLOT].pItem )
	{
		int i = RESOURCE_SLOT1;
		for( ; i < SLOT_COUNT; ++i )
		{
			if( m_ItemSlot[i].pItem )
			{
				m_pButtonOK->EnableComponent( TRUE );
				m_pButtonOK->SetTextClr( TCOLOR_TXT_NORMAL );

				return ;
			}
		}
	}
}
// ====================================================================================================
void CTItemRefineDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTItemRefineDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
BOOL CTItemRefineDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	if( pClientItem->CanGamble() )
	{
		m_strLastError = CTChart::LoadString( TSTR_ITEM_ERR_GAMBLE);
		return FALSE;
	}

	if( pClientItem->GetWrap() )
	{
		m_strLastError = CTChart::LoadString(TSTR_CANT_USE_WRAPED_ITEM);
		return FALSE;
	}

	if( pClientItem->GetRefineMax() == 0 &&
		pClientItem->GetTITEM()->m_bType != IT_REFINE )
	{
		m_strLastError = CTChart::LoadString( TSTR_ITRF_ERR_CANNOTREFINE);
		return FALSE;
	}

	switch( bSlotID )
	{
	case TARGET_SLOT	:  
		if( pClientItem->GetRefineMax() <= pClientItem->GetRefineCurrent() )
		{
			m_strLastError = CTChart::LoadString( TSTR_ITRF_ERR_MAXREFINE);
			return FALSE;
		}
		break;
	}

	for(BYTE i=0; i<SLOT_COUNT; ++i)
	{
		if( bSlotID != i && 
			m_ItemSlot[i].pItem &&
			GetLastClientItem(i) == pClientItem )
		{
			m_strLastError = CTChart::LoadString( TSTR_ITRF_ERR_DUPLICATE);
			return FALSE;
		}
	}

	return TRUE;
}
// ====================================================================================================





















