// TMapView.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "TMapView.h"
#include "MainFrm.h"
#include "TZoomDlg.h"


// CTMapView

IMPLEMENT_DYNCREATE(CTMapView, CScrollView)

CTMapView::CTMapView()
{
	m_pTUNIT = NULL;
	m_hTUNIT = NULL;

	m_bFinding = FALSE;
	m_bTACT = FALSE;
	m_bDRAW = FALSE;

	m_dwMonKind = 0;
	m_dwMapID = 0;
	m_dwLineID = 0;

	m_wPointID = 0;
	m_wUnitX = 0;
	m_wUnitZ = 0;

	m_fSCALE = 1.0f;
	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;

	m_fPosX = 0.0f;
	m_fPosZ = 0.0f;

	ClearTestPath(TRUE);
}

CTMapView::~CTMapView()
{
}

BEGIN_MESSAGE_MAP(CTMapView, CScrollView)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CTMapView 그리기입니다.

void CTMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	OnUpdateSCALE();
}

void CTMapView::OnDraw( CDC *pDC)
{
	CBitmap vBACK;
	CDC dcMEM;

	CPoint vPOS = GetScrollPosition();
	CRect vRECT;

	GetClientRect(&vRECT);
	vRECT.OffsetRect(
		-vRECT.left,
		-vRECT.top);

	dcMEM.CreateCompatibleDC(pDC);
	vBACK.CreateCompatibleBitmap(
		pDC,
		vRECT.right,
		vRECT.bottom);
	CBitmap *pTBMPOLD = dcMEM.SelectObject(&vBACK);

	dcMEM.FillSolidRect(
		&vRECT,
		GetSysColor(COLOR_WINDOW));

	if(m_bDRAW)
		DrawTUNIT(&dcMEM);

	pDC->BitBlt(
		vPOS.x,
		vPOS.y,
		vRECT.right,
		vRECT.bottom,
		&dcMEM,
		0, 0,
		SRCCOPY);

	dcMEM.SelectObject(pTBMPOLD);
	vBACK.DeleteObject();
	dcMEM.DeleteDC();
}

