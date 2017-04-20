#include "StdAfx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

VBYTE CTMap::m_vTCHANNEL;


CTMap::CTMap()
{
	m_mapTMONSPAWN.clear();
	m_mapTMONSTER.clear();
	m_mapTSPAWNPOS.clear();
	m_mapTSWITCH.clear();
	m_mapTGODTOWER.clear();
	m_mapTGODBALL.clear();
	m_mapTTEMPGODBALL.clear();
	m_mapTNMTPlayer.clear();

	m_mapTUNIT.clear();
	m_mapTCELL.clear();

	m_wPartyID = 0;
	m_wMapID = 0;

	m_bType = MAP_WORLD;
	m_bServerID = 0;
	m_bChannel = 0;

	m_bDefBallMax = 0;
	m_bAtkBallMax = 0;
	m_bBlueTeam = TOURNAMENT_SLOT;
	m_bRedTeam = TOURNAMENT_SLOT;

	m_pCastle = NULL;
	m_dwDefPower = 0;
	m_dwAtkPower = 0;
	m_wDefKillPoint = 0;
	m_wAtkKillPoint = 0;
	m_dwBlueHideTick = 0;
	m_dwRedHideTick = 0;

	m_dwEndTick = 0;
}

CTMap::CTMap( CTMap *pTEMP, BYTE bCountryID)
{
	m_bDefBallMax = 0;
	m_bAtkBallMax = 0;
	m_bBlueTeam = TOURNAMENT_SLOT;
	m_bRedTeam = TOURNAMENT_SLOT;

	m_pCastle = NULL;
	m_dwDefPower = 0;
	m_dwAtkPower = 0;
	m_wDefKillPoint = 0;
	m_wAtkKillPoint = 0;
	m_dwBlueHideTick = 0;
	m_dwRedHideTick = 0;

	m_dwEndTick = 0;

	m_mapTMONSPAWN.clear();
	m_mapTMONSTER.clear();
	m_mapTSPAWNPOS.clear();
	m_mapTNMTPlayer.clear();

	m_mapTUNIT.clear();
	m_mapTCELL.clear();
	m_wPartyID = 0;

	m_bServerID = pTEMP->m_bServerID;
	m_bChannel = pTEMP->m_bChannel;
	m_wMapID = pTEMP->m_wMapID;
	m_bType = MAP_INDUN;

	MAPTMONSPAWN::iterator itSPAWN;
	MAPTSPAWNPOS::iterator itPOS;
	MAPTSWITCH::iterator itSWITCH;
	MAPTCELL::iterator itCELL;
	MAPWORD::iterator itUNIT;
	MAPTGODTOWER::iterator itGT;
	MAPTGODBALL::iterator itGB;

	for( itUNIT = pTEMP->m_mapTUNIT.begin(); itUNIT != pTEMP->m_mapTUNIT.end(); itUNIT++)
		m_mapTUNIT.insert( MAPWORD::value_type( (*itUNIT).first, (*itUNIT).second));

	for( itPOS = pTEMP->m_mapTSPAWNPOS.begin(); itPOS != pTEMP->m_mapTSPAWNPOS.end(); itPOS++)
		m_mapTSPAWNPOS.insert( MAPTSPAWNPOS::value_type( (*itPOS).first, (*itPOS).second));

	for( itCELL = pTEMP->m_mapTCELL.begin(); itCELL != pTEMP->m_mapTCELL.end(); itCELL++)
	{
		CTCell *pTCELL = new CTCell();

		for( BYTE i=0; i<MAX_CHANNEL; i++)
		{
			for( BYTE j=0; j<UD_COUNT; j++)
				pTCELL->m_vServerID[j][i] = (*itCELL).second->m_vServerID[j][i];

			pTCELL->m_vExtCell[i] = (*itCELL).second->m_vExtCell[i];
			pTCELL->m_vEnable[i] = (*itCELL).second->m_vEnable[i];
		}
		pTCELL->m_dwID = (*itCELL).second->m_dwID;

		m_mapTCELL.insert( MAPTCELL::value_type( pTCELL->m_dwID, pTCELL));
	}

	for( itSWITCH = pTEMP->m_mapTSWITCH.begin(); itSWITCH != pTEMP->m_mapTSWITCH.end(); itSWITCH++)
	{
		LPTSWITCH pTSWITCH = new TSWITCH();

		pTSWITCH->m_bLockOnClose = (*itSWITCH).second->m_bLockOnClose;
		pTSWITCH->m_bLockOnOpen = (*itSWITCH).second->m_bLockOnOpen;
		pTSWITCH->m_dwSwitchID = (*itSWITCH).second->m_dwSwitchID;
		pTSWITCH->m_dwDuration = (*itSWITCH).second->m_dwDuration;
		pTSWITCH->m_bOpened = (*itSWITCH).second->m_bOpened;
		pTSWITCH->m_wPosX = (*itSWITCH).second->m_wPosX;
		pTSWITCH->m_wPosY = (*itSWITCH).second->m_wPosY;
		pTSWITCH->m_wPosZ = (*itSWITCH).second->m_wPosZ;

		pTSWITCH->m_wMapID = m_wMapID;
		pTSWITCH->m_dwStartTime = 0;

		EnterMAP(pTSWITCH);
	}

	for( itSWITCH = pTEMP->m_mapTSWITCH.begin(); itSWITCH != pTEMP->m_mapTSWITCH.end(); itSWITCH++)
	{
		LPTSWITCH pTSWITCH = FindSwitch((*itSWITCH).first);

		if(pTSWITCH)
			for( DWORD i=0; i<DWORD((*itSWITCH).second->m_vGate.size()); i++)
			{
				MAPTGATE::iterator itGATE = m_mapTGATE.find((*itSWITCH).second->m_vGate[i]->m_dwGateID);

				if( itGATE == m_mapTGATE.end() || (*itSWITCH).second->m_vGate[i]->m_bType != GT_SELECTSWITCH )
				{
					LPTSWITCH pSwitch = pTSWITCH;

					if( (*itSWITCH).second->m_vGate[i]->m_bType == GT_SELECTSWITCH &&
						!(*itSWITCH).second->m_vGate[i]->m_vSwitch.empty())
					{
						BYTE bSELECT = BYTE(rand() % BYTE((*itSWITCH).second->m_vGate[i]->m_vSwitch.size()));
						pSwitch = FindSwitch((*itSWITCH).second->m_vGate[i]->m_vSwitch[bSELECT]->m_dwSwitchID);
					}

					if( itGATE == m_mapTGATE.end() )
					{
						LPTGATE pTGATE = new TGATE();

						pTGATE->m_dwSwitchID = (*itSWITCH).second->m_vGate[i]->m_dwSwitchID;
						pTGATE->m_dwGateID = (*itSWITCH).second->m_vGate[i]->m_dwGateID;
						pTGATE->m_bOpened = (*itSWITCH).second->m_vGate[i]->m_bOpened;
						pTGATE->m_wPosX = (*itSWITCH).second->m_vGate[i]->m_wPosX;
						pTGATE->m_wPosY = (*itSWITCH).second->m_vGate[i]->m_wPosY;
						pTGATE->m_wPosZ = (*itSWITCH).second->m_vGate[i]->m_wPosZ;
						pTGATE->m_bType = (*itSWITCH).second->m_vGate[i]->m_bType;
						pTGATE->m_wMapID = m_wMapID;

						pTGATE->m_vSwitch.push_back(pSwitch);
						pSwitch->m_vGate.push_back(pTGATE);

						EnterMAP(pTGATE);
					}
					else
					{
						(*itGATE).second->m_vSwitch.push_back(pSwitch);
						pSwitch->m_vGate.push_back((*itGATE).second);
					}
				}
			}
	}

	for(itGT = pTEMP->m_mapTGODTOWER.begin(); itGT != pTEMP->m_mapTGODTOWER.end(); itGT++)
	{
		LPTGODTOWER pGodTower = new TGODTOWER();
		pGodTower->m_wID = (*itGT).second->m_wID;
		pGodTower->m_pGodBall = NULL;
		pGodTower->m_fPosX = (*itGT).second->m_fPosX;
		pGodTower->m_fPosY = (*itGT).second->m_fPosY;
		pGodTower->m_fPosZ = (*itGT).second->m_fPosZ;

		EnterMAP(pGodTower);
	}

	for( itGB = pTEMP->m_mapTTEMPGODBALL.begin(); itGB != pTEMP->m_mapTTEMPGODBALL.end(); itGB++)
	{
		LPTGODBALL pGodBall = new TGODBALL();
		pGodBall->m_wID = (*itGB).second->m_wID;
		pGodBall->m_bCamp = (*itGB).second->m_bCamp;
		pGodBall->m_fPosX = (*itGB).second->m_fPosX;
		pGodBall->m_fPosY = (*itGB).second->m_fPosY;
		pGodBall->m_fPosZ = (*itGB).second->m_fPosZ;
		pGodBall->m_pMAP = this;
		pGodBall->m_bMove = TRUE;
		pGodBall->m_dwGuildID = 0;
		pGodBall->m_dwKEY = 0;
		m_mapTTEMPGODBALL.insert(MAPTGODBALL::value_type(pGodBall->m_wID, pGodBall));
	}

	for( itSPAWN = pTEMP->m_mapTMONSPAWN.begin(); itSPAWN != pTEMP->m_mapTMONSPAWN.end(); itSPAWN++)
		AddMonSpawn( (*itSPAWN).second->m_pSPAWN, m_bChannel, bCountryID);
}

