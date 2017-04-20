#include "stdafx.h"
#include "TClientGame.h"
#include "TList.h"


CTAuctionRegist::CTAuctionRegist(TComponent* pParent, LP_FRAMEDESC pDesc)
: CTAuctionInnerFrame( pParent, pDesc, TAUCTION_REGIST )
{
	m_pItem = NULL;
	m_dwTerm = TTERM_BUTTON_COUNT-1; // Default Time : 48hr.
	m_bEditEnable = FALSE;

	m_pIcon = static_cast< TImageList* >( FindKid( ID_CTRLINST_ICON ) );
	m_pUpgrade = FindKid( ID_CTRLINST_ITEMUPGRADE );
	m_pUpgrade->ClearText();
	m_pName = FindKid( ID_CTRLINST_ITEMNAME );
	m_pName->ClearText();
	m_pItemCount = FindKid( ID_CTRLINST_ITEMCOUNT );
	m_pItemCount->m_strText = CTChart::Format( TSTR_FMT_AMOUNT, 0 );

	m_pTitleStartPrice = FindKid( ID_CTRLINST_TEXT_START );
	m_pTitleBuyPrice = FindKid( ID_CTRLINST_TEXT_BUY );

	DWORD dwTerm[ TTERM_BUTTON_COUNT ] =
	{
		ID_CTRLINST_TIME6,
			ID_CTRLINST_TIME12,
			ID_CTRLINST_TIME24,
			ID_CTRLINST_TIME48
	};

	DWORD dwBid[ TMONEY_COUNT ] =
	{
		{ ID_CTRLINST_EDITBID_RUNE },
		{ ID_CTRLINST_EDITBID_LUNA },
		{ ID_CTRLINST_EDITBID_CRON }
	};

	DWORD dwBuy[ TMONEY_COUNT ][ 2 ] =
	{
		{ ID_CTRLINST_EDITBUY_RUNE, ID_CTRLINST_TEXT_BUY_RUNE },
		{ ID_CTRLINST_EDITBUY_LUNA, ID_CTRLINST_TEXT_BUY_LUNA },
		{ ID_CTRLINST_EDITBUY_CRON, ID_CTRLINST_TEXT_BUY_CRON }
	};

	for( int i=0; i<TTERM_BUTTON_COUNT; ++i )
	{
		m_pTermBtn[i] = static_cast< TButton* >( FindKid( dwTerm[i] ) );
		m_pTermBtn[i]->Select( i == m_dwTerm ? TRUE : FALSE );
	}

	INT nMoneyLength[ TMONEY_COUNT ] =
	{
		TRUNE_LENGTH / 2,
		TLUNA_LENGTH,
		TCRON_LENGTH
	};

	for( int i=0; i<TMONEY_COUNT; ++i )
	{
		TEdit* pEdit = static_cast< TEdit* >( FindKid( dwBid[i] ) );
		m_pBid[i] = new CTNumberEdit( this, pEdit->m_pDESC, nMoneyLength[i] );
		m_pBid[i]->ClearText();
		m_pBid[i]->SetText( CTChart::Format( TSTR_FMT_NUMBER, 0 ) );
		m_pBid[i]->EnableComponent( TRUE );

		AddKid( m_pBid[i] );
		RemoveKid( pEdit );
		delete pEdit;
		pEdit = NULL;
		
		pEdit = static_cast< TEdit* >( FindKid( dwBuy[i][0] ) );
		m_pBuy[i] = new CTNumberEdit( this, pEdit->m_pDESC, nMoneyLength[i] );
		m_pBuy[i]->ClearText();
		m_pBuy[i]->SetText( CTChart::Format( TSTR_FMT_NUMBER, 0 ) );
		m_pBuy[i]->EnableComponent( TRUE );
		m_pTextBuy[i] = FindKid( dwBuy[i][1] );

		AddKid( m_pBuy[i] );
		RemoveKid( pEdit );
		delete pEdit;
	}

	ResetInfo();
}

//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.
CTAuctionRegist::~CTAuctionRegist()
{

}


void CTAuctionRegist::RequestInfo()
{
	CTAuctionInnerFrame::RequestInfo();

	CTAuctionCommander::GetInstance()
		->RequestRegistList( GetFindInfo() );
}

