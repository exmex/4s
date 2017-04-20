#pragma once

class CTShopBaseDlg;
class CTShopSlot;

class CTShopSlot : public CTCtrlListSlot
{
public:
	TImageList*		m_pIcon;
	TComponent*		m_pName;
	TComponent*		m_pRune;
	TComponent*		m_pLuna;
	TComponent*		m_pCron;
	TComponent*		m_pMoney;
	TComponent*		m_pPoint;
	TComponent*		m_pMark;
	BOOL			m_bMark;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);
	virtual void Update(CTCtrlListItem* pItem);
	
public:
	CTShopSlot() :
		m_pName(NULL),
		m_pRune(NULL),
		m_pLuna(NULL),
		m_pCron(NULL),
		m_pMoney(NULL),
		m_pPoint(NULL),
		m_pIcon(NULL),
		m_pMark(NULL),
		m_bMark(FALSE)
		{}
	virtual ~CTShopSlot() {}
};

class CTShopBaseDlg : public CTClientUIBase
{
public:
	enum { MAX_ITEM_SLOT = 12 };
	enum { MAX_ITEM_COLUMN = 2 };

protected:
	TComponent*		m_pTitle;
	TComponent*		m_pDownTitle;
	
	TButton*		m_pOk;
	TButton*		m_pCancel;
	
	CTCtrlList*		m_pList;

public:
	void SetTitle(const CString& strTitle, CString strDownTitle="");
	CString GetTitle() const;

	void SetDownTitle(const CString& strTitle);
	CString GetDownTitle() const;

	void SetOkButtonTitle(const CString& strTitle);

	DWORD GetOkGM() const;
	DWORD GetCancelGM() const;

	void SetOkGM(DWORD dwGM);
	void SetCancelGM(DWORD dwGM);

	CTCtrlList* GetList() const		{ return m_pList; }
	void NotifyUpdate()				{ m_pList->NotifyUpdate(); }

public:
	virtual void SwitchFocus(TComponent* pCandidate);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

public:
	CTShopBaseDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTShopBaseDlg();
};
