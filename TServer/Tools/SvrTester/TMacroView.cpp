// TMacroView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TMacroDoc.h"

#include "TMacroView.h"
#include ".\tmacroview.h"

#include "MainFrm.h"


// CTMacroView

IMPLEMENT_DYNCREATE(CTMacroView, CFormView)

CTMacroView::CTMacroView()
	: CFormView(CTMacroView::IDD)
{
}

CTMacroView::~CTMacroView()
{
}

void CTMacroView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeProtocol);
	DDX_Control(pDX, IDC_LIST1, m_ListParam);
}

BEGIN_MESSAGE_MAP(CTMacroView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, OnBnClickedButtonCancle)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSIS, OnBnClickedButtonAnalysis)
	ON_BN_CLICKED(IDC_BUTTON_SELFSET, OnBnClickedButtonSelfset)
	ON_BN_CLICKED(IDC_BUTTON_SELFCANCLE, OnBnClickedButtonSelfcancle)
END_MESSAGE_MAP()


// CTMacroView 진단입니다.

#ifdef _DEBUG
void CTMacroView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTMacroView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTMacroView 메시지 처리기입니다.

void CTMacroView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	m_TreeProtocol.InitTreeCtrl(TMACROVIEWTREE, 16, IDI_ICON_TP2, IDI_ICON_TP3, IDI_ICON_TP1);

	// ListCtrl
	LV_COLUMN lvcolumn;//컬럼 설정 구조체 
	int i; 
	TCHAR *list[2] = {_T("Name"), _T("Type")};
	int width[2] = {120, 90}; 
	for(i=0;i<2;i++) 
	{ 
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        lvcolumn.fmt = LVCFMT_LEFT; 
		lvcolumn.pszText = list[i]; 
		lvcolumn.iSubItem = i; 
		lvcolumn.cx = width[i]; 
		m_ListParam.InsertColumn(i, &lvcolumn); 
	}
	m_ListParam.SetExtendedStyle(LVS_EX_GRIDLINES);

	SetDlgItemText(IDC_EDIT1, _T(""));
	SetDlgItemText(IDC_EDIT2, _T("0"));
	SetDlgItemText(IDC_EDIT3, _T(""));
}

