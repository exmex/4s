#include "stdafx.h"
#include "TItemRepairDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemRepairDlg::CTItemRepairDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM) );
	m_pSlotImage[TARGET_SLOT] = FindKid( ID_CTRLINST_SLOT_IMAGE );

	m_pTitles[TARGET_SLOT] = FindKid( ID_CTRLINST_TEX )->m_strText;
	m_pTitles[SLOT_COUNT] = FindKid( ID_CTRLINST_TITLE )->m_strText;

	m_pToolTips[TARGET_SLOT] = CTChart::LoadString( TSTR_REPAIR_TARGET_TOOLTIP );
	m_pToolTips[SLOT_COUNT] = CTChart::LoadString( TSTR_REPAIR_FRAME_TOOLTIP );

	ClearItem();
	ShowComponent(FALSE);
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTItemRepairDlg::~CTItemRepairDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTItemRepairDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTItemRepairDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
BOOL CTItemRepairDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	if( pItem->m_bCanRepair )
	{
		if( pClientItem->GetDuraCurrent() < pClientItem->GetDuraMax() )
			return TRUE;
		else
			m_strLastError = CTChart::LoadString( TSTR_ITEMREP_ERR_NOTNEED);
	}
	else
		m_strLastError = CTChart::LoadString( TSTR_ITEMREP_ERR_CANNOT);
	
	return FALSE;
}
// ====================================================================================================
