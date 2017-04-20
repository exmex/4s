#include "stdafx.h"
#include "TClientGame.h"
#include "TClientWnd.h"


//////////////////////////////////////////////////////////////////////////
//	AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
CTAuctionCommander::CTAuctionCommander()
:	m_pSession( NULL )
{
	SetNpcID( 0 );
}

CTAuctionCommander::~CTAuctionCommander()
{

}
//	END OF AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	REQUEST FUNCTION IMPLEMENTATION.
void	CTAuctionCommander::RequestDefaultList()
{
	GetSession();

	if( !m_pSession )
		return ;

	TAUCTIONFINDINFO FindInfo;
	FindInfo.Clear();

	FindInfo.wNpcID = GetNpcID();

	m_pSession->SendCS_AUCTIONFIND_REQ( &FindInfo );
}

void	CTAuctionCommander::RequestSearchList( LPTAUCTIONFINDINFO pInfo )
{
	GetSession();

	if( !m_pSession )
		return ;

	pInfo->wNpcID = GetNpcID();

	m_pSession->SendCS_AUCTIONFIND_REQ( pInfo );
}

void	CTAuctionCommander::RequestRegistList( LPTAUCTIONFINDINFO pInfo )
{
	GetSession();

	if( !m_pSession )
		return ;

	pInfo->wNpcID = GetNpcID();

	m_pSession->SendCS_AUCTIONREGLIST_REQ( pInfo );
}

void	CTAuctionCommander::RequestMyBidList( LPTAUCTIONFINDINFO pInfo )
{
	GetSession();

	if( !m_pSession )
		return ;

	pInfo->wNpcID = GetNpcID();

	m_pSession->SendCS_AUCTIONBIDLIST_REQ( pInfo );
}

void	CTAuctionCommander::RequestBasketList( LPTAUCTIONFINDINFO pInfo )
{
	GetSession();

	if( !m_pSession )
		return ;

	pInfo->wNpcID = GetNpcID();

	m_pSession->SendCS_AUCTIONINTERESTLIST_REQ( pInfo );
}

void	CTAuctionCommander::RequestUpdatePage()
{

}

void	CTAuctionCommander::RequestItemRegist()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionRegist* pRegistFrame =
		static_cast< CTAuctionRegist* >( pFrame->GetInnerFrame( TAUCTION_REGIST ) );

	LPTAUCTIONREGINFO pRegInfo = pRegistFrame->GetRegistInfo();
	pRegInfo->wNpcID = GetNpcID();

	GetSession();
	m_pSession->SendCS_AUCTIONREG_REQ( pRegInfo );
}

void	CTAuctionCommander::RequestItemRegCancel()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );

	if( pItem != NULL )
	{
		GetSession();
		m_pSession->SendCS_AUCTIONREGCANCEL_REQ( GetNpcID(), pItem->m_dwAuctionID );
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_DELITEM ) );
	}
}

void CTAuctionCommander::RequestBid()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientWnd* pWnd = pGame->GetMainWnd();
	CTClientChar* pMainChar = pGame->GetMainChar();
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );
	CTAuctionBidOnMsgBox* pBid = static_cast< CTAuctionBidOnMsgBox* >( pGame->GetFrame( TFRAME_BIDON ) );

	INT64 ldwPlayerMoney = 0;

	CTClientGame::AddUpMoney(
		&ldwPlayerMoney,
		pMainChar->m_dwRune,
		pMainChar->m_dwLuna,
		pMainChar->m_dwCron );

	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );

	if( pItem )
	{
		pBid->UpdateByMoney();

		INT64 ldwBuyPrice = 0;
		CTClientGame::AddUpMoney(
			&ldwBuyPrice,
			pItem->m_dwBuyMoney[ TMONEY_RUNE ],
			pItem->m_dwBuyMoney[ TMONEY_LUNA ],
			pItem->m_dwBuyMoney[ TMONEY_CRON ] );

		if( pBid->GetBidPrice() >= ldwBuyPrice )
		{
			pGame->DisableUI( TFRAME_BIDON );
			pWnd->MessageBoxYesNo(
				CTChart::Format( TSTR_AUCTIONBUY_ASK_OVERPRICE ),
				TSTR_YES,
				TSTR_NO,
				GM_AUCTION_DO_BUY,
				GM_CLOSE_MSGBOX );
		}
		else
		{
			m_pSession = pGame->GetSession();
			m_pSession->SendCS_AUCTIONBID_REQ(
				GetNpcID(),
				pItem->m_dwAuctionID,
				pBid->GetBidPrice() );
		}
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_BIDITEM ) );
	}
}

