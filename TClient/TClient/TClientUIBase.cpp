#include "StdAfx.h"

BYTE CTClientUIBase::m_bDragLock = FALSE;
CRect CTClientUIBase::m_rtDEFAULT_SCREEN = CRect(0,0,TBASE_SCREEN_X, TBASE_SCREEN_Y);
CPoint CTClientUIBase::m_vBasis[9] =
{
	CPoint( 0, 0 ),
	CPoint( TBASE_SCREEN_X/2, 0 ),
	CPoint( TBASE_SCREEN_X, 0 ),
	
	CPoint( 0, TBASE_SCREEN_Y/2 ),
	CPoint( TBASE_SCREEN_X/2, TBASE_SCREEN_Y/2 ),
	CPoint( TBASE_SCREEN_X, TBASE_SCREEN_Y/2 ),

	CPoint( 0, TBASE_SCREEN_Y ),
	CPoint( TBASE_SCREEN_X/2, TBASE_SCREEN_Y ),
	CPoint( TBASE_SCREEN_X, TBASE_SCREEN_Y )
};

CPoint CTClientUIBase::m_vDefaultBASIS[9] =
{
	CPoint( 0, 0 ),
	CPoint( TBASE_SCREEN_X/2, 0 ),
	CPoint( TBASE_SCREEN_X, 0 ),
	
	CPoint( 0, TBASE_SCREEN_Y/2 ),
	CPoint( TBASE_SCREEN_X/2, TBASE_SCREEN_Y/2 ),
	CPoint( TBASE_SCREEN_X, TBASE_SCREEN_Y/2 ),

	CPoint( 0, TBASE_SCREEN_Y ),
	CPoint( TBASE_SCREEN_X/2, TBASE_SCREEN_Y ),
	CPoint( TBASE_SCREEN_X, TBASE_SCREEN_Y )
};

void CTClientUIBase::ResetBasis( INT nWidth, INT nHeight)
{
	CTClientUIBase::m_vBasis[TBASISPOINT_LEFT_TOP] = CPoint( 0, 0 );
	CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_TOP] = CPoint( nWidth/2, 0 );
	CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_TOP] = CPoint( nWidth, 0 );

	CTClientUIBase::m_vBasis[TBASISPOINT_LEFT_MIDDLE] = CPoint( 0, nHeight/2 );
	CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_MIDDLE] = CPoint( nWidth/2, nHeight/2 );
	CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_MIDDLE] = CPoint( nWidth, nHeight/2 );

	CTClientUIBase::m_vBasis[TBASISPOINT_LEFT_BOTTOM] = CPoint( 0, nHeight );
	CTClientUIBase::m_vBasis[TBASISPOINT_CENTER_BOTTOM] = CPoint( nWidth/2, nHeight );
	CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_BOTTOM] = CPoint( nWidth, nHeight );
}

CRect CTClientUIBase::GetScreenRect()
{
	return CRect( CPoint(0,0), CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_BOTTOM] );
}

CPoint CTClientUIBase::GetUIBasisPos( BYTE bBasisPoint, CPoint vOffset )
{
	return CTClientUIBase::m_vBasis[ bBasisPoint ] + vOffset;
}

void CTClientUIBase::GetUIOffset( CRect rtScreen, CRect rtComp, TFRAMEOFFSET* pFRAMEOFFSET )
{
	CRect rcDrag(rtScreen);

	CPoint vCenter(
		rtComp.left + rtComp.Width()/2 + rcDrag.left,
		rtComp.top + rtComp.Height()/2 + rcDrag.top ); // rcDrag 좌표계에서 현 컴포넌트의 중앙점 위치값.

	FLOAT unit_w = rcDrag.Width() / 3.0f;
	FLOAT unit_h = rcDrag.Height() / 3.0f;

	INT unit_x = (INT)( FLOAT(vCenter.x) / unit_w );
	INT unit_y = (INT)( FLOAT(vCenter.y) / unit_h );

	static BYTE vBASIS[][3] =
	{
		{ TBASISPOINT_LEFT_TOP, TBASISPOINT_LEFT_MIDDLE, TBASISPOINT_LEFT_BOTTOM },
		{ TBASISPOINT_CENTER_TOP, TBASISPOINT_CENTER_MIDDLE, TBASISPOINT_CENTER_BOTTOM },
		{ TBASISPOINT_RIGHT_TOP, TBASISPOINT_RIGHT_MIDDLE, TBASISPOINT_RIGHT_BOTTOM },
	};

	CPoint vBasis;

	switch( unit_x )
	{
	case 0: // TBASISPOINT_LEFT_X
			vBasis.x = rcDrag.left;
		break;
	case 1: // TBASISPOINT_CENTER_X
			vBasis.x = rcDrag.left + rcDrag.Width()/2;
		break;
	case 2: // TBASISPOINT_RIGHT_X
			vBasis.x = rcDrag.right;
		break;
	default:
			unit_x = 0;
			vBasis.x = rcDrag.left;
		break;
	}

	switch( unit_y )
	{
	case 0:
		vBasis.y = rcDrag.top;
		break;
	case 1:
		vBasis.y = rcDrag.top + rcDrag.Height()/2;
		break;
	case 2:
		vBasis.y = rcDrag.bottom;
		break;
	default:
		unit_y = 0;
		vBasis.y = rcDrag.top;
		break;
	}

	CPoint vCompOffset = CPoint(rtComp.left, rtComp.top)-vBasis;

	if( pFRAMEOFFSET)
	{
		pFRAMEOFFSET->m_bBasisPoint = vBASIS[unit_x][unit_y];
		pFRAMEOFFSET->m_vOffset = vCompOffset;
	}
}

