// TQuestView.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TQuestEditorDoc.h"
#include "TQuestView.h"

#include "TConditionSet.h"
#include "TTermTextSet.h"
#include "TRewardSet.h"
#include "TQuestSet.h"
#include "TTitleSet.h"
#include "TTermSet.h"
#include ".\tquestview.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTQuestView

IMPLEMENT_DYNCREATE(CTQuestView, CFormView)

CTQuestView::CTQuestView()
:CFormView(CTQuestView::IDD)
, m_bEditCtrlEnable(FALSE)
{
	//{{AFX_DATA_INIT(CTQuestView)
	m_nClassIndex = 0;
	m_dwClassID = 0;
	m_strNPCName = _T("");
	m_strReply = _T("");
	m_strCompleteMsg = _T("");
	m_strTriggerMsg = _T("");
	m_strQuestID = _T("");
	m_dwTriggerID = 0;
	m_bCountMax = 0;
	m_bForceRun = 0;
	m_bConCount = 0;
	m_bLevel = 1;
	m_dwConID = 0;
	m_bTermCount = 0;
	m_strTermMsg = _T("");
	m_strObjective = _T("");
	m_bRewardCount = 0;
	m_dwRewardID = 0;
	m_dwTermID = 0;
	m_bTakeData = 0;
	m_strAcceptMsg = _T("");
	m_strRejectMsg = _T("");
	m_strSummaryMsg = _T("");
	m_bType = -1;
	m_bTriggerType = -1;
	m_bConType = -1;
	m_bTermType = -1;
	m_bRewardType = -1;
	m_bTakeMethod = -1;
	m_bClassMain = FALSE;
	m_bMain = FALSE;
	m_bConditionCheck = FALSE;
	m_dwQuestMob = 0;
	m_dwQuestTime = 0;
	m_dwQuestPathMob = 0;
	m_dwTicketID = 0;
	m_bSendQ = FALSE;
	//}}AFX_DATA_INIT
	m_pQUEST = NULL;
	
}

CTQuestView::~CTQuestView()
{
}

void CTQuestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTQuestView)
	DDX_Control(pDX, IDC_STATIC_ACCEPTMSG, m_cAcceptMsgText);
	DDX_Control(pDX, IDC_STATIC_REJECTMSG, m_cRejectMsgText);
	DDX_Control(pDX, IDC_EDIT_REJECTMSG, m_cRejectMsg);
	DDX_Control(pDX, IDC_EDIT_ACCEPTMSG, m_cAcceptMsg);
	DDX_Control(pDX, IDC_EDIT_TAKEDATA, m_cTakeData);
	DDX_Control(pDX, IDC_STATIC_TAKEMETHOD, m_cTakeMethodText);
	DDX_Control(pDX, IDC_STATIC_TAKEDATA, m_cTakeDataText);
	DDX_Control(pDX, IDC_STATIC_TERMTYPE, m_cTermTypeText);
	DDX_Control(pDX, IDC_STATIC_TERMID, m_cTermIDText);
	DDX_Control(pDX, IDC_STATIC_TERMCOUNT, m_cTermCountText);
	DDX_Control(pDX, IDC_STATIC_REWARDTYPE, m_cRewardTypeText);
	DDX_Control(pDX, IDC_STATIC_REWARDID, m_cRewardIDText);
	DDX_Control(pDX, IDC_STATIC_REWARDCOUNT, m_cRewardCountText);
	DDX_Control(pDX, IDC_STATIC_OBJECTIVETEXT, m_cObjectiveText);
	DDX_Control(pDX, IDC_STATIC_INCOMPMSG, m_cTermMsgText);
	DDX_Control(pDX, IDC_STATIC_CONTYPETEXT, m_cConTypeText);
	DDX_Control(pDX, IDC_STATIC_CONIDTEXT, m_cConIDText);
	DDX_Control(pDX, IDC_STATIC_CONCOUNTTEXT, m_cConCountText);
	DDX_Control(pDX, IDC_EDIT_TERMID, m_cTermID);
	DDX_Control(pDX, IDC_EDIT_REWARDID, m_cRewardID);
	DDX_Control(pDX, IDC_EDIT_REWARDCOUNT, m_cRewardCount);
	DDX_Control(pDX, IDC_EDIT_OBJECTIVE, m_cObjective);
	DDX_Control(pDX, IDC_EDIT_INCOMPMSG, m_cTermMsg);
	DDX_Control(pDX, IDC_EDIT_COUNT, m_cTermCount);
	DDX_Control(pDX, IDC_EDIT_CONID, m_cConID);
	DDX_Control(pDX, IDC_EDIT_CONCOUNT, m_cConCount);
	DDX_Control(pDX, IDC_BUTTON_DELTERM, m_cDelTerm);
	DDX_Control(pDX, IDC_BUTTON_DELREWARD, m_cDelReward);
	DDX_Control(pDX, IDC_BUTTON_DELCON, m_cDelCon);
	DDX_Control(pDX, IDC_BUTTON_ADDTERM, m_cAddTerm);
	DDX_Control(pDX, IDC_BUTTON_ADDREWARD, m_cAddReward);
	DDX_Control(pDX, IDC_BUTTON_ADDCON, m_cAddCon);
	DDX_Control(pDX, IDC_LIST_CONDITION, m_cCondition);
	DDX_Control(pDX, IDC_LIST_REWARD, m_cReward);
	DDX_Control(pDX, IDC_LIST_TERM, m_cTerm);
	DDX_Control(pDX, IDC_STATIC_FORCERUN, m_cForceRunText);
	DDX_Control(pDX, IDC_EDIT_FORCERUN, m_cForceRun);
	DDX_Control(pDX, IDC_STATIC_COUNTMAX, m_cCountMaxText);
	DDX_Control(pDX, IDC_EDIT_COUNTMAX, m_cCountMax);
	DDX_Control(pDX, IDC_EDIT_TRIGGERMSG, m_cTriggerMsg);
	DDX_Control(pDX, IDC_EDIT_TRIGGERID, m_cTriggerID);
	DDX_Control(pDX, IDC_EDIT_COMPLETEMSG, m_cCompleteMsg);
	DDX_Control(pDX, IDC_STATIC_COMPLETEMSG, m_cCompleteMsgText);
	DDX_Control(pDX, IDC_STATIC_TRIGGERMSG, m_cTriggerMsgText);
	DDX_Control(pDX, IDC_STATIC_TRIGGERID, m_cTriggerIDText);
	DDX_Control(pDX, IDC_STATIC_TRIGGERTYPE, m_cTriggerTypeText);
	DDX_Control(pDX, IDC_STATIC_TYPE, m_cTypeText);
	DDX_Control(pDX, IDC_STATIC_QUESTID, m_cQuestID);
	DDX_Text(pDX, IDC_EDIT_COMPLETEMSG, m_strCompleteMsg);
	DDX_Text(pDX, IDC_EDIT_TRIGGERMSG, m_strTriggerMsg);
	DDX_Text(pDX, IDC_STATIC_QUESTID, m_strQuestID);
	DDX_Text(pDX, IDC_EDIT_TRIGGERID, m_dwTriggerID);
	DDX_Text(pDX, IDC_EDIT_COUNTMAX, m_bCountMax);
	DDX_Text(pDX, IDC_EDIT_FORCERUN, m_bForceRun);
	DDX_Text(pDX, IDC_EDIT_CONCOUNT, m_bConCount);
	DDX_Text(pDX, IDC_EDIT_CONID, m_dwConID);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_bTermCount);
	DDX_Text(pDX, IDC_EDIT_INCOMPMSG, m_strTermMsg);
	DDX_Text(pDX, IDC_EDIT_OBJECTIVE, m_strObjective);
	DDX_Text(pDX, IDC_EDIT_REWARDCOUNT, m_bRewardCount);
	DDX_Text(pDX, IDC_EDIT_REWARDID, m_dwRewardID);
	DDX_Text(pDX, IDC_EDIT_TERMID, m_dwTermID);
	DDX_Text(pDX, IDC_EDIT_TAKEDATA, m_bTakeData);
	DDX_Text(pDX, IDC_EDIT_ACCEPTMSG, m_strAcceptMsg);
	DDX_Text(pDX, IDC_EDIT_REJECTMSG, m_strRejectMsg);
	DDX_Text(pDX, IDC_EDIT_SUMMARYMSG, m_strSummaryMsg);
	DDX_CBIndex(pDX, IDC_EDIT_TYPE, m_bType);
	DDX_CBIndex(pDX, IDC_EDIT_TRIGGERTYPE, m_bTriggerType);
	DDX_CBIndex(pDX, IDC_EDIT_CONTYPE, m_bConType);
	DDX_CBIndex(pDX, IDC_EDIT_TERMTYPE, m_bTermType);
	DDX_CBIndex(pDX, IDC_EDIT_REWARDTYPE, m_bRewardType);
	DDX_CBIndex(pDX, IDC_EDIT_TAKEMETHOD, m_bTakeMethod);
	DDX_CBIndex(pDX, IDC_CLASS_LIST, m_nClassIndex);
	DDX_Text(pDX, IDC_CLASS_ID, m_dwClassID);
	DDX_Control(pDX, IDC_CLASS_LIST, m_cTQClass);
	DDX_Control(pDX, IDC_EDIT_SUMMARYMSG, m_cSummaryMsg);
	DDX_Control(pDX, IDC_STATIC_NPC_NAME, m_cNPCNameText);
	DDX_Control(pDX, IDC_EDIT_NPC_NAME, m_cNPCName);
	DDX_Text(pDX, IDC_EDIT_NPC_NAME, m_strNPCName);
	DDX_Control(pDX, IDC_STATIC_PLAYER_ANSWER, m_cReplyText);
	DDX_Control(pDX, IDC_EDIT_PLAYER_ANSWER, m_cReply);
	DDX_Text(pDX, IDC_EDIT_PLAYER_ANSWER, m_strReply);
	DDX_Control(pDX, IDC_STATIC_LEVEL, m_cLevelText);
	DDX_Control(pDX, IDC_EDIT_LEVEL, m_cLevel);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_bLevel);

	DDX_Control(pDX, IDC_CHECK_MAIN_CATEGORY, m_cMainCategory);
	DDX_Check(pDX, IDC_CHECK_MAIN_CATEGORY, m_bClassMain);	

	//}}AFX_DATA_MAP

	DDX_Check(pDX, IDC_CHECK_EDIT_CONTROL_ENABLE, m_bEditCtrlEnable);
	DDX_Control(pDX, IDC_CHECK_EDIT_CONTROL_ENABLE, m_cEditCtrlEnable);
	DDX_Control(pDX, IDC_COMBO_MAINCLASS, m_ctrlMainQuest);
	DDX_Control(pDX, IDC_STATIC_MAINQUEST, m_cMainQuestText);
	DDX_Control(pDX, IDC_CHECK_CONCHECK, m_cConCheck);
	DDX_Check(pDX, IDC_CHECK_CONCHECK, m_bConditionCheck);
	DDX_Text(pDX, IDC_EDIT_QUESTMOB, m_dwQuestMob);
	DDX_Text(pDX, IDC_EDIT_QUESTTIME, m_dwQuestTime);
	DDX_Text(pDX, IDC_EDIT_QUESTPATHMOB, m_dwQuestPathMob);
	DDX_Text(pDX, IDC_EDIT_TICKETID, m_dwTicketID);
	DDX_Check(pDX, IDC_CHECK_SENDQ, m_bSendQ);
	DDX_Control(pDX, IDC_STATIC_QUESTMOB, m_cQuestMobText);
	DDX_Control(pDX, IDC_STATIC_QUESTTIME, m_cQuestTimeText);
	DDX_Control(pDX, IDC_STATIC_QUESTPATHMOB, m_cQuestPathMobText);
	DDX_Control(pDX, IDC_STATIC_TICKETID, m_cTicketIDText);
	DDX_Control(pDX, IDC_EDIT_QUESTMOB, m_cQuestMob);
	DDX_Control(pDX, IDC_EDIT_QUESTTIME, m_cQuestTime);
	DDX_Control(pDX, IDC_EDIT_QUESTPATHMOB, m_cQuestPathMob);
	DDX_Control(pDX, IDC_EDIT_TICKETID, m_cTicketID);
	DDX_Control(pDX, IDC_CHECK_SENDQ, m_cSendQ);
}


