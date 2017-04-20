#include "stdafx.h"
#include "Gizimo.h"
#include "Picking.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "D3DCamera.h"


/////////////////////////////////////////////////////////////////
//	Implementation CGizimo.
/////////////////////////////////////////////////////////////////

IDirect3DDevice9*	CGizimo::m_pDevice = NULL;
HWND				CGizimo::m_hWnd = NULL;
CD3DCamera*			CGizimo::m_pCamera = NULL;


CGizimo::CGizimo( HWND hwnd, CD3DCamera* pCamera )
:	m_pGizimo( NULL )
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDevice = pMainFrame->m_3DDevice.m_pDevice;

	m_hWnd = hwnd;
	m_pCamera = pCamera;
}

CGizimo::~CGizimo()
{

}

CGizimoState* CGizimo::GetCurrentState()
{
	return m_pGizimo;
}

void CGizimo::ChangeGizimo( CGizimoState* pGizimo )
{
	if( m_pGizimo )
		m_pGizimo->Exit();
	if( pGizimo )
		pGizimo->Enter();

	m_pGizimo = pGizimo;
}

void CGizimo::Update( D3DXVECTOR3* vecObject )
{
	if( !m_pGizimo )	return ;

	m_pGizimo->Excute( vecObject );
}


/////////////////////////////////////////////////////////////////
//	Implementation CGizimoLine.
/////////////////////////////////////////////////////////////////
CGizimoLine::CGizimoLine()
:	m_pLine( NULL )
{
	InitInstance();
}

CGizimoLine::~CGizimoLine()
{
	Release();
}

CGizimoLine* CGizimoLine::GetInstance()
{
	static CGizimoLine	gizimo;
	return &gizimo;
}

void CGizimoLine::InitInstance()
{
	if( m_pLine )
	{
		m_pLine->Release();
		m_pLine = NULL;
	}

	if( FAILED( D3DXCreateLine( CGizimo::m_pDevice, &m_pLine ) ) )
	{
		MessageBoxA( NULL, "D3DXCreateLine Failed", "ERROR", MB_ICONERROR );
		return ;
	}
	m_pLine->SetAntialias(TRUE);
}

void CGizimoLine::Draw( D3DXVECTOR3* vecObject, GLINE& line )
{
	D3DXMATRIX matWorld, matView, matProj;
	D3DXMatrixIdentity( &matWorld );

	matWorld._41 = vecObject->x;
	matWorld._42 = vecObject->y;
	matWorld._43 = vecObject->z;

	CGizimo::m_pDevice->GetTransform( D3DTS_VIEW, &matView );
	CGizimo::m_pDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	DWORD dwLight;
	CGizimo::m_pDevice->GetRenderState( D3DRS_LIGHTING, &dwLight );
	CGizimo::m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	dwLight ? CGizimo::m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) : 0;

	m_pLine->DrawTransform( &line.VtxList[0], (DWORD)line.VtxList.size(), 
		&(matWorld * matView * matProj), line.Color );

	CGizimo::m_pDevice->SetRenderState( D3DRS_LIGHTING, dwLight );
	CGizimo::m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

void CGizimoLine::Release()
{
	m_pLine->Release();
	m_pLine = NULL;
}


/////////////////////////////////////////////////////////////////
//	Implementation CGizimoMouseController.
/////////////////////////////////////////////////////////////////

CGizimoMouseController* CGizimoMouseController::GetInstance()
{
	static CGizimoMouseController mousectrl;
	return &mousectrl;
}

void CGizimoMouseController::BeginDrag()
{
	m_bDrag = true;
	m_PrevPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

D3DXVECTOR3 CGizimoMouseController::Drag( D3DXPLANE* plane, D3DXVECTOR3* start, D3DXVECTOR3* end )
{
	if( !m_bDrag )
		return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	D3DXVECTOR3 pos;

	D3DXPlaneIntersectLine( &pos, plane, start, end );

	if( m_PrevPos == D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) )
	{
		m_PrevPos = pos;
		return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}

	D3DXVECTOR3	r = pos - m_PrevPos;

	m_PrevPos = pos;

	return r;
}

