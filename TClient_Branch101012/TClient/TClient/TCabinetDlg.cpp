#include "StdAfx.h"
#include "TCabinetDlg.h"
#include "TClientGame.h"
#include "Resource.h"

bool _compare_cabitem(CTCtrlListItem* pL, CTCtrlListItem* pR)
{
	CTCabinetItem* pCL = static_cast<CTCabinetItem*>(pL);
	CTCabinetItem* pCR = static_cast<CTCabinetItem*>(pR);

	LPTITEM pIL = pCL->m_pClientItem->GetTITEM();
	LPTITEM pIR = pCR->m_pClientItem->GetTITEM();

	if(pIL->m_bKind != pIR->m_bKind)
		return (pIL->m_bKind > pIR->m_bKind);

	if(pIL->m_bPrmSlotID != pIR->m_bPrmSlotID)
		return (pIL->m_bPrmSlotID > pIR->m_bPrmSlotID);

	if( pIL->m_bWeaponID != pIR->m_bWeaponID)
		return (pIL->m_bWeaponID > pIR->m_bWeaponID);

	if( pIL->m_bLevel != pIR->m_bLevel)
		return (pIL->m_bLevel > pIR->m_bLevel);
		
	return (pCL->m_dwStItemID > pCR->m_dwStItemID);
}

// CTCabinetSlot
// ====================================================================================================
void CTCabinetSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	
	m_pIcon->ShowComponent(bShow && !m_bEmpty);
	m_pName->ShowComponent(bShow && !m_bEmpty);
	m_pBase->ShowComponent(bShow);
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ====================================================================================================

// CTCabinetItem
// ====================================================================================================
CTCabinetItem* CTCabinetItem::Clone()
{
	CTCabinetItem* pCopy = new CTCabinetItem;

	pCopy->m_nIndex		= m_nIndex;
	pCopy->m_bItemCnt	= m_bItemCnt;
	pCopy->m_dwStItemID	= m_dwStItemID;

	pCopy->m_pClientItem = new CTClientItem;
	*(pCopy->m_pClientItem) = *m_pClientItem;
	
	return pCopy;
}
// ====================================================================================================
void CTCabinetItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	CTCabinetSlot* pCabinetSlot = static_cast<CTCabinetSlot*>(pSlot);

	if( !m_pClientItem )
	{
		pCabinetSlot->m_bEmpty = TRUE;

		pCabinetSlot->m_pIcon->ShowComponent(FALSE);
		pCabinetSlot->m_pName->ShowComponent(FALSE);
	}
	else
	{
		pCabinetSlot->m_bEmpty = FALSE;

		LPTITEM pTItem = m_pClientItem->GetTITEM();
		
		LPTITEMVISUAL pTVISUAL = m_pClientItem->GetVisual();
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

		pCabinetSlot->m_pName->m_strText = pTItem->m_strNAME;

		if( m_pClientItem->GetCount() > 1 )
			pCabinetSlot->m_pIcon->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pClientItem->GetCount());
		else
			pCabinetSlot->m_pIcon->m_strText.Empty();
	}
}
// ----------------------------------------------------------------------------------------------------
CTCabinetItem::~CTCabinetItem()
{
	if( m_pClientItem )
		delete m_pClientItem;
}
// ====================================================================================================

// CTNormalCabinetDlg
// ====================================================================================================
CTCabinetDlg::CTCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:	CTClientUIBase(pParent, pDesc),
	m_nCabUpdateSt(CABUPDATE_NOTYET),
	m_nMaxItemSlot(0),
	m_nMaxItemCol(0),
	m_nMaxCabCnt(0),
	m_nMaxStorage(0),
	m_pTitle(0),
	m_pSelectTxt(0),
	m_pCostTxt(0),
	m_ppOpenBtn(0),
	m_pCabinets(0),
	m_pList(0),
	m_nSelectCab(0),
	m_nLastHitBtn(0),
	m_bNeedSetComp(TRUE),
	m_bNeedUpdate(FALSE),
	m_pCharge(0),
	m_bShowCashCharge(FALSE),
	m_bRememberPrevScrollPos(FALSE)
{
}
// ----------------------------------------------------------------------------------------------------
CTCabinetDlg::~CTCabinetDlg()
{
	ClearAll();

	if( m_ppOpenBtn )
		delete [] m_ppOpenBtn;

	if( m_pCabinets )
		delete [] m_pCabinets;
	
	if( m_pList )
		delete m_pList;
}
// ====================================================================================================

