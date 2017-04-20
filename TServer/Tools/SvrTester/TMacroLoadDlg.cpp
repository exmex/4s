// TMacroLoadDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TMacroLoadDlg.h"
#include ".\tmacroloaddlg.h"

#include "MainFrm.h"

// CTMacroLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroLoadDlg, CDialog)
CTMacroLoadDlg::CTMacroLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroLoadDlg::IDD, pParent)
{
	vMID.clear();
}

CTMacroLoadDlg::~CTMacroLoadDlg()
{
	delete m_pImageList;
	vMID.clear();
}

void CTMacroLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
}


BEGIN_MESSAGE_MAP(CTMacroLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnNMDblclkTree1)
	ON_LBN_DBLCLK(IDC_LIST1, OnLbnDblclkList1)
END_MESSAGE_MAP()


// CTMacroLoadDlg 메시지 처리기입니다.

BOOL CTMacroLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pImageList= new CImageList;
    m_pImageList->Create(16, 16, ILC_COLOR4 | ILC_MASK, 2, 2); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM1));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM2));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_TM3));
	m_tree.SetImageList(m_pImageList, TVSIL_NORMAL);

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->GetTMacroView()->AllProtocolAnalysis();

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	MAPMACRO mMacro = pFrm->GetTabGroup()->GetTMacroTabView()->m_mapMacro;

	MAPMACRO::iterator it;
	for(it = mMacro.begin(); it != mMacro.end(); it++)
	{
		DWORD mID = (*it).first;
		CTMacro* pMacro = (*it).second;
		if( !pMacro->GetMacroEnable() )
			continue;

		BOOL bLoop = pMacro->GetMacroLoop();

		CString strName = pMacro->GetMacroName();
		CString strParent = pMacro->GetMacroParent();	

		if( strParent == _T("") )
		{
			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = NULL;
			if( bLoop )
			{
                tvInsert.item.iImage = 2; 
				tvInsert.item.iSelectedImage = 2;
			}
			else
			{
				tvInsert.item.iImage = 1; 
				tvInsert.item.iSelectedImage = 1; 
			}
			tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT; 	
			tvInsert.item.pszText = strName.GetBuffer(0);
			m_tree.InsertItem(&tvInsert);			
		}
	}

	m_tree.SelectItem(m_tree.GetRootItem());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

///////////////////////////////////////////////////////////////////////
// Macro Insert / Delete
void CTMacroLoadDlg::InsertList()
{
	HTREEITEM hItem = m_tree.GetSelectedItem();
	CString str = m_tree.GetItemText(hItem);

	for(int i = 0; i < m_listbox.GetCount(); i++)
	{
		CString temp;
		m_listbox.GetText(i, temp);
		if( temp == str )
			return;
	}

	m_listbox.AddString(str);
}
void CTMacroLoadDlg::OnBnClickedButtonInsert()
{
    InsertList();
}
void CTMacroLoadDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	InsertList();
	*pResult = 0;
}

void CTMacroLoadDlg::DeleteList()
{
	int pos = m_listbox.GetCurSel();
	m_listbox.DeleteString(pos);
}
void CTMacroLoadDlg::OnBnClickedButtonDelete()
{
	DeleteList();	
}
void CTMacroLoadDlg::OnLbnDblclkList1()
{
	DeleteList();
}
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// IDOK
void CTMacroLoadDlg::OnBnClickedOk()
{		
	for(int i = 0; i < m_listbox.GetCount(); i++)
	{
		CString str;
		m_listbox.GetText(i, str);		

		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
		MAPMACRO mMacro = pFrm->GetTabGroup()->GetTMacroTabView()->m_mapMacro;

		MAPMACRO::iterator it;
		for(it = mMacro.begin(); it != mMacro.end(); it++)
		{
			DWORD mID = (*it).first;
			CTMacro* pMacro = (*it).second;

			if( pMacro->GetMacroName() == str )
			{
				vMID.push_back(mID);
				break;
			}
		}		
	}
	OnOK();
}
///////////////////////////////////////////////////////////////////////