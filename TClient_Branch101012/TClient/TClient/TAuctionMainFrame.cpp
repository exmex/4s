#include "stdafx.h"
#include "TClientGame.h"


void _get_hmsTime( DWORD dwTick, WORD* _hour, WORD* _minute, WORD* _second )
{
//	dwTick /= 1000;

	if( _hour != NULL )
	{
		*_hour = (WORD)(dwTick / 3600);
		dwTick -= *_hour * 3600;
	}
	if( _minute != NULL )
	{
		*_minute = (WORD)(dwTick / 60);
		dwTick -= *_minute * 60;
	}
	if( _second != NULL )
	{
		*_second = (WORD)dwTick;
	}
}

//////////////////////////////////////////////////////////////////////////
//	AUCTION ITEM IMPLEMENTATION.
CTAuctionItem::CTAuctionItem()
:	m_pItem( NULL ),
	m_dwAuctionID( 0 ),
	m_dwRemainTick( 0 ),
	m_nEndTime( 0 ),
	m_bBidCount( 0 ),
	m_dwCode( 0 )
{
	m_pItem = new CTClientItem;

	m_strSeller.Empty();

	size_t i =0;
	for( ; i < TMONEY_COUNT; ++i )
	{
		m_dwBidMoney[i] = 0;
		m_dwBuyMoney[i] = 0;
	}
}

CTAuctionItem::~CTAuctionItem()
{
	if( m_pItem )
	{
		delete m_pItem;
		m_pItem = NULL;
	}
}

void CTAuctionItem::ReflectSlot(CTCtrlListSlot* pSlot)
{
	if( !m_pItem )	return ;

	CTAuctionSlot* pAuctionSlot = static_cast< CTAuctionSlot* >( pSlot );

	// 아이템 아이콘이미지 세팅.
	LPTITEMVISUAL pVisual = m_pItem->GetDefaultVisual();
	if( pVisual )
		pAuctionSlot->m_pImageList->SetCurImage( pVisual->m_wIcon );

	DWORD qt = m_pItem->GetQuality();

	if( m_pItem->GetWrap() )
		pAuctionSlot->m_pImageList->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
	else if ( qt & TITEM_QUALITY_GRADE )
		pAuctionSlot->m_pImageList->SetSkinImage( TITEM_INDEX_GRADE_SKIN + m_pItem->GetGrade() );
	else if ( qt & TITEM_QUALITY_RARE )
		pAuctionSlot->m_pImageList->SetSkinImage( TITEM_INDEX_RARE_SKIN );
	else if ( qt & TITEM_QUALITY_MAGIC )
		pAuctionSlot->m_pImageList->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
	else
		pAuctionSlot->m_pImageList->SetSkinImageEmpty();


	// 아이템 갯수에 따라 처리.
	if( m_pItem->GetTITEM()->m_bStack > 1 )
	{
		pAuctionSlot->m_pImageList->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pItem->GetCount() );

		pAuctionSlot->m_pBuy->ShowComponent( FALSE );
		pAuctionSlot->m_pBid->ShowComponent( FALSE );
		pAuctionSlot->m_pPrice->ShowComponent( TRUE );
		pAuctionSlot->m_pTotal->ShowComponent( TRUE );
	}
	else
	{
		pAuctionSlot->m_pImageList->ClearText();

		pAuctionSlot->m_pBuy->ShowComponent( TRUE );
		pAuctionSlot->m_pBid->ShowComponent( TRUE );
		pAuctionSlot->m_pPrice->ShowComponent( FALSE );
		pAuctionSlot->m_pTotal->ShowComponent( FALSE );
	}

	if( m_pItem->GetGrade() > 0 )
		pAuctionSlot->m_pUpgrade->m_strText.Format( "+%d", m_pItem->GetGrade() );	// 강화레벨
	else
		pAuctionSlot->m_pUpgrade->m_strText.Empty();

	pAuctionSlot->m_pName->m_strText = m_pItem->GetTITEM()->m_strNAME; // 아이템이름
	pAuctionSlot->m_pNeedLevel->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pItem->GetTITEM()->m_bLevel ); // 착용레벨

	// 착용 직업.
	static CString strClassName[TCLASS_COUNT] = 
	{
		CTChart::Format( TSTR_TCLASS_WARRIOR ),	// 전사
		CTChart::Format( TSTR_TCLASS_RANGER ),	// 살수
		CTChart::Format( TSTR_TCLASS_ARCHER ),	// 궁사
		CTChart::Format( TSTR_TCLASS_WIZARD ),	// 마법사
		CTChart::Format( TSTR_TCLASS_PRIEST ),	// 사제
		CTChart::Format( TSTR_TCLASS_SORCERER )	// 소환사
	};

	CString strClass = "";
	DWORD dwClassID = m_pItem->GetTITEM()->m_dwClassID;
	
	if( dwClassID == ( 1 << TCLASS_COUNT ) - 1 )
	{
		strClass = CTChart::Format( TSTR_CHAT_NATION );	// 전체
	}
	else
	{
		size_t i = 0;
		for( ; i < TCLASS_COUNT; ++i )
		{
			DWORD dwClass = 1 << i;
			if( dwClass & m_pItem->GetTITEM()->m_dwClassID )
			{
				if( !strClass.IsEmpty() )
					strClass += ", ";

				strClass += strClassName[i];
			}
		}
	}

	pAuctionSlot->m_pClass->m_strText = strClass;

	pAuctionSlot->m_pRune[0]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBidMoney[TMONEY_RUNE] );
	pAuctionSlot->m_pLuna[0]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBidMoney[TMONEY_LUNA] );
	pAuctionSlot->m_pCron[0]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBidMoney[TMONEY_CRON] );

	pAuctionSlot->m_pRune[1]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBuyMoney[TMONEY_RUNE] );
	pAuctionSlot->m_pLuna[1]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBuyMoney[TMONEY_LUNA] );
	pAuctionSlot->m_pCron[1]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwBuyMoney[TMONEY_CRON] );

	pAuctionSlot->m_pSeller->m_strText = m_strSeller;

	pAuctionSlot->m_pLeftTime->ClearText();
	
	WORD h = 0;
	WORD m = 0;
	WORD s = 0;

	_get_hmsTime( m_dwRemainTick, &h, &m, &s );	// m_dwRemainTick -> 초단위.
	pAuctionSlot->m_pLeftTime->m_strText.Format( "%02d : %02d", h, m );

	pAuctionSlot->m_pBidCount->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_bBidCount );
}
//	END OF AUCTION ITEM.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	AUCTION SLOT IMPLEMENTATION.
CTAuctionSlot::CTAuctionSlot()
:	m_pImageSlot( NULL ),
m_pImageList( NULL ),
m_pUpgrade( NULL ),
m_pName( NULL ),
m_pClass( NULL ),
m_pNeedLevel( NULL ),
m_pBid( NULL ),
m_pBuy( NULL ),
m_pPrice( NULL ),
m_pTotal( NULL ),
m_pImageMoney1( NULL ),
m_pImageMoney2( NULL ),
m_pSeller( NULL ),
m_pLeftTime( NULL ),
m_pBidCount( NULL )
{
	size_t i;
	for( i=0; i<2; ++i )
	{
		m_pRune[i] = NULL;
		m_pLuna[i] = NULL;
		m_pCron[i] = NULL;
	}
}

