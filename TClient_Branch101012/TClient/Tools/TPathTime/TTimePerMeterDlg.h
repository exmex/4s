#pragma once


// CTTimePerMeterDlg 대화 상자입니다.

class CTTimePerMeterDlg : public CDialog
{
	DECLARE_DYNAMIC(CTTimePerMeterDlg)

public:
	CTTimePerMeterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTTimePerMeterDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETTIME };

public:
	float m_fTIME;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