CTClientUIBase::CTClientUIBase( TComponent *pParent, LP_FRAMEDESC pDesc)
:TFrame( pParent, pDesc), m_rcDrag(0,0,0,0)
{
	m_bDropTarget = TRUE;
	m_bCanDrag = FALSE;
	m_bDraging = FALSE;

	m_bBasisPoint = TBASISPOINT_COUNT;
	m_bUseBasis = TRUE;
	m_vCompOffset.x = 0;
	m_vCompOffset.y = 0;
}

CTClientUIBase::~CTClientUIBase()
{
}

ITDetailInfoPtr CTClientUIBase::GetTInfoKey( const CPoint& point )
{
	return ITDetailInfoPtr(NULL);
}

TDROPINFO CTClientUIBase::OnDrop( CPoint point)
{
	TDROPINFO vResult;
	return vResult;
}

BYTE CTClientUIBase::OnBeginDrag( LPTDRAG pDRAG,
								  CPoint point)
{
	return FALSE;
}

BOOL CTClientUIBase::GetTChatInfo( const CPoint& point, TCHATINFO& outInfo )
{
	return FALSE;
}

void CTClientUIBase::OnLButtonDown(UINT nFlags, CPoint pt)
{
	m_bDraging = !m_bDragLock && m_bCanDrag && HitTest(pt);
	m_ptDrag = pt;

	TFrame::OnLButtonDown(nFlags,pt);
}

void CTClientUIBase::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if(m_bDraging)
	{
		NotifyComponentUpdate();
		m_bDraging = FALSE;
	}

	TFrame::OnLButtonUp(nFlags,pt);
}

void CTClientUIBase::OnMouseMove(UINT nFlags, CPoint pt)
{
	if(m_bDraging)
	{
		m_ptDrag.x -= pt.x;
		m_ptDrag.y -= pt.y;

		CRect rect = m_rc;
		INT w =  rect.Width();
		INT h =  rect.Height();

		rect.left -= m_ptDrag.x;
		rect.top -= m_ptDrag.y;

		if( rect.left < m_rcDrag.left )
			rect.left = m_rcDrag.left;
		else if( rect.left+w > m_rcDrag.right )
			rect.left = m_rcDrag.right-w;

		if( rect.top < m_rcDrag.top )
			rect.top = m_rcDrag.top;
		else if( rect.top+h > m_rcDrag.bottom )
			rect.top = m_rcDrag.bottom-h;

		MoveComponent( CPoint(rect.left, rect.top) );

		m_ptDrag = pt;
	}

	TFrame::OnMouseMove(nFlags,pt);
}

UINT CTClientUIBase::GetUniqueID()
{
	UINT nResult;
	do { nResult = (UINT)::rand(); }
	while( FindKid(nResult) );
	return nResult;
}

BOOL CTClientUIBase::CanWithItemUI()
{
	return FALSE;
}

BOOL CTClientUIBase::CanDisable()
{
	return TRUE;
}

void CTClientUIBase::DefaultPosition( CPoint* vBASIS, BOOL bRestore )
{
}

void CTClientUIBase::ResetPosition()
{
	MoveComponent( m_vBasis[m_bBasisPoint] + m_vCompOffset );	
}

void CTClientUIBase::MoveComponent( CPoint pt )
{
	TComponent::MoveComponent(pt);

	if( m_bUseBasis && m_vBasis[m_bBasisPoint] + m_vCompOffset != pt )
	{
		CRect rcDrag( CRect( CPoint(0,0), m_vBasis[TBASISPOINT_RIGHT_BOTTOM] ) );

		CPoint vCenter(
			m_rc.left + m_rc.Width()/2 + rcDrag.left,
			m_rc.top + m_rc.Height()/2 + rcDrag.top ); // rcDrag 좌표계에서 현 컴포넌트의 중앙점 위치값.

		FLOAT unit_w = rcDrag.Width() / 3.0f;
		FLOAT unit_h = rcDrag.Height() / 3.0f;

		INT unit_x = (INT)( FLOAT(vCenter.x) / unit_w );
		INT unit_y = (INT)( FLOAT(vCenter.y) / unit_h );

		if( 0 <= unit_x && unit_x < 3 &&
			0 <= unit_y && unit_y < 3 )
		{
			static BYTE vBASIS[][3] =
			{
				{ TBASISPOINT_LEFT_TOP, TBASISPOINT_LEFT_MIDDLE, TBASISPOINT_LEFT_BOTTOM },
				{ TBASISPOINT_CENTER_TOP, TBASISPOINT_CENTER_MIDDLE, TBASISPOINT_CENTER_BOTTOM },
				{ TBASISPOINT_RIGHT_TOP, TBASISPOINT_RIGHT_MIDDLE, TBASISPOINT_RIGHT_BOTTOM },
			};

			m_bBasisPoint = vBASIS[unit_x][unit_y];
		}
	}

	m_vCompOffset = m_rc.TopLeft() - m_vBasis[m_bBasisPoint];
}
