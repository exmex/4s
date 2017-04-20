#include "StdAfx.h"
#include "TClientGame.h"
#include "Resource.h"

CTWorldmapDlg::CTWorldmapDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:CTClientUIBase( pParent, pDesc)
{
	m_pTMYPOS = FindKid( ID_CTRLINST_TXT_MY_POS );
	m_pTCURSORPOS = FindKid( ID_CTRLINST_TXT_CURSOR_POS );

	m_pTALLWORLDMAP = FindKid(ID_CTRLINST_WORLD);
	RemoveKid(m_pTALLWORLDMAP);
    
	m_pTWORLDMAP = (TImageList *) FindKid(ID_CTRLINST_MAP);
	RemoveKid(m_pTWORLDMAP);

	m_pTITLE = (TImageList *) FindKid(ID_CTRLINST_TITLE);
	m_pTCHAR = (TImageList *) FindKid(ID_CTRLINST_PC);
	RemoveKid(m_pTCHAR);

	m_pTNPC = (TImageList *) FindKid(ID_CTRLINST_NPC);
	RemoveKid(m_pTNPC);

	m_pTPURSUIT = FindKid(ID_CTRLINST_PURSUIT);
	RemoveKid(m_pTPURSUIT);

	m_pTDEAD = FindKid(ID_CTRLINST_DEAD);
	RemoveKid(m_pTDEAD);

	m_pTEXT = FindKid(ID_CTRLINST_NPCNAME);
	RemoveKid(m_pTEXT);

	CD3DFont *pTFONT = m_pTEXT->Get3DFont();
	pTFONT->m_dwLineColor = TTEXTSHADOW_COLOR;
	pTFONT->m_bOutLine = TRUE;

	m_pNPCNAME = FindKid(ID_CTRLINST_NPCNAME2);
	m_pNPCNAME->m_bWordBreak = FALSE;
	RemoveKid(m_pNPCNAME);

	pTFONT = m_pNPCNAME->Get3DFont();
	pTFONT->m_dwLineColor = TTEXTSHADOW_COLOR;
	pTFONT->m_bOutLine = TRUE;

	m_vTCENTER = D3DXVECTOR2( 0.0f, 0.0f);
	m_fTSCALE = 1.0f;

	m_pDevice = NULL;
	m_pTRSCS = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pTMON = NULL;
	m_pCAM = NULL;
	m_bMOVE = FALSE;
	m_bTALLWORLDMODE = FALSE;

	m_pTLASTWBTN = NULL;
	
	m_nMainUnitX = T_INVALID;
	m_nMainUnitZ = T_INVALID;
}

CTWorldmapDlg::~CTWorldmapDlg()
{
	for(INT i=0; i<INT(m_vTWORLDBUTTON.size()); ++i)
		delete m_vTWORLDBUTTON[i];

	if(m_pTALLWORLDMAP)
		delete m_pTALLWORLDMAP;

	if(m_pTWORLDMAP)
		delete m_pTWORLDMAP;

	if(m_pTPURSUIT)
		delete m_pTPURSUIT;

	if(m_pTCHAR)
		delete m_pTCHAR;

	if(m_pTNPC)
		delete m_pTNPC;

	if(m_pTDEAD)
		delete m_pTDEAD;

	if(m_pTEXT)
		delete m_pTEXT;

	if(m_pNPCNAME)
		delete m_pNPCNAME;
}

BOOL CTWorldmapDlg::HitTest( CPoint pt)
{
	return IsVisible() ? TRUE : FALSE;
}

