// TMacroDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TMacroDlg.h"
#include ".\tmacrodlg.h"

#include "MainFrm.h"


// CTMacroDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroDlg, CDialog)
CTMacroDlg::CTMacroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroDlg::IDD, pParent)
{
}

CTMacroDlg::~CTMacroDlg()
{
}

void CTMacroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListParam);
	DDX_Control(pDX, IDC_COMBO1, m_ComboType);
}


BEGIN_MESSAGE_MAP(CTMacroDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_TOP, OnBnClickedButtonTop)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM, OnBnClickedButtonBottom)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnBnClickedButtonInit)
END_MESSAGE_MAP()


// CTMacroDlg 메시지 처리기입니다.
BOOL CTMacroDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ListCtrl
	LV_COLUMN lvcolumn;//컬럼 설정 구조체 
	int i; 
	TCHAR *list[3] = {_T("Name"), _T("Type"), _T("Data")};
	int width[3] = {80, 60, 80}; 
	for(i = 0; i < 3; i++) 
	{ 
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        lvcolumn.fmt = LVCFMT_LEFT; 
		lvcolumn.pszText = list[i]; 
		lvcolumn.iSubItem = i; 
		lvcolumn.cx = width[i]; 
		m_ListParam.InsertColumn(i, &lvcolumn); 
	} 
	m_ListParam.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// ComboBox Value Type
	CString strType[MAX_VALUE_TYPE] = { _T("BYTE"), _T("WORD"), 
		_T("DWORD"), _T("FLOAT"), _T("STRING"), _T("INT64"), _T("SOCKADDR") };
	for(i = 0; i < MAX_VALUE_TYPE; i++)
		m_ComboType.AddString(strType[i]);
	m_ComboType.SetCurSel(0);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// Enter / Escape block
BOOL CTMacroDlg::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////////////////
// Macro Paramater List Insert / Del / Init
void CTMacroDlg::OnBnClickedButtonInsert()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
	{
		AfxMessageBox(_T("매크로를 선택해주세요."));
		return;
	}

	CString strType;
	int CurSel = m_ComboType.GetCurSel();	
	m_ComboType.GetLBText(CurSel, strType);

	CString strName;
	GetDlgItemText(IDC_EDIT1, strName);
	strName.Trim(_T(" "));
	if(strName == _T(""))
		return;

	CString strData;
	GetDlgItemText(IDC_EDIT2, strData);
	SetDlgItemText(IDC_EDIT2, _T(""));

	// Macro Paramater Add
	STRUCTPARAM sParam;
	sParam.strName = strName;
	sParam.strType = strType;
	sParam.strData = strData;
	if( !pMacro->AddMacroParam(sParam) )
	{
		AfxMessageBox(_T("이미 등록된 파라메터입니다."));
		return;
	}    

	// Macro Paramater List Insert
	int i = m_ListParam.GetItemCount();
	LVITEM item; 
	item.mask = LVIF_TEXT; 
	item.iItem = i; 
	item.iSubItem = 0; 		
	item.pszText = strName.GetBuffer(0); 
	m_ListParam.InsertItem(&item); 	
	
	item.iSubItem = 1; 
	item.pszText = strType.GetBuffer(0);
	m_ListParam.SetItem(&item);

	item.iSubItem = 2; 
	item.pszText = strData.GetBuffer(0);
	m_ListParam.SetItem(&item);
}
void CTMacroDlg::OnBnClickedButtonDel()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
	{
		AfxMessageBox(_T("매크로를 선택해주세요."));
		return;
	}

	int i = m_ListParam.GetSelectionMark();
	if( i < 0 )
		return;

	// Macro Paramater Del
	CString strName = m_ListParam.GetItemText(i, 0);
	CString strType = m_ListParam.GetItemText(i, 1);
	pMacro->DelMacroParam(strName, strType);

	// Macro Paramater List Del
	m_ListParam.DeleteItem(i);
	m_ListParam.SetFocus();	
}
void CTMacroDlg::OnBnClickedButtonInit()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
	{
		AfxMessageBox(_T("매크로를 선택해주세요."));
		return;
	}

	if(m_ListParam.GetItemCount() == 0)
		return;

	m_ListParam.DeleteAllItems();

	// Macro Paramater Init
	pMacro->DelAllMacroParam();
}

