#pragma once


//!	Auction InnerFrame Class.
class CTAuctionInnerFrame : public ITInnerFrame
{
private :	// Member Data.

	TAUCTIONFINDINFO		m_FindInfo;
	INT						m_nCurPage;
	INT						m_nLastPage;

public :	// Virtual Function.

	CTAuctionInnerFrame( TComponent* pParent, LP_FRAMEDESC pDESC, UINT nFrameID );
	virtual ~CTAuctionInnerFrame();

	virtual void			RequestInfo();
	/*! 현재 페이지를 유지하면서 아이템리스트를 업데이트요청. */
	virtual void			RequestUpdatePage();
	/*! 에디트 컴포넌트 포커스관리. */
	virtual void			UpdateEditable( CPoint pt );

public :	// Member Function.

public :	// Inline Function.

	void					SetCurPage( INT nCurPage );
	void					SetLastPage( INT nLastPage );
	void					SetAlignKind( BYTE bAlignKind );
	WORD					GetPageNum();
	INT						GetCurPage();
	INT						GetLastPage();
	LPTAUCTIONFINDINFO		GetFindInfo();
};

