#include "StdAfx.h"
#include "Resource.h"
#include "TSiegeUI.h"
#include "TClientGame.h"

void CTSiegeUI::_set_strText( TComponent* pComp, CString strText )
{
	if( pComp )
		pComp->m_strText = strText;
}

void CTSiegeUI::_get_hmsTime( DWORD dwTick, WORD* _hour, WORD* _minute, WORD* _second )
{
	dwTick /= 1000;

	if( _hour != NULL )
	{
		*_hour = (WORD)(dwTick / 3600);
		dwTick -= *_hour * 3600;
	}
	if( _minute != NULL )
	{
		*_minute = (WORD)(dwTick / 60);
		dwTick -= *_minute * 60;
	}
	if( _second != NULL )
	{
		*_second = (WORD)dwTick;
	}
}

CTSiegeUI::CTSiegeUI( TComponent *pParent, LP_FRAMEDESC pDesc )
	: CTClientUIBase(pParent, pDesc)
{
	InitSiegeUI();
}

CTSiegeUI::~CTSiegeUI()
{
	EndWar();
}

void CTSiegeUI::InitSiegeUI()
{
	m_pTTopBase = FindKid( ID_CTRLINST_WINDOW_BASE );
	m_pTGaugeBarAtk = FindKid( ID_CTRLINST_GAUGE_ATK );
	m_pTGaugeBarDef = FindKid( ID_CTRLINST_GAUGE_DEF );
	m_pTGaugeAtk = FindKid( ID_CTRLINST_TXT_GAUGE_ATK );
	m_pTGaugeDef = FindKid( ID_CTRLINST_TXT_GAUGE_DEF );
	m_pTTimer = FindKid( ID_CTRLINST_TIMER );
	m_pTKillPointAtk = FindKid( ID_CTRLINST_KILLPOINT_ATK );
	m_pTKillPointDef = FindKid( ID_CTRLINST_KILLPOINT_DEF );
    m_pTBallCharAtk1 = FindKid( ID_CTRLINST_CHARBALL_ATK1 );
	m_pTBallCharAtk1Time = FindKid( ID_CTRLINST_CHARBALL_ATKTIME1 );
	m_pTBallCharAtk2 = FindKid( ID_CTRLINST_CHARBALL_ATK2 );
	m_pTBallCharAtk2Time = FindKid( ID_CTRLINST_CHARBALL_ATKTIME2 );
	m_pTBallCharDef1 = FindKid( ID_CTRLINST_CHARBALL_DEF1 );
	m_pTBallCharDef1Time = FindKid( ID_CTRLINST_CHARBALL_DEFTIME1 );
	m_pTBallCharDef2 = FindKid( ID_CTRLINST_CHARBALL_DEF2 );
	m_pTBallCharDef2Time = FindKid( ID_CTRLINST_CHARBALL_DEFTIME2 );
	m_pTGuildNameAtk = FindKid( ID_CTRLINST_GUILD_ATK );
	m_pTGuildNameDef = FindKid( ID_CTRLINST_GUILD_DEF );

	_set_strText( m_pTGaugeBarAtk );
	_set_strText( m_pTGaugeBarDef );
	_set_strText( m_pTGaugeAtk/*, CTChart::Format( TSTR_FMT_SIEGE_ATK, 50.0f )*/ );
	_set_strText( m_pTGaugeDef/*, CTChart::Format( TSTR_FMT_SIEGE_DEF, 50.0f )*/ );
	_set_strText( m_pTTimer );
	_set_strText( m_pTKillPointAtk );
	_set_strText( m_pTKillPointDef );
	_set_strText( m_pTBallCharAtk1 );
	_set_strText( m_pTBallCharAtk1Time );
	_set_strText( m_pTBallCharDef1 );
	_set_strText( m_pTBallCharDef1Time );
	_set_strText( m_pTBallCharAtk2 );
	_set_strText( m_pTBallCharAtk2Time );
	_set_strText( m_pTBallCharDef2 );
	_set_strText( m_pTBallCharDef2Time );
	_set_strText( m_pTGuildNameAtk );
	_set_strText( m_pTGuildNameDef );

	((TGauge*)m_pTGaugeBarAtk)->SetStyle(TGS_GROW_RIGHT);
	((TGauge*)m_pTGaugeBarDef)->SetStyle(TGS_GROW_RIGHT);
	((TGauge*)m_pTGaugeBarAtk)->SetGauge(50, 100);
	((TGauge*)m_pTGaugeBarDef)->SetGauge(50, 100);

	//!<---
	m_pTKillPointAtk->m_strText.Format( "%06d", 0 );
	m_pTKillPointDef->m_strText.Format( "%06d", 0 );
	
	//!<---
	m_pTBallCharAtk1Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, 0, 0 );
	m_pTBallCharAtk2Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, 0, 0 );
	m_pTBallCharDef1Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, 0, 0 );
	m_pTBallCharDef2Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, 0, 0 );

	//!<---
	CString strNone = CTChart::Format( TSTR_SIEGEBALL_NONE );
	m_pTBallCharAtk1->m_strText = CTChart::Format( TSTR_FMT_NAME, strNone );
	m_pTBallCharAtk2->m_strText = CTChart::Format( TSTR_FMT_NAME, strNone );
	m_pTBallCharDef1->m_strText = CTChart::Format( TSTR_FMT_NAME, strNone );
	m_pTBallCharDef2->m_strText = CTChart::Format( TSTR_FMT_NAME, strNone );

	CalcTimer( 0 );

	DWORD vHideList[TSIEGE_HIDE_COUNT] =
	{
		ID_CTRLINST_DETAIL_BASE,
		ID_CTRLINST_TXT_KILL,
		ID_CTRLINST_TXT_BALL,
		ID_CTRLINST_GUILD_ATK,
		ID_CTRLINST_GUILD_DEF,
		ID_CTRLINST_KILLPOINT_ATK,
		ID_CTRLINST_KILLPOINT_DEF,
		ID_CTRLINST_CHARBALL_ATK1,
		ID_CTRLINST_CHARBALL_ATK2,
		ID_CTRLINST_CHARBALL_DEF1,
		ID_CTRLINST_CHARBALL_DEF2,
		ID_CTRLINST_CHARBALL_ATKTIME1,
		ID_CTRLINST_CHARBALL_ATKTIME2,
		ID_CTRLINST_CHARBALL_DEFTIME1,
		ID_CTRLINST_CHARBALL_DEFTIME2
	};

	m_bSubHide = FALSE;

	for( int i=0; i<TSIEGE_HIDE_COUNT; ++i )
	{
		m_pTHideComp[i] = FindKid( vHideList[i] );
		m_pTHideComp[i]->ShowComponent( !m_bSubHide );
	}

	ShowComponent( FALSE );
	EnableComponent( FALSE );
}

