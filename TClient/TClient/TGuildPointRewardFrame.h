#pragma once

class CTGuildPointRewardFrame : public CTClientUIBase
{
public:
	CTGuildPointRewardFrame( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildPointRewardFrame();

	TEdit* GetCurEdit();

public:
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);

public:
	TEdit* m_pTargetEdit;
	TEdit* m_pPointEdit;
	TComponent* m_pGuildPoint;
	TMultiLineEdit*	m_pMessage;
};