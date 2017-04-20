// EXTMAPDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EXTMAPFrame.h"
#include "EXTMAPView.h"
#include "EXTMAPDoc.h"
#include "MainFrm.h"
#include ".\extmapdoc.h"


// CEXTMAPDoc

IMPLEMENT_DYNCREATE(CEXTMAPDoc, CMAPDoc)

CEXTMAPDoc::CEXTMAPDoc()
{
	m_bDocType = ITEM_EXTMAP;
	m_fBRUSH = 0.0f;
	m_fFLAT = 0.0f;
	m_fADDFLAT = 0.0f;
	m_bResetVB = FALSE;

	m_pSPRAY = NULL;
	m_pItem = NULL;

	for( int i=0; i<36; i++)
	{
		m_mapHOST[i].clear();
		m_pREGION[i] = NULL;
		m_pSHADE[i] = NULL;
		m_pSKIN[i] = NULL;
	}
	m_mapUNIT.clear();
}

BOOL CEXTMAPDoc::OnNewDocument()
{
	if (!CMAPDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CEXTMAPDoc::~CEXTMAPDoc()
{
	if(m_pItem)
	{
		MAPMAPOBJ::iterator itOBJ;
		MAPMAPSFX::iterator itSFX;
		MAPMAPSND::iterator itSND;

		for( itOBJ = m_pItem->m_pMAP->m_mapOBJ.begin(); itOBJ != m_pItem->m_pMAP->m_mapOBJ.end(); itOBJ++)
			delete (*itOBJ).second;

		for( itSFX = m_pItem->m_pMAP->m_mapSFX.begin(); itSFX != m_pItem->m_pMAP->m_mapSFX.end(); itSFX++)
			delete (*itSFX).second;

		for( itSND = m_pItem->m_pMAP->m_mapSND.begin(); itSND != m_pItem->m_pMAP->m_mapSND.end(); itSND++)
			delete (*itSND).second;

		for( int i=0; i<9; i++)
		{
			m_pItem->m_HugeMAP.m_bUPDATE[i] = FALSE;
			m_pItem->m_HugeMAP.m_bLOAD[i] = FALSE;
			m_pItem->m_HugeMAP.m_bLOCK[i] = FALSE;
			m_pItem->m_HugeMAP.m_pUNIT[i] = NULL;

			m_pItem->m_HugeMAP.m_bINDEX[i] = 0xFF;
			m_pItem->m_HugeMAP.m_bUNIT[i] = 0xFF;
		}

		m_pItem->m_pMAP->m_mapOBJ.clear();
		m_pItem->m_pMAP->m_mapSFX.clear();
		m_pItem->m_pMAP->m_mapSND.clear();
		m_pItem->m_mapOBJ.clear();
		m_pItem->m_mapSFX.clear();

		m_pItem->SetMainDocument(NULL);
		m_pItem = NULL;
	}

	for( int i=0; i<36; i++)
	{
		if(m_pREGION[i])
		{
			m_pREGION[i]->Release();
			m_pREGION[i] = NULL;
		}

		if(m_pSHADE[i])
		{
			m_pSHADE[i]->Release();
			m_pSHADE[i] = NULL;
		}

		if(m_pSKIN[i])
		{
			m_pSKIN[i]->Release();
			m_pSKIN[i] = NULL;
		}
		m_mapHOST[i].clear();
	}

	if(m_pSPRAY)
	{
		m_pSPRAY->Release();
		m_pSPRAY = NULL;
	}

	m_mapUNIT.clear();
	m_vOBJPrePos.clear();
}


BEGIN_MESSAGE_MAP(CEXTMAPDoc, CMAPDoc)
END_MESSAGE_MAP()


// CEXTMAPDoc 진단입니다.

#ifdef _DEBUG
void CEXTMAPDoc::AssertValid() const
{
	CMAPDoc::AssertValid();
}

void CEXTMAPDoc::Dump(CDumpContext& dc) const
{
	CMAPDoc::Dump(dc);
}
#endif //_DEBUG


// CEXTMAPDoc serialization입니다.

void CEXTMAPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CEXTMAPDoc 명령입니다.

CEXTMAPBar *CEXTMAPDoc::GetEXTMAPBar()
{
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndEXTMAPBar : NULL;
}

void CEXTMAPDoc::InitData( LPVOID pData)
{
	CTachyonDoc::InitData(pData);
	if(!pData)
		return;

	m_pItem = (CEXTMAPItem *) pData;
	m_pItem->SetMainDocument(this);	
	m_pMAP = m_pItem;

	Update();
}

void CEXTMAPDoc::Update()
{
	if(m_pItem)
	{
		CEXTMAPBar *pControl = GetEXTMAPBar();

		SetTitle(m_pItem->m_strName);
		m_pItem->Regulate();

		m_pItem->m_HugeMAP.m_nUnitX = -2;
		m_pItem->m_HugeMAP.m_nUnitZ = -2;
		m_pItem->m_HugeMAP.m_nCellX = -2;
		m_pItem->m_HugeMAP.m_nCellZ = -2;

		if(m_pSPRAY)
		{
			m_pSPRAY->Release();
			m_pSPRAY = NULL;
		}

		if(pControl)
		{
			pControl->SetItem(m_pItem);

			if( m_pItem->m_HugeMAP.m_nTextureSize != 0 )
			{
				D3DXCreateTexture(
					CMainFrame::m_3DDevice.m_pDevice,
					m_pItem->m_HugeMAP.m_nTextureSize,
					m_pItem->m_HugeMAP.m_nTextureSize, 1,
					D3DUSAGE_RENDERTARGET,
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					&m_pSPRAY);

				ClearSPRAY();
			}
		}

		RefreshView();
		CMAPDoc::Update();
	}
}

void CEXTMAPDoc::ClearSPRAY()
{
	LPDIRECT3DSURFACE9 pSPRAY = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	m_pSPRAY->GetSurfaceLevel( 0, &pSPRAY);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pSPRAY);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(
		0, 0, 0xFF, 0),
		1.0f, 0);
	pSPRAY->Release();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);

	pBACK->Release();
	pZBUF->Release();
}

void CEXTMAPDoc::SetHeight( FLOAT fHeight)
{
	CEXTMAPBar *pBar = GetEXTMAPBar();

	m_fFLAT = fHeight;
	pBar->m_strFlatH.Format( "%.2f", m_fFLAT);
	pBar->UpdateData(FALSE);
}

BOOL CEXTMAPDoc::GetWorldCenter( LPD3DXVECTOR3 pResult)
{
	if(!CMAPDoc::GetWorldCenter(pResult))
	{
		pResult->x = ((FLOAT) m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength) / 2.0f;
		pResult->z = ((FLOAT) m_pItem->m_HugeMAP.m_nUnitCountZ * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength) / 2.0f;
		pResult->y = GetHeight( pResult->x, 0.0f, pResult->z);
	}

	return TRUE;
}

