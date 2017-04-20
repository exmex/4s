#pragma once


//! 공성전 종료 UI Class.
/*!
*
*
*/
class CTSiegeEnd : public CTClientUIBase
{
private :

	TComponent*		m_pTGuildNameAtk;
	TComponent*		m_pTGuildNameDef;
	TComponent*		m_pTCountryAtk;
	TComponent*		m_pTCountryDef;
	TGauge*			m_pTGaugeTotalAtk;
	TGauge*			m_pTGaugeTotalDef;
	//TComponent*		m_pTPowerAtk;
	//TComponent*		m_pTPowerDef;
	//TComponent*		m_pTPointAtk;
	//TComponent*		m_pTPointDef;
	TComponent*		m_pTKillAtk;
	TComponent*		m_pTKillDef;
	//TComponent*		m_pTTotalAtk;
	//TComponent*		m_pTTotalDef;
	TComponent*		m_pTExit;
	TComponent*		m_pTCloseBtn;

//	DWORD			m_dwTotalAtk;
	DWORD			m_dwPowerAtk;
//	WORD			m_wPointAtk;
	WORD			m_wKillAtk;

//	DWORD			m_dwTotalDef;
	DWORD			m_dwPowerDef;
//	WORD			m_wPointDef;
	WORD			m_wKillDef;

	BOOL			m_bCounting;
	DWORD			m_dwKillCountingAtk;
	DWORD			m_dwKillCountingDef;
	DWORD			m_dwPowerCountingAtk;
	DWORD			m_dwPowerCountingDef;

public :

	CTSiegeEnd( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTSiegeEnd();
	
	void			InitSiegeEnd();
	
	void			Endwar(
		BYTE bType,
		DWORD dwWinGuildID,
		DWORD dwDefTotal,
		DWORD dwAtkTotal,
		CString strDefName,
		DWORD dwDefPower,
		WORD wDefPoint,
		WORD wDefKillPoint,
		CString strAtkName,
		DWORD dwAtkPower,
		WORD wAtkPoint,
		WORD wAtkKillPoint);

	void			EndSkygarden(
		BYTE bWinCountry,
		BYTE bLosecountry);

	BOOL			CountNumber( DWORD* dwCurCount, DWORD dwMaxCount, TComponent* pComp );
	void			SetGauge( DWORD CurCount, DWORD MaxCount, TGauge* pGauge );
	BOOL			IsCounting() { return m_bCounting; }

public :

	virtual HRESULT Render(DWORD dwTickCount);
};
