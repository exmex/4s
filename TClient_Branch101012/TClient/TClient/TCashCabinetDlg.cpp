#include "StdAfx.h"
#include "TNormalCabinetDlg.h"
#include "TClientGame.h"
#include "TClient.h"
#include "Resource.h"

// CTCashCabinetDlg
// ====================================================================================================
CTCashCabinetDlg::CTCashCabinetDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTCabinetDlg(pParent, pDesc),
m_dwRemainDays(0),
m_dwItemCount(0),
m_dwCash(0),
m_dwBonusCash(0),
m_dwTotal(0),
m_bChargeTwinkle(0),
m_bRequestShowCashCabinet(FALSE)
{
	m_bRememberPrevScrollPos = TRUE;
	FindKid(ID_CTRLINST_TITLE)->m_strText = CTChart::LoadString( TSTR_CABINET );
}
// ----------------------------------------------------------------------------------------------------
CTCashCabinetDlg::~CTCashCabinetDlg()
{
}
// ====================================================================================================

// ====================================================================================================
void CTCashCabinetDlg::SetupComponent()
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
		//ID_CTRLINST_TXT_COST,
		//ID_CTRLINST_CAB1,
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
	m_nMaxCabCnt	= 1; // 캐쉬 아이템의 경우 보관함은 하나임!!
	m_nMaxStorage	= 0;

	m_pSelectTxt= NULL;
	m_pCostTxt	= NULL;
	m_ppOpenBtn = NULL;

	m_pExtensionPeriodBtn = FindKid(ID_CTRLINST_CAB1);
	m_pExtensionPeriodBtn->m_strText = CTChart::LoadString( TSTR_EXTENSION_PERIOD);
	m_pExtensionPeriodBtn->m_menu[TNM_LCLICK] = GM_CASHCAB_EXTENSIONPERIOD;
	m_pExtensionPeriodBtn->SetStyle( m_pExtensionPeriodBtn->GetStyle() & ~TBS_STATE_BUTTON );

	m_pGetBtn	= FindKid(ID_CTRLINST_CAB2);
	m_pGetBtn->m_strText = CTChart::LoadString( TSTR_GET_ITEM);
	m_pGetBtn->m_menu[TNM_LCLICK] = GM_CASHCAB_GETITEM;
	m_pGetBtn->SetStyle( m_pGetBtn->GetStyle() & ~TBS_STATE_BUTTON );

	m_pCancelBtn= FindKid(ID_CTRLINST_CAB3);
	m_pCancelBtn->m_strText = CTChart::LoadString( TSTR_CANCEL);
	m_pCancelBtn->m_menu[TNM_LCLICK] = GM_CLOSE_UI;
	m_pCancelBtn->SetStyle( m_pGetBtn->GetStyle() & ~TBS_STATE_BUTTON );

	m_pRemainDayItemCountText = FindKid(ID_CTRLINST_TXT_COUNT);
	m_pRemainDayItemCountText->m_strText = "";
	m_pRemainDayItemCountText->SetTextAlign( ALIGN_CENTER );

	m_pCashText = FindKid(ID_CTRLINST_TXT_COST);
	m_pCashText->m_strText = "";
	m_pCashText->SetTextAlign( ALIGN_RIGHT );

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
	m_bShowCashCharge = TRUE;

	SelectCab(0);
	SetCabinetInfo(0,TRUE);
	CompleteCabinetUpdate(0);
}
// ====================================================================================================
void CTCashCabinetDlg::RequestShowComponent()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession = pGame->GetSession();
	
	m_bRequestShowCashCabinet = TRUE;
	pSession->SendCS_CASHITEMCABINET_REQ();
}
// ====================================================================================================
void CTCashCabinetDlg::SetInfoCashCabinet(
								CTime dEndTime,
								DWORD dwCount,
								DWORD dwCash,
								DWORD dwBonusCash )
{
	m_dEndTime = dEndTime;

	if( m_dEndTime > CTClientApp::m_dCurDate)
	{
		CTimeSpan timeSpan = m_dEndTime - CTClientApp::m_dCurDate;
		m_dwRemainDays = (DWORD) timeSpan.GetDays();
		if( m_dwRemainDays == 0 && timeSpan.GetHours() != 0 )
			m_dwRemainDays = 1;
	}
	else
		m_dwRemainDays = 0;

	m_dwItemCount = dwCount;

	if( IsPeriodRental() )
	{
		m_pRemainDayItemCountText->m_strText = CTChart::Format( TSTR_CASHCABINET_REMAIN_COUNT_ON_PR, m_dwRemainDays, m_dwItemCount);
	}
	else
	{
		m_pRemainDayItemCountText->m_strText = CTChart::Format( TSTR_CASHCABINET_REMAIN_COUNT, m_dwRemainDays, m_dwItemCount);
	}

	m_dwCash = dwCash;
	m_dwBonusCash = dwBonusCash;

	CString strCASH;

	if( CTNationOption::JAPAN )
		strCASH = CTChart::Format( TSTR_FMT_CASH, dwCash );
	else if ( CTNationOption::RUSSIA )
	{
		FLOAT fCash = dwCash / 100.0f;
		FLOAT fBonusCash = dwBonusCash / 100.0f;

		strCASH = CTChart::Format( TSTR_FMT_CASHNBONUS_FLOAT, fCash, fBonusCash );
	}
	else
		strCASH = CTChart::Format( TSTR_FMT_CASHNBONUS, dwCash, dwBonusCash);

	m_pCashText->m_strText = strCASH;
}

