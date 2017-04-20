// TPathTimeView.cpp : implementation of the CTPathTimeView class
//

#include "stdafx.h"
#include "TPathTime.h"

#include "TPathTimeDoc.h"
#include "TPathTimeView.h"
#include "EditJointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTPathTimeView

IMPLEMENT_DYNCREATE(CTPathTimeView, CScrollView)

BEGIN_MESSAGE_MAP(CTPathTimeView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTPathTimeView construction/destruction

CTPathTimeView::CTPathTimeView()
{
}

CTPathTimeView::~CTPathTimeView()
{
}

BOOL CTPathTimeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CTPathTimeView drawing

void CTPathTimeView::OnDraw( CDC* /*pDC*/)
{
	CTPathTimeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	int nPosX = GetScrollPos(SB_HORZ);
	int nPosY = GetScrollPos(SB_VERT);

	CBitmap vBMP;
	CDC dcMEM;

	CClientDC dc(this);
	CRect rect;

	GetClientRect(&rect);
	dcMEM.CreateCompatibleDC(&dc);

	vBMP.CreateCompatibleBitmap(
		&dc,
		rect.Width(),
		rect.Height());

	HBITMAP hOLDMEM = (HBITMAP) dcMEM.SelectObject(HBITMAP(vBMP));
	dcMEM.FillRect( &rect, &CBrush(TBACKGROUND_COLOR));

	if(pDoc->m_hBMP)
	{
		CDC dcSRC;
		dcSRC.CreateCompatibleDC(&dc);

		HBITMAP hOLDSRC = (HBITMAP) dcSRC.SelectObject(pDoc->m_hBMP);
		dcMEM.BitBlt(
			0, 0,
			rect.Width(),
			rect.Height(),
			&dcSRC,
			nPosX,
			nPosY,
			SRCCOPY);

		dcSRC.SelectObject(hOLDSRC);
		dcSRC.DeleteDC();
	}

	switch(pDoc->m_bEditMode)
	{
	case TEDIT_INFO_TSPOT	:
	case TEDIT_SELECT_TSPOT	:
	case TEDIT_ADD_TSPOT	:
	case TEDIT_DELETE_TSPOT	: DrawTSPOT( pDoc, &dcMEM, nPosX, nPosY); break;
	case TEDIT_EDIT_JOINT	:
	case TEDIT_MOVE_JOINT	:
	case TEDIT_DELETE_JOINT	: DrawTJOINT( pDoc, &dcMEM, nPosX, nPosY); break;
	case TEDIT_ADD_JOINT	:
		{
			DrawTJOINT( pDoc, &dcMEM, nPosX, nPosY);
			DrawTJOINTCursor( &dcMEM, nPosX, nPosY);
		}

		break;
	}

	dc.BitBlt(
		0, 0,
		rect.Width(),
		rect.Height(),
		&dcMEM,
		0, 0,
		SRCCOPY);

	dcMEM.SelectObject(hOLDMEM);
	vBMP.DeleteObject();
	dcMEM.DeleteDC();

	pDoc->UpdatePathTime();
	pDoc->UpdateUnitID();
}

void CTPathTimeView::DrawTSPOT( CTPathTimeDoc *pDoc,
							    CDC *pDEST,
								int nPosX,
								int nPosY)
{
	CBitmap vTSPOTSEL;
	CBitmap vTSPOT;
	CBitmap vTMASK;

	CDC dcMASK;
	CDC dcSRC;

	dcMASK.CreateCompatibleDC(pDEST);
	dcSRC.CreateCompatibleDC(pDEST);

	vTSPOTSEL.LoadBitmap(IDB_BITMAP_SPOT_SEL);
	vTMASK.LoadBitmap(IDB_BITMAP_SPOT_MASK);
	vTSPOT.LoadBitmap(IDB_BITMAP_SPOT);

	if( INT(pDoc->m_vTSPOTSEL.size()) > 1 )
	{
		CPen vOutLine( PS_SOLID, 8, TOUTLINE_COLOR);
		CPen vLine( PS_SOLID, 4, TLINE_COLOR);

		HPEN pOLDPEN = (HPEN) pDEST->SelectObject(HPEN(vOutLine));
		pDEST->MoveTo(
			pDoc->m_vTSPOTSEL[0]->m_nPosX - nPosX,
			pDoc->m_vTSPOTSEL[0]->m_nPosY - nPosY);

		for( int i=1; i<INT(pDoc->m_vTSPOTSEL.size()); i++)
		{
			pDEST->LineTo(
				pDoc->m_vTSPOTSEL[i]->m_nPosX - nPosX,
				pDoc->m_vTSPOTSEL[i]->m_nPosY - nPosY);
		}

		pDEST->SelectObject(HPEN(vLine));
		pDEST->MoveTo(
			pDoc->m_vTSPOTSEL[0]->m_nPosX - nPosX,
			pDoc->m_vTSPOTSEL[0]->m_nPosY - nPosY);

		for( int i=1; i<INT(pDoc->m_vTSPOTSEL.size()); i++)
		{
			pDEST->LineTo(
				pDoc->m_vTSPOTSEL[i]->m_nPosX - nPosX,
				pDoc->m_vTSPOTSEL[i]->m_nPosY - nPosY);
		}

		pDEST->SelectObject(pOLDPEN);
		vOutLine.DeleteObject();
		vLine.DeleteObject();
	}

	HBITMAP hOLDMASK = (HBITMAP) dcMASK.SelectObject(HBITMAP(vTMASK));
	HBITMAP hOLDSRC = (HBITMAP) dcSRC.SelectObject(HBITMAP(vTSPOT));

	for( int i=0; i<INT(pDoc->m_vTSPOT.size()); i++)
		DrawTSPOT( &dcMASK, &dcSRC, pDEST, pDoc->m_vTSPOT[i], nPosX, nPosY);

	dcSRC.SelectObject(HBITMAP(vTSPOTSEL));
	for( i=0; i<INT(pDoc->m_vTSPOTSEL.size()); i++)
		DrawTSPOT( &dcMASK, &dcSRC, pDEST, pDoc->m_vTSPOTSEL[i], nPosX, nPosY);

	dcMASK.SelectObject(hOLDMASK);
	dcSRC.SelectObject(hOLDSRC);

	vTSPOTSEL.DeleteObject();
	vTMASK.DeleteObject();
	vTSPOT.DeleteObject();

	dcSRC.DeleteDC();
	dcMASK.DeleteDC();
}

void CTPathTimeView::DrawTSPOT( CDC *pMASK,
							    CDC *pSRC,
							    CDC *pDEST,
								LPTSPOT pTSPOT,
								int nPosX,
								int nPosY)
{
	int nSrcX = 0;
	int nSrcY = 0;

	if( !pTSPOT || !pSRC || !pDEST )
		return;

	nPosX = pTSPOT->m_nPosX - nPosX - TSPOT_SIZE / 2;
	nPosY = pTSPOT->m_nPosY - nPosY - TSPOT_SIZE / 2;

	if( nPosX < 0 )
	{
		nSrcX = -nPosX;
		nPosX = 0;
	}

	if( nPosY < 0 )
	{
		nSrcY = -nPosY;
		nPosY = 0;
	}

	pDEST->BitBlt(
		nPosX,
		nPosY,
		TSPOT_SIZE - nSrcX,
		TSPOT_SIZE - nSrcY,
		pMASK,
		nSrcX,
		nSrcY,
		SRCPAINT);

	pDEST->BitBlt(
		nPosX,
		nPosY,
		TSPOT_SIZE - nSrcX,
		TSPOT_SIZE - nSrcY,
		pSRC,
		nSrcX,
		nSrcY,
		SRCAND);
}

void CTPathTimeView::DrawTJOINTCursor( CDC *pDEST,
									   int nPosX,
									   int nPosY)
{
	CRect rect(
		0, 0,
		theApp.m_vTJOINT.cx,
		theApp.m_vTJOINT.cy);
	CPoint point;

	GetCursorPos(&point);
	ScreenToClient(&point);

	rect.OffsetRect(
		point.x - theApp.m_vTJOINT.cx / 2 + theApp.m_nSNAP / 2 + nPosX,
		point.y - theApp.m_vTJOINT.cy / 2 + theApp.m_nSNAP / 2 + nPosY);

	rect.OffsetRect(
		rect.left / theApp.m_nSNAP * theApp.m_nSNAP - rect.left,
		rect.top / theApp.m_nSNAP * theApp.m_nSNAP - rect.top);
	RegulateTJOINTRect(&rect);

	rect.OffsetRect(
		-nPosX,
		-nPosY);
	CPen vLine( PS_SOLID, 1, TOUTLINE_COLOR);

	HPEN pOLDPEN = (HPEN) pDEST->SelectObject(HPEN(vLine));
	DrawTJOINTEdge( pDEST, &rect);

	pDEST->SelectObject(pOLDPEN);
	vLine.DeleteObject();
}

void CTPathTimeView::DrawTJOINT( CTPathTimeDoc *pDoc,
								 CDC *pDEST,
								 int nPosX,
								 int nPosY)
{
	MAPTJOINT::iterator itTJOINT;

	CBrush vGRAY(TGRAY_COLOR);
	CBitmap vBMP;
	CDC dcMEM;
	CRect rect;

	dcMEM.CreateCompatibleDC(pDEST);
	GetClientRect(&rect);

	vBMP.CreateCompatibleBitmap(
		pDEST,
		rect.Width(),
		rect.Height());

	HBITMAP hOLDMEM = (HBITMAP) dcMEM.SelectObject(HBITMAP(vBMP));
	dcMEM.FillRect( &rect, &CBrush(TBACKGROUND_COLOR));

	for( itTJOINT = pDoc->m_mapTJOINT.begin(); itTJOINT != pDoc->m_mapTJOINT.end(); itTJOINT++)
		if(!(*itTJOINT).second->m_bJOINT)
		{
			rect = (*itTJOINT).second->m_vRECT;
			rect.OffsetRect(
				-nPosX,
				-nPosY);
			dcMEM.FillRect( &rect, &vGRAY);
		}

	GetClientRect(&rect);
	pDEST->BitBlt(
		0, 0,
		rect.Width(),
		rect.Height(),
		&dcMEM,
		0, 0,
		SRCAND);

	dcMEM.SelectObject(hOLDMEM);
	vGRAY.DeleteObject();
	vBMP.DeleteObject();
	dcMEM.DeleteDC();

	CPen vOutLine( PS_SOLID, 1, TOUTLINE_COLOR);
	CPen vLine( PS_SOLID, 1, TLINE_COLOR);
	CPen vMOVE( PS_SOLID, 1, TSELECT_COLOR);
	HPEN pOLDPEN = (HPEN) pDEST->SelectObject(HPEN(vOutLine));

	for( itTJOINT = pDoc->m_mapTJOINT.begin(); itTJOINT != pDoc->m_mapTJOINT.end(); itTJOINT++)
	{
		rect = (*itTJOINT).second->m_vRECT;
		rect.OffsetRect(
			-nPosX,
			-nPosY);
		DrawTJOINT( pDEST, &rect);
	}
	pDEST->SelectObject(HPEN(vLine));

	for( itTJOINT = pDoc->m_mapTJOINT.begin(); itTJOINT != pDoc->m_mapTJOINT.end(); itTJOINT++)
	{
		rect = (*itTJOINT).second->m_vRECT;
		rect.OffsetRect(
			-nPosX,
			-nPosY);
		DrawTJOINTEdge( pDEST, &rect);
	}

	if(pDoc->m_pTMOVE)
	{
		pDEST->SelectObject(HPEN(vMOVE));
		rect = pDoc->m_pTMOVE->m_vRECT;
		rect.OffsetRect(
			-nPosX,
			-nPosY);
		DrawTJOINTEdge( pDEST, &rect);
	}
	pDEST->SetBkMode(TRANSPARENT);

	for( itTJOINT = pDoc->m_mapTJOINT.begin(); itTJOINT != pDoc->m_mapTJOINT.end(); itTJOINT++)
	{
		static int vTOUT[8][2] = {
			{ -1, -1},
			{  1,  0},
			{  1,  0},
			{  0,  1},
			{  0,  1},
			{ -1,  0},
			{ -1,  0},
			{  0, -1}};
		CString strID;

		rect = (*itTJOINT).second->m_vRECT;
		rect.OffsetRect(
			-nPosX,
			-nPosY);

		strID.Format( _T("%d"), (*itTJOINT).first);
		pDEST->SetTextColor(TTEXTOUTLINE_COLOR);

		for( int i=0; i<8; i++)
		{
			rect.OffsetRect( vTOUT[i][0], vTOUT[i][1]);
			pDEST->DrawText(
				strID,
				&rect,
				DT_CENTER|
				DT_SINGLELINE|
				DT_VCENTER);
		}

		pDEST->SetTextColor(TTEXT_COLOR);
		rect.OffsetRect( 1, 0);
		pDEST->DrawText(
			strID,
			&rect,
			DT_CENTER|
			DT_SINGLELINE|
			DT_VCENTER);
	}

	pDEST->SelectObject(pOLDPEN);
	vOutLine.DeleteObject();
	vLine.DeleteObject();
	vMOVE.DeleteObject();
}

void CTPathTimeView::RegulateTJOINTRect( CRect *pRECT)
{
	CSize size = GetTotalSize();

	if( pRECT->right > size.cx )
		pRECT->OffsetRect( size.cx - pRECT->right, 0);

	if( pRECT->bottom > size.cy )
		pRECT->OffsetRect( 0, size.cy - pRECT->bottom);

	if( pRECT->left < 0 )
		pRECT->OffsetRect( -pRECT->left, 0);

	if( pRECT->top < 0 )
		pRECT->OffsetRect( 0, -pRECT->top);
}

void CTPathTimeView::DrawTJOINTEdge( CDC *pDEST,
									 CRect *pRECT)
{
	int vRECT[4][4] = {
		{ pRECT->left, pRECT->top, 0, -pRECT->Height() / 4},
		{ pRECT->right, pRECT->top, pRECT->Width() / 4, 0},
		{ pRECT->right, pRECT->bottom, 0, pRECT->Height() / 4},
		{ pRECT->left, pRECT->bottom, -pRECT->Width() / 4, 0}};

	pDEST->MoveTo( vRECT[3][0], vRECT[3][1]);
	for( int i=0; i<4; i++)
	{
		pDEST->LineTo( vRECT[i][0] - 3 * vRECT[i][2], vRECT[i][1] - 3 * vRECT[i][3]);
		pDEST->MoveTo( vRECT[i][0] - vRECT[i][2], vRECT[i][1] - vRECT[i][3]);
		pDEST->LineTo( vRECT[i][0], vRECT[i][1]);
	}
}

void CTPathTimeView::DrawTJOINT( CDC *pDEST,
								 CRect *pRECT)
{
	int vRECT[4][2] = {
		{ pRECT->left, pRECT->top},
		{ pRECT->right, pRECT->top},
		{ pRECT->right, pRECT->bottom},
		{ pRECT->left, pRECT->bottom}};

	pDEST->MoveTo( vRECT[3][0], vRECT[3][1]);
	for( int i=0; i<4; i++)
		pDEST->LineTo( vRECT[i][0], vRECT[i][1]);
}

void CTPathTimeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	UpdateView();
}


