#include "stdafx.h"
#include "TClientGame.h"
#include "TClientWnd.h"


CTClientNpc::CTClientNpc()
{
	m_strTITLE.Empty();
	m_strNAME.Empty();

	m_pTPOPUP = NULL;
	m_pTNPC = NULL;

	m_bCanSelected = TRUE;
	m_bDynamicOBJ = TRUE;
	m_bDrawName = TRUE;
	m_bRandACT = FALSE;

	m_dwQuestID = 0;
	m_dwExecID = 0;
	m_wItemID = 0;

	m_bLogic = TTGATE_LOGIC_NONE;
	m_bType = OT_NPC;

	m_dwLeftRndActTick = 0;
	m_bCamp = 0;
}

CTClientNpc::~CTClientNpc()
{
}

void CTClientNpc::CalcTick( CD3DDevice* pDevice,
						    CTachyonRes *pRES,
							DWORD dwTick)
{
	if( m_bNPCType == TNPC_BOX )
		ResetBOX();
	CalcRandomAction( pDevice, pRES, dwTick);

	if( pRES && m_pTNPC && m_bRandACT &&
		m_bACTLevel <= CTChart::GetTACTION(m_bAction)->m_bLevel )
	{
		TACTION vActionID = FindActionID(
			m_bAction,
			GetWeaponID(m_bMode));
		BYTE bHideOnCapeMode = CTChart::GetTACTION(m_bAction)->m_bHideOnCapeMode;

		if( vActionID.m_bEquipMode != m_bEquipMode ||
			m_bHideOnCapeMode != bHideOnCapeMode )
		{
			m_bEquipMode = vActionID.m_bEquipMode;
			m_bHideOnCapeMode = bHideOnCapeMode;

			ResetEQUIP(
				pDevice,
				pRES);
		}

		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
		m_bRandACT = FALSE;
	}

	CTClientObjBase::CalcTick(
		pDevice->m_pDevice,
		dwTick);
}

void CTClientNpc::SetLogicAction( DWORD dwActionID)
{
	if(!m_OBJ.m_pOBJ)
		return;

	MAPACTION::iterator itACT = m_OBJ.m_pOBJ->m_mapACTION.find(dwActionID);
	if( itACT == m_OBJ.m_pOBJ->m_mapACTION.end() )
		return;

	LPACTION pTACT = (*itACT).second;
	if(!pTACT)
		return;

	int nCount = INT(pTACT->m_mapANI.size());
	if( nCount <= 0 )
		return;
	nCount = rand() % nCount;

	MAPANI::iterator itANI = pTACT->m_mapANI.begin();
	for( int i=0; i<nCount; i++)
		itANI++;
	DWORD dwAniID = (*itANI).first;

	SetAction( dwActionID, dwAniID);
}

void CTClientNpc::OpenBOX( BYTE bOPEN)
{
	if( m_bNPCType != TNPC_BOX || !m_pTNPC->m_bBoxAnim ) 
		return;

	BYTE bLoopACT = bOPEN ? TA_SITDOWN : TA_STAND;
	BYTE bAction = bOPEN ? TA_SIT : TA_STANDUP;

	TACTION vActionID = FindActionID(
		bAction,
		WT_NORMAL);

	m_bACTLevel = CTChart::GetTACTION( bAction )->m_bLevel;
	m_bAction = bLoopACT;

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
}

void CTClientNpc::ResetBOX()
{
	if( m_bNPCType != TNPC_BOX || !m_pTNPC->m_bBoxAnim || m_bACTLevel > CTChart::GetTACTION( m_bAction )->m_bLevel ) 
		return;

	TACTION vActionID = FindActionID(
		m_bAction,
		WT_NORMAL);

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
}

BYTE CTClientNpc::CanRandACT()
{
	return m_pTNPC && (m_pTNPC->m_bRndActID[0] != (BYTE)T_INVALID ||
		m_pTNPC->m_bRndActID[1] != (BYTE)T_INVALID) ? TRUE : FALSE;
}