void CTMapView::DrawTUNIT( CDC *pDC)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( m_hTUNIT && m_pTUNIT &&
		m_pTUNIT->m_vTBMPINFO.m_pTBUF &&
		m_fSizeX != 0.0f &&
		m_fSizeZ != 0.0f &&
		m_fSCALE != 0.0f )
	{
		CPoint vPOS = GetScrollPosition();
		CRect vRECT;
		CDC dcBMP;

		dcBMP.CreateCompatibleDC(pDC);
		GetClientRect(&vRECT);
		vRECT.OffsetRect(
			-vRECT.left,
			-vRECT.top);

		HBITMAP hBMPOLD = (HBITMAP) SelectObject( dcBMP.GetSafeHdc(), m_hTUNIT);
		StretchBlt(
			pDC->GetSafeHdc(),
			0, 0,
			vRECT.right,
			vRECT.bottom,
			dcBMP.GetSafeHdc(),
			INT(vPOS.x / m_fSCALE),
			INT(vPOS.y / m_fSCALE),
			INT(m_pTUNIT->m_vTBMPINFO.m_pTHEADER->biWidth * vRECT.right / (m_fSizeX * m_fSCALE)),
			INT(m_pTUNIT->m_vTBMPINFO.m_pTHEADER->biHeight * vRECT.bottom / (m_fSizeZ * m_fSCALE)),
			SRCCOPY);

		SelectObject( dcBMP.GetSafeHdc(), hBMPOLD);
		dcBMP.DeleteDC();
	}

	if(pDOC)
	{
		if(m_dwMonKind)
		{
			LPMAPTMDATA pMAPTMDATA = pDOC->m_vTGAMEDB.FindMAPTMDATA(
				&pDOC->m_vTGAMEDB.m_mapTKINDINDEX,
				m_dwMonKind);

			if(pMAPTMDATA)
			{
				LPVTMDATA pVTMDATA = pDOC->m_vTGAMEDB.FindVTMDATA(
					pMAPTMDATA,
					m_dwMapID);
				int nCount = pVTMDATA ? INT(pVTMDATA->size()) : 0;

				for( int i=0; i<nCount; i++)
				{
					FLOAT fPosX = (*pVTMDATA)[i]->m_fPosX - FLOAT(m_wUnitX) * m_fSizeX;
					FLOAT fPosZ = (*pVTMDATA)[i]->m_fPosZ - FLOAT(m_wUnitZ) * m_fSizeZ;

					DrawTPOINT(
						pDC,
						fPosX,
						fPosZ,
						TPOINTSIZE_MON,
						TPOINTCLR_MON);
				}
			}
		}

		if(!pDOC->m_bEditPATH)
		{
			DrawTPOINT(
				pDC,
				m_fPosX,
				m_fPosZ,
				TPOINTSIZE_TARGET,
				TPOINTCLR_TARGET);
		}
		else if(m_pTUNIT)
		{
			MAPT2DPOINT::iterator itT2DPOINT;
			MAPDWORD::iterator itTLINE;

			T2DPOINT lPoint = 0;
			WORD wPointID = 0;

			switch(pDOC->m_nPathMode)
			{
			case TEDITPATH_ADD_BLOCK	:
			case TEDITPATH_ADD_PATH		: wPointID = LOWORD(m_dwLineID); break;
			case TEDITPATH_MOVE_POINT	: wPointID = m_wPointID; break;
			}

			if( pDOC->m_nPathMode != TEDITPATH_TEST_PATH )
				ClearTestPath(TRUE);

			itT2DPOINT = m_pTUNIT->m_mapTPOINT.find(wPointID);
			if( itT2DPOINT == m_pTUNIT->m_mapTPOINT.end() )
			{
				m_dwLineID = 0;
				m_wPointID = 0;
				wPointID = 0;
			}
			else
				lPoint = (*itT2DPOINT).second;

			for( itTLINE = m_pTUNIT->m_mapTLINE.begin(); itTLINE != m_pTUNIT->m_mapTLINE.end(); itTLINE++)
			{
				BYTE bFIND = !m_bFinding && m_mapTPATHLINE.find((*itTLINE).first) != m_mapTPATHLINE.end() ? TRUE : FALSE;

				DrawTLINE(
					pDC,
					m_pTUNIT,
					(*itTLINE).first,
					bFIND ? TLINESIZE_FIND : TLINESIZE_PATH,
					bFIND ? TLINECLR_FIND : (*itTLINE).second == TLINE_PATH ? TLINECLR_PATH : TLINECLR_BLOCK);
			}

			for( int i=0; i<2; i++)
				if( !m_bFinding && m_vPoint[i] && m_vPointID[i] )
				{
					itT2DPOINT = m_pTUNIT->m_mapTPOINT.find(m_vPointID[i]);

					if( itT2DPOINT != m_pTUNIT->m_mapTPOINT.end() )
					{
						DrawTLINE(
							pDC,
							m_vPoint[i],
							(*itT2DPOINT).second,
							TLINESIZE_FIND,
							TLINECLR_FIND);
					}
				}

			if( !m_bFinding &&
				!m_vPointID[0] &&
				!m_vPointID[1] &&
				m_vPoint[0] &&
				m_vPoint[1] &&
				CanMove( m_pTUNIT, m_vPoint[0], m_vPoint[1]) )
			{
				DrawTLINE(
					pDC,
					m_vPoint[0],
					m_vPoint[1],
					TLINESIZE_FIND,
					TLINECLR_FIND);
			}

			if(m_dwLineID)
			{
				CPoint point;

				GetCursorPos(&point);
				ScreenToClient(&point);
				point += GetScrollPosition();

				FLOAT fPosX = (FLOAT) point.x;
				FLOAT fPosZ = (FLOAT) point.y;

				GetUnitCoord(
					fPosX,
					fPosZ);

				DrawTLINE(
					pDC,
					lPoint,
					MAKET2DPOINT(
					fPosX,
					fPosZ),
					TLINESIZE_DRAW,
					pDOC->m_nPathMode == TEDITPATH_ADD_PATH ? TLINECLR_DPATH : TLINECLR_DBLOCK);
			}

			for( itT2DPOINT = m_pTUNIT->m_mapTPOINT.begin(); itT2DPOINT != m_pTUNIT->m_mapTPOINT.end(); itT2DPOINT++)
				if( wPointID != (*itT2DPOINT).first )
				{
					BYTE bFIND = m_mapTPATHPOINT.find((*itT2DPOINT).first) != m_mapTPATHPOINT.end() ? TRUE : FALSE;

					DrawTPOINT(
						pDC,
						GET2D_X((*itT2DPOINT).second),
						GET2D_Y((*itT2DPOINT).second),
						bFIND ? TPOINTSIZE_FIND : TPOINTSIZE_PATH,
						bFIND ? TPOINTCLR_FIND : TPOINTCLR_PATH);
				}

			if(wPointID)
			{
				DrawTPOINT(
					pDC,
					GET2D_X(lPoint),
					GET2D_Y(lPoint),
					TPOINTSIZE_PSEL,
					TPOINTCLR_PSEL);
			}

			static int vTPOINTCLR[2] = {
				TPOINTCLR_PSTART,
				TPOINTCLR_PEND};

			for( int i=0; i<2; i++)
				if(m_vPoint[i])
				{
					DrawTPOINT(
						pDC,
						GET2D_X(m_vPoint[i]),
						GET2D_Y(m_vPoint[i]),
						TPOINTSIZE_PTEST,
						vTPOINTCLR[i]);
				}
		}
	}
}

void CTMapView::DrawTPOINT( CDC *pDC,
						    FLOAT fPosX,
							FLOAT fPosZ,
							int nRadius,
							DWORD dwClr)
{
	CPoint vPOS = GetClientCoord(
		fPosX,
		fPosZ) - GetScrollPosition();
	CBrush vBRUSH(dwClr);

	CBrush *pTOLD = pDC->SelectObject(&vBRUSH);
	pDC->Ellipse(
		vPOS.x - nRadius,
		vPOS.y - nRadius,
		vPOS.x + nRadius,
		vPOS.y + nRadius);
	pDC->SelectObject(pTOLD);
}

void CTMapView::DrawTLINE( CDC *pDC,
						   T2DPOINT lStart,
						   T2DPOINT lEnd,
						   int nWidth,
						   DWORD dwClr)
{
	CPoint vTSTART = GetClientCoord(
		GET2D_X(lStart),
		GET2D_Y(lStart)) - GetScrollPosition();

	CPoint vTEND = GetClientCoord(
		GET2D_X(lEnd),
		GET2D_Y(lEnd)) - GetScrollPosition();

	CPen vOUT( PS_SOLID, nWidth + 2, RGB( 0, 0, 0));
	CPen vPEN( PS_SOLID, nWidth, dwClr);
	CPen *pTOLD = pDC->SelectObject(&vOUT);

	pDC->MoveTo(vTSTART);
	pDC->LineTo(vTEND);

	pDC->SelectObject(&vPEN);
	pDC->LineTo(vTSTART);

	pDC->SelectObject(pTOLD);
}