// CTPathTimeView printing

BOOL CTPathTimeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTPathTimeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTPathTimeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTPathTimeView diagnostics

#ifdef _DEBUG
void CTPathTimeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTPathTimeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTPathTimeDoc* CTPathTimeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTPathTimeDoc)));
	return (CTPathTimeDoc*)m_pDocument;
}
#endif //_DEBUG


// CTPathTimeView message handlers

BOOL CTPathTimeView::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTPathTimeView::UpdateView()
{
	CTPathTimeDoc *pDoc = GetDocument();

	if( pDoc && pDoc->m_hBMP )
	{
		CBitmap *pBMP = CBitmap::FromHandle(pDoc->m_hBMP);

		if(pBMP)
		{
			BITMAP vBMP;
			CSize size;

			pBMP->GetBitmap(&vBMP);
			size.cx = vBMP.bmWidth;
			size.cy = vBMP.bmHeight;

			SetScrollSizes(
				MM_TEXT,
				size);
		}
	}
	else
	{
		CSize size;

		size.cx = 100;
		size.cy = 100;

		SetScrollSizes(
			MM_TEXT,
			size);
	}

	Invalidate(FALSE);
}

BYTE CTPathTimeView::FindTSPOT( LPVTSPOT pVTSPOT,
							    LPTSPOT pTSPOT)
{
	if( !pVTSPOT || !pTSPOT )
		return FALSE;

	for( int i=0; i<INT(pVTSPOT->size()); i++)
		if( pTSPOT == (*pVTSPOT)[i] )
			return TRUE;

	return FALSE;
}