void CGizimoMouseController::EndDrag()
{
	m_bDrag = false;
}

bool CGizimoMouseController::GetDrag() const
{
	return m_bDrag;
}

/////////////////////////////////////////////////////////////////
//	Implementation CGizimoState.
/////////////////////////////////////////////////////////////////

CGizimoState::CGizimoState()
{

}

void CGizimoState::Excute( D3DXVECTOR3* vecObject )
{
	static void (CGizimoState::*fp[])() =
	{
		CGizimoState::UpdateX,
			CGizimoState::UpdateY,
			CGizimoState::UpdateXY,
			CGizimoState::UpdateZ,
			CGizimoState::UpdateXZ,
			CGizimoState::UpdateYZ,
			CGizimoState::UpdateXYZ
	};

	if( m_SelectedAxis.val > 0 )
		(this->*fp[ m_SelectedAxis.val - 1 ])();
}

/////////////////////////////////////////////////////////////////
//	Implementation CGizimoranslate.
/////////////////////////////////////////////////////////////////

CGizimoTranslate::CGizimoTranslate()
{

}

CGizimoTranslate::~CGizimoTranslate()
{

}

void CGizimoTranslate::UpdateX()
{
	D3DXVECTOR3	CamPos = CGizimo::m_pCamera->GetCameraPosition( m_mousePt.x, m_mousePt.y );

	D3DXVec3Normalize( &CamPos, &CamPos );

	float length = CamPos.y * m_vecObject->x + CamPos.z * m_vecObject->z;

	m_AxisPlane[0] = D3DXPLANE( 0.0f, CamPos.y, CamPos.z, -length );	//	XY 평면(X축변환).

	D3DXVECTOR3 start = CPicking::GetRay().m_Position;
	D3DXVECTOR3 end = CPicking::GetRay().m_Direction * 1000.0f;
	D3DXVECTOR3 dist = CGizimoMouseController::GetInstance()->Drag( &m_AxisPlane[0], &start, &end );

	m_vecObject->x += dist.x;
}

void CGizimoTranslate::UpdateY()
{
	D3DXVECTOR3	CamPos = CGizimo::m_pCamera->GetCameraPosition( m_mousePt.x, m_mousePt.y );

	D3DXVec3Normalize( &CamPos, &CamPos );

	float length = CamPos.x * m_vecObject->x + CamPos.z * m_vecObject->z;

	m_AxisPlane[1] = D3DXPLANE( CamPos.x, 0.0f, CamPos.z, -length );

	D3DXVECTOR3 start = CPicking::GetRay().m_Position;
	D3DXVECTOR3 end = CPicking::GetRay().m_Direction * 1000.0f;
	D3DXVECTOR3 dist = CGizimoMouseController::GetInstance()->Drag( &m_AxisPlane[1], &start, &end );

	m_vecObject->y += dist.y;
}

void CGizimoTranslate::UpdateXY()
{

}

void CGizimoTranslate::UpdateZ()
{
	D3DXVECTOR3	CamPos = CGizimo::m_pCamera->GetCameraPosition( m_mousePt.x, m_mousePt.y );

	m_AxisPlane[2] = D3DXPLANE( 0.0f, 1.0f, 0.0f, -m_vecObject->y );

	D3DXVECTOR3 start = CPicking::GetRay().m_Position;
	D3DXVECTOR3 end = CPicking::GetRay().m_Direction * 1000.0f;
	D3DXVECTOR3 dist = CGizimoMouseController::GetInstance()->Drag( &m_AxisPlane[2], &start, &end );

	m_vecObject->z += dist.z;
}

void CGizimoTranslate::UpdateXZ()
{

}

