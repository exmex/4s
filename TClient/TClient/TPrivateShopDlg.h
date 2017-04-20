#pragma once

class CTPrivateShopDlg : public CTShopBaseDlg
{
public:
	INT AddItem(CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot);
	CTPrivateShopItem* AddEmptyItem();
	
	void RemoveItem(INT nIdx);
	void Clear();
	
	CTPrivateShopItem* GetCurSelItem();
	UINT GetItemCount() const;
	INT GetItemIdxByInvenPos(BYTE bInven, BYTE bInvenSlot);
	INT GetItemIdxByNetID(BYTE bNetID);
	CTPrivateShopItem* GetItem(INT nIndex);
	
	void ChangeItemInvenPos(BYTE bFromInven, BYTE bFromInvenSlot, BYTE bToInven, BYTE bToInvenSlot);
	INT ReduceItemCount(BYTE bNetID, BYTE bCount);

public:
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point);
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );
	
	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTPrivateShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTPrivateShopDlg();
};

class CTPrivateShopDlgForSell : public CTPrivateShopDlg
{
protected:
	BOOL			m_bSelling;

	TComponent*		m_pShopNameOut;
	TEdit*			m_pShopNameIn;

	TComponent*		m_pWarn;

public:
	BOOL IsSelling() const			{ return m_bSelling; }
	void StartSelling(BOOL bStart);

	void ClearShopName();

	void SetShopName(const CString& strShopName);
	const CString& GetShopName() const;

	void SetWarn(const CString& strWarn);
	const CString& GetWarn() const;

	void Clear();
	TEdit* GetCurEdit();

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);	
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);

	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTPrivateShopDlgForSell(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTPrivateShopDlgForSell();
};







//class CTPrivateShopDlg : public CTClientUIBase
//{
//public:
//	enum Mode
//	{
//		MODE_SELL_READY,
//		MODE_SELL,
//		MODE_BUY,
//		MODE_COUNT
//	};
//
//	enum{ SHOW_ITEM_SLOT_MAX = 4 };
//
//	struct ItemInfo
//	{
//		BYTE			m_bNetID;
//		CTClientItem*	m_pClientItem;
//		BYTE			m_bItemCnt;
//		DWORD			m_dwRune;
//		DWORD			m_dwLuna;
//		DWORD			m_dwCron;
//		BYTE			m_bInven;
//		BYTE			m_bInvenSlot;
//	};
//	typedef std::vector<ItemInfo>	ItemInfoArr;
//
//	struct ItemSlotCtrl
//	{
//		TImageList*		m_pIcon;
//		TComponent*		m_pItemName;
//		TComponent*		m_pItemCntTxt;
//
//		TComponent*		m_pItemCntOut;
//		TComponent*		m_pRuneOut;
//		TComponent*		m_pLunaOut;
//		TComponent*		m_pCronOut;
//
//		TEdit*			m_pItemCntIn;
//		TEdit*			m_pRuneIn;
//		TEdit*			m_pLunaIn;
//		TEdit*			m_pCronIn;
//
//		void ShowComponent(BOOL bShow, BOOL bIn, BOOL bOut)
//		{
//			m_pIcon->ShowComponent(bShow);
//			m_pItemName->ShowComponent(bShow);
//			m_pItemCntTxt->ShowComponent(bShow);
//
//			m_pItemCntIn->ShowComponent(bShow && bIn);
//			m_pRuneIn->ShowComponent(bShow && bIn);
//			m_pLunaIn->ShowComponent(bShow && bIn);
//			m_pCronIn->ShowComponent(bShow && bIn);
//		
//			m_pItemCntOut->ShowComponent(bShow && bOut);
//			m_pRuneOut->ShowComponent(bShow && bOut);
//			m_pLunaOut->ShowComponent(bShow && bOut);
//			m_pCronOut->ShowComponent(bShow && bOut);
//		}
//	};
//
//public:
//	
//
//protected:
//	TComponent*		m_pWarn;
//	TComponent*		m_pOkButton;
//
//	TEdit*			m_pShopNameIn;
//	TComponent*		m_pShopNameOut;
//
//	ItemSlotCtrl	m_vSlotCtrls[SHOW_ITEM_SLOT_MAX];
//
//	TList*			m_pList;
//	ItemInfoArr		m_vItemInfos;
//
//	Mode			m_Mode;
//	BOOL			m_bNeedUpdate;
//	INT				m_nPrvListTop;
//	INT				m_nPrvListSel;
//
//public:
//	void AddItem(CTClientItem* pItem, BYTE bInven, BYTE bInvenSlot);
//	ItemInfo* AddEmptyItem();
//	void RemoveItem(INT nIdx);
//	void Clear();
//	ItemInfo* GetCurSelItem();
//	UINT GetItemCount() const;
//	INT GetItemIdxByInvenPos(BYTE bInven, BYTE bInvenSlot);
//	INT GetItemIdxByNetID(BYTE bNetID);
//	ItemInfo* GetItem(INT nIndex);
//	void ChangeItemInvenPos(BYTE bFromInven, BYTE bFromInvenSlot, BYTE bToInven, BYTE bToInvenSlot);
//	INT ReduceItemCount(BYTE bNetID, BYTE bCount);
//	void ClearInOutText();
//	void SetMode(Mode m);
//	void SetShopName(const CString& name);
//	TEdit* GetCurEdit();
//	CTClientItem* GetClientItem(BYTE bInven, BYTE bInvenSlot) const;
//	void MaintainSelection();
//	Mode GetMode() const { return m_Mode; }
//	void NotifyUpdate() { m_bNeedUpdate = TRUE; }
//	
//public:
//	void RequestStartSell();
//	void StartSell(const CString& strShopName);
//
//	void RequestStopSell();
//	void StopSell();
//
//	void RequestItemBuy(CTClientChar* pTarget, BYTE bCount);
//
//public:
//	virtual void ShowComponent(BOOL bVisible = TRUE);
//	virtual HRESULT Render(DWORD dwTickCount);
//	virtual void SwitchFocus(TComponent* pCandidate);
//	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
//	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);	
//	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);
//
//	virtual BOOL GetTInfoKey( const CPoint& point, TINFOKEY& outInfo );
//	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );
//	virtual TDROPINFO OnDrop( CPoint point );
//	virtual BYTE OnBeginDrag( LPTDRAG pDRAG, CPoint point );
//
//protected:
//	void UpdateCtrls();
//
//public:
//	CTPrivateShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc, BOOL bSell);
//	virtual ~CTPrivateShopDlg();
//};
