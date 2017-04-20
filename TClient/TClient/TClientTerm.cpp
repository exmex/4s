#include "StdAfx.h"


CTClientTerm::CTClientTerm()
{
	m_pTTERM = NULL;

	m_bStatus = 0;
	m_bCount = 0;
}

CTClientTerm::~CTClientTerm()
{
}

BYTE CTClientTerm::GetResult()
{
	return m_bCount < m_pTTERM->m_bCount ? TTERMRESULT_INCOMPLETE : TTERMRESULT_COMPLETE;
}
