#include "StdAfx.h"


CTManager::CTManager()
{	
	m_bUpload = FALSE;
	m_bAuthority = 0;
	m_strID = _T("");
}

CTManager::~CTManager()
{
}

// 현승룡 권한을 검사하여 결과를 리턴
BYTE CTManager::CheckAuthority(BYTE m_bClass) 
{	
	if( m_bAuthority <= m_bClass)
		return TRUE; 
	else if( m_bAuthority <= MANAGER_SERVICE )
	{		
		SendCT_AUTHORITY_ACK();
	}

	return FALSE;
}

