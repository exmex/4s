// ItemScreenShot.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "Game StudioDoc.h"
#include ".\itemscreenshot.h"
#include <d3dx9.h>


enum 
{
	TOTAL_ITEMTYPE = 1,
	TOTAL_SAVEMODE = 3,
	TOTAL_TEX_TYPE = 5,
	TOTAL_TEX_SIZE = 4
};

static char* g_szItemType[] =
{
	"Object"
};

static char* g_szSaveMode[] =
{
	"All",
	"Selected Folder",
	"Selected Item"
};

static char* g_szTextureType[] =
{
	"BMP",
	"JPG",
	"TGA",
	"PNG",
	"DDS"
};

static char* g_szTextureSize[] =
{
	"64 x 64",
	"128 x 128",
	"256 x 256",
	"512 x 512"
};

static int g_nTextureSize[] =
{
	64, 128, 256, 512
};


CItemPreview::CItemPreview()
	: m_nCamDist(1)
	, m_nCamRotateX(5)
	, m_nColorR(160)
	, m_nColorG(160)
	, m_nColorB(160)
	, m_pItem(NULL)
{

}

CItemPreview::~CItemPreview()
{
}

BOOL CItemPreview::DrawOBJ( COBJItem* pItem )
{
	static D3DXMATRIX mat(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

//	COBJItem *pItem = (COBJItem*)m_pItem;

	if( pItem == NULL )
		return FALSE;

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
				else	return FALSE;
			}
			else	return FALSE;
		}
		else	return FALSE;
	}
	else	return FALSE;

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

	D3DXVECTOR3	position( vCENTER.x, vCENTER.y, vCENTER.z - fDIST );
	D3DXVECTOR3	look = vCENTER - position;
	D3DXVECTOR3	right;
	D3DXVec3Cross( &right, &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), &look );
	D3DXVECTOR3	up;
	D3DXVec3Cross( &up, &look, &right );

	vCamera.SetPosition(
		position,
		vCENTER,
		up,
		FALSE);

	vCamera.ZoomOrthoCamera( (FLOAT)m_nCamDist * 0.1f );
	vCamera.Rotate( 0.0f, -(FLOAT)(m_nCamRotateX * 0.1f), 0.0f );

	vCamera.Activate(TRUE);


	CPreviewCtrl* pCtrl = &CGameStudioDoc::m_pPreview->m_cPreviewCtrl;
	pCtrl->m_LightCenter.SetLight(&pCtrl->m_LightCenter.m_Light);
	pCtrl->m_LightRight.SetLight(&pCtrl->m_LightRight.m_Light);
	pCtrl->m_LightTop.SetLight(&pCtrl->m_LightTop.m_Light);

	pCtrl->m_LightCenter.EnableLight();
	pCtrl->m_LightRight.EnableLight();
	pCtrl->m_LightTop.EnableLight();

	vTOBJ.Render(
		&CMainFrame::m_3DDevice,
		&vCamera);
	vTOBJ.Release();

	pCtrl->m_LightCenter.EnableLight(FALSE);
	pCtrl->m_LightRight.EnableLight(FALSE);
	pCtrl->m_LightTop.EnableLight(FALSE);

	return TRUE;
}

BOOL CItemPreview::RenderItem( COBJItem* pItem )
{
	IDirect3DDevice9* pDevice = CMainFrame::m_3DDevice.m_pDevice;

	if( pDevice == NULL )	return FALSE;

	//	Clear. BeginScene.
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( m_nColorR, m_nColorG, m_nColorB ), 1.0f, 0 );
	pDevice->BeginScene();

	//	Render.
	BOOL bDraw = DrawOBJ(pItem);

	//	EndScene. Present.
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

	return bDraw;
}

void CItemPreview::SetItem( CItemBase* _pItem )
{
	m_pItem = _pItem;
}

