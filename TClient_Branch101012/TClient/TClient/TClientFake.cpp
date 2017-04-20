#include "StdAfx.h"
#include "TClientGame.h"


CTClientFake::CTClientFake()
{
	for( BYTE i=0; i<ES_COUNT; i++)
		m_vTEMP.m_pTITEM[i] = NULL;

	for( i=0; i<TMONSKILL_COUNT; i++)
		m_vTEMP.m_wTSKILL[i] = 0;

	m_vTEMP.m_strTITLE.Empty();
	m_vTEMP.m_strNAME.Empty();
	m_vTEMP.m_pTPOPUP = NULL;

	m_vTEMP.m_fLOST = 0.0f;
	m_vTEMP.m_fLB = 0.0f;
	m_vTEMP.m_fAB = 0.0f;

	m_vTEMP.m_dwOBJ = 0;
	m_vTEMP.m_wKind = 0;

	m_vTEMP.m_bCanSelected = TRUE;
	m_vTEMP.m_bCanFly = FALSE;
	m_vTEMP.m_bCanAttack = TRUE;

	m_vTROAM = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pTEMP = &m_vTEMP;
	m_pHOST = NULL;
}

CTClientFake::~CTClientFake()
{
}

void CTClientFake::ReleaseData()
{
	CTClientRecall::ReleaseData();

	for( BYTE i=0; i<ES_COUNT; i++)
		m_vTEMP.m_pTITEM[i] = NULL;

	for( i=0; i<TMONSKILL_COUNT; i++)
		m_vTEMP.m_wTSKILL[i] = 0;

	m_vTEMP.m_strTITLE.Empty();
	m_vTEMP.m_strNAME.Empty();
	m_vTEMP.m_pTPOPUP = NULL;

	m_vTEMP.m_fLOST = 0.0f;
	m_vTEMP.m_fLB = 0.0f;
	m_vTEMP.m_fAB = 0.0f;

	m_vTEMP.m_dwOBJ = 0;
	m_vTEMP.m_wKind = 0;

	m_vTEMP.m_bCanSelected = TRUE;
	m_vTEMP.m_bCanFly = FALSE;
	m_vTEMP.m_bCanAttack = TRUE;

	m_vTROAM = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pTEMP = &m_vTEMP;
	m_pHOST = NULL;

	m_bRaceID = 0;
	m_bSex = 0;

	m_bHair = 0;
	m_bFace = 0;
	m_bBody = 0;
	m_bPants = 0;
	m_bHand = 0;
	m_bFoot = 0;
}

BYTE CTClientFake::GetDrawName()
{
	return CTClientObjBase::GetDrawName() && CTClientGame::GetPcNAMEOption() ? TRUE : FALSE;
}

void CTClientFake::ResetDEFEQUIP( CD3DDevice *pDevice,
								  BYTE bEquipPOS)
{
	DWORD dwCLK = 0;
	DWORD dwCL = 0;
	DWORD dwMESH = 0;

	switch(bEquipPOS)
	{
	case ES_FACE	:
		{
			dwCLK = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCLK;
			dwCL = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCL;
			dwMESH = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwMESH;
		}

		break;

	case ES_BODY	:
		{
			dwCLK = CTChart::m_vTBODY[m_bBody].m_dwCLK;
			dwCL = CTChart::m_vTBODY[m_bBody].m_dwCL;
			dwMESH = CTChart::m_vTBODY[m_bBody].m_dwMESH;
		}

		break;

	case ES_PANTS	:
		{
			dwCLK = CTChart::m_vTPANTS[m_bPants].m_dwCLK;
			dwCL = CTChart::m_vTPANTS[m_bPants].m_dwCL;
			dwMESH = CTChart::m_vTPANTS[m_bPants].m_dwMESH;
		}

		break;

	case ES_FOOT	:
		{
			dwCLK = CTChart::m_vTFOOT[m_bFoot].m_dwCLK;
			dwCL = CTChart::m_vTFOOT[m_bFoot].m_dwCL;
			dwMESH = CTChart::m_vTFOOT[m_bFoot].m_dwMESH;
		}

		break;

	case ES_HAND	:
		{
			dwCLK = CTChart::m_vTHAND[m_bHand].m_dwCLK;
			dwCL = CTChart::m_vTHAND[m_bHand].m_dwCL;
			dwMESH = CTChart::m_vTHAND[m_bHand].m_dwMESH;
		}

		break;
	}

	if(dwCLK)
		SetCloth( pDevice, dwCLK, dwCL, dwMESH);
}

