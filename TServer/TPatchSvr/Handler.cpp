#include "stdafx.h"
#include "TPatchSvr.h"
#include "TPatchSvrModule.h"

DWORD CTPatchSvrModule::OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, CTPatchSession * pSession )
{
	pSession->m_Recv.m_ptrOffset += sizeof(INT64);

	DWORD dwTick;
	pSession->m_Recv
		>> dwTick;

	pSession->SendCT_SERVICEMONITOR_REQ(dwTick, DWORD(m_mapTSESSION.size()), DWORD(m_mapTSESSION.size()),0);
	pSession->m_bSessionType = SESSION_SERVER;

	SMART_LOCKCS(&m_csPC);
	MAPTSESSION::iterator it;
	for(it=m_mapTSESSION.begin(); it!=m_mapTSESSION.end(); it++)
		if((*it).second->m_bSessionType == SESSION_CLIENT &&
			(*it).second->m_dwTick &&
			GetTickCount() - (*it).second->m_dwTick > 1000 * 60)
			CloseSession((*it).second);

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::OnCT_SERVICEDATACLEAR_ACK(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	return EC_NOERROR;
}

DWORD CTPatchSvrModule::OnCT_PATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	DWORD dwVersion;
	pSession->m_Recv
		>> dwVersion;

	VPATCHFILE vPatch;
	vPatch.clear();

	DEFINE_QUERY(pDB, CTBLVersion)
	query->m_dwVersion = dwVersion;
	if(query->Open())
	{
		while(query->Fetch())
		{
			PATCHFILE pPatch;
			pPatch.m_dwVersion = query->m_dwVersion;
			pPatch.m_strPath = query->m_szPath;
			pPatch.m_strName = query->m_szName;
			pPatch.m_dwSize = query->m_dwSize;
			vPatch.push_back(pPatch);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pSession->SendCT_PATCH_ACK(m_strFTP, &m_addrLOGIN, &vPatch);

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::OnCT_NEWPATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	DWORD dwVersion;
	pSession->m_Recv
		>> dwVersion;

	VPATCHFILE vPatch;
	vPatch.clear();

	DEFINE_QUERY(pDB, CTBLVersion)
	query->m_dwVersion = dwVersion;
	if(query->Open())
	{
		while(query->Fetch())
		{
			PATCHFILE pPatch;
			pPatch.m_dwVersion = query->m_dwVersion;
			pPatch.m_strPath = query->m_szPath;
			pPatch.m_strName = query->m_szName;
			pPatch.m_dwSize = query->m_dwSize;
			pPatch.m_dwBetaVer = query->m_dwBetaVer;
			vPatch.push_back(pPatch);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	DWORD dwMinBetaVer = 0;
	DEFINE_QUERY(pDB, CSPMinBetaVer)
	if(query->Call())
		dwMinBetaVer = query->m_dwMinVer;
	UNDEFINE_QUERY();	

	pSession->SendCT_NEWPATCH_ACK(m_strFTP, &m_addrLOGIN, &vPatch, dwMinBetaVer);

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::OnCT_PREPATCH_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	DWORD dwVersion;
	pSession->m_Recv
		>> dwVersion;

	VPATCHFILE vPatch;
	vPatch.clear();

	DEFINE_QUERY(pDB, CTBLPreVersion)
	query->m_dwBetaVer = dwVersion;
	if(query->Open())
	{
		while(query->Fetch())
		{
			PATCHFILE pPatch;
			pPatch.m_dwBetaVer	= query->m_dwBetaVer;
			pPatch.m_strPath	= query->m_szPath;
			pPatch.m_strName	= query->m_szName;
			pPatch.m_dwSize		= query->m_dwSize;

			vPatch.push_back(pPatch);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pSession->SendCT_PREPATCH_ACK(m_strPreFTP, &m_addrLOGIN, &vPatch);

	return EC_NOERROR;
}

DWORD CTPatchSvrModule::OnCT_PATCHSTART_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	return EC_SESSION_EXIT;
}

DWORD CTPatchSvrModule::OnCT_CTRLSVR_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	return EC_NOERROR;
}
DWORD CTPatchSvrModule::OnCT_PREPATCHCOMPLETE_REQ(CSqlDatabase * pDB, CTPatchSession * pSession)
{
	DWORD dwVersion;
	pSession->m_Recv
		>> dwVersion;

	DEFINE_QUERY(pDB, CSPPreComplete)
		query->m_dwBetaVer = dwVersion;
	if(!query->Call())
		return EC_INITSERVICE_DBOPENFAILED;
	UNDEFINE_QUERY();
	return EC_SESSION_EXIT;
}
