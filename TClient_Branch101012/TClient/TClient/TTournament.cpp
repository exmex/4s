#include "stdafx.h"
#include "TTournament.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TClient.h"

static TSTRING STEP_TITLE[] =
{
	TSTR_NONE, //TNMTSTEP_READY
	TSTR_TOURNAMENT_STEP_1ST, //TNMTSTEP_1st
	TSTR_TOURNAMENT_STEP_NORMAL, //TNMTSTEP_NORMAL
	TSTR_TOURNAMENT_STEP_PARTY, //TNMTSTEP_PARTY
	TSTR_TOURNAMENT_STEP_MATCH, //TNMTSTEP_MATCH
	TSTR_TOURNAMENT_STEP_ENTER, //TNMTSTEP_ENTER
	TSTR_TOURNAMENT_STEP_QFINAL, //TNMTSTEP_QFINAL
	TSTR_TOURNAMENT_STEP_QFEND, //TNMTSTEP_QFEND
	TSTR_TOURNAMENT_STEP_SFENTER, //TNMTSTEP_SFENTER
	TSTR_TOURNAMENT_STEP_SFINAL, //TNMTSTEP_SFINAL
	TSTR_TOURNAMENT_STEP_SFEND, //TNMTSTEP_SFEND
	TSTR_TOURNAMENT_STEP_FENTER, //TNMTSTEP_FENTER
	TSTR_TOURNAMENT_STEP_FINAL, //TNMTSTEP_FINAL
	TSTR_TOURNAMENT_STEP_END //TNMTSTEP_END
};

#ifdef MODIFY_TT

DWORD CTTournament::SCHEDULE_COLOR[ TSCHEDULE_COLOR_COUNT ] =
{
	D3DCOLOR_XRGB( 217, 194, 152 ),
	D3DCOLOR_XRGB( 50, 100, 200 ),
	D3DCOLOR_XRGB( 16, 181, 28 ),
	D3DCOLOR_XRGB( 231, 199, 0 )
};

DWORD CTTournament::SCHEDULE_COLOR_DISABLE[ TSCHEDULE_COLOR_COUNT ] =
{
	D3DCOLOR_XRGB( 88, 68, 31),
	D3DCOLOR_XRGB( 33, 65, 129 ),
	D3DCOLOR_XRGB( 9, 98, 16 ),
	D3DCOLOR_XRGB( 148, 127, 0 )
};

void CTTournament::ScheduleGroup::AddSchedule( LPTTOURNAMENTSCHEDULE pSchedule )
{
	m_vSchedule.push_back( pSchedule );
}

void CTTournament::ScheduleGroup::AddScheduleFrom( ScheduleGroup* pOtherGroup )
{
	VTTOURNAMENTSCHEDULE::iterator it, end;
	it = pOtherGroup->m_vSchedule.begin();
	end = pOtherGroup->m_vSchedule.end();
	for(; it != end ; ++it )
	{
		TTOURNAMENTSCHEDULE* pSchedule = new TTOURNAMENTSCHEDULE;
		*pSchedule = *(*it);
		m_vSchedule.push_back( pSchedule );
	}
}

void CTTournament::ScheduleGroup::ClearSchedule()
{
	VTTOURNAMENTSCHEDULE::iterator it, end;
	it = m_vSchedule.begin();
	end = m_vSchedule.end();
	for(; it != end ; ++it )
		delete (*it);
	m_vSchedule.clear();
}

bool _compare_tournament_step(const LPTTOURNAMENTSCHEDULE& l, const LPTTOURNAMENTSCHEDULE& r)
{
	return l->m_bStep < r->m_bStep;
}

void CTTournament::ScheduleGroup::SortByStep()
{
	std::sort(
		m_vSchedule.begin(),
		m_vSchedule.end(),
		_compare_tournament_step);
}

#endif

