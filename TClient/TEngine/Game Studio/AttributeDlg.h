#pragma once


// CAttributeDlg 대화 상자입니다.

class CAttributeDlg : public CDialog
{
	DECLARE_DYNAMIC(CAttributeDlg)

public:
	CAttributeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAttributeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ATTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTitle;
	CString m_strValue;
	int m_bDataType;
};
