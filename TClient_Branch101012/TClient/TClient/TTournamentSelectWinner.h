#pragma once

#ifdef ADD_TOURNAMENT

class CTTournamentSelectWinner : public CTClientUIBase
{
public:
	CTTournamentSelectWinner( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTTournamentSelectWinner();

public:
	TList* m_pPLAYERLIST;
};

#endif