CTTournament::CTTournament( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_nMode(TT_ENTRY)
#ifdef MODIFY_TT
,m_pCurrentGroup(NULL)
,m_bCurrentGroup(-1)
#else
,
m_bCurrentStep(-1)
#endif
{
	m_pTITLE = FindKid(ID_CTRLINST_TITLE);
	m_pCLOSE = FindKid(ID_CTRLINST_CLOSE);
	m_pMATCHLIST_COL_NAME = FindKid(ID_CTRLINST_MATCHLIST_COL_NAME);
	m_pMATCHLIST_COL_TYPE = FindKid(ID_CTRLINST_MATCHLIST_COL_TYPE);
	m_pMATCHLIST_COL_TARGET = FindKid(ID_CTRLINST_MATCHLIST_COL_TARGET);
	m_pMATCHLIST_COL_PROGRESS = FindKid(ID_CTRLINST_MATCHLIST_COL_PROGRESS);
	m_pMATCHLIST = (TList*) FindKid(ID_CTRLINST_MATCHLIST);
	m_pTXT_REWARD = FindKid(ID_CTRLINST_TXT_REWARD);
	m_pREWARD = (TList*) FindKid(ID_CTRLINST_REWARD);
	m_pSLOT2 = FindKid(ID_CTRLINST_SLOT2);
	m_pMONEY_BASE = FindKid(ID_CTRLINST_MONEY_BASE);
	m_pREWARD_RUNE = FindKid(ID_CTRLINST_REWARD_RUNE);
	m_pREWARD_LUNA = FindKid(ID_CTRLINST_REWARD_LUNA);
	m_pREWARD_CRON = FindKid(ID_CTRLINST_REWARD_CRON);
	m_pBTN_OK = FindKid(ID_CTRLINST_BTN_OK);
	m_pBTN_CANCEL = FindKid(ID_CTRLINST_BTN_CANCEL);
	m_pITEM[0] = (TImageList*) FindKid(ID_CTRLINST_ITEM);
	m_pITEM[1] = (TImageList*) FindKid(ID_CTRLINST_ITEM2);
	m_pENTRY_NOTICE = FindKid(ID_CTRLINST_ENTRY_NOTICE);
	m_pENTRY_COL_NAME = FindKid(ID_CTRLINST_ENTRY_COL_NAME);
	m_pENTRY_COL_COUNTRY = FindKid(ID_CTRLINST_ENTRY_COL_COUNTRY);
	m_pENTRY_COL_LEVEL = FindKid(ID_CTRLINST_ENTRY_COL_LEVEL);
	m_pENTRY_COL_JOB = FindKid(ID_CTRLINST_ENTRY_COL_JOB);
	m_pENTRY_COL_ACCRANK = FindKid(ID_CTRLINST_ENTRY_COL_ACCRANK);
	m_pENTRY_COL_MONRANK = FindKid(ID_CTRLINST_ENTRY_COL_MONRANK);
	m_pENTRY_PLAYER_LIST = (TList*) FindKid(ID_CTRLINST_ENTRY_PLAYER_LIST);
	m_pENTRY_CHARGE1 = FindKid(ID_CTRLINST_ENTRY_CHARGE1);
	m_pENTRY_CHARGE2 = FindKid(ID_CTRLINST_ENTRY_CHARGE2);
	m_pENTRY_CHARGE3 = FindKid(ID_CTRLINST_ENTRY_CHARGE3);
	m_pENTRY_CHARGE1_MONEYBASE = FindKid(ID_CTRLINST_ENTRY_CHARGE1_MONEYBASE);
	m_pENTRY_CHARGE2_MONEYBASE = FindKid(ID_CTRLINST_ENTRY_CHARGE2_MONEYBASE);
	m_pENTRY_CHARGE3_VALUE = FindKid(ID_CTRLINST_ENTRY_CHARGE3_VALUE);
	m_pENTRY_CHARGE1_RUNE = FindKid(ID_CTRLINST_ENTRY_CHARGE1_RUNE);
	m_pENTRY_CHARGE1_LUNA = FindKid(ID_CTRLINST_ENTRY_CHARGE1_LUNA);
	m_pENTRY_CHARGE1_CRON = FindKid(ID_CTRLINST_ENTRY_CHARGE1_CRON);
	m_pENTRY_CHARGE2_RUNE = FindKid(ID_CTRLINST_ENTRY_CHARGE2_RUNE);
	m_pENTRY_CHARGE2_LUNA = FindKid(ID_CTRLINST_ENTRY_CHARGE2_LUNA);
	m_pENTRY_CHARGE2_CRON = FindKid(ID_CTRLINST_ENTRY_CHARGE2_CRON);
	m_pENTRY_PARTY_LIST = (TList*) FindKid(ID_CTRLINST_ENTRY_PARTY_LIST);
	m_pMATCH_BACK = FindKid(ID_CTRLINST_MATCH_BACK);
	m_pMATCH_COL_ENTRY = FindKid(ID_CTRLINST_MATCH_COL_ENTRY);
	m_pMATCH_COL_QF = FindKid(ID_CTRLINST_MATCH_COL_QF);
	m_pMATCH_COL_SF = FindKid(ID_CTRLINST_MATCH_COL_SF);
	m_pMATCH_COL_FINAL = FindKid(ID_CTRLINST_MATCH_COL_FINAL);
	m_pMATCH_ENTRY[0] = FindKid(ID_CTRLINST_MATCH_ENTRY0);
	m_pMATCH_ENTRY[1] = FindKid(ID_CTRLINST_MATCH_ENTRY1);
	m_pMATCH_ENTRY[2] = FindKid(ID_CTRLINST_MATCH_ENTRY2);
	m_pMATCH_ENTRY[3] = FindKid(ID_CTRLINST_MATCH_ENTRY3);
	m_pMATCH_ENTRY[4] = FindKid(ID_CTRLINST_MATCH_ENTRY4);
	m_pMATCH_ENTRY[5] = FindKid(ID_CTRLINST_MATCH_ENTRY5);
	m_pMATCH_ENTRY[6] = FindKid(ID_CTRLINST_MATCH_ENTRY6);
	m_pMATCH_ENTRY[7] = FindKid(ID_CTRLINST_MATCH_ENTRY7);
	m_pMATCH_QF[0] = FindKid(ID_CTRLINST_MATCH_QF0);
	m_pMATCH_QF[1] = FindKid(ID_CTRLINST_MATCH_QF1);
	m_pMATCH_QF[2] = FindKid(ID_CTRLINST_MATCH_QF2);
	m_pMATCH_QF[3] = FindKid(ID_CTRLINST_MATCH_QF3);
	m_pMATCH_SF[0] = FindKid(ID_CTRLINST_MATCH_SF0);
	m_pMATCH_SF[1] = FindKid(ID_CTRLINST_MATCH_SF1);
	m_pMATCH_FINAL = FindKid(ID_CTRLINST_MATCH_FINAL);
	m_pMATCH_QF_UP[0] = FindKid(ID_CTRLINST_MATCH_QF0_UP);
	m_pMATCH_QF_UP_HL[0] = FindKid(ID_CTRLINST_MATCH_QF0_UP_HL);
	m_pMATCH_QF_DOWN[0] = FindKid(ID_CTRLINST_MATCH_QF0_DOWN);
	m_pMATCH_QF_DOWN_HL[0] = FindKid(ID_CTRLINST_MATCH_QF0_DOWN_HL);
	m_pMATCH_QF_UP[1] = FindKid(ID_CTRLINST_MATCH_QF1_UP);
	m_pMATCH_QF_UP_HL[1] = FindKid(ID_CTRLINST_MATCH_QF1_UP_HL);
	m_pMATCH_QF_DOWN[1] = FindKid(ID_CTRLINST_MATCH_QF1_DOWN);
	m_pMATCH_QF_DOWN_HL[1] = FindKid(ID_CTRLINST_MATCH_QF1_DOWN_HL);
	m_pMATCH_QF_UP[2] = FindKid(ID_CTRLINST_MATCH_QF2_UP);
	m_pMATCH_QF_UP_HL[2] = FindKid(ID_CTRLINST_MATCH_QF2_UP_HL);
	m_pMATCH_QF_DOWN[2] = FindKid(ID_CTRLINST_MATCH_QF2_DOWN);
	m_pMATCH_QF_DOWN_HL[2] = FindKid(ID_CTRLINST_MATCH_QF2_DOWN_HL);
	m_pMATCH_QF_UP[3] = FindKid(ID_CTRLINST_MATCH_QF3_UP);
	m_pMATCH_QF_UP_HL[3] = FindKid(ID_CTRLINST_MATCH_QF3_UP_HL);
	m_pMATCH_QF_DOWN[3] = FindKid(ID_CTRLINST_MATCH_QF3_DOWN);
	m_pMATCH_QF_DOWN_HL[3] = FindKid(ID_CTRLINST_MATCH_QF3_DOWN_HL);
	m_pMATCH_SF_UP[0] = FindKid(ID_CTRLINST_MATCH_SF0_UP);
	m_pMATCH_SF_UP_HL[0] = FindKid(ID_CTRLINST_MATCH_SF0_UP_HL);
	m_pMATCH_SF_DOWN[0] = FindKid(ID_CTRLINST_MATCH_SF0_DOWN);
	m_pMATCH_SF_DOWN_HL[0] = FindKid(ID_CTRLINST_MATCH_SF0_DOWN_HL);
	m_pMATCH_SF_UP[1] = FindKid(ID_CTRLINST_MATCH_SF1_UP);
	m_pMATCH_SF_UP_HL[1] = FindKid(ID_CTRLINST_MATCH_SF1_UP_HL);
	m_pMATCH_SF_DOWN[1] = FindKid(ID_CTRLINST_MATCH_SF1_DOWN);
	m_pMATCH_SF_DOWN_HL[1] = FindKid(ID_CTRLINST_MATCH_SF1_DOWN_HL);
	m_pMATCH_FINAL_UP = FindKid(ID_CTRLINST_MATCH_FINAL_UP);
	m_pMATCH_FINAL_UP_HL = FindKid(ID_CTRLINST_MATCH_FINAL_UP_HL);
	m_pMATCH_FINAL_DOWN = FindKid(ID_CTRLINST_MATCH_FINAL_DOWN);
	m_pMATCH_FINAL_DOWN_HL = FindKid(ID_CTRLINST_MATCH_FINAL_DOWN_HL);
	m_pMATCH_FINAL_HL = FindKid(ID_CTRLINST_MATCH_FINAL_HL);

	m_pMATCH_COL_SF->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_STEP_FINAL);
	m_pMATCH_COL_FINAL->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_WIN);

	{
		CPoint ptTarget;
		TComponent* pMATCH_POSITION = FindKid(ID_CTRLINST_MATCH_POSITION);
		pMATCH_POSITION->GetComponentPos( &ptTarget );
		RemoveKid( pMATCH_POSITION );
		delete pMATCH_POSITION;

		CPoint ptBase; 
		m_pMATCH_BACK->GetComponentPos( &ptBase );
		ptTarget -= ptBase;

		CPoint pt;
		m_pMATCH_BACK->GetComponentPos( &pt );
		m_pMATCH_BACK->MoveComponent(pt+ptTarget);
		m_pMATCH_COL_ENTRY->GetComponentPos( &pt );
		m_pMATCH_COL_ENTRY->MoveComponent(pt+ptTarget);
		m_pMATCH_COL_QF->GetComponentPos( &pt );
		m_pMATCH_COL_QF->MoveComponent(pt+ptTarget);
		m_pMATCH_COL_SF->GetComponentPos( &pt );
		m_pMATCH_COL_SF->MoveComponent(pt+ptTarget);
		m_pMATCH_COL_FINAL->GetComponentPos( &pt );
		m_pMATCH_COL_FINAL->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[0]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[1]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[2]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[3]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[4]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[4]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[5]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[5]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[6]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[6]->MoveComponent(pt+ptTarget);
		m_pMATCH_ENTRY[7]->GetComponentPos( &pt );
		m_pMATCH_ENTRY[7]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF[0]->GetComponentPos( &pt );
		m_pMATCH_QF[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF[1]->GetComponentPos( &pt );
		m_pMATCH_QF[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF[2]->GetComponentPos( &pt );
		m_pMATCH_QF[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF[3]->GetComponentPos( &pt );
		m_pMATCH_QF[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF[0]->GetComponentPos( &pt );
		m_pMATCH_SF[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF[1]->GetComponentPos( &pt );
		m_pMATCH_SF[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL->GetComponentPos( &pt );
		m_pMATCH_FINAL->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP[0]->GetComponentPos( &pt );
		m_pMATCH_QF_UP[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN[0]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP[1]->GetComponentPos( &pt );
		m_pMATCH_QF_UP[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN[1]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP[2]->GetComponentPos( &pt );
		m_pMATCH_QF_UP[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN[2]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP[3]->GetComponentPos( &pt );
		m_pMATCH_QF_UP[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN[3]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_UP[0]->GetComponentPos( &pt );
		m_pMATCH_SF_UP[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_DOWN[0]->GetComponentPos( &pt );
		m_pMATCH_SF_DOWN[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_UP[1]->GetComponentPos( &pt );
		m_pMATCH_SF_UP[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_DOWN[1]->GetComponentPos( &pt );
		m_pMATCH_SF_DOWN[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL_UP->GetComponentPos( &pt );
		m_pMATCH_FINAL_UP->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL_DOWN->GetComponentPos( &pt );
		m_pMATCH_FINAL_DOWN->MoveComponent(pt+ptTarget);

		m_pMATCH_QF_UP_HL[0]->GetComponentPos( &pt );
		m_pMATCH_QF_UP_HL[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN_HL[0]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN_HL[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP_HL[1]->GetComponentPos( &pt );
		m_pMATCH_QF_UP_HL[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN_HL[1]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN_HL[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP_HL[2]->GetComponentPos( &pt );
		m_pMATCH_QF_UP_HL[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN_HL[2]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN_HL[2]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_UP_HL[3]->GetComponentPos( &pt );
		m_pMATCH_QF_UP_HL[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_QF_DOWN_HL[3]->GetComponentPos( &pt );
		m_pMATCH_QF_DOWN_HL[3]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_UP_HL[0]->GetComponentPos( &pt );
		m_pMATCH_SF_UP_HL[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_DOWN_HL[0]->GetComponentPos( &pt );
		m_pMATCH_SF_DOWN_HL[0]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_UP_HL[1]->GetComponentPos( &pt );
		m_pMATCH_SF_UP_HL[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_SF_DOWN_HL[1]->GetComponentPos( &pt );
		m_pMATCH_SF_DOWN_HL[1]->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL_UP_HL->GetComponentPos( &pt );
		m_pMATCH_FINAL_UP_HL->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL_DOWN_HL->GetComponentPos( &pt );
		m_pMATCH_FINAL_DOWN_HL->MoveComponent(pt+ptTarget);
		m_pMATCH_FINAL_HL->GetComponentPos( &pt );
		m_pMATCH_FINAL_HL->MoveComponent(pt+ptTarget);
	}

	m_pSTEP_DATE[0] = FindKid(ID_CTRLINST_STEP_1_DATE);
	m_pSTEP_TITLE[0] = FindKid(ID_CTRLINST_STEP_1_TITLE);
	m_pSTEP_DATE[1] = FindKid(ID_CTRLINST_STEP_2_DATE);
	m_pSTEP_TITLE[1] = FindKid(ID_CTRLINST_STEP_2_TITLE);
	m_pSTEP_DATE[2] = FindKid(ID_CTRLINST_STEP_3_DATE);
	m_pSTEP_TITLE[2] = FindKid(ID_CTRLINST_STEP_3_TITLE);
	m_pSTEP_DATE[3] = FindKid(ID_CTRLINST_STEP_4_DATE);
	m_pSTEP_TITLE[3] = FindKid(ID_CTRLINST_STEP_4_TITLE);
	m_pSTEP_DATE[4] = FindKid(ID_CTRLINST_STEP_5_DATE);
	m_pSTEP_TITLE[4] = FindKid(ID_CTRLINST_STEP_5_TITLE);
	m_pSTEP_DATE[5] = FindKid(ID_CTRLINST_STEP_6_DATE);
	m_pSTEP_TITLE[5] = FindKid(ID_CTRLINST_STEP_6_TITLE);
	m_pSTEP_STICK[0] = NULL;
	m_pSTEP_STICK[1] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_STICK_1);
	m_pSTEP_STICK[2] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_STICK_2);
	m_pSTEP_STICK[3] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_STICK_3);
	m_pSTEP_STICK[4] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_STICK_4);
	m_pSTEP_STICK[5] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_STICK_5);
	m_pSTEP_BACK[0] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK1);
	m_pSTEP_BACK[1] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK2);
	m_pSTEP_BACK[2] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK3);
	m_pSTEP_BACK[3] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK4);
	m_pSTEP_BACK[4] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK5);
	m_pSTEP_BACK[5] = (TImageList*) FindKid(ID_CTRLINST_SCHEDULE_BACK6);
	for( INT i=0 ; i < 6 ; ++i )
		m_bSTEP[ i ] = FALSE;

	m_pMATCHLIST->LastItemToFront();
	m_pENTRY_PLAYER_LIST->LastItemToFront();
	m_pENTRY_PARTY_LIST->LastItemToFront();

	m_pBTN_ADD_PARTY = (TButton*)FindKid(ID_CTRLINST_BTN_ADD_PARTY);
	m_pBTN_ADD_PARTY->MoveComponent( CPoint(360,194) );

	m_pBTN_REMOVE_PARTY[0] = (TButton*)FindKid(ID_CTRLINST_BTN_REMOVE_PARTY);
	m_pBTN_REMOVE_PARTY[0]->MoveComponent( CPoint(360,368) );
	m_pBTN_REMOVE_PARTY[0]->m_menu[TNM_LCLICK].m_dwID = GM_TOURNAMENT_REMOVE_PARTY;
	m_pBTN_REMOVE_PARTY[0]->m_menu[TNM_LCLICK].m_dwParam = 0;

	for( INT i=1 ; i < 6 ; ++i )
	{
		m_pBTN_REMOVE_PARTY[ i ] = new TButton( this, m_pBTN_REMOVE_PARTY[0]->m_pDESC );
		m_pBTN_REMOVE_PARTY[ i ]->m_id = GetUniqueID();
		m_pBTN_REMOVE_PARTY[ i ]->MoveComponent( CPoint(360,368+i*17));
		m_pBTN_REMOVE_PARTY[ i ]->m_menu[TNM_LCLICK].m_dwID = GM_TOURNAMENT_REMOVE_PARTY;
		m_pBTN_REMOVE_PARTY[ i ]->m_menu[TNM_LCLICK].m_dwParam = i;
		AddKid( m_pBTN_REMOVE_PARTY[ i ] );
	}

	for( INT i=0 ; i < 4 ; ++i )
	{
		m_bMATCH_QF_UP[i] = FALSE;
		m_bMATCH_QF_DOWN[i] = FALSE;
	}

	for( INT i=0 ; i < 2 ; ++i )
	{
		m_bMATCH_SF_UP[i] = FALSE;
		m_bMATCH_SF_DOWN[i] = FALSE;
	}

	m_bMATCH_FINAL_UP = FALSE;
	m_bMATCH_FINAL_DOWN = FALSE;
	m_bMATCH_FINAL = FALSE;
	m_bShowPartyBtn = FALSE;
	m_bShowJoinBtn = FALSE;

	m_pMATCHLIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pMATCHLIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pMATCHLIST->SetColumnAlign( 3, ALIGN_CENTER );

	m_pENTRY_PLAYER_LIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pENTRY_PLAYER_LIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pENTRY_PLAYER_LIST->SetColumnAlign( 3, ALIGN_CENTER );
	m_pENTRY_PLAYER_LIST->SetColumnAlign( 4, ALIGN_CENTER );
	m_pENTRY_PLAYER_LIST->SetColumnAlign( 5, ALIGN_CENTER );

	m_pENTRY_PARTY_LIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pENTRY_PARTY_LIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pENTRY_PARTY_LIST->SetColumnAlign( 3, ALIGN_CENTER );
	m_pENTRY_PARTY_LIST->SetColumnAlign( 4, ALIGN_CENTER );
	m_pENTRY_PARTY_LIST->SetColumnAlign( 5, ALIGN_CENTER );
#ifdef MODIFY_TT
	m_pMATCHLIST->ApplyUserColor(TRUE);
	m_nPrevSelectedMatch = 0;
#else
	m_nPrevSelectedMatch = -1;
#endif

	for( INT i=0 ; i < 8 ; ++i )
		m_pMATCH_ENTRY_INFO[ i ] = NULL;
	for( INT i=0 ; i < 4 ; ++i )
		m_pMATCH_QF_INFO[ i ] = NULL;
	for( INT i=0 ; i < 2 ; ++i )
		m_pMATCH_SF_INFO[ i ] = NULL;

	m_pMATCH_FINAL_INFO = NULL;
}

CTTournament::~CTTournament()
{
}

void CTTournament::ShowComponent( BOOL bVisible )
{
	CTClientUIBase::ShowComponent( bVisible );

	if( bVisible )
	{
		CalcScheduleTop();
		UpdateSchedule();

		for( int i=0 ; i < 6 ; ++i )
			if( m_pSTEP_STICK[ i ] )
				m_pSTEP_STICK[ i ]->ShowComponent( m_bSTEP[ i ] );

		switch( m_nMode )
		{
		case TT_ENTRY:
			{
				m_pENTRY_NOTICE->ShowComponent(TRUE);
				m_pENTRY_COL_NAME->ShowComponent(TRUE);
				m_pENTRY_COL_COUNTRY->ShowComponent(TRUE);
				m_pENTRY_COL_LEVEL->ShowComponent(TRUE);
				m_pENTRY_COL_JOB->ShowComponent(TRUE);
				m_pENTRY_COL_ACCRANK->ShowComponent(TRUE);
				m_pENTRY_COL_MONRANK->ShowComponent(TRUE);
				m_pENTRY_PLAYER_LIST->ShowComponent(TRUE);
				m_pENTRY_CHARGE1->ShowComponent(TRUE);
				m_pENTRY_CHARGE2->ShowComponent(TRUE);
				m_pENTRY_CHARGE3->ShowComponent(TRUE);
				m_pENTRY_CHARGE1_MONEYBASE->ShowComponent(TRUE);
				m_pENTRY_CHARGE2_MONEYBASE->ShowComponent(TRUE);
				m_pENTRY_CHARGE3_VALUE->ShowComponent(TRUE);
				m_pENTRY_CHARGE1_RUNE->ShowComponent(TRUE);
				m_pENTRY_CHARGE1_LUNA->ShowComponent(TRUE);
				m_pENTRY_CHARGE1_CRON->ShowComponent(TRUE);
				m_pENTRY_CHARGE2_RUNE->ShowComponent(TRUE);
				m_pENTRY_CHARGE2_LUNA->ShowComponent(TRUE);
				m_pENTRY_CHARGE2_CRON->ShowComponent(TRUE);
				m_pENTRY_PARTY_LIST->ShowComponent(FALSE);
				m_pMATCH_BACK->ShowComponent(FALSE);
				m_pMATCH_COL_ENTRY->ShowComponent(FALSE);
				m_pMATCH_COL_QF->ShowComponent(FALSE);
				m_pMATCH_COL_SF->ShowComponent(FALSE);
				m_pMATCH_COL_FINAL->ShowComponent(FALSE);
				m_pMATCH_ENTRY[0]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[1]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[2]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[3]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[4]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[5]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[6]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[7]->ShowComponent(FALSE);
				m_pMATCH_QF[0]->ShowComponent(FALSE);
				m_pMATCH_QF[1]->ShowComponent(FALSE);
				m_pMATCH_QF[2]->ShowComponent(FALSE);
				m_pMATCH_QF[3]->ShowComponent(FALSE);
				m_pMATCH_SF[0]->ShowComponent(FALSE);
				m_pMATCH_SF[1]->ShowComponent(FALSE);
				m_pMATCH_FINAL->ShowComponent(FALSE);
				for( INT i=0 ; i < 4 ; ++i )
				{
					m_pMATCH_QF_UP[i]->ShowComponent( FALSE );
					m_pMATCH_QF_DOWN[i]->ShowComponent( FALSE );
					m_pMATCH_QF_UP_HL[i]->ShowComponent( FALSE );
					m_pMATCH_QF_DOWN_HL[i]->ShowComponent( FALSE );
				}

				for( INT i=0 ; i < 2 ; ++i )
				{
					m_pMATCH_SF_UP[i]->ShowComponent( FALSE );
					m_pMATCH_SF_DOWN[i]->ShowComponent( FALSE );
					m_pMATCH_SF_UP_HL[i]->ShowComponent( FALSE );
					m_pMATCH_SF_DOWN_HL[i]->ShowComponent( FALSE );
				}

				m_pMATCH_FINAL_UP->ShowComponent( FALSE );
				m_pMATCH_FINAL_UP_HL->ShowComponent( FALSE );
				m_pMATCH_FINAL_DOWN->ShowComponent( FALSE );
				m_pMATCH_FINAL_DOWN_HL->ShowComponent( FALSE );
				m_pMATCH_FINAL_HL->ShowComponent( FALSE );

#ifdef MODIFY_TT
				if( m_pCurrentGroup && m_pCurrentGroup->m_pCurrentStep &&
					(m_pCurrentGroup->m_pCurrentStep->m_bStep == TNMTSTEP_1st ||
					m_pCurrentGroup->m_pCurrentStep->m_bStep == TNMTSTEP_NORMAL) )
					m_pBTN_OK->ShowComponent(m_bShowJoinBtn);
				else
					m_pBTN_OK->ShowComponent(FALSE);
#else
				if( m_pCurrentStep && (m_pCurrentStep->m_bStep == TNMTSTEP_1st || m_pCurrentStep->m_bStep == TNMTSTEP_NORMAL ) )
					m_pBTN_OK->ShowComponent(m_bShowJoinBtn);
				else
					m_pBTN_OK->ShowComponent(FALSE);
#endif

				m_pBTN_OK->m_strText = CTChart::LoadString(TSTR_TOURNAMENT_JOIN);
				m_pBTN_CANCEL->ShowComponent(TRUE);
				m_pBTN_CANCEL->m_strText = CTChart::LoadString(TSTR_CLOSE);

				m_pBTN_ADD_PARTY->ShowComponent( FALSE );
				for( INT i=0 ; i < 6 ; ++i )
					m_pBTN_REMOVE_PARTY[ i ]->ShowComponent( FALSE );
			}
			break;

		case TT_JOIN:
			{
				m_pENTRY_NOTICE->ShowComponent(FALSE);
				m_pENTRY_COL_NAME->ShowComponent(TRUE);
				m_pENTRY_COL_COUNTRY->ShowComponent(TRUE);
				m_pENTRY_COL_LEVEL->ShowComponent(TRUE);
				m_pENTRY_COL_JOB->ShowComponent(TRUE);
				m_pENTRY_COL_ACCRANK->ShowComponent(TRUE);
				m_pENTRY_COL_MONRANK->ShowComponent(TRUE);
				m_pENTRY_PLAYER_LIST->ShowComponent(TRUE);
				m_pENTRY_CHARGE1->ShowComponent(FALSE);
				m_pENTRY_CHARGE2->ShowComponent(FALSE);
				m_pENTRY_CHARGE3->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_MONEYBASE->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_MONEYBASE->ShowComponent(FALSE);
				m_pENTRY_CHARGE3_VALUE->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_RUNE->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_LUNA->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_CRON->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_RUNE->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_LUNA->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_CRON->ShowComponent(FALSE);
				m_pENTRY_PARTY_LIST->ShowComponent(TRUE);
				m_pMATCH_BACK->ShowComponent(FALSE);
				m_pMATCH_COL_ENTRY->ShowComponent(FALSE);
				m_pMATCH_COL_QF->ShowComponent(FALSE);
				m_pMATCH_COL_SF->ShowComponent(FALSE);
				m_pMATCH_COL_FINAL->ShowComponent(FALSE);
				m_pMATCH_ENTRY[0]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[1]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[2]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[3]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[4]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[5]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[6]->ShowComponent(FALSE);
				m_pMATCH_ENTRY[7]->ShowComponent(FALSE);
				m_pMATCH_QF[0]->ShowComponent(FALSE);
				m_pMATCH_QF[1]->ShowComponent(FALSE);
				m_pMATCH_QF[2]->ShowComponent(FALSE);
				m_pMATCH_QF[3]->ShowComponent(FALSE);
				m_pMATCH_SF[0]->ShowComponent(FALSE);
				m_pMATCH_SF[1]->ShowComponent(FALSE);
				m_pMATCH_FINAL->ShowComponent(FALSE);

				for( INT i=0 ; i < 4 ; ++i )
				{
					m_pMATCH_QF_UP[i]->ShowComponent( FALSE );
					m_pMATCH_QF_DOWN[i]->ShowComponent( FALSE );
					m_pMATCH_QF_UP_HL[i]->ShowComponent( FALSE );
					m_pMATCH_QF_DOWN_HL[i]->ShowComponent( FALSE );
				}

				for( INT i=0 ; i < 2 ; ++i )
				{
					m_pMATCH_SF_UP[i]->ShowComponent( FALSE );
					m_pMATCH_SF_DOWN[i]->ShowComponent( FALSE );
					m_pMATCH_SF_UP_HL[i]->ShowComponent( FALSE );
					m_pMATCH_SF_DOWN_HL[i]->ShowComponent( FALSE );
				}

				m_pMATCH_FINAL_UP->ShowComponent( FALSE );
				m_pMATCH_FINAL_UP_HL->ShowComponent( FALSE );
				m_pMATCH_FINAL_DOWN->ShowComponent( FALSE );
				m_pMATCH_FINAL_DOWN_HL->ShowComponent( FALSE );
				m_pMATCH_FINAL_HL->ShowComponent( FALSE );

				m_pBTN_OK->ShowComponent(FALSE);
				m_pBTN_CANCEL->ShowComponent(TRUE);
				m_pBTN_CANCEL->m_strText = CTChart::LoadString(TSTR_CLOSE);

				ResetParty();
			}
			break;

		case TT_MATCH:
			{
				m_pENTRY_NOTICE->ShowComponent(FALSE);
				m_pENTRY_COL_NAME->ShowComponent(FALSE);
				m_pENTRY_COL_COUNTRY->ShowComponent(FALSE);
				m_pENTRY_COL_LEVEL->ShowComponent(FALSE);
				m_pENTRY_COL_JOB->ShowComponent(FALSE);
				m_pENTRY_COL_ACCRANK->ShowComponent(FALSE);
				m_pENTRY_COL_MONRANK->ShowComponent(FALSE);
				m_pENTRY_PLAYER_LIST->ShowComponent(FALSE);
				m_pENTRY_CHARGE1->ShowComponent(FALSE);
				m_pENTRY_CHARGE2->ShowComponent(FALSE);
				m_pENTRY_CHARGE3->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_MONEYBASE->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_MONEYBASE->ShowComponent(FALSE);
				m_pENTRY_CHARGE3_VALUE->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_RUNE->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_LUNA->ShowComponent(FALSE);
				m_pENTRY_CHARGE1_CRON->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_RUNE->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_LUNA->ShowComponent(FALSE);
				m_pENTRY_CHARGE2_CRON->ShowComponent(FALSE);
				m_pENTRY_PARTY_LIST->ShowComponent(FALSE);
				m_pMATCH_BACK->ShowComponent(TRUE);
				m_pMATCH_COL_ENTRY->ShowComponent(TRUE);
				m_pMATCH_COL_QF->ShowComponent(TRUE);
				m_pMATCH_COL_SF->ShowComponent(TRUE);
				m_pMATCH_COL_FINAL->ShowComponent(TRUE);
				m_pMATCH_ENTRY[0]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[1]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[2]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[3]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[4]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[5]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[6]->ShowComponent(TRUE);
				m_pMATCH_ENTRY[7]->ShowComponent(TRUE);
				m_pMATCH_QF[0]->ShowComponent(TRUE);
				m_pMATCH_QF[1]->ShowComponent(TRUE);
				m_pMATCH_QF[2]->ShowComponent(TRUE);
				m_pMATCH_QF[3]->ShowComponent(TRUE);
				m_pMATCH_SF[0]->ShowComponent(TRUE);
				m_pMATCH_SF[1]->ShowComponent(TRUE);
				m_pMATCH_FINAL->ShowComponent(TRUE);

				for( INT i=0 ; i < 4 ; ++i )
				{
					m_pMATCH_QF_UP[i]->ShowComponent( m_bMATCH_QF_UP[i] );
					m_pMATCH_QF_DOWN[i]->ShowComponent( m_bMATCH_QF_DOWN[i] );
					m_pMATCH_QF_UP_HL[i]->ShowComponent( m_bMATCH_QF_UP[i] );
					m_pMATCH_QF_DOWN_HL[i]->ShowComponent( m_bMATCH_QF_DOWN[i] );
				}

				for( INT i=0 ; i < 2 ; ++i )
				{
					m_pMATCH_SF_UP[i]->ShowComponent( m_bMATCH_SF_UP[i] );
					m_pMATCH_SF_DOWN[i]->ShowComponent( m_bMATCH_SF_DOWN[i] );
					m_pMATCH_SF_UP_HL[i]->ShowComponent( m_bMATCH_SF_UP[i] );
					m_pMATCH_SF_DOWN_HL[i]->ShowComponent( m_bMATCH_SF_DOWN[i] );
				}

				m_pMATCH_FINAL_UP->ShowComponent( m_bMATCH_FINAL_UP );
				m_pMATCH_FINAL_UP_HL->ShowComponent( m_bMATCH_FINAL_UP );
				m_pMATCH_FINAL_DOWN->ShowComponent( m_bMATCH_FINAL_DOWN );
				m_pMATCH_FINAL_DOWN_HL->ShowComponent( m_bMATCH_FINAL_DOWN );
				m_pMATCH_FINAL_HL->ShowComponent( m_bMATCH_FINAL );

				m_pBTN_OK->ShowComponent(FALSE);
				m_pBTN_CANCEL->ShowComponent(TRUE);
				m_pBTN_CANCEL->m_strText = CTChart::LoadString(TSTR_CLOSE);

				m_pBTN_ADD_PARTY->ShowComponent( FALSE );
				for( INT i=0 ; i < 6 ; ++i )
					m_pBTN_REMOVE_PARTY[ i ]->ShowComponent( FALSE );
			}
			break;
		}
	}
}

void CTTournament::UpdateMatch( TTMODE mode )
{
	static DWORD ENTRY_TYPE[] =
	{
		0,
		TSTR_TOURNAMENT_PARTY,
		TSTR_TOURNAMENT_PRIVATE
	};

	m_nMode = mode;

	m_pMATCHLIST->RemoveAll();
	m_bShowJoinBtn = TRUE;

	// 매치리스트
	VTMATCH::iterator itMATCH, endMATCH;
	itMATCH = m_vMatch.begin();
	endMATCH = m_vMatch.end();
	for(; itMATCH != endMATCH ; ++itMATCH )
	{
		int nLine = m_pMATCHLIST->AddString( (*itMATCH)->m_strEntry );
		m_pMATCHLIST->SetItemString( nLine, 1, 
			((*itMATCH)->m_bType != 0) ? CTChart::LoadString( (TSTRING) ENTRY_TYPE[(*itMATCH)->m_bType]) : _T("") );

		DWORD dwClass = (*itMATCH)->m_dwClass;
		CString strClass;

		if( (dwClass & (1<<TCLASS_WARRIOR)) &&
			(dwClass & (1<<TCLASS_RANGER)) &&
			(dwClass & (1<<TCLASS_ARCHER)) &&
			(dwClass & (1<<TCLASS_WIZARD)) &&
			(dwClass & (1<<TCLASS_PRIEST)) &&
			(dwClass & (1<<TCLASS_SORCERER)) )
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_ALL);
		}
		else if(dwClass & (1<<TCLASS_WARRIOR))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_WARRIOR);
		}
		else if(dwClass & (1<<TCLASS_RANGER))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_RANGER);
		}
		else if(dwClass & (1<<TCLASS_ARCHER))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_ARCHER);
		}
		else if(dwClass & (1<<TCLASS_WIZARD))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_WIZARD);
		}
		else if(dwClass & (1<<TCLASS_PRIEST))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_PRIEST);
		}
		else if(dwClass & (1<<TCLASS_SORCERER))
		{
			strClass = CTChart::LoadString(TSTR_TCLASS_SORCERER);
		}

		m_pMATCHLIST->SetItemString( nLine, 2, strClass );
		m_pMATCHLIST->SetItemString( nLine, 3, (*itMATCH)->m_bMyApplied ? CTChart::LoadString(TSTR_TOURNAMENT_JOIN) : _T("") );

		for( int i=0 ; i < 5 ; ++i )
			m_pMATCHLIST->SetItemData( nLine, i, (DWORD) (*itMATCH) );

		if( (*itMATCH)->m_bMyApplied )
			m_bShowJoinBtn = FALSE;

#ifdef MODIFY_TT
		DWORD dwColor = CTTournament::SCHEDULE_COLOR[ (*itMATCH)->m_bGroup % TSCHEDULE_COLOR_COUNT ];
		m_pMATCHLIST->SetUserColor( nLine, 0, dwColor );
		m_pMATCHLIST->SetUserColor( nLine, 1, dwColor );
		m_pMATCHLIST->SetUserColor( nLine, 2, dwColor );
		m_pMATCHLIST->SetUserColor( nLine, 3, dwColor );
#endif
	}
}

