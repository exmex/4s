#include "StdAfx.h"
#include "Resource.h"
#include "TClientGame.h"


CTNPCTalkDlg::CTNPCTalkDlg( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost, TCMLParser* pParser)
: CTNewQuestDlg( pParent, pDesc, pHost)
{
	m_pACCEPT->m_menu[TNM_LCLICK] = GM_QUEST_ACCEPT;
	m_pREFUSE->m_menu[TNM_LCLICK] = GM_CLOSE_UI;

	m_dwCompleteID = TSTR_COMPLETE;
	m_dwAcceptID = TSTR_ACCEPT;
	m_bPrintMSG = FALSE;

	m_vTDEFTALK.clear();
	m_bSND = FALSE;
	m_pTFollowDlg = NULL;
}

CTNPCTalkDlg::~CTNPCTalkDlg()
{
}

HRESULT CTNPCTalkDlg::Render( DWORD dwTickCount)
{
	if(!m_bVisible)
		return S_OK;

	LPTQUEST pTQUEST = GetCurSelTQuest();
	if( GetCurTQuest() != pTQUEST )
	{
		m_strNPCTalk = "";
		m_strAnswerWhenNPCTalk = "";

		BOOL bDEFTALK = TRUE;

		if(pTQUEST)
		{
			bDEFTALK = FALSE;

			if( pTQUEST->m_bType == QT_MISSION )
			{
				CTClientGame* pTGAME = CTClientGame::GetInstance(); 
				CTClientObjBase* pTARGET = pTGAME->GetTargetObj();
				if( pTARGET )
				{
					MAPDWORD::iterator findCOND = pTARGET->m_mapTQUESTCOND.find(pTQUEST->m_dwID);
					if( findCOND != pTARGET->m_mapTQUESTCOND.end() )
					{
						if( findCOND->second != QCT_NONE )
							bDEFTALK = TRUE;
					}
				}
			}
		}
		
		if(bDEFTALK)
			DefaultTalk();
		else
			Talk(pTQUEST->m_strTriggerMSG);
	}

	return CTNewQuestDlg::Render(dwTickCount);
}

void CTNPCTalkDlg::DefaultTalk()
{
	if(!m_vTDEFTALK.empty())
	{
		LPTQUEST pTALK = m_vTDEFTALK[rand() % INT(m_vTDEFTALK.size())];

		m_strNPCTitle = pTALK->m_strNPCName;
		Talk(pTALK->m_strTriggerMSG);
	}
}

void CTNPCTalkDlg::ShowComponent( BOOL bVisible)
{
	CTNewQuestDlg::ShowComponent(bVisible);
}

void CTNPCTalkDlg::Talk( CString strTALK)
{
	m_strNPCTalk = strTALK;
	m_strAnswerWhenNPCTalk.Empty();

	// NPC대화창이면
	m_pContents->RemoveAll();

	CString strHeader = GetSpeakerString(m_strNPCTitle);
	TextMessage( strHeader, m_strNPCTalk);
}

void CTNPCTalkDlg::Talk( CString strTALK, CString strANSWER)
{
	m_strNPCTalk = strTALK;
	m_strAnswerWhenNPCTalk = strANSWER;

	// NPC대화창이면
	m_pContents->RemoveAll();

	CString strHeader = GetSpeakerString(m_pHost->GetName());
	TextMessage( strHeader, m_strAnswerWhenNPCTalk);

	strHeader = GetSpeakerString(m_strNPCTitle);
	TextMessage( strHeader, m_strNPCTalk);
}

LPTQUEST CTNPCTalkDlg::GetCurSelTQuest()
{
	INT nIndex = GetSel();

	if( nIndex < 0 )
		return NULL;

	return GetTQUEST(nIndex);
}

LPTQUEST CTNPCTalkDlg::GetCurTQuest()
{
	return m_pTQUEST;
}

LPTQUEST CTNPCTalkDlg::GetCurTMission()
{
	if(!m_pTQUEST)
		return NULL;

	switch(m_pTQUEST->m_bType)
	{
	case QT_COMPLETE	: return CTChart::FindTMISSION(m_pTQUEST);
	case QT_MISSION		: return m_pTQUEST;
	}

	return NULL;
}

ITDetailInfoPtr CTNPCTalkDlg::GetTInfoKey( const CPoint& point )
{
	return CTNewQuestDlg::GetTInfoKey(point);
}
