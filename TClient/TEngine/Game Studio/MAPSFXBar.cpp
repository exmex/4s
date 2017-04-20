// MAPSFXBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "PreviewDlg.h"
#include "MainFrm.h"
#include "MAPSFXBar.h"
#include "MAPDoc.h"
#include ".\mapsfxbar.h"


VECTORMAPSFXBAR CMAPSFXBar::m_vMAPSFXBAR;
CSFXItem *CMAPSFXBar::m_pSFX = NULL;

LPDIRECT3DTEXTURE9 CMAPSFXBar::m_pTEX = NULL;
CTachyonMesh CMAPSFXBar::m_MESH;

// CMAPSFXBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMAPSFXBar, CBarContent)

CMAPSFXBar::CMAPSFXBar(CWnd* pParent /*=NULL*/)
: CBarContent(CMAPSFXBar::IDD, pParent)
, m_nMode(0)
, m_strName(_T(""))
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
, m_strRotX(_T(""))
, m_strRotY(_T(""))
, m_strRotZ(_T(""))
, m_bBias(0)
{
	m_pItem = NULL;
}

CMAPSFXBar::~CMAPSFXBar()
{
}

void CMAPSFXBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_INSERT, m_cInsertMode);
	DDX_Control(pDX, IDC_RADIO_EDIT, m_cEditMode);
	DDX_Radio(pDX, IDC_RADIO_INSERT, m_nMode);
	DDX_Control(pDX, IDC_STATIC_SFX, m_cName);
	DDX_Text(pDX, IDC_STATIC_SFX, m_strName);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Text(pDX, IDC_EDIT_POSX, m_strPosX);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Text(pDX, IDC_EDIT_POSY, m_strPosY);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_cPosZ);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_strPosZ);
	DDX_Control(pDX, IDC_SPIN_POSZ, m_spinPosZ);
	DDX_Control(pDX, IDC_EDIT_ROTX, m_cRotX);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_strRotX);
	DDX_Control(pDX, IDC_SPIN_ROTX, m_spinRotX);
	DDX_Control(pDX, IDC_EDIT_ROTY, m_cRotY);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_strRotY);
	DDX_Control(pDX, IDC_SPIN_ROTY, m_spinRotY);
	DDX_Control(pDX, IDC_EDIT_ROTZ, m_cRotZ);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_strRotZ);
	DDX_Control(pDX, IDC_SPIN_ROTZ, m_spinRotZ);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EDIT_BIAS, m_cBias);
	DDX_Text(pDX, IDC_EDIT_BIAS, m_bBias);
	DDX_Control(pDX, IDC_SPIN_BIAS, m_spinBias);
}


BEGIN_MESSAGE_MAP(CMAPSFXBar, CBarContent)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnBnClickedButtonBack)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_EDIT, OnBnClickedRadioEdit)
	ON_BN_CLICKED(IDC_RADIO_INSERT, OnBnClickedRadioInsert)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSZ, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, OnEnChangeEditData)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_BIAS, OnEnChangeEditBias)
END_MESSAGE_MAP()


// CMAPSFXBar 메시지 처리기입니다.

void CMAPSFXBar::InitSFXBAR()
{
	m_vMAPSFXBAR.clear();
	m_pSFX = NULL;
}

void CMAPSFXBar::ReleaseSFXBAR()
{
	m_vMAPSFXBAR.clear();
	m_pSFX = NULL;
}

void CMAPSFXBar::SetSFX( CItemBase *pItem)
{
	BOOL bUpdate = m_pSFX != pItem;

	if(bUpdate)
	{
		m_pSFX = pItem && pItem->m_bType == ITEM_SFX ? (CSFXItem *) pItem : NULL;
		int nCount = INT(m_vMAPSFXBAR.size());

		for( int i=0; i<nCount; i++)
			if(!m_vMAPSFXBAR[i]->m_nMode)
				m_vMAPSFXBAR[i]->UpdateItem();
	}
}

