#pragma once
#include "PlayCtrl.h"
#include "GridView.h"
#include "MAPDoc.h"


#define NAV_INIT_HEIGHT				1.5f

#define EM_NONE				((BYTE) 0x00)
#define EM_INSERTOBJ		((BYTE) 0x01)
#define EM_EDITOBJ			((BYTE) 0x02)
#define EM_INSERTSFX		((BYTE) 0x03)
#define EM_EDITSFX			((BYTE) 0x04)
#define EM_INSERTSND		((BYTE) 0x05)
#define EM_EDITSND			((BYTE) 0x06)
#define EM_INSERTLIGHT		((BYTE) 0x07)
#define EM_EDITLIGHT		((BYTE) 0x08)

#define NAV_MOVE_NONE		((BYTE) 0x00)
#define NAV_MOVE_LEFT		((BYTE) 0x01)
#define NAV_MOVE_FORWARD	((BYTE) 0x02)
#define NAV_MOVE_RIGHT		((BYTE) 0x04)
#define NAV_MOVE_BACK		((BYTE) 0x08)
#define NAV_MOVE_UP			((BYTE) 0x10)
#define NAV_MOVE_DOWN		((BYTE) 0x20)


class CGizimo;

// CMAPView 뷰입니다.

class CMAPView : public CGridView
{
	DECLARE_DYNCREATE(CMAPView)

protected:
	CMAPView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMAPView();

public:
	virtual BYTE PtInView( LPD3DXVECTOR3 pPOS);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	static LPDIRECT3DVERTEXBUFFER9 m_pVB;
	static LPDIRECT3DTEXTURE9 m_pLTARGET;
	static LPDIRECT3DTEXTURE9 m_pLIGHT;

public:
	static void ReleaseRECT();
	static void InitRECT();

protected:
	void SelectOBJ( UINT nFlags);
	void SelectSFX();
	void SelectSND();
	void SelectLIGHT();
	void SelectEqualOBJ();
	BYTE GetEM();

	BOOL MeshHitTest(
		CTachyonMesh *pMesh,
		LPD3DXMATRIX pPOS,
		CD3DCamera *pCamera,
		int nPosX,
		int nPosY,
		FLOAT *pDist);

	BOOL MeshInRect(
		CTachyonMesh *pMesh,
		LPD3DXMATRIX pPOS,
		CD3DCamera *pCamera,
		CRect rect);

	BOOL PtInRect(
		LPD3DXVECTOR3 pPoint,
		CD3DCamera *pCamera,
		CRect rect);

public:
	CPlayCtrl m_cNAV;
	CRect m_rect;

	DWORD m_dwCamTick;
	BYTE m_bDrawOBJ;
	BYTE m_bMove;

protected:
	BOOL m_bShowEdge;
	CGizimo* m_pGizimo;

public:
	CMAPDoc* GetDocument();

protected:
	virtual BYTE GetCursorPos( LPD3DXVECTOR3 pPOS);
	virtual BYTE IsVisible( CTachyonObject *pOBJ);
	virtual BYTE IsVisible( LPD3DXVECTOR3 pPOS);
	virtual BYTE IsVisible( LPMAPSFX pSFX);
	virtual BYTE IsVisible( LPMAPSND pSND);
	virtual BYTE IsPlay();

	virtual FLOAT GetCamHeight();
	virtual void DrawContent();

	virtual void RotateView(
		FLOAT fRotX,
		FLOAT fRotY,
		FLOAT fRotZ);

	virtual void MoveView(
		FLOAT fMoveX,
		FLOAT fMoveY,
		FLOAT fMoveZ);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnGridCamreset();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGridNavigate();
	afx_msg void OnGridShowEdge();
	afx_msg void OnUpdateGridShowEdge(CCmdUI *pCmdUI);
	virtual BYTE HitTest(LPD3DXVECTOR3 pResult, LPD3DXVECTOR3 pPOS, LPD3DXVECTOR3 pDIR);
protected:
	void OnGroupCommand(BYTE bID, BOOL bSave = TRUE);
public:
	afx_msg void OnGroupObjectKey1();
	afx_msg void OnGroupObjectKey2();
	afx_msg void OnGroupObjectKey3();
	afx_msg void OnGroupObjectKey4();
	afx_msg void OnGroupObjectKey5();
	afx_msg void OnGroupObjectKey6();
	afx_msg void OnGroupObjectKey7();
	afx_msg void OnGroupObjectKey8();
	afx_msg void OnGroupObjectKey9();
	afx_msg void OnGroupObjectKey0();
	afx_msg void OnSelectEqualobj();
	afx_msg void OnClearSelobj();
};

inline CMAPDoc *CMAPView::GetDocument()
	{ return (CMAPDoc *) m_pDocument;}
