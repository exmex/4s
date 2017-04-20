// PreVersion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "PreVersion.h"
#include ".\preversion.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CPreVersion 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPreVersion, CDialog)
CPreVersion::CPreVersion(CWnd* pParent /*=NULL*/)
	: CDialog(CPreVersion::IDD, pParent)
{
	m_bSelectList = 0;
}

CPreVersion::~CPreVersion()
{
}

void CPreVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_TVERSION,	m_lcVersion);
	DDX_Control(pDX, IDC_LC_TPREVERSION,m_lcPreVersion);
	DDX_Control(pDX, IDC_BTN_TVERMOVE, m_btnMove);
	DDX_Control(pDX, IDC_BTN_PREDELETE, m_btnDel);
}


BEGIN_MESSAGE_MAP(CPreVersion, CDialog)
	ON_BN_CLICKED(IDC_BTN_TVERMOVE, OnBnClickedBtnTvermove)
	ON_BN_CLICKED(IDC_BTN_PREDELETE, OnBnClickedBtnPredelete)
	ON_BN_CLICKED(ID_BTN_PREUPDATE, OnBnClickedBtnPreupdate)
	ON_NOTIFY(NM_CLICK, IDC_LC_TPREVERSION, OnNMClickLcTpreversion)
	ON_NOTIFY(NM_CLICK, IDC_LC_TVERSION, OnNMClickLcTversion)
END_MESSAGE_MAP()


// CPreVersion 메시지 처리기입니다.

