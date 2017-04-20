// OBJView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GridFrame.h"
#include "OBJView.h"
#include "MainFrm.h"


#define BIAS_DEFAULT				0
#define BIAS_HIT					1
#define BIAS_SEL					3


// COBJView

IMPLEMENT_DYNCREATE(COBJView, CGridView)

COBJView::COBJView()
{
	D3DLIGHT9 light;

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.3f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 0.3f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	light.Ambient.a = 1.0f;
	light.Range = T3DLIGHT_RANGE_MAX;
	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	m_LightCenter.SetLight(&light);
	m_LightRight.SetLight(&light);
	m_LightTop.SetLight(&light);

	m_vMove.x = 0.0f;
	m_vMove.y = 0.0f;
	m_vMove.z = 0.0f;
}

COBJView::~COBJView()
{
}

BEGIN_MESSAGE_MAP(COBJView, CGridView)
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// COBJView 진단입니다.

#ifdef _DEBUG
void COBJView::AssertValid() const
{
	CGridView::AssertValid();
}

void COBJView::Dump(CDumpContext& dc) const
{
	CGridView::Dump(dc);
}
#endif //_DEBUG


// COBJView 메시지 처리기입니다.

void COBJView::DrawContent()
{
	CGridView::DrawContent();

	COBJDoc *pDoc = GetDocument();
	COBJBar *pBar = pDoc->GetOBJBar();

	if(pDoc)
	{
		MAPACTINST::iterator itAI = pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapACT.find(pDoc->m_pItem->m_TOBJ.m_OBJ.m_dwCurACT);
		MAPTSFX::iterator itSFX;
		LPANI pANI = NULL;

		if( itAI != pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapACT.end() )
			pANI = (*itAI).second->m_pANI;

		m_Camera.Move(
			pDoc->m_pItem->m_TOBJ.m_vPosition._41 - m_vMove.x,
			pDoc->m_pItem->m_TOBJ.m_vPosition._42 - m_vMove.y,
			pDoc->m_pItem->m_TOBJ.m_vPosition._43 - m_vMove.z,
			TRUE);

		if( ::GetFocus() == GetSafeHwnd() )
		{
			CD3DSound::ResetLISTENER(
				&m_Camera.m_vPosition,
				&(m_Camera.m_vTarget - m_Camera.m_vPosition),
				&m_Camera.m_vUp);
		}

		m_vMove.x = pDoc->m_pItem->m_TOBJ.m_vPosition._41;
		m_vMove.y = pDoc->m_pItem->m_TOBJ.m_vPosition._42;
		m_vMove.z = pDoc->m_pItem->m_TOBJ.m_vPosition._43;

		if( !pBar->m_bEditPivot && !pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapDRAW.empty() )
		{
			FLOAT fDist = D3DXVec3Length(&D3DXVECTOR3(
				pDoc->m_pItem->m_TOBJ.m_vPosition._41 - m_Camera.m_vPosition.x,
				pDoc->m_pItem->m_TOBJ.m_vPosition._42 - m_Camera.m_vPosition.y,
				pDoc->m_pItem->m_TOBJ.m_vPosition._43 - m_Camera.m_vPosition.z));

			int nIndex = pBar->m_cMI.GetCurSel();
			DWORD dwNodeCount = 0;

			LPMESH pMESH = pBar->GetCurMESH();
			LPD3DXMATRIX pInit = NULL;

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
			EnableLight();

			MAPOBJECT::iterator itOBJ;
			for( itOBJ = pDoc->m_pItem->m_TOBJ.m_mapEQUIP.begin(); itOBJ != pDoc->m_pItem->m_TOBJ.m_mapEQUIP.end(); itOBJ++)
			{
				CTachyonObject *pEQUIP = (*itOBJ).second;

				if(pEQUIP)
				{
					pEQUIP->m_dwTexTick = pDoc->m_pItem->m_TOBJ.m_dwTexTick;
					pEQUIP->m_fActTime = pDoc->m_pItem->m_TOBJ.m_fActTime;
					pEQUIP->CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);
				}
			}

			if(pDoc->m_pItem->m_TOBJ.m_bUseSHADER)
			{
				static int vLightCount[4] = { 3, 0, 1, 0};

				CD3DLight *pLIGHT[3] = {
					&m_LightCenter,
					&m_LightRight,
					&m_LightTop};

				FLOAT vCONST[16] = {
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
				D3DMATERIAL9 mtrl;

				CMainFrame::m_3DDevice.m_pDevice->SetPixelShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantPS[PC_COMMON],
					&vCONST[4], 2);

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_COMMON],
					vCONST, 1);

				CMainFrame::m_3DDevice.m_pDevice->GetMaterial(&mtrl);
				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_MTRLAMBIENT],
					(FLOAT *) &D3DXVECTOR4(
					mtrl.Ambient.r,
					mtrl.Ambient.g,
					mtrl.Ambient.b,
					mtrl.Ambient.a), 1);

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_MTRLDIFFUSE],
					(FLOAT *) &D3DXVECTOR4(
					mtrl.Diffuse.r,
					mtrl.Diffuse.g,
					mtrl.Diffuse.b,
					mtrl.Diffuse.a), 1);

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_AMBIENT],
					(FLOAT *) &D3DXVECTOR4(
					0.0f,
					0.0f,
					0.0f,
					1.0f), 1);

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_CAMPOS],
					(FLOAT *) &D3DXVECTOR4(
					m_Camera.m_vPosition.x,
					m_Camera.m_vPosition.y,
					m_Camera.m_vPosition.z,
					0.0f), 1);

				D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(m_Camera.m_matView * m_Camera.m_matProjection));
				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_PROJ],
					vCONST, 4);

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantI(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_LIGHTCOUNT],
					vLightCount, 1);

				for( int i=0; i<3; i++)
				{
					memcpy( &vCONST[i * 4], &D3DXVECTOR4(
						pLIGHT[i]->m_Light.Ambient.r,
						pLIGHT[i]->m_Light.Ambient.g,
						pLIGHT[i]->m_Light.Ambient.b,
						pLIGHT[i]->m_Light.Ambient.a),
						4 * sizeof(FLOAT));
				}

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_LIGHTAMBIENT],
					vCONST, 3);

				for( i=0; i<3; i++)
				{
					memcpy( &vCONST[i * 4], &D3DXVECTOR4(
						pLIGHT[i]->m_Light.Diffuse.r,
						pLIGHT[i]->m_Light.Diffuse.g,
						pLIGHT[i]->m_Light.Diffuse.b,
						pLIGHT[i]->m_Light.Diffuse.a),
						4 * sizeof(FLOAT));
				}

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_LIGHTDIFFUSE],
					vCONST, 3);

				for( i=0; i<3; i++)
				{
					D3DXVECTOR3 vDIR(
						pLIGHT[i]->m_Light.Direction.x,
						pLIGHT[i]->m_Light.Direction.y,
						pLIGHT[i]->m_Light.Direction.z);
					vDIR /= D3DXVec3Length(&vDIR);

					memcpy( &vCONST[i * 4], &D3DXVECTOR4(
						vDIR.x,
						vDIR.y,
						vDIR.z, 0.0f),
						4 * sizeof(FLOAT));
				}

				CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
					CMainFrame::m_3DDevice.m_vConstantVS[VC_LIGHTDIR],
					vCONST, 3);
			}

			pDoc->m_pItem->m_TOBJ.Render(
				&CMainFrame::m_3DDevice,
				&m_Camera);

			if( pDoc->m_vHIT.m_pMESH && pDoc->m_vHIT.m_pMESH->m_pMESH )
			{
				dwNodeCount = pDoc->m_vHIT.m_pMESH->m_pMESH->m_dwNodeCount;
				pInit = pDoc->m_vHIT.m_pMESH->m_pMESH->m_pBones;
			}
			else if( pMESH && pMESH->m_pMESH )
			{
				dwNodeCount = pMESH->m_pMESH->m_dwNodeCount;
				pInit = pMESH->m_pMESH->m_pBones;
			}

			if( pANI && pANI->m_pANI && pInit )
			{
				CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLDMATRIX(0), &pDoc->m_pItem->m_TOBJ.m_pBone[0]);
				for( int i=0; i<dwNodeCount; i++)
					CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLDMATRIX(i + 1), &(pInit[i] * pDoc->m_pItem->m_TOBJ.m_pBone[i + 1]));
			}
			else
				CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &pDoc->m_pItem->m_TOBJ.m_vPosition);

			if( pDoc->m_vHIT.m_pMESH && pDoc->m_vHIT.m_pMESH->m_pMESH && pDoc->m_vHIT.m_nIndex != -1 )
			{
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0x8000FF00);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_HIT);

				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_CULLMODE, pDoc->m_vHIT.m_pMESH->m_vTEX[pDoc->m_vHIT.m_nIndex]->m_b2side ? D3DCULL_NONE : D3DCULL_CCW);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, pDoc->m_vHIT.m_pMESH->m_vTEX[pDoc->m_vHIT.m_nIndex]->m_bZWrite);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, pDoc->m_vHIT.m_pMESH->m_vTEX[pDoc->m_vHIT.m_nIndex]->m_bZEnable);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, pDoc->m_vHIT.m_pMESH->m_pMESH->m_dwNodeCount && pANI && pANI->m_pANI ? D3DVBF_3WEIGHTS : D3DVBF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetFVF( pDoc->m_vHIT.m_pMESH->m_pMESH->m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);

				pDoc->m_vHIT.m_pMESH->m_pMESH->Render( CMainFrame::m_3DDevice.m_pDevice, pDoc->m_vHIT.m_nIndex, pDoc->m_vHIT.m_pMESH->m_pMESH->GetLevel(fDist));
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			}
			DisableLight();

			if( pBar->m_bShowSelected && pMESH && pMESH->m_pMESH && nIndex != -1 )
			{
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

				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, pMESH->m_pMESH->m_dwNodeCount && pANI && pANI->m_pANI ? D3DVBF_3WEIGHTS : D3DVBF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetFVF( pMESH->m_pMESH->m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);

				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, pBar->m_nSTG == 0 ? 0xFFFFFFFF : 0xFF80FF00);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SEL);

				pMESH->m_pMESH->Render( CMainFrame::m_3DDevice.m_pDevice, nIndex, pMESH->m_pMESH->GetLevel(fDist));
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
				CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
			}

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
		}
		else if( pANI && pANI->m_pANI )
		{
			pANI->m_pANI->m_fCurTime = pDoc->m_pItem->m_TOBJ.m_fActTime;
			LPANIDATA pDATA = pANI->m_pANI->GetAniData();
			MAPDWORD::iterator itID = pDoc->m_pItem->m_mapANI.find((*itAI).second->m_dwCurANI);

			if( pDATA && itID != pDoc->m_pItem->m_mapANI.end() )
			{
				CAnimationItem *pAniItem = CItemBase::GetAnimationItem((*itID).second);

				if(pAniItem)
				{
					pAniItem = pAniItem->GetAniItem(pDATA);

					if(pAniItem)
					{
						LPD3DXMATRIX vLocal = pDoc->m_pItem->m_TOBJ.m_pBone;
						int nPivotCount = pDoc->m_pItem->GetPivotCount();
						EnableLight();

						pDATA->m_pAni->GetFrameMatrix(
							vLocal,
							NULL,
							NULL,
							NULL,
							pDoc->m_pItem->m_TOBJ.m_vPosition, 0,
							pANI->m_pANI->m_fLocalTime,
							0.0f);

						MAPOBJECT::iterator itOBJ;
						for( itOBJ = pDoc->m_pItem->m_TOBJ.m_mapEQUIP.begin(); itOBJ != pDoc->m_pItem->m_TOBJ.m_mapEQUIP.end(); itOBJ++)
						{
							CTachyonObject *pEQUIP = (*itOBJ).second;

							if(pEQUIP)
							{
								pEQUIP->m_dwTexTick = pDoc->m_pItem->m_TOBJ.m_dwTexTick;
								pEQUIP->m_fActTime = pDoc->m_pItem->m_TOBJ.m_fActTime;
								pEQUIP->CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);

								pEQUIP->m_vPosition = pDoc->m_pItem->m_TOBJ.m_pBone[(*itOBJ).first] * pEQUIP->m_vPosition;
								pEQUIP->Render( &CMainFrame::m_3DDevice, &m_Camera);
							}
						}

						pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vOBJSFX);
						pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vACTSFX);
						pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vANISFX);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFF0F0FF);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
						CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
						CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_MESHVERTEX);

						for( int i=0; i<pDATA->m_pAni->GetNodeCount(); i++)
						{
							CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vLocal[i + 1]);
							pAniItem->Render( CMainFrame::m_3DDevice.m_pDevice, i);
						}

						if( nPivotCount > 0 && pBar->m_nPIVOT != -1 )
						{
							CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vLocal[pBar->m_nPIVOT + 1]);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFD0FFD0);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_HIT);
							pAniItem->Render( CMainFrame::m_3DDevice.m_pDevice, pBar->m_nPIVOT);
							DisableLight();

							CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
							CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
							CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

							CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
							CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SEL);
							pAniItem->Render( CMainFrame::m_3DDevice.m_pDevice, pBar->m_nPIVOT);

							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
						}
						else
							DisableLight();

						CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
					}
				}
			}

			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vOBJSFX);
			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vACTSFX);
			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vANISFX);
		}
		else
		{
			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vOBJSFX);
			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vACTSFX);
			pDoc->m_pItem->m_TOBJ.CalcSFX( &CMainFrame::m_3DDevice, &pDoc->m_pItem->m_TOBJ.m_vANISFX);
		}
		EnableLight();

		for( itSFX = CTachyonSFX::m_mapINST.begin(); itSFX != CTachyonSFX::m_mapINST.end(); itSFX++)
			if( (*itSFX).second->m_pHost == pBar && !(*itSFX).second->IsDead() )
			{
				if( (*itSFX).second->m_dwInitTick != INVALID_INIT_TICK )
					(*itSFX).second->CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);
				(*itSFX).second->Render( &CMainFrame::m_3DDevice, &m_Camera);
			}
		DisableLight();
	}
}

