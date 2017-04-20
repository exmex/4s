#include "TInstallSession.h"
#include "stdafx.h"


BOOL CTInstallSession::Open( SOCKET sock, CPacket& init)
{
	if(CSession::Open(sock, init))
	{
		m_dwAddr = m_addr.sin_addr.s_addr;
		return TRUE;
	}
	return FALSE;
}

void CTInstallSession::AddFile(LPINSTFILE pFile)
{
	SMART_LOCKCS(&m_csFileList);
	if(pFile)
	{
		FILELINK FILE;
		FILE.m_bErrCount = 0;
		FILE.m_pInstFile = pFile;

		m_mapFileList.insert(map<DWORD, FILELINK>::value_type(pFile->m_dwFileID, FILE));
	}
}

void CTInstallSession::DeleteAllFileInfo()
{
	SMART_LOCKCS(&m_csFileList);
	for(map<DWORD, FILELINK>::iterator it = m_mapFileList.begin(); it != m_mapFileList.end(); it++)
	{
		LPINSTFILE pFile = it->second.m_pInstFile;
		pFile->RemoveIp(m_dwAddr);
	}
	m_mapFileList.clear();
}

void CTInstallSession::DeleteFileInfo(DWORD dwFileID)
{
	map<DWORD, FILELINK>::iterator it = m_mapFileList.find(dwFileID);
	if(it != m_mapFileList.end())
	{
		LPINSTFILE pFile = it->second.m_pInstFile;
		pFile->RemoveIp(m_dwAddr);

		m_mapFileList.erase(it);
	}	
}


BOOL CTInstallSession::InstallConnect(HANDLE CompletionPort, DWORD dwCompletionKey, DWORD dwNumberOfConcurrentThreads)
{
	if(GetCurrentTime() - m_dwTick < 60000)
		return TRUE;

	m_dwTick = GetCurrentTime();

	if(!CreateSocket(m_Connectsock))
		return FALSE;

	SOCKADDR_IN addr;
	memset( &addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;

	int nError = bind( m_Connectsock, (sockaddr *)&addr, sizeof(SOCKADDR_IN));
	if(nError == SOCKET_ERROR )
	{
		Close();
		return FALSE;
	}

	DWORD dwBytes = 0;
	LPFN_CONNECTEX lpfnConnectEx = NULL; 
	GUID GuidConnectEx = WSAID_CONNECTEX;

	nError = WSAIoctl(
		m_Connectsock,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidConnectEx,
		sizeof(GuidConnectEx),
		(LPVOID)&lpfnConnectEx,
		sizeof(lpfnConnectEx),
		&dwBytes,
		NULL,
		NULL);

	if(nError == SOCKET_ERROR)
	{
		Close();
		return FALSE;
	}

	HANDLE hIOCP = CreateIoCompletionPort(
		(HANDLE)m_Connectsock,
		CompletionPort,
		dwCompletionKey,
		0);

	if(!hIOCP)
	{
		Close();
		return FALSE;
	}

	SOCKADDR_IN addr2;
	memset( &addr2, 0, sizeof(SOCKADDR_IN));
	addr2.sin_family = AF_INET;
	addr2.sin_addr.s_addr = GetIp();
	addr2.sin_port = htons(CLIENT_LISTEN_PORT);

	if(!lpfnConnectEx(m_Connectsock, (sockaddr *)&addr2, sizeof(SOCKADDR_IN), NULL, 0, NULL, (LPOVERLAPPED)&m_ovCONN))
	{
		nError = WSAGetLastError();
		if(nError != WSA_IO_PENDING)
		{
			Close();
			return FALSE;
		}
	}

	return TRUE;
}

void CTInstallSession::CloseConnect()
{
	if(INVALID_SOCKET == m_Connectsock)
		return;
	
	closesocket(m_Connectsock);
	m_Connectsock = INVALID_SOCKET;
}

__int64 CTInstallSession::GetFileCheckValue(DWORD dwOffset, HANDLE phFile)
{
	if(INVALID_HANDLE_VALUE == phFile)
		return 0;

	__int64 nBuffer = 0;
	DWORD	dwReadLen = 0;

	DWORD dwRet = SetFilePointer(phFile, dwOffset, NULL, FILE_BEGIN);
	if(dwRet != INVALID_SET_FILE_POINTER)
	{
		BYTE bRet = ReadFile(phFile, (LPVOID)&nBuffer, sizeof(__int64), &dwReadLen, NULL);
		if(bRet && dwReadLen)
			return nBuffer;
	}
	return 0;
}