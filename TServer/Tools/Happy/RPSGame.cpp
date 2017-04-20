// RPSGame.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Happy.h"
#include "RPSGame.h"
#include ".\rpsgame.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CRPSGame 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRPSGame, CDialog)
CRPSGame::CRPSGame(CWnd* pParent /*=NULL*/)
	: CDialog(CRPSGame::IDD, pParent)
{
	m_bSelectWorld	= NULL;
	m_bSelectType	= NULL;
	m_bTypeCur		= 0;
	m_bView			= FALSE;
}

CRPSGame::~CRPSGame()
{
}

void CRPSGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_RPSLIST,	m_lcRPCList);
	DDX_Control(pDX, IDC_CB_RPSWORLD,	m_cbWorld);
	DDX_Control(pDX, IDC_CB_RPSTYPE,	m_cbType);
}

BEGIN_MESSAGE_MAP(CRPSGame, CDialog)
	ON_CBN_SELCHANGE(IDC_CB_RPSWORLD, OnCbnSelchangeWorld)
	ON_CBN_SELCHANGE(IDC_CB_RPSTYPE, OnCbnSelchangeType)
	ON_BN_CLICKED(ID_BT_RPSUPDATE, OnBnClickedBtRpsupdate)
END_MESSAGE_MAP()

void CRPSGame::Init()
{	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	InitRPSList();
	InitType();
	InitWorld();
}

void CRPSGame::InitType()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_vType.clear();

	while(m_cbType.GetCount())
		m_cbType.DeleteString(0);

	//m_bSelectType = NULL;

	m_cbType.AddString(_T("(Selected Type)"));
	m_cbType.SetCurSel(0);
}

void CRPSGame::AddType(BYTE bType)
{
	vector<BYTE>::iterator it;
	for(it = m_vType.begin(); it != m_vType.end(); it++)
	{
		if(bType == *it)
			return;
	}
	m_vType.push_back(bType);
	CString str;
	str.Format("%d", bType);
	m_cbType.AddString(str);
}

void CRPSGame::InitWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

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
}

void CRPSGame::InitRPSList()
{
	ClearList();

	m_lcRPCList.SetEnableSubCtrl(TRUE);

	while(m_lcRPCList.GetColumns())
        m_lcRPCList.DeleteColumn(0);
	
	const TCHAR *lpszHeader[] = {_T("Round"), _T("Win"), _T("Draw"), _T("Lose"),_T("WinnerLimit"),_T("Period(Day)"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
		lvcolumn.fmt = LVCFMT_CENTER;
	for( int i=0; ; i++ )
	{
		if( lpszHeader[i] == NULL )
			break;

		lvcolumn.pszText = (TCHAR *)lpszHeader[i];
		lvcolumn.iSubItem = i;
		m_lcRPCList.InsertColumn(i, &lvcolumn);
	}		

	m_lcRPCList.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);	
	// 리스트 아이템 높이 설정
	m_lcRPCList.SetRowHeight(20);

	// 언더라인 설정
	m_lcRPCList.SetUnderLine(TRUE);
	m_lcRPCList.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	m_lcRPCList.m_HeaderCtrl.SetHeight(25);

	int arColWidthList[] = {70,70,70,70,95,95};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcRPCList.SetColumnWidth(i, arColWidthList[i]);
}

int CRPSGame::AddEventList(LPRPSGameData pData)
{
	if(!pData)
		return -1;

	int nRow = m_lcRPCList.GetItemCount();	
	int nCol = 0;

	CString str;

	// Round	
	str.Format("%d",pData->m_bWinCount);
	m_lcRPCList.InsertItem(nRow, str);

	// Win 
	nCol++;	
	str.Format("%d",pData->m_bProbWin);
	m_lcRPCList.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcRPCList.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcRPCList.SetItemText(nRow, nCol, str);

	// Draw
	nCol++;	
	str.Format("%d",pData->m_bProbDraw);
	m_lcRPCList.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcRPCList.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcRPCList.SetItemText(nRow, nCol, str);
	
	// Lose
	nCol++;	
	str.Format("%d",pData->m_bProbLose);
	m_lcRPCList.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcRPCList.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcRPCList.SetItemText(nRow, nCol, str);
	
	// WinnerLimit
	nCol++;	
	str.Format("%d",pData->m_wWinKeep);
	m_lcRPCList.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcRPCList.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcRPCList.SetItemText(nRow, nCol, str);
	
	// LimitPeriod
	nCol++;	
	str.Format("%d",pData->m_wWinPeriod);
	m_lcRPCList.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcRPCList.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcRPCList.SetItemText(nRow, nCol, str);

	return nRow;
}

void CRPSGame::RefreshList()
{
	ClearList();

	if(!m_bSelectType)
		return;

	m_cbType.SetCurSel(m_bTypeCur);

	for(DWORD i = 0; i < m_vRpsList.size(); i++)
	{
		LPRPSGameData pData = &m_vRpsList[i];
		if(m_bSelectType == pData->m_bType)
		{
			if(AddEventList(pData) == -1)
				return;
		}
	}
}

void CRPSGame::ClearList()
{
	m_lcRPCList.DeleteAllItems();
}


// CRPSGame 메시지 처리기입니다.
void CRPSGame::OnCbnSelchangeWorld()
{
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

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;
	if( itG == pDoc->m_mapGroup.end() )
	{
		AfxMessageBox(_T("Not found server"));
		return;
	}
	
	m_bTypeCur = 0;
	m_bSelectType = 0;
	m_bSelectWorld = (*itG).second->m_bID;

	InitType();
	ClearList();
	if(m_bSelectWorld)
	{
		m_bView = TRUE;
		pDoc->SendCT_RPSGAMEDATA_REQ(m_bSelectWorld);
	}
}

void CRPSGame::OnCbnSelchangeType()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_bTypeCur = (BYTE)m_cbType.GetCurSel();
	if(!m_bTypeCur)
	{
		ClearList();
		return;
	}

	CString str;
	m_cbType.GetLBText(m_bTypeCur,str);
	m_bSelectType = atoi(str);

	if(m_bSelectWorld && m_bSelectType)
		RefreshList();
}

