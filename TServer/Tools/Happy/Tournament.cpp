// Tournament.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "Tournament.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CTournament 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTournament, CDialog)
CTournament::CTournament(CWnd* pParent /*=NULL*/)
	: CDialog(CTournament::IDD, pParent)
{
	m_pTNT = NULL;
	m_bNewTNT = FALSE;
	m_bSelectWorld = NULL;
	
	m_nFocusTList	= -1;
	m_nFocusTEvent	= -1;
}

CTournament::~CTournament()
{	
	if(m_bNewTNT)
	{
		delete m_pTNT;
		m_pTNT = NULL;
	}
}

void CTournament::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_TWEEK, m_cbWeek);
	DDX_Control(pDX, IDC_CB_TDAY, m_cbDay);
	DDX_Control(pDX, IDC_LC_TEVENT, m_lcTEvent);
	DDX_Control(pDX, IDC_LC_TSTEP, m_lcStep);
	DDX_Control(pDX, IDC_CB_WORLDTO, m_cbWorld);
	DDX_Control(pDX, IDC_LC_CHARINFO, m_lcCharInfo);
	DDX_Control(pDX, IDC_LC_REWARD, m_lcReward);
	DDX_Control(pDX, IDC_LC_TLIST, m_lcTList);
}


BEGIN_MESSAGE_MAP(CTournament, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDTEVENT, OnBnClickedBtnAddEvent)
	ON_BN_CLICKED(IDC_BTN_DELTEVENT, OnBnClickedBtnDelEvent)
	ON_BN_CLICKED(IDC_BTN_ADDENTRY, OnBnClickedBtnAddEntry)
	ON_BN_CLICKED(IDC_BTN_DELENTRY, OnBnClickedBtnDelEntry)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_NEWT, OnBnClickedBtnNewTournament)	
	ON_BN_CLICKED(IDC_BTN_DELT, OnBnClickedBtnDelt)
	ON_BN_CLICKED(IDC_BTN_ADDREWARD, OnBnClickedBtnAddreward)
	ON_BN_CLICKED(IDC_BTN_DELREWARD, OnBnClickedBtnDelreward)		
	ON_BN_CLICKED(IDC_BTN_UPDATETEVENT, OnBnClickedBtnUpdateTEvent)
	ON_BN_CLICKED(IDC_BTN_UPDATESCHEDULE, OnBnClickedBtnUpdateTSchedule)
	ON_BN_CLICKED(IDC_BTN_ENDCHAR, OnBnClickedBtnEndChar)	
	ON_BN_CLICKED(IDC_BTN_TREGISTER, OnBnClickedBtnTregister)		
	ON_BN_CLICKED(IDC_BTN_TUPDATEDATE, OnBnClickedBtnTupdatedate)
	ON_CBN_SELCHANGE(IDC_CB_WORLDTO, OnCbnSelchangeCbTList)
	ON_NOTIFY(NM_RCLICK, IDC_LC_TEVENT, OnNMRclickLcTevent)	
	ON_NOTIFY(NM_CLICK, IDC_LC_TEVENT, OnNMClickLcTevent)
	ON_NOTIFY(NM_DBLCLK, IDC_LC_TEVENT, OnNMDblclkLcTevent)
	ON_NOTIFY(NM_CLICK, IDC_LC_TLIST, OnNMClickLcTlist)

	ON_REGISTERED_MESSAGE(WM_XCTRL_SELECT, OnXCtrlItem)
END_MESSAGE_MAP()


// CTournament 메시지 처리기입니다.

//	기본 초기화
void CTournament::Init()
{
	//	다이얼로그 값 초기화
	m_cbWeek.SetCurSel(0);
	m_cbDay.SetCurSel(0);
	SetDlgItemInt(IDC_EB_THOUR,0);
	SetDlgItemInt(IDC_EB_TMINUTE,0);
	SetDlgItemText(IDC_EB_TITLE,_T(""));

	ClearCtrl();
	InitCharInfo();

	//	토너먼트 데이터 확인
	if(m_bNewTNT)
		delete m_pTNT;	
	m_pTNT = NULL;

	//	다이얼로그 컨트롤 Enable 설정
	SetEnableCtrl(TRUE);
	
	GetDlgItem(IDC_BTN_UPDATETEVENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_UPDATESCHEDULE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TUPDATEDATE)->EnableWindow(TRUE);
}

//	토너먼트 데이터 있을시
void CTournament::Init(LPTOURNAMENT pTNT)
{
	if(!pTNT)
		return;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	ClearCtrl();
	Init();
	ResetStepList(TRUE);

	m_pTNT = pTNT;

    SetDlgItemText(IDC_EB_TITLE,pTNT->m_strTitle);
	m_cbWeek.SetCurSel(pTNT->m_stTSTEP.m_bWeek-1);
	m_cbDay.SetCurSel(pTNT->m_stTSTEP.m_bDay-1);
	int nHour = pTNT->m_stTSTEP.m_dwStartTime / 3600;
	int nMinute = (pTNT->m_stTSTEP.m_dwStartTime % 3600) / 60 ;
	SetDlgItemInt(IDC_EB_THOUR,nHour);
	SetDlgItemInt(IDC_EB_TMINUTE,nMinute);

	int nCount = (int)pTNT->m_vTPVP.size();
	int i;
	
	for( i = 0; i < nCount; i++)
	{		
		TPVP stTPVP = pTNT->m_vTPVP[i];
		AddEvent(&stTPVP);
	}

	CString strTime =_T("");
	for(i = 0; i < MAX_TOURNAMENTSTEP - 1; i++)
	{
		m_lcStep.SetItemInt(i,2,pTNT->m_stTSTEP.m_arStep[i+1] / 60);
		__int64 dStart = pTNT->m_stTSTEP.m_arStart[i+1];
		CTime t(dStart);
		strTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());

		m_lcStep.SetItemText(i,3,strTime);
	}

	if(m_pTNT->m_wID == 1)
		SetEnableCtrl(FALSE);
	else		
		SetEnableCtrl(TRUE);

	GetDlgItem(IDC_BTN_TREGISTER)->EnableWindow(FALSE);	
}

