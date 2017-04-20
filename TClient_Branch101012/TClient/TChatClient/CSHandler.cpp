#include "stdafx.h"
#include "TChatClient.h"

#include "TChatClientDoc.h"
#include "TChatClientView.h"
#include "MainFrm.h"

#include "TChannelDlg.h"
#include "TGroupDlg.h"
#include "TCharDlg.h"


int CTChatClientView::OnCS_LOGIN_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;

	DWORD dwPCRoomID;
	DWORD dwKickID;

	BYTE bPCRoom;
	BYTE bCount;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> m_dwUserID
		>> dwKickID
		>> m_dwKEY
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port
		>> bCount
		>> bPCRoom
		>> dwPCRoomID;

	if( bResult != LR_SUCCESS )
	{
		pTSocket->m_bSVR = SVR_NONE;
		pTSocket->m_bSvrID = 0;
		pTSocket->End();
		OnClose( pTSocket, 0);

		switch(bResult)
		{
		case LR_INVALIDPASSWD	: return IDS_CONNECT_INVALID_PW;
		case LR_VERSION			: return IDS_CONNECT_INVALID_VERSION;
		case LR_NOUSER			: return IDS_CONNECT_INVALID_USER;
		case LR_IPBLOCK			: return IDS_CONNECT_IPBLOCK;
		case LR_DUPLICATE		: return IDS_CONNECT_DUPLICATE;
		default					: return IDS_CONNECT_INTERNAL;
		}
	}

	CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
	CString strMSG;

	strMSG.Format( IDS_FMT_LOGIN, m_strID);
	pFRAME->SetPaneText(strMSG);
	pTSocket->SendCS_GROUPLIST_REQ();

	return 0;
}

int CTChatClientView::OnCS_GROUPLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	CTGroupDlg dlg;
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		LPTGROUP pTGROUP = new TGROUP();

		(*pPacket)
			>> pTGROUP->m_strNAME
			>> pTGROUP->m_bGroupID
			>> pTGROUP->m_bType
			>> pTGROUP->m_bStatus
			>> pTGROUP->m_bCount;

		dlg.m_vTGROUP.push_back(pTGROUP);
	}

	if( dlg.DoModal() == IDCANCEL || !dlg.m_pTGROUP )
	{
		pTSocket->m_bSVR = SVR_NONE;
		pTSocket->m_bSvrID = 0;
		pTSocket->End();
		OnClose( pTSocket, 0);
	}
	else
	{
		pTSocket->SendCS_CHANNELLIST_REQ(dlg.m_pTGROUP->m_bGroupID);
		m_bGroupID = dlg.m_pTGROUP->m_bGroupID;
	}

	return 0;
}

int CTChatClientView::OnCS_CHANNELLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	CTChannelDlg dlg;
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		LPTCHANNEL pTCHANNEL = new TCHANNEL();

		(*pPacket)
			>> pTCHANNEL->m_strNAME
			>> pTCHANNEL->m_bChannel
			>> pTCHANNEL->m_bStatus;

		dlg.m_vTCHANNEL.push_back(pTCHANNEL);
	}

	if( dlg.DoModal() == IDCANCEL || !dlg.m_pTCHANNEL )
	{
		pTSocket->m_bSVR = SVR_NONE;
		pTSocket->m_bSvrID = 0;
		pTSocket->End();
		OnClose( pTSocket, 0);
	}
	else
	{
		pTSocket->SendCS_CHARLIST_REQ(m_bGroupID);
		m_bChannel = dlg.m_pTCHANNEL->m_bChannel;
	}

	return 0;
}