void CTWorldmapDlg::DefaultPosition( CPoint*, BOOL bRestore )
{
	static const INT TWORLDMAP_COMP_COUNT = 10;

	CPoint vBASIS[9] =
	{
		CPoint( 0, 0 ),
		CPoint( 1280/2, 0 ),
		CPoint( 1280, 0 ),
		
		CPoint( 0, 960/2 ),
		CPoint( 1280/2, 960/2 ),
		CPoint( 1280, 960/2 ),

		CPoint( 0, 960 ),
		CPoint( 1280/2, 960 ),
		CPoint( 1280, 960 )
	};

	static DWORD vCOMP[TWORLDMAP_COMP_COUNT][2] =
	{
		{ ID_CTRLINST_FRAME_UP, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_FRAME_DOWN, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_FRAME_LEFT, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_FRAME_RIGHT, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_TITLE, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_ZOOM_OUT, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_ZOOM_IN, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_CLOSE, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_TXT_MY_POS, TBASISPOINT_CENTER_MIDDLE },
		{ ID_CTRLINST_TXT_CURSOR_POS, TBASISPOINT_CENTER_MIDDLE },
	};
	static CPoint vCOMP_OFFSET[ TWORLDMAP_COMP_COUNT];
	static CPoint vMAP_OFFSET;	

	if( bRestore )
	{
		for( INT i=0 ; i < TWORLDMAP_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = (TComponent*) FindKid( vCOMP[ i ][0] );
			if( pCOMP )
			{
				pCOMP->MoveComponent(
					CTClientUIBase::m_vBasis[ vCOMP[i][1] ] + vCOMP_OFFSET[i] );
			}
		}

		m_pTALLWORLDMAP->MoveComponent(
					CTClientUIBase::m_vBasis[ TBASISPOINT_CENTER_MIDDLE ] + vMAP_OFFSET );

		INT nCount = m_vTWORLDBUTTON.size();
		for( INT i=0 ; i < nCount ; ++i )
		{
			TFRAMEOFFSET frame_offset = m_vTWORLDBUTTON_OFFSET[ i ];
			m_vTWORLDBUTTON[ i ]->MoveComponent(
				CTClientUIBase::m_vBasis[frame_offset.m_bBasisPoint] + frame_offset.m_vOffset );
		}
	}
	else
	{
		for( INT i=0 ; i < TWORLDMAP_COMP_COUNT ; ++i )
		{
			TComponent* pCOMP = (TComponent*) FindKid( vCOMP[ i ][0] );
			if( pCOMP )
			{
				CPoint pt;
				pCOMP->GetComponentPos(&pt);
				pt -= vBASIS[ vCOMP[i][1] ];

				vCOMP_OFFSET[i] = pt;
			}
		}

		CPoint pt;
		m_pTALLWORLDMAP->GetComponentPos(&pt);
		pt -= vBASIS[ TBASISPOINT_CENTER_MIDDLE ];
		vMAP_OFFSET = pt;

		TCOMP_ARRAY::iterator it, end;
		it = m_vTWORLDBUTTON.begin();
		end = m_vTWORLDBUTTON.end();

		for(; it != end ; ++it )
		{
			CPoint pt;
			(*it)->GetComponentPos( &pt );
			pt -= vBASIS[ TBASISPOINT_CENTER_MIDDLE ];

			m_vTWORLDBUTTON_OFFSET.push_back( 
				TFRAMEOFFSET(0,TBASISPOINT_CENTER_MIDDLE, pt));
		}
	}
}

void CTWorldmapDlg::ResetPosition()
{
	DefaultPosition( NULL, TRUE );
}

CPoint CTWorldmapDlg::GetPosition( FLOAT fPosX,
								   FLOAT fPosZ,
								   int nWidth,
								   int nHeight)
{
	FLOAT fSCALE = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength) / m_fTSCALE;
	CPoint vRESULT(
		INT(m_pDevice->m_option.m_dwScreenX / 2.0f),
		INT(m_pDevice->m_option.m_dwScreenY / 2.0f));

	fPosX -= m_vTCENTER.x;
	fPosZ -= m_vTCENTER.y;

	vRESULT.x += INT(fPosX * FLOAT(TWORLDMAP_SIZE) / fSCALE);
	vRESULT.y -= INT(fPosZ * FLOAT(TWORLDMAP_SIZE) / fSCALE);

	vRESULT.x -= nWidth / 2;
	vRESULT.y -= nHeight / 2;

	return vRESULT;
}

D3DXVECTOR2 CTWorldmapDlg::GetPositionInverse( CPoint vRESULT,
											  int nWidth,
											  int nHeight)
{
	FLOAT fSCALE = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength) / m_fTSCALE;

	return D3DXVECTOR2(
		((vRESULT.x+(FLOAT)(nWidth)/2-m_pDevice->m_option.m_dwScreenX/2.0f)*fSCALE / FLOAT(TWORLDMAP_SIZE))+ m_vTCENTER.x,
        (-(vRESULT.y+(FLOAT)(nHeight)/2-m_pDevice->m_option.m_dwScreenY/2.0f)*fSCALE / FLOAT(TWORLDMAP_SIZE))+ m_vTCENTER.y);
}

TComponent* CTWorldmapDlg::GetHitWorldButton(const CPoint& pt)
{
	for(INT i=0; i<INT(m_vTWORLDBUTTON.size()); ++i)
	{
		TComponent* pTCAND = m_vTWORLDBUTTON[i];
		if( pTCAND->HitTest(pt) )
			return pTCAND;
	}

	return NULL;
}

DWORD CTWorldmapDlg::MakeUnitID(WORD wMapID, BYTE nUnitX, BYTE nUnitY)
{
	return MAKELONG( MAKEWORD(nUnitX,nUnitY), wMapID );
}

void CTWorldmapDlg::GetUnitPosition(FLOAT fPosX, FLOAT fPosY, INT& outUnitX, INT& outUnitY)
{
	FLOAT fLength = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength);
	
	outUnitX = INT(fPosX / fLength);
	outUnitY = INT(fPosY / fLength);
}

BYTE CTWorldmapDlg::GetWorldmapLevel(FLOAT fPosX, FLOAT fPosY)
{
	INT nUNITX, nUNITZ;
	GetUnitPosition(fPosX,fPosY, nUNITX,nUNITZ);

	DWORD dwUNITID = MakeUnitID(m_pTMAP->m_wMapID, (BYTE)nUNITX,(BYTE)nUNITZ);
	MAPTMINIMAP::iterator itr = CTChart::m_mapTMINIMAP.find(dwUNITID);
	if( itr == CTChart::m_mapTMINIMAP.end() )
		return TWORLDMAP_LEVEL_NONE;

	return (itr->second)->m_bWorldMapLevel;
}

