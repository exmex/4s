// OBJBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "MainFrm.h"
#include "OBJDoc.h"
#include "OBJBar.h"
#include ".\objbar.h"
#include "ItemFinder.h"


#define OPCOUNT					22

static DWORD g_dwOP[OPCOUNT] = {
	D3DTOP_DISABLE,
	D3DTOP_MODULATE,
	D3DTOP_MODULATE2X,
	D3DTOP_MODULATE4X,
	D3DTOP_ADD,
	D3DTOP_ADDSIGNED,
	D3DTOP_ADDSIGNED2X,
	D3DTOP_SUBTRACT,
	D3DTOP_ADDSMOOTH,
	D3DTOP_BLENDDIFFUSEALPHA,
	D3DTOP_BLENDTEXTUREALPHA,
	D3DTOP_BLENDFACTORALPHA,
	D3DTOP_BLENDTEXTUREALPHAPM,
	D3DTOP_BLENDCURRENTALPHA,
	D3DTOP_MODULATEALPHA_ADDCOLOR,
	D3DTOP_MODULATECOLOR_ADDALPHA,
	D3DTOP_MODULATEINVALPHA_ADDCOLOR,
	D3DTOP_MODULATEINVCOLOR_ADDALPHA,
	D3DTOP_DOTPRODUCT3,
	D3DTOP_MULTIPLYADD,
	D3DTOP_LERP,
	D3DTOP_FORCE_DWORD};

static DWORD g_dwBLENDOP[] = {
	D3DBLENDOP_ADD,
	D3DBLENDOP_SUBTRACT,
	D3DBLENDOP_REVSUBTRACT,
	D3DBLENDOP_MIN,
	D3DBLENDOP_MAX};

static DWORD g_dwBLEND[] = {
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_SRCALPHASAT,
	D3DBLEND_BOTHSRCALPHA,
	D3DBLEND_BOTHINVSRCALPHA,
	D3DBLEND_BLENDFACTOR,
	D3DBLEND_INVBLENDFACTOR};

static int g_nBLENDOP[] = {
	0,		// D3DBLENDOP_ADD
	0,		// D3DBLENDOP_ADD
	1,		// D3DBLENDOP_SUBTRACT
	2,		// D3DBLENDOP_REVSUBTRACT
	3,		// D3DBLENDOP_MIN
	4};		// D3DBLENDOP_MAX

static int g_nBLEND[] = {
	0,		// D3DBLEND_ZERO
	0,		// D3DBLEND_ZERO
	1,		// D3DBLEND_ONE
	2,		// D3DBLEND_SRCCOLOR
	3,		// D3DBLEND_INVSRCCOLOR
	4,		// D3DBLEND_SRCALPHA
	5,		// D3DBLEND_INVSRCALPHA
	6,		// D3DBLEND_DESTALPHA
	7,		// D3DBLEND_INVDESTALPHA
	8,		// D3DBLEND_DESTCOLOR
	9,		// D3DBLEND_INVDESTCOLOR
	10,		// D3DBLEND_SRCALPHASAT
	11,		// D3DBLEND_BOTHSRCALPHA
	12,		// D3DBLEND_BOTHINVSRCALPHA
	13,		// D3DBLEND_BLENDFACTOR
	14};	// D3DBLEND_INVBLENDFACTOR

COBJItem *COBJBar::m_pDEST = NULL;
VECTOROBJBAR COBJBar::m_vOBJBAR;

// COBJBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(COBJBar, CBarContent)
COBJBar::COBJBar(CWnd* pParent /*=NULL*/)
: CBarContent(COBJBar::IDD, pParent)
, m_strPivotID(_T(""))
, m_nSTG(FALSE)
, m_nTEX(FALSE)
, m_dwORDER(0)
, m_nPIVOT(0)
, m_nOP(0)
, m_bEditPivot(FALSE)
, m_bShowSelected(TRUE)
, m_b2side(FALSE)
, m_dwRValue(0)
, m_dwGValue(0)
, m_dwBValue(0)
, m_dwAlpha(0)
, m_dwRValue_Amb(0)
, m_dwGValue_Amb(0)
, m_dwBValue_Amb(0)
, m_dwAlpha_Amb(0)
, m_nEOCL(0)
, m_nEOMESH(0)
, m_nEOACT(0)
, m_nEOANI(0)
, m_nHEBA_ACT(0)
, m_nSFXKind(0)
, m_nEPivotID(0)
, m_strEPosX(_T(""))
, m_strEPosY(_T(""))
, m_strEPosZ(_T(""))
, m_strERotX(_T(""))
, m_strERotY(_T(""))
, m_strERotZ(_T(""))
, m_dwEStartAT(0)
, m_bEStop(FALSE)
, m_nSNDKind(0)
, m_dwSNDFUNCTION(0)
, m_dwSStartAT(0)
, m_bSStop(FALSE)
, m_dwSNDOBJFUNC(0)
, m_dwSNDACTFUNC(0)
, m_dwSNDANIFUNC(0)
, m_bZEnable(TRUE)
, m_bZWrite(TRUE)
, m_bBias(0)
, m_bMinAlpha(0)
, m_strIntensity(_T(""))
, m_dwSFXFUNCTION(0)
, m_dwSFXOBJFUNC(0)
, m_dwSFXACTFUNC(0)
, m_dwSFXANIFUNC(0)
{
	m_strColorID.LoadString(IDS_NOTUSE);
	m_strColorID.MakeUpper();

	m_strPivotID.LoadString(IDS_NOTUSE);
	m_strPivotID.MakeUpper();

	m_pItem = NULL;
	m_bCurTEX = 0;
	m_dwTick = 0;
}

COBJBar::~COBJBar()
{
}

void COBJBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MESHORDER, m_cORDER);
	DDX_Control(pDX, IDC_LIST_CLOTHDATA, m_cMESH);
	DDX_Control(pDX, IDC_LIST_CLOTHKIND, m_cCLK);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_cANI);
	DDX_Control(pDX, IDC_EDIT_PIVOT, m_cPivotID);
	DDX_Control(pDX, IDC_LIST_MESHORDER, m_cMI);
	DDX_Control(pDX, IDC_COMBO_PIVOT, m_cPIVOT);
	DDX_Control(pDX, IDC_RADIO_STAGE1, m_cSTG1);
	DDX_Control(pDX, IDC_RADIO_STAGE2, m_cSTG2);
	DDX_Control(pDX, IDC_RADIO_MICOLOR, m_cCLR);
	DDX_Control(pDX, IDC_RADIO_MITEX, m_cTEX);
	DDX_Control(pDX, IDC_RADIO_MIENV, m_cENV);
	DDX_Control(pDX, IDC_LIST_ACTION, m_cACT);
	DDX_Control(pDX, IDC_LIST_CLOTH, m_cCL);
	DDX_Control(pDX, IDC_COMBO_SOP, m_cOP);
	DDX_CBIndex(pDX, IDC_COMBO_PIVOT, m_nPIVOT);
	DDX_CBIndex(pDX, IDC_COMBO_SOP, m_nOP);
	DDX_Text(pDX, IDC_EDIT_MESHORDER, m_dwORDER);
	DDX_Text(pDX, IDC_EDIT_PIVOT, m_strPivotID);
	DDX_Radio(pDX, IDC_RADIO_STAGE1, m_nSTG);
	DDX_Radio(pDX, IDC_RADIO_MITEX, m_nTEX);
	DDX_Control(pDX, IDC_CHECK_EDITPIVOT, m_cEditPivot);
	DDX_Check(pDX, IDC_CHECK_EDITPIVOT, m_bEditPivot);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL2, m_cAmbientControlBox);
	DDX_Control(pDX, IDC_STATIC_COLOR2, m_cAmbientColorBox);
	DDX_Control(pDX, IDC_CHECK_SHOW_SELECTED, m_cShowSelected);
	DDX_Check(pDX, IDC_CHECK_SHOW_SELECTED, m_bShowSelected);
	DDX_Control(pDX, IDC_CHECK_2SIDE, m_c2side);
	DDX_Check(pDX, IDC_CHECK_2SIDE, m_b2side);
	DDX_Control(pDX, IDC_SPIN_MESHORDER, m_spinMeshOrder);
	DDX_Control(pDX, IDC_EDIT_RVALUE, m_cRValue);
	DDX_Control(pDX, IDC_SPIN_RVALUE, m_spinRValue);
	DDX_Control(pDX, IDC_EDIT_GVALUE, m_cGValue);
	DDX_Text(pDX, IDC_EDIT_RVALUE, m_dwRValue);
	DDX_Text(pDX, IDC_EDIT_GVALUE, m_dwGValue);
	DDX_Control(pDX, IDC_SPIN_GVALUE, m_spinGValue);
	DDX_Control(pDX, IDC_EDIT_BVALUE, m_cBValue);
	DDX_Text(pDX, IDC_EDIT_BVALUE, m_dwBValue);
	DDX_Control(pDX, IDC_SPIN_BVALUE, m_spinBValue);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_cAlpha);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_dwAlpha);
	DDX_Control(pDX, IDC_SPIN_ALPHA, m_spinAlpha);
	DDX_Control(pDX, IDC_EDIT_COLORID, m_cColorID);
	DDX_Text(pDX, IDC_EDIT_COLORID, m_strColorID);
	DDX_Control(pDX, IDC_EDIT_EO, m_cEO);
	DDX_Control(pDX, IDC_BUTTON_DELEO, m_cDelEO);
	DDX_Control(pDX, IDC_LIST_ECK, m_cEOCLK);
	DDX_Control(pDX, IDC_COMBO_EC, m_cEOCL);
	DDX_Control(pDX, IDC_COMBO_ECD, m_cEOMESH);
	DDX_Control(pDX, IDC_COMBO_EA, m_cEOACT);
	DDX_Control(pDX, IDC_COMBO_EANI, m_cEOANI);
	DDX_Control(pDX, IDC_COMBO_HEBA_ACT, m_cHEBA_ACT);
	DDX_CBIndex(pDX, IDC_COMBO_EC, m_nEOCL);
	DDX_CBIndex(pDX, IDC_COMBO_ECD, m_nEOMESH);
	DDX_CBIndex(pDX, IDC_COMBO_EA, m_nEOACT);
	DDX_CBIndex(pDX, IDC_COMBO_EANI, m_nEOANI);
	DDX_CBIndex(pDX, IDC_COMBO_HEBA_ACT, m_nHEBA_ACT);
	DDX_Control(pDX, IDC_LIST_SFX, m_cSFX);
	DDX_Control(pDX, IDC_LIST_HEBA, m_cHEBA_LIST);
	DDX_Control(pDX, IDC_RADIO_OSFX, m_cOBJSFX);
	DDX_Control(pDX, IDC_RADIO_ASFX, m_cACTSFX);
	DDX_Control(pDX, IDC_RADIO_ANISFX, m_cANISFX);
	DDX_Radio(pDX, IDC_RADIO_OSFX, m_nSFXKind);
	DDX_Control(pDX, IDC_COMBO_OEP, m_cEPivotID);
	DDX_CBIndex(pDX, IDC_COMBO_OEP, m_nEPivotID);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinEPosX);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinEPosY);
	DDX_Control(pDX, IDC_SPIN_POSZ, m_spinEPosZ);
	DDX_Control(pDX, IDC_EDIT_POS_X, m_cEPosX);
	DDX_Control(pDX, IDC_EDIT_POS_Y, m_cEPosY);
	DDX_Control(pDX, IDC_EDIT_POS_Z, m_cEPosZ);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_strEPosX);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_strEPosY);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_strEPosZ);
	DDX_Control(pDX, IDC_SPIN_ROT_X, m_spinERotX);
	DDX_Control(pDX, IDC_SPIN_ROT_Y, m_spinERotY);
	DDX_Control(pDX, IDC_SPIN_ROT_Z, m_spinERotZ);
	DDX_Control(pDX, IDC_EDIT_ROT_X, m_cERotX);
	DDX_Control(pDX, IDC_EDIT_ROT_Y, m_cERotY);
	DDX_Control(pDX, IDC_EDIT_ROT_Z, m_cERotZ);
	DDX_Text(pDX, IDC_EDIT_ROT_X, m_strERotX);
	DDX_Text(pDX, IDC_EDIT_ROT_Y, m_strERotY);
	DDX_Text(pDX, IDC_EDIT_ROT_Z, m_strERotZ);
	DDX_Control(pDX, IDC_EDIT_START, m_cEStartAT);
	DDX_Text(pDX, IDC_EDIT_START, m_dwEStartAT);
	DDX_Control(pDX, IDC_SPIN_STARTAT, m_spinEStartAT);
	DDX_Control(pDX, IDC_CHECK_DELONSTOP, m_cEStop);
	DDX_Check(pDX, IDC_CHECK_DELONSTOP, m_bEStop);
	DDX_Control(pDX, IDC_LIST_SOUNDID, m_cSOUND);
	DDX_Control(pDX, IDC_RADIO_OS, m_cOBJSND);
	DDX_Control(pDX, IDC_RADIO_AS, m_cACTSND);
	DDX_Control(pDX, IDC_RADIO_ANIS, m_cANISND);
	DDX_Radio(pDX, IDC_RADIO_OS, m_nSNDKind);
	DDX_Control(pDX, IDC_EDIT_FUNCTION, m_cSNDFUNCTION);
	DDX_Text(pDX, IDC_EDIT_FUNCTION, m_dwSNDFUNCTION);
	DDX_Control(pDX, IDC_SPIN_FUNCTION, m_spinSNDFUNCTION);
	DDX_Control(pDX, IDC_EDIT_SSTART, m_cSStartAT);
	DDX_Text(pDX, IDC_EDIT_SSTART, m_dwSStartAT);
	DDX_Control(pDX, IDC_SPIN_SSTARTAT, m_spinSStartAT);
	DDX_Control(pDX, IDC_CHECK_SDELONSTOP, m_cSStop);
	DDX_Check(pDX, IDC_CHECK_SDELONSTOP, m_bSStop);
	DDX_Control(pDX, IDC_EDIT_OBJFUNC, m_cSNDOBJFUNC);
	DDX_Control(pDX, IDC_EDIT_ACTFUNC, m_cSNDACTFUNC);
	DDX_Control(pDX, IDC_EDIT_ANIFUNC, m_cSNDANIFUNC);
	DDX_Text(pDX, IDC_EDIT_OBJFUNC, m_dwSNDOBJFUNC);
	DDX_Text(pDX, IDC_EDIT_ACTFUNC, m_dwSNDACTFUNC);
	DDX_Text(pDX, IDC_EDIT_ANIFUNC, m_dwSNDANIFUNC);
	DDX_Control(pDX, IDC_SPIN_OBJFUNC, m_spinSNDOBJFUNC);
	DDX_Control(pDX, IDC_SPIN_ACTFUNC, m_spinSNDACTFUNC);
	DDX_Control(pDX, IDC_SPIN_ANIFUNC, m_spinSNDANIFUNC);
	DDX_Control(pDX, IDC_BUTTON_SETESTART, m_cSETESTART);
	DDX_Control(pDX, IDC_BUTTON_SETSSTART, m_cSETSSTART);
	DDX_Control(pDX, IDC_CHECK_ZENABLE, m_cZEnable);
	DDX_Check(pDX, IDC_CHECK_ZENABLE, m_bZEnable);
	DDX_Control(pDX, IDC_CHECK_ZWRITE, m_cZWrite);
	DDX_Check(pDX, IDC_CHECK_ZWRITE, m_bZWrite);
	DDX_Control(pDX, IDC_EDIT_BIAS, m_cBias);
	DDX_Text(pDX, IDC_EDIT_BIAS, m_bBias);
	DDX_Control(pDX, IDC_SPIN_BIAS, m_spinBias);
	DDX_Control(pDX, IDC_EDIT_MINALPHA, m_cMinAlpha);
	DDX_Text(pDX, IDC_EDIT_MINALPHA, m_bMinAlpha);
	DDX_Control(pDX, IDC_SPIN_MINALPHA, m_spinMinAlpha);
	DDX_Control(pDX, IDC_EDIT_INTENSITY, m_cIntensity);
	DDX_Text(pDX, IDC_EDIT_INTENSITY, m_strIntensity);
	DDX_Control(pDX, IDC_SPIN_INTENSITY, m_spinIntensity);
	DDX_Control(pDX, IDC_EDIT_SFXFUNCTION, m_cSFXFUNCTION);
	DDX_Text(pDX, IDC_EDIT_SFXFUNCTION, m_dwSFXFUNCTION);
	DDX_Control(pDX, IDC_SPIN_SFXFUNCTION, m_spinSFXFUNCTION);
	DDX_Control(pDX, IDC_EDIT_OBJSFXFUNC, m_cSFXOBJFUNC);
	DDX_Text(pDX, IDC_EDIT_OBJSFXFUNC, m_dwSFXOBJFUNC);
	DDX_Control(pDX, IDC_SPIN_OBJSFXFUNC2, m_spinSFXOBJFUNC);
	DDX_Control(pDX, IDC_EDIT_ACTSFXFUNC, m_cSFXACTFUNC);
	DDX_Text(pDX, IDC_EDIT_ACTSFXFUNC, m_dwSFXACTFUNC);
	DDX_Control(pDX, IDC_SPIN_ACTSFXFUNC, m_spinSFXACTFUNC);
	DDX_Control(pDX, IDC_EDIT_ANISFXFUNC, m_cSFXANIFUNC);
	DDX_Text(pDX, IDC_EDIT_ANISFXFUNC, m_dwSFXANIFUNC);
	DDX_Control(pDX, IDC_SPIN_ANISFXFUNC, m_spinSFXANIFUNC);
	DDX_Control(pDX, IDC_COMBO_BLENDOP, m_cBlendOP);
	DDX_Control(pDX, IDC_COMBO_SRCBLEND, m_cSRCBlend);
	DDX_Control(pDX, IDC_COMBO_DESTBLEND, m_cDESTBlend);
	DDX_Control(pDX, IDC_COPY_SND, m_cCOPYSND);
	DDX_Control(pDX, IDC_BUTTON_HEBA_DELETE, m_cHebaAdd);
	DDX_Control(pDX, IDC_BUTTON_HEBA_ADD, m_cHebaDelete);
	DDX_Control(pDX, IDC_EDIT_RVALUE2, m_cRValue2);
	DDX_Control(pDX, IDC_EDIT_GVALUE2, m_cGValue2);
	DDX_Control(pDX, IDC_EDIT_BVALUE2, m_cBValue2);
	DDX_Text(pDX, IDC_EDIT_RVALUE2, m_dwRValue_Amb);
	DDX_Text(pDX, IDC_EDIT_GVALUE2, m_dwGValue_Amb);
	DDX_Text(pDX, IDC_EDIT_BVALUE2, m_dwBValue_Amb);
	DDX_Control(pDX, IDC_SPIN_RVALUE2, m_spinRValue2);
	DDX_Control(pDX, IDC_SPIN_GVALUE2, m_spinGValue2);
	DDX_Control(pDX, IDC_SPIN_BVALUE2, m_spinBValue2);
	DDX_Control(pDX, IDC_CHECK_USEDIRLIGHT, m_cUseDirLight);
	DDX_Control(pDX, IDC_COMBO_USEDIRLIGHTOP, m_cUseDirLightOp);
	DDX_Control(pDX, IDC_EDIT_ALPHA2, m_cAlpha2);
	DDX_Control(pDX, IDC_SPIN_ALPHA2, m_spinAlpha2);
	DDX_Text(pDX, IDC_EDIT_ALPHA2, m_dwAlpha_Amb);
}


