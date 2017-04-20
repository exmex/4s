// EXTMAPView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "EXTMAPView.h"
#include "EditHideMode.h"
#include "EditFreezeMode.h"
#include "SelectObjByName.h"
#include ".\extmapview.h"

DWORD t_dwTotal;
DWORD t_dwPeriod[10];

LPDIRECT3DTEXTURE9 CEXTMAPView::m_pSPRAYSRC[2][3] = {NULL};
LPDIRECT3DVERTEXBUFFER9 CEXTMAPView::m_pVB = NULL;

#define MAX_SPRAYSIZE				512

#define BIAS_DEFAULT				0
#define BIAS_SPRAY					1
#define BIAS_WIRE					3

// CEXTMAPView

IMPLEMENT_DYNCREATE(CEXTMAPView, CMAPView)

CEXTMAPView::CEXTMAPView()
{
	D3DLIGHT9 light;

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.7f;
	light.Diffuse.g = 0.7f;
	light.Diffuse.b = 0.7f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.2f;
	light.Ambient.g = 0.2f;
	light.Ambient.b = 0.2f;
	light.Ambient.a = 1.0f;
	light.Range = T3DLIGHT_RANGE_MAX;
	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	m_LIGHT.SetLight(&light);
	light.Diffuse.r = 0.3f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 0.3f;

	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	m_INVLIGHT.SetLight(&light);

	m_vEditH = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_ptEditH = CPoint( 0, 0);
	m_prev = CPoint( 0, 0);

	m_bInit = FALSE;
	m_nPrevH = 0;

	m_nUnitX = -2;
	m_nUnitZ = -2;

	for( int i=0; i<9; i++)
		m_bUNIT[i] = 0xFF;

	m_nPrvPaintX = 0;
	m_nPrvPaintZ = 0;

	t_dwTotal = 0;
	ZeroMemory(t_dwPeriod, sizeof(t_dwPeriod));
}

CEXTMAPView::~CEXTMAPView()
{
}

BEGIN_MESSAGE_MAP(CEXTMAPView, CMAPView)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
	ON_COMMAND(ID_GRID_EXPLORER, OnGridExplorer)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_HOTKEY_EDIT_DETAIL, OnHotkeyEditDetail)
	ON_COMMAND(ID_HOTKEY_EDIT_HEIGHT, OnHotkeyEditHeight)
	ON_COMMAND(ID_HOTKEY_EDIT_TEXTURE, OnHotkeyEditTexture)
	ON_COMMAND(ID_HOTKEY_HEIGHT_BRUSH, OnHotkeyHeightBrush)
	ON_COMMAND(ID_HOTKEY_MAKE_FLAT, OnHotkeyMakeFlat)
	ON_COMMAND(ID_HOTKEY_MAKE_SMOOTH, OnHotkeyMakeSmooth)
	ON_COMMAND( ID_HIDEMODE_DIALOG, OnEditHideMode )				// 06_01_21 박상연 하이드 다이얼로그 추가
	ON_COMMAND( ID_FREEZEMODE_DIALOG, OnEditFreezeMode )			// 06_01_21 박상연 프리즈 다이얼로그 추가
	ON_COMMAND( ID_SELBYNAME_DIALOG, OnEditSelctObjByName )		// 06_01_21 박상연 오브젝트 선택 다이얼로그 추가
	ON_WM_KEYUP()
	ON_COMMAND(ID_HOTKET_EDIT_OBJECT, OnHotketEditObject)
	ON_COMMAND(ID_TOGGLE_BRUSHSHAPE, OnToggleBrushshape)
END_MESSAGE_MAP()


// CEXTMAPView 진단입니다.

#ifdef _DEBUG
void CEXTMAPView::AssertValid() const
{
	CMAPView::AssertValid();
}

void CEXTMAPView::Dump(CDumpContext& dc) const
{
	CMAPView::Dump(dc);
}
#endif //_DEBUG


// CEXTMAPView 메시지 처리기입니다.

void CEXTMAPView::DrawContent()
{
	CGridView::DrawContent();

	static D3DXMATRIX vWorld(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CMAPOBJBar *pOBJBar	 = pDoc->GetMAPOBJBar();

	D3DXVECTOR3 vCurPOS;
	BYTE bSPRAY = FALSE;

	IPOINT vPIVOT(
		INT(m_Camera.m_vTarget.x) / pDoc->m_pItem->m_HugeMAP.m_nTileLength,
		INT(m_Camera.m_vTarget.z) / pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	CheckVB(&vPIVOT);
	CheckEDIT();

	if(pDoc->m_bResetVB)
		UpdateVB();

	pDoc->m_pItem->m_HugeMAP.ResetVB(
		&CMainFrame::m_3DDevice,
		&m_Camera,
		FALSE,
		TRUE);

	if( pDoc->m_bResetVB ||
		pBar->m_bEditTEX ||
		pBar->m_bRegion )
		pDoc->m_pItem->m_HugeMAP.ResetTEX( CMainFrame::m_3DDevice.m_pDevice, &vPIVOT);

	pDoc->SelectUNIT(
		this,
		m_nUnitX + 1,
		m_nUnitZ + 1);

	if( pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth ||
		pOBJBar->m_bInsertOBJBrush )
	{
		BYTE bMOVE = FALSE;

		if( m_bCamMode == CM_HEIGHT )
		{
			vCurPOS = FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength) * m_vEditH;
			bMOVE = TRUE;
		}
		else
			bMOVE = GetCursorPos(&vCurPOS);

		if(bMOVE)
		{
			MoveSPRAY( vCurPOS.x, vCurPOS.z);
			bSPRAY = TRUE;
		}
		else
			pDoc->ClearSPRAY();
	}

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_DEFAULT);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);
	m_Camera.Activate(TRUE);

	if(CMainFrame::m_3DDevice.m_bEnableSHADER)
	{
		int vLightCount[4] = { 2, 0, 1, 0};
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
			4.0f,
			1.0f,			//	PC_ARG
			1.0f,
			pDoc->m_pItem->m_HugeMAP.m_fIntensity,
			0.0f};

		CMainFrame::m_3DDevice.m_pDevice->SetPixelShaderConstantF(
			CMainFrame::m_3DDevice.m_vConstantPS[PC_ARG],
			&vCONST[12], 1);

		CMainFrame::m_3DDevice.m_pDevice->SetPixelShaderConstantF(
			CMainFrame::m_3DDevice.m_vConstantPS[PC_COMMON],
			&vCONST[4], 2);

		CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
			CMainFrame::m_3DDevice.m_vConstantVS[VC_COMMON],
			vCONST, 1);

		CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
			CMainFrame::m_3DDevice.m_vConstantVS[VC_AMBIENT],
			(FLOAT *) &D3DXVECTOR4(
			pDoc->m_pItem->m_HugeMAP.m_fSpecRange,
			pDoc->m_pItem->m_HugeMAP.m_fSpecMax,
			0.0f,
			0.0f), 1);

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

		D3DXVECTOR3 vDIR(
			pDoc->m_pItem->m_HugeMAP.m_vLIGHT.x,
			pDoc->m_pItem->m_HugeMAP.m_vLIGHT.y,
			pDoc->m_pItem->m_HugeMAP.m_vLIGHT.z);
		vDIR /= D3DXVec3Length(&vDIR);

		memcpy( vCONST, &D3DXVECTOR4(
			vDIR.x,
			vDIR.y,
			vDIR.z, 0.0f),
			4 * sizeof(FLOAT));

		CMainFrame::m_3DDevice.m_pDevice->SetVertexShaderConstantF(
			CMainFrame::m_3DDevice.m_vConstantVS[VC_LIGHTDIR],
			vCONST, 1);
	}

	if ( !pBar->m_bHideMap )
		pDoc->m_pItem->m_HugeMAP.Render(&CMainFrame::m_3DDevice);

	int nVertexCount = pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount + 1;
	int nCount = 0;

	for( int i=0; i<INT(pDoc->m_pItem->m_HugeMAP.m_vDETAIL.size()); i++)
		nCount += pDoc->m_pItem->m_HugeMAP.m_vCOUNT[i];
	nVertexCount = 9 * nVertexCount * nVertexCount;

	if(bSPRAY)
	{
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SPRAY);
		CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pDoc->m_pSPRAY);

		CMainFrame::m_3DDevice.m_pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, nVertexCount,
			0, nCount);
	}

	if(m_bShowEdge)
	{
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_WIRE);

		CMainFrame::m_3DDevice.m_pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, nVertexCount,
			0, nCount);
	}

	m_INVLIGHT.m_Light.Direction = -pDoc->m_pItem->m_HugeMAP.m_vLIGHT;
	m_LIGHT.m_Light.Direction = pDoc->m_pItem->m_HugeMAP.m_vLIGHT;

	m_INVLIGHT.SetLight(&m_INVLIGHT.m_Light);
	m_LIGHT.SetLight(&m_LIGHT.m_Light);

	m_INVLIGHT.EnableLight(TRUE);
	m_LIGHT.EnableLight(TRUE);

	if(CMainFrame::m_3DDevice.m_bEnableSHADER)
	{
		CD3DLight *pLIGHT[2] = {
			&m_LIGHT,
			&m_INVLIGHT};

		D3DMATERIAL9 mtrl;
		FLOAT vCONST[16];

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

		for( int i=0; i<2; i++)
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
			vCONST, 2);

		for( i=0; i<2; i++)
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
			vCONST, 2);

		for( i=0; i<2; i++)
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
			vCONST, 2);
	}
	m_bDrawOBJ = TRUE;

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
	CMAPView::DrawContent();

	m_INVLIGHT.EnableLight(FALSE);
	m_LIGHT.EnableLight(FALSE);
}

