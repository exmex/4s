#pragma once
#include "afxcmn.h"


// CTExportDlg 대화 상자입니다.

class CTExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CTExportDlg)

public:
	CTExportDlg( CWnd *pParent = NULL);
	virtual ~CTExportDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EXPORT };

public:
	void OnCheckTREE(
		CTreeCtrl& cTree,
		HTREEITEM hITEM);

	void ExportTMAP(
		CStdioFile *pFILE,
		HTREEITEM hITEM);

	void ExportDATA(
		CStdioFile *pFILE,
		HTREEITEM hITEM);

	void CheckParent(
		CTreeCtrl& cTree,
		HTREEITEM hITEM,
		BYTE bCHECK);

	void CheckChild(
		CTreeCtrl& cTree,
		HTREEITEM hITEM,
		BYTE bCHECK);

	void InitTREE(
		HTREEITEM hITEM,
		HTREEITEM hDEST);

	BYTE GetCheck(
		CTreeCtrl& cTree,
		HTREEITEM hITEM);

	void SetCheck(
		CTreeCtrl& cTree,
		HTREEITEM hITEM,
		BYTE bCHECK);

public:
	CImageList m_listCheckImage;

	LPMAPTMAPDATA m_pTMAPDATA;
	CTreeCtrl *m_pTVIEW;
	CTreeCtrl m_cTQUEST;
	CTreeCtrl m_cTMAP;

protected:
	virtual void DoDataExchange( CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnKeydownTreeQuest( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydownTreeMap( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeQuest( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeMap( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonMclearAll();
	afx_msg void OnBnClickedButtonMselAll();
	afx_msg void OnBnClickedButtonClearAll();
	afx_msg void OnBnClickedButtonSelAll();
	afx_msg void OnBnClickedButtonMexport();
	afx_msg void OnBnClickedButtonExport();
};
