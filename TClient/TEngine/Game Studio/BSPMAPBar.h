#pragma once
#include "BarContent.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CBSPMAPBar 대화 상자입니다.

class CBSPMAPBar : public CBarContent
{
	DECLARE_DYNAMIC(CBSPMAPBar)

public:
	CBSPMAPBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBSPMAPBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BSPMAP };

public:
	static DWORD BuildPVS( DWORD_PTR lParam);

public:
	void SetItem( CBSPMAPItem *pItem);

	void SetMeshIDX();
	void SetNodeInfo(
		DWORD dwNodeID,
		DWORD dwNodeCNT,
		DWORD dwPVS);
	void OnEditObject();

protected:
	CTColorCtrl m_cColorCtrl;
	CBSPMAPItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	void EnableAllCtrl( BOOL bEnable);

public:
	CSpinButtonCtrl m_spinRegionID;
	CSpinButtonCtrl m_spinRValue;
	CSpinButtonCtrl m_spinGValue;
	CSpinButtonCtrl m_spinBValue;
	CSpinButtonCtrl m_spinLevel;

	CColorBox m_cColorBox;
	CStatic m_cControlBox;

	CButton m_cEditLight;
	CButton m_cBuildPVS;
	CButton m_cEditOBJ;
	CButton m_cEditSFX;
	CButton m_cEditSND;
	CButton m_cRebuild;
	CButton m_cOverlap;
	CButton m_cRegion;
	CButton m_cADD;
	CButton m_cDEL;

	CEdit m_cRegionID;
	CEdit m_cRValue;
	CEdit m_cGValue;
	CEdit m_cBValue;

	CString m_strPVSINFO;
	CString m_strNodeID;

	DWORD m_dwNodeCNT;
	DWORD m_dwRValue;
	DWORD m_dwGValue;
	DWORD m_dwBValue;
	DWORD m_dwTotal;
	DWORD m_dwPVS;

	BYTE m_bRegionID;
	BOOL m_bRegion;

	int m_nLevel;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEditRGB();
	afx_msg void OnBnClickedButtonRebuild();
	afx_msg void OnBnClickedButtonOverlap();
	afx_msg void OnBnClickedButtonPvs();
	afx_msg void OnBnClickedCheckEditRegion();
	afx_msg void OnEnChangeEditRegion();
	afx_msg void OnBnClickedButtonEditLight();
	afx_msg void OnBnClickedButtonEditObj();
	afx_msg void OnBnClickedButtonEditSfx();
	afx_msg void OnBnClickedButtonEditSnd();
};
