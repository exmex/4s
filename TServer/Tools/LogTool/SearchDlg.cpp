////////////////////////////////////////////////////////////////////////////////
//	File Name	:	SearchDlg.cpp
//
//	Copyright	:	(c) Zemi Interactive, Inc 2002-2009
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogTool.h"
#include "LogToolDlg.h"
#include "SearchDlg.h"
#include "Defines.h"

#include <vector>


using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
//	Global Values
////////////////////////////////////////////////////////////////////////////////

extern LPCTSTR ACTIONLIST[];
extern LPCTSTR DBFIELDSTR[];
extern int ActionNum[];
extern int nActionCount;

////////////////////////////////////////////////////////////////////////////////
//	CSearchDlg dialog
////////////////////////////////////////////////////////////////////////////////
//#include "NamingList.h"

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchDlg)
	//}}AFX_DATA_INIT
}


void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSearchDlg)

	DDX_Control(pDX, IDC_DATETIME, m_ctrlDateInput);
	DDX_Control(pDX, IDC_DATETIME2, m_ctrlDateInput2);

	DDX_Control(pDX, IDC_COMBO_ACTION, m_ctrlActionInput);
	DDX_Control(pDX, IDC_CHECK_ACTION, m_ctrlAction);	
	DDX_Control(pDX, IDC_CHECK_DATE, m_ctrlDate);

	DDX_Control(pDX, IDC_CHECK_IP, m_ctrlIP);			// IP
	DDX_Control(pDX, IDC_EDIT_IP, m_ctrlIPInput);		// Ip input

	DDX_Control(pDX, IDC_CHECK_MAPID, m_ctrlMapID);		// Map
	DDX_Control(pDX, IDC_EDIT_MAPID, m_ctrlMapIDInput);	// Map input
	
	DDX_Control(pDX, IDC_CHECK_DK1, m_ctrlDK1);			// DK1
	DDX_Control(pDX, IDC_EDIT_DK1, m_ctrlDKInput1);		// DK1 input

	DDX_Control(pDX, IDC_CHECK_DK2, m_ctrlDK2);			// DK2
	DDX_Control(pDX, IDC_EDIT_DK2, m_ctrlDKInput2);		// DK2 input

	DDX_Control(pDX, IDC_CHECK_DK3, m_ctrlDK3);			// DK3
	DDX_Control(pDX, IDC_EDIT_DK3, m_ctrlDKInput3);		// DK3 input

	DDX_Control(pDX, IDC_CHECK_DK4, m_ctrlDK4);			// DK4
	DDX_Control(pDX, IDC_EDIT_DK4, m_ctrlDKInput4);		// DK4 input

	DDX_Control(pDX, IDC_CHECK_DK5, m_ctrlDK5);			// DK5
	DDX_Control(pDX, IDC_EDIT_DK5, m_ctrlDKInput5);		// DK5 input

	DDX_Control(pDX, IDC_CHECK_DK6, m_ctrlDK6);			// DK6
	DDX_Control(pDX, IDC_EDIT_DK6, m_ctrlDKInput6);		// DK6 input

	DDX_Control(pDX, IDC_CHECK_DK7, m_ctrlDK7);			// DK7
	DDX_Control(pDX, IDC_EDIT_DK7, m_ctrlDKInput7);		// DK7 input

	DDX_Control(pDX, IDC_CHECK_DK8, m_ctrlDK8);			// DK8
	DDX_Control(pDX, IDC_EDIT_DK8, m_ctrlDKInput8);		// DK7 input

	DDX_Control(pDX, IDC_CHECK_DK9, m_ctrlDK9);			// DK9
	DDX_Control(pDX, IDC_EDIT_DK9, m_ctrlDKInput9);		// DK7 input

	DDX_Control(pDX, IDC_CHECK_KEY1, m_ctrlKey1);		// Key 1
	DDX_Control(pDX, IDC_EDIT_KEY1, m_ctrlKeyInput1);	// Key input 1

	DDX_Control(pDX, IDC_CHECK_KEY2, m_ctrlKey2);		// Key 2
	DDX_Control(pDX, IDC_EDIT_KEY2, m_ctrlKeyInput2);	// Key input 2

	DDX_Control(pDX, IDC_CHECK_KEY3, m_ctrlKey3);		// Key 3
	DDX_Control(pDX, IDC_EDIT_KEY3, m_ctrlKeyInput3);	// Key input 3

	DDX_Control(pDX, IDC_CHECK_KEY4, m_ctrlKey4);		// Key 4
	DDX_Control(pDX, IDC_EDIT_KEY4, m_ctrlKeyInput4);	// Key input 4

	DDX_Control(pDX, IDC_CHECK_KEY5, m_ctrlKey5);		// Key 5
	DDX_Control(pDX, IDC_EDIT_KEY5, m_ctrlKeyInput5);	// Key input 5

	DDX_Control(pDX, IDC_CHECK_KEY6, m_ctrlKey6);		// Key 6
	DDX_Control(pDX, IDC_EDIT_KEY6, m_ctrlKeyInput6);	// Key input 6

	DDX_Control(pDX, IDC_CHECK_KEY7, m_ctrlKey7);		// Key 7
	DDX_Control(pDX, IDC_EDIT_KEY7, m_ctrlKeyInput7);	// Key input 7

	DDX_Control(pDX, IDC_CHECK_LOG, m_ctrlLog);		// Log
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctrlLogInput);	// Log input 

	DDX_Control(pDX, IDC_ACTION_INSERT, m_ctrlActionInsert);
	DDX_Control(pDX, IDC_ACTION_DELETE, m_ctrlActionDelete);

	DDX_Control(pDX, IDC_CHAR_INSERT, m_ctrlCharInsert);
	DDX_Control(pDX, IDC_CHAR_DELETE, m_ctrlCharDelete);
	
	DDX_Control(pDX, IDC_LIST_ACTION, m_ListAcionBox);
	DDX_Control(pDX, IDC_LIST_CHAR, m_ListCharBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchDlg)
