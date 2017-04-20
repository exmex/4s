#include "stdafx.h"
#include "TControlSvr.h"
#include "TControlSvrModule.h"

DWORD CTControlSvrModule::OnSM_DELSESSION_REQ(LPPACKETBUF pBUF)
{
	PostQueuedCompletionStatus(
		m_hIocpControl, 0,
		COMP_CLOSE,
		(LPOVERLAPPED) pBUF->m_pSESSION);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_OPLOGIN_REQ( LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	
	CTManager* pAlreadyM;
	CString strID;
	CString strPasswd;
	
	pBUF->m_packet
		>> strID
		>> strPasswd;


	pAlreadyM =  FindManager(strID);

	DEFINE_QUERY(&m_db, CSPOPLogin)
	lstrcpy(query->m_szID, strID);
	lstrcpy(query->m_szPW, strPasswd);
	if(!query->Call() || !query->m_nRET )	
		pManager->SendCT_OPLOGIN_ACK(1, 0);
	else if( !pAlreadyM )
	{
		m_dwManagerSeq++;
		pManager->m_bManager = TRUE;		
		pManager->m_bAuthority = query->m_nRET;
		pManager->m_dwID = m_dwManagerSeq;
		pManager->m_strID = strID;
		m_mapMANAGER.insert(MAPTMANAGER::value_type(DWORD_PTR(pManager), pManager));

		pManager->SendCT_OPLOGIN_ACK(0, pManager->m_bAuthority,pManager->m_dwID);
		pManager->SendCT_GROUPLIST_ACK(&m_mapTGroup);
		pManager->SendCT_MACHINELIST_ACK(&m_mapTMachine);
		pManager->SendCT_SVRTYPELIST_ACK(&m_mapTSvrType);
		pManager->SendCT_SERVICEAUTOSTART_ACK(m_bAutoStart);
	}
	else
	{
		CloseSession( (CTControlSession*)pAlreadyM);
        return EC_SESSION_INVALIDCHAR;
	}

	UNDEFINE_QUERY()

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_STLOGIN_REQ
DWORD CTControlSvrModule::OnCT_STLOGIN_REQ( LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	
	CString strID;
	CString strPasswd;

	pBUF->m_packet
		>> strID
		>> strPasswd;

	DEFINE_QUERY(&m_db, CSPOPLogin)
	lstrcpy(query->m_szID, strID);
	lstrcpy(query->m_szPW, strPasswd);
	if(!query->Call() || !query->m_nRET )
		pManager->SendCT_STLOGIN_ACK(1, 0);
	else
	{
		pManager->m_bManager = TRUE;		
		pManager->m_bAuthority = query->m_nRET;
		pManager->m_strID = strID;
		m_mapMANAGER.insert(MAPTMANAGER::value_type(DWORD_PTR(pManager), pManager));
		pManager->SendCT_STLOGIN_ACK(0, pManager->m_bAuthority);
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICESTAT_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	pManager->SendCT_SERVICESTAT_ACK(&m_mapTSVRTEMP);
	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICECONTROL_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_SERVICE))  
		return EC_NOERROR;	

	DWORD dwServiceID;
	BYTE bStart;
	pBUF->m_packet
		>> dwServiceID
		>> bStart;	

	MAPTSVRTEMP::iterator it = m_mapTSVRTEMP.find(dwServiceID);
	if(it == m_mapTSVRTEMP.end())
		return EC_NOERROR;

	BYTE bRet = FALSE;
	switch((*it).second->m_dwStatus)
	{
	case DCSVC_STAT_STOPPED:
		if(bStart && StartService((*it).second))
		{
			//(*it).second->m_bManagerControl = TRUE;
			bRet = TRUE;
			LogEvent( "start SvrControl: %s",(*it).second->m_strName);
		}		

		break;

	case DCSVC_STAT_RUNNING:
		if(!bStart && StopService((*it).second))
		{			
			(*it).second->m_bManagerControl = FALSE;
			
			if( (*it).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR ) // ø˘µÂº≠πˆ¿Ã∏È
			{
				MAPTSVRTEMP::iterator itM;
				for( itM = m_mapTSVRTEMP.begin(); itM != m_mapTSVRTEMP.end(); itM++)
				{
					if( (*itM).second->m_pGroup->m_bGroupID == (*it).second->m_pGroup->m_bGroupID ) 
					{
						(*itM).second->m_bManagerControl = FALSE;
						(*itM).second->m_dwAutoStartTick = GetTickCount();
						LogEvent( "Stop SvrControl: %s",(*itM).second->m_strName);
					}
				}
			}
			bRet = TRUE;
		}
		break;
	default:
		bRet = ACK_FAILED;
	}

	pManager->SendCT_SERVICECONTROL_ACK(bRet);

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_NEWCONNECT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwID;

	pBUF->m_packet
		>> dwID;

	LPTSVRTEMP pService = FindService(dwID);

	if(pService && !pService->m_pConn)
	{
		pService->m_pConn = new CTServer();
		pService->m_pConn->m_pSVRTEMP = pService;
		if( !pService->m_pConn->Connect(pService->m_pMachine->m_vPriAddr.front(), pService->m_wPort, m_strMyAddr) )
		{
			LogEvent("Connect Error %s, %d, %d",pService->m_pMachine->m_vPriAddr.front(), pService->m_wPort, GetLastError());
			delete pService->m_pConn;
			pService->m_pConn = NULL;			
			return EC_NOERROR;
		}

		pService->m_pConn->m_dwRecvTick = GetTickCount();
		
		HANDLE hIocpWork;
		hIocpWork = CreateIoCompletionPort(
			(HANDLE) pService->m_pConn->m_sock,
			m_hIocpWork,
			COMP_SESSION, 0);

		if(!hIocpWork)
		{
			LogEvent("Iocp Error %s, %d, %d",pService->m_pMachine->m_vPriAddr.front(), pService->m_wPort, GetLastError());
			delete pService->m_pConn;
			pService->m_pConn = NULL;
			CloseHandle(hIocpWork);			
			return EC_NOERROR;
		}

		if(!pService->m_pConn->WaitForMessage())
		{
			LogEvent("WaitForMessage Error %s, %d, %d",pService->m_pMachine->m_vPriAddr.front(), pService->m_wPort, GetLastError());
			delete pService->m_pConn;
			pService->m_pConn = NULL;
			CloseHandle(hIocpWork);			
			return EC_NOERROR;
		}

		pService->m_pConn->SendCT_CTRLSVR_REQ();		
		SendEventToNewConnect(pService);
		
		LogEvent("New Connect %d",pService->m_dwID);
	}
	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICECHANGE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwID;
	DWORD dwStatus;
	BYTE bSMSType;
	BYTE bSvrType;
	

	pBUF->m_packet
		>> dwID
		>> dwStatus	
		>> bSMSType
		>> bSvrType;

	MAPTMANAGER::iterator it;
	for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
		(*it).second->SendCT_SERVICECHANGE_ACK(dwID, dwStatus);

	if(bSMSType != 0 )
	{
		SendSvrStatusSMS(bSvrType,dwID,bSMSType);
	}
	

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_TIMER_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if((*itSvr).second->m_pConn)
		{
			DWORD dwCurrentTick = GetTickCount();
			(*itSvr).second->m_pConn->m_dwSendTick = dwCurrentTick;

			if((*itSvr).second->m_pConn->m_dwRecvTick &&
				(dwCurrentTick - (*itSvr).second->m_pConn->m_dwRecvTick) > 60000)
			{
				(*itSvr).second->m_pConn->m_dwRecvTick = 0;
				MAPTMANAGER::iterator it;	
				for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
					if( (*it).second->CheckAuthority(MANAGER_SERVICE) )
						(*it).second->SendCT_SERVICEDATA_ACK(
							(*itSvr).second,
							0,
							0,
							60000,
							0);
				LogEvent("TIMER_REQ 1MIN");
				SendSvrStatusSMS((*itSvr).second->m_pSvrType->m_bType,(*itSvr).second->m_dwID,3);
			}

            (*itSvr).second->m_pConn->SendCT_SERVICEMONITOR_ACK(dwCurrentTick);
		}
		else
		{
			MAPTMANAGER::iterator it;	
			for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
				if( (*it).second->CheckAuthority(MANAGER_SERVICE) )
					(*it).second->SendCT_SERVICEDATA_ACK(
						(*itSvr).second,
						0,
						0,
						0,
						0);
		}
	}

	CheckEvent();

	return EC_NOERROR;
}


