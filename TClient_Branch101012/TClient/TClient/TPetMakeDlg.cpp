#include "Stdafx.h"
#include "TPetMakeDlg.h"
#include "Resource.h"

// ======================================================================
CTPetMakeDlg::CTPetMakeDlg(TComponent* pParent, LP_FRAMEDESC pDesc, CD3DDevice* pDevice)
	: CTPetDlg(pParent, pDesc, pDevice)
{
	m_pMakeButton = FindKid(ID_CTRLINST_BTN_MAKE);

	static const DWORD dwLINEID[] = 
	{
		ID_CTRLINST_TXT_LINE1,
		ID_CTRLINST_TXT_LINE2,
		ID_CTRLINST_TXT_LINE3,
		ID_CTRLINST_TXT_LINE4
	};

	for(INT i=0; i<MAX_LINE; ++i)
	{
		m_pLines[i] = FindKid( dwLINEID[i] );
		m_pLines[i]->m_strText.Empty();
	}
}
// ----------------------------------------------------------------------
CTPetMakeDlg::~CTPetMakeDlg()
{
}
// ======================================================================

// ======================================================================
CString CTPetMakeDlg::GetEndTimeStr()
{
	return m_strEndTime;
}
// ======================================================================
void CTPetMakeDlg::Update(DWORD dwTickCount)
{
	if( !m_pDisplayPet )
		return;

	LPTPET pPetTemp = m_pDisplayPet->GetPetTemp();
	if( !pPetTemp )
		return;

	const CString& strPetName = m_pDisplayPet->GetPetName();
	if( strPetName.IsEmpty() )
		m_pKindNameTxt->m_strText = m_pDisplayPet->GetPetKindName();
	else
		m_pKindNameTxt->m_strText = CTChart::Format( TSTR_PETMAKE_KINDNAME, m_pDisplayPet->GetPetKindName(), strPetName);

	CString str;
	INT nCurLine = 0;

	str = KindToString(pPetTemp->m_bRecallKind1, pPetTemp->m_wRecallValue1);
	if( !str.IsEmpty() )
		m_pLines[nCurLine++]->m_strText = str;

	str = KindToString(pPetTemp->m_bRecallKind2, pPetTemp->m_wRecallValue2);
	if( !str.IsEmpty() )
		m_pLines[nCurLine++]->m_strText = str;

	str = CondToString(pPetTemp->m_bConditionType, pPetTemp->m_dwConditionValue);
	if( !str.IsEmpty() )
		m_pLines[nCurLine++]->m_strText = str;

	str = GetEndTimeStr();
	if( !str.IsEmpty() )
		m_pLines[nCurLine++]->m_strText = str;

	for(INT i=nCurLine; i<MAX_LINE; ++i)
		m_pLines[i]->m_strText.Empty();

	m_bNeedUpdate = FALSE;
}