BEGIN_MESSAGE_MAP(CTQuestView, CFormView)
	//{{AFX_MSG_MAP(CTQuestView)
	ON_EN_KILLFOCUS(IDC_EDIT_COMPLETEMSG, OnKillfocusEditCompletemsg)
	ON_EN_KILLFOCUS(IDC_EDIT_TRIGGERID, OnKillfocusEditTriggerid)
	ON_EN_KILLFOCUS(IDC_EDIT_TRIGGERMSG, OnKillfocusEditTriggermsg)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNTMAX, OnKillfocusEditCountmax)
	ON_EN_KILLFOCUS(IDC_EDIT_FORCERUN, OnKillfocusEditForcerun)
	ON_BN_CLICKED(IDC_BUTTON_ADDCON, OnButtonAddcon)
	ON_BN_CLICKED(IDC_BUTTON_ADDREWARD, OnButtonAddreward)
	ON_BN_CLICKED(IDC_BUTTON_ADDTERM, OnButtonAddterm)
	ON_BN_CLICKED(IDC_BUTTON_DELCON, OnButtonDelcon)
	ON_BN_CLICKED(IDC_BUTTON_DELREWARD, OnButtonDelreward)
	ON_BN_CLICKED(IDC_BUTTON_DELTERM, OnButtonDelterm)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CONDITION, OnItemchangedListCondition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REWARD, OnItemchangedListReward)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TERM, OnItemchangedListTerm)
	ON_EN_KILLFOCUS(IDC_EDIT_CONCOUNT, OnKillfocusEditConcount)
	ON_EN_KILLFOCUS(IDC_EDIT_CONID, OnKillfocusEditConid)
	ON_EN_KILLFOCUS(IDC_EDIT_COUNT, OnKillfocusEditCount)
	ON_EN_KILLFOCUS(IDC_EDIT_INCOMPMSG, OnKillfocusEditIncompmsg)
	ON_EN_KILLFOCUS(IDC_EDIT_OBJECTIVE, OnKillfocusEditObjective)
	ON_EN_KILLFOCUS(IDC_EDIT_REWARDCOUNT, OnKillfocusEditRewardcount)
	ON_EN_KILLFOCUS(IDC_EDIT_REWARDID, OnKillfocusEditRewardid)
	ON_EN_KILLFOCUS(IDC_EDIT_TERMID, OnKillfocusEditTermid)
	ON_EN_KILLFOCUS(IDC_EDIT_TAKEDATA, OnKillfocusEditTakedata)
	ON_EN_KILLFOCUS(IDC_EDIT_ACCEPTMSG, OnKillfocusEditAcceptmsg)
	ON_EN_KILLFOCUS(IDC_EDIT_REJECTMSG, OnKillfocusEditRejectmsg)
	ON_CBN_SELCHANGE(IDC_EDIT_TYPE, OnSelchangeEditType)
	ON_CBN_SELCHANGE(IDC_EDIT_TRIGGERTYPE, OnSelchangeEditTriggertype)
	ON_CBN_SELCHANGE(IDC_EDIT_CONTYPE, OnSelchangeEditContype)
	ON_CBN_SELCHANGE(IDC_EDIT_TERMTYPE, OnSelchangeEditTermtype)
	ON_CBN_SELCHANGE(IDC_EDIT_REWARDTYPE, OnSelchangeEditRewardtype)
	ON_CBN_SELCHANGE(IDC_EDIT_TAKEMETHOD, OnSelchangeEditTakemethod)
	ON_EN_KILLFOCUS(IDC_EDIT_SUMMARYMSG, OnKillfocusEditSummarymsg)
	ON_CBN_SELCHANGE(IDC_CLASS_LIST, OnCbnSelchangeClassList)
	ON_EN_KILLFOCUS(IDC_EDIT_NPC_NAME, OnEnKillfocusEditNpcName)
	ON_EN_KILLFOCUS(IDC_EDIT_PLAYER_ANSWER, OnEnKillfocusEditPlayerAnswer)
	ON_EN_KILLFOCUS(IDC_EDIT_LEVEL, OnEnKillfocusEditLevel)
	ON_EN_CHANGE(IDC_CLASS_ID, OnEnChangeClassId)
	ON_BN_CLICKED(IDC_CHECK_EDIT_CONTROL_ENABLE, OnBnClickedCheckEditControlEnable)
	ON_COMMAND(ID_LOCK, OnLock)
	ON_COMMAND(ID_FIND, OnFind)
	ON_BN_CLICKED(IDC_CHECK_MAIN_CATEGORY, OnBnClickedCheckMainCategory)	
	
	//}}AFX_MSG_MAP
    //ON_BN_CLICKED(IDC_CHECK_MAIN_QUEST, OnBnClickedCheckMainQuest)
	ON_COMMAND(ID_SHOW_CLASSTITLEDLG, OnShowClasstitledlg)
	ON_CBN_SELCHANGE(IDC_COMBO_MAINCLASS, OnCbnSelchangeComboMainclass)
	ON_BN_CLICKED(IDC_CHECK_CONCHECK, OnBnClickedCheckConcheck)
	ON_EN_KILLFOCUS(IDC_EDIT_QUESTMOB, OnEnKillfocusEditQuestmob)
	ON_EN_KILLFOCUS(IDC_EDIT_QUESTTIME, OnEnKillfocusEditQuesttime)
	ON_EN_KILLFOCUS(IDC_EDIT_QUESTPATHMOB, OnEnKillfocusEditQuestpathmob)
	ON_EN_KILLFOCUS(IDC_EDIT_TICKETID, OnEnKillfocusEditTicketid)
	ON_BN_CLICKED(IDC_CHECK_SENDQ, OnBnClickedCheckSendq)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTQuestView diagnostics

#ifdef _DEBUG
void CTQuestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTQuestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTQuestView message handlers


