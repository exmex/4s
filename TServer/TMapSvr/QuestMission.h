// QuestMission.h: interface for the CQuestMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTMISSION_H__56FE08DE_6970_4403_B13F_EBBFFAB1F14B__INCLUDED_)
#define AFX_QUESTMISSION_H__56FE08DE_6970_4403_B13F_EBBFFAB1F14B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestMission : public CQuest
{
public:
	virtual void ExecQuest(
		CTPlayer *pPlayer,
		DWORD dwTick,
		LPMAPMAPVQUESTTEMP pTRIGGER,
		LPMAPQUESTTEMP pQUESTTEMP,
		LPMAPTITEMTEMP pITEMTEMP,
		LPMAPTSKILLTEMP pSKILLTEMP,
		LPMAPTMONSTERTEMP pMONTEMP,
		LPMAPCLASS pCLASS);

public:
	CQuestMission();
	virtual ~CQuestMission();
};

#endif // !defined(AFX_QUESTMISSION_H__56FE08DE_6970_4403_B13F_EBBFFAB1F14B__INCLUDED_)
