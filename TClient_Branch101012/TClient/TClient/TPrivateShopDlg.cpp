#include "Stdafx.h"
#include "TPrivateShopDlg.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"

#include "StdAfx.h"
#include "Resource.h"

// CTPrivateShopDlg
// ====================================================================================================
CTPrivateShopDlg::CTPrivateShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTShopBaseDlg(pParent, pDesc)
{
	m_pOk->m_strText = CTChart::LoadString( TSTR_PRIVATE_SHOP_BUY);
	m_pOk->m_menu[TNM_LCLICK] = GM_PRIVATE_SHOP_BUY;
}
// ----------------------------------------------------------------------------------------------------
CTPrivateShopDlg::~CTPrivateShopDlg()
{
}
// ====================================================================================================
ITDetailInfoPtr CTPrivateShopDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( pHitItem )
	{
		CTPrivateShopItem* pPcItem = static_cast<CTPrivateShopItem*>(pHitItem);
		if( pPcItem->m_pClientItem )
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewItemInst(pPcItem->m_pClientItem, rc);
			pInfo->SetDir(TRUE, FALSE, TRUE);
		}
	}

	return pInfo;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTPrivateShopDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	CTPrivateShopItem* pPcItem = static_cast<CTPrivateShopItem*>(pHitItem);

	if( pPcItem->m_pClientItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pPcItem->m_pClientItem->GetTITEM();
		outInfo.m_pClientItem = pPcItem->m_pClientItem;
		
		return TRUE;
	}

	return FALSE;
}
// ====================================================================================================
TDROPINFO CTPrivateShopDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;
	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTPrivateShopDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	return FALSE;
}
// ====================================================================================================
INT CTPrivateShopDlg::AddItem(CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot)
{
	CTClientItem* pNewItem = new CTClientItem;
	
	*pNewItem = *pItem;

	CTPrivateShopItem* pInfo = new CTPrivateShopItem;
	pInfo->m_pClientItem = pNewItem;

	if( m_pList->IsItemEmpty() )
		pInfo->m_bNetID = 0;
	else
	{
		CTPrivateShopItem* pLastInfo = static_cast<CTPrivateShopItem*>
			(m_pList->GetItem( m_pList->GetItemCnt()-1 ));

		pInfo->m_bNetID = pLastInfo->m_bNetID + 1;
	}

	pInfo->m_bItemCnt = pNewItem->GetCount();
	
	pInfo->m_dwRune = 0;
	pInfo->m_dwLuna = 0;
	pInfo->m_dwCron = 0;

	pInfo->m_bInven = bInven;
	pInfo->m_bInvenSlot = bInvenSlot;

	return m_pList->AddItem(pInfo);
}
// ----------------------------------------------------------------------------------------------------
CTPrivateShopItem* CTPrivateShopDlg::AddEmptyItem()
{
	CTClientItem* pNewItem = new CTClientItem;
	pNewItem->SetItemSlot((BYTE)0);
	pNewItem->SetGrade(0);
	pNewItem->SetTITEM(0);
	pNewItem->SetCount(0);

	CTPrivateShopItem* pInfo = new CTPrivateShopItem;
	pInfo->m_pClientItem = pNewItem;

	pInfo->m_bNetID = 0;
	pInfo->m_bItemCnt = 0;
	pInfo->m_dwRune = 0;
	pInfo->m_dwLuna = 0;
	pInfo->m_dwCron = 0;
	pInfo->m_bInven = 0;
	pInfo->m_bInvenSlot = 0;

	m_pList->AddItem(pInfo);

	return pInfo;
}
// ====================================================================================================
void CTPrivateShopDlg::RemoveItem(INT nIdx)
{
	m_pList->RemoveItem(nIdx);
}
// ----------------------------------------------------------------------------------------------------
void CTPrivateShopDlg::Clear()
{
	m_pList->ClearItem();
}
// ====================================================================================================
CTPrivateShopItem* CTPrivateShopDlg::GetCurSelItem()
{
	return static_cast<CTPrivateShopItem*>( m_pList->GetSelectItem() );
}
// ----------------------------------------------------------------------------------------------------
UINT CTPrivateShopDlg::GetItemCount() const
{
	return m_pList->GetItemCnt();
}
// ----------------------------------------------------------------------------------------------------
INT CTPrivateShopDlg::GetItemIdxByInvenPos(BYTE bInven, BYTE bInvenSlot)
{
	INT nCnt = m_pList->GetItemCnt();
	for(INT i=0; i<nCnt; ++i)
	{
		CTPrivateShopItem* pInfo = static_cast<CTPrivateShopItem*>(m_pList->GetItem(i));
		if( pInfo->m_bInven == bInven && pInfo->m_bInvenSlot == bInvenSlot )
			return i;
	}

	return T_INVALID;
}
// ----------------------------------------------------------------------------------------------------
INT CTPrivateShopDlg::GetItemIdxByNetID(BYTE bNetID)
{
	INT nCnt = m_pList->GetItemCnt();
	for(INT i=0; i<nCnt; ++i)
	{
		CTPrivateShopItem* pInfo = static_cast<CTPrivateShopItem*>(m_pList->GetItem(i));
		if( pInfo->m_bNetID == bNetID )
			return i;
	}

	return T_INVALID;
}
// ----------------------------------------------------------------------------------------------------
CTPrivateShopItem* CTPrivateShopDlg::GetItem(INT nIndex)
{
	return static_cast<CTPrivateShopItem*>( m_pList->GetItem(nIndex) );
}
// ====================================================================================================
void CTPrivateShopDlg::ChangeItemInvenPos(BYTE bFromInven, BYTE bFromInvenSlot, BYTE bToInven, BYTE bToInvenSlot)
{
	INT nIndex1 = GetItemIdxByInvenPos( bFromInven, bFromInvenSlot );
	INT nIndex2 = GetItemIdxByInvenPos( bToInven, bToInvenSlot );

	if( nIndex1 != T_INVALID )
	{
		CTPrivateShopItem* pInfo = GetItem(nIndex1);
		if( pInfo )
		{
			pInfo->m_bInven = bToInven;
			pInfo->m_bInvenSlot = bToInvenSlot;
		}
	}

	if( nIndex2 != T_INVALID )
	{
		CTPrivateShopItem* pInfo = GetItem(nIndex2);
		if( pInfo )
		{
			pInfo->m_bInven = bFromInven;
			pInfo->m_bInvenSlot = bFromInvenSlot;	
		}
	}
}
// ----------------------------------------------------------------------------------------------------
INT CTPrivateShopDlg::ReduceItemCount(BYTE bNetID, BYTE bCount)
{
	INT nIndex = GetItemIdxByNetID(bNetID);
	if( nIndex != T_INVALID )
	{
		NotifyUpdate();

		CTPrivateShopItem* pInfo = GetItem(nIndex);
		if( pInfo->m_bItemCnt <= bCount )
		{
			RemoveItem(nIndex);
			return 0;
		}
		else
		{
			pInfo->m_bItemCnt -= bCount;
			return pInfo->m_bItemCnt;
		}
	}

	return T_INVALID;
}
// ====================================================================================================


