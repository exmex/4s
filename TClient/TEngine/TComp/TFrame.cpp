// TFrame.cpp: implementation of the TFrame class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTachyonWnd *TFrame::m_pCommandHandler = NULL;
DWORD TFrame::m_dwCLOSESND = 0;
DWORD TFrame::m_dwOPENSND = 0;


TFrame::TFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_FRAME;
	m_style |= TS_FOCUSABLE;

	m_pFocus = NULL;
	m_blDragging = FALSE;
	m_bSND = TRUE;
	m_ptPrev = CPoint( 0, 0 );
	m_bTabAutoNext = TRUE;
}

TFrame::~TFrame()
{
}

/*
	Message handlers
*/
void TFrame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	BOOL blHitAtLeastOnce = FALSE;
	TComponent *pACTIVE = NULL;
	TComponent *pFOCUS = NULL;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);

		if( pKid && !pKid->IsTypeOf(TCML_TYPE_COMPONENT) && pKid->CanProcess() && pKid->HitTest(pt) )
		{
			blHitAtLeastOnce = TRUE;
			m_tip.Setup( pt, pKid, GetTickCount());

			if(pKid->CanFocus())
				pFOCUS = pKid;
			pACTIVE = pKid;
		}
	}

	if(pFOCUS)
		SwitchFocus(pFOCUS);

	if(pACTIVE)
		pACTIVE->OnLButtonDown( nFlags, pt);

	if( !blHitAtLeastOnce &&
		m_style & TFS_FLOAT )
	{
		m_blDragging = TRUE;
		m_ptPrev = pt;
	}
}

void TFrame::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	if( m_blDragging &&
		m_style & TFS_FLOAT )
		m_blDragging = FALSE;
	else
	{
		TCOMP_LIST::iterator it = GetFirstKidsFinder();
		while(!EndOfKids(it))
		{
			TComponent* pKid = GetNextKid(it);
			if( pKid )
				pKid->OnLButtonUp(nFlags, pt);
		}
	}
}

void TFrame::OnLButtonDblClk( UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
		{
			pKid->OnLButtonDblClk( nFlags, pt);
			break;
		}
	}
}

void TFrame::OnMButtonDown( UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
		{
			pKid->OnMButtonDown( nFlags, pt);
			break;
		}
	}
}

void TFrame::OnMButtonUp( UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid )
			pKid->OnMButtonUp(nFlags, pt);
	}
}

void TFrame::OnMButtonDblClk( UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
		{
			pKid->OnMButtonDblClk( nFlags, pt);
			break;
		}
	}
}

void TFrame::OnRButtonDown( UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
		{
			pKid->OnRButtonDown( nFlags, pt);
			break;
		}
	}
}

void TFrame::OnRButtonUp(UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid )
			pKid->OnRButtonUp(nFlags, pt);
	}
}

void TFrame::OnRButtonDblClk( UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
		{
			pKid->OnRButtonDblClk( nFlags, pt);
			break;
		}
	}
}

void TFrame::OnMouseMove( UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;
		
	if( m_blDragging &&
		m_style & TFS_FLOAT )
	{
		CPoint ptFrame, ptDiffer;
		GetComponentPos( &ptFrame );

		ptDiffer = pt - m_ptPrev;
		m_ptPrev = pt;
		ptFrame += ptDiffer;

		MoveComponent( ptFrame );
	}
	else
	{
		TCOMP_LIST::reverse_iterator it;
		m_tip.Reset();

		for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
		{
			TComponent *pKid = (*it);

			if(pKid)
			{
				pKid->OnMouseMove( nFlags, pt);

				if(pKid->HitTest(pt))
				{
					m_tip.Setup( pt, pKid, GetTickCount());
				}
			}
		}
	}
}

BOOL TFrame::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(!CanProcess())
		return FALSE;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->DoMouseWheel( nFlags, zDelta, pt) )
			return TRUE;
	}

	return TComponent::DoMouseWheel( nFlags, zDelta, pt);
}

BOOL TFrame::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt)
{
	if(!CanProcess())
		return FALSE;

	TCOMP_LIST::reverse_iterator it;
	for( it = m_kids.rbegin(); it != m_kids.rend(); it++)
	{
		TComponent *pKid = (*it);

		if( pKid && pKid->HitTest(pt) )
			return pKid->OnSetCursor( pWnd, nHitTest, message, pt);
	}

	return TComponent::OnSetCursor( pWnd, nHitTest, message, pt);
}

void TFrame::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid &&
			pKid->GetFocus())
			pKid->OnChar(nChar, nRepCnt, nFlags);
	}

	TComponent::OnChar( nChar, nRepCnt, nFlags);
}

void TFrame::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid &&
			pKid->GetFocus())
			pKid->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	if( nChar == VK_TAB )
	{
		if( m_bTabAutoNext )
		{
			BOOL bContinue = TRUE;
			TComponent *pFirst = NULL, *pCandidate = NULL, *pPrev = NULL;

			it = GetFirstKidsFinder();
			while(!EndOfKids(it) && bContinue)
			{
				TComponent* pKid = GetNextKid(it);
				if( pKid && pKid->CanFocus() )
				{
					if(!pFirst) pFirst = pKid;
					
					if( pPrev == m_pFocus )
					{
						bContinue = FALSE;
						pCandidate = (TEdit *)pKid;
					}

					pPrev = pKid;
				}
			}

			if(!pCandidate)
				pCandidate = pFirst;

			SwitchFocus(pCandidate);
		}
		else
		{
			if( m_pFocus->m_pNextTab )
				SwitchFocus( m_pFocus->m_pNextTab );
		}
	}
}

