#include "stdafx.h"
#include "TMagicItemShopDlg.h"
#include "TShopItem.h"

#ifdef MAGICITEMSHOP


CTMagicItemShopDlg::CTMagicItemShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
: CTShopBaseDlg(pParent, pDesc)
{

}

CTMagicItemShopDlg::~CTMagicItemShopDlg()
{

}

ITDetailInfoPtr CTMagicItemShopDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;
	
	CTCtrlListItem* pHitItem = m_pList->GetHitItem( point );
	if( pHitItem )
	{
		CTMagicShopItem* pMagicItem = static_cast<CTMagicShopItem*>( pHitItem );

		CRect rc;
		GetComponentRect(&rc);

		pInfo = CTDetailInfoManager::NewItemInst(pMagicItem->m_pClientItem, rc);
		pInfo->SetDir(TRUE, FALSE, TRUE);
	}

	return pInfo;
}

BOOL CTMagicItemShopDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem( point );
	if( pHitItem )
	{
		CTMagicShopItem* pMagicItem = static_cast<CTMagicShopItem*>( pHitItem );
		LPTITEM pTITEM = pMagicItem->m_pClientItem->GetTITEM();
		if( pTITEM )
		{
			outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
			outInfo.m_pItem = pTITEM;
			outInfo.m_pClientItem = pMagicItem->m_pClientItem;

			return TRUE;
		}
	}

	return FALSE;
}

TDROPINFO CTMagicItemShopDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;
	vResult.m_bDrop = TRUE;
	return vResult;
}

BYTE CTMagicItemShopDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem( point );
	if( !pHitItem )
		return FALSE;

	LPTITEM pTITEM = static_cast<CTMagicShopItem*>( pHitItem )->m_pClientItem->GetTITEM();
	if( !pTITEM )
		return FALSE;

	if( pDRAG )
	{
		CTShopSlot* pSlot0 = static_cast<CTShopSlot*>(m_pList->GetSlot(0));
		pDRAG->m_pIMAGE = new TImageList(NULL, pSlot0->m_pIcon->m_pDESC);

		LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual( pTITEM );
		WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

		pDRAG->m_pIMAGE->SetCurImage(wImg);
		pDRAG->m_bSlotID = pHitItem->m_nIndex;

		CPoint pt = point;
		CSize size;

		pDRAG->m_pIMAGE->GetComponentSize(&size);
		pt.x -= size.cx / 2;
		pt.y -= size.cy / 2;

		pDRAG->m_pIMAGE->ShowComponent(FALSE);
		pDRAG->m_pIMAGE->MoveComponent(pt);
	}

	return TRUE;
}

#endif