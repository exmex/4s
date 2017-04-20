// BitmapSlider.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// CBitmapSlider v1.5
//
// 어디든 무료입니다 - 2003년 9월 16일 - 류준호
//
/////////////////////////////////////////////////////////////////////////////


CBitmapSlider::CBitmapSlider()
{
	m_nPos = m_nMin = 0;
	m_nMax = 100;
	m_nPage = 20;

	m_nMarginLeft = m_nMarginRight = m_nMarginTop = m_nMarginBottom = 0;
	m_nThumbWidth = m_nThumbHeight = 0;

	m_bChannel = m_bVertical = m_bThumb = m_bLButtonDown = FALSE;

	m_bFocusRect = m_bFocus = FALSE;
	m_bDrawFocusRect = TRUE;

	m_bEnable = TRUE;

	m_nThumbBgX = m_nThumbBgY = -1;
}

CBitmapSlider::~CBitmapSlider()
{
}


BEGIN_MESSAGE_MAP(CBitmapSlider, CStatic)
	//{{AFX_MSG_MAP(CBitmapSlider)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapSlider message handlers

BOOL CBitmapSlider::OnEraseBkgnd(CDC* pDC) 
{	
	// Do not erase background for the transparency effect
	return TRUE;
}

// 컨트롤을 그린다
//
void CBitmapSlider::OnPaint() 
{
	CPaintDC dcOrigin(this);

	// "Flicker Free Drawing In MFC" by Keith Rule
	CMemDC dc( &dcOrigin, &m_rect, m_bTransparentChannel );

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dc );

	CBitmap *pbmTmp;

	// 포커스를 얻은 상태임을 알리는 사각형을 그린다.
	if( m_bFocusRect && ( m_bTransparentChannel || !m_bChannel ) ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = FALSE;
	}

	// 배경을 그린다
	if( m_bChannel ) {

		pbmTmp = dcMem.SelectObject( &m_bmChannel );

		// 활성화된 배경을 그릴 필요가 있을 때
		if( m_bChannelActive && m_bEnable ) {

			// 수직
			if( m_bVertical ) {

				// 아래쪽
				DrawBitmap( &dc, 0, Pos2Pixel(m_nPos),
					m_nWidth, m_nHeight - Pos2Pixel(m_nPos),
					&dcMem, 0, Pos2Pixel(m_nPos),
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// 위쪽
				DrawBitmap( &dc, 0, 0, m_nWidth, Pos2Pixel(m_nPos),
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

			// 수평
			} else {

				// 오른쪽
				DrawBitmap( &dc, Pos2Pixel(m_nPos), 0,
					m_nWidth - Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, Pos2Pixel(m_nPos), 0, 
					&m_bmChannelActiveMask, m_bTransparentChannel );

				dcMem.SelectObject( &m_bmChannelActive );

				// 왼쪽
				DrawBitmap( &dc, 0, 0, Pos2Pixel(m_nPos), m_nHeight,
					&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );
			}

		// 활성화된 배경의 비트맵이 없을 때
		} else {

			DrawBitmap( &dc, 0, 0, m_nWidth, m_nHeight,
				&dcMem, 0, 0, &m_bmChannelMask, m_bTransparentChannel );

		}

		dcMem.SelectObject( pbmTmp );
	}

	// 손잡이가 있던 자리를 복구할 비트맵이 있을 때
	if( m_nThumbBgX != -1 ) {

		RestoreBackground(
			&dc, m_nThumbBgX, m_nThumbBgY,
			m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );

		m_nThumbBgX = -1;
	}

	// 손잡이를 그린다
	if( m_bThumb && m_bEnable ) {

		if( m_bThumbActive && m_bLButtonDown )
			pbmTmp = dcMem.SelectObject( &m_bmThumbActive ); // 활성화
		else
			pbmTmp = dcMem.SelectObject( &m_bmThumb ); // 보통

		// 수직
		if( m_bVertical ) {

			// 손잡이가 옮겨간 후 배경을 복구할 필요가 있을 때
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = m_nMarginLeft;
				m_nThumbBgY = Pos2Pixel(m_nPos) - m_nThumbHeight/2;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, m_nMarginLeft, Pos2Pixel(m_nPos) - m_nThumbHeight/2,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		// 수평
		} else {

			// 손잡이가 옮겨간 후 배경을 복구할 필요가 있을 때
			if( m_bTransparentChannel || !m_bChannel ) {

				m_nThumbBgX = Pos2Pixel(m_nPos) - m_nThumbWidth/2;
				m_nThumbBgY = m_nMarginTop;

				CopyBackground(
					&dc, m_nThumbBgX, m_nThumbBgY,
					m_nThumbWidth, m_nThumbHeight, &m_bmThumbBg );
			}

			DrawBitmap(
				&dc, Pos2Pixel(m_nPos) - m_nThumbWidth/2, m_nMarginTop,
				m_nThumbWidth, m_nThumbHeight,
				&dcMem, 0, 0, &m_bmThumbMask, m_bTransparentThumb );

		} // if 수평

		dcMem.SelectObject( pbmTmp );

	} // if 손잡이 그리기

	// 포커스 사각형을 그린다
	if( m_bDrawFocusRect && m_bFocus && m_bEnable ) {

		dc.DrawFocusRect( m_rect );
		m_bFocusRect = TRUE;
	}

	dcMem.DeleteDC();
}

// 슬라이더의 최대값을 설정합니다.
//
// 매개변수:
//		[IN]	nMax
//				최대값.
//		[IN]	bRedraw
//				TRUE이면 컨트롤을 다시 그리고
//				FALSE이면 최대값만 변경합니다.
//
void CBitmapSlider::SetRangeMax(int nMax, BOOL bRedraw)
{
	m_nMax = nMax;
	if( bRedraw )
		Invalidate();
}

// 슬라이더의 최소값을 설정합니다.
//
// 매개변수:
//		[IN]	nMin
//				최소값.
//		[IN]	bRedraw
//				TRUE이면 컨트롤을 다시 그리고
//				FALSE이면 최대값만 변경합니다.
//
void CBitmapSlider::SetRangeMin(int nMin, BOOL bRedraw)
{
	m_nMin = nMin;
	if( bRedraw )
		Invalidate();
}

// 슬라이더의 범위를 설정합니다.
//
// 매개변수:
//		[IN]	nMin
//				최소값.
//		[IN]	nMax
//				최대값.
//		[IN]	bRedraw
//				TRUE이면 컨트롤을 다시 그리고
//				FALSE이면 최대값만 변경합니다.
//
void CBitmapSlider::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	SetRangeMin( nMin, FALSE );
	SetRangeMax( nMax, bRedraw );
}

