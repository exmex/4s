#include "stdafx.h"

CTachyonMedia *CTachyonObject::m_pMedia = NULL;

FLOAT CTachyonObject::m_fMipFactor = DEF_MIPFACTOR;
DWORD CTachyonObject::m_dwBlend = DEF_BLENDTICK;

void tagOBJINST::Release()
{
	MAPATTRIBUTE::iterator itATTR;
	MAPOBJPART::iterator itDRAW;
	MAPCLKINST::iterator itCLK;
	MAPACTINST::iterator itACT;

	for( itDRAW = m_mapDRAW.begin(); itDRAW != m_mapDRAW.end(); itDRAW++)
	{
		VECTOROBJPART *pDRAW = (*itDRAW).second;

		while(!pDRAW->empty())
		{
			delete pDRAW->back();
			pDRAW->pop_back();
		}

		delete pDRAW;
	}

	for( itATTR = m_mapATTR.begin(); itATTR != m_mapATTR.end(); itATTR++)
		delete (*itATTR).second;

	for( itCLK = m_mapCLK.begin(); itCLK != m_mapCLK.end(); itCLK++)
		delete (*itCLK).second;

	for( itACT = m_mapACT.begin(); itACT != m_mapACT.end(); itACT++)
		delete (*itACT).second;

	m_mapATTR.clear();
	m_mapDRAW.clear();
	m_mapCLK.clear();
	m_mapACT.clear();

	m_dwCurCLK = 0;
	m_dwCurACT = 0;
	m_pOBJ = NULL;
};


CTachyonObject::CTachyonObject()
:	m_pBlendKEY(NULL),
	m_pPivot(NULL),
	m_pBlend(NULL),
	m_pBone(NULL),
	m_bUseSHADER(FALSE),
	m_bZEnable(TRUE),
	m_bZWrite(TRUE),
	m_bSFX(TRUE),
	m_bLOD(TRUE),
	m_bAlpha(0xFF),
	m_bSelAlpha(0xFF),
	m_fActTime(0.0f),
	m_dwTexTick(0),
	m_nKeyBase(-1),
	m_pHost(NULL),
	m_dwOBJSFXFuncID(0),
	m_dwACTSFXFuncID(0),
	m_dwANISFXFuncID(0),
	m_dwOBJSNDFuncID(0),
	m_dwACTSNDFuncID(0),
	m_dwANISNDFuncID(0),
	m_bBlend(TRUE)
{
	D3DXMatrixIdentity(&m_vPosition);
	D3DXMatrixIdentity(&m_vWorld);
	D3DXMatrixIdentity(&m_vScale);
	D3DXMatrixIdentity(&m_vPreScale);

	m_dwBlendTick = m_dwBlend;

	m_mapOBJSNDINST.clear();
	m_mapACTSNDINST.clear();
	m_mapANISNDINST.clear();
	m_mapOBJSND.clear();
	m_mapACTSND.clear();
	m_mapANISND.clear();
	m_mapEQUIP.clear();
	m_mapCOLOR.clear();

	m_mapOBJSFXINST.clear();
	m_mapACTSFXINST.clear();
	m_mapANISFXINST.clear();
	m_mapOBJSFX.clear();
	m_mapACTSFX.clear();
	m_mapANISFX.clear();
	m_vOBJSFX.clear();
	m_vACTSFX.clear();
	m_vANISFX.clear();
}

CTachyonObject::~CTachyonObject()
{
	if( m_OBJ.m_pOBJ && m_OBJ.m_pOBJ->m_pRES )
		m_OBJ.m_pOBJ->m_pRES->ReleaseOBJ(m_OBJ.m_pOBJ);

	m_mapCOLOR.clear();
	ClearMatrix();
	ClearEquip();
	ClearSFX();
	ClearSND();
	m_OBJ.Release();
}

void CTachyonObject::Release()
{
	if( m_OBJ.m_pOBJ && m_OBJ.m_pOBJ->m_pRES )
		m_OBJ.m_pOBJ->m_pRES->ReleaseOBJ(m_OBJ.m_pOBJ);

	m_mapCOLOR.clear();
	ClearMatrix();
	ClearEquip();
	ClearSFX();
	ClearSND();
	m_OBJ.Release();
}

void CTachyonObject::ClearMatrix()
{
	if(m_pBlendKEY)
	{
		delete[] m_pBlendKEY;
		m_pBlendKEY = NULL;
	}

	if(m_pPivot)
	{
		delete[] m_pPivot;
		m_pPivot = NULL;
	}

	if(m_pBlend)
	{
		delete[] m_pBlend;
		m_pBlend = NULL;
	}

	if(m_pBone)
	{
		delete[] m_pBone;
		m_pBone = NULL;
	}
}

void CTachyonObject::ClearEquip()
{
	MAPOBJECT::iterator itOBJ;

	for( itOBJ = m_mapEQUIP.begin(); itOBJ != m_mapEQUIP.end(); itOBJ++)
		delete (*itOBJ).second;

	m_mapEQUIP.clear();
}

void CTachyonObject::ClearOBJSND()
{
	ClearSND(&m_mapOBJSND);
	ClearSND(&m_mapACTSND);
	ClearSND(&m_mapANISND);
}

void CTachyonObject::ClearOBJSFX()
{
	ClearSFX( &m_vOBJSFX, &m_mapOBJSFX);
	ClearSFX( &m_vACTSFX, &m_mapACTSFX);
	ClearSFX( &m_vANISFX, &m_mapANISFX);
}

void CTachyonObject::RemoveOBJSFX()
{
	RemoveSFX( &m_vOBJSFX, &m_mapOBJSFX);
	RemoveSFX( &m_vACTSFX, &m_mapACTSFX);
	RemoveSFX( &m_vANISFX, &m_mapANISFX);
}

void CTachyonObject::CalcOBJPIVOT( CD3DDevice *pDevice)
{
	MAPOBJECT::iterator itOBJ;
	LPANI pANI = GetCurANI();

	if( pANI && pANI->m_pANI )
	{
		pANI->m_pANI->m_fCurTime = m_fActTime;
		LPANIDATA pDATA = pANI->m_pANI->GetAniData();

		if( pDATA && pDATA->m_pAni )
		{
			for( int i=0; i<pDATA->m_pAni->GetNodeCount(); i++)
				m_pBone[i + 1] = pDATA->m_pAni->m_pBones[i] * m_vPosition;
			m_pBone[0] = m_vPosition;
		}
	}

	for( itOBJ = m_mapEQUIP.begin(); itOBJ != m_mapEQUIP.end(); itOBJ++)
	{
		CTachyonObject *pEQUIP = (*itOBJ).second;

		if(pEQUIP)
		{
			D3DXMATRIX vPosition = pEQUIP->m_vPosition;

			pEQUIP->m_vPosition *= m_pBone[(*itOBJ).first];
			pEQUIP->m_bAlpha = m_bAlpha;
			pEQUIP->m_bLOD = m_bLOD;

			pEQUIP->CalcOBJPIVOT(pDevice);
			pEQUIP->m_vPosition = vPosition;
		}
	}
}

void CTachyonObject::CalcOBJSFX( CD3DDevice *pDevice)
{
	CalcSFX( pDevice, &m_vOBJSFX);
	CalcSFX( pDevice, &m_vACTSFX);
	CalcSFX( pDevice, &m_vANISFX);
}

void CTachyonObject::ClearACTSFX()
{
	ClearSFX( &m_vACTSFX, &m_mapACTSFX);
	ClearSFX( &m_vANISFX, &m_mapANISFX);
}

void CTachyonObject::ClearSND()
{
	m_mapOBJSNDINST.clear();
	m_mapACTSNDINST.clear();
	m_mapANISNDINST.clear();

	ClearSND(&m_mapOBJSND);
	ClearSND(&m_mapACTSND);
	ClearSND(&m_mapANISND);
}

void CTachyonObject::ClearSFX()
{
	m_mapOBJSFXINST.clear();
	m_mapACTSFXINST.clear();
	m_mapANISFXINST.clear();

	RemoveSFX( &m_vOBJSFX, &m_mapOBJSFX);
	RemoveSFX( &m_vACTSFX, &m_mapACTSFX);
	RemoveSFX( &m_vANISFX, &m_mapANISFX);
}

void CTachyonObject::ClearSND( LPMAPOBJSND pSND)
{
	MAPOBJSND::iterator it;

	for( it = pSND->begin(); it != pSND->end(); it++)
	{
		if((*it).second->m_pWAV)
		{
			if((*it).second->m_pSND->m_bDeleteOnStop)
				(*it).second->m_pWAV->Stop((*it).second->m_nIndex);

			(*it).second->m_pWAV->Unlock((*it).second->m_nIndex);
		}

		delete (*it).second;
	}

	pSND->clear();
}

