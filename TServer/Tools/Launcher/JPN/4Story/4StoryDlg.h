// 4StoryDlg.h : ?E갋파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "HoverButton.h"
#include "GameSetting.h" // 게임 설정 창 띄퓖E갋

#include <d3d9.h>
#include <dxdiag.h>
#include <d3dx9.h>

#pragma comment(lib,"dxguid.lib")

#define WM_RESTART_PATCH			(WM_USER + 109)
#define WM_STOP_DOWNLOAD			(WM_USER + 119)
#define WM_CLOSE_SESSION			(WM_USER + 120)

#define APP_EXT						_T(".exe")
#define BACKUP_EXT					_T("_bak")
#define PATH_DELIMETERS				_T("/\\")
#define REG_SUBKEY					_T("Software\\Zemi Interactive\\")
#define REG_VALUE_VERSION			_T("version")
#define REG_VALUE_LOCAL				_T("directory")
#define REG_VALUE_EXE				_T("exe")
#define REG_VALUE_PATCHSVR			_T("address")
#define REG_VALUE_PATCHPORT			_T("port")
#define REG_VALUE_MODE				_T("mode")

#define DEFAULT_SCREEN_X			1024
#define DEFAULT_SCREEN_Y			768
//=====================================================================================
// 각국 버픸E떪?변경되야 하는 항목탛E
// String Table 의 Error Message 변컖E
#define APP_NAME					_T("4story.exe")
#define REG_SETTIG					_T("Lost Eden")  //option registry
#define HOMEPAGE_NAME				_T("http://losteden.jp/")
#define LAUNCHERWEB_NAME			_T("http://losteden.jp/notices/showLauncher/1/0")
#define REG_COUNTRY					_T("")
#define TOOLTIP_START				_T("긒??귩둎럑궢귏궥갃")
#define TOOLTIP_CLOSE				_T("긒??귩뢎뿹궢귏궥갃")
#define TOOLTIP_HOME				_T("뚺렜긖귽긣귩둎궖귏궥갃")
#define TOOLTIP_OPTION				_T("긆긵긘깈깛귩먠믦궢귏궥갃")
#define MSGBOX_TITLE				_T("Lost Eden")
#define COUNTRY_CODE				_T("JAPAN")
// #define LAUNCHERWEB_NAME			_T("http://losteden.jp/launcher.php") // 원본

//=====================================================================================

#define PROGRESS_TYPE_DOWNLOAD		1
#define PROGRESS_TYPE_UNZIP			2
#define PROGRESS_TYPE_START			3

#define FTP_USER					_T("zemi4story")
#define FTP_PASS					_T("dhkdeoqkr")

#define OPTION_HI					2
#define OPTION_MID					1
#define OPTION_LOW					0
// 그래픽 설정
//#define REG_WINDOW			_T("WindowedMode")
//#define REG_SHADER			_T("UseShader")
//#define REG_CHARACTER			_T("ObjDETAIL")		// 캐릭터 품햨E
//#define REG_PAPERMATRIX		_T("MapDETAIL")		// 햨EE품햨E
//#define REG_BACK				_T("TextureDETAIL")	// 배경시야


typedef struct tagPATCHFILE
{
	DWORD m_dwVersion;
	CString m_strPath;
	CString m_strName;
	DWORD m_dwSize;
}PATCHFILE, *LPPATCHFILE;

typedef queue<LPPATCHFILE>			QPATCHFILE, *LPQPATCHFILE;

typedef struct tagD3DDEVICE
{
	LPDIRECT3D9 m_pD3D;
	D3DPRESENT_PARAMETERS m_vPRESENT;
}D3DDEVICE, *LPD3DDEVICE;

// CStoryDlg 큱E?상자
class CStoryDlg : public CDialog
{
// 생성
public:
	CStoryDlg(CWnd* pParent = NULL);	// 표준 생성자
	~CStoryDlg();

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	DWORD OnReceive(CPacket * pPacket);
	void SessionStart(CString strIp, DWORD dwPort);
	void Say(CPacket * pPacket);

// 큱E?상자 데이터
	enum { IDD = IDD_STORY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지퓖E
	virtual BOOL DestroyWindow();

public:
	void InitWeb();
	BOOL ReadRegistry();
	BOOL CopyRegistry();
	void ReadURLFile();
	void ReadTextFile();
	void LoadSkin();
	void SetControlPos();	// 컨트롤 위치 지정

	CString LoadString(int nID);
	void DownloadEnd();
	static DWORD WINAPI _Download(LPVOID lpParam);
	DWORD Download();
	void DownloadFinish();
	void StartGame();

	BOOL CreateDirectoryEx(CString strPath);
	void Progress(CString strName, DWORD dwSize, DWORD dwCurrent, __int64 nTotal, BYTE bType);
	int Unzip(LPCTSTR strZip, LPCTSTR strDirectory);
	void VerifyDirectory(LPCTSTR target, LPCTSTR path, BOOL bHaveFile);
	void CheckPatch();
	void RemoveDownloadDirectory();
	void DeleteDirectory(LPCTSTR strPath);
	BOOL SetVersion(DWORD dwVersion);

	DWORD OnCT_PATCH_ACK(CPacket * pPacket);
	void SendCT_PATCH_REQ();
	void SendCT_PATCHSTART_REQ();

// 구?E
protected:
	HICON m_hIcon;

	HBITMAP		hBmp;
	CBitmap		m_bSkin;
	BYTE		sFlag;

	CWebBrowser2 m_webCtrl;

	CString		m_strIP;
	WORD		m_wPort;

	CString m_strUserID;
	CString m_strUserPW;
	DWORD	m_dwChannelling;

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
	BOOL		m_bFlash; // 버튼 깜박임
	BOOL		m_bProgressing;

	CString		m_strHompageURLForLuancher;
	CString		m_strNoticeURLForLuancher;
	CString		m_strSubkey;

	// 메시햨E맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CProgressCtrl m_progressCurrent;
	CProgressCtrl m_progressTotal;
	CHoverButton m_bHome;
	CHoverButton m_bSetting;
	CHoverButton m_bOK;
	CHoverButton m_bCANCEL;

	afx_msg void OnBnClickedButtonHomepage();
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();


	// 그래픽 설정 컖E?변펯E
	CPlaySetting* m_pdlgPlaySetting;
	DWORD m_dwWindowMode;
	DWORD m_dwShaderMode;
	DWORD m_dwCharMode;
	DWORD m_dwPaperMode;
	DWORD m_dwBackMode;

	static ULONGLONG m_lVIDEOMEM;
	static ULONGLONG m_lSYSMEM;	

	D3DDEVICE m_Device;

	BOOL WriteRegistry();
	void InitCAPS();
	BOOL InitDevice();
	void CollectResolution(); // 해상도 검퍊E

};
