#include "stdafx.h"
#include "TMapViewer.h"
#include "TMapViewerWnd.h"


BEGIN_MESSAGE_MAP(CTMapViewerWnd, CTachyonWnd)
	ON_MESSAGE( WM_TOBJMSG, OnOBJMessage)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


CTMapViewerWnd::CTMapViewerWnd()
{
	for( BYTE i=0; i<TLIGHT_COUNT; i++)
	{
		m_vTLIGHT[i].m_vDIR = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);

		m_vTLIGHT[i].m_fAmbientR = 0.0f;
		m_vTLIGHT[i].m_fAmbientG = 0.0f;
		m_vTLIGHT[i].m_fAmbientB = 0.0f;
		m_vTLIGHT[i].m_fDiffuseR = 0.0f;
		m_vTLIGHT[i].m_fAmbientG = 0.0f;
		m_vTLIGHT[i].m_fAmbientB = 0.0f;
	}

	m_MainGame.m_pMainChar = &m_MainChar;
	m_MainGame.m_pDevice = &m_Device;
	m_MainGame.m_pCamera = &m_Camera;
	m_MainGame.m_pRES = &m_RES;
	m_MainGame.m_pRESMAP = &m_RESMAP;

	m_MainGame.m_pMainWnd = this;
	m_MainChar.m_pHost = this;

	m_MainChar.ReleaseData();
	m_bResetTick = TRUE;
}

CTMapViewerWnd::~CTMapViewerWnd()
{
}

void CTMapViewerWnd::ProcessHotCommand( int nID)
{
	CTachyonWnd::ProcessHotCommand(nID);
}

void CTMapViewerWnd::OnCommand( DWORD cmd)
{
	m_MainGame.OnCommand(cmd);
}

