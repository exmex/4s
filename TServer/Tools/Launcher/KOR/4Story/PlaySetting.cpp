// PlaySetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "4Story.h"
#include "PlaySetting.h"
#include ".\playsetting.h"

#include "GameSetting.h"

// CPlaySetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPlaySetting, CDialog)
CPlaySetting::CPlaySetting(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaySetting::IDD, pParent)	
	, m_bNPCName(FALSE)
	, m_bCharName(FALSE)
	, m_bMonName(FALSE)
	, m_bAutoHelp(FALSE)
	, m_bTalkBox(FALSE)
	, m_bHUD(FALSE)
	, m_bDenyWhisper(FALSE)
	, m_bDenyCom(FALSE)
	, m_bConChat(FALSE)		
	, m_bClickMove(FALSE)
{

}

CPlaySetting::~CPlaySetting()
{
}

void CPlaySetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_NPCNAME, m_chkNPCName);
	DDX_Control(pDX, IDC_CHK_CHARNAME, m_chkCharName);
	DDX_Control(pDX, IDC_CHK_MONNAME, m_chkMonName);
	DDX_Control(pDX, IDC_CHK_AUTOHELP, m_chkAutoHelp);
	DDX_Control(pDX, IDC_CHK_TALKBOX, m_chkTalkBox);
	DDX_Control(pDX, IDC_CHK_HUD, m_chkHUD);
	DDX_Control(pDX, IDC_CHK_DENYWHI, m_chkDenyWhisper);
	DDX_Control(pDX, IDC_CHK_DENYCOM, m_chkDenyCommunity);
	DDX_Control(pDX, IDC_CHK_CONCHAT, m_chkConChat);
	DDX_Control(pDX, IDC_CLOSE_PLAYSET, m_btnClose);
	DDX_Control(pDX, IDC_BTN_SET, m_btnSys);
	DDX_Control(pDX, IDC_CHK_CLICKMOVE, m_chkClickMove);

	DDX_Check(pDX, IDC_CHK_NPCNAME, m_bNPCName);
	DDX_Check(pDX, IDC_CHK_CHARNAME, m_bCharName);
	DDX_Check(pDX, IDC_CHK_MONNAME, m_bMonName);
	DDX_Check(pDX, IDC_CHK_AUTOHELP, m_bAutoHelp);
	DDX_Check(pDX, IDC_CHK_TALKBOX, m_bTalkBox);
	DDX_Check(pDX, IDC_CHK_HUD, m_bHUD);
	DDX_Check(pDX, IDC_CHK_DENYWHI, m_bDenyWhisper);
	DDX_Check(pDX, IDC_CHK_DENYCOM, m_bDenyCom);
	DDX_Check(pDX, IDC_CHK_CONCHAT, m_bConChat);
	DDX_Check(pDX, IDC_CHK_CLICKMOVE, m_bClickMove);	
}


BEGIN_MESSAGE_MAP(CPlaySetting, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_SET, OnBnClickedBtnSetTab)
	ON_BN_CLICKED(IDC_CLOSE_PLAYSET, OnBnClickedClosePlayset)
	ON_BN_CLICKED(IDC_CHK_NPCNAME, OnBnClickedChkNpcname)
	ON_BN_CLICKED(IDC_CHK_CHARNAME, OnBnClickedChkCharname)
	ON_BN_CLICKED(IDC_CHK_MONNAME, OnBnClickedChkMonname)
	ON_BN_CLICKED(IDC_CHK_AUTOHELP, OnBnClickedChkAutohelp)
	ON_BN_CLICKED(IDC_CHK_TALKBOX, OnBnClickedChkTalkbox)
	ON_BN_CLICKED(IDC_CHK_HUD, OnBnClickedChkHud)
	ON_BN_CLICKED(IDC_CHK_DENYWHI, OnBnClickedChkDenywhi)
	ON_BN_CLICKED(IDC_CHK_DENYCOM, OnBnClickedChkDenycom)
	ON_BN_CLICKED(IDC_CHK_CONCHAT, OnBnClickedChkConchat)
	ON_BN_CLICKED(IDC_CHK_CLICKMOVE, OnBnClickedChkClickmove)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	
