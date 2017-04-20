#include "stdafx.h"
#include "TExplorer.h"
#include "TExplorerWnd.h"

BEGIN_MESSAGE_MAP(CTExplorerWnd, CTachyonWnd)
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


CTExplorerWnd::CTExplorerWnd()
{
	m_Device.m_option.m_bUseSHADER = FALSE;

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

	m_MainGame.m_pMainWnd = this;
	m_MainChar.m_pHost = this;

	m_MainChar.ReleaseData();
	m_bResetTick = TRUE;
}

CTExplorerWnd::~CTExplorerWnd()
{
}

void CTExplorerWnd::ProcessHotCommand( int nID)
{
	CTachyonWnd::ProcessHotCommand(nID);
}

void CTExplorerWnd::OnCommand( TCOMMAND cmd)
{
	m_MainGame.OnCommand(cmd);
}

void CTExplorerWnd::Render()
{
	static DWORD dwPrevTick = GetTExplorerTickCount();
	static CPoint prev;

	if( m_Device.m_pDevice )
	{
		DWORD dwTick = GetTExplorerTickCount();

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
		if(m_MainGame.m_bFPS && m_MainGame.m_bShowUI)
		{
			m_FontFPS.m_dwColor = 0xFFFFFFF0;
			DrawFPS( 0, 0);
		}

		if(m_MainGame.m_bPOS && m_MainGame.m_bShowUI)
		{
			DWORD dwNodeID = NODEID_NULL;
			CString strPOS;

			if( m_MainGame.m_vMAP.m_dwBspID != NODEID_NULL )
			{
				MAPTBSP::iterator finder = m_MainGame.m_vMAP.m_mapTBSP.find(m_MainGame.m_vMAP.m_dwBspID);

				if( finder != m_MainGame.m_vMAP.m_mapTBSP.end() )
					dwNodeID = (*finder).second->GetNodeID(m_Camera.m_vTarget);
			}

			strPOS.Format(
				IDS_FMT_POS,
				m_MainChar.m_vPosition._41,
				m_MainChar.m_vPosition._42,
				m_MainChar.m_vPosition._43,
				m_MainChar.m_wDIR / 5,
				m_MainGame.m_vMAP.m_dwBspID,
				m_MainGame.m_vMAP.m_dwHouseID,
				m_MainGame.m_vMAP.m_bHouseMesh,
				dwNodeID);

			m_FontPOS.TextOut(
				m_Device.m_pDevice,
				strPOS,
				m_FontFPS.GetWidth() + 10, 0);
		}

		if(m_MainGame.m_bLIGHT && m_MainGame.m_bShowUI)
		{
			CString strLIGHT;

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
				m_MainGame.m_vShadow.Diffuse.b);

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

void CTExplorerWnd::InitResource( CString strGroupID,
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
	InitTRESIMG();

	CTachyonWnd::InitResource( strGroupID, bGroupID);
	ReleaseTRESIMG();
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

	CTExplorerObjBase::InitSquareResource(
		&m_Device,
		&m_RES);

	CTChart::InitTREGIONINFO( _T(".\\Tcd\\TRegion.tcd") );
	CTChart::InitTACTIONTEMP( _T(".\\Tcd\\TAction.tcd") );
	CTChart::InitTACTIONDATA( _T(".\\Tcd\\TADEF.tcd") );
	CTChart::InitTPANTSTEMP( _T(".\\Tcd\\TPants.tcd") );
	CTChart::InitTRACETEMP( _T(".\\Tcd\\TRace.tcd") );
	CTChart::InitTFACETEMP( _T(".\\Tcd\\TFace.tcd") );
	CTChart::InitTBODYTEMP( _T(".\\Tcd\\TBody.tcd") );
	CTChart::InitTHANDTEMP( _T(".\\Tcd\\THand.tcd") );
	CTChart::InitTFOOTTEMP( _T(".\\Tcd\\TFoot.tcd") );
	CTChart::InitTITEMTEMP( _T(".\\Tcd\\TItem.tcd") );
	CTChart::InitTITEMVISUAL( _T(".\\Tcd\\TItemVisual.tcd") );
	CTChart::InitTMAPINFO( _T(".\\Tcd\\TNODE.tcd"), _T(".\\Tcd\\TMAP.tcd") );
	CTChart::InitTSFXTEMP( _T(".\\Tcd\\TSFX.tcd") );
	CTChart::InitTNPCTEMP( _T(".\\Tcd\\TNPCTemp.tcd") );
	InitTMINIMAP( _T(".\\Tcd\\TMinimap.tcd") );
	CTChart::InitTSTEPSND( _T(".\\Tcd\\TSTEP.tcd") );
	CTChart::InitTSKYBOX( _T(".\\Tcd\\TSky.tcd") );
	CTChart::InitTLIGHT( _T(".\\Tcd\\TLIGHT.tcd") );
	CTChart::InitTBGM( _T(".\\Tcd\\TBGM.tcd") );
	CTChart::InitTENV( _T(".\\Tcd\\TENV.tcd") );
	CTChart::InitTFOG( _T(".\\Tcd\\TFog.tcd") );
	InitTRECTVB();

	// TSFXTEMP 饶贸府
	{
		MAPTSFXTEMP::iterator itTSFX, end;
		itTSFX = CTChart::m_mapTSFXTEMP.begin();
		end = CTChart::m_mapTSFXTEMP.end();

		for(; itTSFX != end ; ++itTSFX )
		{
			DWORD dwSFX = (DWORD) itTSFX->second->m_pTSFX;

			MAPRES::iterator finder = m_RES.m_mapSFX.find( dwSFX );
			if( finder != m_RES.m_mapSFX.end() )
				itTSFX->second->m_pTSFX = (LPSFX) (*finder).second;
			else
				itTSFX->second->m_pTSFX = NULL;
		}
	}

	// TMINIMAP 饶贸府
	{
//		CTWorldmapDlg* pWorldDlg = (CTWorldmapDlg*) m_MainGame.m_vTFRAME[TFRAME_WORLDMAP];
		CTWorldmapDlg* pWorldDlg = (CTWorldmapDlg*) m_MainGame.m_pWorldmap;

		MAPTMINIMAP::iterator it, end;
		it = CTChart::m_mapTMINIMAP.begin();
		end = CTChart::m_mapTMINIMAP.end();

		for(; it != end ; ++it )
		{
			LPTMINIMAP pTMINIMAP = it->second;
			DWORD dwTexID = (DWORD) pTMINIMAP->m_pTEX;

			MAPRES::iterator finder = m_RES.m_mapTEX.find(dwTexID);	
			if( finder != m_RES.m_mapTEX.end() )
				pTMINIMAP->m_pTEX = (LPTEXTURESET) (*finder).second;
			else
				pTMINIMAP->m_pTEX = NULL;

			TComponent* pTWorldBtn = pWorldDlg->FindKid(pTMINIMAP->m_dwWorldButtonID);
			if( pTWorldBtn )
			{
				pWorldDlg->RemoveKid(pTWorldBtn);
				pWorldDlg->m_vTWORLDBUTTON.push_back(pTWorldBtn);

				CTChart::m_mapTBTNID2UNIT.insert( std::make_pair( pTMINIMAP->m_dwWorldButtonID, pTMINIMAP->m_dwUnitID));
				CTChart::m_mapTBTNID2WORLD.insert( std::make_pair( pTMINIMAP->m_dwWorldButtonID, pTMINIMAP->m_bWorldID));
			}
		}
	}

	CGDIFont *pGDIFont = FindFont(ID_FONT_LOGIN_BOLD);
	CTExplorerObjBase::m_pFONT = pGDIFont->m_pFont;

	m_FontLIGHT.m_dwColor = 0xFFFFFFF0;
	m_FontLIGHT.SetFont(&m_Font);

	m_FontPOS.m_dwColor = 0xFFFFFFF0;
	m_FontPOS.SetFont(&m_Font);

	InitCommonConst();
	InitLIGHT();
	InitCHAR();
	InitNAV();
	CTExplorerBSP::InitTBSPMAP();
}

void CTExplorerWnd::InitCommonConst()
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

#ifdef MODIFY_LOADING
void CTExplorerWnd::ReleaseTRESIMG()
{
	while(!CTachyonRes::m_vTLOADINGSCENE.empty())
	{
		delete CTachyonRes::m_vTLOADINGSCENE.back();
		CTachyonRes::m_vTLOADINGSCENE.pop_back();
	}

	CTachyonRes::m_vTEXT.Release();
	CTachyonRes::m_vFONT.DeleteObject();
}
#else
void CTExplorerWnd::ReleaseTRESIMG()
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
#endif

void CTExplorerWnd::ReleaseTMINIMAP()
{
	MAPTMINIMAP::iterator itTMINIMAP;

	for( itTMINIMAP = CTExplorerMAP::m_mapTMINIMAP.begin(); itTMINIMAP != CTExplorerMAP::m_mapTMINIMAP.end(); itTMINIMAP++)
		delete (*itTMINIMAP).second;

	CTChart::ReleaseTMINIMAP();

	CTExplorerMAP::m_mapTMINIMAP.clear();
	CTMinimapDlg::m_pTMASK = NULL;
	CTExplorerMAP::m_pTPOS = NULL;

	if(CTMinimapDlg::m_pTMINIMAP)
	{
		CTMinimapDlg::m_pTMINIMAP->Release();
		CTMinimapDlg::m_pTMINIMAP = NULL;
	}
}

void CTExplorerWnd::ReleaseTRECTVB()
{
	if(CTExplorerGame::m_pTRECTVB)
	{
		CTExplorerGame::m_pTRECTVB->Release();
		CTExplorerGame::m_pTRECTVB = NULL;
	}
}

void CTExplorerWnd::LoadImageFile( const CString& strFILE, CT3DImage* pT3DIMG)
{
	CFile vFILE;

	if(vFILE.Open( strFILE, CFile::modeRead|CFile::typeBinary))
	{
		DWORD dwSIZE = DWORD(vFILE.GetLength());

		if(dwSIZE)
		{
			LPBYTE pDATA = new BYTE[dwSIZE];

			vFILE.Read( pDATA, dwSIZE);
			pT3DIMG->LoadT3DIMG(
				pDATA,
				dwSIZE);

			delete[] pDATA;
		}
	}
}

#ifdef MODIFY_LOADING
void CTExplorerWnd::InitTRESIMG()
{
	static const CString INTRO_PATH		= ".\\Intro\\";
	static const CString INTRO_LIST		= "Intro.cfg";

	static const CString SEPARATORS		= "\t:=";
	
	static const CString VAR_GAUGE		= "gauge";
	static const CString VAR_GAUGE_X	= "gauge_x";
	static const CString VAR_GAUGE_Y	= "gauge_y";
	static const CString VAR_IMAGE		= "image";

	static const CString VAR_TEXT_X		= "text_x";
	static const CString VAR_TEXT_Y		= "text_y";

	static const CString VAR_FADEIN_TIME = "fade_in";
	static const CString VAR_FADEOUT_TIME = "fade_out";
	static const CString VAR_KEEP_TIME = "keep";
	static const CString VAR_SCENE_START = "scene_start";
	static const CString VAR_SCENE_END = "scene_end";
	static const CString VAR_SHOWBAR = "show_bar";
	static const CString VAR_COLOR = "color";
	
	ReleaseTRESIMG();

	float fScaleX = (float)m_Device.m_option.m_dwScreenX / (float)TBASE_SCREEN_X;
	float fScaleY = (float)m_Device.m_option.m_dwScreenY / (float)TBASE_SCREEN_Y;

	FILE *fpLIST;
	char rec[MAX_PATH], *ret;
	CString optName, optVal;
	
	fpLIST = fopen(INTRO_PATH+INTRO_LIST, "r");
	if( !fpLIST )
		return;

	BOOL bSceneStart = FALSE;
	LPTLOADINGSCENE pLoadingScene = NULL;

	ret = fgets(rec, MAX_PATH, fpLIST);
	while( ret != NULL )
	{
		CString tst = rec;
		tst.Trim();
		
		if( tst.GetLength() > 0 && tst.GetAt(0) != ';' && tst.GetAt(0) != '\n')
		{
			CString optName(strtok(rec, SEPARATORS));
			CString optVal(strtok(NULL, "\n"));
			if( !optName.IsEmpty() )
			{
				optName.Trim();
				optVal.Trim();

				if( !bSceneStart && optName == VAR_SCENE_START )
				{
					bSceneStart = TRUE;
					pLoadingScene = new TLOADINGSCENE;
					if( pLoadingScene )
						ZeroMemory( pLoadingScene, sizeof(TLOADINGSCENE) );
				}
				else if( optName == VAR_COLOR )
				{
					INT n=0;
					CString str;

					str = optVal.Tokenize( " ", n);	
					BYTE bR = atoi( str );
					str = optVal.Tokenize( " ", n);
					BYTE bG = atoi( str );
					str = optVal.Tokenize( " ", n);
					BYTE bB = atoi( str );

					CTachyonRes::m_dwBackColor = D3DCOLOR_XRGB(bR, bG, bB );
				}
				else if( bSceneStart && pLoadingScene )
				{
					if( optName == VAR_GAUGE_X )
					{
						pLoadingScene->m_nGaugeX = atoi(optVal);
						pLoadingScene->m_nGaugeX -= (1024 / 2);
					}
					else if( optName == VAR_GAUGE_Y )
					{
						pLoadingScene->m_nGaugeY = atoi(optVal);
						pLoadingScene->m_nGaugeY -= (768 / 2);
					}
					else if( optName == VAR_TEXT_X )
					{
						pLoadingScene->m_nTextX = atoi(optVal);
						pLoadingScene->m_nTextX -= (1024 / 2);
					}
					else if( optName == VAR_TEXT_Y )
					{
						pLoadingScene->m_nTextY = atoi(optVal);
						pLoadingScene->m_nTextY -= (768 / 2);
					}
					else if( optName == VAR_GAUGE )
					{
						LoadImageFile( INTRO_PATH + optVal, &pLoadingScene->m_vGAUGE);
					}
					else if( optName == VAR_IMAGE )
					{
						LoadImageFile( INTRO_PATH + optVal, &pLoadingScene->m_vIMG);
					}
					else if( optName == VAR_FADEIN_TIME )
					{
						pLoadingScene->m_nFadeInTime = atoi(optVal);
					}
					else if( optName == VAR_FADEOUT_TIME )
					{
						pLoadingScene->m_nFadeOutTime = atoi(optVal);
					}
					else if( optName == VAR_KEEP_TIME )
					{
						pLoadingScene->m_nKeepTime = atoi(optVal);
					}
					else if( optName == VAR_SHOWBAR )
					{
						pLoadingScene->m_bShowBar = atoi(optVal);
					}
					else if( optName == VAR_SCENE_END )
					{
						pLoadingScene->m_nSeq = (INT) CTachyonRes::m_vTLOADINGSCENE.size();
						CTachyonRes::m_vTLOADINGSCENE.push_back( pLoadingScene);
						bSceneStart = FALSE;
						pLoadingScene = NULL;
					}
				}
			}
		}

		ret = fgets(rec, MAX_PATH, fpLIST);
	}

	fclose(fpLIST);

	CTachyonRes::m_vTEXT.m_dwLineColor = TTEXTSHADOW_COLOR;
	CTachyonRes::m_vTEXT.m_dwColor = TLOADING_TEXT_CLR;
	CTachyonRes::m_vTEXT.m_bOutLine = TRUE;
	CTachyonRes::m_vTEXT.SetFont(&m_Font);

	if( !CTachyonRes::m_vTLOADINGSCENE.empty() )
	{
		if( CTachyonRes::m_vTLOADINGSCENE[ 0 ] )
			CTachyonRes::m_vTLOADINGSCENE[ 0 ]->m_nState = 1;
	}
}
#else
void CTExplorerWnd::InitTRESIMG()
{
	static const CString INTRO_PATH		= ".\\Intro\\";
	static const CString INTRO_LIST		= "Intro.cfg";

	static const CString SEPARATORS		= "\t:=";
	
	static const CString VAR_GAUGE		= "gauge";
	static const CString VAR_GAUGE_X	= "gauge_x";
	static const CString VAR_GAUGE_Y	= "gauge_y";
	static const CString VAR_IMAGE		= "image";

	static const CString VAR_TEXT_X		= "text_x";
	static const CString VAR_TEXT_Y		= "text_y";

	ReleaseTRESIMG();

	float fScaleX = (float)m_Device.m_option.m_dwScreenX / (float)TBASE_SCREEN_X;
	float fScaleY = (float)m_Device.m_option.m_dwScreenY / (float)TBASE_SCREEN_Y;

	FILE *fpLIST;
	char rec[MAX_PATH], *ret;
	CString optName, optVal;
	
	fpLIST = fopen(INTRO_PATH+INTRO_LIST, "r");
	if( !fpLIST )
		return;

	ret = fgets(rec, MAX_PATH, fpLIST);
	while( ret != NULL )
	{
		CString tst = rec;
		tst.Trim();
		
		if( tst.GetLength() > 0 && tst.GetAt(0) != ';' && tst.GetAt(0) != '\n')
		{
			CString optName(strtok(rec, SEPARATORS));
			CString optVal(strtok(NULL, "\n"));
			if( !optName.IsEmpty() && !optVal.IsEmpty() )
			{
				optName.Trim();
				optVal.Trim();
							
				if( optName == VAR_GAUGE )
				{
					if( LoadImageFile(INTRO_PATH+optVal, &CTachyonRes::m_vGAUGE) )
					{
						CTachyonRes::m_vGAUGE.SetScaleX(fScaleX);
						CTachyonRes::m_vGAUGE.SetScaleY(fScaleY);
					}
				}
				else if( optName == VAR_IMAGE )
				{
					CD3DImage *pIMG = new CD3DImage();
					if( LoadImageFile(INTRO_PATH+optVal, pIMG) )
					{
						pIMG->SetScaleX(fScaleX);
						pIMG->SetScaleY(fScaleY);

						CTachyonRes::m_vBACKIMG.push_back(pIMG);	
					}
					else
						delete pIMG;
				}
				else if( optName == VAR_GAUGE_X )
					CTachyonRes::m_nGaugeX = atoi(optVal);
				else if( optName == VAR_GAUGE_Y )
					CTachyonRes::m_nGaugeY = atoi(optVal);
				else if( optName == VAR_TEXT_X )
					CTachyonRes::m_nTextX = atoi(optVal);
				else if( optName == VAR_TEXT_Y )
					CTachyonRes::m_nTextY = atoi(optVal);
			}
		}

		ret = fgets(rec, MAX_PATH, fpLIST);
	}

	fclose(fpLIST);

	CTachyonRes::m_vTEXT.m_dwLineColor = TTEXTSHADOW_COLOR;
	CTachyonRes::m_vTEXT.m_dwColor = TLOADING_TEXT_CLR;
	CTachyonRes::m_vTEXT.m_bOutLine = TRUE;
	CTachyonRes::m_vTEXT.SetScaleX(fScaleX);
	CTachyonRes::m_vTEXT.SetScaleY(fScaleY);
	CTachyonRes::m_vTEXT.SetFont(&m_Font);
}
#endif

void CTExplorerWnd::InitTMINIMAP( CString strPath)
{
	ReleaseTMINIMAP();

	CTChart::InitTMINIMAP( strPath );

	MAPTMINIMAP::iterator it, end;
	it = CTChart::m_mapTMINIMAP.begin();
	end = CTChart::m_mapTMINIMAP.end();

	for(; it != end ; ++it )
	{
		LPTMINIMAP pTMINIMAP = it->second;
		DWORD dwTexID = (DWORD) pTMINIMAP->m_pTEX;

		MAPRES::iterator finder = m_RES.m_mapTEX.find(dwTexID);	
		if( finder != m_RES.m_mapTEX.end() )
			pTMINIMAP->m_pTEX = (LPTEXTURESET) (*finder).second;
		else
			pTMINIMAP->m_pTEX = NULL;

		TComponent* pTWorldBtn = m_MainGame.m_pWorldmap->FindKid(pTMINIMAP->m_dwWorldButtonID);
		if( pTWorldBtn )
		{
			m_MainGame.m_pWorldmap->RemoveKid(pTWorldBtn);
			m_MainGame.m_pWorldmap->m_vTWORLDBUTTON.push_back(pTWorldBtn);
			pTWorldBtn->ShowComponent(TRUE);

			CTExplorerMAP::m_mapTBTNID2UNIT.insert( std::make_pair( pTMINIMAP->m_dwWorldButtonID, pTMINIMAP->m_dwUnitID));
			CTExplorerMAP::m_mapTBTNID2WORLD.insert( std::make_pair( pTMINIMAP->m_dwWorldButtonID, pTMINIMAP->m_bWorldID));
		}
	}

	MAPRES::iterator finder = m_RES.m_mapTEX.find(ID_MINIMAP_MASK);
	if( finder != m_RES.m_mapTEX.end() )
		CTMinimapDlg::m_pTMASK = ((LPTEXTURESET) (*finder).second)->GetTexture();

	finder = m_RES.m_mapTEX.find(ID_MINIMAP_POS);
	if( finder != m_RES.m_mapTEX.end() )
		CTExplorerMAP::m_pTPOS = ((LPTEXTURESET) (*finder).second)->GetTexture();

	D3DXCreateTexture(
		m_Device.m_pDevice,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE, 1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&CTMinimapDlg::m_pTMINIMAP);
}

void CTExplorerWnd::InitTRECTVB()
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
		&CTExplorerGame::m_pTRECTVB, NULL);

	CTExplorerGame::m_pTRECTVB->Lock( 0, 0, (LPVOID *) &pDATA, 0);
	memcpy( pDATA, vMESH, 4 * sizeof(PVERTEX));
	CTExplorerGame::m_pTRECTVB->Unlock();
}

