// TMacroToolDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TMacroMachine.h"
#include "TMacroToolDlg.h"
#include ".\tmacrotooldlg.h"
#include "TMacroAddDlg.h"
#include "TMacroValueDlg.h"
#include "TMacroProtocolAddDlg.h"

// CTMacroToolDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroToolDlg, CDialog)
CTMacroToolDlg::CTMacroToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroToolDlg::IDD, pParent)
{
	
}

CTMacroToolDlg::~CTMacroToolDlg()
{
	MAPTMACROTEMP::iterator it;
	for(it=m_mapMacro.begin(); it!=m_mapMacro.end(); it++)
		delete (*it).second;

	m_mapMacro.clear();

	MAPTVALUETEMP::iterator itV;
	for(itV=m_mapValue.begin(); itV!=m_mapValue.end(); itV++)
		delete (*itV).second;

	m_mapValue.clear();
}

void CTMacroToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MACRO, m_treeMacro);
	DDX_Control(pDX, IDC_TREE_PROTOCOL, m_treeProtocol);
	DDX_Control(pDX, IDC_LIST_VALUE, m_listValue);
	DDX_Control(pDX, IDC_LIST_PROTOCOL, m_listProtocol);
	DDX_Control(pDX, IDC_COMBO_SELF, m_cbSelf);
	DDX_Control(pDX, IDC_COMBO_PTLOOP, m_cbPTLoop);
	DDX_Control(pDX, IDC_COMBO_VALUE, m_cbValue);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL, m_cbProtocol);
}


BEGIN_MESSAGE_MAP(CTMacroToolDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_VALUEADD, OnBnClickedButtonValueadd)
	ON_BN_CLICKED(IDC_BUTTON_VALUEUPDATE, OnBnClickedButtonValueupdate)
	ON_BN_CLICKED(IDC_BUTTON_VALUEDEL, OnBnClickedButtonValuedel)
	ON_BN_CLICKED(IDC_BUTTON_MACROADD, OnBnClickedButtonMacroadd)
	ON_BN_CLICKED(IDC_BUTTON_PTUPDATE, OnBnClickedButtonPtupdate)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PTADD, OnBnClickedButtonPtadd)
	ON_BN_CLICKED(IDC_BUTTON_MACRODEL, OnBnClickedButtonMacrodel)
	ON_BN_CLICKED(IDC_BUTTON_PTDEL, OnBnClickedButtonPtdel)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_MACRO, OnTvnEndlabeleditTreeMacro)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_PTINIT, OnBnClickedButtonPtinit)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MACRO, OnTvnSelchangedTreeMacro)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PROTOCOL, OnTvnSelchangedTreeProtocol)
	ON_BN_CLICKED(IDC_BUTTON_PTSET, OnBnClickedButtonPtset)
	ON_BN_CLICKED(IDC_BUTTON_PTUPDATEINIT, OnBnClickedButtonPtupdateinit)
END_MESSAGE_MAP()


// CTMacroToolDlg 메시지 처리기입니다.

BOOL CTMacroToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dwID = 0;
	m_mapMacro.clear();

	SetDlgItemText(IDC_STATIC_LOOP, _T(""));

	SetDlgItemText(IDC_EDIT_SLEEP, _T("0"));

	m_cbSelf.AddString(_T("FALSE"));
	m_cbSelf.AddString(_T("TRUE"));
	m_cbSelf.SetCurSel(0);

	m_cbPTLoop.AddString(_T("FALSE"));
	m_cbPTLoop.AddString(_T("TRUE"));
	m_cbPTLoop.SetCurSel(0);

	LV_COLUMN lvcolumn;
	 
	TCHAR *listV[4] = {_T("자료형"), _T("이름"), _T("타입"), _T("값")};
	BYTE widthV[4] = {80, 80, 80, 80};
	for(BYTE i = 0; i < 4; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = listV[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = widthV[i];
		m_listValue.InsertColumn(i, &lvcolumn);
	}
	m_listValue.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	TCHAR *listP[2] = {_T("자료형"), _T("이름")};
	BYTE widthP[2] = {100, 100};
	for(BYTE i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = listP[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = widthP[i];
		m_listProtocol.InsertColumn(i, &lvcolumn);
	}
	m_listProtocol.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	MAPTPROTOCOL::iterator it;
	for(it=m_mapProtocol.begin(); it!=m_mapProtocol.end(); it++)
	{
		DWORD dwID = m_cbProtocol.AddString((*it).second);
		m_cbProtocol.SetItemData(dwID, (*it).first);
	}
	m_cbProtocol.SetCurSel(0);

	GetDlgItem(IDC_EDIT_SLEEP)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SELF)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PTLOOP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTMacroToolDlg::SetBasicProtocol(MAPTPROTOCOL mapProtocol, MAPTPROTOCOLTYPE mapType)
{
	m_mapProtocol = mapProtocol;
	m_mapType = mapType;
}

void CTMacroToolDlg::OnBnClickedButtonMacroadd()
{
	CTMacroAddDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		if(FindMacro(dlg.m_strName))
		{
			AfxMessageBox(_T("이미 추가된 매크로입니다."));
			return;
		}

		CString strName = dlg.m_strName;
		BYTE bLoop = dlg.m_bLoop;

		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = strName.GetBuffer(0);
		HTREEITEM hItem = m_treeMacro.InsertItem(&tvInsert);
		m_treeMacro.SelectItem(hItem);

		LPTMACROTEMP pMacro = new TMACROTEMP;
		pMacro->m_dwID = ++m_dwID;
		pMacro->m_bLoop = bLoop;
		pMacro->m_strName = strName;
		pMacro->m_mapProtocol.clear();	
		pMacro->m_listChildProtocol.clear();
		m_mapMacro.insert(MAPTMACROTEMP::value_type(pMacro->m_dwID, pMacro));

		if(pMacro->m_bLoop)
			SetDlgItemText(IDC_STATIC_LOOP, _T("LOOP"));
		else
			SetDlgItemText(IDC_STATIC_LOOP, _T(""));

		ClearProtocol();
	}
}