END_MESSAGE_MAP()


// CPlaySetting 메시지 처리기입니다.


void CPlaySetting::LoadBGSkin()
{
	sFlag = m_bmpBG.LoadBitmap(IDB_TABGAME1);

	if(sFlag)
	{
		CRect rect,WinRect;
		GetClientRect(rect);
		GetWindowRect(WinRect);

		BITMAP bmp;
		m_bmpBG.GetBitmap(&bmp);

		m_iBGWidth = bmp.bmWidth;
		m_iBGHeight = bmp.bmHeight;

		int PosX = WinRect.left + rect.right/2;
		int PosY = WinRect.top;

		SetWindowPos( NULL, PosX, PosY, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	}
}

void CPlaySetting::LoadControlSkin()
{
	// 각 버튼들 스킨 입히기
	m_chkNPCName.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);  
	m_chkCharName.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);	
	m_chkMonName.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);

	m_chkAutoHelp.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);
	m_chkTalkBox.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);
	m_chkHUD.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);

	m_chkDenyWhisper.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);
	m_chkDenyCommunity.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);
	m_chkConChat.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);
	m_chkClickMove.SetSkin(IDB_CHK_NORMAL2,IDB_CHK_DOWN2,0,0,0,0,1,0,0);

	m_btnClose.SetSkin(IDB_CLOSE_NORMAL,IDB_CLOSE_DOWN,IDB_CLOSE_HOVER,0,0,0,1,0,0);
	m_btnSys.SetSkin(IDB_BTN_SYSTAB,IDB_BTN_SYSTAB,IDB_BTN_SYSTAB,0,0,0,1,0,0);
}

void CPlaySetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

}

BOOL CPlaySetting::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDefaultOption();
	ReadRegistry();

	// 배경 스킨 로드
	LoadBGSkin();
	
	// 각 버튼들 스킨 입히기
	LoadControlSkin();

	// 스킨 입힌 다이얼로그 투명색 적용
	TransparentDialog(&m_bmpBG,RGB(255,255,255) );
	//TransparentDialog2(RGB(255,255,255) );

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPlaySetting::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 마우스 왼쪽버튼 드래그로 다이얼로그 자유롭게 이동
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CPlaySetting::OnBnClickedBtnSetTab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	WriteRegistry();

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	
	CGameSetting*	pGameSetting = CGameSetting::GetInstance();
	pGameSetting->SetWindowPos(0,rect.left, rect.top, 0,0, SWP_NOSIZE | SWP_SHOWWINDOW);

	ShowWindow(SW_HIDE);
}

BOOL CPlaySetting::ReadRegistry()
{
	HKEY hKeyRet;	
	HKEY hKey = HKEY_CURRENT_USER;
	

	int err = 0;
	CString strSubkey;
	CString strRegSubKey;
	CString strAppName;
	CString strBuf;
	
	
	strAppName = APP_NAME;	
	strSubkey = _T("");
	strSubkey = strAppName + REG_COUNTRY;
	strSubkey += _T("\\Settings");

	strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

	err = RegOpenKey(hKey, strRegSubKey, &hKeyRet);
	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		hKey = HKEY_LOCAL_MACHINE;		
	}

	BYTE	data[1024];
	DWORD   type;
	DWORD   cbdata =1024;

	// NPC 이름
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("NpcNAME") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bNPCName = *((LPWORD)data);

	// 캐릭터 이름
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("PcNAME") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bCharName = *((LPWORD)data);

	// 몬스터 이름
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("MonNAME") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bMonName = *((LPWORD)data);

	// 자동 도움말
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("AUTOHELP") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bAutoHelp = *((LPWORD)data);

	// 말풍선 보이기
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("TALKBOX") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bTalkBox = *((LPWORD)data);

	// HUD 보이기
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("HUD") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bHUD = *((LPWORD)data);

	// 귓말 거부
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("DENYWHI") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bDenyWhisper = *((LPWORD)data);

	// 커뮤니티 거부
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("DENYCOM") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bDenyCom = *((LPWORD)data);

	// 연속채팅
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("CONCHAT") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bConChat = *((LPWORD)data);

	// 마우스 클릭 이동
	cbdata = 1024;
	memset(data, 0, 1024);
	err = RegQueryValueEx(hKeyRet, _T("MOUSECLICKMOVE") , NULL, &type, data, &cbdata);
	if( ERROR_SUCCESS != err || type != REG_DWORD)
		return FALSE;
	m_bClickMove = *((LPWORD)data);

	return TRUE;
}

