#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CQuestDieMon::CQuestDieMon(void)
{
}

CQuestDieMon::~CQuestDieMon(void)
{
}

void CQuestDieMon::ExecQuest( CTPlayer *pPlayer,
							    DWORD dwTick,
							    LPMAPMAPVQUESTTEMP pTRIGGER,
								LPMAPQUESTTEMP pQUESTTEMP,
								LPMAPTITEMTEMP pITEMTEMP,
								LPMAPTSKILLTEMP pSKILLTEMP,
								LPMAPTMONSTERTEMP pMONTEMP,
								LPMAPCLASS pCLASS)
{
	if(!m_pMap)
		return;

	CTMonSpawn * pSPAWN = NULL;
	BYTE bLevel;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_SPAWNID )
			{
				WORD wSpawnID = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
				MAPTMONSPAWN::iterator it = m_pMap->m_mapTMONSPAWN.find(wSpawnID);
				if(it == m_pMap->m_mapTMONSPAWN.end())
					continue;

				pSPAWN = (*it).second;
				if(pSPAWN->m_pSPAWN->m_bEvent == SE_QUESTDEL)
				{
					for(DWORD i=0; i<pSPAWN->m_vTMON.size(); i++)
					{
						pSPAWN->m_vTMON[i]->m_bRemove = TRUE;

						if(pSPAWN->m_vTMON[i]->m_bStatus != OS_DEAD &&
							pSPAWN->m_vTMON[i]->m_bStatus != OS_DISAPPEAR)
							pSPAWN->m_vTMON[i]->OnDie(0,OT_NONE,0);
					}
				}
				else
				{
					for(DWORD i=0; i<pSPAWN->m_vTMON.size(); i++)
						if( pSPAWN->m_vTMON[i]->m_bStatus != OS_DEAD  &&
							pSPAWN->m_vTMON[i]->m_bStatus != OS_DISAPPEAR && 
							!pSPAWN->m_vTMON[i]->Transformation() )
						{
							WORD wPartyID = pPlayer->GetPartyID();
							__int64 nKey = wPartyID ? MAKEINT64( wPartyID, 0) : pPlayer->m_dwID;
							__int64 nValue = MAKEINT64( pPlayer->m_dwID, pSPAWN->m_vTMON[i]->m_dwHP);

							BYTE bType = wPartyID ? OWNER_PARTY : OWNER_PRIVATE;
							DWORD dwKeeper = wPartyID ? wPartyID : pPlayer->m_dwID;
							DWORD dwTotal = pSPAWN->m_vTMON[i]->m_dwHP;

							MAPINT64::iterator finder = pSPAWN->m_vTMON[i]->m_mapDamage.find(nKey);
							if(finder!=pSPAWN->m_vTMON[i]->m_mapDamage.end())
							{
								(*finder).second += pSPAWN->m_vTMON[i]->m_dwHP;
								dwTotal = DWORD((*finder).second);
								nValue = MAKEINT64( pPlayer->m_dwID, dwTotal);
								(*finder).second = nValue;
							}
							else
								pSPAWN->m_vTMON[i]->m_mapDamage.insert(MAPINT64::value_type( nKey, nValue));
							pSPAWN->m_vTMON[i]->SetKeeper( bType, dwKeeper);

							pSPAWN->m_vTMON[i]->OnDie(0,OT_NONE,0); 
						}
				}
			}

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
