// EventInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "MainFrm.h"
#include "EventInfo.h"
#include ".\eventinfo.h"


// CEventInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventInfo, CDialog)
CEventInfo::CEventInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CEventInfo::IDD, pParent)
{	
	m_dwEventIndex = 0;
	m_bDateType = 0;
}

CEventInfo::~CEventInfo()
{
}

void CEventInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_EVENT, m_cbEventType);
	DDX_Control(pDX, IDC_CB_SVR, m_cbServer);
	DDX_Control(pDX, IDC_DT_START, m_dtStart);
	DDX_Control(pDX, IDC_DT_END, m_dtEnd);
	DDX_Control(pDX, IDC_DT_START2, m_dtStart2);
	DDX_Control(pDX, IDC_DT_END2, m_dtEnd2);
	DDX_Control(pDX, IDC_CB_CASHITEM, m_cbCashItem);
	DDX_Control(pDX, IDC_LB_CASHITEM, m_lbCashItemList);
	DDX_Control(pDX, IDC_LB_SPAWNID, m_lbSpawnIDList);
	DDX_Control(pDX, IDC_LB_REGENLIST, m_lcRegenList);
	DDX_Control(pDX, IDC_CB_LOTTYPE, m_cbLotType);
	//DDX_Control(pDX, IDC_LC_LOT, m_lcLottery);
	DDX_Control(pDX, IDC_DT_LOT, m_dtLotStart);
	DDX_Control(pDX, IDC_LC_LOT, m_lcLottery);
}


BEGIN_MESSAGE_MAP(CEventInfo, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedUpdate)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedClose)
	ON_BN_CLICKED(IDC_BTN_CASHADD, OnBnClickedBtnCashAdd)
	ON_BN_CLICKED(IDC_BTN_CASHDEL, OnBnClickedBtnCashDelete)
	ON_CBN_SELCHANGE(IDC_CB_EVENT, OnCbnSelchangeCbEvent)
	ON_BN_CLICKED(IDC_BTN_SPAWNADD, OnBnClickedBtnSpawnAdd)
	ON_BN_CLICKED(IDC_BTN_SPAWNDEL, OnBnClickedBtnSpawnDelete)
	ON_BN_CLICKED(IDC_BTN_ADDREGEN, OnBnClickedBtnAddRegen)
	ON_BN_CLICKED(IDC_BTN_REGENDEL, OnBnClickedBtnRegenDel)
	ON_LBN_SELCHANGE(IDC_LB_REGENLIST, OnLbnSelchangeLbRegenlist)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BTN_LOTADD, OnBnClickedBtnLotadd)
	ON_BN_CLICKED(IDC_BTN_LOTDEL, OnBnClickedBtnLotdel)
	ON_EN_CHANGE(IDC_EB_REGENMAPID, OnEnChangeEbRegenmapid)
END_MESSAGE_MAP()


// CEventInfo 메시지 처리기입니다.

