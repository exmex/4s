// TMonsterChartSet.h : CTMonsterChartSet 클래스의 구현입니다.



// CTMonsterChartSet 구현입니다.

// 코드 생성 위치 2008년 1월 16일 수요일, 오후 5:33

#include "stdafx.h"
#include "TMonsterChartSet.h"
IMPLEMENT_DYNAMIC(CTMonsterChartSet, CRecordset)

CTMonsterChartSet::CTMonsterChartSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_wID = 0;
	m_szName = "";
	m_bRace = 0;
	m_bClass = 0;
	m_wKind = 0;
	m_bLevel = 0;
	m_bAIType = 0;
	m_bRange = 0;
	m_wChaseRange = 0;
	m_bRoamProb = 0;
	m_bMoneyProb = 0;
	m_dwMinMoney = 0;
	m_dwMaxMoney = 0;
	m_bItemProb = 0;
	m_bDropCount = 0;
	m_wExp = 0;
	m_bIsSelf = 0;
	m_bRecallType = 0;
	m_bCanSelect = 0;
	m_bCanAttack = 0;
	m_bTame = 0;
	m_bCall = 0;
	m_bIsSpecial = 0;
	m_bRemove = 0;
	m_wMonAttr = 0;
	m_wSummonAttr = 0;
	m_wTransSkillID = 0;
	m_fSize = 0.0;
	m_wSkill1 = 0;
	m_wSkill2 = 0;
	m_wSkill3 = 0;
	m_wSkill4 = 0;
	m_nFields = 32;
	m_nDefaultType = snapshot;
}

CString CTMonsterChartSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=TQUEST");
}

CString CTMonsterChartSet::GetDefaultSQL()
{
	return _T("[dbo].[TMONSTERCHART]");
}

void CTMonsterChartSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 및 RFX_Int() 같은 매크로는 데이터베이스의 필드
// 형식이 아니라 멤버 변수의 형식에 따라 달라집니다.
// ODBC에서는 자동으로 열 값을 요청된 형식으로 변환하려고 합니다
	RFX_Int(pFX, _T("[wID]"), m_wID);
	RFX_Text(pFX, _T("[szName]"), m_szName);
	RFX_Byte(pFX, _T("[bRace]"), m_bRace);
	RFX_Byte(pFX, _T("[bClass]"), m_bClass);
	RFX_Int(pFX, _T("[wKind]"), m_wKind);
	RFX_Byte(pFX, _T("[bLevel]"), m_bLevel);
	RFX_Byte(pFX, _T("[bAIType]"), m_bAIType);
	RFX_Byte(pFX, _T("[bRange]"), m_bRange);
	RFX_Int(pFX, _T("[wChaseRange]"), m_wChaseRange);
	RFX_Byte(pFX, _T("[bRoamProb]"), m_bRoamProb);
	RFX_Byte(pFX, _T("[bMoneyProb]"), m_bMoneyProb);
	RFX_Long(pFX, _T("[dwMinMoney]"), m_dwMinMoney);
	RFX_Long(pFX, _T("[dwMaxMoney]"), m_dwMaxMoney);
	RFX_Byte(pFX, _T("[bItemProb]"), m_bItemProb);
	RFX_Byte(pFX, _T("[bDropCount]"), m_bDropCount);
	RFX_Int(pFX, _T("[wExp]"), m_wExp);
	RFX_Byte(pFX, _T("[bIsSelf]"), m_bIsSelf);
	RFX_Byte(pFX, _T("[bRecallType]"), m_bRecallType);
	RFX_Byte(pFX, _T("[bCanSelect]"), m_bCanSelect);
	RFX_Byte(pFX, _T("[bCanAttack]"), m_bCanAttack);
	RFX_Byte(pFX, _T("[bTame]"), m_bTame);
	RFX_Byte(pFX, _T("[bCall]"), m_bCall);
	RFX_Byte(pFX, _T("[bIsSpecial]"), m_bIsSpecial);
	RFX_Byte(pFX, _T("[bRemove]"), m_bRemove);
	RFX_Int(pFX, _T("[wMonAttr]"), m_wMonAttr);
	RFX_Int(pFX, _T("[wSummonAttr]"), m_wSummonAttr);
	RFX_Int(pFX, _T("[wTransSkillID]"), m_wTransSkillID);
	RFX_Single(pFX, _T("[fSize]"), m_fSize);
	RFX_Int(pFX, _T("[wSkill1]"), m_wSkill1);
	RFX_Int(pFX, _T("[wSkill2]"), m_wSkill2);
	RFX_Int(pFX, _T("[wSkill3]"), m_wSkill3);
	RFX_Int(pFX, _T("[wSkill4]"), m_wSkill4);

}
/////////////////////////////////////////////////////////////////////////////
// CTMonsterChartSet 진단

#ifdef _DEBUG
void CTMonsterChartSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CTMonsterChartSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