bool _SortMP( const LPTMATCHPLAYER& a, const LPTMATCHPLAYER& b )
{
	return a->m_bSlotID < b->m_bSlotID;
}

void CTTournament::OnSelectMatch( TMATCH* pMATCH )
{
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();

	if( pMATCH )
	{
#ifdef MODIFY_TT
		UpdateToSchedule(pMATCH);
#endif

		switch( m_nMode )
		{
		case TT_ENTRY:
		case TT_JOIN:
			{
				DWORD dwMONEY[3] = { 0, 0, 0 };

				CTClientGame::SplitMoney(
					pMATCH->m_dwFee,
					&dwMONEY[0],
					&dwMONEY[1],
					&dwMONEY[2]);
				m_pENTRY_CHARGE1_RUNE->m_strText.Format( "%u", dwMONEY[0] ); 
				m_pENTRY_CHARGE1_LUNA->m_strText.Format( "%u", dwMONEY[1] );
				m_pENTRY_CHARGE1_CRON->m_strText.Format( "%u", dwMONEY[2] );

				CTClientGame::SplitMoney(
					pMATCH->m_dwBackFee,
					&dwMONEY[0],
					&dwMONEY[1],
					&dwMONEY[2]);
				m_pENTRY_CHARGE2_RUNE->m_strText.Format( "%u", dwMONEY[0] ); 
				m_pENTRY_CHARGE2_LUNA->m_strText.Format( "%u", dwMONEY[1] );
				m_pENTRY_CHARGE2_CRON->m_strText.Format( "%u", dwMONEY[2] );

				m_pENTRY_CHARGE3_VALUE->m_strText = CTChart::Format(TSTR_TOURNAMENT_NEED_PAGE,
					pMATCH->m_bPermitCount );

				m_pENTRY_NOTICE->m_strText = CTChart::Format(TSTR_TOURNAMENT_COUNT,
					pMATCH->m_bNeed,
					pMATCH->m_wTotalApplicant );

				m_pENTRY_PLAYER_LIST->RemoveAll();
				m_pENTRY_PARTY_LIST->RemoveAll();

				VTMATCHPLAYER::iterator itPLAYER, endPLAYER;
				itPLAYER = pMATCH->m_vPlayer.begin();
				endPLAYER = pMATCH->m_vPlayer.end();
				for(; itPLAYER != endPLAYER ; ++itPLAYER )
				{
					if( pMainChar->m_dwID == (*itPLAYER)->m_dwCharID )
					{
						int nLine = m_pENTRY_PLAYER_LIST->AddString( (*itPLAYER)->m_strName );
						m_pENTRY_PLAYER_LIST->SetItemData( nLine, 0, (DWORD) (*itPLAYER) );
						m_pENTRY_PLAYER_LIST->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*itPLAYER)->m_bCountry] ) );
						CString strFMT;
						strFMT.Format( "%d", (*itPLAYER)->m_bLevel );
						m_pENTRY_PLAYER_LIST->SetItemString( nLine, 2, strFMT );
						m_pENTRY_PLAYER_LIST->SetItemString( nLine, 3, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[(*itPLAYER)->m_bClass] ) );
						strFMT.Format( "%d", (*itPLAYER)->m_dwRank );
						m_pENTRY_PLAYER_LIST->SetItemString( nLine, 4, strFMT );
						strFMT.Format( "%d", (*itPLAYER)->m_dwMonthRank );
						m_pENTRY_PLAYER_LIST->SetItemString( nLine, 5, strFMT );
						break;
					}
				}

				itPLAYER = pMATCH->m_vPlayer.begin();
				endPLAYER = pMATCH->m_vPlayer.end();
				for(; itPLAYER != endPLAYER ; ++itPLAYER )
				{
					if( pMainChar->m_dwID == (*itPLAYER)->m_dwCharID )
						continue;

					int nLine = m_pENTRY_PLAYER_LIST->AddString( (*itPLAYER)->m_strName );
					m_pENTRY_PLAYER_LIST->SetItemData( nLine, 0, (DWORD) (*itPLAYER) );
					m_pENTRY_PLAYER_LIST->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*itPLAYER)->m_bCountry] ) );
					CString strFMT;
					strFMT.Format( "%d", (*itPLAYER)->m_bLevel );
					m_pENTRY_PLAYER_LIST->SetItemString( nLine, 2, strFMT );
					m_pENTRY_PLAYER_LIST->SetItemString( nLine, 3, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[(*itPLAYER)->m_bClass] ) );
					strFMT.Format( "%d", (*itPLAYER)->m_dwRank );
					m_pENTRY_PLAYER_LIST->SetItemString( nLine, 4, strFMT );
					strFMT.Format( "%d", (*itPLAYER)->m_dwMonthRank );
					m_pENTRY_PLAYER_LIST->SetItemString( nLine, 5, strFMT );
				}
			}
			break;
		case TT_MATCH:
			{
				for( INT i=0 ; i < 4 ; ++i )
				{
					m_bMATCH_QF_UP[i] = FALSE;
					m_bMATCH_QF_DOWN[i] = FALSE;
				}

				for( INT i=0 ; i < 2 ; ++i )
				{
					m_bMATCH_SF_UP[i] = FALSE;
					m_bMATCH_SF_DOWN[i] = FALSE;
				}

				m_bMATCH_FINAL_UP = FALSE;
				m_bMATCH_FINAL_DOWN = FALSE;
				m_bMATCH_FINAL = FALSE;

				for( INT i=0 ; i < 8 ; ++i )
				{
					m_pMATCH_ENTRY[ i ]->m_strText.Empty();
					m_pMATCH_ENTRY_INFO[i] = NULL;
				}

				for( INT i=0 ; i < 4 ; ++i )
				{
					CString strFMT = CTChart::Format(TSTR_TOURNAMENT_ARENA, 1+(pMATCH->m_bEntryID-1)*TOURNAMENT_ARENA_COUNT + i );
					m_pMATCH_QF[ i ]->m_strText = strFMT;
					m_pMATCH_QF[ i ]->SetTextClr( D3DCOLOR_XRGB(88,68,31) );
					m_pMATCH_QF_INFO[i] = NULL;
				}

				for( INT i=0 ; i < 2 ; ++i )
				{
					CString strFMT = CTChart::Format(TSTR_TOURNAMENT_ARENA, 1+(pMATCH->m_bEntryID-1)*TOURNAMENT_ARENA_COUNT + i*2 );
					m_pMATCH_SF[ i ]->m_strText = strFMT;
					m_pMATCH_SF[ i ]->SetTextClr( D3DCOLOR_XRGB(88,68,31) );
					m_pMATCH_SF_INFO[i] = NULL;
				}

				CString strFMT = CTChart::Format(TSTR_TOURNAMENT_ARENA, 1+(pMATCH->m_bEntryID-1)*TOURNAMENT_ARENA_COUNT );
				m_pMATCH_FINAL->m_strText = strFMT;
				m_pMATCH_FINAL->SetTextClr( D3DCOLOR_XRGB(88,68,31) );
				m_pMATCH_FINAL_INFO = NULL;

				std::sort( pMATCH->m_vPlayer.begin(), pMATCH->m_vPlayer.end(), _SortMP );

				VTMATCHPLAYER::iterator itPLAYER, endPLAYER;
				itPLAYER = pMATCH->m_vPlayer.begin();
				endPLAYER = pMATCH->m_vPlayer.end();
				for( INT e=0 ; e < 8 ; ++e )
				{
					if( itPLAYER != endPLAYER &&
						(*itPLAYER)->m_bSlotID == e )
					{
						INT q = e/2;
						INT q_up_down = e%2;
						INT sf = e/4;
						INT sf_up_down = (e%4)/2;

						m_pMATCH_ENTRY[e]->m_strText = (*itPLAYER)->m_strName;
						m_pMATCH_ENTRY_INFO[e] = (*itPLAYER);

						if( (*itPLAYER)->m_bQfWin == TNMTWIN_WIN)
						{
							m_pMATCH_QF[q]->m_strText = (*itPLAYER)->m_strName;
							
							m_pMATCH_QF_INFO[q] = (*itPLAYER);
							m_bMATCH_QF_UP[q] = q_up_down == 0;
							m_bMATCH_QF_DOWN[q] = q_up_down == 1;

							if( (*itPLAYER)->m_bSfWin == TNMTWIN_LOSE )
								m_pMATCH_QF[q]->SetTextClr( D3DCOLOR_XRGB(150,150,150) );
							else
								m_pMATCH_QF[q]->SetTextClr( D3DCOLOR_XRGB(210,210,210) );
						}

						if( (*itPLAYER)->m_bSfWin == TNMTWIN_WIN )
						{
							m_pMATCH_SF[sf]->m_strText = (*itPLAYER)->m_strName;
							m_pMATCH_SF_INFO[sf] = (*itPLAYER);
							m_bMATCH_SF_UP[sf] = sf_up_down == 0;
							m_bMATCH_SF_DOWN[sf] = sf_up_down == 1;

							if( (*itPLAYER)->m_bFWin == TNMTWIN_LOSE )
								m_pMATCH_SF[sf]->SetTextClr( D3DCOLOR_XRGB(150,150,150) );
							else
								m_pMATCH_SF[sf]->SetTextClr( D3DCOLOR_XRGB(210,210,210) );
						}

						if( (*itPLAYER)->m_bFWin == TNMTWIN_WIN )
						{
							m_pMATCH_FINAL->m_strText = (*itPLAYER)->m_strName;
							m_pMATCH_FINAL->SetTextClr( D3DCOLOR_XRGB(210,210,210) );
							m_pMATCH_FINAL_INFO = (*itPLAYER);
							m_bMATCH_FINAL_UP = (e/4 == 0);
							m_bMATCH_FINAL_DOWN = (e/4 == 1);
							if( m_bMATCH_FINAL_UP || m_bMATCH_FINAL_DOWN )
								m_bMATCH_FINAL = TRUE;
						}

						++itPLAYER;
					}
					else
					{
						m_pMATCH_ENTRY[e]->m_strText.Empty();
						m_pMATCH_ENTRY_INFO[e] = NULL;
					}
				}
			}
			break;
		}

		m_pITEM[0]->SetCurImage( 0 );
		m_pITEM[1]->SetCurImage( 0 );
		m_pREWARD_RUNE->m_strText.Empty();
		m_pREWARD_LUNA->m_strText.Empty();
		m_pREWARD_CRON->m_strText.Empty();

		INT nRewardItemCount=0;
		m_pREWARD->RemoveAll();
		m_pTTOPREWARD = NULL;

		VTMATCHREWARD::iterator itREWARD, endREWARD;
		itREWARD = pMATCH->m_vReward.begin();
		endREWARD = pMATCH->m_vReward.end();
		for(; itREWARD != endREWARD ; ++itREWARD )
		{
			CString strREWARD;

			switch( (*itREWARD)->m_bCheckShield )
			{
			case 0:
				break;
			case 1: // 방패를 차고 있으면 보여준다.
				if( !pMainChar->GetHasShield() )
					continue;
				break;
			case 2: // 방패를 차고있지 않으면 보여준다.
				if( pMainChar->GetHasShield() )
					continue;
				break;
			}

			switch( (*itREWARD)->m_bChartType )
			{
			case 0: // 퀘스트
				{
					LPTQUESTITEM pQuestItem = CTChart::FindTQUESTMAGICITEM( (*itREWARD)->m_wItemID );

					LPTITEM pTITEM = NULL;
					if( pQuestItem )
						pTITEM = CTChart::FindTITEMTEMP( pQuestItem->m_wItemID );
					else
						pTITEM = NULL;

					if( pTITEM )
						strREWARD = pTITEM->m_strNAME;
				}
				break;

			case 1: // 아이템차트
				{
					LPTITEM pTITEM = NULL;
					pTITEM = CTChart::FindTITEMTEMP( (*itREWARD)->m_wItemID );
					if( pTITEM )
						strREWARD = pTITEM->m_strNAME;
				}
				break;
			}

			if(!strREWARD.IsEmpty())
			{
				INT nLine=0;
				INT nCol = nRewardItemCount % 2;

				CString strFMT;
				if( (*itREWARD)->m_bCount > 1 )
				{
					INT nColWidth = m_pREWARD->GetColWidth(nCol);
					CSize sizeText;
					m_pREWARD->GetTextExtentPoint(strREWARD, sizeText);

					if( sizeText.cx <= nColWidth )
					{
						strFMT = CTChart::Format( TSTR_TOURNAMENT_REWARD_2,
							(LPCTSTR) strREWARD,
							(*itREWARD)->m_bCount );
					}
					else
					{
						strFMT = CTChart::Format( TSTR_TOURNAMENT_REWARD_1,
							(LPCTSTR) strREWARD,
							(*itREWARD)->m_bCount );
					}
				}
				else
					strFMT = strREWARD;

				if( nCol == 0  )
				{
					nLine = m_pREWARD->AddString(
						strFMT );
				}
				else
				{
					nLine = m_pREWARD->GetItemCount()-1; // 마지막 라인
					m_pREWARD->SetItemString(
						nLine,
						nCol,
						(LPCTSTR) strFMT );
				}

				m_pREWARD->SetItemData( nLine, nCol, (DWORD) (*itREWARD) );
				++nRewardItemCount;
			}
		}
	}
	else
	{
		m_pENTRY_NOTICE->m_strText.Empty();

		m_pENTRY_CHARGE1_RUNE->m_strText.Empty();
		m_pENTRY_CHARGE1_LUNA->m_strText.Empty();
		m_pENTRY_CHARGE1_CRON->m_strText.Empty();
		m_pENTRY_CHARGE2_RUNE->m_strText.Empty();
		m_pENTRY_CHARGE2_LUNA->m_strText.Empty();
		m_pENTRY_CHARGE2_CRON->m_strText.Empty();
		m_pENTRY_CHARGE3_VALUE->m_strText.Empty();

		m_pITEM[0]->SetCurImage( 0 );
		m_pITEM[1]->SetCurImage( 0 );

		m_pREWARD_RUNE->m_strText.Empty();
		m_pREWARD_LUNA->m_strText.Empty();
		m_pREWARD_CRON->m_strText.Empty();

		m_pREWARD->RemoveAll();
		m_pENTRY_PLAYER_LIST->RemoveAll();
		m_pENTRY_PARTY_LIST->RemoveAll();

		for( INT i=0 ; i < 8 ; ++i )
		{
			m_pMATCH_ENTRY[i]->m_strText.Empty();
			m_pMATCH_ENTRY_INFO[i] = NULL;
		}
		for( INT i=0 ; i < 4 ; ++i )
		{
			m_pMATCH_QF[i]->m_strText.Empty();
			m_pMATCH_QF_INFO[i] = NULL;
		}
		for( INT i=0 ; i < 2 ; ++i )
		{
			m_pMATCH_SF[i]->m_strText.Empty();
			m_pMATCH_SF_INFO[i] = NULL;
		}

		m_pMATCH_FINAL->m_strText.Empty();
		m_pMATCH_FINAL_INFO = NULL;
	}

	if( IsVisible() )
		ShowComponent(TRUE);
}

