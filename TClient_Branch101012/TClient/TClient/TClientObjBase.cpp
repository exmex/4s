#include "stdafx.h"
#include "TClientGame.h"
#include "TClientWnd.h"


LPTEXTURESET CTClientObjBase::m_pTSELECT = NULL;
LPTEXTURESET CTClientObjBase::m_pTSHADOW = NULL;

MAPTOBJTEXCOLORPAIR CTClientObjBase::m_mapOBJTEXCOLOR;
MAPTSELECTOBJ CTClientObjBase::m_mapTSELECTDEADOBJ;
MAPTSELECTOBJ CTClientObjBase::m_mapTSELECTOBJ;
VTOBJBASE CTClientObjBase::m_vTGARBAGE;

FLOAT CTClientObjBase::m_fABOFFSET = 0.25f;
FLOAT CTClientObjBase::m_fCamDist = 0.0f;
CFont *CTClientObjBase::m_pFONT = NULL;

D3DXVECTOR3 CTClientObjBase::m_vADJACENCY[ADJACENCY_COUNT];
CT3DVertex CTClientObjBase::m_vTSQUARE;


void CTClientObjBase::InitADJACENCY()
{
	FLOAT fDegree = 360.0f / FLOAT(ADJACENCY_COUNT);

	D3DXVECTOR3 v( 0.0f, 0.0f, 1.0f );
	for( INT i=0 ; i < ADJACENCY_COUNT ; ++i )
	{
		D3DXMATRIXA16 matY;
		D3DXMatrixRotationY( &matY, (fDegree * i) * D3DX_PI / 180.0f );

		D3DXVECTOR3 out;
		D3DXVec3TransformCoord( &out, &v, &matY );
		D3DXVec3Normalize( &out, &out );

		CTClientChar::m_vADJACENCY[ i ] = out;
	}
}

void CTClientObjBase::ClearAdjacencyMon()
{
	for( INT i=0 ; i < ADJACENCY_COUNT ; ++i )
		m_bAdjacencyMon[ i ] = 0;
}

INT CTClientObjBase::FindNearAdjacencyIndex( D3DXVECTOR3& n )
{
	INT b = -1;
	FLOAT td = -1.0f;

	for( INT i=0 ; i < ADJACENCY_COUNT ; ++i )
	{
		FLOAT d = D3DXVec3Dot( &CTClientChar::m_vADJACENCY[ i ], &n );
		if( d > td )
		{
			td = d;
			b = i;
		}
	}

	return b;
}

INT CTClientObjBase::GetAdjacencyIndex( INT index )
{
	if( index >= ADJACENCY_COUNT )
		return index % ADJACENCY_COUNT;
	else if( index < 0 )
	{
		INT n = -index % 16;
		if( n == 0 )
			return 0;
		else
			return ADJACENCY_COUNT - n;
	}
	else
		return index;
}

INT CTClientObjBase::GetSizeGrade()
{
	if( m_fRadius <= 1.0f )
		return 0;

    if( m_fRadius <= 2.0f )
		return 1;

	return 2;
}

void CTClientObjBase::AddAdjacency( CTClientObjBase* pMonster )
{
	LeaveAdjacency( pMonster );

	D3DXVECTOR3 n = pMonster->GetPosition() - this->GetPosition();
	D3DXVec3Normalize( &n, &n );
	INT index = this->FindNearAdjacencyIndex( n );

	if( index != -1 && index < ADJACENCY_COUNT )
	{
		m_bAdjacencyMon[ index ] += 1;
		pMonster->m_nHostAdjacency = index;
	}
	else
	{
		pMonster->m_nHostAdjacency = -1;
	}
}

void CTClientObjBase::LeaveAdjacency( CTClientObjBase* pMONSTER )
{
	if( pMONSTER->m_nHostAdjacency != -1 )
	{
		m_bAdjacencyMon[ pMONSTER->m_nHostAdjacency ] -= 1;

		if( m_bAdjacencyMon[ pMONSTER->m_nHostAdjacency ] < 0 )
			m_bAdjacencyMon[ pMONSTER->m_nHostAdjacency ] = 0;

		pMONSTER->m_nHostAdjacency = -1;
	}
}

CTClientObjBase::CTClientObjBase()
{
	m_bCollisionType = TCOLLISION_NONE;
	m_bNPCType = TNPC_NONE;
	m_bRootID = 0;

	m_vTSKILLTARGET.clear();
	m_vTGARBAGESFX.clear();
	m_vTACTIONSFX.clear();
	m_vTDAMAGE.clear();
	
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

	m_mapTQUESTTEMP.clear();
	m_mapTQUESTCOND.clear();
	ClearMaintain();
	m_vTPRotation.clear();
	m_vTNRotation.clear();
	m_mapTQCLASS.clear();
	m_mapTSKILL.clear();
	m_mapTINVEN.clear();

	m_vHCOLLISION.clear();
	m_vVCOLLISION.clear();

	m_vTDEAD = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vPushDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_fPushHeight = 0.0f;
	m_fPushSpeed = 0.0f;

	m_dwSHADOWTick = 0;
	m_dwAlphaTick = 0;
	m_dwPushTick = 0;
	m_dwTalkTick = 0;

	m_fSpeedFactor = TDEF_SPEED;
	m_fTDEFAULTSPEED = TDEF_SPEED;

	m_fRangeFactor = 1.0f;
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

	m_bDETAILLevel = TDETAIL_COUNT;
	m_bTCORPSMOVE = TCORPSMOVE_NONE;
	m_bDrawMark = TDRAWMARK_DEFAULT;
	m_bHasQuest = THASQUEST_NONE;
	m_bColorID = TNCOLOR_ALLI;
	m_bMode = MT_NORMAL;
	m_bType = OT_NONE;

	m_pTREGION = NULL;
	m_pTRANS = NULL;
	m_pTDBB = NULL;

	m_bCamCollision = FALSE;
	m_bCanSelected = FALSE;
	m_bDynamicOBJ = FALSE;
	m_bDrawGhost = FALSE;
	m_bCanDefend = TRUE;
	m_bDrawName = FALSE;
	m_bDrawTalk = FALSE;

	m_bHideOnCapeMode = FALSE;
	m_bCLEARTARGET = TRUE;
	m_bActionLock = FALSE;
	m_bSubActEnd = FALSE;
	m_bParashute = FALSE;
	m_bInSHADOW = FALSE;
	m_bSTEPSND = FALSE;
	m_bGARBAGE = FALSE;
	m_bVisible = FALSE;
	m_bFPView = FALSE;
	m_bDetect = FALSE;
	m_bGhost = FALSE;
	m_bFlash = FALSE;
	m_bDown = FALSE;
	m_bHide = FALSE;
	m_bClarity = FALSE;
	m_bStandHide = FALSE;
    m_bLastDraw = FALSE;
	m_bHelmetHide = FALSE;
	m_bDisguise = (BYTE)T_INVALID;
	m_bSelectable = TRUE;
	m_bTakenItemAll = FALSE;

	m_bSkillSilence = 0;
	m_wTDETECT = 0;
	m_nSubKeyBase = -1;
	m_dwTSUBACT = 0;

	m_vNAMEPOS.x = (FLOAT)T_INVALID;
	m_vNAMEPOS.y = (FLOAT)T_INVALID;

	m_vTMARKER.InitOBJ(NULL);
	m_fPickWeight = 0.0f;
	EndSLIDE();

	m_nHostAdjacency = -1;
	ClearAdjacencyMon();

	m_bTournamentTeam = TTOURNAMENT_TEAM_NONE;
	m_bDoubleHead = FALSE;

	for( BYTE i=0; i<TNUMBERPOS_COUNT; i++)
		m_vDamageTick[i] = 0;
}

CTClientObjBase::~CTClientObjBase()
{
	MAPTSELECTOBJ::iterator finder = m_mapTSELECTOBJ.find(this);
	if( finder != m_mapTSELECTOBJ.end() )
		m_mapTSELECTOBJ.erase(finder);

	finder = CTClientObjBase::m_mapTSELECTDEADOBJ.find(this);
	if( finder != m_mapTSELECTDEADOBJ.end() )
		m_mapTSELECTDEADOBJ.erase(finder);

	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();
	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();
	m_vTDAMAGE.clear();

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

	RemoveSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	RemoveSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);

	RemoveAllTMagicItemSFX();
	RemoveAllEquipSFX();
	RemoveAllWeaponSFX();

	CTachyonObject::ClearSND(&m_mapTSUBACTSND);
	CTachyonObject::ClearSND(&m_mapTSUBANISND);

	m_vTSUBACT.m_dwCurANI = 0;
	m_vTSUBACT.m_pANI = NULL;
	m_dwTSUBACT = 0;

	m_pTREGION = NULL;
	m_pTRANS = NULL;

	if(m_pTDBB)
	{
		delete m_pTDBB;
		m_pTDBB = NULL;
	}

	ClearSkillTarget();
	ClearMaintain();
	ClearTQClass();
	ClearSkill();
	ClearInven();
	EndSLIDE();

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);
	m_vTMARKER.InitOBJ(NULL);

	m_mapTQUESTTEMP.clear();
	m_mapTQUESTCOND.clear();

	m_bDoubleHead = FALSE;
	m_bTournamentTeam = TTOURNAMENT_TEAM_NONE;
}

void CTClientObjBase::Release()
{
	MAPTSELECTOBJ::iterator finder = m_mapTSELECTOBJ.find(this);

	if( finder != m_mapTSELECTOBJ.end() )
		m_mapTSELECTOBJ.erase(finder);

	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();
	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();

	for( BYTE i=0; i<TNUMBERPOS_COUNT; i++)
		m_vDamageTick[i] = 0;
	m_vTDAMAGE.clear();

	RemoveSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	RemoveSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	RemoveAllTMagicItemSFX();
	RemoveAllEquipSFX();
	RemoveAllWeaponSFX();

	CTachyonObject::ClearSND(&m_mapTSUBACTSND);
	CTachyonObject::ClearSND(&m_mapTSUBANISND);

	m_pTREGION = NULL;
	m_pTRANS = NULL;

	if(m_pTDBB)
	{
		delete m_pTDBB;
		m_pTDBB = NULL;
	}

	ClearSkillTarget();
	ClearMaintain();
	ClearTQClass();
	ClearSkill();
	ClearInven();

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);
	m_vTMARKER.InitOBJ(NULL);

	m_vTDEAD = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_vTSUBACT.m_dwCurANI = 0;
	m_vTSUBACT.m_pANI = NULL;
	m_dwTSUBACT = 0;
	m_bRootID = 0;

	m_mapTQUESTTEMP.clear();
	m_mapTQUESTCOND.clear();

	m_dwActionID = 0;
	m_dwAniID = 0;
	m_bLastDraw = FALSE;
	m_bHelmetHide = FALSE;

	CTachyonObject::Release();

	m_fPickWeight = 0.0f;
	m_nHostAdjacency = -1;
	ClearAdjacencyMon();

	m_bDoubleHead = FALSE;
	m_bTournamentTeam = TTOURNAMENT_TEAM_NONE;
}

void CTClientObjBase::ClearOBJSFX()
{
	CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	CTachyonObject::ClearOBJSFX();
}

void CTClientObjBase::ClearACTSFX()
{
	CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	CTachyonObject::ClearACTSFX();
}

void CTClientObjBase::CalcRender( CD3DDevice *pDevice)
{
	CalcOBJPIVOT(pDevice);
	CalcOBJSFX(pDevice);
}

void CTClientObjBase::CalcOBJSFX( CD3DDevice *pDevice)
{
	MAPTMAINTAIN::iterator itMAINTAIN;
	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
		CalcSFX( pDevice, &(*itMAINTAIN).second->m_vTSFX);

	CalcSFX( pDevice, &m_vTGARBAGESFX);
	CalcSFX( pDevice, &m_vTACTIONSFX);
	CalcSFX( pDevice, &m_vTSUBACTSFX);
	CalcSFX( pDevice, &m_vTSUBANISFX);

	CTachyonObject::CalcOBJSFX(pDevice);
}

void CTClientObjBase::ClearSND()
{
	m_mapTSUBACTSNDINST.clear();
	m_mapTSUBANISNDINST.clear();

	CTachyonObject::ClearSND(&m_mapTSUBACTSND);
	CTachyonObject::ClearSND(&m_mapTSUBANISND);
	CTachyonObject::ClearSND();
}

void CTClientObjBase::ClearSFX()
{
	MAPTMAINTAIN::iterator itMAINTAIN;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
		RemoveTActionSFX(&(*itMAINTAIN).second->m_vTSFX);

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);

	m_mapTSUBACTSFXINST.clear();
	m_mapTSUBANISFXINST.clear();

	RemoveSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
	RemoveSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
	RemoveAllTMagicItemSFX();
	RemoveAllEquipSFX();
	RemoveAllWeaponSFX();

	CTachyonObject::ClearSFX();
}

void CTClientObjBase::ClearSFX( LPVECTOROBJSFX pTSFX, DWORD dwID )
{
	if( pTSFX == NULL )
		return ;

	VECTOROBJSFX::iterator itor = pTSFX->begin();
	while( itor != pTSFX->end() )
	{
		if( (*itor)->m_pSFX->m_dwFuncID == dwID )
		{
			CTachyonSFX::Release( (*itor)->m_dwID );
			delete (*itor);
			pTSFX->erase( itor );

			return ;
		}
		++itor;
	}
}

void CTClientObjBase::ClearAnimationID()
{
	m_OBJ.m_dwCurACT = 0;

	m_dwActionID = 0;
	m_dwAniID = 0;
}

void CTClientObjBase::ClearTQClass()
{
	MAPTQCLASS::iterator it;

	for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
		delete (*it).second;

	m_mapTQCLASS.clear();
}

void CTClientObjBase::ClearSkillTarget()
{
	while(!m_vTSKILLTARGET.empty())
	{
		delete m_vTSKILLTARGET.back();
		m_vTSKILLTARGET.pop_back();
	}
}

void CTClientObjBase::ClearMaintain()
{
	MAPTMAINTAIN::iterator itMAINTAIN;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
	{
		ClearTActionSFX(&(*itMAINTAIN).second->m_vTSFX);
		delete (*itMAINTAIN).second;
	}

	m_mapTMAINTAIN.clear();
	m_mapSendedAddTMAINTAIN.clear();
	m_mapSendedRemoveTMAINTAIN.clear();
	m_vTPRotation.clear();
	m_vTNRotation.clear();
}

void CTClientObjBase::ClearSkill()
{
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_mapTSKILL.begin(); itTSKILL != m_mapTSKILL.end(); itTSKILL++)
		delete (*itTSKILL).second;

	m_mapTSKILL.clear();
}

void CTClientObjBase::ClearInven()
{
	MAPTINVEN::iterator itINVEN;

	for( itINVEN = m_mapTINVEN.begin(); itINVEN != m_mapTINVEN.end(); itINVEN++)
		delete (*itINVEN).second;

	m_mapTINVEN.clear();
}

void CTClientObjBase::ClearMaintainOnDie()
{
	{
		LISTTMAINTAIN::iterator itMAINTAIN;
		itMAINTAIN = m_vTPRotation.begin();

		while( itMAINTAIN != m_vTPRotation.end() )
		{
			if( FALSE == (*itMAINTAIN)->m_pTSKILL->m_bStaticWhenDie )
				itMAINTAIN = m_vTPRotation.erase( itMAINTAIN );
			else
				++itMAINTAIN;
		}
	}

	{
		LISTTMAINTAIN::iterator itMAINTAIN;
		itMAINTAIN = m_vTNRotation.begin();

		while( itMAINTAIN != m_vTNRotation.end() )
		{
			if( FALSE == (*itMAINTAIN)->m_pTSKILL->m_bStaticWhenDie )
				itMAINTAIN = m_vTNRotation.erase( itMAINTAIN );
			else
				++itMAINTAIN;
		}
	}

	{
		MAPTMAINTAIN::iterator itMAINTAIN;
		itMAINTAIN = m_mapTMAINTAIN.begin();

		while( itMAINTAIN != m_mapTMAINTAIN.end() )
		{
			if( FALSE == (*itMAINTAIN).second->m_pTSKILL->m_bStaticWhenDie )
			{
				OnMaintainEnd(
					CTClientGame::GetInstance()->GetDevice(),
					CTClientGame::GetInstance()->GetResource(),
					(*itMAINTAIN).second );

				ClearTActionSFX(&(*itMAINTAIN).second->m_vTSFX);
				delete (*itMAINTAIN).second;
				
				itMAINTAIN = m_mapTMAINTAIN.erase( itMAINTAIN );
			}
			else
			{
				++itMAINTAIN;
			}
		}
	}
}

void CTClientObjBase::InsertMaintain( WORD wID, CTClientMaintain* pTMAINTAIN )
{
	m_mapTMAINTAIN.insert( MAPTMAINTAIN::value_type( wID, pTMAINTAIN ) );

	if( IsPREMIUMMaintain( pTMAINTAIN ) || pTMAINTAIN->m_pTSKILL->m_bCanCancel )
		return ;

	if( pTMAINTAIN->m_pTSKILL->m_bPositive )
	{
		LISTTMAINTAIN::iterator itor = m_vTPRotation.begin();
		LISTTMAINTAIN::iterator end = m_vTPRotation.end();
		while( itor != end )
		{
			if( (*itor)->m_pTSKILL->m_wSkillID == pTMAINTAIN->m_pTSKILL->m_wSkillID )
			{
				(*itor) = pTMAINTAIN;
				return ;
			}

			++itor;
		}

		m_vTPRotation.push_back( pTMAINTAIN );
	}
	else
	{
		LISTTMAINTAIN::iterator itor = m_vTNRotation.begin();
		LISTTMAINTAIN::iterator end = m_vTNRotation.end();
		while( itor != end )
		{
			if( (*itor)->m_pTSKILL->m_wSkillID == pTMAINTAIN->m_pTSKILL->m_wSkillID )
			{
				(*itor) = pTMAINTAIN;
				return ;
			}

			++itor;
		}

		m_vTNRotation.push_back( pTMAINTAIN );
	}
}

void CTClientObjBase::EraseMaintain( WORD wSkillID )
{
	MAPTMAINTAIN::iterator itTMAINTAIN = m_mapTMAINTAIN.find( wSkillID );
	if( itTMAINTAIN == m_mapTMAINTAIN.end() )
		return ;

	CTClientMaintain* pTMAINTAIN = itTMAINTAIN->second;
	if( pTMAINTAIN == NULL )
		return ;

	if( ! IsPREMIUMMaintain( pTMAINTAIN ) &&
		! pTMAINTAIN->m_pTSKILL->m_bCanCancel )
	{
		if( pTMAINTAIN->m_pTSKILL->m_bPositive )
		{
			LISTTMAINTAIN::iterator itor = m_vTPRotation.begin();
			LISTTMAINTAIN::iterator end = m_vTPRotation.end();

			while( itor != end )
			{
				if( (*itor)->m_pTSKILL->m_wSkillID == pTMAINTAIN->m_pTSKILL->m_wSkillID )
				{
					m_vTPRotation.erase( itor );
					break;
				}

				++itor;
			}
		}
		else
		{
			LISTTMAINTAIN::iterator itor = m_vTNRotation.begin();
			LISTTMAINTAIN::iterator end = m_vTNRotation.end();

			while( itor != end )
			{
				if( (*itor)->m_pTSKILL->m_wSkillID == pTMAINTAIN->m_pTSKILL->m_wSkillID )
				{
					m_vTNRotation.erase( itor );
					break;
				}

				++itor;
			}
		}
	}

	delete pTMAINTAIN;
	m_mapTMAINTAIN.erase( itTMAINTAIN );
}

void CTClientObjBase::EraseSendedAddMaintain( WORD wSkillID )
{
	m_mapSendedAddTMAINTAIN.erase( (DWORD) wSkillID );
}

void CTClientObjBase::EraseSendedRemoveMaintain( WORD wSkillID )
{
	m_mapSendedRemoveTMAINTAIN.erase( (DWORD) wSkillID );
}

CTClientMaintain* CTClientObjBase::FindMaintain( WORD wID )
{
	MAPTMAINTAIN::iterator itr = m_mapTMAINTAIN.find( wID );
	if( itr != m_mapTMAINTAIN.end() )
		return itr->second;
	else
		return NULL;
}

void CTClientObjBase::CalcMaintain( DWORD dwTick, BYTE bHandling, CTClientGame *pTGAME)
{
	MAPTMAINTAIN::iterator itMAINTAIN;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
	{
		CTClientMaintain *pTMAINTAIN = (*itMAINTAIN).second;
		DWORD dwPASS = dwTick;

		if(!pTMAINTAIN->m_bUnlimitedType)
		{
			dwPASS = min( pTMAINTAIN->m_dwTick, dwTick);

			if(dwPASS)
				pTMAINTAIN->m_dwTick -= dwPASS;
		}

		if(dwPASS)
		{
			if(pTMAINTAIN->m_pTIMMER)
			{
				pTMAINTAIN->m_dwTIMMER += dwPASS;

				while( pTMAINTAIN->m_dwTIMMER >= TDOT_TICK )
				{
					CTClientObjBase *pTATTACK = pTGAME->FindOBJ(
							pTMAINTAIN->m_dwAttackID,
							pTMAINTAIN->m_bAttackType);

					if((CTClientSkill::IsAttackSKILL(pTMAINTAIN->m_pTIMMER) || !pTATTACK->m_bClarity) &&
						pTGAME->CanDefend( pTATTACK, this, pTMAINTAIN->m_pTIMMER))
					{
						pTGAME->Defend(
							pTATTACK,
							this,
							&pTMAINTAIN->m_vTSKILLDATA,
							&pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND,
							pTMAINTAIN->m_pTIMMER, 0, TRUE );
					}

					pTMAINTAIN->m_dwTIMMER -= TDOT_TICK;
				}
			}

			if( bHandling && !pTMAINTAIN->m_bUnlimitedType && !pTMAINTAIN->m_dwTick )
			{
				pTGAME->SendCS_SKILLEND_REQ(
					m_dwID,
					m_bType,
					pTMAINTAIN->m_dwHostID,
					pTMAINTAIN->m_dwAttackID,
					pTMAINTAIN->m_bAttackType,
					pTMAINTAIN->m_pTSKILL->m_wSkillID,
					pTGAME->GetClientMap()->m_wMapID,
					pTGAME->GetMainWnd()->m_bChannel);
			}
		}
	}
}