CTAuctionSlot::~CTAuctionSlot()
{

}

void CTAuctionSlot::ShowComponent( BOOL bShow )
{
	m_bShow = bShow;

	TComponent* pComp[18] = 
	{
		m_pBase,
		m_pImageSlot,
		m_pImageList,
		m_pUpgrade,
		m_pName,
		m_pClass,
		m_pNeedLevel,
		m_pImageMoney1,
		m_pImageMoney2,
		m_pSeller,
		m_pLeftTime,
		m_pBidCount,
		m_pRune[0],
		m_pLuna[0],
		m_pCron[0],
		m_pRune[1],
		m_pLuna[1],
		m_pCron[1],
	};

	for( int i=0; i<18; ++i )
		pComp[i]->ShowComponent( bShow );


	TComponent* pHide[4] =
	{
		m_pBid,
		m_pBuy,
		m_pPrice,
		m_pTotal,
	};

	if( !bShow )
	{
		size_t i = 0;
		for( ; i<4; ++i )
			pHide[i]->ShowComponent( bShow );
	}
}

void CTAuctionSlot::Select(BOOL bSel)
{
	static_cast< TButton* >( m_pBase )->Select( bSel );
}
//	END OF AUCTION SLOT.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	AUCTION_MAIN_FRAME CONSTRUCTOR.
CTAuctionMainFrame::CTAuctionMainFrame( TComponent *pParent, LP_FRAMEDESC pDesc )
:	CTFrameGroupBase( pParent, pDesc, ID_CTRLINST_INNERPOS )
{
	LP_FRAMEDESC pListDesc = CTClientGame::GetInstance()->m_pTParser->FindFrameTemplate( ID_FRAME_AUCTION_LIST );
	m_pAuctionListFrame = new TFrame( NULL, pListDesc );
	m_pAuctionListFrame->m_id = GetUniqueID();

	TComponent* pInnerPos = FindKid( ID_CTRLINST_INNERPOS_LIST );
	CPoint ptList;
	pInnerPos->GetComponentPos( &ptList );

	m_pAuctionListFrame->MoveComponent( ptList );
	AddKid( m_pAuctionListFrame );

	DWORD dwSlots[ MAX_ITEM_SLOT ] =
	{
		ID_CTRLINST_SLOT1,
		ID_CTRLINST_SLOT2,
		ID_CTRLINST_SLOT3,
		ID_CTRLINST_SLOT4,
		ID_CTRLINST_SLOT5,
		ID_CTRLINST_SLOT6,
		ID_CTRLINST_SLOT7,
		ID_CTRLINST_SLOT8,
		ID_CTRLINST_SLOT9
	};

	DWORD dwHead[ TCOLUMN_HEAD_COUNT ] =
	{
		ID_CTRLINST_HEAD1,
		ID_CTRLINST_HEAD2,
		ID_CTRLINST_HEAD3,
		ID_CTRLINST_HEAD4,
		ID_CTRLINST_HEAD5,
		ID_CTRLINST_HEAD6,
		ID_CTRLINST_HEAD7
	};

	CTCLSlotArray vSlots;
	CTAuctionSlot* pSlot;

	INT i = 0;
	for( ; i < MAX_ITEM_SLOT; ++i )
	{
		pSlot = new CTAuctionSlot;
		pSlot->SetSlot( i, m_pAuctionListFrame->FindKid( dwSlots[ i ] ) );
		vSlots.push_back( pSlot );
	}

	for( i = 0; i < TCOLUMN_HEAD_COUNT; ++i )
	{
		m_pHeadBtn[i] = static_cast< TButton* >( m_pAuctionListFrame->FindKid( dwHead[i] ) );
		m_strHeadOrigText[i] = m_pHeadBtn[i]->m_strText;
	}

	CTAuctionSlot* pSlot0;
	CPoint ptBase, ptTemp;

	pSlot0 = static_cast< CTAuctionSlot* >( vSlots[0] );
	pSlot0->m_pBase->GetComponentPos( &ptBase );

	pSlot0->m_pImageSlot = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_ICONSLOT ) );
	pSlot0->m_pImageSlot->GetComponentPos( &ptTemp );
	CPoint ptImageSlot_Offset = ptTemp - ptBase;

	pSlot0->m_pImageList = static_cast< TImageList* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_ICON ) );
	pSlot0->m_pImageList->GetComponentPos( &ptTemp );
	pSlot0->m_pImageList->m_strText = "";
	CPoint ptImageList_Offset = ptTemp - ptBase;

	pSlot0->m_pUpgrade = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_UPGRADE ) );
	pSlot0->m_pUpgrade->GetComponentPos( &ptTemp );
	pSlot0->m_pUpgrade->m_strText = "";
	CPoint ptUpgrade_Offset = ptTemp - ptBase;

	pSlot0->m_pName = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT1 ) );
	pSlot0->m_pName->GetComponentPos( &ptTemp );
	pSlot0->m_pName->m_strText = "";
	CPoint ptName_Offset = ptTemp - ptBase;

	pSlot0->m_pClass = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT2 ) );
	pSlot0->m_pClass->GetComponentPos( &ptTemp );
	pSlot0->m_pClass->m_strText = "";
	CPoint ptClass_Offset = ptTemp - ptBase;

	pSlot0->m_pNeedLevel = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT3 ) );
	pSlot0->m_pNeedLevel->GetComponentPos( &ptTemp );
	pSlot0->m_pNeedLevel->m_strText = "";
	CPoint ptNeedLevel_Offset = ptTemp - ptBase;

	pSlot0->m_pBid = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT4 ) );
	pSlot0->m_pBid->GetComponentPos( &ptTemp );
	CPoint ptBid_Offset = ptTemp - ptBase;

	pSlot0->m_pPrice = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT41 ) );
	pSlot0->m_pPrice->GetComponentPos( &ptTemp );
	CPoint ptPrice_Offset = ptTemp - ptBase;

	pSlot0->m_pBuy = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT5 ) );
	pSlot0->m_pBuy->GetComponentPos( &ptTemp );
	CPoint ptBuy_Offset = ptTemp - ptBase;

	pSlot0->m_pTotal = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT51 ) );
	pSlot0->m_pTotal->GetComponentPos( &ptTemp );
	CPoint ptTotal_Offset = ptTemp - ptBase;

	pSlot0->m_pImageMoney1 = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_MONEY1 ) );
	pSlot0->m_pImageMoney1->GetComponentPos( &ptTemp );
	CPoint ptImgMoney_Offset = ptTemp - ptBase;

	pSlot0->m_pImageMoney2 = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_MONEY2 ) );
	pSlot0->m_pImageMoney2->GetComponentPos( &ptTemp );
	CPoint ptImgMoney2_Offset = ptTemp - ptBase;

	pSlot0->m_pRune[0] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_RUNE1 ) );
	pSlot0->m_pRune[0]->GetComponentPos( &ptTemp );
	pSlot0->m_pRune[0]->m_strText = "";
	CPoint ptRune_Offset = ptTemp - ptBase;

	pSlot0->m_pLuna[0] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_RUNA1 ) );
	pSlot0->m_pLuna[0]->GetComponentPos( &ptTemp );
	pSlot0->m_pLuna[0]->m_strText = "";
	CPoint ptLuna_Offset = ptTemp - ptBase;

	pSlot0->m_pCron[0] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_CRONE1 ) );
	pSlot0->m_pCron[0]->GetComponentPos( &ptTemp );
	pSlot0->m_pCron[0]->m_strText = "";
	CPoint ptCron_Offset = ptTemp - ptBase;

	pSlot0->m_pRune[1] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_RUNE2 ) );
	pSlot0->m_pRune[1]->GetComponentPos( &ptTemp );
	pSlot0->m_pRune[1]->m_strText = "";
	CPoint ptRune2_Offset = ptTemp - ptBase;

	pSlot0->m_pLuna[1] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_RUNA2 ) );
	pSlot0->m_pLuna[1]->GetComponentPos( &ptTemp );
	pSlot0->m_pLuna[1]->m_strText = "";
	CPoint ptLuna2_Offset = ptTemp - ptBase;

	pSlot0->m_pCron[1] = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_CRONE2 ) );
	pSlot0->m_pCron[1]->GetComponentPos( &ptTemp );
	pSlot0->m_pCron[1]->m_strText = "";
	CPoint ptCron2_Offset = ptTemp - ptBase;

	pSlot0->m_pSeller = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT6 ) );
	pSlot0->m_pSeller->GetComponentPos( &ptTemp );
	pSlot0->m_pSeller->m_strText = "";
	CPoint ptSeller_Offset = ptTemp - ptBase;

	pSlot0->m_pLeftTime = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT7 ) );
	pSlot0->m_pLeftTime->GetComponentPos( &ptTemp );
	pSlot0->m_pLeftTime->m_strText = "";
	CPoint ptLeftTime_Offset = ptTemp - ptBase;

	pSlot0->m_pBidCount = static_cast< TComponent* >( m_pAuctionListFrame->FindKid( ID_CTRLINST_TEXT8 ) );
	pSlot0->m_pBidCount->GetComponentPos( &ptTemp );
	pSlot0->m_pBidCount->m_strText = "";
	CPoint ptBidCount_Offset = ptTemp - ptBase;

	for( i=1; i < MAX_ITEM_SLOT; ++i )
	{
		pSlot = static_cast< CTAuctionSlot* >( vSlots[i] );
		pSlot->m_pImageSlot = new TComponent( this, pSlot0->m_pImageSlot->m_pDESC );
		pSlot->m_pImageList = new TImageList( this, pSlot0->m_pImageList->m_pDESC );
		pSlot->m_pUpgrade = new TComponent( this, pSlot0->m_pUpgrade->m_pDESC );
		pSlot->m_pName = new TComponent( this, pSlot0->m_pName->m_pDESC );
		pSlot->m_pClass = new TComponent( this, pSlot0->m_pClass->m_pDESC );
		pSlot->m_pNeedLevel = new TComponent( this, pSlot0->m_pNeedLevel->m_pDESC );
		pSlot->m_pBid = new TComponent( this, pSlot0->m_pBid->m_pDESC );
		pSlot->m_pBuy = new TComponent( this, pSlot0->m_pBuy->m_pDESC );
		pSlot->m_pPrice = new TComponent( this, pSlot0->m_pPrice->m_pDESC );
		pSlot->m_pTotal = new TComponent( this, pSlot0->m_pTotal->m_pDESC );
		pSlot->m_pImageMoney1 = new TComponent( this, pSlot0->m_pImageMoney1->m_pDESC );
		pSlot->m_pImageMoney2 = new TComponent( this, pSlot0->m_pImageMoney2->m_pDESC );
		pSlot->m_pSeller = new TComponent( this, pSlot0->m_pSeller->m_pDESC );
		pSlot->m_pLeftTime = new TComponent( this, pSlot0->m_pLeftTime->m_pDESC );
		pSlot->m_pBidCount = new TComponent( this, pSlot0->m_pBidCount->m_pDESC );

		m_pAuctionListFrame->RemoveKid( pSlot->m_pImageSlot );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pImageList );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pUpgrade );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pName );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pClass );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pNeedLevel );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pBid );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pBuy );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pPrice );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pTotal );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pImageMoney1 );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pImageMoney2 );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pSeller );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pLeftTime );
		m_pAuctionListFrame->RemoveKid( pSlot->m_pBidCount );

		m_pAuctionListFrame->AddKid( pSlot->m_pImageSlot );
		m_pAuctionListFrame->AddKid( pSlot->m_pImageList );
		m_pAuctionListFrame->AddKid( pSlot->m_pUpgrade );
		m_pAuctionListFrame->AddKid( pSlot->m_pName );
		m_pAuctionListFrame->AddKid( pSlot->m_pClass );
		m_pAuctionListFrame->AddKid( pSlot->m_pNeedLevel );
		m_pAuctionListFrame->AddKid( pSlot->m_pBid );
		m_pAuctionListFrame->AddKid( pSlot->m_pBuy );
		m_pAuctionListFrame->AddKid( pSlot->m_pPrice );
		m_pAuctionListFrame->AddKid( pSlot->m_pTotal );
		m_pAuctionListFrame->AddKid( pSlot->m_pImageMoney1 );
		m_pAuctionListFrame->AddKid( pSlot->m_pImageMoney2 );
		m_pAuctionListFrame->AddKid( pSlot->m_pSeller );
		m_pAuctionListFrame->AddKid( pSlot->m_pLeftTime );
		m_pAuctionListFrame->AddKid( pSlot->m_pBidCount );

		pSlot->m_pImageSlot->m_id = GetUniqueID();
		pSlot->m_pImageList->m_id = GetUniqueID();
		pSlot->m_pUpgrade->m_id = GetUniqueID();
		pSlot->m_pName->m_id = GetUniqueID();
		pSlot->m_pClass->m_id = GetUniqueID();
		pSlot->m_pNeedLevel->m_id = GetUniqueID();
		pSlot->m_pBid->m_id = GetUniqueID();
		pSlot->m_pBuy->m_id = GetUniqueID();
		pSlot->m_pPrice->m_id = GetUniqueID();
		pSlot->m_pTotal->m_id = GetUniqueID();
		pSlot->m_pImageMoney1->m_id = GetUniqueID();
		pSlot->m_pImageMoney2->m_id = GetUniqueID();
		pSlot->m_pSeller->m_id = GetUniqueID();
		pSlot->m_pLeftTime->m_id = GetUniqueID();
		pSlot->m_pBidCount->m_id = GetUniqueID();

		pSlot->m_pBase->GetComponentPos( &ptBase );

		pSlot->m_pImageSlot->MoveComponent( ptBase + ptImageSlot_Offset );
		pSlot->m_pImageList->MoveComponent( ptBase + ptImageList_Offset );
		pSlot->m_pUpgrade->MoveComponent( ptBase + ptUpgrade_Offset );
		pSlot->m_pName->MoveComponent( ptBase + ptName_Offset );
		pSlot->m_pClass->MoveComponent( ptBase + ptClass_Offset );
		pSlot->m_pNeedLevel->MoveComponent( ptBase + ptNeedLevel_Offset );
		pSlot->m_pBid->MoveComponent( ptBase + ptBid_Offset );
		pSlot->m_pBuy->MoveComponent( ptBase + ptBuy_Offset );
		pSlot->m_pPrice->MoveComponent( ptBase + ptPrice_Offset );
		pSlot->m_pTotal->MoveComponent( ptBase + ptTotal_Offset );
		pSlot->m_pImageMoney1->MoveComponent( ptBase + ptImgMoney_Offset );
		pSlot->m_pImageMoney2->MoveComponent( ptBase + ptImgMoney2_Offset );
		pSlot->m_pSeller->MoveComponent( ptBase + ptSeller_Offset );
		pSlot->m_pLeftTime->MoveComponent( ptBase + ptLeftTime_Offset );
		pSlot->m_pBidCount->MoveComponent( ptBase + ptBidCount_Offset );

		for( int j=0; j<2; ++j )
		{
			pSlot->m_pRune[j] = new TComponent( this, pSlot0->m_pRune[j]->m_pDESC );
			pSlot->m_pLuna[j] = new TComponent( this, pSlot0->m_pLuna[j]->m_pDESC );
			pSlot->m_pCron[j] = new TComponent( this, pSlot0->m_pCron[j]->m_pDESC );

			m_pAuctionListFrame->RemoveKid( pSlot->m_pRune[j] );
			m_pAuctionListFrame->RemoveKid( pSlot->m_pLuna[j] );
			m_pAuctionListFrame->RemoveKid( pSlot->m_pCron[j] );

			m_pAuctionListFrame->AddKid( pSlot->m_pRune[j] );
			m_pAuctionListFrame->AddKid( pSlot->m_pLuna[j] );
			m_pAuctionListFrame->AddKid( pSlot->m_pCron[j] );

			pSlot->m_pRune[j]->m_id = GetUniqueID();
			pSlot->m_pLuna[j]->m_id = GetUniqueID();
			pSlot->m_pCron[j]->m_id = GetUniqueID();
		}

		pSlot->m_pRune[0]->MoveComponent( ptBase + ptRune_Offset );
		pSlot->m_pRune[1]->MoveComponent( ptBase + ptRune2_Offset );
		pSlot->m_pLuna[0]->MoveComponent( ptBase + ptLuna_Offset );
		pSlot->m_pLuna[1]->MoveComponent( ptBase + ptLuna2_Offset );
		pSlot->m_pCron[0]->MoveComponent( ptBase + ptCron_Offset );
		pSlot->m_pCron[1]->MoveComponent( ptBase + ptCron2_Offset );

		pSlot->m_pClass->ClearText();
		pSlot->m_pLeftTime->ClearText();
		pSlot->m_pBidCount->ClearText();
	}

	m_pCurPage = m_pAuctionListFrame->FindKid( ID_CTRLINST_CURPAGE );
	m_pLastPage = m_pAuctionListFrame->FindKid( ID_CTRLINST_LASTPAGE );

	m_pCurPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, 0 );
	m_pLastPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, 0 );

	m_pAuctionList = new CTAuctionCtrlList;
	m_pAuctionList->SetSlot( &vSlots );

	m_pAuctionList->ClearItems();

	m_wNpcID = 0;

	DWORD dwButtonID[ TBUTTON_COUNT ] =
	{
		ID_CTRLINST_BUTTON1,
		ID_CTRLINST_BUTTON2,
		ID_CTRLINST_BUTTON3
	};

	for( i = 0 ; i<TMSGBUTTON_COUNT; ++i )
		m_pBtn[i] = static_cast< TButton* >( FindKid( dwButtonID[i] ) );
}
//	END OF AUCTION_MAIN_FRAME CONSTRUCTOR.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.
CTAuctionMainFrame::~CTAuctionMainFrame()
{
	if( m_pAuctionList )
	{
		delete m_pAuctionList;
		m_pAuctionList = NULL;
	}
}

