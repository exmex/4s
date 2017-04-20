#pragma once

class CTMailSendDlg : public CTClientUIBase
{
public:
	enum { MAX_TAB_IDX = 6 };

protected:
	TEdit*			m_pReceiver;
	TEdit*			m_pTitle;
	TMultiLineEdit*	m_pMessage;
	
	TButton*		m_pSndMoneyBtn;
	TButton*		m_pAskMoneyBtn;

	TEdit*			m_pSendRune;
	TEdit*			m_pSendLuna;
	TEdit*			m_pSendCron;

	TComponent*		m_pMyRune;
	TComponent*		m_pMyLuna;
	TComponent*		m_pMyCron;

	TComponent*		m_pCostTxt;
	TButton*		m_pSendBtn;

	BYTE			m_nInvenID;
	BYTE			m_nSlotID;
	CTClientItem*	m_pItem;
	TImageList*		m_pItemIcon;

	INT				m_nCurTabIndex;
	TEdit*			m_vTabSortEdit[MAX_TAB_IDX];
public:
	BYTE			m_bInvenIDByCashItem;
	BYTE			m_bSlotIDByCashItem;
	BYTE			m_bSendByCashItem;
public:
	CString GetReceiver() const;
	CString GetTitle() const;
	CString GetMessage() const;

	DWORD GetRune() const;
	DWORD GetLuna() const;
	DWORD GetCron() const;

	BYTE GetInvenID() const;
	BYTE GetInvenSlot() const;

	CString GetCostTxt(BYTE bMailType);
	BYTE GetCurrentMailType();
	TEdit* GetCurEdit();

	void SetReceiver(const CString& strReceiver);
	void SetTitle(const CString& strTitle);

	void SetItem(CTClientItem* pItem, BYTE nInvenID=INVEN_NULL, BYTE nInvenSlot=0);
	void ChangeItemPos(BYTE nInvenID, BYTE nInvenSlot);

	TMultiLineEdit* GetMessageEdit() const { return m_pMessage; }
	TEdit* GetTitleEdit() const { return m_pTitle; }

	void SetUsingByCash( BYTE bInvenID, BYTE bSlotID);
	void ClearUsingByCash();
public:
	virtual HRESULT Render( DWORD dwTickCount );
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void SwitchFocus(TComponent* pCandidate);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );

	virtual TDROPINFO OnDrop( CPoint point);
	virtual BYTE OnBeginDrag( LPTDRAG pDRAG, CPoint point );

public:
	CTMailSendDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMailSendDlg();
};