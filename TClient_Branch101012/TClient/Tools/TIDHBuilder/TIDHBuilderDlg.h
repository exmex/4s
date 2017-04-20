// TIDHBuilderDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTIDHBuilderDlg dialog
class CTIDHBuilderDlg : public CDialog
{
// Construction
public:
	CTIDHBuilderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TIDHBUILDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CString m_strTSTRINGFOLDER;
	CString m_strMISSIONFOLDER;
	CString m_strTERMFOLDER;
	CString m_strLANGFOLDER;
	CString m_strINFOFOLDER;
	CString m_strNPCFOLDER;

	CString m_strHEADERFOLDER;
	CString m_strIDHFOLDER;

	CString m_strTSTRING;
	CString m_strMISSION;
	CString m_strTERM;
	CString m_strLANG;
	CString m_strINFO;
	CString m_strNPC;

	CString m_strHEADER;
	CString m_strIDH;

	CButton m_cTSTRING;
	CButton m_cMISSION;
	CButton m_cTERM;
	CButton m_cLANG;
	CButton m_cINFO;
	CButton m_cNPC;

	CButton m_cHEADER;
	CButton m_cIDH;

protected:
	void WriteString(
		CFile* pFile,
		CString strValue);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonSource();
	afx_msg void OnBnClickedButtonTarget();
	afx_msg void OnBnClickedButtonMission();
	afx_msg void OnBnClickedButtonTerm();
	afx_msg void OnBnClickedButtonNpc();
	afx_msg void OnBnClickedButtonTstring();
	afx_msg void OnBnClickedButtonLanguage();
	afx_msg void OnBnClickedButtonInfostring();
	afx_msg void OnBnClickedButtonBuild();
	DECLARE_MESSAGE_MAP()
};
