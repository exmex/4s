#pragma once
#include "afxcmn.h"


// CSaveHeightDlg 대화 상자입니다.

class CSaveHeightDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveHeightDlg)

public:
	CSaveHeightDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaveHeightDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SAVE_HEIGHTMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMinH;
	CString m_strMaxH;
	CSpinButtonCtrl m_spinMinH;
	CSpinButtonCtrl m_spinMaxH;

	
public:
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	int m_nType;
};