void CTAuctionCommander::RequestBuy()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientWnd* pWnd = pGame->GetMainWnd();
	m_pSession = pGame->GetSession();

	if( pWnd->m_MessageBox.GetParent() != pGame )
		return ;

	TFrame *pTMSGBOX = (TFrame *) pWnd->m_MessageBox.FindKid(ID_FRAME_2BTNMSGBOX);
	if( !pTMSGBOX || !pTMSGBOX->CanProcess() )
		return ;

	TEdit *pTEDIT = (TEdit *) pTMSGBOX->FindKid(ID_CTRLINST_EDIT);

	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( CTAuctionMainFrame::GetInstance()->GetSelectedItem() );

	if( pItem )
	{
		INT nCount = (INT) atoi( pTEDIT->m_strText );
		BYTE bMaxCount = pItem->m_pItem->GetCount();

		if( nCount <= bMaxCount &&
			nCount > 0 )
		{
			m_pSession->SendCS_AUCTIONBUYDIRECT_REQ(
				CTAuctionCommander::GetInstance()->GetNpcID(),
				pItem->m_dwAuctionID,
				(BYTE)nCount );
		}
		else
		{
			pWnd->CloseMessageBox();
			OnClicked_BuyButton();

			return ;
		}
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_BUYITEM ) );
	}

	pWnd->CloseMessageBox();
}

/*! 관심물품 등록 요청 */
void	CTAuctionCommander::RequestInsertBasket()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );

	if( pItem )
	{
		CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
		m_pSession = CTClientGame::GetInstance()->GetSession();
		m_pSession->SendCS_AUCTIONINTEREST_REQ( TRUE, pItem->m_dwAuctionID, pComm->GetNpcID() );
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONBASKET_NONE_ADDITEM ) );
	}
}

/*! 관심물품 등록해제 요청 */
void	CTAuctionCommander::RequestEraseBasket()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );

	if( pItem )
	{
		CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
		m_pSession = CTClientGame::GetInstance()->GetSession();
		m_pSession->SendCS_AUCTIONINTEREST_REQ( FALSE, pItem->m_dwAuctionID, pComm->GetNpcID() );
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONBASKET_NONE_DELTITEM ) );
	}
}
//	END OF REQUEST FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	RECEIVE FUNCTION IMPLEMENTATION.
BOOL	CTAuctionCommander::RecvFindList_Result( BYTE bResult )
{
	CString strMSG[ ALR_COUNT ] =
	{
		"",													// ALR_SUCCESS
		CTChart::Format( TSTR_AUCTIONLIST_FAIL ),			// ALR_FAIL
		CTChart::Format( TSTR_AUCTIONLIST_NPCNOTFOUND ),	// ALR_NPCNOTFOUND
		CTChart::Format( TSTR_AUCTIONLIST_NOLIST ),			// ALR_NOLIST
		CTChart::Format( TSTR_AUCTIONLIST_NOPAGE ),			// ALR_NOPAGE
	};

	if( bResult != ALR_SUCCESS )
		SimpleMsgBox( strMSG[ bResult ] );

	return (ALR_SUCCESS == bResult);
}

void	CTAuctionCommander::RecvFindList()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )	return ;

	CTAuctionMainFrame* pMainFrame = static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );
	if( !pMainFrame )	return ;

    if( pMainFrame->IsVisible() )
		pMainFrame->ResetInfo();
}