BEGIN_MESSAGE_MAP(CItemPreview, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CItemPreview::OnPaint()
{
	if( m_pItem && m_pItem->m_bType == ITEM_OBJECT )
		RenderItem( (COBJItem*)m_pItem );
	Default();
}



// CItemScreenShot 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemScreenShot, CDialog)
CItemScreenShot::CItemScreenShot(CWnd* pParent /*=NULL*/)
	: CDialog(CItemScreenShot::IDD, pParent)
	, m_nItemType(0)
	, m_nSaveMode(0)
	, m_nTextureType(1)
	, m_nTextureSize(2)
	, m_strPath("C:\\")
	, m_bInitDialog(false)
{
}

CItemScreenShot::~CItemScreenShot()
{
}

void CItemScreenShot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_SC_ITEMTYPE, m_ctrlItemType);
	DDX_Control(pDX, IDC_COMBO_SC_SAVEMODE, m_ctrlSaveMode);
	DDX_Control(pDX, IDC_COMBO_SC_TEXTURETYPE, m_ctrlTextureType);
	DDX_Control(pDX, IDC_COMBO_SC_TEXTURESIZE, m_ctrlTextureSize);
	DDX_Control(pDX, IDC_SC_PREVIEW, m_ctrlPreview);

	DDX_CBIndex(pDX, IDC_COMBO_SC_ITEMTYPE, m_nItemType);
	DDX_CBIndex(pDX, IDC_COMBO_SC_SAVEMODE, m_nSaveMode);
	DDX_CBIndex(pDX, IDC_COMBO_SC_TEXTURETYPE, m_nTextureType);
	DDX_CBIndex(pDX, IDC_COMBO_SC_TEXTURESIZE, m_nTextureSize);

	DDX_Text(pDX, IDC_STATIC_SC_OPDIR, m_strPath);
	DDX_Text(pDX, IDC_EDIT_SC_CAMDIST, m_ctrlPreview.m_nCamDist);
	DDX_Text(pDX, IDC_EDIT_SC_CAMROTX, m_ctrlPreview.m_nCamRotateX);
	DDX_Text(pDX, IDC_EDIT_SC_COLORR, m_ctrlPreview.m_nColorR);
	DDX_Text(pDX, IDC_EDIT_SC_COLORG, m_ctrlPreview.m_nColorG);
	DDX_Text(pDX, IDC_EDIT_SC_COLORB, m_ctrlPreview.m_nColorB);

	DDV_MinMaxInt(pDX, m_ctrlPreview.m_nCamDist, -10, 100);
	DDV_MinMaxInt(pDX, m_ctrlPreview.m_nCamRotateX, -100, 100);
	DDV_MinMaxInt(pDX, m_ctrlPreview.m_nColorR, 0, 255);
	DDV_MinMaxInt(pDX, m_ctrlPreview.m_nColorG, 0, 255);
	DDV_MinMaxInt(pDX, m_ctrlPreview.m_nColorB, 0, 255);

	DDX_Control(pDX, IDC_SPIN_SC_CAMDIST, m_spinCamDist);
	DDX_Control(pDX, IDC_SPIN_SC_CAMROTX, m_spinCamRotateX);
	DDX_Control(pDX, IDC_SPIN_SC_COLORR, m_spinColorR);
	DDX_Control(pDX, IDC_SPIN_SC_COLORG, m_spinColorG);
	DDX_Control(pDX, IDC_SPIN_SC_COLORB, m_spinColorB);
	DDX_Control(pDX, IDC_PROGRESS_SC, m_ctrlProgress);
}


void CItemScreenShot::PostNcDestroy()
{
	((CMainFrame*)AfxGetMainWnd())->m_pScreenShotDlg = NULL;
	delete this;

	CDialog::PostNcDestroy();
}