BOOL CEventInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return FALSE;
	}

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(!m_dwEventIndex)
		Init();
	else
	{
		MAPEVENTINFO::iterator it = pDoc->m_mapEventInfo.find(m_dwEventIndex);
		if( it == pDoc->m_mapEventInfo.end())
			Init();
		else
            Init( &(*it).second );
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEventInfo::Init()
{	
	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_cbEventType.AddString(_T("SELECT"));
	MAPBYTESTRING::iterator it;
	for( it = pDoc->m_mapEventName.begin();it != pDoc->m_mapEventName.end(); it++)
		m_cbEventType.AddString( (*it).second );

	m_cbEventType.SetCurSel(0);

	DWORD dwID;
	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;

	m_cbServer.AddString(_T("SELECT"));
	MAPDWORDSTRING::iterator itS;
	for( itS = pDoc->m_mapEventSvr.begin(); itS != pDoc->m_mapEventSvr.end(); itS++)
	{
		dwID = (*itS).first;
		bGroupID = SVRGROUP(dwID);
		bSvrType = SVRTYPE(dwID);
		bSvrID = SVRID(dwID);

		if(bSvrType == SVRGRP_WORLDSVR)
			m_cbServer.AddString((*itS).second);		
	}
	m_cbServer.SetCurSel(0);

	SetDlgItemText(IDC_EB_EVENTVALUE,_T(""));
	SetDlgItemText(IDC_EB_STARTHOUR,_T(""));
	SetDlgItemText(IDC_EB_STARTMIN,_T(""));
	SetDlgItemText(IDC_EB_ENDHOUR,_T(""));
	SetDlgItemText(IDC_EB_ENDMIN,_T(""));
	SetDlgItemInt(IDC_EB_STARTALARM, 0, FALSE);
	SetDlgItemInt(IDC_EB_ENDALARM, 0, FALSE);
	SetDlgItemText(IDC_EB_STARTMSG,_T(""));
	SetDlgItemText(IDC_EB_ENDMSG,_T(""));
	SetDlgItemText(IDC_EB_EVENTTITLE, _T(""));
	SetDlgItemText(IDC_EB_CASHSALE, _T(""));
	SetDlgItemText(IDC_EB_SPAWNID, _T(""));
	SetDlgItemText(IDC_EB_MAPID, _T(""));
	SetDlgItemText(IDC_EB_LOTITEMID, _T(""));
	SetDlgItemText(IDC_EB_LOTNUM, _T(""));
	SetDlgItemText(IDC_EB_LOTNUM2, _T(""));
	SetDlgItemText(IDC_EB_LOTMAIL, _T(""));
	SetDlgItemText(IDC_EB_LOTMAIL2, _T(""));

	int nCount = m_lbCashItemList.GetCount();
	for(int i = 0; i < nCount ; i++)
		m_lbCashItemList.DeleteString(i);

	CString strCashItem;
	strCashItem = _T("0 - ALL");
	m_cbCashItem.AddString(strCashItem);

	MAPTCASHITEM::iterator itC;
	for(itC = pDoc->m_mapCashItem.begin(); itC != pDoc->m_mapCashItem.end(); itC++)
	{
		strCashItem.Format("%d - %s",(*itC).second.m_wID,(*itC).second.m_strName);
		
		m_cbCashItem.AddString(strCashItem);
	}
	m_cbCashItem.SetCurSel(0);

	m_cbLotType.AddString(_T("ALL MAP"));
	m_cbLotType.AddString(_T("TOURNAMENT"));
	m_cbLotType.SetCurSel(0);

	m_lcLottery.SetExtendedStyle(LVS_EX_FULLROWSELECT);//|LVS_EX_HEADERDRAGDROP);
	m_lcLottery.m_HeaderCtrl.SetHeight(18);
	m_lcLottery.SetRowHeight(18);

	m_lcLottery.InsertColumn(0,  _T("St"), LVCFMT_LEFT, 40);
	m_lcLottery.InsertColumn(1,  _T("ItemID"), LVCFMT_LEFT, 80);
	m_lcLottery.InsertColumn(2,  _T("Num"), LVCFMT_LEFT, 50);
	m_lcLottery.InsertColumn(3,  _T("Winner"), LVCFMT_LEFT, 80);
	
	SetDlgItemInt(IDC_EB_MINL, 0, 0);
	SetDlgItemInt(IDC_EB_MAXL, 255, 0);

	SetViewCtrl(0, 0, TRUE);
}

void CEventInfo::Init( LPEVENTINFO pData )
{
	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	int nIndex = 0;
	int nCount = 0;
	m_cbEventType.AddString(_T("SELECT"));
	MAPBYTESTRING::iterator it;
	for( it = pDoc->m_mapEventName.begin();it != pDoc->m_mapEventName.end(); it++)
	{
		nCount++;
		m_cbEventType.AddString( (*it).second );

		if( pData->m_bID == (*it).first)
			nIndex = nCount;
	}
	m_cbEventType.SetCurSel(nIndex);
	

	DWORD dwID;
	BYTE bGroupID;
	BYTE bSvrType;
	BYTE bSvrID;
	nIndex = 0;
	nCount = 0;
	m_cbServer.AddString(_T("SELECT"));

	MAPDWORDSTRING::iterator itS;
	for( itS = pDoc->m_mapEventSvr.begin(); itS != pDoc->m_mapEventSvr.end(); itS++)
	{
		dwID = (*itS).first;
		bGroupID = SVRGROUP(dwID);
		bSvrType = SVRTYPE(dwID);
		bSvrID = SVRID(dwID);

		if(bSvrType == SVRGRP_WORLDSVR)
		{
			nCount++;
			m_cbServer.AddString((*itS).second);
			if(!nIndex && pData->m_bGroupID == bGroupID && pData->m_bSvrType == bSvrType && pData->m_bSvrID == bSvrID)
				nIndex = nCount;
		}
		
	}
	m_cbServer.SetCurSel(nIndex);

	CTime ts(pData->m_dStartDate);
	m_dtStart.SetTime(&ts);
	m_dtStart2.SetTime(&ts);
	m_dtLotStart.SetTime(&ts);
	CTime te(pData->m_dEndDate);
	m_dtEnd.SetTime(&te);
	m_dtEnd2.SetTime(&te);	

	SetDlgItemInt(IDC_EB_EVENTVALUE,pData->m_wValue);	
	SetDlgItemInt(IDC_EB_MAPID,pData->m_wMapID);

	SetDlgItemInt(IDC_EB_STARTHOUR,ts.GetHour());
	SetDlgItemInt(IDC_EB_STARTMIN, ts.GetMinute());
	SetDlgItemInt(IDC_EB_ENDHOUR, te.GetHour());
	SetDlgItemInt(IDC_EB_ENDMIN, te.GetMinute());
	SetDlgItemInt(IDC_EB_STARTHOUR2,ts.GetHour());
	SetDlgItemInt(IDC_EB_STARTMIN2, ts.GetMinute());
	SetDlgItemInt(IDC_EB_LOTH,ts.GetHour());
	SetDlgItemInt(IDC_EB_LOTM, ts.GetMinute());
	SetDlgItemInt(IDC_EB_ENDHOUR2, te.GetHour());
	SetDlgItemInt(IDC_EB_ENDMIN2, te.GetMinute());

	SetDlgItemInt(IDC_EB_STARTALARM, pData->m_dwStartAlarm);
	SetDlgItemInt(IDC_EB_ENDALARM, pData->m_dwEndAlarm);
	SetDlgItemText(IDC_EB_STARTMSG, pData->m_strStartMsg);
	SetDlgItemText(IDC_EB_ENDMSG, pData->m_strEndMsg);
	SetDlgItemText(IDC_EB_EVENTTITLE, pData->m_strTitle);
	
	int i;
	int iCount = m_lbCashItemList.GetCount();
	for(i = 0; i < iCount ; i++)
		m_lbCashItemList.DeleteString(i);

	CString strCashItem;
	strCashItem = _T("ALL");
	m_cbCashItem.AddString(strCashItem);

	MAPTCASHITEM::iterator itC;
	for(itC = pDoc->m_mapCashItem.begin(); itC != pDoc->m_mapCashItem.end(); itC++)
	{
		strCashItem.Format("%d - %s",(*itC).second.m_wID,(*itC).second.m_strName);
		
		m_cbCashItem.AddString(strCashItem);
	}
	m_cbCashItem.SetCurSel(0);

	CString strCashSale;	
	for(i = 0; i < (int)pData->m_vCashItem.size(); i++)
	{
		strCashItem = _T("");
		MAPTCASHITEM::iterator itC = pDoc->m_mapCashItem.find(pData->m_vCashItem[i].m_wID);
		if( itC != pDoc->m_mapCashItem.end() )
			strCashItem = (*itC).second.m_strName;
		else if(pData->m_vCashItem[i].m_wID == 0 )
			strCashItem = _T("ALL");

		strCashSale.Format("%d - %s - %d %%",pData->m_vCashItem[i].m_wID , strCashItem,pData->m_vCashItem[i].m_bSaleValue);

		m_lbCashItemList.AddString(strCashSale);
	}

	CString str;
	for(i = 0; i < (int)pData->m_stMONEVENT.m_vSpawnID.size(); i++)
	{
		str.Format("%d",pData->m_stMONEVENT.m_vSpawnID[i]);
		m_lbSpawnIDList.AddString(str);
	}

	CString strVal1,strVal2,strVal3,strVal4,strVal5,strVal6, strResult;
	
	for( i = 0; i < (int)pData->m_vMONREGEN.size(); i++)
	{
		strVal1.Format("%d",pData->m_vMONREGEN[i].m_wMonID);
		strVal2.Format("%d",pData->m_vMONREGEN[i].m_dwDelay / TICKCOUNT_MIN);
		strVal3.Format("%d",pData->m_vMONREGEN[i].m_wMapID);
		strVal4.Format("%.2f",pData->m_vMONREGEN[i].m_fPosX);
		strVal5.Format("%.2f",pData->m_vMONREGEN[i].m_fPosY);
		strVal6.Format("%.2f",pData->m_vMONREGEN[i].m_fPosZ);

		strResult = strVal1 + _T("-") + strVal2 + _T("-") + strVal3 + _T("-") + strVal4 + _T("-") +strVal5 + _T("-") + strVal6;
		m_lcRegenList.AddString(strResult);		
	}
	////	ITEM LOTTERY	
	m_cbLotType.AddString(_T("ALL MAP"));
	m_cbLotType.AddString(_T("TOURNAMENT"));

	m_cbLotType.SetCurSel(pData->m_wMapID);

	m_lcLottery.DeleteAllItems();
	if(pData->m_bID == EVENT_LOTTERY)
	{
		while(m_lcLottery.GetColumns())
			m_lcLottery.DeleteColumn(0);

		m_lcLottery.SetExtendedStyle(LVS_EX_FULLROWSELECT);//|LVS_EX_HEADERDRAGDROP);
		m_lcLottery.m_HeaderCtrl.SetHeight(18);
		m_lcLottery.SetRowHeight(18);

		m_lcLottery.InsertColumn(0,  _T("St"), LVCFMT_LEFT, 40);
		m_lcLottery.InsertColumn(1,  _T("ItemID"), LVCFMT_LEFT, 80);
		m_lcLottery.InsertColumn(2,  _T("Num"), LVCFMT_LEFT, 50);
		m_lcLottery.InsertColumn(3,  _T("Winner"), LVCFMT_LEFT, 80);

		for(int iRow = 0; iRow < (int)pData->m_vLOTTERY.size(); iRow++)
		{
			m_lcLottery.InsertItem(iRow, _T(""));
			m_lcLottery.SetItemInt(iRow,0, iRow+1);
			m_lcLottery.SetItemInt(iRow,1,(int)pData->m_vLOTTERY[iRow].m_wItemID);
			m_lcLottery.SetItemInt(iRow,2,(int)pData->m_vLOTTERY[iRow].m_bNum);
			m_lcLottery.SetItemInt(iRow,3,(int)pData->m_vLOTTERY[iRow].m_wWinner);
		}
	}
	else if(pData->m_bID == EVENT_GIFTTIME && pData->m_vLOTTERY.size() == 1)
	{
		SetDlgItemInt(IDC_EB_LOTITEMID, pData->m_vLOTTERY[0].m_wItemID);
		SetDlgItemInt(IDC_EB_LOTNUM, pData->m_vLOTTERY[0].m_bNum);
		SetDlgItemInt(IDC_EB_LOTNUM2, pData->m_vLOTTERY[0].m_wWinner);

		SetDlgItemInt(IDC_EB_MINL, (int)HIBYTE(pData->m_wValue), 0);
		SetDlgItemInt(IDC_EB_MAXL, (int)LOBYTE(pData->m_wValue), 0);
	}

	int nc = pData->m_strLotMsg.Find('|');
	SetDlgItemText(IDC_EB_LOTMAIL, pData->m_strLotMsg.Left(nc));
	SetDlgItemText(IDC_EB_LOTMAIL2, pData->m_strLotMsg.Mid(nc+1,pData->m_strLotMsg.GetLength()));
	
	SetViewCtrl(pData->m_bID, pData->m_bPartTime, TRUE);
}


void CEventInfo::OnBnClickedUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	EVENTINFO stEVENTINFO;
	stEVENTINFO.Reset();
	int nValLenth = 0;

	//	이벤트 번호
	stEVENTINFO.m_dwIndex = m_dwEventIndex;
	//	이벤트 종류
	stEVENTINFO.m_bID = m_cbEventType.GetCurSel();
	if(!stEVENTINFO.m_bID)
	{
		AfxMessageBox(_T("You should select the event"));
		return;
	}

	//	이벤트 이름
	GetDlgItemText(IDC_EB_EVENTTITLE,stEVENTINFO.m_strTitle);
	if( stEVENTINFO.m_strTitle.GetLength() >= MAX_BOARD_TITLE )
	{
		AfxMessageBox(_T("The title is too long"));
		return;
	}

	//	서버 이름
	CString str;
	m_cbServer.GetLBText(m_cbServer.GetCurSel(),str);

	if( str ==_T("SELECT"))
	{
		AfxMessageBox(_T("You should select the server"));
		return;
	}

	//	캐쉬 할인
	if(stEVENTINFO.m_bID == EVENT_CASHSALE)
	{
		int nCount = m_cbServer.GetCount();
		for(int i = 0; i < nCount; i++)
		{
			//	모든서버 선택
			m_cbServer.GetLBText(i,str);
			if( str == STR_WORLD_ALL )
				break;
		}
		m_cbServer.SetCurSel(i);
	}

	//	맵서버 id 찾기
	DWORD dwID;
	MAPDWORDSTRING::iterator itS;
	for(itS = pDoc->m_mapEventSvr.begin(); itS != pDoc->m_mapEventSvr.end(); itS++)
	{
		if( (*itS).second == str)
		{
			dwID = (*itS).first;
			break;
		}
	}

	//	서버 아이디 분류
	stEVENTINFO.m_bGroupID = SVRGROUP(dwID);
	stEVENTINFO.m_bSvrType = SVRTYPE(dwID);
	stEVENTINFO.m_bSvrID = SVRID(dwID);

	//	이벤트 확률, 맵id
	stEVENTINFO.m_wValue = (WORD)GetDlgItemInt(IDC_EB_EVENTVALUE);
	stEVENTINFO.m_wMapID = (WORD)GetDlgItemInt(IDC_EB_MAPID);
	if(stEVENTINFO.m_bID == EVENT_CASHSALE || stEVENTINFO.m_bID == EVENT_MONSPAWN
		|| stEVENTINFO.m_bID == EVENT_MONREGEN)
		stEVENTINFO.m_wValue = 1;

	CString strCashSale;
	CString strID;
	CString strSale;
	int nStart,nMid,nEnd,nCount;

	////	이벤트 타입별
	switch (stEVENTINFO.m_bID)
	{
	case EVENT_CASHSALE:	//	캐쉬아이템 할인
		{
			//	아이템 리스트 아이템개수 확인
			if( m_lbCashItemList.GetCount() == 0 )
			{
				AfxMessageBox("Empty Data");
				return;
			}

			for(int i = 0; i < m_lbCashItemList.GetCount(); i++)
			{
				//	리스트내 문자열을 데이터로 전환
				m_lbCashItemList.GetText(i,strCashSale);
				nStart = strCashSale.Find('-');
				strID = strCashSale.Left(nStart-1);
				nMid = nStart+1;
				while(-1 != strCashSale.Find('-',nMid+1))
					nMid = strCashSale.Find('-',nMid+1);

				nEnd = strCashSale.Find('%',nMid+1);
				nCount = nEnd - nMid - 1;
				strSale = strCashSale.Mid(nMid+1,nCount);
				strSale.Trim();

				TCASHITEMSALE stSALEITEM;
				stSALEITEM.m_wID = atoi(strID);
				stSALEITEM.m_bSaleValue = (BYTE)atoi(strSale);
				if(stSALEITEM.m_bSaleValue > 100 )
				{
					AfxMessageBox(_T("Invalid Sale Value"));
					return;
				}


				//	중복선택된 캐쉬아이템 검사
				for(WORD i = 0; i < (WORD)stEVENTINFO.m_vCashItem.size(); i++)
				{
					if( stSALEITEM.m_wID == stEVENTINFO.m_vCashItem[i].m_wID)
					{
						AfxMessageBox(_T("Error : There is a Equal Cash Item"));
						return;
					}
				}
				stEVENTINFO.m_vCashItem.push_back(stSALEITEM);
			}
			break;
		}

	case EVENT_MONSPAWN:	//	몬스터 스폰
		{
			CString str1;
			CString str2;

			//	빈칸검사
			if( m_lbSpawnIDList.GetCount() == 0 )
			{
				AfxMessageBox("Empty Data");
				return;
			}

			for(int i = 0; i < m_lbSpawnIDList.GetCount(); i++)
			{
				//	id 얻기
				m_lbSpawnIDList.GetText(i,str1);
				WORD wSpawnID = (WORD)atoi(str1);

				BYTE bExist = FALSE;
				for(WORD i = 0; i < (WORD)stEVENTINFO.m_stMONEVENT.m_vSpawnID.size(); i++)
					if( stEVENTINFO.m_stMONEVENT.m_vSpawnID[i] == wSpawnID)
						bExist = TRUE;

				if(!bExist)
					stEVENTINFO.m_stMONEVENT.m_vSpawnID.push_back(wSpawnID);
			}

			stEVENTINFO.m_stMONEVENT.m_bStartAction = 0;
			stEVENTINFO.m_stMONEVENT.m_bEndAction = ACTION_AT_DEAD;
			break;
		}

	case EVENT_MONREGEN:	//	몬스터리젠
		{
			CString str1;

			if(m_lcRegenList.GetCount() == 0)
			{
				AfxMessageBox("Empty Data");
				return;
			}

			for(int i = 0; i < m_lcRegenList.GetCount(); i++)
			{
				m_lcRegenList.GetText(i,str1);

				int nPos = 0;
				CString strTok;

				BYTE bCount = 0;
				MONREGEN stMONREGEN;

				strTok = str1.Tokenize(_T("-"),nPos);
				while(strTok != _T(""))
				{
					switch(bCount)
					{
					case 0:	stMONREGEN.m_wMonID = atoi(strTok);						break;
					case 1: stMONREGEN.m_dwDelay = atoi(strTok) * TICKCOUNT_MIN;	break;
					case 2: stMONREGEN.m_wMapID = atoi(strTok);				break;
					case 3:	stMONREGEN.m_fPosX = (FLOAT)atof(strTok);		break;
					case 4: stMONREGEN.m_fPosY = (FLOAT)atof(strTok);		break;
					case 5: stMONREGEN.m_fPosZ = (FLOAT)atof(strTok);		break;
					}

					bCount++;
					strTok = str1.Tokenize(_T("-"),nPos);
				}

				stEVENTINFO.m_vMONREGEN.push_back(stMONREGEN);
			}
			break;
		}

	case EVENT_LOTTERY:	//	아이템 추첨
		{
			stEVENTINFO.m_wMapID = m_cbLotType.GetCurSel();
			if(! m_lcLottery.GetItemCount())
			{
				AfxMessageBox(_T("Enter the Reward ItemList"));
				return;
			}
			for(int i = 0; i < m_lcLottery.GetItemCount(); i++)
			{
				TLOTTERY stLOT;
				stLOT.m_wItemID	= (WORD)atoi(m_lcLottery.GetItemText(i, 1));
				stLOT.m_bNum	= (BYTE)atoi(m_lcLottery.GetItemText(i, 2));
				stLOT.m_wWinner	= (WORD)atoi(m_lcLottery.GetItemText(i, 3));

				stEVENTINFO.m_vLOTTERY.push_back(stLOT);
			}		
			//	편지 메시지
			CString tMail, tMail2;
			GetDlgItemText(IDC_EB_LOTMAIL,tMail);
			GetDlgItemText(IDC_EB_LOTMAIL2,tMail2);

			if(!tMail.GetLength() || !tMail2.GetLength())
			{
				AfxMessageBox(_T("Enter the Post"));
				return;
			}		
			stEVENTINFO.m_strLotMsg = tMail+'|'+tMail2;
			nValLenth += tMail.GetLength() + tMail2.GetLength() + 1;
			if(nValLenth > 1024)
			{
				AfxMessageBox(_T("Long to Message"));
				return;
			}

			//	날짜
			BYTE bStartHour = GetDlgItemInt(IDC_EB_LOTH);
			BYTE bStartMinute = GetDlgItemInt(IDC_EB_LOTM);
			if(bStartHour > 23 || bStartMinute > 59)
			{
				AfxMessageBox(_T("Enter the correct Time"));
				return;
			}

			CTime t;
			m_dtLotStart.GetTime(t);
			CTime ts(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute,0);
			stEVENTINFO.m_dStartDate = ts.GetTime();
			CTime te(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute + 1,0);
			stEVENTINFO.m_dEndDate = te.GetTime();
			break;
		}
	case EVENT_GIFTTIME:	//	아이템 배포?
		{
			TLOTTERY stGift;
			memset(&stGift, 0, sizeof(TLOTTERY));

			stGift.m_wItemID		= GetDlgItemInt(IDC_EB_LOTITEMID);
			stGift.m_bNum			= GetDlgItemInt(IDC_EB_LOTNUM);
			stGift.m_wWinner		= GetDlgItemInt(IDC_EB_LOTNUM2);

			if(!stGift.m_wItemID | !stGift.m_bNum)
				return;

			stEVENTINFO.m_vLOTTERY.clear();
			stEVENTINFO.m_vLOTTERY.push_back(stGift);

			//	레벨제한			
			BYTE bMinLevel = (BYTE)GetDlgItemInt(IDC_EB_MINL, 0);
			BYTE bMaxLevel = (BYTE)GetDlgItemInt(IDC_EB_MAXL, 0);
			if(bMinLevel >= bMaxLevel)
			{
				AfxMessageBox(_T("Invalid Level"));
				return;
			}

			stEVENTINFO.m_wValue = MAKEWORD(bMaxLevel, bMinLevel);

			//	편지 메시지
			CString tMail, tMail2;
			GetDlgItemText(IDC_EB_LOTMAIL,tMail);
			GetDlgItemText(IDC_EB_LOTMAIL2,tMail2);

			if(!tMail.GetLength() || !tMail2.GetLength())
			{
				AfxMessageBox(_T("Enter the Post"));
				return;
			}		
			stEVENTINFO.m_strLotMsg = tMail+'|'+tMail2;
			nValLenth += tMail.GetLength() + tMail2.GetLength() + 1;
			if(nValLenth > 1024)
			{
				AfxMessageBox(_T("Long to Message"));
				return;
			}

			//	날짜
			BYTE bStartHour = GetDlgItemInt(IDC_EB_LOTH);
			BYTE bStartMinute = GetDlgItemInt(IDC_EB_LOTM);
			if(bStartHour > 23 || bStartMinute > 59)
			{
				AfxMessageBox(_T("Enter the correct Time"));
				return;
			}

			CTime t;
			m_dtLotStart.GetTime(t);
			CTime ts(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute,0);
			stEVENTINFO.m_dStartDate = ts.GetTime();
			CTime te(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute + 1,0);
			stEVENTINFO.m_dEndDate = te.GetTime();
			break;
		}
	}

	//	기간 타입
	if(stEVENTINFO.m_bID != EVENT_LOTTERY && stEVENTINFO.m_bID != EVENT_GIFTTIME)
	{
		BYTE bStartHour;
		BYTE bStartMinute;
		BYTE bEndHour;
		BYTE bEndMinute;

		stEVENTINFO.m_bPartTime = m_bDateType;

		//	기간타입 확인
		if( !m_bDateType )
		{
			//	시간 구하기
			bStartHour = (BYTE)GetDlgItemInt(IDC_EB_STARTHOUR);
			bStartMinute = (BYTE)GetDlgItemInt(IDC_EB_STARTMIN);
			bEndHour = (BYTE)GetDlgItemInt(IDC_EB_ENDHOUR);
			bEndMinute = (BYTE)GetDlgItemInt(IDC_EB_ENDMIN);

			//	날짜
			CTime t;
			m_dtStart.GetTime(t);
			CTime ts(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute,0);
			stEVENTINFO.m_dStartDate = ts.GetTime();
			m_dtEnd.GetTime(t);
			CTime te(t.GetYear(),t.GetMonth(),t.GetDay(),bEndHour,bEndMinute,0);
			stEVENTINFO.m_dEndDate = te.GetTime();
		}
		else
		{
			//	시간 구하기
			bStartHour = (BYTE)GetDlgItemInt(IDC_EB_STARTHOUR2);
			bStartMinute = (BYTE)GetDlgItemInt(IDC_EB_STARTMIN2);
			bEndHour = (BYTE)GetDlgItemInt(IDC_EB_ENDHOUR2);
			bEndMinute = (BYTE)GetDlgItemInt(IDC_EB_ENDMIN2);

			//	날짜
			CTime t;
			m_dtStart2.GetTime(t);
			CTime ts(t.GetYear(),t.GetMonth(),t.GetDay(),bStartHour,bStartMinute,0);
			stEVENTINFO.m_dStartDate = ts.GetTime();
			m_dtEnd2.GetTime(t);
			CTime te(t.GetYear(),t.GetMonth(),t.GetDay(),bEndHour,bEndMinute,0);
			stEVENTINFO.m_dEndDate = te.GetTime();		
		}

		if( stEVENTINFO.m_dEndDate <= stEVENTINFO.m_dStartDate)
		{
			AfxMessageBox(_T("Enter the correct Date"));
			return;			
		}
		if(bStartHour > 23 || bStartMinute > 59 || bEndHour > 23 || bEndMinute > 59)
		{
			AfxMessageBox(_T("Enter the correct Time"));
			return;
		}
	}
	else
		stEVENTINFO.m_bPartTime = TRUE;

	//	알람
	stEVENTINFO.m_dwStartAlarm = GetDlgItemInt(IDC_EB_STARTALARM);
	GetDlgItemText(IDC_EB_STARTMSG,stEVENTINFO.m_strStartMsg);

	if( stEVENTINFO.m_bID != EVENT_LOTTERY && stEVENTINFO.m_bID != EVENT_GIFTTIME)
	{
		stEVENTINFO.m_dwEndAlarm = GetDlgItemInt(IDC_EB_ENDALARM);
		GetDlgItemText(IDC_EB_ENDMSG,stEVENTINFO.m_strEndMsg);
	}

	//Send
	pDoc->SendCT_EVENTCHANGE_REQ(m_bEventType,&stEVENTINFO);

	//OnOK();
}