void CEXTMAPView::ReleaseSPRAY()
{
	for( int i=0; i<2; i++)
		for( int j=0; j<3; j++)
			if(m_pSPRAYSRC[i][j])
			{
				m_pSPRAYSRC[i][j]->Release();
				m_pSPRAYSRC[i][j] = NULL;
			}

	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CEXTMAPView::InitSPRAY()
{
	ReleaseSPRAY();

	for( int i=0; i<2; i++)
		for( int j=0; j<3; j++)
		{
			LPDIRECT3DSURFACE9 pIMG = NULL;
			D3DLOCKED_RECT lr;

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				MAX_SPRAYSIZE,
				MAX_SPRAYSIZE,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pIMG,
				NULL);
			pIMG->LockRect( &lr, NULL, 0);

			LPBYTE pDATA = (LPBYTE) lr.pBits;
			LPDWORD pCLR = NULL;

			for( int x=0; x<MAX_SPRAYSIZE; x++)
			{
				pCLR = (LPDWORD) pDATA;
				pDATA += lr.Pitch;

				for( int y=0; y<MAX_SPRAYSIZE; y++)
				{
					FLOAT fRadius = ((FLOAT) MAX_SPRAYSIZE) / 2.0f;
					FLOAT fPosX = (FLOAT) fabs(fRadius - ((FLOAT) x));
					FLOAT fPosY = (FLOAT) fabs(fRadius - ((FLOAT) y));
					FLOAT fLength;

					switch(i)
					{
					case 0	: fLength = max( fPosX, fPosY); break;
					case 1	: fLength = sqrtf(fPosX * fPosX + fPosY * fPosY); break;
					}

					switch(j)
					{
					case 0	: fLength = fRadius - fLength * fLength / fRadius; break;
					case 1	: fLength = fRadius - fLength; break;
					case 2	: fLength = fLength > fRadius ? 0.0f : fRadius; break;
					}

					if( fLength < 0.0f )
						fLength = 0.0f;

					BYTE bValueA = (BYTE) (fLength * 0xFF / (1.2f * fRadius));
					pCLR[y] = VALUEARGB(
						bValueA,
						0x00,
						0xFF,
						0x00);
				}
			}

			pIMG->UnlockRect();
			LPT3DDATA pDXT = CD3DImage::CreateDXT(
				CMainFrame::m_3DDevice.m_pDevice,
				pIMG,
				NON_COMP);
			pIMG->Release();

			m_pSPRAYSRC[i][j] = CMainFrame::m_3DDevice.LoadTexture(
				pDXT->m_pData,
				pDXT->m_dwSize,
				D3DX_DEFAULT);
			delete pDXT;
		}

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		4 * sizeof(PVERTEX),
		0, T3DFVF_PVERTEX,
		D3DPOOL_MANAGED,
		&m_pVB, NULL);

	PVERTEX vMESH[4] = {
		{ 0.0f, 0.0f,  0.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f,  0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, -1.0f, 1.0f, 1.0f}};
	LPPVERTEX pBUF = NULL;

	m_pVB->Lock( 0, 4 * sizeof(PVERTEX), (LPVOID *) &pBUF, 0);
	for( i=0; i<4; i++)
	{
		pBUF[i].m_fPosX = vMESH[i].m_fPosX;
		pBUF[i].m_fPosY = vMESH[i].m_fPosY;
		pBUF[i].m_fPosZ = vMESH[i].m_fPosZ;
		pBUF[i].m_fU = vMESH[i].m_fU;
		pBUF[i].m_fV = vMESH[i].m_fV;
	}
	m_pVB->Unlock();
}

void CEXTMAPView::BuildSPRAY( LPDIRECT3DTEXTURE9 pSPRAY,
							  LPDIRECT3DTEXTURE9 pIMG,
							  FLOAT fPosX,
							  FLOAT fPosZ)
{
	D3DXMATRIX vWORLD(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CMAPOBJBar *pOBJBar = pDoc->GetMAPOBJBar();		 

	LPDIRECT3DSURFACE9 pBACK = NULL;
	LPDIRECT3DSURFACE9 pZBUF = NULL;

	int nSpraySize = 2;
	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	fPosX = FLOAT(INT(fPosX / pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	fPosZ = FLOAT(INT(fPosZ / pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	FLOAT fLocalX = fPosX - nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	FLOAT fLocalZ = fPosZ - nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;

	LPDIRECT3DSURFACE9 pTARGET = NULL;
	CD3DCamera vCAMERA;

	D3DVIEWPORT9 vNEW = {
		0, 0,
		pDoc->m_pItem->m_HugeMAP.m_nTextureSize,
		pDoc->m_pItem->m_HugeMAP.m_nTextureSize,
		0.0f,
		1.0f};
	D3DVIEWPORT9 vOLD;

	DWORD dwCOLOR = pBar->m_bRegion ? VALUEARGB(
		0x00,
		GetRValue(pBar->m_cColorBox.m_Color),
		GetGValue(pBar->m_cColorBox.m_Color),
		GetBValue(pBar->m_cColorBox.m_Color)) : VALUEARGB(
		0x00,
		0x00,
		0xFF,
		0x00);

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

	CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
	CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);
	pSPRAY->GetSurfaceLevel( 0, &pTARGET);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTARGET);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		dwCOLOR,
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(PVERTEX));
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	if(pIMG)
	{
		D3DXMATRIX vTEX(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		D3DSURFACE_DESC desc;

		pIMG->GetLevelDesc( 0, &desc);
		vTEX._11 = FLOAT(pDoc->m_pItem->m_HugeMAP.m_nUnitLength) / FLOAT(desc.Width);
		vTEX._22 = -FLOAT(pDoc->m_pItem->m_HugeMAP.m_nUnitLength) / FLOAT(desc.Height);

		vTEX._31 = vTEX._11 * ((fPosX - fLocalX) / desc.Width - INT((fPosX - fLocalX) / desc.Width));
		vTEX._32 = vTEX._22 * ((fPosZ - fLocalZ) / desc.Height - INT((fPosZ - fLocalZ) / desc.Height));

		vWORLD._41 = -0.5f;
		vWORLD._43 = 0.5f;

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_BLUE|
			D3DCOLORWRITEENABLE_GREEN|
			D3DCOLORWRITEENABLE_RED);

		CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &vTEX);
		CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);

		CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pIMG);
		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0, 2);
	}
	// 06/04/24 PSY 수정( OBJ SPRAY 기능 추가 )
	////////////////////////////////////////////////////////////////////////////////
	nSpraySize = pOBJBar->m_bInsertOBJBrush ? pOBJBar->m_nBrushSize : pBar->m_nSize;
	////////////////////////////////////////////////////////////////////////////////
		
	vWORLD._41 = (fLocalX - FLOAT(nSpraySize) / 2.0f) / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nUnitLength) - 0.5f;
	vWORLD._43 = 0.5f - (fLocalZ - FLOAT(nSpraySize) / 2.0f) / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nUnitLength);

	vWORLD._11 = FLOAT(nSpraySize) / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nUnitLength);
	vWORLD._33 = vWORLD._11;

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000000);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, m_pSPRAYSRC[pOBJBar->m_bInsertOBJBrush ? pOBJBar->m_nBrushShape :
																	  pBar->m_nShape][pOBJBar->m_bInsertOBJBrush ? 2 : 
																	  pBar->m_bRegion || pBar->m_bSmooth ? 2 : 
																	  pBar->m_nAttenuation]);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALPHA);

	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
		D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_ALPHA|
		D3DCOLORWRITEENABLE_BLUE|
		D3DCOLORWRITEENABLE_GREEN|
		D3DCOLORWRITEENABLE_RED);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
	CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

	pTARGET->Release();
	pBACK->Release();
	pZBUF->Release();
}

