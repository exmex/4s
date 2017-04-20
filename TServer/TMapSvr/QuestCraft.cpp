#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CQuestCraft::CQuestCraft(void)
{
}

CQuestCraft::~CQuestCraft(void)
{
}


void CQuestCraft::ExecQuest( CTPlayer *pPlayer,
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
		DWORD dwQuestID = m_pQUEST->m_dwQuestID;

		LPQUESTTERM pTERM = CheckComplete(
				pPlayer,
				dwTick);

		if(!pTERM)
		{
			BYTE bResult = pPlayer->OnQuestComplete(
				pSKILLTEMP,
				pITEMTEMP,
				pCLASS,
				m_pQUEST,
				m_bRewardType,
				m_dwRewardID);

			pPlayer->SendCS_QUESTCOMPLETE_ACK(
				bResult,
				dwQuestID,
				0, 0, 0);

			if( bResult == QR_SUCCESS )
			{			
				for( int i=0; i<INT(m_vTERM.size()); i++)
					m_vTERM[i]->m_bCount = 0;

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