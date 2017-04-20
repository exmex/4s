#pragma once
#include "ImageDoc.h"


// CImageView 뷰입니다.

class CImageView : public CScrollView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageView();

public:
	CImageDoc* GetDocument();
	void Render();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void ResetScrollSize();

public:
	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
};

inline CImageDoc* CImageView::GetDocument()
   { return (CImageDoc *) m_pDocument;}