void CTournament::AddEvent(LPTPVP pTPVP)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	AddEventList();
	int nCol = 1;
	int nRow = m_lcTEvent.GetItemCount();
	nRow--;
	
	m_lcTEvent.SetItemText(nRow,nCol++,pTPVP->m_strName);

	int nType = pTPVP->m_bType - 1;
	if(nType < 0)
		AfxMessageBox(_T("Invalid Type"));
	else
        m_lcTEvent.SetComboCurSel(nRow,nCol++,nType);

	int nClass = pDoc->GetClassIndex(pTPVP->m_dwClass);
	if(nClass < 0 )
		AfxMessageBox(_T("Invalid Class"));
	else
        m_lcTEvent.SetComboCurSel(nRow,nCol++,nClass);

	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_dwFee);
	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_dwRefund);
	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_wNeedItem);
	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_bNeedCount);
	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_bMinLevel);
	m_lcTEvent.SetItemInt(nRow,nCol++,pTPVP->m_bMaxLevel);

	for(BYTE i = 0; i < (BYTE)pTPVP->m_vTChar.size(); i++)
		m_lcTEvent.AddStringCombo(nRow,nCol,pTPVP->m_vTChar[i].m_strName);

	m_lcTEvent.SetComboCurSel(nRow,nCol,0);
	
}

void CTournament::InitTitle()
{
	SetDlgItemText(IDC_EB_TITLE,_T(""));

	int nCount = m_cbWeek.GetCount();
	while(nCount > 0)
	{
		m_cbWeek.DeleteString(0);
		nCount--;
	}

	m_cbWeek.AddString(_T("first"));
	m_cbWeek.AddString(_T("second"));
	m_cbWeek.AddString(_T("third"));
	m_cbWeek.AddString(_T("fourth"));
	m_cbWeek.AddString(_T("fifth"));
	m_cbWeek.SetCurSel(0);

	nCount = m_cbDay.GetCount();
	while(nCount > 0 )
	{
		m_cbDay.DeleteString(0);
		nCount--;
	}
	m_cbDay.AddString(_T("SUN"));
	m_cbDay.AddString(_T("MON"));
	m_cbDay.AddString(_T("TUE"));
	m_cbDay.AddString(_T("WED"));
	m_cbDay.AddString(_T("THR"));
	m_cbDay.AddString(_T("FRI"));
	m_cbDay.AddString(_T("SAT"));	
	m_cbDay.SetCurSel(0);

	SetDlgItemInt(IDC_EB_THOUR,0);
	SetDlgItemInt(IDC_EB_TMINUTE,0);
}

void CTournament::InitWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	ClearTList();

	while(m_cbWorld.GetCount())
		m_cbWorld.DeleteString(0);

	m_bSelectWorld = NULL;

	m_cbWorld.AddString(_T("(Selected World Server)"));
	m_cbWorld.SetCurSel(0);

	//	월드서버 리스트 구하기
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
	{	
		if( (*itG).second->m_bID != 0 )
			m_cbWorld.AddString((*itG).second->m_strName);
	}

	Init();
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_LIST, NULL);
}

void CTournament::InitEventList()
{
	for(int i = 0; i < MAX_TOURNAMENT_COLUMN ; i++)
        m_lcTEvent.DeleteColumn(0);
	
	const TCHAR *lpszHeader[] = {_T("No."),_T("Event"), _T("Type"), _T("Class"), _T("Fee [Cron]"),_T("Refund"),_T("Need Item"),_T("Count"),_T("MinL"),_T("MaxL"),_T("Entry Char"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	for( int i=0; ; i++ )
	{
		if( lpszHeader[i] == NULL )
			break;

		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (TCHAR *)lpszHeader[i];
		lvcolumn.iSubItem = i;
		m_lcTEvent.InsertColumn(i, &lvcolumn);
	}		

	m_lcTEvent.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);
	

	// 리스트 아이템 높이 설정
	m_lcTEvent.SetRowHeight(20);

	// 언더라인 설정
	m_lcTEvent.SetUnderLine(TRUE);
	m_lcTEvent.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	m_lcTEvent.m_HeaderCtrl.SetHeight(25);

	int arColWidthList[] = {50,140,60,80,80,80,80,55,50,50,130};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcTEvent.SetColumnWidth(i, arColWidthList[i]);
}

void CTournament::InitStepList()
{
	// 리스트 컨트롤의 모든 컬럼 삭제
	for(int i = 0; i < MAX_TOURNAMENTSTEP_COLUMN ; i++)
        m_lcStep.DeleteColumn(0);

	const TCHAR *lpszHeader[] = {_T("No."), _T("Step"), _T("Minute"),_T("Start Time"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	for( int i=0; ; i++ )
	{
		if( lpszHeader[i] == NULL )
			break;

		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (TCHAR *)lpszHeader[i];
		lvcolumn.iSubItem = i;
		m_lcStep.InsertColumn(i, &lvcolumn);
	}	

	m_lcStep.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);
	
	
	// 리스트 아이템 높이 설정
	m_lcStep.SetRowHeight(20);

	// 언더라인 설정
	m_lcStep.SetUnderLine(TRUE);
	m_lcStep.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	m_lcStep.m_HeaderCtrl.SetHeight(25);

	int arColWidthList[] = {50, 200, 70,120};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcStep.SetColumnWidth(i, arColWidthList[i]);
}

void CTournament::ResetStepList(BYTE bNewList)
{
	if(bNewList)
	{
		CString strNo=_T("");
		COLORREF crTextEdit = RGB(10,10,10);
		COLORREF crText = RGB(242,149,48);
		COLORREF crBgColor = RGB(255,255,255);	

		int nRow = 0;
		int nItemCol = 1;
		int nEditCol = 2;
		int nTimeCol = 3;

		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);	
		m_lcStep.SetItemText(nRow,nItemCol,_T("1st Class Recruitment"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);
		
		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("General Recruitment"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);		
		
		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Party Organization"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Match List"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Admission"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Quarterfinal"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("End of Quarterfinal"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Semifinal Admission"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Semifinal"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("End of Semifinal"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Final Admission"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("Final"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);

		nRow++;
		strNo.Format("%d",nRow+1);
		m_lcStep.InsertItem(nRow, strNo);
		m_lcStep.SetItemText(nRow,nItemCol,_T("End"),crText,crBgColor);
		m_lcStep.SetItemText(nRow,nEditCol,_T("1"),crTextEdit,crBgColor);
		m_lcStep.SetItemText(nRow,nTimeCol,_T("0"),crTextEdit,crBgColor);
		m_lcStep.SetEditBox(nRow,nEditCol,TRUE);
		m_lcStep.SetEditBoxAlign(nRow,nEditCol,ES_CENTER);
	}

	UpdateData();
}


void CTournament::OnBnClickedBtnAddEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if(!m_pTNT)
	{	
		return;
	}
	

	int nRow = AddEventList(RGB(242,149,48));
	if(nRow < 0)
		return;

	TPVP stTPVP;
	stTPVP.Reset();
	stTPVP.m_bEvent = nRow+1;
	m_pTNT->m_vTPVP.push_back(stTPVP);

}

int CTournament::AddEventList(COLORREF crText)
{
	if(!m_pTNT)
		return -1;

	int nRow = m_lcTEvent.GetItemCount();
	if(nRow > 7)
		return - 1;
	
	int nCol = 0;

	COLORREF crBgColor = RGB(255,255,255);

	CString str = _T("");
	str.Format("%d",nRow+1);

	CStringArray sa;
	sa.RemoveAll();

	// No.
	m_lcTEvent.InsertItem(nRow,str);
	m_lcTEvent.SetItemText(nRow,nCol,str, crText, crBgColor);

	// Event 
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow, nCol, _T("Event"), crText, crBgColor);
		

	// Type
	nCol++;	
	sa.RemoveAll();
	sa.Add(_T("Party"));
	sa.Add(_T("Solo"));		
	m_lcTEvent.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcTEvent.SetItemText(nRow,nCol,_T("Party"), crText, crBgColor);
	

	// Target
	nCol++;	
	sa.RemoveAll();
	sa.Add(_T("WARRIOR"));
	sa.Add(_T("ASSASSIN"));
	sa.Add(_T("ARCHER"));
	sa.Add(_T("WIZARD"));
	sa.Add(_T("PRIEST"));
	sa.Add(_T("SUMMONER"));
	sa.Add(_T("ALL"));	
	m_lcTEvent.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcTEvent.SetItemText(nRow,nCol,_T("WARRIOR"), crText, crBgColor);
	
	// Fee
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);
	
	// Refund
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);

	// Need Item
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);

	// Need Count
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);

	// Min Level
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);

	// Max Level
	nCol++;	
	m_lcTEvent.SetEditBox(nRow,nCol,TRUE);
	m_lcTEvent.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcTEvent.SetItemText(nRow,nCol,_T("0"), crText, crBgColor);

	// Entry Char
	nCol++;	
	sa.RemoveAll();		
	m_lcTEvent.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcTEvent.SetItemText(nRow,nCol,_T(""), crText, crBgColor);	

	return nRow;
}

void CTournament::OnBnClickedBtnDelEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_pTNT)	
		return;
	
	int iEventID = ItemSelectTEvent();
	if(iEventID < 0)
		return;

	VTPVP::iterator it = m_pTNT->m_vTPVP.begin();
	m_pTNT->m_vTPVP.erase(it + iEventID);

	int nCount = m_lcTEvent.GetItemCount();
	if(!nCount)
		return;

	int nRow = m_lcTEvent.GetCurSel();
	if(nRow<0)
	{
		AfxMessageBox(_T("You must select row"));
		return;
	}

	m_lcTEvent.DeleteItem(nRow);
	m_lcReward.DeleteAllItems();
}



void CTournament::OnBnClickedBtnAddEntry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT)
		return;

	int nCount = m_lcTEvent.GetItemCount();
	if(!nCount)
		return;

	int nRow = m_lcTEvent.GetCurSel();
	if(nRow < 0)
	{
		AfxMessageBox(_T("You must select row"));
		return;
	}

	int nEventID = ItemSelectTEvent();
	if(nEventID == -1 )
		return;

	CString str=_T("");
	GetDlgItemText(IDC_EB_ENTRYNAME,str);

	if(str.IsEmpty())
		return;

	CString str2 = _T("");
	BYTE bExist = FALSE;
	int nCol = 8;
	int nIndexCount = m_lcTEvent.GetComboItemCount(nRow,nCol);
	for(int i = 0; i < nIndexCount; i++)
	{
		str2 = m_lcTEvent.GetComboText(nRow,nCol,i);
		if(str2 == str)
		{
			bExist = TRUE;
			break;
		}
		
	}
	if(bExist)
		return;

	TNTCHAR stTNTCHAR;
	stTNTCHAR.m_bEventID = nEventID + 1;
	stTNTCHAR.m_strName = str;

	if( (BYTE)m_pTNT->m_vTPVP[nEventID].m_vTChar.size() > 7 )
	{
		AfxMessageBox(_T("The number of player must be less than 8"));
		return;
	}

	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_PLAYERADD,m_pTNT,&stTNTCHAR);	
}

void CTournament::OnBnClickedBtnDelEntry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	int nCount = m_lcTEvent.GetItemCount();
	if(!nCount)
		return;

	int nRow = m_lcTEvent.GetCurSel();
	if(nRow<0)
	{
		AfxMessageBox(_T("You must select row"));
		return;
	}
	int nEventID = ItemSelectTEvent();
	
	CString str=_T("");	
	GetDlgItemText(IDC_EB_ENTRYNAME,str);
	if( str.IsEmpty())
		return;
	
	TNTCHAR stTNTCHAR;
	stTNTCHAR.m_bEventID = nEventID + 1;
	stTNTCHAR.m_strName = str;	
		
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_PLAYERDEL,m_pTNT,&stTNTCHAR);
}


void CTournament::UpdateEntryChar( BYTE bAdd, LPTNTCHAR pTNTCHAR)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	int nRow = ItemSelectTEvent();
	int nEventType = m_lcTEvent.GetComboCurSel(nRow,1);
	
	int nCol = 8;
	if(!bAdd) // del
	{		
		m_lcTEvent.DelStringCombo(nRow,nCol,m_lcTEvent.GetComboCurSel(nRow,nCol));
		if(m_lcTEvent.GetComboItemCount(nRow,nCol) > 0)
            m_lcTEvent.SetComboCurSel(nRow,nCol,0);	
		else
			m_lcTEvent.SetItemText(nRow,nCol,_T(""));

		UpdateCharInfo(NULL);
	}
	else
	{
		m_lcTEvent.AddStringCombo(nRow,nCol,pTNTCHAR->m_strName);
		int nCurSel = 0;
		nCurSel = m_lcTEvent.GetComboItemCount(nRow,nCol);
		m_lcTEvent.SetComboCurSel(nRow,nCol,nCurSel-1);

		UpdateCharInfo(pTNTCHAR);		
	}
}



