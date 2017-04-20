#pragma once


class CTPopupMenu : public CTClientUIBase
{
public:
	TCOMP_ARRAY m_vTPANNEL;
	TCOMP_ARRAY m_vTMENU;

	TComponent *m_pTBACK;
	TComponent *m_pTEND;
	TButton *m_pTBUTTON;

	CRect m_vTBUTTON;
	CRect m_vTBACK;

	INT m_nSELECT;

public:
	void ClearMENU();
	void AddMENU(
		DWORD dwTitleID,
		TCOMMAND dwMenuID);

	void AddMENU(
		CString strTitle,
		TCOMMAND dwMenuID);

	virtual BOOL CanWithItemUI();
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);

public:
	CTPopupMenu( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTPopupMenu();
};
