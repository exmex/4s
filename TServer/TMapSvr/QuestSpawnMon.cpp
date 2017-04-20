// QuestSpawnMon.cpp: implementation of the CQuestSpawnMon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestSpawnMon::CQuestSpawnMon()
{
}

CQuestSpawnMon::~CQuestSpawnMon()
{
}

void CQuestSpawnMon::ExecQuest( CTPlayer *pPlayer,
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

	BYTE bCanSpawn = FALSE;
	BYTE bLevel;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		WORD wSpawnID;
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_SPAWNID )
			{
				wSpawnID = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
				MAPTMONSPAWNTEMP::iterator it = _AtlModule.m_mapTMONSPAWN.find(wSpawnID);
				if(it!=_AtlModule.m_mapTMONSPAWN.end())
				{
					pSPAWN = (*it).second;
					if(pPlayer->m_pMAP->AddTimelimitedMon( pSPAWN, pPlayer->m_pMAP->m_bChannel, TCONTRY_N, m_pQUEST->m_vTerm[i]->m_bCount))
						bCanSpawn = TRUE;
				}
			}

		if(bCanSpawn)
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
