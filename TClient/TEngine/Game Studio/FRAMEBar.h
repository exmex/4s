#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CFRAMEBar 대화 상자입니다.

class CFRAMEBar : public CBarContent
{
	DECLARE_DYNAMIC(CFRAMEBar)

public:
	CFRAMEBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFRAMEBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FRAME };

private:
	static void CALLBACK OnDefaultDragLeave( CGSEditCtrl *pEdit);
	static void CALLBACK OnDisableDragLeave( CGSEditCtrl *pEdit);
	static void CALLBACK OnCTRLDragLeave( CGSListCtrl *pList);

	static BOOL CanDefaultDrop( CItemBase *pItem);
	static BOOL CanDisableDrop( CItemBase *pItem);
	static BOOL CanCTRLDrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnDefaultDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnDisableDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnCTRLDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnDefaultDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnDisableDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnCTRLDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CFRAMEItem *pItem);

	void SetDefault( CImageItem *pIMG);
	void SetDisable( CImageItem *pIMG);
	void ResetCTRL( int nIndex);

	void DeleteCTRL( int nIndex);
	void InsertCTRL(
		LPCTRL pCTRL,
		int nIndex);

	void MoveItemPos(
		int nFrom,
		int nTo);

protected:
	CTColorCtrl m_cColorCtrl;
	CFRAMEItem *m_pItem;

protected:
	void EnableAllCtrl( BOOL bEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSEditCtrl m_cDefault;
	CGSEditCtrl m_cDisable;
	CGSListCtrl m_cCTRL;

	CSpinButtonCtrl m_spinRValue;
	CSpinButtonCtrl m_spinGValue;
	CSpinButtonCtrl m_spinBValue;
	CSpinButtonCtrl m_spinHeight;
	CSpinButtonCtrl m_spinWidth;
	CSpinButtonCtrl m_spinAlpha;
	CSpinButtonCtrl m_spinPosX;
	CSpinButtonCtrl m_spinPosY;

	CColorBox m_cColorBox;
	CStatic m_cControlBox;

	CButton m_cDelDefault;
	CButton m_cDelDisable;
	CButton m_cCusColor;
	CButton m_cTextFont;
	CButton m_cTipFont;
	CButton m_cDelCtrl;
	CButton m_cPasswd;
	CButton m_cState;
	CButton m_cHotTL;
	CButton m_cHotTC;
	CButton m_cHotTR;
	CButton m_cHotCL;
	CButton m_cHotCC;
	CButton m_cHotCR;
	CButton m_cHotBL;
	CButton m_cHotBC;
	CButton m_cHotBR;
	CButton m_cMENU;
	CButton m_cDOWN;
	CButton m_cUP;

	CComboBox m_cNotify;
	CComboBox m_cAlign;
	CEdit m_cToolTip;
	CEdit m_cRValue;
	CEdit m_cGValue;
	CEdit m_cBValue;
	CEdit m_cMenuID;
	CEdit m_cHeight;
	CEdit m_cWidth;
	CEdit m_cAlpha;
	CEdit m_cText;
	CEdit m_cPosX;
	CEdit m_cPosY;

	CString m_strDefault;
	CString m_strDisable;
	CString m_strToolTip;
	CString m_strMenuID;
	CString m_strText;
	CString m_strTEXTFONT;
	CString m_strTIPFONT;

	DWORD m_dwRValue;
	DWORD m_dwGValue;
	DWORD m_dwBValue;
	DWORD m_dwAlpha;

	BOOL m_bCusColor;
	BOOL m_bPasswd;
	BOOL m_bState;

	int m_nHotPoint;
	int m_nNotify;
	int m_nHeight;
	int m_nWidth;
	int m_nAlign;
	int m_nPosX;
	int m_nPosY;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnLvnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeldefimg();
	afx_msg void OnBnClickedButtonDeldisimg();
	afx_msg void OnBnClickedButtonDeleteCtrl();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnLvnBegindragListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditText();
	afx_msg void OnEnChangeEditTooltip();
	afx_msg void OnLvnEndlabeleditListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTipfont();
	afx_msg void OnBnClickedButtonTextfont();
	afx_msg void OnBnClickedButtonMenuid();
	afx_msg void OnBnClickedRadioHotPoint();
	afx_msg void OnBnClickedCheckPasswd();
	afx_msg void OnBnClickedCheckStatebtn();
	afx_msg void OnCbnSelchangeComboAlign();
	afx_msg void OnCbnSelchangeComboNotify();
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnEnChangeEditRGB();
	afx_msg void OnBnClickedCheckCustomclr();
	afx_msg void OnBnClickedButtonFindCtrl();
	afx_msg void OnBnClickedButtonFindDefimg();
	afx_msg void OnBnClickedButtonFindDisimg();
};