void CTachyonObject::ResetSND( LPMAPOBJSND pSND, BYTE bCheckStop)
{
	MAPOBJSND::iterator it;

	for( it = pSND->begin(); it != pSND->end(); it++)
		if( !bCheckStop || ((*it).second->m_pWAV && !(*it).second->m_pWAV->IsPlay((*it).second->m_nIndex)) )
			(*it).second->m_bReset = TRUE;
}

void CTachyonObject::ClearSFX( LPVECTOROBJSFX pSFX, LPMAPDWORD pINDEX)
{
	VECTOROBJSFX::iterator it = pSFX->begin();

	while(it != pSFX->end())
	{
		LPOBJSFX pItem = (*it);

		if(!pItem->m_pSFX->m_bDeleteOnStop)
		{
			CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);
			it++;

			if(pTSFX)
				pTSFX->m_bDeleted = TRUE;
		}
		else
		{
			CTachyonSFX::Release(pItem->m_dwID);
			delete pItem;

			pSFX->erase(it);
		}
	}

	if(pINDEX)
		pINDEX->clear();
}

void CTachyonObject::RemoveSFX( LPVECTOROBJSFX pSFX, LPMAPDWORD pINDEX)
{
	while(!pSFX->empty())
	{
		LPOBJSFX pItem = pSFX->back();

		if(!pItem->m_pSFX->m_bDeleteOnStop)
		{
			CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);

			if(pTSFX)
			{
				pTSFX->m_bHasHost = FALSE;
				pTSFX->m_bDeleted = TRUE;
			}
		}
		else
			CTachyonSFX::Release(pItem->m_dwID);

		pSFX->pop_back();
		delete pItem;
	}

	if(pINDEX)
		pINDEX->clear();
}

void CTachyonObject::RemoveSFX( LPVECTOROBJSFX pSFX, LPMAPDWORD pINDEX, DWORD dwFuncID )
{
	VECTOROBJSFX::iterator itSFX = pSFX->begin();
	
	while( itSFX != pSFX->end() )
	{
		LPOBJSFX pItem = (*itSFX);

		if( pItem->m_pSFX && pItem->m_pSFX->m_dwFuncID == dwFuncID )
		{
			if(!pItem->m_pSFX->m_bDeleteOnStop)
			{
				CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);

				if(pTSFX)
				{
					pTSFX->m_bHasHost = FALSE;
					pTSFX->m_bDeleted = TRUE;
				}
			}
			else
				CTachyonSFX::Release(pItem->m_dwID);

			DWORD dwSFX = pItem->m_pSFX->m_dwSFX;
			MAPDWORD::iterator itIDX = pINDEX->find( dwSFX );
			if( itIDX != pINDEX->end() )
				pINDEX->erase( itIDX );

			delete pItem;
			itSFX = pSFX->erase( itSFX );
			continue;
		}

		++itSFX;
	}
}

void CTachyonObject::InitOBJ( LPOBJECT pOBJ)
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
			pOBJ->m_pRES->LockOBJ(pOBJ);

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
			m_pPivot = new TPIVOT[pOBJ->m_bPivotCount];
			m_pBlend = new D3DXMATRIX[pOBJ->m_bPivotCount];
			m_pBone = new D3DXMATRIX[pOBJ->m_bPivotCount];

			for( BYTE i=0; i<pOBJ->m_bPivotCount; i++)
			{
				D3DXQuaternionIdentity(&m_pBlendKEY[i]);

				D3DXQuaternionIdentity(&m_pPivot[i].m_vRot);
				m_pPivot[i].m_vScale.x = 1.0f;
				m_pPivot[i].m_vScale.y = 1.0f;
				m_pPivot[i].m_vScale.z = 1.0f;

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

void CTachyonObject::InitOBJ( CD3DDevice *pDevice, LPMAPOBJ pOBJ)
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

void CTachyonObject::BeginBlend( LPANISET pPREV,
								 LPANISET pNEXT,
								 FLOAT fPrevActTime,
								 FLOAT fNextActTime)
{
	static D3DXMATRIX vNEXT[MAX_PIVOT];

	D3DXMATRIX vIDENTITY(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	pPREV->m_fCurTime = fPrevActTime;
	pNEXT->m_fCurTime = fNextActTime;

	LPANIDATA pPrevDATA = pPREV->GetAniData();
	LPANIDATA pNextDATA = pNEXT->GetAniData();

	if( !pPrevDATA || !pPrevDATA->m_pAni ||
		!pNextDATA || !pNextDATA->m_pAni )
		return;

	pPrevDATA->m_pAni->GetFrameMatrix(
		m_pBlend,
		NULL,
		NULL,
		NULL,
		vIDENTITY, 0,
		pPREV->m_fLocalTime,
		0.0f);

	pNextDATA->m_pAni->GetFrameMatrix(
		vNEXT,
		NULL,
		NULL,
		NULL,
		vIDENTITY, 0,
		pNEXT->m_fLocalTime,
		0.0f);

	pNextDATA->m_pAni->GetBlendKEY(
		m_pBlendKEY,
		m_pBlend,
		vNEXT);
}

void CTachyonObject::SetAction( DWORD dwAction,
							    DWORD dwAni)
{
	MAPACTION::iterator itACT = m_OBJ.m_pOBJ->m_mapACTION.find(dwAction);

	if( itACT != m_OBJ.m_pOBJ->m_mapACTION.end() )
	{
		MAPACTINST::iterator itAI = m_OBJ.m_mapACT.find(dwAction);
		BOOL bReset = FALSE;

		LPACTION pACT = (*itACT).second;
		LPACTINST pAI = NULL;

		if( itAI == m_OBJ.m_mapACT.end() )
		{
			pAI = new ACTINST();
			m_OBJ.m_mapACT.insert( MAPACTINST::value_type( dwAction, pAI));
		}
		else
			pAI = (*itAI).second;

		if( m_OBJ.m_dwCurACT != dwAction )
		{
			MAPSNDINST::iterator itSND;
			MAPSFXINST::iterator itSFX;

			m_mapACTSFXINST.clear();
			m_mapANISFXINST.clear();
			m_mapACTSNDINST.clear();
			m_mapANISNDINST.clear();

			ClearSFX( &m_vACTSFX, &m_mapACTSFX);
			ClearSFX( &m_vANISFX, &m_mapANISFX);
			ClearSND(&m_mapACTSND);
			ClearSND(&m_mapANISND);
			bReset = TRUE;

			for( itSND = pACT->m_mapSND.begin(); itSND != pACT->m_mapSND.end(); itSND++)
				m_mapACTSNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

			for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
				m_mapACTSFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));

			{
				VECTOROBJSFX::iterator itOBJSFX, endOBJSFX;
				itOBJSFX = m_vOBJSFX.begin();
				endOBJSFX = m_vOBJSFX.end();
				for(; itOBJSFX != endOBJSFX ; ++itOBJSFX )
				{
					if( (*itOBJSFX)->m_pSFX && !(*itOBJSFX)->m_pSFX->m_mapHideByAction.empty())
					{
						MAPDWORD::iterator itEnd = (*itOBJSFX)->m_pSFX->m_mapHideByAction.end();
						MAPDWORD::iterator itBeforeAction = (*itOBJSFX)->m_pSFX->m_mapHideByAction.find( m_OBJ.m_dwCurACT );
						MAPDWORD::iterator itAfterAction = (*itOBJSFX)->m_pSFX->m_mapHideByAction.find( dwAction );
						
						if( itBeforeAction == itEnd &&
							itAfterAction != itEnd )
						{
							CTachyonSFX* pTSFX = CTachyonSFX::GetSFX( (*itOBJSFX)->m_dwID );
							pTSFX->m_bShow = FALSE;
						}
						else if( itBeforeAction != itEnd &&
							itAfterAction == itEnd )
						{
							CTachyonSFX* pTSFX = CTachyonSFX::GetSFX( (*itOBJSFX)->m_dwID );
							pTSFX->m_bShow = TRUE;
						}
					}
				}
			}
		}
		else if( pAI->m_dwCurANI != dwAni )
		{
			m_mapANISFXINST.clear();
			m_mapANISNDINST.clear();

			ClearSFX( &m_vANISFX, &m_mapANISFX);
			ClearSND(&m_mapANISND);
			bReset = TRUE;
		}

		if(bReset)
		{
			MAPANI::iterator itANI = pACT->m_mapANI.find(dwAni);

			if( itANI != pACT->m_mapANI.end() )
			{
				LPANI pANI = GetCurANI();

				MAPSNDINST::iterator itSND;
				MAPSFXINST::iterator itSFX;

				if( pANI && pANI->m_pANI && (*itANI).second && (*itANI).second->m_pANI )
				{
					BeginBlend( pANI->m_pANI, (*itANI).second->m_pANI, m_fActTime, 0.0f);
					m_dwBlendTick = 0;
				}

				pAI->m_pANI = (*itANI).second;
				pAI->m_dwCurANI = dwAni;
				m_OBJ.m_dwCurACT = dwAction;

				for( itSND = pAI->m_pANI->m_mapSND.begin(); itSND != pAI->m_pANI->m_mapSND.end(); itSND++)
					m_mapANISNDINST.insert( MAPSNDINST::value_type( (*itSND).first, (*itSND).second));

				for( itSFX = pAI->m_pANI->m_mapSFX.begin(); itSFX != pAI->m_pANI->m_mapSFX.end(); itSFX++)
					m_mapANISFXINST.insert( MAPSFXINST::value_type( (*itSFX).first, (*itSFX).second));
			}
			else
			{
				m_OBJ.m_dwCurACT = dwAction;
				pAI->m_dwCurANI = 0;
				pAI->m_pANI = NULL;
			}

			m_fActTime = 0.0f;
			m_nKeyBase = -1;
		}
	}
	else
	{
		MAPACTINST::iterator itAI = m_OBJ.m_mapACT.find(dwAction);
		m_OBJ.m_dwCurACT = 0;

		m_mapACTSFXINST.clear();
		m_mapANISFXINST.clear();
		m_mapACTSNDINST.clear();
		m_mapANISNDINST.clear();

		ClearSFX( &m_vACTSFX, &m_mapACTSFX);
		ClearSFX( &m_vANISFX, &m_mapANISFX);
		ClearSND(&m_mapACTSND);
		ClearSND(&m_mapANISND);

		if( itAI != m_OBJ.m_mapACT.end() )
		{
			delete (*itAI).second;
			m_OBJ.m_mapACT.erase(itAI);
		}

		m_fActTime = 0.0f;
		m_nKeyBase = -1;
	}
}

