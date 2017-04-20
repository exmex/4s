#pragma once
#include "ItemBase.h"
#include "ATTRPage.h"
#include "OBJPage.h"


// CEditObjectDlg 대화 상자입니다.

class CEditObjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditObjectDlg)

public:
	CEditObjectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditObjectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDITOBJ };

protected:
	CImageList m_listTabImages;

	CPropertySheet m_wndSHEET;
	CATTRPage m_wndATTR;
	COBJPage m_wndOBJ;

public:
	COBJItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