int CTChatClientView::OnCS_CHARLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	CTCharDlg dlg;
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		LPTCHARACTER pTCHAR = new TCHARACTER();
		DWORD dwRegionID;
		BYTE bItemCount;

		(*pPacket)
			>> pTCHAR->m_dwID
			>> pTCHAR->m_strNAME
			>> pTCHAR->m_bSlotID
			>> pTCHAR->m_bLevel
			>> pTCHAR->m_bClass
			>> pTCHAR->m_bRace
			>> pTCHAR->m_bContry
			>> pTCHAR->m_bSex
			>> pTCHAR->m_bHair
			>> pTCHAR->m_bFace
			>> pTCHAR->m_bBody
			>> pTCHAR->m_bPants
			>> pTCHAR->m_bHand
			>> pTCHAR->m_bFoot
			>> dwRegionID
			>> bItemCount;

		for( BYTE j=0; j<bItemCount; j++)
		{
			WORD wItemID;
			BYTE bItemID;
			BYTE bLevel;
			BYTE bGradeEffect;

			(*pPacket)
				>> bItemID
				>> wItemID
				>> bLevel
				>> bGradeEffect;

		}

		dlg.m_vTCHAR.push_back(pTCHAR);
	}

	if( dlg.DoModal() == IDCANCEL || !dlg.m_pTCHAR )
	{
		pTSocket->m_bSVR = SVR_NONE;
		pTSocket->m_bSvrID = 0;
		pTSocket->End();
		OnClose( pTSocket, 0);
	}
	else
	{
		pTSocket->SendCS_START_REQ( m_bGroupID, m_bChannel, dlg.m_pTCHAR->m_dwID);
		m_strNAME = dlg.m_pTCHAR->m_strNAME;
		m_dwCharID = dlg.m_pTCHAR->m_dwID;

		GetDocument()->SetTitle(m_strNAME);
	}

	return 0;
}

int CTChatClientView::OnCS_START_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;

	BYTE bServerID;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port
		>> bServerID;

	switch(bResult)
	{
	case SR_NOSERVER	: return IDS_CONNECT_NOSERVER;
	case SR_NOGROUP		: return IDS_CONNECT_NOSERVER;
	case SR_SUCCESS		:
		{
			CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
			CTChatSocket *pSOCKET = GetIdleSession();

			pSOCKET->m_bSvrID = bServerID;
			pSOCKET->m_bSVR = SVR_MAP;

			pSOCKET->Start(
				inet_ntoa(vADDR.sin_addr),
				vADDR.sin_port);

			pFRAME->SetPaneText(IDS_CONNECT_MAP);
			OnClose( pTSocket, 0);
			pTSocket->End();
   		}

		break;

	default				: return IDS_CONNECT_INTERNAL;
	}

	return 0;
}

int CTChatClientView::OnCS_ADDCONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		CTChatSocket *pSOCKET = GetIdleSession();

		(*pPacket)
			>> pSOCKET->m_target.sin_addr.S_un.S_addr
			>> pSOCKET->m_target.sin_port
			>> pSOCKET->m_bSvrID;
		pSOCKET->m_bSVR = SVR_MAP;

		pSOCKET->Start(
			inet_ntoa(pSOCKET->m_target.sin_addr),
			pSOCKET->m_target.sin_port);
	}

	return 0;
}

int CTChatClientView::OnCS_INVALIDCHAR_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	return IDS_CONNECT_INVALIDCHAR;
}

int CTChatClientView::OnCS_SHUTDOWN_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	pTSocket->SendCS_DISCONNECT_REQ();
	return 0;
}

int CTChatClientView::OnCS_CONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bResult;

	m_pSOCKET = NULL;
	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case CN_ALREADYEXIST	: return IDS_CONNECT_DUPLICATE;
	case CN_INVALIDVER		: return IDS_CONNECT_INVALID_VERSION;
	case CN_NOCHANNEL		: return IDS_CONNECT_NOCHANNEL;
	case CN_NOCHAR			: return IDS_CONNECT_NOCHAR;
	case CN_SUCCESS			:
		{
			CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
			CString strINFO;
			BYTE bCount;

			m_cListCtrl.EnableWindow(TRUE);
			m_cEditCtrl.EnableWindow(TRUE);
			m_mapTSELLITEM.clear();
			m_pSOCKET = pTSocket;
			m_bLogin = TRUE;
			m_bShop = FALSE;
			m_bOpen = FALSE;

			strINFO.Format( IDS_FMT_START, m_strNAME);
			pFRAME->SetPaneText(strINFO);

			(*pPacket)
				>> bCount;

			for( BYTE i=0; i<bCount; i++)
			{
				BYTE bServerID;

				(*pPacket)
					>> bServerID;

				SendCS_CONREADY_REQ(bServerID);
				m_bPingTestView = TRUE;
			}
		}

		break;

	default					: return IDS_CONNECT_INTERNAL;
	}

	return 0;
}

