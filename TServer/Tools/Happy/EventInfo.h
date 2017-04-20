#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "XListCtrl.h"


// CEventInfo 대화 상자입니다.

class CEventInfo : public CDialog
{
	DECLARE_DYNAMIC(CEventInfo)

public:
	CEventInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EVENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbEventType;
	CComboBox m_cbServer;
	CComboBox m_cbCashItem;
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;
	CDateTimeCtrl m_dtStart2;
	CDateTimeCtrl m_dtEnd2;
	CListBox m_lbCashItemList;
	CListBox m_lbSpawnIDList;
	CListBox m_lcRegenList;
	CComboBox m_cbLotType;
	//CListBox m_lcLottery;
	CXListCtrl m_lcLottery;
	CDateTimeCtrl m_dtLotStart;

	DWORD m_dwEventIndex;
	BYTE m_bEventType;
	BYTE m_bDateType;
	

public:
	void Init();
	void Init(LPEVENTINFO pData);
	void SetEventID(DWORD dwEventIndex,BYTE bEventType);

	void SetViewCtrl(BYTE bEventID, BYTE bDateType, BYTE bEnable);
	void SetValueCtrl(BYTE bEventID,BYTE bEnable = TRUE);
	void SetMapIDCtrl(BYTE bEventID, BYTE bEnable = TRUE);
	void SetWorldCtrl(BYTE bEventID, BYTE bEnable = TRUE);
	void SetPeriodCtrl(BYTE bPeriodOn, BYTE bEventID, BYTE bEnable = TRUE);
	void SetDateCtrl(BYTE bEventID,BYTE bEnable = TRUE);

	void SetCashItemCtrl(BYTE bEventID, BYTE bEnable = TRUE);
	void SetSpawnIDCtrl(BYTE bEventID, BYTE bEnable = TRUE);
	void SetMonRegenCtrl(BYTE bEventID, BYTE bEnable = TRUE);
	void SetLotteryCtrl(BYTE bEventID, BYTE bEnable = TRUE);



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedClose();	
	afx_msg void OnBnClickedBtnCashAdd();
	afx_msg void OnBnClickedBtnCashDelete();
	
	afx_msg void OnCbnSelchangeCbEvent();
	afx_msg void OnBnClickedBtnSpawnAdd();
	afx_msg void OnBnClickedBtnSpawnDelete();	
	afx_msg void OnBnClickedBtnAddRegen();
	
	afx_msg void OnBnClickedBtnRegenDel();
	afx_msg void OnLbnSelchangeLbRegenlist();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedBtnLotadd();
	afx_msg void OnBnClickedBtnLotdel();
	afx_msg void OnEnChangeEbRegenmapid();
};