CTMap::~CTMap()
{
	MAPTMONSPAWN::iterator itSPAWN;
	MAPTCELL::iterator itCELL;
	MAPTSWITCH::iterator itSwc;
	MAPTGATE::iterator itGt;
	MAPTSELFOBJ::iterator itSfObj;
	MAPTGODTOWER::iterator itGT;
	MAPTGODBALL::iterator itGB;
	MAPTGODBALL::iterator itGBT;

	for( itSPAWN = m_mapTMONSPAWN.begin(); itSPAWN != m_mapTMONSPAWN.end(); itSPAWN++)
	{
		if(itSPAWN->second->m_pSPAWN->m_bEvent == SE_DYNAMIC)
		{
			_AtlModule.m_mapTMONSPAWN.erase(itSPAWN->second->m_pSPAWN->m_wID);
			_AtlModule.m_mapExtraSpawnID.insert(MAPWORD::value_type(itSPAWN->second->m_pSPAWN->m_wID,itSPAWN->second->m_pSPAWN->m_wID));
		}

		delete (*itSPAWN).second;
	}

	for( itCELL = m_mapTCELL.begin(); itCELL != m_mapTCELL.end(); itCELL++)
		delete (*itCELL).second;

	for(itSwc=m_mapTSWITCH.begin(); itSwc!=m_mapTSWITCH.end(); itSwc++)
		delete (*itSwc).second;

	for(itGt = m_mapTGATE.begin(); itGt != m_mapTGATE.end(); itGt++)
		delete (*itGt).second;

	for(itGT = m_mapTGODTOWER.begin(); itGT != m_mapTGODTOWER.end(); itGT++)
	{
		if((*itGT).second->m_pGodBall)
			delete (*itGT).second->m_pGodBall;

		delete (*itGT).second;
	}

	for( itGB = m_mapTGODBALL.begin(); itGB != m_mapTGODBALL.end(); itGB++)
		delete (*itGB).second;

	for( itGBT = m_mapTTEMPGODBALL.begin(); itGBT != m_mapTTEMPGODBALL.end(); itGBT++)
		delete (*itGBT).second;

	m_mapTSWITCH.clear();
	m_mapTGATE.clear();
	m_mapTMONSPAWN.clear();
	m_mapTMONSTER.clear();
	m_mapTSPAWNPOS.clear();
	m_mapTGODTOWER.clear();
	m_mapTGODBALL.clear();
	m_mapTTEMPGODBALL.clear();
	m_mapTNMTPlayer.clear();
	m_mapTUNIT.clear();
	m_mapTCELL.clear();
}

void CTMap::InitMap( CSqlDatabase *pDB,
					 BYTE bServerID)
{
	MAPWORD::iterator itUNIT;

	for( itUNIT = m_mapTUNIT.begin(); itUNIT != m_mapTUNIT.end(); itUNIT++)
		InitCell( pDB, bServerID, (*itUNIT).second);

	for( itUNIT = m_mapTUNIT.begin(); itUNIT != m_mapTUNIT.end(); itUNIT++)
		InitExtCell((*itUNIT).second);

	m_bServerID = bServerID;
}

void CTMap::InitCell( CSqlDatabase *pDB,
					  BYTE bServerID,
					  WORD wUnitID)
{
	static int nCELL[UD_COUNT][2] = {
		{ -1, -1},
		{  0, -1},
		{  1, -1},
		{ -1,  0},
		{  1,  0},
		{ -1,  1},
		{  0,  1},
		{  1,  1}};

	BYTE vServerID[UD_COUNT][MAX_CHANNEL];
	BYTE vEnable[MAX_CHANNEL];

	BYTE bUnitX = LOBYTE(wUnitID);
	BYTE bUnitY = HIBYTE(wUnitID);

	for( int i=0; i<MAX_CHANNEL; i++)
	{
		for( int j=0; j<UD_COUNT; j++)
			vServerID[j][i] = bServerID;

		vEnable[i] = FALSE;
	}

	DEFINE_QUERY( pDB, CSPGetServerID)
	for(int i=0; i<UD_COUNT; i++)
	{
		query->m_wUnitID = INT(bUnitX) + nCELL[i][0] < 0 || INT(bUnitY) + nCELL[i][1] < 0 ? wUnitID : MAKEWORD( BYTE(INT(bUnitX) + nCELL[i][0]), BYTE(INT(bUnitY) + nCELL[i][1]));
		query->m_wMapID = m_wMapID;

		for( int j=0; j<INT(m_vTCHANNEL.size()); j++)
		{
			query->m_bChannel = m_vTCHANNEL[j];

			if( query->Call() && !query->m_nRET )
				vServerID[i][m_vTCHANNEL[j]] = query->m_bServerID;
		}
	}

	query->m_wUnitID = wUnitID;
	query->m_wMapID = m_wMapID;

	for(int i=0; i<INT(m_vTCHANNEL.size()); i++)
	{
		query->m_bChannel = m_vTCHANNEL[i];

		if( query->Call() && !query->m_nRET )
			vEnable[m_vTCHANNEL[i]] = query->m_bServerID == bServerID ? TRUE : FALSE;
	}
	UNDEFINE_QUERY()

	WORD wCount = UNIT_SIZE / CELL_SIZE;
	for(int i=0; i<wCount; i++)
	{
		WORD wCellY = WORD(bUnitY) * wCount + i;

		for( int j=0; j<wCount; j++)
		{
			CTCell *pCell = new CTCell();

			BYTE vCheck[UD_COUNT] = {
				i == 0 && j == 0 ? TRUE : FALSE,
				i == 0 ? TRUE : FALSE,
				i == 0 && j + 1 >= wCount ? TRUE : FALSE,
				j == 0 ? TRUE : FALSE,
				j + 1 >= wCount ? TRUE : FALSE,
				i + 1 >= wCount && j == 0 ? TRUE : FALSE,
				i + 1 >= wCount ? TRUE : FALSE,
				i + 1 >= wCount && j + 1 >= wCount ? TRUE : FALSE};
			WORD wCellX = WORD(bUnitX) * wCount + j;

			pCell->m_dwID = MAKELONG( wCellX, wCellY);
			for( int k=0; k<MAX_CHANNEL; k++)
				pCell->m_vEnable[k] = vEnable[k];

			for(int k=0; k<UD_COUNT; k++)
				for( int l=0; l<MAX_CHANNEL; l++)
					pCell->m_vServerID[k][l] = vCheck[k] && vServerID[k][l] != bServerID ? vServerID[k][l] : 0;

			m_mapTCELL.insert( MAPTCELL::value_type( pCell->m_dwID, pCell));
		}
	}
}

void CTMap::InitExtCell( WORD wUnitID)
{
	static int nCELL[UD_COUNT][2] = {
		{ -1, -1},
		{  0, -1},
		{  1, -1},
		{ -1,  0},
		{  1,  0},
		{ -1,  1},
		{  0,  1},
		{  1,  1}};
	WORD wCount = UNIT_SIZE / CELL_SIZE;

	BYTE bUnitX = LOBYTE(wUnitID);
	BYTE bUnitY = HIBYTE(wUnitID);

	for( int i=0; i<wCount; i++)
	{
		WORD wCellY = WORD(bUnitY) * wCount + i;

		for( int j=0; j<wCount; j++)
		{
			WORD wCellX = WORD(bUnitX) * wCount + j;
			MAPTCELL::iterator itCELL = m_mapTCELL.find(MAKELONG( wCellX, wCellY));

			if( itCELL != m_mapTCELL.end() )
			{
				CTCell *pCell = (*itCELL).second;

				for( int k=0; k<UD_COUNT; k++)
				{
					DWORD dwEXT = MAKELONG( WORD(INT(wCellX) + nCELL[k][0]), WORD(INT(wCellY) + nCELL[k][1]));

					for( int l=0; l<INT(m_vTCHANNEL.size()); l++)
						if( pCell->m_vEnable[m_vTCHANNEL[l]] && pCell->m_vServerID[k][m_vTCHANNEL[l]] )
						{
							MAPTCELL::iterator itEXT = m_mapTCELL.find(dwEXT);
							CTCell *pEXT = NULL;

							if( itEXT == m_mapTCELL.end() )
							{
								pEXT = new CTCell();
								pEXT->m_dwID = dwEXT;

								m_mapTCELL.insert( MAPTCELL::value_type( pEXT->m_dwID, pEXT));
							}
							else
								pEXT = (*itEXT).second;

							pEXT->m_vExtCell[m_vTCHANNEL[l]] = TRUE;
							pEXT->m_vEnable[m_vTCHANNEL[l]] = TRUE;
						}
				}
			}
		}
	}
}

