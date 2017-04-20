#include "StdAfx.h"
#include "TNormalCabinetDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// CTNormalCabinetDlg
// ====================================================================================================
CTNormalCabinetDlg::CTNormalCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTCabinetDlg(pParent, pDesc)
{
	m_pOpenByCash = NULL;
}
// ----------------------------------------------------------------------------------------------------
CTNormalCabinetDlg::~CTNormalCabinetDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}
// ====================================================================================================
void CTNormalCabinetDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}
// ====================================================================================================
void CTNormalCabinetDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}
// ====================================================================================================
void CTNormalCabinetDlg::SetupComponent()
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

	static const DWORD dwOPENBTN[] = 
	{
		ID_CTRLINST_CAB1,
		ID_CTRLINST_CAB2,
		ID_CTRLINST_CAB3
	};

	INT i;

	m_nMaxItemSlot	= MAX_ITEM_SLOT;
	m_nMaxItemCol	= MAX_ITEM_COLUMN;
	m_nMaxCabCnt	= TMAX_CABINET_CNT;
	m_nMaxStorage	= CABINET_STORAGE_MAX;

	m_pTitle	= FindKid(ID_CTRLINST_TITLE);
	m_pSelectTxt= FindKid(ID_CTRLINST_TXT_SELCAB);
	m_pCostTxt	= FindKid(ID_CTRLINST_TXT_COST);

	m_pSelectTxt->m_bWordBreak = FALSE;

	m_ppOpenBtn = new TButton*[m_nMaxCabCnt];
	m_pCabinets = new Cabinet[m_nMaxCabCnt];

	for(i=0; i<m_nMaxCabCnt; ++i)
		m_ppOpenBtn[i] = static_cast<TButton*>(FindKid(dwOPENBTN[i]));

	CTCLSlotArray vSlots;
	CTCabinetSlot* pSlot;
	
	for(i=0; i<m_nMaxItemSlot; ++i)
	{
		pSlot = new CTCabinetSlot();
		pSlot->SetSlot(i, FindKid(dwSLOTID[i]));
		vSlots.push_back(pSlot);
	}

	CTCabinetSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast<CTCabinetSlot*>(vSlots[0]);
	pSlot0->m_pBase->GetComponentPos(&ptBase);

	pSlot0->m_pIcon = static_cast<TImageList*>( FindKid(ID_CTRLINST_ICON) );
	pSlot0->m_pIcon->GetComponentPos(&ptTemp);
	CPoint ptIconOffset = ptTemp - ptBase;

	pSlot0->m_pName = FindKid(ID_CTRLINST_NAME);
	pSlot0->m_pName->GetComponentPos(&ptTemp);
	CPoint ptNameOffset = ptTemp - ptBase;

	for(i=1; i<m_nMaxItemSlot; ++i)
	{
		pSlot = static_cast<CTCabinetSlot*>(vSlots[i]);

		pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);
		
		pSlot->m_pIcon->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();

		AddKid( pSlot->m_pIcon );
		AddKid( pSlot->m_pName );
		
		pSlot->m_pBase->GetComponentPos(&ptBase);

		pSlot->m_pIcon->MoveComponent(ptBase+ptIconOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
	}

	m_pList = new CTCtrlList();
	m_pList->SetSlot(&vSlots);
	m_pList->SetupScroll(static_cast<TScroll*>(FindKid(ID_CTRLINST_SCROLL)), m_nMaxItemCol);

	m_pCharge = FindKid( ID_CTRLINST_CHARGE);
}
// ====================================================================================================
void CTNormalCabinetDlg::RequestShowComponent()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	
	switch( m_nCabUpdateSt )
	{
	case CABUPDATE_NOTYET:
		{
			CTClientSession* pSession = pGame->GetSession();
			pSession->SendCS_CABINETLIST_REQ();
			m_nCabUpdateSt = CABUPDATE_DOING;
		}
		break;

	case CABUPDATE_COMPLETE:
		pGame->EnableUI(TFRAME_CABINET);
		break;
	}
}
// ----------------------------------------------------------------------------------------------------
void CTNormalCabinetDlg::SetCabinetInfo(UINT nCab, BOOL bOpen)
{
	CTCabinetDlg::SetCabinetInfo(nCab,bOpen);

	if( bOpen )
	{
		m_ppOpenBtn[nCab]->m_strText = CTChart::Format( TSTR_CABINET_BTN_USE, nCab+1);
		m_ppOpenBtn[nCab]->m_menu[TNM_LCLICK] = GM_CABINET_SELECT;
	}
	else
	{
		m_ppOpenBtn[nCab]->m_strText = CTChart::Format( TSTR_CABINET_BTN_OPEN, nCab+1);
		m_ppOpenBtn[nCab]->m_menu[TNM_LCLICK] = GM_CABINET_OPEN_WARN;
	}
}

// ====================================================================================================
BOOL CTNormalCabinetDlg::SelectCab(UINT nCab)
{
	if( !CTCabinetDlg::SelectCab(nCab) )
		return FALSE;

	if( m_pCabinets[m_nSelectCab].m_bNeedUpdate )
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		CTClientSession* pSession = pGame->GetSession();
		pSession->SendCS_CABINETITEMLIST_REQ(m_nSelectCab);
	}
	else
		NotifyUpdate();

	return TRUE;
}
// ====================================================================================================
INT64 CTNormalCabinetDlg::GetCabOpenCost(UINT nCab)
{
	if( nCab == 0 )
		return CABINET_COST_OPEN1;
	if( nCab == 1 )
		return CABINET_COST_OPEN2;

	return CABINET_COST_OPEN3;
}
// ----------------------------------------------------------------------------------------------------
INT64 CTNormalCabinetDlg::GetCabUseCost(UINT nCab)
{
	if( nCab == 0 )
		return CABINET_COST_USE1;
	if( nCab == 1 )
		return CABINET_COST_USE2;
	
	return CABINET_COST_USE3;
}
// ====================================================================================================












