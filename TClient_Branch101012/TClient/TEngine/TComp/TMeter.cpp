// TMeter.cpp: implementation of the TMeter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TMeter::TMeter(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_METER;

	m_nValue = 0;
	m_nSubLevel = pDesc->m_vCOMP.m_vEX.meter.sublevel;
	m_nSuperLevel = pDesc->m_vCOMP.m_vEX.meter.superlevel;
	
	m_dwTotalTick = 0;

	m_pSuper = FindKid(pDesc->m_vCOMP.m_vEX.meter.super);
	for( int i = 0; i < MAX_METER_SUBLEVEL;i++)
		m_pSub[i] = (i >= m_nSubLevel)? NULL:  FindKid(pDesc->m_vCOMP.m_vEX.meter.sub[i]);
}

TMeter::~TMeter()
{
}

int TMeter::GetValue()
{
	return m_nValue;
}

void TMeter::SetValue(int nValue)
{
	m_nValue = nValue;
}

HRESULT TMeter::DrawKids( DWORD dwTickCount )
{
	if(m_nSubLevel <= 1) return S_OK;

	m_dwTotalTick += dwTickCount;

	int nSubFull =  m_nSubLevel -1;
	int nMagicNum = (m_nSuperLevel>0) ? m_nSuperLevel*nSubFull: nSubFull;	
	int nSuperNum = m_nValue/nMagicNum;	
	
	CSize size = CSize(0, 0);
	CPoint point = CPoint(-1, -1);
	
	if(m_pSuper)
	{		
		m_pSuper->GetComponentSize(&size);
		SetIncremental(size);		
		SetIncrementalStart(point, size);

		for(int i=0; i<nSuperNum; i++)
		{
			LPIMAGESET pImageset = m_pSuper->GetDefaultImage();
			if( pImageset )
			{
				pImageset->m_dwCurTick = m_dwTotalTick % pImageset->m_dwTotalTick;
				CD3DImage *pImage = pImageset->GetImage();

				if( pImage )
				{
					pImage->Render(
						CTachyonRes::m_pDEVICE->m_pDevice,
						pImageset->GetColor(),
						point.x,
						point.y);

					point += size;
				}
			}
		}
	}

	if(point == CPoint(-1, -1))
		SetIncrementalStart(point, size);

	if(m_nValue%nMagicNum)
	{
		for( int i = 0; i < m_nSuperLevel; i++)
		{
			int nSubIndex = 0;
			if(i < (m_nValue%nMagicNum)/nSubFull) nSubIndex = nSubFull;
			else if( i == (m_nValue%nMagicNum)/nSubFull) nSubIndex = (m_nValue%nMagicNum)%nSubFull;				

			TComponent *pSub = m_pSub[nSubIndex];
			if(pSub)
			{
				pSub->GetComponentSize(&size);
				SetIncremental(size);

				LPIMAGESET pImageset = pSub->GetDefaultImage();
				if(pImageset)
				{
					pImageset->m_dwCurTick = m_dwTotalTick % pImageset->m_dwTotalTick;
					CD3DImage *pImage = pImageset->GetImage();

					if( pImage )
					{
						pImage->Render(
							CTachyonRes::m_pDEVICE->m_pDevice,
							pImageset->GetColor(),
							point.x,
							point.y);

						point += size;
					}
				}
			}
		}
	}

	return S_OK;
}

void TMeter::SetIncremental(CSize &size)
{
	if(m_style & TMS_GROW_RIGHT)		{ size.cy = 0;						}//กๆ
	else if(m_style & TMS_GROW_DOWN)	{ size.cx = 0;						}//ก้	
	else if(m_style & TMS_GROW_LEFT)	{ size.cy = 0; size.cx = -size.cx;	}//ก็
	else								{ size.cx = 0; size.cy = -size.cy;	}//ก่
}

void TMeter::SetIncrementalStart(CPoint &point, CSize size)
{
	point = (m_style& TMS_GROW_LEFT|TMS_GROW_DOWN) ?m_rc.TopLeft(): m_rc.BottomRight() - size;
	ComponentToScreen(&point);
}
