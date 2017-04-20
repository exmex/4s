#pragma once
#include "afxcmn.h"

#include "TreeCtrlST.h"

// CTMacroView 폼 뷰입니다.

class CTMacroView : public CFormView
{
	DECLARE_DYNCREATE(CTMacroView)

protected:
	CTMacroView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTMacroView();

public:
	enum { IDD = IDD_TMACROVIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();	

	/////////////////////////////////////////////////
	// Protocol / Protocol Type
	MAPPROTOCOL m_mapProtocol;
	MAPPROTOCOLTYPE m_mapType;

	BOOL SerchProtocol(CString strProtocol); // Serch Protocol
	DWORD GetProtocolID(CString strProtocol); // Get Protocol ID
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Macro Protocol Info Sleep Set / Cancle
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonCancle();
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Macro Protocol Info Self Set / Cancle
	afx_msg void OnBnClickedButtonSelfset();
	afx_msg void OnBnClickedButtonSelfcancle();
	/////////////////////////////////////////////////
	
	/////////////////////////////////////////////////
	// Paramater List
	CListCtrl m_ListParam;
	
	void InsertParamList(CString strName, CString strType); // Insert Paramater List
	void SetParamListName(CString strName, CString strType); // Set Paramater List Name
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonInit();
	/////////////////////////////////////////////////	

	/////////////////////////////////////////////////	
	// Macro Protocol Info Setting
	afx_msg void OnBnClickedButtonSetting();
	/////////////////////////////////////////////////	

	/////////////////////////////////////////////////
	// Protocol Tree
	CTreeCtrlST m_TreeProtocol;

	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Macro Protocol Info Vector Insert / Delete / Update
	BOOL InsertPInfoVector(CString strName, CString strParent);
	void DeletePInfoVector(CString strName);	
	void DeleteAllPInfoVector();
	BOOL UpdatePInfoVector(CString strOld, CString strNew);
	/////////////////////////////////////////////////

	/////////////////////////////////////////////////
	// Macro Protocol Info View
	void MacroPInfoView(CTMacro* pMacro);
	void MacroPInfoChildView(CString strName, HTREEITEM hParent, VECTORPINFO pInfo);
	void ClearMacroPInfoView();
	/////////////////////////////////////////////////	

	/////////////////////////////////////////////////
	// Protocol Analysis
	afx_msg void OnBnClickedButtonAnalysis();
	void AllProtocolAnalysis();
	/////////////////////////////////////////////////	
};