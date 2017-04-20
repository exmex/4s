// TGauge.cpp: implementation of the TGauge class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TGauge::TGauge(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_GAUGE;

	m_dwLastUpdate = GetTickCount();
	m_dwLapTick = DEFAULT_GAUGE_REFRESH;

	m_nMaxValue = 0;
	m_nCurValue = 0;
	m_nNextValue = 0;
	m_nPrevValue = 0;

	m_vBar.clear();

	for( int i=0 ; i<pDesc->m_vCOMP.m_vEX.gauge.count ; ++i )
	{
		TComponent* pBar = FindKid( pDesc->m_vCOMP.m_vEX.gauge.bar[i] );
		if( pBar ) m_vBar.push_back( pBar );
	}
}

TGauge::~TGauge()
{
	m_vBar.clear();
}

void TGauge::SetGauge( int nCurValue, int nMaxValue, BOOL bContinuous )
{
	m_dwLastUpdate = GetTickCount();

	if( nCurValue < 0 )
		nCurValue = 0;

	if( nMaxValue < 0 )
		nMaxValue = 0;

	if( nCurValue > nMaxValue )
		nCurValue = nMaxValue;

	if( bContinuous )
		m_nNextValue = nCurValue;
	else
		m_nNextValue = m_nCurValue = nCurValue;

	m_nPrevValue = m_nCurValue;
	m_nMaxValue = nMaxValue;
}

void TGauge::IncGauge( int nInc, BOOL bContinuous )
{
	m_dwLastUpdate = GetTickCount();

	if( bContinuous )
	{
		m_nNextValue += nInc;
		if( m_nNextValue > m_nMaxValue )
			m_nNextValue = m_nMaxValue;
	}
	else
	{
		m_nCurValue += nInc;
		if( m_nCurValue > m_nMaxValue )
			m_nCurValue = m_nMaxValue;
	}

	m_nPrevValue = m_nCurValue;
}

void TGauge::DecGauge( int nDec, BOOL bContinuous )
{
	m_dwLastUpdate = GetTickCount();

	if( bContinuous )
	{
		m_nNextValue -= nDec;
		if( m_nNextValue < 0 )
			m_nNextValue = 0;
	}
	else
	{
		m_nCurValue -= nDec;
		if( m_nCurValue < 0 )
			m_nCurValue = 0;
	}

	m_nPrevValue = m_nCurValue;
}