void CEXTMAPView::MoveSPRAY( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	LPDIRECT3DSURFACE9 pSPRAY = NULL;
	LPDIRECT3DTEXTURE9 pIMG = NULL;

	if( pBar->m_bEditTEX && CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem->m_bType == ITEM_TEXTURE )
		pIMG = ((CTextureItem *) CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem)->GetLPDIRECT3DTEXTURE9();

	BuildSPRAY(
		pDoc->m_pSPRAY,
		pIMG,
		fPosX,
		fPosZ);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;

	pDoc->m_pSPRAY->GetSurfaceLevel( 0, &pSPRAY);
	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			pDoc->m_pItem->Blend(
				pSPRAY,
				pDoc->m_pSHADE[m_bUNIT[i]],
				FLOAT(nUnitX - nCellX),
				FLOAT(nUnitZ - nCellZ));
		}
	pSPRAY->Release();
}

BOOL CEXTMAPView::DoHEIGHT( CPoint point)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	FLOAT fHeight = 0.0f;
	FLOAT fPrevH = 0.0f;
	FLOAT fRatio = 1.0f;

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	if( m_bViewPort == VIEWPORT_NAVIGATE )
		fRatio = 10.0f;
	else if(!m_Camera.IsOrthoCamera())
		fRatio = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition)) * ((FLOAT) fabs(m_Camera.m_fFOV / 2.0f));
	else
		fRatio = m_Camera.m_fHeight / 2.0f;

	fRatio /= ((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY) / 2.0f;
	ClientToScreen(&point);
	fPrevH = fRatio * ((FLOAT) m_nPrevH);

	m_nPrevH = m_ptEditH.y - point.y;
	fHeight = fRatio * ((FLOAT) m_nPrevH);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(m_vEditH.x) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(m_vEditH.z) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(m_vEditH.x) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(m_vEditH.z) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ <= nEndZ )
			{
				nPosX = INT(INT(m_vEditH.x) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX <= nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX;
						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fRadius = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
						{
							switch(pBar->m_nAttenuation)
							{
							case 0	: fRatio = 1.0f - 4.0f * fRadius * fRadius / ((FLOAT) (pBar->m_nSize * pBar->m_nSize)); break;
							case 1	: fRatio = 1.0f - 2.0f * fRadius / ((FLOAT) pBar->m_nSize); break;
							case 2	: fRatio = 1.0f; break;
							}

							if( fRatio >= 0.0f )
							{
								pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] += fRatio * (fHeight - fPrevH);

								if( fRatio >= 0.99f )
									pDoc->SetHeight(pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex]);
							}
						}
					}

					nPosX++;
				}

				nPosZ++;
			}
		}

	return TRUE;
}

BOOL CEXTMAPView::DoHBRUSH( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	fPosX /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ <= nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX <= nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fRadius = 0.0f;
						FLOAT fRatio = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
						{
							switch(pBar->m_nAttenuation)
							{
							case 0	: fRatio = 1.0f - 4.0f * fRadius * fRadius / ((FLOAT) (pBar->m_nSize * pBar->m_nSize)); break;
							case 1	: fRatio = 1.0f - 2.0f * fRadius / ((FLOAT) pBar->m_nSize); break;
							case 2	: fRatio = 1.0f; break;
							}

							if( fRatio >= 0.0f )
								pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] += fRatio * pDoc->m_fBRUSH;
						}
					}

					nPosX++;
				}

				nPosZ++;
			}
		}

	return TRUE;
}

BOOL CEXTMAPView::DoSMOOTH( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	fPosX /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ <= nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX <= nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						BYTE bWEIGHT[9] = {
							nPosX > 0 ?
							nPosZ > 0 ?
							1 :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							1 : 0 :
							nPosZ > 0 ?
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							1 : 0 :
							i / 3 > 0 && i % 3 > 0 && m_bUNIT[i - 4] != 0xFF?
							1 : 0,
							nPosZ > 0 ?
							2 :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							2 : 0,
							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							nPosZ > 0 ?
							1 :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							1 : 0 :
							nPosZ > 0 ?
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							1 : 0 :
							i % 3 < 2 && i / 3 > 0 && m_bUNIT[i - 2] != 0xFF ?
							1 : 0,
							nPosX > 0 ?
							2 :
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							2 : 0,
							2,
							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							2 :
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							2 : 0,
							nPosX > 0 ?
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							1 :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							1 : 0 :
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							1 : 0 :
							i / 3 < 2 && i % 3 > 0 && m_bUNIT[i + 2] != 0xFF ?
							1 : 0,
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							2 :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							2 : 0,
							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							1 :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							1 : 0 :
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							1 : 0 :
							i / 3 < 2 && i % 3 < 2 && m_bUNIT[i + 4] != 0xFF ?
							1 : 0};

						FLOAT fDATA[9] = {
							nPosX > 0 ?
							nPosZ > 0 ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX - 1] :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 3]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX - 1] : 0.0f :
							nPosZ > 0 ?
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 1]].m_pDATA[(nPosZ - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1] : 0.0f :
							i / 3 > 0 && i % 3 > 0 && m_bUNIT[i - 4] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 4]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1] : 0.0f,

							nPosZ > 0 ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX] :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 3]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX] : 0.0f,

							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							nPosZ > 0 ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX + 1] :
							i / 3 > 0 && m_bUNIT[i - 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 3]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX + 1] : 0.0f :
							nPosZ > 0 ?
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 1]].m_pDATA[(nPosZ - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + 1] : 0.0f :
							i % 3 < 2 && i / 3 > 0 && m_bUNIT[i - 2] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 2]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + 1] : 0.0f,

							nPosX > 0 ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX - 1] :
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 1]].m_pDATA[nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + (pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1)] : 0.0f,

							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX],

							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX + 1] :
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 1]].m_pDATA[nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + 1] : 0.0f,

							nPosX > 0 ?
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ + 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX - 1] :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 3]].m_pDATA[pDoc->m_pItem->m_HugeMAP.m_nUnitLength + nPosX] : 0.0f :
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							i % 3 > 0 && m_bUNIT[i - 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i - 1]].m_pDATA[(nPosZ + 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + (pDoc->m_pItem->m_HugeMAP.m_nUnitLength - 1)] : 0.0f :
							i / 3 < 2 && i % 3 > 0 && m_bUNIT[i + 2] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 2]].m_pDATA[pDoc->m_pItem->m_HugeMAP.m_nUnitLength + pDoc->m_pItem->m_HugeMAP.m_nUnitLength] : 0.0f,

							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ + 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX] :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 3]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX] : 0.0f,

							nPosX < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[(nPosZ + 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX + 1] :
							i / 3 < 2 && m_bUNIT[i + 3] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 3]].m_pDATA[(pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX + 1] : 0.0f :
							nPosZ < pDoc->m_pItem->m_HugeMAP.m_nUnitLength ?
							i % 3 < 2 && m_bUNIT[i + 1] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 1]].m_pDATA[(nPosZ + 1) * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + 1] : 0.0f :
							i / 3 < 2 && i % 3 < 2 && m_bUNIT[i + 4] != 0xFF ?
							pDoc->m_UNIT[m_bUNIT[i + 4]].m_pDATA[pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 2] : 0.0f};
						int nIndex = nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fRadius = 0.0f;
						FLOAT fRatio = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
						{
							FLOAT fHEIGHT = 0.0f;
							int nCount = 0;

							for( int j=0; j<9; j++)
							{
								fHEIGHT += FLOAT(bWEIGHT[j]) * fDATA[j];
								nCount += bWEIGHT[j];
							}

							if( nCount > 0 )
								pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] = fHEIGHT / FLOAT(nCount);
						}
					}

					nPosX++;
				}

				nPosZ++;
			}
		}

	return TRUE;
}

