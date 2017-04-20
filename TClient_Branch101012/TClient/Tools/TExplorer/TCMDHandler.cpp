#include "stdafx.h"
#include "TExplorerWnd.h"
#include "TErrorCode.h"
#include "Resource.h"


DWORD CTExplorerGame::OnTCMD_EXIT( CString strParam)
{
	PostQuitMessage(0);
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LOADCHAR( CString strParam)
{
	m_pMainWnd->InitCHAR();
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LOADMAP( CString strParam)
{
	DWORD dwMapID = 0;

	if(strParam.IsEmpty())
		return TERROR_LOADMAP;
	m_pMainWnd->m_bResetTick = TRUE;

	TRY
	{
		sscanf( LPCSTR(strParam), "%d", &dwMapID);
		m_vMAP.ReleaseMAP();

		m_vMAP.LoadMAP(
			m_pDevice,
			m_pCamera,
			m_pRES,
			WORD(dwMapID));

		if(!m_vMAP.m_pMAP)
			return TERROR_LOADMAP_NOTFOUND;

		ResetTSKY();
	}
	CATCH_ALL(e)
	{
		return TERROR_LOADMAP;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_MOVE( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MOVE;

	TRY
	{
		if(strParam.IsEmpty())
			return TERROR_MOVE;

		VECTORFLOAT vParam;

		int nPos = 0;
		CString str = strParam.Tokenize(" ",nPos);
		while( !str.IsEmpty() )
		{
			vParam.push_back( (FLOAT)::atof(str) );
			str = strParam.Tokenize(" ",nPos);
		}

		if( vParam.size() < 2 )
			return TERROR_NONE;
		else if( vParam.size() == 2 )
		{
			m_pMainChar->m_vWorld._41 = vParam[0];
			m_pMainChar->m_vWorld._43 = vParam[1];

			m_pMainChar->m_vWorld._42 = m_vMAP.GetHeight(
				&m_vMAP.m_vDRAWOBJ,
				m_pMainChar,
				NULL,
				&D3DXVECTOR3(
				m_pMainChar->m_vWorld._41,
				m_pMainChar->m_vWorld._42,
				m_pMainChar->m_vWorld._43),
				0.0f, FALSE);
		}
		else
		{
			m_pMainChar->m_vWorld._41 = vParam[0];
			m_pMainChar->m_vWorld._42 = vParam[1];
			m_pMainChar->m_vWorld._43 = vParam[2];
		}

		ResetTSKY();

		//sscanf( LPCSTR(strParam), "%f %f", &m_pMainChar->m_vWorld._41, &m_pMainChar->m_vWorld._43);
		//m_pMainChar->m_vWorld._42 = m_vMAP.GetHeight(
		//	&m_vMAP.m_vDRAWOBJ,
		//	m_pMainChar,
		//	NULL,
		//	&D3DXVECTOR3(
		//	m_pMainChar->m_vWorld._41,
		//	m_pMainChar->m_vWorld._42,
		//	m_pMainChar->m_vWorld._43),
		//	0.0f, FALSE);
		//ResetTSKY();
	}
	CATCH_ALL(e)
	{
		return TERROR_MOVE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_FPS( CString strParam)
{
	m_bFPS = !m_bFPS;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_MINIMAP( CString strParam)
{
	m_pTMinimap->ShowComponent(!m_pTMinimap->IsVisible());
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_SPEED( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_SPEED;

	TRY
	{
		sscanf( LPCSTR(strParam), "%f", &m_pMainChar->m_fSpeedFactor);
	}
	CATCH_ALL(e)
	{
		return TERROR_SPEED;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_POS( CString strParam)
{
	m_bPOS = !m_bPOS;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_HELP( CString strParam)
{
	CString strTYPE;
	CString strTMSG;

	strTYPE.LoadString(IDS_TEXT_HELP);
	strTMSG.LoadString(IDS_MSG_HELP);
	ChatMSG( strTYPE, m_strTCMD[TCMD_HELP], strTMSG, 0xFFFFFFFF);

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_PATHMODE( CString strParam)
{
	m_bPATHMODE = !m_bPATHMODE;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_DELRANGE( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_DELRANGE;

	TRY
	{
		sscanf( LPCSTR(strParam), "%f", &m_fDeleteRange);
	}
	CATCH_ALL(e)
	{
		return TERROR_DELRANGE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_BUILD( CString strParam)
{
	if( !m_vMAP.m_pMAP || !m_vMAP.m_pMAP->m_pUNIT[4] )
		return TERROR_BUILD_NOMAP;
	m_pMainWnd->m_bResetTick = TRUE;

	BYTE bUnitX = BYTE(INT(m_pMainChar->m_vPosition._41) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));
	BYTE bUnitZ = BYTE(INT(m_pMainChar->m_vPosition._43) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));

	if(m_vMAP.m_pMAP->m_pENABLE[bUnitZ * m_vMAP.m_pMAP->m_nUnitCountX + bUnitX])
	{
		MAPTPATHFLAG::iterator itTPATH = CTExplorerFlag::m_mapTPATHFLAG.begin();
		MAPMAPOBJ::iterator itTOBJ;

		VTPATHFLAG vTFLAG;
		VTPATHFLAG vTPATH;
		VTOBJBASE vTOBJ;

		while(itTPATH != CTExplorerFlag::m_mapTPATHFLAG.end())
		{
			MAPTPATHFLAG::iterator itNEXT = itTPATH;
			itNEXT++;

			if( HIWORD((*itTPATH).second->m_dwID) == MAKEWORD( bUnitX, bUnitZ) )
				delete (*itTPATH).second;

			itTPATH = itNEXT;
		}

		vTFLAG.clear();
		vTPATH.clear();
		vTOBJ.clear();

		for( itTOBJ = m_vMAP.m_pMAP->m_pUNIT[4]->m_mapOBJ.begin(); itTOBJ != m_vMAP.m_pMAP->m_pUNIT[4]->m_mapOBJ.end(); itTOBJ++)
			switch(m_vMAP.GetAttrINT( (*itTOBJ).second, ID_OBJ_TYPE))
			{
			case OT_FOG	:
			case OT_BSP	: break;
			default		:
				if( m_vMAP.GetAttrINT( (*itTOBJ).second, ID_COLLISION_TYPE) != TCOLLISION_NONE )
				{
					CTExplorerObject *pINST = new CTExplorerObject();
					pINST->ReleaseData();

					pINST->InitOBJ( m_pDevice, (*itTOBJ).second);
					pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
					pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
					pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
					pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);

					pINST->InitSIZE(
						(*itTOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
						(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
						(*itTOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
						(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH),
						max( (*itTOBJ).second->m_vSCALE.x, max(
						(*itTOBJ).second->m_vSCALE.y,
						(*itTOBJ).second->m_vSCALE.z)) * pINST->GetAttrFLOAT(ID_SIGHT));
					pINST->CalcTick( m_pDevice->m_pDevice, 0);

					vTOBJ.push_back(pINST);
				}

				break;
			}

		for( int i=0; i<INT(vTOBJ.size()); i++)
			switch(vTOBJ[i]->m_bCollisionType)
			{
			case TCOLLISION_CYLINDER	:
				{
					FLOAT fRadiusOBJ = vTOBJ[i]->m_fRadius;

					D3DXVECTOR3 vPOS( TINV_SIN60 * fRadiusOBJ + TMOVE_RADIUS, 0.0f, 0.0f);
					D3DXMATRIX vMAT;

					D3DXMatrixRotationY(
						&vMAT,
						D3DX_PI / 3.0f);

					for( int j=0; j<6; j++)
					{
						CTMath::Transform(
							&vMAT,
							&vPOS);

						D3DXVECTOR3 vPOINT(
							vPOS.x + vTOBJ[i]->m_vPosition._41,
							vTOBJ[i]->m_vPosition._42 + TPATH_HEIGHT,
							vPOS.z + vTOBJ[i]->m_vPosition._43);

						if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOINT))
						{
							CTExplorerFlag *pTFLAG = new CTExplorerFlag();
							FLOAT fHeight = m_vMAP.GetHeight(
								&vTOBJ,
								m_pMainChar,
								NULL,
								&D3DXVECTOR3(
								vPOINT.x,
								vPOINT.y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
								vPOINT.z),
								0.0f, FALSE);

							pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
							pTFLAG->m_vPOS = vPOINT;
							pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

							vTFLAG.push_back(pTFLAG);
						}
					}
				}

				break;

			case TCOLLISION_BOX			:
				{
					D3DXVECTOR3 vPOINT[8] = {
						D3DXVECTOR3(  vTOBJ[i]->m_fSizeX / 2.0f + TMOVE_RADIUS,  vTOBJ[i]->m_fSizeY + TMOVE_RADIUS,  vTOBJ[i]->m_fSizeZ / 2.0f + TMOVE_RADIUS),
						D3DXVECTOR3( -vTOBJ[i]->m_fSizeX / 2.0f - TMOVE_RADIUS,  vTOBJ[i]->m_fSizeY + TMOVE_RADIUS,  vTOBJ[i]->m_fSizeZ / 2.0f + TMOVE_RADIUS),
						D3DXVECTOR3( -vTOBJ[i]->m_fSizeX / 2.0f - TMOVE_RADIUS,  vTOBJ[i]->m_fSizeY + TMOVE_RADIUS, -vTOBJ[i]->m_fSizeZ / 2.0f - TMOVE_RADIUS),
						D3DXVECTOR3(  vTOBJ[i]->m_fSizeX / 2.0f + TMOVE_RADIUS,  vTOBJ[i]->m_fSizeY + TMOVE_RADIUS, -vTOBJ[i]->m_fSizeZ / 2.0f - TMOVE_RADIUS),
						D3DXVECTOR3(  vTOBJ[i]->m_fSizeX / 2.0f + TMOVE_RADIUS, -vTOBJ[i]->m_fDepth - TMOVE_RADIUS,  vTOBJ[i]->m_fSizeZ / 2.0f + TMOVE_RADIUS),
						D3DXVECTOR3( -vTOBJ[i]->m_fSizeX / 2.0f - TMOVE_RADIUS, -vTOBJ[i]->m_fDepth - TMOVE_RADIUS,  vTOBJ[i]->m_fSizeZ / 2.0f + TMOVE_RADIUS),
						D3DXVECTOR3( -vTOBJ[i]->m_fSizeX / 2.0f - TMOVE_RADIUS, -vTOBJ[i]->m_fDepth - TMOVE_RADIUS, -vTOBJ[i]->m_fSizeZ / 2.0f - TMOVE_RADIUS),
						D3DXVECTOR3(  vTOBJ[i]->m_fSizeX / 2.0f + TMOVE_RADIUS, -vTOBJ[i]->m_fDepth - TMOVE_RADIUS, -vTOBJ[i]->m_fSizeZ / 2.0f - TMOVE_RADIUS)};

					D3DXQUATERNION vQUAT;
					D3DXMATRIX vMAT;

					CTMath::GetRotationQUAT(
						&vQUAT,
						&vTOBJ[i]->m_vPosition);

					D3DXMatrixRotationQuaternion(
						&vMAT,
						&vQUAT);

					vMAT._41 = vTOBJ[i]->m_vPosition._41;
					vMAT._42 = vTOBJ[i]->m_vPosition._42;
					vMAT._43 = vTOBJ[i]->m_vPosition._43;

					for( int j=0; j<8; j++)
					{
						CTMath::Transform(
							&vMAT,
							&vPOINT[j]);
						vPOINT[j].y = vTOBJ[i]->m_vPosition._42 + TPATH_HEIGHT;

						if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOINT[j]))
						{
							CTExplorerFlag *pTFLAG = new CTExplorerFlag();
							FLOAT fHeight = m_vMAP.GetHeight(
								&vTOBJ,
								m_pMainChar,
								NULL,
								&D3DXVECTOR3(
								vPOINT[j].x,
								vPOINT[j].y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
								vPOINT[j].z),
								0.0f, FALSE);

							pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
							pTFLAG->m_vPOS = vPOINT[j];
							pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

							vTFLAG.push_back(pTFLAG);
						}
					}
				}

				break;

			case TCOLLISION_POLY		:
				{
					LPANI pANI = vTOBJ[i]->GetCurANI();
					MAPCLKINST::iterator itCLK;

					for( itCLK = vTOBJ[i]->m_OBJ.m_mapCLK.begin(); itCLK != vTOBJ[i]->m_OBJ.m_mapCLK.end(); itCLK++)
					{
						LPCLKINST pCI = (*itCLK).second;

						if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
							pCI->m_pMESH->m_pMESH->m_pDATA &&
							pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
						{
							CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

							if( pMESH->m_dwNodeCount > 0 )
							{
								LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

								for( int j=0; j<pMESH->m_dwMeshCount; j++)
									for( int k=0; k<INT(pMESH->m_pMESH[j][0]->m_vIB.size()); k++)
										for( int l=0; l<pMESH->m_pMESH[j][0]->m_vIB[k]->m_dwCount / 3; l++)
										{
											D3DXVECTOR3 vPOLY[3];
											D3DXVECTOR3 vNORMAL;

											D3DXVECTOR3 vPOINT(
												0.0f,
												0.0f,
												0.0f);

											for( int m=0; m<3; m++)
											{
												D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
													vTOBJ[i]->m_pBone,
													vTOBJ[i]->GetMeshMatrix(),
													&pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]]) : vTOBJ[i]->m_vPosition;

												vPOLY[m] = D3DXVECTOR3(
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosX,
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosY,
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosZ);

												CTMath::Transform(
													&vTrans,
													&vPOLY[m]);
												vPOINT += vPOLY[m];
											}

											D3DXVec3Cross(
												&vNORMAL,
												&(vPOLY[1] - vPOLY[0]),
												&(vPOLY[2] - vPOLY[0]));

											vNORMAL /= D3DXVec3Length(&vNORMAL);
											if( fabs(vNORMAL.y) < 0.1f )
											{
												vNORMAL.y = 0.0f;
												vNORMAL /= D3DXVec3Length(&vNORMAL);
												vNORMAL *= TMOVE_RADIUS;

												vPOINT /= 3.0f;
												vPOINT += vNORMAL;

												if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOINT))
												{
													CTExplorerFlag *pTFLAG = new CTExplorerFlag();
													FLOAT fHeight = m_vMAP.GetHeight(
														&vTOBJ,
														m_pMainChar,
														NULL,
														&D3DXVECTOR3(
														vPOINT.x,
														vPOINT.y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
														vPOINT.z),
														0.0f, FALSE);

													pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
													pTFLAG->m_vPOS = vPOINT;
													pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

													vTFLAG.push_back(pTFLAG);
												}

												for( m=0; m<3; m++)
												{
													vPOLY[m] += vNORMAL;

													if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOLY[m]))
													{
														CTExplorerFlag *pTFLAG = new CTExplorerFlag();
														FLOAT fHeight = m_vMAP.GetHeight(
															&vTOBJ,
															m_pMainChar,
															NULL,
															&D3DXVECTOR3(
															vPOLY[m].x,
															vPOLY[m].y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
															vPOLY[m].z),
															0.0f, FALSE);

														pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
														pTFLAG->m_vPOS = vPOLY[m];
														pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

														vTFLAG.push_back(pTFLAG);
													}
												}
											}
										}
							}
							else
							{
								LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

								for( int j=0; j<pMESH->m_dwMeshCount; j++)
									for( int k=0; k<INT(pMESH->m_pMESH[j][0]->m_vIB.size()); k++)
										for( int l=0; l<pMESH->m_pMESH[j][0]->m_vIB[k]->m_dwCount / 3; l++)
										{
											D3DXVECTOR3 vPOLY[3];
											D3DXVECTOR3 vNORMAL;

											D3DXVECTOR3 vPOINT(
												0.0f,
												0.0f,
												0.0f);

											for( int m=0; m<3; m++)
											{
												vPOLY[m] = D3DXVECTOR3(
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosX,
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosY,
													pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosZ);

												CTMath::Transform(
													&vTOBJ[i]->m_vPosition,
													&vPOLY[m]);
												vPOINT += vPOLY[m];
											}

											D3DXVec3Cross(
												&vNORMAL,
												&(vPOLY[1] - vPOLY[0]),
												&(vPOLY[2] - vPOLY[0]));

											vNORMAL /= D3DXVec3Length(&vNORMAL);
											if( fabs(vNORMAL.y) < 0.1f )
											{
												vNORMAL.y = 0.0f;
												vNORMAL /= D3DXVec3Length(&vNORMAL);
												vNORMAL *= TMOVE_RADIUS;

												vPOINT /= 3.0f;
												vPOINT += vNORMAL;

												if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOINT))
												{
													CTExplorerFlag *pTFLAG = new CTExplorerFlag();
													FLOAT fHeight = m_vMAP.GetHeight(
														&vTOBJ,
														m_pMainChar,
														NULL,
														&D3DXVECTOR3(
														vPOINT.x,
														vPOINT.y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
														vPOINT.z),
														0.0f, FALSE);

													pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
													pTFLAG->m_vPOS = vPOINT;
													pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

													vTFLAG.push_back(pTFLAG);
												}

												for( m=0; m<3; m++)
												{
													vPOLY[m] += vNORMAL;

													if(CTExplorerFlag::IsValidPoint( vTOBJ, &vPOLY[m]))
													{
														CTExplorerFlag *pTFLAG = new CTExplorerFlag();
														FLOAT fHeight = m_vMAP.GetHeight(
															&vTOBJ,
															m_pMainChar,
															NULL,
															&D3DXVECTOR3(
															vPOLY[m].x,
															vPOLY[m].y + TCOLLISION_HEIGHT - TPATH_HEIGHT,
															vPOLY[m].z),
															0.0f, FALSE);

														pTFLAG->m_vNAME.SetFont(CTExplorerObjBase::m_pFONT);
														pTFLAG->m_vPOS = vPOLY[m];
														pTFLAG->m_vPOS.y = fHeight + TPATH_HEIGHT;

														vTFLAG.push_back(pTFLAG);
													}
												}
											}
										}
							}
						}
					}
				}

				break;
			}

		while(!vTOBJ.empty())
		{
			delete vTOBJ.back();
			vTOBJ.pop_back();
		}

		while(!vTFLAG.empty())
		{
			CTExplorerFlag *pTFLAG = vTFLAG.back();
			BYTE bValid = TRUE;

			for( i=0; i<INT(vTPATH.size()); i++)
				if( D3DXVec3Length(&(pTFLAG->m_vPOS - vTPATH[i]->m_vPOS)) < 0.7f )
				{
					bValid = FALSE;
					break;
				}

			if(bValid)
			{
				CTExplorerFlag::RegTPATH(
					pTFLAG,
					bUnitX,
					bUnitZ);
				vTPATH.push_back(pTFLAG);
			}
			else
				delete pTFLAG;

			vTFLAG.pop_back();
		}

		vTPATH.clear();
	}

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_FINDPATH( CString strParam)
{
	if( !m_vMAP.m_pMAP || !m_vMAP.m_pMAP->m_pUNIT[4] )
		return TERROR_BUILD_NOMAP;
	m_pMainWnd->m_bResetTick = TRUE;

	BYTE bUnitX = BYTE(INT(m_pMainChar->m_vPosition._41) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));
	BYTE bUnitZ = BYTE(INT(m_pMainChar->m_vPosition._43) / (m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength));

	if(m_vMAP.m_pMAP->m_pENABLE[bUnitZ * m_vMAP.m_pMAP->m_nUnitCountX + bUnitX])
	{
		MAPTPATHFLAG::iterator itTPATH = CTExplorerFlag::m_mapTPATHFLAG.begin();
		MAPMAPOBJ::iterator itTOBJ;

		VTPATHFLAG vTFLAG;
		VTPATHFLAG vTPATH;
		VTOBJBASE vTOBJ;

		vTFLAG.clear();
		vTPATH.clear();
		vTOBJ.clear();

		while(itTPATH != CTExplorerFlag::m_mapTPATHFLAG.end())
		{
			MAPTPATHFLAG::iterator itNEXT = itTPATH;
			itNEXT++;

			if( HIWORD((*itTPATH).second->m_dwID) == MAKEWORD( bUnitX, bUnitZ) )
			{
				vTFLAG.push_back((*itTPATH).second);
				(*itTPATH).second->m_vPATH.clear();
			}

			itTPATH = itNEXT;
		}

		for( itTOBJ = m_vMAP.m_pMAP->m_pUNIT[4]->m_mapOBJ.begin(); itTOBJ != m_vMAP.m_pMAP->m_pUNIT[4]->m_mapOBJ.end(); itTOBJ++)
			switch(m_vMAP.GetAttrINT( (*itTOBJ).second, ID_OBJ_TYPE))
			{
			case OT_FOG	:
			case OT_BSP	: break;
			default		:
				if( m_vMAP.GetAttrINT( (*itTOBJ).second, ID_COLLISION_TYPE) != TCOLLISION_NONE )
				{
					CTExplorerObject *pINST = new CTExplorerObject();
					pINST->ReleaseData();

					pINST->InitOBJ( m_pDevice, (*itTOBJ).second);
					pINST->m_bCollisionType = pINST->GetAttrINT(ID_COLLISION_TYPE);
					pINST->m_bCamCollision = pINST->GetAttrINT(ID_CAM_COLLISION);
					pINST->m_dwID = pINST->GetAttrINT(ID_MAP_ID);
					pINST->m_bType = pINST->GetAttrINT(ID_OBJ_TYPE);

					pINST->InitSIZE(
						(*itTOBJ).second->m_vSCALE.x * pINST->GetAttrFLOAT(ID_SIZE_X),
						(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_SIZE_Y),
						(*itTOBJ).second->m_vSCALE.z * pINST->GetAttrFLOAT(ID_SIZE_Z),
						(*itTOBJ).second->m_vSCALE.y * pINST->GetAttrFLOAT(ID_DEPTH),
						max( (*itTOBJ).second->m_vSCALE.x, max(
						(*itTOBJ).second->m_vSCALE.y,
						(*itTOBJ).second->m_vSCALE.z)) * pINST->GetAttrFLOAT(ID_SIGHT));
					pINST->CalcTick( m_pDevice->m_pDevice, 0);

					vTOBJ.push_back(pINST);
				}

				break;
			}

		while(!vTFLAG.empty())
		{
			CTExplorerFlag *pTFLAG = vTFLAG.back();
			BYTE bValid = TRUE;

			for( int i=0; i<INT(vTPATH.size()); i++)
				if( D3DXVec3Length(&(pTFLAG->m_vPOS - vTPATH[i]->m_vPOS)) < 0.7f )
				{
					bValid = FALSE;
					break;
				}

			if(bValid)
				vTPATH.push_back(pTFLAG);
			else
				delete pTFLAG;

			vTFLAG.pop_back();
		}

		for( int i=0; i<INT(vTPATH.size()); i++)
			for( int j=0; j<INT(vTPATH.size()); j++)
			{
				FLOAT fDistH = D3DXVec2Length(&D3DXVECTOR2(
					vTPATH[i]->m_vPOS.x - vTPATH[j]->m_vPOS.x,
					vTPATH[i]->m_vPOS.z - vTPATH[j]->m_vPOS.z));
				FLOAT fDIST = D3DXVec3Length(&(vTPATH[i]->m_vPOS - vTPATH[j]->m_vPOS));

				if( i != j && fDIST < THIT_RANGE && fDistH / fDIST > cosf(D3DX_PI / 6.0f) &&
					m_vMAP.CanMove( vTOBJ, m_pMainChar, &vTPATH[i]->m_vPOS, &vTPATH[j]->m_vPOS) )
					vTPATH[i]->m_vPATH.push_back(vTPATH[j]->m_dwID);
			}

		while(!vTPATH.empty())
		{
			CTExplorerFlag *pTFLAG = vTPATH.back();

			if(pTFLAG->m_vPATH.empty())
				delete pTFLAG;
			else
				vTFLAG.push_back(pTFLAG);

			vTPATH.pop_back();
		}

		while(!vTOBJ.empty())
		{
			delete vTOBJ.back();
			vTOBJ.pop_back();
		}

		CString strFile;
		strFile.Format( IDS_FMT_PATHFILE, MAKELONG( MAKEWORD( bUnitX, bUnitZ), m_vMAP.m_wMapID));

		CFile file( LPCSTR(strFile), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		CArchive ar( &file, CArchive::store);

		ar << DWORD(vTFLAG.size());
		while(!vTFLAG.empty())
		{
			CTExplorerFlag *pTFLAG = vTFLAG.back();

			ar	<< pTFLAG->m_dwID
				<< pTFLAG->m_vPOS.x
				<< pTFLAG->m_vPOS.y
				<< pTFLAG->m_vPOS.z
				<< DWORD(pTFLAG->m_vPATH.size());

			for( i=0; i<INT(pTFLAG->m_vPATH.size()); i++)
				ar << DWORD(pTFLAG->m_vPATH[i]);

			vTFLAG.pop_back();
		}
	}

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_PATH( CString strParam)
{
	m_bPATHFLAG = !m_bPATHFLAG;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LIGHT( CString strParam)
{
	m_bLIGHT = !m_bLIGHT;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_SHADOW( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_SHADOW;

	TRY
	{
		FLOAT fSHADOW;

		sscanf( LPCSTR(strParam), _T("%f"), &fSHADOW);
		m_vMAP.m_bSHADOW = BYTE(fSHADOW * 0xFF);

		if(m_vMAP.m_pMAP)
			m_vMAP.m_pMAP->m_bSHADOW = m_vMAP.m_bSHADOW;
	}
	CATCH_ALL(e)
	{
		return TERROR_SHADOW;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_MATERIALA( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MATERIALA;

	TRY
	{
		sscanf( LPCSTR(strParam), _T("%f %f %f"), &m_vShadow.Ambient.r, &m_vShadow.Ambient.g, &m_vShadow.Ambient.b);
	}
	CATCH_ALL(e)
	{
		return TERROR_MATERIALA;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_MATERIALD( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MATERIALD;

	TRY
	{
		sscanf( LPCSTR(strParam), _T("%f %f %f"), &m_vShadow.Diffuse.r, &m_vShadow.Diffuse.g, &m_vShadow.Diffuse.b);
	}
	CATCH_ALL(e)
	{
		return TERROR_MATERIALD;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_MAPCOLOR( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_MAPCOLOR;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;

		sscanf( LPCSTR(strParam), _T("%f %f %f"), &fRValue, &fGValue, &fBValue);
		m_dwTMapCOLOR = D3DCOLOR_ARGB( 0xFF, BYTE(fRValue * 0xFF), BYTE(fGValue * 0xFF), BYTE(fBValue * 0xFF));
	}
	CATCH_ALL(e)
	{
		return TERROR_MAPCOLOR;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LANDCOLLISION( CString strParam)
{
	m_bLandCollision = !m_bLandCollision;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LIGHTMODE( CString strParam)
{
	m_bTLIGHTMode = !m_bTLIGHTMode;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LIGHTA( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_AMBIENT;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &fRValue, &fGValue, &fBValue);
		if( dwIndex < TLIGHT_COUNT )
		{
			m_vLIGHT[dwIndex].m_Light.Ambient.r = fRValue;
			m_vLIGHT[dwIndex].m_Light.Ambient.g = fGValue;
			m_vLIGHT[dwIndex].m_Light.Ambient.b = fBValue;

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_AMBIENT;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LIGHTD( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_DIFFUSE;

	TRY
	{
		FLOAT fRValue = 0.0f;
		FLOAT fGValue = 0.0f;
		FLOAT fBValue = 0.0f;
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &fRValue, &fGValue, &fBValue);
		if( dwIndex < TLIGHT_COUNT )
		{
			m_vLIGHT[dwIndex].m_Light.Diffuse.r = fRValue;
			m_vLIGHT[dwIndex].m_Light.Diffuse.g = fGValue;
			m_vLIGHT[dwIndex].m_Light.Diffuse.b = fBValue;

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_DIFFUSE;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_LIGHTDIR( CString strParam)
{
	if(strParam.IsEmpty())
		return TERROR_LIGHTDIR;

	TRY
	{
		D3DXVECTOR3 vDIR( 0.0f, 0.0f, 0.0f);
		DWORD dwIndex = 0;

		sscanf( LPCSTR(strParam), _T("%d %f %f %f"), &dwIndex, &vDIR.x, &vDIR.y, &vDIR.z);
		if( dwIndex < TLIGHT_COUNT )
		{
			m_vLIGHT[dwIndex].m_Light.Direction.x = vDIR.x;
			m_vLIGHT[dwIndex].m_Light.Direction.y = vDIR.y;
			m_vLIGHT[dwIndex].m_Light.Direction.z = vDIR.z;

			m_vLIGHT[dwIndex].SetLight(&m_vLIGHT[dwIndex].m_Light);
		}
	}
	CATCH_ALL(e)
	{
		return TERROR_LIGHTDIR;
	}
	END_CATCH_ALL

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_IMPOSTER( CString strParam )
{
	m_bUseImposter = !m_bUseImposter;
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_SAVEIMPOSTER( CString strParam )
{
	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_FOGON( CString strParam )
{
	m_bFogRender = TRUE;

	return TERROR_NONE;
}

DWORD CTExplorerGame::OnTCMD_FOGOFF( CString strParam )
{
	m_bFogRender = FALSE;

	return TERROR_NONE;
}
