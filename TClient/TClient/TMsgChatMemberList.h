#pragma once

/*class CTMsgChatMemberList : public ITInnerFrame
{
public:
	struct Item
	{
		BOOL bNewMsg;
		CTMessengerChat* pChat;
	};
	typedef std::vector<Item> ItemArray;

	enum { MAX_LINE = 3 };

protected:
	TScroll*	m_pScroll;
	TComponent*	m_vLine[MAX_LINE];

	ItemArray m_vItems;

public:
	void Add(CTMessengerChat* pChat, BOOL bNewMsg);
	void NotifyNewMsg(INT nIndex);
	
	void Remove(INT nIndex);
	void Clear();

	BOOL IsEmpty() const;
	INT GetCount() const;

	CTMessengerChat* GetChat(INT nIndex) const;

	INT FindByChat(CTMessengerChat* pChat);
	INT FindByTargetName(const CString& strTarget);

	void Update(DWORD dwTickCount);

public:
	virtual HRESULT Render(DWORD dwTickCount);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);

	virtual void RequestInfo() {}
	virtual void ResetInfo() {}

public:
	CTMsgChatMemberList(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMsgChatMemberList();
};*/