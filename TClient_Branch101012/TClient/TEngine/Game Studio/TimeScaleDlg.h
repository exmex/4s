#pragma once


// CTimeScaleDlg 대화 상자입니다.

class CTimeScaleDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeScaleDlg)

public:
	CTimeScaleDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTimeScaleDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TIMESCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTimeScale;
	DWORD m_dwTotalFrame;

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