DWORD CTControlSvrModule::OnCT_SERVICEMONITOR_REQ(LPPACKETBUF pBUF)
{
	CTServer * pServer = (CTServer *)pBUF->m_pSESSION;

	DWORD dwTick;
	DWORD dwSession;
	DWORD dwUser = 0;
	DWORD dwActiveUser = 0;

	pBUF->m_packet
		>> dwTick
		>> dwSession
		>> dwUser
		>> dwActiveUser;

	LPTSVRTEMP pSvr = pServer->m_pSVRTEMP;
	if(!pSvr)
		return EC_NOERROR;

	pServer->m_dwRecvTick = GetTickCount();
		
	if(pSvr->m_dwMaxUser <= dwUser)
	{
		pSvr->m_dwMaxUser = dwUser;
		CTime t(CTime::GetCurrentTime());
		pSvr->m_nPickTime = t.GetTime();
	}

	MAPTMANAGER::iterator it;	
	for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
		(*it).second->SendCT_SERVICEDATA_ACK(
			pServer->m_pSVRTEMP,
			dwSession,
			dwUser,
			pServer->m_dwRecvTick - dwTick,
			dwActiveUser);

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICEUPLOADSTART_REQ(LPPACKETBUF pBUF)
{
	CTManager *pManager = (CTManager *)pBUF->m_pSESSION;	
	if(!pManager->CheckAuthority(MANAGER_CONTROL))  
		return EC_NOERROR;

	BYTE bMachineID;
	CString strFileName;
	pBUF->m_packet
		>> bMachineID
		>> strFileName;		

	if(!m_strUploadFilePath.IsEmpty())
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(1);
		return EC_NOERROR;
	}

	MAPTMACHINE::iterator it = m_mapTMachine.find(bMachineID);
	if(it == m_mapTMachine.end())
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(2);
		return EC_NOERROR;
	}

	m_fileTemp = fopen(IDS_FILENAME_UPLOADTEMP, "wb");
	if(!m_fileTemp)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(3);
		return EC_NOERROR;
	}

	pManager->m_bUpload = TRUE;
	LPTMACHINE pMachine = (*it).second;
	m_strUploadFilePath.Format("\\\\%s\\c$\\%s\\",pMachine->m_strName, IDS_FOLDERNAME_INSTALLED);
	m_strUploadFileName = strFileName;

	FILE * fileOld = fopen(m_strUploadFilePath+strFileName, "rb");
	
	if(fileOld)
	{
		CTime time = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format("%d%d%d%d",time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
		FILE * fileNew = fopen(m_strUploadFilePath+strTime+strFileName, "wb");

		if(!fileNew)
		{
			if(fileOld)
				fclose(fileOld);

			pManager->SendCT_SERVICEUPLOADEND_ACK(4);
			pManager->m_bUpload = FALSE;
			UploadEnd();
			return EC_NOERROR;
		}

		//char buf[1024];
		BYTE buf[1024];
		while(!feof(fileOld))
		{
			//size_t dwRead = fread(buf, sizeof(char), 1024, fileOld);
			//fwrite(buf, sizeof(char), dwRead, fileNew);

			size_t dwRead = fread(buf, sizeof(BYTE), 1024, fileOld);
			fwrite(buf, sizeof(BYTE), dwRead, fileNew);
		}
		
		fclose(fileNew);
		fclose(fileOld);
	}

	pManager->SendCT_SERVICEUPLOADSTART_ACK(ACK_SUCCESS);

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICEUPLOAD_REQ(LPPACKETBUF pBUF)
{
	CTManager *pManager = (CTManager *)pBUF->m_pSESSION;

	if(!m_fileTemp)
		return EC_NOERROR;

	/*
	DWORD dwSize = pBUF->m_packet.GetSize()-PACKET_HEADER_SIZE;
	if(!dwSize)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}

	BYTE data[DEF_PACKET_SIZE];
	memcpy(data, pBUF->m_packet.GetBuffer()+PACKET_HEADER_SIZE, dwSize);

	size_t dwWrote = fwrite(data, sizeof(BYTE), dwSize, m_fileTemp);
	if(dwWrote != dwSize)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}
	*/

	WORD wSize;
	pBUF->m_packet
		>> wSize;

	if(!wSize)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}
	
	BYTE data[DEF_PACKET_SIZE]={0};
	for(WORD i=0; i<wSize; i++)
		pBUF->m_packet
			>> data[i];
	size_t wWrote = fwrite(data, sizeof(BYTE), wSize, m_fileTemp);
	if(wWrote != wSize)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}

	pManager->SendCT_SERVICEUPLOAD_ACK();

	return EC_NOERROR;
}
DWORD CTControlSvrModule::OnCT_SERVICEUPLOADEND_REQ(LPPACKETBUF pBUF)
{
	CTManager *pManager = (CTManager *)pBUF->m_pSESSION;
	BYTE bCancel;
	pBUF->m_packet
		>> bCancel;

	if(bCancel)
	{
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}

	/*
	DWORD dwSize = pBUF->m_packet.GetSize()-PACKET_HEADER_SIZE;
	if(dwSize)
	{
		BYTE data[DEF_PACKET_SIZE]={0};
		memcpy(data, pBUF->m_packet.GetBuffer()+PACKET_HEADER_SIZE, dwSize);
		size_t dwWrote = fwrite(data, sizeof(BYTE), dwSize, m_fileTemp);
		if(dwWrote != dwSize)
		{
			pManager->SendCT_SERVICEUPLOADEND_ACK(4);
			pManager->m_bUpload = FALSE;
			UploadEnd();
			return EC_NOERROR;
		}
	}
	*/
	WORD wSize;
	pBUF->m_packet
		>> wSize;

	if(wSize)
	{
		BYTE data[DEF_PACKET_SIZE]={0};
		for(WORD i=0; i<wSize; i++)
			pBUF->m_packet
				>> data[i];
		size_t wWrote = fwrite(data, sizeof(BYTE), wSize, m_fileTemp);
		if(wWrote != wSize)
		{
			pManager->SendCT_SERVICEUPLOADEND_ACK(4);
			pManager->m_bUpload = FALSE;
			UploadEnd();
			return EC_NOERROR;
		}
	}

	FILE * fileNew = fopen(m_strUploadFilePath+m_strUploadFileName, "wb");
	if(!fileNew)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}
	fclose(m_fileTemp);

	m_fileTemp = fopen(IDS_FILENAME_UPLOADTEMP, "rb");
	if(!m_fileTemp)
	{
		pManager->SendCT_SERVICEUPLOADEND_ACK(4);
		pManager->m_bUpload = FALSE;
		UploadEnd();
		return EC_NOERROR;
	}

	BYTE buf[1024]={0};
	while(!feof(m_fileTemp))
	{
		size_t dwRead = fread(buf, sizeof(BYTE), 1024, m_fileTemp);
		fwrite(buf, sizeof(BYTE), dwRead, fileNew);
	}

	pManager->SendCT_SERVICEUPLOADEND_ACK(0);
	fclose(fileNew);
	pManager->m_bUpload = FALSE;
	UploadEnd();

	return EC_NOERROR;
}
///////////////////////////////////////////////////////////////////////////////
// «ˆΩ¬∑Ê CT_UPDATEPATCH_REQ
DWORD CTControlSvrModule::OnCT_UPDATEPATCH_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_CONTROL))  
		return EC_NOERROR;	

	WORD		wCount;
	CString		szPath;
	CString		szName;
	DWORD		dwSize;

	pBUF->m_packet
		>> wCount;		

	DEFINE_QUERY(&m_db, CSPUpdatePatch)
	for(WORD i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> szPath
			>> szName
			>> dwSize;

		lstrcpy(query->m_szPath, szPath);
		lstrcpy(query->m_szName, szName);
		query->m_dwSize = dwSize;
		query->m_dwBetaVer = 0;

		query->Call();
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}
/////////////////////////////////////////////////////////////////////////////////

