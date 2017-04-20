#pragma once

//! Basket Class.
class CTAuctionMyBid : public CTAuctionInnerFrame
{
private :	// Member Data.

public :	// VIRTUAL FUNCTION.

							CTAuctionMyBid( TComponent* pParent, LP_FRAMEDESC pDESC );
	virtual					~CTAuctionMyBid();

	virtual void			RequestInfo();
	virtual void			RequestUpdatePage();
	virtual void			ResetInfo();

public :	// Member Function.


public :	// Inline Function.



};