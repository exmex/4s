// 4StoryDlg.h : Змґх ЖДАП
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "HoverButton.h"
#include "GameSetting.h" // °ФАУ јіБ¤ Гў ¶зїм±в

#include <d3d9.h>
#include <dxdiag.h>
#include <d3dx9.h>

#pragma comment(lib,"dxguid.lib")

typedef enum TMP_HACK_DETECT_TYPE
{
	TMP_HACK_NONE = 0,
	TMP_HACK_FOUND,
	TMP_HACK_TIMEOUT,
	TMP_HACK_INVALID_PE
} *LPTMP_HACK_DETECT_TYPE;

#define WM_RESTART_PATCH			(WM_USER + 109)
#define WM_STOP_DOWNLOAD			(WM_USER + 119)
#define WM_CLOSE_SESSION			(WM_USER + 120)

#define APP_EXT						_T(".exe")
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

#define DEFAULT_SCREEN_X			1024
#define DEFAULT_SCREEN_Y			768
//=====================================================================================
// °ў±№ №цАьё¶ґЩ єЇ°жµЗѕЯ ЗПґВ ЗЧёсµй
// String Table АЗ Error Message єЇ°ж
#define HOMEPAGE_NAME				_T("http://www.4sonline.ru/")
#define LAUNCHERWEB_NAME			_T("http://launcher.4sonline.ru")
#define REG_COUNTRY					_T("_RU")
#define TOOLTIP_START				_T("Начать игру")
#define TOOLTIP_CLOSE				_T("Выйт?из игры")
#define TOOLTIP_HOME				_T("Сайт игры")
#define TOOLTIP_OPTION				_T("Установить")
#define COUNTRY_CODE				_T("RU")

#define STRING_CONNECT_FAIL			_T("Не удалос?соединиться ?сервером")
#define STRING_DOWNLOAD_FAIL		_T("Возникла ошибка пр?обновлении")
#define STRING_FTP_FAIL				_T("Не удалос?загрузит?файл")
#define STRING_THREAD_FAIL			_T("Не удалос?создат?поTок")
//#define TOOLTIP_START				_T("Начать игру")
//#define TOOLTIP_CLOSE				_T("Выйт?из игры")
//#define TOOLTIP_HOME				_T("На главну?)
//#define TOOLTIP_OPTION				_T("Установить")
//#define COUNTRY_CODE				_T("RU")
//=====================================================================================

#define PROGRESS_TYPE_DOWNLOAD		1
#define PROGRESS_TYPE_UNZIP			2
#define PROGRESS_TYPE_START			3

#define FTP_USER					_T("zemi4story")
#define FTP_PASS					_T("dhkdeoqkr")

#define OPTION_LOW					0
#define OPTION_MID					1
#define OPTION_HI					2

// ±Ч·ЎЗИ јіБ¤
//#define REG_WINDOW			_T("WindowedMode")
//#define REG_SHADER			_T("UseShader")
//#define REG_CHARACTER			_T("ObjDETAIL")		// ДіёЇЕН З°Бъ
//#define REG_PAPERMATRIX		_T("MapDETAIL")		// БцЗь З°Бъ
//#define REG_BACK				_T("TextureDETAIL")	// №и°жЅГѕЯ


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

// CStoryDlg ґлИ­ »уАЪ
class CStoryDlg : public CDialog
{
	// »эјє
public:
	CStoryDlg(CWnd* pParent = NULL);	// ЗҐБШ »эјєАЪ
	~CStoryDlg();

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	DWORD OnReceive(CPacket * pPacket);
	void SessionStart(CString strIp, DWORD dwPort);
	void Say(CPacket * pPacket);

	// ґлИ­ »уАЪ µҐАМЕН
	enum { IDD = IDD_STORY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV Бцїш
	virtual BOOL DestroyWindow();

public:
	void InitWeb();
	BOOL ReadRegistry();
	BOOL CopyRegistry();
	void ReadURLFile();
	void ReadTextFile();
	void LoadSkin();
	void SetControlPos();	// ДБЖ®·С А§ДЎ БцБ¤

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

	// ±ёЗц
protected:
	HICON m_hIcon;

	HBITMAP		hBmp;
	CBitmap		m_bSkin;
	BYTE		sFlag;

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
	BOOL		m_bFlash; // №цЖ° ±ф№ЪАУ
	BOOL		m_bProgressing;

	CString		m_strHompageURLForLuancher;
	CString		m_strNoticeURLForLuancher;
	CString		m_strSubkey;

	// ёЮЅГБц ёК ЗФјцё¦ »эјєЗЯЅАґПґЩ.
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


	// ±Ч·ЎЗИ јіБ¤ °ь·Г єЇјц
	DWORD m_dwWindowMode;
	DWORD m_dwShaderMode;
	DWORD m_dwCharMode;
	DWORD m_dwPaperMode;
	DWORD m_dwBackMode;

	static ULONGLONG m_lVIDEOMEM;
	static ULONGLONG m_lSYSMEM;

	D3DDEVICE m_Device;

	BOOL WriteRegistry();
	void InitCAPS(); //ДДЗ»ЕН »зѕз °Л»з
	BOOL InitDevice();
	void CollectResolution(); // ЗШ»уµµ °Л»з

	void	ShowSettingFrame();
	BOOL	CheckIsFirst();

	CString GetLocalString();
};
