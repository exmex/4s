// TChatFilterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TChatFilter.h"
#include "TChatFilterDlg.h"
#include ".\tchatfilterdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTChatFilterDlg 대화 상자



CTChatFilterDlg::CTChatFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTChatFilterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTChatFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORDS, m_listWords);
	DDX_Control(pDX, IDC_LIST_EXCEPT, m_listExcept);
	DDX_Control(pDX, IDC_EDIT_CHAT, m_editChat);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_editResult);
	DDX_Control(pDX, IDC_CHECK_XSPACEBAR, m_IgnoreSpace);
	DDX_Control(pDX, IDC_CHECK_XSPCHAR, m_IgnoreNumChar);
	DDX_Control(pDX, IDC_CHECK_CHATALLBLOCK, m_BlockAllSentence);
	DDX_Control(pDX, IDC_EDIT_CHATALLBLOCK, m_editChatBlock);
	DDX_Control(pDX, IDC_CHECK_WORDLIST, m_UseWord);
	DDX_Control(pDX, IDC_CHECK_EXCEPTLIST, m_UseException);
	DDX_Control(pDX, IDC_EDIT_IGNORENUMCHAR, m_editIgnoreNumChar);
}

void CTChatFilterDlg::OnOK()
{
}

BEGIN_MESSAGE_MAP(CTChatFilterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LOADTXT_FILTERWORDLIST, OnLoadtxtFilterwordlist)
	ON_COMMAND(ID_LOADTXT_EXCEPTIONWORDLIST, OnLoadtxtExceptionwordlist)
	ON_EN_CHANGE(IDC_EDIT_CHAT, OnEnChangeEditChat)
	ON_BN_CLICKED(IDC_CHECK_XSPACEBAR, OnBnClickedCheckXspacebar)
	ON_BN_CLICKED(IDC_CHECK_XSPCHAR, OnBnClickedCheckXspchar)
	ON_BN_CLICKED(IDC_CHECK_CHATALLBLOCK, OnBnClickedCheckChatallblock)
	ON_BN_CLICKED(IDC_CHECK_WORDLIST, OnBnClickedCheckWordlist)
	ON_BN_CLICKED(IDC_CHECK_EXCEPTLIST, OnBnClickedCheckExceptlist)
	ON_EN_CHANGE(IDC_EDIT_CHATALLBLOCK, OnEnChangeEditChatallblock)
	ON_COMMAND(ID_FILE_LOADTCD, OnFileLoadTCD)
	ON_COMMAND(ID_SAVETXT_FILTERWORDLIST, OnSavetxtFilterwordlist)
	ON_COMMAND(ID_SAVETXT_EXCEPTIONLIST, OnSavetxtExceptionlist)
	ON_COMMAND(ID_FILE_SAVETCD, OnFileSaveTCD)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_EN_CHANGE(IDC_EDIT_IGNORENUMCHAR, OnEnChangeEditIgnorenumchar)
END_MESSAGE_MAP()


// CTChatFilterDlg 메시지 처리기

BOOL CTChatFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_listWords.InsertColumn( 0, _T("TEXT"), LVCFMT_LEFT, 180 );
	m_listExcept.InsertColumn( 0, _T("TEXT"), LVCFMT_LEFT, 180 );

	InitControls();

	m_FilterSys.SetFilterFile( &m_FilterFile );

	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

void CTChatFilterDlg::InitControls()
{
	m_UseWord.EnableWindow( TRUE );
	m_UseWord.SetCheck( FALSE );
	m_listWords.EnableWindow( FALSE );

	m_listExcept.EnableWindow( FALSE );
	m_UseException.SetCheck( FALSE );
	m_UseException.EnableWindow( FALSE );

	UpdateOptions();
}

void CTChatFilterDlg::UpdateOptions()
{
	m_IgnoreSpace.SetCheck( CTChatFilterOption::bIgnoreSpace );
	m_IgnoreNumChar.SetCheck( CTChatFilterOption::bIgnoreNumChar );
	m_BlockAllSentence.SetCheck( CTChatFilterOption::bBlockAllSentence );
	m_editIgnoreNumChar.EnableWindow( m_IgnoreNumChar.GetCheck() );
	m_editIgnoreNumChar.SetWindowText( CTChatFilter::strIgnoreNumChar );
	m_editChatBlock.EnableWindow( m_BlockAllSentence.GetCheck() );
	m_editChatBlock.SetWindowText( CTChatFilter::strBlockMsg );	
}

