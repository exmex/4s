#pragma once

class CTUpItemSellDlg :	public CTClientUIBase
{
public:
	enum
	{
		MODE_SELL = 0,
		MODE_DROP,
		MODE_TOTAL
	};

	TComponent*		m_pMSG;
	TComponent*		m_pTXT;
	TEdit*			m_pID;
	CString			m_strSecurityCode;
	
protected:
	CTClientItem*	m_pItem;
	TImageList*		m_pItemList;
	BYTE			m_bMode;

public:
	void	Clear();
	void	SetClientItem( CTClientItem* pItem );

	TEdit*	GetCurEdit();
	
	void	SetMode( BYTE bMode );
	BYTE	GetMode() {		return m_bMode;	}


public:
	virtual void ShowComponent(BOOL bShow = TRUE);
	virtual BOOL CanWithItemUI();
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );

public:
	CTUpItemSellDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTUpItemSellDlg(void);
};