FLOAT CEXTMAPDoc::GetHeight( FLOAT fPosX,
							 FLOAT fPosY,
							 FLOAT fPosZ)
{
	CTachyonEXTMAP extmap;

	fPosX /= FLOAT(m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(m_pItem->m_HugeMAP.m_nTileLength);

	int nUnitX = INT(fPosX / m_pItem->m_HugeMAP.m_nUnitLength);
	int nUnitZ = INT(fPosZ / m_pItem->m_HugeMAP.m_nUnitLength);

	fPosX = INT(fPosX) % m_pItem->m_HugeMAP.m_nUnitLength + fPosX - INT(fPosX);
	fPosZ = INT(fPosZ) % m_pItem->m_HugeMAP.m_nUnitLength + fPosZ - INT(fPosZ);

	extmap.m_pDATA = new FLOAT[(m_pItem->m_HugeMAP.m_nUnitLength + 1) * (m_pItem->m_HugeMAP.m_nUnitLength + 1)];
	extmap.m_nTileCount = m_pItem->m_HugeMAP.m_nUnitLength;
	m_pItem->LoadUNIT( NULL, NULL, NULL, NULL, NULL, NULL, extmap.m_pDATA, nUnitZ * m_pItem->m_HugeMAP.m_nUnitCountX + nUnitX);

	return extmap.GetHeight( fPosX, 0.0f, fPosZ);
}

void CEXTMAPDoc::JumpView( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetFrameWnd();
	CEXTMAPView *pView = (CEXTMAPView *) pFrame->GetActiveView();

	if(pView)
		pView->JumpView(D3DXVECTOR3( fPosX, GetHeight( fPosX, 0.0f, fPosZ), fPosZ));
}

void CEXTMAPDoc::SelectUNIT( CWnd *pWnd,
							 int nPosX,
							 int nPosZ)
{
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetFrameWnd();
	CEXTMAPBar *pBar = GetEXTMAPBar();

	if( pWnd == pFrame->GetActiveView() )
	{
		int nIndex = nPosZ * m_pItem->m_HugeMAP.m_nUnitCountX + nPosX;
		CRect rect(
			0, 0,
			m_pItem->m_HugeMAP.m_nUnitCountX,
			m_pItem->m_HugeMAP.m_nUnitCountZ);

		if(!rect.PtInRect(CPoint( nPosX, nPosZ)))
			nIndex = -1;

		if( nIndex != pBar->GetUNIT() )
		{
			pBar->m_cUNIT.SetCurSel( nIndex != -1 ? nPosX * m_pItem->m_HugeMAP.m_nUnitCountZ + nPosZ : -1);
			pBar->OnSelectUNIT(nIndex);
		}
	}
}

BYTE CEXTMAPDoc::LockUNIT( CWnd *pHost, DWORD dwUnitID)
{
	MAPDWORD::iterator finder = m_mapUNIT.find(dwUnitID);
	BYTE bID = 0;

	if( finder != m_mapUNIT.end() )
	{
		bID = (BYTE) (*finder).second;

		MAPVIEW::iterator itHOST = m_mapHOST[bID].find(pHost);
		if( itHOST == m_mapHOST[bID].end() )
			m_mapHOST[bID].insert( MAPVIEW::value_type( pHost, pHost));
	}
	else
	{
		for( BYTE i=0; i<36; i++)
			if(m_mapHOST[i].empty())
			{
				m_mapHOST[i].insert( MAPVIEW::value_type( pHost, pHost));

				bID = i;
				break;
			}

		if(m_pREGION[bID])
		{
			m_pREGION[bID]->Release();
			m_pREGION[bID] = NULL;
		}

		if(m_pSHADE[bID])
		{
			m_pSHADE[bID]->Release();
			m_pSHADE[bID] = NULL;
		}

		if(m_pSKIN[bID])
		{
			m_pSKIN[bID]->Release();
			m_pSKIN[bID] = NULL;
		}

		LoadUNIT(
			&m_UNIT[bID],
			&m_pREGION[bID],
			&m_pSHADE[bID],
			&m_pSKIN[bID],
			dwUnitID);
		m_mapUNIT.insert( MAPDWORD::value_type( dwUnitID, bID));
	}

	return bID;
}

void CEXTMAPDoc::ReleaseUNIT( CWnd *pHost, DWORD dwUnitID)
{
	MAPDWORD::iterator finder = m_mapUNIT.find(dwUnitID);

	if( finder != m_mapUNIT.end() )
	{
		DWORD dwID = (*finder).second;
		MAPVIEW::iterator itHOST = m_mapHOST[dwID].find(pHost);

		if( itHOST != m_mapHOST[dwID].end() )
			m_mapHOST[dwID].erase(itHOST);

		if(m_mapHOST[dwID].empty())
		{
			m_mapUNIT.erase(finder);

			m_pItem->UpdateUNIT(
				m_pSHADE[dwID],
				m_pSKIN[dwID],
				m_UNIT[dwID].m_pTILETEX,
				&m_UNIT[dwID].m_vSHADOWTEX,
				m_UNIT[dwID].m_pREGION,
				m_UNIT[dwID].m_pDETAIL,
				m_UNIT[dwID].m_pDATA,
				dwUnitID);
			RegulateOBJ(dwUnitID);
		}
	}
}

void CEXTMAPDoc::RegulateOBJ( DWORD dwUnitID)
{
	MAPMAPOBJ::iterator itOBJ = m_pItem->m_pMAP->m_mapOBJ.begin();
	MAPMAPSFX::iterator itSFX = m_pItem->m_pMAP->m_mapSFX.begin();
	MAPMAPSND::iterator itSND = m_pItem->m_pMAP->m_mapSND.begin();

	MAPMAPOBJ mapOBJ;
	MAPMAPSFX mapSFX;
	MAPMAPSND mapSND;

	MAPDWORD mapOBJID;
	MAPDWORD mapSFXID;

	CRect rect(
		0, 0,
		m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength,
		m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength);

	rect.OffsetRect(
		(dwUnitID % m_pItem->m_HugeMAP.m_nUnitCountX) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength,
		(dwUnitID / m_pItem->m_HugeMAP.m_nUnitCountX) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength);

	mapOBJ.clear();
	mapSFX.clear();
	mapSND.clear();

	mapOBJID.clear();
	mapSFXID.clear();

	while(itOBJ != m_pItem->m_pMAP->m_mapOBJ.end())
	{
		MAPMAPOBJ::iterator itNEXT = itOBJ;
		DWORD dwID = (*itOBJ).first;
		CPoint point(
			(INT) (*itOBJ).second->m_vPOS.x,
			(INT) (*itOBJ).second->m_vPOS.z);
		itNEXT++;

		if(rect.PtInRect(point))
		{
			MAPDWORD::iterator itID = m_pItem->m_mapOBJ.find(dwID);
			MAPOBJECT::iterator itTOBJ = m_mapOBJ.find(dwID);

			if( itTOBJ != m_mapOBJ.end() )
			{
				delete (*itTOBJ).second;
				m_mapOBJ.erase(itTOBJ);
			}

			if( itID != m_pItem->m_mapOBJ.end() )
			{
				mapOBJID.insert( MAPDWORD::value_type( dwID, (*itID).second));
				m_pItem->m_mapOBJ.erase(itID);
			}

			mapOBJ.insert( MAPMAPOBJ::value_type( dwID, (*itOBJ).second));
			m_pItem->m_pMAP->m_mapOBJ.erase(itOBJ);
		}
		itOBJ = itNEXT;
	}

	while(itSFX != m_pItem->m_pMAP->m_mapSFX.end())
	{
		MAPMAPSFX::iterator itNEXT = itSFX;
		DWORD dwID = (*itSFX).first;
		CPoint point(
			(INT) (*itSFX).second->m_vPOS.x,
			(INT) (*itSFX).second->m_vPOS.z);
		itNEXT++;

		if(rect.PtInRect(point))
		{
			MAPDWORD::iterator itID = m_pItem->m_mapSFX.find(dwID);

			if( itID != m_pItem->m_mapSFX.end() )
			{
				mapSFXID.insert( MAPDWORD::value_type( dwID, (*itID).second));
				m_pItem->m_mapSFX.erase(itID);
			}

			mapSFX.insert( MAPMAPSFX::value_type( dwID, (*itSFX).second));
			m_pItem->m_pMAP->m_mapSFX.erase(itSFX);
		}
		itSFX = itNEXT;
	}

	while(itSND != m_pItem->m_pMAP->m_mapSND.end())
	{
		MAPMAPSND::iterator itNEXT = itSND;
		DWORD dwID = (*itSND).first;
		CPoint point(
			(INT) (*itSND).second->m_vPOS.x,
			(INT) (*itSND).second->m_vPOS.z);
		itNEXT++;

		if(rect.PtInRect(point))
		{
			mapSND.insert( MAPMAPSND::value_type( dwID, (*itSND).second));
			m_pItem->m_pMAP->m_mapSND.erase(itSND);
		}
		itSND = itNEXT;
	}

	m_pItem->UpdateOBJ(
		&mapOBJ,
		&mapSFX,
		&mapSND,
		&mapOBJID,
		&mapSFXID,
		dwUnitID);

	for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
		delete (*itSFX).second;

	for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
		delete (*itSND).second;

	mapOBJ.clear();
	mapSFX.clear();
	mapSND.clear();

	mapOBJID.clear();
	mapSFXID.clear();
}

void CEXTMAPDoc::LoadUNIT( CTachyonEXTMAP *pUNIT,
						   LPDIRECT3DSURFACE9 *pREGION,
						   LPDIRECT3DSURFACE9 *pSHADE,
						   LPDIRECT3DSURFACE9 *pSKIN,
						   DWORD dwUnitID)
{
	int nPosX = dwUnitID % m_pItem->m_HugeMAP.m_nUnitCountX;
	int nPosZ = dwUnitID / m_pItem->m_HugeMAP.m_nUnitCountX;

	if(!pUNIT->m_pDATA)
	{
		pUNIT->m_pDATA = new FLOAT[(m_pItem->m_HugeMAP.m_nUnitLength + 1) * (m_pItem->m_HugeMAP.m_nUnitLength + 1)];
		memset( pUNIT->m_pDATA, 0x00, sizeof(FLOAT) * (m_pItem->m_HugeMAP.m_nUnitLength + 1) * (m_pItem->m_HugeMAP.m_nUnitLength + 1));
	}

	if(!pUNIT->m_pREGION)
	{
		pUNIT->m_pREGION = new BYTE[m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength];
		memset( pUNIT->m_pREGION, 0x00, sizeof(BYTE) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength);
	}

	if(!pUNIT->m_pDETAIL)
	{
		pUNIT->m_pDETAIL = new BYTE[m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength];
		memset( pUNIT->m_pDETAIL, -1, sizeof(BYTE) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength);
	}

	if(!pUNIT->m_pGEOTEX)
	{
		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			m_pItem->m_HugeMAP.m_nTextureSize,
			m_pItem->m_HugeMAP.m_nTextureSize,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			&pUNIT->m_pGEOTEX,
			NULL);
	}

	if(pUNIT->m_pTILETEX)
	{
		pUNIT->m_pTILETEX->Release();
		pUNIT->m_pTILETEX = NULL;
	}

	while(!pUNIT->m_vSHADOWTEX.empty())
	{
		pUNIT->m_vSHADOWTEX.back()->Release();
		pUNIT->m_vSHADOWTEX.pop_back();
	}

	pUNIT->m_nTileCount = m_pItem->m_HugeMAP.m_nUnitLength;
	m_pItem->LoadUNIT(
		pSHADE,
		pSKIN,
		&pUNIT->m_pTILETEX,
		&pUNIT->m_vSHADOWTEX,
		pUNIT->m_pREGION,
		pUNIT->m_pDETAIL,
		pUNIT->m_pDATA,
		dwUnitID);

	LPDWORD pCLR = NULL;
	LPBYTE pDATA = NULL;
	D3DLOCKED_RECT lr;

	CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
		m_pItem->m_HugeMAP.m_nTextureSize,
		m_pItem->m_HugeMAP.m_nTextureSize,
		D3DFMT_A8R8G8B8,
		D3DPOOL_SCRATCH,
		pREGION,
		NULL);

	(*pREGION)->LockRect( &lr, NULL, 0);
	pDATA = (LPBYTE) lr.pBits;

	for( int i=0; i<m_pItem->m_HugeMAP.m_nTextureSize; i++)
	{
		int nLocZ = i * m_pItem->m_HugeMAP.m_nUnitLength / m_pItem->m_HugeMAP.m_nTextureSize;

		pCLR = (LPDWORD) pDATA;
		pDATA += lr.Pitch;

		for( int j=0; j<m_pItem->m_HugeMAP.m_nTextureSize; j++)
		{
			int nLocX = j * m_pItem->m_HugeMAP.m_nUnitLength / m_pItem->m_HugeMAP.m_nTextureSize;

			BYTE bREGION = pUNIT->m_pREGION[nLocZ * m_pItem->m_HugeMAP.m_nUnitLength + nLocX];
			MAPCOLOR::iterator finder = m_pItem->m_mapCOLOR.find(bREGION);

			if(finder != m_pItem->m_mapCOLOR.end())
			{
				pCLR[j] = VALUEARGB(
					0xFF,
					GetRValue((*finder).second),
					GetGValue((*finder).second),
					GetBValue((*finder).second));
			}
			else
				pCLR[j] = 0xFFFFFFFF;
		}
	}
	(*pREGION)->UnlockRect();

	BuildTEX( pUNIT, *pREGION, *pSHADE, *pSKIN);
	BuildOBJ(dwUnitID);
}

void CEXTMAPDoc::BuildOBJ( DWORD dwUnitID)
{
	CMAPOBJBar *pControl = GetMAPOBJBar();

	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	MAPMAPOBJ mapOBJ;
	MAPMAPSFX mapSFX;
	MAPMAPSND mapSND;

	MAPDWORD mapOBJID;
	MAPDWORD mapSFXID;

	mapOBJ.clear();
	mapSFX.clear();
	mapSND.clear();

	mapOBJID.clear();
	mapSFXID.clear();

	m_pItem->LoadOBJ(
		&mapOBJ,
		&mapSFX,
		&mapSND,
		&mapOBJID,
		&mapSFXID,
		dwUnitID);

	for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
	{
		LPMAPOBJ pOBJ = (*itOBJ).second;
		DWORD dwID = (*itOBJ).first;

		if( pOBJ && pOBJ->m_pOBJ && m_pItem->m_pMAP->m_mapOBJ.find(dwID) == m_pItem->m_pMAP->m_mapOBJ.end() )
		{
			MAPDWORD::iterator itID = mapOBJID.find(dwID);
			DWORD dwItemID = (*itID).second;
			CTachyonObject *pTOBJ = m_pItem->CreateTOBJ(
				pOBJ,
				dwItemID);

			if(pTOBJ)
			{
				m_pItem->m_mapOBJ.insert( MAPDWORD::value_type(
					dwID,
					dwItemID));
				m_pItem->m_pMAP->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pOBJ));

				m_mapOBJ.insert( MAPOBJECT::value_type( dwID, pTOBJ));
				pTOBJ->m_pHost = pControl;
			}
		}
		else
			delete pOBJ;
	}

	for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
	{
		LPMAPSFX pSFX = (*itSFX).second;
		DWORD dwID = (*itSFX).first;

		if( pSFX && pSFX->m_pSFX && m_pItem->m_pMAP->m_mapSFX.find(dwID) == m_pItem->m_pMAP->m_mapSFX.end() )
		{
			MAPDWORD::iterator itID = mapSFXID.find(dwID);
			DWORD dwItemID = (*itID).second;

			m_pItem->m_mapSFX.insert( MAPDWORD::value_type(
				dwID,
				dwItemID));
			m_pItem->m_pMAP->m_mapSFX.insert( MAPMAPSFX::value_type( dwID, pSFX));
		}
		else
			delete pSFX;
	}

	for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
	{
		LPMAPSND pSND = (*itSND).second;
		DWORD dwID = (*itSND).first;

		if( pSND && m_pItem->m_pMAP->m_mapSND.find(dwID) == m_pItem->m_pMAP->m_mapSND.end() )
			m_pItem->m_pMAP->m_mapSND.insert( MAPMAPSND::value_type( dwID, pSND));
		else
			delete pSND;
	}

	mapOBJ.clear();
	mapSFX.clear();
	mapSND.clear();

	mapOBJID.clear();
	mapSFXID.clear();
}

