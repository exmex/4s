#include "stdafx.h"
#include "TTournamentMode.h"
#include "TTournamentGage.h"
#include "TClientGame.h"

CTTournamentMode::CTTournamentMode( TComponent *pParent, LP_FRAMEDESC pDesc, CTTournamentMode::MODE mode)
: CTClientUIBase( pParent, pDesc ),
m_eMode(mode)
{
	m_pNAME1 = FindKid(ID_CTRLINST_NAME1);
	m_pNAME2 = FindKid(ID_CTRLINST_NAME2);
	m_pTIME = FindKid(ID_CTRLINST_TIME);
	m_dwRemainTime = 0;
	m_dwRemainMIN = 0;
	m_dwRemainSEC = 0;
	m_bRemainTime = FALSE;
	m_pGAUGE = (TGauge*) FindKid(ID_CTRLINST_GAUGE);
	// Arab Ver.
	m_pGAUGE->m_style = CTClientUIBase::GetGaugeDirection(TGS_GROW_RIGHT);
	m_pKILL1_COUNT = FindKid(ID_CTRLINST_KILL1_COUNT);
	m_pKILL2_COUNT = FindKid(ID_CTRLINST_KILL2_COUNT);
	m_pKILL1_COUNT->SetTextClr( D3DCOLOR_XRGB(254,208,0) );
	m_pKILL2_COUNT->SetTextClr( D3DCOLOR_XRGB(128,204,253) );
	m_pBtnShowPlayerInfo = (TButton*) FindKid(ID_CTRLINST_CHECK_SHOW_INFO);
	m_pTxtShowPlayerInfo = FindKid(ID_CTRLINST_TXT_SHOW_INFO);
}

CTTournamentMode::~CTTournamentMode()
{
}

void CTTournamentMode::Reset()
{
	m_pNAME1->m_strText.Empty();
	m_pNAME2->m_strText.Empty();
	m_pTIME->m_strText.Empty();
	m_pGAUGE->SetGauge(1,2,FALSE);
}

void CTTournamentMode::SetInfoTMode(
	LPTTOURNAMENTTEAM pTLeftTeam,
	LPTTOURNAMENTTEAM pTRightTeam,
	BOOL bResetOption,
	CString strInitTitle )
{
	if( !pTLeftTeam && !pTRightTeam )
	{
		m_pTIME->m_strText = strInitTitle;
		m_pNAME1->m_strText.Empty();
		m_pNAME2->m_strText.Empty();
		m_pKILL1_COUNT->m_strText.Empty();
		m_pKILL2_COUNT->m_strText.Empty();
		m_pGAUGE->SetGauge(1,2,FALSE);
		return ;
	}

	LPTTOURNAMENTPLAYER pLeftTeamChief = pTLeftTeam ? pTLeftTeam->GetChiefPlayer() : NULL;
	LPTTOURNAMENTPLAYER pRightTeamChief = pTRightTeam ? pTRightTeam->GetChiefPlayer() : NULL;

	DWORD dwKillCount1 = pTLeftTeam ? pTLeftTeam->m_wKillCount : 0;
	DWORD dwKillCount2 = pTRightTeam ? pTRightTeam->m_wKillCount : 0;
	m_pNAME1->m_strText = pLeftTeamChief ? pLeftTeamChief->m_strName : "";
	m_pNAME2->m_strText = pRightTeamChief ? pRightTeamChief->m_strName : "";
	m_pKILL1_COUNT->m_strText.Format("%d", dwKillCount1);
	m_pKILL2_COUNT->m_strText.Format("%d", dwKillCount2);

	if( dwKillCount1 == 0 && dwKillCount2 == 0)
	{
		dwKillCount1 = 1;
		dwKillCount2 = 1;
	}

	m_pGAUGE->SetGauge(dwKillCount1,dwKillCount1+dwKillCount2,FALSE);

	if( bResetOption )
	{
		m_pBtnShowPlayerInfo->Select( (BOOL)TRUE );

		CTTournamentGage* pDLG = static_cast<CTTournamentGage*>( CTClientGame::GetInstance()->GetFrame(TFRAME_TOURNAMENT_GAGE) );
		pDLG->m_bShowPlayerInfo = (BOOL)TRUE;
		pDLG->ResetTournamentINFO();
	}
}

HRESULT CTTournamentMode::Render( DWORD dwTickCount )
{
	if( m_bRemainTime )
	{
		DWORD dwPrevMIN = m_dwRemainMIN;
		DWORD dwPrevSEC = m_dwRemainSEC;

		m_dwRemainTime -= min(m_dwRemainTime, dwTickCount);

		if( m_dwRemainTime != 0 )
		{
			DWORD dwRemainTime = m_dwRemainTime / 1000;
			m_dwRemainMIN = dwRemainTime / 60;
			m_dwRemainSEC = dwRemainTime % 60;
		}
		else
		{
			m_dwRemainMIN = 0;
			m_dwRemainSEC = 0;
			m_bRemainTime = FALSE;
		}

		if( dwPrevMIN != m_dwRemainMIN ||
			dwPrevSEC != m_dwRemainSEC )
		{
			m_pTIME->m_strText = CTChart::Format(TSTR_TOURNAMENT_REMAIN_TIME,
				m_dwRemainMIN,
				m_dwRemainSEC);
		}
	}

	return CTClientUIBase::Render( dwTickCount );
}

void CTTournamentMode::ShowComponent(BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);
	if( bVisible && m_eMode == TM_BATTLE)
	{
		m_pBtnShowPlayerInfo->ShowComponent(FALSE);
		m_pTxtShowPlayerInfo->ShowComponent(FALSE);
	}
}
