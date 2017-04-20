// PreviewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"
#include "PreviewDlg.h"
#include "MainFrm.h"
#include "Gizimo.h"

CPreviewCtrl::CPreviewCtrl()
{
	D3DLIGHT9 light;

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.2f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 0.5f;
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

	m_LightCenter.Rotate( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), -D3DX_PI / 6.0f);
	m_LightRight.Rotate( &D3DXVECTOR3( 0.0f, 1.0f, 0.0f), -D3DX_PI / 4.0f);
	m_LightTop.Rotate( &D3DXVECTOR3( 1.0f, 0.0f, 0.0f), D3DX_PI / 4.0f);

	m_pTextureVB = NULL;
	m_pItem = NULL;
}

CPreviewCtrl::~CPreviewCtrl()
{
}

void CPreviewCtrl::DrawItem( LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
}

BEGIN_MESSAGE_MAP(CPreviewCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPreviewCtrl::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CPreviewCtrl::SetCamera( D3DXVECTOR3& vCenter,
							  FLOAT fWidth,
							  FLOAT fHeight,
							  FLOAT fDist)
{
	CD3DCamera vCamera;
	CRect rect;
	GetClientRect(&rect);

	if( ((FLOAT) rect.Width()) * fHeight / ((FLOAT) rect.Height()) < fWidth )
		fHeight = ((FLOAT) rect.Height()) * fWidth / ((FLOAT) rect.Width());
	else
		fWidth = ((FLOAT) rect.Width()) * fHeight / ((FLOAT) rect.Height());

	fDist = max( fDist, fHeight);
	fDist = max( fDist, fWidth);

	vCamera.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		-2.0f * fDist,
		2.0f * fDist,
		fWidth,
		fHeight);

	vCamera.SetPosition(
		D3DXVECTOR3( vCenter.x, vCenter.y, vCenter.z - fDist),
		vCenter,
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
		FALSE);
	vCamera.Activate(TRUE);
}

void CPreviewCtrl::DrawImage( CD3DImage *pImage,
							  int nPosX,
							  int nPosY,
							  int nWidth,
							  int nHeight)
{
	FLOAT fRatio = ((FLOAT) nWidth) / ((FLOAT) pImage->GetWidth());
	int nCount = pImage->GetPartCount();

	CT3DTexture **pT3DTEX = pImage->GetT3DTEX();
	CT3DVertex **pT3DVB = pImage->GetT3DVB();
	LPDIRECT3DVERTEXBUFFER9 pIMG = NULL;

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		4 * sizeof(TVERTEX),
		0, T3DFVF_TVERTEX,
		D3DPOOL_MANAGED,
		&pIMG, NULL);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TVERTEX);

	for( int i=0; i<nCount; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 pVB = pT3DVB[i]->GetVB();

		LPTVERTEX pDEST = NULL;
		LPTVERTEX pSRC = NULL;

		pIMG->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pDEST, 0);
		pVB->Lock( 0, 4 * sizeof(TVERTEX), (LPVOID *) &pSRC, 0);

		for( int j=0; j<4; j++)
		{
			pDEST[j].m_fPosX = ((FLOAT) nPosX) + (pSRC[j].m_fPosX - ((FLOAT) pImage->GetPosX())) * fRatio;
			pDEST[j].m_fPosY = ((FLOAT) nPosY) + (pSRC[j].m_fPosY - ((FLOAT) pImage->GetPosY())) * fRatio;
			pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;

			pDEST[j].m_fRHW = pSRC[j].m_fRHW;
			pDEST[j].m_fU = pSRC[j].m_fU;
			pDEST[j].m_fV = pSRC[j].m_fV;
		}

		pIMG->Unlock();
		pVB->Unlock();

		CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, pIMG, 0, sizeof(TVERTEX));
		CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pT3DTEX[i]->GetTEX());
		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	pIMG->Release();
}

