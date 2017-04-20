#pragma once

#include "TPathWndBase.h"


// CTQuestPathView 뷰입니다.

class CTQuestPathView : public CScrollView
{
	DECLARE_DYNCREATE(CTQuestPathView)

public:
	CTQuestPathDoc* GetDocument() const;

public:
	CTPathWndBase *m_vTWND[TPATHWND_COUNT];
	CSize m_vTWNDSIZE[TPATHWND_COUNT];

public:
	void EnableTPATHWND( int nIndex);
	void UpdateDATA(
		int nIndex,
		int nType,
		LPARAM lParam);

protected:
	CTQuestPathView();
	virtual ~CTQuestPathView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual BOOL Create(
		LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd,
		UINT nID,
		CCreateContext* pContext = NULL);

protected:
	virtual void OnDraw( CDC *pDC);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd( CDC *pDC);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // TQuestPathView.cpp의 디버그 버전
inline CTQuestPathDoc* CTQuestPathView::GetDocument() const
   { return reinterpret_cast<CTQuestPathDoc *>(m_pDocument); }
#endif
