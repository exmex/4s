#include "stdafx.h"
#include "TSiegeManager.h"
#include "TClientGame.h"

//////////////////////////////////////////////////////////////////////////
//	AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
CTSiegeManager::CTSiegeManager()
{
	m_mapBalls.clear();
	m_mapTowers.clear();
}

CTSiegeManager::~CTSiegeManager()
{
	Release();
}
//	END OF AUCTION COMMANDER CONSTRUCTOR & DESTRUCTOR.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	MEMBER FUNCTION IMPLEMENTATION.

CTSiegeManager* CTSiegeManager::GetInstance()
{
	static CTSiegeManager mgr;

	return &mgr;
}

CTGodBall*	CTSiegeManager::FindGodBall( WORD wGodBallID )
{
	if( wGodBallID == 0 )
		return NULL;

	MAPSIEGEGODBALL::iterator finder = m_mapBalls.find( wGodBallID );
	if( finder != m_mapBalls.end() )
		return (*finder).second;

	return NULL;
}

CTGodTower*	CTSiegeManager::FindGodTower( WORD wGodTowerID )
{
	if( wGodTowerID == 0 )
		return NULL;

	MAPSIEGEGODTOWER::iterator finder = m_mapTowers.find( wGodTowerID );
	if( finder != m_mapTowers.end() )
		return (*finder).second;

	return NULL;
}

/*! Visible */
void	CTSiegeManager::ResetVisibleOBJ(DWORD dwTick)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientCAM* pCam = pGame->GetMainCam();
	CTClientMAP* pMap = pGame->GetClientMap();
	CD3DDevice* pDev = pGame->GetDevice();

	MAPSIEGEGODTOWER::iterator itrTw, endTw;
	itrTw = m_mapTowers.begin();
	endTw = m_mapTowers.end();

	for(; itrTw!=endTw; ++itrTw)
	{
		CTGodTower* pGodTower = itrTw->second;

		pGodTower->ResetVisible(
			pCam,
			pMap->m_dwHouseID,
			pMap->m_bHouseMesh,
			dwTick);

		if( pGodTower->m_bVisible )
			pGame->PushDrawOBJ( pGodTower );
		else
			pGodTower->CalcRender( pDev );
	}

	MAPSIEGEGODBALL::iterator itrBall, endBall;
	itrBall = m_mapBalls.begin();
	endBall = m_mapBalls.end();

	for( ; itrBall != endBall; ++itrBall )
	{
		CTGodBall* pGodBall = itrBall->second;

		if( pGodBall->InGround() )
		{
			pGodBall->ResetVisible(
				pCam,
				pMap->m_dwHouseID,
				pMap->m_bHouseMesh,
				dwTick);

			if( pGodBall->IsVisible() )
				pGame->PushDrawOBJ( pGodBall );
			else
				pGodBall->CalcRender( pDev );
		}
	}
}

/*! 성물 or 타워에 대한 이벤트처리.. */
void	CTSiegeManager::OnRClick()
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	pGame->DoDEFACTION();
}

/*! */
void	CTSiegeManager::ChargeAffterProcess( CTGodTower* pGodTower )
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSession =  pGame->GetSession();
	CTClientChar* pMainChar = pGame->GetMainChar();

	CTGodBall* pGodBall = FindGodBall( pMainChar->GetGodBallID() );
	if( pGodBall )
	{
		if( pGodTower->GetCamp() == 0 )
			pSession->SendCS_MOUNTGODBALL_REQ( pGodTower->GetTowerID() );
		else if( pGodTower->GetCamp() != pMainChar->GetCamp() )
			pSession->SendCS_DEMOUNTGODBALL_REQ( pGodTower->GetTowerID() );
	}
}

/*! */
void	CTSiegeManager::CalcTick( CD3DDevice* pDevice, DWORD dwTick )
{
	MAPSIEGEGODBALL::iterator itor = m_mapBalls.begin();

	while( itor != m_mapBalls.end() )
	{
		if( (*itor).second->IsVisible() )
			(*itor).second->CalcTick( pDevice->m_pDevice, dwTick );	

		++itor;
	}
}

/*! 모두초기화.. */
void	CTSiegeManager::Release()
{
	MAPSIEGEGODTOWER::iterator itrTw, endTw;
	itrTw = m_mapTowers.begin();
	endTw = m_mapTowers.end();

	for(; itrTw!=endTw; ++itrTw)
	{
		itrTw->second->ClearSFX();
		delete itrTw->second;
		itrTw->second = NULL;
	}

	m_mapTowers.clear();


	MAPSIEGEGODBALL::iterator itrBall, endBall;
	itrBall = m_mapBalls.begin();
	endBall = m_mapBalls.end();

	for(; itrBall!=endBall; ++itrBall)
	{
		delete itrBall->second;
		itrBall->second = NULL;
	}

	m_mapBalls.clear();
}

