#pragma once

class CTMailListFrame : public ITInnerFrame
{
public:
	enum { MAX_LINE = 7 };

	struct Item
	{
		BOOL			m_bShow;

		TButton*		m_pButton;
		TComponent*		m_pTypeTxt;
		TComponent*		m_pSenderTxt;
		TComponent*		m_pTitleTxt;
		TComponent*		m_pTimeTxt;

		LPTMAIL_SIMPLE	m_pMail;

		void ShowComponent(BOOL bShow);
		void SetMail(LPTMAIL_SIMPLE pMail);
	};

	struct Mail
	{
		LPTMAIL_SIMPLE	pSimple;
		LPTMAIL			pInfo;
	};
	typedef std::vector<Mail>	MailArray;

	TComponent* m_pMailDlgTitle;
	WORD m_wTotalCount;
	WORD m_wCurPage;

protected:
	TScroll*	m_pScroll;
	TButton*	m_pSendBtn;
	TComponent* m_pPage[7];

	INT			m_nSelectIdx;
	INT			m_nPrvScrPos;
	BOOL		m_bNeedUpdate;
	Item		m_vItems[MAX_LINE];

	MailArray	m_vMails;

public:
	void AddMail(LPTMAIL_SIMPLE pMail);
	void SetMail(INT nIdx, LPTMAIL pMail);
	void SortMail();

	void RemoveMail(INT nIdx);
	void ClearMail();
	
	LPTMAIL GetMail(INT nIdx) const;
	LPTMAIL_SIMPLE GetMailSimple(INT nIdx) const;
	
	void SetSelectedIndex(INT nIdx);
	INT GetSelectedIndex() const;
	INT FindIndexByPostID(DWORD dwPostID) const;

	BOOL IsEmpty() const;
	UINT GetCount() const;

	void ViewMail(INT nIdx);
	BOOL IsNewMail() const;

	void NotifyUpdate();
	void Update();
	void SetInfo(WORD wTotalCount, WORD wNotReadCount, WORD wPage);

public:
	virtual void RequestInfo()	{}
	virtual void ResetInfo()	{}

	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);

public:
	CTMailListFrame(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMailListFrame();
};