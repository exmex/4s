#pragma once


class CTCraftCategory
{
public:
	static TComponent* m_pTempCategoryBar;
	static TComponent* m_pTempCategoryName;
	static TComponent* m_pTempCategoryPlusBtn;
	static TComponent* m_pTempCategoryMinusBtn;
	static CPoint m_ptPlusBtnOffset;
	static CPoint m_ptMinusBtnOffset;
	static CPoint m_ptNameBtnOffset;

public:
	CTCraftCategory();
	~CTCraftCategory();

	void DeleteAllItem();
	void SetCategoryPos( CPoint ptBase );
	void SetName( CString strName );
	INT CheckExpandBtn( CPoint pt );
	void ShowComponent( BOOL bVisible );

public:
	DWORD m_dwID;

	TComponent* m_pPlusBtn;
	TComponent* m_pMinusBtn;
	TComponent* m_pName;
	TComponent* m_pCategoryBar;

	vector< LPTQUEST >	m_vTQuest;

	BOOL m_bOpened;
};


//!	Craft Quest Dialog Class.
/*! 아이템제작 다이얼로그.
*
*
*/
class CTCraftQuestDlg : public CTClientUIBase
{
private :

	static CPoint		m_ptPOS;

	TList*				m_pQLIST;
	TList*				m_pRewardList;
	TList*				m_pTermList;
	TList*				m_pSummaryList;
	TImageList*			m_pITEM[TREWARDITEMCOUNT];
	TButton*			m_pCraftOK;

	CPoint				m_ptBasePointCategory;
	INT					m_nCategoryHeight;

	MAPTCRAFTCATEGORY	m_mapCategory;
	VTCRAFTCATEGORY		m_vCategoryOrders;

	BOOL				m_bNeedResetPos;
	BOOL				m_bCraftOK;
	
	LPTQUEST			m_pTQUEST;
	LPTREWARD			m_pTREWARD;
	LPTREWARD			m_pTTOPREWARD;
	LPTTERM				m_pSelTerm;

	DWORD				m_dwQuestID;
	CString				m_strFailMSG;

public :

	CTCraftQuestDlg( TComponent* pParent, LP_FRAMEDESC pDesc );
	virtual ~CTCraftQuestDlg();

	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point);
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL CanWithItemUI();

	DWORD	CharItemCount( DWORD dwID );
	void	ResetTerm();
	void	ResetTQUEST( LPTQUEST pTQuest );
	void	TermMessage( CString strText, CString strRESULT, INT nLine, INT& nNumber, BOOL bCraftOK );
	void	SummaryMessage( CString strText);
	void	ClearAllCategory();

public:

	DWORD m_dwCurCategory;

	enum
	{
		COL_ITEM,
		COL_LEVEL,
		COL_STATE,
		COL_CATEGORY,
		COL_COUNT
	};

	void ResetTree();
	BOOL ResetPos();

	void AddCategory( DWORD dwCategoryID, CString strName );
	void AddItemToCategory( DWORD dwCategoryID, LPTQUEST pTQuest );
	void AddItemToList( LPTQUEST pTQuest );
	void OpenCategory( DWORD dwCategoryID );
	void RemoveAllList();

	INT GetTop() { return m_pQLIST->GetTop(); }
	INT GetCount() { return m_pQLIST->GetItemCount(); }
	INT GetSel() { return m_pQLIST->GetSel(); }
	DWORD GetQuestID() { return m_dwQuestID; }
	void SetCurSel(INT nIdx) { m_pQLIST->SetCurSelItem(nIdx); }
	void RemoveAll() { m_pQLIST->RemoveAll(); }

	CTClientQClass*	GetQCLASS(INT nIdx);
	LPTQUEST		GetTQUEST(INT nIdx);
	LPTTERM			GetSelTerm();

	//////////////////////////////////////////////////////////////////////////
};