void CEventInfo::SetViewCtrl(BYTE bEventID, BYTE bDateType, BYTE bEnable)
{
	m_cbEventType.EnableWindow(bEnable);
	GetDlgItem(IDC_EB_EVENTTITLE)->EnableWindow(bEnable);

	if(!bEventID)
		bEnable = FALSE;

	SetValueCtrl(bEventID, bEnable);
	SetMapIDCtrl(bEventID, bEnable);
	SetWorldCtrl(bEventID, bEnable);
	SetCashItemCtrl(bEventID, bEnable);
	SetSpawnIDCtrl(bEventID, bEnable);
	SetMonRegenCtrl(bEventID, bEnable);	
	SetLotteryCtrl(bEventID, bEnable);
	SetPeriodCtrl(bDateType, bEventID, bEnable);

	GetDlgItem(IDC_EB_STARTALARM)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_ENDALARM)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_STARTMSG)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_ENDMSG)->EnableWindow(bEnable);
	GetDlgItem(IDOK)->ShowWindow(bEnable);
}

void CEventInfo::SetValueCtrl( BYTE bEventID, BYTE bEnable)
{
	if(!bEventID || bEventID == EVENT_CASHSALE || bEventID == EVENT_MONSPAWN || bEventID == EVENT_MONREGEN || bEventID == EVENT_LOTTERY || bEventID == EVENT_GIFTTIME)
		bEventID = FALSE;
	else
	{
		bEventID = TRUE;
		
		GetDlgItem(IDC_EB_EVENTVALUE)->EnableWindow(bEnable);
		GetDlgItem(IDC_STATIC_V1)->ShowWindow(bEnable);
		GetDlgItem(IDC_STATIC_V2)->ShowWindow(bEnable);
	}
	GetDlgItem(IDC_EB_EVENTVALUE)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_V1)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_V2)->ShowWindow(bEventID);
}

