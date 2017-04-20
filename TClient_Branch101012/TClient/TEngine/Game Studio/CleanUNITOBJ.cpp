// CleanUNITOBJ.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EXTMAPDoc.h"
#include "CleanUNITOBJ.h"
#include ".\cleanunitobj.h"


// CCleanUNITOBJ 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCleanUNITOBJ, CDialog)
CCleanUNITOBJ::CCleanUNITOBJ(CWnd* pParent /*=NULL*/)
	: CDialog(CCleanUNITOBJ::IDD, pParent)
	, m_strUNITX(_T(""))
	, m_strUNITZ(_T(""))
{
}

CCleanUNITOBJ::~CCleanUNITOBJ()
{
}

void CCleanUNITOBJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UNIT_POS_X, m_cUNITX);
	DDX_Control(pDX, IDC_COMBO_UNIT_POS_Z, m_cUNITZ);
	DDX_CBString(pDX, IDC_COMBO_UNIT_POS_X, m_strUNITX);
	DDX_CBString(pDX, IDC_COMBO_UNIT_POS_Z, m_strUNITZ);
}

BEGIN_MESSAGE_MAP(CCleanUNITOBJ, CDialog)
	ON_BN_CLICKED(ID_BUTTON_CLEAN_UNIT_OBJ, OnCleanUNITOBJ)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_ALL, OnBnClickedButtonCleanAll)
END_MESSAGE_MAP()


// CCleanUNITOBJ 메시지 처리기입니다.

void CCleanUNITOBJ::InitData(CEXTMAPItem* pItem)
{
	m_pItem = pItem;
}

BOOL CCleanUNITOBJ::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString str;
	for(int i=0; i<m_pItem->m_HugeMAP.m_nUnitCountX; i++)
	{	
		str.Format("%d", i * m_pItem->m_HugeMAP.m_nUnitLength);
		m_cUNITX.InsertString(i,str);
	}

	for(i=0; i<m_pItem->m_HugeMAP.m_nUnitCountZ; i++)
	{	
		str.Format("%d", i * m_pItem->m_HugeMAP.m_nUnitLength);
		m_cUNITZ.InsertString(i,str);
	}

	return TRUE;
}

void CCleanUNITOBJ::OnCleanUNITOBJ()
{
	CEXTMAPDoc* pDoc = (CEXTMAPDoc*)m_pItem->GetMainDocument();
	
	int nUNITX = m_cUNITX.GetCurSel();
	int nUNITZ = m_cUNITZ.GetCurSel();
	
	if( nUNITX == -1 || nUNITZ == -1 )
		return;
	MAPMAPOBJ mapOBJ;
	MAPMAPSFX mapSFX;
	MAPMAPSND mapSND;

	MAPDWORD  mapOBJID;
	MAPDWORD  mapSFXID;

	DWORD dwIndex = nUNITX + (nUNITZ * m_pItem->m_HugeMAP.m_nUnitCountX);
	m_pItem->UpdateOBJ(
		&mapOBJ,
		&mapSFX,
		&mapSND,
		&mapOBJID,
		&mapSFXID,
		dwIndex);

	m_pItem->BuildSHADOWTEX(
		m_pItem->m_HugeMAP.m_nShadowPixel,
		dwIndex);

	if(pDoc)
	{
		pDoc->m_vSELOBJBUF.clear();
		pDoc->m_vSELOBJ.clear();
		pDoc->m_vSELSFX.clear();
		pDoc->m_vSELSND.clear();

		pDoc->m_mapFREEZE.clear();
		pDoc->m_mapHIDE.clear();

		pDoc->UpdateOBJBAR();
		pDoc->UpdateSFXBAR();
		pDoc->UpdateSNDBAR();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}

void CCleanUNITOBJ::OnBnClickedButtonCleanAll()
{
	CEXTMAPDoc* pDoc = (CEXTMAPDoc*)m_pItem->GetMainDocument();

	int nCountX = m_cUNITX.GetCount();
	int nCountZ = m_cUNITZ.GetCount();

	for(int nUNITX = 0; nUNITX < nCountX; nUNITX++)
		for(int nUNITZ = 0; nUNITZ < nCountZ; nUNITZ++)
		{
			if( nUNITX == -1 || nUNITZ == -1 )
				return;

			MAPMAPOBJ mapOBJ;
			MAPMAPSFX mapSFX;
			MAPMAPSND mapSND;

			MAPDWORD  mapOBJID;
			MAPDWORD  mapSFXID;

			DWORD dwIndex = nUNITX + (nUNITZ * m_pItem->m_HugeMAP.m_nUnitCountX);
			m_pItem->UpdateOBJ(
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID,
				dwIndex);

			m_pItem->BuildSHADOWTEX(
				m_pItem->m_HugeMAP.m_nShadowPixel,
				dwIndex);
		}

	if(pDoc)
	{
		pDoc->m_vSELOBJBUF.clear();
		pDoc->m_vSELOBJ.clear();
		pDoc->m_vSELSFX.clear();
		pDoc->m_vSELSND.clear();

		pDoc->m_mapFREEZE.clear();
		pDoc->m_mapHIDE.clear();

		pDoc->UpdateOBJBAR();
		pDoc->UpdateSFXBAR();
		pDoc->UpdateSNDBAR();

		pDoc->Reload();
		pDoc->RefreshView();
	}
}