void CTMacroToolDlg::OnBnClickedButtonMacrodel()
{
	HTREEITEM hItem = m_treeMacro.GetSelectedItem();
	if(hItem)
	{
		LPTMACROTEMP pMacro = FindMacro(hItem);
		if(pMacro)
		{
			m_mapMacro.erase(pMacro->m_dwID);
			delete pMacro;
		}

		m_treeMacro.DeleteItem(hItem);
	}

	if(m_mapMacro.empty())
		ClearMacro();
}

void CTMacroToolDlg::OnTvnEndlabeleditTreeMacro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	CEdit *pEdit = m_treeMacro.GetEditControl();
	if(pEdit)
	{
		CString str;
		pEdit->GetWindowText(str);

		if(FindMacro(str))
		{
			AfxMessageBox(_T("이미 추가된 매크로입니다."));
			return;
		}

		if(str.GetLength() > 0)
		{
			LPTMACROTEMP pMacro = FindMacro(pTVDispInfo->item.hItem);
			if(pMacro)
				pMacro->m_strName = str;

			m_treeMacro.SetItemText(pTVDispInfo->item.hItem, str);
			if(!m_treeMacro.GetCount())
				ClearMacro();
		}
	}

	*pResult = 0;
}

void CTMacroToolDlg::OnTvnSelchangedTreeMacro(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem = m_treeMacro.GetSelectedItem();
	if(hItem)
	{
        LPTMACROTEMP pMacro = FindMacro(hItem);
		if(pMacro)
			UpdateProtocol(pMacro);
	}

	*pResult = 0;
}

void CTMacroToolDlg::OnBnClickedButtonValueadd()
{
	if(!m_mapMacro.size())
	{
		AfxMessageBox(STR_ERROR_MACRO);
		return;
	}

	CTMacroValueDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		if(FindValue(dlg.m_strName))
		{
			AfxMessageBox(_T("이미 추가된 값입니다."));
			return;
		}

		if(!CheckValueSelf(dlg.m_bType, dlg.m_bValueType, dlg.m_strValue))
		{
			AfxMessageBox(_T("OUTPUT으로 있는 공용값의 이름으로 셀프속성을 줘야합니다.\n자료형도 셀프로 지정할 공용값과 같은 자료형으로 설정해야합니다."));
			return;
		}

		BYTE i = m_listValue.GetItemCount();
		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 0;
		CString str;
		switch(dlg.m_bValueType)
		{
		case TYPE_BYTE:		str = STR_TYPE_BYTE;	 break;
		case TYPE_WORD:		str = STR_TYPE_WORD;	 break;
		case TYPE_DWORD:	str = STR_TYPE_DWORD;	 break;
		case TYPE_FLOAT:	str = STR_TYPE_FLOAT;	 break;
		case TYPE_STRING:	str = STR_TYPE_STRING;	 break;
		case TYPE_INT64:	str = STR_TYPE_INT64;	 break;
		case TYPE_SOCKADDR:	str = STR_TYPE_SOCKADDR; break;
		}
		item.pszText = str.GetBuffer(0);
		m_listValue.InsertItem(&item);
		
		item.iSubItem = 1;
		item.pszText = dlg.m_strName.GetBuffer(0);
		m_listValue.SetItem(&item);

		item.iSubItem = 2;
		switch(dlg.m_bType)
		{
		case TYPE_INPUT:	str = STR_TYPE_INPUT;	break;
		case TYPE_OUTPUT:	str = STR_TYPE_OUTPUT;	break;
		case TYPE_SELF:		str = STR_TYPE_SELF;	break;
		case TYPE_END:		str = STR_TYPE_END;		break;
		}
		item.pszText = str.GetBuffer(0);
		m_listValue.SetItem(&item);

		item.iSubItem = 3;
		item.pszText = dlg.m_strValue.GetBuffer(0);
		m_listValue.SetItem(&item);

		LPTVALUE pValue = new TVALUE;
		pValue->m_dwID = ++m_dwID;
		pValue->m_strName = dlg.m_strName;
		pValue->m_bValueType = dlg.m_bValueType;
		pValue->m_bType = dlg.m_bType;
		pValue->m_strValue = dlg.m_strValue;

		m_mapValue.insert(MAPTVALUETEMP::value_type(pValue->m_dwID, pValue));

		UpdateValue();
	}
}

