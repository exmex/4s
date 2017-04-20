#pragma once
#include "quest.h"

class CQuestSwitch :
	public CQuest
{
public:
	CQuestSwitch(void);
	~CQuestSwitch(void);
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