THIDEPART CTClientFake::FindHidePart( CTClientItem *pTITEM,
									  BYTE bSlotID)
{
	if(pTITEM)
		return CTClientRecall::FindHidePart( pTITEM, bSlotID);

	THIDEPART vRESULT = { 0xFF, 0xFF, 0};
	switch(bSlotID)
	{
	case ES_FACE	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTFACE[m_bFace][m_bHair].m_bHideRaceID;
		}

		break;

	case ES_BODY	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTBODY[m_bBody].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTBODY[m_bBody].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTBODY[m_bBody].m_bHideRaceID;
		}

		break;

	case ES_PANTS	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTPANTS[m_bPants].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTPANTS[m_bPants].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTPANTS[m_bPants].m_bHideRaceID;
		}

		break;

	case ES_FOOT	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTFOOT[m_bFoot].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTFOOT[m_bFoot].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTFOOT[m_bFoot].m_bHideRaceID;
		}

		break;

	case ES_HAND	:
		{
			vRESULT.m_bHideSlotID = CTChart::m_vTHAND[m_bHand].m_bHideSlotID;
			vRESULT.m_bHidePartID = CTChart::m_vTHAND[m_bHand].m_bHidePartID;
			vRESULT.m_bHideRaceID = CTChart::m_vTHAND[m_bHand].m_bHideRaceID;
		}

		break;
	}

	return vRESULT;
}

BYTE CTClientFake::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	static BYTE bRaceID[TMAINRACE_COUNT][TSEX_COUNT] = {
		{  1,  2},
		{  4,  8},
		{ 16, 32}};

	return CTClientRecall::CheckOBJPart(pTHIDE) && (bRaceID[m_bRaceID][m_bSex] & pTHIDE->m_bHideRaceID) ? TRUE : FALSE;
}

void CTClientFake::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
{
	DWORD dwCLK = 0;

	switch(pTHIDE->m_bHideSlotID)
	{
	case ES_FACE	: dwCLK = CTChart::m_vTFACE[m_bFace][m_bHair].m_dwCLK; break;
	case ES_BODY	: dwCLK = CTChart::m_vTBODY[m_bBody].m_dwCLK; break;
	case ES_PANTS	: dwCLK = CTChart::m_vTPANTS[m_bPants].m_dwCLK; break;
	case ES_FOOT	: dwCLK = CTChart::m_vTFOOT[m_bFoot].m_dwCLK; break;
	case ES_HAND	: dwCLK = CTChart::m_vTHAND[m_bHand].m_dwCLK; break;
	}

	MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(dwCLK);
	if( itCI != m_OBJ.m_mapCLK.end() )
		(*itCI).second->m_dwHideID = pTHIDE->m_bHidePartID;
}

