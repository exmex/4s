#include "stdafx.h"
#include "TExplorer.h"

LPDIRECT3DVERTEXBUFFER9 CTExplorerObjBase::m_pTSQUARE = NULL;
LPTEXTURESET CTExplorerObjBase::m_pTSHADOW = NULL;
FLOAT CTExplorerObjBase::m_fCamDist = 0.0f;
CFont *CTExplorerObjBase::m_pFONT = NULL;

CTExplorerObjBase::CTExplorerObjBase()
{
	m_bCollisionType = TCOLLISION_NONE;
	m_bRootID = 0;
	m_dwID = 0;

	m_vTGARBAGESFX.clear();
	m_vTACTIONSFX.clear();

	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();
	m_mapTSUBACTSND.clear();
	m_mapTSUBANISND.clear();

	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();
	m_mapTSUBACTSFX.clear();
	m_mapTSUBANISFX.clear();
	m_vTSUBACTSFX.clear();
	m_vTSUBANISFX.clear();

	m_fSpeedFactor = 1.0f;
	m_fSubActTime = 0.0f;
	m_fCamDIST = 0.0f;
	m_fRadiusH = 0.0f;
	m_fRadius = 0.0f;
	m_fZValue = 0.0f;
	m_fRange = 0.0f;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;
	m_fSizeZ = 0.0f;
	m_fDepth = 0.0f;
	m_fSight = 0.0f;

	m_bColorID = TNCOLOR_ALLI;
	m_bMode = MT_NORMAL;
	m_bType = OT_NONE;

	m_bCamCollision = FALSE;
	m_bDrawShadow = FALSE;
	m_bInSHADOW = FALSE;
	m_bDrawName = FALSE;
	m_bSTEPSND = FALSE;
	m_bVisible = FALSE;
	m_bFPView = FALSE;
	m_bHide = FALSE;

	m_dwSHADOWTick = 0;
	m_dwAlphaTick = 0;

	m_nSubKeyBase = -1;
	m_dwTSUBACT = 0;
	m_pTREGION = NULL;
}

CTExplorerObjBase::~CTExplorerObjBase()
{
	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();
	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();

	RemoveSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	RemoveSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);

	CTachyonObject::ClearSND(&m_mapTSUBACTSND);
	CTachyonObject::ClearSND(&m_mapTSUBANISND);
	ClearInven();

	m_vTSUBACT.m_dwCurANI = 0;
	m_vTSUBACT.m_pANI = NULL;
	m_dwTSUBACT = 0;
	m_pTREGION = NULL;

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);
}

void CTExplorerObjBase::Release()
{
	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);
	ClearInven();

	m_vTSUBACT.m_dwCurANI = 0;
	m_vTSUBACT.m_pANI = NULL;
	m_dwTSUBACT = 0;
	m_bRootID = 0;
	m_pTREGION = NULL;

	m_dwActionID = 0;
	m_dwAniID = 0;
	m_dwID = 0;

	CTachyonObject::Release();
}

void CTExplorerObjBase::ClearOBJSFX()
{
	CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	CTachyonObject::ClearOBJSFX();
}

void CTExplorerObjBase::ClearACTSFX()
{
	CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	CTachyonObject::ClearACTSFX();
}

void CTExplorerObjBase::CalcOBJSFX( CD3DDevice *pDevice)
{
	CalcSFX( pDevice, &m_vTSUBACTSFX);
	CalcSFX( pDevice, &m_vTSUBANISFX);
	CTachyonObject::CalcOBJSFX(pDevice);
}

void CTExplorerObjBase::ClearSND()
{
	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();

	CTachyonObject::ClearSND(&m_mapTSUBACTSND);
	CTachyonObject::ClearSND(&m_mapTSUBANISND);
	CTachyonObject::ClearSND();
}

void CTExplorerObjBase::ClearSFX()
{
	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();

	RemoveSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	RemoveSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	CTachyonObject::ClearSFX();
}

void CTExplorerObjBase::ReleaseData()
{
	m_bEquipMode = TEM_NORMAL;
	m_bAction = TA_STAND;
	m_bACTLevel = 0;

	m_bHouseMesh = FALSE;
	m_bHouseOUT = FALSE;
	m_bInSHADOW = FALSE;
	m_bPrevSwim = FALSE;
	m_bSTEPSND = FALSE;
	m_bVisible = FALSE;
	m_bFPView = FALSE;
	m_bShoot = FALSE;
	m_bLand = FALSE;
	m_bSwim = FALSE;
	m_bHide = FALSE;

	SetSubAction( 0, 0);
	m_dwActionID = 0;
	m_dwAniID = 0;

	m_fSquareHeight = 0.0f;
	m_fSpeedFactor = 1.0f;
	m_fCamDIST = 0.0f;
	m_fRadiusH = 0.0f;
	m_fRadius = 0.0f;
	m_fZValue = 0.0f;
	m_fRange = 0.0f;
	m_fSight = 0.0f;

	m_wMoveDIR = MOVE_NONE;
	m_wPITCH = 0;
	m_wDIR = 0;

	m_dwSHADOWTick = 0;
	m_dwAlphaTick = 0;
	m_dwHouseID = 0;

	for( BYTE i=0; i<TWEAPON_COUNT; i++)
		m_vWeaponPOS[i] = 0xFFFFFFFF;

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);
	ClearInven();
	m_bRootID = 0;

	m_vNAME.SetFont(m_pFONT);
	m_vNAME.m_bOutLine = TRUE;
	m_pTREGION = NULL;
}

