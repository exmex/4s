#pragma once

#include "TPathWndBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CTPathWndMON 대화 상자입니다.

class CTPathWndMON : public CTPathWndBase
{
	DECLARE_DYNAMIC(CTPathWndMON)

public:
	CTPathWndMON( CWnd *pParent = NULL);
	virtual ~CTPathWndMON();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TPATHWND_MON };

public:
	virtual void UpdateDATA(
		int nType,
		LPARAM lParam);
	virtual void UpdateTMAP();

public:
	void UpdateTMDATA( DWORD dwKIND);
	void ResetTLIST();

public:
	virtual BOOL Create( CWnd *pParentWnd = NULL);

protected:
	virtual void DoDataExchange( CDataExchange *pDX);

public:
	CString m_strFilterNAME;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;

	CListCtrl m_cLIST;
	CButton m_cMapID;
	CButton m_cMonID;
	CButton m_cKIND;
	CButton m_cNAME;

	DWORD m_dwFilterMapID;
	DWORD m_dwFilterMonID;
	DWORD m_dwFilterKIND;
	DWORD m_dwMapID;
	DWORD m_dwKIND;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListMon( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListMon( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListMon( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeFilterMonName();
	afx_msg void OnEnChangeFilterMonKind();
	afx_msg void OnEnChangeFilterMonId();
	afx_msg void OnEnChangeFilterMapId();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCheck();
};
