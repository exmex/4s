#include "stdafx.h"
#include "TClient.h"
#include "Curse.h"

#ifdef TEST_MODE
#include "TClientGame.h"
#endif

void CTClientSession::SendCS_RELAYCHARDATA_REQ(WORD wVersion,
                                               DWORD dwCharID,
											   CString strName)
{
	CPacket vMSG;

	vMSG.SetID(CS_RELAYCHARDATA_REQ)
		<< wVersion
		<< dwCharID
		<< strName;

	Say(&vMSG);
}

void CTClientSession::SendCS_RELAYPROTECTEDLIST_REQ(VTSTRING& vName, VTBYTE& vBlockOption)
{
	CPacket vMSG;

	vMSG.SetID(CS_RELAYPROTECTEDLIST_REQ)
		<< WORD(vName.size());

	for(INT i=0; i<vName.size(); i++)
	{
		vMSG
			<< vName[i]
			<< vBlockOption[i];
	}

	Say(&vMSG);
}

void CTClientSession::SendCS_RELAYPROTECTEDADD_REQ(CString strName, BOOL bBlockOption )
{
	CPacket vMSG;

	vMSG.SetID(CS_RELAYPROTECTEDADD_REQ)
		<< strName
		<< bBlockOption;

	Say(&vMSG);
}

void CTClientSession::SendCS_RELAYPROTECTEDOPTION_REQ(CString strName, BYTE bOption)
{
	CPacket vMSG;

	vMSG.SetID( CS_RELAYPROTECTEDOPTION_REQ)
		<< strName
		<< bOption;

	Say( &vMSG);
}

void CTClientSession::SendCS_RELAYPROTECTEDDEL_REQ(CString strName)
{
	CPacket vMSG;

	vMSG.SetID(CS_RELAYPROTECTEDDEL_REQ)
		<< strName;

	Say(&vMSG);
}

