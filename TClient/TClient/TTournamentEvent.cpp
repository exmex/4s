#include "stdafx.h"
#include "TTournamentEvent.h"
#include "TClientGame.h"
#include "TTournament.h"

#ifdef ADD_TOURNAMENT

CTTournamentEvent::CTTournamentEvent( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_nMode(TTEVENT_SELECT_EVENT)
{
	m_pCOMMENT = FindKid( ID_CTRLINST_COMMENT );
	m_pCOMMENT2 = FindKid( ID_CTRLINST_COMMENT2 );
	m_pCOMMENT3 = FindKid( ID_CTRLINST_COMMENT3 );
	m_pEVENTLIST = (TList*) FindKid( ID_CTRLINST_EVENTLIST );
	m_pENTRYLIST = (TList*) FindKid( ID_CTRLINST_ENTRYLIST );
	m_pPARTYLIST = (TList*) FindKid( ID_CTRLINST_PARTYLIST );
	m_pBTN_CLOSE = (TButton*) FindKid( ID_CTRLINST_BTN_CLOSE );
	m_pBTN_PREV = (TButton*) FindKid( ID_CTRLINST_BTN_PREV );
	m_pBASE_ENTRYLIST = FindKid( ID_CTRLINST_BASE_ENTRYLIST );
	m_pBASE_PARTYLIST = FindKid( ID_CTRLINST_BASE_PARTYLIST );
	m_pENTRY_COL_1 = FindKid( ID_CTRLINST_ENTRY_COL_1 );
	m_pENTRY_COL_2 = FindKid( ID_CTRLINST_ENTRY_COL_2 );
	m_pENTRY_COL_3 = FindKid( ID_CTRLINST_ENTRY_COL_3 );
	m_pENTRY_COL_4 = FindKid( ID_CTRLINST_ENTRY_COL_4 );
	m_pENTRY_COL_5 = FindKid( ID_CTRLINST_ENTRY_COL_5 );
	m_pENTRY_COL_6 = FindKid( ID_CTRLINST_ENTRY_COL_6 );
	m_pENTRY_COL_7 = FindKid( ID_CTRLINST_ENTRY_COL_7 );
	m_pENTRY_COL_8 = FindKid( ID_CTRLINST_ENTRY_COL_8 );
	m_pEVENTLIST_MATCH= FindKid(ID_CTRLINST_EVENTLIST_MATCH);
	m_pEVENTLIST_TYPE= FindKid(ID_CTRLINST_EVENTLIST_TYPE);
	m_pEVENTLIST_MYCHOICE= FindKid(ID_CTRLINST_EVENTLIST_MYCHOICE);
	m_pEVENTLIST_COUNTRY= FindKid(ID_CTRLINST_EVENTLIST_COUNTRY);
	m_pEVENTLIST_S= FindKid(ID_CTRLINST_EVENTLIST_S);
	m_pEVENTLIST_A= FindKid(ID_CTRLINST_EVENTLIST_A);

	m_pEVENTLIST->LastItemToFront();
	m_pENTRYLIST->LastItemToFront();

	{
		CPoint ptTarget;
		TComponent* pPOS = FindKid(ID_CTRLINST_ENTRYLIST_POS);
		pPOS->GetComponentPos( &ptTarget );
		RemoveKid( pPOS );
		delete pPOS;

		CPoint ptBase, pt;
		m_pBASE_ENTRYLIST->GetComponentPos( &ptBase );
		ptTarget -= ptBase;

		m_pBASE_ENTRYLIST->GetComponentPos( &pt );
		m_pBASE_ENTRYLIST->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_1->GetComponentPos( &pt );
		m_pENTRY_COL_1->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_2->GetComponentPos( &pt );
		m_pENTRY_COL_2->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_3->GetComponentPos( &pt );
		m_pENTRY_COL_3->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_4->GetComponentPos( &pt );
		m_pENTRY_COL_4->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_5->GetComponentPos( &pt );
		m_pENTRY_COL_5->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_6->GetComponentPos( &pt );
		m_pENTRY_COL_6->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_7->GetComponentPos( &pt );
		m_pENTRY_COL_7->MoveComponent(pt+ptTarget);
		m_pENTRY_COL_8->GetComponentPos( &pt );
		m_pENTRY_COL_8->MoveComponent(pt+ptTarget);
		m_pENTRYLIST->GetComponentPos( &pt );
		m_pENTRYLIST->MoveComponent(pt+ptTarget);
	}

	m_pEVENTLIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pEVENTLIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pEVENTLIST->SetColumnAlign( 3, ALIGN_CENTER );
	m_pEVENTLIST->SetColumnAlign( 4, ALIGN_CENTER );
	m_pEVENTLIST->SetColumnAlign( 5, ALIGN_CENTER );
	
	m_pENTRYLIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 3, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 4, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 5, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 6, ALIGN_CENTER );
	m_pENTRYLIST->SetColumnAlign( 7, ALIGN_CENTER );

	m_pCOMMENT->GetComponentPos(&m_ptCOMMENT);
	m_pBTN_PREV->GetComponentPos(&m_ptBTN_PREV);
	m_pBTN_CLOSE->GetComponentPos(&m_ptBTN_CLOSE);
}