BEGIN_MESSAGE_MAP(COBJBar, CBarContent)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_MESSAGE( WM_TOBJMSG, OnOBJMessage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ACTION, OnLvnItemchangedListAction)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ANIMATION, OnLvnItemchangedListAnimation)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLOTHKIND, OnLvnItemchangedListClothkind)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLOTH, OnLvnItemchangedListCloth)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLOTHDATA, OnLvnItemchangedListClothdata)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MESHORDER, OnLvnItemchangedListMeshorder)
	ON_BN_CLICKED(IDC_RADIO_STAGE1, OnBnClickedRadioStage)
	ON_BN_CLICKED(IDC_RADIO_STAGE2, OnBnClickedRadioStage)
	ON_BN_CLICKED(IDC_RADIO_MITEX, OnBnClickedRadioMitex)
	ON_BN_CLICKED(IDC_RADIO_MICOLOR, OnBnClickedRadioMitex)
	ON_BN_CLICKED(IDC_RADIO_MIENV, OnBnClickedRadioMitex)
	ON_EN_CHANGE(IDC_EDIT_MESHORDER, OnEnChangeEditMeshorder)
	ON_CBN_SELCHANGE(IDC_COMBO_SOP, OnCbnSelchangeComboSop)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ECK, OnNMClickListEck)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ECK, OnNMClickListEck)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ECK, OnNMClickListEck)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_ECK, OnNMClickListEck)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACTION, OnNMClickListAction)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnNMClickListAction)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ACTION, OnNMClickListAction)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_ACTION, OnNMClickListAction)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ANIMATION, OnNMClickListAnimation)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ANIMATION, OnNMClickListAnimation)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_ANIMATION, OnNMClickListAnimation)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MESHORDER, OnNMClickListMeshorder)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MESHORDER, OnNMClickListMeshorder)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MESHORDER, OnNMClickListMeshorder)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_MESHORDER, OnNMClickListMeshorder)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CLOTH, OnNMClickListCloth)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLOTH, OnNMClickListCloth)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CLOTH, OnNMClickListCloth)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_CLOTH, OnNMClickListCloth)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CLOTHDATA, OnNMClickListClothdata)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CLOTHDATA, OnNMClickListClothdata)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_CLOTHDATA, OnNMClickListClothdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CLOTHKIND, OnNMClickListClothkind)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLOTHKIND, OnNMClickListClothkind)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CLOTHKIND, OnNMClickListClothkind)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_CLOTHKIND, OnNMClickListClothkind)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLOTHDATA, OnNMDblclkListClothdata)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ANIMATION, OnNMDblclkListAnimation)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_ANIMATION, OnNMRDblclkListAnimation) // KSH. 아이템삭제 : 오른쪽 더블클릭으로 수정.
	ON_BN_CLICKED(IDC_CHECK_SHOW_SELECTED, OnBnClickedCheckShowSelected)
	ON_BN_CLICKED(IDC_CHECK_2SIDE, OnBnClickedCheck2side)
	ON_EN_CHANGE(IDC_EDIT_RVALUE, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_GVALUE, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_BVALUE, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_COLORID, OnEnChangeEditColorid)
	ON_EN_SETFOCUS(IDC_EDIT_COLORID, OnEnSetfocusEditColorid)
	ON_EN_KILLFOCUS(IDC_EDIT_COLORID, OnEnKillfocusEditColorid)
	ON_EN_KILLFOCUS(IDC_EDIT_PIVOT, OnEnKillfocusEditPivot)
	ON_EN_SETFOCUS(IDC_EDIT_PIVOT, OnEnSetfocusEditPivot)
	ON_CBN_SELCHANGE(IDC_COMBO_PIVOT, OnCbnSelchangeComboPivot)
	ON_BN_CLICKED(IDC_CHECK_EDITPIVOT, OnBnClickedCheckEditpivot)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ECK, OnLvnItemchangedListEck)
	ON_CBN_SELCHANGE(IDC_COMBO_EC, OnCbnSelchangeComboEc)
	ON_CBN_SELCHANGE(IDC_COMBO_ECD, OnCbnSelchangeComboEcd)
	ON_CBN_SELCHANGE(IDC_COMBO_EA, OnCbnSelchangeComboEa)
	ON_CBN_SELCHANGE(IDC_COMBO_EANI, OnCbnSelchangeComboEani)
	ON_BN_CLICKED(IDC_BUTTON_DELEO, OnBnClickedButtonDeleo)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_CLOTHDATA, OnLvnEndlabeleditListClothdata)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_ANIMATION, OnLvnEndlabeleditListAnimation)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_CLOTHDATA, OnLvnBeginlabeleditListClothdata)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_ANIMATION, OnLvnBeginlabeleditListAnimation)
	ON_BN_CLICKED(IDC_RADIO_ANISFX, OnBnClickedRadioSFX)
	ON_BN_CLICKED(IDC_RADIO_ASFX, OnBnClickedRadioSFX)
	ON_BN_CLICKED(IDC_RADIO_OSFX, OnBnClickedRadioSFX)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SFX, OnLvnItemchangedListSfx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SFX, OnNMClickListSfx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SFX, OnNMRclickListSfx)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_SFX, OnNMRdblclkListSfx) // KSH. 우클릭 좌클릭 기능을 서로 바꿈. 오른쪽 더블클릭시에 아이템 삭제.
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SFX, OnNMDblclkListSfx)
	ON_EN_CHANGE(IDC_EDIT_POS_X, OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT_ROT_X, OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT_ROT_Y, OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT_ROT_Z, OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT_START, OnEnChangeEditStart)
	ON_BN_CLICKED(IDC_CHECK_DELONSTOP, OnBnClickedCheckDelonstop)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_OEP, OnCbnSelchangeComboOep)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SOUNDID, OnLvnItemchangedListSoundid)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOUNDID, OnNMClickListSoundid)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SOUNDID, OnNMRclickListSoundid)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_SOUNDID, OnNMRdblclkListSoundid)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SOUNDID, OnNMDblclkListSoundid)
	ON_BN_CLICKED(IDC_RADIO_OS, OnBnClickedRadioSND)
	ON_BN_CLICKED(IDC_RADIO_AS, OnBnClickedRadioSND)
	ON_BN_CLICKED(IDC_RADIO_ANIS, OnBnClickedRadioSND)
	ON_EN_CHANGE(IDC_EDIT_SSTART, OnEnChangeEditSstart)
	ON_EN_CHANGE(IDC_EDIT_FUNCTION, OnEnChangeEditFunction)
	ON_BN_CLICKED(IDC_CHECK_SDELONSTOP, OnBnClickedCheckSdelonstop)
	ON_EN_CHANGE(IDC_EDIT_OBJFUNC, OnEnChangeEditObjfunc)
	ON_EN_CHANGE(IDC_EDIT_ACTFUNC, OnEnChangeEditActfunc)
	ON_EN_CHANGE(IDC_EDIT_ANIFUNC, OnEnChangeEditAnifunc)
	ON_BN_CLICKED(IDC_BUTTON_SETESTART, OnBnClickedButtonSetestart)
	ON_BN_CLICKED(IDC_BUTTON_SETSSTART, OnBnClickedButtonSetsstart)
	ON_EN_CHANGE(IDC_EDIT_PIVOT, OnEnChangeEditPivot)
	ON_BN_CLICKED(IDC_CHECK_ZENABLE, OnBnClickedCheckZenable)
	ON_BN_CLICKED(IDC_CHECK_ZWRITE, OnBnClickedCheckZwrite)
	ON_EN_CHANGE(IDC_EDIT_BIAS, OnEnChangeEditBias)
	ON_EN_CHANGE(IDC_EDIT_MINALPHA, OnEnChangeEditMinalpha)
	ON_EN_CHANGE(IDC_EDIT_INTENSITY, OnEnChangeEditIntensity)
	ON_EN_CHANGE(IDC_EDIT_SFXFUNCTION, OnEnChangeEditSfxfunction)
	ON_EN_CHANGE(IDC_EDIT_OBJSFXFUNC, OnEnChangeEditObjsfxfunc)
	ON_EN_CHANGE(IDC_EDIT_ACTSFXFUNC, OnEnChangeEditActsfxfunc)
	ON_EN_CHANGE(IDC_EDIT_ANISFXFUNC, OnEnChangeEditAnisfxfunc)
	ON_CBN_SELCHANGE(IDC_COMBO_BLENDOP, OnCbnSelchangeComboBlendop)
	ON_CBN_SELCHANGE(IDC_COMBO_DESTBLEND, OnCbnSelchangeComboDestblend)
	ON_CBN_SELCHANGE(IDC_COMBO_SRCBLEND, OnCbnSelchangeComboSrcblend)
	ON_BN_CLICKED(IDC_CHECK_BATCH_REG_SNDFUNC, OnBnClickedCheckBatchRegSndfunc)
	ON_BN_CLICKED(IDC_COPY_SND, OnBnClickedCopySnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DELETE_CLOTHDATA, OnBnClickedDeleteClothdata)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CLOTHDATA, OnLvnKeydownListClothdata)
	ON_BN_CLICKED(IDC_CHECK_DELETEANI, OnBnClickedCheckDeleteani)
	ON_BN_CLICKED(IDC_CHECK_DELEFFECT, OnBnClickedCheckDeleffect)
	ON_BN_CLICKED(IDC_CHECK_DELSOUND, OnBnClickedCheckDelsound)
	ON_BN_CLICKED(IDC_BUTTON_SFXUP, OnBnClickedButtonSfxup)
	ON_BN_CLICKED(IDC_BUTTON_SFXDOWN, OnBnClickedButtonSfxdown)
	ON_BN_CLICKED(IDC_BUTTON_HEBA_DELETE, OnBnClickedButtonHebaDelete)
	ON_BN_CLICKED(IDC_BUTTON_HEBA_ADD, OnBnClickedButtonHebaAdd)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_COMBO_HEBA_ACT, OnNMThemeChangedComboHebaAct)
	ON_BN_CLICKED(IDC_FIND_OBJTEXTURE, OnBnClickedFindObjtexture)
	ON_EN_CHANGE(IDC_EDIT_RVALUE2, OnEnChangeEditColor2)
	ON_EN_CHANGE(IDC_EDIT_GVALUE2, OnEnChangeEditColor2)
	ON_EN_CHANGE(IDC_EDIT_BVALUE2, OnEnChangeEditColor2)
	ON_BN_CLICKED(IDC_CHECK_USEDIRLIGHT, OnBnClickedCheckUsedirlight)
	ON_CBN_SELCHANGE(IDC_COMBO_USEDIRLIGHTOP, OnCbnSelchangeComboUsedirlightop)
	END_MESSAGE_MAP()


void COBJBar::InitOBJBAR()
{
	m_vOBJBAR.clear();
	m_pDEST = NULL;
}

void COBJBar::ReleaseOBJBAR()
{
	m_vOBJBAR.clear();
	m_pDEST = NULL;
}

BOOL COBJBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDelEO.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SETDIST);
	m_cSETESTART.SetIcon(hIcon);
	m_cSETSSTART.SetIcon(hIcon);

	m_cControlBox.GetClientRect(&rect);
	m_cControlBox.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;
	m_cColorCtrl = CTColorCtrl::MODE_HSL_PANEL;

	m_cAmbientControlBox.GetClientRect( &rect );
	m_cAmbientControlBox.ClientToScreen( &rect );
	ScreenToClient( &rect );
	rect.DeflateRect( 1, 1 );

	if(!m_cAmbientControlCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0 ) )
		return FALSE;
	m_cAmbientControlCtrl = CTColorCtrl::MODE_HSL_PANEL;

	m_cSOUND.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cEOCLK.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cMESH.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cSFX.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cCLK.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cANI.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cACT.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cCL.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cMI.GetListCtrl()->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

	m_cSOUND.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cSOUND.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cEOCLK.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cEOCLK.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cMESH.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cMESH.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cSFX.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cSFX.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cCLK.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cCLK.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cANI.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cANI.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cACT.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cACT.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());
	
	m_cHEBA_LIST.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cHEBA_LIST.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cCL.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cCL.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cMI.GetListCtrl()->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cMI.GetListCtrl()->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cSOUND.m_DropTarget.Register(&m_cSOUND);
	m_cSOUND.m_pDragEnter = OnSNDDragOver;
	m_cSOUND.m_pDragOver = OnSNDDragOver;
	m_cSOUND.m_pDrop = OnSNDDrop;
	m_cSOUND.m_pLeave = OnSNDDragLeave;

	m_cANI.m_DropTarget.Register(&m_cANI);
	m_cANI.m_pDragEnter = OnANIDragOver;
	m_cANI.m_pDragOver = OnANIDragOver;
	m_cANI.m_pDrop = OnANIDrop;
	m_cANI.m_pLeave = OnANIDragLeave;

	m_cMESH.m_DropTarget.Register(&m_cMESH);
	m_cMESH.m_pDragEnter = OnMESHDragOver;
	m_cMESH.m_pDragOver = OnMESHDragOver;
	m_cMESH.m_pDrop = OnMESHDrop;
	m_cMESH.m_pLeave = OnMESHDragLeave;

	m_cSFX.m_DropTarget.Register(&m_cSFX);
	m_cSFX.m_pDragEnter = OnSFXDragOver;
	m_cSFX.m_pDragOver = OnSFXDragOver;
	m_cSFX.m_pDrop = OnSFXDrop;
	m_cSFX.m_pLeave = OnSFXDragLeave;

	m_cEO.m_DropTarget.Register(&m_cEO);
	m_cEO.m_pDragEnter = OnEODragOver;
	m_cEO.m_pDragOver = OnEODragOver;
	m_cEO.m_pDrop = OnEODrop;
	m_cEO.m_pLeave = OnEODragLeave;

	m_cOP.InsertString( 0, "Stage 1");
	m_cOP.InsertString( 1, "Modulate");
	m_cOP.InsertString( 2, "Modulate 2X");
	m_cOP.InsertString( 3, "Modulate 4X");
	m_cOP.InsertString( 4, "Add");
	m_cOP.InsertString( 5, "Add signed");
	m_cOP.InsertString( 6, "Add signed 2X");
	m_cOP.InsertString( 7, "Subtract");
	m_cOP.InsertString( 8, "Add smooth");
	m_cOP.InsertString( 9, "Blend Diffuse alpha");
	m_cOP.InsertString( 10, "Blend texture alpha");
	m_cOP.InsertString( 11, "Blend factor alpha");
	m_cOP.InsertString( 12, "Blend texture alpha apm");
	m_cOP.InsertString( 13, "Blend current alpha");
	m_cOP.InsertString( 14, "Modulate alpha add color");
	m_cOP.InsertString( 15, "Modulate color add alpha");
	m_cOP.InsertString( 16, "Modulate invalpha add color");
	m_cOP.InsertString( 17, "Modulate invcolor add alpha");
	m_cOP.InsertString( 18, "Dot product");
	m_cOP.InsertString( 19, "Multiply add");
	m_cOP.InsertString( 20, "Lerp");
	m_cOP.InsertString( 21, "Shader");

	m_cUseDirLightOp.SetCurSel( 0 );

	m_spinSFXFUNCTION.SetRange( 0, 10000);
	m_spinSNDFUNCTION.SetRange( 0, 10000);
	m_spinSFXOBJFUNC.SetRange( 0, 10000);
	m_spinSFXACTFUNC.SetRange( 0, 10000);
	m_spinSFXANIFUNC.SetRange( 0, 10000);
	m_spinSNDOBJFUNC.SetRange( 0, 10000);
	m_spinSNDACTFUNC.SetRange( 0, 10000);
	m_spinSNDANIFUNC.SetRange( 0, 10000);
	m_spinEStartAT.SetRange( 0, 10000);
	m_spinSStartAT.SetRange( 0, 10000);
	m_spinMeshOrder.SetRange( 0, 100);
	m_spinEPosX.SetRange( -100, 100);
	m_spinEPosY.SetRange( -100, 100);
	m_spinEPosZ.SetRange( -100, 100);
	m_spinERotX.SetRange( -100, 100);
	m_spinERotY.SetRange( -100, 100);
	m_spinERotZ.SetRange( -100, 100);
	m_spinIntensity.SetRange( -100, 100);
	m_spinMinAlpha.SetRange( 0, 255);
	m_spinRValue.SetRange( 0, 255);
	m_spinGValue.SetRange( 0, 255);
	m_spinBValue.SetRange( 0, 255);
	m_spinAlpha.SetRange( 0, 100);
	m_spinRValue2.SetRange( 0, 255);
	m_spinGValue2.SetRange( 0, 255);
	m_spinBValue2.SetRange( 0, 255);
	m_spinAlpha2.SetRange( 0, 100);
	m_spinBias.SetRange( 0, 255);
	return TRUE;
}

