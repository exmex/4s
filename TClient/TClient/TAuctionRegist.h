#pragma once

enum
{
	TREG_NO_BIDMONEY,
	TREG_LOW_BUYMONEY,
	TREG_COUNT,
};

//! Regist Frame Class.
class CTAuctionRegist : public CTAuctionInnerFrame
{
public :	// DEFINITION.

	enum
	{
		TTERM_BUTTON_COUNT = 4,
		TRUNE_LENGTH_STACKITEM = 2,
		TRUNE_LENGTH_ITEM = TRUNE_LENGTH / 2
	};

private :	// COMPONENT.

	TImageList*				m_pIcon;							//!< 등록할 아이템아이콘.
	TComponent*				m_pUpgrade;							//!< 업그레이드 수치.
	TComponent*				m_pName;							//!< 아이템이름.
	TComponent*				m_pItemCount;						//!< 아이템수량.
	TButton*				m_pTermBtn[ TTERM_BUTTON_COUNT ];	//!< 경매시간 버튼( 6, 12, 24, 48 시간 ).

	TComponent*				m_pTitleStartPrice;					//!< 경매시작가 & 단가.
	TComponent*				m_pTitleBuyPrice;					//!< 즉시구매가 & 총 액.
	TEdit*					m_pBid[ TMONEY_COUNT ];				//!< 경매시작가격 에디트박스.
	TEdit*					m_pBuy[ TMONEY_COUNT ];				//!< 즉시구매가격 에디트박스.
	TComponent*				m_pTextBuy[ TMONEY_COUNT ];			//!< 즉시구매가격 텍스트.
	TList*					m_pListNotice;						//!< 등록프레임 내에 알림사항.

private :	// MEMBER DATA.

	CTClientItem*			m_pItem;							//!< 등록할 아이템데이터.
	DWORD					m_dwTerm;							//!< 경매시간 데이터.
	TAUCTIONREGINFO			m_RegInfo;							//!< 아이템 등록정보.
	BOOL					m_bEditEnable;						//!< if Stack > 1  m_bEditEnable = FALSE.
																//!< 겹칠수 있는 아이템은 즉시구매가격을 수정 할 수 없다.
public :	// VIRTUAL FUNCTION.

							CTAuctionRegist(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual					~CTAuctionRegist();

	/*! */
	virtual void			RequestInfo();
	/*! 현재 페이지를 유지하면서 아이템리스트를 업데이트요청. */
	virtual void			RequestUpdatePage();
	/*! */
	virtual void			ResetInfo();
	/*! 에디트 컴포넌트 포커스관리. */
	virtual void			UpdateEditable( CPoint pt );

	virtual void			OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void			OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags );
	virtual void			OnChar( UINT nChar, int nRepCnt, UINT nFlags);
	virtual BYTE			OnBeginDrag( LPTDRAG pDRAG, CPoint point );
	virtual TDROPINFO		OnDrop( CPoint point );
	virtual ITDetailInfoPtr	GetTInfoKey( const CPoint& point );

public :	// MEMBER FUNCTION.

	void					UpdateRegInfo();
	void					UpdateByComp();
	void					UpdateByNumber();
	BOOL					CheckInvalidPrice();
	TEdit*					GetCurEdit();

public :	// INLINE FUNCTION.

	void					SetItem( CTClientItem* pItem );
	void					SetInvenID( BYTE bInvenID );
	void					SetItemCount( BYTE bCount );
	/*! 단일 이아템의 경우에 사용 */
	void					SetStartPrice( INT64 dwPrice );
	void					SetBuyPrice( INT64 dwPrice );
	/*! 복수 아이템의 경우에 사용 */
	void					SetUnitPrice( INT64 dwPrice );
	void					SetTotalPrice( INT64 dwPrice );

	void					SetEditEnable( BOOL bEnable );
	void					SetEditUnitPrice_RuneLength( INT nRuneLength );

	CTClientItem*			GetItem();
	LPTAUCTIONREGINFO		GetRegistInfo();
	INT64					GetStartPrice();
	INT64					GetBuyPrice();
	INT64					GetTotalPrice();
};