void CTournament::UpdateCharInfo( LPTNTCHAR pTNTCHAR )
{
	int nCol = 1;
	CString str = _T("");

	if(!pTNTCHAR)
	{	
		SetDlgItemText(IDC_EB_ENTRYNAME,_T(""));
		m_lcCharInfo.SetItemText(0,nCol,_T(""));
		m_lcCharInfo.SetItemText(1,nCol,_T(""));
		m_lcCharInfo.SetItemText(2,nCol,_T(""));
		m_lcCharInfo.SetItemText(3,nCol,_T(""));
	}
	else
	{
		SetDlgItemText(IDC_EB_ENTRYNAME,pTNTCHAR->m_strName);

		m_lcCharInfo.SetItemText(0,nCol,pTNTCHAR->m_strName);
		str.Format("%d",pTNTCHAR->m_bLevel);
		m_lcCharInfo.SetItemText(1,nCol,str);

		int nClass = pTNTCHAR->m_bClass;		
		switch(nClass)
		{
		case 0: str = _T("WARRIOR"); break;
		case 1: str = _T("ASSASSIN"); break;
		case 2: str = _T("ARCHER"); break;
		case 3: str = _T("WIZARD"); break;
		case 4: str = _T("PRIEST"); break;
		case 5: str = _T("SUMMONER"); break;
		}

		m_lcCharInfo.SetItemText(2,nCol,str);

		if(!pTNTCHAR->m_bCountry)
			str = _T("Defugel");
		else if(pTNTCHAR->m_bCountry == 1)
			str = _T("Craxion");
		else
			str = _T("Invalid");

		m_lcCharInfo.SetItemText(3,nCol,str);
	}
}

void CTournament::OnBnClickedBtnNewTournament()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_nFocusTList	= -2;
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_LIST, NULL);
}

void CTournament::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearCtrl();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->ClearTournament();

	if(m_bNewTNT)
		delete m_pTNT;
	m_pTNT = NULL;

	OnCancel();
}

void CTournament::ClearCtrl()
{
	m_lcTEvent.DeleteAllItems();
	m_lcStep.DeleteAllItems();
	m_lcCharInfo.DeleteAllItems();
	m_lcReward.DeleteAllItems();
}

void CTournament::ClearTList()
{
	m_lcTList.DeleteAllItems();
}

//	world 콤보박스 클릭
void CTournament::OnCbnSelchangeCbTList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	
	CString str = _T("");
	const CString str2 = _T("(Selected World Server)");

	m_cbWorld.GetLBText(m_cbWorld.GetCurSel(),str);

	ClearTList();
	if(str2 == str) 
	{
		pDoc->ClearTournament();
		ClearCtrl();
		Init();
		return;
	}

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;
	if( itG == pDoc->m_mapGroup.end() )
	{
		AfxMessageBox(_T("Not found server"));
		return;
	}
	
	m_bSelectWorld = (*itG).second->m_bID;
	m_nFocusTList = 0;
	
	ClearCtrl();
	SetEnableCtrl(FALSE);
	//	서버에서 토너먼트 리스트를 달라고 요청
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_LIST, NULL);
}

void CTournament::AddList( CString str, BYTE bFocus)
{
	if(bFocus)
		m_nFocusTList = m_lcTList.InsertItem(m_lcTList.GetItemCount(), str);
	else
		m_lcTList.InsertItem(m_lcTList.GetItemCount(), str);

	UpdateData();
}

void CTournament::DelList(CString str)
{
	ClearCtrl();

	int nCount = m_lcTList.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		if(m_lcTList.GetItemText(i, 0) == str)
		{
			m_lcTList.DeleteItem(i);
			break;
		}
	}

	Init();
	ResetStepList();

	UpdateData();
}

// Tournament delete
void CTournament::OnBnClickedBtnDelt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT)
		return;

	//	현재 토너먼트 삭제 요청
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_SCHEDULEDEL,m_pTNT);
}

void CTournament::OnBnClickedBtnAddreward()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEventID = AddRewardList(TRUE,RGB(242,149,48));
	if(iEventID < 0)
		return;
	
	TREWARD stREWARD;
	stREWARD.Reset();
	m_pTNT->m_vTPVP[iEventID].m_vTReward.push_back(stREWARD);
}

int CTournament::AddRewardList(BYTE bCheckEvent,COLORREF crText)
{
	if(!m_pTNT)
		return -1;

	int iEventID = ItemSelectTEvent();
	if(iEventID < 0 && bCheckEvent)
		return -1;

	int nRow = m_lcReward.GetItemCount();
	int nCol = 1;

	COLORREF crBgColor = RGB(255,255,255);

	CString strNo = _T("");
	strNo.Format("%d",nRow + 1);
	
	//Count
	m_lcReward.InsertItem(nRow,strNo);
	m_lcReward.SetItemText(nRow,0,strNo,crText,crBgColor);

	// Chart Type	
	CStringArray sa;
	sa.RemoveAll();
	sa.Add(_T("Magic"));
	sa.Add(_T("Normal"));
	m_lcReward.SetComboBox(nRow, nCol, TRUE, &sa );
	m_lcReward.SetItemText(nRow, nCol, _T("Magic"), crText, crBgColor);
	
	// ItemID
	nCol++;	
	m_lcReward.SetEditBox(nRow,nCol);
	m_lcReward.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcReward.SetItemText(nRow, nCol, _T("0"), crText, crBgColor);

	// Count
	nCol++;	
	m_lcReward.SetEditBox(nRow,nCol);
	m_lcReward.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcReward.SetItemText(nRow, nCol, _T("0"), crText, crBgColor);

	nCol++;	
	sa.RemoveAll();
	sa.Add(_T("WARRIOR"));
	sa.Add(_T("ASSASSIN"));
	sa.Add(_T("ARCHER"));
	sa.Add(_T("WIZARD"));
	sa.Add(_T("PRIEST"));
	sa.Add(_T("SUMMONER"));
	sa.Add(_T("ALL"));	
	m_lcReward.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcReward.SetItemText(nRow,nCol,_T("WARRIOR"), crText, crBgColor);

	nCol++;
	sa.RemoveAll();
	sa.Add("ALL");
	sa.Add("Defender");
	sa.Add("Berserker");
	m_lcReward.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcReward.SetItemText(nRow,nCol,_T("ALL"), crText,crBgColor);

	return iEventID;
}

