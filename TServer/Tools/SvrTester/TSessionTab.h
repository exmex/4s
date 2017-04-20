#pragma once
#include "afxcmn.h"



// CTSessionTab 폼 뷰입니다.

class CTSessionTab : public CFormView
{
	DECLARE_DYNCREATE(CTSessionTab)

protected:
	CTSessionTab();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTSessionTab();

public:
	enum { IDD = IDD_TSESSIONTAB };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:	
	virtual void OnInitialUpdate();

	//////////////////////////////////////////////////////////////////////
	// Protocol List
	CListCtrl m_listProtocol;

	void LoadList(); // Protocol List Load
	void ClearList(); // Protocol List Clear

	// Protocol List Drag
	BYTE m_Drag;
	int m_ListSelect;
	CRect m_EditRect;
	CRect m_ListRect;

	afx_msg void OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	//////////////////////////////////////////////////////////////////////

	// Enter / Escape block
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
};