// 슬라이더의 위치 값을 설정합니다.
//
// 매개변수:
//		[IN]	nPos
//				새로운 위치 값.
//
void CBitmapSlider::SetPos(int nPos)
{
	m_nPos = nPos;

	// 범위를 검사한다.
	if( m_nPos > m_nMax )
		m_nPos = m_nMax;
	if( m_nPos < m_nMin )
		m_nPos = m_nMin;

	Invalidate();
}

// 슬라이더의 페이지 값을 설정합니다.
//
// 매개변수:
//		[IN]	nSize
//				새로운 페이지 값..
//
// 리턴값:
//		이전의 페이지 값.
//
int CBitmapSlider::SetPageSize(int nSize)
{
	int nRet = m_nPage;

	m_nPage = nSize;

	return nRet;
}

// 컨트롤의 여백을 설정합니다. 손잡이는 여백 안쪽에서 움직입니다.
//
void CBitmapSlider::SetMargin(int nLeft, int nTop, int nRight, int nBottom )
{
	SetMarginLeft( nLeft );
	SetMarginTop( nTop );
	SetMarginRight( nRight );
	SetMarginBottom( nBottom );
}

// 슬라이더의 동작을 억제할 수 있습니다.
//
//		[IN]	bEnable
//				TRUE는 슬라이더를 동작하게 하고,
//				FALSE는 동작을 억제합니다.
//
void CBitmapSlider::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;

	// 드래그하는 중일 경우
	if( !m_bEnable && m_bLButtonDown ) {

		m_bLButtonDown = FALSE;
		ReleaseCapture();
	}

	Invalidate();
}

// 컨트롤이 포커스를 얻었을 때, 이를 표시할 것인지 여부를 설정합니다.
//
//		[IN]	bDraw
//				TRUE는 표시를,
//				FALSE는 표시하지 않음을 의미합니다.
//
//		[IN]	bRedraw
//				TRUE값은 컨트롤을 다시 그리고,
//				FALSE는 상태값만을 바꿉니다.
//
void CBitmapSlider::DrawFocusRect(BOOL bDraw, BOOL bRedraw)
{
	m_bDrawFocusRect = bDraw;

	if( bRedraw )
		Invalidate();
}