void CTMap::DelAllMonAtSpawn( LPTMONSPAWN pSPAWN)
{
	MAPTMONSPAWN::iterator finder = m_mapTMONSPAWN.find(pSPAWN->m_wID);
	if( finder == m_mapTMONSPAWN.end() )
		return;

	while(!(*finder).second->m_vTMON.empty())
	{
		CTMonster *pMON = (*finder).second->m_vTMON.back();
		pMON->m_pMAP->LeaveMAP( pMON, TRUE);

		MAPTMONSTER::iterator itMON = pMON->m_pMAP->m_mapTMONSTER.find(pMON->m_dwID);
		if( itMON != pMON->m_pMAP->m_mapTMONSTER.end() )
			pMON->m_pMAP->m_mapTMONSTER.erase(itMON);

		(*finder).second->m_vTMON.pop_back();
		delete pMON;
	}
}

void CTMap::DelMonSpawn( LPTMONSPAWN pSPAWN)
{
	MAPTMONSPAWN::iterator finder = m_mapTMONSPAWN.find(pSPAWN->m_wID);

	if( finder == m_mapTMONSPAWN.end() )
		return;

	DelAllMonAtSpawn(pSPAWN);
	delete (*finder).second;

	m_mapTMONSPAWN.erase(finder);
}

CTMonSpawn * CTMap::AddTimelimitedMon( LPTMONSPAWN pSPAWN,
							   BYTE bChannel,
							   BYTE bCountry,
							   BYTE bRegenType)
{
	if(pSPAWN->m_lpvGroup && !pSPAWN->m_lpvGroup->empty() )
	{
		MAPTMONSPAWN::iterator itSp = m_mapTMONSPAWN.find(pSPAWN->m_lpvGroup->at(0)->m_wID);
		if(itSp != m_mapTMONSPAWN.end())
		{
			BYTE bGroupOrder = (*itSp).second->m_bGroupOrder + 1;
			BYTE bSize = (BYTE)pSPAWN->m_lpvGroup->size();

			if(bGroupOrder < bSize)	
			{
				pSPAWN = pSPAWN->m_lpvGroup->at(bGroupOrder);
				(*itSp).second->m_bGroupOrder = bGroupOrder;
			}
			else
				return NULL;
		}
		else
			pSPAWN = pSPAWN->m_lpvGroup->at(0);
	}

	MAPTMONSPAWN::iterator finder = m_mapTMONSPAWN.find(pSPAWN->m_wID);
	CTMonSpawn *pMonSpawn = NULL;

	if( finder != m_mapTMONSPAWN.end() )
	{
		if( bRegenType == RT_WAITSPAWN || !(*finder).second->m_vTMON.empty() )
			return NULL;

		pMonSpawn = (*finder).second;
	}
	else
	{
		pMonSpawn = new CTMonSpawn();
		m_mapTMONSPAWN.insert( MAPTMONSPAWN::value_type( pSPAWN->m_wID, pMonSpawn));
	}

	if( bRegenType == RT_WAITSPAWN )
		bRegenType = RT_ETERNAL;

	pMonSpawn->m_pSPAWN = pSPAWN;

	InitMonster(
		pMonSpawn,
		pSPAWN,
		bChannel,
		bCountry,
		bRegenType);

	return pMonSpawn;
}

void CTMap::AddMonSpawn( LPTMONSPAWN pSPAWN,
						 BYTE bChannel,
						 BYTE bCountry)
{
	MAPTMONSPAWN::iterator finder = m_mapTMONSPAWN.find(pSPAWN->m_wID);
	if( finder != m_mapTMONSPAWN.end() )
		return;

	BYTE bRegenType = RT_ETERNAL;

	if(pSPAWN->m_lpvGroup &&
		!pSPAWN->m_lpvGroup->empty() &&
		pSPAWN->m_lpvGroup->back()->m_wID != pSPAWN->m_wID)
		bRegenType = RT_WAITSPAWN;

	CTMonSpawn *pMonSpawn = new CTMonSpawn();
	pMonSpawn->m_pSPAWN = pSPAWN;
	m_mapTMONSPAWN.insert( MAPTMONSPAWN::value_type( pMonSpawn->m_pSPAWN->m_wID, pMonSpawn));

	if( m_bType != MAP_INDUNTEMP )
	{
		InitMonster(
			pMonSpawn,
			pSPAWN,
			bChannel,
			bCountry,
			bRegenType);
	}
}

CTMonSpawn *CTMap::FindMonSpawn( WORD wSpawnID)
{
	MAPTMONSPAWN::iterator finder = m_mapTMONSPAWN.find(wSpawnID);

	if( finder != m_mapTMONSPAWN.end() )
		return (*finder).second;

	return NULL;
}

void CTMap::InitMonster( CTMonSpawn *pMonSpawn,
						 LPTMONSPAWN pSPAWN,
						 BYTE bChannel,
						 BYTE bCountry,
						 BYTE bRegenType)
{
	if( pSPAWN->m_vMAPMON.empty() )
		return;

	BYTE bIndex = 0;

	for( BYTE i=0; i<pSPAWN->m_bCount; i++)
	{
		CTMonster *pMON = new CTMonster();

		pMON->m_dwID = MAKELONG( MAKEWORD( bIndex, bChannel), pSPAWN->m_wID);
		pMON->m_pMAP = this;
		pMON->m_wMapID = m_wMapID;

		pMON->m_pTLEVEL = NULL;
		pMON->m_pTCLASS = NULL;
		pMON->m_pTRACE = NULL;
		pMON->m_pMON = NULL;
		pMON->m_pATTR = NULL;

		pMON->m_dwRegion = pSPAWN->m_dwRegion;
		pMON->m_bCountry = bCountry;
		pMON->m_bChannel = bChannel;

		pMON->m_bEssential = FALSE;
		pMON->m_pSPAWN = pMonSpawn;

		CTInven *pInven = new CTInven();
		pInven->m_bInvenID = INVEN_DEFAULT;
		pInven->m_wItemID = 3;
		pInven->m_pTITEM = _AtlModule.FindTItem(pInven->m_wItemID);

		pMON->m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_DEFAULT, pInven));
		bIndex++;

		m_mapTMONSTER.insert( MAPTMONSTER::value_type( pMON->m_dwID, pMON));
		pMonSpawn->m_vTMON.push_back(pMON);

		switch(bRegenType)
		{
		case RT_ETERNAL		: 
			pMON->Initialize();
			break;
		case RT_TIMELIMIT	:
			{
				pMON->Initialize();
//				pMON->OnEvent( AT_TIMEOUT, 0, 0, 0, 0);
			}

			break;
		}
	}

	for(int i=0; i<INT(pSPAWN->m_vMAPMON.size()); i++)
		if(pSPAWN->m_vMAPMON[i]->m_bEssential)
		{
			LPTMONSTER pTEMP = _AtlModule.FindTMonster(pSPAWN->m_vMAPMON[i]->m_wMonID);

			if(pTEMP)
			{
				CTMonster *pMON = new CTMonster();

				pMON->m_dwID = MAKELONG( MAKEWORD( bIndex, bChannel), pSPAWN->m_wID);
				pMON->m_pMON = pTEMP;
				pMON->m_pMAP = this;
				pMON->m_wMapID = m_wMapID;

				pMON->m_pTLEVEL = _AtlModule.FindTLevel(pMON->m_pMON->m_bLevel);
				pMON->m_pTCLASS = _AtlModule.FindTClass(pMON->m_pMON->m_bClass);
				pMON->m_pTRACE = _AtlModule.FindTRace(pMON->m_pMON->m_bRace);
				pMON->m_pATTR = _AtlModule.FindMonAttr(MAKELONG(pMON->m_pMON->m_wMonAttr, pTEMP->m_bLevel));

				if(!pMON->m_pATTR)
				{
					ATLTRACE(_T("INITMONSTER ATTR NULL ID:%d ATTR ID:%d\n"), pMON->m_pMON->m_wID, MAKELONG(pMON->m_pMON->m_wMonAttr, pTEMP->m_bLevel));
					delete pMON;
					return;
				}

				pMON->m_bLevel = pTEMP->m_bLevel;
				pMON->m_pSPAWN = pMonSpawn;
				pMON->m_bEssential = TRUE;

				pMON->m_dwRegion = pSPAWN->m_dwRegion;
				pMON->m_bCountry = bCountry;
				pMON->m_bChannel = bChannel;

				CTInven *pInven = new CTInven();
				pInven->m_bInvenID = INVEN_DEFAULT;
				pInven->m_wItemID = 3;
				pInven->m_pTITEM = _AtlModule.FindTItem(pInven->m_wItemID);

				pMON->m_mapTINVEN.insert( MAPTINVEN::value_type( INVEN_DEFAULT, pInven));
				bIndex++;

				for( DWORD j=0; j<DWORD(pMON->m_pMON->m_vSKILL.size()); j++)
				{
					CTSkillTemp *pTSKILL = _AtlModule.FindTSkill(pMON->m_pMON->m_vSKILL[j]);

					if(pTSKILL)
					{
						CTSkill *pSkill = new CTSkill();

						pSkill->m_pTSKILL = pTSKILL;
						pSkill->m_bLevel = 1;
						pMON->m_mapTSKILL.insert( MAPTSKILL::value_type( pMON->m_pMON->m_vSKILL[j], pSkill));
					}
				}

				m_mapTMONSTER.insert( MAPTMONSTER::value_type( pMON->m_dwID, pMON));
				pMonSpawn->m_vTMON.push_back(pMON);

				if(pSPAWN->m_vMAPMON[i]->m_bLeader)
					pMonSpawn->m_dwLeader = pMON->m_dwID;

				switch(bRegenType)
				{
				case RT_ETERNAL		: pMON->Initialize(); break;
				case RT_TIMELIMIT	:
					{
						pMON->Initialize();
//						pMON->OnEvent( AT_TIMEOUT, 0, 0, 0, 0);
					}

					break;
				}
			}
		}
}