void CTWorldmapDlg::ShowComponent( BOOL bVisible )
{
	CTClientUIBase::ShowComponent( bVisible );

	if( !bVisible )
	{
		if( m_pDevice->m_pBACKBUF2 )
		{
			m_pDevice->m_pBACKBUF2->Release();
			m_pDevice->m_pBACKBUF2 = NULL;
		}

		if( m_pDevice->m_pBACKTEX2 )
		{
			m_pDevice->m_pBACKTEX2->Release();
			m_pDevice->m_pBACKTEX2 = NULL;
		}
	}
}

void CTWorldmapDlg::OnLButtonDown( UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown( nFlags, pt);

	if( IsVisible() && !TFrame::HitTest(pt) )
	{
		if(m_bTALLWORLDMODE)
			m_pTLASTWBTN = GetHitWorldButton(pt);
#ifdef TEST_MODE
		else if( nFlags & MK_CONTROL && m_pHost )
		{
			D3DXVECTOR2 vTARGET = GetPositionInverse( pt, 1, 1);

			m_pHost->m_vWorld._41 += vTARGET.x - m_pHost->GetPositionX();
			m_pHost->m_vWorld._43 += vTARGET.y - m_pHost->GetPositionZ();
			m_pHost->SetPositionX(vTARGET.x);
			m_pHost->SetPositionZ(vTARGET.y);
		}
#endif
		else if( m_pHost && m_pTMAP && m_pTMAP->m_pMAP )
		{
			D3DXVECTOR2 vTARGET = GetPositionInverse( pt, 1, 1);
			D3DXVECTOR3 vTPOS = m_pHost->GetPosition();
			TTERMPOS vTAUTOPATH;

			vTPOS.y += m_pHost->m_fSizeY / 2.0f;
			vTPOS.x = vTARGET.x;
			vTPOS.z = vTARGET.y;

			vTPOS.y = m_pTMAP->GetHeight(
				m_pHost,
				NULL,
				&vTPOS,
				0.0f,
				FALSE);

			ZeroMemory( &vTAUTOPATH, sizeof(TTERMPOS));
			vTAUTOPATH.m_dwMapID = m_pTMAP->m_wMapID;
			vTAUTOPATH.m_fPosX = vTPOS.x;
			vTAUTOPATH.m_fPosY = vTPOS.y;
			vTAUTOPATH.m_fPosZ = vTPOS.z;

			CTClientGame::GetInstance()->DoTAUTOPATH(&vTAUTOPATH);
		}
	}
}

void CTWorldmapDlg::OnRButtonDown( UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnRButtonDown( nFlags, pt);

	if( IsVisible() && !TFrame::HitTest(pt) && !m_bTALLWORLDMODE )
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		ShowCursor(FALSE);
		SetCursorPos(
			point.x,
			point.y);

		m_bMOVE = TRUE;
	}
}

void CTWorldmapDlg::OnMouseMove( UINT nFlags, CPoint pt)
{
	if( !IsVisible() )
	{
		CTClientUIBase::OnMouseMove( nFlags, pt);
		return;
	}

	if( m_bTALLWORLDMODE )
	{
		TComponent* pTHIT = GetHitWorldButton(pt);

		if( pTHIT )
		{
			MAPDWORD::iterator itr = CTChart::m_mapTBTNID2WORLD.find(pTHIT->m_id);
			if( itr != CTChart::m_mapTBTNID2WORLD.end() )
				m_pTITLE->SetCurImage(itr->second);
		}

		for(INT i=0; i<INT(m_vTWORLDBUTTON.size()); ++i)
			m_vTWORLDBUTTON[i]->OnMouseMove(nFlags, pt);
	}
	else if( m_bMOVE )
	{
		FLOAT fLength = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength);
		FLOAT fMinX = fLength / (2.0f * m_fTSCALE);
		FLOAT fMaxX = fLength - fMinX;
		FLOAT fMinZ = fMinX * FLOAT(m_pDevice->m_option.m_dwScreenY) / FLOAT(m_pDevice->m_option.m_dwScreenX);
		FLOAT fMaxZ = fLength - fMinZ;

		int nUnitX = INT(m_vTCENTER.x / fLength);
		int nUnitZ = INT(m_vTCENTER.y / fLength);

		fMinX += FLOAT(nUnitX) * fLength;
		fMaxX += FLOAT(nUnitX) * fLength;
		fMinZ += FLOAT(nUnitZ) * fLength;
		fMaxZ += FLOAT(nUnitZ) * fLength;

		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		SetCursorPos(
			point.x,
			point.y);
		m_pCommandHandler->ScreenToClient(&point);
		point -= pt;

		m_vTCENTER.x += fLength * FLOAT(point.x) / (m_fTSCALE * FLOAT(TWORLDMAP_SIZE));
		m_vTCENTER.y -= fLength * FLOAT(point.y) / (m_fTSCALE * FLOAT(TWORLDMAP_SIZE));

		m_vTCENTER.x = min( max( m_vTCENTER.x, fMinX), fMaxX);
		m_vTCENTER.y = min( max( m_vTCENTER.y, fMinZ), fMaxZ);

		m_pTCURSORPOS->ShowComponent(FALSE);
	}
	else if( !m_bTALLWORLDMODE && !m_bMOVE)
	{
		CRect rt(0,0,0,0);

		if( m_pTALLWORLDMAP)
		{
			m_pTALLWORLDMAP->GetComponentRect( &rt );
			m_pTALLWORLDMAP->ComponentToScreen( &rt );
		}

		if( rt.PtInRect( pt ))
		{
			m_pTCURSORPOS->ShowComponent(TRUE);

			D3DXVECTOR2 vPOS_INVERSE = GetPositionInverse( pt, 1, 1 );
			if( m_pTCURSORPOS)
				m_pTCURSORPOS->m_strText = CTChart::Format( TSTR_CURSOR_POS,
					vPOS_INVERSE.x,
					vPOS_INVERSE.y );
		}
		else
			m_pTCURSORPOS->ShowComponent(FALSE);
	}

	CTClientUIBase::OnMouseMove( nFlags, pt);
}