// 배경에 사용할 비트맵을 불러옵니다.
//
// 매개변수:
//		[IN]	nChannelID
//				배경 비트맵의 ID 값.
//		[IN]	nActiveID
//				활성화된 배경 비트맵의 ID 값.
//		[IN]	bTransparent
//				투명 효과를 적용하려면 TRUE, 그렇지 않으면 FALSE입니다.
//		[IN]	clrpTransColor
//				투명 효과를 적용할 색상의 RGB 값.
//		[IN]	iTransPixelX
//				투명한 효과를 줄 색상이 위치한 포인트의 x좌표 값.
//		[IN]	iTransPixelY
//				투명한 효과를 줄 색상이 위치한 포인트의 y좌표 값.
//
// 리턴값:
//		성공했으면 TRUE 를 그렇지 않으면FALSE를 리턴합니다
//
BOOL CBitmapSlider::SetBitmapChannel(
	UINT nChannelID, UINT nActiveID , BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// 비트맵의 ID가 정해지지 않았을 때
	if( !nChannelID ) {

		m_bChannel = FALSE;

		m_bmChannel.DeleteObject();
		m_bmChannelMask.DeleteObject();
		m_bmChannelActive.DeleteObject();
		m_bmChannelActiveMask.DeleteObject();
	
		return TRUE;
	}

	// 비트맵을 불러온다
	m_bmChannel.DeleteObject();

	if( !m_bmChannel.LoadBitmap( nChannelID ) )
		return FALSE;

	// 투명한 비트맵을 그리기 위해 마스크를 만든다.
	if( bTransparent ) {

		PrepareMask( &m_bmChannel, &m_bmChannelMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// 활성화 배경 비트맵을 불러온다.
	if( nActiveID ) {

		m_bmChannelActive.DeleteObject();

		if( !m_bmChannelActive.LoadBitmap( nActiveID ) ) {

			m_bmChannel.DeleteObject();
			if( bTransparent )
				m_bmChannelMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmChannelActive, &m_bmChannelActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bChannelActive = TRUE;

	// 활성화 상태의 비트맵이 존재하지 않을 때.
	} else
		m_bChannelActive = FALSE;

	// 비트맵의 크기 정보를 가져온다.
	BITMAP	bitmap;
	m_bmChannel.GetBitmap( &bitmap );

	m_nWidth = bitmap.bmWidth;
	m_nHeight = bitmap.bmHeight;

	// 크기를 비교한다
	if( m_bChannelActive ) {

		BITMAP	bitmap;
		m_bmChannelActive.GetBitmap( &bitmap );

		ASSERT( m_nWidth == bitmap.bmWidth && m_nHeight == bitmap.bmHeight );
	}

	// 컨트롤의 크기를 비트맵의 크기로 바꾼다.
	SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, SWP_NOZORDER | SWP_NOMOVE);

	GetClientRect( &m_rect );

	m_bTransparentChannel = bTransparent;
	m_bChannel = TRUE;

	return TRUE;
}

// 손잡이에 사용할 비트맵을 불러옵니다.
//
// 매개변수:
//		[IN]	nThumbID
//				손잡이 비트맵의 ID.
//		[IN]	nActiveID
//				활성화된 손잡이 비트맵의 ID.
//		[IN]	bTransparent
//				투명 효과를 적용하려면 TRUE, 그렇지 않으면 FALSE입니다.
//		[IN]	clrpTransColor
//				투명 효과를 적용할 색상의 RGB 값.
//		[IN]	iTransPixelX
//				투명한 효과를 줄 색상이 위치한 포인트의 x좌표 값.
//		[IN]	iTransPixelY
//				투명한 효과를 줄 색상이 위치한 포인트의 y좌표 값.
//
// 리턴값:
//		성공했으면 TRUE 를 그렇지 않으면FALSE를 리턴합니다
//
BOOL CBitmapSlider::SetBitmapThumb(
	UINT nThumbID, UINT nActiveID, BOOL bTransparent,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY )
{
	// 손잡이의 ID값이 NULL일때
	if( !nThumbID ) {

		m_bThumb = FALSE;

		m_bmThumb.DeleteObject();
		m_bmThumbMask.DeleteObject();
		m_bmThumbActive.DeleteObject();
		m_bmThumbActiveMask.DeleteObject();
		m_bmThumbBg.DeleteObject();

		return TRUE;
	}

	// 비트맵을 불러온다
	m_bmThumb.DeleteObject();

	if( !m_bmThumb.LoadBitmap( nThumbID ) )
		return FALSE;

	// 투명한 비트맵을 그리기 위해 마스크를 만든다.
	if( bTransparent ) {

		PrepareMask( &m_bmThumb, &m_bmThumbMask,
			clrpTransColor, iTransPixelX, iTransPixelY );
	}

	// 활성화 상태를 위한 비트맵을 불러온다.
	if( nActiveID ) {

		m_bmThumbActive.DeleteObject();

		if( !m_bmThumbActive.LoadBitmap( nActiveID ) ) {

			m_bmThumb.DeleteObject();
			if( bTransparent )
				m_bmThumbMask.DeleteObject();

			return FALSE;
		}

		if( bTransparent ) {

			PrepareMask( &m_bmThumbActive, &m_bmThumbActiveMask,
				clrpTransColor, iTransPixelX, iTransPixelY );
		}
		
		m_bThumbActive = TRUE;

	// 활성화 상태의 비트맵이 존재하지 않을 때.
	} else
		m_bThumbActive = FALSE;

	// 비트맵의 크기 정보를 가져온다.
	BITMAP	bitmap;
	m_bmThumb.GetBitmap( &bitmap );

	m_nThumbWidth = bitmap.bmWidth;
	m_nThumbHeight = bitmap.bmHeight;

	// 배경을 위한 비트맵이 없으면 컨트롤의 사이즈를 기억한다.
	if( !m_bChannel ) {

		GetClientRect( &m_rect );
		m_nHeight = m_rect.Height();
		m_nWidth = m_rect.Width();
	}

	ASSERT( m_nThumbWidth <= m_nWidth && m_nThumbHeight <= m_nHeight );

	// 크기를 비교한다
	if( m_bThumbActive ) {

		BITMAP	bitmap;
		m_bmThumbActive.GetBitmap( &bitmap );

		ASSERT(
			m_nThumbWidth == bitmap.bmWidth &&
			m_nThumbHeight == bitmap.bmHeight );
	}

	// 속성값을 설정한다
	m_bTransparentThumb = bTransparent;
	m_bThumb = TRUE;

	return TRUE;
}

// OnLButtonDown
//
// 드래그가 시작될 때
//
void CBitmapSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	SetCapture();
	SetFocus();

	m_bLButtonDown = TRUE;

	// 손잡이를 마우스로 클릭했을 때
	// 마우스의 좌표와 손잡이 중심의 좌표를 비교하여
	// 둘 사이의 거리를 구해둔다
	if( m_bVertical ) {

		if( abs( point.y - Pos2Pixel( m_nPos ) ) <= m_nThumbHeight / 2 )
			m_nMouseOffset = point.y - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;

	} else {

		if( abs( point.x - Pos2Pixel( m_nPos ) ) <= m_nThumbWidth / 2 )
			m_nMouseOffset = point.x - Pos2Pixel( m_nPos );
		else
			m_nMouseOffset = 0;
	}

	OnMouseMove( nFlags, point );
	Invalidate();

	CStatic::OnLButtonDown(nFlags, point);
}

// OnMouseMove
//
// 드래그 중일 때
//
void CBitmapSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( !m_bLButtonDown || !m_bEnable )
		return;

	int nPixel;

	// 범위를 벗어났는지 검사한다
	if( m_bVertical ) {

		nPixel = point.y - m_nMouseOffset;

		if( nPixel > m_nHeight - m_nMarginBottom - m_nThumbHeight/2 )
			nPixel = m_nHeight - m_nMarginBottom - m_nThumbHeight/2;

		if( nPixel < m_nMarginTop + m_nThumbHeight/2 )
			nPixel = m_nMarginTop + m_nThumbHeight/2;

	} else {

		nPixel = point.x - m_nMouseOffset;

		if( nPixel < m_nMarginLeft + m_nThumbWidth/2 )
			nPixel = m_nMarginLeft + m_nThumbWidth/2;

		if( nPixel > m_nWidth - m_nMarginRight - m_nThumbWidth/2 )
			nPixel = m_nWidth - m_nMarginRight - m_nThumbWidth/2;
	}

	// 변한 내용을 적용한다
	if( Pos2Pixel(m_nPos) != nPixel ) {

		SetPos( Pixel2Pos( nPixel ) );

		::PostMessage(
			GetParent()->GetSafeHwnd(), WM_BITMAPSLIDER_MOVING,
			GetDlgCtrlID(), m_nPos );
	}

	CStatic::OnMouseMove(nFlags, point);
}

