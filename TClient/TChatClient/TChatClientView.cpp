// TChatClientView.cpp : CTChatClientView 클래스의 구현
//

#include "stdafx.h"
#include "TChatClient.h"

#include "TChatClientDoc.h"
#include "TChatClientView.h"
#include "TChatShopDlg.h"
#include "ShopNameDlg.h"
#include "TLoginDlg.h"
#include "MainFrm.h"
#include "NetCode.h"
#include ".\tchatclientview.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TCHAT_MARGINE	4
#define TCHAT_CY		20

// CTChatClientView

IMPLEMENT_DYNCREATE(CTChatClientView, CView)

BEGIN_MESSAGE_MAP(CTChatClientView, CView)
	// 표준 인쇄 명령입니다.
	ON_MESSAGE(WM_SESSION_MSG, OnSessionMsg)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_CON_LOGIN, OnUpdateConLogin)
	ON_COMMAND(ID_CON_LOGIN, OnConLogin)
	ON_UPDATE_COMMAND_UI(ID_CON_LOGOUT, OnUpdateConLogout)
	ON_COMMAND(ID_CON_LOGOUT, OnConLogout)
	ON_UPDATE_COMMAND_UI(ID_CHATTAB_ALL, OnUpdateChattabAll)
	ON_COMMAND(ID_CHATTAB_ALL, OnChattabAll)
	ON_UPDATE_COMMAND_UI(ID_CHATTAB_ANNOUNCE, OnUpdateChattabAnnounce)
	ON_COMMAND(ID_CHATTAB_ANNOUNCE, OnChattabAnnounce)
	ON_UPDATE_COMMAND_UI(ID_CHATTAB_NORMAL, OnUpdateChattabNormal)
	ON_COMMAND(ID_CHATTAB_NORMAL, OnChattabNormal)
	ON_UPDATE_COMMAND_UI(ID_CHATTAB_WISPER, OnUpdateChattabWisper)
	ON_COMMAND(ID_CHATTAB_WISPER, OnChattabWisper)
	ON_UPDATE_COMMAND_UI(ID_SET_TARGET, OnUpdateSetTarget)
	ON_COMMAND(ID_SET_TARGET, OnSetTarget)
	ON_UPDATE_COMMAND_UI(ID_SHOP_OPEN, OnUpdateShopOpen)
	ON_COMMAND(ID_SHOP_OPEN, OnShopOpen)
	ON_UPDATE_COMMAND_UI(ID_SHOP_CLOSE, OnUpdateShopClose)
	ON_COMMAND(ID_SHOP_CLOSE, OnShopClose)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_SHOP_AUTO_OPEN, OnUpdateShopAutoOpen)
	ON_COMMAND(ID_SHOP_AUTO_OPEN, OnShopAutoOpen)
	ON_COMMAND(ID_TOOL_PINGTEST, OnToolPingtest)
	ON_UPDATE_COMMAND_UI(ID_TOOL_PINGTEST, OnUpdateToolPingtest)
END_MESSAGE_MAP()

// CTChatClientView 생성/소멸

CTChatClientView::CTChatClientView()
{
	m_mapTSELLITEM.clear();
	m_mapTCHAR.clear();
	m_strTARGET.Empty();
	m_strNAME.Empty();

	m_pSOCKET = NULL;
	m_pRelay = NULL;
	m_bLogout = TRUE;
	m_bLogin = FALSE;
	m_bShop = FALSE;
	m_bOpen = FALSE;

	m_cEditCtrl.m_pHost = this;
	m_bGroupID = 0;
	m_bChannel = 0;

	m_bChatType = CHAT_NEAR;
	m_bChatView = 0;

	ClearTCHATMSG();
	ClearTINVEN();

	m_pdlgPingTest = new CPingTestDlg();
	m_pdlgPingTest->Create(IDD_PINGTEST);
	m_bPingTestView = FALSE;
	m_bPWEncrypt = FALSE;
}

