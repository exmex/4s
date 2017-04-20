#pragma once

#include "TMiniPopupDlg.h"

class CTMsgGroupList : public ITInnerFrame
{
public:
	enum MemberTxt
	{
		MTXT_NAME,
		MTXT_JOB,
		MTXT_LEVEL,
		MTXT_STATE,
		MTXT_POS,
		MTXT_COUNT
	};

	enum ItemState
	{
		ISTATE_NORMAL,
		ISTATE_SELECT,
		ISTATE_COUNT
	};

	enum ItemType
	{
		ITYPE_MEMBER,
		ITYPE_GROUP,
		ITYPE_COUNT
	};

	enum PopupType
	{
//		POPUPTYPE_NORMAL,
		POPUPTYPE_MEMBER,
		POPUPTYPE_GROUP,
		POPUPTYPE_GROUPLIST,
		POPUPTYPE_SOULMATE,
		POPUPTYPE_COUNT,
	};

	enum NormalPopup
	{
		NPOPUP_INVITE,
		NPOPUP_BLOCKLIST,
		NPOPUP_COUNT
	};

	enum MemberPopup
	{
		//MPOPUP_CHAT,
		MPOPUP_PARTY,
		MPOPUP_SENDMAIL,
		MPOPUP_MOVEGROUP,
		MPOPUP_BLOCK,
		//MPOPUP_BLOCKLIST,
		MPOPUP_DELETE,
		//MPOPUP_NEWGROUP,
		MPOPUP_COUNT
	};

	enum GroupPopup
	{
		GPOPUP_CHAT,
		GPOPUP_CHANGENAME,
		GPOPUP_DELETE,
		GPOPUP_NEWGROUP,
		GPOPUP_COUNT
	};

public:
	class CItem;
	class CGroup;
	class CMember;

	typedef std::vector<CItem*> CItemArray;

	class CItem
	{
	public:
		CTMsgGroupList*	m_pParent;
		
		INT		m_nIndex;
		INT		m_nLine;
		BOOL	m_bSelect;
		DWORD	m_dwID;
		BOOL m_bSoulmate;
		BOOL m_bEtcGroup;

	public:
		virtual ItemType GetType() const = 0;
		virtual INT	 GetLineHeight() const = 0;
		virtual TComponent* GetNameComp() const	= 0;

		virtual void Show(BOOL bShow) = 0;
		virtual INT  GetLastLine() const = 0;
		virtual void Update(INT nShowRangeStart, INT nShowRangeEnd) = 0;
		virtual void SetPos(INT nOffset) = 0;
		virtual void MoveLine(INT nMove) = 0;
		virtual BOOL HitTest(const CPoint& pt) = 0;
		virtual CItem* GetHitItem(const CPoint& pt) = 0;
		virtual void Select(BOOL bSelect) = 0;
		virtual void OnLButtonDown(const CPoint& pt) = 0;
		virtual void OnRButtonDown(const CPoint& pt) = 0;
		virtual void FrameUpdate( DWORD dwTickTime ) = 0;
		
		BOOL IsVisible() const;

	public:
		CItem(CTMsgGroupList* pParent, INT nIdx, INT nLine);
		virtual ~CItem();
	};
	
	class CGroup : public CItem
	{
	public:
		CGroup*		m_pTplt;
		TComponent*	m_pFrame[ISTATE_COUNT];
		TButton*	m_pStateBtn;
		TComponent* m_pGroupName;
		
		CItemArray	m_ItemArray;
	
	public:
		virtual ItemType GetType() const		{ return ITYPE_GROUP; }
		virtual INT	 GetLineHeight() const		{ return 1; }
		virtual TComponent* GetNameComp() const	{ return m_pGroupName; }

		virtual void Show(BOOL bShow);
		virtual INT  GetLastLine() const;
		virtual void Update(INT nShowRangeStart, INT nShowRangeEnd);
		virtual void SetPos(INT nOffset);
		virtual void MoveLine(INT nMove);
		virtual BOOL HitTest(const CPoint& pt);
		virtual CItem* GetHitItem(const CPoint& pt);
		virtual void Select(BOOL bSelect);
		virtual void OnLButtonDown(const CPoint& pt);
		virtual void OnRButtonDown(const CPoint& pt);
		virtual void FrameUpdate( DWORD dwTickTime );
		
		void ClearChildren();
		void HideAllChildren();
		void ToggleSpread();
		void Spread( BOOL );
		
		CMember* FindMemberByName(const CString& strName);
		CMember* FindMemberByID(DWORD dwID);

		void CollectTwinkle( std::map< DWORD, BOOL >& );

	public:
		CGroup(CTMsgGroupList* pParent, INT nIdx, INT nLine, CGroup* pTplt);
		virtual ~CGroup();
	};