int CTChatClientView::OnCS_CHAT_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	LPTCHATMSG pTMSG = new TCHATMSG();
	DWORD dwSenderID;

	(*pPacket)
		>> pTMSG->m_bType
		>> dwSenderID
		>> pTMSG->m_strNAME
		>> pTMSG->m_strMSG;

	switch(pTMSG->m_bType)
	{
	case CHAT_WHISPER	: pTMSG->m_dwFlags = TCHAT_FLAG_WHISPER; break;
	case CHAT_OPERATOR	: pTMSG->m_dwFlags = TCHAT_FLAG_WORLD; break;
	case CHAT_WORLD		: pTMSG->m_dwFlags = TCHAT_FLAG_WORLD; break;
	case CHAT_NEAR		: pTMSG->m_dwFlags = TCHAT_FLAG_NEAR; break;
	case CHAT_MAP		: pTMSG->m_dwFlags = TCHAT_FLAG_MAP; break;
	case CHAT_PARTY		: pTMSG->m_dwFlags = TCHAT_FLAG_PARTY; break;
	case CHAT_GUILD		: pTMSG->m_dwFlags = TCHAT_FLAG_GUILD; break;
	case CHAT_FORCE		: pTMSG->m_dwFlags = TCHAT_FLAG_FORCE; break;
	default				: pTMSG->m_dwFlags = 0; break;
	}

	if(pTMSG->m_dwFlags)
	{
		pTMSG->m_strMSG = MakeNetToLinkText( pTMSG->m_strMSG );
		m_vTCHATMSG.push_back(pTMSG);

		while(INT(m_vTCHATMSG.size()) > TCHAT_MSG_COUNT)
		{
			delete m_vTCHATMSG.front();
			m_vTCHATMSG.erase(m_vTCHATMSG.begin());
		}

		ResetTCHATMSG();
	}
	else
		delete pTMSG;

	return 0;
}

