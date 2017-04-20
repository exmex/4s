// EXTMAPBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "EXTMAPFrame.h"
#include "EXTMAPView.h"
#include "EXTMAPBar.h"
#include "EXTMAPDoc.h"
#include "SaveMinimapDlg.h"
#include "BuildHeightDlg.h"
#include "BuildEMDlg.h"
#include "BuildShadowDlg.h"
#include "BuildShadeDlg.h"
#include "SaveHeightDlg.h"
#include "EditUNITDlg.h"
#include "LoadRegionDataTexDlg.h"
#include "DetailTextureDlg.h"
#include ".\extmapbar.h"


// CEXTMAPBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEXTMAPBar, CBarContent)
CEXTMAPBar::CEXTMAPBar(CWnd* pParent /*=NULL*/)
:CBarContent(CEXTMAPBar::IDD, pParent)
, m_strHeight(_T(""))
, m_strBrushH(_T(""))
, m_strFlatH(_T(""))
, m_strAddFlatH(_T(""))
, m_nSize(2)
, m_nShape(0)
, m_nAttenuation(0)
, m_bHideMap(FALSE)
, m_bEditTEX(FALSE)
, m_bEditH(FALSE)
, m_bFlatH(FALSE)
, m_bAddFlatH(FALSE)
, m_bDisableUNIT(FALSE)
, m_bRegion(FALSE)
, m_bRegionID(0)
, m_bDetail(FALSE)
, m_bHBrush(FALSE)
, m_bSmooth(FALSE)
, m_bUseSHADER(FALSE)
, m_strSpecRange(_T("0.0"))
, m_strSpecMax(_T("0.0"))
, m_strIntensity(_T("0.0"))
, m_strDestFolder(_T(""))
, m_strDestFile(_T(""))
, m_strDetailTexCount(_T(""))
{
	m_pItem = NULL;
}

CEXTMAPBar::~CEXTMAPBar()
{
}

void CEXTMAPBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GEOTEX, m_cGEOTEX);
	DDX_Control(pDX, IDC_LIST_UNIT, m_cUNIT);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_cHeight);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_strHeight);
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_BUTTON_SHADE, m_cBuildShade);
	DDX_Control(pDX, IDC_BUTTON_GEOMETRY, m_cBuildGeoTex);
	DDX_Control(pDX, IDC_BUTTON_SAVE_GEOTEX, m_cSaveGeoTex);
	DDX_Control(pDX, IDC_BUTTON_LOAD_GEOTEX, m_cLoadGeoTex);
	DDX_Control(pDX, IDC_BUTTON_OL_GEOTEX, m_cOLGeoTex);
	DDX_Control(pDX, IDC_EDIT_FLATH, m_cFlatH);
	DDX_Text(pDX, IDC_EDIT_FLATH, m_strFlatH);
	DDX_Control(pDX, IDC_SPIN_FLATH, m_spinFlatH);
	DDX_Control(pDX, IDC_EDIT_ADDFLATH, m_cAddFlatH);
	DDX_Text(pDX, IDC_EDIT_ADDFLATH, m_strAddFlatH);
	DDX_Control(pDX, IDC_SPIN_ADDFLATH, m_spinAddFlatH);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_cSize);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_nSize);
	DDX_Control(pDX, IDC_SPIN_SIZE, m_spinSize);
	DDX_Control(pDX, IDC_COMBO_SHAPE, m_cShape);
	DDX_CBIndex(pDX, IDC_COMBO_SHAPE, m_nShape);
	DDX_Control(pDX, IDC_COMBO_ATTENUATION, m_cAttenuation);
	DDX_CBIndex(pDX, IDC_COMBO_ATTENUATION, m_nAttenuation);
	DDX_Control(pDX, IDC_CHECK_EDIT_HIDEMAP, m_cHideMap);
	DDX_Check(pDX, IDC_CHECK_EDIT_HIDEMAP, m_bHideMap);
	DDX_Control(pDX, IDC_CHECK_EDIT_TEXTURE, m_cEditTexture);
	DDX_Check(pDX, IDC_CHECK_EDIT_TEXTURE, m_bEditTEX);
	DDX_Control(pDX, IDC_CHECK_EDIT_HEIGHT, m_cEditHeight);
	DDX_Check(pDX, IDC_CHECK_EDIT_HEIGHT, m_bEditH);
	DDX_Control(pDX, IDC_CHECK_FLAT, m_cFlat);
	DDX_Check(pDX, IDC_CHECK_FLAT, m_bFlatH);
	DDX_Control(pDX, IDC_CHECK_ADDFLAT, m_cAddFlat);
	DDX_Check(pDX, IDC_CHECK_ADDFLAT, m_bAddFlatH);
	DDX_Control(pDX, IDC_BUTTON_EDIT_OBJ, m_cEditOBJ);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SFX, m_cEditSFX);
	DDX_Control(pDX, IDC_BUTTON_ENABLE_ALL, m_cEnableAll);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_ALL, m_cDisableAll);
	DDX_Control(pDX, IDC_CHECK_DISABLE_UNIT, m_cDisableUNIT);
	DDX_Check(pDX, IDC_CHECK_DISABLE_UNIT, m_bDisableUNIT);
	DDX_Control(pDX, IDC_BUTTON_BUILD_HEIGHT, m_cBuildHeight);
	DDX_Control(pDX, IDC_CHECK_EDIT_REGION, m_cRegion);
	DDX_Check(pDX, IDC_CHECK_EDIT_REGION, m_bRegion);
	DDX_Control(pDX, IDC_EDIT_REGION, m_cRegionID);
	DDX_Text(pDX, IDC_EDIT_REGION, m_bRegionID);
	DDX_Control(pDX, IDC_SPIN_REGION, m_spinRegionID);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SND, m_cEditSND);
	DDX_Control(pDX, IDC_LIST_DETAILTEX, m_cDETAIL);
	DDX_Control(pDX, IDC_CHECK_EDIT_DETAIL, m_cEditDetail);
	DDX_Check(pDX, IDC_CHECK_EDIT_DETAIL, m_bDetail);
	DDX_Control(pDX, IDC_CHECK_HBRUSH, m_cHBrush);
	DDX_Check(pDX, IDC_CHECK_HBRUSH, m_bHBrush);
	DDX_Control(pDX, IDC_EDIT_HBRUSH, m_cBrushH);
	DDX_Text(pDX, IDC_EDIT_HBRUSH, m_strBrushH);
	DDX_Control(pDX, IDC_SPIN_HBRUSH, m_spinBrushH);
	DDX_Control(pDX, IDC_CHECK_SMOOTH, m_cSmooth);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_bSmooth);
	DDX_Check(pDX, IDC_CHECK_SPECULAR, m_bUseSHADER);
	DDX_Control(pDX, IDC_EDIT_SPECRANGE, m_cSpecRange);
	DDX_Text(pDX, IDC_EDIT_SPECRANGE, m_strSpecRange);
	DDX_Control(pDX, IDC_SPIN_SPECRANGE, m_spinSpecRange);
	DDX_Control(pDX, IDC_EDIT_MAXCOLOR, m_cSpecMax);
	DDX_Text(pDX, IDC_EDIT_MAXCOLOR, m_strSpecMax);
	DDX_Control(pDX, IDC_SPIN_MAXCOLOR, m_spinSpecMax);
	DDX_Control(pDX, IDC_EDIT_INTENSITY, m_cIntensity);
	DDX_Text(pDX, IDC_EDIT_INTENSITY, m_strIntensity);
	DDX_Control(pDX, IDC_SPIN_INTENSITY, m_spinIntensity);
	DDX_Control(pDX, IDC_BUTTON_TILETEX, m_cBuildTileTex);
	DDX_Control(pDX, IDC_BUTTON_SAVE_HEIGHTMAP, m_cSaveHeight);
	DDX_Control(pDX, IDC_BUTTON_SAVE_MINIMAP, m_cSaveMinimap);
	DDX_Control(pDX, IDC_BUTTON_LOAD_REGIONTEX, m_cRegionDataTex);
	DDX_Control(pDX, IDC_BUTTON_EDIT_UNIT, m_cEditEnableUNIT);
	DDX_Control(pDX, IDC_STATIC_DETAILTEX, m_cStaticDetailTex);
	DDX_Text(pDX, IDC_STATIC_DETAILTEX, m_strDetailTexCount);
}


BEGIN_MESSAGE_MAP(CEXTMAPBar, CBarContent)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UNIT, OnLvnItemchangedListUnit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GEOTEX, OnLvnItemchangedListGeotex)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEnChangeEditHeight)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_GEOTEX, OnNMDblclkListGeotex) // KSH. 좌 더블클릭 -> 우 더블클릭으로 수정.
	ON_BN_CLICKED(IDC_BUTTON_SHADE, OnBnClickedButtonShade)
	ON_BN_CLICKED(IDC_BUTTON_GEOMETRY, OnBnClickedButtonGeometry)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_GEOTEX, OnBnClickedButtonSaveGeotex)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_GEOTEX, OnBnClickedButtonLoadGeotex)
	ON_BN_CLICKED(IDC_BUTTON_OL_GEOTEX, OnBnClickedButtonOlGeotex)
	ON_BN_CLICKED(IDC_CHECK_EDIT_HIDEMAP, OnBnClickedCheckHideMap)
	ON_BN_CLICKED(IDC_CHECK_EDIT_HEIGHT, OnBnClickedCheckEditHeight)
	ON_BN_CLICKED(IDC_CHECK_EDIT_TEXTURE, OnBnClickedCheckEditTexture)
	ON_BN_CLICKED(IDC_CHECK_FLAT, OnBnClickedCheckFlat)
	ON_BN_CLICKED(IDC_CHECK_ADDFLAT, OnBnClickedCheckAddFlat)
	ON_CBN_SELCHANGE(IDC_COMBO_ATTENUATION, OnCbnSelchangeComboAttenuation)
	ON_CBN_SELCHANGE(IDC_COMBO_SHAPE, OnCbnSelchangeComboShape)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_OBJ, OnBnClickedButtonEditObj)
	ON_EN_CHANGE(IDC_EDIT_FLATH, OnEnChangeEditFlath)
	ON_EN_CHANGE(IDC_EDIT_ADDFLATH, OnEnChangeEditAddFlath)
	ON_EN_CHANGE(IDC_EDIT_SIZE, OnEnChangeEditSize)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_ALL, OnBnClickedButtonEnableAll)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_ALL, OnBnClickedButtonDisableAll)
	ON_BN_CLICKED(IDC_CHECK_DISABLE_UNIT, OnBnClickedCheckDisableUnit)
	ON_BN_CLICKED(IDC_BUTTON_BUILD_HEIGHT, OnBnClickedButtonBuildHeight)
	ON_BN_CLICKED(IDC_CHECK_EDIT_REGION, OnBnClickedCheckEditRegion)
	ON_EN_CHANGE(IDC_EDIT_REGION, OnEnChangeEditRegion)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SFX, OnBnClickedButtonEditSfx)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SND, OnBnClickedButtonEditSnd)
	ON_BN_CLICKED(IDC_CHECK_EDIT_DETAIL, OnBnClickedCheckEditDetail)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DETAILTEX, OnLvnItemchangedListDetailtex)
	ON_BN_CLICKED(IDC_CHECK_HBRUSH, OnBnClickedCheckHbrush)
	ON_EN_CHANGE(IDC_EDIT_HBRUSH, OnEnChangeEditHbrush)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, OnBnClickedCheckSmooth)
	ON_BN_CLICKED(IDC_CHECK_SPECULAR, OnBnClickedCheckSpecular)
	ON_EN_CHANGE(IDC_EDIT_INTENSITY, OnEnChangeEditIntensity)
	ON_EN_CHANGE(IDC_EDIT_MAXCOLOR, OnEnChangeEditMaxcolor)
	ON_EN_CHANGE(IDC_EDIT_SPECRANGE, OnEnChangeEditSpecrange)
	ON_BN_CLICKED(IDC_BUTTON_TILETEX, OnBnClickedButtonTiletex)
	ON_BN_CLICKED(IDC_BUTTON_SHADOWTEX, OnBnClickedButtonShadowtex)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_HEIGHTMAP, OnBnClickedButtonSaveHeightmap)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MINIMAP, OnBnClickedButtonSaveMinimap)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_REGIONTEX, OnBnClickedButtonLoadRegiontex)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_UNIT, OnBnClickedButtonEditEnableUnit)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DETTEX, OnBnClickedButtonEditDettex)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_Shadow, OnBnClickedButtonSaveShadow)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SHADOW, OnBnClickedButtonLoadShadow)
