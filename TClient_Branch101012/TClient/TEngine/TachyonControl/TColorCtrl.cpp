#include "stdafx.h"
#include "TColorCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define __SEL_BOX_DX				6
#define __SEL_BOX_DY				6

#define NUM_LEVELS					7
#define TAN30						0.57735026918962f
#define OFFSETY						(1.5f * TAN30)
#define PI							3.14159265358979f
#define NUM_HEX_FULL_CELLS			144
#define NUM_HEX_GRAY_CELLS			17
#define HEX_DIST_GAP				10
#define NUM_RGB_SINGLE_CELLS		256
#define NUM_RGB_FULL_CELLS			(NUM_RGB_SINGLE_CELLS * 3)

#define DEFAULT_RGB_CUBE_GAP		5
#define DEF_RGB_CUBE_HL				0
#define DEF_RGB_CUBE_TRANSPARENT	RGB( 255, 255, 255)

#define DEFAULT_RGB_CUBE_GAP		5
#define DEF_RGB_CUBE_HL				0
#define DEF_RGB_CUBE_TRANSPARENT	RGB( 255, 255, 255)

#define DEFAULT_LUMINANCE			0.5f
#define DEFAULT_LUM_BAR_HEIGHT		20
#define DEFAULT_HSL_AREA_GAP		4
#define DEF_HSL_TRANSPARENT			RGB( 255, 255, 255)

UINT CTColorCtrl::m_nMsgColorChanged = ::RegisterWindowMessage(_T("CTColorCtrl::m_nMsgColorChanged"));


static const float fOffsetX[] = { -0.5f, -1.0f, -0.5f, 0.5f, 1.0f, 0.5f};
static const float fOffsetY[] = { OFFSETY, 0.0f, -OFFSETY, -OFFSETY, 0.0, OFFSETY};

static void _CreateCompatibleBitmapImpl( CBitmap &bmp,
										 CDC &dc,
										 int dX,
										 int dY)
{
	bmp.CreateCompatibleBitmap( &dc, dX, dY);
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::any_picker_data

CTColorCtrl::any_picker_data::any_picker_data() : m_nMoveMode(_MM_NONE),
	m_szTotal( 50, 50),
	m_clr(RGB( 255, 255, 255)),
	m_clrOld(RGB( 255, 255, 255))
{
	CTPaintManager::RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrSaturation,
		&m_fCurrLuminance);
}

CTColorCtrl::any_picker_data::~any_picker_data()
{
}

void CTColorCtrl::any_picker_data::OnInit( CDC &dc,
										   CPalette *pPalette,
										   CSize szTotal,
										   BOOL bOnlySizeChanged)
{
	m_szTotal = szTotal;

	if( m_szTotal.cx <= 0 )
		m_szTotal.cx = 1;

	if( m_szTotal.cy <= 0 )
		m_szTotal.cy = 1;
}

void CTColorCtrl::any_picker_data::OnSetOldRGB( COLORREF clr)
{
	m_clrOld = clr;
}

void CTColorCtrl::any_picker_data::OnSetRGB( COLORREF clr)
{
	m_clr = clr;

	CTPaintManager::RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrSaturation,
		&m_fCurrLuminance);
}

void CTColorCtrl::any_picker_data::OnSetHLS( double hue,
											 double luminance,
											 double saturation)
{
	m_fCurrSaturation = saturation;
	m_fCurrLuminance = luminance;
	m_fCurrHue = hue;

	m_clr = CTPaintManager::HLStoRGB(
		m_fCurrHue,
		m_fCurrSaturation,
		m_fCurrLuminance);
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::cell_based_data

static int GetAngleFromPoint( int nX, int nY)
{
	double dAngle = atan2( double(nY), double(nX));
	return INT(dAngle * 180.0 / PI);
}

void CTColorCtrl::cell_based_data::OnInit( CDC &dc,
										   CPalette *pPalette,
										   CSize szTotal,
										   BOOL bOnlySizeChanged)
{
	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	Generate(bOnlySizeChanged);
}

void CTColorCtrl::cell_based_data::OnDone()
{
	ReleaseAll();
}

void CTColorCtrl::cell_based_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	int nFocus0 = m_vCells.GetSize();
	int nFocus1 = m_vCells.GetSize();

	for( int i=0; i<m_vCells.GetSize(); i++)
	{
		cell& _cell = m_vCells[i];

		_cell.OnDraw( dc, pPalette, FALSE);
		if( _cell.m_clr != m_clr )
			continue;

		if( nFocus0 == m_vCells.GetSize() )
		{
			nFocus0 = i;
			continue;
		}

		if( nFocus1 == m_vCells.GetSize() )
			nFocus1 = i;
	}

	if( nFocus0 != m_vCells.GetSize() )
		m_vCells[nFocus0].OnDraw( dc, pPalette, TRUE);

	if( nFocus1 != m_vCells.GetSize() )
		m_vCells[nFocus1].OnDraw( dc, pPalette, TRUE);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::cell_based_data::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_CELL_SCAN;
	OnMouseMove( nFlags|MK_LBUTTON, point);
}

void CTColorCtrl::cell_based_data::OnLButtonUp( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::cell_based_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_nMoveMode != _MM_CELL_SCAN )
		return;

	for( int i=0; i<m_vCells.GetSize(); i++)
	{
		cell& _cell = m_vCells[i];

		if(_cell.HitTest(point))
		{
			OnSetRGB(_cell.m_clr);
			return;
		}
	}
}

void CTColorCtrl::cell_based_data::Generate( BOOL bOnlySizeChanged)
{
	int nCellSizeByX = m_szTotal.cx / (2 * (NUM_LEVELS + 1)) + 2;
	int nCellSizeByY = (m_szTotal.cy - HEX_DIST_GAP) / (2 * (NUM_LEVELS + 1)) + 2;
	int nCellSize = min( nCellSizeByX, nCellSizeByY);
	int x = m_szTotal.cx / 2;
	int y =	(m_szTotal.cy - nCellSize * 2 - HEX_DIST_GAP) / 2;

	m_vCells.SetSize(NUM_HEX_FULL_CELLS);
	if(!bOnlySizeChanged)
		m_vCells[0] = RGB( 255, 255, 255);

	m_vCells[0].m_bAtFirstType = TRUE;
	m_vCells[0].AdjustCellPosition( x, y, nCellSize);

	int nNumber = 1;
	for( int nLevel = 1; nLevel<NUM_LEVELS; nLevel++)
	{
		int nPosX = x + (nCellSize * nLevel);
		int nPosY = y;

		for( int nSide = 0; nSide<NUM_LEVELS - 1; nSide++)
		{
			int nDx = INT( FLOAT(nCellSize) * fOffsetX[nSide]);
			int nDy = INT( FLOAT(nCellSize) * fOffsetY[nSide]);

			for( int nCell = 0; nCell<nLevel; nCell++)
			{
				int nAngle = GetAngleFromPoint(
					nPosX - x,
					nPosY - y);

				double L = 1.0f * (NUM_LEVELS - nLevel) / NUM_LEVELS + 0.1f;
				if(!bOnlySizeChanged)
				{
					m_vCells[nNumber].SetCellColor(
						CTPaintManager::GetRGBFromHLSExtend(
						(FLOAT) nAngle,
						L, 1.0F));
				}

				m_vCells[nNumber].m_bAtFirstType = TRUE;
				m_vCells[nNumber].AdjustCellPosition(
					nPosX,
					nPosY,
					nCellSize);

				nNumber++;
				nPosX += nDx;
				nPosY += nDy;
			}
		}
	}

	int nGrayPosX = (m_szTotal.cx - ( nCellSize * 3 + nCellSize / 2 + ( nCellSize * (NUM_HEX_GRAY_CELLS - 2) ) / 2)) / 2;
	int nGrayCenterY = m_szTotal.cy - nCellSize * 1 - HEX_DIST_GAP / 2;
	int nTmp = INT( FLOAT(nCellSize) / 2.3f );
	int nUpperY = nGrayCenterY - nTmp;
	int nLowerY = nGrayCenterY + nTmp;
	int nGrayColorPos = 0;

	for( int nCell = 0; nCell < NUM_HEX_GRAY_CELLS; nCell++, nNumber++, nGrayColorPos += 255 / NUM_HEX_GRAY_CELLS)
	{
		if(!nCell)
		{
			if(!bOnlySizeChanged)
				m_vCells[nNumber].m_clr = RGB( 255, 255, 255);

			m_vCells[nNumber].m_bAtFirstType = TRUE;
			m_vCells[nNumber].AdjustCellPosition(
				nGrayPosX,
				nGrayCenterY,
				nCellSize * 2);

			nGrayPosX += nCellSize * 2;
			continue;
		}

		if( nCell == NUM_HEX_GRAY_CELLS - 1 )
		{
			nGrayPosX += (nCellSize * 3) / 2 - 1;
			if(!bOnlySizeChanged)
				m_vCells[nNumber].m_clr = RGB( 0, 0, 0);

			m_vCells[nNumber].m_bAtFirstType = TRUE;
			m_vCells[nNumber].AdjustCellPosition(
				nGrayPosX,
				nGrayCenterY,
				nCellSize * 2);

			break;
		}

		COLORREF clrGray = RGB(
			255 - nGrayColorPos,
			255 - nGrayColorPos,
			255 - nGrayColorPos);

		if(!bOnlySizeChanged)
			m_vCells[nNumber].m_clr = clrGray;

		m_vCells[nNumber].m_bAtFirstType = TRUE;
		m_vCells[nNumber].AdjustCellPosition(
			nGrayPosX,
			(nCell & 1 ) != 0 ? nUpperY : nLowerY,
			nCellSize);

		nGrayPosX += nCellSize / 2;
	}
}

