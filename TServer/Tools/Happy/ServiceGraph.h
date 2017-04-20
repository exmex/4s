// 현승룡 ServiceGraph.h

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define MAX_ADD_SERVICE 20 // 최대 추가 가능 서비스 갯수
#define MAX_COL 6 // 최대 리스트 컬럼 수

// CServiceGraph 폼 뷰입니다.

class CServiceGraph : public CFormView
{
	DECLARE_DYNCREATE(CServiceGraph)

protected:
	CServiceGraph();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CServiceGraph();

public:
	enum { IDD = IDD_SERVICEGRAPH };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CDC MemDC; // 더블 버퍼링
	CBitmap *pOldBitmap, MemBitmap;
	BOOL m_bFlag;
	CPen lineColor[20];
	CPen lineTotalColor;

	CRect m_rectBack;
	CRect m_rectGraph;
	CRect m_rectUser;
	CRect m_rectPing;
	CRect m_rectList;

	DWORD m_dwMaxUser;
	DWORD m_dwMaxPing;

	BYTE  m_bWorldRelay;
	DWORD m_dwTotalUser;
	VDWORD m_vTOTALUSER;

	CListCtrl m_list;
	CImageList m_pImageList;

	CButton m_ClearButton;

	VDWORD m_vSERVICEGRAPH;

	void ClearGraph();
	void DrawBackGround();
	void DrawRect();

	void AddServiceGraph(DWORD dwID);
	
	void UpdateServiceGraph(DWORD dwID);
	void DrawService();
	void DrawGraph();
	BYTE DrawTotalUser();
	void DrawServiceList(BYTE bCnt, SERVICEGRAPH sGraph);
	BYTE DrawServiceGraph(BYTE bColor, VDWORD vCurUser, VDWORD vPing);

	int m_listDrag;
	BOOL m_listFlag;
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	
	afx_msg void OnLvnBegindragAddGraphList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClearButton();
	virtual void OnInitialUpdate();
};


