#pragma once

class CTMailRecvDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pSender;
	TComponent*		m_pTitle;
	
	TComponent*		m_pRune;
	TComponent*		m_pLuna;
	TComponent*		m_pCron;
	
	TButton*		m_pReplyBtn;
	TButton*		m_pDeleteBtn;
	TButton*		m_pGetItemBtn;

	TList*			m_pMessage;
	
	LPTMAIL			m_pMail;
	LPTMAIL_SIMPLE	m_pMailSimple;

	TImageList*		m_vItemIcons[TMAIL_ITEM_MAX];

public:
	void SetInfo(LPTMAIL pMail, LPTMAIL_SIMPLE pMailSimple);
	void Clear();
	void ClearAppendedItem();

	LPTMAIL GetMail() const					{ return m_pMail; }
	LPTMAIL_SIMPLE GetMailSimple() const	{ return m_pMailSimple; }

public:
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );

	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);

public:
	CTMailRecvDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMailRecvDlg();
};