void	CTAuctionMainFrame::RequestInfo()
{
	ResetColumnString();

	CTFrameGroupBase::RequestInfo();
}

void	CTAuctionMainFrame::RequestUpdatePage()
{
	INT nSelFrame = GetSelectedFrame();

	if( nSelFrame != T_INVALID )
	{
		CTAuctionInnerFrame* pFrame = 
			static_cast< CTAuctionInnerFrame* >( GetInnerFrame(nSelFrame) );

		pFrame->RequestUpdatePage();
	}
}

void	CTAuctionMainFrame::ResetInfo()
{
	CTFrameGroupBase::ResetInfo();
}

void	CTAuctionMainFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !IsVisible() )
		return ;

	CTCtrlListItem* pItem = m_pAuctionList->GetHitItem( pt );
	if( pItem )
	{
		UINT selected = pItem->GetIndex() % TAUCTION_SLOT_COUNT;
		if( selected != m_pAuctionList->GetSelectedIndex() )	// 아이템선택.
		{
			m_pAuctionList->Select( selected );

			UpdateByButtons();
		}
		else
		{
			m_pAuctionList->Select( T_INVALID );
		}
	}

	CTFrameGroupBase::OnLButtonDown( nFlags, pt );

	CTAuctionInnerFrame* pInnerFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	pInnerFrame->UpdateEditable( pt );
}

