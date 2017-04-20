#include "stdafx.h"
#include "HappyDoc.h"
#include "Happy.h"
#include "MainFrm.h"
#include "HappyView.h"

void CHappyDoc::OnCT_OPLOGIN_ACK(CPacket * pPacket)
{
	BYTE bRet;
	BYTE bAuthority;
	DWORD dwManagerID;

	(*pPacket) 
		>> bRet
		>> bAuthority
		>> dwManagerID;

	if(bRet != ACK_SUCCESS)
	{
		AfxMessageBox(IDS_CONNECT_FAIL);
		m_session.End();
		
		// 박상연 06/02/02 연결시 Compare버튼 활성화
		//-------------------------------------------------
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetPatchBarCompareVisible(FALSE);
		//-------------------------------------------------
		return;
	}

	m_bAuthority = bAuthority;
	m_dwManagerID = dwManagerID;
	
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();

	///////////////////////////////////////
	// 현승룡 버튼 활성화/비활성화
	pFrm->SetGraphButton(TRUE); 
	pFrm->SetAllView(TRUE);	
	pFrm->SetAlertButton(TRUE);
	pFrm->SetServiceButton(FALSE);
	pFrm->SetServiceDataClear(FALSE);
	pFrm->SetExportHappyData(FALSE);
	pFrm->SetChatBanList(FALSE);

	switch(m_bAuthority) 
	{	
	case MANAGER_ALL:
		pFrm->SetServiceDataClear(TRUE);
		pFrm->SetChatBanList(TRUE);

	case MANAGER_CONTROL:		
		pFrm->SetPatchBarCompareVisible(TRUE); // 박상연 06/02/02 연결시 Compare버튼 활성화
	
	case MANAGER_USER:
		pFrm->SetServiceLog(TRUE);
		pFrm->SetUserWeb(TRUE);		
		pFrm->SetAutoStart(TRUE);
		
	case MANAGER_SERVICE:
		pFrm->SetServiceButton(TRUE);
		pFrm->SetExportHappyData(TRUE);

	default: 
		pFrm->SetGMTool(TRUE);
		break;
	}
	///////////////////////////////////////

	// 메인 윈도우 타이틀 바꾸기	
	CString strTitle = _T(" - Happy ") + m_strIP;
	CWnd* pWnd = AfxGetMainWnd();	
	pWnd->SetWindowText(strTitle);

	InitEventTitle();
}

void CHappyDoc::OnCT_SVRTYPELIST_ACK(CPacket * pPacket)
{
	DWORD	dwNum;
	BYTE	bType;
	CString strName;

	ClearSvrType();

	(*pPacket) >> dwNum;

	for( DWORD i = 0; i < dwNum; i++)
	{
		(*pPacket)
			>> bType
            >> strName;

		LPHAPPYSVRTYPE pSvrType = new HAPPYSVRTYPE();
		pSvrType->m_bType = bType;
		pSvrType->m_strName = strName;
		m_mapSvrType.insert(MAPSVRTYPE::value_type(bType, pSvrType));
	}
}

void CHappyDoc::OnCT_MACHINELIST_ACK(CPacket * pPacket)
{
	DWORD	dwNum;
	BYTE	bID;
	CString strName;

	ClearMachine();
	ClearPlatform();

	(*pPacket) >> dwNum;

	for( DWORD i = 0; i < dwNum; i++)
	{
		(*pPacket) >> bID
			      >> strName;

		LPHAPPYMACHINE pMachine = new HAPPYMACHINE();
		pMachine->m_bID = bID;
		pMachine->m_strName = strName;
		m_mapMachine.insert(MAPMACHINE::value_type(bID, pMachine));

		VPLATFORM vPlatform;
		vPlatform.clear();
		m_mapPLATFORM.insert(MAPPLATFORM::value_type(bID, vPlatform));
	}
}

void CHappyDoc::OnCT_GROUPLIST_ACK(CPacket * pPacket)
{
	BYTE	bID;
	DWORD	dwNum;	
	CString strName;

	ClearGroup();

	(*pPacket) >> dwNum;

	for( DWORD i = 0; i < dwNum; i++)
	{
		(*pPacket)
			>> bID
            >> strName;

		LPHAPPYGROUP pGroup = new HAPPYGROUP();
		pGroup->m_bID = bID;
		pGroup->m_strName = strName;
		m_mapGroup.insert(MAPGROUP::value_type(bID, pGroup));
	}

	SendCT_SERVICESTAT_REQ();
}

