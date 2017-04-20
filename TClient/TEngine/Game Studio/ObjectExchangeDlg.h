#pragma once
#include "afxwin.h"


// CObjectExchangeDlg 대화 상자입니다.

class CObjectExchangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CObjectExchangeDlg)

public:
	CObjectExchangeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectExchangeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OBJECT_EXCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bOBJExchange;
	BOOL m_bAllOBJ;
	afx_msg void OnBnClickedCheck();
};
