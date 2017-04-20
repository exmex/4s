#if !defined(AFX_TTERMSET_H__5CC70DD2_8992_4BAD_A6D7_09B60590EEE0__INCLUDED_)
#define AFX_TTERMSET_H__5CC70DD2_8992_4BAD_A6D7_09B60590EEE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTermSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTermSet recordset

class CTTermSet : public CRecordset
{
public:
	CTTermSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTTermSet)

// Field/Param Data
	//{{AFX_FIELD(CTTermSet, CRecordset)
	BYTE	m_bCount;
	BYTE	m_bTermType;
	long	m_dwID;
	long	m_dwQuestID;
	long	m_dwTermID;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTermSet)
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

#endif // !defined(AFX_TTERMSET_H__5CC70DD2_8992_4BAD_A6D7_09B60590EEE0__INCLUDED_)
