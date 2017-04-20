// ParticleBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "MainFrm.h"
#include "SFXFrame.h"
#include "SFXDoc.h"
#include "ParticleBar.h"
#include ".\particlebar.h"


// CParticleBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CParticleBar, CBarContent)
CParticleBar::CParticleBar(CWnd* pParent /*=NULL*/)
: CBarContent(CParticleBar::IDD, pParent)
, m_nSKind(0)
, m_dwGTick(0)
, m_dwGCnt(0)
, m_strInX(_T(""))
, m_strInY(_T(""))
, m_strInZ(_T(""))
, m_strOutX(_T(""))
, m_strOutY(_T(""))
, m_strOutZ(_T(""))
, m_dwLifeTime(0)
, m_strEPosX(_T(""))
, m_strEPosY(_T(""))
, m_strEPosZ(_T(""))
, m_strERotX(_T(""))
, m_strERotY(_T(""))
, m_strERotZ(_T(""))
, m_strEScaleX(_T(""))
, m_strEScaleY(_T(""))
, m_strEScaleZ(_T(""))
, m_dwETime(0)
, m_strSANI(_T(""))
, m_bStrict(FALSE)
, m_bFollow(FALSE)
, m_strFollow(_T(""))
{
	m_pItem = NULL;
}

CParticleBar::~CParticleBar()
{
}

void CParticleBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SETBASIC, m_cBasic);
	DDX_Control(pDX, IDC_COMBO_SPRAYKIND, m_cSKind);
	DDX_Control(pDX, IDC_EDIT_SANI, m_cSANI);
	DDX_Control(pDX, IDC_BUTTON_DELSANI, m_cDELSANI);
	DDX_Control(pDX, IDC_EDIT_PGT, m_cGTick);
	DDX_Control(pDX, IDC_SPIN_PGT, m_spinGTick);
	DDX_Control(pDX, IDC_EDIT_PGC, m_cGCnt);
	DDX_Control(pDX, IDC_SPIN_PGC, m_spinGCnt);
	DDX_Control(pDX, IDC_EDIT_MINX, m_cInX);
	DDX_Control(pDX, IDC_SPIN_MINX, m_spinInX);
	DDX_Control(pDX, IDC_EDIT_MINY, m_cInY);
	DDX_Control(pDX, IDC_SPIN_MINY, m_spinInY);
	DDX_Control(pDX, IDC_EDIT_MINZ, m_cInZ);
	DDX_Control(pDX, IDC_SPIN_MINZ, m_spinInZ);
	DDX_Control(pDX, IDC_EDIT_MAXX, m_cOutX);
	DDX_Control(pDX, IDC_SPIN_MAXX, m_spinOutX);
	DDX_Control(pDX, IDC_EDIT_MAXY, m_cOutY);
	DDX_Control(pDX, IDC_SPIN_MAXY, m_spinOutY);
	DDX_Control(pDX, IDC_EDIT_MAXZ, m_cOutZ);
	DDX_Control(pDX, IDC_SPIN_MAXZ, m_spinOutZ);
	DDX_Control(pDX, IDC_EDIT_PLT, m_cLifeTime);
	DDX_Control(pDX, IDC_SPIN_PLT, m_spinLifeTime);
	DDX_Control(pDX, IDC_EDIT_PEPX, m_cEPosX);
	DDX_Control(pDX, IDC_SPIN_PEPX, m_spinEPosX);
	DDX_Control(pDX, IDC_EDIT_PEPY, m_cEPosY);
	DDX_Control(pDX, IDC_SPIN_PEPY, m_spinEPosY);
	DDX_Control(pDX, IDC_EDIT_PEPZ, m_cEPosZ);
	DDX_Control(pDX, IDC_SPIN_PEPZ, m_spinEPosZ);
	DDX_Control(pDX, IDC_EDIT_PERX, m_cERotX);
	DDX_Control(pDX, IDC_SPIN_PERX, m_spinERotX);
	DDX_Control(pDX, IDC_EDIT_PERY, m_cERotY);
	DDX_Control(pDX, IDC_SPIN_PERY, m_spinERotY);
	DDX_Control(pDX, IDC_EDIT_PERZ, m_cERotZ);
	DDX_Control(pDX, IDC_SPIN_PERZ, m_spinERotZ);
	DDX_Control(pDX, IDC_EDIT_PESX, m_cEScaleX);
	DDX_Control(pDX, IDC_SPIN_PESX, m_spinEScaleX);
	DDX_Control(pDX, IDC_EDIT_PESY, m_cEScaleY);
	DDX_Control(pDX, IDC_SPIN_PESY, m_spinEScaleY);
	DDX_Control(pDX, IDC_EDIT_PESZ, m_cEScaleZ);
	DDX_Control(pDX, IDC_SPIN_PESZ, m_spinEScaleZ);
	DDX_Control(pDX, IDC_EDIT_PET, m_cETime);
	DDX_Control(pDX, IDC_SPIN_PET, m_spinETime);
	DDX_CBIndex(pDX, IDC_COMBO_SPRAYKIND, m_nSKind);
	DDX_Text(pDX, IDC_EDIT_PGT, m_dwGTick);
	DDX_Text(pDX, IDC_EDIT_PGC, m_dwGCnt);
	DDX_Text(pDX, IDC_EDIT_MINX, m_strInX);
	DDX_Text(pDX, IDC_EDIT_MINY, m_strInY);
	DDX_Text(pDX, IDC_EDIT_MINZ, m_strInZ);
	DDX_Text(pDX, IDC_EDIT_MAXX, m_strOutX);
	DDX_Text(pDX, IDC_EDIT_MAXY, m_strOutY);
	DDX_Text(pDX, IDC_EDIT_MAXZ, m_strOutZ);
	DDX_Text(pDX, IDC_EDIT_PLT, m_dwLifeTime);
	DDX_Text(pDX, IDC_EDIT_PEPX, m_strEPosX);
	DDX_Text(pDX, IDC_EDIT_PEPY, m_strEPosY);
	DDX_Text(pDX, IDC_EDIT_PEPZ, m_strEPosZ);
	DDX_Text(pDX, IDC_EDIT_PERX, m_strERotX);
	DDX_Text(pDX, IDC_EDIT_PERY, m_strERotY);
	DDX_Text(pDX, IDC_EDIT_PERZ, m_strERotZ);
	DDX_Text(pDX, IDC_EDIT_PESX, m_strEScaleX);
	DDX_Text(pDX, IDC_EDIT_PESY, m_strEScaleY);
	DDX_Text(pDX, IDC_EDIT_PESZ, m_strEScaleZ);
	DDX_Text(pDX, IDC_EDIT_PET, m_dwETime);
	DDX_Text(pDX, IDC_EDIT_SANI, m_strSANI);
	DDX_Check(pDX, IDC_CHECK_STRICT, m_bStrict);
	DDX_Check(pDX, IDC_CHECK_FOLLOW, m_bFollow);
	DDX_Control(pDX, IDC_EDIT_FOLLOW, m_cFValue);
	DDX_Text(pDX, IDC_EDIT_FOLLOW, m_strFollow);
	DDX_Control(pDX, IDC_SPIN_FOLLOW, m_spinFollow);
	DDX_Control(pDX, IDC_CHECK_STRICT, m_cStrict);
	DDX_Control(pDX, IDC_CHECK_FOLLOW, m_cFollow);
}


