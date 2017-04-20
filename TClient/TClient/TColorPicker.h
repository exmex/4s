#pragma once

class CTColorPicker : public CTClientUIBase
{
public:
	CTColorPicker( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTColorPicker();
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual HRESULT Render( DWORD dwTickCount );
	void StartRandAnimation();
	void StopRandAnimation();
	void SetBindTarget( CTClientUIBase* pBindTarget );
	void SetCanSelect( BOOL bCanSelect );
	void StartProgress();
	void SelectColor( WORD wColor );

public:
	BOOL m_bCanSelectColor;
	CTClientUIBase* m_pBindTarget;
	BOOL m_bRandomPickingAnimation;
	DWORD m_dwRandomPickingTick;
	WORD m_wSelectedColor;
	TImageList* m_pTIcon[TCOLOR_COUNT];
	TComponent* m_pText;
};