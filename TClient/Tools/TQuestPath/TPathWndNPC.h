#pragma once

#include "TPathWndBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CTPathWndNPC 대화 상자입니다.

class CTPathWndNPC : public CTPathWndBase
{
	DECLARE_DYNAMIC(CTPathWndNPC)

public:
	CTPathWndNPC( CWnd *pParent = NULL);
	virtual ~CTPathWndNPC();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TPATHWND_NPC };

public:
	virtual void UpdateDATA(
		int nType,
		LPARAM lParam);
	virtual void UpdateTMAP();

public:
	void UpdateTNDATA( LPTNDATA pTNDATA);
	void ResetTLIST();

public:
	virtual BOOL Create( CWnd *pParentWnd = NULL);

protected:
	virtual void DoDataExchange( CDataExchange *pDX);

public:
	CString m_strFilterNpcName;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;

	CButton m_cNpcName;
	CButton m_cMapID;
	CButton m_cNpcID;
	CListCtrl m_cLIST;

	DWORD m_dwFilterMapID;
	DWORD m_dwFilterNpcID;
	DWORD m_dwNpcID;
	DWORD m_dwMapID;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListNpc( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListNpc( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListNpc( NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeFilterNpcName();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnEnChangeFilterMapId();
	afx_msg void OnEnChangeFilterNpcId();
	afx_msg void OnBnClickedCheck();
};
