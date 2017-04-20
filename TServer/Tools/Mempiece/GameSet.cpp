// GameSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Mempiece.h"
#include "GameSet.h"
#include ".\gameset.h"


// CGameSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGameSet, CDialog)
CGameSet::CGameSet(CWnd* pParent /*=NULL*/)
	: CDialog(CGameSet::IDD, pParent)
	, m_bWindow(FALSE)
	, m_bShader(FALSE)
	, m_bChar(FALSE)
	, m_bPaper(FALSE)
	, m_bBack(FALSE)
{
}

CGameSet::~CGameSet()
{
}

void CGameSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bWindow);
	DDX_Check(pDX, IDC_CHECK2, m_bShader);
	DDX_Radio(pDX, IDC_RADIO2, m_bChar);
	DDX_Radio(pDX, IDC_RADIO5, m_bPaper);
	DDX_Radio(pDX, IDC_RADIO6, m_bBack);
}


BEGIN_MESSAGE_MAP(CGameSet, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_RADIO10, OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO11, OnBnClickedRadio11)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO5, OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnBnClickedRadio9)
END_MESSAGE_MAP()


// CGameSet 메시지 처리기입니다.
////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic Mode Set / Get
void CGameSet::SetGraphicMode(DWORD window, DWORD shader, DWORD character, DWORD paper, DWORD back)
{
	m_bWindow = window;
	m_bShader = shader;
	m_bChar = character;
	m_bPaper = paper;
	m_bBack = back;
}
void CGameSet::GetGraphicMode(DWORD *window, DWORD *shader, DWORD *character, DWORD *paper, DWORD *back)
{
	*window = m_bWindow;
	*shader = m_bShader;
	*character = m_bChar;
	*paper = m_bPaper;
	*back = m_bBack;
}

// Window / Shader Mode Check Box
void CGameSet::OnBnClickedCheck1()
{		
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedCheck2()
{
	UpdateData(TRUE);	
}

// Graphic Mode Radio Button
void CGameSet::OnBnClickedRadio10()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio11()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio2()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio3()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio5()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio6()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio7()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio8()
{
	UpdateData(TRUE);
}
void CGameSet::OnBnClickedRadio9()
{
	UpdateData(TRUE);
}
////////////////////////////////////////////////////////////////////////////////////////////////////