	class CMember : public CItem
	{
	public:
		CMember*	m_pTplt;
		CGroup*		m_pParentGrp;
		TComponent* m_pTopTplt[ISTATE_COUNT];
		TComponent* m_pDownTplt[ISTATE_COUNT];
		TComponent* m_pTopFrame[ISTATE_COUNT];
		TComponent* m_pDownFrame[ISTATE_COUNT];
		TComponent*	m_vTxt[MTXT_COUNT];
		BOOL		m_bOnLine;
		BOOL		m_bTwinkle;
		DWORD	m_dwTwinkleAccTime;
		BOOL m_bShowTop;
		BOOL m_bShowDown;

	public:
		virtual ItemType GetType() const		{ return ITYPE_MEMBER; }
		virtual INT	 GetLineHeight() const		{ return 2; }
		virtual TComponent* GetNameComp() const	{ return m_vTxt[MTXT_NAME]; }
		
		virtual void Show(BOOL bShow);
		virtual INT  GetLastLine() const;
		virtual void Update(INT nShowRangeStart, INT nShowRangeEnd);
		virtual void SetPos(INT nOffset);
		virtual void MoveLine(INT nMove);
		virtual BOOL HitTest(const CPoint& pt);
		virtual CItem* GetHitItem(const CPoint& pt);
		virtual void Select(BOOL bSelect);
		virtual void OnLButtonDown(const CPoint& pt);
		virtual void OnRButtonDown(const CPoint& pt);
		virtual void FrameUpdate( DWORD dwTickTime );

		void ShowTop(BOOL bShow);
		void ShowDown(BOOL bShow);

		void StopTwinkle();

	public:
		CMember(CTMsgGroupList* pParent, CGroup* pParentGrp, INT nIdx, INT nLine, CMember* pTplt);
		virtual ~CMember();
	};

public:
	TScroll*	m_pScroll;
	CItemArray	m_ItemArray;

	CMember		m_MemberTplt;
	CGroup		m_GroupTplt;

	BOOL		m_bNeedUpdate;
	INT			m_nPrvScrPos;
	INT			m_nLinePerPage;
	INT			m_nItemHeight;

	CItem*		m_pSelectItem;
	BOOL		m_bShutDown;

	BOOL		m_bShowPopup;
	CTMiniPopupDlg* m_vPopup[POPUPTYPE_COUNT];

public:
	CGroup* m_pSoulmateGroup;
	CMember* m_pSoulmateMember;
	CGroup* m_pEtcGroup;

public:
	CGroup* GetEtcGroup();

	void InitPopup(LP_FRAMEDESC pPopupDESC);
	
	void HideAllPopup();
	void ShowPopup(PopupType type, const CPoint* pt=NULL);

	CTMiniPopupDlg* GetPopup(PopupType type) const { return m_vPopup[type]; }

	CGroup* AddGroup();
	CMember* AddMember(CGroup* pParent=NULL);
	
	CGroup* InsertGroup(INT nIdx);
	CMember* InsertMember(INT nIdx, CGroup* pParent=NULL);

	UINT GetItemCount(CGroup* pParent=NULL) const;

	CItem* GetItem(INT nIdx, CGroup* pParent=NULL) const;
	CItem* NextItem(CItem* pItem, CGroup* pParent=NULL) const;
	CItem* PrevItem(CItem* pItem, CGroup* pParent=NULL) const;
	CItem* GetLastItem(CGroup* pParent=NULL) const;
	CItem* GetHitItem(const CPoint& pt);

	CMember* FindMemberByName(const CString& strName);
	CGroup* FindGroupByName(const CString& strName);
	
	CMember* FindMemberByID(DWORD dwID);
	CGroup* FindGroupByID(DWORD dwID);

	void Clear();
	void RemoveItem(INT nIdx, CGroup* pParent=NULL);

	void ChangerGroup(CMember* pMember, CGroup* pGroup=NULL);

	void MoveIndex(INT nMove, CItem* pStartItem=NULL);
	void MoveLine(INT nMove, CItem* pStartItem=NULL);
	
	void NotifyUpdate();

	void UpdateSoulmateInfo( DWORD dwSoulID, CString strSoulName, DWORD dwSoulRegion, BYTE bSoulLevel, BYTE bSoulClass, BYTE bSoulConnected );

	void CollectTwinkle( std::map< DWORD, BOOL >& );

public:
	void SetSelectItem(CItem* pItem)	{ m_pSelectItem = pItem; }
	CItem* GetSelectItem() const		{ return m_pSelectItem; }

	INT GetItemHeight() const			{ return m_nItemHeight; }
	TScroll* GetScroll() const			{ return m_pScroll; }

	BOOL IsShutDown() const				{ return m_bShutDown; }

public:
	virtual void SwitchFocus(TComponent* pCandidate);
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual HRESULT Render(DWORD dwTickCount);

	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnMButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnMButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);

	virtual void RequestInfo() {}
	virtual void ResetInfo() {}

protected:
	void Update();

public:
	CTMsgGroupList(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMsgGroupList();

};


















