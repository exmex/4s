// TMHBuilderDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTMHBuilderDlg dialog
class CTMHBuilderDlg : public CDialog
{
// Construction
public:
	CTMHBuilderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TMHBUILDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

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
	CString ReadString( CStdioFile *pFILE);
	void SetProgressText(
		CString strTASK,
		CString strDETAIL);

	void BuildTMH(
		LPMAPTHEIGHTINFO pTHEIGHTINFO,
		WORD wMapID,
		WORD wUnitID);

	void ReleaseDATA();
	void BuildDATA();
	void LoadDATA();
	void InitNAV();

	void ReleaseLIST();
	void LoadLIST();

	void ReleaseDevice();
	void InitDevice();

public:
	MAPTMAPITEM m_mapTMAPITEM;
	CTachyonRes *m_pRES;
	CD3DDevice m_vDEVICE;

public:
	CString m_strDETAIL;
	CString m_strTASK;
	CString m_strGroupID;

	CString m_strLISTFOLDER;
	CString m_strRESFOLDER;
	CString m_strTMHFOLDER;

	CString m_strLIST;
	CString m_strRES;
	CString m_strTMH;

	CButton m_cLIST;
	CButton m_cRES;
	CButton m_cTMH;

	afx_msg void OnBnClickedButtonList();
	afx_msg void OnBnClickedButtonRes();
	afx_msg void OnBnClickedButtonTmh();
	afx_msg void OnBnClickedButtonBuild();
	afx_msg void OnDestroy();
};