//	ON_BN_CLICKED(IDC_CHECK_FROM, OnCheckFrom)
	ON_BN_CLICKED(IDC_CHECK_IP, OnCheckIP)			// Map ID

	ON_BN_CLICKED(IDC_CHECK_MAPID, OnCheckMapID)		// Map ID
	
	ON_BN_CLICKED(IDC_CHECK_DK1, OnCheckDK1)			// DK 1
	ON_BN_CLICKED(IDC_CHECK_DK2, OnCheckDK2)			// DK 2
	ON_BN_CLICKED(IDC_CHECK_DK3, OnCheckDK3)			// DK 3
	ON_BN_CLICKED(IDC_CHECK_DK4, OnCheckDK4)			// DK 4
	ON_BN_CLICKED(IDC_CHECK_DK5, OnCheckDK5)			// DK 5
	ON_BN_CLICKED(IDC_CHECK_DK6, OnCheckDK6)			// DK 6
	ON_BN_CLICKED(IDC_CHECK_DK7, OnCheckDK7)			// DK 7
	ON_BN_CLICKED(IDC_CHECK_DK8, OnCheckDK8)			// DK 8
	ON_BN_CLICKED(IDC_CHECK_DK9, OnCheckDK9)			// DK 9

	ON_BN_CLICKED(IDC_CHECK_KEY1, OnCheckKey1)			// Key 1
	ON_BN_CLICKED(IDC_CHECK_KEY2, OnCheckKey2)			// Key 2
	ON_BN_CLICKED(IDC_CHECK_KEY3, OnCheckKey3)			// Key 3
	ON_BN_CLICKED(IDC_CHECK_KEY4, OnCheckKey4)			// Key 4
	ON_BN_CLICKED(IDC_CHECK_KEY5, OnCheckKey5)			// Key 5
	ON_BN_CLICKED(IDC_CHECK_KEY6, OnCheckKey6)			// Key 6
	ON_BN_CLICKED(IDC_CHECK_KEY7, OnCheckKey7)			// Key 7

	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)			// Log
	
	ON_BN_CLICKED(IDC_ACTION_INSERT, OnActionInsert)
	ON_BN_CLICKED(IDC_ACTION_DELETE, OnActionDelete)

	ON_BN_CLICKED(IDC_CHAR_INSERT, OnCharInsert)
	ON_BN_CLICKED(IDC_CHAR_DELETE, OnCharDelete)

	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_ACTION, OnCheckAction)
	ON_BN_CLICKED(IDOK, OnOk)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