// CTPrivateShopDlgForSell
// ====================================================================================================
CTPrivateShopDlgForSell::CTPrivateShopDlgForSell(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTPrivateShopDlg(pParent, pDesc), m_bSelling(FALSE)
{
	m_pShopNameOut	= FindKid(ID_CTRLINST_SHOPNAME_OUT);
	m_pShopNameIn	= static_cast<TEdit*>( FindKid(ID_CTRLINST_SHOPNAME_IN) );

	m_pWarn			= FindKid(ID_CTRLINST_WARN);

	m_pShopNameOut->ShowComponent(FALSE);
	m_pShopNameOut->EnableComponent(FALSE);
	
	m_pShopNameIn->ShowComponent(TRUE);
	m_pShopNameIn->EnableComponent(TRUE);

	m_pOk->m_strText = CTChart::LoadString( TSTR_PRIVATE_SHOP_START);
	m_pOk->m_menu[TNM_LCLICK] = GM_PRIVATE_SHOP_START;
}
// ----------------------------------------------------------------------------------------------------
CTPrivateShopDlgForSell::~CTPrivateShopDlgForSell()
{
}
// ====================================================================================================
void CTPrivateShopDlgForSell::StartSelling(BOOL bStart)
{
	if( m_bSelling == bStart )
		return;

	m_bSelling = bStart;
	
	if( m_bSelling )
	{
		m_pShopNameOut->m_strText = m_pShopNameIn->m_strText;

		m_pShopNameOut->ShowComponent(IsVisible());
		m_pShopNameOut->EnableComponent(TRUE);

		m_pShopNameIn->ShowComponent(FALSE);
		m_pShopNameIn->EnableComponent(FALSE);

		m_pOk->m_strText = CTChart::LoadString( TSTR_PRIVATE_SHOP_END);
		m_pOk->m_menu[TNM_LCLICK] = GM_PRIVATE_SHOP_END;
	}
	else
	{
		m_pShopNameOut->ShowComponent(FALSE);
		m_pShopNameOut->EnableComponent(FALSE);

		m_pShopNameIn->ShowComponent(IsVisible());
		m_pShopNameIn->EnableComponent(TRUE);

		m_pOk->m_strText = CTChart::LoadString( TSTR_PRIVATE_SHOP_START);
		m_pOk->m_menu[TNM_LCLICK] = GM_PRIVATE_SHOP_START;
	}
}
// ====================================================================================================
void CTPrivateShopDlgForSell::ClearShopName()
{
	m_pShopNameOut->m_strText.Empty();
	m_pShopNameIn->ClearText();
}
// ----------------------------------------------------------------------------------------------------
void CTPrivateShopDlgForSell::SetShopName(const CString& strShopName)
{
	m_pShopNameOut->m_strText = strShopName;
	m_pShopNameIn->SetText(strShopName);
}
// ----------------------------------------------------------------------------------------------------
const CString& CTPrivateShopDlgForSell::GetShopName() const
{
	if( m_bSelling )
		return m_pShopNameOut->m_strText;
	else
		return m_pShopNameIn->m_strText;
}
// ====================================================================================================
void CTPrivateShopDlgForSell::SetWarn(const CString& strWarn)
{
	m_pWarn->m_strText = strWarn;
}
// ----------------------------------------------------------------------------------------------------
const CString& CTPrivateShopDlgForSell::GetWarn() const
{
	return m_pWarn->m_strText;
}
// ====================================================================================================
TEdit* CTPrivateShopDlgForSell::GetCurEdit()
{
	if( IsVisible() && m_bFocus && !m_bSelling && m_pShopNameIn->GetFocus() )
		return m_pShopNameIn;

	return NULL;
}
// ====================================================================================================
void CTPrivateShopDlgForSell::Clear()
{
	m_pList->ClearItem();
	
	ClearShopName();
	StartSelling(FALSE);
}
// ====================================================================================================
void CTPrivateShopDlgForSell::ShowComponent( BOOL bVisible)
{
	if( m_bVisible == bVisible )
		return;

	CTPrivateShopDlg::ShowComponent(bVisible);

	if( m_bVisible )
	{
		if( m_bSelling )
		{
			m_pShopNameOut->m_strText = m_pShopNameIn->m_strText;

			m_pShopNameOut->ShowComponent(TRUE);
			m_pShopNameIn->ShowComponent(FALSE);
		}
		else
		{
			m_pShopNameOut->ShowComponent(FALSE);
			m_pShopNameIn->ShowComponent(TRUE);
		}
	}
}
// ====================================================================================================
void CTPrivateShopDlgForSell::OnChar(UINT nChar, int nRepCnt, UINT nFlags)
{
	if( !m_bSelling )
		CTPrivateShopDlg::OnChar(nChar, nRepCnt, nFlags);
}
// ----------------------------------------------------------------------------------------------------
void CTPrivateShopDlgForSell::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if( !m_bSelling )
		CTPrivateShopDlg::OnImeComposition(hWnd, wParam, lParam);
}
// ----------------------------------------------------------------------------------------------------
void CTPrivateShopDlgForSell::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	if( !m_bSelling )
		CTPrivateShopDlg::OnLButtonDblClk(nFlags, pt);
}
// ====================================================================================================
TDROPINFO CTPrivateShopDlgForSell::OnDrop(CPoint point)
{
	TDROPINFO vResult;

	if( m_bDropTarget && !IsSelling() )
		vResult.m_bDrop = TRUE;

	return vResult;
}
// ====================================================================================================
BYTE CTPrivateShopDlgForSell::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	if( IsSelling() )
		return FALSE;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	LPTITEM pTITEM = NULL;
	if( static_cast<CTPrivateShopItem*>(pHitItem)->m_pClientItem )
		pTITEM = static_cast<CTPrivateShopItem*>(pHitItem)->m_pClientItem->GetTITEM();

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




