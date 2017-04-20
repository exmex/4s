#include "StdAfx.h"

CQuestChapterMsg::CQuestChapterMsg(void)
{
}

CQuestChapterMsg::~CQuestChapterMsg(void)
{
}
void CQuestChapterMsg::ExecQuest( CTPlayer *pPlayer,
								   DWORD dwTick,
								   LPMAPMAPVQUESTTEMP pTRIGGER,
								   LPMAPQUESTTEMP pQUESTTEMP,
								   LPMAPTITEMTEMP pITEMTEMP,
								   LPMAPTSKILLTEMP pSKILLTEMP,
								   LPMAPTMONSTERTEMP pMONTEMP,
								   LPMAPCLASS pCLASS)
{
	pPlayer->SendCS_CHAPTERMSG_ACK(m_pQUEST->m_dwQuestID);
}
