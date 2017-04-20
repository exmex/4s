// D3DLight.cpp: implementation of the CD3DLight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPDIRECT3DDEVICE9 CD3DLight::m_pDevice = NULL;
QUEUEDWORD CD3DLight::m_queueFreeLight;
MAPDWORD CD3DLight::m_mapUsedLight;

DWORD CD3DLight::m_dwNextIndex = 0;
BOOL CD3DLight::m_bInit = FALSE;


CD3DLight::CD3DLight()
{
	ZeroMemory( &m_Light, sizeof(D3DLIGHT9));
	m_dwIndex = PickLightIndex();
}

CD3DLight::~CD3DLight()
{
	Release();
}

void CD3DLight::Release()
{
	ZeroMemory( &m_Light, sizeof(D3DLIGHT9));
	if(m_pDevice)
		m_pDevice->LightEnable( m_dwIndex, FALSE);

	ReleaseLightIndex(m_dwIndex);
}

void CD3DLight::SetDevice( LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;
}

DWORD CD3DLight::PickLightIndex()
{
	if(!m_bInit)
		InitD3DLight();

	DWORD dwIndex = m_dwNextIndex;

	if(!m_queueFreeLight.empty())
	{
		dwIndex = m_queueFreeLight.front();
		m_queueFreeLight.pop();
	}
	else
		m_dwNextIndex++;

	m_mapUsedLight.insert( MAPDWORD::value_type( dwIndex, dwIndex));
	return dwIndex;
}

void CD3DLight::ReleaseLightIndex( DWORD dwIndex)
{
	MAPDWORD::iterator finder = m_mapUsedLight.find(dwIndex);

	if( finder != m_mapUsedLight.end() )
	{
		m_mapUsedLight.erase(finder);
		m_queueFreeLight.push(dwIndex);
	}

	if(m_mapUsedLight.empty())
		ReleaseD3DLight();
}

void CD3DLight::ReleaseD3DLight()
{
	while(!m_queueFreeLight.empty())
		m_queueFreeLight.pop();

	if(!m_mapUsedLight.empty())
		m_mapUsedLight.clear();

	m_dwNextIndex = 0;
	m_bInit = FALSE;
}

void CD3DLight::InitD3DLight()
{
	ReleaseD3DLight();
	m_bInit = TRUE;
}

void CD3DLight::SetLight( D3DLIGHT9 *pLight)
{
	if(pLight)
		memcpy( &m_Light, pLight, sizeof(D3DLIGHT9));

	if(m_pDevice)
		m_pDevice->SetLight( m_dwIndex, &m_Light);
}

void CD3DLight::EnableLight( BOOL bEnable)
{
	if(m_pDevice)
		m_pDevice->LightEnable( m_dwIndex, bEnable);
}

void CD3DLight::Move( FLOAT fMoveX,
					  FLOAT fMoveY,
					  FLOAT fMoveZ)
{
	m_Light.Position.x += fMoveX;
	m_Light.Position.y += fMoveY;
	m_Light.Position.z += fMoveZ;

	if(m_pDevice)
		m_pDevice->SetLight( m_dwIndex, &m_Light);
}

void CD3DLight::Rotate( LPD3DXVECTOR3 pAxis,
					    FLOAT fAngle)
{
	D3DXMATRIX vRotation;

	D3DXVECTOR3 vDirection = m_Light.Direction;
	D3DXVECTOR4 vResult;

	D3DXMatrixRotationAxis(
		&vRotation,
		pAxis,
		fAngle);

	D3DXVec3Transform(
		&vResult,
		&vDirection,
		&vRotation);

	m_Light.Direction.x = vResult.x;
	m_Light.Direction.y = vResult.y;
	m_Light.Direction.z = vResult.z;

	if(m_pDevice)
		m_pDevice->SetLight( m_dwIndex, &m_Light);
}

void CD3DLight::SetPosition( LPD3DXVECTOR3 pPOS,
							 LPD3DXVECTOR3 pDIR)
{
	if(pDIR)
		m_Light.Direction = *pDIR;

	if(pPOS)
		m_Light.Position = *pPOS;

	if(m_pDevice)
		m_pDevice->SetLight( m_dwIndex, &m_Light);
}
