// 현승룡 UserMove.h

#pragma once
#include "afxwin.h"


#include "SavePos.h"
#include "PosListDlg.h"
#include "afxcmn.h"

// CUserMove 대화 상자입니다.

class CUserMove : public CDialog
{
	DECLARE_DYNAMIC(CUserMove)

public:
	CUserMove(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserMove();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USERMOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	BYTE m_bType;
	CComboBox m_comboWorld;
	CListCtrl m_listctrPos;

	void SetWorld();
	
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedSavePos();
	afx_msg void OnBnClickedRadioCoord(); // '좌표입력' 라디오 버튼을 클릭했을 때
	afx_msg void OnBnClickedRadioChar();  // '대상캐릭터' 라디오 버튼을 클릭했을 때
	afx_msg void OnBnClickedLoadpos(); // 저장리스트를 보여주는 다이얼로그를 보여준다.

	void ShowLocationItem(BYTE bShow);	
	void Init(); // [좌표입력] , [대상 캐릭터로의 소환] 부분을 모두 Disable 상태로 만든다.
	
	void SetData(LPUSERPOS pUserPos);
	void InsertItemToListCtr(int _iRow, LPUSERPOS _USERPOS);
	void LoadPosListFromData();
	void ItemDelete();
	void ClearListItem();
	void MoveNow(LPUSERPOS pUserPos = NULL);

	BYTE FindListData(LPUSERPOS pPos);
	void DeleteListData(LPUSERPOS pPos);

	afx_msg void OnBnClickedDelPos();
	afx_msg void OnNMDblclkPosfind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedPosfind(NMHDR *pNMHDR, LRESULT *pResult);
};