BOOL	CTAuctionCommander::RecvBid_Result( BYTE bResult )
{
	static CString strMSG[ ABR_COUNT ] =
	{
		CTChart::Format( TSTR_AUCTIONBID_SUCCESS ),		// ABR_SUCCESS
		CTChart::Format( TSTR_AUCTIONBID_FAIL ),		// ABR_FAIL
		CTChart::Format( TSTR_AUCTION_NPCNOTFOUND ),	// ABR_NPCNOTFOUND
		CTChart::Format( TSTR_AUCTIONBID_ITEMNOTFOUND ),// ABR_AUCTIONNOTFOUND
		CTChart::Format( TSTR_AUCTIONBID_SELF ),		// ABR_SELF
		CTChart::Format( TSTR_AUCTIONBID_LOWPRICE ),	// ABR_LOWPRICE
		CTChart::Format( TSTR_AUCTIONBID_NEEDMONEY ),	// ABR_NEEDMONEY
		CTChart::Format( TSTR_AUCTIONBID_COUNTERROR ),	// ABR_COUNTERROR
		CTChart::Format( TSTR_AUCTIONBID_DUPLICATE ),	// ABR_DUPLICATE
		CTChart::Format( TSTR_AUCTIONBID_INVALIDCHAR ),	// ABR_INVALIDCHAR
		CTChart::Format( TSTR_AUCTIONBID_MAXPOST ) // ABR_MAXPOST
	};

	SimpleMsgBox( strMSG[ bResult ] );

	CTClientGame* pGame = CTClientGame::GetInstance();

	pGame->DisableUI( TFRAME_BIDON );

	CTAuctionMainFrame* pFrame =
		static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );
	INT nSelFrame = pFrame->GetSelectedFrame();
	CTAuctionInnerFrame* pInnerFrame =
		static_cast< CTAuctionInnerFrame* >( pFrame->GetInnerFrame( nSelFrame ) );

	// refresh.
	pInnerFrame->RequestUpdatePage();


	return (ABR_SUCCESS == bResult);
}

BOOL	CTAuctionCommander::RecvBuy_Result( BYTE bResult )
{
	static CString strMSG[ ABR_COUNT ] =
	{
		CTChart::Format( TSTR_AUCTIONBUY_SUCCESS ),			// ABR_SUCCESS
		CTChart::Format( TSTR_AUCTIONBUY_FAIL ),			// ABR_FAIL
		CTChart::Format( TSTR_AUCTION_NPCNOTFOUND ),		// ABR_NPCNOTFOUND
		CTChart::Format( TSTR_AUCTIONBUY_ITEMNOTFOUND ),	// ABR_AUCTIONNOTFOUND
		CTChart::Format( TSTR_AUCTIONBID_SELF ),			// ABR_SELF
		"",													// ABR_LOWPRICE
		CTChart::Format( TSTR_AUCTIONBUY_NEEDMONEY ),		// ABR_NEEDMONEY
		CTChart::Format( TSTR_AUCTIONBUY_COUNTERROR ),		// ABR_COUNTERROR
		"duplicate_error",									// ABR_DUPLICATE
		CTChart::Format( TSTR_AUCTIONBID_INVALIDCHAR ),		// ABR_INVALIDCHAR
		CTChart::Format( TSTR_AUCTIONBID_MAXPOST ) // ABR_MAXPOST
	};

	SimpleMsgBox( strMSG[ bResult ] );


	CTClientGame* pGame = CTClientGame::GetInstance();
	CTAuctionMainFrame* pFrame =
		static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );
	INT nSelFrame = pFrame->GetSelectedFrame();
	CTAuctionInnerFrame* pInnerFrame =
		static_cast< CTAuctionInnerFrame* >( pFrame->GetInnerFrame( nSelFrame ) );

	// refresh.
	pInnerFrame->RequestUpdatePage();


	return (ABR_SUCCESS == bResult);
}