BEGIN_MESSAGE_MAP(CParticleBar, CBarContent)
	ON_BN_CLICKED(IDC_BUTTON_SETBASIC, OnBnClickedButtonSetbasic)
	ON_BN_CLICKED(IDC_BUTTON_DELSANI, OnBnClickedButtonDelsani)
	ON_EN_CHANGE(IDC_EDIT_MAXX, OnEnChangeEditMaxx)
	ON_EN_CHANGE(IDC_EDIT_MAXY, OnEnChangeEditMaxy)
	ON_EN_CHANGE(IDC_EDIT_MAXZ, OnEnChangeEditMaxz)
	ON_EN_CHANGE(IDC_EDIT_MINX, OnEnChangeEditMinx)
	ON_EN_CHANGE(IDC_EDIT_MINY, OnEnChangeEditMiny)
	ON_EN_CHANGE(IDC_EDIT_MINZ, OnEnChangeEditMinz)
	ON_EN_CHANGE(IDC_EDIT_PEPX, OnEnChangeEditPepx)
	ON_EN_CHANGE(IDC_EDIT_PEPY, OnEnChangeEditPepy)
	ON_EN_CHANGE(IDC_EDIT_PEPZ, OnEnChangeEditPepz)
	ON_EN_CHANGE(IDC_EDIT_PERX, OnEnChangeEditPerx)
	ON_EN_CHANGE(IDC_EDIT_PERY, OnEnChangeEditPery)
	ON_EN_CHANGE(IDC_EDIT_PERZ, OnEnChangeEditPerz)
	ON_EN_CHANGE(IDC_EDIT_PESX, OnEnChangeEditPesx)
	ON_EN_CHANGE(IDC_EDIT_PESY, OnEnChangeEditPesy)
	ON_EN_CHANGE(IDC_EDIT_PESZ, OnEnChangeEditPesz)
	ON_EN_CHANGE(IDC_EDIT_PET, OnEnChangeEditPet)
	ON_EN_CHANGE(IDC_EDIT_PGC, OnEnChangeEditPgc)
	ON_EN_CHANGE(IDC_EDIT_PGT, OnEnChangeEditPgt)
	ON_EN_CHANGE(IDC_EDIT_PLT, OnEnChangeEditPlt)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_SPRAYKIND, OnCbnSelchangeComboSpraykind)
	ON_BN_CLICKED(IDC_CHECK_FOLLOW, OnBnClickedCheckFollow)
	ON_EN_CHANGE(IDC_EDIT_FOLLOW, OnEnChangeEditFollow)
	ON_BN_CLICKED(IDC_CHECK_STRICT, OnBnClickedCheckStatic)
END_MESSAGE_MAP()


// CParticleBar 메시지 처리기입니다.

void CParticleBar::OnBnClickedButtonSetbasic()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetParentFrame();

	pFrame->m_wndParticleBar.ShowWindow(SW_HIDE);
	pFrame->m_wndControlBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndControlBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

BOOL CParticleBar::OnInitDialog()
{
	CBarContent::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDELSANI.SetIcon(hIcon);

	m_cSANI.m_DropTarget.Register(&m_cSANI);
	m_cSANI.m_pDragEnter = OnANIDragOver;
	m_cSANI.m_pDragOver = OnANIDragOver;
	m_cSANI.m_pDrop = OnANIDrop;
	m_cSANI.m_pLeave = OnANIDragLeave;

	m_cSKind.InsertString( 0, "Box");
	m_cSKind.InsertString( 1, "Sphere");
	m_cSKind.InsertString( 2, "Cylinder");

	m_spinEScaleX.SetRange( -100, 100);
	m_spinEScaleY.SetRange( -100, 100);
	m_spinEScaleZ.SetRange( -100, 100);
	m_spinFollow.SetRange( -100, 100);
	m_spinEPosX.SetRange( -100, 100);
	m_spinEPosY.SetRange( -100, 100);
	m_spinEPosZ.SetRange( -100, 100);
	m_spinERotX.SetRange( -100, 100);
	m_spinERotY.SetRange( -100, 100);
	m_spinERotZ.SetRange( -100, 100);
	m_spinETime.SetRange( 0, 10000);

	m_spinLifeTime.SetRange( 0, 10000);
	m_spinGTick.SetRange( 0, 10000);
	m_spinGCnt.SetRange( 0, 10000);
	m_spinOutX.SetRange( -100, 100);
	m_spinOutY.SetRange( -100, 100);
	m_spinOutZ.SetRange( -100, 100);
	m_spinInX.SetRange( -100, 100);
	m_spinInY.SetRange( -100, 100);
	m_spinInZ.SetRange( -100, 100);

	return TRUE;
}

