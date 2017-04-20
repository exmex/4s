#pragma once


// CTPathWndBase 대화 상자입니다.

class CTPathWndBase : public CDialog
{
	DECLARE_DYNAMIC(CTPathWndBase)

public:
	CTQuestPathDoc* GetDocument() const;

public:
	virtual void UpdateDATA(
		int nType,
		LPARAM lParam) {};

	virtual void ShowWnd( BYTE bShow);
	virtual void UpdateTMAP() {};

public:
	CTPathWndBase( UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CTPathWndBase();

protected:
	DECLARE_MESSAGE_MAP()
};