void CTClientObjBase::CheckMaintainOBJ( CTClientGame* pTGAME, BOOL bBroadCast )
{
	MAPTMAINTAIN::iterator itTMAINTAIN;
	static VTMAINTAIN vDeleteMaintain;
	vDeleteMaintain.clear();

	for( itTMAINTAIN = m_mapTMAINTAIN.begin(); itTMAINTAIN != m_mapTMAINTAIN.end(); itTMAINTAIN++)
	{
		CTClientMaintain *pTMAINTAIN = (*itTMAINTAIN).second;

		if( pTMAINTAIN &&
			pTMAINTAIN->m_pTSKILL &&
			pTMAINTAIN->m_pTSKILL->m_fBufRange > 0.0f && (
			pTMAINTAIN->m_dwAttackID != m_dwID ||
			pTMAINTAIN->m_bAttackType != m_bType) )
		{
			CTClientObjBase *pTBUF = pTGAME->FindOBJ(
				pTMAINTAIN->m_dwAttackID,
				pTMAINTAIN->m_bAttackType);

			if( !pTBUF || D3DXVec3Length(&D3DXVECTOR3(
				pTBUF->GetPositionX() - GetPositionX(),
				pTBUF->GetPositionY() - GetPositionY(),
				pTBUF->GetPositionZ() - GetPositionZ())) > pTMAINTAIN->m_pTSKILL->m_fBufRange ||
				pTBUF->m_mapTMAINTAIN.find(pTMAINTAIN->m_pTSKILL->m_wSkillID) == pTBUF->m_mapTMAINTAIN.end() )
			{
				vDeleteMaintain.push_back( pTMAINTAIN );
			}
		}
	}

	for( INT i=0 ; i < vDeleteMaintain.size() ; ++i )
	{
		CTClientMaintain *pTMAINTAIN = vDeleteMaintain[ i ];

		if( bBroadCast )
		{
			MAPDWORD::iterator finder = m_mapSendedRemoveTMAINTAIN.find( pTMAINTAIN->m_pTSKILL->m_wSkillID );
			if( finder == m_mapSendedRemoveTMAINTAIN.end() )
			{
				pTGAME->SendCS_SKILLEND_REQ(
					m_dwID,
					m_bType,
					GetHostID(),
					pTMAINTAIN->m_dwAttackID,
					pTMAINTAIN->m_bAttackType,
					pTMAINTAIN->m_pTSKILL->m_wSkillID,
					pTGAME->GetClientMap()->m_wMapID,
					pTGAME->GetMainWnd()->m_bChannel);

				m_mapSendedRemoveTMAINTAIN.insert(
					MAPDWORD::value_type( pTMAINTAIN->m_pTSKILL->m_wSkillID, pTMAINTAIN->m_pTSKILL->m_wSkillID ) );

/*#ifdef TEST_MODE
				CString strFMT;
				strFMT.Format( "Send SkillEnd %s(%d)", GetName(), pTMAINTAIN->m_pTSKILL->m_wSkillID );
				CTClientGame::DebugMSG( strFMT );
#endif*/
			}
		}
		else
		{
			CPacket packet;

			packet.SetID( CS_SKILLEND_ACK )
				<< m_dwID
				<< m_bType
				<< pTMAINTAIN->m_pTSKILL->m_wSkillID;

			packet.Rewind(FALSE);
			pTGAME->OnSessionMsg( pTGAME->GetSession(), &packet );
		}
	}

	for( int i=0; i<INT(pTGAME->m_vTMAINTAINOBJ.size()); i++)
	{
		CTClientMaintain* pTMAINTAINOBJ = pTGAME->m_vTMAINTAINOBJ[ i ];

		if( D3DXVec3Length(&D3DXVECTOR3(
			pTMAINTAINOBJ->m_vTSKILLDATA.m_vTGROUND.x - GetPositionX(),
			pTMAINTAINOBJ->m_vTSKILLDATA.m_vTGROUND.y - GetPositionY(),
			pTMAINTAINOBJ->m_vTSKILLDATA.m_vTGROUND.z - GetPositionZ())) <
			pTMAINTAINOBJ->m_pTSKILL->m_fBufRange && (
			pTMAINTAINOBJ->m_dwAttackID != m_dwID ||
			pTMAINTAINOBJ->m_bAttackType != m_bType ))
		{
			LPTSKILL pTSKILL = pTMAINTAINOBJ->m_pTSKILL;

			if( CTChart::IsHideDetectSkill( pTSKILL ) &&
				CountHideDetectMaintain() )
				continue;

			MAPTMAINTAIN::iterator finder = m_mapTMAINTAIN.find( pTSKILL->m_wSkillID );
			MAPDWORD::iterator finder2 = m_mapSendedAddTMAINTAIN.find( pTSKILL->m_wSkillID );

			if( (bBroadCast && finder == m_mapTMAINTAIN.end() && finder2 == m_mapSendedAddTMAINTAIN.end()) ||
				(!bBroadCast && finder == m_mapTMAINTAIN.end()) )
			{
				CTClientObjBase *pTATTACK = pTGAME->FindOBJ(
					pTMAINTAINOBJ->m_dwAttackID,
					pTMAINTAINOBJ->m_bAttackType);

				if( pTATTACK && pTGAME->CanDefend( pTATTACK, this, pTSKILL) )
				{
					if( pTGAME->Defend( 
						pTATTACK, 
						this, 
						&pTMAINTAINOBJ->m_vTSKILLDATA, 
						&pTMAINTAINOBJ->m_vTSKILLDATA.m_vTGROUND, 
						pTSKILL,
						pTMAINTAINOBJ->m_dwTick,
						bBroadCast ) )
					{
						if( bBroadCast )
						{
							m_mapSendedAddTMAINTAIN.insert(
								MAPDWORD::value_type( pTSKILL->m_wSkillID, pTSKILL->m_wSkillID ) );
						}
					}
				}
			}
		}
	}
}

void CTClientObjBase::BuildMaintainList( CTClientGame* pTGAME )
{
	MAPTMAINTAIN::iterator itTMAINTAIN;

	for( itTMAINTAIN = m_mapTMAINTAIN.begin(); itTMAINTAIN != m_mapTMAINTAIN.end(); itTMAINTAIN++)
	{
		CTClientMaintain *pTMAINTAIN = (*itTMAINTAIN).second;

		if( pTMAINTAIN && pTMAINTAIN->m_pTSKILL && pTMAINTAIN->m_pTSKILL->m_fBufRange > 0.0f &&
			pTMAINTAIN->m_dwAttackID == m_dwID && pTMAINTAIN->m_bAttackType == m_bType )
		{
			pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x = GetPositionX();
			pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y = GetPositionY();
			pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z = GetPositionZ();

			pTGAME->m_vTMAINTAINOBJ.push_back(pTMAINTAIN);
		}
	}
}

void CTClientObjBase::AdjustSDT_STATUS_MOVESPEED()
{
	// CTClientObjBase *pTOBJ 자기자신
	// pTOBJ->m_mapTMAINTAIN을 돌려면 스킬의 Function이 SDT_STATUS_MOVESPEED인 것들을 찾아서
	// 그것들 중에 가장 값이 낮은(속도가 가장 낮은) 애를 현재 pTOBJ의 스피드로 지정한다.

	if( IsDead() )
		return;

	m_fSpeedFactor = m_fTDEFAULTSPEED;
	BOOL bInitSpeedFactor = FALSE;

	MAPTMAINTAIN::iterator itMAINTAIN;
	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
	{
		LPTSKILL pTSKILL = (*itMAINTAIN).second->m_pTSKILL;
		BYTE bLevel = (*itMAINTAIN).second->m_bLevel;

		for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[i];

			if( pTSKILLFUNC->m_bType == SDT_STATUS &&
				pTSKILLFUNC->m_bFunctionID == SDT_STATUS_MOVESPEED )
			{
				DWORD dwValue = CTClientSkill::CalcTSKILLDATA(
										pTSKILL,
										i, bLevel);

				FLOAT fSpeedFactor = TDEF_SPEED;

				switch(pTSKILLFUNC->m_bOPCode)
				{
				case SVI_INCREASE	: fSpeedFactor = m_fTDEFAULTSPEED + FLOAT(dwValue) / 100.0f; break;
				default				: fSpeedFactor = m_fTDEFAULTSPEED - FLOAT(dwValue) / 100.0f; break;
				}

				if( bInitSpeedFactor )
				{
					m_fSpeedFactor = m_fSpeedFactor > fSpeedFactor ? fSpeedFactor : m_fSpeedFactor;
				}
				else
				{
					m_fSpeedFactor = fSpeedFactor;
					bInitSpeedFactor = TRUE;
				}
			}
		}
	}
}

void CTClientObjBase::DoSkillFunc( LPTSKILL pTSKILL,
								WORD wPursuit,
								BYTE bLevel,
								LPD3DXVECTOR3 pAtkPos,
								LPD3DXVECTOR3 pDefPos,
								BOOL bApplyMaintain,
								CTClientGame* pTGAME )
{
	// CTClientObjBase *pDEFEND 자기자신

	if( IsDead() )
		return;

	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
	{
		LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[i];
		if( !bApplyMaintain && pTSKILLFUNC->m_bMethodID == SA_BUFF )
			continue;

		switch(pTSKILLFUNC->m_bType)
		{
		case SDT_STATUS		:
			{
				DWORD dwValue = CTClientSkill::CalcTSKILLDATA(
					pTSKILL,
					i, bLevel);

				switch(pTSKILLFUNC->m_bFunctionID)
				{
				case SDT_STATUS_SLIDE		:
					if( m_bType == OT_SELF ) // 의도:포탑은 밀리지 않는다.
						break;

					if( m_bType == OT_MON ) // 넉백금지된 몬스터는 넉백 통과.
					{
						CTClientMonster* mon = (CTClientMonster*) this;
						if( mon->m_pMON && mon->m_pMON->m_bNotNockBack )
							continue;
					}

					if( pAtkPos && pDefPos )
					{
						if( this != pTGAME->GetMainChar() )
						{
							D3DXVECTOR3 vMOVE(
								pDefPos->x - GetPositionX(),
								pDefPos->y - GetPositionY(),
								pDefPos->z - GetPositionZ());

							if( D3DXVec3Length(&vMOVE) > FIX_DIST )
							{
								SetPositionX( GetPositionX() + vMOVE.x );
								SetPositionY( GetPositionY() + vMOVE.y );
								SetPositionZ( GetPositionZ() + vMOVE.z );

								m_vWorld._41 += vMOVE.x;
								m_vWorld._42 += vMOVE.y;
								m_vWorld._43 += vMOVE.z;
							}
						}

						CalcDIR(
							m_wMoveDIR,
							GetTargetDIR(
							pAtkPos->x - pDefPos->x,
							pAtkPos->z - pDefPos->z),
							WORD(0));

						if( CTChart::GetTACTION(m_bAction)->m_bLevel <= CTChart::GetTACTION(TA_PUSH)->m_bLevel )
						{
							FinishSUBACT();
							FinishACT();
						}

						Push(
							FLOAT(dwValue) / 200.0f,
							&D3DXVECTOR2(
							pDefPos->x - pAtkPos->x,
							pDefPos->z - pAtkPos->z));
					}

					break;

				case SDT_STATUS_ATKRANGE	:
					switch(pTSKILLFUNC->m_bOPCode)
					{
					case SVI_INCREASE	: m_fRangeFactor = 1.0f + FLOAT(dwValue) / 100.0f; break;
					default				: m_fRangeFactor = 1.0f - FLOAT(dwValue) / 100.0f; break;
					}

					break;

				case SDT_STATUS_HYPNOSIS	:
				case SDT_STATUS_BLOCK		:
					{
						m_bLastACT = TA_STUN;
						m_bIsStuned = TRUE;

						if( CTChart::GetTACTION(m_bAction)->m_bLevel <= CTChart::GetTACTION(TA_STUN)->m_bLevel )
						{
							m_bACTLevel = CTChart::GetTACTION(TA_STUN)->m_bLevel;
							m_bAction = TA_STUN;

							FinishSUBACT();
							FinishACT();
						}
					}

					break;

				case SDT_STATUS_HOLD		:
					{
						m_bLastACT = TA_HOLD;
						m_bIsHolded = TRUE;

						if( CTChart::GetTACTION(m_bAction)->m_bLevel <= CTChart::GetTACTION(TA_HOLD)->m_bLevel )
						{
							m_bACTLevel = CTChart::GetTACTION(TA_HOLD)->m_bLevel;
							m_bAction = TA_HOLD;

							FinishSUBACT();
							FinishACT();
						}
					}

					break;

				case SDT_STATUS_SEEKMINE	: m_wTDETECT |= (1 << OT_SELF); break;
				case SDT_STATUS_HIDE		:
					switch(pTSKILLFUNC->m_bOPCode)
					{
					case SVI_DECREASE	: m_wTDETECT |= (1 << OT_PC); break;
					case SVI_INCREASE	: SetHide(TRUE); break;
					}

					break;

				case SDT_STATUS_HIDEDETECT:
					{
						m_wTDETECT |= (1 << OT_PC);
					}
					break;

				case SDT_STATUS_CLARITY		: 
					{
						m_bClarity = TRUE;  
						SetHide(TRUE);
					}
					break;

				case SDT_STATUS_STANDHIDE	: m_bStandHide = TRUE; break;
				case SDT_STATUS_SILENCE		:
					if( this == pTGAME->GetMainChar() )
						pTGAME->GetMainChar()->m_bSkillSilence = TRUE;
					break;

				case SDT_STATUS_DARK		:
				case SDT_STATUS_FLASH		:
					{
						m_bFlash = TRUE;
						
						if( pTSKILLFUNC->m_bFunctionID == SDT_STATUS_DARK )
							m_dwFlashColor = TFLASHCOLOR_BLACK;
						else
							m_dwFlashColor = TFLASHCOLOR_WHITE;

						if( this == pTGAME->GetMainChar() )
						{
							CTClientObjBase::m_mapTSELECTOBJ.clear();
							pTGAME->ResetTargetOBJ(NULL);
							pTGAME->ResetTargetINFO(FALSE);
						}
					}

					break;

				case SDT_STATUS_PARACHUTE	: m_bParashute = TRUE; break;
				case SDT_STATUS_BATTLE		: m_bInBattle = TRUE; break;
				case SDT_STATUS_MERMAID		: m_bMermaid = TRUE; break;
				case SDT_STATUS_MARK		: m_wPursuit = wPursuit; break;
				
				case SDT_STATUS_NOTSELECT	: 
					{
						m_bSelectable = FALSE; 
						if( this == pTGAME->GetTargetObj() )
						{
							pTGAME->ResetTargetOBJ(NULL);
							pTGAME->ResetTargetINFO(FALSE);

							pTGAME->LoopSkillOFF();
						}

						break;
					}

				case SDT_STATUS_DOUBLEHEAD:
					{
						m_bDoubleHead = TRUE;
						SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f );
						break;
					}
				}
			}
			break;

		case SDT_TRANS		:
			switch(pTSKILLFUNC->m_bFunctionID)
			{
				case SDT_TRANS_DISGUISE_D	: DoDISGUISE( pTGAME->GetDevice(), pTGAME->GetResource(), TCONTRY_D); break;
				case SDT_TRANS_DISGUISE_C	: DoDISGUISE( pTGAME->GetDevice(), pTGAME->GetResource(), TCONTRY_C); break;
				case SDT_TRANS_DISGUISE_B	: DoDISGUISE( pTGAME->GetDevice(), pTGAME->GetResource(), TCONTRY_B); break;
				default						:
					{
						DoTRANS(
							pTGAME->GetDevice(),
							pTGAME->GetResource(),
							pTSKILLFUNC->m_wValue);
					}
					break;
			}
			break;

		case SDT_AI			:
			switch(pTSKILLFUNC->m_bFunctionID)
			{
			case SDT_RUNAWAY				:
				if( this == pTGAME->GetMainChar() )
					pTGAME->DoMainRunAway(TRUE);
				else if( this == pTGAME->GetMainRecall() )
					pTGAME->DoRecallRunAway( (CTClientRecall *) this, TRUE );
				break;
			}
			break;
		}
	}

	WORD wSquadID = pTGAME->GetPartyID( pTGAME->GetMainChar() );
	if( wSquadID )
		if( pTGAME->m_pTRSCS->FindTENEMY( m_dwID, m_bType) &&
			!pTGAME->IsRSCSEnemy( this ) )
		{
			pTGAME->RelaySession()->SendCS_DELCORPSENEMY_REQ(
				pTGAME->GetMainChar()->m_dwID,
				m_dwID,
				m_bType);
		}
		else if( pTGAME->IsHostingEnemy( this ) )
			m_bTCORPSMOVE = TCORPSMOVE_ENEMY;
		else if( this == pTGAME->GetMainChar() )
			pTGAME->GetMainChar()->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
}

void CTClientObjBase::DoSkillFuncAll( CTClientGame* pTGAME )
{
	MAPTMAINTAIN::iterator itMAINTAIN;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
	{
		DoSkillFunc(
			(*itMAINTAIN).second->m_pTSKILL,
			(*itMAINTAIN).second->m_vTSKILLDATA.m_wAL,
			(*itMAINTAIN).second->m_bLevel,
			NULL,
			NULL,
			TRUE,
			pTGAME );
	}

	AdjustSDT_STATUS_MOVESPEED();
}

bool CTClientObjBase::VerifySkillFunc()
{
	if( IsDead() || m_bGhost )
		return true;

	BYTE bHide = FALSE;
	BYTE bClarity = FALSE;
	BYTE bStandHide = FALSE;
	BYTE bParashute = FALSE;

	for( MAPTMAINTAIN::iterator itMAINTAIN = m_mapTMAINTAIN.begin() ; itMAINTAIN != m_mapTMAINTAIN.end() ; itMAINTAIN++)
	{
		LPTSKILL pTSKILL = (*itMAINTAIN).second->m_pTSKILL;
		WORD wPursuit = (*itMAINTAIN).second->m_vTSKILLDATA.m_wAL;
		BYTE bLevel = (*itMAINTAIN).second->m_bLevel;

		for( size_t i=0 ; i < pTSKILL->m_vTFUNCTION.size() ; ++i )
		{
			LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[ i ];
			switch(pTSKILLFUNC->m_bType)
			{
			case SDT_STATUS:
				{
					DWORD dwValue = CTClientSkill::CalcTSKILLDATA( pTSKILL, i, bLevel );
					switch( pTSKILLFUNC->m_bFunctionID )
					{
					case SDT_STATUS_HIDE:
						switch(pTSKILLFUNC->m_bOPCode)
						{
						case SVI_INCREASE	: bHide = TRUE; break;
						}
						break;

					case SDT_STATUS_CLARITY: 
						{
							bClarity = TRUE;  
							bHide = TRUE;
						}
						break;

					case SDT_STATUS_STANDHIDE: bStandHide = TRUE; break;
					case SDT_STATUS_PARACHUTE: bParashute = TRUE; break;
					}
					break;
				}
				break;
			}
		}
	}

	if( bHide != m_bHide ||
		bClarity != m_bClarity ||
		bStandHide != m_bStandHide ||
		bParashute != m_bParashute )
	{
		return false;
	}

	FLOAT fSPEEDFACTOR = m_fTDEFAULTSPEED;
	BOOL bInitSpeedFactor = FALSE;
	for( MAPTMAINTAIN::iterator itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
	{
		LPTSKILL pTSKILL = (*itMAINTAIN).second->m_pTSKILL;
		BYTE bLevel = (*itMAINTAIN).second->m_bLevel;

		for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[i];

			if( pTSKILLFUNC->m_bType == SDT_STATUS &&
				pTSKILLFUNC->m_bFunctionID == SDT_STATUS_MOVESPEED )
			{
				DWORD dwValue = CTClientSkill::CalcTSKILLDATA( pTSKILL, i, bLevel );
				FLOAT fSpeedFactor = TDEF_SPEED;

				switch(pTSKILLFUNC->m_bOPCode)
				{
				case SVI_INCREASE	: fSpeedFactor = m_fTDEFAULTSPEED + FLOAT(dwValue) / 100.0f; break;
				default				: fSpeedFactor = m_fTDEFAULTSPEED - FLOAT(dwValue) / 100.0f; break;
				}

				if( bInitSpeedFactor )
				{
					fSPEEDFACTOR = fSPEEDFACTOR > fSpeedFactor ? fSpeedFactor : fSPEEDFACTOR;
				}
				else
				{
					fSPEEDFACTOR = fSpeedFactor;
					bInitSpeedFactor = TRUE;
				}
			}
		}
	}

	if( fSPEEDFACTOR == m_fSpeedFactor )
		return true;
    
	if( (fSPEEDFACTOR > m_fSpeedFactor && fSPEEDFACTOR - m_fSpeedFactor > 0.01f) ||
		(m_fSpeedFactor > fSPEEDFACTOR && m_fSpeedFactor - fSPEEDFACTOR > 0.01f) )
		return false;

	return true;
}

CTClientQClass *CTClientObjBase::FindTQClass( DWORD dwClassID)
{
	MAPTQCLASS::iterator finder = m_mapTQCLASS.find(dwClassID);

	if( finder != m_mapTQCLASS.end() )
		return (*finder).second;

	return NULL;
}

