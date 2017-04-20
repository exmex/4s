#include "stdafx.h"
#include "TMonSpawnInspectorObjBase.h"
#include "TMonSpawnInspectorRes.h"
#include "TMonSpawnInspectorMAP.h"

FLOAT CTMonSpawnInspectorObjBase::m_fCamDist = 0.0f;

CTMonSpawnInspectorObjBase::CTMonSpawnInspectorObjBase()
:	CTachyonObject()
{
}

CTMonSpawnInspectorObjBase::~CTMonSpawnInspectorObjBase()
{
	while(!m_vHCOLLISION.empty())
	{
		delete m_vHCOLLISION.back();
		m_vHCOLLISION.pop_back();
	}

	while(!m_vVCOLLISION.empty())
	{
		delete m_vVCOLLISION.back();
		m_vVCOLLISION.pop_back();
	}
}

void CTMonSpawnInspectorObjBase::InitSIZE( FLOAT fSizeX,
										  FLOAT fSizeY,
										  FLOAT fSizeZ,
										  FLOAT fDepth,
										  FLOAT fSight)
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

	m_fRange = max(
		m_fSizeY,
		m_fRadius);
	m_fRadius /= 2.0f;

	m_fRadiusH = max(
		m_fSizeY,
		m_fRadius);

	m_fRadiusH = max(
		m_fRadiusH,
		m_fDepth);
	m_fRadiusH *= 1.2f;

	m_fSight = max(
		m_fRadiusH,
		fSight);

	if( m_fRange < TLOWCULL_BOUND )
		m_fRange = TLOWCULL_FACTOR * m_fCamDist;
	else if( m_fRange < TMIDCULL_BOUND )
		m_fRange = TMIDCULL_FACTOR * m_fCamDist;
	else if( m_fRange < THICULL_BOUND )
		m_fRange = THICULL_FACTOR * m_fCamDist;
	else
		m_fRange = m_fCamDist;
}

void CTMonSpawnInspectorObjBase::ReleaseData()
{
	m_bHouseMesh = FALSE;
	m_bHouseOUT = FALSE;

	m_fCamDIST = 0.0f;
	m_fRadiusH = 0.0f;
	m_fRadius = 0.0f;
	m_fRange = 0.0f;
	m_fSight = 0.0f;
}

void CTMonSpawnInspectorObjBase::InitOBJ( CD3DDevice *pDevice, LPMAPOBJ pOBJ)
{
	MAPATTRIBUTE::iterator itATTR;
	MAPCLKINST::iterator itCI;
	D3DXMATRIX vSCALE;
	D3DXMATRIX vRotX;
	D3DXMATRIX vRotY;
	D3DXMATRIX vRotZ;
	D3DXMATRIX vPOS;

	InitOBJ(pOBJ->m_pOBJ);
	for( itATTR = pOBJ->m_mapATTR.begin(); itATTR != pOBJ->m_mapATTR.end(); itATTR++)
		switch((*itATTR).second->m_bValueType)
	{
		case ATTRTYPE_STRING	: SetATTR( (*itATTR).first, CString((*itATTR).second->m_pValue).Left((*itATTR).second->m_nSize)); break;
		case ATTRTYPE_FLOAT		: SetATTR( (*itATTR).first, *((FLOAT *) (*itATTR).second->m_pValue)); break;
		case ATTRTYPE_INT		: SetATTR( (*itATTR).first, *((int *) (*itATTR).second->m_pValue)); break;
	}

	for( itCI = pOBJ->m_mapCLK.begin(); itCI != pOBJ->m_mapCLK.end(); itCI++)
	{
		LPCLKINST pCI = (*itCI).second;
		SetCloth( pDevice, (*itCI).first, pCI->m_dwCurCL, pCI->m_dwCurMESH);
	}
	ResetOBJPart(pDevice);

	SetAction( pOBJ->m_dwCurACT, pOBJ->m_dwCurANI);
	D3DXMatrixScaling(
		&vSCALE,
		pOBJ->m_vSCALE.x,
		pOBJ->m_vSCALE.y,
		pOBJ->m_vSCALE.z);

	D3DXMatrixRotationX(
		&vRotX,
		pOBJ->m_vROT.x);

	D3DXMatrixRotationY(
		&vRotY,
		pOBJ->m_vROT.y);

	D3DXMatrixRotationZ(
		&vRotZ,
		pOBJ->m_vROT.z);

	D3DXMatrixTranslation(
		&vPOS,
		pOBJ->m_vPOS.x,
		pOBJ->m_vPOS.y,
		pOBJ->m_vPOS.z);

	m_vWorld = vRotX * vRotY * vRotZ * vPOS;
	m_vScale = vSCALE;

	m_dwOBJSNDFuncID = pOBJ->m_dwOBJFuncID;
	m_dwACTSNDFuncID = pOBJ->m_dwACTFuncID;
	m_dwANISNDFuncID = pOBJ->m_dwANIFuncID;

	m_dwOBJSFXFuncID = 0;
	m_dwACTSFXFuncID = 0;
	m_dwANISFXFuncID = 0;
	m_vPosition = m_vScale * m_vWorld;
}