//	TPREVERSION ≈◊¿Ã∫Ìø° µÓ∑œµ» ∆ƒ¿œ ∏Ò∑œ ∞°¡Æø¿±‚
DWORD CTControlSvrModule::OnCT_PREVERSIONTABLE_REQ(LPPACKETBUF pBUF)
{	
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_CONTROL))  
		return EC_NOERROR;	
	
	VPATCHFILE vList;

	DEFINE_QUERY(&m_db,CTBLPreVersion)
	if(query->Open())
	{
		while(query->Fetch())
		{
			PATCHFILE stPatch;
			stPatch.m_dwBetaVer = query->m_dwBetaVer;
			stPatch.m_strPath = query->m_szPath;
			stPatch.m_strName = query->m_szName;
			stPatch.m_dwSize = query->m_dwSize;

			vList.push_back(stPatch);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pManager->SendCT_PREVERSIONTABLE_ACK(&vList);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_PREVERSIONUPDATE_REQ(LPPACKETBUF pBUF)//WORD wNewCount, LPPATCHFILE pNew, WORD wMoveCount, LPPATCHFILE pMove, WORD wDelCount, LPPATCHFILE pDel)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_CONTROL))  
		return EC_NOERROR;	

	WORD wCount;

	pBUF->m_packet	>> wCount;	

	// 1. TPREVERSION ø°º≠ TVERSION¿∏∑Œ ¿Ãµø
	DEFINE_QUERY(&m_db, CSPBetaToVer)
	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwBetaVer;
		
		query->Call();
	}
	UNDEFINE_QUERY()

	pBUF->m_packet	>> wCount;	

	//	2. TRPEVERSION ¡¶∞≈
	DEFINE_QUERY(&m_db, CSPDeletePreVersion)
	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> query->m_dwBetaVer;
		
		query->Call();
	}
	UNDEFINE_QUERY()

	pBUF->m_packet	>> wCount;	

	//	3. TPREVERSION Ω≈±‘µÓ∑œ
	DEFINE_QUERY(&m_db, CSPUpdatePrePatch)
	for(WORD i = 0; i < wCount; i++)
	{
		CString	strPath;
		CString	strName;

		pBUF->m_packet
			>> strPath
			>> strName
			>> query->m_dwSize;

		lstrcpy(query->m_szPath, strPath);
		lstrcpy(query->m_szName, strName);

		query->Call();
	}
	UNDEFINE_QUERY()

	//	∏Ò∑œ ∫∏≥ª±‚.
	VPATCHFILE vList;
	DEFINE_QUERY(&m_db,CTBLPreVersion)
	if(query->Open())
	{
		while(query->Fetch())
		{
			PATCHFILE stPatch;
			stPatch.m_dwBetaVer = query->m_dwBetaVer;
			stPatch.m_strPath = query->m_szPath;
			stPatch.m_strName = query->m_szName;
			stPatch.m_dwSize = query->m_dwSize;

			vList.push_back(stPatch);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pManager->SendCT_PREVERSIONTABLE_ACK(&vList);

	return EC_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////
// «ˆΩ¬∑Ê CT_ANNOUNCEMENT_REQ
DWORD CTControlSvrModule::OnCT_ANNOUNCEMENT_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL1))  
		return EC_NOERROR;	

	VWORD		vSvrGroup;
	DWORD		nID;
	CString		strAnnounce;	

	pBUF->m_packet
		>> nID
		>> strAnnounce;		

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{		
		if( (*itSvr).second->m_pSvrType->m_bType == SVRGRP_RLYSVR  )
		{
			int id = (*itSvr).second->m_pGroup->m_bGroupID;
			if( (*itSvr).second->m_pConn )
			{				
				if( !nID )
					(*itSvr).second->m_pConn->SendCT_ANNOUNCEMENT_ACK(strAnnounce);
				else if( nID == id )
				{
					(*itSvr).second->m_pConn->SendCT_ANNOUNCEMENT_ACK(strAnnounce);
					vSvrGroup.clear();
					break;
				}
			}
			else if( nID == 0 || nID == id )			
				vSvrGroup.push_back( (WORD)(*itSvr).second->m_pGroup->m_bGroupID );
		}
	}

	for(DWORD i = 0; i < vSvrGroup.size(); i++ )
	{
		MAPTSVRTEMP::iterator itMap;
		for(itMap=m_mapTSVRTEMP.begin(); itMap!=m_mapTSVRTEMP.end(); itMap++)
		{	
			LPTSVRTEMP pSvrTemp = (*itMap).second;
			if( (*itMap).second->m_pConn &&
				(*itMap).second->m_pSvrType->m_bType == SVRGRP_MAPSVR &&
				(*itMap).second->m_pGroup->m_bGroupID == vSvrGroup[i] )	
			{				
				(*itMap).second->m_pConn->SendCT_ANNOUNCEMENT_ACK(strAnnounce);
			}
		}
	}

	vSvrGroup.clear();

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_USERKICKOUT_REQ
DWORD CTControlSvrModule::OnCT_USERKICKOUT_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL2))  
		return EC_NOERROR;	

	CString		strUser;	

	pBUF->m_packet		
		>> strUser;		

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_MAPSVR) )
			(*itSvr).second->m_pConn->SendCT_USERKICKOUT_ACK(strUser);
	}

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_USERMOVE_REQ
DWORD CTControlSvrModule::OnCT_USERMOVE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL2))  
		return EC_NOERROR;
	
	BYTE		bWorld;
	WORD		wCount;
	BYTE		bChannel;
	WORD		wMapID;
	FLOAT		fPosX;
	FLOAT		fPosY;
	FLOAT		fPosZ;

	pBUF->m_packet
		>> bWorld
		>> bChannel
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wCount;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR) &&
			((*itSvr).second->m_pGroup->m_bGroupID == bWorld))
			break;				
	}
	if(itSvr == m_mapTSVRTEMP.end())
		return EC_NOERROR;

	for(WORD i = 0; i < wCount; i++)
	{
		CString strUser;
		pBUF->m_packet
			>> strUser;

		(*itSvr).second->m_pConn->SendCT_USERMOVE_ACK(strUser, bChannel, wMapID, fPosX, fPosY, fPosZ);
	}


	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_USERPOSITION_REQ
