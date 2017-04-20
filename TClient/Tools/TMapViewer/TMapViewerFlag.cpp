#include "StdAfx.h"
#include "Resource.h"

MAPTPATHFLAG CTMapViewerFlag::m_mapTPATHFLAG;
CTachyonMesh CTMapViewerFlag::m_vTMESH;
WORD CTMapViewerFlag::m_wNewID = 1;


CTMapViewerFlag::CTMapViewerFlag()
{
	m_vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPATH.clear();

	m_dwID = TPATHFLAG_NULL;
}

CTMapViewerFlag::~CTMapViewerFlag()
{
	MAPTPATHFLAG::iterator finder = m_mapTPATHFLAG.find(m_dwID);

	if( finder != m_mapTPATHFLAG.end() )
		m_mapTPATHFLAG.erase(finder);

	m_vPATH.clear();
}

void CTMapViewerFlag::InitTPATH( CD3DDevice *pDevice)
{
	HRSRC hResInfo = FindResource( NULL, MAKEINTRESOURCE(IDR_MESH_PATH_FLAG), "MESH");
	HGLOBAL hGlobal = LoadResource( NULL, hResInfo);

	m_vTMESH.LoadMesh(
		pDevice->m_pDevice,
		(LPBYTE) LockResource(hGlobal));

	m_mapTPATHFLAG.clear();
	m_wNewID = 1;
}

void CTMapViewerFlag::ReleaseTPATH()
{
	MAPTPATHFLAG::iterator itTPATH = m_mapTPATHFLAG.begin();

	while(itTPATH != m_mapTPATHFLAG.end())
	{
		MAPTPATHFLAG::iterator itNEXT = itTPATH;
		itNEXT++;

		delete (*itTPATH).second;
		itTPATH = itNEXT;
	}

	m_mapTPATHFLAG.clear();
	m_vTMESH.Release();
	m_wNewID = 1;
}

void CTMapViewerFlag::RegTPATH( CTMapViewerFlag *pTPATH,
							   BYTE bUnitX,
							   BYTE bUnitZ)
{
	WORD wUnitID = MAKEWORD( bUnitX, bUnitZ);

	while( m_wNewID == TPATHFLAG_NULL || m_mapTPATHFLAG.find(MAKELONG( m_wNewID, wUnitID)) != m_mapTPATHFLAG.end() )
		m_wNewID++;

	pTPATH->m_dwID = MAKELONG( m_wNewID, wUnitID);
	m_mapTPATHFLAG.insert( MAPTPATHFLAG::value_type( pTPATH->m_dwID, pTPATH));
}

BYTE CTMapViewerFlag::IsValidPoint( VTOBJBASE& vOBJ, LPD3DXVECTOR3 pPOINT)
{
	CTMapViewerObjBase vMOVE;
	vMOVE.ReleaseData();

	D3DXMatrixIdentity(&vMOVE.m_vWorld);
	vMOVE.m_vWorld._41 = pPOINT->x;
	vMOVE.m_vWorld._42 = pPOINT->y;
	vMOVE.m_vWorld._43 = pPOINT->z;
	vMOVE.m_vPosition = vMOVE.m_vWorld;

	vMOVE.InitSIZE(
		TCOLLISION_SIZE,
		TCOLLISION_HEIGHT,
		TCOLLISION_SIZE,
		0.0f);
	vMOVE.m_bCollisionType = TCOLLISION_CYLINDER;

	for( int i=0; i<INT(vOBJ.size()); i++)
	{
		FLOAT fLength = D3DXVec3Length(&D3DXVECTOR3(
			vOBJ[i]->m_vPosition._41 - vMOVE.m_vPosition._41,
			vOBJ[i]->m_vPosition._42 - vMOVE.m_vPosition._42,
			vOBJ[i]->m_vPosition._43 - vMOVE.m_vPosition._43));
		FLOAT fRadius = vOBJ[i]->m_fRadiusH + vMOVE.m_fRadiusH;

		if( fLength < fRadius && vMOVE.CheckCollision(vOBJ[i]) )
			return FALSE;
	}

	return TRUE;
}

void CTMapViewerFlag::Render( CD3DDevice *pDevice)
{
	D3DXMATRIX vMAT(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		m_vPOS.x, m_vPOS.y, m_vPOS.z, 1.0f);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFA0A0FF);
	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);

	CTMapViewerFlag::m_vTMESH.Render(pDevice->m_pDevice);
	D3DXMatrixIdentity(&vMAT);

	pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);
	pDevice->m_pDevice->SetFVF(D3DFVF_XYZ);

	for( int i=0; i<INT(m_vPATH.size()); i++)
	{
		MAPTPATHFLAG::iterator finder = m_mapTPATHFLAG.find(m_vPATH[i]);

		if( finder != m_mapTPATHFLAG.end() )
		{
			D3DVECTOR vLINE[2] = {
				{ m_vPOS.x, m_vPOS.y, m_vPOS.z},
				{ (*finder).second->m_vPOS.x, (*finder).second->m_vPOS.y, (*finder).second->m_vPOS.z}};

			pDevice->m_pDevice->DrawPrimitiveUP(
				D3DPT_LINELIST, 1,
				vLINE,
				sizeof(D3DVECTOR));
		}
	}
}
