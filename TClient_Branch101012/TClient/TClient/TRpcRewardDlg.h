#pragma once

#ifdef ADD_RPS

class CTRpsRewardDlg : public CTClientUIBase
{
public:
	CTRpsRewardDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRpsRewardDlg();
	void SetReward( BYTE bGameType, BYTE bWinCount );
	void ShowComponent(BOOL bVisible = 1 );
	BOOL CanDisable();

public:
	TImageList* m_pITEM0;
	TImageList* m_pITEM1;
	TImageList* m_pITEM2;
	TImageList* m_pITEM3;
	TComponent* m_pITEMNAME0;
	TComponent* m_pITEMNAME1;
	TComponent* m_pITEMNAME2;
	TComponent* m_pITEMNAME3;
	TComponent* m_pGold0;
	TComponent* m_pSilver0;
	TComponent* m_pCooper0;
	TComponent* m_pGold1;
	TComponent* m_pSilver1;
	TComponent* m_pCooper1;
	BOOL m_bClose;
	TButton* m_pBtnTry;
	TButton* m_pBtnGiveup;
	TComponent* m_pTEXT;
};

#endif