void CTQuestView::UpdateData( LPTQUEST pQUEST)
{
	m_pQUEST = pQUEST;
	m_bEditCtrlEnable = TRUE;

	if(m_pQUEST)
	{
		m_strQuestID.Format( " Quest ID : %d ", m_pQUEST->m_dwQuestID);
		m_strCompleteMsg.Format( "%s", m_pQUEST->m_strComplete);
		m_strTriggerMsg.Format( "%s", m_pQUEST->m_strMessage);
		m_strAcceptMsg.Format( "%s", m_pQUEST->m_strAccept);
		m_strRejectMsg.Format( "%s", m_pQUEST->m_strReject);
		m_strSummaryMsg.Format( "%s", m_pQUEST->m_strSummary);
		m_strNPCName.Format( "%s", m_pQUEST->m_strNPCName);
		m_strReply.Format( "%s", m_pQUEST->m_strReply);

		m_bTriggerType = m_pQUEST->m_bTriggerType;
		m_dwTriggerID = m_pQUEST->m_dwTriggerID;
		m_bForceRun = m_pQUEST->m_bForceRun;
		m_bMain = m_pQUEST->m_bMain;
		m_bLevel = m_pQUEST->m_bLevel;
		m_bType = m_pQUEST->m_bType;
		m_bCountMax = m_pQUEST->m_bCountMax;
		m_bConditionCheck = m_pQUEST->m_bConditionCheck;

		m_dwClassID = m_pQUEST->m_dwClassID;
		m_bMain = m_pQUEST->m_bMain;
		m_bClassMain = GetDocument()->GetClassMain(m_pQUEST->m_dwClassID);

		m_nClassIndex = 0;

		for( int i=0; i<m_cTQClass.GetCount(); i++)
		{
			LPTQCLASS pTQCLASS = (LPTQCLASS) m_cTQClass.GetItemData(i);

			if( pTQCLASS->m_dwClassID == m_pQUEST->m_dwClassID )
				m_nClassIndex = i;
		}

		m_cCondition.DeleteAllItems();
		m_cReward.DeleteAllItems();
		m_cTerm.DeleteAllItems();

		for( int i=0; i<INT(m_pQUEST->m_vCondition.size()); i++)
		{
			CString strText;

			strText.Format( "Condition [%d]", i);
			int nIndex = m_cCondition.InsertItem( i, strText);
			m_cCondition.SetItemData( nIndex, (DWORD) m_pQUEST->m_vCondition[i]);
		}

		if( m_cCondition.GetItemCount() > 0 )
			m_cCondition.SetCurSel(0);

		for( i=0; i<INT(m_pQUEST->m_vReward.size()); i++)
		{
			CString strText;

			strText.Format( "Reward [%d]", i);
			int nIndex = m_cReward.InsertItem( i, strText);
			m_cReward.SetItemData( nIndex, (DWORD) m_pQUEST->m_vReward[i]);
		}

		if( m_cReward.GetItemCount() > 0 )
			m_cReward.SetCurSel(0);

		for( i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
		{
			CString strText;

			strText.Format( "Term [%d]", i);
			int nIndex = m_cTerm.InsertItem( i, strText);
			m_cTerm.SetItemData( nIndex, (DWORD) m_pQUEST->m_vTerm[i]);
		}

		if( m_cTerm.GetItemCount() > 0 )
			m_cTerm.SetCurSel(0);

		m_ctrlMainQuest.SetCurSel(m_pQUEST->m_bMain);
	}
	else
	{
		m_strQuestID.Format(" No quest selected ");
		m_strCompleteMsg.Empty();
		m_strTriggerMsg.Empty();
		m_strAcceptMsg.Empty();
		m_strRejectMsg.Empty();
		m_strSummaryMsg.Empty();
		m_strNPCName.Empty();
		m_strReply.Empty();

		m_cCondition.DeleteAllItems();
		m_cReward.DeleteAllItems();
		m_cTerm.DeleteAllItems();

		m_bTriggerType = 0;
		m_dwTriggerID = 0;
		m_bForceRun = 0;
		m_bMain = 0;
		m_bLevel = 1;
		m_bType = 0;
		m_bCountMax = 0;
		m_bConditionCheck = 0;
	}

	CFormView::UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	OnCbnSelchangeClassList();

	CTQuestEditorDoc *pDoc = GetDocument();
	if(pDoc->m_pClassTitleDlg != NULL)
		UpdateDataClassTitleDlg();
}	

void CTQuestView::EnableAllCtrl( BOOL bEnable)
{
	LPTQUESTREWARD pREWARD = NULL;
	LPTCONDITION pCONDITION = NULL;
	LPTQUESTTERM pTERM = NULL;

	m_cCountMaxText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable/* && !m_pQUEST->m_dwParentID */);
	m_cCountMax.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable/* && !m_pQUEST->m_dwParentID */);

	int nIndex = m_cReward.GetCurSel();
	if( nIndex >= 0 )
		pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

	nIndex = m_cTerm.GetCurSel();
	if( nIndex >= 0 )
		pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

	nIndex = m_cCondition.GetCurSel();
	if( nIndex >= 0 )
		pCONDITION = (LPTCONDITION) m_cCondition.GetItemData(nIndex);

	m_cEditCtrlEnable.EnableWindow(bEnable && m_pQUEST);

	m_cTermTypeText.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermIDText.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermCountText.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cObjectiveText.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermMsgText.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermID.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cObjective.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermMsg.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	m_cTermCount.EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);

	m_cRewardTypeText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cRewardIDText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cRewardCountText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cRewardID.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cRewardCount.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cTakeData.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cTakeMethodText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cTakeDataText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestMob.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestMobText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestTime.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestTimeText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestPathMob.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cQuestPathMobText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cTicketID.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cTicketIDText.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	m_cSendQ.EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);

	m_cConTypeText.EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);
	m_cConIDText.EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);
	m_cConCountText.EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);
	m_cConID.EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);
	m_cConCount.EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);

	m_cDelTerm.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cDelReward.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cDelCon.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cAddTerm.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cAddReward.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cAddCon.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cCondition.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cReward.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTerm.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cForceRunText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cForceRun.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTriggerMsg.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cCompleteMsg.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cCompleteMsgText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTriggerTypeText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTriggerIDText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTriggerID.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTriggerMsgText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cTypeText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cQuestID.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cAcceptMsgText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cRejectMsgText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cAcceptMsg.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cRejectMsg.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cSummaryMsg.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cNPCNameText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cNPCName.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cReplyText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cReply.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cLevelText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cLevel.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cMainCategory.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable && m_pQUEST->m_dwClassID);
	m_cMainQuestText.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
    m_ctrlMainQuest.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	m_cConCheck.EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	
	GetDlgItem(IDC_EDIT_TYPE)->EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	GetDlgItem(IDC_EDIT_TRIGGERTYPE)->EnableWindow(bEnable && m_pQUEST && !m_bEditCtrlEnable);
	GetDlgItem(IDC_EDIT_CONTYPE)->EnableWindow(bEnable && m_pQUEST && pCONDITION && !m_bEditCtrlEnable);
	GetDlgItem(IDC_EDIT_TERMTYPE)->EnableWindow(bEnable && m_pQUEST && pTERM && !m_bEditCtrlEnable);
	GetDlgItem(IDC_EDIT_REWARDTYPE)->EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);
	GetDlgItem(IDC_EDIT_TAKEMETHOD)->EnableWindow(bEnable && m_pQUEST && pREWARD && !m_bEditCtrlEnable);

	GetDlgItem(IDC_STATIC_CLASS_INFO)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
	GetDlgItem(IDC_CLASS_LIST)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
	GetDlgItem(IDC_CLASS_ID)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
	GetDlgItem(IDC_STATIC_CLASS_NAME)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
	GetDlgItem(IDC_STATIC_CLASS_ID)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);

	GetDlgItem(IDC_STATIC_SUMMARYMSG)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
	GetDlgItem(IDC_COMBO_MAINCLASS)->EnableWindow( bEnable && m_pQUEST  && !m_bEditCtrlEnable);
}

void CTQuestView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CRect rect;

	m_cCondition.GetClientRect(&rect);
	m_cCondition.InsertColumn( 0, "Condition", LVCFMT_LEFT, rect.Width());
	m_cCondition.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_cReward.GetClientRect(&rect);
	m_cReward.InsertColumn( 0, "Reward", LVCFMT_LEFT, rect.Width());
	m_cReward.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_cTerm.GetClientRect(&rect);
	m_cTerm.InsertColumn( 0, "Term", LVCFMT_LEFT, rect.Width());
	m_cTerm.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	GetDocument()->m_pTQuestView = this;
	
	static CString strMainQuest[] = { "일반", "중요", "대장", "던전"};

	int count = m_ctrlMainQuest.GetCount();
	for(int j = 0; j < count; j++)
		m_ctrlMainQuest.DeleteString(j);

	for(int i = 0; i < 4; i++)
		m_ctrlMainQuest.AddString(strMainQuest[i]);
	m_ctrlMainQuest.SetCurSel(0);

	UpdateData(NULL);
}