void	CTAuctionMainFrame::OnRButtonDown( UINT nFlags, CPoint pt )
{
	// 우클릭 방지.
}

BOOL	CTAuctionMainFrame::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() )
		return FALSE;

	return GetInnerFrame( GetSelectedFrame() )->DoMouseWheel( nFlags, zDelta, pt );
}

ITDetailInfoPtr	CTAuctionMainFrame::GetTInfoKey( const CPoint& pt )
{
	ITDetailInfoPtr pInfo;

	if( !IsVisible() )
		return pInfo;

	pInfo = GetInnerFrame( GetSelectedFrame() )->GetTInfoKey( pt );
	if( pInfo )
		return pInfo;

	size_t size = m_pAuctionList->GetSlotCount();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		CTCtrlListSlot* pSlot = m_pAuctionList->GetSlot( i );
		CTAuctionSlot* pAuctionSlot = static_cast< CTAuctionSlot* >( pSlot );

		if( pAuctionSlot->m_pImageList &&
			pAuctionSlot->m_pImageList->HitTest( pt ) )
		{
			CPoint ptImage;
			CRect rcImage;

			pAuctionSlot->m_pImageList->GetComponentRectWithKid(&rcImage);
			pAuctionSlot->m_pImageList->ComponentToScreen(&rcImage);

			CTAuctionItem* pAuctionItem = static_cast< CTAuctionItem* >( pAuctionSlot->m_pItem );
			if( pAuctionItem &&
				pAuctionItem->m_pItem )
			{
				pInfo = CTDetailInfoManager::NewItemInst( pAuctionItem->m_pItem, rcImage );
				pInfo->SetDir( TRUE, FALSE, TRUE );
			}
		}
	}

	return pInfo;
}