int CTChatClientView::OnCS_CHARINFO_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bFirstConnet;
	BYTE bLuckyID;
	BYTE bClassID;
	BYTE bRaceID;
	BYTE bSex;
	BYTE bHair;
	BYTE bFace;
	BYTE bBody;
	BYTE bPants;
	BYTE bHand;
	BYTE bFoot;
	BYTE bHelmetHide;
	BYTE bLevel;
	WORD wPartyID;
	DWORD dwGuildID;
	BYTE bGuildDuty;
	BYTE bGuildPeer;
	CString strTACTICS;
	CString strGUILD;
	DWORD dwTacticsID;
	DWORD dwGuildMark;
	DWORD dwGuildColor;
	DWORD dwRune;
	DWORD dwLuna;
	DWORD dwCron;
	DWORD dwPrevEXP;
	DWORD dwNextEXP;
	DWORD dwEXP;
	DWORD dwMaxHP;
	DWORD dwHP;
	DWORD dwMaxMP;
	DWORD dwMP;
	DWORD dwChiefID;
	WORD wCommanderID;
	DWORD dwRegionID;
	WORD wMapID;
	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wDIR;
	WORD vTSKILLPOINT[TPOINT_COUNT];
	BYTE bCount;

	(*pPacket)
		>> m_dwCharID
		>> m_strNAME
		>> bFirstConnet
		>> bClassID
		>> bRaceID
		>> m_bContryID
		>> bSex
		>> bHair
		>> bFace
		>> bBody
		>> bPants
		>> bHand
		>> bFoot
		>> bHelmetHide
		>> bLevel
		>> wPartyID
		>> dwGuildID
		>> dwGuildMark
		>> dwGuildColor
		>> bGuildDuty
		>> bGuildPeer
		>> strGUILD
		>> dwTacticsID
		>> strTACTICS
		>> dwRune
		>> dwLuna
		>> dwCron
		>> dwPrevEXP
		>> dwNextEXP
		>> dwEXP
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP
		>> dwChiefID
		>> wCommanderID
		>> dwRegionID
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wDIR
		>> vTSKILLPOINT[TPOINT_MINE]
		>> bLuckyID
		>> vTSKILLPOINT[TPOINT_TAB1]
		>> vTSKILLPOINT[TPOINT_TAB2]
		>> vTSKILLPOINT[TPOINT_TAB3]
		>> vTSKILLPOINT[TPOINT_TAB4]
		>> bCount;
	ClearTINVEN();

	for( BYTE i=0; i<bCount; i++)
	{
		CTChatInven *pTINVEN = new CTChatInven();
		BYTE bItemCount;
		__time64_t dEndTime;

		(*pPacket)
			>> pTINVEN->m_bInvenID
			>> pTINVEN->m_wItemID
			>> dEndTime
			>> bItemCount;

		pTINVEN->m_pTITEM = CTChatItem::FindTItem(pTINVEN->m_wItemID);

		for( BYTE j=0; j<bItemCount; j++)
		{
			CTChatItem *pTITEM = new CTChatItem();
			BYTE bMagicCount;

			DWORD m_dwDuraMax;
			DWORD m_dwDuraCurrent;
			BYTE m_bRefineMax;
			BYTE m_bRefineCurrent;
			BYTE m_bCanGamble;
			__time64_t m_dEndTime;
			BYTE m_bGradeEffect;

			(*pPacket)
				>> pTITEM->m_bItemID
				>> pTITEM->m_wItemID
				>> pTITEM->m_bGrade
				>> pTITEM->m_bCount
				>> m_dwDuraMax
				>> m_dwDuraCurrent
				>> m_bRefineMax
				>> m_bRefineCurrent
				>> m_bCanGamble
				>> m_dEndTime
				>> m_bGradeEffect
				>> bMagicCount;
			
			for( BYTE k=0; k<bMagicCount; k++)
			{
				DWORD dwTime;
				WORD wValue;
				BYTE bMagic;

				(*pPacket)
					>> bMagic
					>> wValue
					>> dwTime;
			}

			pTITEM->m_pTITEM = CTChatItem::FindTItem(pTITEM->m_wItemID);
			pTINVEN->m_mapTITEM.insert( MAPTITEM::value_type( pTITEM->m_bItemID, pTITEM));
		}

		m_mapTINVEN.insert( MAPTINVEN::value_type( pTINVEN->m_bInvenID, pTINVEN));
	}

	return 0;
}

int CTChatClientView::OnCS_STOREOPEN_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	CString strNAME;
	DWORD dwCharID;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> dwCharID
		>> strNAME;

	if( m_dwCharID != dwCharID )
		return 0;
	LPTCHATMSG pTMSG = new TCHATMSG();

	if( bResult == STORE_SUCCESS )
	{
		m_bShop = TRUE;
		m_bOpen = TRUE;

		pTMSG->m_strMSG.LoadString(IDS_MSG_STOREOPEN_SUCC);

		CString strPANEMSG;
		strPANEMSG.Format(IDS_SHOPPANE_MSG, m_mapTSELLITEM.size());

		CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
		pFRAME->SetShopText(strPANEMSG);
	}
	else
	{
		m_bShop = FALSE;
		m_bOpen = FALSE;

		pTMSG->m_strMSG.LoadString(IDS_MSG_STOREOPEN_FAIL);
	}

	pTMSG->m_strNAME.LoadString(IDS_MSG_STORENAME);
	pTMSG->m_bType = CHAT_NEAR;
	
	m_vTCHATMSG.push_back(pTMSG);
	while(INT(m_vTCHATMSG.size()) > TCHAT_MSG_COUNT)
	{
		delete m_vTCHATMSG.front();
		m_vTCHATMSG.erase(m_vTCHATMSG.begin());
	}

	ResetTCHATMSG();
	return 0;
}