void CHappyDoc::OnCT_SERVICESTAT_ACK(CPacket * pPacket)
{
	DWORD dwNum;

	BYTE bID;
	BYTE bGroup;
	BYTE bMachine;
	BYTE bType;
	DWORD dwStatus;	
	CString strName;

	ClearService();
	ClearServiceGraph();

	(*pPacket) >> dwNum;
	for( DWORD i = 0; i < dwNum; i++)
	{
		(*pPacket)
			>> bGroup
			>> bType
			>> bID
			>> strName
			>> bMachine
			>> dwStatus;

		DWORD dwID = MAKESVRID(bGroup, bType, bID);
		MAPSERVICE::iterator itS = m_mapService.find(dwID);
		if(itS!=m_mapService.end())
			continue;

		LPHAPPYSERVICE pService = new HAPPYSERVICE();
		pService->m_dwID = dwID;
		pService->m_bID = bID;
		pService->m_dwStatus = dwStatus;
		pService->m_strName = strName;
		pService->m_dwCurSession = 0;
		pService->m_dwCurUser = 0;
		pService->m_dwMaxUser = 0;
		pService->m_dwActiveUser = 0;
		pService->m_dwPing = 0;
		pService->m_dwStopCount = 0;		
		pService->m_nStopTime = 0;
		pService->m_nPickTime = 0;
		pService->m_pMachine = NULL;
		pService->m_pGroup = NULL;
		pService->m_pSvrType = NULL;

		MAPMACHINE::iterator itMc = m_mapMachine.find(bMachine);
		if(itMc != m_mapMachine.end())
			pService->m_pMachine = (*itMc).second;
		MAPGROUP::iterator itGp = m_mapGroup.find(bGroup);
		if(itGp != m_mapGroup.end())
			pService->m_pGroup = (*itGp).second;
		MAPSVRTYPE::iterator itSt = m_mapSvrType.find(bType);
		if(itSt != m_mapSvrType.end())
			pService->m_pSvrType = (*itSt).second;

		m_mapService.insert(MAPSERVICE::value_type(dwID, pService));

		// 현승룡 Service Graph
		SERVICEGRAPH sGraph;
		sGraph.m_dwID = pService->m_dwID;
		sGraph.m_strWorld = pService->m_pGroup->m_strName;
		sGraph.m_strGroup = pService->m_pSvrType->m_strName;
		sGraph.m_strService = pService->m_strName;
		sGraph.m_dwPeekUser = 0;
		sGraph.m_vPing.push_back(0);
		sGraph.m_vCurUser.push_back(0);
		sGraph.m_pSvrType = (*itSt).second;

		m_mapSERVICEGRAPH.insert(MAPSERVICEGRAPH::value_type(dwID, sGraph));



		DWORD dwNewID = dwID;
		BYTE bSvrType = bType;
		CString strGroup = strName;
		if(bSvrType != SVRGRP_LOGINSVR && bSvrType != SVRGRP_WORLDSVR && bSvrType != SVRGRP_MAPSVR  )
			continue;
		
		if(bGroup && bSvrType == SVRGRP_WORLDSVR)
		{
			dwNewID = MAKESVRID(0,SVRGRP_WORLDSVR,0);
			m_mapEventSvr.insert(MAPDWORDSTRING::value_type(dwNewID,STR_WORLD_ALL));
		}

		if( bSvrType == SVRGRP_WORLDSVR)
		{
			bSvrType = SVRGRP_WORLDSVR;
			dwNewID = MAKESVRID(bGroup,bSvrType,0);
		}
		if(bGroup)
		{
			MAPGROUP::iterator itG = m_mapGroup.find(bGroup);
			if(itG != m_mapGroup.end())
                strGroup = (*itG).second->m_strName;
		}

		m_mapEventSvr.insert(MAPDWORDSTRING::value_type(dwNewID,strGroup));
	}

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
	CServiceList * pList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);	
	CMachine * pMachine = (CMachine *)pFrm->m_wndSplitter.GetView(pFrm->m_nMachine); // 현승룡 Machine

	pMachine->InsertInitData();
	pTree->InsertInitData();
	pTree->UpdateServiceTree(0);
	pList->UpdateServiceList(0);
}
void CHappyDoc::OnCT_SERVICECHANGE_ACK(CPacket * pPacket)
{
	DWORD dwID;
	DWORD dwStatus;

	(*pPacket)
		>> dwID
		>> dwStatus;

	MAPSERVICE::iterator it;
	for(it=m_mapService.begin(); it!=m_mapService.end(); it++)
	{
		LPHAPPYSERVICE pService = (*it).second;

		if(pService->m_dwID == dwID)
		{
			if(pService->m_dwStatus != dwStatus)
			{
				pService->m_dwStatus = dwStatus;
				CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
				CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
				pTree->UpdateServiceTree(dwID);
			}
		}
	}
}
void CHappyDoc::OnCT_SERVICEDATA_ACK(CPacket * pPacket)
{
	DWORD dwID;
	DWORD dwSession;
	DWORD dwCurUser;
	DWORD dwMaxUser;
	DWORD dwActiveUser = 0;
	DWORD dwPing;
	DWORD dwStopCnt;
	__int64 nPickTime;
	__int64 nStopTime;

	(*pPacket)
		>> dwID
		>> dwSession
		>> dwCurUser
		>> dwMaxUser		
		>> dwPing
		>> nPickTime
		>> dwStopCnt
		>> nStopTime
		>> dwActiveUser;

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceList * pList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);

	MAPSERVICE::iterator it = m_mapService.find(dwID);	
	if(it!=m_mapService.end())
	{		
		(*it).second->m_dwCurSession = dwSession;
		(*it).second->m_dwCurUser = dwCurUser;
		(*it).second->m_dwMaxUser = dwMaxUser;		
		(*it).second->m_nPickTime = nPickTime;
		(*it).second->m_dwPing = dwPing;
		(*it).second->m_dwStopCount = dwStopCnt;
		(*it).second->m_nStopTime = nStopTime;
		(*it).second->m_dwActiveUser = dwActiveUser;
		
		pList->UpdateServiceList(dwID);

		// 현승룡 Service Graph
		MAPSERVICEGRAPH::iterator itG = m_mapSERVICEGRAPH.find(dwID);
		if(itG!=m_mapSERVICEGRAPH.end())
		{
			(*itG).second.m_vCurUser.push_back(dwCurUser);
			(*itG).second.m_vPing.push_back(dwPing);
			if(dwCurUser > (*itG).second.m_dwPeekUser)
                (*itG).second.m_dwPeekUser = dwCurUser;

			if((*itG).second.m_vCurUser.size() == MAX_POINT_CNT)
				(*itG).second.m_vCurUser.erase((*itG).second.m_vCurUser.begin());

			if((*itG).second.m_vPing.size() == MAX_POINT_CNT)
				(*itG).second.m_vPing.erase((*itG).second.m_vPing.begin());
		}
	}

	// 현승룡 Service Graph
	if( GetCurView() == 1 && pFrm->GetGraphCheck() )
	{
		CServiceGraph * pGraph = (CServiceGraph *)pFrm->m_wndSplitter.GetView(pFrm->m_nGraph);
		pGraph->UpdateServiceGraph(dwID);
	}
}
void CHappyDoc::OnCT_SERVICEUPLOADSTART_ACK(CPacket * pPacket)
{
	BYTE bRet;
	(*pPacket) >> bRet;

	if(!bRet)
		ServiceUpload();
	else
		NotifyUploadEnd(bRet);
}
void CHappyDoc::OnCT_SERVICEUPLOAD_ACK(CPacket * pPacket)
{
	ServiceUpload();
}
void CHappyDoc::OnCT_SERVICEUPLOADEND_ACK(CPacket * pPacket)
{
	BYTE bRet;
	(*pPacket)
		>> bRet;

	NotifyUploadEnd(bRet);
}
void CHappyDoc::OnCT_UPDATEPATCH_ACK(CPacket * pPacket)
{
	BYTE bRet;
	(*pPacket)
		>> bRet;
	if( bRet != 0 )
	{
		m_bErrorStatus = _ERR_DBWRITE;
	}
}
////////////////////////////////////////////////////////////
// 현승룡 CT_AUTHORITY_ACK
void CHappyDoc::OnCT_AUTHORITY_ACK(CPacket * pPacket)
{
	AfxMessageBox("You don't have the authority"); //권한이 아닙니다.
}