//////////////////////////////////////////////////////////////////////////////////
// Serch Protocol
BOOL CTMacroView::SerchProtocol(CString strProtocol)
{
	MAPPROTOCOL::iterator it;
	for(it = m_mapProtocol.begin(); it != m_mapProtocol.end(); it++)
	{
		if((*it).second == strProtocol)
			return TRUE;
	}
	return FALSE;
}
// Get Protocol ID
DWORD CTMacroView::GetProtocolID(CString strProtocol)
{
	MAPPROTOCOL::iterator it;
	for(it = m_mapProtocol.begin(); it != m_mapProtocol.end(); it++)
	{
		if((*it).second == strProtocol)
			return (*it).first;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Sleep Set / Cancle
void CTMacroView::OnBnClickedButtonSet()
{
	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
		return;

	CString str;
	GetDlgItemText(IDC_EDIT2, str);
	int sleep = atoi(str);
	
	if(sleep > 0)
	{
		pMacro->SetPInfoSleep(strProtocol, sleep);
		m_TreeProtocol.ChangeImage(2, 2);
	}
	else
	{
		pMacro->ClearPInfoSleep(strProtocol);
		m_TreeProtocol.ChangeImage(0, 1);
		SetDlgItemText(IDC_EDIT2, _T("0"));
	}	
}
void CTMacroView::OnBnClickedButtonCancle()
{
	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
		return;

	pMacro->ClearPInfoSleep(strProtocol);
	m_TreeProtocol.ChangeImage(0, 1);
	SetDlgItemText(IDC_EDIT2, _T("0"));
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Self Set / Cancle
void CTMacroView::OnBnClickedButtonSelfset()
{
	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);

	if( !strstr(strProtocol, _T("_ACK")) )
	{
		AfxMessageBox(_T("Self 설정을 할 수 없는 프로토콜입니다."));
		return;
	}

	int i = m_ListParam.GetSelectionMark();
	if(i < 0)
	{
		AfxMessageBox(_T("Self로 설정할 파라메터를 선택하세요."));
		return;
	}

	CString sType = m_ListParam.GetItemText(i, 1);
	if( sType == _T("LOOPSTART") || sType == _T("LOOPEND") )
	{
		AfxMessageBox(_T("Self 설정을 할 수 없는 파라메터입니다."));
		return;
	}

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
		return;

	m_ListParam.SetItemText(i, 0, _T("<Self>"));
	
	VECTORPARAM vParam;
	for(int j = 0; j < m_ListParam.GetItemCount(); j++)
	{		
		if( m_ListParam.GetItemText(j, 0) == _T("<Self>") )
		{
			if( j != i )			
				m_ListParam.SetItemText(j, 0, _T("Default"));			
		}

		STRUCTPARAM sParam;
		sParam.strName = m_ListParam.GetItemText(j, 0);
		sParam.strType = m_ListParam.GetItemText(j, 1);
		vParam.push_back(sParam);
	}
	
	pMacro->SetPInfoSelf(strProtocol, TRUE, vParam);
	SetDlgItemText(IDC_EDIT3, _T("TRUE"));
}

void CTMacroView::OnBnClickedButtonSelfcancle()
{
	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);

	if( !strstr(strProtocol, _T("_ACK")) )
	{
		AfxMessageBox(_T("Self 설정을 할 수 없는 프로토콜입니다."));
		return;
	}

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
		return;

	pMacro->ClearPInfoSelf(strProtocol);
	SetDlgItemText(IDC_EDIT3, _T("FALSE"));

	for(int j = 0; j < m_ListParam.GetItemCount(); j++)
	{		
		if( m_ListParam.GetItemText(j, 0) == _T("<Self>") )
		{
			m_ListParam.SetItemText(j, 0, _T("Default"));
			break;
		}		
	}
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Insert Paramater List
void CTMacroView::InsertParamList(CString strName, CString strType)
{
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
}
// Set Paramater List Name
void CTMacroView::SetParamListName(CString strName, CString strType)
{
	int i = m_ListParam.GetSelectionMark();
	if(i < 0)
		return;

	if( strType != m_ListParam.GetItemText(i, 1) )
	{
		AfxMessageBox(_T("자료 타입이 틀립니다."));
		return;
	}
    m_ListParam.SetItemText(i, 0, strName);
}
// Paramater List Clear / Init
void CTMacroView::OnBnClickedButtonClear()
{
	int i = m_ListParam.GetSelectionMark();
	if(i < 0)
		return;
	m_ListParam.SetItemText(i, 0, _T("Default"));
}
void CTMacroView::OnBnClickedButtonInit()
{
	int cnt = m_ListParam.GetItemCount();
	for(int i = 0; i < cnt; i++)
        m_ListParam.SetItemText(i, 0, _T("Default"));
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Setting
void CTMacroView::OnBnClickedButtonSetting()
{
	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);

	DWORD pID = GetProtocolID(strProtocol);

	if( !GetProtocolID(strProtocol) )
	{
		AfxMessageBox(_T("정의된 프로토콜이 아닙니다."));
		return;
	}

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
	{
        AfxMessageBox(_T("매크로를 선택해주세요."));
		return;
	}

	STRUCTPINFO temp = pMacro->GetMacroPInfo(strProtocol);
	STRUCTPINFO pInfo;

	pInfo.dwPID = temp.dwPID;
	pInfo.strName = temp.strName;
	pInfo.strParent = temp.strParent;
	pInfo.bSelf = temp.bSelf;	

	CString str;
	GetDlgItemText(IDC_EDIT2, str);
	int sleep = atoi(str);
	if(sleep > 0)
	{
		pInfo.bSleep = TRUE;
		pInfo.sleep = sleep;
		m_TreeProtocol.ChangeImage(2, 2);
	}
	else
	{
		pInfo.bSleep = FALSE;
		pInfo.sleep = 0;
		m_TreeProtocol.ChangeImage(0, 1);
		SetDlgItemText(IDC_EDIT2, _T("0"));
	}
	
	pInfo.vParam.clear();
	VECTORPARAM vParam;
	STRUCTPARAM sParam;
	for(int i = 0; i < m_ListParam.GetItemCount(); i++)
	{
		sParam.strName = m_ListParam.GetItemText(i, 0);
		sParam.strType = m_ListParam.GetItemText(i, 1);
		vParam.push_back(sParam);
	}
	pInfo.vParam = vParam;

	pMacro->SetMacroPInfo(strProtocol, pInfo);
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Tree
void CTMacroView::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem = m_TreeProtocol.GetSelectedItem();
	if(hItem == NULL)
		return;
	CString strProtocol = m_TreeProtocol.GetItemText(hItem);	

	if( SerchProtocol(strProtocol) )
		SetDlgItemText(IDC_EDIT1, strProtocol);
	else
		SetDlgItemText(IDC_EDIT1, _T("존재하지 않는 프로토콜입니다."));

	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )	
		return;	

	STRUCTPINFO pInfo = pMacro->GetMacroPInfo(strProtocol);

	CString sSleep;
    sSleep.Format(_T("%d"), pInfo.sleep);
	SetDlgItemText(IDC_EDIT2, sSleep);

	if( pInfo.bSelf )	
		SetDlgItemText(IDC_EDIT3, _T("TRUE"));	
	else
		SetDlgItemText(IDC_EDIT3, _T("FALSE"));
	
	m_ListParam.DeleteAllItems();
	VECTORPARAM vParam = pInfo.vParam;			
	for(int i = 0; i < (int)vParam.size(); i++)
	{
		STRUCTPARAM sParam = vParam[i];
		CString strName = sParam.strName;
		CString strType = sParam.strType;
		InsertParamList(strName, strType);
	}	

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info Vector Insert / Delete / Update
BOOL CTMacroView::InsertPInfoVector(CString strName, CString strParent)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
	{
		AfxMessageBox(_T("매크로를 선택해주세요."));
		return FALSE;
	}

	STRUCTPINFO pInfo;	
	pInfo.dwPID = 0;
	pInfo.strName = strName;
	pInfo.strParent = strParent;
	pInfo.bSleep = FALSE;
	pInfo.sleep = 0;
	pInfo.bSelf = FALSE;
	pInfo.vParam.clear();

	pMacro->AddMacroPInfo(pInfo);
	return TRUE;
}
void CTMacroView::DeletePInfoVector(CString strName)
{
    CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();	

	pMacro->DelMacroPInfo(strName);	
}
void CTMacroView::DeleteAllPInfoVector()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();

	pMacro->DelAllMacroPInfo();

	m_TreeProtocol.m_TreeCnt = 0;
}
BOOL CTMacroView::UpdatePInfoVector(CString strOld, CString strNew)
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
		return FALSE;

	DWORD pID = GetProtocolID(strNew);

	if( !pMacro->UpdatePInfoName(strOld, strNew, pID) )
        return FALSE;		

	if( SerchProtocol(strNew) )
	{
		STRUCTPINFO pInfo = pMacro->GetMacroPInfo(strNew);

		SetDlgItemText(IDC_EDIT1, strNew);
		int sleep = pInfo.sleep;
		CString sSleep;
		sSleep.Format(_T("%d"), sleep);
		SetDlgItemText(IDC_EDIT2, sSleep);

		if( pInfo.bSelf )
			SetDlgItemText(IDC_EDIT3, _T("TRUE"));
		else
			SetDlgItemText(IDC_EDIT3, _T("FALSE"));

		m_ListParam.DeleteAllItems();

		VECTORPARAM vParam;
		MAPPROTOCOLTYPE::iterator findP = m_mapType.find(pID);
		VECTORTYPE vType = (*findP).second;
		for(int i = 0; i < (int)vType.size(); i++)
		{
			CString strType;
			switch(vType[i])
			{
			case TYPE_BYTE: 
				strType = _T("BYTE");
				break;
			case TYPE_WORD:
				strType = _T("WORD");
				break;
			case TYPE_DWORD:
				strType = _T("DWORD");
				break;
			case TYPE_FLOAT:
				strType = _T("FLOAT");
				break;
			case TYPE_STRING:
				strType = _T("STRING");
				break;
			case TYPE_SOCKADDR:
				strType = _T("SOCKADDR");
				break;
			case TYPE_LOOPSTART:
				strType = _T("LOOPSTART");
				break;
			case TYPE_LOOPEND:
				strType = _T("LOOPEND");
				break;
			}

			STRUCTPARAM sParam;
			sParam.strName = _T("Default");
			sParam.strType = strType;

			InsertParamList(sParam.strName, strType);	
			vParam.push_back(sParam);
		}
		pMacro->SetPInfoParam(strNew, vParam);
	}
	else
	{
		SetDlgItemText(IDC_EDIT1, _T("존재하지 않는 프로토콜입니다."));
		SetDlgItemText(IDC_EDIT2, _T("0"));
		SetDlgItemText(IDC_EDIT3, _T(""));
		m_ListParam.DeleteAllItems();

		pMacro->ClearPInfoSleep(strNew);
		pMacro->ClearPInfoParam(strNew);
	}	

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Macro Protocol Info View
void CTMacroView::MacroPInfoView(CTMacro* pMacro)
{	
	ClearMacroPInfoView();

	if( pMacro->IsEmptyPInfo() )
		return;

	VECTORPINFO pInfo = pMacro->GetMacroPInfo();

	for(int i = 0; i < (int)pInfo.size(); i++)
	{
		if( pInfo[i].strParent == _T("") )
		{
			HTREEITEM hParent = m_TreeProtocol.InsertTreeItem(pInfo[i].strName, NULL);
			m_TreeProtocol.SelectItem(hParent);
			if(pInfo[i].bSleep)
				m_TreeProtocol.ChangeImage(2, 2);
			MacroPInfoChildView(pInfo[i].strName, hParent, pInfo);
		}
	}

	m_TreeProtocol.SelectItem(m_TreeProtocol.GetRootItem());
}
void CTMacroView::MacroPInfoChildView(CString strName, HTREEITEM hParent, VECTORPINFO pInfo)
{
	for(int i = 0; i < (int)pInfo.size(); i++)
	{
		if( pInfo[i].strParent == strName )
		{
			HTREEITEM hP = m_TreeProtocol.InsertTreeItem(pInfo[i].strName, hParent);
			m_TreeProtocol.SelectItem(hP);
			if(pInfo[i].bSleep)
				m_TreeProtocol.ChangeImage(2, 2);
			MacroPInfoChildView(pInfo[i].strName, hP, pInfo);
		}
	}
}
void CTMacroView::ClearMacroPInfoView()
{
	m_ListParam.DeleteAllItems();
	m_TreeProtocol.DeleteAllItems();
	m_TreeProtocol.m_TreeCnt = 0;

	SetDlgItemText(IDC_EDIT1, _T(""));
	SetDlgItemText(IDC_EDIT2, _T("0"));
	SetDlgItemText(IDC_EDIT3, _T(""));	
}
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Protocol Analysis
void CTMacroView::OnBnClickedButtonAnalysis()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CTMacro* pMacro = pFrm->GetTabGroup()->GetTMacroTabView()->GetCurSelectedMacro();
	if( pMacro == NULL )
	{
		AfxMessageBox(_T("매크로를 선택해주세요."));
		return;
	}

	if( pMacro->ValidCheck(m_mapProtocol) )	
        AfxMessageBox(_T("사용가능한 매크로입니다."));			
	else	
        AfxMessageBox(_T("사용하지 못하는 프로토콜이 있습니다."));		
}
void CTMacroView::AllProtocolAnalysis()
{
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	pFrm->GetTabGroup()->GetTMacroTabView()->AllValidCheck(m_mapProtocol);
}
//////////////////////////////////////////////////////////////////////////////////