// OnLButtonUp
//
// 드래그가 끝났을 때
//
void CBitmapSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( !m_bEnable )
		return;

	ReleaseCapture();
	m_bLButtonDown = FALSE;

	Invalidate();

	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnLButtonUp(nFlags, point);
}

// 컨트롤의 위치값에 해당하는 손잡이 중심의 좌표값을 구한다
//
int CBitmapSlider::Pos2Pixel(int nPos)
{
	if( m_bVertical ) {

		return
			m_nMarginTop + m_nThumbHeight/2 +
			(int)(
			( m_nHeight - m_nMarginTop - m_nMarginBottom - m_nThumbHeight ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			);

	} else {

		return (int)(
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			((double) ( nPos - m_nMin ) / ( m_nMax - m_nMin ) )
			) + m_nMarginLeft + m_nThumbWidth/2;
	}
}

// 마우스의 좌표값에 해당하는 컨트롤의 위치값을 구한다
//
int CBitmapSlider::Pixel2Pos(int nPixel)
{
	if( m_bVertical ) {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginTop - m_nThumbHeight/2) /
			( m_nHeight - m_nMarginBottom - m_nMarginTop - m_nThumbHeight ) *
			( m_nMax - m_nMin )
			);

	} else {

		return (int)(
			m_nMin +
			(double)( nPixel - m_nMarginLeft - m_nThumbWidth/2 ) /
			( m_nWidth - m_nMarginLeft - m_nMarginRight - m_nThumbWidth ) *
			( m_nMax - m_nMin )
			);
	}
}

