#include "stdafx.h"
#include "TItemGambleDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemGambleDlg::CTItemGambleDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM) );
	m_pSlotImage[TARGET_SLOT] = FindKid( ID_CTRLINST_GAMBLE_IMAGE );

	TComponent* pComp = FindKid( ID_CTRLINST_WEAPON_IMAGE );
	pComp->ShowComponent(FALSE);
	pComp->EnableComponent(FALSE);

	m_pTitles[TARGET_SLOT] = FindKid( ID_CTRLINST_TEX )->m_strText;
	m_pTitles[SLOT_COUNT] = FindKid( ID_CTRLINST_TITLE )->m_strText;

	m_pToolTips[TARGET_SLOT] = CTChart::LoadString( TSTR_GAMBLE_TARGET_TOOLTIP );
	m_pToolTips[SLOT_COUNT] = CTChart::LoadString( TSTR_GAMBLE_FRAME_TOOLTIP );

	ClearItem();
	ShowComponent(FALSE);
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTItemGambleDlg::~CTItemGambleDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTItemGambleDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTItemGambleDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
BOOL CTItemGambleDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	if( pItem->m_bType == IT_GAMBLE ||
		pItem->m_bCanGamble ||
		pClientItem->CanGamble() ||
		pClientItem->GetWrap() )
	{
		return TRUE;
	}

	m_strLastError = CTChart::LoadString( TSTR_ITEMGAMBLE_ERR);
	return FALSE;
}
// ====================================================================================================
