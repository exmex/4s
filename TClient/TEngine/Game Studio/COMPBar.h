#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CCOMPBar 대화 상자입니다.

class CCOMPBar : public CBarContent
{
	DECLARE_DYNAMIC(CCOMPBar)

public:
	CCOMPBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCOMPBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONTROL };

private:
	static void CALLBACK OnDefaultDragLeave( CGSEditCtrl *pEdit);
	static void CALLBACK OnDisableDragLeave( CGSEditCtrl *pEdit);
	static void CALLBACK OnCOMPDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnSNDDragLeave( CGSEditCtrl *pEdit);

	static BOOL CanDefaultDrop( CItemBase *pItem);
	static BOOL CanDisableDrop( CItemBase *pItem);
	static BOOL CanSNDDrop( CItemBase *pItem);
	static BOOL CanCOMPDrop( CCOMPBar *pBar, CItemBase *pItem);

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

	static DROPEFFECT CALLBACK OnSNDDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnCOMPDragOver(
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

	static BOOL CALLBACK OnSNDDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnCOMPDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CCOMPItem *pItem);

	void SetDefault( CImageItem *pIMG);
	void SetDisable( CImageItem *pIMG);
	void SetSOUND( CMediaItem *pSND);
	void ResetCOMP( int nIndex);

	void DeleteCOMP( int nIndex);
	void InsertCOMP(
		LPCOMP pCOMP,
		int nIndex);

	void MoveItemPos(
		int nFrom,
		int nTo);

protected:
	CCOMPItem *m_pItem;

protected:
	void EnableAllCtrl( BOOL bEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSEditCtrl m_cDefault;
	CGSEditCtrl m_cDisable;
	CGSEditCtrl m_cSOUND;
	CGSListCtrl m_cCOMP;

	CSpinButtonCtrl m_spinSuperLevel;
	CSpinButtonCtrl m_spinHMargine;
	CSpinButtonCtrl m_spinVMargine;
	CSpinButtonCtrl m_spinHeight;
	CSpinButtonCtrl m_spinWidth;
	CSpinButtonCtrl m_spinPosX;
	CSpinButtonCtrl m_spinPosY;

	CButton m_cDelDefault;
	CButton m_cDelDisable;
	CButton m_cDELSND;
	CButton m_cDelIMG;
	CButton m_cHotTL;
	CButton m_cHotTC;
	CButton m_cHotTR;
	CButton m_cHotCL;
	CButton m_cHotCC;
	CButton m_cHotCR;
	CButton m_cHotBL;
	CButton m_cHotBC;
	CButton m_cHotBR;
	CButton m_cDOWN;
	CButton m_cUP;
	CButton m_PrintList;

	CComboBox m_cCtrlType;
	CComboBox m_cUseage;

	CEdit m_cSuperLevel;
	CEdit m_cHMargine;
	CEdit m_cVMargine;
	CEdit m_cHeight;
	CEdit m_cWidth;
	CEdit m_cPosX;
	CEdit m_cPosY;

	CString m_strDefault;
	CString m_strDisable;
	CString m_strSOUND;

	int m_nSuperLevel;
	int m_nHMargine;
	int m_nVMargine;
	int m_nHotPoint;
	int m_nCtrlType;
	int m_nWidth;
	int m_nHeight;
	int m_nUseage;
	int m_nPosX;
	int m_nPosY;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboCtrltype();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnCbnSelchangeComboUseage();
	afx_msg void OnLvnItemchangedListComp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeldefimg();
	afx_msg void OnBnClickedButtonDeldisimg();
	afx_msg void OnBnClickedButtonDeleteImage();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnLvnBegindragListComp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateListComp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioHotPoint();
	afx_msg void OnEnChangeEditSl();
	afx_msg void OnBnClickedButtonDelsound();
	afx_msg void OnEnChangeEditHmargine();
	afx_msg void OnEnChangeEditVmargine();
	afx_msg void OnBnClickedButtonAllMove();
	afx_msg void OnBnClickedPrintlist();
	afx_msg void OnBnClickedButtonFindComp();
	afx_msg void OnBnClickedButtonFindDefimg();
	afx_msg void OnBnClickedButtonFindDisimg();
	afx_msg void OnBnClickedExportimgs();
};
