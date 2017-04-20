// MainFrm.h : CMainFrame 클래스의 인터페이스
//


#pragma once

#include "STType.h"
#include "TabGroup.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 특성
public:

// 작업
public:

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

// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()


public:
	CDialogBar	m_wndSideBar;
	CTabGroup	m_dlgSideBar;

	CString m_strCurDirectroy;

	CString m_strMIP;
	CString m_strMPort;
	CString m_strMUser;
	CString m_strSIP;
	CString m_strSPort;

	/////////////////////////////////////////////////////////////////////////////////////
	// WM_USER_MSG
	afx_msg LRESULT OnSessionMSG( WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnSessionClose( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPushList( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCharinfoView( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSessionlistView( WPARAM wParam, LPARAM lParam);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// string -> byte
	DWORD strTobyte(CString str);

	// Protocol Base
	BYTE LoadProtocolBase();
	void SplitBase(CString strB);
	MAPPROTOCOL m_mapBase;

	// Protocol
	BYTE LoadProtocol(CString fileName);
	DWORD SplitProtocol(CString strP);
	MAPPROTOCOL m_mapProtocol;

	// Protocol Type
	void SplitType(DWORD value, CString strT);
	MAPPROTOCOLTYPE m_mapType;	
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////	
	// Get CTabGroup
	CTabGroup* GetTabGroup();
	/////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////////////////////////
	// Manager Svr Connect / DisConnect
	afx_msg void OnManagerConnect(); 
	afx_msg void OnManagerDisconnect();
	// Manager Connect / DisConnect Enable
	BOOL m_ConnectEnabel;
	void SetEnableManagerConnect(BOOL ConnectEnable);
	BOOL GetEnableManagerConnect();
	afx_msg void OnUpdateManagerConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateManagerDisconnect(CCmdUI *pCmdUI);
	
	// Session / Macro Window View
	afx_msg void OnWindowSession(); 
	afx_msg void OnWindowMacro();
	// Set Check Window
	BOOL m_CheckSession;
	BOOL m_CheckMacro;
	void SetCheckWindow(int nID);
	// Check Session / Macro Window View
	afx_msg void OnUpdateWindowSession(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWindowMacro(CCmdUI *pCmdUI);
	/////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////
	// Mecro File New / Save / Load
	afx_msg void OnFileNewmacro();	
	afx_msg void OnFileMacrosave();
	afx_msg void OnFileMacroload();
	afx_msg void OnUpdateFileNewmacro(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileMacrosave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileMacroload(CCmdUI *pCmdUI);	
	virtual void Serialize(CArchive& ar);
	/////////////////////////////////////////////////////////////////////////////////////	
};