void CTournament::OnBnClickedBtnDelreward()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_pTNT)
		return;

	int nCount = m_lcReward.GetItemCount();
	if(!nCount)
		return;

	int nRow = m_lcReward.GetCurSel();
	if(nRow<0)
	{
		AfxMessageBox(_T("You must select row"));
		return;
	}

	m_lcReward.DeleteItem(nRow);
	int iEventID = ItemSelectTEvent();
	if(iEventID < 0)
		return;

	VTREWARD::iterator it = m_pTNT->m_vTPVP[iEventID].m_vTReward.begin();
	m_pTNT->m_vTPVP[iEventID].m_vTReward.erase(it+nRow);
}

void CTournament::InitCharInfo()
{
	SetDlgItemText(IDC_EB_ENTRYNAME,_T(""));

	for(int i = 0; i < MAX_TCHARINFO_COLUMN ; i++)
        m_lcCharInfo.DeleteColumn(0);	

	m_lcCharInfo.DeleteAllItems();

	const TCHAR *lpszHeader[] = {_T("Item"), _T("Value"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	
	int nCount = 0;
	while(NULL != lpszHeader[nCount])
	{
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (TCHAR *)lpszHeader[nCount];
		lvcolumn.iSubItem = nCount;
		m_lcCharInfo.InsertColumn(nCount, &lvcolumn);
		nCount++;
	}

	m_lcCharInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);
	
	// 리스트 아이템 높이 설정
	m_lcCharInfo.SetRowHeight(20);

	// 언더라인 설정
	m_lcCharInfo.SetUnderLine(TRUE);
	m_lcCharInfo.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	m_lcCharInfo.m_HeaderCtrl.SetHeight(25);	

	int arColWidthList[] = {60, 120};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcCharInfo.SetColumnWidth(i, arColWidthList[i]);

	COLORREF crText = RGB(242,149,48);
	COLORREF crBgColor = RGB(255,255,255);	
	CString strNo=_T("");

	int nRow = 0;
	int nItemCol = 0;
	int nEditCol = 1;	
	m_lcCharInfo.InsertItem(nRow, _T("Name"));
	m_lcCharInfo.SetItemText(nRow,nItemCol,_T("Name"),crText,crBgColor);
	m_lcCharInfo.SetEditBox(nRow,nEditCol,FALSE);
	

	nRow++;	
	m_lcCharInfo.InsertItem(nRow, _T("Level"));
	m_lcCharInfo.SetItemText(nRow,nItemCol,_T("Level"),crText,crBgColor);
	m_lcCharInfo.SetEditBox(nRow,nEditCol,FALSE);
	
	
	nRow++;	
	m_lcCharInfo.InsertItem(nRow, _T("Class"));
	m_lcCharInfo.SetItemText(nRow,nItemCol,_T("Class"),crText,crBgColor);
	m_lcCharInfo.SetEditBox(nRow,nEditCol,FALSE);

	nRow++;	
	m_lcCharInfo.InsertItem(nRow, _T("Country"));
	m_lcCharInfo.SetItemText(nRow,nItemCol,_T("Country"),crText,crBgColor);
	m_lcCharInfo.SetEditBox(nRow,nEditCol,FALSE);
}

void CTournament::InitReward()
{
	for(int i = 0; i < MAX_TREWARD_COLUMN ; i++)
        m_lcReward.DeleteColumn(0);
	

	const TCHAR *lpszHeader[] = {_T("No."),_T("Chart Type"), _T("Item ID"), _T("Count"), _T("Class"),_T("Shield"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	for( int i=0; ; i++ )
	{
		if( lpszHeader[i] == NULL )
			break;

		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (TCHAR *)lpszHeader[i];
		lvcolumn.iSubItem = i;
		m_lcReward.InsertColumn(i, &lvcolumn);
	}	

	m_lcReward.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);
	
	// 리스트 아이템 높이 설정
	m_lcReward.SetRowHeight(20);

	// 언더라인 설정
	m_lcReward.SetUnderLine(TRUE);
	m_lcReward.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	m_lcReward.m_HeaderCtrl.SetHeight(25);

	int arColWidthList[] = {40, 90, 70, 60, 90, 60};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcReward.SetColumnWidth(i, arColWidthList[i]);
}

void CTournament::AddReward( LPTREWARD pREWARD,BYTE bIsEvent )
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	AddRewardList(bIsEvent);

	int nRow = m_lcReward.GetItemCount();
	nRow--;

	m_lcReward.SetComboCurSel(nRow,1,pREWARD->m_bChartType);
	m_lcReward.SetItemInt(nRow,2,pREWARD->m_wItemID);
	m_lcReward.SetItemInt(nRow,3,pREWARD->m_bCount);
	int nClass = pDoc->GetClassIndex(pREWARD->m_dwClass);
	if(nClass < 0 )
	{
		CString str =_T("");
		str.Format("Invalid Class : %d row %d col",nRow,3);
		AfxMessageBox(str);
		nClass = 0;
	}
	m_lcReward.SetComboCurSel(nRow,4,nClass);
	m_lcReward.SetComboCurSel(nRow,5,pREWARD->m_bShield);
}

void CTournament::OnBnClickedBtnUpdateTEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	
	if(!m_pTNT)
		return;

	for(BYTE i = 0;i < m_pTNT->m_vTPVP.size(); i++ )
	{
		if( m_pTNT->m_vTPVP[i].m_strName.IsEmpty())
		{
			AfxMessageBox(_T("You must enter the Event's Name"));
			return;
		}
	}

	m_bNewTNT = FALSE;
	
	COLORREF crText = RGB(10,10,10);
	COLORREF crBgColor = RGB(255,255,255);
	
	SetColorEventRow(-1,crText,crBgColor);
	SetColorRewardRow(-1,crText,crBgColor);
	
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_ENTRYADD,m_pTNT);
	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_LIST,NULL);
	m_wTournamentID = m_pTNT->m_wID;
}

