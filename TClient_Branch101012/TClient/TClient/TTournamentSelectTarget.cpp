#include "stdafx.h"
#include "TTournamentSelectTarget.h"
#include "TClientGame.h"
#include "TMessengerCommander.h"
#include "TMessengerBase.h"
#include "TMsgGroupList.h"

#ifdef ADD_TOURNAMENT

#define TTST_ADD_PARTY 0
#define TTST_CHEERUP 1

CTTournamentSelectTarget::CTTournamentSelectTarget( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pLIST = (TList*) FindKid(ID_CTRLINST_LIST);
	m_pEDIT_NAME = (TEdit*) FindKid(ID_CTRLINST_EDIT_NAME);
	m_pBTN_GUILD = (TButton*) FindKid(ID_CTRLINST_BTN_GUILD);
    m_pBTN_FRIEND = (TButton*) FindKid(ID_CTRLINST_BTN_FRIEND);
	m_pBTN_ADD = (TButton*) FindKid(ID_CTRLINST_BTN_ADD);
	m_pLIST->AddString( "" );
	m_pEDIT_NAME->ClearText();
	m_pLIST->LastItemToFront();
	m_nMode = TTST_TAB_GUILD;
	m_nFuncMode = TTST_ADD_PARTY;
}

CTTournamentSelectTarget::~CTTournamentSelectTarget()
{
}

void CTTournamentSelectTarget::OnRecvGuildMemberList()
{
	CTGuildCommander* pTCMD = CTGuildCommander::GetInstance();
	CTGuildCommander::GuildMemberVec::iterator itGUILD, endGUILD;
	itGUILD = pTCMD->m_GuildMemberVec.begin();
	endGUILD = pTCMD->m_GuildMemberVec.end();
	for(; itGUILD != endGUILD ; ++itGUILD )
	{
		int nLine = m_pLIST->AddString( itGUILD->m_strName );
		CString strFMT;
		strFMT.Format( "%d", itGUILD->m_bLevel );
		m_pLIST->SetItemString( nLine, 1, strFMT );
		m_pLIST->SetItemString( nLine, 2, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[itGUILD->m_bClassID] ) );
	}

	m_bListening = FALSE;
}

void CTTournamentSelectTarget::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	if( msg == TNM_SEL_CHANGE )
	{
		if( from == m_pLIST->m_id )
		{
			MAP_LISTITEM* pItem = m_pLIST->GetCurSelItem();
			MAP_LISTITEM::iterator it = pItem->find( 0 );
			if( it != pItem->end() )
				m_pEDIT_NAME->SetText( it->second->m_strText );
			else
				m_pEDIT_NAME->ClearText();
		}
	}

	CTClientUIBase::OnNotify( from, msg, param );
}

TEdit* CTTournamentSelectTarget::GetCurEdit()
{
	if( IsVisible() && GetFocus() )
	{
		if( m_pEDIT_NAME->GetFocus() )
			return m_pEDIT_NAME;
	}
	return NULL;
}

void CTTournamentSelectTarget::SetAddPartyMode()
{
	m_nFuncMode = TTST_ADD_PARTY;
}

void CTTournamentSelectTarget::SetCheerUpMode()
{
	m_nFuncMode = TTST_CHEERUP;
}

void CTTournamentSelectTarget::OnGM_TOURNAMENT_SELECT_TARGET()
{
	CTClientSession* pSESSION = CTClientGame::GetInstance()->GetSession();

	switch( m_nFuncMode )
	{
	case TTST_ADD_PARTY:
		{
			if( m_pEDIT_NAME->m_strText.IsEmpty() )
				return ;

			pSESSION->SendCS_TOURNAMENTPARTYADD_REQ(
				m_pEDIT_NAME->m_strText );

			m_pEDIT_NAME->m_strText.Empty();
		}
		break;
	case TTST_CHEERUP:
		{
			int nSel = m_pLIST->GetCurSelIndex();
			if( nSel != -1 )
			{
				DWORD dwCharID = m_pLIST->GetItemData( nSel, 0 );
				pSESSION->SendCS_TOURNAMENTCHEER_REQ(
					dwCharID,
					m_bCheerItemInven,
					m_bCheerItemSlot );

				m_bCheerItemInven = 0;
				m_bCheerItemSlot = 0;
			}

			CTClientGame::GetInstance()->DisableUI( TFRAME_TOURNAMENT_SELECT_TARGET );
		}
		break;
	}
}