END_MESSAGE_MAP()


// CEXTMAPBar 메시지 처리기입니다.

BOOL CEXTMAPBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	m_cControlBox.GetClientRect(&rect);
	m_cControlBox.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;

	m_cGEOTEX.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cGEOTEX.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cGEOTEX.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cUNIT.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cUNIT.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cUNIT.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cDETAIL.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cDETAIL.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cDETAIL.InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cGEOTEX.m_DropTarget.Register(&m_cGEOTEX);
	m_cGEOTEX.m_pDragEnter = OnGEODragOver;
	m_cGEOTEX.m_pDragOver = OnGEODragOver;
	m_cGEOTEX.m_pDrop = OnGEODrop;
	m_cGEOTEX.m_pLeave = OnGEODragLeave;

	m_spinIntensity.SetRange( -100, 100);
	m_spinSpecRange.SetRange( -100, 100);
	m_spinSpecMax.SetRange( -100, 100);
	m_spinHeight.SetRange( -100, 100);
	m_spinBrushH.SetRange( -100, 100);
	m_spinFlatH.SetRange( -100, 100);
	m_spinAddFlatH.SetRange( -100, 100);

	m_spinRegionID.SetRange( 0, 255);
	m_spinSize.SetRange( 0, 1000);

	return TRUE;
}

void CEXTMAPBar::SetItem( CEXTMAPItem *pItem)
{
	m_pItem = pItem;
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	int nIndex = m_cUNIT.GetCurSel();
	m_cUNIT.GetListCtrl()->DeleteAllItems();

	for( int i=0; i<m_pItem->m_HugeMAP.m_nUnitCountX; i++)
		for( int j=0; j<m_pItem->m_HugeMAP.m_nUnitCountZ; j++)
		{
			CString strUNIT;

			strUNIT.Format( "%d, %d", m_pItem->m_HugeMAP.m_nUnitLength * i, m_pItem->m_HugeMAP.m_nUnitLength * j);
			if( nIndex == m_cUNIT.GetListCtrl()->InsertItem( m_cUNIT.GetListCtrl()->GetItemCount(), strUNIT) )
				m_cUNIT.SetCurSel(nIndex);
		}
	m_cUNIT.UpdateColumnWidth();
	ResetDETAIL();

	CListCtrl *pListCtrl = m_cGEOTEX.GetListCtrl();
	int nCount = INT(m_pItem->m_vGEO.size());

	nIndex = m_cGEOTEX.GetCurSel();
	pListCtrl->DeleteAllItems();

	for( int i=0; i<nCount; i++)
	{
		CTextureItem *pTEX = CItemBase::GetTextureItem(m_pItem->m_vGEO[i]->m_dwTEXID);

		if( pTEX && nIndex == pListCtrl->InsertItem( i, pTEX->m_strName) )
			m_cGEOTEX.SetCurSel(nIndex);
	}

	m_strIntensity.Format( "%.2f", m_pItem->m_HugeMAP.m_fIntensity);
	m_strSpecRange.Format( "%.2f", m_pItem->m_HugeMAP.m_fSpecRange);
	m_strSpecMax.Format( "%.2f", m_pItem->m_HugeMAP.m_fSpecMax);
	m_bUseSHADER = m_pItem->m_HugeMAP.m_bUseSHADER;

	m_strBrushH.Format( "%.2f", pDoc->m_fBRUSH);
	m_strFlatH.Format( "%.2f", pDoc->m_fFLAT);
	m_strAddFlatH.Format( "%.2f", pDoc->m_fADDFLAT);

	MAPCOLOR::iterator finder = m_pItem->m_mapCOLOR.find(m_bRegionID);
	DWORD dwCOLOR = RGB( 0xFF, 0xFF, 0xFF);

	if( finder != m_pItem->m_mapCOLOR.end() )
		dwCOLOR = (*finder).second;

	m_cColorCtrl.SetupColor( dwCOLOR, TRUE);
	m_cColorBox.SetColor(dwCOLOR);
	OnSelectUNIT(GetUNIT());

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::EnableAllCtrl( BOOL bEnable)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = GetUNIT();

	m_spinHeight.EnableWindow(bEnable && m_cGEOTEX.GetCurSel() != -1);
	m_cHeight.EnableWindow(bEnable && m_cGEOTEX.GetCurSel() != -1);
	m_cDETAIL.EnableWindow(bEnable);

	m_spinBrushH.EnableWindow(bEnable && m_bHBrush);
	m_cBrushH.EnableWindow(bEnable && m_bHBrush);

	m_spinFlatH.EnableWindow(bEnable && m_bFlatH);
	m_cFlatH.EnableWindow(bEnable && m_bFlatH);

	m_spinAddFlatH.EnableWindow(bEnable && m_bAddFlatH);
	m_cAddFlatH.EnableWindow(bEnable && m_bAddFlatH);
	
	m_cDisableUNIT.EnableWindow(bEnable && nIndex != -1);
	m_cSaveGeoTex.EnableWindow(bEnable && nIndex != -1);
	m_cLoadGeoTex.EnableWindow(bEnable && nIndex != -1);
	m_cOLGeoTex.EnableWindow(bEnable && nIndex != -1);
	m_cRegionDataTex.EnableWindow(bEnable && nIndex != -1);

	m_spinIntensity.EnableWindow(bEnable && m_bUseSHADER);
	m_spinSpecRange.EnableWindow(bEnable && m_bUseSHADER);
	m_spinSpecMax.EnableWindow(bEnable && m_bUseSHADER);

	m_cIntensity.EnableWindow(bEnable && m_bUseSHADER);
	m_cSpecRange.EnableWindow(bEnable && m_bUseSHADER);
	m_cSpecMax.EnableWindow(bEnable && m_bUseSHADER);

	m_spinRegionID.EnableWindow(bEnable && m_bRegion);
	m_cColorCtrl.EnableWindow(bEnable && m_bRegion);
	m_cRegionID.EnableWindow(bEnable && m_bRegion);

	m_cAttenuation.EnableWindow(bEnable && !m_bRegion && !m_bSmooth);
	m_cShape.EnableWindow(bEnable);

	m_cBuildHeight.EnableWindow(bEnable);
	m_cEditTexture.EnableWindow(bEnable);
	m_cBuildGeoTex.EnableWindow(bEnable);
	m_cEditHeight.EnableWindow(bEnable);
	m_cBuildShade.EnableWindow(bEnable);
	m_cEditDetail.EnableWindow(bEnable);
	m_cDisableAll.EnableWindow(bEnable);
	m_cEditEnableUNIT.EnableWindow(bEnable);		
	m_cEnableAll.EnableWindow(bEnable);
	m_cEditOBJ.EnableWindow(bEnable);
	m_cEditSFX.EnableWindow(bEnable);
	m_cEditSND.EnableWindow(bEnable);
	m_spinSize.EnableWindow(bEnable);
	m_cRegion.EnableWindow(bEnable);
	m_cSmooth.EnableWindow(bEnable);
	m_cHBrush.EnableWindow(bEnable);
	m_cSize.EnableWindow(bEnable);
	
	m_cGEOTEX.EnableWindow(bEnable);
	m_cUNIT.EnableWindow(bEnable);

	UpdateData(FALSE);
}

void CEXTMAPBar::ResetDETAIL()
{
	if(!m_pItem)
		return;
	int nCount = INT(m_pItem->m_vDETAIL.size());
	
	m_cDETAIL.DeleteAllItems();
	for( int i=0; i<nCount; i++)
	{
		if(m_pItem->m_vDETAIL[i])
		{
			CTextureItem *pDETAIL = CItemBase::GetTextureItem(m_pItem->m_vDETAIL[i]);
			CString strNAME;

			if(!pDETAIL)
				strNAME.Format( _T("Unknown (ID : 0x%08X)"), m_pItem->m_vDETAIL[i]);
			else
				strNAME = pDETAIL->m_strName;

			int nIndex = m_cDETAIL.InsertItem(
				m_cDETAIL.GetItemCount(),
				strNAME);

			m_cDETAIL.SetItemData(
				nIndex,
				m_pItem->m_vDETAIL[i]);
		}
	}

	m_strDetailTexCount.Format( "Detail Texture(%d)", nCount );
	UpdateData( FALSE );
}

void CEXTMAPBar::OnSelectUNIT( int nIndex)
{
	if( nIndex != -1 )
		m_bDisableUNIT = !m_pItem->m_HugeMAP.m_pENABLE[nIndex];

	EnableAllCtrl(TRUE);
}

DWORD CEXTMAPBar::__BuildSHADOWTEX( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;
	pParam->m_pHost->BuildSHADOWTEX(pParam);

	return 0;
}

DWORD CEXTMAPBar::__BuildTILETEX( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;
	pParam->m_pHost->BuildTILETEX(pParam);

	return 0;
}

DWORD CEXTMAPBar::__BuildHEIGHT( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;
	pParam->m_pHost->BuildHEIGHT(pParam);

	return 0;
}

DWORD CEXTMAPBar::__BuildSHADE( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;
	pParam->m_pHost->BuildSHADE(pParam);

	return 0;
}

DWORD CEXTMAPBar::__BuildGEO( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;
	pParam->m_pHost->BuildGEO(pParam);

	return 0;
}

