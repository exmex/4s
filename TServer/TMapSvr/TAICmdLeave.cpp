#include "StdAfx.h"


CTAICmdLeave::CTAICmdLeave()
{
}

CTAICmdLeave::~CTAICmdLeave()
{
}

BYTE CTAICmdLeave::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_DEAD  ||
		(pMON->m_pSPAWN->m_bStatus == MONSPAWN_SUSPEND &&
		!pMON->m_pMON->m_bRemove))
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdLeave::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( !pMON->m_bLeaveCount && pMON->m_dwInvenLock)
	{
		pMON->m_bLeaveCount++;
		return TRUE;
	}

	CTInven * pInven = pMON->FindTInven(INVEN_DEFAULT);
	if(pInven)
	{
		MAPTITEM::iterator it;
		for(it=pInven->m_mapTITEM.begin(); it!=pInven->m_mapTITEM.end(); )
		{
			CTItem * pItem = (*it).second;
			it++;

			if(pItem->m_bMaxRouting)
				return TRUE;
		}
	}

	MAPTITEM::iterator itList;
	for(itList=pMON->m_mapTItemList.begin(); itList!=pMON->m_mapTItemList.end(); itList++)
		delete (*itList).second;
	pMON->m_mapTItemList.clear();

	pMON->m_mapDamage.clear();
	pMON->m_mapAggro.clear();
	pMON->m_mapGuildDamage.clear(); 

	pMON->m_pMAP->LeaveMAP( pMON, TRUE);
	if(pMON->m_pSPAWN->m_pSPAWN->m_dwDelay &&
		!pMON->m_bRemove)
	{
		pMON->OnEvent( AT_DELETE, 0, 0, dwRHId, bRHType);
	}
	else
	{
		pMON->OnEvent( AT_TIMEOUT, 0, 0, dwRHId, bRHType);
		return FALSE;
	}

	pMON->m_bStatus = OS_DISAPPEAR;
	pMON->m_dwHostID = 0;
	pMON->m_dwTargetID = 0;
	pMON->m_bTargetType = 0;
	pMON->m_dwInvenLock = 0;

	if(!pMON->m_bEssential)
	{
		MAPTSKILL::iterator itSk;

		for(itSk=pMON->m_mapTSKILL.begin(); itSk!=pMON->m_mapTSKILL.end(); itSk++)
			delete (*itSk).second;

		pMON->m_vRemainSkill.clear();
		pMON->m_mapTSKILL.clear();
		pMON->m_pMON = NULL;
	}

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