void CTachyonObject::SetCloth( CD3DDevice *pDevice,
							   DWORD dwKind,
							   DWORD dwCloth,
							   DWORD dwMesh)
{
	MAPCLKIND::iterator itCLK = m_OBJ.m_pOBJ->m_mapCLKIND.find(dwKind);

	if( itCLK != m_OBJ.m_pOBJ->m_mapCLKIND.end() )
	{
		MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(dwKind);
		LPCLKINST pCI = NULL;
		LPCLKIND pCLK = (*itCLK).second;

		if( itCI == m_OBJ.m_mapCLK.end() )
		{
			pCI = new CLKINST();
			m_OBJ.m_mapCLK.insert( MAPCLKINST::value_type( dwKind, pCI));
		}
		else
			pCI = (*itCI).second;

		m_OBJ.m_dwCurCLK = dwKind;
		pCI->m_dwHideID = 0xFFFFFFFF;
		pCI->m_dwCurMESH = 0;
		pCI->m_dwCurCL = 0;
		pCI->m_pMESH = NULL;

		MAPCLOTH::iterator itCL = pCLK->m_mapCLOTH.find(dwCloth);
		if( itCL != pCLK->m_mapCLOTH.end() )
		{
			LPCLOTH pCL = (*itCL).second;
			MAPMESH::iterator itMESH = pCL->m_mapMESH.find(dwMesh);

			if( itMESH != pCL->m_mapMESH.end() )
			{
				pCI->m_pMESH = (*itMESH).second;
				pCI->m_dwCurMESH = dwMesh;
			}

			pCI->m_dwCurCL = dwCloth;
		}
	}
	else
	{
		MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find(dwKind);
		m_OBJ.m_dwCurCLK = 0;

		if( itCI != m_OBJ.m_mapCLK.end() )
		{
			delete (*itCI).second;
			m_OBJ.m_mapCLK.erase(itCI);
		}
	}
}

void CTachyonObject::ResetOBJPart( CD3DDevice *pDevice)
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

					if(!m_bUseSHADER)
						m_bUseSHADER = pDevice->m_bEnableSHADER && pPART->m_pTEX->m_dwOP == D3DTOP_FORCE_DWORD;

					if( m_nVS == VS_COUNT )
						m_nVS = pPART->m_pMESH->m_dwNodeCount ? VS_WMESHVERTEX : VS_MESHVERTEX;

					pDRAW->push_back(pPART);
				}
	}

	m_dwTexTick = 0;
}

void CTachyonObject::ResetOBJ( CD3DDevice *pDevice)
{
	DWORD dwCurCLK = m_OBJ.m_dwCurCLK;
	MAPCLKINST::iterator itCI;

	for( itCI = m_OBJ.m_mapCLK.begin(); itCI != m_OBJ.m_mapCLK.end(); itCI++)
	{
		LPCLKINST pCI = (*itCI).second;
		SetCloth( pDevice, (*itCI).first, pCI->m_dwCurCL, pCI->m_dwCurMESH);
	}

	m_OBJ.m_dwCurCLK = dwCurCLK;
	ResetOBJPart(pDevice);

	MAPACTINST::iterator itAI = m_OBJ.m_mapACT.find(m_OBJ.m_dwCurACT);
	if( itAI != m_OBJ.m_mapACT.end() )
	{
		LPACTINST pAI = (*itAI).second;
		DWORD dwACT = m_OBJ.m_dwCurACT;
		DWORD dwANI = pAI->m_dwCurANI;

		m_OBJ.m_dwCurACT = 0;
		pAI->m_dwCurANI = 0;

		SetAction( dwACT, dwANI);
	}
	else
		SetAction( 0, 0);
}

LPANI CTachyonObject::GetCurANI()
{
	MAPACTINST::iterator itAI = m_OBJ.m_mapACT.find(m_OBJ.m_dwCurACT);

	if( itAI != m_OBJ.m_mapACT.end() )
		return (*itAI).second->m_pANI;

	return NULL;
}

void CTachyonObject::CalcFrame( BOOL bUpdate)
{
	LPANI pANI = GetCurANI();

	if( pANI && pANI->m_pANI )
	{
		pANI->m_pANI->m_fCurTime = m_fActTime;

		ANIKEY vKey = pANI->m_pANI->GetAniKey();
		D3DXMATRIX vScale;
		D3DXMATRIX vRotX;
		D3DXMATRIX vRotY;
		D3DXMATRIX vRotZ;
		D3DXMATRIX vPos;

		D3DXMatrixScaling(
			&vScale,
			vKey.m_fScaleX,
			vKey.m_fScaleY,
			vKey.m_fScaleZ);

		D3DXMatrixRotationX(
			&vRotX,
			vKey.m_fRotX);

		D3DXMatrixRotationY(
			&vRotY,
			vKey.m_fRotY);

		D3DXMatrixRotationZ(
			&vRotZ,
			vKey.m_fRotZ);

		D3DXMatrixTranslation(
			&vPos,
			vKey.m_fPosX,
			vKey.m_fPosY,
			vKey.m_fPosZ);

		if(bUpdate)
		{
			m_vWorld = vRotX * vRotY * vRotZ * vPos * m_vWorld;
			m_vScale = m_vScale * vScale;

			m_vPosition = m_vScale * m_vWorld;
		}
		else
			m_vPosition = m_vScale * vScale * vRotX * vRotY * vRotZ * vPos * m_vWorld;

		if(!pANI->m_pANI->m_vKey.empty())
			while( m_nKeyBase < pANI->m_pANI->m_nKeyBase )
			{
				m_nKeyBase++;

				if( m_pHost && pANI->m_pANI->m_vKey[m_nKeyBase]->m_dwEventID > 0 )
				{
					OBJMSG msg;

					if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
						msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

					msg.m_dwMSG = pANI->m_pANI->m_vKey[m_nKeyBase]->m_dwEventID;
					msg.m_pOBJ = (DWORD_PTR) this;

					m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);
				}
			}
	}
	else
		m_vPosition = m_vScale * m_vWorld;
}

