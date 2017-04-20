#pragma once

class CTMailAskMoneyDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pRune;
	TComponent*		m_pLuna;
	TComponent*		m_pCron;

	TImageList*		m_pItemIcon;

	LPTMAIL			m_pMail;
	LPTMAIL_SIMPLE	m_pMailSimple;

	TButton*		m_pPayBtn;
	TButton*		m_pReturnBtn;

	BOOL			m_bWaitPayAck;

public:
	void SetInfo(LPTMAIL pMail, LPTMAIL_SIMPLE pMailSimple);
	void Clear();

	LPTMAIL GetMail() const					{ return m_pMail; }
	LPTMAIL_SIMPLE GetMailSimple() const	{ return m_pMailSimple; }

	BOOL IsWaitPayAck() const				{ return m_bWaitPayAck; }
	void AnswerPay(BOOL bPay);

public:
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );

public:
	CTMailAskMoneyDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMailAskMoneyDlg();
};