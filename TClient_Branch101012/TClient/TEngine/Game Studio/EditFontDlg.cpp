// EditFontDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditFontDlg.h"
#include "PreviewDlg.h"
#include "ItemBase.h"
#include ".\editfontdlg.h"


// CEditFontDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditFontDlg, CDialog)
CEditFontDlg::CEditFontDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEditFontDlg::IDD, pParent)
{
	m_strSEL.Empty();
}

CEditFontDlg::~CEditFontDlg()
{
}

void CEditFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FONT, m_cFONT);
}


BEGIN_MESSAGE_MAP(CEditFontDlg, CDialog)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_FONT, OnLvnItemActivateListFont)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_FONT, OnLvnEndlabeleditListFont)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_FONT, OnLvnBeginlabeleditListFont)
	ON_BN_CLICKED(ID_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(ID_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(ID_BUTTON_EDIT, OnBnClickedButtonEdit)
	ON_NOTIFY(HDN_BEGINTRACK, 0, OnHdnBegintrackListFont)
	ON_NOTIFY(HDN_TRACK, 0, OnHdnTrackListFont)
	ON_BN_CLICKED(ID_BUTTON_EDIT_COLOR, OnBnClickedButtonEditColor)
END_MESSAGE_MAP()


// CEditFontDlg 메시지 처리기입니다.

BOOL CEditFontDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	
	CRect rect;

	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	
	LVCOLUMN lvcolmn;
	lvcolmn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolmn.fmt = LVCFMT_LEFT;

	for(int i=0; i<2; i++ )
	{
		lvcolmn.iSubItem = i;
		switch(i)
		{
		case 0:
			lvcolmn.cx = rect.Width() - 50;
			lvcolmn.pszText = "ID";
			break;
		case 1:
			lvcolmn.cx = 50;
			lvcolmn.pszText = "COLOR";
			break;
		}
		pListCtrl->InsertColumn( i, &lvcolmn);
	}

	MAPGSFONT::iterator it;
	for( it = CItemBase::m_mapFont.begin(); it != CItemBase::m_mapFont.end(); it++)
	{
		int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*it).second->m_strFontID);

		if( m_strSEL == (*it).second->m_strFontID )
			m_cFONT.SetCurSel(nIndex);
	}

	return TRUE;
}

void CEditFontDlg::DeleteFONT( int nIndex)
{
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CString strName = pListCtrl->GetItemText( nIndex, 0);
	MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strName);

	if( finder != CItemBase::m_mapGlobalID.end() )
	{
		MAPGSFONT::iterator it = CItemBase::m_mapFont.find((*finder).second);

		if( it != CItemBase::m_mapFont.end() )
		{
			delete (*it).second;
			CItemBase::m_mapFont.erase(it);
		}

		CItemBase::m_mapGlobalID.erase(finder);
		pListCtrl->DeleteItem(nIndex);

		CTachyonDoc::UpdateAll(NULL);
	}
}

void CEditFontDlg::OnLvnItemActivateListFont(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteFONT(pNMIA->iItem);
	*pResult = 0;
}

void CEditFontDlg::OnLvnEndlabeleditListFont(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CString strOLD = pListCtrl->GetItemText( pDispInfo->item.iItem, 0);
	CString strNEW(pDispInfo->item.pszText);

	if( strNEW != strOLD )
		if( strNEW.IsEmpty() || !CItemBase::CheckID(strNEW) )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else if(!CItemBase::CheckDupID( strOLD, strNEW))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else
		{
			MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strOLD);
			DWORD dwID = 1;

			if( finder != CItemBase::m_mapGlobalID.end() )
			{
				dwID = (*finder).second;
				CItemBase::m_mapGlobalID.erase(finder);
			}
			else
			{
				while(CItemBase::m_mapFont.find(dwID) != CItemBase::m_mapFont.end())
					dwID++;
			}
			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strNEW, dwID));

			MAPGSFONT::iterator it = CItemBase::m_mapFont.find(dwID);
			if( it == CItemBase::m_mapFont.end() )
			{
				LPGSFONT pFONT = new GSFONT();

				pFONT->m_strFontID = strNEW;
				CItemBase::m_mapFont.insert( MAPGSFONT::value_type( dwID, pFONT));
			}
			else
				(*it).second->m_strFontID = strNEW;
			pListCtrl->SetItemText( pDispInfo->item.iItem, 0, strNEW);
		}

	*pResult = 0;
}

void CEditFontDlg::OnLvnBeginlabeleditListFont(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CEdit *pEdit = m_cFONT.GetListCtrl()->GetEditControl();
	pEdit->ModifyStyle( 0, ES_UPPERCASE);
	*pResult = 0;
}

