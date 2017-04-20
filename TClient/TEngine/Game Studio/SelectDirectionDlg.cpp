// SelectDirection.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SelectDirectionDlg.h"
#include ".\selectdirectiondlg.h"


// CSelectDirection 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelectDirectionDlg, CDialog)
CSelectDirectionDlg::CSelectDirectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDirectionDlg::IDD, pParent)
{
	for( int i = 0 ; i < _FAR_IMG_CNT ; i++ )
		m_bImgDirection[i] = FALSE;
	m_pDoc = NULL;
}

CSelectDirectionDlg::CSelectDirectionDlg(CEXTMAPDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDirectionDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	for( int i = 0 ; i < _FAR_IMG_CNT ; i++ )
	{
		m_bImgDirection[i] = FALSE;
		m_fAngle[i] = FLOAT(i + 1) * BASIC_FAR_DIR_ANG;
		m_pFARIMG[i].ResetFARIMGPT();
	}
}

CSelectDirectionDlg::~CSelectDirectionDlg()
{
}

void CSelectDirectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ANG_1, m_bImgDirection[0]);
	DDX_Check(pDX, IDC_CHECK_ANG_2, m_bImgDirection[1]);
	DDX_Check(pDX, IDC_CHECK_ANG_3, m_bImgDirection[2]);
	DDX_Check(pDX, IDC_CHECK_ANG_4, m_bImgDirection[3]);
	DDX_Check(pDX, IDC_CHECK_ANG_5, m_bImgDirection[4]);
	DDX_Check(pDX, IDC_CHECK_ANG_6, m_bImgDirection[5]);
	DDX_Check(pDX, IDC_CHECK_ANG_7, m_bImgDirection[6]);
	DDX_Check(pDX, IDC_CHECK_ANG_8, m_bImgDirection[7]);
	DDX_Check(pDX, IDC_CHECK_ANG_9, m_bImgDirection[8]);
	DDX_Check(pDX, IDC_CHECK_ANG_10, m_bImgDirection[9]);
	DDX_Check(pDX, IDC_CHECK_ANG_11, m_bImgDirection[10]);
	DDX_Check(pDX, IDC_CHECK_ANG_12, m_bImgDirection[11]);
	DDX_Check(pDX, IDC_CHECK_ANG_13, m_bImgDirection[12]);
	DDX_Check(pDX, IDC_CHECK_ANG_14, m_bImgDirection[13]);
	DDX_Check(pDX, IDC_CHECK_ANG_15, m_bImgDirection[14]);
	DDX_Check(pDX, IDC_CHECK_ANG_16, m_bImgDirection[15]);
	DDX_Check(pDX, IDC_CHECK_ANG_17, m_bImgDirection[16]);
	DDX_Check(pDX, IDC_CHECK_ANG_18, m_bImgDirection[17]);
	DDX_Check(pDX, IDC_CHECK_ANG_19, m_bImgDirection[18]);
	DDX_Check(pDX, IDC_CHECK_ANG_20, m_bImgDirection[19]);
	DDX_Check(pDX, IDC_CHECK_ANG_21, m_bImgDirection[20]);
	DDX_Check(pDX, IDC_CHECK_ANG_22, m_bImgDirection[21]);
	DDX_Check(pDX, IDC_CHECK_ANG_23, m_bImgDirection[22]);
	DDX_Check(pDX, IDC_CHECK_ANG_24, m_bImgDirection[23]);
	DDX_Check(pDX, IDC_CHECK_ANG_25, m_bImgDirection[24]);
	DDX_Check(pDX, IDC_CHECK_ANG_26, m_bImgDirection[25]);
	DDX_Check(pDX, IDC_CHECK_ANG_27, m_bImgDirection[26]);
	DDX_Check(pDX, IDC_CHECK_ANG_28, m_bImgDirection[27]);
	DDX_Check(pDX, IDC_CHECK_ANG_29, m_bImgDirection[28]);
	DDX_Check(pDX, IDC_CHECK_ANG_30, m_bImgDirection[29]);
	DDX_Check(pDX, IDC_CHECK_ANG_31, m_bImgDirection[30]);
	DDX_Check(pDX, IDC_CHECK_ANG_32, m_bImgDirection[31]);
	DDX_Radio(pDX, IDC_RADIO_DIR_FOUR, m_nDir);
}