void CTMacroToolDlg::OnBnClickedButtonValueupdate()
{
	CTMacroValueDlg dlg;

	INT i = m_listValue.GetSelectionMark();
	if( i < 0 )
		return;

	BYTE bValueType = TYPE_BYTE;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_WORD)
		bValueType = TYPE_WORD;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_DWORD)
		bValueType = TYPE_DWORD;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_FLOAT)
		bValueType = TYPE_FLOAT;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_STRING)
		bValueType = TYPE_STRING;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_INT64)
		bValueType = TYPE_INT64;
	if(m_listValue.GetItemText(i, 0) == STR_TYPE_SOCKADDR)
		bValueType = TYPE_SOCKADDR;

	CString strName = m_listValue.GetItemText(i, 1);

	BYTE bType = TYPE_INPUT;
	if(m_listValue.GetItemText(i, 2) == STR_TYPE_OUTPUT)
		bType = TYPE_OUTPUT;
	else if(m_listValue.GetItemText(i, 2) == STR_TYPE_SELF)
		bType = TYPE_SELF;
	else if(m_listValue.GetItemText(i, 2) == STR_TYPE_END)
		bType = TYPE_END;

	CString strValue = m_listValue.GetItemText(i, 3);

	dlg.SetValue(bValueType, bType, strName, strValue);
	if(dlg.DoModal() == IDOK)
	{
		if(strName!=dlg.m_strName &&
			FindValue(dlg.m_strName))
		{
			AfxMessageBox(_T("이미 추가된 값입니다."));
			return;
		}

		if(!CheckValueSelf(dlg.m_bType, dlg.m_bValueType, dlg.m_strValue))
		{
			AfxMessageBox(_T("OUTPUT으로 있는 공용값의 이름으로 셀프속성을 줘야합니다.\n자료형도 셀프로 지정할 공용값과 같은 자료형으로 설정해야합니다."));
			return;
		}

		LPTVALUE pValue = FindValue(m_listValue.GetItemText(i, 1));
		if(pValue)
		{
			pValue->m_bValueType = dlg.m_bValueType;
			pValue->m_strName = dlg.m_strName;
			pValue->m_bType = dlg.m_bType;
			pValue->m_strValue = dlg.m_strValue;
		}

		CString str;
		switch(dlg.m_bValueType)
		{
		case TYPE_BYTE:		str = STR_TYPE_BYTE;	 break;
		case TYPE_WORD:		str = STR_TYPE_WORD;	 break;
		case TYPE_DWORD:	str = STR_TYPE_DWORD;	 break;
		case TYPE_FLOAT:	str = STR_TYPE_FLOAT;	 break;
		case TYPE_STRING:	str = STR_TYPE_STRING;	 break;
		case TYPE_INT64:	str = STR_TYPE_INT64;	 break;
		case TYPE_SOCKADDR:	str = STR_TYPE_SOCKADDR; break;
		}
		m_listValue.SetItemText(i, 0, str.GetBuffer(0));

		m_listValue.SetItemText(i, 1, dlg.m_strName.GetBuffer(0));

		switch(dlg.m_bType)
		{
		case TYPE_INPUT:	str = STR_TYPE_INPUT;	break;
		case TYPE_OUTPUT:	str = STR_TYPE_OUTPUT;	break;
		case TYPE_SELF:		str = STR_TYPE_SELF;	break;
		case TYPE_END:		str = STR_TYPE_END;		break;
		}
		m_listValue.SetItemText(i, 2, str.GetBuffer(0));

		m_listValue.SetItemText(i, 3, dlg.m_strValue.GetBuffer(0));

		UpdateValue();
	}
}

void CTMacroToolDlg::OnBnClickedButtonValuedel()
{
	INT i = m_listValue.GetSelectionMark();
	if(i < 0)
		return;

	LPTVALUE pValue = FindValue(m_listValue.GetItemText(i, 1));
	if(pValue)
	{
		m_mapValue.erase(pValue->m_dwID);
		delete pValue;
	}

	m_listValue.DeleteItem(i);

	UpdateValue();
}

void CTMacroToolDlg::OnBnClickedButtonPtinit()
{
	HTREEITEM hItem = m_treeMacro.GetSelectedItem();
	if(hItem)
	{
		LPTMACROTEMP pMacro = FindMacro(hItem);
		if(pMacro)
		{
			MAPTPROTOCOLTEMP::iterator it;			
			for(it=pMacro->m_mapProtocol.begin(); it!=pMacro->m_mapProtocol.end(); it++)
				delete (*it).second;
			pMacro->m_mapProtocol.clear();

			LISTTPROTOCOLTEMP::iterator itCh;
			for(itCh = pMacro->m_listChildProtocol.begin(); itCh != pMacro->m_listChildProtocol.end(); itCh++)
				delete (*itCh);
			pMacro->m_listChildProtocol.clear();
		}
		ClearProtocol();
	}
}

void CTMacroToolDlg::OnBnClickedButtonPtadd()
{
	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
	if(!hMacroItem)
	{
		AfxMessageBox(STR_ERROR_MACROSEL);
		return;
	}

	LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(!pMacro)
	{
		AfxMessageBox(STR_ERROR_MACROSEL);
		return;
	}

	CTMacroProtocolAddDlg dlg;

	dlg.SetProtocol(m_mapProtocol);

	if(dlg.DoModal() == IDOK)
	{
		if(IsUsedProtocol(dlg.m_dwID))
		{
			AfxMessageBox(_T("이미 추가된 프로토콜입니다."));
			return;
		}

		if(!CheckProtocol(dlg.m_strName, dlg.m_bCheck))
		{
			AfxMessageBox(_T("_REQ : 루트체크 o \n_ACK : 루트체크 x"));
			return;
		}

		DWORD dwID = 0;
		HTREEITEM hSelItem = m_treeProtocol.GetSelectedItem();

		if(!CheckProtocol(hSelItem, dlg.m_bCheck))
		{
			AfxMessageBox(_T("_REQ는 한개의 _ACK를 가지며 _ACK는 자식을 가질 수 없습니다."));
			return;
		}

		TVINSERTSTRUCT tvInsert;		
		if(hSelItem && !dlg.m_bCheck)
		{
			tvInsert.hParent = hSelItem;
			dwID = FindProtocolID(hSelItem);
		}
		else
			tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = dlg.m_strName.GetBuffer(0);
		HTREEITEM hItem = m_treeProtocol.InsertItem(&tvInsert);
		m_treeProtocol.SelectItem(hItem);

		LPTPROTOCOL pProtocol = new TPROTOCOL;
		pProtocol->m_dwID = dlg.m_dwID;
		pProtocol->m_bLoop = dlg.m_bLoop;
		pProtocol->m_bSelf = dlg.m_bSelf;
		pProtocol->m_wSleep = dlg.m_wSleep;
		pProtocol->m_dwParent = dwID;
		MAPTPROTOCOLTYPE::iterator find = m_mapType.find(pProtocol->m_dwID);
		if(find!=m_mapType.end())
		{
			VECTORTYPE vType = (*find).second;
			VTPTVALUE vPTValue;
			for(BYTE i=0; i<vType.size(); i++)
			{
				TPTVALUE tPTValue;
				tPTValue.m_bValueType = vType[i];
				tPTValue.m_dwValueID = 0;
				
				vPTValue.push_back(tPTValue);
			}
			pProtocol->m_vType = vPTValue;
		}
		if(pProtocol->m_dwParent)
		{			
			pMacro->m_listChildProtocol.push_back(pProtocol);
			pProtocol->m_dwIndex = 0;
		}
		else
		{
			pMacro->m_mapProtocol.insert(MAPTPROTOCOLTEMP::value_type(pProtocol->m_dwID, pProtocol));
			pProtocol->m_dwIndex = ++m_dwID;
		}

		m_treeProtocol.SetItemData(hItem, pProtocol->m_dwID);

		ClearProtocolInfo();
		UpdateProtocolInfo(pProtocol);
	}
}

