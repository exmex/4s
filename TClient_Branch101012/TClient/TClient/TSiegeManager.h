#pragma once


//! SiegeManager Class.
/*!
*	@brief	성물쟁탈전 관리자 클래스..
*
*	@date	2009/08/21
*	@author	권 성혁
*
*/
class CTSiegeManager
{
private :

	CTSiegeManager();
	~CTSiegeManager();

public :	// MEMBER DATA.

	MAPSIEGEGODBALL			m_mapBalls;
	MAPSIEGEGODTOWER		m_mapTowers;

public :	// MEMBER FUNCTION.

	static CTSiegeManager*	GetInstance();

	/*! */
	CTGodBall*				FindGodBall( WORD wGodBallID );
	/*! */
	CTGodTower*				FindGodTower( WORD wGodTowerID );
	/*! Visible */
	void					ResetVisibleOBJ( DWORD dwTick );
	/*! 성물 or 타워에 대한 이벤트처리.. */
	void					OnRClick();
	/*! */
	void					ChargeAffterProcess( CTGodTower* pGodTower );
	/*! */
	void					CalcTick( CD3DDevice* pDevice, DWORD dwTick );
	/*! 모두초기화.. */
	void					Release();
	/*! 전투종료.. */
	void					Endwar(
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
		WORD wAtkKillPoint);

public :	// RECEIVE FUNCTION.

	/*! 성물이 땅에떨어지거나 새로 생성되는 경우.. */
	void					RecvAddGodBall( WORD wBallID, BYTE bCamp, BOOL bInGround, D3DXVECTOR3& vPosition );
	/*! 해당 캐릭터에서 성물을 제거.. */
	void					RecvRemoveGodBall( DWORD dwCharID );
	/*! 해당 캐릭터가 성물을 소유.. */
	void					RecvTakeGodBall( DWORD dwCharID, WORD wBallID );
	/*! 성물이 맵에서 제거됨.. */
	void					RecvDelGodBall( WORD wBallID );
	/*! 타워생성.. */
	void					RecvAddGodTower( WORD wTowerID, BYTE bCamp, D3DXVECTOR3& vPosition );
	/*! 타워제거.. */
	void					RecvDelGodTower( WORD wTowerID );
	/*! 캐릭터가 성물을 타워에 꽂았음.. */
	void					RecvMountGodBall( WORD wTowerID, WORD wBallID, BYTE bCamp, DWORD dwCharID );
	/*! 적국 캐릭터가 해당 타워에서 성물을 제거.. */
	void					RecvDemountGodBall( WORD wTowerID );

};

#define SiegeMgr CTSiegeManager::GetInstance()
