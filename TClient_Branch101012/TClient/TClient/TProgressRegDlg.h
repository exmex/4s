#pragma once


class CTProgressRegDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pTitle;
	TComponent*		m_pMsg;
	TGauge*			m_pGauge;

	DWORD			m_dwTick;
	DWORD			m_dwPeriod;
	DWORD			m_dwEndGM;

public:
	void Show(
		CString strTitle,
		CString strMsg,
		DWORD dwPeriod,
		DWORD dwEndGM);

	BOOL Update(DWORD dwTickCount);

	virtual void ShowComponent(BOOL bVisible);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt) {}

	virtual BOOL CanWithItemUI();

public:
	CTProgressRegDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTProgressRegDlg();
};
