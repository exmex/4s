#include "stdafx.h"
#include "TAidCountry.h"
#include "TClientGame.h"

CTAidCountry::CTAidCountry( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pGuageD = (TGauge*) FindKid( ID_CTRLINST_GAUGE_D );
	m_pGuageC = (TGauge*) FindKid( ID_CTRLINST_GAUGE_C );
	m_pPercentD = FindKid( ID_CTRLINST_TEXT_D );
	m_pPercentC = FindKid( ID_CTRLINST_TEXT_C );
	m_pMsg = FindKid( ID_CTRLINST_MSG );
	m_pGuageD->SetStyle(TGS_GROW_RIGHT);
	m_pGuageC->SetStyle(TGS_GROW_LEFT);
}

CTAidCountry::~CTAidCountry()
{
}

void CTAidCountry::SetInfo(DWORD dwDefugel, DWORD dwCraxion)
{
	FLOAT fD = (100.0f * dwDefugel) / FLOAT(dwDefugel + dwCraxion);
	FLOAT fC = (100.0f * dwCraxion) / FLOAT(dwDefugel + dwCraxion);

	m_pGuageD->SetGauge( (INT) dwDefugel, dwDefugel + dwCraxion );
	m_pGuageC->SetGauge( (INT) 100, 100 );

	CString strD;
	strD.Format( " (%.1f%%)", fD );
	m_pPercentD->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ TCONTRY_D ] );
	m_pPercentD->m_strText += strD;
	
	CString strC;
	strC.Format( " (%.1f%%)", fC );
	m_pPercentC->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ TCONTRY_C ] );
	m_pPercentC->m_strText += strC;

	if( dwDefugel < dwCraxion  )
	{
		if( 5.0f <= fC-fD )
		{
			m_pMsg->m_strText = CTChart::LoadString( TSTR_NATIONALITY_D );
			m_eAllow = ALLOW_D;
		}
		else
		{
			m_pMsg->m_strText = CTChart::LoadString( TSTR_NATIONALITY_BOTH );
			m_eAllow = ALLOW_ALL;
		}
	}
	else if( dwCraxion < dwDefugel )
	{
		if( 5.0f <= fD-fC )
		{
			m_pMsg->m_strText = CTChart::LoadString( TSTR_NATIONALITY_C );
			m_eAllow = ALLOW_C;
		}
		else
		{
			m_pMsg->m_strText = CTChart::LoadString( TSTR_NATIONALITY_BOTH );
			m_eAllow = ALLOW_ALL;
		}
	}
	else
	{
		m_pMsg->m_strText = CTChart::LoadString( TSTR_NATIONALITY_BOTH );
		m_eAllow = ALLOW_ALL;
	}
}