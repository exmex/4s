#include "stdafx.h"

void CTManager::SendCT_OPLOGIN_ACK(BYTE bRet, BYTE bAuthority,DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_OPLOGIN_ACK)
		<< bRet
		<< bAuthority // Çö½Â·æ ¸Å´ÏÀú ±ÇÇÑ
		<< dwID;

	Say(pMsg);
}
// Çö½Â·æ CT_STLOGIN
void CTManager::SendCT_STLOGIN_ACK(BYTE bRet, BYTE bAuthority)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_STLOGIN_ACK)
		<< bRet
		<< bAuthority; // Çö½Â·æ ¸Å´ÏÀú ±ÇÇÑ

	Say(pMsg);
}
void CTManager::SendCT_SVRTYPELIST_ACK(LPMAPTSVRTYPE pType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SVRTYPELIST_ACK)
		<< DWORD(pType->size());

	MAPTSVRTYPE::iterator it;
	for(it=pType->begin(); it!=pType->end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_bType
			<< (*it).second->m_strName;
	}

	Say(pMsg);
}
void CTManager::SendCT_MACHINELIST_ACK(LPMAPTMACHINE pMachines)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_MACHINELIST_ACK)
		<< DWORD(pMachines->size());

	MAPTMACHINE::iterator it;
	for(it=pMachines->begin(); it!=pMachines->end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_bMachineID
			<< (*it).second->m_strName;
	}

	Say(pMsg);
}
void CTManager::SendCT_GROUPLIST_ACK(LPMAPTGROUP pGroups)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_GROUPLIST_ACK)
		<< DWORD(pGroups->size());

	MAPTGROUP::iterator it;
	for(it=pGroups->begin(); it!=pGroups->end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_bGroupID
			<< (*it).second->m_strName;
	}

	Say(pMsg);
}
void CTManager::SendCT_SERVICESTAT_ACK(LPMAPTSVRTEMP pServices)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICESTAT_ACK)
		<< DWORD(pServices->size());

	MAPTSVRTEMP::iterator it;
	for(it=pServices->begin(); it!=pServices->end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_pGroup->m_bGroupID
			<< (*it).second->m_pSvrType->m_bType
			<< (*it).second->m_bServerID
			<< (*it).second->m_strName
			<< (*it).second->m_pMachine->m_bMachineID
			<< (*it).second->m_dwStatus;
	}

	Say(pMsg);
}
void CTManager::SendCT_SERVICECONTROL_ACK(BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICECONTROL_ACK)
		<< bRet;

	Say(pMsg);
}
void CTManager::SendCT_SERVICECHANGE_ACK(DWORD dwID, DWORD dwStatus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICECHANGE_ACK)
		<< dwID
		<< dwStatus;

	Say(pMsg);
}
void CTManager::SendCT_SERVICEDATA_ACK(LPTSVRTEMP pSvr,
									  DWORD dwSession,
									  DWORD dwCurUser,
									  DWORD dwPing,
									  DWORD dwActiveUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEDATA_ACK)
		<< pSvr->m_dwID
		<< dwSession
		<< dwCurUser
		<< pSvr->m_dwMaxUser
		<< dwPing
		<< pSvr->m_nPickTime
		<< pSvr->m_dwStopCount
		<< pSvr->m_nLatestStop
		<< dwActiveUser;

	Say(pMsg);
}
void CTManager::SendCT_SERVICEUPLOADSTART_ACK(BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOADSTART_ACK)
		<< bRet;

	Say(pMsg);
}
void CTManager::SendCT_SERVICEUPLOAD_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOAD_ACK);

	Say(pMsg);
}
void CTManager::SendCT_SERVICEUPLOADEND_ACK(BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOADEND_ACK)
		<< bRet;

	Say(pMsg);
}
////////////////////////////////////////////////////////
// Çö½Â·æ CT_AUTHORITY_ACK
void CTManager::SendCT_AUTHORITY_ACK()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_AUTHORITY_ACK);	

	Say(pMsg);
}
// Çö½Â·æ CT_ACCOUNTINPUT_ACK
void CTManager::SendCT_ACCOUNTINPUT_ACK(BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_ACCOUNTINPUT_ACK)
		<< bRet;

	Say(pMsg);
}
// Çö½Â·æ CT_PLATFORM_ACK
void CTManager::SendCT_PLATFORM_ACK(BYTE bMachineID, DWORD dwCPU, DWORD dwMEM, float fNET)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PLATFORM_ACK)
		<< bMachineID
		<< dwCPU
		<< dwMEM
		<< fNET;

	Say(pMsg);
}
// Çö½Â·æ CT_MONSPAWNFIND_ACK
void CTManager::SendCT_MONSPAWNFIND_ACK(CPacket *pMsg)
{
	Say(pMsg);
}
void CTManager::SendCT_USERPROTECTED_ACK(BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPROTECTED_ACK)
		<< bRet;

	Say(pMsg);
}
////////////////////////////////////////////////////////