void CTExplorerObjBase::InitSIZE( FLOAT fSizeX,
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

void CTExplorerObjBase::ResetWeaponPOS( CD3DDevice *pDevice,
									    CTachyonRes *pRES,
										BYTE bEquipMode)
{
	static BYTE vWEAPON[TWEAPON_COUNT] = {
		ES_PRMWEAPON,
		ES_SNDWEAPON,
		ES_LONGWEAPON};

	CTExplorerInven *pTEQUIP = FindTInven(INVEN_EQUIP);
	CTachyonObject *pTOBJ[TWEAPON_COUNT];
	CTExplorerItem *pTITEM[TWEAPON_COUNT];

	for( BYTE i=0; i<TWEAPON_COUNT; i++)
	{
		MAPOBJECT::iterator finder = m_mapEQUIP.find(m_vWeaponPOS[i]);
		pTITEM[i] = pTEQUIP->FindTItem(vWEAPON[i]);

		if( finder != m_mapEQUIP.end() )
		{
			pTOBJ[i] = (*finder).second;
			m_mapEQUIP.erase(finder);
		}
		else
			pTOBJ[i] = NULL;

		if( pTITEM[i] )
		{
			LPTITEMVISUAL pTVISUAL = pTITEM[i]->GetVisual();
			if( pTVISUAL && !pTVISUAL->m_dwObjectID )
				SetCloth( pDevice, pTVISUAL->m_dwCLKID, 0, 0);
		}

		m_vWeaponPOS[i] = 0xFFFFFFFF;
	}

	for( i=0; i<TWEAPON_COUNT; i++)
	{
		BYTE bMode = GetEquipModeIndex(
			vWEAPON[i],
			bEquipMode);

		if( pTITEM[i] && pTITEM[i]->m_pTITEM )
		{
			LPTITEMVISUAL pTVISUAL = pTITEM[i]->GetVisual();
			if( pTVISUAL )
			{
				if( pTVISUAL->m_dwObjectID )
				{
					MAPDWORD::iterator itID = m_OBJ.m_pOBJ
						->m_mapPIVOT.find(pTVISUAL->m_dwPivot[bMode]);

					if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() &&
						pTVISUAL->m_dwMesh[bMode] )
					{
						m_vWeaponPOS[i] = (*itID).second + 1;

						if( !pTOBJ[i] )
						{
							pTOBJ[i] = new CTachyonObject();

							pTOBJ[i]->InitOBJ(pRES->GetOBJ(pTVISUAL->m_dwObjectID));
							
							pTOBJ[i]->SetCloth(
								pDevice,
								pTVISUAL->m_dwCLKID,
								pTVISUAL->m_dwCLIID,
								pTVISUAL->m_dwMesh[bMode]);

							pTOBJ[i]->ResetOBJPart(pDevice);
						}

						D3DXMatrixIdentity(&pTOBJ[i]->m_vWorld);

						m_mapEQUIP.insert( MAPOBJECT::value_type( m_vWeaponPOS[i], pTOBJ[i]));
					}
					else if(pTOBJ[i])
					{
						delete pTOBJ[i];
						pTOBJ[i] = NULL;
					}
				}
				else
				{
					if( pTOBJ[i] )
					{
						delete pTOBJ[i];
						pTOBJ[i] = NULL;
					}

					SetCloth(
						pDevice,
						pTVISUAL->m_dwCLKID,
						pTVISUAL->m_dwCLIID,
						pTVISUAL->m_dwMesh[bMode]);
				}
			}
		}
		else if(pTOBJ[i])
		{
			delete pTOBJ[i];
			pTOBJ[i] = NULL;
		}
	}

	m_bEquipMode = bEquipMode;
	ResetOBJPart(pDevice);
}

void CTExplorerObjBase::ResetEQUIP( CD3DDevice *pDevice,
								    CTachyonRes *pRES)
{
	static BYTE vREPLACE[TREPLACE_COUNT] = {
		ES_HEAD,
		ES_BACK,
		ES_BODY,
		ES_PANTS,
		ES_FOOT,
		ES_HAND,
		ES_FACE};

	static DWORD vCLEAR[TCLEAR_COUNT] = {
		ID_CLK_RHAND_WEAPON,
		ID_CLK_LHAND_WEAPON,
		ID_CLK_LONG_WEAPON,
		ID_CLK_HEAD,
		ID_CLK_BACK};
	CTExplorerInven *pTEQUIP = FindTInven(INVEN_EQUIP);

	for( BYTE i=0; i<TCLEAR_COUNT; i++)
		SetCloth( pDevice, vCLEAR[i], 0, 0);

	for( i=0; i<TWEAPON_COUNT; i++)
		m_vWeaponPOS[i] = 0xFFFFFFFF;
	ClearEquip();

	for( i=0; i<TREPLACE_COUNT; i++)
	{
		BOOL bUseDefault = TRUE;

		CTExplorerItem *pTITEM = pTEQUIP ? pTEQUIP->FindTItem(vREPLACE[i]) : NULL;

		if(pTITEM)
		{
			LPTITEMVISUAL pTVISUAL =  pTITEM->GetVisual();
			if( pTVISUAL )
			{
				BYTE bMode = GetEquipModeIndex(vREPLACE[i], m_bEquipMode);
				
				SetCloth(
					pDevice, 
					pTVISUAL->m_dwCLKID, 
					pTVISUAL->m_dwCLIID, 
					pTVISUAL->m_dwMesh[bMode]);

				bUseDefault = FALSE;
			}
		}
		
		if( bUseDefault )
			ResetDEFEQUIP( pDevice, vREPLACE[i]);
	}

	ResetWeaponPOS(
		pDevice,
		pRES,
		m_bEquipMode);
}

BYTE CTExplorerObjBase::GetEquipModeIndex( BYTE bEquipPOS,
										   BYTE bEquipMode)
{
	switch(bEquipPOS)
	{
	case ES_PRMWEAPON	:
	case ES_SNDWEAPON	:
		switch(bEquipMode)
		{
		case TEM_NORMAL	:
		case TEM_LONG	: return MT_NORMAL;
		case TEM_WEAPON	: return MT_BATTLE;
		}

		break;

	case ES_LONGWEAPON	:
		switch(bEquipMode)
		{
		case TEM_NORMAL	:
		case TEM_WEAPON	: return MT_NORMAL;
		case TEM_LONG	: return MT_BATTLE;
		}

		break;
	}

	return MT_NORMAL;
}

void CTExplorerObjBase::ResetOBJPart( CD3DDevice *pDevice)
{
	CTExplorerInven *pTEQUIP = FindTInven(INVEN_EQUIP);

	if(pTEQUIP)
		for( BYTE i=0; i<ES_COUNT; i++)
		{
			THIDEPART vTHIDE = FindHidePart( pTEQUIP->FindTItem(i), i);

			if(CheckOBJPart(&vTHIDE))
			{
				CTExplorerItem *pHIDE = pTEQUIP->FindTItem(vTHIDE.m_bHideSlotID);

				if( pHIDE && pHIDE->m_pTITEM )
				{
					LPTITEMVISUAL pTVISUAL = pHIDE->GetVisual();
					if( pTVISUAL )
					{
						LPCLKINST pCI = NULL;

						if( pTVISUAL->m_dwObjectID )
						{
							BYTE bMode = GetEquipModeIndex( i, m_bEquipMode);

							MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(pTVISUAL->m_dwPivot[bMode]);
							if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
							{
								MAPOBJECT::iterator itTOBJ = m_mapEQUIP.find((*itID).second + 1);

								if( itTOBJ != m_mapEQUIP.end() )
								{
									MAPCLKINST::iterator itCI = (*itTOBJ).second
										->m_OBJ.m_mapCLK.find(pTVISUAL->m_dwCLKID);

									if( itCI != (*itTOBJ).second->m_OBJ.m_mapCLK.end() )
										pCI = (*itCI).second;
								}
							}
						}
						else
						{
							MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(pTVISUAL->m_dwCLKID);

							if( itCI != m_OBJ.m_mapCLK.end() )
								pCI = (*itCI).second;
						}

						if( pCI )
							pCI->m_dwHideID = vTHIDE.m_bHidePartID;
					}
				}
				else
					ResetDEFOBJPart(&vTHIDE);
			}

			//if(CheckOBJPart(&vTHIDE))
			//{
			//	CTExplorerItem *pHIDE = pTEQUIP->FindTItem(vTHIDE.m_bHideSlotID);

			//	if( pHIDE && pHIDE->m_pTITEM )
			//	{
			//		LPCLKINST pCI = NULL;

			//		if(pHIDE->m_pTITEM->m_dwOBJ)
			//		{
			//			BYTE bMode = GetEquipModeIndex( i, m_bEquipMode);

			//			MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(pHIDE->m_pTITEM->m_dwPivot[bMode]);
			//			if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
			//			{
			//				MAPOBJECT::iterator itTOBJ = m_mapEQUIP.find((*itID).second + 1);

			//				if( itTOBJ != m_mapEQUIP.end() )
			//				{
			//					MAPCLKINST::iterator itCI = (*itTOBJ).second->m_OBJ.m_mapCLK.find(pHIDE->m_pTITEM->m_dwCLK);

			//					if( itCI != (*itTOBJ).second->m_OBJ.m_mapCLK.end() )
			//						pCI = (*itCI).second;
			//				}
			//			}
			//		}
			//		else
			//		{
			//			MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(pHIDE->m_pTITEM->m_dwCLK);

			//			if( itCI != m_OBJ.m_mapCLK.end() )
			//				pCI = (*itCI).second;
			//		}

			//		if(pCI)
			//			pCI->m_dwHideID = vTHIDE.m_bHidePartID;
			//	}
			//	else
			//		ResetDEFOBJPart(&vTHIDE);
			//}
		}

	CTachyonObject::ResetOBJPart(pDevice);
}

