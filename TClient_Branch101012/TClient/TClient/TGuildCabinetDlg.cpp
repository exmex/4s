#include "StdAfx.h"
#include "TNormalCabinetDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// CTGuildCabinetDlg
// ====================================================================================================
CTGuildCabinetDlg::CTGuildCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTCabinetDlg(pParent, pDesc)
{
}
// ----------------------------------------------------------------------------------------------------
CTGuildCabinetDlg::~CTGuildCabinetDlg()
{
}
// ====================================================================================================

// ====================================================================================================
void CTGuildCabinetDlg::SetupComponent()
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

	static const DWORD dwDELETE[] = 
	{
		ID_CTRLINST_TXT_SELCAB,
		ID_CTRLINST_TXT_COST,
		ID_CTRLINST_CAB1,
		NULL
	};

	INT i = 0;
	while( dwDELETE[i] )
	{
		TComponent* pDEL = FindKid(dwDELETE[i]);
		RemoveKid(pDEL);
		delete pDEL;

		++i;
	}

	m_nMaxItemSlot	= MAX_ITEM_SLOT;
	m_nMaxItemCol	= MAX_ITEM_COLUMN;
	m_nMaxCabCnt	= 1; // 길드 보관함은 하나임!!
	m_nMaxStorage	= 0; // 길드 레벨에 따라 결정! 아직 정해 지지 않았음!

	m_pSelectTxt= NULL;
	m_pCostTxt	= NULL;
	m_ppOpenBtn = NULL;

	m_pGetBtn	= FindKid(ID_CTRLINST_CAB2);
	m_pGetBtn->m_strText = CTChart::LoadString( TSTR_GET_ITEM);
	m_pGetBtn->m_menu[TNM_LCLICK] = GM_GUILDCAB_GETITEM;
	m_pGetBtn->SetStyle( m_pGetBtn->GetStyle() & ~TBS_STATE_BUTTON );

	m_pCancelBtn= FindKid(ID_CTRLINST_CAB3);
	m_pCancelBtn->m_strText = CTChart::LoadString( TSTR_CANCEL);
	m_pCancelBtn->m_menu[TNM_LCLICK] = GM_CLOSE_UI;
	m_pCancelBtn->SetStyle( m_pGetBtn->GetStyle() & ~TBS_STATE_BUTTON );

	m_pTitle	= FindKid(ID_CTRLINST_TITLE);
	m_pCabinets = new Cabinet[m_nMaxCabCnt];

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

	SelectCab(0);
	SetCabinetInfo(0,TRUE);
	CompleteCabinetUpdate(0);
}
// ====================================================================================================
void CTGuildCabinetDlg::RequestShowComponent()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession = pGame->GetSession();
	pSession->SendCS_GUILDCABINETLIST_REQ();
}
// ====================================================================================================












