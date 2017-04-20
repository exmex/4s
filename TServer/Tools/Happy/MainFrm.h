// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "HappySplitter.h"
#include "PatchBar.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	CDocument * GetFirstDocument();
	void ShowToolBar(int nIndex);
	void GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort);

	void GetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort);
	void SetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort);
// 특성
public:
	CHappySplitter m_wndSplitter;
	int m_nGroup;
	int m_nTree;
	int m_nList;
	int m_nPatch;
	int m_nListPatch;

	///////////////////////////////////////////////////////////
	// 현승룡 Graph
	BYTE m_bGraph;
	int m_nGraph; 
	BYTE GetCurView();
	void SetCurView(BYTE bCur);
	void SetGraphButton(BYTE bGraph);	
	
	BYTE GetGraphCheck(); // 현승룡 Graph
	void SetGraphCheck();

	// 현승룡 경보음, 서비스 전체보기, 서비스 시작/정지 버튼 활성화/비활성화
	BYTE m_bAllView;
	BYTE m_bAlert;
	BYTE m_bService;
	BYTE m_bAutoStart;
	BYTE m_bServiceDataClear;
	BYTE m_bExportHappyData;
	BYTE m_bChatBanList;

	DWORD m_dwAutoNormal;
	DWORD m_dwAutoPush;

	void SetAllView(BYTE bAllView);
	void SetAlertButton(BYTE bAlert);
	void SetServiceButton(BYTE bService);
	void SetAutoStart(BYTE _bAutoStart);
	void SetServiceDataClear(BYTE bView);
	void SetExportHappyData(BYTE bView);
	void SetChatBanList(BYTE bView);

	// 현승룡 유저관리 활성화/비활성화
	BYTE m_bLog;
	BYTE m_bUserWeb;
	BYTE m_bGMTool;

	void SetServiceLog(BYTE bLog);
	void SetUserWeb(BYTE bWeb);
	void SetGMTool(BYTE bGMTool);

	void SetLogTimer(BYTE bResult, DWORD dwTime);

	void SetAllControlEnable(BYTE bEnable, BYTE bServiceChange = TRUE );
	// 현승룡 Machine
	int m_nMachine;
	int m_nMachineGraph;
	///////////////////////////////////////////////////////////

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CPatchBar	m_wndPatchBar;

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateServerConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServerDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceAlert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceAllView(CCmdUI *pCmdUI);	
	void SetPatchBarCompareVisible(BYTE bCompare); // PatchBar의 Compare 버튼 Visible Setting	
	void SetPatchBarPatchUpdateVisible(BYTE bPatchUpdate);// PatchBar의 PatchUpdate 버튼 Visible Setting
	afx_msg void OnUpdateServiceUpload(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonCompare(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonPatchup(CCmdUI *pCmdUI);	
	afx_msg void OnUpdateCuruserGraph(CCmdUI *pCmdUI);// 현승룡 서버 그래프 활성화/비활성화	
	afx_msg void OnUpdateAnnounce(CCmdUI *pCmdUI);// 현승룡 공지사항 활성화/비활성화
	afx_msg void OnUpdateUserkickout(CCmdUI *pCmdUI);// 현승룡 유저 강제퇴장 활성화/비활성화	
	afx_msg void OnUpdateUsermove(CCmdUI *pCmdUI);// 현승룡 유저 위치이동 활성화/비활성화	
	afx_msg void OnUpdateMonsterspawn(CCmdUI *pCmdUI);// 현승룡 몬스터 관리 활성화/비활성화	
	afx_msg void OnUpdateServcieLog(CCmdUI *pCmdUI);// 현승룡 로그 기록 활성화/비활성화	
	afx_msg void OnUpdateUserweb(CCmdUI *pCmdUI);// 현승룡 유저관리 웹 활성화/비활성화	
	afx_msg void OnUpdateGmtool(CCmdUI *pCmdUI);// 현승룡 운영자 툴 활성화/비활성화
	afx_msg void OnUpdateServiceAutoStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceDataClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateExportHappydata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUseradminChatbanlist(CCmdUI *pCmdUI);
};