void COBJBar::EnableAllCtrl( BOOL bEnable)
{
	m_cMESH.EnableWindow(bEnable && GetCurCL());
	m_cANI.EnableWindow(bEnable && GetCurACT());
	m_cCL.EnableWindow(bEnable && GetCurCLK());

	m_cORDER.EnableWindow(bEnable && GetCurMESH());
	m_cSTG1.EnableWindow(bEnable && GetCurMESH());
	m_cSTG2.EnableWindow(bEnable && GetCurMESH());
	m_cTEX.EnableWindow(bEnable && GetCurMESH());
	m_cCLR.EnableWindow(bEnable && GetCurMESH());
	m_cENV.EnableWindow(bEnable && GetCurMESH());
	m_cOP.EnableWindow(bEnable && GetCurMESH());
	m_cMI.EnableWindow(bEnable && GetCurMESH());

	m_cDESTBlend.EnableWindow(bEnable && GetCurMESH());
	m_cSRCBlend.EnableWindow(bEnable && GetCurMESH());
	m_cBlendOP.EnableWindow(bEnable && GetCurMESH());

	m_spinMeshOrder.EnableWindow(bEnable && GetCurMESH());
	m_spinIntensity.EnableWindow(bEnable && GetCurMESH());
	m_spinMinAlpha.EnableWindow(bEnable && GetCurMESH());
	m_cIntensity.EnableWindow(bEnable && GetCurMESH());
	m_cMinAlpha.EnableWindow(bEnable && GetCurMESH());
	m_cZEnable.EnableWindow(bEnable && GetCurMESH());
	m_cZWrite.EnableWindow(bEnable && GetCurMESH());
	m_c2side.EnableWindow(bEnable && GetCurMESH());

	m_spinRValue.EnableWindow(bEnable && GetCurMESH());
	m_spinGValue.EnableWindow(bEnable && GetCurMESH());
	m_spinBValue.EnableWindow(bEnable && GetCurMESH());
	m_spinAlpha.EnableWindow(bEnable && GetCurMESH());

	m_cColorCtrl.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);
	m_cColorID.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);
	m_cRValue.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);
	m_cGValue.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);
	m_cBValue.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);
	m_cAlpha.EnableWindow(bEnable && GetCurMESH() && m_nTEX == TT_COLOR);

	int nCheck = m_cUseDirLight.GetCheck();
	m_spinRValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_spinGValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_spinBValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);

	m_cAmbientControlCtrl.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_cRValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_cGValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_cBValue2.EnableWindow(bEnable && GetCurMESH() && !nCheck);
	m_cAlpha2.EnableWindow(bEnable && GetCurMESH() && !nCheck);

	m_cUseDirLight.EnableWindow(bEnable && GetCurMESH());

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	int nPivotCount = m_pItem->GetPivotCount();

	m_cEditPivot.EnableWindow(bEnable && nPivotCount > 0);
	m_cPivotID.EnableWindow(bEnable && nPivotCount > 0);
	m_cPIVOT.EnableWindow(bEnable && nPivotCount > 0);
	m_cEO.EnableWindow(bEnable && nPivotCount > 0);

	m_cDelEO.EnableWindow(bEnable && nPivotCount > 0 && GetCurEI());
	m_cEOCLK.EnableWindow(bEnable && nPivotCount > 0 && GetCurEI());

	m_cEOMESH.EnableWindow(bEnable && nPivotCount > 0 && m_cEOMESH.GetCount() > 0);
	m_cEOACT.EnableWindow(bEnable && nPivotCount > 0 && m_cEOACT.GetCount() > 0);
	m_cEOANI.EnableWindow(bEnable && nPivotCount > 0 && m_cEOANI.GetCount() > 0);
	m_cEOCL.EnableWindow(bEnable && nPivotCount > 0 && m_cEOCL.GetCount() > 0);

	m_cShowSelected.EnableWindow(bEnable);
	m_cACT.EnableWindow(bEnable);
	m_cCLK.EnableWindow(bEnable);

	switch(m_nSFXKind)
	{
	case OEK_OBJ	: m_cSFX.EnableWindow(bEnable); break;
	case OEK_ACT	: m_cSFX.EnableWindow(bEnable && GetCurACT()); break;
	case OEK_ANI	:
		{
			LPACTINST pAI = GetCurAI();
			m_cSFX.EnableWindow(bEnable && pAI && pAI->m_pANI);
		}

		break;
	}

	switch(m_nSNDKind)
	{
	case OSK_OBJ	: m_cSOUND.EnableWindow(bEnable); break;
	case OSK_ACT	: m_cSOUND.EnableWindow(bEnable && GetCurACT()); break;
	case OSK_ANI	:
		{
			LPACTINST pAI = GetCurAI();
			m_cSOUND.EnableWindow(bEnable && pAI && pAI->m_pANI);
		}

		break;
	}

	m_spinEPosX.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinEPosY.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinEPosZ.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinERotX.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinERotY.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinERotZ.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinBias.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEPosX.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEPosY.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEPosZ.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cERotX.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cERotY.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cERotZ.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cBias.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);

	m_spinSFXFUNCTION.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_spinEStartAT.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cSFXFUNCTION.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEPivotID.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEStartAT.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);
	m_cEStop.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1);

	m_spinSNDFUNCTION.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1);
	m_spinSStartAT.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1);
	m_cSNDFUNCTION.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1);
	m_cSStartAT.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1);
	m_cSStop.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1);

	m_cSETSSTART.EnableWindow(bEnable && m_cSOUND.GetCurSel() != -1 && GetCurAI() && GetCurAI()->m_pANI);
	m_cSETESTART.EnableWindow(bEnable && m_cSFX.GetCurSel() != -1 && GetCurAI() && GetCurAI()->m_pANI);
	m_cCOPYSND.EnableWindow(bEnable && m_pDEST && m_pDEST != m_pItem && m_nSNDKind == OSK_OBJ);

	m_spinSFXOBJFUNC.EnableWindow(bEnable);
	m_spinSFXACTFUNC.EnableWindow(bEnable);
	m_spinSFXANIFUNC.EnableWindow(bEnable);
	m_spinSNDOBJFUNC.EnableWindow(bEnable);
	m_spinSNDACTFUNC.EnableWindow(bEnable);
	m_spinSNDANIFUNC.EnableWindow(bEnable);

	m_cSFXOBJFUNC.EnableWindow(bEnable);
	m_cSFXACTFUNC.EnableWindow(bEnable);
	m_cSFXANIFUNC.EnableWindow(bEnable);
	m_cSNDOBJFUNC.EnableWindow(bEnable);
	m_cSNDACTFUNC.EnableWindow(bEnable);
	m_cSNDANIFUNC.EnableWindow(bEnable);

	m_cOBJSFX.EnableWindow(bEnable);
	m_cACTSFX.EnableWindow(bEnable);
	m_cANISFX.EnableWindow(bEnable);
	m_cOBJSND.EnableWindow(bEnable);
	m_cACTSND.EnableWindow(bEnable);
	m_cANISND.EnableWindow(bEnable);

	//여기
	m_cHEBA_ACT.EnableWindow(bEnable && m_nSFXKind == OEK_OBJ && m_cSFX.GetCurSel() != -1);
	m_cHebaAdd.EnableWindow(bEnable && m_nSFXKind == OEK_OBJ && m_cSFX.GetCurSel() != -1);
	m_cHebaDelete.EnableWindow(bEnable && m_nSFXKind == OEK_OBJ && m_cSFX.GetCurSel() != -1);
	m_cHEBA_LIST.EnableWindow(bEnable && m_nSFXKind == OEK_OBJ && m_cSFX.GetCurSel() != -1);
}

void COBJBar::SetDEST( CItemBase *pItem)
{
	m_pDEST = pItem && pItem->m_bType == ITEM_OBJECT ? (COBJItem *) pItem : NULL;

	for( int i=0; i<INT(m_vOBJBAR.size()); i++)
	{
		COBJDoc *pDoc = (COBJDoc *) m_vOBJBAR[i]->m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		m_vOBJBAR[i]->EnableAllCtrl(!pCtrl->m_bPlay);
	}
}

void COBJBar::SetItem( COBJItem *pItem)
{
	m_pItem = pItem;

	m_dwSFXOBJFUNC = m_pItem->m_TOBJ.m_dwOBJSFXFuncID;
	m_dwSFXACTFUNC = m_pItem->m_TOBJ.m_dwACTSFXFuncID;
	m_dwSFXANIFUNC = m_pItem->m_TOBJ.m_dwANISFXFuncID;
	m_dwSNDOBJFUNC = m_pItem->m_TOBJ.m_dwOBJSNDFuncID;
	m_dwSNDACTFUNC = m_pItem->m_TOBJ.m_dwACTSNDFuncID;
	m_dwSNDANIFUNC = m_pItem->m_TOBJ.m_dwANISNDFuncID;

	SetActionList();
	SetClothList();
	LoadPivot();
	InitSFXList();
	InitSNDList();

	EnableAllCtrl(TRUE);
}

void COBJBar::SetActionList()
{
	CListCtrl *pListCtrl = m_cACT.GetListCtrl();
	MAPACTION::iterator it;

	m_cANI.GetListCtrl()->DeleteAllItems();
	pListCtrl->DeleteAllItems();

	m_cHEBA_ACT.ResetContent();

	for( it = m_pItem->m_OBJ.m_mapACTION.begin(); it != m_pItem->m_OBJ.m_mapACTION.end(); it++)
	{
		MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*it).first);

		int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*finder).second);
		pListCtrl->SetItemData( nIndex, (*it).first);

		if( (*it).first == m_pItem->m_TOBJ.m_OBJ.m_dwCurACT )
			m_cACT.SetCurSel(nIndex);

		nIndex = m_cHEBA_ACT.InsertString( m_cHEBA_ACT.GetCount(), (*finder).second);
		m_cHEBA_ACT.SetItemData( nIndex, (*it).first);
	}

	if( m_cACT.GetCurSel() == -1 )
		m_pItem->m_TOBJ.m_OBJ.m_dwCurACT = 0;

	m_cACT.UpdateColumnWidth();
	SetAction();
}

void COBJBar::SetClothList()
{
	CListCtrl *pListCtrl = m_cCLK.GetListCtrl();
	MAPCLKIND::iterator it;

	m_cMESH.GetListCtrl()->DeleteAllItems();
	m_cCL.GetListCtrl()->DeleteAllItems();
	m_cMI.GetListCtrl()->DeleteAllItems();
	pListCtrl->DeleteAllItems();

	for( it = m_pItem->m_OBJ.m_mapCLKIND.begin(); it != m_pItem->m_OBJ.m_mapCLKIND.end(); it++)
	{
		MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*it).first);

		int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*finder).second);
		pListCtrl->SetItemData( nIndex, (*it).first);

		if( (*it).first == m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK )
			m_cCLK.SetCurSel(nIndex);
	}

	if( m_cCLK.GetCurSel() == -1 )
		m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK = 0;
	m_cCLK.UpdateColumnWidth();
}

LPCLKINST COBJBar::GetCurCI()
{
	if( m_pItem && m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK != 0 )
	{
		MAPCLKINST::iterator finder = m_pItem->m_TOBJ.m_OBJ.m_mapCLK.find(m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK);

		if( finder == m_pItem->m_TOBJ.m_OBJ.m_mapCLK.end() )
		{
			LPCLKINST pCI = new CLKINST();

			m_pItem->m_TOBJ.m_OBJ.m_mapCLK.insert( MAPCLKINST::value_type(
				m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK,
				pCI));

			return pCI;
		}
		else
			return (*finder).second;
	}

	return NULL;
}

LPACTINST COBJBar::GetCurAI()
{
	if( m_pItem && m_pItem->m_TOBJ.m_OBJ.m_dwCurACT != 0 )
	{
		MAPACTINST::iterator finder = m_pItem->m_TOBJ.m_OBJ.m_mapACT.find(m_pItem->m_TOBJ.m_OBJ.m_dwCurACT);

		if( finder == m_pItem->m_TOBJ.m_OBJ.m_mapACT.end() )
		{
			LPACTINST pAI = new ACTINST();

			m_pItem->m_TOBJ.m_OBJ.m_mapACT.insert( MAPACTINST::value_type(
				m_pItem->m_TOBJ.m_OBJ.m_dwCurACT,
				pAI));

			return pAI;
		}
		else
			return (*finder).second;
	}

	return NULL;
}

LPACTION COBJBar::GetCurACT()
{
	if(m_pItem)
	{
		MAPACTION::iterator finder = m_pItem->m_OBJ.m_mapACTION.find(m_pItem->m_TOBJ.m_OBJ.m_dwCurACT);

		if( finder != m_pItem->m_OBJ.m_mapACTION.end() )
			return (*finder).second;
	}

	return NULL;
}

LPCLKIND COBJBar::GetCurCLK()
{
	if(m_pItem)
	{
		MAPCLKIND::iterator finder = m_pItem->m_OBJ.m_mapCLKIND.find(m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK);

		if( finder != m_pItem->m_OBJ.m_mapCLKIND.end() )
			return (*finder).second;
	}

	return NULL;
}

LPCLOTH COBJBar::GetCurCL()
{
	if(m_pItem)
	{
		LPCLKIND pCLK = GetCurCLK();
		LPCLKINST pCI = GetCurCI();

		if(pCLK)
		{
			MAPCLOTH::iterator finder = pCLK->m_mapCLOTH.find(pCI->m_dwCurCL);

			if( finder != pCLK->m_mapCLOTH.end() )
				return (*finder).second;
		}
	}

	return NULL;
}

LPMESH COBJBar::GetCurMESH()
{
	if(m_pItem)
	{
		LPCLKINST pCI = GetCurCI();
		LPCLOTH pCL = GetCurCL();

		if(pCL)
		{
			MAPMESH::iterator finder = pCL->m_mapMESH.find(pCI->m_dwCurMESH);

			if( finder != pCL->m_mapMESH.end() )
				return (*finder).second;
		}
	}

	return NULL;
}

CTachyonObject *COBJBar::GetCurEO()
{
	MAPOBJECT::iterator finder = m_pItem->m_TOBJ.m_mapEQUIP.find(m_nPIVOT + 1);

	if( m_nPIVOT != -1 && finder != m_pItem->m_TOBJ.m_mapEQUIP.end() )
		return (*finder).second;

	return NULL;
}

COBJItem *COBJBar::GetCurEI()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	MAPDWORD::iterator finder = pDoc->m_mapEQUIP.find(m_nPIVOT + 1);

	if( m_nPIVOT != -1 && finder != pDoc->m_mapEQUIP.end() )
		return CItemBase::GetOBJItem((*finder).second);

	return NULL;
}

LPACTION COBJBar::GetCurEACT()
{
	CTachyonObject *pOBJ = GetCurEO();

	if(pOBJ)
	{
		MAPACTION::iterator finder = pOBJ->m_OBJ.m_pOBJ->m_mapACTION.find(pOBJ->m_OBJ.m_dwCurACT);

		if( finder != pOBJ->m_OBJ.m_pOBJ->m_mapACTION.end() )
			return (*finder).second;
	}

	return NULL;
}

LPCLKIND COBJBar::GetCurECLK()
{
	CTachyonObject *pOBJ = GetCurEO();

	if(pOBJ)
	{
		MAPCLKIND::iterator finder = pOBJ->m_OBJ.m_pOBJ->m_mapCLKIND.find(pOBJ->m_OBJ.m_dwCurCLK);

		if( finder != pOBJ->m_OBJ.m_pOBJ->m_mapCLKIND.end() )
			return (*finder).second;
	}

	return NULL;
}

LPCLOTH COBJBar::GetCurECL()
{
	LPCLKIND pCLK = GetCurECLK();
	LPCLKINST pCI = GetCurECI();

	if(pCLK)
	{
		MAPCLOTH::iterator finder = pCLK->m_mapCLOTH.find(pCI->m_dwCurCL);

		if( finder != pCLK->m_mapCLOTH.end() )
			return (*finder).second;
	}

	return NULL;
}

LPACTINST COBJBar::GetCurEAI()
{
	CTachyonObject *pOBJ = GetCurEO();

	if( pOBJ && pOBJ->m_OBJ.m_dwCurACT != 0 )
	{
		MAPACTINST::iterator finder = pOBJ->m_OBJ.m_mapACT.find(pOBJ->m_OBJ.m_dwCurACT);

		if( finder == pOBJ->m_OBJ.m_mapACT.end() )
		{
			LPACTINST pAI = new ACTINST();

			pOBJ->m_OBJ.m_mapACT.insert( MAPACTINST::value_type(
				pOBJ->m_OBJ.m_dwCurACT,
				pAI));

			return pAI;
		}
		else
			return (*finder).second;
	}

	return NULL;
}

LPCLKINST COBJBar::GetCurECI()
{
	CTachyonObject *pOBJ = GetCurEO();

	if( pOBJ && pOBJ->m_OBJ.m_dwCurCLK != 0 )
	{
		MAPCLKINST::iterator finder = pOBJ->m_OBJ.m_mapCLK.find(pOBJ->m_OBJ.m_dwCurCLK);

		if( finder == pOBJ->m_OBJ.m_mapCLK.end() )
		{
			LPCLKINST pCI = new CLKINST();

			pOBJ->m_OBJ.m_mapCLK.insert( MAPCLKINST::value_type(
				pOBJ->m_OBJ.m_dwCurCLK,
				pCI));

			return pCI;
		}
		else
			return (*finder).second;
	}

	return NULL;
}

LPSFXINST COBJBar::GetCurSFX()
{
	CListCtrl *pList = m_cSFX.GetListCtrl();
	LPSFXINST pResult = NULL;

	int nIndex = m_cSFX.GetCurSel();
	if( nIndex != -1 )
	{
		DWORD dwID = (DWORD) pList->GetItemData(nIndex);
		LPMAPSFXINST pINST = NULL;

		switch(m_nSFXKind)
		{
		case OEK_OBJ	: pINST = &m_pItem->m_OBJ.m_mapSFX; break;
		case OEK_ACT	:
			{
				LPACTION pACT = GetCurACT();
				if(pACT)
					pINST = &pACT->m_mapSFX;
			}

			break;

		case OEK_ANI	:
			{
				LPACTINST pAI = GetCurAI();
				if( pAI && pAI->m_pANI )
					pINST = &pAI->m_pANI->m_mapSFX;
			}

			break;
		}

		if(pINST)
		{
			MAPSFXINST::iterator finder = pINST->find(dwID);
			if( finder != pINST->end() )
				pResult = (*finder).second;
		}
	}

	return pResult;
}