BOOL CEXTMAPView::DoFLATH( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	fPosX /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ <= nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX <= nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fHeight = pDoc->m_fFLAT - pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex];
						FLOAT fRadius = 0.0f;
						FLOAT fRatio = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
						{
							switch(pBar->m_nAttenuation)
							{
							case 0	: fRatio = 1.0f - 4.0f * fRadius * fRadius / ((FLOAT) (pBar->m_nSize * pBar->m_nSize)); break;
							case 1	: fRatio = 1.0f - 2.0f * fRadius / ((FLOAT) pBar->m_nSize); break;
							case 2	: fRatio = 1.0f; break;
							}

							if( fRatio >= 0.0f )
							{
								FLOAT fNEW = pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] + fRatio * fHeight;

								if( fabs(fNEW - pDoc->m_fFLAT) < fabs(fHeight) )
									pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] = fNEW;
							}
						}
					}

					nPosX++;
				}

				nPosZ++;
			}
		}

	return TRUE;
}

BOOL CEXTMAPView::DoADDFLATH( FLOAT fPosX, FLOAT fPosZ, BOOL bCalcFlathH = FALSE )
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	fPosX /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			if ( bCalcFlathH )
			{
				int nCenterIndex = (INT) (fCenterZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + fCenterX);
				pDoc->SetHeight( (FLOAT) pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nCenterIndex] + pDoc->m_fADDFLAT );
				bCalcFlathH = FALSE;
			}
			

			while( nPosZ <= nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX <= nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * (pDoc->m_pItem->m_HugeMAP.m_nUnitLength + 1) + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fHeight = pDoc->m_fFLAT - pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex];
						FLOAT fRadius = 0.0f;
						FLOAT fRatio = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
						{
							switch(pBar->m_nAttenuation)
							{
							case 0	: fRatio = 1.0f - 4.0f * fRadius * fRadius / ((FLOAT) (pBar->m_nSize * pBar->m_nSize)); break;
							case 1	: fRatio = 1.0f - 2.0f * fRadius / ((FLOAT) pBar->m_nSize); break;
							case 2	: fRatio = 1.0f; break;
							}

							if( fRatio >= 0.0f )
							{
								FLOAT fNEW = pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] + fRatio * fHeight;

								if( fabs(fNEW - pDoc->m_fFLAT) < fabs(fHeight) )
									pDoc->m_UNIT[m_bUNIT[i]].m_pDATA[nIndex] = fNEW;
							}
						}
					}

					nPosX++;
				}

				nPosZ++;
			}
		}

	return TRUE;
}

BOOL CEXTMAPView::DoSPRAY( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	LPDIRECT3DTEXTURE9 pIMG = NULL;

	if( pBar->m_bEditTEX && CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem->m_bType == ITEM_TEXTURE )
		pIMG = ((CTextureItem *) CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem)->GetLPDIRECT3DTEXTURE9();

	BuildSPRAY(
		pDoc->m_pSPRAY,
		pIMG,
		fPosX,
		fPosZ);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			pDoc->m_pItem->Blend(
				pDoc->m_pSKIN[m_bUNIT[i]],
				pDoc->m_pSPRAY,
				FLOAT(nCellX - nUnitX),
				FLOAT(nCellZ - nUnitZ));

			pDoc->BuildTEX(
				&pDoc->m_UNIT[m_bUNIT[i]],
				pDoc->m_pREGION[m_bUNIT[i]],
				pDoc->m_pSHADE[m_bUNIT[i]],
				pDoc->m_pSKIN[m_bUNIT[i]]);
		}

	return TRUE;
}

void CEXTMAPView::OnRButtonDown( UINT nFlags, CPoint point)
{
	CMAPView::OnRButtonDown( nFlags, point);

	CEXTMAPDoc *pDoc = GetDocument();
	CRect rect;
	GetClientRect(&rect);

	D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
	D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
	D3DXVECTOR3 vResult;

	if(HitTest( &vResult, &vPos, &vDir))
		pDoc->SetHeight(vResult.y);
}

void CEXTMAPView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CMAPView::OnLButtonDown( nFlags, point);
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CMAPOBJBar *pOBJBar = pDoc->GetMAPOBJBar();		// 06/04/24 PSY

	if( m_bViewPort == VIEWPORT_NAVIGATE &&
		m_bCamMode != CM_NONE &&
		!(nFlags&MK_CONTROL) && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth ||
		pOBJBar->m_bInsertOBJBrush))
	{
		CPoint screen = point;

		ClientToScreen(&screen);
		SetCursorPos(
			screen.x,
			screen.y);

		m_bCamMode = CM_NONE;
	}

	if( m_bCamMode == CM_NONE && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth ||
		pOBJBar->m_bInsertOBJBrush ))
	{
		CRect rect;
		GetClientRect(&rect);

		D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vResult;

		if(pBar->m_bDetail)
		{
			if(HitTest( &vResult, &vPos, &vDir))
			{
				m_bCamMode = CM_DETAIL;
				DoDETAIL( vResult.x, vResult.z, TRUE);
			}

			pDoc->RedrawAllView();
		}
		else if(pBar->m_bRegion)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoREGION( vResult.x, vResult.z);

			m_bCamMode = CM_REGION;
			pDoc->RedrawAllView();
		}
		else if(pBar->m_bEditTEX)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoSPRAY( vResult.x, vResult.z);

			m_bCamMode = CM_PAINT;
			pDoc->RedrawAllView();
		}
		else if(pBar->m_bHBrush)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoHBRUSH( vResult.x, vResult.z);

			m_bCamMode = CM_HBRUSH;
			pDoc->RedrawAllView();
		}
		else if(pBar->m_bSmooth)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoSMOOTH( vResult.x, vResult.z);

			m_bCamMode = CM_SMOOTH;
			pDoc->RedrawAllView();
		}
		else if(pBar->m_bFlatH)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoFLATH( vResult.x, vResult.z);

			m_bCamMode = CM_FLATH;
			pDoc->RedrawAllView();
		}
		else if(pBar->m_bAddFlatH)
		{
			if(HitTest( &vResult, &vPos, &vDir))
				DoADDFLATH( vResult.x, vResult.z, TRUE );

			m_bCamMode = CM_ADDFLATH;
			pDoc->RedrawAllView();
		}
		else if(HitTest( &vResult, &vPos, &vDir))
		{
			m_bCamMode = CM_HEIGHT;
			m_vEditH = vResult;
			m_ptEditH = point;
			m_nPrevH = 0;

			m_vEditH.x /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
			m_vEditH.z /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

			ClientToScreen(&m_ptEditH);
		}		
	}
	else if( pBar->m_bEditTEX || pBar->m_bEditH || pBar->m_bFlatH || pBar->m_bAddFlatH || pBar->m_bRegion || pBar->m_bDetail || pBar->m_bHBrush || pBar->m_bSmooth )
		pDoc->RedrawAllView();
}