CTChatClientView::~CTChatClientView()
{
	m_mapTSELLITEM.clear();
	ClearTCHATMSG();
	ClearTINVEN();
	ReleaseTITEMTEMP();

	if(m_pdlgPingTest)
	{
		m_pdlgPingTest->DestroyWindow();
		delete m_pdlgPingTest;
		m_pdlgPingTest = NULL;
	}
}

BOOL CTChatClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTChatClientView 그리기

void CTChatClientView::OnDraw(CDC* /*pDC*/)
{
	CTChatClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
}


// CTChatClientView 인쇄

BOOL CTChatClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTChatClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTChatClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTChatClientView 진단

#ifdef _DEBUG
void CTChatClientView::AssertValid() const
{
	CView::AssertValid();
}

void CTChatClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTChatClientDoc* CTChatClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTChatClientDoc)));
	return (CTChatClientDoc *) m_pDocument;
}
#endif //_DEBUG


// CTChatClientView 메시지 처리기

void CTChatClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	for( BYTE i=0; i<MAX_CON; i++)
		m_vTSESSION[i].SetOwner(this);

	m_cListCtrl.Create(
		WS_CHILD|
		WS_VISIBLE|
		LVS_REPORT|
		LVS_NOSORTHEADER,
		CRect( 0, 0, -1, -1),
		this, 0);

	m_cEditCtrl.Create(
		WS_CHILD|
		WS_VISIBLE,
		CRect( 0, 0, -1, -1),
		this,
		NULL);

	m_cListCtrl.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		WS_EX_STATICEDGE);

	SetWindowLong(
		m_cEditCtrl.GetSafeHwnd(),
		GWL_EXSTYLE,
		WS_EX_STATICEDGE);

	m_cListCtrl.DeleteAllItems();
	m_cListCtrl.InsertColumn( 0, _T("Name"), LVCFMT_LEFT, 150);
	m_cListCtrl.InsertColumn( 1, _T("Chat"), LVCFMT_LEFT, 450);

	m_cListCtrl.EnableWindow(FALSE);
	m_cEditCtrl.EnableWindow(FALSE);
	m_mapTSELLITEM.clear();
	m_bLogout = TRUE;
	m_bLogin = FALSE;
	m_bShop = FALSE;
	m_bOpen = FALSE;
	SetChatTarget();
	InitTITEMTEMP();
}

void CTChatClientView::OnSize( UINT nType, int cx, int cy)
{
	if(m_cListCtrl.GetSafeHwnd())
		m_cListCtrl.MoveWindow( TCHAT_MARGINE, TCHAT_MARGINE, cx - 2 * TCHAT_MARGINE, cy - TCHAT_CY - 3 * TCHAT_MARGINE);

	if(m_cEditCtrl.GetSafeHwnd())
		m_cEditCtrl.MoveWindow( TCHAT_MARGINE, cy - TCHAT_CY - TCHAT_MARGINE, cx - 2 * TCHAT_MARGINE, TCHAT_CY);

	CView::OnSize( nType, cx, cy);
}

void CTChatClientView::OnUpdateEditCopy( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_cEditCtrl.GetSafeHwnd() && GetFocus() == &m_cEditCtrl ? TRUE : FALSE);
}

void CTChatClientView::OnEditCopy()
{
	m_cEditCtrl.Copy();
}

void CTChatClientView::OnUpdateEditPaste( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_cEditCtrl.GetSafeHwnd() && GetFocus() == &m_cEditCtrl ? TRUE : FALSE);
}

void CTChatClientView::OnEditPaste()
{
	m_cEditCtrl.Paste();
}

BOOL CTChatClientView::OnEraseBkgnd( CDC* pDC)
{
	CRect rect;

	GetClientRect(&rect);
	pDC->FillRect(
		&rect,
		&CBrush(GetSysColor(COLOR_3DFACE)));

	return TRUE;
}