void COBJBar::OnLvnItemchangedListAction( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		m_pItem->m_TOBJ.m_OBJ.m_dwCurACT = (DWORD) m_cACT.GetListCtrl()->GetItemData(pNMLV->iItem);

		CListCtrl *pListCtrl = m_cANI.GetListCtrl();
		LPACTION pACT = GetCurACT();
		LPACTINST pAI = GetCurAI();

		pListCtrl->DeleteAllItems();
		if(pACT)
		{
			MAPANI::iterator it;

			for( it = pACT->m_mapANI.begin(); it != pACT->m_mapANI.end(); it++)
			{
				MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*it).first);

				int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*finder).second);
				pListCtrl->SetItemData( nIndex, (*it).first);

				if( (*it).first == pAI->m_dwCurANI )
					m_cANI.SetCurSel(nIndex);
			}

			if( m_cANI.GetCurSel() == -1 )
			{
				pAI->m_dwCurANI = 0;
				SetAction();
			}
		}

		m_cANI.UpdateColumnWidth();
		EnableAllCtrl(TRUE);
		InitSFXList();
		InitSNDList();
	}

	*pResult = 0;
}

void COBJBar::OnLvnItemchangedListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		LPACTINST pAI = GetCurAI();

		if(pAI)
			pAI->m_dwCurANI = (DWORD) m_cANI.GetListCtrl()->GetItemData(pNMLV->iItem);

		SetAction();
		EnableAllCtrl(TRUE);
		InitSFXList();
		InitSNDList();
	}

	*pResult = 0;
}

void COBJBar::OnLvnItemchangedListClothkind( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK = (DWORD) m_cCLK.GetListCtrl()->GetItemData(pNMLV->iItem);

		CListCtrl *pListCtrl = m_cCL.GetListCtrl();
		LPCLKIND pCLK = GetCurCLK();
		LPCLKINST pCI = GetCurCI();

		pListCtrl->DeleteAllItems();
		if(pCLK)
		{
			MAPCLOTH::iterator it;

			for( it = pCLK->m_mapCLOTH.begin(); it != pCLK->m_mapCLOTH.end(); it++)
			{
				MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*it).first);

				int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*finder).second);
				pListCtrl->SetItemData( nIndex, (*it).first);

				if( (*it).first == pCI->m_dwCurCL )
					m_cCL.SetCurSel(nIndex);
			}

			if( m_cCL.GetCurSel() == -1 )
			{
				m_cMESH.GetListCtrl()->DeleteAllItems();
				m_cMI.GetListCtrl()->DeleteAllItems();

				pCI->m_dwCurCL = 0;
				SetCloth();
			}
		}

		m_cCL.UpdateColumnWidth();
		EnableAllCtrl(TRUE);
	}

	*pResult = 0;
}

void COBJBar::OnLvnItemchangedListCloth( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		CListCtrl *pListCtrl = m_cMESH.GetListCtrl();
		LPCLKINST pCI = GetCurCI();

		pListCtrl->DeleteAllItems();
		if(pCI)
		{
			pCI->m_dwCurCL = (DWORD) m_cCL.GetListCtrl()->GetItemData(pNMLV->iItem);
			LPCLOTH pCL = GetCurCL();

			if(pCL)
			{
				MAPMESH::iterator it;

				for( it = pCL->m_mapMESH.begin(); it != pCL->m_mapMESH.end(); it++)
				{
					MAPSTR::iterator finder = m_pItem->m_mapNAME.find((*it).first);

					int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*finder).second);
					pListCtrl->SetItemData( nIndex, (*it).first);

					if( (*it).first == pCI->m_dwCurMESH )
						m_cMESH.SetCurSel(nIndex);
				}

				if( m_cMESH.GetCurSel() == -1 )
				{
					m_cMI.GetListCtrl()->DeleteAllItems();
					pCI->m_dwCurMESH = 0;
					SetCloth();
				}
			}
		}

		m_cMESH.UpdateColumnWidth();
		EnableAllCtrl(TRUE);
	}

	*pResult = 0;
}

void COBJBar::OnLvnItemchangedListClothdata( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		CListCtrl *pListCtrl = m_cMI.GetListCtrl();
		LPCLKINST pCI = GetCurCI();
		LPCLOTH pCL = GetCurCL();

		pListCtrl->DeleteAllItems();
		m_bCurTEX = 0;

		if(pCL)
		{
			pCI->m_dwCurMESH = (DWORD) m_cMESH.GetListCtrl()->GetItemData(pNMLV->iItem);
			MAPMESH::iterator finder = pCL->m_mapMESH.find(pCI->m_dwCurMESH);

			if( finder != pCL->m_mapMESH.end() )
			{
				LPMESH pMESH = (*finder).second;

				for( int i=0; i<pMESH->m_pMESH->m_dwMeshCount; i++)
				{
					CString strName;

					strName.Format( IDS_FMT_SUBMESH, i);
					pListCtrl->InsertItem( i, strName);
				}

				m_cMI.SetCurSel(m_bCurTEX);
			}

			SetCloth();
		}

		m_cMI.UpdateColumnWidth();
		EnableAllCtrl(TRUE);
	}

	*pResult = 0;
}

void COBJBar::OnLvnItemchangedListMeshorder( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		m_bCurTEX = pNMLV->iItem;
		LoadTEX();
	}

	*pResult = 0;
}

void COBJBar::OnNMDblclkListClothdata( NMHDR *pNMHDR, LRESULT *pResult)
{
	/*int nIndex = m_cMESH.GetCurSel();

	if( nIndex == -1 )
	{
		LPCLKINST pCI = GetCurCI();

		if(pCI)
		{
			CListCtrl *pListCtrl = m_cMESH.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pCI->m_dwCurMESH == pListCtrl->GetItemData(i) )
				{
					m_cMESH.SetCurSel(i);
					break;
				}
		}
	}
	else
		DeleteMESH(nIndex);*/

	*pResult = 0;
}

void COBJBar::OnNMDblclkListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cANI.GetCurSel();

	if( nIndex == -1 )
	{
		LPACTINST pAI = GetCurAI();

		if(pAI)
		{
			CListCtrl *pListCtrl = m_cANI.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pAI->m_dwCurANI == pListCtrl->GetItemData(i) )
				{
					m_cANI.SetCurSel(i);
					break;
				}
		}
	}
//	else
//		DeleteANI(nIndex);

	*pResult = 0;
}

void COBJBar::OnNMRDblclkListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cANI.GetCurSel();

	DeleteANI(nIndex);

	*pResult = 0;
}

void COBJBar::OnNMClickListAction( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cACT.GetCurSel();

	if( nIndex == -1 )
	{
		CListCtrl *pListCtrl = m_cACT.GetListCtrl();

		for( int i=0; i<pListCtrl->GetItemCount(); i++)
			if( m_pItem->m_TOBJ.m_OBJ.m_dwCurACT == pListCtrl->GetItemData(i) )
			{
				m_cACT.SetCurSel(i);
				break;
			}
	}

	*pResult = 0;
}

void COBJBar::OnNMClickListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cANI.GetCurSel();

	if( nIndex == -1 )
	{
		LPACTINST pAI = GetCurAI();

		if(pAI)
		{
			CListCtrl *pListCtrl = m_cANI.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pAI->m_dwCurANI == pListCtrl->GetItemData(i) )
				{
					m_cANI.SetCurSel(i);
					break;
				}
		}
	}

	*pResult = 0;
}

void COBJBar::OnNMClickListMeshorder( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cMI.GetCurSel();

	if( nIndex == -1 )
		m_cMI.SetCurSel(m_bCurTEX);

	*pResult = 0;
}

void COBJBar::OnNMClickListCloth( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cCL.GetCurSel();

	if( nIndex == -1 )
	{
		LPCLKINST pCI = GetCurCI();

		if(pCI)
		{
			CListCtrl *pListCtrl = m_cCL.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pCI->m_dwCurCL == pListCtrl->GetItemData(i) )
				{
					m_cCL.SetCurSel(i);
					break;
				}
		}
	}

	*pResult = 0;
}

void COBJBar::OnNMClickListClothdata( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cMESH.GetCurSel();

	if( nIndex == -1 )
	{
		LPCLKINST pCI = GetCurCI();

		if(pCI)
		{
			CListCtrl *pListCtrl = m_cMESH.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pCI->m_dwCurMESH == pListCtrl->GetItemData(i) )
				{
					m_cMESH.SetCurSel(i);
					break;
				}
		}
	}

	*pResult = 0;
}

void COBJBar::OnNMClickListClothkind( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cCLK.GetCurSel();

	if( nIndex == -1 )
	{
		CListCtrl *pListCtrl = m_cCLK.GetListCtrl();

		for( int i=0; i<pListCtrl->GetItemCount(); i++)
			if( m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK == pListCtrl->GetItemData(i) )
			{
				m_cCLK.SetCurSel(i);
				break;
			}
	}

	*pResult = 0;
}

void COBJBar::OnNMClickListEck( NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_cEOCLK.GetCurSel();

	if( nIndex == -1 )
	{
		CTachyonObject *pOBJ = GetCurEO();

		if(pOBJ)
		{
			CListCtrl *pListCtrl = m_cEOCLK.GetListCtrl();

			for( int i=0; i<pListCtrl->GetItemCount(); i++)
				if( pOBJ->m_OBJ.m_dwCurCLK == pListCtrl->GetItemData(i) )
				{
					m_cEOCLK.SetCurSel(i);
					break;
				}
		}
	}

	*pResult = 0;
}

void CALLBACK COBJBar::OnMESHDragLeave( CGSListCtrl *pList)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	LPCLKINST pCI = pBar->GetCurCI();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	if(pCI)
	{
		CListCtrl *pListCtrl = pBar->m_cMESH.GetListCtrl();

		for( int i=0; i<pListCtrl->GetItemCount(); i++)
			if( pCI->m_dwCurMESH == (DWORD) pListCtrl->GetItemData(i) )
			{
				pBar->m_cMESH.SetCurSel(i);
				break;
			}
	}

	pList->CListView::OnDragLeave();
}

void CALLBACK COBJBar::OnSFXDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

void CALLBACK COBJBar::OnSNDDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

void CALLBACK COBJBar::OnANIDragLeave( CGSListCtrl *pList)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	LPACTINST pAI = pBar->GetCurAI();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	if(pAI)
	{
		CListCtrl *pListCtrl = pBar->m_cANI.GetListCtrl();

		for( int i=0; i<pListCtrl->GetItemCount(); i++)
			if( pAI->m_dwCurANI == (DWORD) pListCtrl->GetItemData(i) )
			{
				pBar->m_cANI.SetCurSel(i);
				break;
			}
	}

	pList->CListView::OnDragLeave();
}

void CALLBACK COBJBar::OnEODragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

BOOL COBJBar::CanMESHDrop( COBJBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_MESH && pBar->GetCurCL();
}

BOOL COBJBar::CanSFXDrop( COBJBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_SFX;
}

BOOL COBJBar::CanSNDDrop( COBJBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_MEDIA && ((CMediaItem *) pItem)->GetPlayerType() == MEDIA_TSOUND;
}

BOOL COBJBar::CanANIDrop( COBJBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_ANIMATION && pBar->GetCurACT();
}

BOOL COBJBar::CanEODrop( COBJBar *pBar, CItemBase *pItem)
{
	return pItem != pBar->m_pItem && pItem->m_bType == ITEM_OBJECT && pBar->m_pItem->GetPivotCount() > 0;
}