BOOL	CTAuctionMainFrame::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	CTCtrlListItem* pHitItem = m_pAuctionList->GetHitItem( point );
	if( !pHitItem )
		return FALSE;

	CTAuctionItem* pItem = static_cast< CTAuctionItem* >( pHitItem );

	if( pItem->m_pItem )
	{
		outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
		outInfo.m_pItem = pItem->m_pItem->GetTITEM();
		outInfo.m_pClientItem = pItem->m_pItem;
		
		return TRUE;
	}

	return FALSE;
}

TDROPINFO CTAuctionMainFrame::OnDrop( CPoint point )
{
	TDROPINFO vResult;

	ITInnerFrame* pInnerFrame = GetInnerFrame( m_nSelectedFrame );
	if( pInnerFrame &&
		pInnerFrame->IsVisible() )
	{
		return pInnerFrame->OnDrop( point );
	}

	return vResult;
}

void	CTAuctionMainFrame::ShowComponent( BOOL bVisible )
{
	CTFrameGroupBase::ShowComponent( bVisible );
	m_pAuctionList->ShowComponent( bVisible );

	if( bVisible )
	{
		UpdateByListComp();
		m_pAuctionList->MoveFirstPage();

		SetSelect( TAUCTION_SEARCH );
		SetSearchFrame();

		CTAuctionSearch* pFrame = static_cast< CTAuctionSearch* >( GetInnerFrame( TAUCTION_SEARCH ) );
		pFrame->ResetComp();
		pFrame->UpdateByComp();
	}

	CTClientGame* pGame = CTClientGame::GetInstance();
	if( pGame->m_pTRSCS->IsVisible() )
	{
		pGame->m_pTRSCS->m_bMinimize = bVisible;
		pGame->m_pTRSCS->ResetVisible();
	}
	else
	{
		pGame->m_pTMinimap->m_bMinimize = bVisible;
		pGame->m_pTMinimap->ResetVisible();
	}
}