void CTExplorerObjBase::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
{
}

void CTExplorerObjBase::ResetDEFEQUIP( CD3DDevice *pDevice,
									   BYTE bEquipPOS)
{
}

THIDEPART CTExplorerObjBase::FindHidePart( CTExplorerItem *pTITEM,
										 BYTE bSlotID)
{
	THIDEPART vRESULT = { 0xFF, 0xFF, 0};

	if( pTITEM && pTITEM->m_pTITEM )
	{
		LPTITEMVISUAL pTVISUAL = pTITEM->GetVisual();
		if( pTVISUAL )
		{
			vRESULT.m_bHideSlotID = pTVISUAL->m_bHideSlot;
			vRESULT.m_bHidePartID = pTVISUAL->m_bHidePart;
			vRESULT.m_bHideRaceID = pTVISUAL->m_bHideRace;
		}
	}

	return vRESULT;
}

BYTE CTExplorerObjBase::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	return pTHIDE && pTHIDE->m_bHideSlotID != 0xFF ? TRUE : FALSE;
}

CString CTExplorerObjBase::GetName()
{
	return CString("");
}

CString CTExplorerObjBase::GetTitle()
{
	return CString("");
}

BYTE CTExplorerObjBase::IsJump()
{
	return m_bAction == TA_JUMP || m_bAction == TA_FLY ? TRUE : FALSE;
}

BYTE CTExplorerObjBase::IsFall()
{
	return m_bAction == TA_FALL ? TRUE : FALSE;
}

void CTExplorerObjBase::SetAction( DWORD dwActionID,
								   DWORD dwAniID)
{
	if( m_dwActionID != dwActionID || m_dwAniID != dwAniID )
	{
		if(m_dwANISNDFuncID)
			m_bSTEPSND = TRUE;

		m_vWorld._41 = m_vPosition._41;
		m_vWorld._42 = m_vPosition._42;
		m_vWorld._43 = m_vPosition._43;

		m_dwActionID = dwActionID;
		m_dwAniID = dwAniID;

		CTachyonObject::SetAction(
			m_dwActionID,
			m_dwAniID);
	}
}

void CTExplorerObjBase::SetSubAction( DWORD dwActionID,
									  DWORD dwAniID)
{
	if(m_bRootID)
		if( m_dwTSUBACT != dwActionID || m_vTSUBACT.m_dwCurANI != dwAniID )
		{
			MAPACTION::iterator itACT = m_OBJ.m_pOBJ->m_mapACTION.find(dwActionID);

			if( itACT != m_OBJ.m_pOBJ->m_mapACTION.end() )
			{
				LPACTION pACT = (*itACT).second;
				MAPANI::iterator itANI = pACT->m_mapANI.find(dwAniID);

				if( m_dwTSUBACT != dwActionID )
				{
					MAPSNDINST::iterator itSND;
					MAPSFXINST::iterator itSFX;

					m_mapTSUBACTSFXINST.clear();
					m_mapTSUBANISFXINST.clear();
					m_mapTSUBACTSNDINST.clear();
					m_mapTSUBANISNDINST.clear();

					CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
					CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
					CTachyonObject::ClearSND(&m_mapTSUBACTSND);
					CTachyonObject::ClearSND(&m_mapTSUBANISND);

					for( itSND = pACT->m_mapSND.begin(); itSND != pACT->m_mapSND.end(); itSND++)
						m_mapTSUBACTSNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

					for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
						m_mapTSUBACTSFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));
				}
				else
				{
					m_mapTSUBANISFXINST.clear();
					m_mapTSUBANISNDINST.clear();

					CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
					CTachyonObject::ClearSND(&m_mapTSUBANISND);
				}

				if( itANI != pACT->m_mapANI.end() )
				{
					LPANI pANI = m_vTSUBACT.m_pANI ? m_vTSUBACT.m_pANI : GetCurANI();

					MAPSNDINST::iterator itSND;
					MAPSFXINST::iterator itSFX;

					m_vTSUBACT.m_pANI = (*itANI).second;
					m_vTSUBACT.m_dwCurANI = dwAniID;
					m_dwTSUBACT = dwActionID;

					for( itSND = m_vTSUBACT.m_pANI->m_mapSND.begin(); itSND != m_vTSUBACT.m_pANI->m_mapSND.end(); itSND++)
						m_mapTSUBANISNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

					for( itSFX = m_vTSUBACT.m_pANI->m_mapSFX.begin(); itSFX != m_vTSUBACT.m_pANI->m_mapSFX.end(); itSFX++)
						m_mapTSUBANISFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));
				}
				else
				{
					m_vTSUBACT.m_dwCurANI = 0;
					m_vTSUBACT.m_pANI = NULL;
					m_dwTSUBACT = dwActionID;
				}

				m_fSubActTime = 0.0f;
				m_nSubKeyBase = -1;
			}
			else
			{
				m_mapTSUBACTSFXINST.clear();
				m_mapTSUBANISFXINST.clear();
				m_mapTSUBACTSNDINST.clear();
				m_mapTSUBANISNDINST.clear();

				CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
				CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
				CTachyonObject::ClearSND(&m_mapTSUBACTSND);
				CTachyonObject::ClearSND(&m_mapTSUBANISND);

				m_vTSUBACT.m_dwCurANI = 0;
				m_vTSUBACT.m_pANI = NULL;
				m_dwTSUBACT = 0;

				m_fSubActTime = 0.0f;
				m_nSubKeyBase = -1;
			}
		}
}

