/*
 *	TUser.cpp
 */
#include "StdAfx.h"


CTUser::CTUser()
{
	m_strUserID.Empty();
	m_strPasswd.Empty();

	m_bLogout	= FALSE;
	m_dwID		= 0;
	m_bCreateCnt = 0;
	m_bAgreement = TRUE;
	m_bGroupID = 0;

	m_dwAcceptTick = 0;
	m_dwSendTick = 0;
	m_dlCheckKey = 0;
	
	while(!m_qCheckPoint.empty())
		m_qCheckPoint.pop();

#ifdef DEF_UDPLOG
    m_vCHAR.clear();
#endif
}

CTUser::~CTUser()
{
#ifdef DEF_UDPLOG

	//	Delete the Char List Memory
	for(DWORD i=0; i < m_vCHAR.size(); i++)
		delete m_vCHAR[i];

	m_vCHAR.clear();

#endif
}

BOOL CTUser::Say( CPacket *pPacket)
{
	m_dwSendTick = GetTickCount();
	return CSession::Say(pPacket);
}

/*
 *
 */
#ifdef DEF_UDPLOG


LPTCHARACTER CTUser::FindCharacterBase(DWORD pCharID)
{
	for(DWORD i=0; i < m_vCHAR.size(); i++)
	{
		if( m_vCHAR[i]->m_dwCharID  == pCharID )
			return m_vCHAR[i];
	}

	return NULL;
}


void CTUser::InsertCharacter(LPTCHARACTER pChar)
{
	m_vCHAR.push_back(pChar);

}

void CTUser::DeleteCharacter(DWORD pCharID)
{
	LPTCHARACTER pChar = FindCharacterBase(pCharID);

	if(pChar != NULL)
	{
		vector<LPTCHARACTER>::iterator where = find(m_vCHAR.begin(), m_vCHAR.end(), pChar);
        m_vCHAR.erase( where);	
	}
	
}
#endif	//	DEF_UDPLOG