HRESULT	CTAuctionMainFrame::Render( DWORD dwTickCount )
{
	if( !IsVisible() )
		return S_OK;

	m_pAuctionList->Render();

	return CTFrameGroupBase::Render( dwTickCount );
}
//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
void	CTAuctionMainFrame::ClearItems()
{
	if( m_pAuctionList )
	{
		m_pAuctionList->ClearItems();

		UpdateByListComp();
	}
}

void CTAuctionMainFrame::ChangedTab( INT nSelectFrameID, INT nPrevFrameID )
{
	ITInnerFrame* pFrame = GetInnerFrame( nSelectFrameID );

	if( pFrame )
	{
		pFrame->RequestInfo();
	}
}

void	CTAuctionMainFrame::UpdateByListComp()
{
	CTAuctionInnerFrame* pFrame = 
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	m_pCurPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFrame->GetCurPage() );
	m_pLastPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pFrame->GetLastPage() );
}

void	CTAuctionMainFrame::UpdateByButtons()
{
	CTAuctionItem* pSelItem = static_cast< CTAuctionItem* >( GetSelectedItem() );
	
	if( CTClientGame::GetInstance()->GetMainChar()->GetName() == pSelItem->m_strSeller )
	{
		SetRegistFrame();

		return ;
	}
	if( pSelItem->m_pItem->GetTITEM()->m_bStack > 1 )	// 복수개의 아이템.
	{
		if( GetSelectedFrame() == TAUCTION_SEARCH )
			SetStackItem_Search();
		else if( GetSelectedFrame() == TAUCTION_BASKET )
			SetStackItem_Basket();
	}
	else
	{
		if( GetSelectedFrame() == TAUCTION_SEARCH )
			SetCanBidItem_Search();
		else if( GetSelectedFrame() == TAUCTION_BASKET )
			SetCanBidItem_Basket();
	}
}

