// MAPDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "MAPFrame.h"
#include "MAPView.h"
#include "MAPDoc.h"

#define UNDOBUF_SIZE			50


// CMAPDoc

IMPLEMENT_DYNCREATE( CMAPDoc, CGridDoc)

CMAPDoc::CMAPDoc()
{
	m_bPushUndoBUF = TRUE;
	m_bInsertLIGHT = TRUE;
	m_bInsertOBJ = TRUE;
	m_bInsertSFX = TRUE;
	m_bInsertSND = TRUE;

	m_bEditLIGHT = FALSE;
	m_bEditOBJ = FALSE;
	m_bEditSFX = FALSE;
	m_bEditSND = FALSE;

	m_vSELOBJBUF.clear();
	m_vSELLIGHT.clear();
	m_vSELOBJ.clear();
	m_vSELSFX.clear();
	m_vSELSND.clear();
	m_vUNDOBUF.clear();

	for( int i=0; i<10; i++)
		m_vGROUPOBJ[i].clear();

	m_mapFREEZE.clear();
	m_mapHIDE.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapSND.clear();

	m_nUndoIndex = -1;
	m_pMAP = NULL;
}

BOOL CMAPDoc::OnNewDocument()
{
	if (!CGridDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CMAPDoc::~CMAPDoc()
{
	MAPOBJECT::iterator itOBJ;
	MAPDWORD::iterator itSFX;
	MAPDWORD::iterator itSND;

	for( itSND = m_mapSND.begin(); itSND != m_mapSND.end(); itSND++)
	{
		MAPMAPSND::iterator finder = m_pMAP->m_pMAP->m_mapSND.find((*itSND).first);

		if( finder != m_pMAP->m_pMAP->m_mapSND.end() )
		{
			CD3DSound *pSND = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID);

			CMainFrame::m_TachyonMedia.Stop(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID,
				(*itSND).second);

			pSND->Unlock((*itSND).second);
		}
	}

	for( itOBJ = m_mapOBJ.begin(); itOBJ != m_mapOBJ.end(); itOBJ++)
		delete (*itOBJ).second;

	for( itSFX = m_mapSFX.begin(); itSFX != m_mapSFX.end(); itSFX++)
		CTachyonSFX::Release((*itSFX).second);

	while(!m_vUNDOBUF.empty())
	{
		delete m_vUNDOBUF.back();
		m_vUNDOBUF.pop_back();
	}

	for( int i = 0 ; i < 10 ; i++ )
		m_vGROUPOBJ[i].clear();

	m_vSELOBJBUF.clear();
	m_vSELLIGHT.clear();
	m_vSELOBJ.clear();
	m_vSELSFX.clear();
	m_vSELSND.clear();

	m_mapFREEZE.clear();
	m_mapHIDE.clear();
	m_mapOBJ.clear();
	m_mapSFX.clear();
	m_mapSND.clear();
	RemoveSFX();
}


BEGIN_MESSAGE_MAP( CMAPDoc, CGridDoc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
END_MESSAGE_MAP()


// CMAPDoc 진단입니다.

#ifdef _DEBUG
void CMAPDoc::AssertValid() const
{
	CGridDoc::AssertValid();
}

void CMAPDoc::Dump(CDumpContext& dc) const
{
	CGridDoc::Dump(dc);
}
#endif //_DEBUG


// CMAPDoc serialization입니다.

void CMAPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// CMAPDoc 명령입니다.

CMAPLIGHTBar *CMAPDoc::GetMAPLIGHTBar()
{
	CMAPFrame *pFrame = (CMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndMAPLIGHTDlg : NULL;
}

CMAPOBJBar *CMAPDoc::GetMAPOBJBar()
{
	CMAPFrame *pFrame = (CMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndMAPOBJDlg : NULL;
}

CMAPSFXBar *CMAPDoc::GetMAPSFXBar()
{
	CMAPFrame *pFrame = (CMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndMAPSFXDlg : NULL;
}

CMAPSNDBar *CMAPDoc::GetMAPSNDBar()
{
	CMAPFrame *pFrame = (CMAPFrame *) GetFrameWnd();
	return pFrame ? &pFrame->m_wndMAPSNDDlg : NULL;
}

CPlayCtrl *CMAPDoc::GetPlayCtrl()
{
	CMAPFrame *pFrame = (CMAPFrame *) GetFrameWnd();
	return pFrame ? pFrame->m_wndPlayCtrlBar.GetPlayCtrl() : NULL;
}

void CMAPDoc::Update()
{
	if(m_pMAP)
	{
		CMAPLIGHTBar *pLIGHTBar = GetMAPLIGHTBar();
		CMAPOBJBar *pOBJBar = GetMAPOBJBar();
		CMAPSFXBar *pSFXBar = GetMAPSFXBar();
		CMAPSNDBar *pSNDBar = GetMAPSNDBar();

		if(pLIGHTBar)
			pLIGHTBar->SetItem(m_pMAP);

		if(pOBJBar)
			pOBJBar->SetItem(m_pMAP);

		if(pSFXBar)
			pSFXBar->SetItem(m_pMAP);

		if(pSNDBar)
			pSNDBar->SetItem(m_pMAP);
	}
}

void CMAPDoc::RedrawAllView()
{
	if(!m_bAutoDelete)
		return;

	POSITION pos = GetFirstViewPosition();
	while(pos)
	{
		CView *pView = GetNextView(pos);

		if( pView && pView->IsKindOf(RUNTIME_CLASS(CMAPView)) )
			((CMAPView *) pView)->Render();
	}
}

BOOL CMAPDoc::IsSelLIGHT( DWORD dwID)
{
	int nCount = INT(m_vSELLIGHT.size());

	for( int i=0; i<nCount; i++)
		if( m_vSELLIGHT[i] == dwID )
			return TRUE;

	return FALSE;
}

BOOL CMAPDoc::IsSelOBJ( DWORD dwID)
{
	int nCount = INT(m_vSELOBJ.size());

	for( int i=0; i<nCount; i++)
		if( m_vSELOBJ[i] == dwID )
			return TRUE;

	nCount = INT(m_vSELOBJBUF.size());
	for( i=0; i<nCount; i++)
		if( m_vSELOBJBUF[i] == dwID )
			return TRUE;

	return FALSE;
}

BOOL CMAPDoc::IsSelSFX( DWORD dwID)
{
	int nCount = INT(m_vSELSFX.size());

	for( int i=0; i<nCount; i++)
		if( m_vSELSFX[i] == dwID )
			return TRUE;

	return FALSE;
}

BOOL CMAPDoc::IsSelSND( DWORD dwID)
{
	int nCount = INT(m_vSELSND.size());

	for( int i=0; i<nCount; i++)
		if( m_vSELSND[i] == dwID )
			return TRUE;

	return FALSE;
}

void CMAPDoc::UpdateLIGHTBAR()
{
	if(m_pMAP)
	{
		CMAPLIGHTBar *pControl = GetMAPLIGHTBar();
		pControl->UpdateItem();
	}
}

void CMAPDoc::UpdateOBJBAR()
{
	if(m_pMAP)
	{
		CMAPOBJBar *pControl = GetMAPOBJBar();
		MAPDWORD::iterator it;
		MAPDWORD mapSELOBJ;
		mapSELOBJ.clear();

		for( int i=0; i<INT(m_vSELOBJBUF.size()); i++)
			if( mapSELOBJ.find(m_vSELOBJBUF[i]) == mapSELOBJ.end() )
				mapSELOBJ.insert( MAPDWORD::value_type( m_vSELOBJBUF[i], m_vSELOBJBUF[i]));

		for( i=0; i<INT(m_vSELOBJ.size()); i++)
			if( mapSELOBJ.find(m_vSELOBJ[i]) == mapSELOBJ.end() )
				mapSELOBJ.insert( MAPDWORD::value_type( m_vSELOBJ[i], m_vSELOBJ[i]));

		m_vSELOBJBUF.clear();
		m_vSELOBJ.clear();

		for( it = mapSELOBJ.begin(); it != mapSELOBJ.end(); it++)
			m_vSELOBJ.push_back((*it).first);
		mapSELOBJ.clear();

		pControl->UpdateItem();
	}
}

void CMAPDoc::UpdateSFXBAR()
{
	if(m_pMAP)
	{
		CMAPSFXBar *pControl = GetMAPSFXBar();
		pControl->UpdateItem();
	}
}

void CMAPDoc::UpdateSNDBAR()
{
	if(m_pMAP)
	{
		CMAPSNDBar *pControl = GetMAPSNDBar();
		pControl->UpdateItem();
	}
}

D3DXVECTOR3 CMAPDoc::GetSELSNDPOS()
{
	D3DXVECTOR3 vResult( 0.0f, 0.0f, 0.0f);
	int nCount = INT(m_vSELSND.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator finder = m_pMAP->m_pMAP->m_mapSND.find(m_vSELSND[i]);
		vResult += (*finder).second->m_vPOS;
	}

	if( nCount > 0 )
		vResult /= (FLOAT) nCount;

	return vResult;
}

D3DXVECTOR3 CMAPDoc::GetSELSFXPOS()
{
	D3DXVECTOR3 vResult( 0.0f, 0.0f, 0.0f);
	int nCount = INT(m_vSELSFX.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator finder = m_pMAP->m_pMAP->m_mapSFX.find(m_vSELSFX[i]);
		vResult += (*finder).second->m_vPOS;
	}

	if( nCount > 0 )
		vResult /= (FLOAT) nCount;

	return vResult;
}

D3DXVECTOR3 CMAPDoc::GetSELSFXROT()
{
	if( INT(m_vSELSFX.size()) == 1 )
	{
		MAPMAPSFX::iterator finder = m_pMAP->m_pMAP->m_mapSFX.find(m_vSELSFX[0]);
		return (*finder).second->m_vROT;
	}

	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}

D3DXVECTOR3 CMAPDoc::GetSELOBJSCALE()
{
	if( INT(m_vSELOBJ.size()) == 1 )
	{
		MAPMAPOBJ::iterator finder = m_pMAP->m_pMAP->m_mapOBJ.find(m_vSELOBJ[0]);
		return (*finder).second->m_vSCALE;
	}

	return D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
}

D3DXVECTOR3 CMAPDoc::GetSELOBJPOS()
{
	D3DXVECTOR3 vResult( 0.0f, 0.0f, 0.0f);
	int nCount = INT(m_vSELOBJ.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator finder = m_pMAP->m_pMAP->m_mapOBJ.find(m_vSELOBJ[i]);
		vResult += (*finder).second->m_vPOS;
	}

	if( nCount > 0 )
		vResult /= (FLOAT) nCount;

	return vResult;
}

D3DXVECTOR3 CMAPDoc::GetSELOBJROT()
{
	if( INT(m_vSELOBJ.size()) == 1 )
	{
		MAPMAPOBJ::iterator finder = m_pMAP->m_pMAP->m_mapOBJ.find(m_vSELOBJ[0]);
		return (*finder).second->m_vROT;
	}

	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}

D3DXVECTOR3 CMAPDoc::GetSELLIGHTPOS()
{
	D3DXVECTOR3 vResult( 0.0f, 0.0f, 0.0f);
	int nCount = INT(m_vSELLIGHT.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPLIGHT::iterator finder = m_pMAP->m_mapLIGHT.find(m_vSELLIGHT[i]);
		vResult += (*finder).second->m_vPOS;
	}

	if( nCount > 0 )
		vResult /= (FLOAT) nCount;

	return vResult;
}

void CMAPDoc::ClearSELOBJ()
{
	CMAPOBJBar *pControl = GetMAPOBJBar();
	pControl->UpdateData();

	int nCount = INT(m_vSELOBJ.size());

	D3DXVECTOR3 vSCALE = GetSELOBJSCALE();
	D3DXVECTOR3 vPOS = GetSELOBJPOS();
	D3DXVECTOR3 vROT = GetSELOBJROT();

	vSCALE.x = ((FLOAT) atof(pControl->m_strScaleX)) - vSCALE.x;
	vSCALE.y = ((FLOAT) atof(pControl->m_strScaleY)) - vSCALE.y;
	vSCALE.z = ((FLOAT) atof(pControl->m_strScaleZ)) - vSCALE.z;

	vPOS.x = ((FLOAT) atof(pControl->m_strPosX)) - vPOS.x;
	vPOS.y = ((FLOAT) atof(pControl->m_strPosY)) - vPOS.y;
	vPOS.z = ((FLOAT) atof(pControl->m_strPosZ)) - vPOS.z;

	vROT.x = ((FLOAT) atof(pControl->m_strRotX)) * D3DX_PI / 180.0f - vROT.x;
	vROT.y = ((FLOAT) atof(pControl->m_strRotY)) * D3DX_PI / 180.0f - vROT.y;
	vROT.z = ((FLOAT) atof(pControl->m_strRotZ)) * D3DX_PI / 180.0f - vROT.z;

	if(m_bPushUndoBUF)
		PushUndoBUF( vSCALE, vPOS, vROT, BT_OBJ);

	for( int i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find(m_vSELOBJ[i]);
		LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

		pMAPOBJ->m_vSCALE += vSCALE;
		pMAPOBJ->m_vROT += vROT;
		pMAPOBJ->m_vPOS += vPOS;

		MAPOBJECT::iterator itOBJECT = m_mapOBJ.find( m_vSELOBJ[i] );
		if( itOBJECT != m_mapOBJ.end() )
			(*itOBJECT).second->m_bSelAlpha = 0xFF;
	}

	pControl->m_strScaleX.Format( "%.2f", 1.0f);
	pControl->m_strScaleY.Format( "%.2f", 1.0f);
	pControl->m_strScaleZ.Format( "%.2f", 1.0f);
	pControl->m_strPosX.Format( "%.4f", 0.0f);
	pControl->m_strPosY.Format( "%.4f", 0.0f);
	pControl->m_strPosZ.Format( "%.4f", 0.0f);
	pControl->m_strRotX.Format( "%.2f", 0.0f);
	pControl->m_strRotY.Format( "%.2f", 0.0f);
	pControl->m_strRotZ.Format( "%.2f", 0.0f);
	pControl->UpdateData(FALSE);

	m_vSELOBJ.clear();
}

void CMAPDoc::ClearSELSFX()
{
	CMAPSFXBar *pControl = GetMAPSFXBar();
	pControl->UpdateData();

	int nCount = INT(m_vSELSFX.size());

	D3DXVECTOR3 vPOS = GetSELSFXPOS();
	D3DXVECTOR3 vROT = GetSELSFXROT();

	vPOS.x = ((FLOAT) atof(pControl->m_strPosX)) - vPOS.x;
	vPOS.y = ((FLOAT) atof(pControl->m_strPosY)) - vPOS.y;
	vPOS.z = ((FLOAT) atof(pControl->m_strPosZ)) - vPOS.z;

	vROT.x = ((FLOAT) atof(pControl->m_strRotX)) * D3DX_PI / 180.0f - vROT.x;
	vROT.y = ((FLOAT) atof(pControl->m_strRotY)) * D3DX_PI / 180.0f - vROT.y;
	vROT.z = ((FLOAT) atof(pControl->m_strRotZ)) * D3DX_PI / 180.0f - vROT.z;

	if(m_bPushUndoBUF)
		PushUndoBUF( D3DXVECTOR3( 0.0f, 0.0f, 0.0f), vPOS, vROT, BT_SFX);

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator itMAPSFX = m_pMAP->m_pMAP->m_mapSFX.find(m_vSELSFX[i]);
		LPMAPSFX pMAPSFX = (*itMAPSFX).second;

		pMAPSFX->m_vROT += vROT;
		pMAPSFX->m_vPOS += vPOS;
	}

	pControl->m_strPosX.Format( "%.2f", 0.0f);
	pControl->m_strPosY.Format( "%.2f", 0.0f);
	pControl->m_strPosZ.Format( "%.2f", 0.0f);
	pControl->m_strRotX.Format( "%.2f", 0.0f);
	pControl->m_strRotY.Format( "%.2f", 0.0f);
	pControl->m_strRotZ.Format( "%.2f", 0.0f);
	pControl->m_strName.Empty();
	pControl->UpdateData(FALSE);

	m_vSELSFX.clear();
}

void CMAPDoc::ClearSELSND()
{
	CMAPSNDBar *pControl = GetMAPSNDBar();
	pControl->UpdateData();

	int nCount = INT(m_vSELSND.size());
	D3DXVECTOR3 vPOS = GetSELSNDPOS();

	vPOS.x = ((FLOAT) atof(pControl->m_strPosX)) - vPOS.x;
	vPOS.y = ((FLOAT) atof(pControl->m_strPosY)) - vPOS.y;
	vPOS.z = ((FLOAT) atof(pControl->m_strPosZ)) - vPOS.z;

	if(m_bPushUndoBUF)
		PushUndoBUF( D3DXVECTOR3( 0.0f, 0.0f, 0.0f), vPOS, D3DXVECTOR3( 0.0f, 0.0f, 0.0f), BT_SND);

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator itMAPSND = m_pMAP->m_pMAP->m_mapSND.find(m_vSELSND[i]);
		LPMAPSND pMAPSND = (*itMAPSND).second;

		pMAPSND->m_vPOS += vPOS;
	}

	pControl->m_strPosX.Format( "%.2f", 0.0f);
	pControl->m_strPosY.Format( "%.2f", 0.0f);
	pControl->m_strPosZ.Format( "%.2f", 0.0f);
	pControl->m_strName.Empty();
	pControl->UpdateData(FALSE);

	m_vSELSND.clear();
}

void CMAPDoc::ClearSELLIGHT()
{
	CMAPLIGHTBar *pControl = GetMAPLIGHTBar();
	pControl->UpdateData();

	int nCount = INT(m_vSELLIGHT.size());
	D3DXVECTOR3 vPOS = GetSELLIGHTPOS();

	vPOS.x = ((FLOAT) atof(pControl->m_strPosX)) - vPOS.x;
	vPOS.y = ((FLOAT) atof(pControl->m_strPosY)) - vPOS.y;
	vPOS.z = ((FLOAT) atof(pControl->m_strPosZ)) - vPOS.z;

	for( int i=0; i<nCount; i++)
	{
		MAPMAPLIGHT::iterator finder = m_pMAP->m_mapLIGHT.find(m_vSELLIGHT[i]);
		(*finder).second->m_vPOS += vPOS;
	}

	pControl->m_strPosX.Format( "%.2f", 0.0f);
	pControl->m_strPosY.Format( "%.2f", 0.0f);
	pControl->m_strPosZ.Format( "%.2f", 0.0f);
	pControl->UpdateData(FALSE);

	m_vSELLIGHT.clear();
}

void CMAPDoc::RotateOBJ( D3DXVECTOR3 vROT)
{
	CMAPOBJBar *pControl = GetMAPOBJBar();

	vROT.x += ((FLOAT) atof(pControl->m_strRotX)) * D3DX_PI / 180.0f;
	vROT.y += ((FLOAT) atof(pControl->m_strRotY)) * D3DX_PI / 180.0f;
	vROT.z += ((FLOAT) atof(pControl->m_strRotZ)) * D3DX_PI / 180.0f;

	pControl->m_strRotX.Format( "%.2f", vROT.x * 180.0f / D3DX_PI);
	pControl->m_strRotY.Format( "%.2f", vROT.y * 180.0f / D3DX_PI);
	pControl->m_strRotZ.Format( "%.2f", vROT.z * 180.0f / D3DX_PI);

	pControl->UpdateData(FALSE);
	pControl->OnEnChangeEditData();
}

void CMAPDoc::RotateSFX( D3DXVECTOR3 vROT)
{
	CMAPSFXBar *pControl = GetMAPSFXBar();

	vROT.x += ((FLOAT) atof(pControl->m_strRotX)) * D3DX_PI / 180.0f;
	vROT.y += ((FLOAT) atof(pControl->m_strRotY)) * D3DX_PI / 180.0f;
	vROT.z += ((FLOAT) atof(pControl->m_strRotZ)) * D3DX_PI / 180.0f;

	pControl->m_strRotX.Format( "%.2f", vROT.x * 180.0f / D3DX_PI);
	pControl->m_strRotY.Format( "%.2f", vROT.y * 180.0f / D3DX_PI);
	pControl->m_strRotZ.Format( "%.2f", vROT.z * 180.0f / D3DX_PI);

	pControl->UpdateData(FALSE);
	pControl->OnEnChangeEditData();
}

void CMAPDoc::RotateLIGHT( D3DXVECTOR3 vROT)
{
}

void CMAPDoc::DeleteLIGHT()
{
}

void CMAPDoc::UpdateLIGHT()
{
}

void CMAPDoc::MoveLIGHT( D3DXVECTOR3 vMOVE)
{
}

void CMAPDoc::MoveOBJ( D3DXVECTOR3 vMOVE)
{
	CMAPOBJBar *pControl = GetMAPOBJBar();

	vMOVE.x += (FLOAT) atof(pControl->m_strPosX);
	vMOVE.y += (FLOAT) atof(pControl->m_strPosY);
	vMOVE.z += (FLOAT) atof(pControl->m_strPosZ);

	pControl->m_strPosX.Format( "%.4f", vMOVE.x);
	pControl->m_strPosY.Format( "%.4f", vMOVE.y);
	pControl->m_strPosZ.Format( "%.4f", vMOVE.z);

	pControl->UpdateData(FALSE);
	pControl->OnEnChangeEditData();
}

void CMAPDoc::MoveSFX( D3DXVECTOR3 vMOVE)
{
	CMAPSFXBar *pControl = GetMAPSFXBar();

	vMOVE.x += (FLOAT) atof(pControl->m_strPosX);
	vMOVE.y += (FLOAT) atof(pControl->m_strPosY);
	vMOVE.z += (FLOAT) atof(pControl->m_strPosZ);

	pControl->m_strPosX.Format( "%.2f", vMOVE.x);
	pControl->m_strPosY.Format( "%.2f", vMOVE.y);
	pControl->m_strPosZ.Format( "%.2f", vMOVE.z);

	pControl->UpdateData(FALSE);
	pControl->OnEnChangeEditData();
}

void CMAPDoc::MoveSND( D3DXVECTOR3 vMOVE)
{
	CMAPSNDBar *pControl = GetMAPSNDBar();

	vMOVE.x += (FLOAT) atof(pControl->m_strPosX);
	vMOVE.y += (FLOAT) atof(pControl->m_strPosY);
	vMOVE.z += (FLOAT) atof(pControl->m_strPosZ);

	pControl->m_strPosX.Format( "%.2f", vMOVE.x);
	pControl->m_strPosY.Format( "%.2f", vMOVE.y);
	pControl->m_strPosZ.Format( "%.2f", vMOVE.z);

	pControl->UpdateData(FALSE);
	pControl->OnEnChangeEditData();
}

BOOL CMAPDoc::GetWorldCenter( LPD3DXVECTOR3 pResult)
{
	(*pResult) = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	if(!m_vSELLIGHT.empty())
	{
		CMAPLIGHTBar *pControl = GetMAPLIGHTBar();

		pResult->x = (FLOAT) atof(pControl->m_strPosX);
		pResult->y = (FLOAT) atof(pControl->m_strPosY);
		pResult->z = (FLOAT) atof(pControl->m_strPosZ);

		return TRUE;
	}

	if(!m_vSELOBJ.empty())
	{
		CMAPOBJBar *pControl = GetMAPOBJBar();

		pResult->x = (FLOAT) atof(pControl->m_strPosX);
		pResult->y = (FLOAT) atof(pControl->m_strPosY);
		pResult->z = (FLOAT) atof(pControl->m_strPosZ);

		return TRUE;
	}

	if(!m_vSELSFX.empty())
	{
		CMAPSFXBar *pControl = GetMAPSFXBar();

		pResult->x = (FLOAT) atof(pControl->m_strPosX);
		pResult->y = (FLOAT) atof(pControl->m_strPosY);
		pResult->z = (FLOAT) atof(pControl->m_strPosZ);

		return TRUE;
	}

	if(!m_vSELSND.empty())
	{
		CMAPSNDBar *pControl = GetMAPSNDBar();

		pResult->x = (FLOAT) atof(pControl->m_strPosX);
		pResult->y = (FLOAT) atof(pControl->m_strPosY);
		pResult->z = (FLOAT) atof(pControl->m_strPosZ);

		return TRUE;
	}

	return FALSE;
}

void CMAPDoc::CalcSFX( DWORD dwTick)
{
	MAPTSFX::iterator it = CTachyonSFX::m_mapINST.begin();
	CMAPOBJBar *pControl = GetMAPOBJBar();

	while(it != CTachyonSFX::m_mapINST.end())
	{
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if( (*it).second->m_pSFX && (*it).second->m_pHost == pControl )
			if(!(*it).second->IsDead())
				(*it).second->m_dwInitTick = dwTick;
			else if(!(*it).second->m_bHasHost)
				CTachyonSFX::Release((*it).first);

		it = itNEXT;
	}
}

void CMAPDoc::RemoveSFX()
{
	MAPTSFX::iterator it = CTachyonSFX::m_mapINST.begin();
	CMAPOBJBar *pControl = GetMAPOBJBar();

	while(it != CTachyonSFX::m_mapINST.end())
	{
		MAPTSFX::iterator itNEXT = it;
		itNEXT++;

		if( (*it).second->m_pHost == pControl &&
			!(*it).second->m_bHasHost )
			CTachyonSFX::Release((*it).first);

		it = itNEXT;
	}
}

void CMAPDoc::CalcMAPTick( DWORD dwTick)
{
}

DWORD CMAPDoc::GetNewLIGHTID()
{
	DWORD dwID = 1;

	while( m_pMAP->m_mapLIGHT.find(dwID) != m_pMAP->m_mapLIGHT.end() )
		dwID++;

	return dwID;
}

DWORD CMAPDoc::GetNewOBJID()
{
	DWORD dwID = 1;

	while( m_mapOBJ.find(dwID) != m_mapOBJ.end() )
		dwID++;

	return dwID;
}

DWORD CMAPDoc::GetNewSFXID()
{
	DWORD dwID = 1;

	while( m_pMAP->m_mapSFX.find(dwID) != m_pMAP->m_mapSFX.end() )
		dwID++;

	return dwID;
}

DWORD CMAPDoc::GetNewSNDID()
{
	DWORD dwID = 1;

	while( m_pMAP->m_pMAP->m_mapSND.find(dwID) != m_pMAP->m_pMAP->m_mapSND.end() )
		dwID++;

	return dwID;
}

void CMAPDoc::AddLIGHT( LPD3DXVECTOR3 pPOS)
{
}

void CMAPDoc::CopyLIGHT( D3DXVECTOR3 vPOS)
{
}

void CMAPDoc::CopyOBJ( D3DXVECTOR3 vPOS)
{
	int nCount = INT(m_vSELOBJ.size());
    CMAPOBJBar* pOBJBar = GetMAPOBJBar();
	VECTORDWORD vCOPYOBJ;
	vCOPYOBJ.clear();

	pOBJBar->ResetSELOBJState();						// PSY 06/08/01 Copy Obj Select (Select OBJ OBJBar Pos Text Edit)
	pOBJBar->UpdateData(FALSE);

	for( int i=0; i<nCount; i++)
	{
		CTachyonObject *pOBJ = new CTachyonObject();
		LPMAPOBJ pMAPOBJ = new MAPOBJ();
		
		MAPMAPOBJ::iterator itOBJ = m_pMAP->m_pMAP->m_mapOBJ.find(m_vSELOBJ[i]);
		MAPDWORD::iterator itID = m_pMAP->m_mapOBJ.find(m_vSELOBJ[i]);
		MAPCLKINST::iterator itCI;

		COBJItem *pITEM = CItemBase::GetOBJItem((*itID).second);
		DWORD dwID = GetNewOBJID();
		
		m_pMAP->m_mapOBJ.insert( MAPDWORD::value_type( dwID, pITEM->m_dwID));
		pMAPOBJ->m_pOBJ = &pITEM->m_OBJ;

		for( itCI = (*itOBJ).second->m_mapCLK.begin(); itCI != (*itOBJ).second->m_mapCLK.end(); itCI++)
		{
			LPCLKINST pOBJCI = new CLKINST();
			LPCLKINST pCI = (*itCI).second;

			memcpy( pOBJCI, pCI, sizeof(CLKINST));
			pMAPOBJ->m_mapCLK.insert( MAPCLKINST::value_type( (*itCI).first, pOBJCI));
		}
		
		pMAPOBJ->m_dwOBJFuncID = (*itOBJ).second->m_dwOBJFuncID;
		pMAPOBJ->m_dwACTFuncID = (*itOBJ).second->m_dwACTFuncID;
		pMAPOBJ->m_dwANIFuncID = (*itOBJ).second->m_dwANIFuncID;

		pMAPOBJ->m_dwCurACT = (*itOBJ).second->m_dwCurACT;
		pMAPOBJ->m_dwCurANI = (*itOBJ).second->m_dwCurANI;

		pMAPOBJ->m_vSCALE = (*itOBJ).second->m_vSCALE;
		pMAPOBJ->m_vROT = (*itOBJ).second->m_vROT;
		pMAPOBJ->m_vPOS = (*itOBJ).second->m_vPOS + vPOS;

		pOBJ->InitOBJ( &CMainFrame::m_3DDevice, pMAPOBJ);
		pOBJ->m_pHost = pOBJBar;

		m_pMAP->m_pMAP->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
		m_mapOBJ.insert( MAPOBJECT::value_type( dwID, pOBJ));
		
		vCOPYOBJ.push_back(dwID);
	}
	m_vSELOBJ.clear();
	for( int i = 0 ; i < vCOPYOBJ.size() ; i++ )		// PSY 06/08/01 Copy Obj Select (OBJ Select)
		m_vSELOBJ.push_back( vCOPYOBJ[i] );

	pOBJBar->ResetSELOBJState();
	pOBJBar->UpdateData(FALSE);

	vCOPYOBJ.clear();
	RedrawAllView();
}

void CMAPDoc::CopySFX( D3DXVECTOR3 vPOS)
{
	int nCount = INT(m_vSELSFX.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator itSFX = m_pMAP->m_pMAP->m_mapSFX.find(m_vSELSFX[i]);
		MAPDWORD::iterator itID = m_pMAP->m_mapSFX.find(m_vSELSFX[i]);

		CSFXItem *pITEM = CItemBase::GetSFXItem((*itID).second);
		LPMAPSFX pMAPSFX = new MAPSFX();
		DWORD dwID = GetNewSFXID();

		m_pMAP->m_mapSFX.insert( MAPDWORD::value_type( dwID, pITEM->m_dwID));
		pMAPSFX->m_pSFX = &pITEM->m_SFX;
		pMAPSFX->m_vPOS = (*itSFX).second->m_vPOS + vPOS;
		pMAPSFX->m_vROT = (*itSFX).second->m_vROT;

		m_pMAP->m_pMAP->m_mapSFX.insert( MAPMAPSFX::value_type( dwID, pMAPSFX));
	}

	RedrawAllView();
}

void CMAPDoc::CopySND( D3DXVECTOR3 vPOS)
{
	int nCount = INT(m_vSELSND.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator itSND = m_pMAP->m_pMAP->m_mapSND.find(m_vSELSND[i]);

		LPMAPSND pMAPSND = new MAPSND();
		DWORD dwID = GetNewSNDID();

		pMAPSND->m_dwMediaID = (*itSND).second->m_dwMediaID;
		pMAPSND->m_vPOS = (*itSND).second->m_vPOS + vPOS;

		m_pMAP->m_pMAP->m_mapSND.insert( MAPMAPSND::value_type( dwID, pMAPSND));
	}

	RedrawAllView();
}

void CMAPDoc::PushUndoBUF( D3DXVECTOR3 vSCALE,
						   D3DXVECTOR3 vPOS,
						   D3DXVECTOR3 vROT,
						   BYTE bType)
{
	LPVECTORDWORD pINDEX = NULL;

	switch(bType)
	{
	case BT_OBJ	: pINDEX = &m_vSELOBJ; break;
	case BT_SFX	: pINDEX = &m_vSELSFX; break;
	case BT_SND	: pINDEX = &m_vSELSND; break;
	}

	if( pINDEX && !pINDEX->empty() && (
		vSCALE != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) ||
		vPOS != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) ||
		vROT != D3DXVECTOR3( 0.0f, 0.0f, 0.0f)) )
	{
		LPUNDOBUF pBUF = new UNDOBUF();

		while( INT(m_vUNDOBUF.size()) > m_nUndoIndex + 1 )
		{
			delete m_vUNDOBUF.back();
			m_vUNDOBUF.pop_back();
		}

		pBUF->m_vSCALE = vSCALE;
		pBUF->m_vPOS = vPOS;
		pBUF->m_vROT = vROT;

		for( int i=0; i<INT(pINDEX->size()); i++)
			pBUF->m_vID.push_back((*pINDEX)[i]);
		pBUF->m_bType = bType;

		m_vUNDOBUF.push_back(pBUF);
		m_nUndoIndex++;

		while( INT(m_vUNDOBUF.size()) > UNDOBUF_SIZE )
		{
			delete m_vUNDOBUF[0];

			m_vUNDOBUF.erase(m_vUNDOBUF.begin());
			m_nUndoIndex--;
		}
	}
}

void CMAPDoc::OnEditUndo()
{
	ClearSELOBJ();
	ClearSELSFX();
	ClearSELSND();

	if( m_nUndoIndex < 0 )
		return;

	switch(m_vUNDOBUF[m_nUndoIndex]->m_bType)
	{
	case BT_OBJ	: UndoOBJ(m_vUNDOBUF[m_nUndoIndex]); break;
	case BT_SFX	: UndoSFX(m_vUNDOBUF[m_nUndoIndex]); break;
	case BT_SND	: UndoSND(m_vUNDOBUF[m_nUndoIndex]); break;
	}
	m_nUndoIndex--;
}

void CMAPDoc::OnEditRedo()
{
	if( m_nUndoIndex + 1 >= INT(m_vUNDOBUF.size()) )
		return;

	m_bPushUndoBUF = FALSE;
	ClearSELOBJ();
	ClearSELSFX();
	ClearSELSND();
	m_bPushUndoBUF = TRUE;
	m_nUndoIndex++;

	switch(m_vUNDOBUF[m_nUndoIndex]->m_bType)
	{
	case BT_OBJ	: RedoOBJ(m_vUNDOBUF[m_nUndoIndex]); break;
	case BT_SFX	: RedoSFX(m_vUNDOBUF[m_nUndoIndex]); break;
	case BT_SND	: RedoSND(m_vUNDOBUF[m_nUndoIndex]); break;
	}
}

void CMAPDoc::OnUpdateEditRedo( CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_nUndoIndex + 1 < INT(m_vUNDOBUF.size()));
}

