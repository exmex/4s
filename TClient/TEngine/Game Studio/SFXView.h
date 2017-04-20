#pragma once
#include "GridView.h"
#include "SFXDoc.h"
#include "FrameSkip.h"


// CSFXView 뷰입니다.

class CSFXView : public CGridView
{
	DECLARE_DYNCREATE(CSFXView)

protected:
	CSFXView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSFXView();

public:
	CSFXDoc* GetDocument();

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

public :

	CFrameSkip m_FrameRender;

};

inline CSFXDoc* CSFXView::GetDocument()
   { return (CSFXDoc *) m_pDocument;}
