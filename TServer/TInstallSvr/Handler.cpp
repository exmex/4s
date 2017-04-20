#include "stdafx.h"
#include "TInstallSvr.h"
#include "TInstallSvrModule.h"


DWORD CTInstallSvrModule::OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, CTInstallSession * pSession )
{
	//pSession->m_Recv.m_ptrOffset += sizeof(INT64);

	DWORD dwTick;
	pSession->m_Recv
		>> dwTick;

	pSession->SendCT_SERVICEMONITOR_REQ(dwTick, DWORD(m_mapTSESSION.size()), DWORD(m_mapTSESSION.size()),0);
	pSession->m_bSessionType = SESSION_SERVER;

	//	오래된 클라이언트 세션 접속 해제
	SMART_LOCKCS(&m_csPC);
	MAPTSESSION::iterator it;
	for(it=m_mapTSESSION.begin(); it!=m_mapTSESSION.end(); it++)
		if((*it).second->m_bSessionType == SESSION_CLIENT && (*it).second->m_dwTick &&
			GetTickCount() - (*it).second->m_dwTick > 60000)
			CloseSession((*it).second);

	return EC_NOERROR;
}

DWORD CTInstallSvrModule::OnCT_INSTALL_REQ( CSqlDatabase *pDB, CTInstallSession * pSession)
{
	QINSTFILE QFile;
	QFile.clear();
	DWORD dwVersion, dwSegmentSize;

	DEFINE_QUERY(&m_db, CTBLInstallVer)
	if(query->Open())
	{
		if(query->Fetch())
		{
			dwVersion = query->m_dwVersion;
			dwSegmentSize = query->m_dwSegmentSize;
		}
		query->Close();
	}

	UNDEFINE_QUERY()

	DEFINE_QUERY(pDB, CTBLInstallFile)
	if(query->Open())
	{
		while(query->Fetch())
		{
			InstFile * pfile = new InstFile;
			pfile->m_dwVersion	= query->m_dwVersion;
			pfile->m_strName	= query->m_szName;
			pfile->m_dwSize		= query->m_dwSize;
			pfile->m_strCheckSum = query->m_szCheckSum;

			QFile.push_back(pfile);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pSession->SendCT_INSTALL_ACK(dwVersion, dwSegmentSize, m_strFTP, &QFile);

	return EC_SESSION_EXIT;
}

DWORD CTInstallSvrModule::OnCT_INSTALLEND_REQ( CSqlDatabase *pDB, CTInstallSession * pSession)
{
	DeleteIP(pDB, pSession->m_addr.sin_addr.s_addr);
	
	return EC_SESSION_EXIT;
}

DWORD CTInstallSvrModule::OnCT_FILEADDR_REQ( CSqlDatabase *pDB, CTInstallSession * pSession)
{
	CString strCheckSum;
	deque<DWORD> qIPAddr;

	DWORD dwFileID;
	WORD wSegmentID, wContry = 0;	

	//	IPAddr 얻기
	DEFINE_QUERY(pDB, CSPTGetContry)
	query->m_dwIPaddr = pSession->m_addr.sin_addr.s_addr;
	if(query->Call())
		wContry = query->m_wContry;
	UNDEFINE_QUERY()

	pSession->m_Recv
		>> dwFileID
		>> wSegmentID;

	//	세그먼트 체크섬 얻기
	DEFINE_QUERY(pDB, CTBLSegmentCheck)
	query->m_dwFileID = dwFileID;
	query->m_wSegment = wSegmentID;
	if(query->Open())
	{
		if(query->Fetch())
			strCheckSum = query->m_szCheckSum;
		else
			return EC_SESSION_EXIT;
		query->Close();
	}
	UNDEFINE_QUERY()

	//	IPAddr 얻기
	DEFINE_QUERY(pDB, CTBLInstallIPaddr)
	query->	m_dwFileID	= dwFileID;
	query->	m_wSegment	= wSegmentID;
	query->	m_wContry	= wContry;
	if(query->Open())
	{
		while(query->Fetch())
		{
			if(pSession->m_addr.sin_addr.s_addr != query->m_dwIPaddr)
				qIPAddr.push_back((DWORD)query->m_dwIPaddr);
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	//	현재 IP 리스트에 추가 TRegistSegment(dwIPaddr, dwFileID, wSegment)
	DEFINE_QUERY(pDB, CSPRegistSegment)
	query->m_dwFileID = dwFileID;
	query->m_wSegment = wSegmentID;
	query->m_dwIPaddr = pSession->m_addr.sin_addr.s_addr;
	query->m_wContry = wContry;
	query->Call();
	UNDEFINE_QUERY()

	pSession->SendCT_FILEADDR_ACK(dwFileID, wSegmentID, strCheckSum, &qIPAddr);

	return EC_SESSION_EXIT;
}