void CTSiegeUI::CalcTimer( DWORD dwLeftTick )
{
	WORD _hour = 0;
	WORD _minute = 0;
	WORD _second = 0;
	
	_get_hmsTime( dwLeftTick, &_hour, &_minute, &_second );

	m_pTTimer->m_strText = CTChart::Format(
		TSTR_FMT_SIEGE_TIMER,
		_hour,
		_minute,
		_second );
}

void CTSiegeUI::RecvBalanceOfPower(
								   FLOAT fPower_Def,
								   DWORD dwLeftTick,
								   WORD wKillPoint_Atk,
								   WORD wKillPoint_Def,
								   CString strOwner1_Atk,
								   CString strOwner2_Atk,
								   CString strOwner1_Def,
								   CString strOwner2_Def,
								   WORD wKeep1_Atk,
								   WORD wKeep2_Atk,
								   WORD wKeep1_Def,
								   WORD wKeep2_Def )
{
	//!<---
	CalcTimer( dwLeftTick );


	//!<---
	FLOAT fPower_Atk = 100.0f - fPower_Def;

	TGauge* pGauge_Atk = static_cast<TGauge*>( m_pTGaugeBarAtk );
	TGauge* pGauge_Def = static_cast<TGauge*>( m_pTGaugeBarDef );
	pGauge_Atk->SetGauge( (int)fPower_Atk, 100 );
	pGauge_Def->SetGauge( (int)fPower_Def, 100 );
	m_pTGaugeAtk->m_strText = CTChart::Format( TSTR_FMT_SIEGE_ATK, fPower_Atk );
	m_pTGaugeDef->m_strText = CTChart::Format( TSTR_FMT_SIEGE_DEF, fPower_Def );


	//!<---
//	m_pTKillPointAtk->m_strText = CTChart::Format( TSTR_FMT_NUMBER, wKillPoint_Atk );
//	m_pTKillPointDef->m_strText = CTChart::Format( TSTR_FMT_NUMBER, wKillPoint_Def );
	m_pTKillPointAtk->m_strText.Format( "%06d", wKillPoint_Atk );
	m_pTKillPointDef->m_strText.Format( "%06d", wKillPoint_Def );

	//!<---
	CString strNone = CTChart::Format( TSTR_SIEGEBALL_NONE );
	m_pTBallCharAtk1->m_strText = CTChart::Format( TSTR_FMT_NAME, strOwner1_Atk.IsEmpty() ? strNone : strOwner1_Atk );
	m_pTBallCharAtk2->m_strText = CTChart::Format( TSTR_FMT_NAME, strOwner2_Atk.IsEmpty() ? strNone : strOwner2_Atk );
	m_pTBallCharDef1->m_strText = CTChart::Format( TSTR_FMT_NAME, strOwner1_Def.IsEmpty() ? strNone : strOwner1_Def );
	m_pTBallCharDef2->m_strText = CTChart::Format( TSTR_FMT_NAME, strOwner2_Def.IsEmpty() ? strNone : strOwner2_Def );

	WORD _minute = 0;
	WORD _second = 0;
	WORD color = 0;

	_get_hmsTime( (DWORD)wKeep1_Atk * 1000, NULL, &_minute, &_second );
	m_pTBallCharAtk1Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, _minute, _second );
	color = (WORD)(200 - ((float)(wKeep1_Atk / 300.0f) * 200.0f));
	m_pTBallCharAtk1Time->SetTextClr( D3DCOLOR_ARGB( 255, 200, color, color ) );

	_get_hmsTime( (DWORD)wKeep2_Atk * 1000, NULL, &_minute, &_second );
	m_pTBallCharAtk2Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, _minute, _second );
	color = (WORD)(200 - ((float)(wKeep2_Atk / 300.0f) * 200.0f));
	m_pTBallCharAtk2Time->SetTextClr( D3DCOLOR_ARGB( 255, 200, color, color ) );
	
	_get_hmsTime( (DWORD)wKeep1_Def * 1000, NULL, &_minute, &_second );
	m_pTBallCharDef1Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, _minute, _second );
	color = (WORD)(200 - ((float)(wKeep1_Def / 300.0f) * 200.0f));
	m_pTBallCharDef1Time->SetTextClr( D3DCOLOR_ARGB( 255, 200, color, color ) );

	_get_hmsTime( (DWORD)wKeep2_Def * 1000, NULL, &_minute, &_second );
	m_pTBallCharDef2Time->m_strText = CTChart::Format( TSTR_FMT_SIEGE_KEEPTIME, _minute, _second );
	color = (WORD)(200 - ((float)(wKeep2_Def / 300.0f) * 200.0f));
	m_pTBallCharDef2Time->SetTextClr( D3DCOLOR_ARGB( 255, 200, color, color ) );
}

