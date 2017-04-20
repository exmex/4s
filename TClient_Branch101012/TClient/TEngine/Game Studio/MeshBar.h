#pragma once
#include "BarContent.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CMeshBar 대화 상자입니다.

class CMeshBar : public CBarContent
{
	DECLARE_DYNAMIC(CMeshBar)

public:
	CMeshBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MESH };

public:
	void SetItem( CMeshItem *pItem);
	void SetCamDist( FLOAT fDist);
	void SetItemData();

protected:
	void EnableAllCtrl( BOOL bEnable);

protected:
	CMeshItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinPolyLevel;
	CSpinButtonCtrl m_spinLevel;
	CSpinButtonCtrl m_spinDist;
	CSliderCtrl m_cPolyCtrl;

	CButton m_cUnselectAll;
	CButton m_cLoadFromTMF;
	CButton m_cSelectAll;
	CButton m_cInvSelect;
	CButton m_cAddLevel;
	CButton m_cDelLevel;
	CButton m_cSetDist;
	CButton m_cUseVB;

	CEdit m_cPolyLevel;
	CEdit m_cDist;

	DWORD m_dwTotalCount;
	DWORD m_dwMeshCount;
	DWORD m_dwCount;
	DWORD m_dwLevel;

	CString m_strPolyLevel;
	CString m_strCamDist;
	CString m_strDist;

	BYTE m_bLevelCount;
	BOOL m_bTestMode;
	BOOL m_bUseVB;
	int m_nPolyLevel;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioEditmode();
	afx_msg void OnBnClickedRadioTestmode();
	afx_msg void OnBnClickedButtonAddlevel();
	afx_msg void OnBnClickedButtonDellevel();
	afx_msg void OnBnClickedButtonLoadFromTmf();
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonSelectInv();
	afx_msg void OnBnClickedButtonSetdist();
	afx_msg void OnBnClickedButtonUnselectAll();
	afx_msg void OnEnChangeEditDist();
	afx_msg void OnEnKillfocusEditPolyLevel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditPolyLevel();
	afx_msg void OnBnClickedCheckUsevb();
};
