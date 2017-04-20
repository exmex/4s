#include "StdAfx.h"


CTMonsterAI::CTMonsterAI()
{
	for( int i=0; i<AT_COUNT; i++)
		m_mapVCOMMAND[i].clear();

	m_bAIType = 0;
}

CTMonsterAI::~CTMonsterAI()
{
	for( int i=0; i<AT_COUNT; i++)
	{
		MAPVTAICOMMAND::iterator it;

		for( it = m_mapVCOMMAND[i].begin(); it != m_mapVCOMMAND[i].end(); it++)
		{
			while(!(*it).second->empty())
			{
				delete (*it).second->back();
				(*it).second->pop_back();
			}

			delete (*it).second;
		}

		m_mapVCOMMAND[i].clear();
	}
}
