// MAPView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSDialogBar.h"
#include "MainFrm.h"
#include "MAPOBJBar.h"
#include "MAPView.h"
#include ".\mapview.h"
//#include "Gizimo.h"


inline DWORD FtoDW( FLOAT f) { return *((DWORD*)&f);}


LPDIRECT3DVERTEXBUFFER9 CMAPView::m_pVB = NULL;
LPDIRECT3DTEXTURE9 CMAPView::m_pLTARGET = NULL;
LPDIRECT3DTEXTURE9 CMAPView::m_pLIGHT = NULL;

#define MOVE_RATE					20.0f
#define SEG_COUNT					180

#define BIAS_DEFAULT				0
#define BIAS_SEL					1

// CMAPView

IMPLEMENT_DYNCREATE(CMAPView, CGridView)

CMAPView::CMAPView()
{
	m_bMove = NAV_MOVE_NONE;
	m_rect.SetRectEmpty();

	m_cNAV.InitHandler(
		this,
		WM_PLAYCTRL_PROC,
		WM_PLAYCTRL_PLAY,
		WM_PLAYCTRL_STOP,
		WM_PLAYCTRL_POS);
	DisableLight();

	m_bShowEdge = FALSE;
	m_bDrawOBJ = TRUE;

//	m_pGizimo = new CGizimo( m_hWnd, &m_Camera );
//	m_pGizimo->ChangeGizimo( CGizimoTranslate::GetInstance() );
}

CMAPView::~CMAPView()
{
//	if( m_pGizimo )
//		delete m_pGizimo;

//	m_pGizimo = NULL;
}
BEGIN_MESSAGE_MAP(CMAPView, CGridView)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_GRID_NAVIGATE, OnGridNavigate)
	ON_COMMAND(ID_GRID_SHOW_EDGE, OnGridShowEdge)
	ON_UPDATE_COMMAND_UI(ID_GRID_SHOW_EDGE, OnUpdateGridShowEdge)	
	ON_COMMAND(ID_GROUP_OBJECT_KEY_1, OnGroupObjectKey1)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_2, OnGroupObjectKey2)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_3, OnGroupObjectKey3)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_4, OnGroupObjectKey4)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_5, OnGroupObjectKey5)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_6, OnGroupObjectKey6)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_7, OnGroupObjectKey7)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_8, OnGroupObjectKey8)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_9, OnGroupObjectKey9)
	ON_COMMAND(ID_GROUP_OBJECT_KEY_0, OnGroupObjectKey0)
	ON_WM_CHAR()
	ON_COMMAND(ID_SELECT_EQUALOBJ, OnSelectEqualobj)
	ON_COMMAND(ID_CLEAR_SELOBJ, OnClearSelobj)
END_MESSAGE_MAP()


// CMAPView 진단입니다.

#ifdef _DEBUG
void CMAPView::AssertValid() const
{
	CGridView::AssertValid();
}

void CMAPView::Dump(CDumpContext& dc) const
{
	CGridView::Dump(dc);
}
#endif //_DEBUG


// CMAPView 메시지 처리기입니다.

void CMAPView::ReleaseRECT()
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CMAPView::InitRECT()
{
	ReleaseRECT();

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		5 * sizeof(TNLPOINT),
		0, T3DFVF_TNLPOINT,
		D3DPOOL_MANAGED,
		&m_pVB, NULL);

	static FLOAT vPoint[5][2] = {
		{ 0.0f, 0.0f},
		{ 1.0f, 0.0f},
		{ 1.0f, 1.0f},
		{ 0.0f, 1.0f},
		{ 0.0f, 0.0f}};
	LPTNLPOINT pBUF = NULL;

	m_pVB->Lock( 0, 5 * sizeof(TNLPOINT), (LPVOID *) &pBUF, 0);
	for( int i=0; i<5; i++)
	{
		pBUF[i].m_fPosX = vPoint[i][0];
		pBUF[i].m_fPosY = vPoint[i][1];
		pBUF[i].m_fPosZ = 0.5f;
		pBUF[i].m_fRHW = 1.0f;
		pBUF[i].m_dwColor = VALUEARGB( 0xFF, 0xFF, 0xFF, 0);
	}
	m_pVB->Unlock();
}

