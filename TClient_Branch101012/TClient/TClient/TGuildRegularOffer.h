#pragma once

class CTGuildRegularOffer : public CTClientUIBase
{
protected:
	INT m_nMode; //0:View 1:Edit 2:Modify

public:
	TComponent* m_pViewArticleTitle;
	TComponent* m_pViewGuildName;
	TComponent* m_pViewLevelMin;
	TComponent* m_pViewLevelMax;
	TList* m_pViewContent;
	TButton* m_pBtnOK;
	TButton* m_pBtnCancel;

	TEdit* m_pEditArticleTitle;
	CTNumberEdit* m_pEditLevelMin;
	CTNumberEdit* m_pEditLevelMax;
	TMultiLineEdit* m_pEditContent;

	DWORD m_dwArticleID;
	TEdit* GetCurEdit();

	void ResetViewInfo( CTGuildCommander::GuildOffer& );
	void ResetModifyInfo( CTGuildCommander::GuildOffer& );
	void ResetNewEdit();
	virtual void ShowComponent( BOOL bVisible);
	INT GetMode();
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	TEdit* GetCurEditLink();
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);

public:
	CTGuildRegularOffer( TComponent* pParent, LP_FRAMEDESC pDesc);
	~CTGuildRegularOffer();
};