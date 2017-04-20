#pragma once

#ifdef ADD_TOURNAMENT

class CTTournamentMode;

class CTTournamentGage : public CTClientUIBase
{
public:
	CTTournamentGage( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser, CTClientGame *pMainGame);
	virtual ~CTTournamentGage();
	BOOL CanWithItemUI();
	virtual void ResetPosition();
	virtual void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point);
	void ResetTMAINTAIN();
	void ResetTournamentINFO();

public:
	TComponent* m_pBACK[2][7];
	TComponent* m_pBASE[2][7];
	TComponent* m_pLVBASE[2][7];
	TImageList* m_pFACE[2][7];
	TGauge* m_pHP[2][7];
	TGauge* m_pMP[2][7];
	TComponent* m_pHPText[2][7];
	TComponent* m_pCLASSText[2][7];
	TComponent* m_pNAME[2][7];
	TComponent* m_pLEVEL[2][7];
	CTMaintainPannel* m_pMAINTAIN[2][7];
	CTTournamentMode* m_pTournamentInfo;
	CTClientUIBase* m_pPARTY_GAUGE[2];
	BOOL m_bShowPlayerInfo;
};

#endif