CTTournamentEvent::~CTTournamentEvent()
{
}

void CTTournamentEvent::SetInfo( BYTE bBase, DWORD dwSum )
{
	CString strMSG = CTChart::LoadString(TSTR_TOURNAMENT_TOTAL_PRIZE_MONEY);
	strMSG += CTClientGame::MakeMoneyStr(dwSum*TOURNAMENT_BASEPRIZE);
	m_pCOMMENT2->m_strText = strMSG;
	m_pCOMMENT3->m_strText = CTChart::Format( TSTR_TOURNAMENT_DEFAULT_PRIZE_MONEY, bBase );
}

void CTTournamentEvent::OnMatchDataReset()
{
	static DWORD ENTRY_TYPE[] =
	{
		0,
		TSTR_TOURNAMENT_PARTY,
		TSTR_TOURNAMENT_PRIVATE
	};

	m_pEVENTLIST->RemoveAll();
	OnListDataReset(NULL);

	VTTEVENTMATCH::iterator it, end;
	it = m_vMatch.begin();
	end = m_vMatch.end();

	CString strFMT;
	for(; it!=end ; ++it )
	{
		int nLine = m_pEVENTLIST->AddString( (*it)->m_strEntry );
		m_pEVENTLIST->SetItemData( nLine, 0, (DWORD) (*it) );

		m_pEVENTLIST->SetItemString( nLine, 1, ((*it)->m_bType != 0) ? CTChart::LoadString( (TSTRING) ENTRY_TYPE[(*it)->m_bType]) : _T("") );
		m_pEVENTLIST->SetItemString( nLine, 2, (*it)->m_strChoice );

		if( (*it)->m_bCountry != TCONTRY_N )
			m_pEVENTLIST->SetItemString( nLine, 3, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*it)->m_bCountry] ) );
		else
			m_pEVENTLIST->SetItemString( nLine, 3, _T("") );

		strFMT.Format( "%.2f", (*it)->m_fRate );
		m_pEVENTLIST->SetItemString( nLine, 4, strFMT );
		if( (*it)->m_dwAmount )
			m_pEVENTLIST->SetItemString( nLine, 5, CTClientGame::MakeMoneyStr((*it)->m_dwAmount) );
		else
			m_pEVENTLIST->SetItemString( nLine, 5, _T("") );
	}
}

void CTTournamentEvent::OnListDataReset( LPTTEVENTMATCH pMatch )
{
	m_pENTRYLIST->RemoveAll();
	m_pPARTYLIST->RemoveAll();
	
	if( pMatch )
	{
		VTTEVENTTARGET::iterator it, end;
		it = pMatch->m_vList.begin();
		end = pMatch->m_vList.end();

		CString strFMT;
		for(; it != end ; ++it )
		{
			int nLine = m_pENTRYLIST->AddString( (*it)->m_strName );
			m_pENTRYLIST->SetItemData( nLine, 0, (DWORD) (*it) );

			m_pENTRYLIST->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*it)->m_bCountry] ) );
			m_pENTRYLIST->SetItemString( nLine, 2, (*it)->m_strGuildName );

			strFMT.Format( "%d", (*it)->m_bLevel );
			m_pENTRYLIST->SetItemString( nLine, 3, strFMT );

			m_pENTRYLIST->SetItemString( nLine, 4, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[(*it)->m_bClass] ) );

			strFMT.Format( "%d", (*it)->m_dwRank );
			m_pENTRYLIST->SetItemString( nLine, 5, strFMT );

			strFMT.Format( "%d", (*it)->m_dwMonthRank );
			m_pENTRYLIST->SetItemString( nLine, 6, strFMT );

			strFMT.Format( "%.2f", (*it)->m_fRate );
			m_pENTRYLIST->SetItemString( nLine, 7, strFMT );
		}
	}
}