void CMAPView::DrawContent()
{
	CGridView::DrawContent();

	CMAPDoc *pDoc = GetDocument();

	if( pDoc && m_bDrawOBJ )
	{
		CMAPLIGHTBar *pLIGHTBar = pDoc->GetMAPLIGHTBar();
		CMAPOBJBar *pOBJBar = pDoc->GetMAPOBJBar();
		CMAPSFXBar *pSFXBar = pDoc->GetMAPSFXBar();
		CMAPSNDBar *pSNDBar = pDoc->GetMAPSNDBar();
		CPlayCtrl *pPlayCtrl = pDoc->GetPlayCtrl();

		BOOL bPlaySND = pPlayCtrl->m_bPlay && ::GetFocus() == GetSafeHwnd();
		MAPMAPLIGHT::iterator itLIGHT;
		MAPOBJECT::iterator itOBJ;
		MAPMAPSFX::iterator itSFX;
		MAPMAPSND::iterator itSND;

		if(bPlaySND)
		{
			CD3DSound::ResetLISTENER(
				&m_Camera.m_vPosition,
				&(m_Camera.m_vTarget - m_Camera.m_vPosition),
				&m_Camera.m_vUp);
		}

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);

		switch(GetEM())
		{
		case EM_INSERTOBJ	:
			{
				D3DXVECTOR3 vPOS;

				if(GetCursorPos(&vPOS))
				{
					CMAPOBJBar::m_pTOBJ->m_vWorld._41 = vPOS.x;
					CMAPOBJBar::m_pTOBJ->m_vWorld._42 = vPOS.y;
					CMAPOBJBar::m_pTOBJ->m_vWorld._43 = vPOS.z;

					CMAPOBJBar::m_pTOBJ->CalcFrame(FALSE);
					CMAPOBJBar::m_pTOBJ->Render(
						&CMainFrame::m_3DDevice,
						&m_Camera);
				}
			}

			break;

		case EM_INSERTSFX	:
			{
				D3DXVECTOR3 vPOS;

				if(GetCursorPos(&vPOS))
					CMAPSFXBar::RenderSFXBOX( vPOS, D3DXVECTOR3( 0.0f, 0.0f, 0.0f));
			}

			break;

		case EM_INSERTSND	:
			{
				D3DXVECTOR3 vPOS;

				if(GetCursorPos(&vPOS))
					CMAPSNDBar::RenderSNDBOX(vPOS);
			}

			break;
		}

		if(!pPlayCtrl->m_bPlay)
		{
			D3DXVECTOR3 vPOS( 0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vROT( 0.0f, 0.0f, 0.0f);

			if( m_bCamMode != CM_SELSFX )
			{
				vPOS = D3DXVECTOR3(
					(FLOAT) atof(pSFXBar->m_strPosX),
					(FLOAT) atof(pSFXBar->m_strPosY),
					(FLOAT) atof(pSFXBar->m_strPosZ)) - pDoc->GetSELSFXPOS();

				vROT = D3DXVECTOR3(
					(FLOAT) atof(pSFXBar->m_strRotX) * D3DX_PI / 180.0f,
					(FLOAT) atof(pSFXBar->m_strRotY) * D3DX_PI / 180.0f,
					(FLOAT) atof(pSFXBar->m_strRotZ) * D3DX_PI / 180.0f) - pDoc->GetSELSFXROT();
			}

			for( itSFX = pDoc->m_pMAP->m_pMAP->m_mapSFX.begin(); itSFX != pDoc->m_pMAP->m_pMAP->m_mapSFX.end(); itSFX++)
				if(IsVisible((*itSFX).second))
					if(pDoc->IsSelSFX((*itSFX).first))
					{
						CMAPSFXBar::RenderSFXBOX( (*itSFX).second->m_vPOS + vPOS, (*itSFX).second->m_vROT + vROT);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SEL);

						for( DWORD i=0; i<CMAPSFXBar::m_MESH.m_dwMeshCount; i++)
							CMAPSFXBar::m_MESH.Render( CMainFrame::m_3DDevice.m_pDevice, i, 0);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
					}
					else
						CMAPSFXBar::RenderSFXBOX( (*itSFX).second->m_vPOS, (*itSFX).second->m_vROT);

			vPOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			if( m_bCamMode != CM_SELSND )
			{
				vPOS = D3DXVECTOR3(
					(FLOAT) atof(pSNDBar->m_strPosX),
					(FLOAT) atof(pSNDBar->m_strPosY),
					(FLOAT) atof(pSNDBar->m_strPosZ)) - pDoc->GetSELSNDPOS();
			}

			for( itSND = pDoc->m_pMAP->m_pMAP->m_mapSND.begin(); itSND != pDoc->m_pMAP->m_pMAP->m_mapSND.end(); itSND++)
				if(IsVisible((*itSND).second))
					if(pDoc->IsSelSND((*itSND).first))
					{
						CMAPSNDBar::RenderSNDBOX((*itSND).second->m_vPOS + vPOS);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SEL);

						for( DWORD i=0; i<CMAPSNDBar::m_MESH.m_dwMeshCount; i++)
							CMAPSNDBar::m_MESH.Render( CMainFrame::m_3DDevice.m_pDevice, i, 0);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
					}
					else
						CMAPSNDBar::RenderSNDBOX((*itSND).second->m_vPOS);
		}

		for( itOBJ = pDoc->m_mapOBJ.begin(); itOBJ != pDoc->m_mapOBJ.end(); itOBJ++)
			if(!pDoc->IsHideOBJ((*itOBJ).first))
			{
				CTachyonObject *pOBJ = (*itOBJ).second;
				pOBJ->CalcFrame(FALSE);

				DWORD	dwTexColorFactor = 0xffffffff;
													
				if(IsVisible(pOBJ))	
				{
					pOBJ->Render(
						&CMainFrame::m_3DDevice,
						&m_Camera);

					if(pDoc->IsSelOBJ((*itOBJ).first))
					{
						MAPACTINST::iterator itAI = pOBJ->m_OBJ.m_mapACT.find(pOBJ->m_OBJ.m_dwCurACT);
						MAPOBJPART::iterator itDRAW;
						LPANI pANI = NULL;

						FLOAT fDist = D3DXVec3Length(&D3DXVECTOR3(
							pOBJ->m_vPosition._41 - m_Camera.m_vPosition.x,
							pOBJ->m_vPosition._42 - m_Camera.m_vPosition.y,
							pOBJ->m_vPosition._43 - m_Camera.m_vPosition.z));

						if( itAI != pOBJ->m_OBJ.m_mapACT.end() )
							pANI = (*itAI).second->m_pANI;

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwTexColorFactor );
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SEL);

						if(pOBJ->m_bUseSHADER)
						{
							LPD3DXMATRIX pInit = NULL;
							DWORD dwNodeCount = 0;

							for( itDRAW = pOBJ->m_OBJ.m_mapDRAW.begin(); itDRAW != pOBJ->m_OBJ.m_mapDRAW.end(); itDRAW++)
							{
								VECTOROBJPART *pDRAW = (*itDRAW).second;

								if( pDRAW->size() > 0 )
								{
									dwNodeCount = (*pDRAW)[0]->m_pMESH->m_dwNodeCount;
									pInit = (*pDRAW)[0]->m_pMESH->m_pBones;

									break;
								}
							}

							if( pANI && pANI->m_pANI && pInit )
							{
								CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLDMATRIX(0), &pOBJ->m_pBone[0]);
								for( int i=0; i<dwNodeCount; i++)
									CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLDMATRIX(i + 1), &(pInit[i] * pOBJ->m_pBone[i + 1]));
							}
							else
								CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &pOBJ->m_vPosition);
						}

						for( itDRAW = pOBJ->m_OBJ.m_mapDRAW.begin(); itDRAW != pOBJ->m_OBJ.m_mapDRAW.end(); itDRAW++)
						{
							VECTOROBJPART *pDRAW = (*itDRAW).second;
							int nCount = INT(pDRAW->size());

							for( int i=0; i<nCount; i++)
							{
								CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, (*pDRAW)[i]->m_pMESH->m_dwNodeCount && pANI && pANI->m_pANI ? D3DVBF_3WEIGHTS : D3DVBF_DISABLE);
								CMainFrame::m_3DDevice.m_pDevice->SetFVF( (*pDRAW)[i]->m_pMESH->m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);

								(*pDRAW)[i]->m_pMESH->Render( CMainFrame::m_3DDevice.m_pDevice, (*pDRAW)[i]->m_dwIndex, (*pDRAW)[i]->m_pMESH->GetLevel(fDist));
							}
						}

						D3DXVECTOR3 objpos( pOBJ->m_vPosition._41, pOBJ->m_vPosition._42, pOBJ->m_vPosition._43 );

						pOBJ->m_vPosition._41 = objpos.x;
						pOBJ->m_vPosition._42 = objpos.y;
						pOBJ->m_vPosition._43 = objpos.z;

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
					}
				}
			}

		if(bPlaySND)
		{
			for( itSND = pDoc->m_pMAP->m_pMAP->m_mapSND.begin(); itSND != pDoc->m_pMAP->m_pMAP->m_mapSND.end(); itSND++)
				if(IsVisible((*itSND).second))
				{
					MAPDWORD::iterator finder = pDoc->m_mapSND.find((*itSND).first);

					if( finder != pDoc->m_mapSND.end() )
					{
						CD3DSound *pSND = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
							MEDIA_TSOUND,
							(*itSND).second->m_dwMediaID);

						if(!pSND->IsPlay((*finder).second))
							pSND->Play((*finder).second);
					}
				}
		}

		if(pPlayCtrl->m_bPlay)
		{
			MAPTSFX::iterator it;

			for( it = CTachyonSFX::m_mapINST.begin(); it != CTachyonSFX::m_mapINST.end(); it++)
				if( (*it).second->m_pHost == pOBJBar &&
					!(*it).second->IsDead() &&
					IsVisible( &D3DXVECTOR3(
					(*it).second->m_vGlobal._41,
					(*it).second->m_vGlobal._42,
					(*it).second->m_vGlobal._43)))
				{
					if( (*it).second->m_dwInitTick != INVALID_INIT_TICK )
						(*it).second->CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);
					(*it).second->Render( &CMainFrame::m_3DDevice, &m_Camera);
				}
		}

		if(pDoc->m_bEditLIGHT)
		{
			static D3DXMATRIX vPosition(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
			D3DXVECTOR3 vMOVE( 0.0f, 0.0f, 0.0f);

			if( m_bCamMode != CM_SELLIGHT )
			{
				vMOVE = D3DXVECTOR3(
					(FLOAT) atof(pLIGHTBar->m_strPosX),
					(FLOAT) atof(pLIGHTBar->m_strPosY),
					(FLOAT) atof(pLIGHTBar->m_strPosZ)) - pDoc->GetSELLIGHTPOS();
			}

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSCALEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSIZE, FtoDW(1.0f));
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSCALE_A, FtoDW(0.0f));
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSCALE_B, FtoDW(0.0f));
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSCALE_C, FtoDW(1.0f));

			CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vPosition);
			CMainFrame::m_3DDevice.m_pDevice->SetFVF(D3DFVF_XYZ);

			for( itLIGHT = pDoc->m_pMAP->m_mapLIGHT.begin(); itLIGHT != pDoc->m_pMAP->m_mapLIGHT.end(); itLIGHT++)
			{
				BOOL bSEL = pDoc->IsSelLIGHT((*itLIGHT).first);
				D3DXVECTOR3 vPOS = (*itLIGHT).second->m_vPOS;

				if(bSEL)
					vPOS += vMOVE;

				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, bSEL ? 0xFFFFFFFF : 0xFFFFFF00);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(16.0f));
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSIZE_MAX, FtoDW(16.0f));

				CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, CMAPView::m_pLIGHT);
				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
					D3DPT_POINTLIST,
					1, &vPOS,
					sizeof(D3DXVECTOR3));

				switch((*itLIGHT).second->m_bType)
				{
				case MLT_DIRECTION	:
					{
						D3DXVECTOR3 vPoint[2];

						vPoint[1] = vPOS + (*itLIGHT).second->m_fRange * (*itLIGHT).second->m_vDIR / D3DXVec3Length(&((*itLIGHT).second->m_vDIR));
						vPoint[0] = vPOS;

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(8.0f));
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSIZE_MAX, FtoDW(8.0f));

						CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, CMAPView::m_pLTARGET);
						CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
							D3DPT_POINTLIST,
							1, &vPoint[1],
							sizeof(D3DXVECTOR3));

						CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, NULL);
						CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
							D3DPT_LINELIST,
							1, vPoint,
							sizeof(D3DXVECTOR3));
					}

					break;

				case MLT_POINT		:
					{
						D3DXVECTOR3 vAxisY = m_Camera.m_vUp / D3DXVec3Length(&m_Camera.m_vUp);
						D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
						D3DXVECTOR3 vPoint[SEG_COUNT + 1];
						D3DXMATRIX vRotZ;

						vPoint[0] = vPOS + (*itLIGHT).second->m_fRange * vAxisY;
						vAxisZ /= D3DXVec3Length(&vAxisZ);

						D3DXMatrixRotationAxis(
							&vRotZ,
							&vAxisZ,
							2.0f * D3DX_PI / ((FLOAT) SEG_COUNT));

						for( int i=1; i<SEG_COUNT; i++)
						{
							vPoint[i] = vPoint[i - 1] - vPOS;

							CTMath::Transform(
								&vRotZ,
								&vPoint[i]);

							vPoint[i] += vPOS;
						}
						vPoint[SEG_COUNT] = vPoint[0];

						CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, NULL);
						CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
							D3DPT_LINESTRIP,
							SEG_COUNT,
							vPoint,
							sizeof(D3DXVECTOR3));
					}

					break;
				}
			}

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_POINTSCALEENABLE, FALSE);
		}

		if( m_bCamMode == CM_SELLIGHT ||
			m_bCamMode == CM_SELOBJ ||
			m_bCamMode == CM_SELSFX ||
			m_bCamMode == CM_SELSND )	
		{
			FLOAT vPoint[5][2] = {
				{ (FLOAT) m_rect.left, (FLOAT) m_rect.top},
				{ (FLOAT) m_rect.right, (FLOAT) m_rect.top},
				{ (FLOAT) m_rect.right, (FLOAT) m_rect.bottom},
				{ (FLOAT) m_rect.left, (FLOAT) m_rect.bottom},
				{ (FLOAT) m_rect.left, (FLOAT) m_rect.top}};
			LPTNLPOINT pBUF = NULL;

			m_pVB->Lock( 0, 5 * sizeof(TNLPOINT), (LPVOID *) &pBUF, 0);
			for( int i=0; i<5; i++)
			{
				pBUF[i].m_fPosX = vPoint[i][0];
				pBUF[i].m_fPosY = vPoint[i][1];
			}
			m_pVB->Unlock();

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

			CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(TNLPOINT));
			CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TNLPOINT);

			CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_LINESTRIP, 0, 4);

			pOBJBar->m_strSELOBJCount.Format("%d",INT(pDoc->m_vSELOBJ.size()));		// 06/08/02 PSY
			pOBJBar->UpdateData(FALSE);
		}
	}
}