BOOL CTChatFilterDlg::PreTranslateMessage( MSG* pMsg )
{
	if( pMsg && pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE )
			return TRUE;
	}

	return CDialog::PreTranslateMessage( pMsg );
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CTChatFilterDlg::OnPaint() 
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
HCURSOR CTChatFilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTChatFilterDlg::OnLoadtxtFilterwordlist()
{
	LoadWords();
	m_UseWord.SetCheck( TRUE );
}

void CTChatFilterDlg::OnLoadtxtExceptionwordlist()
{
	LoadException();
	m_UseException.SetCheck( TRUE );
}

void CTChatFilterDlg::LoadWords()
{
	CString str = _T("Text Files(*.TCD, *.txt)|*.TCD;*.txt|All Files(*.*)|*.*||");
	CFileDialog fDialog( TRUE, NULL, NULL, OFN_HIDEREADONLY, str, this );
	if( fDialog.DoModal() == IDOK )
	{
		CString strExt = fDialog.GetFileExt();
		CString strPath = fDialog.GetPathName();

		strExt.MakeUpper();
		if( strExt == _T("TCD") )
		{
			m_FilterFile.LoadData( strPath );

			int nRes = UpdateListWords();
			m_UseWord.SetCheck( nRes );

			nRes = UpdateListExcept();
			m_UseException.SetCheck( nRes );

			UpdateOptions();
		}
		else if( strExt == _T("TXT") )
		{
			if( !m_FilterFile.LoadTxtWords( strPath ) )
			{
				m_UseWord.SetCheck( FALSE );
				return ;
			}

			int nRes = UpdateListWords();
			m_UseWord.SetCheck( nRes );
		}
	}
	else
	{
		m_UseWord.SetCheck( FALSE );
	}
}

void CTChatFilterDlg::LoadException()
{
	CString str = _T("Text Files(*.TCD, *.txt)|*.TCD;*.txt|All Files(*.*)|*.*||");
	CFileDialog fDialog( TRUE, NULL, NULL, OFN_HIDEREADONLY, str, this );
	if( fDialog.DoModal() == IDOK )
	{
		CString strExt = fDialog.GetFileExt();
		CString strPath = fDialog.GetPathName();

		strExt.MakeUpper();
		if( strExt == _T("TXT") )
		{
			if( !m_FilterFile.LoadTxtExcept( strPath ) )
			{
				m_UseException.SetCheck( FALSE );
				return ;
			}

			int nRes = UpdateListExcept();
			m_UseException.SetCheck( nRes );
		}
	}
	else
	{
		m_UseException.SetCheck( FALSE );
	}
}

void CTChatFilterDlg::UpdateChatFilterTest()
{
	CString strChat;
	m_editChat.GetWindowText( strChat );

	// Filtering.
	m_FilterSys.ConvertString( strChat );

	if( m_FilterSys.UsedFobiddenWords() && CTChatFilterOption::bBlockAllSentence )
		m_editResult.SetWindowText( CTChatFilter::strBlockMsg );
	else
		m_editResult.SetWindowText( strChat );
}

bool CTChatFilterDlg::UpdateListWords()
{
	TMAP_FILTERDATA* pFilterData = m_FilterFile.GetFilterData();

	if( !pFilterData || pFilterData->size() == 0 )
		return false;

	m_listWords.EnableWindow( TRUE );
	m_UseException.EnableWindow( TRUE );

	int nItem = 0;
	TOuter_Iter iter = pFilterData->begin();
	while( iter != pFilterData->end() )
	{
		m_listWords.InsertItem( nItem, (*iter).first );
		++iter;
	}

	return true;
}

bool CTChatFilterDlg::UpdateListExcept()
{
	TMAP_FILTERDATA* pFilterData = m_FilterFile.GetFilterData();

	if( !pFilterData || pFilterData->size() == 0 )
		return false;

	m_listExcept.EnableWindow( TRUE );

	int nItem = 0;
	TOuter_Iter o_iter = pFilterData->begin();
	while( o_iter != pFilterData->end() )
	{
		TMAP_EXCEPTDATA* pExceptData = (*o_iter).second;
		if( pExceptData )
		{
			TInner_Iter i_iter = pExceptData->begin();
			while( i_iter != pExceptData->end() )
			{
				m_listExcept.InsertItem( nItem++, (*i_iter).first );
				++i_iter;
			}
		}
		++o_iter;
	}

	return true;
}