// ====================================================================================================
void CTCabinetDlg::Initial()
{
	if( m_bNeedSetComp )
	{
		SetupComponent();
		m_bNeedSetComp = FALSE;
	}

	m_nCabUpdateSt = CABUPDATE_NOTYET;

	for(INT i=0; i<m_nMaxCabCnt; ++i)
		m_pCabinets[i].m_bNeedUpdate = TRUE;

	ClearAll();
	SelectCab(T_INVALID);
}
// ====================================================================================================
void CTCabinetDlg::SetCabinetInfo(UINT nCab, BOOL bOpen)
{
	m_pCabinets[nCab].m_bOpen = bOpen;
	m_pCabinets[nCab].m_bNeedUpdate = TRUE;

	NotifyUpdate();
}
// ====================================================================================================

// ====================================================================================================
void CTCabinetDlg::CompleteCabUpdate()
{
	m_nCabUpdateSt = CABUPDATE_COMPLETE;
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetDlg::CompleteCabinetUpdate(UINT nCab)
{
	m_pCabinets[nCab].m_bNeedUpdate = FALSE;
}
// ====================================================================================================
BOOL CTCabinetDlg::AddItem(UINT nCab, CTCabinetItem* pItem)
{
	if( m_pCabinets[nCab].m_vItems.size() >= m_nMaxStorage )
		return FALSE;

	m_pCabinets[nCab].m_vItems.push_back( pItem );
	NotifyUpdate();

	return TRUE;
}
VOID CTCabinetDlg::DeleteItem(UINT nCab, DWORD dwStItemID)
{
	CTCLItemArray::iterator it, end;
	it = m_pCabinets[nCab].m_vItems.begin();
	end = m_pCabinets[nCab].m_vItems.end();

	for(; it != end ; ++it )
	{
		CTCabinetItem* pCabItem = static_cast<CTCabinetItem*>( *it );

		if( pCabItem->m_dwStItemID == dwStItemID )
		{
			delete pCabItem;
			m_pCabinets[nCab].m_vItems.erase( it );
			break;
		}
	}

	NotifyUpdate();
}
// ====================================================================================================
CTCabinetItem* CTCabinetDlg::GetCabItemByStID(UINT nCab, DWORD dwStItemID)
{
	CTCLItemArray& vArr = m_pCabinets[nCab].m_vItems;
	for(INT i=0; i<INT(vArr.size()); ++i)
	{
		CTCabinetItem* pCabItem = static_cast<CTCabinetItem*>(vArr[i]);
		if( pCabItem->m_dwStItemID == dwStItemID )
			return pCabItem;
	}

	return NULL;
}
// ----------------------------------------------------------------------------------------------------
CTCabinetItem* CTCabinetDlg::GetSelectCabItem()
{
	return static_cast<CTCabinetItem*>(m_pList->GetSelectItem());
}
// ====================================================================================================
BOOL CTCabinetDlg::SelectCab(UINT nCab)
{
	if( nCab >= m_nMaxCabCnt )
	{
		m_nSelectCab = T_INVALID;

		if( m_pCostTxt )
			m_pCostTxt->m_strText.Empty();
		
		if( m_pSelectTxt )
			m_pSelectTxt->m_strText.Empty();
		
		return FALSE;
	}

	m_nSelectCab = nCab;

	INT64 nCOST = GetCabUseCost(nCab);
	if( nCOST )
	{
		CString strCOST = CTClientGame::MakeMoneyStr(nCOST);

		if( m_pCostTxt )
		{
			m_pCostTxt->ShowComponent(TRUE);
			m_pCostTxt->m_strText = CTChart::Format( TSTR_CABINET_COST, strCOST);
		}

		if( m_pSelectTxt )
		{
			m_pSelectTxt->ShowComponent(TRUE);
			m_pSelectTxt->m_strText = CTChart::Format( TSTR_CABINET_NAME, nCab+1);
		}
	}
	else
	{
		if( m_pCostTxt )
			m_pCostTxt->ShowComponent(FALSE);
		
		if( m_pSelectTxt )
			m_pSelectTxt->ShowComponent(FALSE);
	}

	if( m_ppOpenBtn )
	{
		for(UINT i=0; i<m_nMaxCabCnt; ++i)
			m_ppOpenBtn[i]->Select( m_nSelectCab == i );
	}

	return TRUE;
}
// ====================================================================================================
UINT CTCabinetDlg::GetSelectCab() const
{
	return m_nSelectCab;
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetDlg::SetMaxStorage(INT nMax)
{
	m_nMaxStorage = nMax;
	NotifyUpdate();
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetDlg::SetTitle(const CString& strTITLE)
{
	if( m_pTitle )
		m_pTitle->m_strText = strTITLE;
}
// ====================================================================================================
void CTCabinetDlg::Clear(UINT nCab)
{
	CTCLItemArray& vArr = m_pCabinets[nCab].m_vItems;
	for(INT i=0; i<INT(vArr.size()); ++i)
		delete vArr[i];

	vArr.clear();
	NotifyUpdate();
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetDlg::ClearAll()
{
	for(UINT i=0; i<m_nMaxCabCnt; ++i)
		Clear(i);
}
// ====================================================================================================
void CTCabinetDlg::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// ====================================================================================================
void CTCabinetDlg::Update(DWORD dwTick)
{
	if( m_bNeedSetComp )
		return;

	if( IsVisible() && m_bNeedUpdate )
	{
		INT i;
		
		INT nPrevScrollPos = m_pList->GetScrollPos();
		//INT nPrevSelectedItem = m_pList->GetSelectItemIndex();
		m_pList->ClearItem();

		if( m_nSelectCab < m_nMaxCabCnt )
		{
			CTCLItemArray& vArr = m_pCabinets[m_nSelectCab].m_vItems;
			sort(vArr.begin(), vArr.end(), _compare_cabitem);

			INT nCnt = INT(vArr.size());
			for(i=0; i<nCnt; ++i)
			{
				CTCabinetItem* pItem = static_cast<CTCabinetItem*>(vArr[i]);
				m_pList->AddItem( pItem->Clone() );
			}

			for(i=nCnt; i<m_nMaxStorage; ++i)
				m_pList->AddItem( new CTCabinetItem );
		}

		if( m_bRememberPrevScrollPos )
		{
			m_pList->SetScroll(
				m_pList->GetScrollRng(),
				nPrevScrollPos );

			//if( nPrevSelectedItem >= m_pList->GetItemCnt() )
			//	nPrevSelectedItem = m_pList->GetItemCnt()-1;
			//m_pList->Select( nPrevSelectedItem );
		}

		m_bNeedUpdate = FALSE;
	}
	m_pList->Update();
}
// ====================================================================================================
INT64 CTCabinetDlg::GetCabOpenCost(UINT nCab)
{
	return 0;
}
// ----------------------------------------------------------------------------------------------------
INT64 CTCabinetDlg::GetCabUseCost(UINT nCab)
{
	return 0;
} 
// ====================================================================================================

// ====================================================================================================
void CTCabinetDlg::SwitchFocus(TComponent* pCandidate)
{
	if( m_bNeedSetComp )
		return;

	if( m_pFocus == pCandidate )
		return;

	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if(pCandidate)
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}
// ====================================================================================================
BOOL CTCabinetDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !IsVisible() || m_bNeedSetComp )
		return FALSE;

	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	return m_pList->DoMouseWheel(nFlags,zDelta,pt);
}
// ====================================================================================================
void CTCabinetDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( !IsVisible() || m_bNeedSetComp )
		return;

	CTCtrlListItem* pItem = m_pList->GetHitItem(pt);
	if( pItem )
	{
		m_pList->Select(pItem->m_nIndex);
		return;
	}
	
	if( m_ppOpenBtn )
	{
		for(INT i=0; i<m_nMaxCabCnt; ++i)
		{
			if( m_ppOpenBtn[i]->HitTest(pt) )
			{
				m_nLastHitBtn = i;
				m_pCommandHandler->m_vCOMMAND.push_back( m_ppOpenBtn[i]->m_menu[TNM_LCLICK] );
				return;
			}
		}
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------------------------------------
void CTCabinetDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( IsVisible() && !m_pList->GetHitItem(pt) )
		CTClientUIBase::OnRButtonDown(nFlags,pt);
}
// ====================================================================================================
void CTCabinetDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if( m_pList )
		m_pList->ShowComponent(bVisible);

	if( m_pCharge)
		m_pCharge->ShowComponent(m_bShowCashCharge);
}
// ----------------------------------------------------------------------------------------------------
HRESULT CTCabinetDlg::Render(DWORD dwTickCount)
{
	if( m_bNeedSetComp )
		return S_OK;

	Update(dwTickCount);

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================================================
TDROPINFO CTCabinetDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;
	vResult.m_bDrop = TRUE;

	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTCabinetDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	if( m_bNeedSetComp )
		return FALSE;

	CTCabinetItem* pHitItem = static_cast<CTCabinetItem*>(m_pList->GetHitItem(point));
	if( !pHitItem )
		return FALSE;

	LPTITEM pTITEM = NULL;
	if( pHitItem->m_pClientItem )
		pTITEM = pHitItem->m_pClientItem->GetTITEM();

	if( !pTITEM )
		return FALSE;

	if( pDRAG )
	{
		CTCabinetSlot* pSlot0 = static_cast<CTCabinetSlot*>(m_pList->GetSlot(0));
		pDRAG->m_pIMAGE = new TImageList(NULL, pSlot0->m_pIcon->m_pDESC);

		LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
		WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

		pDRAG->m_pIMAGE->SetCurImage(wImg);
		pDRAG->m_bSlotID = (BYTE)GetSelectCab();
		pDRAG->m_dwParam = pHitItem->m_dwStItemID;

		CPoint pt = point;
		CSize size;

		pDRAG->m_pIMAGE->GetComponentSize(&size);
		pt.x -= size.cx / 2;
		pt.y -= size.cy / 2;

		pDRAG->m_pIMAGE->ShowComponent(FALSE);
		pDRAG->m_pIMAGE->MoveComponent(pt);
		pDRAG->m_pIMAGE->m_strText.Empty();
	}

	return TRUE;
}
// ====================================================================================================
ITDetailInfoPtr CTCabinetDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( m_bNeedSetComp )
		return pInfo;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( pHitItem )
	{
		CTCabinetItem* pCabItem = static_cast<CTCabinetItem*>(pHitItem);
		if( pCabItem->m_pClientItem )
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewItemInst(pCabItem->m_pClientItem, rc);
		}
	}

	return pInfo;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTCabinetDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	if( m_bNeedSetComp )
		return FALSE;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	CTCabinetItem* pCabItem = static_cast<CTCabinetItem*>(pHitItem);
	
	if( pCabItem->m_pClientItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pCabItem->m_pClientItem->GetTITEM();
		outInfo.m_pClientItem = pCabItem->m_pClientItem;
		
		return TRUE;
	}

	return FALSE;
}
// ====================================================================================================
void CTCabinetDlg::SelectLastestItem()
{
}