void CTachyonObject::CalcTick( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	LPANI pANI = GetCurANI();

	ResetSND( &m_mapOBJSND, TRUE);
	if( pANI && pANI->m_pANI )
	{
		FLOAT fTotal = pANI->m_pANI->GetTotalTime();

		if( m_dwBlendTick < m_dwBlend )
			m_dwBlendTick += dwTick;

		m_fActTime += CTMath::GetTimeValue(dwTick);

		if( m_fActTime >= fTotal )
		{
			FLOAT fTime = m_fActTime;

			ResetSND( &m_mapACTSND, FALSE);
			ResetSND( &m_mapANISND, FALSE);
			ClearACTSFX();

			if(m_pHost)
			{
				OBJMSG msg;

				if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
					msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

				msg.m_dwMSG = OM_ACTENDING;
				msg.m_pOBJ = (DWORD_PTR) this;

				if(m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg))
				{
					m_fActTime = fTotal;
					return;
				}
			}

			while( fTime >= fTotal )
			{
				m_fActTime = fTotal;

				CalcFrame(TRUE);
				m_nKeyBase = -1;

				fTime -= fTotal;
			}
			m_fActTime = fTime;

			if(m_pHost)
			{
				OBJMSG msg;

				if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
					msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;

				msg.m_dwMSG = OM_ACTENDED;
				msg.m_pOBJ = (DWORD_PTR) this;

				if(m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg))
					return;
			}
		}

		CalcFrame(FALSE);
	}
	else
		m_vPosition = m_vScale * m_vWorld;
		
	m_dwTexTick += dwTick;

	MAPOBJECT::iterator itOBJ;
	for( itOBJ = m_mapEQUIP.begin(); itOBJ != m_mapEQUIP.end(); itOBJ++)
	{
		CTachyonObject *pEQUIP = (*itOBJ).second;

		if(pEQUIP)
			pEQUIP->CalcTick( pDevice, dwTick);
	}

	if(m_bSFX)
	{
		GenerateSFX( &m_mapOBJSFXINST, &m_vOBJSFX, &m_mapOBJSFX, m_fActTime, m_dwOBJSFXFuncID);
		GenerateSFX( &m_mapACTSFXINST, &m_vACTSFX, &m_mapACTSFX, m_fActTime, m_dwACTSFXFuncID);
		GenerateSFX( &m_mapANISFXINST, &m_vANISFX, &m_mapANISFX, m_fActTime, m_dwANISFXFuncID);
	}
	else
		ClearOBJSFX();

	if( CTachyonMedia::m_bON && CD3DSound::m_bON )
	{
		GenerateSND( &m_mapOBJSNDINST, &m_mapOBJSND, m_fActTime, m_dwOBJSNDFuncID);
		GenerateSND( &m_mapACTSNDINST, &m_mapACTSND, m_fActTime, m_dwACTSNDFuncID);
		GenerateSND( &m_mapANISNDINST, &m_mapANISND, m_fActTime, m_dwANISNDFuncID);
	}
	else
		ClearOBJSND();
}

void CTachyonObject::GenerateSND( LPMAPSNDINST pINST,
								  LPMAPOBJSND pSND,
								  FLOAT fActTime,
								  DWORD dwFuncID)
{
	DWORD dwTick = CTMath::GetTimeTick(fActTime);
	MAPSNDINST::iterator it;

	for( it = pINST->begin(); it != pINST->end(); it++)
		if( (*it).second->m_dwTick <= dwTick && (!(*it).second->m_dwFuncID || (*it).second->m_dwFuncID == dwFuncID) )
		{
			MAPOBJSND::iterator finder = pSND->find((*it).first);

			if( finder == pSND->end() )
			{
				CD3DSound *pWAV = (CD3DSound *) m_pMedia->GetData(
					MEDIA_TSOUND,
					(*it).second->m_dwMediaID);
				int nIndex = pWAV->Lock();

				if( nIndex >= 0 )
				{
					LPOBJSND pItem = new OBJSND();

					pItem->m_pSND = (*it).second;
					pItem->m_pWAV = pWAV;
					pItem->m_nIndex = nIndex;

					pSND->insert( MAPOBJSND::value_type(
						(*it).first,
						pItem));
				}
			}
			else if((*finder).second->m_bReset)
			{
				(*finder).second->m_bReset = FALSE;
				(*finder).second->m_bPlay = TRUE;
			}
		}
}

void CTachyonObject::GenerateSFX( LPMAPSFXINST pINST,
								  LPVECTOROBJSFX pSFX,
								  LPMAPDWORD pINDEX,
								  FLOAT fActTime,
								  DWORD dwFuncID)
{
	DWORD dwTick = CTMath::GetTimeTick(fActTime);
	MAPSFXINST::iterator it;

	for( it = pINST->begin(); it != pINST->end(); it++)
	{
		if( (*it).second->m_dwTick <= dwTick && (!(*it).second->m_dwFuncID || (*it).second->m_dwFuncID == dwFuncID) )
		{
#ifdef HEBA
			if( !(*it).second->m_mapHideByAction.empty() )
			{
				MAPDWORD::iterator heba_finder = (*it).second->m_mapHideByAction.find(m_OBJ.m_dwCurACT);
				if( heba_finder != (*it).second->m_mapHideByAction.end() )
					continue;
			}
#endif
			MAPDWORD::iterator finder = pINDEX->find((*it).first);

			if( finder == pINDEX->end() )
			{
				CTachyonSFX *pTSFX = new CTachyonSFX();
				LPOBJSFX pItem = new OBJSFX();

				pItem->m_pSFX = (*it).second;
				pTSFX->m_bHasHost = TRUE;
				pTSFX->m_pHost = m_pHost;

				pTSFX->m_dwInitTick = dwTick - (*it).second->m_dwTick;
				pItem->m_dwID = CTachyonSFX::RegSFX(
					pTSFX,
					(*it).second->m_bBias);
				pSFX->push_back(pItem);

				pINDEX->insert( MAPDWORD::value_type(
					(*it).first,
					(*it).first));
			}
		}
	}
}

LPD3DXMATRIX CTachyonObject::GetMeshMatrix()
{
	MAPCLKINST::iterator it;

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH )
			return pCI->m_pMESH->m_pMESH->m_pBones;
	}

	return NULL;
}

