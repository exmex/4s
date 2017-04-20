// MeshBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "MeshBar.h"
#include "MeshDoc.h"
#include ".\meshbar.h"


// CMeshBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshBar, CBarContent)
CMeshBar::CMeshBar(CWnd* pParent /*=NULL*/)
: CBarContent(CMeshBar::IDD, pParent)
, m_bTestMode(FALSE)
, m_strCamDist(_T(""))
, m_dwMeshCount(0)
, m_bLevelCount(0)
, m_dwTotalCount(0)
, m_dwCount(0)
, m_strDist(_T(""))
, m_nPolyLevel(0)
, m_dwLevel(0)
, m_strPolyLevel(_T(""))
, m_bUseVB(FALSE)
{
	m_pItem = NULL;
}

CMeshBar::~CMeshBar()
{
}

void CMeshBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_EDITMODE, m_bTestMode);
	DDX_Text(pDX, IDC_STATIC_CAMDIST, m_strCamDist);
	DDX_Control(pDX, IDC_BUTTON_SETDIST, m_cSetDist);
	DDX_Text(pDX, IDC_STATIC_MESHCOUNT, m_dwMeshCount);
	DDX_Text(pDX, IDC_STATIC_LEVELCOUNT, m_bLevelCount);
	DDX_Text(pDX, IDC_STATIC_POLYCOUNT, m_dwTotalCount);
	DDX_Text(pDX, IDC_STATIC_SELECTEDCOUNT, m_dwCount);
	DDX_Control(pDX, IDC_BUTTON_ADDLEVEL, m_cAddLevel);
	DDX_Control(pDX, IDC_BUTTON_DELLEVEL, m_cDelLevel);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ALL, m_cSelectAll);
	DDX_Control(pDX, IDC_BUTTON_UNSELECT_ALL, m_cUnselectAll);
	DDX_Control(pDX, IDC_BUTTON_SELECT_INV, m_cInvSelect);
	DDX_Control(pDX, IDC_BUTTON_LOAD_FROM_TMF, m_cLoadFromTMF);
	DDX_Control(pDX, IDC_SPIN_LEVEL, m_spinLevel);
	DDX_Control(pDX, IDC_EDIT_DIST, m_cDist);
	DDX_Text(pDX, IDC_EDIT_DIST, m_strDist);
	DDX_Control(pDX, IDC_SPIN_DIST, m_spinDist);
	DDX_Control(pDX, IDC_SLIDER_POLYGON, m_cPolyCtrl);
	DDX_Slider(pDX, IDC_SLIDER_POLYGON, m_nPolyLevel);
	DDX_Text(pDX, IDC_STATIC_LEVEL, m_dwLevel);
	DDX_Control(pDX, IDC_EDIT_POLY_LEVEL, m_cPolyLevel);
	DDX_Control(pDX, IDC_SPIN_POLY_LEVEL, m_spinPolyLevel);
	DDX_Text(pDX, IDC_EDIT_POLY_LEVEL, m_strPolyLevel);
	DDX_Check(pDX, IDC_CHECK_USEVB, m_bUseVB);
	DDX_Control(pDX, IDC_CHECK_USEVB, m_cUseVB);
}


BEGIN_MESSAGE_MAP(CMeshBar, CBarContent)
	ON_BN_CLICKED(IDC_RADIO_EDITMODE, OnBnClickedRadioEditmode)
	ON_BN_CLICKED(IDC_RADIO_TESTMODE, OnBnClickedRadioTestmode)
	ON_BN_CLICKED(IDC_BUTTON_ADDLEVEL, OnBnClickedButtonAddlevel)
	ON_BN_CLICKED(IDC_BUTTON_DELLEVEL, OnBnClickedButtonDellevel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FROM_TMF, OnBnClickedButtonLoadFromTmf)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_INV, OnBnClickedButtonSelectInv)
	ON_BN_CLICKED(IDC_BUTTON_SETDIST, OnBnClickedButtonSetdist)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT_ALL, OnBnClickedButtonUnselectAll)
	ON_EN_CHANGE(IDC_EDIT_DIST, OnEnChangeEditDist)
	ON_EN_KILLFOCUS(IDC_EDIT_POLY_LEVEL, OnEnKillfocusEditPolyLevel)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_POLY_LEVEL, OnEnChangeEditPolyLevel)
	ON_BN_CLICKED(IDC_CHECK_USEVB, OnBnClickedCheckUsevb)
