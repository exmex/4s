// 4StoryDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#ifdef _LOGMSG
#define ERRMSG(p)					AfxMessageBox(p);LOGMSG(p)
#define LOGMSG(p)					{FILE* f = fopen("c:\\PrePatch.log" ,"a");\
									fprintf(f,"%s\n", p);\
									fclose(f);}
#else
#define ERRMSG(p)					NULL
#define LOGMSG(p)					NULL
#endif

#define UM_TRAY						(WM_USER + 100)
#define WM_CLOSE_SESSION			(WM_USER + 120)
#define WM_CONNECT_SESSION			(WM_USER + 121)

#define WM_PREPATCH_RRESET			(WM_USER + 7110)
#define WM_PREPATCH_RQUIT			(WM_USER + 7111)

#ifdef _GOA
	#define APP_NAME					_T("GOA PrePatch")
	#define REG_NAME					_T("Gates Of Andaron")
	#define IDI_TMENU					IDI_GOAICON
#else
	#define APP_NAME					_T("4Story PrePatch")
	#define REG_NAME					_T("4Story")
	#define IDI_TMENU					IDI_4SICON
#endif

#define APP_DOWNFOLDER				_T("_BetaPatch\\")
#define APP_EXT						_T(".exe")
#define REG_SUBKEY					_T("Software\\Zemi Interactive\\")
#define REG_VALUE_LOCAL				_T("directory")
#define REG_VALUE_PATCHSVR			_T("address")
#define REG_VALUE_PATCHPORT			_T("port")

//=====================================================================================
// 각국 버전마다 변경되야 하는 항목들
// String Table 의 Error Message 변경
#define REG_COUNTRY					_T("")
#define COUNTRY_CODE				_T("")
//=====================================================================================

#define FTP_USER					_T("zemi4story")
#define FTP_PASS					_T("dhkdeoqkr")

typedef struct tagPATCHFILE
{
	DWORD m_dwVersion;
	CString m_strPath;
	CString m_strName;
	DWORD m_dwSize;
	DWORD m_dwBetaVer;
}PATCHFILE, *LPPATCHFILE;

typedef vector<LPPATCHFILE>			VPATCHFILE, *LPVPATCHFILE;

// CStoryDlg 대화 상자
class CStoryDlg : public CDialog
{
// 생성
public:
	CStoryDlg(CWnd* pParent = NULL);	// 표준 생성자
	~CStoryDlg();

// 대화 상자 데이터
	enum { IDD = IDD_STORY_DIALOG };

	void LoadSkin();

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	void Say(CPacket * pPacket);
	void SessionStart();
	DWORD OnReceive(CPacket * pPacket);
	void DisConnected();

	static DWORD WINAPI _Download(LPVOID lpParam);
	DWORD Download();
	void DownloadFinish();
	void DownloadEnd();

	BOOL ReadRegistry();
	void SetVersion(LPPATCHFILE p_PatchFile);
	void InfoFileWrite();
	void InfoFileRead();
	void GotoTray(void);
	void ReadTextFile();

	BYTE FindPatchFile(CString lpPathName, DWORD dwSize);
	BOOL CreateDirectoryEx(CString strPath);

	DWORD OnCT_PREPATCH_ACK(CPacket * pPacket);

	void SendCT_PREPATCH_REQ();
	void SendCT_PATCHSTART_REQ();
	void SendCT_PREPATCHCOMPLETE_REQ();

	void Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal);
	inline CString LoadString(int nID) {CString strMsg;	strMsg.LoadString(nID);	return strMsg;}
	inline void DisplayText(CString str) {if(m_strDrawText != str){m_strDrawText = str; if(m_bShow)InvalidateRect(&m_rtText);}}

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnTray(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCancel();
	afx_msg void OnMenuConnect();
	afx_msg void OnCloseBtn(){this->ShowWindow(SW_HIDE);};
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	BYTE		m_bConnect;
protected:
	HICON		m_hIcon;
	CBitmap		m_bSkin;
	BYTE		m_bFlag;
	BYTE		m_bFinish;
	DWORD		m_dwLastVer;
	BYTE		m_bInCom;

	CString		m_strIP;
	WORD		m_wPort;
	CHSession	m_session;
	WORD		m_wGamePort;
	WORD		m_wFtpPort;
	__int64		m_nPatchTotalSize;
	int			m_nTotalPos;

	CString 	m_strRegSubKey;
	CString 	m_strGame;
	CString 	m_strGameSvr;
	CString		m_strSubkey;

	CString		m_strLocal;
	CString		m_strDownload;
	CString		m_strFtpSvr;
	char		m_szPatchFileName[_MAX_PATH+1];

	VPATCHFILE	m_VPATCHFILE;
	VPATCHFILE	m_VLocal;
	HANDLE		m_hDownload;
	BYTE		m_bDownloading;
	BYTE		m_bShow;
	CString		m_strDrawText;
	RECT		m_rtText;

	CProgressCtrl m_progressCurrent;
	CProgressCtrl m_progressTotal;
	CBitmapButton m_btnClose;
	
	NOTIFYICONDATA	m_nid;
};
