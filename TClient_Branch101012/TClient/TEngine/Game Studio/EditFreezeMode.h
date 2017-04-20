#pragma once
#include "GSListCtrl.h"
#include "MAPDoc.h"

// CEditFreezeMode 대화 상자입니다.

class CEditFreezeModeDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditFreezeModeDlg)

public:
	CEditFreezeModeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditFreezeModeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FREEZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:
	CMAPDoc	*m_pDoc; 

	CButton m_cUnFreeze;			// 선택된 오브젝트를 Non-Hide Mode로 변경 버튼
	CButton m_cFreeze;				// 선택된 오브젝트를 Hide Mode로 변경 버튼

	CGSListCtrl m_cFreezeObject;	// Freeze 오브젝트 리스트 컨트롤을 위한 변수
	CGSListCtrl m_cNonFreezeObject;	// Non-Freeze 오브젝트 리스트 컨트롤을 위한 변수

	MAPDWORD		m_mapFreezeObject;			// Freeze 오브젝트 MAP
	MAPDWORD		m_mapNonFreezeObject;		// NON-Freeze 오브젝트 MAP

	MAPDWORD::iterator		m_itFreezeObj;		// Freeze MAP iterator
	MAPDWORD::iterator		m_itNonFreezeObj;	// NonFreeze MAP iterator

	int m_nUnitLength;
	int m_nTileLength;

public:
	void InitDocument(CMAPDoc* pDoc, int nUNITLength, int nTileLength );

public:
	virtual BOOL OnInitDialog(void);	
	afx_msg void OnBnClickedButtonNonfreeze();
	afx_msg void OnBnClickedButtonFreeze();
	afx_msg void OnNMClickListNonfreeze(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListFreeze(NMHDR *pNMHDR, LRESULT *pResult);
};
