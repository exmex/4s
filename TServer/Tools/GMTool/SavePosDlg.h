#pragma once
#include "afxwin.h"


// CSavePosDlg 대화 상자입니다.

class CSavePosDlg : public CDialog
{
	DECLARE_DYNAMIC(CSavePosDlg)

public:
	CSavePosDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSavePosDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVEPOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	USERPOS m_stUserPos;
	CString m_strSaveName;

public:
	void SetUserPos(USERPOS& _UserPos);
	void Init();
	
	afx_msg void OnBnClickedOk();
	CEdit m_editSaveName;
	virtual BOOL OnInitDialog();
};