void	CTAuctionRegist::RequestUpdatePage()
{
	CTAuctionInnerFrame::RequestUpdatePage();

	CTAuctionCommander::GetInstance()
		->RequestRegistList( GetFindInfo() );
}

void CTAuctionRegist::ResetInfo()
{
	m_dwTerm = TTERM_BUTTON_COUNT-1;
	m_pItem = NULL;

	m_RegInfo.Clear();

	UpdateByComp();
}

void CTAuctionRegist::OnLButtonDown( UINT nFlags, CPoint pt )
{
	UpdateEditable( pt );

	if( IsVisible() )
	{
		ITInnerFrame::OnLButtonDown( nFlags, pt );

		DWORD dwTemp = m_dwTerm;

		size_t i = 0;
		for( ; i<TTERM_BUTTON_COUNT; ++i )
		{
			if( m_dwTerm != i &&
				m_pTermBtn[i]->IsStateDown() )
			{
				dwTemp = i;
				m_pTermBtn[i]->Select( TRUE );
			}
			else
			{
				m_pTermBtn[i]->Select( FALSE );
			}
		}

		m_dwTerm = dwTemp;

		for( i = 0; i<TTERM_BUTTON_COUNT; ++i )
			m_pTermBtn[i]->Select( (m_dwTerm == i) ? TRUE : FALSE );
	}
}

void	CTAuctionRegist::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags )
{
	ITInnerFrame::OnKeyDown( nChar, nRepCnt, nFlags );

	if( !m_pItem ||
		!CanProcess() )
		return ;

	//! 경매시작가를 즉시구매가 보다 높게 설정하면
	//! UpdateByNumber함수에서 즉시구매가를 입력하는 경매시작가와 똑같이 세팅된다.
	//! 반대로 즉시구매가를 보다 작게 입력하게 되면 [경매시작가 > 즉시구매가] 가 될 수 있기때문에,
	//! 이를 방지하기 위해 Delete 키를 눌렀을때 UpdateByNumber를 호출해준다.

	switch( nChar )
	{
	case VK_DELETE :
		UpdateByNumber();
		break;
	};
}

void	CTAuctionRegist::OnChar( UINT nChar, int nRepCnt, UINT nFlags )
{
	ITInnerFrame::OnChar( nChar, nRepCnt, nFlags );

	if( !m_pItem ||
		!CanProcess() )
		return ;

	UpdateByNumber();
}

BYTE CTAuctionRegist::OnBeginDrag( LPTDRAG pDRAG, CPoint point )
{
	if( m_pItem && m_pIcon->HitTest( point ) )
	{
		if( pDRAG )
		{
			pDRAG->m_pIMAGE = new TImageList(NULL, m_pIcon->m_pDESC);

			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual( m_pItem->GetTITEM() );
			WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

			pDRAG->m_pIMAGE->SetCurImage(wImg);
			pDRAG->m_bSlotID = 0;

			DWORD qt = m_pItem->GetQuality();

			if( m_pItem->GetWrap() )
				pDRAG->m_pIMAGE->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
			else if ( qt & TITEM_QUALITY_GRADE )
				pDRAG->m_pIMAGE->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pItem->GetGrade() );
			else if ( qt & TITEM_QUALITY_RARE )
				pDRAG->m_pIMAGE->SetSkinImage( TITEM_INDEX_RARE_SKIN );
			else if ( qt & TITEM_QUALITY_MAGIC )
				pDRAG->m_pIMAGE->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
			else
				pDRAG->m_pIMAGE->SetSkinImageEmpty();

			CPoint pt = point;
			CSize size;

			pDRAG->m_pIMAGE->GetComponentSize(&size);
			pt.x -= size.cx / 2;
			pt.y -= size.cy / 2;

			pDRAG->m_pIMAGE->ShowComponent(FALSE);
			pDRAG->m_pIMAGE->MoveComponent(pt);
		}
	}

	return TRUE;
}

TDROPINFO CTAuctionRegist::OnDrop( CPoint point )
{
	TDROPINFO vResult;

	if( m_bDropTarget )
		vResult.m_bDrop = TRUE;

	return vResult;
}