CTObjBase * CTMap::FindNeighbor(BYTE bType,
								DWORD dwObjID,
								FLOAT fPosX,
								FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
				{
					CTCell *pCell = (*finder).second;
					MAPTRECALLMON::iterator itREC;
					MAPTSELFOBJ::iterator itSELF;
					MAPTMONSTER::iterator itMON;
					MAPPLAYER::iterator itCHAR;

					switch(bType)
					{
					case OT_PC:
						for( itCHAR = pCell->m_mapPLAYER.begin(); itCHAR != pCell->m_mapPLAYER.end(); itCHAR++)
							if((*itCHAR).second->m_dwID == dwObjID)
								return (*itCHAR).second;
						break;
					case OT_MON:
						for( itMON = pCell->m_mapMONSTER.begin(); itMON != pCell->m_mapMONSTER.end(); itMON++)
							if((*itMON).second->m_dwID == dwObjID)
								return (*itMON).second;
						break;
					case OT_RECALL:
						for( itREC = pCell->m_mapRECALLMON.begin(); itREC != pCell->m_mapRECALLMON.end(); itREC++)
							if((*itREC).second->m_dwID == dwObjID)
								return (*itREC).second;
						break;
					case OT_SELF:
						for( itSELF = pCell->m_mapSELFOBJ.begin(); itSELF != pCell->m_mapSELFOBJ.end(); itSELF++)
							if((*itSELF).second->m_dwID == dwObjID)
								return (*itSELF).second;
						break;
					}
				}
			}
		}
	}

	return NULL;
}

void CTMap::GetNeighbor( LPVPLAYER pPLAYERS,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
				{
					CTCell *pCell = (*finder).second;
					MAPPLAYER::iterator it;

					for( it = pCell->m_mapPLAYER.begin(); it != pCell->m_mapPLAYER.end(); it++)
						pPLAYERS->push_back((*it).second);
				}
			}
		}
	}
}

void CTMap::GetNeighbor( LPVTMONSTER pMONSTERS,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
				{
					CTCell *pCell = (*finder).second;
					MAPTMONSTER::iterator it;

					for( it = pCell->m_mapMONSTER.begin(); it != pCell->m_mapMONSTER.end(); it++)
						pMONSTERS->push_back((*it).second);
				}
			}
		}
	}
}

void CTMap::GetNeerPlayer(LPVPLAYER pPLAYERS,
                          FLOAT fPosX,
                          FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
				{
					CTCell *pCell = (*finder).second;
					MAPPLAYER::iterator it;

					for( it = pCell->m_mapPLAYER.begin(); it != pCell->m_mapPLAYER.end(); it++)
					{
						if(GetDistance(fPosX, fPosZ, (*it).second->m_fPosX, (*it).second->m_fPosZ) <= CELL_SIZE)
							pPLAYERS->push_back((*it).second);
					}
				}
			}
		}
	}
}

void CTMap::GetNeerMonster(LPVTMONSTER pMONSTERS,
                           FLOAT fPosX,
                           FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
				{
					CTCell *pCell = (*finder).second;
					MAPTMONSTER::iterator it;

					for( it = pCell->m_mapMONSTER.begin(); it != pCell->m_mapMONSTER.end(); it++)
					{
						if(GetDistance(fPosX, fPosZ, (*it).second->m_fPosX, (*it).second->m_fPosZ) <= CELL_SIZE)
							pMONSTERS->push_back((*it).second);
					}
				}
			}
		}
	}
}

void CTMap::GetNeighbor( LPMAPTCELL pCELLS,
						 FLOAT fPosX,
						 FLOAT fPosZ)
{
	int nPosX = WORD(fPosX) / CELL_SIZE - 1;
	int nPosZ = WORD(fPosZ) / CELL_SIZE - 1;

	for( int i=0; i<3; i++)
	{
		int nLocZ = nPosZ + i;

		for( int j=0; j<3; j++)
		{
			int nLocX = nPosX + j;

			if( 0 <= nLocX && 0 <= nLocZ )
			{
				MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG((WORD) nLocX, (WORD) nLocZ));

				if( finder != m_mapTCELL.end() )
					pCELLS->insert( MAPTCELL::value_type( (*finder).first, (*finder).second));
			}
		}
	}
}

