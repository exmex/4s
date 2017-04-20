#include "stdafx.h"
#include "Resource.h"
#include "TClientWnd.h"
#include "TItemUpDlg.h"
#include "TItemRefineDlg.h"
#include "TItemRepairDlg.h"
#include "TItemGambleDlg.h"
#include "TPrivateShopDlg.h"
#include "TBlockListDlg.h"
#include "TMailListFrame.h"
#include "TMailAskMoneyDlg.h"
#include "TMailRecvDlg.h"
#include "TMailSendDlg.h"
#include "TPetMakeDlg.h"
#include "TPetManageDlg.h"
#include "TMessengerBase.h"
#include "TSoulmateFrame.h"

int CTClientGame::OnCS_RELAYCONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;

	(*pPacket)
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port;

	for( BYTE i=1; i<MAX_CON; i++)
	{
		if(m_pMainWnd->m_session[i].m_bSVR == SVR_RELAY)
			m_pMainWnd->m_session[i].OnClose(0);
	}

	CTClientSession *pSESSION = m_pMainWnd->GetIdleSession();

	pSESSION->m_bSvrID = 0;
	pSESSION->m_bSVR = SVR_RELAY;
	pSESSION->m_bLogicalValid = TRUE;
	m_dwRelayTick = 0;

	pSESSION->Start(
		inet_ntoa(vADDR.sin_addr),
		vADDR.sin_port);

	return TERR_NONE;
}

int CTClientGame::OnCS_RELAYCHARDATA_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket)
		>> bResult;

	if(bResult)
	{
		m_pRelay = (CTClientSession *)pSession;
		m_dwRelayTick = 0;

		VTSTRING vName;
		VTBYTE vBlockOption;
		
		vName.clear();
		vBlockOption.clear();
		
		CTBlockListDlg* pDlg = m_pBlockList;
		if(pDlg)
		{
			for(INT i=0; i<pDlg->GetCount(); i++)
			{
				vName.push_back(pDlg->GetName(i));
				vBlockOption.push_back( pDlg->GetBlockOption(i) );
			}

			m_pRelay->SendCS_RELAYPROTECTEDLIST_REQ(
				vName,
				vBlockOption );

			vName.clear();
			vBlockOption.clear();
		}
	}
	else
		pSession->OnClose(0);

	return TERR_NONE;
}

