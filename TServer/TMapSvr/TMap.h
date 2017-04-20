#pragma once


class CTMap
{
public:
	static VBYTE m_vTCHANNEL;

public:
	MAPTMONSPAWN m_mapTMONSPAWN;
	MAPTSPAWNPOS m_mapTSPAWNPOS;
	MAPTMONSTER m_mapTMONSTER;
	MAPTSWITCH m_mapTSWITCH;
	MAPTGATE m_mapTGATE;
	MAPTCELL m_mapTCELL;
	MAPWORD m_mapTUNIT;
	MAPTGODBALL m_mapTTEMPGODBALL;
	MAPTGODTOWER m_mapTGODTOWER;
	MAPTGODBALL m_mapTGODBALL;
	MAPTOURNAMENTPLAYER m_mapTNMTPlayer;

	WORD m_wPartyID;
	WORD m_wMapID;

	BYTE m_bServerID;
	BYTE m_bChannel;
	BYTE m_bType;

	LPTLOCAL m_pCastle;
	BYTE m_bDefBallMax;
	BYTE m_bAtkBallMax;
	DWORD m_dwDefPower;
	DWORD m_dwAtkPower;
	BYTE m_bBlueTeam;
	BYTE m_bRedTeam;

	WORD m_wDefKillPoint;
	WORD m_wAtkKillPoint;
	DWORD m_dwBlueHideTick;
	DWORD m_dwRedHideTick;

	DWORD m_dwEndTick;

public:
	void EnterMAP(
		CTPlayer *pPlayer,
		BYTE bNewMember);
	void LeaveMAP( CTPlayer *pPlayer);

	void LeaveMAP(
		CTMonster *pMON,
		BYTE bExitMap);

	void LeaveMAP(LPTGODBALL pGodBall, BYTE bErase);

	void EnterMAP( CTMonster *pMON);
	BYTE EnterMAP( LPTSWITCH pSwitch);
	BYTE EnterMAP( LPTGATE pGate);
	BYTE EnterMAP( LPTGODTOWER pGodTower);
	BYTE EnterMAP( LPTGODBALL pGodBall);

	void OnMove(
		CTPlayer *pPlayer,
		FLOAT fPosX,
		FLOAT fPosZ);

	void OnMove(
		CTMonster *pMON,
		FLOAT fPosX,
		FLOAT fPosZ);

	void GetNeighbor(
		LPMAPTCELL pCELLS,
		FLOAT fPosX,
		FLOAT fPosZ);

	void GetNeighbor(
		LPVPLAYER pPLAYERS,
		FLOAT fPosX,
		FLOAT fPosZ);

	void GetNeighbor(
		LPVTMONSTER pMONSTERS,
		FLOAT fPosX,
		FLOAT fPosZ);

	CTObjBase * FindNeighbor(
		BYTE bType,
		DWORD dwObjID,
		FLOAT fPosX,
		FLOAT fPosZ);

	CTMonster *FindMonster(
		DWORD dwID);

	LPTSWITCH FindSwitch(
		DWORD dwID);

	void StartWar(
		WORD wCastle,
		BYTE bDefCountry,
		DWORD dwDefGuild,
		WORD wDefPoint,
		DWORD dwAtkGuild,
		WORD wAtkPoint,
		DWORD dwEndTick);

	void EndWar();

	LPTGODBALL FindGodBall(WORD wBall);
	LPTGODBALL FindMapGodBall(WORD wBall);
	LPTGODTOWER FindGodTower( WORD wTower);
	void MoveGodBall(LPTGODBALL pBall);
	void MoveTempGodBall(LPTGODBALL pBall);
	void ResetTempGodBall();
	LPTGODBALL GenGodBall(BYTE bCamp, WORD & wID);
	void InitGodBall(BYTE bCamp, DWORD dwGuild, WORD wCastle);
	BYTE CheckPower();
	void SetCastle(LPTLOCAL pCastle);
	void GetUnitPlayer(
		LPMAPPLAYER pPLAYERS,
		FLOAT fPosX,
		FLOAT fPosZ);
	void GetMapPlayers(LPVPLAYER vPlayers);

	void ClearMonster();

	void GetNeerPlayer(
		LPVPLAYER pPLAYERS,
		FLOAT fPosX,
		FLOAT fPosZ);

	void GetNeerMonster(
		LPVTMONSTER pMONSTERS,
		FLOAT fPosX,
		FLOAT fPosZ);

public:
	void AddMonSpawn(
		LPTMONSPAWN pSPAWN,
		BYTE bChannel,
		BYTE bCountry);

	CTMonSpawn *FindMonSpawn(
		WORD wSpawnID);

	CTMonSpawn * AddTimelimitedMon(
		LPTMONSPAWN pSPAWN,
		BYTE bChannel,
		BYTE bCountry,
		BYTE bRegenType);

	void InitMonster(
		CTMonSpawn * pMonSpawn,
		LPTMONSPAWN pSPAWN,
		BYTE bChannel,
		BYTE bCountry,
		BYTE bReady);

	void GetGodBallOwner(
		CString &strAtk1,
		CString &strAtk2,
		CString &strDef1,
		CString &strDef2,
		WORD &wKeepAtk1,
		WORD &wKeepAtk2,
		WORD &wKeepDef1,
		WORD &wKeepDef2);

	void DelAllMonAtSpawn( LPTMONSPAWN pSPAWN);
	void DelMonSpawn( LPTMONSPAWN pSPAWN);

	void InitMap(
		CSqlDatabase *pDB,
		BYTE bServerID);

	void InitExtCell( WORD wUnitID);
	void InitCell(
		CSqlDatabase *pDB,
		BYTE bServerID,
		WORD wUnitID);

	WORD GetLocalBonus(BYTE bDefCountry, WORD wDPoint, WORD wCPoint);
	void AddTNMTPlayer(LPTOURNAMENTPLAYER pPlayer);
	BYTE EndTournament(BYTE & bRet, DWORD & dwWinner, DWORD & dwLoser, BYTE bStep = 0);
	void ResetTournament();
	BYTE CanDoTournament(BYTE bStep = 0);
	void SelectWinner();
	void SetHideTick(DWORD dwCharID, DWORD Tick);

public:
	CTMap( CTMap *pTEMP, BYTE bCountryID);
	CTMap();
	virtual ~CTMap();
};
