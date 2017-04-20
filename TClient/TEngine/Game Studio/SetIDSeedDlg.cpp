// SetIDSeedDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SetIDSeedDlg.h"


// CSetIDSeedDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetIDSeedDlg, CDialog)
CSetIDSeedDlg::CSetIDSeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIDSeedDlg::IDD, pParent)
	, m_dwID_Animation(0)
	, m_dwID_ObjectType(0)
	, m_dwID_Texture(0)
	, m_dwID_Image(0)
	, m_dwID_Media(0)
	, m_dwID_Frame(0)
	, m_dwID_Component(0)
	, m_dwID_Mesh(0)
	, m_dwID_Map(0)
	, m_dwID_Object(0)
	, m_dwID_SFX(0)
	, m_dwSeed(0)
{
}

CSetIDSeedDlg::~CSetIDSeedDlg()
{
}

void CSetIDSeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ID_ANIMATION, m_dwID_Animation);
	DDX_Text(pDX, IDC_STATIC_ID_OBJECTTYPE, m_dwID_ObjectType);
	DDX_Text(pDX, IDC_STATIC_ID_TEXTURE, m_dwID_Texture);
	DDX_Text(pDX, IDC_STATIC_ID_IMAGE, m_dwID_Image);
	DDX_Text(pDX, IDC_STATIC_ID_MEDIA, m_dwID_Media);
	DDX_Text(pDX, IDC_STATIC_ID_FRAME, m_dwID_Frame);
	DDX_Text(pDX, IDC_STATIC_ID_COMPONENT, m_dwID_Component);
	DDX_Text(pDX, IDC_STATIC_ID_MESH, m_dwID_Mesh);
	DDX_Text(pDX, IDC_STATIC_ID_MAP, m_dwID_Map);
	DDX_Text(pDX, IDC_STATIC_OBJECT, m_dwID_Object);
	DDX_Text(pDX, IDC_STATIC_ID_SFX, m_dwID_SFX);
	DDX_Text(pDX, IDC_EDIT1, m_dwSeed);
	DDV_MinMaxUInt(pDX, m_dwSeed, 0, 999999999);
}


BEGIN_MESSAGE_MAP(CSetIDSeedDlg, CDialog)
END_MESSAGE_MAP()


// CSetIDSeedDlg 메시지 처리기입니다.
