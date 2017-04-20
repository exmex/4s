#include "Stdafx.h"
#include "Gizmo.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "D3DCamera.h"



LPDIRECT3DDEVICE9	CGizmo::m_pDevice = NULL;
HWND				CGizmo::m_hWnd = NULL;
CD3DCamera*			CGizmo::m_pCamera = NULL;




CGizmo::CGizmo( HWND hwnd, CD3DCamera* pCamera )
: m_bGizmoState(GIZMO_NONE)
, m_pGizmo(NULL)
, m_bMouse(FALSE)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDevice = pMainFrame->m_3DDevice.m_pDevice;

	m_hWnd = hwnd;
	m_pCamera = pCamera;
}

CGizmo::~CGizmo(void)
{
}

void CGizmo::SetGizemo( CGizmoState* pGizmo )
{
	if ( pGizmo ) 
		m_pGizmo = pGizmo;
	
	m_bMouse = FALSE;
}

void CGizmo::BeginMouseMove()
{

	if ( m_pGizmo && m_pGizmo->CheckIntersect() )
	{
		m_bMouse = TRUE;
	}

}

void CGizmo::MouseMove()
{
	if ( m_bMouse )
	{
		m_pGizmo->Update();
	}

}

void CGizmo::EndMouseMove()
{
	m_bMouse = FALSE;
}

void CGizmo::Draw()
{
	if ( m_pGizmo )
		m_pGizmo->Draw();
}



//////////////////////////////////////////////////////////////////////////
// CGizmo State

CGizmoState::CGizmoState( void )
{

}




//////////////////////////////////////////////////////////////////////////
// CGizmo Rotate

CGizmoRotate::CGizmoRotate( void )
{

}

CGizmoRotate::~CGizmoRotate( void )
{

}

void CGizmoRotate::Update()
{

}

void CGizmoRotate::Draw()
{

}

BOOL CGizmoRotate::CheckIntersect()
{






	//D3DXVECTOR3	CamPos = CGizimo::m_pCamera->GetCameraPosition( m_mousePt.x, m_mousePt.y );

	//D3DXVec3Normalize( &CamPos, &CamPos );

	//float length = CamPos.y * m_vecObject->x + CamPos.z * m_vecObject->z;

	//m_AxisPlane[0] = D3DXPLANE( 0.0f, CamPos.y, CamPos.z, -length );	//	XY 평면(X축변환).

	//D3DXVECTOR3 start = CPicking::GetRay().m_Position;
	//D3DXVECTOR3 end = CPicking::GetRay().m_Direction * 1000.0f;
	//D3DXVECTOR3 dist = CGizimoMouseController::GetInstance()->Drag( &m_AxisPlane[0], &start, &end );

	//m_vecObject->x += dist.x;


	return TRUE;
}




//////////////////////////////////////////////////////////////////////////

CGizmoTranslate::CGizmoTranslate( void )
{

}

CGizmoTranslate::~CGizmoTranslate( void )
{

}

void CGizmoTranslate::Update()
{

}

void CGizmoTranslate::Draw()
{

}

BOOL CGizmoTranslate::CheckIntersect()
{




	return TRUE;
}