#pragma once

class CTSelectCountry : public CTClientUIBase
{
public:
	CTSelectCountry( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTSelectCountry();
	virtual void ShowComponent(BOOL bVisible = 1);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);

public:
	TButton* m_pOK;
	BYTE m_bSelectedCountry;
	BYTE m_bCountryLeft;
	BYTE m_bCountryRight;
	TImageList* m_pLeftImage;
	TImageList* m_pRightImage;
	TList* m_pLeftDesc;
	TList* m_pRightDesc;
	TComponent* m_pLeftTitle;
	TComponent* m_pRightTitle;
};