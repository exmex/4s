// SaveMinimapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SaveMinimapDlg.h"


// CSaveMinimapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaveMinimapDlg, CDialog)
CSaveMinimapDlg::CSaveMinimapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveMinimapDlg::IDD, pParent)
	, m_nImageSize(512)
	, m_bOBJCollision(FALSE)
{
}

CSaveMinimapDlg::~CSaveMinimapDlg()
{
}

void CSaveMinimapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMAGESIZE, m_nImageSize);
	DDX_Check(pDX, IDC_CHECK_MINIMAP_OBJ_COLLISION_AREA, m_bOBJCollision);
}


BEGIN_MESSAGE_MAP(CSaveMinimapDlg, CDialog)
END_MESSAGE_MAP()


// CSaveMinimapDlg 메시지 처리기입니다.