LRESULT CTChatClientView::OnSessionMsg( WPARAM wParam, LPARAM lParam)
{
	CTChatSocket *pSocket = CTChatSocket::GetTChatSocket(wParam);

	if(pSocket)
	{
		int nError = WSAGETSELECTERROR(lParam);

		switch(WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT	: OnConnect( pSocket, nError); break;
		case FD_READ	: OnReceive( pSocket, nError); break;
		case FD_CLOSE	: OnClose( pSocket, nError); break;
		}
	}

	return 1;
}

void CTChatClientView::OnUpdateConLogin( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bLogout ? TRUE : FALSE);
}

void CTChatClientView::OnConLogin()
{
	CTLoginDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();

		if(m_vTSESSION[0].Start( dlg.m_strIPAddr, dlg.m_dwPort))
		{
			pFRAME->SetPaneText(IDS_CONNECT_LOGIN);
			m_vTSESSION[0].m_bSVR = SVR_LOGIN;

			m_strIP = dlg.m_strIPAddr;
			m_dwPort = dlg.m_dwPort;
			m_strID = dlg.m_strID;			
			m_bPWEncrypt = dlg.m_bPWEncrypt;
			m_bLogout = FALSE;
			
			if(m_bPWEncrypt)
				m_strPW = EncryptPW(dlg.m_strPW);
			else
				m_strPW = dlg.m_strPW;

		}
		else
		{
			pFRAME->SetPaneText(IDS_CANNOT_CREATE_SOCK);
			m_vTSESSION[0].End();
		}
	}
}