BYTE CMAPView::GetEM()
{
	CMAPDoc *pDoc = GetDocument();

	if(pDoc)
		if(pDoc->m_bEditOBJ)
		{
			if( pDoc->m_bInsertOBJ && m_bCamMode == CM_NONE &&
				CMAPOBJBar::m_pTOBJ &&
				!CMAPOBJBar::m_pTOBJ->m_OBJ.m_mapDRAW.empty() )
				return EM_INSERTOBJ;
			else if(!pDoc->m_bInsertOBJ)
				return EM_EDITOBJ;
		}
		else if(pDoc->m_bEditSFX)
		{
			if( pDoc->m_bInsertSFX &&
				m_bCamMode == CM_NONE &&
				CMAPSFXBar::m_pSFX)
				return EM_INSERTSFX;
			else if(!pDoc->m_bInsertSFX)
				return EM_EDITSFX;
		}
		else if(pDoc->m_bEditSND)
		{
			if( pDoc->m_bInsertSND &&
				m_bCamMode == CM_NONE &&
				CMAPSNDBar::m_pSND)
				return EM_INSERTSND;
			else if(!pDoc->m_bInsertSND)
				return EM_EDITSND;
		}
		else if(pDoc->m_bEditLIGHT)
		{
			if( pDoc->m_bInsertLIGHT &&
				m_bCamMode == CM_NONE )
				return EM_INSERTLIGHT;
			else if(!pDoc->m_bInsertLIGHT)
				return EM_EDITLIGHT;
		}

	return EM_NONE;
}

BYTE CMAPView::GetCursorPos( LPD3DXVECTOR3 pPOS)
{
	(*pPOS) = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	return TRUE;
}

void CMAPView::OnMouseMove( UINT nFlags, CPoint point)
{
	static CPoint prev( 0, 0);

	CMAPDoc *pDoc = GetDocument();
	D3DXVECTOR3 vPOS;

	if(GetCursorPos(&vPOS))
	{
		CMainFrame *pMainFrame = (CMainFrame *) AfxGetMainWnd();
		CString strText;

		strText.Format( "X : %.2f", vPOS.x);
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSX);

		strText.Format( "Y : %.2f", vPOS.y);
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSY);

		strText.Format( "Z : %.2f", vPOS.z);
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSZ);
	}

	switch(GetEM())
	{
	case EM_INSERTOBJ	:
	case EM_INSERTSFX	:
	case EM_INSERTSND	:
		if( prev != point )
			pDoc->RedrawAllView();

		break;

	case EM_EDITSFX		:
		if( m_bCamMode == CM_SELSFX )
		{
			if( prev != point )
			{
				CRect rect;
				GetClientRect(&rect);

				m_rect.right = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
				m_rect.bottom = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
				prev = point;

				SelectSFX();
				pDoc->RedrawAllView();
			}

			CView::OnMouseMove( nFlags, point);
			return;
		}

		break;

	case EM_EDITSND		:
		if( m_bCamMode == CM_SELSND )
		{
			if( prev != point )
			{
				CRect rect;
				GetClientRect(&rect);

				m_rect.right = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
				m_rect.bottom = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
				prev = point;

				SelectSND();
				pDoc->RedrawAllView();
			}

			CView::OnMouseMove( nFlags, point);
			return;
		}

		break;

	case EM_EDITOBJ		:
		if( m_bCamMode == CM_SELOBJ )
		{
			if( prev != point )
			{
				CRect rect;
				GetClientRect(&rect);
				m_rect.right = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
				m_rect.bottom = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;

				if( m_rect.Width() < 5 && m_rect.Height() < 5 )
				{
					m_rect.bottom = m_rect.top;
					m_rect.right = m_rect.left;
				}

				prev = point;
				SelectOBJ(nFlags);
				pDoc->RedrawAllView();
			}

			CView::OnMouseMove( nFlags, point);
			return;
		}

		break;

	case EM_EDITLIGHT	:
		if( m_bCamMode == CM_SELLIGHT )
		{
			if( prev != point )
			{
				CRect rect;
				GetClientRect(&rect);

				m_rect.right = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
				m_rect.bottom = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
				prev = point;

				SelectLIGHT();
				pDoc->RedrawAllView();
			}

			CView::OnMouseMove( nFlags, point);
			return;
		}

		break;
	}

	prev = point;

	CGridView::OnMouseMove( nFlags, point);
}