void CTQuestView::OnKillfocusEditCompletemsg() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strCompleteMsg.GetLength() > 1023 )
			m_strCompleteMsg = m_strCompleteMsg.Left(1023);

		if( m_pQUEST->m_strComplete != m_strCompleteMsg )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szComplete = m_strCompleteMsg;
				set.Update();
				set.Close();

				m_pQUEST->m_strComplete = m_strCompleteMsg;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditTriggermsg() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strTriggerMsg.GetLength() > 1023 )
			m_strTriggerMsg = m_strTriggerMsg.Left(1023);

		if( m_pQUEST->m_strMessage != m_strTriggerMsg )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szMessage = m_strTriggerMsg;
				set.Update();
				set.Close();

				m_pQUEST->m_strMessage = m_strTriggerMsg;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditAcceptmsg() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strAcceptMsg.GetLength() > 1023 )
			m_strAcceptMsg = m_strAcceptMsg.Left(1023);

		if( m_pQUEST->m_strAccept != m_strAcceptMsg )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szAccept = m_strAcceptMsg;
				set.Update();
				set.Close();

				m_pQUEST->m_strAccept = m_strAcceptMsg;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditRejectmsg() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strRejectMsg.GetLength() > 1023 )
			m_strRejectMsg = m_strRejectMsg.Left(1023);

		if( m_pQUEST->m_strReject != m_strRejectMsg )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szReject = m_strRejectMsg;
				set.Update();
				set.Close();

				m_pQUEST->m_strReject = m_strRejectMsg;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditTriggerid() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_dwTriggerID != m_dwTriggerID )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_dwTriggerID = m_dwTriggerID;
				set.Update();
				set.Close();

				m_pQUEST->m_dwTriggerID = m_dwTriggerID;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditCountmax() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bCountMax != m_bCountMax )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bCountMax = m_bCountMax;
				set.Update();
				set.Close();

				m_pQUEST->m_bCountMax = m_bCountMax;
			}
		}
	}
}

void CTQuestView::OnKillfocusEditForcerun() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bForceRun != m_bForceRun )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bForceRun = m_bForceRun;
				set.Update();
				set.Close();

				m_pQUEST->m_bForceRun = m_bForceRun;
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditLevel()
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bLevel != m_bLevel )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bLevel = m_bLevel;
				set.Update();
				set.Close();

				m_pQUEST->m_bLevel = m_bLevel;
			}
		}
	}
}

void CTQuestView::OnEditCopy() 
{
	CWnd *pWnd = GetFocus();

	if( pWnd && pWnd->IsKindOf(_RUNTIME_CLASS(CEdit)) )
	{
		CEdit *pEdit = (CEdit *) pWnd;

		if(pEdit)
			pEdit->Copy();
	}
}

void CTQuestView::OnEditCut() 
{
	CWnd *pWnd = GetFocus();

	if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
	{
		CEdit *pEdit = (CEdit *) pWnd;

		if(pEdit)
			pEdit->Cut();
	}
}

void CTQuestView::OnEditPaste() 
{
	CWnd *pWnd = GetFocus();

	if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
	{
		CEdit *pEdit = (CEdit *) pWnd;

		if(pEdit)
			pEdit->Paste();
	}
}

void CTQuestView::OnEditUndo() 
{
	CWnd *pWnd = GetFocus();

	if( pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CEdit)) )
	{
		CEdit *pEdit = (CEdit *) pWnd;

		if(pEdit)
			pEdit->Undo();
	}
}

void CTQuestView::OnButtonAddcon() 
{
	if(m_pQUEST)
	{
		CTQuestEditorDoc *pDoc = GetDocument();
		CTConditionSet set(&pDoc->m_db);

		if(set.Open())
		{
			set.AddNew();
			set.m_dwQuestID = m_pQUEST->m_dwQuestID;
			set.m_bConditionType = 0;
			set.m_dwConditionID = 0;
			set.m_bCount = 0;
			set.Update();
			set.Requery();
			set.MoveLast();

			LPTCONDITION pCONDITION = new TCONDITION;
			int nIndex = m_cCondition.GetItemCount();
			CString strText;

			pCONDITION->m_bConditionType = set.m_bConditionType;
			pCONDITION->m_dwConditionID = set.m_dwConditionID;
			pCONDITION->m_bCount = set.m_bCount;
			pCONDITION->m_dwID = set.m_dwID;

			strText.Format( "Condition [%d]", nIndex);
			nIndex = m_cCondition.InsertItem( nIndex, strText);
			m_cCondition.SetItemData( nIndex, (DWORD) pCONDITION);

			m_pQUEST->m_vCondition.push_back(pCONDITION);
			set.Close();
		}
	}
}

void CTQuestView::OnButtonAddreward()
{
	if(m_pQUEST)
	{
		CTQuestEditorDoc *pDoc = GetDocument();
		CTRewardSet set(&pDoc->m_db);

		if(set.Open())
		{
			set.AddNew();
			set.m_dwQuestID = m_pQUEST->m_dwQuestID;
			set.m_bRewardType = 0;
			set.m_dwRewardID = 0;
			set.m_bTakeMethod = 0;
			set.m_bTakeData = 0;
			set.m_bCount = 0;
			set.m_dwQuestMob = 0;
			set.m_dwQuestTime = 0;
			set.m_dwQuestPathMob = 0;
			set.m_dwTicketID = 0;
			set.m_bSendQ = 0;
			set.Update();
			set.Requery();
			set.MoveLast();

			LPTQUESTREWARD pREWARD = new TQUESTREWARD;
			int nIndex = m_cReward.GetItemCount();
			CString strText;

			pREWARD->m_bRewardType = set.m_bRewardType;
			pREWARD->m_dwRewardID = set.m_dwRewardID;
			pREWARD->m_bTakeMethod = set.m_bTakeMethod;
			pREWARD->m_bTakeData = set.m_bTakeData;
			pREWARD->m_bCount = set.m_bCount;
			pREWARD->m_dwID = set.m_dwID;
			pREWARD->m_dwQuestMob = set.m_dwQuestMob;
			pREWARD->m_dwQuestPathMob = set.m_dwQuestPathMob;
			pREWARD->m_dwQuestTime = set.m_dwQuestTime;
			pREWARD->m_dwTicketID = set.m_dwTicketID;
			pREWARD->m_bSendQ = set.m_bSendQ;

			strText.Format( "Reward [%d]", nIndex);
			nIndex = m_cReward.InsertItem( nIndex, strText);
			m_cReward.SetItemData( nIndex, (DWORD) pREWARD);

			m_pQUEST->m_vReward.push_back(pREWARD);
			set.Close();
		}
	}
}

