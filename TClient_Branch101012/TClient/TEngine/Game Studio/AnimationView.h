#pragma once
#include "GridView.h"
#include "AnimationDoc.h"


// CAnimationView 뷰입니다.

class CAnimationView : public CGridView
{
	DECLARE_DYNCREATE(CAnimationView)

protected:
	CAnimationView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAnimationView();

public:
	CAnimationDoc* GetDocument();

protected:
	D3DXVECTOR3 m_vMove;

protected:
	virtual void DrawContent();
	virtual void InitCamera();
	virtual BYTE IsPlay();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnGridCamreset();
};

inline CAnimationDoc* CAnimationView::GetDocument()
   { return (CAnimationDoc *) m_pDocument;}