void CTMap::EnterMAP( CTPlayer *pPlayer,
					  BYTE bNewMember)
{
	pPlayer->m_pMAP = this;

	WORD wPosX = WORD(pPlayer->m_fPosX) / CELL_SIZE;
	WORD wPosZ = WORD(pPlayer->m_fPosZ) / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( finder != m_mapTCELL.end() &&
		(*finder).second->IsEnable(pPlayer->m_bChannel))
	{
		if(m_pCastle && bNewMember)
		{
			MAPTGODBALL::iterator itGb;
			for(itGb = m_mapTGODBALL.begin(); itGb!=m_mapTGODBALL.end(); itGb++)
				pPlayer->SendCS_ADDGODBALL_ACK((*itGb).second);

			MAPTGODTOWER::iterator itGt;
			for(itGt = m_mapTGODTOWER.begin(); itGt!=m_mapTGODTOWER.end(); itGt++)
				pPlayer->SendCS_ADDGODTOWER_ACK((*itGt).second);
		}

		MAPTCELL::iterator it;
		MAPTCELL mapTCELL;

		(*finder).second->AddPlayer(pPlayer);
		GetNeighbor(
			&mapTCELL,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		for( it = mapTCELL.begin(); it != mapTCELL.end(); it++)
			(*it).second->EnterPlayer( pPlayer, (*finder).second, bNewMember, _AtlModule.m_dwTick);
	}
}

void CTMap::EnterMAP( CTMonster *pMON)
{
	WORD wPosX = WORD(pMON->m_fPosX) / CELL_SIZE;
	WORD wPosZ = WORD(pMON->m_fPosZ) / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( pMON->m_bType == OT_RECALL )
		((CTRecallMon *) pMON)->m_bMain = FALSE;

	if( finder != m_mapTCELL.end() &&
		(pMON->m_bType != OT_RECALL ||
		(*finder).second->IsEnable(pMON->m_bChannel)))
	{
		MAPTCELL::iterator it;
		MAPTCELL mapTCELL;

		if( pMON->m_bType == OT_RECALL )
			((CTRecallMon *) pMON)->m_bMain = (*finder).second->IsMainCell(pMON->m_bChannel);

		(*finder).second->AddMonster(pMON);

		GetNeighbor(
			&mapTCELL,
			pMON->m_fPosX,
			pMON->m_fPosZ);

		for( it = mapTCELL.begin(); it != mapTCELL.end(); it++)
			(*it).second->EnterMonster( pMON, (*finder).second, TRUE, _AtlModule.m_dwTick);
	}
	pMON->m_pMAP = this;

	if( pMON->m_bType == OT_MON )
		pMON->OnEvent( AT_ENTER, 0, 0, 0, 0);
}

BYTE CTMap::EnterMAP( LPTSWITCH pSwitch)
{
	WORD wPosX = pSwitch->m_wPosX / CELL_SIZE;
	WORD wPosZ = pSwitch->m_wPosZ / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( finder != m_mapTCELL.end() )
		(*finder).second->AddSwitch(pSwitch);

	pSwitch->m_pMAP = this;
	m_mapTSWITCH.insert( MAPTSWITCH::value_type( pSwitch->m_dwSwitchID, pSwitch));

	return TRUE;
}

BYTE CTMap::EnterMAP( LPTGATE pGate)
{
	MAPTGATE::iterator itGATE = m_mapTGATE.find(pGate->m_dwGateID);

	if( itGATE != m_mapTGATE.end() )
		return TRUE;

	WORD wPosX = pGate->m_wPosX / CELL_SIZE;
	WORD wPosZ = pGate->m_wPosZ / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( finder != m_mapTCELL.end() )
		(*finder).second->AddGate(pGate);

	m_mapTGATE.insert( MAPTGATE::value_type( pGate->m_dwGateID, pGate));

	return TRUE;
}

BYTE CTMap::EnterMAP( LPTGODTOWER pGodTower)
{
	pGodTower->m_pMAP = this;
	m_mapTGODTOWER.insert( MAPTGODTOWER::value_type( pGodTower->m_wID, pGodTower));

	return TRUE;
}

BYTE CTMap::EnterMAP( LPTGODBALL pGodBall)
{
	pGodBall->m_pMAP = this;
	pGodBall->m_bGround = TRUE;

	VPLAYER vPlayer;
	vPlayer.clear();
	GetMapPlayers(&vPlayer);
	while(!vPlayer.empty())
	{
		CTPlayer * pChar = vPlayer.back();
		pChar->SendCS_ADDGODBALL_ACK( pGodBall );
		vPlayer.pop_back();
	}

	MAPTGODBALL::iterator itGB = m_mapTGODBALL.find(pGodBall->m_wID);
	if(itGB == m_mapTGODBALL.end())
		m_mapTGODBALL.insert( MAPTGODBALL::value_type( pGodBall->m_wID, pGodBall));

	return TRUE;
}

void CTMap::LeaveMAP( CTPlayer *pPlayer)
{
	WORD wPosX = WORD(pPlayer->m_fPosX) / CELL_SIZE;
	WORD wPosZ = WORD(pPlayer->m_fPosZ) / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( finder != m_mapTCELL.end() )
		(*finder).second->DelPlayer(pPlayer->m_dwID);

	MAPTCELL::iterator it;
	MAPTCELL mapTCELL;
	mapTCELL.clear();

	GetNeighbor(
		&mapTCELL,
		pPlayer->m_fPosX,
		pPlayer->m_fPosZ);

	for( it = mapTCELL.begin(); it != mapTCELL.end(); it++)
		(*it).second->LeavePlayer( pPlayer, TRUE);

	pPlayer->m_pMAP = NULL;

	mapTCELL.clear();
}

void CTMap::LeaveMAP( CTMonster *pMON, BYTE bExitMap)
{
	WORD wPosX = WORD(pMON->m_fPosX) / CELL_SIZE;
	WORD wPosZ = WORD(pMON->m_fPosZ) / CELL_SIZE;

	MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG( wPosX, wPosZ));
	if( finder != m_mapTCELL.end() )
		(*finder).second->DelMonster(pMON);

	MAPTCELL::iterator it;
	MAPTCELL mapTCELL;

	GetNeighbor(
		&mapTCELL,
		pMON->m_fPosX,
		pMON->m_fPosZ);

	for( it = mapTCELL.begin(); it != mapTCELL.end(); it++)
		(*it).second->LeaveMonster( pMON, bExitMap);
}

void CTMap::LeaveMAP(LPTGODBALL pGodBall, BYTE bErase)
{
	if(pGodBall->m_bGround)
	{
		VPLAYER vPlayer;
		vPlayer.clear();
		GetMapPlayers(&vPlayer);
		while(!vPlayer.empty())
		{
			CTPlayer * pChar = vPlayer.back();
			pChar->SendCS_DELGODBALL_ACK( pGodBall->m_wID );
			vPlayer.pop_back();
		}

		pGodBall->m_bGround = FALSE;
	}

	if(bErase)
		m_mapTGODBALL.erase(pGodBall->m_wID);
}

void CTMap::OnMove( CTPlayer *pPlayer,
				    FLOAT fPosX,
					FLOAT fPosZ)
{
	WORD wCellX = WORD(pPlayer->m_fPosX) / CELL_SIZE;
	WORD wCellZ = WORD(pPlayer->m_fPosZ) / CELL_SIZE;
	WORD wNewX = WORD(fPosX) / CELL_SIZE;
	WORD wNewZ = WORD(fPosZ) / CELL_SIZE;

	DWORD dwOLD = MAKELONG( wCellX, wCellZ);
	DWORD dwNEW = MAKELONG( wNewX, wNewZ);

	if( dwOLD != dwNEW )
	{
		MAPTCELL::iterator finder;
		MAPTCELL::iterator it;
		MAPBYTE::iterator itCH;

		CTCell *pOLD = NULL;
		CTCell *pNEW = NULL;

		finder = m_mapTCELL.find(dwOLD);
		if( finder != m_mapTCELL.end() && (*finder).second->IsEnable(pPlayer->m_bChannel) )
			pOLD = (*finder).second;

		finder = m_mapTCELL.find(dwNEW);
		if( finder != m_mapTCELL.end() && (*finder).second->IsEnable(pPlayer->m_bChannel) )
			pNEW = (*finder).second;

		if(!pNEW && m_bType == MAP_INDUN)
		{
			pPlayer->CloseSession();
			return;
		};

		if(pPlayer->m_bMain) // Check connection
		{
			MAPBYTE mapCON;
			mapCON.clear();

			if( pNEW && pNEW->IsMainCell(pPlayer->m_bChannel) )
			{
				for( BYTE i=0; i<UD_COUNT; i++)
					if(pNEW->m_vServerID[i][pPlayer->m_bChannel])
						mapCON.insert( MAPBYTE::value_type( pNEW->m_vServerID[i][pPlayer->m_bChannel], pNEW->m_vServerID[i][pPlayer->m_bChannel]));
				mapCON.insert( MAPBYTE::value_type( m_bServerID, m_bServerID));
			}

			_AtlModule.SendMW_CHECKCONNECT_ACK(
				pPlayer->m_dwID,
				pPlayer->m_dwKEY,
				pPlayer->m_bChannel,
				pPlayer->m_wMapID,
				fPosX,
				pPlayer->m_fPosY,
				fPosZ,
				&mapCON);

			mapCON.clear();
		}

		MAPTCELL mapOLD;
		MAPTCELL mapNEW;
		MAPBYTE mapCH;

		mapOLD.clear();
		mapNEW.clear();
		mapCH.clear();

		if(pOLD)
		{
			for( BYTE i=0; i<BYTE(m_vTCHANNEL.size()); i++)
				if(pOLD->IsMainCell(m_vTCHANNEL[i]))
					mapCH.insert( MAPBYTE::value_type( m_vTCHANNEL[i], m_vTCHANNEL[i]));

			pOLD->DelPlayer(pPlayer->m_dwID);
		}

		GetNeighbor(
			&mapOLD,
			pPlayer->m_fPosX,
			pPlayer->m_fPosZ);

		GetNeighbor(
			&mapNEW,
			fPosX,
			fPosZ);

		for( it = mapOLD.begin(); it != mapOLD.end(); it++)
		{
			finder = mapNEW.find((*it).first);

			if( finder == mapNEW.end() )
				(*it).second->LeavePlayer( pPlayer, FALSE);
		}

		pPlayer->m_fPosX = fPosX;
		pPlayer->m_fPosZ = fPosZ;

		if(pNEW)
		{
			for( BYTE i=0; i<BYTE(m_vTCHANNEL.size()); i++)
				if(pNEW->IsMainCell(m_vTCHANNEL[i]))
				{
					itCH = mapCH.find(m_vTCHANNEL[i]);

					if( itCH != mapCH.end() )
						mapCH.erase(itCH);
				}

			pNEW->AddPlayer(pPlayer);
		}
		BYTE bLeaveCH = mapCH.find(pPlayer->m_bChannel) != mapCH.end() ? TRUE : FALSE;

		for( it = mapNEW.begin(); it != mapNEW.end(); it++)
		{
			finder = mapOLD.find((*it).first);

			if( finder == mapOLD.end() && pNEW )
				(*it).second->EnterPlayer( pPlayer, pNEW, FALSE, _AtlModule.m_dwTick);

			if(!bLeaveCH)
			{
				for( itCH = mapCH.begin(); itCH != mapCH.end(); itCH++)
					(*it).second->LeaveChannel( pPlayer, (*itCH).second, FALSE);

				if(!mapCH.empty())
					(*it).second->LeaveChannel(pPlayer);
			}
			else
				(*it).second->LeaveChannel( pPlayer, FALSE);
		}

		mapOLD.clear();
		mapNEW.clear();
		mapCH.clear();
	}
	else
	{
		pPlayer->m_fPosX = fPosX;
		pPlayer->m_fPosZ = fPosZ;
	}
}

