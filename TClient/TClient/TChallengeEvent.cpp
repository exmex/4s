#include "stdafx.h"
#include "TChallengeEvent.h"

CTChallengeEvent::CTChallengeEvent(TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pMSG_UP = FindKid( ID_CTRLINST_MSG_UP );
	m_pGAUGE = (TGauge*) FindKid( ID_CTRLINST_GAUGE );
	m_pLIST = (TList*) FindKid( ID_CTRLINST_LIST );
	m_dwGaugeTick = 0;
}

CTChallengeEvent::~CTChallengeEvent(void)
{
}

HRESULT CTChallengeEvent::Render(DWORD dwTickCount)
{
	if( m_bVisible )
	{
		m_dwGaugeTick -= min(m_dwGaugeTick, dwTickCount);
		m_pGAUGE->SetGauge( m_dwGaugeTick, 60*1000, FALSE );
		m_pGAUGE->m_strText = CTChart::Format( TSTR_CHALLENGE_PROGRESS_TIME,
			(INT)(m_dwGaugeTick / 1000) );
	}
	return CTClientUIBase::Render( dwTickCount );
}