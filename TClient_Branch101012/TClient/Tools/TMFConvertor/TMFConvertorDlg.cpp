// TMFConvertorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMFConvertor.h"
#include "TMFConvertorDlg.h"
#include ".\TMFConvertordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTMFConvertorDlg dialog



CTMFConvertorDlg::CTMFConvertorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMFConvertorDlg::IDD, pParent)
	, m_strDESTFOLDER(_T(""))
	, m_strSRCFOLDER(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pMesh = NULL;
}

CTMFConvertorDlg::~CTMFConvertorDlg()
{
	if(m_pMesh)
	{
		m_pMesh->Release();
		delete m_pMesh;
		m_pMesh = NULL;
	}
	m_pDevice.ReleaseDevice();
}

void CTMFConvertorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DESTFOLDER, m_cDESTFOLDER);
	DDX_Text(pDX, IDC_EDIT_DESTFOLDER, m_strDESTFOLDER);
	DDX_Control(pDX, IDC_BUTTON_SRCFOLDER, m_cSRCFOLDER);
	DDX_Text(pDX, IDC_EDIT_SRCFOLDER, m_strSRCFOLDER);
}

BEGIN_MESSAGE_MAP(CTMFConvertorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DESTFOLDER, OnBnClickedButtonDestfloder)
	ON_BN_CLICKED(ID_CONVERTOR, OnBnClickedConvert)
	ON_BN_CLICKED(IDC_BUTTON_SRCFOLDER, OnBnClickedButtonSrcfolder)
END_MESSAGE_MAP()


// CTMFConvertorDlg message handlers

