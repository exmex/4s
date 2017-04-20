#include "Stdafx.h"
#include "TShopItem.h"
#include "TTradeDlg.h"
#include "TRentSkillDlg.h"
#include "Resource.h"

// ====================================================================================================
void CTTradeItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pClientItem )
		return;

	CTTradeSlot* pTradeSlot = static_cast<CTTradeSlot*>(pSlot);
	LPTITEM pTItem = m_pClientItem->GetTITEM();
	
	LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTItem);
	WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

	pTradeSlot->m_pIcon->SetCurImage(wImg);

	DWORD qt = m_pClientItem->GetQuality();

	if( m_pClientItem->GetWrap() )
		pTradeSlot->m_pIcon->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
	else if ( qt & TITEM_QUALITY_GRADE )
		pTradeSlot->m_pIcon->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pClientItem->GetGrade() );
	else if ( qt & TITEM_QUALITY_RARE )
		pTradeSlot->m_pIcon->SetSkinImage( TITEM_INDEX_RARE_SKIN );
	else if ( qt & TITEM_QUALITY_MAGIC )
		pTradeSlot->m_pIcon->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
	else
		pTradeSlot->m_pIcon->SetSkinImageEmpty();

	pTradeSlot->m_pName->m_strText = pTItem->m_strNAME;

	if( m_pClientItem->GetCount() > 1 )
		pTradeSlot->m_pIcon->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pClientItem->GetCount());
	else
		pTradeSlot->m_pIcon->m_strText.Empty();
}
// ----------------------------------------------------------------------------------------------------
CTTradeItem::~CTTradeItem()
{
	if( m_pClientItem )
		delete m_pClientItem;
}
// ====================================================================================================
void CTShopNpcItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pTItem )
		return;
	
	CTShopSlot* pShopSlot = static_cast<CTShopSlot*>(pSlot);
	
	LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(m_pTItem);
	WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

	pShopSlot->m_pIcon->SetCurImage(wImg);
	pShopSlot->m_pName->m_strText = m_pTItem->m_strNAME;
	pShopSlot->m_pIcon->m_strText.Empty();

	DWORD dwPRICE = m_pTItem->m_dwPrice;

	if( pShopSlot->m_dwUserData == TSHOP_PHURCHASE_TYPE_POINT)
	{
		pShopSlot->m_pPoint->m_strText = CTChart::Format( TSTR_FMT_PVPPOINT,
			dwPRICE );

		pShopSlot->m_pRune->m_strText.Empty();
		pShopSlot->m_pLuna->m_strText.Empty();
		pShopSlot->m_pCron->m_strText.Empty();
	}
	else
	{
		pShopSlot->m_pPoint->m_strText.Empty();

		pShopSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			dwPRICE / TRUNE_DIVIDER);

		pShopSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			(dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER);

		pShopSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			dwPRICE % TLUNA_DIVIDER);
	}
}
// ====================================================================================================
void CTShopPortal::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pTPortal )
		return;

	CTShopSlot* pShopSlot = static_cast<CTShopSlot*>(pSlot);
	
	pShopSlot->m_pIcon->SetCurImage(TPORTAL_IMAGEID);
	pShopSlot->m_pName->m_strText = m_pTPortal->m_strNAME;
	pShopSlot->m_pIcon->m_strText.Empty();

	DWORD dwPRICE = m_pTPortal->m_dwPrice;
	pShopSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		dwPRICE / TRUNE_DIVIDER);

	pShopSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		(dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER);

	pShopSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		dwPRICE % TLUNA_DIVIDER);
}
// ====================================================================================================
void CTPrivateShopItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pClientItem )
		return;

	CTShopSlot* pShopSlot = static_cast<CTShopSlot*>(pSlot);
	LPTITEM pTItem = m_pClientItem->GetTITEM();
	
	LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTItem);
	WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

	pShopSlot->m_pIcon->SetCurImage(wImg);

	DWORD qt = m_pClientItem->GetQuality();

	if( m_pClientItem->GetWrap() )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
	else if ( qt & TITEM_QUALITY_GRADE )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pClientItem->GetGrade() );
	else if ( qt & TITEM_QUALITY_RARE )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_RARE_SKIN );
	else if ( qt & TITEM_QUALITY_MAGIC )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
	else
		pShopSlot->m_pIcon->SetSkinImageEmpty();

	pShopSlot->m_pName->m_strText = pTItem->m_strNAME;

	m_dwLuna = min( m_dwLuna, 999 );
	m_dwCron = min( m_dwCron, 999 );

	pShopSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwRune); 
	pShopSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwLuna);
	pShopSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwCron);

	if( m_bItemCnt > 1 )
		pShopSlot->m_pIcon->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_bItemCnt);
	else
		pShopSlot->m_pIcon->m_strText.Empty();
}
// ----------------------------------------------------------------------------------------------------
CTPrivateShopItem::~CTPrivateShopItem()
{
	if( m_pClientItem )
		delete m_pClientItem;
}
// ====================================================================================================
void CTRentNpcItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pSkill )
		return;
	
	CTRentSkillSlot* pRentSkillSlot = static_cast<CTRentSkillSlot*>(pSlot);
	
	pRentSkillSlot->m_pSkill->SetCurImage(m_pSkill->m_wIconID);
	pRentSkillSlot->m_pName->m_strText = m_pSkill->m_strNAME;
	pRentSkillSlot->m_pSkill->m_strText.Empty();

	pRentSkillSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice / TRUNE_DIVIDER);

	pRentSkillSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		(m_dwRealPrice % TRUNE_DIVIDER) / TLUNA_DIVIDER);

	pRentSkillSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice % TLUNA_DIVIDER);
}
// ====================================================================================================
void CTMonsterShopItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pTMon )
		return;
	
	CTShopSlot* pShopSlot = static_cast<CTShopSlot*>(pSlot);
	
	pShopSlot->m_pIcon->SetCurImage(m_pTMon->m_dwIcon);
	pShopSlot->m_pName->m_strText = m_pTMon->m_strName;
	pShopSlot->m_pIcon->m_strText.Empty();

	pShopSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice / TRUNE_DIVIDER);

	pShopSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		(m_dwRealPrice % TRUNE_DIVIDER) / TLUNA_DIVIDER);

	pShopSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice % TLUNA_DIVIDER);
}
// ====================================================================================================


