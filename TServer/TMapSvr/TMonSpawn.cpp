#include "StdAfx.h"


CTMonSpawn::CTMonSpawn()
{
	m_pSPAWN = NULL;
	m_vTMON.clear();

	m_bStatus = MONSPAWN_READY;
	m_dwLeader = 0;
	m_bGroupOrder = 0;
}

CTMonSpawn::~CTMonSpawn()
{
	while(!m_vTMON.empty())
	{
		delete m_vTMON.back();
		m_vTMON.pop_back();
	}

	m_vTMON.clear();

	m_pSPAWN = NULL;
}
