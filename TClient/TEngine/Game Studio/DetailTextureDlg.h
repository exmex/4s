#pragma once
#include "GSListCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"


// CDetailTextureDlg 대화 상자입니다.

class CDetailTextureDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailTextureDlg)

public:
	CDetailTextureDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDetailTextureDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DETAILTEXTURE };

private:
	static BYTE CanInsert( CGSListCtrl *pList, DWORD dwTextureID);
	static BYTE CanDrop( CGSListCtrl *pList, CItemBase *pItem);

	static void CALLBACK OnDragLeave( CGSListCtrl *pList);
	static DROPEFFECT CALLBACK OnDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	static void StartEdit(
		CWnd *pParent,
		CEXTMAPItem *pItem);
	static DWORD __ResetTILE( DWORD_PTR lParam);

public:
	BYTE CanReplace( int nIndex);

	void MoveTexture( int nFROM, int nTO);
	void SetItem( CEXTMAPItem *pItem);
	void EnableAllCtrl( BYTE bEnable);
	void ResetTILE( LPBMPARAM lParam);

public:
	static CDetailTextureDlg m_vDETAILTEXDlg;

	CEXTMAPItem *m_pItem;
	CGSListCtrl m_cLIST;
	CButton m_cREPLACE;
	CButton m_cDELETE;
	CButton m_cDOWN;
	CButton m_cUP;
	CEdit m_cSCALE;

	CString m_strSCALE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnLvnDeleteitemListDettex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListDettex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragListDettex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditScale();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonReplace();
	afx_msg void OnBnClickedButtonSavetex();
};