void CTMap::OnMove( CTMonster *pMON,
				    FLOAT fPosX,
					FLOAT fPosZ)
{
	WORD wCellX = WORD(pMON->m_fPosX) / CELL_SIZE;
	WORD wCellZ = WORD(pMON->m_fPosZ) / CELL_SIZE;
	WORD wNewX = WORD(fPosX) / CELL_SIZE;
	WORD wNewZ = WORD(fPosZ) / CELL_SIZE;

	DWORD dwOLD = MAKELONG( wCellX, wCellZ);
	DWORD dwNEW = MAKELONG( wNewX, wNewZ);

	if( dwOLD != dwNEW )
	{
		MAPTCELL::iterator finder = m_mapTCELL.find(dwOLD);
		MAPTCELL::iterator it;
		MAPBYTE::iterator itCH;

		MAPTCELL mapOLD;
		MAPTCELL mapNEW;
		MAPBYTE mapCH;

		mapOLD.clear();
		mapNEW.clear();
		mapCH.clear();

		if( finder != m_mapTCELL.end() && (*finder).second->IsEnable(pMON->m_bChannel) )
		{
			for( BYTE i=0; i<BYTE(m_vTCHANNEL.size()); i++)
				if((*finder).second->IsMainCell(m_vTCHANNEL[i]))
					mapCH.insert( MAPBYTE::value_type( m_vTCHANNEL[i], m_vTCHANNEL[i]));

			(*finder).second->DelMonster(pMON);
		}

		GetNeighbor(
			&mapOLD,
			pMON->m_fPosX,
			pMON->m_fPosZ);

		GetNeighbor(
			&mapNEW,
			fPosX,
			fPosZ);

		for( it = mapOLD.begin(); it != mapOLD.end(); it++)
		{
			finder = mapNEW.find((*it).first);

			if( finder == mapNEW.end() )
				(*it).second->LeaveMonster( pMON, FALSE);
		}

		pMON->m_fPosX = fPosX;
		pMON->m_fPosZ = fPosZ;

		finder = m_mapTCELL.find(dwNEW);
		if( finder != m_mapTCELL.end() && (*finder).second->IsEnable(pMON->m_bChannel) )
		{
			CTCell *pCell = (*finder).second;

			BYTE bEnterCH = FALSE;
			BYTE bLeaveCH = FALSE;

			for( BYTE i=0; i<BYTE(m_vTCHANNEL.size()); i++)
				if( pCell->IsMainCell(m_vTCHANNEL[i]) && mapCH.find(m_vTCHANNEL[i]) == mapCH.end() )
				{
					bEnterCH = TRUE;
					break;
				}

			for(int i=0; i<BYTE(m_vTCHANNEL.size()); i++)
				if(pCell->IsMainCell(m_vTCHANNEL[i]))
				{
					itCH = mapCH.find(m_vTCHANNEL[i]);

					if( itCH != mapCH.end() )
						mapCH.erase(itCH);
				}

			if( pMON->m_bType == OT_RECALL )
			{
//				BYTE bPrevMain = ((CTRecallMon *) pMON)->m_bMain;
//				((CTRecallMon *) pMON)->m_bMain = pCell->IsMainCell(pMON->m_bChannel);
				if(((CTRecallMon *) pMON)->m_bMain && !pCell->IsMainCell(pMON->m_bChannel))
				{
					_AtlModule.SendMW_RECALLMONDATA_ACK((CTRecallMon *) pMON);
					((CTRecallMon *) pMON)->m_bMain = FALSE;
				}

				bLeaveCH = mapCH.find(pMON->m_bChannel) != mapCH.end() ? TRUE : FALSE;
			}
			(*finder).second->AddMonster(pMON);

			for( it = mapNEW.begin(); it != mapNEW.end(); it++)
			{
				if(!bLeaveCH)
					for( itCH = mapCH.begin(); itCH != mapCH.end(); itCH++)
						(*it).second->LeaveChannel( pMON, (*itCH).second, FALSE);
				else
					(*it).second->LeaveChannel( pMON, FALSE);

				if( bEnterCH || bLeaveCH || mapOLD.find((*it).first) == mapOLD.end() )
					(*it).second->EnterMonster( pMON, pCell, FALSE, _AtlModule.m_dwTick);
			}
		}
		pMON->OnEvent(AT_LEAVE, 0, 0, 0, 0);

		mapOLD.clear();
		mapNEW.clear();
		mapCH.clear();
	}
	else
	{
		pMON->m_fPosX = fPosX;
		pMON->m_fPosZ = fPosZ;
	}
}

CTMonster *CTMap::FindMonster( DWORD dwID)
{
	MAPTMONSTER::iterator finder = m_mapTMONSTER.find(dwID);

	if( finder != m_mapTMONSTER.end() &&
		(*finder).second->m_pMON &&
		(*finder).second->m_bStatus != OS_DISAPPEAR)
		return (*finder).second;

	return NULL;
}

LPTSWITCH CTMap::FindSwitch( DWORD dwID)
{
	MAPTSWITCH::iterator finder = m_mapTSWITCH.find(dwID);

	if( finder != m_mapTSWITCH.end() )
		return (*finder).second;

	return NULL;
}

LPTGODBALL CTMap::FindGodBall(WORD wBall)
{
	MAPTGODBALL::iterator it = m_mapTGODBALL.find(wBall);
	if(it!=m_mapTGODBALL.end())
		return (*it).second;
	else
	{
		MAPTGODTOWER::iterator itTw;
		for(itTw=m_mapTGODTOWER.begin(); itTw!=m_mapTGODTOWER.end(); itTw++)
		{
			if((*itTw).second->m_pGodBall && (*itTw).second->m_pGodBall->m_wID == wBall)
				return (*itTw).second->m_pGodBall;
		}

		return NULL;
	}
}

LPTGODBALL CTMap::FindMapGodBall(WORD wBall)
{
	MAPTGODBALL::iterator it = m_mapTGODBALL.find(wBall);
	if(it!=m_mapTGODBALL.end())
		return (*it).second;
	else
		return NULL;
}

LPTGODTOWER CTMap::FindGodTower(WORD wTower)
{
	MAPTGODTOWER::iterator it = m_mapTGODTOWER.find(wTower);
	if(it!=m_mapTGODTOWER.end())
		return (*it).second;
	else
		return NULL;
}

LPTGODBALL CTMap::GenGodBall(BYTE bCamp, WORD & wID)
{
	if(!wID)
	{
		BYTE bCreate = FALSE;

		WORD wTs = WORD(m_mapTGODTOWER.size());
		if(bCamp == CAMP_DEFEND)
		{
			for(WORD i=1; i<=wTs; i++)
			{
				if(!FindGodBall(i))
				{
					wID = i;
					bCreate = TRUE;
					break;
				}
			}
		}
		else
		{
			for(WORD i=wTs+1; i<=wTs*2; i++)
			{
				if(!FindGodBall(i))
				{
					wID = i;
					bCreate = TRUE;
					break;
				}
			}
		}

		if(!bCreate)
			return NULL;
	}

	MAPTGODBALL::iterator itTemp;
	for(itTemp=m_mapTTEMPGODBALL.begin(); itTemp!=m_mapTTEMPGODBALL.end(); itTemp++)
	{
		LPTGODBALL pTempBall = (*itTemp).second;
		if(pTempBall->m_bCamp == bCamp && pTempBall->m_bMove)
		{
			pTempBall->m_bMove = FALSE;
			return pTempBall;
		}
	}

	return NULL;
}

void CTMap::InitGodBall(BYTE bCamp, DWORD dwGuild, WORD wCastle)
{
	WORD wID = 0;
	LPTGODBALL pTemp = GenGodBall(bCamp, wID);
	if(pTemp)
	{
		if(wID)
		{
			LPTGODBALL pBall = new TGODBALL();
			pBall->m_wTempID = pTemp->m_wID;
			pBall->m_bCamp = bCamp;
			pBall->m_dwGuildID = dwGuild;
			pBall->m_dwKEY = 0;
			pBall->m_wID = wID;
			pBall->m_fPosX = pTemp->m_fPosX;
			pBall->m_fPosY = pTemp->m_fPosY;
			pBall->m_fPosZ = pTemp->m_fPosZ;
			pBall->m_wCastle = wCastle;
			pBall->m_strOwner = NAME_NULL;
			pBall->m_dlTime = 0;
			EnterMAP(pBall);
		}
	}
}