DWORD CTControlSvrModule::OnCT_USERPOSITION_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL2))  
		return EC_NOERROR;

	BYTE		bWorld;
	WORD		wCount;
	CString		strTargetName;

	pBUF->m_packet
		>> bWorld
		>> strTargetName
		>> wCount;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR) &&
			((*itSvr).second->m_pGroup->m_bGroupID == bWorld))
				break;			
	}
	if(itSvr == m_mapTSVRTEMP.end())
		return EC_NOERROR;

	for(WORD i = 0; i < wCount; i++)
	{
		CString		strMoveUser;
		pBUF->m_packet
			>> strMoveUser;

		(*itSvr).second->m_pConn->SendCT_USERPOSITION_ACK(strTargetName, strMoveUser);
	}

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_MONSPAWNFIND_REQ
DWORD CTControlSvrModule::OnCT_MONSPAWNFIND_REQ(LPPACKETBUF pBUF) 
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL1))  
		return EC_NOERROR;

	BYTE bGroupID;
	BYTE bChannel;
	WORD wMapID;
	WORD wSpawnID;

	pBUF->m_packet
		>> bGroupID
		>> bChannel
		>> wMapID
		>> wSpawnID;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_MAPSVR) )
		{
			if( bGroupID == (*itSvr).second->m_pGroup->m_bGroupID )
				(*itSvr).second->m_pConn->SendCT_MONSPAWNFIND_ACK((DWORD)DWORD_PTR(pManager), bChannel, wMapID, wSpawnID);
		}
	}

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_MONSPAWNFIND_ACK
DWORD CTControlSvrModule::OnCT_MONSPAWNFIND_ACK(LPPACKETBUF pBUF)
{
	DWORD dwManager;
	WORD wMapID;
	WORD wSpawnID;
	BYTE bMonCnt;

	DWORD dwMonID;
	DWORD dwHostID;
	BYTE bStat;
	float fPosX;
	float fPosY;
	float fPosZ;

	pBUF->m_packet
		>> dwManager
		>> wMapID
		>> wSpawnID
		>> bMonCnt;

	CPacket *pMSG = new CPacket();
	pMSG->SetID(CT_MONSPAWNFIND_ACK)	
		<< wMapID
		<< wSpawnID
		<< bMonCnt;

    for(BYTE bCnt=0; bCnt<bMonCnt; bCnt++)
	{
		pBUF->m_packet
			>> dwMonID
			>> dwHostID
			>> bStat
			>> fPosX
			>> fPosY
			>> fPosZ;

		(*pMSG)
			<< dwMonID
			<< dwHostID
			<< bStat
			<< fPosX
			<< fPosY
			<< fPosZ;
	}

	MAPTMANAGER::iterator it = m_mapMANAGER.find(dwManager);
	if(it!=m_mapMANAGER.end())
		(*it).second->SendCT_MONSPAWNFIND_ACK(pMSG);

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_MONACTION_REQ
DWORD CTControlSvrModule::OnCT_MONACTION_REQ(LPPACKETBUF pBUF)
{
	BYTE bGroup;
	BYTE bChannel;
	WORD wMapID;
	DWORD dwMonID;
	BYTE bAction;
	DWORD dwTriggerID;
	DWORD dwHostID;
	DWORD dwRHID;
	BYTE bRHType;
	WORD wSpawnID;

	pBUF->m_packet
		>> bGroup
		>> bChannel
		>> wMapID
		>> dwMonID
		>> bAction
		>> dwTriggerID
		>> dwHostID
		>> dwRHID
		>> bRHType
		>> wSpawnID;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pGroup->m_bGroupID == bGroup) &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_MAPSVR) )
			(*itSvr).second->m_pConn->SendCT_MONACTION_ACK(bChannel, wMapID, dwMonID, bAction, dwTriggerID, dwHostID, dwRHID, bRHType,wSpawnID);
	}

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_PLATFORM_REQ
DWORD CTControlSvrModule::OnCT_PLATFORM_REQ(LPPACKETBUF pBUF)
{
	BYTE bMachineID;
	DWORD dwCPU;
	DWORD dwMEM;
	float fNET;

	pBUF->m_packet
		>> bMachineID
		>> dwCPU
		>> dwMEM
		>> fNET;

	MAPTMANAGER::iterator it;
	for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)	
		if( (*it).second->CheckAuthority(MANAGER_SERVICE) )
			(*it).second->SendCT_PLATFORM_ACK(bMachineID, dwCPU, dwMEM, fNET);

	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_USERPROTECTED_REQ
DWORD CTControlSvrModule::OnCT_USERPROTECTED_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_USER))  
		return EC_NOERROR;

	CString strUserID;
	DWORD dwDuration;
	BYTE bRet = 0;

	pBUF->m_packet
		>> strUserID
		>> dwDuration;

	/*
	DEFINE_QUERY(&m_db, CSPUserProtectedAdd)
	lstrcpy(query->m_szUserID, strUserID);
	query->m_dwDuration = dwDuration;
	query->Call();
	bRet = query->m_nRET;
	UNDEFINE_QUERY()

	pManager->SendCT_USERPROTECTED_ACK(bRet);
	*/
	
	return EC_NOERROR;
}
// «ˆΩ¬∑Ê CT_CHARMSG_REQ
DWORD CTControlSvrModule::OnCT_CHARMSG_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL2))  
		return EC_NOERROR;	

	VWORD		vSvrGroup;
	CString strName;
	CString strMsg;

	pBUF->m_packet
		>> strName
		>> strMsg;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{		
		if( (*itSvr).second->m_pSvrType->m_bType == SVRGRP_RLYSVR  )
		{
			int id = (*itSvr).second->m_pGroup->m_bGroupID;
			if( (*itSvr).second->m_pConn )
			{				
				(*itSvr).second->m_pConn->SendCT_CHARMSG_ACK(strName,strMsg);				
			}
			else 		
				vSvrGroup.push_back( (WORD)(*itSvr).second->m_pGroup->m_bGroupID );
		}
	}

	for(DWORD i = 0; i < vSvrGroup.size(); i++ )
	{
		MAPTSVRTEMP::iterator itMap;
		for(itMap=m_mapTSVRTEMP.begin(); itMap!=m_mapTSVRTEMP.end(); itMap++)
		{	
			LPTSVRTEMP pSvrTemp = (*itMap).second;
			if( (*itMap).second->m_pConn &&
				(*itMap).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR &&
				(*itMap).second->m_pGroup->m_bGroupID == vSvrGroup[i] )	
			{				
				(*itMap).second->m_pConn->SendCT_CHARMSG_ACK(strName,strMsg);
			}
		}
	}

	vSvrGroup.clear();

	return EC_NOERROR;
}
///////////////////////////////////////////////////////////////////////////////

