// TButton.cpp: implementation of the TButton class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


DWORD TButton::m_dwHOVERSND = 0;
DWORD TButton::m_dwDOWNSND = 0;
DWORD TButton::m_dwUPSND = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TButton::TButton(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_BUTTON;
	m_bState = TBUTTON_STATE_NULL;
	m_bImageIndex = 0;

	m_pUp =	FindKid(pDesc->m_vCOMP.m_vEX.button.up);
	m_pDown = FindKid(pDesc->m_vCOMP.m_vEX.button.down);
	m_pHover = FindKid(pDesc->m_vCOMP.m_vEX.button.hover);
	m_bSND = TRUE;

	CSize size = m_rc.Size();
	if(0==size.cx || 0==size.cy)
	{
		TComponent *pRep = NULL;;
		if(m_pUp)
			pRep = m_pUp;
		else if(m_pDown)
			pRep = m_pDown;
		else if(m_pHover)
			pRep = m_pHover;

		if(pRep)
		{
			m_rc.right = m_rc.left;
			m_rc.bottom = m_rc.top;

			pRep->GetComponentSize(&size);
			m_rc.right += size.cx;
			m_rc.bottom += size.cy;
		}		
	}

	m_bTwinkle = FALSE;
	m_bTwinkleUpdate = FALSE;
	m_wTwinkleGap = 500;
	m_dwTwinkleAccTime = 0;
}

TButton::~TButton()
{

}

HRESULT TButton::Render( DWORD dwTickCount )
{
	if( !IsVisible() )
		return S_OK;

	if( m_bTwinkleUpdate )
		UpdateTwinkle( dwTickCount );

	if( m_pHover )
	{
		if(m_bState & TBUTTON_STATE_DOWN)
		{
			m_pHover->ShowComponent( FALSE );
			if( m_pUp ) m_pUp->ShowComponent( FALSE );
			if( m_pDown ) m_pDown->ShowComponent( TRUE );		
		}
		else if(m_bState & TBUTTON_STATE_HOVER)
		{
			m_pHover->ShowComponent( TRUE );
			if( m_pUp ) m_pUp->ShowComponent( FALSE );
			if( m_pDown ) m_pDown->ShowComponent( FALSE );		
		}
		else
		{
			m_pHover->ShowComponent( FALSE );
			if( m_pUp ) m_pUp->ShowComponent( TRUE );
			if( m_pDown ) m_pDown->ShowComponent( FALSE );					
		}
	}
	else
	{
		if(m_bState & TBUTTON_STATE_DOWN)
		{
			if( m_pUp ) m_pUp->ShowComponent( FALSE );
			if( m_pDown ) m_pDown->ShowComponent( TRUE );		
			
		}		
		else
		{
			if( m_pUp ) m_pUp->ShowComponent( TRUE );
			if( m_pDown ) m_pDown->ShowComponent( FALSE );					
		}	
	}

	if( m_pHover && m_pHover->IsTypeOf( TCML_TYPE_IMAGELIST ) )
		((TImageList*) m_pHover)->SetCurImage( m_bImageIndex );

	if( m_pDown && m_pDown->IsTypeOf( TCML_TYPE_IMAGELIST ) )
		((TImageList*) m_pDown)->SetCurImage( m_bImageIndex );

	if( m_pUp && m_pUp->IsTypeOf( TCML_TYPE_IMAGELIST ) )
		((TImageList*) m_pUp)->SetCurImage( m_bImageIndex );

	return TComponent::Render( dwTickCount );
}

void TButton::OnLButtonDblClk(UINT nFlags, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt)) return;
	SAFE_NOTIFY(m_id, TNM_DBLCLICK, (LPVOID)&m_index);
}

void TButton::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return;

	if(m_style&TBS_STATE_BUTTON)
	{//State button
		m_bState ^= TBUTTON_STATE_DOWN;
		SAFE_NOTIFY(m_id, TNM_LCLICK, (LPVOID)&m_index);
	}
	else
	{//Push button
		m_bState |=TBUTTON_STATE_DOWN;
		if(m_bState&TBUTTON_STATE_HOVER)
			m_bState &= (~TBUTTON_STATE_HOVER);
	}

	if(m_bSND)
		if( m_pDown && m_pDown->m_dwMediaID != 0 )
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_pDown->m_dwMediaID);
		else if(m_dwDOWNSND)
			CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwDOWNSND);
}