void CTQuestView::OnButtonAddterm()
{
	if(m_pQUEST)
	{
		CTQuestEditorDoc *pDoc = GetDocument();
		LPTQUESTTERM pTERM = new TQUESTTERM();

		CTTermTextSet setTermText(&pDoc->m_db);
		CTTermSet setTerm(&pDoc->m_db);

		if(setTerm.Open())
		{
			setTerm.AddNew();
			setTerm.m_dwQuestID = m_pQUEST->m_dwQuestID;
			setTerm.m_bTermType = 0;
			setTerm.m_dwTermID = 0;
			setTerm.m_bCount = 0;
			setTerm.Update();
			setTerm.Requery();
			setTerm.MoveLast();

			pTERM->m_bTermType = setTerm.m_bTermType;
			pTERM->m_dwTermID = setTerm.m_dwTermID;
			pTERM->m_bCount = setTerm.m_bCount;
			pTERM->m_dwID = setTerm.m_dwID;

			setTerm.Close();
		}

		if(setTermText.Open())
		{
			setTermText.AddNew();
			setTermText.m_dwQuestID = m_pQUEST->m_dwQuestID;
			setTermText.m_dwTermID = pTERM->m_dwID;
			setTermText.m_szObjective.Empty();
			setTermText.m_szMessage.Empty();
			setTermText.Update();
			setTermText.Requery();
			setTermText.MoveLast();

			pTERM->m_strObjective = setTermText.m_szObjective;
			pTERM->m_strMessage = setTermText.m_szMessage;

			setTermText.Close();
		}

		int nIndex = m_cTerm.GetItemCount();
		CString strText;

		strText.Format( "Term [%d]", nIndex);
		nIndex = m_cTerm.InsertItem( nIndex, strText);
		m_cTerm.SetItemData( nIndex, (DWORD) pTERM);

		m_pQUEST->m_vTerm.push_back(pTERM);
	}
}

void CTQuestView::OnButtonDelcon() 
{
	if(m_pQUEST)
	{
		int nIndex = m_cCondition.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTCONDITION pCONDITION = (LPTCONDITION) m_cCondition.GetItemData(nIndex);

			if(pCONDITION)
			{
				VTCONDITION::iterator it;

				CTQuestEditorDoc *pDoc = GetDocument();
				CTConditionSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pCONDITION->m_dwID);
				if(set.Open())
				{
					if(!set.IsEOF())
					{
						set.MoveFirst();
						set.Delete();
					}

					set.Close();
				}

				for( it = m_pQUEST->m_vCondition.begin(); it != m_pQUEST->m_vCondition.end(); it++)
					if( (*it) == pCONDITION )
					{
						m_pQUEST->m_vCondition.erase(it);
						delete pCONDITION;

						break;
					}

				m_cCondition.DeleteItem(nIndex);
				for( int i=0; i<m_cCondition.GetItemCount(); i++)
				{
					CString strText;

					strText.Format( "Condition [%d]", i);
					m_cCondition.SetItemText( i, 0, strText);
				}
			}
		}
	}
}

void CTQuestView::OnButtonDelreward() 
{
	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if(pREWARD)
			{
				VTQUESTREWARD::iterator it;

				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					if(!set.IsEOF())
					{
						set.MoveFirst();
						set.Delete();
					}

					set.Close();
				}

				for( it = m_pQUEST->m_vReward.begin(); it != m_pQUEST->m_vReward.end(); it++)
					if( (*it) == pREWARD )
					{
						m_pQUEST->m_vReward.erase(it);
						delete pREWARD;

						break;
					}

				m_cReward.DeleteItem(nIndex);
				for( int i=0; i<m_cReward.GetItemCount(); i++)
				{
					CString strText;

					strText.Format( "Reward [%d]", i);
					m_cReward.SetItemText( i, 0, strText);
				}
			}
		}
	}
}

void CTQuestView::OnButtonDelterm() 
{
	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if(pTERM)
			{
				VTQUESTTERM::iterator it;

				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermTextSet setTermText(&pDoc->m_db);
				CTTermSet setTerm(&pDoc->m_db);

				setTerm.m_strFilter.Format( "dwID = %d", pTERM->m_dwID);
				if(setTerm.Open())
				{
					if(!setTerm.IsEOF())
					{
						setTerm.MoveFirst();
						setTerm.Delete();
					}

					setTerm.Close();
				}

				setTermText.m_strFilter.Format( "dwTermID = %d", pTERM->m_dwID);
				if(setTermText.Open())
				{
					if(!setTermText.IsEOF())
					{
						setTermText.MoveFirst();
						setTermText.Delete();
					}

					setTermText.Close();
				}

				for( it = m_pQUEST->m_vTerm.begin(); it != m_pQUEST->m_vTerm.end(); it++)
					if( (*it) == pTERM )
					{
						m_pQUEST->m_vTerm.erase(it);
						delete pTERM;

						break;
					}

				m_cTerm.DeleteItem(nIndex);
				for( int i=0; i<m_cTerm.GetItemCount(); i++)
				{
					CString strText;

					strText.Format( "Term [%d]", i);
					m_cTerm.SetItemText( i, 0, strText);
				}
			}
		}
	}
}

void CTQuestView::OnItemchangedListCondition(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( (pNMListView->uChanged & LVIF_STATE) &&
		(pNMListView->uNewState & LVIS_SELECTED) &&
		(pNMListView->uOldState & LVIS_SELECTED) != (pNMListView->uNewState & LVIS_SELECTED) )
	{
		LPTCONDITION pCONDITION = (LPTCONDITION) pNMListView->lParam;
		UpdateCondition(pCONDITION);
	}

	*pResult = 0;
}

void CTQuestView::OnItemchangedListReward(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( (pNMListView->uChanged & LVIF_STATE) &&
		(pNMListView->uNewState & LVIS_SELECTED) &&
		(pNMListView->uOldState & LVIS_SELECTED) != (pNMListView->uNewState & LVIS_SELECTED) )
	{
		LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) pNMListView->lParam;
		UpdateReward(pREWARD);
	}

	*pResult = 0;
}

void CTQuestView::OnItemchangedListTerm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( (pNMListView->uChanged & LVIF_STATE) &&
		(pNMListView->uNewState & LVIS_SELECTED) &&
		(pNMListView->uOldState & LVIS_SELECTED) != (pNMListView->uNewState & LVIS_SELECTED) )
	{
		LPTQUESTTERM pTERM = (LPTQUESTTERM) pNMListView->lParam;
		UpdateTerm(pTERM);
	}

	*pResult = 0;
}