void CTournament::OnBnClickedBtnUpdateTSchedule()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT)
	{
		m_pTNT = new TOURNAMENT();
		m_pTNT->m_wID = 0;
		m_bNewTNT = TRUE;
	}
	
	m_pTNT->m_stTSTEP.m_bWeek = m_cbWeek.GetCurSel() + 1;
	m_pTNT->m_stTSTEP.m_bDay = m_cbDay.GetCurSel() + 1;
	if( m_pTNT->m_stTSTEP.m_bWeek == 255 || m_pTNT->m_stTSTEP.m_bWeek == 0 
		|| m_pTNT->m_stTSTEP.m_bDay == 255 || m_pTNT->m_stTSTEP.m_bDay == 0)
	{
		AfxMessageBox(_T("Invalid Date"));
		if(!m_pTNT->m_wID)
		{
			delete m_pTNT;
			m_pTNT = NULL;
		}

		return;
	}
	int nHour = GetDlgItemInt(IDC_EB_THOUR);
	int nMinute = GetDlgItemInt(IDC_EB_TMINUTE);
	if(nHour > 23 || nMinute > 59)
	{
		AfxMessageBox(_T("Invalid Time"));

		if(!m_pTNT->m_wID)
		{
			delete m_pTNT;
			m_pTNT = NULL;
		}

		return;
	}

	m_pTNT->m_stTSTEP.m_dwStartTime = nHour * 3600 + nMinute * 60;
		
	for(int nStep = 1; nStep < MAX_TOURNAMENTSTEP; nStep++)
	{
		int nTime = (atoi(m_lcStep.GetItemText(nStep-1,2))) * 60;
		if(nTime < 1 && nStep > 3)
		{
			AfxMessageBox(_T("Step time is Invalid"));

			if(!m_pTNT->m_wID)
			{
				delete m_pTNT;
				m_pTNT = NULL;
			}

			return;
		}
		m_pTNT->m_stTSTEP.m_arStep[nStep] = nTime;
	}

	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_SCHEDULEADD,m_pTNT);
}

void CTournament::OnBnClickedBtnEndChar()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT || !m_pTNT->m_wID)
		return;

	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_PLAYEREND,NULL);
}

int	CTournament::ItemSelectTList()
{
	// 선택된 아이템 찾기		
	int iCount = m_lcTList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcTList.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			return i;
		}
	}	
	return -1;
}

int CTournament::ItemSelectTEvent()
{
	// 선택된 아이템 찾기		
	int iCount = m_lcTEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcTEvent.GetItemState(i, LVIS_SELECTED) != 0 )
		{			
			return i;
		}
	}
	
	return -1;
}

void CTournament::UpdateEventList( int nRow, int nCol )
{			
	if(!m_pTNT)
		return;

	int nCol2 = 1;
	m_pTNT->m_vTPVP[nRow].m_strName = m_lcTEvent.GetItemText(nRow,nCol2++);	
	m_pTNT->m_vTPVP[nRow].m_bType = m_lcTEvent.GetComboCurSel(nRow,nCol2++) + 1;
	
	int nIndex = m_lcTEvent.GetComboCurSel(nRow,nCol2++);
	if(nIndex == 6)
		m_pTNT->m_vTPVP[nRow].m_dwClass = 63;
	else
        m_pTNT->m_vTPVP[nRow].m_dwClass = BITSHIFTID(nIndex);	
	
	m_pTNT->m_vTPVP[nRow].m_dwFee = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	m_pTNT->m_vTPVP[nRow].m_dwRefund = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	m_pTNT->m_vTPVP[nRow].m_wNeedItem = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	m_pTNT->m_vTPVP[nRow].m_bNeedCount = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	m_pTNT->m_vTPVP[nRow].m_bMinLevel = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	m_pTNT->m_vTPVP[nRow].m_bMaxLevel = atoi(m_lcTEvent.GetItemText(nRow,nCol2++));
	if(0 == m_pTNT->m_vTPVP[nRow].m_bMaxLevel)
		m_pTNT->m_vTPVP[nRow].m_bMaxLevel = 0xFF;

	if(nCol == 9)
	{
		CString str = m_lcTEvent.GetComboText(nRow,nCol,m_lcTEvent.GetComboCurSel(nRow,nCol));
		BYTE bCount = (BYTE)m_pTNT->m_vTPVP[nRow].m_vTChar.size();
		for(BYTE i = 0; i <  bCount ; i++)
		{
			TNTCHAR stTNTCHAR = m_pTNT->m_vTPVP[nRow].m_vTChar[i];
			if( str == stTNTCHAR.m_strName )
			{
				UpdateCharInfo(&stTNTCHAR);
				UpdateData();
				break;
			}
		}
	}
}

void CTournament::UpdateReward( int nRow,int nCol )
{	
	int nEventID = ItemSelectTEvent();
	
	if(!m_pTNT)
		return;

	m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_bChartType = m_lcReward.GetComboCurSel(nRow,1);
	m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_wItemID = m_lcReward.GetItemInt(nRow,2);
	m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_bCount = m_lcReward.GetItemInt(nRow,3);
	int nIndex = m_lcReward.GetComboCurSel(nRow,4);
	if(nIndex == 6)
        m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_dwClass = 63;
	else
		m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_dwClass = BITSHIFTID(nIndex);

	m_pTNT->m_vTPVP[nEventID].m_vTReward[nRow].m_bShield = m_lcReward.GetComboCurSel(nRow,5);
}

void CTournament::UpdateStep( int nRow,int nCol )
{
	if(!m_pTNT)
		return;

	m_pTNT->m_stTSTEP.m_bWeek = m_cbWeek.GetCurSel()+1;
	m_pTNT->m_stTSTEP.m_bDay = m_cbDay.GetCurSel()+1;
	m_pTNT->m_stTSTEP.m_dwStartTime = GetDlgItemInt(IDC_EB_THOUR) * 3600 + GetDlgItemInt(IDC_EB_TMINUTE) * 60;
	m_pTNT->m_stTSTEP.m_arStep[nRow+1] = (m_lcStep.GetItemInt(nRow,nCol)) * 60;
}

void CTournament::SendNewTNT()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(m_bNewTNT)
	{
		pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_ENTRYADD,m_pTNT);
		m_bNewTNT =  FALSE;
	}
}

void CTournament::SetTitle( CString str )
{
	SetDlgItemText(IDC_EB_TITLE,str);
	UpdateData();
}

void CTournament::ChangeList( CString strSource, CString strTarget )
{	
	int nCount = m_lcTList.GetItemCount();
	CString str;
	for(int i = 0; i < nCount; i++)
	{
		str = m_lcTList.GetItemText(i,0);
		if(str == strSource)
		{
			m_nFocusTList = i;
			break;
		}
	}
}

