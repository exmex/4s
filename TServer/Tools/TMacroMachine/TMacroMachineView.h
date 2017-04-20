// TMacroMachineView.h : iCTMacroMachineView 클래스의 인터페이스
//


#pragma once
#include "afxwin.h"


class CTMacroMachineView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CTMacroMachineView();
	DECLARE_DYNCREATE(CTMacroMachineView)

public:
	enum{ IDD = IDD_TMACROMACHINE_FORM };

// 특성
public:
	CTMacroMachineDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현
public:
	virtual ~CTMacroMachineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	void ViewSessionCnt(BYTE bRet);
	void UpdateSessionCnt(WORD wCount);
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonProtocol();
	afx_msg void OnBnClickedButtonMacro();
	afx_msg void OnBnClickedButtonExcute();
	afx_msg void OnBnClickedButtonMacrotool();
	CButton m_chkRelayChatting;
};

#ifndef _DEBUG  // TMacroMachineView.cpp의 디버그 버전
inline CTMacroMachineDoc* CTMacroMachineView::GetDocument() const
   { return reinterpret_cast<CTMacroMachineDoc*>(m_pDocument); }
#endif

