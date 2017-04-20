#pragma once


// CTMapView 뷰입니다.

class CTMapView : public CScrollView
{
	DECLARE_DYNCREATE(CTMapView)

private:
	static void _FindTestPath( LPTFINDINFO lParam);

public:
	CTQuestPathDoc* GetDocument() const;

public:
	CStatusBar* GetStatusBar();
	CPoint GetClientCoord(
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetT2DDotCoord(
		T2DPOINT lPoint,
		FLOAT fA,
		FLOAT fB,
		FLOAT fC);

	BYTE CheckBreak(
		LPTMAPIMG pTUNIT,
		T2DPOINT lPoint,
		WORD wLineS,
		WORD wLineE);

	BYTE GetT2DNORMALLINE(
		LPTMAPIMG pTUNIT,
		T2DPOINT lPoint,
		WORD wLineS,
		WORD wLineE,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE GetT2DNORMALLINE(
		T2DPOINT lPoint,
		T2DPOINT lLineS,
		T2DPOINT lLineE,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE GetT2DLINE(
		T2DPOINT lStart,
		T2DPOINT lEnd,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE GetT2DLINE(
		FLOAT fStartX,
		FLOAT fStartY,
		FLOAT fEndX,
		FLOAT fEndY,
		FLOAT *pA,
		FLOAT *pB,
		FLOAT *pC);

	BYTE CanMove(
		LPTMAPIMG pTUNIT,
		T2DPOINT lStart,
		T2DPOINT lEnd);

	BYTE GetWorldCoord(
		FLOAT& fPosX,
		FLOAT& fPosZ);

	BYTE GetUnitCoord(
		FLOAT& fPosX,
		FLOAT& fPosZ);

	WORD PickPathPoint(
		LPTMAPIMG pTUNIT,
		CPoint point,
		WORD wCurrentID,
		int nCurrentR,
		int nRadius);

	DWORD PickTLINE(
		LPTMAPIMG pTUNIT,
		CPoint point,
		int nWidth);

	void OnUpdateSCALE();
	void OnUpdateSIZE();
	void OnMapScale();

	void OnUpdatePOS(
		FLOAT fPosX,
		FLOAT fPosZ);

	void UpdateDATA( LPTTITEM pTTITEM);
	void GoToTPOINT();

	void FindTestPath( LPTFINDINFO lParam);
	void ClearTestPath( BYTE bClearPoint);

	void DrawTUNIT( CDC *pDC);
	void DrawTPOINT(
		CDC *pDC,
		FLOAT fPosX,
		FLOAT fPosZ,
		int nRadius,
		DWORD dwClr);

	void DrawTLINE(
		CDC *pDC,
		T2DPOINT lStart,
		T2DPOINT lEnd,
		int nWidth,
		DWORD dwClr);

	void DrawTLINE(
		CDC *pDC,
		LPTMAPIMG pTUNIT,
		DWORD dwLineID,
		int nWidth,
		DWORD dwClr);

	int GetHeight();
	int GetWidth();

public:
	MAPT2DPOINT m_mapTPATHPOINT;
	MAPDWORD m_mapTPATHLINE;

	T2DPOINT m_vPoint[2];
	WORD m_vPointID[2];

	LPTMAPIMG m_pTUNIT;
	HBITMAP m_hTUNIT;

	DWORD m_dwMonKind;
	DWORD m_dwMapID;
	DWORD m_dwLineID;

	WORD m_wPointID;
	WORD m_wUnitX;
	WORD m_wUnitZ;

	BYTE m_bFinding;
	BYTE m_bDRAW;
	BYTE m_bTACT;

	FLOAT m_fSCALE;
	FLOAT m_fSizeX;
	FLOAT m_fSizeZ;

	FLOAT m_fPosX;
	FLOAT m_fPosZ;

protected:
	CTMapView();
	virtual ~CTMapView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void OnDraw( CDC *pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC *pDC);
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point);
	afx_msg void OnMouseMove( UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // TMapView.cpp의 디버그 버전
inline CTQuestPathDoc* CTMapView::GetDocument() const
   { return reinterpret_cast<CTQuestPathDoc*>(m_pDocument); }
#endif
