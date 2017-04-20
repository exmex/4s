#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMonsterSpawnDlg 대화 상자입니다.

class CMonsterSpawnDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterSpawnDlg)

public:
	CMonsterSpawnDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonsterSpawnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MONSPAWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboWorld;
	CComboBox m_comboAction;

	DWORD m_dwMapID;
	DWORD m_dwChannel;
	DWORD m_dwSpawnID;
	DWORD m_dwTriggerID;
	DWORD m_dwHostID;
	DWORD m_dwRHID;
	DWORD m_dwRHType;

	CListCtrl m_listMon;

	void SetWorld();
	void AddItemData(DWORD dwMonID, DWORD dwHostID, BYTE bStat, float fPosX, float fPosY, float fPosZ);
	void UpdateData();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSearchMonster();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedCancel();

};