void CTMapViewerWnd::Render()
{
	static DWORD dwPrevTick = GetTMapViewerTickCount();
	static CPoint prev;

	if( m_Device.m_pDevice )
	{
		DWORD dwTick = GetTMapViewerTickCount();

		TComponent *pFRAME = m_pMainFrame;
		CPoint point;

		if( dwPrevTick + TMAXPASS_TICK > dwTick )
		{
			m_MainGame.m_vKEY.m_dwTick = dwTick;
			m_bResetTick = FALSE;
		}
		else if(m_bResetTick)
		{
			m_MainGame.m_vKEY.m_dwTick = dwTick;
			dwPrevTick = dwTick;
		}
		else
			m_MainGame.m_vKEY.m_dwTick = dwPrevTick + TMAXPASS_TICK;

		if( m_MainGame.m_vKEY.m_dwTick == dwPrevTick )
			return;

		GetCursorPos(&point);
		ScreenToClient(&point);

		if( point != prev )
		{
			m_pMainFrame->OnMouseMove( 0, point);
			prev = point;
		}

		while( pFRAME && pFRAME->GetParent() )
			pFRAME = pFRAME->GetParent();

		m_Device.m_pDevice->Clear(
			0, NULL,
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB( 0, 0, 0),
			1.0f, 0);
		m_Device.m_pDevice->BeginScene();

		pFRAME->Render(m_MainGame.m_vKEY.m_dwTick - dwPrevTick);
		if(m_MainGame.m_bFPS)
		{
			m_FontFPS.m_dwColor = 0xFFFFFFF0;
			DrawFPS( 0, 0);
		}

		if(m_MainGame.m_bPOS)
		{
			CString strPOS;

			strPOS.Format(
				IDS_FMT_POS,
				m_MainChar.m_vPosition._41,
				m_MainChar.m_vPosition._42,
				m_MainChar.m_vPosition._43);

			m_FontPOS.TextOut(
				m_Device.m_pDevice,
				strPOS,
				m_FontFPS.GetWidth() + 10, 0);
		}

		if(m_MainGame.m_bLIGHT)
		{
			CString strLIGHT;

			FLOAT fUnitLength = FLOAT(m_MainGame.m_vMAP.m_pMAP->m_nUnitLength * m_MainGame.m_vMAP.m_pMAP->m_nTileLength);
			BYTE bUnitX = BYTE(m_MainGame.m_pCamera->m_vTarget.x / fUnitLength);
			BYTE bUnitZ = BYTE(m_MainGame.m_pCamera->m_vTarget.z / fUnitLength);

			LPTUNITFOG pTFOG = m_MainGame.FindTFOG(
				m_MainGame.m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			strLIGHT.Format(
				IDS_FMT_LIGHT,
				FLOAT(GETVALUE_R(m_MainGame.m_vMAP.m_dwTMapCOLOR)) / 255.0f,
				FLOAT(GETVALUE_G(m_MainGame.m_vMAP.m_dwTMapCOLOR)) / 255.0f,
				FLOAT(GETVALUE_B(m_MainGame.m_vMAP.m_dwTMapCOLOR)) / 255.0f,
				FLOAT(m_MainGame.m_vMAP.m_bSHADOW) / 255.0f,
				m_MainGame.m_vLIGHT[0].m_Light.Direction.x,
				m_MainGame.m_vLIGHT[0].m_Light.Direction.y,
				m_MainGame.m_vLIGHT[0].m_Light.Direction.z,
				m_MainGame.m_vLIGHT[0].m_Light.Ambient.r,
				m_MainGame.m_vLIGHT[0].m_Light.Ambient.g,
				m_MainGame.m_vLIGHT[0].m_Light.Ambient.b,
				m_MainGame.m_vLIGHT[0].m_Light.Diffuse.r,
				m_MainGame.m_vLIGHT[0].m_Light.Diffuse.g,
				m_MainGame.m_vLIGHT[0].m_Light.Diffuse.b,
				m_MainGame.m_vLIGHT[1].m_Light.Direction.x,
				m_MainGame.m_vLIGHT[1].m_Light.Direction.y,
				m_MainGame.m_vLIGHT[1].m_Light.Direction.z,
				m_MainGame.m_vLIGHT[1].m_Light.Ambient.r,
				m_MainGame.m_vLIGHT[1].m_Light.Ambient.g,
				m_MainGame.m_vLIGHT[1].m_Light.Ambient.b,
				m_MainGame.m_vLIGHT[1].m_Light.Diffuse.r,
				m_MainGame.m_vLIGHT[1].m_Light.Diffuse.g,
				m_MainGame.m_vLIGHT[1].m_Light.Diffuse.b,
				m_MainGame.m_vLIGHT[2].m_Light.Direction.x,
				m_MainGame.m_vLIGHT[2].m_Light.Direction.y,
				m_MainGame.m_vLIGHT[2].m_Light.Direction.z,
				m_MainGame.m_vLIGHT[2].m_Light.Ambient.r,
				m_MainGame.m_vLIGHT[2].m_Light.Ambient.g,
				m_MainGame.m_vLIGHT[2].m_Light.Ambient.b,
				m_MainGame.m_vLIGHT[2].m_Light.Diffuse.r,
				m_MainGame.m_vLIGHT[2].m_Light.Diffuse.g,
				m_MainGame.m_vLIGHT[2].m_Light.Diffuse.b,
				m_MainGame.m_vShadow.Ambient.r,
				m_MainGame.m_vShadow.Ambient.g,
				m_MainGame.m_vShadow.Ambient.b,
				m_MainGame.m_vShadow.Diffuse.r,
				m_MainGame.m_vShadow.Diffuse.g,
				m_MainGame.m_vShadow.Diffuse.b,
				pTFOG->m_pTGLOBAL->m_fSTART,
				pTFOG->m_pTGLOBAL->m_fEND,
				pTFOG->m_pTGLOBAL->m_bRValue,
				pTFOG->m_pTGLOBAL->m_bGValue,
				pTFOG->m_pTGLOBAL->m_bBValue,
				pTFOG->m_pTGLOBAL->m_fDENSITY);

			m_FontLIGHT.TextOut(
				m_Device.m_pDevice,
				strLIGHT,
				0, 50);
		}

		m_Device.m_pDevice->EndScene();
		if( !IsIconic() && FAILED(m_Device.m_pDevice->Present( NULL, NULL, NULL, NULL)) )
			RestoreDevice();
		dwPrevTick = m_MainGame.m_vKEY.m_dwTick;
	}
}

void CTMapViewerWnd::InitResource( CString strGroupID,
								  BYTE bGroupID)
{
	CTachyonHUGEMAP::m_nDiffuseCount = DEF_DIFFUSECNT;
	CTachyonHUGEMAP::m_nMaxLength = DEF_MAXLENGTH;
	CTachyonHUGEMAP::m_nMaxLevel = DEF_MAXLEVEL;
	CTachyonHUGEMAP::m_nLODBound = DEF_LODBOUND;
	CTachyonHUGEMAP::m_nIndexCount = DEF_INDEXCNT;
	CTachyonHUGEMAP::m_fCullDIST = TCAM_LENGTH;

	SetCursor(LoadCursor( NULL, IDC_ARROW));
	srand(DWORD(time(NULL)));

	//CTachyonWnd::InitResource( strGroupID, bGroupID);
	
	/// 리소스 로딩 변경부분
	InitTRESIMG();
	CTachyonWnd::InitResource( "TRunner", 0);
	m_MainGame.m_pTParser = m_pTParser;
	ReleaseTRESIMG();

	InitTRESIMG();
	m_RESMAP.Init("TRunnerMap", 1);

	TCMLParser* pCMLParser = new TCMLParser();
	strGroupID.Format("TRunnerMap");
	pCMLParser->Load((char *) LPCTSTR(strGroupID + _T(".TIF")));
	ReleaseTRESIMG();
	////////////////////////////////////////////////////////////

	CTBSPNode::InitCommon(&m_Device);

	m_MainGame.m_pTParser = m_pTParser;
	m_pMainFrame = &m_MainGame;

	LP_FRAMEDESC pDESC = m_pTParser->FindFrameTemplate(ID_FRAME_GAUGE);
	TFrame *pTFRAME = new TFrame( NULL, pDESC);
	TComponent *pPOS = pTFRAME->FindKid(ID_CTRLINST_MINIMAP);
	CRect rect;

	m_MainGame.m_pTMinimap = new CTMinimapDlg( &m_MainGame, m_pTParser->FindFrameTemplate(ID_FRAME_MINIMAP));
	pPOS->GetComponentRect(&rect);
	m_MainGame.m_pTMinimap->SetComponentRect(rect);
	m_MainGame.m_pTMinimap->ShowComponent(TRUE);
	m_MainGame.m_pTMinimap->EnableFloat(FALSE);

	m_MainGame.m_pTMinimap->m_pDevice = &m_Device;
	m_MainGame.m_pTMinimap->m_pTMAP = &m_MainGame.m_vMAP;
	m_MainGame.m_pTMinimap->m_pHost = &m_MainChar;
	m_MainGame.m_pTMinimap->m_pCAM = &m_Camera;
	delete pTFRAME;

	pDESC = m_pTParser->FindFrameTemplate(ID_FRAME_CHAT);
	pTFRAME = new TFrame( NULL, pDESC);

	m_MainGame.m_pTCHATEDIT = (TEdit *) pTFRAME->FindKid(ID_CTRLINST_CHAT_INPUT);
	m_MainGame.m_pTCHAT = (TList *) pTFRAME->FindKid(ID_CTRLINST_CHAT_LIST);

	pTFRAME->RemoveKid(m_MainGame.m_pTCHATEDIT);
	pTFRAME->RemoveKid(m_MainGame.m_pTCHAT);
	delete pTFRAME;

	pDESC = m_pTParser->FindFrameTemplate(ID_FRAME_POS);
	pTFRAME = new TFrame( NULL, pDESC);
	pPOS = pTFRAME->FindKid(ID_CTRLINST_WORLDMAP);

	m_MainGame.m_pWorldmap = new CTWorldmapDlg( &m_MainGame, m_pTParser->FindFrameTemplate(ID_FRAME_WORLDMAP));
	pPOS->GetComponentRect(&rect);
	m_MainGame.m_pWorldmap->SetComponentRect(rect);
	m_MainGame.m_pWorldmap->ShowComponent(FALSE);
	m_MainGame.m_pWorldmap->EnableFloat(FALSE);

	m_MainGame.m_pWorldmap->m_pDevice = &m_Device;
	m_MainGame.m_pWorldmap->m_pCAM = &m_Camera;

	m_MainGame.m_pWorldmap->m_pTMAP = &m_MainGame.m_vMAP;
	m_MainGame.m_pWorldmap->m_pHost = &m_MainChar;
	delete pTFRAME;

	m_MainGame.m_pTCHATEDIT->ResetParent(&m_MainGame);
	m_MainGame.m_pTCHAT->ResetParent(&m_MainGame);

	m_MainGame.AddKid(m_MainGame.m_pTCHATEDIT);
	m_MainGame.AddKid(m_MainGame.m_pTCHAT);
	m_MainGame.AddKid(m_MainGame.m_pTMinimap);
	m_MainGame.AddKid(m_MainGame.m_pWorldmap);

	m_MainGame.m_pTCHATEDIT->GetComponentRect(&rect);
	int nMoveY = m_Device.m_option.m_dwScreenY - rect.top - rect.Height();
	rect.OffsetRect( -rect.left, nMoveY);
	m_MainGame.m_pTCHATEDIT->SetComponentRect(&rect);

	m_MainGame.m_pTCHAT->GetComponentRect(&rect);
	rect.OffsetRect( -rect.left, nMoveY);
	m_MainGame.m_pTCHAT->SetComponentRect(&rect);

	m_MainGame.m_pTCHATEDIT->ShowComponent(FALSE);
	m_MainGame.m_pTCHAT->ShowComponent(FALSE);

	CTMapViewerObjBase::InitSquareResource(
		&m_Device,
		&m_RES);

	InitTREGIONINFO();
	InitTACTIONTEMP();
	InitTACTIONDATA();
	InitTPANTSTEMP();
	InitTRACETEMP();
	InitTFACETEMP();
	InitTBODYTEMP();
	InitTHANDTEMP();
	InitTFOOTTEMP();

	InitTITEMTEMP();
	InitTMAPINFO();
	InitTSFXTEMP();
	InitTMINIMAP();
	InitTSTEPSND();
	InitTRECTVB();
	InitTSKYBOX();
	InitTLIGHT();
	InitTNODE();
	InitTBGM();
	InitTFOG();

	CGDIFont *pGDIFont = FindFont(ID_FONT_LOGIN_BOLD);
	CTMapViewerObjBase::m_pFONT = &pGDIFont->m_font;

	m_FontLIGHT.m_dwColor = 0xFFFFFFF0;
	m_FontLIGHT.SetFont(&m_Font);

	m_FontPOS.m_dwColor = 0xFFFFFFF0;
	m_FontPOS.SetFont(&m_Font);

	InitCommonConst();
	InitLIGHT();
	InitCHAR();
	InitNAV();
}

void CTMapViewerWnd::InitCommonConst()
{
	static FLOAT vCONST[16] = {
		1.0f,			//	VC_COMMON
		0.0f,
		0.0f,
		765.005859f,
		0.5f,			//	PC_COMMON
		0.0f,
		256.0f,
		257.0f,
		0.0f,
		1.0f,
		2.0f,
		4.0f};

	m_Device.m_pDevice->SetPixelShaderConstantF(
		m_Device.m_vConstantPS[PC_COMMON],
		&vCONST[4], 2);

	m_Device.m_pDevice->SetVertexShaderConstantF(
		m_Device.m_vConstantVS[VC_COMMON],
		vCONST, 1);
}

void CTMapViewerWnd::ReleaseTRESIMG()
{
	while(!CTachyonRes::m_vBACKIMG.empty())
	{
		delete CTachyonRes::m_vBACKIMG.back();
		CTachyonRes::m_vBACKIMG.pop_back();
	}

	CTachyonRes::m_vGAUGE.Release();
	CTachyonRes::m_vTEXT.Release();
	CTachyonRes::m_vFONT.DeleteObject();
}

void CTMapViewerWnd::ReleaseTITEMTEMP()
{
	MAPTITEMTEMP::iterator itTITEM;

	for( itTITEM = CTMapViewerItem::m_mapTITEMTEMP.begin(); itTITEM != CTMapViewerItem::m_mapTITEMTEMP.end(); itTITEM++)
		delete (*itTITEM).second;

	CTMapViewerItem::m_mapTITEMTEMP.clear();
}

void CTMapViewerWnd::ReleaseTACTIONTEMP()
{
	MAPTACTION::iterator itTACTION;

	for( itTACTION = CTMapViewerObjBase::m_mapTACTIONTEMP.begin(); itTACTION != CTMapViewerObjBase::m_mapTACTIONTEMP.end(); itTACTION++)
		delete (*itTACTION).second;

	CTMapViewerObjBase::m_mapTACTIONTEMP.clear();
}

void CTMapViewerWnd::ReleaseTACTIONDATA()
{
	while(!CTMapViewerObjBase::m_vTACTION.empty())
	{
		delete CTMapViewerObjBase::m_vTACTION.back();
		CTMapViewerObjBase::m_vTACTION.pop_back();
	}
}

void CTMapViewerWnd::ReleaseTSTEPSND()
{
	CTMapViewerObjBase::m_mapTSTEPSND.clear();
}

void CTMapViewerWnd::ReleaseTFOG()
{
	MAPTUNITFOG::iterator itTFOG;

	for( itTFOG = CTMapViewerGame::m_mapTFOG.begin(); itTFOG != CTMapViewerGame::m_mapTFOG.end(); itTFOG++)
		delete (*itTFOG).second;

	CTMapViewerGame::m_mapTFOG.clear();
}

void CTMapViewerWnd::ReleaseTSKYBOX()
{
	MAPTSKYBOX::iterator itTSKY;

	for( itTSKY = CTMapViewerGame::m_mapTSKYBOX.begin(); itTSKY != CTMapViewerGame::m_mapTSKYBOX.end(); itTSKY++)
		delete (*itTSKY).second;

	CTMapViewerGame::m_mapTSKYBOX.clear();
}

void CTMapViewerWnd::ReleaseTLIGHT()
{
	MAPTLIGHT::iterator itTLIGHT;

	for( itTLIGHT = CTMapViewerGame::m_mapTLIGHT.begin(); itTLIGHT != CTMapViewerGame::m_mapTLIGHT.end(); itTLIGHT++)
		delete (*itTLIGHT).second;

	CTMapViewerGame::m_mapTLIGHT.clear();
}

void CTMapViewerWnd::ReleaseTMINIMAP()
{
	MAPTMINIMAP::iterator itTMINIMAP;

	for( itTMINIMAP = CTMapViewerMAP::m_mapTMINIMAP.begin(); itTMINIMAP != CTMapViewerMAP::m_mapTMINIMAP.end(); itTMINIMAP++)
		delete (*itTMINIMAP).second;

	CTMapViewerMAP::m_mapTMINIMAP.clear();
	CTMinimapDlg::m_pTMASK = NULL;
	CTMapViewerMAP::m_pTPOS = NULL;

	if(CTMinimapDlg::m_pTMINIMAP)
	{
		CTMinimapDlg::m_pTMINIMAP->Release();
		CTMinimapDlg::m_pTMINIMAP = NULL;
	}
}

void CTMapViewerWnd::ReleaseTSFXTEMP()
{
	MAPTSFXTEMP::iterator itTSFX;

	for( itTSFX = CTMapViewerSFX::m_mapTSFXTEMP.begin(); itTSFX != CTMapViewerSFX::m_mapTSFXTEMP.end(); itTSFX++)
		delete (*itTSFX).second;

	CTMapViewerSFX::m_mapTSFXTEMP.clear();
}


void CTMapViewerWnd::ReleaseTREGIONINFO()
{
	MAPTREGIONINFO::iterator itTREGION;

	for( itTREGION = CTMapViewerMAP::m_mapTREGIONINFO.begin(); itTREGION != CTMapViewerMAP::m_mapTREGIONINFO.end(); itTREGION++)
		delete (*itTREGION).second;

	CTMapViewerMAP::m_mapTREGIONINFO.clear();
}

void CTMapViewerWnd::ReleaseTMAPINFO()
{
	MAPTMAPINFO::iterator itTMAP;

	for( itTMAP = CTMapViewerMAP::m_mapTMAPINFO.begin(); itTMAP != CTMapViewerMAP::m_mapTMAPINFO.end(); itTMAP++)
		delete (*itTMAP).second;

	CTMapViewerMAP::m_mapTMAPINFO.clear();
}

void CTMapViewerWnd::ReleaseTRECTVB()
{
	if(CTMapViewerGame::m_pTRECTVB)
	{
		CTMapViewerGame::m_pTRECTVB->Release();
		CTMapViewerGame::m_pTRECTVB = NULL;
	}
}

void CTMapViewerWnd::InitTRESIMG()
{
	ReleaseTRESIMG();

	LPBYTE pDATA = NULL;
	DWORD dwSIZE = 0;

	CString strFILE;
	CFile vFILE;

	for( BYTE i=0; i<TLOADING_IMAGE_COUNT; i++)
	{
		CD3DImage *pIMG = new CD3DImage();

		strFILE.Format( IDS_FILE_LOADINGIMG, i);
		vFILE.Open( strFILE, CFile::modeRead|CFile::typeBinary);

		dwSIZE = DWORD(vFILE.GetLength());
		pDATA = new BYTE[dwSIZE];
		vFILE.Read( pDATA, dwSIZE);

		pIMG->LoadImageFile(
			m_Device.m_pDevice,
			pDATA,
			dwSIZE);
		CTachyonRes::m_vBACKIMG.push_back(pIMG);

		delete[] pDATA;
		vFILE.Close();
	}

	strFILE.LoadString(IDS_FILE_GAUGE);
	vFILE.Open( strFILE, CFile::modeRead|CFile::typeBinary);

	dwSIZE = DWORD(vFILE.GetLength());
	pDATA = new BYTE[dwSIZE];
	vFILE.Read( pDATA, dwSIZE);

	CTachyonRes::m_vGAUGE.LoadImageFile(
		m_Device.m_pDevice,
		pDATA,
		dwSIZE);

	CTachyonRes::m_nGaugeX = TLOADING_GAUGE_X;
	CTachyonRes::m_nGaugeY = TLOADING_GAUGE_Y;

	delete[] pDATA;
	vFILE.Close();

	CTachyonRes::m_vFONT.CreateFont(
		12, 0, 0, 0, 0,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		_T("굴림체"));

	CTachyonRes::m_vTEXT.SetFont(&CTachyonRes::m_vFONT);
	CTachyonRes::m_vTEXT.m_dwColor = TLOADING_TEXT_CLR;
}

void CTMapViewerWnd::InitTREGIONINFO()
{
	ReleaseTREGIONINFO();
	CString strFile;
	strFile.LoadString(IDS_FILE_REGIONCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		BYTE bCanMail;

		LPTREGIONINFO pTREGION = new TREGIONINFO();

		ar	>> pTREGION->m_dwID
			>> pTREGION->m_strNAME
			>> pTREGION->m_bContryID
			>> pTREGION->m_bCanFly
			>> pTREGION->m_wLocalID
			>> pTREGION->m_vTHUMB.x
			>> pTREGION->m_vTHUMB.y
			>> pTREGION->m_vTHUMB.z
			>> pTREGION->m_vTHUMB.x
			>> pTREGION->m_vTHUMB.y
			>> pTREGION->m_vTHUMB.z
			>> pTREGION->m_vTHUMB.x
			>> pTREGION->m_vTHUMB.y
			>> pTREGION->m_vTHUMB.z
			>> bCanMail;

		CTMapViewerMAP::m_mapTREGIONINFO.insert( MAPTREGIONINFO::value_type( pTREGION->m_dwID, pTREGION));
	}
}

void CTMapViewerWnd::InitTMAPINFO()
{
	ReleaseTMAPINFO();
	CString strFile;
	strFile.LoadString(IDS_FILE_MAPCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTMAPINFO pTMAPINFO = new TMAPINFO();
		DWORD dwMapID;

		ar	>> dwMapID
			>> pTMAPINFO->m_dwMapID;

		CTMapViewerMAP::m_mapTMAPINFO.insert( MAPTMAPINFO::value_type( dwMapID, pTMAPINFO));
	}
}

void CTMapViewerWnd::InitTNODE()
{
	CString strFile;
	strFile.LoadString(IDS_FILE_NODECHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPD3DXVECTOR3 pPOS = new D3DXVECTOR3();
		DWORD dwNodeID;
		DWORD dwMapID;

		ar	>> dwMapID
			>> dwNodeID
			>> pPOS->x
			>> pPOS->y
			>> pPOS->z;

		LPTMAPINFO pTMAPINFO = CTMapViewerMAP::GetMapINFO(dwMapID);
		pTMAPINFO->m_mapTNODE.insert( MAPD3DXVECTOR3::value_type( dwNodeID, pPOS));
	}
}

void CTMapViewerWnd::InitTACTIONTEMP()
{
	ReleaseTACTIONTEMP();
	CString strFile;
	strFile.LoadString(IDS_FILE_ACTIONCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTACTION pTACTION = new TACTION();
		DWORD dwActionID;

		ar	>> dwActionID
			>> pTACTION->m_dwActID
			>> pTACTION->m_bEquipMode
			>> pTACTION->m_dwTSLASH;

		CTMapViewerObjBase::m_mapTACTIONTEMP.insert( MAPTACTION::value_type( dwActionID, pTACTION));
	}
}

void CTMapViewerWnd::InitTACTIONDATA()
{
	ReleaseTACTIONDATA();
	CString strFile;
	strFile.LoadString(IDS_FILE_ACTIONDEFCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTACTIONDATA pTDATA = new TACTIONDATA();
		CString strCMD;

		ar	>> strCMD
			>> pTDATA->m_bLevel
			>> pTDATA->m_bSubAction
			>> pTDATA->m_bLoopAction
			>> pTDATA->m_bContinue
			>> pTDATA->m_bCancel
			>> pTDATA->m_bNavAction
			>> pTDATA->m_bSkipMain;

		CTMapViewerObjBase::m_vTACTION.push_back(pTDATA);
	}
}

void CTMapViewerWnd::InitTFOG()
{
	ReleaseTFOG();
	CString strFile;
	strFile.LoadString(IDS_FILE_FOGCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTFOG pTFOG = new TFOG();

		DWORD dwFogID;
		BYTE bTGLOBAL;

		ar	>> dwFogID
			>> bTGLOBAL
			>> pTFOG->m_bType
			>> pTFOG->m_bRValue
			>> pTFOG->m_bGValue
			>> pTFOG->m_bBValue
			>> pTFOG->m_fRADIUS
			>> pTFOG->m_fRANGE
			>> pTFOG->m_fPosX
			>> pTFOG->m_fPosZ
			>> pTFOG->m_fDENSITY
			>> pTFOG->m_fSTART
			>> pTFOG->m_fEND;

		LPTUNITFOG pTUNITFOG = CTMapViewerGame::FindTFOG(dwFogID);
		if(!pTUNITFOG)
		{
			pTUNITFOG = new TUNITFOG();
			CTMapViewerGame::m_mapTFOG.insert(MAPTUNITFOG::value_type( dwFogID, pTUNITFOG));
		}

		if(bTGLOBAL)
		{
			if(pTUNITFOG->m_pTGLOBAL)
				delete pTUNITFOG->m_pTGLOBAL;

			pTUNITFOG->m_pTGLOBAL = pTFOG;
		}
		else
			pTUNITFOG->m_vTLOCAL.push_back(pTFOG);
	}
}

void CTMapViewerWnd::InitTSKYBOX()
{
	ReleaseTSKYBOX();
	CString strFile;
	strFile.LoadString(IDS_FILE_SKYCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSKYBOX pTSKYBOX = new TSKYBOX();
		DWORD dwID;

		ar	>> dwID
			>> pTSKYBOX->m_dwOBJ
			>> pTSKYBOX->m_dwCLK
			>> pTSKYBOX->m_dwCL
			>> pTSKYBOX->m_dwMESH
			>> pTSKYBOX->m_dwACT
			>> pTSKYBOX->m_dwANI;

		CTMapViewerGame::m_mapTSKYBOX.insert( MAPTSKYBOX::value_type( dwID, pTSKYBOX));
	}
}

void CTMapViewerWnd::InitTLIGHT()
{
	ReleaseTLIGHT();
	CString strFile;
	strFile.LoadString(IDS_FILE_LIGHTCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTLIGHT pTLIGHT = new TLIGHT();
		DWORD dwLightID;

		ar	>> dwLightID
			>> pTLIGHT->m_vDIR.x
			>> pTLIGHT->m_vDIR.y
			>> pTLIGHT->m_vDIR.z
			>> pTLIGHT->m_fAmbientR
			>> pTLIGHT->m_fAmbientG
			>> pTLIGHT->m_fAmbientB
			>> pTLIGHT->m_fDiffuseR
			>> pTLIGHT->m_fDiffuseG
			>> pTLIGHT->m_fDiffuseB;

		CTMapViewerGame::m_mapTLIGHT.insert( MAPTLIGHT::value_type( dwLightID, pTLIGHT));
	}
}

void CTMapViewerWnd::InitTBGM()
{
	CString strFile;
	strFile.LoadString(IDS_FILE_BGMCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTREGIONINFO pTREGION = NULL;

		DWORD dwRegionID;
		DWORD dwBgmID;

		ar	>> dwRegionID
			>> dwBgmID;
		pTREGION = CTMapViewerMAP::GetRegionINFO(dwRegionID);

		if(pTREGION)
			pTREGION->m_vTBGM.push_back(dwBgmID);
	}
}

void CTMapViewerWnd::InitTPANTSTEMP()
{
	for( int i=0; i<TPANTS_COUNT; i++)
	{
		CTMapViewerObjBase::m_vTPANTS[i].m_bHideSlotID = 0xFF;
		CTMapViewerObjBase::m_vTPANTS[i].m_bHidePartID = 0xFF;
		CTMapViewerObjBase::m_vTPANTS[i].m_bHideRaceID = 0;

		CTMapViewerObjBase::m_vTPANTS[i].m_dwCLK = 0;
		CTMapViewerObjBase::m_vTPANTS[i].m_dwCL = 0;
		CTMapViewerObjBase::m_vTPANTS[i].m_dwMESH = 0;
	}

	CString strFile;
	strFile.LoadString(IDS_FILE_PANTSCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bPantsID;
		ar >> bPantsID;

		ar	>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_dwCLK
			>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_dwCL
			>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_dwMESH
			>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_bHideSlotID
			>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_bHidePartID
			>> CTMapViewerObjBase::m_vTPANTS[bPantsID].m_bHideRaceID;
	}
}

void CTMapViewerWnd::InitTRACETEMP()
{
	for( int i=0; i<TMAINRACE_COUNT; i++)
		for( int j=0; j<TSEX_COUNT; j++)
			CTMapViewerObjBase::m_vTRACE[i][j] = 0;

	CString strFile;
	strFile.LoadString(IDS_FILE_RACECHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bRACE;
		BYTE bSEX;

		ar	>> bRACE
			>> bSEX;

		ar	>> CTMapViewerObjBase::m_vTRACE[bRACE][bSEX];
		m_RES.LockOBJ(m_RES.GetOBJ(CTMapViewerObjBase::m_vTRACE[bRACE][bSEX]));
	}
}

void CTMapViewerWnd::InitTFACETEMP()
{
	for( int i=0; i<TFACE_COUNT; i++)
		for( int j=0; j<THAIR_COUNT; j++)
		{
			CTMapViewerObjBase::m_vTFACE[i][j].m_bHideSlotID = 0xFF;
			CTMapViewerObjBase::m_vTFACE[i][j].m_bHidePartID = 0xFF;
			CTMapViewerObjBase::m_vTFACE[i][j].m_bHideRaceID = 0;

			CTMapViewerObjBase::m_vTFACE[i][j].m_dwCLK = 0;
			CTMapViewerObjBase::m_vTFACE[i][j].m_dwCL = 0;
			CTMapViewerObjBase::m_vTFACE[i][j].m_dwMESH = 0;
		}

	CString strFile;
	strFile.LoadString(IDS_FILE_HEADCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bFACE;
		BYTE bHAIR;

		ar	>> bFACE
			>> bHAIR;

		ar	>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_dwCLK
			>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_dwCL
			>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_dwMESH
			>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_bHideSlotID
			>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_bHidePartID
			>> CTMapViewerObjBase::m_vTFACE[bFACE][bHAIR].m_bHideRaceID;
	}
}

void CTMapViewerWnd::InitTBODYTEMP()
{
	for( int i=0; i<TBODY_COUNT; i++)
	{
		CTMapViewerObjBase::m_vTBODY[i].m_bHideSlotID = 0xFF;
		CTMapViewerObjBase::m_vTBODY[i].m_bHidePartID = 0xFF;
		CTMapViewerObjBase::m_vTBODY[i].m_bHideRaceID = 0;

		CTMapViewerObjBase::m_vTBODY[i].m_dwCLK = 0;
		CTMapViewerObjBase::m_vTBODY[i].m_dwCL = 0;
		CTMapViewerObjBase::m_vTBODY[i].m_dwMESH = 0;
	}

	CString strFile;
	strFile.LoadString(IDS_FILE_BODYCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bBODY;
		ar >> bBODY;

		ar	>> CTMapViewerObjBase::m_vTBODY[bBODY].m_dwCLK
			>> CTMapViewerObjBase::m_vTBODY[bBODY].m_dwCL
			>> CTMapViewerObjBase::m_vTBODY[bBODY].m_dwMESH
			>> CTMapViewerObjBase::m_vTBODY[bBODY].m_bHideSlotID
			>> CTMapViewerObjBase::m_vTBODY[bBODY].m_bHidePartID
			>> CTMapViewerObjBase::m_vTBODY[bBODY].m_bHideRaceID;
	}
}

void CTMapViewerWnd::InitTHANDTEMP()
{
	for( int i=0; i<THAND_COUNT; i++)
	{
		CTMapViewerObjBase::m_vTHAND[i].m_bHideSlotID = 0xFF;
		CTMapViewerObjBase::m_vTHAND[i].m_bHidePartID = 0xFF;
		CTMapViewerObjBase::m_vTHAND[i].m_bHideRaceID = 0;

		CTMapViewerObjBase::m_vTHAND[i].m_dwCLK = 0;
		CTMapViewerObjBase::m_vTHAND[i].m_dwCL = 0;
		CTMapViewerObjBase::m_vTHAND[i].m_dwMESH = 0;
	}

	CString strFile;
	strFile.LoadString(IDS_FILE_HANDCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bHAND;
		ar >> bHAND;

		ar	>> CTMapViewerObjBase::m_vTHAND[bHAND].m_dwCLK
			>> CTMapViewerObjBase::m_vTHAND[bHAND].m_dwCL
			>> CTMapViewerObjBase::m_vTHAND[bHAND].m_dwMESH
			>> CTMapViewerObjBase::m_vTHAND[bHAND].m_bHideSlotID
			>> CTMapViewerObjBase::m_vTHAND[bHAND].m_bHidePartID
			>> CTMapViewerObjBase::m_vTHAND[bHAND].m_bHideRaceID;
	}
}

void CTMapViewerWnd::InitTFOOTTEMP()
{
	for( int i=0; i<TFOOT_COUNT; i++)
	{
		CTMapViewerObjBase::m_vTFOOT[i].m_bHideSlotID = 0xFF;
		CTMapViewerObjBase::m_vTFOOT[i].m_bHidePartID = 0xFF;
		CTMapViewerObjBase::m_vTFOOT[i].m_bHideRaceID = 0;

		CTMapViewerObjBase::m_vTFOOT[i].m_dwCLK = 0;
		CTMapViewerObjBase::m_vTFOOT[i].m_dwCL = 0;
		CTMapViewerObjBase::m_vTFOOT[i].m_dwMESH = 0;
	}

	CString strFile;
	strFile.LoadString(IDS_FILE_FOOTCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( i=0; i<wCount; i++)
	{
		BYTE bFOOT;
		ar >> bFOOT;

		ar	>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_dwCLK
			>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_dwCL
			>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_dwMESH
			>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_bHideSlotID
			>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_bHidePartID
			>> CTMapViewerObjBase::m_vTFOOT[bFOOT].m_bHideRaceID;
	}
}

void CTMapViewerWnd::InitTITEMTEMP()
{
	ReleaseTITEMTEMP();
	CString strFile;
	strFile.LoadString(IDS_FILE_ITEMCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTITEM pTITEM = new TITEM();

		DWORD dwInvenID;
		DWORD dwClassID;
		DWORD dwInfoID;
		DWORD dwPrice;
		DWORD dwDelay;

		FLOAT fShotSpeed;
		FLOAT fGravity;

		BYTE bSkillItemType;
		BYTE bSlotCount;
		BYTE bMinRange;
		BYTE bMaxRange;
		BYTE bCanTrade;
		BYTE bStack;
		BYTE bLevel;
		BYTE bType;
		BYTE bKind;

		BYTE bCanGrade;
		BYTE bCanMagic;
		BYTE bCanRare;

		ar	>> pTITEM->m_wItemID
			>> bType
			>> bKind
			>> pTITEM->m_strNAME
			>> pTITEM->m_dwSlotID
			>> dwClassID
			>> pTITEM->m_bPrmSlotID
			>> pTITEM->m_bSubSlotID
			>> bLevel
			>> dwPrice
			>> bMinRange
			>> bMaxRange
			>> bStack
			>> bSlotCount
			>> bCanGrade
			>> bCanMagic
			>> bCanRare
			>> dwDelay
			>> bCanTrade
			>> pTITEM->m_wImageID
			>> pTITEM->m_bWeaponID
			>> dwInvenID
			>> pTITEM->m_dwOBJ
			>> pTITEM->m_dwCLK
			>> pTITEM->m_dwCL
			>> pTITEM->m_dwMESH[MT_NORMAL]
			>> pTITEM->m_dwMESH[MT_BATTLE]
			>> pTITEM->m_dwPivot[MT_NORMAL]
			>> pTITEM->m_dwPivot[MT_BATTLE]
			>> pTITEM->m_fPosX[MT_NORMAL]
			>> pTITEM->m_fPosY[MT_NORMAL]
			>> pTITEM->m_fPosZ[MT_NORMAL]
			>> pTITEM->m_fPosX[MT_BATTLE]
			>> pTITEM->m_fPosY[MT_BATTLE]
			>> pTITEM->m_fPosZ[MT_BATTLE]
			>> fShotSpeed
			>> fGravity
			>> dwInfoID
			>> pTITEM->m_bHideSlotID
			>> pTITEM->m_bHidePartID
			>> pTITEM->m_bHideRaceID
			>> pTITEM->m_dwTSFXColor
			>> pTITEM->m_dwTSFXTexID
			>> pTITEM->m_fLength
			>> bSkillItemType;

		CTMapViewerItem::m_mapTITEMTEMP.insert( MAPTITEMTEMP::value_type( pTITEM->m_wItemID, pTITEM));
	}
}

void CTMapViewerWnd::InitTSFXTEMP()
{
	ReleaseTSFXTEMP();
	CString strFile;
	strFile.LoadString(IDS_FILE_SFXCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTSFXTEMP pTSFX = new TSFXTEMP();
		DWORD dwSFX = 0;

		ar	>> pTSFX->m_dwID
			>> dwSFX
			>> pTSFX->m_dwPosID;

		MAPRES::iterator finder = m_RESMAP.m_mapSFX.find(dwSFX);
		if( finder != m_RESMAP.m_mapSFX.end() )
			pTSFX->m_pTSFX = (LPSFX) (*finder).second;
		else
			pTSFX->m_pTSFX = NULL;

		CTMapViewerSFX::m_mapTSFXTEMP.insert( MAPTSFXTEMP::value_type( pTSFX->m_dwID, pTSFX));
	}
}

void CTMapViewerWnd::InitTMINIMAP()
{
	ReleaseTMINIMAP();

	CString strFile;
	strFile.LoadString(IDS_FILE_MINIMAP);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		LPTMINIMAP pTMINIMAP = new TMINIMAP();
		DWORD dwUnitID;
		DWORD dwTexID;

		ar	>> dwUnitID
			>> dwTexID
			>> pTMINIMAP->m_fSCALE
			>> pTMINIMAP->m_fPosX
			>> pTMINIMAP->m_fPosZ
			>> pTMINIMAP->m_bWorldID;

		MAPRES::iterator finder = m_RES.m_mapTEX.find(dwTexID);
		if( finder != m_RES.m_mapTEX.end() )
			pTMINIMAP->m_pTEX = ((LPTEXTURESET) (*finder).second)->GetTexture();
		CTMapViewerMAP::m_mapTMINIMAP.insert( MAPTMINIMAP::value_type( dwUnitID, pTMINIMAP));
	}

	MAPRES::iterator finder = m_RES.m_mapTEX.find(ID_MINIMAP_MASK);
	if( finder != m_RES.m_mapTEX.end() )
		CTMinimapDlg::m_pTMASK = ((LPTEXTURESET) (*finder).second)->GetTexture();

	finder = m_RES.m_mapTEX.find(ID_MINIMAP_POS);
	if( finder != m_RES.m_mapTEX.end() )
		CTMapViewerMAP::m_pTPOS = ((LPTEXTURESET) (*finder).second)->GetTexture();

	D3DXCreateTexture(
		m_Device.m_pDevice,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&CTMinimapDlg::m_pTMINIMAP);
}

void CTMapViewerWnd::InitTSTEPSND()
{
	ReleaseTSTEPSND();
	CString strFile;
	strFile.LoadString(IDS_FILE_STEPCHART);

	CFile file( LPCSTR(strFile), CFile::modeRead|CFile::typeBinary);
	CArchive ar( &file, CArchive::load);

	WORD wCount;
	ar >> wCount;

	for( WORD i=0; i<wCount; i++)
	{
		DWORD dwTileID;
		DWORD dwFuncID;

		ar	>> dwTileID
			>> dwFuncID;

		CTMapViewerObjBase::m_mapTSTEPSND.insert( MAPDWORD::value_type( dwTileID, dwFuncID));
	}
}

void CTMapViewerWnd::InitTRECTVB()
{
	static PVERTEX vMESH[4] = {
		{ 0.0f, 0.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, -1.0f, 1.0f, 1.0f}};
	LPPVERTEX pDATA = NULL;

	ReleaseTRECTVB();
	m_Device.m_pDevice->CreateVertexBuffer(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&CTMapViewerGame::m_pTRECTVB, NULL);

	CTMapViewerGame::m_pTRECTVB->Lock( 0, 0, (LPVOID *) &pDATA, 0);
	memcpy( pDATA, vMESH, 4 * sizeof(PVERTEX));
	CTMapViewerGame::m_pTRECTVB->Unlock();
}

void CTMapViewerWnd::InitCamera()
{
	CTachyonObject vOBJ;

	vOBJ.InitOBJ(m_RESMAP.GetOBJ(ID_SETTING_CAMERA));
	m_Camera.InitCamera(
		m_Device.m_pDevice,
		vOBJ.GetAttrFLOAT(ID_CAM_NEAR),
		vOBJ.GetAttrFLOAT(ID_CAM_FAR),
		vOBJ.GetAttrFLOAT(ID_CAM_FOV) * D3DX_PI / 180.0f,
		m_Device.m_option.m_dwScreenX,
		m_Device.m_option.m_dwScreenY);

	m_Camera.m_fZoomMIN = m_MainChar.GetAttrFLOAT(ID_TRUNNER_CAM_ZOOMMIN);
	m_Camera.m_fZoomMAX = m_MainChar.GetAttrFLOAT(ID_TRUNNER_CAM_ZOOMMAX);
	m_Camera.m_fZoomSPEED = vOBJ.GetAttrFLOAT(ID_CAM_ZOOMSPEED);
	CTMapViewerObjBase::m_fCamDist = vOBJ.GetAttrFLOAT(ID_CAM_CULL);

	m_Camera.m_fCamHEIGHT = m_MainChar.GetAttrFLOAT(ID_TRUNNER_CAM_HEIGHT);
	m_Camera.m_fCamDIST = m_MainChar.GetAttrFLOAT(ID_TRUNNER_CAM_DIST);
	m_Camera.SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, m_Camera.m_fCamDIST),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	FLOAT fPitch = 5.0f * vOBJ.GetAttrFLOAT(ID_CAM_DEFPITCH);
	while(fPitch < 0.0f)
		fPitch += 1800.0f;

	m_Camera.m_bCamRotSPEED = (BYTE) vOBJ.GetAttrINT(ID_CAM_ROTSPEED);
	m_Camera.m_wCamPITCH = WORD(fPitch) % 1800;

	m_MainGame.m_wCamDIR = m_MainChar.m_wDIR;
	m_MainGame.m_bCamFIX = FALSE;
	m_MainGame.m_nCamZOOM = 0;

	m_Camera.Rotate( 0.0f, -FLOAT(m_Camera.m_wCamPITCH) * D3DX_PI / 900.0f, 0.0f);
	m_Camera.Activate(TRUE);
}

