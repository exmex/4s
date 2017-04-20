// TMeshRegulatorDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTMeshRegulatorDlg dialog
class CTMeshRegulatorDlg : public CDialog
{
// Construction
public:
	CTMeshRegulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TMESHREGULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	BYTE IsMeshFile( CString strFileName);

	void Regulate(
		CString strSRC,
		CString strDEST);

	void Regulate(
		CFile *pSRC,
		CFile *pDEST);

public:
	D3DXMATRIX m_vMAT[MAX_PIVOT];
	DWORD m_dwNodeCount;

	CString m_strDESTFOLDER;
	CString m_strTMFFOLDER;
	CString m_strSRCFOLDER;

	CString m_strDEST;
	CString m_strTMF;
	CString m_strSRC;

	CButton m_cSRCFOLDER;
	CButton m_cDEST;
	CButton m_cSRC;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedRegulate();
	afx_msg void OnBnClickedButtonSrcfolder();
};
