#include "StdAfx.h"
#include "TChatSocket.h"


void CTChatSocket::SendCS_DISCONNECT_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_DISCONNECT_REQ);
	Say(&vMSG);
}

void CTChatSocket::SendCS_LOGIN_REQ( CString strUserID, CString strPasswd)
{
	CPacket vMSG;

	vMSG.SetID(CS_LOGIN_REQ)
		<< TVERSION
		<< strUserID
		<< strPasswd;

	Say(&vMSG);
}

void CTChatSocket::SendCS_GROUPLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GROUPLIST_REQ);
	Say(&vMSG);
}

void CTChatSocket::SendCS_CHANNELLIST_REQ( BYTE bGroupID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHANNELLIST_REQ)
		<< bGroupID;

	Say(&vMSG);
}

void CTChatSocket::SendCS_CHARLIST_REQ( BYTE bGroupID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHARLIST_REQ)
		<< bGroupID;

	Say(&vMSG);
}

void CTChatSocket::SendCS_START_REQ( BYTE bGroupID,
									 BYTE bChannel,
									 DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_START_REQ)
		<< bGroupID
		<< bChannel
		<< dwCharID;

	Say(&vMSG);
}

void CTChatSocket::SendCS_CONNECT_REQ( WORD wVersion,
									   BYTE bChannel,
									   DWORD dwUserID,
									   DWORD dwCharID,
									   DWORD dwKEY,
									   DWORD dwIPAddr,
									   WORD wPort)
{
	CPacket vMSG;

	vMSG.SetID(CS_CONNECT_REQ)
		<< wVersion
		<< bChannel
		<< dwUserID
		<< dwCharID
		<< dwKEY
		<< dwIPAddr
		<< wPort;

	Say(&vMSG);
}

void CTChatSocket::SendCS_CHAT_REQ( CString strName,
								    BYTE bGroup,
								    DWORD dwTargetID,
									CString strNAME,
									CString strMSG,
									BYTE bCountry,
									MAPCHAR & mapCHAR)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHAT_REQ)
		<< strName
		<< bGroup
		<< dwTargetID
		<< strNAME
		<< strMSG;

	if(bGroup == CHAT_NEAR)
	{
		MAPCHAR mapTarget;
		mapTarget.clear();

		MAPCHAR::iterator it;
		for(it=mapCHAR.begin(); it!=mapCHAR.end(); it++)
		{
			if((*it).second == bCountry)
				mapTarget.insert(MAPCHAR::value_type((*it).first, (*it).second));
		}

		vMSG << WORD(mapTarget.size());

		for(it=mapTarget.begin(); it!=mapTarget.end(); it++)
			vMSG << (*it).first;

		mapTarget.clear();
	}

	Say(&vMSG);
}

void CTChatSocket::SendCS_RELAYCHARDATA_REQ(WORD wVersion,
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

void CTChatSocket::SendCT_SERVICEMONITOR_ACK(DWORD dwTick)
{
	CPacket vMSG;
	vMSG.SetID(CT_SERVICEMONITOR_ACK)
		<< dwTick;

	Say(&vMSG);
}

void CTChatSocket::SendCS_CHECKRELAY_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_CHECKRELAY_REQ);

	Say(&vMSG);
}