void CTachyonObject::ApplyMatrix( CD3DDevice *pDevice)
{
	static FLOAT vWORLD[4 * 3 * MAX_PIVOT + 4];
	LPANI pANI = GetCurANI();

	if( pANI && pANI->m_pANI )
	{
		pANI->m_pANI->m_fCurTime = m_fActTime;
		LPANIDATA pDATA = pANI->m_pANI->GetAniData();

		if( pDATA && pDATA->m_pAni )
		{
			FLOAT fBlendTime = FLOAT(m_dwBlendTick) / FLOAT(m_dwBlend);
			BYTE bBlend = (m_dwBlendTick < m_dwBlend ? TRUE : FALSE) && m_bBlend;

			LPD3DXMATRIX pInit = GetMeshMatrix();

			pDATA->m_pAni->GetFrameMatrix(
				m_pBone,
				bBlend ? m_pBlend : NULL,
				bBlend ? m_pBlendKEY : NULL,
				m_pPivot,
				m_vPosition, 0,
				pANI->m_pANI->m_fLocalTime,
				fBlendTime);

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

void CTachyonObject::ApplyTexture( CD3DDevice *pDevice,
								   LPOBJTEX pTEX,
								   FLOAT *pARGCONST,
								   int nSTG)
{
	static D3DXMATRIX vUV;

	if( pTEX && pTEX->m_pTEX[nSTG] )
	{
		pTEX->m_pTEX[nSTG]->m_dwCurTick = m_dwTexTick;
		pTEX->m_pTEX[nSTG]->m_dwCurTick %= pTEX->m_pTEX[nSTG]->m_dwTotalTick;

		LPDIRECT3DTEXTURE9 pTexture = pTEX->m_pTEX[nSTG]->GetTexture();
		UVKEY vKey = pTEX->m_pTEX[nSTG]->GetUV();
		FLOAT fMIP = pTEX->m_pTEX[nSTG]->m_fMipBias + m_fMipFactor;

		CTMath::GetUVMatrix( &vUV, &vKey);
		if(m_bUseSHADER)
		{
			static FLOAT vTEX[16];

			D3DXMatrixTranspose( (LPD3DXMATRIX) vTEX, &vUV);
			pDevice->m_pDevice->SetVertexShaderConstantF(
				pDevice->m_vConstantVS[nSTG == 0 ? VC_TEXTRAN0 : VC_TEXTRAN1],
				vTEX, 2);
			pARGCONST[nSTG == 0 ? 0 : 1] = 1.0f;
		}
		else
			pDevice->m_pDevice->SetTransform( nSTG == 0 ? D3DTS_TEXTURE0 : D3DTS_TEXTURE1, &vUV);

		pDevice->m_pDevice->SetSamplerState( nSTG, D3DSAMP_MIPFILTER, pTEX->m_pTEX[nSTG]->m_dwMipFilter);
		pDevice->m_pDevice->SetSamplerState( nSTG, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
		pDevice->m_pDevice->SetTexture( nSTG, pTexture);
	}
	else
	{
		D3DXMatrixIdentity(&vUV);

		if(m_bUseSHADER)
		{
			pARGCONST[nSTG == 0 ? 0 : 1] = -1.0f;
			pTEX->m_dwCOLOR = 0xFFFFFFFF;

			pDevice->m_pDevice->SetVertexShaderConstantF(
				pDevice->m_vConstantVS[nSTG == 0 ? VC_TEXTRAN0 : VC_TEXTRAN1],
				(FLOAT *) &vUV, 2);
		}
		else
			pDevice->m_pDevice->SetTransform( nSTG == 0 ? D3DTS_TEXTURE0 : D3DTS_TEXTURE1, &vUV);

		pDevice->m_pDevice->SetTexture( nSTG, NULL);
	}
}

void CTachyonObject::Render( CD3DDevice *pDevice, CD3DCamera *pCamera)
{
	static DWORD vCOLOROP[PS_COUNT] = {
		D3DTOP_DISABLE,						// PS_STAGE1
		D3DTOP_MODULATE,					// PS_MODULATE
		D3DTOP_MODULATE2X,					// PS_MODULATE2X
		D3DTOP_MODULATE4X,					// PS_MODULATE4X
		D3DTOP_ADD,							// PS_ADD
		D3DTOP_ADDSIGNED,					// PS_ADDSIGNED
		D3DTOP_ADDSIGNED2X,					// PS_ADDSIGNED2X
		D3DTOP_SUBTRACT,					// PS_SUBTRACT
		D3DTOP_ADDSMOOTH,					// PS_ADDSMOOTH
		D3DTOP_BLENDDIFFUSEALPHA,			// PS_BLENDDIFFUSEALPHA
		D3DTOP_BLENDTEXTUREALPHA,			// PS_BLENDTEXTUREALPHA
		D3DTOP_BLENDFACTORALPHA,			// PS_BLENDFACTORALPHA
		D3DTOP_BLENDTEXTUREALPHAPM,			// PS_BLENDTEXTUREALPHAPM
		D3DTOP_BLENDCURRENTALPHA,			// PS_BLENDCURRENTALPHA
		D3DTOP_MODULATEALPHA_ADDCOLOR,		// PS_MODULATEALPHA_ADDCOLOR
		D3DTOP_MODULATECOLOR_ADDALPHA,		// PS_MODULATECOLOR_ADDALPHA
		D3DTOP_MODULATEINVALPHA_ADDCOLOR,	// PS_MODULATEINVALPHA_ADDCOLOR
		D3DTOP_MODULATEINVCOLOR_ADDALPHA,	// PS_MODULATEINVCOLOR_ADDALPHA
		D3DTOP_DOTPRODUCT3,					// PS_DOTPRODUCT3
		D3DTOP_MULTIPLYADD,					// PS_MULTIPLYADD
		D3DTOP_LERP,						// PS_LERP
		D3DTOP_DISABLE,						// PS_SHADER
		D3DTOP_DISABLE,						// PS_DETAILMAP
		D3DTOP_DISABLE};					// PS_MAP

	static BYTE vALPHAOP[PS_COUNT] = {
		FALSE,	// PS_STAGE1
		TRUE,	// PS_MODULATE
		TRUE,	// PS_MODULATE2X
		TRUE,	// PS_MODULATE4X
		TRUE,	// PS_ADD
		TRUE,	// PS_ADDSIGNED
		TRUE,	// PS_ADDSIGNED2X
		TRUE,	// PS_SUBTRACT
		TRUE,	// PS_ADDSMOOTH
		TRUE,	// PS_BLENDDIFFUSEALPHA
		TRUE,	// PS_BLENDTEXTUREALPHA
		TRUE,	// PS_BLENDFACTORALPHA
		TRUE,	// PS_BLENDTEXTUREALPHAPM
		TRUE,	// PS_BLENDCURRENTALPHA
		FALSE,	// PS_MODULATEALPHA_ADDCOLOR
		FALSE,	// PS_MODULATECOLOR_ADDALPHA
		FALSE,	// PS_MODULATEINVALPHA_ADDCOLOR
		FALSE,	// PS_MODULATEINVCOLOR_ADDALPHA
		TRUE,	// PS_DOTPRODUCT3
		TRUE,	// PS_MULTIPLYADD
		TRUE,	// PS_LERP
		TRUE,	// PS_SHADER
		TRUE,	// PS_DETAILMAP
		TRUE};	// PS_MAP

	FLOAT fDIST = D3DXVec3Length(&D3DXVECTOR3(
		m_vPosition._41 - pCamera->m_vPosition.x,
		m_vPosition._42 - pCamera->m_vPosition.y,
		m_vPosition._43 - pCamera->m_vPosition.z));

	BYTE bPSC = pDevice->m_vCAPS.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT ? TRUE : FALSE;
	BYTE bALPHA = max( 1, m_bAlpha) - 1;

	if( m_bSelAlpha != 0xFF )
		bALPHA = max( 1, m_bSelAlpha) - 1;

	LPANI pANI = GetCurANI();
	MAPOBJPART::iterator it;

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->m_pDevice->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	ApplyMatrix(pDevice);

	for( it = m_OBJ.m_mapDRAW.begin(); it != m_OBJ.m_mapDRAW.end(); it++)
	{
		VECTOROBJPART *pDRAW = (*it).second;

		for( int i=0; i<INT(pDRAW->size()); i++)
		{
			LPOBJPART pPART = (*pDRAW)[i];

			PSTYPE nPS = GetPSType(pPART->m_pTEX->m_dwOP);
			FLOAT vARG[4] = {
				-1.0f,
				-1.0f,
				pPART->m_pTEX->m_fIntensity, 0.0f};
			BYTE bUseDirLight = !bPSC || pPART->m_pTEX->m_bUseDirLight ? TRUE : FALSE;

			DWORD dwALPHAOP = vALPHAOP[nPS] ? vCOLOROP[nPS] : D3DTOP_DISABLE;
			DWORD dwCOLOROP = vCOLOROP[nPS];
			DWORD dwARG2 = bUseDirLight ? D3DTA_DIFFUSE : D3DTA_CONSTANT;

			switch(pPART->m_pTEX->m_bType[0])
			{
			case TT_TEX		:
				{
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwARG2);

					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwARG2);

					ApplyTexture( pDevice, pPART->m_pTEX, vARG, 0);
				}

				break;

			case TT_COLOR	:
				if(!m_bUseSHADER)
				{
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwARG2);

					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwARG2);
					pDevice->m_pDevice->SetTexture( 0, NULL);
				}

				break;

			case TT_ENV		:
				{
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, dwARG2);

					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, dwARG2);
				}

				break;
			}

			switch(pPART->m_pTEX->m_bType[1])
			{
			case TT_TEX		:
				{
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, pPART->m_pMESH->m_dwNodeCount ? 0 : 1);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwCOLOROP);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

					ApplyTexture( pDevice, pPART->m_pTEX, vARG, 1);
				}

				break;

			case TT_COLOR	:
				if(!m_bUseSHADER)
				{
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwCOLOROP);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TFACTOR);

					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

					pDevice->m_pDevice->SetTexture( 1, NULL);
				}

				break;

			case TT_ENV		:
				{
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, dwCOLOROP);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
					pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
				}

				break;
			}

			pDevice->m_pDevice->SetRenderState( D3DRS_CULLMODE, pPART->m_pTEX->m_b2side ? D3DCULL_NONE : D3DCULL_CCW);
			pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, (DWORD) min( bALPHA, pPART->m_pTEX->m_bMinAlpha));
			pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, m_bZWrite && pPART->m_pTEX->m_bZWrite);
			pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, m_bZEnable && pPART->m_pTEX->m_bZEnable);

			pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, pPART->m_pTEX->m_dwDESTBlend);
			pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, pPART->m_pTEX->m_dwSRCBlend);
			pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, pPART->m_pTEX->m_dwBlendOP);

			MAPDWORD::iterator itCOLOR = m_mapCOLOR.find(pPART->m_pTEX->m_dwCOLORID);
			DWORD dwCOLOR = pPART->m_pTEX->m_dwCOLOR;

			if(!bUseDirLight)
				pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_CONSTANT, pPART->m_pTEX->m_dwAmbientCOLOR);

			if( itCOLOR != m_mapCOLOR.end() )
				dwCOLOR = (*itCOLOR).second;

			if(m_bUseSHADER)
			{
				FLOAT vCOLOR[4] = {
					((FLOAT) ((dwCOLOR & 0x00FF0000) >> 16)) / 255.0f,
					((FLOAT) ((dwCOLOR & 0x0000FF00) >> 8)) / 255.0f,
					((FLOAT) (dwCOLOR & 0x000000FF)) / 255.0f,
					((FLOAT) ((dwCOLOR & 0xFF000000) >> 24)) / 255.0f};

				if( m_nVS == VS_WMESHVERTEX )
				{
					BOOL bSkinning = pPART->m_pMESH->m_dwNodeCount && pANI && pANI->m_pANI ? TRUE : FALSE;

					pDevice->m_pDevice->SetVertexShaderConstantB(
						pDevice->m_vConstantVS[VC_SKINNING],
						&bSkinning, 1);
				}

				pDevice->m_pDevice->SetPixelShaderConstantF(
					pDevice->m_vConstantPS[PC_ARG],
					vARG, 1);

				pDevice->m_pDevice->SetPixelShaderConstantF(
					pDevice->m_vConstantPS[PC_TFACTOR],
					vCOLOR, 1);

				pDevice->m_pDevice->SetVertexShader(pDevice->m_pVertexShader[m_nVS]);
				pDevice->m_pDevice->SetVertexDeclaration(pDevice->m_pDECL[m_nVS]);
				pDevice->m_pDevice->SetPixelShader(pDevice->m_pPixelShader[nPS]);
			}
			else
			{
				pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, pPART->m_pMESH->m_dwNodeCount && pANI && pANI->m_pANI ? D3DVBF_3WEIGHTS : D3DVBF_DISABLE);
				pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwCOLOR);

				pDevice->m_pDevice->SetFVF(pPART->m_pMESH->m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);
			}

			if( pPART->m_pMESH->m_bVBType != VBTYPE_GLOBAL || pPART->m_pMESH->m_bUseVB )
				pPART->m_pMESH->Render( pDevice->m_pDevice, pPART->m_dwIndex, m_bLOD ? pPART->m_pMESH->GetLevel(fDIST) : 0);

			if(bPSC)
				pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_CONSTANT, 0xFFFFFFFF);

			pDevice->m_pDevice->SetVertexShader(NULL);
			pDevice->m_pDevice->SetPixelShader(NULL);
		}
	}

	MAPOBJECT::iterator itOBJ;
	for( itOBJ = m_mapEQUIP.begin(); itOBJ != m_mapEQUIP.end(); itOBJ++)
	{
		CTachyonObject *pEQUIP = (*itOBJ).second;

		if(pEQUIP)
		{
			D3DXMATRIX vPosition = pEQUIP->m_vPosition;

			pEQUIP->m_vPosition *= m_pBone[(*itOBJ).first];
			pEQUIP->m_bAlpha = m_bAlpha;
			pEQUIP->m_bLOD = m_bLOD;
			pEQUIP->Render( pDevice, pCamera);
			pEQUIP->m_vPosition = vPosition;
		}
	}

	CalcSND( &m_mapOBJSNDINST, &m_mapOBJSND, m_fActTime, m_dwOBJSNDFuncID);
	CalcSND( &m_mapACTSNDINST, &m_mapACTSND, m_fActTime, m_dwACTSNDFuncID);
	CalcSND( &m_mapANISNDINST, &m_mapANISND, m_fActTime, m_dwANISNDFuncID);

	if( m_bSFX )
	{
		CalcSFX( pDevice, &m_vOBJSFX);
		CalcSFX( pDevice, &m_vACTSFX);
		CalcSFX( pDevice, &m_vANISFX);
	}

	pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	pDevice->m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
}

