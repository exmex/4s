#include "stdafx.h"
#include "TColorPicker.h"
#include "TClientGame.h"

DWORD dwColorIcon[TCOLOR_COUNT] =
{
	0,
	ID_CTRLINST_SKIN_001,
	ID_CTRLINST_SKIN_002,
	ID_CTRLINST_SKIN_003,
	ID_CTRLINST_SKIN_004,
	ID_CTRLINST_SKIN_005,
	ID_CTRLINST_SKIN_006,
	ID_CTRLINST_SKIN_007,
	ID_CTRLINST_SKIN_008,
	ID_CTRLINST_SKIN_009,
	ID_CTRLINST_SKIN_010,
	ID_CTRLINST_SKIN_011,
	ID_CTRLINST_SKIN_012,
	ID_CTRLINST_SKIN_013,
	ID_CTRLINST_SKIN_014,
	ID_CTRLINST_SKIN_015,
	ID_CTRLINST_SKIN_016,
	ID_CTRLINST_SKIN_017,
	ID_CTRLINST_SKIN_018,
	ID_CTRLINST_SKIN_019,
	ID_CTRLINST_SKIN_020,
	ID_CTRLINST_SKIN_021,
	ID_CTRLINST_SKIN_022,
	ID_CTRLINST_SKIN_023,
	ID_CTRLINST_SKIN_024,
	ID_CTRLINST_SKIN_025,
	ID_CTRLINST_SKIN_026,
	ID_CTRLINST_SKIN_027,
	ID_CTRLINST_SKIN_028,
	ID_CTRLINST_SKIN_029,
	ID_CTRLINST_SKIN_030,
	ID_CTRLINST_SKIN_031,
	ID_CTRLINST_SKIN_032,
	ID_CTRLINST_SKIN_033,
	ID_CTRLINST_SKIN_034,
	ID_CTRLINST_SKIN_035,
	ID_CTRLINST_SKIN_036,
	ID_CTRLINST_SKIN_037,
	ID_CTRLINST_SKIN_038,
	ID_CTRLINST_SKIN_039,
	ID_CTRLINST_SKIN_040,
	ID_CTRLINST_SKIN_041,
	ID_CTRLINST_SKIN_042,
	ID_CTRLINST_SKIN_043,
	ID_CTRLINST_SKIN_044,
	ID_CTRLINST_SKIN_045,
	ID_CTRLINST_SKIN_046,
	ID_CTRLINST_SKIN_047,
	ID_CTRLINST_SKIN_048,
	ID_CTRLINST_SKIN_049
};

CTColorPicker::CTColorPicker( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_pBindTarget( NULL )
{
	for( int i=0 ; i < TCOLOR_COUNT ; ++i )
	{
		m_pTIcon[i] = (TImageList*) FindKid( dwColorIcon[i] );
		if( m_pTIcon[i] )
			m_pTIcon[i]->SetCurImage( i );
	}

	m_bRandomPickingAnimation = FALSE;
	m_dwRandomPickingTick = 0;
	m_bCanSelectColor = FALSE;
	m_wSelectedColor = 1;

	m_pText = FindKid( ID_CTRLINST_47 );
}

CTColorPicker::~CTColorPicker()
{
}

void CTColorPicker::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !CanProcess() )
		return ;

	if( !HitTest(pt) )
		return ;

	if( !m_bCanSelectColor )
		return ;

	BOOL bFind = FALSE;
	for( int i=0 ; i < TCOLOR_COUNT ; ++i )
	{
		if( m_pTIcon[i] )
			if( m_pTIcon[i]->HitTest(pt) )
			{
				m_pTIcon[i]->SetSkinImage( 50 );
				m_wSelectedColor = i;
				bFind = TRUE;
			}
			else
				m_pTIcon[i]->SetSkinImageEmpty();
	}

	if( !bFind )
	{
		m_pTIcon[1]->SetSkinImage( 50 );
		m_wSelectedColor = 1;
	}

	CTClientUIBase::OnLButtonDown( nFlags, pt );
}

HRESULT CTColorPicker::Render( DWORD dwTickCount )
{
	if( m_bVisible )
	{
		if( m_pBindTarget )
		{
			SIZE sizeMy;
			GetComponentSize( &sizeMy );

			CTClientGame* pTGAME = CTClientGame::GetInstance();
			CTClientUIBase* pTDLG = pTGAME->GetFrame( TFRAME_ITEM_UP );
			POINT ptTDLG;
			SIZE sizeTDLG;
			pTDLG->GetComponentPos( &ptTDLG );
			pTDLG->GetComponentSize( &sizeTDLG );
			ptTDLG.y += (sizeTDLG.cy-2);
			ptTDLG.x += ((sizeTDLG.cx - sizeMy.cx) / 2);

			MoveComponent( CPoint(ptTDLG) );
		}
	}

	if( m_bRandomPickingAnimation )
	{
		m_dwRandomPickingTick += dwTickCount;

		if( m_dwRandomPickingTick > 100 )
		{
			m_wSelectedColor = 1+rand()%(TCOLOR_COUNT-1);

			for( int i=0 ; i < TCOLOR_COUNT ; ++i )
			{
				if( m_pTIcon[i] )
					if( m_wSelectedColor == i )
						m_pTIcon[i]->SetSkinImage( 50 );
					else
						m_pTIcon[i]->SetSkinImageEmpty();
			}

			m_dwRandomPickingTick = 0;
		}
	}

	return CTClientUIBase::Render( dwTickCount );
}

void CTColorPicker::StartRandAnimation()
{
	m_bRandomPickingAnimation = TRUE;
	m_dwRandomPickingTick = 0;
}

void CTColorPicker::StopRandAnimation()
{
	m_bRandomPickingAnimation = FALSE;
	m_dwRandomPickingTick = 0;
}

void CTColorPicker::SetBindTarget( CTClientUIBase* pBindTarget )
{
	m_pBindTarget = pBindTarget;
}

void CTColorPicker::SetCanSelect( BOOL bCanSelect )
{
	m_bCanSelectColor = bCanSelect;

	if( m_bCanSelectColor )
		m_pText->m_strText = CTChart::LoadString( TSTR_COLOR_SELECTING );
	else
		m_pText->m_strText = CTChart::LoadString( TSTR_COLOR_RANDOM_SELECTING );
}

void CTColorPicker::StartProgress()
{
	if( !m_bCanSelectColor )
		StartRandAnimation();
	m_bCanSelectColor = FALSE;
	m_pText->m_strText.Empty();
}

void CTColorPicker::SelectColor( WORD wColor )
{
	m_wSelectedColor = wColor;
	for( int i=0 ; i < TCOLOR_COUNT ; ++i )
	{
		if( m_pTIcon[i] )
			if( m_wSelectedColor == i )
				m_pTIcon[i]->SetSkinImage( 50 );
			else
				m_pTIcon[i]->SetSkinImageEmpty();
	}
}