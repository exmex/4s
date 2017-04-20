#pragma once


class CTMonSpawn
{
public:
	LPTMONSPAWN m_pSPAWN;
	VTMONSTER m_vTMON;

	BYTE m_bStatus;
	DWORD m_dwLeader;
	BYTE m_bGroupOrder; //그룹내에서의 순번.
public:
	CTMonSpawn();
	virtual ~CTMonSpawn();
};
