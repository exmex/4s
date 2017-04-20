#pragma once

#include "TabCtrlST.h"

#include "TSessionTab.h"
#include "TMacroTab.h"

// CTabGroup 대화 상자입니다.

class CTabGroup : public CDialog
{
	DECLARE_DYNAMIC(CTabGroup)

public:
	CTabGroup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabGroup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TABGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CTabCtrlST m_TabGroup; // CTabCtrlST Class
	CImageList *m_pImageList; // Image List
public:
	//////////////////////////////////////////////////////////////////////
	// Session Protocol List Load / Clear
	void TSessionTabLoadList();	
	void TSessionListClear();
	//////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////
	// Session / Macro Tab Active
	void TSessionActive();
	void TMacroActive();

	// Get Current Tab
	BOOL GetCurTab();

	// Get Session / Macro Tab View
	CTSessionTab* GetTSessionTabView();
	CTMacroTab* GetTMacroTabView();
	//////////////////////////////////////////////////////////////////////	

	// Enter / Escape block
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