void CTMapView::DrawTLINE( CDC *pDC,
						   LPTMAPIMG pTUNIT,
						   DWORD dwLineID,
						   int nWidth,
						   DWORD dwClr)
{
	if(!pTUNIT)
		return;

	MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(LOWORD(dwLineID));
	T2DPOINT lStart = 0;
	T2DPOINT lEnd = 0;

	if( finder != pTUNIT->m_mapTPOINT.end() )
		lStart = (*finder).second;
	else
		return;

	finder = pTUNIT->m_mapTPOINT.find(HIWORD(dwLineID));
	if( finder != pTUNIT->m_mapTPOINT.end() )
		lEnd = (*finder).second;
	else
		return;

	DrawTLINE(
		pDC,
		lStart,
		lEnd,
		nWidth,
		dwClr);
}

CPoint CTMapView::GetClientCoord( FLOAT fPosX,
								  FLOAT fPosZ)
{
	fPosZ = m_fSizeZ - fPosZ;
	fPosX *= m_fSCALE;
	fPosZ *= m_fSCALE;

	return CPoint(
		INT(fPosX),
		INT(fPosZ));
}

BYTE CTMapView::GetUnitCoord( FLOAT& fPosX,
							  FLOAT& fPosZ)
{
	if( m_fSCALE == 0.0f )
		return FALSE;

	fPosX /= m_fSCALE;
	fPosZ /= m_fSCALE;
	fPosZ = m_fSizeZ - fPosZ;

	return TRUE;
}

BYTE CTMapView::GetWorldCoord( FLOAT& fPosX,
							   FLOAT& fPosZ)
{
	if(!GetUnitCoord( fPosX, fPosZ))
		return FALSE;

	fPosX += FLOAT(m_wUnitX) * m_fSizeX;
	fPosZ += FLOAT(m_wUnitZ) * m_fSizeZ;

	return TRUE;
}

WORD CTMapView::PickPathPoint( LPTMAPIMG pTUNIT,
							   CPoint point,
							   WORD wCurrentID,
							   int nCurrentR,
							   int nRadius)
{
	MAPT2DPOINT::reverse_iterator itT2DPOINT;

	for( itT2DPOINT = pTUNIT->m_mapTPOINT.rbegin(); itT2DPOINT != pTUNIT->m_mapTPOINT.rend(); itT2DPOINT++)
		if( (*itT2DPOINT).first != wCurrentID )
		{
			CPoint vPOS = GetClientCoord(
				GET2D_X((*itT2DPOINT).second),
				GET2D_Y((*itT2DPOINT).second)) - GetScrollPosition() - point;

			if( vPOS.x * vPOS.x + vPOS.y * vPOS.y < nRadius * nRadius )
				return (*itT2DPOINT).first;
		}

	MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(wCurrentID);
	if( finder != pTUNIT->m_mapTPOINT.end() )
	{
		CPoint vPOS = GetClientCoord(
			GET2D_X((*finder).second),
			GET2D_Y((*finder).second)) - GetScrollPosition() - point;

		if( vPOS.x * vPOS.x + vPOS.y * vPOS.y < nCurrentR * nCurrentR )
			return wCurrentID;
	}

	return 0;
}

DWORD CTMapView::PickTLINE( LPTMAPIMG pTUNIT,
						    CPoint point,
							int nWidth)
{
	MAPDWORD::reverse_iterator itTLINE;
	point += GetScrollPosition();

	T2DPOINT lPoint = MAKET2DPOINT(
		point.x,
		point.y);

	for( itTLINE = pTUNIT->m_mapTLINE.rbegin(); itTLINE != pTUNIT->m_mapTLINE.rend(); itTLINE++)
	{
		MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(LOWORD((*itTLINE).first));

		if( finder != pTUNIT->m_mapTPOINT.end() )
		{
			T2DPOINT lStart = (*finder).second;

			finder = pTUNIT->m_mapTPOINT.find(HIWORD((*itTLINE).first));
			if( finder != pTUNIT->m_mapTPOINT.end() )
			{
				T2DPOINT lEnd = (*finder).second;

				point = GetClientCoord(
					GET2D_X(lStart),
					GET2D_Y(lStart));

				lStart = MAKET2DPOINT(
					point.x,
					point.y);

				point = GetClientCoord(
					GET2D_X(lEnd),
					GET2D_Y(lEnd));

				lEnd = MAKET2DPOINT(
					point.x,
					point.y);

				FLOAT fA = 0.0f;
				FLOAT fB = 0.0f;
				FLOAT fC = 0.0f;

				if( GetT2DLINE( lStart, lEnd, &fA, &fB, &fC) && fabs(GetT2DDotCoord( lPoint, fA, fB, fC)) < nWidth )
				{
					fC = fA;
					fA = -fB;
					fB = fC;
					fC = -fA * GET2D_X(lPoint) - fB * GET2D_Y(lPoint);

					if( GetT2DDotCoord( lStart, fA, fB, fC) * GetT2DDotCoord( lEnd, fA, fB, fC) < 0.0f )
						return (*itTLINE).first;
				}
			}
		}
	}

	return 0;
}