void CTMonSpawnInspectorObjBase::InitOBJ( LPOBJECT pOBJ)
{
	LPOBJECT pPREV = m_OBJ.m_pOBJ;

	m_mapCOLOR.clear();
	m_OBJ.Release();
	ClearMatrix();
	ClearSFX();
	ClearSND();
	m_OBJ.m_pOBJ = pOBJ;

	if(pOBJ)
	{
		MAPSFXINST::iterator itSFX;
		MAPSNDINST::iterator itSND;
		MAPCLKIND::iterator itCLK;
		MAPACTION::iterator itACT;

		if(pOBJ->m_pRES)
			((CTMonSpawnInspectorRes*)pOBJ->m_pRES)->LockOBJ(pOBJ);

		for( itCLK = pOBJ->m_mapCLKIND.begin(); itCLK != pOBJ->m_mapCLKIND.end(); itCLK++)
		{
			LPCLKINST pCLK = new CLKINST();

			m_OBJ.m_mapCLK.insert( MAPCLKINST::value_type(
				(*itCLK).first,
				pCLK));
		}

		for( itACT = pOBJ->m_mapACTION.begin(); itACT != pOBJ->m_mapACTION.end(); itACT++)
		{
			LPACTINST pACT = new ACTINST();

			m_OBJ.m_mapACT.insert( MAPACTINST::value_type(
				(*itACT).first,
				pACT));
		}

		for( itSFX = pOBJ->m_mapSFX.begin(); itSFX != pOBJ->m_mapSFX.end(); itSFX++)
			m_mapOBJSFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));

		for( itSND = pOBJ->m_mapSND.begin(); itSND != pOBJ->m_mapSND.end(); itSND++)
			m_mapOBJSNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

		if(pOBJ->m_bPivotCount)
		{
			m_pBlendKEY = new D3DXQUATERNION[pOBJ->m_bPivotCount];
			m_pPivot = new D3DXQUATERNION[pOBJ->m_bPivotCount];
			m_pBlend = new D3DXMATRIX[pOBJ->m_bPivotCount];
			m_pBone = new D3DXMATRIX[pOBJ->m_bPivotCount];

			for( BYTE i=0; i<pOBJ->m_bPivotCount; i++)
			{
				D3DXQuaternionIdentity(&m_pBlendKEY[i]);
				D3DXQuaternionIdentity(&m_pPivot[i]);

				D3DXMatrixIdentity(&m_pBlend[i]);
				D3DXMatrixIdentity(&m_pBone[i]);
			}
		}
	}

	if( pPREV && pPREV->m_pRES )
		pPREV->m_pRES->ReleaseOBJ(pPREV);

	m_fActTime = 0.0f;
	m_dwTexTick = 0;
	m_nKeyBase = -1;
}