int CTChatClientView::OnCS_STORECLOSE_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;

	(*pPacket) 
		>> bResult
		>> dwCharID;

	if(m_dwCharID != dwCharID)
		return 0;

	LPTCHATMSG pTMSG = new TCHATMSG();

	if( bResult == STORE_SUCCESS )
	{
		m_bShop = FALSE;
		m_bOpen = FALSE;

		m_mapTSELLITEM.clear();

		CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
		pFRAME->SetShopText("");

		pTMSG->m_strMSG.LoadString(IDS_MSG_STORECLOSE_SUCC);
	}
	else
	{
		if( m_bShop )
			m_bOpen = TRUE;

		pTMSG->m_strMSG.LoadString(IDS_MSG_STORECLOSE_FAIL);
	}

	pTMSG->m_strNAME.LoadString(IDS_MSG_STORENAME);
	pTMSG->m_bType = CHAT_NEAR;
	
	m_vTCHATMSG.push_back(pTMSG);
	while(INT(m_vTCHATMSG.size()) > TCHAT_MSG_COUNT)
	{
		delete m_vTCHATMSG.front();
		m_vTCHATMSG.erase(m_vTCHATMSG.begin());
	}

	ResetTCHATMSG();
	return 0;
}

int CTChatClientView::OnCS_STOREITEMLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	DWORD dwCharID;
	CString strName;
	BYTE bCount;
	
	(*pPacket) 
		>> dwCharID
		>> strName
		>> bCount;

	if(m_dwCharID != dwCharID || m_mapTSELLITEM.size() != bCount)
		return 0;

	MAPTSELLITEM::iterator itr, end;
	itr = m_mapTSELLITEM.begin();
	end = m_mapTSELLITEM.end();
	for(; itr!=end; ++itr)
	{
		DWORD dwMoney;
		BYTE bItemCnt;
		WORD wItemID;
		BYTE bGrade;
		BYTE bMagicCount;
		DWORD dwDuraMax;
		DWORD dwDuraCurrent;
		BYTE bRefineMax;
		BYTE bRefineCurrent;
		BYTE bCanGamble;
		BYTE bGradeEffect;

		(*pPacket) 
			>> itr->first->m_bNetID
			>> dwMoney
			>> dwMoney
			>> dwMoney
			>> bItemCnt
			>> wItemID
			>> bGrade
			>> dwDuraMax
			>> dwDuraCurrent
			>> bRefineMax
			>> bRefineCurrent
			>> bCanGamble
			>> bGradeEffect
			>> bMagicCount;

		for(BYTE j=0; j<bMagicCount; ++j)
		{
			BYTE bMagic;
			WORD wValue;
			DWORD dwTime;

			(*pPacket) 
				>> bMagic
				>> wValue
				>> dwTime;
		}
	}

	return 0;
}

