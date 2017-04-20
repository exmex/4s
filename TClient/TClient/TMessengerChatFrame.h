#pragma once

class CTMessengerChatFrame : public CTClientUIBase
{
protected:
	CTMessengerChat* m_pHost;

	TComponent*		m_pTitle;
	TButton*		m_pImeBtn;
	TButton*		m_pInviteBtn;
	TButton*		m_pSendBtn;
	TButton*		m_pCloseBtn;
	TList*			m_pChatList;
	TList*			m_pChatMembers;
	TImageList*		m_pMyFace;
	TImageList*		m_pTargetFace;

	TMultiLineEdit*	m_pChatEdit;

	BOOL			m_bMyFaceID;
	BOOL			m_bTargetFaceID;

public:
	void Clear();

	void SetTitle(const CString& strTarget, UINT nCnt=1);

	void SetMyFace(BYTE bFaceID);
	void SetTargetFace(BYTE bFaceID);

	void ClearTargetList();
	void AddTargetToList(const CString& strTargetName);

	void ClearEditTxt();
	CString GetEditTxt() const;
	void SetEditTxt(const CString& strTxt);
	
	void ClearChatList();
	void AddChatMsgToList(const CString& strSender, const CString& strMsg);

	void SetChatLines(const VECTORSTRING& inLINES, const VECTORDWORD& inCOLORS, const TLLINESETTING_ARRAY2D& inSETTINGS);
	void GetChatLines(VECTORSTRING& outLINES, VECTORDWORD& outCOLORS, TLLINESETTING_ARRAY2D& outSETTINGS);

public:
	virtual HRESULT Render(DWORD dwTickCount);
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags);

protected:
	void NotifyImeLocalModeChange();

public:
	CTMessengerChatFrame(CTMessengerChat* pHost, TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMessengerChatFrame();
};