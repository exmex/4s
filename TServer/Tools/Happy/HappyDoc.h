// HappyDoc.h : CHappyDoc 클래스의 인터페이스
//


#pragma once

#include <afxinet.h>

#include "HSession.h"
#include "DlgUploadProgress.h"
#include "DlgPatchProgress.h"
#include "ServiceTree.h"
#include "ServiceList.h"
#include "AutoPatch.h"
#include "PatchList.h"
#include "ServiceGraph.h" // 현승룡 Graph
#include "Machine.h" // 현승룡 Machine
#include "MachineGraph.h"
#include "HappyGroup.h"
#include "MonsterSpawn.h"
#include "DlgServiceLog.h"
#include "RetDlg.h"
#include "PreVersion.h"

#include "GMToolDlg.h"

class CHappyDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CHappyDoc();
	DECLARE_DYNCREATE(CHappyDoc)

public:
	void ClearMachine();
	void ClearGroup();
	void ClearSvrType();
	void ClearService();
	void ClearPlatform();
	void ClearPatchFiles();
	void ClearServiceGraph();
	void ClearData();
	void DeletePatchProgress();
	BYTE ServiceUpload();
	void NotifyUploadEnd(BYTE bRet);
	void UpdateUploadProgress();
	BYTE IsSamePatchFile(LPPATCHFILE pFile); // 같은 이름의 파일이 없으면 FALSE 리턴
	void AddPatchFile(LPPATCHFILE pFile);
	void OnPatchUpload();
	BOOL CreateTempDirectory(CString &strFullPath);
	BOOL ZipPatchFile(LPCTSTR strTargetPath, LPPATCHFILE pPatchFile);
	BOOL ZipPatchFiles(LPCTSTR strTargetPath);
	BOOL UploadPatchFiles(LPCTSTR strSourcePath);
	void RetrieveZipFile(LPCTSTR strTargetPath, LPPATCHFILE pPatchFile);
	void DeleteDirContents(LPCTSTR strPath);
	BOOL CreateDirectoryEx(CString strPath);
	BOOL VerifyRemoteDirectory(HINTERNET hConnect, LPPATCHFILE pPatch);
	BOOL CreateFtpDirectoryEx(HINTERNET hConnect, CString strPath);
	BYTE UploadPatchFile(HINTERNET hConnect, LPPATCHFILE pPatch, int& nSentTotal);
	CWnd * GetPatchListView();
	CWnd * GetServiceListView();
	void SaveLogData();
	BYTE NewUploadPatchFiles(LPCTSTR strSourcePath);
	BYTE CheckBeep();
	void CloseAllDialog();
	CString GetSvrName(BYTE bGroup,BYTE bSvrType,BYTE bSvrID);
	void InitEventTitle();
	void ClearLuckyEvent();
	void AddLuckEvent(LUCKYEVENT stLuckyEvent);
	void DelLuckyEvent(WORD wID);
	LPLUCKYEVENT GetLuckyEvent(WORD wID);
	void AddTournament(LPTOURNAMENT pTNT);
	void DelTournament(WORD wID);
	void ClearTournament();
	LPTOURNAMENT GetTournament(WORD wID);
	LPTOURNAMENT GetTournament(CString strTitle);
	void ChangeXListCtrl(DWORD dwCtrlID,int nRow, int nCol, int nCtrlType);
	int GetClassIndex(DWORD dwClass);



	

// 특성
public:
	CHSession	m_session;
	BYTE	m_bAlertOnFail;
	BYTE	m_bViewAllService;
	BYTE	m_bErrorStatus;
	BYTE	m_bServiceAutoStart; // 서비스 Stop 시에 자동으로 Start
	BYTE	m_bBtnAutoStart; //툴바 자동시작 버튼의 상태값(푸쉬상태인가 아닌가)

	DWORD	m_dwIP;
	DWORD	m_dwPort;
	CString m_strIP;
	CString m_strUserID;
	CString m_strPasswd;

	DWORD	m_dwPatchIP;
	CString m_strPatchUserID;
	CString m_strPatchPasswd;
	DWORD	m_dwPatchPort;
	BYTE	m_bSameFileExist;
	
	CString m_strFtpAddr;
	CString m_strFtpUser;
	CString m_strFtpPasswd;
	WORD	m_wFtpPort;

	MAPMACHINE	m_mapMachine;
	MAPGROUP	m_mapGroup;
	MAPSERVICE	m_mapService;
	MAPSVRTYPE	m_mapSvrType;
	VPATCHFILE	m_vPatchFiles;

	// 현승룡 Machine
	MAPPLATFORM m_mapPLATFORM;

	// 현승룡 Service Graph
	MAPSERVICEGRAPH m_mapSERVICEGRAPH;

	DWORD m_dwSelectedService;
	CDlgUploadProgress  m_dlgUploadProgress;
	CPreVersion*		m_dlgPreVersion;
	CFile m_fileUpload;
	DWORD m_dwUploadedSize;
	DWORD m_dwPatchSize;

	CString m_strNextFolder;
	CString m_strAdminWeb;
	MAPBYTESTRING m_mapEventName;
	MAPDWORDSTRING m_mapEventSvr;
	MAPEVENTINFO m_mapEventInfo;
	MAPTCASHITEM m_mapCashItem;
	MAPLUCKYEVENT m_mapLuckEvent;
	MAPTOURNAMENT m_mapTournament;
	
	BYTE	m_bPatchType;
