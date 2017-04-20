#pragma once
#include "OBJTypeDoc.h"
#include "ATTRPage.h"
#include "OBJPage.h"


// COBJTypeView 뷰입니다.

class COBJTypeView : public CView
{
	DECLARE_DYNCREATE(COBJTypeView)

protected:
	COBJTypeView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~COBJTypeView();

public:
	COBJTypeDoc* GetDocument();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImageList m_listTabImages;

	CPropertySheet m_wndSHEET;
	CATTRPage m_wndATTR;
	COBJPage m_wndOBJ;

public:
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

inline COBJTypeDoc* COBJTypeView::GetDocument()
   { return (COBJTypeDoc *) m_pDocument;}
