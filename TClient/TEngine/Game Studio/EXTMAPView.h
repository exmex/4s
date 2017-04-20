#pragma once
#include "MAPView.h"
#include "EXTMAPDoc.h"

// CEXTMAPView 뷰입니다.

class CEXTMAPView : public CMAPView
{
	DECLARE_DYNCREATE(CEXTMAPView)

protected:
	CEXTMAPView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEXTMAPView();

public:
	BYTE m_bUNIT[9];

	CD3DLight m_INVLIGHT;
	CD3DLight m_LIGHT;

	D3DXVECTOR3 m_vEditH;
	CPoint m_ptEditH;
	CPoint m_prev;

	BOOL m_bInit;
	int m_nPrevH;

	int m_nUnitX;
	int m_nUnitZ;

	int m_nPrvPaintX;
	int m_nPrvPaintZ;

protected:
	virtual BYTE HitTest(
		LPD3DXVECTOR3 pResult,
		LPD3DXVECTOR3 pPOS,
		LPD3DXVECTOR3 pDIR);

	void CheckEDIT();

	void UpdateDIFFUSE();
	
	void MoveSPRAY(
		FLOAT fPosX,
		FLOAT fPosZ);

	void BuildSPRAY(
		LPDIRECT3DTEXTURE9 pSPRAY,
		LPDIRECT3DTEXTURE9 pIMG,
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoHEIGHT( CPoint point);
	BOOL DoSPRAY(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoDETAIL(
		FLOAT fPosX,
		FLOAT fPosZ, 
		BOOL bStart);

	BOOL DoREGION(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoHBRUSH(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoSMOOTH(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoFLATH(
		FLOAT fPosX,
		FLOAT fPosZ);

	BOOL DoADDFLATH(
		FLOAT fPosX,
		FLOAT fPosZ,
		BOOL bCalcFlathH);

	BOOL DoOBJSPRAY(
		FLOAT fPosX,
		FLOAT fPosZ);	

public:
	static LPDIRECT3DTEXTURE9 m_pSPRAYSRC[2][3];
	static LPDIRECT3DVERTEXBUFFER9 m_pVB;

public:
	static void ReleaseSPRAY();
	static void InitSPRAY();

public:
	void JumpView( D3DXVECTOR3& vPOS);
	void CheckVB( LPIPOINT pPIVOT);
	void UpdateVB();

public:
	CEXTMAPDoc* GetDocument();

protected:
	virtual BYTE GetCursorPos( LPD3DXVECTOR3 pPOS);
	virtual BYTE IsVisible( CTachyonObject *pOBJ);

	virtual FLOAT GetCamHeight();
	virtual void DrawContent();

public:
	virtual BYTE PtInView( LPD3DXVECTOR3 pPOS);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGridExplorer();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnGridCamreset();
	afx_msg void OnHotkeyEditDetail();
	afx_msg void OnHotkeyEditHeight();
	afx_msg void OnHotkeyEditTexture();
	afx_msg void OnHotkeyHeightBrush();
	afx_msg void OnHotkeyMakeFlat();
	afx_msg void OnHotkeyMakeSmooth();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHotketEditObject();
	afx_msg void OnEditHideMode();				//	 오브젝트 리스트를 통해 오브젝트의 Hide/NonHide 모드 지정
	afx_msg void OnEditFreezeMode(void);		//	 오브젝트 리스트를 통해 오브젝트의 Freeze/NonFreeze 모드 지정
	afx_msg void OnEditSelctObjByName(void);	//	 오브젝트 리스트를 통해 오브젝트를 선택
	afx_msg void OnToggleBrushshape();
};

inline CEXTMAPDoc* CEXTMAPView::GetDocument()
   { return (CEXTMAPDoc *) m_pDocument;}
