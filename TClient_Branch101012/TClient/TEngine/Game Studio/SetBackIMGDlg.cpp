// SetBackIMGDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SetBackIMGDlg.h"
#include ".\setbackimgdlg.h"

#include "GridView.h"

// CSetBackIMGDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetBackIMGDlg, CDialog)
CSetBackIMGDlg::CSetBackIMGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBackIMGDlg::IDD, pParent)
	, m_strDist(_T(""))
	, m_strRateSizeX(_T(""))
	, m_strRateSizeY(_T(""))
	, m_bFixSizeScreen(FALSE)
	, m_bConstrain(FALSE)
	, m_nDist(0)
	, m_strSizeX(_T(""))
	, m_strSizeY(_T(""))
	, m_bPOSFIX(FALSE)
{
}

CSetBackIMGDlg::~CSetBackIMGDlg()
{
}

void CSetBackIMGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text   (pDX, IDC_EDIT_DIST, m_strDist);
	DDX_Control(pDX, IDC_EDIT_DIST, m_cDISTEDIT);
	DDX_Scroll (pDX, IDC_SCROLL_DIST, m_nDist);
	DDX_Control(pDX, IDC_SCROLL_DIST, m_cDistScroll);

	DDX_Text   (pDX, IDC_EDIT_RATE_SIZEX, m_strRateSizeX);
	DDX_Text   (pDX, IDC_EDIT_RATE_SIZEY, m_strRateSizeY);
	DDX_Control(pDX, IDC_EDIT_RATE_SIZEX, m_cRateSizeXEDIT);
	DDX_Control(pDX, IDC_EDIT_RATE_SIZEY, m_cRateSizeYEDIT);

	DDX_Control(pDX, IDC_SCROLL_SIZEX, m_cSizeXScroll);
	DDX_Control(pDX, IDC_SCROLL_SIZEY, m_cSizeYScroll);

	DDX_Check  (pDX, IDC_CHECK_SCREENFIX, m_bFixSizeScreen);
	DDX_Control(pDX, IDC_CHECK_CONSTRAIN, m_cConstrain);
	DDX_Check  (pDX, IDC_CHECK_CONSTRAIN, m_bConstrain);

	DDX_Text   (pDX, IDC_EDIT_SIZEX, m_strSizeX);
	DDX_Text   (pDX, IDC_EDIT_SIZEY, m_strSizeY);
	DDX_Control(pDX, IDC_EDIT_SIZEX, m_cSizeXEDIT);
	DDX_Control(pDX, IDC_EDIT_SIZEY, m_cSizeYEDIT);
	DDX_Control(pDX, IDC_CHECK_FIXPOS, m_cPOSFIX);
	DDX_Check  (pDX, IDC_CHECK_FIXPOS, m_bPOSFIX);
}


BEGIN_MESSAGE_MAP(CSetBackIMGDlg, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_SCREENFIX, OnBnClickedCheckScreenfix)
	ON_BN_CLICKED(IDC_CHECK_CONSTRAIN, OnBnClickedCheckConstrain)
	ON_EN_CHANGE(IDC_EDIT_SIZEX, OnChangeSizeX)
	ON_EN_CHANGE(IDC_EDIT_SIZEY, OnChangeSizeY)
	ON_BN_CLICKED(IDC_CHECK_FIXPOS, OnBnClickedCheckFixpos)
END_MESSAGE_MAP()


// CSetBackIMGDlg 메시지 처리기입니다.

void CSetBackIMGDlg::InitData(LPBACKIMAGE pBackIMG, LPVOID pWnd)
{
	m_pBackIMG = pBackIMG;

	m_cDistScroll.SetScrollRange(0,3000);
	m_cDistScroll.SetScrollPos(INT(m_pBackIMG->m_fDist));
	m_nDist = INT(m_pBackIMG->m_fDist);
	m_strDist.Format("%d", INT(m_pBackIMG->m_fDist));

	m_cSizeXScroll.SetScrollRange(1, 200);
	m_cSizeXScroll.SetScrollPos(INT(m_pBackIMG->m_fSizeRateX * 100));
	m_strRateSizeX.Format("%.2f", m_pBackIMG->m_fSizeRateX);

	m_cSizeYScroll.SetScrollRange(1, 200);
	m_cSizeYScroll.SetScrollPos(INT(m_pBackIMG->m_fSizeRateY * 100));
	m_strRateSizeY.Format("%.2f", m_pBackIMG->m_fSizeRateY);

	m_strSizeX.Format("%d", INT(m_pBackIMG->m_fSizeRateX * m_pBackIMG->m_nSizeX));
	m_strSizeY.Format("%d", INT(m_pBackIMG->m_fSizeRateY * m_pBackIMG->m_nSizeY));

	m_pVIEW = pWnd;

	m_bFixSizeScreen = m_pBackIMG->m_bFixSizeScreen;

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CSetBackIMGDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMinPos, nMaxPos;
	int nCurPos = pScrollBar->GetScrollPos();

	pScrollBar->GetScrollRange(&nMinPos, &nMaxPos); 
	nMaxPos = pScrollBar->GetScrollLimit();

	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		nCurPos = nMinPos;
		break;

	case SB_RIGHT:      // Scroll to far right.
		nCurPos = nMaxPos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINELEFT:      // Scroll left.
		if (nCurPos > nMinPos)
			nCurPos--;
		break;

	case SB_LINERIGHT:   // Scroll right.
		if (nCurPos < nMaxPos)
			nCurPos++;
		break;

	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);

			if (nCurPos > nMinPos)
				nCurPos = max(nMinPos, nCurPos - (int) info.nPage);
		}
		break;

	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			pScrollBar->GetScrollInfo(&info, SIF_ALL);

			if (nCurPos < nMaxPos)
				nCurPos = min(nMaxPos, nCurPos + (int) info.nPage);
		}
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		nCurPos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		nCurPos = nPos;     // position that the scroll box has been dragged to.
		break;
	}

	if((DWORD_PTR) pScrollBar == (DWORD_PTR) &m_cDistScroll)
		SetDistScrollPos(nCurPos);
	else if((DWORD_PTR) pScrollBar == (DWORD_PTR) &m_cSizeXScroll)
	{
		SetSizeXScrollPos(nCurPos);
		if(m_bConstrain)
		{
			FLOAT fRATE = FLOAT(m_pBackIMG->m_nSizeY) / FLOAT(m_pBackIMG->m_nSizeX);
			
			SetSizeYScrollPos(nCurPos);
		}
	}
	else if((DWORD_PTR) pScrollBar == (DWORD_PTR) &m_cSizeYScroll)
	{
		SetSizeYScrollPos(nCurPos);
		if(m_bConstrain)
		{
			FLOAT fRATE = FLOAT(m_pBackIMG->m_nSizeX) / FLOAT(m_pBackIMG->m_nSizeY);

			SetSizeXScrollPos(nCurPos);
		}
	}

	((CGridView*)m_pVIEW)->Render();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSetBackIMGDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}