BOOL CItemScreenShot::OnInitDialog()
{
	CDialog::OnInitDialog();

	int i = 0;
	for( i=0; i<TOTAL_ITEMTYPE; ++i )
		m_ctrlItemType.AddString( g_szItemType[i] );

	for( i=0; i<TOTAL_SAVEMODE; ++i )
		m_ctrlSaveMode.AddString( g_szSaveMode[i] );

	for( i=0; i<TOTAL_TEX_TYPE; ++i )
		m_ctrlTextureType.AddString( g_szTextureType[i] );

	for( i=0; i<TOTAL_TEX_SIZE; ++i )
		m_ctrlTextureSize.AddString( g_szTextureSize[i] );

	m_spinCamDist.SetRange(-10, 100);
	m_spinCamRotateX.SetRange(-100, 100);
	m_spinColorR.SetRange(0, 255);
	m_spinColorG.SetRange(0, 255);
	m_spinColorB.SetRange(0, 255);

	UpdateData( FALSE );

	m_bInitDialog = true;

	return TRUE;
}

BOOL CItemScreenShot::Save()
{
	static BOOL (CItemScreenShot::*fp[3])(CTreeCtrl&) = 
	{
		CItemScreenShot::SaveModeAll,
		CItemScreenShot::SaveModeSelectedFolder,
		CItemScreenShot::SaveModeSelectedItem
	};

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();
	//	지금은 Object만 지원.
	CTreeCtrl& TCtrl = pMainWnd->m_pDocObject->GetTreeCtrl();

	return (this->*fp[m_nSaveMode])( TCtrl );
}

void CItemScreenShot::SearchTree( CTreeCtrl& TCtrl, HTREEITEM hItem, CString strPath )
{
	if( hItem == NULL )
		return ;

	if( TCtrl.ItemHasChildren( hItem ) )
	{
		HTREEITEM hChild = TCtrl.GetChildItem( hItem );
		if( hChild == NULL )
			return ;

		while(1)
		{
			CString cdPath;

			CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hChild );

			if( pItem->m_bMenuID != 1 && pItem->m_bType != ITEM_OBJTYPE )
			{
				cdPath.Format( "%s\\%s.%s", strPath, pItem->m_strName, g_szTextureType[ m_nTextureType ] );
				m_strPath = cdPath;
				if( ScreenShot( (COBJItem*)pItem, cdPath ) )
				{
					UpdateData(FALSE);
					m_ctrlPreview.RenderItem( (COBJItem*)pItem );
					m_ctrlProgress.StepIt();
				}
			}
			else
			{
				cdPath.Format( "%s\\%s", strPath, pItem->m_strName );
				CreateDirectory( cdPath, NULL );
				m_ctrlProgress.StepIt();
				SearchTree( TCtrl, hChild, cdPath );
			}
			HTREEITEM hNext = TCtrl.GetNextItem( hChild, TVGN_NEXT );
			if( hNext == NULL )
				break;

			hChild = hNext;
		}
	}
	else
	{
		while(1)
		{
			CString sPath;

			CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hItem );

			if( pItem->m_bMenuID != 1 && pItem->m_bType != ITEM_OBJTYPE )	//	아이템일때.
			{
				sPath.Format( "%s\\%s.%s", strPath, pItem->m_strName, g_szTextureType[ m_nTextureType ] );
				m_strPath = sPath;

				if( ScreenShot( (COBJItem*)pItem, sPath ) )
				{
					UpdateData(FALSE);
					m_ctrlPreview.RenderItem( (COBJItem*)pItem );
					m_ctrlProgress.StepIt();
				}
			}

			HTREEITEM hNext = TCtrl.GetNextItem( hItem, TVGN_NEXT );
			if( hNext == NULL )
				break;

			hItem = hNext;
		}
	}
}