BEGIN_MESSAGE_MAP(CSelectDirectionDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ANG_1, OnBnClickedCheckAng1)
	ON_BN_CLICKED(IDC_CHECK_ANG_2, OnBnClickedCheckAng2)
	ON_BN_CLICKED(IDC_CHECK_ANG_3, OnBnClickedCheckAng3)
	ON_BN_CLICKED(IDC_CHECK_ANG_4, OnBnClickedCheckAng4)
	ON_BN_CLICKED(IDC_CHECK_ANG_5, OnBnClickedCheckAng5)
	ON_BN_CLICKED(IDC_CHECK_ANG_6, OnBnClickedCheckAng6)
	ON_BN_CLICKED(IDC_CHECK_ANG_7, OnBnClickedCheckAng7)
	ON_BN_CLICKED(IDC_CHECK_ANG_8, OnBnClickedCheckAng8)
	ON_BN_CLICKED(IDC_CHECK_ANG_9, OnBnClickedCheckAng9)
	ON_BN_CLICKED(IDC_CHECK_ANG_10, OnBnClickedCheckAng10)
	ON_BN_CLICKED(IDC_CHECK_ANG_11, OnBnClickedCheckAng11)
	ON_BN_CLICKED(IDC_CHECK_ANG_12, OnBnClickedCheckAng12)
	ON_BN_CLICKED(IDC_CHECK_ANG_13, OnBnClickedCheckAng13)
	ON_BN_CLICKED(IDC_CHECK_ANG_14, OnBnClickedCheckAng14)
	ON_BN_CLICKED(IDC_CHECK_ANG_15, OnBnClickedCheckAng15)
	ON_BN_CLICKED(IDC_CHECK_ANG_16, OnBnClickedCheckAng16)
	ON_BN_CLICKED(IDC_CHECK_ANG_17, OnBnClickedCheckAng17)
	ON_BN_CLICKED(IDC_CHECK_ANG_18, OnBnClickedCheckAng18)
	ON_BN_CLICKED(IDC_CHECK_ANG_19, OnBnClickedCheckAng19)
	ON_BN_CLICKED(IDC_CHECK_ANG_20, OnBnClickedCheckAng20)
	ON_BN_CLICKED(IDC_CHECK_ANG_21, OnBnClickedCheckAng21)
	ON_BN_CLICKED(IDC_CHECK_ANG_22, OnBnClickedCheckAng22)
	ON_BN_CLICKED(IDC_CHECK_ANG_23, OnBnClickedCheckAng23)
	ON_BN_CLICKED(IDC_CHECK_ANG_24, OnBnClickedCheckAng24)
	ON_BN_CLICKED(IDC_CHECK_ANG_25, OnBnClickedCheckAng25)
	ON_BN_CLICKED(IDC_CHECK_ANG_26, OnBnClickedCheckAng26)
	ON_BN_CLICKED(IDC_CHECK_ANG_27, OnBnClickedCheckAng27)
	ON_BN_CLICKED(IDC_CHECK_ANG_28, OnBnClickedCheckAng28)
	ON_BN_CLICKED(IDC_CHECK_ANG_29, OnBnClickedCheckAng29)
	ON_BN_CLICKED(IDC_CHECK_ANG_30, OnBnClickedCheckAng30)
	ON_BN_CLICKED(IDC_CHECK_ANG_31, OnBnClickedCheckAng31)
	ON_BN_CLICKED(IDC_CHECK_ANG_32, OnBnClickedCheckAng32)
	ON_BN_CLICKED(ID_MAKE_FAR_IMAGE, OnBnClickedMakeFarImage)
	ON_BN_CLICKED(IDC_RADIO_DIR_FOUR, OnBnClickedRadioDirFour)
	ON_BN_CLICKED(IDC_RADIO_DIR_EIGHT, OnBnClickedRadioDirEight)
	ON_BN_CLICKED(IDC_RADIO_DIR_SIXTEEN, OnBnClickedRadioDirSixteen)
	ON_BN_CLICKED(IDC_RADIO_THIRTYTWO, OnBnClickedRadioThirtytwo)
	ON_BN_CLICKED(IDC_RADIO_DIR_FOURX, OnBnClickedRadioDirFourx)
