// TGiftSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "XListCtrl.h"
#include "TGiftSet.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CTGiftSet 대화 상자입니다.

IMPLEMENT_DYNCREATE(CTGiftSet, CDialog)
CTGiftSet::CTGiftSet(CWnd* pParent /*=NULL*/)
	: CDialog(CTGiftSet::IDD, pParent)
{
}

CTGiftSet::~CTGiftSet()
{
	m_mapCMGift.clear();
}

void CTGiftSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_GIFTSET, m_lcCMGift);
}

BOOL CTGiftSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bGroup = 0;
	m_stDefalultGift.m_wGiftID = 0;
	m_stDefalultGift.m_bGiftType = 0;
	m_stDefalultGift.m_dwValue = 0;
	m_stDefalultGift.m_bCount = 0;
	m_stDefalultGift.m_bTakeType = 0;
	m_stDefalultGift.m_bLevel = 0;
	m_stDefalultGift.m_bMaxTakeCount = 0;
	m_stDefalultGift.m_bToolOnly = 0;
	m_stDefalultGift.m_wErrGiftID = 0;
	m_stDefalultGift.m_strTitle = _T("Title");
	m_stDefalultGift.m_strMsg = _T("Message");
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BEGIN_MESSAGE_MAP(CTGiftSet, CDialog)
	ON_BN_CLICKED(IDC_GIFTADD, OnBnClickedGiftadd)
	ON_BN_CLICKED(IDC_GIFTDEL, OnBnClickedGiftdel)
	ON_BN_CLICKED(IDC_GIFTRE, OnBnClickedGiftre)
	ON_BN_CLICKED(IDC_GIFTUP, OnBnClickedGiftup)

	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHANGE, OnXCtrlUpdate)	
END_MESSAGE_MAP()

LRESULT CTGiftSet::OnXCtrlUpdate(WPARAM wParam, LPARAM lParam)
{
	CheckValues(HIWORD(wParam));
	return 0;
}

void CTGiftSet::SetData()
{
	m_lcCMGift.DeleteAllItems();

	for(MAPCMGIFT::iterator its = m_mapCMGift.begin(); its != m_mapCMGift.end(); its++)
		InsertListItem(&its->second);
}

void CTGiftSet::InitList()
{
	m_lcCMGift.SetEnableSubCtrl(TRUE);

	for(int i = 0; i < MAX_CMGIFT_COLUMN ; i++)
        m_lcCMGift.DeleteColumn(0);
	
	const TCHAR *lpszHeader[] = {_T("Edit"),_T("ID"),_T("Type"), _T("Value"), _T("Count"), _T("Take"), _T("MaxCount"), _T("Level"), _T("ToolOnly"),_T("ErrID"),_T("PostTitle"),_T("Message"),NULL};
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	for( int i=0; ; i++ )
	{
		if( lpszHeader[i] == NULL )
			break;

		lvcolumn.pszText = (TCHAR *)lpszHeader[i];
		lvcolumn.iSubItem = i;
		m_lcCMGift.InsertColumn(i, &lvcolumn);
	}		
	m_lcCMGift.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcCMGift.SetRowHeight(20);
	m_lcCMGift.SetUnderLine(TRUE);
	m_lcCMGift.SetColorUnderLine( RGB(240,240,240) );
	m_lcCMGift.m_HeaderCtrl.SetHeight(25);

	int arColWidthList[] = {40,30,80,80,50,60,80,50,70,60,100,220};
	int nColumnCount = sizeof(arColWidthList) / sizeof(arColWidthList[0]);
	for(int i = 0; i < nColumnCount; i++)
		m_lcCMGift.SetColumnWidth(i, arColWidthList[i]);
}