void COBJView::OnInitialUpdate()
{
	CGridView::OnInitialUpdate();
	m_DropTarget.Register(this);

	COBJDoc *pDoc = GetDocument();
	pDoc->m_pItem->m_TOBJ.CalcFrame(FALSE);

	D3DXVECTOR3 vMove(
		pDoc->m_pItem->m_TOBJ.m_vPosition._41,
		pDoc->m_pItem->m_TOBJ.m_vPosition._42,
		pDoc->m_pItem->m_TOBJ.m_vPosition._43);

	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	if(pDoc->m_pItem->GetSize( vMin, vMax))
		vMove += (vMin + vMax) / 2.0f;

	m_Camera.Move(
		vMove.x,
		vMove.y,
		vMove.z,
		TRUE);
}

void COBJView::InitCamera()
{
	COBJDoc *pDoc = GetDocument();

	m_fGrid = 1.0f;
	CGridView::InitCamera();

	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	if(pDoc->m_pItem->GetSize( vMin, vMax))
	{
		FLOAT fHeight = max( vMax.x - vMin.x, vMax.y - vMin.y);
		CRect rect;

		fHeight = max( vMax.z - vMin.z, fHeight);
		GetClientRect(&rect);

		fHeight = fHeight * CMainFrame::m_3DDevice.m_option.m_dwScreenY / rect.Height() * 1.2f;
		FitCamDist(fHeight);
	}
}