void CEXTMAPBar::BuildSHADOWTEX( LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SHADOWTEX);
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	MAPDWORD::iterator finder = pDoc->m_mapUNIT.find(GetUNIT());

	D3DXVECTOR3 vLIGHT = m_pItem->m_HugeMAP.m_vLIGHT;
	FLOAT fLength = D3DXVec3Length(&vLIGHT);

	if( fLength > 0.001f )
		vLIGHT /= fLength;

	if( fLength > 0.001f && finder != pDoc->m_mapUNIT.end() &&
		fabs(vLIGHT.y) > 0.001f && (
		fabs(vLIGHT.x) > 0.001f ||
		fabs(vLIGHT.z) > 0.001f ))
	{
		CTachyonEXTMAP *pUNIT = &pDoc->m_UNIT[(*finder).second];
		MAPOBJECT::iterator it;
		VECTOROBJECT vTOBJ;
		vTOBJ.clear();

		while(!pUNIT->m_vSHADOWTEX.empty())
		{
			pUNIT->m_vSHADOWTEX.back()->Release();
			pUNIT->m_vSHADOWTEX.pop_back();
		}

		for( it = pDoc->m_mapOBJ.begin(); it != pDoc->m_mapOBJ.end(); it++)
			if(!pDoc->IsHideOBJ((*it).first))
			{
				MAPMAPOBJ::iterator itOBJ = m_pItem->m_pMAP->m_mapOBJ.find((*it).first);
				MAPDWORD::iterator itID = m_pItem->m_mapOBJ.find((*it).first);

				if(m_pItem->HasTSHADOW( CItemBase::GetOBJItem((*itID).second), (*itOBJ).second))
					vTOBJ.push_back((*it).second);
			}

		m_pItem->BuildSHADOWTEX(
			&pUNIT->m_vSHADOWTEX,
			&vTOBJ,
			&vLIGHT,
			pParam->m_nType,
			(*finder).first);
		vTOBJ.clear();

		pDoc->m_pItem->UpdateUNIT(
			NULL,
			NULL,
			NULL,
			&pUNIT->m_vSHADOWTEX,
			NULL,
			&pUNIT->m_vDETAIL,
			NULL,
			NULL,
			(*finder).first);
	}

	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPBar::BuildTILETEX( LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_TILETEX);
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	if(pParam->m_nType)
	{
		for( DWORD i=0; i<m_pItem->m_HugeMAP.m_nUnitCountZ; i++)
			for( DWORD j=0; j<m_pItem->m_HugeMAP.m_nUnitCountX; j++)
			{
				m_pItem->BuildTILETEX( j, i);

				pParam->m_pWnd->m_bPercent = (BYTE) ((i * m_pItem->m_HugeMAP.m_nUnitCountX + j) * 100 / (m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ));
				pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}
	}
	else
	{
		int nIndex = GetUNIT();

		if( nIndex != -1 )
		{
			m_pItem->BuildTILETEX(
				nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
				nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

			pParam->m_pWnd->m_bPercent = (BYTE) 100;
			pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
			pParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
	}

	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPBar::BuildHEIGHT( LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_GEO);
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	if(pParam->m_nType)
	{
		FLOAT *pHEIGHT = new FLOAT[(m_pItem->m_HugeMAP.m_nUnitLength + 1) * (m_pItem->m_HugeMAP.m_nUnitLength + 1)];

		LPDIRECT3DSURFACE9 pDUMY;
		LPDIRECT3DSURFACE9 pIMG;
		D3DXIMAGE_INFO vINFO;

		D3DXGetImageInfoFromFile(
			pParam->m_strFileName,
			&vINFO);

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			m_pItem->m_HugeMAP.m_nUnitLength + 1,
			m_pItem->m_HugeMAP.m_nUnitLength + 1,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			&pIMG,
			NULL);

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			3 * m_pItem->m_HugeMAP.m_nUnitLength,
			3 * m_pItem->m_HugeMAP.m_nUnitLength,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			&pDUMY,
			NULL);

		FLOAT fRatioY = ((FLOAT) vINFO.Height) / ((FLOAT) (m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitCountZ + 1));
		FLOAT fRatioX = ((FLOAT) vINFO.Width) / ((FLOAT) (m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitCountX + 1));

		for( DWORD i=0; i<m_pItem->m_HugeMAP.m_nUnitCountZ; i++)
			for( DWORD j=0; j<m_pItem->m_HugeMAP.m_nUnitCountX; j++)
			{
				DWORD dwIndex = i * m_pItem->m_HugeMAP.m_nUnitCountX + j;
				CRect rcDEST(
					0, 0,
					3 * m_pItem->m_HugeMAP.m_nUnitLength,
					3 * m_pItem->m_HugeMAP.m_nUnitLength);

				CRect rcSRC(
					-INT(m_pItem->m_HugeMAP.m_nUnitLength),
					-INT(m_pItem->m_HugeMAP.m_nUnitLength),
					2 * m_pItem->m_HugeMAP.m_nUnitLength,
					2 * m_pItem->m_HugeMAP.m_nUnitLength);

				rcSRC.OffsetRect(
					j * m_pItem->m_HugeMAP.m_nUnitLength,
					(m_pItem->m_HugeMAP.m_nUnitCountZ - i - 1) * m_pItem->m_HugeMAP.m_nUnitLength);

				if( rcSRC.left < 0 )
				{
					rcDEST.left += m_pItem->m_HugeMAP.m_nUnitLength;
					rcSRC.left += m_pItem->m_HugeMAP.m_nUnitLength;
				}

				if( rcSRC.top < 0 )
				{
					rcDEST.top += m_pItem->m_HugeMAP.m_nUnitLength;
					rcSRC.top += m_pItem->m_HugeMAP.m_nUnitLength;
				}

				if( rcSRC.right > m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitCountX + 1 )
				{
					rcDEST.right -= m_pItem->m_HugeMAP.m_nUnitLength;
					rcSRC.right -= m_pItem->m_HugeMAP.m_nUnitLength;
				}

				if( rcSRC.bottom > m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitCountZ + 1 )
				{
					rcDEST.bottom -= m_pItem->m_HugeMAP.m_nUnitLength;
					rcSRC.bottom -= m_pItem->m_HugeMAP.m_nUnitLength;
				}

				rcSRC.left = (INT) (((FLOAT) rcSRC.left) * fRatioX);
				rcSRC.right = (INT) (((FLOAT) rcSRC.right) * fRatioX);

				rcSRC.top = (INT) (((FLOAT) rcSRC.top) * fRatioY);
				rcSRC.bottom = (INT) (((FLOAT) rcSRC.bottom) * fRatioY);

				D3DXLoadSurfaceFromFile(
					pDUMY,
					NULL,
					&rcDEST,
					pParam->m_strFileName,
					&rcSRC,
					D3DX_DEFAULT,
					0, NULL);

				rcSRC.SetRect(
					m_pItem->m_HugeMAP.m_nUnitLength,
					m_pItem->m_HugeMAP.m_nUnitLength,
					2 * m_pItem->m_HugeMAP.m_nUnitLength + 1,
					2 * m_pItem->m_HugeMAP.m_nUnitLength + 1);

				D3DXLoadSurfaceFromSurface(
					pIMG,
					NULL,
					NULL,
					pDUMY,
					NULL,
					&rcSRC,
					D3DX_FILTER_NONE, 0);

				m_pItem->BuildHEIGHT( pIMG, dwIndex, pHEIGHT);
				m_pItem->UpdateUNIT( NULL, NULL, NULL, NULL, NULL, NULL, NULL, pHEIGHT, dwIndex);
				m_pItem->RegulateHEIGHT( j, i);

				pParam->m_pWnd->m_bPercent = (BYTE) (dwIndex * 100 / (m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ));
				pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}

		pDUMY->Release();
		pDUMY = NULL;

		pIMG->Release();
		pIMG = NULL;
		delete[] pHEIGHT;
	}
	else
	{
		int nIndex = GetUNIT();

		if( nIndex != -1 )
		{
			FLOAT *pHEIGHT = new FLOAT[(m_pItem->m_HugeMAP.m_nUnitLength + 1) * (m_pItem->m_HugeMAP.m_nUnitLength + 1)];
			LPDIRECT3DSURFACE9 pIMG;

			CPoint point(
				nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
				nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

			CRect rect(
				0, 0,
				m_pItem->m_HugeMAP.m_nUnitCountX,
				m_pItem->m_HugeMAP.m_nUnitCountZ);

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				m_pItem->m_HugeMAP.m_nUnitLength + 1,
				m_pItem->m_HugeMAP.m_nUnitLength + 1,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pIMG,
				NULL);

			D3DXLoadSurfaceFromFile(
				pIMG,
				NULL,
				NULL,
				pParam->m_strFileName,
				NULL,
				D3DX_DEFAULT,
				0, NULL);

			m_pItem->BuildHEIGHT( pIMG, nIndex, pHEIGHT);
			m_pItem->UpdateUNIT( NULL, NULL, NULL, NULL, NULL, NULL, NULL, pHEIGHT, nIndex);

			m_pItem->RegulateHEIGHT(
				point.x,
				point.y);

			if(rect.PtInRect(CPoint( point.x + 1, point.y)))
			{
				m_pItem->RegulateHEIGHT(
					point.x + 1,
					point.y);
			}

			if(rect.PtInRect(CPoint( point.x, point.y + 1)))
			{
				m_pItem->RegulateHEIGHT(
					point.x,
					point.y + 1);
			}

			if(rect.PtInRect(CPoint( point.x + 1, point.y + 1)))
			{
				m_pItem->RegulateHEIGHT(
					point.x + 1,
					point.y + 1);
			}

			pParam->m_pWnd->m_bPercent = (BYTE) 100;
			pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
			pParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);

			pIMG->Release();
			pIMG = NULL;
			delete[] pHEIGHT;
		}
	}

	BuildSHADE(pParam);
}

