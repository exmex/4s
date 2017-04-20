#pragma once
#include "afxcmn.h"



// CMachine 폼 뷰입니다.
class CHappyDoc;
class CMachine : public CFormView
{
	DECLARE_DYNCREATE(CMachine)

protected:
	CMachine();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMachine();

public:
	enum { IDD = IDD_MACHINE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTreeCtrl m_treeCtrl;
	CRect treeRect;
	CString m_strSelected;

public:
	void InsertInitData();
	void ClearMachineTree();
	BYTE GetSelectedMachineID();

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeMachine(NMHDR *pNMHDR, LRESULT *pResult);
};