void CEventInfo::SetMapIDCtrl(BYTE bEventID, BYTE bEnable)
{
	if(bEventID == EVENT_ITEMDROP || bEventID == EVENT_ITEMMAGICDROP || bEventID == EVENT_MONEYDROP)
	{
		bEventID = TRUE;
		GetDlgItem(IDC_EB_MAPID)->EnableWindow(bEnable);
	}
	else
		bEventID = FALSE;

	GetDlgItem(IDC_EB_MAPID)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M)->ShowWindow(bEventID);

}
void CEventInfo::SetWorldCtrl(BYTE bEventID, BYTE bEnable)
{
	CString str;
	int nCount = m_cbServer.GetCount();
	for(int i = 0; i < nCount; i++)
	{			
		m_cbServer.GetLBText(i,str);
		if( str == STR_WORLD_ALL )
			break;
	}
	if(bEventID == EVENT_CASHSALE)
		m_cbServer.SetCurSel(i);
	
	m_cbServer.EnableWindow((bEventID == EVENT_CASHSALE || !bEnable)? FALSE : TRUE);
}

void CEventInfo::SetPeriodCtrl(BYTE bPeriodOn, BYTE bEventID, BYTE bEnable)
{
	m_bDateType = bPeriodOn;

	if(bEventID == EVENT_LOTTERY || bEventID == EVENT_GIFTTIME)
	{
		GetDlgItem(IDC_DATESET)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);

		m_dtStart.ShowWindow(SW_HIDE);
		m_dtEnd.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_STARTHOUR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_STARTMIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_ENDHOUR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_ENDMIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_T6)->ShowWindow(SW_HIDE);

		m_dtStart2.ShowWindow(SW_HIDE);
		m_dtEnd2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_STARTHOUR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_STARTMIN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_ENDHOUR2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_ENDMIN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TT4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TT5)->ShowWindow(SW_HIDE);
		return;
	}

	GetDlgItem(IDC_DATESET)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOWNORMAL);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOWNORMAL);

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(!bPeriodOn);	
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(bPeriodOn);	

	m_dtStart.ShowWindow(!bPeriodOn);
	m_dtEnd.ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_EB_STARTHOUR)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_EB_STARTMIN)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_EB_ENDHOUR)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_EB_ENDMIN)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T1)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T2)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T3)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T4)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T5)->ShowWindow(!bPeriodOn);
	GetDlgItem(IDC_STATIC_T6)->ShowWindow(!bPeriodOn);

	m_dtStart2.ShowWindow(bPeriodOn);
	m_dtEnd2.ShowWindow(bPeriodOn);
	GetDlgItem(IDC_EB_STARTHOUR2)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_EB_STARTMIN2)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_EB_ENDHOUR2)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_EB_ENDMIN2)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_STATIC_TT1)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_STATIC_TT2)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_STATIC_TT4)->ShowWindow(bPeriodOn);
	GetDlgItem(IDC_STATIC_TT5)->ShowWindow(bPeriodOn);
	
	GetDlgItem(IDC_RADIO1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO2)->EnableWindow(bEnable);

	if(bPeriodOn)
	{
		m_dtStart2.EnableWindow(bEnable);
		m_dtEnd2.EnableWindow(bEnable);
		GetDlgItem(IDC_EB_STARTHOUR2)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_STARTMIN2)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_ENDHOUR2)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_ENDMIN2)->EnableWindow(bEnable);
	}
	else
	{
		m_dtStart.EnableWindow(bEnable);
		m_dtEnd.EnableWindow(bEnable);
		GetDlgItem(IDC_EB_STARTHOUR)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_STARTMIN)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_ENDHOUR)->EnableWindow(bEnable);
		GetDlgItem(IDC_EB_ENDMIN)->EnableWindow(bEnable);
	}
}