void CTManager::SendCT_SERVICEAUTOSTART_ACK(BYTE _bAutoStart)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEAUTOSTART_ACK)
		<< _bAutoStart;

	Say(pMsg);
}

void CTManager::SendCT_CHATBAN_ACK(BYTE _bRet)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBAN_ACK)
		<< _bRet;

	Say(pMsg);
}

void CTManager::SendCT_ITEMFIND_ACK(CPacket* pPacket)
{
	//CPacket* pMsg = new CPacket();
	//pMsg->Copy(pPacket);
	//Say(pMsg);

	Say(pPacket);
}

void CTManager::SendCT_ITEMSTATE_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_ITEMSTATE_ACK);
		
	Say(pMsg);
}

void CTManager::SendCT_CHATBANLIST_ACK(LPMAPBANINFO mapBanInfo)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBANLIST_ACK)
		<< WORD(mapBanInfo->size());

	MAPBANINFO::iterator it;
	for(it = mapBanInfo->begin(); it != mapBanInfo->end(); it++)
	{
		(*pMsg)			
			<< (*it).second->m_dwID
			<< (*it).second->m_strBanName
			<< (*it).second->m_Time
			<< (*it).second->m_wMin
			<< (*it).second->m_strReason
			<< (*it).second->m_strOPName;
	}
	
	Say(pMsg);
}

void CTManager::SendCT_CASTLEINFO_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_CASTLEINFO_ACK);

	Say(pMsg);
}

void CTManager::SendCT_CASTLEGUILDCHG_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_CASTLEGUILDCHG_ACK);		

	Say(pMsg);
}

void CTManager::SendCT_EVENTCHANGE_ACK(BYTE bRet,BYTE bType,LPEVENTINFO pEvent)
{
	CPacket* pMsg = new CPacket();	
	pMsg->SetID(CT_EVENTCHANGE_ACK)
		<< bRet
		<< bType;

	pEvent->WrapPacketIn(pMsg);

	Say(pMsg);
}

void CTManager::SendCT_EVENTLIST_ACK(LPMAPEVENTINFO pMapEvent)
{
	CPacket* pMsg = new CPacket();	
	pMsg->SetID(CT_EVENTLIST_ACK)
		<< (WORD)pMapEvent->size();

	MAPEVENTINFO::iterator it;
	for(it = pMapEvent->begin(); it != pMapEvent->end(); it++)	
		(*it).second.WrapPacketIn(pMsg);	

	Say(pMsg);
}

void CTManager::SendCT_CASHITEMLIST_ACK(LPVTCASHITEM pvCashItem)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASHITEMLIST_ACK)
		<< (WORD)pvCashItem->size();

	for(WORD i = 0; i < (WORD)pvCashItem->size(); i++)
	{
		(*pMsg)
			<< pvCashItem->at(i).m_wID
			<< pvCashItem->at(i).m_strName;
	}
	
	Say(pMsg);
}
 
void CTManager::SendCT_EVENTQUARTERLIST_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_EVENTQUARTERLIST_ACK);

	Say(pMsg);
}

void CTManager::SendCT_EVENTQUARTERUPDATE_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_EVENTQUARTERUPDATE_ACK);

	Say(pMsg);
}

void CTManager::SendCT_TOURNAMENTEVENT_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_TOURNAMENTEVENT_ACK);

	Say(pMsg);	
}
void CTManager::SendCT_RPSGAMEDATA_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_RPSGAMEDATA_ACK);

	Say(pMsg);	
}
void CTManager::SendCT_PREVERSIONTABLE_ACK(LPVPATCHFILE pList)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_PREVERSIONTABLE_ACK)
		<< (WORD)pList->size();

	for(WORD i = 0; i < (WORD)pList->size(); i++)
	{
		PATCHFILE pPat = pList->at(i);
		(*pMsg)
			<< pPat.m_dwBetaVer
			<< pPat.m_strPath
			<< pPat.m_strName
			<< pPat.m_dwSize;
	}
	
	Say(pMsg);
}

void CTManager::SendCT_CMGIFTLIST_ACK(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();	
	pMsg->CopyData(pPacket, sizeof(DWORD));
	pMsg->SetID(CT_CMGIFTLIST_ACK);
	Say(pMsg);
}

void CTManager::SendCT_CMGIFT_ACK(BYTE bRet)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFT_ACK)
		<< bRet;

	Say(pMsg);
}