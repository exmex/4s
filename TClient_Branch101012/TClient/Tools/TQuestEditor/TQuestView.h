#if !defined(AFX_TQUESTVIEW_H__76A8E8C6_B434_40E6_A1E2_6EDA8A5FD764__INCLUDED_)
#define AFX_TQUESTVIEW_H__76A8E8C6_B434_40E6_A1E2_6EDA8A5FD764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TQuestView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTQuestView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"

class CTQuestView : public CFormView
{
protected:
	CTQuestView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTQuestView)

// Form Data
public:
	//{{AFX_DATA(CTQuestView)
	enum { IDD = IDD_DIALOG_QUEST };
	CStatic	m_cAcceptMsgText;
	CStatic	m_cRejectMsgText;
	CEdit	m_cRejectMsg;
	CEdit	m_cAcceptMsg;
	CEdit	m_cTakeData;
	CStatic	m_cTakeMethodText;
	CStatic	m_cTakeDataText;
	CStatic	m_cTermTypeText;
	CStatic	m_cTermIDText;
	CStatic	m_cTermCountText;
	CStatic	m_cRewardTypeText;
	CStatic	m_cRewardIDText;
	CStatic	m_cRewardCountText;
	CStatic	m_cObjectiveText;
	CStatic	m_cTermMsgText;
	CStatic	m_cConTypeText;
	CStatic	m_cConIDText;
	CStatic	m_cConCountText;
	CEdit	m_cTermID;
	CEdit	m_cRewardID;
	CEdit	m_cRewardCount;
	CEdit	m_cObjective;
	CEdit	m_cTermMsg;
	CEdit	m_cTermCount;
	CEdit	m_cConID;
	CEdit	m_cConCount;
	CButton	m_cDelTerm;
	CButton	m_cDelReward;
	CButton	m_cDelCon;
	CButton	m_cAddTerm;
	CButton	m_cAddReward;
	CButton	m_cAddCon;
	CTListCtrl	m_cCondition;
	CTListCtrl	m_cReward;
	CTListCtrl	m_cTerm;
	CStatic	m_cForceRunText;
	CEdit	m_cForceRun;
	CStatic	m_cCountMaxText;
	CEdit	m_cCountMax;
	CEdit	m_cTriggerMsg;
	CEdit	m_cTriggerID;
	CEdit	m_cCompleteMsg;
	CStatic	m_cCompleteMsgText;
	CStatic	m_cTriggerMsgText;
	CStatic	m_cTriggerIDText;
	CStatic	m_cTriggerTypeText;
	CStatic	m_cTypeText;
	CButton	m_cQuestID;
	CString	m_strCompleteMsg;
	CString	m_strTriggerMsg;
	CString	m_strQuestID;
	DWORD	m_dwTriggerID;
	BYTE	m_bCountMax;
	BYTE	m_bForceRun;
	BYTE	m_bConCount;
	DWORD	m_dwConID;
	BYTE	m_bTermCount;
	CString	m_strTermMsg;
	CString	m_strObjective;
	BYTE	m_bRewardCount;
	DWORD	m_dwRewardID;
	DWORD	m_dwTermID;
	BYTE	m_bTakeData;
	CString	m_strAcceptMsg;
	CString	m_strRejectMsg;
	CString	m_strSummaryMsg;
	int		m_bType;
	int		m_bTriggerType;
	int		m_bConType;
	int		m_bTermType;
	int		m_bRewardType;
	int		m_bTakeMethod;
	int		m_nClassIndex;
	DWORD	m_dwClassID;
	CComboBox	m_cTQClass;
	CEdit	m_cSummaryMsg;
	CStatic	m_cNPCNameText;
	CEdit	m_cNPCName;
	CString	m_strNPCName;
	CStatic	m_cReplyText;
	CEdit	m_cReply;
	CString	m_strReply;
	CStatic	m_cLevelText;
	CEdit	m_cLevel;
	BYTE	m_bLevel;
	CButton m_cMainQuest;
	CButton m_cMainCategory;
	BOOL m_bEditCtrlEnable;
	CButton m_cEditCtrlEnable;
	BOOL m_bClassMain;
	BOOL m_bMain;
	//}}AFX_DATA
	
