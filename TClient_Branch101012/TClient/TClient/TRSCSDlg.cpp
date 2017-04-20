#include "StdAfx.h"
#include "TClientGame.h"
#include "Resource.h"

LPDIRECT3DTEXTURE9 CTRSCSDlg::m_pTBACK = NULL;
LPDIRECT3DTEXTURE9 CTRSCSDlg::m_pTRSCS = NULL;

LPTEXTURESET CTRSCSDlg::m_pTARROW = NULL;
LPTEXTURESET CTRSCSDlg::m_pTMASK = NULL;


CTRSCSDlg::CTRSCSDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:CTClientUIBase( pParent, pDesc)
{
	static DWORD dwSquadNameID[TRSCSHOTKEY_COUNT] = {
		ID_CTRLINST_SQUAD_NAME_1,
		ID_CTRLINST_SQUAD_NAME_2,
		ID_CTRLINST_SQUAD_NAME_3,
		ID_CTRLINST_SQUAD_NAME_4,
		ID_CTRLINST_SQUAD_NAME_5,
		ID_CTRLINST_SQUAD_NAME_6,
		ID_CTRLINST_SQUAD_NAME_7};

	static DWORD dwSquadStatID[TRSCSHOTKEY_COUNT] = {
		ID_CTRLINST_SQUAD_STAT_1,
		ID_CTRLINST_SQUAD_STAT_2,
		ID_CTRLINST_SQUAD_STAT_3,
		ID_CTRLINST_SQUAD_STAT_4,
		ID_CTRLINST_SQUAD_STAT_5,
		ID_CTRLINST_SQUAD_STAT_6,
		ID_CTRLINST_SQUAD_STAT_7};

	static DWORD dwUnitNameID[TRSCSHOTKEY_COUNT] = {
		ID_CTRLINST_UNIT_NAME_1,
		ID_CTRLINST_UNIT_NAME_2,
		ID_CTRLINST_UNIT_NAME_3,
		ID_CTRLINST_UNIT_NAME_4,
		ID_CTRLINST_UNIT_NAME_5,
		ID_CTRLINST_UNIT_NAME_6,
		ID_CTRLINST_UNIT_NAME_7};

	static DWORD dwGaugeID[TRSCSHOTKEY_COUNT] = {
		ID_CTRLINST_GAUGE_1,
		ID_CTRLINST_GAUGE_2,
		ID_CTRLINST_GAUGE_3,
		ID_CTRLINST_GAUGE_4,
		ID_CTRLINST_GAUGE_5,
		ID_CTRLINST_GAUGE_6,
		ID_CTRLINST_GAUGE_7};
	
	static DWORD dwCompID[] = {
		ID_CTRLINST_MAX,
		ID_CTRLINST_WORLD,
		ID_CTRLINST_MINIMAP,
		ID_CTRLINST_ZOOM,
		ID_CTRLINST_TOGGLE_SQUAD,
		ID_CTRLINST_SEL_SQUAD};

	for( BYTE i=0; i<TRSCSCOMP_COUNT; i++)
		m_vTCOMP[i] = FindKid(dwCompID[i]);

	CRect rect;

	for( BYTE i=0; i<TRSCSHOTKEY_COUNT; i++)
	{
		m_pTSQUADNAME[i] = FindKid(dwSquadNameID[i]);
		m_pTSQUADSTAT[i] = FindKid(dwSquadStatID[i]);
		m_pTUNITNAME[i]  = FindKid(dwUnitNameID[i]);
		m_pTUNITGAUGE[i] = (TGauge *) FindKid(dwGaugeID[i]);

		m_pTSQUADNAME[i]->m_bNoHIT = TRUE;
		m_pTSQUADSTAT[i]->m_bNoHIT = TRUE;
		m_pTUNITNAME[i]->m_bNoHIT = TRUE;
		m_pTUNITGAUGE[i]->m_bNoHIT = TRUE;
	}

	m_pTITLE = FindKid(ID_CTRLINST_MAX_TITLE);
	m_pTSQUAD = (TList *) FindKid(ID_CTRLINST_SQUAD_LIST);
	m_pTUNIT = (TList *) FindKid(ID_CTRLINST_UNIT_LIST);
	m_pTDRAG = (TImageList *) FindKid(ID_CTRLINST_DRAGIMAGE);
	RemoveKid(m_pTDRAG);

	TComponent *pTPOS = FindKid(ID_CTRLINST_AREA);
	pTPOS->GetComponentRect(&m_vTAREA);
	
	m_pTBACKAREA = pTPOS;
	m_pTCURSQUAD = NULL;
	m_pTCURUNIT = NULL;

	m_bMOVE = FALSE;
	m_bSND = FALSE;

	m_pDevice = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;

	m_fTSCALE = TMINIMAP_INIT_SCALE / powf( TMINIMAP_SCALE_FACTOR, TMINIMAP_SCALE_COUNT);
	m_dwBspID = NODEID_NULL;
	m_wMapID = 0;

	m_vTCENTER = D3DXVECTOR2( 0.0f, 0.0f);
	m_vTALARM = D3DXVECTOR2( 0.0f, 0.0f);
	m_dwTALARM = 0;

	m_pTZOOMSCROLL = (TScroll*) m_vTCOMP[TRSCSCOMP_ZOOM];
	m_pTZOOMSCROLL->SetScrollPos((INT)TMINIMAP_SCALE_COUNT, (INT)TMINIMAP_SCALE_COUNT);

	m_bMinimize = FALSE;
	m_bHideSquad = FALSE;

	m_bDropping = FALSE;
}

