#include "StdAfx.h"

CTAICmdGohome::CTAICmdGohome(void)
{
}

CTAICmdGohome::~CTAICmdGohome(void)
{
}

DWORD CTAICmdGohome::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdGohome::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_bMode != MT_GOHOME )
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdGohome::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( GetDistance(pMON->m_fStartX, pMON->m_fStartZ, pMON->m_fPosX, pMON->m_fPosZ) <= ZONE_HOMESIZE )
	{
		pMON->OnEvent(AT_ATHOME, 0, pMON->m_dwHostID, pMON->m_dwTargetID, pMON->m_bTargetType);
		return FALSE;
	}

	CTPlayer *pHOST = pMON->FindHost(pMON->m_dwHostID);

	for(DWORD i=0; i<pMON->m_vMaintainSkill.size();)
	{
		if(pMON->m_vMaintainSkill[i]->m_pTSKILL->IsBlockType())
			pMON->EraseMaintainSkill(pMON->m_vMaintainSkill[i], i);
		else
			i++;
	}

	if(pHOST)
	{
		pHOST->SendCS_MONACTION_ACK(
			pMON->m_dwID,
			TA_RUN,
			pMON->m_fStartX,
			pMON->m_fStartY,
			pMON->m_fStartZ);
	}
	pMON->m_dwHostKEY++;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
