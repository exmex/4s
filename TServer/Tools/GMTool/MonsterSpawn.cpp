// MonsterSpawn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "MonsterSpawn.h"

#include "GMToolDoc.h"
#include ".\monsterspawn.h"

// CMonsterSpawnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonsterSpawnDlg, CDialog)
CMonsterSpawnDlg::CMonsterSpawnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterSpawnDlg::IDD, pParent)
	, m_dwMapID(0)
	, m_dwChannel(0)
	, m_dwSpawnID(0)
	, m_dwTriggerID(0)
	, m_dwHostID(0)
	, m_dwRHID(0)
	, m_dwRHType(0)
{
}

CMonsterSpawnDlg::~CMonsterSpawnDlg()
{
}

void CMonsterSpawnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WORLD, m_comboWorld);
	DDX_Control(pDX, IDC_COMBO_EVENT, m_comboAction);
	DDX_Control(pDX, IDC_LIST_MON, m_listMon);
}


BEGIN_MESSAGE_MAP(CMonsterSpawnDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCHMONSTER, OnBnClickedBtnSearchMonster)
	ON_BN_CLICKED(IDC_BTN_ACTION, OnBnClickedBtnAction)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CMonsterSpawnDlg 메시지 처리기입니다.

BOOL CMonsterSpawnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	char *text[MAX_MONSPAWN_STAT_COLUMN] =
		{"MON ID",
		 "HOST ID",
		 "MON STATS",
		 "X POS",
		 "Y POS",
		 "Z POS" };

	int width[MAX_MONSPAWN_STAT_COLUMN] = {80,80,80,50,50,50};

	LV_COLUMN lv;
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_CENTER;
	for(int j=0; j<MAX_MONSPAWN_STAT_COLUMN; j++)
	{
		lv.pszText = text[j];
		lv.iSubItem = j;
		lv.cx = width[j];
		m_listMon.InsertColumn(j, &lv);
	}

	//m_comboAction.AddString(_T("AT_AICOMPLETE"));
	//m_comboAction.AddString(_T("AT_ENTERLB"));
	//m_comboAction.AddString(_T("AT_LEAVELB"));
	//m_comboAction.AddString(_T("AT_ENTERAB"));
	//m_comboAction.AddString(_T("AT_LEAVEAB"));
	//m_comboAction.AddString(_T("AT_DEFEND"));
	//m_comboAction.AddString(_T("AT_DELETE"));
	//m_comboAction.AddString(_T("AT_ENTER"));
	//m_comboAction.AddString(_T("AT_LEAVE"));
	m_comboAction.AddString(_T("AT_DEAD"));
	//m_comboAction.AddString(_T("AT_TIMEOUT"));
	//m_comboAction.AddString(_T("AT_ATHOME"));
	//m_comboAction.AddString(_T("AT_HELP"));

	m_comboAction.SetCurSel(0);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);// 모달리스 대화상자가 윈도우 태스크바에 안 뜨게 하기
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMonsterSpawnDlg::SetWorld()
{
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);

    m_comboWorld.SetCurSel(0);
}

void CMonsterSpawnDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDlgItemText(IDC_EDIT_MAPID, _T("0"));
	SetDlgItemText(IDC_EDIT_SPAWNID, _T("0"));
	SetDlgItemText(IDC_EDIT_TRIGGERID, _T("0"));
	SetDlgItemText(IDC_EDIT_HOSTID, _T("0"));
	SetDlgItemText(IDC_EDIT_RHID, _T("0"));
	SetDlgItemText(IDC_EDIT_RHTYPE, _T("0"));
	m_listMon.DeleteAllItems();
	m_comboAction.SetCurSel(0);

	OnCancel();
}

void CMonsterSpawnDlg::OnBnClickedBtnSearchMonster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	CString str;
	m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;
	BYTE bID = (*itG).second->m_bID;

	m_listMon.DeleteAllItems();

	UpdateData();
	if(!m_dwSpawnID)
		return;

	pDoc->SendCT_MONSPAWNFIND_REQ(bID, (BYTE)m_dwChannel, (WORD)m_dwMapID, (WORD)m_dwSpawnID);

}

void CMonsterSpawnDlg::AddItemData(DWORD dwMonID, DWORD dwHostID, BYTE bStat, float fPosX, float fPosY, float fPosZ)
{
	LV_ITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_listMon.GetItemCount();
	TCHAR szText[50];

	for(int i=0; i< MAX_MONSPAWN_STAT_COLUMN; i++)
	{
		switch(i)
		{
		case 0: itoa(dwMonID, szText, 10); break;
		case 1: itoa(dwHostID, szText, 10); break;
		case 2: itoa(bStat, szText, 10); break;
		case 3: itoa((DWORD)fPosX, szText, 10); break;
		case 4: itoa((DWORD)fPosY, szText, 10); break;
		case 5: itoa((DWORD)fPosZ, szText, 10); break;
		}
		item.iSubItem = i;
		item.pszText = szText;
		if(i==0) m_listMon.InsertItem(&item);
		else m_listMon.SetItem(&item);
	}

	m_listMon.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CMonsterSpawnDlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int i = m_listMon.GetSelectionMark();
	if( i < 0 )
		return;

	DWORD dwMonID = atoi(m_listMon.GetItemText(i, 0));

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	CString str;
	m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;
	BYTE bID = (*itG).second->m_bID;

	// AT_DEAD 외의 기능 막기
	m_comboAction.GetLBText(m_comboAction.GetCurSel(), str);
	if(str != _T("AT_DEAD"))
		return;

	UpdateData();
	BYTE bAction = ACTION_AT_DEAD;//m_comboAction.GetCurSel();
	
	pDoc->SendCT_MONACTION_REQ(bID, (BYTE)m_dwChannel, (WORD)m_dwMapID, dwMonID, bAction, m_dwTriggerID, m_dwHostID, m_dwRHID, (BYTE)m_dwRHType);
}

void CMonsterSpawnDlg::UpdateData()
{
	m_dwMapID = GetDlgItemInt(IDC_EDIT_MAPID);
	m_dwChannel = GetDlgItemInt(IDC_EDIT_CHANNEL);
	m_dwSpawnID = GetDlgItemInt(IDC_EDIT_SPAWNID);
	m_dwTriggerID = GetDlgItemInt(IDC_EDIT_TRIGGERID);
	m_dwHostID = GetDlgItemInt(IDC_EDIT_HOSTID);
	m_dwRHID = GetDlgItemInt(IDC_EDIT_RHID);
	m_dwRHType = GetDlgItemInt(IDC_EDIT_RHTYPE);
}