CTRSCSDlg::~CTRSCSDlg()
{
	if(m_pTDRAG)
	{
		delete m_pTDRAG;
		m_pTDRAG = NULL;
	}
}

void CTRSCSDlg::ResetVisible()
{
	for( BYTE i=0; i<TRSCSCOMP_COUNT; i++)
	{
		m_vTCOMP[i]->EnableComponent(!m_bMinimize);
		m_vTCOMP[i]->ShowComponent(!m_bMinimize);
	}

	for( i=0; i<TRSCSHOTKEY_COUNT; i++ )
	{
		m_pTSQUADNAME[i]->EnableComponent(!m_bMinimize && !m_bHideSquad);
		m_pTSQUADNAME[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);

		m_pTSQUADSTAT[i]->EnableComponent(!m_bMinimize && !m_bHideSquad);
		m_pTSQUADSTAT[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);

		m_pTUNITNAME[i]->EnableComponent(!m_bMinimize && !m_bHideSquad);
		m_pTUNITNAME[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);

		m_pTUNITGAUGE[i]->EnableComponent(!m_bMinimize && !m_bHideSquad);
		m_pTUNITGAUGE[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);
	}

	m_pTSQUAD->EnableComponent(!m_bMinimize && !m_bHideSquad);
	m_pTSQUAD->ShowComponent(!m_bMinimize && !m_bHideSquad);

	m_pTUNIT->EnableComponent(!m_bMinimize && !m_bHideSquad);
	m_pTUNIT->ShowComponent(!m_bMinimize && !m_bHideSquad);

	if( !m_bMinimize && !m_bHideSquad )
		ResetCORPS();
}

