#pragma once


// CClassTitleDlg 대화 상자입니다.

class CClassTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CClassTitleDlg)

public:
	CClassTitleDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CClassTitleDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CLASS_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strClassTitle;
protected:
	virtual void OnCancel();
};