void CTExplorerObjBase::ResetVisible( CTExplorerCAM *pCamera,
									  DWORD dwTick)
{
	m_fCamDIST = D3DXVec3Length(&D3DXVECTOR3(
		m_vPosition._41 - pCamera->m_vTarget.x,
		m_vPosition._42 - pCamera->m_vTarget.y,
		m_vPosition._43 - pCamera->m_vTarget.z));

	m_fZValue = D3DXVec3Length(&D3DXVECTOR3(
		m_vPosition._41 - pCamera->m_vPosition.x,
		m_vPosition._42 - pCamera->m_vPosition.y,
		m_vPosition._43 - pCamera->m_vPosition.z));
	m_bVisible = FALSE;

	if( m_bType != OT_COLLISION && m_bType != OT_PATHWALL )
	{
		FLOAT fDIST = m_fCamDIST - m_fSight;

		if( !m_bFPView && fDIST < m_fRange && !m_bHide )
			m_dwAlphaTick = min( TMAXALPHA_TICK, m_dwAlphaTick + dwTick);
		else
			m_dwAlphaTick -= min( m_dwAlphaTick, dwTick);

		if( fDIST > m_fRange + TALPHA_DIST )
			m_dwAlphaTick = 0;
		m_bAlpha = BYTE(ALPHA_MAX * m_dwAlphaTick / TMAXALPHA_TICK);

		if(!m_bInSHADOW)
			m_dwSHADOWTick = min( TMAXSHADOW_TICK, m_dwSHADOWTick + dwTick);
		else
			m_dwSHADOWTick -= min( m_dwSHADOWTick, dwTick);

		if(m_bAlpha)
		{
			D3DXVECTOR3 vPOS(
				m_vPosition._41,
				m_vPosition._42,
				m_vPosition._43);

			D3DXVECTOR3 vU = pCamera->m_vTarget - pCamera->m_vPosition;
			D3DXVECTOR3 vA = vPOS - pCamera->m_vPosition;

			vU /= D3DXVec3Length(&vU);
			fDIST = D3DXVec3Dot( &vA, &vU);

			if( m_fCamDIST < m_fSight || fDIST + m_fSight > 0.5f )
			{
				vA = pCamera->m_vPosition + fDIST * vU - vPOS;

				FLOAT fLength = D3DXVec3Length(&vA) - m_fSight;
				FLOAT fHeight = fDIST * tanf(pCamera->m_fFOV / 2.0f);
				FLOAT fWidth = fHeight * pCamera->m_fWidth / pCamera->m_fHeight;

				m_bVisible = sqrtf(fHeight * fHeight + fWidth * fWidth) > fLength ? TRUE : FALSE;
			}
		}
	}
}

void CTExplorerObjBase::SetTAction( BYTE bAction)
{
	if( CTChart::GetTACTION( m_bAction )->m_bLevel > CTChart::GetTACTION( bAction )->m_bLevel )
		return;

	m_bAction = bAction;
}

BYTE CTExplorerObjBase::GetTAction()
{
	return TA_STAND;
}

BYTE CTExplorerObjBase::GetEquipMode( BYTE bActionID,
									  BYTE bWeaponID)
{
	LPTACTION pTACTION = CTChart::FindTACTIONTEMP( bActionID, bWeaponID );

	if( pTACTION )
		return pTACTION->m_bEquipMode;

	return MT_NORMAL;
}

LPTACTION CTExplorerObjBase::FindTACTION( BYTE bActionID,
										  BYTE bWeaponID)
{
	LPTACTION pTACTION = CTChart::FindTACTIONTEMP( bActionID, bWeaponID );

	if( pTACTION )
		return pTACTION;

	return NULL;
}

TACTION CTExplorerObjBase::FindActionID( BYTE bActionID,
										 BYTE bWeaponID)
{
	TACTION vResult = { 0, 0, 0, 0};
	
	LPTACTION pTACTION = CTChart::FindTACTIONTEMP( bActionID, bWeaponID );
	if( !pTACTION )
		return vResult;

	MAPACTION::iterator itACT = m_OBJ.m_pOBJ->m_mapACTION.find( pTACTION->m_dwActID);
	if( itACT == m_OBJ.m_pOBJ->m_mapACTION.end() )
		return vResult;

	LPACTION pACT = (*itACT).second;
	if(!pACT)
		return vResult;

	int nCount = INT(pACT->m_mapANI.size());
	if( nCount <= 0 )
		return vResult;

	vResult.m_bEquipMode = pTACTION->m_bEquipMode;
	vResult.m_dwTSLASH = pTACTION->m_dwTSLASH;
	vResult.m_dwActID = pTACTION->m_dwActID;
	nCount = rand() % nCount;

	MAPANI::iterator itANI = pACT->m_mapANI.begin();
	for( int i=0; i<nCount; i++)
		itANI++;
	vResult.m_dwAniID = (*itANI).first;

	return vResult;
}

BYTE CTExplorerObjBase::GetWeaponID( BYTE bMode)
{
	if( bMode == MT_NORMAL )
		return WT_NORMAL;

	CTExplorerInven *pTEQUIP = FindTInven(INVEN_EQUIP);
	if(!pTEQUIP)
		return WT_HAND;

	if(m_bShoot)
	{
		CTExplorerItem *pWEAPON = pTEQUIP->FindTItem(ES_LONGWEAPON);

		if(pWEAPON)
			return pWEAPON->m_pTITEM->m_bWeaponID;
	}
	else
	{
		CTExplorerItem *pRHAND = pTEQUIP->FindTItem(ES_PRMWEAPON);
		CTExplorerItem *pLHAND = pTEQUIP->FindTItem(ES_SNDWEAPON);

		if( pLHAND && pLHAND->m_pTITEM && pLHAND->m_pTITEM->m_bWeaponID == WT_1HAND )
			if( pRHAND && pRHAND->m_pTITEM )
				return rand() % 2 ? WT_1HAND : WT_LHAND;
			else
				return WT_LHAND;
		else if( pRHAND && pRHAND->m_pTITEM )
			return pRHAND->m_pTITEM->m_bWeaponID;
	}

	return WT_HAND;
}

