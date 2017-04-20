#pragma once
#include "EditListCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"


// COBJPage 대화 상자입니다.

class COBJPage : public CPropertyPage
{
	DECLARE_DYNAMIC(COBJPage)

public:
	COBJPage();
	virtual ~COBJPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJ_PAGE };

protected:
	int m_nCurACT;
	int m_nCurCLK;
	int m_nCurCL;

protected:
	void UpdateColumnWidth( CListCtrl *pListCtrl);

	BOOL CheckACTID( CString strID);
	BOOL CheckCLKID( CString strID);
	BOOL CheckCLID( CString strID);

	BOOL OnACTListLButtonDown();
	BOOL OnCLKListLButtonDown();
	BOOL OnCLListLButtonDown();
	BOOL OnTabKey( HWND hWnd);
	BOOL ApplyACTData();
	BOOL ApplyCLKData();
	BOOL ApplyCLData();

	void PrepareACTCtrl( BOOL bLoad);
	void PrepareCLKCtrl( BOOL bLoad);
	void PrepareCLCtrl( BOOL bLoad);

	void UpdateACT( CString strACT);
	void UpdateCLK( CString strCLK);
	void UpdateCL( CString strCL);

	void DeleteACTRow();
	void DeleteCLKRow();
	void DeleteCLRow();

	void EditACTData();
	void EditCLKData();
	void EditCLData();

public:
	void SetData(
		COBJItem *pItem,
		LPOBJECT pOBJ,
		MAPSTR *pSTR);

public:
	COBJItem *m_pItem;

	LPOBJECT m_pOBJ;
	MAPSTR *m_pSTR;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CEditListCtrl m_cListACT;
	CEditListCtrl m_cListCLK;
	CEditListCtrl m_cListCL;

	CEdit m_cACT;
	CEdit m_cCLK;
	CEdit m_cCL;

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListUpdate( WPARAM wParam, LPARAM lParam);
	afx_msg void OnHdnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchangedListClothkind(NMHDR *pNMHDR, LRESULT *pResult);
};