void CMAPView::OnLButtonDown( UINT nFlags, CPoint point)
{
//	CGizimoMouseController::GetInstance()->BeginDrag();

	CGridView::OnLButtonDown(nFlags, point);
	CMAPDoc *pDoc = GetDocument();
	CMAPOBJBar* pOBJBar = pDoc->GetMAPOBJBar();

	if( m_bViewPort == VIEWPORT_NAVIGATE &&
		m_bCamMode != CM_NONE &&
		!(nFlags&MK_CONTROL) && (
		pDoc->m_bEditLIGHT ||
		pDoc->m_bEditOBJ ||
		pDoc->m_bEditSFX ||
		pDoc->m_bEditSND ))
	{
		CPoint screen = point;

		ClientToScreen(&screen);
		SetCursorPos(
			screen.x,
			screen.y);

		m_bCamMode = CM_NONE;
	}

	switch(GetEM())
	{
	case EM_INSERTLIGHT	:
		{
			D3DXVECTOR3 vPOS;

			if(GetCursorPos(&vPOS))
			{
				pDoc->AddLIGHT(&vPOS);
				pDoc->RedrawAllView();
			}
		}

		break;

	case EM_INSERTSFX	:
		{
			D3DXVECTOR3 vPOS;

			if(GetCursorPos(&vPOS))
			{
				LPMAPSFX pMAPSFX = new MAPSFX();
				DWORD dwID = pDoc->GetNewSFXID();

				pDoc->m_pMAP->m_mapSFX.insert( MAPDWORD::value_type( dwID, CMAPSFXBar::m_pSFX->m_dwID));
				pMAPSFX->m_pSFX = &CMAPSFXBar::m_pSFX->m_SFX;
				pMAPSFX->m_vPOS = vPOS;

				pDoc->m_pMAP->m_pMAP->m_mapSFX.insert( MAPMAPSFX::value_type( dwID, pMAPSFX));
			}
		}

		break;

	case EM_INSERTSND	:
		{
			D3DXVECTOR3 vPOS;

			if(GetCursorPos(&vPOS))
			{
				LPMAPSND pMAPSND = new MAPSND();
				DWORD dwID = pDoc->GetNewSNDID();

				pMAPSND->m_dwMediaID = CMAPSNDBar::m_pSND->m_dwID;
				pMAPSND->m_vPOS = vPOS;

				pDoc->m_pMAP->m_pMAP->m_mapSND.insert( MAPMAPSND::value_type( dwID, pMAPSND));
			}
		}

		break;

	case EM_INSERTOBJ	:
		{
			D3DXVECTOR3 vPOS;
			
            if(GetCursorPos(&vPOS))
			{
				int nOBJSprayCount = 1;
				if( pOBJBar->m_bInsertOBJBrush )
					nOBJSprayCount = pOBJBar->m_nOBJCount;
				
				pDoc->m_vOBJPrePos.clear();


				D3DXVECTOR3 vPOSTEMP = vPOS;
				
				for( int i = 0 ; i < nOBJSprayCount ; i++ )			// 카운트 만큼
				{
					CTachyonObject *pOBJ = new CTachyonObject();
					LPMAPOBJ pMAPOBJ = new MAPOBJ();
					DWORD dwID = pDoc->GetNewOBJID();
					
					MAPCLKINST::iterator itCI;

					pDoc->m_pMAP->m_mapOBJ.insert( MAPDWORD::value_type( dwID, CMAPOBJBar::m_pOBJ->m_dwID));
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
					
					vPOS = vPOSTEMP;
					pMAPOBJ->m_vPOS = vPOS;
					pOBJ->InitOBJ( &CMainFrame::m_3DDevice, pMAPOBJ);
					if( pOBJBar->m_bInsertOBJBrush ) 
					{
						// 06/04/27 PSY OBJ SPRAY RANDOM ROTATION, RANDOM SCALE, OBJ LANDING
						////////////////////////////////////////////////////////

						// OBJ RANDOM POSITION
						//-----------------------------------------------------
						pDoc->GetOBJRANDOMPos(&vPOS);		// Get Object Radom Position
						//-----------------------------------------------------

						srand(timeGetTime());
						rand();
						// OBJ RANDOM ROTATION
						//-----------------------------------------------------
						INT fROTY = rand() % INT(D3DX_PI * 2 * 100);
						pMAPOBJ->m_vROT.y = FLOAT(fROTY) / 100;
						//-----------------------------------------------------

						// OBJ RANDOM SCALE
						//-----------------------------------------------------
						pOBJBar->UpdateData();
						FLOAT fScale = FLOAT( (rand() % INT( ( atof( pOBJBar->m_strOBJScaleMax ) - atof( pOBJBar->m_strOBJScaleMin ) ) * 100 ) ) ) / 100.0f;
						fScale += fScale < atof( pOBJBar->m_strOBJScaleMin ) ? FLOAT( atof( pOBJBar->m_strOBJScaleMin ) ) : 0;
						D3DXVECTOR3 vScale( fScale, fScale, fScale );
						D3DXMATRIX vSCALE;
						D3DXMatrixScaling(
							&vSCALE,
							fScale,
							fScale,
							fScale);
						pMAPOBJ->m_vSCALE = vScale;
						pOBJ->m_vScale = vSCALE;

						//-----------------------------------------------------
						// OBJ LANDING
						//-----------------------------------------------------
						D3DXVECTOR3 vHEIGHT(
							0.0f,
							pDoc->GetHeight( 
							vPOS.x, 
							vPOS.y, 
							vPOS.z ) - vPOS.y,
							0.0f);
						
						vHEIGHT.y = FLOAT(INT(vHEIGHT.y * 100.0f));
						vHEIGHT.y /= 100.0f;

						vPOS.y += vHEIGHT.y;
						//-----------------------------------------------------
						pDoc->RotateOBJbyPlane(&pMAPOBJ->m_vROT, &vPOS, pOBJ);
						////////////////////////////////////////////////////////
						pMAPOBJ->m_vPOS = vPOS;
					}
					
					pOBJ->m_pHost = pDoc->GetMAPOBJBar();
					
					pDoc->m_pMAP->m_pMAP->m_mapOBJ.insert( MAPMAPOBJ::value_type( dwID, pMAPOBJ));
					pDoc->m_mapOBJ.insert( MAPOBJECT::value_type( dwID, pOBJ));
				}
			}
		}

		break;

	case EM_EDITLIGHT	:
		if( !pDoc->m_vSELLIGHT.empty() && (nFlags&MK_SHIFT) && (nFlags&MK_CONTROL) )
		{
			CPoint screen = point;
			D3DXVECTOR3 vPOS;

			ClientToScreen(&screen);
			SetCursorPos(
				screen.x,
				screen.y);

			SetCursor(LoadCursor( NULL, IDC_ARROW));
			m_bCamMode = CM_NONE;

			if(GetCursorPos(&vPOS))
				pDoc->CopyLIGHT(vPOS - pDoc->GetSELLIGHTPOS());
		}
		else if( m_bCamMode == CM_NONE )
		{
			CRect rect;
			GetClientRect(&rect);

			m_rect.left = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
			m_rect.top = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
			m_rect.right = m_rect.left;
			m_rect.bottom = m_rect.top;

			m_bCamMode = CM_SELLIGHT;
			pDoc->ClearSELLIGHT();
			SelectLIGHT();
			pDoc->RedrawAllView();
		}

		break;

	case EM_EDITSFX		:
		if( !pDoc->m_vSELSFX.empty() && (nFlags&MK_SHIFT) && (nFlags&MK_CONTROL) )
		{
			CPoint screen = point;
			D3DXVECTOR3 vPOS;

			ClientToScreen(&screen);
			SetCursorPos(
				screen.x,
				screen.y);

			SetCursor(LoadCursor( NULL, IDC_ARROW));
			m_bCamMode = CM_NONE;

			if(GetCursorPos(&vPOS))
				pDoc->CopySFX(vPOS - pDoc->GetSELSFXPOS());
		}
		else if( m_bCamMode == CM_NONE )
		{
			CRect rect;
			GetClientRect(&rect);

			m_rect.left = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
			m_rect.top = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
			m_rect.right = m_rect.left;
			m_rect.bottom = m_rect.top;

			m_bCamMode = CM_SELSFX;
			pDoc->ClearSELSFX();
			SelectSFX();
			pDoc->RedrawAllView();
		}

		break;

	case EM_EDITSND		:
		if( !pDoc->m_vSELSND.empty() && (nFlags&MK_SHIFT) && (nFlags&MK_CONTROL) )
		{
			CPoint screen = point;
			D3DXVECTOR3 vPOS;

			ClientToScreen(&screen);
			SetCursorPos(
				screen.x,
				screen.y);

			SetCursor(LoadCursor( NULL, IDC_ARROW));
			m_bCamMode = CM_NONE;

			if(GetCursorPos(&vPOS))
				pDoc->CopySND(vPOS - pDoc->GetSELSNDPOS());
		}
		else if( m_bCamMode == CM_NONE )
		{
			CRect rect;
			GetClientRect(&rect);

			m_rect.left = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
			m_rect.top = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
			m_rect.right = m_rect.left;
			m_rect.bottom = m_rect.top;

			m_bCamMode = CM_SELSND;
			pDoc->ClearSELSND();
			SelectSND();
			pDoc->RedrawAllView();
		}

		break;

	case EM_EDITOBJ		:
		if( !pDoc->m_vSELOBJ.empty() && (nFlags&MK_SHIFT) && (nFlags&MK_CONTROL) )
		{
			CPoint screen = point;
			D3DXVECTOR3 vPOS;

			ClientToScreen(&screen);
			SetCursorPos(
				screen.x,
				screen.y);
			
			SetCursor(LoadCursor( NULL, IDC_ARROW));
			m_bCamMode = CM_NONE;

			// 10/12 PSY Select Object Pos Data Update And Reset		
			VECTORDWORD vSELOBJ;
			VECTORDWORD::iterator it;

			for(it=pDoc->m_vSELOBJ.begin();it!=pDoc->m_vSELOBJ.end(); it++)
				vSELOBJ.push_back((*it));

			pDoc->ClearSELOBJ();
			pDoc->m_vSELOBJ = vSELOBJ;
			vSELOBJ.clear();

			if(GetCursorPos(&vPOS))
				pDoc->CopyOBJ(vPOS - pDoc->GetSELOBJPOS());
		}
		else if( m_bCamMode == CM_NONE )
		{
			CRect rect;
			GetClientRect(&rect);

			m_rect.left = point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2;
			m_rect.top = point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2;
			m_rect.right = m_rect.left;
			m_rect.bottom = m_rect.top;

			m_bCamMode = CM_SELOBJ;

			VECTORDWORD vSELOBJ;
			VECTORDWORD::iterator it;

			for(it=pDoc->m_vSELOBJ.begin();it!=pDoc->m_vSELOBJ.end(); it++)
				vSELOBJ.push_back((*it));

			pDoc->ClearSELOBJ();
			pDoc->m_vSELOBJ = vSELOBJ;
			vSELOBJ.clear();

			SelectOBJ(nFlags);
			pDoc->RedrawAllView();
		}

		break;
	}
}

void CMAPView::OnLButtonUp(UINT nFlags, CPoint point)
{
//	CGizimoMouseController::GetInstance()->EndDrag();

	CMAPDoc *pDoc = GetDocument();
	BOOL bRedraw = m_bCamMode == CM_SELLIGHT || m_bCamMode == CM_SELOBJ || m_bCamMode == CM_SELSFX || m_bCamMode == CM_SELSND ;
	CGridView::OnLButtonUp( nFlags, point);

	if(bRedraw)
	{
		CMAPDoc *pDoc = GetDocument();

		pDoc->UpdateLIGHTBAR();
		pDoc->UpdateOBJBAR();
		pDoc->UpdateSFXBAR();
		pDoc->UpdateSNDBAR();

		if(!IsPlay())
			Render();
	}
}

void CMAPView::SelectLIGHT()
{
	CMAPDoc *pDoc = GetDocument();

	if(pDoc)
	{
		MAPMAPLIGHT::iterator it;

		pDoc->m_vSELLIGHT.clear();
		for( it = pDoc->m_pMAP->m_mapLIGHT.begin(); it != pDoc->m_pMAP->m_mapLIGHT.end(); it++)
			if(PtInRect( &((*it).second->m_vPOS), &m_Camera, m_rect))
				pDoc->m_vSELLIGHT.push_back((*it).first);
	}
}