void CPreVersion::Init()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	InitList();

	ShowWindow(SW_SHOW);
	pDoc->SendCT_PREVERSIONTABLE_REQ();
}
void CPreVersion::InitList()
{
	m_lcVersion.DeleteAllItems();
	m_lcPreVersion.DeleteAllItems();

	while(m_lcVersion.GetColumns())
        m_lcVersion.DeleteColumn(0);

	while(m_lcPreVersion.GetColumns())
        m_lcPreVersion.DeleteColumn(0);

	//	리스트 컨트롤 초기화
	LV_COLUMN lvColumn;
	memset(&lvColumn, 0, sizeof(LV_COLUMN));
	lvColumn.mask		=  LVCF_SUBITEM | LVCF_TEXT;
	
	m_lcVersion.m_HeaderCtrl.SetHeight(20);	
	m_lcPreVersion.m_HeaderCtrl.SetHeight(20);

	char*list[]={"Ver", "Name", "Path", "Size", "State"};
	int nWidth[]={40,110,65,60,50};

	for(int i = 0; ; i++)
	{
		if(list[i] == NULL)
			break;

		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;

		m_lcVersion.InsertColumn(i,&lvColumn);
		m_lcPreVersion.InsertColumn(i,&lvColumn);

		m_lcVersion.SetColumnWidth(i, nWidth[i]);
		m_lcPreVersion.SetColumnWidth(i, nWidth[i]);
	}

	m_lcVersion.SetUnderLine(TRUE);
	m_lcPreVersion.SetUnderLine(TRUE);

	m_lcVersion.SetColorUnderLine( RGB(240,240,240) );
	m_lcPreVersion.SetColorUnderLine( RGB(240,240,240) );

	m_lcVersion.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcPreVersion.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CPreVersion::OnBnClickedBtnTvermove()
{
	if(2 == m_bSelectList)
	{
		for(int i = 0; i < m_lcPreVersion.GetItemCount(); i++)
		{
			if( m_lcPreVersion.GetItemState(i, LVIS_SELECTED) != 0 )
			{
				DWORD dwSelect = m_lcPreVersion.GetItemData(i);
				if(m_vPrePatchTable[dwSelect].m_bState == STATE_NONE)
					m_vPrePatchTable[dwSelect].m_bState = STATE_MOVE;
			}
		}
	}
	else if(1 == m_bSelectList)
	{
		for(int i = 0; i < m_lcVersion.GetItemCount(); i++)
		{
			if( m_lcVersion.GetItemState(i, LVIS_SELECTED) != 0 )
			{
				DWORD dwSelect = m_lcVersion.GetItemData(i);
				if(m_vPrePatchTable[dwSelect].m_bState == STATE_MOVE)
					m_vPrePatchTable[dwSelect].m_bState = STATE_NONE;
			}
		}

	}

	RefreshList();
}

void CPreVersion::OnBnClickedBtnPredelete()
{
	if(2 == m_bSelectList)
	{
		for(int i = 0; i < m_lcPreVersion.GetItemCount(); i++)
		{
			if( m_lcPreVersion.GetItemState(i, LVIS_SELECTED) != 0 )
			{
				DWORD dwSelect = m_lcPreVersion.GetItemData(i);
				switch(m_vPrePatchTable[dwSelect].m_bState)
				{
				case STATE_DEL:		m_vPrePatchTable[dwSelect].m_bState = STATE_NONE; break;
				case STATE_NONE:	m_vPrePatchTable[dwSelect].m_bState = STATE_DEL; break;
				case STATE_NEWDEL:	m_vPrePatchTable[dwSelect].m_bState = STATE_NEW; break;
				case STATE_NEW:		m_vPrePatchTable[dwSelect].m_bState = STATE_NEWDEL; break;
				}
			}
		}
	}

	RefreshList();
}

void CPreVersion::OnBnClickedBtnPreupdate()
{
	WORD wMoveCount, wDelCount, wNewCount;	
	wMoveCount= wDelCount= wNewCount= 0;
	for(VPATFILE::iterator it = m_vPrePatchTable.begin(); it != m_vPrePatchTable.end(); it++)
	{
		switch(it->m_bState)
		{
		case STATE_MOVE:wMoveCount++;break;
		case STATE_DEL:	wDelCount++; break;
		case STATE_NEW:	wNewCount++; break;
		//default:m_vPrePatchTable.erase(it); break;
		}
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	pDoc->SendCT_PREVERSIONUPDATE_REQ(wMoveCount, wDelCount, wNewCount, &m_vPrePatchTable);
}
void CPreVersion::RefreshList()
{
	SetDlgItemText(IDC_BTN_TVERMOVE, "<<");
	SetDlgItemText(IDC_BTN_PREDELETE,"Delete");

	m_bSelectList = 0;

	m_btnDel.EnableWindow(FALSE);
	m_btnMove.EnableWindow(FALSE);

	m_lcVersion.DeleteAllItems();
	m_lcPreVersion.DeleteAllItems();

	int nCount = (int)m_vPrePatchTable.size();
	for(int i = 0; i < nCount; i++)
	{
		if(m_vPrePatchTable[i].m_bState == STATE_MOVE)
			PushVerList(&m_vPrePatchTable[i], i);
		else
			PushPreVerList(&m_vPrePatchTable[i], i);
	}
}

void CPreVersion::PushPreVerList(LPTPATFILE pPatch, DWORD dwData, int nItem/*=-1*/)
{
	if(nItem == -1)
		nItem = m_lcPreVersion.GetItemCount();
	if(!pPatch)
		return;

	CString str;
	int nCol = 0;

	const COLORREF colBg = RGB(255,255,255);
	COLORREF colState = RGB(0,0,0);
	
	
	// State
	switch(pPatch->m_bState)
	{
	case STATE_NONE	:
		{
			str.Format("%d",pPatch->m_dwVersion);
			m_lcPreVersion.InsertItem(nItem, str);
			str = _T("");
			break;
		}
	case STATE_NEW	:
		{
			str = _T("");
			m_lcPreVersion.InsertItem(nItem, str);
			str = _T("New");
			colState = RGB(0,0,255);
			break;
		}
	case STATE_DEL	:
		{
			str.Format("%d",pPatch->m_dwVersion);
			m_lcPreVersion.InsertItem(nItem, str);
			str = _T("Del");
			colState = RGB(255,0,0);
			break;
		}
	case STATE_NEWDEL:
		{
			str = _T("");
			m_lcPreVersion.InsertItem(nItem, str);
			str = _T("Del");
			colState = RGB(200,0,100);
			break;
		}
	default:
		return;
	}

	m_lcPreVersion.SetItemTextColor(nItem,nCol,colState, colBg);

	//	Name
	nCol++;
	m_lcPreVersion.SetItemText(nItem, nCol, pPatch->m_strName);
	m_lcPreVersion.SetItemTextColor(nItem,nCol,colState, colBg);

	//	Path 
	nCol++;
	m_lcPreVersion.SetItemText(nItem, nCol, pPatch->m_strPath);
	m_lcPreVersion.SetItemTextColor(nItem,nCol,colState, colBg);

	//	Size
	nCol++;
	m_lcPreVersion.SetItemInt(nItem, nCol, (int)pPatch->m_dwSize);
	m_lcPreVersion.SetItemTextColor(nItem,nCol,colState, colBg);

	//	State
	nCol++;	
	m_lcPreVersion.SetItemText(nItem, nCol, str);
	m_lcPreVersion.SetItemTextColor(nItem,nCol,colState, colBg);

	//	Data
	m_lcPreVersion.SetItemData(nItem, dwData);
}

void CPreVersion::PushVerList(LPTPATFILE pPatch, DWORD dwData, int nItem/*=-1*/)
{
	if(nItem == -1)
		nItem = m_lcVersion.GetItemCount();
	if(!pPatch)
		return;

	CString str;
	int nCol = 0;

	const COLORREF colBg = RGB(255,255,255);
	const COLORREF colMove = RGB(0,200,0);

	// BetaVer
	str.Format("%d",pPatch->m_dwVersion);
	m_lcVersion.InsertItem(nItem, str);
	m_lcVersion.SetItemTextColor(nItem, nCol, colMove, colBg);

	// Name
	nCol++;
	m_lcVersion.SetItemText(nItem, nCol, pPatch->m_strName);
	m_lcVersion.SetItemTextColor(nItem, nCol, colMove, colBg);

	// Path 
	nCol++;
	m_lcVersion.SetItemText(nItem, nCol, pPatch->m_strPath);
	m_lcVersion.SetItemTextColor(nItem, nCol, colMove, colBg);

	// Size
	nCol++;
	m_lcVersion.SetItemInt(nItem, nCol, (int)pPatch->m_dwSize);
	m_lcVersion.SetItemTextColor(nItem, nCol, colMove, colBg);

	// State
	nCol++;
	str = _T("Move");
	m_lcVersion.SetItemText(nItem, nCol, str);
	m_lcVersion.SetItemTextColor(nItem, nCol, colMove, colBg);

	//	Data
	m_lcVersion.SetItemData(nItem, dwData);
}

void CPreVersion::AddNewFileList(LPVPATCHFILE pvList)
{
	if(!pvList)
		return;
	int nCount = (int)pvList->size();
	if(!nCount)
		return;

	for(int i = 0; i < nCount ; i++)
	{
		LPPATCHFILE pNew = pvList->at(i);

		PushPreVersionTable(0, pNew->m_strPath.Right(pNew->m_strPath.GetLength()-1), 
			pNew->m_strZipFile, pNew->m_dwZipSize, STATE_NEW);
	}

}
void CPreVersion::OnNMClickLcTpreversion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	SetDlgItemText(IDC_BTN_TVERMOVE, "<<");
	SetDlgItemText(IDC_BTN_PREDELETE,"Delete");
	m_bSelectList = 2;

	if(pNMLV->iItem < 0)
		return;

	DWORD dwSelect = m_lcPreVersion.GetItemData(pNMLV->iItem);

	switch(m_vPrePatchTable[dwSelect].m_bState)
	{
	case STATE_NONE:
		{
			m_btnDel.EnableWindow(TRUE);
			m_btnMove.EnableWindow(TRUE);
		}
		break;
	case STATE_NEW:
		{
			m_btnDel.EnableWindow(TRUE);
			m_btnMove.EnableWindow(FALSE);
		}
		break;
	case STATE_DEL:
	case STATE_NEWDEL:
		{
			SetDlgItemText(IDC_BTN_PREDELETE,"UnDelete");
			m_btnDel.EnableWindow(TRUE);
			m_btnMove.EnableWindow(FALSE);
		}
	default:
		return;
	}

	*pResult = 0;
}

void CPreVersion::OnNMClickLcTversion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	m_bSelectList = 1;

	if(pNMLV->iItem < 0)
		return;

	DWORD dwSelect = m_lcVersion.GetItemData(pNMLV->iItem);

	if(m_vPrePatchTable[dwSelect].m_bState == STATE_MOVE)
	{
		SetDlgItemText(IDC_BTN_PREDELETE,"Delete");
		SetDlgItemText(IDC_BTN_TVERMOVE, ">>");
		m_btnDel.EnableWindow(FALSE);
		m_btnMove.EnableWindow(TRUE);
	}
	*pResult = 0;
}
