// QuestTeleport.h: interface for the CQuestTeleport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTTELEPORT_H__76703854_270B_4361_8565_C05E06FC8657__INCLUDED_)
#define AFX_QUESTTELEPORT_H__76703854_270B_4361_8565_C05E06FC8657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CQuestTeleport : public CQuest
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
	CQuestTeleport();
	virtual ~CQuestTeleport();
};

#endif // !defined(AFX_QUESTTELEPORT_H__76703854_270B_4361_8565_C05E06FC8657__INCLUDED_)