void CTQuestView::UpdateCondition( LPTCONDITION pCONDITION)
{
	m_bConType = pCONDITION->m_bConditionType;
	m_bConCount = pCONDITION->m_bCount;
	m_dwConID = pCONDITION->m_dwConditionID;

	CFormView::UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CTQuestView::UpdateReward( LPTQUESTREWARD pREWARD)
{
	m_bRewardType = pREWARD->m_bRewardType;
	m_dwRewardID = pREWARD->m_dwRewardID;
	m_bRewardCount = pREWARD->m_bCount;
	m_bTakeMethod = pREWARD->m_bTakeMethod;
	m_bTakeData = pREWARD->m_bTakeData;
	m_dwQuestMob = pREWARD->m_dwQuestMob;
	m_dwQuestTime = pREWARD->m_dwQuestTime;
	m_dwQuestPathMob = pREWARD->m_dwQuestPathMob;
	m_dwTicketID = pREWARD->m_dwTicketID;
	m_bSendQ = pREWARD->m_bSendQ;

	CFormView::UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CTQuestView::UpdateTerm( LPTQUESTTERM pTERM)
{
	m_strObjective = pTERM->m_strObjective;
	m_strTermMsg = pTERM->m_strMessage;

	m_bTermType = pTERM->m_bTermType;
	m_dwTermID = pTERM->m_dwTermID;
	m_bTermCount = pTERM->m_bCount;

	CFormView::UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CTQuestView::OnKillfocusEditConcount() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cCondition.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTCONDITION pCONDITION = (LPTCONDITION) m_cCondition.GetItemData(nIndex);

			if( pCONDITION && pCONDITION->m_bCount != m_bConCount )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTConditionSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pCONDITION->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bCount = m_bConCount;
					set.Update();
					set.Close();

					pCONDITION->m_bCount = m_bConCount;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditConid() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cCondition.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTCONDITION pCONDITION = (LPTCONDITION) m_cCondition.GetItemData(nIndex);

			if( pCONDITION && pCONDITION->m_dwConditionID != m_dwConID )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTConditionSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pCONDITION->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwConditionID = m_dwConID;
					set.Update();
					set.Close();

					pCONDITION->m_dwConditionID = m_dwConID;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditRewardcount() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_bCount != m_bRewardCount )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bCount = m_bRewardCount;
					set.Update();
					set.Close();

					pREWARD->m_bCount = m_bRewardCount;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditRewardid() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_dwRewardID != m_dwRewardID )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwRewardID = m_dwRewardID;
					set.Update();
					set.Close();

					pREWARD->m_dwRewardID = m_dwRewardID;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditTakedata() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_bTakeData != m_bTakeData )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bTakeData = m_bTakeData;
					set.Update();
					set.Close();

					pREWARD->m_bTakeData = m_bTakeData;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditIncompmsg() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if( m_strTermMsg.GetLength() > 1023 )
				m_strTermMsg = m_strTermMsg.Left(1023);

			if( pTERM && pTERM->m_strMessage != m_strTermMsg )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermTextSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwTermID = %d", pTERM->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_szMessage = m_strTermMsg;
					set.Update();
					set.Close();

					pTERM->m_strMessage = m_strTermMsg;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditObjective() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if( m_strObjective.GetLength() > 1023 )
				m_strObjective = m_strObjective.Left(1023);

			if( pTERM && pTERM->m_strObjective != m_strObjective )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermTextSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwTermID = %d", pTERM->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_szObjective = m_strObjective;
					set.Update();
					set.Close();

					pTERM->m_strObjective = m_strObjective;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditCount() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if( pTERM && pTERM->m_bCount != m_bTermCount )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pTERM->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bCount = m_bTermCount;
					set.Update();
					set.Close();

					pTERM->m_bCount = m_bTermCount;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditTermid() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if( pTERM && pTERM->m_dwTermID != m_dwTermID )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pTERM->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwTermID = m_dwTermID;
					set.Update();
					set.Close();

					pTERM->m_dwTermID = m_dwTermID;
				}
			}
		}
	}
}

void CTQuestView::OnSelchangeEditType() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bType != m_bType )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bType = m_bType;
				set.Update();
				set.Close();

				m_pQUEST->m_bType = m_bType;
			}
		}
	}
}

void CTQuestView::OnSelchangeEditTriggertype()
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bTriggerType != m_bTriggerType )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bTriggerType = m_bTriggerType;
				set.Update();
				set.Close();

				m_pQUEST->m_bTriggerType = m_bTriggerType;
			}
		}
	}
}

void CTQuestView::OnCbnSelchangeClassList()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTQClass.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQCLASS pTQCLASS = (LPTQCLASS) m_cTQClass.GetItemData(nIndex);

			if(pTQCLASS)
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTitleSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
				if(set.Open())
				{
					if(!set.IsEOF())
					{
						set.MoveFirst();
						set.Edit();
						set.m_dwClassID = pTQCLASS->m_dwClassID;
						set.Update();

						m_pQUEST->m_dwClassID = pTQCLASS->m_dwClassID;
						m_pQUEST->m_bClassMain = pTQCLASS->m_bClassMain;
						m_dwClassID = pTQCLASS->m_dwClassID;

						CFormView::UpdateData(FALSE);
					}

					set.Close();
				}
			}

			UpdateDataClassTitleDlg();
		}
	}
	if( m_pQUEST && m_pQUEST->m_dwClassID )
	{
		m_bClassMain = GetDocument()->GetClassMain(m_dwClassID);
		CFormView::UpdateData(FALSE);
	}
	m_cMainCategory.EnableWindow( m_pQUEST && !m_bEditCtrlEnable && m_dwClassID);	
}

void CTQuestView::OnEnChangeClassId()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nItemCnt = m_cTQClass.GetCount();
		CTQuestEditorDoc *pDoc = GetDocument();

		for(int i=0; i<nItemCnt ;i++)
		{
			LPTQCLASS pTQCLASS = (LPTQCLASS) m_cTQClass.GetItemData(i);
			if(pTQCLASS && pTQCLASS->m_dwClassID == m_dwClassID)
			{
				CTTitleSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID );
				if(set.Open())
				{
					if(!set.IsEOF())
					{
						set.MoveFirst();
						set.Edit();
						set.m_dwClassID = pTQCLASS->m_dwClassID;
						set.Update();

						m_pQUEST->m_dwClassID = pTQCLASS->m_dwClassID;
						m_pQUEST->m_bClassMain = pTQCLASS->m_bClassMain;

						CFormView::UpdateData(FALSE);
					}
					set.Close();
				}
				
				m_cTQClass.SetCurSel(i);
				break;
			}
		}

		if(i==nItemCnt)
			m_cTQClass.SetCurSel(0);
		
		UpdateDataClassTitleDlg();
	}
}

void CTQuestView::OnSelchangeEditContype() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cCondition.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTCONDITION pCONDITION = (LPTCONDITION) m_cCondition.GetItemData(nIndex);

			if( pCONDITION && pCONDITION->m_bConditionType != m_bConType )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTConditionSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pCONDITION->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bConditionType = m_bConType;
					set.Update();
					set.Close();

					pCONDITION->m_bConditionType = m_bConType;
				}
			}
		}
	}
}

void CTQuestView::OnSelchangeEditTermtype() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cTerm.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTTERM pTERM = (LPTQUESTTERM) m_cTerm.GetItemData(nIndex);

			if( pTERM && pTERM->m_bTermType != m_bTermType )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTTermSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pTERM->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bTermType = m_bTermType;
					set.Update();
					set.Close();

					pTERM->m_bTermType = m_bTermType;
				}
			}
		}
	}
}

void CTQuestView::OnSelchangeEditRewardtype() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_bRewardType != m_bRewardType )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bRewardType = m_bRewardType;
					set.Update();
					set.Close();

					pREWARD->m_bRewardType = m_bRewardType;
				}
			}
		}
	}
}

