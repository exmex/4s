#pragma once
#include "afxcmn.h"
#include "EventInfo.h"

// CEventDlg 대화 상자입니다.
class CEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventDlg)

public:
	CEventDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcEvent;
	CEventInfo* m_pdlg;

	MAPEVENTINFO m_mapEventInfo;

	void Init();
	void AddListData(LPEVENTINFO pData);
	void ClearListItem();
	DWORD ItemSelect();

	afx_msg void OnBnClickedBtnAddEvent();
	afx_msg void OnBnClickedBtnDeleteEvent();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpdate();
};