ITDetailInfoPtr	CTAuctionRegist::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	if( !IsVisible() )
		return pInfo;

	if( m_pIcon &&
		m_pItem &&
		m_pIcon->HitTest( point ) )
	{
		CPoint	ptImage;
		CRect	rcImage;

		m_pIcon->GetComponentRectWithKid( &rcImage );
		m_pIcon->ComponentToScreen( &rcImage );

		pInfo = CTDetailInfoManager::NewItemInst( m_pItem, rcImage );
		pInfo->SetDir( TRUE, FALSE, TRUE );
	}

	return pInfo;
}
//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
void	CTAuctionRegist::UpdateEditable( CPoint pt )
{
	size_t i = 0;
	for( ; i < TMONEY_COUNT; ++i )
	{
		BOOL bHit = m_pBid[i]->HitTest( pt );
		m_pBid[i]->EnableComponent( bHit );
		m_pBid[i]->SetFocus( bHit );

		bHit = m_pBuy[i]->HitTest( pt );
		m_pBuy[i]->EnableComponent( bHit );
		m_pBuy[i]->SetFocus( bHit );
	}
}

void	CTAuctionRegist::UpdateRegInfo()
{
	WORD wHour[TTERM_BUTTON_COUNT] =
	{
		6, 12, 24, 48
	};

	m_RegInfo.wHour = (WORD)wHour[m_dwTerm];
	m_RegInfo.bCount = m_pItem->GetCount();
	m_RegInfo.bItemID = (BYTE)m_pItem->GetItemSlot();
	m_RegInfo.dwCode = m_pItem->GetTITEM()->m_dwAuctionCode;

	m_RegInfo.ldwStartPrice = GetStartPrice();

	if( m_pItem->GetTITEM()->m_bStack == 1 )
		m_RegInfo.ldwDirectPrice = GetBuyPrice();
	else
		m_RegInfo.ldwDirectPrice = GetTotalPrice();
}

void	CTAuctionRegist::UpdateByComp()
{
	// 아이템정보.
	if( m_pItem )
	{
		LPTITEMVISUAL pTVISUAL = m_pItem->GetVisual();

		WORD wID = pTVISUAL ? pTVISUAL->m_wIcon : 0;
		m_pIcon->SetCurImage( wID );
		m_pIcon->ShowComponent( TRUE );

		DWORD qt = m_pItem->GetQuality();

		if( m_pItem->GetWrap() )
			m_pIcon->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
		else if ( qt & TITEM_QUALITY_GRADE )
			m_pIcon->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pItem->GetGrade() );
		else if ( qt & TITEM_QUALITY_RARE )
			m_pIcon->SetSkinImage( TITEM_INDEX_RARE_SKIN );
		else if ( qt & TITEM_QUALITY_MAGIC )
			m_pIcon->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
		else
			m_pIcon->SetSkinImageEmpty();

		// 업그레이드 수치 텍스트.
		if( m_pItem->GetGrade() > 0 )
			m_pUpgrade->m_strText.Format( "+%d", m_pItem->GetGrade() );
		else
			m_pUpgrade->m_strText.Empty();

		// 아이템이름 텍스트.
		m_pName->m_strText = m_pItem->GetTITEM()->m_strNAME;
		
		if( m_pItem->GetTITEM()->m_bStack > 1 )
			m_pItemCount->m_strText = CTChart::Format( TSTR_FMT_AMOUNT, m_RegInfo.bCount );
		else
			m_pItemCount->m_strText.Empty();
	}
	else
	{
		m_pIcon->SetCurImage( 0 );
		m_pIcon->ShowComponent( FALSE );
		m_pUpgrade->ShowComponent( FALSE );
		m_pName->m_strText = "";
		m_pItemCount->m_strText = CTChart::Format( TSTR_FMT_AMOUNT, m_RegInfo.bCount );
	}

	// 시간세팅.
	size_t i = 0;
	for( ; i < TTERM_BUTTON_COUNT; ++i )
		m_pTermBtn[i]->Select( (BOOL)(i == m_dwTerm) );

	// 시작가, 즉시구매가 세팅.
	DWORD dwStartPrice[ TMONEY_COUNT ] = {0};
	
	CTClientGame::SplitMoney(
		m_RegInfo.ldwStartPrice,
		&dwStartPrice[ TMONEY_RUNE ],
		&dwStartPrice[ TMONEY_LUNA ],
		&dwStartPrice[ TMONEY_CRON ] );

	DWORD dwBuyPrice[ TMONEY_COUNT ] = {0};

	CTClientGame::SplitMoney(
		m_RegInfo.ldwDirectPrice,
		&dwBuyPrice[ TMONEY_RUNE ],
		&dwBuyPrice[ TMONEY_LUNA ],
		&dwBuyPrice[ TMONEY_CRON ] );

	// UpdateByComponent.
	for( i = 0; i < TMONEY_COUNT; ++i )
	{
		m_pBid[i]->ShowComponent( TRUE );
		m_pBid[i]->ClearText();
		m_pBid[i]->SetText( CTChart::Format( TSTR_FMT_NUMBER, dwStartPrice[i] ) );

		m_pBuy[i]->ShowComponent( FALSE );
		m_pBuy[i]->ClearText();
		m_pTextBuy[i]->ShowComponent( TRUE );
		m_pTextBuy[i]->ClearText();
		m_pTextBuy[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwBuyPrice[i] );

		if( m_pItem )
		{
			BOOL bStack = (m_pItem->GetTITEM()->m_bStack > 1) ? TRUE : FALSE;
			m_pBuy[i]->ShowComponent( !bStack );
			m_pTextBuy[i]->ShowComponent( bStack );

			m_pTextBuy[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwBuyPrice[i] );

			if( !bStack )
			{
				m_pBuy[i]->ClearText();
				m_pBuy[i]->SetText( CTChart::Format( TSTR_FMT_NUMBER, dwBuyPrice[i] ) );

				m_pTitleStartPrice->m_strText = CTChart::Format( TSTR_AUCTIONREG_TEXTSTARTPRICE );
				m_pTitleBuyPrice->m_strText = CTChart::Format( TSTR_AUCTIONREG_TEXTBUYPRICE );
			}
			else
			{
				m_pTitleStartPrice->m_strText = CTChart::Format( TSTR_AUCTIONREG_TEXTUNITPRICE );
				m_pTitleBuyPrice->m_strText = CTChart::Format( TSTR_AUCTIONREG_TEXTTOTALPRICE );
			}
		}
	}
}