#ifdef MAGICITEMSHOP

CTMagicShopItem::~CTMagicShopItem()
{
	if( m_pClientItem )
	{
		delete m_pClientItem;
		m_pClientItem = NULL;
	}
}

void CTMagicShopItem::ReflectSlot( CTCtrlListSlot* pSlot )
{
	if( !m_pClientItem )
		return ;

	CTShopSlot* pShopSlot = static_cast<CTShopSlot*>(pSlot);
	LPTITEM pTItem = m_pClientItem->GetTITEM();

	LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTItem);
	WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

	pShopSlot->m_pIcon->SetCurImage(wImg);

	DWORD qt = m_pClientItem->GetQuality();

	if( m_pClientItem->GetWrap() )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
	else if ( qt & TITEM_QUALITY_GRADE )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pClientItem->GetGrade() );
	else if ( qt & TITEM_QUALITY_RARE )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_RARE_SKIN );
	else if ( qt & TITEM_QUALITY_MAGIC )
		pShopSlot->m_pIcon->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
	else
		pShopSlot->m_pIcon->SetSkinImageEmpty();

	pShopSlot->m_pIcon->m_strText.Empty();

	pShopSlot->m_pName->m_strText = pTItem->m_strNAME;
	pShopSlot->m_pPoint->m_strText.Empty();

	pShopSlot->m_pRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice / TRUNE_DIVIDER);

	pShopSlot->m_pLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		(m_dwRealPrice % TRUNE_DIVIDER) / TLUNA_DIVIDER);

	pShopSlot->m_pCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_dwRealPrice % TLUNA_DIVIDER);
}

#endif











