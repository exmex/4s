#pragma once

class CTHelpFrame : public CTClientUIBase
{
public:
	CTHelpFrame( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTHelpFrame();

	void SetHelp( DWORD dwHelpID );
	void SetPage( WORD wPage );
	void ClearHelp();

	void LeftPage();
	void RightPage();
	void NextHelp();
	void PrevHelp();

	void OnQuestEvent( THELPLINK_TRIGGER, DWORD dwQuestID );

	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL CanWithItemUI();
	void ShowComponent(BOOL bVisible );
	void OnMouseMove(UINT nFlags, CPoint pt);
	HRESULT Render(DWORD dwTickCount);

protected:
	LPTHELP m_pCurrentHelp;
	WORD m_wCurrentPage;
	TComponent* m_pTITLE;
	TList* m_pTEXTLIST;
	TImageList* m_pImage;
	TButton* m_pLeftBtn;
	TButton* m_pRightBtn;
	TButton* m_pUpBtn;
	TButton* m_pDownBtn;
	BOOL m_bShowBtn;
};