// TSessionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TSessionDlg.h"
#include ".\tsessiondlg.h"

#include "MainFrm.h"
#include "TMacroLoadDlg.h"

// CTSessionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTSessionDlg, CDialog)
CTSessionDlg::CTSessionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTSessionDlg::IDD, pParent)
{
}

CTSessionDlg::~CTSessionDlg()
{
	delete m_pImageList;
}

void CTSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARINFO, m_listCharInfo);
	DDX_Control(pDX, IDC_TREE1, m_TreeMacro);
	DDX_Control(pDX, IDC_LIST_DISREGARD, m_listDisregard);
}


BEGIN_MESSAGE_MAP(CTSessionDlg, CDialog)
	ON_BN_CLICKED(IDC_CONNECT, OnBnClickedConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MON, OnBnClickedButtonMon)
END_MESSAGE_MAP()


// CTSessionDlg 메시지 처리기입니다.

BOOL CTSessionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_strIP = _T("0.0.0.0");
	m_strPort = _T("0");

	CString strCurDirectroy;
	TCHAR temp[200]={0};
	GetCurrentDirectory(200, temp);
	strCurDirectroy.Format(_T("%s"), temp);
	CString path = strCurDirectroy + _T("\\") + FILE_NAME_CONNECT;
	CFile f;
	if(f.Open(path, CFile::modeRead))
	{
		CString strData;
		TCHAR id[50]={0};

		f.Read(id, 50);
		f.Read(id, 50);
		f.Read(id, 50);

		if(f.Read(id, 50))
			m_strIP = id;

		if(f.Read(id, 50))
			m_strPort = id;

		f.Close();
	}

	SetDlgItemText(IDC_IPADDRESS1, m_strIP);
	SetDlgItemText(IDC_EDIT2, m_strPort);
	SetDlgItemText(IDC_EDIT3, _T("1"));

	LV_COLUMN lvcolumn;//컬럼 설정 구조체 
	int i; 
	TCHAR *list[2] = {_T("CharInfo"), _T("Data")};
	int width[2] = {70, 100}; 
	for( i = 0; i < 2 ;i++ )
	{ 
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        lvcolumn.fmt = LVCFMT_LEFT; 
		lvcolumn.pszText = list[i]; 
		lvcolumn.iSubItem = i;		
		lvcolumn.cx = width[i]; 
		m_listCharInfo.InsertColumn(i, &lvcolumn); 
	}
	m_listCharInfo.SetExtendedStyle(LVS_EX_GRIDLINES);

	TCHAR *cList[MAX_CHARINFO] = { _T("UserID"), _T("CharID"), _T("KEY"), 
		_T("GroupID"), _T("Channel"), _T("SvrID") };
	for( i = 0; i < MAX_CHARINFO; i++ )
	{
		int cnt = m_listCharInfo.GetItemCount();
		LVITEM item; 
		item.mask = LVIF_TEXT; 
		item.iItem = cnt;
		item.iSubItem = 0; 
		item.pszText = cList[i];
		m_listCharInfo.InsertItem(&item); 

		item.iSubItem = 1; 
		item.pszText = _T(""); 
		m_listCharInfo.SetItem(&item);
	}

	m_pImageList= new CImageList;
    m_pImageList->Create(16, 16, ILC_COLOR4 | ILC_MASK, 2, 2); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM1));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM2));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM3));
	m_TreeMacro.SetImageList(m_pImageList, TVSIL_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//////////////////////////////////////////////////////////////////////
// Sever Connect / DisConnect
void CTSessionDlg::OnBnClickedConnect()
{
	GetDlgItemText(IDC_IPADDRESS1, m_strIP);
	GetDlgItemText(IDC_EDIT2, m_strPort);
	GetDlgItemText(IDC_EDIT3, m_strThread);

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	pView->Connect(m_strIP, m_strPort, m_strThread);

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_strSIP = m_strIP;
	pFrm->m_strSPort = m_strPort;
}
void CTSessionDlg::OnBnClickedDisconnect()
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	if( !pView->GetDocument()->m_bManagerConnect )
	{
		AfxMessageBox(_T("관리자 인증이 확인되지 않았습니다."));
		return;
	}
	if( pView->GetDocument()->m_mapSession.size() == 0 )
	{
		AfxMessageBox(_T("현재 열려있는 세션이 없습니다."));
		return;
	}
	pView->DisConnect();
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// CharInfo Set
void CTSessionDlg::SetCharInfo(CString UserID, CString CharID, CString KEY, 
				 CString GroupID, CString Channel, CString SvrID)
{
	m_listCharInfo.SetItemText(CHAR_USERID, 1, UserID);
	m_listCharInfo.SetItemText(CHAR_CHARID, 1, CharID);
	m_listCharInfo.SetItemText(CHAR_KEY, 1, KEY);
	m_listCharInfo.SetItemText(CHAR_GROUPID, 1, GroupID);
	m_listCharInfo.SetItemText(CHAR_CHANNEL, 1, Channel);
	m_listCharInfo.SetItemText(CHAR_SVRID, 1, SvrID);
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Disregard Protocol List
CRect CTSessionDlg::GetListPosition()
{
	CRect listRect;
	GetDlgItem(IDC_LIST_DISREGARD)->GetWindowRect(&listRect);	
	return listRect;
}
void CTSessionDlg::SetDisregardList(CString str)
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	if( pView->GetDocument()->m_mapSession.size() != 0 )
	{
		AfxMessageBox(_T("현재 세션이 열려있습니다."));
		return;
	}
	if( strstr(str, _T("_REQ")) )
	{
		AfxMessageBox(_T("추가 할 수 없는 프로토콜입니다."));
		return;
	}	
	for(int i = 0; i < m_listDisregard.GetCount(); i++)
	{
		CString strP;
		m_listDisregard.GetText(i, strP);
		if( str == strP )
		{
			AfxMessageBox("이미 같은 프로토콜이 존재합니다.");
			return;
		}
	}
	m_listDisregard.AddString(str);
}
BOOL CTSessionDlg::GetDisregardList(CString strP)
{	
	for(int i = 0; i < m_listDisregard.GetCount(); i++)
	{
		CString str;
		m_listDisregard.GetText(i, str);
		if( strP == str )
			return TRUE;
	}
	return FALSE;
}
void CTSessionDlg::ClearDisregardList()
{
	while( m_listDisregard.GetCount() )
		m_listDisregard.DeleteString(0);
}
void CTSessionDlg::OnBnClickedButtonInit()
{
	ClearDisregardList();
}
void CTSessionDlg::OnBnClickedButtonDel()
{
	int i = m_listDisregard.GetCurSel();
	m_listDisregard.DeleteString(i);
}
void CTSessionDlg::OnBnClickedButtonMon()
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	if( !pView->GetDocument()->m_bManagerConnect )
	{
		AfxMessageBox(_T("관리자 인증이 확인되지 않았습니다."));
		return;
	}
	if( pView->GetDocument()->m_mapSession.size() != 0 )
	{
		AfxMessageBox(_T("현재 세션이 열려있습니다."));
		return;
	}

	SetDisregardList(_T("CS_ADDMON_ACK"));
	SetDisregardList(_T("CS_DELMON_ACK"));
	SetDisregardList(_T("CS_ENTER_ACK"));
	SetDisregardList(_T("CS_MONACTION_ACK"));
	SetDisregardList(_T("CS_MONHOST_ACK"));
	SetDisregardList(_T("CS_MONMOVE_ACK"));
    SetDisregardList(_T("CS_MONSETSKILL_ACK"));
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Macro Tree Clear
void CTSessionDlg::OnBnClickedButtonClear()
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	if( pView->GetDocument()->m_mapSession.size() )
	{
		AfxMessageBox(_T("현재 세션이 열려있습니다."));
		return;
	}

	ClearMacroTree();
}
void CTSessionDlg::ClearMacroTree()
{
	m_TreeMacro.DeleteAllItems();

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	pView->GetDocument()->SetMacroFlag(FALSE);
	pView->GetDocument()->ClearSMacroMap();
	pView->GetDocument()->ClearSMacroQueue();
}
// Macro Tree Load
HTREEITEM CTSessionDlg::InsertTreeItem(CString strName, HTREEITEM hTreeItem)
{
    TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hTreeItem;
    tvInsert.hInsertAfter = NULL;
	tvInsert.item.iImage = 1; 
	tvInsert.item.iSelectedImage = 1; 
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT; 	
	tvInsert.item.pszText = strName.GetBuffer(0);
    return m_TreeMacro.InsertItem(&tvInsert);
}
void CTSessionDlg::OnBnClickedButtonLoad()
{
    CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	if( !pView->GetDocument()->m_bManagerConnect )
	{
		AfxMessageBox(_T("관리자 인증이 확인되지 않았습니다."));
		return;
	}

	if( pView->GetDocument()->m_mapSession.size() )
	{
		AfxMessageBox(_T("현재 세션이 열려있습니다."));
		return;
	}

	CTMacroLoadDlg dlg;

	if( dlg.DoModal() == IDOK )
	{
		VECTORDWORD vMID = dlg.vMID;
		if( vMID.size() == 0 )
		{
			AfxMessageBox(_T("매크로가 비어있습니다."));
			return;
		}

		m_TreeMacro.DeleteAllItems();
		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
		MAPMACRO mMacro = pFrm->GetTabGroup()->GetTMacroTabView()->m_mapMacro;
		if(mMacro.size() == 0)
			return;

		CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
		pApp->GetTSessionView()->GetDocument()->ClearSMacroMap();
		pView->GetDocument()->ClearSMacroQueue();

		for(int i = 0 ; i < (int)vMID.size(); i++)
		{
			MAPMACRO::iterator find = mMacro.find(vMID[i]);
			DWORD mID = (*find).first;
			CTMacro* pMacro = (*find).second;

			CString strName = pMacro->GetMacroName();

			HTREEITEM hItem = InsertTreeItem(strName, NULL);
			m_TreeMacro.SetItemData(hItem, mID);			
			if( pMacro->GetMacroLoop() )
				m_TreeMacro.SetItemImage(hItem, 2, 2);

			pMacro->SetSendRecvInfo();			
			pApp->GetTSessionView()->GetDocument()->InsertSMacroMap(mID, pMacro);			

			InsertChildItem(strName, hItem);		
		}

		m_TreeMacro.SelectItem(m_TreeMacro.GetRootItem());

		pView->GetDocument()->SetMacroFlag(TRUE);
	}	
}
void CTSessionDlg::InsertChildItem(CString strName, HTREEITEM hParent)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	MAPMACRO mMacro = pFrm->GetTabGroup()->GetTMacroTabView()->m_mapMacro;

	MAPMACRO::iterator it;
	for(it = mMacro.begin(); it != mMacro.end(); it++)
	{
		DWORD mID = (*it).first;
		CTMacro* pMacro = (*it).second;

		CString sName = pMacro->GetMacroName();
		CString sParent = pMacro->GetMacroParent();
		if( sParent == strName )
		{
			HTREEITEM hP = InsertTreeItem(sName, hParent);
			m_TreeMacro.SetItemData(hP, mID);
			if( pMacro->GetMacroLoop() )
				m_TreeMacro.SetItemImage(hP, 2, 2);

			pMacro->SetSendRecvInfo();
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			pApp->GetTSessionView()->GetDocument()->InsertSMacroMap(mID, pMacro);

			InsertChildItem(sName, hP);
		}
	}
}
//////////////////////////////////////////////////////////////////////

// Enter / Escape block
BOOL CTSessionDlg::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}