void CTMap::MoveTempGodBall(LPTGODBALL pBall)
{
	MAPTGODBALL::iterator itTemp = m_mapTTEMPGODBALL.find(pBall->m_wTempID);
	if(itTemp != m_mapTTEMPGODBALL.end())
		(*itTemp).second->m_bMove = TRUE;
}

void CTMap::ResetTempGodBall()
{
	MAPTGODBALL::iterator itTemp;
	for(itTemp=m_mapTTEMPGODBALL.begin(); itTemp!=m_mapTTEMPGODBALL.end(); itTemp++)
		(*itTemp).second->m_bMove = TRUE;
}

void CTMap::MoveGodBall(LPTGODBALL pBall)
{
	LeaveMAP(pBall, TRUE);

	BYTE bCount = 0;
	MAPTGODBALL::iterator it;
	for(it=m_mapTGODBALL.begin(); it!=m_mapTGODBALL.end(); it++)
	{
		if((*it).second->m_bCamp == pBall->m_bCamp)
			bCount++;
	}

	BYTE bMaxCount = pBall->m_bCamp == CAMP_DEFEND ? m_bDefBallMax : m_bAtkBallMax;
	if(bCount < bMaxCount)
	{
		WORD wID = pBall->m_wID;
		LPTGODBALL pTemp = GenGodBall(pBall->m_bCamp, wID);

		if(pTemp)
		{
			pBall->m_wID = wID;
			pBall->m_fPosX = pTemp->m_fPosX;
			pBall->m_fPosY = pTemp->m_fPosY;
			pBall->m_fPosZ = pTemp->m_fPosZ;
			pBall->m_wTempID = pTemp->m_wID;
			pBall->m_strOwner = NAME_NULL;
			pBall->m_dlTime = 0;
			EnterMAP(pBall);
		}
		else
			delete pBall;
	}
	else
		delete pBall;
}

BYTE CTMap::CheckPower()
{
	if(m_bDefBallMax == 0 || m_bAtkBallMax == 0)
		return FALSE;

	MAPTGODTOWER::iterator it;
	for(it=m_mapTGODTOWER.begin(); it!=m_mapTGODTOWER.end(); it++)
	{
		LPTGODBALL pBall = (*it).second->m_pGodBall;
		if(pBall)
		{
			if(pBall->m_bCamp == CAMP_ATTACK)
			{
				m_dwAtkPower++;
				m_dwDefPower--;
			}
			else if(pBall->m_bCamp == CAMP_DEFEND)
			{
				m_dwDefPower++;
				m_dwAtkPower--;
			}
		}

		if(m_dwAtkPower == 0 || m_dwDefPower == 0)
			return FALSE;
	}

	MAPTCELL::iterator itCell;
	FLOAT fDefPower = m_dwDefPower*100.0f/FLOAT(m_dwDefPower + m_dwAtkPower);
	DWORD dwLeftTime = m_dwEndTick - _AtlModule.m_dwTick < DAY_ONE * 1000 ? m_dwEndTick - _AtlModule.m_dwTick : 0;
	if(dwLeftTime == 0)
		return FALSE;

	CString strAtk1, strAtk2, strDef1, strDef2;
	WORD wKA1, wKA2, wKD1, wKD2;

	GetGodBallOwner(
		strAtk1, strAtk2, strDef1, strDef2,
		wKA1, wKA2, wKD1, wKD2);

	for(itCell=m_mapTCELL.begin(); itCell!=m_mapTCELL.end(); itCell++)
	{
		CTCell * pCell = (*itCell).second;
		MAPPLAYER::iterator itChar;
		for(itChar=pCell->m_mapPLAYER.begin(); itChar != pCell->m_mapPLAYER.end(); itChar++)
		{
			if((*itChar).second->m_bMain)
				(*itChar).second->SendCS_BALANCEOFPOWER_ACK(
					fDefPower,
					dwLeftTime,
					m_wAtkKillPoint,
					m_wDefKillPoint,
					strAtk1, strAtk2, strDef1, strDef2,
					wKA1, wKA2, wKD1, wKD2);
		}
	}

	return TRUE;
}

void CTMap::GetUnitPlayer(LPMAPPLAYER pPLAYERS, FLOAT fPosX, FLOAT fPosZ)
{
	BYTE bUnitX = BYTE(fPosX / UNIT_SIZE);
	BYTE bUnitZ = BYTE(fPosZ / UNIT_SIZE);

	WORD wCount = UNIT_SIZE/CELL_SIZE;
	for(WORD i=0; i<wCount; i++)
	{
		WORD wCellZ = WORD(bUnitZ) * wCount + i;
		for(WORD j=0; j<wCount; j++)
		{
			WORD wCellX = WORD(bUnitX) * wCount + j;
			MAPTCELL::iterator finder = m_mapTCELL.find(MAKELONG(wCellX, wCellZ));
			if( finder != m_mapTCELL.end() )
			{
				CTCell *pCell = (*finder).second;
				MAPPLAYER::iterator it;

				for( it = pCell->m_mapPLAYER.begin(); it != pCell->m_mapPLAYER.end(); it++)
				{
					MAPPLAYER::iterator itChar = pPLAYERS->find((*it).second->m_dwID);
					if(itChar == pPLAYERS->end())
						pPLAYERS->insert(MAPPLAYER::value_type((*it).second->m_dwID, (*it).second));
				}
			}
		}
	}
}

void CTMap::GetMapPlayers(LPVPLAYER vPlayers)
{
	MAPTCELL::iterator itC;
	MAPPLAYER::iterator itP;
	for(itC = m_mapTCELL.begin(); itC != m_mapTCELL.end(); itC++)
	{
		for(itP=(*itC).second->m_mapPLAYER.begin(); itP != (*itC).second->m_mapPLAYER.end(); itP++)
			vPlayers->push_back((*itP).second);
	}
}

WORD CTMap::GetLocalBonus(BYTE bDefCountry, WORD wDPoint, WORD wCPoint)
{
	WORD wDefPoint = 0;
	WORD wAtkPoint = 0;

	if(bDefCountry == TCONTRY_D)
	{
		wDefPoint = wDPoint;
		wAtkPoint = wCPoint;
	}
	else
	{
		wDefPoint = wCPoint;
		wAtkPoint = wDPoint;
	}

	if(wDefPoint+wAtkPoint == 0)
		wDefPoint = wAtkPoint = TLOCALBONUS_POINT;

	return WORD(DWORD(wDefPoint) * TLOCALBONUS_POINT / (wDefPoint+wAtkPoint));
}

void CTMap::StartWar(WORD wCastle,
					 BYTE bDefCountry,
                     DWORD dwDefGuild,
					 WORD wDefPoint,
                     DWORD dwAtkGuild,
					 WORD wAtkPoint,
					 DWORD dwEndTick)
{
	WORD wBonus = GetLocalBonus(bDefCountry, wDefPoint, wAtkPoint);
	m_dwDefPower = DEFAULT_WARPOWER + wBonus;
	m_dwAtkPower = DEFAULT_WARPOWER + TLOCALBONUS_POINT - wBonus;
	m_wDefKillPoint = 0;
	m_wAtkKillPoint = 0;
	m_dwBlueHideTick = 0;
	m_dwRedHideTick = 0;

	m_dwEndTick = dwEndTick;

	BYTE bInitBall = 2;
	m_bDefBallMax = m_bAtkBallMax = bInitBall;
	for(BYTE i=0; i<bInitBall; i++)
		InitGodBall(CAMP_DEFEND, dwDefGuild, wCastle);

	for(BYTE i=0; i<bInitBall; i++)
		InitGodBall(CAMP_ATTACK, dwAtkGuild, wCastle);
}

void CTMap::EndWar()
{
	m_bDefBallMax = 0;
	m_bAtkBallMax = 0;
	m_dwDefPower = 0;
	m_dwAtkPower = 0;
	m_wDefKillPoint = 0;
	m_wAtkKillPoint = 0;
	m_dwBlueHideTick = 0;
	m_dwRedHideTick = 0;

	m_dwEndTick = 0;
	
	ResetTempGodBall();

	MAPTGODTOWER::iterator it;
	for(it=m_mapTGODTOWER.begin(); it!=m_mapTGODTOWER.end(); it++)
		if((*it).second->m_pGodBall)
		{
			delete (*it).second->m_pGodBall;
			(*it).second->m_pGodBall = NULL;
		}

	MAPTGODBALL::iterator itGB;
	for(itGB=m_mapTGODBALL.begin(); itGB!=m_mapTGODBALL.end(); itGB++)
	{
		LeaveMAP((*itGB).second, FALSE);
		delete (*itGB).second;
	}
	m_mapTGODBALL.clear();

	MAPTCELL::iterator itCell;
	for(itCell=m_mapTCELL.begin(); itCell!=m_mapTCELL.end(); itCell++)
	{
		CTCell * pCell = (*itCell).second;
		MAPPLAYER::iterator itChar;
		for(itChar=pCell->m_mapPLAYER.begin(); itChar != pCell->m_mapPLAYER.end(); itChar++)
		{
			if((*itChar).second->m_bMain && (*itChar).second->m_wGodBall)
				(*itChar).second->RemoveGodBall();
		}
	}
}