void CTColorCtrl::cell_based_data::ReleaseAll()
{
	m_nMoveMode = _MM_NONE;
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::grayscale_mixer_data

void CTColorCtrl::grayscale_mixer_data::OnInit( CDC &dc,
											    CPalette *pPalette,
												CSize szTotal,
												BOOL bOnlySizeChanged)
{
	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	m_vCells.SetSize(NUM_RGB_SINGLE_CELLS);
	int nCellWidth = m_szTotal.cx / 3 - 20;
	int nCellCenter = m_szTotal.cx / 2;
	int nWorkHeight = m_szTotal.cy - 20;
	int nWorkY1 = 10;
	int nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;

	if( nCellHeight < 2 )
		nCellHeight = 2;

	for( int i=0; i<NUM_RGB_SINGLE_CELLS; i++)
	{
		if(!bOnlySizeChanged)
		{
			m_vCells[i].m_clr = RGB( i, i, i);
			m_vCells[i].m_bAtFirstType = FALSE;
			m_vCells[i].m_nBorderSize = 0;
		}

		CRect rect;
		rect.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
		rect.bottom = rect.top + nCellHeight;
		rect.left = nCellCenter - nCellWidth / 2;
		rect.right = rect.left + nCellWidth;
		m_vCells[i].m_rcPosition = rect;
	}
}

void CTColorCtrl::grayscale_mixer_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	for( int i=0; i<m_vCells.GetSize(); i++)
		m_vCells[i].OnDraw( dc, pPalette, FALSE);

	if( GetRValue(m_clr) == GetGValue(m_clr) &&
		GetRValue(m_clr) == GetBValue(m_clr))
		m_vCells[GetRValue(m_clr)].OnDraw( dc, pPalette, TRUE);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::rgb_mixer_data

void CTColorCtrl::rgb_mixer_data::OnInit( CDC &dc,
										  CPalette *pPalette,
										  CSize szTotal,
										  BOOL bOnlySizeChanged)
{
	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);
	m_vCells.SetSize(NUM_RGB_FULL_CELLS);

	int nCellWidth = m_szTotal.cx / 3 - 20;
	int nCellCenter = m_szTotal.cx / 2;
	int nWorkHeight = m_szTotal.cy - 20;
	int nWorkY1 = 10;
	int nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;

	if( nCellHeight < 2 )
		nCellHeight = 2;

	for( int i=0; i<NUM_RGB_SINGLE_CELLS; i++)
	{
		if(!bOnlySizeChanged)
		{
			m_vCells[i].m_clr = RGB( i, 0, 0);
			m_vCells[i].m_bAtFirstType = FALSE;
			m_vCells[i].m_nBorderSize = 0;
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_clr = RGB( 0, i, 0);
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_bAtFirstType = FALSE;
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_clr = RGB( 0, 0, i);
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_bAtFirstType = FALSE;
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_nBorderSize = 0;
		}

		CRect rcG;
		rcG.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
		rcG.bottom = rcG.top + nCellHeight;
		rcG.left = nCellCenter - nCellWidth / 2;
		rcG.right = rcG.left + nCellWidth;

		CRect rcR(rcG);
		CRect rcB(rcG);

		rcR.OffsetRect( -nCellWidth - 20, 0);
		rcB.OffsetRect( nCellWidth + 20, 0);
		m_vCells[i].m_rcPosition = rcR;
		m_vCells[i + NUM_RGB_SINGLE_CELLS].m_rcPosition = rcG;
		m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_rcPosition = rcB;
	}
}

void CTColorCtrl::rgb_mixer_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	for( int i=0; i<m_vCells.GetSize(); i++)
		m_vCells[i].OnDraw( dc, pPalette, FALSE);

	m_vCells[GetRValue(m_clr)].OnDraw( dc, pPalette, TRUE);
	m_vCells[GetGValue(m_clr) + NUM_RGB_SINGLE_CELLS].OnDraw( dc, pPalette, TRUE);
	m_vCells[GetBValue(m_clr) + NUM_RGB_SINGLE_CELLS * 2].OnDraw( dc, pPalette, TRUE);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::rgb_mixer_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_nMoveMode != _MM_CELL_SCAN )
		return;

	for( int i=0; i<NUM_RGB_FULL_CELLS; i++)
	{
		if(!m_vCells[i].HitTest(point))
			continue;

		if( i < NUM_RGB_SINGLE_CELLS )
		{
			m_clr = RGB(
				i, GetGValue(m_clr),
				GetBValue(m_clr));

			return;
		}

		if( i < NUM_RGB_SINGLE_CELLS * 2 )
		{
			m_clr = RGB(
				GetRValue(m_clr),
				i - NUM_RGB_SINGLE_CELLS,
				GetBValue(m_clr));

			return;
		}

		m_clr = RGB(
			GetRValue(m_clr),
			GetGValue(m_clr),
			i - NUM_RGB_SINGLE_CELLS * 2);

		return;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::rgb_cube_data

CTColorCtrl::rgb_cube_data::rgb_cube_data()
{
	m_nSideGap = DEFAULT_RGB_CUBE_GAP;
	m_szHalfSelBox.cx = 4;
	m_szHalfSelBox.cy = 4;
	RecalcLayout();
}

CTColorCtrl::rgb_cube_data::~rgb_cube_data()
{
	ReleaseAll();
}

void CTColorCtrl::rgb_cube_data::ReleaseAll()
{
	if(m_dcCube.GetSafeHdc())
		m_dcCube.DeleteDC();

	m_nMoveMode	= _MM_NONE;
}

void CTColorCtrl::rgb_cube_data::Generate( CDC &dc, CPalette *pPalette)
{
	if(!m_dcCube.GetSafeHdc())
	{
		m_rcCube.SetRect(
			0, 0,
			m_szTotal.cx,
			m_szTotal.cy);

		m_rcCube.DeflateRect( m_nSideGap, m_nSideGap);
		CSize szCube = m_rcCube.Size();

		if( szCube.cx > szCube.cy )
			m_rcCube.DeflateRect( (szCube.cx - szCube.cy) / 2, 0);

		if( szCube.cy > szCube.cx )
			m_rcCube.DeflateRect( 0, (szCube.cy - szCube.cx) / 2);

		szCube = m_rcCube.Size();
		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		szCube = m_rcCube.Size();
		int nMetric = szCube.cy / 4;

		szCube = m_rcCube.Size();
		m_ptCubeC = m_rcCube.CenterPoint();
		m_ptCubeR.x = m_ptCubeC.x;
		m_ptCubeR.y = m_rcCube.top;
		m_ptCubeG.x = m_rcCube.left;
		m_ptCubeG.y = m_rcCube.bottom - nMetric;
		m_ptCubeB.x = m_rcCube.right;
		m_ptCubeB.y = m_rcCube.bottom - nMetric;

		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp, dc,
			szCube.cx,
			szCube.cy);

		m_dcCube.CreateCompatibleDC(&dc);
		m_dcCube.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcCube.SelectPalette( pPalette, FALSE);
			m_dcCube.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		m_dcCube.FillSolidRect(
			0, 0,
			szCube.cx,
			szCube.cy,
			DEF_RGB_CUBE_TRANSPARENT);

		CPoint ptStart(
			szCube.cx / 2,
			szCube.cy / 2);

		int nHalf = szCube.cy / 2;
		for( int nR=0; nR<=nHalf; nR++)
		{
			for( int nG=0; nG<=nHalf; nG++)
			{
				CPoint pt(ptStart);

				pt.x -= nG + 1;
				pt.y += nG / 2;
				pt.y -= nR;

				COLORREF clr = RGB(
					(nR * 255) / nHalf,
					(nG * 255) / nHalf,
					DEF_RGB_CUBE_HL);

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(
						CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
			}
		}

		for( nR=0; nR<=nHalf; nR++)
		{
			for( int nB=0; nB<=nHalf; nB++)
			{
				CPoint pt(ptStart);

				pt.x += nB;
				pt.y += nB / 2;
				pt.y -= nR;

				COLORREF clr = RGB(
					(nR * 255) / nHalf,
					DEF_RGB_CUBE_HL,
					(nB * 255) / nHalf);

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(
						CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
			}
		}

		for( int nB=0; nB<=nHalf; nB++)
		{
			for( int nG=0; nG<=nHalf; nG++)
			{
				CPoint pt(ptStart);

				pt.x -= nG + 1;
				pt.y += nG / 2;
				pt.x += nB + 1;
				pt.y += nB / 2;

				COLORREF clr = RGB(
					DEF_RGB_CUBE_HL,
					(nG * 255) / nHalf,
					(nB * 255) / nHalf);

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(
						CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
				{
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
					m_dcCube.SetPixelV( pt.x - 1, pt.y, clr);
				}
			}
		}

		if(pOldPalette)
			m_dcCube.SelectPalette( pOldPalette, FALSE);

		RecalcLayout();
	}
}

void CTColorCtrl::rgb_cube_data::OnInit( CDC &dc,
										 CPalette *pPalette,
										 CSize szTotal,
										 BOOL bOnlySizeChanged)
{
	if( m_szTotal != szTotal )
		ReleaseAll();

	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	Generate( dc, pPalette);
	RecalcLayout();
}

void CTColorCtrl::rgb_cube_data::OnDone()
{
	ReleaseAll();
}

void CTColorCtrl::rgb_cube_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	Generate( dc, pPalette);
	CSize szCube = m_rcCube.Size();

	CTPaintManager::TransparentBlt(
		dc.GetSafeHdc(),
		m_rcCube.left,
		m_rcCube.top,
		-1, -1,
		m_dcCube.GetSafeHdc(),
		0, 0,
		szCube.cx,
		szCube.cy,
		DEF_RGB_CUBE_TRANSPARENT);

	CPen penBlack;
	CPen penWhite;
	CPen *pOldPen;

	penBlack.CreateStockObject(BLACK_PEN);
	penWhite.CreateStockObject(WHITE_PEN);
	pOldPen = dc.SelectObject(&penBlack);

	dc.MoveTo(m_ptCubeR);
	dc.LineTo(m_ptCubeC);
	dc.LineTo(m_ptCubeB);
	dc.MoveTo(m_ptCubeG);
	dc.LineTo(m_ptCubeC);

	pOldPen = dc.SelectObject(&penWhite);
	dc.MoveTo(m_ptPlainRG);
	dc.LineTo(m_ptClrG);
	dc.LineTo(m_ptPlainGB);
	dc.LineTo(m_ptClrB);
	dc.LineTo(m_ptPlainRB);
	dc.LineTo(m_ptClrR);
	dc.LineTo(m_ptPlainRG);
	dc.LineTo(m_ptClrCurr);
	dc.LineTo(m_ptPlainRB);
	dc.MoveTo(m_ptClrCurr);
	dc.LineTo(m_ptPlainGB);

	CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx + 1,
		m_szHalfSelBox.cy + 1);

	dc.Draw3dRect( &rcDrag, RGB( 0, 0, 0), RGB( 0, 0, 0));
	rcDrag.DeflateRect( 1, 1);

	dc.Draw3dRect( &rcDrag, RGB( 255, 255, 255), RGB( 255, 255, 255));
	rcDrag.SetRect( m_ptClrG, m_ptClrG);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	dc.Draw3dRect( &rcDrag, RGB( 0, 0, 0), RGB( 0, 0, 0));
	rcDrag.DeflateRect( 1, 1);
	dc.Draw3dRect( &rcDrag, RGB( 255, 255, 255), RGB( 255, 255, 255));

	rcDrag.SetRect( m_ptClrB, m_ptClrB);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	dc.Draw3dRect( &rcDrag, RGB( 0, 0, 0), RGB( 0, 0, 0));
	rcDrag.DeflateRect( 1, 1);
	dc.Draw3dRect( &rcDrag, RGB( 255, 255, 255), RGB( 255, 255, 255));
	dc.SelectObject(pOldPen);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::rgb_cube_data::RecalcLayout()
{
	CSize szCube = m_rcCube.Size();

	if( szCube.cx < 1 || szCube.cy < 1 )
		return;

	m_ptClrR = m_ptCubeC;
	m_ptClrG = m_ptCubeC;
	m_ptClrB = m_ptCubeC;
	m_ptClrCurr = m_ptCubeC;

	int nHalf = szCube.cy / 2;
	m_ptClrR.y -= (GetRValue(m_clr) * nHalf) / 255;
	m_ptClrG.x -= (GetGValue(m_clr) * nHalf) / 255;
	m_ptClrG.y += (GetGValue(m_clr) * nHalf) / 255 / 2;
	m_ptClrB.x += (GetBValue(m_clr) * nHalf) / 255;
	m_ptClrB.y += (GetBValue(m_clr) * nHalf) / 255 / 2;

	m_ptPlainRG.x = m_ptClrG.x;
	m_ptPlainRG.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrG.y);
	m_ptPlainRB.x = m_ptClrB.x;
	m_ptPlainRB.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrB.y);
	m_ptPlainGB.x = m_ptClrG.x - (m_ptCubeC.x - m_ptClrB.x);
	m_ptPlainGB.y = m_ptClrG.y - (m_ptCubeC.y - m_ptClrB.y);

	m_ptClrCurr = m_ptPlainGB;
	m_ptClrCurr.y -= m_ptCubeC.y - m_ptClrR.y;
}