void CMAPSFXBar::UpdateItem()
{
	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		if( INT(pDoc->m_vSELSFX.size()) == 1 )
		{
			MAPMAPSFX::iterator itSFX = pDoc->m_pMAP->m_pMAP->m_mapSFX.find(pDoc->m_vSELSFX[0]);
			MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapSFX.find(pDoc->m_vSELSFX[0]);
			CSFXItem *pItem = CItemBase::GetSFXItem((*itID).second);

			m_bBias = (*itSFX).second->m_bBias;
			m_strName = pItem->m_strName;
		}
		else
			m_strName.Empty();

		if( INT(pDoc->m_vSELSFX.size()) > 0 )
		{
			D3DXVECTOR3 vROT = pDoc->GetSELSFXROT();
			D3DXVECTOR3 vPOS = pDoc->GetSELSFXPOS();

			m_strPosX.Format( "%.2f", vPOS.x);
			m_strPosY.Format( "%.2f", vPOS.y);
			m_strPosZ.Format( "%.2f", vPOS.z);
			m_strRotX.Format( "%.2f", vROT.x * 180.0f / D3DX_PI);
			m_strRotY.Format( "%.2f", vROT.y * 180.0f / D3DX_PI);
			m_strRotZ.Format( "%.2f", vROT.z * 180.0f / D3DX_PI);
		}

		UpdateData(FALSE);
	}

	EnableAllCtrl(TRUE);
}

void CMAPSFXBar::SetItem( CMAPItem *pItem)
{
	m_pItem = pItem;
	UpdateItem();
}

BOOL CMAPSFXBar::OnInitDialog()
{
	CBarContent::OnInitDialog();

	m_spinPosX.SetRange( -100, 100);
	m_spinPosY.SetRange( -100, 100);
	m_spinPosZ.SetRange( -100, 100);
	m_spinRotX.SetRange( -100, 100);
	m_spinRotY.SetRange( -100, 100);
	m_spinRotZ.SetRange( -100, 100);
	m_spinBias.SetRange( 0, 255);

	return TRUE;
}

void CMAPSFXBar::EnableAllCtrl( BOOL bEnable)
{
	m_cInsertMode.EnableWindow(bEnable);
	m_cEditMode.EnableWindow(bEnable);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		m_spinPosX.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_spinPosY.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_spinPosZ.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_spinRotX.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_spinRotY.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_spinRotZ.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cDelete.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());

		m_spinBias.EnableWindow(bEnable && INT(pDoc->m_vSELSFX.size()) == 1);
		m_cBias.EnableWindow(bEnable && INT(pDoc->m_vSELSFX.size()) == 1);
		m_cName.EnableWindow(bEnable && INT(pDoc->m_vSELSFX.size()) == 1);

		m_cPosX.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cPosY.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cPosZ.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cRotX.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cRotY.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
		m_cRotZ.EnableWindow(bEnable && !pDoc->m_vSELSFX.empty());
	}
	else
	{
		m_spinPosX.EnableWindow(FALSE);
		m_spinPosY.EnableWindow(FALSE);
		m_spinPosZ.EnableWindow(FALSE);
		m_spinRotX.EnableWindow(FALSE);
		m_spinRotY.EnableWindow(FALSE);
		m_spinRotZ.EnableWindow(FALSE);
		m_spinBias.EnableWindow(FALSE);
		m_cDelete.EnableWindow(FALSE);
		m_cName.EnableWindow(FALSE);

		m_cPosX.EnableWindow(FALSE);
		m_cPosY.EnableWindow(FALSE);
		m_cPosZ.EnableWindow(FALSE);
		m_cRotX.EnableWindow(FALSE);
		m_cRotY.EnableWindow(FALSE);
		m_cRotZ.EnableWindow(FALSE);
		m_cBias.EnableWindow(FALSE);
	}
}

void CMAPSFXBar::OnBnClickedButtonBack()
{
	OnBack();
}

