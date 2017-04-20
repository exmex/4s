#pragma once


class CTMiniPopupDlg : public CTClientUIBase
{
public:
	class CTButtonMP;
	typedef void (*FP_BTN_ACT)(CTMiniPopupDlg* pPopupDlg, CTButtonMP* pButton);

	class CTButtonMP : public TButton
	{
		friend class CTMiniPopupDlg;

	private:
		CTMiniPopupDlg*	m_pHost;
		INT				m_nIndex;
		FP_BTN_ACT		m_fpAct;
		BOOL			m_bDown;
		DWORD			m_dwUser;
		DWORD			m_dwMenuID;
	
	private:
		virtual void OnLButtonUp(UINT nFlags, CPoint pt);
		virtual void OnLButtonDown(UINT nFlags, CPoint pt);

	public:
		INT GetButtonIndex() const	{ return m_nIndex; }
		DWORD GetUserData() const	{ return m_dwUser; }

	private:
		CTButtonMP(CTMiniPopupDlg* pHost, LP_FRAMEDESC pDesc);
	};
	typedef ::std::vector<CTButtonMP*> ButtonArray;
	
public:
	static const DWORD	FONT_COLOR;

	BOOL				m_bStateMode;
	ButtonArray			m_vButtons;
	
	TComponent*			m_pUpper;
	TComponent*			m_pLower;
	TComponent*			m_pBtnTemplate;

	INT					m_nUpperHeight;
	INT					m_nButtonHeight;
	INT					m_nLastSelIndex;

public:
	void AddButton(const CString& strTitle, DWORD dwMenuID);
	void AddButton(const CString& strTitle, FP_BTN_ACT pBtnFunc, DWORD dwUserData=0);
	void ClearButtons();
	
	void SetButtonColor(INT nIdx, DWORD dwColor);

	BOOL IsButtonEmpty() const			{ return m_vButtons.empty(); }
	INT GetButtonCount() const			{ return (INT)m_vButtons.size(); }
	
	DWORD GetButtonUserData(INT nBtnIdx) const
	{
		return m_vButtons[nBtnIdx]->GetUserData();
	}
	void SelectButton(INT nBtnIdx, BOOL bSelect)
	{
		if( bSelect )
			m_nLastSelIndex = nBtnIdx;

		m_vButtons[nBtnIdx]->Select(bSelect);
	}

	void SetLastSelIndex(INT nIdx)	{ m_nLastSelIndex = nIdx; }
	INT GetLastSelIndex() const		{ return m_nLastSelIndex; }

	INT GetUpperHeight() const		{ return m_nUpperHeight; }
	INT GetButtonHeight()	const	{ return m_nButtonHeight; }

	virtual BOOL CanWithItemUI();

protected:
	void ReFitSize();

public:
	CTMiniPopupDlg(TComponent* pParent, LP_FRAMEDESC pDesc, BOOL bStateMode);
	virtual ~CTMiniPopupDlg();

};