CTClientQuest *CTClientObjBase::FindTQuest( DWORD dwQuestID)
{
	MAPTQCLASS::iterator it;

	for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
	{
		CTClientQuest *pQuest = (*it).second->FindTQuest(dwQuestID);

		if(pQuest)
			return pQuest;
	}

	return NULL;
}

CTClientSkill *CTClientObjBase::FindTSkill( WORD wSkillID)
{
	MAPTSKILL::iterator finder = m_mapTSKILL.find(wSkillID);

	if( finder != m_mapTSKILL.end() )
		return (*finder).second;

	return NULL;
}

CTClientInven *CTClientObjBase::FindTInven( BYTE bInvenID)
{
	MAPTINVEN::iterator finder = m_mapTINVEN.find(bInvenID);

	if( finder != m_mapTINVEN.end() )
		return (*finder).second;

	return NULL;
}

WORD CTClientObjBase::GetItemCount( WORD wItemID)
{
	MAPTINVEN::iterator it;
	WORD wCount = 0;

	for( it = m_mapTINVEN.begin(); it != m_mapTINVEN.end(); it++)
	{
		MAPTITEM::iterator itTITEM;

		for( itTITEM = (*it).second->m_mapTITEM.begin(); itTITEM != (*it).second->m_mapTITEM.end(); itTITEM++)
			if( (*itTITEM).second->GetItemID() == wItemID )
				wCount += (*itTITEM).second->GetCount();
	}

	return wCount;
}

void CTClientObjBase::ReleaseData()
{
	m_bEquipMode = TEM_NORMAL;
	m_bAction = TA_STAND;
	m_bACTLevel = 0;

	m_bLastACT = TA_STAND;
	m_bIsHolded = FALSE;
	m_bIsStuned = FALSE;

	m_bTCORPSMOVE = TCORPSMOVE_NONE;
	m_bDrawMark = TDRAWMARK_DEFAULT;
	m_bHasQuest = THASQUEST_NONE;
	m_bInBattle = FALSE;
	m_bPrevFLY = FALSE;
	m_bMermaid = FALSE;

	m_bHideOnCapeMode = FALSE;
	m_bCLEARTARGET = TRUE;
	m_bActionLock = FALSE;
	m_bSubActEnd = FALSE;
	m_bParashute = FALSE;
	m_bDrawGhost = FALSE;
	m_bHouseMesh = FALSE;
	m_bForceDraw = FALSE;
	m_bInSHADOW = FALSE;
	m_bSTEPSND = FALSE;
	m_bVisible = FALSE;
	m_bFPView = FALSE;
	m_bDetect = FALSE;
	m_bShoot = FALSE;
	m_bGhost = FALSE;
	m_bFlash = FALSE;
	m_bDown = FALSE;
	m_bLand = FALSE;
	m_bSwim = FALSE;
	m_bHide = FALSE;
	m_bClarity = FALSE;
	m_bStandHide = FALSE;
	m_bTakenItemAll = FALSE;

	m_bDisguise = (BYTE)T_INVALID;

	m_pTREGION = NULL;
	m_pTRANS = NULL;

	SetSubAction( 0, 0);
	m_dwActionID = 0;
	m_dwAniID = 0;
	m_dwID = 0;

	m_dwMaxHP = 0;
	m_dwMaxMP = 0;

	m_fSpeedFactor = TDEF_SPEED;
	m_fTDEFAULTSPEED = TDEF_SPEED;

	m_fBreathHeight = 0.0f;
	m_fSquareHeight = 0.0f;
	m_fRangeFactor = 1.0f;
	m_fCamDIST = 0.0f;
	m_fRadiusH = 0.0f;
	m_fRadius = 0.0f;
	m_fZValue = 0.0f;
	m_fRange = 0.0f;
	m_fSight = 0.0f;

	m_bAllianceID = TCONTRY_N;
	m_bContryID = TCONTRY_D;
	m_bColorID = TNCOLOR_ALLI;

	m_wMoveDIR = MOVE_NONE;
	m_wPursuit = 0xFFFF;
	m_wTDETECT = 0;
	m_wPITCH = 0;
	m_wDIR = 0;
	m_bSkillSilence = 0;

	for( BYTE i=0; i<TWEAPON_COUNT; i++)
		m_vWeaponPOS[i] = 0xFFFFFFFF;

	for( i=0; i<TAD_COUNT; i++)
	{
		m_dwAttackDelay[i] = 0;
		m_dwAttackDelayRate[i] = 0;
	}

	m_bChargeSpeed = 0;
	m_bClassID = 0;
	m_bRootID = 0;
	m_bLevel = 0;

	m_dwDropDamage = 0;
	m_dwHouseID = 0;
	m_dwInstID = 0;
	m_dwHP = 0;
	m_dwMP = 0;

	ClearSkillTarget();
	ClearMaintain();
	ClearTQClass();
	ClearSkill();
	ClearInven();

	RemoveTActionSFX(&m_vTGARBAGESFX);
	RemoveTActionSFX(&m_vTACTIONSFX);

	m_vPushDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_vJumpDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_vFallDIR = D3DXVECTOR2( 0.0f, 0.0f);

	m_dwPushTick = 0;
	m_dwJumpTick = 0;
	m_dwFallTick = 0;

	m_fPushHeight = 0.0f;
	m_fFallHeight = 0.0f;
	m_fJumpHeight = 0.0f;
	m_fMoveSpeed = 0.0f;
	m_fPushSpeed = 0.0f;
	m_fJumpSpeed = 0.0f;

	m_dwSHADOWTick = 0;
	m_dwAlphaTick = 0;

	m_mapTQUESTTEMP.clear();
	m_mapTQUESTCOND.clear();

	m_vTMARKER.InitOBJ(NULL);

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

	for( i=0; i<TNUMBERPOS_COUNT; i++)
		m_vDamageTick[i] = 0;
	m_vTDAMAGE.clear();

	m_vTALK.SetFont(CTClientTalkBox::m_pFONT);
	m_vNAME.SetFont(m_pFONT);
	m_vNAME.m_bOutLine = TRUE;

	m_vUSERTITLE.SetFont(m_pFONT);
	m_vUSERTITLE.m_bOutLine = TRUE;
	m_vUSERTITLE.m_dwColor = 0xFFECDB90;

	EndSLIDE();

	m_bSelectable = TRUE;
	m_nHostAdjacency = -1;
	ClearAdjacencyMon();

	m_bDoubleHead = FALSE;
	m_bTournamentTeam = TTOURNAMENT_TEAM_NONE;
}

void CTClientObjBase::InitCollision()
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
								D3DXMATRIX vTrans = m_vPosition;

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

void CTClientObjBase::PushCollisionPoly( LPD3DXVECTOR3 pPOLY,
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

	if( fLength != 0.0f && fabs(vPLANE.b) / fLength <= CTClientMAP::m_fMoveBound )
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

void CTClientObjBase::InitSIZE( FLOAT fSizeX,
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

	if(m_bCanSelected)
	{
		m_fRange = TSELCULL_FACTOR * m_fCamDist;
		m_bDETAILLevel = TDETAIL_LOW;
	}
	else if( m_fRange < TLOWCULL_BOUND )
	{
		m_bDETAILLevel = m_bCollisionType == TCOLLISION_NONE ? TDETAIL_HI : TDETAIL_LOW;
		m_fRange = TLOWCULL_FACTOR * m_fCamDist;
	}
	else if( m_fRange < TMIDCULL_BOUND )
	{
		m_bDETAILLevel = m_bCollisionType == TCOLLISION_NONE ? TDETAIL_MED : TDETAIL_LOW;
		m_fRange = TMIDCULL_FACTOR * m_fCamDist;
	}
	else if( m_fRange < THICULL_BOUND )
	{
		m_fRange = THICULL_FACTOR * m_fCamDist;
		m_bDETAILLevel = TDETAIL_LOW;
	}
	else
	{
		m_fRange = TNOCULL_FACTOR * m_fCamDist;
		m_bDETAILLevel = TDETAIL_LOW;
	}
}

void CTClientObjBase::ResetTQUESTMARKER( CTachyonRes *pRES)
{
	if( !(m_bDrawMark & (1 << TDRAWMARK_QUEST)))
		return ;

	if( m_bNPCType == TNPC_CRAFT )
		return ;

	LPOBJECT pOBJ = NULL;

	switch(m_bHasQuest)
	{
	case THASQUEST_COMPLETE		:
		pOBJ = pRES->GetOBJ(ID_MARK_QUESTION); 
		break;
	
	case THASQUEST_COMPLETE_X	:
		pOBJ = pRES->GetOBJ(ID_MARK_QUESTION_GRAY); 
		break;

	case THASQUEST_MISSION_X	:
		pOBJ = pRES->GetOBJ(ID_MARK_EXCLAM_GRAY);
		break;
	
	case THASQUEST_ROUTING		:
		pOBJ = pRES->GetOBJ(ID_MARK_BOX); 
		break;

	case THASQUEST_MISSION		: 
		pOBJ = pRES->GetOBJ(ID_MARK_EXCLAM); 
		break;
	}

	if( pOBJ != m_vTMARKER.m_OBJ.m_pOBJ )
	{
		m_vTMARKER.InitOBJ(pOBJ);
		m_vTMARKER.m_bSFX = TRUE;
	}
}

void CTClientObjBase::ResetWeaponPOS(
	DWORD dwHideEquipSlot,
	CD3DDevice *pDevice,
	CTachyonRes *pRES)
{
	static DWORD dwTSLASH[TSLASHSFX_COUNT][2] = {
		{ ID_RHAND, ES_PRMWEAPON},
		{ ID_LHAND, ES_SNDWEAPON}};

	static BYTE vWEAPON[TWEAPON_COUNT] = {
		ES_PRMWEAPON,
		ES_SNDWEAPON,
		ES_LONGWEAPON};

	static DWORD dwSLOT[TWEAPON_COUNT] = {
		(1 << ES_PRMWEAPON),
		(1 << ES_SNDWEAPON),
		(1 << ES_LONGWEAPON)};

	CTClientInven *pTEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);
	CTachyonObject *pTOBJ[TWEAPON_COUNT] = { NULL, };
	CTClientItem *pTITEM[TWEAPON_COUNT] = { NULL, };
	CTClientItem *pApplyGradeList[TWEAPON_COUNT] = { NULL, };

	BYTE bCapeMode = pTEQUIP && pTEQUIP->FindTItem(ES_BACK) ? TRUE : FALSE;
	BYTE pTApplyGradeMode[TWEAPON_COUNT] = { 0, };

	RemoveAllTMagicItemSFX();
	RemoveAllWeaponSFX();

	for( BYTE i=0; i<TWEAPON_COUNT; i++)
	{
		if( m_bEquipMode == TEM_NORMAL &&
			(dwSLOT[ i ] & dwHideEquipSlot) == dwSLOT[ i ] )
		{
			pTOBJ[i] = NULL;
			pTITEM[i] = NULL;
			m_vWeaponPOS[i] = 0xFFFFFFFF;
			continue;
		}

		MAPOBJECT::iterator finder = m_mapEQUIP.find(m_vWeaponPOS[i]);
		pTITEM[i] = pTEQUIP ? pTEQUIP->FindTItem(vWEAPON[i]) : NULL;

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
			m_bEquipMode);
		BYTE bHideWeapon = FALSE;

		if( pTITEM[i] && pTITEM[i]->GetTITEM() )
		{
			LPTITEMVISUAL pTVISUAL = pTITEM[i]->GetVisual();

			switch(pTITEM[i]->GetTITEM()->m_bKind)
			{
			case IK_CAKRAM	:
				if( m_bHideOnCapeMode || m_bEquipMode == TEM_LONG )
					bHideWeapon = TRUE;

				break;

			case IK_SHIELD	:
			case IK_MSTICK	:
			case IK_2HAND	:
			case IK_AX		:
				if(bCapeMode)
				{
					if( m_bHideOnCapeMode || m_bEquipMode == TEM_LONG )
						bHideWeapon = TRUE;
					bMode = MT_BATTLE;
				}

				break;
			case IK_BACK	:
				{
					bMode = MT_NORMAL;

					if( !pTITEM[i]->GetTITEM()->m_bCanColor && pTITEM[i]->GetRegGuild() )
						bMode = MT_BATTLE;
				}
				break;
			}

			if(pTVISUAL)
			{
				DWORD dwMeshID = bHideWeapon ? 0x00000000 : pTVISUAL->m_dwMesh[bMode];

				if(pTVISUAL->m_dwObjectID)
				{
					MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(pTVISUAL->m_dwPivot[bMode]);

					if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() && dwMeshID )
					{
						m_vWeaponPOS[i] = (*itID).second + 1;

						if(!pTOBJ[i])
						{
							pTOBJ[i] = new CTachyonObject();

							pTOBJ[i]->InitOBJ(pRES->GetOBJ(pTVISUAL->m_dwObjectID));
							
							pTOBJ[i]->SetCloth(
								pDevice,
								pTVISUAL->m_dwCLKID,
								pTVISUAL->m_dwCLIID,
								dwMeshID);

							pTOBJ[i]->ResetOBJPart(pDevice);
							ApplyGradeEffect(pTOBJ[i], pTITEM[i]);
						}

						D3DXMatrixIdentity(&pTOBJ[i]->m_vWorld);

						VECTORDWORD vItemSFX = pTITEM[i]->GetSFX();
						for(size_t j=0; j<vItemSFX.size(); ++j)
							PlayTMagicItemSFX(pTOBJ[i], vItemSFX[j]);

						m_mapEQUIP.insert( MAPOBJECT::value_type( m_vWeaponPOS[i], pTOBJ[i]));
						m_mapEQUIPInfo.insert( TMAPWEAPONEQUIPINFO::value_type(m_vWeaponPOS[i], TWEAPONEQUIPINFO(bMode,pTVISUAL)) );

						if( pTVISUAL->m_dwEffectFuncID[bMode] != 0 )
						{
							pTOBJ[i]->GenerateSFX(
								&pTOBJ[i]->m_mapOBJSFXINST,
								&pTOBJ[i]->m_vOBJSFX,
								&pTOBJ[i]->m_mapOBJSFX,
								0.0f,
								pTVISUAL->m_dwEffectFuncID[bMode] );
						}
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
						dwMeshID);

					pApplyGradeList[i] = pTITEM[i];
					pTApplyGradeMode[i] = bMode;
				}
			}
		}
		else if(pTOBJ[i])
		{
			delete pTOBJ[i];
			pTOBJ[i] = NULL;
		}
	}

	ResetOBJPart(pDevice);
	ApplyGradeEffect(pApplyGradeList, TWEAPON_COUNT);

	for( INT i=0 ; i < TWEAPON_COUNT ; ++i )
	{
		if( !pApplyGradeList[i] )
			continue;

		BYTE bMode = pTApplyGradeMode[i];
		DWORD dwFuncID = pApplyGradeList[ i ]->GetVisual()->m_dwEffectFuncID[bMode];

		if( dwFuncID != 0 )
		{
			GenerateSFX(
				&m_mapOBJSFXINST,
				&m_vOBJSFX,
				&m_mapOBJSFX,
				0.0f,
				dwFuncID );

			m_vEquipSFXFuncID.push_back( dwFuncID );
		}
	}

	for( i=0; i<TSLASHSFX_COUNT; i++)
	{
		BYTE bResetKEY = TRUE;

		if(pTEQUIP)
		{
			MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwTSLASH[i][0]);

			if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
			{
				CTClientItem *pTWEAPON = pTEQUIP->FindTItem(BYTE(dwTSLASH[i][1]));

				if( pTWEAPON )
				{
					LPTITEMVISUAL pTVISUAL = pTWEAPON->GetVisual();
					if( pTVISUAL )
					{
						MAPRES::iterator itTEX = pRES->m_mapTEX.find(pTVISUAL->m_dwSlashTex);
						LPTEXTURESET pTEX = NULL;

						if( itTEX != pRES->m_mapTEX.end() )
							pTEX = (LPTEXTURESET) (*itTEX).second;

						if( m_vTSLASH[i].m_pTEX != pTEX ||
							m_vTSLASH[i].m_dwCOLOR != pTVISUAL->m_dwSlashColor ||
							m_vTSLASH[i].m_vLENGTH._11 != pTVISUAL->m_fSlashLen )
						{
							m_vTSLASH[i].InitSFX(
								&m_pBone[(*itID).second + 1],
								pTEX,
								pTVISUAL->m_dwSlashColor,
								pTVISUAL->m_fSlashLen,
								TSLASH_INTER);
						}
						else
							m_vTSLASH[i].m_pPIVOT = &m_pBone[(*itID).second + 1];
					
						bResetKEY = FALSE;
					}
				}
			}
		}

		if(bResetKEY)
			m_vTSLASH[i].ResetKEY();
	}
}

void CTClientObjBase::ResetEQUIP( CD3DDevice *pDevice,
								  CTachyonRes *pRES)
{
	static BYTE vREPLACE[TREPLACE_COUNT] = {
		ES_HEAD,
		ES_BACK,
		ES_BODY,
		ES_PANTS,
		ES_FOOT,
		ES_HAND,
		ES_FACE,
		ES_COSTUME_HAT,
		ES_COSTUME_DRESS,
		ES_COSTUME_ACCESSORY,
		ES_COSTUME_RACE
	};

	static DWORD vCOSTUMEHIDE[TREPLACE_COUNT] = {
		(1 << ES_HEAD), // 8
		(1 << ES_BACK), // 16
		(1 << ES_BODY), // 32
		(1 << ES_PANTS), // 64
		(1 << ES_FOOT), // 128
		(1 << ES_HAND), // 256
		(1 << ES_FACE), // 16384
		(1 << ES_COSTUME_HAT),
		(1 << ES_COSTUME_DRESS),
		(1 << ES_COSTUME_ACCESSORY),
		(1 << ES_COSTUME_RACE),
	};

	static DWORD vCLEAR[TCLEAR_COUNT] = {
		ID_CLK_RHAND_WEAPON,
		ID_CLK_LHAND_WEAPON,
		ID_CLK_LONG_WEAPON,
		ID_CLK_HEAD,
		ID_CLK_BACK};

	RemoveAllEquipSFX();

	CTClientInven *pTEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);

	BYTE i;
	for( i=0; i<TCLEAR_COUNT; i++)
		SetCloth( pDevice, vCLEAR[i], 0, 0);

	for( i=0; i<TWEAPON_COUNT; i++)
		m_vWeaponPOS[i] = 0xFFFFFFFF;

	ClearEquip();

	CTClientItem *pReplaceItem[TREPLACE_COUNT] = { NULL, };
	CTClientItem *pTApplyGrade[TREPLACE_COUNT] = { NULL, };
	BYTE pTApplyGradeMode[TREPLACE_COUNT] = { 0, };
	BOOL bCostumeHide[TREPLACE_COUNT] = { FALSE, };

	if( pTEQUIP )
		for( i=0 ; i < TREPLACE_COUNT ; ++i )
			pReplaceItem[ i ] = pTEQUIP->FindTItem( vREPLACE[i] );

	DWORD dwCostumeHide = 0;
	if( !IsDisguise() )
	{
		for( i=7 ; i < 11 ; ++i )
			if( pReplaceItem[ i ] )
			{
				LPTITEMVISUAL pTCostumeVISUAL = pReplaceItem[ i ]->GetVisual();
				if( pTCostumeVISUAL )
					dwCostumeHide |= pTCostumeVISUAL->m_dwCostumeHide;
			}

		for( i=0 ; i < TREPLACE_COUNT ; ++i )
			if( (dwCostumeHide & vCOSTUMEHIDE[ i ]) == vCOSTUMEHIDE[ i ])
			{
				pReplaceItem[ i ] = NULL;
				bCostumeHide[ i ] = TRUE;
			}
	}

	for( i=0; i<TREPLACE_COUNT; i++)
	{
		BOOL bUseDefault = TRUE;

		if( !IsDisguise() &&
			(!m_bHelmetHide || (vREPLACE[i] != ES_HEAD && vREPLACE[i] != ES_FACE)) )
		{
			CTClientItem *pTITEM = pReplaceItem[ i ];

			if( pTITEM )
			{
				LPTITEMVISUAL pTVISUAL =  pTITEM->GetVisual();
				if( pTVISUAL )
				{
					BYTE bMode = GetEquipModeIndex( vREPLACE[i], m_bEquipMode);

					if( vREPLACE[i] == ES_BACK && !pTITEM->GetTITEM()->m_bCanColor && pTITEM->GetRegGuild() )
						bMode = MT_BATTLE;

					SetCloth(
						pDevice, 
						pTVISUAL->m_dwCLKID, 
						pTVISUAL->m_dwCLIID, 
						pTVISUAL->m_dwMesh[bMode]);

					bUseDefault = FALSE;

					pTApplyGrade[i] = pTITEM; // 밑에서 Item Grade Visual을 적용하기 위해 대상 수집.
					pTApplyGradeMode[i] = bMode;
				}
			}
		}

		if( bCostumeHide[ i ] )
		{
			ClearDEFEQUIP( pDevice, vREPLACE[i] ); // 해당 부위를 없애주는 코드 추가
			continue;
		}

		if( bUseDefault )
			ResetDEFEQUIP( pDevice, vREPLACE[i] );
	}

	ResetWeaponPOS(
		dwCostumeHide,
		pDevice,
		pRES);

	ApplyGradeEffect( pTApplyGrade, TREPLACE_COUNT ); // 방어구에 강화비주얼 적용

	for( INT i=0 ; i < TREPLACE_COUNT ; ++i )
	{
		if( !pTApplyGrade[i] )
			continue;

		BYTE bMode = pTApplyGradeMode[i];
		DWORD dwFuncID = pTApplyGrade[ i ]->GetVisual()->m_dwEffectFuncID[bMode];

		if( dwFuncID != 0 )
		{
			GenerateSFX(
				&m_mapOBJSFXINST,
				&m_vOBJSFX,
				&m_mapOBJSFX,
				0.0f,
				dwFuncID );

			m_vEquipSFXFuncID.push_back( dwFuncID );
		}
	}
}

