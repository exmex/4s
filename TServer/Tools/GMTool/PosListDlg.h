#pragma once
#include "afxcmn.h"


// CPosListDlg 대화 상자입니다.

class CPosListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPosListDlg)

public:
	CPosListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPosListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POSLISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listctrPos;
	
	DWORD	m_dwSelectIndex;	
	BYTE	m_bLoad;

	void Init(); // 리스트 박스 초기 세팅
	void LoadPosListFromData();
	void ItemDelete();
	bool ItemSelect();
	void ItemDelete_Multi(); // 리스트 컨트롤에서 다중 선택된 아이템 삭제
	void ClearListItem(); // 리스트 아이템 모두 삭제
	void InsertItemToListCtr(int _iRow, USERPOS* _USERPOS);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedPosfind();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
