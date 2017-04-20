#pragma once

#ifdef ADD_RPS

class CTRpsDlg : public CTClientUIBase
{
public:
	CTRpsDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRpsDlg();
	HRESULT Render(DWORD dwTickCount);
	BOOL CanDisable();
	virtual BOOL CanWithItemUI();

public:
	CTClientObjBase* m_pNPC;
	BYTE m_bSelectMode;
	RPSCARD_TYPE m_ePlayer;
	RPSCARD_TYPE m_eNpc;
	DWORD m_dwRandTick;
	INT m_nRand;
	CSize m_Size;

	TButton* m_pR;
	TButton* m_pP;
	TButton* m_pS;
	TButton* m_p1;
	TButton* m_p2;
	TButton* m_p3;
	TImageList* m_pRPSNpc;
	TImageList* m_pRPSPlayer;
};

#endif