void TFrame::OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid &&
			pKid->GetFocus())
			pKid->OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

void TFrame::OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid &&
			pKid->GetFocus())
			pKid->OnImeComposition(hWnd, wParam, lParam);
	}
}

void TFrame::OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid &&
			pKid->GetFocus())
			pKid->OnImeNotify(hWnd, wParam, lParam);
	}
}

BOOL TFrame::OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	if(!CanProcess())
		return TRUE;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent* pKid = GetNextKid(it);
		if( pKid && pKid->GetFocus() && !	pKid->OnInputLangChangeRequest(hWnd, wParam, lParam) )
			return FALSE;
	}

	return TRUE;
}

void TFrame::OnSetFocus( TComponent* pLoseFocus)
{
	if(m_pFocus)
	{
		m_pFocus->SetFocus(TRUE);
		m_pFocus->OnSetFocus(pLoseFocus);
	}

	TComponent::OnSetFocus(pLoseFocus);
}

void TFrame::OnLoseFocus( TComponent* pSetFocus)
{
	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pSetFocus);
	}

	TComponent::OnLoseFocus(pSetFocus);
}

void TFrame::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	SAFE_NOTIFY( from, msg, param);

	if( msg != TNM_BASE )
	{
		TComponent *pFrom = FindKid(from);

		if( m_pCommandHandler &&
			pFrom && pFrom->m_menu[msg] != TCML_ID_NULL )
			m_pCommandHandler->m_vCOMMAND.push_back(pFrom->m_menu[msg]);
	}
}

HRESULT TFrame::DrawKids( DWORD dwTickCount )
{
	if(!CTachyonRes::m_pDEVICE)
		return E_FAIL;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	HRESULT hr = S_OK;

	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);

		if(pKid)
		{
			hr = pKid->Render(dwTickCount);

			if(FAILED(hr))
				return hr;
		}
	}

	return hr;
}

DWORD TFrame::SetFocus( DWORD dwID)
{
	DWORD dwReturn = m_pFocus ? m_pFocus->m_id : (DWORD) -1;

	SwitchFocus(FindKid(dwID));
	return dwReturn;
}

void TFrame::SwitchFocus(TComponent *pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	RemoveKid(pCandidate);
	AddKid(pCandidate);

	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if(pCandidate)
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}

void TFrame::SetCommandHandler(CTachyonWnd *pCmdHandler)
{
	m_pCommandHandler = pCmdHandler;
}

HRESULT TFrame::Render( DWORD dwTickCount)
{
	if(!m_bVisible)
		return S_OK;

	HRESULT hr = TComponent::Render(dwTickCount);
	if( hr == S_OK && m_tip.m_ptr &&
		m_tip.m_ptr->Get3DTipFont() )
	{
		CString strTooltip;

		m_tip.GetTooltipText(strTooltip);
		if( !strTooltip.IsEmpty() &&
			m_tip.IsVisible(GetTickCount()) )
		{
			m_tip.m_ptr->Get3DTipFont()->TextOut(
				CTachyonRes::m_pDEVICE->m_pDevice,
				strTooltip,
				m_tip.m_pt.x,
				m_tip.m_pt.y,
				TRUE);
		}
	}

	return hr;
}

void TFrame::EnableFloat( BOOL bFloatable)
{
	SetStyle( (bFloatable) ? GetStyle() | TFS_FLOAT : GetStyle() & (~TFS_FLOAT) );
}

BOOL TFrame::HitTest( CPoint pt)
{
	if(m_bVisible && !m_bNoHIT)
	{
		if( CanProcess() )
			return HitTest(pt, m_pEnable);
		else
			return HitTest(pt, m_pDisable);
	}

	return FALSE;
}

BOOL TFrame::HitTest( CPoint pt, LPIMAGESET pImgset)
{
	CRect rc;
	GetComponentRectWithKid(&rc);
	ComponentToScreen(&rc);

	if( rc.PtInRect(pt) )
	{
		if( pImgset )
		{
			pImgset->m_dwCurTick = m_dwTotalTick % m_pEnable->m_dwTotalTick;
			CD3DImage *pImage = pImgset->GetImage();

			if( pImage )
			{
				CPoint point = pt;
				InComponentScreenPt(&point);

				if(pImage->GetMask( point.x, point.y))
					return TRUE;
			}
		}

		TCOMP_LIST::iterator it;
		for( it=m_kids.begin(); it!=m_kids.end(); it++)
		{
			TComponent *pKid = *it;

			if( pKid->HitTest(pt) )
				return TRUE;
		}
	}

	return FALSE;
}

void TFrame::ShowComponent( BOOL bVisible)
{
	if( m_bVisible != bVisible && m_bSND )
		if( bVisible && m_dwOPENSND )
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwOPENSND);
		else if( !bVisible && m_dwCLOSESND )
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwCLOSESND);

	TComponent::ShowComponent(bVisible);
}
