#pragma once
#include "GridDoc.h"
#include "SetBackIMGDlg.h"


#define VIEWPORT_TOP			((BYTE) 0)
#define VIEWPORT_FRONT			((BYTE) 1)
#define VIEWPORT_LEFT			((BYTE) 2)
#define VIEWPORT_PERSPECTIVE	((BYTE) 3)
#define VIEWPORT_NAVIGATE		((BYTE) 4)


enum GSVIEW_MODE
{
	GSVIEW_NONE = 0,
	GSVIEW_SFX,
	GSVIEW_MAP,
	GSVIEW_COUNT
};

// CGridView 뷰입니다.

class CGridView : public CView
{
	DECLARE_DYNCREATE(CGridView)

protected:
	CGridView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGridView();

protected:
	LPDIRECT3DTEXTURE9 m_pBackIMGTEX;
	D3DXIMAGE_INFO	m_InfoBackIMG;
	
	CD3DLight m_LightCenter;
	CD3DLight m_LightRight;
	CD3DLight m_LightTop;

	CD3DCamera m_Camera;
	CD3DFont m_ViewType;
	CD3DFont m_AxisType;

	CPoint m_point;

	CString m_strViewType;
	CFont m_FontAxis;
	CFont m_Font;
	FLOAT m_fGrid;

	BYTE m_bApplyMode;
	BYTE m_bCamMode;
	BYTE m_bGrid;

	CSetBackIMGDlg* m_pBackIMGDlg;
	BACKIMAGE m_BackIMG;
	BYTE m_bBackIMG;

public:
	BYTE m_bViewPort;
	DWORD m_dwFPS;
	UINT m_nFrameMode;

public:
	virtual void DeleteForRestore();
	virtual void Restore();
	virtual void Render();
	void InitFOV(FLOAT fFOV);
	

protected:
	virtual BYTE IsPlay();

	virtual void DrawContent();
	virtual void InitCamera();
	

	virtual void DrawGrid();
	virtual void DrawLine(
		D3DXVECTOR3 vFrom,
		D3DXVECTOR3 vTo,
		DWORD dwColor);

	virtual void ZoomView( FLOAT fZoom);
	virtual void RotateView(
		FLOAT fRotX,
		FLOAT fRotY,
		FLOAT fRotZ);

	virtual void MoveView(
		FLOAT fMoveX,
		FLOAT fMoveY,
		FLOAT fMoveZ);

protected:
	void FitCamDist( FLOAT fHeight);
	void ResetCam( FLOAT fGrid);
	void DisableLight();
	void EnableLight();

	void GetGridBoundary(
		LPD3DXVECTOR3 pMinPoint,
		LPD3DXVECTOR3 pMaxPoint);

	FLOAT GetFitGrid();

public:
	void DrawBackIMG(void);
	void DrawFPSnParticle();

public:
	virtual void OnDraw(CDC* pDC);
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGridShow();
	afx_msg void OnUpdateGridShow(CCmdUI *pCmdUI);
	afx_msg void OnGridTop();
	afx_msg void OnUpdateGridTop(CCmdUI *pCmdUI);
	afx_msg void OnGridPerspective();
	afx_msg void OnUpdateGridPerspective(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridNavigate(CCmdUI *pCmdUI);
	afx_msg void OnGridLeft();
	afx_msg void OnUpdateGridLeft(CCmdUI *pCmdUI);
	afx_msg void OnGridFront();
	afx_msg void OnUpdateGridFront(CCmdUI *pCmdUI);
	afx_msg void OnGridCamreset();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBackimgLoad();
	afx_msg void OnBackimgShow();
	afx_msg void OnUpdateBackimgShow(CCmdUI *pCmdUI);
	afx_msg void OnSetBackIMG();
	afx_msg void OnUpdateSetBackIMG(CCmdUI *pCmdUI);
	FLOAT CalcBackIMGDist();
};
