// MempieceDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
/////////////////////////////
// 현승룡 스킨이미지
#include "HoverButton.h"
/////////////////////////////

#define WM_RESTART_PATCH			(WM_USER + 109)
#define WM_STOP_DOWNLOAD			(WM_USER + 119)
#define WM_CLOSE_SESSION			(WM_USER + 120)

#define APP_NAME					_T("4story")
#define BACKUP_EXT					_T("_bak")
#define PATH_DELIMETERS				_T("/\\")
#define REG_SUBKEY					_T("Software\\Zemi Interactive\\")
#define REG_VALUE_VERSION			_T("version")
#define REG_VALUE_LOCAL				_T("directory")
#define REG_VALUE_EXE				_T("exe")
#define REG_VALUE_PATCHSVR			_T("address")
#define REG_VALUE_PATCHPORT			_T("port")
#define REG_VALUE_MODE				_T("mode")
#define HOMEPAGE_NAME				_T("http://www.4story.com")
/*
/////////////////////////////////////////////////////////
// 현승룡 background 이미지
#define REG_VALUE_BACKGROUND		_T("background")
// 현승룡 그래픽 설정
#define REG_VALUE_WINDOW			_T("window")
#define REG_VALUE_SHADER			_T("shader")
#define REG_VALUE_CHARACTER			_T("character")
#define REG_VALUE_PAPERMATRIX		_T("papermatrix")
#define REG_VALUE_BACK				_T("back")
/////////////////////////////////////////////////////////
*/
#define PROGRESS_TYPE_DOWNLOAD		1
#define PROGRESS_TYPE_UNZIP			2
#define PROGRESS_TYPE_START			3

#define FTP_USER					_T("zemi4story")
#define FTP_PASS					_T("dhkdeoqkr")

typedef struct tagPATCHFILE
{
	DWORD m_dwVersion;
	CString m_strPath;
	CString m_strName;
	DWORD m_dwSize;
}PATCHFILE, *LPPATCHFILE;

typedef queue<LPPATCHFILE>			QPATCHFILE, *LPQPATCHFILE;

// CMempieceDlg 대화 상자
class CMempieceDlg : public CDialog
{
// 생성
public:
	CMempieceDlg(CWnd* pParent = NULL);	// 표준 생성자

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	DWORD OnReceive(CPacket * pPacket);
	void SessionStart(CString strIp, DWORD dwPort);
	void Say(CPacket * pPacket);

private:
	DWORD OnCT_PATCH_ACK(CPacket * pPacket);

	void SendCT_PATCH_REQ();
	void SendCT_PATCHSTART_REQ();

// 대화 상자 데이터
	enum { IDD = IDD_MEMPIECE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

protected:
	CWebBrowser2 m_webCtrl;

	CString		m_strIP;
	WORD		m_wPort;

	CString	m_strAppName;
	CString m_strRegSubKey;
	CString m_strGame;
	CString m_strGameSvr;

	CString		m_strLocal;
	CString		m_strDownload;
	CString		m_strFtpSvr;
	char		m_szPatchFileName[_MAX_PATH+1];

	WORD		m_wGamePort;
	WORD		m_wFtpPort;
	DWORD		m_dwVersion;
	DWORD		m_dwNextVersion;
	__int64		m_nPatchTotalSize;

	CHSession	m_session;
	QPATCHFILE	m_qPatchFile;
	HANDLE		m_hDownload;
	BYTE		m_blPatchFileUpdate;
	BYTE		m_bDownloading;
	BYTE		m_bCancel;
	BYTE		m_bIsFtpConnecting;	

	void InitWeb();
	BOOL ReadRegistry();
	BOOL CopyRegistry();
	static DWORD WINAPI _Download(LPVOID lpParam);
	DWORD Download();
	void DownloadFinish();
	void StartGame();
	CString LoadString(int nID);
	
	BOOL CreateDirectoryEx(CString strPath);
	void Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal, BYTE bType);
	int Unzip(LPCTSTR strZip, LPCTSTR strDirectory);
	void VerifyDirectory(LPCTSTR target, LPCTSTR path, BOOL bHaveFile);
	void CheckPatch();
	void RemoveDownloadDirectory();
	void DeleteDirectory(LPCTSTR strPath);
	BOOL SetVersion(DWORD dwVersion);
// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();	
	CProgressCtrl m_progressCurrent;
	CProgressCtrl m_progressTotal;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	////////////////////////////////////////////////////////////////////
	// 현승룡 스킨이미지	
	/*
	CLabel		m_strCurrent;
	CLabel		m_strTotal;

	CLabel		m_staticCurrent;
	CLabel		m_staticTotal;
	*/

	HBITMAP		hBmp;
	CBitmap		m_bSkin;
	BYTE		sFlag;

	~CMempieceDlg();	// 소멸자
	void LoadSkin();
	
	CHoverButton m_bOK;
	CHoverButton m_bCANCEL;
	CHoverButton m_bHome;
	CHoverButton m_bSetting;

	BOOL m_bFlash; // 버튼 깜박임

	void SetControlPos();	// 컨트롤 위치 지정
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);			
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	// 현승룡 그래픽 설정
	DWORD m_dwWindowMode;
	DWORD m_dwShaderMode;
	DWORD m_dwCharMode;
	DWORD m_dwPaperMode;
	DWORD m_dwBackMode;

	// Clicked Homepage / Setting Button
	afx_msg void OnBnClickedButtonHomepage();
	afx_msg void OnBnClickedButtonSetting();
	// Write Registry
	BOOL WriteRegistry();
	////////////////////////////////////////////////////////////////////
	afx_msg void OnTimer(UINT nIDEvent);

	void DownloadEnd(); // Patch Download End
};
