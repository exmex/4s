#pragma once


// CTZoomDlg 대화 상자입니다.

class CTZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CTZoomDlg)

public:
	CString m_strSCALE;

public:
	CTZoomDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTZoomDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
