#include "stdafx.h"
#include "TSelectCountry.h"
#include "TClientGame.h"

CTSelectCountry::CTSelectCountry( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pLeftImage = (TImageList*) FindKid(ID_CTRLINST_COUNTRY_LEFT);
	m_pRightImage = (TImageList*) FindKid(ID_CTRLINST_COUNTRY_RIGHT);
	m_pLeftDesc = (TList*) FindKid(ID_CTRLINST_INFO_LEFT);
	m_pRightDesc = (TList*) FindKid(ID_CTRLINST_INFO_RIGHT);
	m_pLeftTitle = (TComponent*) FindKid(ID_CTRLINST_CNAME_LEFT);
	m_pRightTitle = (TComponent*) FindKid(ID_CTRLINST_CNAME_RIGHT);
	m_pOK = (TButton*) FindKid( ID_CTRLINST_OK );
	m_pOK->m_menu[TNM_LCLICK] = GM_SELECT_COUNTRY_DLG;
	m_bSelectedCountry = (BYTE)-1;
}

CTSelectCountry::~CTSelectCountry()
{
}

void CTSelectCountry::ShowComponent(BOOL bVisible)
{
	if( bVisible )
	{
		m_bSelectedCountry = (BYTE)-1;
		m_bCountryLeft = rand()%2;
		m_bCountryRight = (m_bCountryLeft==TCONTRY_D) ? TCONTRY_C : TCONTRY_D;

		m_pLeftImage->SetCurImage( m_bCountryLeft + 2 );
		CString strINFO = CTChart::GetTCOUNTRYINFO( m_bCountryLeft );
		m_pLeftDesc->RemoveAll();
		m_pLeftDesc->ClipAndInsertString( strINFO );
		m_pLeftDesc->ShowScroll( m_pLeftDesc->GetItemCount() > m_pLeftDesc->m_itemholders.size() );
		m_pLeftTitle->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ m_bCountryLeft ] );

		m_pRightImage->SetCurImage( m_bCountryRight + 2 );
		strINFO = CTChart::GetTCOUNTRYINFO( m_bCountryRight );
		m_pRightDesc->RemoveAll();
		m_pRightDesc->ClipAndInsertString( strINFO );
		m_pRightDesc->ShowScroll( m_pRightDesc->GetItemCount() > m_pRightDesc->m_itemholders.size() );
		m_pRightTitle->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ m_bCountryRight ] );
	}

	CTClientUIBase::ShowComponent( bVisible );
}

void CTSelectCountry::OnMouseMove(UINT nFlags, CPoint pt)
{
	if( m_pLeftImage->HitTest(pt) )
		m_pLeftImage->SetCurImage( m_bCountryLeft );
	else
		m_pLeftImage->SetCurImage( m_bSelectedCountry == m_bCountryLeft ? m_bCountryLeft : m_bCountryLeft + 2 );
	
	if( m_pRightImage->HitTest(pt) )
		m_pRightImage->SetCurImage( m_bCountryRight );
	else
		m_pRightImage->SetCurImage( m_bSelectedCountry == m_bCountryRight ? m_bCountryRight : m_bCountryRight + 2 );

	CTClientUIBase::OnMouseMove(nFlags, pt);
}

void CTSelectCountry::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pLeftImage->HitTest(pt) )
	{
		m_bSelectedCountry = m_bCountryLeft;
		m_pLeftImage->SetCurImage( m_bCountryLeft );
		m_pRightImage->SetCurImage( m_bCountryRight + 2 );
	}

	if( m_pRightImage->HitTest(pt) )
	{
		m_bSelectedCountry = m_bCountryRight;
		m_pRightImage->SetCurImage( m_bCountryRight );
		m_pLeftImage->SetCurImage( m_bCountryLeft + 2 );
	}

	CTClientUIBase::OnLButtonUp(nFlags, pt);
}