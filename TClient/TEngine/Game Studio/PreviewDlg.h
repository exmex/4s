#pragma once
#include "afxwin.h"
#include "ItemBase.h"

class CPreviewCtrl : public CStatic
{
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pTextureVB;

public :

	CD3DLight m_LightCenter;
	CD3DLight m_LightRight;
	CD3DLight m_LightTop;

protected:
	void SetCamera(
		D3DXVECTOR3& vCenter,
		FLOAT fWidth,
		FLOAT fHeight,
		FLOAT fDist);

public:
	void DrawLOGO( CDC *pDC);
	void DrawAnimation();
	void DrawTexture();
	void DrawImage();
	void DrawMesh();
	void DrawOBJ();

	void DeleteTextureVB();
	void InitTextureVB();
	void InitSize( int cx, int cy);

	void DrawImage(
		CD3DImage *pImage,
		int nPosX,
		int nPosY,
		int nWidth,
		int nHeight);

public:
	CItemBase *m_pItem;

public:
	CPreviewCtrl();
	virtual ~CPreviewCtrl();

	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


// CPreviewDlg 대화 상자입니다.

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPreviewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	void SetItem(CItemBase *pItem);

public:
	CPreviewCtrl m_cPreviewCtrl;
	CStatic m_cTitleID;
	CEdit m_cItemID;

	CString m_strItemID;
	BOOL m_bUpdate;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMessageBox( WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeEditItemid();
	afx_msg void OnDestroy();
	afx_msg void OnEnSetfocusEditItemid();
	afx_msg void OnEnKillfocusEditItemid();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
