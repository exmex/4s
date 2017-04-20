#pragma once

class CTChatFrame : public CTClientUIBase
{
public:
	enum PopupEnum
	{
		POPUP_CHATTYPE,
		POPUP_CHATFLAG_ALL,
		POPUP_CHATFLAG_NORMAL,
		POPUP_LATEST_WHISPER,
		POPUP_COUNT,
	};

public:
	static const LONG		SNAP_REACT_VALUE;					///< 채팅 리스트의 스냅 반응 거리
	static const CHAR		CHAT_CMD_MARK;						///< 채팅 커맨드 마크
	static const DWORD		CHAT_FLAGS[TCHAT_FLAG_COUNT];		///< 채팅 플래그 배열
	static const DWORD		CHAT_FLAG_STRINGS[TCHAT_FLAG_COUNT];///< 채팅 플래그 이름 배열
	static const DWORD		DEFAULT_CHAT_FLAGS[];				///< 리스트에 배정될 기본 채팅 플래그
	static const BYTE		CHAT_SHOW_NEWMSG[];					///< 새로운 채팅 메시지의 도착 여부를 알릴지 여부
	static const INT		POPUP_TO_LIST[];					///< 팝업 인덱스 -  리스트 인덱스 테이블
	static const DWORD		CHAT_MODE_CMD[];					///< 채팅 모드 명령

protected:
	CTClientGame*			m_pMainGame;						///< 메인 게임 프레임

	TComponent*				m_pListTemplate;					///< 채팅 리스트 생성을 위한 템플릿 객체
	TComponent*				m_pListTemplate2;					///< 공지 리스트 생성을 위한 템플릿 객체
	
	CTChatList*				m_vLists[TCHAT_SET_COUNT];			///< 채팅 리스트 배열
	CRect					m_rcDefaultList;					///< 채팅 리스트의 기본 위치및 크기

	CTChatList*				m_pNotifyList;						///< 운영자 공지 메시지 전용 리스트
	CTChatList*				m_pUpgradeShowList;				///< 강화 성공 메시지 전용 리스트
	DWORD					m_dwNotifyTime;						///< 공지 사항을 화면에 표시할 남은 시간
	
	CTChatTabButton*		m_vTabs[TCHAT_SET_COUNT];			///< 탭 배열
	CRect					m_vTabRect[TCHAT_SET_COUNT];		///< 슬롯에 따른 탭의 위치 배열
	INT						m_iTabCount;						///< 탭의 갯수
   
	CTChatTabButton*		m_pSelectTab;						///< 선택된 탭
	CTChatTabButton*		m_pHitTab;							///< 가장 최근에 클릭된 탭

	TComponent*				m_pChatTypeCtrl;					///< 채팅 타입을 표시하기위한 컨트롤
	TEdit*					m_pChatEditCtrl;					///< 채팅 입력을 받기위한 에디터 컨트롤
	TEdit*					m_pChatWhisperTargetEditCtrl;	///< 귓말 대상 입력을 받기 위한 에디터 컨트롤
	BOOL					m_bChatEditON;						///< 현재 채팅 입력중인지 여부
	TComponent*				m_pChatEditBack;					///< 채팅 입력 배경
	TComponent*				m_pChatTabBack;
	TComponent*				m_pChatMode;

	//TButton*				m_pChatLockCtrl;					///< 채팅 리스트 락/언락 버튼			
	TButton*				m_pChatImeCtrl;						///< 채팅 입력 언어 버튼

	DWORD					m_dwTick;							///< 채팅창에 어떠한 이벤트가 발생 한 후부터의 경과시간.

	BYTE					m_bChatMode;						///< 현재 채팅 모드
	CString					m_strChatTarget;					///< 가장 최근의 귓말 타겟 이름
	DWORD					m_dwChatTargetID;					///< 가장 최근의 귓말 타겟 아이디
	CString					m_strLastWhisper;					///< 가장 마지막으로 받은 귓말 상대
	CString					m_strCandiWhisper;				///< 귓말 상대 예비자 (오른쪽 버튼으로 귓말 상대 선택시 사용)

	CTMiniPopupDlg*			m_vPopup[POPUP_COUNT];				///< 채팅 관련 팝업 리스트