int CTClientGame::OnCS_CHAT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{ 
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	BYTE bGroup;

	(*pPacket)
		>> bGroup;

	CString strNAME;
	CString strMSG;
	DWORD dwSenderID;

	(*pPacket)
		>> dwSenderID
		>> strNAME
		>> strMSG;
	
	if( bGroup == CHAT_WHISPER && m_vTOPTION.m_bDenyWhisper &&
		dwSenderID != m_pMainChar->m_dwID )
		return TERR_NONE;

	DWORD dwSoundID = 0;
	BYTE bSoundType = MEDIA_TSOUND;
	DWORD dwChatFlag;

	CString strTYPE = CTChatFrame::GetChatTypeString(bGroup);
	DWORD dwColor = CTChatFrame::GetChatTypeColor(bGroup);

	switch( bGroup )
	{
	case CHAT_WHISPER	: 
		{
			dwChatFlag = TCHAT_FLAG_WHISPER;	
			dwSoundID = ID_SND_INFO; 
			bSoundType = MEDIA_TSOUND;

			m_pChatFrame->AddWhisperList( strNAME );

			if( dwSenderID == GetMainChar()->m_dwID )
			{
				CString strRESULT;
				strRESULT = CTChart::Format( TSTR_FMT_WHISPER, strNAME, _T("") );
				strRESULT = pTextLinker->BuildNetString( CString(), strRESULT );
				strMSG = pTextLinker->AppendNetString( strRESULT, strMSG );
				strNAME = GetMainChar()->m_strNAME;
			}
		}
		break;

	case CHAT_OPERATOR	: 
	case CHAT_WORLD		: 
		dwChatFlag = TCHAT_FLAG_WORLD;
		dwSoundID = ID_SND_INFO; 
		bSoundType = MEDIA_TSOUND;
		break;

	case CHAT_NEAR		: dwChatFlag = TCHAT_FLAG_NEAR;    break;
	case CHAT_MAP		: dwChatFlag = TCHAT_FLAG_MAP;     break;
	case CHAT_PARTY		: dwChatFlag = TCHAT_FLAG_PARTY;   break;
	case CHAT_GUILD		: dwChatFlag = TCHAT_FLAG_GUILD;   break;
	case CHAT_TACTICS	: dwChatFlag = TCHAT_FLAG_TACTICS; break;
	
	case CHAT_FORCE		: 
		{
			const VTSQUAD& vSQUAD = m_pTRSCS->m_vTCORPS.m_vTSQUAD;
			for(INT i=0; i<INT(vSQUAD.size()); ++i)
			{
				if( vSQUAD[i]->m_wPartyID == m_pMainChar->m_wCommanderID &&
					vSQUAD[i]->m_dwChiefID == dwSenderID )
				{
					dwColor = TCOLOR_CHAT_COMMANDER;
					dwSoundID = ID_SND_INFO; 
					bSoundType = MEDIA_TSOUND;
					break;
				}
			}

			dwChatFlag = TCHAT_FLAG_FORCE;   
		}
		break;
	case CHAT_SHOW:
		{
			dwChatFlag = TCHAT_FLAG_SHOW;
			dwSoundID = ID_SND_INFO; 
			bSoundType = MEDIA_TSOUND;
		}
		break;
	default				: dwChatFlag = 0;
	}

	if( !dwChatFlag )
		return TERR_NONE;

	if( dwSenderID == m_pMainChar->m_dwID )
		CTChatList::m_bUpdateScroll = TRUE;

	CString strTALKBOX = m_pChatFrame->ChatMSG(
		strTYPE,
		strNAME,
		strMSG,
		dwColor,
		dwChatFlag,
		dwSoundID,
		bSoundType,
		dwSenderID );

	CTChatList::m_bUpdateScroll = FALSE;

	DWORD dwChatTargetID = m_pChatFrame->GetChatTargetID();
	if( bGroup == CHAT_WHISPER &&
		!dwChatTargetID &&
		m_pMainChar->m_dwID != dwSenderID &&
		m_pChatFrame->GetChatMode() != CHAT_WHISPER)
        m_pChatFrame->SetChatTarget(strNAME, dwSenderID);

	CTClientObjBase *pTOBJ = FindPC(dwSenderID);
	if(pTOBJ)
	{
		pTOBJ->ActivateTALK(
			m_pDevice->m_pDevice,
			strTALKBOX,
			dwColor);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MOVECORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwCharID;

	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		m_pTRSCS->FindTSQUAD(wSquadID),
		dwCharID);

	if(pTUNIT)
	{
		(*pPacket)
			>> pTUNIT->m_fSPEED
			>> pTUNIT->m_wMapID
			>> pTUNIT->m_dwBspID
			>> pTUNIT->m_wPosX
			>> pTUNIT->m_wPosZ
			>> pTUNIT->m_wDIR;
		pTUNIT->m_dwTick = 0;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSCMD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wSquadID;
	WORD wMapID;

	(*pPacket)
		>> wSquadID
		>> dwCharID
		>> wMapID;

	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		m_pTRSCS->FindTSQUAD(wSquadID),
		dwCharID);

	if( pTUNIT && (wMapID == 0xFFFF || pTUNIT->m_wMapID == wMapID) )
	{
		(*pPacket)
			>> pTUNIT->m_bTCMD
			>> pTUNIT->m_dwTargetID
			>> pTUNIT->m_bTargetType
			>> pTUNIT->m_wTargetX
			>> pTUNIT->m_wTargetZ;

		if( dwCharID == m_pMainChar->m_dwID )
		{
			ResetTRSCSCMD(
				&m_vTCMDTARGET,
				&m_vTCMDDIR,
				&m_vTCMDARROW,
				pTUNIT->m_bTCMD,
				pTUNIT->m_dwTargetID,
				pTUNIT->m_bTargetType,
				pTUNIT->m_wTargetX,
				pTUNIT->m_wTargetZ);
		}

		ResetRSCS(m_bRSCS);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSHP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwCharID;

	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		m_pTRSCS->FindTSQUAD(wSquadID),
		dwCharID);

	if(pTUNIT)
	{
		(*pPacket)
			>> pTUNIT->m_dwMaxHP
			>> pTUNIT->m_dwHP;

		ResetRSCS(m_bRSCS);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSENEMYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		LPMAPTENEMY pTENEMY = NULL;

		DWORD dwEnemyID;
		BYTE bEnemyType;

		(*pPacket)
			>> dwEnemyID
			>> bEnemyType;

		switch(bEnemyType)
		{
		case OT_RECALL	: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapRECALL; break;
		case OT_SELF	: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapFIXRECALL; break;
		case OT_MON		: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapMON; break;
		case OT_PC		: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapPC; break;
		}

		if(pTENEMY)
		{
			MAPTENEMY::iterator finder = pTENEMY->find(dwEnemyID);

			LPTENEMY pENEMY = NULL;
			BYTE bREPORTER;

			if( finder == pTENEMY->end() )
			{
				FLOAT fSPEED;
				WORD wMapID;
				WORD wPosX;
				WORD wPosZ;
				WORD wDIR;

				(*pPacket)
					>> fSPEED
					>> wMapID
					>> wPosX
					>> wPosZ
					>> wDIR
					>> bREPORTER;

				BOOL HasMyPartyMember = FALSE;

				MAPDWORD mapTREPORTER;
				for( BYTE j=0; j<bREPORTER; j++)
				{
					DWORD dwReporterID;

					(*pPacket)
						>> dwReporterID;

					if( mapTREPORTER.find(dwReporterID) == mapTREPORTER.end() )
						continue;

					if( FindParty( dwReporterID ) || dwReporterID == m_pMainChar->m_dwID )
					{
						mapTREPORTER.insert( MAPDWORD::value_type( dwReporterID, dwReporterID));
						HasMyPartyMember = TRUE;
					}
				}

				if( HasMyPartyMember )
				{
					pENEMY = new TENEMY();

					pENEMY->m_bType = bEnemyType;
					pENEMY->m_dwID = dwEnemyID;
					pENEMY->m_dwTick = 0;
					pENEMY->m_fSPEED = fSPEED;
					pENEMY->m_wMapID = wMapID;
					pENEMY->m_wPosX = wPosX;
					pENEMY->m_wPosZ = wPosZ;
					pENEMY->m_wDIR = wDIR;
					pENEMY->m_mapTREPORTER = mapTREPORTER;

					pTENEMY->insert( MAPTENEMY::value_type( dwEnemyID, pENEMY));
				}
			}
			else
			{
				pENEMY = (*finder).second;

				pENEMY->m_dwTick = 0;
				(*pPacket)
					>> pENEMY->m_fSPEED
					>> pENEMY->m_wMapID
					>> pENEMY->m_wPosX
					>> pENEMY->m_wPosZ
					>> pENEMY->m_wDIR
					>> bREPORTER;

				for( BYTE j=0; j<bREPORTER; j++)
				{
					DWORD dwReporterID;

					(*pPacket)
						>> dwReporterID;

					if( pENEMY->m_mapTREPORTER.find(dwReporterID) == pENEMY->m_mapTREPORTER.end() &&
						(FindParty( dwReporterID ) || dwReporterID == m_pMainChar->m_dwID) )
					{
						pENEMY->m_mapTREPORTER.insert( MAPDWORD::value_type( dwReporterID, dwReporterID));
					}
				}
			}
		}
		else
		{
			FLOAT fSPEED;

			WORD wMapID;
			WORD wPosX;
			WORD wPosZ;
			WORD wDIR;

			BYTE bREPORTER;

			(*pPacket)
				>> fSPEED
				>> wMapID
				>> wPosX
				>> wPosZ
				>> wDIR
				>> bREPORTER;

			for( BYTE j=0; j<bREPORTER; j++)
			{
				DWORD dwReporterID;

				(*pPacket)
					>> dwReporterID;
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MOVECORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;

	(*pPacket)
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType;

	if( !FindParty( dwReporterID ) && dwReporterID != m_pMainChar->m_dwID )
		return TERR_NONE;

	LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
		dwEnemyID,
		bEnemyType);

	if(!pTENEMY)
	{
		LPMAPTENEMY pTMAPENEMY = NULL;

		switch(bEnemyType)
		{
		case OT_RECALL	: pTMAPENEMY = &m_pTRSCS->m_vTCORPS.m_mapRECALL; break;
		case OT_SELF	: pTMAPENEMY = &m_pTRSCS->m_vTCORPS.m_mapFIXRECALL; break;
		case OT_MON		: pTMAPENEMY = &m_pTRSCS->m_vTCORPS.m_mapMON; break;
		case OT_PC		: pTMAPENEMY = &m_pTRSCS->m_vTCORPS.m_mapPC; break;
		}

		if(pTMAPENEMY)
		{
			pTENEMY = new TENEMY();

			pTENEMY->m_bType = bEnemyType;
			pTENEMY->m_dwID = dwEnemyID;
			pTMAPENEMY->insert( MAPTENEMY::value_type( dwEnemyID, pTENEMY));

			if( pTENEMY->m_mapTREPORTER.find(dwReporterID) == pTENEMY->m_mapTREPORTER.end() )
				pTENEMY->m_mapTREPORTER.insert( MAPDWORD::value_type( dwReporterID, dwReporterID));
		}
	}

	if(pTENEMY)
	{
		(*pPacket)
			>> pTENEMY->m_fSPEED
			>> pTENEMY->m_wMapID
			>> pTENEMY->m_wPosX
			>> pTENEMY->m_wPosZ
			>> pTENEMY->m_wDIR;
		pTENEMY->m_dwTick = 0;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDCORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	LPMAPTENEMY pTENEMY = NULL;

	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;
	FLOAT fSPEED;
	WORD wMapID;
	WORD wPosX;
	WORD wPosZ;
	WORD wDIR;

	(*pPacket)
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType
		>> fSPEED
		>> wMapID
		>> wPosX
		>> wPosZ
		>> wDIR;

	if( !FindParty( dwReporterID ) && m_pMainChar->m_dwID != dwReporterID )
		return TERR_NONE;

	m_pTRSCS->AddEnemy(
		dwReporterID,
		dwEnemyID,
		bEnemyType,
		fSPEED,
		wMapID,
		wPosX,
		wPosZ,
		wDIR );

	return TERR_NONE;
}

int CTClientGame::OnCS_DELCORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	LPMAPTENEMY pTENEMY = NULL;

	DWORD dwReporterID;
	DWORD dwEnemyID;
	BYTE bEnemyType;

	(*pPacket)
		>> dwReporterID
		>> dwEnemyID
		>> bEnemyType;

	switch(bEnemyType)
	{
	case OT_RECALL	: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapRECALL; break;
	case OT_SELF	: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapFIXRECALL; break;
	case OT_MON		: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapMON; break;
	case OT_PC		: pTENEMY = &m_pTRSCS->m_vTCORPS.m_mapPC; break;
	}

	if(pTENEMY)
	{
		MAPTENEMY::iterator itTENEMY = pTENEMY->find(dwEnemyID);

		if( itTENEMY != pTENEMY->end() )
		{
			MAPDWORD::iterator finder = (*itTENEMY).second->m_mapTREPORTER.find(dwReporterID);

			if( finder != (*itTENEMY).second->m_mapTREPORTER.end() )
				(*itTENEMY).second->m_mapTREPORTER.erase(finder);

			if((*itTENEMY).second->m_mapTREPORTER.empty())
			{
				delete (*itTENEMY).second;
				pTENEMY->erase(itTENEMY);
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_REPORTENEMYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwCharID;

	if(m_pTRSCS->FindTUNIT( m_pTRSCS->FindTSQUAD(wSquadID), dwCharID))
		RelaySession()->SendCS_CORPSENEMYLIST_REQ( wSquadID, dwCharID, &m_pTRSCS->m_vTCORPS);

	return TERR_NONE;
}
