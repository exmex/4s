// CastleSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "MainFrm.h"
#include "HappyDoc.h"
#include "CastleSettingDlg.h"
#include ".\castlesettingdlg.h"


// CCastleSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCastleSettingDlg, CDialog)
CCastleSettingDlg::CCastleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCastleSettingDlg::IDD, pParent)
{
}

CCastleSettingDlg::~CCastleSettingDlg()
{
}

void CCastleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_CASTLE, m_lcCastleInfo);
	DDX_Control(pDX, IDC_CB_CASTLEID, m_cbCastleID);	
	DDX_Control(pDX, IDC_CB_WORLD, m_comboWorld);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_datetimepicker);
}


BEGIN_MESSAGE_MAP(CCastleSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CASTLEINFO, OnBnClickedBtnCastleinfo)
	ON_BN_CLICKED(IDC_BTN_CHGGUILD, OnBnClickedBtnChgGuild)
	ON_BN_CLICKED(IDC_BTN_CHGSTATUS, OnBnClickedBtnChgStatus)
END_MESSAGE_MAP()


// CCastleSettingDlg 메시지 처리기입니다.

void CCastleSettingDlg::Init()
{
	m_mapCastleInfo.clear();
	m_cbCastleID.AddString(_T("Choice"));
	
	ClearListItem();
	ClearCastleIDCombo();
	ClearCastleState();

	SetDlgItemText(IDC_ED_DEFGUILD,_T(""));
	SetDlgItemText(IDC_EB_ATKGUILD,_T(""));
	SetDlgItemText(IDC_EB_C_SECOND,_T(""));
	SetEnableCastleInfoGroup(FALSE);
	SetEnableCastleStatusGroup(FALSE);
	
	UpdateData();
}

void CCastleSettingDlg::InitCastleInfoListControl()
{
	// 리스트 컨트롤의 모든 컬럼 삭제
	for(int i = 0; i < MAX_CASTLELIST_COLUMN ; i++)
        m_lcCastleInfo.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_CASTLELIST_COLUMN]={"ID", "Name", "State", "Defend Guild(ID)","Attack Guild(ID)","Next War Time"};
	int nWidth[MAX_CASTLELIST_COLUMN]={30,100,55,150,150,100};
	
	for(int i = 0; i< MAX_CASTLELIST_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_LEFT;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcCastleInfo.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// 하나의 라인 전체가 선택되도록 설정
	m_lcCastleInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CCastleSettingDlg::InsertItemToListCtr(CASTLEINFO stCASTLEINFO, int _iRow)
{

	CString strTmp;
	LV_ITEM lvItem;
	CTime t(stCASTLEINFO.m_tNextWar);
	int iRowCount = _iRow;

	if(iRowCount == -1 )
		iRowCount = m_lcCastleInfo.GetItemCount();
	
	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= iRowCount; // Row 인덱스
	
	for(int iCol = 0; iCol < MAX_CASTLELIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%d",stCASTLEINFO.m_wCastleID);	break;
		case 1 : strTmp.Format("%s",stCASTLEINFO.m_strName );	break;
		case 2 : 
			{
				switch(stCASTLEINFO.m_bState)
				{
				case 0: strTmp.Format("%s",_T("NORMAL") );	break;
				case 1: strTmp.Format("%s",_T("BATTLE") );	break;
				case 4: strTmp.Format("%s",_T("PEACE") );	break;
				default:strTmp.Format("%s",_T("UNKNOWN") );	break;
				}
			}
			break;
		case 3 : strTmp.Format("%s (%d)",stCASTLEINFO.m_strDefGuild,stCASTLEINFO.m_dwDefGuildID );	break;
		case 4 : strTmp.Format("%s (%d)",stCASTLEINFO.m_strAtkGuild,stCASTLEINFO.m_dwAtkGuildID );	break;
		case 5 : strTmp.Format("%d-%d-%d %d:%d",t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() ); break;
		}
		
		lvItem.iSubItem = iCol ; // 컬럼 인덱스
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_lcCastleInfo.InsertItem(&lvItem);
		else
            m_lcCastleInfo.SetItem(&lvItem);
	}

	MAPCASTLEINFO::iterator itM = m_mapCastleInfo.find(stCASTLEINFO.m_wCastleID);
	if(itM != m_mapCastleInfo.end())
		m_mapCastleInfo.erase(itM);

	m_mapCastleInfo.insert(MAPCASTLEINFO::value_type(stCASTLEINFO.m_wCastleID,stCASTLEINFO) );

	//SetCastleState(stCASTLEINFO.m_bState);

	
	if(stCASTLEINFO.m_bState == 0)
		SetEnableCastleInfoGroup(TRUE);
	else
		SetEnableCastleInfoGroup(FALSE);

	//CTime curtime = CTime::GetCurrentTime();
	//if(stCASTLEINFO.m_bState == 0 && curtime > stCASTLEINFO.m_tNextWar )
	//	SetEnableCastleStatusGroup(FALSE);
	//else 
	//	SetEnableCastleStatusGroup(TRUE);
	SetEnableCastleStatusGroup(TRUE);
}

