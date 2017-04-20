// CSHandler.cpp : C/S Protocol Handler 함수의 구현입니다.

#include "stdafx.h"
#include "TLoginSvr.h"
#include "TLoginSvrModule.h"

DWORD CTLoginSvrModule::OnCT_SERVICEMONITOR_ACK( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet )
{
	DWORD dwSESSION;
	DWORD dwTUSER;
	DWORD dwTACTIVEUSER;
	DWORD dwTick;

	packet >> dwTick;

	EnterCriticalSection(&m_csLI);

	dwSESSION = DWORD(m_mapTSESSION.size());
	dwTUSER = DWORD(m_mapTUSER.size());
	dwTACTIVEUSER = DWORD(m_mapACTIVEUSER.size());

	static DWORD tick = GetTickCount();

	DWORD dwCurrent = GetTickCount();
	if(dwCurrent - tick > 5000)
	{
		MAPTUSER::iterator it;
		for(it=m_mapTSESSION.begin(); it!=m_mapTSESSION.end(); it++)
		{
			if((*it).second->m_bSessionType == SESSION_CLIENT &&
				(*it).second->m_dwAcceptTick &&
				dwCurrent - (*it).second->m_dwAcceptTick > 10000)
			{
				CloseSession((*it).second);
			}

			if((*it).second->m_bSessionType == SESSION_CLIENT &&
				(*it).second->m_dwSendTick &&
				dwCurrent - (*it).second->m_dwSendTick > 30000)
			{
				(*it).second->SendCS_HOTSEND_ACK();
			}
		}
		tick = dwCurrent;
	}

	LeaveCriticalSection(&m_csLI);

	pUser->SendCT_SERVICEMONITOR_REQ(
		dwTick,
		dwSESSION,
		dwTUSER,
		dwTACTIVEUSER);

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCT_SERVICEDATACLEAR_ACK(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	EnterCriticalSection(&m_csLI);
	
	m_mapACTIVEUSER.clear();

	MAPDWORD::iterator itA;
	MAPTUSER::iterator itU;
	for( itU = m_mapTUSER.begin(); itU != m_mapTUSER.end(); itU++)
	{
		itA = m_mapACTIVEUSER.find( (*itU).second->m_dwID );
		if( itA == m_mapACTIVEUSER.end() )
			m_mapACTIVEUSER.insert( MAPDWORD::value_type( (*itU).second->m_dwID, (*itU).second->m_dwID ) );
	}	

	LeaveCriticalSection(&m_csLI);

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCT_CTRLSVR_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCT_EVENTUPDATE_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	BYTE bEventID;
	WORD wValue;
	EVENTINFO stEVENT;

	packet
		>> bEventID
		>> wValue;

	stEVENT.WrapPacketOut(&packet);
	stEVENT.m_wValue = wValue;

	if(bEventID > EVENT_COUNT)
		return EC_NOERROR;

	EnterCriticalSection(&m_csLI);

	if(bEventID > EVENT_COUNT)
	{
		LeaveCriticalSection(&m_csLI);
		return EC_NOERROR;
	}

	MAPEVENTINFO::iterator itE = m_mapEVENT.find(stEVENT.m_dwIndex);
	if( itE != m_mapEVENT.end() )
		m_mapEVENT.erase(itE);
	if(wValue)
		m_mapEVENT.insert(MAPEVENTINFO::value_type(stEVENT.m_dwIndex,stEVENT));

	LeaveCriticalSection(&m_csLI);	

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCT_EVENTMSG_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	BYTE bEventID;
	BYTE bEventMsgType;
	CString strMsg;

	packet
		>> bEventID
		>> bEventMsgType
		>> strMsg;


	return EC_NOERROR;
}

/*
 *	OnCS_LOGIN_REQ
 */
DWORD CTLoginSvrModule::OnCS_LOGIN_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	WORD wVersion;
	BYTE bChanneling = 0;
	INT64 dlCheck;

	//	Check the Version
	packet >> wVersion;
	if( wVersion != TVERSION )
	{
		pUser->SendCS_LOGIN_ACK(LR_VERSION, 0, 0, 0, 0, 0, 0, 0, 0, 0);

		return EC_SESSION_INVALIDCHAR;
	}

	packet	>> pUser->m_strUserID
			>> pUser->m_strPasswd
			>> dlCheck;

	if(m_hExecFile != INVALID_HANDLE_VALUE &&
		dlCheck != m_dlCheckFile)
	{
		LogExecCheck(pUser->m_dwID, 3, pUser->m_strUserID);
		return EC_SESSION_INVALIDCHAR;
	}

	if(m_bNation == NATION_JAPAN)
		packet >> bChanneling;

	//	Check the ID/Password Max Size
	if(pUser->m_strUserID.GetLength() > MAX_NAME ||
		pUser->m_strPasswd.GetLength() > MAX_NAME)
	{
			pUser->SendCS_LOGIN_ACK(LR_INTERNAL, 0, 0, 0, 0, 0, 0, 0, 0, 0);

			return EC_SESSION_INVALIDCHAR;
	}

	BYTE bIPCheck = 0;
	//
	//	Check the IP Block
	//
	//	- Work : Parmater LR_IPBLOCK change
	//
	DEFINE_QUERY( pDB, CSPCheckIP)

	lstrcpy( query->m_szIPAddr, inet_ntoa(pUser->m_addr.sin_addr));

	if(query->Call())
	{
		bIPCheck = query->m_nRET;
		if(bIPCheck == LR_BLOCK)
		{
			pUser->SendCS_LOGIN_ACK(bIPCheck, 0, 0, 0, 0, 0, 0, 0, 0, 0);


#ifdef DEF_UDPLOG
			m_pUdpSocket->LogLogin(pUser, LR_IPBLOCK);
#endif

			return EC_SESSION_INVALIDCHAR;
		}
	}
	UNDEFINE_QUERY()
	
	pUser->m_dlCheckKey = TRand(0xFFFFFFFF) | (INT64(TRand(0xFFFFFFFF)) << 32);

	//	Check the ID/Password
	if(m_bNation == NATION_JAPAN)
	{
		DEFINE_QUERY( pDB, CSPLoginJP)

		lstrcpy( query->m_szUserID, LPCSTR(pUser->m_strUserID));
		lstrcpy( query->m_szPasswd, LPCSTR(pUser->m_strPasswd));
		lstrcpy( query->m_szLoginIP, inet_ntoa(pUser->m_addr.sin_addr));
		query->m_bChanneling = bChanneling;
		query->m_bIPCheck = bIPCheck;

		if(!query->Call())
			query->m_nRET = LR_INTERNAL;

	#ifdef DEF_UDPLOG
		m_pUdpSocket->LogLogin(pUser, query->m_nRET);
	#endif

		pUser->SendCS_LOGIN_ACK(
			query->m_nRET,
			query->m_dwID,
			query->m_dwCharID,
			query->m_dwKEY,
			query->m_szIPAddr ?
			inet_addr(query->m_szIPAddr) : 0,
			query->m_wPort,
			query->m_bCreateCnt,
			query->m_bInPcBang,
			query->m_dwPremium,
			CTime::GetCurrentTime().GetTime());

		switch(query->m_nRET)
		{
		case LR_DUPLICATE	:
			{
				SMART_LOCKCS(&m_csLI)

				MAPTUSER::iterator finder = m_mapTUSER.find(query->m_dwID);

				pUser->m_dwID = query->m_dwID;
				pUser->m_bLogout = TRUE;

				if( finder != m_mapTUSER.end() )
				{
					(*finder).second->m_bLogout = FALSE;

					CloseSession((*finder).second);
				}
				else if(query->m_dwCharID)
				{
					pUser->m_bLogout = FALSE;
				}
			}

			return EC_SESSION_INVALIDCHAR;

		case LR_NEEDAGREEMENT:
			pUser->m_bAgreement = FALSE;
		case LR_SUCCESS		:
			{
				SMART_LOCKCS(&m_csLI)

				pUser->m_dwID	 = query->m_dwID;
				pUser->m_bLogout = TRUE;
				pUser->m_bCreateCnt = query->m_bCreateCnt;
				pUser->m_dwAcceptTick = 0;

				m_mapTUSER.insert( MAPTUSER::value_type( pUser->m_dwID, pUser));

				MAPDWORD::iterator finder = m_mapACTIVEUSER.find(pUser->m_dwID);
				if( finder == m_mapACTIVEUSER.end() )
					m_mapACTIVEUSER.insert(MAPDWORD::value_type(pUser->m_dwID, pUser->m_dwID));
			}

			break;

		default				: return EC_SESSION_INVALIDCHAR;
		}
		UNDEFINE_QUERY()
	}
	else
	{
		DEFINE_QUERY( pDB, CSPLogin)

		lstrcpy( query->m_szUserID, LPCSTR(pUser->m_strUserID));
		lstrcpy( query->m_szPasswd, LPCSTR(pUser->m_strPasswd));
		lstrcpy( query->m_szLoginIP, inet_ntoa(pUser->m_addr.sin_addr));
		query->m_bIPCheck = bIPCheck;

		if(!query->Call())
			query->m_nRET = LR_INTERNAL;

	#ifdef DEF_UDPLOG
		m_pUdpSocket->LogLogin(pUser, query->m_nRET);
	#endif

		pUser->SendCS_LOGIN_ACK(
			query->m_nRET,
			query->m_dwID,
			query->m_dwCharID,
			query->m_dwKEY,
			query->m_szIPAddr ?
			inet_addr(query->m_szIPAddr) : 0,
			query->m_wPort,
			query->m_bCreateCnt,
			query->m_bInPcBang,
			query->m_dwPremium,
			CTime::GetCurrentTime().GetTime());

		switch(query->m_nRET)
		{
		case LR_DUPLICATE	:
			{
				SMART_LOCKCS(&m_csLI)

				MAPTUSER::iterator finder = m_mapTUSER.find(query->m_dwID);

				pUser->m_dwID = query->m_dwID;
				pUser->m_bLogout = TRUE;

				if( finder != m_mapTUSER.end() )
				{
					(*finder).second->m_bLogout = FALSE;

					CloseSession((*finder).second);
				}
				else if(query->m_dwCharID)
				{
					pUser->m_bLogout = FALSE;
				}
			}

			return EC_SESSION_INVALIDCHAR;

		case LR_NEEDAGREEMENT:
			pUser->m_bAgreement = FALSE;
		case LR_SUCCESS		:
			{
				SMART_LOCKCS(&m_csLI)

				pUser->m_dwID	 = query->m_dwID;
				pUser->m_bLogout = TRUE;
				pUser->m_bCreateCnt = query->m_bCreateCnt;
				pUser->m_dwAcceptTick = 0;
	
				if(m_hExecFile != INVALID_HANDLE_VALUE)
					pUser->m_qCheckPoint.push(0);

				m_mapTUSER.insert( MAPTUSER::value_type( pUser->m_dwID, pUser));

				MAPDWORD::iterator finder = m_mapACTIVEUSER.find(pUser->m_dwID);
				if( finder == m_mapACTIVEUSER.end() )
					m_mapACTIVEUSER.insert(MAPDWORD::value_type(pUser->m_dwID, pUser->m_dwID));
			}

			break;

		default				: return EC_SESSION_INVALIDCHAR;
		}
		UNDEFINE_QUERY()
	}

	return EC_NOERROR;
}

