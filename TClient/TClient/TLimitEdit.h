#pragma once

class CTLimitEdit : public TEdit
{
protected:
	INT m_nValidByteSize;

public:
	void SetValidByteSize( INT nValidByteSize );
	void NotifyError();

	virtual void ClearText();
	virtual void SetText(const CString& txt);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLoseFocus( TComponent* pSetFocus );
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);	

protected:
	void CheckAndFixString(CString& txt);

public:
	CTLimitEdit(TComponent *pParent, LP_FRAMEDESC pDesc, INT nValidCnt);
	virtual ~CTLimitEdit();

};