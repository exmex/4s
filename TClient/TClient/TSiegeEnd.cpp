#include "StdAfx.h"
#include "TSiegeEnd.h"
#include "TClientGame.h"

CTSiegeEnd::CTSiegeEnd( TComponent *pParent, LP_FRAMEDESC pDesc )
	: CTClientUIBase(pParent, pDesc)
{
	InitSiegeEnd();
}

CTSiegeEnd::~CTSiegeEnd()
{

}

void CTSiegeEnd::InitSiegeEnd()
{
	m_pTGuildNameAtk = FindKid( ID_CTRLINST_GUILDNAME_ATK );
	m_pTGuildNameDef = FindKid( ID_CTRLINST_GUILDNAME_DEF );
	m_pTCountryAtk = FindKid( ID_CTRLINST_GUILDMARK_ATK );
	m_pTCountryDef = FindKid( ID_CTRLINST_GUILDMARK_DEF );
	m_pTGaugeTotalAtk = static_cast< TGauge* >( FindKid( ID_CTRLINST_GAUGE_ATK ) );
	m_pTGaugeTotalDef = static_cast< TGauge* >( FindKid( ID_CTRLINST_GAUGE_DEF ) );
	m_pTKillAtk = FindKid( ID_CTRLINST_KILL_ATK );
	m_pTKillDef = FindKid( ID_CTRLINST_KILL_DEF );
	m_pTExit = FindKid( ID_CTRLINST_CLOSE );
	m_pTCloseBtn = FindKid( ID_CTRLINST_CLOSE_BTN );

	m_pTGaugeTotalAtk->SetStyle( TGS_GROW_LEFT );
	m_pTGaugeTotalDef->SetStyle( TGS_GROW_RIGHT );

	m_pTGaugeTotalAtk->SetGauge( 0, 100 );
	m_pTGaugeTotalDef->SetGauge( 0, 100 );
	m_pTGaugeTotalAtk->m_strText.Format( "%.1f%%", 0.0f );
	m_pTGaugeTotalDef->m_strText.Format( "%.1f%%", 0.0f );

	CString initNum;
	initNum.Format( "%08d", 0 );

	m_pTKillAtk->m_strText = m_pTKillDef->m_strText = initNum;

	((TButton*)m_pTExit)->EnableComponent( FALSE );
	((TButton*)m_pTCloseBtn)->EnableComponent( FALSE );

	m_bCounting = FALSE;
}

void CTSiegeEnd::Endwar(
			BYTE bType,
			DWORD dwWinGuildID,
			DWORD dwDefTotal,
			DWORD dwAtkTotal,
			CString strDefName,
			DWORD dwDefPower,
			WORD wDefPoint,
			WORD wDefKillPoint,
			CString strAtkName,
			DWORD dwAtkPower,
			WORD wAtkPoint,
			WORD wAtkKillPoint)
{
	ShowComponent( TRUE );
	EnableComponent( TRUE );

	m_pTGuildNameAtk->m_strText = strAtkName;
	m_pTGuildNameDef->m_strText = strDefName;

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	if( pMainChar )
	{
		if( pMainChar->m_strGUILD == strAtkName ) // 공격이라면
		{
			((TImageList*)m_pTCountryAtk)->SetCurImage( pMainChar->m_bContryID );
			((TImageList*)m_pTCountryDef)->SetCurImage( !pMainChar->m_bContryID );
		}
		else // 수비측이라면
		{
			((TImageList*)m_pTCountryDef)->SetCurImage( pMainChar->m_bContryID );
			((TImageList*)m_pTCountryAtk)->SetCurImage( !pMainChar->m_bContryID );
		}
	}

	m_dwPowerAtk = dwAtkPower;
	m_wKillAtk = wAtkKillPoint;

	m_dwPowerDef = dwDefPower;
	m_wKillDef = wDefKillPoint;

	m_pTKillAtk->m_strText.Format( "%06d", 0 );
	m_pTKillDef->m_strText.Format( "%06d", 0 );

	m_pTKillAtk->m_dwColor = D3DCOLOR_ARGB( 255, 190, 190, 190 );
	m_pTKillDef->m_dwColor = D3DCOLOR_ARGB( 255, 190, 190, 190 );

	((TButton*)m_pTExit)->EnableComponent( FALSE );
	((TButton*)m_pTCloseBtn)->EnableComponent( FALSE );

	m_bCounting = TRUE;
	m_dwKillCountingAtk = m_dwKillCountingDef = 0;
	m_dwPowerCountingAtk = m_dwPowerCountingDef = 0;
}