void CTChatClientView::OnUpdateConLogout( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnConLogout()
{
	for( BYTE i=0; i<MAX_CON; i++)
		if( m_vTSESSION[i].m_bSVR != SVR_NONE )
		{
			OnClose( &m_vTSESSION[i], 0);
			m_vTSESSION[i].End();
		}
}

void CTChatClientView::OnReceive( CTChatSocket *pTSocket, int nErrorCode)
{
	if( pTSocket->m_sock == INVALID_SOCKET || nErrorCode )
		return;

	if(pTSocket->m_packet.IsReadBufferFull())
		pTSocket->m_packet.ExpandIoBuffer(pTSocket->m_packet.GetSize());

	int nRead = recv(
		pTSocket->m_sock,
		(char *) pTSocket->m_packet.GetBuffer() + pTSocket->m_packet.GetReadBytes(),
		pTSocket->m_packet.m_dwBufferSize - pTSocket->m_packet.GetReadBytes(), 0);

	if( nRead == SOCKET_ERROR )
		return;

	if(!pTSocket->Read(nRead))
		return;

	while( pTSocket->CheckMSG() == PACKET_COMPLETE )
	{
		OnSessionMsg( pTSocket, &pTSocket->m_packet);
		pTSocket->Flush();
	}
}

void CTChatClientView::OnConnect( CTChatSocket *pTSocket, int nErrorCode)
{
	pTSocket->m_bValid = pTSocket->m_sock != INVALID_SOCKET && !nErrorCode ? TRUE : FALSE;
	CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();

	if(nErrorCode)
	{
		OnClose( pTSocket, nErrorCode);
		pTSocket->End();
		pFRAME->SetPaneText(IDS_CONNECT_FAILED);

		return;
	}

	switch(pTSocket->m_bSVR)
	{
	case SVR_LOGIN	:
		{	
			pTSocket->SendCS_LOGIN_REQ(
				m_strID,
				m_strPW,
				0);
			pFRAME->SetPaneText(IDS_LOGIN_INFO);
		}

		break;

	case SVR_MAP	:
		{
			pTSocket->SendCS_CONNECT_REQ(
				TVERSION,
				m_bChannel,
				m_dwUserID,
				m_dwCharID,
				m_dwKEY,
				pTSocket->m_target.sin_addr.S_un.S_addr,
				htons(pTSocket->m_target.sin_port));

			m_strIP = inet_ntoa(pTSocket->m_target.sin_addr) ;			
			m_dwPort = ntohs(pTSocket->m_target.sin_port);

		}

		break;

	case SVR_RELAY	:
		{
			pTSocket->SendCS_RELAYCHARDATA_REQ(
				TVERSION,
				m_dwCharID,
				m_strNAME);
		}

		break;
	}
}

void CTChatClientView::OnClose( CTChatSocket *pTSocket, int nErrorCode)
{
	if(pTSocket->m_bSVR == SVR_RELAY)
	{
		if(m_pRelay = pTSocket)
		{
			m_pRelay = NULL;
			pTSocket->m_bSVR = SVR_NONE;
			pTSocket->m_bSvrID = 0;
			pTSocket->End();
		}
		return;
	}

	BYTE bLogout = TRUE;

	pTSocket->m_bSVR = SVR_NONE;
	pTSocket->m_bSvrID = 0;
	pTSocket->End();

	for( int i=0; i<MAX_CON; i++)
		if( m_vTSESSION[i].m_bSVR != SVR_NONE && m_vTSESSION[i].m_bSVR != SVR_RELAY)
		{
			bLogout = FALSE;
			break;
		}

	if(bLogout)
	{
		m_mapTCHAR.clear();

		if(m_pRelay)
			OnClose(m_pRelay, 0);

		CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();

		m_cListCtrl.EnableWindow(FALSE);
		m_cEditCtrl.EnableWindow(FALSE);
		m_mapTSELLITEM.clear();
		m_bLogout = TRUE;
		m_bLogin = FALSE;
		m_bShop = FALSE;
		m_bOpen = FALSE;

		pFRAME->SetPaneText(_T(""));
		SetChatTarget();
		GetDocument()->SetTitle(_T("Untitled"));
		ShowWindow(SW_SHOW);
	}
}

void CTChatClientView::OnSessionMsg( CTChatSocket *pTSocket, CPacket *pPacket)
{
	if(!pPacket)
		return;

	DWORD dwID = pPacket->GetID();
	int nERROR = FALSE;

	switch(dwID)
	{
	ON_TRECEIVE(CS_INVALIDCHAR_ACK)
	ON_TRECEIVE(CS_ADDCONNECT_ACK)
	ON_TRECEIVE(CS_SHUTDOWN_ACK)
	ON_TRECEIVE(CS_CONNECT_ACK)

	ON_TRECEIVE(CS_CHANNELLIST_ACK)
	ON_TRECEIVE(CS_GROUPLIST_ACK)
	ON_TRECEIVE(CS_CHARLIST_ACK)
	ON_TRECEIVE(CS_LOGIN_ACK)
	ON_TRECEIVE(CS_START_ACK)
	ON_TRECEIVE(CS_CHAT_ACK)

	ON_TRECEIVE(CS_STOREOPEN_ACK)
	ON_TRECEIVE(CS_STORECLOSE_ACK)
	ON_TRECEIVE(CS_STOREITEMLIST_ACK)
	ON_TRECEIVE(CS_STOREITEMSELL_ACK)
	
	ON_TRECEIVE(CS_CHARINFO_ACK)
	
	ON_TRECEIVE(CS_RELAYCONNECT_ACK)
	ON_TRECEIVE(CS_RELAYCHARDATA_ACK)
	ON_TRECEIVE(CS_ENTER_ACK)
	ON_TRECEIVE(CS_LEAVE_ACK)
	ON_TRECEIVE(CT_SERVICEMONITOR_REQ)
	}

	if(nERROR)
		OnERROR(nERROR);
}

void CTChatClientView::OnERROR( int nERROR)
{
	CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();
	pFRAME->SetPaneText(nERROR);
}

CTChatSocket *CTChatClientView::GetIdleSession()
{
	for( BYTE i=1; i<MAX_CON; i++)
		if( m_vTSESSION[i].m_bSVR == SVR_NONE )
			return &m_vTSESSION[i];

	return NULL;
}

void CTChatClientView::SayToSvr( CPacket& packet, BYTE bServerID)
{
	for( int i=0; i<MAX_CON; i++)
		if( m_vTSESSION[i].m_bSvrID == bServerID )
		{
			m_vTSESSION[i].Say(&packet);
			return;
		}
}

void CTChatClientView::SayToAll( CPacket& packet)
{
	for( int i=0; i<MAX_CON; i++)
		if( m_vTSESSION[i].m_bSVR == SVR_MAP )
		{
			CPacket copy;
			copy.Copy(&packet);
			m_vTSESSION[i].Say(&copy);
		}
}

void CTChatClientView::SetChatTarget()
{
	static CString strTARGET[] = {
		_T(""),
		_T("General"),
		_T("All"),
		_T("Notifications"),
		_T("Party"),
		_T("Guild"),
		_T("Corps"),
		_T("Infomation")};
	CMainFrame *pFRAME = (CMainFrame *) GetParentFrame();

	if( m_bChatType == CHAT_WHISPER )
		pFRAME->SetTarget(m_strTARGET);
	else
		pFRAME->SetTarget(strTARGET[m_bChatType]);
}

void CTChatClientView::Chat( CString strTMSG)
{
	RelaySocket()->SendCS_CHAT_REQ(
		m_strNAME,
		m_bChatType, 0,
		m_strTARGET,
		strTMSG,
		m_bContryID,
		m_mapTCHAR);
}

void CTChatClientView::ClearTCHATMSG()
{
	while(!m_vTCHATMSG.empty())
	{
		delete m_vTCHATMSG.back();
		m_vTCHATMSG.pop_back();
	}
}

void CTChatClientView::ClearTINVEN()
{
	MAPTINVEN::iterator itTINVEN;

	for( itTINVEN = m_mapTINVEN.begin(); itTINVEN != m_mapTINVEN.end(); itTINVEN++)
		delete (*itTINVEN).second;

	m_mapTINVEN.clear();
}

void CTChatClientView::ResetTCHATMSG()
{
	m_cListCtrl.DeleteAllItems();

	for( int i=0; i<INT(m_vTCHATMSG.size()); i++)
		AddChatMSG(m_vTCHATMSG[i]);

	if( m_cListCtrl.GetItemCount() > 0 )
		m_cListCtrl.EnsureVisible( m_cListCtrl.GetItemCount() - 1, FALSE);
}

void CTChatClientView::AddChatMSG( LPTCHATMSG pTMSG)
{
	static CString strTARGET[] = {
		_T("Whisper"),
		_T("General"),
		_T("All"),
		_T("Notifications"),
		_T("Party"),
		_T("Guild"),
		_T("Corps"),
		_T("Infomation")};

	static DWORD dwFlags[] = {
		TCHAT_FLAG_ALL_CHAT_WITH_INFO,
		TCHAT_FLAG_ALL_CHAT,
		TCHAT_FLAG_WHISPER,
		TCHAT_FLAG_WORLD};

	if( pTMSG && (pTMSG->m_dwFlags & dwFlags[m_bChatView]) )
	{
		CString strNAME;
		CString strMSG;

		strNAME.Format( _T("[%s]%s"), strTARGET[pTMSG->m_bType], pTMSG->m_strNAME);
		int nIndex = m_cListCtrl.InsertItem( m_cListCtrl.GetItemCount(), strNAME);
		strMSG = pTMSG->m_strMSG;

		while( strMSG.GetLength() > 64 )
		{
			int nPOS = 0;

			while(nPOS < 64)
				if(MBCS_LEAD_TEST(strMSG[nPOS]))
					nPOS += 2;
				else
					nPOS++;

			m_cListCtrl.SetItemText( nIndex, 1, strMSG.Left(nPOS));
			strMSG = strMSG.Mid(nPOS);

			if(!strMSG.IsEmpty())
				nIndex = m_cListCtrl.InsertItem( m_cListCtrl.GetItemCount(), _T(""));
		}

		if(!strMSG.IsEmpty())
			m_cListCtrl.SetItemText( nIndex, 1, strMSG);
	}
}

void CTChatClientView::OnUpdateChattabAll( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bChatView == 0 ? TRUE : FALSE);
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnChattabAll()
{
	m_bChatView = 0;
	ResetTCHATMSG();
}

void CTChatClientView::OnUpdateChattabAnnounce( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bChatView == 3 ? TRUE : FALSE);
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnChattabAnnounce()
{
	m_bChatView = 3;
	ResetTCHATMSG();
}

void CTChatClientView::OnUpdateChattabNormal( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bChatView == 1 ? TRUE : FALSE);
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnChattabNormal()
{
	m_bChatView = 1;
	ResetTCHATMSG();
}

void CTChatClientView::OnUpdateChattabWisper( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bChatView == 2 ? TRUE : FALSE);
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnChattabWisper()
{
	m_bChatView = 2;
	ResetTCHATMSG();
}

void CTChatClientView::OnUpdateSetTarget(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bLogin ? TRUE : FALSE);
}

