#include "StdAfx.h"


CTAICmdChkHost::CTAICmdChkHost()
{
}

CTAICmdChkHost::~CTAICmdChkHost()
{
}

DWORD CTAICmdChkHost::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdChkHost::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP)
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdChkHost::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTPlayer *pHOST = NULL;
	DWORD dwDIST = 0;

	CTPlayer * pOLD = pMON->FindHost(pMON->m_dwHostID);
	if(pOLD &&	pOLD->CanHost(pMON->m_fPosX, pMON->m_fPosZ))
		return FALSE;

	VPLAYER vPLAYER;
	vPLAYER.clear();

	pMON->m_pMAP->GetNeighbor(
		&vPLAYER,
		pMON->m_fPosX,
		pMON->m_fPosZ);

	for( int i=0; i<INT(vPLAYER.size()); i++)
	{
		if(!vPLAYER[i]->CanHost(pMON->m_fPosX, pMON->m_fPosZ))
			continue;

		DWORD dwLOCAL = DWORD(abs(pMON->m_fPosX - vPLAYER[i]->m_fPosX) + abs(pMON->m_fPosZ - vPLAYER[i]->m_fPosZ));

		if( !pHOST || dwLOCAL < dwDIST )
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
		pMON->NotifyHost(pOLD);
		pMON->SelectSkill(pHOST, TRUE);
	}
	else
		return FALSE;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
