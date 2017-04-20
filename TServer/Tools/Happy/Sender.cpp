#include "Stdafx.h"
#include "HappyDoc.h"

void CHappyDoc::SendCT_OPLOGIN_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_OPLOGIN_REQ)
		<< m_strUserID
		<< m_strPasswd;

	Say(pMsg);
}
void CHappyDoc::SendCT_SERVICESTAT_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICESTAT_REQ);

	Say(pMsg);
}
void CHappyDoc::SendCT_SERVICECONTROL_REQ(BYTE bStart)
{
	MAPSERVICE::iterator it = m_mapService.find(m_dwSelectedService);
	if(it!=m_mapService.end())
	{
		CPacket * pMsg = new CPacket();
		pMsg->SetID(CT_SERVICECONTROL_REQ)
			<< (*it).second->m_dwID
			<< bStart;

		Say(pMsg);
	}
	else
		AfxMessageBox("Select the service");//¼­ºñ½º¸¦ ¼±ÅÃÇÏ¼¼¿ä
}
void CHappyDoc::SendCT_SERVICECONTROL_REQ(DWORD dwID, BYTE bStart)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICECONTROL_REQ)
		<< dwID
		<< bStart;

	Say(pMsg);
}
void CHappyDoc::SendCT_SERVICEUPLOADSTART_REQ(BYTE bMachine, CString strFile)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOADSTART_REQ)
		<< bMachine
		<< strFile;

	Say(pMsg);
}
void CHappyDoc::SendCT_SERVICEUPLOAD_REQ(BYTE data[], WORD wSize )
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOAD_REQ)
		<< wSize;
	/*
	pMsg->m_pHeader->m_wSize += wSize;
	memcpy(pMsg->m_ptrOffset, data, wSize);
	*/
	for(WORD i=0; i<wSize; i++)
		(*pMsg)
			<< data[i];

	Say(pMsg);
}
void CHappyDoc::SendCT_SERVICEUPLOADEND_REQ(BYTE bCancel, BYTE data[], WORD wSize)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEUPLOADEND_REQ)
		<< bCancel;

	/*
	pMsg->m_pHeader->m_wSize += wSize;
	memcpy(pMsg->m_ptrOffset, data, wSize);
	*/

	(*pMsg)
		<< wSize;

	for(WORD i=0; i<wSize; i++)
		(*pMsg)
			<< data[i];

	Say(pMsg);
}
void CHappyDoc::SendCT_UPDATEPATCH_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_UPDATEPATCH_REQ)
		<< WORD(m_vPatchFiles.size());

	for(WORD i=0; i<(WORD)m_vPatchFiles.size(); i++)
	{
		CString strPath = m_vPatchFiles[i]->m_strPath;
		strPath = strPath.Right(strPath.GetLength()-1);

		(*pMsg) << strPath
			<< m_vPatchFiles[i]->m_strZipFile
			<< m_vPatchFiles[i]->m_dwZipSize;
	}

	Say(pMsg);
}
//void CHappyDoc::SendCT_PREUPDATEPATCH_REQ()
//{
//	CPacket * pMsg = new CPacket();
//	pMsg->SetID(CT_PREUPDATEPATCH_REQ)
//		<< WORD(m_vPatchFiles.size());
//
//	for(WORD i=0; i<(WORD)m_vPatchFiles.size(); i++)
//	{
//		CString strPath = m_vPatchFiles[i]->m_strPath;
//		strPath = strPath.Right(strPath.GetLength()-1);
//
//		(*pMsg) << strPath
//			<< m_vPatchFiles[i]->m_strZipFile
//			<< m_vPatchFiles[i]->m_dwZipSize;
//	}
//
//	Say(pMsg);
//}

//	TPREVERSION Å×ÀÌºí¿¡ µî·ÏµÈ ÆÄÀÏ ¸ñ·Ï °¡Á®¿À±â
void CHappyDoc::SendCT_PREVERSIONTABLE_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PREVERSIONTABLE_REQ);

	Say(pMsg);
}
// TPREVERSION ½Å±Ôµî·Ï
// TPREVERSION ¿¡¼­ TVERSIONÀ¸·Î ÀÌµ¿
// TRPEVERSION Á¦°Å