void CTClientNpc::CalcRandomAction( CD3DDevice *pDevice,
								    CTachyonRes *pRES,
									DWORD dwTick)
{
	if( !CanRandACT() || m_bRandACT )
		return;

	if( m_dwLeftRndActTick > dwTick )
	{
		m_dwLeftRndActTick -= dwTick;
		return;
	}
	SetLeftRndActTick();

	if( rand() % 5 == 0 )
		return;

	BYTE bIndex = rand() % TNPC_RNDACT_COUNT;
	BYTE bAction = m_pTNPC->m_bRndActID[bIndex];

	if( bAction == (BYTE)T_INVALID )
		bAction = m_pTNPC->m_bRndActID[(bIndex+1)%TNPC_RNDACT_COUNT];
	BYTE bWeaponID = GetWeaponID(m_bMode);

	TACTION vActionID = FindActionID(
		bAction,
		bWeaponID);
	BYTE bHideOnCapeMode = CTChart::GetTACTION(bAction)->m_bHideOnCapeMode;

	if( vActionID.m_bEquipMode != m_bEquipMode ||
		m_bHideOnCapeMode != bHideOnCapeMode )
	{
		m_bEquipMode = vActionID.m_bEquipMode;
		m_bHideOnCapeMode = bHideOnCapeMode;

		ResetEQUIP(
			pDevice,
			pRES);
	}

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel + 1;
	m_bRandACT = TRUE;
}

void CTClientNpc::SetLeftRndActTick()
{
	m_dwLeftRndActTick = 4000 + (rand()%3000);
}

void CTClientNpc::Release()
{
	CTClientObjBase::Release();

	m_strTITLE.Empty();
	m_strNAME.Empty();

	m_bOriginalContryID = TCONTRY_N;
	m_pTNPC = NULL;
}

void CTClientNpc::ReleaseData()
{
	CTClientObjBase::ReleaseData();

	m_strTITLE.Empty();
	m_strNAME.Empty();

	m_pTPOPUP = NULL;
	m_pTNPC = NULL;

	m_dwQuestID = 0;
	m_dwExecID = 0;
	m_wItemID = 0;

	m_bOriginalContryID = TCONTRY_N;
	m_bLand = TRUE;
}

CString CTClientNpc::GetTitle()
{
	CString strResult = m_strTITLE;
	strResult.Remove('_');

	return strResult;
}

CString CTClientNpc::GetName()
{
	CString strResult = m_strNAME;
	strResult.Remove('_');

	return strResult;
}

void CTClientNpc::InitTNPC( CD3DDevice *pDevice,
						    CTachyonRes *pRES,
							LPTNPC pTNPC)
{
	MAPRES::iterator itOBJ = pRES->m_mapOBJ.find(m_pTNPC->m_dwOBJ);
	
#ifdef TEST_MODE
	if( itOBJ == pRES->m_mapOBJ.end() )
	{
		FILE* fp = fopen("npc_error.txt", "at");
		if( fp )
		{
			fseek(fp, 0, SEEK_END);
			fprintf(fp,"NPC Object ID - 0x%.8X를 찾을 수 없습니다.\n", m_pTNPC->m_dwOBJ);
			fclose(fp);
		}		
		return;
	}
#endif

	CTClientInven *pTEQUIP = new CTClientInven();
	ClearInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));

	InitOBJ((LPOBJECT) (*itOBJ).second);
	
	for( BYTE i=0; i<ES_COUNT; i++)
		if(m_pTNPC->m_pTITEM[i])
		{
			CTClientItem *pTITEM = new CTClientItem();

			pTITEM->SetItemSlot(i);
			pTITEM->SetTITEM(m_pTNPC->m_pTITEM[i]);
			pTITEM->SetCount(1);

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}
	SetLeftRndActTick();

	TACTION vActionID = FindActionID(
		m_pTNPC->m_bActionID,
		GetWeaponID(m_bMode));
	LPTACTIONDATA pTACTDATA = CTChart::GetTACTION(m_pTNPC->m_bActionID);

	m_bHideOnCapeMode = pTACTDATA->m_bHideOnCapeMode;
	m_bEquipMode = vActionID.m_bEquipMode;

	m_bAction = m_pTNPC->m_bActionID;
	m_bACTLevel = pTACTDATA->m_bLevel;
	m_bRandACT = FALSE;

	ResetEQUIP( pDevice, pRES);
	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

	if( m_bNPCType == TNPC_BOX || CanRandACT() )
		m_pHost = AfxGetMainWnd();

	InitSIZE(
		pTNPC->m_fScaleX * pTNPC->m_fSizeX,
		pTNPC->m_fScaleY * pTNPC->m_fSizeY,
		pTNPC->m_fScaleZ * pTNPC->m_fSizeZ,
		0.0f, 0.0f);

	D3DXMatrixRotationY(
		&m_vWorld,
		pTNPC->m_fDIR);

	m_vWorld._41 = pTNPC->m_fPosX;
	m_vWorld._42 = pTNPC->m_fPosY;
	m_vWorld._43 = pTNPC->m_fPosZ;

	D3DXMatrixScaling(
		&m_vScale,
		pTNPC->m_fScaleX,
		pTNPC->m_fScaleY,
		pTNPC->m_fScaleZ);
	m_vPosition = m_vScale * m_vWorld;

	switch(m_bNPCType)
	{
	case TNPC_CRAFT		:
		{
			LPOBJECT pOBJ = pRES->GetOBJ(ID_MARK_BOX);

			if( pOBJ != m_vTMARKER.m_OBJ.m_pOBJ )
			{
				m_vTMARKER.InitOBJ(pOBJ);
				m_vTMARKER.m_bSFX = TRUE;
			}
		}

		break;
	}
}