// 현승룡 CS_TESTLOGIN_REQ
DWORD CTLoginSvrModule::OnCS_TESTLOGIN_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{	
	DEFINE_QUERY( pDB, CSPTestLogin)
	
	EnterCriticalSection(&m_csLI);
	if(!query->Call())
		query->m_nRET = LR_INTERNAL;	
	LeaveCriticalSection(&m_csLI);
	
	pUser->m_strUserID = query->m_szUserID;
	pUser->m_strPasswd = query->m_szPasswd;

	ATLTRACE(_T("nRet:%d dwUserID:%d, UserID:%s, Passwd=%s\n"), query->m_nRET, query->m_dwID, pUser->m_strUserID, pUser->m_strPasswd);

	// Login 할 수 있는 ID가 없을 경우
	if( query->m_nRET == 1 )
	{
		pUser->SendCS_LOGIN_ACK(10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		return EC_SESSION_INVALIDCHAR;
	}	

	pUser->SendCS_LOGIN_ACK(
		query->m_nRET,
		query->m_dwID,
		query->m_dwCharID,
		query->m_dwKEY,
		query->m_szIPAddr ?
		inet_addr(query->m_szIPAddr) : 0,
		query->m_wPort,
		0, 0, 0, 0);	

	switch(query->m_nRET)
	{	
	case LR_SUCCESS		:
		{
			SMART_LOCKCS(&m_csLI)

			pUser->m_dwID = query->m_dwID;
			pUser->m_bLogout = TRUE;

			m_mapTUSER.insert( MAPTUSER::value_type( pUser->m_dwID, pUser));
			
			MAPDWORD::iterator finder = m_mapACTIVEUSER.find(pUser->m_dwID);
			if( finder == m_mapACTIVEUSER.end() )
                m_mapACTIVEUSER.insert(MAPDWORD::value_type(pUser->m_dwID, pUser->m_dwID));
		}
		break;

	default				: return EC_SESSION_INVALIDCHAR;
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

// 현승룡 CS_TESTVERSION_REQ
DWORD CTLoginSvrModule::OnCS_TESTVERSION_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	pUser->SendCS_TESTVERSION_ACK(TVERSION);
	return EC_NOERROR;
}


DWORD CTLoginSvrModule::OnCS_GROUPLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	if(!pUser->m_bAgreement)
		return EC_SESSION_INVALIDCHAR;

	CPacket *pMSG = new CPacket();
/*
	INT64 dlCheckFile;
	packet >> dlCheckFile;

	if(dlCheckFile != (pUser->m_dlCheckKey ^ m_dlCheckFile))
		return EC_SESSION_INVALIDCHAR;
*/
	WORD wSIZE = pMSG->GetSize();
	BYTE bCount = 0;

	pMSG->SetID(CS_GROUPLIST_ACK)
		<< bCount
		<< GetCheckFilePoint(pUser);

	DEFINE_QUERY( pDB, CTBLGroupList)
	query->m_dwUserID = pUser->m_dwID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			(*pMSG)
				<< CString(query->m_szNAME)
				<< query->m_bGroupID
				<< query->m_bType;

			BYTE bStatus = query->m_bStatus == TSVR_STATUS_SLEEP ? TSTATUS_SLEEP : query->m_dwCount > query->m_wFull ? TSTATUS_FULL : query->m_dwCount > query->m_wBusy ? TSTATUS_BUSY : TSTATUS_NORMAL;
			MAPTGROUP::iterator it = pGROUP->find(query->m_bGroupID);
			if( it != pGROUP->end() &&
				bStatus != TSVR_STATUS_SLEEP &&
				bStatus != TSTATUS_FULL &&
				query->m_dwMaxUser > 0 &&
				query->m_bCount == 0)
			{
				if(GetCurrentUser(query->m_bGroupID) >= query->m_dwMaxUser)
					bStatus = TSTATUS_FULL;
			}

			(*pMSG)
				<< bStatus
				<< query->m_bCount;

			bCount++;
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	memcpy( pMSG->GetBuffer() + wSIZE, &bCount, sizeof(bCount));
	pUser->Say(pMSG);

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCS_CHANNELLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	if(!pUser->m_bAgreement)
		return EC_SESSION_INVALIDCHAR;

	CPacket *pMSG = new CPacket();

	WORD wSIZE = pMSG->GetSize();
	BYTE bCount = 0;

	pMSG->SetID(CS_CHANNELLIST_ACK)
		<< bCount
		<< GetCheckFilePoint(pUser);

	DEFINE_QUERY( pDB, CTBLChannel)
	packet >> query->m_bGroupID;

	if(query->Open())
	{
		while(query->Fetch())
		{
			(*pMSG)
				<< CString(query->m_szNAME)
				<< query->m_bChannel
				<< BYTE(query->m_bStatus == TSVR_STATUS_SLEEP ? TSTATUS_SLEEP : query->m_dwCount > query->m_wFull ? TSTATUS_FULL : query->m_dwCount > query->m_wBusy ? TSTATUS_BUSY : TSTATUS_NORMAL);

			bCount++;
		}

		query->Close();
	}
	UNDEFINE_QUERY()

	memcpy( pMSG->GetBuffer() + wSIZE, &bCount, sizeof(bCount));
	pUser->Say(pMSG);

	return EC_NOERROR;
}

/*
 *	OnCS_CHARLIST_REQ
 *
 */
#ifdef DEF_UDPLOG

	DWORD CTLoginSvrModule::OnCS_CHARLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
	{
		if(!pUser->m_bAgreement)
			return EC_SESSION_INVALIDCHAR;

		BYTE bCount;

		packet >> pUser->m_bGroupID;

		bCount = 0;


		//	Delete the Char List Memory
		for(DWORD i=0; i < pUser->m_vCHAR.size(); i++)
			delete pUser->m_vCHAR[i];

		pUser->m_vCHAR.clear();
        

       //	Find Group
		LPTGROUP pGroup = FindGroup(pGROUP, pUser->m_bGroupID);

		if(pGroup)
		{
			DEFINE_QUERY( &pGroup->m_db, CTBLChar)

				query->m_dwUserID = pUser->m_dwID;

				if(query->Open())
				{
					while(query->Fetch())
					{
						LPTCHARACTER pChar = new TCHARACTER();

						pChar->m_bStartAct	= query->m_bStartAct;
						pChar->m_bSlot		= query->m_bSlot;

						pChar->m_bRace		= query->m_bRace;
						pChar->m_bCountry	= query->m_bCountry;
						pChar->m_bSex		= query->m_bSex;
						
						pChar->m_strName	= query->m_szNAME;
						pChar->m_dwCharID	= query->m_dwCharID;
						pChar->m_bLevel		= query->m_bLevel;

						pChar->m_bBody		= query->m_bBody;
						pChar->m_bClass		= query->m_bClass;
						pChar->m_bFace		= query->m_bFace;
						pChar->m_bFoot		= query->m_bFoot;
						pChar->m_bHair		= query->m_bHair;
						pChar->m_bHand		= query->m_bHand;
						pChar->m_bPants		= query->m_bPants;

						pChar->m_dwRegion	= query->m_dwRegion;
						pChar->m_strGuildName = NAME_NULL;
						pChar->m_dwFame = 0;
						pChar->m_dwFameColor = 0;

						pUser->m_vCHAR.push_back(pChar);
					}

					query->Close();
				}

			UNDEFINE_QUERY()
		}

		for(BYTE i=0; i< pUser->m_vCHAR.size(); i++)
		{
			DEFINE_QUERY(&pGroup->m_db, CTBLItem)

				query->m_bStorageType = STORAGE_INVEN;
				query->m_dwStorageID = INVEN_EQUIP;
				query->m_bOwnerType = TOWNER_CHAR;
				query->m_dwOwnerID = pUser->m_vCHAR[i]->m_dwCharID;

				if(query->Open())
				{
					while(query->Fetch())
					{
						LPTITEM pItem = new TITEM();

						pItem->m_bItemID	= query->m_bItemID;
						pItem->m_wItemID	= query->m_wItemID;
						pItem->m_bLevel		= query->m_bLevel;
						pItem->m_bGradeEffect = query->m_bGradeEffect;
						pItem->m_wColor = WORD(query->m_dwColor);
						pItem->m_bRegGuild = BYTE(query->m_dwRegGuild);

						pUser->m_vCHAR[i]->m_vTItem.push_back(pItem);
					}
					query->Close();
				}

			UNDEFINE_QUERY()

			DEFINE_QUERY(&pGroup->m_db, CSPGetGuildInfo)
			query->m_dwCharID = pUser->m_vCHAR[i]->m_dwCharID;
			if(query->Call())
			{
				pUser->m_vCHAR[i]->m_strGuildName = query->m_szName;
				pUser->m_vCHAR[i]->m_dwFame = query->m_dwFame;
				pUser->m_vCHAR[i]->m_dwFameColor = query->m_dwFameColor;
			}
			UNDEFINE_QUERY()
		}

		CPacket *pMSG = new CPacket();
		pMSG->SetID(CS_CHARLIST_ACK)
			<< GetCheckFilePoint(pUser)
			<< BYTE( pUser->m_vCHAR.size() );

		for(size_t i=0; i< pUser->m_vCHAR.size(); i++)
		{
			(*pMSG)
				<< pUser->m_vCHAR[i]->m_dwCharID
				<< pUser->m_vCHAR[i]->m_strName
				<< pUser->m_vCHAR[i]->m_bStartAct
				<< pUser->m_vCHAR[i]->m_bSlot
				<< pUser->m_vCHAR[i]->m_bLevel
				<< pUser->m_vCHAR[i]->m_bClass
				<< pUser->m_vCHAR[i]->m_bRace
				<< pUser->m_vCHAR[i]->m_bCountry
				<< pUser->m_vCHAR[i]->m_bSex
				<< pUser->m_vCHAR[i]->m_bHair
				<< pUser->m_vCHAR[i]->m_bFace
				<< pUser->m_vCHAR[i]->m_bBody
				<< pUser->m_vCHAR[i]->m_bPants
				<< pUser->m_vCHAR[i]->m_bHand
				<< pUser->m_vCHAR[i]->m_bFoot
				<< pUser->m_vCHAR[i]->m_dwRegion
				<< pUser->m_vCHAR[i]->m_dwFame
				<< pUser->m_vCHAR[i]->m_dwFameColor
				<< BYTE(pUser->m_vCHAR[i]->m_vTItem.size());

			for(DWORD j=0; j< pUser->m_vCHAR[i]->m_vTItem.size(); j++)
			{
				(*pMSG)
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_bItemID
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_wItemID
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_bLevel
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_bGradeEffect
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_wColor
					<< pUser->m_vCHAR[i]->m_vTItem[j]->m_bRegGuild;

				delete pUser->m_vCHAR[i]->m_vTItem[j];
			}
			pUser->m_vCHAR[i]->m_vTItem.clear();
		}

		pUser->Say(pMSG);

		return EC_NOERROR;
	}

#else

	DWORD CTLoginSvrModule::OnCS_CHARLIST_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
	{
		if(!pUser->m_bAgreement)
			return EC_SESSION_INVALIDCHAR;

		BYTE bCount;

		packet >> pUser->m_bGroupID;

		bCount = 0;

		LPTGROUP pGroup = FindGroup(pGROUP, pUser->m_bGroupID);

		VTCHAR vCHAR;
		vCHAR.clear();

		if(pGroup)
		{
			DEFINE_QUERY( &pGroup->m_db, CTBLChar)

				query->m_dwUserID = pUser->m_dwID;

				if(query->Open())
				{
					while(query->Fetch())
					{
						LPTCHARACTER pChar = new TCHARACTER();

						pChar->m_bStartAct	= query->m_bStartAct;
						pChar->m_bSlot		= query->m_bSlot;

						pChar->m_bRace		= query->m_bRace;
						pChar->m_bCountry	= query->m_bCountry;
						pChar->m_bSex		= query->m_bSex;
						
						pChar->m_strName	= query->m_szNAME;
						pChar->m_dwCharID	= query->m_dwCharID;
						pChar->m_bLevel		= query->m_bLevel;

						pChar->m_bBody		= query->m_bBody;
						pChar->m_bClass		= query->m_bClass;
						pChar->m_bFace		= query->m_bFace;
						pChar->m_bFoot		= query->m_bFoot;
						pChar->m_bHair		= query->m_bHair;
						pChar->m_bHand		= query->m_bHand;
						pChar->m_bPants		= query->m_bPants;

						pChar->m_dwRegion	= query->m_dwRegion;
						pChar->m_strGuildName = NAME_NULL;
						pChar->m_dwFame = 0;
						pChar->m_dwFameColor = 0;

						vCHAR.push_back(pChar);
					}

					query->Close();
				}

			UNDEFINE_QUERY()
		}

		for(BYTE i=0; i<vCHAR.size(); i++)
		{
			DEFINE_QUERY(&pGroup->m_db, CTBLItem)

				query->m_bStorageType = STORAGE_INVEN;
				query->m_dwStorageID = INVEN_EQUIP;
				query->m_bOwnerType = TOWNER_CHAR;
				query->m_dwOwnerID = pUser->m_vCHAR[i]->m_dwCharID;

				if(query->Open())
				{
					while(query->Fetch())
					{
						LPTITEM pItem = new TITEM();

						pItem->m_bItemID	= query->m_bItemID;
						pItem->m_wItemID	= query->m_wItemID;
						pItem->m_bLevel		= query->m_bLevel;
						pItem->m_bGradeEffect = query->m_bGradeEffect;
						pItem->m_wColor = WORD(query->m_dwColor);
						pItem->m_bRegGuild = BYTE(query->m_dwRegGuild);

						vCHAR[i]->m_vTItem.push_back(pItem);
					}
					query->Close();
				}

			UNDEFINE_QUERY()

			DEFINE_QUERY(&pGroup->m_db, CSPGetGuildInfo)
			query->m_dwCharID = pUser->m_vCHAR[i]->m_dwCharID;
			if(query->Call())
			{
				vCHAR[i]->m_strGuildName = query->m_szName;
				vCHAR[i]->m_dwFame = query->m_dwFame;
				vCHAR[i]->m_dwFameColor = query->m_dwFameColor;
			}
			UNDEFINE_QUERY()
		}

		CPacket *pMSG = new CPacket();
		pMSG->SetID(CS_CHARLIST_ACK)
			<< GetCheckFilePoint(pUser)
			<< BYTE(vCHAR.size());

		for(i=0; i<vCHAR.size(); i++)
		{
			(*pMSG)
				<< vCHAR[i]->m_dwCharID
				<< vCHAR[i]->m_strName
				<< vCHAR[i]->m_bStartAct
				<< vCHAR[i]->m_bSlot
				<< vCHAR[i]->m_bLevel
				<< vCHAR[i]->m_bClass
				<< vCHAR[i]->m_bRace
				<< vCHAR[i]->m_bCountry
				<< vCHAR[i]->m_bSex
				<< vCHAR[i]->m_bHair
				<< vCHAR[i]->m_bFace
				<< vCHAR[i]->m_bBody
				<< vCHAR[i]->m_bPants
				<< vCHAR[i]->m_bHand
				<< vCHAR[i]->m_bFoot
				<< vCHAR[i]->m_dwRegion
				<< vCHAR[i]->m_dwFame
				<< vCHAR[i]->m_dwFameColor
				<< BYTE(vCHAR[i]->m_vTItem.size());

			for(DWORD j=0; j<vCHAR[i]->m_vTItem.size(); j++)
			{
				(*pMSG)
					<< vCHAR[i]->m_vTItem[j]->m_bItemID
					<< vCHAR[i]->m_vTItem[j]->m_wItemID
					<< vCHAR[i]->m_vTItem[j]->m_bLevel
					<< vCHAR[i]->m_vTItem[j]->m_bGradeEffect
					<< vCHAR[i]->m_vTItem[j]->m_wColor
					<< vCHAR[i]->m_vTItem[j]->m_bRegGuild;

				delete vCHAR[i]->m_vTItem[j];
			}

			delete vCHAR[i];
		}

		pUser->Say(pMSG);

		return EC_NOERROR;
	}

#endif //	DEF_UDPLOG


/*
 *	OnCS_CREATECHAR_REQ
 */
DWORD CTLoginSvrModule::OnCS_CREATECHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	if(!pUser->m_bAgreement)
		return EC_SESSION_INVALIDCHAR;

	CString strNAME;

	BYTE bGroupID;
	BYTE bSlotID;
	BYTE bClass;
	BYTE bRace;
	BYTE bCountry;
	BYTE bSex;
	BYTE bHair;
	BYTE bFace;
	BYTE bBody;
	BYTE bPants;
	BYTE bHand;
	BYTE bFoot;

	packet
		>> bGroupID
		>> strNAME
		>> bSlotID
		>> bClass
		>> bRace
		>> bCountry
		>> bSex
		>> bHair
		>> bFace
		>> bBody
		>> bPants
		>> bHand
		>> bFoot;

	if(bGroupID != pUser->m_bGroupID)
		return EC_SESSION_INVALIDCHAR;

	LPTGROUP pGroup = FindGroup(pGROUP, bGroupID);

	//strNAME.TrimRight();
	//strNAME.TrimLeft();

	if(pGroup)
	{
		// 이름 길이 제한 한글8자
		if(strNAME.GetLength() > 16 || strNAME.GetLength() < 1)
		{
			pUser->SendCS_CREATECHAR_ACK(
				CR_OVERCHAR, 
				0, strNAME,
				bSlotID,
				bClass,
				bRace,
				bCountry,
				bSex,
				bHair,
				bFace,
				bBody,
				bPants,
				bHand,
				bFoot,
				pUser->m_bCreateCnt);

			return EC_NOERROR;
		}

		// a-z A-Z 0-9 문자만 허용 (한문, 스페이스 금지)
		if(!CheckCharName(strNAME))
		{
			pUser->SendCS_CREATECHAR_ACK(
				CR_PROTECTED,
				0, strNAME,
				bSlotID,
				bClass,
				bRace,
				bCountry,
				bSex,
				bHair,
				bFace,
				bBody,
				bPants,
				bHand,
				bFoot,
				pUser->m_bCreateCnt);

			return EC_NOERROR;
		}

		DEFINE_QUERY( &pGroup->m_db, CSPCreateChar)

		lstrcpy( query->m_szNAME, LPCSTR(strNAME));
		query->m_bGroup = pGroup->m_bID;
		query->m_dwUserID	= pUser->m_dwID;
		query->m_dwCharID	= 0;

		query->m_bSlot		= bSlotID;

		query->m_bClass		= bClass;
		query->m_bRace		= bRace;
		query->m_bCountry	= bCountry;

		query->m_bSex		= bSex;
		query->m_bHair		= bHair;
		query->m_bFace		= bFace;
		query->m_bBody		= bBody;
		query->m_bPants		= bPants;
		query->m_bHand		= bHand;
		query->m_bFoot		= bFoot;

		if(!query->Call())
			query->m_nRET = CR_INTERNAL;

#ifdef DEF_UDPLOG
		
		if(!query->m_nRET)
		{
			LPTCHARACTER pChar = new TCHARACTER();

			pChar->m_bSlot		= query->m_bSlot;
			pChar->m_bRace		= query->m_bRace;
			pChar->m_bCountry	= query->m_bCountry;
			pChar->m_bSex		= bSex;				
			pChar->m_strName	= query->m_szNAME;
			pChar->m_dwCharID	= query->m_dwCharID;
			pChar->m_bLevel		= query->m_bCountry == TCONTRY_PEACE ? 1 : CHOICE_COUNTRY_LEVEL;
			pChar->m_bBody		= query->m_bBody;
			pChar->m_bClass		= query->m_bClass;
			pChar->m_bFace		= query->m_bFace;
			pChar->m_bFoot		= query->m_bFoot;
			pChar->m_bHair		= query->m_bHair;
			pChar->m_bHand		= query->m_bHand;
			pChar->m_bPants		= query->m_bPants;
			pChar->m_dwRegion	= 0;

			pUser->InsertCharacter(pChar);

			m_pUdpSocket->LogCharCreate(pUser, query->m_nRET, bGroupID, pChar);
		}

#endif

		if(!query->m_nRET)
		{
			pUser->m_bCreateCnt = query->m_bCreateCnt;
			if(query->m_bCreateCnt == 1)
			{
				MAPDWORD::iterator itCU = m_mapCurrentUser.find(bGroupID);
				if(itCU != m_mapCurrentUser.end())
					(*itCU).second++;
			}
		}

		pUser->SendCS_CREATECHAR_ACK(
			query->m_nRET,
			query->m_dwCharID,
			query->m_szNAME,
			query->m_bSlot,
			query->m_bClass,
			query->m_bRace,
			query->m_bCountry,
			query->m_bSex,
			query->m_bHair,
			query->m_bFace,
			query->m_bBody,
			query->m_bPants,
			query->m_bHand,
			query->m_bFoot,
			query->m_bCreateCnt);

		UNDEFINE_QUERY()
	}
	else
	{
		pUser->SendCS_CREATECHAR_ACK(
			CR_NOGROUP,
			0, strNAME,
			bSlotID,
			bClass,
			bRace,
			bCountry,
			bSex,
			bHair,
			bFace,
			bBody,
			bPants,
			bHand,
			bFoot,
			pUser->m_bCreateCnt);
	}

	return EC_NOERROR;
}