DWORD CTControlSvrModule::OnCT_RECONNECT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwID;

	pBUF->m_packet
		>> dwID;

	LPTSVRTEMP pService = FindService(dwID);

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(CT_NEWCONNECT_REQ)
		<< pService->m_dwID;

	SayToBATCH(pMSG);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_SERVICEAUTOSTART_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet;

	pBUF->m_packet
		>> bRet;

	m_bAutoStart = bRet;

	MAPTMANAGER::iterator it;
	for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
		(*it).second->SendCT_SERVICEAUTOSTART_ACK(m_bAutoStart);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CHATBAN_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL3))  
		return EC_NOERROR;

	BYTE bSend = FALSE;
	CString strName;
	CString strReason;
	WORD wMin;

	pBUF->m_packet
		>> strName
		>> wMin
		>> strReason;
	
	m_dwSendCount = 0;
	m_bChatBanSuccess = FALSE;

	m_dwChatBanSeq++;
	
	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR ||
			 (*itSvr).second->m_pSvrType->m_bType == SVRGRP_RLYSVR ) ) 
		{			
			(*itSvr).second->m_pConn->SendCT_CHATBAN_REQ(strName, wMin,m_dwChatBanSeq,pManager->m_dwID);
			bSend = TRUE;
		
			if( (*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR )
				m_dwSendCount++;
		}
	}

	if(!bSend)
		return EC_NOERROR;

	LPBANINFO stBANINFO = new BANINFO();;
	stBANINFO->m_strOPName = pManager->m_strID;
	stBANINFO->m_strBanName = strName;
	stBANINFO->m_wMin = wMin;
	stBANINFO->m_strReason = strReason;
	stBANINFO->m_dwID = m_dwChatBanSeq;
	stBANINFO->m_Time = CTime::GetCurrentTime().GetTime();

	m_mapBanInfo.insert(MAPBANINFO::value_type(m_dwChatBanSeq,stBANINFO));

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CHATBAN_ACK(LPPACKETBUF pBUF)
{
	DWORD dwBanSeq;
	DWORD dwManagerID;
	BYTE bRet;

	pBUF->m_packet
		>> bRet
		>> dwBanSeq
		>> dwManagerID;

    m_dwSendCount--;

	if(bRet)
		m_bChatBanSuccess = TRUE;

	if(m_dwSendCount)
		return EC_NOERROR;	

	MAPBANINFO::iterator it = m_mapBanInfo.find(dwBanSeq);
	if(it == m_mapBanInfo.end())
	{
		m_bChatBanSuccess = FALSE;
	}
	else if( !m_bChatBanSuccess)
	{
		delete (*it).second;
		m_mapBanInfo.erase(it);
	}

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager && m_dwSendCount == 0)
		pManager->SendCT_CHATBAN_ACK(m_bChatBanSuccess);
	
	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_SERVICEDATACLEAR_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{			
		(*itSvr).second->m_dwMaxUser = 0;
		(*itSvr).second->m_dwStopCount = 0;
		(*itSvr).second->m_nLatestStop = 0;
		(*itSvr).second->m_nPickTime = 0;
		(*itSvr).second->m_dwActiveUser = 0;

		if( (*itSvr).second->m_pConn )
		{
			CTime t(CTime::GetCurrentTime());
			(*itSvr).second->m_nLatestStop = t.GetTime();
			(*itSvr).second->m_nPickTime = t.GetTime();
			(*itSvr).second->m_pConn->SendCT_SERVICEDATACLEAR_ACK();
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_ITEMFIND_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))  
		return EC_NOERROR;

	WORD wItemID;
	CString strName;
	BYTE bWorldID;

	pBUF->m_packet
		>> wItemID
		>> strName
		>> bWorldID;


	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR &&
			(*itSvr).second->m_pGroup->m_bGroupID == bWorldID )
		{			
			(*itSvr).second->m_pConn->SendCT_ITEMFIND_REQ(pManager->m_dwID,wItemID, strName);
		}
	}

	return EC_NOERROR;

}

DWORD CTControlSvrModule::OnCT_ITEMFIND_ACK(LPPACKETBUF pBUF)
{
	DWORD dwManagerID;
	WORD wCount;
	WORD wItemID;
	BYTE bInitState;
	CString strName;

	pBUF->m_packet
		>> wCount
		>> dwManagerID;		

	CPacket* pMSG = new CPacket();
	pMSG->SetID(CT_ITEMFIND_ACK)
		<< wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wItemID
			>> bInitState
			>> strName;

		(*pMSG)
			<< wItemID
			<< bInitState
			<< strName;
	}

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_ITEMFIND_ACK(pMSG);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_ITEMSTATE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))  
		return EC_NOERROR;

	BYTE bWorldID;
	DWORD dwManagerID;
	WORD wCount;
	WORD wItemID;
	BYTE bInitState;

	pBUF->m_packet
		>> bWorldID
		>> dwManagerID
		>> wCount;

	CPacket* pMSG = new CPacket();
	pMSG->SetID(CT_ITEMSTATE_REQ)
		<< dwManagerID
		<< wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> wItemID
			>> bInitState;
			
		(*pMSG)
			<< wItemID
			<< bInitState;
	}

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR &&
			(*itSvr).second->m_pGroup->m_bGroupID == bWorldID)
		{			
			(*itSvr).second->m_pConn->SendCT_ITEMSTATE_REQ(pMSG);
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_ITEMSTATE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwID;
	
	pBUF->m_packet
		>> dwID;

	CTManager* pManager = FindManager(dwID);
	if(pManager)
		pManager->SendCT_ITEMSTATE_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CHATBANLIST_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))  
		return EC_NOERROR;

	pManager->SendCT_CHATBANLIST_ACK(&m_mapBanInfo);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CHATBANLISTDEL_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))  
		return EC_NOERROR;

	DWORD dwID;
	CString strBanName;
	MAPBANINFO::iterator it;

	pBUF->m_packet
		>> dwID;		
    
	if(!dwID)
	{
		for( it = m_mapBanInfo.begin(); it != m_mapBanInfo.end(); it++)		
			delete (*it).second;
		
		m_mapBanInfo.clear();
		return EC_NOERROR;
	}

	it = m_mapBanInfo.find(dwID);
	if(it != m_mapBanInfo.end() )
	{
		if( (*it).second->m_dwID == dwID )
		{
			delete (*it).second;
			m_mapBanInfo.erase(it);
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CASTLEINFO_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE bWorld;
	pBUF->m_packet
		>> bWorld;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_MAPSVR ) && (*itSvr).second->m_pGroup->m_bGroupID == bWorld )
		{			
			(*itSvr).second->m_pConn->SendCT_CASTLEINFO_REQ(pManager->m_dwID);
			break;
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CASTLEINFO_ACK(LPPACKETBUF pBUF)
{	

	DWORD dwManagerID;

	pBUF->m_packet
		>> dwManagerID;

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_CASTLEINFO_ACK(&(pBUF->m_packet));

	return EC_NOERROR;		

}

DWORD CTControlSvrModule::OnCT_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE bWorld;
	WORD wCastleID;
	DWORD dwDefGuildID;
	DWORD dwAtkGuildID;
	__time64_t tTime;

	pBUF->m_packet
		>> bWorld
		>> wCastleID
		>> dwDefGuildID
		>> dwAtkGuildID
		>> tTime;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR && (*itSvr).second->m_pGroup->m_bGroupID == bWorld ) )
		{			
			(*itSvr).second->m_pConn->SendCT_CASTLEGUILDCHG_REQ(wCastleID,dwDefGuildID,dwAtkGuildID,pManager->m_dwID,tTime);			
		}
	}

	return EC_NOERROR;	
}

