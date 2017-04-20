#include "StdAfx.h"

void CTServer::SendRW_RELAYSVR_ACK(BYTE bNation,
								   LPVECTORDWORD vTOPERATOR,
								   LPMAPTSTRING mapTSVRMSG)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_RELAYSVR_ACK)
		<< bNation
		<< WORD(vTOPERATOR->size());

	for(WORD i=0; i<vTOPERATOR->size(); i++)
	{
		(*pMSG)
			<< (*vTOPERATOR)[i];
	}

	(*pMSG)
		<< WORD(mapTSVRMSG->size());

	MAPTSTRING::iterator it;
	for(it=mapTSVRMSG->begin(); it != mapTSVRMSG->end(); it++)
	{
		(*pMSG)
			<< (*it).first
			<< (*it).second;
	}

	Say(pMSG);
}

void CTServer::SendRW_ENTERCHAR_ACK(DWORD dwCharID,
									CString strName,
									BYTE bResult,
									BYTE bCountry,
									BYTE bAidCountry,
									DWORD dwGuildID,
									DWORD dwGuildChief,
									BYTE bDuty,
									WORD wPartyID,
									DWORD dwPartyChiefID,
									WORD wCorpsID,
									DWORD dwGeneralID,
									DWORD dwTacticsID,
									DWORD dwTacticsChief,
									WORD wMapID,
									WORD wUnitID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_ENTERCHAR_ACK)
		<< dwCharID
		<< strName
		<< bResult
		<< bCountry
		<< bAidCountry
		<< dwGuildID
		<< dwGuildChief
		<< bDuty
		<< wPartyID
		<< dwPartyChiefID
		<< wCorpsID
		<< dwGeneralID
		<< dwTacticsID
		<< dwTacticsChief
		<< wMapID
		<< wUnitID;

	Say(pMSG);
}

void CTServer::SendRW_PARTYADD_ACK(DWORD dwCharID,
                                   WORD wPartyID,
                                   DWORD dwChiefID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_PARTYADD_ACK)
		<< dwCharID
		<< wPartyID
		<< dwChiefID;

	Say(pMSG);
}
void CTServer::SendRW_PARTYDEL_ACK(DWORD dwCharID,
                                   WORD wPartyID,
                                   DWORD dwChiefID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_PARTYDEL_ACK)
		<< dwCharID
		<< wPartyID
		<< dwChiefID;

	Say(pMSG);
}
void CTServer::SendRW_PARTYCHGCHIEF_ACK(WORD wPartyID,
                                        DWORD dwChiefID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_PARTYCHGCHIEF_ACK)
		<< wPartyID
		<< dwChiefID;

	Say(pMSG);
}
void CTServer::SendRW_GUILDADD_ACK(DWORD dwCharID,
                                   DWORD dwGuildID,
								   DWORD dwMasterID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_GUILDADD_ACK)
		<< dwCharID
		<< dwGuildID
		<< dwMasterID;

	Say(pMSG);
}
void CTServer::SendRW_GUILDDEL_ACK(DWORD dwCharID,
                                   DWORD dwGuildID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_GUILDDEL_ACK)
		<< dwCharID
		<< dwGuildID;

	Say(pMSG);
}
void CTServer::SendRW_GUILDCHGMASTER_ACK(DWORD dwGuildID,
                                         DWORD dwMasterID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_GUILDCHGMASTER_ACK)
		<< dwGuildID
		<< dwMasterID;

	Say(pMSG);
}
void CTServer::SendRW_CORPSJOIN_ACK(WORD wPartyID,
									WORD wCorpsID,
									WORD wCommander)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_CORPSJOIN_ACK)
		<< wPartyID
		<< wCorpsID
		<< wCommander;

	Say(pMSG);
}
void CTServer::SendRW_CHANGENAME_ACK(DWORD dwCharID,
									 BYTE bType,
									 BYTE bValue,
									 CString strName)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_CHANGENAME_ACK)
		<< dwCharID
		<< bType
		<< bValue
		<< strName;

	Say(pMSG);
}
void CTServer::SendRW_TACTICSADD_ACK(DWORD dwCharID,
									 DWORD dwGuildID,
									 DWORD dwGuildMaster)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_TACTICSADD_ACK)
		<< dwCharID
		<< dwGuildID
		<< dwGuildMaster;

	Say(pMSG);
}
void CTServer::SendRW_TACTICSDEL_ACK(DWORD dwCharID,
									 DWORD dwGuildID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(RW_TACTICSDEL_ACK)
		<< dwCharID
		<< dwGuildID;

	Say(pMSG);
}

void CTServer::SendRW_CHATBAN_ACK(CString strName, 
								  __int64 nChatBanTime)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(RW_CHATBAN_ACK)
		<< strName
		<< nChatBanTime;
	
	Say(pMSG);
}

void CTServer::SendRW_CHANGEMAP_ACK(DWORD dwCharID,
                                    WORD wMap,
									WORD wUnitID)

{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(RW_CHANGEMAP_ACK)
		<< dwCharID
		<< wMap
		<< wUnitID;
	
	Say(pMSG);
}