void CCastleSettingDlg::SetCastleState()
{
	ClearCastleState();

	int bLowState = BS_NOBATTLE;
	int iCount = m_lcCastleInfo.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{			
			CString str = _T("");
			str = m_lcCastleInfo.GetItemText(i,2);	

			BYTE bStatus;
			if(str == _T("NORMAL"))
				bStatus = BS_NORMAL;
			else if(str == _T("BATTLE"))
				bStatus = BS_BATTLE;
			else if(str == _T("PEACE"))
				bStatus = BS_PEACE;

			if(bLowState > bStatus )
				bLowState = bStatus;
	}

	switch(bLowState)
	{
	case BS_NORMAL:	SetDlgItemText(IDC_EB_STATUS,_T("BATTLE"));	break;
	case BS_BATTLE:	SetDlgItemText(IDC_EB_STATUS,_T("PEACE"));	break;
	case BS_PEACE:	SetDlgItemText(IDC_EB_STATUS,_T("NORMAL"));	break;
	default:SetDlgItemText(IDC_EB_STATUS,_T("UNKNOWN"));		break;
	}
}

void CCastleSettingDlg::ClearListItem()
{
	int iCount = m_lcCastleInfo.GetItemCount();
	for(int i = 0; i < iCount; i++)	
		m_lcCastleInfo.DeleteItem(i);	
    
	m_lcCastleInfo.DeleteAllItems();
}

void CCastleSettingDlg::OnBnClickedBtnCastleinfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	ClearListItem();

	CString str;
	m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);
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

	pDoc->SendCT_CASTLEINFO_REQ(m_bSelectWorld);

	ClearCastleIDCombo();
}

void CCastleSettingDlg::ClearCastleIDCombo()
{
	while(m_cbCastleID.GetCount())
		m_cbCastleID.DeleteString(0);

	m_cbCastleID.AddString(_T("Choice"));
	m_cbCastleID.SetCurSel(0);
}

void CCastleSettingDlg::OnBnClickedBtnChgGuild()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	CString str;
	m_cbCastleID.GetLBText(m_cbCastleID.GetCurSel(), str);

	if(str == _T(""))
	{
		AfxMessageBox(_T("Castle Not Selected"),MB_OK);
		return;
	}

	WORD wCastleID = (WORD)atoi(str.GetBuffer(0));

	if(wCastleID == 0)
	{
		AfxMessageBox(_T("Castle Not Selected"),MB_OK);
		return;	
	}

	DWORD dwDefGuildID = (DWORD)GetDlgItemInt(IDC_ED_DEFGUILD);
	DWORD dwAtkGuildID = (DWORD)GetDlgItemInt(IDC_EB_ATKGUILD);

	if(!dwDefGuildID  || !dwAtkGuildID )
	{
		AfxMessageBox(_T("Invalid Guild ID"),MB_OK);
		return;
	}

	if(AfxMessageBox(_T("Are you Sure you want to change?"),MB_YESNO) == IDNO)
		return;

	
	CTime time;
	m_datetimepicker.GetTime(time);
	int iYear = time.GetYear();
	int iMonth = time.GetMonth();
	int iDay = time.GetDay();
	
	int iHour = (int)GetDlgItemInt(IDC_EB_HOUR);
	int iMinute = (int)GetDlgItemInt(IDC_EB_MINUTE);

	if(!iMonth || iMonth < 1 || iMonth > 12 || iDay > 31 || iDay < 1 || iHour > 24 || iMinute > 60 )
	{
		AfxMessageBox(_T("Invalid Data   "),MB_OK);
		return;
	}

	//CTime t = CTime::GetCurrentTime();
	// iYear = t.GetYear();
	//BYTE bYear = tNowTime.GetYear();
	//__time64_t timeCurrent = CTime::GetCurrentTime().GetTime();

	CTime tNextWarTime(iYear,iMonth, iDay, iHour, iMinute, 0);
	
	pDoc->SendCT_CASTLEGUILDCHG_REQ(m_bSelectWorld,wCastleID,dwDefGuildID,dwAtkGuildID,tNextWarTime.GetTime());
}

