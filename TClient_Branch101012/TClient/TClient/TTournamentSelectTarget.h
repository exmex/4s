#pragma once

#ifdef ADD_TOURNAMENT

#define TTST_TAB_GUILD 0
#define TTST_TAB_FRIEND 1

class CTTournamentSelectTarget : public CTClientUIBase
{
public:
	CTTournamentSelectTarget( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTTournamentSelectTarget();
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	void OnRecvGuildMemberList();
	TEdit* GetCurEdit();
	void SetAddPartyMode();
	void SetCheerUpMode();
	void OnGM_TOURNAMENT_SELECT_TARGET();
	void ChangeTab(INT nMode);
	void OnEnableUI();

public:
	TList* m_pLIST;
	TEdit* m_pEDIT_NAME;
	TButton* m_pBTN_GUILD;
	TButton* m_pBTN_FRIEND;
	TButton* m_pBTN_ADD;
	INT m_nMode;
	INT m_nFuncMode;
	BOOL m_bListening;
	BYTE m_bCheerItemInven;
	BYTE m_bCheerItemSlot;
};

#endif