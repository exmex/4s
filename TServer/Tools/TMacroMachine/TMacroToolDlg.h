#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTMacroToolDlg 대화 상자입니다.

class CTMacroToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CTMacroToolDlg)

public:
	CTMacroToolDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTMacroToolDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TMACROTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	MAPTPROTOCOL m_mapProtocol;
	MAPTPROTOCOLTYPE m_mapType;

	DWORD m_dwID;
	MAPTVALUETEMP m_mapValue;
	MAPTMACROTEMP m_mapMacro;

	void SetBasicProtocol(MAPTPROTOCOL mapProtocol, MAPTPROTOCOLTYPE mapType);

	LPTMACROTEMP FindMacro(HTREEITEM hItem);
	LPTMACROTEMP FindMacro(CString strName);
	LPTVALUE FindValue(CString strName);
	LPTPROTOCOL FindProtocol(LPTMACROTEMP pMacro, HTREEITEM hItem);
	DWORD FindProtocolID(HTREEITEM hItem);
	HTREEITEM FindProtocolItem(HTREEITEM hItem, DWORD dwData);

	BOOL CheckValueSelf(BYTE bType, BYTE bValueType, CString strValue);
	BOOL CheckProtocol(CString strName, BYTE bCheck);
	BOOL CheckProtocol(DWORD dwOld, DWORD dwNew);
	BOOL CheckProtocol(HTREEITEM hItem, BYTE bCheck);
	BYTE IsUsedProtocol(DWORD dwID);
	VECTORDWORD SortProtocol(LPTMACROTEMP pMacro);

	void ClearMacro();
	void ClearProtocol();
	void ClearProtocolInfo();

	void UpdateMacro();
	void UpdateValue();
	void UpdateProtocol(LPTMACROTEMP pMacro);
	void UpdateProtocolInfo(LPTPROTOCOL pProtocol);

public:
	CTreeCtrl m_treeMacro;
	CTreeCtrl m_treeProtocol;
	CListCtrl m_listValue;
	CListCtrl m_listProtocol;
	CComboBox m_cbSelf;
	CComboBox m_cbPTLoop;
	CComboBox m_cbValue;
	CComboBox m_cbProtocol;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonValueadd();
	afx_msg void OnBnClickedButtonValueupdate();
	afx_msg void OnBnClickedButtonValuedel();
	afx_msg void OnBnClickedButtonMacroadd();
	afx_msg void OnBnClickedButtonPtupdate();
	afx_msg void OnBnClickedButtonPtset();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPtadd();
	afx_msg void OnBnClickedButtonMacrodel();
	afx_msg void OnBnClickedButtonPtdel();
	afx_msg void OnTvnEndlabeleditTreeMacro(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPtinit();
	afx_msg void OnTvnSelchangedTreeMacro(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void Serialize(CArchive& ar);
	afx_msg void OnBnClickedButtonPtupdateinit();
};