void	CTAuctionRegist::UpdateByNumber()
{
	BOOL bStack = (m_pItem->GetTITEM()->m_bStack > 1) ? TRUE : FALSE;

	if( bStack )
	{
		BOOL bEditPrice = FALSE;

		size_t i = 0;
		for( ; i < TMONEY_COUNT; ++i )
			if( !bEditPrice && m_pBid[i] == m_pFocus )
				bEditPrice = TRUE;

		if( bEditPrice )
		{
			DWORD dwMoney[ TMONEY_COUNT ];
	
			for( i = 0; i < TMONEY_COUNT; ++i )
				dwMoney[i] = atoi( m_pBid[i]->m_strText );

			INT64 ldwPrice = GetStartPrice();
			INT64 ldwTotalPrice = ldwPrice * m_pItem->GetCount();
			SetTotalPrice( ldwTotalPrice );

			DWORD dwTotalPrice[ TMONEY_COUNT ];
			CTClientGame::SplitMoney(
				ldwTotalPrice,
				&dwTotalPrice[ TMONEY_RUNE ],
				&dwTotalPrice[ TMONEY_LUNA ],
				&dwTotalPrice[ TMONEY_CRON ] );

			for( i = 0; i < TMONEY_COUNT; ++i )
				m_pTextBuy[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwTotalPrice[i] );
		}
	}
	else
	{
		BOOL bEditBid = FALSE;

		size_t i = 0;
        for( ; i < TMONEY_COUNT; ++i )
			if( !bEditBid && m_pBid[i] == m_pFocus )
				bEditBid = TRUE;

		if( bEditBid )
		{
			INT64 ldwStartPrice = GetStartPrice();
			INT64 ldwBuyPrice = GetBuyPrice();

			if( ldwStartPrice >= ldwBuyPrice )
			{
				ldwBuyPrice = ldwStartPrice + 1;
				SetBuyPrice( ldwBuyPrice );

				DWORD dwBuyPrice[ TMONEY_COUNT ];
				CTClientGame::SplitMoney(
					ldwBuyPrice,
					&dwBuyPrice[ TMONEY_RUNE ],
					&dwBuyPrice[ TMONEY_LUNA ],
					&dwBuyPrice[ TMONEY_CRON ] );

				for( i = 0; i<TMONEY_COUNT; ++i )
				{
					m_pBuy[i]->ClearText();
					m_pBuy[i]->SetText( CTChart::Format( TSTR_FMT_NUMBER, dwBuyPrice[i] ) );
				}
			}
		}
	}
}

