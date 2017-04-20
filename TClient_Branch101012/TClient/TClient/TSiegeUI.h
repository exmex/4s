#pragma once


//! 공성전 진행상황 UI Class.
/*!
*
*
*/
class CTSiegeUI : public CTClientUIBase
{
private :
	void _set_strText( TComponent* pComp, CString strText = "" );
	void _get_hmsTime( DWORD dwTick, WORD* _hour, WORD* _minute, WORD* _second );

	TComponent*		m_pTTopBase;
	TComponent*		m_pTGaugeBarAtk;
	TComponent*		m_pTGaugeBarDef;
	TComponent*		m_pTGaugeAtk;
	TComponent*		m_pTGaugeDef;
	TComponent*		m_pTTimer;
	TComponent*		m_pTKillPointAtk;
	TComponent*		m_pTKillPointDef;
	TComponent*		m_pTBallCharAtk1;
	TComponent*		m_pTBallCharAtk1Time;
	TComponent*		m_pTBallCharAtk2;
	TComponent*		m_pTBallCharAtk2Time;
	TComponent*		m_pTBallCharDef1;
	TComponent*		m_pTBallCharDef1Time;
	TComponent*		m_pTBallCharDef2;
	TComponent*		m_pTBallCharDef2Time;
	TComponent*		m_pTGuildNameAtk;
	TComponent*		m_pTGuildNameDef;

	BOOL			m_bSubHide;
	enum			{ TSIEGE_HIDE_COUNT = 15 };
	TComponent*		m_pTHideComp[TSIEGE_HIDE_COUNT];

public :

	CTSiegeUI( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual	~CTSiegeUI();

	void			InitSiegeUI();
	void			CalcTimer( DWORD dwLeftTick );
	void			RecvBalanceOfPower(
		FLOAT fPower_Def,
		DWORD dwLeftTick,
		WORD wKillPoint_Atk,
		WORD wKillPoint_Def,
		CString strOwner1_Atk,
		CString strOwner2_Atk,
		CString strOwner1_Def,
		CString strOwner2_Def,
		WORD wKeep1_Atk,
		WORD wKeep2_Atk,
		WORD wKeep1_Def,
		WORD wKeep2_Def );

	void			EndWar();
	void			ShowSubComponent( BOOL bShow = TRUE );
	virtual BOOL	CanWithItemUI();

public :

	void			SetSubwinHide( BOOL bHide ) { m_bSubHide = bHide; }
	BOOL			GetSubwinHide() const { return m_bSubHide; }

	void			SetAtkGuildName( CString strAtkGName ) { if( m_pTGuildNameAtk ) m_pTGuildNameAtk->m_strText = strAtkGName; }
	void			SetDefGuildName( CString strDefGName ) { if( m_pTGuildNameDef ) m_pTGuildNameDef->m_strText = strDefGName; }
};

