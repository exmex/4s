#include "StdAfx.h"


CTAICmdFollow::CTAICmdFollow()
{
}

CTAICmdFollow::~CTAICmdFollow()
{
}

DWORD CTAICmdFollow::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdFollow::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_bMode != MT_BATTLE ||
		pMON->m_dwHostID != dwEventHost ||
		!pMON->m_dwTargetID ||
		pMON->m_pSPAWN->m_pSPAWN->m_bRoamType == 4 ||
		!pMON->CheckAttack()) //고정몬스터
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdFollow::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTPlayer *pHOST = pMON->FindHost(pMON->m_dwHostID);

	if(pHOST)
	{
		pHOST->SendCS_MONACTION_ACK(
			pMON->m_dwID,
			TA_FOLLOW,
			pHOST->m_fPosX,
			pHOST->m_fPosY,
			pHOST->m_fPosZ,
			pMON->m_dwTargetID,
			pMON->m_bTargetType);

		pMON->SelectSkill(pHOST, FALSE);
	}
	pMON->m_dwHostKEY++;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