BYTE CTClientObjBase::GetEquipModeIndex( BYTE bEquipPOS,
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

void CTClientObjBase::ResetOBJPart( CD3DDevice *pDevice)
{
	static dwCOSTUME[4] =
	{
		ES_COSTUME_HAT,
		ES_COSTUME_DRESS,
		ES_COSTUME_ACCESSORY,
		ES_COSTUME_RACE
	};
	
	CTClientInven *pTEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);

	if(pTEQUIP)
	{
		DWORD dwCostumeHide = 0;
		for( BYTE i=0 ; i < 4 ; ++i )
		{
			CTClientItem* pItem = pTEQUIP->FindTItem(dwCOSTUME[i]);
			if( pItem )
			{
				LPTITEMVISUAL pTCostumeVISUAL = pItem->GetVisual();
				if( pTCostumeVISUAL )
					dwCostumeHide |= pTCostumeVISUAL->m_dwCostumeHide;
			}
		}

		for( BYTE i=0; i<ES_COUNT; i++)
		{
			if( IsDisguise() || (m_bHelmetHide && ( i == ES_HEAD || i == ES_FACE )) || (dwCostumeHide & (1<<i)) )
				continue;

			THIDEPART vTHIDE = FindHidePart( pTEQUIP->FindTItem(i), i);

			if(CheckOBJPart(&vTHIDE))
			{
				CTClientItem *pHIDE = pTEQUIP->FindTItem(vTHIDE.m_bHideSlotID);

				if( pHIDE && pHIDE->GetTITEM() )
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
		}
	}

	CTachyonObject::ResetOBJPart(pDevice);
}

void CTClientObjBase::ResetDEFOBJPart( LPTHIDEPART pTHIDE)
{
}

void CTClientObjBase::ResetDEFEQUIP( CD3DDevice *pDevice,
									 BYTE bEquipPOS)
{
}

void CTClientObjBase::ClearDEFEQUIP(
	CD3DDevice *pDevice,
	BYTE bEquipPOS)
{
}

THIDEPART CTClientObjBase::FindHidePart( CTClientItem *pTITEM,
										 BYTE bSlotID)
{
	THIDEPART vRESULT = { 0xFF, 0xFF, 0};

	if( pTITEM && pTITEM->GetTITEM() )
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

BYTE CTClientObjBase::CheckOBJPart( LPTHIDEPART pTHIDE)
{
	return pTHIDE && pTHIDE->m_bHideSlotID != 0xFF ? TRUE : FALSE;
}

CString CTClientObjBase::GetName()
{
	return CString("");
}

CString CTClientObjBase::GetTitle()
{
	return CString("");
}

CString CTClientObjBase::GetUserTitle()
{
	return CString("");
}

DWORD CTClientObjBase::GetHostID()
{
	return m_dwID;
}

BYTE CTClientObjBase::IsDead()
{
	return m_bAction == TA_DEAD || m_bAction == TA_DIE ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsDrawNameWhenDead()
{
	return TRUE;
}

BYTE CTClientObjBase::IsRide()
{
	return m_bAction == TA_RIDING ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsRoam()
{
	return m_bAction == TA_WALK || m_bAction == TA_RUN ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsJump()
{
	return m_bAction == TA_JUMP || m_bAction == TA_FLY ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsFall()
{
	return m_bAction == TA_FALL ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsDown()
{
	return IsPush() || m_bDown ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsPush()
{
	return m_bAction == TA_PUSH || m_bAction == TA_PUSHSTART ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsBlock()
{
	return m_bAction == TA_STUN || m_bAction == TA_HOLD ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsSlidingWhenLastHit()
{
	return TRUE;
}

void CTClientObjBase::ClearTARROW()
{
	static DWORD dwPIVOT[TSHOTPIVOT_COUNT] = {
		ID_PIVOT_ARROW,
		ID_PIVOT_CARROW};

	for( int i=0; i<TSHOTPIVOT_COUNT; i++)
	{
		MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwPIVOT[i]);

		if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
		{
			MAPOBJECT::iterator finder = m_mapEQUIP.find((*itID).second + 1);

			if( finder != m_mapEQUIP.end() )
			{
				delete (*finder).second;
				m_mapEQUIP.erase(finder);
			}
		}
	}
}

void CTClientObjBase::StopTSLASH()
{
	for( int i=0; i<TSLASHSFX_COUNT; i++)
		m_vTSLASH[i].SFXStop();
}

void CTClientObjBase::SetAction( DWORD dwActionID,
								 DWORD dwAniID)
{
	if( m_dwActionID != dwActionID || m_dwAniID != dwAniID )
	{
		if( m_bAction == TA_STAND )
			FinishSUBACT();

		if(m_bActionLock)
			m_bActionLock = FALSE;

		if(m_dwANISNDFuncID)
			m_bSTEPSND = TRUE;

		if(m_bDown)
			m_bDown = FALSE;

		ClearTARROW();
		StopTSLASH();

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

void CTClientObjBase::SetSubAction( DWORD dwActionID,
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

			ClearTARROW();
			StopTSLASH();
		}

	if(m_bActionLock)
		m_bActionLock = FALSE;
}

void CTClientObjBase::ResetVisible( CTClientCAM *pCamera,
								    DWORD dwHouseID,
									BYTE bHouseMesh,
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

	if( m_bType != OT_COLLISION && m_bType != OT_PATHWALL && !m_bGARBAGE && (
		dwHouseID == m_dwHouseID || ((!dwHouseID || bHouseMesh) && (!m_dwHouseID || m_bHouseMesh))) )
	{
		FLOAT fDIST = m_fCamDIST - m_fSight;
		BYTE bPrevAlpha = m_bAlpha;

		if( !m_bFPView && CheckRequired() && fDIST < GetCullRANGE() && (!m_bHide || (m_bDetect && fDIST < GetCullRANGE() / 2.0f)) )
		{
			DWORD dwMaxTick = DWORD(m_bHide ? TMAXALPHA_TICK / 2.0f : TMAXALPHA_TICK);
			m_dwAlphaTick = min( dwMaxTick, m_dwAlphaTick + dwTick);
		}
		else
			m_dwAlphaTick -= min( m_dwAlphaTick, dwTick);

		if( fDIST > GetCullRANGE() + TALPHA_DIST )
			m_dwAlphaTick = 0;
		m_bAlpha = BYTE(ALPHA_MAX * m_dwAlphaTick / TMAXALPHA_TICK);

		if( ((CTClientGame::GetInstance()->GetClientMap()->m_wMapID/100) == 5) ) // 500번대 맵
			if( (CTClientGame::GetInstance()->m_bTournamentWatching && m_bTournamentTeam != TTOURNAMENT_TEAM_NONE) ||
				!CTClientGame::GetInstance()->m_bTournamentBattle )
			{
				if( m_bAlpha < 128 )
					m_bAlpha = 128;
				m_bVisible = TRUE;
			}

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

			if(!bPrevAlpha)
			{
				HideSFX();
				ShowSFX();
			}

			if( bPrevAlpha != m_bAlpha )
			{
				SetAlphaOfAllWeaponSFX( m_bAlpha );
				SetAlphaOfAllEquipSFX( m_bAlpha );
			}
		}
		else if(bPrevAlpha)
			HideSFX();
	}
}

void CTClientObjBase::ShowSFX()
{
	MAPTMAINTAIN::iterator itMAINTAIN;
	VECTOROBJSFX::iterator itSFX;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
		PlayTActionSFX( &(*itMAINTAIN).second->m_vTSFX, (*itMAINTAIN).second->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);

	for( itSFX = m_vTGARBAGESFX.begin(); itSFX != m_vTGARBAGESFX.end(); ++itSFX )
	{
		CTClientSFX* pTSFX = (CTClientSFX*)( (*itSFX)->m_pUserData );
		if( pTSFX )
			pTSFX->m_bShow = TRUE;
	}

	if(m_vTMARKER.m_OBJ.m_pOBJ)
		m_vTMARKER.m_bSFX = TRUE;

	{
		VECTORDWORD::iterator it, end;
		it = m_vEquipSFXFuncID.begin();
		end = m_vEquipSFXFuncID.end();
		for(; it != end ; ++it )
		{
			GenerateSFX(
				&m_mapOBJSFXINST,
				&m_vOBJSFX,
				&m_mapOBJSFX,
				0.0f,
				(*it) );
		}
	}

	{
		MAPOBJECT::iterator it, end;
		it = m_mapEQUIP.begin();
		end = m_mapEQUIP.end();

		for(; it != end ; ++it )
		{
			DWORD dwWeaponPOS = it->first;
			CTachyonObject* pTOBJ = it->second;

			TMAPWEAPONEQUIPINFO::iterator itINFO = m_mapEQUIPInfo.find( dwWeaponPOS );
			if( itINFO != m_mapEQUIPInfo.end() )
			{
				LPTITEMVISUAL pEquipVisual = itINFO->second.m_pItemVisual;
				BYTE bMode = itINFO->second.m_bMode;

				if( pEquipVisual->m_dwEffectFuncID[bMode] != 0 )
				{
					pTOBJ->GenerateSFX(
						&pTOBJ->m_mapOBJSFXINST,
						&pTOBJ->m_vOBJSFX,
						&pTOBJ->m_mapOBJSFX,
						0.0f,
						pEquipVisual->m_dwEffectFuncID[bMode] );
				}
			}
		}
	}

	m_bSFX = TRUE;
}

void CTClientObjBase::HideSFX()
{
	MAPTMAINTAIN::iterator itMAINTAIN;
	VECTOROBJSFX::iterator itSFX;

	for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
		RemoveTActionSFX(&(*itMAINTAIN).second->m_vTSFX);

	for( itSFX = m_vTGARBAGESFX.begin(); itSFX != m_vTGARBAGESFX.end(); ++itSFX )
	{
		CTClientSFX* pTSFX = (CTClientSFX*)( (*itSFX)->m_pUserData );
		if( pTSFX )
			pTSFX->m_bShow = FALSE;
	}

	if(m_vTMARKER.m_OBJ.m_pOBJ)
	{
		m_vTMARKER.RemoveOBJSFX();
		m_vTMARKER.m_bSFX = FALSE;
	}

	m_bSFX = FALSE;
	RemoveOBJSFX();
}

void CTClientObjBase::ApplyTAction( CD3DDevice *pDevice,
								    CTachyonRes *pRES,
									BYTE bAction)
{
	if( CTChart::GetTACTION(m_bAction)->m_bLevel > CTChart::GetTACTION(bAction)->m_bLevel ||
		m_bGhost || IsDead() || IsBlock() || IsDown() )
		return;

	BYTE bLoopACT = GetTLOOPAction(bAction);
	BYTE bWeaponID = GetWeaponID(m_bMode);

	BYTE bEquipMode = GetEquipMode(
		bAction,
		bWeaponID);

	TACTION vActionID = FindActionID(
		bAction,
		bWeaponID);
	LPTACTIONDATA pTACTDATA = CTChart::GetTACTION(bAction);

	if( m_bHideOnCapeMode != pTACTDATA->m_bHideOnCapeMode ||
		m_bEquipMode != bEquipMode )
	{
		m_bHideOnCapeMode = pTACTDATA->m_bHideOnCapeMode;
		m_bEquipMode = bEquipMode;

		ResetEQUIP(
			pDevice,
			pRES);
	}

	if( bLoopACT != TA_COUNT )
		m_bAction = bLoopACT;
	else if(pTACTDATA->m_bContinue)
		m_bAction = bAction;

	SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	m_bACTLevel = pTACTDATA->m_bLevel;
}

void CTClientObjBase::SetTAction( BYTE bAction)
{
	if( CTChart::GetTACTION( m_bAction )->m_bLevel > CTChart::GetTACTION( bAction )->m_bLevel )
		return;

	m_bAction = bAction;
}

BYTE CTClientObjBase::GetTLOOPAction( BYTE bActionID)
{
	return CTChart::GetTACTION( bActionID )->m_bLoopAction;
}

BYTE CTClientObjBase::GetTAction()
{
	return m_bIsStuned || m_bIsHolded ? m_bLastACT : TA_STAND;
}

BYTE CTClientObjBase::GetEquipMode( BYTE bActionID,
								    BYTE bWeaponID)
{
	LPTACTION pTACTION = CTChart::FindTACTIONTEMP( bActionID, bWeaponID );

	if( pTACTION )
		pTACTION->m_bEquipMode;

	return MT_NORMAL;
}

FLOAT CTClientObjBase::GetCullRANGE()
{
	switch(m_bType)
	{
	case OT_RECALL	:
	case OT_SELF	:
	case OT_MON		:
	case OT_NPC		:
	case OT_PC		:
		if(CTClientGame::GetInstance()->GetMainChar()->m_bStandHide)
			return TSTANDHIDE_SIGHT_DIST;
	}

	return CTClientGame::GetOBJRangeOption() * m_fRange;
}

FLOAT CTClientObjBase::GetCullDIST()
{
	return CTClientGame::GetOBJRangeOption() * m_fCamDist;
}

FLOAT CTClientObjBase::LockDamagePOS()
{
	static FLOAT vTPOS[TNUMBERPOS_COUNT] = { 0.0f, -1.0f, 1.0f, -2.0f, 2.0f};

	DWORD dwMinTICK = m_vDamageTick[0];
	BYTE bINDEX = 0;

	for( BYTE i=0; i<TNUMBERPOS_COUNT; i++)
	{
		if(!m_vDamageTick[i])
		{
			m_vDamageTick[i] = TDAMAGE_TICK;
			return vTPOS[i];
		}

		if( m_vDamageTick[i] < dwMinTICK )
		{
			dwMinTICK = m_vDamageTick[i];
			bINDEX = i;
		}
	}
	m_vDamageTick[bINDEX] = TDAMAGE_TICK;

	return vTPOS[bINDEX];
}

LPOBJTEX CTClientObjBase::NewGradeObjTex( LPOBJTEX pTEX,
										  INT nType, // only TT_COLOR or TT_TEX
										  DWORD dwCOLOR,
										  LPTEXTURESET pSkinTEX,
										  BYTE bPSTYPE)
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
		
	TOBJTEXCOLORKEY key;
	key.m_pTEX = pTEX;
	key.m_dwCOLOR = dwCOLOR;
	key.m_pSkinTEX = pSkinTEX;
	key.m_bPSTYPE = bPSTYPE;

	MAPTOBJTEXCOLORPAIR::iterator itr = m_mapOBJTEXCOLOR.find(key);
	if( itr != m_mapOBJTEXCOLOR.end() )
		return itr->second;

	LPOBJTEX pNewTEX = new OBJTEX;
	*pNewTEX = *pTEX;

	pNewTEX->m_dwOP = (DWORD) (nType == TT_TEX ? vCOLOROP[bPSTYPE] : D3DTOP_MULTIPLYADD);
	pNewTEX->m_bType[1] = nType;
	pNewTEX->m_dwCOLOR = dwCOLOR;
	pNewTEX->m_pTEX[1] = pSkinTEX;
	m_mapOBJTEXCOLOR.insert( make_pair(key,pNewTEX) );

	return pNewTEX;
}

void CTClientObjBase::ReleaseGradeObjTex()
{
	MAPTOBJTEXCOLORPAIR::iterator itr,end;
	itr = m_mapOBJTEXCOLOR.begin();
	end = m_mapOBJTEXCOLOR.end();

	for(;itr!=end; ++itr)
		delete itr->second;

	m_mapOBJTEXCOLOR.clear();
}

LPTACTION CTClientObjBase::FindEquipTACTION( BYTE bActionID,
											 BYTE bWeaponID)
{
	LPTACTION pTACTION = CTChart::FindTACTIONTEMP( bActionID, bWeaponID);

	if(pTACTION)
	{
		if( !m_bShoot && pTACTION->m_bEquipMode == TEM_LONG )
		{
			CTClientInven *pTEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);

			if(pTEQUIP)
			{
				CTClientItem *pWEAPON = pTEQUIP->FindTItem(ES_LONGWEAPON);

				if(pWEAPON)
					pTACTION = CTChart::FindTACTIONTEMP( bActionID, pWEAPON->GetTITEM()->m_bWeaponID);
			}
		}
	}

	return pTACTION;
}

TACTION CTClientObjBase::FindActionID( BYTE bActionID,
									   BYTE bWeaponID)
{
	TACTION vResult = { 0, 0, 0, 0};

	if(FindActionID( &vResult, bActionID, bWeaponID))
		return vResult;

	if(FindActionID( &vResult, bActionID, WT_NORMAL))
		return vResult;

	if(FindActionID( &vResult, TA_STAND, bWeaponID))
		return vResult;
	FindActionID( &vResult, TA_STAND, WT_NORMAL);

	return vResult;
}

BYTE CTClientObjBase::FindActionID( LPTACTION pTACTION,
								    BYTE bActionID,
									BYTE bWeaponID)
{
	LPTACTION pTACT = FindEquipTACTION(
		bActionID,
		bWeaponID);

	if(!pTACT)
		return FALSE;

	MAPACTION::iterator itACT = m_OBJ.m_pOBJ->m_mapACTION.find(pTACT->m_dwActID);
	if( itACT == m_OBJ.m_pOBJ->m_mapACTION.end() )
		return FALSE;

	LPACTION pOBJACT = (*itACT).second;
	if(!pOBJACT)
		return FALSE;

	int nCount = INT(pOBJACT->m_mapANI.size());
	if( nCount <= 0 )
		return FALSE;

	if( bActionID == TA_STAND )
		nCount = 1;

	pTACTION->m_bEquipMode = pTACT->m_bEquipMode;
	pTACTION->m_dwTSLASH = pTACT->m_dwTSLASH;
	pTACTION->m_dwActID = pTACT->m_dwActID;
	nCount = rand() % nCount;

	MAPANI::iterator itANI = pOBJACT->m_mapANI.begin();
	for( int i=0; i<nCount; i++)
		itANI++;
	pTACTION->m_dwAniID = (*itANI).first;

	return TRUE;
}

BYTE CTClientObjBase::GetWeaponID( BYTE bMode)
{
	if( bMode == MT_NORMAL )
		return WT_NORMAL;

	CTClientInven *pTEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);
	if(!pTEQUIP)
		return WT_HAND;

	if(m_bShoot)
	{
		CTClientItem *pWEAPON = pTEQUIP->FindTItem(ES_LONGWEAPON);

		if(pWEAPON)
			return pWEAPON->GetTITEM()->m_bWeaponID;
	}
	else
	{
		CTClientItem *pRHAND = pTEQUIP->FindTItem(ES_PRMWEAPON);
		CTClientItem *pLHAND = pTEQUIP->FindTItem(ES_SNDWEAPON);

		if( pRHAND && pRHAND->GetTITEM() && pRHAND->GetTITEM()->m_bWeaponID == WT_1HAND &&
			pRHAND->GetTITEM()->m_bSubSlotID == ES_SNDWEAPON && !pLHAND )
			pLHAND = pRHAND;

		if( pLHAND && pLHAND->GetTITEM() && (
			pLHAND->GetTITEM()->m_bWeaponID == WT_1HAND ||
			pLHAND->GetTITEM()->m_bWeaponID == WT_LHAND) )
			if( pRHAND && pRHAND->GetTITEM() )
				return rand() % 2 ? WT_1HAND : WT_LHAND;
			else
				return WT_LHAND;
		else if( pRHAND && pRHAND->GetTITEM() )
			return pRHAND->GetTITEM()->m_bWeaponID;
	}

	return WT_HAND;
}

FLOAT CTClientObjBase::GetHeight( CTClientObjBase *pOBJ,
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

FLOAT CTClientObjBase::GetCylinderHeight( CTClientObjBase *pOBJ,
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

FLOAT CTClientObjBase::GetBoxHeight( CTClientObjBase *pOBJ,
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
			FLOAT fMaxHeight;
			FLOAT fMinHeight;

			for( int k=0; k<3; k++)
			{
				/*!
				*	@brief 권성혁
				*		pOBJ->m_vPosition을 ..m_vWorld로 바꿈
				*		m_vPosition은 scale도 적용된 상태라서
				*		박스의 실제 size에 scale이 한번더 적용되서 커짐.
				*/
				CTMath::Transform(
					&m_vWorld,
					&vPOLY[i][j][k]);

				if( k > 0 )
				{
					if( fMaxHeight < vPOLY[i][j][k].y )
						fMaxHeight = vPOLY[i][j][k].y;

					if( fMinHeight > vPOLY[i][j][k].y )
						fMinHeight = vPOLY[i][j][k].y;
				}
				else
				{
					fMaxHeight = vPOLY[i][j][k].y;
					fMinHeight = vPOLY[i][j][k].y;
				}
			}

			if( fMaxHeight > fHeight && pPOINT->y + pOBJ->m_fSizeY > fMinHeight )
			{
				FLOAT fLocal = GetPolyHeight(
					vPOLY[i][j],
					pPOINT,
					pOBJ->m_fRadius);

				if( fHeight < fLocal )
					fHeight = fLocal;
			}
		}

	return fHeight;
}

FLOAT CTClientObjBase::GetPolyHeight( CTClientObjBase *pOBJ,
									  LPD3DXVECTOR3 pPOINT)
{
	if( pPOINT->y + pOBJ->m_fSizeY <= m_vPosition._42 - m_fDepth )
		return TMIN_HEIGHT;
	FLOAT fHeight = TMIN_HEIGHT;

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
				int nCount = INT(m_vVCOLLISION.size());

				for( int i=0; i<nCount; i++)
				{
					LPTVERTEXINDEX pVCOLLISION = m_vVCOLLISION[i];

					WORD wINDEX = pVCOLLISION->m_wINDEX;
					WORD wMESH = pVCOLLISION->m_wMESH;
					WORD wPOLY = pVCOLLISION->m_wPOLY;

					LPIBDATA pIBDATA = pMESH->m_pMESH[wMESH][0]->m_vIB[wINDEX];
					D3DXVECTOR3 vPOLY[3];
					FLOAT fMaxHeight;
					FLOAT fMinHeight;

					for( int j=0; j<3; j++)
					{
						LPWMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * wPOLY + j]];

						D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
							m_pBone,
							pMESHMAT,
							pMESHDATA) : m_vPosition;

						vPOLY[j] = D3DXVECTOR3(
							pMESHDATA->m_fPosX,
							pMESHDATA->m_fPosY,
							pMESHDATA->m_fPosZ);

						CTMath::Transform(
							&vTrans,
							&vPOLY[j]);

						if( j > 0 )
						{
							if( fMaxHeight < vPOLY[j].y )
								fMaxHeight = vPOLY[j].y;

							if( fMinHeight > vPOLY[j].y )
								fMinHeight = vPOLY[j].y;
						}
						else
						{
							fMaxHeight = vPOLY[j].y;
							fMinHeight = vPOLY[j].y;
						}
					}

					if( fMaxHeight > fHeight && pPOINT->y + pOBJ->m_fSizeY > fMinHeight )
					{
						FLOAT fLocal = GetPolyHeight(
							vPOLY,
							pPOINT,
							pOBJ->m_fRadius);

						if( fHeight < fLocal )
							fHeight = fLocal;
					}
				}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;
				int nCount = INT(m_vVCOLLISION.size());

				for( int i=0; i<nCount; i++)
				{
					LPTVERTEXINDEX pVCOLLISION = m_vVCOLLISION[i];

					WORD wINDEX = pVCOLLISION->m_wINDEX;
					WORD wMESH = pVCOLLISION->m_wMESH;
					WORD wPOLY = pVCOLLISION->m_wPOLY;

					LPIBDATA pIBDATA = pMESH->m_pMESH[wMESH][0]->m_vIB[wINDEX];
					D3DXVECTOR3 vPOLY[3];
					FLOAT fMaxHeight;
					FLOAT fMinHeight;

					for( int j=0; j<3; j++)
					{
						LPMESHVERTEX pMESHDATA = &pDATA[pIBDATA->m_pIB[3 * wPOLY + j]];

						vPOLY[j] = D3DXVECTOR3(
							pMESHDATA->m_fPosX,
							pMESHDATA->m_fPosY,
							pMESHDATA->m_fPosZ);

						CTMath::Transform(
							&m_vPosition,
							&vPOLY[j]);

						if( j > 0 )
						{
							if( fMaxHeight < vPOLY[j].y )
								fMaxHeight = vPOLY[j].y;

							if( fMinHeight > vPOLY[j].y )
								fMinHeight = vPOLY[j].y;
						}
						else
						{
							fMaxHeight = vPOLY[j].y;
							fMinHeight = vPOLY[j].y;
						}
					}

					if( fMaxHeight > fHeight && pPOINT->y + pOBJ->m_fSizeY > fMinHeight )
					{
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
	}

	return fHeight;
}

FLOAT CTClientObjBase::GetPolyHeight( LPD3DXVECTOR3 pPOLY,
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
		D3DXVECTOR3 vCENTER( 0.0f, 0.0f, 0.0f);

		FLOAT fLength = D3DXVec3Length(&vPOS);
		FLOAT fPolyR = 0.0f;
		FLOAT fDist;
		FLOAT fU;
		FLOAT fV;

		if( fLength > 0.0f )
		{
			if( fabs(vPLANE.b) / fLength < CTClientMAP::m_fMoveBound )
				return TMIN_HEIGHT;

			vPOS *= fRadius / fLength;
			vPOS = (*pPOS) - vPOS;
		}
		else
			vPOS = (*pPOS);

		for( int i=0; i<3; i++)
			vCENTER += pPOLY[i];
		vCENTER /= 3.0f;

		for( i=0; i<3; i++)
		{
			FLOAT fLocal = D3DXVec2Length(&D3DXVECTOR2(
				pPOLY[i].x - vCENTER.x,
				pPOLY[i].z - vCENTER.z));

			if( fLocal > fPolyR )
				fPolyR = fLocal;
		}

		if( fPolyR + fRadius < D3DXVec2Length(&D3DXVECTOR2( pPOS->x - vCENTER.x, pPOS->z - vCENTER.z)) )
			return TMIN_HEIGHT;

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

		for( i=0; i<3; i++)
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

BYTE CTClientObjBase::CheckCollision( CTClientObjBase *pOBJ,
									  LPD3DXVECTOR3 pTSTART,
									  LPD3DXVECTOR3 pTEND,
									  FLOAT fSizeY,
									  FLOAT *pDIST)
{
	if( pOBJ->m_bType == OT_PATHWALL )
		return FALSE;

	switch(pOBJ->m_bCollisionType)
	{
	case TCOLLISION_CYLINDER	: return CheckCylinder( pOBJ, pTSTART, pTEND, fSizeY, pDIST);
	case TCOLLISION_BOX			: return CheckBox( pOBJ, pTSTART, pTEND, fSizeY, pDIST);
	case TCOLLISION_POLY		: return CheckPoly( pOBJ, pTSTART, pTEND, fSizeY, pDIST);
	}

	return FALSE;
}

BYTE CTClientObjBase::CheckCylinder( CTClientObjBase *pOBJ,
									 LPD3DXVECTOR3 pTSTART,
									 LPD3DXVECTOR3 pTEND,
									 FLOAT fSizeY,
									 FLOAT *pDIST)
{
	FLOAT fMaxH = max( pTSTART->y, pTEND->y) + fSizeY;
	FLOAT fMinH = min( pTSTART->y, pTEND->y);

	if( fMaxH <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		fMinH >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;

	FLOAT fRADIUS = m_fRadius + pOBJ->m_fRadius;
	FLOAT fDIST;

	D3DXVECTOR3 vTSTART(
		pTSTART->x,
		0.0f,
		pTSTART->z);

	D3DXVECTOR3 vTEND(
		pTEND->x,
		0.0f,
		pTEND->z);

	D3DXVECTOR3 vTCENTER(
		pOBJ->m_vPosition._41,
		0.0f,
		pOBJ->m_vPosition._43);
	D3DXVECTOR3 vTPOINT;

	if(!CTMath::GetProjectionPoint( &vTPOINT, &vTCENTER, &vTSTART, &vTEND))
		return FALSE;
	vTPOINT.y = 0.0f;

	fDIST = D3DXVec3LengthSq(&(vTPOINT - vTCENTER));
	fRADIUS *= fRADIUS;

	if( fDIST < fRADIUS )
	{
		D3DXVECTOR3 vTDIR = vTSTART - vTPOINT;
		D3DXPLANE vTPLANE;

		fRADIUS -= fDIST;
		fDIST = D3DXVec3Length(&vTDIR);

		if( fDIST == 0.0f )
			return FALSE;
		vTDIR /= fDIST;

		vTPOINT += sqrt(fRADIUS) * vTDIR;
		vTDIR = vTPOINT - vTCENTER;

		D3DXPlaneFromPointNormal(
			&vTPLANE,
			&vTCENTER,
			&vTDIR);

		D3DXPlaneNormalize(
			&vTPLANE,
			&vTPLANE);
		vTPLANE.d -= pOBJ->m_fRadius;

		if(CTMath::CheckCollision( &vTPLANE, &vTSTART, &vTEND, m_fRadius, pDIST))
		{
			pTSTART->x = vTSTART.x;
			pTSTART->z = vTSTART.z;

			pTEND->x = vTEND.x;
			pTEND->z = vTEND.z;

			return TRUE;
		}
	}

	return FALSE;
}

BYTE CTClientObjBase::CheckBox( CTClientObjBase *pOBJ,
							    LPD3DXVECTOR3 pTSTART,
								LPD3DXVECTOR3 pTEND,
								FLOAT fSizeY,
								FLOAT *pDIST)
{
	FLOAT fMaxH = max( pTSTART->y, pTEND->y) + fSizeY;
	FLOAT fMinH = min( pTSTART->y, pTEND->y);

	if( fMaxH <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		fMinH >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
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

	D3DXVECTOR3 vTSTART = (*pTSTART);
	D3DXVECTOR3 vTEND = (*pTEND);

	D3DXQUATERNION vQUAT;
	D3DXMATRIX vMAT;
	BYTE bResult = FALSE;

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
			D3DXVECTOR3 vTS = vTSTART;
			D3DXVECTOR3 vTE = vTEND;
			FLOAT fLocal = 0.0f;

			for( int k=0; k<3; k++)
			{
				CTMath::Transform(
					&vMAT,
					&vPOLY[i][j][k]);
			}

			if( CheckPoly( vPOLY[i][j], &vTS, &vTE, m_fRadius, fSizeY, CTClientMAP::m_fMoveBound, &fLocal) && (!bResult || fLocal < (*pDIST)) )
			{
				(*pTSTART) = vTS;
				(*pTEND) = vTE;
				(*pDIST) = fLocal;

				bResult = TRUE;
			}
		}

	return bResult;
}

BYTE CTClientObjBase::CheckPoly( CTClientObjBase *pOBJ,
								 LPD3DXVECTOR3 pTSTART,
								 LPD3DXVECTOR3 pTEND,
								 FLOAT fSizeY,
								 FLOAT *pDIST)
{
	FLOAT fMaxH = max( pTSTART->y, pTEND->y) + fSizeY;
	FLOAT fMinH = min( pTSTART->y, pTEND->y);

	if( fMaxH <= pOBJ->m_vPosition._42 - pOBJ->m_fDepth ||
		fMinH >= pOBJ->m_vPosition._42 + pOBJ->m_fSizeY )
		return FALSE;
	BYTE bResult = FALSE;

	LPD3DXMATRIX pMESHMAT = pOBJ->GetMeshMatrix();
	LPANI pANI = pOBJ->GetCurANI();
	MAPCLKINST::iterator it;

	D3DXVECTOR3 vTSTART = (*pTSTART);
	D3DXVECTOR3 vTEND = (*pTEND);

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

					D3DXVECTOR3 vTS = vTSTART;
					D3DXVECTOR3 vTE = vTEND;
					FLOAT fLocal = 0.0f;

					if( CheckPoly( vPOLY, &vTS, &vTE, m_fRadius, fSizeY, CTClientMAP::m_fMoveBound, &fLocal) && (!bResult || fLocal < (*pDIST)) )
					{
						(*pTSTART) = vTS;
						(*pTEND) = vTE;
						(*pDIST) = fLocal;

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

					D3DXVECTOR3 vTS = vTSTART;
					D3DXVECTOR3 vTE = vTEND;
					FLOAT fLocal = 0.0f;

					if( CheckPoly( vPOLY, &vTS, &vTE, m_fRadius, fSizeY, CTClientMAP::m_fMoveBound, &fLocal) && (!bResult || fLocal < (*pDIST)) )
					{
						(*pTSTART) = vTS;
						(*pTEND) = vTE;
						(*pDIST) = fLocal;

						bResult = TRUE;
					}
				}
			}
		}
	}

	return bResult;
}

BYTE CTClientObjBase::CheckPoly( LPD3DXVECTOR3 pPOLY,
								 LPD3DXVECTOR3 pTSTART,
								 LPD3DXVECTOR3 pTEND,
								 FLOAT fRadius,
								 FLOAT fSizeY,
								 FLOAT fMoveBound,
								 FLOAT *pDIST)
{
	FLOAT fMaxH = max( pTSTART->y, pTEND->y) + fSizeY;
	FLOAT fMinH = min( pTSTART->y, pTEND->y);
	FLOAT fREV = 0.0f;

	D3DXPLANE vTROOF( 0.0f, -1.0f, 0.0f, fMaxH);
	D3DXPLANE vTLAND( 0.0f, 1.0f, 0.0f, -fMinH);

	D3DXPLANE vTNORMAL;
	D3DXPLANE vTPLANE;

	FLOAT fDist = 0.0f;
	FLOAT fDotP = 0.0f;
	FLOAT fDotO = 0.0f;
	BYTE bINIT = FALSE;

	D3DXVECTOR3 vTSTART(
		pTSTART->x,
		fMinH,
		pTSTART->z);

	D3DXVECTOR3 vTEND(
		pTEND->x,
		fMinH,
		pTEND->z);

	D3DXVECTOR3 vPoint;
	D3DXVECTOR3 vP;
	D3DXVECTOR3 vO;

	D3DXPlaneFromPoints(
		&vTPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	D3DXPlaneNormalize(
		&vTPLANE,
		&vTPLANE);

	if( D3DXPlaneDotCoord( &vTPLANE, pTSTART) < 0.0f )
		return FALSE;

	if( D3DXPlaneDotCoord( &vTPLANE, &vTSTART) < 0.0f )
	{
		vTPLANE.d -= DEF_EPSILON;

		if(!D3DXPlaneIntersectLine( &vPoint, &vTPLANE, &vTSTART, &vTEND))
			return FALSE;

		vP = vPoint - vTSTART;
		vO = vTEND - vTSTART;

		fREV = (D3DXVec3Dot( &vO, &vP) > 0.0f ? 1.0f : -1.0f) * D3DXVec3Length(&vP);
		vTSTART = vPoint;
	}

	vPoint.x = vTPLANE.a;
	vPoint.y = 0.0f;
	vPoint.z = vTPLANE.c;
	fDist = D3DXVec3Length(&vPoint);

	if( fDist == 0.0f || fabs(vTPLANE.b) / fDist > fMoveBound )
		return FALSE;
	vPoint /= fDist;

	vTPLANE = D3DXPLANE(
		vPoint.x,
		vPoint.y,
		vPoint.z,
		0.0f);

	D3DXVec3Cross(
		&vPoint,
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
		&vPoint);
	fDist = D3DXVec3Length(&vPoint);

	if( fDist == 0.0f )
		return FALSE;
	vPoint /= fDist;

	vTNORMAL = D3DXPLANE(
		vPoint.x,
		vPoint.y,
		vPoint.z,
		0.0f);

	for( int i=0; i<3; i++)
	{
		D3DXVECTOR3 vB = pPOLY[(i + 1) % 3];
		D3DXVECTOR3 vA = pPOLY[i];

		FLOAT fDotA = D3DXPlaneDotCoord( &vTLAND, &vA);
		FLOAT fDotB = D3DXPlaneDotCoord( &vTLAND, &vB);

		if( fDotA > 0.0f || fDotB > 0.0f )
		{
			if( fDotA * fDotB < 0.0f )
				D3DXPlaneIntersectLine( fDotA < 0.0f ? &vA : &vB, &vTLAND, &vA, &vB);

			fDotA = D3DXPlaneDotCoord( &vTROOF, &vA);
			fDotB = D3DXPlaneDotCoord( &vTROOF, &vB);

			if( fDotA > 0.0f || fDotB > 0.0f )
			{
				if( fDotA * fDotB < 0.0f )
					D3DXPlaneIntersectLine( fDotA < 0.0f ? &vA : &vB, &vTROOF, &vA, &vB);

				vA.y = 0.0f;
				vB.y = 0.0f;

				FLOAT fDistA = D3DXPlaneDotCoord( &vTPLANE, &vA);
				FLOAT fDistB = D3DXPlaneDotCoord( &vTPLANE, &vB);

				fDotA = D3DXPlaneDotCoord( &vTNORMAL, &vA);
				fDotB = D3DXPlaneDotCoord( &vTNORMAL, &vB);

				if(!bINIT)
				{
					fDist = fDistA;
					fDotP = fDotA;
					fDotO = fDotA;

					vPoint = vA;
					vP = vA;
					vO = vA;

					bINIT = TRUE;
				}

				if( fDistA > fDist )
				{
					fDist = fDistA;
					vPoint = vA;
				}

				if( fDistB > fDist )
				{
					fDist = fDistB;
					vPoint = vB;
				}

				if( fDotA < fDotO )
				{
					fDotO = fDotA;
					vO = vA;
				}

				if( fDotB < fDotO )
				{
					fDotO = fDotB;
					vO = vB;
				}

				if( fDotA > fDotP )
				{
					fDotP = fDotA;
					vP = vA;
				}

				if( fDotB > fDotP )
				{
					fDotP = fDotB;
					vP = vB;
				}
			}
		}
	}

	if(!bINIT)
		return FALSE;

	D3DXVECTOR3 vTDIR(
		vTPLANE.a,
		vTPLANE.b,
		vTPLANE.c);

	D3DXPlaneFromPointNormal(
		&vTPLANE,
		&vPoint,
		&vTDIR);

	D3DXPlaneNormalize(
		&vTPLANE,
		&vTPLANE);

	vP -= D3DXPlaneDotCoord( &vTPLANE, &vP) * vTDIR;
	vO -= D3DXPlaneDotCoord( &vTPLANE, &vO) * vTDIR;

	vTDIR = vTEND - vTSTART;
	vTDIR.y = 0.0f;
	fDist = D3DXVec3Length(&vTDIR);

	if( fDist == 0.0f )
		return FALSE;
	vTDIR /= fDist;

	D3DXPlaneFromPointNormal(
		&vTLAND,
		&vTSTART,
		&vTDIR);

	D3DXPlaneNormalize(
		&vTLAND,
		&vTLAND);

	if( D3DXPlaneDotCoord( &vTLAND, &vP) < 0.0f &&
		D3DXPlaneDotCoord( &vTLAND, &vO) < 0.0f )
		return FALSE;

	vTLAND = vTPLANE;
	vTLAND.d += D3DXPlaneDotNormal( &vTPLANE, &vTDIR) > 0.0f ? fRadius : -fRadius;

	if(!D3DXPlaneIntersectLine( &vPoint, &vTLAND, &vTSTART, &vTEND))
		return FALSE;

	D3DXPlaneFromPointNormal(
		&vTNORMAL,
		&vPoint,
		&D3DXVECTOR3(
		vTNORMAL.a,
		vTNORMAL.b,
		vTNORMAL.c));

	D3DXPlaneNormalize(
		&vTNORMAL,
		&vTNORMAL);

	fDotP = D3DXPlaneDotCoord( &vTNORMAL, &vP);
	fDotO = D3DXPlaneDotCoord( &vTNORMAL, &vO);

	if( fDotP * fDotO > 0.0f )
	{
		fDotP = fabs(fDotP);
		fDotO = fabs(fDotO);

		if( fDotO < fDotP )
			vP = vO;

		if(!CTMath::GetProjectionPoint( &vO, &vP, &vTSTART, &vTEND))
			return FALSE;
		vO.y = 0.0f;

		fDotP = D3DXVec3LengthSq(&(vP - vO));
		fDotO = fRadius * fRadius;

		if( fDotP > fDotO )
			return FALSE;

		vTDIR = vTSTART - vO;
		vTDIR.y = 0.0f;
		fDist = D3DXVec3Length(&vTDIR);

		if( fDist == 0.0f )
			return FALSE;
		vTDIR /= fDist;

		fDotO -= fDotP;
		fDotO = sqrt(fDotO);

		vO = vO + fDotO * vTDIR;
		vO -= vP;
		vO /= fRadius;

		D3DXPlaneFromPointNormal(
			&vTPLANE,
			&vP, &vO);

		D3DXPlaneNormalize(
			&vTPLANE,
			&vTPLANE);
	}

	if(!CTMath::CheckCollision( &vTPLANE, &vTSTART, &vTEND, fRadius, pDIST))
		return FALSE;

	pTSTART->x = vTSTART.x;
	pTSTART->z = vTSTART.z;

	pTEND->x = vTEND.x;
	pTEND->z = vTEND.z;
	(*pDIST) += fREV;

	return TRUE;
}

BYTE CTClientObjBase::CheckAction( DWORD dwAction,
								   DWORD dwAni)
{
	MAPACTION::iterator finder = m_OBJ.m_pOBJ->m_mapACTION.find(dwAction);

	if( finder != m_OBJ.m_pOBJ->m_mapACTION.end() )
	{
		LPACTION pTACT = (*finder).second;
		MAPANI::iterator itANI = pTACT->m_mapANI.find(dwAni);

		if( itANI != pTACT->m_mapANI.end() )
			return TRUE;
	}

	return FALSE;
}

BYTE CTClientObjBase::IsDisguise()
{
	return m_bDisguise != (BYTE) T_INVALID ? TRUE : FALSE;
}

OBJHIT CTClientObjBase::HitTest( CTClientCAM *pCamera,
								 int nPosX,
								 int nPosY,
								 float fRadScale)
{
	static int vINDEX[6][4] = {
		{ 0, 1, 2, 3},
		{ 7, 6, 5, 4},
		{ 0, 4, 5, 1},
		{ 1, 5, 6, 2},
		{ 2, 6, 7, 3},
		{ 3, 7, 4, 0}};

	D3DXVECTOR3 vPos = pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDir = pCamera->GetRayDirection( nPosX, nPosY);

	OBJHIT vHIT;

	float fRadH = m_fRadiusH * fRadScale;
	float fSizeX = m_fSizeX * fRadScale;
	float fSizeY = m_fSizeY * fRadScale;
	float fSizeZ = m_fSizeZ * fRadScale;
	float fDepth = m_fDepth * fRadScale;

	if(!D3DXSphereBoundProbe(
		&D3DXVECTOR3(
		m_vPosition._41,
		m_vPosition._42,
		m_vPosition._43),
		fRadH,
		&vPos, &vDir))
	{
		return vHIT;
	}

	D3DXVECTOR3 vPOINT[8] = {
		D3DXVECTOR3( -fSizeX / 2.0f,   -fDepth / 2.0f,  -fSizeZ / 2.0f),
		D3DXVECTOR3(  fSizeX / 2.0f,   -fDepth / 2.0f,  -fSizeZ / 2.0f),
		D3DXVECTOR3(  fSizeX / 2.0f,   -fDepth / 2.0f,   fSizeZ / 2.0f),
		D3DXVECTOR3( -fSizeX / 2.0f,   -fDepth / 2.0f,   fSizeZ / 2.0f),
		D3DXVECTOR3( -fSizeX / 2.0f,	fSizeY / 2.0f,  -fSizeZ / 2.0f),
		D3DXVECTOR3(  fSizeX / 2.0f,	fSizeY / 2.0f,  -fSizeZ / 2.0f),
		D3DXVECTOR3(  fSizeX / 2.0f,	fSizeY / 2.0f,   fSizeZ / 2.0f),
		D3DXVECTOR3( -fSizeX / 2.0f,	fSizeY / 2.0f,   fSizeZ / 2.0f)};

	D3DXVECTOR3 vPOS(
		m_vPosition._41,
		m_vPosition._42,
		m_vPosition._43);

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
		D3DXPLANE vPLANE;

		D3DXPlaneFromPoints(
			&vPLANE,
			&vPOINT[vINDEX[i][0]],
			&vPOINT[vINDEX[i][1]],
			&vPOINT[vINDEX[i][2]]);

		if( D3DXPlaneDotCoord( &vPLANE, &vPos) > 0.0f )
		{
			static int vPOLY[2][3] = {
				{ 0, 1, 2},
				{ 0, 2, 3}};

			for( int j=0; j<2; j++)
			{
				FLOAT fCurDist = 0.0f;
				FLOAT fU;
				FLOAT fV;

				if( D3DXIntersectTri(
					&vPOINT[vINDEX[i][vPOLY[j][0]]],
					&vPOINT[vINDEX[i][vPOLY[j][1]]],
					&vPOINT[vINDEX[i][vPOLY[j][2]]],
					&vPos, &vDir,
					&fU, &fV,
					&fCurDist) && (
					vHIT.m_nIndex == -1 ||
					fCurDist < vHIT.m_fDist))
				{
					vHIT.m_fDist = fCurDist;
					vHIT.m_nIndex = 0;
				}
			}
		}
	}

	return vHIT;
}

BYTE CTClientObjBase::HitTest( LPD3DXVECTOR3 pSTART,
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

BYTE CTClientObjBase::HitCylinder( LPD3DXVECTOR3 pSTART,
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

BYTE CTClientObjBase::HitBox( LPD3DXVECTOR3 pSTART,
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

		if( D3DXPlaneDotCoord( &vWINDING.m_vPlane, pSTART) > 0.0f &&
			D3DXPlaneDotCoord( &vWINDING.m_vPlane, pEND) < 0.0f &&
			CTMath::IsCut( &vPlaneV, &vWINDING) && CTMath::IsCut( &vPlaneH, &vWINDING) )
		{
			vWINDING.m_vPoint.clear();
			return TRUE;
		}

		vWINDING.m_vPoint.clear();
	}

	return FALSE;
}

BYTE CTClientObjBase::HitPoly( FLOAT *pDIST,
							   LPD3DXVECTOR3 pSTART,
							   LPD3DXVECTOR3 pEND,
							   FLOAT fLineH,
							   FLOAT fLineV)
{
	D3DXVECTOR3 vDIR = (*pEND) - (*pSTART);
	FLOAT fLength = D3DXVec3Length(&vDIR);

	LPD3DXMATRIX pMESHMAT = GetMeshMatrix();
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
					{
						LPIBINFO pIBINFO = pMESH->m_pMESH[i][0];

						for( int j=0; j<INT(pIBINFO->m_vIB.size()); j++)
						{
							LPIBDATA pIBDATA = pIBINFO->m_vIB[j];

							for( int k=0; k<pIBDATA->m_dwCount / 3; k++)
							{
								D3DXVECTOR3 vPOLY[3];
								FLOAT fLocal = 0.0f;

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
								FLOAT fLocal = 0.0f;

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
		}
	}

	return bResult;
}

void CTClientObjBase::PlayRandomSND( LPMAPSNDINST pINST,
									 LPMAPOBJSND pSND,
									 DWORD dwFuncID,
									 BYTE b3DSound)
{
	if( dwFuncID == 0 )
		return ;

	MAPSNDINST::iterator it;

	VECTORDWORD vTSNDID;
	VSNDINST vTSNDINST;

	vTSNDINST.clear();
	vTSNDID.clear();

	for( it = pINST->begin(); it != pINST->end(); it++)
		if( (*it).second->m_dwFuncID == dwFuncID )
		{
			vTSNDINST.push_back((*it).second);
			vTSNDID.push_back((*it).first);
		}

	if(!vTSNDINST.empty())
	{
		int nRand = rand() % INT(vTSNDID.size());

		for( int i=0; i<INT(vTSNDID.size()); i++)
		{
			MAPOBJSND::iterator finder = pSND->find(vTSNDID[i]);

			if( finder != pSND->end() )
			{
				CD3DSound *pWAV = (CD3DSound *) m_pMedia->GetData(
					MEDIA_TSOUND,
					(*finder).second->m_pSND->m_dwMediaID);

				if(pWAV)
				{
					pWAV->Stop((*finder).second->m_nIndex);
					pWAV->Unlock((*finder).second->m_nIndex);
				}

				delete (*finder).second;
				pSND->erase(finder);
			}
		}

		CD3DSound *pWAV = (CD3DSound *) m_pMedia->GetData(
			MEDIA_TSOUND,
			vTSNDINST[nRand]->m_dwMediaID);
		LPOBJSND pItem = new OBJSND();

		pItem->m_nIndex = pWAV->Lock();
		pItem->m_pSND = vTSNDINST[nRand];

		if( pItem->m_nIndex >= 0 )
		{
			if(b3DSound)
			{
				LPDIRECTSOUND3DBUFFER p3DBUF = pWAV->GetDS3D(pItem->m_nIndex);

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
			}

			m_pMedia->Play(
				MEDIA_TSOUND,
				vTSNDINST[nRand]->m_dwMediaID,
				pItem->m_nIndex);

			pSND->insert( MAPOBJSND::value_type(
				vTSNDID[nRand],
				pItem));
		}
		else
			delete pItem;
	}
}

void CTClientObjBase::PlaySND( DWORD dwSndID,
							   BYTE bType,
							   BYTE b3DSound)
{
	switch(bType)
	{
	case MEDIA_TSOUND	:
		{
			CD3DSound *pWAV = (CD3DSound *) m_pMedia->GetData(
				MEDIA_TSOUND,
				dwSndID);
			int nIndex = pWAV->Lock();

			if( nIndex >= 0 )
			{
				if(b3DSound)
				{
					LPDIRECTSOUND3DBUFFER p3DBUF = pWAV->GetDS3D(nIndex);

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
				}

				pWAV->Play(nIndex);
				pWAV->Unlock(nIndex);
			}
		}

		break;
	}
}

void CTClientObjBase::EraseTQuest( DWORD dwQuestID)
{
	MAPTQCLASS::iterator it;

	for( it = m_mapTQCLASS.begin(); it != m_mapTQCLASS.end(); it++)
	{
		MAPTQUEST::iterator finder = (*it).second->m_mapTQUEST.find(dwQuestID);

		if( finder != (*it).second->m_mapTQUEST.end() )
		{
			delete (*finder).second;
			(*it).second->m_mapTQUEST.erase(finder);

			if((*it).second->m_mapTQUEST.empty())
			{
				delete (*it).second;
				m_mapTQCLASS.erase(it);
			}

			break;
		}
	}
}
void CTClientObjBase::ApplyGradeEffect(CTachyonObject* pItemObj, 
									  CTClientItem* pTITEM)
{
	// 텍스처효과
	LPTITEMGRADEVISUAL pGradeVISUAL = pTITEM->GetGradeVISUAL();
	if( pGradeVISUAL && pGradeVISUAL->m_pSkinTex)
	{
		LPTEXTURESET pSkinTEX = pGradeVISUAL->m_pSkinTex;
		BYTE bPSTYPE = pGradeVISUAL->m_bPSTYPE;

		MAPOBJPART::iterator itr,end;
		itr = pItemObj->m_OBJ.m_mapDRAW.begin();
		end = pItemObj->m_OBJ.m_mapDRAW.end();

		for(; itr!=end; ++itr)
		{
			VECTOROBJPART* pDRAW = itr->second;

			for( int i=0; i<INT(pDRAW->size()); ++i)
			{
				LPOBJPART pPART = (*pDRAW)[i];
				pPART->m_pTEX = CTClientObjBase::NewGradeObjTex(
					pPART->m_pTEX,
					TT_TEX,
					0,
					pSkinTEX,
					bPSTYPE);
			}
		}

		return ; // 텍스처효과가 있으면 컬러효과는 무시.
	}

	// 컬러효과
	/*DWORD dwGradeColor = pTITEM->GetVisualColor();
	if( !dwGradeColor )
		return;

	MAPOBJPART::iterator itr,end;
	itr = pItemObj->m_OBJ.m_mapDRAW.begin();
	end = pItemObj->m_OBJ.m_mapDRAW.end();

	for(; itr!=end; ++itr)
	{
		VECTOROBJPART* pDRAW = itr->second;

		for( int i=0; i<INT(pDRAW->size()); ++i)
		{
			LPOBJPART pPART = (*pDRAW)[i];
			pPART->m_pTEX = CTClientObjBase::NewGradeObjTex(
				pPART->m_pTEX,
				TT_COLOR,
				dwGradeColor,
				NULL,
				0);
		}
	}*/
}

void CTClientObjBase::ApplyGradeEffect( CTClientItem** pApplyGradeList, INT nCount)
{
	for( INT i=0;i < nCount ; ++i )
	{
		if( !pApplyGradeList[i] )
			continue;

		CTClientItem *pTITEM = pApplyGradeList[i];
		LPTITEMVISUAL pTVISUAL =  pTITEM->GetVisual();
		LPTITEMGRADEVISUAL pGradeVISUAL = pTITEM->GetGradeVISUAL();

		if( pTVISUAL &&
			pTVISUAL->m_dwCLKID &&
			pGradeVISUAL)
		{
			LPTEXTURESET pSkinTEX = pGradeVISUAL->m_pSkinTex;
			BYTE bPSTYPE = pGradeVISUAL->m_bPSTYPE;

			MAPCLKINST::iterator itCI = m_OBJ.m_mapCLK.find( pTVISUAL->m_dwCLKID);
			if( itCI == m_OBJ.m_mapCLK.end() )
				continue;

			LPMESH pMESH2 = (*itCI).second->m_pMESH;
			if( !pMESH2 )
				continue;

			CTachyonMesh* pMESH = pMESH2->m_pMESH;
			if( !pMESH )
				continue;

			MAPOBJPART::iterator itDRAW, endDRAW;
			itDRAW = m_OBJ.m_mapDRAW.begin();
			endDRAW = m_OBJ.m_mapDRAW.end();
			for(; itDRAW != endDRAW ; ++itDRAW)
			{
				VECTOROBJPART *pDRAW = itDRAW->second;

				if( !pDRAW )
					continue;

				VECTOROBJPART::iterator itPART, endPART;
				itPART = pDRAW->begin();
				endPART = pDRAW->end();

				for(; itPART != endPART ; ++itPART)
				{
					LPOBJPART pPART = (*itPART);

					if( pPART &&
						pPART->m_pMESH == pMESH)
					{
						pPART->m_pTEX = CTClientObjBase::NewGradeObjTex(
							pPART->m_pTEX,
							TT_TEX,
							0,
							pSkinTEX,
							bPSTYPE);
					}
				}
			}
		}
	}
}

void CTClientObjBase::InsertTQuest( CTClientQuest *pTQuest)
{
	CTClientQClass *pTQClass = FindTQClass(pTQuest->m_pTQUEST->m_dwClassID);

	if(!pTQClass)
	{
		pTQClass = new CTClientQClass();
		pTQClass->m_pTQCLASS = CTChart::FindTQCLASS( pTQuest->m_pTQUEST->m_dwClassID);

		m_mapTQCLASS.insert( MAPTQCLASS::value_type( pTQClass->m_pTQCLASS->m_dwClassID, pTQClass));
	}

	pTQClass->m_mapTQUEST.insert( MAPTQUEST::value_type( pTQuest->m_pTQUEST->m_dwID, pTQuest));
}

BYTE CTClientObjBase::IsInBattle()
{
	BYTE bContryID = m_pTREGION ? m_pTREGION->m_bContryID : TCONTRY_N;
	return bContryID == TCONTRY_PEACE || (bContryID != TCONTRY_N && m_bContryID == bContryID && !m_bInBattle) ? FALSE : TRUE;
}

BYTE CTClientObjBase::IsReadyForUSE( CTClientSkill *pTSKILL)
{
	return pTSKILL && !pTSKILL->m_bTimerON && !pTSKILL->m_dwExceptTick && !m_bActionLock ? TRUE : FALSE;
}

BYTE CTClientObjBase::IsAlliance( CTClientObjBase *pTARGET)
{
	return m_bContryID == pTARGET->m_bContryID || (
		pTARGET->m_bAllianceID != TCONTRY_N &&
		m_bContryID == pTARGET->m_bAllianceID ) || (
		m_bAllianceID != TCONTRY_N && (
		m_bAllianceID == pTARGET->m_bAllianceID ||
		m_bAllianceID == pTARGET->m_bContryID )) ? TRUE : FALSE;
}

void CTClientObjBase::SetHide(BOOL bHide)
{
	m_bHide = bHide;
	CTClientGame::GetInstance()->NotifyObjHide(this);
}

void CTClientObjBase::CancelHiding( CTClientGame* pTGAME )
{
	CTClientMaintain* pTMAINTAIN = FindMaintain( TID_HIDING_SKILL );
	if( pTMAINTAIN )
	{
		pTGAME->SendCS_SKILLEND_REQ(
			m_dwID,
			m_bType,
			GetHostID(),
			pTMAINTAIN->m_dwAttackID,
			pTMAINTAIN->m_bAttackType,
			pTMAINTAIN->m_pTSKILL->m_wSkillID,
			pTGAME->GetClientMap()->m_wMapID,
			pTGAME->GetMainWnd()->m_bChannel);
	}
}

void CTClientObjBase::CancelMaintainByFunction( CTClientGame* pTGAME, DWORD* dwFunction, DWORD dwCount )
{
	MAPTMAINTAIN::iterator it, end;
	it = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();
	for(; it != end ; ++it )
	{
		CTClientMaintain* pMaintain = (*it).second;
		LPTSKILL pTSKILL = pMaintain->m_pTSKILL;

		for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[i];
			if( pTSKILLFUNC->m_bType == SDT_STATUS )
			{
				for( DWORD m=0 ; m < dwCount ; ++m )
				{
					if( pTSKILLFUNC->m_bFunctionID == dwFunction[m] )
					{
						pTGAME->SendCS_SKILLEND_REQ(
							m_dwID,
							m_bType,
							GetHostID(),
							pMaintain->m_dwAttackID,
							pMaintain->m_bAttackType,
							pMaintain->m_pTSKILL->m_wSkillID,
							pTGAME->GetClientMap()->m_wMapID,
							pTGAME->GetMainWnd()->m_bChannel);
					}
				}
			}
		}
	}
}

UINT CTClientObjBase::CountMaintain(WORD wSkillID, UINT nLimit)
{
	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;
		if( pCMAINTAIN->m_pTSKILL->m_wSkillID == wSkillID )
		{
			++nCNT;

			if( nCNT == nLimit )
				break;
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountMaintainFunc( BYTE bFuncType, BYTE bFuncID, UINT nLimit)
{
	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == bFuncType &&
				pSKILLFUNC->m_bFunctionID == bFuncID )
			{
				++nCNT;

				if( nCNT == nLimit )
					break;
			}
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountBlockMaintain(UINT nLimit)
{
	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == SDT_STATUS &&
				(pSKILLFUNC->m_bFunctionID == SDT_STATUS_BLOCK ||
				 pSKILLFUNC->m_bFunctionID == SDT_STATUS_HYPNOSIS) )
			{
				++nCNT;

				if( nCNT == nLimit )
					break;
			}
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountFlashMaintain(UINT nLimit)
{
	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == SDT_STATUS &&
				(pSKILLFUNC->m_bFunctionID == SDT_STATUS_DARK ||
				 pSKILLFUNC->m_bFunctionID == SDT_STATUS_FLASH) )
			{
				++nCNT;

				if( nCNT == nLimit )
					break;
			}
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountHideMaintain(UINT nLimit)
{
	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == SDT_STATUS && (
				pSKILLFUNC->m_bFunctionID == SDT_STATUS_CLARITY ||
				pSKILLFUNC->m_bFunctionID == SDT_STATUS_HIDE) )
			{
				++nCNT;

				if( nCNT == nLimit )
					break;
			}
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountPriceUpMaintain(
		INT* pValue )
{
	if( pValue )
		*pValue = 0;

	UINT nCNT = 0;

	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == SDT_STATUS &&
				(pSKILLFUNC->m_bFunctionID == SDT_STATUS_PRICEUP))
			{
				++nCNT;

				if( pValue )
					*pValue += pSKILLFUNC->m_wValue;
			}
		}
	}

	return nCNT;
}

UINT CTClientObjBase::CountCanNotUsePetMaintain()
{
	INT nCount = 0;

	MAPTMAINTAIN::iterator it, end;
	it = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; it != end; ++it )
	{
		LPTSKILL pTSKILL = it->second->m_pTIMMER;
		if( pTSKILL && !pTSKILL->m_bIsRide )
			++nCount;
	}

	return nCount;
}

UINT CTClientObjBase::CountHideDetectMaintain()
{
	MAPTMAINTAIN::iterator itr,end;
	itr = m_mapTMAINTAIN.begin();
	end = m_mapTMAINTAIN.end();

	for(; itr!=end; ++itr)
	{
		CTClientMaintain* pCMAINTAIN = itr->second;

		for( int i=0; i<INT(pCMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
		{
			LPTSKILLFUNCTION pSKILLFUNC = pCMAINTAIN->m_pTSKILL->m_vTFUNCTION[i];
			if( pSKILLFUNC->m_bType == SDT_STATUS &&
				pSKILLFUNC->m_bFunctionID == SDT_STATUS_HIDEDETECT )
			{
				return 1;
			}
		}
	}

	return 0;
}

void CTClientObjBase::OnMaintainEnd( CD3DDevice *pDevice,
									 CTachyonRes *pRES,
									 CTClientSkill *pTSKILL)
{
	for( int i=0; i<INT(pTSKILL->m_pTSKILL->m_vTFUNCTION.size()); i++)
	{
		LPTSKILLFUNCTION pSKILLFUNC = pTSKILL->m_pTSKILL->m_vTFUNCTION[i];

		BYTE bType = pSKILLFUNC->m_bType;
		BYTE bFuncID = pSKILLFUNC->m_bFunctionID;

		switch(bType)
		{
		case SDT_STATUS		:
			switch(bFuncID)
			{
			case SDT_STATUS_ATKRANGE	: m_fRangeFactor = 1.0f; break;
			case SDT_STATUS_HYPNOSIS	:
			case SDT_STATUS_BLOCK		:
				if( CountBlockMaintain(2) < 2 )
				{
					m_bLastACT = m_bIsHolded ? TA_HOLD : TA_STAND;
					m_bIsStuned = FALSE;

					m_bAction = GetTAction();
					m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
				}

				break;

			case SDT_STATUS_HOLD		:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
				{
					m_bLastACT = m_bIsStuned ? TA_STUN : TA_STAND;
					m_bIsHolded = FALSE;

					m_bAction = GetTAction();
					m_bACTLevel = CTChart::GetTACTION( m_bAction )->m_bLevel;
				}

				break;

			case SDT_STATUS_SEEKMINE	: m_wTDETECT &= ~(1 << OT_SELF); break;
			case SDT_STATUS_HIDE		:
				switch(pTSKILL->m_pTSKILL->m_vTFUNCTION[i]->m_bOPCode)
				{
				case SVI_DECREASE	: m_wTDETECT &= ~(1 << OT_PC); break;
				case SVI_INCREASE	:
					if( CountHideMaintain(2) < 2 )
						SetHide(FALSE);

					break;
				}

				break;

			case SDT_STATUS_HIDEDETECT	:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
					m_wTDETECT &= ~(1 << OT_PC);

				break;

			case SDT_STATUS_CLARITY		: 
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
				{
					m_bClarity = FALSE;
					if( CountHideMaintain(2) < 2 )
						SetHide(FALSE);
				}

				break;

			case SDT_STATUS_STANDHIDE	:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
					m_bStandHide = FALSE;

				break;

			case SDT_STATUS_BATTLE		: m_bInBattle = FALSE; break;
			case SDT_STATUS_MARK		: m_wPursuit = 0xFFFF; break;

			case SDT_STATUS_SILENCE		:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
					m_bSkillSilence = FALSE;

				break;

			case SDT_STATUS_MERMAID		: m_bMermaid = FALSE; break;
			case SDT_STATUS_DARK		:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
					if( CountFlashMaintain(2) < 2 )
						m_bFlash = FALSE;
					else
						m_dwFlashColor = TFLASHCOLOR_WHITE;

				break;

			case SDT_STATUS_FLASH		:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
					if( CountFlashMaintain(2) < 2 )
						m_bFlash = FALSE;
					else
						m_dwFlashColor = TFLASHCOLOR_BLACK;

				break;

			case SDT_STATUS_NOTSELECT	: m_bSelectable = TRUE; break;
			case SDT_STATUS_PARACHUTE	:
				{
					D3DXVECTOR2 vDIR( 0.0f, 0.0f);
					BYTE bFall = FALSE;

					m_bParashute = FALSE;
					if(IsFall())
					{
						vDIR = m_vFallDIR;
						bFall = TRUE;
					}
					else if( IsJump() && FLOAT(m_dwJumpTick) / 1000.0f > m_fJumpSpeed / CTClientMAP::m_fGravity )
					{
						vDIR = m_vJumpDIR;
						bFall = TRUE;
					}
					else if( IsPush() && FLOAT(m_dwPushTick) / 1000.0f > m_fPushSpeed / (TPUSH_BUST * CTClientMAP::m_fGravity) )
					{
						vDIR = m_vPushDIR;
						bFall = TRUE;
					}

					if(bFall)
					{
						TACTION vActionID = FindActionID(
							TA_FALL,
							GetWeaponID(m_bMode));
						SetTAction(TA_FALL);

						m_bACTLevel = CTChart::GetTACTION( TA_FALL )->m_bLevel;
						SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);

						if( vDIR.x != 0.0f || vDIR.y != 0.0f )
							vDIR /= D3DXVec2Length(&vDIR);

						m_fFallHeight = m_vPosition._42;
						m_dwFallTick = 0;
						m_vFallDIR = max( TMINFALL_SPEED, m_fMoveSpeed) * vDIR;
					}
				}

				break;
			case SDT_STATUS_DOUBLEHEAD:
				{
					m_bDoubleHead = FALSE;
					SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );
				}
				break;
			}

			break;

		case SDT_TRANS		:		
			switch(bFuncID)
			{
				case SDT_TRANS_DISGUISE_D	:  
				case SDT_TRANS_DISGUISE_C	: 
				case SDT_TRANS_DISGUISE_B	: EndDISGUISE(pDevice, pRES); break;
				default						: DoRETRANS( pDevice, pRES); break;
			}

		case SDT_AI			:
			switch(bFuncID)
			{
			case SDT_RUNAWAY				:
				if( CountMaintainFunc( bType, bFuncID, 2) < 2 )
				{
					CTClientGame* pGame = CTClientGame::GetInstance();
					if( this == pGame->GetMainChar() )
						pGame->DoMainRunAway(FALSE);
					else if( this == pGame->GetMainRecall() )
						pGame->DoRecallRunAway( (CTClientRecall*) this,FALSE);
				}
				break;
			}
			break;
		}
	}

	PlayTActionSFX( &m_vTGARBAGESFX, pTSKILL->m_pTSKILL->m_dwSFX[TSKILLSFX_END]);
	ClearTActionSFX(&pTSKILL->m_vTSFX);
}

WORD CTClientObjBase::GetTargetPITCH( FLOAT fTargetX,
									  FLOAT fTargetY,
									  FLOAT fTargetZ)
{
	return 0;
}

WORD CTClientObjBase::GetTargetDIR( FLOAT fTargetX,
								    FLOAT fTargetZ)
{
	return 1800;
}

void CTClientObjBase::CalcDIR( WORD wMoveDIR,
							   WORD wDIR,
							   WORD wPITCH)
{
}

void CTClientObjBase::FinishSUBACT()
{
	if( !m_pHost || m_bType != OT_PC )
		return;
	LPANI pANI = m_vTSUBACT.m_pANI;

	if( pANI && pANI->m_pANI )
	{
		OBJMSG msg;

		m_fSubActTime = pANI->m_pANI->GetTotalTime();
		pANI->m_pANI->m_fCurTime = m_fSubActTime;

		if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
			msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;
		msg.m_pOBJ = (DWORD_PTR) this;

		if(!pANI->m_pANI->m_vKey.empty())
		{
			pANI->m_pANI->m_nKeyBase = INT(pANI->m_pANI->m_vKey.size()) - 1;

			while( m_nSubKeyBase < pANI->m_pANI->m_nKeyBase )
			{
				m_nSubKeyBase++;

				if( pANI->m_pANI->m_vKey[m_nSubKeyBase]->m_dwEventID > 0 )
				{
					msg.m_dwMSG = pANI->m_pANI->m_vKey[m_nSubKeyBase]->m_dwEventID;
					m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);
				}
			}
		}

		msg.m_dwMSG = OM_TSUBACTENDING;
		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

		msg.m_dwMSG = OM_TSUBACTENDED;
		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

		SetSubAction( 0, 0);
		m_bSubActEnd = FALSE;
	}
}

void CTClientObjBase::FinishACT()
{
	if( !m_pHost || (
		m_bType != OT_RECALL &&
		m_bType != OT_SELF &&
		m_bType != OT_PC) )
		return;
	LPANI pANI = GetCurANI();

	if( pANI && pANI->m_pANI )
	{
		OBJMSG msg;

		m_fActTime = pANI->m_pANI->GetTotalTime();
		pANI->m_pANI->m_fCurTime = m_fActTime;

		if( pANI->m_pANI->m_bLoop && pANI->m_pANI->m_dwLoopID )
			msg.m_pPARAM = (DWORD_PTR) pANI->m_pANI->m_dwLoopID;
		msg.m_pOBJ = (DWORD_PTR) this;

		if(!pANI->m_pANI->m_vKey.empty())
		{
			pANI->m_pANI->m_nKeyBase = INT(pANI->m_pANI->m_vKey.size()) - 1;

			while( m_nKeyBase < pANI->m_pANI->m_nKeyBase )
			{
				m_nKeyBase++;

				if( pANI->m_pANI->m_vKey[m_nKeyBase]->m_dwEventID > 0 )
				{
					msg.m_dwMSG = pANI->m_pANI->m_vKey[m_nKeyBase]->m_dwEventID;
					m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);
				}
			}
		}

		msg.m_dwMSG = OM_ACTENDING;
		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

		msg.m_dwMSG = OM_ACTENDED;
		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

		SetAction( 0, 0);
	}
}

BYTE CTClientObjBase::ForecastHeight( CTClientMAP *pTMAP,
									  FLOAT *pTHEIGHT,
									  FLOAT *pTWATER,
									  DWORD dwTick)
{
	FLOAT fWaterHeight = pTMAP->GetWaterHeight(
		m_vPosition._41,
		m_vPosition._43) - m_fBreathHeight;
	BYTE bResult = FALSE;

	FLOAT fHeight;
	FLOAT fTime;

	if(IsPush())
	{
		fTime = FLOAT(m_dwPushTick) / 1000.0f;
		fHeight = m_fPushHeight + m_fPushSpeed * fTime - TPUSH_BUST * CTClientMAP::m_fGravity * fTime * fTime / 2.0f;

		if( !IsFlyOBJ() || (m_bSwim && fHeight > fWaterHeight) || fTime < m_fPushSpeed / (TPUSH_BUST * CTClientMAP::m_fGravity) )
			bResult = TRUE;
	}
	else if(IsFall())
	{
		fTime = FLOAT(m_dwFallTick) / 1000.0f;
		fHeight = m_fFallHeight - CTClientMAP::m_fGravity * fTime * fTime / 2.0f;

		if( !IsFlyOBJ() || (m_bSwim && fHeight > fWaterHeight) )
			bResult = TRUE;
	}
	else if(IsJump())
	{
		fTime = FLOAT(m_dwJumpTick) / 1000.0f;
		fHeight = m_fJumpHeight + m_fJumpSpeed * fTime - CTClientMAP::m_fGravity * fTime * fTime / 2.0f;

		if( !IsFlyOBJ() || (m_bSwim && fHeight > fWaterHeight) || fTime < m_fJumpSpeed / CTClientMAP::m_fGravity )
			bResult = TRUE;
	}

	if(bResult)
	{
		if( m_bParashute && fHeight < m_vPosition._42 )
			fHeight = max( fHeight, m_vPosition._42 - FLOAT(dwTick) * TPARASHUTE_SPEED / 1000.0f);

		if(pTHEIGHT)
			(*pTHEIGHT) = fHeight;
	}

	if(pTWATER)
		(*pTWATER) = fWaterHeight;

	return bResult;
}

void CTClientObjBase::CalcTick( LPDIRECT3DDEVICE9 pDevice,
							    DWORD dwTick)
{
	CTachyonObject::CalcTick(
		pDevice,
		m_wMoveDIR != MOVE_NONE ? DWORD(m_fSpeedFactor * FLOAT(dwTick)) : dwTick);

	if(m_bRootID)
		CalcSUBTick( pDevice, dwTick);
	m_fSquareHeight = m_vPosition._42;

	if(m_vTMARKER.m_OBJ.m_pOBJ)
	{
		m_vTMARKER.m_vWorld._41 = m_vPosition._41;
		m_vTMARKER.m_vWorld._42 = m_vPosition._42 + m_fSizeY + TMARKER_HEIGHT;
		m_vTMARKER.m_vWorld._43 = m_vPosition._43;

		m_vTMARKER.CalcTick( pDevice, dwTick);
	}

	if(m_bDrawTalk)
		if( m_dwTalkTick > TTALK_TIMER )
		{
			m_bDrawTalk = FALSE;
			m_dwTalkTick = 0;
		}
		else
			m_dwTalkTick += dwTick;

	for( int i=0; i<TSLASHSFX_COUNT; i++)
		m_vTSLASH[i].CalcTick(dwTick);

	for( i=0; i<TNUMBERPOS_COUNT; i++)
		m_vDamageTick[i] -= min( m_vDamageTick[i], dwTick);

	if(m_bSubActEnd)
	{
		SetSubAction( 0, 0);
		m_bSubActEnd = FALSE;
	}
}

void CTClientObjBase::CalcSUBTick( LPDIRECT3DDEVICE9 pDevice, DWORD dwTick)
{
	LPANI pANI = m_vTSUBACT.m_pANI;

	if( pANI && pANI->m_pANI )
	{
		FLOAT fTotal = pANI->m_pANI->GetTotalTime();
		ASSERT(fTotal>0.0f && "Invalid sub animation.");

		m_fSubActTime += CTMath::GetTimeValue(dwTick);

		if( m_fSubActTime >= fTotal )
		{
			FLOAT fTime = m_fSubActTime;

			CTachyonObject::ClearSFX( &m_vTSUBACTSFX, &m_mapTSUBACTSFX);
			CTachyonObject::ClearSFX( &m_vTSUBANISFX, &m_mapTSUBANISFX);
			CTachyonObject::ResetSND( &m_mapTSUBACTSND, FALSE);
			CTachyonObject::ResetSND( &m_mapTSUBANISND, FALSE);

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
		}
		CalcSUBFrame();

		GenerateSFX( &m_mapTSUBACTSFXINST, &m_vTSUBACTSFX, &m_mapTSUBACTSFX, m_fSubActTime, m_dwACTSFXFuncID);
		GenerateSFX( &m_mapTSUBANISFXINST, &m_vTSUBANISFX, &m_mapTSUBANISFX, m_fSubActTime, m_dwANISFXFuncID);

		GenerateSND( &m_mapTSUBACTSNDINST, &m_mapTSUBACTSND, m_fSubActTime, m_dwACTSNDFuncID);
		GenerateSND( &m_mapTSUBANISNDINST, &m_mapTSUBANISND, m_fSubActTime, m_dwACTSNDFuncID);
	}
}

void CTClientObjBase::CalcSUBFrame()
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

void CTClientObjBase::ApplyMatrix( CD3DDevice *pDevice)
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
					D3DXMATRIXA16 matScale;
					D3DXMatrixScaling( &matScale, 2.0f, 2.0f, 2.0f );

					pDevice->m_pDevice->SetTransform( D3DTS_WORLDMATRIX(0), &m_pBone[0]);

					for( int i=0; i<pDATA->m_pAni->GetNodeCount(); i++)
						pDevice->m_pDevice->SetTransform( D3DTS_WORLDMATRIX(i + 1), pInit ? &(pInit[i] * m_pBone[i + 1]) : &(matScale*m_pBone[i + 1]) );
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

void CTClientObjBase::DeleteOBJ( CTClientObjBase *pTOBJ)
{
	m_vTGARBAGE.push_back(pTOBJ);
	pTOBJ->m_bGARBAGE = TRUE;
}

void CTClientObjBase::ClearGARBAGE()
{
	while(!m_vTGARBAGE.empty())
	{
		delete m_vTGARBAGE.back();
		m_vTGARBAGE.pop_back();
	}
}

void CTClientObjBase::InitSquareResource( CTachyonRes *pRES)
{
	static PVERTEX vPOINT[4] = {
		{ -0.5f, 0.0f, -0.5f, 0.0f, 0.0f},
		{ -0.5f, 0.0f,  0.5f, 0.0f, 1.0f},
		{  0.5f, 0.0f, -0.5f, 1.0f, 0.0f},
		{  0.5f, 0.0f,  0.5f, 1.0f, 1.0f}};

	ReleaseSquareResource();
	m_vTSQUARE.LoadT3DVB(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX);

	m_vTSQUARE.LoadT3DVBDATA(
		(LPBYTE) vPOINT,
		4 * sizeof(PVERTEX), 0);

	MAPRES::iterator finder = pRES->m_mapTEX.find(ID_TEX_SHADOW);
	if( finder != pRES->m_mapTEX.end() )
		m_pTSHADOW = (LPTEXTURESET) (*finder).second;

	finder = pRES->m_mapTEX.find(ID_TEX_SELECT);
	if( finder != pRES->m_mapTEX.end() )
		m_pTSELECT = (LPTEXTURESET) (*finder).second;
}

void CTClientObjBase::ReleaseSquareResource()
{
	m_vTSQUARE.ReleaseDATA();
	m_pTSHADOW = NULL;
	m_pTSELECT = NULL;
}

void CTClientObjBase::BeginDrawSquare( CD3DDevice *pDevice)
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

	pDevice->m_pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	pDevice->m_pDevice->SetVertexShader(NULL);
	pDevice->m_pDevice->SetPixelShader(NULL);

	pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
}

