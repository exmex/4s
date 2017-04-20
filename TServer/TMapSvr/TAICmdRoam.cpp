#include "StdAfx.h"


CTAICmdRoam::CTAICmdRoam()
{
}

CTAICmdRoam::~CTAICmdRoam()
{
}

DWORD CTAICmdRoam::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if(!m_dwDelay)
		return 0;

	return m_dwDelay + rand() % (ROAM_DELAY_BOUND * m_dwDelay);
}

BYTE CTAICmdRoam::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_bMode == MT_BATTLE ||
		pMON->m_pMON->m_bIsSelf)
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdRoam::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTPlayer *pHOST = pMON->FindHost(pMON->m_dwHostID);

	BYTE bAction = pMON->m_bAction;
	FLOAT fPosX = pMON->m_fPosX;
	FLOAT fPosY = pMON->m_fPosY;
	FLOAT fPosZ = pMON->m_fPosZ;

	if(!pHOST)
	{
		pMON->m_dwHostKEY++;
		pMON->OnEvent(AT_LEAVE, 0, 0, 0, 0);
		return FALSE;
	}

	if(pMON->MoveNext(fPosX, fPosY, fPosZ, bAction, pHOST))
	{
		pMON->m_fNextX = fPosX;
		pMON->m_fNextY = fPosY;
		pMON->m_fNextZ = fPosZ;

		pHOST->SendCS_MONACTION_ACK(
			pMON->m_dwID,
			bAction,
			fPosX,
			fPosY,
			fPosZ);

		if(pMON->m_pSPAWN->m_dwLeader == pMON->m_dwID )
			pMON->MoveFollower(fPosX, fPosY, fPosZ);
	}

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