void CTQuestView::OnSelchangeEditTakemethod() 
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_bTakeMethod != m_bTakeMethod )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bTakeMethod = m_bTakeMethod;
					set.Update();
					set.Close();

					pREWARD->m_bTakeMethod = m_bTakeMethod;
				}
			}
		}
	}
}

void CTQuestView::OnKillfocusEditSummarymsg() 
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strSummaryMsg.GetLength() > 1023 )
			m_strSummaryMsg = m_strSummaryMsg.Left(1023);

		if( m_pQUEST->m_strSummary != m_strSummaryMsg )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szSummary = m_strSummaryMsg;
				set.Update();
				set.Close();

				m_pQUEST->m_strSummary = m_strSummaryMsg;
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditNpcName()
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strNPCName.GetLength() > 1023 )
			m_strNPCName = m_strNPCName.Left(1023);

		if( m_pQUEST->m_strNPCName != m_strNPCName )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szNPCName = m_strNPCName;
				set.Update();
				set.Close();

				m_pQUEST->m_strNPCName = m_strNPCName;
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditPlayerAnswer()
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_strReply.GetLength() > 1023 )
			m_strReply = m_strReply.Left(1023);

		if( m_pQUEST->m_strReply != m_strReply )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTTitleSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_szReply = m_strReply;
				set.Update();
				set.Close();

				m_pQUEST->m_strReply = m_strReply;
			}
		}
	}
}

void CTQuestView::OnBnClickedCheckEditControlEnable()
{
	CFormView::UpdateData(TRUE);
	EnableAllCtrl(TRUE);
}

void CTQuestView::OnLock()
{
	m_bEditCtrlEnable = !m_bEditCtrlEnable;
	CFormView::UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CTQuestView::OnFind()
{
	GetDocument()->OnQuestFind();
}

void CTQuestView::OnBnClickedCheckMainCategory()
{
	if(m_pQUEST)
	{
		m_bClassMain = !m_bClassMain;
		CTQuestEditorDoc *pDoc = GetDocument();
		pDoc->SetClassMain(m_bClassMain);
		
		CFormView::UpdateData(TRUE);
	}
}

void CTQuestView::OnBnClickedCheckMainQuest()
{
	OnCbnSelchangeComboMainclass();
}

void CTQuestView::OnShowClasstitledlg()
{
	if(m_pQUEST)
	{
		CTQuestEditorDoc *pDoc = GetDocument();

		if(pDoc->m_pClassTitleDlg == NULL)
		{
			pDoc->m_pClassTitleDlg = new CClassTitleDlg();
			BOOL ret = pDoc->m_pClassTitleDlg->Create(IDD_DIALOG_CLASS_TITLE, this);
		
			if(!ret)  //Create failed.
				AfxMessageBox("Error creating Dialog");
			pDoc->m_pClassTitleDlg->ShowWindow(SW_SHOW);

			UpdateDataClassTitleDlg();
		}
		else
		{
			pDoc->m_pClassTitleDlg->ShowWindow(SW_HIDE);
			pDoc->m_pClassTitleDlg->DestroyWindow();
			delete pDoc->m_pClassTitleDlg;
			pDoc->m_pClassTitleDlg = NULL;
		}	
	}
}
void CTQuestView::UpdateDataClassTitleDlg(void)
{
	
	CTQuestEditorDoc *pDoc = GetDocument();
	
	if(pDoc->m_pClassTitleDlg != NULL)
	{
		int nIndex = m_cTQClass.GetCurSel();
		LPTQCLASS pQUEST = (LPTQCLASS) m_cTQClass.GetItemData(nIndex);
		
		pDoc->m_pClassTitleDlg->m_strClassTitle = pQUEST->m_strNAME;
		pDoc->m_pClassTitleDlg->m_strClassTitle.Format("%s",pQUEST->m_strNAME);
		pDoc->m_pClassTitleDlg->UpdateData(FALSE);
	}
}

void CTQuestView::OnCbnSelchangeComboMainclass()
{
	if(m_pQUEST)
	{
		m_bMain = m_ctrlMainQuest.GetCurSel();
		CTQuestEditorDoc *pDoc = GetDocument();
		CTQuestSet set(&pDoc->m_db);

		set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
		if(set.Open())
		{
			set.Edit();
			set.m_bMain = m_bMain;
			set.Update();
			set.Close();
			m_pQUEST->m_bMain = m_bMain;
		}
		CFormView::UpdateData(TRUE);
	}
}

void CTQuestView::OnBnClickedCheckConcheck()
{
	if(m_pQUEST)
	{
		CFormView::UpdateData();

		if( m_pQUEST->m_bConditionCheck != m_bConditionCheck )
		{
			CTQuestEditorDoc *pDoc = GetDocument();
			CTQuestSet set(&pDoc->m_db);

			set.m_strFilter.Format( "dwQuestID = %d", m_pQUEST->m_dwQuestID);
			if(set.Open())
			{
				set.Edit();
				set.m_bConditionCheck = m_bConditionCheck;
				set.Update();
				set.Close();

				m_pQUEST->m_bConditionCheck = m_bConditionCheck;
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditQuestmob()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_dwQuestMob != m_dwQuestMob )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwQuestMob = m_dwQuestMob;
					set.Update();
					set.Close();

					pREWARD->m_dwQuestMob = m_dwQuestMob;
				}
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditQuesttime()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_dwQuestTime != m_dwQuestTime )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwQuestTime = m_dwQuestTime;
					set.Update();
					set.Close();

					pREWARD->m_dwQuestTime = m_dwQuestTime;
				}
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditQuestpathmob()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_dwQuestPathMob != m_dwQuestPathMob )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwQuestPathMob = m_dwQuestPathMob;
					set.Update();
					set.Close();

					pREWARD->m_dwQuestPathMob = m_dwQuestPathMob;
				}
			}
		}
	}
}

void CTQuestView::OnEnKillfocusEditTicketid()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_dwTicketID != m_dwTicketID )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_dwTicketID = m_dwTicketID;
					set.Update();
					set.Close();

					pREWARD->m_dwTicketID = m_dwTicketID;
				}
			}
		}
	}
}

void CTQuestView::OnBnClickedCheckSendq()
{
	CFormView::UpdateData();

	if(m_pQUEST)
	{
		int nIndex = m_cReward.GetCurSel();

		if( nIndex >= 0 )
		{
			LPTQUESTREWARD pREWARD = (LPTQUESTREWARD) m_cReward.GetItemData(nIndex);

			if( pREWARD && pREWARD->m_bSendQ != (BYTE)m_bSendQ )
			{
				CTQuestEditorDoc *pDoc = GetDocument();
				CTRewardSet set(&pDoc->m_db);

				set.m_strFilter.Format( "dwID = %d", pREWARD->m_dwID);
				if(set.Open())
				{
					set.Edit();
					set.m_bSendQ = (BYTE)m_bSendQ;
					set.Update();
					set.Close();

					pREWARD->m_bSendQ = (BYTE)m_bSendQ;
				}
			}
		}
	}
}