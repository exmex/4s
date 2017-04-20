#pragma once

class CTChallengeWaiting : public CTClientUIBase
{
public:
	CTChallengeWaiting(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTChallengeWaiting(void);
	void ShowComponent( BOOL bVisible );
	HRESULT Render( DWORD dwTickCount );

public:
	TButton* m_pOK;
	TComponent* m_pMSG_UP;
	TGauge* m_pGAUGE;
	TComponent* m_pMSG;
	DWORD m_dwNPC;
	DWORD m_dwGaugeTick;
	DWORD m_dwGaugeMax;
	BYTE m_bWinCount;
};