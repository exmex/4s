#pragma once

class CTTextPopup : public TFrame
{
public:
	CTTextPopup( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTTextPopup();
	void ClearBlock();
	void SetText( CString& strText );
	void RealignPos();
	HRESULT Render(DWORD dwTickCount);

protected:
	TComponent* m_pTOP;
	TComponent* m_pBOTTOM;
	TComponent* m_pMIDDLE;
	TComponent* m_pTEXT;
	CPoint m_ptTEXTOffset;
	INT m_nHeightBLOCK;
	CSize m_sizeTEXT;
	TCOMP_ARRAY m_vBLOCK;
	TCOMP_ARRAY m_vTEXT;
};