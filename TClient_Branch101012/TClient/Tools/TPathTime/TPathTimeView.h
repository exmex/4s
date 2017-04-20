// TPathTimeView.h : interface of the CTPathTimeView class
//


#pragma once


class CTPathTimeView : public CScrollView
{
protected: // create from serialization only
	CTPathTimeView();
	DECLARE_DYNCREATE(CTPathTimeView)

// Attributes
public:
	CTPathTimeDoc* GetDocument() const;

// Operations
public:
	void UpdateView();

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTPathTimeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BYTE FindTSPOT(
		LPVTSPOT pVTSPOT,
		LPTSPOT pTSPOT);

	int SelectTSPOT(
		LPVTSPOT pTEXCEPT,
		LPVTSPOT pTSRC,
		int nPosX,
		int nPosY);

	void DrawTSPOT(
		CTPathTimeDoc *pDoc,
		CDC *pDEST,
		int nPosX,
		int nPosY);

	void DrawTSPOT(
		CDC *pMASK,
		CDC *pSRC,
		CDC *pDEST,
		LPTSPOT pTSPOT,
		int nPosX,
		int nPosY);

	void DrawTJOINTCursor(
		CDC *pDEST,
		int nPosX,
		int nPosY);

	void DrawTJOINT(
		CTPathTimeDoc *pDoc,
		CDC *pDEST,
		int nPosX,
		int nPosY);

	void RegulateTJOINTRect( CRect *pRECT);
	void DrawTJOINT(
		CDC *pDEST,
		CRect *pRECT);

	void DrawTJOINTEdge(
		CDC *pDEST,
		CRect *pRECT);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in TPathTimeView.cpp
inline CTPathTimeDoc* CTPathTimeView::GetDocument() const
   { return reinterpret_cast<CTPathTimeDoc*>(m_pDocument); }
#endif