void CTColorCtrl::rgb_cube_data::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
	CRect rcDrag;

	rcDrag.SetRect( m_ptClrR, m_ptClrR);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_RED;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	rcDrag.SetRect( m_ptClrG, m_ptClrG);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_GREEN;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	rcDrag.SetRect( m_ptClrB, m_ptClrB);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_BLUE;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	int nBPP = g_PaintManager->GetBPP();
	if( nBPP > 8 )
	{
		CPoint pta[4];
		CRgn rgn;
		int nQuad = m_rcCube.Size().cy / 4;

		pta[0] = m_ptCubeG;
		pta[1] = CPoint( m_ptCubeG.x, m_ptCubeR.y + nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(clr),
					GetBValue(m_clr));
			}

			OnSetRGB(clr);
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeB;
		pta[1] = CPoint( m_ptCubeB.x, m_ptCubeR.y + nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(m_clr),
					GetBValue(clr));
			}

			OnSetRGB(clr);
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeG;
		pta[1] = CPoint( m_ptCubeC.x, m_ptCubeG.y + nQuad);
		pta[2] = m_ptCubeB;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(m_clr),
					GetGValue(clr),
					GetBValue(clr));
			}

			OnSetRGB(clr);
			return;
		}
	}
}

void CTColorCtrl::rgb_cube_data::OnLButtonUp( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::rgb_cube_data::OnMouseMove( UINT nFlags, CPoint point)
{
	switch(m_nMoveMode)
	{
	case _MM_RED	:
		{
			if( point.y > m_ptCubeC.y )
				point.y = m_ptCubeC.y;

			if( point.y < m_ptCubeR.y )
				point.y = m_ptCubeR.y;

			int nVal = GetRValue(m_clr);
			int nValNew = 255 * (point.y - m_ptCubeC.y) / (m_ptCubeR.y - m_ptCubeC.y);

			if( nVal == nValNew )
				return;

			m_clr = RGB(
				nValNew,
				GetGValue(m_clr),
				GetBValue(m_clr));

			RecalcLayout();
		}

		return;

	case _MM_GREEN	:
		{
			if( point.x > m_ptCubeC.x )
				point.x = m_ptCubeC.x;

			if( point.x < m_ptCubeG.x )
				point.x = m_ptCubeG.x;

			int nVal = GetGValue(m_clr);
			int nValNew = 255 * (point.x-m_ptCubeC.x) / (m_ptCubeG.x - m_ptCubeC.x);

			if( nVal == nValNew )
				return;

			m_clr = RGB(
				GetRValue(m_clr),
				nValNew,
				GetBValue(m_clr));

			RecalcLayout();
		}

		return;

	case _MM_BLUE	:
		{
			if( point.x < m_ptCubeC.x )
				point.x = m_ptCubeC.x;

			if( point.x > m_ptCubeB.x )
				point.x = m_ptCubeB.x;

			int nVal = GetBValue(m_clr);
			int nValNew = 255 * (m_ptCubeC.x - point.x) / (m_ptCubeC.x - m_ptCubeB.x);

			if( nVal == nValNew )
				return;

			m_clr = RGB(
				GetRValue(m_clr),
				GetGValue(m_clr),
				nValNew);
			RecalcLayout();
		}

		return;
	}
}

void CTColorCtrl::rgb_cube_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);
	RecalcLayout();
}