void CTChatClientView::OnSetTarget()
{
	for( int i=0; i<INT(m_vTCHATMSG.size()); i++)
		if( m_vTCHATMSG[i]->m_bType == CHAT_WHISPER && m_vTCHATMSG[i]->m_strNAME != m_strNAME )
		{
			m_strTARGET = m_vTCHATMSG[i]->m_strNAME;
			m_bChatType = CHAT_WHISPER;
		}

	SetChatTarget();
}

void CTChatClientView::InitTITEMTEMP()
{
	ReleaseTITEMTEMP();	

	TCHAR szFN[MAX_PATH];
	GetModuleFileName(NULL, szFN, MAX_PATH);
	char *pSlash = ::strrchr(szFN, '\\');
	if( pSlash )
		::lstrcpy(pSlash + 1, "\\Tcd\\TItem.tcd");

	//CString strFile;
	//strFile.LoadString(IDS_FILE_ITEMCHART);

	//CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CFile file( szFN, CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEM pTITEM = new TITEM();

		ar	>> pTITEM->m_wItemID				///일련번호
			>> pTITEM->m_bType					///종류
			>> pTITEM->m_bKind					///구분
			>> pTITEM->m_wAttrID				///성능일련번호
			>> pTITEM->m_strNAME				///이름
			>> pTITEM->m_wUseValue				///사용효과값
			>> pTITEM->m_dwSlotID				///장착위치
			>> pTITEM->m_dwClassID				///사용직업
			>> pTITEM->m_bPrmSlotID				///주무기장착위치
			>> pTITEM->m_bSubSlotID				///보조무기장착위치
			>> pTITEM->m_bLevel					///필요레벨
			>> pTITEM->m_bCanRepair				///수리여부(추가)
			>> pTITEM->m_dwDuraMax				///최대내구(추가)
			>> pTITEM->m_bRefineMax				///제련횟수(추가)
			>> pTITEM->m_fPriceRate				///가격비율
			>> pTITEM->m_dwPrice				///기준가격
			>> pTITEM->m_bMinRange				///최소사정거리
			>> pTITEM->m_bMaxRange				///최대사정거리
			>> pTITEM->m_bStack					///최대수량
			>> pTITEM->m_bSlotCount				///슬롯갯수
			>> pTITEM->m_bCanGamble				///봉인 생성여부
			>> pTITEM->m_bGambleProb			///치환여부(추가)
			>> pTITEM->m_bDestoryProb			///소멸여부(추가)
			>> pTITEM->m_bCanGrade				///등급 가능여부
			>> pTITEM->m_bCanMagic				///마법 생성여부
			>> pTITEM->m_bCanRare				///희귀 생성여부
			>> pTITEM->m_wDelayGroupID			///재사용대기그룹
			>> pTITEM->m_dwDelay				///재사용대기시간
			>> pTITEM->m_bCanTrade				///거래,판매 가능 여부
			>> pTITEM->m_bIsSpecial				///캐쉬 아이템 여부
			>> pTITEM->m_wUseTime				///사용 기간(일/시간)
			>> pTITEM->m_bUseType				///사용 타입
			>> pTITEM->m_bWeaponID				///WEAPON ID
			>> pTITEM->m_fShotSpeed				///SHOT SPEED
			>> pTITEM->m_fGravity				///GRAVITY
			>> pTITEM->m_dwInfoID				///INFO ID
			>> pTITEM->m_bSkillItemType			///발사타입
			>> pTITEM->m_wVisual[0]				///0단계 비주얼(추가)
			>> pTITEM->m_wVisual[1]				///1단계 비주얼(추가)
			>> pTITEM->m_wVisual[2]				///2단계 비주얼(추가)
			>> pTITEM->m_wVisual[3]				///3단계 비주얼(추가)
			>> pTITEM->m_wVisual[4]				///4단계 비주얼(추가)
			>> pTITEM->m_wGradeSFX				///등급 이펙트(추가)
			>> pTITEM->m_wOptionSFX[0]			///0단계 옵션 이펙트(추가)
			>> pTITEM->m_wOptionSFX[1]			///1단계 옵션 이펙트(추가)
			>> pTITEM->m_wOptionSFX[2]			///2단계 옵션 이펙트(추가)
			>> pTITEM->m_bCanWrap				/// 밀랍 가능
			>> pTITEM->m_dwAuctionCode
			>> pTITEM->m_bCanColor;				///염색가능

		if(!CTChatItem::FindTItem(pTITEM->m_wItemID))
			CTChatItem::m_mapTITEMTEMP.insert( MAPTITEMTEMP::value_type( pTITEM->m_wItemID, pTITEM));
		else
			delete pTITEM;
	}
}