BYTE CTMapView::CanMove( LPTMAPIMG pTUNIT,
						 T2DPOINT lStart,
						 T2DPOINT lEnd)
{
	MAPDWORD::iterator itTLINE;
	FLOAT fA = 0.0f;
	FLOAT fB = 0.0f;
	FLOAT fC = 0.0f;

	if( !pTUNIT || lStart == lEnd || !GetT2DLINE( lStart, lEnd, &fA, &fB, &fC) )
		return TRUE;

	for( itTLINE = pTUNIT->m_mapTLINE.begin(); itTLINE != pTUNIT->m_mapTLINE.end(); itTLINE++)
		if( (*itTLINE).second == TLINE_BLOCK )
		{
			T2DPOINT lPoint[2] = { 0, 0};
			WORD vPointID[2] = {
				LOWORD((*itTLINE).first),
				HIWORD((*itTLINE).first)};

			for( int i=0; i<2; i++)
			{
				MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(vPointID[i]);

				if( finder != pTUNIT->m_mapTPOINT.end() )
					lPoint[i] = (*finder).second;
			}

			FLOAT fLA = 0.0f;
			FLOAT fLB = 0.0f;
			FLOAT fLC = 0.0f;

			if( lPoint[0] != 0 && lPoint[1] != 0 && GetT2DLINE( lPoint[0], lPoint[1], &fLA, &fLB, &fLC) &&
				GetT2DDotCoord( lPoint[0], fA, fB, fC) * GetT2DDotCoord( lPoint[1], fA, fB, fC) < 0.0f &&
				GetT2DDotCoord( lStart, fLA, fLB, fLC) * GetT2DDotCoord( lEnd, fLA, fLB, fLC) < 0.0f )
				return FALSE;
		}

	return TRUE;
}

BYTE CTMapView::CheckBreak( LPTMAPIMG pTUNIT,
						    T2DPOINT lPoint,
							WORD wLineS,
							WORD wLineE)
{
	WORD wLineID[2] = {
		wLineS,
		wLineE};
	T2DPOINT vPoint[2];

	FLOAT fA = 0.0f;
	FLOAT fB = 0.0f;
	FLOAT fC = 0.0f;

	for( int i=0; i<2; i++)
	{
		MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(wLineID[i]);

		if( finder == pTUNIT->m_mapTPOINT.end() )
			return FALSE;

		vPoint[i] = (*finder).second;
	}

	if(!GetT2DNORMALLINE( lPoint, vPoint[0], vPoint[1], &fA, &fB, &fC))
		return FALSE;

	if( GetT2DDotCoord( vPoint[0], fA, fB, fC) * GetT2DDotCoord( vPoint[1], fA, fB, fC) < 0.0f &&
		CanMove( pTUNIT, lPoint, vPoint[1]) )
		return TRUE;

	return FALSE;
}

BYTE CTMapView::GetT2DNORMALLINE( LPTMAPIMG pTUNIT,
								  T2DPOINT lPoint,
								  WORD wLineS,
								  WORD wLineE,
								  FLOAT *pA,
								  FLOAT *pB,
								  FLOAT *pC)
{
	WORD wLineID[2] = {
		wLineS,
		wLineE};
	T2DPOINT vPoint[2];

	for( int i=0; i<2; i++)
	{
		MAPT2DPOINT::iterator finder = pTUNIT->m_mapTPOINT.find(wLineID[i]);

		if( finder == pTUNIT->m_mapTPOINT.end() )
			return FALSE;

		vPoint[i] = (*finder).second;
	}

	return GetT2DNORMALLINE(
		lPoint,
		vPoint[0],
		vPoint[1],
		pA, pB, pC);
}

BYTE CTMapView::GetT2DNORMALLINE( T2DPOINT lPoint,
								  T2DPOINT lLineS,
								  T2DPOINT lLineE,
								  FLOAT *pA,
								  FLOAT *pB,
								  FLOAT *pC)
{
	FLOAT fStartX = GET2D_X(lPoint);
	FLOAT fStartY = GET2D_Y(lPoint);

	return GetT2DLINE(
		fStartX,
		fStartY,
		fStartX - GET2D_Y(lLineE) + GET2D_Y(lLineS),
		fStartY + GET2D_X(lLineE) - GET2D_X(lLineS),
		pA, pB, pC);
}

BYTE CTMapView::GetT2DLINE( T2DPOINT lStart,
						    T2DPOINT lEnd,
							FLOAT *pA,
							FLOAT *pB,
							FLOAT *pC)
{
	return GetT2DLINE(
		GET2D_X(lStart),
		GET2D_Y(lStart),
		GET2D_X(lEnd),
		GET2D_Y(lEnd),
		pA, pB, pC);
}

BYTE CTMapView::GetT2DLINE( FLOAT fStartX,
						    FLOAT fStartY,
							FLOAT fEndX,
							FLOAT fEndY,
							FLOAT *pA,
							FLOAT *pB,
							FLOAT *pC)
{
	FLOAT fC = fStartY * fEndX - fStartX * fEndY;
	FLOAT fA = fEndY - fStartY;
	FLOAT fB = fStartX - fEndX;
	FLOAT fL = sqrt(fA * fA + fB * fB);

	if( fL != 0.0f )
	{
		(*pA) = fA / fL;
		(*pB) = fB / fL;
		(*pC) = fC / fL;

		return TRUE;
	}

	return FALSE;
}

FLOAT CTMapView::GetT2DDotCoord( T2DPOINT lPoint,
								 FLOAT fA,
								 FLOAT fB,
								 FLOAT fC)
{
	return GET2D_X(lPoint) * fA + GET2D_Y(lPoint) * fB + fC;
}

void CTMapView::ClearTestPath( BYTE bClearPoint)
{
	m_mapTPATHPOINT.clear();
	m_mapTPATHLINE.clear();

	for( int i=0; i<2; i++)
	{
		if(bClearPoint)
			m_vPoint[i] = 0;

		m_vPointID[i] = 0;
	}
}