BOOL CPlaySetting::WriteRegistry()
{
	HKEY hKeyRet;	
	HKEY hKey = HKEY_CURRENT_USER;

	int err = 0;
	DWORD dwDisposition = 0;
	CString strSubkey;
	CString strRegSubKey;
	CString strAppName;

	strAppName = APP_NAME;	
	strSubkey = _T("");
	strSubkey = strAppName + REG_COUNTRY;
	strSubkey += _T("\\Settings");

	strRegSubKey.Format(_T("%s%s"), REG_SUBKEY, strSubkey);

//	err = RegOpenKey(hKey, strRegSubKey, &hKeyRet);
	err = RegCreateKeyEx( hKey, strRegSubKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
		&hKeyRet, &dwDisposition );

	if(ERROR_SUCCESS != err)
	{
		RegCloseKey(hKeyRet);
		hKey = HKEY_LOCAL_MACHINE;		
	}

	char	strTmp[1024] = {0,};
	BYTE	data[1024] = {0};	
	DWORD   cbData = 4;
	DWORD	cbData1 = sizeof(BOOL);

	// NPC 이름
	memcpy(data, &m_bNPCName, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("NpcNAME"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 캐릭터 이름
	memcpy(data, &m_bCharName, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("PcNAME"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 몬스터 이름
	memcpy(data, &m_bMonName, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("MonNAME"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 자동 도움말
	memcpy(data, &m_bAutoHelp, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("AUTOHELP"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 말풍선 보이기
	memcpy(data, &m_bTalkBox, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("TALKBOX"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// HUD 보이기
	memcpy(data, &m_bHUD, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("HUD"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 귓말 거부
	memcpy(data, &m_bDenyWhisper, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("DENYWHI"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 커뮤니티 거부
	memcpy(data, &m_bDenyCom, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("DENYCOM"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 연속채팅 지원
	memcpy(data, &m_bConChat, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("CONCHAT"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	// 마우스 클릭 이동
	memcpy(data, &m_bClickMove, sizeof(BOOL));
	err = RegSetValueEx(hKeyRet, _T("MOUSECLICKMOVE"), 0, REG_DWORD, data, cbData1 );
	if( ERROR_SUCCESS != err )
		return FALSE;

	return TRUE;
}

void CPlaySetting::OnBnClickedClosePlayset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
	WriteRegistry();
}

void CPlaySetting::OnBnClickedChkNpcname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkCharname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkMonname()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkAutohelp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkTalkbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkHud()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkDenywhi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkDenycom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkConchat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnBnClickedChkClickmove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void CPlaySetting::OnDestroy()
{
	CDialog::OnDestroy();
}

void CPlaySetting::TransparentDialog(CBitmap* _pCBitmap, COLORREF  _color)
{
	// 다이얼로그 배경 비트맵 투명색 적용.	
	COLORREF crTransparent = _color;
	BITMAP Bitmap;	
	_pCBitmap->GetBitmap(&Bitmap);

	
	CPaintDC dc(this);	
	CDC dcMem;
	
	dcMem.CreateCompatibleDC(&dc);	
	CBitmap* pOldBitmap = dcMem.SelectObject(_pCBitmap);
	
	CRgn crRgn, crRgnTmp;
	
	crRgn.CreateRectRgn(0, 0, 0, 0);
		
	int iX = 0;
	for (int iY = 0; iY < Bitmap.bmHeight; iY++)
	{
		do
		{			
			while (iX <= Bitmap.bmWidth && dcMem.GetPixel(iX, iY) == crTransparent)
				iX++;
			
			int iLeftX = iX;
			
			while (iX <= Bitmap.bmWidth && dcMem.GetPixel(iX, iY) != crTransparent)
				++iX;
			
			crRgnTmp.CreateRectRgn(iLeftX, iY, iX, iY+1);
			
			crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
			
			crRgnTmp.DeleteObject();
		}while(iX < Bitmap.bmWidth);
		iX = 0;
	}
	
	SetWindowRgn(crRgn, TRUE);
	iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (Bitmap.bmWidth / 2);
	iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (Bitmap.bmHeight / 2);
	SetWindowPos(&wndTopMost, iX, iY, Bitmap.bmWidth, Bitmap.bmHeight, NULL); 
	
	dcMem.SelectObject(pOldBitmap);
	dcMem.DeleteDC();
	crRgn.DeleteObject();
}

void CPlaySetting::TransparentDialog2(COLORREF _color)
{
	int window_style = GetWindowLong(m_hWnd, GWL_EXSTYLE); 

	if(!(window_style & WS_EX_LAYERED))
	{ 
		SetWindowLong(m_hWnd, GWL_EXSTYLE, window_style | WS_EX_LAYERED); 
	} 

	HMODULE h_user32_dll = GetModuleHandle("USER32.DLL"); 

	if(h_user32_dll != NULL)
	{ 
		BOOL (WINAPI *fp_set_layered_window_attributes)(HWND, COLORREF, BYTE, DWORD) = 

			(BOOL (WINAPI *)(HWND, COLORREF, BYTE, DWORD))GetProcAddress(h_user32_dll, 

			"SetLayeredWindowAttributes"); 

		if(fp_set_layered_window_attributes != NULL)
		{ 
			(*fp_set_layered_window_attributes)(m_hWnd, _color, 0, LWA_COLORKEY); 
		} 

	} 
}


// 배경이 지워지면 다시 그린다.(WM_ERASEBKGND)
BOOL CPlaySetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 다이얼로그 배경 그림 입히기
	CRect rect;
	GetClientRect(&rect);

	CDC MemDC;
	CRect rc;
	GetClientRect(rc);	
	MemDC.CreateCompatibleDC(pDC);
	int nSavedDC = MemDC.SaveDC();
	
	BITMAP bmp;	
	m_bmpBG.GetBitmap(&bmp);
	CBitmap* pOldBmp = MemDC.SelectObject(&m_bmpBG);	
	pDC->StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &MemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	MemDC.SelectObject(pOldBmp);
	MemDC.RestoreDC(nSavedDC);
	MemDC.DeleteDC();
	ReleaseDC(pDC);

	return CDialog::OnEraseBkgnd(pDC);
}

void CPlaySetting::SetDefaultOption()
{
	m_strCountry = COUNTRY_CODE;

	if(m_strCountry == _T("GERMANY") )
	{
		switch(theApp.m_bOptionLevel)
		{
		case OPTION_LOW:
			{
				m_bNPCName		= FALSE;
				m_bCharName		= FALSE;
				m_bMonName		= FALSE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;

		case OPTION_MID:
			{
				m_bNPCName		= FALSE;
				m_bCharName		= FALSE;
				m_bMonName		= FALSE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;

		case OPTION_HI:
			{
				m_bNPCName		= FALSE;
				m_bCharName		= FALSE;
				m_bMonName		= FALSE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;
		}
	}
	else
	{
		switch(theApp.m_bOptionLevel)
		{
		case OPTION_LOW:
			{
				m_bNPCName		= TRUE;
				m_bCharName		= TRUE;
				m_bMonName		= TRUE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;

		case OPTION_MID:
			{
				m_bNPCName		= TRUE;
				m_bCharName		= TRUE;
				m_bMonName		= TRUE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;

		case OPTION_HI:
			{
				m_bNPCName		= TRUE;
				m_bCharName		= TRUE;
				m_bMonName		= TRUE;
				m_bAutoHelp		= TRUE;
				m_bTalkBox		= TRUE;
				m_bHUD			= TRUE;
				m_bDenyWhisper	= FALSE;
				m_bDenyCom		= FALSE;
				m_bConChat		= FALSE;
				m_bClickMove	= TRUE;
			}
			break;
		}
	}
}

CPlaySetting*	CPlaySetting::GetInstance()
{
	static CPlaySetting	inst;
	return &inst;
}
