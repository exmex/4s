#pragma once


//! Auction Commander Class.
class CTAuctionCommander
{
private :

	CTAuctionCommander();
	~CTAuctionCommander();

private :	// MEMBER DATA.

	CTClientSession*			m_pSession;
	WORD						m_wNpcID;

public :	// REQUEST FUNCTION.

	/*! 경매 등록순서로 리스트요청. */
	void						RequestDefaultList();
	/*! 검색탭의 리스트요청. */
	void						RequestSearchList( LPTAUCTIONFINDINFO pInfo );
	/*! 등록탭의 리스트요청. */
	void						RequestRegistList( LPTAUCTIONFINDINFO pInfo );
	/*! 입찰목록탭의 리스트요청. */
	void						RequestMyBidList( LPTAUCTIONFINDINFO pInfo );
	/*! 관심물품탭의 리스트요청. */
	void						RequestBasketList( LPTAUCTIONFINDINFO pInfo );
	/*! Update Current Page */
	void						RequestUpdatePage();
	/*! 아이템등록 요청. */
	void						RequestItemRegist();
	/*! 아이템등록취소 요청. */
	void						RequestItemRegCancel();
	/*! 입찰요청. */
	void						RequestBid();
	/*! 즉시구매 요청. */
	void						RequestBuy();
	/*! 관심물품 등록 요청 */
	void						RequestInsertBasket();
	/*! 관심물품 등록해제 요청 */
	void						RequestEraseBasket();

public :	// RECEIVE FUNCTION.

	/*! 요청한 아이템 리스트를 처리한다. */
	BOOL						RecvFindList_Result( BYTE bResult );
	void						RecvFindList();
	/*! 입찰결과 메세지출력 */
	BOOL						RecvBid_Result( BYTE bResult );
	/*! 즉시구매결과 메세지출력 */
	BOOL						RecvBuy_Result( BYTE bResult );
	/*! 경매아이템 등록 메세지출력 */
	BOOL						RecvReg_Result( BYTE bResult );
	/*! 경매아이템 등록취소 메세지출력 */
	BOOL						RecvRegCancel_Result( BYTE bResult );

public :	// MEMBER FUNCTION.

	/*! MainFrame( 입찰버튼클릭 처리 ) */
	void						OnClicked_BidButton();
	/*! MainFrame( 즉시구매버튼클릭 처리 ) */
	void						OnClicked_BuyButton();
	/*! MainFrame( 관심물품버튼클릭 처리 ) */
	void						OnClicked_InsertBasketButton();
	/*! BasketFrame( 관신물품 해제 버튼클릭 처리 ) */
	void						OnClicked_DeleteBasketButton();
	/*! SearchFrame( 검색버튼클릭 처리 ) */
	void						OnClicked_SearchButton();
	/*! SearchFrame( 검색정보 초기화 처리 ) */
	void						OnClicked_SearchResetButton();
	/*! RegistFrame( 등록버튼클릭 처리 ) */
	void						OnClicked_RegistButton();
	/*! RegistFrame( 아이템 등록정보 초기화 ) */
	void						OnClicked_RegistResetButton();
	/*! RegistFrame( 등록취소버튼클릭 처리 ) */
	void						OnClicked_RegistCancelButton();
	/*! Page 초기화 */
	void						ClearPageNumber();
	/*! 1버튼 확인 메세지창 */
	void						SimpleMsgBox( CString strMSG );
	/* 정렬버튼 클릭처리. */
	void						OnClicked_AlignKind( BYTE bColumn, BYTE bASC, BYTE bDESC );

public :	// INLINE FUNCTION.

	/*!	인스턴스를 얻어온다. */
	static CTAuctionCommander*	GetInstance();
	/*! Session을 얻어온다. */
	CTClientSession*			GetSession();
	/*! NpcID를 가져옴. */
	WORD						GetNpcID();
	/*! NpcID를 셋팅. */
	void						SetNpcID( WORD wNpcID );
};