void CTClientObjBase::RenderTSLASH( CD3DDevice *pDevice)
{
	for( int i=0; i<TSLASHSFX_COUNT; i++)
		m_vTSLASH[i].Render(pDevice);
}

void CTClientObjBase::Render( CD3DDevice *pDevice,
							  CD3DCamera *pCamera)
{
	MAPTMAINTAIN::iterator itMAINTAIN;

	CTachyonObject::Render(
		pDevice,
		pCamera);

	if( m_bSFX )
		CalcTMagicItemSFX(pDevice);

	CalcSND( &m_mapTSUBACTSNDINST, &m_mapTSUBACTSND, m_fSubActTime, m_dwACTSNDFuncID);
	CalcSND( &m_mapTSUBANISNDINST, &m_mapTSUBANISND, m_fSubActTime, m_dwACTSNDFuncID);

	if( m_bSFX )
	{
		for( itMAINTAIN = m_mapTMAINTAIN.begin(); itMAINTAIN != m_mapTMAINTAIN.end(); itMAINTAIN++)
			CalcSFX( pDevice, &(*itMAINTAIN).second->m_vTSFX);

		CalcSFX( pDevice, &m_vTGARBAGESFX);
		CalcSFX( pDevice, &m_vTACTIONSFX);
	}
}