int CTChatClientView::OnCS_STOREITEMSELL_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bItem;
	BYTE bCount;

	(*pPacket) 
		>> bItem
		>> bCount;

	MAPTSELLITEM::iterator itr, end;
	itr = m_mapTSELLITEM.begin();
	end = m_mapTSELLITEM.end();
	for(; itr!=end; ++itr)
	{
		CTChatItem* pITEM = itr->first;
		if( pITEM->m_bNetID == bItem )
		{
			pITEM->m_bCount -= bCount;
			
			LPTCHATMSG pTMSG = new TCHATMSG();

			pTMSG->m_strNAME.LoadString(IDS_MSG_STORENAME);
			pTMSG->m_bType = CHAT_NEAR;
			pTMSG->m_strMSG.Format(IDS_MSG_STORESELL, 
				pITEM->m_pTITEM->m_strNAME, bCount);
			
			m_vTCHATMSG.push_back(pTMSG);
			while(INT(m_vTCHATMSG.size()) > TCHAT_MSG_COUNT)
			{
				delete m_vTCHATMSG.front();
				m_vTCHATMSG.erase(m_vTCHATMSG.begin());
			}

			ResetTCHATMSG();

			if( pITEM->m_bCount == 0 )
				m_mapTSELLITEM.erase(itr);

			UINT nItemCnt = (UINT) m_mapTSELLITEM.size();

			CString strPANEMSG;
			strPANEMSG.Format(IDS_SHOPPANE_MSG, m_mapTSELLITEM.size());

			CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
			pFRAME->SetShopText(strPANEMSG);

			if( nItemCnt == 0 )
			{
				ShowWindow(SW_SHOW);
				StartBeep(8000);
			}

			break;
		}
	}
	return 0;
}

int CTChatClientView::OnCS_RELAYCONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;

	(*pPacket)
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port;

	for( BYTE i=1; i<MAX_CON; i++)
	{
		if(m_vTSESSION[i].m_bSVR == SVR_RELAY)
			OnClose(&m_vTSESSION[i], 0);
	}

	CTChatSocket *pSOCKET = GetIdleSession();

	pSOCKET->m_bSvrID = 0;
	pSOCKET->m_bSVR = SVR_RELAY;

	pSOCKET->Start(
		inet_ntoa(vADDR.sin_addr),
		vADDR.sin_port);

	return 0;
}

int CTChatClientView::OnCS_RELAYCHARDATA_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket)
		>> bResult;

	if(bResult)
		m_pRelay = pTSocket;
	else
		OnClose(pTSocket, 0);

	return 0;
}

int CTChatClientView::OnCS_ENTER_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	DWORD dwCharID;
	CString strNAME;
	DWORD dwGuildID;
	DWORD dwGuildMark;
	DWORD dwGuildColor;
	CString strGUILD;
	BYTE bGuildPeer;
	DWORD dwTacticsID;
	CString strTACTICS;
	BYTE bPrivateShop;
	CString strPrivateShop;
	DWORD dwRideMonID;
	BYTE bClassID;
	BYTE bRaceID;
	BYTE bContryID;

	(*pPacket)
		>> dwCharID
		>> strNAME
		>> dwGuildID
		>> dwGuildMark
		>> dwGuildColor
		>> strGUILD
		>> bGuildPeer
		>> dwTacticsID
		>> strTACTICS
		>> bPrivateShop
		>> strPrivateShop
		>> dwRideMonID
		>> bClassID
		>> bRaceID
		>> bContryID;

	MAPCHAR::iterator it = m_mapTCHAR.find(dwCharID);
	if(it == m_mapTCHAR.end())
		m_mapTCHAR.insert(MAPCHAR::value_type(dwCharID, bContryID));

	return 0;
}

int CTChatClientView::OnCS_LEAVE_ACK( CTChatSocket *pTSocket, CPacket *pPacket)
{
	DWORD dwCharID;
	(*pPacket)
		>> dwCharID;

	m_mapTCHAR.erase(dwCharID);
	return 0;
}

int CTChatClientView::OnCT_SERVICEMONITOR_REQ(CTChatSocket *pTSocket, CPacket *pPacket)
{
	DWORD dwTick;
	DWORD dwSession;
	DWORD dwUser;
	(*pPacket)
		>> dwTick
		>> dwSession
		>> dwUser;

	DWORD dwRecvTick = GetTickCount();

	if(m_pdlgPingTest)
		m_pdlgPingTest->SetPing(dwRecvTick - dwTick);

	return 0;
}