void CEXTMAPView::OnLButtonUp( UINT nFlags, CPoint point)
{
	BOOL bEditTEX = m_bCamMode == CM_PAINT;
	BOOL bEditH = m_bCamMode == CM_HEIGHT;

	CMAPView::OnLButtonUp( nFlags, point);
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	if(bEditTEX)
		pDoc->UpdateData();

	if(bEditH)
	{
		SetCursorPos(
			m_ptEditH.x,
			m_ptEditH.y);

		ScreenToClient(&m_ptEditH);
		point = m_ptEditH;
	}

	if( m_bCamMode == CM_NONE && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth))
	{
		D3DXVECTOR3 vResult;
		CRect rect;
		GetClientRect(&rect);

		D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

		D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

		if(HitTest( &vResult, &vPos, &vDir))
			SetCursor(NULL);

		pDoc->RedrawAllView();
	}
}

void CEXTMAPView::OnMButtonDown( UINT nFlags, CPoint point)
{
	CMAPView::OnMButtonDown( nFlags, point);
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	if( m_bCamMode != CM_NONE && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth))
		pDoc->RedrawAllView();
}

void CEXTMAPView::OnMButtonUp( UINT nFlags, CPoint point)
{
	CMAPView::OnMButtonUp( nFlags, point);
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	if( m_bCamMode == CM_NONE && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth))
	{
		CRect rect;
		GetClientRect(&rect);

		D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vResult;

		if(HitTest( &vResult, &vPos, &vDir))
			SetCursor(NULL);
		pDoc->RedrawAllView();
	}
}

void CEXTMAPView::OnMouseMove( UINT nFlags, CPoint point)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CMAPOBJBar *pOBJBar = pDoc->GetMAPOBJBar();

	if( m_bCamMode == CM_PAINT ||
		m_bCamMode == CM_DETAIL ||
		m_bCamMode == CM_REGION ||
		m_bCamMode == CM_HEIGHT ||
		m_bCamMode == CM_HBRUSH ||
		m_bCamMode == CM_SMOOTH ||
		m_bCamMode == CM_FLATH ||
		m_bCamMode == CM_ADDFLATH ||
		m_bCamMode == CM_OBJSPARY || (
		m_bCamMode == CM_NONE && (
		pBar->m_bEditTEX ||
		pBar->m_bEditH ||
		pBar->m_bFlatH ||
		pBar->m_bAddFlatH ||
		pBar->m_bRegion ||
		pBar->m_bDetail ||
		pBar->m_bHBrush ||
		pBar->m_bSmooth ||
		pOBJBar->m_bInsertOBJBrush )))
	{
		// 06/05/13 PSY 
		// OBJ Brush 상태에서 브러쉬의 중심 Pos를 표시 
		////////////////////////////////////////////////////////
		D3DXVECTOR3 vPOS;

		if( GetCursorPos(&vPOS) && (pOBJBar->m_bInsertOBJBrush || pBar->m_bRegion) )
		{
			CMainFrame *pMainFrame = (CMainFrame *) AfxGetMainWnd();
			FLOAT fCenterX;
			FLOAT fCenterZ;

			INT nPOSX = INT( vPOS.x / pDoc->m_pItem->m_HugeMAP.m_nTileLength );
			INT nPOSZ = INT( vPOS.z / pDoc->m_pItem->m_HugeMAP.m_nTileLength );

			fCenterX = ( ( nPOSX * pDoc->m_pItem->m_HugeMAP.m_nTileLength ) + (pOBJBar->m_nBrushSize % 2 ? FLOAT( pDoc->m_pItem->m_HugeMAP.m_nTileLength ) / 2.0f : 0 ) );
			fCenterZ = ( ( nPOSZ * pDoc->m_pItem->m_HugeMAP.m_nTileLength ) + (pOBJBar->m_nBrushSize % 2 ? FLOAT( pDoc->m_pItem->m_HugeMAP.m_nTileLength ) / 2.0f : 0 ) );

			CString strText;

			strText.Format( "X : %.2f", fCenterX);
			pMainFrame->SetPaneText(
				strText,
				ID_PANE_POSX);

			strText.Format( "Y : %.2f", pDoc->GetHeight( fCenterX, 0.0f ,fCenterZ ));
			pMainFrame->SetPaneText(
				strText,
				ID_PANE_POSY);

			strText.Format( "Z : %.2f", fCenterZ);
			pMainFrame->SetPaneText(
				strText,
				ID_PANE_POSZ);

			strText.Format( "K : %.2f", fCenterZ);
			pMainFrame->SetPaneText(
				strText,
				4);
		}
		////////////////////////////////////////////////////////
		static CPoint prev( 0, 0);

		if( !IsPlay() && point != prev )
		{
			CRect rect;
			GetClientRect(&rect);

			D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
				point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
			D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
				point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
			D3DXVECTOR3 vResult;

			BOOL bDO = FALSE;
			switch(m_bCamMode)
			{
			case CM_HEIGHT	: 
				bDO = DoHEIGHT(point); 
				break;

			case CM_HBRUSH	:
				if(HitTest( &vResult, &vPos, &vDir))
					DoHBRUSH( vResult.x, vResult.z);

				break;

			case CM_SMOOTH	:
				if(HitTest( &vResult, &vPos, &vDir))
					DoSMOOTH( vResult.x, vResult.z);

				break;

			case CM_FLATH	:
				if(HitTest( &vResult, &vPos, &vDir))
					DoFLATH( vResult.x, vResult.z);

				break;

			case CM_ADDFLATH	:
				 if(HitTest( &vResult, &vPos, &vDir))
					DoADDFLATH( vResult.x, vResult.z );

				break;

			case CM_DETAIL	:
				if(HitTest( &vResult, &vPos, &vDir))
					 DoDETAIL( vResult.x, vResult.z, FALSE);
					
				break;

			case CM_REGION	:
				if(HitTest( &vResult, &vPos, &vDir))
					DoREGION( vResult.x, vResult.z);

				break;

			case CM_PAINT	:
				if(HitTest( &vResult, &vPos, &vDir))
					DoSPRAY( vResult.x, vResult.z);

				break;

			case CM_OBJSPARY:
				if(HitTest( &vResult, &vPos, &vDir))
					DoOBJSPRAY( vResult.x, vResult.z);

				break;
			}

			pDoc->RedrawAllView();
			prev = point;
		}

		CView::OnMouseMove(nFlags, point);
	}
	else
	{
		CMAPView::OnMouseMove( nFlags, point);

		if( m_bCamMode == CM_NONE && (
			pBar->m_bEditTEX ||
			pBar->m_bEditH ||
			pBar->m_bFlatH ||
			pBar->m_bAddFlatH ||
			pBar->m_bRegion ||
			pBar->m_bDetail ||
			pBar->m_bHBrush ||
			pBar->m_bSmooth ||
			pOBJBar->m_bInsertOBJBrush ))
			SetCursor(NULL);
	}
}

BOOL CEXTMAPView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	if( pBar->m_bEditTEX || pBar->m_bEditH || pBar->m_bFlatH || pBar->m_bAddFlatH || pBar->m_bRegion || pBar->m_bDetail || pBar->m_bHBrush || pBar->m_bSmooth )
	{
		CPoint point;
		CRect rect;

		::GetCursorPos(&point);
		ScreenToClient(&point);
		GetClientRect(&rect);

		D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vResult;

		if( !IsPlay() && HitTest( &vResult, &vPos, &vDir) )
		{
			SetCursor(NULL);
			return TRUE;
		}
	}

	return CMAPView::OnSetCursor( pWnd, nHitTest, message);
}

BYTE CEXTMAPView::GetCursorPos( LPD3DXVECTOR3 pPOS)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CPoint point;
	CRect rect;
	
	::GetCursorPos(&point);
	ScreenToClient(&point);
	GetClientRect(&rect);

	if(!rect.PtInRect(point))
		return FALSE;

	D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	return HitTest( pPOS, &vPos, &vDir);
}