void CTMonSpawnInspectorObjBase::ResetOBJPart( CD3DDevice *pDevice)
{
	MAPCLKINST::iterator itCI;
	MAPOBJPART::iterator it;

	m_bUseSHADER = FALSE;
	m_nVS = VS_COUNT;

	for( it = m_OBJ.m_mapDRAW.begin(); it != m_OBJ.m_mapDRAW.end(); it++)
	{
		VECTOROBJPART *pDRAW = (*it).second;

		while(!pDRAW->empty())
		{
			delete pDRAW->back();
			pDRAW->pop_back();
		}

		delete pDRAW;
	}
	m_OBJ.m_mapDRAW.clear();

	for( itCI = m_OBJ.m_mapCLK.begin(); itCI != m_OBJ.m_mapCLK.end(); itCI++)
	{
		LPMESH pMESH = (*itCI).second->m_pMESH;

		if( pMESH && pMESH->m_pMESH )
			for( int i=0; i<INT(pMESH->m_vTEX.size()); i++)
				if( (*itCI).second->m_dwHideID != pMESH->m_vTEX[i]->m_dwOrder )
				{
					MAPOBJPART::iterator finder = m_OBJ.m_mapDRAW.find(pMESH->m_vTEX[i]->m_dwOrder);
					VECTOROBJPART *pDRAW = NULL;

					if( finder == m_OBJ.m_mapDRAW.end() )
					{
						pDRAW = new VECTOROBJPART();

						m_OBJ.m_mapDRAW.insert( MAPOBJPART::value_type(
							pMESH->m_vTEX[i]->m_dwOrder,
							pDRAW));
					}
					else
						pDRAW = (*finder).second;

					LPOBJPART pPART = new OBJPART();
					pPART->m_pTEX = pMESH->m_vTEX[i];
					pPART->m_pMESH = pMESH->m_pMESH;
					pPART->m_dwIndex = i;

					//if(!m_bUseSHADER)
					//	m_bUseSHADER = pDevice->m_bEnableSHADER && pPART->m_pTEX->m_dwOP == D3DTOP_FORCE_DWORD;

					if( m_nVS == VS_COUNT )
						m_nVS = pPART->m_pMESH->m_dwNodeCount ? VS_WMESHVERTEX : VS_MESHVERTEX;

					pDRAW->push_back(pPART);
				}
	}

	m_dwTexTick = 0;
}

void CTMonSpawnInspectorObjBase::InitCollision()
{
	if( m_bCollisionType != TCOLLISION_POLY )
		return;

	while(!m_vHCOLLISION.empty())
	{
		delete m_vHCOLLISION.back();
		m_vHCOLLISION.pop_back();
	}

	while(!m_vVCOLLISION.empty())
	{
		delete m_vVCOLLISION.back();
		m_vVCOLLISION.pop_back();
	}

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

							PushCollisionPoly( vPOLY, WORD(i), WORD(j), WORD(k));
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

							PushCollisionPoly( vPOLY, WORD(i), WORD(j), WORD(k));
						}
					}
				}
			}
		}
	}
}