BOOL CItemScreenShot::SaveModeAll( CTreeCtrl& TCtrl )
{
	m_strPath = OpenSaveDirectory();

	if( m_strPath == "" ) 
		return FALSE;

	if( m_nItemType == 0 )	//	Object Type.
	{
		m_strPath += "Object";
		CreateDirectory( m_strPath, NULL );
	}

	int child_cnt = TCtrl.GetCount();
	m_ctrlProgress.SetRange( 0, child_cnt );
	m_ctrlProgress.SetPos( 0 );
	m_ctrlProgress.SetStep( 1 );

	HTREEITEM hRoot;
	std::vector<HTREEITEM> hList;
	std::vector<CString> szList;

	hRoot = TCtrl.GetRootItem();
	while(hRoot)
	{
		if( hRoot != NULL )
		{
			CString strPath;
			strPath.Format( "%s\\%s", m_strPath, ((CItemBase*)TCtrl.GetItemData( hRoot ))->m_strName );
			CreateDirectory( strPath, NULL );
			hList.push_back( hRoot );
			szList.push_back( strPath );
		}
		else
			break;

		hRoot = TCtrl.GetNextItem( hRoot, TVGN_NEXT );
	}

	for( int i=0; i<hList.size(); ++i )
		SearchTree( TCtrl, hList[i], szList[i] );

	return TRUE;
}

BOOL CItemScreenShot::SaveModeSelectedFolder( CTreeCtrl& TCtrl )
{
	HTREEITEM hItem = TCtrl.GetSelectedItem();
	CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hItem );

	if( pItem == NULL )
	{
		MessageBox( "폴더가 선택되지 않았습니다." );
		return FALSE;
	}
	if( !TCtrl.ItemHasChildren( hItem ) )
	{
		MessageBox( "폴더가 아니거나 하위항목이 없습니다." );
		return FALSE;
	}

	//	폴더지정.
	m_strPath = OpenSaveDirectory();

	if( m_strPath == "" )
		return FALSE;

	m_strPath += pItem->m_strName;
	CreateDirectory( m_strPath, NULL );
	
	if( pItem->m_bMenuID == 1 )
		SearchTree( TCtrl, TCtrl.GetSelectedItem(), m_strPath );
	else
		return FALSE;

	return TRUE;
}

BOOL CItemScreenShot::SaveModeSelectedItem( CTreeCtrl& TCtrl )
{
	HTREEITEM hItem = TCtrl.GetSelectedItem();
	CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hItem );

	if( pItem == NULL )
	{
		MessageBox( "아이템이 선택되지 않았습니다." );
		return FALSE;
	}
	if( TCtrl.ItemHasChildren( hItem ) )
	{
		MessageBox( "폴더가 선택되어 있습니다. 아이템을 선택해주세요" );
		return FALSE;
	}

	//	폴더지정.
	m_strPath = OpenSaveDirectory();

	if( m_strPath == "" )
		return FALSE;
	
	CString strPath;
	strPath.Format( "%s\\%s.%s", m_strPath, pItem->m_strName, g_szTextureType[ m_nTextureType ] );
	m_strPath = strPath;
	UpdateData(FALSE);

	return ScreenShot( (COBJItem*)TCtrl.GetItemData( hItem ), strPath );
}

CString CItemScreenShot::OpenSaveDirectory()
{
	char szFilter[] = "모든폴더|*.FD||";
	CFileDialog	dlg( FALSE, NULL, "Save Texture!", OFN_HIDEREADONLY, szFilter );

	if( dlg.DoModal() == IDOK )
	{
		CString strPath;
		strPath = dlg.GetPathName();

		ParentDirectory( strPath );

		return strPath;
	}

	return NULL;
}

