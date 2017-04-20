#include "Stdafx.h"
#include "TGuildMarkMakeDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
CTGuildMarkMakeDlg::CTGuildMarkMakeDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc)
{
	static const DWORD dwMIDDLE[] = 
	{
		ID_CTRLINST_MARK_TYPE,			///< ¹®¾ç
		ID_CTRLINST_BACK_TYPE,			///< ¹ÙÅÁ
		ID_CTRLINST_MCOL_BAR,			///< ¹®¾ç»ö
		ID_CTRLINST_BCOL1_BAR,			///< ¹ÙÅÁ»ö1
		ID_CTRLINST_BCOL2_BAR,			///< ¹ÙÅÁ»ö2
	};

	static const DWORD dwLEFT[] = 
	{
		ID_CTRLINST_MARK_LB,			///< ¹®¾ç
		ID_CTRLINST_BACK_LB,			///< ¹ÙÅÁ
		ID_CTRLINST_MCOL_LB,			///< ¹®¾ç»ö
		ID_CTRLINST_BCOL1_LB,			///< ¹ÙÅÁ»ö1
		ID_CTRLINST_BCOL2_LB,			///< ¹ÙÅÁ»ö2
	};

	static const DWORD dwRIGHT[] = 
	{
		ID_CTRLINST_MARK_RB,			///< ¹®¾ç
		ID_CTRLINST_BACK_RB,			///< ¹ÙÅÁ
		ID_CTRLINST_MCOL_RB,			///< ¹®¾ç»ö
		ID_CTRLINST_BCOL1_RB,			///< ¹ÙÅÁ»ö1
		ID_CTRLINST_BCOL2_RB,			///< ¹ÙÅÁ»ö2
	};


	for(int i=0; i<TYPE_COUNT; ++i)
	{
		m_pLeft[i]	 = FindKid(dwLEFT[i]);
		m_pRight[i]  = FindKid(dwRIGHT[i]);
		m_pMiddle[i] = FindKid(dwMIDDLE[i]);
	}

	m_pMarkImgset = static_cast<TImageList*>( FindKid(ID_CTRLINST_MARK) );
	m_pBackImgset = static_cast<TImageList*>( FindKid(ID_CTRLINST_MARK_BACK) );
	m_pBackBase = FindKid(ID_CTRLINST_MARK_BASE);

	m_pMarkImgset->EnableUserColor(TRUE);
	m_pBackImgset->EnableUserColor(TRUE);

	TCOMP_LIST::iterator itr;

	itr = m_pMarkImgset->GetFirstImgsFinder();
	while( !m_pMarkImgset->EndOfImgs(itr) )
	{
		TComponent* pKID = *itr;
		LPIMAGESET pImgSet = pKID->GetDefaultImage();

		CD3DImage* pDXIMG = pImgSet->GetImage();
		CTClientGuildMark::MAddFrontImage(pDXIMG);

		m_pMarkImgset->GetNextImg(itr);
	}

	itr = m_pBackImgset->GetFirstImgsFinder();
	while( !m_pBackImgset->EndOfImgs(itr) )
	{
		TComponent* pKID = *itr;
		LPIMAGESET pImgSet = pKID->GetDefaultImage();

		CD3DImage* pDXIMG = pImgSet->GetImage();
		CTClientGuildMark::MAddBackImage(pDXIMG);

		m_pBackImgset->GetNextImg(itr);
	}

	m_pGuildMark = new CTClientGuildMark(0,0, 0,0,0);

	Init();
}
// --------------------------------------------------------------------
CTGuildMarkMakeDlg::~CTGuildMarkMakeDlg()
{
	delete m_pGuildMark;
}
// ====================================================================