void CTMonSpawnInspectorObjBase::PushCollisionPoly( LPD3DXVECTOR3 pPOLY,
												   WORD wMESH,
												   WORD wINDEX,
												   WORD wPOLY)
{
	D3DXPLANE vPLANE;

	D3DXPlaneFromPoints(
		&vPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	D3DXPlaneNormalize(
		&vPLANE,
		&vPLANE);

	D3DXVECTOR3 vNormal(
		vPLANE.a,
		0.0f,
		vPLANE.c);
	FLOAT fLength = D3DXVec3Length(&vNormal);

	if( fLength != 0.0f && fabs(vPLANE.b) / fLength <= CTMonSpawnInspectorMAP::m_fMoveBound )
	{
		LPTVERTEXINDEX pINDEX = new TVERTEXINDEX();

		pINDEX->m_wINDEX = wINDEX;
		pINDEX->m_wMESH = wMESH;
		pINDEX->m_wPOLY = wPOLY;

		m_vHCOLLISION.push_back(pINDEX);
	}
	else if( vPLANE.b > 0.0f )
	{
		LPTVERTEXINDEX pINDEX = new TVERTEXINDEX();

		pINDEX->m_wINDEX = wINDEX;
		pINDEX->m_wMESH = wMESH;
		pINDEX->m_wPOLY = wPOLY;

		m_vVCOLLISION.push_back(pINDEX);
	}
}

BYTE CTMonSpawnInspectorObjBase::CheckCollision( CTMonSpawnInspectorObjBase *pOBJ)
{
	if( pOBJ->m_bType == OT_PATHWALL )
		return FALSE;

	switch(pOBJ->m_bCollisionType)
	{
	case TCOLLISION_CYLINDER	: return CheckCylinder(pOBJ);
	case TCOLLISION_BOX			: return CheckBox(pOBJ);
	case TCOLLISION_POLY		: return CheckPoly(pOBJ);
	}

	return FALSE;
}


BYTE CTMonSpawnInspectorObjBase::CheckCylinder( CTMonSpawnInspectorObjBase *pOBJ)
{
	if( m_vPosition._42 + m_fSizeY <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		m_vPosition._42 >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;

	D3DXVECTOR2 vDIR(
		m_vPosition._41 - pOBJ->m_vPosition._41,
		m_vPosition._43 - pOBJ->m_vPosition._43);

	FLOAT fDist = D3DXVec2Length(&vDIR);
	vDIR /= fDist;

	fDist = pOBJ->m_fRadius + m_fRadius - fDist;
	vDIR *= fDist;

	if( fDist > 0.0f )
	{
		m_vWorld._41 += vDIR.x;
		m_vWorld._43 += vDIR.y;

		return TRUE;
	}

	return FALSE;
}

BYTE CTMonSpawnInspectorObjBase::CheckBox( CTMonSpawnInspectorObjBase *pOBJ)
{
	if( m_vPosition._42 + m_fSizeY <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		m_vPosition._42 >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;

	D3DXVECTOR3 vPOLY[6][2][3] = {{{
		D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f),
			D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f),
			D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f)}, {
				D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
					D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f),
					D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f)}}, {{
						D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
							D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
							D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f)}, {
								D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
									D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
									D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f)}}, {{
										D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
											D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
											D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f)}, {
												D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
													D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
													D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f)}}, {{
														D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
															D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f),
															D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f)}, {
																D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
																	D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f),
																	D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f)}}, {{
																		D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
																			D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
																			D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f)}, {
																				D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
																					D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
																					D3DXVECTOR3(  pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f)}}, {{
																						D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY, -pOBJ->m_fSizeZ / 2.0f),
																							D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
																							D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f)}, {
																								D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f,  pOBJ->m_fSizeY,  pOBJ->m_fSizeZ / 2.0f),
																									D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth, -pOBJ->m_fSizeZ / 2.0f),
																									D3DXVECTOR3( -pOBJ->m_fSizeX / 2.0f, -pOBJ->m_fDepth,  pOBJ->m_fSizeZ / 2.0f)}}};

																								D3DXVECTOR3 vPOS(
																									m_vPosition._41,
																									m_vPosition._42,
																									m_vPosition._43);
																								BYTE bResult = FALSE;

																								for( int i=0; i<6; i++)
																									for( int j=0; j<2; j++)
																									{
																										D3DXPLANE vPLANE;

																										for( int k=0; k<3; k++)
																										{
																											CTMath::Transform(
																												&pOBJ->m_vPosition,
																												&vPOLY[i][j][k]);
																										}

																										if(CheckPoly( &vPLANE, vPOLY[i][j], &vPOS, m_fRadius, m_fSizeY))
																										{
																											FLOAT fLocal = m_fRadius - D3DXPlaneDotCoord(
																												&vPLANE,
																												&vPOS);

																											D3DXVECTOR2 vDIR(
																												vPLANE.a,
																												vPLANE.c);

																											vDIR *= fLocal / D3DXVec2Length(&vDIR);
																											vPOS.x += vDIR.x;
																											vPOS.z += vDIR.y;
																											bResult = TRUE;
																										}
																									}

																									if(bResult)
																									{
																										m_vWorld._41 += vPOS.x - m_vPosition._41;
																										m_vWorld._43 += vPOS.z - m_vPosition._43;
																									}

																									return bResult;
}

