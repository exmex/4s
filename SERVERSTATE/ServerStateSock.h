#pragma once

class CServerStateSock : public CAsyncSocket
{
public:
	static CListCtrl *m_pctrlServerState;
	static CWnd *m_pHost;

	int m_nIndex;

public:
	CServerStateSock(void);
	~CServerStateSock(void);

	virtual void OnClose( int nErrorCode);
	virtual void OnConnect( int nErrorCode);
};