void CTExplorerWnd::InitCamera()
{
	CTachyonObject vOBJ;

	vOBJ.InitOBJ(m_RES.GetOBJ(ID_SETTING_CAMERA));
	m_Camera.InitCamera(
		m_Device.m_pDevice,
		vOBJ.GetAttrFLOAT(ID_CAM_NEAR),
		vOBJ.GetAttrFLOAT(ID_CAM_FAR),
		vOBJ.GetAttrFLOAT(ID_CAM_FOV) * D3DX_PI / 180.0f,
		m_Device.m_option.m_dwScreenX,
		m_Device.m_option.m_dwScreenY);

	m_Camera.m_fZoomMIN = m_MainChar.GetAttrFLOAT(ID_CAM_ZOOMMIN);
	m_Camera.m_fZoomMAX = m_MainChar.GetAttrFLOAT(ID_CAM_ZOOMMAX);
	m_Camera.m_fZoomSPEED = vOBJ.GetAttrFLOAT(ID_CAM_ZOOMSPEED);
	CTExplorerObjBase::m_fCamDist = vOBJ.GetAttrFLOAT(ID_CAM_CULL);

	m_Camera.m_fCamHEIGHT = m_MainChar.GetAttrFLOAT(ID_CAM_HEIGHT);
	m_Camera.m_fCamDIST = m_MainChar.GetAttrFLOAT(ID_CAM_DIST);
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

void CTExplorerWnd::InitLIGHT()
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

void CTExplorerWnd::InitCHAR()
{
	m_MainChar.ReleaseData();

	m_MainChar.m_mapTINVEN.insert( MAPTINVEN::value_type(
		INVEN_EQUIP,
		new CTExplorerInven()));

	m_bRaceID = ++m_bRaceID % 3;
	m_MainChar.m_bRaceID = m_bRaceID;

	D3DXMATRIX vWorld = m_MainChar.m_vWorld;
	FLOAT fSpeedFactor = m_MainChar.m_fSpeedFactor;

	m_MainChar.InitOBJ(m_RES.GetOBJ( CTChart::m_vTRACE[m_MainChar.m_bRaceID][m_MainChar.m_bSex]));
	m_MainChar.ResetEQUIP( &m_Device, &m_RES);
	m_MainChar.ResetRootID(ID_PIVOT_WAIST);
	InitCamera();

	m_MainChar.m_fBreathHeight = m_MainChar.GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_MainChar.m_fJumpSpeed = m_MainChar.GetAttrFLOAT(ID_JUMP_SPEED);
	m_MainChar.InitSIZE(
		TCOLLISION_SIZE,
		m_MainChar.GetAttrFLOAT(ID_SIZE_Y),
		TCOLLISION_SIZE,
		0.0f, 0.0f);

	//D3DXMatrixIdentity(&m_MainChar.m_vWorld);

	m_MainChar.m_vWorld = vWorld;
	m_MainChar.m_vPosition = m_MainChar.m_vWorld;
	m_MainChar.m_fSpeedFactor = fSpeedFactor;

	m_MainChar.m_dwANISNDFuncID = TDEFSTEP_SND;
	m_MainChar.m_bSTEPSND = TRUE;
	m_MainChar.m_pTREGION = NULL;
}

void CTExplorerWnd::InitNAV()
{
	CTachyonObject vOBJ;
	vOBJ.InitOBJ(m_RES.GetOBJ(ID_MAPNAV_SETTING));

	CTExplorerMAP::m_fClimbBound = vOBJ.GetAttrFLOAT(ID_CLIMB_BOUND);
	CTExplorerMAP::m_fMoveBound = vOBJ.GetAttrFLOAT(ID_MOVE_BOUND);
	CTExplorerMAP::m_fFallBound = vOBJ.GetAttrFLOAT(ID_FALL_BOUND);
	CTExplorerMAP::m_fGravity = vOBJ.GetAttrFLOAT(ID_GRAVITY);
}

void CTExplorerWnd::OnDestroy()
{
	CTachyonRes::m_MEDIA.Stop(
		MEDIA_TMUSIC,
		m_MainGame.m_dwBGM, 0);

	m_MainGame.ReleaseGame();
	m_pMainFrame = NULL;

	CTExplorerObjBase::ReleaseSquareResource();
	CTBSPNode::ReleaseCommon();

	m_FontLIGHT.Release();
	m_FontPOS.Release();

	CTChart::ReleaseTREGIONINFO();
	CTChart::ReleaseTACTIONTEMP();
	CTChart::ReleaseTACTIONDATA();
	CTChart::ReleaseTITEMTEMP();
	CTChart::ReleaseTITEMVISUAL();
	CTChart::ReleaseTSFXTEMP();
	CTChart::ReleaseTNPCTEMP();
	ReleaseTMINIMAP();
	CTChart::ReleaseTMAPINFO();
	CTChart::ReleaseTSTEPSND();
	CTChart::ReleaseTSKYBOX();
	CTChart::ReleaseTLIGHT();
	CTChart::ReleaseTFOG();
	
	ReleaseTRECTVB();
	ReleaseTRESIMG();

	CTachyonWnd::OnDestroy();
}

void CTExplorerWnd::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTExplorerWnd::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnKeyUp( nChar, nRepCnt, nFlags);
}