void CMAPDoc::UndoOBJ( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find(pBUF->m_vID[i]);

		if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
		{
			MAPOBJECT::iterator itOBJ = m_mapOBJ.find(pBUF->m_vID[i]);
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			pMAPOBJ->m_vSCALE -= pBUF->m_vSCALE;
			pMAPOBJ->m_vPOS -= pBUF->m_vPOS;
			pMAPOBJ->m_vROT -= pBUF->m_vROT;

			if( itOBJ != m_mapOBJ.end() )
			{
				CTachyonObject *pOBJ = (*itOBJ).second;

				D3DXMATRIX vScale;
				D3DXMATRIX vRotX;
				D3DXMATRIX vRotY;
				D3DXMATRIX vRotZ;
				D3DXMATRIX vPos;

				D3DXMatrixScaling(
					&vScale,
					pMAPOBJ->m_vSCALE.x,
					pMAPOBJ->m_vSCALE.y,
					pMAPOBJ->m_vSCALE.z);

				D3DXMatrixRotationX(
					&vRotX,
					pMAPOBJ->m_vROT.x);

				D3DXMatrixRotationY(
					&vRotY,
					pMAPOBJ->m_vROT.y);

				D3DXMatrixRotationZ(
					&vRotZ,
					pMAPOBJ->m_vROT.z);

				D3DXMatrixTranslation(
					&vPos,
					pMAPOBJ->m_vPOS.x,
					pMAPOBJ->m_vPOS.y,
					pMAPOBJ->m_vPOS.z);

				pOBJ->m_vWorld = vRotX * vRotY * vRotZ * vPos;
				pOBJ->m_vScale = vScale;
			}
		}
	}

	RedrawAllView();
}