void CTRSCSDlg::ResetCORPS()
{
	for( BYTE i=0; i<TRSCSHOTKEY_COUNT; i++)
	{
		m_pTSQUADNAME[i]->ShowComponent(FALSE);
		m_pTSQUADSTAT[i]->ShowComponent(FALSE);
	}

	m_pTSQUAD->RemoveAll();

	for( i=0; i<INT(m_vTCORPS.m_vTSQUAD.size()); i++)
	{
		static DWORD dwTCMD[TRSCSCMD_COUNT + 1] = {
			TSTR_TCMD_NONE,
			TSTR_TCMD_MOVE,
			TSTR_TCMD_ATTACK,
			TSTR_TCMD_PROTECT,
			TSTR_TCMD_NONE};
		int nIndex = m_pTSQUAD->AddString("");

		LPTUNIT pTUNIT = FindTUNIT(
			m_vTCORPS.m_vTSQUAD[i],
			m_vTCORPS.m_vTSQUAD[i]->m_dwChiefID);
		CString strNAME;

		m_pTSQUAD->SetItemData( nIndex, 0, (DWORD) m_vTCORPS.m_vTSQUAD[i]);
		if(pTUNIT)
		{
			strNAME = CTChart::Format( TSTR_FMT_SQUAD_NAME, pTUNIT->m_strNAME);
			m_pTSQUADNAME[nIndex]->m_strText = strNAME;

			strNAME = CTChart::LoadString( (TSTRING) dwTCMD[m_vTCORPS.m_vTSQUAD[i]->m_bTCMD]);
			m_pTSQUADSTAT[nIndex]->m_strText = strNAME;

			CD3DFont* pTSDNAMEFNT = m_pTSQUADNAME[nIndex]->Get3DFont();
			CD3DFont* pTSDSTATFNT = m_pTSQUADSTAT[nIndex]->Get3DFont();

			if( pTSDNAMEFNT && pTSDSTATFNT )
			{
				if( m_pHost->m_wCommanderID == m_vTCORPS.m_vTSQUAD[i]->m_wPartyID )
				{
					pTSDNAMEFNT->m_dwColor = TCOLOR_SQUAD_COMMANDER;
					pTSDSTATFNT->m_dwColor = TCOLOR_SQUAD_COMMANDER;
				}
				else
				{
					pTSDNAMEFNT->m_dwColor = TCOLOR_SQUAD_NORMAL;
					pTSDSTATFNT->m_dwColor = TCOLOR_SQUAD_NORMAL;
				}
			}
		}
		else
		{
			strNAME = CTChart::LoadString( TSTR_UNKNOWN);
			m_pTSQUADNAME[nIndex]->m_strText = strNAME;
			m_pTSQUADSTAT[nIndex]->m_strText = strNAME;
		}

		if( m_pTCURSQUAD == m_vTCORPS.m_vTSQUAD[i] )
			m_pTSQUAD->SetCurSelItem(nIndex);

		m_pTSQUADNAME[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);
		m_pTSQUADSTAT[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);
	}

	if( m_pTSQUAD->GetSel() < 0 )
	{
		if(!m_vTCORPS.m_vTSQUAD.empty())
		{
			m_pTCURSQUAD = m_vTCORPS.m_vTSQUAD[0];
			m_pTSQUAD->SetCurSelItem(0);
		}
		else
			m_pTCURSQUAD = NULL;

		OnSelChange();
	}

	ResetUNIT();
}

void CTRSCSDlg::ResetUNIT()
{
	CString strTEXT;

	for( BYTE i=0; i<TRSCSHOTKEY_COUNT; i++)
	{
		m_pTUNITNAME[i]->ShowComponent(FALSE);
		m_pTUNITGAUGE[i]->ShowComponent(FALSE);
	}
	m_pTUNIT->RemoveAll();

	if(!m_pTCURSQUAD)
		return;

	for( i=0; i<INT(m_pTCURSQUAD->m_vTUNIT.size()); i++)
	{
		int nIndex = m_pTUNIT->AddString("");
		m_pTUNIT->SetItemData( nIndex, 0, (DWORD) m_pTCURSQUAD->m_vTUNIT[i]);

		m_pTUNITNAME[i]->m_strText = m_pTCURSQUAD->m_vTUNIT[i]->m_strNAME;
		m_pTUNITNAME[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);
		
		m_pTUNITGAUGE[i]->SetGauge( m_pTCURSQUAD->m_vTUNIT[i]->m_dwHP, m_pTCURSQUAD->m_vTUNIT[i]->m_dwMaxHP, FALSE);
		m_pTUNITGAUGE[i]->ShowComponent(!m_bMinimize && !m_bHideSquad);

		if( m_pTCURUNIT == m_pTCURSQUAD->m_vTUNIT[i] )
			m_pTUNIT->SetCurSelItem(nIndex);
	}

	if( m_pTUNIT->GetSel() < 0 )
	{
		if(!m_pTCURSQUAD->m_vTUNIT.empty())
		{
			m_pTCURUNIT = m_pTCURSQUAD->m_vTUNIT[0];
			m_pTUNIT->SetCurSelItem(0);
		}
		else
			m_pTCURUNIT = NULL;

		OnSelChange();
	}
}

D3DXVECTOR2 CTRSCSDlg::GetEnemyPOS( LPTENEMY pTENEMY)
{
	D3DXVECTOR2 vTRESULT(
		FLOAT(pTENEMY->m_wPosX),
		FLOAT(pTENEMY->m_wPosZ));

	if( pTENEMY->m_wDIR != MOVE_NONE )
	{
		FLOAT fRadian = D3DX_PI + FLOAT(pTENEMY->m_wDIR) * D3DX_PI / 900.0f;
		vTRESULT += D3DXVECTOR2( sinf(fRadian), cosf(fRadian)) * pTENEMY->m_fSPEED * FLOAT(pTENEMY->m_dwTick) / 1000.0f;
	}

	return vTRESULT;
}

