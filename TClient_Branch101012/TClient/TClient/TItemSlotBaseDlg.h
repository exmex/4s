#pragma once


class CTItemSlotBaseDlg : public CTClientUIBase
{
protected:
	int				m_nSlotCount;

	TImageList**	m_pSlotCtrl;
	TComponent**	m_pSlotImage;
	LPTITEM_SLOT	m_ItemSlot;
	TButton*		m_pButtonOK;
	CString*		m_pTitles;
	CString*		m_pToolTips;

	mutable CString	m_strLastError;

private :

	virtual void	UpdateState_ButtonOK();

public:
	virtual BOOL CheckItem(BYTE bSlotID, CTClientItem* pClientItem) const;
	virtual BOOL CheckFrame(DWORD dwFrame) const;
	virtual BOOL SetItem(BYTE bSlotID, LPTITEM pItem, BYTE bInven=T_INVALID, BYTE bInvenSlot=T_INVALID);
	virtual void ClearItem();
	
	virtual CTClientItem* GetLastClientItem(BYTE bSlotID) const;

public:
	int GetSlotCount() const					{ return m_nSlotCount; }
	LPTITEM GetItem(BYTE bSlotID) const 		{ return m_ItemSlot[bSlotID].pItem; }
	BYTE GetLastInvenID(BYTE bSlotID) const 	{ return m_ItemSlot[bSlotID].bInven; }
	BYTE GetLastInvenSlot(BYTE bSlotID) const	{ return m_ItemSlot[bSlotID].bInvenSlot; }
	const CString& GetLastErrorMsg() const		{ return m_strLastError; }
	
public:
	virtual void ShowComponent( BOOL bVisible = TRUE);

	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );
	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTItemSlotBaseDlg(TComponent* pParent, LP_FRAMEDESC pDesc, int nSlotCnt);
	virtual ~CTItemSlotBaseDlg();
};