void CMAPView::SelectOBJ( UINT nFlags)
{
	CMAPDoc *pDoc = GetDocument();
	if(pDoc)
	{
		BOOL bSelOne = m_rect.left == m_rect.right && m_rect.top == m_rect.bottom;
		BOOL bInit = FALSE;

		MAPOBJECT::iterator it;
		FLOAT fDist = 0.0f;

		if( GetKeyState(VK_MENU) >= 0 )
			pDoc->m_vSELOBJ.clear();
		pDoc->m_vSELOBJBUF.clear();

		for( it = pDoc->m_mapOBJ.begin(); it != pDoc->m_mapOBJ.end(); it++)
		{
			CTachyonObject *pOBJ = (*it).second;
			if( !pDoc->IsFreezeOBJ(( *it ).first) && !pDoc->IsHideOBJ(( *it ).first) )	// 감춰졌거나 동결된것은 선택 불가
			{
				if(bSelOne)
				{
					OBJHIT vHIT = pOBJ->HitTest(
						&m_Camera,
						m_rect.left,
						m_rect.top);

					if( vHIT.m_nIndex != -1 && ( !bInit || vHIT.m_fDist < fDist ))
					{
						fDist = vHIT.m_fDist;
						bInit = TRUE;

						pDoc->m_vSELOBJBUF.clear();
						pDoc->m_vSELOBJBUF.push_back((*it).first);
					}
				}
				else if(pOBJ->OBJInRect( &m_Camera, m_rect))
					pDoc->m_vSELOBJBUF.push_back((*it).first);
			}
		}
	}
}

void CMAPView::SelectSFX()
{
	CMAPDoc *pDoc = GetDocument();
	if(pDoc)
	{
		BOOL bSelOne = m_rect.left == m_rect.right && m_rect.top == m_rect.bottom;
		BOOL bInit = FALSE;

		MAPMAPSFX::iterator it;
		FLOAT fDist = 0.0f;

		pDoc->m_vSELSFX.clear();
		for( it = pDoc->m_pMAP->m_pMAP->m_mapSFX.begin(); it != pDoc->m_pMAP->m_pMAP->m_mapSFX.end(); it++)
		{
			LPMAPSFX pSFX = (*it).second;
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;
			D3DXMATRIX vPOS;

			D3DXMatrixIdentity(&vPOS);
			vPOS._41 = pSFX->m_vPOS.x;
			vPOS._42 = pSFX->m_vPOS.y;
			vPOS._43 = pSFX->m_vPOS.z;

			D3DXMatrixRotationX(
				&vRotX,
				pSFX->m_vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				pSFX->m_vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				pSFX->m_vROT.z);
			vPOS = vRotX * vRotY * vRotZ * vPOS;

			if(bSelOne)
			{
				FLOAT fCurDist = 0.0f;
				BOOL bResult = MeshHitTest(
					&CMAPSFXBar::m_MESH,
					&vPOS,
					&m_Camera,
					m_rect.left,
					m_rect.top,
					&fCurDist);

				if( bResult && ( !bInit || fCurDist < fDist ))
				{
					fDist = fCurDist;
					bInit = TRUE;

					pDoc->m_vSELSFX.clear();
					pDoc->m_vSELSFX.push_back((*it).first);
				}
			}
			else if(MeshInRect( &CMAPSFXBar::m_MESH, &vPOS, &m_Camera, m_rect))
				pDoc->m_vSELSFX.push_back((*it).first);
		}
	}
}

void CMAPView::SelectSND()
{
	CMAPDoc *pDoc = GetDocument();

	if(pDoc)
	{
		BOOL bSelOne = m_rect.left == m_rect.right && m_rect.top == m_rect.bottom;
		BOOL bInit = FALSE;

		MAPMAPSND::iterator it;
		FLOAT fDist = 0.0f;

		pDoc->m_vSELSND.clear();
		for( it = pDoc->m_pMAP->m_pMAP->m_mapSND.begin(); it != pDoc->m_pMAP->m_pMAP->m_mapSND.end(); it++)
		{
			LPMAPSND pSND = (*it).second;
			D3DXMATRIX vPOS;

			D3DXMatrixIdentity(&vPOS);
			vPOS._41 = pSND->m_vPOS.x;
			vPOS._42 = pSND->m_vPOS.y;
			vPOS._43 = pSND->m_vPOS.z;

			if(bSelOne)
			{
				FLOAT fCurDist = 0.0f;
				BOOL bResult = MeshHitTest(
					&CMAPSNDBar::m_MESH,
					&vPOS,
					&m_Camera,
					m_rect.left,
					m_rect.top,
					&fCurDist);

				if( bResult && ( !bInit || fCurDist < fDist ))
				{
					fDist = fCurDist;
					bInit = TRUE;

					pDoc->m_vSELSND.clear();
					pDoc->m_vSELSND.push_back((*it).first);
				}
			}
			else if(MeshInRect( &CMAPSNDBar::m_MESH, &vPOS, &m_Camera, m_rect))
				pDoc->m_vSELSND.push_back((*it).first);
		}
	}
}

void CMAPView::RotateView( FLOAT fRotX,
						   FLOAT fRotY,
						   FLOAT fRotZ)
{
	CMAPDoc *pDoc = GetDocument();

	switch(GetEM())
	{
	case EM_EDITLIGHT	:
		if( INT(pDoc->m_vSELLIGHT.size()) == 1 )
		{
			MAPMAPLIGHT::iterator finder = pDoc->m_pMAP->m_mapLIGHT.find(pDoc->m_vSELLIGHT[0]);

			switch((*finder).second->m_bType)
			{
			case MLT_DIRECTION	:
				{
					switch(m_bApplyMode)
					{
					case APPLY_X	:
						{
							fRotY = 0.0f;
							fRotZ = 0.0f;
						}

						break;

					case APPLY_Y	:
						{
							fRotX = 0.0f;
							fRotZ = 0.0f;
						}

						break;

					case APPLY_Z	:
						{
							fRotX = 0.0f;
							fRotY = 0.0f;
						}

						break;
					}
				}

				break;

			case MLT_POINT		:
				{
					fRotX = fabs(fRotX) < fabs(fRotY) ? fRotY : fRotX;
					fRotX = fabs(fRotX) < fabs(fRotZ) ? fRotZ : fRotX;
					fRotY = 0.0f;
					fRotZ = 0.0f;
				}

				break;
			}

			pDoc->RotateLIGHT(D3DXVECTOR3( fRotX, fRotY, fRotZ));
		}
		else
			CGridView::RotateView( fRotX, fRotY, fRotZ);

		break;

	case EM_EDITOBJ		:
		if(!pDoc->m_vSELOBJ.empty())
		{
			switch(m_bApplyMode)
			{
			case APPLY_X	:
				{
					fRotY = 0.0f;
					fRotZ = 0.0f;
				}

				break;

			case APPLY_Y	:
				{
					fRotX = 0.0f;
					fRotZ = 0.0f;
				}

				break;

			case APPLY_Z	:
				{
					fRotX = 0.0f;
					fRotY = 0.0f;
				}

				break;
			}

			pDoc->RotateOBJ(D3DXVECTOR3( fRotX, fRotY, fRotZ));
		}
		else
			CGridView::RotateView( fRotX, fRotY, fRotZ);

		break;

	case EM_EDITSFX		:
		if(!pDoc->m_vSELSFX.empty())
		{
			switch(m_bApplyMode)
			{
			case APPLY_X	:
				{
					fRotY = 0.0f;
					fRotZ = 0.0f;
				}

				break;

			case APPLY_Y	:
				{
					fRotX = 0.0f;
					fRotZ = 0.0f;
				}

				break;

			case APPLY_Z	:
				{
					fRotX = 0.0f;
					fRotY = 0.0f;
				}

				break;
			}

			pDoc->RotateSFX(D3DXVECTOR3( fRotX, fRotY, fRotZ));
		}
		else
			CGridView::RotateView( fRotX, fRotY, fRotZ);

		break;

	default				: CGridView::RotateView( fRotX, fRotY, fRotZ); break;
	}
}