void CTWorldmapDlg::OnLButtonUp( UINT nFlags, CPoint pt)
{
	if( IsVisible() && m_bTALLWORLDMODE &&
		m_pCommandHandler && m_pTLASTWBTN )
	{
		TComponent *pTHIT = GetHitWorldButton(pt);

		if( pTHIT && pTHIT == m_pTLASTWBTN )
			m_pCommandHandler->m_vCOMMAND.push_back(pTHIT->m_menu[TNM_LCLICK]);
	}

	CTClientUIBase::OnLButtonUp( nFlags, pt);
}

void CTWorldmapDlg::OnRButtonUp( UINT nFlags, CPoint pt)
{
	if(m_bMOVE)
	{
		CPoint point(
			m_pDevice->m_option.m_dwScreenX / 2,
			m_pDevice->m_option.m_dwScreenY / 2);

		m_pCommandHandler->ClientToScreen(&point);
		SetCursorPos(
			point.x,
			point.y);
		ShowCursor(TRUE);

		m_bMOVE = FALSE;
	}

	CTClientUIBase::OnRButtonUp( nFlags, pt);
}

void CTWorldmapDlg::RenderALLWORLDMAP( DWORD dwTickCount)
{
	MAPTMINIMAP::iterator finder = CTChart::m_mapTMINIMAP.find(
		MakeUnitID(m_pTMAP->m_wMapID, (BYTE)m_nMainUnitX, (BYTE)m_nMainUnitZ));

	DWORD dwCURWORLDBTN = 0;
	if( finder != CTChart::m_mapTMINIMAP.end() )
		dwCURWORLDBTN = finder->second->m_dwWorldButtonID;

	DrawBack();
	m_pTALLWORLDMAP->Render(dwTickCount);

	for(INT i=0; i<INT(m_vTWORLDBUTTON.size()); ++i)
	{
		if( m_vTWORLDBUTTON[i]->m_id == dwCURWORLDBTN )
			( (TButton*) m_vTWORLDBUTTON[i] )->Select(TRUE);

		m_vTWORLDBUTTON[i]->Render(dwTickCount);
	}
}