BOOL CTSiegeUI::CanWithItemUI()
{
	return TRUE;
}

void CTSiegeUI::EndWar()
{
	InitSiegeUI();
	ShowComponent(FALSE);

	_set_strText( m_pTGaugeBarAtk );
	_set_strText( m_pTGaugeBarDef );
	_set_strText( m_pTGaugeAtk );
	_set_strText( m_pTGaugeDef );
	_set_strText( m_pTTimer );
	_set_strText( m_pTKillPointAtk );
	_set_strText( m_pTKillPointDef );
	_set_strText( m_pTBallCharAtk1 );
	_set_strText( m_pTBallCharAtk1Time );
	_set_strText( m_pTBallCharDef1 );
	_set_strText( m_pTBallCharDef1Time );
	_set_strText( m_pTBallCharAtk2 );
	_set_strText( m_pTBallCharAtk2Time );
	_set_strText( m_pTBallCharDef2 );
	_set_strText( m_pTBallCharDef2Time );
}

void CTSiegeUI::ShowSubComponent( BOOL bShow )
{
	m_bSubHide = !bShow;

	for( int i=0; i<TSIEGE_HIDE_COUNT; ++i )
	{
		m_pTHideComp[i]->EnableComponent( bShow );
		m_pTHideComp[i]->ShowComponent( bShow );
	}
}