END_MESSAGE_MAP()


void CSelectDirectionDlg::OnBnClickedCheckAng1()
{
	OnChangeCheckState(_FDA_1);
}

void CSelectDirectionDlg::OnBnClickedCheckAng2()
{
	OnChangeCheckState(_FDA_2);
}

void CSelectDirectionDlg::OnBnClickedCheckAng3()
{
	OnChangeCheckState(_FDA_3);
}

void CSelectDirectionDlg::OnBnClickedCheckAng4()
{
	OnChangeCheckState(_FDA_4);
}

void CSelectDirectionDlg::OnBnClickedCheckAng5()
{
	OnChangeCheckState(_FDA_5);
}

void CSelectDirectionDlg::OnBnClickedCheckAng6()
{
	OnChangeCheckState(_FDA_6);
}

void CSelectDirectionDlg::OnBnClickedCheckAng7()
{
	OnChangeCheckState(_FDA_7);
}

void CSelectDirectionDlg::OnBnClickedCheckAng8()
{
	OnChangeCheckState(_FDA_8);
}

void CSelectDirectionDlg::OnBnClickedCheckAng9()
{
	OnChangeCheckState(_FDA_9);
}

void CSelectDirectionDlg::OnBnClickedCheckAng10()
{
	OnChangeCheckState(_FDA_10);
}

void CSelectDirectionDlg::OnBnClickedCheckAng11()
{
	OnChangeCheckState(_FDA_11);
}

void CSelectDirectionDlg::OnBnClickedCheckAng12()
{
	OnChangeCheckState(_FDA_12);
}

void CSelectDirectionDlg::OnBnClickedCheckAng13()
{
	OnChangeCheckState(_FDA_13);
}

void CSelectDirectionDlg::OnBnClickedCheckAng14()
{
	OnChangeCheckState(_FDA_14);
}

void CSelectDirectionDlg::OnBnClickedCheckAng15()
{
	OnChangeCheckState(_FDA_15);
}

void CSelectDirectionDlg::OnBnClickedCheckAng16()
{
	OnChangeCheckState(_FDA_16);
}

void CSelectDirectionDlg::OnBnClickedCheckAng17()
{
	OnChangeCheckState(_FDA_17);
}

void CSelectDirectionDlg::OnBnClickedCheckAng18()
{
	OnChangeCheckState(_FDA_18);
}

void CSelectDirectionDlg::OnBnClickedCheckAng19()
{
	OnChangeCheckState(_FDA_19);
}

void CSelectDirectionDlg::OnBnClickedCheckAng20()
{
	OnChangeCheckState(_FDA_20);
}

void CSelectDirectionDlg::OnBnClickedCheckAng21()
{
	OnChangeCheckState(_FDA_21);
}

void CSelectDirectionDlg::OnBnClickedCheckAng22()
{
	OnChangeCheckState(_FDA_22);
}

void CSelectDirectionDlg::OnBnClickedCheckAng23()
{
	OnChangeCheckState(_FDA_23);
}

void CSelectDirectionDlg::OnBnClickedCheckAng24()
{
	OnChangeCheckState(_FDA_24);
}

void CSelectDirectionDlg::OnBnClickedCheckAng25()
{
	OnChangeCheckState(_FDA_25);
}