void CTWorldmapDlg::RenderWORLDMAP( DWORD dwTickCount)
{
	if( m_pDevice->m_pBACKTEX2 == NULL )
	{
		D3DXCreateTexture(
			m_pDevice->m_pDevice,
			m_pDevice->m_vPRESENT.BackBufferWidth,
			m_pDevice->m_vPRESENT.BackBufferHeight, 1,
			D3DUSAGE_RENDERTARGET,
			m_pDevice->m_vPRESENT.BackBufferFormat,
			D3DPOOL_DEFAULT,
			&(m_pDevice->m_pBACKTEX2));

		if( !m_pDevice->m_pBACKTEX2 )
			return ;

		m_pDevice->m_pBACKTEX2->GetSurfaceLevel( 0, &(m_pDevice->m_pBACKBUF2));
	}

	SIZE sizeFRAME;
	m_pTALLWORLDMAP->GetComponentSize( &sizeFRAME);

	FLOAT fLength = ((FLOAT) m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength);
	FLOAT fMinX = fLength / (2.0f * m_fTSCALE);
	FLOAT fMaxX = fLength - fMinX;
	FLOAT fMinZ = fMinX * ( FLOAT(sizeFRAME.cy) / FLOAT(sizeFRAME.cx) );
	FLOAT fMaxZ = fLength - fMinZ;
	BYTE bResult = FALSE;

	int nUnitX, nUnitZ, nMainUnitX, nMainUnitZ;

	GetUnitPosition(m_vTCENTER.x,m_vTCENTER.y, nUnitX,nUnitZ);
	GetUnitPosition(m_pHost->GetPositionX(),m_pHost->GetPositionZ(), nMainUnitX,nMainUnitZ);

	if( m_nMainUnitX != nMainUnitX || m_nMainUnitZ != nMainUnitZ )
	{
		if( m_nMainUnitX != T_INVALID && m_nMainUnitZ != T_INVALID &&
			m_nMainUnitX == nUnitX && m_nMainUnitZ == nUnitZ )
		{
			m_vTCENTER.x = m_pHost->GetPositionX();
			m_vTCENTER.y = m_pHost->GetPositionZ();

			nUnitX = nMainUnitX;
			nUnitZ = nMainUnitZ;
		}

		m_nMainUnitX = nMainUnitX;
		m_nMainUnitZ = nMainUnitZ;
	}

	m_bDrawNPC = (nUnitX == nMainUnitX) && (nUnitZ == nMainUnitZ);

	fMinX += FLOAT(nUnitX) * fLength;
	fMaxX += FLOAT(nUnitX) * fLength;
	fMinZ += FLOAT(nUnitZ) * fLength;
	fMaxZ += FLOAT(nUnitZ) * fLength;

	m_vTCENTER.x = min( max( m_vTCENTER.x, fMinX), fMaxX);
	m_vTCENTER.y = min( max( m_vTCENTER.y, fMinZ), fMaxZ);

	MAPTMINIMAP::iterator finder = CTChart::m_mapTMINIMAP.find(
		MakeUnitID(m_pTMAP->m_wMapID, (BYTE)nUnitX, (BYTE)nUnitZ));

	if( finder != CTChart::m_mapTMINIMAP.end() )
	{
		int nPosX = INT((FLOAT(nUnitX) * fLength + fLength / 2.0f - m_vTCENTER.x) * TWORLDMAP_SIZE / fLength) + INT(m_pDevice->m_option.m_dwScreenX - TWORLDMAP_SIZE) / 2;
		int nPosZ = INT((FLOAT(nUnitZ) * fLength + fLength / 2.0f - m_vTCENTER.y) * TWORLDMAP_SIZE / fLength) - INT(m_pDevice->m_option.m_dwScreenY - TWORLDMAP_SIZE) / 2;

		CTClientCAM vCAMERA;
		D3DXMATRIX vWorld;

		D3DXMatrixScaling(
			&vWorld,
			m_fTSCALE,
			m_fTSCALE,
			m_fTSCALE);

		vCAMERA.InitOrthoCamera(
			m_pDevice->m_pDevice,
			-1.0f,
			1.0f,
			1.0f,
			1.0f );

		vCAMERA.SetPosition(
			D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
			FALSE);
		vCAMERA.Activate(TRUE);

		m_pTWORLDMAP->SetCurImage((*finder).second->m_bWorldID);
		m_pTITLE->SetCurImage((*finder).second->m_bWorldID);

		m_pTWORLDMAP->MoveComponent(CPoint( nPosX, -nPosZ));
		m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pBACKBUF);

		m_pDevice->m_pDevice->Clear(
			0, NULL,
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB( 0, 0, 0, 0),
			1.0f, 0);
		
		m_pTWORLDMAP->Render(dwTickCount);
		vCAMERA.Activate(TRUE);

		m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pBACKBUF2);

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

		m_pDevice->m_pDevice->SetStreamSource( 0, m_pDevice->m_pBACKVB, 0, sizeof(PVERTEX));
		m_pDevice->m_pDevice->SetTexture( 0, m_pDevice->m_pBACKTEX);

		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
		m_pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
		m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

		CRect rect( 0, 0, m_pDevice->m_option.m_dwScreenX, m_pDevice->m_option.m_dwScreenY);

		TNLVERTEX vRECT[4] = {
			{ FLOAT(rect.left)+0.5f, FLOAT(rect.top)+0.5f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f},
			{ FLOAT(rect.right)+0.5f, FLOAT(rect.top)+0.5f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f},
			{ FLOAT(rect.left)+0.5f, FLOAT(rect.bottom)+0.5f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f},
			{ FLOAT(rect.right)+0.5f, FLOAT(rect.bottom)+0.5f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f}
		};

		m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pRTARGET);
		
		DrawBack();

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

		m_pDevice->m_pDevice->SetTexture( 0, m_pDevice->m_pBACKTEX2);
		m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

		D3DVIEWPORT9 vVIEW;
		m_pDevice->m_pDevice->GetViewport(&vVIEW);

		CRect rtBack;
		m_pTALLWORLDMAP->GetComponentRect(&rtBack);
		m_pTALLWORLDMAP->ComponentToScreen( &rtBack );

		D3DVIEWPORT9 vCULLRECT;
		vCULLRECT.X = (DWORD) (rtBack.left > 0 ? rtBack.left + 1 : 1);
		vCULLRECT.Y = (DWORD) (rtBack.top > 0 ? rtBack.top + 1 : 1);
		vCULLRECT.Width = (DWORD) rtBack.Width();
		vCULLRECT.Height = (DWORD) rtBack.Height();
		vCULLRECT.MinZ = 0.0f;
		vCULLRECT.MaxZ = 1.0f;

		if( vVIEW.Y + vVIEW.Height < vCULLRECT.Y + vCULLRECT.Height )
			vCULLRECT.Height = vVIEW.Y + vVIEW.Height - vCULLRECT.Y;

		if( vVIEW.X + vVIEW.Width < vCULLRECT.X + vCULLRECT.Width )
			vCULLRECT.Width = vVIEW.X + vVIEW.Width - vCULLRECT.X;
		m_pDevice->m_pDevice->SetViewport(&vCULLRECT);

		m_pDevice->m_pDevice->DrawPrimitiveUP(
			D3DPT_TRIANGLESTRIP,
			2, vRECT, sizeof(TNLVERTEX));

		m_pDevice->m_pDevice->SetViewport(&vVIEW);
		m_pCAM->Activate(TRUE);
	}
}

