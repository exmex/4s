#pragma once

class CTWelcome : public CTClientUIBase
{
public:
	CTWelcome( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTWelcome();

	void ShowComponent(BOOL bVisible);
	void OnLButtonUp(UINT nFlags, CPoint pt);

public:
	TImageList* m_pID_CTRLINST_BACK;
	TComponent* m_pID_CTRLINST_TEXT1;
	TComponent* m_pID_CTRLINST_TEXT2;
	TComponent* m_pID_CTRLINST_BTN_START;
	TComponent* m_pID_CTRLINST_BTN_SKIP;
	BYTE m_bMode;
};