//	CSearchDlg message handlers
////////////////////////////////////////////////////////////////////////////////
BOOL CSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	//날짜 외는 UNCHECK 한다. 날짜로 모든 필드를 찾는게 기본 쿼리
	m_ctrlDate.SetCheck(BST_CHECKED);
	m_ctrlDate.EnableWindow(FALSE);

	
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	
	SysTime.wDay = SysTime.wDay -1;
	
	m_ctrlDateInput.SetTime(&SysTime);
	m_ctrlDateInput2.SetTime(&SysTime);
	



	m_ctrlActionInput.EnableWindow(FALSE);

	m_ctrlIPInput.EnableWindow(FALSE);	// IP
	m_ctrlMapIDInput.EnableWindow(FALSE); // Map ID

	m_ctrlDKInput1.EnableWindow(FALSE);  // DK1
	m_ctrlDKInput2.EnableWindow(FALSE);  // DK2
	m_ctrlDKInput3.EnableWindow(FALSE);  // DK3
	m_ctrlDKInput4.EnableWindow(FALSE);  // DK4
	m_ctrlDKInput5.EnableWindow(FALSE);  // DK5
	m_ctrlDKInput6.EnableWindow(FALSE);  // DK6
	m_ctrlDKInput7.EnableWindow(FALSE);  // DK7
	m_ctrlDKInput8.EnableWindow(FALSE);  // DK8
	m_ctrlDKInput9.EnableWindow(FALSE);  // DK9


	m_ctrlKeyInput1.EnableWindow(FALSE); // key input 1
	m_ctrlKeyInput2.EnableWindow(FALSE); // key input 2
	m_ctrlKeyInput3.EnableWindow(FALSE); // key input 3
	m_ctrlKeyInput4.EnableWindow(FALSE); // key input 4
	m_ctrlKeyInput5.EnableWindow(FALSE); // key input 5
	m_ctrlKeyInput6.EnableWindow(FALSE); // key input 6
	m_ctrlKeyInput7.EnableWindow(FALSE); // key input 7

	m_ctrlLogInput.EnableWindow(FALSE); // Log input 

	m_ctrlActionInsert.EnableWindow(FALSE);
	m_ctrlActionDelete.EnableWindow(FALSE);
	m_ListAcionBox.EnableWindow(FALSE);

	m_ctrlCharInsert.EnableWindow(FALSE);
	m_ctrlCharDelete.EnableWindow(FALSE);
	m_ListCharBox.EnableWindow(FALSE);