void CParticleBar::SetItem( CSFXItem *pItem)
{
	m_pItem = pItem;

	CAnimationItem *pANI = CItemBase::GetAnimationItem(m_pItem->m_dwSANISRC);
	if(pANI)
		m_strSANI = pANI->m_strName;
	else
		m_strSANI.LoadString(IDS_NOTUSE);

	m_strEScaleX.Format( "%.2f", m_pItem->m_SFX.m_vERRScale.x);
	m_strEScaleY.Format( "%.2f", m_pItem->m_SFX.m_vERRScale.y);
	m_strEScaleZ.Format( "%.2f", m_pItem->m_SFX.m_vERRScale.z);
	m_strEPosX.Format( "%.2f", m_pItem->m_SFX.m_vERRPos.x);
	m_strEPosY.Format( "%.2f", m_pItem->m_SFX.m_vERRPos.y);
	m_strEPosZ.Format( "%.2f", m_pItem->m_SFX.m_vERRPos.z);
	m_strERotX.Format( "%.2f", m_pItem->m_SFX.m_vERRRot.x * 180.0f / D3DX_PI);
	m_strERotY.Format( "%.2f", m_pItem->m_SFX.m_vERRRot.y * 180.0f / D3DX_PI);
	m_strERotZ.Format( "%.2f", m_pItem->m_SFX.m_vERRRot.z * 180.0f / D3DX_PI);
	m_strOutX.Format( "%.2f", m_pItem->m_SFX.m_vOutBound.x);
	m_strOutY.Format( "%.2f", m_pItem->m_SFX.m_vOutBound.y);
	m_strOutZ.Format( "%.2f", m_pItem->m_SFX.m_vOutBound.z);
	m_strInX.Format( "%.2f", m_pItem->m_SFX.m_vInBound.x);
	m_strInY.Format( "%.2f", m_pItem->m_SFX.m_vInBound.y);
	m_strInZ.Format( "%.2f", m_pItem->m_SFX.m_vInBound.z);
	m_strFollow.Format( "%.2f", m_pItem->m_SFX.m_fFollow);

	m_dwLifeTime = m_pItem->m_SFX.m_dwPLifeTime;
	m_dwGTick = m_pItem->m_SFX.m_dwGTime;
	m_dwETime = m_pItem->m_SFX.m_dwERRTime;
	m_dwGCnt = m_pItem->m_SFX.m_dwGCnt;

	m_bStrict = m_pItem->m_SFX.m_bStrict;
	m_bFollow = m_pItem->m_SFX.m_bFollow;
	m_nSKind = m_pItem->m_SFX.m_bSprayType;

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CParticleBar::InitSFX()
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	CSFXBar *pBar = pDoc->GetSFXBar();

	m_pItem->m_TSFX.InitSFX(
		&CMainFrame::m_3DDevice,
		&m_pItem->m_SFX);

	switch(pDoc->m_pItem->m_SFX.m_bSFXType)
	{
	case SFX_PARTICLE	:
		{
			pCtrl->SetPos(0);
			pBar->UpdateCtrl(0);
		}

		break;

	case SFX_SINGLE		: pBar->UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos()); break;
	}

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void CParticleBar::EnableAllCtrl( BOOL bEnable)
{
	m_spinOutY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType == SPRAY_BOX);
	m_spinInY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType == SPRAY_BOX);
	m_cOutY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType == SPRAY_BOX);
	m_cInY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType == SPRAY_BOX);

	m_spinOutZ.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType != SPRAY_SPHERE);
	m_spinInZ.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType != SPRAY_SPHERE);
	m_cOutZ.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType != SPRAY_SPHERE);
	m_cInZ.EnableWindow(bEnable && m_pItem->m_SFX.m_bSprayType != SPRAY_SPHERE);

	m_cDELSANI.EnableWindow(bEnable && m_pItem->m_dwSANISRC != 0);

	m_spinFollow.EnableWindow(bEnable && m_bFollow);
	m_cFValue.EnableWindow(bEnable && m_bFollow);
	m_cStrict.EnableWindow(bEnable && m_bFollow);

	m_spinOutX.EnableWindow(bEnable);
	m_spinInX.EnableWindow(bEnable);
	m_cOutX.EnableWindow(bEnable);
	m_cInX.EnableWindow(bEnable);

	m_cSANI.EnableWindow(bEnable);
	m_spinEScaleX.EnableWindow(bEnable);
	m_spinEScaleY.EnableWindow(bEnable);
	m_spinEScaleZ.EnableWindow(bEnable);
	m_spinEPosX.EnableWindow(bEnable);
	m_spinEPosY.EnableWindow(bEnable);
	m_spinEPosZ.EnableWindow(bEnable);
	m_spinERotX.EnableWindow(bEnable);
	m_spinERotY.EnableWindow(bEnable);
	m_spinERotZ.EnableWindow(bEnable);
	m_spinETime.EnableWindow(bEnable);

	m_spinLifeTime.EnableWindow(bEnable);
	m_spinGTick.EnableWindow(bEnable);
	m_spinGCnt.EnableWindow(bEnable);

	m_cEScaleX.EnableWindow(bEnable);
	m_cEScaleY.EnableWindow(bEnable);
	m_cEScaleZ.EnableWindow(bEnable);
	m_cEPosX.EnableWindow(bEnable);
	m_cEPosY.EnableWindow(bEnable);
	m_cEPosZ.EnableWindow(bEnable);
	m_cERotX.EnableWindow(bEnable);
	m_cERotY.EnableWindow(bEnable);
	m_cERotZ.EnableWindow(bEnable);
	m_cETime.EnableWindow(bEnable);

	m_cLifeTime.EnableWindow(bEnable);
	m_cGTick.EnableWindow(bEnable);
	m_cGCnt.EnableWindow(bEnable);

	m_cFollow.EnableWindow(bEnable);
	m_cSKind.EnableWindow(bEnable);
	m_cBasic.EnableWindow(bEnable);
}

