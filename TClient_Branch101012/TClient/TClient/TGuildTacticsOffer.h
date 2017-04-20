#pragma once

class CTGuildTacticsOffer : public CTClientUIBase
{
protected:
	INT m_nMode; //0:View 1:New 2:Modify

public:
	TComponent* m_pViewArticleTitle;
	TComponent* m_pViewGuildName;
	TComponent* m_pViewHornor;
	TComponent* m_pViewPeriod;
	TComponent* m_pViewLevelMin;
	TComponent* m_pViewLevelMax;
	TComponent* m_pViewRune;
	TComponent* m_pViewLuna;
	TComponent* m_pViewCron;
	TList* m_pViewContent;
	TButton* m_pBtnOK;
	TButton* m_pBtnCancel;

	TEdit* m_pEditArticleTitle;
	CTNumberEdit* m_pEditHornor;
	CTNumberEdit* m_pEditPeriod;
	CTNumberEdit* m_pEditLevelMin;
	CTNumberEdit* m_pEditLevelMax;
	CTNumberEdit* m_pEditRune;
	CTNumberEdit* m_pEditLuna;
	CTNumberEdit* m_pEditCron;
	TMultiLineEdit* m_pEditContent;

	DWORD m_dwArticleID;
	TEdit* GetCurEdit();

	void ResetViewInfo( CTGuildCommander::GuildTacticsOffer& );
	void ResetModifyInfo( CTGuildCommander::GuildTacticsOffer& );
	void ResetNewEdit();
	virtual void ShowComponent( BOOL bVisible);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	TEdit* GetCurEditLink();

	INT GetMode();

public:
	CTGuildTacticsOffer( TComponent* pParent, LP_FRAMEDESC pDesc);
	~CTGuildTacticsOffer();
};