/*! 전투종료.. */
void	CTSiegeManager::Endwar(
							   BYTE bType,
							   DWORD dwWinGuildID,
							   DWORD dwDefTotal,
							   DWORD dwAtkTotal,
							   CString strDefName,
							   DWORD dwDefPower,
							   WORD wDefPoint,
							   WORD wDefKillPoint,
							   CString strAtkName,
							   DWORD dwAtkPower,
							   WORD wAtkPoint,
							   WORD wAtkKillPoint)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	CTSiegeUI* pUI = static_cast< CTSiegeUI* >( pGame->GetFrame( TFRAME_SIEGEUI ) );
	pUI->EndWar();

	CTSiegeEnd* pEnd = static_cast< CTSiegeEnd* >( pGame->GetFrame( TFRAME_SIEGEEND ) );
	pEnd->Endwar(
		bType,
		dwWinGuildID,
		dwDefTotal,
		dwAtkTotal,
		strDefName,
		dwDefPower,
		wDefPoint,
		wDefKillPoint,
		strAtkName,
		dwAtkPower,
		wAtkPoint,
		wAtkKillPoint );
}

//	END OF MEMBER FUNCTION.
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//	RECEIVE FUNCTION IMPLEMENTATION.

/*! 성물이 땅에떨어지거나 새로 생성되는 경우.. */
void	CTSiegeManager::RecvAddGodBall( WORD wBallID, BYTE bCamp, BOOL bInGround, D3DXVECTOR3& vPosition )
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	CTGodBall* pGodBall = FindGodBall( wBallID );
	if( pGodBall == NULL )
	{
		pGodBall = new CTGodBall;
		pGodBall->ReleaseData();

		pGodBall->InitGodBall( wBallID, pGame->GetDevice(), pGame->GetResource() );
		m_mapBalls.insert( make_pair( wBallID, pGodBall ) );
	}

	pGodBall->AddGodBall( bCamp, vPosition, bInGround );
}

/*! 해당 캐릭터에서 성물을 제거.. */
void	CTSiegeManager::RecvRemoveGodBall( DWORD dwCharID )
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pChar = pGame->FindPC( dwCharID );
	if( pChar )
	{
		pChar->ClearGodBallSFX();
		pChar->SetGodBallID( 0 );
	}
}

/*! 해당 캐릭터가 성물을 소유.. */
void	CTSiegeManager::RecvTakeGodBall( DWORD dwCharID, WORD wBallID )
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	CTClientChar* pChar = pGame->FindPC( dwCharID );
	if( pChar )
	{
		pChar->SetGodBallID( wBallID );

		// 적군이 Hide 상태에서 ball을 잡으면 처음에 이펙트를 보이지않음..
		BOOL bShow = !pChar->m_bHide || pMainChar->IsAlliance( pChar );

		pChar->PlayGodBallSFX( bShow );
	}

	CTGodBall* pGodBall = FindGodBall( wBallID );
	if( pGodBall )
	{
		pGodBall->TakeGodBall();
	}
}

/*! 성물이 맵에서 제거됨.. */
void	CTSiegeManager::RecvDelGodBall( WORD wBallID )
{
	CTGodBall* pGodBall = FindGodBall( wBallID );
	if( pGodBall )
	{
		pGodBall->DelGodBall();
	}
}

/*! 타워생성.. */
void	CTSiegeManager::RecvAddGodTower( WORD wTowerID, BYTE bCamp, D3DXVECTOR3& vPosition )
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	CTGodTower* pGodTower = FindGodTower( wTowerID );
	if( pGodTower == NULL )
	{
		pGodTower = new CTGodTower;
		pGodTower->ReleaseData();

		pGodTower->InitTower( wTowerID, vPosition, pGame->GetDevice(), pGame->GetResource() );

		m_mapTowers.insert( make_pair( wTowerID, pGodTower ) );

		CTClientMAP* pMap = pGame->GetClientMap();
		pMap->LandTOBJ( pGodTower );
	}

	pGodTower->AddGodTower( bCamp, vPosition );
}

/*! 타워제거.. */
void	CTSiegeManager::RecvDelGodTower( WORD wTowerID )
{
	CTGodTower* pGodTower = FindGodTower( wTowerID );
	if( pGodTower )
	{
		pGodTower->DelGodTower();
	}
}

/*! 캐릭터가 성물을 타워에 꽂았음.. */
void	CTSiegeManager::RecvMountGodBall( WORD wTowerID, WORD wBallID, BYTE bCamp, DWORD dwCharID )
{
	CTGodTower* pGodTower = FindGodTower( wTowerID );

	if( pGodTower )
	{
		pGodTower->MountBall( bCamp );
	}
}

/*! 적국 캐릭터가 해당 타워에서 성물을 제거.. */
void	CTSiegeManager::RecvDemountGodBall( WORD wTowerID )
{
	CTGodTower* pGodTower = FindGodTower( wTowerID );
	if( pGodTower )
	{
		pGodTower->DeMountBall();
	}
}

//	END OF RECEIVE FUNCTION.
//////////////////////////////////////////////////////////////////////////