void CMAPDoc::UndoSFX( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator itMAPSFX = m_pMAP->m_pMAP->m_mapSFX.find(pBUF->m_vID[i]);

		if( itMAPSFX != m_pMAP->m_pMAP->m_mapSFX.end() )
		{
			LPMAPSFX pMAPSFX = (*itMAPSFX).second;

			pMAPSFX->m_vROT -= pBUF->m_vROT;
			pMAPSFX->m_vPOS -= pBUF->m_vPOS;
		}
	}

	RedrawAllView();
}

void CMAPDoc::UndoSND( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator itMAPSND = m_pMAP->m_pMAP->m_mapSND.find(pBUF->m_vID[i]);

		if( itMAPSND != m_pMAP->m_pMAP->m_mapSND.end() )
		{
			LPMAPSND pMAPSND = (*itMAPSND).second;
			pMAPSND->m_vPOS -= pBUF->m_vPOS;
		}
	}

	RedrawAllView();
}

void CMAPDoc::RedoOBJ( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pMAP->m_pMAP->m_mapOBJ.find(pBUF->m_vID[i]);

		if( itMAPOBJ != m_pMAP->m_pMAP->m_mapOBJ.end() )
		{
			MAPOBJECT::iterator itOBJ = m_mapOBJ.find(pBUF->m_vID[i]);
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			pMAPOBJ->m_vSCALE += pBUF->m_vSCALE;
			pMAPOBJ->m_vPOS += pBUF->m_vPOS;
			pMAPOBJ->m_vROT += pBUF->m_vROT;

			if( itOBJ != m_mapOBJ.end() )
			{
				CTachyonObject *pOBJ = (*itOBJ).second;

				D3DXMATRIX vScale;
				D3DXMATRIX vRotX;
				D3DXMATRIX vRotY;
				D3DXMATRIX vRotZ;
				D3DXMATRIX vPos;

				D3DXMatrixScaling(
					&vScale,
					pMAPOBJ->m_vSCALE.x,
					pMAPOBJ->m_vSCALE.y,
					pMAPOBJ->m_vSCALE.z);

				D3DXMatrixRotationX(
					&vRotX,
					pMAPOBJ->m_vROT.x);
				
				D3DXMatrixRotationY(
					&vRotY,
					pMAPOBJ->m_vROT.y);

				D3DXMatrixRotationZ(
					&vRotZ,
					pMAPOBJ->m_vROT.z);

				D3DXMatrixTranslation(
					&vPos,
					pMAPOBJ->m_vPOS.x,
					pMAPOBJ->m_vPOS.y,
					pMAPOBJ->m_vPOS.z);

				pOBJ->m_vWorld = vRotX * vRotY * vRotZ * vPos;
				pOBJ->m_vScale = vScale;
			}
		}
	}

	RedrawAllView();
}