void CHappyDoc::SendCT_PREVERSIONUPDATE_REQ(WORD wMoveCount, WORD wDelCount, WORD wNewCount, LPVPATFILE pPatch)
{
	if(!wMoveCount && !wDelCount && !wNewCount)
		return;

	if(!pPatch)
		return;

	m_vPatchFiles.clear();
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PREVERSIONUPDATE_REQ)
		<< wMoveCount;

	WORD wCount = 0;
	for(VPATFILE::iterator it = pPatch->begin(); it != pPatch->end(); it++)
	{
		if(it->m_bState == STATE_MOVE)
		{
			(*pMsg) << it->m_dwVersion;
			wCount++;
			if(wCount >= wMoveCount)
				break;
		}
	}

	(*pMsg)	<< wDelCount;
	wCount = 0;
	for(VPATFILE::iterator it = pPatch->begin(); it != pPatch->end(); it++)
	{
		if(it->m_bState == STATE_DEL)
		{
			(*pMsg) << it->m_dwVersion;
			wCount++;
			if(wCount >= wDelCount)
				break;
		}
	}

	(*pMsg)	<< wNewCount;
	wCount = 0;
	for(VPATFILE::iterator it = pPatch->begin(); it != pPatch->end(); it++)
	{
		if(it->m_bState == STATE_NEW)
		{
			(*pMsg)
				<< it->m_strPath
				<< it->m_strName
				<< it->m_dwSize;

			wCount++;
			if(wCount >= wNewCount)
				break;
		}
	}
	Say(pMsg);
}

/////////////////////////////////////////////////////
// Çö½Â·æ CT_ANNOUNCEMENT_REQ
void CHappyDoc::SendCT_ANNOUNCEMENT_REQ(DWORD nID, CString strAnnounce)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_ANNOUNCEMENT_REQ)
		<< nID
        << strAnnounce;

	Say(pMsg);
}
// Çö½Â·æ CT_USERKICKOUT_REQ
void CHappyDoc::SendCT_USERKICKOUT_REQ(CString strUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERKICKOUT_REQ)
		<< strUser;

	Say(pMsg);
}
// Çö½Â·æ CT_USERMOVE_REQ
void CHappyDoc::SendCT_USERMOVE_REQ(queue<CString>* pqStrUser, BYTE bWorld, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERMOVE_REQ)
		<< bWorld
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< (WORD)pqStrUser->size();

	while(!pqStrUser->empty())
	{
		(*pMsg)
			<< pqStrUser->front();
		pqStrUser->pop();
	}

	Say(pMsg);
}
// Çö½Â·æ CT_USERPOSITION_REQ
void CHappyDoc::SendCT_USERPOSITION_REQ(BYTE bWorld, queue<CString>* pqStrUser, CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPOSITION_REQ)
		<< bWorld
		<< strTarget		
		<< (WORD)pqStrUser->size();

	while(!pqStrUser->empty())
	{
		(*pMsg)
			<< pqStrUser->front();
		pqStrUser->pop();
	}

	Say(pMsg);
}
// Çö½Â·æ CT_MONSPAWNFIND_REQ
void CHappyDoc::SendCT_MONSPAWNFIND_REQ(BYTE bGroupID, BYTE bChannel, WORD wMapID, WORD wSpawnID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_MONSPAWNFIND_REQ)
		<< bGroupID
		<< bChannel
		<< wMapID
		<< wSpawnID;

	Say(pMsg);
}
// Çö½Â·æ CT_MONACTION_REQ
void CHappyDoc::SendCT_MONACTION_REQ(BYTE bGroup, BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType,WORD wSpawnID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_MONACTION_REQ)
		<< bGroup
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bAction
		<< dwTriggerID
		<< dwHostID
		<< dwRHID
		<< bRHType
		<< wSpawnID;

	Say(pMsg);
}
void CHappyDoc::SendCT_USERPROTECTED_REQ(CString strUserID, DWORD dwTime)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPROTECTED_REQ)
		<< strUserID
		<< dwTime;

	Say(pMsg);
}
void CHappyDoc::SendCT_CHARMSG_REQ(CString strName, CString strMsg)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_CHARMSG_REQ)
		<< strName
		<< strMsg;

	Say(pMsg);
}

/////////////////////////////////////////////////////

void CHappyDoc::SendCT_RECONNECT_REQ(DWORD _dwID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_RECONNECT_REQ)
		<< _dwID;

	Say(pMsg);
}

