#include "StdAfx.h"
#include "Resource.h"
#include "TTradeDlg.h"

// CTTradeSlot
// ====================================================================================================
void CTTradeSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	
	m_pIcon->ShowComponent(bShow);
	m_pName->ShowComponent(bShow);

	m_pBase->ShowComponent(TRUE);
}
// ----------------------------------------------------------------------------------------------------
void CTTradeSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ====================================================================================================

// ====================================================================================================
CTTradeDlg::CTTradeDlg( TComponent *pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost)
	: CTClientUIBase(pParent, pDesc), m_bLock(FALSE), m_pMainChar(pHost)
{
	//////////////////////////////////////////////////////////////////////////
	m_bTargetMask = FALSE;
	m_bMyMask = FALSE;
	//////////////////////////////////////////////////////////////////////////

	static const DWORD dwTSLOTID[] = 
	{
		ID_CTRLINST_T_SLOT1,
		ID_CTRLINST_T_SLOT2,
		ID_CTRLINST_T_SLOT3,
		ID_CTRLINST_T_SLOT4,
		ID_CTRLINST_T_SLOT5,
		ID_CTRLINST_T_SLOT6
	};

	static const DWORD dwMSLOTID[] = 
	{
		ID_CTRLINST_M_SLOT1,
		ID_CTRLINST_M_SLOT2,
		ID_CTRLINST_M_SLOT3,
		ID_CTRLINST_M_SLOT4,
		ID_CTRLINST_M_SLOT5,
		ID_CTRLINST_M_SLOT6
	};

	m_pTitle		= FindKid(ID_CTRLINST_TITLE);

	m_pLockBtn		= static_cast<TButton*>(FindKid(ID_CTRLINST_OK));
	m_pTradeBtn		= static_cast<TButton*>(FindKid(ID_CTRLINST_CANCEL));
	
	m_pTargetName	= FindKid(ID_CTRLINST_T_CHAR);
	m_pMyName		= FindKid(ID_CTRLINST_M_CHAR);

	m_pTargetRune	= FindKid(ID_CTRLINST_T_RUNE);
	m_pTargetLuna	= FindKid(ID_CTRLINST_T_LUNA);
	m_pTargetCron	= FindKid(ID_CTRLINST_T_CRON);

	TComponent* pMyRuneTemp = FindKid(ID_CTRLINST_M_RUNE);
	m_pMyRune = new CTNumberEdit(this, pMyRuneTemp->m_pDESC, TRUNE_LENGTH);
	
	TComponent* pMyLunaTemp = FindKid(ID_CTRLINST_M_LUNA);
	m_pMyLuna = new CTNumberEdit(this, pMyLunaTemp->m_pDESC, TLUNA_LENGTH);

	TComponent* pMyCronTemp = FindKid(ID_CTRLINST_M_CRON);
	m_pMyCron = new CTNumberEdit(this, pMyCronTemp->m_pDESC, TCRON_LENGTH);
	
	AddKid(m_pMyRune);
	AddKid(m_pMyLuna);
	AddKid(m_pMyCron);

	RemoveKid(pMyRuneTemp);
	RemoveKid(pMyLunaTemp);
	RemoveKid(pMyCronTemp);
	
	delete pMyRuneTemp;
	delete pMyLunaTemp;
	delete pMyCronTemp;

	CTCLSlotArray vTSlots, vMSlots;
	CTTradeSlot* pSlot;
	
	for(INT i=0; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = new CTTradeSlot();
		pSlot->SetSlot(i, FindKid(dwTSLOTID[i]));
		vTSlots.push_back(pSlot);

		pSlot = new CTTradeSlot();
		pSlot->SetSlot(i, FindKid(dwMSLOTID[i]));
		vMSlots.push_back(pSlot);
	}

	CTTradeSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast<CTTradeSlot*>(vTSlots[0]);
	pSlot0->m_pBase->GetComponentPos(&ptBase);

	pSlot0->m_pIcon = static_cast<TImageList*>( FindKid(ID_CTRLINST_ICON) );
	pSlot0->m_pIcon->GetComponentPos(&ptTemp);
	pSlot0->m_pIcon->m_strText = "";
	CPoint ptIconOffset = ptTemp - ptBase;

	pSlot0->m_pName = FindKid(ID_CTRLINST_NAME);
	pSlot0->m_pName->GetComponentPos(&ptTemp);
	pSlot0->m_pName->m_strText = "";
	CPoint ptNameOffset = ptTemp - ptBase;

	for(INT i=1; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = static_cast<CTTradeSlot*>(vTSlots[i]);

		pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);

		pSlot->m_pIcon->m_strText = "";
		pSlot->m_pName->m_strText = "";
		
		pSlot->m_pIcon->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();

		AddKid( pSlot->m_pIcon );
		AddKid( pSlot->m_pName );
		
		pSlot->m_pBase->GetComponentPos(&ptBase);

		pSlot->m_pIcon->MoveComponent(ptBase+ptIconOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
	}

	for(INT i=0; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = static_cast<CTTradeSlot*>(vMSlots[i]);

		pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);

		pSlot->m_pIcon->m_strText = "";
		pSlot->m_pName->m_strText = "";
		
		pSlot->m_pIcon->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();

		AddKid( pSlot->m_pIcon );
		AddKid( pSlot->m_pName );
		
		pSlot->m_pBase->GetComponentPos(&ptBase);

		pSlot->m_pIcon->MoveComponent(ptBase+ptIconOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
	}

	m_pTargetList = new CTCtrlList();
	m_pTargetList->SetSlot(&vTSlots);
	m_pTargetList->SetupScroll(static_cast<TScroll*>(FindKid(ID_CTRLINST_T_SCROLL)), MAX_ITEM_COLUMN);

	m_pMyList = new CTCtrlList();
	m_pMyList->SetSlot(&vMSlots);
	m_pMyList->SetupScroll(static_cast<TScroll*>(FindKid(ID_CTRLINST_M_SCROLL)), MAX_ITEM_COLUMN);

	m_pMyRune->EnableComponent(TRUE);
	m_pMyLuna->EnableComponent(TRUE);
	m_pMyCron->EnableComponent(TRUE);
	m_pLockBtn->EnableComponent(TRUE);
	m_pTradeBtn->EnableComponent(FALSE);
}
// ----------------------------------------------------------------------------------------------------
CTTradeDlg::~CTTradeDlg()
{
	if( m_pTargetList )
		delete m_pTargetList;
	if( m_pMyList )
		delete m_pMyList;
}
// ====================================================================================================