END_MESSAGE_MAP()


// CMeshBar 메시지 처리기입니다.

void CMeshBar::SetItem( CMeshItem *pItem)
{
	m_pItem = pItem;
	SetItemData();
}

void CMeshBar::SetItemData()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();
	DWORD dwCount = 0;

	m_dwMeshCount = m_pItem->m_Mesh.m_dwMeshCount;
	m_dwTotalCount = 0;
	m_dwCount = 0;
	m_nPolyLevel = 0;
	m_dwLevel = min( m_dwLevel, DWORD(m_pItem->m_Mesh.m_vDist.size()));

	for( int i=0; i<m_dwMeshCount; i++)
	{
		for( int j=0; j<INT(m_pItem->m_Mesh.m_pMESH[i][m_dwLevel]->m_vIB.size()); j++)
		{
			m_dwTotalCount += m_pItem->m_Mesh.m_pMESH[i][m_dwLevel]->m_vIB[j]->m_dwCount / 3;

			if(pDoc->m_pCheck[i])
				m_dwCount += m_pItem->m_Mesh.m_pMESH[i][m_dwLevel]->m_vIB[j]->m_dwCount / 3;
		}

		if( m_dwLevel && pDoc->m_pCheck[i] )
			for( j=0; j<INT(m_pItem->m_Mesh.m_pMESH[i][m_dwLevel - 1]->m_vIB.size()); j++)
				dwCount += m_pItem->m_Mesh.m_pMESH[i][m_dwLevel - 1]->m_vIB[j]->m_dwCount / 3;
	}

	if( m_dwLevel && dwCount )
		m_nPolyLevel = m_dwCount * 1000 / dwCount;

	m_strDist.Format( "%.2f", m_dwLevel > 0 ? m_pItem->m_Mesh.m_vDist[m_dwLevel - 1] : 0.0f);
	m_strPolyLevel.Format( "%.1f", ((FLOAT) m_nPolyLevel) / 10.0f);
	m_spinPolyLevel.SetPos(m_nPolyLevel);

	m_bLevelCount = BYTE(m_pItem->m_Mesh.m_vDist.size()) + 1;
	m_bUseVB = BYTE(m_pItem->m_Mesh.m_bUseVB);
	UpdateData(FALSE);

	EnableAllCtrl(!m_bTestMode);
	if(!m_bTestMode)
		pDoc->RedrawAllView();
}

void CMeshBar::SetCamDist( FLOAT fDist)
{
	m_strCamDist.Format( "%.2f", fDist);

	if(m_bTestMode)
	{
		m_dwLevel = m_pItem->m_Mesh.GetLevel(fDist);
		SetItemData();
	}
	else
		UpdateData(FALSE);
}

BOOL CMeshBar::OnInitDialog()
{
	CBarContent::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SETDIST);
	m_cSetDist.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_ADDLEVEL);
	m_cAddLevel.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DELLEVEL);
	m_cDelLevel.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SELECT_ALL);
	m_cSelectAll.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_UNSELECT_ALL);
	m_cUnselectAll.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_INVSELECT);
	m_cInvSelect.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOAD_FROM_TMF);
	m_cLoadFromTMF.SetIcon(hIcon);

	m_spinLevel.SetRange( -100, 100);
	m_spinDist.SetRange( -100, 100);

	m_spinPolyLevel.SetRange( 0, 1000);
	m_cPolyCtrl.SetRange( 0, 1000);
	m_cPolyCtrl.SetPageSize(0);
	EnableAllCtrl(!m_bTestMode);

	return TRUE;
}

void CMeshBar::EnableAllCtrl( BOOL bEnable)
{
	m_cDelLevel.EnableWindow(bEnable && m_bLevelCount > 0 && m_dwLevel > 0);
	m_spinLevel.EnableWindow(bEnable && m_bLevelCount > 0);

	m_spinPolyLevel.EnableWindow(bEnable && m_dwLevel > 0 && m_dwCount > 0);
	m_cPolyLevel.EnableWindow(bEnable && m_dwLevel > 0 && m_dwCount > 0);
	m_cPolyCtrl.EnableWindow(bEnable && m_dwLevel > 0 && m_dwCount > 0);
	m_spinDist.EnableWindow(bEnable && m_dwLevel > 0);
	m_cSetDist.EnableWindow(bEnable && m_dwLevel > 0);
	m_cDist.EnableWindow(bEnable && m_dwLevel > 0);

	m_cUnselectAll.EnableWindow(bEnable);
	m_cLoadFromTMF.EnableWindow(bEnable);
	m_cSelectAll.EnableWindow(bEnable);
	m_cInvSelect.EnableWindow(bEnable);
	m_cAddLevel.EnableWindow(bEnable);
	m_cUseVB.EnableWindow(bEnable);
}

