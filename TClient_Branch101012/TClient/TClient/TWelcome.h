#pragma once

class CTWelcome : public CTClientUIBase
{
public:
	CTWelcome( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTWelcome();

	void ShowComponent(BOOL bVisible);
	void OnLButtonUp(UINT nFlags, CPoint pt);
};