void CEXTMAPBar::BuildSHADE( LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SHADE);
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	if(pParam->m_nType)
	{
		for( DWORD i=0; i<m_pItem->m_HugeMAP.m_nUnitCountZ; i++)
			for( DWORD j=0; j<m_pItem->m_HugeMAP.m_nUnitCountX; j++)
			{
				m_pItem->BuildSHADE( j, i);
				m_pItem->RegulateSHADE( j, i);

				pParam->m_pWnd->m_bPercent = (BYTE) ((i * m_pItem->m_HugeMAP.m_nUnitCountX + j) * 100 / (m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ));
				pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}
	}
	else
	{
		int nIndex = GetUNIT();

		if( nIndex != -1 )
		{
			CPoint point(
				nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
				nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

			CRect rect(
				0, 0,
				m_pItem->m_HugeMAP.m_nUnitCountX,
				m_pItem->m_HugeMAP.m_nUnitCountZ);

			m_pItem->BuildSHADE(
				point.x,
				point.y);

			m_pItem->RegulateSHADE(
				point.x,
				point.y);

			if(rect.PtInRect(CPoint( point.x + 1, point.y)))
			{
				m_pItem->RegulateSHADE(
					point.x + 1,
					point.y);
			}

			if(rect.PtInRect(CPoint( point.x, point.y + 1)))
			{
				m_pItem->RegulateSHADE(
					point.x,
					point.y + 1);
			}

			if(rect.PtInRect(CPoint( point.x + 1, point.y + 1)))
			{
				m_pItem->RegulateSHADE(
					point.x + 1,
					point.y + 1);
			}

			pParam->m_pWnd->m_bPercent = (BYTE) 100;
			pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
			pParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
	}

	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPBar::BuildGEO( LPEMBPARAM pParam)
{
	MAPLAYER::iterator it;
	MAPLAYER mapLAYER;

	pParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_GEOTEX);
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	mapLAYER.clear();
	m_pItem->BuildLAYER(&mapLAYER);

	if(pParam->m_nType)
	{
		for( DWORD i=0; i<m_pItem->m_HugeMAP.m_nUnitCountZ; i++)
			for( DWORD j=0; j<m_pItem->m_HugeMAP.m_nUnitCountX; j++)
			{
				m_pItem->BuildSKIN( &mapLAYER, j, i);
				m_pItem->RegulateSKIN( j, i);

				pParam->m_pWnd->m_bPercent = (BYTE) ((i * m_pItem->m_HugeMAP.m_nUnitCountX + j) * 100 / (m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ));
				pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}
	}
	else
	{
		int nIndex = GetUNIT();

		if( nIndex != -1 )
		{
			CPoint point(
				nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
				nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

			CRect rect(
				0, 0,
				m_pItem->m_HugeMAP.m_nUnitCountX,
				m_pItem->m_HugeMAP.m_nUnitCountZ);

			m_pItem->BuildSKIN(
				&mapLAYER,
				point.x,
				point.y);

			m_pItem->RegulateSKIN(
				point.x,
				point.y);

			if(rect.PtInRect(CPoint( point.x + 1, point.y)))
			{
				m_pItem->RegulateSKIN(
					point.x + 1,
					point.y);
			}

			if(rect.PtInRect(CPoint( point.x, point.y + 1)))
			{
				m_pItem->RegulateSKIN(
					point.x,
					point.y + 1);
			}

			if(rect.PtInRect(CPoint( point.x + 1, point.y + 1)))
			{
				m_pItem->RegulateSKIN(
					point.x + 1,
					point.y + 1);
			}

			pParam->m_pWnd->m_bPercent = (BYTE) 100;
			pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
			pParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}
	}

	for( it = mapLAYER.begin(); it != mapLAYER.end(); it++)
		delete (*it).second;
	mapLAYER.clear();

	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CALLBACK CEXTMAPBar::OnGEODragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

BOOL CEXTMAPBar::CanGEODrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_TEXTURE && ((CTextureItem *) pItem)->GetTexData();
}

DROPEFFECT CALLBACK CEXTMAPBar::OnGEODragOver( CGSListCtrl *pList,
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

			if(CanGEODrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pList->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

BOOL CALLBACK CEXTMAPBar::OnGEODrop( CGSListCtrl *pList,
									 COleDataObject* pDataObject,
									 DROPEFFECT dropEffect,
									 CPoint point)
{
	CEXTMAPBar *pBar = (CEXTMAPBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanGEODrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->AddGEO((CTextureItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnGEODragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

void CEXTMAPBar::AddGEO( CTextureItem *pItem)
{
	CListCtrl *pListCtrl = m_cGEOTEX.GetListCtrl();
	LPGEOTEX pGEOTEX = new GEOTEX();

	pListCtrl->InsertItem( pListCtrl->GetItemCount(), pItem->m_strName);
	pGEOTEX->m_dwTEXID = pItem->m_dwID;
	m_pItem->m_vGEO.push_back(pGEOTEX);

	UpdateData(FALSE);
}

void CEXTMAPBar::OnLvnItemchangedListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cUNIT.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		FLOAT fPosX = (FLOAT) (pNMLV->iItem / m_pItem->m_HugeMAP.m_nUnitCountZ);
		FLOAT fPosZ = (FLOAT) (pNMLV->iItem % m_pItem->m_HugeMAP.m_nUnitCountZ);
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		fPosX *= m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength;
		fPosZ *= m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength;

		fPosX += ((FLOAT) (m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength)) / 2.0f;
		fPosZ += ((FLOAT) (m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nTileLength)) / 2.0f;

		OnSelectUNIT(GetUNIT());
		pDoc->JumpView( fPosX, fPosZ);
	}

	*pResult = 0;
}

void CEXTMAPBar::OnLvnItemchangedListDetailtex( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		CGameStudioDoc::m_pPreview->SetItem(CItemBase::GetTextureItem(DWORD(pNMLV->lParam)));

	*pResult = 0;
}

void CEXTMAPBar::OnNMClickListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	pDoc->RedrawAllView();
	*pResult = 0;
}

int CEXTMAPBar::GetUNIT()
{
	int nIndex = m_cUNIT.GetCurSel();

	if( nIndex != -1 )
	{
		int nPosX = nIndex / m_pItem->m_HugeMAP.m_nUnitCountZ;
		int nPosZ = nIndex % m_pItem->m_HugeMAP.m_nUnitCountZ;

		return nPosZ * m_pItem->m_HugeMAP.m_nUnitCountX + nPosX;
	}

	return -1;
}

void CEXTMAPBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinHeight )
	{
		FLOAT fHeight = (FLOAT) atof(m_strHeight);

		fHeight += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinHeight.SetPos(0);

		m_strHeight.Format( "%.2f", fHeight);
		UpdateData(FALSE);
		OnEnChangeEditHeight();
	}

	if( pScrollBar == (CScrollBar *) &m_spinFlatH )
	{
		FLOAT fFlatH = (FLOAT) atof(m_strFlatH);

		fFlatH += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinFlatH.SetPos(0);

		m_strFlatH.Format( "%.2f", fFlatH);
		UpdateData(FALSE);
		OnEnChangeEditFlath();
	}

	if( pScrollBar == (CScrollBar *) &m_spinAddFlatH )
	{
		FLOAT fddFlatH = (FLOAT) atof(m_strAddFlatH);

		fddFlatH += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinAddFlatH.SetPos(0);

		m_strAddFlatH.Format( "%.2f", fddFlatH);
		UpdateData(FALSE);
		OnEnChangeEditAddFlath();
	}

	if( pScrollBar == (CScrollBar *) &m_spinBrushH )
	{
		FLOAT fBrushH = (FLOAT) atof(m_strBrushH);

		fBrushH += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinBrushH.SetPos(0);

		m_strBrushH.Format( "%.2f", fBrushH);
		UpdateData(FALSE);
		OnEnChangeEditHbrush();
	}

	if( pScrollBar == (CScrollBar *) &m_spinIntensity )
	{
		FLOAT fIntensity = (FLOAT) atof(m_strIntensity);

		fIntensity += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinIntensity.SetPos(0);

		m_strIntensity.Format( "%.2f", fIntensity);
		UpdateData(FALSE);
		OnEnChangeEditIntensity();
	}

	if( pScrollBar == (CScrollBar *) &m_spinSpecMax )
	{
		FLOAT fSpecMax = (FLOAT) atof(m_strSpecMax);

		fSpecMax += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinSpecMax.SetPos(0);

		m_strSpecMax.Format( "%.2f", fSpecMax);
		UpdateData(FALSE);
		OnEnChangeEditMaxcolor();
	}

	if( pScrollBar == (CScrollBar *) &m_spinSpecRange )
	{
		FLOAT fSpecRange = (FLOAT) atof(m_strSpecRange);

		fSpecRange += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinSpecRange.SetPos(0);
		
		m_strSpecRange.Format( "%.2f", fSpecRange);
		UpdateData(FALSE);
		OnEnChangeEditSpecrange();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CEXTMAPBar::OnLvnItemchangedListGeotex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		m_strHeight.Format( "%.2f", m_pItem->m_vGEO[pNMLV->iItem]->m_fHeight);
		UpdateData(FALSE);
	}

	EnableAllCtrl(TRUE);
	*pResult = 0;
}

void CEXTMAPBar::OnNMDblclkListGeotex(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cGEOTEX.GetCurSel();

	if( nIndex != -1 )
	{
		delete m_pItem->m_vGEO[nIndex];
		m_pItem->m_vGEO.erase(m_pItem->m_vGEO.begin() + nIndex);

		m_cGEOTEX.GetListCtrl()->DeleteItem(nIndex);
	}

	*pResult = 0;
}

void CEXTMAPBar::OnEnChangeEditHeight()
{
	UpdateData();
	int nIndex = m_cGEOTEX.GetCurSel();

	if( nIndex != -1 )
		m_pItem->m_vGEO[nIndex]->m_fHeight = (FLOAT) atof(m_strHeight);
}

void CEXTMAPBar::OnBnClickedButtonShade()
{
	CBuildShadeDlg dlg;

	dlg.m_strLightX.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.x);
	dlg.m_strLightY.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.y);
	dlg.m_strLightZ.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.z);

	if( dlg.DoModal() == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		CProgressDlg Progress;
		EMBPARAM Param;

		Param.m_pWnd = &Progress;
		Param.m_pHost = this;
		Param.m_nType = dlg.m_nType;
		pDoc->UpdateData();

		m_pItem->m_HugeMAP.m_vLIGHT.x = (FLOAT) atof(dlg.m_strLightX);
		m_pItem->m_HugeMAP.m_vLIGHT.y = (FLOAT) atof(dlg.m_strLightY);
		m_pItem->m_HugeMAP.m_vLIGHT.z = (FLOAT) atof(dlg.m_strLightZ);

		Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_SHADE);
		Progress.m_strTitle.LoadString(IDS_BUILD_SHADEDATA);
		Progress.m_pThreadFunc = (AFX_THREADPROC) __BuildSHADE;
		Progress.m_lParam = &Param;
		Progress.DoModal();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CEXTMAPBar::OnBnClickedButtonGeometry()
{
	CBuildEMDlg dlg;

	dlg.m_strTitle.LoadString(IDS_BUILD_GEOTEX);
	if( dlg.DoModal() == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		CProgressDlg Progress;
		EMBPARAM Param;

		Param.m_pWnd = &Progress;
		Param.m_pHost = this;
		Param.m_nType = dlg.m_nType;
		pDoc->UpdateData();

		Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_GEO);
		Progress.m_strTitle.LoadString(IDS_BUILD_GEOTEX);
		Progress.m_pThreadFunc = (AFX_THREADPROC) __BuildGEO;
		Progress.m_lParam = &Param;
		Progress.DoModal();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CEXTMAPBar::OnBnClickedButtonSaveGeotex()
{
	CFileDialog dlg(
		FALSE,
		_T("*.bmp; *.dds"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.bmp, *.dds)|*.bmp; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CString strName = dlg.GetPathName();
		CString strEXT = dlg.GetFileExt();

		LPDIRECT3DSURFACE9 pSKIN = NULL;
		BOOL bDDS = FALSE;

		strEXT.MakeUpper();
		if( strEXT == _T("DDS") )
			bDDS = TRUE;
		else if( strEXT != _T("BMP") )
			strName = strName + _T(".bmp");

		m_pItem->LoadUNIT(
			NULL,
			&pSKIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			GetUNIT());

		D3DXSaveSurfaceToFile(
			strName,
			bDDS ? D3DXIFF_DDS : D3DXIFF_BMP,
			pSKIN,
			NULL,
			NULL);
		pSKIN->Release();
	}
}

void CEXTMAPBar::OnBnClickedButtonLoadGeotex()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tga; *.bmp; *.png; *.jpg; *.dds"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.tga, *.bmp, *.png, *.jpg, *.dds)|*.tga; *.bmp; *.png; *.jpg; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		LPDIRECT3DSURFACE9 pSKIN = NULL;
		int nIndex = GetUNIT();
		pDoc->UpdateData();

		CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
			m_pItem->m_HugeMAP.m_nTextureSize,
			m_pItem->m_HugeMAP.m_nTextureSize,
			D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH,
			&pSKIN,
			NULL);

		D3DXLoadSurfaceFromFile(
			pSKIN,
			NULL,
			NULL,
			dlg.GetPathName(),
			NULL,
			D3DX_FILTER_POINT,
			NULL,
			NULL);

		CPoint point(
			nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
			nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

		CRect rect(
			0, 0,
			m_pItem->m_HugeMAP.m_nUnitCountX,
			m_pItem->m_HugeMAP.m_nUnitCountZ);

		m_pItem->UpdateUNIT(
			NULL,
			pSKIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			nIndex);

		m_pItem->RegulateSKIN(
			point.x,
			point.y);

		if(rect.PtInRect(CPoint( point.x + 1, point.y)))
		{
			m_pItem->RegulateSKIN(
				point.x + 1,
				point.y);
		}

		if(rect.PtInRect(CPoint( point.x, point.y + 1)))
		{
			m_pItem->RegulateSKIN(
				point.x,
				point.y + 1);
		}

		if(rect.PtInRect(CPoint( point.x + 1, point.y + 1)))
		{
			m_pItem->RegulateSKIN(
				point.x + 1,
				point.y + 1);
		}
		pSKIN->Release();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CEXTMAPBar::OnBnClickedButtonOlGeotex()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tga; *.bmp; *.png; *.jpg; *.dds"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.tga, *.bmp, *.png, *.jpg, *.dds)|*.tga; *.bmp; *.png; *.jpg; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		D3DXMATRIX vWORLD(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.0f, 0.5f, 1.0f);

		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		int nIndex = GetUNIT();

		LPDIRECT3DSURFACE9 pTARGET = NULL;
		LPDIRECT3DSURFACE9 pBACK = NULL;
		LPDIRECT3DSURFACE9 pZBUF = NULL;
		LPDIRECT3DSURFACE9 pSKIN = NULL;
		LPDIRECT3DTEXTURE9 pTEX = NULL;

		D3DSURFACE_DESC vDESC;
		CD3DCamera vCAMERA;

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
		pDoc->UpdateData();

		D3DXCreateTextureFromFileEx(
			CMainFrame::m_3DDevice.m_pDevice,
			dlg.GetPathName(),
			m_pItem->m_HugeMAP.m_nTextureSize,
			m_pItem->m_HugeMAP.m_nTextureSize,
			1, 0,
			D3DFMT_DXT3,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0, NULL,
			NULL,
			&pTEX);

		m_pItem->LoadUNIT(
			NULL,
			&pSKIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			nIndex);
		pSKIN->GetDesc(&vDESC);

		CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
			vDESC.Width,
			vDESC.Height,
			D3DFMT_A8R8G8B8,
			D3DMULTISAMPLE_NONE,
			0, FALSE,
			&pTARGET,
			NULL);

		D3DXLoadSurfaceFromSurface(
			pTARGET,
			NULL,
			NULL,
			pSKIN,
			NULL,
			NULL,
			D3DX_FILTER_NONE,
			NULL);

		D3DVIEWPORT9 vNEW = {
			0, 0,
			vDESC.Width,
			vDESC.Height,
			0.0f,
			1.0f};
		D3DVIEWPORT9 vOLD;

		CMainFrame::m_3DDevice.m_pDevice->GetDepthStencilSurface(&pZBUF);
		CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
		CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTARGET);
		CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(NULL);
		CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);
		CMainFrame::m_3DDevice.m_pDevice->BeginScene();

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

		CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, CEXTMAPView::m_pVB, 0, sizeof(PVERTEX));
		CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

		CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
		CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pTEX);

		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		CMainFrame::m_3DDevice.m_pDevice->EndScene();

		CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
		CMainFrame::m_3DDevice.m_pDevice->SetDepthStencilSurface(pZBUF);
		CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

		m_pItem->GetRenderTargetData(
			pTARGET,
			pSKIN);

		CPoint point(
			nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
			nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

		CRect rect(
			0, 0,
			m_pItem->m_HugeMAP.m_nUnitCountX,
			m_pItem->m_HugeMAP.m_nUnitCountZ);

		m_pItem->UpdateUNIT(
			NULL,
			pSKIN,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			nIndex);

		m_pItem->RegulateSKIN(
			point.x,
			point.y);

		if(rect.PtInRect(CPoint( point.x + 1, point.y)))
		{
			m_pItem->RegulateSKIN(
				point.x + 1,
				point.y);
		}

		if(rect.PtInRect(CPoint( point.x, point.y + 1)))
		{
			m_pItem->RegulateSKIN(
				point.x,
				point.y + 1);
		}

		if(rect.PtInRect(CPoint( point.x + 1, point.y + 1)))
		{
			m_pItem->RegulateSKIN(
				point.x + 1,
				point.y + 1);
		}

		pTARGET->Release();
		pBACK->Release();
		pZBUF->Release();
		pSKIN->Release();
		pTEX->Release();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CEXTMAPBar::OnBnClickedCheckEditHeight()
{
	UpdateData();
	OnEditHeight();
}
void CEXTMAPBar::OnEditHeight()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;

	if(m_bEditH)
	{
		m_bEditTEX = FALSE;
		m_bSmooth = FALSE;
		m_bHBrush = FALSE;
		m_bFlatH = FALSE;
		m_bRegion = FALSE;
		m_bDetail = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedCheckHbrush()
{
	UpdateData();
	OnHeightBrush();
}

void CEXTMAPBar::OnHeightBrush()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;
	
	if(m_bHBrush)
	{
		m_bEditTEX = FALSE;
		m_bEditH = FALSE;
		m_bFlatH = FALSE;
		m_bRegion = FALSE;
		m_bDetail = FALSE;
		m_bSmooth = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedCheckSmooth()
{
	UpdateData();
	OnMakeSmooth();	
}
void CEXTMAPBar::OnMakeSmooth()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;

	if(m_bSmooth)
	{
		m_bEditTEX = FALSE;
		m_bEditH = FALSE;
		m_bFlatH = FALSE;
		m_bRegion = FALSE;
		m_bDetail = FALSE;
		m_bHBrush = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}


void CEXTMAPBar::OnBnClickedCheckHideMap()
{
	UpdateData();
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	UpdateData(FALSE);

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);
}


void CEXTMAPBar::OnBnClickedCheckEditTexture()
{
	UpdateData();
	OnEditTexture();
}

void CEXTMAPBar::OnEditTexture()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;

	if(m_bEditTEX)
	{
		m_bRegion = FALSE;
		m_bDetail = FALSE;
		m_bHBrush = FALSE;
		m_bSmooth = FALSE;
		m_bEditH = FALSE;
		m_bFlatH = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedCheckFlat()
{
	UpdateData();
	OnMakeFlat();
}

void CEXTMAPBar::OnMakeFlat()
{	
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;
	
	if(m_bFlatH)
	{
		m_bEditTEX = FALSE;
		m_bDetail = FALSE;
		m_bRegion = FALSE;
		m_bHBrush = FALSE;
		m_bSmooth = FALSE;
		m_bEditH = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}	

void CEXTMAPBar::OnBnClickedCheckAddFlat()
{
	UpdateData();
	OnMakeAddFlat();
}

void CEXTMAPBar::OnMakeAddFlat()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;

	if(m_bAddFlatH)
	{
		m_bEditTEX = FALSE;
		m_bDetail = FALSE;
		m_bRegion = FALSE;
		m_bHBrush = FALSE;
		m_bSmooth = FALSE;
		m_bEditH = FALSE;
		m_bFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedCheckEditDetail()
{
	UpdateData();
	OnEditDetail();
}
void CEXTMAPBar::OnEditDetail()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;
	
	if(m_bDetail)
	{
		m_bEditTEX = FALSE;
		m_bRegion = FALSE;
		m_bHBrush = FALSE;
		m_bSmooth = FALSE;
		m_bFlatH = FALSE;
		m_bEditH = FALSE;
		m_bAddFlatH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedCheckEditRegion()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	BOOL bRegion = m_bRegion;

	UpdateData();
	if(m_bRegion)
	{
		m_bEditTEX = FALSE;
		m_bDetail = FALSE;
		m_bHBrush = FALSE;
		m_bSmooth = FALSE;
		m_bFlatH = FALSE;
		m_bEditH = FALSE;
	}
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnCbnSelchangeComboAttenuation()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnCbnSelchangeComboShape()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);
}

void CEXTMAPBar::OnBnClickedButtonEditObj()
{
	OnEditObject();
}

void CEXTMAPBar::OnEditObject()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bEditTEX = FALSE;
	m_bDetail = FALSE;
	m_bRegion = FALSE;
	m_bHBrush = FALSE;
	m_bSmooth = FALSE;
	m_bEditH = FALSE;
	m_bFlatH = FALSE;
	m_bAddFlatH = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditSFX = FALSE;
	pDoc->m_bEditSND = FALSE;
	pDoc->m_bEditOBJ = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);
	
	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPOBJBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPOBJBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CEXTMAPBar::OnBnClickedButtonEditSfx()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bEditTEX = FALSE;
	m_bDetail = FALSE;
	m_bRegion = FALSE;
	m_bHBrush = FALSE;
	m_bSmooth = FALSE;
	m_bEditH = FALSE;
	m_bFlatH = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditSND = FALSE;
	pDoc->m_bEditOBJ = FALSE;
	pDoc->m_bEditSFX = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPSFXBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPSFXBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CEXTMAPBar::OnBnClickedButtonEditSnd()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	CEXTMAPFrame *pFrame = (CEXTMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bEditTEX = FALSE;
	m_bDetail = FALSE;
	m_bRegion = FALSE;
	m_bHBrush = FALSE;
	m_bSmooth = FALSE;
	m_bEditH = FALSE;
	m_bFlatH = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
	{
		pDoc->UpdateData();
		pDoc->Reload();
	}

	pDoc->RefreshView();
	EnableAllCtrl(TRUE);

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditOBJ = FALSE;
	pDoc->m_bEditSFX = FALSE;
	pDoc->m_bEditSND = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPSNDBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPSNDBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CEXTMAPBar::OnEnChangeEditHbrush()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	UpdateData();
	pDoc->m_fBRUSH = (FLOAT) atof(m_strBrushH);
}

void CEXTMAPBar::OnEnChangeEditFlath()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	UpdateData();
	pDoc->m_fFLAT = (FLOAT) atof(m_strFlatH);
}

void CEXTMAPBar::OnEnChangeEditAddFlath()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	UpdateData();
	pDoc->m_fADDFLAT = (FLOAT) atof(m_strAddFlatH);
}

void CEXTMAPBar::OnEnChangeEditSize()
{
	if(m_pItem)
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		pDoc->RedrawAllView();
		EnableAllCtrl(TRUE);
	}
}

