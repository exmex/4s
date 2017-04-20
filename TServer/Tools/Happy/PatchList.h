#pragma once
#include "afxcmn.h"



// CPatchList 폼 뷰입니다.

class CPatchList : public CFormView
{
	DECLARE_DYNCREATE(CPatchList)

protected:
	CPatchList();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CPatchList();

public:
	void UpdatePatchList();
	enum { IDD = IDD_PATCHLIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_listCtrl;
	virtual void OnInitialUpdate();
};