void CMeshBar::OnBnClickedRadioEditmode()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();
	UpdateData();

	EnableAllCtrl(!m_bTestMode);
	pDoc->RedrawAllView();
}

void CMeshBar::OnBnClickedRadioTestmode()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();
	UpdateData();

	EnableAllCtrl(!m_bTestMode);
	pDoc->RedrawAllView();
}

void CMeshBar::OnBnClickedButtonAddlevel()
{
	if(m_pItem)
	{
		m_pItem->AddLevel(m_dwLevel);
		m_dwLevel++;

		SetItemData();
	}
}

void CMeshBar::OnBnClickedButtonDellevel()
{
	if(m_pItem)
	{
		m_pItem->DelLevel(m_dwLevel);
		m_dwLevel--;

		SetItemData();
	}
}

void CMeshBar::OnBnClickedButtonLoadFromTmf()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tmf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Mesh Files (*.tmf)|*.tmf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile file( dlg.GetPathName(), CFile::modeRead);
		CTachyonMesh mesh;

		DWORD dwSize = (DWORD) file.GetLength();
		LPBYTE pData = new BYTE[dwSize];

		file.Read( pData, dwSize);
		mesh.LoadMesh(
			CMainFrame::m_3DDevice.m_pDevice,
			pData);

		if( !m_pItem->m_Mesh.m_pDATA )
		{
			delete [] pData;
			return ;
		}

		INT nError = 0;

		switch(m_pItem->m_Mesh.m_pDATA->m_dwFVF)
		{
		case T3DFVF_WMESHVERTEX	:
			{
				if( m_pItem->m_Mesh.m_dwNodeCount != mesh.m_dwNodeCount ||
					m_pItem->m_Mesh.m_dwMeshCount != mesh.m_dwMeshCount )
				{
					nError = -1;
					break;
				}

				LPWMESHVERTEX pMESH = (LPWMESHVERTEX) m_pItem->m_Mesh.m_pDATA->m_pVB;
				LPWMESHVERTEX pBUF = (LPWMESHVERTEX) mesh.m_pDATA->m_pVB;

				VECTORPTR vDATA;
				vDATA.clear();

				for( DWORD i=0; i<m_pItem->m_Mesh.m_pDATA->m_dwCount; i++)
				{
					LPWMESHVERTEX pDATA = new WMESHVERTEX();

					memcpy( pDATA, &pMESH[i], sizeof(WMESHVERTEX));
					vDATA.push_back(pDATA);
				}

				for( i=0; i<m_pItem->m_Mesh.m_dwMeshCount; i++)
				{
					LPIBINFO pINFO = new IBINFO();

					for( DWORD j=0; j<DWORD(mesh.m_pMESH[i][0]->m_vIB.size()); j++)
					{
						LPIBDATA pIB = new IBDATA();

						pIB->m_dwCount = mesh.m_pMESH[i][0]->m_vIB[j]->m_dwCount;
						pIB->m_wMatrix = mesh.m_pMESH[i][0]->m_vIB[j]->m_wMatrix;
						pIB->m_pIB = new WORD[pIB->m_dwCount];

						for( DWORD k=0; k<pIB->m_dwCount; k++)
						{
							WORD wINDEX = m_pItem->FindMESH(
								&vDATA,
								&pBUF[mesh.m_pMESH[i][0]->m_vIB[j]->m_pIB[k]]);

							if( wINDEX >= WORD(vDATA.size()) )
							{
								LPWMESHVERTEX pDATA = new WMESHVERTEX();

								memcpy( pDATA, &pBUF[mesh.m_pMESH[i][0]->m_vIB[j]->m_pIB[k]], sizeof(WMESHVERTEX));
								wINDEX = WORD(vDATA.size());
								vDATA.push_back(pDATA);
							}

							pIB->m_pIB[k] = wINDEX;
						}

						pINFO->m_vIB.push_back(pIB);
					}

					delete m_pItem->m_Mesh.m_pMESH[i][m_dwLevel];
					m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] = pINFO;
				}

				delete m_pItem->m_Mesh.m_pDATA;
				m_pItem->m_Mesh.m_pDATA = new VBDATA();

				m_pItem->m_Mesh.m_pDATA->m_dwCount = DWORD(vDATA.size());
				m_pItem->m_Mesh.m_pDATA->m_dwFVF = T3DFVF_WMESHVERTEX;

				m_pItem->m_Mesh.m_pDATA->m_pVB = new BYTE[m_pItem->m_Mesh.m_pDATA->m_dwCount * sizeof(WMESHVERTEX)];
				pMESH = (LPWMESHVERTEX) m_pItem->m_Mesh.m_pDATA->m_pVB;

				for( i=0; i<DWORD(vDATA.size()); i++)
				{
					memcpy( &pMESH[i], vDATA[i], sizeof(WMESHVERTEX));
					delete vDATA[i];
				}

				vDATA.clear();
			}

			break;

		case T3DFVF_MESHVERTEX	:
			{
				if( m_pItem->m_Mesh.m_dwNodeCount != mesh.m_dwNodeCount ||
					m_pItem->m_Mesh.m_dwMeshCount < mesh.m_dwMeshCount )
				{
					nError = -1;
					break;
				}

				LPMESHVERTEX pMESH = (LPMESHVERTEX) m_pItem->m_Mesh.m_pDATA->m_pVB;
				LPMESHVERTEX pBUF = (LPMESHVERTEX) mesh.m_pDATA->m_pVB;

				VECTORPTR vDATA;
				vDATA.clear();

				for( DWORD i=0; i<m_pItem->m_Mesh.m_pDATA->m_dwCount; i++)
				{
					LPMESHVERTEX pDATA = new MESHVERTEX();

					memcpy( pDATA, &pMESH[i], sizeof(MESHVERTEX));
					vDATA.push_back(pDATA);
				}

				for( i=0; i<m_pItem->m_Mesh.m_dwMeshCount; i++)
				{
					if( m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] )
						delete m_pItem->m_Mesh.m_pMESH[i][m_dwLevel];
					m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] = NULL;
				}

				//for( i=0; i<m_pItem->m_Mesh.m_dwMeshCount; i++)
				for( i=0; i<mesh.m_dwMeshCount; i++)
				{
					LPIBINFO pINFO = new IBINFO();

					for( DWORD j=0; j<DWORD(mesh.m_pMESH[i][0]->m_vIB.size()); j++)
					{
						LPIBDATA pIB = new IBDATA();

						pIB->m_dwCount = mesh.m_pMESH[i][0]->m_vIB[j]->m_dwCount;
						pIB->m_wMatrix = mesh.m_pMESH[i][0]->m_vIB[j]->m_wMatrix;
						pIB->m_pIB = new WORD[pIB->m_dwCount];

						for( DWORD k=0; k<pIB->m_dwCount; k++)
						{
							WORD wINDEX = m_pItem->FindMESH(
								&vDATA,
								&pBUF[mesh.m_pMESH[i][0]->m_vIB[j]->m_pIB[k]]);

							if( wINDEX >= WORD(vDATA.size()) )
							{
								LPMESHVERTEX pDATA = new MESHVERTEX();

								memcpy( pDATA, &pBUF[mesh.m_pMESH[i][0]->m_vIB[j]->m_pIB[k]], sizeof(MESHVERTEX));
								wINDEX = WORD(vDATA.size());
								vDATA.push_back(pDATA);
							}

							pIB->m_pIB[k] = wINDEX;
						}

						pINFO->m_vIB.push_back(pIB);
					}

					//delete m_pItem->m_Mesh.m_pMESH[i][m_dwLevel]; //위에서 삭제하기 때문에 무조건 NULL 이다.
					m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] = pINFO;
				}
				
				for( i=0; i<m_pItem->m_Mesh.m_dwMeshCount; i++)
				{
					if( m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] == NULL )
						m_pItem->m_Mesh.m_pMESH[i][m_dwLevel] = new IBINFO();
				}

				delete m_pItem->m_Mesh.m_pDATA;
				m_pItem->m_Mesh.m_pDATA = new VBDATA();

				m_pItem->m_Mesh.m_pDATA->m_dwCount = DWORD(vDATA.size());
				m_pItem->m_Mesh.m_pDATA->m_dwFVF = T3DFVF_MESHVERTEX;

				m_pItem->m_Mesh.m_pDATA->m_pVB = new BYTE[m_pItem->m_Mesh.m_pDATA->m_dwCount * sizeof(MESHVERTEX)];
				pMESH = (LPMESHVERTEX) m_pItem->m_Mesh.m_pDATA->m_pVB;

				for( i=0; i<DWORD(vDATA.size()); i++)
				{
					memcpy( &pMESH[i], vDATA[i], sizeof(MESHVERTEX));
					delete vDATA[i];
				}

				vDATA.clear();
			}

			break;
		}

		if( nError == -1 )
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_LOD_FILE);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else
		{
			m_pItem->m_Mesh.Restore(CMainFrame::m_3DDevice.m_pDevice);
			SetItemData();
		}

		delete[] pData;
	}
}