BYTE CTMonSpawnInspectorObjBase::CheckPoly( CTMonSpawnInspectorObjBase *pOBJ)
{
	if( m_vPosition._42 + m_fSizeY <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		m_vPosition._42 >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;

	LPD3DXMATRIX pMESHMAT = pOBJ->GetMeshMatrix();
	LPANI pANI = pOBJ->GetCurANI();
	BYTE bResult = FALSE;
	MAPCLKINST::iterator it;

	D3DXVECTOR3 vPOS(
		m_vPosition._41,
		m_vPosition._42,
		m_vPosition._43);

	for( it = pOBJ->m_OBJ.m_mapCLK.begin(); it != pOBJ->m_OBJ.m_mapCLK.end(); it++)
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
				int nCount = INT(pOBJ->m_vHCOLLISION.size());

				for( int i=0; i<nCount; i++)
				{
					LPTVERTEXINDEX pHCOLLISION = pOBJ->m_vHCOLLISION[i];

					WORD wINDEX = pHCOLLISION->m_wINDEX;
					WORD wMESH = pHCOLLISION->m_wMESH;
					WORD wPOLY = pHCOLLISION->m_wPOLY;

					LPIBDATA pIBDATA = pMESH->m_pMESH[wMESH][0]->m_vIB[wINDEX];
					D3DXVECTOR3 vPOLY[3];
					D3DXPLANE vPLANE;

					for( int j=0; j<3; j++)
					{
						LPWMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * wPOLY + j]];

						D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
							pOBJ->m_pBone,
							pMESHMAT,
							pMESHDATA) : pOBJ->m_vPosition;

						vPOLY[j] = D3DXVECTOR3(
							pMESHDATA->m_fPosX,
							pMESHDATA->m_fPosY,
							pMESHDATA->m_fPosZ);

						CTMath::Transform(
							&vTrans,
							&vPOLY[j]);
					}

					if(CheckPoly( &vPLANE, vPOLY, &vPOS, m_fRadius, m_fSizeY))
					{
						FLOAT fLocal = m_fRadius - D3DXPlaneDotCoord(
							&vPLANE,
							&vPOS);

						D3DXVECTOR2 vDIR(
							vPLANE.a,
							vPLANE.c);

						vDIR *= fLocal / D3DXVec2Length(&vDIR);
						vPOS.x += vDIR.x;
						vPOS.z += vDIR.y;
						bResult = TRUE;
					}
				}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;
				int nCount = INT(pOBJ->m_vHCOLLISION.size());

				for( int i=0; i<nCount; i++)
				{
					LPTVERTEXINDEX pHCOLLISION = pOBJ->m_vHCOLLISION[i];

					WORD wINDEX = pHCOLLISION->m_wINDEX;
					WORD wMESH = pHCOLLISION->m_wMESH;
					WORD wPOLY = pHCOLLISION->m_wPOLY;

					LPIBDATA pIBDATA = pMESH->m_pMESH[wMESH][0]->m_vIB[wINDEX];
					D3DXVECTOR3 vPOLY[3];
					D3DXPLANE vPLANE;

					for( int j=0; j<3; j++)
					{
						LPMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * wPOLY + j]];

						vPOLY[j] = D3DXVECTOR3(
							pMESHDATA->m_fPosX,
							pMESHDATA->m_fPosY,
							pMESHDATA->m_fPosZ);

						CTMath::Transform(
							&pOBJ->m_vPosition,
							&vPOLY[j]);
					}

					if(CheckPoly( &vPLANE, vPOLY, &vPOS, m_fRadius, m_fSizeY))
					{
						FLOAT fLocal = m_fRadius - D3DXPlaneDotCoord(
							&vPLANE,
							&vPOS);

						D3DXVECTOR2 vDIR(
							vPLANE.a,
							vPLANE.c);

						vDIR *= fLocal / D3DXVec2Length(&vDIR);
						vPOS.x += vDIR.x;
						vPOS.z += vDIR.y;
						bResult = TRUE;
					}
				}
			}
		}
	}

	if(bResult)
	{
		m_vWorld._41 += vPOS.x - m_vPosition._41;
		m_vWorld._43 += vPOS.z - m_vPosition._43;
	}

	return bResult;
}