// 현승룡 CT_PLATFORM_ACK
void CHappyDoc::OnCT_PLATFORM_ACK(CPacket * pPacket)
{
	BYTE bMachineID;
	DWORD dwCPU;
	DWORD dwMEM;
	float fNET;

	(*pPacket)
		>> bMachineID
		>> dwCPU
		>> dwMEM
		>> fNET;

	MAPPLATFORM::iterator it = m_mapPLATFORM.find(bMachineID);
	if(it!=m_mapPLATFORM.end())
	{
		PLATFORM plat;
		plat.m_dwCPU = dwCPU;
		plat.m_dwMEM = dwMEM;
		plat.m_fNET = fNET;
		(*it).second.push_back(plat);

		if((*it).second.size() == MAX_POINT_CNT)
			(*it).second.erase((*it).second.begin());

		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		CMachineGraph * pGraph = (CMachineGraph *)pFrm->m_wndSplitter.GetView(pFrm->m_nMachineGraph);
		if( GetCurView() == 3 ) // Machine Tab
			pGraph->UpdateMachineGraph(bMachineID);
	}
}

// 현승룡 CT_MONSPAWNFIND_ACK
void CHappyDoc::OnCT_MONSPAWNFIND_ACK(CPacket * pPacket)
{
	WORD wMapID;
	WORD wSpawnID;
	BYTE bMonCnt;

	DWORD dwMonID;
	DWORD dwHostID;
	BYTE bStat;
	float fPosX;
	float fPosY;
	float fPosZ;

	(*pPacket)
		>> wMapID
		>> wSpawnID
		>> bMonCnt;

	for(BYTE bCnt=0; bCnt<bMonCnt; bCnt++)
	{
		(*pPacket)
			>> dwMonID
			>> dwHostID
			>> bStat
			>> fPosX
			>> fPosY
			>> fPosZ;

		m_pMonSpawn->AddItemData(dwMonID, dwHostID, bStat, fPosX, fPosY, fPosZ);
	}
}

