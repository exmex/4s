#include "StdAfx.h"
#include "GMTool.h"
#include "GMToolDlg.h"
#include "GMToolDoc.h"

//CGMToolDoc theDoc;

CGMToolDoc::CGMToolDoc(void)
{
	m_session.Close();
	m_session.SetOwner(this);
	m_bLoadPosList = FALSE;	
}

CGMToolDoc::~CGMToolDoc(void)
{		
}


void CGMToolDoc::OnClose(int nErrorCode)
{
	AfxMessageBox(MSG_CONNECT_CLOSE);
	
	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
	if(pdlgGMTool)
	{
		pdlgGMTool->GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
		pdlgGMTool->SetEnableMenu(MANAGER_COUNT);
		pdlgGMTool->SetShowChildDlgAll(FALSE);
	}
}

void CGMToolDoc::OnConnect(int nErrorCode)
{
	if(nErrorCode)
	{
		AfxMessageBox(MSG_CONNECT_FAIL); // 서버 접속에 실패했습니다
		return;
	}
	SendCT_OPLOGIN_REQ();
}

void CGMToolDoc::OnReceive(CPacket* pPacket)
{
	WORD wMsgID = pPacket->GetID();

	switch(wMsgID)
	{
	HP_RECEIVE(CT_OPLOGIN_ACK)	
    HP_RECEIVE(CT_GROUPLIST_ACK)
	HP_RECEIVE(CT_SERVICEDATA_ACK)
	HP_RECEIVE(CT_PLATFORM_ACK)
	HP_RECEIVE(CT_MONSPAWNFIND_ACK)
	HP_RECEIVE(CT_CHATBAN_ACK)

	default:
		ATLTRACE2("Invalid Packet : %d \n",wMsgID);break;
	}	
}

void CGMToolDoc::Say(CPacket* pPacket)
{
	m_session.Say(pPacket);
}

void CGMToolDoc::DisconnectServer()
{
	m_session.Close();
	m_session.m_bValid = FALSE;

	ClearData();
	//UpdateAllViews(NULL);
}

void CGMToolDoc::SessionStart(CString strIP,DWORD dwPort)
{
	if(!m_session.Start(strIP, dwPort))
		AfxMessageBox(MSG_CONNECT_FAIL); //서버 접속에 실패했습니다
}

void CGMToolDoc::SetUserData(CString strID, CString strPW)
{
	m_strUserID = strID;
	m_strPasswd = strPW;
}


void CGMToolDoc::SavePos(USERPOS& _UserPos)
{
	LPUSERPOS pUSER = new USERPOS;

	pUSER->ChannelNum = _UserPos.ChannelNum;
    pUSER->MapID = _UserPos.MapID;;

	pUSER->PosX = _UserPos.PosX;
	pUSER->PosY = _UserPos.PosY;
	pUSER->PosZ = _UserPos.PosZ;

	strcpy(pUSER->POSNAME, _UserPos.POSNAME);

	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
	if(!pdlgGMTool)
		return;

	LISTUSERPOS::iterator it = find_if(m_listUSERPOS.begin(), m_listUSERPOS.end(),CPosEqual(pUSER->POSNAME) );
	if( it != m_listUSERPOS.end() )
	{
		(*it)->ChannelNum = pUSER->ChannelNum;
		(*it)->MapID = pUSER->MapID;
		(*it)->PosX = pUSER->PosX;
		(*it)->PosY = pUSER->PosY;
		(*it)->PosZ = pUSER->PosZ;
		
		pdlgGMTool->m_pdlgUserMove->m_pdlgPosList->m_listctrPos.UpdateData();

		delete pUSER;
	}
	else
	{
		pdlgGMTool->m_pdlgUserMove->m_pdlgPosList->InsertItemToListCtr( (int)m_listUSERPOS.size(), pUSER);
		pdlgGMTool->m_pdlgUserMove->m_pdlgPosList->m_listctrPos.UpdateData();
		m_listUSERPOS.push_back(pUSER);
	}
}

void CGMToolDoc::LoadPosListFromFile()
{	
	int index = 0;
	
	FILE* fp = fopen(POSLIST_FILE,"rt");

	ClearPosList();	

	char strTemp[100];
	
	
	if(fp != NULL)
	{
		fscanf(fp,"%s%s%s%s%s%s",strTemp,strTemp,strTemp,strTemp,strTemp,strTemp);
		while(1)
		{
			LPUSERPOS pUSER = new USERPOS();

			if( fscanf(fp,"%s\t%d\t%d\t%f\t%f\t%f\n",pUSER->POSNAME,&pUSER->ChannelNum,&pUSER->MapID,&pUSER->PosX ,&pUSER->PosY,&pUSER->PosZ) == EOF)
			{
				delete pUSER;
				break;
			}

			m_listUSERPOS.push_back(pUSER);
		}

		m_bLoadPosList = TRUE;
		fclose(fp);
	}
}

void CGMToolDoc::ClearPosList()
{
	LISTUSERPOS::iterator it;
	for(it = m_listUSERPOS.begin(); it != m_listUSERPOS.end(); it++)
		delete (*it);
	m_listUSERPOS.clear();	
}


void CGMToolDoc::SavePosListToFile()
{
	if( !m_bLoadPosList )
		return;

	FILE* fp = fopen(POSLIST_FILE,"wt");
	
	if( fp == NULL)
		return;	

	fprintf(fp,"NAME\tChannel\tMapID\tPosX\t\tPosY\t\tPosZ\n");

	USERPOS USER;
	LISTUSERPOS::iterator it;
	for( it = m_listUSERPOS.begin() ; it != m_listUSERPOS.end(); it++)
	{
		USER.ChannelNum = (*it)->ChannelNum;
		USER.MapID = (*it)->MapID;
		USER.PosX = (*it)->PosX;
		USER.PosY = (*it)->PosY;
		USER.PosZ = (*it)->PosZ;
		strcpy(USER.POSNAME, (*it)->POSNAME );

		fprintf(fp,"%s\t%d\t%d\t%f\t%f\t%f\n",USER.POSNAME,USER.ChannelNum,USER.MapID,USER.PosX,USER.PosY,USER.PosZ);
	}

	m_bLoadPosList = FALSE;
	fclose(fp);
}

void CGMToolDoc::ClearData()
{
	ClearGroup();
	SavePosListToFile();
	ClearPosList();	
}

