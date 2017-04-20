#pragma once


//!	리스트 아이템 정보
class CTAuctionItem : public CTCtrlListItem
{
public :	// MEMBER DATA.

	CTClientItem*			m_pItem;
	DWORD					m_dwAuctionID;
	DWORD					m_dwBidMoney[ TMONEY_COUNT ];
	DWORD					m_dwBuyMoney[ TMONEY_COUNT ];
	CString					m_strSeller;
	DWORD					m_dwRemainTick;
	INT64					m_nEndTime;
	BYTE					m_bBidCount;
	DWORD					m_dwCode;

public :	// VIRTUAL FUNCTION.

	virtual void			ReflectSlot(CTCtrlListSlot* pSlot);

public :
							CTAuctionItem();
	virtual					~CTAuctionItem();
};


//!	슬롯
class CTAuctionSlot : public CTCtrlListSlot
{
public :

	TComponent*				m_pImageSlot;
	TImageList*				m_pImageList;
	TComponent*				m_pUpgrade;
	TComponent*				m_pName;
	TComponent*				m_pClass;
	TComponent*				m_pNeedLevel;

	/*! 등록된 아이템이 한개일때 ( 입찰, 즉구 표시 ) */
	TComponent*				m_pBid;
	TComponent*				m_pBuy;
	/*! 등록된 아이템이 복수개 일때 ( 단가, 총액 표시 ) */
	TComponent*				m_pPrice;
	TComponent*				m_pTotal;

	TComponent*				m_pImageMoney1;
	TComponent*				m_pImageMoney2;
	TComponent*				m_pRune[2];
	TComponent*				m_pLuna[2];
	TComponent*				m_pCron[2];

	TComponent*				m_pSeller;
	TComponent*				m_pLeftTime;
	TComponent*				m_pBidCount;

public :	// VIRTUAL FUNCTION.
							CTAuctionSlot();
	virtual					~CTAuctionSlot();

	virtual void			ShowComponent( BOOL bShow = TRUE );
	virtual void			Select( BOOL bSel );
};


class CTAuctionCtrlList;

//!	Auction Main Frame.
/*!
*	@brief	경매 메인 프레임.
*
*/
class CTAuctionMainFrame : public CTFrameGroupBase
{
public :	// DEFINITION.

	enum
	{
		MAX_ITEM_SLOT = 9,
		MAX_COLUMN_HEAD = 7,
	};

	enum
	{
		TBUTTON_FIRST,
		TBUTTON_SECOND,
		TBUTTON_THIRD,
		TBUTTON_COUNT,
	};

	enum
	{
		TCOLUMN_HEAD_NAME,
		TCOLUMN_HEAD_CLASS,
		TCOLUMN_HEAD_LEVEL,
		TCOLUMN_HEAD_PRICE,
		TCOLUMN_HEAD_SELLER,
		TCOLUMN_HEAD_LEFTTIME,
		TCOLUMN_HEAD_BIDCOUNT,
		TCOLUMN_HEAD_COUNT
	};

private :	// MEMBER DATA.

	TFrame*					m_pAuctionListFrame;
	CTAuctionCtrlList*		m_pAuctionList;

	TButton*				m_pHeadBtn[ TCOLUMN_HEAD_COUNT ];
	CString					m_strHeadOrigText[ TCOLUMN_HEAD_COUNT ];

	TComponent*				m_pCurPage;
	TComponent*				m_pLastPage;
	TButton*				m_pMoveLeftPage;
	TButton*				m_pMoveRightPage;
	TButton*				m_pMoveLeftEnd;
	TButton*				m_pMoveRightEnd;
	TButton*				m_pBtn[ TBUTTON_COUNT ];

	WORD					m_wNpcID;

public :	// VIRTUAL FUNCTION.

							CTAuctionMainFrame( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual					~CTAuctionMainFrame();
	virtual void			SetSelect(UINT nFrame);
	virtual void			RequestInfo();
	virtual void			RequestUpdatePage();
	virtual void			ResetInfo();
	virtual void			OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void			OnRButtonDown( UINT nFlags, CPoint pt );
	virtual BOOL			DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual ITDetailInfoPtr	GetTInfoKey( const CPoint& point );
	virtual BOOL			GetTChatInfo( const CPoint& point, TCHATINFO& outInfo );
	virtual TDROPINFO		OnDrop( CPoint point );
	virtual void			ShowComponent( BOOL bVisible );
	virtual HRESULT			Render( DWORD dwTickCount );
	
public :	// MEMBER FUNCTION.

	/* 현재 아이템 리스트를 모두 클리어한다. */
	void					ClearItems();
	/* 탭이 변경되면 호출한다. */
	void					ChangedTab( INT nSelectFrameID, INT nPrevFrameID );
	/* 리스트관련 Component 업데이트 */
	void					UpdateByListComp();
	/* 버튼활성/비활성 처리. ( 관심물품, 입찰, 즉시구매, 구입 ) */
	void					UpdateByButtons();
	/* 리스트를 전부 업데이트 한다. */
	void					RefreshList();

	void					MoveNextPage();
	void					MovePrevPage();
	void					MoveFirstPage();
	void					MoveLastPage();
	BOOL					AvailableBID();
	void					SetButton( TButton* pButton, CString strBtn, DWORD dwFuncGM );
	void					SetSearchFrame();
	void					SetRegistFrame();
	void					SetMyBidFrame();
	void					SetBasketFrame();

	void					SetStackItem_Search();
	void					SetCanBidItem_Search();
	void					SetStackItem_Basket();
	void					SetCanBidItem_Basket();

	/* 현재 포커싱된 에디트컨트롤을 얻어온다. */
	TEdit*					GetCurEdit();

public :	// INLINE FUNCTION.

	static CTAuctionMainFrame*		GetInstance();
	CTAuctionCtrlList*		GetList();
	CTCtrlListItem*			GetSelectedItem();
    CTCtrlListItem*			GetAuctionItem( size_t index );
	void					SetCurPage( DWORD dwCurPage );
	void					SetLastPage( DWORD dwLastPage );
	void					SetColumnString( BYTE bColumn, CString& strArrowKey );
	void					ResetColumnString();
};