void CTClientObjBase::RenderWeaponEffect(
	CD3DDevice* pDevice,
	CD3DCamera *pCamera )
{
	MAPOBJECT::iterator itEQUIP, itEND;
	itEQUIP = m_mapEQUIP.begin();
	itEND = m_mapEQUIP.end();

	for(; itEQUIP != itEND ; ++itEQUIP )
	{
		VECTOROBJSFX::iterator itOBJSFX, itOBJSFX_END;
		itOBJSFX = itEQUIP->second->m_vOBJSFX.begin();
		itOBJSFX_END = itEQUIP->second->m_vOBJSFX.end();

		for(; itOBJSFX != itOBJSFX_END ; ++itOBJSFX )
		{
			MAPTSFX::iterator itINST = CTachyonSFX::m_mapINST.find( (*itOBJSFX)->m_dwID );
			if( itINST != CTachyonSFX::m_mapINST.end() )
			{
				if( itINST->second->m_pSFX && !itINST->second->IsDead() )
				{
					itINST->second->Render(
						pDevice,
						pCamera,
						itINST->second->m_bMaxAlpha );
				}
			}
		}
	}
}

void CTClientObjBase::RemoveTActionSFX( LPVECTOROBJSFX pTACTIONSFX)
{
	CTachyonObject::RemoveSFX( pTACTIONSFX, NULL);
}

