#pragma once
#include "EXTMAPDoc.h"
#include "itembase.h"

// CEditUNITDlg 대화 상자입니다.

class CEditUNITDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditUNITDlg)

public:
	CEditUNITDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditUNITDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT_UNIT };

public:
	CEXTMAPItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditUNITEnable();
	afx_msg void OnCleanUNIT();
	void InitData( CEXTMAPItem *pItem);
};
