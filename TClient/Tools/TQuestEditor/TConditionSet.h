#if !defined(AFX_TCONDITIONSET_H__EA3D257C_254F_4C49_9D73_1B8C2F0D9A6E__INCLUDED_)
#define AFX_TCONDITIONSET_H__EA3D257C_254F_4C49_9D73_1B8C2F0D9A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TConditionSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTConditionSet recordset

class CTConditionSet : public CRecordset
{
public:
	CTConditionSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTConditionSet)

// Field/Param Data
	//{{AFX_FIELD(CTConditionSet, CRecordset)
	long	m_dwQuestID;
	BYTE	m_bConditionType;
	long	m_dwConditionID;
	BYTE	m_bCount;
	long	m_dwID;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTConditionSet)
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

#endif // !defined(AFX_TCONDITIONSET_H__EA3D257C_254F_4C49_9D73_1B8C2F0D9A6E__INCLUDED_)
