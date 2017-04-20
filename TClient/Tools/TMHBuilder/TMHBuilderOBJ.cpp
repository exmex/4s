#include "StdAfx.h"


CTMHBuilderOBJ::CTMHBuilderOBJ()
{
	m_bCollisionType = TCOLLISION_NONE;
	m_bCamCollision = FALSE;
	m_bType = OT_NONE;

	m_fRadiusH = 0.0f;
	m_fRadius = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fSizeZ = 0.0f;
	m_fDepth = 0.0f;
}

CTMHBuilderOBJ::~CTMHBuilderOBJ()
{
}

BYTE CTMHBuilderOBJ::RectTest( FLOAT fPosX,
							   FLOAT fPosZ,
							   FLOAT fGrid)
{
	switch(m_bCollisionType)
	{
	case TCOLLISION_CYLINDER	: return CylinderRectTest( fPosX, fPosZ, fGrid);
	case TCOLLISION_BOX			: return BoxRectTest( fPosX, fPosZ, fGrid);
	case TCOLLISION_POLY		: return PolyRectTest( fPosX, fPosZ, fGrid);
	}

	return FALSE;
}

BYTE CTMHBuilderOBJ::CylinderRectTest( FLOAT fPosX,
									   FLOAT fPosZ,
									   FLOAT fGrid)
{
	D3DXVECTOR2 vPOINT[4] = {
		D3DXVECTOR2(		 fPosX,			fPosZ),
		D3DXVECTOR2( fPosX + fGrid,			fPosZ),
		D3DXVECTOR2( fPosX + fGrid, fPosZ + fGrid),
		D3DXVECTOR2(		 fPosX, fPosZ + fGrid)};

	D3DXVECTOR2 vCENTER(
		m_vPosition._41,
		m_vPosition._43);

	if( vPOINT[0].x - m_fRadius < vCENTER.x && vPOINT[1].y < vCENTER.y &&
		vPOINT[1].x + m_fRadius > vCENTER.x && vPOINT[2].y > vCENTER.y )
		return TRUE;

	if( vPOINT[0].x < vCENTER.x && vPOINT[1].y - m_fRadius < vCENTER.y &&
		vPOINT[1].x > vCENTER.x && vPOINT[2].y + m_fRadius > vCENTER.y )
		return TRUE;

	for( int i=0; i<4; i++)
		if( D3DXVec2Length(&(vPOINT[i] - vCENTER)) < m_fRadius )
			return TRUE;

	return FALSE;
}

BYTE CTMHBuilderOBJ::BoxRectTest( FLOAT fPosX,
								  FLOAT fPosZ,
								  FLOAT fGrid)
{
	D3DXVECTOR3 vPOLY[6][2][3] = {{{
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f)}}, {{
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f)}}, {{
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f)}}, {{
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f)}}, {{
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3(  m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f)}}, {{
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f)}, {
		D3DXVECTOR3( -m_fSizeX / 2.0f,  m_fSizeY,  m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth, -m_fSizeZ / 2.0f),
		D3DXVECTOR3( -m_fSizeX / 2.0f, -m_fDepth,  m_fSizeZ / 2.0f)}}};

	D3DXQUATERNION vQUAT;
	D3DXMATRIX vMAT;

	CTMath::GetRotationQUAT(
		&vQUAT,
		&m_vPosition);

	D3DXMatrixRotationQuaternion(
		&vMAT,
		&vQUAT);

	vMAT._41 = m_vPosition._41;
	vMAT._42 = m_vPosition._42;
	vMAT._43 = m_vPosition._43;

	for( int i=0; i<6; i++)
		for( int j=0; j<2; j++)
		{
			for( int k=0; k<3; k++)
			{
				CTMath::Transform(
					&vMAT,
					&vPOLY[i][j][k]);
			}

			if(PolyRectTest( vPOLY[i][j], fPosX, fPosZ, fGrid))
				return TRUE;
		}

	return FALSE;
}