void COBJView::OnGridCamreset()
{
	COBJDoc *pDoc = GetDocument();
	pDoc->m_pItem->m_TOBJ.CalcFrame(FALSE);

	D3DXVECTOR3 vMove(
		pDoc->m_pItem->m_TOBJ.m_vPosition._41,
		pDoc->m_pItem->m_TOBJ.m_vPosition._42,
		pDoc->m_pItem->m_TOBJ.m_vPosition._43);
	InitCamera();

	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	if(pDoc->m_pItem->GetSize( vMin, vMax))
		vMove += (vMin + vMax) / 2.0f;

	m_Camera.Move(
		vMove.x,
		vMove.y,
		vMove.z,
		TRUE);

	if(!IsPlay())
		Render();
}

BYTE COBJView::IsPlay()
{
	COBJDoc *pDoc = GetDocument();
	return pDoc->GetPlayCtrl()->m_bPlay;
}

BYTE COBJView::CanDrop( CItemBase *pItem)
{
	COBJDoc *pDoc = GetDocument();
	COBJBar *pBar = pDoc->GetOBJBar();

	return !pBar->m_bEditPivot &&
		!pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapDRAW.empty() &&
		!IsPlay() && pItem->m_bType == ITEM_TEXTURE ? TRUE : FALSE;
}