BOOL	CTAuctionCommander::RecvReg_Result( BYTE bResult )
{
	static CString strMSG[ AR_COUNT ] =
	{
		"", // AR_SUCCESS
		CTChart::Format( TSTR_AUCTIONREG_FAIL ), // AR_FAIL
		CTChart::Format( TSTR_AUCTION_NPCNOTFOUND ), // AR_NPCNOTFOUND
		CTChart::Format( TSTR_AUCTIONREG_INVALIDCHAR ),	// AR_INVALIDCHAR
		CTChart::Format( TSTR_AUCTIONREG_INVALIDPRICE ), // AR_INVALIDPRICE
		CTChart::Format( TSTR_AUCTIONREG_INVALIDSTARTPRICE ), // AR_INVALIDSTARTPRICE
		CTChart::Format( TSTR_AUCTIONREG_ITEMNOTFOUND ), // AR_ITEMNOTFOUND
		CTChart::Format( TSTR_AUCTIONREG_INVALIDITEM ), // AR_INVALIDITEM
		CTChart::Format( TSTR_AUCTIONREG_INVALIDAUCTION ), // AR_INVALIDAUCTION
		CTChart::Format( TSTR_AUCTIONREG_NEEDMONEY ), // AR_NEEDMONEY
		CTChart::Format( TSTR_AUCTIONREG_MAXPOST ), // AR_MAXPOST
		CTChart::Format( TSTR_AUCTIONREG_CHANNEL ) // AR_CHANNEL
	};

	if( bResult != AR_SUCCESS )
		SimpleMsgBox( strMSG[ bResult ] );

	return (AR_SUCCESS == bResult );
}

BOOL	CTAuctionCommander::RecvRegCancel_Result( BYTE bResult )
{
	static CString strMSG[ AR_COUNT ] =
	{
		"", // AR_SUCCESS
		CTChart::Format( TSTR_AUCTIONREGCANCEL_FAIL ), // AR_FAIL
		CTChart::Format( TSTR_AUCTION_NPCNOTFOUND ), // AR_NPCNOTFOUND
		CTChart::Format( TSTR_AUCTIONREG_INVALIDCHAR ),	// AR_INVALIDCHAR
		CTChart::Format( TSTR_AUCTIONREG_INVALIDPRICE ), // AR_INVALIDPRICE
		CTChart::Format( TSTR_AUCTIONREG_INVALIDSTARTPRICE ), // AR_INVALIDSTARTPRICE
		CTChart::Format( TSTR_AUCTIONREGCANCEL_ITEMNOTFOUND ), // AR_ITEMNOTFOUND
		CTChart::Format( TSTR_AUCTIONREGCANCEL_INVALIDITEM ), // AR_INVALIDITEM
		CTChart::Format( TSTR_AUCTIONREGCANCEL_INVALIDAUCTION ), // AR_INVALIDAUCTION
		"AR_NEEDMONEY", // AR_NEEDMONEY
	};

	if( bResult != AR_SUCCESS )
		SimpleMsgBox( strMSG[ bResult ] );

	CTAuctionMainFrame::GetInstance()
		->RequestInfo();

	return (AR_SUCCESS == bResult );
}
//	END OF RECEIVE FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
/*! MainFrame( 입찰버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_BidButton()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	CTClientUIBase* pUIBase = pGame->GetFrame( TFRAME_AUCTION );
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( pUIBase );

	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );
	if( pItem )
	{
		if( pItem->m_strSeller == pMainChar->GetName() )
		{
			SimpleMsgBox( CTChart::Format( TSTR_AUCTIONBID_SELF ) );
		}
		else
		{
			CTClientUIBase* pUIBase = pGame->GetFrame( TFRAME_BIDON );
			CTAuctionBidOnMsgBox* pBidOn = static_cast< CTAuctionBidOnMsgBox* >( pUIBase );
			INT64 ldwPrice = 0;
			INT64 ldwBuyPrice = 0;

			CTClientGame::AddUpMoney(
				&ldwPrice,
				pItem->m_dwBidMoney[ TMONEY_RUNE ],
				pItem->m_dwBidMoney[ TMONEY_LUNA ],
				pItem->m_dwBidMoney[ TMONEY_CRON ] );

			CTClientGame::AddUpMoney(
				&ldwBuyPrice,
				pItem->m_dwBuyMoney[ TMONEY_RUNE ],
				pItem->m_dwBuyMoney[ TMONEY_LUNA ],
				pItem->m_dwBuyMoney[ TMONEY_CRON ] );

			pBidOn->SetCurPrice( ldwPrice );
			pBidOn->SetBidPrice( (INT64)(ldwPrice * 1.05 + 1) );
			pBidOn->SetBuyPrice( ldwBuyPrice );
			pBidOn->UpdateByComp();

			pGame->EnableUI( TFRAME_BIDON );
		}
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_BIDITEM ) );
	}
}

/*! MainFrame( 즉시구매버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_BuyButton()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	CTClientWnd* pWnd = pGame->GetMainWnd();
	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );

	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );
	if( pItem )
	{
		if( pItem->m_strSeller == pMainChar->GetName() )
		{
			SimpleMsgBox( CTChart::Format( TSTR_AUCTIONBUY_DROP ) );
		}
		else
		{
			LPTITEM pTITEM = pItem->m_pItem->GetTITEM();

			CString strEDITTITLE;
			CString strEDIT;
			CString strMSG;

			strEDITTITLE = CTChart::LoadString( TSTR_EA);
			strEDIT = CTChart::Format( TSTR_FMT_NUMBER, 1);

			CString strPRICE;
			CString strStack;
			INT64 ldwPrice = 0;

			if( pItem->m_pItem->GetTITEM()->m_bStack > 1 )
			{
				CTClientGame::AddUpMoney(
					&ldwPrice,
					pItem->m_dwBidMoney[ TMONEY_RUNE ],
					pItem->m_dwBidMoney[ TMONEY_LUNA ],
					pItem->m_dwBidMoney[ TMONEY_CRON ] );
			}
			else
			{
				CTClientGame::AddUpMoney(
					&ldwPrice,
					pItem->m_dwBuyMoney[ TMONEY_RUNE ],
					pItem->m_dwBuyMoney[ TMONEY_LUNA ],
					pItem->m_dwBuyMoney[ TMONEY_CRON ] );
			}

			strPRICE = CTClientGame::MakeMoneyStr( ldwPrice );

			strMSG = CTChart::Format( TSTR_FMT_BUYITEM_AS_PRICE,
				pTITEM->m_strNAME,
				strPRICE);

			if( pTITEM->m_bStack > 1 )
				strMSG += CTChart::Format( TSTR_FMT_BUYITEM_STACK, pItem->m_pItem->GetCount() );

			pWnd->MessageBox(
				strMSG,
				strEDITTITLE,
				strEDIT,
				TSTR_YES,
				TSTR_NO,
				ID_FRAME_2BTNMSGBOX,
				GM_AUCTION_DO_BUY,
				GM_CLOSE_MSGBOX,
				0,
				pTITEM->m_bStack > 1 ? TRUE : FALSE,
				TRUE);

			pGame->EnableChat(FALSE);
		}
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_BUYITEM ) );
	}
}

/*! MainFrame( 관심물품버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_InsertBasketButton()
{

}

/*! BasketFrame( 관신물품 해제 버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_DeleteBasketButton()
{

}

/*! SearchFrame( 검색버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_SearchButton()
{

}

/*! SearchFrame( 검색정보 초기화 처리 ) */
void	CTAuctionCommander::OnClicked_SearchResetButton()
{
	CTAuctionMainFrame::GetInstance()
		->ResetColumnString();
}