DROPEFFECT CALLBACK COBJBar::OnMESHDragOver( CGSListCtrl *pList,
											 COleDataObject *pDataObject,
											 DWORD dwKeyState,
											 CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanMESHDrop( pBar, pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pList->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

DROPEFFECT CALLBACK COBJBar::OnSFXDragOver( CGSListCtrl *pList,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSFXDrop( pBar, pItem))
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

DROPEFFECT CALLBACK COBJBar::OnSNDDragOver( CGSListCtrl *pList,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSNDDrop( pBar, pItem))
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

DROPEFFECT CALLBACK COBJBar::OnANIDragOver( CGSListCtrl *pList,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop( pBar, pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pList->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

DROPEFFECT CALLBACK COBJBar::OnEODragOver( CGSEditCtrl *pEdit,
										   COleDataObject *pDataObject,
										   DWORD dwKeyState,
										   CPoint point)
{
	COBJBar *pBar = (COBJBar *) pEdit->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanEODrop( pBar, pItem))
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

BOOL CALLBACK COBJBar::OnMESHDrop( CGSListCtrl *pList,
								   COleDataObject* pDataObject,
								   DROPEFFECT dropEffect,
								   CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanMESHDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertMeshItem( (CMeshItem *) pItem, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnMESHDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK COBJBar::OnSFXDrop( CGSListCtrl *pList,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSFXDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertSFX((CSFXItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnSFXDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK COBJBar::OnSNDDrop( CGSListCtrl *pList,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSNDDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertSND((CMediaItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnSNDDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK COBJBar::OnANIDrop( CGSListCtrl *pList,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	COBJBar *pBar = (COBJBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertAniItem( (CAnimationItem *) pItem, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnANIDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK COBJBar::OnEODrop( CGSEditCtrl *pEdit,
								 COleDataObject* pDataObject,
								 DROPEFFECT dropEffect,
								 CPoint point)
{
	COBJBar *pBar = (COBJBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanEODrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertEO((COBJItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnEODragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

CString COBJBar::GetNewName( DWORD dwFMT)
{
	CString strResult;
	DWORD dwIndex = 0;

	do strResult.Format( dwFMT, dwIndex++);
	while(!m_pItem->CheckDupLocalID( _T(""), strResult));

	return strResult;
}

void COBJBar::DeleteMESH( int nIndex)
{
	CListCtrl *pListCtrl = m_cMESH.GetListCtrl();
	DWORD dwID = (DWORD) pListCtrl->GetItemData(nIndex);
	LPCLOTH pCL = GetCurCL();

	MAPMESH::iterator finder = pCL->m_mapMESH.find(dwID);
	delete (*finder).second;

	m_cMI.GetListCtrl()->DeleteAllItems();
	pListCtrl->DeleteItem(nIndex);
	pCL->m_mapMESH.erase(finder);

	MAPTEXINDEX::iterator itTI = m_pItem->m_mapTI.begin();
	while(itTI != m_pItem->m_mapTI.end())
	{
		LPTEXINDEX pTI = (*itTI).second;

		if( pTI->m_dwMeshID == dwID )
		{
			MAPDWORD::iterator itTEX = m_pItem->m_mapTEX.find((*itTI).first);
			MAPTEXINDEX::iterator itNEXT = itTI;
			itNEXT++;

			m_pItem->m_mapTEX.erase(itTEX);
			m_pItem->m_mapTI.erase(itTI);
			delete pTI;

			itTI = itNEXT;
		}
		else
			itTI++;
	}

	itTI = m_pItem->m_mapCOLOR.begin();
	while(itTI != m_pItem->m_mapCOLOR.end())
	{
		LPTEXINDEX pTI = (*itTI).second;

		if( pTI->m_dwMeshID == dwID )
		{
			MAPSTR::iterator itCLR = m_pItem->m_mapNAME.find((*itTI).first);
			MAPTEXINDEX::iterator itNEXT = itTI;
			itNEXT++;

			CItemBase::ReleaseLocalID( (*itCLR).second, m_pItem);
			m_pItem->m_mapNAME.erase(itCLR);
			m_pItem->m_mapCOLOR.erase(itTI);
			delete pTI;

			itTI = itNEXT;
		}
		else
			itTI++;
	}

	MAPDWORD::iterator itMESH = m_pItem->m_mapMESH.find(dwID);
	MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwID);

	if( itMESH != m_pItem->m_mapMESH.end() )
	{
		CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);

		m_pItem->m_mapMESH.erase(itMESH);
		m_pItem->m_mapNAME.erase(itSTR);
	}

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->Regulate();
	pDoc->UpdateAll();

	if( m_pItem->GetPivotCount() == -1 )
	{
		MAPSTR::iterator itSTR;

		for( itSTR = m_pItem->m_mapPIVOT.begin(); itSTR != m_pItem->m_mapPIVOT.end(); itSTR++)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
		pDoc->m_mapEQUIP.clear();

		m_pItem->m_TOBJ.ClearEquip();
		m_pItem->m_mapPIVOT.clear();
		m_cPIVOT.ResetContent();
	}

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();

	EnableAllCtrl(TRUE);
}

void COBJBar::DeleteANI( int nIndex)
{
	CListCtrl *pListCtrl = m_cANI.GetListCtrl();
	DWORD dwID = (DWORD) pListCtrl->GetItemData(nIndex);
	LPACTION pACT = GetCurACT();

	MAPANI::iterator itANI = pACT->m_mapANI.find(dwID);
	pListCtrl->DeleteItem(nIndex);
	delete (*itANI).second;
	pACT->m_mapANI.erase(itANI);

	MAPDWORD::iterator itANIID = m_pItem->m_mapANI.find(dwID);
	MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwID);

	if( itANIID != m_pItem->m_mapANI.end() )
	{
		CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);

		m_pItem->m_mapNAME.erase(itSTR);
		m_pItem->m_mapANI.erase(itANIID);
	}

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->Regulate();
	pDoc->UpdateAll();

	if( m_pItem->GetPivotCount() == -1 )
	{
		MAPSTR::iterator itSTR;

		for( itSTR = m_pItem->m_mapPIVOT.begin(); itSTR != m_pItem->m_mapPIVOT.end(); itSTR++)
			CItemBase::ReleaseLocalID( (*itSTR).second, m_pItem);
		pDoc->m_mapEQUIP.clear();

		m_pItem->m_TOBJ.ClearEquip();
		m_pItem->m_mapPIVOT.clear();
		m_cPIVOT.ResetContent();
	}

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();

	EnableAllCtrl(TRUE);
}

void COBJBar::DeleteCurSFX()
{
	CListCtrl *pList = m_cSFX.GetListCtrl();

	int nIndex = m_cSFX.GetCurSel();
	if( nIndex != -1 )
	{
		DWORD dwID = (DWORD) pList->GetItemData(nIndex);
		LPMAPSFXINST pOBJINST = NULL;
		LPMAPSFXINST pINST = NULL;

		m_pItem->m_TOBJ.RemoveOBJSFX();
		switch(m_nSFXKind)
		{
		case OEK_OBJ	:
			{
				pOBJINST = &m_pItem->m_TOBJ.m_mapOBJSFXINST;
				pINST = &m_pItem->m_OBJ.m_mapSFX;
			}

			break;

		case OEK_ACT	:
			{
				LPACTION pACT = GetCurACT();

				pOBJINST = &m_pItem->m_TOBJ.m_mapACTSFXINST;
				if(pACT)
					pINST = &pACT->m_mapSFX;
			}

			break;

		case OEK_ANI	:
			{
				LPACTINST pAI = GetCurAI();

				pOBJINST = &m_pItem->m_TOBJ.m_mapANISFXINST;
				if( pAI && pAI->m_pANI )
					pINST = &pAI->m_pANI->m_mapSFX;
			}

			break;
		}

		if(pOBJINST)
		{
			MAPSFXINST::iterator finder = pOBJINST->find(dwID);

			if( finder != pOBJINST->end() )
				pOBJINST->erase(finder);
		}

		if(pINST)
		{
			MAPSFXINST::iterator finder = pINST->find(dwID);
			if( finder != pINST->end() )
			{
				MAPDWORD::iterator itID = m_pItem->m_mapSFX.find(dwID);

				delete (*finder).second;
				pINST->erase(finder);

				m_pItem->m_mapSFX.erase(itID);
				pList->DeleteItem(nIndex);
			}
		}
	}

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	pDoc->RemoveSFX();
	SetCurSFX();

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::InsertSFX( CSFXItem *pItem)
{
	LPMAPSFXINST pOBJINST = NULL;
	LPMAPSFXINST pINST = NULL;

	switch(m_nSFXKind)
	{
	case OEK_OBJ	:
		{
			pOBJINST = &m_pItem->m_TOBJ.m_mapOBJSFXINST;
			pINST = &m_pItem->m_OBJ.m_mapSFX;
		}

		break;

	case OEK_ACT	:
		{
			LPACTION pACT = GetCurACT();

			pOBJINST = &m_pItem->m_TOBJ.m_mapACTSFXINST;
			pINST = &pACT->m_mapSFX;
		}

		break;

	case OEK_ANI	:
		{
			LPACTINST pAI = GetCurAI();

			pOBJINST = &m_pItem->m_TOBJ.m_mapANISFXINST;
			pINST = &pAI->m_pANI->m_mapSFX;
		}

		break;
	}

	if(pINST)
	{
		LPSFXINST pSFXINST = new SFXINST();
		DWORD dwInstID = 1;

		while(m_pItem->m_mapSFX.find(dwInstID) != m_pItem->m_mapSFX.end())
			dwInstID++;
		pSFXINST->m_pSFX = &pItem->m_SFX;

		m_pItem->m_mapSFX.insert( MAPDWORD::value_type(
			dwInstID,
			pItem->m_dwID));

		pOBJINST->insert( MAPSFXINST::value_type(
			dwInstID,
			pSFXINST));

		pINST->insert( MAPSFXINST::value_type(
			dwInstID,
			pSFXINST));

		int nIndex = m_cSFX.GetListCtrl()->InsertItem(
			m_cSFX.GetListCtrl()->GetItemCount(),
			pItem->m_strName);
		m_cSFX.GetListCtrl()->SetItemData( nIndex, dwInstID);
	}
}

void COBJBar::InsertEO( COBJItem *pItem)
{
	if( m_nPIVOT != -1 && m_pItem->GetPivotCount() > 0 )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		MAPOBJECT::iterator itOBJ = m_pItem->m_TOBJ.m_mapEQUIP.find(m_nPIVOT + 1);
		MAPDWORD::iterator itEO = pDoc->m_mapEQUIP.find(m_nPIVOT + 1);

		if( itEO != pDoc->m_mapEQUIP.end() )
			pDoc->m_mapEQUIP.erase(itEO);

		if( itOBJ != m_pItem->m_TOBJ.m_mapEQUIP.end() )
		{
			CTachyonObject *pOBJ = (*itOBJ).second;

			if(pOBJ)
				delete pOBJ;

			m_pItem->m_TOBJ.m_mapEQUIP.erase(itOBJ);
		}

		CTachyonObject *pOBJ = new CTachyonObject();
		pOBJ->m_pHost = m_pItem->m_TOBJ.m_pHost;
		pOBJ->InitOBJ(&pItem->m_OBJ);

		m_pItem->m_TOBJ.m_mapEQUIP.insert( MAPOBJECT::value_type( m_nPIVOT + 1, pOBJ));
		pDoc->m_mapEQUIP.insert( MAPDWORD::value_type( m_nPIVOT + 1, pItem->m_dwID));

		SetEOACTList();
		SetEOCLKList();
	}
}

void COBJBar::OnBnClickedButtonDeleo()
{
	if( m_nPIVOT != -1 && m_pItem->GetPivotCount() > 0 )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		MAPOBJECT::iterator itOBJ = m_pItem->m_TOBJ.m_mapEQUIP.find(m_nPIVOT + 1);
		MAPDWORD::iterator itEO = pDoc->m_mapEQUIP.find(m_nPIVOT + 1);

		if( itEO != pDoc->m_mapEQUIP.end() )
			pDoc->m_mapEQUIP.erase(itEO);

		if( itOBJ != m_pItem->m_TOBJ.m_mapEQUIP.end() )
		{
			CTachyonObject *pOBJ = (*itOBJ).second;

			if(pOBJ)
				delete pOBJ;

			m_pItem->m_TOBJ.m_mapEQUIP.erase(itOBJ);
		}

		SetEOACTList();
		SetEOCLKList();
	}
}

void COBJBar::InsertMeshItem( CMeshItem *pItem, int nIndex)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CListCtrl *pListCtrl = m_cMESH.GetListCtrl();
	LPCLKINST pCI = GetCurCI();
	LPCLOTH pCL = GetCurCL();

	int nPivotCnt = m_pItem->GetPivotCount();

	if( nPivotCnt != -1 && nPivotCnt != pItem->m_Mesh.m_dwNodeCount )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_FORMAT);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		return;
	}

	if( nIndex == -1 )
		nIndex = pListCtrl->GetItemCount();

	if(pCL)
	{
		LPMESH pMESH = new MESH();

		CString strName = GetNewName(IDS_FMT_MESH);
		DWORD dwID = ROOT_MESHID;

		while( m_pItem->m_mapNAME.find(dwID) != m_pItem->m_mapNAME.end() )
			dwID++;
		CItemBase::RegLocalID( strName, m_pItem);

		m_pItem->m_mapMESH.insert( MAPDWORD::value_type( dwID, pItem->m_dwID));
		m_pItem->m_mapNAME.insert( MAPSTR::value_type( dwID, strName));
		pCL->m_mapMESH.insert( MAPMESH::value_type( dwID, pMESH));

		nIndex = pListCtrl->InsertItem( nIndex, strName);
		pListCtrl->SetItemData( nIndex, dwID);

		pMESH->m_pMESH = &pItem->m_Mesh;
		pMESH->m_vTEX.clear();
		m_bCurTEX = 0;

		for( int i=0; i<pItem->m_Mesh.m_dwMeshCount; i++)
		{
			LPOBJTEX pTEX = new OBJTEX();

			pMESH->m_vTEX.push_back(pTEX);
			pTEX->m_dwOrder = i;
		}

		m_cMI.SetCurSel(m_bCurTEX);
		m_cMI.UpdateColumnWidth();

		pCI->m_dwCurMESH = dwID;
		if( nPivotCnt == -1 )
			LoadPivot();
		EnableAllCtrl(TRUE);
	}

	pDoc->UpdateAll();
}

void COBJBar::InsertAniItem( CAnimationItem *pItem, int nIndex)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CListCtrl *pListCtrl = m_cANI.GetListCtrl();
	LPACTION pACT = GetCurACT();
	LPACTINST pAI = GetCurAI();

	int nPivotCnt = m_pItem->GetPivotCount();

	if( nPivotCnt != -1 && nPivotCnt != m_pItem->GetPivotCount(&pItem->m_Aniset) )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_FORMAT);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		return;
	}

	if( nIndex == -1 )
		nIndex = pListCtrl->GetItemCount();

	if(pACT)
	{
		CString strName = GetNewName(IDS_FMT_ANI);
		DWORD dwID = ROOT_ANIID;
		LPANI pANI = new ANI();

		while( m_pItem->m_mapNAME.find(dwID) != m_pItem->m_mapNAME.end() )
			dwID++;
		CItemBase::RegLocalID( strName, m_pItem);

		m_pItem->m_mapANI.insert( MAPDWORD::value_type( dwID, pItem->m_dwID));
		m_pItem->m_mapNAME.insert( MAPSTR::value_type( dwID, strName));
		pANI->m_pANI = &pItem->m_Aniset;
		pACT->m_mapANI.insert( MAPANI::value_type( dwID, pANI));

		nIndex = pListCtrl->InsertItem( nIndex, strName);
		pListCtrl->SetItemData( nIndex, dwID);

		m_cANI.UpdateColumnWidth();
		pAI->m_dwCurANI = dwID;

		if( nPivotCnt == -1 )
			LoadPivot();
		EnableAllCtrl(TRUE);
	}

	pDoc->UpdateAll();
}

void COBJBar::SetAction()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	LPACTINST pAI = GetCurAI();

	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	BOOL bIsEmptyOBJ = IsEmptyOBJ();

	if( pDoc && pAI )
	{
		DWORD dwAction = m_pItem->m_TOBJ.m_OBJ.m_dwCurACT;
		DWORD dwAni = pAI->m_dwCurANI;

		m_pItem->m_TOBJ.m_OBJ.m_dwCurACT = 0;
		pAI->m_dwCurANI = 0;

		m_pItem->m_TOBJ.SetAction(
			dwAction,
			dwAni);
		pCtrl->EnableCtrl(pAI->m_dwCurANI != 0);

		if( pAI->m_dwCurANI != 0 )
		{
			MAPDWORD::iterator finder = m_pItem->m_mapANI.find(pAI->m_dwCurANI);

			if( finder != m_pItem->m_mapANI.end() )
			{
				CAnimationItem *pANI = CItemBase::GetAnimationItem((*finder).second);

				if(pANI)
					pCtrl->SetRange( 0, pANI->m_dwTotalFrame);
				else
					pCtrl->SetRange( 0, 1);
			}
			else
				pCtrl->SetRange( 0, 1);
		}
		else
			pCtrl->SetRange( 0, 1);
	}
	else
	{
		pCtrl->EnableCtrl(FALSE);
		pCtrl->SetRange( 0, 1);
	}

	if( bIsEmptyOBJ != IsEmptyOBJ() )
		pDoc->ResetCamera();

	m_pItem->m_TOBJ.RemoveOBJSFX();
	m_pItem->m_TOBJ.ClearOBJSND();
	pDoc->RemoveSFX();

	pCtrl->m_cPlayCtrl.SetPos(0);
	UpdatePos(0);
}

void COBJBar::SetCloth()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	LPCLKINST pCI = GetCurCI();

	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	BOOL bIsEmptyOBJ = IsEmptyOBJ();

	if( pDoc && pCI )
	{
		m_pItem->m_TOBJ.SetCloth(
			&CMainFrame::m_3DDevice,
			m_pItem->m_TOBJ.m_OBJ.m_dwCurCLK,
			pCI->m_dwCurCL,
			pCI->m_dwCurMESH);
		m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
	}

	if( bIsEmptyOBJ != IsEmptyOBJ() )
		pDoc->ResetCamera();
	else if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::LoadPivot()
{
	int nCount = m_pItem->GetPivotCount();

	m_cEPivotID.ResetContent();
	m_cPIVOT.ResetContent();

	m_cEPivotID.InsertString( 0, "PIVOT_NONE");
	for( int i=0; i<nCount; i++)
	{
		CString strID;

		strID.Format( IDS_FMT_PIVOT, i);
		m_cEPivotID.InsertString( i + 1, strID);
		m_cPIVOT.InsertString( i, strID);

		if( m_nPIVOT == i )
			m_cPIVOT.SetCurSel(i);
	}

	if( m_cPIVOT.GetCurSel() == -1 )
		m_nPIVOT = 0;

	UpdateData(FALSE);
	OnCbnSelchangeComboPivot();
}

void COBJBar::LoadTEX()
{
	LPMESH pMESH = GetCurMESH();
	LPCLKINST pCI = GetCurCI();

	if( pMESH && pCI && pCI->m_dwCurMESH )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		for( int i=0; i<OPCOUNT; i++)
			if( pTEX->m_dwOP == g_dwOP[i] )
			{
				m_nOP = i;
				break;
			}

		m_cDESTBlend.SetCurSel(g_nBLEND[pTEX->m_dwDESTBlend]);
		m_cSRCBlend.SetCurSel(g_nBLEND[pTEX->m_dwSRCBlend]);
		m_cBlendOP.SetCurSel(g_nBLENDOP[pTEX->m_dwBlendOP]);
		m_cUseDirLight.SetCheck(pTEX->m_bUseDirLight);

		DWORD dwColorID = m_pItem->GetColorID(
			pCI->m_dwCurMESH,
			m_bCurTEX);
		MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwColorID);

		if( itSTR == m_pItem->m_mapNAME.end() )
		{
			m_strColorID.LoadString(IDS_NOTUSE);
			m_strColorID.MakeUpper();
		}
		else
			m_strColorID = (*itSTR).second;

		m_strIntensity.Format( "%.2f", pTEX->m_fIntensity);
		m_bMinAlpha = pTEX->m_bMinAlpha;
		m_bZEnable = pTEX->m_bZEnable;
		m_bZWrite = pTEX->m_bZWrite;
		m_dwORDER = pTEX->m_dwOrder;
		m_b2side = pTEX->m_b2side;
		m_nTEX = pTEX->m_bType[m_nSTG];

		m_dwRValue = GETVALUE_R(pTEX->m_dwCOLOR);
		m_dwGValue = GETVALUE_G(pTEX->m_dwCOLOR);
		m_dwBValue = GETVALUE_B(pTEX->m_dwCOLOR);
		m_dwAlpha = GETVALUE_A(pTEX->m_dwCOLOR) * 100 / 255;

		m_cColorBox.SetColor( RGB(
			m_dwRValue,
			m_dwGValue,
			m_dwBValue));

		m_cColorCtrl.SetupColor( RGB(
			m_dwRValue,
			m_dwGValue,
			m_dwBValue),
			FALSE);

		m_dwRValue_Amb = GETVALUE_R( pTEX->m_dwAmbientCOLOR );
		m_dwGValue_Amb = GETVALUE_G( pTEX->m_dwAmbientCOLOR );
		m_dwBValue_Amb = GETVALUE_B( pTEX->m_dwAmbientCOLOR );
		m_dwAlpha_Amb = GETVALUE_A(pTEX->m_dwCOLOR) * 100 / 255;

		m_cAmbientColorBox.SetColor( RGB(
			m_dwRValue_Amb,
			m_dwGValue_Amb,
			m_dwBValue_Amb ) );

		m_cAmbientControlCtrl.SetupColor( RGB(
			m_dwRValue_Amb,
			m_dwGValue_Amb,
			m_dwBValue_Amb ),
			FALSE );

		UpdateData(FALSE);
		EnableAllCtrl(!pCtrl->m_bPlay);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnBnClickedRadioStage()
{
	UpdateData();
	LoadTEX();
}

void COBJBar::OnBnClickedRadioMitex()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwCOLOR = D3DCOLOR_ARGB(
			m_dwAlpha * 255 / 100,
			m_dwRValue,
			m_dwGValue,
			m_dwBValue);
		pTEX->m_bType[m_nSTG] = m_nTEX;
		EnableAllCtrl(!pCtrl->m_bPlay);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditMeshorder()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwOrder = m_dwORDER;
		m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditMinalpha()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_bMinAlpha = m_bMinAlpha;
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditIntensity()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_fIntensity = (FLOAT) atof(m_strIntensity);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnCbnSelchangeComboSop()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwOP = g_dwOP[m_nOP];
		pDoc->m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

int COBJBar::GetPosFromTime( FLOAT fTime)
{
	LPACTINST pAI = GetCurAI();

	if( pAI && pAI->m_dwCurANI != 0 )
	{
		MAPDWORD::iterator finder = m_pItem->m_mapANI.find(pAI->m_dwCurANI);

		if( finder != m_pItem->m_mapANI.end() )
		{
			CAnimationItem *pANI = CItemBase::GetAnimationItem((*finder).second);

			if(pANI)
			{
				FLOAT fPos = fTime * ((FLOAT) pANI->m_dwTotalFrame) / pANI->m_Aniset.GetTotalTime();
				return fPos == ((FLOAT) INT(fPos)) ? INT(fPos) : INT(fPos) + 1;
			}
		}
	}

	return 0;
}

FLOAT COBJBar::GetTimeFromPos( int nPos)
{
	LPACTINST pAI = GetCurAI();

	if( pAI && pAI->m_dwCurANI != 0 )
	{
		MAPDWORD::iterator finder = m_pItem->m_mapANI.find(pAI->m_dwCurANI);

		if( finder != m_pItem->m_mapANI.end() )
		{
			CAnimationItem *pANI = CItemBase::GetAnimationItem((*finder).second);

			if(pANI)
				return pANI->m_dwTotalFrame ? ((FLOAT) nPos) * pANI->m_Aniset.GetTotalTime() / ((FLOAT) pANI->m_dwTotalFrame) : 0.0f;
		}
	}

	return 0.0f;
}

LRESULT COBJBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	DWORD dwTick = (DWORD) wParam;

	if( dwTick > m_dwTick )
		dwTick -= m_dwTick;
	else
		dwTick = 0;
	m_dwTick = (DWORD) wParam;

	m_pItem->m_TOBJ.CalcTick( CMainFrame::m_3DDevice.m_pDevice, dwTick);
	pCtrl->m_cPlayCtrl.SetPos(GetPosFromTime(m_pItem->m_TOBJ.m_fActTime));
	pDoc->CalcSFX(dwTick);
	pDoc->RedrawAllView();

	return 0;
}

LRESULT COBJBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	CAnimationItem *pANI = NULL;
	LPACTINST pAI = GetCurAI();

	if( pAI && pAI->m_dwCurANI != 0 )
	{
		MAPDWORD::iterator finder = m_pItem->m_mapANI.find(pAI->m_dwCurANI);

		if( finder != m_pItem->m_mapANI.end() )
			pANI = CItemBase::GetAnimationItem((*finder).second);
	}

	if( pANI && pCtrl->m_cPlayCtrl.GetPos() >= pANI->m_dwTotalFrame )
	{
		m_pItem->m_TOBJ.m_fActTime = pANI->m_Aniset.GetTotalTime();
		m_pItem->m_TOBJ.CalcFrame(TRUE);

		m_pItem->m_TOBJ.m_dwTexTick = 0;
		m_pItem->m_TOBJ.m_fActTime = 0;
		m_dwTick = 0;
	}
	else
	{
		m_dwTick = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);
		pCtrl->m_dwBeginTick -= m_dwTick;
	}
	EnableAllCtrl(FALSE);

	return 0;
}

LRESULT COBJBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	UpdatePos(GetPosFromTime(m_pItem->m_TOBJ.m_fActTime));
	EnableAllCtrl(TRUE);

	return 0;
}

LRESULT COBJBar::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();

	m_pItem->m_TOBJ.RemoveOBJSFX();
	pDoc->RemoveSFX();

	UpdatePos(INT(lParam));
	return 0;
}

LRESULT COBJBar::OnOBJMessage( WPARAM wParam, LPARAM lParam)
{
	LPOBJMSG pMSG = (LPOBJMSG) wParam;

	switch(pMSG->m_dwMSG)
	{
	case OM_ACTENDING	:
		if( pMSG->m_pOBJ == (DWORD_PTR) &m_pItem->m_TOBJ )
		{
			COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

			if(!pCtrl->m_bLoop)
			{
				LPACTINST pAI = GetCurAI();

				m_pItem->m_TOBJ.m_fActTime = pAI->m_pANI->m_pANI->GetTotalTime();
				m_pItem->m_TOBJ.m_dwTexTick = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);

				m_pItem->m_TOBJ.ClearOBJSFX();
				m_pItem->m_TOBJ.ClearOBJSND();
				pCtrl->OnBnClickedButtonStop();

				return 1;
			}
		}

		break;

	case OM_ACTENDED	:
		if( pMSG->m_pOBJ == (DWORD_PTR) &m_pItem->m_TOBJ )
		{
			COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

			if(pCtrl->m_bLoop)
			{
				m_dwTick = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);
				pCtrl->m_dwBeginTick = pCtrl->m_dwTickCount - m_dwTick;
			}
		}

		break;
	}

	return 0;
}