void CTColorCtrl::rgb_cube_data::OnSetHLS( double hue,
										   double luminance,
										   double saturation)
{
	any_picker_data::OnSetHLS(
		hue,
		luminance,
		saturation);
	RecalcLayout();
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::cmy_mixer_data

void CTColorCtrl::cmy_mixer_data::OnInit( CDC &dc,
										  CPalette *pPalette,
										  CSize szTotal,
										  BOOL bOnlySizeChanged)
{
	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	m_vCells.SetSize(NUM_RGB_FULL_CELLS);
	int nCellWidth = m_szTotal.cx / 3 - 20;
	int nCellCenter = m_szTotal.cx / 2;
	int nWorkHeight = m_szTotal.cy - 20;
	int nWorkY1 = 10;
	int nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;

	if( nCellHeight < 2 )
		nCellHeight = 2;

	for( int i=0; i<NUM_RGB_SINGLE_CELLS; i++)
	{
		if(!bOnlySizeChanged)
		{
			m_vCells[i].m_clr = CTPaintManager::CMYtoRGB(RGB( i, 0, 0));
			m_vCells[i].m_bAtFirstType = FALSE;
			m_vCells[i].m_nBorderSize = 0;
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_clr = CTPaintManager::CMYtoRGB(RGB( 0, i, 0));
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_bAtFirstType = FALSE;
			m_vCells[i + NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_clr = CTPaintManager::CMYtoRGB(RGB( 0, 0, i));
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_bAtFirstType = FALSE;
			m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_nBorderSize = 0;
		}

		CRect rcM;
		rcM.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
		rcM.bottom = rcM.top + nCellHeight;
		rcM.left = nCellCenter - nCellWidth / 2;
		rcM.right = rcM.left + nCellWidth;

		CRect rcC(rcM);
		CRect rcY(rcM);

		rcC.OffsetRect( -nCellWidth - 20, 0);
		rcY.OffsetRect( nCellWidth + 20, 0);
		m_vCells[i].m_rcPosition = rcC;
		m_vCells[i + NUM_RGB_SINGLE_CELLS].m_rcPosition = rcM;
		m_vCells[i + NUM_RGB_SINGLE_CELLS*2].m_rcPosition = rcY;
	}
}

void CTColorCtrl::cmy_mixer_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	for( int i=0; i<m_vCells.GetSize(); i++)
		m_vCells[i].OnDraw( dc, pPalette, FALSE);

	COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
	m_vCells[GetRValue(clr)].OnDraw( dc, pPalette, TRUE);
	m_vCells[GetGValue(clr) + NUM_RGB_SINGLE_CELLS].OnDraw( dc, pPalette, TRUE);
	m_vCells[GetBValue(clr) + NUM_RGB_SINGLE_CELLS * 2].OnDraw( dc, pPalette, TRUE);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::cmy_mixer_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_nMoveMode != _MM_CELL_SCAN )
		return;

	for( int i=0; i<NUM_RGB_FULL_CELLS; i++)
	{
		if(!m_vCells[i].HitTest(point))
			continue;

		COLORREF clrOrg = CTPaintManager::RGBtoCMY(m_clr);
		if( i < NUM_RGB_SINGLE_CELLS )
		{
			COLORREF clr = RGB(
				i, GetGValue(clrOrg),
				GetBValue(clrOrg));

			m_clr = CTPaintManager::CMYtoRGB(clr);
			return;
		}

		if( i < NUM_RGB_SINGLE_CELLS * 2 )
		{
			COLORREF clr = RGB(
				GetRValue(clrOrg), i,
				GetBValue(clrOrg));

			m_clr = CTPaintManager::CMYtoRGB(clr);
			return;
		}

		COLORREF clr = RGB(
			GetRValue(clrOrg),
			GetGValue(clrOrg), i);

		m_clr = CTPaintManager::CMYtoRGB(clr);
		return;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::cmy_cube_data

CTColorCtrl::cmy_cube_data::cmy_cube_data()
{
	m_nSideGap = DEFAULT_RGB_CUBE_GAP;
	m_szHalfSelBox.cx = 4;
	m_szHalfSelBox.cy = 4;
	RecalcLayout();
}

CTColorCtrl::cmy_cube_data::~cmy_cube_data()
{
	ReleaseAll();
}

void CTColorCtrl::cmy_cube_data::ReleaseAll()
{
	if(m_dcCube.GetSafeHdc())
		m_dcCube.DeleteDC();

	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::cmy_cube_data::Generate( CDC &dc, CPalette *pPalette)
{
	if(!m_dcCube.GetSafeHdc())
	{
		m_rcCube.SetRect(
			0, 0,
			m_szTotal.cx,
			m_szTotal.cy);

		m_rcCube.DeflateRect( m_nSideGap, m_nSideGap);
		CSize szCube = m_rcCube.Size();

		if( szCube.cx > szCube.cy )
			m_rcCube.DeflateRect( (szCube.cx - szCube.cy) / 2, 0);

		if( szCube.cy > szCube.cx )
			m_rcCube.DeflateRect( 0, (szCube.cy - szCube.cx) / 2);

		szCube = m_rcCube.Size();
		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cx & 0x3)
			m_rcCube.right--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		if(szCube.cy & 0x3)
			m_rcCube.bottom--;

		szCube = m_rcCube.Size();
		int nMetric = szCube.cy / 4;

		szCube = m_rcCube.Size();
		m_ptCubeC = m_rcCube.CenterPoint();
		m_ptCubeR.x = m_ptCubeC.x;
		m_ptCubeR.y = m_rcCube.top;
		m_ptCubeG.x = m_rcCube.left;
		m_ptCubeG.y = m_rcCube.bottom - nMetric;
		m_ptCubeB.x = m_rcCube.right;
		m_ptCubeB.y = m_rcCube.bottom - nMetric;

		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp, dc,
			szCube.cx,
			szCube.cy);

		m_dcCube.CreateCompatibleDC(&dc);
		m_dcCube.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcCube.SelectPalette( pPalette, FALSE);
			m_dcCube.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		m_dcCube.FillSolidRect(
			0, 0,
			szCube.cx,
			szCube.cy,
			DEF_RGB_CUBE_TRANSPARENT);

		CPoint ptStart(
			szCube.cx / 2,
			szCube.cy / 2);

		int nHalf = szCube.cy / 2;
		for( int nR=0; nR<=nHalf; nR++)
		{
			for( int nG=0; nG<=nHalf; nG++)
			{
				CPoint pt(ptStart);

				pt.x -= nG + 1;
				pt.y += nG / 2;
				pt.y -= nR;

				COLORREF clr = CTPaintManager::CMYtoRGB(RGB(
					(nR * 255) / nHalf,
					(nG * 255) / nHalf,
					DEF_RGB_CUBE_HL));

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
			}
		}

		for( int nR=0; nR<=nHalf; nR++)
		{
			for( int nB=0; nB<=nHalf; nB++)
			{
				CPoint pt(ptStart);

				pt.x += nB;
				pt.y += nB / 2;
				pt.y -= nR;

				COLORREF clr = CTPaintManager::CMYtoRGB(RGB(
					(nR * 255) / nHalf,
					DEF_RGB_CUBE_HL,
					(nB * 255) / nHalf));

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(
						CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
			}
		}

		for( int nB=0; nB<=nHalf; nB++)
		{
			for( int nG=0; nG<=nHalf; nG++)
			{
				CPoint pt(ptStart);
				pt.x -= nG + 1;
				pt.y += nG / 2;
				pt.x += nB + 1;
				pt.y += nB / 2;

				COLORREF clr = CTPaintManager::CMYtoRGB(RGB(
					DEF_RGB_CUBE_HL,
					(nG * 255) / nHalf,
					(nB * 255) / nHalf));

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcCube.FillRect(
						CRect(
						CPoint(
						pt.x - nStep / 2,
						pt.y - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
				{
					m_dcCube.SetPixelV( pt.x, pt.y, clr);
					m_dcCube.SetPixelV( pt.x - 1, pt.y, clr);
				}
			}
		}

		if( pOldPalette != NULL )
			m_dcCube.SelectPalette( pOldPalette, FALSE);

		RecalcLayout();
	}
}

void CTColorCtrl::cmy_cube_data::OnInit( CDC &dc,
										 CPalette *pPalette,
										 CSize szTotal,
										 BOOL bOnlySizeChanged)
{
	if( m_szTotal != szTotal )
		ReleaseAll();

	any_picker_data::OnInit(
		dc, pPalette,
		szTotal,
		bOnlySizeChanged);

	Generate( dc, pPalette);
	RecalcLayout();
}

void CTColorCtrl::cmy_cube_data::OnDone()
{
	ReleaseAll();
}

void CTColorCtrl::cmy_cube_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	Generate( dc, pPalette);
	CSize szCube = m_rcCube.Size();

	CTPaintManager::TransparentBlt(
		dc.GetSafeHdc(),
		m_rcCube.left,
		m_rcCube.top,
		-1, -1,
		m_dcCube.GetSafeHdc(),
		0, 0,
		szCube.cx,
		szCube.cy,
		DEF_RGB_CUBE_TRANSPARENT);

	CPen penBlack;
	CPen penWhite;
	CPen *pOldPen;

	penBlack.CreateStockObject(BLACK_PEN);
	penWhite.CreateStockObject(WHITE_PEN);
	pOldPen = dc.SelectObject(&penBlack);

	dc.MoveTo(m_ptCubeR);
	dc.LineTo(m_ptCubeC);
	dc.LineTo(m_ptCubeB);
	dc.MoveTo(m_ptCubeG);
	dc.LineTo(m_ptCubeC);

	pOldPen = dc.SelectObject(&penWhite);
	dc.MoveTo(m_ptPlainRG);
	dc.LineTo(m_ptClrG);
	dc.LineTo(m_ptPlainGB);
	dc.LineTo(m_ptClrB);
	dc.LineTo(m_ptPlainRB);
	dc.LineTo(m_ptClrR);
	dc.LineTo(m_ptPlainRG);
	dc.LineTo(m_ptClrCurr);
	dc.LineTo(m_ptPlainRB);
	dc.MoveTo(m_ptClrCurr);
	dc.LineTo(m_ptPlainGB);

	CRect rcDrag;
	rcDrag.SetRect( m_ptClrR, m_ptClrR);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx + 1,
		m_szHalfSelBox.cy + 1);

	dc.Draw3dRect(
		&rcDrag,
		RGB( 0, 0, 0),
		RGB( 0, 0, 0));

	rcDrag.DeflateRect( 1, 1);
	dc.Draw3dRect(
		&rcDrag,
		RGB( 255, 255, 255),
		RGB( 255, 255, 255));

	rcDrag.SetRect( m_ptClrG, m_ptClrG);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	dc.Draw3dRect(
		&rcDrag,
		RGB( 0, 0, 0),
		RGB( 0, 0, 0));

	rcDrag.DeflateRect( 1, 1);
	dc.Draw3dRect(
		&rcDrag,
		RGB( 255, 255, 255),
		RGB( 255, 255, 255));

	rcDrag.SetRect( m_ptClrB, m_ptClrB);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	dc.Draw3dRect(
		&rcDrag,
		RGB( 0, 0, 0),
		RGB( 0, 0, 0));

	rcDrag.DeflateRect( 1, 1);
	dc.Draw3dRect(
		&rcDrag,
		RGB( 255, 255, 255),
		RGB( 255, 255, 255));

	dc.SelectObject(pOldPen);
	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::cmy_cube_data::RecalcLayout()
{
	CSize szCube = m_rcCube.Size();

	if( szCube.cx < 1 || szCube.cy < 1 )
		return;

	m_ptClrR = m_ptCubeC;
	m_ptClrG = m_ptCubeC;
	m_ptClrB = m_ptCubeC;
	m_ptClrCurr = m_ptCubeC;

	int nHalf = szCube.cy / 2;
	COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
	m_ptClrR.y -= (GetRValue(clr) * nHalf) / 255;
	m_ptClrG.x -= (GetGValue(clr) * nHalf) / 255;
	m_ptClrG.y += (GetGValue(clr) * nHalf) / 255 / 2;
	m_ptClrB.x += (GetBValue(clr) * nHalf) / 255;
	m_ptClrB.y += (GetBValue(clr) * nHalf) / 255 / 2;

	m_ptPlainRG.x = m_ptClrG.x;
	m_ptPlainRG.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrG.y);
	m_ptPlainRB.x = m_ptClrB.x;
	m_ptPlainRB.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrB.y);
	m_ptPlainGB.x = m_ptClrG.x - (m_ptCubeC.x - m_ptClrB.x);
	m_ptPlainGB.y = m_ptClrG.y - (m_ptCubeC.y - m_ptClrB.y);

	m_ptClrCurr = m_ptPlainGB;
	m_ptClrCurr.y -= m_ptCubeC.y - m_ptClrR.y;
}

void CTColorCtrl::cmy_cube_data::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
	CRect rcDrag;

	rcDrag.SetRect( m_ptClrR, m_ptClrR);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_RED;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	rcDrag.SetRect( m_ptClrG, m_ptClrG);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_GREEN;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	rcDrag.SetRect( m_ptClrB, m_ptClrB);
	rcDrag.InflateRect(
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy,
		m_szHalfSelBox.cx,
		m_szHalfSelBox.cy);

	if(rcDrag.PtInRect(point))
	{
		m_nMoveMode = _MM_BLUE;
		OnMouseMove( nFlags|MK_LBUTTON, point);

		return;
	}

	int nBPP = g_PaintManager->GetBPP();
	if( nBPP > 8 )
	{
		CPoint pta[4];
		CRgn rgn;
		int nQuad = m_rcCube.Size().cy / 4;

		pta[0] = m_ptCubeG;
		pta[1] = CPoint( m_ptCubeG.x, m_ptCubeR.y + nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(clr),
					GetBValue(m_clr));
			}

			OnSetRGB(clr);
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeB;
		pta[1] = CPoint( m_ptCubeB.x, m_ptCubeR.y + nQuad);
		pta[2] = m_ptCubeR;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(clr),
					GetGValue(m_clr),
					GetBValue(clr));
			}

			OnSetRGB(clr);
			return;
		}

		rgn.DeleteObject();
		pta[0] = m_ptCubeG;
		pta[1] = CPoint( m_ptCubeC.x, m_ptCubeG.y + nQuad);
		pta[2] = m_ptCubeB;
		pta[3] = m_ptCubeC;
		rgn.CreatePolygonRgn( pta, 4, ALTERNATE);

		if(rgn.PtInRegion(point))
		{
			point -= m_rcCube.TopLeft();
			COLORREF clr = m_dcCube.GetPixel(point);

			if( clr == DEF_RGB_CUBE_TRANSPARENT )
				return;

			if(nFlags & MK_CONTROL)
			{
				clr = RGB(
					GetRValue(m_clr),
					GetGValue(clr),
					GetBValue(clr));
			}

			OnSetRGB(clr);
			return;
		}
	}
}

void CTColorCtrl::cmy_cube_data::OnLButtonUp( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::cmy_cube_data::OnMouseMove( UINT nFlags, CPoint point)
{
	switch(m_nMoveMode)
	{
	case _MM_RED	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			if( point.y > m_ptCubeC.y )
				point.y = m_ptCubeC.y;

			if( point.y < m_ptCubeR.y )
				point.y = m_ptCubeR.y;

			int nVal = GetRValue(m_clr);
			int nValNew = 255 * (point.y - m_ptCubeC.y) / (m_ptCubeR.y - m_ptCubeC.y);

			if( nVal == nValNew )
				return;

			clr = RGB(
				nValNew,
				GetGValue(clr),
				GetBValue(clr));

			m_clr = CTPaintManager::CMYtoRGB(clr);
			RecalcLayout();
		}

		return;

	case _MM_GREEN	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			if( point.x > m_ptCubeC.x )
				point.x = m_ptCubeC.x;

			if( point.x < m_ptCubeG.x )
				point.x = m_ptCubeG.x;

			int nVal = GetGValue(clr);
			int nValNew = 255 * (point.x - m_ptCubeC.x) / (m_ptCubeG.x - m_ptCubeC.x);

			if( nVal == nValNew )
				return;

			clr = RGB(
				GetRValue(clr),
				nValNew,
				GetBValue(clr));

			m_clr = CTPaintManager::CMYtoRGB(clr);
			RecalcLayout();
		}

		return;

	case _MM_BLUE	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			if( point.x < m_ptCubeC.x )
				point.x = m_ptCubeC.x;

			if( point.x > m_ptCubeB.x )
				point.x = m_ptCubeB.x;

			int nVal = GetBValue(clr);
			int nValNew = 255 * (m_ptCubeC.x - point.x) / (m_ptCubeC.x - m_ptCubeB.x);

			if( nVal == nValNew )
				return;

			clr = RGB(
				GetRValue(clr),
				GetGValue(clr),
				nValNew);

			m_clr = CTPaintManager::CMYtoRGB(clr);
			RecalcLayout();
		}

		return;
	}
}