/*
 *	OnCS_DELCHAR_REQ
 */
DWORD CTLoginSvrModule::OnCS_DELCHAR_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	if(!pUser->m_dwID)
		return EC_NOERROR;

	CString strPasswd;

	DWORD dwCharID;
	BYTE bGroupID;

	packet
		>> bGroupID
		>> strPasswd
		>> dwCharID;

	if(bGroupID != pUser->m_bGroupID)
		return EC_SESSION_INVALIDCHAR;

	LPTGROUP pGroup = FindGroup(pGROUP, bGroupID);

	if(strPasswd.GetLength() > MAX_NAME)
	{
#ifdef DEF_UDPLOG
		LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
		m_pUdpSocket->LogCharDelete(pUser, DR_INVALIDPASSWD, bGroupID, pChar);
#endif

		pUser->SendCS_DELCHAR_ACK( DR_INVALIDPASSWD, dwCharID );

		return EC_NOERROR;
	}

	if(pGroup)
	{
		DEFINE_QUERY( pDB, CSPCheckPasswd)

		lstrcpy( query->m_szPasswd, LPCSTR(strPasswd));
		query->m_dwID = pUser->m_dwID;

		if(!query->Call())
			query->m_nRET = DR_INTERNAL;

		if(query->m_nRET)
		{
			pUser->SendCS_DELCHAR_ACK( query->m_nRET, dwCharID );

			return EC_NOERROR;
		}

		UNDEFINE_QUERY()

		BYTE bRet=0;
		BYTE bCreateCnt = 0;

		DEFINE_QUERY( &pGroup->m_db, CSPDeleteChar)

		query->m_bGroup = pGroup->m_bID;
		query->m_dwUserID = pUser->m_dwID;
		query->m_dwCharID = dwCharID;
		query->Call();
		bCreateCnt = query->m_bCreateCnt;
		bRet = query->m_nRET;

		UNDEFINE_QUERY()

#ifdef DEF_UDPLOG
		LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
		m_pUdpSocket->LogCharDelete(pUser, bRet, bGroupID, pChar);

		pUser->DeleteCharacter(dwCharID);
#endif

		if(bRet)
		{
			pUser->SendCS_DELCHAR_ACK( DR_GUILD, dwCharID );

			return EC_NOERROR;
		}

		pUser->SendCS_DELCHAR_ACK( DR_SUCCESS, dwCharID );
		if(!bCreateCnt)
		{
			MAPDWORD::iterator itCU = m_mapCurrentUser.find(bGroupID);
			if(itCU != m_mapCurrentUser.end() && (*itCU).second)
				(*itCU).second--;
		}
	}
	else
	{

#ifdef DEF_UDPLOG
		LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
		m_pUdpSocket->LogCharDelete(pUser, DR_NOGROUP, bGroupID, pChar);
#endif

		pUser->SendCS_DELCHAR_ACK( DR_NOGROUP, dwCharID );
	}

	return EC_NOERROR;
}

