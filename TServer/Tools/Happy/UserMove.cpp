// 현승룡 UserMove.cpp

// UserMove.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "UserMove.h"
#include ".\usermove.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CUserMove 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserMove, CDialog)
CUserMove::CUserMove(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMove::IDD, pParent)
	, m_bType(0)
{	
}

CUserMove::~CUserMove()
{
}

void CUserMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboWorld);
	DDX_Control(pDX, IDC_POSFIND, m_listctrPos);
}


BEGIN_MESSAGE_MAP(CUserMove, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE_POS, OnBnClickedSavePos)
	ON_BN_CLICKED(IDC_RADIO_COORD, OnBnClickedRadioCoord)
	ON_BN_CLICKED(IDC_RADIO_CHAR, OnBnClickedRadioChar)

	ON_BN_CLICKED(IDC_DEL_POS, OnBnClickedDelPos)
	ON_NOTIFY(NM_DBLCLK, IDC_POSFIND, OnNMDblclkPosfind)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_POSFIND, OnLvnItemchangedPosfind)
END_MESSAGE_MAP()

// 좌표이동
void CUserMove::MoveNow(LPUSERPOS pUserPos)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();

	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
		
	queue<CString> qStrUser;
	CString strTmp;
	CString strName;
	int nString = 0;
	BYTE bGroupID;

	//	이름 얻기
	GetDlgItemText(IDC_EDIT1, strName);
	strName.Replace(" ","");

	if(strName.IsEmpty())
	{
		AfxMessageBox(_T("Enter user name"));
		return;
	}

	strTmp = strName.Tokenize(",", nString);
	while(!strTmp.IsEmpty())
	{
		qStrUser.push(strTmp);
		strTmp = strName.Tokenize(",", nString);
	}

	if(qStrUser.empty())
	{
		AfxMessageBox(_T("Enter user name"));
		return;
	}

	//	그룹 얻기
	m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), strTmp);

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
	{
		if((*itG).second->m_strName == strTmp)
		{
			bGroupID = (*itG).second->m_bID;
			break;
		}
	}
	if( itG == pDoc->m_mapGroup.end() )
	{
		AfxMessageBox(_T("Not found server"));
		return;
	}	

	if(pUserPos)
		return pDoc->SendCT_USERMOVE_REQ(&qStrUser, bGroupID, pUserPos->bChannelNum, pUserPos->wMapID, pUserPos->PosX, pUserPos->PosY, pUserPos->PosZ);

	//	타입에 따른 분류
	if(!m_bType)
	{
		BYTE bChannelID;
		WORD wMapID;
		float fPosX, fPosY, fPosZ;

		bChannelID = (BYTE)GetDlgItemInt(IDC_EDIT6);

		wMapID = (WORD)GetDlgItemInt(IDC_EDIT7);

		GetDlgItemText(IDC_EDIT2,strTmp);
		fPosX = (float)atof(strTmp);

		GetDlgItemText(IDC_EDIT3,strTmp);
		fPosY = (float)atof(strTmp);

		GetDlgItemText(IDC_EDIT4,strTmp);
		fPosZ = (float)atof(strTmp);

		if(!fPosX && !fPosY && !fPosZ)
			return;
		
		pDoc->SendCT_USERMOVE_REQ(&qStrUser, bGroupID, bChannelID, wMapID, fPosX, fPosY, fPosZ);
	}
	else
	{
		GetDlgItemText(IDC_EDIT_TARGET,strTmp);

		if(strTmp.IsEmpty())
		{
			AfxMessageBox(_T("Enter target name"));
			return;
		}
		pDoc->SendCT_USERPOSITION_REQ(bGroupID, &qStrUser, strTmp);
	}
}

// CUserMove 메시지 처리기입니다.
void CUserMove::OnBnClickedOk()
{
	MoveNow();
}

void CUserMove::OnBnClickedDelPos()
{
	ItemDelete();
}

void CUserMove::OnBnClickedRadioCoord()
{
	ShowLocationItem(TRUE);
}

void CUserMove::OnBnClickedRadioChar()
{
	ShowLocationItem(FALSE);
}

void CUserMove::OnBnClickedSavePos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();

	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	CString strTmp;
	CSavePos DlgSavePos(this);

	DlgSavePos.m_pstrSaveName = &strTmp;
	if(IDCANCEL == DlgSavePos.DoModal())
		return;

	LPUSERPOS pUSER = new USERPOS;

	pUSER->strName		= strTmp;
	pUSER->bChannelNum	= GetDlgItemInt(IDC_EDIT6);
	pUSER->wMapID		= GetDlgItemInt(IDC_EDIT7);

	GetDlgItemText(IDC_EDIT2, strTmp);
	pUSER->PosX = (float)atof(strTmp);
	
	GetDlgItemText(IDC_EDIT3, strTmp);
	pUSER->PosY = (float)atof(strTmp);

	GetDlgItemText(IDC_EDIT4, strTmp);
	pUSER->PosZ = (float)atof(strTmp);
	
	pDoc->m_setUSERPOS.insert(pUSER);
	InsertItemToListCtr(m_listctrPos.GetItemCount(), pUSER);
}