// 지정한 영역의 비트맵 정보를 가져온다
//
void CBitmapSlider::CopyBackground(
	CDC *pDC, int nXSrc, int nYSrc, int nWidth, int nHeight, CBitmap *pBmDst)
{
	pBmDst->DeleteObject();
	pBmDst->CreateCompatibleBitmap( pDC, nWidth, nHeight );

	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmDst );

	memDC.BitBlt( 0, 0, nWidth, nHeight, pDC, nXSrc, nYSrc, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

// 비트맵을 지정한 영역에 그린다
//
void CBitmapSlider::RestoreBackground(
	CDC *pDC, int nXDst, int nYDst, int nWidth, int nHeight, CBitmap *pBmSrc)
{
	CDC memDC;

	memDC.CreateCompatibleDC( pDC );
	CBitmap *pBmTmp = memDC.SelectObject( pBmSrc );

	pDC->BitBlt( nXDst, nYDst, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( pBmTmp );
	memDC.DeleteDC();
}

// DrawBitmap
//
// OnPaint의 코드를 읽기 쉽게 하기위한 함수
//
void CBitmapSlider::DrawBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask, BOOL bTransparent )
{
	if( bTransparent ) {

		DrawTransparentBitmap(
			pDC, xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, bmMask );

	} else {

		pDC->BitBlt( xStart, yStart,
			wWidth, wHeight,
			pTmpDC, xSource, ySource, SRCCOPY );
	}
}

// PrepareMask
//
// "Drawing Transparent Bitmap with ease with on the fly masks in MFC"
// By Raja Segar
//
// clrpTransColor의 기본값을 NULL(black)에서
// 0xFF000000(RGB 값이 아닌 값)으로 바꿨습니다.
//
void CBitmapSlider::PrepareMask(
	CBitmap *pBmpSource, CBitmap *pBmpMask,
	COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY)
{
	BITMAP bm;

	// Get the dimensions of the source bitmap
	pBmpSource->GetObject(sizeof(BITMAP), &bm);

	// Create the mask bitmap
	pBmpMask->DeleteObject();
	pBmpMask->CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// We will need two DCs to work with. One to hold the Image
	// (the source), and one to hold the mask (destination).
	// When blitting onto a monochrome bitmap from a color, pixels
	// in the source color bitmap that are equal to the background
	// color are blitted as white. All the remaining pixels are
	// blitted as black.

	CDC hdcSrc, hdcDst;

	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);

	// Load the bitmaps into memory DC
	CBitmap* hbmSrcT = (CBitmap*) hdcSrc.SelectObject(pBmpSource);
	CBitmap* hbmDstT = (CBitmap*) hdcDst.SelectObject(pBmpMask);

	// Dynamically get the transparent color
	COLORREF clrTrans;
	if (clrpTransColor == 0xFF000000)
	{
		// User did not specify trans color so get it from bmp
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}


	// Change the background to trans color
	COLORREF clrSaveBk  = hdcSrc.SetBkColor(clrTrans);

	// This call sets up the mask bitmap.
	hdcDst.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcSrc,0,0,SRCCOPY);

	// Now, we need to paint onto the original image, making
	// sure that the "transparent" area is set to black. What
	// we do is AND the monochrome image onto the color Image
	// first. When blitting from mono to color, the monochrome
	// pixel is first transformed as follows:
	// if  1 (black) it is mapped to the color set by SetTextColor().
	// if  0 (white) is is mapped to the color set by SetBkColor().
	// Only then is the raster operation performed.

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(RGB(255,255,255));
	hdcSrc.SetBkColor(RGB(0,0,0));

	hdcSrc.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &hdcDst,0,0,SRCAND);

	// Clean up by deselecting any objects, and delete the
	// DC's.
	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();
}

