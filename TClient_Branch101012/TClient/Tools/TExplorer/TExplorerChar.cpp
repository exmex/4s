#include "stdafx.h"
#include "TExplorer.h"


CTExplorerChar::CTExplorerChar()
{
	m_bCollisionType = TCOLLISION_CYLINDER;
	m_bType = OT_PC;
}

CTExplorerChar::~CTExplorerChar()
{
}

void CTExplorerChar::ReleaseData()
{
	CTExplorerMoveObj::ReleaseData();
	m_strNAME.Empty();

	m_bRaceID = 0;
	m_bSex = 0;

	m_bHair = 0;
	m_bFace = 0;
	m_bBody = 0;
	m_bPants = 0;
	m_bHand = 0;
	m_bFoot = 0;
}

CString CTExplorerChar::GetName()
{
	return m_strNAME;
}

void CTExplorerChar::ResetDEFEQUIP( CD3DDevice *pDevice,
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

THIDEPART CTExplorerChar::FindHidePart( CTExplorerItem *pTITEM,
									    BYTE bSlotID)
{
	if(pTITEM)
		CTExplorerMoveObj::FindHidePart( pTITEM, bSlotID);

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

BYTE CTExplorerChar::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	static BYTE bRaceID[TMAINRACE_COUNT][TSEX_COUNT] = {
		{  1,  2},
		{  4,  8},
		{ 16, 32}};

	return CTExplorerMoveObj::CheckOBJPart(pTHIDE) && (bRaceID[m_bRaceID][m_bSex] & pTHIDE->m_bHideRaceID) ? TRUE : FALSE;
}

void CTExplorerChar::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
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
