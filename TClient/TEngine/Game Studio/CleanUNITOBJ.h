#pragma once
#include "afxwin.h"
#include "itembase.h"


// CCleanUNITOBJ 대화 상자입니다.

class CCleanUNITOBJ : public CDialog
{
	DECLARE_DYNAMIC(CCleanUNITOBJ)

public:
	CCleanUNITOBJ(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCleanUNITOBJ();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT_CLEAN_UNIT_OBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cUNITX;
	CComboBox m_cUNITZ;
	CString m_strUNITX;
	CString m_strUNITZ;

private:
	CEXTMAPItem* m_pItem;

public:
	void InitData(CEXTMAPItem* pItem);
	virtual BOOL OnInitDialog();
	afx_msg void OnCleanUNITOBJ();
	afx_msg void OnBnClickedButtonCleanAll();
};
