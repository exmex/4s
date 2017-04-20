#pragma once
#include "afxwin.h"


// CUnitDlg 대화 상자입니다.

class CUnitDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnitDlg)

public:
	CUnitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_UNIT };

public:
	static DWORD m_vControl[TUNITCOUNT_X][TUNITCOUNT_Z];

public:
	WORD m_wUnitID;

public:
	void UpdateUNIT();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUnit00();
	afx_msg void OnBnClickedButtonUnit01();
	afx_msg void OnBnClickedButtonUnit02();
	afx_msg void OnBnClickedButtonUnit03();
	afx_msg void OnBnClickedButtonUnit04();
	afx_msg void OnBnClickedButtonUnit05();
	afx_msg void OnBnClickedButtonUnit06();
	afx_msg void OnBnClickedButtonUnit07();
	afx_msg void OnBnClickedButtonUnit10();
	afx_msg void OnBnClickedButtonUnit11();
	afx_msg void OnBnClickedButtonUnit12();
	afx_msg void OnBnClickedButtonUnit13();
	afx_msg void OnBnClickedButtonUnit14();
	afx_msg void OnBnClickedButtonUnit15();
	afx_msg void OnBnClickedButtonUnit16();
	afx_msg void OnBnClickedButtonUnit17();
	afx_msg void OnBnClickedButtonUnit20();
	afx_msg void OnBnClickedButtonUnit21();
	afx_msg void OnBnClickedButtonUnit22();
	afx_msg void OnBnClickedButtonUnit23();
	afx_msg void OnBnClickedButtonUnit24();
	afx_msg void OnBnClickedButtonUnit25();
	afx_msg void OnBnClickedButtonUnit26();
	afx_msg void OnBnClickedButtonUnit27();
	afx_msg void OnBnClickedButtonUnit30();
	afx_msg void OnBnClickedButtonUnit31();
	afx_msg void OnBnClickedButtonUnit32();
	afx_msg void OnBnClickedButtonUnit33();
	afx_msg void OnBnClickedButtonUnit34();
	afx_msg void OnBnClickedButtonUnit35();
	afx_msg void OnBnClickedButtonUnit36();
	afx_msg void OnBnClickedButtonUnit37();
	afx_msg void OnBnClickedButtonUnit40();
	afx_msg void OnBnClickedButtonUnit41();
	afx_msg void OnBnClickedButtonUnit42();
	afx_msg void OnBnClickedButtonUnit43();
	afx_msg void OnBnClickedButtonUnit44();
	afx_msg void OnBnClickedButtonUnit45();
	afx_msg void OnBnClickedButtonUnit46();
	afx_msg void OnBnClickedButtonUnit47();
	afx_msg void OnBnClickedButtonUnit50();
	afx_msg void OnBnClickedButtonUnit51();
	afx_msg void OnBnClickedButtonUnit52();
	afx_msg void OnBnClickedButtonUnit53();
	afx_msg void OnBnClickedButtonUnit54();
	afx_msg void OnBnClickedButtonUnit55();
	afx_msg void OnBnClickedButtonUnit56();
	afx_msg void OnBnClickedButtonUnit57();
	afx_msg void OnBnClickedButtonUnit60();
	afx_msg void OnBnClickedButtonUnit61();
	afx_msg void OnBnClickedButtonUnit62();
	afx_msg void OnBnClickedButtonUnit63();
	afx_msg void OnBnClickedButtonUnit64();
	afx_msg void OnBnClickedButtonUnit65();
	afx_msg void OnBnClickedButtonUnit66();
	afx_msg void OnBnClickedButtonUnit67();
};