void CTachyonObject::CalcSND( LPMAPSNDINST pINST,
							  LPMAPOBJSND pSND,
							  FLOAT fActTime,
							  DWORD dwFuncID)
{
	DWORD dwTick = CTMath::GetTimeTick(fActTime);
	MAPSNDINST::iterator it;

	for( it = pINST->begin(); it != pINST->end(); it++)
		if( (*it).second->m_dwTick <= dwTick && (!(*it).second->m_dwFuncID || (*it).second->m_dwFuncID == dwFuncID) )
		{
			MAPOBJSND::iterator finder = pSND->find((*it).first);

			if( finder != pSND->end() && (*finder).second->m_bPlay )
			{
				LPDIRECTSOUND3DBUFFER p3DBUF = (*finder).second->m_pWAV->GetDS3D((*finder).second->m_nIndex);

				(*finder).second->m_pWAV->Stop((*finder).second->m_nIndex);
				if(p3DBUF)
				{
					D3DXVECTOR3 vSNDPOS = CD3DSound::ConvertPOS(
						m_vPosition._41,
						m_vPosition._42,
						m_vPosition._43);

					p3DBUF->SetPosition(
						vSNDPOS.x,
						vSNDPOS.y,
						vSNDPOS.z,
						DS3D_IMMEDIATE);
				}

				(*finder).second->m_pWAV->Play((*finder).second->m_nIndex);
				(*finder).second->m_bPlay = FALSE;
			}
		}
}

void CTachyonObject::CalcSFX( CD3DDevice *pDevice, LPVECTOROBJSFX pSFX)
{
	VECTOROBJSFX::iterator it = pSFX->begin();

	while(it != pSFX->end())
	{
		LPOBJSFX pItem = (*it);
		CTachyonSFX *pTSFX = CTachyonSFX::GetSFX(pItem->m_dwID);

		if( !pTSFX->m_pSFX || !pTSFX->IsDead() )
		{
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;

			D3DXMatrixRotationX(
				&vRotX,
				pItem->m_pSFX->m_vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				pItem->m_pSFX->m_vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				pItem->m_pSFX->m_vROT.z);
			pTSFX->m_vGlobal = vRotX * vRotY * vRotZ;

			pTSFX->m_vGlobal._41 = pItem->m_pSFX->m_vPOS.x;
			pTSFX->m_vGlobal._42 = pItem->m_pSFX->m_vPOS.y;
			pTSFX->m_vGlobal._43 = pItem->m_pSFX->m_vPOS.z;

			if(pItem->m_pSFX->m_nPivotID)
				pTSFX->m_vGlobal *= m_pBone[pItem->m_pSFX->m_nPivotID];
			else
				pTSFX->m_vGlobal *= m_vPosition;

			if(!pTSFX->m_pSFX)
			{
				DWORD dwInitTick = pTSFX->m_dwInitTick;

				pTSFX->InitSFX(
					pDevice,
					pItem->m_pSFX->m_pSFX);
				pTSFX->m_dwInitTick = dwInitTick;
			}

			it++;
		}
		else
		{
			CTachyonSFX::Release(pItem->m_dwID);
			delete pItem;

			pSFX->erase(it);
		}
	}
}

BOOL CTachyonObject::GetSize( D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
{
	MAPCLKINST::iterator it;
	BOOL bInit = FALSE;

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH )
			if(bInit)
			{
				D3DXVECTOR3 vLocalMax = pCI->m_pMESH->m_pMESH->m_vCenterPoint + D3DXVECTOR3(
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius);

				D3DXVECTOR3 vLocalMin = pCI->m_pMESH->m_pMESH->m_vCenterPoint - D3DXVECTOR3(
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius);

				if( vMax.x < vLocalMax.x )
					vMax.x = vLocalMax.x;

				if( vMax.y < vLocalMax.y )
					vMax.y = vLocalMax.y;

				if( vMax.z < vLocalMax.z )
					vMax.z = vLocalMax.z;

				if( vMin.x > vLocalMin.x )
					vMin.x = vLocalMin.x;

				if( vMin.y > vLocalMin.y )
					vMin.y = vLocalMin.y;

				if( vMin.z > vLocalMin.z )
					vMin.z = vLocalMin.z;
			}
			else
			{
				vMax = pCI->m_pMESH->m_pMESH->m_vCenterPoint + D3DXVECTOR3(
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius);

				vMin = pCI->m_pMESH->m_pMESH->m_vCenterPoint - D3DXVECTOR3(
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius,
					pCI->m_pMESH->m_pMESH->m_fRadius);

				bInit = TRUE;
			}
	}

	return bInit;
}