void CEXTMAPDoc::BuildTEX( CTachyonEXTMAP *pUNIT,
						   LPDIRECT3DSURFACE9 pREGION,
						   LPDIRECT3DSURFACE9 pSHADE,
						   LPDIRECT3DSURFACE9 pSKIN)
{
	D3DXMATRIX vWORLD(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, 0.5f, 1.0f);

	LPDIRECT3DTEXTURE9 pTSHADETEX = NULL;
	LPDIRECT3DTEXTURE9 pTSKINTEX = NULL;

	LPDIRECT3DSURFACE9 pTRENDER = NULL;
	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	D3DSURFACE_DESC vDESC;
	CD3DCamera vCAMERA;
	pUNIT->m_pGEOTEX->GetDesc(&vDESC);

	D3DVIEWPORT9 vNEW = {
		0, 0,
		vDESC.Width,
		vDESC.Height,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	vCAMERA.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
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

	CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
		vDESC.Width,
		vDESC.Height,
		D3DFMT_A8R8G8B8,
		D3DMULTISAMPLE_NONE,
		0, FALSE,
		&pTRENDER,
		NULL);

	pBACK = GetEXTMAPBar()->m_bRegion ? pREGION : pSKIN;
	pBACK->GetDesc(&vDESC);
	D3DXCreateTexture(
		CMainFrame::m_3DDevice.m_pDevice,
		vDESC.Width,
		vDESC.Height,
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&pTSKINTEX);

	pTSKINTEX->GetSurfaceLevel( 0, &pZBUF);
	D3DXLoadSurfaceFromSurface(
		pZBUF,
		NULL,
		NULL,
		pBACK,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);
	pZBUF->Release();

	pSHADE->GetDesc(&vDESC);
	D3DXCreateTexture(
		CMainFrame::m_3DDevice.m_pDevice,
		vDESC.Width,
		vDESC.Height,
		1, 0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&pTSHADETEX);

	pTSHADETEX->GetSurfaceLevel( 0, &pBACK);
	D3DXLoadSurfaceFromSurface(
		pBACK,
		NULL,
		NULL,
		pSHADE,
		NULL,
		NULL,
		D3DX_FILTER_NONE,
		NULL);
	pBACK->Release();

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTRENDER);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(
		0, 0, 0, 0),
		1.0f, 0);
	HRESULT hr = CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);

	CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, CEXTMAPView::m_pVB, 0, sizeof(PVERTEX));
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pTSKINTEX);
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pTSHADETEX);
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	if(SUCCEEDED(hr))
		CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

	m_pItem->GetRenderTargetData(
		pTRENDER,
		pUNIT->m_pGEOTEX);

	pTSHADETEX->Release();
	pTSKINTEX->Release();

	pTRENDER->Release();
	pBACK->Release();
	pZBUF->Release();
}

void CEXTMAPDoc::UpdateData()
{
	MAPDWORD::iterator it;

	for( it = m_mapUNIT.begin(); it != m_mapUNIT.end(); it++)
	{
		DWORD dwUnitID = (*it).first;
		DWORD dwID = (*it).second;

		MAPMAPOBJ::iterator itOBJ;
		MAPMAPSFX::iterator itSFX;
		MAPMAPSND::iterator itSND;

		MAPMAPOBJ mapOBJ;
		MAPMAPSFX mapSFX;
		MAPMAPSND mapSND;

		MAPDWORD mapOBJID;
		MAPDWORD mapSFXID;

		CRect rect(
			0, 0,
			m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength,
			m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength);

		rect.OffsetRect(
			(dwUnitID % m_pItem->m_HugeMAP.m_nUnitCountX) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength,
			(dwUnitID / m_pItem->m_HugeMAP.m_nUnitCountX) * m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength);

		m_pItem->UpdateUNIT(
			m_pSHADE[dwID],
			m_pSKIN[dwID],
			m_UNIT[dwID].m_pTILETEX,
			&m_UNIT[dwID].m_vSHADOWTEX,
			m_UNIT[dwID].m_pREGION,
			m_UNIT[dwID].m_pDETAIL,
			m_UNIT[dwID].m_pDATA,
			dwUnitID);

		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();

		for( itOBJ = m_pItem->m_pMAP->m_mapOBJ.begin(); itOBJ != m_pItem->m_pMAP->m_mapOBJ.end(); itOBJ++)
		{
			DWORD dwOBJID = (*itOBJ).first;
			CPoint point(
				(INT) (*itOBJ).second->m_vPOS.x,
				(INT) (*itOBJ).second->m_vPOS.z);

			if(rect.PtInRect(point))
			{
				MAPDWORD::iterator itID = m_pItem->m_mapOBJ.find(dwOBJID);

				if( itID != m_pItem->m_mapOBJ.end() )
					mapOBJID.insert( MAPDWORD::value_type( dwOBJID, (*itID).second));

				mapOBJ.insert( MAPMAPOBJ::value_type( dwOBJID, (*itOBJ).second));
			}
		}

		for( itSFX = m_pItem->m_pMAP->m_mapSFX.begin(); itSFX != m_pItem->m_pMAP->m_mapSFX.end(); itSFX++)
		{
			DWORD dwSFXID = (*itSFX).first;
			CPoint point(
				(INT) (*itSFX).second->m_vPOS.x,
				(INT) (*itSFX).second->m_vPOS.z);

			if(rect.PtInRect(point))
			{
				MAPDWORD::iterator itID = m_pItem->m_mapSFX.find(dwSFXID);

				if( itID != m_pItem->m_mapSFX.end() )
					mapSFXID.insert( MAPDWORD::value_type( dwSFXID, (*itID).second));

				mapSFX.insert( MAPMAPSFX::value_type( dwSFXID, (*itSFX).second));
			}
		}

		for( itSND = m_pItem->m_pMAP->m_mapSND.begin(); itSND != m_pItem->m_pMAP->m_mapSND.end(); itSND++)
		{
			DWORD dwSNDID = (*itSND).first;
			CPoint point(
				(INT) (*itSND).second->m_vPOS.x,
				(INT) (*itSND).second->m_vPOS.z);

			if(rect.PtInRect(point))
				mapSND.insert( MAPMAPSND::value_type( dwSNDID, (*itSND).second));
		}

		m_pItem->UpdateOBJ(
			&mapOBJ,
			&mapSFX,
			&mapSND,
			&mapOBJID,
			&mapSFXID,
			dwUnitID);

		mapOBJ.clear();
		mapSFX.clear();
		mapSND.clear();

		mapOBJID.clear();
		mapSFXID.clear();
	}
}

void CEXTMAPDoc::Reload()
{
	MAPOBJECT::iterator itTOBJ;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator it;

	for( itOBJ = m_pItem->m_pMAP->m_mapOBJ.begin(); itOBJ != m_pItem->m_pMAP->m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	for( itSFX = m_pItem->m_pMAP->m_mapSFX.begin(); itSFX != m_pItem->m_pMAP->m_mapSFX.end(); itSFX++)
		delete (*itSFX).second;

	for( itSND = m_pItem->m_pMAP->m_mapSND.begin(); itSND != m_pItem->m_pMAP->m_mapSND.end(); itSND++)
		delete (*itSND).second;

	for( it = m_mapSND.begin(); it != m_mapSND.end(); it++)
	{
		MAPMAPSND::iterator finder = m_pMAP->m_pMAP->m_mapSND.find((*it).first);

		if( finder != m_pMAP->m_pMAP->m_mapSND.end() )
		{
			CD3DSound *pSND = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID);

			CMainFrame::m_TachyonMedia.Stop(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID,
				(*it).second);

			pSND->Unlock((*it).second);
		}
	}

	for( itTOBJ = m_mapOBJ.begin(); itTOBJ != m_mapOBJ.end(); itTOBJ++)
		delete (*itTOBJ).second;

	for( it = m_mapSFX.begin(); it != m_mapSFX.end(); it++)
		CTachyonSFX::Release((*it).second);

	m_pItem->m_pMAP->m_mapOBJ.clear();
	m_pItem->m_pMAP->m_mapSFX.clear();
	m_pItem->m_pMAP->m_mapSND.clear();
	m_pItem->m_mapOBJ.clear();
	m_pItem->m_mapSFX.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapSND.clear();

	for( it = m_mapUNIT.begin(); it != m_mapUNIT.end(); it++)
	{
		DWORD dwUnitID = (*it).first;
		DWORD dwID = (*it).second;

		if(m_pREGION[dwID])
		{
			m_pREGION[dwID]->Release();
			m_pREGION[dwID] = NULL;
		}

		if(m_pSHADE[dwID])
		{
			m_pSHADE[dwID]->Release();
			m_pSHADE[dwID] = NULL;
		}

		if(m_pSKIN[dwID])
		{
			m_pSKIN[dwID]->Release();
			m_pSKIN[dwID] = NULL;
		}

		LoadUNIT(
			&m_UNIT[dwID],
			&m_pREGION[dwID],
			&m_pSHADE[dwID],
			&m_pSKIN[dwID],
			dwUnitID);
	}
}

void CEXTMAPDoc::RefreshView()
{
	if(m_pItem)
	{
		m_pItem->m_HugeMAP.m_nUnitX -= 3;
		m_pItem->m_HugeMAP.m_nUnitZ -= 3;
		m_pItem->m_HugeMAP.m_nCellX -= 3;
		m_pItem->m_HugeMAP.m_nCellZ -= 3;
		m_bResetVB = TRUE;

		RedrawAllView();
		m_bResetVB = FALSE;
	}
}

DWORD CEXTMAPDoc::GetNewOBJID()
{
	DWORD dwID = m_pItem->m_dwNewOBJID;

	m_pItem->m_dwNewOBJID++;
	return dwID;
}

DWORD CEXTMAPDoc::GetNewSFXID()
{
	DWORD dwID = m_pItem->m_dwNewSFXID;

	m_pItem->m_dwNewSFXID++;
	return dwID;
}

DWORD CEXTMAPDoc::GetNewSNDID()
{
	DWORD dwID = m_pItem->m_dwNewSNDID;

	m_pItem->m_dwNewSNDID++;
	return dwID;
}

void CEXTMAPDoc::DeleteForRestore()
{
	CMAPDoc::DeleteForRestore();

	if(m_pSPRAY)
	{
		m_pSPRAY->Release();
		m_pSPRAY = NULL;
	}
}