D3DXVECTOR2 CTRSCSDlg::GetUnitPOS( LPTUNIT pTUNIT)
{
	D3DXVECTOR2 vTRESULT(
		FLOAT(pTUNIT->m_wPosX),
		FLOAT(pTUNIT->m_wPosZ));

	if( pTUNIT->m_wDIR != MOVE_NONE )
	{
		FLOAT fRadian = D3DX_PI + FLOAT(pTUNIT->m_wDIR) * D3DX_PI / 900.0f;
		vTRESULT += D3DXVECTOR2( sinf(fRadian), cosf(fRadian)) * pTUNIT->m_fSPEED * FLOAT(pTUNIT->m_dwTick) / 1000.0f;
	}

	return vTRESULT;
}

void CTRSCSDlg::CalcTick( DWORD dwTick)
{
	for( BYTE i=0; i<BYTE(m_vTCORPS.m_vTSQUAD.size()); i++)
		for( BYTE j=0; j<BYTE(m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
			if( m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_wDIR != MOVE_NONE )
				m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwTick += dwTick;

	CalcEnemyTick( &m_vTCORPS.m_mapFIXRECALL, dwTick);
	CalcEnemyTick( &m_vTCORPS.m_mapRECALL, dwTick);
	CalcEnemyTick( &m_vTCORPS.m_mapMON, dwTick);
	CalcEnemyTick( &m_vTCORPS.m_mapPC, dwTick);
}

void CTRSCSDlg::CalcEnemyTick( LPMAPTENEMY pTENEMY,
							   DWORD dwTick)
{
	MAPTENEMY::iterator itTENEMY;

	for( itTENEMY = pTENEMY->begin(); itTENEMY != pTENEMY->end(); itTENEMY++)
		if( (*itTENEMY).second->m_wDIR != MOVE_NONE )
			(*itTENEMY).second->m_dwTick += dwTick;
}

void CTRSCSDlg::DeleteReporter( DWORD dwReporterID)
{
	DeleteReporter( &m_vTCORPS.m_mapFIXRECALL, dwReporterID);
	DeleteReporter( &m_vTCORPS.m_mapRECALL, dwReporterID);
	DeleteReporter( &m_vTCORPS.m_mapMON, dwReporterID);
	DeleteReporter( &m_vTCORPS.m_mapPC, dwReporterID);
}

void CTRSCSDlg::DeleteReporter( LPMAPTENEMY pTENEMY,
							    DWORD dwReporterID)
{
	MAPTENEMY::iterator itTENEMY = pTENEMY->begin();

	while(itTENEMY != pTENEMY->end())
	{
		MAPDWORD::iterator finder = (*itTENEMY).second->m_mapTREPORTER.find(dwReporterID);
		MAPTENEMY::iterator itNEXT = itTENEMY;
		itNEXT++;

		if( finder != (*itTENEMY).second->m_mapTREPORTER.end() )
			(*itTENEMY).second->m_mapTREPORTER.erase(finder);

		if((*itTENEMY).second->m_mapTREPORTER.empty())
		{
			delete (*itTENEMY).second;
			pTENEMY->erase(itTENEMY);
		}

		itTENEMY = itNEXT;
	}
}

LPTSQUAD CTRSCSDlg::FindTSQUAD( WORD wSquadID)
{
	for( BYTE i=0; i<BYTE(m_vTCORPS.m_vTSQUAD.size()); i++)
		if( m_vTCORPS.m_vTSQUAD[i]->m_wPartyID == wSquadID )
			return m_vTCORPS.m_vTSQUAD[i];

	return NULL;
}

LPTENEMY CTRSCSDlg::FindTENEMY( DWORD dwID,
							    BYTE bType)
{
	LPMAPTENEMY pTENEMY = NULL;

	switch(bType)
	{
	case OT_RECALL	: pTENEMY = &m_vTCORPS.m_mapRECALL; break;
	case OT_SELF	: pTENEMY = &m_vTCORPS.m_mapFIXRECALL; break;
	case OT_MON		: pTENEMY = &m_vTCORPS.m_mapMON; break;
	case OT_PC		: pTENEMY = &m_vTCORPS.m_mapPC; break;
	}

	if(pTENEMY)
	{
		MAPTENEMY::iterator finder = pTENEMY->find(dwID);

		if( finder != pTENEMY->end() )
			return (*finder).second;
	}

	return NULL;
}

LPTUNIT CTRSCSDlg::FindTUNIT( LPTSQUAD pTSQUAD,
							  DWORD dwCharID)
{
	if(!pTSQUAD)
		return NULL;

	for( BYTE i=0; i<BYTE(pTSQUAD->m_vTUNIT.size()); i++)
		if( pTSQUAD->m_vTUNIT[i]->m_dwCharID == dwCharID )
			return pTSQUAD->m_vTUNIT[i];

	return NULL;
}

void CTRSCSDlg::RenderTRSCS()
{
	int nRng, nPos;
	m_pTZOOMSCROLL->GetScrollPos(nRng, nPos);
	m_fTSCALE = TMINIMAP_INIT_SCALE / powf(TMINIMAP_SCALE_FACTOR, (FLOAT)nPos);

	CRect rect(m_vTAREA);

	// Arab Ver.
	CRect rc = m_rc;
	ComponentToScreen(&rc);

	rect.OffsetRect(
		rc.left,
		rc.top);

	TNLVERTEX vRECT[4] = {
		{ FLOAT(rect.left), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f},
		{ FLOAT(rect.right), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f},
		{ FLOAT(rect.left), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f},
		{ FLOAT(rect.right), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f}};
	FLOAT fMIP = 0.0f;

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->m_pDevice->SetTexture( 0, m_pTBACK);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	m_pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vRECT, sizeof(TNLVERTEX));
}

void CTRSCSDlg::OnSelChange()
{
	if( !m_bVisible || !m_pHost || !m_pTCURSQUAD )
		return;
	
	LPTUNIT pTUNIT = NULL;
	if( m_pTCURSQUAD->m_wPartyID == m_pHost->m_wPartyID )
	{
		for( BYTE i=0; i<BYTE(m_pTCURSQUAD->m_vTUNIT.size()); i++)
		{
			if( m_pTCURSQUAD->m_vTUNIT[i] == m_pTCURUNIT )
			{
				pTUNIT = m_pTCURUNIT;
				break;
			}
		}
			
		if( !pTUNIT )
			m_pTCURUNIT = NULL;
	}
	else if( m_pHost->m_dwChiefID == m_pHost->m_dwID )
		pTUNIT = FindTUNIT( m_pTCURSQUAD, m_pTCURSQUAD->m_dwChiefID);
	
	
	if(pTUNIT)
	{
		m_vTCENTER.x = FLOAT(pTUNIT->m_wPosX);
		m_vTCENTER.y = FLOAT(pTUNIT->m_wPosZ);

		m_dwBspID = pTUNIT->m_dwBspID;
		m_wMapID = pTUNIT->m_wMapID;
	}
}

void CTRSCSDlg::CheckCURSEL( BYTE bRESET)
{
	if( !m_bVisible || !m_pHost )
		return;

	LPTSQUAD pTSQUAD = NULL;
	LPTUNIT pTUNIT = NULL;

	int nIndex = m_pTSQUAD->GetSel();
	if( nIndex >= 0 )
		pTSQUAD = (LPTSQUAD) m_pTSQUAD->GetItemData( nIndex, 0);

	nIndex = m_pTUNIT->GetSel();
	if( nIndex >= 0 )
		pTUNIT = (LPTUNIT) m_pTUNIT->GetItemData( nIndex, 0);

	if( bRESET || pTSQUAD != m_pTCURSQUAD || pTUNIT != m_pTCURUNIT )
	{
		m_pTCURSQUAD = pTSQUAD;
		m_pTCURUNIT = pTUNIT;

		OnSelChange();
	}
}

BYTE CTRSCSDlg::OnBeginDrag( LPTDRAG pDRAG,
							 CPoint point)
{
	int nIndex = m_pTUNIT->GetSel();

	if( m_pTUNIT->HitItemArea(point) && nIndex >= 0 )
	{
		LPTUNIT pTUNIT = (LPTUNIT) m_pTUNIT->GetItemData( nIndex, 0);

		if( pDRAG )
		{
			pDRAG->m_bSlotID = (BYTE) nIndex;
			pDRAG->m_dwParam = -1;

			if(m_pTDRAG)
			{
				CPoint pt = point;
				CSize size;

				pDRAG->m_pIMAGE = new TImageList(
					NULL,
					m_pTDRAG->m_pDESC);

				pDRAG->m_pIMAGE->SetCurImage(THAIR_COUNT * ((pTUNIT->m_bRaceID * TSEX_COUNT + pTUNIT->m_bSexID) * TFACE_COUNT + pTUNIT->m_bFaceID) + pTUNIT->m_bHairID);
				pDRAG->m_pIMAGE->GetComponentSize(&size);
				pt.x -= size.cx / 2;
				pt.y -= size.cy / 2;

				pDRAG->m_pIMAGE->ShowComponent(FALSE);
				pDRAG->m_pIMAGE->MoveComponent(pt);
			}
		}

		return TRUE;
	}

	return FALSE;
}

TDROPINFO CTRSCSDlg::OnDrop( CPoint point)
{
	TDROPINFO vResult;

	m_bDropping = FALSE;

	if(m_bDropTarget)
	{
		CSize size = m_pTSQUAD->GetItemSize(0);
		CRect rect;

		m_pTSQUAD->GetComponentRect(&rect);
		m_pTSQUAD->ComponentToScreen(&rect);

		rect.bottom = rect.top + size.cy;
		rect.right = rect.left + size.cx;

		for( BYTE i=0; i<BYTE(m_vTCORPS.m_vTSQUAD.size()); i++)
		{
			if(rect.PtInRect(point))
			{
				m_bDropping = TRUE;

				vResult.m_bDrop = TRUE;
				vResult.m_bSlotID = i;

				return vResult;
			}

			rect.OffsetRect( 0, size.cy);
		}
	}

	return vResult;
}

BOOL CTRSCSDlg::CanWithItemUI()
{
	return TRUE;
}

void CTRSCSDlg::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	CTClientUIBase::OnKeyDown( nChar, nRepCnt, nFlags);
	CheckCURSEL(FALSE);
}

