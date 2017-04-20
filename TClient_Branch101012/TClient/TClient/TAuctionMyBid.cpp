#include "stdafx.h"


CTAuctionMyBid::CTAuctionMyBid( TComponent* pParent, LP_FRAMEDESC pDESC )
:	CTAuctionInnerFrame( pParent, pDESC, TAUCTION_BID )
{
}

//////////////////////////////////////////////////////////////////////////
//	Virtual Function Implementation.
CTAuctionMyBid::~CTAuctionMyBid()
{

}

void	CTAuctionMyBid::RequestInfo()
{
	CTAuctionInnerFrame::RequestInfo();

	CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
	pComm->RequestMyBidList( GetFindInfo() );
}

void	CTAuctionMyBid::RequestUpdatePage()
{
	CTAuctionInnerFrame::RequestUpdatePage();

	CTAuctionCommander::GetInstance()
		->RequestMyBidList( GetFindInfo() );
}

void	CTAuctionMyBid::ResetInfo()
{

}
//	End of Virtual Function.
//////////////////////////////////////////////////////////////////////////