void CHappyDoc::SendCT_DISCONNECT_REQ(DWORD _dwID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_DISCONNECT_REQ)
		<< _dwID;

	Say(pMsg);
}


void CHappyDoc::SendCT_SERVICEAUTOSTART_REQ(BYTE _bAutoStart)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEAUTOSTART_REQ)
		<< _bAutoStart;

	Say(pMsg);
}


void CHappyDoc::SendCT_CHATBAN_REQ(CString strName, WORD wMin, CString strReason)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBAN_REQ)
		<< strName
		<< wMin
		<< strReason;

	Say(pMsg);
}

void CHappyDoc::SendCT_SERVICEDATACLEAR_REQ()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEDATACLEAR_REQ);

	Say(pMsg);
}

void CHappyDoc::SendCT_ITEMFIND_REQ(WORD wItemID, CString strName,BYTE bWorldID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_ITEMFIND_REQ)
		<< wItemID
		<< strName
		<< bWorldID;

	Say(pMsg);
}

void CHappyDoc::SendCT_ITEMSTATE_REQ(LPVITEMSTATE pVITEMSTATE,BYTE bWorldID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_ITEMSTATE_REQ)
		<< bWorldID
		<< m_dwManagerID
		<< WORD(pVITEMSTATE->size());

	for(WORD i = 0; i < (WORD)pVITEMSTATE->size(); i++)
	{
		ITEMSTATE ItemState = pVITEMSTATE->at(i);

		(*pMsg)
			<< ItemState.m_wItemID
			<< ItemState.m_bInitState;
	}	

	Say(pMsg);
}

void CHappyDoc::SendCT_CHATBANLIST_REQ()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBANLIST_REQ);

	Say(pMsg);
}

void CHappyDoc::SendCT_CHATBANLISTDEL_REQ(DWORD dwID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBANLISTDEL_REQ)
		<< dwID;

	Say(pMsg);
}

void CHappyDoc::SendCT_CASTLEINFO_REQ(BYTE bWorld)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASTLEINFO_REQ)
		<< bWorld;

	Say(pMsg);
}

void CHappyDoc::SendCT_CASTLEGUILDCHG_REQ(BYTE bWorld,
										  WORD wCastleID,
										  DWORD dwDefGuildID,
										  DWORD dwAtkGuildID,
										  __time64_t tTime)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASTLEGUILDCHG_REQ)
		<< bWorld
		<< wCastleID
		<< dwDefGuildID
		<< dwAtkGuildID
		<< tTime;

	Say(pMsg);
}

void CHappyDoc::SendCT_CASTLEENABLE_REQ(BYTE bWorld,BYTE bStatus,DWORD dwSecond)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASTLEENABLE_REQ)
		<< bWorld
		<< bStatus
		<< dwSecond;

	Say(pMsg);
}

void CHappyDoc::SendCT_EVENTCHANGE_REQ(BYTE bType, LPEVENTINFO pEvent)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTCHANGE_REQ)
		<< bType;
	pEvent->WrapPacketIn(pMsg);

	Say(pMsg);
}

void CHappyDoc::SendCT_EVENTLIST_REQ()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTLIST_REQ);		

	Say(pMsg);
}

void CHappyDoc::SendCT_CASHITEMLIST_REQ()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASHITEMLIST_REQ);

	Say(pMsg);

}

void CHappyDoc::SendCT_EVENTQUARTERUPDATE_REQ(BYTE bGroup, BYTE bType, LPLUCKYEVENT pLucky)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTQUARTERUPDATE_REQ)
		<< bGroup
		<< m_dwManagerID
		<< bType;

	pLucky->WrapPacketIn(pMsg);

	Say(pMsg);
}

void CHappyDoc::SendCT_EVENTQUARTERLIST_REQ(BYTE bGroup, BYTE bDay)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTQUARTERLIST_REQ)
		<< bGroup
		<< m_dwManagerID
		<< bDay;

	Say(pMsg);
}

