#include "Stdafx.h"
#include "TItemUpCashDlg.h"
#include "TClientGame.h"
#include "TClientItem.h"
#include "TDetailInfoManager.h"

// CTItemUpCashSlot
// ====================================================================================================
void CTItemUpCashSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;

	m_pIcon->ShowComponent(bShow);
	m_pName->ShowComponent(bShow);
	m_pExplanation->ShowComponent(bShow);
	m_pBase->ShowComponent(bShow);
}
// ----------------------------------------------------------------------------------------------------
void CTItemUpCashSlot::Select(BOOL bSel)
{
	static_cast<TButton*>(m_pBase)->Select(bSel);
}
// ====================================================================================================

CTItemUpCashDlg::CTItemUpCashDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
 : CTClientUIBase(pParent, pDesc), m_nSlotNum(0)
{
	static const DWORD dwSLOTID[] = 
	{
		ID_CTRLINST_SLOT1,
		ID_CTRLINST_SLOT2,
		ID_CTRLINST_SLOT3,
	};

	CTCLSlotArray	vSlots;
	CTItemUpCashSlot* pSlot;

    m_pTopMessage = FindKid( ID_CTRLINST_MSG1 );
	m_pBottonMessage = FindKid( ID_CTRLINST_MSG2 );
	m_pBottonMessage2 = FindKid( ID_CTRLINST_MSG3 );

	for(INT i=0; i< MAX_ITEM_SLOT; ++i)
	{
		pSlot = new CTItemUpCashSlot();
		pSlot->SetSlot(i, FindKid(dwSLOTID[i]));
		vSlots.push_back(pSlot);
	}

	CTItemUpCashSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast<CTItemUpCashSlot*>(vSlots[0]);
	pSlot0->m_pBase->GetComponentPos(&ptBase);

	pSlot0->m_pIcon = static_cast<TImageList*>( FindKid(ID_CTRLINST_ITEMLIST) );
	pSlot0->m_pIcon->GetComponentPos(&ptTemp);
	CPoint pItemOffset = ptTemp - ptBase;

	pSlot0->m_pName = FindKid(ID_CTRLINST_NAME);
	pSlot0->m_pName->GetComponentPos(&ptTemp);
	pSlot0->m_pName->m_strText = "";
	CPoint ptNameOffset = ptTemp - ptBase;

	pSlot0->m_pExplanation = FindKid(ID_CTRLINST_EXPLANATION);
	pSlot0->m_pExplanation->GetComponentPos(&ptTemp);
	pSlot0->m_pExplanation->m_strText = "";
	CPoint ptExplanationOffset = ptTemp - ptBase;


	for(INT i=1; i < MAX_ITEM_SLOT; ++i)
	{
		pSlot = static_cast<CTItemUpCashSlot*>(vSlots[i]);

		pSlot->m_pIcon = new TImageList(this, pSlot0->m_pIcon->m_pDESC);
		pSlot->m_pName = new TComponent(this, pSlot0->m_pName->m_pDESC);
		pSlot->m_pExplanation = new TComponent(this, pSlot0->m_pExplanation->m_pDESC);

		pSlot->m_pName->m_strText = "";
		pSlot->m_pExplanation->m_strText = "";

		pSlot->m_pIcon->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();
		pSlot->m_pExplanation->m_id = GetUniqueID();

		AddKid( pSlot->m_pIcon );
		AddKid( pSlot->m_pName );
		AddKid( pSlot->m_pExplanation );

		pSlot->m_pBase->GetComponentPos(&ptBase);

		pSlot->m_pIcon->MoveComponent(ptBase+pItemOffset);
		pSlot->m_pName->MoveComponent(ptBase+ptNameOffset);
		pSlot->m_pExplanation->MoveComponent(ptBase+ptExplanationOffset);

	}

	m_pList = new CTCtrlList();
	m_pList->SetSlot(&vSlots);
}

CTItemUpCashDlg::~CTItemUpCashDlg(void)
{
	delete m_pList;
}

void CTItemUpCashDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown(nFlags, pt);
}

ITDetailInfoPtr CTItemUpCashDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo(NULL);

	for ( INT i = 0; i < m_pList->GetSlotCnt(); ++i )
	{
		CTItemUpCashSlot* pSlot = static_cast<CTItemUpCashSlot*>(m_pList->GetSlotNotUpdate(i));

		if ( pSlot->m_pIcon->HitTest( point ) )
		{
			if ( pSlot->m_pIcon->IsEnable() )
			{
				CRect rc;

				GetComponentRect(&rc);
				
				CTClientItem* pItem = m_vItems[i%2];
				pInfo = CTDetailInfoManager::NewItemInst( pItem, rc );
				pInfo->SetDir(TRUE, TRUE, TRUE);

				return pInfo;
			}
		}
	}

	return pInfo;
}

void CTItemUpCashDlg::ShowComponent( BOOL bVisible /*= TRUE*/ )
{
	static const BYTE bEnable[][MAX_ITEM_SLOT]	= { 
		{ FALSE, FALSE, TRUE},
		{ TRUE, TRUE, FALSE}
	};

	CTClientUIBase::ShowComponent(bVisible);
	
	INT nSlotNum = (INT) m_vItems.size() - 1;
	
	if ( nSlotNum < 0 )
		nSlotNum = 0;
	
	INT nIconCnt = 0;
	for( INT i = 0; i < m_pList->GetSlotCnt(); ++i )
	{
		CTItemUpCashSlot* pSlot = static_cast<CTItemUpCashSlot*>(m_pList->GetSlotNotUpdate(i));
		
		if ( bEnable[nSlotNum][i] && !m_vItems.empty() )
		{
			LPTITEM pItem = m_vItems[nIconCnt]->GetTITEM();
			pSlot->m_pIcon->SetCurImage( m_vItems[nIconCnt]->GetVisual()->m_wIcon );
			pSlot->m_pName->m_strText = pItem->m_strNAME;
			pSlot->m_pExplanation->m_strText = m_vExplanation[nIconCnt];
			
			nIconCnt++;
		}
		
		pSlot->m_pIcon->ShowComponent( bVisible && bEnable[nSlotNum][i] );
		pSlot->m_pName->ShowComponent( bVisible && bEnable[nSlotNum][i] );
		pSlot->m_pExplanation->ShowComponent( bVisible && bEnable[nSlotNum][i] );
	}

}

void CTItemUpCashDlg::AddItem( LPTITEM pItem, CString strString )
{
	CTClientItem* pClientItem = new CTClientItem;
	pClientItem->SetTITEM( pItem );
	m_vItems.push_back( pClientItem );

	m_vExplanation.push_back( strString );

	m_nSlotNum++;
}


void CTItemUpCashDlg::Clear()
{
	for ( INT i = 0; i < (INT) m_vItems.size(); ++i )
		delete m_vItems[i];

	m_vItems.clear();
	m_vExplanation.clear();

	m_pTopMessage->m_strText = "";
	m_pBottonMessage->m_strText = "";
	m_pBottonMessage2->m_strText = "";

	m_pBottonMessage->m_pFont->m_dwColor = TCOLOR_TXT_YELLOW;
}

void CTItemUpCashDlg::SetMode( INT nMode )
{
	m_nMode = nMode % MODE_TOTAL;
}

INT CTItemUpCashDlg::GetMode()
{
	return m_nMode;
}