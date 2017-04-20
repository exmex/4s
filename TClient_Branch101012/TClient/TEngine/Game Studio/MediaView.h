#pragma once
#include "MediaDoc.h"


// CMediaView 뷰입니다.

class CMediaView : public CView
{
	DECLARE_DYNCREATE(CMediaView)

protected:
	CMediaView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMediaView();

public:
	CMediaDoc* GetDocument();

	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnDestroy();
};

inline CMediaDoc* CMediaView::GetDocument()
   { return (CMediaDoc *) m_pDocument;}
