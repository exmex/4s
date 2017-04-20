// TSessionView.cpp : CTSessionView 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionDoc.h"
#include "TSessionView.h"
#include ".\tsessionview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSessionView

IMPLEMENT_DYNCREATE(CTSessionView, CFormView)

BEGIN_MESSAGE_MAP(CTSessionView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_LIST_CLEAR, OnBnClickedListClear)
	ON_CBN_SELCHANGE(IDC_COMBO_THREAD, OnCbnSelchangeComboThread)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnBnClickedCheckAll)
END_MESSAGE_MAP()

// CTSessionView 생성/소멸

CTSessionView::CTSessionView()
	: CFormView(CTSessionView::IDD)
{
	m_bAllFlag = FALSE; // All Check
}

CTSessionView::~CTSessionView()
{
}

void CTSessionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_COMBO_THREAD, m_comboThread);
	DDX_Control(pDX, IDC_CHECK_ALL, m_AllCheck);
}

BOOL CTSessionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CTSessionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	LV_COLUMN lvcolumn;//컬럼 설정 구조체 
	int i; 
	TCHAR *list[2] = {_T("Protocol"), _T("Return Value")};
	int width[2] = {200, 500}; 
	for(i=0;i<2;i++) 
	{ 
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        lvcolumn.fmt = LVCFMT_LEFT; 
		lvcolumn.pszText = list[i]; 
		lvcolumn.iSubItem = i; 
		lvcolumn.cx = width[i]; 
		m_listResult.InsertColumn(i, &lvcolumn); 
	} 
	m_listResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	InitList();

	SetDlgItemText(IDC_EDIT_TOTAL, _T("0"));
	SetDlgItemText(IDC_EDIT_CONNECT, _T("0"));	
}


// CTSessionView 진단

#ifdef _DEBUG
void CTSessionView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTSessionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTSessionDoc* CTSessionView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTSessionDoc)));
	return (CTSessionDoc*)m_pDocument;
}
#endif //_DEBUG


// CTSessionView 메시지 처리기