BYTE CTMHBuilderOBJ::PolyRectTest( FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fGrid)
{
	LPD3DXMATRIX pMESHMAT = GetMeshMatrix();
	LPANI pANI = GetCurANI();
	MAPCLKINST::iterator it;

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

			if( pMESH->m_dwNodeCount > 0 )
			{
				LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
				{
					LPIBINFO pIBINFO = pMESH->m_pMESH[i][0];

					for( int j=0; j<INT(pIBINFO->m_vIB.size()); j++)
					{
						LPIBDATA pIBDATA = pIBINFO->m_vIB[j];

						for( int k=0; k<pIBDATA->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];

							for( int l=0; l<3; l++)
							{
								LPWMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * k + l]];

								D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
									m_pBone,
									pMESHMAT,
									pMESHDATA) : m_vPosition;

								vPOLY[l] = D3DXVECTOR3(
									pMESHDATA->m_fPosX,
									pMESHDATA->m_fPosY,
									pMESHDATA->m_fPosZ);

								CTMath::Transform(
									&vTrans,
									&vPOLY[l]);
							}

							if(PolyRectTest( vPOLY, fPosX, fPosZ, fGrid))
								return TRUE;
						}
					}
				}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
				{
					LPIBINFO pIBINFO = pMESH->m_pMESH[i][0];

					for( int j=0; j<INT(pIBINFO->m_vIB.size()); j++)
					{
						LPIBDATA pIBDATA = pIBINFO->m_vIB[j];

						for( int k=0; k<pIBDATA->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];

							for( int l=0; l<3; l++)
							{
								LPMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * k + l]];

								vPOLY[l] = D3DXVECTOR3(
									pMESHDATA->m_fPosX,
									pMESHDATA->m_fPosY,
									pMESHDATA->m_fPosZ);

								CTMath::Transform(
									&m_vPosition,
									&vPOLY[l]);
							}

							if(PolyRectTest( vPOLY, fPosX, fPosZ, fGrid))
								return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BYTE CTMHBuilderOBJ::PolyRectTest( LPD3DXVECTOR3 pPOLY,
								   FLOAT fPosX,
								   FLOAT fPosZ,
								   FLOAT fGrid)
{
	D3DXVECTOR3 vPOINT[4] = {
		D3DXVECTOR3(		 fPosX, 0.0f,		  fPosZ),
		D3DXVECTOR3( fPosX + fGrid, 0.0f,		  fPosZ),
		D3DXVECTOR3( fPosX + fGrid, 0.0f, fPosZ + fGrid),
		D3DXVECTOR3(		 fPosX, 0.0f, fPosZ + fGrid)};
	D3DXVECTOR3 vCROSS;

	if( vPOINT[0].x > pPOLY[0].x &&
		vPOINT[0].x > pPOLY[1].x &&
		vPOINT[0].x > pPOLY[2].x )
		return FALSE;

	if( vPOINT[1].x < pPOLY[0].x &&
		vPOINT[1].x < pPOLY[1].x &&
		vPOINT[1].x < pPOLY[2].x )
		return FALSE;

	if( vPOINT[1].z > pPOLY[0].z &&
		vPOINT[1].z > pPOLY[1].z &&
		vPOINT[1].z > pPOLY[2].z )
		return FALSE;

	if( vPOINT[2].z < pPOLY[0].z &&
		vPOINT[2].z < pPOLY[1].z &&
		vPOINT[2].z < pPOLY[2].z )
		return FALSE;

	D3DXVec3Cross(
		&vCROSS,
		&(pPOLY[1] - pPOLY[0]),
		&(pPOLY[2] - pPOLY[0]));
	FLOAT fLENGTH = D3DXVec3Length(&vCROSS);

	if( fLENGTH > 0.0f )
		vCROSS /= fLENGTH;

	if( fabs(vCROSS.y) > 0.00001f )
	{
		D3DXPLANE vPLANE;

		for( int i=0; i<3; i++)
		{
			D3DXPlaneFromPoints(
				&vPLANE,
				&pPOLY[i],
				&(pPOLY[i] + D3DXVECTOR3( 0.0f, 1.0f, 0.0f)),
				&pPOLY[(i + 1) % 3]);

			if( D3DXPlaneDotCoord( &vPLANE, &pPOLY[(i + 2) % 3]) > 0.0f )
				vPLANE = -vPLANE;

			if( D3DXPlaneDotCoord( &vPLANE, &vPOINT[0]) > 0.0f &&
				D3DXPlaneDotCoord( &vPLANE, &vPOINT[1]) > 0.0f &&
				D3DXPlaneDotCoord( &vPLANE, &vPOINT[2]) > 0.0f &&
				D3DXPlaneDotCoord( &vPLANE, &vPOINT[3]) > 0.0f )
				return FALSE;
		}
	}
	else
	{
		D3DXVECTOR3 vPOLY[3] = {
			D3DXVECTOR3( pPOLY[0].x, 0.0f, pPOLY[0].z),
			D3DXVECTOR3( pPOLY[1].x, 0.0f, pPOLY[1].z),
			D3DXVECTOR3( pPOLY[2].x, 0.0f, pPOLY[2].z)};
		D3DXPLANE vPLANE;

		FLOAT vDIFF[2] = {
			D3DXVec3Length(&(vPOLY[1] - vPOLY[0])),
			D3DXVec3Length(&(vPOLY[2] - vPOLY[0]))};

		if( vDIFF[0] < 0.00001f && vDIFF[1] < 0.00001f )
			return FALSE;

		if( vDIFF[0] > vDIFF[1] )
			vPOLY[2] = vPOLY[1];

		D3DXPlaneFromPoints(
			&vPLANE,
			&vPOLY[0],
			&(vPOLY[0] + D3DXVECTOR3( 0.0f, 1.0f, 0.0f)),
			&vPOLY[2]);

		FLOAT fDOT[4] = {
			D3DXPlaneDotCoord( &vPLANE, &vPOINT[0]),
			D3DXPlaneDotCoord( &vPLANE, &vPOINT[1]),
			D3DXPlaneDotCoord( &vPLANE, &vPOINT[2]),
			D3DXPlaneDotCoord( &vPLANE, &vPOINT[3])};

		if( fDOT[0] < 0.0f &&
			fDOT[1] < 0.0f &&
			fDOT[2] < 0.0f &&
			fDOT[3] < 0.0f )
			return FALSE;

		if( fDOT[0] > 0.0f &&
			fDOT[1] > 0.0f &&
			fDOT[2] > 0.0f &&
			fDOT[3] > 0.0f )
			return FALSE;
	}

	return TRUE;
}

void CTMHBuilderOBJ::InitSIZE( FLOAT fSizeX,
							   FLOAT fSizeY,
							   FLOAT fSizeZ,
							   FLOAT fDepth)
{
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;
	m_fSizeZ = fSizeZ;
	m_fDepth = fDepth;

	m_fRadius = max(
		m_fSizeX,
		m_fSizeZ);

	if( m_bCollisionType != TCOLLISION_CYLINDER )
		m_fRadius *= 1.5f;
	m_fRadius /= 2.0f;

	m_fRadiusH = max(
		m_fSizeY,
		m_fRadius);

	m_fRadiusH = max(
		m_fRadiusH,
		m_fDepth);
	m_fRadiusH *= 1.2f;
}