/*! RegistFrame( 등록버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_RegistButton()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientWnd* pWnd = pGame->GetMainWnd();
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();

	if( pFrame->IsSelect( TAUCTION_REGIST ) )
	{
		CTAuctionRegist* pRegistFrame =
			static_cast< CTAuctionRegist* >( pFrame->GetInnerFrame( TAUCTION_REGIST ) );

		CTClientItem* pItem = pRegistFrame->GetItem();
		if( pItem )
		{
			pRegistFrame->UpdateRegInfo();
			if( !pRegistFrame->CheckInvalidPrice() )
			{
				SimpleMsgBox( CTChart::Format( TSTR_AUCTION_NEED_STARTPRICE ) );
				return ;
			}

			DWORD dwRune = 0;
			DWORD dwLuna = 0;
			DWORD dwCron = 0;
			INT64 ldwMoney = 0;

			if( pItem->GetTITEM()->m_bStack > 1 )
				ldwMoney = pRegistFrame->GetTotalPrice() / 50;
			else
				ldwMoney = pRegistFrame->GetBuyPrice() / 50;

			pGame->SplitMoney( ldwMoney, &dwRune, &dwLuna, &dwCron );
			CString strMoney = pGame->MakeMoneyStr( dwRune, dwLuna, dwCron );

			pWnd->MessageBoxYesNo(
				CTChart::Format( TSTR_FMT_AUCTIONREG_ASK, strMoney ),
				TSTR_YES,
				TSTR_NO,
				GM_AUCTION_DO_REGIST,
				GM_CLOSE_MSGBOX );
		}
		else
		{
			SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_REGITEM ) );
		}
	}
}

/*! RegistFrame( 아이템 등록정보 초기화 ) */
void	CTAuctionCommander::OnClicked_RegistResetButton()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionRegist* pRegFrame = static_cast< CTAuctionRegist* >( pFrame->GetInnerFrame( TAUCTION_REGIST ) );

	if( pRegFrame->CanProcess() )
	{
		pRegFrame->ResetInfo();
	}
}