FLOAT CTExplorerObjBase::GetHeight( CTExplorerObjBase *pOBJ,
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

FLOAT CTExplorerObjBase::GetCylinderHeight( CTExplorerObjBase *pOBJ,
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

FLOAT CTExplorerObjBase::GetBoxHeight( CTExplorerObjBase *pOBJ,
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

FLOAT CTExplorerObjBase::GetPolyHeight( CTExplorerObjBase *pOBJ,
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

FLOAT CTExplorerObjBase::GetPolyHeight( LPD3DXVECTOR3 pPOLY,
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
			if( fabs(vPLANE.b) / fLength < CTExplorerMAP::m_fMoveBound )
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

BYTE CTExplorerObjBase::CheckCollision( CTExplorerObjBase *pOBJ)
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

BYTE CTExplorerObjBase::CheckCylinder( CTExplorerObjBase *pOBJ)
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

BYTE CTExplorerObjBase::CheckBox( CTExplorerObjBase *pOBJ)
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

	D3DXQUATERNION vQUAT;
	D3DXMATRIX vMAT;

	CTMath::GetRotationQUAT(
		&vQUAT,
		&pOBJ->m_vPosition);

	D3DXMatrixRotationQuaternion(
		&vMAT,
		&vQUAT);

	vMAT._41 = pOBJ->m_vPosition._41;
	vMAT._42 = pOBJ->m_vPosition._42;
	vMAT._43 = pOBJ->m_vPosition._43;

	for( int i=0; i<6; i++)
		for( int j=0; j<2; j++)
		{
			D3DXPLANE vPLANE;

			for( int k=0; k<3; k++)
			{
				CTMath::Transform(
					&vMAT,
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

BYTE CTExplorerObjBase::CheckPoly( CTExplorerObjBase *pOBJ)
{
	if( m_vPosition._42 + m_fSizeY <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		m_vPosition._42 >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;

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

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
					for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
						for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];
							D3DXPLANE vPLANE;

							for( int l=0; l<3; l++)
							{
								D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
									pOBJ->m_pBone,
									pOBJ->GetMeshMatrix(),
									&pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]]) : pOBJ->m_vPosition;

								vPOLY[l] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

								CTMath::Transform(
									&vTrans,
									&vPOLY[l]);
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

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
					for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
						for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vPOLY[3];
							D3DXPLANE vPLANE;

							for( int l=0; l<3; l++)
							{
								vPOLY[l] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
									pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

								CTMath::Transform(
									&pOBJ->m_vPosition,
									&vPOLY[l]);
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

BYTE CTExplorerObjBase::CheckPoly( LPD3DXPLANE pPLANE,
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

BYTE CTExplorerObjBase::GetCollisionPlane( LPD3DXPLANE pPLANE,
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

	if( fLength == 0.0f || fabs(pPLANE->b) / fLength > CTExplorerMAP::m_fMoveBound )
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

BYTE CTExplorerObjBase::HitTest( LPD3DXVECTOR3 pSTART,
								 LPD3DXVECTOR3 pEND,
								 FLOAT fLineH,
								 FLOAT fLineV)
{
	FLOAT fDIST = 0.0f;

	switch(m_bCollisionType)
	{
	case TCOLLISION_CYLINDER	: return HitCylinder( pSTART, pEND, fLineH, fLineV);
	case TCOLLISION_BOX			: return HitBox( pSTART, pEND, fLineH, fLineV);
	case TCOLLISION_POLY		: return HitPoly( &fDIST, pSTART, pEND, fLineH, fLineV);
	}

	return FALSE;
}

BYTE CTExplorerObjBase::HitCylinder( LPD3DXVECTOR3 pSTART,
									 LPD3DXVECTOR3 pEND,
									 FLOAT fLineH,
									 FLOAT fLineV)
{
	BYTE bResult = FALSE;

	D3DXVECTOR3 vDIR = (*pEND) - (*pSTART);
	D3DXVECTOR3 vPOS(
		m_vPosition._41,
		m_vPosition._42,
		m_vPosition._43);

	POINTWINDING vWINDING;
	D3DXPLANE vPLANE;

	vDIR.y = 0.0f;
	vDIR /= D3DXVec3Length(&vDIR);

	D3DXPlaneFromPointNormal(
		&vPLANE,
		pSTART,
		&vDIR);

	if( D3DXPlaneDotCoord( &vPLANE, &vPOS) < 0.0f )
		return bResult;
	vDIR = -vDIR;

	D3DXPlaneFromPointNormal(
		&vPLANE,
		pEND,
		&vDIR);

	if( D3DXPlaneDotCoord( &vPLANE, &vPOS) < 0.0f )
		return bResult;

	D3DXPlaneFromPoints(
		&vPLANE,
		&D3DXVECTOR3(
		pSTART->x,
		pSTART->y + 1.0f,
		pSTART->z),
		pSTART,
		pEND);

	FLOAT fLength = fabs(D3DXPlaneDotCoord( &vPLANE, &vPOS));
	if( fLength < m_fRadius + fLineH )
	{
		D3DXVECTOR3 vCross;

		vDIR = (*pEND) - (*pSTART);
		D3DXVec3Cross(
			&vCross,
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
			&vDIR);
		vCross += (*pSTART);

		fLength = fLineH + sqrtf(m_fRadius * m_fRadius - fLength * fLength);
		vDIR.y = 0.0f;
		vDIR /= D3DXVec3Length(&vDIR);

		D3DXPlaneFromPoints(
			&vPLANE,
			pSTART,
			pEND,
			&vCross);

		D3DXVECTOR3 vPOINT[4] = {
			vPOS + fLength * vDIR + D3DXVECTOR3( 0.0f, -fLineV - m_fDepth, 0.0f),
			vPOS + fLength * vDIR + D3DXVECTOR3( 0.0f,			 m_fSizeY, 0.0f),
			vPOS - fLength * vDIR + D3DXVECTOR3( 0.0f,			 m_fSizeY, 0.0f),
			vPOS - fLength * vDIR + D3DXVECTOR3( 0.0f, -fLineV - m_fDepth, 0.0f)};

		for( int i=0; i<4; i++)
			vWINDING.m_vPoint.push_back(&vPOINT[i]);

		D3DXPlaneFromPoints(
			&vWINDING.m_vPlane,
			vWINDING.m_vPoint[0],
			vWINDING.m_vPoint[1],
			vWINDING.m_vPoint[2]);

		if(CTMath::IsCut( &vPLANE, &vWINDING))
			bResult = TRUE;

		vWINDING.m_vPoint.clear();
	}

	return bResult;
}

BYTE CTExplorerObjBase::HitBox( LPD3DXVECTOR3 pSTART,
							    LPD3DXVECTOR3 pEND,
								FLOAT fLineH,
								FLOAT fLineV)
{
	static int vINDEX[6][4] = {
		{ 0, 1, 2, 3},
		{ 7, 6, 5, 4},
		{ 0, 4, 5, 1},
		{ 1, 5, 6, 2},
		{ 2, 6, 7, 3},
		{ 3, 7, 4, 0}};

	D3DXVECTOR3 vPOINT[8] = {
		D3DXVECTOR3( -fLineH - m_fSizeX / 2.0f, -fLineV - m_fDepth, -fLineH - m_fSizeZ / 2.0f),
		D3DXVECTOR3(  fLineH + m_fSizeX / 2.0f, -fLineV - m_fDepth, -fLineH - m_fSizeZ / 2.0f),
		D3DXVECTOR3(  fLineH + m_fSizeX / 2.0f, -fLineV - m_fDepth,  fLineH + m_fSizeZ / 2.0f),
		D3DXVECTOR3( -fLineH - m_fSizeX / 2.0f, -fLineV - m_fDepth,  fLineH + m_fSizeZ / 2.0f),
		D3DXVECTOR3( -fLineH - m_fSizeX / 2.0f,			  m_fSizeY, -fLineH - m_fSizeZ / 2.0f),
		D3DXVECTOR3(  fLineH + m_fSizeX / 2.0f,			  m_fSizeY, -fLineH - m_fSizeZ / 2.0f),
		D3DXVECTOR3(  fLineH + m_fSizeX / 2.0f,			  m_fSizeY,  fLineH + m_fSizeZ / 2.0f),
		D3DXVECTOR3( -fLineH - m_fSizeX / 2.0f,			  m_fSizeY,  fLineH + m_fSizeZ / 2.0f)};

	D3DXVECTOR3 vPOS(
		m_vPosition._41,
		m_vPosition._42,
		m_vPosition._43);
	D3DXVECTOR3 vCross;

	D3DXPLANE vPlaneV;
	D3DXPLANE vPlaneH;

	D3DXPlaneFromPoints(
		&vPlaneV,
		&D3DXVECTOR3(
		pSTART->x,
		pSTART->y + 1.0f,
		pSTART->z),
		pSTART,
		pEND);

	D3DXVec3Cross(
		&vCross,
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
		&((*pEND) - (*pSTART)));
	vCross += (*pSTART);

	D3DXPlaneFromPoints(
		&vPlaneH,
		pSTART,
		pEND,
		&vCross);

	D3DXQUATERNION vQUAT;
	D3DXMATRIX vMAT;

	CTMath::GetRotationQUAT(
		&vQUAT,
		&m_vPosition);

	D3DXMatrixRotationQuaternion(
		&vMAT,
		&vQUAT);

	vMAT._41 = vPOS.x;
	vMAT._42 = vPOS.y;
	vMAT._43 = vPOS.z;

	for( int i=0; i<8; i++)
	{
		CTMath::Transform(
			&vMAT,
			&vPOINT[i]);
	}

	for( i=0; i<6; i++)
	{
		POINTWINDING vWINDING;

		for( int j=0; j<4; j++)
			vWINDING.m_vPoint.push_back(&vPOINT[vINDEX[i][j]]);

		D3DXPlaneFromPoints(
			&vWINDING.m_vPlane,
			vWINDING.m_vPoint[0],
			vWINDING.m_vPoint[1],
			vWINDING.m_vPoint[2]);

		if( D3DXPlaneDotCoord( &vWINDING.m_vPlane, pSTART) * D3DXPlaneDotCoord( &vWINDING.m_vPlane, pEND) < 0.0f &&
			CTMath::IsCut( &vPlaneV, &vWINDING) && CTMath::IsCut( &vPlaneH, &vWINDING) )
		{
			vWINDING.m_vPoint.clear();
			return TRUE;
		}

		vWINDING.m_vPoint.clear();
	}

	return FALSE;
}

BYTE CTExplorerObjBase::HitPoly( FLOAT *pDIST,
								 LPD3DXVECTOR3 pSTART,
								 LPD3DXVECTOR3 pEND,
								 FLOAT fLineH,
								 FLOAT fLineV)
{
	D3DXVECTOR3 vDIR = (*pEND) - (*pSTART);
	FLOAT fLength = D3DXVec3Length(&vDIR);

	LPANI pANI = GetCurANI();
	BYTE bResult = FALSE;
	MAPCLKINST::iterator it;

	if( fLength == 0.0f )
		return bResult;
	vDIR /= fLength;

	D3DXVECTOR3 vRECT[4] = {
		D3DXVECTOR3( -fLineV / 2.0f, 0.0f,  fLineH / 2.0f),
		D3DXVECTOR3( -fLineV / 2.0f, 0.0f, -fLineH / 2.0f),
		D3DXVECTOR3(  fLineV / 2.0f, 0.0f, -fLineH / 2.0f),
		D3DXVECTOR3(  fLineV / 2.0f, 0.0f,  fLineH / 2.0f)};

	D3DXPLANE vBOUND[4];
	D3DXMATRIX vBASIS;

	CTMath::GetBasisMatrix(
		&vBASIS,
		pSTART,
		&vDIR);

	if( fLineV != 0.0f && fLineH != 0.0f )
		for( int i=0; i<4; i++)
		{
			D3DXPlaneFromPoints(
				&vBOUND[i],
				&vRECT[i],
				&D3DXVECTOR3(
				vRECT[i].x,
				vRECT[i].y + 1.0f,
				vRECT[i].z),
				&vRECT[(i + 1) % 4]);
		}

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			D3DXVECTOR3 vCENTER = pCI->m_pMESH->m_pMESH->m_vCenterPoint;
			FLOAT fSCALE = max( max(
				m_vScale._11,
				m_vScale._22),
				m_vScale._33);

			CTMath::Transform(
				&m_vPosition,
				&vCENTER);

			if(D3DXSphereBoundProbe( &vCENTER, fSCALE * pCI->m_pMESH->m_pMESH->m_fRadius, pSTART, &vDIR))
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
								FLOAT fLocal = 0.0f;

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

									CTMath::Transform(
										&vBASIS,
										&vPOLY[l]);
								}

								if( CTMath::HitPoly(
									vPOLY,
									fLineV != 0.0f && fLineH != 0.0f ? vRECT : NULL,
									fLineV != 0.0f && fLineH != 0.0f ? vBOUND : NULL,
									&fLocal) && fLocal < fLength && (
									!bResult || fLocal < (*pDIST) ))
								{
									(*pDIST) = fLocal;
									bResult = TRUE;
								}
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
								FLOAT fLocal = 0.0f;

								for( int l=0; l<3; l++)
								{
									vPOLY[l] = D3DXVECTOR3(
										pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
										pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
										pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

									CTMath::Transform(
										&m_vPosition,
										&vPOLY[l]);

									CTMath::Transform(
										&vBASIS,
										&vPOLY[l]);
								}

								if( CTMath::HitPoly(
									vPOLY,
									fLineV != 0.0f && fLineH != 0.0f ? vRECT : NULL,
									fLineV != 0.0f && fLineH != 0.0f ? vBOUND : NULL,
									&fLocal) && fLocal < fLength && (
									!bResult || fLocal < (*pDIST) ))
								{
									(*pDIST) = fLocal;
									bResult = TRUE;
								}
							}
				}
			}
		}
	}

	return bResult;
}

WORD CTExplorerObjBase::GetTargetDIR( FLOAT fTargetX, FLOAT fTargetZ)
{
	return 1800;
}

void CTExplorerObjBase::CalcDIR( WORD wMoveDIR, WORD wDIR)
{
}

void CTExplorerObjBase::CalcTick( LPDIRECT3DDEVICE9 pDevice,
								  DWORD dwTick)
{
	CTachyonObject::CalcTick(
		pDevice,
		m_wMoveDIR != MOVE_NONE ? DWORD(m_fSpeedFactor * FLOAT(dwTick)) : dwTick);

	m_fSquareHeight = m_vPosition._42;
	if(m_bRootID)
		CalcSUBTick( pDevice, dwTick);
}

void CTExplorerObjBase::CalcSUBTick( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	LPANI pANI = m_vTSUBACT.m_pANI;

	if( pANI && pANI->m_pANI )
	{
		FLOAT fTotal = pANI->m_pANI->GetTotalTime();
		m_fSubActTime += CTMath::GetTimeValue(dwTick);

		if( m_fSubActTime >= fTotal )
		{
			FLOAT fTime = m_fSubActTime;

			if(m_pHost)
			{
				OBJMSG msg;

				if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
					msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

				msg.m_dwMSG = OM_TSUBACTENDING;
				msg.m_pOBJ = (DWORD_PTR) this;

				if(m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg))
				{
					m_fSubActTime = fTotal;
					return;
				}
			}

			while( fTime >= fTotal )
			{
				m_fSubActTime = fTotal;

				CalcSUBFrame();
				m_nSubKeyBase = -1;

				fTime -= fTotal;
			}
			m_fSubActTime = fTime;

			if(m_pHost)
			{
				OBJMSG msg;

				if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
					msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

				msg.m_dwMSG = OM_TSUBACTENDED;
				msg.m_pOBJ = (DWORD_PTR) this;

				if(m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg))
					return;
			}

			CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
			CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
			CTachyonObject::ClearSND(&m_mapTSUBACTSND);
			CTachyonObject::ClearSND(&m_mapTSUBANISND);
		}
		CalcSUBFrame();

		GenerateSFX( &m_mapTSUBACTSFXINST, &m_vTSUBACTSFX, &m_mapTSUBACTSFX, m_fSubActTime, m_dwACTSFXFuncID);
		GenerateSFX( &m_mapTSUBANISFXINST, &m_vTSUBANISFX, &m_mapTSUBANISFX, m_fSubActTime, m_dwANISFXFuncID);

		GenerateSND( &m_mapTSUBACTSNDINST, &m_mapTSUBACTSND, m_fSubActTime, m_dwACTSNDFuncID);
		GenerateSND( &m_mapTSUBANISNDINST, &m_mapTSUBANISND, m_fSubActTime, m_dwACTSNDFuncID);
	}
}

void CTExplorerObjBase::CalcSUBFrame()
{
	LPANI pANI = m_vTSUBACT.m_pANI;

	if( pANI && pANI->m_pANI )
	{
		pANI->m_pANI->m_fCurTime = m_fSubActTime;
		pANI->m_pANI->GetAniKey();

		if(!pANI->m_pANI->m_vKey.empty())
			while( m_nSubKeyBase < pANI->m_pANI->m_nKeyBase )
			{
				m_nSubKeyBase++;

				if( m_pHost && pANI->m_pANI->m_vKey[m_nSubKeyBase]->m_dwEventID > 0 )
				{
					OBJMSG msg;

					if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
						msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

					msg.m_dwMSG = pANI->m_pANI->m_vKey[m_nSubKeyBase]->m_dwEventID;
					msg.m_pOBJ = (DWORD_PTR) this;

					m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);
				}
			}
	}
}

void CTExplorerObjBase::ApplyMatrix( CD3DDevice *pDevice)
{
	if( m_bRootID && m_vTSUBACT.m_pANI )
	{
		static FLOAT vWORLD[4 * 3 * MAX_PIVOT + 4];
		LPANI pANI = GetCurANI();

		if( pANI && pANI->m_pANI )
		{
			LPANI pTSUBANI = m_vTSUBACT.m_pANI;

			pTSUBANI->m_pANI->m_fCurTime = m_fSubActTime;
			pANI->m_pANI->m_fCurTime = m_fActTime;

			LPANIDATA pTSUBDATA = pTSUBANI->m_pANI->GetAniData();
			LPANIDATA pDATA = pANI->m_pANI->GetAniData();

			if( pTSUBDATA && pTSUBDATA->m_pAni && pDATA && pDATA->m_pAni )
			{
				FLOAT fBlendTime = FLOAT(m_dwBlendTick) / FLOAT(m_dwBlend);
				BYTE bBlend = m_dwBlendTick < m_dwBlend ? TRUE : FALSE;

				LPD3DXMATRIX pInit = GetMeshMatrix();

				pDATA->m_pAni->GetFrameMatrix(
					m_pBone,
					bBlend ? m_pBlend : NULL,
					bBlend ? m_pBlendKEY : NULL,
					m_pPivot,
					m_vPosition, 0,
					pANI->m_pANI->m_fLocalTime,
					fBlendTime);

				pTSUBDATA->m_pAni->GetFrameMatrix(
					m_pBone,
					NULL,
					NULL,
					m_pPivot,
					m_pBone[m_bRootID],
					m_bRootID,
					pTSUBANI->m_pANI->m_fLocalTime,
					0.0f);

				if(m_bUseSHADER)
				{
					D3DXMatrixTranspose( (LPD3DXMATRIX) vWORLD, m_pBone);
					for( int i=0; i<pDATA->m_pAni->GetNodeCount(); i++)
						D3DXMatrixTranspose( (LPD3DXMATRIX) &vWORLD[12 * (i + 1)], &(pInit ? pInit[i] * m_pBone[i + 1] : m_pBone[i + 1]));

					pDevice->m_pDevice->SetVertexShaderConstantF(
						pDevice->m_vConstantVS[VC_WORLD],
						vWORLD,
						3 * (pDATA->m_pAni->GetNodeCount() + 1));
				}
				else
				{
					pDevice->m_pDevice->SetTransform( D3DTS_WORLDMATRIX(0), &m_pBone[0]);

					for( int i=0; i<pDATA->m_pAni->GetNodeCount(); i++)
						pDevice->m_pDevice->SetTransform( D3DTS_WORLDMATRIX(i + 1), pInit ? &(pInit[i] * m_pBone[i + 1]) : &m_pBone[i + 1]);
				}
			}
			else if(m_bUseSHADER)
			{
				D3DXMatrixTranspose( (LPD3DXMATRIX) vWORLD, &m_vPosition);
				pDevice->m_pDevice->SetVertexShaderConstantF(
					pDevice->m_vConstantVS[VC_WORLD],
					vWORLD, 3);
			}
			else
				pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &m_vPosition);
		}
		else if(m_bUseSHADER)
		{
			D3DXMatrixTranspose( (LPD3DXMATRIX) vWORLD, &m_vPosition);
			pDevice->m_pDevice->SetVertexShaderConstantF(
				pDevice->m_vConstantVS[VC_WORLD],
				vWORLD, 3);
		}
		else
			pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &m_vPosition);
	}
	else
		CTachyonObject::ApplyMatrix(pDevice);
}