void CTRSCSDlg::OnLButtonDown( UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown( nFlags, pt);

	if(IsVisible())
	{
		BYTE bReset = FALSE;

		if(m_pTSQUAD->HitTest(pt))
			bReset = TRUE;

		if(m_pTUNIT->HitTest(pt))
			bReset = TRUE;
		CheckCURSEL(bReset);

		if(bReset && !m_bMinimize && !m_bHideSquad)
			ResetCORPS();
	}
}

void CTRSCSDlg::OnRButtonDown( UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnRButtonDown( nFlags, pt);

	if(IsVisible())
	{
		CRect rect(m_vTAREA);
		BYTE bReset = FALSE;

		rect.OffsetRect(
			m_rc.left,
			m_rc.top);
		ComponentToScreen(&rect);

		if(rect.PtInRect(pt))
		{
			CPoint point = rect.CenterPoint();

			//SetCapture(m_pCommandHandler->GetSafeHwnd());
			ShowCursor(FALSE);
			MoveCursor(point);
			//ClipCursor(&rect);

			m_bMOVE = TRUE;
		}

		if(m_pTSQUAD->HitTest(pt))
			bReset = TRUE;

		if(m_pTUNIT->HitTest(pt))
			bReset = TRUE;
		CheckCURSEL(bReset);

		if(bReset && !m_bMinimize && !m_bHideSquad)
			ResetCORPS();
	}
}