void CMAPView::MoveView( FLOAT fMoveX,
						 FLOAT fMoveY,
						 FLOAT fMoveZ)
{
	CMAPDoc *pDoc = GetDocument();
	CMAPOBJBar *pBar = pDoc->GetMAPOBJBar();

	switch(GetEM())
	{
	case EM_EDITLIGHT	:
		if( !pDoc->m_vSELLIGHT.empty() && !IsPlay() )
		{
			D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
			D3DXVECTOR3 vAxisY = m_Camera.m_vUp;
			D3DXVECTOR3 vAxisX;

			D3DXVec3Cross(
				&vAxisX,
				&vAxisZ,
				&vAxisY);
			vAxisX /= D3DXVec3Length(&vAxisX);
			vAxisY /= D3DXVec3Length(&vAxisY);

			if( m_bApplyMode != APPLY_X && m_bApplyMode != APPLY_Z )
				fMoveX = 0.0f;

			if( m_bApplyMode != APPLY_Y && m_bApplyMode != APPLY_Z )
				fMoveY = 0.0f;

			pDoc->MoveLIGHT(fMoveX * vAxisX + fMoveY * vAxisY);
		}
		else
			CGridView::MoveView( fMoveX, fMoveY, fMoveZ);

		break;

	case EM_EDITOBJ		:
		if( !pDoc->m_vSELOBJ.empty() && !IsPlay() )
		{
			D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
			D3DXVECTOR3 vAxisY = m_Camera.m_vUp;
			D3DXVECTOR3 vAxisX;

			D3DXVec3Cross(
				&vAxisX,
				&vAxisZ,
				&vAxisY);
			vAxisX /= D3DXVec3Length(&vAxisX);
			vAxisY /= D3DXVec3Length(&vAxisY);

			if( m_bApplyMode != APPLY_X && m_bApplyMode != APPLY_Z )
				fMoveX = 0.0f;

			if( m_bApplyMode != APPLY_Y && m_bApplyMode != APPLY_Z )
				fMoveY = 0.0f;

			pDoc->MoveOBJ(fMoveX * vAxisX + fMoveY * vAxisY);
		}
		else
			CGridView::MoveView( fMoveX, fMoveY, fMoveZ);

		break;

	case EM_EDITSFX		:
		if( !pDoc->m_vSELSFX.empty() && !IsPlay() )
		{
			D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
			D3DXVECTOR3 vAxisY = m_Camera.m_vUp;
			D3DXVECTOR3 vAxisX;

			D3DXVec3Cross(
				&vAxisX,
				&vAxisZ,
				&vAxisY);
			vAxisX /= D3DXVec3Length(&vAxisX);
			vAxisY /= D3DXVec3Length(&vAxisY);

			if( m_bApplyMode != APPLY_X && m_bApplyMode != APPLY_Z )
				fMoveX = 0.0f;

			if( m_bApplyMode != APPLY_Y && m_bApplyMode != APPLY_Z )
				fMoveY = 0.0f;

			pDoc->MoveSFX(fMoveX * vAxisX + fMoveY * vAxisY);
		}
		else
			CGridView::MoveView( fMoveX, fMoveY, fMoveZ);

		break;

	case EM_EDITSND		:
		if( !pDoc->m_vSELSND.empty() && !IsPlay() )
		{
			D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
			D3DXVECTOR3 vAxisY = m_Camera.m_vUp;
			D3DXVECTOR3 vAxisX;

			D3DXVec3Cross(
				&vAxisX,
				&vAxisZ,
				&vAxisY);
			vAxisX /= D3DXVec3Length(&vAxisX);
			vAxisY /= D3DXVec3Length(&vAxisY);

			if( m_bApplyMode != APPLY_X && m_bApplyMode != APPLY_Z )
				fMoveX = 0.0f;

			if( m_bApplyMode != APPLY_Y && m_bApplyMode != APPLY_Z )
				fMoveY = 0.0f;

			pDoc->MoveSND(fMoveX * vAxisX + fMoveY * vAxisY);
		}
		else
			CGridView::MoveView( fMoveX, fMoveY, fMoveZ);

		break;

	default				: CGridView::MoveView( fMoveX, fMoveY, fMoveZ); break;
	}
}

void CMAPView::OnInitialUpdate()
{
	CGridView::OnInitialUpdate();
	CMAPDoc *pDoc = GetDocument();
	D3DXVECTOR3 vPOS;

	pDoc->GetWorldCenter(&vPOS);
	vPOS -= m_Camera.m_vTarget;

	m_Camera.Move(
		vPOS.x,
		vPOS.y,
		vPOS.z,
		TRUE);

	if(!IsPlay())
		Render();
}

void CMAPView::OnGridCamreset()
{
	FLOAT fDist = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition));
	D3DXVECTOR3 vPOS;

	CMAPDoc *pDoc = GetDocument();
	CGridView::InitCamera();

	if(!m_Camera.IsOrthoCamera())
		fDist = 2.0f * fDist * tanf(m_Camera.m_fFOV / 2.0f);
	pDoc->GetWorldCenter(&vPOS);

	if( m_bViewPort != VIEWPORT_NAVIGATE )
	{
		vPOS -= m_Camera.m_vTarget;
		FitCamDist(fDist);
	}
	else
		vPOS.y += NAV_INIT_HEIGHT;

	m_Camera.Move(
		vPOS.x,
		vPOS.y,
		vPOS.z,
		TRUE);

	if(!IsPlay())
		Render();
}

BYTE CMAPView::IsVisible( LPD3DXVECTOR3 pPOS)
{
	return PtInView(pPOS);
}

BYTE CMAPView::IsVisible( CTachyonObject *pOBJ)
{
	D3DXVECTOR3 vPOS(
		pOBJ->m_vPosition._41,
		pOBJ->m_vPosition._42,
		pOBJ->m_vPosition._43);

	D3DXVECTOR3 vMAX;
	D3DXVECTOR3 vMIN;
	D3DXPLANE vPlane;
	FLOAT fRadius;
	FLOAT fLength;
	FLOAT fDist;

	pOBJ->GetSize( vMIN, vMAX);
	fRadius = (FLOAT) fabs(vMAX.x - vMIN.x) / 2.0f;

	D3DXPlaneFromPointNormal(
		&vPlane,
		&vPOS,
		&m_Camera.m_vAxisZ);

	CTMath::PlaneIntersectLine(
		&vMAX,
		&m_Camera.m_vPosition,
		&m_Camera.m_vTarget,
		&vPlane);
	fDist = D3DXVec3Length(&(vMAX - vPOS));

	if(m_Camera.IsOrthoCamera())
		fLength = sqrt(m_Camera.m_fWidth * m_Camera.m_fWidth + m_Camera.m_fHeight * m_Camera.m_fHeight) / 2.0f;
	else
		fLength = D3DXVec3Length(&(vMAX - m_Camera.m_vPosition)) * tanf(m_Camera.m_fFOV / 2.0f);

	return fLength + fRadius > fDist && PtInView(&vPOS);
}

BYTE CMAPView::IsVisible( LPMAPSFX pSFX)
{
	return PtInView(&pSFX->m_vPOS);
}

BYTE CMAPView::IsVisible( LPMAPSND pSND)
{
	return PtInView(&pSND->m_vPOS);
}

BYTE CMAPView::PtInView( LPD3DXVECTOR3 pPOS)
{
	return TRUE;
}

void CMAPView::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( m_bViewPort == VIEWPORT_NAVIGATE )
	{
		switch(nChar)
		{
		case VK_LEFT	: m_bMove |= NAV_MOVE_LEFT; break;
		case VK_RIGHT	: m_bMove |= NAV_MOVE_RIGHT; break;
		case VK_UP		: m_bMove |= NAV_MOVE_FORWARD; break;
		case VK_DOWN	: m_bMove |= NAV_MOVE_BACK; break;
		case VK_PRIOR	: m_bMove |= NAV_MOVE_UP; break;
		case VK_NEXT	: m_bMove |= NAV_MOVE_DOWN; break;
		}

		if( m_bMove != NAV_MOVE_NONE )
			m_cNAV.Play();
	}
	
	CMAPDoc *pDoc = GetDocument();
	CMAPOBJBar		*pOBJBar = pDoc->GetMAPOBJBar();
	CMAPSFXBar		*pSFXBar = pDoc->GetMAPSFXBar();
	CMAPSNDBar		*pSNDBar = pDoc->GetMAPSNDBar();
	CMAPLIGHTBar	*pLightBar = pDoc->GetMAPLIGHTBar();

	if( UINT('0') <= nChar && UINT('9') >= nChar )
		OnGroupCommand( nChar - UINT('0'), FALSE );
	
	switch( nChar )									// Edit Object 모드에서 Del 키를 누르면 선택오브젝트 삭제
	{												// 06.02.18 박상연
	case VK_DELETE:
		pOBJBar->OnObjectDelete();	
		break;

	case VK_BACK:									// back space를 누르면 이전 모드로
		if( pDoc->m_bEditSND )	
		{
			pSNDBar->OnBack();
		}
		else if( pDoc->m_bEditOBJ )
		{
			pOBJBar->OnBack();
		}
		else if( pDoc->m_bEditSFX )
		{
			pSFXBar->OnBack();
		}
		else if( pDoc->m_bEditLIGHT )
		{
			pLightBar->OnBack();
		}
		break;
	case VK_TAB:
		if( pDoc->m_bEditSND )	
		{
			if( !pDoc->m_bInsertSND )
			{
				pDoc->m_bInsertSND = pSNDBar->m_nMode == 1;
				pSNDBar->m_cEditMode.SetCheck(BST_UNCHECKED);
				pSNDBar->m_cInsertMode.SetCheck(BST_CHECKED);
				pSNDBar->OnBnClickedRadioInsert();
			}
			else
			{
				pDoc->m_bInsertSND = pSNDBar->m_nMode == 1;
				pSNDBar->m_cInsertMode.SetCheck(BST_UNCHECKED);
				pSNDBar->m_cEditMode.SetCheck(BST_CHECKED);
				pSNDBar->OnBnClickedRadioEdit();
			}
		}
		else if( pDoc->m_bEditOBJ )
		{
			if( !pDoc->m_bInsertOBJ )
			{
				pDoc->m_bInsertOBJ = pOBJBar->m_nMode == 1;
				pOBJBar->m_cEditMode.SetCheck(BST_UNCHECKED);
				pOBJBar->m_cInsertMode.SetCheck(BST_CHECKED);
				//if( pOBJBar->m_cSelectTile.GetCheck() )
				//	pOBJBar->m_cSelectTile.SetCheck( BST_UNCHECKED );
				pOBJBar->OnBnClickedRadioInsert();
				
			}
			else
			{
				pDoc->m_bInsertOBJ = pOBJBar->m_nMode == 1;
				pOBJBar->m_cEditMode.SetCheck(BST_CHECKED);
				pOBJBar->m_cInsertMode.SetCheck(BST_UNCHECKED);
				pOBJBar->OnBnClickedRadioEdit();
			}
		}
		else if( pDoc->m_bEditSFX )
		{
			if( !pDoc->m_bInsertSFX )
			{
				pDoc->m_bInsertSFX = pSFXBar->m_nMode == 1;
				pSFXBar->m_cEditMode.SetCheck( BST_UNCHECKED );
				pSFXBar->m_cInsertMode.SetCheck(BST_CHECKED);
				pSFXBar->OnBnClickedRadioInsert();
			}
			else
			{
				pDoc->m_bInsertSFX = pSFXBar->m_nMode == 1;
				pSFXBar->m_cInsertMode.SetCheck(BST_UNCHECKED);
				pSFXBar->m_cEditMode.SetCheck(BST_CHECKED);
				pSFXBar->OnBnClickedRadioEdit();
			}
		}
		else if( pDoc->m_bEditLIGHT )
		{
			if( !pDoc->m_bInsertLIGHT )
			{
				pDoc->m_bInsertLIGHT = pLightBar->m_nMode == 1;
				pLightBar->m_cEditMode.SetCheck( BST_UNCHECKED );
				pLightBar->m_cInsertMode.SetCheck(BST_CHECKED);
				pLightBar->OnBnClickedRadioInsert();
			}
			else
			{
				pDoc->m_bInsertLIGHT = pLightBar->m_nMode == 1;
				pLightBar->m_cInsertMode.SetCheck(BST_UNCHECKED);
				pLightBar->m_cEditMode.SetCheck(BST_CHECKED);
				pLightBar->OnBnClickedRadioEdit();
			}
		}
		break;
	}
	
	CGridView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMAPView::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( m_bViewPort == VIEWPORT_NAVIGATE )
	{
		switch(nChar)
		{
		case VK_LEFT	: m_bMove &= ~NAV_MOVE_LEFT; break;
		case VK_RIGHT	: m_bMove &= ~NAV_MOVE_RIGHT; break;
		case VK_UP		: m_bMove &= ~NAV_MOVE_FORWARD; break;
		case VK_DOWN	: m_bMove &= ~NAV_MOVE_BACK; break;
		case VK_PRIOR	: m_bMove &= ~NAV_MOVE_UP; break;
		case VK_NEXT	: m_bMove &= ~NAV_MOVE_DOWN; break;
		}

		if( m_bMove == NAV_MOVE_NONE )
			m_cNAV.Stop();
	}

	CGridView::OnKeyUp(nChar, nRepCnt, nFlags);
}