void CHappyDoc::OnCT_USERPROTECTED_ACK(CPacket * pPacket)
{
	BYTE bRet;

	(*pPacket)
		>> bRet;

	CString str = bRet ? MSG_FAIL : MSG_SUCCESS;
}
////////////////////////////////////////////////////////////

void CHappyDoc::OnCT_SERVICEAUTOSTART_ACK(CPacket* pPacket)
{
	BYTE bRet;
	(*pPacket)
		>> bRet;
	
	m_bServiceAutoStart = bRet;
}

void CHappyDoc::OnCT_CHATBAN_ACK(CPacket* pPacket)
{
	BYTE bRet;
	(*pPacket)
		>> bRet;

	// 여기서 AfxMessageBox 를 띄우면 이 함수(OnCT_CHATBAN_ACK())가 무한 실행됨.
	// 메세지 처리중에 MessageBox 나 DialogBox 를 뛰우면 똑같은 메세지를 무한호출함.
	//if(!bRet)
	//	AfxMessageBox(_T("Invalid User"), MB_OK);
	//else
	//	AfxMessageBox(_T("Success"), MB_OK);

	ShowRetDlg(SW_SHOW,bRet,_T(""));

	// view 얻기
	CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if(pView && pView->m_dlgGMTool && bRet)	
		pView->m_dlgGMTool->m_dlgChatBan->Init();
}

void CHappyDoc::OnCT_ITEMFIND_ACK(CPacket* pPacket)
{
	WORD wCount;
	BYTE bInitState;
	WORD wItemID;
	CString strName;

	(*pPacket)
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		(*pPacket)
			>> wItemID
			>> bInitState
			>> strName;

		ITEMSTATE ItemState;
		ItemState.m_wItemID = wItemID;
		ItemState.m_bInitState = bInitState;
		ItemState.m_strName = strName;

		if(m_pdlgGMTool)
			m_pdlgGMTool->m_dlgItemChgState->InsertItemToListCtr(&ItemState);
	}
}

void CHappyDoc::OnCT_ITEMSTATE_ACK(CPacket* pPacket)
{
	WORD wCount;	
	BYTE bInitState;
	WORD wItemID;
	DWORD dwID;

	(*pPacket)
		>> dwID
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		(*pPacket)
			>> wItemID			
			>> bInitState;

		if(m_pdlgGMTool)
			m_pdlgGMTool->m_dlgItemChgState->ChangeItemState(wItemID,bInitState);
	}

	CString str;
	str.Format(_T("%d Items Success!!"),wCount);

	ShowRetDlg(SW_SHOW,(BYTE)wCount,str);
}

void CHappyDoc::OnCT_CHATBANLIST_ACK(CPacket* pPacket)
{
	WORD wCount;
	DWORD dwID;
	CString strName;
	__time64_t dBanTime;
	WORD wMin;
	CString strReason;
	CString strOPName;
    
	(*pPacket)
		>> wCount;

	for(WORD i = 0; i < wCount ; i++)
	{
		(*pPacket)
			>> dwID
			>> strName
			>> dBanTime
			>> wMin
			>> strReason
			>> strOPName;

		// View 얻기
		CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
		if(pView)
            pView->m_dlgChatBanList->InsertToListCtrl(dwID,strName,dBanTime,wMin,strReason,strOPName);
	}
}

