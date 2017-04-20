#pragma once

class CTTournamentMode : public CTClientUIBase
{
public:
	enum MODE
	{
		TM_NONE=0,
		TM_WATCHING=1,
		TM_BATTLE=2
	};

public:
	CTTournamentMode( TComponent *pParent, LP_FRAMEDESC pDesc, CTTournamentMode::MODE mode);
	virtual ~CTTournamentMode();
	void SetInfoTMode(
		LPTTOURNAMENTTEAM pTLeftTeam,
		LPTTOURNAMENTTEAM pTRightTeam,
		BOOL bResetOption,
		CString strInitTitle=_T("") );

	void Reset();
	HRESULT Render( DWORD dwTickCount );
	void ShowComponent(BOOL bVisible);

public:
	MODE m_eMode;
	TComponent* m_pNAME1;
	TComponent* m_pNAME2;
	TComponent* m_pTIME;
	TGauge* m_pGAUGE;
	TComponent* m_pKILL1_COUNT;
	TComponent* m_pKILL2_COUNT;
	DWORD m_dwRemainTime;
	DWORD m_dwRemainMIN;
	DWORD m_dwRemainSEC;
	BOOL m_bRemainTime;
	TButton* m_pBtnShowPlayerInfo;
	TComponent* m_pTxtShowPlayerInfo;
};