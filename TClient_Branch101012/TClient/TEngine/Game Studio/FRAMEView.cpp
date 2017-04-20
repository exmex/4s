// FRAMEView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "FRAMEView.h"
#include "MainFrm.h"
#include ".\frameview.h"

#define GUIDE_DIST					10
#define _MOVE_KEYBOARD				0
#define _MOVE_MOUSE					1

// CFRAMEView

IMPLEMENT_DYNCREATE(CFRAMEView, CGuideView)

CFRAMEView::CFRAMEView()
{
}

CFRAMEView::~CFRAMEView()
{
	CGuideDoc *pDoc = GetDocument();
	pDoc->m_vSEL.clear();
	pDoc->m_vPreRECTinSEL.clear();
}

BEGIN_MESSAGE_MAP(CFRAMEView, CGuideView)
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CFRAMEView 진단입니다.

#ifdef _DEBUG
void CFRAMEView::AssertValid() const
{
	CGuideView::AssertValid();
}

void CFRAMEView::Dump(CDumpContext& dc) const
{
	CGuideView::Dump(dc);
}
#endif //_DEBUG


// CFRAMEView 메시지 처리기입니다.

void CFRAMEView::DrawContent()
{
	CFRAMEDoc *pDoc = GetDocument();
	
	if(pDoc)
	{
		pDoc->m_pItem->Render(
			-GetScrollPos(SB_HORZ),
			-GetScrollPos(SB_VERT));
	}
}

GUIDEHIT CFRAMEView::HitTest( CPoint point)
{
	GUIDEHIT vHIT = CGuideView::HitTest(point);

	if( vHIT.m_bType == GH_NONE )
	{
		CFRAMEDoc *pDoc = GetDocument();

		point.x += GetScrollPos(SB_HORZ) - GNC_SIZE_X;
		point.y += GetScrollPos(SB_VERT) - GNC_SIZE_Y;

		vHIT.m_nHIT = pDoc->m_pItem->HitTest(point);
		if( vHIT.m_nHIT != -1 && vHIT.m_nHIT < INT(pDoc->m_pItem->m_vCOMP.size()) )
		{
			vHIT.m_bType = GH_ITEM;
			vHIT.m_point.x = point.x - pDoc->m_pItem->m_vCTRL[vHIT.m_nHIT]->m_nPosX;
			vHIT.m_point.y = point.y - pDoc->m_pItem->m_vCTRL[vHIT.m_nHIT]->m_nPosY;
		}
		else
			vHIT.m_nHIT = -1;
	}

	return vHIT;
}

CRect CFRAMEView::GetItemRect()
{
	CFRAMEDoc *pDoc = GetDocument();
	CFRAMEBar *pBar = pDoc->GetFRAMEBar();
	int nIndex = pBar->m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		CCOMPItem *pCOMP = pDoc->m_pItem->m_vCOMP[nIndex];
		CPoint point(
			pDoc->m_pItem->m_vCTRL[nIndex]->m_nPosX,
			pDoc->m_pItem->m_vCTRL[nIndex]->m_nPosY);

		return CRect( point, point + pCOMP->GetSize());
	}

	return CRect( 0, 0, 0, 0);
}

CRect CFRAMEView::GetItemRect( CPoint point)
{
	CFRAMEDoc *pDoc = GetDocument();
	CCOMPItem *pCOMP = pDoc->m_pItem->m_vCOMP[m_vHIT.m_nHIT];

	return CRect( point, point + pCOMP->GetSize());
}

CRect CFRAMEView::GetItemRect(int nCnt)
{
	CFRAMEDoc *pDoc = GetDocument();
	CPoint point(
			pDoc->m_pItem->m_vCTRL[nCnt]->m_nPosX,
			pDoc->m_pItem->m_vCTRL[nCnt]->m_nPosY);
	
	return CRect( point, point + pDoc->m_pItem->m_vCOMP[nCnt]->GetSize());
}


void CFRAMEView::DoMoveItem( CPoint point)
{
	CGuideDoc *pGuidDoc = GetDocument();
	CFRAMEDoc *pDoc = GetDocument();
	CPoint ptBasic(0,0);
	CPoint pos(0,0);
	CRect rect = GetSelectItemRECT();

	if( rect == NULL )
		return;

	if( pDoc->m_vSEL.size() == 1 )
	{
		pDoc->m_pItem->m_vCTRL[m_vHIT.m_nHIT]->m_nPosX = max( point.x, 0);
		pDoc->m_pItem->m_vCTRL[m_vHIT.m_nHIT]->m_nPosY = max( point.y, 0);
	}
	else
	{
		BOOL bMoveX = TRUE;
		BOOL bMoveY = TRUE;		
    			
		CPoint center = rect.CenterPoint();

		pos = CPoint( point.x - pGuidDoc->m_ptPreItemPos.x, point.y - pGuidDoc->m_ptPreItemPos.y);
		 

		if( rect.left + pos.x < 0 )
			bMoveX = FALSE;
		if( rect.top + pos.y < 0 )
			bMoveY = FALSE;

		VECTORINT::iterator it;
		for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end(); it++ )
		{
			if( bMoveX )
				pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosX = max( pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosX + pos.x, 0 );
			if( bMoveY )
				pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosY = max( pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosY + pos.y, 0 );
		}
	}
}

