// MAPSNDBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "PreviewDlg.h"
#include "MainFrm.h"
#include "MAPSNDBar.h"
#include "MAPDoc.h"
#include ".\mapsndbar.h"


VECTORMAPSNDBAR CMAPSNDBar::m_vMAPSNDBAR;
CMediaItem *CMAPSNDBar::m_pSND = NULL;

LPDIRECT3DTEXTURE9 CMAPSNDBar::m_pTEX = NULL;
CTachyonMesh CMAPSNDBar::m_MESH;

// CMAPSNDBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMAPSNDBar, CBarContent)

CMAPSNDBar::CMAPSNDBar(CWnd* pParent /*=NULL*/)
: CBarContent(CMAPSNDBar::IDD, pParent)
, m_nMode(0)
, m_strName(_T(""))
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
{
	m_pItem = NULL;
}

CMAPSNDBar::~CMAPSNDBar()
{
}

void CMAPSNDBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_INSERT, m_cInsertMode);
	DDX_Control(pDX, IDC_RADIO_EDIT, m_cEditMode);
	DDX_Radio(pDX, IDC_RADIO_INSERT, m_nMode);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_STATIC_SND, m_cName);
	DDX_Text(pDX, IDC_STATIC_SND, m_strName);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinPosX);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinPosY);
	DDX_Control(pDX, IDC_SPIN_POSZ, m_spinPosZ);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_cPosZ);
	DDX_Text(pDX, IDC_EDIT_POSX, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_strPosZ);
}


BEGIN_MESSAGE_MAP(CMAPSNDBar, CBarContent)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnBnClickedButtonBack)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_EDIT, OnBnClickedRadioEdit)
	ON_BN_CLICKED(IDC_RADIO_INSERT, OnBnClickedRadioInsert)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSZ, OnEnChangeEditData)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CMAPSNDBar 메시지 처리기입니다.

void CMAPSNDBar::InitSNDBAR()
{
	m_vMAPSNDBAR.clear();
	m_pSND = NULL;
}

void CMAPSNDBar::ReleaseSNDBAR()
{
	m_vMAPSNDBAR.clear();
	m_pSND = NULL;
}

void CMAPSNDBar::SetSND( CItemBase *pItem)
{
	BOOL bUpdate = m_pSND != pItem;

	if(bUpdate)
	{
		m_pSND = pItem && pItem->m_bType == ITEM_MEDIA ? (CMediaItem *) pItem : NULL;
		int nCount = INT(m_vMAPSNDBAR.size());

		if( m_pSND && m_pSND->GetPlayerType() != MEDIA_TSOUND )
			m_pSND = NULL;

		for( int i=0; i<nCount; i++)
			if(!m_vMAPSNDBAR[i]->m_nMode)
				m_vMAPSNDBAR[i]->UpdateItem();
	}
}

void CMAPSNDBar::UpdateItem()
{
	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		if( INT(pDoc->m_vSELSND.size()) == 1 )
		{
			MAPMAPSND::iterator itSND = m_pItem->m_pMAP->m_mapSND.find(pDoc->m_vSELSND[0]);
			CMediaItem *pItem = CItemBase::GetMediaItem((*itSND).second->m_dwMediaID);

			m_strName = pItem->m_strName;
		}
		else
			m_strName.Empty();

		if( INT(pDoc->m_vSELSND.size()) > 0 )
		{
			D3DXVECTOR3 vPOS = pDoc->GetSELSNDPOS();

			m_strPosX.Format( "%.2f", vPOS.x);
			m_strPosY.Format( "%.2f", vPOS.y);
			m_strPosZ.Format( "%.2f", vPOS.z);
		}

		UpdateData(FALSE);
	}

	EnableAllCtrl(TRUE);
}

void CMAPSNDBar::SetItem( CMAPItem *pItem)
{
	m_pItem = pItem;
	UpdateItem();
}

BOOL CMAPSNDBar::OnInitDialog()
{
	CBarContent::OnInitDialog();

	m_spinPosX.SetRange( -100, 100);
	m_spinPosY.SetRange( -100, 100);
	m_spinPosZ.SetRange( -100, 100);

	return TRUE;
}