void CGizimoTranslate::UpdateYZ()
{

}


CGizimoTranslate* CGizimoTranslate::GetInstance()
{
	static CGizimoTranslate gizimo;
	return &gizimo;
}

void CGizimoTranslate::Enter()
{
	GLINE line;

	//	X축.
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.9f, 0.05f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.9f, -0.05f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 1.0f, 0.0f, 0.0f ) );
	line.BBoxMin.push_back( D3DXVECTOR3( 0.0f, -0.5f, -0.5f ) );
	line.BBoxMax.push_back( D3DXVECTOR3( 1.0f, 0.5f, 0.5f ) );
	line.Color = D3DCOLOR_ARGB( 255, 255, 0, 0 );

	m_vLine.push_back( line );

	line.Release();
	//	Y축.
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.05f, 0.9f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( -0.05f, 0.9f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	line.BBoxMin.push_back( D3DXVECTOR3( -0.5f, 0.0f, -0.5f ) );
	line.BBoxMax.push_back( D3DXVECTOR3( 0.5f, 1.0f, 0.5f ) );
	line.Color = D3DCOLOR_ARGB( 255, 0, 255, 0 );

	m_vLine.push_back( line );

	line.Release();
	// Z축.
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.05f, 0.0f, 0.9f ) );
	line.VtxList.push_back( D3DXVECTOR3( -0.05f, 0.0f, 0.9f ) );
	line.VtxList.push_back( D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) );
	line.BBoxMin.push_back( D3DXVECTOR3( -0.5f, -0.5f, 0.0f ) );
	line.BBoxMax.push_back( D3DXVECTOR3( 0.5f, 0.5f, 1.0f ) );
	line.Color = D3DCOLOR_ARGB( 255, 0, 0, 255 );

	m_vLine.push_back( line );
}

void CGizimoTranslate::Excute( D3DXVECTOR3* vecObject )
{
	m_vecObject = vecObject;

	CGizimoState::Excute( vecObject );

	D3DCOLOR color[3] = 
	{
		D3DCOLOR_ARGB(155, 255, 0, 0),
		D3DCOLOR_ARGB(155, 0, 255, 0),
		D3DCOLOR_ARGB(155, 0, 0, 255)
	};

	if( !CGizimoMouseController::GetInstance()->GetDrag() )
	{
		m_SelectedAxis.val = 0;

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		GetCursorPos( &m_mousePt );
		ScreenToClient( CGizimo::m_hWnd, &m_mousePt );

//		SPickRay ray = CPicking::GetRay();
		SPickRay ray;
		ray.m_Position = CGizimo::m_pCamera->GetCameraPosition( m_mousePt.x, m_mousePt.y );
		ray.m_Direction = CGizimo::m_pCamera->GetRayDirection( m_mousePt.x, m_mousePt.y );

		float near_len = 30000.0f;
		int selected = -1;

		for( int i=0; i<3; ++i )
		{
			m_vLine[i].Color = color[i];

			if( D3DXBoxBoundProbe(
				&(m_vLine[i].BBoxMin[0] + (*vecObject)),
				&(m_vLine[i].BBoxMax[0] + (*vecObject)),
				&ray.m_Position, &ray.m_Direction ) )
			{
				float len = D3DXVec3Length( &( ray.m_Position - ( m_vLine[i].BBoxMax[0] + (*vecObject) ) ) );
				if( len < near_len )
				{
					near_len = len;
					selected = i;
				}
			}
		}

		if( selected != -1 )
		{
			m_vLine[selected].Color	= D3DCOLOR_ARGB( 255, 255, 255, 0 );
			m_SelectedAxis.val		= 1 << selected;
		}
	}

	//	Draw.
	for( int i=0; i<(int)m_vLine.size(); ++i )
		CGizimoLine::GetInstance()->Draw( vecObject, m_vLine[i] );
}

void CGizimoTranslate::Exit()
{
	m_vLine.clear();
}