int CRPSGame::GetSelectItem()
{
	// 선택된 아이템 찾기		
	int iCount = m_lcRPCList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcRPCList.GetItemState(i, LVIS_SELECTED) != 0 )
			return i;
	}
	return -1;
}

void CRPSGame::OnBnClickedBtRpsupdate()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	VRPSGameData vRpsUpdate;
	int nCount = m_lcRPCList.GetItemCount();
	
	for(int i = 0; i < nCount; i++)
	{		
		TRPSGameData tData;
		tData.m_bType		= m_bSelectType;
		tData.m_bWinCount	= m_lcRPCList.GetItemInt(i,0);
		tData.m_bProbWin	= m_lcRPCList.GetItemInt(i,1);
		tData.m_bProbDraw	= m_lcRPCList.GetItemInt(i,2);
		tData.m_bProbLose	= m_lcRPCList.GetItemInt(i,3);
		tData.m_wWinKeep	= m_lcRPCList.GetItemInt(i,4);
		tData.m_wWinPeriod	= m_lcRPCList.GetItemInt(i,5);

		if(tData.m_wWinKeep && !tData.m_wWinPeriod)
		{
			AfxMessageBox(_T("Period(Day) Error"));
			m_lcRPCList.SelectItem(i, 5);
			return;
		}
		
		LPRPSGameData pList = FindRPSData(tData.m_bType, tData.m_bWinCount);        
		if(!pList)
			return;

		if(	pList->m_bProbWin	!= tData.m_bProbWin		||
			pList->m_bProbDraw	!= tData.m_bProbDraw	||
			pList->m_bProbLose	!= tData.m_bProbLose	||
			pList->m_wWinKeep	!= tData.m_wWinKeep		||
			pList->m_wWinPeriod	!= tData.m_wWinPeriod	)
			vRpsUpdate.push_back(tData);
	}

	if(vRpsUpdate.size())
	{
		InitType();
		ClearList();
		pDoc->SendCT_RPSGAMECHANGE_REQ(m_bSelectWorld, &vRpsUpdate);
	}
	else
		AfxMessageBox(_T("There are no update contents."));

}

LPRPSGameData CRPSGame::FindRPSData(BYTE bType, BYTE bRound)
{
	VRPSGameData::iterator it;
	for(it = m_vRpsList.begin(); it != m_vRpsList.end(); it++)
	{
		LPRPSGameData pData = &(*it);
		if(pData->m_bType == bType && pData->m_bWinCount == bRound)
			return pData;
	}
	return NULL;
}

void CRPSGame::CheckValue(int nRow, int nCol)
{
	int nVal = 0;
	int nEditVal = m_lcRPCList.GetItemInt(nRow, nCol);

	switch(nCol)
	{
	case 1:	//	win
		nVal = FindRPSData(m_bSelectType, m_lcRPCList.GetItemInt(nRow,0))->m_bProbWin;			
		break;
	case 2:	//	Draw
		nVal = FindRPSData(m_bSelectType, m_lcRPCList.GetItemInt(nRow,0))->m_bProbDraw;
		break;
	case 3:	//	Lose
		nVal = FindRPSData(m_bSelectType, m_lcRPCList.GetItemInt(nRow,0))->m_bProbLose;
		break;
	case 4:	//	Keep
		nVal = FindRPSData(m_bSelectType, m_lcRPCList.GetItemInt(nRow,0))->m_wWinKeep;
		break;
	case 5:	//	Period
		nVal = FindRPSData(m_bSelectType, m_lcRPCList.GetItemInt(nRow,0))->m_wWinPeriod;
		break;
	}
	if(nVal == nEditVal)
		m_lcRPCList.SetItemTextColor(nRow, nCol, RGB(0,0,0), -1);
	else			
		m_lcRPCList.SetItemTextColor(nRow, nCol, RGB(255,0,0), -1);
}

// Enter / Escape block
BOOL CRPSGame::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
		(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}
