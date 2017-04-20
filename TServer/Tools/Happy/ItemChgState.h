#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CItemChgState 대화 상자입니다.

class CItemChgState : public CDialog
{
	DECLARE_DYNAMIC(CItemChgState)

public:
	CItemChgState(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemChgState();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ITEMSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	CEdit m_ebInput;
	CStatic m_lblFindType;
	CListCtrl m_lcFindList;
	CComboBox m_cbWorld;

	BYTE	m_bFindType;
	BYTE	m_bEnable;
	BYTE	m_bMaxFindListColumn;
	BYTE	m_bWorldID;

public:
	void Init();
	void SetWorld();
	void InitFindListCtrl();
	void InsertItemToListCtr(ITEMSTATE* pITEMSTATE, int _iRow = -1);
	void ChangeItemState(WORD wItemID, BYTE bInitState);
	bool ItemSelect();
	void ItemMultiSelect();
	void ClearListItem();
	void ResultItemState(WORD wCount);

public:
	afx_msg void OnBnClickedRaItid();
	afx_msg void OnBnClickedRbItname();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnFinditem();	
	
};