void CTTournamentEvent::OnPartyDataReset( LPTTEVENTTARGET pTarget )
{
	m_pPARTYLIST->RemoveAll();

	if( pTarget )
	{
		CString strFMT;
		VTTEVENTTARGETPARTY::iterator it, end;
		it = pTarget->m_vParty.begin();
		end = pTarget->m_vParty.end();
		for(; it != end ; ++it )
		{
			int nLine = m_pPARTYLIST->AddString( (*it)->m_strName );
			m_pPARTYLIST->SetItemData( nLine, 0, (DWORD) (*it) );

			m_pPARTYLIST->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*it)->m_bCountry] ) );
			m_pPARTYLIST->SetItemString( nLine, 2, (*it)->m_strGuildName );

			strFMT.Format( "%d", (*it)->m_bLevel );
			m_pPARTYLIST->SetItemString( nLine, 3, strFMT );

			m_pPARTYLIST->SetItemString( nLine, 4, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[(*it)->m_bClass] ) );

			strFMT.Format( "%d", (*it)->m_dwRank );
			m_pPARTYLIST->SetItemString( nLine, 5, strFMT );

			strFMT.Format( "%d", (*it)->m_dwMonthRank );
			m_pPARTYLIST->SetItemString( nLine, 6, strFMT );
		}
	}
}

LPTTEVENTMATCH CTTournamentEvent::GetEntry( BYTE bEntryID )
{
	VTTEVENTMATCH::iterator itMATCH, endMATCH;
	itMATCH = m_vMatch.begin();
	endMATCH = m_vMatch.end();

	for(; itMATCH != endMATCH ; ++itMATCH )
		if( (*itMATCH)->m_bEntryID == bEntryID )
			return (*itMATCH);

	return NULL;
}

void CTTournamentEvent::ClearMatch()
{
	VTTEVENTMATCH::iterator itMATCH, endMATCH;
	itMATCH = m_vMatch.begin();
	endMATCH = m_vMatch.end();

	for(; itMATCH != endMATCH ; ++itMATCH )
		delete (*itMATCH);

	m_vMatch.clear();
}

void CTTournamentEvent::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	if( msg == TNM_SEL_CHANGE )
	{
		if( from == m_pENTRYLIST->m_id )
		{
			INT nCur = m_pENTRYLIST->GetCurSelIndex();
			if( nCur != -1 )
			{
				LPTTEVENTTARGET pTarget = (LPTTEVENTTARGET) m_pENTRYLIST->GetItemData( nCur, 0 );
				OnPartyDataReset( pTarget );
			}
			else
				OnPartyDataReset( NULL );
		}
	}

	if( IsVisible() )
		ResetShow();

	CTClientUIBase::OnNotify( from, msg, param );
}

void CTTournamentEvent::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent( bVisible );
	ResetShow();
}