void CTTournament::RequestData()
{
	CTClientGame::GetInstance()->GetSession()->SendCS_TOURNAMENTAPPLYINFO_REQ();
}

void CTTournament::ReleaseData()
{
	VTMATCH::iterator itMATCH, endMATCH;
	itMATCH = m_vMatch.begin();
	endMATCH = m_vMatch.end();
	for(; itMATCH != endMATCH ; ++itMATCH )
		delete (*itMATCH);

	m_vMatch.clear();
}

void CTTournament::ReleaseSchedule()
{
#ifdef MODIFY_TT
	MAPSCHEDULEGROUP::iterator it, end;
	it = m_mapGroup.begin();
	end = m_mapGroup.end();
	for(; it != end ; ++it)
		delete it->second;

	m_mapGroup.clear();
	m_pCurrentGroup = NULL;
#else
	VTTOURNAMENTSCHEDULE::iterator it, end;
	it = m_vSchedule.begin();
	end = m_vSchedule.end();
	for(; it != end ; ++it )
		delete (*it);

	m_vSchedule.clear();
	m_pCurrentStep = NULL;
#endif
}

void CTTournament::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	if( msg == TNM_SEL_CHANGE )
	{
		if( from == m_pMATCHLIST->m_id )
		{
			INT nCur = m_pMATCHLIST->GetCurSelIndex();

			if( nCur != -1 )
			{
				TMATCH* pMATCH = (TMATCH*) m_pMATCHLIST->GetItemData( nCur, 0 );
				OnSelectMatch( pMATCH );
				m_nPrevSelectedMatch = nCur;
			}
			else
				OnSelectMatch( NULL );
		}
		else if( from == m_pENTRY_PLAYER_LIST->m_id )
		{
			INT nCur = m_pENTRY_PLAYER_LIST->GetCurSelIndex();

			if( nCur != -1 )
			{
				TMATCHPLAYER* pPLAYER = (TMATCHPLAYER*) m_pENTRY_PLAYER_LIST->GetItemData( nCur, 0 );
				CTClientGame::GetInstance()->GetSession()->SendCS_TOURNAMENTPARTYLIST_REQ(pPLAYER->m_dwCharID);
			}
			else
			{
				m_pENTRY_PARTY_LIST->RemoveAll();
			}

			ResetParty();
		}
	}

	CTClientUIBase::OnNotify( from, msg, param );
}