void CHappyDoc::OnCT_CASTLEINFO_ACK(CPacket* pPacket)
{
	CASTLEINFO CastleInfo;
	DWORD dwManagerID;
	WORD wCount;

	if(!m_pdlgGMTool)
		return;

	m_pdlgGMTool->m_dlgCastleSetting->ClearCastleIDCombo();

	(*pPacket)
		>> dwManagerID
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		(*pPacket)
			>> CastleInfo.m_wCastleID
			>> CastleInfo.m_strName
			>> CastleInfo.m_bState
			>> CastleInfo.m_dwDefGuildID
			>> CastleInfo.m_strDefGuild
			>> CastleInfo.m_bCountry
			>> CastleInfo.m_dwAtkGuildID
			>> CastleInfo.m_strAtkGuild
			>> CastleInfo.m_tNextWar;

			m_pdlgGMTool->m_dlgCastleSetting->InsertItemToListCtr(CastleInfo);
			m_pdlgGMTool->m_dlgCastleSetting->InsertCastleID(CastleInfo.m_wCastleID);		
	}

	m_pdlgGMTool->m_dlgCastleSetting->SetCastleState();
}

void CHappyDoc::OnCT_CASTLEGUILDCHG_ACK(CPacket* pPacket)
{	
	DWORD dwManagerID;
	BYTE bRet;
	WORD wCastleID;
	DWORD dwDefGuildID;
	CString strDefGuild;
	DWORD dwAtkGuildID;
	CString strAtkGuild;
	__time64_t tTime;

	(*pPacket)
		>> dwManagerID
		>> bRet
		>> wCastleID
		>> dwDefGuildID
		>> strDefGuild
		>> dwAtkGuildID
		>> strAtkGuild
		>> tTime;


	CString str;
	if(bRet)
		str = _T("Castle Guild Info Change Success");
	else
		str = _T("Castle Guild Info is invalid");

	ShowRetDlg(SW_SHOW,bRet,str);

	if(m_pdlgGMTool && m_pdlgGMTool->m_dlgCastleSetting && bRet)
		m_pdlgGMTool->m_dlgCastleSetting->ChangeCastleInfo(wCastleID,dwDefGuildID,strDefGuild,dwAtkGuildID,strAtkGuild,tTime);

}

void CHappyDoc::OnCT_EVENTCHANGE_ACK(CPacket* pPacket)
{
	BYTE bRet;
	BYTE bType;
	EVENTINFO stEVENT;

	(*pPacket)
		>> bRet
		>> bType;

	stEVENT.WrapPacketOut(pPacket);
	CString str = _T("");
	MAPBYTESTRING::iterator it = m_mapEventName.find(stEVENT.m_bID);
	if(it == m_mapEventName.end())
		return;

	str = (*it).second;

	if(m_pdlgGMTool && m_pdlgGMTool->m_dlgEvent)
	{
		SendCT_EVENTLIST_REQ();
		SendCT_CASHITEMLIST_REQ();
	}

	MAPEVENTINFO::iterator itE = m_mapEventInfo.find(stEVENT.m_dwIndex);
	if(itE != m_mapEventInfo.end())
		m_mapEventInfo.erase(itE);
	m_mapEventInfo.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT));

	if(bRet)
		str = str + _T(" Event Update Failed");
	else
		str = str + _T(" Event Update Success");

	//ShowRetDlg(SW_SHOW,TRUE,str);
	AfxMessageBox(str, MB_ICONINFORMATION);

	if(!bRet && m_pdlgGMTool->m_dlgEvent->m_pdlg)
		m_pdlgGMTool->m_dlgEvent->m_pdlg->EndDialog(IDOK);
}

void CHappyDoc::OnCT_EVENTLIST_ACK(CPacket* pPacket)
{
	if(!m_pdlgGMTool || !m_pdlgGMTool->m_dlgEvent)
		return;

	m_pdlgGMTool->m_dlgEvent->ClearListItem();
	m_mapEventInfo.clear();

	WORD wCount;	

	(*pPacket)
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		EVENTINFO stEVENT;

		stEVENT.WrapPacketOut(pPacket);
		m_pdlgGMTool->m_dlgEvent->AddListData(&stEVENT);

		m_mapEventInfo.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT));
	}
}

void CHappyDoc::OnCT_CASHITEMLIST_ACK(CPacket* pPacket)
{
	WORD wCount;

	(*pPacket)
		>> wCount;

	m_mapCashItem.clear();
	for(WORD i = 0; i < wCount; i++)
	{
		TCASHITEM stCASHITEM;
		(*pPacket)
			>> stCASHITEM.m_wID
			>> stCASHITEM.m_strName;

		m_mapCashItem.insert(MAPTCASHITEM::value_type(stCASHITEM.m_wID,stCASHITEM));
	}
}

