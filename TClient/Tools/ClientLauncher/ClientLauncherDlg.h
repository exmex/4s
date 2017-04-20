// ClientLauncherDlg.h : header file
//

#pragma once
#include "afxwin.h"


typedef enum TMP_HACK_DETECT_TYPE
{
	TMP_HACK_NONE = 0,
	TMP_HACK_FOUND,
	TMP_HACK_TIMEOUT,
	TMP_HACK_INVALID_PE
} *LPTMP_HACK_DETECT_TYPE;


// CClientLauncherDlg dialog
class CClientLauncherDlg : public CDialog
{
// Construction
public:
	CClientLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENTLAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	static DWORD WINAPI _MPThread( LPVOID lpParam);
	void MPThread();

	void SetHWND( HWND hWND);
	void SetRUN( BYTE bRUN);
	void PostQUIT();
	void HackMSG();
	BYTE GetRUN();

public:
	BYTE LaunchTClient();

	BYTE BeginProtect();
	void EndProtect();
	void EndThread();

protected:
	CTModuleProtector m_vModuleGuard;
	CRITICAL_SECTION m_cs;

	HANDLE m_hMP;
	HWND m_hWND;
	BYTE m_bRUN;

public:
	CString m_strPARAM;
	BYTE m_bHACK;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
