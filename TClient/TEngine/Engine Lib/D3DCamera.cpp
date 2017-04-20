// D3DCamera.cpp: implementation of the CD3DCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DCamera::CD3DCamera()
{
	D3DXMatrixIdentity(&m_matProjection);
	D3DXMatrixIdentity(&m_matInvView);
	D3DXMatrixIdentity(&m_matView);

	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	m_pDevice = NULL;
	m_fAspect = 0.0f;
	m_fDotFOV = 1.0f;
	m_fFOV = 0.0f;

	m_fNearPlane = 0.0f;
	m_fFarPlane = 0.0f;

	m_fHeight = 0.0f;
	m_fWidth = 0.0f;

	m_vAxisZ = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vAxisY = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vAxisX = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}

CD3DCamera::~CD3DCamera()
{
}

BOOL CD3DCamera::InitOrthoCamera( LPDIRECT3DDEVICE9 pDevice,
								  FLOAT fNearPlane,
								  FLOAT fFarPlane,
								  FLOAT fWidth,
								  FLOAT fHeight)
{
	if(!pDevice)
		return FALSE;

	m_fHeight = fHeight;
	m_fWidth = fWidth;

	m_fAspect = m_fWidth / m_fHeight;
	m_fDotFOV = 1.0f;
	m_fFOV = 0.0f;

	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	D3DXMatrixOrthoLH(
		&m_matProjection,
		m_fWidth,
		m_fHeight,
		m_fNearPlane,
		m_fFarPlane);

	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pDevice = pDevice;

	return TRUE;
}

BOOL CD3DCamera::InitCamera( LPDIRECT3DDEVICE9 pDevice,
							 FLOAT fNearPlane,
							 FLOAT fFarPlane,
							 FLOAT fFovV,
							 int nWidth,
							 int nHeight)
{
	if(!pDevice)
		return FALSE;

	m_fHeight = (FLOAT) nHeight;
	m_fWidth = (FLOAT) nWidth;

	m_fAspect = m_fWidth / m_fHeight;
	m_fDotFOV = (FLOAT) cos(fFovV / 2.0f);
	m_fFOV = fFovV;

	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	D3DXMatrixPerspectiveFovLH(
		&m_matProjection,
		m_fFOV,
		m_fAspect,
		m_fNearPlane,
		m_fFarPlane);

	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pDevice = pDevice;

	return TRUE;
}

void CD3DCamera::ResetFOV( FLOAT fFOV)
{
	if(IsOrthoCamera())
		return;
	m_fFOV = fFOV;

	D3DXMatrixPerspectiveFovLH(
		&m_matProjection,
		m_fFOV,
		m_fAspect,
		m_fNearPlane,
		m_fFarPlane);
}

void CD3DCamera::SetPosition( D3DXVECTOR3& vPosition,
							  D3DXVECTOR3& vTarget,
							  D3DXVECTOR3& vUp,
							  BOOL bMove)
{
	D3DXMatrixLookAtLH(
		&m_matView,
		&vPosition,
		&vTarget,
		&vUp);

	if( bMove && m_pDevice )
		m_pDevice->SetTransform( D3DTS_VIEW, &m_matView);

	m_matInvView = CTMath::Inverse(&m_matView);
	m_vPosition = vPosition;
	m_vTarget = vTarget;
	m_vUp = vUp;
}

void CD3DCamera::Activate( BOOL bInit)
{
	if(m_pDevice)
	{
		if(bInit)
		{
			D3DXMATRIX matWorld;

			D3DXMatrixIdentity(&matWorld);
			m_pDevice->SetTransform( D3DTS_WORLD, &matWorld);
		}

		m_pDevice->SetTransform( D3DTS_VIEW, &m_matView);
		
		if(bInit)
			m_pDevice->SetTransform( D3DTS_PROJECTION, &m_matProjection);

		m_vAxisZ = m_vTarget - m_vPosition;
		m_vAxisY = m_vUp;

		D3DXVec3Cross(
			&m_vAxisX,
			&m_vAxisY,
			&m_vAxisZ);

		m_vAxisX /= D3DXVec3Length(&m_vAxisX);
		m_vAxisY /= D3DXVec3Length(&m_vAxisY);
		m_vAxisZ /= D3DXVec3Length(&m_vAxisZ);
	}
}

void CD3DCamera::ZoomOrthoCamera( FLOAT fZoom)
{
	FLOAT fZoomH = fZoom * m_fHeight / m_fWidth;

	m_fHeight += fZoomH;
	m_fWidth += fZoom;

	D3DXMatrixOrthoLH(
		&m_matProjection,
		m_fWidth,
		m_fHeight,
		m_fNearPlane,
		m_fFarPlane);
	Activate(TRUE);
}