int CTPathTimeView::SelectTSPOT( LPVTSPOT pTEXCEPT,
								 LPVTSPOT pTSRC,
								 int nPosX,
								 int nPosY)
{
	int nResult = -1;
	CPoint point(
		nPosX,
		nPosY);

	for( int i=0; i<INT(pTSRC->size()); i++)
	{
		LPTSPOT pTSPOT = (*pTSRC)[i];
		CRect rect(
			0, 0,
			TSPOT_SIZE,
			TSPOT_SIZE);

		rect.OffsetRect(
			pTSPOT->m_nPosX - TSPOT_SIZE / 2,
			pTSPOT->m_nPosY - TSPOT_SIZE / 2);

		if( rect.PtInRect(point) && !FindTSPOT( pTEXCEPT, pTSPOT) )
			nResult = i;
	}

	return nResult;
}

void CTPathTimeView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CTPathTimeDoc *pDoc = GetDocument();

	if(pDoc)
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);

		switch(pDoc->m_bEditMode)
		{
		case TEDIT_SELECT_TSPOT	:
			{
				int nTSEL = SelectTSPOT(
					&pDoc->m_vTSPOTSEL,
					&pDoc->m_vTSPOT,
					point.x + nPosX,
					point.y + nPosY);

				if( nTSEL >= 0 )
					pDoc->m_vTSPOTSEL.push_back(pDoc->m_vTSPOT[nTSEL]);

				Invalidate(FALSE);
			}

			break;

		case TEDIT_INFO_TSPOT	:
			{
				int nTSEL = SelectTSPOT(
					NULL,
					&pDoc->m_vTSPOT,
					point.x + nPosX,
					point.y + nPosY);

				if( nTSEL >= 0 )
				{
					CString strMSG;

					strMSG.Format(
						_T("Pos X = %d, Pos Y = %d"),
						pDoc->m_vTSPOT[nTSEL]->m_nPosX,
						pDoc->m_vTSPOT[nTSEL]->m_nPosY);

					MessageBox(strMSG);
				}

				pDoc->m_vTSPOTSEL.clear();
				Invalidate(FALSE);
			}

			break;

		case TEDIT_DELETE_TSPOT	:
			{
				int nTSEL = SelectTSPOT(
					NULL,
					&pDoc->m_vTSPOT,
					point.x + nPosX,
					point.y + nPosY);

				if( nTSEL >= 0 )
				{
					delete pDoc->m_vTSPOT[nTSEL];
					pDoc->m_vTSPOT.erase(pDoc->m_vTSPOT.begin() + nTSEL);
				}

				pDoc->m_vTSPOTSEL.clear();
				Invalidate(FALSE);
			}

			break;

		case TEDIT_ADD_TSPOT	:
			{
				LPTSPOT pTSPOT = new TSPOT();

				pTSPOT->m_nPosX = point.x + nPosX;
				pTSPOT->m_nPosY = point.y + nPosY;

				pDoc->m_vTSPOT.push_back(pTSPOT);
				pDoc->m_vTSPOTSEL.clear();
				Invalidate(FALSE);
			}

			break;

		case TEDIT_EDIT_JOINT	:
			{
				MAPTJOINT::reverse_iterator itTJOINT;
				CPoint pt(
					point.x + nPosX,
					point.y + nPosY);

				for( itTJOINT = pDoc->m_mapTJOINT.rbegin(); itTJOINT != pDoc->m_mapTJOINT.rend(); itTJOINT++)
					if((*itTJOINT).second->m_vRECT.PtInRect(pt))
					{
						CEditJointDlg dlg;

						dlg.m_pTJOINT = (*itTJOINT).second;
						dlg.m_dwID = (*itTJOINT).first;
						dlg.DoModal();

						break;
					}
				Invalidate(FALSE);
			}

			break;

		case TEDIT_MOVE_JOINT	:
			{
				MAPTJOINT::reverse_iterator itTJOINT;
				CPoint pt(
					point.x + nPosX,
					point.y + nPosY);
				pDoc->m_pTMOVE = NULL;

				for( itTJOINT = pDoc->m_mapTJOINT.rbegin(); itTJOINT != pDoc->m_mapTJOINT.rend(); itTJOINT++)
					if((*itTJOINT).second->m_vRECT.PtInRect(pt))
					{
						pDoc->m_pTMOVE = (*itTJOINT).second;
						break;
					}

				if(pDoc->m_pTMOVE)
				{
					CRect rect;

					GetClientRect(&rect);
					ClientToScreen(&rect);

					ClipCursor(&rect);
					SetCapture();
					Invalidate(FALSE);
				}
			}

			break;

		case TEDIT_ADD_JOINT	:
			{
				LPTJOINT pTJOINT = new TJOINT();

				pTJOINT->m_vRECT.SetRect(
					0, 0,
					theApp.m_vTJOINT.cx,
					theApp.m_vTJOINT.cy);

				pTJOINT->m_vRECT.OffsetRect(
					point.x - theApp.m_vTJOINT.cx / 2 + theApp.m_nSNAP / 2 + nPosX,
					point.y - theApp.m_vTJOINT.cy / 2 + theApp.m_nSNAP / 2 + nPosY);

				pTJOINT->m_vRECT.OffsetRect(
					pTJOINT->m_vRECT.left / theApp.m_nSNAP * theApp.m_nSNAP - pTJOINT->m_vRECT.left,
					pTJOINT->m_vRECT.top / theApp.m_nSNAP * theApp.m_nSNAP - pTJOINT->m_vRECT.top);
				RegulateTJOINTRect(&pTJOINT->m_vRECT);

				pDoc->m_mapTJOINT.insert( MAPTJOINT::value_type(
					pDoc->GetNewJointID(),
					pTJOINT));
				Invalidate(FALSE);
			}

			break;

		case TEDIT_DELETE_JOINT	:
			{
				MAPTJOINT::reverse_iterator itTJOINT;
				CPoint pt(
					point.x + nPosX,
					point.y + nPosY);

				for( itTJOINT = pDoc->m_mapTJOINT.rbegin(); itTJOINT != pDoc->m_mapTJOINT.rend(); itTJOINT++)
					if((*itTJOINT).second->m_vRECT.PtInRect(pt))
					{
						delete (*itTJOINT).second;
						pDoc->m_mapTJOINT.erase(pDoc->m_mapTJOINT.find((*itTJOINT).first));

						break;
					}
				Invalidate(FALSE);
			}

			break;
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTPathTimeView::OnMouseMove(UINT nFlags, CPoint point)
{
	CTPathTimeDoc *pDoc = GetDocument();

	if(pDoc)
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);

		switch(pDoc->m_bEditMode)
		{
		case TEDIT_ADD_JOINT	: Invalidate(FALSE); break;
		case TEDIT_MOVE_JOINT	:
			if(pDoc->m_pTMOVE)
			{
				pDoc->m_pTMOVE->m_vRECT.OffsetRect(
					point.x - pDoc->m_pTMOVE->m_vRECT.Width() / 2 + theApp.m_nSNAP / 2 + nPosX - pDoc->m_pTMOVE->m_vRECT.left,
					point.y - pDoc->m_pTMOVE->m_vRECT.Height() / 2 + theApp.m_nSNAP / 2 + nPosY - pDoc->m_pTMOVE->m_vRECT.top);

				pDoc->m_pTMOVE->m_vRECT.OffsetRect(
					pDoc->m_pTMOVE->m_vRECT.left / theApp.m_nSNAP * theApp.m_nSNAP - pDoc->m_pTMOVE->m_vRECT.left,
					pDoc->m_pTMOVE->m_vRECT.top / theApp.m_nSNAP * theApp.m_nSNAP - pDoc->m_pTMOVE->m_vRECT.top);

				RegulateTJOINTRect(&pDoc->m_pTMOVE->m_vRECT);
				Invalidate(FALSE);
			}

			break;
		}
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CTPathTimeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTPathTimeDoc *pDoc = GetDocument();

	if( pDoc && pDoc->m_pTMOVE )
	{
		pDoc->m_pTMOVE = NULL;
		ClipCursor(NULL);
		ReleaseCapture();
		Invalidate(FALSE);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTPathTimeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTPathTimeDoc *pDoc = GetDocument();

	if(pDoc)
	{
		switch(pDoc->m_bEditMode)
		{
		case TEDIT_INFO_TSPOT	:
		case TEDIT_ADD_TSPOT	:
		case TEDIT_DELETE_TSPOT	: pDoc->m_bEditMode = TEDIT_INFO_TSPOT; break;
		case TEDIT_MOVE_JOINT	:
		case TEDIT_ADD_JOINT	:
		case TEDIT_DELETE_JOINT	: pDoc->m_bEditMode = TEDIT_EDIT_JOINT; break;
		}

		pDoc->m_vTSPOTSEL.clear();
		Invalidate(FALSE);
	}

	CScrollView::OnRButtonDown(nFlags, point);
}