DROPEFFECT COBJView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return COBJView::OnDragOver( pDataObject, dwKeyState, point);
}

void COBJView::OnDragLeave()
{
	COBJDoc *pDoc = GetDocument();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pDoc->m_vHIT.m_pMESH = NULL;
	pDoc->m_vHIT.m_nIndex = -1;
	pDoc->m_vHIT.m_dwMESH = 0;
	pDoc->m_vHIT.m_dwCLK = 0;
	pDoc->m_vHIT.m_dwCL = 0;

	if(!IsPlay())
		pDoc->RedrawAllView();

	CGridView::OnDragLeave();
}

DROPEFFECT COBJView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT nResult = DROPEFFECT_NONE;

	if(!IsPlay())
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
		CPoint screenpos = point;

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
		case GSCT_TEX		:
			{
				CItemBase *pItem = (CItemBase *) pData->m_pData;
				COBJDoc *pDoc = GetDocument();
				CRect rect;

				GetClientRect(&rect);
				if(CanDrop(pItem))
				{
					OBJHIT vHIT = pDoc->m_pItem->m_TOBJ.HitTest(
						&m_Camera,
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

					if( vHIT.m_nIndex != -1 )
						nResult = DROPEFFECT_MOVE;

					if( pDoc->m_vHIT.m_nIndex != vHIT.m_nIndex ||
						pDoc->m_vHIT.m_dwMESH != vHIT.m_dwMESH ||
						pDoc->m_vHIT.m_dwCLK != vHIT.m_dwCLK ||
						pDoc->m_vHIT.m_dwCL != vHIT.m_dwCL )
					{
						pDoc->m_vHIT.m_dwMESH = vHIT.m_dwMESH;
						pDoc->m_vHIT.m_dwCLK = vHIT.m_dwCLK;
						pDoc->m_vHIT.m_dwCL = vHIT.m_dwCL;

						pDoc->m_vHIT.m_nIndex = vHIT.m_nIndex;
						pDoc->m_vHIT.m_pMESH = vHIT.m_pMESH;

						if( vHIT.m_nIndex != -1 )
						{
							CGameStudioView::m_pDragImage->DragMove( CPoint(
								-10000,
								-10000));

							pDoc->GetOBJBar()->SetCurSel(
								vHIT.m_dwCLK,
								vHIT.m_dwCL,
								vHIT.m_dwMESH,
								vHIT.m_nIndex);

							pDoc->RedrawAllView();
						}
						else if(!IsPlay())
						{
							CGameStudioView::m_pDragImage->DragMove( CPoint(
								-10000,
								-10000));

							pDoc->RedrawAllView();
						}
					}
				}
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);

		ClientToScreen(&screenpos);
		CGameStudioView::m_pDragImage->DragMove(screenpos);
	}

	return nResult;
}