void CD3DCamera::Move( LPD3DXPLANE pPlane,
					   FLOAT fAxisX,
					   FLOAT fAxisY,
					   BOOL bAbsAxis)
{
	if( !pPlane || (
		pPlane->a == 0 &&
		pPlane->b == 0 &&
		pPlane->c == 0))
		return;

	D3DXPLANE vPlane = *pPlane;

	D3DXVECTOR3 vDirection( 0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vAxisX;
	D3DXVECTOR3 vAxisY;
	D3DXVECTOR3 vAxisZ;

	D3DXMATRIX matInvBasis;
	D3DXMATRIX matBasis;

	if(!bAbsAxis)
	{
		FLOAT fDet;

		matBasis = m_matView;
		matBasis._41 = 0.0f;
		matBasis._42 = 0.0f;
		matBasis._43 = 0.0f;
		matBasis._44 = 1.0f;

		D3DXMatrixInverse(
			&matInvBasis,
			&fDet,
			&matBasis);

		D3DXPlaneTransform(
			&vPlane,
			pPlane,
			&matBasis);
	}

	vAxisZ = D3DXVECTOR3(
		vPlane.a,
		vPlane.b,
		vPlane.c);

	if( vPlane.a == 0.0f )
	{
		vAxisX = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);

		D3DXVec3Cross(
			&vAxisY,
			&vAxisZ,
			&vAxisX);
	}
	else if( vPlane.b == 0.0f )
	{
		vAxisY = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

		D3DXVec3Cross(
			&vAxisX,
			&vAxisY,
			&vAxisZ);
	}
	else if( vPlane.c == 0.0f )
	{
		vAxisX = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

		D3DXVec3Cross(
			&vAxisY,
			&vAxisZ,
			&vAxisX);
	}
	else
	{
		vAxisX = D3DXVECTOR3( 1.0f, 0.0f, - ( vPlane.a + vPlane.d ) / vPlane.c);

		D3DXVec3Cross(
			&vAxisY,
			&vAxisZ,
			&vAxisX);
	}

	if(!bAbsAxis)
	{
		D3DXVECTOR4 vResult;

		D3DXVec3Transform(
			&vResult,
			&vAxisX,
			&matInvBasis);

		vAxisX = D3DXVECTOR3(
			vResult.x,
			vResult.y,
			vResult.z);

		D3DXVec3Transform(
			&vResult,
			&vAxisY,
			&matInvBasis);

		vAxisY = D3DXVECTOR3(
			vResult.x,
			vResult.y,
			vResult.z);

		D3DXVec3Transform(
			&vResult,
			&vAxisZ,
			&matInvBasis);

		vAxisZ = D3DXVECTOR3(
			vResult.x,
			vResult.y,
			vResult.z);

		if( D3DXVec3Dot( &(m_vTarget - m_vPosition), &vAxisZ) < 0 )
			vAxisY = -vAxisY;
	}

	vAxisX /= D3DXVec3Length(&vAxisX);
	vAxisY /= D3DXVec3Length(&vAxisY);
	vAxisZ /= D3DXVec3Length(&vAxisZ);

	vDirection = fAxisX * vAxisX + fAxisY * vAxisY;
	Move( vDirection.x, vDirection.y, vDirection.z, TRUE);
}

void CD3DCamera::Move( FLOAT fAxisX,
					   FLOAT fAxisY,
					   FLOAT fAxisZ,
					   BOOL bAbsAxis)
{
	D3DXVECTOR3 vDirection(
		fAxisX,
		fAxisY,
		fAxisZ);

	if(!bAbsAxis)
	{
		D3DXVECTOR3 vAxisZ = m_vTarget - m_vPosition;
		D3DXVECTOR3 vAxisY = m_vUp;
		D3DXVECTOR3 vAxisX;

		D3DXVec3Cross(
			&vAxisX,
			&vAxisY,
			&vAxisZ);

		vAxisX /= D3DXVec3Length(&vAxisX);
		vAxisY /= D3DXVec3Length(&vAxisY);
		vAxisZ /= D3DXVec3Length(&vAxisZ);

		vDirection = fAxisX * vAxisX + fAxisY * vAxisY + fAxisZ * vAxisZ;
	}

	m_vPosition += vDirection;
	m_vTarget += vDirection;

	SetPosition(
		m_vPosition,
		m_vTarget,
		m_vUp);
}

