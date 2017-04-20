#pragma once

bool IsPREMIUMMaintain( CTClientMaintain* pMAINTAIN );
bool IsPREMIUMMaintain( LPTSKILL pTSKILL );


class CTMaintainPannel : public CTClientUIBase
{
public:
	CTMaintainPannel( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMaintainPannel();

public:
	void ResetMAINTAIN(
		CTClientObjBase *pOBJ,
		DWORD dwTick);

	void ResetROTATION( DWORD dwTick);
	void HideAll();
	void ClickedPNext();
	void ClickedPPrev();
	void ClickedNNext();
	void ClickedNPrev();

public:
	virtual BOOL HitTest( CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);

protected:
	TImageList*				m_pMAINTAIN_ICON[ TMTYPE_COUNT ][ TMAINTAIN_COUNT ];
	TComponent*			m_pMAINTAIN_TIME[ TMTYPE_COUNT ][ TMAINTAIN_COUNT ];
	LPTSKILL					m_vMAINTAININFO_TSKILL[ TMTYPE_COUNT ][ TMAINTAIN_COUNT ];
	BYTE						m_vMAINTAININFO_LEVEL[ TMTYPE_COUNT ][ TMAINTAIN_COUNT ];
	TButton*					m_pNextBtn[TMTYPE_COUNT];
	BYTE						m_bCount[TMTYPE_COUNT];
	CTClientObjBase*		m_pOBJ;
};