LRESULT CMAPView::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	static D3DXVECTOR3 vMOVE[6] = {
		D3DXVECTOR3( -1.0f,  0.0f,  0.0f),
		D3DXVECTOR3(  0.0f,  0.0f,  1.0f),
		D3DXVECTOR3(  1.0f,  0.0f,  0.0f),
		D3DXVECTOR3(  0.0f,  0.0f, -1.0f),
		D3DXVECTOR3(  0.0f,  1.0f,  0.0f),
		D3DXVECTOR3(  0.0f, -1.0f,  0.0f)};

	static BYTE bDIR[6] = {
		NAV_MOVE_LEFT,
		NAV_MOVE_FORWARD,
		NAV_MOVE_RIGHT,
		NAV_MOVE_BACK,
		NAV_MOVE_UP,
		NAV_MOVE_DOWN};

	FLOAT fHeight = m_Camera.m_vPosition.y - GetCamHeight();
	DWORD dwTick = ((DWORD) wParam) - m_dwCamTick;
	m_dwCamTick = (DWORD) wParam;

	for( int i=0; i<6; i++)
		if( m_bMove & bDIR[i] )
		{
			m_Camera.Move(
				vMOVE[i].x * FLOAT(dwTick) / MOVE_RATE,
				0.0f,
				vMOVE[i].z * FLOAT(dwTick) / MOVE_RATE);
			fHeight += vMOVE[i].y * FLOAT(dwTick) / MOVE_RATE;
		}

	m_Camera.Move(
		0.0f,
		GetCamHeight() + fHeight - m_Camera.m_vPosition.y,
		0.0f,
		TRUE);

	CMAPDoc *pDoc = GetDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	if(!pCtrl->m_bPlay)
		Render();

	return 0;
}

LRESULT CMAPView::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	m_dwCamTick = 0;
	return 0;
}

LRESULT CMAPView::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	m_dwCamTick = 0;
	return 0;
}

LRESULT CMAPView::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BYTE CMAPView::IsPlay()
{
	CMAPDoc *pDoc = GetDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	return m_cNAV.m_bPlay || pCtrl->m_bPlay;
}

FLOAT CMAPView::GetCamHeight()
{
	return 0.0f;
}

void CMAPView::OnGridNavigate()
{
	if( m_bViewPort != VIEWPORT_NAVIGATE )
	{
		D3DXVECTOR3 vPosition = m_Camera.m_vPosition;

		m_bViewPort = VIEWPORT_NAVIGATE;
		InitCamera();
		m_Camera.Move(
			vPosition.x,
			0.0f,
			vPosition.z,
			TRUE);

		m_Camera.Move(
			0.0f,
			GetCamHeight() + NAV_INIT_HEIGHT,
			0.0f,
			TRUE);

		if(!IsPlay())
			Render();
	}
}

BOOL CMAPView::MeshHitTest( CTachyonMesh *pMesh,
						    LPD3DXMATRIX pPOS,
							CD3DCamera *pCamera,
							int nPosX,
							int nPosY,
							FLOAT *pDist)
{
	D3DXVECTOR3 vPos = pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDir = pCamera->GetRayDirection( nPosX, nPosY);
	BOOL bResult = FALSE;

	if( !pMesh->m_pDATA || !pMesh->m_pDATA->m_dwCount )
		return bResult;

	LPD3DXVECTOR3 vPoint = new D3DXVECTOR3[pMesh->m_pDATA->m_dwCount];
	if( pMesh->m_dwNodeCount > 0 )
	{
		LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMesh->m_pDATA->m_pVB;

		for( int i=0; i<pMesh->m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);

			CTMath::Transform(
				pPOS,
				&vPoint[i]);
		}
	}
	else
	{
		LPMESHVERTEX pDATA = (LPMESHVERTEX) pMesh->m_pDATA->m_pVB;

		for( int i=0; i<pMesh->m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);

			CTMath::Transform(
				pPOS,
				&vPoint[i]);
		}
	}

	for( int i=0; i<pMesh->m_dwMeshCount; i++)
		for( int j=0; j<INT(pMesh->m_pMESH[i][0]->m_vIB.size()); j++)
			for( int k=0; k<pMesh->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
			{
				D3DXVECTOR3 vCross;

				FLOAT fCurDist = 0.0f;
				FLOAT fU;
				FLOAT fV;

				D3DXVec3Cross(
					&vCross,
					&(vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 1]] - vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k]]),
					&(vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 2]] - vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k]]));

				if( D3DXIntersectTri(
					&vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k]],
					&vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 1]],
					&vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 2]],
					&vPos, &vDir,
					&fU, &fV,
					&fCurDist) &&
					D3DXVec3Dot(
					&vCross,
					&vDir) < 0.0f && (
					!bResult || fCurDist < *pDist))
				{
					*pDist = fCurDist;
					bResult = TRUE;
				}
			}
	delete[] vPoint;

	return bResult;
}

