// ReplaceDetailTexDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "ReplaceDetailTexDlg.h"


// CReplaceDetailTexDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CReplaceDetailTexDlg, CDialog)
CReplaceDetailTexDlg::CReplaceDetailTexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDetailTexDlg::IDD, pParent)
{
	m_strNAME.Empty();
	m_dwTexID = 0;
	m_pLIST = NULL;
}

CReplaceDetailTexDlg::~CReplaceDetailTexDlg()
{
}

void CReplaceDetailTexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DETTEX, m_cLIST);
	DDX_Control(pDX, IDOK, m_cOK);
}


BEGIN_MESSAGE_MAP(CReplaceDetailTexDlg, CDialog)
END_MESSAGE_MAP()


// CReplaceDetailTexDlg 메시지 처리기입니다.

BOOL CReplaceDetailTexDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_cLIST.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cLIST.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cLIST.InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	InitLIST(m_pLIST);
	EnableAllCtrl(TRUE);

	return TRUE;
}

void CReplaceDetailTexDlg::InitLIST( CListCtrl *pLIST)
{
	m_cLIST.DeleteAllItems();

	if(pLIST)
	{
		int nCount = pLIST->GetItemCount();
		int nFROM = GetCurSel(pLIST);
		int nTO = -1;

		for( int i=0; i<nCount; i++)
		{
			LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(i);

			if( i == nFROM || pDETTEX->m_dwREPID == pDETTEX->m_dwTEXID )
			{
				int nIndex = m_cLIST.InsertItem( m_cLIST.GetItemCount(), pLIST->GetItemText( i, 0));

				m_cLIST.SetItemData( nIndex, pDETTEX->m_dwTEXID);
				if( i == nFROM )
					nTO = nIndex;
			}
		}

		if( nTO >= 0 && nTO < m_cLIST.GetItemCount() )
			SetCurSel(nTO);
	}
}

BYTE CReplaceDetailTexDlg::IsValidSEL( CListCtrl *pLIST)
{
	int nIndex = GetCurSel(pLIST);
	return nIndex < 0 || nIndex >= m_cLIST.GetItemCount() ? FALSE : TRUE;
}

void CReplaceDetailTexDlg::EnableAllCtrl( BYTE bEnable)
{
	bEnable = bEnable && IsValidSEL(&m_cLIST) ? TRUE : FALSE;
	m_cOK.EnableWindow(bEnable);
}

void CReplaceDetailTexDlg::SetCurSel( int nIndex)
{
	POSITION pos = m_cLIST.GetFirstSelectedItemPosition();
	BOOL bUpdate = nIndex != -1;

	while(pos)
	{
		int nSel = m_cLIST.GetNextSelectedItem(pos);

		if( nSel != nIndex )
		{
			m_cLIST.SetItem(
				nSel, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);

			m_cLIST.Update(nSel);
		}
		else
			bUpdate = FALSE;
	}

	if(bUpdate)
	{
		m_cLIST.SetItem(
			nIndex, 0,
			LVIF_STATE,
			NULL, 0,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			NULL);

		m_cLIST.Update(nIndex);
	}
}

int CReplaceDetailTexDlg::GetCurSel( CListCtrl *pList)
{
	POSITION pos = pList->GetFirstSelectedItemPosition();

	if(pos)
		return pList->GetNextSelectedItem(pos);

	return -1;
}

void CReplaceDetailTexDlg::OnOK()
{
	m_strNAME.Empty();
	m_dwTexID = 0;
	m_pLIST = NULL;

	if(IsValidSEL(&m_cLIST))
	{
		int nIndex = GetCurSel(&m_cLIST);

		m_dwTexID = (DWORD) m_cLIST.GetItemData(nIndex);
		m_strNAME = m_cLIST.GetItemText( nIndex, 0);
	}

	CDialog::OnOK();
}