void CD3DCamera::Rotate( FLOAT fYaw,
						 FLOAT fPitch,
						 FLOAT fRoll,
						 BOOL bAbsAxis)
{
	D3DXMATRIX matRotation;
	D3DXVECTOR4 vResult;

	D3DXMatrixRotationYawPitchRoll(
		&matRotation,
		-fYaw,
		fPitch,
		fRoll);

	if(!bAbsAxis)
	{
		D3DXMATRIX matInvBasis;
		D3DXMATRIX matBasis;

		matBasis = m_matView;
		matBasis._41 = 0.0f;
		matBasis._42 = 0.0f;
		matBasis._43 = 0.0f;
		matBasis._44 = 1.0f;
		matInvBasis = CTMath::Inverse(&matBasis);

		matRotation = matBasis * matRotation * matInvBasis;
	}

	D3DXMATRIX matInvTrans;
	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);
	matTrans._41 = -m_vTarget.x;
	matTrans._42 = -m_vTarget.y;
	matTrans._43 = -m_vTarget.z;
	matTrans._44 = 1.0f;
	matInvTrans = CTMath::Inverse(&matTrans);

	m_matView = matTrans * matRotation * matInvTrans * m_matView;
	m_matInvView = CTMath::Inverse(&m_matView);

	m_vPosition = D3DXVECTOR3(
		m_matInvView._41,
		m_matInvView._42,
		m_matInvView._43);

	m_vUp = D3DXVECTOR3(
		m_matInvView._21,
		m_matInvView._22,
		m_matInvView._23);

	Activate();
}

D3DXVECTOR3 CD3DCamera::GetRayDirection( int nPosX, int nPosY)
{
	FLOAT fPosX = (FLOAT) nPosX;
	FLOAT fPosY = (FLOAT) nPosY;

	FLOAT fDX = tanf( 0.5f * m_fFOV ) * ( fPosX / (m_fWidth / 2.0f) - 1.0f ) * m_fAspect;
	FLOAT fDY = tanf( 0.5f * m_fFOV ) * ( 1.0f - fPosY / (m_fHeight / 2.0f));

	D3DXVECTOR3 vStart(
		fDX * m_fNearPlane,
		fDY * m_fNearPlane,
		m_fNearPlane);

	D3DXVECTOR3 vStop(
		fDX * m_fFarPlane,
		fDY * m_fFarPlane,
		m_fFarPlane);

	CTMath::Transform(
		&m_matInvView,
		&vStart);

	CTMath::Transform(
		&m_matInvView,
		&vStop);

	return (vStop - vStart) / D3DXVec3Length(&(vStop - vStart));
}

D3DXVECTOR3 CD3DCamera::GetPlanePoint( LPD3DXPLANE pPlane,
									   int nPosX,
									   int nPosY)
{
	D3DXVECTOR3 vDirection = GetRayDirection( nPosX, nPosY);
	D3DXVECTOR3 vResult( 0.0f, 0.0f, 0.0f);

	if(!CTMath::PlaneIntersectLine(
		&vResult,
		&m_vPosition,
		&(m_vPosition + vDirection),
		pPlane))
	{
		vResult = D3DXVECTOR3(
			0.0f,
			0.0f,
			0.0f);
	}

	return vResult;
}

BOOL CD3DCamera::IsOrthoCamera()
{
	return m_fFOV == 0.0f;
}

D3DXVECTOR3 CD3DCamera::GetCameraPosition( int nPosX, int nPosY)
{
	if( IsOrthoCamera() && m_pDevice )
	{
		D3DVIEWPORT9 viewport;

		m_pDevice->GetViewport(&viewport);
		nPosX -= viewport.X;
		nPosY -= viewport.Y;

		D3DXVECTOR3 vPosition(
			m_fWidth * (((FLOAT) nPosX) - ((FLOAT) viewport.Width) / 2.0f) / ((FLOAT) viewport.Width),
			m_fHeight * (((FLOAT) viewport.Height) / 2.0f - ((FLOAT) nPosY)) / ((FLOAT) viewport.Height),
			0.0f);

		D3DXVECTOR3 vAxisZ = m_vTarget - m_vPosition;
		D3DXVECTOR3 vAxisY = m_vUp;
		D3DXVECTOR3 vAxisX;

		D3DXVec3Cross(
			&vAxisX,
			&vAxisY,
			&vAxisZ);

		vAxisX /= D3DXVec3Length(&vAxisX);
		vAxisY /= D3DXVec3Length(&vAxisY);
		vAxisZ /= D3DXVec3Length(&vAxisZ);

		return m_vPosition + vPosition.x * vAxisX + vPosition.y * vAxisY;
	}

	return m_vPosition;
}

D3DXMATRIX CD3DCamera::GetWorldMatrix( FLOAT fPosX,
									   FLOAT fPosY,
									   FLOAT fPosZ )
{
	return D3DXMATRIX(
		m_vAxisX.x, m_vAxisX.y, m_vAxisX.z, 0.0f,
		m_vAxisY.x, m_vAxisY.y, m_vAxisY.z, 0.0f,
		m_vAxisZ.x, m_vAxisZ.y, m_vAxisZ.z, 0.0f,
		fPosX, fPosY, fPosZ, 1.0f );
}