void CHappyDoc::OnCT_EVENTQUARTERLIST_ACK(CPacket* pPacket)
{
	DWORD dwManagerID;
	WORD wCount;

	(*pPacket)
		>> dwManagerID
		>> wCount;

	m_pdlgGMTool->m_dlgLuckyEvent->ClearListItem();
	ClearLuckyEvent();

	for(WORD i = 0; i < wCount; i++ )
	{
		LUCKYEVENT stLKEVENT;

		stLKEVENT.WrapPacketOut(pPacket);

		AddLuckEvent(stLKEVENT);
		m_pdlgGMTool->m_dlgLuckyEvent->InsertItemToListCtr(&stLKEVENT);
	}
}

void CHappyDoc::OnCT_EVENTQUARTERUPDATE_ACK(CPacket* pPacket)
{
	BYTE bRet;
	BYTE bType;
	DWORD dwManagerID;
	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();

	(*pPacket)
		>> bRet
		>> dwManagerID
		>> bType;

	stLKEVENT.WrapPacketOut(pPacket);
	
	if(!bRet)
	{
		switch(bType)
		{
		case EK_DEL:
			DelLuckyEvent(stLKEVENT.m_wID);
			m_pdlgGMTool->m_dlgLuckyEvent->DeleteListData(stLKEVENT.m_wID);
			break;
		case EK_ADD:
			AddLuckEvent(stLKEVENT);
			m_pdlgGMTool->m_dlgLuckyEvent->InsertItemToListCtr(&stLKEVENT);
			break;
		case EK_UPDATE:
			{
				LPLUCKYEVENT pEvent = GetLuckyEvent(stLKEVENT.m_wID);
				if(pEvent)
					pEvent->Copy(&stLKEVENT);
				m_pdlgGMTool->m_dlgLuckyEvent->UpdateListData(pEvent);
			}
			break;
		}
	}

	CString str;
	if(bRet)
		str = _T("Lucky Event Update Failed !!");
	else
	//{
		str = _T("Lucky Event Update Success !!");
		//m_pdlgGMTool->m_dlgLuckyEvent->OnBnClickedBtnLkRefresh();
	//}

	ShowRetDlg(SW_SHOW,(BYTE)bRet,str);

}