void CTColorCtrl::cmy_cube_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);
	RecalcLayout();
}

void CTColorCtrl::cmy_cube_data::OnSetHLS( double hue,
										   double luminance,
										   double saturation)
{
	any_picker_data::OnSetHLS(
		hue,
		luminance,
		saturation);

	RecalcLayout();
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::hsl_roller_data

void CTColorCtrl::hsl_roller_data::GeneratePicker( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidPicker && m_dcPicker.GetSafeHdc() )
		m_dcPicker.DeleteDC();

	if(!m_dcPicker.GetSafeHdc())
	{
		if(m_rgnSpaceHueSat.GetSafeHandle())
			m_rgnSpaceHueSat.DeleteObject();

		if(m_rgnMouseSelHueSat.GetSafeHandle())
			m_rgnMouseSelHueSat.DeleteObject();

		CBitmap bmp;
		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorPicker.cx,
			m_szColorPicker.cy);

		m_dcPicker.CreateCompatibleDC(&dc);
		m_dcPicker.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcPicker.SelectPalette( pPalette, FALSE);
			m_dcPicker.RealizePalette();
		}

		CRect rcArea(
			0, 0,
			m_szTotal.cx,
			m_szTotal.cy);

		m_dcPicker.FillSolidRect( rcArea, DEF_HSL_TRANSPARENT);
		int nBPP = g_PaintManager->GetBPP();
		int nBorder = min( m_szColorPicker.cx, m_szColorPicker.cy);

		nBorder -= m_nAreaGap;
		m_ptCenterSpaceHueSat.x = m_szColorPicker.cx / 2;
		m_ptCenterSpaceHueSat.y = m_szColorPicker.cy / 2;
		double fLuminance = DEFAULT_LUMINANCE;

		m_nSatTranslation = nBorder / 2;
		int nRad2 = m_nSatTranslation * m_nSatTranslation;
		CRect rcEllipse(
			m_ptCenterSpaceHueSat.x,
			m_ptCenterSpaceHueSat.y,
			m_ptCenterSpaceHueSat.x,
			m_ptCenterSpaceHueSat.y);

		rcEllipse.InflateRect(
			m_nSatTranslation,
			m_nSatTranslation);

		m_rgnSpaceHueSat.CreateEllipticRgn(
			rcEllipse.left,
			rcEllipse.top,
			rcEllipse.right,
			rcEllipse.bottom);

		CRect rcEllipseMouseSel(rcEllipse);
		rcEllipseMouseSel.InflateRect(
			__SEL_BOX_DX * 3,
			__SEL_BOX_DY * 3);

		m_rgnMouseSelHueSat.CreateEllipticRgn(
			rcEllipseMouseSel.left,
			rcEllipseMouseSel.top,
			rcEllipseMouseSel.right,
			rcEllipseMouseSel.bottom);

		BOOL bResetRgn = FALSE;
		if( nBPP <= 8 )
		{
			bResetRgn = TRUE;
			m_dcPicker.SelectClipRgn(&m_rgnSpaceHueSat);
		}

		for( int nPosX = rcEllipse.left; nPosX<rcEllipse.right; nPosX++)
		{
			int nPosXoffs = m_ptCenterSpaceHueSat.x - nPosX;
			int nPosX2 = nPosXoffs * nPosXoffs;

			for( int nPosY = rcEllipse.top; nPosY<rcEllipse.bottom; nPosY++)
			{
				int nPosYoffs = m_ptCenterSpaceHueSat.y - nPosY;
				int nPosY2 = nPosYoffs * nPosYoffs;
				int nRadCalc2 = nPosX2 + nPosY2;

				if( nRadCalc2 > nRad2 )
					continue;

				if( nBPP <= 8 )
				{
					if( (nPosX % 4) || (nPosY % 4) )
						continue;
				}

				double fSaturation = double(nRadCalc2) / double(nRad2);
				double fHue = ::atan2( double(nPosYoffs), double(nPosXoffs));

				fHue /= 2.0 * PI;
				COLORREF clr = CTPaintManager::HLStoRGB(
					fHue,
					fLuminance,
					fSaturation);

				if( nBPP <= 8 )
				{
					CRect _rect( nPosX, nPosY, nPosX, nPosY);
					_rect.InflateRect( 2, 2);

					CBrush _brush(clr);
					m_dcPicker.FillRect(
						&_rect,
						&_brush);
				}
				else
				{
					m_dcPicker.SetPixel(
						nPosX,
						nPosY,
						clr);
				}
			}
		}

		if(pOldPalette)
			m_dcPicker.SelectPalette( pOldPalette, FALSE);

		if(bResetRgn)
			m_dcPicker.SelectClipRgn(NULL);

		m_bBmpValidPicker = TRUE;
	}
}

void CTColorCtrl::hsl_roller_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	GeneratePicker( dc, pPalette);
	GenerateSlider( dc, pPalette);

	CTPaintManager::TransparentBlt(
		dc.GetSafeHdc(),
		m_nAreaGap,
		m_nAreaGap,
		-1, -1,
		m_dcPicker.GetSafeHdc(),
		0, 0,
		m_szColorPicker.cx,
		m_szColorPicker.cy,
		DEF_HSL_TRANSPARENT);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap * 2 + m_szColorPicker.cy,
		m_szColorSlider.cx,
		m_szTotal.cy,
		&m_dcSlider,
		0, 0,
		SRCCOPY);

	CSize szHueSat( __SEL_BOX_DX, __SEL_BOX_DY);
	double fHueAngle = m_fCurrHue * 2.0 * PI;
	double fSatRadius = sqrt(m_fCurrSaturation) * double(m_nSatTranslation);
	CPoint ptHueSat(
		INT(::cos(fHueAngle) * fSatRadius),
		INT(::sin(fHueAngle) * fSatRadius));

	CPoint ptDrawCalc(m_ptCenterSpaceHueSat - ptHueSat);
	ptDrawCalc += CPoint(m_nAreaGap,m_nAreaGap);

	DrawCrossAt(
		ptDrawCalc,
		dc, szHueSat);

	CSize szLumTracker( __SEL_BOX_DX, m_szColorSlider.cy);
	CPoint ptLum(
		INT(m_fCurrLuminance * m_szColorSlider.cx),
		m_szColorPicker.cy + m_szColorSlider.cy / 2);

	ptDrawCalc = ptLum + CPoint( m_nAreaGap, m_nAreaGap * 2);
	DrawCrossAt( ptDrawCalc, dc, szLumTracker);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::hsl_roller_data::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(m_bBmpValidPicker)
	{
		m_nMoveMode = _MM_NONE;

		if(m_rgnMouseSelHueSat.PtInRegion(point))
			m_nMoveMode = _MM_PICKER;
		else if( point.y - m_nAreaGap * 2 > m_szColorPicker.cy )
			m_nMoveMode = _MM_SLIDER;

		OnMouseMove( nFlags|MK_LBUTTON, point);
	}
}

void CTColorCtrl::hsl_roller_data::OnLButtonUp( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::hsl_roller_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_bBmpValidPicker && (nFlags&MK_LBUTTON) )
	{
		if( m_nMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap * 2;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorSlider.cx )
				point.x = m_szColorSlider.cx;

			m_fCurrLuminance = ((double) point.x) / ((double) m_szColorSlider.cx);
			m_clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				m_fCurrLuminance,
				m_fCurrSaturation);
		}
		else if( m_nMoveMode == _MM_PICKER )
		{
			if(m_rgnMouseSelHueSat.PtInRegion(point))
			{
				CPoint ptOffsHueSat(
					m_ptCenterSpaceHueSat.x - point.x,
					m_ptCenterSpaceHueSat.y - point.y);

				double fHueAngle = ::atan2( double(ptOffsHueSat.y), double(ptOffsHueSat.x));
				double fHue = fHueAngle / (2.0 * PI);
				double fSaturation = 1.0;
				double fLuminance = m_fCurrLuminance;

				int nRadCalc2 = ptOffsHueSat.x * ptOffsHueSat.x + ptOffsHueSat.y * ptOffsHueSat.y;
				int nRadCalc = INT(::sqrt((FLOAT) nRadCalc2));

				if( nRadCalc > m_nSatTranslation )
				{
					ptOffsHueSat.x = INT(double(m_nSatTranslation) * ::cos(fHueAngle));
					ptOffsHueSat.y = INT(double(m_nSatTranslation) * ::sin(fHueAngle));
					nRadCalc2 = ptOffsHueSat.x * ptOffsHueSat.x + ptOffsHueSat.y * ptOffsHueSat.y;
				}
				else
					fSaturation = double(nRadCalc2) / double(m_nSatTranslation * m_nSatTranslation);

				OnSetHLS(
					fHue,
					m_fCurrLuminance,
					fSaturation);

				OnSetRGB(
					CTPaintManager::HLStoRGB(
					fHue,
					m_fCurrLuminance,
					fSaturation));

				m_fCurrLuminance = fLuminance;
			}
		}
	}
}

void CTColorCtrl::hsl_roller_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);
	m_bBmpValidSlider = FALSE;
}

void CTColorCtrl::hsl_roller_data::OnSetHLS( double hue,
											 double luminance,
											 double saturation)
{
	any_picker_data::OnSetHLS(
		hue,
		luminance,
		saturation);
	m_bBmpValidSlider = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::hsl_panel_data

CTColorCtrl::hsl_panel_data::hsl_panel_data()
{
	m_bBmpValidSlider = FALSE;
	m_bBmpValidPicker = FALSE;
	m_szColorPicker.cx = 10;
	m_szColorPicker.cy = 10;
	m_szColorSlider.cx = 10;
	m_szColorSlider.cy = 10;
	m_nLumBarDy = DEFAULT_LUM_BAR_HEIGHT;
	m_nAreaGap = DEFAULT_HSL_AREA_GAP;
};

CTColorCtrl::hsl_panel_data::~hsl_panel_data()
{
	ReleaseAll();
};

void CTColorCtrl::hsl_panel_data::ReleaseAll()
{
	if(m_dcPicker.GetSafeHdc())
		m_dcPicker.DeleteDC();

	if(m_dcSlider.GetSafeHdc())
		m_dcSlider.DeleteDC();

	m_bBmpValidPicker = FALSE;
	m_bBmpValidSlider = FALSE;
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::hsl_panel_data::GenerateSlider( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidSlider && m_dcSlider.GetSafeHdc() )
		m_dcSlider.DeleteDC();

	if(!m_dcSlider.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorSlider.cx,
			m_szColorSlider.cy);

		m_dcSlider.CreateCompatibleDC(&dc);
		m_dcSlider.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcSlider.SelectPalette( pPalette, FALSE);
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = m_szColorSlider.cx; i > 0; i -= nStep )
		{
			double fLuminance = double(i - 1) / double(m_szColorSlider.cx);
			COLORREF clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				fLuminance,
				m_fCurrSaturation);

			if( nBPP <= 8 )
			{
				CBrush brush(clr);

				m_dcSlider.FillRect(
					CRect(
					CPoint( i - nStep, 0),
					CSize( nStep, m_szColorSlider.cy)),
					&brush);
			}
			else
			{
				m_dcSlider.FillSolidRect(
					i - 1,
					0, 1,
					m_szColorSlider.cy,
					clr);
			}
		}

		if(pOldPalette)
			m_dcSlider.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidSlider = TRUE;
	}
}