// Attributes
public:
	CTQuestEditorDoc* GetDocument();
	LPTQUEST m_pQUEST;

// Operations
public:
	void UpdateData( LPTQUEST pQUEST);
	void EnableAllCtrl( BOOL bEnable);

	void UpdateCondition( LPTCONDITION pCONDITION);
	void UpdateReward( LPTQUESTREWARD pREWARD);
	void UpdateTerm( LPTQUESTTERM pTERM);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTQuestView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTQuestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTQuestView)
	afx_msg void OnKillfocusEditCompletemsg();
	afx_msg void OnKillfocusEditTriggerid();
	afx_msg void OnKillfocusEditTriggermsg();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnKillfocusEditCountmax();
	afx_msg void OnKillfocusEditForcerun();
	afx_msg void OnButtonAddcon();
	afx_msg void OnButtonAddreward();
	afx_msg void OnButtonAddterm();
	afx_msg void OnButtonDelcon();
	afx_msg void OnButtonDelreward();
	afx_msg void OnButtonDelterm();
	afx_msg void OnItemchangedListCondition(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListReward(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListTerm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditConcount();
	afx_msg void OnKillfocusEditConid();
	afx_msg void OnKillfocusEditCount();
	afx_msg void OnKillfocusEditIncompmsg();
	afx_msg void OnKillfocusEditObjective();
	afx_msg void OnKillfocusEditRewardcount();
	afx_msg void OnKillfocusEditRewardid();
	afx_msg void OnKillfocusEditTermid();
	afx_msg void OnKillfocusEditTakedata();
	afx_msg void OnKillfocusEditAcceptmsg();
	afx_msg void OnKillfocusEditRejectmsg();
	afx_msg void OnSelchangeEditType();
	afx_msg void OnSelchangeEditTriggertype();
	afx_msg void OnSelchangeEditContype();
	afx_msg void OnSelchangeEditTermtype();
	afx_msg void OnSelchangeEditRewardtype();
	afx_msg void OnSelchangeEditTakemethod();
	afx_msg void OnKillfocusEditSummarymsg();
	afx_msg void OnCbnSelchangeClassList();
	afx_msg void OnEnKillfocusEditNpcName();
	afx_msg void OnEnKillfocusEditPlayerAnswer();
	afx_msg void OnEnChangeClassId();
	afx_msg void OnLock();
	afx_msg void OnFind();
	afx_msg void OnBnClickedCheckMainCategory();
	afx_msg void OnBnClickedCheckMainQuest();
	afx_msg void OnEnKillfocusEditLevel();
	afx_msg void OnBnClickedCheckEditControlEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnShowClasstitledlg();
	void UpdateDataClassTitleDlg(void);
	afx_msg void OnCbnSelchangeComboMainclass();
	CComboBox m_ctrlMainQuest;
	CStatic m_cMainQuestText;
	afx_msg void OnBnClickedCheckConcheck();
	CButton m_cConCheck;
	BOOL m_bConditionCheck;
	afx_msg void OnEnKillfocusEditQuestmob();
	afx_msg void OnEnKillfocusEditQuesttime();
	afx_msg void OnEnKillfocusEditQuestpathmob();
	afx_msg void OnEnKillfocusEditTicketid();
	afx_msg void OnBnClickedCheckSendq();
	DWORD m_dwQuestMob;
	DWORD m_dwQuestTime;
	DWORD m_dwQuestPathMob;
	DWORD m_dwTicketID;
	BOOL m_bSendQ;
	CStatic m_cQuestMobText;
	CStatic m_cQuestTimeText;
	CStatic m_cQuestPathMobText;
	CStatic m_cTicketIDText;
	CEdit m_cQuestMob;
	CEdit m_cQuestTime;
	CEdit m_cQuestPathMob;
	CEdit m_cTicketID;
	CButton m_cSendQ;
};

inline CTQuestEditorDoc* CTQuestView::GetDocument()
   { return (CTQuestEditorDoc *) m_pDocument; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TQUESTVIEW_H__76A8E8C6_B434_40E6_A1E2_6EDA8A5FD764__INCLUDED_)