void CSelectDirectionDlg::OnBnClickedCheckAng26()
{
	OnChangeCheckState(_FDA_26);
}

void CSelectDirectionDlg::OnBnClickedCheckAng27()
{
	OnChangeCheckState(_FDA_27);
}

void CSelectDirectionDlg::OnBnClickedCheckAng28()
{
	OnChangeCheckState(_FDA_28);
}

void CSelectDirectionDlg::OnBnClickedCheckAng29()
{
	OnChangeCheckState(_FDA_29);
}

void CSelectDirectionDlg::OnBnClickedCheckAng30()
{
	OnChangeCheckState(_FDA_30);
}

void CSelectDirectionDlg::OnBnClickedCheckAng31()
{
	OnChangeCheckState(_FDA_31);
}

void CSelectDirectionDlg::OnBnClickedCheckAng32()
{
	OnChangeCheckState(_FDA_32);
}

void CSelectDirectionDlg::OnChangeCheckState(BYTE nANG)
{
	m_pDoc->m_fFarImageDir = m_fAngle[ nANG ];
	m_pDoc->RedrawAllView();
	
	UpdateData();
	m_bImgDirection[ nANG ] ? m_pFARIMG[ nANG ] = m_pDoc->m_pFARIMG : m_pFARIMG[ nANG ].ResetFARIMGPT();
}

void CSelectDirectionDlg::OnBnClickedMakeFarImage()
{
	BYTE bCnt = 0;
	if( m_pDoc->m_vSELTILE.empty() )
	{
		AfxMessageBox(_T("To Select The Tile First"),MB_OK|MB_ICONHAND);
		return;
	}

	for( BYTE i = 0 ; i < _FAR_IMG_CNT ; i++ )
		if( m_bImgDirection[i] )
			bCnt++;

	if( !bCnt )
	{
		AfxMessageBox(_T("To Select The Angle First"),MB_OK|MB_ICONHAND);
		return;
	}

	if( AfxMessageBox(_T("Do U Want Make the Far Image?"),MB_OKCANCEL) == IDOK )
	{
		m_pDoc->CreatFarImage(m_pFARIMG, m_bImgDirection);
		this->EndDialog(TRUE);
	}
}
void CSelectDirectionDlg::OnBnClickedRadioDirFour()
{
	ChangeFixDirection();	
}

void CSelectDirectionDlg::OnBnClickedRadioDirFourx()
{
	ChangeFixDirection();
}

void CSelectDirectionDlg::OnBnClickedRadioDirEight()
{
	ChangeFixDirection();
}

void CSelectDirectionDlg::OnBnClickedRadioDirSixteen()
{
	ChangeFixDirection();
}

void CSelectDirectionDlg::OnBnClickedRadioThirtytwo()
{
	ChangeFixDirection();
}
void CSelectDirectionDlg::ChangeFixDirection(void)
{
	UpdateData();
	BYTE bStd = 0;
	for( BYTE i = 0 ; i < _FAR_IMG_CNT ; i++ )
		m_bImgDirection[i] = FALSE;
	UpdateData(FALSE);
	switch( m_nDir )
	{
	case 0:
		bStd = 8;
		break;
	case 1:
		bStd = 4;
		break;
	case 2:
		bStd = 4;
		break;
	case 3:
		bStd = 2;
		break;
	case 4:
		bStd = 1;
		break;
	}

	if( m_nDir == 1 )
	{
		for( BYTE i = 1 ; i < _FAR_IMG_CNT + 1 ; i++ )
			if( i == bStd )	
			{
				m_bImgDirection[i - 1] = TRUE;
				bStd += 8;
			}
	}
	else
		for( BYTE i = 1 ; i < _FAR_IMG_CNT + 1 ; i++ )
			if( i % bStd == 0 )
				m_bImgDirection[i - 1] = TRUE;
	UpdateData(FALSE);
}

















