// AdjustOBJPosDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AdjustOBJPosDlg.h"
#include ".\adjustobjposdlg.h"


// CAdjustOBJPosDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAdjustOBJPosDlg, CDialog)
CAdjustOBJPosDlg::CAdjustOBJPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdjustOBJPosDlg::IDD, pParent)
	, m_strUnitX(_T("0"))
	, m_strUnitZ(_T("0"))
	, m_bSpecifyUnit(FALSE)
	, m_ptUnitXZ(CPoint(0,0))
	, m_bOBJMoveOFFSET(FALSE)
	, m_strOBJOffsetX(_T(""))
	, m_strOBJOffsetZ(_T(""))
	, m_vMoveOffset(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_bAttachType(FALSE)
{
}
CAdjustOBJPosDlg::CAdjustOBJPosDlg(CMAPDoc* pDoc, CWnd* pParent /*= NULL*/)
	: CDialog(CAdjustOBJPosDlg::IDD, pParent)
	, m_strUnitX(_T("0"))
	, m_strUnitZ(_T("0"))
	, m_bSpecifyUnit(FALSE)
	, m_pDoc((CEXTMAPDoc*)pDoc)
	, m_ptUnitXZ(CPoint(0,0))
	, m_bOBJMoveOFFSET(FALSE)
	, m_strOBJOffsetX(_T(""))
	, m_strOBJOffsetZ(_T(""))
	, m_vMoveOffset(D3DXVECTOR3(0.0f,0.0f,0.0f))
	, m_bAttachType(FALSE)
{
}

CAdjustOBJPosDlg::~CAdjustOBJPosDlg()
{
}

void CAdjustOBJPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UNIT_X, m_cUnitX);
	DDX_Control(pDX, IDC_COMBO_UNIT_Z, m_cUnitZ);
	DDX_CBString(pDX, IDC_COMBO_UNIT_X, m_strUnitX);
	DDX_CBString(pDX, IDC_COMBO_UNIT_Z, m_strUnitZ);
	DDX_Radio(pDX, IDC_RADIO_SPECIFY_OBJ, m_bSpecifyUnit);
	DDX_Check(pDX, IDC_CHECK_OBJ_MOVE_OFFSET, m_bOBJMoveOFFSET);
	DDX_Control(pDX, IDC_EDIT_MOVE_OFFSET_X, m_cOBJOffsetX);
	DDX_Control(pDX, IDC_EDIT_MOVE_OFFSET_Z, m_cOBJOffsetZ);
	DDX_Text(pDX, IDC_EDIT_MOVE_OFFSET_X, m_strOBJOffsetX);
	DDX_Text(pDX, IDC_EDIT_MOVE_OFFSET_Z, m_strOBJOffsetZ);
	DDX_Radio(pDX, IDC_RADIO_REPLACE, m_bAttachType);
}


BEGIN_MESSAGE_MAP(CAdjustOBJPosDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SPECIFY_OBJ, OnCheckEnable)
	ON_BN_CLICKED(IDC_RADIO_ALLUNIT_OBJ, OnCheckEnable)
	ON_BN_CLICKED(IDC_RADIO_CURRENT_UNIT_OBJ, OnCheckEnable)
	ON_BN_CLICKED(IDC_CHECK_OBJ_MOVE_OFFSET, OnCheckEnable)
	ON_CBN_DROPDOWN(IDC_COMBO_UNIT_X, OnCbnDropdownComboUnitX)
	ON_CBN_DROPDOWN(IDC_COMBO_UNIT_Z, OnCbnDropdownComboUnitZ)
END_MESSAGE_MAP()


// CAdjustOBJPosDlg 메시지 처리기입니다.
void CAdjustOBJPosDlg::OnCheckEnable()
{
	UpdateData();
	EnableAllCtrl(TRUE);
}
void CAdjustOBJPosDlg::EnableAllCtrl(bool bEnable)
{
	m_cUnitX.EnableWindow(bEnable && !m_bSpecifyUnit);
	m_cUnitZ.EnableWindow(bEnable && !m_bSpecifyUnit);

	m_cOBJOffsetX.EnableWindow(bEnable && m_bOBJMoveOFFSET);
	m_cOBJOffsetZ.EnableWindow(bEnable && m_bOBJMoveOFFSET);
}

void CAdjustOBJPosDlg::OnCbnDropdownComboUnitX()
{
	m_cUnitX.ResetContent();
	
	CString str;	
	for( int i = 0 ; i < m_pDoc->m_pItem->m_HugeMAP.m_nUnitCountX ; i++ )
	{
		str.Format("%d",i * m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength);
		m_cUnitX.AddString(str);
	}
}

void CAdjustOBJPosDlg::OnCbnDropdownComboUnitZ()
{
	m_cUnitZ.ResetContent();

	CString str;
	for( int i = 0 ; i < m_pDoc->m_pItem->m_HugeMAP.m_nUnitCountZ ; i++ )
	{
		str.Format("%d",i * m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength);
		m_cUnitZ.AddString(str);
	}
}

void CAdjustOBJPosDlg::OnOK()
{
	UpdateData();

	CPoint pt = CPoint(-1,-1);
	D3DXVECTOR3 vMovePos(0.0f,0.0f,0.0f);
	if( !m_bSpecifyUnit )
	{
		if( m_strUnitX.IsEmpty() || m_strUnitZ.IsEmpty() || 
			INT(atoi(m_strUnitX) % m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength != 0 ) || 
			INT(atoi(m_strUnitZ) % m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength != 0 ) ||
			INT(atoi(m_strUnitX) > m_pDoc->m_pItem->m_HugeMAP.m_nUnitCountX * m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength ) || 
			INT(atoi(m_strUnitZ) > m_pDoc->m_pItem->m_HugeMAP.m_nUnitCountZ * m_pDoc->m_pItem->m_HugeMAP.m_nUnitLength ))
		{
			UpdateData(FALSE);
			return;
		}
		pt = CPoint(INT(atoi(m_strUnitX)),INT(atoi(m_strUnitZ)));
	}

	if( m_bOBJMoveOFFSET )
	{
		if( m_strOBJOffsetX.IsEmpty() || m_strOBJOffsetZ.IsEmpty() )
		{
			UpdateData(FALSE);
			return;
		}
		vMovePos = D3DXVECTOR3(FLOAT(atof(m_strOBJOffsetX) ),0.0f,FLOAT( atof(m_strOBJOffsetZ) ));
	}
	
	m_ptUnitXZ = pt;
	m_vMoveOffset = vMovePos;
	CDialog::OnOK();
}

CPoint CAdjustOBJPosDlg::GetUnitXZ(void)
{
	return m_ptUnitXZ;
}

D3DXVECTOR3 CAdjustOBJPosDlg::GetOBJMoveOffset(void)
{
	return m_vMoveOffset;
}

BOOL CAdjustOBJPosDlg::GetAttachType(void)
{
	return m_bAttachType;
}
