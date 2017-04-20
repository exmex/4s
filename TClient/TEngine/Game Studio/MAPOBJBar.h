#pragma once
#include "BarContent.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


class CMAPOBJBar;
typedef vector<CMAPOBJBar *>		VECTORMAPOBJBAR;


// CMAPOBJBar 대화 상자입니다.

class CMAPOBJBar : public CBarContent
{
	DECLARE_DYNAMIC(CMAPOBJBar)

public:
	CMAPOBJBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMAPOBJBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPOBJ };

public:
	static VECTORMAPOBJBAR m_vMAPOBJBAR;
	static CTachyonObject *m_pTOBJ;
	static COBJItem *m_pOBJ;

protected:
	void EnableAllCtrl( BOOL bEnable);

	void SetCLKList();
	void SetACTList();

public:
	static void SetOBJ( CItemBase *pItem);
	static void ReleaseOBJBAR();
	static void InitOBJBAR();

public:
	void SetItem( CMAPItem *pItem);
	void UpdateItem();
	void OnObjectDelete();	// 06.02.18 박상연
	void OnBack();			// 06.02.18 박상연
	void ResetSELOBJState();

protected:
	CMAPItem *m_pItem;
	DWORD m_dwTick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinOBJFUNC;
	CSpinButtonCtrl m_spinACTFUNC;
	CSpinButtonCtrl m_spinANIFUNC;
	CSpinButtonCtrl m_spinScaleX;
	CSpinButtonCtrl m_spinScaleY;
	CSpinButtonCtrl m_spinScaleZ;
	CSpinButtonCtrl m_spinPosX;
	CSpinButtonCtrl m_spinPosY;
	CSpinButtonCtrl m_spinPosZ;
	CSpinButtonCtrl m_spinRotX;
	CSpinButtonCtrl m_spinRotY;
	CSpinButtonCtrl m_spinRotZ;
	CSpinButtonCtrl m_spinBrushSize;
	CSpinButtonCtrl m_spinRectInOBJCount;

	CButton m_cAdjustOBJPOS;
	CButton m_cInsertMode;
	CButton m_cEditMode;
	CButton m_cLanding;
	CButton m_cDelete;
	CButton m_cInsertOBJBrush;
	CButton m_cOBJExchange;
	CButton m_cResetOBJATTR;
	CButton m_cFINDOBJ;
	CButton m_cGroup;

	BOOL m_bInsertOBJBrush;

	CStatic m_cATTRTYPE;

	CComboBox m_cCLK;
	CComboBox m_cCL;
	CComboBox m_cMESH;
	CComboBox m_cBrushShape;
	CComboBox m_cVerticalOfPlane;
	
	CListCtrl m_cATTR;
	CComboBox m_cACT;
	CComboBox m_cANI;	

	CEdit m_cATTRVALUE;
	CEdit m_cOBJFUNC;
	CEdit m_cACTFUNC;
	CEdit m_cANIFUNC;
	CEdit m_cScaleX;
	CEdit m_cScaleY;
	CEdit m_cScaleZ;
	CEdit m_cPosX;
	CEdit m_cPosY;
	CEdit m_cPosZ;
	CEdit m_cRotX;
	CEdit m_cRotY;
	CEdit m_cRotZ;
	CEdit m_cRectInOBJCount;
	CEdit m_cBrushSize;
	CEdit m_cOBJMinSize;
	CEdit m_cOBJMaxSize;
	CEdit m_cSELOBJCount;

	CString m_strATTRVALUE;
	CString m_strATTRTYPE;
	CString m_strScaleX;
	CString m_strScaleY;
	CString m_strScaleZ;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	CString m_strRotX;
	CString m_strRotY;
	CString m_strRotZ;
	CString m_strOBJScaleMin;
	CString m_strOBJScaleMax;
	CString m_strSELOBJCount;

	DWORD m_dwOBJFUNC;
	DWORD m_dwACTFUNC;
	DWORD m_dwANIFUNC;

	int m_nMode;
	int m_nCLK;
	int m_nCL;
	int m_nMESH;

	int m_nACT;
	int m_nANI;
	int m_nBrushSize;
	int m_nBrushShape;
	int m_nOBJCount;
	int m_nVerticalOfPlane;
	int m_nGroup;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageBox( WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnCbnSelchangeComboClk();
	afx_msg void OnCbnSelchangeComboCl();
	afx_msg void OnCbnSelchangeComboMesh();
	afx_msg void OnCbnSelchangeComboAct();
	afx_msg void OnCbnSelchangeComboAni();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadioEdit();
	afx_msg void OnBnClickedRadioInsert();
	afx_msg void OnBnClickedGroup();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEditObjfunc();
	afx_msg void OnEnChangeEditActfunc();
	afx_msg void OnEnChangeEditAnifunc();
	afx_msg void OnLvnItemchangedListAttr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditValue();
	afx_msg void OnBnClickedButtonLanding();
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnBnClickedButtonFreeze();
	afx_msg void OnBnClickedButtonUnfreezeall();
	afx_msg void OnBnClickedButtonUnhideall();
	afx_msg void OnEnChangeEditObjbrushSize();
	afx_msg void OnCbnSelchangeComboObjbrushShape();
	afx_msg void OnEnChangeEditObjcount();
	afx_msg void OnBnClickedCheckInsertobjbybrush();
	afx_msg void OnBnClickedButtonObjectExchange();
	afx_msg void OnBnClickedButtonResetobjAttri();
	afx_msg void OnBnClickedButtonAdjustObjectPos();
	afx_msg void OnBnClickedButtonFindobj();
	afx_msg void OnBnClickedButtonHidecoll();
	afx_msg void OnBnClickedButtonUnhidecoll();
	afx_msg void OnBnClickedButtonHidesilh();
	afx_msg void OnBnClickedButtonUnhidesilh();
	afx_msg void OnBnClickedButtonHidewater();
	afx_msg void OnBnClickedButtonUnhidewater();
};
