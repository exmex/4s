// QuestMission.cpp: implementation of the CQuestMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestMission::CQuestMission()
{
}

CQuestMission::~CQuestMission()
{
}

void CQuestMission::ExecQuest( CTPlayer *pPlayer,
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
		MAPQUEST::iterator finder = pPlayer->m_mapQUEST.find(m_pQUEST->m_dwQuestID);
		CQuest *pQuest = NULL;

		if( finder == pPlayer->m_mapQUEST.end() )
		{
			pQuest = CreateQuest(m_pQUEST);
			pPlayer->m_mapQUEST.insert( MAPQUEST::value_type( m_pQUEST->m_dwQuestID, pQuest));
		}
		else
			pQuest = (*finder).second;

		DWORD dwLevel = pPlayer->GetQuestLevel(m_pQUEST);
		if(dwLevel)
			pPlayer->m_mapLevelQuest.insert(MAPDWORD::value_type(dwLevel, m_pQUEST->m_dwQuestID));

		pPlayer->SendCS_QUESTADD_ACK(
			m_pQUEST->m_dwQuestID,
			m_pQUEST->m_bType);

		if(pQuest->m_bTriggerCount == 0xFF)
		{
			pQuest->m_bTriggerCount = 1;
			pQuest->m_bCompleteCount = 1;
		}

		pQuest->m_bTriggerCount++;
		pQuest->m_bSave = TRUE; 

		for(DWORD tc = 0; tc < pQuest->m_pQUEST->m_vTerm.size(); tc++)
			pPlayer->CheckQuest(
				dwTick,
				pQuest->m_pQUEST->m_vTerm[tc]->m_dwTermID,
				pQuest->m_pQUEST->m_vTerm[tc]->m_bTermType,
				0);

		for( INT i=0; i<INT(pQuest->m_pQUEST->m_vTerm.size()); i++)
			if( pQuest->m_pQUEST->m_vTerm[i]->m_bTermType == QTT_TIMER )
			{
				pQuest->m_dwTick = pQuest->m_pQUEST->m_vTerm[i]->m_dwTermID;
				pQuest->m_dwBeginTick = dwTick;

				pPlayer->SendCS_QUESTSTARTTIMER_ACK(
					pQuest->m_pQUEST->m_dwQuestID,
					pQuest->m_dwTick);
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