void CTChatFilterDlg::ReleaseList()
{
	m_listWords.DeleteAllItems();
	m_listWords.EnableWindow( FALSE );

	ReleaseListExcept();
}

void CTChatFilterDlg::ReleaseListExcept()
{
	m_listExcept.DeleteAllItems();
	m_UseException.SetCheck( FALSE );
	m_UseException.EnableWindow( m_UseWord.GetCheck() );
	m_listExcept.EnableWindow( FALSE );
}


////////////////////////////////////////////////////////////////
//	Control Event.

void CTChatFilterDlg::OnEnChangeEditIgnorenumchar()
{
	m_editIgnoreNumChar.GetWindowText( CTChatFilter::strIgnoreNumChar );

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnEnChangeEditChat()
{
	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnBnClickedCheckXspacebar()
{
	int getCheck = m_IgnoreSpace.GetCheck();
	CTChatFilterOption::bIgnoreSpace = getCheck;

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnBnClickedCheckXspchar()
{
	int getCheck = m_IgnoreNumChar.GetCheck();
	CTChatFilterOption::bIgnoreNumChar = getCheck;
	m_editIgnoreNumChar.EnableWindow( getCheck );

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnBnClickedCheckChatallblock()
{
	int getCheck = m_BlockAllSentence.GetCheck();
	CTChatFilterOption::bBlockAllSentence = getCheck;
	m_editChatBlock.EnableWindow( getCheck );

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnBnClickedCheckWordlist()
{
	int getCheck = m_UseWord.GetCheck();

	if( getCheck )	// TRUE.
	{
		LoadWords();
	}
	else	// FALSE.
	{
		m_FilterFile.ReleaseAll();
		ReleaseList();
	}

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnBnClickedCheckExceptlist()
{
	int getCheck = m_UseException.GetCheck();
	CTChatFilterOption::bUseExceptionList = getCheck;

	if( getCheck ) // TRUE;
	{
		LoadException();
	}
	else	// FALSE.
	{
		m_FilterFile.ReleaseExcept();
		ReleaseListExcept();
	}

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnEnChangeEditChatallblock()
{
	m_editChatBlock.GetWindowText( CTChatFilter::strBlockMsg );

	UpdateChatFilterTest();
}

void CTChatFilterDlg::OnFileLoadTCD()
{
	CString str = _T("Text Type(*.TCD)|*.TCD||");
	CFileDialog fDialog( TRUE,
		CTChatFilter::strFileExt, 
		CTChatFilter::strFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this );

	if( fDialog.DoModal() == IDOK )
	{
		CString strPath = fDialog.GetPathName();
		m_FilterFile.LoadData( strPath );

		int nRes = UpdateListWords();
		m_UseWord.SetCheck( nRes );

		nRes = UpdateListExcept();
		m_UseException.SetCheck( nRes );

		UpdateOptions();
	}
}

void CTChatFilterDlg::OnSavetxtFilterwordlist()
{
	CString str = _T("Text Type(*.txt)|*.txt||");
	CFileDialog fDialog( FALSE, 
		CTChatFilter::strFileExt, 
		CTChatFilter::strFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this );

	if( fDialog.DoModal() == IDOK )
	{
		CString strPath = fDialog.GetPathName();
		m_FilterFile.SaveTxtWords( strPath );
	}
}

void CTChatFilterDlg::OnSavetxtExceptionlist()
{
	CString str = _T("Text Type(*.txt)|*.txt||");
	CFileDialog fDialog( FALSE, 
		CTChatFilter::strFileExt, 
		CTChatFilter::strFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this );

	if( fDialog.DoModal() == IDOK )
	{
		CString strPath = fDialog.GetPathName();
		m_FilterFile.SaveTxtExcept( strPath );
	}	
}

void CTChatFilterDlg::OnFileSaveTCD()
{
	CString str = _T("Text Type(*.tcd)|*.tcd||");
	CFileDialog fDialog( FALSE, 
		CTChatFilter::strFileExt, 
		CTChatFilter::strFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this );

	if( fDialog.DoModal() == IDOK )
	{
		CString strPath = fDialog.GetPathName();
		m_FilterFile.SaveData( strPath );

		MessageBox( _T("Complete") );
	}
}

void CTChatFilterDlg::OnFileExit()
{
	OnCancel();
}