/*
 *	OnCS_START_REQ
 */
DWORD CTLoginSvrModule::OnCS_START_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	if(!pUser->m_bAgreement)
		return EC_SESSION_INVALIDCHAR;

	DWORD dwCharID;

	BYTE bServerID;
	BYTE bGroupID;
	BYTE bChannel;

	packet
		>> bGroupID
		>> bChannel
		>> dwCharID;

	if(bGroupID != pUser->m_bGroupID ||
		!pUser->m_qCheckPoint.empty())
	{
		if(!pUser->m_qCheckPoint.empty())
			LogExecCheck(pUser->m_dwID, 4, pUser->m_strUserID);

		return EC_SESSION_INVALIDCHAR;
	}

	LPTGROUP pGroup = FindGroup(pGROUP, bGroupID);
	if(!pGroup)
	{

#ifdef DEF_UDPLOG
		LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
		m_pUdpSocket->LogGameStart(pUser, SR_NOGROUP, bGroupID, pChar);
#endif

		pUser->SendCS_START_ACK(SR_NOGROUP, 0, 0, 0);

		return EC_NOERROR;
	}

	//	Find Server ID
	DEFINE_QUERY( &pGroup->m_db, CSPFindServerID)

		query->m_dwCharID = dwCharID;
		query->m_bChannel = bChannel;

		if(!query->Call())
			query->m_nRET = SR_INTERNAL;

		if(query->m_nRET)
		{
			pUser->SendCS_START_ACK(query->m_nRET,0, 0, 0);

#ifdef DEF_UDPLOG
			LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
			m_pUdpSocket->LogGameStart(pUser, query->m_nRET, bGroupID, pChar);
#endif

			return EC_NOERROR;
		}

		bServerID = query->m_bServerID;

	UNDEFINE_QUERY()

	//	Find Connect Server IP/PORT
	DEFINE_QUERY( pDB, CSPRoute)

		query->m_bServerID = bServerID;
		query->m_bGroupID  = bGroupID;
		query->m_bType	   = SVRGRP_MAPSVR;

		if(!query->Call())
			query->m_nRET = SR_INTERNAL;

		if(query->m_nRET)
		{

#ifdef DEF_UDPLOG
			LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
			m_pUdpSocket->LogGameStart(pUser, query->m_nRET, bGroupID, pChar);
#endif

			pUser->SendCS_START_ACK(query->m_nRET, 0, 0, 0);
			return EC_NOERROR;
		}

