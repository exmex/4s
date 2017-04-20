#pragma once


// CTEditTQClassDlg 대화 상자입니다.

class CTEditTQClassDlg : public CDialog
{
	DECLARE_DYNAMIC(CTEditTQClassDlg)

public:
	CTEditTQClassDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTEditTQClassDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADD_CLASS };

public:
	CString m_strTITLE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNAME;
	BYTE	m_bClassMain;
	virtual BOOL OnInitDialog();
};