void CTChatClientView::ReleaseTITEMTEMP()
{
	MAPTITEMTEMP::iterator itTITEM;

	for( itTITEM = CTChatItem::m_mapTITEMTEMP.begin(); itTITEM != CTChatItem::m_mapTITEMTEMP.end(); itTITEM++)
		delete (*itTITEM).second;

	CTChatItem::m_mapTITEMTEMP.clear();
}

void CTChatClientView::OnUpdateShopAutoOpen( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bLogin || m_bShop ? FALSE : TRUE);
}

void CTChatClientView::OnShopAutoOpen()
{
	CShopNameDlg dlg;

	if( dlg.DoModal() == IDOK && !dlg.m_strNAME.IsEmpty() )
	{
		MAPTINVEN::iterator itTINVEN = m_mapTINVEN.find(INVEN_DEFAULT);

		if( itTINVEN != m_mapTINVEN.end() )
		{
			CTChatInven *pTINVEN = (*itTINVEN).second;

			if(pTINVEN)
			{
				MAPTITEM::iterator itTITEM;
				srand(::GetCurrentTime());

				for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
					if( (rand() % 100) < 70 )
					{
						CTChatItem *pTITEM = (*itTITEM).second;

						if(pTITEM)
						{
							DWORD dwPrice = max( 1, pTITEM->m_pTITEM->m_dwPrice / 2);

							dwPrice = max( 1, dwPrice - dwPrice * (rand() % 30) / 100);
							pTITEM->m_dwRune = dwPrice / 1000000;
							pTITEM->m_dwLuna = (dwPrice % 1000000) / 1000;
							pTITEM->m_dwCron = dwPrice % 1000;

							m_mapTSELLITEM.insert( MAPTSELLITEM::value_type( pTITEM, MAKELONG( pTITEM->m_bItemID, pTINVEN->m_bInvenID)));
						}
					}

				if(!m_mapTSELLITEM.empty())
				{
					BYTE bCount = BYTE(m_mapTSELLITEM.size());

					MAPTSELLITEM::iterator itTSELL;
					CPacket vMSG;

					vMSG.SetID(CS_STOREOPEN_REQ)
						<< dlg.m_strNAME
						<< bCount;

					for( itTSELL = m_mapTSELLITEM.begin(); itTSELL != m_mapTSELLITEM.end(); itTSELL++)
					{
						BYTE bInvenID = BYTE(HIWORD((*itTSELL).second));
						BYTE bItemID = BYTE(LOWORD((*itTSELL).second));

						vMSG<< (*itTSELL).first->m_dwRune
							<< (*itTSELL).first->m_dwLuna
							<< (*itTSELL).first->m_dwCron
							<< bInvenID
							<< bItemID
							<< (*itTSELL).first->m_bCount;
					}

					m_pSOCKET->Say(&vMSG);
					m_bShop = TRUE;
				}
			}
		}
	}
}