//	m_ctrlUniqItemInput.EnableWindow(FALSE);
//	m_ctrlBaseItemInput.EnableWindow(FALSE);

	
	for(int i=0; i< nActionCount; i++) //NONE을 뺐음 //LATER
	{
		m_ctrlActionInput.InsertString(i, ACTIONLIST[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchDlg::OnCheckDate() 
{
	
}

void CSearchDlg::OnCheckIP()
{
	if(m_ctrlIP.GetCheck())
	{
		m_ctrlIPInput.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlIPInput.EnableWindow(FALSE);
		m_ctrlIPInput.SetWindowText("");
	}
}

void CSearchDlg::OnCheckMapID()
{

	if(	m_ctrlMapID.GetCheck() )
	{
		m_ctrlMapIDInput.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlMapIDInput.EnableWindow(FALSE);
		m_ctrlMapIDInput.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK1()
{
	if( m_ctrlDK1.GetCheck() )
	{
		m_ctrlDKInput1.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput1.EnableWindow(FALSE);
		m_ctrlDKInput1.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK2()
{
	if( m_ctrlDK2.GetCheck() )
	{
		m_ctrlDKInput2.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput2.EnableWindow(FALSE);
		m_ctrlDKInput2.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK3()
{
	if( m_ctrlDK3.GetCheck() )
	{
		m_ctrlDKInput3.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput3.EnableWindow(FALSE);
		m_ctrlDKInput3.SetWindowText("");		
	}
}

void CSearchDlg::OnCheckDK4()
{
	if( m_ctrlDK4.GetCheck() )
	{
		m_ctrlDKInput4.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput4.EnableWindow(FALSE);
		m_ctrlDKInput4.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK5()
{
	if( m_ctrlDK5.GetCheck() )
	{
		m_ctrlDKInput5.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput5.EnableWindow(FALSE);
		m_ctrlDKInput5.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK6()
{
	if( m_ctrlDK6.GetCheck() )
	{
		m_ctrlDKInput6.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput6.EnableWindow(FALSE);
		m_ctrlDKInput6.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK7()
{
	if( m_ctrlDK7.GetCheck() )
	{
		m_ctrlDKInput7.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput7.EnableWindow(FALSE);
		m_ctrlDKInput7.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK8()
{
	if( m_ctrlDK8.GetCheck() )
	{
		m_ctrlDKInput8.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput8.EnableWindow(FALSE);
		m_ctrlDKInput8.SetWindowText("");
	}
}

void CSearchDlg::OnCheckDK9()
{
	if( m_ctrlDK9.GetCheck() )
	{
		m_ctrlDKInput9.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlDKInput9.EnableWindow(FALSE);
		m_ctrlDKInput9.SetWindowText("");
	}
}

void CSearchDlg::OnCheckKey1()
{
	if( m_ctrlKey1.GetCheck() )
	{
		m_ctrlKeyInput1.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlKeyInput1.EnableWindow(FALSE);
		m_ctrlKeyInput1.SetWindowText("");
	}
}

void CSearchDlg::OnCheckKey2()
{
	if( m_ctrlKey2.GetCheck() )
	{
		m_ctrlKeyInput2.EnableWindow(TRUE);

		m_ctrlCharInsert.EnableWindow(TRUE);
		m_ctrlCharDelete.EnableWindow(TRUE);
		m_ListCharBox.EnableWindow(TRUE);		
	}
	else
	{
		m_ctrlKeyInput2.EnableWindow(FALSE);
		m_ctrlKeyInput2.SetWindowText("");

		m_ctrlCharInsert.EnableWindow(FALSE);
		m_ctrlCharDelete.EnableWindow(FALSE);
		m_ListCharBox.EnableWindow(FALSE);
		m_ListCharBox.ResetContent();
	}
}

void CSearchDlg::OnCheckKey3()
{
	if( m_ctrlKey3.GetCheck() )
	{
		m_ctrlKeyInput3.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlKeyInput3.EnableWindow(FALSE);
		m_ctrlKeyInput3.SetWindowText("");
	}
}

void CSearchDlg::OnCheckKey4()
{
	if( m_ctrlKey4.GetCheck() )
	{
		m_ctrlKeyInput4.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlKeyInput4.EnableWindow(FALSE);
		m_ctrlKeyInput4.SetWindowText("");
	}
}

void CSearchDlg::OnCheckKey5()
{
	if( m_ctrlKey5.GetCheck() )
	{
		m_ctrlKeyInput5.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlKeyInput5.EnableWindow(FALSE);
		m_ctrlKeyInput5.SetWindowText("");
	}
}

void CSearchDlg::OnCheckKey6()
{
	if( m_ctrlKey6.GetCheck() )
	{
		m_ctrlKeyInput6.EnableWindow(TRUE);
	}
	else
	{
		m_ctrlKeyInput6.EnableWindow(FALSE);
		m_ctrlKeyInput6.SetWindowText("");
	}
}

void CSearchDlg::OnActionInsert()
{
	int nIndx = m_ctrlActionInput.GetCurSel();
	if( nIndx > -1)
	{
		CString str = _T("");
		m_ctrlActionInput.GetLBText(nIndx, str);
		int nRet = m_ListAcionBox.FindString(0, str);
	
		if (nRet < 0)
			m_ListAcionBox.InsertString(0, str);

	}
}

void CSearchDlg::OnActionDelete()
{
	// TODO: Add your control notification handler code here
	int nIndx = m_ListAcionBox.GetCurSel();
	if(nIndx > -1)
		m_ListAcionBox.DeleteString(nIndx);	
}

void CSearchDlg::OnCharInsert()
{
	// TODO: Add your control notification handler code here
	CString str;

	m_ctrlKeyInput2.GetWindowText(str);
	if(0 != str.GetLength())
	{
		int nRet = m_ListCharBox.FindString(0, str);
	
		if (nRet < 0)
			m_ListCharBox.InsertString(0, str);
	}

}

void CSearchDlg::OnCharDelete()
{
	// TODO: Add your control notification handler code here

	int nIndx = m_ListCharBox.GetCurSel();
	if(nIndx > -1)
		m_ListCharBox.DeleteString(nIndx);
		
}

void CSearchDlg::OnCheckKey7()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_ctrlKey7.GetCheck();
	if(1 == nCheck)
	{
		m_ctrlKeyInput7.EnableWindow(TRUE);
	}
	else if(0 == nCheck)
	{
		m_ctrlKeyInput7.EnableWindow(FALSE);
		m_ctrlKeyInput7.SetWindowText("");
	}
}


void CSearchDlg::OnCheckLog()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_ctrlLog.GetCheck();
	if(1 == nCheck)
	{
		m_ctrlLogInput.EnableWindow(TRUE);
	}
	else if(0 == nCheck)
	{
		m_ctrlLogInput.EnableWindow(FALSE);
		m_ctrlLogInput.SetWindowText("");
	}
}

void CSearchDlg::OnCheckAction() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_ctrlAction.GetCheck();
	if(1 == nCheck)
	{
		m_ctrlActionInput.EnableWindow(TRUE);

		m_ctrlActionInsert.EnableWindow(TRUE);
		m_ctrlActionDelete.EnableWindow(TRUE);
		m_ListAcionBox.EnableWindow(TRUE);

	}
	else if(0 == nCheck)
	{
		m_ctrlActionInput.EnableWindow(FALSE);
		m_ctrlActionInput.SetCurSel(-1);

		m_ctrlActionInsert.EnableWindow(FALSE);
		m_ctrlActionDelete.EnableWindow(FALSE);

		m_ListAcionBox.EnableWindow(FALSE);
		m_ListAcionBox.ResetContent();
	}
}


//
//	Search the logs
//
void CSearchDlg::OnOk() 
{
	CString					strValue;
	vector<QUERY_CONDITION>	query;		
	QUERY_CONDITION			condition;
	BOOL					bCondition = FALSE;

	//
	//	Action
	//
	if(m_ctrlAction.GetCheck() && m_ListAcionBox.GetCount() )
	{
		int nSize = m_ListAcionBox.GetCount();

		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_ACTION;
		
		for(int i=0; i < nSize; i++)
		{
			m_ListAcionBox.GetText(i, strValue);

			int idx = 0;
			for(int j=0; j < nActionCount; j++)
			{					
				if(0 == _tcscmp(ACTIONLIST[j], strValue))
				{
					idx = ActionNum[j];
					break;
				}
			}

			char cTemp[4];
			memset(cTemp, 0x00, sizeof(cTemp));
			itoa(idx, cTemp, 10);	

			_tcscat(condition.value3, (LPCTSTR)cTemp);
			
			int nNum = i+1;
			if( nSize != nNum )
				_tcscat(condition.value3, ",");				
		}			

		query.push_back(condition);	
		bCondition = TRUE;
	}
	
	//
	//	IP
	//
	m_ctrlIPInput.GetWindowText(strValue);
	if(strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_CLIENTIP;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Map ID
	//
	m_ctrlMapIDInput.GetWindowText(strValue);
	if(strValue.GetLength())
	{
		DWORD dwID = atoi((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_MAPID;
		condition.value2	= dwID;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}


	//
	//	DK1
	//
	m_ctrlDKInput1.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK1;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK2
	//
	m_ctrlDKInput2.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK2;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK3
	//
	m_ctrlDKInput3.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK3;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK4
	//
	m_ctrlDKInput4.GetWindowText(strValue);
	if(0 != strValue.GetLength())
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK4;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK5
	//
	m_ctrlDKInput5.GetWindowText(strValue);
	if(0 != strValue.GetLength())
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK5;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK6
	//
	m_ctrlDKInput6.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK6;
		//condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	DK7
	//
	m_ctrlDKInput7.GetWindowText(strValue);
	if(0 != strValue.GetLength())
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK7;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}



	//
	//	DK8
	//
	m_ctrlDKInput8.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK8;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	m_ctrlDKInput9.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		__int64 i64k = _atoi64((LPCTSTR)strValue);
	
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_DK9;
		condition.value4	= i64k;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}


	//
	//	Key1
	//
	m_ctrlKeyInput1.GetWindowText(strValue);
	if(0 != strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY1;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}
	

	if(m_ctrlKey2.GetCheck())
	{
		//리스트 박스의 숫자가 0 이상일때만
		int nSize = m_ListCharBox.GetCount();
		if(nSize > 0)
		{
			memset(&condition, 0x00, sizeof(QUERY_CONDITION));
			condition.Idx		= LT_KEY2;
		
			for(int i=0; i < nSize; i++)
			{
				m_ListCharBox.GetText(i, strValue);			

				_tcscat(condition.value3, "'");
				_tcscat(condition.value3, strValue);
				_tcscat(condition.value3, "'");
				
				int nNum = i+1;
				if( nSize != nNum )
					_tcscat(condition.value3, ",");
			}			

			query.push_back(condition);	
			bCondition = TRUE;
		}

	}


	//
	//	Key3
	//
	m_ctrlKeyInput3.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY3;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Key4
	//
	m_ctrlKeyInput4.GetWindowText(strValue);
	if( strValue.GetLength() )
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY4;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Key5
	//
	m_ctrlKeyInput5.GetWindowText(strValue);
	if(0 != strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY5;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Key6
	//
	m_ctrlKeyInput6.GetWindowText(strValue);
	if(0 != strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY6;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Key7
	//
	m_ctrlKeyInput7.GetWindowText(strValue);
	if(0 != strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_KEY7;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Log
	//
	m_ctrlLogInput.GetWindowText(strValue);
	if(0 != strValue.GetLength())	
	{
		memset(&condition, 0x00, sizeof(QUERY_CONDITION));
		condition.Idx		= LT_LOG;
		_tcscpy(condition.value3, (LPCTSTR)strValue);
		query.push_back(condition);

		bCondition = TRUE;
	}

	//
	//	Date & Time
	//
	m_ctrlDateInput.GetTime(m_DateFrom);	
	m_ctrlDateInput2.GetTime(m_DateTo);
	
	memset(m_strConditions, 0x00, sizeof(m_strConditions));
	_tcscpy(m_strConditions, "");

	//
	//	Make Condtion
	//
	if(bCondition)
	{
		_tcscat(m_strConditions, _T(" WHERE "));
	}
	
	int i, j = query.size();

	for(i=0; i<j; ++i)
	{
		_tcscat(m_strConditions, DBFIELDSTR[query[i].Idx]);

		//
		//	Search Operation
		//		
		switch(query[i].Idx) 
		{
		case	LT_ACTION	:
		case	LT_KEY1		:
		case	LT_KEY2		:
			{
				_tcscat(m_strConditions, _T(" IN "));
			}
			break;

		case	LT_CLIENTIP	:
		case	LT_KEY3	:
		case	LT_KEY4	:
		case	LT_KEY5	:
		case	LT_KEY6	:
		case	LT_KEY7	:
		case	LT_LOG	:
			{
				_tcscat(m_strConditions, _T(" LIKE "));
			}
			break;

		case	LT_DK6	:
		case	LT_DK7	:
			{
				_tcscat(m_strConditions, _T(" >= "));
			}
			break;

		default	:
			{
				_tcscat(m_strConditions, _T(" = "));
			}
			break;
		}


		//
		//	Search Data
		//
		switch(query[i].Idx)
		{
		case LT_LOGDATE:
		case LT_KEY1:		// 계정 
			{
				//스트링 값이면 값과 '' 넣는다.
				_tcscat(m_strConditions, _T("'"));
				_tcscat(m_strConditions, _tcsupr((LPTSTR)(LPCTSTR)query[i].value3));
				_tcscat(m_strConditions, _T("'"));
			}
			break;

		// IN
		case LT_ACTION:		// 엑션 
		case LT_KEY2:		// 케릭터
			{
				_tcscat(m_strConditions, _T("("));
				_tcscat(m_strConditions, _tcsupr((LPTSTR)(LPCTSTR)query[i].value3));
				_tcscat(m_strConditions, _T(")"));
			}
			break;


		//	LIKE 
		case LT_CLIENTIP:	
		case LT_KEY3:
		case LT_KEY4:
		case LT_KEY5:
		case LT_KEY6:
		case LT_KEY7:
		case LT_LOG:
			{
				_tcscat(m_strConditions, _T("('%"));
				_tcscat(m_strConditions, _tcsupr((LPTSTR)(LPCTSTR)query[i].value3));
				_tcscat(m_strConditions, _T("%')"));
			}
			break;


		default:
			{
				//스트링이 아니면
				_tcscat( m_strConditions, query[i].value3 );
			}
			break;
		}

		//조건이 더 있으면 
		if(i+1 < j)
		_tcscat(m_strConditions, _T(" AND "));
	}

	//
	//	ORDERBY
	//
	_tcscat(m_strConditions, _T(" "));
	_tcscat(m_strConditions, SQLQUERY_ORDERBY);

	this->ShowWindow(SW_HIDE);
	
	GETINSTANCE()->Search(m_DateFrom, m_DateTo, m_strConditions);
}