void CPreviewCtrl::DrawLOGO( CDC *pDC)
{
	CRect rect;
	CDC dcBMP;
	CDC dcMEM;

	CBitmap bmpLOGO;
	CBitmap bmpMEM;
	BITMAP bmp;

	dcBMP.CreateCompatibleDC(pDC);
	dcMEM.CreateCompatibleDC(pDC);
	GetClientRect(&rect);

	bmpMEM.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height());
	bmpLOGO.LoadBitmap(IDB_BITMAP_PREVIEW);
	bmpLOGO.GetBitmap(&bmp);

	CBitmap *pOLDBMP = dcBMP.SelectObject(&bmpLOGO);
	CBitmap *pOLDMEM = dcMEM.SelectObject(&bmpMEM);

	dcMEM.FillSolidRect( &rect, RGB( 255, 255, 255));
	dcMEM.BitBlt(
		(rect.Width() - bmp.bmWidth) / 2,
		(rect.Height() - bmp.bmHeight) / 2,
		bmp.bmWidth,
		bmp.bmHeight,
		&dcBMP,
		0, 0,
		SRCCOPY);

	pDC->BitBlt(
		0, 0,
		rect.Width(),
		rect.Height(),
		&dcMEM,
		0, 0,
		SRCCOPY);

	dcMEM.SelectObject(pOLDMEM);
	dcBMP.SelectObject(pOLDBMP);

	bmpLOGO.DeleteObject();
	bmpMEM.DeleteObject();

	dcMEM.DeleteDC();
	dcBMP.DeleteDC();
}

