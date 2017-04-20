#pragma once
#include "EditListCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"


// CATTRPage 대화 상자입니다.

class CATTRPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CATTRPage)

public:
	CATTRPage();
	virtual ~CATTRPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR_PAGE };

protected:
	int m_nCurColumn;
	int m_nCurRow;

protected:
	BYTE GetType( CString strType);
	BOOL CheckID( CString strID);
	BOOL OnTabKey( HWND hWnd);
	BOOL OnListLButtonDown();
	BOOL ApplyData();

	void UpdateValue( CString strValue);
	void UpdateType( BYTE bType);
	void UpdateID( CString strID);

	void PrepareCtrl( BOOL bLoad);
	void DeleteRow();
	void EditData();

public:
	void SetData(
		COBJItem *pItem,
		MAPATTRITEM *pATTR,
		MAPSTR *pSTR);

public:
	COBJItem *m_pItem;

	MAPATTRITEM *m_pATTR;
	MAPSTR *m_pSTR;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CEditListCtrl m_cListCtrl;
	CComboBox m_cType;

	CEdit m_cEditID;
	CEdit m_cValue;

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListUpdate( WPARAM wParam, LPARAM lParam);
	afx_msg void OnHdnItemchangedListAttr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
