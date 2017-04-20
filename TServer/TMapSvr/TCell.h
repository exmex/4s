#pragma once


class CTCell
{
public:
	MAPTRECALLMON m_mapRECALLMON;
	MAPTSELFOBJ m_mapSELFOBJ;

	MAPTMONSTER m_mapMONSTER;
	MAPPLAYER m_mapPLAYER;

	MAPTSWITCH m_mapSWITCH;
	MAPTGATE m_mapGATE;

public:
	BYTE m_vServerID[UD_COUNT][MAX_CHANNEL];
	BYTE m_vExtCell[MAX_CHANNEL];
	BYTE m_vEnable[MAX_CHANNEL];

	DWORD m_dwID;

public:
	BYTE IsMainCell( BYTE bChannel);
	BYTE IsEnable( BYTE bChannel);

	void AddPlayer( CTPlayer *pPlayer);
	void DelPlayer( DWORD dwID);
	void AddSwitch( LPTSWITCH pSwitch);
	void AddGate( LPTGATE pGate);
	void AddSelfObj( CTSelfObj *pSelf);
	void DelSelfObj( DWORD dwID);

	void EnterPlayer(
		CTPlayer *pPlayer,
		CTCell *pCELL,
		BYTE bNewMember,
		DWORD dwTick);

	void LeavePlayer(
		CTPlayer *pPlayer,
		BYTE bExitMap);

	void LeaveChannel( CTPlayer *pPlayer);
	void LeaveChannel(
		CTPlayer *pPlayer,
		BYTE bChannel,
		BYTE bExitMap);

	void LeaveChannel(
		CTPlayer *pPlayer,
		BYTE bExitMap);

	void LeaveChannel(
		CTMonster *pMON,
		BYTE bChannel,
		BYTE bExitMap);

	void LeaveChannel(
		CTMonster *pMON,
		BYTE bExitMap);

	void AddMonster( CTMonster *pMON);
	void DelMonster( CTMonster *pMON);

	void EnterMonster(
		CTMonster *pMON,
		CTCell *pCELL,
		BYTE bNewMember,
		DWORD dwTick);

	void LeaveMonster(
		CTMonster *pMON,
		BYTE bExitMap);

public:
	CTCell();
	virtual ~CTCell();
};