void CPreviewCtrl::DrawMesh()
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 255, 255, 255),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
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

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	CMeshItem *pItem = (CMeshItem *) m_pItem;
	SetCamera(
		pItem->m_Mesh.m_vCenterPoint,
		pItem->m_Mesh.m_fRadius * 2.2f,
		pItem->m_Mesh.m_fRadius * 2.2f,
		pItem->m_Mesh.m_fRadius * 2.2f);

	m_LightCenter.SetLight(&m_LightCenter.m_Light);
	m_LightRight.SetLight(&m_LightRight.m_Light);
	m_LightTop.SetLight(&m_LightTop.m_Light);

	m_LightCenter.EnableLight();
	m_LightRight.EnableLight();
	m_LightTop.EnableLight();

	CMainFrame::m_3DDevice.m_pDevice->SetFVF(pItem->m_Mesh.m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &mat);

	for( DWORD i=0; i<pItem->m_Mesh.m_dwMeshCount; i++)
		pItem->m_Mesh.Render( CMainFrame::m_3DDevice.m_pDevice, i, 0);

	m_LightCenter.EnableLight(FALSE);
	m_LightRight.EnableLight(FALSE);
	m_LightTop.EnableLight(FALSE);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			NULL,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CPreviewCtrl::DrawOBJ()
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 160, 160, 160),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	/////////
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	//CGizimo::Instance()->SetGizimo( GizimoTranslate::Instance() );
	//CGizimo::Instance()->SetItem( m_pItem );
	//CGizimo::Instance()->Update();
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	/////////

	COBJItem *pItem = (COBJItem *) m_pItem;
	CTachyonObject vTOBJ;
	vTOBJ.InitOBJ(&pItem->m_OBJ);

	if(!pItem->m_OBJ.m_mapCLKIND.empty())
	{
		MAPCLKIND::iterator itCLK = pItem->m_OBJ.m_mapCLKIND.begin();
		LPCLKIND pTCLK = (*itCLK).second;

		if( pTCLK && !pTCLK->m_mapCLOTH.empty() )
		{
			MAPCLOTH::iterator itCL = pTCLK->m_mapCLOTH.begin();
			LPCLOTH pCL = (*itCL).second;

			if( pCL && !pCL->m_mapMESH.empty() )
			{
				MAPMESH::iterator itMESH = pCL->m_mapMESH.begin();
				LPMESH pMESH = (*itMESH).second;

				if(pMESH)
				{
					vTOBJ.SetCloth(
						&CMainFrame::m_3DDevice,
						(*itCLK).first,
						(*itCL).first,
						(*itMESH).first);
					vTOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
				}
			}
		}
	}

	D3DXVECTOR3 vCENTER;
	D3DXVECTOR3 vMIN;
	D3DXVECTOR3 vMAX;

	FLOAT fHEIGHT;
	FLOAT fWIDTH;
	FLOAT fDIST;

	vTOBJ.CalcTick( CMainFrame::m_3DDevice.m_pDevice, 0);
	vTOBJ.GetSize(
		vMIN,
		vMAX);

	vCENTER = (vMIN + vMAX) / 2.0f;
	vMAX = vMAX - vMIN;

	fHEIGHT = 2.2f * max( fabs(vMAX.x), max( fabs(vMAX.y), fabs(vMAX.z))) / 2.0f;
	fWIDTH = fHEIGHT;
	fDIST = fWIDTH;


	CD3DCamera vCamera;
	CRect rect;
	GetClientRect(&rect);

	if( ((FLOAT) rect.Width()) * fHEIGHT / ((FLOAT) rect.Height()) < fWIDTH )
		fHEIGHT = ((FLOAT) rect.Height()) * fWIDTH / ((FLOAT) rect.Width());
	else
		fWIDTH = ((FLOAT) rect.Width()) * fHEIGHT / ((FLOAT) rect.Height());

	fDIST = max( fDIST, fHEIGHT);
	fDIST = max( fDIST, fWIDTH);

	vCamera.InitOrthoCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		-2.0f * fDIST,
		2.0f * fDIST,
		fWIDTH,
		fHEIGHT);

	vCamera.SetPosition(
		D3DXVECTOR3( vCENTER.x + (fDIST * 0.5f), vCENTER.y + (fDIST * 0.5f), vCENTER.z - fDIST),
		vCENTER,
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
		FALSE);
	vCamera.Activate(TRUE);

	m_LightCenter.SetLight(&m_LightCenter.m_Light);
	m_LightRight.SetLight(&m_LightRight.m_Light);
	m_LightTop.SetLight(&m_LightTop.m_Light);

	m_LightCenter.EnableLight();
	m_LightRight.EnableLight();
	m_LightTop.EnableLight();

	vTOBJ.Render(
		&CMainFrame::m_3DDevice,
		&vCamera);
	vTOBJ.Release();

	m_LightCenter.EnableLight(FALSE);
	m_LightRight.EnableLight(FALSE);
	m_LightTop.EnableLight(FALSE);

	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			NULL,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CPreviewCtrl::DrawAnimation()
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 255, 255, 255),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

	CAnimationItem *pItem = (CAnimationItem *) m_pItem;
	if( !pItem->GetAnimation().IsEnable() && !pItem->m_vAniID.empty() )
		pItem = CItemBase::GetAnimationItem(pItem->m_vAniID[0]);

	if( pItem && pItem->GetAnimation().IsEnable() )
	{
		D3DXVECTOR3 vMin;
		D3DXVECTOR3 vMax;

		if(pItem->GetAnimation().GetSize( vMin, vMax))
		{
			SetCamera(
				(vMax + vMin) / 2.0f,
				(vMax.x - vMin.x) * 1.1f,
				(vMax.y - vMin.y) * 1.1f,
				(vMax.z - vMin.z) * 1.1f);

			m_LightCenter.SetLight(&m_LightCenter.m_Light);
			m_LightRight.SetLight(&m_LightRight.m_Light);
			m_LightTop.SetLight(&m_LightTop.m_Light);

			m_LightCenter.EnableLight();
			m_LightRight.EnableLight();
			m_LightTop.EnableLight();

			pItem->Render(
				CMainFrame::m_3DDevice.m_pDevice,
				NULL, mat, 0.0f);

			m_LightCenter.EnableLight(FALSE);
			m_LightRight.EnableLight(FALSE);
			m_LightTop.EnableLight(FALSE);
		}
	}
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			NULL,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CPreviewCtrl::DrawTexture()
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 255, 255, 255),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	CTextureItem *pItem = (CTextureItem *) m_pItem;
	if( !pItem->GetLPDIRECT3DTEXTURE9() && !pItem->m_vTextureID.empty() )
		pItem = CItemBase::GetTextureItem(pItem->m_vTextureID[0]);

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pItem ? pItem->GetLPDIRECT3DTEXTURE9() : NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &mat);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &mat);

	CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pTextureVB, 0, sizeof(TNLVERTEX));
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CRect rect;
	GetClientRect(&rect);

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			&rect,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CPreviewCtrl::DrawImage()
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CImageItem *pItem = (CImageItem *) m_pItem;
	CRect rect;

	int nHeight = pItem->m_Image.GetHeight();
	int nWidth = pItem->m_Image.GetWidth();

	int nPosX = 0;
	int nPosY = 0;

	GetClientRect(&rect);
	rect.DeflateRect( 2, 2);

	if( nWidth * rect.Height() / nHeight < rect.Width() )
	{
		nWidth = nWidth * rect.Height() / nHeight;
		nHeight = rect.Height();

		nPosX = 2 + (rect.Width() - nWidth) / 2;
		nPosY = 2;
	}
	else
	{
		nHeight = nHeight * rect.Width() / nWidth;
		nWidth = rect.Width();

		nPosY = 2 + (rect.Height() - nHeight) / 2;
		nPosX = 2;
	}
	GetClientRect(&rect);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 255, 255, 255),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &mat);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &mat);

	DrawImage( &pItem->m_Image, nPosX, nPosY, nWidth, nHeight);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			&rect,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CPreviewCtrl::OnPaint()
{
	CPaintDC dc(this);

	if(!CMainFrame::m_bRender)
		return;

	if(!m_pItem)
	{
		DrawLOGO(&dc);
		return;
	}

	switch(m_pItem->m_bType)
	{
	case ITEM_ANIMATION	: DrawAnimation(); break;
	case ITEM_TEXTURE	: DrawTexture(); break;
	case ITEM_IMAGE		: DrawImage(); break;
	case ITEM_MESH		: DrawMesh(); break;
	case ITEM_OBJECT	: DrawOBJ(); break;
	default				: DrawLOGO(&dc); break;
	}
}

