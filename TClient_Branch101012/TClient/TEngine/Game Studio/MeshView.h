#pragma once
#include "GridView.h"
#include "MeshDoc.h"


// CMeshView 뷰입니다.

class CMeshView : public CGridView
{
	DECLARE_DYNCREATE(CMeshView)

protected:
	CMeshView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMeshView();

public:
	CMeshDoc* GetDocument();

protected:
	virtual void DrawContent();
	virtual void InitCamera();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnGridCamreset();
};

inline CMeshDoc* CMeshView::GetDocument()
   { return (CMeshDoc *) m_pDocument;}