void CTMapView::_FindTestPath( LPTFINDINFO lParam)
{
	lParam->m_pHost->FindTestPath(lParam);
}

void CTMapView::FindTestPath( LPTFINDINFO lParam)
{
	CTQuestPathDoc *pDOC = GetDocument();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_MSG_FIND_PATH);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	ClearTestPath(FALSE);

	if(pDOC)
	{
		MAPTPATHDATA::iterator itTPATH;
		MAPTPATHDATA mapTPATHDATA;
		VDWORD vTFULLPATH;

		BYTE bDirect = CanMove(
			m_pTUNIT,
			m_vPoint[0],
			m_vPoint[1]);

		FLOAT fDIST = bDirect ? pDOC->GetLineLength(
			m_vPoint[0],
			m_vPoint[1]) : 0.0f;
		FLOAT fDirect = fDIST;

		pDOC->MakePathData(
			&mapTPATHDATA,
			m_pTUNIT);
		vTFULLPATH.clear();

		DWORD dwTotal = DWORD(mapTPATHDATA.size());
		DWORD dwTCUR = 0;

		for( itTPATH = mapTPATHDATA.begin(); itTPATH != mapTPATHDATA.end(); itTPATH++)
		{
			WORD wPointID = (*itTPATH).first;
			T2DPOINT lPoint = 0;

			MAPT2DPOINT::iterator finder = m_pTUNIT->m_mapTPOINT.find(wPointID);
			if( finder != m_pTUNIT->m_mapTPOINT.end() )
				lPoint = (*finder).second;

			if(lPoint)
			{
				FLOAT fLocal = pDOC->GetLineLength(
					m_vPoint[0],
					lPoint);

				if( ((!bDirect && !m_vPointID[1]) || fLocal < fDIST) && CanMove( m_pTUNIT, m_vPoint[0], lPoint) )
				{
					MAPTPATHINFO::iterator itTLAST;
					MAPTPATHINFO mapTLOCALPATH;

					MAPT2DPOINT mapTPATH;
					VDWORD vTPATH;

					FLOAT fLAST = 0.0f;
					WORD wLastID = 0;

					mapTLOCALPATH.clear();
					mapTPATH.clear();
					vTPATH.clear();

					pDOC->MakeFullPathData(
						&mapTPATHDATA,
						&mapTLOCALPATH,
						wPointID,
						&mapTPATH,
						&vTPATH, 0.0f);

					for( itTLAST = mapTLOCALPATH.begin(); itTLAST != mapTLOCALPATH.end(); itTLAST++)
					{
						LPTPATHINFO pTPATHINFO = (*itTLAST).second;
						WORD wLocalLastID = (*itTLAST).first;
						T2DPOINT lLast = 0;

						finder = m_pTUNIT->m_mapTPOINT.find(wLocalLastID);
						if( finder != m_pTUNIT->m_mapTPOINT.end() )
							lLast = (*finder).second;

						if(lLast)
						{
							FLOAT fLocalLast = pDOC->GetLineLength(
								m_vPoint[1],
								lLast);

							if( (!wLastID || fLocalLast < fLAST) && CanMove( m_pTUNIT, m_vPoint[1], lLast) )
							{
								wLastID = wLocalLastID;
								fLAST = fLocalLast;

								m_vPointID[0] = wPointID;
								m_vPointID[1] = wLastID;

								vTFULLPATH.clear();
								fDIST = fLocal;

								for( int i=0; i<INT(pTPATHINFO->m_vTPATH.size()); i++)
									vTFULLPATH.push_back(pTPATHINFO->m_vTPATH[i]);
							}
						}

						delete pTPATHINFO;
					}

					mapTLOCALPATH.clear();
					mapTPATH.clear();
					vTPATH.clear();
				}
			}

			dwTCUR++;
			lParam->m_pWnd->m_bPercent = BYTE(dwTCUR * 100 / dwTotal);

			lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
			lParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

		if( m_vPointID[0] && m_vPointID[1] && !vTFULLPATH.empty() )
		{
			int nCount = INT(vTFULLPATH.size());

			while( nCount > 1 && CheckBreak( m_pTUNIT, m_vPoint[0], WORD(vTFULLPATH[0]), WORD(vTFULLPATH[1])) )
			{
				vTFULLPATH.erase(vTFULLPATH.begin());
				m_vPointID[0] = WORD(vTFULLPATH[0]);

				nCount--;
			}

			nCount = INT(vTFULLPATH.size());
			while( nCount > 1 && CheckBreak( m_pTUNIT, m_vPoint[1], WORD(vTFULLPATH[nCount - 1]), WORD(vTFULLPATH[nCount - 2])) )
			{
				vTFULLPATH.pop_back();
				m_vPointID[1] = WORD(vTFULLPATH.back());

				nCount--;
			}

			if( bDirect && m_vPointID[0] && m_vPointID[1] )
				for( int i=0; i<2; i++)
				{
					MAPT2DPOINT::iterator finder = m_pTUNIT->m_mapTPOINT.find(m_vPointID[i]);

					if( finder == m_pTUNIT->m_mapTPOINT.end() ||
						fDirect < pDOC->GetLineLength( m_vPoint[i], (*finder).second) )
					{
						vTFULLPATH.clear();
						m_vPointID[0] = 0;
						m_vPointID[1] = 0;

						break;
					}
				}
		}

		if( m_vPointID[0] && m_vPointID[1] && !vTFULLPATH.empty() )
		{
			m_mapTPATHPOINT.insert( MAPT2DPOINT::value_type( WORD(vTFULLPATH[0]), 0));

			for( int i=1; i<INT(vTFULLPATH.size()); i++)
			{
				DWORD vTLINE[2] = {
					MAKELONG( WORD(vTFULLPATH[i - 1]), WORD(vTFULLPATH[i])),
					MAKELONG( WORD(vTFULLPATH[i]), WORD(vTFULLPATH[i - 1]))};

				m_mapTPATHPOINT.insert( MAPT2DPOINT::value_type( WORD(vTFULLPATH[i]), 0));
				for( int j=0; j<2; j++)
				{
					MAPDWORD::iterator itTLINE = m_pTUNIT->m_mapTLINE.find(vTLINE[j]);

					if( itTLINE != m_pTUNIT->m_mapTLINE.end() && (*itTLINE).second == TLINE_PATH )
						m_mapTPATHLINE.insert( MAPDWORD::value_type( (*itTLINE).first, (*itTLINE).second));
				}
			}
		}
		else if(!bDirect)
			ClearTestPath(TRUE);

		pDOC->ClearPathData(&mapTPATHDATA);
		vTFULLPATH.clear();
	}

	lParam->m_pWnd->m_cProgressBar.SetPos(100);
	lParam->m_pWnd->m_bPercent = 100;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}