void CMAPSNDBar::EnableAllCtrl( BOOL bEnable)
{
	m_cInsertMode.EnableWindow(bEnable);
	m_cEditMode.EnableWindow(bEnable);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		m_spinPosX.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
		m_spinPosY.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
		m_spinPosZ.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());

		m_cDelete.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
		m_cName.EnableWindow(bEnable && INT(pDoc->m_vSELSND.size()) == 1);

		m_cPosX.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
		m_cPosY.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
		m_cPosZ.EnableWindow(bEnable && !pDoc->m_vSELSND.empty());
	}
	else
	{
		m_spinPosX.EnableWindow(FALSE);
		m_spinPosY.EnableWindow(FALSE);
		m_spinPosZ.EnableWindow(FALSE);
		m_cDelete.EnableWindow(FALSE);
		m_cName.EnableWindow(FALSE);

		m_cPosX.EnableWindow(FALSE);
		m_cPosY.EnableWindow(FALSE);
		m_cPosZ.EnableWindow(FALSE);
	}
}

void CMAPSNDBar::OnBnClickedButtonBack()
{
	OnBack();
}
 
void CMAPSNDBar::OnBack()
{	
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CFrameWnd *pFrame = GetParentFrame();

	pDoc->ClearSELSND();
	pDoc->m_bEditSND = FALSE;

	pFrame->SendMessage( WM_COMMAND, ID_MAP_BACK);
	pDoc->RedrawAllView();
}

int CMAPSNDBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBarContent::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_vMAPSNDBAR.push_back(this);
	return 0;
}

void CMAPSNDBar::OnDestroy()
{
	CBarContent::OnDestroy();
	VECTORMAPSNDBAR::iterator it = m_vMAPSNDBAR.begin();
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	while( it != m_vMAPSNDBAR.end() )
		if( (*it) == this )
			m_vMAPSNDBAR.erase(it);
		else
			it++;
	pDoc->ClearSELSND();
}

void CMAPSNDBar::OnBnClickedRadioEdit()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELSND();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertSND = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPSNDBar::OnBnClickedRadioInsert()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELSND();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertSND = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPSNDBar::RenderSNDBOX( D3DXVECTOR3& vPOS)
{
	static D3DXMATRIX vWorld;

	D3DXMatrixIdentity(&vWorld);
	vWorld._41 = vPOS.x;
	vWorld._42 = vPOS.y;
	vWorld._43 = vPOS.z;

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_MESHVERTEX);

	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, m_pTEX);

	for( DWORD i=0; i<m_MESH.m_dwMeshCount; i++)
		m_MESH.Render( CMainFrame::m_3DDevice.m_pDevice, i, 0);
}

void CMAPSNDBar::OnEnChangeEditData()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	if(pDoc)
		pDoc->RedrawAllView();
}

void CMAPSNDBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinPosX )
	{
		FLOAT fPosX = (FLOAT) atof(m_strPosX);

		fPosX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosX.SetPos(0);

		m_strPosX.Format( "%.2f", fPosX);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosY )
	{
		FLOAT fPosY = (FLOAT) atof(m_strPosY);

		fPosY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosY.SetPos(0);

		m_strPosY.Format( "%.2f", fPosY);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosZ )
	{
		FLOAT fPosZ = (FLOAT) atof(m_strPosZ);

		fPosZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosZ.SetPos(0);

		m_strPosZ.Format( "%.2f", fPosZ);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CMAPSNDBar::OnBnClickedButtonDelete()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELSND.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSND::iterator itMAPSND = m_pItem->m_pMAP->m_mapSND.find(pDoc->m_vSELSND[i]);

		if( itMAPSND != m_pItem->m_pMAP->m_mapSND.end() )
		{
			delete (*itMAPSND).second;
			m_pItem->m_pMAP->m_mapSND.erase(itMAPSND);
		}
	}

	pDoc->m_vSELSND.clear();
	UpdateItem();
	pDoc->RedrawAllView();
}

LRESULT CMAPSNDBar::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_ERR_DUP_ATTRID);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}