	/**
		채팅 프레임이 잠겼는지 여부.
		프레임이 잠겼다는 의미는 리스트와 탭의 이동이나 추가 제거를
		할 수 없게 한다는 뜻이다. 또한 프레임이 잠긴 상태에서는 채팅
		리스트는 모든 HitTest 명령을 무시하게 된다. 즉, 리스트를 클릭
		할 수 없게 된다는 의미이다.
	*/
	BOOL					m_bLock;

	/// 채팅 프레임을 화면에 표시할지 여부
	BOOL					m_bChatFrameVisible;

	// 스냅 여부 판단에 사용되는 변수들
	BOOL					m_bProcSnapLeft;
	BOOL					m_bProcSnapTop;
	BOOL					m_bProcSnapRight;
	BOOL					m_bProcSnapBottom;

	TMAPCHATCMD	m_FpChatCmdMap;
	STRINGQUEUE		m_queLatestWhisper;
	BOOL					m_bInitialized;

public:
	/// 채팅 타입에 따른 색을 얻는다.
	static DWORD GetChatTypeColor(BYTE bChatMode);
	/// 채팅 타입에 따른 명칭을 얻는다.
	static const CString& GetChatTypeString(BYTE bChatMode);
	/// 채팅 타입에 따른 대표 명칭을 얻는다.
	const CString& GetChatTypeRepresentation(BYTE bChatMode);

	/// 주어진 리스트 인덱스에 해당하는 팝업 인덱스를 얻는다.
	static INT GetPopupIndex(INT nListIndex);

public:
	/// 주어진 팝업 대화상자를 채팅 모드 선택용으로 사용하도록 설정한다.
	void SetPopup(PopupEnum ePopup, CTMiniPopupDlg* pPopup);
	/// 채팅 팝업을 화면에 띄우거나 감춘다.
	void ShowPopup(PopupEnum ePopup, BOOL bShow);
	/// 모든 채팅 팝업을 화면에서 감춘다.
	void HideAllPopup();
	/// 채팅 팝업이 보여지는지 여부를 얻는다.
	BOOL IsVisiblePopup(PopupEnum ePopup);

	/// 채팅 플래그의 갱신을 팝업에 반영시킨다.
	void UpdateChatFlagToPopup();
	void SetHeightAllInnerChatList( INT nHeight );

	/**
		채팅 프레임을 갱신한다.

		@param dwTick	경과 시간
	*/
	void Update(DWORD dwTick);

	/**
		선택 탭을 변경한다. 이때 탭과 링크된 리스트가 화면에 표시된다.

		@warning	만약 탭이 밖으로 빠진 상태라면 해당 탭은 선택할 수 없다.
	*/
	void ChangeSelection(CTChatTabButton* pTab);
	void ChangeSelection(TCHAT_SET);

	/// 주어진 두 탭을 교환한다. 탭 교환은 슬롯의 교환과 함께 위치 교환을 의미한다.
	void SwapTab(CTChatTabButton* pTab1, CTChatTabButton* pTab2);
	/// 주어진 탭과 그에 링크된 리스트를 프레임 안으로 추가한다.
	void AddTabAndList(CTChatTabButton* pTab);
	/// 주어진 탭과 그에 링크된 리스트를 프레임 밖으로 뺀다.
	void RemoveTabAndList(CTChatTabButton* pTab);

	/**
		리스트가 주변과 스냅되어야 하는지 조사하고 만약 그렇다면
		스냅후 변경될 위치를 반환하다.

		@param pList		스냅여부를 조사할 리스트
		@param outRect		스냅 후 변경될 위치. 반환값아 TRUE 인 
							경우만 유효하다.
		@return				스냅 발생 여부.
	*/
	BOOL SnapList(CTChatList* pList, CRect& outRect);

	///
	void ResetSetting();

	/// 레즈스트리에 저장된 채팅 프레임의 상태로 현재 상태를 변경한다.
	void LoadSetting( BOOL bReset = FALSE);
	/// 현재 채팅 프레임의 상태를 레지스트리에 저장한다.
	void SaveSetting();

	/// 주어진 위치에 해당하는 탭 버튼을 얻는다.
	CTChatTabButton* GetHitTab(const CPoint& pt);
	/// 주어진 리스트와 링크된 탭 버튼을 얻는다.
	CTChatTabButton* FindTab(CTChatList* pList);

	/// 주어진 위치에 해당하는 채팅 리스트를 얻는다.
	CTChatList* GetHitList(const CPoint& pt);

	/// 주어진 팝업과 링크된 채팅 리스트를 얻는다.
	CTChatList* FindList(CTMiniPopupDlg* pPopup);

