#pragma once


// CBuildHeightDlg 대화 상자입니다.

class CBuildHeightDlg : public CDialog
{
	DECLARE_DYNAMIC(CBuildHeightDlg)

public:
	CBuildHeightDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBuildHeightDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BUILD_HEIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinMinH;
	CSpinButtonCtrl m_spinMaxH;
	CSpinButtonCtrl m_spinLX;
	CSpinButtonCtrl m_spinLY;
	CSpinButtonCtrl m_spinLZ;
	CButton m_cFile;

	CString m_strLightX;
	CString m_strLightY;
	CString m_strLightZ;
	CString m_strMinH;
	CString m_strMaxH;
	CString m_strFile;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonImagefile();
protected:
	virtual void OnOK();
};
