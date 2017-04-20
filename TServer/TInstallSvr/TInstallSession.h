#pragma once

class CTInstallSession : public CSession
{
public:
	DWORD m_dwTick;

public:
	CTInstallSession(void)
	{
		m_bSessionType = SESSION_CLIENT;
		m_dwTick = 0;
	}
	~CTInstallSession(void){}

	void SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwCurSession, DWORD dwCurUser, DWORD dwActiveUser);
	void SendCT_INSTALL_ACK(DWORD dwVersion, DWORD dwSegmentSize, CString strFTP, LPQINSTFILE pQFile);
	void SendCT_FILEADDR_ACK(DWORD dwFileID, WORD wSegmentID, CString strCheckSum, deque<DWORD>* qIpAddr);
};