void CEXTMAPDoc::Restore()
{
	UpdateData();
	Reload();
	RefreshView();

	if( m_pItem->m_HugeMAP.m_nTextureSize != 0 )
	{
		D3DXCreateTexture(
			CMainFrame::m_3DDevice.m_pDevice,
			m_pItem->m_HugeMAP.m_nTextureSize,
			m_pItem->m_HugeMAP.m_nTextureSize, 1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pSPRAY);

		ClearSPRAY();
	}

	CMAPDoc::Restore();
}

void CEXTMAPDoc::OnCloseDocument()
{
	UpdateData();
	CMAPDoc::OnCloseDocument();
}

void CEXTMAPDoc::GetOBJRANDOMPos(D3DXVECTOR3* vPos)
{
	CMAPOBJBar* pBar = GetMAPOBJBar();
	
	D3DXVECTOR3 vTEMP = *vPos;
	BOOL bCheck = FALSE;
	D3DXVECTOR3 vPoint;
	bCheck = FALSE;

	int nTileLength = m_pItem->m_HugeMAP.m_nTileLength;
	FLOAT fRadius = FLOAT( pBar->m_nBrushSize * nTileLength ) / 2.0f;

	while( !bCheck )
	{
		*vPos = vTEMP;

		INT nPOSX = INT( vPos->x / m_pItem->m_HugeMAP.m_nTileLength );
		INT nPOSZ = INT( vPos->z / m_pItem->m_HugeMAP.m_nTileLength );

		FLOAT fCenterX = ( ( nPOSX * m_pItem->m_HugeMAP.m_nTileLength ) + (pBar->m_nBrushSize % 2 ? FLOAT( m_pItem->m_HugeMAP.m_nTileLength ) / 2.0f : 0 ) );
		FLOAT fCenterZ = ( ( nPOSZ * m_pItem->m_HugeMAP.m_nTileLength ) + (pBar->m_nBrushSize % 2 ? FLOAT( m_pItem->m_HugeMAP.m_nTileLength ) / 2.0f : 0 ) );

		srand(timeGetTime());
		
		vPoint.x = FLOAT(rand() % ( INT(fRadius) ) );
		vPoint.z = FLOAT(rand() % ( INT(fRadius) ) );

		vPoint.x += FLOAT( rand() % 100 ) / 100.0f;
		vPoint.z += FLOAT( rand() % 100 ) / 100.0f;

		FLOAT x = rand() % 2 == 1 ? -1.0f : 1.0f;
		FLOAT z = rand() % 2 == 1 ? -1.0f : 1.0f;
		
		vPoint.x *= x;
		vPoint.z *= z;

		switch( pBar->m_nBrushShape )
		{
		case 0: break;
		case 1:
			D3DXVECTOR2 vResult;
			D3DXVECTOR2 vFrom( fCenterX, fCenterZ );
			D3DXVECTOR2 vDest( fCenterX + vPoint.x, fCenterZ + vPoint.z );
			
			D3DXVec2Subtract( &vResult, &vDest, &vFrom );
			FLOAT fLength = D3DXVec2Length( &vResult );
			if( fabs(fLength) > fRadius )
				continue;
			break;
		}
		
		VECTORVEC::iterator it;
		for( it = m_vOBJPrePos.begin() ; it != m_vOBJPrePos.end() ; it++ )
		{
			D3DXVECTOR3 vec = (*it);
			if( vPoint == (*it) )
			{
				bCheck = FALSE;
				break;
			}
		}

		if( it == m_vOBJPrePos.end() )
		{
			m_vOBJPrePos.push_back(vPoint);
			bCheck = TRUE;
		}
		
		vPos->x = fCenterX + vPoint.x;
		vPos->z = fCenterZ + vPoint.z;
	}
}

void CEXTMAPDoc::RotateOBJbyPlane(D3DXVECTOR3* vROT, D3DXVECTOR3* vPOS, CTachyonObject* pOBJ)
{
	enum _PICK_POS
	{
		_PICK_POS_UPPER = 0,
		_PICK_POS_LOWER
	};

	BYTE bPICKPOS;
	CMAPOBJBar* pBar = GetMAPOBJBar();
	
	D3DXPLANE pPLANE;
	D3DXPLANE pNormalPLANE;
	
	INT nSTARTPosX	= INT( vPOS->x / m_pItem->m_HugeMAP.m_nTileLength ) * m_pItem->m_HugeMAP.m_nTileLength;
	INT nSTARTPosZ	= INT( vPOS->z / m_pItem->m_HugeMAP.m_nTileLength ) * m_pItem->m_HugeMAP.m_nTileLength;
	INT nENDPosX	= INT( vPOS->x / m_pItem->m_HugeMAP.m_nTileLength ) * m_pItem->m_HugeMAP.m_nTileLength + m_pItem->m_HugeMAP.m_nTileLength;
	INT nENDPosZ	= INT( vPOS->z / m_pItem->m_HugeMAP.m_nTileLength ) * m_pItem->m_HugeMAP.m_nTileLength + m_pItem->m_HugeMAP.m_nTileLength;

	FLOAT fDistX = vPOS->x - nSTARTPosX;
	FLOAT fDistZ = vPOS->z - nSTARTPosZ;

	D3DXVECTOR3 vPOINT_1(0,0,0);
	D3DXVECTOR3 vPOINT_2(0,0,0);
	D3DXVECTOR3 vPOINT_3(0,0,0);

	if( fDistX + fDistZ < m_pItem->m_HugeMAP.m_nTileLength )
	{		
		vPOINT_1 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nSTARTPosZ)),  FLOAT(nSTARTPosZ));
		vPOINT_2 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nSTARTPosZ)),    FLOAT(nSTARTPosZ));
		vPOINT_3 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nENDPosZ)),    FLOAT(nENDPosZ));
		bPICKPOS = _PICK_POS_LOWER;
	}
	else if( fDistX + fDistZ > m_pItem->m_HugeMAP.m_nTileLength )
	{
		vPOINT_1 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nENDPosZ)),      FLOAT(nENDPosZ));
		vPOINT_2 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nENDPosZ)),    FLOAT(nENDPosZ));
		vPOINT_3 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nSTARTPosZ)),    FLOAT(nSTARTPosZ));
		bPICKPOS = _PICK_POS_UPPER;
	}
	else if( fDistX + fDistZ == m_pItem->m_HugeMAP.m_nTileLength )
	{
		if( GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nSTARTPosZ)) > GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nENDPosZ) ) )
		{
			vPOINT_1 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nSTARTPosZ)), FLOAT(nSTARTPosZ));
			vPOINT_2 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nSTARTPosZ)),	FLOAT(nSTARTPosZ));
			vPOINT_3 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nENDPosZ)),   FLOAT(nENDPosZ));
			bPICKPOS = _PICK_POS_LOWER;
		}
		else
		{
			vPOINT_1 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nENDPosZ)),     FLOAT(nENDPosZ));
			vPOINT_2 = D3DXVECTOR3(FLOAT(nSTARTPosX), GetHeight(FLOAT(nSTARTPosX),0.0f, FLOAT(nENDPosZ)),	FLOAT(nENDPosZ));
			vPOINT_3 = D3DXVECTOR3(FLOAT(nENDPosX),   GetHeight(FLOAT(nENDPosX),0.0f, FLOAT(nSTARTPosZ)),   FLOAT(nSTARTPosZ));
			bPICKPOS = _PICK_POS_UPPER;
		}
	}
	D3DXVECTOR3 vPlanNormal;
	D3DXVECTOR3 vPosNormalY(0,-1,0);
	
	FLOAT fDOTY;
	
	FLOAT fANGLEY;
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vROTY;
	
	D3DXMATRIX vPos;
	D3DXVec3Normalize( &vPosNormalY, &vPosNormalY );
	D3DXPlaneFromPoints( &pPLANE, &vPOINT_1, &vPOINT_2, &vPOINT_3 );
	D3DXPlaneNormalize( &pNormalPLANE, &pPLANE );

	vPlanNormal.x = pNormalPLANE.a;
	vPlanNormal.y = pNormalPLANE.b;
	vPlanNormal.z = pNormalPLANE.c;

	D3DXVec3Normalize( &vPlanNormal, &vPlanNormal );

	FLOAT fLengthPOSNormal;

	fDOTY = D3DXVec3Dot( &vPosNormalY, &vPlanNormal );
	fLengthPOSNormal = D3DXVec3Length(&vPosNormalY);
	
	fDOTY /= fLengthPOSNormal * fLengthPOSNormal;

	fANGLEY = acos(fDOTY);
	
	D3DXVec3Cross( &vCross, &vPosNormalY, &vPlanNormal );
	D3DXVec3Normalize( &vCross, &vCross );

	D3DXMATRIX matResult;
	D3DXMATRIX vRotY;
	D3DXMatrixIdentity( &vRotY );
	D3DXMatrixIdentity( &matResult );

	if( pBar->m_nVerticalOfPlane == 1 )
		D3DXMatrixRotationAxis( &matResult, &vCross, fANGLEY );

	D3DXMatrixRotationY(
		&vRotY,
		vROT->y);

	D3DXMatrixTranslation(
				&vPos,
				vPOS->x,
				vPOS->y,
				vPOS->z );

	pOBJ->m_vWorld = vRotY * matResult * vPos;
	
	FLOAT fX;
	FLOAT fY;
	FLOAT fZ;

	fY = asin(matResult._13);
	fX = asin(matResult._23 / cos(fY));	
	fZ = asin(matResult._12 / cos(fY));

	(*vROT).x = fX;
	(*vROT).y += fY;
	(*vROT).z = fZ;
}

void CEXTMAPDoc::ObjectReplace(BOOL bALLBOJ)
{	
	CMAPOBJBar* pBar = GetMAPOBJBar();

	VECTORDWORD* vEqualOBJ = new VECTORDWORD;
	VECTORDWORD::iterator itEqualOBJ;
	MAPMAPOBJ::iterator itMAPOBJ;

	BOOL bCheck = TRUE;
	if( bALLBOJ )
	{
		GetEqualOBJ( vEqualOBJ, bALLBOJ );

		for( itEqualOBJ = vEqualOBJ->begin() ; itEqualOBJ != vEqualOBJ->end() ; itEqualOBJ++ )
		{
			itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find( (*itEqualOBJ) );
			if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
			{
				if( !( bCheck = InsertMapOBJ( (*itMAPOBJ).second ) ) )
					break;
				m_vSELOBJ.push_back( (*itEqualOBJ) );
			}	
			else 
				continue;
		}
	}
	else
	{
		GetEqualOBJ( vEqualOBJ, bALLBOJ );
		itEqualOBJ = vEqualOBJ->begin();

		itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find( (*itEqualOBJ) );
		if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
		{
			bCheck = InsertMapOBJ( (*itMAPOBJ).second );
			
			m_vSELOBJ.push_back( (*itEqualOBJ) );
		}
	}

	if( bCheck ) pBar->OnObjectDelete();
	vEqualOBJ->clear();
	delete vEqualOBJ;
}