BYTE CFRAMEView::CanDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE || pItem->m_bType == ITEM_COMP ? TRUE : FALSE;
}

void CFRAMEView::AddItem( CItemBase *pItem)
{
	CFRAMEBar *pBar = GetDocument()->GetFRAMEBar();

	switch(pItem->m_bType)
	{
	case ITEM_IMAGE		: pBar->SetDefault((CImageItem *) pItem); break;
	case ITEM_COMP		:
		{
			CPoint point;

			GetCursorPos(&point);
			ScreenToClient(&point);

			if( point.x >= GNC_SIZE_X &&
				point.y >= GNC_SIZE_Y )
			{
				LPCTRL pCTRL = new CTRL();

				point.x -= GNC_SIZE_X - GetScrollPos(SB_HORZ);
				point.y -= GNC_SIZE_Y - GetScrollPos(SB_VERT);

				for( int i=0; i<TCML_MENU_COUNT; i++)
					pCTRL->m_strMenuID[i].Empty();

				pCTRL->m_strToolTip.Empty();
				pCTRL->m_strCtrlID.Empty();
				pCTRL->m_strFontID.Empty();
				pCTRL->m_strTipID.Empty();
				pCTRL->m_strText.Empty();

				pCTRL->m_dwCompID = pItem->m_dwID;
				pCTRL->m_dwColor = 0xFF000000;
				pCTRL->m_bAlign = GSTA_LEFT;
				pCTRL->m_nPosX = point.x;
				pCTRL->m_nPosY = point.y;
				pCTRL->m_dwFlag = 0;

				pBar->InsertCTRL( pCTRL, -1);
			}
		}

		break;
	}
}



void CFRAMEView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
		CGuideView::OnMouseMove(nFlags, point);
}

void CFRAMEView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CGuideView::OnLButtonDown(nFlags, point);
}

void CFRAMEView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CGuideView::OnLButtonUp(nFlags, point);
}

void CFRAMEView::HitTestRect(CRect rect)
{
	CRect rectResult;
	CFRAMEDoc *pDoc = GetDocument();
	int nCnt = INT(pDoc->m_pItem->m_vCOMP.size());
	int i = 0;
	if( rect.left > rect.right )
	{
		int Temp;
		Temp = rect.left;
		rect.left = rect.right;
		rect.right = Temp;
	}
	if( rect.top > rect.bottom )
	{
		int Temp;
		Temp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = Temp;
	}

	rect.left	+= GetScrollPos(SB_HORZ); 
	rect.top	+= GetScrollPos(SB_VERT); 
	rect.right	+= GetScrollPos(SB_HORZ); 
	rect.bottom += GetScrollPos(SB_VERT); 

	if( GetAsyncKeyState(VK_LCONTROL) ) 
	{
		pDoc->m_vSEL.clear();
		pDoc->m_vSEL = pDoc->m_vPreRECTinSEL;
		for( i = 0 ; i < pDoc->m_pItem->m_vCOMP.size() ; i++ )
		{
			int x = pDoc->m_pItem->m_vCTRL[nCnt - i - 1]->m_nPosX;
			int y = pDoc->m_pItem->m_vCTRL[nCnt - i - 1]->m_nPosY;
			CSize size = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->GetSize();
			CRect reResult;
			CRect rtCOMP( x + GNC_SIZE_Y, y + GNC_SIZE_Y, x + size.cx + GNC_SIZE_Y, y + size.cy + GNC_SIZE_Y );
			reResult.IntersectRect( 
				&rect,
				&rtCOMP );
			if( !reResult.IsRectEmpty() )
			{

				if( IsPreRectInDrag(nCnt - i - 1) )
				{
					if( !IsSelectedItem(nCnt - i - 1) )
						InsertSelectITEM( nCnt - i - 1 );
					else
						DeleteSelectedITEM(  nCnt - i - 1  );

				}
				else
				{
					if( !IsSelectedItem(nCnt - i - 1) )
						InsertSelectITEM( nCnt - i - 1 );
				}
			}
		}
	}
	else
	{
		pDoc->m_vPreRECTinSEL.clear();
		pDoc->m_vSEL.clear();
		for( i = 0 ; i < pDoc->m_pItem->m_vCOMP.size() ; i++ )
		{
			int x = pDoc->m_pItem->m_vCTRL[nCnt - i - 1]->m_nPosX;
			int y = pDoc->m_pItem->m_vCTRL[nCnt - i - 1]->m_nPosY;
			CSize size = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->GetSize();
			CRect reResult;
			CRect rtCOMP( x + GNC_SIZE_Y, y + GNC_SIZE_Y, x + size.cx + GNC_SIZE_Y, y + size.cy + GNC_SIZE_Y );
			reResult.IntersectRect( 
				&rect,
				&rtCOMP );
			if( !reResult.IsRectEmpty() )
			{
				InsertSelectITEM( nCnt - i - 1 );
			}
		}
	}

	Invalidate();
}