void CTMapViewerWnd::InitLIGHT()
{
	for( int i=0; i<TLIGHT_COUNT; i++)
	{
		memset( &m_MainGame.m_vLIGHT[i].m_Light, 0, sizeof(D3DLIGHT9));

		m_MainGame.m_vLIGHT[i].m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_MainGame.m_vLIGHT[i].m_Light.Direction.x = m_vTLIGHT[i].m_vDIR.x;
		m_MainGame.m_vLIGHT[i].m_Light.Direction.y = m_vTLIGHT[i].m_vDIR.y;
		m_MainGame.m_vLIGHT[i].m_Light.Direction.z = m_vTLIGHT[i].m_vDIR.z;

		m_MainGame.m_vLIGHT[i].m_Light.Ambient.r = m_vTLIGHT[i].m_fAmbientR;
		m_MainGame.m_vLIGHT[i].m_Light.Ambient.g = m_vTLIGHT[i].m_fAmbientG;
		m_MainGame.m_vLIGHT[i].m_Light.Ambient.b = m_vTLIGHT[i].m_fAmbientB;
		m_MainGame.m_vLIGHT[i].m_Light.Ambient.a = 1.0f;

		m_MainGame.m_vLIGHT[i].m_Light.Diffuse.r = m_vTLIGHT[i].m_fDiffuseR;
		m_MainGame.m_vLIGHT[i].m_Light.Diffuse.g = m_vTLIGHT[i].m_fDiffuseG;
		m_MainGame.m_vLIGHT[i].m_Light.Diffuse.b = m_vTLIGHT[i].m_fDiffuseB;
		m_MainGame.m_vLIGHT[i].m_Light.Diffuse.a = 1.0f;

		m_MainGame.m_vLIGHT[i].SetLight(&m_MainGame.m_vLIGHT[i].m_Light);
		m_MainGame.m_vLIGHT[i].EnableLight();
	}
}

