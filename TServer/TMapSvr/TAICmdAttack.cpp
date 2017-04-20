#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


CTAICmdAttack::CTAICmdAttack()
{
}

CTAICmdAttack::~CTAICmdAttack()
{
}

BYTE CTAICmdAttack::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_bMode != MT_BATTLE ||
		!pMON->m_dwTargetID ||
		!pMON->FindHost(pMON->m_dwHostID))
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdAttack::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if(	pMON->m_dwTargetID == dwRHId &&
		pMON->m_bTargetType == bRHType)
	{
		CTPlayer *pHOST = pMON->FindHost(pMON->m_dwHostID);
		if(pHOST &&	pMON->m_pNextSkill)
		{
			if( pMON->m_pNextSkill->CanUse(_AtlModule.m_dwTick) &&
				pMON->CanSilenceAttack(pMON->m_pNextSkill->m_pTSKILL->m_wID))
			{
				DWORD dwTargetID = 0;
				BYTE bTargetType = OT_NONE;

				if(pMON->m_pNextSkill->m_pTSKILL->m_bTargetRange)
				{
					dwTargetID = pMON->m_dwID;
					bTargetType = pMON->m_bType;
				}
				else
				{
					dwTargetID = pMON->m_dwTargetID;
					bTargetType = pMON->m_bTargetType;
				}

				pHOST->SendCS_MONATTACK_ACK(
					pMON->m_dwID,
					dwTargetID,
					pMON->m_bType,
					bTargetType,
					pMON->m_pNextSkill->m_pTSKILL->m_wID);

				pMON->SkillUse(pMON->m_pNextSkill, _AtlModule.m_dwTick);
			}
		}

		pMON->SelectSkill(pHOST, FALSE);
	}

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