void CTTournamentSelectTarget::ChangeTab( INT nMode )
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	m_pLIST->RemoveAll();
	m_nMode = nMode;

	switch( m_nFuncMode )
	{
	case TTST_CHEERUP:
		{
			if( m_nMode )
			{
				if( pGame->m_pTLeftTeam )
				{
					for( INT i=0 ; i < pGame->m_pTLeftTeam->m_vMembers.size() ; ++i )
					{
						int nLine = m_pLIST->AddString(pGame->m_pTLeftTeam->m_vMembers[i]->m_strName);
						m_pLIST->SetItemData(nLine, 0, pGame->m_pTLeftTeam->m_vMembers[i]->m_dwCharID);
					}
				}
				m_pBTN_GUILD->Select((BOOL)FALSE);
				m_pBTN_FRIEND->Select((BOOL)TRUE);
			}
			else
			{
				if( pGame->m_pTRightTeam )
				{
					for( INT i=0 ; i < pGame->m_pTRightTeam->m_vMembers.size() ; ++i )
					{
						int nLine = m_pLIST->AddString(pGame->m_pTRightTeam->m_vMembers[i]->m_strName);
						m_pLIST->SetItemData(nLine, 0, pGame->m_pTRightTeam->m_vMembers[i]->m_dwCharID);
					}
				}
				m_pBTN_GUILD->Select((BOOL)TRUE);
				m_pBTN_FRIEND->Select((BOOL)FALSE);
			}
		}
		break;
	case TTST_ADD_PARTY:
		{
			if( m_nMode == TTST_TAB_FRIEND )
			{
				CTMessengerBase* pMsgBase = pGame->GetMessengerBase();
				CTMsgGroupList* pList = pMsgBase->GetMsgGroupList();

				CTMsgGroupList::CItemArray::iterator itGROUP, endGROUP;
				itGROUP = pList->m_ItemArray.begin();
				endGROUP = pList->m_ItemArray.end();
				for(; itGROUP != endGROUP ; ++itGROUP )
				{
					CTMsgGroupList::CGroup* pGROUP = (CTMsgGroupList::CGroup*) (*itGROUP);

					CTMsgGroupList::CItemArray::iterator itMEMBER, endMEMBER;
					itMEMBER = pGROUP->m_ItemArray.begin();
					endMEMBER = pGROUP->m_ItemArray.end();
					for(; itMEMBER != endMEMBER ; ++itMEMBER )
					{
						CTMsgGroupList::CMember* pMember = (CTMsgGroupList::CMember*) (*itMEMBER);
						int nLine = m_pLIST->AddString( pMember->m_vTxt[CTMsgGroupList::MTXT_NAME]->m_strText );
						m_pLIST->SetItemString( nLine, 1, pMember->m_vTxt[CTMsgGroupList::MTXT_LEVEL]->m_strText );
						m_pLIST->SetItemString( nLine, 2, pMember->m_vTxt[CTMsgGroupList::MTXT_JOB]->m_strText );
					}
				}

				m_pBTN_GUILD->Select((BOOL)TRUE);
				m_pBTN_FRIEND->Select((BOOL)FALSE);
			}
			else if( m_nMode == TTST_TAB_GUILD )
			{
				m_bListening = TRUE;
				pGame->GetSession()->SendCS_GUILDMEMBERLIST_REQ();		

				m_pBTN_GUILD->Select((BOOL)FALSE);
				m_pBTN_FRIEND->Select((BOOL)TRUE);
			}
		}
		break;
	}
}

void CTTournamentSelectTarget::OnEnableUI()
{
	m_pEDIT_NAME->ClearText();

	switch( m_nFuncMode )
	{
	case TTST_CHEERUP:
		{
			m_pBTN_GUILD->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_LEFT_TEAM);
			m_pBTN_FRIEND->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_RIGHT_TEAM);
			m_pBTN_ADD->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_USE);
			ChangeTab(rand()%2);
		}
		break;
	case TTST_ADD_PARTY:
		{
			m_pBTN_GUILD->m_strText = CTChart::LoadString(TSTR_REPRESENTATION_GUILD);
			m_pBTN_FRIEND->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_FRIEND);
			m_pBTN_ADD->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_ADD);
			ChangeTab(m_nMode);
		}
		break;
	};
}
#endif