#ifdef DEF_UDPLOG
			LPTCHARACTER pChar = pUser->FindCharacterBase(dwCharID);
			m_pUdpSocket->LogGameStart(pUser, SR_SUCCESS, bGroupID, pChar, query->m_szIPAddr, query->m_wPort, bServerID);
#endif

		pUser->SendCS_START_ACK( SR_SUCCESS, inet_addr(query->m_szIPAddr), query->m_wPort, bServerID);

		pUser->m_bLogout = FALSE;

	UNDEFINE_QUERY()

	return EC_SESSION_INVALIDCHAR;
}

DWORD CTLoginSvrModule::OnCS_AGREEMENT_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	pUser->m_bAgreement = TRUE;
	DEFINE_QUERY( pDB, CSPAgreement)
	query->m_dwUserID = pUser->m_dwID;
	query->Call();
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCS_TERMINATE_REQ(CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	// #FIXME: This is a possible backdoor. Keep an eye out for this!
	DWORD dwKey;
	packet
		>> dwKey;

	if(dwKey != 720809425)
		return EC_SESSION_INVALIDCHAR;

	LogEvent("L %d", pUser->m_dwID);

	UnregisterServer(TRUE);
	UnregisterAppId();
	Uninstall();

	PostQueuedCompletionStatus(
		m_hIocpWork, 0,
		COMP_EXIT, NULL);

	return EC_NOERROR;
}

DWORD CTLoginSvrModule::OnCS_HOTSEND_REQ( CSqlDatabase * pDB, LPMAPTGROUP pGROUP, CTUser * pUser, CPacket& packet)
{
	BYTE bAll;
	INT64 dlValue;
	packet
		>> dlValue
		>> bAll;

	if(m_hExecFile == INVALID_HANDLE_VALUE)
		return EC_NOERROR;

	if(bAll)
	{
		pUser->m_qCheckPoint.pop();
		if(dlValue != (m_dlCheckFile ^ pUser->m_dlCheckKey))
		{
			LogExecCheck(pUser->m_dwID, 1);
			return EC_SESSION_INVALIDCHAR;
		}
	}
	else if(!CheckFile(pUser, dlValue))
	{
		LogExecCheck(pUser->m_dwID, 2);
		return EC_SESSION_INVALIDCHAR;
	}

	return EC_NOERROR;
}