void CTExplorerObjBase::InitSquareResource( CD3DDevice *pDevice,
										    CTachyonRes *pRES)
{
	static PVERTEX vPOINT[4] = {
		{ -0.5f, 0.0f, -0.5f, 0.0f, 0.0f},
		{ -0.5f, 0.0f,  0.5f, 0.0f, 1.0f},
		{  0.5f, 0.0f, -0.5f, 1.0f, 0.0f},
		{  0.5f, 0.0f,  0.5f, 1.0f, 1.0f}};
	LPVOID pDATA = NULL;

	ReleaseSquareResource();
	pDevice->m_pDevice->CreateVertexBuffer(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&m_pTSQUARE, NULL);

	m_pTSQUARE->Lock( 0, 0, &pDATA, 0);
	memcpy( pDATA, vPOINT, 4 * sizeof(PVERTEX));
	m_pTSQUARE->Unlock();

	MAPRES::iterator finder = pRES->m_mapTEX.find(ID_TEX_SHADOW);
	if( finder != pRES->m_mapTEX.end() )
		m_pTSHADOW = (LPTEXTURESET) (*finder).second;
}

void CTExplorerObjBase::ReleaseSquareResource()
{
	if(m_pTSQUARE)
	{
		m_pTSQUARE->Release();
		m_pTSQUARE = NULL;
	}

	m_pTSHADOW = NULL;
}

