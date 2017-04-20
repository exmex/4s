#pragma once


// CSetJointSizeDlg 대화 상자입니다.

class CSetJointSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetJointSizeDlg)

public:
	CSetJointSizeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetJointSizeDlg();

public:
	CSize m_vTJOINT;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETJOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