void CTChatClientView::OnUpdateShopOpen( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bLogin || m_bShop ? FALSE : TRUE);
}

void CTChatClientView::OnShopOpen()
{
	CTChatShopDlg dlg;

	m_mapTSELLITEM.clear();
	dlg.m_pHost = this;

	if( dlg.DoModal() == IDOK && !dlg.m_strTSHOPNAME.IsEmpty() &&
		!m_mapTSELLITEM.empty() && INT(m_mapTSELLITEM.size()) < 256 )
	{
		BYTE bCount = BYTE(m_mapTSELLITEM.size());

		MAPTSELLITEM::iterator itTITEM;
		CPacket vMSG;

		vMSG.SetID(CS_STOREOPEN_REQ)
			<< dlg.m_strTSHOPNAME
			<< bCount;

		for( itTITEM = m_mapTSELLITEM.begin(); itTITEM != m_mapTSELLITEM.end(); itTITEM++)
		{
			BYTE bInvenID = BYTE(HIWORD((*itTITEM).second));
			BYTE bItemID = BYTE(LOWORD((*itTITEM).second));

			vMSG<< (*itTITEM).first->m_dwRune
				<< (*itTITEM).first->m_dwLuna
				<< (*itTITEM).first->m_dwCron
				<< bInvenID
				<< bItemID
				<< (*itTITEM).first->m_bCount;
		}

		m_pSOCKET->Say(&vMSG);
		m_bShop = TRUE;
	}
}