void CHappyDoc::OnCT_TOURNAMENTEVENT_ACK(CPacket* pPacket)
{
	DWORD dwManagerID;
	BYTE bType;

	(*pPacket)
		>> dwManagerID
		>> bType;

	
	switch(bType)
	{
	case TET_LIST:
		{
			ClearTournament();
			m_pdlgGMTool->m_dlgTournament->ClearCtrl();
			m_pdlgGMTool->m_dlgTournament->Init();
			m_pdlgGMTool->m_dlgTournament->ClearTList();

			WORD wTNTID;
			BYTE bCount;
			BYTE i,j;

			(*pPacket)
				>> bCount;

			for( i = 0; i < bCount; i++)
			{
				(*pPacket)
					>> wTNTID;

				LPTOURNAMENT pTNT = new TOURNAMENT();
				pTNT->m_wID = wTNTID;

				pTNT->m_stTSTEP.WrapPacketOut(pPacket);

				CString s1,s2,s3,s4;
				s1.Format("%d-",pTNT->m_stTSTEP.m_bWeek);s2.Format("%d-",pTNT->m_stTSTEP.m_bDay);
				int nHour = pTNT->m_stTSTEP.m_dwStartTime / 3600;
				int nMinute = (pTNT->m_stTSTEP.m_dwStartTime % 3600) / 60;
				s3.Format("%d-",nHour); s4.Format("%d",nMinute);
				pTNT->m_strTitle = s1 + s2 + s3 + s4;

				AddTournament(pTNT);							
				m_pdlgGMTool->m_dlgTournament->AddList(pTNT->m_strTitle, FALSE);				
			}

			(*pPacket)
				>> bCount;
			
			BYTE bEventCount;
			for( i = 0; i < bCount; i++)
			{
				(*pPacket)
					>> wTNTID
					>> bEventCount;

				LPTOURNAMENT pTNT = GetTournament(wTNTID);
				for( j = 0; j < bEventCount; j++)
				{
					TPVP stTPVP;
					stTPVP.WrapPacketOut(pPacket);
					if(pTNT)
						pTNT->m_vTPVP.push_back(stTPVP);
				}
			}

			if(m_pdlgGMTool->m_dlgTournament->m_wTournamentID)
			{
				LPTOURNAMENT pTNT = GetTournament(m_pdlgGMTool->m_dlgTournament->m_wTournamentID);
				m_pdlgGMTool->m_dlgTournament->m_wTournamentID = 0;
			}
            m_pdlgGMTool->m_dlgTournament->UpdateFocus();
		}
		break;
	case TET_SCHEDULEADD:
		{
			WORD wTNTID;

			(*pPacket)
				>> wTNTID;

			if(!wTNTID)
			{
				ShowRetDlg(SW_SHOW,0,_T("The Schedule is NOT changed"));
				return;
			}

 			LPTOURNAMENT pTNT = GetTournament(wTNTID);
			if(!pTNT)
			{	
				pTNT = m_pdlgGMTool->m_dlgTournament->m_pTNT;
				pTNT->m_wID = wTNTID;
				AddTournament(pTNT);
				
				
				CString str,s1,s2,s3,s4;
				s1.Format("%d-",pTNT->m_stTSTEP.m_bWeek);s2.Format("%d-",pTNT->m_stTSTEP.m_bDay);
				int nHour = pTNT->m_stTSTEP.m_dwStartTime / 3600;
				int nMinute = (pTNT->m_stTSTEP.m_dwStartTime % 3600) / 60;
				s3.Format("%d-",nHour); s4.Format("%d",nMinute);
				str = s1 + s2 + s3 + s4;			

				if(str != pTNT->m_strTitle)
				{					
					m_pdlgGMTool->m_dlgTournament->AddList(str);				
					pTNT->m_strTitle = str;					
				}
				m_pdlgGMTool->m_dlgTournament->SetTitle(pTNT->m_strTitle);
				

				m_pdlgGMTool->m_dlgTournament->EnableBtnSTEP(TRUE);
				m_pdlgGMTool->m_dlgTournament->EnableBtnEventList(TRUE);

			}
			else
			{
				pTNT = m_pdlgGMTool->m_dlgTournament->m_pTNT;

				
				CString str,s1,s2,s3,s4;
				s1.Format("%d-",pTNT->m_stTSTEP.m_bWeek);s2.Format("%d-",pTNT->m_stTSTEP.m_bDay);
				int nHour = pTNT->m_stTSTEP.m_dwStartTime / 3600;
				int nMinute = (pTNT->m_stTSTEP.m_dwStartTime % 3600) / 60;
				s3.Format("%d-",nHour); s4.Format("%d",nMinute);
				str = s1 + s2 + s3 + s4;
				
				if(str != pTNT->m_strTitle)
				{					
					m_pdlgGMTool->m_dlgTournament->ChangeList(pTNT->m_strTitle, str);					
					pTNT->m_strTitle = str;
				}
				m_pdlgGMTool->m_dlgTournament->SetTitle(pTNT->m_strTitle);
				
			}

			m_pdlgGMTool->m_dlgTournament->m_wTournamentID = wTNTID;
			m_pdlgGMTool->m_dlgTournament->m_bNewTNT = FALSE;

			SendCT_TOURNAMENTEVENT_REQ(m_pdlgGMTool->m_dlgTournament->m_bSelectWorld, TET_LIST,NULL);
			m_pdlgGMTool->m_dlgTournament->m_pTNT = NULL;
		}
		break;
	case TET_SCHEDULEDEL:
		{
			WORD wTNTID;
			(*pPacket)
				>> wTNTID;

			if(!wTNTID)
			{
				ShowRetDlg(SW_SHOW,0,_T("The Schedule is NOT canceled"));
				return;
			}

			LPTOURNAMENT pTNT = GetTournament(wTNTID);
			if(pTNT)
			{
				m_pdlgGMTool->m_dlgTournament->DelList(pTNT->m_strTitle);
				m_pdlgGMTool->m_dlgTournament->m_pTNT = NULL;
			}

			DelTournament(wTNTID);		
		}
		break;
	case TET_ENTRYADD:
		{
			if(m_pdlgGMTool->m_dlgTournament->m_bNewTNT)
			{
				m_pdlgGMTool->m_dlgTournament->m_bNewTNT = FALSE;
				m_pdlgGMTool->m_dlgTournament->EnableBtnEventList(TRUE);
								
				ShowRetDlg(SW_SHOW,0,_T("The new tournament is added"));
			}
			else
			{
				ShowRetDlg(SW_SHOW,0,_T("The Entry list is changed"));
			}
		}
		break;
	case TET_ENTRYDEL:
		{			
		}
		break;
	case TET_PLAYERADD:
		{			
			TNTCHAR stTNTCHAR;

			(*pPacket)				
				>> stTNTCHAR.m_bEventID
				>> stTNTCHAR.m_dwCharID
				>> stTNTCHAR.m_strName
				>> stTNTCHAR.m_bLevel
				>> stTNTCHAR.m_bClass
				>> stTNTCHAR.m_bCountry;
			
			if(!stTNTCHAR.m_dwCharID)
			{
				ShowRetDlg(SW_SHOW,0,_T("The Player is NOT added"));
			}
			else
			{
				m_pdlgGMTool->m_dlgTournament->AddEntryChar(&stTNTCHAR);
				m_pdlgGMTool->m_dlgTournament->UpdateEntryChar(TRUE,&stTNTCHAR);
				ShowRetDlg(SW_SHOW,0,_T("The Player is added"));
			}
                
		}
		break;
	case TET_PLAYERDEL:
		{			
			BYTE bEventID;
			DWORD dwCharID;
			CString strName;

			(*pPacket)				
				>> bEventID
				>> dwCharID;

			TNTCHAR stTNTCHAR;
			stTNTCHAR.m_bEventID = bEventID;
			stTNTCHAR.m_strName = _T("");

			if(!dwCharID)
			{			
				ShowRetDlg(SW_SHOW,0,_T("The Player is NOT deleted"));				
			}
			else
			{
				m_pdlgGMTool->m_dlgTournament->UpdateEntryChar(FALSE,&stTNTCHAR);
				ShowRetDlg(SW_SHOW,0,_T("The Player is deleted"));				
			}

		}
		break;
	case TET_PLAYEREND:
		{
		}
		break;	
	default:
		break;
	}
}


