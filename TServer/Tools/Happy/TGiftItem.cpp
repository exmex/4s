// TGiftItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "TGiftItem.h"
#include "MainFrm.h"
#include "HappyDoc.h"

// CTGiftItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTGiftItem, CDialog)
CTGiftItem::CTGiftItem(CWnd* pParent /*=NULL*/)
	: CDialog(CTGiftItem::IDD, pParent),
	m_pDlgGiftSet(NULL)
{
}

CTGiftItem::~CTGiftItem()
{
	if(m_pDlgGiftSet)
	{
		m_pDlgGiftSet->DestroyWindow();
		delete m_pDlgGiftSet;
		m_pDlgGiftSet = NULL;
	}
}

void CTGiftItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIFT_WORLD, m_cbWorld);
}


BEGIN_MESSAGE_MAP(CTGiftItem, CDialog)
	ON_BN_CLICKED(IDC_GIFT_SET, OnBnClickedGiftSet)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTGiftItem 메시지 처리기입니다.

void CTGiftItem::OnBnClickedGiftSet()
{
	// Send List Req
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	BYTE bGroup;
	if(GetWorld(bGroup))
	{
		CreateGiftSet();
		m_pDlgGiftSet->m_bGroup = bGroup;

		pDoc->SendCT_CMGIFTLIST_REQ(bGroup);	
	
	}
	
}

void CTGiftItem::OnBnClickedOk()
{
	//	Send Gift Req	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	BYTE bGroup;
	if(!GetWorld(bGroup))
		return;

	WORD wGiftID = GetDlgItemInt(IDC_GIFT_GID);
	CString strTarget;
	GetDlgItemText(IDC_GIFT_NAME, strTarget);
	
	pDoc->SendCT_CMGIFT_REQ(strTarget, wGiftID, bGroup);
}

void CTGiftItem::Init()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	if(!m_cbWorld.GetCount())
	{
		m_cbWorld.AddString(_T("SELECT"));

		//	월드서버 리스트 구하기
		MAPGROUP::iterator itG;
		for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		{	
			if( (*itG).second->m_bID != 0 )
				m_cbWorld.AddString((*itG).second->m_strName);
		}
	}
	m_cbWorld.SetCurSel(0);
	
	//	텍스트 박스 초기화
	SetDlgItemText(IDC_GIFT_NAME, _T(""));
	SetDlgItemText(IDC_GIFT_GID, _T(""));
}

BYTE CTGiftItem::GetWorld(BYTE& p_bOut)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return false;
	}

	CString str = _T("");
	m_cbWorld.GetLBText(m_cbWorld.GetCurSel(),str);

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
	{
		if((*itG).second->m_strName == str)
			break;
		if( str == STR_WORLD_ALL )
		{
			p_bOut = 0;
			return true;
		}
	}

	if( itG == pDoc->m_mapGroup.end() )
		return false;

	p_bOut = (*itG).second->m_bID;

	return true;
}

void CTGiftItem::ShowSetting(BYTE bShow)
{	
	CreateGiftSet();

	m_pDlgGiftSet->SetData();
	m_pDlgGiftSet->ShowWindow(bShow);
}

void CTGiftItem::ReturnMsg(BYTE bRet)
{
	switch(bRet)
	{
	case CMGIFT_SUCCESS:	AfxMessageBox(_T("GIFT SUCCESS"));return;
	case CMGIFT_TARGET:		AfxMessageBox(_T("Invalid Target"));return;
	case CMGIFT_ID:			AfxMessageBox(_T("Invalid GiftID"));return;
	case CMGIFT_DUPLICATE:	AfxMessageBox(_T("Duplicate Gift"));return;
	case CMGIFT_ERRPOST:	AfxMessageBox(_T("Duplicate Gift\n Sent Err Post"));return;
	case CMGIFT_FAIL:		AfxMessageBox(_T("FAILED"));return;
	}
}

LRESULT CTGiftItem::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CMGIFTRET)
		ReturnMsg((BYTE)wParam);

	return CDialog::WindowProc(message, wParam, lParam);
}

LPMAPCMGIFT CTGiftItem::GetGiftItemMap()
{
	CreateGiftSet();

	return &m_pDlgGiftSet->m_mapCMGift;
}


void CTGiftItem::CreateGiftSet()
{
	if(!m_pDlgGiftSet)
	{
		m_pDlgGiftSet = new CTGiftSet;
		m_pDlgGiftSet->Create(IDD_GIFTSET);
	}
	m_pDlgGiftSet->InitList();
}