void CUserMove::ShowLocationItem(BYTE bShow)
{
	m_bType = !bShow;

	if(bShow)
		GetDlgItem(IDC_GROUP_NAME)->SetWindowText("Location");
	else
		GetDlgItem(IDC_GROUP_NAME)->SetWindowText("User");

	if(bShow)
		bShow = SW_SHOW;

	GetDlgItem(IDC_EDIT2)->ShowWindow(bShow); // PosX
	GetDlgItem(IDC_EDIT3)->ShowWindow(bShow); // PosY
	GetDlgItem(IDC_EDIT4)->ShowWindow(bShow); // PosZ
	GetDlgItem(IDC_EDIT6)->ShowWindow(bShow); // Channel
	GetDlgItem(IDC_EDIT7)->ShowWindow(bShow); // Map
	GetDlgItem(IDC_SAVE_POS)->ShowWindow(bShow);
	GetDlgItem(IDC_MV_STATIC1)->ShowWindow(bShow);
	GetDlgItem(IDC_MV_STATIC2)->ShowWindow(bShow);
	GetDlgItem(IDC_MV_STATIC3)->ShowWindow(bShow);
	GetDlgItem(IDC_MV_STATIC4)->ShowWindow(bShow);
	GetDlgItem(IDC_MV_STATIC5)->ShowWindow(bShow);
	    
	if(!bShow)
		bShow = SW_SHOW;
	else
		bShow = SW_HIDE;

	GetDlgItem(IDC_EDIT_TARGET)->ShowWindow(bShow);
	GetDlgItem(IDC_ST_CHARNAME)->ShowWindow(bShow);	
}

void CUserMove::SetData(LPUSERPOS pUserPos)
{
	if(!pUserPos)
		return;

	CString strTemp;
	SetDlgItemInt(IDC_EDIT6, pUserPos->bChannelNum);	
	SetDlgItemInt(IDC_EDIT7, pUserPos->wMapID);

	strTemp.Format("%.2f",pUserPos->PosX);
	SetDlgItemText(IDC_EDIT2,strTemp);
	
	strTemp.Format("%.2f", pUserPos->PosY);
	SetDlgItemText(IDC_EDIT3,strTemp);
	
	strTemp.Format("%.2f", pUserPos->PosZ);
	SetDlgItemText(IDC_EDIT4,strTemp);
}

void CUserMove::SetWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Choice Server"));
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)	
		if((*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);
	
    m_comboWorld.SetCurSel(0);
}

void CUserMove::Init()
{
	SetWorld();

	SetDlgItemText(IDC_EDIT1,"");
	SetDlgItemText(IDC_EDIT2,"");
	SetDlgItemText(IDC_EDIT3,"");
	SetDlgItemText(IDC_EDIT4,"");
	SetDlgItemText(IDC_EDIT6,"1");
	SetDlgItemText(IDC_EDIT7,"0");
	SetDlgItemText(IDC_EDIT_TARGET,"");
	
	CButton* pradioCoord = (CButton*)GetDlgItem(IDC_RADIO_COORD);
	pradioCoord->SetCheck(TRUE);
	CButton* pradioChar = (CButton*)GetDlgItem(IDC_RADIO_CHAR);
	pradioChar->SetCheck(FALSE);
	ShowLocationItem(TRUE);

	GetDlgItem(IDC_EDIT1)->SetFocus();

	// 리스트 컨트롤의 모든 컬럼 삭제
    m_listctrPos.DeleteColumn(0);
	m_listctrPos.InsertColumn(0,"name", LVCFMT_LEFT, 190);
	m_listctrPos.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	LoadPosListFromData();
}

void CUserMove::InsertItemToListCtr(int _iRow, LPUSERPOS _USERPOS)
{
	m_listctrPos.InsertItem(_iRow, _USERPOS->strName.GetBuffer(0));
	m_listctrPos.SetItemData(_iRow, (DWORD)_USERPOS);
}

void CUserMove::LoadPosListFromData()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	ClearListItem();

	SETUSERPOS::iterator it;
	int i = 0;
	for( it = pDoc->m_setUSERPOS.begin(); it != pDoc->m_setUSERPOS.end(); it++, i++)
		InsertItemToListCtr(i,(*it));

	m_listctrPos.UpdateData();
}

void CUserMove::ItemDelete()
{
	if(AfxMessageBox("Are you sure you want to delete?",MB_YESNO) == IDNO ) //진정 삭제하시겠습니까
		return;	

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}	

	// 선택된 아이템 찾기	
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			LPUSERPOS pPos = (LPUSERPOS)m_listctrPos.GetItemData(i);
			DeleteListData(pPos);
			m_listctrPos.DeleteItem(i);
		}
	}
}

void CUserMove::ClearListItem()
{
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
		m_listctrPos.DeleteItem(i);
    
	m_listctrPos.DeleteAllItems();
}

BYTE CUserMove::FindListData(LPUSERPOS pPos)
{
	if(!pPos)
		return FALSE;

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return FALSE;
	}
	if(pDoc->m_setUSERPOS.empty())
		return FALSE;

	SETUSERPOS::iterator it = pDoc->m_setUSERPOS.find(pPos);
	if(it != pDoc->m_setUSERPOS.end())
		return TRUE;

	return FALSE;
}

void CUserMove::DeleteListData(LPUSERPOS pPos)
{
	if(!pPos)
		return ;

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	if(pDoc->m_setUSERPOS.empty())
		return ;

	SETUSERPOS::iterator it = pDoc->m_setUSERPOS.find(pPos);
	if(it != pDoc->m_setUSERPOS.end())
	{
		delete (*it);
		pDoc->m_setUSERPOS.erase(it);
	}
}


void CUserMove::OnNMDblclkPosfind(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	// 선택된 아이템 찾기
	for(int i = 0; i < m_listctrPos.GetItemCount(); i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
			return MoveNow((LPUSERPOS)m_listctrPos.GetItemData(i));
	}
}

void CUserMove::OnLvnItemchangedPosfind(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	// 선택된 아이템 찾기
	for(int i = 0; i < m_listctrPos.GetItemCount(); i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
			return SetData((LPUSERPOS)m_listctrPos.GetItemData(i));
	}
}
