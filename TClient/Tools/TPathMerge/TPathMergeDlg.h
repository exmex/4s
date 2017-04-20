// TPathMergeDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTPathMergeDlg dialog
class CTPathMergeDlg : public CDialog
{
// Construction
public:
	CTPathMergeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TPATHMERGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;

	CString m_strDEST;
	CString m_strSRC;

	CButton m_cDEST;
	CButton m_cSRC;

protected:
	BYTE IsPathFile( CString strFILE);
	void LoadTPATH( CString strSRC);
	void SaveTPATH();

// Implementation
protected:
	HICON m_hIcon;

	BYTE m_bUnitX;
	BYTE m_bUnitZ;
	BYTE m_bInit;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedMerge();
};