void CTMacroToolDlg::OnBnClickedButtonPtdel()
{
	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
	if(!hMacroItem)
		return;

	LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(!pMacro)
		return;

	HTREEITEM hItem = m_treeProtocol.GetSelectedItem();
	if(hItem)
	{
		if(m_treeProtocol.GetChildItem(hItem))
		{
			AfxMessageBox(_T("하위프로토콜을 먼저 지워주세요."));
			return;
		}

		LPTPROTOCOL pProtocol = FindProtocol(pMacro, hItem);
		if(pProtocol)
		{
			if(pProtocol->m_dwParent)
			{								
				LISTTPROTOCOLTEMP::iterator finder;
				finder = find_if(pMacro->m_listChildProtocol.begin(), pMacro->m_listChildProtocol.end(),CTProtocol_Equal(pProtocol->m_dwID) );
				if( finder != pMacro->m_listChildProtocol.end() )
					pMacro->m_listChildProtocol.erase(finder);
			}
			else
				pMacro->m_mapProtocol.erase(pProtocol->m_dwID);
			delete pProtocol;
		}

		m_treeProtocol.DeleteItem(hItem);
	}

	if(pMacro->m_mapProtocol.empty())
		ClearProtocolInfo();
}

void CTMacroToolDlg::OnBnClickedButtonPtset()
{
	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
	if(!hMacroItem)
		return;

	LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(!pMacro)
		return;

	HTREEITEM hItem = m_treeProtocol.GetSelectedItem();
	if(!hItem)
		return;

	LPTPROTOCOL pOld = FindProtocol(pMacro, hItem);
	LPTPROTOCOL pNew = NULL;

	if(pOld)
	{
		DWORD dwID = DWORD(m_cbProtocol.GetItemData(m_cbProtocol.GetCurSel()));
		if(pOld->m_dwID!=dwID &&
			IsUsedProtocol(dwID))
		{
			AfxMessageBox(_T("이미 추가된 프로토콜입니다."));
			return;
		}

		if(!CheckProtocol(pOld->m_dwID, dwID))
		{
			AfxMessageBox(_T("_REQ / _ACK는 이전 프로토콜을 따라서 정의해주세요."));
			return;
		}

		CString strName;
		m_cbProtocol.GetLBText(m_cbProtocol.GetCurSel(), strName);
		m_treeProtocol.SetItemText(hItem, strName);

		pNew = new TPROTOCOL;
		pNew->m_dwIndex = pOld->m_dwIndex;
		pNew->m_dwID = dwID;
		pNew->m_bLoop = m_cbPTLoop.GetCurSel();
		pNew->m_bSelf = m_cbSelf.GetCurSel();
		CString str;
		GetDlgItemText(IDC_EDIT_SLEEP, str);
		pNew->m_wSleep = atoi(str);
		pNew->m_dwParent = pOld->m_dwParent;
		if(pOld->m_dwParent)
		{        
			LISTTPROTOCOLTEMP::iterator finder;
			finder = find_if(pMacro->m_listChildProtocol.begin(), pMacro->m_listChildProtocol.end(), CTProtocol_Equal(pOld->m_dwID) );
			if( finder != pMacro->m_listChildProtocol.end() )
			{
				pMacro->m_listChildProtocol.erase(finder);
				pMacro->m_listChildProtocol.push_back(pNew);
			}
		}
		else
		{
			pMacro->m_mapProtocol.erase(pOld->m_dwID);
			pMacro->m_mapProtocol.insert(MAPTPROTOCOLTEMP::value_type(pNew->m_dwID, pNew));

			LISTTPROTOCOLTEMP::iterator it;
			for( it = pMacro->m_listChildProtocol.begin(); it != pMacro->m_listChildProtocol.end(); it++)
			{
				if( (*it)->m_dwParent == pOld->m_dwID )
				{
					(*it)->m_dwParent = pNew->m_dwID;
					break;
				}
			}
		}

		if(pOld->m_dwID != dwID)
		{
			ClearProtocolInfo();
			if(pNew)
			{
				MAPTPROTOCOLTYPE::iterator find = m_mapType.find(pNew->m_dwID);
				if(find!=m_mapType.end())
				{
					VECTORTYPE vType = (*find).second;
					VTPTVALUE vPTValue;
					for(BYTE i=0; i<vType.size(); i++)
					{
						TPTVALUE tPTValue;
						tPTValue.m_bValueType = vType[i];
						tPTValue.m_dwValueID = 0;
						
						vPTValue.push_back(tPTValue);
					}
					pNew->m_vType = vPTValue;
				}

				UpdateProtocolInfo(pNew);
			}
		}
		else
			pNew->m_vType = pOld->m_vType;

		delete pOld;
	}
}

void CTMacroToolDlg::OnBnClickedButtonPtupdate()
{
	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
	if(!hMacroItem)
		return;

	LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(!pMacro)
		return;

	HTREEITEM hItem = m_treeProtocol.GetSelectedItem();
	if(!hItem)
		return;

	if(!m_cbValue.GetCount())
		return;

	INT i = m_listProtocol.GetSelectionMark();
	if( i < 0 )
		return;

	DWORD dwValueID = DWORD(m_cbValue.GetItemData(m_cbValue.GetCurSel()));
	MAPTVALUETEMP::iterator find = m_mapValue.find(dwValueID);
	if(find!=m_mapValue.end())
	{
		m_listProtocol.SetItemText(i, 1, (*find).second->m_strName);

		LPTPROTOCOL pProtocol = FindProtocol(pMacro, hItem);
		if(pProtocol)
			pProtocol->m_vType[i].m_dwValueID = dwValueID;
	}
}

