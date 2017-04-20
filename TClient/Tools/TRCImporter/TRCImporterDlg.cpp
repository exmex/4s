// TRCImporterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TRCImporter.h"
#include "TRCImporterDlg.h"
#include ".\trcimporterdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTRCImporterDlg 대화 상자



CTRCImporterDlg::CTRCImporterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTRCImporterDlg::IDD, pParent)
	, m_strRCPath(_T(""))
	, m_strCSV(_T(""))
	, m_nNation(0)
	, m_strTargetPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTRCImporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRCPath);
	DDX_Text(pDX, IDC_EDIT2, m_strCSV);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nNation);
	DDX_Text(pDX, IDC_EDIT3, m_strTargetPath);
}

BEGIN_MESSAGE_MAP(CTRCImporterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CTRCImporterDlg 메시지 처리기

BOOL CTRCImporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CTRCImporterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CTRCImporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTRCImporterDlg::OnBnClickedButton2()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("RC Text Files (*.rc)|*.rc|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		m_strRCPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTRCImporterDlg::OnBnClickedButton3()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("CSV Text Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		m_strCSV = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTRCImporterDlg::OnBnClickedButton4()
{
	CFileDialog dlg(
		TRUE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("RC Text Files (*.rc)|*.rc|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		m_strTargetPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTRCImporterDlg::OnBnClickedOk()
{
	UpdateData();

	if( Import( m_strCSV, m_strRCPath, m_strTargetPath, m_nNation) )
		AfxMessageBox("Success");
	else
		AfxMessageBox("Fail");

	OnOK();
}

int ParseCSV( LPCTSTR str, std::vector<CString> *pList) 
{ 
    int cnt = 0; 
    CString buffer; 
    int mode = 0; 
    
    while(*str) 
    {  
        if(mode == 0) 
        {      
            if(*str == ',') 
            { 
                pList->push_back(buffer); 
                buffer = ""; 
            } 
            else if(*str == '\"') 
            { 
                mode = 1;            
            } 
            else 
                buffer += *str; 
        } 
        else if(mode == 1) 
        { 
            if(*str == '\"') 
            { 
                ++str; 
                if(*str == '\"') 
                    buffer += '\"'; 
                else if(*str == ',') 
                { 
                    mode = 0; 
                    pList->push_back(buffer); 
                    buffer = ""; 
                } 
                else 
                    buffer += *str; 
            } 
            else 
                buffer += *str; 
        } 
        
        ++str; 
    }    

    pList->push_back(buffer); 
    return (int)pList->size();
}

static int NONE = 0;
static int START = 1;
static int END = 2;

INT ReplaceNewText( CString& strLINE, CString& strNEW )
{
	CString strOLD;

	INT nLineLength = strLINE.GetLength();
	INT nMode = NONE;

	for( INT i=0; i<nLineLength; i++)
	{
		BYTE c = strLINE.GetAt(i);

		if(MBCS_LEAD_TEST(c))
		{
			strOLD += c;
			strOLD += strLINE.GetAt(i + 1);
			i++;

			continue;
		}
		else if( c == '"' && nMode == NONE) 
		{
			nMode = START;
			strOLD += c;
			continue;
		}
		else if( c == '"' && nMode == START)
		{
			if( i + 1 != nLineLength &&
				strLINE.GetAt(i+1) == '"')
			{
				strOLD += c;
				strOLD += strLINE.GetAt(i + 1);
				i++;

				continue;
			}
			else
			{
				nMode = END;
				strOLD += c;
				break; // 마지막 따옴표이다.
			}
		}
		else if( nMode == START )
		{
			strOLD += c;
			continue;
		}
	}

	if( nMode == END )
	{
		strLINE.Replace(
			strOLD,
			strNEW);
		return 1;
	}
	else
	{
		return 0;	// 텍스트는 다음줄에 있다.
	}
}

static LPCTSTR vNATION[][3] =
{
	{ "LANG_GERMAN", "code_page(1252)", "#ifdef GERMANY\n" }, // GERMANY
	{ "LANG_KOREAN", "code_page(949)", "#ifdef ENGLISH\n" }, // ENGLISH
	{ "LANG_JAPANESE", "code_page(932)", "#ifdef JAPAN\n" }, // JAPAN
};

int CTRCImporterDlg::Import(CString strCSVPath, CString strRCPath, CString strTargetPath, int nNation)
{
	std::map<CString,CString> m_mapIDtoTRANS;

	// CSV파일 파싱
	{
		CStdioFile vSRC;

		if( vSRC.Open( strCSVPath, CFile::modeRead|CFile::typeText) )
		{
			CString strDATA;
			std::vector<CString> vELEM;

			while(vSRC.ReadString(strDATA))
			{
				vELEM.clear();

				ParseCSV( (LPCTSTR)strDATA, &vELEM );
				if( vELEM.size() >= 3 )
				{
					CString& strID = vELEM[0];
					CString strTRANS = vELEM[2];

					if( strID.IsEmpty() )
						continue;

					strTRANS.Replace( "\"", "\"\"");

					CString strTEXT("\"A\"");
					strTEXT.Replace( "A", strTRANS);

					m_mapIDtoTRANS.insert(
						std::map<CString,CString>::value_type(
							strID, strTEXT));
				}
			}

			vSRC.Close();
		}
		else
			return FALSE;
	}

	{
		BOOL bChangedLocal = FALSE;
		BOOL bChangedCodePage = FALSE;

		CStdioFile vSRC;
		CStdioFile vDEST;

		if( vDEST.Open( strTargetPath, CFile::modeCreate|CFile::modeWrite|CFile::typeText) &&
			vSRC.Open( strRCPath, CFile::modeRead|CFile::typeText) )
		{
			CString strLINE;

			vDEST.WriteString( vNATION[ nNation ][2] );

			while(vSRC.ReadString(strLINE))
			{
				int iStart=0;
				CString strTOKEN = strLINE.Tokenize( " \t\n\r", iStart);

				std::map<CString,CString>::iterator it =
					m_mapIDtoTRANS.find( strTOKEN);

				if( it != m_mapIDtoTRANS.end() )
				{
					//찾았다.
					CString& strNEW = it->second;
					if( !ReplaceNewText( strLINE, strNEW ) )
					{
						vDEST.WriteString(strLINE);
						vDEST.WriteString("\n");
						// 다음줄에 텍스트가 있구나.
						strLINE.Empty();
						vSRC.ReadString(strLINE);
						if( 0 == ReplaceNewText( strLINE, strNEW) )
							ASSERT(1); // 큰일난것임.
					}
				}
				
				if( !bChangedLocal )
				{
					if( strLINE.Find( "LANG_KOREAN", 0 ) != -1 )
					{
						strLINE.Replace( "LANG_KOREAN", vNATION[ nNation][0] );
						bChangedLocal = TRUE;
					}
				}

				if( !bChangedCodePage )
				{
					if( strLINE.Find( "code_page(949)", 0 ) != -1 )
					{
						strLINE.Replace( "code_page(949)", vNATION[ nNation][ 1 ] );
						bChangedCodePage = TRUE;
					}
				}

				vDEST.WriteString( strLINE);
				vDEST.WriteString( "\n");
			}

			vDEST.WriteString( "\n#endif" );

			vSRC.Close();
			vDEST.Close();
		}
		else
			return FALSE;
	}

	return TRUE;
}