#include "stdafx.h"
#include "TMapViewer.h"

MAPNPCTEMP CTMapViewerNpc::m_mapTNPCTEMP;


CTMapViewerNpc::CTMapViewerNpc()
{
	m_strTITLE.Empty();
	m_strNAME.Empty();
	m_pTNPC = NULL;

	m_bDrawShadow = TRUE;
	m_bDrawName = TRUE;

	m_bType = OT_NPC;
}

CTMapViewerNpc::~CTMapViewerNpc()
{
}

LPTNPCTEMP CTMapViewerNpc::FindTNPC( WORD wTempID)
{
	MAPNPCTEMP::iterator finder = m_mapTNPCTEMP.find(wTempID);

	if( finder == m_mapTNPCTEMP.end() )
		return NULL;

	return (*finder).second;
}

void CTMapViewerNpc::Release()
{
	CTMapViewerObjBase::Release();
	m_strTITLE.Empty();
	m_strNAME.Empty();
	m_pTNPC = NULL;
}

void CTMapViewerNpc::ReleaseData()
{
	CTMapViewerObjBase::ReleaseData();
	m_strTITLE.Empty();
	m_strNAME.Empty();

	m_pTNPC = NULL;
	m_bLand = TRUE;
}

CString CTMapViewerNpc::GetTitle()
{
	CString strResult = m_strTITLE;
	strResult.Remove('_');

	return strResult;
}

CString CTMapViewerNpc::GetName()
{
	CString strResult = m_strNAME;
	strResult.Remove('_');

	return strResult;
}

void CTMapViewerNpc::InitTNPC( CD3DDevice *pDevice, CTachyonRes *pRES)
{
	MAPRES::iterator itOBJ = pRES->m_mapOBJ.find(m_pTNPC->m_dwOBJ);
	CTMapViewerInven *pTEQUIP = new CTMapViewerInven();
	ClearInven();

	m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, pTEQUIP));
	InitOBJ((LPOBJECT) (*itOBJ).second);

	for( BYTE i=0; i<ES_COUNT; i++)
		if(m_pTNPC->m_pTITEM[i])
		{
			CTMapViewerItem *pTITEM = new CTMapViewerItem();

			pTITEM->m_wItemID = m_pTNPC->m_pTITEM[i]->m_wItemID;
			pTITEM->m_bItemID = i;
			pTITEM->m_pTITEM = m_pTNPC->m_pTITEM[i];

			pTEQUIP->m_mapTITEM.insert( MAPTITEM::value_type( pTITEM->m_bItemID, pTITEM));
		}
	TACTION vActionID = FindActionID(
		m_pTNPC->m_bActionID,
		GetWeaponID(m_bMode));

	m_bEquipMode = vActionID.m_bEquipMode;
	ResetEQUIP( pDevice, pRES);

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
}