void CTRSCSDlg::OnMouseMove( UINT nFlags, CPoint pt)
{
	if( IsVisible() && m_bMOVE )
	{
		FLOAT fSCALE = FLOAT(m_pTMAP->m_pMAP->m_nUnitLength * m_pTMAP->m_pMAP->m_nTileLength) / m_fTSCALE;
		CRect rect(m_vTAREA);

		rect.OffsetRect(m_rc.left, m_rc.top);
		ComponentToScreen(&rect);

		CPoint point = rect.CenterPoint();
		MoveCursor(point);
		point -= pt;

		point.x *= TMINIMAPTEX_SIZE;
		point.y *= TMINIMAPTEX_SIZE;

		point.y /= m_vTAREA.Height();
		point.x /= m_vTAREA.Width();

		m_vTCENTER.x += fSCALE * FLOAT(point.x) / FLOAT(TMINIMAPTEX_SIZE);
		m_vTCENTER.y -= fSCALE * FLOAT(point.y) / FLOAT(TMINIMAPTEX_SIZE);
	}

	CTClientUIBase::OnMouseMove( nFlags, pt);
}

BOOL CTRSCSDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( m_bMinimize )
		return FALSE;

	CRect rect(m_vTAREA);
	
	rect.OffsetRect(
		m_rc.left,
		m_rc.top);
	ComponentToScreen(&rect);
	if(!rect.PtInRect(pt))
		return FALSE;

	int nRange, nPos;
	m_pTZOOMSCROLL->GetScrollPos(nRange, nPos);

	nPos += zDelta>0? -1: 1;
	nPos = min(max(nPos, 0), nRange);

	m_pTZOOMSCROLL->SetScrollPos(nRange, nPos);

	return TRUE;
}

