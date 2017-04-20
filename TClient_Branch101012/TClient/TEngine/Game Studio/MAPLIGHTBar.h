#pragma once
#include "BarContent.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CMAPLIGHTBar 대화 상자입니다.

class CMAPLIGHTBar : public CBarContent
{
	DECLARE_DYNAMIC(CMAPLIGHTBar)

public:
	CMAPLIGHTBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMAPLIGHTBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPLIGHT };

private:
	static void CALLBACK OnDragLeave( CGSEditCtrl *pEdit);
	static BYTE CanDrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

protected:
	void EnableAllCtrl( BOOL bEnable);
	void SetSRC( CTextureItem *pItem);

public:
	void SetItem( CMAPItem *pItem);
	void UpdateItem();
	void OnBack();

protected:
	CTColorCtrl m_cColorCtrlA;
	CTColorCtrl m_cColorCtrlD;
	CMAPItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSEditCtrl m_cSRC;

	CSpinButtonCtrl m_spinRValueA;
	CSpinButtonCtrl m_spinGValueA;
	CSpinButtonCtrl m_spinBValueA;
	CSpinButtonCtrl m_spinRValueD;
	CSpinButtonCtrl m_spinGValueD;
	CSpinButtonCtrl m_spinBValueD;
	CSpinButtonCtrl m_spinRange;
	CSpinButtonCtrl m_spinPosX;
	CSpinButtonCtrl m_spinPosY;
	CSpinButtonCtrl m_spinPosZ;
	CSpinButtonCtrl m_spinDirX;
	CSpinButtonCtrl m_spinDirY;
	CSpinButtonCtrl m_spinDirZ;
	CSpinButtonCtrl m_spinROT;

	CButton m_cInsertMode;
	CButton m_cEditMode;
	CButton m_cDELSRC;
	CButton m_cDelete;
	CButton m_cUseDL;
	CButton m_cUseLM;

	CColorBox m_cColorBoxA;
	CColorBox m_cColorBoxD;

	CStatic m_cControlBoxA;
	CStatic m_cControlBoxD;

	CComboBox m_cType;

	CEdit m_cRValueA;
	CEdit m_cGValueA;
	CEdit m_cBValueA;
	CEdit m_cRValueD;
	CEdit m_cGValueD;
	CEdit m_cBValueD;
	CEdit m_cRange;
	CEdit m_cPosX;
	CEdit m_cPosY;
	CEdit m_cPosZ;
	CEdit m_cDirX;
	CEdit m_cDirY;
	CEdit m_cDirZ;
	CEdit m_cROT;

	DWORD m_dwTextureID;
	DWORD m_dwRValueA;
	DWORD m_dwGValueA;
	DWORD m_dwBValueA;
	DWORD m_dwRValueD;
	DWORD m_dwGValueD;
	DWORD m_dwBValueD;

	CString m_strRange;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	CString m_strDirX;
	CString m_strDirY;
	CString m_strDirZ;
	CString m_strSRC;
	CString m_strROT;

	BOOL m_bUseDL;
	BOOL m_bUseLM;

	int m_nType;
	int m_nMode;

	DECLARE_MESSAGE_MAP()
	public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnEnChangeEditRGBA();
	afx_msg void OnEnChangeEditRGBD();
	afx_msg void OnBnClickedRadioEdit();
	afx_msg void OnBnClickedRadioInsert();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDellightsrc();
	afx_msg void OnBnClickedCheckUseDynamic();
	afx_msg void OnBnClickedCheckUseLightmap();
	afx_msg void OnEnChangeEditRange();
	afx_msg void OnEnChangeEditPOS();
	afx_msg void OnEnChangeEditDIR();
	afx_msg void OnEnChangeEditROT();
	afx_msg void OnBnClickedButtonDelete();
};
