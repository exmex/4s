#pragma once

#include "stdafx.h"
#include "GMToolDoc.h"

// CGMToolDlg 를 참조하기 위해
#include "GMTool.h"
#include "GMToolDlg.h"

void CGMToolDoc::ClearGroup()
{
	MAPGROUP::iterator itG;
	for(itG=m_mapGroup.begin(); itG!=m_mapGroup.end(); itG++)
		delete (*itG).second;
	m_mapGroup.clear();
}

void CGMToolDoc::OnCT_OPLOGIN_ACK(CPacket* pPacket)
{
	BYTE bRet;
	BYTE bAuthority;
	DWORD dwManagerID;

	(*pPacket) 
		>> bRet
		>> bAuthority
		>> dwManagerID;

	if(bRet != 0)
	{
		AfxMessageBox(MSG_CONNECT_FAIL);
		m_session.End();		

		return;
	}

	m_bAuthority = bAuthority;
	m_dwManagerID = dwManagerID;

	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서	
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
	if(pdlgGMTool)
	{
        pdlgGMTool->SetEnableMenu(m_bAuthority);
		LoadPosListFromFile();
		pdlgGMTool->m_bDisconnect = FALSE;
		pdlgGMTool->m_pdlgIPChange->ShowWindow(SW_HIDE);
		pdlgGMTool->GetDlgItem(IDC_BTN_CHGIP)->EnableWindow(FALSE);
		pdlgGMTool->GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);

		// 메인 윈도우 타이틀 바꾸기	
		CString strTitle = _T(" - GMTool ") + pdlgGMTool->m_strIP;
		CWnd* pWnd = AfxGetMainWnd();	
		pWnd->SetWindowText(strTitle);
	}		
}

void CGMToolDoc::OnCT_AUTHORITY_ACK(CPacket* pPacket)
{
	AfxMessageBox(MSG_AUTHORITY_NO,MB_OK); //권한이 아닙니다.
}

void CGMToolDoc::OnCT_GROUPLIST_ACK(CPacket * pPacket)
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

	//SendCT_SERVICESTAT_REQ();
}

void CGMToolDoc::OnCT_SERVICEDATA_ACK(CPacket * pPacket)
{
	DWORD dwID;
	DWORD dwSession;
	DWORD dwCurUser;
	DWORD dwMaxUser;
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
		>> nStopTime;

}

void CGMToolDoc::OnCT_PLATFORM_ACK(CPacket * pPacket)
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

}



void CGMToolDoc::OnCT_MONSPAWNFIND_ACK(CPacket * pPacket)
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

		CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서	
		CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
		if(pdlgGMTool && pdlgGMTool->m_pdlgMonsterSpawn )		
			pdlgGMTool->m_pdlgMonsterSpawn->AddItemData(dwMonID, dwHostID, bStat, fPosX, fPosY, fPosZ);		
	}
}

void CGMToolDoc::OnCT_CHATBAN_ACK(CPacket* pPacket)
{
	BYTE bRet;
	(*pPacket)
		>> bRet;

	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서	
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
	if(pdlgGMTool && pdlgGMTool->m_pdlgChatBan)
	{
		pdlgGMTool->m_pdlgChatBan->ShowDlgRet(SW_SHOW,bRet);
		if(bRet)
			pdlgGMTool->m_pdlgChatBan->Init();
	}			
}