void CTGiftSet::InsertListItem(LPCMGIFT pGift)
{
	BYTE bNew = FALSE;
	if(!pGift)
	{
		pGift = &m_stDefalultGift;
		bNew = TRUE;
	}

	int nRow = m_lcCMGift.GetItemCount();	
	int nCol = 0;

	COLORREF crBgColor = RGB(255,255,255);
	COLORREF crText = RGB(0,0,0);	
	CString str = _T("");
	CStringArray sa;

	if(bNew)
		crText = RGB(0,0,255);

	// Edit	
	if(bNew)
		str = _T("Add");

	m_lcCMGift.InsertItem(nRow, str);
	m_lcCMGift.SetItemText(nRow, nCol, str, crText, crBgColor);

	// GiftID 
	nCol++;
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_wGiftID, crText, crBgColor);

	// Reward Type
	nCol++;	
	sa.RemoveAll();
	sa.Add(_T("None"));
	sa.Add(_T("Item"));
	sa.Add(_T("MagicItem"));
	sa.Add(_T("Money"));
	sa.Add(_T("Merit"));
	sa.Add(_T("EXP"));
	m_lcCMGift.SetComboBox(nRow,nCol,TRUE,&sa);	
	m_lcCMGift.SetItemText(nRow,nCol, sa.GetAt(pGift->m_bGiftType), crText, crBgColor);

	// Value
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_dwValue, crText, crBgColor);

	// Count
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_bCount, crText, crBgColor);
	
	// TakeType
	nCol++;	
	sa.RemoveAll();
	sa.Add(_T("NoLimit"));
	sa.Add(_T("Account"));
	sa.Add(_T("Character"));
	m_lcCMGift.SetComboBox(nRow,nCol,TRUE,&sa);
	m_lcCMGift.SetItemText(nRow,nCol, sa.GetAt(pGift->m_bTakeType), crText, crBgColor);

	// MaxTake
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_bMaxTakeCount, crText, crBgColor);

	// Level
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_bLevel, crText, crBgColor);

	// ToolOnly
	nCol++;	
	m_lcCMGift.SetCheckbox(nRow,nCol,pGift->m_bToolOnly);

	// 중복시
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_CENTER|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_NUMBER);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemInt(nRow, nCol, (int)pGift->m_wErrGiftID, crText, crBgColor);

	// PostTitle
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE|ES_AUTOHSCROLL);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemText(nRow, nCol, pGift->m_strTitle, crText, crBgColor);

	// Message
	nCol++;	
	m_lcCMGift.SetEditBox(nRow,nCol,TRUE,-1,-1,-1,FALSE,NULL,ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE|ES_AUTOHSCROLL);
	m_lcCMGift.SetEditBoxAlign(nRow,nCol,ES_CENTER);
	m_lcCMGift.SetItemText(nRow, nCol, pGift->m_strMsg, crText, crBgColor);

}

void CTGiftSet::DelteListItem(int nRow)
{
	WORD wGiftID = m_lcCMGift.GetItemInt(nRow, 1);
	COLORREF cFont = RGB(128,128,128);
	if(0 == wGiftID)
	{
		m_lcCMGift.DeleteItem(nRow);
		return;
	}
	else if(m_lcCMGift.GetItemText(nRow, 0) == _T("Del"))
	{
		m_lcCMGift.SetItemText(nRow, 0, _T(""));
		CheckValues(nRow);
	}
	else
	{
		m_lcCMGift.SetItemText(nRow, 0, _T("Del"));

		for(int i = 0; i < m_lcCMGift.GetColumns(); i++)
			m_lcCMGift.SetItemTextColor(nRow, i, RGB(128,128,128), RGB(255,255,255));
	}	
}

void CTGiftSet::ClearList()
{
	m_lcCMGift.DeleteAllItems();
}