void CTMapViewerWnd::InitCHAR()
{
	m_MainChar.ReleaseData();

	m_MainChar.m_mapTINVEN.insert( MAPTINVEN::value_type(
		INVEN_EQUIP,
		new CTMapViewerInven()));

	m_MainChar.InitOBJ(m_RES.GetOBJ(CTMapViewerObjBase::m_vTRACE[0][1]));
	//m_MainChar.ResetEQUIP( &m_Device, &m_RES);

	DWORD vCLEAR[TCLEAR_COUNT] = {
			ID_CLK_RHAND_WEAPON,
			ID_CLK_LHAND_WEAPON,
			ID_CLK_LONG_WEAPON,
			ID_CLK_HEAD,
			ID_CLK_BACK};

	for( BYTE i=0; i<TCLEAR_COUNT; i++)
		m_MainChar.SetCloth( &m_Device, vCLEAR[i], 0, 0);
	m_MainChar.ClearEquip();

	m_MainChar.SetCloth(&m_Device,	ID_CLK_FACE,	ID_FACE_4,		ID_FACE_041);
	m_MainChar.SetCloth(&m_Device,	ID_CLK_BODY,	ID_BODY_BASE,	ID_BODY_BASE_D_012);
	m_MainChar.SetCloth(&m_Device,	ID_CLK_HAND,	ID_HAND_BASE,	ID_HAND_BASE_D);
	m_MainChar.SetCloth(&m_Device,	ID_CLK_PANTS,	ID_PANTS_BASE,	ID_PANTS_BASE_D_013);
	m_MainChar.SetCloth(&m_Device,	ID_CLK_FOOT,	ID_FOOT_BASE,	ID_FOOT_BASE_D);
	m_MainChar.ResetOBJPart(&m_Device);
	m_MainChar.ResetRootID(ID_PIVOT_WAIST);
	InitCamera();

	m_MainChar.m_fBreathHeight = m_MainChar.GetAttrFLOAT(ID_TRUNNER_BREATH_HEIGHT);
	m_MainChar.m_fJumpSpeed = m_MainChar.GetAttrFLOAT(ID_TRUNNER_JUMP_SPEED);
	m_MainChar.InitSIZE(
		TCOLLISION_SIZE,
		m_MainChar.GetAttrFLOAT(ID_TRUNNER_SIZE_Y),
		TCOLLISION_SIZE,
		0.0f);

	D3DXMatrixIdentity(&m_MainChar.m_vWorld);
	m_MainChar.m_vPosition = m_MainChar.m_vWorld;

	m_MainChar.m_dwANISNDFuncID = TDEFSTEP_SND;
	m_MainChar.m_bSTEPSND = TRUE;
	m_MainChar.m_pTREGION = NULL;
}