void CEXTMAPBar::OnBnClickedButtonEnableAll()
{
	DWORD dwCount = m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ;

	for( DWORD i=0; i<dwCount; i++)
		m_pItem->m_HugeMAP.m_pENABLE[i] = TRUE;

	m_bDisableUNIT = FALSE;
	EnableAllCtrl(TRUE);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	if(pDoc)
		pDoc->RefreshView();
}

void CEXTMAPBar::OnBnClickedButtonDisableAll()
{
	DWORD dwCount = m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ;

	for( DWORD i=0; i<dwCount; i++)
		m_pItem->m_HugeMAP.m_pENABLE[i] = FALSE;

	m_bDisableUNIT = TRUE;
	EnableAllCtrl(TRUE);
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	if(pDoc)
		pDoc->RefreshView();
}

void CEXTMAPBar::OnBnClickedCheckDisableUnit()
{
	int nIndex = GetUNIT();
	UpdateData();

	m_pItem->m_HugeMAP.m_pENABLE[nIndex] = !m_bDisableUNIT;
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	if(pDoc)
		pDoc->RefreshView();
}

void CEXTMAPBar::OnBnClickedButtonBuildHeight()
{
	CBuildEMDlg dlg;

	dlg.m_strTitle.LoadString(IDS_BUILD_GEO);
	if( dlg.DoModal() == IDOK )
	{
		CBuildHeightDlg dlgBH;

		dlgBH.m_strLightX.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.x);
		dlgBH.m_strLightY.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.y);
		dlgBH.m_strLightZ.Format( "%.2f", m_pItem->m_HugeMAP.m_vLIGHT.z);
		dlgBH.m_strMinH.Format( "%.2f", m_pItem->m_fMinHeight);
		dlgBH.m_strMaxH.Format( "%.2f", m_pItem->m_fMaxHeight);

		if( dlgBH.DoModal() == IDOK )
		{
			CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
			CProgressDlg Progress;
			EMBPARAM Param;

			m_pItem->m_HugeMAP.m_vLIGHT.x = (FLOAT) atof(dlgBH.m_strLightX);
			m_pItem->m_HugeMAP.m_vLIGHT.y = (FLOAT) atof(dlgBH.m_strLightY);
			m_pItem->m_HugeMAP.m_vLIGHT.z = (FLOAT) atof(dlgBH.m_strLightZ);
			m_pItem->m_fMinHeight = (FLOAT) atof(dlgBH.m_strMinH);
			m_pItem->m_fMaxHeight = (FLOAT) atof(dlgBH.m_strMaxH);

			Param.m_strFileName = dlgBH.m_strFile;
			Param.m_nType = dlg.m_nType;
			Param.m_pWnd = &Progress;
			Param.m_pHost = this;
			pDoc->UpdateData();

			Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_EXTMAP);
			Progress.m_strTitle.LoadString(IDS_BUILD_EXTMAP);
			Progress.m_pThreadFunc = (AFX_THREADPROC) __BuildHEIGHT;
			Progress.m_lParam = &Param;
			Progress.DoModal();

			pDoc->Reload();
			pDoc->RefreshView();
		}
	}
}