HRESULT CTTournament::Render( DWORD dwTickCount )
{
	if(IsVisible())
	{
		TMATCHREWARD* pTREWARD[2] = { NULL, NULL};
		INT nIndex = m_pREWARD->GetTop();
        
		for( INT i=0 ; i < 2 ; ++i )
		{
			if( m_pREWARD->GetItemCount() > 0 && nIndex >= 0 )
			{
				DWORD dwValue = m_pREWARD->GetItemData( nIndex, i);

				if( dwValue == -1 )
					pTREWARD[i] = NULL;
				else
					pTREWARD[i] = (TMATCHREWARD*)dwValue;
			}

			if(!pTREWARD[i])
			{
				m_pITEM[i]->ShowComponent(FALSE);
			}
		}

		if( m_pTTOPREWARD != pTREWARD[0] )
		{
			m_pTTOPREWARD = pTREWARD[0];
			for( INT i=0; i<2; i++)
			{
				m_pITEM[i]->ShowComponent(FALSE);

				if(pTREWARD[i])
				{
					LPTITEM pTITEM = NULL;
					switch( pTREWARD[i]->m_bChartType )
					{
					case 0: // 퀘스트
						{
							LPTQUESTITEM pQuestItem = CTChart::FindTQUESTMAGICITEM( pTREWARD[i]->m_wItemID );
							if( pQuestItem )
								pTITEM = CTChart::FindTITEMTEMP( pQuestItem->m_wItemID );
						}
						break;

					case 1: // 아이템차트
						{
							pTITEM = CTChart::FindTITEMTEMP( pTREWARD[i]->m_wItemID );
						}
						break;
					}

					if( pTITEM )
					{
						LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
						WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;
						m_pITEM[i]->SetCurImage(wImg);
						m_pITEM[i]->ShowComponent(TRUE);

						if( pTREWARD[i]->m_bCount > 1 )
							m_pITEM[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pTREWARD[i]->m_bCount);
						else
							m_pITEM[i]->m_strText.Empty();
					}
					else
					{
						m_pITEM[i]->m_strText.Empty();
						m_pITEM[i]->ShowComponent(FALSE);
					}
				}
			}
		}
	}

	return CTClientUIBase::Render( dwTickCount );
}

