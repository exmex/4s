#pragma once
#include "afxwin.h"
#include "GSListCtrl.h"

// EditTargetUNITDlg 대화 상자입니다.

class CEditEnableUNITDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditEnableUNITDlg)

public:
	CEditEnableUNITDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditEnableUNITDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT_ENABLE_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_cDisable;
	CButton m_cEnable;

	CGSListCtrl m_cListDisableUnit;
	CGSListCtrl m_cListEnableUnit;

private:
	CTachyonHUGEMAP *m_pHugeMAP;

public:
	void InitData( CTachyonHUGEMAP *pHugeMap);

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnBnClickedButtonDisable();
	afx_msg void OnBnClickedButtonEnable();
	afx_msg void OnBnClickedButtonEanbleUnitList();
};