BYTE CTMonSpawnInspectorObjBase::CheckPoly( LPD3DXPLANE pPLANE,
										   LPD3DXVECTOR3 pPOLY,
										   LPD3DXVECTOR3 pPOS,
										   FLOAT fRadius,
										   FLOAT fSizeY)
{
	if(!GetCollisionPlane( pPLANE, pPOLY))
		return FALSE;

	FLOAT fDist = D3DXPlaneDotCoord(
		pPLANE,
		pPOS);

	if( fDist < 0.0f || fRadius < fDist )
		return FALSE;

	D3DXVECTOR3 vNormal(
		pPLANE->a,
		0.0f,
		pPLANE->c);
	vNormal /= D3DXVec3Length(&vNormal);

	D3DXVECTOR3 vPOLY[3] = {
		pPOLY[0],
			pPOLY[1],
			pPOLY[2]};
		D3DXVECTOR3 vPOS = (*pPOS);
		D3DXVECTOR3 vDIR;

		for( int i=0; i<3; i++)
			vPOLY[i] -= D3DXPlaneDotCoord( pPLANE, &vPOLY[i]) * vNormal;
		vPOS -= fDist * vNormal;

		D3DXVec3Cross(
			&vDIR,
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
			&vNormal);
		fRadius = sqrtf(fRadius * fRadius - fDist * fDist);

		D3DXVECTOR3 vPOINT[4] = {
			vPOS + fRadius * vDIR,
				D3DXVECTOR3( vPOS.x, vPOS.y + fSizeY, vPOS.z) + fRadius * vDIR,
				D3DXVECTOR3( vPOS.x, vPOS.y + fSizeY, vPOS.z) - fRadius * vDIR,
				vPOS - fRadius * vDIR};

			for( i=0; i<3; i++)
			{
				BYTE bResult = TRUE;
				D3DXPLANE vPLANE;

				D3DXPlaneFromPoints(
					&vPLANE,
					&vPOLY[i],
					&vPOLY[(i + 1) % 3],
					&(vPOLY[i] + vNormal));

				D3DXPlaneNormalize(
					&vPLANE,
					&vPLANE);

				for( int j=0; j<4; j++)
					if( D3DXPlaneDotCoord( &vPLANE, &vPOINT[j]) < 0.0f )
					{
						bResult = FALSE;
						break;
					}

					if(bResult)
						return FALSE;
			}

			for( i=0; i<4; i++)
			{
				BYTE bResult = TRUE;
				D3DXPLANE vPLANE;

				D3DXPlaneFromPoints(
					&vPLANE,
					&vPOINT[i],
					&vPOINT[(i + 1) % 4],
					&(vPOINT[i] + vNormal));

				D3DXPlaneNormalize(
					&vPLANE,
					&vPLANE);

				for( int j=0; j<3; j++)
					if( D3DXPlaneDotCoord( &vPLANE, &vPOLY[j]) < 0.0f )
					{
						bResult = FALSE;
						break;
					}

					if(bResult)
						return FALSE;
			}

			return TRUE;
}

