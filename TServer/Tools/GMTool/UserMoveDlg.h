#pragma once
#include "afxwin.h"

#include "PosListDlg.h"
#include "SavePosDlg.h"

// CUserMoveDlg 대화 상자입니다.

class CUserMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserMoveDlg)

public:
	CUserMoveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserMoveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_USERMOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CPosListDlg*	m_pdlgPosList;
	CSavePosDlg*	m_pdlgSavePos;

	CString m_strUser;
	DWORD m_dwChannel;
	DWORD m_dwMapID;
	CString m_strTarget1;
	CComboBox m_comboWorld;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	CString m_strPos;

public:
	void SetWorld();
	void SetEnableCoordinatesList(BOOL _bEnable); // [좌표입력] 관련 항목들을 Enable 할 것인가
	void SetEnableCharList(BOOL _bEnable); // [대상 캐릭터로의 소환] 관련 항목들을 Enable 할 것인가.
	void Init(); // [좌표입력] , [대상 캐릭터로의 소환] 부분을 모두 Disable 상태로 만든다.	
	void SetData(USERPOS& _UserPos);	
	void SetShowChildDlgAll(BOOL _bShow);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioCoord();
	afx_msg void OnBnClickedRadioChar();
	afx_msg void OnBnClickedLoadpos();
	afx_msg void OnBnClickedSavePos();
	afx_msg void OnBnClickedOk();  // [Move] 버튼 클릭했을 때
	afx_msg void OnBnClickedOk2(); // [Summon] 버튼 클릭했을 때
	afx_msg void OnBnClickedCancel();
};