void CEXTMAPView::OnContextMenu( CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void CEXTMAPView::CheckVB( LPIPOINT pPIVOT)
{
	CEXTMAPDoc *pDoc = GetDocument();

	int nCellX = pPIVOT->m_nX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nCellZ = pPIVOT->m_nY * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);
	int nUnitX = pPIVOT->m_nX / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (pPIVOT->m_nX < 0 ? 2 : 1);
	int nUnitZ = pPIVOT->m_nY / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (pPIVOT->m_nY < 0 ? 2 : 1);

	if( nUnitX != pDoc->m_pItem->m_HugeMAP.m_nUnitX ||
		nUnitZ != pDoc->m_pItem->m_HugeMAP.m_nUnitZ )
	{
		BYTE bUNIT[9];

		for( int i=0; i<9; i++)
		{
			pDoc->m_pItem->m_HugeMAP.m_pUNIT[i] = NULL;
			bUNIT[i] = m_bUNIT[i];
		}

		for( i=0; i<9; i++)
		{
			int nLocalX = nUnitX - m_nUnitX + i % 3;
			int nLocalZ = nUnitZ - m_nUnitZ + i / 3;

			if( nLocalX < 0 || nLocalX > 2 ||
				nLocalZ < 0 || nLocalZ > 2 )
				m_bUNIT[i] = 0xFF;
			else
			{
				int nIndex = nLocalZ * 3 + nLocalX;

				if( bUNIT[nIndex] != 0xFF )
				{
					pDoc->m_pItem->m_HugeMAP.m_pUNIT[i] = &pDoc->m_UNIT[bUNIT[nIndex]];
					m_bUNIT[i] = bUNIT[nIndex];

					bUNIT[nIndex] = 0xFF;
				}
				else
					m_bUNIT[i] = 0xFF;
			}
		}

		for( i=0; i<9; i++)
			if( bUNIT[i] != 0xFF )
				pDoc->ReleaseUNIT( this, (m_nUnitZ + i / 3) * pDoc->m_pItem->m_HugeMAP.m_nUnitCountX + m_nUnitX + i % 3);

		pDoc->m_pItem->m_HugeMAP.m_nUnitX = nUnitX;
		pDoc->m_pItem->m_HugeMAP.m_nUnitZ = nUnitZ;
		m_nUnitX = nUnitX;
		m_nUnitZ = nUnitZ;
	}

	if( nCellX != pDoc->m_pItem->m_HugeMAP.m_nCellX ||
		nCellZ != pDoc->m_pItem->m_HugeMAP.m_nCellZ )
	{
		BYTE bINDEX[9];

		for( int i=0; i<9; i++)
		{
			bINDEX[i] = pDoc->m_pItem->m_HugeMAP.m_bINDEX[i];
			pDoc->m_pItem->m_HugeMAP.m_bLOCK[i] = FALSE;
		}

		for( i=0; i<9; i++)
		{
			int nLocalX = nCellX - pDoc->m_pItem->m_HugeMAP.m_nCellX + i % 3;
			int nLocalZ = nCellZ - pDoc->m_pItem->m_HugeMAP.m_nCellZ + i / 3;

			if( nLocalX < 0 || nLocalX > 2 ||
				nLocalZ < 0 || nLocalZ > 2 )
				pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] = 0xFF;
			else
			{
				int nIndex = nLocalZ * 3 + nLocalX;

				if( bINDEX[nIndex] != 0xFF )
				{
					pDoc->m_pItem->m_HugeMAP.m_bLOCK[bINDEX[nIndex]] = TRUE;
					pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] = bINDEX[nIndex];
				}
				else
					pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] = 0xFF;
			}
		}

		for( i=0; i<9; i++)
			if(!pDoc->m_pItem->m_HugeMAP.m_bLOCK[i])
				pDoc->m_pItem->m_HugeMAP.FreeDIFFUSE(i);

		pDoc->m_pItem->m_HugeMAP.m_nCellX = nCellX;
		pDoc->m_pItem->m_HugeMAP.m_nCellZ = nCellZ;

		for( i=0; i<9; i++)
			if( pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] == 0xFF )
			{
				nCellX = pDoc->m_pItem->m_HugeMAP.m_nCellX + i % 3;
				nCellZ = pDoc->m_pItem->m_HugeMAP.m_nCellZ + i / 3;

				nUnitX = nCellX / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellX < 0 ? 1 : 0);
				nUnitZ = nCellZ / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellZ < 0 ? 1 : 0);

				if( nUnitX >= 0 && nUnitX < pDoc->m_pItem->m_HugeMAP.m_nUnitCountX &&
					nUnitZ >= 0 && nUnitZ < pDoc->m_pItem->m_HugeMAP.m_nUnitCountZ )
				{
					int nIndex = (nUnitZ - pDoc->m_pItem->m_HugeMAP.m_nUnitZ) * 3 + nUnitX - pDoc->m_pItem->m_HugeMAP.m_nUnitX;
					int nUnitID = nUnitZ * pDoc->m_pItem->m_HugeMAP.m_nUnitCountX + nUnitX;

					if(pDoc->m_pItem->m_HugeMAP.m_pENABLE[nUnitID])
					{
						if( m_bUNIT[nIndex] == 0xFF )
							m_bUNIT[nIndex] = pDoc->LockUNIT( this, nUnitID);

						if(!pDoc->m_pItem->m_HugeMAP.m_pUNIT[nIndex])
							pDoc->m_pItem->m_HugeMAP.m_pUNIT[nIndex] = &pDoc->m_UNIT[m_bUNIT[nIndex]];

						pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] = pDoc->m_pItem->m_HugeMAP.FindFreeCELL();
						pDoc->m_pItem->m_HugeMAP.m_bLOCK[pDoc->m_pItem->m_HugeMAP.m_bINDEX[i]] = TRUE;

						pDoc->m_pItem->m_HugeMAP.LoadVB(
							pDoc->m_pItem->m_HugeMAP.m_pUNIT[nIndex], i,
							nUnitX,
							nUnitZ,
							nCellX,
							nCellZ);

						pDoc->m_pItem->m_HugeMAP.BuildDIFFUSE(
							CMainFrame::m_3DDevice.m_pDevice, i,
							nCellX,
							nCellZ);
					}
				}
			}

		pDoc->m_pItem->m_HugeMAP.m_vPIVOT.m_dwID = 0xFFFFFFFF;
		pDoc->m_pItem->m_HugeMAP.ResetUV();
	}
}

void CEXTMAPView::CheckEDIT()
{
	switch(m_bCamMode)
	{
	case CM_DETAIL		: UpdateDIFFUSE(); break;
	case CM_HEIGHT		:
	case CM_HBRUSH		:
	case CM_SMOOTH		:
	case CM_FLATH		: UpdateVB(); break;
	case CM_ADDFLATH	: UpdateVB(); break;
	}
}

void CEXTMAPView::UpdateDIFFUSE()
{
	CEXTMAPDoc *pDoc = GetDocument();

	for( int i=0; i<9; i++)
		if( pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] != 0xFF )
		{
			int nCellX = pDoc->m_pItem->m_HugeMAP.m_nCellX + i % 3;
			int nCellZ = pDoc->m_pItem->m_HugeMAP.m_nCellZ + i / 3;

			int nUnitX = nCellX / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellX < 0 ? 1 : 0);
			int nUnitZ = nCellZ / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellZ < 0 ? 1 : 0);

			if( nUnitX >= 0 && nUnitX < pDoc->m_pItem->m_HugeMAP.m_nUnitCountX &&
				nUnitZ >= 0 && nUnitZ < pDoc->m_pItem->m_HugeMAP.m_nUnitCountZ )
			{
				int nUnitID = nUnitZ * pDoc->m_pItem->m_HugeMAP.m_nUnitCountX + nUnitX;

				if(pDoc->m_pItem->m_HugeMAP.m_pENABLE[nUnitID])
				{
					pDoc->m_pItem->m_HugeMAP.BuildDIFFUSE(
						CMainFrame::m_3DDevice.m_pDevice, i,
						nCellX,
						nCellZ);
				}
			}
		}

	pDoc->m_pItem->m_HugeMAP.m_vPIVOT.m_dwID = 0xFFFFFFFF;
}

