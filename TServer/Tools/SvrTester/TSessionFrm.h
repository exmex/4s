// TSessionFrm.h : CTSessionFrm 클래스의 인터페이스
//


#pragma once

#include "TSessionDlg.h"

class CTSessionFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTSessionFrm)
public:
	CTSessionFrm();

// 특성
public:

// 작업
public:

// 재정의
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CTSessionFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void ActivateFrame(int nCmdShow = -1);
public:
	CDialogBar		m_wndSideBar;
	CTSessionDlg	m_dlgSideBar;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	////////////////////////////////////////////////////////
	// Get Session Dlg
	CTSessionDlg* GetTSessionDlg();
    ////////////////////////////////////////////////////////
};
