#pragma once

class CTPatchSession :
	public CSession
{
public:
	DWORD m_dwTick;

public:
	CTPatchSession(void);
	~CTPatchSession(void);

	void SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwCurSession, DWORD dwCurUser, DWORD dwActiveUser);
	void SendCT_PATCH_ACK(CString strFTP, SOCKADDR_IN * pAddrLogin, LPVPATCHFILE pvPatch);
	void SendCT_NEWPATCH_ACK(CString strFTP, SOCKADDR_IN * pAddrLogin, LPVPATCHFILE pvPatch,DWORD dwMinBetaVer);
	void SendCT_PREPATCH_ACK(CString strFTP, SOCKADDR_IN * pAddrLogin, LPVPATCHFILE pvPatch);
};
