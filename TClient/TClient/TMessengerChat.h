#pragma once

class CTMessengerChat : public CTClientUIBase
{
public:
	static LP_FRAMEDESC		ms_ChatFrameDesc;

protected:
	static VTMSGCHATARRAY		ms_FreeChats;
	static TMAPMSGTOCHAT		ms_TmsActChats;
	static BOOL				ms_bShow;

public:
	/// 새로운 메신저 채팅창을 생성한다.
	static CTMessengerChat* NewInstance(DWORD dwTmsID = 0);
	/// 기존의 메신저 채팅창을 제거한다.
	static BOOL DeleteInstance(CTMessengerChat* pChat);
	/// 현재 떠 있는 모든 메신저 채팅창을 제거한다.
	static void DeleteAllInstance();

	/// 주어진 TMS ID 를 가진 채팅창을 얻는다.
	static CTMessengerChat* GetChat(DWORD dwTmsID);
	/// 채팅 메시지를 전송한다.
	static BOOL DoChat(CTMessengerChat* pChat, const CString& strMsg);
	/// 주어진 채팅창에서의 해당 타겟 퇴장을 처리한다.
	static BOOL OnOutChatTarget(CTMessengerChat* pChat, const CString& strOutTarget);

	/// 현재 활성 창이 보이는지 여부를 얻는다.
	static BOOL IsAllVisible()	{ return ms_bShow; }
	/// 모든 활성 창을 보이거나 안보이게 한다.
	static void ShowAll(BOOL bShow);
	
	/// 현재 활성화된 채팅창을 얻는다.
	static CTMessengerChat* GetCurMsgChat();
	/// 모든 채팅창 중에서 현재 활성화된 에디터를 얻는다.
	static TEdit* GetCurMsgChatEdit();
    
	/// P2P 형식의 채팅창중 해당 타겟을 가진 창을 얻는다.
	static CTMessengerChat* FindInP2PByTarget(const CString& strTarget);

protected:
	CTClientGame*			m_pGame;
	CTMessengerCommander*	m_pCmd;
	
	DWORD					m_dwTmsID;
	CString					m_strMyName;
	MCTARGET_ARRAY			m_vChatTargets;

	TComponent*				m_pChatTitle;
	TButton*				m_pImeBtn;
	TButton*				m_pInviteBtn;
	//TButton*				m_pSendBtn;
	TButton*				m_pCloseBtn;
	TList*					m_pChatList;
	TList*					m_pChatMembers;
	TImageList*				m_pMyFace;
	//TImageList*				m_pTargetFace;
	//TComponent*				m_pTargetName;
	TMultiLineEdit*			m_pChatEdit;

public:
	/// 현재 포커스된 에디트 컨트롤을 얻는다.
	TEdit* GetCurEdit();

	/// 타겟들을 설정한다.
	void SetTargets(LPMCTARGET_ARRAY pTargets);
	/// 타겟을 하나 설정한다.
	void SetTarget(LPMSGCHAT_TARGET pTarget);
	/// 타겟을 추가한다.
	void AddTarget(LPMSGCHAT_TARGET pTarget);
	/// 타겟을 하나 제거한다.
	BOOL RemoveTarget(const CString& strTargetName);
	/// 전체 타겟을 제거한다.
	void RemoveAllTargets();

	/// 변경된 타겟 정보를 반영시킨다.
	void UpdateTarget();

	/// 타겟의 수를 얻는다.
	INT GetTargetCount() const					{ return (INT)m_vChatTargets.size(); }
	/// 주어진 인덱스의 타겟을 얻는다.
	LPMSGCHAT_TARGET GetTarget(INT nIdx) const	{ return const_cast<MSGCHAT_TARGET*>(&m_vChatTargets[nIdx]); }
	/// 타겟 배열을 얻는다.
	LPMCTARGET_ARRAY GetTargets() const			{ return const_cast<LPMCTARGET_ARRAY>(&m_vChatTargets); }
	
	/// 이름으로 타겟정보를 얻는다.
	LPMSGCHAT_TARGET FindTargetByName(const CString& strName);

	/// 채팅 타이틀을 설정한다.
	void SetChatTitle(const CString& strTarget, UINT nCnt);

	DWORD GetTmsID() const						{ return m_dwTmsID; }
	const CString& GetMyName() const			{ return m_strMyName; }

	/// 에디터에 입력된 내용에 대한 전송을 시도한다.
	void TryChatMsgInEdit();
	/// 수신된 채트 메시지를 출력한다.
	void OnChatMsg(const CString& strSender, const CString& strMessage);

	/// 친구 목록에서 선택된 친구를 초대한다.
	BOOL InviteSelectedFriendInList();

public:
	virtual HRESULT Render(DWORD dwTickCount);
	
	virtual void EnableComponent( BOOL bEnable = TRUE);
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags);

protected:
	void NotifyImeLocalModeChange();
	
public:
	CTMessengerChat(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMessengerChat();
};