void CMAPDoc::RedoSFX( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator itMAPSFX = m_pMAP->m_pMAP->m_mapSFX.find(pBUF->m_vID[i]);

		if( itMAPSFX != m_pMAP->m_pMAP->m_mapSFX.end() )
		{
			LPMAPSFX pMAPSFX = (*itMAPSFX).second;

			pMAPSFX->m_vROT += pBUF->m_vROT;
			pMAPSFX->m_vPOS += pBUF->m_vPOS;
		}
	}

	RedrawAllView();
}

void CMAPDoc::RedoSND( LPUNDOBUF pBUF)
{
	int nCount = INT(pBUF->m_vID.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator itMAPSND = m_pMAP->m_pMAP->m_mapSND.find(pBUF->m_vID[i]);

		if( itMAPSND != m_pMAP->m_pMAP->m_mapSND.end() )
		{
			LPMAPSND pMAPSND = (*itMAPSND).second;
			pMAPSND->m_vPOS += pBUF->m_vPOS;
		}
	}

	RedrawAllView();
}

FLOAT CMAPDoc::GetHeight( FLOAT fPosX,
						  FLOAT fPosY,
						  FLOAT fPosZ)
{
	return 0.0f;
}

BOOL CMAPDoc::IsHideOBJ( DWORD dwID)
{
	return m_mapHIDE.find(dwID) != m_mapHIDE.end() ? TRUE : FALSE;
}