void CEXTMAPView::UpdateVB()
{
	CEXTMAPDoc *pDoc = GetDocument();

	for( int i=0; i<9; i++)
		if( pDoc->m_pItem->m_HugeMAP.m_bINDEX[i] != 0xFF )
		{
			int nCellX = pDoc->m_pItem->m_HugeMAP.m_nCellX + i % 3;
			int nCellZ = pDoc->m_pItem->m_HugeMAP.m_nCellZ + i / 3;

			int nUnitX = nCellX / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellX < 0 ? 1 : 0);
			int nUnitZ = nCellZ / pDoc->m_pItem->m_HugeMAP.m_nCellCount - (nCellZ < 0 ? 1 : 0);

			if( nUnitX >= 0 && nUnitX < pDoc->m_pItem->m_HugeMAP.m_nUnitCountX &&
				nUnitZ >= 0 && nUnitZ < pDoc->m_pItem->m_HugeMAP.m_nUnitCountZ )
			{
				int nIndex = (nUnitZ - pDoc->m_pItem->m_HugeMAP.m_nUnitZ) * 3 + nUnitX - pDoc->m_pItem->m_HugeMAP.m_nUnitX;
				int nUnitID = nUnitZ * pDoc->m_pItem->m_HugeMAP.m_nUnitCountX + nUnitX;

				if(pDoc->m_pItem->m_HugeMAP.m_pENABLE[nUnitID])
				{
					pDoc->m_pItem->m_HugeMAP.LoadVB(
						pDoc->m_pItem->m_HugeMAP.m_pUNIT[nIndex], i,
						nUnitX,
						nUnitZ,
						nCellX,
						nCellZ);
				}
			}
		}
}

void CEXTMAPView::JumpView( D3DXVECTOR3& vPOS)
{
	m_Camera.Move(
		vPOS.x - m_Camera.m_vTarget.x,
		vPOS.y - m_Camera.m_vTarget.y,
		vPOS.z - m_Camera.m_vTarget.z,
		TRUE);

	if(!IsPlay())
		Render();
}

BYTE CEXTMAPView::HitTest( LPD3DXVECTOR3 pResult,
						   LPD3DXVECTOR3 pPOS,
						   LPD3DXVECTOR3 pDIR)
{
	CEXTMAPDoc *pDoc = GetDocument();

	for( int i=0; i<9; i++)
		pDoc->m_pItem->m_HugeMAP.m_pUNIT[i] = m_bUNIT[i] == 0xFF ? NULL : &pDoc->m_UNIT[m_bUNIT[i]];

	pDoc->m_pItem->m_HugeMAP.m_nUnitX = m_nUnitX;
	pDoc->m_pItem->m_HugeMAP.m_nUnitZ = m_nUnitZ;

	return pDoc->m_pItem->m_HugeMAP.HitTest(
		pResult,
		pPOS,
		pDIR);
}

BYTE CEXTMAPView::IsVisible( CTachyonObject *pOBJ)
{
	return CMAPView::IsVisible(pOBJ);
}

BYTE CEXTMAPView::PtInView( LPD3DXVECTOR3 pPOS)
{
	CEXTMAPDoc *pDoc = GetDocument();

	int nCellX = INT(pPOS->x) * pDoc->m_pItem->m_HugeMAP.m_nCellCount / (pDoc->m_pItem->m_HugeMAP.m_nUnitLength * pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	int nCellZ = INT(pPOS->z) * pDoc->m_pItem->m_HugeMAP.m_nCellCount / (pDoc->m_pItem->m_HugeMAP.m_nUnitLength * pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	CRect rect(
		pDoc->m_pItem->m_HugeMAP.m_nCellX,
		pDoc->m_pItem->m_HugeMAP.m_nCellZ,
		pDoc->m_pItem->m_HugeMAP.m_nCellX + 3,
		pDoc->m_pItem->m_HugeMAP.m_nCellZ + 3);

	if(rect.PtInRect(CPoint( nCellX, nCellZ)))
	{
		nCellX -= pDoc->m_pItem->m_HugeMAP.m_nCellX;
		nCellZ -= pDoc->m_pItem->m_HugeMAP.m_nCellZ;

		return pDoc->m_pItem->m_HugeMAP.m_bLOCK[pDoc->m_pItem->m_HugeMAP.m_bINDEX[nCellZ * 3 + nCellX]];
	}

	return FALSE;
}

FLOAT CEXTMAPView::GetCamHeight()
{
	CEXTMAPDoc *pDoc = GetDocument();

	return pDoc->m_pItem->m_HugeMAP.GetHeight(
		m_Camera.m_vPosition.x,
		m_Camera.m_vPosition.y,
		m_Camera.m_vPosition.z);
}

BOOL CEXTMAPView::DoDETAIL( FLOAT fPosX, FLOAT fPosZ, BOOL bStart)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CTachyonHUGEMAP* pHugeMap = &pDoc->m_pItem->m_HugeMAP;

	CTextureItem *pDETAIL = NULL;
	CRect rcCELL(
		0, 0,
		3 * pHugeMap->m_nUnitLength / pHugeMap->m_nCellCount,
		3 * pHugeMap->m_nUnitLength / pHugeMap->m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pHugeMap->m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pHugeMap->m_nTileLength));

	nCellX = nCellX * pHugeMap->m_nCellCount / pHugeMap->m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pHugeMap->m_nCellCount / pHugeMap->m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pHugeMap->m_nUnitLength / pHugeMap->m_nCellCount;
	nCellZ = nCellZ * pHugeMap->m_nUnitLength / pHugeMap->m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	fPosX /= FLOAT(pHugeMap->m_nTileLength);
	fPosZ /= FLOAT(pHugeMap->m_nTileLength);

	if( !bStart && m_nPrvPaintX == INT(fPosX) && m_nPrvPaintZ == INT(fPosZ) )
		return FALSE;

	m_nPrvPaintX = INT(fPosX);
	m_nPrvPaintZ = INT(fPosZ);

	if( pBar->m_bDetail && CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem->m_bType == ITEM_TEXTURE )
		pDETAIL = (CTextureItem *) CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem;
	else
		return FALSE;

	BYTE bDETAIL = TILE_NULL;
	BYTE bCount = (BYTE) pDoc->m_pItem->m_vDETAIL.size();
	for( BYTE j=0; j<bCount; j++)
		if( pDETAIL->m_dwID == pDoc->m_pItem->m_vDETAIL[j] )
		{
			bDETAIL = j;
			break;
		}

	if( bDETAIL == TILE_NULL )
	{
		CString strMessage, strTitle;
		strMessage.LoadString(IDS_ERR_NODETAILTEXINLIST);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox(strMessage,strTitle, MB_OK|MB_ICONHAND);

		m_bCamMode = CM_NONE;
		return FALSE;
	}

	if( pBar->m_nAttenuation == 2 )
		bDETAIL |= 0x80;

	for( int i=0; i<9; i++)
	{
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pHugeMap->m_nUnitLength,
				pHugeMap->m_nUnitLength);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pHugeMap->m_nUnitLength;
			nUnitZ *= pHugeMap->m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ < nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX < nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * pHugeMap->m_nUnitLength + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fRadius = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
							pDoc->m_UNIT[m_bUNIT[i]].m_pDETAIL[nIndex] = bDETAIL;
					}

					nPosX++;
				}

				nPosZ++;
			}
		}
	}

	return TRUE;
}

