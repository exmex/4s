#pragma once
#include "GSListCtrl.h"
#include "MAPDoc.h"

// SelectObjByName 대화 상자입니다.

class CSelectObjByName : public CDialog
{
	DECLARE_DYNAMIC(CSelectObjByName)

public:
	CSelectObjByName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectObjByName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SELBYNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CGSListCtrl m_cSelectObjectByName;		// 오브젝트 리스트 컨트롤을 위한 변수
	CMAPDoc	*m_pDoc;
	int m_nShowALL;
	
	void InitDocument( CMAPDoc* pDoc , int nUNITLength, int nTileLength );
	void InitOBJList(CListCtrl *pSBNListCtrl);

public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnNMClickListObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSelobjDelete();
	afx_msg void OnBnClickedRadioEnableobj();
	afx_msg void OnBnClickedRadioAllobj();
	int m_nUnitLength;
	int m_nTileLength;
};