void CTClientObjBase::ClearTActionSFX( LPVECTOROBJSFX pTACTIONSFX)
{
	CTachyonObject::ClearSFX( pTACTIONSFX, NULL);
}

void CTClientObjBase::PlayTActionSFX( LPVECTOROBJSFX pTACTIONSFX,
									  DWORD dwFuncID,
									  BOOL bShow )
{
	LPTSFXTEMP pTEMP = CTChart::FindTSFXTEMP( dwFuncID );

	if(!pTEMP)
		return;

	if(pTEMP->m_dwPosID)
	{
		MAPSFXINST::iterator it;

		for( it = m_mapOBJSFXINST.begin(); it != m_mapOBJSFXINST.end(); it++)
			if( (*it).second->m_dwFuncID == pTEMP->m_dwPosID )
			{
				CTClientSFX *pTSFX = new CTClientSFX();
				LPOBJSFX pItem = new OBJSFX();
				pItem->m_pUserData = pTSFX;

				pItem->m_pSFX = new SFXINST();
				pItem->m_bUseOBJScale = TRUE;
				pItem->m_bDeleteINST = TRUE;

				pItem->m_pSFX->m_bDeleteOnStop = (*it).second->m_bDeleteOnStop;
				pItem->m_pSFX->m_nPivotID = (*it).second->m_nPivotID;
				pItem->m_pSFX->m_dwTick = (*it).second->m_dwTick;
				pItem->m_pSFX->m_bBias = (*it).second->m_bBias;
				pItem->m_pSFX->m_vPOS = (*it).second->m_vPOS;
				pItem->m_pSFX->m_vROT = (*it).second->m_vROT;

				pItem->m_pSFX->m_pSFX = pTEMP->m_pTSFX;
				pItem->m_pSFX->m_dwFuncID = dwFuncID;

				pTSFX->m_bHasHost = TRUE;
				pTSFX->m_pHost = m_pHost;
				pTSFX->m_dwInitTick = 0;
				pTSFX->m_bShow = bShow;

				pItem->m_dwID = CTachyonSFX::RegSFX(
					pTSFX,
					pItem->m_pSFX->m_bBias);

				pTACTIONSFX->push_back(pItem);
			}
	}
	else
	{
		CTClientSFX *pTSFX = new CTClientSFX();
		LPOBJSFX pItem = new OBJSFX();
		pItem->m_pUserData = pTSFX;

		pItem->m_pSFX = new SFXINST();
		pItem->m_bUseOBJScale = TRUE;
		pItem->m_bDeleteINST = TRUE;

		pItem->m_pSFX->m_bDeleteOnStop = FALSE;
		pItem->m_pSFX->m_dwFuncID = dwFuncID;
		pItem->m_pSFX->m_nPivotID = 0;
		pItem->m_pSFX->m_dwTick = 0;
		pItem->m_pSFX->m_bBias = 0;

		pItem->m_pSFX->m_vPOS = D3DXVECTOR3( 0, 0, 0);
		pItem->m_pSFX->m_vROT = D3DXVECTOR3( 0, 0, 0);
		pItem->m_pSFX->m_pSFX = pTEMP->m_pTSFX;

		pTSFX->m_bHasHost = TRUE;
		pTSFX->m_pHost = m_pHost;
		pTSFX->m_dwInitTick = 0;
		pTSFX->m_bShow = bShow;

		pItem->m_dwID = CTachyonSFX::RegSFX(
			pTSFX,
			pItem->m_pSFX->m_bBias);

		pTACTIONSFX->push_back(pItem);
	}
}

void CTClientObjBase::PlayTSLASH( DWORD dwTSLASH)
{
	static DWORD dwCODE[TSLASHSFX_COUNT] = {
		0x00000001,
		0x00000002};

	for( int i=0; i<TSLASHSFX_COUNT; i++)
		if(dwCODE[i]&dwTSLASH)
			m_vTSLASH[i].SFXStart();
}

void CTClientObjBase::InsertTItemMagicSFX( LPOBJSFX pOBJSFX, CTClientSFX * pTSFX, CTachyonObject* pTOBJ )
{
	pOBJSFX->m_dwID = CTachyonSFX::RegSFX(
		pTSFX,
		pOBJSFX->m_pSFX->m_bBias);

	VECTOROBJSFX* pOBJSFXS;
	MAPTOBJSFXS::iterator finder = m_mapITEMOBJSFX.find(pTOBJ);
	if( finder != m_mapITEMOBJSFX.end() )
		pOBJSFXS = finder->second;
	else
	{
		pOBJSFXS = new VECTOROBJSFX;
		m_mapITEMOBJSFX.insert( make_pair(pTOBJ,pOBJSFXS) );
	}

	pOBJSFXS->push_back(pOBJSFX);
}

void CTClientObjBase::PlayTMagicItemSFX( CTachyonObject* pTOBJ,
										 DWORD dwFuncID)
{
	if( !dwFuncID )
		return;

	LPTSFXTEMP pTEMP = CTChart::FindTSFXTEMP( dwFuncID );
	if( !pTEMP )
		return;

	if( pTEMP->m_dwPosID )
	{
		MAPSFXINST::iterator itr, end;
		itr = pTOBJ->m_mapOBJSFXINST.begin();
		end = pTOBJ->m_mapOBJSFXINST.end();

		for( ; itr != end; ++itr)
		{
			if( itr->second->m_dwFuncID == pTEMP->m_dwPosID )
			{
                CTClientSFX *pTSFX = new CTClientSFX();
				LPOBJSFX pOBJSFX = new OBJSFX();

				pOBJSFX->m_pSFX = new SFXINST();
				pOBJSFX->m_bDeleteINST = TRUE;

				LPSFXINST pSFXINST = itr->second;

				pOBJSFX->m_pSFX->m_bDeleteOnStop = pSFXINST->m_bDeleteOnStop;
				pOBJSFX->m_pSFX->m_nPivotID = pSFXINST->m_nPivotID;
				pOBJSFX->m_pSFX->m_dwTick = pSFXINST->m_dwTick;
				pOBJSFX->m_pSFX->m_bBias = pSFXINST->m_bBias;
				pOBJSFX->m_pSFX->m_vPOS = pSFXINST->m_vPOS;
				pOBJSFX->m_pSFX->m_vROT = pSFXINST->m_vROT;

				pOBJSFX->m_pSFX->m_pSFX = pTEMP->m_pTSFX;
				pOBJSFX->m_pSFX->m_dwFuncID = dwFuncID;

				pTSFX->m_bHasHost = TRUE;
				pTSFX->m_pHost = m_pHost;
				pTSFX->m_dwInitTick = 0;

				InsertTItemMagicSFX(pOBJSFX, pTSFX, pTOBJ);
			}
		}
	}
	else
	{
		CTClientSFX *pTSFX = new CTClientSFX();
		LPOBJSFX pOBJSFX = new OBJSFX();

		pOBJSFX->m_pSFX = new SFXINST();
		pOBJSFX->m_bDeleteINST = TRUE;

		pOBJSFX->m_pSFX->m_bDeleteOnStop = FALSE;
		pOBJSFX->m_pSFX->m_dwFuncID = dwFuncID;
		pOBJSFX->m_pSFX->m_nPivotID = 0;
		pOBJSFX->m_pSFX->m_dwTick = 0;
		pOBJSFX->m_pSFX->m_bBias = 0;

		pOBJSFX->m_pSFX->m_vPOS = D3DXVECTOR3( 0, 0, 0);
		pOBJSFX->m_pSFX->m_vROT = D3DXVECTOR3( 0, 0, 0);
		pOBJSFX->m_pSFX->m_pSFX = pTEMP->m_pTSFX;

		pTSFX->m_bHasHost = TRUE;
		pTSFX->m_pHost = m_pHost;
		pTSFX->m_dwInitTick = 0;

		InsertTItemMagicSFX(pOBJSFX, pTSFX, pTOBJ);
	}
}