BYTE CTMonSpawnInspectorObjBase::GetCollisionPlane( LPD3DXPLANE pPLANE,
												   LPD3DXVECTOR3 pPOLY)
{
	D3DXPlaneFromPoints(
		pPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	D3DXPlaneNormalize(
		pPLANE,
		pPLANE);

	D3DXVECTOR3 vPOINT(
		0.0f,
		0.0f,
		0.0f);

	D3DXVECTOR3 vNormal(
		pPLANE->a,
		0.0f,
		pPLANE->c);

	FLOAT fLength = D3DXVec3Length(&vNormal);
	FLOAT fDist;

	if( fLength == 0.0f || fabs(pPLANE->b) / fLength > CTMonSpawnInspectorMAP::m_fMoveBound )
		return FALSE;
	vNormal /= fLength;

	D3DXPlaneFromPointNormal(
		pPLANE,
		&vPOINT,
		&vNormal);

	D3DXPlaneNormalize(
		pPLANE,
		pPLANE);

	for( int i=0; i<3; i++)
	{
		fLength = D3DXPlaneDotCoord(
			pPLANE,
			&pPOLY[i]);

		if( i == 0 || fDist < fLength )
		{
			vPOINT = pPOLY[i];
			fDist = fLength;
		}
	}

	D3DXPlaneFromPointNormal(
		pPLANE,
		&vPOINT,
		&vNormal);

	D3DXPlaneNormalize(
		pPLANE,
		pPLANE);

	return TRUE;
}

D3DXMATRIX& CTMonSpawnInspectorObjBase::GetPositionMat()
{
	return m_vPosition;
}

FLOAT CTMonSpawnInspectorObjBase::GetPositionX()
{
	return GetPositionMat()._41;
}

FLOAT CTMonSpawnInspectorObjBase::GetPositionY()
{
	return GetPositionMat()._42;
}

FLOAT CTMonSpawnInspectorObjBase::GetPositionZ()
{
	return GetPositionMat()._43;
}

FLOAT CTMonSpawnInspectorObjBase::GetHeight( CTMonSpawnInspectorObjBase *pOBJ,
											LPD3DXVECTOR3 pPOINT)
{
	if( m_bType == OT_PATHWALL )
		return TMIN_HEIGHT;

	switch(m_bCollisionType)
	{
	case TCOLLISION_CYLINDER	: return GetCylinderHeight( pOBJ, pPOINT);
	case TCOLLISION_BOX			: return GetBoxHeight( pOBJ, pPOINT);
	case TCOLLISION_POLY		: return GetPolyHeight( pOBJ, pPOINT);
	}

	return TMIN_HEIGHT;
}

FLOAT CTMonSpawnInspectorObjBase::GetCylinderHeight( CTMonSpawnInspectorObjBase *pOBJ,
													LPD3DXVECTOR3 pPOINT)
{
	if( pPOINT->y + pOBJ->m_fSizeY <= m_vPosition._42 - m_fDepth )
		return TMIN_HEIGHT;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		pPOINT->x - m_vPosition._41,
		pPOINT->z - m_vPosition._43));

	if( fDist < pOBJ->m_fRadius + m_fRadius )
		return m_vPosition._42 + m_fSizeY;

	return TMIN_HEIGHT;
}

FLOAT CTMonSpawnInspectorObjBase::GetBoxHeight( CTMonSpawnInspectorObjBase *pOBJ,
											   LPD3DXVECTOR3 pPOINT)
{
	if( pPOINT->y + pOBJ->m_fSizeY <= m_vPosition._42 - m_fDepth )
		return TMIN_HEIGHT;

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
																								FLOAT fHeight = TMIN_HEIGHT;

																								for( int i=0; i<6; i++)
																									for( int j=0; j<2; j++)
																									{
																										D3DXPLANE vPLANE;

																										for( int k=0; k<3; k++)
																										{
																											CTMath::Transform(
																												&m_vPosition,
																												&vPOLY[i][j][k]);
																										}

																										FLOAT fLocal = GetPolyHeight(
																											vPOLY[i][j],
																											pPOINT,
																											pOBJ->m_fRadius);

																										if( fHeight < fLocal )
																											fHeight = fLocal;
																									}

																									return fHeight;
}

FLOAT CTMonSpawnInspectorObjBase::GetPolyHeight( CTMonSpawnInspectorObjBase *pOBJ,
												LPD3DXVECTOR3 pPOINT)
{
	if( pPOINT->y + pOBJ->m_fSizeY <= m_vPosition._42 - m_fDepth )
		return TMIN_HEIGHT;
	FLOAT fHeight = TMIN_HEIGHT;

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
					for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
						for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];

							for( int l=0; l<3; l++)
							{
								D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
									m_pBone,
									GetMeshMatrix(),
									&pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]]) : m_vPosition;

								vPOLY[l] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

								CTMath::Transform(
									&vTrans,
									&vPOLY[l]);
							}

							FLOAT fLocal = GetPolyHeight(
								vPOLY,
								pPOINT,
								pOBJ->m_fRadius);

							if( fHeight < fLocal )
								fHeight = fLocal;
						}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
					for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
						for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];

							for( int l=0; l<3; l++)
							{
								vPOLY[l] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

								CTMath::Transform(
									&m_vPosition,
									&vPOLY[l]);
							}

							FLOAT fLocal = GetPolyHeight(
								vPOLY,
								pPOINT,
								pOBJ->m_fRadius);

							if( fHeight < fLocal )
								fHeight = fLocal;
						}
			}
		}
	}

	return fHeight;
}