void CALLBACK CParticleBar::OnANIDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

BOOL CParticleBar::CanANIDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_ANIMATION;
}

DROPEFFECT CALLBACK CParticleBar::OnANIDragOver( CGSEditCtrl *pEdit,
												 COleDataObject *pDataObject,
												 DWORD dwKeyState,
												 CPoint point)
{
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pEdit->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

BOOL CALLBACK CParticleBar::OnANIDrop( CGSEditCtrl *pEdit,
									   COleDataObject* pDataObject,
									   DROPEFFECT dropEffect,
									   CPoint point)
{
	CParticleBar *pBar = (CParticleBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetSANISRC((CAnimationItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnANIDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

void CParticleBar::SetSANISRC( CAnimationItem *pItem)
{
	m_pItem->m_SFX.m_pSprayANI = &pItem->m_Aniset;
	m_pItem->m_dwSANISRC = pItem->m_dwID;
	InitSFX();

	m_strSANI = pItem->m_strName;
	UpdateData(FALSE);

	EnableAllCtrl(TRUE);
}

void CParticleBar::OnBnClickedButtonDelsani()
{
	m_pItem->m_SFX.m_pSprayANI = NULL;
	m_pItem->m_dwSANISRC = 0;
	InitSFX();

	m_strSANI.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);

	EnableAllCtrl(TRUE);
}

void CParticleBar::OnBnClickedCheckStatic()
{
	UpdateData();

	m_pItem->m_SFX.m_bStrict = (BYTE) m_bStrict;
	InitSFX();
}

void CParticleBar::OnBnClickedCheckFollow()
{
	UpdateData();
	m_pItem->m_SFX.m_bFollow = (BYTE) m_bFollow;
	InitSFX();

	EnableAllCtrl(TRUE);
}

void CParticleBar::OnEnChangeEditMaxx()
{
	UpdateData();
	m_pItem->m_SFX.m_vOutBound.x = (FLOAT) atof(m_strOutX);
	InitSFX();
}

void CParticleBar::OnEnChangeEditMaxy()
{
	UpdateData();
	m_pItem->m_SFX.m_vOutBound.y = (FLOAT) atof(m_strOutY);
	InitSFX();
}

void CParticleBar::OnEnChangeEditMaxz()
{
	UpdateData();
	m_pItem->m_SFX.m_vOutBound.z = (FLOAT) atof(m_strOutZ);
	InitSFX();
}

void CParticleBar::OnEnChangeEditMinx()
{
	UpdateData();
	m_pItem->m_SFX.m_vInBound.x = (FLOAT) atof(m_strInX);
	InitSFX();
}

void CParticleBar::OnEnChangeEditMiny()
{
	UpdateData();
	m_pItem->m_SFX.m_vInBound.y = (FLOAT) atof(m_strInY);
	InitSFX();
}

void CParticleBar::OnEnChangeEditMinz()
{
	UpdateData();
	m_pItem->m_SFX.m_vInBound.z = (FLOAT) atof(m_strInZ);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPepx()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRPos.x = (FLOAT) atof(m_strEPosX);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPepy()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRPos.y = (FLOAT) atof(m_strEPosY);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPepz()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRPos.z = (FLOAT) atof(m_strEPosZ);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPerx()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRRot.x = (FLOAT) atof(m_strERotX) * D3DX_PI / 180.0f;
	InitSFX();
}

void CParticleBar::OnEnChangeEditPery()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRRot.y = (FLOAT) atof(m_strERotY) * D3DX_PI / 180.0f;
	InitSFX();
}

void CParticleBar::OnEnChangeEditPerz()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRRot.z = (FLOAT) atof(m_strERotZ) * D3DX_PI / 180.0f;
	InitSFX();
}

void CParticleBar::OnEnChangeEditPesx()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRScale.x = (FLOAT) atof(m_strEScaleX);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPesy()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRScale.y = (FLOAT) atof(m_strEScaleY);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPesz()
{
	UpdateData();
	m_pItem->m_SFX.m_vERRScale.z = (FLOAT) atof(m_strEScaleZ);
	InitSFX();
}

void CParticleBar::OnEnChangeEditPet()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_dwERRTime = m_dwETime;
		InitSFX();
	}
}