void CTMacroToolDlg::OnBnClickedButtonPtupdateinit()
{
	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
	if(!hMacroItem)
		return;

	LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(!pMacro)
		return;

	HTREEITEM hItem = m_treeProtocol.GetSelectedItem();
	if(!hItem)
		return;

	if(!m_cbValue.GetCount())
		return;

	INT i = m_listProtocol.GetSelectionMark();
	if( i < 0 )
		return;

	m_listProtocol.SetItemText(i, 1, _T("NULL"));

	LPTPROTOCOL pProtocol = FindProtocol(pMacro, hItem);
	if(pProtocol)
		pProtocol->m_vType[i].m_dwValueID = 0;
}

void CTMacroToolDlg::OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	ClearProtocolInfo();

	HTREEITEM hMacroItem = m_treeMacro.GetSelectedItem();
    LPTMACROTEMP pMacro = FindMacro(hMacroItem);
	if(pMacro)
	{
		HTREEITEM hItem = m_treeProtocol.GetSelectedItem();
		LPTPROTOCOL pProtocol = FindProtocol(pMacro, hItem);
		if(pProtocol)
			UpdateProtocolInfo(pProtocol);
	}

	*pResult = 0;
}

void CTMacroToolDlg::OnBnClickedButtonClear()
{
	ClearMacro();
}

LPTMACROTEMP CTMacroToolDlg::FindMacro(HTREEITEM hItem)
{
	MAPTMACROTEMP::iterator it;
	for(it=m_mapMacro.begin(); it!=m_mapMacro.end(); it++)
	{
		if((*it).second->m_strName == m_treeMacro.GetItemText(hItem))
			return (*it).second;
	}

	return NULL;
}

LPTMACROTEMP CTMacroToolDlg::FindMacro(CString strName)
{
	MAPTMACROTEMP::iterator it;
	for(it=m_mapMacro.begin(); it!=m_mapMacro.end(); it++)
	{
		if((*it).second->m_strName == strName)
			return (*it).second;
	}

	return NULL;
}

LPTVALUE CTMacroToolDlg::FindValue(CString strName)
{
	MAPTVALUETEMP::iterator it;
	for(it=m_mapValue.begin(); it!=m_mapValue.end(); it++)
	{
		if((*it).second->m_strName == strName)
			return (*it).second;
	}

	return NULL;
}

LPTPROTOCOL CTMacroToolDlg::FindProtocol(LPTMACROTEMP pMacro, HTREEITEM hItem)
{
	DWORD dwID = FindProtocolID(hItem);

	MAPTPROTOCOLTEMP::iterator find;

	if(m_treeMacro.GetParentItem(hItem))
	{
		LISTTPROTOCOLTEMP::iterator it;
		it = find_if(pMacro->m_listChildProtocol.begin(), pMacro->m_listChildProtocol.end(), CTProtocol_Equal(dwID) );
		if( it != pMacro->m_listChildProtocol.end() )
			return (*it);
	}
	else
	{
		find = pMacro->m_mapProtocol.find(dwID);
		if(find!=pMacro->m_mapProtocol.end())
			return (*find).second;
	}

	return NULL;
}

DWORD CTMacroToolDlg::FindProtocolID(HTREEITEM hItem)
{
	CString strName = m_treeProtocol.GetItemText(hItem);

	MAPTPROTOCOL::iterator it;
	for(it=m_mapProtocol.begin(); it!=m_mapProtocol.end(); it++)
	{
		if((*it).second == strName)
			return (*it).first;
	}

	return 0;
}

HTREEITEM CTMacroToolDlg::FindProtocolItem(HTREEITEM hItem, DWORD dwData)
{
	HTREEITEM hitemFind, hItemChile, hItemSibling;
    hitemFind = hItemChile = hItemSibling = NULL;

	if(m_treeProtocol.GetItemData(hItem) == dwData)
		hitemFind = hItem;
    else
    {
		hItemChile = m_treeProtocol.GetChildItem(hItem);
		if(hItemChile)
			hitemFind = FindProtocolItem(hItemChile, dwData);

        hItemSibling = m_treeProtocol.GetNextSiblingItem(hItem);
		if(hitemFind==NULL && hItemSibling)
			hitemFind = FindProtocolItem(hItemSibling, dwData);
    }

    return hitemFind;
}