void CMeshBar::OnBnClickedButtonSelectAll()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();

	for( int i=0; i<m_dwMeshCount; i++)
		pDoc->m_pCheck[i] = TRUE;

	SetItemData();
}

void CMeshBar::OnBnClickedButtonUnselectAll()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();

	ZeroMemory( pDoc->m_pCheck, m_dwMeshCount * sizeof(BYTE));
	SetItemData();
}

void CMeshBar::OnBnClickedButtonSelectInv()
{
	CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();

	for( int i=0; i<m_dwMeshCount; i++)
		pDoc->m_pCheck[i] = !pDoc->m_pCheck[i];

	SetItemData();
}

void CMeshBar::OnBnClickedButtonSetdist()
{
	m_strDist = m_strCamDist;
	UpdateData(FALSE);
	OnEnChangeEditDist();
}

void CMeshBar::OnEnChangeEditDist()
{
	UpdateData();
	m_pItem->m_Mesh.m_vDist[m_dwLevel - 1] = (FLOAT) atof(m_strDist);

	if( m_pItem->m_Mesh.m_vDist[m_dwLevel - 1] < 0.0f )
	{
		m_pItem->m_Mesh.m_vDist[m_dwLevel - 1] = 0.0f;
		m_strDist.Format( "%.2f", 0.0f);

		UpdateData(FALSE);
	}
}