// ====================================================================
void CTGuildMarkMakeDlg::Init()
{
	SetMark(0);
	SetBack(0);
	
	SetMarkColor(0);
	SetBackColor1(0);
	SetBackColor2(0);
}
// ====================================================================
void CTGuildMarkMakeDlg::SetMark(BYTE bID)
{
	m_pGuildMark->SetFront(bID);
	m_pMarkImgset->SetCurImage(bID);

	m_pMiddle[TYPE_MARK]->m_strText = CTChart::Format( TSTR_FMT_TYPE, bID);
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::SetBack(BYTE bID)
{
	m_pGuildMark->SetBack(bID);
	m_pBackImgset->SetCurImage(bID);

	m_pMiddle[TYPE_BACK]->m_strText = CTChart::Format( TSTR_FMT_TYPE, bID);
}
// ====================================================================
void CTGuildMarkMakeDlg::SetMarkColor(BYTE bColor)
{
	m_pGuildMark->SetFrontColorID(bColor);
	DWORD dwColor = m_pGuildMark->GetFrontColor();
	m_pMarkImgset->SetUserColor(dwColor);
	m_pMiddle[TYPE_MARKCOLOR]->m_dwColor = dwColor;
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::SetBackColor1(BYTE bColor)
{
	m_pGuildMark->SetBackColorID1(bColor);
	m_pBackBase->m_dwColor = m_pGuildMark->GetBackColor1();
	
	m_pMiddle[TYPE_BACKCOLOR1]->m_dwColor = m_pBackBase->m_dwColor;
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::SetBackColor2(BYTE bColor)
{
	m_pGuildMark->SetBackColorID2(bColor);
	DWORD dwColor = m_pGuildMark->GetBackColor2();
	m_pBackImgset->SetUserColor(dwColor);
	m_pMiddle[TYPE_BACKCOLOR2]->m_dwColor = dwColor;
}
// ====================================================================
void CTGuildMarkMakeDlg::MovMark(BOOL bLeft)
{
	BYTE bMARK = GetMark();

	if( bLeft )
		bMARK = (bMARK > 0 )? bMARK-1: TMAXGUILD_MARK-1;
	else
		bMARK = (bMARK < TMAXGUILD_MARK-1)? bMARK+1: 0;

	SetMark(bMARK);
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::MovBack(BOOL bLeft)
{
	BYTE bBACK = GetBack();

	if( bLeft )
		bBACK = (bBACK > 0 )? bBACK-1: TMAXGUILD_MARKBACK-1;
	else
		bBACK = (bBACK < TMAXGUILD_MARKBACK-1)? bBACK+1: 0;

	SetBack(bBACK);
}
// ====================================================================
void CTGuildMarkMakeDlg::MovMarkColor(BOOL bLeft)
{
	BYTE bCOLOR = GetMarkColor();

	if( bLeft )
		bCOLOR = (bCOLOR > 0 )? bCOLOR-1: TMAXGUILD_MARK_COLOR-1;
	else
		bCOLOR = (bCOLOR < TMAXGUILD_MARK_COLOR-1)? bCOLOR+1: 0;

	SetMarkColor(bCOLOR);
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::MovBackColor1(BOOL bLeft)
{
	BYTE bCOLOR = GetBackColor1();

	if( bLeft )
		bCOLOR = (bCOLOR > 0 )? bCOLOR-1: TMAXGUILD_MARK_COLOR-1;
	else
		bCOLOR = (bCOLOR < TMAXGUILD_MARK_COLOR-1)? bCOLOR+1: 0;

	SetBackColor1(bCOLOR);
}
// --------------------------------------------------------------------
void CTGuildMarkMakeDlg::MovBackColor2(BOOL bLeft)
{
	BYTE bCOLOR = GetBackColor2();

	if( bLeft )
		bCOLOR = (bCOLOR > 0 )? bCOLOR-1: TMAXGUILD_MARK_COLOR-1;
	else
		bCOLOR = (bCOLOR < TMAXGUILD_MARK_COLOR-1)? bCOLOR+1: 0;

	SetBackColor2(bCOLOR);
}
// ====================================================================

// ====================================================================
void CTGuildMarkMakeDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	BOOL bLEFT;
	INT nTYPE;

	for( nTYPE=0; nTYPE<TYPE_COUNT; ++nTYPE)
	{
		if( m_pLeft[nTYPE]->HitTest(pt) )
		{
			bLEFT = TRUE;
			break;
		}
		
		if( m_pRight[nTYPE]->HitTest(pt) )
		{
			bLEFT = FALSE;
			break;
		}
	}

	switch(nTYPE)
	{
	case TYPE_MARK			: MovMark(bLEFT); break;
	case TYPE_BACK			: MovBack(bLEFT) ; break;		
	case TYPE_MARKCOLOR		: MovMarkColor(bLEFT); break;
	case TYPE_BACKCOLOR1	: MovBackColor1(bLEFT); break;
	case TYPE_BACKCOLOR2	: MovBackColor2(bLEFT); break;
	}						 

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}
// ====================================================================
void CTGuildMarkMakeDlg::ShowComponent( BOOL bVisible)
{
	if( bVisible && !m_bVisible )
	{
		BYTE bMark, bBack;
		BYTE bMarkCol, bBackCol1, bBackCol2;

		CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
		if( pMainChar->m_pGuildMark )
		{
			CTClientGuildMark* pGuildMark = pMainChar->m_pGuildMark;

			bMark		= pGuildMark->GetFront();
			bBack		= pGuildMark->GetBack();

			bMarkCol	= pGuildMark->GetFrontColorID();
			bBackCol1	= pGuildMark->GetBackColorID1();
			bBackCol2	= pGuildMark->GetBackColorID2();
		}
		else
		{
			bMark		= ::rand() % TMAXGUILD_MARK;
			bBack		= ::rand() % TMAXGUILD_MARKBACK;

			bMarkCol	= ::rand() % TMAXGUILD_MARK_COLOR;
			bBackCol1	= ::rand() % TMAXGUILD_MARK_COLOR;
			bBackCol2	= ::rand() % TMAXGUILD_MARK_COLOR;
		}

		SetMark(bMark);
		SetBack(bBack);

		SetMarkColor(bMarkCol);
		SetBackColor1(bBackCol1);
		SetBackColor2(bBackCol2);
	}

	CTClientUIBase::ShowComponent(bVisible);
}
// ====================================================================















