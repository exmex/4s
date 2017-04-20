#include "StdAfx.h"


CTAICmdRefill::CTAICmdRefill()
{
}

CTAICmdRefill::~CTAICmdRefill()
{
}

DWORD CTAICmdRefill::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdRefill::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_dwHostID != dwEventHost )
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdRefill::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if(!pMON->m_pMAP)
		return FALSE;

	pMON->m_dwHP = pMON->GetMaxHP();
	pMON->m_dwMP = pMON->GetMaxMP();

	VPLAYER vPLAYERS;
	vPLAYERS.clear();

	pMON->m_pMAP->GetNeighbor(
		&vPLAYERS,
		pMON->m_fPosX,
		pMON->m_fPosZ);

	while(!vPLAYERS.empty())
	{
		CTPlayer *pChar = vPLAYERS.back();

		pChar->SendCS_HPMP_ACK(
			pMON->m_dwID,
			pMON->m_bType,
			pMON->m_bLevel,
			pMON->GetMaxHP(),
			pMON->m_dwHP,
			pMON->GetMaxMP(),
			pMON->m_dwMP);

		vPLAYERS.pop_back();
	}

	vPLAYERS.clear();

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