void CTWorldmapDlg::RenderOBJ( DWORD dwTickCount)
{
	D3DVIEWPORT9 vVIEW;
	m_pDevice->m_pDevice->GetViewport(&vVIEW);
	
	CRect rtBack;
	m_pTALLWORLDMAP->GetComponentRect(&rtBack);
	m_pTALLWORLDMAP->ComponentToScreen( &rtBack );

	D3DVIEWPORT9 vRECT;
	vRECT.X = (DWORD)( rtBack.left > 0 ? rtBack.left : 0  );
	vRECT.Y = (DWORD)( rtBack.top > 0 ? rtBack.top : 0 );
	vRECT.Width = (DWORD)( rtBack.Width() );
	vRECT.Height = (DWORD)( rtBack.Height() );
	vRECT.MinZ = 0.0f;
	vRECT.MaxZ = 1.0f;

	m_pDevice->m_pDevice->SetViewport(&vRECT);
	CD3DImage::m_bUseGlobalClipRect = TRUE;

	CRect rect(
		0, 0,
		m_pDevice->m_option.m_dwScreenX,
		m_pDevice->m_option.m_dwScreenY);

	CTClientChar *pTHITCHAR = NULL;
	LPTUNIT pTHITUNIT = NULL;
	static const int MAX_THITNPC = 10;
	LPTNPC pTHITNPC[MAX_THITNPC] = { NULL, };
	INT nTHITNPC=0;

	CSize szTEXT;
	CSize size;
	CPoint pt;
	D3DXVECTOR3 vPOS;

	m_pTEXT->GetComponentSize(&szTEXT);
	m_pTEXT->ShowComponent(TRUE);

	m_pTNPC->GetComponentSize(&size);
	m_pTNPC->ShowComponent(TRUE);
	rect.InflateRect(size);

	if(!m_bMOVE)
	{
		GetCursorPos(&pt);
		m_pCommandHandler->ScreenToClient(&pt);
	}

	if(m_bDrawNPC)
	{
		for( BYTE i=0; i<9; i++)
			if(m_pTMAP->m_pMAP->m_pUNIT[i])
				for( WORD j=0; j<TOBJCELL_COUNT; j++)
					for( WORD k=0; k<WORD(m_pTMAP->m_vTMAPNPC[m_pTMAP->m_pMAP->m_bUNIT[i]][j].size()); k++)
					{
						LPTNPC pTNPC = m_pTMAP->m_vTMAPNPC[m_pTMAP->m_pMAP->m_bUNIT[i]][j][k];
						CPoint point = GetPosition(
							pTNPC->m_fPosX,
							pTNPC->m_fPosZ,
							size.cx,
							size.cy);

						if( (pTNPC->m_bDrawMark & (1 << TDRAWMARK_WORLDMAP)) && rect.PtInRect(point) && CheckRequireItem(pTNPC) )
						{
							BYTE bDRAW = m_pHost->m_bGhost ? FALSE : TRUE;

							switch(pTNPC->m_bNPCType)
							{
							case TNPC_SKILL_MASTER	:
								if( pTNPC->m_bClassID & (1 << m_pHost->m_bClassID) )
									m_pTNPC->SetCurImage(CTClientGame::m_vTNPCICONIDX[pTNPC->m_bNPCType]);
								else
									m_pTNPC->SetCurImage(0);

								break;

							case TNPC_BOX			: bDRAW = FALSE; break;
							case TNPC_REVIVAL		: bDRAW = TRUE;
							case TNPC_ITEM			:
							case TNPC_PORTAL		: m_pTNPC->SetCurImage(CTClientGame::m_vTNPCICONIDX[pTNPC->m_bNPCType]); break;
							default					: m_pTNPC->SetCurImage(0); break;
							}

							if(bDRAW)
							{
								m_pTNPC->MoveComponent(point);
								m_pTNPC->Render(dwTickCount);

								if( !m_bMOVE && m_pTNPC->HitTest(pt) )
								{
									if( nTHITNPC < MAX_THITNPC )
										pTHITNPC[ nTHITNPC++ ] = pTNPC;
								}
							}
						}
					}
	}

	if(m_pHost->m_bGhost)
	{
		rect.DeflateRect(size);
		m_pTDEAD->GetComponentSize(&size);
		m_pTDEAD->ShowComponent(TRUE);
		rect.InflateRect(size);
		
		vPOS = m_pDead->GetPosition();
		CPoint point = GetPosition(vPOS.x,vPOS.z, size.cx,size.cy);

		if(rect.PtInRect(point))
		{
			m_pTDEAD->MoveComponent(point);
			m_pTDEAD->Render(dwTickCount);
		}

		rect.DeflateRect(size);
		m_pTCHAR->GetComponentSize(&size);
		m_pTCHAR->ShowComponent(TRUE);
		rect.InflateRect(size);
	}
	else
	{
		if( m_pTMON && m_pHost->m_wPursuit != 0xFFFF )
		{
			CTClientBSP *pTBSP = NULL;

			rect.DeflateRect(size);
			m_pTPURSUIT->GetComponentSize(&size);
			m_pTPURSUIT->ShowComponent(TRUE);
			rect.InflateRect(size);

			if( m_pTMAP->m_dwBspID != NODEID_NULL )
			{
				MAPTBSP::iterator itTBSP = m_pTMAP->m_mapTBsp.find(m_pTMAP->m_dwBspID);

				if( itTBSP != m_pTMAP->m_mapTBsp.end() )
					pTBSP = (*itTBSP).second;
			}

			if(pTBSP)
			{
				MAPMONSTER::iterator itTMON;

				for( itTMON = m_pTMON->begin(); itTMON != m_pTMON->end(); itTMON++)
					if( (*itTMON).second && (*itTMON).second->m_pMON && (*itTMON).second->m_pMON->m_wKind == m_pHost->m_wPursuit )
					{
						vPOS = (*itTMON).second->GetPosition();
						DWORD dwNodeID = pTBSP->GetNodeID(vPOS);

						if( dwNodeID != NODEID_NULL )
						{
							CPoint point = GetPosition(vPOS.x,vPOS.z, size.cx,size.cy);

							if(rect.PtInRect(point))
							{
								m_pTPURSUIT->MoveComponent(point);
								m_pTPURSUIT->Render(dwTickCount);
							}
						}
					}
			}
			else
			{
				MAPMONSTER::iterator itTMON;

				for( itTMON = m_pTMON->begin(); itTMON != m_pTMON->end(); itTMON++)
					if( (*itTMON).second && (*itTMON).second->m_pMON && (*itTMON).second->m_pMON->m_wKind == m_pHost->m_wPursuit )
					{
						DWORD dwBspID = NODEID_NULL;

						for( int i=0; i<INT(m_pTMAP->m_vDRAWBSP.size()); i++)
						{
							vPOS = (*itTMON).second->GetPosition();
							DWORD dwNodeID = m_pTMAP->m_vDRAWBSP[i]->GetNodeID(vPOS);

							if( dwNodeID != NODEID_NULL )
							{
								dwBspID = m_pTMAP->m_vDRAWBSP[i]->m_dwBspID;
								break;
							}
						}

						if( dwBspID == NODEID_NULL )
						{
							vPOS = (*itTMON).second->GetPosition();
							CPoint point = GetPosition(vPOS.x,vPOS.z, size.cx,size.cy);

							if(rect.PtInRect(point))
							{
								m_pTPURSUIT->MoveComponent(point);
								m_pTPURSUIT->Render(dwTickCount);
							}
						}
					}
			}
		}

		rect.DeflateRect(size);
		m_pTCHAR->GetComponentSize(&size);
		m_pTCHAR->ShowComponent(TRUE);
		rect.InflateRect(size);

		for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
			for( BYTE j=0; j<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
			{
				LPTUNIT pTUNIT = m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j];
				D3DXVECTOR2 vTPOS = m_pTRSCS->GetUnitPOS(pTUNIT);

				CPoint point = GetPosition(
					vTPOS.x,
					vTPOS.y,
					size.cx,
					size.cy);

				if( pTUNIT->m_dwCharID != m_pHost->m_dwID && rect.PtInRect(point) )
				{
					if( pTUNIT->m_dwCharID == m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_dwChiefID )
						m_pTCHAR->SetCurImage(TWORLDMAP_SQUAD_CHIEF);
					else
						m_pTCHAR->SetCurImage(TWORLDMAP_SQUAD_UNIT);

					m_pTCHAR->MoveComponent(point);
					m_pTCHAR->Render(dwTickCount);

					if( !m_bMOVE && m_pTCHAR->HitTest(pt) )
						pTHITUNIT = pTUNIT;
				}
			}
	}

	vPOS = m_pHost->GetPosition();
	CPoint point = GetPosition(vPOS.x,vPOS.z, size.cx,size.cy);

	if(rect.PtInRect(point))
	{
		m_pTCHAR->SetCurImage(TWORLDMAP_HOST_CHAR);
		m_pTCHAR->MoveComponent(point);
		m_pTCHAR->Render(dwTickCount);

		if( !m_bMOVE && m_pTCHAR->HitTest(pt) )
			pTHITCHAR = m_pHost;

		m_pTMYPOS->m_strText = CTChart::Format( TSTR_MY_POS,
			vPOS.x,
			vPOS.z);
	}

	if(pTHITCHAR)
	{
		vPOS = pTHITCHAR->GetPosition();
		m_pTCHAR->GetComponentSize(&size);

		point = GetPosition(vPOS.x,vPOS.z, size.cx,size.cy);
		point.x -= (szTEXT.cx - size.cx) / 2;
		point.y -= szTEXT.cy;

		m_pTEXT->m_strText = pTHITCHAR->GetName();
		m_pTEXT->MoveComponent(point);
		m_pTEXT->Render(dwTickCount);
	}
	else if(pTHITUNIT)
	{
		D3DXVECTOR2 vTPOS = m_pTRSCS->GetUnitPOS(pTHITUNIT);

		m_pTCHAR->GetComponentSize(&size);
		point = GetPosition(
			vTPOS.x,
			vTPOS.y,
			size.cx,
			size.cy);

		point.x -= (szTEXT.cx - size.cx) / 2;
		point.y -= szTEXT.cy;

		m_pTEXT->m_strText = pTHITUNIT->m_strNAME;
		m_pTEXT->MoveComponent(point);
		m_pTEXT->Render(dwTickCount);
	}
	else if(pTHITNPC[0])
	{
		CString strRESULT;

		for( INT i=0 ; i < nTHITNPC ; ++i )
		{
			CString strTITLE = pTHITNPC[i]->m_strTITLE;
			CString strNAME = pTHITNPC[i]->m_strNAME;

			strTITLE.Remove('_');
			strNAME.Remove('_');

			CString TEXT;
			if( strTITLE.IsEmpty() )
				TEXT = CTChart::Format( TSTR_FMT_WORLDMAP_NAME, strTITLE, strNAME);
			else
				TEXT = CTChart::Format( TSTR_FMT_WORLDMAP_TITLE, strTITLE, strNAME);

			strRESULT += TEXT;
			strRESULT += '\n';
		}

		strRESULT.Left( strRESULT.GetLength()-1 );
		m_pTNPC->GetComponentSize(&size);

		point = GetPosition(
			pTHITNPC[0]->m_fPosX,
			pTHITNPC[0]->m_fPosZ,
			size.cx,
			size.cy);

		// NPC아이콘 위치가 화면 가장자리에 가까이 붙으면 반대방향쪽으로 글자정렬을 해서 잘 보이게 해준다.
		if( point.x < 80 )
			m_pNPCNAME->SetTextAlign( ALIGN_RIGHT );
		else if( point.x > m_pDevice->m_option.m_dwScreenX-80 )
			m_pNPCNAME->SetTextAlign( ALIGN_LEFT );
		else
			m_pNPCNAME->SetTextAlign( ALIGN_CENTER );
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		CSize szNPCNAME;
		m_pNPCNAME->GetComponentSize( &szNPCNAME );
		szNPCNAME.cy = 15*nTHITNPC;
        m_pNPCNAME->SetComponentSize( szNPCNAME );

		point.x -= (szNPCNAME.cx - size.cx) / 2;
		point.y -= szNPCNAME.cy;

		m_pNPCNAME->m_strText = strRESULT;
		m_pNPCNAME->MoveComponent(point);
		m_pNPCNAME->Render(dwTickCount);
	}

	m_pDevice->m_pDevice->SetViewport(&vVIEW);
	CD3DImage::m_bUseGlobalClipRect = FALSE;
}

