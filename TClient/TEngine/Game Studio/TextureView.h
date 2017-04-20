#pragma once
#include "TextureDoc.h"


// CTextureView 뷰입니다.

class CTextureView : public CScrollView
{
	DECLARE_DYNCREATE(CTextureView)

public:
	static LPDIRECT3DVERTEXBUFFER9 m_pVertexSphere;
	static LPDIRECT3DINDEXBUFFER9 m_pIndexSphere;

	static LPDIRECT3DVERTEXBUFFER9 m_pVertexPlane;
	static LPDIRECT3DVERTEXBUFFER9 m_pVertexBox;

	static void ReleaseTextureMesh();
	static void InitTextureMesh();

	virtual void DeleteForRestore();
	virtual void Restore();

protected:
	CTextureView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTextureView();

public:
	CTextureDoc* GetDocument();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CD3DCamera m_Camera;
	CD3DLight m_Light;

	BYTE m_bCamMode;

public:
	void InitDraw();
	void Render();

protected:
	void SetState();

protected:
	virtual void OnDraw( CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy();
};

inline CTextureDoc* CTextureView::GetDocument()
   { return (CTextureDoc *) m_pDocument;}