void CTClientFake::InitRecall( CD3DDevice *pDevice,
							   CTachyonRes *pRES,
							   WORD wTempID,
							   BYTE bLevel)
{
	static WORD wBasicSkill[][2] = {
		{ 31, FALSE},
		{ 32, TRUE},
		{ 33, TRUE},
		{ 34, TRUE}};

	CTClientInven *pTHOSTEQUIP = m_pHOST->FindTInven(INVEN_EQUIP);
	CTClientInven *pTEQUIP = new CTClientInven();
	ClearInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

	m_bRaceID = m_pHOST->GetRaceID();
	m_bSex = m_pHOST->m_bSex;

	m_bHair = m_pHOST->m_bHair;
	m_bFace = m_pHOST->m_bFace;
	m_bBody = m_pHOST->m_bBody;
	m_bPants = m_pHOST->m_bPants;
	m_bHand = m_pHOST->m_bHand;
	m_bFoot = m_pHOST->m_bFoot;

	InitOBJ(m_pHOST->m_OBJ.m_pOBJ);
	for( BYTE i=0; i<ES_COUNT; i++)
	{	
		CTClientItem *pTHOSTITEM = pTHOSTEQUIP->FindTItem(i);

		if( pTHOSTITEM && pTHOSTITEM->GetTITEM() )
		{
			CTClientItem *pTITEM = new CTClientItem();
			(*pTITEM) = (*pTHOSTITEM);

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			m_vTEMP.m_pTITEM[i] = pTITEM->GetTITEM();
		}
	}

	m_fBreathHeight = m_pHOST->m_fBreathHeight;
	m_bCanSelected = m_pHOST->m_bCanSelected;
	ResetEQUIP( pDevice, pRES);

	InitSIZE(
		m_pHOST->m_fSizeX,
		m_pHOST->m_fSizeY,
		m_pHOST->m_fSizeZ,
		0.0f, 0.0f);

	D3DXMatrixScaling(
		&m_vScaleRACE,
		CTChart::m_vTRACESCALE[m_bRaceID][m_bSex],
		CTChart::m_vTRACESCALE[m_bRaceID][m_bSex],
		CTChart::m_vTRACESCALE[m_bRaceID][m_bSex]);

	m_vPosition = 
	m_vWorld = m_pHOST->GetPositionMat();

	for( i=0; i<4; i++)
	{
		CTClientSkill *pTHOSTSKILL = m_pHOST->FindTSkill(wBasicSkill[i][0]);

		if( pTHOSTSKILL && pTHOSTSKILL->m_pTSKILL )
		{
			if( m_vTEMP.m_fLOST < pTHOSTSKILL->m_pTSKILL->m_fMaxRange )
				m_vTEMP.m_fLOST = pTHOSTSKILL->m_pTSKILL->m_fMaxRange;

			if( m_vTEMP.m_fLB < pTHOSTSKILL->m_pTSKILL->m_fMaxRange )
				m_vTEMP.m_fLB = pTHOSTSKILL->m_pTSKILL->m_fMaxRange;

			if( !wBasicSkill[i][1] || pTEQUIP->FindTItem(ES_LONGWEAPON) )
			{
				if(m_vTEMP.m_wTSKILL[TMONSKILL_DEFAULT])
				{
					MAPTSKILL::iterator finder = m_mapTSKILL.find(m_vTEMP.m_wTSKILL[TMONSKILL_DEFAULT]);

					if( finder != m_mapTSKILL.end() )
					{
						delete (*finder).second;
						m_mapTSKILL.erase(finder);
					}
				}

				if( m_vTEMP.m_fAB < pTHOSTSKILL->m_pTSKILL->m_fMaxRange )
					m_vTEMP.m_fAB = pTHOSTSKILL->m_pTSKILL->m_fMaxRange;

				m_vTEMP.m_wTSKILL[TMONSKILL_DEFAULT] = wBasicSkill[i][0];
				CTClientSkill *pTSKILL = new CTClientSkill();

				pTSKILL->m_pTSKILL = pTHOSTSKILL->m_pTSKILL;
				pTSKILL->m_dwTick = 0;
				pTSKILL->m_bLevel = pTHOSTSKILL->m_bLevel;

				m_mapTSKILL.insert( MAPTSKILL::value_type( wBasicSkill[i][0], pTSKILL));
			}
		}
	}

	m_pTCURSKILL = FindTSkill(m_vTEMP.m_wTSKILL[TMONSKILL_DEFAULT]);
	m_bAI = TRECALLAI_ACTIVE;

	m_vTSKILLDATA.m_bAtkCountryID = m_bContryID;
	m_vTSKILLDATA.m_bAglow = m_bCanSelected;

	m_vTEMP.m_strTITLE = m_pHOST->GetTitle();
	m_vTEMP.m_strNAME = m_pHOST->GetName();

	m_vTROAM = D3DXVECTOR3(
		GetPositionX(),
		GetPositionY(),
		GetPositionZ());
	m_bLevel = m_pHOST->m_bLevel;
}

D3DXVECTOR3 CTClientFake::GetRoamTarget( LPD3DXMATRIX pDIR,
										 FLOAT fPosX,
										 FLOAT fPosY,
										 FLOAT fPosZ)
{
	return m_vTROAM;
}

void CTClientFake::DoRETRANS( CD3DDevice *pDevice,
							  CTachyonRes *pRES)
{
	CTClientMoveObj::DoRETRANS( pDevice, pRES);

	InitOBJ(pRES->GetOBJ( CTChart::m_vTRACE[m_bRaceID][m_bSex]));
	ResetEQUIP( pDevice, pRES);

	m_fBreathHeight = GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_bCanSelected = TRUE;

	InitSIZE(
		GetAttrFLOAT(ID_SIZE_X),
		GetAttrFLOAT(ID_SIZE_Y),
		GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	ResetRootID(ID_PIVOT_WAIST);
}
