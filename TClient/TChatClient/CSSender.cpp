#include "StdAfx.h"
#include "TChatSocket.h"


void CTChatSocket::SendCS_DISCONNECT_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_DISCONNECT_REQ);
	Say(&vMSG);
}

void CTChatSocket::SendCS_LOGIN_REQ( CString strUserID, CString strPasswd, DWORD dwSiteCode)
{
	CPacket vMSG;

	vMSG.SetID(CS_LOGIN_REQ)
		<< TVERSION
		<< strUserID
		<< strPasswd;

	//HANDLE hFile = INVALID_HANDLE_VALUE;
	//TCHAR szFN[MAX_PATH];
	//GetModuleFileName(NULL, szFN, MAX_PATH);
	//char * pSlash = ::strrchr(szFN, '\\');

	//	hFile = CreateFile(
	//		szFN,
	//		GENERIC_READ,
	//		FILE_SHARE_READ,
	//		NULL,
	//		OPEN_EXISTING,
	//		FILE_ATTRIBUTE_NORMAL |
	//		FILE_FLAG_RANDOM_ACCESS,
	//		NULL);

	//m_dlCheckFile = 0;
	//INT64 dlValue = 0;
	//DWORD dwRead = 0;

	//if(hFile != INVALID_HANDLE_VALUE)
	//{
	//	BYTE bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
	//	while(bResult && dwRead)
	//	{
	//		m_dlCheckFile ^= dlValue;
	//		bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
	//	}
	//}

	TCHAR szFN[MAX_PATH];
	GetModuleFileName(NULL, szFN, MAX_PATH);
	char *pSlash = ::strrchr(szFN, '\\');
	if( pSlash )
		::lstrcpy(pSlash + 1, "\\TClient.exe");

	HANDLE hFile = INVALID_HANDLE_VALUE;

	hFile = CreateFile(
		szFN,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_RANDOM_ACCESS,
		NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		INT64 dlValue = 0;
		DWORD dwNumberOfBytesRead = 0;

		BYTE bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwNumberOfBytesRead, NULL);
		while(bResult && dwNumberOfBytesRead)
		{
			m_dlCheckFile ^= dlValue;
			bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwNumberOfBytesRead, NULL);
		}
	}

	vMSG
		<< m_dlCheckFile;

	//if( CTNationOption::MODIFY_DIRECTLOGIN )
		//vMSG << dwSiteCode;

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

void CTChatSocket::SendCS_HOTSEND_REQ(INT64 dlValue, BYTE bAll)
{
	CPacket vMSG;

	vMSG.SetID(CS_HOTSEND_REQ)
		<< dlValue
		<< bAll;

	Say(&vMSG);
}

BYTE CTChatSocket::CheckModuleFile(BYTE bAll, INT64 dlCP)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	TCHAR szFN[MAX_PATH];
	GetModuleFileName(NULL, szFN, MAX_PATH);
	char * pSlash = ::strrchr(szFN, '\\');
	if( pSlash )
		::lstrcpy(pSlash + 1, "\\TClient.exe");

//	if(!lstrcmp(pSlash+1, _T("TClient.exe")))
//	{
//		hFile = CreateFile(
//			szFN,
//			GENERIC_READ,
//			FILE_SHARE_READ,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL |
//			FILE_FLAG_RANDOM_ACCESS,
//			NULL);
//	}
//	else if(!lstrcmp(pSlash+1, _T("TLoader.exe")))
//	{
//		hFile = CreateFile(
//			_T(".\\TClient.exe"),
//			GENERIC_READ,
//			FILE_SHARE_READ,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL |
//			FILE_FLAG_RANDOM_ACCESS,
//			NULL);
//	}
//	else
//	{
//#ifdef TEST_MODE
		hFile = CreateFile(
			szFN,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL |
			FILE_FLAG_RANDOM_ACCESS,
			NULL);
//#else
//		return FALSE;
//#endif
//	}

	INT64 dlCheck = 0;
	INT64 dlValue = 0;
	DWORD dwRead = 0;

	if(hFile != INVALID_HANDLE_VALUE)
	{
		if(bAll)
		{
			BYTE bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
			while(bResult && dwRead)
			{
				dlCheck ^= dlValue;
				bResult = ReadFile(hFile, (LPVOID)&dlValue, sizeof(INT64), &dwRead, NULL);
			}

			if(dlCheck != m_dlCheckFile)
				return FALSE;

			dlCheck ^= dlCP;
		}
		else
		{
			DWORD dwRet = SetFilePointer(hFile, DWORD(dlCP), NULL, FILE_BEGIN);
			if(dwRet != INVALID_SET_FILE_POINTER)
			{
				ReadFile(hFile, (LPVOID)&dlCheck, sizeof(INT64), &dwRead, NULL);
			}
		}

		CloseHandle(hFile);
		SendCS_HOTSEND_REQ(dlCheck, bAll);
	}

	return TRUE;
}