void CPreviewCtrl::DeleteTextureVB()
{
	if(m_pTextureVB)
	{
		m_pTextureVB->Release();
		m_pTextureVB = NULL;
	}
}

void CPreviewCtrl::InitTextureVB()
{
	LPTNLVERTEX pBuffer = NULL;
	DeleteTextureVB();

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		4 * sizeof(TNLVERTEX),
		0, T3DFVF_TNLVERTEX,
		D3DPOOL_MANAGED,
		&m_pTextureVB, NULL);

	m_pTextureVB->Lock( 0, 4 * sizeof(TNLVERTEX), (LPVOID *) &pBuffer, 0);
	for( int i=0; i<4; i++)
	{
		pBuffer[i].m_fPosX = 0.0f;
		pBuffer[i].m_fPosY = 0.0f;
		pBuffer[i].m_fPosZ = 0.5f;
		pBuffer[i].m_fRHW = 1.0f;

		pBuffer[i].m_dwColor = 0xFFFFFFFF;
		pBuffer[i].m_fU = (FLOAT) (i%2);
		pBuffer[i].m_fV = (FLOAT) (i/2);
	}
	m_pTextureVB->Unlock();
}

void CPreviewCtrl::InitSize( int cx, int cy)
{
	FLOAT fUnit = (cx > cy ? cy : cx) - 4.0f;
	FLOAT fRect[4][2] = {{
		(((FLOAT) cx) - fUnit) / 2.0f - 0.5f,
		(((FLOAT) cy) - fUnit) / 2.0f - 0.5f }, {
		(((FLOAT) cx) + fUnit) / 2.0f - 0.5f,
		(((FLOAT) cy) - fUnit) / 2.0f - 0.5f }, {
		(((FLOAT) cx) - fUnit) / 2.0f - 0.5f,
		(((FLOAT) cy) + fUnit) / 2.0f - 0.5f }, {
		(((FLOAT) cx) + fUnit) / 2.0f - 0.5f,
		(((FLOAT) cy) + fUnit) / 2.0f - 0.5f }};

	LPTNLVERTEX pBuffer = NULL;
	m_pTextureVB->Lock(
		0, 4 * sizeof(TNLVERTEX),
		(LPVOID *) &pBuffer, 0);

	for( int i=0; i<4; i++)
	{
		pBuffer[i].m_fPosX = fRect[i][0];
		pBuffer[i].m_fPosY = fRect[i][1];
	}

	m_pTextureVB->Unlock();
}