BOOL COBJView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if(!IsPlay())
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
		case GSCT_TEX		:
			{
				CItemBase *pItem = (CItemBase *) pData->m_pData;
				COBJDoc *pDoc = GetDocument();
				CRect rect;

				GetClientRect(&rect);
				if(CanDrop(pItem))
				{
					pDoc->m_vHIT = pDoc->m_pItem->m_TOBJ.HitTest(
						&m_Camera,
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

					if( pDoc->m_vHIT.m_nIndex != -1 )
						InsertTexture((CTextureItem *) pItem);
				}
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}

	OnDragLeave();
	return TRUE;
}

void COBJView::InsertTexture( CTextureItem *pItem)
{
	COBJDoc *pDoc = GetDocument();

	if( pDoc && pDoc->m_vHIT.m_dwCLK != 0 &&
		pDoc->m_vHIT.m_dwCL != 0 &&
		pDoc->m_vHIT.m_dwMESH != 0 &&
		pDoc->m_vHIT.m_nIndex != -1 &&
		pDoc->m_vHIT.m_pMESH )
	{
		COBJBar *pBar = pDoc->GetOBJBar();
		DWORD dwID = pDoc->m_pItem->GetTexID(
			pDoc->m_vHIT.m_dwMESH,
			pDoc->m_vHIT.m_nIndex,
			pBar->m_nSTG);

		pItem->Regulate();
		if( dwID == 0 )
		{
			LPTEXINDEX pTI = new TEXINDEX();

			dwID = ROOT_TEXID;
			while( pDoc->m_pItem->m_mapTEX.find(dwID) != pDoc->m_pItem->m_mapTEX.end() )
				dwID++;

			pTI->m_dwMeshID = pDoc->m_vHIT.m_dwMESH;
			pTI->m_nIndex = pDoc->m_vHIT.m_nIndex;
			pTI->m_nStage = pBar->m_nSTG;

			pDoc->m_pItem->m_mapTEX.insert( MAPDWORD::value_type( dwID, pItem->m_dwID));
			pDoc->m_pItem->m_mapTI.insert( MAPTEXINDEX::value_type( dwID, pTI));
		}
		else
		{
			MAPDWORD::iterator itTEX = pDoc->m_pItem->m_mapTEX.find(dwID);

			if( itTEX != pDoc->m_pItem->m_mapTEX.end() )
				(*itTEX).second = pItem->m_dwID;
			else
				pDoc->m_pItem->m_mapTEX.insert( MAPDWORD::value_type( dwID, pItem->m_dwID));
		}

		pDoc->m_vHIT.m_pMESH->m_vTEX[pDoc->m_vHIT.m_nIndex]->m_pTEX[pBar->m_nSTG] = pItem->GetTEXTURESET();
		pDoc->UpdateAll();

		pBar->SetCurSel(
			pDoc->m_vHIT.m_dwCLK,
			pDoc->m_vHIT.m_dwCL,
			pDoc->m_vHIT.m_dwMESH,
			pDoc->m_vHIT.m_nIndex);
	}
}

