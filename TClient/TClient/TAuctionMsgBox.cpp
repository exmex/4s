#include "stdafx.h"
#include "TClientGame.h"


CTAuctionBidOnMsgBox::CTAuctionBidOnMsgBox( TComponent* pParent, LP_FRAMEDESC pDESC )
:	CTClientUIBase( pParent, pDESC ),
	m_ldwCurPrice( 0 ),
	m_ldwBidPrice( 0 )
{
	DWORD dwTextMoneyID[ TMONEY_COUNT ] =
	{
		ID_CTRLINST_CURPRICE_RUNE,
		ID_CTRLINST_CURPRICE_LUNA,
		ID_CTRLINST_CURPRICE_CRON
	};

	DWORD dwEditMoneyID[ TMONEY_COUNT ] =
	{
		ID_CTRLINST_EDIT_RUNE,
		ID_CTRLINST_EDIT_LUNA,
		ID_CTRLINST_EDIT_CRON
	};

	size_t i = 0;
	for( ; i < TMONEY_COUNT; ++i )
	{
		m_pTextMoney[i] = FindKid( dwTextMoneyID[i] );
		m_pEditMoney[i] = static_cast< TEdit* >( FindKid( dwEditMoneyID[i] ) );
	}

	m_pWarningMsg = FindKid( ID_CTRLINST_MSG );

	UpdateByComp();
}

CTAuctionBidOnMsgBox::~CTAuctionBidOnMsgBox()
{

}

//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.
CTAuctionBidOnMsgBox*	CTAuctionBidOnMsgBox::GetInstance()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )	return NULL;

	return static_cast< CTAuctionBidOnMsgBox* >( pGame->GetFrame( TFRAME_BIDON ) );
}

void			CTAuctionBidOnMsgBox::UpdateByMoney()
{
	DWORD dwCurRune = (DWORD) atoi( m_pTextMoney[ TMONEY_RUNE ]->m_strText );
	DWORD dwCurLuna = (DWORD) atoi( m_pTextMoney[ TMONEY_LUNA ]->m_strText );
	DWORD dwCurCron = (DWORD) atoi( m_pTextMoney[ TMONEY_CRON ]->m_strText );

	CTClientGame::AddUpMoney( &m_ldwCurPrice, dwCurRune, dwCurLuna, dwCurCron );

	DWORD dwEditRune = (DWORD) atoi( m_pEditMoney[ TMONEY_RUNE ]->m_strText );
	DWORD dwEditLuna = (DWORD) atoi( m_pEditMoney[ TMONEY_LUNA ]->m_strText );
	DWORD dwEditCron = (DWORD) atoi( m_pEditMoney[ TMONEY_CRON ]->m_strText );

	CTClientGame::AddUpMoney( &m_ldwBidPrice, dwEditRune, dwEditLuna, dwEditCron );
}

void			CTAuctionBidOnMsgBox::UpdateByComp()
{
	DWORD dwCurRune = 0;
	DWORD dwCurLuna = 0;
	DWORD dwCurCron = 0;

	CTClientGame::SplitMoney( m_ldwCurPrice, &dwCurRune, &dwCurLuna, &dwCurCron );

	m_pTextMoney[ TMONEY_RUNE ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwCurRune );
	m_pTextMoney[ TMONEY_LUNA ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwCurLuna );
	m_pTextMoney[ TMONEY_CRON ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwCurCron );

	DWORD dwEditRune = 0;
	DWORD dwEditLuna = 0;
	DWORD dwEditCron = 0;

	CTClientGame::SplitMoney( m_ldwBidPrice, &dwEditRune, &dwEditLuna, &dwEditCron );

	m_pEditMoney[ TMONEY_RUNE ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwEditRune );
	m_pEditMoney[ TMONEY_LUNA ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwEditLuna );
	m_pEditMoney[ TMONEY_CRON ]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwEditCron );
}

TBID_RESULT		CTAuctionBidOnMsgBox::BidOn( INT64 ldwPlayerMoney, FLOAT fIncrement )
{
	UpdateByMoney();

	if( m_ldwBidPrice < (DWORD)(m_ldwCurPrice * fIncrement) )
	{
		return TBID_LOW_MONEY;
	}
	else if( ldwPlayerMoney < m_ldwBidPrice )
	{
		return TBID_NOT_ENOUGH_MONEY;
	}
	else if( m_ldwBuyPrice < m_ldwBidPrice )
	{
		return TBID_OVER_BUYPRICE;
	}
	else if( m_ldwBuyPrice == m_ldwBidPrice )
	{
		return TBID_EQUAL_BUYPRICE;
	}

	return TBID_SUCCESS;
}

TEdit*			CTAuctionBidOnMsgBox::GetCurEdit()
{
	size_t i = 0;
	for( ; i < TMONEY_COUNT; ++i )
	{
		if( m_pEditMoney[i] &&
			m_pEditMoney[i]->CanProcess() &&
			m_pEditMoney[i] == m_pFocus )
		{
			return m_pEditMoney[i];
		}
	}

	return NULL;
}
// END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION IMPLEMENTATION.
void			CTAuctionBidOnMsgBox::SetCurPrice( INT64 ldwCurPrice )
{
	m_ldwCurPrice = ldwCurPrice;
}

void			CTAuctionBidOnMsgBox::SetBidPrice( INT64 ldwBidPrice )
{
	m_ldwBidPrice = ldwBidPrice;
}

void			CTAuctionBidOnMsgBox::SetBuyPrice( INT64 ldwBuyPrice )
{
	m_ldwBuyPrice = ldwBuyPrice;
}

INT64			CTAuctionBidOnMsgBox::GetBidPrice()
{
	return m_ldwBidPrice;
}
// END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////