BOOL CTachyonObject::OBJInRect( CD3DCamera *pCamera, CRect rect)
{
	if( rect.left > rect.right )
	{
		int nBUF = rect.right;

		rect.right = rect.left;
		rect.left = nBUF;
	}

	if( rect.top > rect.bottom )
	{
		int nBUF = rect.bottom;

		rect.bottom = rect.top;
		rect.top = nBUF;
	}

	if( rect.left == rect.right &&
		rect.top == rect.bottom )
	{
		OBJHIT vHIT = HitTest(
			pCamera,
			rect.left,
			rect.top);

		return vHIT.m_nIndex != -1;
	}

	if( rect.left == rect.right )
		rect.right++;

	if( rect.top == rect.bottom )
		rect.bottom++;

	FLOAT fLevel = D3DXVec3Length(&D3DXVECTOR3(
		m_vPosition._41 - pCamera->m_vPosition.x,
		m_vPosition._42 - pCamera->m_vPosition.y,
		m_vPosition._43 - pCamera->m_vPosition.z));

	D3DXVECTOR3 vDIR[4] = {
		pCamera->GetRayDirection( rect.left, rect.top),
		pCamera->GetRayDirection( rect.right, rect.top),
		pCamera->GetRayDirection( rect.right, rect.bottom),
		pCamera->GetRayDirection( rect.left, rect.bottom)};

	for( int i=0; i<4; i++)
		vDIR[i] /= D3DXVec3Length(&vDIR[i]);

	D3DXVECTOR3 vPOS[4][3] = {{
		pCamera->GetCameraPosition( rect.left, rect.top),
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0],
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1]}, {
		pCamera->GetCameraPosition( rect.right, rect.top),
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1],
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2]}, {
		pCamera->GetCameraPosition( rect.right, rect.bottom),
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2],
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3]}, {
		pCamera->GetCameraPosition( rect.left, rect.bottom),
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3],
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0]}};
	D3DXPLANE vPLANE[5];

	for( int i=0; i<4; i++)
	{
		D3DXPlaneFromPoints(
			&vPLANE[i],
			&vPOS[i][0],
			&vPOS[i][1],
			&vPOS[i][2]);
	}

	D3DXPlaneFromPointNormal(
		&vPLANE[4],
		&pCamera->m_vPosition,
		&(pCamera->m_vPosition - pCamera->m_vTarget));

	LPANI pANI = GetCurANI();
	MAPCLKINST::iterator it;

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			LPD3DXVECTOR3 vPoint = new D3DXVECTOR3[pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount];
			CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;
			int nLevel = pMESH->GetLevel(fLevel);

			if( pMESH->m_dwNodeCount > 0 )
			{
				LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( i=0; i<pMESH->m_pDATA->m_dwCount; i++)
				{
					D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
						m_pBone,
						GetMeshMatrix(),
						&pDATA[i]) : m_vPosition;

					vPoint[i] = D3DXVECTOR3(
						pDATA[i].m_fPosX,
						pDATA[i].m_fPosY,
						pDATA[i].m_fPosZ);

					CTMath::Transform(
						&vTrans,
						&vPoint[i]);
				}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( i=0; i<pMESH->m_pDATA->m_dwCount; i++)
				{
					vPoint[i] = D3DXVECTOR3(
						pDATA[i].m_fPosX,
						pDATA[i].m_fPosY,
						pDATA[i].m_fPosZ);

					CTMath::Transform(
						&m_vPosition,
						&vPoint[i]);
				}
			}

			for( i=0; i<pMESH->m_dwMeshCount; i++)
				for( int j=0; j<INT(pMESH->m_pMESH[i][nLevel]->m_vIB.size()); j++)
					for( int k=0; k<pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_dwCount / 3; k++)
					{
						BOOL bResult = TRUE;

						for( int l=0; l<5; l++)
							if( D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]]) > 0 &&
								D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 1]]) > 0 &&
								D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 2]]) > 0 )
							{
								bResult = FALSE;
								break;
							}

						if(bResult)
						{
							delete[] vPoint;
							return TRUE;
						}
					}

			delete[] vPoint;
		}
	}

	return FALSE;
}

OBJHIT CTachyonObject::HitTest( CD3DCamera *pCam,
							    int nPosX,
								int nPosY)
{
	FLOAT fLevel = D3DXVec3Length(&D3DXVECTOR3(
		m_vPosition._41 - pCam->m_vPosition.x,
		m_vPosition._42 - pCam->m_vPosition.y,
		m_vPosition._43 - pCam->m_vPosition.z));

	D3DXVECTOR3 vPos = pCam->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDir = pCam->GetRayDirection( nPosX, nPosY);

	LPANI pANI = GetCurANI();
	MAPCLKINST::iterator it;
	OBJHIT vHIT;

	for( it = m_OBJ.m_mapCLK.begin(); it != m_OBJ.m_mapCLK.end(); it++)
	{
		LPCLKINST pCI = (*it).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			D3DXVECTOR3 vCenter = pCI->m_pMESH->m_pMESH->m_vCenterPoint;
			FLOAT fScale = max( max(
				m_vScale._11,
				m_vScale._22),
				m_vScale._33);

			CTMath::Transform(
				&m_vPosition,
				&vCenter);

			if(D3DXSphereBoundProbe( &vCenter, fScale * pCI->m_pMESH->m_pMESH->m_fRadius, &vPos, &vDir))
			{
				LPD3DXVECTOR3 vPoint = new D3DXVECTOR3[pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount];
				CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;
				int nLevel = pMESH->GetLevel(fLevel);

				if( pMESH->m_dwNodeCount > 0 )
				{
					LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

					for( int i=0; i<pMESH->m_pDATA->m_dwCount; i++)
					{
						D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
							m_pBone,
							GetMeshMatrix(),
							&pDATA[i]) : m_vPosition;

						vPoint[i] = D3DXVECTOR3(
							pDATA[i].m_fPosX,
							pDATA[i].m_fPosY,
							pDATA[i].m_fPosZ);

						CTMath::Transform(
							&vTrans,
							&vPoint[i]);
					}
				}
				else
				{
					LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

					for( int i=0; i<pMESH->m_pDATA->m_dwCount; i++)
					{
						vPoint[i] = D3DXVECTOR3(
							pDATA[i].m_fPosX,
							pDATA[i].m_fPosY,
							pDATA[i].m_fPosZ);

						CTMath::Transform(
							&m_vPosition,
							&vPoint[i]);
					}
				}

				for( int i=0; i<pMESH->m_dwMeshCount; i++)
				{
					nLevel = min( nLevel, INT(pMESH->m_pMESH[i].size()) - 1);

					for( int j=0; j<INT(pMESH->m_pMESH[i][nLevel]->m_vIB.size()); j++)
						for( int k=0; k<pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_dwCount / 3; k++)
						{
							D3DXVECTOR3 vCross;

							FLOAT fCurDist = 0.0f;
							FLOAT fU;
							FLOAT fV;

							D3DXVec3Cross(
								&vCross,
								&(vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 1]] - vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]]),
								&(vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 2]] - vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]]));

							if( D3DXIntersectTri(
								&vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k]],
								&vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 1]],
								&vPoint[pMESH->m_pMESH[i][nLevel]->m_vIB[j]->m_pIB[3 * k + 2]],
								&vPos, &vDir,
								&fU, &fV,
								&fCurDist) && ((
								INT(pCI->m_pMESH->m_vTEX.size()) > i &&
								pCI->m_pMESH->m_vTEX[i] &&
								pCI->m_pMESH->m_vTEX[i]->m_b2side) ||
								D3DXVec3Dot(
								&vCross,
								&vDir) < 0.0f ) && (
								vHIT.m_nIndex == -1 ||
								fCurDist < vHIT.m_fDist))
							{
								vHIT.m_dwMESH = pCI->m_dwCurMESH;
								vHIT.m_dwCLK = (*it).first;
								vHIT.m_dwCL = pCI->m_dwCurCL;

								vHIT.m_pMESH = pCI->m_pMESH;
								vHIT.m_fDist = fCurDist;
								vHIT.m_nIndex = i;
							}
						}
				}

				delete[] vPoint;
			}
		}
	}

	return vHIT;
}

CString CTachyonObject::GetAttrSTR( DWORD dwAttrID)
{
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);

	if( finder != m_OBJ.m_mapATTR.end() && (*finder).second )
	{
		LPATTRIBUTE pATTR = (*finder).second;

		if( pATTR->m_bValueType == ATTRTYPE_STRING && pATTR->m_pValue )
			return CString(pATTR->m_pValue).Left(pATTR->m_nSize);
	}

	if(m_OBJ.m_pOBJ)
	{
		finder = m_OBJ.m_pOBJ->m_mapATTR.find(dwAttrID);

		if( finder != m_OBJ.m_pOBJ->m_mapATTR.end() && (*finder).second )
		{
			LPATTRIBUTE pATTR = (*finder).second;

			if( pATTR->m_bValueType == ATTRTYPE_STRING && pATTR->m_pValue )
				return CString(pATTR->m_pValue).Left(pATTR->m_nSize);
		}
	}

	return CString("");
}

