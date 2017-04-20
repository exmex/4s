#pragma once


//!
class CTClientNICEventSink : public CTNICEventSink
{
public :

	BOOL			InitTEVENT( CTClientWnd* pWnd, CString& strIP, int nTimer );
	void			ReleaseTEVENT();
	BOOL			UpdateService();

protected:

	virtual void	OnMediaDisconnect( IWbemClassObject *pObj );

private :

	BOOL			OpenService_Winmgmt();
	void			Close();

	DWORD			GetStartType();
	int				GetCurrentStatus();

	BOOL			SC_Start();
	BOOL			SC_Continue();
	BOOL			SC_Change_to_AutoStart();

private :

	CTClientWnd*	m_pMainWnd;
	CString			m_strIP;
	int				m_nTimer;

	SC_HANDLE		m_hServiceMgr;
	SC_HANDLE		m_hService;
	int				m_nStatus;
	bool			m_bInit;

public :

	CTClientNICEventSink();
	virtual ~CTClientNICEventSink();
};