void CEventInfo::SetCashItemCtrl( BYTE bEventID, BYTE bEnable)
{
	if( bEventID == EVENT_CASHSALE)
		bEventID = TRUE;
	else 
		bEventID = FALSE;

	m_lbCashItemList.ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC1)->ShowWindow(bEventID);

	if(!bEnable)
		bEventID = bEnable;

	m_cbCashItem.ShowWindow(bEventID);
	GetDlgItem(IDC_EB_CASHSALE)->ShowWindow(bEventID);	
	GetDlgItem(IDC_BTN_CASHADD)->ShowWindow(bEventID);
	GetDlgItem(IDC_BTN_CASHDEL)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC2)->ShowWindow(bEventID);


	UpdateData(FALSE);

}

void CEventInfo::SetSpawnIDCtrl(BYTE bEventID, BYTE bEnable)
{
	if( bEventID == EVENT_MONSPAWN)
		bEventID = TRUE;
	else 
		bEventID = FALSE;

	m_lbSpawnIDList.ShowWindow(bEventID);

	GetDlgItem(IDC_STATIC3)->ShowWindow(bEventID);
	if(!bEnable)
		bEventID = bEnable;

	GetDlgItem(IDC_EB_SPAWNID)->ShowWindow(bEventID);
	GetDlgItem(IDC_BTN_SPAWNADD)->ShowWindow(bEventID);
	GetDlgItem(IDC_BTN_SPAWNDEL)->ShowWindow(bEventID);

}

