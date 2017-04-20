#pragma once

//////////////////////////////////////////////////////////////////////////

class CTCategoryEntity;

class CTSearchItem : public CTCtrlListItem
{
private :	// Member Data.

	CTCategoryEntity*			m_pEntity;

public :	// Const... & Destructure Function.

	CTSearchItem();
	virtual						~CTSearchItem();

public :	// Virtual Function.

	virtual	void				ReflectSlot( CTCtrlListSlot* pSlot );

public :	// Member Function.

	void						SetEntity( CTCategoryEntity* pEntity );
	CTCategoryEntity*			GetEntity();
};


//////////////////////////////////////////////////////////////////////////


class CTSearchSlot : public CTCtrlListSlot
{
public :	// Member Data.

	TButton*					m_pPlusBtn;
	TButton*					m_pMinusBtn;
	TComponent*					m_pText;
	TButton*					m_pCheckBtn;

public :

	CTSearchSlot();
	virtual ~CTSearchSlot();

	virtual	void				Select( BOOL bSel );
	virtual void				ShowComponent( BOOL bShow );

public :	// Member Function.

	BOOL						OnLButtonDown( CPoint pt );

};


//////////////////////////////////////////////////////////////////////////

class CTSearchCategory;

//!	Auction Search.
class CTAuctionSearch : public CTAuctionInnerFrame
{
public :	// DEFINITION.

	enum
	{
		TMODE_INVALID = -1,
		TMODE_EDITING = 1
	};

	enum
	{
		TEDIT_SEARCH,
		TEDIT_NEEDLEVEL_MIN,
		TEDIT_NEEDLEVEL_MAX,
		TEDIT_UPGRADE_MIN,
		TEDIT_UPGRADE_MAX,
		TEDIT_COUNT = 5
	};

	// Category.
	enum { MAX_ITEM_LIST = 9 };
	enum
	{
		SC_TREE_DEPTH1,
		SC_TREE_DEPTH2,
		SC_TREE_DEPTH3,
		SC_TREE_DEPTH_COUNT
	};

public :	// Static Member.

	static TButton*				s_pPlusBtn[SC_TREE_DEPTH_COUNT];
	static TButton*				s_pMinusBtn[SC_TREE_DEPTH_COUNT];
	static TComponent*			s_pString[SC_TREE_DEPTH_COUNT];
	static CPoint				s_ptButton_Offset[SC_TREE_DEPTH_COUNT];
	static CPoint				s_ptString_Offset[SC_TREE_DEPTH_COUNT];
	static TButton*				s_pCheck;
	static CPoint				s_ptCheck_Offset;

private :	// MEMBER DATA.

	TEdit*						m_pEdit[ TEDIT_COUNT ];
	TButton*					m_pSearchBtn;
	TButton*					m_pResetBtn;

	BOOL						m_bCurMode;
	CString						m_strSearch;

	CTSearchCategory*			m_pCategory;
	CTCtrlList*					m_pList;

public :	// VIRTUAL FUNCTION.

								CTAuctionSearch( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual						~CTAuctionSearch();

	virtual void				RequestInfo();
	virtual void				RequestUpdatePage();
	virtual void				ResetInfo();
	virtual void				UpdateEditable( CPoint pt );
	virtual void				OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void				OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags );
	virtual BOOL				DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void				SwitchFocus( TComponent *pCandidate );
	virtual void				ShowComponent( BOOL bVisible );
	virtual HRESULT				Render( DWORD dwTickCount );

public :	// MEMBER FUNCTION.

	void						DoSearch();
	void						ClearText();
	void						ResetComp();
	void						UpdateByFindInfo();
	void						UpdateByComp();
	TEdit*						GetCurEdit();

public :	// INLINE FUNCTION.

	BOOL						IsEditMode();

};