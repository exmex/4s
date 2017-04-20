// SavePos.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "SavePos.h"
#include ".\savepos.h"

#include "MainFrm.h"
#include "HappyDoc.h"

// CSavePos 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSavePos, CDialog)
CSavePos::CSavePos(CWnd* pParent /*=NULL*/)
	: CDialog(CSavePos::IDD, pParent)
{
}

CSavePos::~CSavePos()
{
}


BEGIN_MESSAGE_MAP(CSavePos, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CSavePos 메시지 처리기입니다.
void CSavePos::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;

	GetDlgItemText(IDC_EDIT_SAVENAME, strTemp);

	if( strTemp.IsEmpty())
		AfxMessageBox(_T("You should enter the name"), MB_OK); // 이름을 입력해야 합니다
	else if(m_pstrSaveName)
	{
		m_pstrSaveName->Format("%s",strTemp);
		OnOK();
		return;
	}
	OnCancel();
}