void CEventInfo::SetMonRegenCtrl(BYTE bEventID, BYTE bEnable)
{	
	if( bEventID == EVENT_MONREGEN)
		bEventID = TRUE;
	else 
		bEventID = FALSE;

	GetDlgItem(IDC_LB_REGENLIST)->ShowWindow(bEventID);

	GetDlgItem(IDC_EB_MONID)->ShowWindow(bEventID);
	GetDlgItem(IDC_EB_MONDELAY)->ShowWindow(bEventID);
	GetDlgItem(IDC_EB_REGENMAPID)->ShowWindow(bEventID);
	GetDlgItem(IDC_EB_POSX)->ShowWindow(bEventID);
	GetDlgItem(IDC_EB_POSY)->ShowWindow(bEventID);
	GetDlgItem(IDC_EB_POSZ)->ShowWindow(bEventID);

	GetDlgItem(IDC_STATIC_M1)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M2)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M3)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M4)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M5)->ShowWindow(bEventID);
	GetDlgItem(IDC_STATIC_M6)->ShowWindow(bEventID);

	if(!bEnable)
		bEventID = bEnable;

	GetDlgItem(IDC_EB_MONID)->EnableWindow(bEventID);
	GetDlgItem(IDC_EB_MONDELAY)->EnableWindow(bEventID);
	GetDlgItem(IDC_EB_REGENMAPID)->EnableWindow(bEventID);
	GetDlgItem(IDC_EB_POSX)->EnableWindow(bEventID);
	GetDlgItem(IDC_EB_POSY)->EnableWindow(bEventID);
	GetDlgItem(IDC_EB_POSZ)->EnableWindow(bEventID);

	GetDlgItem(IDC_BTN_REGENDEL)->ShowWindow(bEventID);
	GetDlgItem(IDC_BTN_ADDREGEN)->ShowWindow(bEventID);
}