void CTMapViewerWnd::InitNAV()
{
	CTachyonObject vOBJ;
	vOBJ.InitOBJ(m_RESMAP.GetOBJ(ID_MAPNAV_SETTING));

	CTMapViewerMAP::m_fClimbBound = vOBJ.GetAttrFLOAT(ID_CLIMB_BOUND);
	CTMapViewerMAP::m_fMoveBound = vOBJ.GetAttrFLOAT(ID_MOVE_BOUND);
	CTMapViewerMAP::m_fFallBound = vOBJ.GetAttrFLOAT(ID_FALL_BOUND);
	CTMapViewerMAP::m_fGravity = vOBJ.GetAttrFLOAT(ID_GRAVITY);
}

void CTMapViewerWnd::OnDestroy()
{
	m_MainGame.ReleaseGame();
	m_pMainFrame = NULL;

	CTMapViewerObjBase::ReleaseSquareResource();
	CTBSPNode::ReleaseCommon();

	m_FontLIGHT.Release();
	m_FontPOS.Release();

	ReleaseTREGIONINFO();
	ReleaseTACTIONTEMP();
	ReleaseTACTIONDATA();
	ReleaseTITEMTEMP();
	ReleaseTSFXTEMP();
	ReleaseTMINIMAP();
	ReleaseTMAPINFO();
	ReleaseTSTEPSND();
	ReleaseTRECTVB();
	ReleaseTSKYBOX();
	ReleaseTRESIMG();
	ReleaseTLIGHT();
	ReleaseTFOG();

	CTachyonWnd::OnDestroy();
}

