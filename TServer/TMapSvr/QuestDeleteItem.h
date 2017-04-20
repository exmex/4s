#pragma once
#include "quest.h"

class CQuestDeleteItem : public CQuest
{
public:
	CQuestDeleteItem(void);
	~CQuestDeleteItem(void);

	virtual void ExecQuest(	
		CTPlayer *pPlayer,
		DWORD dwTick,
		LPMAPMAPVQUESTTEMP pTRIGGER,
		LPMAPQUESTTEMP pQUESTTEMP,
		LPMAPTITEMTEMP pITEMTEMP,
		LPMAPTSKILLTEMP pSKILLTEMP,
		LPMAPTMONSTERTEMP pMONTEMP,
		LPMAPCLASS pCLASS);
};
