#pragma once


#include "GSListCtrl.h"
#include "MAPDoc.h"

#define _HIDE_			0
#define _NONHIDE_		1

// CEditHideMode 대화 상자입니다.


class CEditHideModeDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditHideModeDlg)

public:
	CEditHideModeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditHideModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_HIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_cHide;			// 선택된 오브젝트를 Freeze Mode로 변경
	CButton m_cUnHide;			// 선택된 오브젝트를 Non-Hide Mode로 변경
	
	CGSListCtrl m_cHideObject;		// Hide 오브젝트 리스트 컨트롤을 위한 변수
	CGSListCtrl m_cNonHideObject;	// Non-Hide 오브젝트 리스트 컨트롤을 위한 변수

	CMAPDoc	*m_pDoc;
	int m_nUnitLength;
	int m_nTileLength;

public:
	virtual BOOL OnInitDialog(void);
	
	void InitDocument(CMAPDoc* pDoc, int nUNITLength, int nTileLength );
	afx_msg void OnBnClickedButtonNonhide();
	afx_msg void OnBnClickedButtonHide();
	afx_msg void OnNMClickListNonhide(NMHDR *pNMHDR, LRESULT *pResult);
};