void CTTournamentEvent::ResetShow()
{
	switch( m_nMode )
	{
	case TTEVENT_SELECT_ENTRY:
		{
			m_pCOMMENT->ShowComponent(TRUE);
			m_pCOMMENT2->ShowComponent(TRUE);
			m_pCOMMENT3->ShowComponent(TRUE);
			m_pEVENTLIST->ShowComponent(FALSE);
			m_pENTRYLIST->ShowComponent(TRUE);
			m_pBTN_CLOSE->ShowComponent(TRUE);
			m_pBTN_PREV->ShowComponent(TRUE);
			m_pBASE_ENTRYLIST->ShowComponent(TRUE);
			m_pENTRY_COL_1->ShowComponent(TRUE);
			m_pENTRY_COL_2->ShowComponent(TRUE);
			m_pENTRY_COL_3->ShowComponent(TRUE);
			m_pENTRY_COL_4->ShowComponent(TRUE);
			m_pENTRY_COL_5->ShowComponent(TRUE);
			m_pENTRY_COL_6->ShowComponent(TRUE);
			m_pENTRY_COL_7->ShowComponent(TRUE);
			m_pENTRY_COL_8->ShowComponent(TRUE);
			m_pEVENTLIST_MATCH->ShowComponent(FALSE);
			m_pEVENTLIST_TYPE->ShowComponent(FALSE);
			m_pEVENTLIST_MYCHOICE->ShowComponent(FALSE);
			m_pEVENTLIST_COUNTRY->ShowComponent(FALSE);
			m_pEVENTLIST_S->ShowComponent(FALSE);
			m_pEVENTLIST_A->ShowComponent(FALSE);
			m_pBASE_PARTYLIST->ShowComponent(FALSE);
			m_pPARTYLIST->ShowComponent(FALSE);
			m_pCOMMENT->MoveComponent(m_ptCOMMENT);
			m_pBTN_PREV->MoveComponent(m_ptBTN_PREV);
			m_pBTN_CLOSE->MoveComponent(m_ptBTN_CLOSE);

			int nCur = m_pEVENTLIST->GetCurSelIndex();
			if( nCur != -1 )
			{
				LPTTEVENTMATCH pEVENT = (LPTTEVENTMATCH) m_pEVENTLIST->GetItemData(nCur,0);
				if( pEVENT->m_bType == 1)
				{
					m_pBASE_PARTYLIST->ShowComponent(TRUE);
					m_pPARTYLIST->ShowComponent(TRUE);

					CPoint pt;
					pt = m_ptCOMMENT;
					pt.y += 126;
					m_pCOMMENT->MoveComponent(pt);
					pt = m_ptBTN_PREV;
					pt.y += 126;
					m_pBTN_PREV->MoveComponent(pt);
					pt = m_ptBTN_CLOSE;
					pt.y += 126;
					m_pBTN_CLOSE->MoveComponent(pt);
				}
			}
		}
		break;
	case TTEVENT_SELECT_EVENT:
		{
			m_pCOMMENT->ShowComponent(TRUE);
			m_pCOMMENT2->ShowComponent(TRUE);
			m_pCOMMENT3->ShowComponent(TRUE);
			m_pEVENTLIST->ShowComponent(TRUE);
			m_pENTRYLIST->ShowComponent(FALSE);
			m_pPARTYLIST->ShowComponent(FALSE);
			CTTournament* pDLG = static_cast<CTTournament*>( CTClientGame::GetInstance()->GetFrame( TFRAME_TOURNAMENT ) );
			m_pBTN_CLOSE->ShowComponent(TRUE);
			m_pBTN_PREV->ShowComponent(FALSE);
			m_pBASE_ENTRYLIST->ShowComponent(FALSE);
			m_pBASE_PARTYLIST->ShowComponent(FALSE);
			m_pENTRY_COL_1->ShowComponent(FALSE);
			m_pENTRY_COL_2->ShowComponent(FALSE);
			m_pENTRY_COL_3->ShowComponent(FALSE);
			m_pENTRY_COL_4->ShowComponent(FALSE);
			m_pENTRY_COL_5->ShowComponent(FALSE);
			m_pENTRY_COL_6->ShowComponent(FALSE);
			m_pENTRY_COL_7->ShowComponent(FALSE);
			m_pENTRY_COL_8->ShowComponent(FALSE);
			m_pEVENTLIST_MATCH->ShowComponent(TRUE);
			m_pEVENTLIST_TYPE->ShowComponent(TRUE);
			m_pEVENTLIST_MYCHOICE->ShowComponent(TRUE);
			m_pEVENTLIST_COUNTRY->ShowComponent(TRUE);
			m_pEVENTLIST_S->ShowComponent(TRUE);
			m_pEVENTLIST_A->ShowComponent(TRUE);
			m_pCOMMENT->MoveComponent(m_ptCOMMENT);
			m_pBTN_PREV->MoveComponent(m_ptBTN_PREV);
			m_pBTN_CLOSE->MoveComponent(m_ptBTN_CLOSE);
		}
		break;
	}
}

void CTTournamentEvent::SetMode( INT nMode )
{
	m_nMode = nMode;
}

#endif