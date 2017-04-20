#include "Stdafx.h"
#include "TShopBaseDlg.h"

// CTShopSlot
// ====================================================================================================
void CTShopSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	
	m_pIcon->ShowComponent(bShow);
	m_pName->ShowComponent(bShow);
	m_pBase->ShowComponent(bShow);

	m_pMark->ShowComponent(bShow && m_bMark);

	if( m_dwUserData == TSHOP_PHURCHASE_TYPE_POINT)
	{
		m_pRune->ShowComponent(FALSE);
		m_pLuna->ShowComponent(FALSE);
		m_pCron->ShowComponent(FALSE);

		if( m_pMoney )
			m_pMoney->ShowComponent(FALSE);
		if( m_pPoint )
			m_pPoint->ShowComponent(bShow);
	}
	else
	{
		m_pRune->ShowComponent(bShow);
		m_pLuna->ShowComponent(bShow);
		m_pCron->ShowComponent(bShow);
		if( m_pMoney )
			m_pMoney->ShowComponent(bShow);
		if( m_pPoint )
			m_pPoint->ShowComponent(FALSE);
	}
}
// ----------------------------------------------------------------------------------------------------
void CTShopSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ----------------------------------------------------------------------------------------------------
void CTShopSlot::Update(CTCtrlListItem* pItem)
{
	m_pItem = pItem;

	if( m_pItem )
	{
		m_pItem->ReflectSlot(this);
		m_bMark = pItem->m_bMark;
		ShowComponent(TRUE);
	}
	else
		ShowComponent(FALSE);
}
// ====================================================================================================

// ====================================================================================================
CTShopBaseDlg::CTShopBaseDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc)
{
	static const DWORD dwSLOTID[] = 
	{
		ID_CTRLINST_SLOT1,
		ID_CTRLINST_SLOT2,
		ID_CTRLINST_SLOT3,
		ID_CTRLINST_SLOT4,
		ID_CTRLINST_SLOT5,
		ID_CTRLINST_SLOT6,
		ID_CTRLINST_SLOT7,
		ID_CTRLINST_SLOT8,
		ID_CTRLINST_SLOT9,
		ID_CTRLINST_SLOT10,
		ID_CTRLINST_SLOT11,
		ID_CTRLINST_SLOT12
	};

	m_pTitle	= FindKid(ID_CTRLINST_TITLE);
	m_pDownTitle = FindKid(ID_CTRLINST_DOWN_TITLE);

	m_pOk		= static_cast<TButton*>(FindKid(ID_CTRLINST_OK));
	m_pCancel	= static_cast<TButton*>(FindKid(ID_CTRLINST_CANCEL));

	CTCLSlotArray vSlots;
	CTShopSlot* pSlot;
	
	for(INT i=0; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = new CTShopSlot();
		pSlot->SetSlot(i, FindKid(dwSLOTID[i]));
		vSlots.push_back(pSlot);
	}

	CTShopSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast<CTShopSlot*>(vSlots[0]);
	pSlot0->m_pBase->GetComponentPos(&ptBase);

	pSlot0->m_pMark = FindKid(ID_CTRLINST_MARK_SLOT);
	pSlot0->m_pMark->GetComponentPos(&ptTemp);
	CPoint ptMarkOffset = ptTemp - ptBase;

	pSlot0->m_pIcon = static_cast<TImageList*>( FindKid(ID_CTRLINST_ICON) );
	pSlot0->m_pIcon->GetComponentPos(&ptTemp);
	CPoint ptIconOffset = ptTemp - ptBase;

	pSlot0->m_pName = FindKid(ID_CTRLINST_NAME);
	pSlot0->m_pName->GetComponentPos(&ptTemp);
	CPoint ptNameOffset = ptTemp - ptBase;

	pSlot0->m_pRune = FindKid(ID_CTRLINST_RUNE);
	pSlot0->m_pRune->GetComponentPos(&ptTemp);
	CPoint ptRuneset = ptTemp - ptBase;

	pSlot0->m_pLuna = FindKid(ID_CTRLINST_LUNA);
	pSlot0->m_pLuna->GetComponentPos(&ptTemp);
	CPoint ptLunaset = ptTemp - ptBase;

	pSlot0->m_pCron = FindKid(ID_CTRLINST_CRON);
	pSlot0->m_pCron->GetComponentPos(&ptTemp);
	CPoint ptCronset = ptTemp - ptBase;

	CPoint ptMoneyset;
	pSlot0->m_pMoney = FindKid( ID_CTRLINST_MONEY);
	if( pSlot0->m_pMoney )
	{
		pSlot0->m_pMoney->GetComponentPos(&ptTemp);
		ptMoneyset = ptTemp - ptBase;
	}

	CPoint ptPointset;
	pSlot0->m_pPoint = FindKid( ID_CTRLINST_POINT);
	if( pSlot0->m_pPoint)
	{
		pSlot0->m_pPoint->GetComponentPos(&ptTemp);
		ptPointset = ptTemp - ptBase;
	}

	for(INT i=1; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = static_cast<CTShopSlot*>(vSlots[i]);

		pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);
		pSlot->m_pRune = new TComponent(this, pSlot0->m_pRune->m_pDESC);
		pSlot->m_pLuna = new TComponent(this, pSlot0->m_pLuna->m_pDESC);
		pSlot->m_pCron = new TComponent(this, pSlot0->m_pCron->m_pDESC);
		pSlot->m_pMark = new TComponent(this, pSlot0->m_pMark->m_pDESC);

		pSlot->m_pIcon->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();
		pSlot->m_pRune->m_id = GetUniqueID();
		pSlot->m_pLuna->m_id = GetUniqueID();
		pSlot->m_pCron->m_id = GetUniqueID();
		pSlot->m_pMark->m_id = GetUniqueID();
		
		AddKid( pSlot->m_pIcon );
		AddKid( pSlot->m_pName );
		AddKid( pSlot->m_pRune );
		AddKid( pSlot->m_pLuna );
		AddKid( pSlot->m_pCron );
		AddKid( pSlot->m_pMark );

		pSlot->m_pBase->GetComponentPos(&ptBase);

		if( pSlot0->m_pMoney )
		{
			pSlot->m_pMoney = new TComponent( this, pSlot0->m_pMoney->m_pDESC );
			pSlot->m_pMoney->m_id = GetUniqueID();
			AddKid( pSlot->m_pMoney );
			pSlot->m_pMoney->MoveComponent(ptBase+ptMoneyset);
		}
		if( pSlot0->m_pPoint )
		{
			pSlot->m_pPoint = new TComponent( this, pSlot0->m_pPoint->m_pDESC );
			pSlot->m_pPoint->m_id = GetUniqueID();
			AddKid( pSlot->m_pPoint );
			pSlot->m_pPoint->MoveComponent(ptBase+ptPointset);
		}

		pSlot->m_pIcon->MoveComponent(ptBase+ptIconOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
		pSlot->m_pRune->MoveComponent(ptBase+ptRuneset);
		pSlot->m_pLuna->MoveComponent(ptBase+ptLunaset);
		pSlot->m_pCron->MoveComponent(ptBase+ptCronset);
		pSlot->m_pMark->MoveComponent(ptBase+ptMarkOffset);
	}

	m_pList = new CTCtrlList();
	m_pList->SetSlot(&vSlots);
	m_pList->SetupScroll(static_cast<TScroll*>(FindKid(ID_CTRLINST_SCROLL)), MAX_ITEM_COLUMN);
}
// ----------------------------------------------------------------------------------------------------
CTShopBaseDlg::~CTShopBaseDlg()
{
	delete m_pList;
}
// ====================================================================================================