void	CTAuctionMainFrame::RefreshList()
{
	CTFrameGroupBase::RequestInfo();
}

void	CTAuctionMainFrame::MoveNextPage()
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	INT nCurPage = pFrame->GetCurPage();
	INT nLastPage = pFrame->GetLastPage();

	if( nCurPage >= nLastPage )
		return ;

	pFrame->SetCurPage( ++nCurPage );

	if( m_pAuctionList->NextPage() )
	{
		m_pCurPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, nCurPage );
	}
	else
	{
		m_pAuctionList->ClearItems();
		pFrame->RequestUpdatePage();
	}
}

void	CTAuctionMainFrame::MovePrevPage()
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	INT nCurPage = pFrame->GetCurPage();
	INT nLastPage = pFrame->GetLastPage();

	if( nCurPage <= 1 )
		return ;

	pFrame->SetCurPage( --nCurPage );

	if( m_pAuctionList->PrevPage() )
	{
		m_pCurPage->m_strText = CTChart::Format( TSTR_FMT_NUMBER, nCurPage );
	}
	else
	{
		m_pAuctionList->ClearItems();
		pFrame->RequestUpdatePage();
	}
}

void	CTAuctionMainFrame::MoveFirstPage()
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	m_pAuctionList->MoveFirstPage();
	m_pAuctionList->ClearItems();

	pFrame->SetCurPage( 1 );
	pFrame->RequestUpdatePage();
}

void	CTAuctionMainFrame::MoveLastPage()
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	INT nCurPage = pFrame->GetCurPage();
	INT nLastPage = pFrame->GetLastPage();

	nCurPage = nLastPage;

	INT nLastPage_List = nLastPage;
	while( nLastPage_List > TAUCTION_MAXPAGE_COUNT )
		nLastPage_List -= TAUCTION_MAXPAGE_COUNT;

	m_pAuctionList->SetLastPage( nLastPage_List );
	m_pAuctionList->MoveLastPage();
	m_pAuctionList->ClearItems();

	pFrame->SetCurPage( nCurPage );
	pFrame->RequestUpdatePage();
}

TEdit*	CTAuctionMainFrame::GetCurEdit()
{
	if( !IsVisible() )
		return NULL;

	size_t size = m_FrameInfoArray.size();
	size_t i = 0;
	for( ; i < size; ++i )
	{
		ITInnerFrame* pFrame = GetInnerFrame( i );

		if( pFrame && pFrame->IsVisible() )
		{
			switch( i )
			{
			case TAUCTION_SEARCH : return static_cast< CTAuctionSearch* >( pFrame )->GetCurEdit(); break;
			case TAUCTION_REGIST : return static_cast< CTAuctionRegist* >( pFrame )->GetCurEdit(); break;
			}
		}
	}

	return NULL;
}

void	CTAuctionMainFrame::SetButton( TButton* pButton, CString strBtn, DWORD dwFuncGM )
{
	if( pButton )
	{
		pButton->m_strText = strBtn;
		pButton->m_menu[TNM_LCLICK] = dwFuncGM;
	}
}