LRESULT CEXTMAPBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

	if(m_pItem)
	{
		MAPCOLOR::iterator finder = m_pItem->m_mapCOLOR.find(m_bRegionID);
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		if( finder == m_pItem->m_mapCOLOR.end() )
			m_pItem->m_mapCOLOR.insert( MAPCOLOR::value_type( m_bRegionID, ((CTColorCtrl::selection_colors *) lParam)->m_clrNew));
		else
			(*finder).second = ((CTColorCtrl::selection_colors *) lParam)->m_clrNew;

		pDoc->UpdateData();
		pDoc->Reload();
		pDoc->RefreshView();
	}

	return 0;
}

void CEXTMAPBar::OnEnChangeEditRegion()
{
	if(m_pItem)
	{
		UpdateData();
		MAPCOLOR::iterator finder = m_pItem->m_mapCOLOR.find(m_bRegionID);

		if( finder == m_pItem->m_mapCOLOR.end() )
			m_cColorCtrl.SetupColor( m_cColorBox.m_Color, TRUE);
		else
			m_cColorCtrl.SetupColor( (*finder).second, TRUE);
	}
}

void CEXTMAPBar::OnBnClickedCheckSpecular()
{
	UpdateData();
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

	m_pItem->m_HugeMAP.m_bUseSHADER = m_bUseSHADER;
	EnableAllCtrl(TRUE);

	pDoc->RedrawAllView();
}

void CEXTMAPBar::OnEnChangeEditIntensity()
{
	UpdateData();

	if(m_pItem)
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		m_pItem->m_HugeMAP.m_fIntensity = (FLOAT) atof(m_strIntensity);
		pDoc->RedrawAllView();
	}
}

void CEXTMAPBar::OnEnChangeEditMaxcolor()
{
	UpdateData();

	if(m_pItem)
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		m_pItem->m_HugeMAP.m_fSpecMax = (FLOAT) atof(m_strSpecMax);
		pDoc->RedrawAllView();
	}
}

void CEXTMAPBar::OnEnChangeEditSpecrange()
{
	UpdateData();

	if(m_pItem)
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		m_pItem->m_HugeMAP.m_fSpecRange = (FLOAT) atof(m_strSpecRange);
		pDoc->RedrawAllView();
	}
}

void CEXTMAPBar::OnBnClickedButtonTiletex()
{
	CBuildEMDlg dlg;

	dlg.m_strTitle.LoadString(IDS_BUILD_TILETEX);
	if( dlg.DoModal() == IDOK )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		CProgressDlg Progress;
		EMBPARAM Param;

		CMainFrame::m_bRender = FALSE;
		Param.m_strFileName.Empty();
		Param.m_nType = dlg.m_nType;
		Param.m_pWnd = &Progress;
		Param.m_pHost = this;
		pDoc->UpdateData();

		Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_TILETEX);
		Progress.m_strTitle.LoadString(IDS_BUILD_TILETEX);
		Progress.m_pThreadFunc = (AFX_THREADPROC) __BuildTILETEX;
		Progress.m_lParam = &Param;
		Progress.DoModal();
		CMainFrame::m_bRender = TRUE;

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CEXTMAPBar::OnBnClickedButtonShadowtex()
{
	int nUNIT = GetUNIT();

	// nUNIT만 Enable되고 나머지는 Disable되어있어야만 BuildSHADOWTEX가 제대로 작동된다. 그 검사를 여기서.
	INT nTotalUnitCount = m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ;

	for( INT i=0 ; i < nTotalUnitCount ; ++i )
		if( i != nUNIT && m_pItem->m_HugeMAP.m_pENABLE[i] )
		{
			// nUNIT외에 Enable된 맵이 있다.
			CString strMessage;
			CString strTitle;

			strMessage = _T("해당 맵만 Enable 시키고 반드시 반드시 반드시 맵 창을 닫았다 열어주십시오.");
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
			return ;
		}

	if( nUNIT != -1 && m_pItem->m_HugeMAP.m_pENABLE[nUNIT] )
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		MAPDWORD::iterator finder = pDoc->m_mapUNIT.find(nUNIT);

		if( finder != pDoc->m_mapUNIT.end() )
		{
			CBuildShadowDlg dlg;

			dlg.m_bShadowPixel = (BYTE) m_pItem->m_HugeMAP.m_nShadowPixel;
			if( dlg.DoModal() == IDOK )
			{
				CProgressDlg Progress;
				EMBPARAM Param;

				CMainFrame::m_bRender = FALSE;
				Param.m_strFileName.Empty();
				Param.m_nType = dlg.m_bShadowPixel;
				Param.m_pWnd = &Progress;
				Param.m_pHost = this;
				pDoc->UpdateData();

				Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_SHADOWTEX);
				Progress.m_strTitle.LoadString(IDS_BUILD_SHADOWTEX);
				Progress.m_pThreadFunc = (AFX_THREADPROC) __BuildSHADOWTEX;
				Progress.m_lParam = &Param;
				Progress.DoModal();
				CMainFrame::m_bRender = TRUE;

				pDoc->Reload();
				pDoc->RefreshView();
			}
		}
	}
}