// ====================================================================================================
void CTShopBaseDlg::SetTitle(const CString& strTitle, CString strDownTitle)
{
	if( m_pTitle )
		m_pTitle->m_strText = strTitle;

	if( m_pDownTitle)
		m_pDownTitle->m_strText = strDownTitle;
}
// ----------------------------------------------------------------------------------------------------
CString CTShopBaseDlg::GetTitle() const
{
	if( m_pTitle )
		return m_pTitle->m_strText;

	return CString("");
}
// ====================================================================================================
void CTShopBaseDlg::SetDownTitle(const CString& strTitle)
{
	if( m_pDownTitle)
		m_pDownTitle->m_strText = strTitle;
}
// ====================================================================================================
CString CTShopBaseDlg::GetDownTitle() const
{
	if( m_pDownTitle )
		return m_pDownTitle->m_strText;

	return CString("");
}
// ====================================================================================================
void CTShopBaseDlg::SetOkButtonTitle(const CString& strTitle)
{
	if( m_pOk )
		m_pOk->m_strText = strTitle;
}
// ====================================================================================================
DWORD CTShopBaseDlg::GetOkGM() const
{
	if( m_pOk )
		return m_pOk->m_menu[TNM_LCLICK];

	return 0;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTShopBaseDlg::GetCancelGM() const
{
	if( m_pCancel )
		return m_pCancel->m_menu[TNM_LCLICK];

	return 0;
}
// ====================================================================================================
void CTShopBaseDlg::SetOkGM(DWORD dwGM)
{
	if( m_pOk )
		m_pOk->m_menu[TNM_LCLICK] = dwGM;
}
// ----------------------------------------------------------------------------------------------------
void CTShopBaseDlg::SetCancelGM(DWORD dwGM)
{
	if( m_pCancel )
		m_pCancel->m_menu[TNM_LCLICK] = dwGM;
}
// ====================================================================================================

// ====================================================================================================
void CTShopBaseDlg::SwitchFocus(TComponent* pCandidate)
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
BOOL CTShopBaseDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	return m_pList->DoMouseWheel(nFlags,zDelta,pt);
}
// ====================================================================================================
void CTShopBaseDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTCtrlListItem* pItem = m_pList->GetHitItem(pt);
	if( pItem )
		m_pList->Select(pItem->m_nIndex);
	else
		CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------------------------------------
void CTShopBaseDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_pList->GetHitItem(pt) )
		CTClientUIBase::OnRButtonDown(nFlags,pt);
}
// ====================================================================================================
void CTShopBaseDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	m_pList->ShowComponent(bVisible);
}
// ----------------------------------------------------------------------------------------------------
HRESULT CTShopBaseDlg::Render(DWORD dwTickCount)
{
	m_pList->Update();

	return CTClientUIBase::Render(dwTickCount);
}