void CTMap::GetGodBallOwner(CString &strAtk1,
                            CString &strAtk2,
                            CString &strDef1,
                            CString &strDef2,
							WORD &wKeepAtk1,
							WORD &wKeepAtk2,
							WORD &wKeepDef1,
							WORD &wKeepDef2)
{
	strAtk1 = strAtk2 = strDef1 = strDef2 = NAME_NULL;
	wKeepAtk1 = wKeepAtk2 = wKeepDef1 = wKeepDef2 = 0;

	MAPTGODBALL::iterator it;
	for(it=m_mapTGODBALL.begin(); it!=m_mapTGODBALL.end(); it++)
	{
		LPTGODBALL pBall = (*it).second;
		if(pBall->m_strOwner != NAME_NULL)
		{
			if(pBall->m_bCamp == CAMP_ATTACK)
			{
				if(strAtk1 == NAME_NULL)
				{
					strAtk1 = pBall->m_strOwner;
					wKeepAtk1 = WORD(_AtlModule.m_timeCurrent - pBall->m_dlTime);
				}
				else
				{
					strAtk2 = pBall->m_strOwner;
					wKeepAtk2 = WORD(_AtlModule.m_timeCurrent - pBall->m_dlTime);
				}
			}
			else if(pBall->m_bCamp == CAMP_DEFEND)
			{
				if(strDef1 == NAME_NULL)
				{
					strDef1 = pBall->m_strOwner;
					wKeepDef1 = WORD(_AtlModule.m_timeCurrent - pBall->m_dlTime);
				}
				else
				{
					strDef2 = pBall->m_strOwner;
					wKeepDef2 = WORD(_AtlModule.m_timeCurrent - pBall->m_dlTime);
				}
			}
		}
	}
}

void CTMap::SetCastle(LPTLOCAL pCastle)
{
	m_pCastle = pCastle;
}

void CTMap::ClearMonster()
{
	MAPTMONSPAWN::iterator it;
	for(it = m_mapTMONSPAWN.begin(); it!=m_mapTMONSPAWN.end(); it++)
	{
		CTMonSpawn * pSPAWN = (*it).second;
		for(DWORD i=0; i<pSPAWN->m_vTMON.size(); i++)
			if( pSPAWN->m_vTMON[i]->m_bStatus != OS_DEAD  &&
				pSPAWN->m_vTMON[i]->m_bStatus != OS_DISAPPEAR)
			{
				pSPAWN->m_vTMON[i]->OnDie(0,OT_NONE,0); 
			}
	}
}

void CTMap::AddTNMTPlayer(LPTOURNAMENTPLAYER pPlayer)
{
	m_mapTNMTPlayer.erase(pPlayer->m_dwCharID);
	m_mapTNMTPlayer.insert(MAPTOURNAMENTPLAYER::value_type(pPlayer->m_dwCharID, pPlayer));
	if(m_mapTNMTPlayer.size() == 1)
		m_bBlueTeam = pPlayer->m_bSlot;

	MAPTOURNAMENTPLAYER::iterator it;
	for(it=m_mapTNMTPlayer.begin(); it!=m_mapTNMTPlayer.end(); it++)
	{
		if((*it).second->m_bSlot == pPlayer->m_bSlot)
			continue;

		if((*it).second->m_bSlot < pPlayer->m_bSlot)
		{
			m_bBlueTeam = (*it).second->m_bSlot;
			m_bRedTeam = pPlayer->m_bSlot;
			break;
		}
		else
		{
			m_bBlueTeam = pPlayer->m_bSlot;
			m_bRedTeam = (*it).second->m_bSlot;
			break;
		}
	}
}

BYTE CTMap::EndTournament(BYTE & bRet, DWORD & dwWinner, DWORD & dwLoser, BYTE bStep)
{
	dwWinner = dwLoser = 0;
	bRet = TRUE;

	BYTE bWinSlot = TOURNAMENT_SLOT;

	if(m_mapTNMTPlayer.empty())
		return FALSE;

	if(m_bBlueTeam == TOURNAMENT_SLOT &&
		m_bRedTeam == TOURNAMENT_SLOT)
		return FALSE;

	if(m_bBlueTeam == TOURNAMENT_SLOT)
		bWinSlot = m_bRedTeam;
	else if(m_bRedTeam == TOURNAMENT_SLOT)
		bWinSlot = m_bBlueTeam;
	else
	{
		BYTE bBlue = 0;
		BYTE bRed = 0;

		MAPTOURNAMENTPLAYER::iterator it;
		for(it=m_mapTNMTPlayer.begin(); it!=m_mapTNMTPlayer.end(); it++)
		{
			if((*it).second->m_wMapID == m_wMapID)
			{
				if(m_bBlueTeam == (*it).second->m_bSlot)
					bBlue++;
				else
					bRed++;
			}
		}

		if(!bBlue && !bRed)
		{
			bRet = FALSE;
			bWinSlot = m_bBlueTeam;
		}
		else if(!bBlue)
			bWinSlot = m_bRedTeam;
		else if(!bRed)
			bWinSlot = m_bBlueTeam;
		else if(m_wAtkKillPoint < m_wDefKillPoint)
			bWinSlot = m_bBlueTeam;
		else if(m_wAtkKillPoint > m_wDefKillPoint)
			bWinSlot = m_bRedTeam;
		else if(m_dwRedHideTick < m_dwBlueHideTick)
			bWinSlot = m_bRedTeam;
		else
			bWinSlot = m_bBlueTeam;
	}

	BYTE bID;
	if(bStep == TNMTSTEP_FINAL)	bID = MATCH_FINAL;
	else if(bStep == TNMTSTEP_SFINAL) bID = MATCH_SFINAL;
	else bID = MATCH_QFINAL;

	MAPTOURNAMENTPLAYER::iterator it;
	for(it=m_mapTNMTPlayer.begin(); it!=m_mapTNMTPlayer.end(); it++)
	{
		if(bStep != TNMTSTEP_READY && (*it).second->m_bResult[bID] != TNMTWIN_NONE)
			return FALSE;

		if((*it).second->m_bSlot == bWinSlot)
			dwWinner = (*it).second->m_dwChiefID;
		else
			dwLoser = (*it).second->m_dwChiefID;

		if(dwWinner && dwLoser)
			break;
	}

	ResetTournament();

	return TRUE;
}

void CTMap::ResetTournament()
{
	m_wDefKillPoint = 0;
	m_wAtkKillPoint = 0;
	m_dwBlueHideTick = 0;
	m_dwRedHideTick = 0;

	m_dwEndTick = 0;
	m_bBlueTeam = TOURNAMENT_SLOT;
	m_bRedTeam = TOURNAMENT_SLOT;

	m_mapTNMTPlayer.clear();
}

BYTE CTMap::CanDoTournament(BYTE bStep)
{
	if(m_bBlueTeam == TOURNAMENT_SLOT ||
		m_bRedTeam == TOURNAMENT_SLOT ||
		m_mapTNMTPlayer.empty())
		return FALSE;

	BYTE bID;

	if(bStep == TNMTSTEP_FINAL)	bID = MATCH_FINAL;
	else if(bStep == TNMTSTEP_SFINAL) bID = MATCH_SFINAL;
	else bID = MATCH_QFINAL;

	BYTE bBlue = 0;
	BYTE bRed = 0;

	MAPTOURNAMENTPLAYER::iterator it;
	for(it=m_mapTNMTPlayer.begin(); it!=m_mapTNMTPlayer.end(); it++)
	{
		if((*it).second->m_wMapID == m_wMapID)
		{
			if(m_bBlueTeam == (*it).second->m_bSlot)
				bBlue++;
			else
				bRed++;
		}

		if(bStep != TNMTSTEP_READY && (*it).second->m_bResult[bID] != TNMTWIN_NONE)
			return FALSE;
	}

	if(!bBlue || !bRed)
		return FALSE;

	return TRUE;
}

void CTMap::SelectWinner()
{
	while(m_wDefKillPoint == m_wAtkKillPoint)
	{
		m_wDefKillPoint = rand() % 6 + 1;
		m_wAtkKillPoint = rand() % 6 + 1;
	}
}

void CTMap::SetHideTick(DWORD dwCharID, DWORD dwTick)
{
	MAPTOURNAMENTPLAYER::iterator it = m_mapTNMTPlayer.find(dwCharID);
	if(it!=m_mapTNMTPlayer.end())
	{
		if(m_bBlueTeam == (*it).second->m_bSlot)
			m_dwBlueHideTick += dwTick;
		else
			m_dwRedHideTick += dwTick;
	}
}