HRESULT TGauge::DrawKids( DWORD dwTickCount )
{
	DWORD dwCurTick = GetTickCount();

	switch( m_vBar.size() )
	{
	case 0	: break;

	case 1	:
		{
			TComponent* pBar = m_vBar[0];

			if( pBar )
			{
				if( dwCurTick > m_dwLastUpdate + m_dwLapTick )
				{
					if( m_nNextValue > m_nCurValue )
						++m_nCurValue;
					else if( m_nNextValue < m_nCurValue )
						--m_nCurValue;

					m_dwLastUpdate = dwCurTick;
				}

				if(m_nCurValue)
				{
					CRect rcBar;
					pBar->GetComponentRect( &rcBar );
					CSize size = rcBar.Size();
					CRect rcSrc = CRect( 0, 0, size.cx, size.cy );

					if( m_style & TGS_GROW_RIGHT )
						rcSrc.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
					else if( m_style & TGS_GROW_DOWN )
						rcSrc.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
					else if( m_style & TGS_GROW_UP )
						rcSrc.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
					else if( m_style & TGS_GROW_LEFT )
						rcSrc.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );

					pBar->ShowComponent(TRUE);
					pBar->SetSrcRect(rcSrc);
				}
				else
					pBar->ShowComponent(FALSE);
			}
		}

		break;

	case 2	:
		{
			TComponent* pBar1 = m_vBar[1];
			TComponent* pBar2 = m_vBar[0];

			if( pBar1 && pBar2 )
			{
				BOOL blIsInc = FALSE;
				BOOL blIsDec = FALSE;

				if( dwCurTick > m_dwLastUpdate + m_dwLapTick )
				{
					if( m_nNextValue > m_nCurValue )
					{
						++m_nCurValue;
						blIsInc = TRUE;
					}
					else if( m_nNextValue < m_nCurValue )
					{
						--m_nCurValue;
						blIsDec = TRUE;
					}

					m_dwLastUpdate = dwCurTick;
				}

				if(m_nCurValue)
				{
					CRect rcBar;
					pBar1->GetComponentRect( &rcBar );
					CSize size = rcBar.Size();
					CRect rcSrc1 = CRect( 0, 0, size.cx, size.cy );
					CRect rcSrc2 = CRect( 0, 0, size.cx, size.cy );

					if( blIsInc )
					{
						if( m_style & TGS_GROW_RIGHT )
						{
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nPrevValue) / m_nMaxValue) );
							rcSrc2.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_DOWN )
						{
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nPrevValue) / m_nMaxValue) );
							rcSrc2.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_UP )
						{
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nPrevValue) / m_nMaxValue))) );
							rcSrc2.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
						else if( m_style & TGS_GROW_LEFT )
						{
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nPrevValue) / m_nMaxValue))) );
							rcSrc2.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
					}
					else if( blIsDec )
					{
						if( m_style & TGS_GROW_RIGHT )
						{
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nNextValue) / m_nMaxValue) );
							rcSrc2.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_DOWN )
						{
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nNextValue) / m_nMaxValue) );
							rcSrc2.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_UP )
						{
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nNextValue) / m_nMaxValue))) );
							rcSrc2.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
						else if( m_style & TGS_GROW_LEFT )
						{
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nNextValue) / m_nMaxValue))) );
							rcSrc2.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
					}
					else
					{
						if( m_style & TGS_GROW_RIGHT )
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						else if( m_style & TGS_GROW_DOWN )
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						else if( m_style & TGS_GROW_UP )
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						else if( m_style & TGS_GROW_LEFT )
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );

						rcSrc2 = rcSrc1;
					}

					pBar1->ShowComponent(TRUE);
					pBar2->ShowComponent(TRUE);

					pBar1->SetSrcRect(rcSrc1);
					pBar2->SetSrcRect(rcSrc2);
				}
				else
				{
					pBar1->ShowComponent(FALSE);
					pBar2->ShowComponent(FALSE);
				}
			}
		}

		break;

	default	:
		{
			TComponent* pBar1 = m_vBar[2];
			TComponent* pBar2 = m_vBar[1];
			TComponent* pBar3 = m_vBar[0];

			if( pBar1 && pBar2 && pBar3 )
			{
				BOOL blIsInc = FALSE;
				BOOL blIsDec = FALSE;

				if( dwCurTick > m_dwLastUpdate + m_dwLapTick )
				{
					if( m_nNextValue > m_nCurValue )
					{
						++m_nCurValue;
						blIsInc = TRUE;
					}
					else if( m_nNextValue < m_nCurValue )
					{
						--m_nCurValue;
						blIsDec = TRUE;
					}

					m_dwLastUpdate = dwCurTick;
				}

				if(m_nCurValue)
				{
					CRect rcBar;
					pBar1->GetComponentRect( &rcBar );
					CSize size = rcBar.Size();
					CRect rcSrc1 = CRect( 0, 0, size.cx, size.cy );
					CRect rcSrc2 = CRect( 0, 0, size.cx, size.cy );
					CRect rcSrc3 = CRect( 0, 0, size.cx, size.cy );

					if( blIsInc )
					{
						if( m_style & TGS_GROW_RIGHT )
						{
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nPrevValue) / m_nMaxValue) );
							rcSrc2.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_DOWN )
						{
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nPrevValue) / m_nMaxValue) );
							rcSrc2.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_UP )
						{
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nPrevValue) / m_nMaxValue))) );
							rcSrc2.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
						else if( m_style & TGS_GROW_LEFT )
						{
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nPrevValue) / m_nMaxValue))) );
							rcSrc2.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}

						rcSrc3 = rcSrc1;
					}
					else if( blIsDec )
					{
						if( m_style & TGS_GROW_RIGHT )
						{
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nNextValue) / m_nMaxValue) );
							rcSrc3.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_DOWN )
						{
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nNextValue) / m_nMaxValue) );
							rcSrc3.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						}
						else if( m_style & TGS_GROW_UP )
						{
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nNextValue) / m_nMaxValue))) );
							rcSrc3.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}
						else if( m_style & TGS_GROW_LEFT )
						{
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nNextValue) / m_nMaxValue))) );
							rcSrc3.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						}

						rcSrc2 = rcSrc1;
					}
					else
					{
						if( m_style & TGS_GROW_RIGHT )
							rcSrc1.right = (long) ( m_nMaxValue == 0 ? 0 : size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						else if( m_style & TGS_GROW_DOWN )
							rcSrc1.bottom = (long) ( m_nMaxValue == 0 ? 0 : size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue) );
						else if( m_style & TGS_GROW_UP )
							rcSrc1.top = ( m_nMaxValue == 0 ? 0 : size.cy - ((int) (size.cy * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );
						else if( m_style & TGS_GROW_LEFT )
							rcSrc1.left = ( m_nMaxValue == 0 ? 0 : size.cx - ((int) (size.cx * ((FLOAT)(m_nCurValue) / m_nMaxValue))) );

						rcSrc2 = rcSrc1;
						rcSrc3 = rcSrc1;
					}

					pBar1->ShowComponent(TRUE);
					pBar2->ShowComponent(TRUE);
					pBar3->ShowComponent(TRUE);

					pBar1->SetSrcRect(rcSrc1);
					pBar2->SetSrcRect(rcSrc2);
					pBar3->SetSrcRect(rcSrc3);
				}
				else
				{
					pBar1->ShowComponent(FALSE);
					pBar2->ShowComponent(FALSE);
					pBar3->ShowComponent(FALSE);
				}
			}
		}

		break;
	}

	return TComponent::DrawKids( dwTickCount );
}

void TGauge::OnMouseMove( UINT nFlags, CPoint pt )
{
	if( CanProcess() && m_bShowCurValue && HitTest(pt) )
		m_strText.Format("%d%%",m_nCurValue);
	else
		m_strText.Empty();

	TComponent::OnMouseMove( nFlags, pt );
}

int TGauge::GetCurValue()
{
	return m_nNextValue;
}

int TGauge::GetMaxValue()
{
	return m_nMaxValue;
}

void TGauge::SetRefreshTime(DWORD dwRefreshTime)
{
	m_dwLapTick = dwRefreshTime;
}	

void TGauge::SetCurrBarImage( BYTE bCurrBarImage )
{
	for( int i=0 ; i<m_vBar.size() ; ++i )
	{
		TImageList* pImage = (TImageList*) m_vBar[i];

		if( pImage )
			pImage->SetCurImage( bCurrBarImage );
	}
}

void TGauge::SetShowTextOnMouse( BOOL b )
{
	m_bShowCurValue = b;
}