BOOL CEXTMAPView::DoREGION( FLOAT fPosX, FLOAT fPosZ)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();

	CTextureItem *pDETAIL = NULL;
	CRect rcCELL(
		0, 0,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount,
		3 * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount);

	int nCellX = INT(m_Camera.m_vTarget.x / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));
	int nCellZ = INT(m_Camera.m_vTarget.z / FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength));

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellX < 0 ? 2 : 1);
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nCellCount / pDoc->m_pItem->m_HugeMAP.m_nUnitLength - (nCellZ < 0 ? 2 : 1);

	nCellX = nCellX * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	nCellZ = nCellZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength / pDoc->m_pItem->m_HugeMAP.m_nCellCount;
	rcCELL.OffsetRect( nCellX, nCellZ);

	BuildSPRAY(
		pDoc->m_pSPRAY,
		NULL,
		fPosX,
		fPosZ);

	fPosX /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);
	fPosZ /= FLOAT(pDoc->m_pItem->m_HugeMAP.m_nTileLength);

	for( int i=0; i<9; i++)
		if( m_bUNIT[i] != 0xFF )
		{
			CRect rect(
				0, 0,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength,
				pDoc->m_pItem->m_HugeMAP.m_nUnitLength);

			int nUnitX = m_nUnitX + i % 3;
			int nUnitZ = m_nUnitZ + i / 3;

			nUnitX *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;
			nUnitZ *= pDoc->m_pItem->m_HugeMAP.m_nUnitLength;

			rect.OffsetRect(
				nUnitX,
				nUnitZ);

			rect.IntersectRect(
				&rect,
				&rcCELL);

			rect.OffsetRect(
				-nUnitX,
				-nUnitZ);

			int nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nPosZ = INT(INT(fPosZ) - nUnitZ - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndX = INT(INT(fPosX) - nUnitX + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);
			int nEndZ = INT(INT(fPosZ) - nUnitZ + ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

			FLOAT fCenterX = ((FLOAT) nPosX) + ((FLOAT) (nEndX - nPosX)) / 2.0f;
			FLOAT fCenterZ = ((FLOAT) nPosZ) + ((FLOAT) (nEndZ - nPosZ)) / 2.0f;

			while( nPosZ < nEndZ )
			{
				nPosX = INT(INT(fPosX) - nUnitX - ((FLOAT) pBar->m_nSize) / 2.0f) + (pBar->m_nSize % 2 ? 1 : 0);

				while( nPosX < nEndX )
				{
					if(rect.PtInRect(CPoint( nPosX, nPosZ)))
					{
						int nIndex = nPosZ * pDoc->m_pItem->m_HugeMAP.m_nUnitLength + nPosX;

						FLOAT fDistX = (FLOAT) fabs(fCenterX - ((FLOAT) nPosX));
						FLOAT fDistZ = (FLOAT) fabs(fCenterZ - ((FLOAT) nPosZ));
						FLOAT fRadius = 0.0f;

						switch(pBar->m_nShape)
						{
						case 0	: fRadius = max( fDistX, fDistZ); break;
						case 1	: fRadius = sqrtf(fDistX * fDistX + fDistZ * fDistZ); break;
						}

						if( fRadius <= ((FLOAT) pBar->m_nSize) / 2.0f )
							pDoc->m_UNIT[m_bUNIT[i]].m_pREGION[nIndex] = pBar->m_bRegionID;
					}

					nPosX++;
				}

				nPosZ++;
			}

			pDoc->m_pItem->Blend(
				pDoc->m_pREGION[m_bUNIT[i]],
				pDoc->m_pSPRAY,
				FLOAT(nCellX - nUnitX),
				FLOAT(nCellZ - nUnitZ));

			pDoc->BuildTEX(
				&pDoc->m_UNIT[m_bUNIT[i]],
				pDoc->m_pREGION[m_bUNIT[i]],
				pDoc->m_pSHADE[m_bUNIT[i]],
				pDoc->m_pSKIN[m_bUNIT[i]]);
		}

	return TRUE;
}

void CEXTMAPView::OnGridCamreset()
{
	FLOAT fDist = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition));
	D3DXVECTOR3 vPOS;

	CEXTMAPDoc *pDoc = GetDocument();
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
	{
		vPOS.z -= 15.0f;
		vPOS.y = pDoc->GetHeight( vPOS.x, 0.0f, vPOS.z) + NAV_INIT_HEIGHT;
	}

	m_Camera.Move(
		vPOS.x,
		vPOS.y,
		vPOS.z,
		TRUE);

	if(!IsPlay())
		Render();
}


void CEXTMAPView::OnHotkeyEditDetail()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bDetail = TRUE;
	pBar->OnEditDetail();
}

void CEXTMAPView::OnHotkeyEditHeight()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bEditH = TRUE;
	pBar->OnEditHeight();
}

void CEXTMAPView::OnHotkeyEditTexture()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bEditTEX = TRUE;
	pBar->OnEditTexture();
}

void CEXTMAPView::OnHotkeyHeightBrush()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bHBrush = TRUE;
	pBar->OnHeightBrush();
}

void CEXTMAPView::OnHotkeyMakeFlat()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bFlatH = TRUE;
	pBar->OnMakeFlat();
}

void CEXTMAPView::OnHotkeyMakeSmooth()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->m_bSmooth = TRUE;
	pBar->OnMakeSmooth();
}

void CEXTMAPView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	CMAPOBJBar *pOBJBar = pDoc->GetMAPOBJBar();

	switch(nChar)
	{
	case 0xDD :
		if( pOBJBar->m_bInsertOBJBrush )
		{
			pOBJBar->UpdateData();
			pOBJBar->m_nBrushSize++;
			pOBJBar->UpdateData(FALSE);
		}
		else
		{
			pBar->UpdateData();
			pBar->m_nSize++;
			pBar->UpdateData(FALSE);
		}
		break;
	case 0xDB:
		if( pOBJBar->m_bInsertOBJBrush )
		{
			pOBJBar->UpdateData();
			pOBJBar->m_nBrushSize--;
			pOBJBar->UpdateData(FALSE);
		}
		else
		{
			pBar->UpdateData();
			if( pBar->m_nSize > 1 )
				pBar->m_nSize--;
			pBar->UpdateData(FALSE);
		}
		break;
	case VK_BACK:
		
		break;
	}
	CMAPView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEXTMAPView::OnHotketEditObject()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	pBar->OnEditObject();
}


void CEXTMAPView::OnEditHideMode(void)
{
	CEditHideModeDlg dlg;
	CMAPDoc *pDoc = GetDocument();
	CEXTMAPDoc *pExtDoc = GetDocument();

	dlg.InitDocument( GetDocument(), pExtDoc->m_pItem->m_HugeMAP.m_nUnitLength, pExtDoc->m_pItem->m_HugeMAP.m_nTileLength );

	dlg.DoModal();
}

void CEXTMAPView::OnEditFreezeMode(void)
{
	CEditFreezeModeDlg dlg;
	
	CMAPDoc *pDoc = GetDocument();
	CEXTMAPDoc *pExtDoc = GetDocument();

	dlg.InitDocument( GetDocument(), pExtDoc->m_pItem->m_HugeMAP.m_nUnitLength, pExtDoc->m_pItem->m_HugeMAP.m_nTileLength  );

	dlg.DoModal();
}

void CEXTMAPView::OnEditSelctObjByName(void)
{
	CMAPDoc *pDoc = GetDocument();
	CEXTMAPDoc *pExtDoc = (CEXTMAPDoc *)GetDocument();

	CSelectObjByName dlg;

	dlg.InitDocument( GetDocument(), pExtDoc->m_pItem->m_HugeMAP.m_nUnitLength, pExtDoc->m_pItem->m_HugeMAP.m_nTileLength  );

	dlg.DoModal();
}

BOOL CEXTMAPView::DoOBJSPRAY( FLOAT fPosX, FLOAT fPosZ)
{	
	return FALSE;
}

void CEXTMAPView::OnToggleBrushshape()
{
	CEXTMAPDoc *pDoc = GetDocument();
	CEXTMAPBar *pBar = pDoc->GetEXTMAPBar();
	int sel = pBar->m_cShape.GetCurSel();
	pBar->m_cShape.SetCurSel(!sel);
	pBar->UpdateData();
	pDoc->RedrawAllView();	
}

void CEXTMAPView::OnGridExplorer()
{
	CString classname = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		LoadIcon(NULL,IDI_APPLICATION));
}