void TButton::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return;

	if( !(m_style&TBS_STATE_BUTTON) &&
		(m_bState&TBUTTON_STATE_DOWN) )
	{
		m_bState &= (~TBUTTON_STATE_DOWN);
		m_bState |= TBUTTON_STATE_HOVER;
		SAFE_NOTIFY(m_id, TNM_LCLICK, (LPVOID)&m_index);

		if(m_bSND)
			if( m_pUp && m_pUp->m_dwMediaID != 0 )
				CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_pUp->m_dwMediaID);
			else if(m_dwUPSND)
				CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwUPSND);
	}
}

void TButton::OnMouseMove(UINT nFlags, CPoint pt)
{
	if( !CanProcess())
		return;

	BOOL bHit = HitTest(pt);

	if( m_bTwinkle )
	{
		m_bTwinkleUpdate = !bHit;

		if( m_bTwinkleUpdate )
			return ;
	}
	
	if( !IsVisible() )
		return ;

	if( !bHit )
	{
		m_bState &= (~TBUTTON_STATE_HOVER);

		if( !(m_style & TBS_STATE_BUTTON) )
			m_bState &= (~TBUTTON_STATE_DOWN);
	}
	else if( !(m_bState & TBUTTON_STATE_DOWN) && !(m_bState & TBUTTON_STATE_HOVER) && m_pHover )
	{
		m_bState |= TBUTTON_STATE_HOVER;

		if(m_bSND)
			if(m_pHover->m_dwMediaID)
				CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_pHover->m_dwMediaID);
			else if(m_dwHOVERSND)
				CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, m_dwHOVERSND);
	}
}

void TButton::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt)) return;

	if(m_style&TBS_STATE_BUTTON)
	{//State button
		m_bState ^= TBUTTON_STATE_DOWN;
		SAFE_NOTIFY(m_id, TNM_RCLICK, (LPVOID)&m_index);
	}
	else
	{//Push button
		m_bState |=TBUTTON_STATE_DOWN;
		if(m_bState&TBUTTON_STATE_HOVER)
			m_bState &= (~TBUTTON_STATE_HOVER);
	}	
}

void TButton::OnRButtonUp(UINT nFlags, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt)) return;

	if( (!(m_style & TBS_STATE_BUTTON)) &&
		(m_bState&TBUTTON_STATE_DOWN) )
	{
		m_bState &= (~TBUTTON_STATE_DOWN);
		SAFE_NOTIFY(m_id, TNM_RCLICK, (LPVOID)&m_index);
	}
}	

BOOL TButton::IsStateDown()
{
	return (m_bState &TBUTTON_STATE_DOWN);
}

void TButton::SetPushButton()
{
	m_style &= (~TBS_STATE_BUTTON);	
}

void TButton::SetStateButton()
{
	m_style |= TBS_STATE_BUTTON;
}

BYTE TButton::GetState()
{
	return m_bState;
}

void TButton::Select( BOOL bSelect )
{
	if( !m_bEnable )
		return ;

	if(bSelect)
		m_bState |= TBUTTON_STATE_DOWN;
	else
		m_bState &= (~TBUTTON_STATE_DOWN);
}

void TButton::Select( BYTE bState )
{
	m_bState = bState;
}

void TButton::SetImageIndex( int nIndex )
{
	m_bImageIndex = nIndex;
}

BOOL TButton::HitTest( CPoint pt)
{
	BOOL bHit = FALSE;

	if(!m_bVisible || m_bNoHIT)
		return bHit;

	if( m_pUp && m_pUp->IsVisible() )
		bHit |= m_pUp->HitTest(pt);

	if( m_pDown && m_pDown->IsVisible() )
		bHit |= m_pDown->HitTest(pt);

	if( m_pHover && m_pHover->IsVisible() )
		bHit |= m_pHover->HitTest(pt);

	return bHit || TComponent::HitTest(pt) ? TRUE : FALSE;
}

void TButton::EnableComponent( BOOL bEnable)
{
    m_bState &= (~TBUTTON_STATE_HOVER);
	TComponent::EnableComponent( bEnable );
}

void TButton::UpdateTwinkle( DWORD dwTickCount )
{
	m_dwTwinkleAccTime += dwTickCount;

	if( m_dwTwinkleAccTime > m_wTwinkleGap )
	{		
		if( (GetState()&TBUTTON_STATE_HOVER) == TBUTTON_STATE_HOVER)
			Select( (BYTE)0 );
		else
			Select( (BYTE)TBUTTON_STATE_HOVER);

		m_dwTwinkleAccTime = 0;
	}
}