#pragma once


// CFindQuestTitleDlg 대화 상자입니다.

class CFindQuestTitleDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindQuestTitleDlg)

public:
	CFindQuestTitleDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFindQuestTitleDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FIND_QUEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSearchText;
	afx_msg void OnBnClickedOk();
	void GetSearchString(CString* pStr);
	//BOOL m_bSearchType;
	void GetSearchType(INT* nType);
	//BYTE m_bSearchType;
	int m_nSearchType;
};
