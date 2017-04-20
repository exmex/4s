#pragma once
#include "afxwin.h"


// CRetDlg 대화 상자입니다.

class CRetDlg : public CDialog
{
	DECLARE_DYNAMIC(CRetDlg)

public:
	CRetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_lbRet;

public:
	void SetMsg(CString str);
};
