#include "stdafx.h"
#include "TAuctionInnerFrame.h"


CTAuctionInnerFrame::CTAuctionInnerFrame( TComponent* pParent, LP_FRAMEDESC pDESC, UINT nFrameID )
:	ITInnerFrame( pParent, pDESC, nFrameID )
{
	m_FindInfo.Clear();
	m_nCurPage = m_nLastPage = 1;
}

CTAuctionInnerFrame::~CTAuctionInnerFrame()
{

}

//////////////////////////////////////////////////////////////////////////
//	VIRTUAL FUNCTION IMPLEMENTATION.
void	CTAuctionInnerFrame::RequestInfo()
{
	m_nCurPage = m_nLastPage = 1;
	m_FindInfo.wPageNum = GetPageNum();
	m_FindInfo.Clear();
}

void	CTAuctionInnerFrame::RequestUpdatePage()
{
	m_FindInfo.wPageNum = GetPageNum();
}

void	CTAuctionInnerFrame::UpdateEditable( CPoint pt )
{

}
//	END OF VIRTUAL FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	INLINE FUNCTION IMPLEMENTATION.
void	CTAuctionInnerFrame::SetCurPage( INT nCurPage )
{
	m_nCurPage = nCurPage;
}

void	CTAuctionInnerFrame::SetLastPage( INT nLastPage )
{
	m_nLastPage = nLastPage;
}

void	CTAuctionInnerFrame::SetAlignKind( BYTE bAlignKind )
{
	m_FindInfo.bAlignKind = bAlignKind;
}

WORD	CTAuctionInnerFrame::GetPageNum()	
{
	DWORD value = (DWORD)((m_nCurPage - 1) / TAUCTION_MAXPAGE_COUNT);

	WORD wPageNum = (WORD)(value * TAUCTION_MAXPAGE_COUNT);

	return wPageNum + 1;
}

INT		CTAuctionInnerFrame::GetCurPage()
{
	return m_nCurPage;
}

INT		CTAuctionInnerFrame::GetLastPage()
{
	return m_nLastPage;
}

LPTAUCTIONFINDINFO	CTAuctionInnerFrame::GetFindInfo()
{
	return &m_FindInfo;
}
//	END OF INLINE FUNCTION.
//////////////////////////////////////////////////////////////////////////