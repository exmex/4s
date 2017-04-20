#include "stdafx.h"
#include "TTournamentSelectWinner.h"

#ifdef ADD_TOURNAMENT

CTTournamentSelectWinner::CTTournamentSelectWinner( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc )
{
	m_pPLAYERLIST = (TList*)FindKid( ID_CTRLINST_PLAYERLIST );
	m_pPLAYERLIST->AddString( "" );
}

CTTournamentSelectWinner::~CTTournamentSelectWinner()
{
}
#endif