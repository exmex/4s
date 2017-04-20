#include "stdafx.h"
#include "Picking.h"
#include "Game Studio.h"
#include "MainFrm.h"


CPicking::CPicking()
{

}

CPicking::~CPicking()
{

}

SPickRay CPicking::GetRay()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	D3DVIEWPORT9	vp;
	pMainFrame->m_3DDevice.m_pDevice->GetViewport( &vp );
	D3DXMATRIX		matProj;
	pMainFrame->m_3DDevice.m_pDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	POINT	ptCursor;
	GetCursorPos( &ptCursor );
	ScreenToClient( pMainFrame->m_hWnd, &ptCursor );

	D3DXVECTOR3	v;

	v.x = ((  (((ptCursor.x-vp.X)*2.0f/vp.Width ) - 1.0f)) - matProj._31 ) / matProj._11;
	v.y = ((- (((ptCursor.y-vp.Y)*2.0f/vp.Height) - 1.0f)) - matProj._32 ) / matProj._22;
	v.z =  1.0f;

	D3DXMATRIX		matView;
	pMainFrame->m_3DDevice.m_pDevice->GetTransform( D3DTS_VIEW, &matView );
	D3DXMATRIX		m;
	D3DXMatrixInverse( &m, NULL, &matView );

	SPickRay	ray;

	ray.m_Direction.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	ray.m_Direction.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	ray.m_Direction.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	ray.m_Position.x = m._41;
	ray.m_Position.y = m._42;
	ray.m_Position.z = m._43;

	return ray;
}