void CEXTMAPDoc::ObjectAttach(BOOL bALLBOJ)
{	
	CMAPOBJBar* pBar = GetMAPOBJBar();

	VECTORDWORD* vEqualOBJ = new VECTORDWORD;
	VECTORDWORD::iterator itEqualOBJ;
	MAPMAPOBJ::iterator itMAPOBJ;

	GetEqualOBJ( vEqualOBJ , bALLBOJ );

	if(bALLBOJ)
	{
		for( itEqualOBJ = vEqualOBJ->begin() ; itEqualOBJ != vEqualOBJ->end() ; itEqualOBJ++ )
		{
			itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find( (*itEqualOBJ) );
			if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
				if( !InsertMapOBJ( (*itMAPOBJ).second) )
					break;
			else 
				continue;
		}
	}
	else
	{
		itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find( m_vSELOBJ[0]);
		if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
			InsertMapOBJ( (*itMAPOBJ).second);
	}

	vEqualOBJ->clear();
	delete vEqualOBJ;
}

void CEXTMAPDoc::ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos )
{
	VECTORDWORD* vEqualOBJ = new VECTORDWORD;
	MAPMAPOBJ* vMAPMAPOBJ = new MAPMAPOBJ;
	VECTORDWORD::iterator itEqualOBJ;
	MAPMAPOBJ::iterator itMAPOBJ;

	GetEqualOBJ( vEqualOBJ, pMAPOBJ, pMAPID, pOBJ, dwID );
	
	for( itEqualOBJ = vEqualOBJ->begin() ; itEqualOBJ != vEqualOBJ->end() ; itEqualOBJ++ )
	{
		itMAPOBJ = pMAPOBJ->find( (*itEqualOBJ) );
		if( itMAPOBJ != pMAPOBJ->end() )
			InsertMapOBJ( vMAPMAPOBJ, pMAPID, (*itMAPOBJ).second, vMovePos );
	}

	for( itMAPOBJ = vMAPMAPOBJ->begin() ; itMAPOBJ != vMAPMAPOBJ->end() ; itMAPOBJ++ )
		pMAPOBJ->insert( MAPMAPOBJ::value_type( (*itMAPOBJ).first, (*itMAPOBJ).second ) );

	vMAPMAPOBJ->clear();
	vEqualOBJ->clear();

	delete vMAPMAPOBJ;
	delete vEqualOBJ;
}

void CEXTMAPDoc::ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos )
{
	VECTORDWORD* vEqualOBJ = new VECTORDWORD;
	MAPMAPOBJ* vMAPMAPOBJ = new MAPMAPOBJ;
	VECTORDWORD::iterator itEqualOBJ;
	MAPMAPOBJ::iterator itMAPOBJ;
	MAPDWORD::iterator itMAPOBJID;

	GetEqualOBJ( vEqualOBJ, pMAPOBJ, pMAPID, pOBJ, dwID );
	
	for( itEqualOBJ = vEqualOBJ->begin() ; itEqualOBJ != vEqualOBJ->end() ; itEqualOBJ++ )
	{
		itMAPOBJ = pMAPOBJ->find( (*itEqualOBJ) );
		if( itMAPOBJ != pMAPOBJ->end() )
			InsertMapOBJ( vMAPMAPOBJ, pMAPID, (*itMAPOBJ).second, vMovePos );
	}

	for( itMAPOBJ = vMAPMAPOBJ->begin() ; itMAPOBJ != vMAPMAPOBJ->end() ; itMAPOBJ++ )
		pMAPOBJ->insert( MAPMAPOBJ::value_type( (*itMAPOBJ).first, (*itMAPOBJ).second ) );

	for( itEqualOBJ = vEqualOBJ->begin() ; itEqualOBJ != vEqualOBJ->end() ; itEqualOBJ++ )
	{
		itMAPOBJ = pMAPOBJ->find((*itEqualOBJ));
		itMAPOBJID = pMAPID->find((*itEqualOBJ));
		if( itMAPOBJ != pMAPOBJ->end() && itMAPOBJID != pMAPID->end() )
		{
			pMAPOBJ->erase(itMAPOBJ);
			pMAPID->erase(itMAPOBJID);
		}
		
	}
    
	vMAPMAPOBJ->clear();
	vEqualOBJ->clear();

	delete vMAPMAPOBJ;
	delete vEqualOBJ;
}

void CEXTMAPDoc::GetEqualOBJ(VECTORDWORD* vEqualOBJ, MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID)
{
	MAPDWORD::iterator itFindOBJID;
	MAPMAPOBJ::iterator itMAPMAPOBJ;
	
	MAPCLKINST::iterator itCI;
	MAPCLKINST::iterator itMAPMAPCI;
	
	for( itFindOBJID = pMAPID->begin() ; itFindOBJID != pMAPID->end() ; itFindOBJID++ )
	{
		if( (*itFindOBJID).second == dwID )
		{
			itCI = pOBJ->m_OBJ.m_mapCLK.find( pOBJ->m_OBJ.m_dwCurCLK );
			
			if( itCI == pOBJ->m_OBJ.m_mapCLK.end() )
				continue;
			
			DWORD dwMeshID = (*itCI).second->m_dwCurMESH;

			itMAPMAPOBJ = pMAPOBJ->find( (*itFindOBJID).first );

			if( itMAPMAPOBJ == pMAPOBJ->end() )
				continue;

			itMAPMAPCI = (*itMAPMAPOBJ).second->m_mapCLK.find( pOBJ->m_OBJ.m_dwCurCLK );

			if( itMAPMAPCI == (*itMAPMAPOBJ).second->m_mapCLK.end() )
				continue;
			if( (*itMAPMAPCI).second->m_dwCurMESH == dwMeshID )
				vEqualOBJ->push_back((*itFindOBJID).first);
		}
	}
}

void CEXTMAPDoc::GetEqualOBJ(VECTORDWORD* vEqualOBJ, BOOL bALLOBJ)
{
	MAPDWORD::iterator itOBJID = m_pMAP->m_mapOBJ.find(m_vSELOBJ[0]);
	MAPDWORD::iterator itFindOBJID;
	MAPOBJECT::iterator itMAPOBJ;
	MAPOBJECT::iterator itCURMAPOBJ;
	
	LPCLKINST pCIMAP = NULL;
	LPCLKINST pCICUR = NULL;

	CMAPOBJBar* pBar = GetMAPOBJBar();
	
	if( bALLOBJ )
	{
		for( itFindOBJID = m_pMAP->m_mapOBJ.begin() ; itFindOBJID != m_pMAP->m_mapOBJ.end() ; itFindOBJID++ )
		{
			if( (*itFindOBJID).second == (*itOBJID).second)
			{
				itMAPOBJ = m_mapOBJ.find( (*itFindOBJID).first );
				itCURMAPOBJ = m_mapOBJ.find( (*itOBJID).first );

				if( itMAPOBJ != m_mapOBJ.end() )
				{
					if( (*itMAPOBJ).second->m_OBJ.m_dwCurCLK == (*itCURMAPOBJ).second->m_OBJ.m_dwCurCLK )
					{
						pCIMAP = (*(*itMAPOBJ).second->m_OBJ.m_mapCLK.find( (*itMAPOBJ).second->m_OBJ.m_dwCurCLK )).second;
						pCICUR = (*(*itCURMAPOBJ).second->m_OBJ.m_mapCLK.find( (*itCURMAPOBJ).second->m_OBJ.m_dwCurCLK)).second;
						if( pCIMAP->m_dwCurCL == pCICUR->m_dwCurCL )
							if( pCIMAP->m_dwCurMESH == pCICUR->m_dwCurMESH )
								vEqualOBJ->push_back( (*itFindOBJID).first );
					}
				}
			}
		}
	}
	else
	{
		itMAPOBJ = m_mapOBJ.find( (*itOBJID).first );
		if( itMAPOBJ != m_mapOBJ.end() )
			if( (*itMAPOBJ).second->m_OBJ.m_dwCurCLK )
				vEqualOBJ->push_back( (*itOBJID).first );
	}
}

BOOL CEXTMAPDoc::InsertMapOBJ(MAPMAPOBJ* pMAPMAPOBJ, MAPDWORD* pMAPOBJID, LPMAPOBJ pOLDOBJ, D3DXVECTOR3* vMovePos)
{	
	m_vOBJPrePos.clear();
	LPMAPOBJ pMAPOBJ = new MAPOBJ();
	DWORD dwID = GetNewOBJID();

	MAPCLKINST::iterator itCI;

	if( !CMAPOBJBar::m_pOBJ )
	{
		MessageBox(NULL,"Select Object In Object Tree", NULL, MB_ICONWARNING|MB_OK );
		return FALSE;
	}
	
	if( CMAPOBJBar::m_pTOBJ->m_OBJ.m_dwCurCLK == 0)
	{
		MessageBox(NULL,"Select Object Cloth Kind In Object Bar", NULL, MB_ICONWARNING|MB_OK );
		return FALSE;
	}

	pMAPOBJ->m_pOBJ = &CMAPOBJBar::m_pOBJ->m_OBJ;

	for( itCI = CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapCLK.begin(); itCI != CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapCLK.end(); itCI++)
	{
		LPCLKINST pOBJCI = new CLKINST();
		LPCLKINST pCI = (*itCI).second;

		memcpy( pOBJCI, pCI, sizeof(CLKINST));
		pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type( (*itCI).first, pOBJCI));
	}

	MAPACTINST::iterator itAI = CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapACT.find(CMAPOBJBar::m_pTOBJ->m_OBJ.m_dwCurACT);
	if( itAI != CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapACT.end() )
	{
		LPACTINST pAI = (*itAI).second;

		pMAPOBJ->m_dwCurANI = pAI->m_dwCurANI;
		pMAPOBJ->m_dwCurACT = (*itAI).first;
	}
	
	MAPATTRITEM::iterator itATTR;
	for( itATTR = CMAPOBJBar::m_pOBJ->m_mapATTR.begin(); itATTR != CMAPOBJBar::m_pOBJ->m_mapATTR.end(); itATTR++)
	{
		LPATTRIBUTE pATTR = new ATTRIBUTE();

		pATTR->m_bValueType = (*itATTR).second->m_bType;
		switch(pATTR->m_bValueType)
		{
		case ATTRTYPE_STRING	:
			{
				pATTR->m_nSize = (*itATTR).second->m_strValue.GetLength();
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				memcpy( pATTR->m_pValue, LPCSTR((*itATTR).second->m_strValue), pATTR->m_nSize);
			}

			break;

		case ATTRTYPE_FLOAT		:
			{
				pATTR->m_nSize = sizeof(FLOAT);
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				*((FLOAT *) pATTR->m_pValue) = (FLOAT) atof((*itATTR).second->m_strValue);
			}

			break;

		case ATTRTYPE_INT		:
			{
				pATTR->m_nSize = sizeof(int);
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				*((int *) pATTR->m_pValue) = atoi((*itATTR).second->m_strValue);
			}

			break;
		}

		pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type(
			(*itATTR).first,
			pATTR));
	}

	D3DXMATRIX matSCALE, matROT;
	D3DXMatrixIdentity(&matSCALE);
	D3DXMatrixIdentity(&matROT);

	D3DXMatrixScaling(&matSCALE, (*pOLDOBJ).m_vSCALE.x, (*pOLDOBJ).m_vSCALE.y, (*pOLDOBJ).m_vSCALE.z);
	D3DXMatrixRotationX(&matROT, (*pOLDOBJ).m_vROT.x);
	D3DXMatrixRotationY(&matROT, (*pOLDOBJ).m_vROT.y);
	D3DXMatrixRotationZ(&matROT, (*pOLDOBJ).m_vROT.z);
	CTMath::Transform(&matSCALE, vMovePos);
	CTMath::Transform(&matROT, vMovePos);
	
	(*pMAPOBJ).m_vPOS   = (*pOLDOBJ).m_vPOS + (*vMovePos);
	(*pMAPOBJ).m_vSCALE = (*pOLDOBJ).m_vSCALE;
	(*pMAPOBJ).m_vROT   = (*pOLDOBJ).m_vROT;	

	pMAPMAPOBJ->insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
	pMAPOBJID->insert( MAPDWORD::value_type( dwID, CMAPOBJBar::m_pOBJ->m_dwID ) );
	return TRUE;
}	

