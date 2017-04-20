#pragma once

class CTTradeDlg;
class CTTradeSlot;

class CTTradeSlot : public CTCtrlListSlot
{
public:
	TImageList*		m_pIcon;
	TComponent*		m_pName;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);
	
public:
	CTTradeSlot() {}
	virtual ~CTTradeSlot() {}
};

class CTTradeDlg : public CTClientUIBase
{
public:
	enum { MAX_ITEM_SLOT = 6 };
	enum { MAX_ITEM_COLUMN = 2 };

protected:
	TComponent*		m_pTitle;
	
	TButton*		m_pTradeBtn;
	TButton*		m_pLockBtn;
	
	TComponent*		m_pTargetName;
	TComponent*		m_pMyName;

	TComponent*		m_pTargetRune;
	TComponent*		m_pTargetLuna;
	TComponent*		m_pTargetCron;

	TEdit*			m_pMyRune;
	TEdit*			m_pMyLuna;
	TEdit*			m_pMyCron;

	CTCtrlList*		m_pTargetList;
	CTCtrlList*		m_pMyList;

	CTClientChar*	m_pMainChar;
	BOOL			m_bLock;

	//////////////////////////////////////////////////////////////////////////
	BOOL			m_bTargetMask;
	BOOL			m_bMyMask;

public:
	void SetTitle(const CString& strTitle);
	CString GetTitle() const;

	void SetTargetName(const CString strName);
	void SetTargetMoney(DWORD dwRune, DWORD dwLuna, DWORD dwCron);
	void SetTargetMask(BOOL bMask);
	void SetMyMask(BOOL bMask);

	DWORD GetMyRune() const;
	DWORD GetMyLuna() const;
	DWORD GetMyCron() const;

	DWORD GetTradeBtnGM() const;
	DWORD GetLockBtnGM() const;

	void SetTradeBtnGM(DWORD dwGM);
	void SetLockBtnGM(DWORD dwGM);

	CTCtrlList* GetTargetList() const	{ return m_pTargetList; }
	CTCtrlList* GetMyList() const		{ return m_pMyList; }

	void NotifyUpdate()					
	{
		m_pTargetList->NotifyUpdate(); 
		m_pMyList->NotifyUpdate(); 
	}

	void SetLock(BOOL bLock);
	void SetTradeEnable(BOOL bEnable);
	
	INT AddItem(BOOL bTarget, CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot);
	CTTradeItem* AddEmptyItem(BOOL bTarget);

	void RemoveItem(BOOL bTarget, INT nIdx);
	void Clear(BOOL bTarget);
	
	UINT GetItemCount(BOOL bTarget) const;
	CTTradeItem* GetItem(BOOL bTarget, INT nIndex);
	
	INT GetItemIdxByInvenPos(BYTE bInven, BYTE bInvenSlot);
	void ChangeItemInvenPos(BYTE bFromInven, BYTE bFromInvenSlot, BYTE bToInven, BYTE bToInvenSlot);

	TEdit* GetCurEdit();

public:
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );
	
	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

	virtual void SwitchFocus(TComponent* pCandidate);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

public:
	CTTradeDlg( TComponent *pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost);
	virtual ~CTTradeDlg();
};

//class CTTradeDlg : public CTClientUIBase
//{
//public:
//	TImageList *m_pTSLOT[TTRADE_HOST_COUNT][MAX_DRAGSLOT];
//	TComponent *m_pTNAME[TTRADE_HOST_COUNT];
//
//	TComponent *m_pTRUNE;
//	TComponent *m_pTLUNA;
//	TComponent *m_pTCRON;
//
//	TEdit *m_pTEDITRUNE;
//	TEdit *m_pTEDITLUNA;
//	TEdit *m_pTEDITCRON;
//
//	TButton *m_pTTRADE;
//	TButton *m_pTLOCK;
//
//public:
//	CTClientItem m_vTTARGETITEM[MAX_DRAGSLOT];
//	WORD m_vTHOSTITEM[MAX_DRAGSLOT];
//
//public:
//	CTClientChar *m_pHost;
//
//public:
//	CTClientItem *FindTItem(
//		BYTE bHostID,
//		BYTE bSlotID);
//
//	void ResetTICON( BYTE bHostID);
//	void Clear();
//	void Lock();
//
//public:
//	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags);
//	virtual void SwitchFocus( TComponent *pCandidate);
//
//	virtual TDROPINFO OnDrop( CPoint point);
//	virtual BYTE OnBeginDrag(
//		LPTDRAG pDRAG,
//		CPoint point);
//	
//	virtual BOOL GetTInfoKey( const CPoint& point, TINFOKEY& outInfo );
//	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );
//
//public:
//	CTTradeDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
//	virtual ~CTTradeDlg();
//};