void CTClientObjBase::RemoveTMagicItemSFX(CTachyonObject* pTOBJ)
{
	MAPTOBJSFXS::iterator finder = m_mapITEMOBJSFX.find(pTOBJ);
	if( finder != m_mapITEMOBJSFX.end() )
	{
		CTachyonObject* pTOBJ = finder->first;
		VECTOROBJSFX* pOBJSFXS = finder->second;

		pTOBJ->RemoveSFX(pOBJSFXS, NULL);
		delete pOBJSFXS;

		m_mapITEMOBJSFX.erase(finder);
	}
}

void CTClientObjBase::RemoveAllTMagicItemSFX()
{
	MAPTOBJSFXS::iterator itr,end;
	itr = m_mapITEMOBJSFX.begin();
	end = m_mapITEMOBJSFX.end();

	for( ;itr!=end; ++itr)
	{
		CTachyonObject* pTOBJ = itr->first;
		VECTOROBJSFX* pOBJSFXS = itr->second;

		pTOBJ->RemoveSFX(pOBJSFXS, NULL);
		delete pOBJSFXS;
	}

	m_mapITEMOBJSFX.clear();
}

void CTClientObjBase::RemoveAllEquipSFX()
{
	VECTORDWORD::iterator it, end;
	it = m_vEquipSFXFuncID.begin();
	end = m_vEquipSFXFuncID.end();
	for(; it != end ; ++it )
		RemoveSFX( &m_vOBJSFX, &m_mapOBJSFX, (*it) );
	m_vEquipSFXFuncID.clear();
}

void CTClientObjBase::SetAlphaOfAllEquipSFX( BYTE bAlpha )
{
	VECTORDWORD::iterator itFUNCID, endFUNCID;
	itFUNCID = m_vEquipSFXFuncID.begin();
	endFUNCID = m_vEquipSFXFuncID.end();
	for(; itFUNCID != endFUNCID ; ++itFUNCID )
	{
		VECTOROBJSFX::iterator itSFX = m_vOBJSFX.begin();
		
		while( itSFX != m_vOBJSFX.end() )
		{
			LPOBJSFX pItem = (*itSFX);

			if( pItem->m_pSFX && pItem->m_pSFX->m_dwFuncID == (*itFUNCID) )
			{
				MAPTSFX::iterator itTSFX = CTachyonSFX::m_mapINST.find( pItem->m_dwID );
				if( itTSFX != CTachyonSFX::m_mapINST.end() )
					itTSFX->second->m_bMaxAlpha = bAlpha;			
			}

			++itSFX;
		}
	}
}

void CTClientObjBase::RemoveAllWeaponSFX()
{
	MAPOBJECT::iterator it, end;
	it = m_mapEQUIP.begin();
	end = m_mapEQUIP.end();

	for(; it != end ; ++it )
	{
		DWORD dwWeaponPOS = it->first;
		CTachyonObject* pTOBJ = it->second;

		TMAPWEAPONEQUIPINFO::iterator itINFO = m_mapEQUIPInfo.find( dwWeaponPOS );
		if( itINFO != m_mapEQUIPInfo.end() )
		{
			LPTITEMVISUAL pEquipVisual = itINFO->second.m_pItemVisual;
			BYTE bMode = itINFO->second.m_bMode;

			if( pEquipVisual->m_dwEffectFuncID[bMode] != 0 )
			{
				pTOBJ->RemoveSFX(
					&pTOBJ->m_vOBJSFX,
					&pTOBJ->m_mapOBJSFX,
					pEquipVisual->m_dwEffectFuncID[bMode] );
			}
		}
	}

	m_mapEQUIPInfo.clear();
}

void CTClientObjBase::SetAlphaOfAllWeaponSFX( BYTE bAlpha )
{
	MAPOBJECT::iterator it, end;
	it = m_mapEQUIP.begin();
	end = m_mapEQUIP.end();

	for(; it != end ; ++it )
	{
		DWORD dwWeaponPOS = it->first;
		CTachyonObject* pTOBJ = it->second;

		TMAPWEAPONEQUIPINFO::iterator itINFO = m_mapEQUIPInfo.find( dwWeaponPOS );
		if( itINFO != m_mapEQUIPInfo.end() )
		{
			LPTITEMVISUAL pEquipVisual = itINFO->second.m_pItemVisual;
			BYTE bMode = itINFO->second.m_bMode;

			if( pEquipVisual->m_dwEffectFuncID[bMode] != 0 )
			{
				VECTOROBJSFX::iterator itSFX = pTOBJ->m_vOBJSFX.begin();
				while( itSFX != pTOBJ->m_vOBJSFX.end() )
				{
					LPOBJSFX pItem = (*itSFX);

					if( pItem->m_pSFX &&
						pItem->m_pSFX->m_dwFuncID == pEquipVisual->m_dwEffectFuncID[bMode] )
					{
						MAPTSFX::iterator itTSFX = CTachyonSFX::m_mapINST.find( pItem->m_dwID );
						if( itTSFX != CTachyonSFX::m_mapINST.end() )
							itTSFX->second->m_bMaxAlpha = bAlpha;
					}

					++itSFX;
				}
			}
		}
	}
}

void CTClientObjBase::CalcTMagicItemSFX(CD3DDevice *pDevice)
{
	MAPOBJECT::iterator itrOBJ, endOBJ;
	itrOBJ = m_mapEQUIP.begin();
	endOBJ = m_mapEQUIP.end();

	for( ; itrOBJ!=endOBJ; ++itrOBJ)
	{
		CTachyonObject* pEQUIP = itrOBJ->second;
		if( !pEQUIP )
			continue;

		MAPTOBJSFXS::iterator finder = m_mapITEMOBJSFX.find(pEQUIP);
		if( finder == m_mapITEMOBJSFX.end() )
			continue;

		D3DXMATRIX vPosition = pEQUIP->m_vPosition;
		pEQUIP->m_vPosition *= m_pBone[itrOBJ->first];
		
		VECTOROBJSFX* pOBJSFXS = finder->second;
		pEQUIP->CalcSFX(pDevice, pOBJSFXS);

		pEQUIP->m_vPosition = vPosition;
	}
}

void CTClientObjBase::DoSLIDE( LPD3DXVECTOR3 pATTACK,
							   LPD3DXVECTOR3 pDEFEND,
							   BYTE bForceRUN)
{
}

void CTClientObjBase::EndSLIDE()
{
	m_vSlideDIR = D3DXVECTOR2( 0.0f, 0.0f);
	m_dwSlideTick = 0;
	m_bSlide = FALSE;
}

void CTClientObjBase::Push( FLOAT fPushDist,
							LPD3DXVECTOR2 pDIR)
{
}

void CTClientObjBase::CancelTAction()
{
}

void CTClientObjBase::RemoveTAction()
{
}

void CTClientObjBase::ResetRootID( DWORD dwRootID)
{
	MAPDWORD::iterator itID = m_OBJ.m_pOBJ->m_mapPIVOT.find(dwRootID);

	if( itID != m_OBJ.m_pOBJ->m_mapPIVOT.end() )
		m_bRootID = BYTE((*itID).second + 1);
	else
		m_bRootID = 0;
}

BYTE CTClientObjBase::CanUseSubANI()
{
	return m_bRootID ? TRUE : FALSE;
}

BYTE CTClientObjBase::GetSkillActivateID( LPTSKILL pTSKILL)
{
	return m_bRootID && pTSKILL->m_bActionID[TSKILLACTION_MOVEACTIVATE] && (m_wMoveDIR != MOVE_NONE || IsJump() || IsFall()) ? pTSKILL->m_bActionID[TSKILLACTION_MOVEACTIVATE] : pTSKILL->m_bActionID[TSKILLACTION_ACTIVATE];
}

BYTE CTClientObjBase::GetSkillItemType( LPTSKILL pTSKILL)
{
	CTClientItem *pTITEM = FindSkillWeapon(pTSKILL);
	return pTITEM && pTITEM->GetTITEM() ? pTITEM->GetTITEM()->m_bSkillItemType : 0;
}

CTClientItem *CTClientObjBase::FindSkillWeapon( LPTSKILL pTSKILL)
{
	if( pTSKILL && pTSKILL->m_dwWeaponID )
	{
		CTClientInven *pEQUIP = FindTInven(m_pTRANS ? INVEN_TRANS : INVEN_EQUIP);

		if(pEQUIP)
		{
			static dwEquipID[] = {
				ES_PRMWEAPON,
				ES_SNDWEAPON,
				ES_LONGWEAPON};

			for( int i=0; i<TWEAPON_COUNT; i++)
			{
				CTClientItem *pTITEM = pEQUIP->FindTItem(dwEquipID[i]);

				if( pTITEM &&
					(!pTITEM->GetDuraMax() || pTITEM->GetDuraCurrent()) &&
					TCHECK_WEAPON( pTSKILL->m_dwWeaponID, pTITEM->GetTITEM()->m_bKind) )
				{
					return pTITEM;
				}
			}
		}
	}

	return NULL;
}

BYTE CTClientObjBase::CheckSkillPrevACT( LPTSKILL pTSKILL)
{
	return !pTSKILL->m_wActSkillID || m_mapTMAINTAIN.find(pTSKILL->m_wActSkillID) != m_mapTMAINTAIN.end() ? TRUE : FALSE;
}

BYTE CTClientObjBase::CheckSkillRegion( LPTSKILL pTSKILL)
{
	return !CTClientSkill::IsFlySKILL(pTSKILL) || (m_pTREGION && m_pTREGION->m_bCanFly && !m_bSwim) ? TRUE : FALSE;
}

BYTE CTClientObjBase::CheckSkillWespon( LPTSKILL pTSKILL)
{
	return !pTSKILL->m_dwWeaponID || FindSkillWeapon(pTSKILL) ? TRUE : FALSE;
}

BYTE CTClientObjBase::CheckSkillSilence( LPTSKILL pTSKILL)
{
	if( pTSKILL->m_bKind && m_bSkillSilence )
		return FALSE;

	return TRUE;
}

BYTE CTClientObjBase::CheckSkillMode( LPTSKILL pTSKILL)
{
	return pTSKILL && (!pTSKILL->m_wModeID || m_mapTMAINTAIN.find(pTSKILL->m_wModeID) != m_mapTMAINTAIN.end()) ? TRUE : FALSE;
}

BYTE CTClientObjBase::CheckTransMode( LPTSKILL pTSKILL)
{
	if( !m_pTRANS || m_bType == OT_MON )
		return TRUE;

	if( m_bType == OT_PC )
	{
		BYTE bActionID = GetSkillActivateID(pTSKILL);

		TACTION vActionID = FindActionID(
			bActionID,
			GetWeaponID(MT_BATTLE));

		if( vActionID.m_dwActID == 0 && vActionID.m_dwAniID == 0 )
			return FALSE;
	}

	if( m_pTRANS->m_bCanAttack && (pTSKILL->m_bLoop == 1) )
		return TRUE;

	return FALSE;
}

BYTE CTClientObjBase::CheckSkill( LPTSKILL pTSKILL)
{
	return !IsDead() && !m_bGhost &&
		CheckSkillPrevACT(pTSKILL) &&
		CheckSkillRegion(pTSKILL) &&
		CheckSkillWespon(pTSKILL) &&
		CheckSkillSilence(pTSKILL) &&
		CheckSkillMode(pTSKILL) &&
		CheckTransMode(pTSKILL) ? TRUE : FALSE;
}

void CTClientObjBase::ActivateTALK( LPDIRECT3DDEVICE9 pDevice,
								    CString strTALK,
									DWORD dwCOLOR)
{
	m_vTALK.SetTALK( pDevice, strTALK);
	m_vTALK.m_dwColor = dwCOLOR;

	m_bDrawTalk = TRUE;
	m_dwTalkTick = 0;
}

CString CTClientObjBase::GetDefaultTALK()
{
	MAPTQUESTTEMP::iterator itTQUEST;

	LPVTQUESTTEMP pTDEFTALK = NULL;
	VTQUESTTEMP vTDEFTALK;
	VTQUESTTEMP vTNPCTALK;
	CString strRESULT;

	strRESULT.Empty();
	vTDEFTALK.clear();
	vTNPCTALK.clear();

	for( itTQUEST = m_mapTQUESTTEMP.begin(); itTQUEST != m_mapTQUESTTEMP.end(); itTQUEST++)
		switch((*itTQUEST).second->m_bType)
		{
		case QT_NPCTALK	: vTNPCTALK.push_back((*itTQUEST).second); break;
		case QT_DEFTALK	: vTDEFTALK.push_back((*itTQUEST).second); break;
		}

	if(vTNPCTALK.empty())
		pTDEFTALK = &vTDEFTALK;
	else
		pTDEFTALK = &vTNPCTALK;

	if(!pTDEFTALK->empty())
		strRESULT = (*pTDEFTALK)[rand() % INT(pTDEFTALK->size())]->m_strTriggerMSG;

	vTDEFTALK.clear();
	vTNPCTALK.clear();

	return strRESULT;
}

BYTE CTClientObjBase::CheckDIR( WORD wDIR,
							    WORD wPITCH)
{
	if( m_bType == OT_SELF )
		return FALSE;

	int nPITCH = INT(wPITCH) - INT(m_wPITCH);
	int nDIR = INT(wDIR) - INT(m_wDIR);

	while(nPITCH < 0)
		nPITCH += 1800;
	nPITCH %= 1800;

	while(nDIR < 0)
		nDIR += 1800;
	nDIR %= 1800;

	if( nPITCH > 900 )
		nPITCH = 1800 - nPITCH;

	if( nDIR > 900 )
		nDIR = 1800 - nDIR;

	return nDIR > FOLLOW_RANGE || nPITCH > FOLLOW_RANGE ? TRUE : FALSE;
}

void CTClientObjBase::DoTRANS( CD3DDevice *pDevice,
							   CTachyonRes *pRES,
							   WORD wMonID)
{
	if( m_pTRANS && m_pHost )
	{
		OBJMSG msg;

		msg.m_dwMSG = OM_ACTENDING;
		msg.m_pOBJ = (DWORD_PTR) this;
		m_dwActionID = 0;
		m_dwAniID = 0;

		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);

		switch( GetTAction() )
		{
		case TA_WALK		:
		case TA_RUN			:
		case TA_BACK		:
		case TA_TURN_LEFT	:
		case TA_TURN_RIGHT	: break;
		default				: ApplyTAction( pDevice, pRES, TA_SPAWN); break;
		}
	}
}

void CTClientObjBase::DoRETRANS( CD3DDevice *pDevice,
								 CTachyonRes *pRES)
{
	MAPTINVEN::iterator finder = m_mapTINVEN.find(INVEN_TRANS);

	if( finder != m_mapTINVEN.end() )
	{
		delete (*finder).second;
		m_mapTINVEN.erase(finder);
	}

	if( m_pTRANS && m_pHost )
	{
		OBJMSG msg;

		msg.m_dwMSG = OM_ACTENDING;
		msg.m_pOBJ = (DWORD_PTR) this;
		m_dwActionID = 0;
		m_dwAniID = 0;

		m_pHost->SendMessage( WM_TOBJMSG, (WPARAM) &msg);
	}

	m_pTRANS = NULL;
}

void CTClientObjBase::DoDISGUISE( CD3DDevice *pDevice,
								  CTachyonRes *pRES,
								  BYTE bCountry)
{
	m_bDisguise = bCountry;

	ResetEQUIP(pDevice, pRES);
}
	
void CTClientObjBase::EndDISGUISE( CD3DDevice *pDevice,
								   CTachyonRes *pRES)
{
	m_bDisguise = (BYTE)T_INVALID;

	ResetEQUIP(pDevice, pRES);
}

FLOAT CTClientObjBase::GetMinRange( CTClientObjBase *pTARGET,
								    LPTSKILL pTSKILL)
{
	FLOAT fRESULT = fabs(pTSKILL->m_fMinRange) > TCLIENT_ZERO ? pTSKILL->m_fMinRange + m_fRadius : 0.0f;

	if( fRESULT != 0.0f && pTARGET )
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

FLOAT CTClientObjBase::GetMaxRange( CTClientObjBase *pTARGET,
								    LPTSKILL pTSKILL)
{
	FLOAT fRESULT = m_fRangeFactor * pTSKILL->m_fMaxRange + m_fRadius;

	if(pTARGET)
		fRESULT += pTARGET->m_fRadius;

	return fRESULT;
}

BYTE CTClientObjBase::IsFlyOBJ()
{
	return m_bSwim || (m_pTRANS && m_pTRANS->m_bCanFly) ? TRUE : FALSE;
}

BYTE CTClientObjBase::CanDIVE()
{
	return TRUE;
}

BYTE CTClientObjBase::CanFLY()
{
	return m_pTRANS && m_pTRANS->m_bCanFly ? TRUE : FALSE;
}

void CTClientObjBase::OnDIE( CD3DDevice *pDevice,
							 CTachyonRes *pRES)
{
	ClearMaintainOnDie();
	CancelTAction();
	ClearAdjacencyMon();

	m_fSpeedFactor = TDEF_SPEED;
	m_dwHP = 0;

	if( !IsJump() && !IsFall() && !IsPush() )
	{
		TACTION vActionID = FindActionID(
			TA_DIE,
			GetWeaponID(m_bMode));
		LPTACTIONDATA pTACTDATA = CTChart::GetTACTION(TA_DIE);
		SetTAction(TA_DEAD);

		if( m_bHideOnCapeMode != pTACTDATA->m_bHideOnCapeMode ||
			m_bEquipMode != vActionID.m_bEquipMode )
		{
			m_bHideOnCapeMode = pTACTDATA->m_bHideOnCapeMode;
			m_bEquipMode = vActionID.m_bEquipMode;

			ResetEQUIP(
				pDevice,
				pRES);
		}

		m_bACTLevel = pTACTDATA->m_bLevel;
		SetAction( vActionID.m_dwActID, vActionID.m_dwAniID);
	}

	CTClientGame::GetInstance()->NotifyObjDie(this);
}

LRESULT CTClientObjBase::OnActEndMsg()
{
	if(m_bCLEARTARGET)
		ClearSkillTarget();
	m_bActionLock = FALSE;

	if( m_bACTLevel != CTChart::GetTACTION(m_bAction)->m_bLevel )
	{
		m_bACTLevel = CTChart::GetTACTION(m_bAction)->m_bLevel;
		return 1;
	}

	return 0;
}

BYTE CTClientObjBase::CheckRequired()
{
	return TRUE;
}

void CTClientObjBase::OnShotSkill()
{
}

BYTE CTClientObjBase::GetDrawShadow()
{
	return m_bDynamicOBJ && CTClientGame::GetObjSHADOWOption() ? TRUE : FALSE;
}

BYTE CTClientObjBase::GetDrawName()
{
	return m_bDrawName;
}

D3DXMATRIX& CTClientObjBase::GetPositionMat()
{
	return m_vPosition;
}

D3DXVECTOR3 CTClientObjBase::GetPosition()
{
	return D3DXVECTOR3(GetPositionX(),GetPositionY(),GetPositionZ());
}

FLOAT CTClientObjBase::GetPositionX()
{
	return GetPositionMat()._41;
}

FLOAT CTClientObjBase::GetPositionY()
{
	return GetPositionMat()._42;
}

FLOAT CTClientObjBase::GetPositionZ()
{
	return GetPositionMat()._43;
}

void CTClientObjBase::SetPosition( D3DXMATRIX& matPOS)
{
	m_vPosition = matPOS;
}

void CTClientObjBase::SetPosition( D3DXVECTOR3& vPOS)
{
	m_vPosition._41 = vPOS.x;
	m_vPosition._42 = vPOS.y;
	m_vPosition._43 = vPOS.z;
}

void CTClientObjBase::SetPositionX(FLOAT fX)
{
	m_vPosition._41 = fX;
}
void CTClientObjBase::SetPositionY(FLOAT fY)
{
	m_vPosition._42 = fY;
}
void CTClientObjBase::SetPositionZ(FLOAT fZ)
{
	m_vPosition._43 = fZ;
}

void CTClientObjBase::AddPositionX(FLOAT fX)
{
	m_vPosition._41 += fX;
}

void CTClientObjBase::AddPositionY(FLOAT fY)
{
	m_vPosition._42 += fY;
}

void CTClientObjBase::AddPositionZ(FLOAT fZ)
{
	m_vPosition._43 += fZ;
}

BYTE CTClientObjBase::GetAftermathStep()
{
	return 0;
}