void CSetBackIMGDlg::SetDistScrollPos(UINT nPos)
{
	m_nDist = nPos;
	
	m_strDist.Format("%d", m_nDist);
	
	m_cDistScroll.SetScrollPos(m_nDist);

	UpdateData(FALSE);
	m_pBackIMG->m_fDist = FLOAT(m_nDist);
}

void CSetBackIMGDlg::SetSizeXScrollPos(UINT nSizeX)
{
	m_pBackIMG->m_fSizeRateX = FLOAT(nSizeX) / 100.0f;
	m_strRateSizeX.Format("%.2f", m_pBackIMG->m_fSizeRateX);

	m_cSizeXScroll.SetScrollPos(nSizeX);

	UpdateData(FALSE);
}

void CSetBackIMGDlg::SetSizeYScrollPos(UINT nSizeY)
{
	m_pBackIMG->m_fSizeRateY = FLOAT(nSizeY) / 100.0f;
	m_strRateSizeY.Format("%.2f", m_pBackIMG->m_fSizeRateY);

	m_cSizeYScroll.SetScrollPos(nSizeY);

	UpdateData(FALSE);
}

void CSetBackIMGDlg::EnableAllCtrl(BYTE bEnable)
{
	m_cSizeXScroll.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);
	m_cSizeYScroll.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);

	m_cRateSizeXEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);
	m_cRateSizeYEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);

	m_cRateSizeXEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);
	m_cRateSizeYEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);

	m_cSizeXEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);
	m_cSizeYEDIT.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);

	m_cConstrain.EnableWindow(!m_bFixSizeScreen && !m_bPOSFIX);
}

void CSetBackIMGDlg::OnBnClickedCheckScreenfix()
{
	UpdateData();
	m_pBackIMG->m_bFixSizeScreen = m_bFixSizeScreen;
	EnableAllCtrl(TRUE);

	((CGridView*)m_pVIEW)->Render();
}

void CSetBackIMGDlg::OnBnClickedCheckConstrain()
{
	UpdateData();
}

void CSetBackIMGDlg::OnChangeSizeX()
{
	UpdateData();

	int nSize = 0;
	sscanf(m_strSizeX,"%d",&nSize);

	INT nRate = INT((FLOAT(nSize) / FLOAT(m_pBackIMG->m_nSizeX)) * 100);
	SetSizeXScrollPos(nRate);
	
	if(m_bConstrain)
	{
		m_strSizeY.Format("%d", INT((FLOAT(nRate) / 100.0f) * FLOAT(m_pBackIMG->m_nSizeY)) );
		SetSizeYScrollPos(nRate);
	}

	((CGridView*)m_pVIEW)->Render();
}

void CSetBackIMGDlg::OnChangeSizeY()
{
	UpdateData();

	int nSize = 0;
	sscanf(m_strSizeY,"%d",&nSize);

	INT nRate = INT((FLOAT(nSize) / FLOAT(m_pBackIMG->m_nSizeY)) * 100);
	SetSizeYScrollPos(nRate);

	if(m_bConstrain)
	{
		m_strSizeX.Format("%d", INT((FLOAT(nRate) / 100.0f) * FLOAT(m_pBackIMG->m_nSizeX)) );
		SetSizeXScrollPos(nRate);
		
	}
	((CGridView*)m_pVIEW)->Render();
}

void CSetBackIMGDlg::OnChangeSize()
{
	((CGridView*)m_pVIEW)->Render();
}

void CSetBackIMGDlg::OnBnClickedCheckFixpos()
{
	UpdateData();
	m_pBackIMG->m_bPosFix = m_bPOSFIX;
	EnableAllCtrl(TRUE);
}