FLOAT CTachyonObject::GetAttrFLOAT( DWORD dwAttrID)
{
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);

	if( finder != m_OBJ.m_mapATTR.end() && (*finder).second )
	{
		LPATTRIBUTE pATTR = (*finder).second;

		if( pATTR->m_bValueType == ATTRTYPE_FLOAT && pATTR->m_pValue )
			return *((FLOAT *) pATTR->m_pValue);
	}

	if(m_OBJ.m_pOBJ)
	{
		finder = m_OBJ.m_pOBJ->m_mapATTR.find(dwAttrID);

		if( finder != m_OBJ.m_pOBJ->m_mapATTR.end() && (*finder).second )
		{
			LPATTRIBUTE pATTR = (*finder).second;

			if( pATTR->m_bValueType == ATTRTYPE_FLOAT && pATTR->m_pValue )
				return *((FLOAT *) pATTR->m_pValue);
		}
	}

	return 0.0f;
}

int CTachyonObject::GetAttrINT( DWORD dwAttrID)
{
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);

	if( finder != m_OBJ.m_mapATTR.end() && (*finder).second )
	{
		LPATTRIBUTE pATTR = (*finder).second;

		if( pATTR->m_bValueType == ATTRTYPE_INT && pATTR->m_pValue )
			return *((int *) pATTR->m_pValue);
	}

	if(m_OBJ.m_pOBJ)
	{
		finder = m_OBJ.m_pOBJ->m_mapATTR.find(dwAttrID);

		if( finder != m_OBJ.m_pOBJ->m_mapATTR.end() && (*finder).second )
		{
			LPATTRIBUTE pATTR = (*finder).second;

			if( pATTR->m_bValueType == ATTRTYPE_INT && pATTR->m_pValue )
				return *((int *) pATTR->m_pValue);
		}
	}

	return 0;
}

void CTachyonObject::SetATTR( DWORD dwAttrID, CString strValue)
{
	if( strValue == GetAttrSTR(dwAttrID) )
		return;
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);
	LPATTRIBUTE pATTR = NULL;

	if( finder == m_OBJ.m_mapATTR.end() )
	{
		pATTR = new ATTRIBUTE();
		m_OBJ.m_mapATTR.insert( MAPATTRIBUTE::value_type( dwAttrID, pATTR));
	}
	else if((*finder).second)
	{
		pATTR = (*finder).second;
		if(pATTR->m_pValue)
			delete[] pATTR->m_pValue;
	}
	else
	{
		pATTR = new ATTRIBUTE();
		(*finder).second = pATTR;
	}

	pATTR->m_nSize = strValue.GetLength();
	pATTR->m_pValue = new BYTE[pATTR->m_nSize];
	pATTR->m_bValueType = ATTRTYPE_STRING;

	memcpy( pATTR->m_pValue, LPCSTR(strValue), pATTR->m_nSize);
}

void CTachyonObject::SetATTR( DWORD dwAttrID, FLOAT fValue)
{
	if( fValue == GetAttrFLOAT(dwAttrID) )
		return;
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);
	LPATTRIBUTE pATTR = NULL;

	if( finder == m_OBJ.m_mapATTR.end() )
	{
		pATTR = new ATTRIBUTE();
		m_OBJ.m_mapATTR.insert( MAPATTRIBUTE::value_type( dwAttrID, pATTR));
	}
	else if((*finder).second)
	{
		pATTR = (*finder).second;
		if(pATTR->m_pValue)
			delete[] pATTR->m_pValue;
	}
	else
	{
		pATTR = new ATTRIBUTE();
		(*finder).second = pATTR;
	}

	pATTR->m_pValue = new BYTE[sizeof(FLOAT)];
	pATTR->m_bValueType = ATTRTYPE_FLOAT;
	pATTR->m_nSize = sizeof(FLOAT);

	*((FLOAT *) pATTR->m_pValue) = fValue;
}

void CTachyonObject::SetATTR( DWORD dwAttrID, int nValue)
{
	if( nValue == GetAttrINT(dwAttrID) )
		return;
	MAPATTRIBUTE::iterator finder = m_OBJ.m_mapATTR.find(dwAttrID);
	LPATTRIBUTE pATTR = NULL;

	if( finder == m_OBJ.m_mapATTR.end() )
	{
		pATTR = new ATTRIBUTE();
		m_OBJ.m_mapATTR.insert( MAPATTRIBUTE::value_type( dwAttrID, pATTR));
	}
	else if((*finder).second)
	{
		pATTR = (*finder).second;
		if(pATTR->m_pValue)
			delete[] pATTR->m_pValue;
	}
	else
	{
		pATTR = new ATTRIBUTE();
		(*finder).second = pATTR;
	}

	pATTR->m_pValue = new BYTE[sizeof(int)];
	pATTR->m_bValueType = ATTRTYPE_INT;
	pATTR->m_nSize = sizeof(int);

	*((int *) pATTR->m_pValue) = nValue;
}

void CTachyonObject::SetPIVOT( DWORD dwPivotID,
							   FLOAT fRotX,
							   FLOAT fRotY,
							   FLOAT fRotZ,
							   FLOAT fScaleX,
							   FLOAT fScaleY,
							   FLOAT fScaleZ)
{
	MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwPivotID);
	D3DXQUATERNION vPIVOT;

	if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
	{
		D3DXQuaternionRotationAxis(
			&m_pPivot[(*itID).second].m_vRot,
			&D3DXVECTOR3( 1.0f, 0.0f, 0.0f),
			fRotX);

		D3DXQuaternionRotationAxis(
			&vPIVOT,
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
			fRotY);
		m_pPivot[(*itID).second].m_vRot *= vPIVOT;

		D3DXQuaternionRotationAxis(
			&vPIVOT,
			&D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
			fRotZ);

		m_pPivot[(*itID).second].m_vRot *= vPIVOT;

		m_pPivot[(*itID).second].m_vScale.x = fScaleX;
		m_pPivot[(*itID).second].m_vScale.y = fScaleY;
		m_pPivot[(*itID).second].m_vScale.z = fScaleZ;
	}
}

PSTYPE CTachyonObject::GetPSType( DWORD dwOP)
{
	switch(dwOP)
	{
	case D3DTOP_DISABLE						: return PS_STAGE1;
	case D3DTOP_MODULATE					: return PS_MODULATE;
	case D3DTOP_MODULATE2X					: return PS_MODULATE2X;
	case D3DTOP_MODULATE4X					: return PS_MODULATE4X;
	case D3DTOP_ADD							: return PS_ADD;
	case D3DTOP_ADDSIGNED					: return PS_ADDSIGNED;
	case D3DTOP_ADDSIGNED2X					: return PS_ADDSIGNED2X;
	case D3DTOP_SUBTRACT					: return PS_SUBTRACT;
	case D3DTOP_ADDSMOOTH					: return PS_ADDSMOOTH;
	case D3DTOP_BLENDDIFFUSEALPHA			: return PS_BLENDDIFFUSEALPHA;
	case D3DTOP_BLENDTEXTUREALPHA			: return PS_BLENDTEXTUREALPHA;
	case D3DTOP_BLENDFACTORALPHA			: return PS_BLENDFACTORALPHA;
	case D3DTOP_BLENDTEXTUREALPHAPM			: return PS_BLENDTEXTUREALPHAPM;
	case D3DTOP_BLENDCURRENTALPHA			: return PS_BLENDCURRENTALPHA;
	case D3DTOP_MODULATEALPHA_ADDCOLOR		: return PS_MODULATEALPHA_ADDCOLOR;
	case D3DTOP_MODULATECOLOR_ADDALPHA		: return PS_MODULATECOLOR_ADDALPHA;
	case D3DTOP_MODULATEINVALPHA_ADDCOLOR	: return PS_MODULATEINVALPHA_ADDCOLOR;
	case D3DTOP_MODULATEINVCOLOR_ADDALPHA	: return PS_MODULATEINVCOLOR_ADDALPHA;
	case D3DTOP_DOTPRODUCT3					: return PS_DOTPRODUCT3;
	case D3DTOP_MULTIPLYADD					: return PS_MULTIPLYADD;
	case D3DTOP_LERP						: return PS_LERP;
	case D3DTOP_FORCE_DWORD					: return PS_SHADER;
	}

	return PS_STAGE1;
}