BOOL CMAPDoc::IsFreezeOBJ( DWORD dwID)
{
	return m_mapFREEZE.find(dwID) != m_mapFREEZE.end() ? TRUE : FALSE;
}

void CMAPDoc::AddFREEZEOBJ( DWORD dwID)
{
	if(!IsFreezeOBJ(dwID))
		m_mapFREEZE.insert( MAPDWORD::value_type( dwID, dwID));
}

void CMAPDoc::AddHIDEOBJ( DWORD dwID)
{
	if(!IsHideOBJ(dwID))
		m_mapHIDE.insert( MAPDWORD::value_type( dwID, dwID));
}

void CMAPDoc::EraseHIDEOBJ( DWORD dwID)
{
	MAPDWORD::iterator finder = m_mapHIDE.find(dwID);

	if( finder != m_mapHIDE.end() )
		m_mapHIDE.erase(finder);
}

void CMAPDoc::EraseFREEZEOBJ( DWORD dwID)
{
	MAPDWORD::iterator finder = m_mapFREEZE.find(dwID);

	if( finder != m_mapFREEZE.end() )
		m_mapFREEZE.erase(finder);
}

void CMAPDoc::GetOBJRANDOMPos(D3DXVECTOR3* vPos)
{
}

void CMAPDoc::RotateOBJbyPlane(D3DXVECTOR3* vROT, D3DXVECTOR3* vPOS, CTachyonObject* pOBJ)
{
}

void CMAPDoc::ObjectReplace(BOOL bALLBOJ)
{
}

void CMAPDoc::ObjectAttach(BOOL bALLBOJ)
{
}

void CMAPDoc::ResetOBJATTR()
{
}

void CMAPDoc::GetEqualOBJ(VECTORDWORD* vEqualOBJ, BOOL bALLOBJ)
{
}

void CMAPDoc::AdjustObjectPos(CPoint* pUnitXZ, D3DXVECTOR3* vMoveOffset, BOOL bAttachType)
{
}

void CMAPDoc::ObjectAttach(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos )
{
}

void CMAPDoc::ObjectReplace(MAPMAPOBJ* pMAPOBJ, MAPDWORD* pMAPID, CTachyonObject* pOBJ, DWORD dwID, DWORD* dwUnitPos, D3DXVECTOR3* vMovePos )
{
}
