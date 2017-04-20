#pragma once

//
//class CTRentSkillDlg : public CTShopBaseDlg
//{
//protected:
//
//	TComponent*		m_pHelp;
//	TSHOP_TYPE		m_eShopType;
//
//public:
//	LPTOPENBYCASH m_pOpenByCash;
//	void SetOpenByCash( LPTOPENBYCASH );
//	void ClearOpenByCash();
//
//public:
//	TSHOP_TYPE GetType() const { return m_eShopType; }
//	void Clear();
//
//public:
//	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
//	
//	virtual TDROPINFO OnDrop(CPoint point);
//	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);
//
//public:
//	CTRentSkillDlg(TComponent *pParent, LP_FRAMEDESC pDesc, TSHOP_TYPE eShopType);
//	virtual ~CTRentSkillDlg();
//};
//

class CTShopBaseDlg;
class CTRentSlot;

class CTRentSkillSlot : public CTCtrlListSlot
{
public:
	TImageList*		m_pSkill;
	TComponent*		m_pName;
	TComponent*		m_pRune;
	TComponent*		m_pLuna;
	TComponent*		m_pCron;
	TComponent*		m_pMoney;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);
	
public:
	CTRentSkillSlot() {}
	virtual ~CTRentSkillSlot() {}
};

class CTRentSkillDlg : public CTClientUIBase
{
public:
	enum { MAX_ITEM_SLOT = 12 };
	enum { MAX_ITEM_COLUMN = 2 };

protected:
	TComponent*		m_pTitle;
	TComponent*		m_pDownTitle;
	TButton*		m_pOK;
	TButton*		m_pCancel;
	CTCtrlList*		m_pList;

public:
	void SetTitle(const CString& strTitle);
	CString GetTitle() const;

	DWORD GetCancelGM() const;
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
	
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
//	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );
	
	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTRentSkillDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTRentSkillDlg();
};