private:
	HANDLE		m_hThreadPatchUpload;
	static DWORD WINAPI _ThreadPatchUpload(LPVOID lpParam);
	DWORD ThreadPatchUpload();

	BYTE	m_bPatchUploading;
	CDlgPatchProgress * m_pPatchProgress;

// 작업
public:
	LPHAPPYSERVICE FindService(DWORD dwID);
	BYTE IsGroup();
	void GroupStart();
	void GroupStop();
	void DisconnectServer();

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	void OnReceive(CPacket * pPacket);
	void SessionStart(CString strIp, DWORD dwPort);
	void Say(CPacket * pPacket);

	void OnCT_OPLOGIN_ACK(CPacket * pPacket);
	void OnCT_SVRTYPELIST_ACK(CPacket * pPacket);
	void OnCT_MACHINELIST_ACK(CPacket * pPacket);
	void OnCT_GROUPLIST_ACK(CPacket * pPacket);
	void OnCT_SERVICESTAT_ACK(CPacket * pPacket);
	void OnCT_SERVICECHANGE_ACK(CPacket * pPacket);
	void OnCT_SERVICEDATA_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOADSTART_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOAD_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOADEND_ACK(CPacket * pPacket);
	void OnCT_UPDATEPATCH_ACK(CPacket * pPacket);
	void OnCT_AUTHORITY_ACK(CPacket * pPacket); // 현승룡 CT_AUTHORITY_ACK
	void OnCT_PLATFORM_ACK(CPacket * pPacket); // 현승룡 CT_PLATFORM_ACK
	void OnCT_MONSPAWNFIND_ACK(CPacket * pPacket); // 현승룡 CT_MONSPAWNFIND_ACK
	void OnCT_USERPROTECTED_ACK(CPacket * pPacket);		
	void OnCT_SERVICEAUTOSTART_ACK(CPacket* pPacket);	// 서비스 Stop 시 자동으로 Start 하는지를 수신.
	void OnCT_CHATBAN_ACK(CPacket* pPacket); // 채팅금지요청의 응답.
	void OnCT_ITEMFIND_ACK(CPacket* pPacket);
	void OnCT_ITEMSTATE_ACK(CPacket* pPacket);
	void OnCT_CHATBANLIST_ACK(CPacket* pPacket);
	void OnCT_CASTLEINFO_ACK(CPacket* pPacket);
	void OnCT_CASTLEGUILDCHG_ACK(CPacket* pPacket);
	void OnCT_EVENTCHANGE_ACK(CPacket* pPacket);
	void OnCT_EVENTLIST_ACK(CPacket* pPacket);
	void OnCT_CASHITEMLIST_ACK(CPacket* pPacket);
	void OnCT_EVENTQUARTERUPDATE_ACK(CPacket* pPacket);
	void OnCT_EVENTQUARTERLIST_ACK(CPacket* pPacket);
	void OnCT_TOURNAMENTEVENT_ACK(CPacket* pPacket);
	void OnCT_RPSGAMEDATA_ACK(CPacket* pPacket);
	void OnCT_PREVERSIONTABLE_ACK(CPacket* pPacket);	
	void OnCT_CMGIFT_ACK(CPacket* pPacket);
	void OnCT_CMGIFTLIST_ACK(CPacket* pPacket);

	void SendCT_OPLOGIN_REQ();
	void SendCT_SERVICESTAT_REQ();
	void SendCT_SERVICECONTROL_REQ(BYTE bStart);
	void SendCT_SERVICECONTROL_REQ(DWORD dwID, BYTE bStart);
	void SendCT_SERVICEUPLOADSTART_REQ(BYTE bMachine, CString strFile);
	void SendCT_SERVICEUPLOAD_REQ(BYTE data[], WORD wSize);
	void SendCT_SERVICEUPLOADEND_REQ(BYTE bCancel, BYTE data[], WORD wSize);
	void SendCT_UPDATEPATCH_REQ();

	void SendCT_ANNOUNCEMENT_REQ(DWORD nID, CString strAnnounce); // 현승룡 CT_ANNOUNCEMENT_REQ
	void SendCT_USERKICKOUT_REQ(CString strUser); // 현승룡 CT_USERKICKOUT_REQ
	void SendCT_USERMOVE_REQ(queue<CString>* pqStrUser, BYTE bWorld, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ); // 현승룡 CT_USERMOVE_REQ
	void SendCT_USERPOSITION_REQ(BYTE bWorld, queue<CString>* pqStrUser, CString strTarget); // 현승룡 CT_USERPOSITION_REQ
	void SendCT_MONSPAWNFIND_REQ(BYTE bGroupID, BYTE bChannel, WORD wMapID, WORD wSpawnID); // 현승룡 CT_MONSPAWNFIND_REQ
	void SendCT_MONACTION_REQ(BYTE bGroup, BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType,WORD wSpawnID);
	void SendCT_USERPROTECTED_REQ(CString strUserID, DWORD dwTime);
	void SendCT_CHARMSG_REQ(CString strName, CString strMsg);

	//////////////////////////////////////////////////////////////
	
	void SendCT_RECONNECT_REQ(DWORD _dwID);
	void SendCT_DISCONNECT_REQ(DWORD _dwID);
	void SendCT_SERVICEAUTOSTART_REQ(BYTE _bAutoStart); // 서비스 Stop 시 자동으로 Start 할지를 요청	
	void SendCT_CHATBAN_REQ(CString strName,WORD wMin,CString strReason); // 특정 캐릭터 채팅 금지 요청.
	void SendCT_SERVICEDATACLEAR_REQ(); // 서비스의 데이터(MAXUSER,ACTIVEUSER,STOPCOUNT,LATEST START,PEEK TIME) 초기화
	
	void SendCT_ITEMFIND_REQ(WORD wItemID,CString strName,BYTE bWorldID);
	void SendCT_ITEMSTATE_REQ(LPVITEMSTATE pVITEMSTATE,BYTE bWorldID);
	void SendCT_CHATBANLIST_REQ();
	void SendCT_CHATBANLISTDEL_REQ(DWORD dwID);
	void SendCT_CASTLEINFO_REQ(BYTE bWorld);
	void SendCT_CASTLEGUILDCHG_REQ(BYTE bWorld,WORD wCastleID,DWORD dwDefGuildID,DWORD dwAtkGuildID,__time64_t tTime);	
	void SendCT_CASTLEENABLE_REQ(BYTE bWorld,BYTE bStatus,DWORD dwSecond);

	void SendCT_EVENTCHANGE_REQ(BYTE bType, LPEVENTINFO pEvent);
	void SendCT_EVENTLIST_REQ();
	void SendCT_CASHITEMLIST_REQ();
	void SendCT_EVENTQUARTERUPDATE_REQ(BYTE bGroup, BYTE bType, LPLUCKYEVENT pLucky);
	void SendCT_EVENTQUARTERLIST_REQ(BYTE bGroup, BYTE bDay);
	
	void SendCT_TOURNAMENTEVENT_REQ(BYTE bGroupID, BYTE bType,LPTOURNAMENT pTNT,LPTNTCHAR pTNTCHAR = NULL);
	
	void SendCT_HELPMESSAGE_REQ(BYTE bGroupID, LPHELPMSG pHelpMsg);

	void SendCT_PREVERSIONTABLE_REQ();
	void SendCT_PREVERSIONUPDATE_REQ(WORD wMoveCount, WORD wDelCount, WORD wNewCount, LPVPATFILE pPatch);

	void SendCT_RPSGAMEDATA_REQ(BYTE bGroupID);
	void SendCT_RPSGAMECHANGE_REQ(BYTE bGroupID, LPVRPSGameData pData);

	void SendCT_CMGIFT_REQ(CString strTarget, WORD wGiftID, BYTE bGroup);
	void SendCT_CMGIFTLIST_REQ(BYTE bGroup);
	void SendCT_CMGIFTCHARTUPDATE_REQ(LPQCMGIFT pQList, BYTE bGroup);
public:
	// 현승룡 Graph 
	BYTE	m_bGraphCheck; 
	BYTE	m_curView; 

	BYTE	GetCurView();
	void	SetCurView(BYTE bCur);

	BYTE	m_bAuthority; // 매니저 권한
	DWORD	m_dwManagerID;

	// 현승룡 로그 기록
	BYTE	m_bLog;
	BYTE	m_bLogTime;

	CMonsterSpawn * m_pMonSpawn;

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CHappyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnServiceAlert();
	afx_msg void OnBnClickedButtonCompare();
	afx_msg void OnBnClickedButtonPatchup();
	afx_msg void OnBnClickedButtonPrepatchup();

	
public:
	TCHAR	m_szCurrentDirectory[_MAX_PATH];

	CGMToolDlg* m_pdlgGMTool;	
	SETUSERPOS m_setUSERPOS;
	void SavePos(USERPOS& _UserPos); // return 값이 true면 새로운 값 저장 false 면 기존값 수정
	void LoadPosListFromFile();
	void ClearPosList();
	void SavePosListToFile();
	void CopyFileNotZip();

	CRetDlg* m_pdlgRet;
	void ShowRetDlg(BYTE bShow,BYTE bRet, CString str);
};