int CTTournament::OnGM_TOURNAMENT_OK()
{
	switch( m_nMode )
	{
	case TT_ENTRY:
		{
			INT nSel = m_pMATCHLIST->GetCurSelIndex();
			if( nSel != -1 )
			{
				MAP_LISTITEM* pItem = m_pMATCHLIST->GetCurSelItem();
				MAP_LISTITEM::iterator it = pItem->find( 0 );
				if( it != pItem->end() )
				{
					TMATCH* pMATCH = (TMATCH*) it->second->m_param;
					DWORD dwParam = (DWORD)( pMATCH->m_bEntryID );

					CString strMSG = CTChart::Format(TSTR_TOURNAMENT_JOIN_ENTRY, it->second->m_strText);
					CTClientGame::GetInstance()->GetMainWnd()->MessageBoxYesNo(
						strMSG,
						TSTR_YES,
						TSTR_NO,
						TCOMMAND( GM_TOURNAMENT_ENTRY_DO, dwParam ),
						GM_CLOSE_MSGBOX,
						GM_CLOSE_MSGBOX,
						FALSE );
				}
			}
		}
		break;
	case TT_JOIN:
		break;
	case TT_MATCH:
		break;
	}
	return TERR_NONE;
}

int CTTournament::OnGM_TOURNAMENT_CANCEL()
{
	CTClientGame::GetInstance()->OnGM_CLOSE_UI();
	return TERR_NONE;
}
#ifdef MODIFY_TT
BYTE GetStepType( BYTE bStep )
{
	switch( bStep )
	{
	case TNMTSTEP_ENTER:
	case TNMTSTEP_QFINAL:
	case TNMTSTEP_QFEND:
		return 8;

	case TNMTSTEP_SFENTER:
	case TNMTSTEP_SFINAL:
	case TNMTSTEP_SFEND:
		return 4;

	case TNMTSTEP_FENTER:
	case TNMTSTEP_FINAL:
		return 2;
	}

	return 0;
}
#else
BYTE GetStepType( BYTE bStep )
{
	switch( bStep )
	{
	case TNMTSTEP_ENTER:
	case TNMTSTEP_QFINAL:
	case TNMTSTEP_QFEND:
		return 8;

	case TNMTSTEP_SFENTER:
	case TNMTSTEP_SFINAL:
	case TNMTSTEP_SFEND:
		return 4;

	case TNMTSTEP_FENTER:
	case TNMTSTEP_FINAL:
	case TNMTSTEP_END:
		return 2;
	}

	return 0;
}
#endif

