#pragma once


class CTCinematicDlg : public CTClientUIBase
{
public:
	virtual void ShowComponent( BOOL bVisible);
	virtual void ResetPosition();

	virtual	void DefaultPosition(
		CPoint *pBASIS,
		BOOL bRestore);

public:
	void ResetTEXT( CString *pTEXT);
	void ShowTSKIP( BYTE bShow);

public:
	CPoint m_vTCOMP_OFFSET[5];
	BYTE m_bTSKIP;

public:
	TButton *m_pTSKIP;
	TList *m_pMLIST;

	TComponent *m_pNPCNAME;
	TComponent *m_pBOTTOM;
	TComponent *m_pTOP;

public:
	CTCinematicDlg( TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCinematicDlg();
};
