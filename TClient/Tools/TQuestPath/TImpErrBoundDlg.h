#pragma once


// CTImpErrBoundDlg 대화 상자입니다.

class CTImpErrBoundDlg : public CDialog
{
	DECLARE_DYNAMIC(CTImpErrBoundDlg)

public:
	CTImpErrBoundDlg( CWnd *pParent = NULL);
	virtual ~CTImpErrBoundDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ERRBOUND };

protected:
	virtual void DoDataExchange( CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	DWORD m_dwBound;
};