void CTExplorerObjBase::BeginDrawSquare( CD3DDevice *pDevice)
{
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	pDevice->m_pDevice->SetVertexShader(NULL);
	pDevice->m_pDevice->SetPixelShader(NULL);

	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
}

void CTExplorerObjBase::Render( CD3DDevice *pDevice,
							    CD3DCamera *pCamera)
{
	CTachyonObject::Render(
		pDevice,
		pCamera);

	CalcSFX( pDevice, &m_vTGARBAGESFX);
	CalcSFX( pDevice, &m_vTACTIONSFX);
}

void CTExplorerObjBase::RemoveTActionSFX( LPVECTOROBJSFX pTACTIONSFX)
{
	for( int i=0; i<INT(pTACTIONSFX->size()); i++)
	{
		CTachyonSFX::Release((*pTACTIONSFX)[i]->m_dwID);
		delete (*pTACTIONSFX)[i];
	}

	pTACTIONSFX->clear();
}

void CTExplorerObjBase::ClearTActionSFX( LPVECTOROBJSFX pTACTIONSFX)
{
	for( int i=0; i<INT(pTACTIONSFX->size()); i++)
	{
		CTachyonSFX *pTSFX = CTachyonSFX::GetSFX((*pTACTIONSFX)[i]->m_dwID);

		if( pTSFX && pTSFX->m_pSFX )
		{
			m_vTGARBAGESFX.push_back((*pTACTIONSFX)[i]);
			pTSFX->m_bDeleted = TRUE;
		}
		else
		{
			CTachyonSFX::Release((*pTACTIONSFX)[i]->m_dwID);
			delete (*pTACTIONSFX)[i];
		}
	}

	pTACTIONSFX->clear();
}

