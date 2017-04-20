#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CQuestRegen::CQuestRegen(void)
{
}

CQuestRegen::~CQuestRegen(void)
{
}
void CQuestRegen::ExecQuest( CTPlayer *pPlayer,
                             DWORD dwTick,
                             LPMAPMAPVQUESTTEMP pTRIGGER,
                             LPMAPQUESTTEMP pQUESTTEMP,
                             LPMAPTITEMTEMP pITEMTEMP,
                             LPMAPTSKILLTEMP pSKILLTEMP,
                             LPMAPTMONSTERTEMP pMONTEMP,
                             LPMAPCLASS pCLASS)
{
	if(!pPlayer->m_pMAP)
		return;

	LPTMONSPAWN pSPAWN = NULL;

	BYTE bRegen = FALSE;
	BYTE bLevel;

	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_MONID )
			{
				WORD wMonID = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
				WORD wSpawnID = _AtlModule.RegenDynamicMonster(pPlayer->m_pMAP->m_wMapID, wMonID, m_fPosX, m_fPosY, m_fPosZ, 0, m_pQUEST->m_vTerm[i]->m_bCount);
				if(wSpawnID)
				{
					MAPTMONSPAWNTEMP::iterator it = _AtlModule.m_mapTMONSPAWN.find(wSpawnID);
					if(it!=_AtlModule.m_mapTMONSPAWN.end())
					{
						pSPAWN = (*it).second;
						CTMonSpawn * pMonSpawn = pPlayer->m_pMAP->AddTimelimitedMon( pSPAWN, pPlayer->m_pMAP->m_bChannel, TCONTRY_N, RT_ETERNAL);
						if(pMonSpawn)
						{
							CTMonster * pMonster = pPlayer->m_pMAP->FindMonster(m_dwMonID);
							if(pMonster)
								pMonster->m_wRegenDelSpawn = pMonSpawn->m_pSPAWN->m_wID;

							bRegen = TRUE;
						}
					}
				}
			}

		if(bRegen)
			CQuest::ExecQuest(
				pPlayer,
				dwTick,
				pTRIGGER,
				pQUESTTEMP,
				pITEMTEMP,
				pSKILLTEMP,
				pMONTEMP,
				pCLASS);
	}
}