#pragma once

//! Basket Class.
class CTAuctionBasket : public CTAuctionInnerFrame
{
private :	// Member Data.

    TAUCTIONFINDINFO		m_FindInfo;

public :	// VIRTUAL FUNCTION.

							CTAuctionBasket( TComponent* pParent, LP_FRAMEDESC pDESC );
	virtual					~CTAuctionBasket();

	virtual void			RequestInfo();
    virtual void			RequestUpdatePage();	
	virtual void			ResetInfo();

public :	// Member Function.


public :	// Inline Function.



};