void CTRSCSDlg::OnRButtonUp( UINT nFlags, CPoint pt)
{
	if( IsVisible() && m_bMOVE )
	{
		CPoint point = m_vTAREA.CenterPoint() + m_rc.TopLeft();

		ComponentToScreen(&point);
		MoveCursor(point);
		ReleaseCapture();
		ClipCursor(NULL);
		ShowCursor(TRUE);

		m_bMOVE = FALSE;
	}

	CTClientUIBase::OnRButtonUp( nFlags, pt);
}

void CTRSCSDlg::MoveCursor( CPoint point)
{
	m_pCommandHandler->ClientToScreen(&point);
	SetCursorPos( point.x, point.y);
}

void CTRSCSDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if(m_bVisible)
		ResetVisible();
}

HRESULT CTRSCSDlg::Render( DWORD dwTickCount)
{
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	if( !pMainChar->m_bFlash )
	{
		if( m_bVisible && m_pTMAP && !m_bMinimize )
		{
			m_pTBACKAREA->Render(dwTickCount);

			m_pTMAP->ResetTRSCS( m_pTBACK, m_pTRSCS, CTMinimapDlg::m_pTMINIMAP, this);
			RenderTRSCS();
		}

		m_pTBACKAREA->ShowComponent(FALSE);
		HRESULT r = CTClientUIBase::Render(dwTickCount);
		m_pTBACKAREA->ShowComponent(m_bVisible);

		return r;
	}
	
	return S_OK;
}

void CTRSCSDlg::SwitchFocus(TComponent* pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	if(m_pFocus)
	{
		m_pFocus->SetFocus(FALSE);
		m_pFocus->OnLoseFocus(pCandidate);
	}

	if(pCandidate)
	{
		pCandidate->SetFocus(TRUE);
		pCandidate->OnSetFocus(m_pFocus);
	}

	m_pFocus = pCandidate;
}

void CTRSCSDlg::AddEnemy(
	DWORD dwReporterID,
	DWORD dwEnemyID,
	BYTE bEnemyType,
	FLOAT fSpeed,
	WORD wMapID,
	WORD wPosX,
	WORD wPosZ,
	WORD wDIR )
{
	MAPTENEMY* pTENEMY = NULL;

	switch(bEnemyType)
	{
	case OT_RECALL	: pTENEMY = &m_vTCORPS.m_mapRECALL; break;
	case OT_SELF	: pTENEMY = &m_vTCORPS.m_mapFIXRECALL; break;
	case OT_MON		: pTENEMY = &m_vTCORPS.m_mapMON; break;
	case OT_PC		: pTENEMY = &m_vTCORPS.m_mapPC; break;
	}

	if(pTENEMY)
	{
		MAPTENEMY::iterator finder = pTENEMY->find(dwEnemyID);
		LPTENEMY pENEMY = NULL;

		if( finder == pTENEMY->end() )
		{
			pENEMY = new TENEMY();

			pENEMY->m_bType = bEnemyType;
			pENEMY->m_dwID = dwEnemyID;
			pENEMY->m_fSPEED = fSpeed;
			pENEMY->m_wMapID = wMapID;
			pENEMY->m_wPosX = wPosX;
			pENEMY->m_wPosZ = wPosZ;
			pENEMY->m_wDIR = wDIR;

			pTENEMY->insert( MAPTENEMY::value_type( dwEnemyID, pENEMY));
			pENEMY->m_dwTick = 0;
		}
		else
			pENEMY = (*finder).second;

		if( pENEMY->m_mapTREPORTER.find(dwReporterID) == pENEMY->m_mapTREPORTER.end() )
			pENEMY->m_mapTREPORTER.insert( MAPDWORD::value_type( dwReporterID, dwReporterID));
	}
}