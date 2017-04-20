#if !defined(AFX_TQUESTSET_H__E840F084_1A5C_457A_966D_956BF418A27F__INCLUDED_)
#define AFX_TQUESTSET_H__E840F084_1A5C_457A_966D_956BF418A27F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TQuestSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTQuestSet recordset

class CTQuestSet : public CRecordset
{
public:
	CTQuestSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTQuestSet)

// Field/Param Data
	//{{AFX_FIELD(CTQuestSet, CRecordset)
	long	m_dwQuestID;
	long	m_dwParentID;
	BYTE	m_bType;
	BYTE	m_bTriggerType;
	long	m_dwTriggerID;
	BYTE	m_bCountMax;
	BYTE	m_bForceRun;
	BYTE	m_bLevel;
	BYTE	m_bMain;
	BYTE m_bConditionCheck;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTQuestSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TQUESTSET_H__E840F084_1A5C_457A_966D_956BF418A27F__INCLUDED_)