BOOL CTMacroToolDlg::CheckValueSelf(BYTE bType, BYTE bValueType, CString strValue)
{
	if(bType==TYPE_SELF)
	{
		MAPTVALUETEMP::iterator it;
		for(it=m_mapValue.begin(); it!=m_mapValue.end(); it++)
		{
			if( (*it).second->m_strName == strValue &&
				(*it).second->m_bValueType == bValueType &&
				(*it).second->m_bType == TYPE_OUTPUT)
				return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CTMacroToolDlg::CheckProtocol(CString strName, BYTE bCheck)
{
	if(bCheck && strstr(strName, _T("_REQ")))
			return TRUE;
	else if(!bCheck && strstr(strName, _T("_ACK")))
			return TRUE;

	return FALSE;
}

BOOL CTMacroToolDlg::CheckProtocol(DWORD dwOld, DWORD dwNew)
{
	MAPTPROTOCOL::iterator findOld = m_mapProtocol.find(dwOld);
	if(findOld==m_mapProtocol.end())
		return FALSE;

	MAPTPROTOCOL::iterator findNew = m_mapProtocol.find(dwNew);
	if(findNew==m_mapProtocol.end())
		return FALSE;

	if(strstr((*findOld).second, _T("_REQ")) && strstr((*findNew).second, _T("_REQ")))
		return TRUE;
	else if(strstr((*findOld).second, _T("_ACK")) && strstr((*findNew).second, _T("_ACK")))
		return TRUE;

	return FALSE;
}

BOOL CTMacroToolDlg::CheckProtocol(HTREEITEM hItem, BYTE bCheck)
{
	HTREEITEM hParent = m_treeProtocol.GetParentItem(hItem);
	if(hParent && !bCheck)
		return FALSE;

	//HTREEITEM hChild = m_treeProtocol.GetChildItem(hItem);
	//if(hChild && !bCheck)
	//	return FALSE;

	return TRUE;
}

BYTE CTMacroToolDlg::IsUsedProtocol(DWORD dwID)
{
	MAPTMACROTEMP::iterator it;
	for(it=m_mapMacro.begin(); it!=m_mapMacro.end(); it++)
	{
		LPTMACROTEMP pMacro = (*it).second;

		MAPTPROTOCOLTEMP::iterator find = pMacro->m_mapProtocol.find(dwID);
		if(find!=pMacro->m_mapProtocol.end())
			return TRUE;

		LISTTPROTOCOLTEMP::iterator itCh;
		itCh = find_if(pMacro->m_listChildProtocol.begin(), pMacro->m_listChildProtocol.end(),CTProtocol_Equal(dwID) );
		if( itCh != pMacro->m_listChildProtocol.end() )
			return TRUE;
	}

	return FALSE;
}

VECTORDWORD CTMacroToolDlg::SortProtocol(LPTMACROTEMP pMacro)
{
	VECTORDWORD vIndex;
	VECTORDWORD vID;

	vIndex.clear();
	vID.clear();

	MAPTPROTOCOLTEMP::iterator it;
	for(it=pMacro->m_mapProtocol.begin(); it!=pMacro->m_mapProtocol.end(); it++)
	{
		vIndex.push_back((*it).second->m_dwIndex);
		vID.push_back((*it).second->m_dwID);
	}

	if(!vIndex.size())
		return vID;

	DWORD dwTemp;
	for(DWORD i=0; i<(DWORD)vIndex.size()-1; i++)
	{
		for(DWORD j=i+1; j<(DWORD)vIndex.size(); j++)
		{
			if(vIndex[i] > vIndex[j])
			{
                dwTemp = vIndex[i];
				vIndex[i] = vIndex[j];
				vIndex[j] = dwTemp;

				dwTemp = vID[i];
				vID[i] = vID[j];
				vID[j] = dwTemp;
			}
		}
	}

	return vID;
}

void CTMacroToolDlg::ClearMacro()
{
	m_dwID = 0;

	MAPTVALUETEMP::iterator itV;
	for(itV=m_mapValue.begin(); itV!=m_mapValue.end(); itV++)
		delete (*itV).second;

	MAPTMACROTEMP::iterator itM;
	for(itM=m_mapMacro.begin(); itM!=m_mapMacro.end(); itM++)
		delete (*itM).second;

	m_mapValue.clear();
	m_mapMacro.clear();

	while(m_cbValue.GetCount())
		m_cbValue.DeleteString(0);

	m_treeMacro.DeleteAllItems();
	m_listValue.DeleteAllItems();
	SetDlgItemText(IDC_STATIC_LOOP, _T(""));

	ClearProtocol();
}

void CTMacroToolDlg::ClearProtocol()
{
	m_treeProtocol.DeleteAllItems();
	ClearProtocolInfo();
}

void CTMacroToolDlg::ClearProtocolInfo()
{
	m_listProtocol.DeleteAllItems();

	SetDlgItemText(IDC_EDIT_SLEEP, _T("0"));
	m_cbProtocol.SetCurSel(0);
	m_cbSelf.SetCurSel(0);
	m_cbPTLoop.SetCurSel(0);
}

void CTMacroToolDlg::UpdateMacro()
{
	MAPTVALUETEMP::iterator itV;
	for(itV=m_mapValue.begin(); itV!=m_mapValue.end(); itV++)
	{
		LPTVALUE pValue = (*itV).second;

		BYTE i = m_listValue.GetItemCount();
		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 0;
		CString str;
		switch(pValue->m_bValueType)
		{
		case TYPE_BYTE:		str = STR_TYPE_BYTE;	 break;
		case TYPE_WORD:		str = STR_TYPE_WORD;	 break;
		case TYPE_DWORD:	str = STR_TYPE_DWORD;	 break;
		case TYPE_FLOAT:	str = STR_TYPE_FLOAT;	 break;
		case TYPE_STRING:	str = STR_TYPE_STRING;	 break;
		case TYPE_INT64:	str = STR_TYPE_INT64;	 break;
		case TYPE_SOCKADDR:	str = STR_TYPE_SOCKADDR; break;
		}
		item.pszText = str.GetBuffer(0);
		m_listValue.InsertItem(&item);
		
		item.iSubItem = 1;
		item.pszText = pValue->m_strName.GetBuffer(0);
		m_listValue.SetItem(&item);

		item.iSubItem = 2;
		switch(pValue->m_bType)
		{
		case TYPE_INPUT:	str = STR_TYPE_INPUT;	break;
		case TYPE_OUTPUT:	str = STR_TYPE_OUTPUT;	break;
		case TYPE_SELF:		str = STR_TYPE_SELF;	break;
		case TYPE_END:		str = STR_TYPE_END;		break;
		}
		item.pszText = str.GetBuffer(0);
		m_listValue.SetItem(&item);

		item.iSubItem = 3;
		item.pszText = pValue->m_strValue.GetBuffer(0);
		m_listValue.SetItem(&item);
	}
	UpdateValue();

	MAPTMACROTEMP::iterator it;
	for(it=m_mapMacro.begin(); it!=m_mapMacro.end(); it++)
	{
		LPTMACROTEMP pMacro = (*it).second;

		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		tvInsert.item.pszText = pMacro->m_strName.GetBuffer(0);
		m_treeMacro.InsertItem(&tvInsert);
	}

	m_treeMacro.SelectItem(m_treeMacro.GetRootItem());
}

void CTMacroToolDlg::UpdateValue()
{
	while(m_cbValue.GetCount())
		m_cbValue.DeleteString(0);

	MAPTVALUETEMP::iterator it;
	for(it=m_mapValue.begin(); it!=m_mapValue.end(); it++)
	{
		BYTE index = m_cbValue.AddString((*it).second->m_strName);
		m_cbValue.SetItemData(index, (*it).second->m_dwID);
	}
	m_cbValue.SetCurSel(0);
}

void CTMacroToolDlg::UpdateProtocol(LPTMACROTEMP pMacro)
{
	ClearProtocol();

	if(pMacro->m_bLoop)
		SetDlgItemText(IDC_STATIC_LOOP, _T("LOOP"));
	else
		SetDlgItemText(IDC_STATIC_LOOP, _T(""));

	VECTORDWORD vID = SortProtocol(pMacro);
	for(DWORD i=0; i<vID.size(); i++)
	{
		MAPTPROTOCOLTEMP::iterator findID = pMacro->m_mapProtocol.find(vID[i]);
		if(findID!=pMacro->m_mapProtocol.end())
		{
			LPTPROTOCOL pProtocol = (*findID).second;

			TVINSERTSTRUCT tvInsert;
			tvInsert.hParent = NULL;
			tvInsert.hInsertAfter = NULL;
			tvInsert.item.mask = TVIF_TEXT;
			MAPTPROTOCOL::iterator find = m_mapProtocol.find(pProtocol->m_dwID);
			if(find!=m_mapProtocol.end())
				tvInsert.item.pszText = (*find).second.GetBuffer(0);
			else
				tvInsert.item.pszText = _T("NOT FOUND");
			HTREEITEM hItem = m_treeProtocol.InsertItem(&tvInsert);
			m_treeProtocol.SetItemData(hItem, pProtocol->m_dwID);
		}
	}


	LISTTPROTOCOLTEMP::iterator it;
	for( it = pMacro->m_listChildProtocol.begin(); it != pMacro->m_listChildProtocol.end(); it++ )
	{
		
		LPTPROTOCOL pProtocol = (*it);

		TVINSERTSTRUCT tvInsert;
		tvInsert.hParent = FindProtocolItem(m_treeProtocol.GetRootItem(), pProtocol->m_dwParent);
		tvInsert.hInsertAfter = NULL;
		tvInsert.item.mask = TVIF_TEXT;
		MAPTPROTOCOL::iterator find = m_mapProtocol.find(pProtocol->m_dwID);
		if(find!=m_mapProtocol.end())
			tvInsert.item.pszText = (*find).second.GetBuffer(0);
		else
            tvInsert.item.pszText = _T("NOT FOUND");
		HTREEITEM hItem = m_treeProtocol.InsertItem(&tvInsert);
		m_treeProtocol.SetItemData(hItem, pProtocol->m_dwID);

		m_treeProtocol.Expand(tvInsert.hParent, TVE_EXPAND);
	}

	m_treeProtocol.SelectItem(m_treeProtocol.GetRootItem());
}

void CTMacroToolDlg::UpdateProtocolInfo(LPTPROTOCOL pProtocol)
{
	CString strName;
	MAPTPROTOCOL::iterator find = m_mapProtocol.find(pProtocol->m_dwID);
	if(find!=m_mapProtocol.end())
	{
		strName = (*find).second;
		for(DWORD dwCnt=0; dwCnt<(DWORD)m_cbProtocol.GetCount(); dwCnt++)
		{
			CString strTemp;
			m_cbProtocol.GetLBText(dwCnt, strTemp);
			if(strName == strTemp)
			{
				m_cbProtocol.SetCurSel(dwCnt);
				break;
			}
		}
	}
	
	m_cbSelf.SetCurSel(pProtocol->m_bSelf);
	m_cbPTLoop.SetCurSel(pProtocol->m_bLoop);

	CString str;
	str.Format(_T("%d"), pProtocol->m_wSleep);
	SetDlgItemText(IDC_EDIT_SLEEP, str);

	if(strstr(strName, _T("_REQ")))
	{
		GetDlgItem(IDC_EDIT_SLEEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SELF)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PTLOOP)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SLEEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SELF)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PTLOOP)->EnableWindow(FALSE);
	}

	for(BYTE i=0; i<pProtocol->m_vType.size(); i++)
	{
		BYTE bCnt = m_listProtocol.GetItemCount();
		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem = bCnt;
		item.iSubItem = 0;
		CString str;
		switch(pProtocol->m_vType[i].m_bValueType)
		{
		case TYPE_BYTE:		str = STR_TYPE_BYTE;	 break;
		case TYPE_WORD:		str = STR_TYPE_WORD;	 break;
		case TYPE_DWORD:	str = STR_TYPE_DWORD;	 break;
		case TYPE_FLOAT:	str = STR_TYPE_FLOAT;	 break;
		case TYPE_STRING:	str = STR_TYPE_STRING;	 break;
		case TYPE_INT64:	str = STR_TYPE_INT64;	 break;
		case TYPE_SOCKADDR:	str = STR_TYPE_SOCKADDR; break;
		case TYPE_LOOPSTART: str = STR_TYPE_LOOPSTART; break;
		case TYPE_LOOPEND:	str = STR_TYPE_LOOPEND; break;
		}
		item.pszText = str.GetBuffer(0);
		m_listProtocol.InsertItem(&item);
		
		item.iSubItem = 1;
		MAPTVALUETEMP::iterator find = m_mapValue.find(pProtocol->m_vType[i].m_dwValueID);
		if(find!=m_mapValue.end())
			item.pszText = (*find).second->m_strName.GetBuffer(0);
		else
            item.pszText = _T("NULL");
		m_listProtocol.SetItem(&item);
	}
}

