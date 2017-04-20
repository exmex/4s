#include "stdafx.h"
#include "TItemUpDlg.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TColorPicker.h"

// ====================================================================================================
CTItemUpDlg::CTItemUpDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
:	CTItemSlotBaseDlg(pParent, pDesc, SLOT_COUNT)
{
	m_pSlotCtrl[TARGET_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITUP_SLOT1) );
	m_pSlotCtrl[SCROLL_SLOT] = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITUP_SLOT2) );

	m_pSlotImage[TARGET_SLOT] = FindKid( ID_CTRLINST_SLOT1_IMAGE );
	m_pSlotImage[SCROLL_SLOT] = FindKid( ID_CTRLINST_SLOT2_IMAGE );

	m_pTitles[TARGET_SLOT] = FindKid( ID_CTRLINST_TEX1 )->m_strText;
	m_pTitles[SCROLL_SLOT] = FindKid( ID_CTRLINST_TEX2 )->m_strText;
	m_pTitles[SLOT_COUNT] = FindKid( ID_CTRLINST_ITUP_MAINTITLE )->m_strText;

	m_pToolTips[TARGET_SLOT] = CTChart::LoadString( TSTR_UPGRADE_TARGET_TOOLTIP ); // SLOT1 mouse over
	m_pToolTips[SCROLL_SLOT] = CTChart::LoadString( TSTR_UPGRADE_SCROLL_TOOLTIP ); // SLOT2 mouse over
	m_pToolTips[SLOT_COUNT] = CTChart::LoadString( TSTR_UPGRADE_FRAME_TOOLTIP ); // FRAME mouse over

	ClearItem();
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTItemUpDlg::~CTItemUpDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTItemUpDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTItemUpDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
BOOL CTItemUpDlg::CheckItem(CTClientItem* pTargetItem, CTClientItem* pScrollItem) const
{
	LPTITEM pTargetItemTemp = pTargetItem->GetTITEM();
	LPTITEM pScrollItemTemp = pScrollItem->GetTITEM();

	if( !CanUpgrade(pScrollItemTemp, pTargetItemTemp) )
		return FALSE;

	DWORD dwTargetQt = pTargetItem->GetQuality();
	switch( pScrollItemTemp->m_bKind )
	{
	case IK_UPGRADE:	
		{
			if( (dwTargetQt&TITEM_QUALITY_MAGIC) || (dwTargetQt&TITEM_QUALITY_RARE) )
			{
				if( pTargetItem->HasMainMagicOption() )
				{
					m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_GRADEMAGIC);
					return FALSE;
				}

				return TRUE;
			}

			if( (dwTargetQt&TITEM_QUALITY_NORMAL) || (dwTargetQt&TITEM_QUALITY_GRADE) )
				return TRUE;
			else
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTGRADE);
		}
		break;

	case IK_DOWNGRADE:	
		{
			if( dwTargetQt&TITEM_QUALITY_GRADE )
			{
				if( (dwTargetQt&TITEM_QUALITY_MAGIC) || (dwTargetQt&TITEM_QUALITY_RARE) )
				{
					if( pTargetItem->HasMainMagicOption() )
					{
						m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_GRADEMAGIC);
						return FALSE;
					}
				}

				return TRUE;
			}
			else
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_DOWNGRADE);
		}
		break;

	case IK_MAGICGRADE:
		{
			if( dwTargetQt&TITEM_QUALITY_NORMAL )
				return TRUE;
			else if( dwTargetQt&TITEM_QUALITY_GRADE )
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICGRADE);
			else if( dwTargetQt&TITEM_QUALITY_MAGIC )
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICMAGIC);
			else if( dwTargetQt&TITEM_QUALITY_RARE )
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICRARE);
		}
		break;

	case IK_CLEARMAGIC:
		{
			if( !pTargetItem->GetTITEM()->m_bCanMagic )
			{
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTREMOVEMAGIC );
				return FALSE;
			}

			if( pTargetItem->GetTMAGIC()->size() > 0 )
			{
				return TRUE;
			}

			m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_DONT_HAVE_MAGIC );		
			return FALSE;
		}
		break;

	case IK_RAREGRADE:	
		{
			if( dwTargetQt&TITEM_QUALITY_GRADE )
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICGRADE);
			else if( dwTargetQt&TITEM_QUALITY_RARE )
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_RARE);
			else if( dwTargetQt&TITEM_QUALITY_MAGIC )
				return TRUE;
			else
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICRARE);
		}

		break;

	case IK_WRAP:
		{
			if( pTargetItem->GetWrap() )
			{
				m_strLastError = CTChart::LoadString( TSTR_ALREADY_WRAP );
				return FALSE;
			}

			return TRUE;
		}
		break;
	
	case IK_ELD:
		{
			if( pTargetItem->GetELD() < TMAX_ITEMLEVELDOWN )
			{
				return TRUE;
			}
			else
			{
				m_strLastError = CTChart::LoadString( TSTR_MAXELD );
				return FALSE;
			}
		}
		break;

	case IK_CLEARREFINE: //제련초기화
		{
			if( pTargetItem->GetRefineCurrent() > 0 )
				return TRUE;
			else
			{
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_NOREFINE );
				return FALSE;
			}
		}
		break;

	case IK_CHGGRADEEFFECT: //강화이펙트변경
		{
			if( 17 <= pTargetItem->GetGrade() && pTargetItem->GetGrade() <= 24 )
				return TRUE;
			else
			{
				m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_UNDER17 );
				return FALSE;
			}
		}
		break;

	case IK_COLOR:
		{
			if( pTargetItemTemp->m_dwSlotID == 16 )
				return TRUE;
		}
		break;

	default :
		{
			m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_INVALIDITEM);
		}
		break;
	}

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTItemUpDlg::CanUpgrade( LPTITEM pScrollItemTemp, LPTITEM pTargetItemTemp ) const
{
	if( !pScrollItemTemp || !pTargetItemTemp )
		return FALSE;

	switch( pScrollItemTemp->m_bKind )
	{
	case IK_UPGRADE		:
	case IK_DOWNGRADE	:	
		if( !pTargetItemTemp->m_bCanGrade )
		{
			m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTGRADE);
			return FALSE;
		}
		return TRUE;

	case IK_MAGICGRADE	:
		if( !pTargetItemTemp->m_bCanMagic )
		{
			m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTMAGIC);
			return FALSE;
		}
		return TRUE;

	case IK_RAREGRADE	:	
		if( !pTargetItemTemp->m_bCanRare )
		{
			m_strLastError = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTRARE);
			return FALSE;
		}
		return TRUE;

	case IK_WRAP:
		{
			if( !pTargetItemTemp->m_bCanWrap )
			{
				m_strLastError = CTChart::LoadString( TSTR_CANT_WRAP_ITEM );
				return FALSE;
			}
		}
		return TRUE;

	case IK_CLEARMAGIC:
	case IK_ELD:
	case IK_CLEARREFINE: //제련초기화
	case IK_CHGGRADEEFFECT: //강화이펙트변경
		return TRUE;

	case IK_COLOR:
		{
			if( !pTargetItemTemp->m_bCanColor )
				return FALSE;
		}
		return TRUE;

	default:
		return FALSE;
	}

	return TRUE;
}
// ====================================================================================================