void CTColorCtrl::hsl_panel_data::GeneratePicker( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidPicker && m_dcPicker.GetSafeHdc() )
		m_dcPicker.DeleteDC();

	if(!m_dcPicker.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorPicker.cx,
			m_szColorPicker.cy);

		m_dcPicker.CreateCompatibleDC(&dc);
		m_dcPicker.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcPicker.SelectPalette( pPalette, FALSE);
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i=0; i<m_szColorPicker.cx; i++)
		{
			for( int j=0; j<m_szColorPicker.cy; j++)
			{
				COLORREF clr = CTPaintManager::HLStoRGB(
					double(i) / double(m_szColorPicker.cx),
					DEFAULT_LUMINANCE,
					double(m_szColorPicker.cy - j - 1) / double(m_szColorPicker.cy));

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcPicker.FillRect(
						CRect(
						CPoint(
						i - nStep / 2,
						j - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcPicker.SetPixelV( i, j, clr);
			}
		}

		if(pOldPalette)
			m_dcPicker.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidPicker = TRUE;
	}
}

void CTColorCtrl::hsl_panel_data::OnInit( CDC &dc,
										  CPalette *pPalette,
										  CSize szTotal,
										  BOOL bOnlySizeChanged)
{
	BOOL bRegen = FALSE;

	if( m_szTotal != szTotal )
	{
		ReleaseAll();
		bRegen = TRUE;
	}

	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	if(bRegen)
	{
		int maxi = m_szTotal.cx - m_nAreaGap * 2;
		int maxj = m_szTotal.cy - m_nAreaGap * 3;

		m_szColorPicker.cx = maxi;
		m_szColorPicker.cy = maxj - m_nLumBarDy;
		m_szColorSlider.cx = maxi;
		m_szColorSlider.cy = m_nLumBarDy;
		m_szTotal.cx = maxi;
		m_szTotal.cy = maxj;

		GeneratePicker( dc, pPalette);
		GenerateSlider( dc, pPalette);
	}
}

void CTColorCtrl::hsl_panel_data::OnDone()
{
	ReleaseAll();
}

void CTColorCtrl::hsl_panel_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	GeneratePicker( dc, pPalette);
	GenerateSlider( dc, pPalette);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_szColorPicker.cx,
		m_szColorPicker.cy,
		&m_dcPicker,
		0, 0,
		SRCCOPY);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap * 2 + m_szColorPicker.cy,
		m_szColorSlider.cx,
		m_szTotal.cy,
		&m_dcSlider,
		0, 0,
		SRCCOPY);

	SIZE size = { __SEL_BOX_DX, __SEL_BOX_DY};
	CPoint ptDrawCalc = m_ptMousePos + CPoint( m_nAreaGap, m_nAreaGap);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	size.cx = __SEL_BOX_DX;
	size.cy = m_szColorSlider.cy;

	CPoint point( INT(m_fCurrLuminance * m_szColorSlider.cx), m_szColorPicker.cy + m_szColorSlider.cy / 2);
	ptDrawCalc = point + CPoint( m_nAreaGap, m_nAreaGap * 2);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::hsl_panel_data::DrawCrossAt( CPoint &point,
											   CDC &dc,
											   SIZE &size)
{
	CRect rc( point, size);

	rc.OffsetRect( -size.cx / 2, -size.cy / 2);
	dc.Draw3dRect(
		&rc,
		RGB( 255, 255, 255),
		RGB( 255, 255, 255));

	rc.InflateRect( 1, 1);
	dc.Draw3dRect(
		&rc,
		RGB( 0, 0, 0),
		RGB( 0, 0, 0));
}

void CTColorCtrl::hsl_panel_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);

	m_ptMousePos.x = long(double(m_szColorPicker.cx) * m_fCurrHue);
	m_ptMousePos.y = long(double(m_szColorPicker.cy) * m_fCurrSaturation);
	m_ptMousePos.y = m_szColorPicker.cy - m_ptMousePos.y - 1;
	m_bBmpValidSlider = FALSE;
}

void CTColorCtrl::hsl_panel_data::OnSetHLS( double hue,
										    double luminance,
											double saturation)
{
	any_picker_data::OnSetHLS(
		hue,
		luminance,
		saturation);

	m_ptMousePos.x = long(double(m_szColorPicker.cx) * m_fCurrHue);
	m_ptMousePos.y = long(double(m_szColorPicker.cy) * m_fCurrSaturation);
	m_ptMousePos.y = m_szColorPicker.cy - m_ptMousePos.y - 1;
	m_bBmpValidSlider = FALSE;
}

void CTColorCtrl::hsl_panel_data::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(m_bBmpValidPicker)
	{
		m_nMoveMode = _MM_PICKER;

		if( point.y - m_nAreaGap * 2 > m_szColorPicker.cy )
			m_nMoveMode = _MM_SLIDER;

		OnMouseMove( nFlags|MK_LBUTTON, point);
		OnMouseMove( nFlags|MK_LBUTTON, point);
	}
}

void CTColorCtrl::hsl_panel_data::OnLButtonUp( UINT nFlags, CPoint point)
{
	m_nMoveMode = _MM_NONE;
}

void CTColorCtrl::hsl_panel_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_bBmpValidPicker && (nFlags&MK_LBUTTON) )
	{
		if( m_nMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap * 2;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorSlider.cx )
				point.x = m_szColorSlider.cx;

			m_fCurrLuminance = ((double) point.x) / ((double) m_szColorSlider.cx);
			m_clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				m_fCurrLuminance,
				m_fCurrSaturation);
		}
		else if( m_nMoveMode == _MM_PICKER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorPicker.cx )
				point.x = m_szColorPicker.cx;

			if( point.y > m_szColorPicker.cy )
				point.y = m_szColorPicker.cy;

			if( nFlags & MK_CONTROL )
				point.x = m_ptMousePos.x;

			if( nFlags & MK_SHIFT )
				point.y = m_ptMousePos.y;

			m_fCurrSaturation = double(m_szColorPicker.cy - point.y) / double(m_szColorPicker.cy);
			m_fCurrHue = double(point.x) / double(m_szColorPicker.cx);
			m_clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				m_fCurrLuminance,
				m_fCurrSaturation);

			m_ptMousePos = point;
			m_bBmpValidSlider = FALSE;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::hsl_basic_hue_data

void CTColorCtrl::hsl_basic_hue_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);
	m_bBmpValidPicker = FALSE;
}

void CTColorCtrl::hsl_basic_hue_data::OnSetHLS( double hue,
											    double luminance,
												double saturation)
{
	any_picker_data::OnSetHLS(
		hue,
		luminance,
		saturation);

	m_bBmpValidPicker = FALSE;
}

void CTColorCtrl::hsl_basic_hue_data::GenerateSlider( CDC &dc,
													  CPalette *pPalette)
{
	if( !m_bBmpValidSlider && m_dcSlider.GetSafeHdc() )
		m_dcSlider.DeleteDC();

	if(!m_dcSlider.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorSlider.cx,
			m_szColorSlider.cy);

		m_dcSlider.CreateCompatibleDC(&dc);
		m_dcSlider.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcSlider.SelectPalette( pPalette, FALSE);
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = 0; i < m_szColorSlider.cx; i += nStep)
		{
			double fHue = double(i) / double(m_szColorSlider.cx);
			COLORREF clr = CTPaintManager::HLStoRGB(
				fHue,
				DEFAULT_LUMINANCE,
				1.0f);

			if( nBPP <= 8 )
			{
				CBrush brush(clr);

				m_dcSlider.FillRect(
					CRect(
					CPoint( i, 0),
					CSize(
					nStep,
					m_szColorSlider.cy)),
					&brush);
			}
			else
			{
				m_dcSlider.FillSolidRect(
					i, 0, 1,
					m_szColorSlider.cy,
					clr);
			}
		}

		if(pOldPalette)
			m_dcSlider.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidSlider = TRUE;
	}
}

void CTColorCtrl::hsl_basic_hue_data::GeneratePicker( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidPicker && m_dcPicker.GetSafeHdc() )
		m_dcPicker.DeleteDC();

	if(!m_dcPicker.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorPicker.cx,
			m_szColorPicker.cy);

		m_dcPicker.CreateCompatibleDC(&dc);
		m_dcPicker.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcPicker.SelectPalette( pPalette, FALSE);
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i = 0; i < m_szColorPicker.cx; i += nStep)
		{
			for( int j = 0; j < m_szColorPicker.cy; j += nStep)
			{
				COLORREF clr = CTPaintManager::HLStoRGB(
					m_fCurrHue,
					double(m_szColorPicker.cy - j - 1) / double(m_szColorPicker.cy),
					double(i) / double(m_szColorPicker.cx));

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcPicker.FillRect(
						CRect(
						CPoint(
						i - nStep / 2,
						j - nStep / 2),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcPicker.SetPixelV( i, j, clr);
			}
		}

		if(pOldPalette)
			m_dcPicker.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidPicker = TRUE;
	}
}