void CEXTMAPBar::OnBnClickedButtonSaveHeightmap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlgFile(
		FALSE,
		_T("*.bmp"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image File (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	CTShell		dlgFolder;
	CSaveHeightDlg dlg;

	if( dlg.DoModal() == IDOK )
	{
		CEXTMAPDoc *pDoc = ( CEXTMAPDoc * )m_pItem->GetMainDocument();
		CProgressDlg Progress;
		EMBPARAM Param;

		Param.m_pWnd	= &Progress;
		Param.m_pHost	= this;
		Param.m_nType	= dlg.m_nType;
		pDoc->UpdateData();

		m_pItem->m_fMinHeight = ( FLOAT )atof( dlg.m_strMinH );
		m_pItem->m_fMaxHeight = ( FLOAT )atof( dlg.m_strMaxH );
		

		if( Param.m_nType )	// All Unit 
		{
			dlgFolder.m_strTitle = _T("Image Folder");
			if( dlgFolder.BrowseForFolder(this) == IDOK )
			{
				m_strDestFolder = dlgFolder.m_strPath;
				
				Param.m_strFileName = m_strDestFolder;

				Progress.m_strCancel.LoadString( IDS_CANCEL_SAVE_HEIGHTMAP );
				Progress.m_strTitle.LoadString( IDS_SAVE_HEIGHTMAPDATA );
				Progress.m_pThreadFunc = ( AFX_THREADPROC ) __SaveHeight;
				Progress.m_lParam = &Param;
				Progress.DoModal();
			}	
		}
		else				// Current Unit
		{
			dlgFile.m_ofn.lpstrInitialDir = LPCSTR(m_strDestFile);
			if( dlgFile.DoModal() == IDOK )
			{
				UpdateData();
				m_strDestFolder = dlgFile.GetPathName();
				UpdateData(FALSE);

				Param.m_strFileName = m_strDestFolder;

				Progress.m_strCancel.LoadString( IDS_CANCEL_SAVE_HEIGHTMAP );
				Progress.m_strTitle.LoadString( IDS_SAVE_HEIGHTMAPDATA );
				Progress.m_pThreadFunc = ( AFX_THREADPROC ) __SaveHeight;
				Progress.m_lParam = &Param;
				Progress.DoModal();

			}
		}

	}
}		

// Height Map을 이미지(TGA) 화일로 저장
DWORD CEXTMAPBar::__SaveHeight(DWORD_PTR lParam)
{
	LPEMBPARAM pParam = ( LPEMBPARAM )lParam;
	pParam->m_pHost->SaveHeight( pParam );

	return 0;
}

// Map을 높이 데이터를 Height Image(TGA)로 저장
void CEXTMAPBar::SaveHeight(LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_strStatus.LoadString( IDS_SAVE_HEIGHTMAP );
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	
	if( pParam->m_nType )	// All Unit
	{
		CString strFolder = pParam->m_strFileName;
		for( DWORD i = 0 ; i < m_pItem->m_HugeMAP.m_nUnitCountZ ; i++ )
		{
			for( DWORD j = 0 ; j < m_pItem->m_HugeMAP.m_nUnitCountX ; j++ )
			{	
				pParam->m_strFileName = strFolder;
				TCHAR strFileName[10] = "";
				sprintf( strFileName, "X%dY%d.bmp", j, i );
				pParam->m_strFileName += _T("\\MapItem_");
				pParam->m_strFileName.Insert( pParam->m_strFileName.GetLength(), strFileName );
                
				m_pItem->SaveHeight( j, i, pParam->m_strFileName );

				pParam->m_pWnd->m_bPercent = ( BYTE )( ( i * m_pItem->m_HugeMAP.m_nUnitCountX + j ) * 100 / ( m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ ) );
				pParam->m_pWnd->m_cProgressBar.SetPos( pParam->m_pWnd->m_bPercent );
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}
		}
	}
	else					// Current Unit
	{
		int nIndex = GetUNIT();
		CPoint point(
			nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
			nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

		CRect rect(
			0, 0,
			m_pItem->m_HugeMAP.m_nUnitCountX,
			m_pItem->m_HugeMAP.m_nUnitCountZ );

		m_pItem->SaveHeight( point.x, point.y, pParam->m_strFileName );

		pParam->m_pWnd->m_bPercent = (BYTE) 100;
		pParam->m_pWnd->m_cProgressBar.SetPos(pParam->m_pWnd->m_bPercent);
		pParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPBar::OnBnClickedButtonSaveMinimap()
{
	CSaveMinimapDlg dlg;
	CFileDialog dlgFile(
		FALSE,
		_T("*.bmp"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image File (*.bmp)|*.bmp|All Files (*.*)|*.*||"));
	if( dlgFile.DoModal() == IDOK )
	{
		if( dlg.DoModal() == IDOK )
		{
			CProgressDlg Progress;
			EMBPARAM Param;

			CMainFrame::m_bRender = FALSE;
			Param.m_strFileName = dlgFile.GetPathName();
			Param.m_nType = dlg.m_nImageSize;
			Param.m_bOBJCOLLISION = dlg.m_bOBJCollision;
			Param.m_pWnd = &Progress;
			Param.m_pHost = this;

			Progress.m_strCancel.LoadString(IDS_CANCEL_SAVE_MINIMAP);
			Progress.m_strTitle.LoadString(IDS_SAVE_MINIMAP);
			Progress.m_pThreadFunc = (AFX_THREADPROC) __SaveMinimap;
			Progress.m_lParam = &Param;
			Progress.DoModal();
			CMainFrame::m_bRender = TRUE;
		}
	}
}

DWORD CEXTMAPBar::__SaveMinimap( DWORD_PTR lParam)
{
	LPEMBPARAM pParam = (LPEMBPARAM) lParam;

	pParam->m_pHost->SaveMinimap(pParam);
	return 0;
}

void CEXTMAPBar::SaveMinimap( LPEMBPARAM pParam)
{
	pParam->m_pWnd->m_cProgressBar.SetPos(0);
	pParam->m_pWnd->m_bPercent = 0;
	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = GetUNIT();

	MAPDWORD::iterator finder = pDoc->m_mapUNIT.find(nIndex);
	CTachyonEXTMAP *pUNIT = NULL;

	CPoint point(
	nIndex % m_pItem->m_HugeMAP.m_nUnitCountX,
	nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

	int nTileLength = m_pItem->m_HugeMAP.m_nTileLength;
	int nUnitLength = m_pItem->m_HugeMAP.m_nUnitLength;
	int nUnitLeft 	= point.x * nUnitLength * nTileLength;
	int nUnitTop  	= point.y * nUnitLength * nTileLength;

	if( finder != pDoc->m_mapUNIT.end() )
		pUNIT = &pDoc->m_UNIT[(*finder).second];

	if(pUNIT)
	{
		static D3DXMATRIX vWORLD(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		LPDIRECT3DTEXTURE9 pTGEOTEX = NULL;

		LPDIRECT3DSURFACE9 pTARGET = NULL;
		LPDIRECT3DSURFACE9 pBACK = NULL;

		D3DVIEWPORT9 vNEW = {
			0, 0,
			pParam->m_nType,
			pParam->m_nType,
			0.0f,
			1.0f};
		D3DVIEWPORT9 vOLD;

		D3DSURFACE_DESC vDESC;
		CD3DCamera vCAM;

		FLOAT fPosX = FLOAT(nIndex % m_pItem->m_HugeMAP.m_nUnitCountX);
		FLOAT fPosZ = FLOAT(nIndex / m_pItem->m_HugeMAP.m_nUnitCountX);

		fPosX *= FLOAT(nUnitLength);
		fPosZ *= FLOAT(nUnitLength);

		fPosX += FLOAT(nUnitLength) / 2.0f;
		fPosZ += FLOAT(nUnitLength) / 2.0f;

		fPosX *= FLOAT(nTileLength);
		fPosZ *= FLOAT(nTileLength);

		CMainFrame::m_3DDevice.m_pDevice->CreateRenderTarget(
			pParam->m_nType,
			pParam->m_nType,
			D3DFMT_A8R8G8B8,
			D3DMULTISAMPLE_NONE,
			0, FALSE,
			&pTARGET,
			NULL);

		pUNIT->m_pGEOTEX->GetDesc(&vDESC);
		D3DXCreateTexture(
			CMainFrame::m_3DDevice.m_pDevice,
			vDESC.Width,
			vDESC.Height,
			1, 0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&pTGEOTEX);

		pTGEOTEX->GetSurfaceLevel( 0, &pBACK);
		D3DXLoadSurfaceFromSurface(
			pBACK,
			NULL,
			NULL,
			pUNIT->m_pGEOTEX,
			NULL,
			NULL,
			D3DX_FILTER_NONE,
			NULL);
		pBACK->Release();

		vCAM.InitOrthoCamera(
			CMainFrame::m_3DDevice.m_pDevice,
			-10.0f,
			m_pItem->m_fMaxHeight + 500.0f,
			FLOAT(nUnitLength * nTileLength),
			FLOAT(nUnitLength * nTileLength));
		
		vCAM.SetPosition(
			D3DXVECTOR3( fPosX, m_pItem->m_fMaxHeight + 200.0f, fPosZ),
			D3DXVECTOR3( fPosX, m_pItem->m_fMinHeight - 200.0f, fPosZ),
			D3DXVECTOR3( 0.0f, 0.0f, 1.0f), FALSE);
		vCAM.Activate(TRUE);

		CMainFrame::m_3DDevice.m_pDevice->GetViewport(&vOLD);
		CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vNEW);

		CMainFrame::m_3DDevice.m_pDevice->GetRenderTarget( 0, &pBACK);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pTARGET);

		CMainFrame::m_3DDevice.m_pDevice->Clear(
			0, NULL,
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB( 255, 0, 0, 0),
			1.0f, 0);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
		CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

		CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
		CMainFrame::m_3DDevice.m_pDevice->SetTexture( 1, pTGEOTEX);

		FLOAT fPercent = 100.0f / nUnitLength;
		FLOAT fResult = 0.0f;
	
		for(int i = 0 ; i < nUnitLength ; i++)
		{
			for(int j = 0 ; j < nUnitLength ; j++)
			{
				PVERTEX vMESH[4] = {{
					FLOAT(nUnitLeft + j * nTileLength),																						// V0 - PosX
					m_pItem->m_HugeMAP.GetHeight(FLOAT(nUnitLeft + j * nTileLength), 1.0f, FLOAT(nUnitTop + i * nTileLength)),				// V0 - PosY
					FLOAT(nUnitTop + i * nTileLength),																						// V0 - PosZ
					FLOAT(j) / FLOAT(nUnitLength),																							// V0 - U
					FLOAT(i) / FLOAT(nUnitLength)},																							// V0 - V

					{ FLOAT(nUnitLeft + j * nTileLength),																					// V2 - PosX
					m_pItem->m_HugeMAP.GetHeight(FLOAT(nUnitLeft + j * nTileLength), 1.0f, FLOAT(nUnitTop + (i + 1) * nTileLength)),		// V2 - PosY
					FLOAT(nUnitTop + (i + 1) * nTileLength),																				// V2 - PosZ
					FLOAT(j) / FLOAT(nUnitLength),																							// V2 - U
					FLOAT(i+1) / FLOAT(nUnitLength)},																						// V2 - V

					{ FLOAT(nUnitLeft + (j + 1) * nTileLength),																				// V1 - PosX
					m_pItem->m_HugeMAP.GetHeight(FLOAT(nUnitLeft + (j + 1) * nTileLength), 1.0f, FLOAT(nUnitTop + i * nTileLength)),		// V1 - PosY
					FLOAT(nUnitTop + i * nTileLength),																						// V1 - PosZ
					FLOAT(j+1) / FLOAT(nUnitLength),																						// V1 - U
					FLOAT(i) / FLOAT(nUnitLength)},																							// V1 - V

					{ FLOAT(nUnitLeft + (j + 1) * nTileLength),																				// V3 - PosX
					m_pItem->m_HugeMAP.GetHeight(FLOAT(nUnitLeft + (j + 1) * nTileLength), 1.0f, FLOAT(nUnitTop + (i + 1) * nTileLength)),	// V3 - PosY
					FLOAT(nUnitTop + (i + 1) * nTileLength),																				// V3 - PosZ
					FLOAT(j+1) / FLOAT(nUnitLength),																						// V3 - U
					FLOAT(i+1) / FLOAT(nUnitLength)}																						// V3 - V
				};

				WORD wDETAIL = pUNIT->FindDetailID(i * nUnitLength + j);
				CTextureItem *pDETAIL = NULL;

				wDETAIL = wDETAIL == TILE_NULL ? TILE_NULL : wDETAIL & TILE_MASK;
				if( wDETAIL != TILE_NULL )
				{
					D3DXMATRIX vSCALE;
					D3DXMatrixIdentity(&vSCALE);

					CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE1, &vSCALE);
					vSCALE._11 = m_pItem->m_vDETAILS[wDETAIL];
					vSCALE._22 = m_pItem->m_vDETAILS[wDETAIL];

					CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &vSCALE);
					pDETAIL = CItemBase::GetTextureItem(m_pItem->m_vDETAIL[wDETAIL]);

					if(pDETAIL)
						pDETAIL->GetTEXTURESET()->m_dwCurTick = 0;
				}

				if(pDETAIL)
				{
					CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
					CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pDETAIL->GetTEXTURESET()->GetTexture());
				}
				else
					CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, &vMESH, sizeof(PVERTEX));
			}

			if( fPercent >= 100.0f )
				fPercent = 100.0f;
			fResult += fPercent;

			pParam->m_pWnd->m_bPercent = ( BYTE )fResult;
			pParam->m_pWnd->m_cProgressBar.SetPos( pParam->m_pWnd->m_bPercent );
			pParam->m_pWnd->SendMessage(
				WM_COMMAND,
				ID_PROGRESS_UPDATE);
		}

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		MAPOBJECT::iterator	itOBJ;

		fPercent = 100.0f / FLOAT(pDoc->m_mapOBJ.size());
		fResult = 0.0f;

		pParam->m_pWnd->m_bPercent = (0);
		pParam->m_pWnd->m_cProgressBar.SetPos( pParam->m_pWnd->m_bPercent );
		pParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);

		for( itOBJ = pDoc->m_mapOBJ.begin() ; itOBJ != pDoc->m_mapOBJ.end() ; itOBJ++ )
			if(!pDoc->IsHideOBJ((*itOBJ).first))
			{
				CTachyonObject *pOBJ = (*itOBJ).second;
				if(!pParam->m_bOBJCOLLISION)
				{
					pOBJ->CalcFrame(FALSE);
					pOBJ->Render(&CMainFrame::m_3DDevice, &vCAM);
				}
				else
				{
					MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find((*itOBJ).first); 

					COBJItem* pOBJITEM = NULL;
					if(itID != pDoc->m_pMAP->m_mapOBJ.end())
						pOBJITEM = pDoc->m_pItem->GetOBJItem((*itID).second);

					if(pOBJITEM)
					{
						LPVECLPOINT pVECPOINT = pDoc->GetCollisionPos(pOBJ, pOBJITEM);

						if(pVECPOINT != NULL)
						{
							LPLPOINT lpPOINT = new LPOINT[pVECPOINT->size()];

							for(int k=0; k<pVECPOINT->size(); k++)
								lpPOINT[k] = (*pVECPOINT)[k];
							
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

							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
							CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE);


							CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_LPOINT);

							CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
								D3DPT_TRIANGLEFAN, UINT(pVECPOINT->size() - 1),
								lpPOINT,
								sizeof(LPOINT));
						}
					}
				}

				if( fPercent >= 100.0f )
					fPercent = 100.0f;
				fResult += fPercent;
				pParam->m_pWnd->m_bPercent = ( BYTE )fResult;
				pParam->m_pWnd->m_cProgressBar.SetPos( pParam->m_pWnd->m_bPercent );
				pParam->m_pWnd->SendMessage(
					WM_COMMAND,
					ID_PROGRESS_UPDATE);
			}
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
		CMainFrame::m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
		CMainFrame::m_3DDevice.m_pDevice->SetViewport(&vOLD);

		D3DXSaveSurfaceToFile(
			pParam->m_strFileName,
			D3DXIFF_BMP,
			pTARGET,
			NULL,
			NULL);

		pTGEOTEX->Release();
		pTARGET->Release();
		pBACK->Release();

		pParam->m_pWnd->m_bPercent = 100;
		pParam->m_pWnd->m_cProgressBar.SetPos( pParam->m_pWnd->m_bPercent );
		pParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	pParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CEXTMAPBar::OnBnClickedButtonLoadRegiontex()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	CLoadRegionDataTexDlg dlg;

	dlg.m_strTitle.LoadString(IDS_LOAD_REGION_DATA_TEX);
	if( dlg.DoModal() == IDOK )
	{
		int nIndex = GetUNIT();

		if( nIndex != -1 )
		{
			LPBYTE pREGION = new BYTE[m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength];

			LPDIRECT3DSURFACE9 pIMG = NULL;
			D3DXIMAGE_INFO vINFO;
			D3DLOCKED_RECT lr;

			if(pDoc)
				pDoc->UpdateData();

			m_pItem->LoadUNIT(
				NULL,
				NULL,
				NULL,
				NULL,
				pREGION,
				NULL,
				NULL,
				NULL,
				nIndex);

			D3DXGetImageInfoFromFile(
				dlg.m_strFileName,
				&vINFO);

			CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
				vINFO.Width,
				vINFO.Height,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SCRATCH,
				&pIMG, NULL);

			D3DXLoadSurfaceFromFile(
				pIMG,
				NULL,
				NULL,
				dlg.m_strFileName,
				NULL,
				D3DX_FILTER_NONE,
				0, NULL);
			pIMG->LockRect( &lr, NULL, 0);

			LPBYTE pDATA = (LPBYTE) lr.pBits;
			LPDWORD pCLR = (LPDWORD) pDATA;

			MAPDWORD mapREGIONID;
			BYTE bRegionID = 1;
			int nINDEX = 0;

			m_pItem->m_mapCOLOR.clear();
			mapREGIONID.clear();

			while( nINDEX < vINFO.Width )
			{
				if( mapREGIONID.find(pCLR[nINDEX]) == mapREGIONID.end() )
				{
					m_pItem->m_mapCOLOR.insert( MAPDWORD::value_type( bRegionID, D3DCOLOR_ARGB(
						0xFF,
						GetRValue(pCLR[nINDEX]),
						GetGValue(pCLR[nINDEX]),
						GetBValue(pCLR[nINDEX]))));
					mapREGIONID.insert( MAPDWORD::value_type( pCLR[nINDEX], bRegionID));
				}

				nINDEX += REGID_IMG_SIZE;
				bRegionID++;
			}

			for( int i=0; i<m_pItem->m_HugeMAP.m_nUnitLength; i++)
			{
				pCLR = (LPDWORD) (pDATA + lr.Pitch * (vINFO.Height - i * (vINFO.Height - REGID_IMG_SIZE) / m_pItem->m_HugeMAP.m_nUnitLength - 1));

				for( int j=0; j<m_pItem->m_HugeMAP.m_nUnitLength; j++)
				{
					nINDEX = i * m_pItem->m_HugeMAP.m_nUnitLength + j;

					if( pREGION[nINDEX] != BLANK_REGION )
					{
						MAPDWORD::iterator finder = mapREGIONID.find(pCLR[j * vINFO.Width / m_pItem->m_HugeMAP.m_nUnitLength]);
						pREGION[nINDEX] = finder == mapREGIONID.end() ? 0x00 : (BYTE) (*finder).second;
					}
				}
			}

			mapREGIONID.clear();
			pIMG->UnlockRect();

			m_pItem->UpdateUNIT(
				NULL,
				NULL,
				NULL,
				NULL,
				pREGION,
				NULL,
				NULL,
				NULL,
				nIndex);

			if(pDoc)
			{
				MAPDWORD::iterator finder = pDoc->m_mapUNIT.find(nIndex);

				if( finder != pDoc->m_mapUNIT.end() )
				{
					CTachyonEXTMAP *pUNIT = &pDoc->m_UNIT[(*finder).second];

					if(pUNIT->m_pREGION)
						delete[] pUNIT->m_pREGION;

					DWORD dwSIZE = m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength;
					pUNIT->m_pREGION = new BYTE[dwSIZE];

					memcpy( pUNIT->m_pREGION, pREGION, dwSIZE);
				}
			}

			if(pIMG)
			{
				pIMG->Release();
				pIMG = NULL;
			}

			if(pREGION)
			{
				delete[] pREGION;
				pREGION = NULL;
			}
		}
	}

	if( m_bRegion && pDoc )
		pDoc->RefreshView();
}