void CFRAMEView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CGuideView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFRAMEView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CGuideView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFRAMEView::MoveItems(CPoint pt, CPoint ptMouseHit, BYTE bINPUTTYPE)
{
	
}

BOOL CFRAMEView::IsSelectedItem(int nSelected)
{
	CFRAMEDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
			return TRUE;
	}
	return FALSE;
}

void CFRAMEView::DeleteSelectedITEM(int nSelected)
{
	CFRAMEDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
		{
			pDoc->m_vSEL.erase(it);
			return;
		}
	}
}

void CFRAMEView::InsertSelectITEM(int nSelect)
{
	CFRAMEDoc *pDoc = GetDocument();
	pDoc->m_vSEL.push_back( nSelect );
}

BOOL CFRAMEView::IsPreRectInDrag(int nSelect)
{
	CFRAMEDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vPreRECTinSEL.begin() ; it != pDoc->m_vPreRECTinSEL.end() ; it++ )
	{
		if( (*it) == nSelect )
			return TRUE;
	}
	return FALSE;
}

void CFRAMEView::DeletePreRectInITEM(int nSelected)
{

}

void CFRAMEView::InsertPreRectInITEM(int nSelect)
{

}

CRect CFRAMEView::GetSelectItemRECT(void)
{
	CFRAMEDoc *pDoc = GetDocument();
	CRect rt;
	rt.SetRect(0,0,0,0);
	VECTORINT::iterator it;

	if( pDoc->m_vSEL.empty() )
		return NULL;

	if( 1 == pDoc->m_vSEL.size() )
	{
		it = pDoc->m_vSEL.begin();	
		rt = GetItemRect( (*it) );
	}
	else if( !pDoc->m_vSEL.empty() )
	{
		it = pDoc->m_vSEL.begin();
		
		rt = GetItemRect( (*it) );
		it++;
		while( it != pDoc->m_vSEL.end() )
		{
			rt.top		= min( rt.top,		GetItemRect( (*it) ).top );
			rt.bottom	= max( rt.bottom,	GetItemRect( (*it) ).bottom );
			rt.left		= min( rt.left,		GetItemRect( (*it) ).left );
			rt.right	= max( rt.right,	GetItemRect( (*it) ).right );
			it++;
		}
	}
	return rt;
}


void CFRAMEView::MoveItemByKeyBoard(CPoint pt)
{
	CFRAMEDoc *pDoc = GetDocument();
	CFRAMEBar *pBar = pDoc->GetFRAMEBar();
	BOOL bMoveX = TRUE;
	BOOL bMoveY = TRUE;

	CRect rect = GetSelectItemRECT();

	if( rect == NULL )
		return;
	
	if( rect.left + pt.x < 0 )
			bMoveX = FALSE;
	if( rect.top + pt.y < 0 )
			bMoveY = FALSE;

	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( bMoveX )
			pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosX += pt.x;
		if( bMoveY )
			pDoc->m_pItem->m_vCTRL[(*it)]->m_nPosY += pt.y;
	}

	if(pDoc->m_vSEL.size() == 1)
	{
		pBar->m_nPosX = pDoc->m_pItem->m_vCTRL[ pDoc->m_vSEL[0]]->m_nPosX;
		pBar->m_nPosY = pDoc->m_pItem->m_vCTRL[ pDoc->m_vSEL[0]]->m_nPosY;
	}
	pBar->UpdateData(FALSE);
}

CPoint CFRAMEView::GetRectDist(void)
{
	CPoint point;

	CFRAMEDoc *pDoc = GetDocument();
	CRect rectSelect;
	CRect rectHit;

	rectSelect = GetSelectItemRECT();

	rectHit = GetItemRect( m_vHIT.m_nHIT );

	CPoint centerSelect = rectSelect.CenterPoint();
	CPoint centerHit	= rectHit.CenterPoint();

	switch(pDoc->m_bHotPoint)
	{
	case GHP_TL	: return CPoint( rectHit.left - rectSelect.left , rectHit.top - rectSelect.top );
	case GHP_TC	: return CPoint( centerHit.x - centerSelect.x,  rectHit.top - rectSelect.top );
	case GHP_TR	: return CPoint( rectHit.right - rectSelect.right, rectHit.top - rectSelect.top );
	case GHP_CL	: return CPoint( rectHit.left - rectSelect.left,  centerHit.y - centerSelect.y);
	case GHP_CC	: return CPoint( centerHit.x - centerSelect.x, centerHit.y - centerSelect.y );
	case GHP_CR : return CPoint( rectHit.right - rectSelect.right, centerHit.y - centerSelect.y);
	case GHP_BL	: return CPoint( rectHit.left - rectSelect.left, rectHit.bottom - rectSelect.bottom );
	case GHP_BC : return CPoint( centerHit.x - centerSelect.x, rectHit.bottom - rectSelect.bottom);
	case GHP_BR	: return CPoint( rectHit.right - rectSelect.right, rectHit.bottom - rectSelect.bottom );
	} 

	return point;
}