BOOL	CTAuctionRegist::CheckInvalidPrice()
{
	if( m_RegInfo.ldwStartPrice == 0 )
		return TRUE;

	return TRUE;
}

TEdit* CTAuctionRegist::GetCurEdit()
{
	if( !CanProcess() && !m_bEditEnable )
		return NULL;

	for( int i=0; i<TMONEY_COUNT; ++i )
	{
		if( m_pBid[i] &&
			m_pBid[i]->CanProcess() &&
			m_pBid[i] == m_pFocus )
			return m_pBid[i];

		if( m_pBuy[i] &&
			m_pBuy[i]->CanProcess() &&
			m_pBuy[i] == m_pFocus )
			return m_pBuy[i];
	}

	return NULL;
}
//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
void	CTAuctionRegist::SetItem( CTClientItem* pItem )
{
	m_pItem = pItem;
}

void	CTAuctionRegist::SetInvenID( BYTE bInvenID )
{
	m_RegInfo.bInvenID = bInvenID;
}

void	CTAuctionRegist::SetItemCount( BYTE bCount )
{
	m_RegInfo.bCount = bCount;
}

void	CTAuctionRegist::SetStartPrice( INT64 dwPrice )
{
	m_RegInfo.ldwStartPrice = dwPrice;
}

void	CTAuctionRegist::SetBuyPrice( INT64 dwPrice )
{
	m_RegInfo.ldwDirectPrice = dwPrice;
}

void	CTAuctionRegist::SetUnitPrice( INT64 dwPrice )
{
	m_RegInfo.ldwStartPrice = dwPrice;
}

void	CTAuctionRegist::SetTotalPrice( INT64 dwPrice )
{
	m_RegInfo.ldwDirectPrice = dwPrice;
}

void	CTAuctionRegist::SetEditEnable( BOOL bEnable )
{
	m_bEditEnable = bEnable;
}

void	CTAuctionRegist::SetEditUnitPrice_RuneLength( INT nRuneLength )
{
	static_cast< CTNumberEdit* >( m_pBid[ TMONEY_RUNE ] )->SetValidCount( nRuneLength );
}

CTClientItem*	CTAuctionRegist::GetItem()
{
	return m_pItem;
}

LPTAUCTIONREGINFO	CTAuctionRegist::GetRegistInfo()
{
	return &m_RegInfo;
}

INT64	CTAuctionRegist::GetStartPrice()
{
	INT64	ldwPrice = 0;

	DWORD dwRune = (DWORD) atoi( m_pBid[ TMONEY_RUNE ]->m_strText );
	DWORD dwLuna = (DWORD) atoi( m_pBid[ TMONEY_LUNA ]->m_strText );
	DWORD dwCron = (DWORD) atoi( m_pBid[ TMONEY_CRON ]->m_strText );

	CTClientGame::AddUpMoney(
		&ldwPrice,
		dwRune,
		dwLuna,
		dwCron );

    return ldwPrice;
}

INT64	CTAuctionRegist::GetBuyPrice()
{
	INT64	ldwPrice = 0;

	DWORD dwRune = (DWORD) atoi( m_pBuy[ TMONEY_RUNE ]->m_strText );
	DWORD dwLuna = (DWORD) atoi( m_pBuy[ TMONEY_LUNA ]->m_strText );
	DWORD dwCron = (DWORD) atoi( m_pBuy[ TMONEY_CRON ]->m_strText );

	CTClientGame::AddUpMoney(
		&ldwPrice,
		dwRune,
		dwLuna,
		dwCron );

	return ldwPrice;
}

INT64	CTAuctionRegist::GetTotalPrice()
{
	INT64	ldwPrice = 0;

	DWORD dwRune = (DWORD) atoi( m_pTextBuy[ TMONEY_RUNE ]->m_strText );
	DWORD dwLuna = (DWORD) atoi( m_pTextBuy[ TMONEY_LUNA ]->m_strText );
	DWORD dwCron = (DWORD) atoi( m_pTextBuy[ TMONEY_CRON ]->m_strText );

	CTClientGame::AddUpMoney(
		&ldwPrice,
		dwRune,
		dwLuna,
		dwCron );

	return ldwPrice;
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
