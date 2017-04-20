#pragma once


// CEditJointDlg 대화 상자입니다.

class CEditJointDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditJointDlg)

public:
	CEditJointDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditJointDlg();

public:
	LPTJOINT m_pTJOINT;
	DWORD m_dwID;

	BOOL m_vJOINT[8];
	CRect m_vRECT;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDITJOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