void CTExplorerObjBase::PlayTActionSFX( LPVECTOROBJSFX pTACTIONSFX,
									    DWORD dwFuncID)
{
	LPTSFXTEMP pTEMP = CTChart::FindTSFXTEMP( dwFuncID );

	if(!pTEMP)
		return;

	CTExplorerSFX *pTSFX = NULL;
	LPOBJSFX pItem = NULL;

	if(pTEMP->m_dwPosID)
	{
		MAPSFXINST::iterator it;

		for( it = m_mapOBJSFXINST.begin(); it != m_mapOBJSFXINST.end(); it++)
			if( (*it).second->m_dwFuncID == pTEMP->m_dwPosID )
			{
				pTSFX = new CTExplorerSFX();
				pItem = new OBJSFX();

				pItem->m_pSFX = new SFXINST();
				pItem->m_bDeleteINST = TRUE;

				pItem->m_pSFX->m_bDeleteOnStop = (*it).second->m_bDeleteOnStop;
				pItem->m_pSFX->m_nPivotID = (*it).second->m_nPivotID;
				pItem->m_pSFX->m_dwTick = (*it).second->m_dwTick;
				pItem->m_pSFX->m_bBias = (*it).second->m_bBias;
				pItem->m_pSFX->m_vPOS = (*it).second->m_vPOS;
				pItem->m_pSFX->m_vROT = (*it).second->m_vROT;

				pItem->m_pSFX->m_pSFX = pTEMP->m_pTSFX;
				pItem->m_pSFX->m_dwFuncID = dwFuncID;
			}
	}
	else
	{
		pTSFX = new CTExplorerSFX();
		pItem = new OBJSFX();

		pItem->m_pSFX = new SFXINST();
		pItem->m_bDeleteINST = TRUE;

		pItem->m_pSFX->m_bDeleteOnStop = FALSE;
		pItem->m_pSFX->m_dwFuncID = dwFuncID;
		pItem->m_pSFX->m_nPivotID = 0;
		pItem->m_pSFX->m_dwTick = 0;
		pItem->m_pSFX->m_bBias = 0;

		pItem->m_pSFX->m_vPOS = D3DXVECTOR3( 0, 0, 0);
		pItem->m_pSFX->m_vROT = D3DXVECTOR3( 0, 0, 0);
		pItem->m_pSFX->m_pSFX = pTEMP->m_pTSFX;
	}

	if(pTSFX)
	{
		pTSFX->m_bHasHost = TRUE;
		pTSFX->m_pHost = m_pHost;
		pTSFX->m_dwInitTick = 0;

		pItem->m_dwID = CTachyonSFX::RegSFX(
			pTSFX,
			pItem->m_pSFX->m_bBias);

		pTACTIONSFX->push_back(pItem);
	}
}

void CTExplorerObjBase::CancelTAction()
{
}

void CTExplorerObjBase::RemoveTAction()
{
}

void CTExplorerObjBase::ResetRootID( DWORD dwRootID)
{
	MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwRootID);

	if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
		m_bRootID = BYTE((*itID).second + 1);
	else
		m_bRootID = 0;
}

BYTE CTExplorerObjBase::CanUseSubANI()
{
	return m_bRootID ? TRUE : FALSE;
}

BYTE CTExplorerObjBase::CheckDIR( WORD wDIR)
{
	int nDIR = INT(wDIR) - INT(m_wDIR);

	while(nDIR < 0)
		nDIR += 1800;
	nDIR %= 1800;

	if( nDIR > 900 )
		nDIR = 1800 - nDIR;

	return nDIR > FOLLOW_RANGE ? TRUE : FALSE;
}

void CTExplorerObjBase::ClearInven()
{
	MAPTINVEN::iterator itINVEN;

	for( itINVEN = m_mapTINVEN.begin(); itINVEN != m_mapTINVEN.end(); itINVEN++)
		delete (*itINVEN).second;

	m_mapTINVEN.clear();
}

CTExplorerInven *CTExplorerObjBase::FindTInven( BYTE bInvenID)
{
	MAPTINVEN::iterator finder = m_mapTINVEN.find(bInvenID);

	if( finder != m_mapTINVEN.end() )
		return (*finder).second;

	return NULL;
}