void	CTAuctionMainFrame::SetSearchFrame()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_INSERT_BASKET ), GM_AUCTION_INSERT_BASKET );
	SetButton( m_pBtn[TBUTTON_SECOND], CTChart::LoadString( TSTR_AUCTION_BID ), GM_AUCTION_BID );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_AUCTION_BUY ), GM_AUCTION_BUY );
}

// 내가 등록한 물품리스트 프레임.
void	CTAuctionMainFrame::SetRegistFrame()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( FALSE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( FALSE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], "", 0 );
	SetButton( m_pBtn[TBUTTON_SECOND], "", 0 );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_CANCEL_REG ), GM_AUCTION_CANCEL_REG );
}

// 내가 입찰중인 리스트 프레임.
void	CTAuctionMainFrame::SetMyBidFrame()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( FALSE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_INSERT_BASKET ), GM_AUCTION_INSERT_BASKET );
	SetButton( m_pBtn[TBUTTON_SECOND], CTChart::LoadString( TSTR_AUCTION_BID ), GM_AUCTION_BID );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_AUCTION_BUY ), GM_AUCTION_BUY );
}

// 관심물품 프레임.
void	CTAuctionMainFrame::SetBasketFrame()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_ERASE_BASKET ), GM_AUCTION_ERASE_BASKET );
	SetButton( m_pBtn[TBUTTON_SECOND], CTChart::LoadString( TSTR_AUCTION_BID ), GM_AUCTION_BID );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_AUCTION_BUY ), GM_AUCTION_BUY );
}

void	CTAuctionMainFrame::SetStackItem_Search()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( FALSE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );	// 즉시구매 활성 (구입).

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_INSERT_BASKET ), GM_AUCTION_INSERT_BASKET );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_BUY ), GM_AUCTION_BUY );
}

void	CTAuctionMainFrame::SetCanBidItem_Search()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_INSERT_BASKET ), GM_AUCTION_INSERT_BASKET );
	SetButton( m_pBtn[TBUTTON_SECOND], CTChart::LoadString( TSTR_AUCTION_BID ), GM_AUCTION_BID );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_AUCTION_BUY ), GM_AUCTION_BUY );
}

void	CTAuctionMainFrame::SetStackItem_Basket()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( FALSE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );	// 즉시구매 활성 (구입).

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_ERASE_BASKET ), GM_AUCTION_ERASE_BASKET );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_BUY ), GM_AUCTION_BUY );	
}

void	CTAuctionMainFrame::SetCanBidItem_Basket()
{
	m_pBtn[TBUTTON_FIRST]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_SECOND]->ShowComponent( TRUE );
	m_pBtn[TBUTTON_THIRD]->ShowComponent( TRUE );

	SetButton( m_pBtn[TBUTTON_FIRST], CTChart::LoadString( TSTR_ERASE_BASKET ), GM_AUCTION_ERASE_BASKET );
	SetButton( m_pBtn[TBUTTON_SECOND], CTChart::LoadString( TSTR_AUCTION_BID ), GM_AUCTION_BID );
	SetButton( m_pBtn[TBUTTON_THIRD], CTChart::LoadString( TSTR_AUCTION_BUY ), GM_AUCTION_BUY );
}
//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
CTAuctionMainFrame*		CTAuctionMainFrame::GetInstance()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTAuctionMainFrame* pFrame = NULL;

	if( !pGame )
		return pFrame;

	pFrame = static_cast< CTAuctionMainFrame* >( pGame->GetFrame( TFRAME_AUCTION ) );

	return pFrame;
}

CTAuctionCtrlList* CTAuctionMainFrame::GetList()
{
	return m_pAuctionList;
}

CTCtrlListItem*	CTAuctionMainFrame::GetSelectedItem()
{
	return m_pAuctionList->GetSelectedItem();
}

CTCtrlListItem* CTAuctionMainFrame::GetAuctionItem( size_t index )
{
	return m_pAuctionList->GetItem( index );
}

void	CTAuctionMainFrame::SetCurPage( DWORD dwCurPage )
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	pFrame->SetCurPage( (INT)dwCurPage );
}

void	CTAuctionMainFrame::SetLastPage( DWORD dwLastPage )
{
	CTAuctionInnerFrame* pFrame =
		static_cast< CTAuctionInnerFrame* >( GetInnerFrame( GetSelectedFrame() ) );

	pFrame->SetLastPage( (INT)dwLastPage );
}

void	CTAuctionMainFrame::SetColumnString( BYTE bColumn, CString& strArrowKey )
{
	size_t i = 0;
	for( ; i < TCOLUMN_HEAD_COUNT; ++i )
		m_pHeadBtn[i]->m_strText = m_strHeadOrigText[i];

	m_pHeadBtn[ bColumn ]->m_strText += strArrowKey;
}

void	CTAuctionMainFrame::ResetColumnString()
{
	size_t i = 0;
	for( ; i < TCOLUMN_HEAD_COUNT; ++i )
		m_pHeadBtn[i]->m_strText = m_strHeadOrigText[i];
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