void CPreviewCtrl::OnSize( UINT nType, int cx, int cy)
{
	CStatic::OnSize( nType, cx, cy);

	if( GetSafeHwnd() && m_pTextureVB )
		InitSize( cx, cy);
}


// CPreviewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)
CPreviewDlg::CPreviewDlg( CWnd* pParent /*=NULL*/)
: CDialog( CPreviewDlg::IDD, pParent)
, m_strItemID(_T(""))
{
	m_bUpdate = TRUE;
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_cPreviewCtrl);
	DDX_Control(pDX, IDC_STATIC_ID, m_cTitleID);
	DDX_Control(pDX, IDC_EDIT_ITEMID, m_cItemID);
	DDX_Text(pDX, IDC_EDIT_ITEMID, m_strItemID);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_ITEMID, OnEnChangeEditItemid)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_EDIT_ITEMID, OnEnSetfocusEditItemid)
	ON_EN_KILLFOCUS(IDC_EDIT_ITEMID, OnEnKillfocusEditItemid)
END_MESSAGE_MAP()


// CPreviewDlg 메시지 처리기입니다.

void CPreviewDlg::OnSize( UINT nType, int cx, int cy)
{
	CDialog::OnSize( nType, cx, cy);

	if( m_cPreviewCtrl.GetSafeHwnd() &&
		m_cTitleID.GetSafeHwnd() &&
		m_cItemID.GetSafeHwnd())
	{
		CControlBar *pBar = (CControlBar *) GetParent();

		CRect rcPreviewCtrl;
		CRect rcTitleID;
		CRect rcItemID;
		CRect rect;

		m_cPreviewCtrl.GetWindowRect(&rcPreviewCtrl);
		m_cTitleID.GetWindowRect(&rcTitleID);
		m_cItemID.GetWindowRect(&rcItemID);
		pBar->GetClientRect(&rect);
		pBar->ClientToScreen(&rect);

		if( rect.left < 0 &&
			rect.right < 0 &&
			rect.top < 0 &&
			rect.bottom < 0)
			return;

		if(pBar->IsFloating())
		{
			CFrameWnd *pFrame = pBar->GetParentFrame();
			CRect rcFrame = rect;

			pFrame->ScreenToClient(&rcFrame);
			rect.InflateRect(
				2 * rcFrame.left,
				2 * rcFrame.top);
		}

		ScreenToClient(&rcPreviewCtrl);
		ScreenToClient(&rcTitleID);
		ScreenToClient(&rcItemID);
		ScreenToClient(&rect);

		rect.DeflateRect(
			rcPreviewCtrl.left,
			rcPreviewCtrl.top);

		rect.bottom -= 2;
		rect.right -= 1;

		int nMoveY = rect.Height() - rcItemID.bottom + rcPreviewCtrl.top;
		int nMoveX = rect.Width() - rcPreviewCtrl.Width();

		rcPreviewCtrl.right += nMoveX;
		rcItemID.right += nMoveX;

		rcTitleID.OffsetRect( 0, nMoveY);
		rcItemID.OffsetRect( 0, nMoveY);
		rcPreviewCtrl.bottom = rcItemID.top - 5;

		m_cPreviewCtrl.MoveWindow(&rcPreviewCtrl);
		m_cTitleID.MoveWindow(&rcTitleID);
		m_cItemID.MoveWindow(&rcItemID);
	}
}

BOOL CPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cPreviewCtrl.InitTextureVB();
	SetItem(NULL);

	return TRUE;
}