DWORD CTControlSvrModule::OnCT_CASTLEGUILDCHG_ACK(LPPACKETBUF pBUF)
{	
	DWORD dwManagerID;

	pBUF->m_packet
		>> dwManagerID;		

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_CASTLEGUILDCHG_ACK(&(pBUF->m_packet));

	return EC_NOERROR;    
}

DWORD CTControlSvrModule::OnCT_CASTLEENABLE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE bWorld;
	BYTE bStatus;
	DWORD dwSecond;
	pBUF->m_packet
		>> bWorld
		>> bStatus
		>> dwSecond;

	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			((*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR ) && (*itSvr).second->m_pGroup->m_bGroupID == bWorld )
		{			
			(*itSvr).second->m_pConn->SendCT_CASTLEENABLE_REQ(BT_CASTLE,bStatus,dwSecond);
		}
	}

	return EC_NOERROR;    
}

DWORD CTControlSvrModule::OnCT_EVENTCHANGE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_GMLEVEL3))
		return EC_NOERROR;

	CString strValue=_T("");
	BYTE bRet;
	BYTE bType;
	EVENTINFO stEVENT;
	stEVENT.Reset();

	pBUF->m_packet
		>> bType;

	stEVENT.WrapPacketOut(&(pBUF->m_packet));		
	
	MAPEVENTINFO::iterator it = m_mapEVENT.find(stEVENT.m_dwIndex);

	if(bType == EK_DEL && it == m_mapEVENT.end() )
	{
		pManager->SendCT_EVENTCHANGE_ACK(ER_SUCCESS,bType,&stEVENT);
		return EC_NOERROR;
	}

	if(bType != EK_DEL)
	{
		CTime tStart(stEVENT.m_dStartDate);
		CTime tEnd(stEVENT.m_dEndDate);
		DWORD dwStartTime	= ( tStart.GetHour() * 60 ) + tStart.GetMinute();
		DWORD dwEndTime		= ( tEnd.GetHour()   * 60 ) + tEnd.GetMinute();

		__time64_t tCurTime = CTime::GetCurrentTime().GetTime();
		if(stEVENT.m_dEndDate <= tCurTime)
		{
			pManager->SendCT_EVENTCHANGE_ACK(ER_INVALIDTIME,bType,&stEVENT);  
			return EC_NOERROR;
		}

		if( stEVENT.m_dEndDate <= stEVENT.m_dStartDate )
		{
			pManager->SendCT_EVENTCHANGE_ACK(ER_INVALIDTIME,bType,&stEVENT);  
			return EC_NOERROR;
		}
			
		MAPEVENTINFO::iterator it2;
		for( it2 = m_mapEVENT.begin(); it2 != m_mapEVENT.end(); it2++)
		{	
			EVENTINFO*	OldEVENT = &((EVENTINFO)(*it2).second);

			if( bType == EK_UPDATE && it == it2 )
				continue;

			if( stEVENT.m_bID == OldEVENT->m_bID )
			{
				if( (OldEVENT->m_dStartDate <= stEVENT.m_dStartDate && OldEVENT->m_dEndDate >= stEVENT.m_dStartDate ) ||
					(OldEVENT->m_dStartDate <= stEVENT.m_dEndDate && OldEVENT->m_dEndDate >= stEVENT.m_dEndDate ) ||
					(stEVENT.m_dStartDate <= OldEVENT->m_dStartDate && stEVENT.m_dEndDate >= OldEVENT->m_dStartDate ) ||
					(stEVENT.m_dStartDate <= OldEVENT->m_dEndDate && stEVENT.m_dEndDate >= OldEVENT->m_dEndDate ))
				{
					if( !OldEVENT->m_bPartTime && !stEVENT.m_bPartTime )
					{
						CTime tOldStart(OldEVENT->m_dStartDate);
						CTime tOldEnd(OldEVENT->m_dEndDate);
						DWORD dwOldStartTime	= ( tOldStart.GetHour() * 60 ) + tOldStart.GetMinute();
						DWORD dwOldEndTime		= ( tOldEnd.GetHour()   * 60 ) + tOldEnd.GetMinute();

						if( (dwOldStartTime <= dwStartTime && dwOldEndTime >= dwStartTime) ||
							(dwOldStartTime <= dwEndTime) && (dwOldEndTime >= dwEndTime) ||
							(dwStartTime <= dwOldStartTime && dwEndTime >= dwOldStartTime) ||
							(dwStartTime <= dwOldEndTime) && (dwEndTime >= dwOldEndTime))
						{
							pManager->SendCT_EVENTCHANGE_ACK(ER_INVALIDTIME,bType,&stEVENT);  
							return EC_NOERROR;
						}
						else
							continue;
					}
					else
					{
						pManager->SendCT_EVENTCHANGE_ACK(ER_INVALIDTIME,bType,&stEVENT);  
						return EC_NOERROR;
					}
				}
			}
		}
	}

	if( !stEVENT.m_dwIndex && bType == EK_ADD)
		stEVENT.m_dwIndex = ++m_dwEventIndex;

	DEFINE_QUERY(&m_db,CSPEventUpdate)
	query->m_dwIndex = stEVENT.m_dwIndex;
	query->m_bType = bType;
	query->m_bID = stEVENT.m_bID;
	query->m_bGroupID = stEVENT.m_bGroupID;
	query->m_bSvrType = stEVENT.m_bSvrType;
	query->m_bSvrID = stEVENT.m_bSvrID;
	__TIMETODB(stEVENT.m_dStartDate,query->m_dStartDate);
	__TIMETODB(stEVENT.m_dEndDate, query->m_dEndDate);
	query->m_wValue = stEVENT.m_wValue;
	query->m_wMapID = stEVENT.m_wMapID;  
	query->m_dwStartAlarm = stEVENT.m_dwStartAlarm;
	query->m_dwEndAlarm =  stEVENT.m_dwEndAlarm;
	query->m_bPartTime =  stEVENT.m_bPartTime;
	lstrcpy(query->m_szStartMsg, stEVENT.m_strStartMsg);
	//lstrcpy(query->m_szMidMsg, stEVENT.m_strMidMsg);
	memset(query->m_szMidMsg, 0, sizeof(TCHAR) * (MAX_BOARD_TEXT +1));
	lstrcpy(query->m_szEndMsg, stEVENT.m_strEndMsg);
	lstrcpy(query->m_strTitle, stEVENT.m_strTitle);
	MakeStrValue(stEVENT,strValue,stEVENT.m_bID);
	lstrcpy(query->m_szValue, strValue);

	if(query->Call() )
		bRet = (BYTE)query->m_nRET;
	else
		bRet = ER_FAIL;

	UNDEFINE_QUERY()

	pManager->SendCT_EVENTCHANGE_ACK(bRet,bType,&stEVENT);	

	if(bRet)
		return EC_NOERROR;
	
	//	Ω««‡¡ﬂ¿Œ ∞ÊøÏ, (æ∆¿Ã≈€ √ﬂ√∑¿∫ Ω««‡¡ﬂ ∏¯∞Ìƒß,)
	if((*it).second.m_bState && stEVENT.m_bID != EVENT_LOTTERY && stEVENT.m_bID != EVENT_GIFTTIME)
	{
		MAPTSVRTEMP::iterator itSvr;
		for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
		{	
			if( (*itSvr).second->m_pConn &&
				(*itSvr).second->m_pSvrType->m_bType == stEVENT.m_bSvrType && 
				(!stEVENT.m_bGroupID || (*itSvr).second->m_pGroup->m_bGroupID == stEVENT.m_bGroupID ) &&
				(!stEVENT.m_bSvrID || stEVENT.m_bSvrID == (*itSvr).second->m_bServerID) )
			{	
				if(stEVENT.m_bID == EVENT_CASHSALE)
					(*itSvr).second->m_pConn->SendCT_CASHITEMSALE_REQ(stEVENT.m_dwIndex, 0, &stEVENT.m_vCashItem );
				else
					(*itSvr).second->m_pConn->SendCT_EVENTUPDATE_REQ(stEVENT.m_bID, 0, &(stEVENT));
			}
		}
	}

	if( it != m_mapEVENT.end())	
		m_mapEVENT.erase(it);
	
	if(bType != EK_DEL)
		m_mapEVENT.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT));		

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTUPDATE_REQ(LPPACKETBUF pBUF)
{	
	DWORD dwIndex;
	BYTE bEventID;
	WORD wValue;

	pBUF->m_packet
		>> dwIndex
		>> wValue;
	
	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;

	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it != m_mapEVENT.end() )
	{
		bGroupID = (*it).second.m_bGroupID;
		bSvrType = (*it).second.m_bSvrType;
		bSvrID = (*it).second.m_bSvrID;
		bEventID = (*it).second.m_bID;
		

		MAPTSVRTEMP::iterator itSvr;
		for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
		{	
			if( (*itSvr).second->m_pConn &&
				(*itSvr).second->m_pSvrType->m_bType == bSvrType && 
				(!bGroupID || (*itSvr).second->m_pGroup->m_bGroupID == bGroupID ) &&
				(!bSvrID || bSvrID == (*itSvr).second->m_bServerID) )
			{			
				(*itSvr).second->m_pConn->SendCT_EVENTUPDATE_REQ(bEventID,wValue,&((*it).second));				
			}
		}		
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CASHITEMSALE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwIndex;
	BYTE bEventID;
	WORD wValue;

	pBUF->m_packet
		>> dwIndex		
		>> wValue;
	
	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;
	BYTE bSend = FALSE;

	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it != m_mapEVENT.end() )
	{
		bGroupID = (*it).second.m_bGroupID;
		bSvrType = (*it).second.m_bSvrType;
		bSvrID = (*it).second.m_bSvrID;
		bEventID = (*it).second.m_bID;

		wValue = min(wValue,100);

		MAPTSVRTEMP::iterator itSvr;
		for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
		{	
			if( (*itSvr).second->m_pConn &&
				(*itSvr).second->m_pSvrType->m_bType == bSvrType && 
				(!bGroupID || (*itSvr).second->m_pGroup->m_bGroupID == bGroupID ) &&
				(!bSvrID || bSvrID == (*itSvr).second->m_bServerID) )
			{			
				(*itSvr).second->m_pConn->SendCT_CASHITEMSALE_REQ(dwIndex,wValue, &((*it).second.m_vCashItem) );				
			}
		}		
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF)
{
	DWORD dwIndex;
	BYTE bEventID;
	BYTE bEventMsgType;

	pBUF->m_packet
		>> dwIndex		
		>> bEventMsgType;

	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;
	CString strMsg = _T("");

	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it != m_mapEVENT.end() )
	{
		bGroupID = (*it).second.m_bGroupID;
		bSvrType = (*it).second.m_bSvrType;
		bSvrID = (*it).second.m_bSvrID;
		bEventID = (*it).second.m_bID;
		
		if( !bEventMsgType)
			strMsg = (*it).second.m_strStartMsg;
		else
			strMsg = (*it).second.m_strEndMsg;

		VBYTE vSvrGroup;
		if(!bGroupID && bSvrType == SVRGRP_WORLDSVR && !bSvrID)
		{			
			MAPTSVRTEMP::iterator itSvr;
			for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
			{		
				if( (*itSvr).second->m_pSvrType->m_bType == SVRGRP_RLYSVR  )
				{
					int id = (*itSvr).second->m_pGroup->m_bGroupID;
					if( (*itSvr).second->m_pConn )
					{				
						(*itSvr).second->m_pConn->SendCT_EVENTMSG_REQ(bEventID,bEventMsgType,strMsg);						
					}
					else 		
						vSvrGroup.push_back( (*itSvr).second->m_pGroup->m_bGroupID );
				}
			}
		}
		else
			vSvrGroup.push_back(bGroupID);

		for(DWORD i = 0; i < vSvrGroup.size(); i++ )
		{
			MAPTSVRTEMP::iterator itMap;
			for(itMap=m_mapTSVRTEMP.begin(); itMap!=m_mapTSVRTEMP.end(); itMap++)
			{	
				LPTSVRTEMP pSvrTemp = (*itMap).second;
				if( (*itMap).second->m_pConn &&
					(*itMap).second->m_pSvrType->m_bType == bSvrType &&
					(*itMap).second->m_pGroup->m_bGroupID == vSvrGroup[i]  &&
					(!bSvrID || bSvrID == bSvrID) )	
				{				
					(*itMap).second->m_pConn->SendCT_EVENTMSG_REQ(bEventID,bEventMsgType,strMsg);
				}
			}
		}		
	}

	return EC_NOERROR;
}