void CTTournament::CalcScheduleTop()
{
#ifdef MODIFY_TT
	if( m_pCurrentGroup )
	{
		m_pCurrentGroup->m_pCurrentStep = NULL;
		BOOL bFind = FALSE;
		for( INT i=0 ; i < m_pCurrentGroup->m_vSchedule.size() ; ++i )
		{
			if( m_pCurrentGroup->m_vSchedule[i]->m_bStep == m_pCurrentGroup->m_bCurrentStep )
			{
				m_pCurrentGroup->m_pCurrentStep = m_pCurrentGroup->m_vSchedule[i];
				bFind = TRUE;
				break;
			}
		}

		if( bFind )
		{
			m_pCurrentGroup->m_nScheduleTop = i - 3;

			if( m_pCurrentGroup->m_nScheduleTop < 0 )
				m_pCurrentGroup->m_nScheduleTop = 0;
			if( m_pCurrentGroup->m_nScheduleTop > m_pCurrentGroup->m_vSchedule.size()-6 )
				m_pCurrentGroup->m_nScheduleTop = m_pCurrentGroup->m_vSchedule.size()-6;
		}
		else
			m_pCurrentGroup->m_nScheduleTop = 0;
	}
#else
	m_pCurrentStep = NULL;
	BOOL bFind = FALSE;
	for( INT i=0 ; i < m_vSchedule.size() ; ++i )
	{
		if( m_vSchedule[i]->m_bStep == m_bCurrentStep )
		{
			m_pCurrentStep = m_vSchedule[i];
			bFind = TRUE;
			break;
		}
	}

	if( bFind )
	{
		m_nScheduleTop = i - 3;

		if( m_nScheduleTop < 0 )
			m_nScheduleTop = 0;
		if( m_nScheduleTop > m_vSchedule.size()-6 )
			m_nScheduleTop = m_vSchedule.size()-6;
	}
	else
		m_nScheduleTop = 0;
#endif
}

bool CTTournament::IsCommonSchedule(BYTE bStep)
{
	switch( bStep )
	{
	case TNMTSTEP_READY:
	case TNMTSTEP_1st:
	case TNMTSTEP_NORMAL:
	case TNMTSTEP_PARTY:
		return true;
	}
	return false;
}

void CTTournament::UpdateSchedule()
{
#ifdef MODIFY_TT
	if( m_pCurrentGroup )
	{
		for( INT i=0 ; i < 6 ; ++i )
		{
			int idx = m_pCurrentGroup->m_nScheduleTop + i;
			if( idx < 0 || idx >= m_pCurrentGroup->m_vSchedule.size() )
			{
				m_pSTEP_DATE[i]->m_strText.Empty();
				m_pSTEP_TITLE[i]->m_strText.Empty();
				if( m_pSTEP_STICK[i] )
					m_pSTEP_STICK[i]->SetCurImage(0);
				m_bSTEP[i] = FALSE;
				continue;
			}

			LPTTOURNAMENTSCHEDULE pSchedule = m_pCurrentGroup->m_vSchedule[idx];

			m_pSTEP_DATE[i]->m_strText.Format( "%d/%d %.2d:%.2d",
				pSchedule->m_StartTime.GetMonth(),
				pSchedule->m_StartTime.GetDay(),
				pSchedule->m_StartTime.GetHour(),
				pSchedule->m_StartTime.GetMinute());

			m_pSTEP_TITLE[i]->m_strText = (pSchedule->m_bStep != 0) ? CTChart::LoadString( (TSTRING) STEP_TITLE[ pSchedule->m_bStep ] ) : _T("");
			m_bSTEP[i] = TRUE;

			if( m_pSTEP_STICK[i] )
				if( pSchedule->m_bStep <= m_pCurrentGroup->m_bCurrentStep )
					m_pSTEP_STICK[i]->SetCurImage(0);
				else
					m_pSTEP_STICK[i]->SetCurImage(1);

			int nColor = IsCommonSchedule( pSchedule->m_bStep ) ? 0 : m_pCurrentGroup->m_bGroupID % TSCHEDULE_COLOR_COUNT;

			if( pSchedule->m_bStep == m_pCurrentGroup->m_bCurrentStep )
				m_pCurrentGroup->m_pCurrentStep = pSchedule;

			if( (m_bCurrentGroup == m_pCurrentGroup->m_bGroupID || m_bCurrentGroup == 0) &&
				pSchedule->m_bStep == m_pCurrentGroup->m_bCurrentStep )
			{
				DWORD dwColor = CTTournament::SCHEDULE_COLOR[ nColor ];
				m_pSTEP_DATE[i]->SetTextClr( dwColor );
				m_pSTEP_TITLE[i]->SetTextClr( dwColor );
				m_pSTEP_BACK[i]->SetCurImage(1);
				m_pCurrentGroup->m_pCurrentStep = pSchedule;
			}
			else
			{
				DWORD dwColor = CTTournament::SCHEDULE_COLOR_DISABLE[ nColor ];
				m_pSTEP_DATE[i]->SetTextClr( dwColor );
				m_pSTEP_TITLE[i]->SetTextClr( dwColor );
				m_pSTEP_BACK[i]->SetCurImage(0);
			}
		}
	}
#endif
}