void CPreviewDlg::SetItem(CItemBase *pItem)
{
	m_cPreviewCtrl.m_pItem = pItem;
	m_cItemID.EnableWindow(pItem != NULL);

	m_strItemID = _T("");
	if(pItem)
	{
		m_strItemID = pItem->m_strID;
		m_strItemID.TrimRight();
		m_strItemID.TrimLeft();
	}

	if( m_strItemID.IsEmpty() || !CItemBase::CheckID(m_strItemID) )
		m_strItemID.LoadString(IDS_NOTUSE);

	m_cPreviewCtrl.Invalidate();
	UpdateData(FALSE);
}

void CPreviewDlg::OnEnChangeEditItemid()
{
	if(m_cPreviewCtrl.m_pItem)
	{
		UpdateData();

		if(!CItemBase::CheckID(m_strItemID))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);
			m_bUpdate = FALSE;

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
			m_bUpdate = TRUE;
			m_cItemID.SetFocus();
		}
	}
}

void CPreviewDlg::OnEnSetfocusEditItemid()
{
	UpdateData();

	if(!CItemBase::CheckID(m_strItemID))
	{
		m_strItemID = _T("");
		UpdateData(FALSE);
	}
}

void CPreviewDlg::OnEnKillfocusEditItemid()
{
	if(m_bUpdate)
	{
		BOOL bValid = FALSE;

		m_strItemID.TrimRight();
		m_strItemID.TrimLeft();

		if( !m_strItemID.IsEmpty() && CItemBase::CheckID(m_strItemID) )
			if(!CItemBase::CheckDupID( m_cPreviewCtrl.m_pItem->m_strID, m_strItemID))
				PostMessage(WM_MESSAGEBOX);
			else
				bValid = TRUE;

		if(bValid)
		{
			if(!m_cPreviewCtrl.m_pItem->m_strID.IsEmpty())
			{
				MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(m_cPreviewCtrl.m_pItem->m_strID);

				if( finder != CItemBase::m_mapGlobalID.end() )
					CItemBase::m_mapGlobalID.erase(finder);
			}

			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type(
				m_strItemID,
				m_cPreviewCtrl.m_pItem->m_dwID));

			m_cPreviewCtrl.m_pItem->m_strID = m_strItemID;
		}
		else if( !m_cPreviewCtrl.m_pItem->m_bNeedID && m_strItemID.IsEmpty() )
		{
			if(!m_cPreviewCtrl.m_pItem->m_strID.IsEmpty())
			{
				MAPSTRID::iterator finder = CItemBase::m_mapGlobalID.find(m_cPreviewCtrl.m_pItem->m_strID);

				if( finder != CItemBase::m_mapGlobalID.end() )
					CItemBase::m_mapGlobalID.erase(finder);
			}

			m_cPreviewCtrl.m_pItem->m_strID = _T("");
			m_strItemID.LoadString(IDS_NOTUSE);
		}
		else if(!m_cPreviewCtrl.m_pItem->m_strID.IsEmpty())
			m_strItemID = m_cPreviewCtrl.m_pItem->m_strID;
		else
			m_strItemID.LoadString(IDS_NOTUSE);

		UpdateData(FALSE);
	}
}

BOOL CPreviewDlg::OnEraseBkgnd( CDC* pDC)
{
	CRect rect;

	m_cPreviewCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->ExcludeClipRect(&rect);

	m_cTitleID.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->ExcludeClipRect(&rect);

	m_cItemID.GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->ExcludeClipRect(&rect);

	return CDialog::OnEraseBkgnd(pDC);
}

LRESULT CPreviewDlg::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.Format( IDS_ERR_DUP_ID, m_cPreviewCtrl.m_pItem->m_strName);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}

void CPreviewDlg::OnDestroy()
{
	m_cPreviewCtrl.DeleteTextureVB();
	CDialog::OnDestroy();
}

void CPreviewDlg::OnOK()
{
}

void CPreviewDlg::OnCancel()
{
}