DWORD CTControlSvrModule::OnCT_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF)
{
	DWORD dwIndex;

	pBUF->m_packet
		>> dwIndex;
		
	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;
	
	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it != m_mapEVENT.end() )
	{
		bGroupID = (*it).second.m_bGroupID;
		bSvrType = (*it).second.m_bSvrType;
		bSvrID = (*it).second.m_bSvrID;
		
		MAPTSVRTEMP::iterator itMap;
		for(itMap=m_mapTSVRTEMP.begin(); itMap!=m_mapTSVRTEMP.end(); itMap++)
		{	
			LPTSVRTEMP pSvrTemp = (*itMap).second;
			if( (*itMap).second->m_pConn &&
				(*itMap).second->m_pSvrType->m_bType == bSvrType &&
				(!bGroupID || (*itMap).second->m_pGroup->m_bGroupID == bGroupID)  &&
				(!bSvrID || bSvrID == bSvrID) )	
			{
				(*itMap).second->m_pConn->SendCT_CASHSHOPSTOP_REQ(TRUE);				
			}
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTLIST_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_USER))
		return EC_NOERROR;

	pManager->SendCT_EVENTLIST_ACK(&m_mapEVENT);	

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTDEL_REQ(LPPACKETBUF pBUF)
{
	BYTE bRet = TRUE;
	DWORD dwIndex;
	EVENTINFO stEVENT;
	stEVENT.Reset();
	

	pBUF->m_packet
		>> dwIndex;

	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it == m_mapEVENT.end())
		return EC_NOERROR;

	stEVENT.m_bID = (*it).second.m_bID;
	
	
	DEFINE_QUERY(&m_db,CSPEventUpdate)
	query->m_dwIndex = dwIndex;
	query->m_bType = EK_DEL;
	query->m_bID = stEVENT.m_bID;
	query->m_bGroupID = stEVENT.m_bGroupID;
	query->m_bSvrType = stEVENT.m_bSvrType;
	query->m_bSvrID = stEVENT.m_bSvrID;
	__TIMETODB(stEVENT.m_dStartDate,query->m_dStartDate);
	__TIMETODB(stEVENT.m_dEndDate, query->m_dEndDate);
	query->m_wValue = stEVENT.m_wValue;
	query->m_wMapID = stEVENT.m_wMapID;
	query->m_dwStartAlarm = stEVENT.m_dwStartAlarm;
	query->m_dwEndAlarm =  stEVENT.m_dwEndAlarm;
	lstrcpy(query->m_szStartMsg, stEVENT.m_strStartMsg);
	//lstrcpy(query->m_szMidMsg, stEVENT.m_strMidMsg);
	memset(query->m_szMidMsg, 0, sizeof(TCHAR) * (MAX_BOARD_TEXT +1));
	lstrcpy(query->m_szEndMsg, stEVENT.m_strEndMsg);
	lstrcpy(query->m_strTitle, stEVENT.m_strTitle);
	//lstrcpy(query->m_szValue , stEVENT.m_strValue);
	memset(query->m_szValue, 0, sizeof(TCHAR) * (MAX_BOARD_TEXT +1));


	if(query->Call() )
		bRet = (BYTE)query->m_nRET;
	UNDEFINE_QUERY()

	if(bRet)
		return EC_NOERROR;

	m_mapEVENT.erase(it);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CASHITEMLIST_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION;

	
	VTCASHITEM vCashItem;
	vCashItem.clear();


	DEFINE_QUERY(&m_db,CTBLCashShopItem)
	if(query->Open())
	{
		while(query->Fetch())
		{
			TCASHITEM stCASHITEM;
			stCASHITEM.m_wID = query->m_wID;
			stCASHITEM.m_strName = query->m_szName;

			vCashItem.push_back(stCASHITEM);
		}
		query->Close();
	}
	UNDEFINE_QUERY()

	pManager->SendCT_CASHITEMLIST_ACK(&vCashItem);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CASHITEMSALE_ACK(LPPACKETBUF pBUF)
{
	DWORD dwIndex;
	WORD wValue;

	pBUF->m_packet
		>> dwIndex
		>> wValue;

	if(wValue)
		return EC_NOERROR;

	MAPEVENTINFO::iterator it = m_mapEVENT.find(dwIndex);
	if( it == m_mapEVENT.end())
		return EC_NOERROR;

	return EC_NOERROR;	
}
 
