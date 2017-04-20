#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "XListCtrl.h"
#include "resource.h"

// CTournament 대화 상자입니다.

class CTournament : public CDialog
{
	DECLARE_DYNAMIC(CTournament)

public:
	CTournament(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTournament();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOURNAMENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbWorld;;
	CComboBox m_cbWeek;
	CComboBox m_cbDay;
	CXListCtrl m_lcTEvent;
	CXListCtrl m_lcStep;
	CXListCtrl m_lcCharInfo;
	CXListCtrl m_lcReward;
	CListCtrl m_lcTList;

	WORD	m_wTournamentID;
	LPTOURNAMENT m_pTNT;

	BYTE m_bNewTNT;
	BYTE m_bSelectWorld;

	int	m_nFocusTList;
	int	m_nFocusTEvent;

public:
	void Init();
	void Init(LPTOURNAMENT pTNT);
	void InitTitle();
	void InitEventList();
	void InitStepList();
	void InitCharInfo();
	void InitReward();
	void InitWorld();

	void ResetStepList(BYTE bNewList = FALSE);

	void ClearCtrl();
	void ClearTList();
	void SetEnableCtrl(BYTE bEnable);

	void SetTitle(CString str);	
	void DelList(CString str);
	void ChangeList(CString strSource, CString strTarget);

	void AddList(CString str, BYTE bFocus = TRUE);
	void AddEvent(LPTPVP pTPVP);
	void AddReward(LPTREWARD pREWARD,BYTE bIsEvent);
	int  AddEventList(COLORREF crText = RGB(10,10,10));
	int	 AddRewardList(BYTE bCheckEvent = FALSE,COLORREF crText = RGB(10,10,10));
	void AddEntryChar(LPTNTCHAR pTNTCHAR);

	
	int	 ItemSelectTList();
	int	 ItemSelectTEvent();
	void SetColorEventRow(int nRow , COLORREF crText,COLORREF crBgColor);
	void SetColorRewardRow(int nRow , COLORREF crText,COLORREF crBgColor);

	void UpdateEntryChar(BYTE bAdd,LPTNTCHAR pTNTCHAR);
	void UpdateCharInfo(LPTNTCHAR pTNTCHAR);
	void UpdateStep(int nRow,int nCol);
	void UpdateEventList(int nRow, int nCol);
	void UpdateReward(int nRow,int nCol);	
	

	void SendNewTNT();
	void EnableBtnEventList(BYTE bEnable);
	void EnableBtnSTEP(BYTE bEnable);
	void UpdateFocus();
	
	afx_msg void OnBnClickedBtnAddEvent();
	afx_msg void OnBnClickedBtnDelEvent();
	afx_msg void OnBnClickedBtnAddEntry();
	afx_msg void OnBnClickedBtnDelEntry();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnNewTournament();	
	afx_msg void OnCbnSelchangeCbTList();
	afx_msg void OnBnClickedBtnDelt();
	afx_msg void OnBnClickedBtnAddreward();
	afx_msg void OnBnClickedBtnDelreward();		
	afx_msg void OnBnClickedBtnUpdateTEvent();
	afx_msg void OnBnClickedBtnUpdateTSchedule();
	afx_msg void OnBnClickedBtnEndChar();		
	afx_msg void OnNMRclickLcTevent(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnNMClickLcTevent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLcTevent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLcTlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnTregister();		
	afx_msg void OnBnClickedBtnTupdatedate();	

	afx_msg LRESULT OnXCtrlItem(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
