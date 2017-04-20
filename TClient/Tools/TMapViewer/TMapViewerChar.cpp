#include "stdafx.h"
#include "TMapViewer.h"


CTMapViewerChar::CTMapViewerChar()
{
	m_bCollisionType = TCOLLISION_CYLINDER;
	m_bType = OT_PC;
}

CTMapViewerChar::~CTMapViewerChar()
{
}

void CTMapViewerChar::ReleaseData()
{
	CTMapViewerMoveObj::ReleaseData();
	m_strNAME.Empty();

	m_bRaceID = 0;
	m_bSex = 1;

	m_bHair = 0;
	m_bFace = 0;
	m_bBody = 0;
	m_bPants = 0;
	m_bHand = 0;
	m_bFoot = 0;
}

CString CTMapViewerChar::GetName()
{
	return m_strNAME;
}

void CTMapViewerChar::ResetDEFEQUIP( CD3DDevice *pDevice,
								    BYTE bEquipPOS)
{
	DWORD dwCLK = 0;
	DWORD dwCL = 0;
	DWORD dwMESH = 0;

	switch(bEquipPOS)
	{
	case ES_FACE	:
		{
			dwCLK = m_vTFACE[m_bFace][m_bHair].m_dwCLK;
			dwCL = m_vTFACE[m_bFace][m_bHair].m_dwCL;
			dwMESH = m_vTFACE[m_bFace][m_bHair].m_dwMESH;
		}

		break;

	case ES_BODY	:
		{
			dwCLK = m_vTBODY[m_bBody].m_dwCLK;
			dwCL = m_vTBODY[m_bBody].m_dwCL;
			dwMESH = m_vTBODY[m_bBody].m_dwMESH;
		}

		break;

	case ES_PANTS	:
		{
			dwCLK = m_vTPANTS[m_bPants].m_dwCLK;
			dwCL = m_vTPANTS[m_bPants].m_dwCL;
			dwMESH = m_vTPANTS[m_bPants].m_dwMESH;
		}

		break;

	case ES_FOOT	:
		{
			dwCLK = m_vTFOOT[m_bFoot].m_dwCLK;
			dwCL = m_vTFOOT[m_bFoot].m_dwCL;
			dwMESH = m_vTFOOT[m_bFoot].m_dwMESH;
		}

		break;

	case ES_HAND	:
		{
			dwCLK = m_vTHAND[m_bHand].m_dwCLK;
			dwCL = m_vTHAND[m_bHand].m_dwCL;
			dwMESH = m_vTHAND[m_bHand].m_dwMESH;
		}

		break;
	}

	if(dwCLK)
		SetCloth( pDevice, dwCLK, dwCL, dwMESH);
}

THIDEPART CTMapViewerChar::FindHidePart( CTMapViewerItem *pTITEM,
									    BYTE bSlotID)
{
	if(pTITEM)
		CTMapViewerMoveObj::FindHidePart( pTITEM, bSlotID);

	THIDEPART vRESULT = { 0xFF, 0xFF, 0};
	switch(bSlotID)
	{
	case ES_FACE	:
		{
			vRESULT.m_bHideSlotID = m_vTFACE[m_bFace][m_bHair].m_bHideSlotID;
			vRESULT.m_bHidePartID = m_vTFACE[m_bFace][m_bHair].m_bHidePartID;
			vRESULT.m_bHideRaceID = m_vTFACE[m_bFace][m_bHair].m_bHideRaceID;
		}

		break;

	case ES_BODY	:
		{
			vRESULT.m_bHideSlotID = m_vTBODY[m_bBody].m_bHideSlotID;
			vRESULT.m_bHidePartID = m_vTBODY[m_bBody].m_bHidePartID;
			vRESULT.m_bHideRaceID = m_vTBODY[m_bBody].m_bHideRaceID;
		}

		break;

	case ES_PANTS	:
		{
			vRESULT.m_bHideSlotID = m_vTPANTS[m_bPants].m_bHideSlotID;
			vRESULT.m_bHidePartID = m_vTPANTS[m_bPants].m_bHidePartID;
			vRESULT.m_bHideRaceID = m_vTPANTS[m_bPants].m_bHideRaceID;
		}

		break;

	case ES_FOOT	:
		{
			vRESULT.m_bHideSlotID = m_vTFOOT[m_bFoot].m_bHideSlotID;
			vRESULT.m_bHidePartID = m_vTFOOT[m_bFoot].m_bHidePartID;
			vRESULT.m_bHideRaceID = m_vTFOOT[m_bFoot].m_bHideRaceID;
		}

		break;

	case ES_HAND	:
		{
			vRESULT.m_bHideSlotID = m_vTHAND[m_bHand].m_bHideSlotID;
			vRESULT.m_bHidePartID = m_vTHAND[m_bHand].m_bHidePartID;
			vRESULT.m_bHideRaceID = m_vTHAND[m_bHand].m_bHideRaceID;
		}

		break;
	}

	return vRESULT;
}

BYTE CTMapViewerChar::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	static BYTE bRaceID[TMAINRACE_COUNT][TSEX_COUNT] = {
		{  1,  2},
		{  4,  8},
		{ 16, 32}};

	return CTMapViewerMoveObj::CheckOBJPart(pTHIDE) && (bRaceID[m_bRaceID][m_bSex] & pTHIDE->m_bHideRaceID) ? TRUE : FALSE;
}

void CTMapViewerChar::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
{
	DWORD dwCLK = 0;

	switch(pTHIDE->m_bHideSlotID)
	{
	case ES_FACE	: dwCLK = m_vTFACE[m_bFace][m_bHair].m_dwCLK; break;
	case ES_BODY	: dwCLK = m_vTBODY[m_bBody].m_dwCLK; break;
	case ES_PANTS	: dwCLK = m_vTPANTS[m_bPants].m_dwCLK; break;
	case ES_FOOT	: dwCLK = m_vTFOOT[m_bFoot].m_dwCLK; break;
	case ES_HAND	: dwCLK = m_vTHAND[m_bHand].m_dwCLK; break;
	}

	MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(dwCLK);
	if( itCI != m_OBJ.m_mapCLK.end() )
		(*itCI).second->m_dwHideID = pTHIDE->m_bHidePartID;
}