void CEditFontDlg::OnBnClickedButtonAdd()
{
	CFontDialog dlg(0,CF_SCREENFONTS,0,0);

	
	if( dlg.DoModal() == IDOK )
	{
		CString strName = CItemBase::GetNewID(IDS_FMT_NEWFONT);
		LPGSFONT pFONT = new GSFONT();
		CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
		DWORD dwID = 1;

		while(CItemBase::m_mapFont.find(dwID) != CItemBase::m_mapFont.end())
			dwID++;

		memcpy( &pFONT->m_FONT, dlg.m_cf.lpLogFont, sizeof(LOGFONT));
		pFONT->m_GDIFONT.CreateFontIndirect(&pFONT->m_FONT);
		pFONT->m_D3DFONT.SetFont(&pFONT->m_GDIFONT);
		pFONT->m_dwColor = 0xFF000000 | dlg.GetColor();
		pFONT->m_D3DFONT.m_dwColor = pFONT->m_dwColor;
		pFONT->m_strFontID = strName;
		pListCtrl->InsertItem( pListCtrl->GetItemCount(), strName);

		CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strName, dwID));
		CItemBase::m_mapFont.insert( MAPGSFONT::value_type( dwID, pFONT));
	}
}

void CEditFontDlg::OnBnClickedButtonDel()
{
	int nIndex = m_cFONT.GetCurSel();

	if( nIndex != -1 )
		DeleteFONT(nIndex);
}

void CEditFontDlg::OnBnClickedButtonEdit()
{
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CString strName = pListCtrl->GetItemText( m_cFONT.GetCurSel(), 0);

	MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strName);
	if( finder != CItemBase::m_mapGlobalID.end() )
	{
		MAPGSFONT::iterator it = CItemBase::m_mapFont.find((*finder).second);

		if( it != CItemBase::m_mapFont.end() )
		{
			LPGSFONT pFONT = (*it).second;
			CFontDialog dlg(&pFONT->m_FONT,CF_SCREENFONTS, NULL,NULL);

			if( dlg.DoModal() == IDOK )
			{
				pFONT->m_GDIFONT.DeleteObject();
				pFONT->m_D3DFONT.Release();

				memcpy( &pFONT->m_FONT, dlg.m_cf.lpLogFont, sizeof(LOGFONT));
				pFONT->m_GDIFONT.CreateFontIndirect(&pFONT->m_FONT);
				pFONT->m_D3DFONT.SetFont(&pFONT->m_GDIFONT);
				pFONT->m_dwColor = VALUEARGB(
					0xFF, GetRValue(dlg.GetColor()),
					GetGValue(dlg.GetColor()),
					GetBValue(dlg.GetColor()));
				pFONT->m_D3DFONT.m_dwColor = pFONT->m_dwColor;
			}
		}
	}
}

void CEditFontDlg::OnOK()
{
	int nIndex = m_cFONT.GetCurSel();

	if( nIndex != -1 )
		m_strSEL = m_cFONT.GetListCtrl()->GetItemText( nIndex, 0);
	else
		m_strSEL.Empty();

	CDialog::OnOK();
}

void CEditFontDlg::OnHdnBegintrackListFont(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CRect rect;
	
	pListCtrl->GetClientRect(&rect);

	LVCOLUMN lvcolmn;
	lvcolmn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	for(int i=0; i<2; i++ )
	{
		switch(i)
		{
		case 0:
			lvcolmn.cx =pListCtrl->GetColumnWidth(i);
			break;
		case 1:
			lvcolmn.cx = pListCtrl->GetColumnWidth(i);
			break;
		}
		pListCtrl->SetColumn(i, &lvcolmn);
	}
	*pResult = 0;*/
}

void CEditFontDlg::OnHdnTrackListFont(NMHDR *pNMHDR, LRESULT *pResult)
{
	int k = 0;
	/*LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CRect rect;

	pListCtrl->GetClientRect(&rect);

	LVCOLUMN lvcolmn;
	lvcolmn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	
	for(int i=0; i<2; i++ )
	{
		switch(i)
		{
		case 0:
			lvcolmn.cx =pListCtrl->GetColumnWidth(i);
			lvcolmn.pszText= "ssss";
			break;
		case 1:
			lvcolmn.cx = pListCtrl->GetColumnWidth(i);
			lvcolmn.pszText= "skdslkdslksk";
			break;
		}
		pListCtrl->SetColumn(i, &lvcolmn);
		pListCtrl->UpdateData(FALSE);
	}
	*pResult = 0;*/
}

void CEditFontDlg::OnBnClickedButtonEditColor()
{
	CEditFontColorDlg dlg;
	CListCtrl *pListCtrl = m_cFONT.GetListCtrl();
	CString strName = pListCtrl->GetItemText( m_cFONT.GetCurSel(), 0);

	MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(strName);
	if( finder != CItemBase::m_mapGlobalID.end() )
	{
		MAPGSFONT::iterator it = CItemBase::m_mapFont.find((*finder).second);
		if( it != CItemBase::m_mapFont.end() )
		{
			LPGSFONT pFONT = (*it).second;
			dlg.InitColor( &pFONT->m_D3DFONT.m_dwColor );
			if( dlg.DoModal() == IDOK)
				pFONT->m_dwColor = pFONT->m_D3DFONT.m_dwColor;

			LVCOLUMN lvcolmn;
			lvcolmn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		}
	}
}