// CTMapView 진단입니다.

#ifdef _DEBUG
void CTMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTQuestPathDoc* CTMapView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTQuestPathDoc)));
	return (CTQuestPathDoc*)m_pDocument;
}
#endif //_DEBUG


// CTMapView 메시지 처리기입니다.

void CTMapView::OnDestroy()
{
	CScrollView::OnDestroy();

	if(m_hTUNIT)
	{
		DeleteObject(m_hTUNIT);
		m_hTUNIT = NULL;
	}
}

void CTMapView::UpdateDATA( LPTTITEM pTTITEM)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(m_hTUNIT)
	{
		DeleteObject(m_hTUNIT);
		m_hTUNIT = NULL;
	}
	ClearTestPath(TRUE);

	m_pTUNIT = NULL;
	m_bDRAW = FALSE;

	m_dwMonKind = 0;
	m_dwMapID = 0;
	m_dwLineID = 0;

	m_wPointID = 0;
	m_wUnitX = 0;
	m_wUnitZ = 0;

	m_fSizeX = 0.0f;
	m_fSizeZ = 0.0f;

	m_fPosX = 0.0f;
	m_fPosZ = 0.0f;

	if(pDOC)
	{
		pDOC->UpdateDATA( TPATHWND_MAP, TUPDATE_POS, m_wPointID);

		if(pTTITEM)
		{
			LPTMAPDATA pTMAP = pDOC->FindTMAPDATA(pTTITEM->m_dwMapID);

			m_dwMonKind = pTTITEM->m_dwMonKind;
			m_dwMapID = pTTITEM->m_dwMapID;

			if(pTMAP)
			{
				m_fSizeX = FLOAT(pTMAP->m_dwUnitX);
				m_fSizeZ = FLOAT(pTMAP->m_dwUnitZ);

				m_wUnitX = WORD(pTTITEM->m_fPosX / m_fSizeX);
				m_wUnitZ = WORD(pTTITEM->m_fPosZ / m_fSizeZ);

				m_fPosX = pTTITEM->m_fPosX - FLOAT(m_wUnitX) * m_fSizeX;
				m_fPosZ = pTTITEM->m_fPosZ - FLOAT(m_wUnitZ) * m_fSizeZ;

				m_pTUNIT = pDOC->FindTMAPIMG(
					pTMAP,
					m_wUnitX,
					m_wUnitZ);
				m_bDRAW = TRUE;

				if( m_pTUNIT && m_pTUNIT->m_vTBMPINFO.m_pTBUF && m_pTUNIT->m_vTBMPBITS.m_pTBUF )
				{
					CClientDC dc(this);

					m_hTUNIT = CreateDIBitmap(
						dc.GetSafeHdc(),
						m_pTUNIT->m_vTBMPINFO.m_pTHEADER,
						CBM_INIT,
						m_pTUNIT->m_vTBMPBITS.m_pTDATA,
						m_pTUNIT->m_vTBMPINFO.m_pTINFO,
						DIB_RGB_COLORS);
				}
			}
		}
	}

	OnUpdateSIZE();
	GoToTPOINT();
	Invalidate(FALSE);
}

void CTMapView::GoToTPOINT()
{
	CPoint vTPOINT = GetClientCoord(
		m_fPosX,
		m_fPosZ);

	int nHeight = GetHeight();
	int nWidth = GetWidth();
	CRect vRECT;

	GetClientRect(&vRECT);
	vRECT.OffsetRect(
		-vRECT.left,
		-vRECT.top);

	vTPOINT -= vRECT.CenterPoint();
	vRECT.OffsetRect(vTPOINT);

	vRECT.OffsetRect(
		vRECT.right > nWidth ? nWidth - vRECT.right : 0,
		vRECT.bottom > nHeight ? nHeight - vRECT.bottom : 0);

	vRECT.OffsetRect(
		vRECT.left < 0 ? -vRECT.left : 0,
		vRECT.top < 0 ? -vRECT.top : 0);

	ScrollToPosition(vRECT.TopLeft());
}

CStatusBar* CTMapView::GetStatusBar()
{
	CMainFrame *pMainWnd = (CMainFrame *) AfxGetMainWnd();
	return pMainWnd ? pMainWnd->GetStatusBar() : NULL;
}

