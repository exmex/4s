#include "stdafx.h"
#include "TChallengeWaiting.h"
#include "TClientGame.h"
#include "TTournamentMode.h"

CTChallengeWaiting::CTChallengeWaiting(TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pOK = (TButton*) FindKid( ID_CTRLINST_BTN_OK );
	m_pMSG_UP = FindKid( ID_CTRLINST_MSG_UP );
	m_pMSG = FindKid( ID_CTRLINST_MSG );
	m_pGAUGE = (TGauge*) FindKid( ID_CTRLINST_GAUGE );
	m_pGAUGE->SetStyle( TGS_GROW_RIGHT );
	m_pMSG_UP->m_strText.Empty();
	m_pMSG->m_strText.Empty();
	m_dwNPC = 0;
	m_dwGaugeTick = 0;
	m_dwGaugeMax = 0;
	m_bWinCount = 0;
}

CTChallengeWaiting::~CTChallengeWaiting(void)
{
}

void CTChallengeWaiting::ShowComponent(BOOL bVisible)
{
	CTClientGame* pTGAME = CTClientGame::GetInstance();

	switch( pTGAME->m_bChallengeStatus )
	{
	case BS_NOBATTLE:
	case BS_NORMAL:
		{
			m_dwGaugeTick = 0;
			m_dwGaugeMax = 0;		

			CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
			if(	pMainChar->m_bChallengeTeam != TTOURNAMENT_TEAM_NONE )
			{
				m_pOK->m_strText = CTChart::LoadString( TSTR_CHALLENGE_GIVEUP_SHORT );
				m_pOK->m_menu[ TNM_LCLICK ] = GM_CHALLENGE_GIVEUP;
				m_pMSG_UP->m_strText = CTChart::Format( TSTR_CHALLENGE_WINNING,
					m_bWinCount );
				m_pMSG->m_strText = CTChart::LoadString( TSTR_CHALLENGE_NOT_STARTING );
				m_pGAUGE->m_strText.Empty();
			}
			else
			{
				m_pOK->m_strText = CTChart::LoadString( TSTR_CHALLENGE_BTN );
				m_pOK->m_menu[ TNM_LCLICK ] = GM_CHALLENGE_JOIN;
				m_pMSG_UP->m_strText.Empty();
				m_pMSG->m_strText = CTChart::LoadString( TSTR_CHALLENGE_GO );

				CTClientObjBase* pTARGET = pTGAME->GetTargetObj();
				if( pTARGET && pTARGET->m_bType == OT_NPC )
				{
					DWORD dwFee = 0;
					LPTARENA pTARENA = NULL;
					switch( ((CTClientNpc *) pTARGET)->m_dwExecID )
					{
					case GM_NPC_DUEL3: pTARENA = CTChart::FindTArena( 1 ); break;
					case GM_NPC_DUEL7: pTARENA = CTChart::FindTArena( 2 ); break;
					}

					if( pTARENA )
					{
						CString strFEE = CTClientGame::MakeMoneyStr( pTARENA->m_dwFee );
						m_pMSG_UP->m_strText = CTChart::Format( TSTR_CHALLENGE_TITLE_FEE, (LPCTSTR) strFEE );
					}
					else
					{
						m_pMSG_UP->m_strText.Empty();
					}
				}
			}
		}
		break;
	case BS_READY:
		{
			CTGaugePannel* pGP = static_cast<CTGaugePannel*>( CTClientGame::GetInstance()->GetFrame( TFRAME_GAUGE ) );
			m_dwGaugeTick = pGP->m_pTournamentInfo->m_dwRemainTime;
			m_dwGaugeMax = 60*1000;
			m_pMSG_UP->m_strText.Empty();
			m_pMSG->m_strText = CTChart::LoadString( TSTR_CHALLENGE_MSG_STANDBY );
		}
		break;
	case BS_BATTLE:
		{
			m_dwGaugeTick = 0;
			m_dwGaugeMax = 0;
			return CTClientUIBase::ShowComponent(FALSE);
		}
	}

	CTClientUIBase::ShowComponent( bVisible );

	if( pTGAME->m_bChallengeStatus == BS_READY )
		m_pOK->ShowComponent(FALSE);
}

HRESULT CTChallengeWaiting::Render(DWORD dwTickCount)
{
	if( m_dwGaugeTick != 0 )
		m_dwGaugeTick -= min(m_dwGaugeTick, dwTickCount);

	if( m_bVisible )
	{
		m_pGAUGE->SetGauge( m_dwGaugeTick, m_dwGaugeMax, FALSE );

		if( m_dwGaugeTick != 0 )
			m_pGAUGE->m_strText = CTChart::Format( TSTR_CHALLENGE_PROGRESS_TIME, (INT)(m_dwGaugeTick / 1000) );
		else
			m_pGAUGE->m_strText.Empty();
	}

	return CTClientUIBase::Render( dwTickCount );
}