void CEventInfo::SetLotteryCtrl(BYTE bEventID, BYTE bEnable)
{
	if(bEventID == EVENT_LOTTERY || bEventID == EVENT_GIFTTIME)
		bEnable = TRUE;
	else 
		bEnable = FALSE;

	//	그룹
	GetDlgItem(IDC_EVESET)->ShowWindow(!bEnable);
	GetDlgItem(IDC_DATESET)->ShowWindow(!bEnable);
	GetDlgItem(IDC_EVESET2)->ShowWindow(bEnable);

	//	스태틱
	GetDlgItem(IDC_LOT_STATIC1)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC2)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC4)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC6)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC7)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC8)->ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC9)->ShowWindow(bEnable);

	//	기타 컨트롤
	GetDlgItem(IDC_DT_LOT)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTH)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTM)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTITEMID)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTNUM)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTNUM2)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTMAIL)->ShowWindow(bEnable);
	GetDlgItem(IDC_EB_LOTMAIL2)->ShowWindow(bEnable);

	if(bEventID == EVENT_GIFTTIME)	
	{	
		GetDlgItem(IDC_EVESET2)->SetWindowText("GiftTime");
		GetDlgItem(IDC_LOT_STATIC8)->SetWindowText("UseTime");
		bEnable = FALSE;

		//	GIFT::LEVEL
		GetDlgItem(IDC_LOT_STATIC10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LOT_STATIC11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EB_MINL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EB_MAXL)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EVESET2)->SetWindowText("Item Lottery");
		GetDlgItem(IDC_LOT_STATIC8)->SetWindowText("Winner");

		//	GIFT::LEVEL
		GetDlgItem(IDC_LOT_STATIC10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LOT_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_MINL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EB_MAXL)->ShowWindow(SW_HIDE);
	}

	m_cbLotType.ShowWindow(bEnable);
	m_lcLottery.ShowWindow(bEnable);
	GetDlgItem(IDC_LOT_STATIC5)->ShowWindow(bEnable);
	GetDlgItem(IDC_BTN_LOTDEL)->ShowWindow(bEnable);
	GetDlgItem(IDC_BTN_LOTADD)->ShowWindow(bEnable);
	GetDlgItem(IDC_REWSET)->ShowWindow(bEnable);

}

void CEventInfo::SetEventID( DWORD dwEventIndex, BYTE bEventType)
{	
	m_bEventType = bEventType;
	m_dwEventIndex = dwEventIndex;
}

void CEventInfo::OnCbnSelchangeCbEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BYTE bEventID = m_cbEventType.GetCurSel();

	SetViewCtrl(bEventID, 0, TRUE);

	UpdateData(FALSE);
}

//	캐쉬
void CEventInfo::OnBnClickedBtnCashAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCashItemSale;
	CString strCashItem;
	CString strSale;

	m_cbCashItem.GetLBText(m_cbCashItem.GetCurSel(),strCashItem);
	int nValue = GetDlgItemInt(IDC_EB_CASHSALE);
	strSale.Format("%d",nValue);

	strCashItemSale = strCashItem + _T(" - ") +strSale + _T(" %");

	m_lbCashItemList.AddString(strCashItemSale);
	UpdateData(FALSE);
}

void CEventInfo::OnBnClickedBtnCashDelete()
{
	m_lbCashItemList.DeleteString( m_lbCashItemList.GetCurSel() );

	UpdateData(FALSE);
}

