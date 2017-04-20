// TSessionView.h : iCTSessionView 클래스의 인터페이스
//


#pragma once
#include "afxcmn.h"
#include "afxwin.h"


class CTSessionView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CTSessionView();
	DECLARE_DYNCREATE(CTSessionView)

public:
	enum{ IDD = IDD_SVRTESTER_FORM };

// 특성
public:
	CTSessionDoc* GetDocument() const;

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
	virtual ~CTSessionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	///////////////////////////////////////////////////////////////////////////////////
	// Manager Connect / DisConnect
	void ManagerConnect(CString strIP, CString strPort, CString strUser, CString strPass); 
	void ManagerDisConnect(); 
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	// Sever Connect / DisConnect
	void Connect(CString strIP, CString strPort, CString strThread);
	void DisConnect();

	// Protocol List Drag
	CRect GetEditPosition();	
	void SetEditProtocol(CString str);

	// All Check
	BOOL m_bAllFlag;
	CButton m_AllCheck;	

	afx_msg void OnBnClickedCheckAll();	

	// Protocol Send
	afx_msg void OnBnClickedButtonSend();
	
	// Receive Message Value
	CListCtrl m_listResult;

	void InitList();	
	void InsertList(CTachyonSession* pSession);
	void ClearList();
	afx_msg void OnBnClickedListClear();

	// Thread ComboBox
	CComboBox m_comboThread;

	// Init ComboBox
	void SetComboThread(DWORD bID); 
	// ComboBox Current Thread Number Get
	int GetDataCurrentComboThread(); 
	int GetIndexCurrentComboThread();
	int GetIndexComboThread(DWORD dwID);
	int GetDataComboThread(int index);
	afx_msg void OnCbnSelchangeComboThread();

	void SetEditConnect(); // Connect Edit Box Set
	///////////////////////////////////////////////////////////////////////////////////			
};

#ifndef _DEBUG  // TSessionView.cpp의 디버그 버전
inline CTSessionDoc* CTSessionView::GetDocument() const
   { return reinterpret_cast<CTSessionDoc*>(m_pDocument); }
#endif