#include "Stdafx.h"
#include "TPartyItemLottery.h"
#include "TClientGame.h"
#include "Resource.h"

// ===============================================================================
CTPartyItemLottery::VECTORINSTANCES		CTPartyItemLottery::ms_vFREES;
CTPartyItemLottery::VECTORINSTANCES		CTPartyItemLottery::ms_vACTIVES;
TComponent*								CTPartyItemLottery::ms_pPARENT;
LP_FRAMEDESC							CTPartyItemLottery::ms_pFRAMEDESC;
CSize									CTPartyItemLottery::ms_szSCREEN;
CPoint									CTPartyItemLottery::ms_ptINITPOS;
CRect									CTPartyItemLottery::ms_rcRECT;
CTPartyItemLottery::LISTINSTANCES		CTPartyItemLottery::ms_vGMTARGETS;
// ===============================================================================

// ===============================================================================
void CTPartyItemLottery::InitInstance(TComponent* pPARENT, LP_FRAMEDESC pDESC, CSize szSCREEN)
{
	ReleaseInstance();

	ms_pPARENT = pPARENT;
	ms_pFRAMEDESC = pDESC;

	ms_szSCREEN = szSCREEN;

	ms_ptINITPOS.x = T_INVALID;
	ms_ptINITPOS.y = T_INVALID;
}
// -------------------------------------------------------------------------------
void CTPartyItemLottery::ReleaseInstance()
{
	VECTORINSTANCES::iterator itr,end;
	
	itr = ms_vFREES.begin();
	end = ms_vFREES.end();
	for(;itr!=end; ++itr)
		delete *itr;

	ms_vFREES.clear();
	ms_vACTIVES.clear();
	ms_vGMTARGETS.clear();
}
// ===============================================================================
CTPartyItemLottery* CTPartyItemLottery::NewInstance(DWORD dwMonID, CTClientItem* pItem)
{
	CTPartyItemLottery* pInst;
	if( ms_vFREES.empty() )
		pInst = new CTPartyItemLottery(ms_pPARENT, ms_pFRAMEDESC);
	else
	{
		pInst = ms_vFREES.back();
		ms_vFREES.pop_back();
	}

	pInst->SetData(dwMonID, pItem);

	DWORD dwID;
	do { dwID = (UINT)::rand(); }
	while( ms_pPARENT->FindKid(dwID) );
	pInst->m_id = dwID;

	ms_pPARENT->AddKid(pInst);
	pInst->ShowComponent(TRUE);
	pInst->m_bActive = TRUE;

	ms_vACTIVES.push_back(pInst); 

	if( ms_ptINITPOS.x == T_INVALID )
	{
		pInst->GetComponentRect(&ms_rcRECT);
		
		ms_ptINITPOS.x = TPARTYITEMLOTTERY_INIT_X;
		ms_ptINITPOS.y = TPARTYITEMLOTTERY_INIT_Y;
	}

	INT nACTCNT = (INT) ms_vACTIVES.size();
	for(INT i=0; i<nACTCNT; ++i)
	{
		INT nCntX = i / TPARTYITEMLOTTERY_COUNT_Y;
		INT nCntY = i % TPARTYITEMLOTTERY_COUNT_Y;
        
		CPoint pt(
			ms_ptINITPOS.x + (ms_rcRECT.Width()+TPARTYITEMLOTTERY_SPACE_X)*nCntX,
			ms_ptINITPOS.y + (ms_rcRECT.Height()+TPARTYITEMLOTTERY_SPACE_Y)*nCntY );

		ms_vACTIVES[i]->MoveComponent(pt);
	}

	return pInst;
}
// -------------------------------------------------------------------------------
void CTPartyItemLottery::DeleteInstance(CTPartyItemLottery* pInst)
{
	VECTORINSTANCES::iterator itr,end;
	
	itr = ms_vACTIVES.begin();
	end = ms_vACTIVES.end();
	for(;itr!=end; ++itr)
	{
		if( (*itr) == pInst )
			break;
	}

	if( itr == end )
		return;

	pInst->ShowComponent(FALSE);
	ms_pPARENT->RemoveKid(pInst);

	pInst->SetData(NULL,NULL);
	pInst->m_bActive = FALSE;
	
	ms_vACTIVES.erase(itr);
	ms_vFREES.push_back(pInst);
}
// -------------------------------------------------------------------------------
void CTPartyItemLottery::DeleteAllInstance()
{
	while( !ms_vACTIVES.empty() )
	{
		CTPartyItemLottery* pInst = ms_vACTIVES.back();

		pInst->ShowComponent(FALSE);
		ms_pPARENT->RemoveKid(pInst);

		pInst->SetData(NULL,NULL);
		pInst->m_bActive = FALSE;
		
		ms_vFREES.push_back(pInst);
		ms_vACTIVES.pop_back();
	}
}
// ===============================================================================
INT CTPartyItemLottery::GetActiveInstCnt()
{
	return (INT) ms_vACTIVES.size();
}
// -------------------------------------------------------------------------------
CTPartyItemLottery* CTPartyItemLottery::GetActiveInst(INT nIdx)
{
	return ms_vACTIVES[nIdx];
}
// ===============================================================================
CTPartyItemLottery* CTPartyItemLottery::GetGMTarget()
{
	if( ms_vGMTARGETS.empty() )
		return NULL;

	CTPartyItemLottery* pTarget = ms_vGMTARGETS.front();
	ms_vGMTARGETS.pop_front();
	
	return pTarget;
}
// ===============================================================================

