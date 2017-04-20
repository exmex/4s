#pragma once

class CTChatTabButton : public TButton
{
protected:
	static const DWORD FONT_COLOR_NORMAL;		///< 탭이 선택되지 않을을 때의 폰트 색
	static const DWORD FONT_COLOR_PUSH;			///< 탭이 선택됬을때의 폰트 색
	static const DWORD FONT_COLOR_HOVER;		///< 탭위로 커서가 올라왔을때의 폰트 색

	CTChatFrame*	m_pHost;					///< 이 탭이 속한 프레임
	CTChatList*		m_pList;					///< 이 탭과 링크된 리스트
	INT				m_iSlot;					///< 이 탭의 슬롯 번호 ( 이 번호에 따라 탭의 위치가 결정된다 )
	BOOL			m_bSelect;					///< 이 탭이 선택되었는지 여부
	
	DWORD			m_dwTotalTick;				///< 경과 시간
	BOOL			m_bHovering;				///< 탭 위로 커서가 올라왔는지 여부
	BOOL			m_bNewMsg;					///< 해당 탭과 연결된 리스트로 새로운 메시지가 도착했는지 여부

public:
	/// 주어진 슬롯번호에 맞는 탭의 위치정보를 얻는다.
	static void SetTabSlotRect(INT iSlot, const CRect& rcTab);

public:
	/// 탭 초기화. 슬롯을 정하고 리스트와 연결한다.
	void Init(INT iSlot, CTChatList* pList);
	/// 탭을 갱신시킨다. 전달인자는 이전 갱신부터 지금까지 경과한 시간을 의미한다.
	void Update(DWORD dwTick);

	/// 주어진 리스트와 링크한다.
	void LinkWithList(CTChatList* pList);
	/// 슬롯을 정한다.
	void SetSlot(INT iSlot);
	/// 이 탭을 선택하거나 선택해제 한다.
	void SetSelect(BOOL bSel);
	
	/// 새로운 메시지의 도착을 탭에 통보한다.
	void NotifyNewMsg();
	
public:
	CTChatList* GetChatList() const			{ return m_pList; }
	INT GetSlot() const						{ return m_iSlot; }
	BOOL IsOuter() const					{ return m_pList->IsOuter(); }
	BOOL IsSelect() const					{ return m_bSelect; }
	INT GetChatListIndex() const			{ return m_pList->GetIndexInGroup(); }

public:
	virtual void OnMouseMove(UINT nFlags, CPoint pt);

	virtual void OnLButtonUp(UINT nFlags, CPoint pt)			{ TComponent::OnLButtonUp(nFlags,pt); }
	virtual void OnRButtonUp(UINT nFlags, CPoint pt)			{ TComponent::OnRButtonUp(nFlags,pt); }
	virtual void OnRButtonDown(UINT nFlags, CPoint pt)			{ TComponent::OnRButtonDown(nFlags,pt); }
	virtual void OnLButtonDown(UINT nFlags, CPoint pt)			{ TComponent::OnLButtonDown(nFlags,pt); }
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt)		{ TComponent::OnLButtonDblClk(nFlags,pt); }

protected:
	/// 현재 상태에 맞게 폰트 칼라를 재설정한다.
	void ResetFontColor();

public:
	CTChatTabButton::CTChatTabButton(CTChatFrame* pHost, LP_FRAMEDESC pDesc);
};