void CTClientSession::SendCS_CHAT_REQ( CString strSender,
									   BYTE bGroup,
									   DWORD dwTargetID,
									   CString strNAME,
									   CString strMSG,
									   BYTE bMyCountry,
									   MAPPLAYER & mapTarget)
{
	strMSG = CCurse::ConvertString( strMSG, TRUE);
	CPacket vMSG;

	vMSG.SetID(CS_CHAT_REQ)
		<< strSender
		<< bGroup
		<< dwTargetID
		<< strNAME
		<< strMSG;

	if(bGroup == CHAT_NEAR)
	{
		vMSG << WORD(mapTarget.size());

		MAPPLAYER::iterator it, end;
		it = mapTarget.begin();
		end = mapTarget.end();
		for(; it != end ; ++it )
			vMSG << it->second->m_dwID; //vTarget[i];
	}

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSCMD_REQ( WORD wSquadID,
										   DWORD dwCharID,
										   WORD wMapID,
										   BYTE bCMD,
										   DWORD dwTargetID,
										   BYTE bTargetType,
										   WORD wTargetX,
										   WORD wTargetZ)
{
	CPacket vMSG;

	vMSG.SetID(CS_CORPSCMD_REQ)
		<< wSquadID
		<< dwCharID
		<< wMapID
		<< bCMD
		<< dwTargetID
		<< bTargetType
		<< wTargetX
		<< wTargetZ;

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSENEMYLIST_REQ( WORD wSquadID,
												 DWORD dwCharID,
												 LPTCORPS pTCORPS)
{
	CPacket vMSG;

	MAPTENEMY::iterator itTENEMY;
	BYTE bCount;

	bCount = BYTE(pTCORPS->m_mapFIXRECALL.size() + pTCORPS->m_mapRECALL.size() + pTCORPS->m_mapMON.size() + pTCORPS->m_mapPC.size());
	vMSG.SetID(CS_CORPSENEMYLIST_REQ)
		<< wSquadID
		<< dwCharID
		<< bCount;

	for( itTENEMY = pTCORPS->m_mapFIXRECALL.begin(); itTENEMY != pTCORPS->m_mapFIXRECALL.end(); itTENEMY++)
	{
		MAPDWORD::iterator itTREPORTER;

		vMSG
			<< (*itTENEMY).second->m_dwID
			<< (*itTENEMY).second->m_bType
			<< (*itTENEMY).second->m_fSPEED
			<< (*itTENEMY).second->m_wMapID
			<< (*itTENEMY).second->m_wPosX
			<< (*itTENEMY).second->m_wPosZ
			<< (*itTENEMY).second->m_wDIR
			<< BYTE((*itTENEMY).second->m_mapTREPORTER.size());

		for( itTREPORTER = (*itTENEMY).second->m_mapTREPORTER.begin(); itTREPORTER != (*itTENEMY).second->m_mapTREPORTER.end(); itTREPORTER++)
			vMSG << DWORD((*itTREPORTER).second);
	}

	for( itTENEMY = pTCORPS->m_mapRECALL.begin(); itTENEMY != pTCORPS->m_mapRECALL.end(); itTENEMY++)
	{
		MAPDWORD::iterator itTREPORTER;

		vMSG
			<< (*itTENEMY).second->m_dwID
			<< (*itTENEMY).second->m_bType
			<< (*itTENEMY).second->m_fSPEED
			<< (*itTENEMY).second->m_wMapID
			<< (*itTENEMY).second->m_wPosX
			<< (*itTENEMY).second->m_wPosZ
			<< (*itTENEMY).second->m_wDIR
			<< BYTE((*itTENEMY).second->m_mapTREPORTER.size());

		for( itTREPORTER = (*itTENEMY).second->m_mapTREPORTER.begin(); itTREPORTER != (*itTENEMY).second->m_mapTREPORTER.end(); itTREPORTER++)
			vMSG << DWORD((*itTREPORTER).second);
	}

	for( itTENEMY = pTCORPS->m_mapMON.begin(); itTENEMY != pTCORPS->m_mapMON.end(); itTENEMY++)
	{
		MAPDWORD::iterator itTREPORTER;

		vMSG
			<< (*itTENEMY).second->m_dwID
			<< (*itTENEMY).second->m_bType
			<< (*itTENEMY).second->m_fSPEED
			<< (*itTENEMY).second->m_wMapID
			<< (*itTENEMY).second->m_wPosX
			<< (*itTENEMY).second->m_wPosZ
			<< (*itTENEMY).second->m_wDIR
			<< BYTE((*itTENEMY).second->m_mapTREPORTER.size());

		for( itTREPORTER = (*itTENEMY).second->m_mapTREPORTER.begin(); itTREPORTER != (*itTENEMY).second->m_mapTREPORTER.end(); itTREPORTER++)
			vMSG << DWORD((*itTREPORTER).second);
	}

	for( itTENEMY = pTCORPS->m_mapPC.begin(); itTENEMY != pTCORPS->m_mapPC.end(); itTENEMY++)
	{
		MAPDWORD::iterator itTREPORTER;

		vMSG
			<< (*itTENEMY).second->m_dwID
			<< (*itTENEMY).second->m_bType
			<< (*itTENEMY).second->m_fSPEED
			<< (*itTENEMY).second->m_wMapID
			<< (*itTENEMY).second->m_wPosX
			<< (*itTENEMY).second->m_wPosZ
			<< (*itTENEMY).second->m_wDIR
			<< BYTE((*itTENEMY).second->m_mapTREPORTER.size());

		for( itTREPORTER = (*itTENEMY).second->m_mapTREPORTER.begin(); itTREPORTER != (*itTENEMY).second->m_mapTREPORTER.end(); itTREPORTER++)
			vMSG << DWORD((*itTREPORTER).second);
	}

	Say(&vMSG);
}

void CTClientSession::SendCS_MOVECORPSENEMY_REQ( DWORD dwReporterID,
												 CTClientMoveObj *pTOBJ,
												 WORD wMapID)
{
	CPacket vMSG;

	vMSG.SetID(CS_MOVECORPSENEMY_REQ)
		<< dwReporterID
		<< pTOBJ->m_dwID
		<< pTOBJ->m_bType
		<< pTOBJ->m_fMoveSpeed
		<< wMapID
		<< WORD(pTOBJ->GetPositionX())
		<< WORD(pTOBJ->GetPositionZ())
		<< WORD(pTOBJ->m_bBlock ? MOVE_NONE : pTOBJ->m_wMoveDIR);

	Say(&vMSG);
}

void CTClientSession::SendCS_MOVECORPSUNIT_REQ( WORD wSquadID,
											    WORD wMapID,
												DWORD dwBspID,
											    CTClientMoveObj *pTOBJ)
{
	FLOAT fMoveSpeed = pTOBJ->m_fMoveSpeed;
	WORD wMoveDIR = pTOBJ->m_wMoveDIR;

	if(pTOBJ->m_bType == OT_PC)
	{
		CTClientChar* pCHAR = (CTClientChar*) pTOBJ;

		if( pCHAR->m_pRidingPet )
		{
			fMoveSpeed = pCHAR->m_pRidingPet->m_fMoveSpeed;
			wMoveDIR = pCHAR->m_pRidingPet->m_wMoveDIR;
		}
	}


	CPacket vMSG;
    
	vMSG.SetID(CS_MOVECORPSUNIT_REQ)
		<< wSquadID
		<< pTOBJ->m_dwID
		<< fMoveSpeed //pTOBJ->m_fMoveSpeed
		<< wMapID
		<< dwBspID
		<< WORD(pTOBJ->GetPositionX())
		<< WORD(pTOBJ->GetPositionZ())
		<< WORD(pTOBJ->m_bBlock ? MOVE_NONE : wMoveDIR /*pTOBJ->m_wMoveDIR*/);

	Say(&vMSG);
}

void CTClientSession::SendCS_ADDCORPSENEMY_REQ( DWORD dwReporterID,
											    WORD wMapID,
											    CTClientMoveObj *pTOBJ)
{
	if( pTOBJ->m_bType == OT_PC && pTOBJ->m_bGhost )
		return ;

	CPacket vMSG;
	vMSG.SetID(CS_ADDCORPSENEMY_REQ)
		<< dwReporterID
		<< pTOBJ->m_dwID
		<< pTOBJ->m_bType
		<< pTOBJ->m_fMoveSpeed
		<< wMapID
		<< WORD(pTOBJ->GetPositionX())
		<< WORD(pTOBJ->GetPositionZ())
		<< pTOBJ->m_wMoveDIR;

	Say(&vMSG);
}

void CTClientSession::SendCS_DELCORPSENEMY_REQ( DWORD dwReporterID,
											    DWORD dwEnemyID,
												BYTE bEnemyType)
{
	CPacket vMSG;

	vMSG.SetID(CS_DELCORPSENEMY_REQ)
		<< dwReporterID
		<< dwEnemyID
		<< bEnemyType;

	Say(&vMSG);
}

void CTClientSession::SendCS_CORPSHP_REQ( WORD wSquadID,
										  DWORD dwCharID,
										  DWORD dwMaxHP,
										  DWORD dwHP)
{
	CPacket vMSG;

	vMSG.SetID(CS_CORPSHP_REQ)
		<< wSquadID
		<< dwCharID
		<< dwMaxHP
		<< dwHP;

	Say(&vMSG);
}