void CTMapView::OnUpdateSCALE()
{
	CStatusBar *pWnd = GetStatusBar();

	OnUpdateSIZE();
	if(pWnd)
	{
		CString strINFO;

		strINFO.Format( IDS_FMT_SCALE, m_fSCALE);
		pWnd->SetPaneText( TPANE_SCALE, strINFO);
	}
}

void CTMapView::OnUpdateSIZE()
{
	CSize szTotal(
		GetWidth(),
		GetHeight());

	SetScrollSizes( MM_TEXT, szTotal);
}

void CTMapView::OnUpdatePOS( FLOAT fPosX,
							 FLOAT fPosZ)
{
	CStatusBar *pWnd = GetStatusBar();

	if(pWnd)
	{
	}
}

int CTMapView::GetHeight()
{
	return INT(m_fSizeZ * m_fSCALE) + 1;
}

int CTMapView::GetWidth()
{
	return INT(m_fSizeX * m_fSCALE) + 1;
}

void CTMapView::OnMapScale()
{
	CTZoomDlg dlg;

	dlg.m_strSCALE.Format( IDS_FMT_POS_DATA, m_fSCALE);
	if( dlg.DoModal() == IDOK )
	{
		sscanf( LPCSTR(dlg.m_strSCALE), _T("%f"), &m_fSCALE);
		OnUpdateSCALE();
		Invalidate();
	}
}

BOOL CTMapView::OnEraseBkgnd( CDC *pDC)
{
	return TRUE;
}