// Paramater List Move
void CTMacroDlg::OnBnClickedButtonTop()
{
	int i = m_ListParam.GetSelectionMark();
	if( i <= 0 )
		return;

	CString strName = m_ListParam.GetItemText(i, 0);
	CString strType = m_ListParam.GetItemText(i, 1);
	CString strData = m_ListParam.GetItemText(i, 2);

	m_ListParam.DeleteItem(i);

	LVITEM item;
	item.mask = LVIF_TEXT; 
	item.iItem = 0;
	item.iSubItem = 0; 		
	item.pszText = strName.GetBuffer(0); 
	m_ListParam.InsertItem(&item); 	
	
	item.iSubItem = 1; 
	item.pszText = strType.GetBuffer(0);
	m_ListParam.SetItem(&item);

	item.iSubItem = 2; 
	item.pszText = strData.GetBuffer(0);
	m_ListParam.SetItem(&item);

	m_ListParam.SetSelectionMark(0);
	m_ListParam.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListParam.SetFocus();

	// Paramater Map Set
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
        return;	

	SetMacroParam(pMacro);
}
void CTMacroDlg::OnBnClickedButtonUp()
{
	int i = m_ListParam.GetSelectionMark();
	if( i <= 0 )
		return;

	CString strName = m_ListParam.GetItemText(i-1, 0);
	CString strType = m_ListParam.GetItemText(i-1, 1);
	CString strData = m_ListParam.GetItemText(i-1, 2);

	m_ListParam.SetItemText(i-1, 0, m_ListParam.GetItemText(i, 0));
	m_ListParam.SetItemText(i-1, 1, m_ListParam.GetItemText(i, 1));
	m_ListParam.SetItemText(i-1, 2, m_ListParam.GetItemText(i, 2));

	m_ListParam.SetItemText(i, 0, strName);
	m_ListParam.SetItemText(i, 1, strType);
	m_ListParam.SetItemText(i, 2, strData);

	m_ListParam.SetSelectionMark(i-1);
	m_ListParam.SetItemState(i-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListParam.SetFocus();

	// Paramater Map Set
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
        return;	

	SetMacroParam(pMacro);
}
void CTMacroDlg::OnBnClickedButtonBottom()
{
	int i = m_ListParam.GetSelectionMark();
	if( i < 0 || i+1 == m_ListParam.GetItemCount() )
		return;

	CString strName = m_ListParam.GetItemText(i, 0);
	CString strType = m_ListParam.GetItemText(i, 1);
	CString strData = m_ListParam.GetItemText(i, 2);

	m_ListParam.DeleteItem(i);

	LVITEM item;
	item.mask = LVIF_TEXT; 
	item.iItem = m_ListParam.GetItemCount();
	item.iSubItem = 0; 		
	item.pszText = strName.GetBuffer(0); 
	m_ListParam.InsertItem(&item); 	
	
	item.iSubItem = 1; 
	item.pszText = strType.GetBuffer(0);
	m_ListParam.SetItem(&item);

	item.iSubItem = 2; 
	item.pszText = strData.GetBuffer(0);
	m_ListParam.SetItem(&item);

	m_ListParam.SetSelectionMark(m_ListParam.GetItemCount()-1);
	m_ListParam.SetItemState(m_ListParam.GetItemCount()-1, 
		LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListParam.SetFocus();

	// Paramater Map Set
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
        return;	

	SetMacroParam(pMacro);
}
void CTMacroDlg::OnBnClickedButtonDown()
{
	int i = m_ListParam.GetSelectionMark();
	if( i < 0 || i+1 == m_ListParam.GetItemCount() )
		return;

	CString strName = m_ListParam.GetItemText(i+1, 0);
	CString strType = m_ListParam.GetItemText(i+1, 1);
	CString strData = m_ListParam.GetItemText(i+2, 2);

	m_ListParam.SetItemText(i+1, 0, m_ListParam.GetItemText(i, 0));
	m_ListParam.SetItemText(i+1, 1, m_ListParam.GetItemText(i, 1));
	m_ListParam.SetItemText(i+1, 2, m_ListParam.GetItemText(i, 2));

	m_ListParam.SetItemText(i, 0, strName);
	m_ListParam.SetItemText(i, 1, strType);
	m_ListParam.SetItemText(i, 2, strData);

	m_ListParam.SetSelectionMark(i+1);
	m_ListParam.SetItemState(i+1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListParam.SetFocus();

	// Paramater Map Set
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if(pMacro == NULL)
        return;	

	SetMacroParam(pMacro);
}

// Paramater List DblClk
void CTMacroDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int i = m_ListParam.GetSelectionMark();
	if(i < 0)
		return;	

    CString strName = m_ListParam.GetItemText(i, 0);
	CString strType = m_ListParam.GetItemText(i, 1);

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->GetTMacroView()->SetParamListName(strName, strType);

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Paramater Set
void CTMacroDlg::SetMacroParam(CTMacro* pMacro)
{
	pMacro->DelAllMacroParam();

	STRUCTPARAM sParam;
	for(int i = 0; i < m_ListParam.GetItemCount(); i++)
	{
		sParam.strName = m_ListParam.GetItemText(i, 0);
		sParam.strType = m_ListParam.GetItemText(i, 1);
		sParam.strData = m_ListParam.GetItemText(i, 2);		
		pMacro->AddMacroParam(sParam);
	}
}
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Macro Paramater View
void CTMacroDlg::MacroParamView(CTMacro* pMacro)
{
	ClearMacroParamView();
	
	if( pMacro->IsEmptyParam() )	
		return;	

	VECTORPARAM vParam = pMacro->GetMacroParam();	

	LVITEM item; 
	item.mask = LVIF_TEXT;

	for(int i = 0; i < (int)vParam.size(); i++)
	{
		item.iItem = i;
		item.iSubItem = 0;
		item.pszText = vParam[i].strName.GetBuffer(0);
		m_ListParam.InsertItem(&item);

        item.iSubItem = 1;
		item.pszText = vParam[i].strType.GetBuffer(0);
		m_ListParam.SetItem(&item);

		item.iSubItem = 2;
		item.pszText = vParam[i].strData.GetBuffer(0);
		m_ListParam.SetItem(&item);
	}		
}
void CTMacroDlg::ClearMacroParamView()
{
    SetDlgItemText(IDC_EDIT1, _T(""));
	SetDlgItemText(IDC_EDIT2, _T(""));
	m_ComboType.SetCurSel(0);
	m_ListParam.DeleteAllItems();	
}
//////////////////////////////////////////////////////////////////////////////////////