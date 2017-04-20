#include "StdAfx.h"


CTAICmdChgHost::CTAICmdChgHost()
{
}

CTAICmdChgHost::~CTAICmdChgHost()
{
}

DWORD CTAICmdChgHost::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdChgHost::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP )
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdChgHost::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTPlayer *pOLD = pMON->FindHost(pMON->m_dwHostID);
	CTPlayer *pNEW = pMON->FindHost(dwEventHost);

	if(!pNEW)
	{
		pMON->m_dwHostKEY++;
		if(!pMON->LeaveAggro(dwEventHost, dwRHId, bRHType))
		{
			pMON->ResetHost();
			pMON->OnEvent(AT_ENTER, 0,0,0,0);
		}
		return FALSE;
	}

	if(pMON->m_bCountry == pNEW->GetCountry())
		return FALSE;

	if(pNEW != pOLD )
	{
		pMON->m_dwHostKEY++;

		pMON->m_dwHostID = dwEventHost;
		pMON->NotifyHost(pOLD);
		pMON->SelectSkill(pNEW, TRUE);
	}

	pMON->m_dwTargetID = dwRHId;
	pMON->m_bTargetType = bRHType;
	pMON->AddAggro(dwEventHost, dwRHId, bRHType, pNEW->GetWarCountry(), 1);
	pMON->SetEventToFollower();

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