void CTGiftSet::CheckValues(int nRow)
{
	WORD wGiftID = m_lcCMGift.GetItemInt(nRow, 1);
	if(!wGiftID)
		return;

	if(m_lcCMGift.GetItemText(nRow,0) == _T("Del"))
		return;

	MAPCMGIFT::iterator it = m_mapCMGift.find(wGiftID);
	if(it == m_mapCMGift.end())
		return;

	BYTE bEdit = FALSE;
	LPCMGIFT pGift = &it->second;
	COLORREF rgbBlack = RGB(0,0,0), rgbWhite = RGB(255,255,255), rgbRed = RGB(255,0,0);	
	TCMGIFT stTempGift;
	int nCol = 2;

	if(!GetListData(nRow, stTempGift))
		return;

	if(pGift->m_bGiftType == stTempGift.m_bGiftType)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}

	if(pGift->m_dwValue == stTempGift.m_dwValue)
		m_lcCMGift.SetItemTextColor(nRow, nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow, nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	
	if(pGift->m_bCount == stTempGift.m_bCount)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	
	if(pGift->m_bTakeType == stTempGift.m_bTakeType)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	
	if(pGift->m_bMaxTakeCount == stTempGift.m_bMaxTakeCount)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	if(pGift->m_bLevel == stTempGift.m_bLevel)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}

	if(pGift->m_bToolOnly == stTempGift.m_bToolOnly)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbRed);
		bEdit = TRUE;
	}
	
	if(pGift->m_wErrGiftID == stTempGift.m_wErrGiftID)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	
	if(pGift->m_strTitle == stTempGift.m_strTitle)
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol++, rgbRed, rgbWhite);
		bEdit = TRUE;
	}
	
	if(pGift->m_strMsg == stTempGift.m_strMsg)
		m_lcCMGift.SetItemTextColor(nRow,nCol, rgbBlack, rgbWhite);
	else
	{
		m_lcCMGift.SetItemTextColor(nRow,nCol, rgbRed, rgbWhite);
		bEdit = TRUE;
	}

	if(bEdit)
		m_lcCMGift.SetItemText(nRow, 0, _T("Edit"), rgbRed, rgbWhite);
	else
		m_lcCMGift.SetItemText(nRow, 0, _T(""));
}

 BYTE CTGiftSet::GetListData(int nRow, TCMGIFT& tGift)
{
	if(m_lcCMGift.GetItemCount() <= nRow)
		return FALSE;
	
	int nCol = 0;
	CString str;
	str = m_lcCMGift.GetItemText(nRow, nCol++);
	if(str == _T(""))
		tGift.m_bUpdate = CGU_NONE;
	else if(str == _T("Del"))
		tGift.m_bUpdate = CGU_DEL;
	else if(str == _T("Add"))
		tGift.m_bUpdate = CGU_ADD;
	else if(str == _T("Edit"))
		tGift.m_bUpdate = CGU_UPDATE;

	tGift.m_wGiftID =		(WORD)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_bGiftType =		(BYTE)	m_lcCMGift.GetComboCurSel(nRow, nCol++);
	tGift.m_dwValue =		(DWORD)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_bCount =		(BYTE)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_bTakeType =		(BYTE)	m_lcCMGift.GetComboCurSel(nRow, nCol++);
	tGift.m_bMaxTakeCount = (BYTE)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_bLevel =		(BYTE)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_bToolOnly =		(BYTE)	m_lcCMGift.GetCheckbox(nRow, nCol++);
	tGift.m_wErrGiftID =	(WORD)	m_lcCMGift.GetItemInt(nRow, nCol++);
	tGift.m_strTitle =				m_lcCMGift.GetItemText(nRow, nCol++);
	tGift.m_strMsg =				m_lcCMGift.GetItemText(nRow, nCol);
	
	return TRUE;
}

// Enter / Escape block
BOOL CTGiftSet::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
		(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}

int CTGiftSet::GetSelectItem()
{
	// 선택된 아이템 찾기		
	int iCount = m_lcCMGift.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcCMGift.GetItemState(i, LVIS_SELECTED) != 0 )
			return i;
	}
	return -1;
}

void CTGiftSet::OnBnClickedGiftadd()
{
	InsertListItem();
}

void CTGiftSet::OnBnClickedGiftdel()
{
	int nRow = GetSelectItem();
	if(-1 == nRow)
		return;

	DelteListItem(nRow);
}

void CTGiftSet::OnBnClickedGiftre()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_CMGIFTLIST_REQ(m_bGroup);
}

void CTGiftSet::OnBnClickedGiftup()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	QCMGIFT qList;
	TCMGIFT tGift;
	for(int i = 0; i < m_lcCMGift.GetItemCount(); i++)
	{
		if(GetListData(i,tGift) && tGift.m_bUpdate)
			qList.push_back(tGift);		
	}

	if(qList.size())
		pDoc->SendCT_CMGIFTCHARTUPDATE_REQ(&qList, m_bGroup);
}