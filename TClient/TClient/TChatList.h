#pragma once

class CTChatListItem : public TListItem
{
public:
	CString m_strName;
};

class CTChatList : public TList
{
protected:
	static CRect		s_rcDragZone;

public:
	static BYTE			m_bUpdateScroll;

protected:
	BOOL				m_bOuter;
	UINT				m_IndexInGroup;
	CString 			m_strTitle;
	DWORD				m_dwChatFlag;
	BOOL				m_bEnableDrag;
	BOOL				m_bMoveDragging;
	BOOL				m_bResizeDragging;
	INT				m_nResizeDir;
	CPoint				m_ptDragOffset;
	
	CTChatFrame*		m_pHost;
	TComponent*			m_pOuterHost;

	BOOL				m_bHideBackground;

	BOOL				m_bNotifyMessage;
	BOOL				m_bAutoScroll;

	TCURSOR_TYPE m_eCursorType;

public:
	TButton*			m_pSizeBarUP;
	TButton*			m_pSizeBarDOWN;

public:
	CPoint			m_vCompOffset;
	BYTE				m_bBasisPoint;

public:
	void Init(UINT iIndex, const CString& strTitle, DWORD dwFlag, const CRect& rc, LP_FRAMEDESC pDescFastDown);

	void StartDrag(const CPoint& pt);
	void SetDragEnable(BOOL bEnable);

	void SetOuter(BOOL bOuter);

	INT AddMsgLine(
		const CString& strTYPENAME,
		const CString& strLINE,
		DWORD dwColor,
		CString strSenderName,
		DWORD dwSenderID,
		DWORD dwChatFlag,
		INT nHMargine=0 );

	void SetHideBackground(BOOL bHide);

	BOOL GetApplyCursor( LPTCURSOR_TYPE pbCursor );

public:
	static void SetDragZone(const CRect& rc)	{ s_rcDragZone = rc; }

	BOOL IsOuter() const						{ return m_bOuter; }

	UINT GetIndexInGroup() const				{ return m_IndexInGroup; }
	const CString& GetTitle() const				{ return m_strTitle; }

	void SetChatFlag(DWORD dwFlag)				{ m_dwChatFlag = dwFlag; }
	DWORD GetChatFlag() const					{ return m_dwChatFlag; }
	BOOL CheckChatFlag(DWORD dwChatFlag) const	{ return (BOOL)(m_dwChatFlag & dwChatFlag); }

	BOOL IsHideBackground() const				{ return m_bHideBackground; }

public:
	virtual BOOL HitTest( CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void MoveComponent( CPoint pt );
	void ResetPosition();
	void SetHeight( INT nHeight );

protected:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	
	virtual HRESULT Render( DWORD dwTickCount );

protected:
	void CheckDragZone();
	
public:
	CTChatList(CTChatFrame* pHost, LP_FRAMEDESC pDesc, TComponent* pOuterParent);
	virtual ~CTChatList();
};