void CTournament::SetEnableCtrl( BYTE bEnable )
{
	m_lcTEvent.SetEnableSubCtrl(bEnable);
	m_lcReward.SetEnableSubCtrl(bEnable);
	m_lcStep.SetEnableSubCtrl(bEnable);
	m_cbWeek.EnableWindow(bEnable);
	m_cbDay.EnableWindow(bEnable);

	GetDlgItem(IDC_EB_THOUR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_TMINUTE)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_ADDTEVENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DELTEVENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_UPDATETEVENT)->EnableWindow(bEnable);
	
	GetDlgItem(IDC_BTN_ADDREWARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DELREWARD)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_ADDENTRY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DELENTRY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_ENDCHAR)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_UPDATESCHEDULE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TUPDATEDATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DELT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TREGISTER)->EnableWindow(bEnable);	
}

void CTournament::OnNMRclickLcTevent(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnNMClickLcTevent(pNMHDR, pResult);
}

void CTournament::OnNMDblclkLcTevent(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnNMClickLcTevent(pNMHDR, pResult);
}

void CTournament::OnNMClickLcTevent(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int iEventID = ItemSelectTEvent();
	if(iEventID < 0)
	{
		if(m_nFocusTEvent < 0 || m_nFocusTEvent == iEventID)
			return;
		iEventID = m_nFocusTEvent;
		m_lcTEvent.SetItemState(m_nFocusTEvent, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
		m_nFocusTEvent = iEventID;

	if( (BYTE)m_pTNT->m_vTPVP.size() < iEventID + 1 )
		return;


	m_lcReward.DeleteAllItems();
	int n = (int)m_pTNT->m_vTPVP[iEventID].m_vTReward.size();
	for(int i = 0; i < n; i++)
	{
		TREWARD stREWARD = m_pTNT->m_vTPVP[iEventID].m_vTReward[i];
		AddReward( &stREWARD,TRUE);			
	}
	UpdateCharInfo(NULL);
}

void CTournament::OnBnClickedBtnTregister()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT)
		return;

	BYTE bWeek = m_cbWeek.GetCurSel() + 1;
	BYTE bDay = m_cbDay.GetCurSel() + 1; 
	int nHour = GetDlgItemInt(IDC_EB_THOUR);
	int nMinute = GetDlgItemInt(IDC_EB_TMINUTE);

	CString str =_T("");
	str.Format("%d-%d-%d-%d",bWeek,bDay,nHour,nMinute);

	LPTOURNAMENT pT = pDoc->GetTournament(str);
	if(pT)
	{
		AfxMessageBox("The Tournament is exist");
		return;
	}
	
	m_bNewTNT = TRUE;
	LPTOURNAMENT pTNT = NULL;
	if(m_pTNT->m_wID)
	{		
		pTNT = new TOURNAMENT();
		pTNT->Copy(m_pTNT);
		pTNT->m_wID = 0;
		pTNT->m_stTSTEP.m_bWeek = bWeek;
		pTNT->m_stTSTEP.m_bDay = bDay;
		if( pTNT->m_stTSTEP.m_bWeek == 255 || pTNT->m_stTSTEP.m_bWeek == 0 
			|| pTNT->m_stTSTEP.m_bDay == 255 || pTNT->m_stTSTEP.m_bDay == 0)
		{
			AfxMessageBox(_T("Invalid Date"));
			return;
		}
		
		pTNT->m_stTSTEP.m_dwStartTime = nHour * 3600 + nMinute * 60;
		
		m_pTNT = pTNT;
	}
	else
	{
		pTNT = m_pTNT;
	}

	pTNT->m_stTSTEP.m_bWeek = bWeek;
	pTNT->m_stTSTEP.m_bDay = bDay;
	pTNT->m_stTSTEP.m_dwStartTime = nHour * 3600 + nMinute * 60;

	for(int nStep = 1; nStep < MAX_TOURNAMENTSTEP; nStep++)
	{
		int nTime = atoi(m_lcStep.GetItemText(nStep-1,2));
		if(nTime < 0)
		{
			AfxMessageBox(_T("Step time is Invalid"));
			return;
		}
		pTNT->m_stTSTEP.m_arStep[nStep] = nTime;
	}

	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_SCHEDULEADD,pTNT);	
}

void CTournament::EnableBtnEventList( BYTE bEnable )
{
	GetDlgItem(IDC_BTN_UPDATETEVENT)->EnableWindow(bEnable);
}

void CTournament::EnableBtnSTEP( BYTE bEnable )
{
	GetDlgItem(IDC_BTN_UPDATESCHEDULE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_TUPDATEDATE)->EnableWindow(bEnable);
}
void CTournament::OnBnClickedBtnTupdatedate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_pTNT)
	{
		m_pTNT = new TOURNAMENT();
		m_pTNT->m_wID = 0;
		m_bNewTNT = TRUE;
	}
	
	m_pTNT->m_stTSTEP.m_bWeek = m_cbWeek.GetCurSel() + 1;
	m_pTNT->m_stTSTEP.m_bDay = m_cbDay.GetCurSel() + 1;
	if( m_pTNT->m_stTSTEP.m_bWeek == 255 || m_pTNT->m_stTSTEP.m_bWeek == 0 
		|| m_pTNT->m_stTSTEP.m_bDay == 255 || m_pTNT->m_stTSTEP.m_bDay == 0)
	{
		AfxMessageBox(_T("Invalid Date"));
		return;
	}
	int nHour = GetDlgItemInt(IDC_EB_THOUR);
	int nMinute = GetDlgItemInt(IDC_EB_TMINUTE);
	if(nHour > 23 || nMinute > 59)
	{
		AfxMessageBox(_T("Invalid Time"));
		return;
	}

	m_pTNT->m_stTSTEP.m_dwStartTime = nHour * 3600 + nMinute * 60;
		
	for(int nStep = 1; nStep < MAX_TOURNAMENTSTEP; nStep++)
	{
		int nTime = atoi(m_lcStep.GetItemText(nStep-1,2));
		if(nTime < 0)
		{
			AfxMessageBox(_T("Step time is Invalid"));
			return;
		}
		m_pTNT->m_stTSTEP.m_arStep[nStep] = nTime;
	}

	pDoc->SendCT_TOURNAMENTEVENT_REQ(m_bSelectWorld, TET_SCHEDULEADD,m_pTNT);	
}