BOOL CItemScreenShot::ScreenShot( COBJItem* pItem, CString strPath )
{
	if( pItem == NULL )
	{
		MessageBox( "선택된 오브젝트가 없습니다." );
		return FALSE;
	}

	IDirect3DDevice9*	pDevice = CMainFrame::m_3DDevice.m_pDevice;

	LPDIRECT3DTEXTURE9	pTexture;

	if( FAILED( pDevice->CreateTexture(
		256, 256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, &pTexture, NULL ) ) )
	{
		MessageBox( "CreateTexture Failed!" );
		return FALSE;
	}

	LPDIRECT3DSURFACE9	dss;

	if( FAILED( pDevice->CreateDepthStencilSurface(
		256, 256, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &dss, NULL ) ) )
	{
		MessageBox( "CreateDepthStencilSurface Failed!" );
		return FALSE;
	}

	LPDIRECT3DSURFACE9	pFace;
	pTexture->GetSurfaceLevel( 0, &pFace );

	LPDIRECT3DSURFACE9	pBackBuffer, pZBuffer;
	pDevice->GetRenderTarget( 0, &pBackBuffer );
	pDevice->GetDepthStencilSurface( &pZBuffer );

	//	Set.
	pDevice->SetRenderTarget( 0, pFace );
	pDevice->SetDepthStencilSurface( dss );
	pFace->Release();
	dss->Release();

	//	Render.
	if( m_ctrlPreview.RenderItem( pItem ) )
	{
		//	SaveTexture.
		D3DXSaveTextureToFile( strPath, (D3DXIMAGE_FILEFORMAT)m_nTextureType, pTexture, NULL );
	}

	pTexture->Release();

	pDevice->SetRenderTarget( 0, pBackBuffer );
	pDevice->SetDepthStencilSurface( pZBuffer );
	pBackBuffer->Release();
	pZBuffer->Release();

	return TRUE;
}

void CItemScreenShot::ParentDirectory( CString& strPath )
{
	if( strPath.GetLength() == 0 )
		return ;

	for( int i = strPath.GetLength()-1; i>=0; --i )
	{
		if( strPath[i] != '\\' )
			strPath.Delete(i);
		else
			break;
	}
}

void CItemScreenShot::OnOK()
{
	UpdateData( TRUE );
	UpdateData( FALSE );

	Save() ? MessageBox( "Complete!" ) : MessageBox( "Failed" );

	m_ctrlPreview.OnPaint();
}

BEGIN_MESSAGE_MAP(CItemScreenShot, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SC_ITEMTYPE, OnCbnSelchangeComboScItemtype)
	ON_CBN_SELCHANGE(IDC_COMBO_SC_SAVEMODE, OnCbnSelchangeComboScSavemode)
	ON_CBN_SELCHANGE(IDC_COMBO_SC_TEXTURETYPE, OnCbnSelchangeComboScTexturetype)
	ON_CBN_SELCHANGE(IDC_COMBO_SC_TEXTURESIZE, OnCbnSelchangeComboScTexturesize)
	ON_EN_CHANGE(IDC_EDIT_SC_COLORR, OnEnChangeEditScColorr)
	ON_EN_CHANGE(IDC_EDIT_SC_COLORG, OnEnChangeEditScColorg)
	ON_EN_CHANGE(IDC_EDIT_SC_COLORB, OnEnChangeEditScColorb)
	ON_EN_CHANGE(IDC_EDIT_SC_CAMDIST, OnEnChangeEditScCamdist)
	ON_EN_CHANGE(IDC_EDIT_SC_CAMROTX, OnEnChangeEditScCamrotx)
END_MESSAGE_MAP()


// CItemScreenShot 메시지 처리기입니다.

void CItemScreenShot::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
}

void CItemScreenShot::OnCbnSelchangeComboScItemtype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CItemScreenShot::OnCbnSelchangeComboScSavemode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CItemScreenShot::OnCbnSelchangeComboScTexturetype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CItemScreenShot::OnCbnSelchangeComboScTexturesize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CItemScreenShot::OnEnChangeEditScColorr()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bInitDialog )
	{
		UpdateData();
		m_ctrlPreview.OnPaint();
	}
}

void CItemScreenShot::OnEnChangeEditScColorg()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bInitDialog )
	{
		UpdateData();
		m_ctrlPreview.OnPaint();
	}
}

void CItemScreenShot::OnEnChangeEditScColorb()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bInitDialog )
	{
		UpdateData();
		m_ctrlPreview.OnPaint();
	}
}

void CItemScreenShot::OnEnChangeEditScCamdist()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bInitDialog )
	{
		UpdateData();
		m_ctrlPreview.OnPaint();
	}
}

void CItemScreenShot::OnEnChangeEditScCamrotx()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_bInitDialog )
	{
		UpdateData();
		m_ctrlPreview.OnPaint();
	}
}