void CParticleBar::OnEnChangeEditPgc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_dwGCnt = m_dwGCnt;
		InitSFX();
	}
}

void CParticleBar::OnEnChangeEditPgt()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_dwGTime = m_dwGTick;
		InitSFX();
	}
}

void CParticleBar::OnEnChangeEditPlt()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_dwPLifeTime = m_dwLifeTime;
		InitSFX();
	}
}

void CParticleBar::OnEnChangeEditFollow()
{
	UpdateData();
	m_pItem->m_SFX.m_fFollow = (FLOAT) atof(m_strFollow);
	InitSFX();
}

void CParticleBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinEScaleX )
	{
		FLOAT fEScaleX = (FLOAT) atof(m_strEScaleX);

		fEScaleX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEScaleX.SetPos(0);

		m_strEScaleX.Format( "%.2f", fEScaleX);
		UpdateData(FALSE);
		OnEnChangeEditPesx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEScaleY )
	{
		FLOAT fEScaleY = (FLOAT) atof(m_strEScaleY);

		fEScaleY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEScaleY.SetPos(0);

		m_strEScaleY.Format( "%.2f", fEScaleY);
		UpdateData(FALSE);
		OnEnChangeEditPesy();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEScaleZ )
	{
		FLOAT fEScaleZ = (FLOAT) atof(m_strEScaleZ);

		fEScaleZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEScaleZ.SetPos(0);

		m_strEScaleZ.Format( "%.2f", fEScaleZ);
		UpdateData(FALSE);
		OnEnChangeEditPesz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEPosX )
	{
		FLOAT fEPosX = (FLOAT) atof(m_strEPosX);

		fEPosX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosX.SetPos(0);

		m_strEPosX.Format( "%.2f", fEPosX);
		UpdateData(FALSE);
		OnEnChangeEditPepx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEPosY )
	{
		FLOAT fEPosY = (FLOAT) atof(m_strEPosY);

		fEPosY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosY.SetPos(0);

		m_strEPosY.Format( "%.2f", fEPosY);
		UpdateData(FALSE);
		OnEnChangeEditPepy();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEPosZ )
	{
		FLOAT fEPosZ = (FLOAT) atof(m_strEPosZ);

		fEPosZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosZ.SetPos(0);

		m_strEPosZ.Format( "%.2f", fEPosZ);
		UpdateData(FALSE);
		OnEnChangeEditPepz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotX )
	{
		FLOAT fERotX = (FLOAT) atof(m_strERotX);

		fERotX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinERotX.SetPos(0);

		m_strERotX.Format( "%.2f", fERotX);
		UpdateData(FALSE);
		OnEnChangeEditPerx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotY )
	{
		FLOAT fERotY = (FLOAT) atof(m_strERotY);

		fERotY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinERotY.SetPos(0);

		m_strERotY.Format( "%.2f", fERotY);
		UpdateData(FALSE);
		OnEnChangeEditPery();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotZ )
	{
		FLOAT fERotZ = (FLOAT) atof(m_strERotZ);

		fERotZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinERotZ.SetPos(0);

		m_strERotZ.Format( "%.2f", fERotZ);
		UpdateData(FALSE);
		OnEnChangeEditPerz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinOutX )
	{
		FLOAT fOutX = (FLOAT) atof(m_strOutX);

		fOutX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinOutX.SetPos(0);

		m_strOutX.Format( "%.2f", fOutX);
		UpdateData(FALSE);
		OnEnChangeEditMaxx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinOutY )
	{
		FLOAT fOutY = (FLOAT) atof(m_strOutY);

		fOutY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinOutY.SetPos(0);

		m_strOutY.Format( "%.2f", fOutY);
		UpdateData(FALSE);
		OnEnChangeEditMaxy();
	}

	if( pScrollBar == (CScrollBar *) &m_spinOutZ )
	{
		FLOAT fOutZ = (FLOAT) atof(m_strOutZ);

		fOutZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinOutZ.SetPos(0);

		m_strOutZ.Format( "%.2f", fOutZ);
		UpdateData(FALSE);
		OnEnChangeEditMaxz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinInX )
	{
		FLOAT fInX = (FLOAT) atof(m_strInX);

		fInX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinInX.SetPos(0);

		m_strInX.Format( "%.2f", fInX);
		UpdateData(FALSE);
		OnEnChangeEditMinx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinInY )
	{
		FLOAT fInY = (FLOAT) atof(m_strInY);

		fInY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinInY.SetPos(0);

		m_strInY.Format( "%.2f", fInY);
		UpdateData(FALSE);
		OnEnChangeEditMiny();
	}

	if( pScrollBar == (CScrollBar *) &m_spinInZ )
	{
		FLOAT fInZ = (FLOAT) atof(m_strInZ);

		fInZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinInZ.SetPos(0);

		m_strInZ.Format( "%.2f", fInZ);
		UpdateData(FALSE);
		OnEnChangeEditMinz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinFollow )
	{
		FLOAT fFollow = (FLOAT) atof(m_strFollow);

		fFollow += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinFollow.SetPos(0);

		m_strFollow.Format( "%.2f", fFollow);
		UpdateData(FALSE);
		OnEnChangeEditFollow();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CParticleBar::OnCbnSelchangeComboSpraykind()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_bSprayType = m_nSKind;
		InitSFX();

		EnableAllCtrl(TRUE);
	}
}
