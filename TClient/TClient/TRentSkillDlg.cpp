#include "Stdafx.h"
#include "TRentSkillDlg.h"

// CTShopSlot
// ====================================================================================================
void CTRentSkillSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	
	m_pSkill->ShowComponent(bShow);
	m_pName->ShowComponent(bShow);
	m_pRune->ShowComponent(bShow);
	m_pLuna->ShowComponent(bShow);
	m_pCron->ShowComponent(bShow);
	m_pMoney->ShowComponent(bShow);
	m_pBase->ShowComponent(bShow);
}
// ----------------------------------------------------------------------------------------------------
void CTRentSkillSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ====================================================================================================

// ====================================================================================================
CTRentSkillDlg::CTRentSkillDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
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
	m_pTitle->m_strText = CTChart::LoadString( TSTR_SHOPTITLE_RENT );

	m_pDownTitle = FindKid(ID_CTRLINST_DOWN_TITLE);
	m_pDownTitle->m_strText = CTChart::LoadString( TSTR_SHOPHELP_RENT );

	m_pOK		= static_cast<TButton*>(FindKid(ID_CTRLINST_OK));
	m_pOK->ShowComponent( FALSE );
	m_pOK->EnableComponent( FALSE );
	m_pCancel	= static_cast<TButton*>(FindKid(ID_CTRLINST_CANCEL));

	CTCLSlotArray vSlots;
	CTRentSkillSlot* pSlot;
	
	for(INT i=0; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = new CTRentSkillSlot();
		pSlot->SetSlot(i, FindKid(dwSLOTID[i]));
		vSlots.push_back(pSlot);
	}

	CTRentSkillSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast<CTRentSkillSlot*>(vSlots[0]);
	pSlot0->m_pBase->GetComponentPos(&ptBase);

	pSlot0->m_pSkill = static_cast<TImageList*>( FindKid(ID_CTRLINST_SKILL) );
	pSlot0->m_pSkill->GetComponentPos(&ptTemp);
	pSlot0->m_pSkill->m_strText.Empty();
	pSlot0->m_pSkill->m_strPrvText.Empty();
	CPoint ptSkillOffset = ptTemp - ptBase;

	pSlot0->m_pName = FindKid(ID_CTRLINST_NAME);
	pSlot0->m_pName->GetComponentPos(&ptTemp);
	pSlot0->m_pName->m_strText.Empty();
	CPoint ptNameOffset = ptTemp - ptBase;

	pSlot0->m_pRune = FindKid(ID_CTRLINST_RUNE);
	pSlot0->m_pRune->GetComponentPos(&ptTemp);
	pSlot0->m_pRune->m_strText.Empty();
	CPoint ptRuneset = ptTemp - ptBase;

	pSlot0->m_pLuna = FindKid(ID_CTRLINST_LUNA);
	pSlot0->m_pLuna->GetComponentPos(&ptTemp);
	pSlot0->m_pLuna->m_strText.Empty();
	CPoint ptLunaset = ptTemp - ptBase;

	pSlot0->m_pCron = FindKid(ID_CTRLINST_CRON);
	pSlot0->m_pCron->GetComponentPos(&ptTemp);
	pSlot0->m_pCron->m_strText.Empty();
	CPoint ptCronset = ptTemp - ptBase;

	pSlot0->m_pMoney = FindKid(ID_CTRLINST_MONEY);
	pSlot0->m_pMoney->GetComponentPos(&ptTemp);
	pSlot0->m_pMoney->m_strText.Empty();
	CPoint ptMoneyset = ptTemp - ptBase;

	for(INT i=1; i<MAX_ITEM_SLOT; ++i)
	{
		pSlot = static_cast<CTRentSkillSlot*>(vSlots[i]);

		pSlot->m_pSkill = new TImageList(this, pSlot0->m_pSkill->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);
		pSlot->m_pRune = new TComponent(this, pSlot0->m_pRune->m_pDESC);
		pSlot->m_pLuna = new TComponent(this, pSlot0->m_pLuna->m_pDESC);
		pSlot->m_pCron = new TComponent(this, pSlot0->m_pCron->m_pDESC);
		pSlot->m_pMoney = new TComponent(this, pSlot0->m_pMoney->m_pDESC);
		
		pSlot->m_pSkill->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();
		pSlot->m_pRune->m_id = GetUniqueID();
		pSlot->m_pLuna->m_id = GetUniqueID();
		pSlot->m_pCron->m_id = GetUniqueID();
		pSlot->m_pMoney->m_id = GetUniqueID();

		AddKid( pSlot->m_pSkill );
		AddKid( pSlot->m_pName );
		AddKid( pSlot->m_pRune );
		AddKid( pSlot->m_pLuna );
		AddKid( pSlot->m_pCron );
		AddKid( pSlot->m_pMoney );
		
		pSlot->m_pBase->GetComponentPos(&ptBase);

		pSlot->m_pSkill->MoveComponent(ptBase+ptSkillOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
		pSlot->m_pRune->MoveComponent(ptBase+ptRuneset);
		pSlot->m_pLuna->MoveComponent(ptBase+ptLunaset);
		pSlot->m_pCron->MoveComponent(ptBase+ptCronset);
		pSlot->m_pMoney->MoveComponent(ptBase+ptMoneyset);
	}

	m_pList = new CTCtrlList();
	m_pList->SetSlot(&vSlots);
	m_pList->SetupScroll(static_cast<TScroll*>(FindKid(ID_CTRLINST_SCROLL)), MAX_ITEM_COLUMN);


	///////////////////////////////////////////////////////
	// 사용불가 마크없앰.
	TComponent* pCOMP = FindKid( ID_CTRLINST_MARK_SLOT );
	if( pCOMP )
	{
		RemoveKid( pCOMP );
		delete pCOMP;
	}

	pCOMP = FindKid( ID_CTRLINST_ICON );
	if( pCOMP )
	{
		RemoveKid( pCOMP );
		delete pCOMP;
	}
}
// ----------------------------------------------------------------------------------------------------
CTRentSkillDlg::~CTRentSkillDlg()
{
	delete m_pList;
}
// ====================================================================================================

