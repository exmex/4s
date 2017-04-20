#pragma once

class CTNumberEdit : public TEdit
{
protected:
	INT m_nValidCnt;
	BOOL m_bUseLimit;
	INT m_nMin;
	INT m_nMax;
	CString m_strDefault;

public:
	CString m_strWarning;

	void SetUseLimit( INT nMin, INT nMax );
	void SetDefault( CString );
	void SetValidCount( INT nValidCnt );
	DWORD GetValue() const;

	virtual void ClearText();
	virtual void SetText(const CString& txt);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLoseFocus( TComponent* pSetFocus );
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);	

protected:
	void CheckAndFixString(CString& txt);

public:
	CTNumberEdit(TComponent *pParent, LP_FRAMEDESC pDesc, INT nValidCnt);
	virtual ~CTNumberEdit();

};