void CTMacroToolDlg::OnBnClickedButtonLoad()
{
	LPCTSTR lpszFilter = _T("mac파일(*.mac)|*.mac|모든파일|*.*||");
	
	CFileDialog dlg(TRUE, _T(".mac"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter);
	
	if(dlg.DoModal() == IDOK)
	{
		ClearMacro();
		CFile file(dlg.GetFileName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void CTMacroToolDlg::OnBnClickedButtonSave()
{
	if(m_mapMacro.size() == 0)
	{
		AfxMessageBox(_T("매크로가 비어있습니다."));
		return;
	}

	LPCTSTR lpszFilter = _T("mac파일(*.mac)|*.mac|모든파일|*.*||");

	CFileDialog dlg(FALSE, _T(".mac"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter);
	if(dlg.DoModal() == IDOK)
	{
		CFile file(dlg.GetFileName(), CFile::modeWrite | CFile::modeCreate);
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void CTMacroToolDlg::OnBnClickedOk()
{
	ClearMacro();
	OnOK();
}

void CTMacroToolDlg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		CString str = STR_TMACROMACHINE;
		ar << str;

		ar << (INT)m_mapValue.size();
		MAPTVALUETEMP::iterator itV;
		for(itV=m_mapValue.begin(); itV!=m_mapValue.end(); itV++)
		{
			LPTVALUE pValue = (*itV).second;
			ar << pValue->m_dwID << pValue->m_strName << pValue->m_bType << pValue->m_bValueType << pValue->m_strValue;
		}

		ar << (INT)m_mapMacro.size();

		MAPTMACROTEMP::iterator itM;
		for(itM=m_mapMacro.begin(); itM!=m_mapMacro.end(); itM++)
		{
			LPTMACROTEMP pMacro = (*itM).second;
			ar << pMacro->m_dwID << pMacro->m_strName << pMacro->m_bLoop;

			ar << (INT)pMacro->m_mapProtocol.size();

			VECTORDWORD vID = SortProtocol((*itM).second);
			for(DWORD i=0; i<vID.size(); i++)
			{
				MAPTPROTOCOLTEMP::iterator find = (*itM).second->m_mapProtocol.find(vID[i]);
				if(find!=(*itM).second->m_mapProtocol.end())
				{
					LPTPROTOCOL pProtocol = (*find).second;
					ar << pProtocol->m_dwIndex << pProtocol->m_dwID << pProtocol->m_dwParent << pProtocol->m_bSelf << pProtocol->m_bLoop << pProtocol->m_wSleep;

					ar << (INT)pProtocol->m_vType.size();
					for(DWORD j=0; j<(DWORD)pProtocol->m_vType.size(); j++)
					{
						TPTVALUE tValue = pProtocol->m_vType[j];
						ar << tValue.m_bValueType << tValue.m_dwValueID;
					}
				}
			}

			ar << (INT)pMacro->m_listChildProtocol.size();

			LISTTPROTOCOLTEMP::iterator itP;
			for(itP = pMacro->m_listChildProtocol.begin(); itP != pMacro->m_listChildProtocol.end(); itP++)
			{
				LPTPROTOCOL pProtocol = (*itP);
				ar << pProtocol->m_dwIndex << pProtocol->m_dwID << pProtocol->m_dwParent << pProtocol->m_bSelf << pProtocol->m_bLoop << pProtocol->m_wSleep;

				ar << (INT)pProtocol->m_vType.size();
				for(BYTE i=0; i<pProtocol->m_vType.size(); i++)
				{
					TPTVALUE tValue = pProtocol->m_vType[i];
					ar << tValue.m_bValueType << tValue.m_dwValueID;
				}
			}
		}
	}
	else
	{	// loading code
		CString str;
		ar >> str;
		if(str != STR_TMACROMACHINE)
			return;

		INT cnt;
		ar >> cnt;

		for(INT i=0; i<cnt; i++)
		{
			LPTVALUE pValue = new TVALUE;
			ar >> pValue->m_dwID >> pValue->m_strName >> pValue->m_bType >> pValue->m_bValueType >> pValue->m_strValue;
			m_mapValue.insert(MAPTVALUETEMP::value_type(pValue->m_dwID, pValue));

			if(m_dwID < pValue->m_dwID)
				m_dwID = pValue->m_dwID;
		}

		ar >> cnt;

		for(i=0; i<cnt; i++)
		{
			LPTMACROTEMP pMacro = new TMACROTEMP;
			ar >> pMacro->m_dwID >> pMacro->m_strName >> pMacro->m_bLoop;

			if(m_dwID < pMacro->m_dwID)
				m_dwID = pMacro->m_dwID;
			
			INT cntPt;
			ar >> cntPt;
			for(INT j=0; j<cntPt; j++)
			{
				LPTPROTOCOL pProtocol = new TPROTOCOL;
				ar >> pProtocol->m_dwIndex >> pProtocol->m_dwID >> pProtocol->m_dwParent >> pProtocol->m_bSelf >> pProtocol->m_bLoop >> pProtocol->m_wSleep;

				if(m_dwID < pProtocol->m_dwIndex)
					m_dwID = pProtocol->m_dwIndex;

				INT cntType;
				ar >> cntType;
				for(INT k=0; k<cntType; k++)
				{
					TPTVALUE tValue;
					ar >> tValue.m_bValueType >> tValue.m_dwValueID;
					pProtocol->m_vType.push_back(tValue);
				}

				pMacro->m_mapProtocol.insert(MAPTPROTOCOLTEMP::value_type(pProtocol->m_dwID, pProtocol));
			}

			ar >> cntPt;
			for(j=0; j<cntPt; j++)
			{
				LPTPROTOCOL pProtocol = new TPROTOCOL;
				ar >> pProtocol->m_dwIndex >> pProtocol->m_dwID >> pProtocol->m_dwParent >> pProtocol->m_bSelf >> pProtocol->m_bLoop >> pProtocol->m_wSleep;

				INT cntType;
				ar >> cntType;
				for(INT k=0; k<cntType; k++)
				{
					TPTVALUE tValue;
					ar >> tValue.m_bValueType >> tValue.m_dwValueID;
					pProtocol->m_vType.push_back(tValue);
				}
				
				pMacro->m_listChildProtocol.push_back(pProtocol);
				pProtocol->m_dwIndex = 0;
			}

			m_mapMacro.insert(MAPTMACROTEMP::value_type(pMacro->m_dwID, pMacro));
		}
		UpdateMacro();
	}
}