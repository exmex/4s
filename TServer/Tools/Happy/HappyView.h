// HappyView.h : iCHappyView 클래스의 인터페이스
//


#pragma once

#include "HappyDoc.h"
#include "HSession.h"
#include "DlgConnect.h"
#include "DlgUpload.h"
#include "GMToolDlg.h"
#include "ChatBanList.h"

class CHappyView : public CView
{
protected: // serialization에서만 만들어집니다.
	CHappyView();
	DECLARE_DYNCREATE(CHappyView)

public:
	CDlgConnect m_dlgConnect;
	CDlgUpload m_dlgUpload;
	CGMToolDlg *m_dlgGMTool;
	CChatBanList* m_dlgChatBanList;
	
// 작업

// 특성
public:
	CHappyDoc* GetDocument() const;

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CHappyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnServerConnect();
	afx_msg void OnServiceStart();
	afx_msg void OnServiceStop();
	afx_msg void OnServerDisconnect();
	afx_msg void OnServiceAllView();
	afx_msg void OnServiceUpload();
	afx_msg void OnCuruserGraph();
	afx_msg void OnGmtool();
	afx_msg void OnServcieLog();
	afx_msg void OnUserweb();
	afx_msg void OnServiceAutoStart();
	afx_msg void OnServiceDataClear();
	afx_msg void OnExportHappydata();
	afx_msg void OnUseradminChatbanlist();
};

#ifndef _DEBUG  // HappyView.cpp의 디버그 버전
inline CHappyDoc* CHappyView::GetDocument() const
   { return reinterpret_cast<CHappyDoc*>(m_pDocument); }
#endif