BOOL CTCashCabinetDlg::AddItem(UINT nCab, CTCabinetItem* pItem)
{
	m_pCabinets[nCab].m_vItems.push_back( pItem );
	NotifyUpdate();

	return TRUE;
}

void CTCashCabinetDlg::ReCountItem()
{
	DWORD dwCount = 0;

	for( INT i=0 ; i < m_pCabinets[0].m_vItems.size() ; ++i )
	{
		CTCabinetItem* pItem = (CTCabinetItem*)m_pCabinets[0].m_vItems[i];
		LPTITEM pTITEM = pItem->m_pClientItem->GetTITEM();

		if( pTITEM &&
			pTITEM->m_bIsSpecial == 0 )
			++dwCount;
	}

	m_dwItemCount = dwCount;

	SetInfoCashCabinet(
		m_dEndTime,
		m_dwItemCount,
		m_dwCash,
		m_dwBonusCash);
}

HRESULT CTCashCabinetDlg::Render(DWORD dwTickCount)
{
	UpdateChargeBtnTwinkle( dwTickCount);
	return CTCabinetDlg::Render( dwTickCount );
}

void CTCashCabinetDlg::OnMouseMove(UINT nFlags, CPoint pt)
{
	m_bChargeTwinkle = m_pCharge && !m_pCharge->HitTest( pt );

	if( m_pCharge &&
		m_dwCash == 0 &&
		m_dwBonusCash == 0)
	{
		m_pCharge->EnableComponent(FALSE);
		CTClientUIBase::OnMouseMove( nFlags, pt);
		m_pCharge->EnableComponent(TRUE);
		return ;
	}

	CTClientUIBase::OnMouseMove( nFlags, pt);
}

ITDetailInfoPtr CTCashCabinetDlg::GetTInfoKey( const CPoint& point )
{
	CTCtrlListItem* pHitItem = m_pList->GetHitItem(point);
	if( !pHitItem )
	{
		CRect rc;
		GetComponentRect( &rc );
		return CTDetailInfoManager::NewDefTooltipInst(
			CTChart::LoadString( TSTR_CABINET ),
			CTChart::LoadString( TSTR_CASHCABINET_TOOLTIP ),
			rc );
	}

	return CTCabinetDlg::GetTInfoKey( point );
}

void CTCashCabinetDlg::UpdateChargeBtnTwinkle(DWORD dwTick)
{
	if( m_pCharge &&
		m_bShowCashCharge &&
		m_bChargeTwinkle &&
		m_dwCash == 0 &&
		m_dwBonusCash == 0)
	{
		m_dwTotal += dwTick;

		if( m_dwTotal > 500 )
		{
			TButton* pBTN = (TButton*) m_pCharge;
			
			if( (pBTN->GetState()&TBUTTON_STATE_HOVER) == TBUTTON_STATE_HOVER)
				pBTN->Select( (BYTE)0 );
			else
				pBTN->Select( (BYTE)TBUTTON_STATE_HOVER);

			m_dwTotal = 0;
		}
	}
}