//	리젠
void CEventInfo::OnBnClickedBtnAddRegen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strX,strY,strZ;
	INT nMonID = GetDlgItemInt(IDC_EB_MONID);
	INT nDelay = GetDlgItemInt(IDC_EB_MONDELAY);
	INT nMapID = GetDlgItemInt(IDC_EB_REGENMAPID);
	GetDlgItemText(IDC_EB_POSX,strX);
	FLOAT fPosX = (FLOAT)atof(strX);
	GetDlgItemText(IDC_EB_POSY,strY);
	FLOAT fPosY = (FLOAT)atof(strY);
	GetDlgItemText(IDC_EB_POSZ,strZ);
	FLOAT fPosZ = (FLOAT)atof(strZ);

	CString strVal1,strVal2,strVal3,strVal4,strVal5,strVal6,strResult;
	strVal1.Format("%d",nMonID);
	strVal2.Format("%d",nDelay);
	strVal3.Format("%d",nMapID);
	strVal4.Format("%.2f",fPosX);
	strVal5.Format("%.2f",fPosY);
	strVal6.Format("%.2f",fPosZ);

	strResult = strVal1 + _T("-") + strVal2 + _T("-") + strVal3 + _T("-") + strVal4 + _T("-") +strVal5 + _T("-") + strVal6;
	m_lcRegenList.AddString(strResult);
	UpdateData(FALSE);
}

void CEventInfo::OnBnClickedBtnRegenDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lcRegenList.DeleteString( m_lcRegenList.GetCurSel() );
	UpdateData(FALSE);
}

void CEventInfo::OnLbnSelchangeLbRegenlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nPos = 0;
	BYTE bCount = 0;
	CString strTok;
	CString str1;

	m_lcRegenList.GetText(m_lcRegenList.GetCurSel(),str1);		

	strTok = str1.Tokenize(_T("-"),nPos);
	while(strTok != _T(""))
	{
		switch(bCount)
		{
		case 0:	SetDlgItemInt(IDC_EB_MONID, atoi(strTok));		break;
		case 1: SetDlgItemInt(IDC_EB_MONDELAY, atoi(strTok));	break;
		case 2: SetDlgItemInt(IDC_EB_REGENMAPID, atoi(strTok));	break;
		case 3:	SetDlgItemText(IDC_EB_POSX,strTok);				break;
		case 4: SetDlgItemText(IDC_EB_POSY,strTok);				break;
		case 5: SetDlgItemText(IDC_EB_POSZ,strTok);				break;
		}
		bCount++;
		strTok = str1.Tokenize(_T("-"),nPos);
	}
	UpdateData(FALSE);
}

//	스폰
void CEventInfo::OnBnClickedBtnSpawnAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str1;
	
	int nValue = GetDlgItemInt(IDC_EB_SPAWNID);	
	str1.Format("%d",nValue);
	m_lbSpawnIDList.AddString(str1);
	UpdateData(FALSE);
}

void CEventInfo::OnBnClickedBtnSpawnDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_lbSpawnIDList.DeleteString( m_lbSpawnIDList.GetCurSel() );

	UpdateData(FALSE);
}

void CEventInfo::OnBnClickedRadio1()
{	
	CTime t;
	m_dtStart2.GetTime(t);
	m_dtStart.SetTime(&t);
	m_dtEnd2.GetTime(t);
	m_dtEnd.SetTime(&t);

	SetDlgItemInt(IDC_EB_STARTHOUR, GetDlgItemInt(IDC_EB_STARTHOUR2));
	SetDlgItemInt(IDC_EB_STARTMIN, GetDlgItemInt(IDC_EB_STARTMIN2));
	SetDlgItemInt(IDC_EB_ENDHOUR, GetDlgItemInt(IDC_EB_ENDHOUR2));
	SetDlgItemInt(IDC_EB_ENDMIN, GetDlgItemInt(IDC_EB_ENDMIN2));	

	SetPeriodCtrl(0, 0);
}

void CEventInfo::OnBnClickedRadio2()
{
	CTime t;
	m_dtStart.GetTime(t);
	m_dtStart2.SetTime(&t);
	m_dtEnd.GetTime(t);
	m_dtEnd2.SetTime(&t);

	SetDlgItemInt(IDC_EB_STARTHOUR2, GetDlgItemInt(IDC_EB_STARTHOUR));
	SetDlgItemInt(IDC_EB_STARTMIN2, GetDlgItemInt(IDC_EB_STARTMIN));
	SetDlgItemInt(IDC_EB_ENDHOUR2, GetDlgItemInt(IDC_EB_ENDHOUR));
	SetDlgItemInt(IDC_EB_ENDMIN2, GetDlgItemInt(IDC_EB_ENDMIN));	

	SetPeriodCtrl(1, 0);
}

void CEventInfo::OnBnClickedClose()
{
	OnCancel();
}
void CEventInfo::OnBnClickedBtnLotadd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//VWORD	vMapID;

	int iRow = (int)m_lcLottery.GetItemCount();
	if(iRow > 7)
	{
		AfxMessageBox(_T("full Reward (max:7)"));
			
		SetDlgItemText(IDC_EB_LOTITEMID, _T(""));
		SetDlgItemText(IDC_EB_LOTNUM, _T(""));
		SetDlgItemText(IDC_EB_LOTNUM2, _T(""));
		return;			
	}

	TLOTTERY stLOT;
	memset(&stLOT, 0, sizeof(TLOTTERY));

	stLOT.m_wItemID		= GetDlgItemInt(IDC_EB_LOTITEMID);
	stLOT.m_bNum		= GetDlgItemInt(IDC_EB_LOTNUM);
	stLOT.m_wWinner		= GetDlgItemInt(IDC_EB_LOTNUM2);

	if(!stLOT.m_wItemID | !stLOT.m_bNum | !stLOT.m_wWinner)
		return;

	m_lcLottery.InsertItem(iRow, _T(""));
	m_lcLottery.SetItemInt(iRow,0, iRow+1);
	m_lcLottery.SetItemInt(iRow,1,(int)stLOT.m_wItemID);
	m_lcLottery.SetItemInt(iRow,2,(int)stLOT.m_bNum);
	m_lcLottery.SetItemInt(iRow,3,(int)stLOT.m_wWinner);

	SetDlgItemText(IDC_EB_LOTITEMID, _T(""));
	SetDlgItemText(IDC_EB_LOTNUM, _T(""));
	SetDlgItemText(IDC_EB_LOTNUM2, _T(""));

	UpdateData(FALSE);

}

void CEventInfo::OnBnClickedBtnLotdel()
{
	int nCount = m_lcLottery.GetItemCount();
	if(!nCount)
		return;
	m_lcLottery.DeleteItem(m_lcLottery.GetCurSel());
	nCount--;
	for(int i = 0; i <nCount; i++)
		m_lcLottery.SetItemInt(i,0,i+1);

	UpdateData(FALSE);
}
void CEventInfo::OnEnChangeEbRegenmapid()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