FLOAT CTMonSpawnInspectorObjBase::GetPolyHeight( LPD3DXVECTOR3 pPOLY,
												LPD3DXVECTOR3 pPOS,
												FLOAT fRadius)
{
	D3DXPLANE vPLANE;

	D3DXPlaneFromPoints(
		&vPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	D3DXPlaneNormalize(
		&vPLANE,
		&vPLANE);

	if( vPLANE.b > 0.0f && D3DXPlaneDotCoord( &vPLANE, pPOS) >= 0.0f )
	{
		D3DXVECTOR3 vPOS(
			vPLANE.a,
			0.0f,
			vPLANE.c);

		FLOAT fLength = D3DXVec3Length(&vPOS);
		FLOAT fDist;
		FLOAT fU;
		FLOAT fV;

		if( fLength > 0.0f )
		{
			if( fabs(vPLANE.b) / fLength < CTMonSpawnInspectorMAP::m_fMoveBound )
				return TMIN_HEIGHT;

			vPOS *= fRadius / fLength;
			vPOS = (*pPOS) - vPOS;
		}
		else
			vPOS = (*pPOS);

		if( D3DXIntersectTri(
			&pPOLY[0],
			&pPOLY[1],
			&pPOLY[2],
			&D3DXVECTOR3(
			vPOS.x,
			-TMIN_HEIGHT,
			vPOS.z),
			&D3DXVECTOR3(
			0.0f,
			-1.0f,
			0.0f),
			&fU, &fV,
			&fDist) )
		{
			fDist = -TMIN_HEIGHT - fDist;
			return pPOS->y < fDist ? TMIN_HEIGHT : fDist;
		}
		fDist = TMIN_HEIGHT;

		for( int i=0; i<3; i++)
		{
			D3DXVECTOR2 vA(
				pPOLY[(i + 1) % 3].x - pPOLY[i].x,
				pPOLY[(i + 1) % 3].z - pPOLY[i].z);

			D3DXVECTOR2 vB(
				pPOS->x - pPOLY[i].x,
				pPOS->z - pPOLY[i].z);

			D3DXVECTOR2 vC = D3DXVec2Dot( &vA, &vB) / D3DXVec2Dot( &vA, &vA) * vA - vB;
			fLength = fRadius * fRadius - D3DXVec2Dot( &vC, &vC);

			if( D3DXVec2Dot( &vB, &vB) <= fRadius * fRadius && fDist < pPOLY[i].y )
				fDist = pPOLY[i].y;

			if( fLength >= 0.0f )
			{
				FLOAT fEdge = D3DXVec2Length(&vA);

				fLength = sqrtf(fLength);
				vA /= fEdge;

				D3DXVECTOR3 vNormal( vA.x, 0.0f, vA.y);
				D3DXVECTOR3 vPOINT[2] = {
					D3DXVECTOR3( pPOS->x + vC.x + fLength * vA.x, 0.0f, pPOS->z + vC.y + fLength * vA.y),
						D3DXVECTOR3( pPOS->x + vC.x - fLength * vA.x, 0.0f, pPOS->z + vC.y - fLength * vA.y)};

					for( int j=0; j<2; j++)
					{
						D3DXPlaneFromPointNormal(
							&vPLANE,
							&vPOINT[j],
							&vNormal);

						D3DXPlaneNormalize(
							&vPLANE,
							&vPLANE);

						if( D3DXPlaneDotCoord( &vPLANE, &pPOLY[(i + 1) % 3]) * D3DXPlaneDotCoord( &vPLANE, &pPOLY[i]) <= 0.0f )
						{
							fLength = D3DXVec2Length(&D3DXVECTOR2(
								vPOINT[j].x - pPOLY[i].x,
								vPOINT[j].z - pPOLY[i].z));
							fLength = pPOLY[i].y + fLength * (pPOLY[(i + 1) % 3].y - pPOLY[i].y) / fEdge;

							if( fDist < fLength )
								fDist = fLength;
						}
					}
			}
		}

		return pPOS->y < fDist ? TMIN_HEIGHT : fDist;
	}

	return TMIN_HEIGHT;
}