void CTColorCtrl::hsl_basic_hue_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	GeneratePicker( dc, pPalette);
	GenerateSlider( dc, pPalette);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_szColorPicker.cx,
		m_szColorPicker.cy,
		&m_dcPicker,
		0, 0,
		SRCCOPY);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap * 2 + m_szColorPicker.cy,
		m_szColorSlider.cx,
		m_szTotal.cy,
		&m_dcSlider,
		0, 0,
		SRCCOPY);

	CPoint ptValidLS;
	ptValidLS.x = INT(m_fCurrSaturation * double(m_szColorPicker.cx));
	ptValidLS.y = m_szColorPicker.cy - INT(m_fCurrLuminance * double(m_szColorPicker.cy));

	SIZE size = { __SEL_BOX_DX, __SEL_BOX_DY };
	CPoint ptDrawCalc = ptValidLS + CPoint( m_nAreaGap, m_nAreaGap);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	size.cx = __SEL_BOX_DX;
	size.cy = m_szColorSlider.cy;

	CPoint point( INT(m_fCurrHue * m_szColorSlider.cx), m_szColorPicker.cy + m_szColorSlider.cy / 2);
	ptDrawCalc = point + CPoint( m_nAreaGap, m_nAreaGap * 2);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::hsl_basic_hue_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_bBmpValidSlider && (nFlags&MK_LBUTTON) )
	{
		if( m_nMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap * 2;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorSlider.cx )
				point.x = m_szColorSlider.cx;

			m_fCurrHue = ((double) point.x) / ((double) m_szColorSlider.cx);
			m_clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				m_fCurrLuminance,
				m_fCurrSaturation);

			m_bBmpValidPicker = FALSE;
		}
		else if( m_nMoveMode == _MM_PICKER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorPicker.cx )
				point.x = m_szColorPicker.cx;

			if( point.y > m_szColorPicker.cy )
				point.y = m_szColorPicker.cy;

			if(nFlags&MK_CONTROL)
				point.x = m_ptMousePos.x;

			if(nFlags&MK_SHIFT)
				point.y = m_ptMousePos.y;

			m_fCurrLuminance = double(m_szColorPicker.cy - point.y) / double(m_szColorPicker.cy);
			m_fCurrSaturation = double(point.x) / double(m_szColorPicker.cx);

			m_clr = CTPaintManager::HLStoRGB(
				m_fCurrHue,
				m_fCurrLuminance,
				m_fCurrSaturation);
			m_ptMousePos = point;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::xyz_plain_data

void CTColorCtrl::xyz_plain_data::OnSetRGB( COLORREF clr)
{
	any_picker_data::OnSetRGB(clr);
	m_bBmpValidPicker = FALSE;
}

void CTColorCtrl::xyz_plain_data::OnSetHLS( double hue,
										    double luminance,
											double saturation)
{
	any_picker_data::OnSetHLS( hue, luminance, saturation);
	m_bBmpValidPicker = FALSE;
}

COLORREF CTColorCtrl::xyz_plain_data::CalcColorPicker( int nX, int nY)
{
	nX = (nX * 255) / m_szColorPicker.cx;
	nY = (nY * 255) / m_szColorPicker.cy;

	if( nX > 255 )
		nX = 255;

	if( nY > 255 )
		nY = 255;

	switch(m_nXYZmode)
	{
	case MXYZ_R	: return RGB( GetRValue(m_clr), nX, nY);
	case MXYZ_G	: return RGB( nX, GetGValue(m_clr), nY);
	case MXYZ_B	: return RGB( nX, nY, GetBValue(m_clr));
	case MXYZ_C	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			return CTPaintManager::CMYtoRGB(RGB(
				GetRValue(clr),
				nX,
				nY));
		}

		break;

	case MXYZ_M	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			return CTPaintManager::CMYtoRGB(RGB(
				nX,
				GetGValue(clr),
				nY));
		}

		break;

	case MXYZ_Y	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			return CTPaintManager::CMYtoRGB(RGB(
				nX,
				nY,
				GetBValue(clr)));
		}

		break;
	}

	return RGB( 0, 0, 0);
}

COLORREF CTColorCtrl::xyz_plain_data::CalcColorSlider( int nZ)
{
	nZ = (nZ * 255) / m_szColorSlider.cx;
	if( nZ > 255 )
		nZ = 255;

	switch(m_nXYZmode)
	{
	case MXYZ_R	: return RGB( nZ, 0, 0);
	case MXYZ_G	: return RGB( 0, nZ, 0);
	case MXYZ_B	: return RGB( 0, 0, nZ);
	case MXYZ_C	: return CTPaintManager::CMYtoRGB(RGB( nZ, 0, 0));
	case MXYZ_M	: return CTPaintManager::CMYtoRGB(RGB( 0, nZ, 0));
	case MXYZ_Y	: return CTPaintManager::CMYtoRGB(RGB( 0, 0, nZ));
	}

	return RGB( 0, 0, 0);
}

void CTColorCtrl::xyz_plain_data::GenerateSlider( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidSlider && m_dcSlider.GetSafeHdc() )
		m_dcSlider.DeleteDC();

	if(!m_dcSlider.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorSlider.cx,
			m_szColorSlider.cy);

		m_dcSlider.CreateCompatibleDC(&dc);
		m_dcSlider.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcSlider.SelectPalette( pPalette, FALSE);
			m_dcSlider.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i=0; i<m_szColorSlider.cx; i+=nStep)
		{
			COLORREF clr = CalcColorSlider(i);

			if( nBPP <= 8 )
			{
				CBrush brush(clr);

				m_dcSlider.FillRect(
					CRect(
					CPoint( i, 0),
					CSize( nStep, m_szColorSlider.cy)),
					&brush);
			}
			else
			{
				m_dcSlider.FillSolidRect(
					i, 0, 1,
					m_szColorSlider.cy,
					clr);
			}
		}

		if(pOldPalette)
			m_dcSlider.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidSlider = TRUE;
	}
}

void CTColorCtrl::xyz_plain_data::GeneratePicker( CDC &dc, CPalette *pPalette)
{
	if( !m_bBmpValidPicker && m_dcPicker.GetSafeHdc() )
		m_dcPicker.DeleteDC();

	if(!m_dcPicker.GetSafeHdc())
	{
		CBitmap bmp;

		_CreateCompatibleBitmapImpl(
			bmp, dc,
			m_szColorPicker.cx,
			m_szColorPicker.cy);

		m_dcPicker.CreateCompatibleDC(&dc);
		m_dcPicker.SelectObject(&bmp);

		CPalette *pOldPalette = NULL;
		if(pPalette)
		{
			pOldPalette = m_dcPicker.SelectPalette( pPalette, FALSE);
			m_dcPicker.RealizePalette();
		}

		int nBPP = g_PaintManager->GetBPP();
		int nStep = ( nBPP <= 8 ) ? 4 : 1;

		for( int i=0; i<m_szColorPicker.cx; i+=nStep)
		{
			for( int j=0; j<m_szColorPicker.cy; j+=nStep)
			{
				COLORREF clr = CalcColorPicker( i, j);

				if( nBPP <= 8 )
				{
					CBrush brush(clr);

					m_dcPicker.FillRect(
						CRect(
						CPoint( i, j),
						CSize( nStep, nStep)),
						&brush);
				}
				else
					m_dcPicker.SetPixelV( i, j, clr);
			}
		}

		if(pOldPalette)
			m_dcPicker.SelectPalette( pOldPalette, FALSE);

		m_bBmpValidPicker = TRUE;
	}
}

void CTColorCtrl::xyz_plain_data::RecalcByZ( int nZ)
{
	int nX;
	int nY;

	switch(m_nXYZmode)
	{
	case MXYZ_R	:
		{
			nX = GetGValue(m_clr);
			nY = GetBValue(m_clr);
			m_clr = RGB( nZ, nX, nY);
		}

		break;

	case MXYZ_G	:
		{
			nX = GetRValue(m_clr);
			nY = GetBValue(m_clr);
			m_clr = RGB( nX, nZ, nY);
		}

		break;

	case MXYZ_B	:
		{
			nX = GetRValue(m_clr);
			nY = GetGValue(m_clr);
			m_clr = RGB( nX, nY, nZ);
		}

		break;

	case MXYZ_C	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			nX = GetGValue(clr);
			nY = GetBValue(clr);
			m_clr = CTPaintManager::CMYtoRGB(RGB( nZ, nX, nY));
		}

		break;

	case MXYZ_M	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			nX = GetRValue(clr);
			nY = GetBValue(clr);
			m_clr = CTPaintManager::CMYtoRGB(RGB( nX, nZ, nY));
		}

		break;

	case MXYZ_Y	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);

			nX = GetRValue(clr);
			nY = GetGValue(clr);
			m_clr = CTPaintManager::CMYtoRGB(RGB( nX, nY, nZ));
		}

		break;
	}

	CTPaintManager::RGBtoHSL(
		m_clr,
		&m_fCurrHue,
		&m_fCurrLuminance,
		&m_fCurrSaturation);
}

void CTColorCtrl::xyz_plain_data::CalcPoints( int &nX,
											  int &nY,
											  int &nZ,
											  BOOL bDcScale)
{
	nX = 0;
	nY = 0;
	nZ = 0;

	switch(m_nXYZmode)
	{
	case MXYZ_R	:
		{
			nX = GetGValue(m_clr);
			nY = GetBValue(m_clr);
			nZ = GetRValue(m_clr);
		}

		break;

	case MXYZ_G	:
		{
			nX = GetRValue(m_clr);
			nY = GetBValue(m_clr);
			nZ = GetGValue(m_clr);
		}

		break;

	case MXYZ_B	:
		{
			nX = GetRValue(m_clr);
			nY = GetGValue(m_clr);
			nZ = GetBValue(m_clr);
		}

		break;

	case MXYZ_C	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			nX = GetGValue(clr);
			nY = GetBValue(clr);
			nZ = GetRValue(clr);
		}

		break;

	case MXYZ_M	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			nX = GetRValue(clr);
			nY = GetBValue(clr);
			nZ = GetGValue(clr);
		}

		break;

	case MXYZ_Y	:
		{
			COLORREF clr = CTPaintManager::RGBtoCMY(m_clr);
			nX = GetRValue(clr);
			nY = GetGValue(clr);
			nZ = GetBValue(clr);
		}

		break;
	}

	if(bDcScale)
	{
		nX = INT((double(nX) / 255.0f) * m_szColorPicker.cx);
		nY = INT((double(nY) / 255.0f) * m_szColorPicker.cy);
		nZ = INT((double(nZ) / 255.0f) * m_szColorSlider.cx);
	}
}

