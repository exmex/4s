// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__DF59B79F_8A5F_4294_A1A6_69559CED7EAB__INCLUDED_)
#define AFX_QUEST_H__DF59B79F_8A5F_4294_A1A6_69559CED7EAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CQuest
{
public:
	static CQuest *CreateQuest(
		LPQUESTTEMP pQUEST);

public:
	virtual LPQUESTTERM CheckComplete(
		CTPlayer *pPlayer,
		DWORD dwTick);

	virtual BYTE CheckComplete(
		CTPlayer *pPlayer,
		DWORD dwTick,
		LPQUESTTERM pTERM);

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
	LPQUESTTERM FindRunningTerm(
		DWORD dwTermID,
		BYTE bType);

	LPQUESTTERM FindTerm(
		DWORD dwTermID,
		BYTE bType);

public:
	LPQUESTTEMP m_pQUEST;
	VQUESTTERM m_vTERM;

	DWORD m_dwBeginTick;
	DWORD m_dwTick;

	BYTE m_bCompleteCount;
	BYTE m_bTriggerCount;

	CTMap *m_pMap;
	BYTE m_bSave; 

public:
	// CQuestComplete param
	DWORD m_dwRewardID;
	BYTE m_bRewardType;

	// CQuestDropItem param
	DWORD m_dwMonID;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

public:
	CQuest();
	virtual ~CQuest();
};

#endif // !defined(AFX_QUEST_H__DF59B79F_8A5F_4294_A1A6_69559CED7EAB__INCLUDED_)