void CTChatClientView::OnUpdateShopClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bLogin && m_bOpen ? TRUE : FALSE);
}

void CTChatClientView::OnShopClose()
{
	CPacket vMSG;
	vMSG.SetID(CS_STORECLOSE_REQ);
	m_pSOCKET->Say(&vMSG);

	m_bOpen = FALSE;
}

void CTChatClientView::StartBeep(DWORD nDuration)
{
	m_nLeftTick = nDuration;
	m_dwPrvTick = GetTickCount();

	SetTimer(1, 1000, NULL);
}

void CTChatClientView::OnTimer(UINT nIDEvent)
{
	static int nCNT = 0;

	DWORD dwTick = GetTickCount();
	DWORD dwElap = dwTick - m_dwPrvTick;
	if( dwElap > m_nLeftTick )
	{
		m_nLeftTick = 0;
		KillTimer(nIDEvent);
	}
	else
	{
		m_nLeftTick -= dwElap;
		m_dwPrvTick = dwTick;

		if( ++nCNT % 2 )
			Beep(1500, 300);
		else
			Beep(2000, 300);
	}

	CView::OnTimer(nIDEvent);
}

CTChatSocket *CTChatClientView::RelaySocket()
{
	if(m_pRelay)
		return m_pRelay;
	else
		return m_pSOCKET;
}

void CTChatClientView::OnUpdateToolPingtest(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_bPingTestView);
}

void CTChatClientView::OnToolPingtest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pdlgPingTest->SetIPPort(m_strIP,m_dwPort);
	m_pdlgPingTest->SetSocket(m_pSOCKET);
	m_pdlgPingTest->Init();
	m_pdlgPingTest->ShowWindow(SW_SHOW);
	
}

CString CTChatClientView::EncryptPW(CString& strSOURCE)
{
	MD5 context;
	context.update( (unsigned char*) strSOURCE.GetBuffer(), strSOURCE.GetLength() );
	context.finalize();

	char* pstr = context.hex_digest();
	CString strRESULT( pstr );
	delete [] pstr;
	
	return strRESULT;
}
