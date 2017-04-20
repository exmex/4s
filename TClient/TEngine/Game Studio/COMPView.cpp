// COMPView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "COMPView.h"
#include "MainFrm.h"
#include ".\compview.h"


// CCOMPView

IMPLEMENT_DYNCREATE(CCOMPView, CGuideView)

CCOMPView::CCOMPView()
{
}

CCOMPView::~CCOMPView()
{
}

BEGIN_MESSAGE_MAP(CCOMPView, CGuideView)
END_MESSAGE_MAP()


// CCOMPView 진단입니다.

#ifdef _DEBUG
void CCOMPView::AssertValid() const
{
	CGuideView::AssertValid();
}

void CCOMPView::Dump(CDumpContext& dc) const
{
	CGuideView::Dump(dc);
}
#endif //_DEBUG


// CCOMPView 메시지 처리기입니다.

void CCOMPView::DrawContent()
{
	CCOMPDoc *pDoc = GetDocument();

	if(pDoc)
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);
		
		pDoc->m_pItem->Render(
			0, FALSE,
			-nPosX,
			-nPosY );

		if( pDoc->m_pItem->m_nWidth != 0 &&
			pDoc->m_pItem->m_nHeight != 0 )
		{
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TNLPOINT);

			CRect rect;
			GetClientRect(&rect);
			rect.bottom -= GNC_SIZE_Y;
			rect.right -= GNC_SIZE_X;

			TNLPOINT vPoint[5] = {
				{ ((FLOAT) ( 0-nPosX)), ((FLOAT) 0-nPosY), 0.0f, 1.0f, 0xFFFF0000 },
				{ ((FLOAT) ( pDoc->m_pItem->m_nWidth-nPosX)), ((FLOAT) 0-nPosY), 0.0f, 1.0f, 0xFFFF0000 },
				{ ((FLOAT) ( pDoc->m_pItem->m_nWidth-nPosX)), ((FLOAT) pDoc->m_pItem->m_nHeight-nPosY), 0.0f, 1.0f, 0xFFFF0000 },
				{ ((FLOAT) ( 0-nPosX)), ((FLOAT) pDoc->m_pItem->m_nHeight-nPosY), 0.0f, 1.0f, 0xFFFF0000 },
				{ ((FLOAT) ( 0-nPosX)), ((FLOAT) 0-nPosY), 0.0f, 1.0f, 0xFFFF0000 },
			};

			CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
				D3DPT_LINESTRIP, 4,
				vPoint, sizeof(TNLPOINT));
		}
	}
}

GUIDEHIT CCOMPView::HitTest( CPoint point)
{
	GUIDEHIT vHIT = CGuideView::HitTest(point);

	if( vHIT.m_bType == GH_NONE )
	{
		CCOMPDoc *pDoc = GetDocument();

		point.x += GetScrollPos(SB_HORZ) - GNC_SIZE_X;
		point.y += GetScrollPos(SB_VERT) - GNC_SIZE_Y;

		vHIT.m_nHIT = pDoc->m_pItem->HitTest(point);
		if( vHIT.m_nHIT != -1 && vHIT.m_nHIT < INT(pDoc->m_pItem->m_vCOMP.size()) )
		{
			vHIT.m_bType = GH_ITEM;
			vHIT.m_point.x = point.x - pDoc->m_pItem->m_vCOMP[vHIT.m_nHIT]->m_nPosX;
			vHIT.m_point.y = point.y - pDoc->m_pItem->m_vCOMP[vHIT.m_nHIT]->m_nPosY;
		}
		else
			vHIT.m_nHIT = -1;
	}

	return vHIT;
}

CRect CCOMPView::GetItemRect()
{
	CCOMPDoc *pDoc = GetDocument();
	CCOMPBar *pBar = pDoc->GetCOMPBar();
	int nIndex = pBar->m_cCOMP.GetCurSel();

	if( nIndex != -1 )
	{
		CCOMPItem *pCOMP = pDoc->m_pItem->m_vCOMPITEM[nIndex];
		CPoint point(
			pDoc->m_pItem->m_vCOMP[nIndex]->m_nPosX,
			pDoc->m_pItem->m_vCOMP[nIndex]->m_nPosY);

		return CRect( point, point + pCOMP->GetSize());
	}

	return CRect( 0, 0, 0, 0);
}

CRect CCOMPView::GetItemRect( CPoint point)
{
	CCOMPDoc *pDoc = GetDocument();
	CCOMPItem *pCOMP = pDoc->m_pItem->m_vCOMPITEM[m_vHIT.m_nHIT];

	return CRect( point, point + pCOMP->GetSize());
}

