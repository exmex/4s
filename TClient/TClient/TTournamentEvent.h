#pragma once

#ifdef ADD_TOURNAMENT

//m_nMode
#define TTEVENT_SELECT_EVENT 0
#define TTEVENT_SELECT_ENTRY 1

class CTTournamentEvent : public CTClientUIBase
{
public:
	CTTournamentEvent( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTTournamentEvent();
	void SetInfo( BYTE bBase, DWORD dwSum );
	LPTTEVENTMATCH GetEntry( BYTE bEntryID );
	void OnMatchDataReset();
	void OnListDataReset( LPTTEVENTMATCH pMatch );
	void OnPartyDataReset( LPTTEVENTTARGET pTarget );
	void ClearMatch();
	void ResetShow();
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual void ShowComponent( BOOL bVisible);
	void SetMode( INT nMode );

public:
	INT m_nMode;
	VTTEVENTMATCH m_vMatch;

public:
	TComponent* m_pCOMMENT;
	TComponent* m_pCOMMENT2;
	TComponent* m_pCOMMENT3;
	TList* m_pEVENTLIST;
	TList* m_pENTRYLIST;
	TList* m_pPARTYLIST;
	TButton* m_pBTN_SELECT;
	TButton* m_pBTN_CLOSE;
	TButton* m_pBTN_PREV;
	TComponent* m_pBASE_ENTRYLIST;
	TComponent* m_pBASE_PARTYLIST;
	TComponent* m_pENTRY_COL_1;
	TComponent* m_pENTRY_COL_2;
	TComponent* m_pENTRY_COL_3;
	TComponent* m_pENTRY_COL_4;
	TComponent* m_pENTRY_COL_5;
	TComponent* m_pENTRY_COL_6;
	TComponent* m_pENTRY_COL_7;
	TComponent* m_pENTRY_COL_8;
	TComponent* m_pEVENTLIST_MATCH;
	TComponent* m_pEVENTLIST_TYPE;
	TComponent* m_pEVENTLIST_MYCHOICE;
	TComponent* m_pEVENTLIST_COUNTRY;
	TComponent* m_pEVENTLIST_S;
	TComponent* m_pEVENTLIST_A;
	CPoint m_ptCOMMENT;
	CPoint m_ptBTN_SELECT;
	CPoint m_ptBTN_PREV;
	CPoint m_ptBTN_CLOSE;
};

#endif