void CEXTMAPBar::OnBnClickedButtonEditEnableUnit()
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
	CEditUNITDlg dlg;

	dlg.InitData(m_pItem);
	dlg.DoModal();

	if(pDoc)
		pDoc->RefreshView();
}

void CEXTMAPBar::OnBnClickedButtonEditDettex()
{
	CDetailTextureDlg::StartEdit( NULL, m_pItem);
}

void CEXTMAPBar::OnBnClickedButtonSaveShadow()
{
	CFileDialog dlg(
		FALSE,
		_T("*.bmp"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CString strName = dlg.GetPathName();
		CString strFolderPath = strName.Mid( 0, strName.ReverseFind( '\\' ) );
		CString strFile = dlg.GetFileTitle();
		CString strEXT = dlg.GetFileExt();

		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
		MAPDWORD::iterator finder = pDoc->m_mapUNIT.find( GetUNIT() );
		if( finder != pDoc->m_mapUNIT.end() )
		{
			CTachyonEXTMAP *pUNIT = &pDoc->m_UNIT[(*finder).second];
			size_t size = pUNIT->m_vSHADOWTEX.size();
			for( size_t i = 0; i < size; ++i )
			{
				CString strTexFile;
				strTexFile.Format( "%s_%02d.%s", strFile, i, strEXT );
				strTexFile = strFolderPath + '\\' + strTexFile;

				D3DXSaveSurfaceToFile(
					strTexFile,
					D3DXIFF_BMP,
					pUNIT->m_vSHADOWTEX[i],
					NULL,
					NULL );				
			}
		}

		MessageBox( _T("Complete!"), _T("Save Shadow Texture"), MB_OK );
	}
}

void CEXTMAPBar::OnBnClickedButtonLoadShadow()
{
	CBuildShadowDlg dlg;

	dlg.m_bShadowPixel = (BYTE) m_pItem->m_HugeMAP.m_nShadowPixel;
	if( dlg.DoModal() == IDOK )
	{
		m_pItem->m_HugeMAP.m_nShadowPixel = dlg.m_bShadowPixel;

		CTShell dlg;

		dlg.m_strTitle = _T("Shadow Texture Folder");
		if( dlg.BrowseForFolder(AfxGetMainWnd()) == IDOK )
		{
			CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();
			MAPDWORD::iterator funit = pDoc->m_mapUNIT.find( GetUNIT() );
			if( funit == pDoc->m_mapUNIT.end() )
				return ;

			VECTORSURFACE vSHADOWTEX;
			vSHADOWTEX.clear();

			CFileFind finder;
			CString strFolder = dlg.m_strPath;
			strFolder.TrimRight('\\');
			CString strFind = strFolder + "\\*.*";

			BOOL bFound = finder.FindFile( strFind );
			while( bFound )
			{
				bFound = finder.FindNextFile();

				if( finder.IsDots() )
					continue;

				CString strExt;
				CTShell shell;

				CString strFileName = finder.GetFilePath();
				strExt = shell.GetFileExt( strFileName );
				strExt.MakeUpper();
				strExt.TrimRight();
				strExt.TrimLeft();

				if( strExt == _T(".BMP") )
				{
					LPDIRECT3DSURFACE9 pSurface;

					CMainFrame::m_3DDevice.m_pDevice->CreateOffscreenPlainSurface(
						SHADOWTEX_SIZE,
						SHADOWTEX_SIZE,
						D3DFMT_A8R8G8B8,
						D3DPOOL_SCRATCH,
						&pSurface,
						NULL);

					D3DXLoadSurfaceFromFile(
						pSurface,
						NULL,
						NULL,
						strFileName,
						NULL,
						D3DX_FILTER_NONE,
						NULL,
						NULL);

					vSHADOWTEX.push_back( pSurface );
				}
			}

			if( vSHADOWTEX.size() > 0 )
			{	// Shadow Texture 불러오기에 성공했을때.
				CTachyonEXTMAP *pUNIT = &pDoc->m_UNIT[(*funit).second];
				while(!pUNIT->m_vSHADOWTEX.empty())
				{
					pUNIT->m_vSHADOWTEX.back()->Release();
					pUNIT->m_vSHADOWTEX.pop_back();
				}

				pUNIT->m_vSHADOWTEX = vSHADOWTEX;

				pDoc->UpdateData();
				pDoc->Reload();
				pDoc->RefreshView();

				MessageBox( _T("Complete!"), _T("Load Shadow Texture"), MB_OK );
			}
			else
			{	// Shadow Texture 불러오기에 실패했을때.
				MessageBox( _T("Can not found texture files!"), _T("Load Shadow Texture"), MB_OK|MB_ICONERROR );
			}
		}
	}
}