// ====================================================================================================
BOOL CTItemUpDlg::CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const
{
	LPTITEM pItem = pClientItem->GetTITEM();

	if( pClientItem->CanGamble() )
	{
		m_strLastError = CTChart::LoadString( TSTR_ITEM_ERR_GAMBLE);
		return FALSE;
	}

	switch( bSlotID )
	{
	case TARGET_SLOT: 
		{
			if( pClientItem->GetWrap() )
				return FALSE;

			if( m_ItemSlot[SCROLL_SLOT].pItem )
				return CheckItem(pClientItem, GetLastClientItem(SCROLL_SLOT));
			else
				return (pItem->m_bCanGrade || pItem->m_bCanMagic || pItem->m_bCanRare || pItem->m_bCanColor );
		}
		break;
		
	case SCROLL_SLOT: 
		{
			if( pClientItem->GetWrap() )
				return FALSE;

			if( pItem->m_bType != IT_GRADE )
				return FALSE;
			else
			{
				if( m_ItemSlot[TARGET_SLOT].pItem )
				{
					CTClientItem* pUpItem = GetLastClientItem(TARGET_SLOT);
					if( !pUpItem )
						return FALSE;

					return CheckItem(pUpItem, pClientItem);
				}
				else
					return TRUE;
			}
		}
		break;
	}

	return FALSE;
}
// ====================================================================================================
BOOL CTItemUpDlg::SetItem(BYTE bSlotID, LPTITEM pItem, BYTE bInven, BYTE bInvenSlot)
{
	CTClientGame* pTGAME = CTClientGame::GetInstance();

	BOOL bRet = CTItemSlotBaseDlg::SetItem( bSlotID, pItem, bInven, bInvenSlot );

	if( bRet )
	{
		if( bSlotID == SCROLL_SLOT )
		{
			CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( pTGAME->GetFrame( TFRAME_COLOR_PICKER ) );

			if( pItem && pItem->m_bKind == IK_COLOR )
			{
				pColorPicker->ShowComponent( TRUE );
				pColorPicker->SetBindTarget( this );
				pColorPicker->SetCanSelect(pItem->m_wUseValue);
				pColorPicker->SelectColor( 1 );
			}
			else
			{
				pColorPicker->ShowComponent( FALSE );
				pColorPicker->SetBindTarget( NULL );
			}
		}
	}

	return bRet;
}

void CTItemUpDlg::ShowComponent( BOOL bVisible )
{
	if( !bVisible )
	{
		CTClientGame* pTGAME = CTClientGame::GetInstance();
		CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( pTGAME->GetFrame( TFRAME_COLOR_PICKER ) );
		pColorPicker->ShowComponent( FALSE );
		pColorPicker->SetBindTarget( NULL );
	}

	CTItemSlotBaseDlg::ShowComponent( bVisible );
}







