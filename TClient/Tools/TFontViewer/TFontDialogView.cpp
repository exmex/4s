// TFontDialogView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "TFontViewer.h"
#include "TFontDialogView.h"
#include ".\tfontdialogview.h"


IMPLEMENT_DYNCREATE(CTFontDialogView, CFormView)

CTFontDialogView::CTFontDialogView()
	: CFormView(CTFontDialogView::IDD)
	, m_strFontName(_T("Arial"))
	, m_strText(_T("Test"))
	, m_nFontSize(0)
	, m_nFontRender(4)
	, m_rtPos(0, 0, 0, 0)
{

}

CTFontDialogView::~CTFontDialogView()
{
	ReleaseFontData();
}

void CTFontDialogView::InitFontData()
{
	CMainFrame* pMainWnd = (CMainFrame*) AfxGetMainWnd();

	CComboBox* pCombo = (CComboBox*) GetDlgItem(IDC_COMBO_FONT_FONTSIZE);
	pCombo->Clear();

	FILE* fp = fopen(_T("fontlist.txt"), "rt");
	INT nSize = 0;
	
	if ( fp != NULL )
	{
		fscanf(fp, "%d", &nSize );

		for(int i = 0; i < nSize; i++)
		{
			TFONT_TYPE *pFont = new TFONT_TYPE;
			char in_char[256];
			fscanf(fp, "%s %d %d %x", in_char, &pFont->m_nType, &pFont->m_dwSize, &pFont->m_dwColor );
			pFont->m_strType.Format("%s", in_char );
			m_vtFontType.push_back( pFont );

			pCombo->AddString( pFont->m_strType );
		}
		fclose(fp);
	}
	else
	{
		TFONT_TYPE arFont[21] = {
			{ "ID_FONT_BIG_1", 0, 36, 0xffd2d2d2 },
			{ "ID_FONT_BIGFONT", 0, 24, 0xff000000},
			{ "ID_FONT_DISABLE", 0, 9, 0xff7d7d7d},
			{ "ID_FONT_FAME_LIST", 0, 9, 0xff989284},
			{ "ID_FONT_LOGIN", 0, 9,  0xffd2d2d2},
			{ "ID_FONT_LOGIN_BLUE", 0, 9, 0xff80ccfd},
			{ "ID_FONT_LOGIN_1", 0, 10, 0xffd2d2d2},
			{ "ID_FONT_LOGIN_1_BLUE", 0, 10, 0xff80ccfd},
			{ "ID_FONT_LOGIN_1_BROWN", 1, 10, 0xffd9c298},
			{ "ID_FONT_LOGIN_1_ORANGE", 0, 10, 0xfffed000},
			{ "ID_FONT_LOGIN_BROWN", 0, 9, 0xffd9c297},
			{ "ID_FONT_LOGIN_ORANGE", 0, 9, 0xfffed000},
			{ "ID_FONT_LOGIN_YELLOW", 0, 9, 0xffe6e699},
			{ "ID_FONT_MID_1", 0, 18, 0xffd2d2d2},
			{ "ID_FONT_NORMAL", 0, 18, 0xff000000},
			{ "ID_FONT_NORMAL_1", 1, 10, 0xffd2d2d2},
			{ "ID_FONT_QUEST_BLUE", 0, 11, 0xff80ccfd},
			{ "ID_FONT_QUEST_BROWN", 0, 11, 0xffc7b48a},
			{ "ID_FONT_QUEST_NORMAL", 0, 11, 0xffd2d2d2},
			{ "ID_FONT_QUEST_TEXT", 0, 10, 0xffd2d2d2},
			{ "ID_FONT_USE_NOT", 0, 9, 0xff3c3c3c}
		};
	
		for ( int i = 0; i < 21; ++i )
		{
			TFONT_TYPE* pFont = new TFONT_TYPE;
			
			pFont->m_dwColor = arFont[i].m_dwColor;
			pFont->m_dwSize = arFont[i].m_dwSize;
			pFont->m_nType = arFont[i].m_nType;
			pFont->m_strType = arFont[i].m_strType;
			
			m_vtFontType.push_back( pFont );
			pCombo->AddString( pFont->m_strType );
		}

	}




	memset( &lf, 0x00, sizeof(LOGFONT) );

	strcpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = m_vtFontType[TDEFAULT_FONT_TYPE]->m_dwSize;
	
	//if ( m_vtFontType.size() > TDEFAULT_FONT_TYPE )
	//	pMainWnd->SetFont( &lf , m_bOutLine, m_bShadow, (m_vtFontType[TDEFAULT_FONT_TYPE])->m_dwColor, m_nFontRender, m_rtPos );
	
	m_dwFontColor = (m_vtFontType[TDEFAULT_FONT_TYPE])->m_dwColor;
	
	m_nFontSize = TDEFAULT_FONT_TYPE;
	UpdateData(FALSE);




} 