void CTournament::SetColorEventRow( int nRow, COLORREF crText,COLORREF crBgColor)
{
	int nCount = m_lcTEvent.GetItemCount();
	int nRowIndex = 0;
	CString str = _T("");

	if(nRow != -1)
	{
		for(int nCol = 0; nCol < MAX_TOURNAMENT_COLUMN; nCol++)
		{			
			m_lcTEvent.SetItemTextColor(nRow,nCol,crText,crBgColor);
		}		
	}
	else
	{
		for(int nItem = 0; nItem < nCount; nItem++)
		{
			for(int nCol = 0; nCol < MAX_TOURNAMENT_COLUMN; nCol++)
			{
				m_lcTEvent.SetItemTextColor(nItem,nCol,crText,crBgColor);
			}
		}
	}
}

void CTournament::SetColorRewardRow( int nRow, COLORREF crText,COLORREF crBgColor )
{
	int nCount = m_lcReward.GetItemCount();
	int nRowIndex = 0;
	CString str = _T("");

	if(nRow != -1)
	{
		for(int nCol = 0; nCol < MAX_TREWARD_COLUMN; nCol++)
		{			
			m_lcReward.SetItemTextColor(nRow,nCol,crText,crBgColor);
		}		
	}
	else
	{
		for(int nItem = 0; nItem < nCount; nItem++)
		{
			for(int nCol = 0; nCol < MAX_TREWARD_COLUMN; nCol++)
			{
				m_lcReward.SetItemTextColor(nItem,nCol,crText,crBgColor);
			}
		}
	}
}

void CTournament::AddEntryChar( LPTNTCHAR pTNTCHAR )
{
	if(!m_pTNT)
		return;
	
	VTNTCHAR::iterator it;
	for(it = m_pTNT->m_vTPVP[pTNTCHAR->m_bEventID - 1].m_vTChar.begin(); it != m_pTNT->m_vTPVP[pTNTCHAR->m_bEventID - 1].m_vTChar.end(); it++)
	{
		if( (*it).m_dwCharID == pTNTCHAR->m_dwCharID )
		{
			m_pTNT->m_vTPVP[pTNTCHAR->m_bEventID - 1].m_vTChar.erase(it);
			break;
		}
	}

	TNTCHAR stTNTCHAR;
	stTNTCHAR.m_bClass = pTNTCHAR->m_bClass;
	stTNTCHAR.m_bCountry = pTNTCHAR->m_bCountry;
	stTNTCHAR.m_bEventID = pTNTCHAR->m_bEventID;
	stTNTCHAR.m_bLevel = pTNTCHAR->m_bLevel;
	stTNTCHAR.m_dwCharID = pTNTCHAR->m_dwCharID;
	stTNTCHAR.m_strName = pTNTCHAR->m_strName;

	m_pTNT->m_vTPVP[pTNTCHAR->m_bEventID - 1].m_vTChar.push_back(stTNTCHAR);
}

void CTournament::OnNMClickLcTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	// 선택된 아이템 찾기	
	int iCount = ItemSelectTList();
	if(0 > iCount)
		return;

	m_nFocusTList = iCount;
	m_nFocusTEvent = -1;

	m_pTNT = pDoc->GetTournament(m_lcTList.GetItemText(iCount, 0));
	Init(m_pTNT);
	return;	
}

//	포커스 설정
void CTournament::UpdateFocus()
{	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	//	TList 포커스
	if(0 <= m_nFocusTList)
	{
		m_lcTList.SetItemState(m_nFocusTList, LVIS_SELECTED, LVIS_SELECTED);

		m_pTNT = pDoc->GetTournament(m_lcTList.GetItemText(m_nFocusTList, 0));
		if(!m_pTNT)
			return;

		Init(m_pTNT);

		//	TEvent 포커스
		if(0 <= m_nFocusTEvent)
		{
			m_lcTEvent.SetItemState(m_nFocusTEvent, LVIS_SELECTED, LVIS_SELECTED);

			m_lcReward.DeleteAllItems();
			if(m_pTNT->m_vTPVP.size() > (unsigned int)m_nFocusTEvent)
			{
				int n = (int)m_pTNT->m_vTPVP[m_nFocusTEvent].m_vTReward.size();
				for(int i = 0; i < n; i++)
				{
					TREWARD stREWARD = m_pTNT->m_vTPVP[m_nFocusTEvent].m_vTReward[i];
					AddReward( &stREWARD,TRUE);			
				}
			}
			UpdateCharInfo(NULL);
		}
	}
	else if(-2 == m_nFocusTList)
	{
		ResetStepList(TRUE);
		GetDlgItem(IDC_BTN_DELT)->EnableWindow(FALSE);	
		SetDlgItemText(IDC_EB_TITLE,_T("Click UPDATE button to add new category."));
	}
}


BOOL CTournament::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitTitle();
	InitEventList();
	InitCharInfo();
	InitReward();
	InitStepList();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CTournament::OnXCtrlItem(WPARAM wParam, LPARAM lParam)
{
	HWND pWnd = (HWND)lParam;

	if(pWnd == GetDlgItem(IDC_LC_TEVENT)->m_hWnd)
	{
		if(m_nFocusTEvent != (int)wParam)
		{
			m_nFocusTEvent = (int)wParam;
			m_lcTEvent.SetItemState(m_nFocusTEvent, LVIS_SELECTED, LVIS_SELECTED);

			if( (BYTE)m_pTNT->m_vTPVP.size() < m_nFocusTEvent + 1 )
				return 0;
			
			m_lcReward.DeleteAllItems();
			int n = (int)m_pTNT->m_vTPVP[m_nFocusTEvent].m_vTReward.size();
			for(int i = 0; i < n; i++)
			{
				TREWARD stREWARD = m_pTNT->m_vTPVP[m_nFocusTEvent].m_vTReward[i];
				AddReward( &stREWARD,TRUE);			
			}
			UpdateCharInfo(NULL);
		}
	}
	else if(pWnd == GetDlgItem(IDC_LC_REWARD)->m_hWnd)
	{
		m_lcReward.SetItemState((int)wParam, LVIS_SELECTED, LVIS_SELECTED);
	}
	else if(pWnd == GetDlgItem(IDC_LC_TSTEP)->m_hWnd)
	{
		m_lcStep.SetItemState((int)wParam, LVIS_SELECTED, LVIS_SELECTED);
	}

	return 0;
}
BOOL CTournament::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN) 
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}