BOOL CEXTMAPDoc::InsertMapOBJ(LPMAPOBJ pOLDOBJ )
{
	m_vOBJPrePos.clear();
	CTachyonObject *pOBJ = new CTachyonObject();
	LPMAPOBJ pMAPOBJ = new MAPOBJ();
	DWORD dwID = GetNewOBJID();
	
	MAPCLKINST::iterator itCI;

	if( !CMAPOBJBar::m_pOBJ )
	{
		MessageBox(NULL,"Select Object In Object Tree", NULL, MB_ICONWARNING|MB_OK );
		return FALSE;
	}
	
	if( CMAPOBJBar::m_pTOBJ->m_OBJ.m_dwCurCLK == 0)
	{
		MessageBox(NULL,"Select Object Cloth Kind In Object Bar", NULL, MB_ICONWARNING|MB_OK );
		return FALSE;
	}

	m_pMAP->m_mapOBJ.insert( MAPDWORD::value_type( dwID, CMAPOBJBar::m_pOBJ->m_dwID));
	pMAPOBJ->m_pOBJ = &CMAPOBJBar::m_pOBJ->m_OBJ;
	
	for( itCI = CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapCLK.begin(); itCI != CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapCLK.end(); itCI++)
	{
		LPCLKINST pOBJCI = new CLKINST();
		LPCLKINST pCI = (*itCI).second;

		memcpy( pOBJCI, pCI, sizeof(CLKINST));
		pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type( (*itCI).first, pOBJCI));
	}

	MAPACTINST::iterator itAI = CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapACT.find(CMAPOBJBar::m_pTOBJ->m_OBJ.m_dwCurACT);
	if( itAI != CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapACT.end() )
	{
		LPACTINST pAI = (*itAI).second;

		pMAPOBJ->m_dwCurANI = pAI->m_dwCurANI;
		pMAPOBJ->m_dwCurACT = (*itAI).first;
	}

	MAPATTRITEM::iterator itATTR;
	for( itATTR = CMAPOBJBar::m_pOBJ->m_mapATTR.begin(); itATTR != CMAPOBJBar::m_pOBJ->m_mapATTR.end(); itATTR++)
	{
		LPATTRIBUTE pATTR = new ATTRIBUTE();

		pATTR->m_bValueType = (*itATTR).second->m_bType;
		switch(pATTR->m_bValueType)
		{
		case ATTRTYPE_STRING	:
			{
				pATTR->m_nSize = (*itATTR).second->m_strValue.GetLength();
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				memcpy( pATTR->m_pValue, LPCSTR((*itATTR).second->m_strValue), pATTR->m_nSize);
			}

			break;

		case ATTRTYPE_FLOAT		:
			{
				pATTR->m_nSize = sizeof(FLOAT);
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				*((FLOAT *) pATTR->m_pValue) = (FLOAT) atof((*itATTR).second->m_strValue);
			}

			break;

		case ATTRTYPE_INT		:
			{
				pATTR->m_nSize = sizeof(int);
				pATTR->m_pValue = new BYTE[pATTR->m_nSize];

				*((int *) pATTR->m_pValue) = atoi((*itATTR).second->m_strValue);
			}

			break;
		}

		pMAPOBJ->m_mapATTR.insert( MAPATTRIBUTE::value_type(
			(*itATTR).first,
			pATTR));
	}
	
	(*pMAPOBJ).m_vPOS   = (*pOLDOBJ).m_vPOS;
	(*pMAPOBJ).m_vSCALE = (*pOLDOBJ).m_vSCALE;
	(*pMAPOBJ).m_vROT   = (*pOLDOBJ).m_vROT;	

	pOBJ->InitOBJ( &CMainFrame::m_3DDevice, pMAPOBJ);
	pOBJ->m_pHost = GetMAPOBJBar();
	
	m_pMAP->m_pMAP->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
	m_mapOBJ.insert( MAPOBJECT::value_type( dwID, pOBJ));

	return TRUE;
}

FLOAT CEXTMAPDoc::GetHeight( D3DXVECTOR3 vPos) 
{
	FLOAT fMapHeight = GetHeight( vPos.x, vPos.y, vPos.z);
	FLOAT fRadius = 0.0f;
	FLOAT fHeight = m_pItem->m_fMinHeight;

	MAPOBJECT::iterator itOBJ;
	for( itOBJ = m_mapOBJ.begin(); itOBJ != m_mapOBJ.end(); itOBJ++)
	{
		CTachyonObject *pOBJ = (*itOBJ).second;
		BOOL bHideChk = IsHideOBJ((*itOBJ).first);			// 박상연 Hide 상태 체크

		if(!bHideChk)	
		{
			MAPCLKINST::iterator it;

			for( it = pOBJ->m_OBJ.m_mapCLK.begin(); it != pOBJ->m_OBJ.m_mapCLK.end(); it++)
			{
				LPCLKINST pCI = (*it).second;

				if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
					pCI->m_pMESH->m_pMESH->m_pDATA &&
					pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
				{
					CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

					if( pMESH->m_dwNodeCount > 0 )
					{
						LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

						for( int i=0; i<pMESH->m_dwMeshCount; i++)
							for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
								for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
								{
									D3DXVECTOR3 vPOLY[3];

									for( int l=0; l<3; l++)
									{
										vPOLY[l] = D3DXVECTOR3(
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

										CTMath::Transform(
											&pOBJ->m_vPosition,
											&vPOLY[l]);
									}

									FLOAT fLocal = GetPolyHeight(
										vPOLY,
										&vPos,
										fRadius);

									if( fHeight < fLocal )
										fHeight = fLocal;
								}
					}
					else
					{
						LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;

						for( int i=0; i<pMESH->m_dwMeshCount; i++)
							for( int j=0; j<INT(pMESH->m_pMESH[i][0]->m_vIB.size()); j++)
								for( int k=0; k<pMESH->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
								{
									D3DXVECTOR3 vPOLY[3];

									for( int l=0; l<3; l++)
									{
										vPOLY[l] = D3DXVECTOR3(
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosX,
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosY,
											pDATA[pMESH->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + l]].m_fPosZ);

										CTMath::Transform(
											&pOBJ->m_vPosition,
											&vPOLY[l]);
									}

									FLOAT fLocal = GetPolyHeight(
										vPOLY,
										&vPos,
										fRadius);

									if( fHeight < fLocal )
										fHeight = fLocal;
								}
					}
				}
			}
		}
	}

	return fHeight > fMapHeight ? fHeight : fMapHeight;
}

FLOAT CEXTMAPDoc::GetPolyHeight(LPD3DXVECTOR3 pPOLY , LPD3DXVECTOR3 pPOS , FLOAT fRadius)
{
	if( pPOLY[0].y > pPOS->y + 0.1f &&
		pPOLY[1].y > pPOS->y + 0.1f &&
		pPOLY[2].y > pPOS->y + 0.1f )
		return m_pItem->m_fMinHeight;
	D3DXPLANE vPLANE;

	D3DXPlaneFromPoints(
		&vPLANE,
		&pPOLY[0],
		&pPOLY[1],
		&pPOLY[2]);

	D3DXPlaneNormalize(
		&vPLANE,
		&vPLANE);

	if( vPLANE.b > 0.0f )
	{
		D3DXVECTOR3 vPOS(
			vPLANE.a,
			0.0f,
			vPLANE.c);

		FLOAT fLength = D3DXVec3Length(&vPOS);
		FLOAT fDist;
		FLOAT fU;
		FLOAT fV;

		if( fLength > 0.0f )
		{
			vPOS *= fRadius / fLength;
			vPOS = (*pPOS) - vPOS;
		}
		else
			vPOS = (*pPOS);

		if( D3DXIntersectTri(
			&pPOLY[0],
			&pPOLY[1],
			&pPOLY[2],
			&D3DXVECTOR3( vPOS.x, 0.0f, vPOS.z),
			&D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
			&fU, &fV, &fDist) )
			return pPOS->y < fDist ? m_pItem->m_fMinHeight : fDist;
		fDist = m_pItem->m_fMinHeight;

		for( int i=0; i<3; i++)
		{
			D3DXVECTOR2 vA(
				pPOLY[(i + 1) % 3].x - pPOLY[i].x,
				pPOLY[(i + 1) % 3].z - pPOLY[i].z);

			D3DXVECTOR2 vB(
				pPOS->x - pPOLY[i].x,
				pPOS->z - pPOLY[i].z);

			D3DXVECTOR2 vC = D3DXVec2Dot( &vA, &vB) / D3DXVec2Dot( &vA, &vA) * vA - vB;
			fLength = fRadius * fRadius - D3DXVec2Dot( &vC, &vC);

			if( D3DXVec2Dot( &vB, &vB) <= fRadius * fRadius && fDist < pPOLY[i].y )
				fDist = pPOLY[i].y;
			if( fLength >= 0.0f )
			{
				FLOAT fEdge = D3DXVec2Length(&vA);

				fLength = sqrtf(fLength);
				vA /= fEdge;

				D3DXVECTOR3 vNormal( vA.x, 0.0f, vA.y);
				D3DXVECTOR3 vPOINT[2] = {
					D3DXVECTOR3( pPOS->x + vC.x + fLength * vA.x, 0.0f, pPOS->z + vC.y + fLength * vA.y),
					D3DXVECTOR3( pPOS->x + vC.x - fLength * vA.x, 0.0f, pPOS->z + vC.y - fLength * vA.y)};

				for( int j=0; j<2; j++)
				{
					D3DXPlaneFromPointNormal(
						&vPLANE,
						&vPOINT[j],
						&vNormal);

					D3DXPlaneNormalize(
						&vPLANE,
						&vPLANE);

					if( D3DXPlaneDotCoord( &vPLANE, &pPOLY[(i + 1) % 3]) * D3DXPlaneDotCoord( &vPLANE, &pPOLY[i]) <= 0.0f )
					{
						fLength = D3DXVec2Length(&D3DXVECTOR2(
							vPOINT[j].x - pPOLY[i].x,
							vPOINT[j].z - pPOLY[i].z));
						fLength = pPOLY[i].y + fLength * (pPOLY[(i + 1) % 3].y - pPOLY[i].y) / fEdge;

						if( fDist < fLength )
							fDist = fLength;
					}
				}
			}
		}

		return pPOS->y < fDist ? m_pItem->m_fMinHeight : fDist;
	}

	return m_pItem->m_fMinHeight;
}

void CEXTMAPDoc::ResetOBJATTR()
{
	CMAPOBJBar *pBAR = GetMAPOBJBar();
	CProgressDlg Progress;
	BMPARAM Param;
	UpdateData();

	Progress.m_strCancel.LoadString(IDS_CANCEL_RESET_ATTRIBUTE);
	Progress.m_strTitle.LoadString(IDS_RESET_ATTRIBUTE);

	Param.m_pWnd = &Progress;
	Param.m_pHost = m_pItem;

	Progress.m_pThreadFunc = (AFX_THREADPROC) CEXTMAPItem::__ResetATTR;
	Progress.m_lParam = &Param;
	Progress.DoModal();

	Reload();
	RefreshView();

	if(pBAR)
		pBAR->UpdateItem();
}

void CEXTMAPDoc::AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType)
{
	UpdateData();

	MAPOBJECT::iterator itOBJ;
	MAPDWORD::iterator itOBJID;

	CProgressDlg Progress;
	ADOBPARAM Param;
	
	Param.m_pWnd = &Progress;
	Param.m_pHost = m_pItem;
	
	if( pUnitXZ != NULL ) 
		Param.m_dwUnitPOS = ( ( pUnitXZ->y / m_pItem->m_HugeMAP.m_nUnitLength ) * m_pItem->m_HugeMAP.m_nUnitCountX ) +
							( pUnitXZ->x % m_pItem->m_HugeMAP.m_nUnitLength );
	else
		Param.m_dwUnitPOS = -1;
	
	itOBJ = m_mapOBJ.find(m_vSELOBJ[0]);
	if( itOBJ != m_mapOBJ.end() )
		Param.m_pOBJ = (*itOBJ).second;
	else 
		return;

	Param.m_vMovePos = vMoveOffset;
	Param.m_bAttachType = bAttachType;

	itOBJID = m_pMAP->m_mapOBJ.find(m_vSELOBJ[0]);
	if( itOBJID != m_pMAP->m_mapOBJ.end() )
		Param.m_dwBASEOBJID = (*itOBJID).second;
	else 
		return;
	ClearSELOBJ();
	Progress.m_strCancel.LoadString(IDS_CANCEL_IMPOBJ);
	Progress.m_strTitle.LoadString(IDS_RESET_ATTRIBUTE);
	Progress.m_pThreadFunc = (AFX_THREADPROC)CEXTMAPItem::__AdjustOBJPOS;
	Progress.m_lParam = &Param;
	Progress.DoModal();

	Reload();
	RefreshView();
}