	CTChatList* FindList( DWORD dwChatFlag );

	/// 현재까지 입력된 채팅 메시지를 초기화한다.
	void ResetChatMsg(BOOL bEnable);
	/// 리스트의 모든 채팅 기록을 초기화한다.
	void ResetChatList();
	
	/// 귓말 대상을 입력한다.
	void ShowWhisperTargetInput( BOOL bShow );
	void ProcWhisperTargetInput( BOOL bCancel );
	void LoseFocusWhisperTargetInput( BOOL bCancel );

	void AddWhisperList( CString strWhisperTarget );

	/// 채팅 에디터로 퀘스트 링크 데이타를 넣는다.
	void InsertInfoToChatEdit(LPTQUEST pQuest);
	/// 채팅 에디터로 아이템 링크 데이타를 넣는다.
	void InsertInfoToChatEdit(LPTITEM pItem, CTClientItem* pClientItem);
	/// 채팅 에디터의 맵 앞으로 주어진 문자열을 삽입한다.
	void PushFrontMsgToEdit(const CString& strMsg);

	/// 현재 에디터에 입력된 채팅 메시지를 처리하고 전송해야할 메시지를 얻는다.
	CString ProcChatMsgInEdit();

	/// 현재 에디터에 입력된 메시지의 좌우 공백을 제거한다.	
	void TrimMsgInEdit();
	/// 현재 에디터에 입력된 메시지를 모두 제거한다.
	void ClearMsgInEdit();

	/**
		채팅 메시지를 프레임으로 전달하여 리스트에 그 내용을 표기하게 한다.

		@param strTYPE		채팅 타입을 나타내는 문자열
		@param strNAME		메시지를 보낸 곳의 이름
		@param strMSG		표시하고자 하는 메시지
		@param dwCOLOR		표시하고자 하는 메시지의 색
		@param dwChatFlag	이 메시지의 타입 플래그
		@param dwSoundID	이 메시지를 리스트에 표기할때 낼 사운드 ID 
							아무 소리도 원하지 않는다면 0 을 전달하면 된다.
							기본값을 0 이다.
		@param bSoundType	위에 사운드 ID 의 타입. (Only WAV:MEDIA_TSOUND)
		@return				실제로 표시되는 메시지
	*/
	CString ChatSysMSG(
		const CString& strTYPE,
		const CString& strNAME,
		const CString& strMSG,
		DWORD dwCOLOR,
		DWORD dwChatFlag,
		DWORD dwSoundID = 0,
		BYTE bSoundType = MEDIA_TSOUND);

	CString ChatMSG(
		const CString& strTYPE,
		const CString& strNAME,
		const CString& strMSG,
		DWORD dwCOLOR,
		DWORD dwChatFlag,
		DWORD dwSoundID,
		BYTE bSoundType,
		DWORD dwSenderID );

	/// 채팅 프래임으로 포커스를 강제한다.
	void ForceFocus();

	/// 모든 채팅 리스트의 배경을 보이게 할지 여부를 설정한다.
	void SetHideBackgroundOfList(BOOL bHide);
	/// 프레임 잠금 여부를 설정한다.
	void SetLock(BOOL bLock);

	/// IME 언어 모드가 갱신되었음을 채팅 프레임에 통보한다.
	void NotifyImeLocalModeChange();

	/// 에디터에 입력된 정보를 통해 현재 채팅 모드를 검사한다.
	void CheckChatMode();
	/// 현재 채팅 모드를 변경한다.
	void SetChatMode(BYTE bChatMode, CString strTarget = "");
	/// 귓말 모드에서의 채팅 타겟을 설정한다.
	void SetChatTarget(const CString& strTarget, DWORD dwTargetID=0);

	/// 채팅 모드 변경 커맨드 이벤트를 처리한다.
	static CString OnChatModeCmd(const CString& strMSG, const VECTORSTRING& vPARAM, DWORD dwUSER);

	/// 호칭 변경
	static CString OnChatTitleCmd(const CString& strMSG, const VECTORSTRING& vPARAM, DWORD dwUSER);

	/// 메시지에 따른 함수 포인터를 설정한다.
	void AddChatCmdProc(const CString& strCMD, FP_CHATCMDPROC fp, DWORD dwUSER=0);
	/// 설정된 메시지 함수 포인터 테이블을 모두 제거한다.
	void ClearChatCmdProc();

