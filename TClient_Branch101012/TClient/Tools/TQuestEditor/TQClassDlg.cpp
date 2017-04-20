// TQClassDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TListCtrl.h"
#include "TQuestEditor.h"
#include "TEditTQClassDlg.h"
#include "TQClassDlg.h"
#include "TQClassSet.h"
#include "TTitleSet.h"


// CTQClassDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTQClassDlg, CDialog)
CTQClassDlg::CTQClassDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTQClassDlg::IDD, pParent)
{
	m_pTQCLASS = NULL;
	m_pDB = NULL;
}

CTQClassDlg::~CTQClassDlg()
{
}

void CTQClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLASS, m_cLIST);
}


BEGIN_MESSAGE_MAP(CTQClassDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CLASS, OnBnClickedButtonAddClass)
	ON_BN_CLICKED(IDC_BUTTON_DEL_CLASS, OnBnClickedButtonDelClass)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_CLASS, OnBnClickedButtonEditClass)
END_MESSAGE_MAP()


// CTQClassDlg 메시지 처리기입니다.

BOOL CTQClassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_cLIST.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	m_cLIST.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cLIST.InsertColumn( 0, "ID", LVCFMT_LEFT, 120);
	m_cLIST.InsertColumn( 1, "NAME", LVCFMT_LEFT, rect.Width() - 200);
	m_cLIST.InsertColumn( 2, "MAIN", LVCFMT_LEFT, 80);

	MAPTQCLASS::iterator it;
	for( it = m_pTQCLASS->begin(); it != m_pTQCLASS->end(); it++)
	{
		CString strID;
		CString strMain;
		strID.Format( "%d", (*it).second->m_dwClassID);
		strMain.Format("%s", (*it).second->m_bClassMain ? "TRUE" : "FALSE" );

		int nIndex = m_cLIST.InsertItem( m_cLIST.GetItemCount(), strID);
		m_cLIST.SetItemText( nIndex, 1, (*it).second->m_strNAME);

		m_cLIST.SetItemText( nIndex, 2, strMain);
		m_cLIST.SetItemData( nIndex, (DWORD_PTR) (*it).second);
	}

	return TRUE;
}

void CTQClassDlg::OnBnClickedButtonAddClass()
{
	CTEditTQClassDlg dlg;

	dlg.m_strTITLE.Format("Add Category");
	if( dlg.DoModal() == IDOK )
	{
		dlg.m_strNAME.TrimRight();
		dlg.m_strNAME.TrimLeft();

		if(!dlg.m_strNAME.IsEmpty())
		{
			CTQClassSet set(m_pDB);

			if(set.Open())
			{
				LPTQCLASS pTQCLASS = new TQCLASS();

				set.AddNew();
				set.m_szNAME = dlg.m_strNAME;
				set.Update();
				set.Requery();
				set.MoveLast();

				pTQCLASS->m_dwClassID = set.m_dwClassID;
				pTQCLASS->m_strNAME = set.m_szNAME;
				pTQCLASS->m_bClassMain = set.m_bClassMain;

				m_pTQCLASS->insert( MAPTQCLASS::value_type( pTQCLASS->m_dwClassID, pTQCLASS));

				CString strID;
				CString strMain;
				strID.Format( "%d", pTQCLASS->m_dwClassID);
				strMain.Format("%s", pTQCLASS->m_bClassMain ? "TRUE" : "FALSE" );

				int nIndex = m_cLIST.InsertItem( m_cLIST.GetItemCount(), strID);
				m_cLIST.SetItemText( nIndex, 1, pTQCLASS->m_strNAME);
				m_cLIST.SetItemText( nIndex, 2, strMain);
				m_cLIST.SetItemData( nIndex, (DWORD_PTR) pTQCLASS);
				m_cLIST.SetCurSel(nIndex);

				set.Close();
			}
		}
	}
}

void CTQClassDlg::OnBnClickedButtonDelClass()
{
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex >= 0 )
	{
		LPTQCLASS pTQCLASS = (LPTQCLASS) m_cLIST.GetItemData(nIndex);

		if(pTQCLASS)
		{
			CTTitleSet set(m_pDB);

			set.m_strFilter.Format( "dwClassID = %d", pTQCLASS->m_dwClassID);
			if(set.Open())
			{
				if(!set.IsEOF())
				{
					CString strERROR;

					set.MoveFirst();
					strERROR.Format(
						IDS_FMT_DEL_CLASS_ERROR,
						set.m_szTitle,
						set.m_dwClassID);

					MessageBox(strERROR);
					set.Close();

					return;
				}

				set.Close();
			}

			CTQClassSet setCLASS(m_pDB);

			setCLASS.m_strFilter.Format( "dwClassID = %d", pTQCLASS->m_dwClassID);
			if(setCLASS.Open())
			{
				if(!setCLASS.IsEOF())
				{
					setCLASS.MoveFirst();
					setCLASS.Delete();

					MAPTQCLASS::iterator finder = m_pTQCLASS->find(pTQCLASS->m_dwClassID);
					if( finder != m_pTQCLASS->end() )
						m_pTQCLASS->erase(finder);
					m_cLIST.DeleteItem(nIndex);

					delete pTQCLASS;
				}

				setCLASS.Close();
			}
		}
	}
}

void CTQClassDlg::OnBnClickedButtonEditClass()
{
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex >= 0 )
	{
		LPTQCLASS pTQCLASS = (LPTQCLASS) m_cLIST.GetItemData(nIndex);

		if(pTQCLASS)
		{
			CTEditTQClassDlg dlg;

			dlg.m_strTITLE.Format("Edit Category");
			dlg.m_strNAME = pTQCLASS->m_strNAME;

			if( dlg.DoModal() == IDOK )
			{
				dlg.m_strNAME.TrimRight();
				dlg.m_strNAME.TrimLeft();

				if( !dlg.m_strNAME.IsEmpty() && dlg.m_strNAME != pTQCLASS->m_strNAME )
				{
					CTQClassSet set(m_pDB);

					set.m_strFilter.Format( "dwClassID = %d", pTQCLASS->m_dwClassID);
					if(set.Open())
					{
						if(!set.IsEOF())
						{
							set.MoveFirst();
							set.Edit();
							set.m_szNAME = dlg.m_strNAME;
							set.m_bClassMain = dlg.m_bClassMain;
							set.Update();

							pTQCLASS->m_strNAME = dlg.m_strNAME;
							pTQCLASS->m_bClassMain = dlg.m_bClassMain;
							m_cLIST.SetItemText( nIndex, 1, dlg.m_strNAME);
						}

						set.Close();
					}
				}
			}
		}
	}
}