void CTMapView::OnMouseMove( UINT nFlags, CPoint point)
{
	CStatusBar *pWnd = GetStatusBar();

	if(pWnd)
	{
		CTQuestPathDoc *pDOC = GetDocument();
		CPoint pt = GetScrollPosition();
		BYTE bDRAW = m_bDRAW;

		CString strINFO;
		CRect rect;

		CRect rcTMAP(
			0, 0,
			GetWidth() - pt.x,
			GetHeight() - pt.y);
		GetClientRect(&rect);

		if( !rect.IntersectRect( &rcTMAP, &rect) || !rect.PtInRect(point) )
		{
			ReleaseCapture();

			m_bTACT = FALSE;
			bDRAW = FALSE;
			m_dwLineID = 0;
		}
		else
			SetCapture();

		if( pDOC && pDOC->m_bEditPATH && m_pTUNIT )
			switch(pDOC->m_nPathMode)
			{
			case TEDITPATH_MOVE_POINT	:
				if( m_wPointID && m_bTACT )
				{
					FLOAT fPosX = (FLOAT) pt.x + point.x;
					FLOAT fPosZ = (FLOAT) pt.y + point.y;

					GetUnitCoord(
						fPosX,
						fPosZ);

					pDOC->MovePathPoint(
						m_pTUNIT,
						m_wPointID,
						fPosX,
						fPosZ);

					pDOC->UpdateDATA( TPATHWND_MAP, TUPDATE_POS, m_wPointID);
					Invalidate(FALSE);
				}

				break;

			case TEDITPATH_ADD_BLOCK	:
			case TEDITPATH_ADD_PATH		: Invalidate(FALSE); break;
			}

		if(bDRAW)
		{
			FLOAT fPosX = (FLOAT) pt.x + point.x;
			FLOAT fPosZ = (FLOAT) pt.y + point.y;

			GetWorldCoord(
				fPosX,
				fPosZ);

			strINFO.Format( IDS_FMT_POS_X, fPosX);
			pWnd->SetPaneText( TPANE_POS_X, strINFO);
			strINFO.Format( IDS_FMT_POS_Z, fPosZ);
			pWnd->SetPaneText( TPANE_POS_Z, strINFO);
		}
		else
		{
			strINFO.Empty();
			pWnd->SetPaneText( TPANE_POS_X, strINFO);
			pWnd->SetPaneText( TPANE_POS_Z, strINFO);
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CTMapView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( pDOC && !m_bFinding &&
		m_pTUNIT && (
		pDOC->m_bEditPATH ||
		pDOC->m_nPathMode) )
	{
		CPoint pt = GetScrollPosition();
		CRect rect;
		BYTE bDRAW = m_bDRAW;

		CRect rcTMAP(
			0, 0,
			GetWidth() - pt.x,
			GetHeight() - pt.y);
		GetClientRect(&rect);

		if( !rect.IntersectRect( &rcTMAP, &rect) || !rect.PtInRect(point) )
			bDRAW = FALSE;

		switch(pDOC->m_nPathMode)
		{
		case TEDITPATH_TEST_PATH	:
			if(bDRAW)
			{
				FLOAT fPosX = (FLOAT) pt.x + point.x;
				FLOAT fPosZ = (FLOAT) pt.y + point.y;

				GetUnitCoord(
					fPosX,
					fPosZ);

				T2DPOINT lPoint = MAKET2DPOINT(
					fPosX,
					fPosZ);

				if(!m_vPoint[0])
					m_vPoint[0] = lPoint;
				else if( m_vPoint[0] != lPoint )
				{
					CProgressDlg vTPROGRESS;
					TFINDINFO vTPARAM;

					vTPROGRESS.m_strCancel.LoadString(IDS_MSG_FIND_CANCEL);
					vTPROGRESS.m_strTitle.LoadString(IDS_TITLE_FIND_PATH);
					vTPROGRESS.m_pThreadFunc = (AFX_THREADPROC) _FindTestPath;
					vTPROGRESS.m_lParam = &vTPARAM;

					vTPARAM.m_pWnd = &vTPROGRESS;
					vTPARAM.m_pHost = this;

					m_vPoint[1] = lPoint;
					m_bFinding = TRUE;
					vTPROGRESS.DoModal();
					m_bFinding = FALSE;
				}

				Invalidate(FALSE);
			}
			else
			{
				ClearTestPath(TRUE);
				Invalidate(FALSE);
			}

			break;

		case TEDITPATH_PICK_POS		:
			if(bDRAW)
			{
				TTITEM vTPOS;

				m_fPosX = (FLOAT) pt.x + point.x;
				m_fPosZ = (FLOAT) pt.y + point.y;

				GetUnitCoord(
					m_fPosX,
					m_fPosZ);

				vTPOS.m_fPosX = m_fPosX + FLOAT(m_wUnitX) * m_fSizeX;
				vTPOS.m_fPosZ = m_fPosZ + FLOAT(m_wUnitZ) * m_fSizeZ;
				vTPOS.m_dwMapID = m_dwMapID;

				pDOC->UpdateDATA( TPATHWND_POS, TUPDATE_POS, (LPARAM) &vTPOS);
				Invalidate(FALSE);
			}

			break;

		case TEDITPATH_DEL_POINT	:
			{
				WORD wPointID = PickPathPoint(
					m_pTUNIT,
					point,
					0, 0,
					TPOINTSIZE_PATH);

				if(wPointID)
				{
					pDOC->DeletePathPoint( m_pTUNIT, wPointID, 0);
					Invalidate(FALSE);
				}
			}

			break;

		case TEDITPATH_DEL_LINE		:
			{
				DWORD dwLineID = PickTLINE(
					m_pTUNIT,
					point,
					TLINESIZE_PATH);

				if(dwLineID)
				{
					MAPDWORD::iterator finder = m_pTUNIT->m_mapTLINE.find(dwLineID);

					if( finder != m_pTUNIT->m_mapTLINE.end() )
					{
						m_pTUNIT->m_mapTLINE.erase(finder);
						Invalidate(FALSE);
					}
				}
			}

			break;

		case TEDITPATH_ADD_BLOCK	:
		case TEDITPATH_ADD_PATH		:
			{
				WORD wPointID = PickPathPoint(
					m_pTUNIT,
					point,
					LOWORD(m_dwLineID), 0,
					TPOINTSIZE_PATH);

				if(!m_dwLineID)
					m_dwLineID = MAKELONG( wPointID, 0);
				else
				{
					if( wPointID &&
						m_pTUNIT->m_mapTLINE.find(MAKELONG( LOWORD(m_dwLineID), wPointID)) == m_pTUNIT->m_mapTLINE.end() &&
						m_pTUNIT->m_mapTLINE.find(MAKELONG( wPointID, LOWORD(m_dwLineID))) == m_pTUNIT->m_mapTLINE.end() )
						m_pTUNIT->m_mapTLINE.insert( MAPDWORD::value_type( MAKELONG( LOWORD(m_dwLineID), wPointID), pDOC->m_nPathMode == TEDITPATH_ADD_PATH ? TLINE_PATH : TLINE_BLOCK));

					m_dwLineID = 0;
				}

				Invalidate(FALSE);
			}

			break;

		case TEDITPATH_MOVE_POINT	:
			{
				m_wPointID = PickPathPoint(
					m_pTUNIT,
					point,
					m_wPointID,
					TPOINTSIZE_PSEL,
					TPOINTSIZE_PATH);

				if(m_wPointID)
					m_bTACT = TRUE;

				pDOC->UpdateDATA( TPATHWND_MAP, TUPDATE_POS, m_wPointID);
				Invalidate(FALSE);
			}

			break;

		case TEDITPATH_ADD_POINT	:
			if(bDRAW)
			{
				FLOAT fPosX = (FLOAT) pt.x + point.x;
				FLOAT fPosZ = (FLOAT) pt.y + point.y;

				GetUnitCoord(
					fPosX,
					fPosZ);

				T2DPOINT lPoint = MAKET2DPOINT(
					fPosX,
					fPosZ);

				if( m_pTUNIT->m_mapTINDEX.find(lPoint) == m_pTUNIT->m_mapTINDEX.end() )
				{
					WORD wPointID = 0;

					while( !wPointID || m_pTUNIT->m_mapTPOINT.find(wPointID) != m_pTUNIT->m_mapTPOINT.end() )
						wPointID++;

					m_pTUNIT->m_mapTPOINT.insert( MAPT2DPOINT::value_type( wPointID, lPoint));
					m_pTUNIT->m_mapTINDEX.insert( MAPT2DINDEX::value_type( lPoint, wPointID));

					Invalidate(FALSE);
				}
			}

			break;
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTMapView::OnRButtonDown( UINT nFlags, CPoint point)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( !m_bFinding && pDOC && pDOC->m_bEditPATH )
		switch(pDOC->m_nPathMode)
		{
		case TEDITPATH_TEST_PATH	:
			{
				ClearTestPath(TRUE);
				Invalidate(FALSE);
			}

			break;
		}

	CScrollView::OnRButtonDown(nFlags, point);
}

void CTMapView::OnLButtonUp( UINT nFlags, CPoint point)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( !m_bFinding && pDOC && pDOC->m_bEditPATH )
		switch(pDOC->m_nPathMode)
		{
		case TEDITPATH_MOVE_POINT	: m_bTACT = FALSE; break;
		}

	CScrollView::OnLButtonUp(nFlags, point);
}