HRESULT CTWorldmapDlg::Render( DWORD dwTickCount)
{
	if(IsVisible())
	{
		if( m_bTALLWORLDMODE )
		{
			RenderALLWORLDMAP(dwTickCount);
		}
		else
		{
			RenderWORLDMAP(dwTickCount);
			RenderOBJ(dwTickCount);
		}
	}

	return CTClientUIBase::Render(dwTickCount);
}

void CTWorldmapDlg::DrawBack()
{
	CTClientCAM vCAMERA;
	D3DXMATRIX vWorld;

	D3DXMatrixScaling(
		&vWorld,
		m_fTSCALE,
		m_fTSCALE,
		m_fTSCALE);

	vCAMERA.InitOrthoCamera(
		m_pDevice->m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	static LPOINT vVertices[] =
	{
		{ -0.5f, 0.0f,  0.5f, 0x99000000},
		{  0.5f, 0.0f,  0.5f, 0x99000000},
		{ -0.5f, 0.0f, -0.5f, 0x99000000},
		{  0.5f, 0.0f, -0.5f, 0x99000000}
	};

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA );
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	m_pDevice->m_pDevice->SetTexture( 0, NULL);
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_LPOINT);

	m_pDevice->m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vVertices, sizeof(LPOINT));
}

BYTE CTWorldmapDlg::CheckRequireItem( LPTNPC pTNPC)
{
	if( !m_pHost || !pTNPC )
		return FALSE;

	if(!pTNPC->m_wItemID)
		return TRUE;
	MAPTINVEN::iterator itINVEN;

	for( itINVEN = m_pHost->m_mapTINVEN.begin(); itINVEN != m_pHost->m_mapTINVEN.end(); itINVEN++)
	{
		CTClientInven *pTINVEN = (*itINVEN).second;

		if(pTINVEN)
		{
			MAPTITEM::iterator itTITEM;

			for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
				if( (*itTITEM).second && (*itTITEM).second->GetItemID() == pTNPC->m_wItemID )
					return TRUE;
		}
	}

	return FALSE;
}
