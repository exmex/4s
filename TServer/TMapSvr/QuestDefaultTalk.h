// QuestDefaultTalk.h: interface for the CQuestDefaultTalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDEFAULTTALK_H__B6342A8D_2F66_45B0_9DD7_BEB4ADB2113B__INCLUDED_)
#define AFX_QUESTDEFAULTTALK_H__B6342A8D_2F66_45B0_9DD7_BEB4ADB2113B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestDefaultTalk : public CQuest
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
	CQuestDefaultTalk();
	virtual ~CQuestDefaultTalk();
};

#endif // !defined(AFX_QUESTDEFAULTTALK_H__B6342A8D_2F66_45B0_9DD7_BEB4ADB2113B__INCLUDED_)