void CEXTMAPDoc::ResetDETAIL()
{
	CEXTMAPBar *pControl = GetEXTMAPBar();

	if(m_pItem)
		m_pItem->Regulate();

	if(pControl)
		pControl->ResetDETAIL();
}

LPVECLPOINT CEXTMAPDoc::GetCollisionPos( CTachyonObject *pOBJ, COBJItem *pOBJITEM)
{
	enum TCOLLISION_TYPE
	{
		COLLISION_NONE = 0,
		COLLISION_BOX,
		COLLISION_CYLINDER,
		COLLISION_POLY,
		COLLISION_COUNT
	};

	FLOAT fHeight	= 180.0f;

	DWORD dwCOLLTypeID	= pOBJITEM->GetAttributeID("ID_COLLISION_TYPE");
	DWORD dwTypeID		= pOBJITEM->GetAttributeID("ID_OBJ_TYPE");
	DWORD dwSizeXID		= pOBJITEM->GetAttributeID("ID_SIZE_X");
	DWORD dwSizeYID		= pOBJITEM->GetAttributeID("ID_SIZE_Y");
	DWORD dwSizeZID		= pOBJITEM->GetAttributeID("ID_SIZE_Z");
	DWORD dwDepthID		= pOBJITEM->GetAttributeID("ID_DEPTH");

	INT nCollType	= dwCOLLTypeID	== -1 ? 0 : pOBJ->GetAttrINT(dwCOLLTypeID);
	INT	nType		= dwTypeID		== -1 ? 0 : pOBJ->GetAttrINT(dwTypeID);
	FLOAT fSizeX	= dwSizeXID 	== -1 ? 0 : pOBJ->GetAttrFLOAT(dwSizeXID) * pOBJ->m_vScale._11;
	FLOAT fSizeY	= dwSizeYID 	== -1 ? 0 : pOBJ->GetAttrFLOAT(dwSizeYID) * pOBJ->m_vScale._22;
	FLOAT fSizeZ	= dwSizeZID 	== -1 ? 0 : pOBJ->GetAttrFLOAT(dwSizeZID) * pOBJ->m_vScale._33;
	FLOAT fDepth	= dwDepthID 	== -1 ? 0 : pOBJ->GetAttrFLOAT(dwDepthID) * pOBJ->m_vScale._22;

	if(nCollType == COLLISION_NONE)
		return NULL;

	FLOAT fRadius	= 0.0f;
	FLOAT fRadiusH	= 0.0f;

	fRadius = max( fSizeX,	fSizeZ);

	if( nCollType != COLLISION_CYLINDER )
		fRadius *= 1.5f;

	fRadius /= 2.0f;

	fRadiusH = max(
		fSizeY,
		fRadius);

	fRadiusH = max(
		fRadiusH,
		fDepth);

	LPVECLPOINT vCOLPOINT = new VECLPOINT;
	LPOINT vPoint;

	vPoint.m_fPosX		= pOBJ->m_vPosition._41;
	vPoint.m_fPosY		= fHeight;
	vPoint.m_fPosZ		= pOBJ->m_vPosition._43;
	vPoint.m_dwColor	= 0xFFFF0000;

	LPANI pANI = pOBJ->GetCurANI();
	MAPCLKINST::iterator itCLK;

	for( itCLK = pOBJ->m_OBJ.m_mapCLK.begin(); itCLK != pOBJ->m_OBJ.m_mapCLK.end(); itCLK++)
	{
		LPCLKINST pCI = (*itCLK).second;

		if( pCI && pCI->m_pMESH && pCI->m_pMESH->m_pMESH &&
			pCI->m_pMESH->m_pMESH->m_pDATA &&
			pCI->m_pMESH->m_pMESH->m_pDATA->m_dwCount )
		{
			CTachyonMesh *pMESH = pCI->m_pMESH->m_pMESH;

			if( pMESH->m_dwNodeCount > 0 )
			{
				LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_pDATA->m_pVB;

				for( int j=0; j<pMESH->m_dwMeshCount; j++)
					for( int k=0; k<INT(pMESH->m_pMESH[j][0]->m_vIB.size()); k++)
						for( int l=0; l<pMESH->m_pMESH[j][0]->m_vIB[k]->m_dwCount / 3; l++)
						{
							D3DXVECTOR3 vPOLY[3];
							D3DXVECTOR3 vNORMAL;

							D3DXVECTOR3 vPOINT(
								0.0f,
								0.0f,
								0.0f);

							for( int m=0; m<3; m++)
							{
								D3DXMATRIX vTrans = pANI && pANI->m_pANI ? CTMath::GetTransformMatrix(
									pOBJ->m_pBone,
									pOBJ->GetMeshMatrix(),
									&pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]]) : pOBJ->m_vPosition;

								vPOLY[m] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosX,
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosY,
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosZ);

								CTMath::Transform(
									&vTrans,
									&vPOLY[m]);
								vPOINT += vPOLY[m];
							}

							D3DXVec3Cross(
								&vNORMAL,
								&(vPOLY[1] - vPOLY[0]),
								&(vPOLY[2] - vPOLY[0]));

							vNORMAL /= D3DXVec3Length(&vNORMAL);
							vNORMAL.y = 0.0f;
							vNORMAL /= D3DXVec3Length(&vNORMAL);
							vNORMAL *= 1.1f;

							vPOINT /= 3.0f;
							vPOINT += vNORMAL;

							for( m=0; m<3; m++)
							{
								vPoint.m_fPosX		= vPOLY[m].x;
								vPoint.m_fPosY		= /*fHeight;//*/vPOLY[m].y;
								vPoint.m_fPosZ		= vPOLY[m].z;
								vPoint.m_dwColor	= 0xFFFF0000;

								vCOLPOINT->push_back(vPoint);
							}
							break;
						}
			}
			else
			{
				LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_pDATA->m_pVB;
				for( int j=0; j<pMESH->m_dwMeshCount; j++)
					for( int k=0; k<INT(pMESH->m_pMESH[j][0]->m_vIB.size()); k++)
						for( int l=0; l<pMESH->m_pMESH[j][0]->m_vIB[k]->m_dwCount / 3; l++)
						{
							D3DXVECTOR3 vPOLY[3];
							D3DXVECTOR3 vNORMAL;

							D3DXVECTOR3 vPOINT(
								0.0f,
								0.0f,
								0.0f);

							for( int m=0; m<3; m++)
							{
								vPOLY[m] = D3DXVECTOR3(
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosX,
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosY,
									pDATA[pMESH->m_pMESH[j][0]->m_vIB[k]->m_pIB[3 * l + m]].m_fPosZ);

								CTMath::Transform(
									&pOBJ->m_vPosition,
									&vPOLY[m]);
								vPOINT += vPOLY[m];
							}

							D3DXVec3Cross(
								&vNORMAL,
								&(vPOLY[1] - vPOLY[0]),
								&(vPOLY[2] - vPOLY[0]));

							vNORMAL /= D3DXVec3Length(&vNORMAL);
							vNORMAL.y = 0.0f;
							vNORMAL /= D3DXVec3Length(&vNORMAL);
							vNORMAL *= 1.1f;

							vPOINT /= 3.0f;
							vPOINT += vNORMAL;

							for( m=0; m<3; m++)
							{
								vPoint.m_fPosX		= vPOLY[m].x;
								vPoint.m_fPosY		= /*fHeight;//*/vPOLY[m].y;
								vPoint.m_fPosZ		= vPOLY[m].z;
								vPoint.m_dwColor	= 0xFFFF0000;

								vCOLPOINT->push_back(vPoint);
							}
						}
			}
		}
	}
	
	INT nUnitSize	= m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength;
	INT nUnitX		= INT(pOBJ->m_vPosition._41) / nUnitSize;
	INT nUnitZ		= INT(pOBJ->m_vPosition._43) / nUnitSize;

	D3DXVECTOR3 ptEdge[3] =
	{
		D3DXVECTOR3(FLOAT(nUnitX * nUnitSize),					110.f/*pOBJ->m_vPosition._42 + 1.0f*/,	FLOAT(nUnitZ * nUnitSize)),
		D3DXVECTOR3(FLOAT(nUnitX * nUnitSize),					110.f/*pOBJ->m_vPosition._42 + 1.0f*/,	FLOAT((nUnitZ * nUnitSize) + nUnitSize)),
		D3DXVECTOR3(FLOAT((nUnitX * nUnitSize) + nUnitSize),	110.f/*pOBJ->m_vPosition._42 + 1.0f*/,	FLOAT((nUnitZ * nUnitSize) + nUnitSize))
	};

	D3DXPLANE plane(0,1,0,-110);
	
	LPVECLPOINT vEDGE = new VECLPOINT;
	VECLPOINT::iterator it;

	for(it = vCOLPOINT->begin(); it != vCOLPOINT->end(); it++)
	{
		if(it == vCOLPOINT->end())	break;

		D3DXVECTOR3 vPOINT[3];

		for(BYTE i=0; i<3; i++)
		{
			vPOINT[i].x = (*it).m_fPosX;
			vPOINT[i].y = (*it).m_fPosY;
			vPOINT[i].z = (*it).m_fPosZ;
			it++;

			if(it == vCOLPOINT->end())
				break;
		}

		if(it == vCOLPOINT->end())
			break;
		
		if((vPOINT[0].y >= 110.0f && vPOINT[1].y >= 110.0f && vPOINT[2].y >= 110.0f) ||
			vPOINT[0].y <= 110.0f && vPOINT[1].y <= 110.0f && vPOINT[2].y <= 110.0f )
			continue;
		
		D3DXVECTOR3 vRESULT(0.0f, 0.0f, 0.0f);

		LPOINT point;
		if(D3DXPlaneIntersectLine(&vRESULT, &plane, &vPOINT[0], &vPOINT[1]) != NULL)
		{
			point.m_fPosX = vRESULT.x;
			point.m_fPosY = vRESULT.y + 50.0f;
			point.m_fPosZ = vRESULT.z;
			point.m_dwColor = 0xFFFF0000;

			if(vRESULT.x < (max(vPOINT[0].x, vPOINT[1].x) + 0.3f) && vRESULT.x > (min(vPOINT[0].x, vPOINT[1].x) - 0.3f) )
				if(vRESULT.z <= (max(vPOINT[0].z, vPOINT[1].z) + 0.3f) && vRESULT.z > (min(vPOINT[0].z, vPOINT[1].z) - 0.3f))
				{
					VECLPOINT::iterator itPT;
					for(itPT = vEDGE->begin(); itPT != vEDGE->end(); itPT++)
					{
						if((*itPT).m_fPosX == point.m_fPosX &&
						   (*itPT).m_fPosY == point.m_fPosY &&
						   (*itPT).m_fPosZ == point.m_fPosZ)
						   break;
					}
					if(itPT == vEDGE->end() )	
						vEDGE->push_back(point);
				}
		}

		if(D3DXPlaneIntersectLine(&vRESULT, &plane, &vPOINT[1], &vPOINT[2]) != NULL)
		{
			point.m_fPosX = vRESULT.x;
			point.m_fPosY = vRESULT.y + 50.0f;
			point.m_fPosZ = vRESULT.z;
			point.m_dwColor = 0xFFFF0000;

			if(vRESULT.x < (max(vPOINT[1].x, vPOINT[2].x) + 0.3f) && vRESULT.x > (min(vPOINT[1].x, vPOINT[2].x) - 0.3f) )
				if(vRESULT.z <= (max(vPOINT[1].z, vPOINT[2].z) + 0.3f) && vRESULT.z > (min(vPOINT[1].z, vPOINT[2].z) - 0.3f))
				{
					VECLPOINT::iterator itPT;
					for(itPT = vEDGE->begin(); itPT != vEDGE->end(); itPT++)
					{
						if((*itPT).m_fPosX == point.m_fPosX &&
							(*itPT).m_fPosY == point.m_fPosY &&
							(*itPT).m_fPosZ == point.m_fPosZ)
							break;
					}
					if(itPT == vEDGE->end() )	
						vEDGE->push_back(point);
				}
		}
		
		if(D3DXPlaneIntersectLine(&vRESULT, &plane, &vPOINT[2], &vPOINT[0]) != NULL)
		{
			point.m_fPosX = vRESULT.x;
			point.m_fPosY = vRESULT.y + 50.0f;
			point.m_fPosZ = vRESULT.z;
			point.m_dwColor = 0xFFFF0000;
			
			if(vRESULT.x < (max(vPOINT[0].x, vPOINT[2].x) + 0.3f) && vRESULT.x > (min(vPOINT[0].x, vPOINT[2].x) - 0.3f) )
				if(vRESULT.z <= (max(vPOINT[0].z, vPOINT[2].z) + 0.3f) && vRESULT.z > (min(vPOINT[0].z, vPOINT[2].z) - 0.3f))
				{
					VECLPOINT::iterator itPT;
					for(itPT = vEDGE->begin(); itPT != vEDGE->end(); itPT++)
					{
						if((*itPT).m_fPosX == point.m_fPosX &&
							(*itPT).m_fPosY == point.m_fPosY &&
							(*itPT).m_fPosZ == point.m_fPosZ)
							break;
					}
					if(itPT == vEDGE->end() )	
						vEDGE->push_back(point);
				}
		}
	}

	vCOLPOINT->clear();
	delete vCOLPOINT;
	vCOLPOINT = NULL;
	
	if(!vEDGE->empty())
	{
		VECLPOINT::iterator itPT = vEDGE->begin();

		FLOAT fMaxPosX = (*itPT).m_fPosX;
		FLOAT fMinPosX = (*itPT).m_fPosX;
		FLOAT fMaxPosZ = (*itPT).m_fPosZ;
		FLOAT fMinPosZ = (*itPT).m_fPosZ;

		for(itPT = vEDGE->begin(); itPT != vEDGE->end(); itPT++)
		{
			fMaxPosX = max((*itPT).m_fPosX, fMaxPosX);
			fMinPosX = min((*itPT).m_fPosX, fMinPosX);
			fMaxPosZ = max((*itPT).m_fPosZ, fMaxPosZ);
			fMinPosZ = min((*itPT).m_fPosZ, fMinPosZ);
		}

		LPOINT ptCenter;

		ptCenter.m_fPosX = ((fMaxPosX - fMinPosX) / 2) + fMinPosX;
		ptCenter.m_fPosY = 160.0f;
		ptCenter.m_fPosZ = ((fMaxPosZ - fMinPosZ) / 2) + fMinPosZ;

		MAPFLOAT mapAngle;

		D3DXVECTOR3 vA(0.0f, 0.0f, 1.0f);

		/*for(i=0; i<vHigh.size() - 1; i++)
		{
			D3DXVECTOR3 vA(ptCenter.m_fPosX,	ptCenter.m_fPosY,	ptCenter.m_fPosZ);
			D3DXVECTOR3 vB(vHigh[i].m_fPosX,	vHigh[i].m_fPosY,	vHigh[i].m_fPosZ);
			D3DXVECTOR3 vC(vHigh[i+1].m_fPosX,	vHigh[i+1].m_fPosY, vHigh[i+1].m_fPosZ);

			D3DXVECTOR3 vVEC1, vVEC2;
			D3DXVec3Subtract(&vVEC1, &vB, &vA);
			D3DXVec3Subtract(&vVEC2, &vC, &vA);

			D3DXVec3Normalize(&vVEC1, &vVEC1);
			D3DXVec3Normalize(&vVEC2, &vVEC2);

			FLOAT  theta = ((acos(D3DXVec3Dot(&vVEC1, &vVEC2))) / D3DX_PI ) * 180.0f;
			if(theta > 90.0f)
			{
				LPOINT pt = vHigh[i];
				vHigh[i]	= vHigh[i+1];
				vHigh[i+1]  = pt;
			}
		}*/

		/*VECLPOINT vHigh;
		VECLPOINT vLow;

		for(itPT = vEDGE->begin(); itPT != vEDGE->end(); itPT++)
		{
			LPOINT pt = (*itPT);
			if((*itPT).m_fPosZ >= ptCenter.m_fPosZ)
				vHigh.push_back(pt);
			else
				vLow.push_back(pt);
		}

		for(DWORD i=0; i<vHigh.size(); i++)
		{
			for(DWORD j=0; j<(vHigh.size() - i) - 1; j++)
			{
				LPOINT pt1 = vHigh[j];
				if(vHigh[j].m_fPosX > vHigh[j+1].m_fPosX)
				{
					LPOINT pt = vHigh[j];

					vHigh[j]	= vHigh[j+1];
					vHigh[j+1]	= pt;
				}
				else if(vHigh[j].m_fPosX == vHigh[j+1].m_fPosX)
				{
					if(vHigh[j].m_fPosX < ptCenter.m_fPosX)
					{
						if(vHigh[j].m_fPosZ > vHigh[j+1].m_fPosZ)
						{
							LPOINT pt = vHigh[j];

							vHigh[j]	= vHigh[j+1];
							vHigh[j+1]	= pt;
						}
					}
					else
					{
						if(vHigh[j].m_fPosZ < vHigh[j+1].m_fPosZ)
						{
							LPOINT pt = vHigh[j];

							vHigh[j]	= vHigh[j+1];
							vHigh[j+1]	= pt;
						}
					}
				}
			}
		}

		for(i=0; i<vLow.size(); i++)
		{
			for(DWORD j=0; j<(vLow.size() - i) - 1; j++)
			{
				LPOINT pt1 = vLow[j];
				if(vLow[j].m_fPosX > vLow[j+1].m_fPosX)
				{
					LPOINT pt = vLow[j];

					vLow[j]		= vLow[j+1];
					vLow[j+1]	= pt;
				}
				else if(vLow[j].m_fPosX == vLow[j+1].m_fPosX)
				{
					if(vLow[j].m_fPosX < ptCenter.m_fPosX)
					{
						if(vLow[j].m_fPosZ < vLow[j+1].m_fPosZ)
						{
							LPOINT pt = vLow[j];

							vLow[j]		= vLow[j+1];
							vLow[j+1]	= pt;
						}
					}
					else
					{
						if(vLow[j].m_fPosZ > vLow[j+1].m_fPosZ)
						{
							LPOINT pt = vLow[j];

							vLow[j]		= vLow[j+1];
							vLow[j+1]	= pt;
						}
					}
				}
			}
		}*/

		/*for(i=0; i<vHigh.size() - 1; i++)
		{
			D3DXVECTOR3 vA(ptCenter.m_fPosX,	ptCenter.m_fPosY,	ptCenter.m_fPosZ);
			D3DXVECTOR3 vB(vHigh[i].m_fPosX,	vHigh[i].m_fPosY,	vHigh[i].m_fPosZ);
			D3DXVECTOR3 vC(vHigh[i+1].m_fPosX,	vHigh[i+1].m_fPosY, vHigh[i+1].m_fPosZ);

			D3DXVECTOR3 vVEC1, vVEC2;
			D3DXVec3Subtract(&vVEC1, &vB, &vA);
			D3DXVec3Subtract(&vVEC2, &vC, &vA);

			D3DXVec3Normalize(&vVEC1, &vVEC1);
			D3DXVec3Normalize(&vVEC2, &vVEC2);

			FLOAT  theta = ((acos(D3DXVec3Dot(&vVEC1, &vVEC2))) / D3DX_PI ) * 180.0f;
			if(theta > 90.0f)
			{
				LPOINT pt = vHigh[i];
				vHigh[i]	= vHigh[i+1];
				vHigh[i+1]  = pt;
			}
		}
		for(i=0; i<vLow.size(); i++)
		{

		}*/

		/*vEDGE->clear();
		vEDGE->push_back(ptCenter);
		for(i=0; i<vHigh.size(); i++)
			vEDGE->push_back(vHigh[i]);

		for(i=DWORD(vLow.size()); i>0; i--)
			vEDGE->push_back(vLow[i-1]);

		vEDGE->push_back(vHigh[0]);

		vHigh.clear();
		vLow.clear();*/
	}

	return vEDGE;
}