void CTMapViewerWnd::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTMapViewerWnd::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnKeyUp( nChar, nRepCnt, nFlags);
}

void CTMapViewerWnd::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CTMapViewerWnd::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonDown(nFlags, point);

	CTachyonWnd::OnLButtonDown(nFlags, point);
}

void CTMapViewerWnd::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonUp(nFlags, point);

	CTachyonWnd::OnLButtonUp(nFlags, point);
}

void CTMapViewerWnd::OnMButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonDown(nFlags, point);

	CTachyonWnd::OnMButtonDown(nFlags, point);
}

void CTMapViewerWnd::OnMButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonUp(nFlags, point);

	CTachyonWnd::OnMButtonUp(nFlags, point);
}

void CTMapViewerWnd::OnRButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonDown(nFlags, point);

	CTachyonWnd::OnRButtonDown(nFlags, point);
}

void CTMapViewerWnd::OnRButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonUp(nFlags, point);

	CTachyonWnd::OnRButtonUp(nFlags, point);
}

void CTMapViewerWnd::OnRButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonDblClk(nFlags, point);

	CTachyonWnd::OnRButtonDblClk(nFlags, point);
}

void CTMapViewerWnd::OnMButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonDblClk(nFlags, point);

	CTachyonWnd::OnMButtonDblClk(nFlags, point);
}