void CTFontDialogView::ReleaseFontData()
{
	vtLPTFONT_TYPE::iterator itor;

	for ( itor = m_vtFontType.begin(); itor != m_vtFontType.end(); ++itor )
	{
	    delete (*itor);
	}

	m_vtFontType.clear();

}

void CTFontDialogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FONT_FONTNAME, m_strFontName);
	DDX_CBIndex(pDX, IDC_COMBO_FONT_FONTSIZE, m_nFontSize);
	DDX_CBIndex(pDX, IDC_COMBO_FONT_RENDER, m_nFontRender);
	DDX_Check(pDX, IDC_CHECK_FONT2_OUTLINE, m_bOutLine );
	DDX_Check(pDX, IDC_CHECK_FONT2_SHADOW, m_bShadow );
	DDX_Text(pDX, IDC_EDIT_FONT_TEXT, m_strText);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_rtPos.left );
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_rtPos.top );

}

BEGIN_MESSAGE_MAP(CTFontDialogView, CFormView)
	ON_EN_CHANGE(IDC_EDIT_FONT_TEXT, OnEnChangeEditFontText)
	ON_BN_CLICKED(IDC_CHECK_FONT2_OUTLINE, OnBnClickedCheckFont2Outline)
	ON_BN_CLICKED(IDC_CHECK_FONT2_SHADOW, OnBnClickedCheckFont2Shadow)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_RENDER, OnCbnSelchangeComboFontRender)
	ON_BN_CLICKED(IDC_BTN_FONT_FONTSELECT, OnBnClickedBtnFontFontselect)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT_FONTSIZE, OnCbnSelchangeComboFontFontsize)
	ON_EN_CHANGE(IDC_EDIT_POS_X, OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, OnEnChangeEditPosY)
	ON_BN_CLICKED(IDC_BTN_IMG_LOAD, OnBnClickedBtnImgLoad)
END_MESSAGE_MAP()


// CTFontDialogView 진단입니다.

#ifdef _DEBUG
void CTFontDialogView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTFontDialogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
void CTFontDialogView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	InitFontData();
}

void CTFontDialogView::OnEnChangeEditFontText()
{
	UpdateData();
	CMainFrame* pMainWnd = (CMainFrame*) AfxGetMainWnd();

	pMainWnd->SetText( m_strText );

}

void CTFontDialogView::UpdateFont()
{
	CMainFrame* pMainWnd = (CMainFrame*) AfxGetMainWnd();

	UpdateData();
	pMainWnd->SetFont( &lf , m_bOutLine, m_bShadow, m_dwFontColor , m_nFontRender, m_rtPos );
}

void CTFontDialogView::OnBnClickedCheckFont2Outline()
{
	UpdateFont();	
}

void CTFontDialogView::OnBnClickedCheckFont2Shadow()
{
	UpdateFont();
}


void CTFontDialogView::OnCbnSelchangeComboFontRender()
{
	UpdateFont();

//  TextRenderingHintSystemDefault = 0,            // Glyph with system default rendering hint
//	TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
//	TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
//	TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
//	TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
//	TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
	
}

void CTFontDialogView::OnBnClickedBtnFontFontselect()
{
	CFontDialog Dlg(&lf);
	if(Dlg.DoModal() == IDOK)
	{
		UpdateFont();
	}
}

void CTFontDialogView::OnCbnSelchangeComboFontFontsize()
{
	CClientDC dc(this);

	lf.lfHeight = -MulDiv(m_vtFontType[m_nFontSize]->m_dwSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
	m_dwFontColor = m_vtFontType[m_nFontSize]->m_dwColor;
	
	UpdateFont();
}

void CTFontDialogView::OnEnChangeEditPosX()
{
	UpdateFont();
}

void CTFontDialogView::OnEnChangeEditPosY()
{
	UpdateFont();
}

void CTFontDialogView::OnBnClickedBtnImgLoad()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,_T("All Files(*.*)|*.*||"),this);
	if(dlg.DoModal()==IDOK)
	{
		CString str = dlg.GetPathName();
		CMainFrame* pMainWnd = (CMainFrame*) AfxGetMainWnd();

		pMainWnd->LoadImage( str );
	}
}
