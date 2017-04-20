#include "Stdafx.h"
#include "TUpItemSellDlg.h"
#include "TClientGame.h"

CTUpItemSellDlg::CTUpItemSellDlg(TComponent* pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_bMode = MODE_SELL;
	m_pMSG = FindKid( ID_CTRLINST_MSG );
	m_pID = (TEdit*) FindKid( ID_CTRLINST_PW );
	m_pItemList = (TImageList*) FindKid( ID_CTRLINST_ITEMLIST );
	m_pTXT = FindKid( ID_CTRLINST_TXT1 );
	m_strSecurityCode = "0000";

	// 일본이면 인증번호로 바꾼다.
	if ( CTNationOption::JAPAN )
		m_pTXT->m_strText = CTChart::LoadString( TSTR_WARN_UPITEM_SECURITYCODE );
}

CTUpItemSellDlg::~CTUpItemSellDlg(void)
{


}

BOOL CTUpItemSellDlg::CanWithItemUI()
{
	return TRUE;
}

void CTUpItemSellDlg::Clear()
{
	m_pItemList->SetCurImage( 0 );
	m_pID->ClearText();
	m_pMSG->ClearText();
	
	m_pItem = NULL;
}


void CTUpItemSellDlg::SetClientItem( CTClientItem* pItem )
{
	m_pItem = pItem;
	m_pItemList->SetCurImage( m_pItem->GetVisual()->m_wIcon );
}

void CTUpItemSellDlg::ShowComponent( BOOL bVisible /*= TRUE*/ )
{
	CTClientUIBase::ShowComponent(bVisible);

	if ( bVisible )
		m_menu[TNM_ENTER] = GM_WARN_UPITEM_SELL_OK;
	else
		m_menu[TNM_ENTER] = TCML_ID_NULL;
	//// m_menu[TNM_ENTER] = vYesID;
}


void CTUpItemSellDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pID->GetFocus() )
	{
		//pGame->OnGM_WARN_UPITEM_SELL_OK();	
	}
	else
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}


void CTUpItemSellDlg::SetMode( BYTE bMode )
{
	m_bMode = bMode;
}

ITDetailInfoPtr CTUpItemSellDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo(NULL);

	if ( m_pItemList->HitTest( point ) )
	{
		CRect rc;

		GetComponentRect(&rc);

		pInfo = CTDetailInfoManager::NewItemInst( m_pItem, rc );
		pInfo->SetDir(TRUE, TRUE, TRUE);

		return pInfo;
	}

	return pInfo;
}

TEdit* CTUpItemSellDlg::GetCurEdit()
{
	if ( IsVisible() && m_bFocus )
		return m_pID;

	return NULL;
}