/*! RegistFrame( 등록취소버튼클릭 처리 ) */
void	CTAuctionCommander::OnClicked_RegistCancelButton()
{
	CTClientWnd* pWnd = CTClientGame::GetInstance()->GetMainWnd();
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pFrame->GetSelectedItem() );

	if( pItem )
	{
		pWnd->MessageBoxYesNo(
			CTChart::Format( TSTR_AUCTIONREGCANCEL_ASK ),
			TSTR_YES,
			TSTR_NO,
			GM_AUCTION_DO_CANCEL_REG,
			GM_CLOSE_MSGBOX,
			0,
			TRUE);
	}
	else
	{
		SimpleMsgBox( CTChart::LoadString( TSTR_AUCTIONMSG_NONE_DELITEM ) );
	}
}

void	CTAuctionCommander::ClearPageNumber()
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();

	pFrame->ClearItems();
}

void	CTAuctionCommander::SimpleMsgBox( CString strMSG )
{
	CTClientWnd* pWnd = CTClientGame::GetInstance()->GetMainWnd();

	pWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0 );
}

void	CTAuctionCommander::OnClicked_AlignKind( BYTE bColumn, BYTE bASC, BYTE bDESC )
{
	CTAuctionMainFrame* pFrame = CTAuctionMainFrame::GetInstance();
	CTAuctionInnerFrame* pInnerFrame =
		static_cast< CTAuctionInnerFrame* >( pFrame->GetInnerFrame( pFrame->GetSelectedFrame() ) );

	if( pInnerFrame )
	{
		LPTAUCTIONFINDINFO pFindInfo = pInnerFrame->GetFindInfo();

		if( bColumn == CTAuctionMainFrame::TCOLUMN_HEAD_PRICE )
		{
			if( pFindInfo->bAlignKind < bASC ||
				pFindInfo->bAlignKind >= bDESC )
				pFindInfo->bAlignKind = bASC;
			else
				pFindInfo->bAlignKind++;
		}
		else
		{
			if( pFindInfo->bAlignKind == bASC )
				pFindInfo->bAlignKind = bDESC;
			else
				pFindInfo->bAlignKind = bASC;
		}

		//!< Arrow.
		static CString strArrowArray[4] =
		{
			CTChart::LoadString( TSTR_AUCTION_ARROW_UP ),
			CTChart::LoadString( TSTR_AUCTION_ARROW_DOWN ),
			CTChart::LoadString( TSTR_AUCTION_ARROW_UP2 ),
			CTChart::LoadString( TSTR_AUCTION_ARROW_DOWN2 )
		};

		CString strArrowKey;
		strArrowKey = strArrowArray[ pFindInfo->bAlignKind - bASC ];

		if( bColumn == CTAuctionMainFrame::TCOLUMN_HEAD_NAME )
			strArrowKey = CTChart::Format( TSTR_AUCTIONLIST_HEAD_NAME, strArrowKey );

		pFrame->SetColumnString( bColumn, strArrowKey );

		pInnerFrame->RequestUpdatePage();
	}
}
//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
CTAuctionCommander* CTAuctionCommander::GetInstance()
{
	static CTAuctionCommander sCom;
	return &sCom;
}

CTClientSession*	CTAuctionCommander::GetSession()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return NULL;

	return m_pSession = pGame->GetSession();
}

WORD	CTAuctionCommander::GetNpcID()
{
	return m_wNpcID;
}

void	CTAuctionCommander::SetNpcID( WORD wNpcID )
{
	m_wNpcID = wNpcID;
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////