#pragma once
#include "afxwin.h"

class CTMonSpawnInspectorCnPDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMonSpawnInspectorCnPDlg)

public:
	CTMonSpawnInspectorCnPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMonSpawnInspectorCnPDlg();

	CString*		m_pDest;

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_CNP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	CEdit m_edtDATA;
};