BOOL CTMFConvertorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cSRCFOLDER.SetIcon(hIcon);
	m_cDESTFOLDER.SetIcon(hIcon);

	m_pDevice.m_option.m_bWindowedMode = TRUE;
	m_pDevice.InitDevices(this);
	CTachyonRes::m_pDEVICE = &m_pDevice;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTMFConvertorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTMFConvertorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTMFConvertorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTMFConvertorDlg::OnBnClickedButtonDestfloder()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strDESTFOLDER;
	dlg.m_strTitle = _T("Target Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strDESTFOLDER = dlg.m_strPath.IsEmpty() ? m_strDESTFOLDER : dlg.m_strPath;
		m_strDEST = dlg.m_strPath.IsEmpty() ? m_strDESTFOLDER : dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMFConvertorDlg::OnBnClickedButtonSrcfolder()
{
	CTShell dlg;

	dlg.m_strSelDir = m_strSRCFOLDER;
	dlg.m_strTitle = _T("Mesh Folder");

	if( dlg.BrowseForFolder(this) == IDOK )
	{
		UpdateData();
		m_strSRCFOLDER = dlg.m_strPath.IsEmpty() ? m_strSRCFOLDER : dlg.m_strPath;
		m_strSRC = dlg.m_strPath.IsEmpty() ? m_strSRCFOLDER : dlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CTMFConvertorDlg::OnBnClickedConvert()
{
	UpdateData();
	m_strSRC = m_strSRCFOLDER;
	m_strDEST = m_strDESTFOLDER;
	Convert( m_strSRC, m_strDEST);
}

BYTE CTMFConvertorDlg::IsMeshFile( CString strFileName)
{
	CString strExt;
	CTShell shell;

	strExt = shell.GetFileExt(strFileName);
	strExt.MakeUpper();
	strExt.TrimRight();
	strExt.TrimLeft();

	if( strExt == _T(".TMF") )
	{
		CFile vSRC( LPCSTR(strFileName), CFile::modeRead|CFile::typeBinary);
		DWORD dwNodeCount = 0;
		int nTMFVersion = 0;

		vSRC.Read( &nTMFVersion, sizeof(int));
		vSRC.Read( &dwNodeCount, sizeof(DWORD));

		if( TARGET_TMF_VERSION == nTMFVersion )
			return TRUE;
	}
	return FALSE;
}

WORD CTMFConvertorDlg::FindMESH( LPVECTORPTR pMESH,
						  LPWMESHVERTEX pPOINT)
{
	WORD wCount = WORD(pMESH->size());

	for( WORD i=0; i<wCount; i++)
		if(CTMath::IsEqual( (LPWMESHVERTEX) (*pMESH)[i], pPOINT))
			return i;

	return wCount;
}

WORD CTMFConvertorDlg::FindMESH( LPVECTORPTR pMESH,
						  LPMESHVERTEX pPOINT)
{
	WORD wCount = WORD(pMESH->size());

	for( WORD i=0; i<wCount; i++)
		if(CTMath::IsEqual( (LPMESHVERTEX) (*pMESH)[i], pPOINT))
			return i;

	return wCount;
}

void CTMFConvertorDlg::Convert( CString strSRC, CString strDEST)
{
	CFileFind finder;
	CString strFIND;

	strDEST.TrimRight('\\');
	strSRC.TrimRight('\\');
	strFIND.Format("%s\\*.*", strSRC);

	BOOL bFind = finder.FindFile(strFIND);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			CreateDirectory( LPCSTR(strDEST + CString("\\") + finder.GetFileTitle()), NULL);
			Convert(
				strSRC + CString("\\") + finder.GetFileTitle(),
				strDEST + CString("\\") + finder.GetFileTitle());

			continue;
		}

		if(IsMeshFile(finder.GetFilePath()))
		{
			CFile vDEST( LPCSTR(strDEST + CString("\\") + finder.GetFileName()), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
			CFile vSRC( LPCSTR(finder.GetFilePath()), CFile::modeRead|CFile::typeBinary);

			Convert(&vSRC, &vDEST);
			vSRC.Close();
			vDEST.Close();
		}
	}
}

void CTMFConvertorDlg::Convert( CFile *pSRC, CFile *pDEST )
{
	m_pMesh = new CTMeshTemp();

	DWORD dwSize = (DWORD) pSRC->GetLength();
	LPBYTE pDATA = new BYTE[dwSize];
	pSRC->Read( pDATA, dwSize);

	m_pMesh->Release();
	if(!Load(pDATA, m_pMesh))
	{
		MessageBox(_T("Load Error"));
		return;
	}
	Save(pDEST, m_pMesh);
	m_pMesh->Release();
	delete m_pMesh;
	m_pMesh = NULL;

	delete pDATA;
	pDATA = NULL;
}

BOOL CTMFConvertorDlg::Load(LPBYTE pDATA, CTMeshTemp * pMesh)
{
	pMesh->m_nTMFVersion = *((int *) pDATA); pDATA += sizeof(int);
	if( pMesh->m_nTMFVersion != TARGET_TMF_VERSION )
		return FALSE;

	pMesh->m_dwNodeCount = *((LPDWORD) pDATA); pDATA += sizeof(DWORD);
	pMesh->m_fRadius = *((FLOAT *) pDATA); pDATA += sizeof(FLOAT);
	pMesh->m_vCenterPoint = *((LPD3DXVECTOR3) pDATA); pDATA += sizeof(D3DXVECTOR3);
	pMesh->m_pBones = NULL;

	if( pMesh->m_dwNodeCount > 0 )
	{
		pMesh->m_pBones = new VECTORBONE;

		for(int i=0; i<pMesh->m_dwNodeCount; i++)
		{
			D3DXMATRIX vBone;

			memcpy(&vBone, pDATA, sizeof(D3DXMATRIX));
			pDATA += sizeof(D3DXMATRIX);

			pMesh->m_pBones->push_back(vBone);
		}
	}

	DWORD dwMeshCount = *((LPDWORD) pDATA);	pDATA += sizeof(DWORD);
	LPVECTORPTR vMESH;
	LPIBDATA pINDEX = NULL;

	if( dwMeshCount > 0 )
	{
		pMesh->m_pMESHIBDATA = new VECTORIBDATA[dwMeshCount];
		pMesh->m_pMESHIB = new VECTORIB[dwMeshCount];
	}

	pMesh->m_pMESH = new VECTORPTR;
	pMesh->m_pMESH->clear();
	vMESH = new VECTORPTR;
	pMesh->m_dwVBCnt = new DWORD[dwMeshCount];
	pMesh->m_nMESHCNT = dwMeshCount;
	int nCount = 0;

	for( int i=0; i<dwMeshCount; i++)
	{
		int nCount = *((int *)pDATA); pDATA += sizeof(int);
		LPIBDATA pINDEX = new IBDATA();

		pMesh->m_dwVBCnt[i] = (DWORD)nCount;
		if( nCount > 0 )
		{
			LPVBDATA pBUF = new VBDATA();
			LPMESHVB pVB = new MESHVB();

			pBUF->m_dwCount = nCount / 3;
			pVB->m_dwCount = pBUF->m_dwCount;	

			if( pMesh->m_dwNodeCount > 0 )
			{
				pBUF->m_pVB = new BYTE[nCount * sizeof(WMESHVERTEX)];
				pBUF->m_dwFVF = T3DFVF_WMESHVERTEX;

				pVB->m_vVB.LoadT3DVB(
					nCount * sizeof(WMESHVERTEX),
					0, T3DFVF_WMESHVERTEX);

				pVB->m_vVB.LoadT3DVBDATA(
					pDATA,
					nCount * sizeof(WMESHVERTEX), 0);
				memcpy( pBUF->m_pVB, pDATA, nCount * sizeof(WMESHVERTEX));

				pDATA += nCount * sizeof(WMESHVERTEX);
			}
			else
			{
				LPWMESHVERTEX pSRC = (LPWMESHVERTEX) pDATA;

				pBUF->m_pVB = new BYTE[nCount * sizeof(MESHVERTEX)];
				pBUF->m_dwFVF = T3DFVF_MESHVERTEX;

				LPMESHVERTEX pDEST = (LPMESHVERTEX) pBUF->m_pVB;
				for( int j=0; j<nCount; j++)
				{
					pDEST[j].m_fNormalX = pSRC[j].m_fNormalX;
					pDEST[j].m_fNormalY = pSRC[j].m_fNormalY;
					pDEST[j].m_fNormalZ = pSRC[j].m_fNormalZ;
					pDEST[j].m_fPosX = pSRC[j].m_fPosX;
					pDEST[j].m_fPosY = pSRC[j].m_fPosY;
					pDEST[j].m_fPosZ = pSRC[j].m_fPosZ;
					pDEST[j].m_fU1 = pSRC[j].m_fU;
					pDEST[j].m_fV1 = pSRC[j].m_fV;
					pDEST[j].m_fU2 = pSRC[j].m_fU;
					pDEST[j].m_fV2 = pSRC[j].m_fV;
				}

				pVB->m_vVB.LoadT3DVB(
					nCount * sizeof(MESHVERTEX),
					0, T3DFVF_MESHVERTEX);

				pVB->m_vVB.LoadT3DVBDATA(
					pBUF->m_pVB,
					nCount * sizeof(MESHVERTEX), 0);

				pDATA += nCount * sizeof(WMESHVERTEX);
			}

			ConvertMESH(pMesh->m_pMESH, pINDEX, pBUF);
			pMesh->m_pMESHIBDATA[i].push_back(pINDEX);

			delete pBUF;
			delete pVB;
		}
	}
	pMesh->m_dwMeshCount = DWORD(pMesh->m_pMESH->size());

	if( !pMesh->m_pMESH->empty() )
	{
		pMesh->m_pDATA = new VBDATA();

		pMesh->m_pDATA->m_dwFVF = pMesh->m_dwNodeCount > 0 ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX;
		pMesh->m_pDATA->m_dwCount = DWORD(pMesh->m_pMESH->size());

		if( pMesh->m_dwNodeCount > 0 )
		{
			LPWMESHVERTEX pBUF = NULL;

			pMesh->m_pDATA->m_pVB = new BYTE[sizeof(WMESHVERTEX) * pMesh->m_pDATA->m_dwCount];
			pBUF = (LPWMESHVERTEX) pMesh->m_pDATA->m_pVB;

			VECTORPTR::iterator itPTR;
			for( itPTR=pMesh->m_pMESH->begin(); itPTR!=pMesh->m_pMESH->end(); itPTR++ )
			{
				LPWMESHVERTEX ptr = (LPWMESHVERTEX)(*itPTR);
				memcpy( pBUF, ptr, sizeof(WMESHVERTEX));
				pBUF += sizeof(BYTE);
			}
		}
		else
		{
			LPMESHVERTEX pBUF = NULL;

			pMesh->m_pDATA->m_pVB = new BYTE[sizeof(MESHVERTEX) * pMesh->m_pDATA->m_dwCount];
			pBUF = (LPMESHVERTEX) pMesh->m_pDATA->m_pVB;

			VECTORPTR::iterator itPTR;
			for( itPTR=pMesh->m_pMESH->begin(); itPTR!=pMesh->m_pMESH->end(); itPTR++ )
			{
				LPMESHVERTEX ptr = (LPMESHVERTEX)(*itPTR);
				memcpy( pBUF, ptr, sizeof(MESHVERTEX));
				pBUF += sizeof(BYTE);
			}
		}
	}
	delete vMESH;
	return TRUE;
}

void CTMFConvertorDlg::ConvertMESH(LPVECTORPTR pMESH, LPIBDATA pINDEX, LPVBDATA pDATA)
{
	pINDEX->m_dwCount = pDATA->m_dwCount * 3 ;
	pINDEX->m_pIB = new WORD[pINDEX->m_dwCount];

	switch(pDATA->m_dwFVF)
	{
	case T3DFVF_WMESHVERTEX	:
		{
			LPWMESHVERTEX pBUF = (LPWMESHVERTEX) pDATA->m_pVB;

			for( DWORD i=0; i<pINDEX->m_dwCount; i++)
			{
				pINDEX->m_pIB[i] = FindMESH(
					pMESH,
					&pBUF[i]);

				if( pINDEX->m_pIB[i] >= DWORD(pMESH->size()) )
				{
					LPWMESHVERTEX pPOINT = new WMESHVERTEX;

					memcpy( pPOINT, &pBUF[i], sizeof(WMESHVERTEX));
					pMESH->push_back(pPOINT);
				}
			}
		}

		break;

	case T3DFVF_MESHVERTEX	:
		{
			LPMESHVERTEX pBUF = (LPMESHVERTEX) pDATA->m_pVB;

			for( DWORD i=0; i<pINDEX->m_dwCount; i++)
			{
				pINDEX->m_pIB[i] = FindMESH(
					pMESH,
					&pBUF[i]);

				if( pINDEX->m_pIB[i] >= DWORD(pMESH->size()) )
				{
					LPMESHVERTEX pPOINT = new MESHVERTEX;

					memcpy( pPOINT, &pBUF[i], sizeof(MESHVERTEX));
					pMESH->push_back(pPOINT);
				}
			}
		}
		break;
	}
}

void CTMFConvertorDlg::Save(CFile *pDEST, CTMeshTemp *pMesh)
{
	pMesh->m_nTMFVersion = 300;
	pDEST->Write( &pMesh->m_nTMFVersion, sizeof(int));
	pDEST->Write( &pMesh->m_dwNodeCount, sizeof(DWORD));
	pDEST->Write( &pMesh->m_fRadius, sizeof(FLOAT));
	pDEST->Write( &pMesh->m_vCenterPoint, sizeof(D3DXVECTOR3));

	if(pMesh->m_dwNodeCount>0)
	{
		VECTORBONE::iterator itBONE;
		for( itBONE=pMesh->m_pBones->begin(); itBONE!=pMesh->m_pBones->end(); itBONE++ )
		{
			D3DXMATRIX vBONE = (*itBONE);
			pDEST->Write( &vBONE, sizeof(D3DXMATRIX));
		}
	}
	DWORD dwMeshCnt = pMesh->m_dwMeshCount;
	pDEST->Write( &dwMeshCnt, sizeof(DWORD));

	if(pMesh->m_dwNodeCount > 0)
	{
		LPWMESHVERTEX pt = (LPWMESHVERTEX)pMesh->m_pDATA->m_pVB;

		for( int i=0; i<pMesh->m_pDATA->m_dwCount; i++ )
		{
			WMESHVERTEX wMesh;
			wMesh.m_fNormalX 	= pt->m_fNormalX;
			wMesh.m_fNormalY 	= pt->m_fNormalY;
			wMesh.m_fNormalZ 	= pt->m_fNormalZ;
			wMesh.m_fPosX		= pt->m_fPosX;
			wMesh.m_fPosY		= pt->m_fPosY;
			wMesh.m_fPosZ		= pt->m_fPosZ;
			wMesh.m_fU			= pt->m_fU;
			wMesh.m_fV			= pt->m_fV;
			wMesh.m_dwMatIndex	= pt->m_dwMatIndex;
			for( int j=0; j<WEIGHT_COUNT; j++)
				wMesh.m_fWeight[j] = pt->m_fWeight[j];
			
			pDEST->Write(&wMesh, sizeof(WMESHVERTEX));
			pt += sizeof(BYTE);
		}
	}
	else
	{
		LPMESHVERTEX pt = (LPMESHVERTEX)pMesh->m_pDATA->m_pVB;

		for( int i=0; i<pMesh->m_pDATA->m_dwCount; i++ )
		{
			WMESHVERTEX wMesh;
			wMesh.m_fNormalX 	= pt->m_fNormalX;
			wMesh.m_fNormalY 	= pt->m_fNormalY;
			wMesh.m_fNormalZ 	= pt->m_fNormalZ;
			wMesh.m_fPosX		= pt->m_fPosX;
			wMesh.m_fPosY		= pt->m_fPosY;
			wMesh.m_fPosZ		= pt->m_fPosZ;
			wMesh.m_fU			= pt->m_fU1;
			wMesh.m_fV			= pt->m_fV1;
			pDEST->Write(&wMesh, sizeof(WMESHVERTEX));
			pt += sizeof(BYTE);
		}
	}

	DWORD dwMESHCnt = (DWORD)pMesh->m_nMESHCNT;
	pDEST->Write(&dwMESHCnt, sizeof(DWORD));
	
	DWORD prePos = 0;
	for( DWORD i=0; i<dwMESHCnt; i++)
	{
		DWORD dwCnt = pMesh->m_dwVBCnt[i]; 
		pDEST->Write( &dwCnt, sizeof(DWORD));
		
		VECTORIBDATA pIBDATA = pMesh->m_pMESHIBDATA[i];

		for(int j=0; j<pIBDATA.size(); j++)
		{
			for( int k=0; k<pIBDATA[j]->m_dwCount; k++ )
			{
				WORD dwINDEX = pIBDATA[j]->m_pIB[k];
				pDEST->Write(&pIBDATA[j]->m_pIB[k], sizeof(WORD));
			}
		}
	}
	ReleaseMesh(pMesh);
}
void CTMFConvertorDlg::ReleaseMesh(CTMeshTemp *pMesh)
{
	if(pMesh)
	{
		if(pMesh->m_pDATA)
		{
			if(pMesh->m_pDATA->m_pVB)
			{
				delete pMesh->m_pDATA->m_pVB;
				pMesh->m_pDATA->m_pVB = NULL;
			}
			delete pMesh->m_pDATA;
			pMesh->m_pDATA = NULL;
		}
		if(pMesh->m_pMESH)
		{
			if(!pMesh->m_pMESH->empty())
			{
				VECTORPTR::iterator itPT;
				for(itPT=pMesh->m_pMESH->begin(); itPT!=pMesh->m_pMESH->end(); itPT++)
				{
					delete (*itPT);
				}
				pMesh->m_pMESH->clear();
			}
			delete pMesh->m_pMESH;
		}
		if(pMesh->m_pMESHIB)
			delete[] pMesh->m_pMESHIB;
		if(pMesh->m_pMESHIBDATA)
		{
			for(int i=0; i<pMesh->m_nMESHCNT; i++)
			{
				VECTORIBDATA::iterator itIBDATA;
				for(itIBDATA=pMesh->m_pMESHIBDATA[i].begin(); itIBDATA!=pMesh->m_pMESHIBDATA[i].end(); itIBDATA++)
				{
					LPIBDATA pINDEX = (*itIBDATA);
					delete pINDEX;
				}
			}
			delete[] pMesh->m_pMESHIBDATA;
		}
		if(pMesh->m_dwVBCnt)
			delete[] pMesh->m_dwVBCnt;
	}
}