DWORD CTControlSvrModule::OnCT_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	DWORD dwManagerID;
	BYTE bGroup, bDay;

	pBUF->m_packet
		>> bGroup
		>> dwManagerID
		>> bDay;
	
	if(pManager->m_dwID != dwManagerID)
		return EC_NOERROR;

    
	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn && (*itSvr).second->m_pGroup->m_bGroupID == bGroup &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR )
		{			
			(*itSvr).second->m_pConn->SendCT_EVENTQUARTERLIST_REQ(&(pBUF->m_packet));
			break;
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTQUARTERLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD dwManagerID;
	WORD wCount;

	pBUF->m_packet
		>> dwManagerID
		>> wCount;

	CTManager* pManager = FindManager(dwManagerID);
	if(!pManager)
		return EC_NOERROR;

	pManager->SendCT_EVENTQUARTERLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	DWORD dwManagerID;
	BYTE bGroup;
	BYTE bType;

	pBUF->m_packet
		>> bGroup
		>> dwManagerID
		>> bType;

	if(pManager->m_dwID != dwManagerID)
		return EC_NOERROR;


	MAPTSVRTEMP::iterator itSvr;
	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn && (*itSvr).second->m_pGroup->m_bGroupID == bGroup &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR )
		{			
			(*itSvr).second->m_pConn->SendCT_EVENTQUARTERUPDATE_REQ(&(pBUF->m_packet));			
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_EVENTQUARTERUPDATE_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwManagerID;	

	pBUF->m_packet
		>> bRet
		>> dwManagerID;
		
	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_EVENTQUARTERUPDATE_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	BYTE bGroupID;

	pBUF->m_packet
		>> bGroupID;

	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR &&
			(*itSvr).second->m_pGroup->m_bGroupID == bGroupID)
		{			
			(*itSvr).second->m_pConn->SendCT_TOURNAMENTEVENT_REQ(&(pBUF->m_packet));			
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_TOURNAMENTEVENT_ACK(LPPACKETBUF pBUF)
{

	DWORD dwManagerID;

	pBUF->m_packet
		>> dwManagerID;

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_TOURNAMENTEVENT_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_HELPMESSAGE_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	BYTE bGroupID;

	pBUF->m_packet
		>> bGroupID;

	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_HELPMESSAGE_REQ(&(pBUF->m_packet));
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_RPSGAMEDATA_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	BYTE bGroupID;

	pBUF->m_packet
		>> bGroupID;

	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_RPSGAMEDATA_REQ(&(pBUF->m_packet));
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF)
{
	MAPTSVRTEMP::iterator itSvr;
	BYTE bGroupID;

	pBUF->m_packet
		>> bGroupID;

	for(itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_RPSGAMECHANGE_REQ(&(pBUF->m_packet));
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_RPSGAMEDATA_ACK(LPPACKETBUF pBUF)
{	
	MAPTMANAGER::iterator it;
	for(it=m_mapMANAGER.begin(); it!=m_mapMANAGER.end(); it++)
		(*it).second->SendCT_RPSGAMEDATA_ACK(&(pBUF->m_packet));	
	return EC_NOERROR;
}


DWORD CTControlSvrModule::OnCT_CMGIFT_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE	bGroupID;
	CString strTarget;
	WORD	wGiftID;

	pBUF->m_packet
		>> bGroupID
		>> strTarget
		>> wGiftID;

	for(MAPTSVRTEMP::iterator itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_CMGIFT_REQ(strTarget, wGiftID, pManager->m_dwID);
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CMGIFT_ACK(LPPACKETBUF pBUF)
{
	BYTE bRet;
	DWORD dwManagerID;

	pBUF->m_packet
		>> bRet
		>> dwManagerID;

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_CMGIFT_ACK(bRet);

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE	bGroupID;
	pBUF->m_packet
		>> bGroupID;

	for(MAPTSVRTEMP::iterator itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_CMGIFTCHARTUPDATE_REQ(&(pBUF->m_packet), pManager->m_dwID);
		}
	}
	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CMGIFTLIST_REQ(LPPACKETBUF pBUF)
{
	CTManager * pManager = (CTManager *)pBUF->m_pSESSION; 
	if(!pManager->CheckAuthority(MANAGER_ALL))
		return EC_NOERROR;

	BYTE	bGroupID;
	pBUF->m_packet
		>> bGroupID;

	for(MAPTSVRTEMP::iterator itSvr=m_mapTSVRTEMP.begin(); itSvr!=m_mapTSVRTEMP.end(); itSvr++)
	{	
		if( (*itSvr).second->m_pConn &&
			(*itSvr).second->m_pSvrType->m_bType == SVRGRP_WORLDSVR)
		{
			if(	(*itSvr).second->m_pGroup->m_bGroupID == bGroupID || !bGroupID)
				(*itSvr).second->m_pConn->SendCT_CMGIFTLIST_REQ(pManager->m_dwID);
		}
	}

	return EC_NOERROR;
}

DWORD CTControlSvrModule::OnCT_CMGIFTLIST_ACK(LPPACKETBUF pBUF)
{
	DWORD	dwManagerID;
	pBUF->m_packet
		>> dwManagerID;

	CTManager* pManager = FindManager(dwManagerID);
	if(pManager)
		pManager->SendCT_CMGIFTLIST_ACK(&(pBUF->m_packet));

	return EC_NOERROR;
}
