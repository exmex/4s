#pragma once


class CTItemUpGaugeDlg : public CTClientUIBase
{
public:
	void SetImage(WORD wTarget, WORD wScroll);
	void SetSkinImage( WORD wSkinImage );
	void SetSkinImageEmpty();
	BOOL Update(DWORD dwTickCount);

	virtual void ShowComponent(BOOL bVisible);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);

public:
	CTItemUpGaugeDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemUpGaugeDlg();

public:
	BOOL m_bShowCancel;
	TImageList* m_pTarget;
	TImageList* m_pScroll;
	TComponent* m_pCancelButton;
	TComponent* m_pCloseButton;

	TGauge*	m_pGauge;
	DWORD	m_dwTick;
};
