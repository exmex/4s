
#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

CQuestSendPost::CQuestSendPost(void)
{
}

CQuestSendPost::~CQuestSendPost(void)
{
}

void CQuestSendPost::ExecQuest( CTPlayer *pPlayer,
								   DWORD dwTick,
								   LPMAPMAPVQUESTTEMP pTRIGGER,
								   LPMAPQUESTTEMP pQUESTTEMP,
								   LPMAPTITEMTEMP pITEMTEMP,
								   LPMAPTSKILLTEMP pSKILLTEMP,
								   LPMAPTMONSTERTEMP pMONTEMP,
								   LPMAPCLASS pCLASS)
{		
	VTITEM vITEM;
	vITEM.clear();

	BYTE bLevel;
	BYTE bRun = FALSE;

	BYTE bRandTake = BYTE(rand() % 100);
	BYTE bRandSum = 0;	
	BYTE bSuccess = TRUE;
	
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{	
/*		switch( m_pQUEST->m_bTriggerType )
		{
		case TT_COMPLETE: bSuccess = TRUE; 			
			break;

		case TT_LEVELUP:
			if(m_pQUEST->m_dwTriggerID == pPlayer->m_bLevel)
				bSuccess = TRUE;				
			break;
		}
*/
		if(bSuccess)
		{
			for(BYTE i = 0; i < m_pQUEST->m_vReward.size(); i++)
			{
				if( m_pQUEST->m_vReward[i]->m_bRewardType != RT_ITEM &&
					m_pQUEST->m_vReward[i]->m_bRewardType != RT_MAGICITEM )
					continue;

				BYTE bTake = FALSE;

				switch(m_pQUEST->m_vReward[i]->m_bTakeMethod)
				{				
				case RM_PROB	: bTake = (rand() % 100) < m_pQUEST->m_vReward[i]->m_bTakeData ? TRUE : FALSE; break;
				case RM_RANDOM	:
					bRandSum += m_pQUEST->m_vReward[i]->m_bTakeData;
					if(bRandSum > bRandTake) bTake = TRUE;
					break;
				case RM_DEFAULT	: bTake = TRUE; break;
				}

				if(!bTake)
					continue;

				WORD wNewItemID = 0;

				if(m_pQUEST->m_vReward[i]->m_bRewardType == RT_MAGICITEM)
				{
					if(m_pQUEST->m_vReward[i]->m_pMagicItem)				
						wNewItemID = m_pQUEST->m_vReward[i]->m_pMagicItem->m_pTITEM->m_wItemID;

				}
				else
					wNewItemID = (WORD)m_pQUEST->m_vReward[i]->m_dwRewardID;

				MAPTITEMTEMP::iterator finder = pITEMTEMP->find(wNewItemID);

				if( finder != pITEMTEMP->end() &&
					((*finder).second->m_dwClassID & BITSHIFTID(pPlayer->m_bClass)))
				{
					int nCount = m_pQUEST->m_vReward[i]->m_bCount;

					while( nCount > 0 )
					{
						CTItem *pITEM = new CTItem();
						BYTE bTemp = nCount > (*finder).second->m_bStack ? (*finder).second->m_bStack : nCount;

						if(m_pQUEST->m_vReward[i]->m_bRewardType == RT_MAGICITEM)
						{
							if(m_pQUEST->m_vReward[i]->m_pMagicItem)
							{
								pITEM->Copy(m_pQUEST->m_vReward[i]->m_pMagicItem, TRUE);
								pITEM->SetEndTime(DWORD(pITEM->m_dEndTime) * DAY_ONE);
								pITEM->m_bCount = bTemp;
							}
						}
						else
						{						
							pITEM->m_pTITEM = (*finder).second;
							_AtlModule.SetItemAttr(pITEM, 0);
							pITEM->SetDuration(FALSE);
							pITEM->m_wItemID = pITEM->m_pTITEM->m_wItemID;
							pITEM->m_bCount = bTemp;
						}

						bRun = TRUE;
						vITEM.push_back(pITEM);
						nCount -= bTemp;
					}
				}
			}

			if(vITEM.size())
				_AtlModule.SendDM_QUESTSENDPOST_REQ(m_pQUEST->m_dwQuestID,pPlayer->m_dwID,&vITEM);

			while(!vITEM.empty())
			{
				delete vITEM.back();
				vITEM.pop_back();
			}
			vITEM.clear();

			if(bRun)			
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
};