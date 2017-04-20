#pragma once

#include "TPathWndBase.h"
#include "afxcmn.h"
#include "afxwin.h"


// CTPathWndPOS 대화 상자입니다.

class CTPathWndPOS : public CTPathWndBase
{
	DECLARE_DYNAMIC(CTPathWndPOS)

public:
	CTPathWndPOS( CWnd *pParent = NULL);
	virtual ~CTPathWndPOS();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TPATHWND_POS };

public:
	virtual void UpdateDATA(
		int nType,
		LPARAM lParam);
	virtual void UpdateTMAP();

public:
	void ResetTUNIT( LPTMAPIMG pTUNIT);
	void ResetTMAP( LPTMAPDATA pTMAP);
	void ResetTLIST();
	void EnableCtrl();
	BYTE CheckValid();

	void SelectTUNIT( LPTMAPIMG pTUNIT);
	void SelectTMAP( LPTMAPDATA pTMAP);

public:
	virtual BOOL Create( CWnd *pParentWnd = NULL);

protected:
	virtual void DoDataExchange( CDataExchange *pDX);

public:
	LPTMAPDATA m_pTMAP;
	LPTMAPIMG m_pTUNIT;

	CString m_strPosX;
	CString m_strPosZ;
	BYTE m_bUpdateMAP;

protected:
	CListCtrl m_cTUNIT;
	CListCtrl m_cTMAP;

	CButton m_cApply;
	CEdit m_cPosX;
	CEdit m_cPosZ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListMap( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListMap( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListMap( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListUnit( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListUnit( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListUnit( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPOS();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonApply();
};
