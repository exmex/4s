#pragma once


// CFramesetDlg 대화 상자입니다.

class CFramesetDlg : public CDialog
{
	DECLARE_DYNAMIC(CFramesetDlg)

public:
	CFramesetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFramesetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FRAMESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwTotalFrame;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