// ====================================================================================================
void CTRentSkillDlg::SetTitle(const CString& strTitle)
{
	if( m_pTitle )
		m_pTitle->m_strText = strTitle;
}
// ----------------------------------------------------------------------------------------------------
CString CTRentSkillDlg::GetTitle() const
{
	if( m_pTitle )
		return m_pTitle->m_strText;

	return CString("");
}
// ----------------------------------------------------------------------------------------------------
DWORD CTRentSkillDlg::GetCancelGM() const
{
	if( m_pCancel )
		return m_pCancel->m_menu[TNM_LCLICK];

	return 0;
}
// ----------------------------------------------------------------------------------------------------
void CTRentSkillDlg::SetCancelGM(DWORD dwGM)
{
	if( m_pCancel )
		m_pCancel->m_menu[TNM_LCLICK] = dwGM;
}
// ====================================================================================================

// ====================================================================================================
void CTRentSkillDlg::SwitchFocus(TComponent* pCandidate)
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
BOOL CTRentSkillDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	return m_pList->DoMouseWheel(nFlags,zDelta,pt);
}
// ====================================================================================================
void CTRentSkillDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTCtrlListItem* pItem = m_pList->GetHitItem(pt);
	if( pItem )
		m_pList->Select(pItem->m_nIndex);
	else
		CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ----------------------------------------------------------------------------------------------------
void CTRentSkillDlg::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_pList->GetHitItem(pt) )
		CTClientUIBase::OnRButtonDown(nFlags,pt);
}
// ====================================================================================================
void CTRentSkillDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	m_pList->ShowComponent(bVisible);
	m_pOK->ShowComponent(FALSE);
}
// ----------------------------------------------------------------------------------------------------
HRESULT CTRentSkillDlg::Render(DWORD dwTickCount)
{
	m_pList->Update();

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================================================
ITDetailInfoPtr CTRentSkillDlg::GetTInfoKey(const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( pHitItem )
	{
		CTRentNpcItem* pSkillItem = static_cast<CTRentNpcItem*>(pHitItem);
		if( pSkillItem->m_pSkill )
		{
			CRect rc;
			GetComponentRect(&rc);

			pInfo = CTDetailInfoManager::NewSkillInst( pSkillItem->m_pSkill, 1, TRUE, rc );
			pInfo->SetDir(TRUE, FALSE, TRUE);
		}
	}

	return pInfo;
}
// ====================================================================================================
TDROPINFO CTRentSkillDlg::OnDrop(CPoint point)
{
	TDROPINFO vResult;

	if( m_bDropTarget )
		vResult.m_bDrop = TRUE;

	return vResult;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTRentSkillDlg::OnBeginDrag(LPTDRAG pDRAG, CPoint point)
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
		return FALSE;

	LPTSKILL pSKILL = static_cast<CTRentNpcItem*>(pHitItem)->m_pSkill;
	if( !pSKILL )
		return FALSE;

	if( pDRAG )
	{
		CTRentSkillSlot* pSlot0 = static_cast<CTRentSkillSlot*>(m_pList->GetSlot(0));
		pDRAG->m_pIMAGE = new TImageList(NULL, pSlot0->m_pSkill->m_pDESC);

		pDRAG->m_pIMAGE->SetCurImage(pSKILL->m_wIconID);
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