void COBJView::RemoveTexture( LPMESH pMESH, DWORD dwID)
{
	COBJDoc *pDoc = GetDocument();

	MAPTEXINDEX::iterator itTI = pDoc->m_pItem->m_mapTI.find(dwID);
	MAPDWORD::iterator itTEX = pDoc->m_pItem->m_mapTEX.find(dwID);

	if( itTEX != pDoc->m_pItem->m_mapTEX.end() )
		pDoc->m_pItem->m_mapTEX.erase(itTEX);

	if( itTI != pDoc->m_pItem->m_mapTI.end() )
	{
		LPTEXINDEX pTI = (*itTI).second;

		if( pTI->m_nIndex != -1 && pTI->m_dwMeshID != 0 )
			pMESH->m_vTEX[pTI->m_nIndex]->m_pTEX[pTI->m_nStage] = NULL;

		pDoc->m_pItem->m_mapTI.erase(itTI);
		delete pTI;
	}

	pDoc->UpdateAll();
	pDoc->RedrawAllView();
}

void COBJView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CGridView::OnLButtonDown( nFlags, point);
	COBJDoc *pDoc = GetDocument();
	COBJBar *pBar = pDoc->GetOBJBar();

	if( !IsPlay() && m_bCamMode == CM_NONE )
		if( !pBar->m_bEditPivot && !pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapDRAW.empty() )
		{
			CRect rect;

			GetClientRect(&rect);
			OBJHIT vHIT = pDoc->m_pItem->m_TOBJ.HitTest(
				&m_Camera,
				point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

			if( vHIT.m_nIndex != -1 )
			{
				DWORD dwID = pDoc->m_pItem->GetTexID(
					vHIT.m_dwMESH,
					vHIT.m_nIndex,
					pBar->m_nSTG);
				MAPDWORD::iterator itTEX = pDoc->m_pItem->m_mapTEX.find(dwID);

				ReleaseCapture();
				pBar->SetCurSel(
					vHIT.m_dwCLK,
					vHIT.m_dwCL,
					vHIT.m_dwMESH,
					vHIT.m_nIndex);

				if( itTEX != pDoc->m_pItem->m_mapTEX.end() )
				{
					CTextureItem *pItem = CItemBase::GetTextureItem((*itTEX).second);

					if(pItem)
					{
						RemoveTexture( vHIT.m_pMESH, dwID);

						HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
						COleDataSource source;

						CGameStudioView::m_pDragImage = new CImageList();
						LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
						pData->m_pData = (LPVOID) pItem;
						pData->m_bType = GSCT_TEX;
						::GlobalUnlock(hGlobal);

						source.CacheGlobalData( CGameStudioView::m_cfGS, hGlobal);
						if( source.DoDragDrop() == DROPEFFECT_NONE )
						{
							GetCursorPos(&point);
							ScreenToClient(&point);

							pDoc->m_vHIT = pDoc->m_pItem->m_TOBJ.HitTest(
								&m_Camera,
								point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
								point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

							if( pDoc->m_vHIT.m_dwMESH == vHIT.m_dwMESH &&
								pDoc->m_vHIT.m_dwCLK == vHIT.m_dwCLK &&
								pDoc->m_vHIT.m_dwCL == vHIT.m_dwCL &&
								pDoc->m_vHIT.m_nIndex == vHIT.m_nIndex &&
								pDoc->m_vHIT.m_pMESH == vHIT.m_pMESH )
								InsertTexture(pItem);

							pDoc->m_vHIT.m_dwMESH = 0;
							pDoc->m_vHIT.m_dwCLK = 0;
							pDoc->m_vHIT.m_dwCL = 0;

							pDoc->m_vHIT.m_pMESH = NULL;
							pDoc->m_vHIT.m_nIndex = -1;

							pDoc->RedrawAllView();
						}

						::GlobalFree(hGlobal);
						CGameStudioView::InitDragResource();
					}
				}
			}
		}
		else
		{
			MAPACTINST::iterator itAI = pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapACT.find(pDoc->m_pItem->m_TOBJ.m_OBJ.m_dwCurACT);
			LPANI pANI = NULL;

			if( itAI != pDoc->m_pItem->m_TOBJ.m_OBJ.m_mapACT.end() )
				pANI = (*itAI).second->m_pANI;

			if( pANI && pANI->m_pANI )
			{
				pANI->m_pANI->m_fCurTime = pDoc->m_pItem->m_TOBJ.m_fActTime;
				LPANIDATA pDATA = pANI->m_pANI->GetAniData();

				if(pDATA)
				{
					int nPivotCount = pDoc->m_pItem->GetPivotCount();
					static D3DXMATRIX vLocal[255];

					if( nPivotCount > 0 )
					{
						CRect rect;

						pDATA->m_pAni->GetFrameMatrix(
							vLocal,
							NULL,
							NULL,
							NULL,
							pDoc->m_pItem->m_TOBJ.m_vPosition, 0,
							pANI->m_pANI->m_fLocalTime,
							0.0f);
						GetClientRect(&rect);

						int nIndex = pDATA->m_pAni->HitTest(
							vLocal,
							&m_Camera,
							point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

						if( nIndex != -1 )
						{
							if(pBar->GetCurSFX())
							{
								pBar->m_cEPivotID.SetCurSel(nIndex + 1);
								pBar->OnCbnSelchangeComboOep();
							}

							pBar->m_cPIVOT.SetCurSel(nIndex);
							pBar->OnCbnSelchangeComboPivot();
						}
					}
				}
			}
		}
}