BYTE CTClientNpc::GetDrawName()
{
	return CTClientObjBase::GetDrawName() && CTClientGame::GetNpcNAMEOption() ? TRUE : FALSE;
}

BYTE CTClientNpc::GetDrawNodeMark( LPTTERMPOS pTNODEPATH,
								   LPTNPCPORTAL pTNODEPORTAL)
{
	if( (pTNODEPORTAL && pTNODEPORTAL->m_wNpcID && pTNODEPORTAL->m_wNpcID == m_dwID) ||
		(pTNODEPATH && pTNODEPATH->m_dwNpcID && pTNODEPATH->m_dwNpcID == m_dwID) )
		return TRUE;

	return CTClientObjBase::GetDrawNodeMark( pTNODEPATH, pTNODEPORTAL);
}

BYTE CTClientNpc::CheckRequired()
{
	return CheckRequiredQuest() && CheckRequiredItem() ? TRUE : FALSE;
}

BYTE CTClientNpc::CheckRequiredQuest()
{
	if(!m_dwQuestID)
		return TRUE;

	CTClientWnd *pMainWnd = (CTClientWnd *) AfxGetMainWnd();
	CTClientChar *pMainChar = pMainWnd ? pMainWnd->m_MainGame.GetMainChar() : NULL;

	return pMainChar && pMainChar->FindTQuest(m_dwQuestID) ? TRUE : FALSE;
}

BYTE CTClientNpc::CheckRequiredItem()
{
	if(!m_wItemID)
		return TRUE;

	CTClientWnd *pMainWnd = (CTClientWnd *) AfxGetMainWnd();
	CTClientChar *pMainChar = pMainWnd ? pMainWnd->m_MainGame.GetMainChar() : NULL;

	if(pMainChar)
	{
		MAPTINVEN::iterator itINVEN;

		for( itINVEN = pMainChar->m_mapTINVEN.begin(); itINVEN != pMainChar->m_mapTINVEN.end(); itINVEN++)
		{
			CTClientInven *pTINVEN = (*itINVEN).second;

			if(pTINVEN)
			{
				MAPTITEM::iterator itTITEM;

				for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
					if( (*itTITEM).second && (*itTITEM).second->GetItemID() == m_wItemID )
						return TRUE;
			}
		}
	}

	return FALSE;
}

LRESULT CTClientNpc::OnActEndMsg()
{
	switch(m_bLogic)
	{
	case TTGATE_LOGIC_ENTER	:
		{
			m_vWorld._41 = m_vPosition._41;
			m_vWorld._42 = m_vPosition._42;
			m_vWorld._43 = m_vPosition._43;

			m_dwActionID = ID_STAND_NORMAL;
			m_bLogic = TTGATE_LOGIC_NONE;

			SetLogicAction(m_dwActionID);
		}

		break;

	case TTGATE_LOGIC_END	:
		{
			m_vWorld._41 = m_vPosition._41;
			m_vWorld._42 = m_vPosition._42;
			m_vWorld._43 = m_vPosition._43;

			m_dwActionID = ID_DIE_NORMAL;
			m_bLogic = TTGATE_LOGIC_NONE;

			SetLogicAction(m_dwActionID);
		}

		break;
	}

	return CTClientObjBase::OnActEndMsg();
}