void CMeshBar::OnEnKillfocusEditPolyLevel()
{
	UpdateData();

	if(m_pItem)
	{
		CMeshDoc *pDoc = (CMeshDoc *) m_pItem->GetMainDocument();

		m_pItem->ProgressMesh(
			pDoc->m_pCheck,
			m_dwLevel,
			FLOAT(m_nPolyLevel) / 10.0f);
	}
}

void CMeshBar::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinLevel && nPos != 0 )
	{
		if( INT(nPos) < 0 )
			if( m_dwLevel == 0 )
				m_dwLevel = m_bLevelCount - 1;
			else
				m_dwLevel--;
		else if( m_dwLevel + 1 == m_bLevelCount )
			m_dwLevel = 0;
		else
			m_dwLevel++;

		m_spinLevel.SetPos(0);
		SetItemData();
	}

	if( pScrollBar == (CScrollBar *) &m_cPolyCtrl )
		if(!(nSBCode&SB_ENDSCROLL))
		{
			m_strPolyLevel.Format( "%.1f", ((FLOAT) nPos) / 10.0f);
			m_spinPolyLevel.SetPos(nPos);
			m_nPolyLevel = nPos;

			UpdateData(FALSE);
		}
		else
			OnEnKillfocusEditPolyLevel();

	CBarContent::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CMeshBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinDist && nPos != 0 )
	{
		FLOAT fDist = (FLOAT) atof(m_strDist);

		fDist += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinDist.SetPos(0);

		m_strDist.Format( "%.2f", fDist);
		UpdateData(FALSE);
		OnEnChangeEditDist();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPolyLevel )
		if(!(nSBCode&SB_ENDSCROLL))
		{
			m_strPolyLevel.Format( "%.1f", ((FLOAT) nPos) / 10.0f);
			m_nPolyLevel = nPos;

			UpdateData(FALSE);
		}
		else
			OnEnKillfocusEditPolyLevel();

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CMeshBar::OnEnChangeEditPolyLevel()
{
	UpdateData();
	m_nPolyLevel = INT(atof(m_strPolyLevel) * 10.0f);
	m_spinPolyLevel.SetPos(m_nPolyLevel);
	UpdateData(FALSE);
}

void CMeshBar::OnBnClickedCheckUsevb()
{
	UpdateData();
	m_pItem->m_Mesh.m_bUseVB = m_bUseVB;
}
