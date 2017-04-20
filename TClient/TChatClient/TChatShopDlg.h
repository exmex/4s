#pragma once
#include "afxcmn.h"


class CTChatClientView;

// CTChatShopDlg 대화 상자입니다.

class CTChatShopDlg : public CDialog
{
	DECLARE_DYNAMIC(CTChatShopDlg)

public:
	CTChatShopDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTChatShopDlg();

public:
	CTChatClientView *m_pHost;
	CString m_strTSHOPNAME;

	CListCtrl m_cInvenList;
	CListCtrl m_cItemList;
	CListCtrl m_cShopList;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_INVEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListInven(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonToshop();
	afx_msg void OnBnClickedButtonToinven();
protected:
	virtual void OnOK();
};
