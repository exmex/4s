#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CTAICmdRemove::CTAICmdRemove(void)
{
}

CTAICmdRemove::~CTAICmdRemove(void)
{
}

BYTE CTAICmdRemove::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return TRUE;
}

DWORD CTAICmdRemove::GetDelay(CTMonster * pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdRemove::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	pMON->m_pMAP->LeaveMAP( pMON, TRUE);

	MAPTMONSTER::iterator itMon = pMON->m_pMAP->m_mapTMONSTER.find(pMON->m_dwID);
	if(itMon != pMON->m_pMAP->m_mapTMONSTER.end())
		pMON->m_pMAP->m_mapTMONSTER.erase(itMon);

	DWORD dwCount = DWORD(pMON->m_pSPAWN->m_vTMON.size());

	for(DWORD i=0; i<pMON->m_pSPAWN->m_vTMON.size(); i++)
	{
		if(pMON->m_pSPAWN->m_vTMON[i]->m_dwID == pMON->m_dwID)
		{
			pMON->m_pSPAWN->m_vTMON.erase(pMON->m_pSPAWN->m_vTMON.begin()+i);
			dwCount--;
			break;
		}
	}

	if(!dwCount && pMON->m_pSPAWN->m_pSPAWN->m_bEvent == SE_DYNAMIC)
	{
		_AtlModule.DelMonSpawn(pMON->m_pMAP, pMON->m_pSPAWN->m_pSPAWN->m_wID, TRUE);
	}
	else
		delete pMON;

	return FALSE;
}
