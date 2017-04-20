#pragma once
#include "afxwin.h"

// CDlgUpload 대화 상자입니다.

class CDlgUpload : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpload)

public:
	CDlgUpload(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUpload();

	CString m_strFile;
	BYTE m_bMachine;

	void UploadService();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_UPLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	virtual BOOL OnInitDialog();
	CComboBox m_comboBox;
	afx_msg void OnCbnSelchangeComboMachine();
	CListBox m_listBox;
	afx_msg void OnBnClickedOk();
};