void CTExplorerWnd::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTachyonWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CTExplorerWnd::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonDown(nFlags, point);

	CTachyonWnd::OnLButtonDown(nFlags, point);
}

void CTExplorerWnd::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonUp(nFlags, point);

	CTachyonWnd::OnLButtonUp(nFlags, point);
}

void CTExplorerWnd::OnMButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonDown(nFlags, point);

	CTachyonWnd::OnMButtonDown(nFlags, point);
}

void CTExplorerWnd::OnMButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonUp(nFlags, point);

	CTachyonWnd::OnMButtonUp(nFlags, point);
}

void CTExplorerWnd::OnRButtonDown( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonDown(nFlags, point);

	CTachyonWnd::OnRButtonDown(nFlags, point);
}

void CTExplorerWnd::OnRButtonUp( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonUp(nFlags, point);

	CTachyonWnd::OnRButtonUp(nFlags, point);
}

void CTExplorerWnd::OnRButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnRButtonDblClk(nFlags, point);

	CTachyonWnd::OnRButtonDblClk(nFlags, point);
}

void CTExplorerWnd::OnMButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnMButtonDblClk(nFlags, point);

	CTachyonWnd::OnMButtonDblClk(nFlags, point);
}

void CTExplorerWnd::OnLButtonDblClk( UINT nFlags, CPoint point)
{
	if(m_pMainFrame)
		m_pMainFrame->OnLButtonDblClk(nFlags, point);

	CTachyonWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CTExplorerWnd::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(m_pMainFrame)
		m_pMainFrame->DoMouseWheel( nFlags, zDelta, pt);

	return CTachyonWnd::OnMouseWheel( nFlags, zDelta, pt);
}

void CTExplorerWnd::ReFresh()
{
	if(m_pMainFrame)
	{
		CPoint point;

		GetCursorPos(&point);
		ScreenToClient(&point);

		m_pMainFrame->OnMouseMove( 0, point);
	}
}

DWORD CTExplorerWnd::GetTExplorerTickCount()
{
	return GetTickCount() - CTExplorerApp::m_dwAppTick;
}

LRESULT CTExplorerWnd::OnOBJMessage( WPARAM wParam, LPARAM lParam)
{
	return m_MainGame.OnOBJMsg((LPOBJMSG) wParam);
}

void CTExplorerWnd::RestoreDevice()
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