// DrawTransparentBitmap
//
// "Drawing Transparent Bitmap with ease with on the fly masks in MFC"
// By Raja Segar
//
void CBitmapSlider::DrawTransparentBitmap(
	CDC *pDC, int xStart, int yStart, int wWidth, int wHeight,
	CDC *pTmpDC, int xSource, int ySource, CBitmap *bmMask )
{
	// We are going to paint the two DDB's in sequence to the destination.
	// 1st the monochrome bitmap will be blitted using an AND operation to
	// cut a hole in the destination. The color image will then be ORed
	// with the destination, filling it into the hole, but leaving the
	// surrounding area untouched.

	CDC hdcMem;
	hdcMem.CreateCompatibleDC(NULL);

	CBitmap* hbmT = hdcMem.SelectObject(bmMask);

	pDC->BitBlt( xStart, yStart, wWidth, wHeight, &hdcMem,
		xSource, ySource, SRCAND);

	// Also note the use of SRCPAINT rather than SRCCOPY.

	pDC->BitBlt(xStart, yStart, wWidth, wHeight, pTmpDC,
		xSource, ySource,SRCPAINT);

	// Now, clean up.
	hdcMem.SelectObject(hbmT);
	hdcMem.DeleteDC();
}

// 키보드 입력을 받기 위해
//
UINT CBitmapSlider::OnGetDlgCode() 
{
	if( GetKeyState(VK_TAB) >= 0 ) {

		return  DLGC_WANTALLKEYS;
	}
	
	return CStatic::OnGetDlgCode();
}

// 키보드의 입력을 처리한다
//
void CBitmapSlider::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( !m_bEnable )
		return;

	switch( nChar ) {

	// 왼쪽, 위로
	case VK_LEFT :
	case VK_UP :

		SetPos( m_nPos-1 );
		break;

	// 오른쪽, 아래로
	case VK_RIGHT :
	case VK_DOWN :

		SetPos( m_nPos+1 );
		break;

	// Home
	case VK_HOME :

		SetPos( m_nMin );
		break;

	// End
	case VK_END :

		SetPos( m_nMax );
		break;

	// Page up
	case VK_PRIOR :

		SetPos( m_nPos - m_nPage );
		break;

	// Page down
	case VK_NEXT :

		SetPos( m_nPos + m_nPage );
		break;

	default :

		CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	
	::PostMessage(
		GetParent()->GetSafeHwnd(),	WM_BITMAPSLIDER_MOVED,
		GetDlgCtrlID(), m_nPos );

	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

// 컨트롤이 포커스를 잃었을 때
//
void CBitmapSlider::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);

	m_bFocus = FALSE;
	Invalidate();
}

// 컨트롤이 포커스를 얻었을 때
//
void CBitmapSlider::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);

	m_bFocus = TRUE;
	Invalidate();
}

// 비트맵을 해제한다
//
void CBitmapSlider::OnDestroy() 
{
	CStatic::OnDestroy();
	
	m_bmThumb.DeleteObject();
	m_bmThumbMask.DeleteObject();

	m_bmThumbActive.DeleteObject();
	m_bmThumbActiveMask.DeleteObject();

	m_bmThumbBg.DeleteObject();

	m_bmChannel.DeleteObject();
	m_bmChannelMask.DeleteObject();

	m_bmChannelActive.DeleteObject();
	m_bmChannelActiveMask.DeleteObject();
}