void CTColorCtrl::xyz_plain_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	GeneratePicker( dc, pPalette);
	GenerateSlider( dc, pPalette);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap,
		m_szColorPicker.cx,
		m_szColorPicker.cy,
		&m_dcPicker,
		0, 0,
		SRCCOPY);

	dc.BitBlt(
		m_nAreaGap,
		m_nAreaGap * 2 + m_szColorPicker.cy,
		m_szColorSlider.cx,
		m_szTotal.cy,
		&m_dcSlider,
		0, 0,
		SRCCOPY);

	int nX;
	int nY;
	int nZ;

	CalcPoints( nX, nY, nZ, TRUE);
	SIZE size = { __SEL_BOX_DX, __SEL_BOX_DY};
	CPoint ptDrawCalc = CPoint( nX, nY) + CPoint( m_nAreaGap, m_nAreaGap);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	size.cx = __SEL_BOX_DX;
	size.cy = m_szColorSlider.cy;

	CPoint point( nZ, m_szColorPicker.cy + m_szColorSlider.cy / 2);
	ptDrawCalc = point + CPoint( m_nAreaGap, m_nAreaGap * 2);

	DrawCrossAt(
		ptDrawCalc,
		dc, size);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::xyz_plain_data::OnMouseMove( UINT nFlags, CPoint point)
{
	if( m_bBmpValidSlider && (nFlags&MK_LBUTTON) )
	{
		if( m_nMoveMode == _MM_SLIDER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap * 2;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorSlider.cx )
				point.x = m_szColorSlider.cx;

			int nZ = (point.x * 255) / m_szColorSlider.cx;
			if( nZ > 255 )
				nZ = 255;

			RecalcByZ(nZ);
			m_bBmpValidPicker = FALSE;
		}
		else if( m_nMoveMode == _MM_PICKER )
		{
			point.x -= m_nAreaGap;
			if( point.x < 0 )
				point.x = 0;

			point.y -= m_nAreaGap;
			if( point.y < 0 )
				point.y = 0;

			if( point.x > m_szColorPicker.cx )
				point.x = m_szColorPicker.cx;

			if( point.y > m_szColorPicker.cy )
				point.y = m_szColorPicker.cy;

			if(nFlags&MK_CONTROL)
				point.x = m_ptMousePos.x;

			if(nFlags&MK_SHIFT)
				point.y = m_ptMousePos.y;

			m_clr = CalcColorPicker( point.x, point.y);
			CTPaintManager::RGBtoHSL(
				m_clr,
				&m_fCurrHue,
				&m_fCurrLuminance,
				&m_fCurrSaturation);

			m_ptMousePos = point;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl::diff_data

void CTColorCtrl::diff_data::Generate( BOOL bOnlySizeChanged)
{
	if(!bOnlySizeChanged)
	{
		m_vCells[0].m_clr = m_clr;
		m_vCells[0].m_bAtFirstType = FALSE;
		m_vCells[0].m_nBorderSize = 0;
		m_vCells[1].m_clr = m_clrOld;
		m_vCells[1].m_bAtFirstType = FALSE;
		m_vCells[1].m_nBorderSize = 0;
	}

	m_vCells[0].m_rcPosition = CRect(
		CPoint( 0, 0),
		CSize( m_szTotal.cx, m_szTotal.cy / 2));

	m_vCells[1].m_rcPosition = CRect(
		CPoint( 0, m_szTotal.cy / 2),
		CSize( m_szTotal.cx, m_szTotal.cy / 2));
}

void CTColorCtrl::diff_data::OnInit( CDC &dc,
									 CPalette *pPalette,
									 CSize szTotal,
									 BOOL bOnlySizeChanged)
{
	any_picker_data::OnInit(
		dc,
		pPalette,
		szTotal,
		bOnlySizeChanged);

	Generate(bOnlySizeChanged);
}

void CTColorCtrl::diff_data::OnDraw( CDC &dc, CPalette *pPalette)
{
	CPalette *pOldPalette = NULL;

	if(pPalette)
	{
		pOldPalette = dc.SelectPalette( pPalette, FALSE);
		dc.RealizePalette();
	}

	for( int i=0; i<m_vCells.GetSize(); i++)
		m_vCells[i].OnDraw( dc, pPalette, FALSE);

	if(pOldPalette)
		dc.SelectPalette( pOldPalette, FALSE);
}

void CTColorCtrl::diff_data::OnSetRGB( COLORREF clr)
{
	grayscale_mixer_data::OnSetRGB(clr);
	Generate(FALSE);
}

void CTColorCtrl::diff_data::OnSetHLS( double hue,
									   double luminance,
									   double saturation)
{
	grayscale_mixer_data::OnSetHLS(
		hue,
		luminance,
		saturation);

	Generate(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl

CTColorCtrl::CTColorCtrl( e_mode nMode) : m_xyz_r(xyz_plain_data::MXYZ_R),
	m_xyz_g(xyz_plain_data::MXYZ_G),
	m_xyz_b(xyz_plain_data::MXYZ_B),
	m_xyz_c(xyz_plain_data::MXYZ_C),
	m_xyz_m(xyz_plain_data::MXYZ_M),
	m_xyz_y(xyz_plain_data::MXYZ_Y)
{
	m_nMode = nMode;
	m_bMouseTracking = FALSE;

	m_algorithms[0] = &m_hexagon_data;
	m_algorithms[1] = &m_hsl_panel_data;
	m_algorithms[2] = &m_hsl_basic_hue_data;
	m_algorithms[3] = &m_hsl_roller_data;
	m_algorithms[4] = &m_rgb_cube_data;
	m_algorithms[5] = &m_rgb_mixer_data;
	m_algorithms[6] = &m_grayscale_mixer_data;
	m_algorithms[7] = &m_cmy_cube_data;
	m_algorithms[8] = &m_cmy_mixer_data;
	m_algorithms[9] = &m_xyz_r;
	m_algorithms[10] = &m_xyz_g;
	m_algorithms[11] = &m_xyz_b;
	m_algorithms[12] = &m_xyz_c;
	m_algorithms[13] = &m_xyz_m;
	m_algorithms[14] = &m_xyz_y;
	m_algorithms[15] = &m_diff_data;
}

CTColorCtrl::~CTColorCtrl()
{
	_Cleanup();
}

void CTColorCtrl::_Cleanup()
{
	for( int i=0; i<=MODE_VAL_MAX; i++)
		m_algorithms[i]->OnDone();
}

void CTColorCtrl::_Init( BOOL bOnlySizeChanged)
{
	if(!bOnlySizeChanged)
		_Cleanup();

	if(!GetSafeHwnd())
		return;

	CRect rcClient;
	GetClientRect(&rcClient);

	CClientDC dcClient(this);
	m_algorithms[m_nMode]->OnInit(
		dcClient,
		&g_PaintManager->m_PaletteWide,
		rcClient.Size(),
		bOnlySizeChanged);

	m_algorithms[m_nMode]->OnSetRGB(m_selClrs.m_clrNew);
	m_algorithms[m_nMode]->OnSetOldRGB(m_selClrs.m_clrCurrent);
}

BEGIN_MESSAGE_MAP( CTColorCtrl, CWnd)
	//{{AFX_MSG_MAP(CTColorCtrl)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_CAPTURECHANGED()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTColorCtrl message handlers

void CTColorCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	_MouseTracking(FALSE);
	CWnd::OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTColorCtrl::OnLButtonDown( UINT nFlags, CPoint point)
{
	_MouseTracking(TRUE);

	if( m_algorithms[m_nMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_algorithms[m_nMode]->OnSetRGB(m_selClrs.m_clrNew);
		Invalidate();
		UpdateWindow();
	}

	m_algorithms[m_nMode]->OnLButtonDown( nFlags, point);
	OnMouseMove( 0, point);

	if( m_algorithms[m_nMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrPrev = m_selClrs.m_clrNew;
		m_selClrs.m_clrNew = m_algorithms[m_nMode]->m_clr;

		NotifyColorChanged();
		Invalidate();
		UpdateWindow();
	}
}

void CTColorCtrl::OnLButtonUp( UINT nFlags, CPoint point)
{
	_MouseTracking(FALSE);
	m_algorithms[m_nMode]->OnLButtonUp( nFlags, point);
}

void CTColorCtrl::NotifyColorChanged()
{
	GetParent()->SendMessage(
		m_nMsgColorChanged,
		WPARAM(GetSafeHwnd()),
		LPARAM(&m_selClrs));
}

void CTColorCtrl::OnMouseMove( UINT nFlags, CPoint point)
{
	if(!m_bMouseTracking)
		return;

	m_algorithms[m_nMode]->OnMouseMove( nFlags, point);
	if( m_algorithms[m_nMode]->m_clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrPrev = m_selClrs.m_clrNew;
		m_selClrs.m_clrNew = m_algorithms[m_nMode]->m_clr;

		NotifyColorChanged();
		Invalidate();
		UpdateWindow();
	}
}

void CTColorCtrl::OnCancelMode()
{
	CWnd::OnCancelMode();
	_MouseTracking(FALSE);
}

void CTColorCtrl::OnKillFocus( CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	_MouseTracking(FALSE);
}

void CTColorCtrl::OnCaptureChanged( CWnd *pWnd)
{
	CWnd::OnCaptureChanged(pWnd);
}

BOOL CTColorCtrl::OnQueryNewPalette()
{
	Invalidate();
	return CWnd::OnQueryNewPalette();
}

void CTColorCtrl::OnPaletteChanged( CWnd* pFocusWnd)
{
	CWnd::OnPaletteChanged(pFocusWnd);
	Invalidate();
}

BOOL CTColorCtrl::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTColorCtrl::OnPaint()
{
	CPaintDC dcPaint(this);
	CRect rcClient;

	GetClientRect(&rcClient);
	if( rcClient.IsRectEmpty() || !dcPaint.RectVisible(&rcClient) )
		return;

	CPalette *pOldPalette1 = NULL;
	CPalette *pOldPalette2 = NULL;

	if(dcPaint.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)
	{
		pOldPalette1 = dcPaint.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		dcPaint.RealizePalette();
	}

	CTMemoryDC dc( &dcPaint, &rcClient);
	dc.FillSolidRect(
		&rcClient,
		g_PaintManager->GetColor(CTPaintManager::CLR_3DFACE_OUT));

	if(dc.GetDeviceCaps(RASTERCAPS)&RC_PALETTE)
	{
		pOldPalette2 = dc.SelectPalette( &g_PaintManager->m_PaletteWide, FALSE);
		dc.RealizePalette();
	}

	m_algorithms[m_nMode]->OnDraw(
		dc,
		pOldPalette2 ? &g_PaintManager->m_PaletteWide : NULL);

	if(pOldPalette2)
		dc.SelectPalette( pOldPalette2, FALSE);

	if(pOldPalette1)
		dcPaint.SelectPalette( pOldPalette1, FALSE);
}

void CTColorCtrl::_MouseTracking( BOOL bStart)
{
	if( m_bMouseTracking && bStart )
		return;

	if( !m_bMouseTracking && !bStart )
		return;

	m_bMouseTracking = bStart;
	if(m_bMouseTracking)
		SetCapture();
	else if( GetCapture() == this )
		ReleaseCapture();
}

void CTColorCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	_Init(FALSE);
}

void CTColorCtrl::OnSize( UINT nType, int cx, int cy)
{
	CWnd::OnSize( nType, cx, cy);
	_Init(TRUE);

	Invalidate();
	UpdateWindow();
}

void CTColorCtrl::SetupColor( COLORREF clr, BOOL bNotifyChanged)
{
	if( clr != m_selClrs.m_clrNew )
	{
		m_selClrs.m_clrNew = clr;

		if(bNotifyChanged)
			NotifyColorChanged();
	}

	m_algorithms[m_nMode]->OnSetRGB(m_selClrs.m_clrNew);
	Invalidate();
	UpdateWindow();
}
