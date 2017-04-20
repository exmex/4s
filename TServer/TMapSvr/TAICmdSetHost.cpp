#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CTAICmdSetHost::CTAICmdSetHost()
{
}

CTAICmdSetHost::~CTAICmdSetHost()
{
}

DWORD CTAICmdSetHost::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdSetHost::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bMode == MT_BATTLE ||
		pMON->m_bStatus == OS_DEAD)
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdSetHost::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRealHost, BYTE bRHType)
{
	CTPlayer *pHOST = NULL;
	DWORD dwDIST = 0xFFFFFFFF;

	VPLAYER vPLAYER;
	vPLAYER.clear();

	pMON->m_pMAP->GetNeighbor(
		&vPLAYER,
		pMON->m_fPosX,
		pMON->m_fPosZ);

	if(!vPLAYER.empty() &&
		vPLAYER[0]->CanHost(pMON->m_fPosX, pMON->m_fPosZ))
		pHOST = vPLAYER[0];

	for( int i=0; i<INT(vPLAYER.size()); i++)
	{
		if(!vPLAYER[i]->CanHost(pMON->m_fPosX, pMON->m_fPosZ))
			continue;

		DWORD dwLOCAL = DWORD(abs(pMON->m_fPosX - vPLAYER[i]->m_fPosX) + abs(pMON->m_fPosZ - vPLAYER[i]->m_fPosZ));
		if(dwLOCAL < dwDIST && _AtlModule.m_dwTick - vPLAYER[i]->m_dwMoveTick < 3000)
		{
			pHOST = vPLAYER[i];
			dwDIST = dwLOCAL;
		}
	}

	if( !pHOST &&
		!vPLAYER.empty() &&
		!vPLAYER[0]->m_bCanHost &&
		!vPLAYER[0]->m_bIsGhost)
	{
		pHOST = vPLAYER[0];
		pHOST->m_bCanHost = TRUE;
	}
	vPLAYER.clear();

	CTPlayer * pOLD = pMON->FindHost(pMON->m_dwHostID);

	if(!pHOST)
	{
		pMON->m_dwHostKEY++;
		pMON->ResetHost();
		return FALSE;
	}

	if(pOLD != pHOST)
	{
		pMON->m_dwHostKEY++;

		pMON->m_dwHostID = pHOST->m_dwID;
		pMON->m_bStatus = OS_WAKEUP;
		pMON->NotifyHost(pOLD);
		pMON->SelectSkill(pHOST, TRUE);
	}
	else
		return FALSE;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRealHost, bRHType);
}