// ===============================================================================
CTPartyItemLottery::CTPartyItemLottery( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_pItem(NULL), m_bActive(FALSE)
{
	m_pItemIcon 	= static_cast<TImageList*>( FindKid(ID_CTRLINST_ICON) );
	m_pItemName 	= FindKid(ID_CTRLINST_NAME);
	m_pLeftTime 	= FindKid(ID_CTRLINST_TIME);
	m_pOkButton 	= FindKid(ID_CTRLINST_OK);
	m_pCancelButton	= FindKid(ID_CTRLINST_CANCEL);
	m_pCloseButton	= FindKid(ID_CTRLINST_CLOSE);
}
// -------------------------------------------------------------------------------
CTPartyItemLottery::~CTPartyItemLottery()
{
	if( m_pItem )
		delete m_pItem;
}
// ===============================================================================

// ===============================================================================
void CTPartyItemLottery::SetData(DWORD dwMonID, CTClientItem* pItem)
{
	if( m_pItem )
		delete m_pItem;

	m_dwMonID = dwMonID;
	m_pItem = pItem;

	if( m_pItem )
	{
		m_pItemName->m_strText = pItem->GetTITEM()->m_strNAME;

		LPTITEMVISUAL pTVISUAL = pItem->GetVisual();
		WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;
		m_pItemIcon->SetCurImage(wImg);
		
		m_dwLeftTime = LOTTERY_TIME;
		m_pLeftTime->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwLeftTime/1000);
	}
}
// ===============================================================================
void CTPartyItemLottery::NotifyGM(DWORD dwGM)
{
	if( m_pCommandHandler )
	{
		m_pCommandHandler->m_vCOMMAND.push_back(dwGM);
		ms_vGMTARGETS.push_back(this);
	}
}
// ===============================================================================

// ===============================================================================
BOOL CTPartyItemLottery::CanWithItemUI()
{
	return TRUE;
}
// ===============================================================================
void CTPartyItemLottery::OnLButtonUp(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonUp(nFlags,pt);

	if( m_pOkButton->HitTest(pt) )
		NotifyGM(GM_JOIN_ITEM_LOTTERY);
	else if( m_pCancelButton->HitTest(pt) || m_pCloseButton->HitTest(pt) )
		NotifyGM(GM_GIVEUP_ITEM_LOTTERY);
}
// ===============================================================================
HRESULT CTPartyItemLottery::Render( DWORD dwTickCount )
{
	if( IsVisible() )
	{
		if( m_dwLeftTime <= dwTickCount )
		{
			m_dwLeftTime = 0;
			NotifyGM(GM_GIVEUP_ITEM_LOTTERY);

			return S_OK;
		}
		
		m_dwLeftTime -= dwTickCount;
		m_pLeftTime->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwLeftTime/1000);
	}

	return CTClientUIBase::Render(dwTickCount);
}
// ===============================================================================
ITDetailInfoPtr CTPartyItemLottery::GetTInfoKey( const CPoint& point)
{
	ITDetailInfoPtr pInfo;

	if( !m_pItem || !m_pItemIcon->HitTest(point) )
		return pInfo;
	
	CRect rc;
	GetComponentRect(&rc);

	pInfo = CTDetailInfoManager::NewItemInst(m_pItem, rc);
	return pInfo;
}
// ===============================================================================