void CHappyDoc::SendCT_TOURNAMENTEVENT_REQ(BYTE bGroupID, BYTE bType, LPTOURNAMENT pTNT,LPTNTCHAR pTNTCHAR)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_TOURNAMENTEVENT_REQ)
		<< bGroupID
		<< m_dwManagerID
		<< bType;
		

	switch(bType)
	{
	case TET_SCHEDULEADD:
		{
			if(!pTNT)
				return;

			(*pMsg)
				<< pTNT->m_wID;

			pTNT->m_stTSTEP.WrapPacketIn(pMsg);	
		}
		break;
	case TET_SCHEDULEDEL:
		{
			if(!pTNT)
				return;
			
			(*pMsg)
				<< pTNT->m_wID;
		}
		break;
	case TET_ENTRYADD:
		{
			if(!pTNT)
				return;

			(*pMsg)
				<< pTNT->m_wID;

			(*pMsg)
				<< (BYTE)pTNT->m_vTPVP.size();

			for( BYTE i = 0; i < (BYTE)pTNT->m_vTPVP.size(); i++)
				pTNT->m_vTPVP[i].WrapPacketIn(pMsg);
		}
		break;
	case TET_PLAYERADD:
		{
			if(!pTNT)
				return;

			(*pMsg)
				<< pTNT->m_wID;

			(*pMsg)
				<< pTNTCHAR->m_bEventID
				<< pTNTCHAR->m_strName;
		}
		break;
	case TET_PLAYERDEL:
		{
			if(!pTNT)
				return;

			(*pMsg)
				<< pTNT->m_wID;

			(*pMsg)
				<< pTNTCHAR->m_bEventID
				<< pTNTCHAR->m_strName;
		}
		break;
	case TET_PLAYEREND:		
		break;

	default:
		break;
	}
	

	Say(pMsg);
}

void CHappyDoc::SendCT_HELPMESSAGE_REQ(BYTE bGroupID, LPHELPMSG pHelpMsg)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_HELPMESSAGE_REQ)
		<< bGroupID;

	pHelpMsg->WrapPacketIn(pMsg);

	Say(pMsg);
}

void CHappyDoc::SendCT_RPSGAMEDATA_REQ(BYTE bGroupID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_RPSGAMEDATA_REQ)
		<< bGroupID;

	Say(pMsg);
}

void CHappyDoc::SendCT_RPSGAMECHANGE_REQ(BYTE bGroupID, LPVRPSGameData pData)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_RPSGAMECHANGE_REQ)
		<< bGroupID;
	
	WORD wCount = (WORD)pData->size();
	(*pMsg) 
		<< wCount;

	for(WORD i = 0; i < wCount; i++)
	{
		TRPSGameData RPSData = pData->at(i);

		(*pMsg)
			<< RPSData.m_bType
			<< RPSData.m_bWinCount
			<< RPSData.m_bProbWin
			<< RPSData.m_bProbDraw
			<< RPSData.m_bProbLose
			<< RPSData.m_wWinKeep
			<< RPSData.m_wWinPeriod;
	}
	Say(pMsg);
}

void CHappyDoc::SendCT_CMGIFT_REQ(CString strTarget, WORD wGiftID, BYTE bGroup)
{
	if(strTarget.IsEmpty())
		return;

	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFT_REQ)
		<< bGroup
		<< strTarget
		<< wGiftID;

	Say(pMsg);
}

void CHappyDoc::SendCT_CMGIFTLIST_REQ(BYTE bGroup)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFTLIST_REQ)
		<< bGroup;

	Say(pMsg);
}

void CHappyDoc::SendCT_CMGIFTCHARTUPDATE_REQ(LPQCMGIFT pQList, BYTE bGroup)
{
	if(!pQList)
		return;

	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFTCHARTUPDATE_REQ)
		<< bGroup
		<< (WORD)pQList->size();
	
	while(!pQList->empty())
	{
		TCMGIFT tGift = pQList->front();	
		switch (tGift.m_bUpdate)
		{
		case CGU_NONE:
			break;
		case CGU_DEL:
			(*pMsg)
			<< tGift.m_bUpdate
			<< tGift.m_wGiftID;
			break;
		case CGU_ADD:
		case CGU_UPDATE:
			(*pMsg)
			<< tGift.m_bUpdate
			<< tGift.m_wGiftID
			<< tGift.m_bGiftType
			<< tGift.m_dwValue
			<< tGift.m_bCount
			<< tGift.m_bTakeType
			<< tGift.m_bMaxTakeCount
			<< tGift.m_bLevel
			<< tGift.m_bToolOnly
			<< tGift.m_wErrGiftID
			<< tGift.m_strTitle
			<< tGift.m_strMsg;
			break;
		}
		pQList->pop_front();
	}
	Say(pMsg);
}

/////////////////////////////////////////////////////