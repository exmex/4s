#include "Stdafx.h"
#include "TCashShopDlg.h"
#include "Resource.h"

// CTCashShopSlot
// ====================================================================================================
void CTCashShopSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	
	m_pIcon->ShowComponent(bShow);
	m_pName->ShowComponent(bShow);
	m_pCash->ShowComponent(bShow);

	if( m_pBestIcon && m_pNewIcon )
		if( m_bShow )
		{
			switch( m_bKind )
			{
			case CASHSHOPITEM_KIND_BEST:
				m_pBestIcon->ShowComponent(TRUE);
				m_pNewIcon->ShowComponent(FALSE);
				break;
			case CASHSHOPITEM_KIND_NEW:
				m_pBestIcon->ShowComponent(FALSE);
				m_pNewIcon->ShowComponent(TRUE);
				break;
			default:
				m_pBestIcon->ShowComponent(FALSE);
				m_pNewIcon->ShowComponent(FALSE);
				break;
			}
		}
		else
		{
			m_pBestIcon->ShowComponent(FALSE);
			m_pNewIcon->ShowComponent(FALSE);
		}

	m_pBase->ShowComponent(bShow);
}
// ----------------------------------------------------------------------------------------------------
void CTCashShopSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ====================================================================================================

// CTCashShopItem
// ====================================================================================================
CTCashShopItem* CTCashShopItem::Clone()
{
	CTCashShopItem* pCopy = new CTCashShopItem;

	pCopy->m_nIndex			= m_nIndex;
	pCopy->m_wItemID		= m_wItemID;
	pCopy->m_bItemCnt		= m_bItemCnt;
	pCopy->m_wCashItemID	= m_wCashItemID;
	pCopy->m_dwPrice		= m_dwPrice;
	pCopy->m_wUseDay		= m_wUseDay;
	pCopy->m_bKind			= m_bKind;
	
	pCopy->m_pClientItem = new CTClientItem;
	*(pCopy->m_pClientItem) = *m_pClientItem;
	
	return pCopy;
}
// ====================================================================================================
void CTCashShopItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	CTCashShopSlot* pCabinetSlot = static_cast<CTCashShopSlot*>(pSlot);

	if( !m_pClientItem )
	{
		pCabinetSlot->m_pIcon->ShowComponent(FALSE);
		pCabinetSlot->m_pName->ShowComponent(FALSE);
		pCabinetSlot->m_pCash->ShowComponent(FALSE);
		if( pCabinetSlot->m_pBestIcon )
			pCabinetSlot->m_pBestIcon->ShowComponent(FALSE);
		if( pCabinetSlot->m_pNewIcon )
			pCabinetSlot->m_pNewIcon->ShowComponent(FALSE);
	}
	else
	{
		LPTITEM pTItem = m_pClientItem->GetTITEM();
		
		LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTItem);
		WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

		pCabinetSlot->m_pIcon->SetCurImage(wImg);

		DWORD qt = m_pClientItem->GetQuality();

		if( m_pClientItem->GetWrap() )
			pCabinetSlot->m_pIcon->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
		else if ( qt & TITEM_QUALITY_GRADE )
			pCabinetSlot->m_pIcon->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pClientItem->GetGrade() );
		else if ( qt & TITEM_QUALITY_RARE )
			pCabinetSlot->m_pIcon->SetSkinImage( TITEM_INDEX_RARE_SKIN );
		else if ( qt & TITEM_QUALITY_MAGIC )
			pCabinetSlot->m_pIcon->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
		else
			pCabinetSlot->m_pIcon->SetSkinImageEmpty();

		if( m_wUseDay)
			pCabinetSlot->m_pName->m_strText = CTChart::Format( TSTR_ITEM_NAME_PERIOD, (LPCTSTR)pTItem->m_strNAME, m_wUseDay);
		else
			pCabinetSlot->m_pName->m_strText = pTItem->m_strNAME;

		if ( CTNationOption::RUSSIA )
		{
			FLOAT fPrice = m_dwPrice / 100.0f;
			pCabinetSlot->m_pCash->m_strText = CTChart::Format( TSTR_FMT_CASH_FLOAT, fPrice );
		}
		else
			pCabinetSlot->m_pCash->m_strText = CTChart::Format( TSTR_FMT_CASH, m_dwPrice);

		if( m_pClientItem->GetCount() > 1 )
			pCabinetSlot->m_pIcon->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pClientItem->GetCount());
		else
			pCabinetSlot->m_pIcon->m_strText.Empty();

		pCabinetSlot->m_bKind = m_bKind;		
	}
}
// ----------------------------------------------------------------------------------------------------
CTCashShopItem::~CTCashShopItem()
{
	if( m_pClientItem )
		delete m_pClientItem;
}