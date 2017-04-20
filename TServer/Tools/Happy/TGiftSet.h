#pragma once
#include "xlistctrl.h"


// CTGiftSet 대화 상자입니다.

class CTGiftSet : public CDialog
{
	DECLARE_DYNCREATE(CTGiftSet)

public:
	CTGiftSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTGiftSet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GIFTSET };

	void SetData();
	void InitList();
	void InsertListItem(LPCMGIFT pGift= NULL);
	void DelteListItem(int nRow);
	void ClearList();
	void CheckValues(int nRow);
	BYTE GetListData(int nRow, TCMGIFT& tGift);
	int GetSelectItem();

public:
	CXListCtrl m_lcCMGift;
	MAPCMGIFT m_mapCMGift;
	TCMGIFT m_stDefalultGift;
	BYTE m_bGroup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGiftadd();
	afx_msg void OnBnClickedGiftdel();
	afx_msg void OnBnClickedGiftre();
	afx_msg void OnBnClickedGiftup();

	afx_msg LRESULT OnXCtrlUpdate(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