void CCastleSettingDlg::InsertCastleID(WORD wID)
{
	CString str;
	str.Format("%d",wID);

	m_cbCastleID.AddString(str);
}

void CCastleSettingDlg::OnBnClickedBtnChgStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	DWORD dwMinute = (DWORD)GetDlgItemInt(IDC_EB_C_SECOND);
	DWORD dwSecond = dwMinute * 60;

	if(dwSecond == 0 )
	{
		AfxMessageBox(_T("Enter the minute"));
		return;
	}

	BYTE bStatus;
	CString str;
	GetDlgItemText(IDC_EB_STATUS,str);

	if(str == _T("NORMAL"))
		bStatus = BS_NORMAL;
	else if(str == _T("BATTLE"))
		bStatus = BS_BATTLE;
	else if(str == _T("PEACE"))
		bStatus = BS_PEACE;
	else
	{
		AfxMessageBox(_T("Invalid Status"),MB_OK);
		return;
	}

	pDoc->SendCT_CASTLEENABLE_REQ(m_bSelectWorld,bStatus,dwSecond);

	ClearCastleState();
	
	OnBnClickedBtnCastleinfo();
}

void CCastleSettingDlg::ClearCastleState()
{
	SetDlgItemText(IDC_EB_STATUS,_T(""));
}

void CCastleSettingDlg::SetWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Choice Server"));
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)	
		if((*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);
	
    m_comboWorld.SetCurSel(0);
}

void CCastleSettingDlg::ChangeCastleInfo(WORD wCastleID,DWORD dwDefGuildID,CString strDefGuild,DWORD dwAtkGuildID,CString strAtkGuild,__time64_t tTime)
{
	CString str;
	int iCount = m_lcCastleInfo.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{		
		str = m_lcCastleInfo.GetItemText(i,0);		
		if(wCastleID == atoi(str) )
		{
			str.Format(_T("%s (%d)"),strDefGuild,dwDefGuildID);
			m_lcCastleInfo.SetItemText(i,3,str);

			str.Format(_T("%s (%d)"),strAtkGuild,dwAtkGuildID);
			m_lcCastleInfo.SetItemText(i,4,str);

			CTime t(tTime);
			str.Format("%d-%d-%d %d:%d",t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() );

			m_lcCastleInfo.SetItemText(i,5,str);
		}		
	}
	
	//CTime curtime = CTime::GetCurrentTime();
	//if(curtime > tTime )
	//	SetEnableCastleStatusGroup(FALSE);
	//else
	//	SetEnableCastleStatusGroup(TRUE);

	UpdateData(TRUE);
}

void CCastleSettingDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialog::OnOK();
}

void CCastleSettingDlg::SetEnableCastleInfoGroup(BYTE bEnable)
{
	GetDlgItem(IDC_CB_CASTLEID)->EnableWindow(bEnable);
	GetDlgItem(IDC_ED_DEFGUILD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_ATKGUILD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CHGGUILD)->EnableWindow(bEnable);	
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_HOUR)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_MINUTE)->EnableWindow(bEnable);
}

void CCastleSettingDlg::SetEnableCastleStatusGroup(BYTE bEnable)
{
	GetDlgItem(IDC_EB_STATUS)->EnableWindow(bEnable);
	GetDlgItem(IDC_EB_C_SECOND)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_CHGSTATUS)->EnableWindow(bEnable);
}