// ====================================================================================================
void CTTradeDlg::SetTitle(const CString& strTitle)
{
	if( m_pTitle )
		m_pTitle->m_strText = strTitle;
}
// ----------------------------------------------------------------------------------------------------
CString CTTradeDlg::GetTitle() const
{
	if( m_pTitle )
		return m_pTitle->m_strText;

	return CString("");
}
// ====================================================================================================
void CTTradeDlg::SetTargetName(const CString strName)
{
	m_pMyName->m_strText = m_pMainChar->m_strNAME;
	m_pTargetName->m_strText = strName;
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::SetTargetMoney(DWORD dwRune, DWORD dwLuna, DWORD dwCron)
{
	m_pTargetRune->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwRune);
	m_pTargetLuna->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwLuna);
	m_pTargetCron->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwCron);
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::SetTargetMask(BOOL bMask)
{
	m_bTargetMask = bMask;

	int count = m_pTargetList->GetSlotCnt();

	for( int i=0; i<count; ++i )
	{
		CTCtrlListSlot* pSlot = m_pTargetList->GetSlotNotUpdate( i );
		TButton* pBtn = static_cast<TButton*>( pSlot->m_pBase );
		if( pBtn )
		{
			if( bMask )
			{
//				pBtn->m_dwColor = m_dwMaskColor;
//				pBtn->SetStyle( TS_CUSTOM_COLOR );

				pBtn->Select( TBUTTON_STATE_DOWN );
				pBtn->EnableComponent( FALSE );
			}
			else
			{
				pBtn->m_dwColor = 0;
				pBtn->SetStyle( TS_NONE );

				pBtn->Select( TBUTTON_STATE_NULL );
				pBtn->EnableComponent( TRUE );
			}
		}
	}
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::SetMyMask(BOOL bMask)
{
	m_bMyMask = bMask;

	int count = m_pMyList->GetSlotCnt();

	for( int i=0; i<count; ++i )
	{
		CTCtrlListSlot* pSlot = m_pMyList->GetSlotNotUpdate( i );
		TButton* pBtn = static_cast<TButton*>( pSlot->m_pBase );
		if( pBtn )
		{
			if( bMask )
			{
//				pBtn->m_dwColor = m_dwMaskColor;
//				pBtn->SetStyle( TS_CUSTOM_COLOR );

				pBtn->Select( TBUTTON_STATE_DOWN );
				pBtn->EnableComponent( FALSE );
			}
			else
			{
				pBtn->m_dwColor = 0;
				pBtn->SetStyle( TS_NONE );

				pBtn->Select( TBUTTON_STATE_NULL );
				pBtn->EnableComponent( TRUE );
			}
		}
	}
}
// ====================================================================================================
DWORD CTTradeDlg::GetMyRune() const
{
	return ::atoi( (LPCSTR) m_pMyRune->m_strText );
}
// ----------------------------------------------------------------------------------------------------
DWORD CTTradeDlg::GetMyLuna() const
{
	return ::atoi( (LPCSTR) m_pMyLuna->m_strText );
}
// ----------------------------------------------------------------------------------------------------
DWORD CTTradeDlg::GetMyCron() const
{
	return ::atoi( (LPCSTR) m_pMyCron->m_strText );
}
// ====================================================================================================
DWORD CTTradeDlg::GetTradeBtnGM() const
{
	if( m_pTradeBtn )
		return m_pTradeBtn->m_menu[TNM_LCLICK];

	return 0;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTTradeDlg::GetLockBtnGM() const
{
	if( m_pLockBtn )
		return m_pLockBtn->m_menu[TNM_LCLICK];

	return 0;
}
// ====================================================================================================
void CTTradeDlg::SetTradeBtnGM(DWORD dwGM)
{
	if( m_pTradeBtn )
		m_pTradeBtn->m_menu[TNM_LCLICK] = dwGM;
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::SetLockBtnGM(DWORD dwGM)
{
	if( m_pLockBtn )
		m_pLockBtn->m_menu[TNM_LCLICK] = dwGM;
}
// ====================================================================================================
void CTTradeDlg::SetLock(BOOL bLock)
{
	if( m_bLock == bLock )
		return;

	m_bLock = bLock;

	if( m_bLock )
	{
		m_pMyRune->EnableComponent(FALSE);
		m_pMyLuna->EnableComponent(FALSE);
		m_pMyCron->EnableComponent(FALSE);

		m_pLockBtn->EnableComponent(FALSE);
		m_pTradeBtn->EnableComponent(TRUE);

		SetMyMask(TRUE);
	}
	else
	{
		m_pMyRune->EnableComponent(TRUE);
		m_pMyLuna->EnableComponent(TRUE);
		m_pMyCron->EnableComponent(TRUE);

		m_pLockBtn->EnableComponent(TRUE);
		m_pTradeBtn->EnableComponent(FALSE);

		SetMyMask(FALSE);
	}
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::SetTradeEnable(BOOL bEnable)
{
	m_pTradeBtn->EnableComponent(bEnable);
}
// ====================================================================================================
INT CTTradeDlg::AddItem(BOOL bTarget, CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot)
{
	CTClientItem* pNewItem = new CTClientItem;
	
	*pNewItem = *pItem;
	
	CTTradeItem* pInfo = new CTTradeItem;
	pInfo->m_pClientItem = pNewItem;
	pInfo->m_bInven = bInven;
	pInfo->m_bInvenSlot = bInvenSlot;

	if( bTarget )
		return m_pTargetList->AddItem(pInfo);
	else
		return m_pMyList->AddItem(pInfo);
}
// ----------------------------------------------------------------------------------------------------
CTTradeItem* CTTradeDlg::AddEmptyItem(BOOL bTarget)
{
	CTClientItem* pNewItem = new CTClientItem;
	pNewItem->SetItemSlot((BYTE)0);
	pNewItem->SetGrade(0);
	pNewItem->SetTITEM(0);
	pNewItem->SetItemID(0);
	pNewItem->SetCount(0);

	CTTradeItem* pInfo = new CTTradeItem;
	pInfo->m_pClientItem = pNewItem;
	pInfo->m_bInven = 0;
	pInfo->m_bInvenSlot = 0;

	if( bTarget )
		m_pTargetList->AddItem(pInfo);
	else
		m_pMyList->AddItem(pInfo);

	return pInfo;
}
// ====================================================================================================
void CTTradeDlg::RemoveItem(BOOL bTarget, INT nIdx)
{
	if( bTarget )
		m_pTargetList->RemoveItem(nIdx);
	else
		m_pMyList->RemoveItem(nIdx);
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::Clear(BOOL bTarget)
{
	if( bTarget )
	{
		m_pTargetRune->m_strText.Empty();
		m_pTargetLuna->m_strText.Empty();
		m_pTargetCron->m_strText.Empty();

		m_pTargetList->ClearItem();
	}
	else
	{
		m_pMyRune->ClearText();
		m_pMyLuna->ClearText();
		m_pMyCron->ClearText();
	
		m_pMyList->ClearItem();
	}
}
// ====================================================================================================
UINT CTTradeDlg::GetItemCount(BOOL bTarget) const
{
	if( bTarget )
		return m_pTargetList->GetItemCnt();
	else
		return m_pMyList->GetItemCnt();
}
// ----------------------------------------------------------------------------------------------------
CTTradeItem* CTTradeDlg::GetItem(BOOL bTarget, INT nIndex)
{
	if( bTarget )
		return static_cast<CTTradeItem*>( m_pTargetList->GetItem(nIndex) );
	else
		return static_cast<CTTradeItem*>( m_pMyList->GetItem(nIndex) );
}
// ====================================================================================================
INT CTTradeDlg::GetItemIdxByInvenPos(BYTE bInven, BYTE bInvenSlot)
{
	INT nCnt = m_pMyList->GetItemCnt();
	for(INT i=0; i<nCnt; ++i)
	{
		CTTradeItem* pInfo = static_cast<CTTradeItem*>(m_pMyList->GetItem(i));
		if( pInfo->m_bInven == bInven && pInfo->m_bInvenSlot == bInvenSlot )
			return i;
	}

	return T_INVALID;
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::ChangeItemInvenPos(BYTE bFromInven, BYTE bFromInvenSlot, BYTE bToInven, BYTE bToInvenSlot)
{
	INT nIndex = GetItemIdxByInvenPos(bFromInven,bFromInvenSlot);
	if( nIndex != T_INVALID )
	{
		CTTradeItem* pInfo = GetItem(FALSE, nIndex);
		
		pInfo->m_bInven = bToInven;
		pInfo->m_bInvenSlot = bToInvenSlot;
	}
}
// ====================================================================================================
TEdit* CTTradeDlg::GetCurEdit()
{
	if( IsVisible() && GetFocus() && !m_bLock )
	{
		if( m_pMyRune->GetFocus() )
			return m_pMyRune;

		if( m_pMyLuna->GetFocus() )
			return m_pMyLuna;

		if( m_pMyCron->GetFocus() )
			return m_pMyCron;
	}

	return NULL;
}
// ====================================================================================================

// ====================================================================================================
ITDetailInfoPtr CTTradeDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	CTCtrlListItem* pHitItem = m_pTargetList->GetHitItem(point);
	if( !pHitItem )
	{
		pHitItem = m_pMyList->GetHitItem(point);
		if( !pHitItem )
			return pInfo;
	}

	CTTradeItem* pTradeItem = static_cast<CTTradeItem*>(pHitItem);
	if( pTradeItem->m_pClientItem )
	{
		CRect rc;
		GetComponentRect(&rc);

		pInfo = CTDetailInfoManager::NewItemInst(pTradeItem->m_pClientItem, rc);
		pInfo->SetDir(TRUE, FALSE, TRUE);
	}

	return pInfo;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTTradeDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo )
{
	CTCtrlListItem* pHitItem = m_pTargetList->GetHitItem(point);
	if( !pHitItem )
	{
		pHitItem = m_pMyList->GetHitItem(point);
		if( !pHitItem )
			return FALSE;
	}

	CTTradeItem* pTradeItem = static_cast<CTTradeItem*>(pHitItem);

	if( pTradeItem->m_pClientItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pTradeItem->m_pClientItem->GetTITEM();
		outInfo.m_pClientItem = pTradeItem->m_pClientItem;
		
		return TRUE;
	}

	return FALSE;
}
// ====================================================================================================
TDROPINFO CTTradeDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;

	if( m_bDropTarget && !m_bLock && m_pMyList->GetItemCnt() < MAX_DRAGSLOT )
		vResult.m_bDrop = TRUE;

	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTTradeDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	if( m_bLock )
		return FALSE;

	CTCtrlListItem* pHitItem = m_pMyList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	LPTITEM pTITEM = NULL;
	if( static_cast<CTTradeItem*>(pHitItem)->m_pClientItem )
		pTITEM = static_cast<CTTradeItem*>(pHitItem)->m_pClientItem->GetTITEM();

	if( !pTITEM )
		return FALSE;

	if( pDRAG )
	{
		CTTradeSlot* pSlot0 = static_cast<CTTradeSlot*>(m_pTargetList->GetSlot(0));
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

// ====================================================================================================
void CTTradeDlg::SwitchFocus(TComponent* pCandidate)
{
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
void CTTradeDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTCtrlListItem* pItem = m_pTargetList->GetHitItem(pt);
	if( pItem && !m_bTargetMask )
	{
		m_pTargetList->Select(pItem->m_nIndex);
		return;
	}
	
	pItem = m_pMyList->GetHitItem(pt);
	if( pItem && !m_bMyMask )
	{
		m_pMyList->Select(pItem->m_nIndex);
		return;
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------------------------------------
void CTTradeDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_pTargetList->GetHitItem(pt) || !m_pMyList->GetHitItem(pt) )
		CTClientUIBase::OnRButtonDown(nFlags,pt);
}
// ====================================================================================================
void CTTradeDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	m_pTargetList->ShowComponent(bVisible);
	m_pMyList->ShowComponent(bVisible);
}
// ----------------------------------------------------------------------------------------------------
HRESULT CTTradeDlg::Render(DWORD dwTickCount)
{
	m_pTargetList->Update();
	m_pMyList->Update();

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================================================