CRect CCOMPView::GetItemRect(int nCnt)
{
	CCOMPDoc *pDoc = GetDocument();
	CPoint point(
			pDoc->m_pItem->m_vCOMP[nCnt]->m_nPosX,
			pDoc->m_pItem->m_vCOMP[nCnt]->m_nPosY);
	
	return CRect( point, point + pDoc->m_pItem->m_vCOMPITEM[nCnt]->GetSize());
}

void CCOMPView::DoMoveItem( CPoint point)
{
	CCOMPDoc *pDoc = GetDocument();
	CRect rect = GetSelectItemRECT();
	CPoint pos(0,0);

	if( rect == NULL )
		return;

	if( pDoc->m_vSEL.size() == 1 )
	{
		pDoc->m_pItem->m_vCOMP[m_vHIT.m_nHIT]->m_nPosX = max( point.x, 0);
		pDoc->m_pItem->m_vCOMP[m_vHIT.m_nHIT]->m_nPosY = max( point.y, 0);
	}
	else 
	{
		BOOL bMoveX = TRUE;
		BOOL bMoveY = TRUE;		
    			
		CPoint center = rect.CenterPoint();

		pos = CPoint( point.x - pDoc->m_ptPreItemPos.x, point.y - pDoc->m_ptPreItemPos.y);
		 

		if( rect.left + pos.x < 0 )
			bMoveX = FALSE;
		if( rect.top + pos.y < 0 )
			bMoveY = FALSE;

		VECTORINT::iterator it;
		for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end(); it++ )
		{
			if( bMoveX )
				pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosX = max( pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosX + pos.x, 0 );
			if( bMoveY )
				pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosY = max( pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosY + pos.y, 0 );
		}
	}
}

BYTE CCOMPView::CanDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE || pItem->m_bType == ITEM_COMP ? TRUE : FALSE;
}

void CCOMPView::AddItem( CItemBase *pItem)
{
	CCOMPBar *pBar = GetDocument()->GetCOMPBar();

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
				LPCOMP pCOMP = new COMP();

				point.x -= GNC_SIZE_X - GetScrollPos(SB_HORZ);
				point.y -= GNC_SIZE_Y - GetScrollPos(SB_VERT);

				pCOMP->m_dwCompID = pItem->m_dwID;
				pCOMP->m_nPosX = point.x;
				pCOMP->m_nPosY = point.y;
				pCOMP->m_bUseage = 0;

				pBar->InsertCOMP( pCOMP, -1);
			}
		}
		break;
	}
}

void CCOMPView::HitTestRect(CRect rect)
{
	CRect rectResult;
	CCOMPDoc *pDoc = GetDocument();
	
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
			
			int x = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->m_nPosX;
			int y = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->m_nPosY;
			CSize size = pDoc->m_pItem->m_vCOMPITEM[nCnt - i - 1]->GetSize();
			
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
			int x = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->m_nPosX;
			int y = pDoc->m_pItem->m_vCOMP[nCnt - i - 1]->m_nPosY;
			CSize size = pDoc->m_pItem->m_vCOMPITEM[nCnt - i - 1]->GetSize();
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
BOOL CCOMPView::IsSelectedItem(int nSelected)
{
	CCOMPDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
			return TRUE;
	}
	return FALSE;
}

void CCOMPView::DeleteSelectedITEM(int nSelected)
{
	CCOMPDoc *pDoc = GetDocument();
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

void CCOMPView::InsertSelectITEM(int nSelect)
{
	CCOMPDoc *pDoc = GetDocument();
	pDoc->m_vSEL.push_back( nSelect );
}

CRect CCOMPView::GetSelectItemRECT(void)
{
	CCOMPDoc *pDoc = GetDocument();
	CRect rt;
	rt.SetRect(0,0,0,0);
	VECTORINT::iterator it;
	
	if( pDoc->m_vSEL.empty() )
		return NULL;

	it = pDoc->m_vSEL.begin();

	if( 1 == pDoc->m_vSEL.size() )
		rt = GetItemRect( (*it) );
	else
	{
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

BOOL CCOMPView::IsPreRectInDrag(int nSelect)
{
	CCOMPDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vPreRECTinSEL.begin() ; it != pDoc->m_vPreRECTinSEL.end() ; it++ )
	{
		if( (*it) == nSelect )
			return TRUE;
	}
	return FALSE;
}

void CCOMPView::MoveItemByKeyBoard(CPoint pt)
{
	CCOMPDoc *pDoc = GetDocument();
	CCOMPBar *pBar = pDoc->GetCOMPBar();
	
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
			pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosX += pt.x;
		if( bMoveY )
			pDoc->m_pItem->m_vCOMP[(*it)]->m_nPosY += pt.y;
	}
}

CPoint CCOMPView::GetRectDist(void)
{
	CPoint point;

	CCOMPDoc *pDoc = GetDocument();
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