	TListItem* HitTestChatList( CPoint pt );

public:
	/// 아이템 관련 창과 같이 띄울 수 있는지 여부를 얻는다.
	virtual BOOL CanWithItemUI();
	/// 프레임을 화면에 띄우거나 감춘다.
	virtual void ShowComponent(BOOL bVisible = TRUE);
	/// 이 프레임이 주어진 좌표를 포함하는지 검사한다.
	virtual BOOL HitTest( CPoint pt);

	virtual void ResetPosition();
	virtual void DefaultPosition( CPoint* vBASIS, BOOL bRestore );

protected:
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnLoseFocus( TComponent* pSetFocus );
	/// 채팅 입력 에디터로 포커스를 강제하기 위해 재구현 되었다.
	virtual void SwitchFocus(TComponent* pCandidate);
    	
public:
	/**
		채팅창은 이 함수 호출 후부터 TCHAT_TIMER 에 정의된 
		시간 경과후 자동으로 화면에서 사라지게 된다.
	*/
	void ResetTick()								{ m_dwTick = 0; }

	/// 주어진 인덱스의 채팅 리스트를 얻는다.
	CTChatList* GetChatList(INT iIndex) const		{ return m_vLists[iIndex]; }

	/// 채팅 에디터 컨트롤을 얻는다.
	TEdit* GetChatEditCtrl() const					{ return m_pChatEditCtrl; }
	/// 귓말 대상 입력 컨트롤 얻는다.
	TEdit* GetWhisperTargetEditCtrl() const					{ return m_pChatWhisperTargetEditCtrl; }
	/// 채팅 입력중인지 여부를 얻는다.
	BOOL IsChatEditON() const						{ return m_bChatEditON; }

	/// 채팅 리스트의 기본 위치를 얻는다.
	const CRect& GetDefaultListRect() const			{ return m_rcDefaultList; }
	const INT GetDefaultListHeight() const { return m_rcDefaultList.Height(); }
	/// 주어진 슬롯에 해당하는 탭의 위치를 얻는다.
	const CRect& GetTabSlotRect(INT iSlot) const	{ return m_vTabRect[iSlot]; }

	/// 프레임이 잠겼는지 여부를 얻는다.
	BOOL IsLock() const								{ return m_bLock; }

	/// 메인 게임 프레임을 얻는다.
	CTClientGame* GetMainGame() const				{ return m_pMainGame; }

	BYTE GetChatMode() const						{ return m_bChatMode; }
	DWORD GetChatTargetID() const					{ return m_dwChatTargetID; }
	const CString& GetChatTarget() const			{ return m_strChatTarget; }

	CTChatList* GetNotifyList() const				{ return m_pNotifyList; }

	/// 가장 마지막에 받은 귓말 상대를 얻는다.
	const CString& GetLastWhisperTarget() const		{ return m_strLastWhisper; }

	void SetCandiWhisperTarget( CString strCandiWhisper ) { m_strCandiWhisper = strCandiWhisper; }
	const CString& GetCandiWhisperTarget() const		{ return m_strCandiWhisper; }

	TEdit* GetCurEdit();
	BOOL GetApplyCursor( LPTCURSOR_TYPE pbCursor );

	void ToggleTacticsChatMode( DWORD dwTactics );

#ifdef DEBUG
	/// 현재 프레임의 설정을 출력한다.
	inline void _LOG(LPCSTR func);
#endif

protected:
	/// 이 프레임에서 사용할 수 있는 유일한 ID 를 얻는다.
	UINT GetUniqueID(TComponent* pParent);

	/// rcFrom 리스트와 rcTo 리스트가 스냅 돼는지 검사하고 스냅 된다면 변할 위치를 얻는다.
	BOOL SnapList(const CRect& rcFrom, const CRect& rcTo, CRect& outRect);

	// 스냅 검사 함수들..
	BOOL SnapList_LeftToRight(const CRect& rcFrom, const CRect& rcTo, CRect& outRect);
	BOOL SnapList_RightToLeft(const CRect& rcFrom, const CRect& rcTo, CRect& outRect);
	BOOL SnapList_UpToDown(const CRect& rcFrom, const CRect& rcTo, CRect& outRect);
	BOOL SnapList_DownToUp(const CRect& rcFrom, const CRect& rcTo, CRect& outRect);

public:
	CTChatFrame(TComponent *pParent, LP_FRAMEDESC pDesc, CTClientGame* pMainGame, TCMLParser *pParser);
	virtual ~CTChatFrame();
};
