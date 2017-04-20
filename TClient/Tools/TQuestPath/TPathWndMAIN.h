#pragma once

#include "TPathWndBase.h"


// CTPathWndMAIN 대화 상자입니다.

class CTPathWndMAIN : public CTPathWndBase
{
	DECLARE_DYNAMIC(CTPathWndMAIN)

public:
	CTPathWndMAIN( CWnd *pParent = NULL);
	virtual ~CTPathWndMAIN();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TPATHWND_MAIN };

public:
	virtual void UpdateDATA(
		int nType,
		LPARAM lParam);
	virtual void UpdateTMAP();

public:
	void UpdateTERMDATA( LPTTITEM pTTITEM);
	void EnableTermCtrl();
	void EnableCtrl();
	void UpdateDATA();
	void UpdateTERM();

public:
	virtual BOOL Create( CWnd *pParentWnd = NULL);

protected:
	virtual void DoDataExchange( CDataExchange *pDX);

public:
	LPTTITEM m_pTTITEM;

	CString m_strNpcName;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	CString m_strMSG;

	DWORD m_dwQuestID;
	DWORD m_dwMonKind;
	DWORD m_dwNpcID;
	DWORD m_dwMapID;

protected:
	CListCtrl m_cMonName;
	CListCtrl m_cTSTEP;

	CStatic m_cQuestID;
	CStatic m_cMonKind;
	CStatic m_cNpcID;

	CButton m_cAutoset;
	CButton m_cRestore;
	CButton m_cApply;
	CButton m_cLOCK;

	CButton m_cFindMON;
	CButton m_cFindNPC;
	CButton m_cPickPOS;

	CEdit m_cNpcName;
	CEdit m_cMapID;
	CEdit m_cPosX;
	CEdit m_cPosY;
	CEdit m_cPosZ;
	CEdit m_cMSG;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListStep(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAutoSet();
	afx_msg void OnBnClickedButtonRestore();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCheckLock();
	afx_msg void OnBnClickedButtonShowNpc();
	afx_msg void OnBnClickedButtonShowMon();
	afx_msg void OnNMClickListStep(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListStep(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPickPos();
};