void CHappyDoc::OnCT_RPSGAMEDATA_ACK(CPacket* pPacket)
{	
	if(!m_pdlgGMTool->m_dlgRPSGame->IsWindowVisible())
		return;

	WORD wCount;
	BYTE bChange;
	BYTE bGroupID;
	CRPSGame*	pRPS = m_pdlgGMTool->m_dlgRPSGame;

	(*pPacket)
		>> bChange
		>> bGroupID
		>> wCount;

	if(!bChange && !pRPS->m_bView)
		return;

	if(bGroupID != pRPS->m_bSelectWorld)
		return;

	pRPS->m_bView = FALSE;

	pRPS->InitType();
	pRPS->m_vRpsList.clear();
	for(WORD i = 0; i < wCount; i++)
	{
		TRPSGameData RPSData;
		(*pPacket)
			>> RPSData.m_bType
			>> RPSData.m_bWinCount
			>> RPSData.m_bProbWin
			>> RPSData.m_bProbDraw
			>> RPSData.m_bProbLose
			>> RPSData.m_wWinKeep
			>> RPSData.m_wWinPeriod;

		pRPS->AddType(RPSData.m_bType);
		pRPS->m_vRpsList.push_back(RPSData);
	}
	pRPS->RefreshList();
}

void CHappyDoc::OnCT_PREVERSIONTABLE_ACK(CPacket* pPacket)
{
	WORD wCount;

	(*pPacket)
		>> wCount;
	
	m_dlgPreVersion->ClearPreVersionTable();

	for(WORD i = 0; i < wCount; i++)
	{
		TPATFILE stFile;

		(*pPacket)
			>> stFile.m_dwVersion
			>> stFile.m_strPath
			>> stFile.m_strName
			>> stFile.m_dwSize;
		stFile.m_bState = 0;

		m_dlgPreVersion->PushPreVersionTable(stFile);
	}	
	m_dlgPreVersion->AddNewFileList(&m_vPatchFiles);

	m_dlgPreVersion->RefreshList();
}

void CHappyDoc::OnCT_CMGIFT_ACK(CPacket* pPacket)
{	
	// View 얻기
	CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if(!pView || !pView->m_dlgGMTool)
		return;
	
	BYTE bRet;
	(*pPacket)
		>> bRet;

	pView->m_dlgGMTool->m_dlgCMGift->PostMessage(WM_CMGIFTRET, (WPARAM)bRet);
}

void CHappyDoc::OnCT_CMGIFTLIST_ACK(CPacket* pPacket) 
{
	// View 얻기
	CHappyView* pView = (CHappyView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView();
	if(!pView || !pView->m_dlgGMTool)
		return;
	
	LPMAPCMGIFT pmapGift = pView->m_dlgGMTool->m_dlgCMGift->GetGiftItemMap();
	if(!pmapGift)
		return;

	pmapGift->clear();

	WORD wCount;
	(*pPacket)
		>> wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		TCMGIFT stGift;
		stGift.m_bUpdate = 0;

		(*pPacket)
			>> stGift.m_wGiftID
			>> stGift.m_bGiftType
			>> stGift.m_dwValue
			>> stGift.m_bCount
			>> stGift.m_bTakeType
			>> stGift.m_bMaxTakeCount
			>> stGift.m_bLevel
			>> stGift.m_bToolOnly
			>> stGift.m_wErrGiftID
			>> stGift.m_strTitle
			>> stGift.m_strMsg;

		pmapGift->insert(MAPCMGIFT::value_type(stGift.m_wGiftID, stGift));
	}

	pView->m_dlgGMTool->m_dlgCMGift->ShowSetting(SW_SHOW);
}