void CMAPSFXBar::OnBack()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CFrameWnd *pFrame = GetParentFrame();

	pDoc->ClearSELSFX();
	pDoc->m_bEditSFX = FALSE;

	pFrame->SendMessage( WM_COMMAND, ID_MAP_BACK);
	pDoc->RedrawAllView();
}

int CMAPSFXBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBarContent::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_vMAPSFXBAR.push_back(this);
	return 0;
}

void CMAPSFXBar::OnDestroy()
{
	CBarContent::OnDestroy();
	VECTORMAPSFXBAR::iterator it = m_vMAPSFXBAR.begin();
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	while( it != m_vMAPSFXBAR.end() )
		if( (*it) == this )
			m_vMAPSFXBAR.erase(it);
		else
			it++;
	pDoc->ClearSELSFX();
}

void CMAPSFXBar::OnBnClickedRadioEdit()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELSFX();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertSFX = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPSFXBar::OnBnClickedRadioInsert()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELSFX();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertSFX = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPSFXBar::RenderSFXBOX( D3DXVECTOR3& vPOS, D3DXVECTOR3& vROT)
{
	static D3DXMATRIX vWorld;
	static D3DXMATRIX vRotX;
	static D3DXMATRIX vRotY;
	static D3DXMATRIX vRotZ;

	D3DXMatrixIdentity(&vWorld);
	vWorld._41 = vPOS.x;
	vWorld._42 = vPOS.y;
	vWorld._43 = vPOS.z;

	D3DXMatrixRotationX(
		&vRotX,
		vROT.x);

	D3DXMatrixRotationY(
		&vRotY,
		vROT.y);

	D3DXMatrixRotationZ(
		&vRotZ,
		vROT.z);
	vWorld = vRotX * vRotY * vRotZ * vWorld;

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

void CMAPSFXBar::OnEnChangeEditData()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	if(pDoc)
		pDoc->RedrawAllView();
}

void CMAPSFXBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

	if( pScrollBar == (CScrollBar *) &m_spinRotX )
	{
		FLOAT fRotX = (FLOAT) atof(m_strRotX);

		fRotX += (FLOAT) INT(nPos);
		m_spinRotX.SetPos(0);

		m_strRotX.Format( "%.2f", fRotX);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotY )
	{
		FLOAT fRotY = (FLOAT) atof(m_strRotY);

		fRotY += (FLOAT) INT(nPos);
		m_spinRotY.SetPos(0);

		m_strRotY.Format( "%.2f", fRotY);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotZ )
	{
		FLOAT fRotZ = (FLOAT) atof(m_strRotZ);

		fRotZ += (FLOAT) INT(nPos);
		m_spinRotZ.SetPos(0);

		m_strRotZ.Format( "%.2f", fRotZ);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CMAPSFXBar::OnBnClickedButtonDelete()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELSFX.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPSFX::iterator itMAPSFX = m_pItem->m_pMAP->m_mapSFX.find(pDoc->m_vSELSFX[i]);
		MAPDWORD::iterator itID = m_pItem->m_mapSFX.find(pDoc->m_vSELSFX[i]);

		if( itMAPSFX != m_pItem->m_pMAP->m_mapSFX.end() )
		{
			delete (*itMAPSFX).second;
			m_pItem->m_pMAP->m_mapSFX.erase(itMAPSFX);
		}

		if( itID != m_pItem->m_mapSFX.end() )
			m_pItem->m_mapSFX.erase(itID);
	}

	pDoc->m_vSELSFX.clear();
	UpdateItem();
	pDoc->RedrawAllView();
}

void CMAPSFXBar::OnEnChangeEditBias()
{
	if(m_pItem)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		MAPMAPSFX::iterator itSFX = m_pItem->m_pMAP->m_mapSFX.find(pDoc->m_vSELSFX[0]);

		UpdateData();
		(*itSFX).second->m_bBias = m_bBias;
	}
}

LRESULT CMAPSFXBar::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_ERR_DUP_ATTRID);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}
