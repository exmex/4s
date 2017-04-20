#include "stdafx.h"
#include "TItemRegGuildDlg.h"
#include "Resource.h"
#include "TClientGame.h"

// ====================================================================================================
CTItemRegGuildDlg::CTItemRegGuildDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEM) );
	m_pSlotImage[TARGET_SLOT] = FindKid( ID_CTRLINST_WEAPON_IMAGE );

	TComponent* pComp = FindKid( ID_CTRLINST_GAMBLE_IMAGE );
	pComp->ShowComponent(FALSE);
	pComp->EnableComponent(FALSE);

	m_pTitles[TARGET_SLOT] = FindKid( ID_CTRLINST_TEX )->m_strText;
	m_pTitles[SLOT_COUNT] = FindKid( ID_CTRLINST_TITLE )->m_strText;

	m_pToolTips[TARGET_SLOT] = CTChart::LoadString( TSTR_REGGUILD_TARGET_TOOLTIP );
	m_pToolTips[SLOT_COUNT] = CTChart::LoadString( TSTR_REGGUILD_FRAME_TOOLTIP );

	ClearItem();
	ShowComponent(FALSE);
	m_pOpenByCash = NULL;

	FindKid( ID_CTRLINST_CANCEL )->m_menu[ TNM_LCLICK ] = GM_CLOSE_UI;
	FindKid( ID_CTRLINST_CLOSE )->m_menu[ TNM_LCLICK ] = GM_CLOSE_UI;

	SetRegMode( 1 );
}
// ----------------------------------------------------------------------------------------------------
CTItemRegGuildDlg::~CTItemRegGuildDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTItemRegGuildDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTItemRegGuildDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
BOOL CTItemRegGuildDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	if( pItem->m_dwSlotID == 16 )
		return TRUE;

	return FALSE;
}
// ====================================================================================================
void CTItemRegGuildDlg::SetRegMode( int nRegMode )
{
	if( nRegMode )
	{
		m_pTitles[SLOT_COUNT] =
			FindKid( ID_CTRLINST_TITLE )->m_strText = CTChart::LoadString( TSTR_REGGUILDDLG_BTN_REG );

		m_pTitles[TARGET_SLOT] =
			FindKid( ID_CTRLINST_TEX )->m_strText = CTChart::LoadString( TSTR_REGGUILDDLG_TEXT_REG );

		FindKid( ID_CTRLINST_OK )->m_menu[ TNM_LCLICK ] = GM_REGGM_CHECK;
	}
	else
	{
		m_pTitles[SLOT_COUNT] =
			FindKid( ID_CTRLINST_TITLE )->m_strText = CTChart::LoadString( TSTR_REGGUILDDLG_BTN_UNREG );

		m_pTitles[TARGET_SLOT] =
			FindKid( ID_CTRLINST_TEX )->m_strText = CTChart::LoadString( TSTR_REGGUILDDLG_TEXT_UNREG );

		FindKid( ID_CTRLINST_OK )->m_menu[ TNM_LCLICK ] = GM_UNREGGM_CHECK;
	}
}