void CTSiegeEnd::EndSkygarden(
		BYTE bWinCountry,
		BYTE bLosecountry)
{
	CString strWin = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ bWinCountry ] );
	CString strLose = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ bLosecountry ] );

	m_pTGuildNameAtk->m_strText = strWin;
	m_pTGuildNameDef->m_strText = strLose;

	((TImageList*)m_pTCountryAtk)->SetCurImage( bWinCountry );
	((TImageList*)m_pTCountryDef)->SetCurImage( bLosecountry );
		
	m_pTGaugeTotalAtk->m_strText = "승리";
	m_pTGaugeTotalDef->m_strText = "패배";
	m_pTGaugeTotalAtk->SetGauge( 100, 100 );
	m_pTGaugeTotalDef->SetGauge( 100, 100 );

	m_dwPowerAtk = 0;
	m_wKillAtk = 0;

	m_dwPowerDef = 0;
	m_wKillDef = 0;

	m_pTKillAtk->m_strText.Empty();
	m_pTKillDef->m_strText.Empty();

	m_pTKillAtk->m_dwColor = D3DCOLOR_ARGB( 255, 190, 190, 190 );
	m_pTKillDef->m_dwColor = D3DCOLOR_ARGB( 255, 190, 190, 190 );

	((TButton*)m_pTExit)->EnableComponent( FALSE );
	((TButton*)m_pTCloseBtn)->EnableComponent( FALSE );

	m_bCounting = FALSE;
	m_dwKillCountingAtk = m_dwKillCountingDef = 0;
	m_dwPowerCountingAtk = m_dwPowerCountingDef = 0;

	ShowComponent( TRUE );
	EnableComponent( TRUE );
}

HRESULT CTSiegeEnd::Render(DWORD dwTickCount)
{
	if(!IsVisible())
		return S_OK;

	if( m_bCounting )
	{
		BOOL bFullCount[4] = {FALSE};
		bFullCount[0] = !CountNumber( &m_dwKillCountingAtk, (DWORD)m_wKillAtk, m_pTKillAtk );
		bFullCount[1] = !CountNumber( &m_dwKillCountingDef, (DWORD)m_wKillDef, m_pTKillDef );
		bFullCount[2] = !CountNumber( &m_dwPowerCountingAtk, m_dwPowerAtk, NULL );
		bFullCount[3] = !CountNumber( &m_dwPowerCountingDef, m_dwPowerDef, NULL );

		if( bFullCount[0] &&
			bFullCount[1] &&
			bFullCount[2] &&
			bFullCount[3] )
		{
			m_bCounting = FALSE;
			((TButton*)m_pTExit)->EnableComponent( TRUE );
			((TButton*)m_pTCloseBtn)->EnableComponent( TRUE );
		}

		// total gauge
		DWORD dwTotal = m_dwPowerAtk + m_dwPowerDef;
		DWORD dwAtkCurTotal = 0;
		DWORD dwDefCurTotal = 0;

		SetGauge( m_dwPowerCountingAtk, dwTotal, m_pTGaugeTotalAtk );
		SetGauge( m_dwPowerCountingDef, dwTotal, m_pTGaugeTotalDef );

		FLOAT fTotalAtk = (FLOAT)((FLOAT)m_dwPowerCountingAtk * 100.0f/ (FLOAT)dwTotal);
		FLOAT fTotalDef = 100.0f - fTotalAtk;
		m_pTGaugeTotalAtk->m_strText.Format( "%.1f%%", fTotalAtk );
		m_pTGaugeTotalDef->m_strText.Format( "%.1f%%", fTotalDef );
	}

	CTClientUIBase::Render( dwTickCount );

	return S_OK;
}

BOOL CTSiegeEnd::CountNumber( DWORD* dwCurCount, DWORD dwMaxCount, TComponent* pComp )
{
	if( dwMaxCount < 100 )
		*dwCurCount += 1;
	else
		(*dwCurCount) += (DWORD)(dwMaxCount * 0.01f);

	if( *dwCurCount >= dwMaxCount )
	{
		*dwCurCount = dwMaxCount;

		if( pComp )
			pComp->m_strText.Format( "%06d", *dwCurCount );

		return FALSE;
	}

	if( pComp )
		pComp->m_strText.Format( "%06d", *dwCurCount );

	return TRUE;
}

void CTSiegeEnd::SetGauge( DWORD CurCount, DWORD MaxCount, TGauge* pGauge )
{
	if( pGauge )
		pGauge->SetGauge( (int)CurCount, (int)MaxCount );
}



