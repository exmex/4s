#include "StdAfx.h"

CTClientQuest::CTClientQuest()
{
	m_pTQUEST = NULL;
	m_vTTERM.clear();

	m_bTimmer = FALSE;
	m_dwTick = 0;
	m_bCheckShowRight = TRUE;
}

CTClientQuest::~CTClientQuest()
{
	while(!m_vTTERM.empty())
	{
		delete m_vTTERM.back();
		m_vTTERM.pop_back();
	}
}

LPTTERM CTClientQuest::FindTTerm( LPTQUEST pTQUEST,
								  DWORD dwTermID,
								  BYTE bTermType)
{
	for( BYTE i=0; i<INT(pTQUEST->m_vTTERM.size()); i++)
		if( pTQUEST->m_vTTERM[i]->m_bType == bTermType &&
			pTQUEST->m_vTTERM[i]->m_dwID == dwTermID )
			return pTQUEST->m_vTTERM[i];

	return NULL;
}

CTClientTerm *CTClientQuest::FindTTerm( LPTTERM pTTERM)
{
	for( BYTE i=0; i<INT(m_vTTERM.size()); i++)
		if( m_vTTERM[i]->m_pTTERM == pTTERM )
			return m_vTTERM[i];

	return NULL;
}

CTClientTerm *CTClientQuest::FindTTerm( DWORD dwTermID,
									    BYTE bTermType)
{
	for( BYTE i=0; i<INT(m_vTTERM.size()); i++)
		if( m_vTTERM[i]->m_pTTERM->m_bType == bTermType &&
			m_vTTERM[i]->m_pTTERM->m_dwID == dwTermID )
			return m_vTTERM[i];

	return NULL;
}

BYTE CTClientQuest::GetResult()
{
	BYTE bResult = TTERMRESULT_COMPLETE;

	if( m_bTimmer && !m_dwTick )
		return TTERMRESULT_TIMEOUT;

	for( BYTE i=0; i<INT(m_pTQUEST->m_vTTERM.size()); i++)
		if( m_pTQUEST->m_vTTERM[i]->m_bType != QTT_TIMER )
		{
			CTClientTerm *pTTERM = FindTTerm(m_pTQUEST->m_vTTERM[i]);
			BYTE bLocal = pTTERM ? pTTERM->GetResult() : TTERMRESULT_INCOMPLETE;

			if( bResult < bLocal )
				bResult = bLocal;
		}

	return bResult;
}