LPTMATCHPLAYER CTTournament::FindPlayer( DWORD dwCharID )
{
	VTMATCH::iterator itMATCH, endMATCH;
	itMATCH = m_vMatch.begin();
	endMATCH = m_vMatch.end();
	for(; itMATCH != endMATCH ; ++itMATCH)
	{
		VTMATCHPLAYER::iterator itPLAYER, endPLAYER;
		itPLAYER = (*itMATCH)->m_vPlayer.begin();
		endPLAYER = (*itMATCH)->m_vPlayer.end();
 
		for(; itPLAYER != endPLAYER ; ++itPLAYER)
		{
			if( (*itPLAYER)->m_dwCharID == dwCharID )
			{
				return (*itPLAYER);
			}
		}
	}

	return NULL;
}

void CTTournament::ResetParty()
{
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();

	INT nCur = m_pMATCHLIST->GetCurSelIndex();
	TMATCH* pMATCH = NULL;
	if( nCur != -1 )
		pMATCH = (TMATCH*) m_pMATCHLIST->GetItemData( nCur, 0 );

	m_pENTRY_PARTY_LIST->RemoveAll();
	m_pBTN_ADD_PARTY->ShowComponent( FALSE );
	for( INT i=0 ; i < 6 ; ++i )
		m_pBTN_REMOVE_PARTY[ i ]->ShowComponent( FALSE );

#ifdef MODIFY_TT
	if( pMATCH && pMATCH->m_bType == ENTRY_PARTY && m_pCurrentGroup && m_pCurrentGroup->m_bCurrentStep == TNMTSTEP_PARTY )
#else
	if( pMATCH && pMATCH->m_bType == ENTRY_PARTY && m_bCurrentStep == TNMTSTEP_PARTY )
#endif
		if( m_pENTRY_PLAYER_LIST->GetItemCount() > 0 )
		{
			LPTMATCHPLAYER pPLAYER = (LPTMATCHPLAYER) m_pENTRY_PLAYER_LIST->GetItemData(0,0);
			if( pPLAYER->m_dwCharID == CTClientGame::GetInstance()->GetMainChar()->m_dwID )
			{
				m_pBTN_ADD_PARTY->ShowComponent( TRUE );
			}
		}

	INT nCurSel = m_pENTRY_PLAYER_LIST->GetCurSelIndex();
	if(nCurSel != -1)
	{
		LPTMATCHPLAYER pPLAYER = (LPTMATCHPLAYER) m_pENTRY_PLAYER_LIST->GetItemData( nCurSel, 0 );

		BOOL bPartyLeader = FALSE;
		if( pPLAYER->m_strName == pMainChar->m_strNAME )
			bPartyLeader = TRUE;

		INT n=0;
		VTMATCHPARTY::iterator itPARTY, endPARTY;
		itPARTY = pPLAYER->m_vParty.begin();
		endPARTY = pPLAYER->m_vParty.end();
		for(; itPARTY != endPARTY ; ++itPARTY )
		{
			if( n >= 6 )
				break;

			TMATCHPARTY* pPARTY = (*itPARTY);
			int nLine = m_pENTRY_PARTY_LIST->AddString( pPARTY->m_strName );
			m_pENTRY_PARTY_LIST->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[pPARTY->m_bCountry] ) );
			CString strFMT;
			strFMT.Format( "%d", pPARTY->m_bLevel );
			m_pENTRY_PARTY_LIST->SetItemString( nLine, 2, strFMT );
			m_pENTRY_PARTY_LIST->SetItemString( nLine, 3, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[pPARTY->m_bClass] ) );
			strFMT.Format( "%d", pPARTY->m_dwRank );
			m_pENTRY_PARTY_LIST->SetItemString( nLine, 4, strFMT );
			strFMT.Format( "%d", pPARTY->m_dwMonthRank );
			m_pENTRY_PARTY_LIST->SetItemString( nLine, 5, strFMT );
			m_pENTRY_PARTY_LIST->SetItemData( nLine, 0, (DWORD) (pPARTY) );
			m_pBTN_REMOVE_PARTY[ n++ ]->ShowComponent( bPartyLeader || pMainChar->m_strNAME == pPARTY->m_strName );
		}
	}
}

ITDetailInfoPtr CTTournament::GetTInfoKey(const CPoint& point)
{
	CRect rc;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = point.x+32;
	rc.bottom = point.y+32;
	
	ITDetailInfoPtr pInfo;
	int nIndex = m_pREWARD->GetTop();

	for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
	{
		LPTMATCHREWARD pTREWARD = NULL;

		if( m_pREWARD->GetItemCount() > 0 && nIndex >= 0 )
			pTREWARD = (LPTMATCHREWARD) m_pREWARD->GetItemData( nIndex, i );

		if(pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) )
		{
			switch(pTREWARD->m_bChartType)
			{
			case 0:// 0:퀘스트
			case 1	://  1:아이템차트
				if(m_pITEM[i]->HitTest(point))
				{
					if( pTREWARD->m_bChartType == 1 )
					{
						LPTITEM pTITEM = CTChart::FindTITEMTEMP(WORD(pTREWARD->m_wItemID));
						pInfo = CTDetailInfoManager::NewItemInst(pTITEM, rc);
					}
					else
					{
						CTClientItem* pCItem = CTClientItem::FindTQuestMagicClientItem( (WORD) pTREWARD->m_wItemID );
						pInfo = CTDetailInfoManager::NewItemInst(pCItem, rc);
					}
				}
				break;
			}
		}
	}

	if( pInfo )
		return pInfo;

	for( INT i=0 ; i < 8 ; ++i )
		if( m_pMATCH_ENTRY[i]->HitTest(point) && m_pMATCH_ENTRY_INFO[i] )
			return GetTInfoKey_Player(m_pMATCH_ENTRY_INFO[i], m_pMATCH_ENTRY_INFO[i]->m_bQfWin, rc);

	for( INT i=0 ; i < 4 ; ++i )
	{
		if( m_pMATCH_QF[i]->HitTest(point) && m_pMATCH_QF_INFO[i] )
			return GetTInfoKey_Player(m_pMATCH_QF_INFO[i], m_pMATCH_QF_INFO[i]->m_bSfWin, rc);
	}

	for( INT i=0 ; i < 2 ; ++i )
	{
		if( m_pMATCH_SF[i]->HitTest(point) && m_pMATCH_SF_INFO[i] )
			return GetTInfoKey_Player(m_pMATCH_SF_INFO[i], m_pMATCH_SF_INFO[i]->m_bFWin, rc);
	}

	if( m_pMATCH_FINAL->HitTest(point) && m_pMATCH_FINAL_INFO )
		return GetTInfoKey_Player(m_pMATCH_FINAL_INFO, TNMTWIN_WIN, rc);

	if( m_pMATCHLIST->HitTest(point) )
	{
		TListItem* pHITITEM = m_pMATCHLIST->GetHitItem( point );
		if( pHITITEM && pHITITEM->m_param != -1 )
		{
			LPTMATCH pTMATCH = (LPTMATCH)( pHITITEM->m_param );
			if( pTMATCH )
			{
				ScheduleGroup* pGroup = FindScheduleGroup(pTMATCH->m_bGroup);
				if( pGroup && pGroup->m_bCurrentStep != TNMTSTEP_READY )
				{
					LPTTOURNAMENTSCHEDULE pEnterStep = pGroup->GetTNMTSTEP_ENTER();
					if( pEnterStep && pGroup->m_bCurrentStep < TNMTSTEP_ENTER )
					{
						CString strText;
						strText = CTChart::Format( TSTR_TOURNAMENT_STARTTIME,
							pEnterStep->m_StartTime.GetMonth(),
							pEnterStep->m_StartTime.GetDay(),
							pEnterStep->m_StartTime.GetHour(),
							pEnterStep->m_StartTime.GetMinute());

						return CTDetailInfoManager::NewTournamentPlayerInst(
							strText,
							"",
							(int) pTMATCH,
							rc );
					}
				}
			}
		}
	}

	return pInfo;
}

ITDetailInfoPtr CTTournament::GetTInfoKey_Player(LPTMATCHPLAYER pPlayer, BYTE bResult, const CRect& rc)
{
	return CTDetailInfoManager::NewTournamentPlayerInst(
		pPlayer->m_strName,
		bResult,
		pPlayer->m_bCountry,
		pPlayer->m_bClass,
		pPlayer->m_bLevel,
		rc );
}

#ifdef MODIFY_TT
CTTournament::ScheduleGroup* CTTournament::FindScheduleGroup( BYTE bGroup )
{
	MAPSCHEDULEGROUP::iterator finder = m_mapGroup.find( bGroup );
	if( finder != m_mapGroup.end() )
		return finder->second;

	ScheduleGroup* pGroup = new ScheduleGroup;
	pGroup->m_bGroupID = bGroup;
	pGroup->m_bCurrentStep = -1;
	pGroup->m_nScheduleTop = 0;
	pGroup->m_pCurrentStep = NULL;
	
	m_mapGroup.insert( MAPSCHEDULEGROUP::value_type(bGroup, pGroup) );

	return pGroup;
}

void CTTournament::UpdateToSchedule(TMATCH* pMATCH)
{
	if( !pMATCH)
		return ;

	ScheduleGroup* pGroup = FindScheduleGroup(pMATCH->m_bGroup);
	if( pGroup )
	{
		m_pCurrentGroup = pGroup;
		CalcScheduleTop();
		UpdateSchedule();
	}
}
#endif