void COBJBar::UpdatePos( int nPos)
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->m_TOBJ.m_fActTime = GetTimeFromPos(nPos);
	m_pItem->m_TOBJ.m_dwTexTick = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);
	m_pItem->m_TOBJ.CalcFrame(FALSE);

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

BOOL COBJBar::IsEmptyOBJ()
{
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	return m_pItem->GetSize( vMin, vMax);
}

void COBJBar::SetCurSel( DWORD dwCLK,
						 DWORD dwCL,
						 DWORD dwMESH,
						 int nIndex)
{
	CListCtrl *pListCtrl = m_cCLK.GetListCtrl();

	for( int i=0; i<pListCtrl->GetItemCount(); i++)
	{
		DWORD dwID = (DWORD) pListCtrl->GetItemData(i);

		if( dwCLK == dwID )
		{
			m_cCLK.SetCurSel(i);
			break;
		}
	}

	pListCtrl = m_cCL.GetListCtrl();
	for( i=0; i<pListCtrl->GetItemCount(); i++)
	{
		DWORD dwID = (DWORD) pListCtrl->GetItemData(i);

		if( dwCL == dwID )
		{
			m_cCL.SetCurSel(i);
			break;
		}
	}

	pListCtrl = m_cMESH.GetListCtrl();
	for( i=0; i<pListCtrl->GetItemCount(); i++)
	{
		DWORD dwID = (DWORD) pListCtrl->GetItemData(i);

		if( dwMESH == dwID )
		{
			m_cMESH.SetCurSel(i);
			break;
		}
	}

	m_cMI.SetCurSel(nIndex);
}

void COBJBar::OnBnClickedCheckShowSelected()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	UpdateData();

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::OnBnClickedCheckZenable()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_bZEnable = m_bZEnable;
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnBnClickedCheckZwrite()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_bZWrite = m_bZWrite;
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnBnClickedCheck2side()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_b2side = m_b2side;
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

LRESULT COBJBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		if( (HWND)wParam == m_cColorCtrl.GetSafeHwnd() )
		{
			m_dwRValue = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
			m_dwGValue = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
			m_dwBValue = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

			m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
			pTEX->m_dwCOLOR = D3DCOLOR_ARGB(
				m_dwAlpha * 255 / 100,
				m_dwRValue,
				m_dwGValue,
				m_dwBValue);
		}
		else if( (HWND)wParam == m_cAmbientControlCtrl.GetSafeHwnd() )
		{
			m_dwRValue_Amb = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
			m_dwGValue_Amb = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
			m_dwBValue_Amb = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

			m_cAmbientColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

			int nSel = m_cUseDirLightOp.GetCurSel();
			if( nSel == -1 ) return 0;
			if( nSel == 0 ) // 전체 메쉬에 적용.
			{
				VECTOROBJTEX::iterator itor = pMESH->m_vTEX.begin();
				VECTOROBJTEX::iterator end = pMESH->m_vTEX.end();
				while( itor != end )
				{
					(*itor)->m_dwAmbientCOLOR = D3DCOLOR_ARGB(
						m_dwAlpha_Amb * 255 / 100,
						m_dwRValue_Amb,
						m_dwGValue_Amb,
						m_dwBValue_Amb );

					++itor;
				}
			}
			else if( nSel == 1 )	// 선택된 메쉬에만 적용.
			{
				pTEX->m_dwAmbientCOLOR = D3DCOLOR_ARGB(
					m_dwAlpha_Amb * 255 / 100,
					m_dwRValue_Amb,
					m_dwGValue_Amb,
					m_dwBValue_Amb );
			}
		}

		UpdateData(FALSE);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}

	return 0;
}

void COBJBar::OnEnChangeEditColor()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwCOLOR = D3DCOLOR_ARGB(
			m_dwAlpha * 255 / 100,
			m_dwRValue,
			m_dwGValue,
			m_dwBValue);

		m_cColorCtrl.SetupColor( RGB(
			m_dwRValue,
			m_dwGValue,
			m_dwBValue),
			TRUE);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditColor2()
{
	LPMESH pMESH = GetCurMESH();

	if( pMESH )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwAmbientCOLOR = D3DCOLOR_ARGB(
			m_dwAlpha_Amb * 255 / 100,
			m_dwRValue_Amb,
			m_dwGValue_Amb,
			m_dwBValue_Amb );

		m_cAmbientControlCtrl.SetupColor( RGB(
			m_dwRValue_Amb,
			m_dwGValue_Amb,
			m_dwBValue_Amb ),
			TRUE);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditColorid()
{
	LPMESH pMESH = GetCurMESH();
	LPCLKINST pCI = GetCurCI();

	if( pMESH && pCI && pCI->m_dwCurMESH )
	{
		UpdateData();

		if(!CItemBase::CheckID(m_strColorID))
		{
			DWORD dwID = m_pItem->GetColorID(
				pCI->m_dwCurMESH,
				m_bCurTEX);
			MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwID);

			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);

			if( itSTR != m_pItem->m_mapNAME.end() )
				m_strColorID = (*itSTR).second;
			else
				m_strColorID.Empty();
			UpdateData(FALSE);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
			m_cColorID.SetFocus();
		}
	}
}

void COBJBar::OnEnSetfocusEditColorid()
{
	LPMESH pMESH = GetCurMESH();
	LPCLKINST pCI = GetCurCI();

	if( pMESH && pCI && pCI->m_dwCurMESH )
	{
		DWORD dwID = m_pItem->GetColorID(
			pCI->m_dwCurMESH,
			m_bCurTEX);
		MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwID);

		if( itSTR != m_pItem->m_mapNAME.end() )
			m_strColorID = (*itSTR).second;
		else
			m_strColorID.Empty();
		UpdateData(FALSE);
	}
}

void COBJBar::OnEnKillfocusEditColorid()
{
	LPMESH pMESH = GetCurMESH();
	LPCLKINST pCI = GetCurCI();

	if( pMESH && pCI && pCI->m_dwCurMESH )
	{
		DWORD dwID = m_pItem->GetColorID(
			pCI->m_dwCurMESH,
			m_bCurTEX);
		MAPSTR::iterator itSTR = m_pItem->m_mapNAME.find(dwID);
		CString strID;

		if( itSTR != m_pItem->m_mapNAME.end() )
			strID = (*itSTR).second;
		else
			strID.Empty();

		if( m_strColorID == strID )
		{
			if(m_strColorID.IsEmpty())
			{
				m_strColorID.LoadString(IDS_NOTUSE);
				UpdateData(FALSE);
			}

			return;
		}

		if(m_strColorID.IsEmpty())
		{
			MAPTEXINDEX::iterator itTI = m_pItem->m_mapCOLOR.find(dwID);

			if( itTI != m_pItem->m_mapCOLOR.end() )
			{
				delete (*itTI).second;
				m_pItem->m_mapCOLOR.erase(itTI);
			}

			CItemBase::ReleaseLocalID( strID, m_pItem);
			m_pItem->m_mapNAME.erase(itSTR);

			m_strColorID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);
		}
		else if(!m_pItem->CheckDupLocalID( strID, m_strColorID))
		{
			m_strColorID = strID;
			if(m_strColorID.IsEmpty())
				m_strColorID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);

			PostMessage(WM_MESSAGEBOX);
		}
		else if(strID.IsEmpty())
		{
			LPTEXINDEX pTI = new TEXINDEX();
			dwID = ROOT_COLORID;

			while( m_pItem->m_mapNAME.find(dwID) != m_pItem->m_mapNAME.end() )
				dwID++;
			m_pItem->m_mapNAME.insert( MAPSTR::value_type( dwID, m_strColorID));
			CItemBase::RegLocalID( m_strColorID, m_pItem);

			pTI->m_dwMeshID = pCI->m_dwCurMESH;
			pTI->m_nIndex = m_bCurTEX;

			m_pItem->m_mapCOLOR.insert( MAPTEXINDEX::value_type( dwID, pTI));
		}
		else
		{
			CItemBase::ReleaseLocalID( strID, m_pItem);
			CItemBase::RegLocalID( m_strColorID, m_pItem);

			(*itSTR).second = m_strColorID;
		}
	}
}

void COBJBar::OnEnChangeEditPivot()
{
	MAPSTR::iterator finder = m_pItem->m_mapPIVOT.find((DWORD) m_nPIVOT);
	CString strID;

	if( finder != m_pItem->m_mapPIVOT.end() )
		strID = (*finder).second;
	else
		strID.Empty();

	UpdateData();
	if(!CItemBase::CheckID(m_strPivotID))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);

		m_strPivotID = strID;
		UpdateData(FALSE);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		m_cPivotID.SetFocus();
	}
}

void COBJBar::OnEnSetfocusEditPivot()
{
	MAPSTR::iterator finder = m_pItem->m_mapPIVOT.find((DWORD) m_nPIVOT);

	if( finder != m_pItem->m_mapPIVOT.end() )
		m_strPivotID = (*finder).second;
	else
		m_strPivotID.Empty();
	UpdateData(FALSE);
}

void COBJBar::OnEnKillfocusEditPivot()
{
	MAPSTR::iterator finder = m_pItem->m_mapPIVOT.find((DWORD) m_nPIVOT);
	CString strID;

	if( finder != m_pItem->m_mapPIVOT.end() )
		strID = (*finder).second;
	else
		strID.Empty();

	if( m_strPivotID == strID )
	{
		if(m_strPivotID.IsEmpty())
		{
			m_strPivotID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);
		}

		return;
	}

	if(m_strPivotID.IsEmpty())
	{
		CItemBase::ReleaseLocalID( strID, m_pItem);
		m_pItem->m_mapPIVOT.erase(finder);

		m_strPivotID.LoadString(IDS_NOTUSE);
		UpdateData(FALSE);
	}
	else if(!m_pItem->CheckDupLocalID( strID, m_strPivotID))
	{
		m_strPivotID = strID;
		if(m_strPivotID.IsEmpty())
			m_strPivotID.LoadString(IDS_NOTUSE);
		UpdateData(FALSE);

		PostMessage(WM_MESSAGEBOX);
	}
	else if(strID.IsEmpty())
	{
		m_pItem->m_mapPIVOT.insert( MAPSTR::value_type(
			(DWORD) m_nPIVOT,
			m_strPivotID));
		CItemBase::RegLocalID( m_strPivotID, m_pItem);
	}
	else
	{
		CItemBase::ReleaseLocalID( strID, m_pItem);
		CItemBase::RegLocalID( m_strPivotID, m_pItem);

		(*finder).second = m_strPivotID;
	}
}

void COBJBar::OnCbnSelchangeComboPivot()
{
	UpdateData();

	if(m_pItem)
	{
		MAPSTR::iterator finder = m_pItem->m_mapPIVOT.find((DWORD) m_nPIVOT);

		if( finder == m_pItem->m_mapPIVOT.end() )
		{
			m_strPivotID.LoadString(IDS_NOTUSE);
			m_strPivotID.MakeUpper();
		}
		else
			m_strPivotID = (*finder).second;

		UpdateData(FALSE);
		SetEOACTList();
		SetEOCLKList();
	}
}

void COBJBar::OnBnClickedCheckEditpivot()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	UpdateData();

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::SetEOACTList()
{
	if(m_pItem)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		CTachyonObject *pOBJ = GetCurEO();
		COBJItem *pEO = GetCurEI();

		m_cEOACT.ResetContent();
		m_cEOANI.ResetContent();
		if( pOBJ && pEO )
		{
			MAPACTION::iterator itACT;

			for( itACT = pEO->m_OBJ.m_mapACTION.begin(); itACT != pEO->m_OBJ.m_mapACTION.end(); itACT++)
			{
				MAPSTR::iterator itSTR = pEO->m_mapNAME.find((*itACT).first);

				if( itSTR != pEO->m_mapNAME.end() )
				{
					int nIndex = m_cEOACT.InsertString( m_cEOACT.GetCount(), (*itSTR).second);

					m_cEOACT.SetItemData( nIndex, (*itACT).first);
					if( (*itACT).first == pOBJ->m_OBJ.m_dwCurACT )
					{
						m_cEOACT.SetCurSel(pOBJ->m_OBJ.m_dwCurACT);
						OnCbnSelchangeComboEa();
					}
				}
			}

			if( m_cEOACT.GetCount() > 0 && m_cEOACT.GetCurSel() == -1 )
			{
				m_cEOACT.SetCurSel(0);
				OnCbnSelchangeComboEa();
			}
		}
	}
}

void COBJBar::OnCbnSelchangeComboEa()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	CTachyonObject *pOBJ = GetCurEO();
	COBJItem *pEO = GetCurEI();

	pOBJ->m_OBJ.m_dwCurACT = (DWORD) m_cEOACT.GetItemData(m_cEOACT.GetCurSel());
	LPACTION pACT = GetCurEACT();
	LPACTINST pAI = GetCurEAI();

	m_cEOANI.ResetContent();
	if( pEO && pACT )
	{
		MAPANI::iterator itANI;

		for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
		{
			MAPSTR::iterator itSTR = pEO->m_mapNAME.find((*itANI).first);

			if( itSTR != pEO->m_mapNAME.end() )
			{
				int nIndex = m_cEOANI.InsertString( m_cEOANI.GetCount(), (*itSTR).second);

				m_cEOANI.SetItemData( nIndex, (*itANI).first);
				if( (*itANI).first == pAI->m_dwCurANI )
				{
					m_cEOANI.SetCurSel(nIndex);
					OnCbnSelchangeComboEani();
				}
			}
		}

		if( m_cEOANI.GetCount() > 0 && m_cEOANI.GetCurSel() == -1 )
		{
			m_cEOANI.SetCurSel(0);
			OnCbnSelchangeComboEani();
		}

		pOBJ->SetAction( pOBJ->m_OBJ.m_dwCurACT, pAI->m_dwCurANI);
	}

	EnableAllCtrl(!pCtrl->m_bPlay);
	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::OnCbnSelchangeComboEani()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	CTachyonObject *pOBJ = GetCurEO();
	if(pOBJ)
	{
		DWORD dwCurANI = (DWORD) m_cEOANI.GetItemData(m_cEOANI.GetCurSel());
		pOBJ->SetAction( pOBJ->m_OBJ.m_dwCurACT, dwCurANI);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}

	EnableAllCtrl(!pCtrl->m_bPlay);
}

