// QuestComplete.cpp: implementation of the CQuestComplete class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestComplete::CQuestComplete()
{
}

CQuestComplete::~CQuestComplete()
{
}

void CQuestComplete::ExecQuest( CTPlayer *pPlayer,
							    DWORD dwTick,
							    LPMAPMAPVQUESTTEMP pTRIGGER,
								LPMAPQUESTTEMP pQUESTTEMP,
								LPMAPTITEMTEMP pITEMTEMP,
								LPMAPTSKILLTEMP pSKILLTEMP,
								LPMAPTMONSTERTEMP pMONTEMP,
								LPMAPCLASS pCLASS)
{
	BYTE bLevel;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		DWORD dwQuestID = 0;

		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_COMPQUEST )
			{
				dwQuestID = m_pQUEST->m_vTerm[i]->m_dwTermID;
				break;
			}

		if(pPlayer->IsRunningQuest(dwQuestID))
		{
			MAPQUEST::iterator finder = pPlayer->m_mapQUEST.find(dwQuestID);
			LPQUESTTERM pTERM = (*finder).second->CheckComplete(
				pPlayer,
				dwTick);

			if(!pTERM)
			{
				BYTE bResult = pPlayer->OnQuestComplete(
					pSKILLTEMP,
					pITEMTEMP,
					pCLASS,
					(*finder).second->m_pQUEST,
					m_bRewardType,
					m_dwRewardID);

				pPlayer->SendCS_QUESTCOMPLETE_ACK(
					bResult,
					dwQuestID,
					0, 0, 0);
				
				if( bResult == QR_SUCCESS )
				{
					(*finder).second->m_bCompleteCount++;
					(*finder).second->m_dwBeginTick = 0;
					(*finder).second->m_dwTick = 0;
					(*finder).second->m_bSave = TRUE;  

					for( int i=0; i<INT((*finder).second->m_vTERM.size()); i++)
						(*finder).second->m_vTERM[i]->m_bCount = 0;

					_AtlModule.CheckQuest(
						pPlayer,
						0,
						pPlayer->m_fPosX,
						pPlayer->m_fPosY,
						pPlayer->m_fPosZ,
						dwQuestID,
						QTT_QUESTCOMPLETED,
						TT_COMPLETE,
						1);

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
			else if(!m_pQUEST->m_bForceRun)
			{
				pPlayer->SendCS_QUESTCOMPLETE_ACK(
					QR_TERM,
					dwQuestID,
					pTERM->m_dwTermID,
					pTERM->m_bTermType, 0);
			}
		}
	}
}
