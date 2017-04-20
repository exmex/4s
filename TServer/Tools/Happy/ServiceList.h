#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CServiceList 폼 뷰입니다.

class CServiceList : public CFormView
{
	DECLARE_DYNCREATE(CServiceList)

protected:
	CServiceList();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CServiceList();

void AddItemData(LPHAPPYSERVICE pService);
void AddItemDataUpdate(LPHAPPYSERVICE pService, int listCnt); // 06/4/14 현승룡 List Data Update

public:
	void UpdateServiceList(DWORD dwID);	

	enum { IDD = IDD_SERVICELIST };
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

public:
	virtual void OnInitialUpdate();
};


