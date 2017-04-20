#pragma once
#include "GridView.h"


class CGridSplitterWnd : public CSplitterWnd
{
public:
	CWnd *GetActiveView() { return GetActivePane(); };
	void SetColumnCount( int nCount) { m_nCols = nCount; };
	void SetRowCount( int nCount) { m_nRows = nCount; };

public:
	CGridSplitterWnd() {};
	virtual ~CGridSplitterWnd() {};

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

class CGridFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGridFrame)
protected:
	CGridFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGridFrame();

public:
	void ResetCamera();
	CWnd *GetActiveView();
	void SetFPS( DWORD fFPS );

public:
	CGridView* m_pGridView[2][2];

	BOOL m_bFrameReady;
	BOOL m_bFullScreen;

protected:
	virtual void ResizeViews();

protected:
	CGridSplitterWnd m_wndSplitter;

protected:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridToggle();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