void CTMapViewerWnd::OnLButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonDblClk(nFlags, point);

	CTachyonWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CTMapViewerWnd::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMouseWheel( nFlags, zDelta, pt);

	return CTachyonWnd::OnMouseWheel( nFlags, zDelta, pt);
}

void CTMapViewerWnd::ReFresh()
{
	if(m_pMainFrame)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		m_pMainFrame->OnMouseMove( 0, point);
	}
}

DWORD CTMapViewerWnd::GetTMapViewerTickCount()
{
	return GetTickCount() - CTMapViewerApp::m_dwAppTick;
}

LRESULT CTMapViewerWnd::OnOBJMessage( WPARAM wParam, LPARAM lParam)
{
	return m_MainGame.OnOBJMsg((LPOBJMSG) wParam);
}

void CTMapViewerWnd::RestoreDevice()
{
	if(CTMinimapDlg::m_pTMINIMAP)
	{
		CTMinimapDlg::m_pTMINIMAP->Release();
		CTMinimapDlg::m_pTMINIMAP = NULL;
	}

	if(m_MainGame.m_vMAP.m_pMAP)
		m_MainGame.m_vMAP.m_pMAP->DeleteForRestore();

	while( m_Device.m_pD3D && m_Device.m_pDevice )
		if(m_Device.Reset())
			break;

	for( int i=0; i<TLIGHT_COUNT; i++)
	{
		m_MainGame.m_vLIGHT[i].SetLight(&m_MainGame.m_vLIGHT[i].m_Light);
		m_MainGame.m_vLIGHT[i].EnableLight();
	}
	InitCommonConst();

	if(m_MainGame.m_vMAP.m_pMAP)
		m_MainGame.m_vMAP.m_pMAP->Restore( &m_Device, &m_Camera);

	D3DXCreateTexture(
		m_Device.m_pDevice,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&CTMinimapDlg::m_pTMINIMAP);
	m_bResetTick = TRUE;
}