void COBJBar::SetEOCLKList()
{
	if(m_pItem)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
		CListCtrl *pListCtrl = m_cEOCLK.GetListCtrl();

		CTachyonObject *pOBJ = GetCurEO();
		COBJItem *pEO = GetCurEI();

		pListCtrl->DeleteAllItems();
		m_cEOCL.ResetContent();
		m_cEOMESH.ResetContent();

		if( pOBJ && pEO )
		{
			MAPCLKIND::iterator itCLK;

			for( itCLK = pEO->m_OBJ.m_mapCLKIND.begin(); itCLK != pEO->m_OBJ.m_mapCLKIND.end(); itCLK++)
			{
				MAPSTR::iterator itSTR = pEO->m_mapNAME.find((*itCLK).first);

				if( itSTR != pEO->m_mapNAME.end() )
				{
					int nIndex = pListCtrl->InsertItem( pListCtrl->GetItemCount(), (*itSTR).second);
					pListCtrl->SetItemData( nIndex, (*itCLK).first);

					if( (*itCLK).first == pOBJ->m_OBJ.m_dwCurCLK )
						m_cEOCLK.SetCurSel(nIndex);
				}
			}

			m_cEO.SetWindowText(pEO->m_strName);
		}
		else
			m_cEO.SetWindowText("");

		m_cEOCLK.UpdateColumnWidth();
		EnableAllCtrl(!pCtrl->m_bPlay);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnLvnItemchangedListEck( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		CTachyonObject *pOBJ = GetCurEO();
		COBJItem *pEO = GetCurEI();

		if(pOBJ)
			pOBJ->m_OBJ.m_dwCurCLK = (DWORD) m_cEOCLK.GetListCtrl()->GetItemData(pNMLV->iItem);

		LPCLKIND pCLK = GetCurECLK();
		LPCLKINST pCI = GetCurECI();

		m_cEOMESH.ResetContent();
		m_cEOCL.ResetContent();

		if( pEO && pCLK )
		{
			MAPCLOTH::iterator itCL;

			for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
			{
				MAPSTR::iterator itSTR = pEO->m_mapNAME.find((*itCL).first);

				if( itSTR != pEO->m_mapNAME.end() )
				{
					int nIndex = m_cEOCL.InsertString( m_cEOCL.GetCount(), (*itSTR).second);

					m_cEOCL.SetItemData( nIndex, (*itCL).first);
					if( (*itCL).first == pCI->m_dwCurCL )
					{
						m_cEOCL.SetCurSel(nIndex);
						OnCbnSelchangeComboEc();
					}
				}
			}

			if( m_cEOCL.GetCount() > 0 && m_cEOCL.GetCurSel() == -1 )
			{
				m_cEOCL.SetCurSel(0);
				OnCbnSelchangeComboEc();
			}

			pOBJ->SetCloth( &CMainFrame::m_3DDevice, pOBJ->m_OBJ.m_dwCurCLK, pCI->m_dwCurCL, pCI->m_dwCurMESH);
			pOBJ->ResetOBJPart(&CMainFrame::m_3DDevice);
		}

		EnableAllCtrl(!pCtrl->m_bPlay);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}

	*pResult = 0;
}

void COBJBar::OnCbnSelchangeComboEc()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	LPCLKINST pCI = GetCurECI();
	pCI->m_dwCurCL = (DWORD) m_cEOCL.GetItemData(m_cEOCL.GetCurSel());

	CTachyonObject *pOBJ = GetCurEO();
	COBJItem *pEO = GetCurEI();
	LPCLOTH pCL = GetCurECL();

	m_cEOMESH.ResetContent();
	if( pEO && pCL )
	{
		MAPMESH::iterator itMESH;

		for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
		{
			MAPSTR::iterator itSTR = pEO->m_mapNAME.find((*itMESH).first);

			if( itSTR != pEO->m_mapNAME.end() )
			{
				int nIndex = m_cEOMESH.InsertString( m_cEOMESH.GetCount(), (*itSTR).second);

				m_cEOMESH.SetItemData( nIndex, (*itMESH).first);
				if( pCI->m_dwCurMESH == (*itMESH).first )
				{
					m_cEOMESH.SetCurSel(nIndex);
					OnCbnSelchangeComboEcd();
				}
			}
		}

		if( m_cEOMESH.GetCount() > 0 && m_cEOMESH.GetCurSel() == -1 )
		{
			m_cEOMESH.SetCurSel(0);
			OnCbnSelchangeComboEcd();
		}

		pOBJ->SetCloth( &CMainFrame::m_3DDevice, pOBJ->m_OBJ.m_dwCurCLK, pCI->m_dwCurCL, pCI->m_dwCurMESH);
		pOBJ->ResetOBJPart(&CMainFrame::m_3DDevice);
	}

	EnableAllCtrl(!pCtrl->m_bPlay);
	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::OnCbnSelchangeComboEcd()
{
	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	CTachyonObject *pOBJ = GetCurEO();
	LPCLKINST pCI = GetCurECI();

	if(pOBJ)
	{
		DWORD dwCurMESH = (DWORD) m_cEOMESH.GetItemData(m_cEOMESH.GetCurSel());

		pOBJ->SetCloth(
			&CMainFrame::m_3DDevice,
			pOBJ->m_OBJ.m_dwCurCLK,
			pCI->m_dwCurCL,
			dwCurMESH);
		pOBJ->ResetOBJPart(&CMainFrame::m_3DDevice);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}

	EnableAllCtrl(!pCtrl->m_bPlay);
}

void COBJBar::OnLvnEndlabeleditListClothdata( NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl *pListCtrl = m_cMESH.GetListCtrl();
	CString strNEW(pDispInfo->item.pszText);

	if( NULL == pDispInfo->item.pszText )
	{
		*pResult = 0;
		return ;
	}


	if( strNEW.IsEmpty() || !CItemBase::CheckID(strNEW) )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
	}
	else if(!m_pItem->CheckDupLocalID( pListCtrl->GetItemText( pDispInfo->item.iItem, 0), strNEW))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_DUP_ATTRID);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
	}
	else
	{
		DWORD dwID = (DWORD) pListCtrl->GetItemData(pDispInfo->item.iItem);
		MAPSTR::iterator finder = m_pItem->m_mapNAME.find(dwID);

		if( finder != m_pItem->m_mapNAME.end() &&
			strNEW != (*finder).second )
		{
			CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
			CItemBase::RegLocalID( strNEW, m_pItem);

			(*finder).second = strNEW;
			pListCtrl->SetItemText(
				pDispInfo->item.iItem,
				0, strNEW);

			m_pItem->GetMainDocument()->UpdateAll();
		}
	}

	*pResult = 0;
}

void COBJBar::OnLvnEndlabeleditListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl *pListCtrl = m_cANI.GetListCtrl();
	CString strNEW(pDispInfo->item.pszText);

	if( strNEW.IsEmpty() || !CItemBase::CheckID(strNEW) )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
	}
	else if(!m_pItem->CheckDupLocalID( pListCtrl->GetItemText( pDispInfo->item.iItem, 0), strNEW))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_DUP_ATTRID);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
	}
	else
	{
		DWORD dwID = (DWORD) pListCtrl->GetItemData(pDispInfo->item.iItem);
		MAPSTR::iterator finder = m_pItem->m_mapNAME.find(dwID);

		if( finder != m_pItem->m_mapNAME.end() &&
			strNEW != (*finder).second )
		{
			CItemBase::ReleaseLocalID( (*finder).second, m_pItem);
			CItemBase::RegLocalID( strNEW, m_pItem);

			(*finder).second = strNEW;
			pListCtrl->SetItemText(
				pDispInfo->item.iItem,
				0, strNEW);

			m_pItem->GetMainDocument()->UpdateAll();
		}
	}

	*pResult = 0;
}

void COBJBar::OnLvnBeginlabeleditListClothdata( NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CEdit *pEdit = m_cMESH.GetListCtrl()->GetEditControl();
	pEdit->ModifyStyle( 0, ES_UPPERCASE);
	*pResult = 0;
}

void COBJBar::OnLvnBeginlabeleditListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CEdit *pEdit = m_cANI.GetListCtrl()->GetEditControl();
	pEdit->ModifyStyle( 0, ES_UPPERCASE);
	*pResult = 0;
}

void COBJBar::InitSFXList()
{
	m_cSFX.GetListCtrl()->DeleteAllItems();
	SetCurSFX();

	switch(m_nSFXKind)
	{
	case OEK_OBJ	: InitSFXList(&m_pItem->m_OBJ.m_mapSFX); break;
	case OEK_ACT	:
		{
			LPACTION pACT = GetCurACT();
			if(pACT)
				InitSFXList(&pACT->m_mapSFX);
		}

		break;

	case OEK_ANI	:
		{
			LPACTINST pAI = GetCurAI();
			if( pAI && pAI->m_pANI )
				InitSFXList(&pAI->m_pANI->m_mapSFX);
		}

		break;
	}
}

void COBJBar::InitSFXList( LPMAPSFXINST pSFX)
{
	CListCtrl *pList = m_cSFX.GetListCtrl();
	MAPSFXINST::iterator it;

	for( it = pSFX->begin(); it != pSFX->end(); it++)
	{
		MAPDWORD::iterator finder = m_pItem->m_mapSFX.find((*it).first);

		if( finder != m_pItem->m_mapSFX.end() )
		{
			CSFXItem *pItem = CItemBase::GetSFXItem((*finder).second);
			int nIndex = pList->InsertItem(
				pList->GetItemCount(),
				pItem->m_strName);

			pList->SetItemData( nIndex, (*it).first);
		}
	}
}

void COBJBar::SetCurSFX()
{
	LPSFXINST pSFX = GetCurSFX();

	m_strERotX.Format( "%.2f", pSFX ? pSFX->m_vROT.x * 180.0f / D3DX_PI : 0.0f);
	m_strERotY.Format( "%.2f", pSFX ? pSFX->m_vROT.y * 180.0f / D3DX_PI : 0.0f);
	m_strERotZ.Format( "%.2f", pSFX ? pSFX->m_vROT.z * 180.0f / D3DX_PI : 0.0f);
	m_strEPosX.Format( "%.2f", pSFX ? pSFX->m_vPOS.x : 0.0f);
	m_strEPosY.Format( "%.2f", pSFX ? pSFX->m_vPOS.y : 0.0f);
	m_strEPosZ.Format( "%.2f", pSFX ? pSFX->m_vPOS.z : 0.0f);

	m_dwSFXFUNCTION = pSFX ? pSFX->m_dwFuncID : 0;
	m_dwEStartAT = pSFX ? pSFX->m_dwTick : 0;
	m_bEStop = pSFX ? pSFX->m_bDeleteOnStop : FALSE;
	m_bBias = pSFX ? pSFX->m_bBias : 0;
	m_nEPivotID = pSFX ? pSFX->m_nPivotID : 0;

	if( pSFX && m_nSFXKind == OEK_OBJ )
	{
		CListCtrl* pListCtrl = m_cHEBA_LIST.GetListCtrl();
		pListCtrl->DeleteAllItems();

		MAPDWORD::iterator it, end;
		it = pSFX->m_mapHideByAction.begin();
		end = pSFX->m_mapHideByAction.end();
		for(; it != end ; ++it )
		{
			DWORD dwAction = (*it).first;
			MAPSTR::iterator finder = m_pItem->m_mapNAME.find(dwAction);
			
			int nIndex = pListCtrl->InsertItem(pListCtrl->GetItemCount(), (*finder).second);
			pListCtrl->SetItemData(nIndex, dwAction);
		}
		m_cHEBA_LIST.UpdateColumnWidth();
	}

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void COBJBar::OnBnClickedRadioSFX()
{
	UpdateData();
	InitSFXList();
	EnableAllCtrl(TRUE);
}

void COBJBar::OnLvnItemchangedListSfx( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		SetCurSFX();

	*pResult = 0;
}

void COBJBar::OnNMClickListSfx( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSFX();
	*pResult = 0;
}

void COBJBar::OnNMRclickListSfx( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSFX();
	*pResult = 0;
}

void COBJBar::OnNMRdblclkListSfx( NMHDR *pNMHDR, LRESULT *pResult)
{
	DeleteCurSFX();
	*pResult = 0;
}

void COBJBar::OnNMDblclkListSfx( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSFX();
	*pResult = 0;
}

void COBJBar::OnEnChangeEditPosX()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vPOS.x = (FLOAT) atof(m_strEPosX);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditPosY()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vPOS.y = (FLOAT) atof(m_strEPosY);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditPosZ()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vPOS.z = (FLOAT) atof(m_strEPosZ);

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditRotX()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vROT.x = (FLOAT) atof(m_strERotX) * D3DX_PI / 180.0f;

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditRotY()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vROT.y = (FLOAT) atof(m_strERotY) * D3DX_PI / 180.0f;

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditRotZ()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		UpdateData();
		pSFX->m_vROT.z = (FLOAT) atof(m_strERotZ) * D3DX_PI / 180.0f;

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnEnChangeEditStart()
{
	if(m_pItem)
	{
		LPSFXINST pSFX = GetCurSFX();

		if(pSFX)
		{
			UpdateData();
			pSFX->m_dwTick = m_dwEStartAT;
		}
	}
}

void COBJBar::OnEnChangeEditBias()
{
	if(m_pItem)
	{
		LPSFXINST pSFX = GetCurSFX();

		if(pSFX)
		{
			UpdateData();
			pSFX->m_bBias = m_bBias;
		}
	}
}

void COBJBar::OnCbnSelchangeComboOep()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		UpdateData();
		pSFX->m_nPivotID = m_nEPivotID;
	}
}

void COBJBar::OnBnClickedCheckDelonstop()
{
	LPSFXINST pSFX = GetCurSFX();

	if(pSFX)
	{
		UpdateData();
		pSFX->m_bDeleteOnStop = m_bEStop;
	}
}

void COBJBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinEPosX )
	{
		FLOAT fEPosX = (FLOAT) atof(m_strEPosX);

		fEPosX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosX.SetPos(0);

		m_strEPosX.Format( "%.2f", fEPosX);
		UpdateData(FALSE);
		OnEnChangeEditPosX();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEPosY )
	{
		FLOAT fEPosY = (FLOAT) atof(m_strEPosY);

		fEPosY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosY.SetPos(0);

		m_strEPosY.Format( "%.2f", fEPosY);
		UpdateData(FALSE);
		OnEnChangeEditPosY();
	}

	if( pScrollBar == (CScrollBar *) &m_spinEPosZ )
	{
		FLOAT fEPosZ = (FLOAT) atof(m_strEPosZ);

		fEPosZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinEPosZ.SetPos(0);

		m_strEPosZ.Format( "%.2f", fEPosZ);
		UpdateData(FALSE);
		OnEnChangeEditPosZ();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotX )
	{
		FLOAT fERotX = (FLOAT) atof(m_strERotX);

		fERotX += (FLOAT) INT(nPos);
		m_spinERotX.SetPos(0);

		m_strERotX.Format( "%.2f", fERotX);
		UpdateData(FALSE);
		OnEnChangeEditRotX();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotY )
	{
		FLOAT fERotY = (FLOAT) atof(m_strERotY);

		fERotY += (FLOAT) INT(nPos);
		m_spinERotY.SetPos(0);

		m_strERotY.Format( "%.2f", fERotY);
		UpdateData(FALSE);
		OnEnChangeEditRotY();
	}

	if( pScrollBar == (CScrollBar *) &m_spinERotZ )
	{
		FLOAT fERotZ = (FLOAT) atof(m_strERotZ);

		fERotZ += (FLOAT) INT(nPos);
		m_spinERotZ.SetPos(0);

		m_strERotZ.Format( "%.2f", fERotZ);
		UpdateData(FALSE);
		OnEnChangeEditRotZ();
	}

	if( pScrollBar == (CScrollBar *) &m_spinIntensity )
	{
		FLOAT fIntensity = (FLOAT) atof(m_strIntensity);

		fIntensity += (FLOAT) INT(nPos) / 100.0f;
		m_spinIntensity.SetPos(0);

		m_strIntensity.Format( "%.2f", fIntensity);
		UpdateData(FALSE);
		OnEnChangeEditIntensity();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void COBJBar::InsertSND( CMediaItem *pItem)
{
	LPMAPSNDINST pOBJINST = NULL;
	LPMAPSNDINST pINST = NULL;

	switch(m_nSNDKind)
	{
	case OSK_OBJ	:
		{
			pOBJINST = &m_pItem->m_TOBJ.m_mapOBJSNDINST;
			pINST = &m_pItem->m_OBJ.m_mapSND;
		}

		break;

	case OSK_ACT	:
		{
			LPACTION pACT = GetCurACT();

			pOBJINST = &m_pItem->m_TOBJ.m_mapACTSNDINST;
			pINST = &pACT->m_mapSND;
		}

		break;

	case OSK_ANI	:
		{
			LPACTINST pAI = GetCurAI();

			pOBJINST = &m_pItem->m_TOBJ.m_mapANISNDINST;
			pINST = &pAI->m_pANI->m_mapSND;
		}

		break;
	}

	if(pINST)
	{
		LPSNDINST pSNDINST = new SNDINST();
		DWORD dwInstID = 1;

		while(pINST->find(dwInstID) != pINST->end())
			dwInstID++;
		pSNDINST->m_dwMediaID = pItem->m_dwID;

		pOBJINST->insert( MAPSNDINST::value_type(
			dwInstID,
			pSNDINST));

		pINST->insert( MAPSNDINST::value_type(
			dwInstID,
			pSNDINST));

		int nIndex = m_cSOUND.GetListCtrl()->InsertItem(
			m_cSOUND.GetListCtrl()->GetItemCount(),
			pItem->m_strName);
		m_cSOUND.GetListCtrl()->SetItemData( nIndex, dwInstID);
	}
}

LPSNDINST COBJBar::GetCurSND()
{
	CListCtrl *pList = m_cSOUND.GetListCtrl();
	LPSNDINST pResult = NULL;

	int nIndex = m_cSOUND.GetCurSel();
	if( nIndex != -1 )
	{
		DWORD dwID = (DWORD) pList->GetItemData(nIndex);
		LPMAPSNDINST pINST = NULL;

		switch(m_nSNDKind)
		{
		case OSK_OBJ	: pINST = &m_pItem->m_OBJ.m_mapSND; break;
		case OSK_ACT	:
			{
				LPACTION pACT = GetCurACT();
				if(pACT)
					pINST = &pACT->m_mapSND;
			}

			break;

		case OSK_ANI	:
			{
				LPACTINST pAI = GetCurAI();
				if( pAI && pAI->m_pANI )
					pINST = &pAI->m_pANI->m_mapSND;
			}

			break;
		}

		if(pINST)
		{
			MAPSNDINST::iterator finder = pINST->find(dwID);
			if( finder != pINST->end() )
				pResult = (*finder).second;
		}
	}

	return pResult;
}

void COBJBar::SetCurSND()
{
	LPSNDINST pSND = GetCurSND();

	if(pSND)
	{
		m_dwSNDFUNCTION = pSND->m_dwFuncID;
		m_dwSStartAT = pSND->m_dwTick;
		m_bSStop = pSND->m_bDeleteOnStop;

		UpdateData(FALSE);
	}

	EnableAllCtrl(TRUE);
}

void COBJBar::OnLvnItemchangedListSoundid( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		SetCurSND();

	*pResult = 0;
}

void COBJBar::OnNMClickListSoundid( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSND();
	*pResult = 0;
}

void COBJBar::OnNMRclickListSoundid( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSND();
	*pResult = 0;
}

void COBJBar::OnNMRdblclkListSoundid( NMHDR *pNMHDR, LRESULT *pResult)
{
	DeleteCurSND();
	*pResult = 0;
}

void COBJBar::OnNMDblclkListSoundid( NMHDR *pNMHDR, LRESULT *pResult)
{
	SetCurSND();
	*pResult = 0;
}

void COBJBar::DeleteCurSND()
{
	CListCtrl *pList = m_cSOUND.GetListCtrl();
	int nIndex = m_cSOUND.GetCurSel();

	if( nIndex != -1 )
	{
		DWORD dwID = (DWORD) pList->GetItemData(nIndex);
		LPMAPSNDINST pOBJINST = NULL;
		LPMAPOBJSND pOBJSND = NULL;
		LPMAPSNDINST pINST = NULL;

		switch(m_nSNDKind)
		{
		case OSK_OBJ	:
			{
				pOBJINST = &m_pItem->m_TOBJ.m_mapOBJSNDINST;
				pOBJSND = &m_pItem->m_TOBJ.m_mapOBJSND;
				pINST = &m_pItem->m_OBJ.m_mapSND;
			}

			break;

		case OSK_ACT	:
			{
				LPACTION pACT = GetCurACT();

				pOBJINST = &m_pItem->m_TOBJ.m_mapACTSNDINST;
				pOBJSND = &m_pItem->m_TOBJ.m_mapACTSND;
				if(pACT)
					pINST = &pACT->m_mapSND;
			}

			break;

		case OSK_ANI	:
			{
				LPACTINST pAI = GetCurAI();

				pOBJINST = &m_pItem->m_TOBJ.m_mapANISNDINST;
				pOBJSND = &m_pItem->m_TOBJ.m_mapANISND;
				if( pAI && pAI->m_pANI )
					pINST = &pAI->m_pANI->m_mapSND;
			}

			break;
		}

		if(pOBJINST)
		{
			MAPSNDINST::iterator finder = pOBJINST->find(dwID);

			if( finder != pOBJINST->end() )
				pOBJINST->erase(finder);
		}

		if(pOBJSND)
		{
			MAPOBJSND::iterator finder = pOBJSND->find(dwID);

			if( finder != pOBJSND->end() )
			{
				CD3DSound *pWAV = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TSOUND,
					(*finder).second->m_pSND->m_dwMediaID);

				pWAV->Stop((*finder).second->m_nIndex);
				pWAV->Unlock((*finder).second->m_nIndex);

				delete (*finder).second;
				pOBJSND->erase(finder);
			}
		}

		if(pINST)
		{
			MAPSNDINST::iterator finder = pINST->find(dwID);

			if( finder != pINST->end() )
			{
				delete (*finder).second;
				pINST->erase(finder);

				pList->DeleteItem(nIndex);
			}
		}
	}

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	SetCurSND();

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void COBJBar::InitSNDList()
{
	m_cSOUND.GetListCtrl()->DeleteAllItems();
	SetCurSND();

	switch(m_nSNDKind)
	{
	case OSK_OBJ	: InitSNDList(&m_pItem->m_OBJ.m_mapSND); break;
	case OSK_ACT	:
		{
			LPACTION pACT = GetCurACT();
			if(pACT)
				InitSNDList(&pACT->m_mapSND);
		}

		break;

	case OSK_ANI	:
		{
			LPACTINST pAI = GetCurAI();
			if( pAI && pAI->m_pANI )
				InitSNDList(&pAI->m_pANI->m_mapSND);
		}

		break;
	}
}

void COBJBar::InitSNDList( LPMAPSNDINST pSND)
{
	CListCtrl *pList = m_cSOUND.GetListCtrl();
	MAPSNDINST::iterator it;

	for( it = pSND->begin(); it != pSND->end(); it++)
	{
		CMediaItem *pItem = CItemBase::GetMediaItem((*it).second->m_dwMediaID);
		int nIndex = pList->InsertItem(
			pList->GetItemCount(),
			pItem->m_strName);

		pList->SetItemData( nIndex, (*it).first);
	}
}

void COBJBar::OnBnClickedRadioSND()
{
	UpdateData();
	InitSNDList();
	EnableAllCtrl(TRUE);
}

void COBJBar::OnEnChangeEditSstart()
{
	if(m_pItem)
	{
		LPSNDINST pSND = GetCurSND();

		if(pSND)
		{
			UpdateData();
			pSND->m_dwTick = m_dwSStartAT;
		}
	}
}

void COBJBar::OnEnChangeEditFunction()
{
	if(m_pItem)
	{
		LPSNDINST pSND = GetCurSND();

		if(pSND)
		{
			UpdateData();
			pSND->m_dwFuncID = m_dwSNDFUNCTION;
		}
	}
}

void COBJBar::OnBnClickedCheckSdelonstop()
{
	LPSNDINST pSND = GetCurSND();

	if(pSND)
	{
		UpdateData();
		pSND->m_bDeleteOnStop = m_bSStop;
	}
}

void COBJBar::OnEnChangeEditObjfunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwOBJSNDFuncID = m_dwSNDOBJFUNC;
	}
}

void COBJBar::OnEnChangeEditActfunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwACTSNDFuncID = m_dwSNDACTFUNC;
	}
}

void COBJBar::OnEnChangeEditAnifunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwANISNDFuncID = m_dwSNDANIFUNC;
	}
}

void COBJBar::OnBnClickedButtonSetestart()
{
	if(m_pItem)
	{
		LPSFXINST pSFX = GetCurSFX();

		if(pSFX)
		{
			m_dwEStartAT = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);
			pSFX->m_dwTick = m_dwEStartAT;
			UpdateData(FALSE);
		}
	}
}

void COBJBar::OnBnClickedButtonSetsstart()
{
	if(m_pItem)
	{
		LPSNDINST pSND = GetCurSND();

		if(pSND)
		{
			m_dwSStartAT = CTMath::GetTimeTick(m_pItem->m_TOBJ.m_fActTime);
			pSND->m_dwTick = m_dwSStartAT;
			UpdateData(FALSE);
		}
	}
}

LRESULT COBJBar::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_ERR_DUP_ATTRID);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}

void COBJBar::OnEnChangeEditSfxfunction()
{
	if(m_pItem)
	{
		LPSFXINST pSFX = GetCurSFX();

		if(pSFX)
		{
			UpdateData();
			pSFX->m_dwFuncID = m_dwSFXFUNCTION;
		}
	}
}

void COBJBar::OnEnChangeEditObjsfxfunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwOBJSFXFuncID = m_dwSFXOBJFUNC;
	}
}

void COBJBar::OnEnChangeEditActsfxfunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwACTSFXFuncID = m_dwSFXACTFUNC;
	}
}

void COBJBar::OnEnChangeEditAnisfxfunc()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_TOBJ.m_dwANISFXFuncID = m_dwSFXANIFUNC;
	}
}

void COBJBar::OnCbnSelchangeComboBlendop()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwBlendOP = g_dwBLENDOP[m_cBlendOP.GetCurSel()];
		pDoc->m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnCbnSelchangeComboDestblend()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwDESTBlend = g_dwBLEND[m_cDESTBlend.GetCurSel()];
		pDoc->m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnCbnSelchangeComboSrcblend()
{
	LPMESH pMESH = GetCurMESH();

	if(pMESH)
	{
		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		LPOBJTEX pTEX = pMESH->m_vTEX[m_bCurTEX];
		UpdateData();

		pTEX->m_dwSRCBlend = g_dwBLEND[m_cSRCBlend.GetCurSel()];
		pDoc->m_pItem->m_TOBJ.ResetOBJPart(&CMainFrame::m_3DDevice);
		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void COBJBar::OnBnClickedCheckBatchRegSndfunc()
{
	CListCtrl *pList = m_cSOUND.GetListCtrl();

	if(!pList)
		return;

	VECTORDWORD vNewFuncArray;
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		_T("List File (*.txt)|*.txt|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		FILE *pFILE = fopen( (LPCTSTR) dlg.GetPathName(), "rt");

		while(!feof(pFILE))
		{
			char szLINE[10] = _T("");
			DWORD dwFuncID = 0;

			fgets( szLINE, 10, pFILE);
			if( szLINE[0] == '\0' ||
				szLINE[0] == '\n' ||
				szLINE[0] == '\r' ||
				szLINE[0] == '\t' )
				continue;

			sscanf( szLINE, "%d", &dwFuncID);
			vNewFuncArray.push_back(dwFuncID);
		}

		fclose(pFILE);
	}
	else
		return;

	if( vNewFuncArray.size() == 0 )
		return;

	if( vNewFuncArray.size() != pList->GetItemCount() )
	{
		CString strMessage;
		strMessage.LoadString(ID_NOT_MATCH_COUNT);
		MessageBox( strMessage, "Error", MB_OK|MB_ICONHAND);
		return;
	}
	LPMAPSNDINST pINST = NULL;

	switch(m_nSNDKind)
	{
	case OSK_OBJ	: pINST = &m_pItem->m_OBJ.m_mapSND; break;
	case OSK_ACT	:
		{
			LPACTION pACT = GetCurACT();

			if(pACT)
				pINST = &pACT->m_mapSND;
		}

		break;

	case OSK_ANI	:
		{
			LPACTINST pAI = GetCurAI();

			if( pAI && pAI->m_pANI )
				pINST = &pAI->m_pANI->m_mapSND;
		}

		break;
	}

	if(pINST)
	{
		int nCount = pList->GetItemCount();

		for( int i=0; i<nCount; i++)
		{
			DWORD dwID = (DWORD) pList->GetItemData(i);
			MAPSNDINST::iterator finder = pINST->find(dwID);

			if( finder != pINST->end() )
			{
				LPSNDINST pSND = (*finder).second;

				if(pSND)
					pSND->m_dwFuncID = vNewFuncArray[i];
			}
		}
	}

	UpdateData(FALSE);
}

void COBJBar::OnBnClickedCopySnd()
{
	if( m_pDEST && m_pDEST != m_pItem )
	{
		COBJDoc *pDoc = (COBJDoc *) m_pDEST->GetMainDocument();

		LPMAPSNDINST pDEST = &m_pDEST->m_OBJ.m_mapSND;
		LPMAPSNDINST pSRC = &m_pItem->m_OBJ.m_mapSND;
		MAPSNDINST::iterator it;

		if(pDoc)
			pDoc->OnCloseDocument();

		for( it = pDEST->begin(); it != pDEST->end(); it++)
			delete (*it).second;
		pDEST->clear();

		for( it = pSRC->begin(); it != pSRC->end(); it++)
		{
			LPSNDINST pSNDINST = new SNDINST();
			DWORD dwInstID = 1;

			while(pDEST->find(dwInstID) != pDEST->end())
				dwInstID++;

			pSNDINST->m_bDeleteOnStop = (*it).second->m_bDeleteOnStop;
			pSNDINST->m_dwMediaID = (*it).second->m_dwMediaID;
			pSNDINST->m_dwFuncID = (*it).second->m_dwFuncID;
			pSNDINST->m_dwTick = (*it).second->m_dwTick;

			pDEST->insert( MAPSNDINST::value_type(
				dwInstID,
				pSNDINST));
		}
	}
}

int COBJBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBarContent::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_vOBJBAR.push_back(this);
	return 0;
}

void COBJBar::OnDestroy()
{
	CBarContent::OnDestroy();
	VECTOROBJBAR::iterator it = m_vOBJBAR.begin();

	while( it != m_vOBJBAR.end() )
		if( (*it) == this )
			m_vOBJBAR.erase(it);
		else
			it++;
}

void COBJBar::OnBnClickedDeleteClothdata()
{
	int res = MessageBox( _T("삭제 하시겠습니까?"), _T("Delete ClothData!"), MB_ICONQUESTION | MB_YESNO );
	if( res == IDYES )
	{
		int nIndex = m_cMESH.GetCurSel();

		if( nIndex == -1 )
		{
			LPCLKINST pCI = GetCurCI();

			if(pCI)
			{
				CListCtrl *pListCtrl = m_cMESH.GetListCtrl();

				for( int i=0; i<pListCtrl->GetItemCount(); i++)
					if( pCI->m_dwCurMESH == pListCtrl->GetItemData(i) )
					{
						m_cMESH.SetCurSel(i);
						break;
					}
			}
		}
		else
			DeleteMESH(nIndex);
	}
}

void COBJBar::OnLvnKeydownListClothdata(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	if(pLVKeyDow->wVKey == VK_F2 &&
		m_cMESH.GetListCtrl() )
	{
		int nIndex = m_cMESH.GetCurSel();
		if( nIndex != -1 )
		{
			m_cMESH.SetFocus();
			m_cMESH.GetListCtrl()->EditLabel( nIndex );
		}
	}

	*pResult = 0;
}

void COBJBar::OnBnClickedCheckDeleteani()
{
	int nIndex = m_cANI.GetCurSel();
	
	if( nIndex != -1 )
		DeleteANI(nIndex);
}

void COBJBar::OnBnClickedCheckDeleffect()
{
	COBJBar::DeleteCurSFX();
}

void COBJBar::OnBnClickedCheckDelsound()
{
	COBJBar::DeleteCurSND();
}

void COBJBar::OnBnClickedButtonSfxup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void COBJBar::OnBnClickedButtonSfxdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void COBJBar::OnBnClickedButtonHebaDelete()
{
	if( m_nSFXKind == OEK_OBJ )
	{
		LPSFXINST pSFX = GetCurSFX();
		int nCurSel = m_cHEBA_LIST.GetCurSel();
		CListCtrl* pListCtrl = m_cHEBA_LIST.GetListCtrl();
		if( nCurSel != -1 )
		{
			DWORD dwAction = pListCtrl->GetItemData(nCurSel);
			MAPDWORD::iterator finder = pSFX->m_mapHideByAction.find( dwAction );
			if( pSFX->m_mapHideByAction.end() != finder  )
			{
				pSFX->m_mapHideByAction.erase(finder);
				pListCtrl->DeleteItem(nCurSel);
				m_cHEBA_LIST.UpdateColumnWidth();
				UpdateData();
			}
		}

		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		m_pItem->m_TOBJ.RemoveOBJSFX();
		pDoc->RemoveSFX();
		pCtrl->m_cPlayCtrl.SetPos(0);
		UpdatePos(0);
	}
}

void COBJBar::OnBnClickedButtonHebaAdd()
{
	if( m_nSFXKind == OEK_OBJ )
	{
		LPSFXINST pSFX = GetCurSFX();
		int nCurSel = m_cHEBA_ACT.GetCurSel();
		if( nCurSel != -1 )
		{
			DWORD dwAction = m_cHEBA_ACT.GetItemData(nCurSel);
			if( pSFX->m_mapHideByAction.end() == pSFX->m_mapHideByAction.find( dwAction ) )
			{
				pSFX->m_mapHideByAction.insert( MAPDWORD::value_type(dwAction, dwAction) );

				CString strName;
				m_cHEBA_ACT.GetLBText(nCurSel, strName);

				CListCtrl* pListCtrl = m_cHEBA_LIST.GetListCtrl();
				int nIndex = pListCtrl->InsertItem(pListCtrl->GetItemCount(), strName);
				pListCtrl->SetItemData(nIndex, dwAction);

				m_cHEBA_LIST.UpdateColumnWidth();
				UpdateData();
			}
		}

		COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		m_pItem->m_TOBJ.RemoveOBJSFX();
		pDoc->RemoveSFX();
		pCtrl->m_cPlayCtrl.SetPos(0);
		UpdatePos(0);
	}
}

void COBJBar::OnBnClickedFindObjtexture()
{
	LPCLKINST pClkInst = GetCurCI();

	COBJDoc* pDoc = (COBJDoc*)m_pItem->GetMainDocument();
	DWORD dwTexID = pDoc->m_pItem->GetTexID( pClkInst->m_dwCurMESH, m_bCurTEX, m_nSTG );

	MAPDWORD::iterator itTEX = pDoc->m_pItem->m_mapTEX.find( dwTexID );
	if( itTEX == pDoc->m_pItem->m_mapTEX.end() )
		return ;

	DWORD dwTexItemID = (*itTEX).second;
	CString strID;
	strID.Format( "%d", dwTexItemID );

	// Find.
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();
	pMainWnd->OnItemFinder();
	pMainWnd->m_pFinderDlg->ClearFinder();
	pMainWnd->m_pFinderDlg->SetData( strID, 0, 2, 0 );
	pMainWnd->m_pFinderDlg->FindItem();
	pMainWnd->m_pFinderDlg->ShowWindow( SW_HIDE );
}

void COBJBar::OnBnClickedCheckUsedirlight()
{
	int nSel = m_cUseDirLightOp.GetCurSel();
	if( nSel == -1 ) return ;

	if( nSel == 0 )	// 전체 메쉬에 대해 설정.
	{
		LPMESH pMesh = GetCurMESH();
		if( pMesh )
		{
			VECTOROBJTEX::iterator itor = pMesh->m_vTEX.begin();
			VECTOROBJTEX::iterator end = pMesh->m_vTEX.end();

			while( itor != end )
			{
				(*itor)->m_bUseDirLight = m_cUseDirLight.GetCheck();

				++itor;
			}
		}
	}
	else if( nSel == 1 )	// 선택된 메쉬만 설정.
	{
		LPMESH pMesh = GetCurMESH();
		if( pMesh )
		{
			LPOBJTEX pTex = pMesh->m_vTEX[ m_bCurTEX ];
			pTex->m_bUseDirLight = m_cUseDirLight.GetCheck();
		}
	}
	else
		return ;

	COBJDoc *pDoc = (COBJDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	if(!pCtrl->m_bPlay)
	{
		EnableAllCtrl( TRUE );
		pDoc->RedrawAllView();
	}
}

void COBJBar::OnCbnSelchangeComboUsedirlightop()
{
	
}