///////////////////////////////////////////////////////////////////////////////////
// Manager Connect / DisConnect
void CTSessionView::ManagerConnect(CString strIP, CString strPort, CString strUser, CString strPass)
{
	DWORD wPort = atoi(strPort);

	CTSessionDoc* pDoc = GetDocument();

	pDoc->CTSvrConnect(strIP, wPort, strUser, strPass);
}
void CTSessionView::ManagerDisConnect()
{
	CTSessionDoc* pDoc = GetDocument();
	pDoc->CTSvrDisConnect();

	m_comboThread.ResetContent();
	ClearList();

	SetDlgItemText(IDC_EDIT_PROTOCOL, _T(""));
	
	SetDlgItemText(IDC_EDIT_TOTAL, _T("0"));
	SetEditConnect();
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Sever Connect / DisConnect
void CTSessionView::Connect(CString strIP, CString strPort, CString strThread)
{
	CTSessionDoc* pDoc = GetDocument();
	if( !pDoc->m_bManagerConnect )
	{
		AfxMessageBox(_T("관리자 인증이 확인되지 않았습니다."));
		return;
	}

	SetDlgItemText(IDC_EDIT_TOTAL, strThread);	

	pDoc->Connect(strIP, strPort, strThread);
}
void CTSessionView::DisConnect()
{
	ClearList();

	CTSessionDoc* pDoc = GetDocument();	
	pDoc->DisConnect();

	m_comboThread.ResetContent();	
	pDoc->ShowCharInfo();

	SetDlgItemText(IDC_EDIT_PROTOCOL, _T(""));
	
	SetDlgItemText(IDC_EDIT_TOTAL, _T("0"));
	SetEditConnect();
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Protocol List Drag
CRect CTSessionView::GetEditPosition()
{
	CRect editRect;
	GetDlgItem(IDC_EDIT_PROTOCOL)->GetWindowRect(&editRect);	
	return editRect;
}
void CTSessionView::SetEditProtocol(CString str)
{
	if( GetDocument()->m_mapSession.size() == 0 )
	{
		AfxMessageBox(_T("현재 열려있는 세션이 없습니다."));
		return;
	}
	if( strstr(str, _T("_ACK")) )
	{
		AfxMessageBox(_T("추가 할 수 없는 프로토콜입니다."));
		return;
	}
	SetDlgItemText(IDC_EDIT_PROTOCOL, str);
}

// All Check
void CTSessionView::OnBnClickedCheckAll()
{
	int stat = m_AllCheck.GetCheck();

	switch(stat)
	{
	case BST_UNCHECKED:
		m_bAllFlag = FALSE;
		break;
	case BST_CHECKED:
		m_bAllFlag = TRUE;
		break;
	}	
}

// Protocol Send
void CTSessionView::OnBnClickedButtonSend()
{
	CTSessionDoc* pDoc = GetDocument();	
	if( !pDoc->m_bManagerConnect )
	{
		AfxMessageBox(_T("관리자 인증이 확인되지 않았습니다."));
		return;
	}
	if( pDoc->m_mapSession.size() == 0 )
	{
		AfxMessageBox(_T("현재 열려있는 세션이 없습니다."));
		return;
	}

	if( pDoc->GetMacroFlag() )
	{
		AfxMessageBox(_T("매크로가 실행중 입니다."));
		return;
	}
    
	CString str;
	CString strP;
	CString strParam;

	GetDlgItemText(IDC_EDIT_PROTOCOL, str);

	int pos = str.Find(_T(" "));
	if( pos < 0 ) // Param Empty
	{
		strP = str;
		strParam = _T("");
	}
	else
	{
        strP = str.Left(pos);
		strParam = str.Mid(pos+1);
	}

    DWORD pID = 0;

	MAPPROTOCOL::iterator it;
	for(it = pDoc->m_mapProtocol.begin(); it != pDoc->m_mapProtocol.end(); it++)
	{
		if( strP == (*it).second )
		{
			pID = (*it).first;
			break;
		}
	}

	if( !pID )
	{
		AfxMessageBox(_T("프로토콜을 찾을 수 없습니다."));
		return;
	}
	
	pDoc->Send(pID, strParam, m_bAllFlag);
}
// Receive Message Value Init / Insert / Clear
void CTSessionView::InitList()
{
	for(int i = 0; i < LIST_MAX; i++)
	{
        int count = m_listResult.GetItemCount();
		LVITEM item; 
		item.mask = LVIF_TEXT; 
		item.iItem = count; 
		item.iSubItem = 0; 
		item.pszText = _T("");
		m_listResult.InsertItem(&item);

		item.iSubItem = 1;
		item.pszText = _T("");
		m_listResult.SetItem(&item);
	}
}
void CTSessionView::InsertList(CTachyonSession* pSession)
{	
	for(int i = 0; i < (int)pSession->m_dqList.size(); i++)
	{
		STRUCTSTRING temp = pSession->PopList(i);
		m_listResult.SetItemText(i, 0, temp.strProtocol);
		m_listResult.SetItemText(i, 1, temp.strValue);		
	}

	while( i < LIST_MAX )
	{
		m_listResult.SetItemText(i, 0, _T(""));
		m_listResult.SetItemText(i, 1, _T(""));
		i++;
	}
}
void CTSessionView::ClearList()
{
	for(int i = 0; i < LIST_MAX; i++)
	{        
		m_listResult.SetItemText(i, 0, _T(""));
		m_listResult.SetItemText(i, 1, _T(""));
	}
}
void CTSessionView::OnBnClickedListClear()
{
	ClearList();	
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Init ComboBox
void CTSessionView::SetComboThread(DWORD bID)
{
	CString str;
	str.Format(_T("Thread (%d)"), bID);
	int index = m_comboThread.AddString(str);
	m_comboThread.SetItemData(index, bID);
	m_comboThread.SetCurSel(0);	
}
// ComboBox Current Thread Number Get
int CTSessionView::GetIndexCurrentComboThread()
{
	return m_comboThread.GetCurSel();
}
int CTSessionView::GetDataCurrentComboThread()
{
	int CurSel = m_comboThread.GetCurSel();
	if(CurSel < 0)
	{
		AfxMessageBox(_T("쓰레드를 선택해주세요."));
		return CurSel;
	}
	return int(m_comboThread.GetItemData(CurSel));
}
int CTSessionView::GetIndexComboThread(DWORD dwID)
{
	for(int i = 0; i < m_comboThread.GetCount(); i++)
	{
		if( m_comboThread.GetItemData(i) == dwID )
			return i;
	}
	return -1;
}
int CTSessionView::GetDataComboThread(int index)
{
	return (int)m_comboThread.GetItemData(index);
}

void CTSessionView::OnCbnSelchangeComboThread()
{		
	if( !GetDocument()->m_bRun )
		return;
	
	GetDocument()->PostCharinfoView();
	GetDocument()->PostSessionlistView();	
}
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Connect Edit Box Set
void CTSessionView::SetEditConnect()
{
	CString str;
	str.Format(_T("%d"), m_comboThread.GetCount());
	SetDlgItemText(IDC_EDIT_CONNECT, str);
}
///////////////////////////////////////////////////////////////////////////////////