BOOL CMAPView::MeshInRect( CTachyonMesh *pMesh,
						   LPD3DXMATRIX pPOS,
						   CD3DCamera *pCamera,
						   CRect rect)
{
	if( !pMesh->m_pDATA || !pMesh->m_pDATA->m_dwCount )
		return FALSE;

	if( rect.left > rect.right )
	{
		int nBUF = rect.right;

		rect.right = rect.left;
		rect.left = nBUF;
	}

	if( rect.top > rect.bottom )
	{
		int nBUF = rect.bottom;

		rect.bottom = rect.top;
		rect.top = nBUF;
	}

	if( rect.left == rect.right &&
		rect.top == rect.bottom )
	{
		FLOAT fDist = 0.0f;

		return MeshHitTest(
			pMesh,
			pPOS,
			pCamera,
			rect.left,
			rect.top,
			&fDist);
	}

	if( rect.left == rect.right )
		rect.right++;

	if( rect.top == rect.bottom )
		rect.bottom++;

	D3DXVECTOR3 vDIR[4] = {
		pCamera->GetRayDirection( rect.left, rect.top),
		pCamera->GetRayDirection( rect.right, rect.top),
		pCamera->GetRayDirection( rect.right, rect.bottom),
		pCamera->GetRayDirection( rect.left, rect.bottom)};

	for( int i=0; i<4; i++)
		vDIR[i] /= D3DXVec3Length(&vDIR[i]);

	D3DXVECTOR3 vPOS[4][3] = {{
		pCamera->GetCameraPosition( rect.left, rect.top),
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0],
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1]}, {
		pCamera->GetCameraPosition( rect.right, rect.top),
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1],
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2]}, {
		pCamera->GetCameraPosition( rect.right, rect.bottom),
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2],
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3]}, {
		pCamera->GetCameraPosition( rect.left, rect.bottom),
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3],
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0]}};
	D3DXPLANE vPLANE[5];

	for( i=0; i<4; i++)
	{
		D3DXPlaneFromPoints(
			&vPLANE[i],
			&vPOS[i][0],
			&vPOS[i][1],
			&vPOS[i][2]);
	}

	D3DXPlaneFromPointNormal(
		&vPLANE[4],
		&pCamera->m_vPosition,
		&(pCamera->m_vPosition - pCamera->m_vTarget));

	LPD3DXVECTOR3 vPoint = new D3DXVECTOR3[pMesh->m_pDATA->m_dwCount];
	if( pMesh->m_dwNodeCount > 0 )
	{
		LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMesh->m_pDATA->m_pVB;

		for( i=0; i<pMesh->m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);

			CTMath::Transform(
				pPOS,
				&vPoint[i]);
		}
	}
	else
	{
		LPMESHVERTEX pDATA = (LPMESHVERTEX) pMesh->m_pDATA->m_pVB;

		for( i=0; i<pMesh->m_pDATA->m_dwCount; i++)
		{
			vPoint[i] = D3DXVECTOR3(
				pDATA[i].m_fPosX,
				pDATA[i].m_fPosY,
				pDATA[i].m_fPosZ);

			CTMath::Transform(
				pPOS,
				&vPoint[i]);
		}
	}

	for( i=0; i<pMesh->m_dwMeshCount; i++)
		for( int j=0; j<INT(pMesh->m_pMESH[i][0]->m_vIB.size()); j++)
			for( int k=0; k<pMesh->m_pMESH[i][0]->m_vIB[j]->m_dwCount / 3; k++)
			{
				BOOL bResult = TRUE;

				for( int l=0; l<5; l++)
					if( D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k]]) > 0 &&
						D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 1]]) > 0 &&
						D3DXPlaneDotCoord( &vPLANE[l], &vPoint[pMesh->m_pMESH[i][0]->m_vIB[j]->m_pIB[3 * k + 2]]) > 0 )
					{
						bResult = FALSE;
						break;
					}

				if(bResult)
				{
					delete[] vPoint;
					return TRUE;
				}
			}
	delete[] vPoint;

	return FALSE;
}

BOOL CMAPView::PtInRect( LPD3DXVECTOR3 pPoint,
						 CD3DCamera *pCamera,
						 CRect rect)
{
	if( rect.left > rect.right )
	{
		int nBUF = rect.right;

		rect.right = rect.left;
		rect.left = nBUF;
	}

	if( rect.top > rect.bottom )
	{
		int nBUF = rect.bottom;

		rect.bottom = rect.top;
		rect.top = nBUF;
	}

	if( rect.left == rect.right )
		rect.right++;

	if( rect.top == rect.bottom )
		rect.bottom++;

	D3DXVECTOR3 vDIR[4] = {
		pCamera->GetRayDirection( rect.left, rect.top),
		pCamera->GetRayDirection( rect.right, rect.top),
		pCamera->GetRayDirection( rect.right, rect.bottom),
		pCamera->GetRayDirection( rect.left, rect.bottom)};

	for( int i=0; i<4; i++)
		vDIR[i] /= D3DXVec3Length(&vDIR[i]);

	D3DXVECTOR3 vPOS[4][3] = {{
		pCamera->GetCameraPosition( rect.left, rect.top),
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0],
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1]}, {
		pCamera->GetCameraPosition( rect.right, rect.top),
		pCamera->GetCameraPosition( rect.right, rect.top) + vDIR[1],
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2]}, {
		pCamera->GetCameraPosition( rect.right, rect.bottom),
		pCamera->GetCameraPosition( rect.right, rect.bottom) + vDIR[2],
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3]}, {
		pCamera->GetCameraPosition( rect.left, rect.bottom),
		pCamera->GetCameraPosition( rect.left, rect.bottom) + vDIR[3],
		pCamera->GetCameraPosition( rect.left, rect.top) + vDIR[0]}};
	D3DXPLANE vPLANE[5];

	for( i=0; i<4; i++)
	{
		D3DXPlaneFromPoints(
			&vPLANE[i],
			&vPOS[i][0],
			&vPOS[i][1],
			&vPOS[i][2]);
	}

	D3DXPlaneFromPointNormal(
		&vPLANE[4],
		&pCamera->m_vPosition,
		&(pCamera->m_vPosition - pCamera->m_vTarget));

	for( i=0; i<5; i++)
		if( D3DXPlaneDotCoord( &vPLANE[i], pPoint) > 0 )
			return FALSE;

	return TRUE;
}

void CMAPView::OnGridShowEdge()
{
	m_bShowEdge = !m_bShowEdge;
	if(!IsPlay())
		Render();
}

void CMAPView::OnUpdateGridShowEdge(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowEdge);
}

BYTE CMAPView::HitTest(LPD3DXVECTOR3 pResult, LPD3DXVECTOR3 pPOS, LPD3DXVECTOR3 pDIR)
{
	return 0;
}

void CMAPView::OnGroupCommand(BYTE bID, BOOL bSave )
{
	CMAPDoc *pDoc = GetDocument();

	if( bSave )
	{
		pDoc->m_vGROUPOBJ[bID].clear();
		if( !pDoc->m_vSELOBJ.empty() )
		{
			VECTORDWORD::iterator itSELOBJ;
			for( itSELOBJ = pDoc->m_vSELOBJ.begin() ; itSELOBJ != pDoc->m_vSELOBJ.end() ; itSELOBJ++ )
				pDoc->m_vGROUPOBJ[bID].push_back( VECTORDWORD::value_type( (*itSELOBJ) ) );
		}
	}
	else
	{
		pDoc->m_vSELOBJ.clear();
		VECTORDWORD::iterator itGROUPOBJ;
			for( itGROUPOBJ = pDoc->m_vGROUPOBJ[bID].begin() ; itGROUPOBJ != pDoc->m_vGROUPOBJ[bID].end() ; itGROUPOBJ++ )
				pDoc->m_vSELOBJ.push_back( VECTORDWORD::value_type( (*itGROUPOBJ) ) );
	}
	pDoc->RedrawAllView();
}

void CMAPView::OnGroupObjectKey1()
{
	OnGroupCommand(1);		// ID_GROUP_OBJECT_KEY_1	Ctrl + Shift + 1
}

void CMAPView::OnGroupObjectKey2()
{
	OnGroupCommand(2);		// ID_GROUP_OBJECT_KEY_2
}

void CMAPView::OnGroupObjectKey3()
{
	OnGroupCommand(3);		// ID_GROUP_OBJECT_KEY_3
}

void CMAPView::OnGroupObjectKey4()
{
	OnGroupCommand(4);		// ID_GROUP_OBJECT_KEY_4
}

void CMAPView::OnGroupObjectKey5()
{
	OnGroupCommand(5);		// ID_GROUP_OBJECT_KEY_5
}

void CMAPView::OnGroupObjectKey6()
{
	OnGroupCommand(6);		// ID_GROUP_OBJECT_KEY_6
}

void CMAPView::OnGroupObjectKey7()
{
	OnGroupCommand(7);		// ID_GROUP_OBJECT_KEY_7
}

void CMAPView::OnGroupObjectKey8()
{
	OnGroupCommand(8);		// ID_GROUP_OBJECT_KEY_8
}

void CMAPView::OnGroupObjectKey9()
{
	OnGroupCommand(9);		// ID_GROUP_OBJECT_KEY_9
}

void CMAPView::OnGroupObjectKey0()
{
	OnGroupCommand(0);		// ID_GROUP_OBJECT_KEY_0
}

void CMAPView::SelectEqualOBJ(void)	// PSY 06/08/01 Add Func ( Shortening Key (OBJ Mult Select) )
{
	CMAPDoc *pDoc = GetDocument();
	
	if(pDoc)
	{
		VECTORDWORD vEQUALOBJ;
		pDoc->GetEqualOBJ( &vEQUALOBJ, TRUE );
		for( int i = 0 ; i < vEQUALOBJ.size() ; i++ )
		{
			if( !pDoc->IsFreezeOBJ(vEQUALOBJ[i]) && !pDoc->IsHideOBJ(vEQUALOBJ[i]))
				pDoc->m_vSELOBJ.push_back( vEQUALOBJ[i] );
		}

		CMAPOBJBar* pOBJBar = pDoc->GetMAPOBJBar();
		pOBJBar->ResetSELOBJState();				// PSY 06/08/01 Copy Obj Select (Select OBJ OBJBar Pos Text Edit)
		pOBJBar->UpdateData(FALSE);
	}
}

void CMAPView::OnSelectEqualobj()	// PSY 06/08/01 Add Func ( Shortening Key (OBJ Mult Select) )
{
	CMAPDoc *pDoc = GetDocument();
	if( pDoc->m_vSELOBJ.size())
		SelectEqualOBJ();			
	pDoc->RedrawAllView();
}

void CMAPView::OnClearSelobj()		// PSY 06/08/02 Clear Select OBJ Func ( Shortening Key )
{
	CMAPDoc *pDoc = GetDocument();
	pDoc->ClearSELOBJ();
	pDoc->RedrawAllView();
}

