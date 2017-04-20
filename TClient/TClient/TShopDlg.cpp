#include "StdAfx.h"
#include "TShopItem.h"
#include "TShopDlg.h"
#include "Resource.h"

// ====================================================================================================
CTShopDlg::CTShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc, TSHOP_TYPE eShopType)
	: CTShopBaseDlg(pParent, pDesc), m_eShopType(eShopType)
{
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTShopDlg::~CTShopDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTShopDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTShopDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
void CTShopDlg::Clear()
{
	m_pList->ClearItem();
}
// ====================================================================================================

// ====================================================================================================
ITDetailInfoPtr CTShopDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( m_eShopType == TSHOP_TYPE_NPC_PORTAL ||
		m_eShopType == TSHOP_TYPE_NPC_MONSTER )
		return pInfo;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( pHitItem )
	{
		CTShopNpcItem* pShopItem = static_cast<CTShopNpcItem*>(pHitItem);
		if( pShopItem->m_pTItem )
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewItemInst(pShopItem->m_pTItem, rc);
			pInfo->SetDir(TRUE, FALSE, TRUE);
		}
	}

	return pInfo;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTShopDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	if( m_eShopType == TSHOP_TYPE_NPC_PORTAL ||
		m_eShopType == TSHOP_TYPE_NPC_MONSTER )
		return FALSE;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	CTShopNpcItem* pShopItem = static_cast<CTShopNpcItem*>(pHitItem);
	
	if( pShopItem->m_pTItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pShopItem->m_pTItem;
		outInfo.m_pClientItem = NULL;
		
		return TRUE;
	}

	return FALSE;
}
// ====================================================================================================
TDROPINFO CTShopDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;

	if( m_eShopType != TSHOP_TYPE_NPC_PORTAL &&
		m_eShopType != TSHOP_TYPE_NPC_MONSTER &&
		m_bDropTarget )
		vResult.m_bDrop = TRUE;

	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTShopDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	if( m_eShopType == TSHOP_TYPE_NPC_PORTAL ||
		m_eShopType == TSHOP_TYPE_NPC_MONSTER )
		return FALSE;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	LPTITEM pTITEM = static_cast<CTShopNpcItem*>(pHitItem)->m_pTItem;
	if( !pTITEM )
		return FALSE;

	if( pDRAG )
	{
		CTShopSlot* pSlot0 = static_cast<CTShopSlot*>(m_pList->GetSlot(0));
		pDRAG->m_pIMAGE = new TImageList(NULL, pSlot0->m_pIcon->m_pDESC);

		LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
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
// ====================================================================================================
void CTShopDlg::SetPhurchaseType( TSHOPPHURCHASETYPE ePhurchaseType )
{
	m_ePhurchaseType = ePhurchaseType;
	m_pList->SetAllSlotUserData( (DWORD) ePhurchaseType );
}

