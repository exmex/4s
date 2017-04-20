#include "stdafx.h"
#include "TExplorer.h"

CTExplorerNpc::CTExplorerNpc()
{
	m_strTITLE.Empty();
	m_strNAME.Empty();
	m_pTNPC = NULL;

	m_bDrawShadow = TRUE;
	m_bDrawName = TRUE;

	m_bType = OT_NPC;
}

CTExplorerNpc::~CTExplorerNpc()
{
}

void CTExplorerNpc::Release()
{
	CTExplorerObjBase::Release();
	m_strTITLE.Empty();
	m_strNAME.Empty();
	m_pTNPC = NULL;
}

void CTExplorerNpc::ReleaseData()
{
	CTExplorerObjBase::ReleaseData();
	m_strTITLE.Empty();
	m_strNAME.Empty();

	m_pTNPC = NULL;
	m_bLand = TRUE;
}

CString CTExplorerNpc::GetTitle()
{
	CString strResult = m_strTITLE;
	strResult.Remove('_');

	return strResult;
}

CString CTExplorerNpc::GetName()
{
	CString strResult = m_strNAME;
	strResult.Remove('_');

	return strResult;
}

void CTExplorerNpc::InitTNPC( CD3DDevice *pDevice,
							  CTachyonRes *pRES,
							  LPTNPC pTNPC)
{
	MAPRES::iterator itOBJ = pRES->m_mapOBJ.find(m_pTNPC->m_dwOBJ);
	CTExplorerInven *pTEQUIP = new CTExplorerInven();
	ClearInven();

	m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_EQUIP, pTEQUIP));
	InitOBJ((LPOBJECT) (*itOBJ).second);

	for( BYTE i=0; i<ES_COUNT; i++)
		if(m_pTNPC->m_pTITEM[i])
		{
			CTExplorerItem *pTITEM = new CTExplorerItem();

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
}
