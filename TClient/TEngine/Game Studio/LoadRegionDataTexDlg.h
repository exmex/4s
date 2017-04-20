#pragma once
#include "afxwin.h"


// CLoadRegionDataTexDlg 대화 상자입니다.

class CLoadRegionDataTexDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadRegionDataTexDlg)

public:
	CLoadRegionDataTexDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoadRegionDataTexDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOAD_REGION_TEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTitle;
	CString m_strFileName;

	CButton m_cFile;

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

public:
	afx_msg void OnBnClickedButtonImagefile();
};
