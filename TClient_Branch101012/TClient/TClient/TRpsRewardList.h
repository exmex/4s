#pragma once

class CTRpsRewardList : public CTClientUIBase
{
public:
	CTRpsRewardList( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRpsRewardList();
	void OnNotify(DWORD from, WORD msg, LPVOID param);
	void UpdateList( int top );
	void UpdateElement( LPTRPS pTRPS, int index);
	void UpdateTitle( LPTRPS pTRPS, int index);
	void ShowComponent(BOOL bVisible=1);
	BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);

public:
	struct Element
	{
		TImageList* m_pITEM0;
		TImageList* m_pITEM1;
		TComponent* m_pTEXT0;
		TComponent* m_pTEXT1;
		TComponent* m_pGOLD;
		TComponent* m_pSILVER;
		TComponent* m_pCOOPER;
	};

	Element m_vComp[9];
	TScroll* m_pSCROLL;
	TComponent* m_pCOL[3];
	TComponent* m_pROW[3];
	int m_nListTop;
	int m_nListMax;
};