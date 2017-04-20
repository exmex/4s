#if !defined(AFX_TTERMTEXTSET_H__C5E69900_5836_4E60_A141_1F2F05758D83__INCLUDED_)
#define AFX_TTERMTEXTSET_H__C5E69900_5836_4E60_A141_1F2F05758D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTermTextSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTermTextSet recordset

class CTTermTextSet : public CRecordset
{
public:
	CTTermTextSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTTermTextSet)

// Field/Param Data
	//{{AFX_FIELD(CTTermTextSet, CRecordset)
	long	m_dwQuestID;
	long	m_dwTermID;
	CString	m_szMessage;
	CString	m_szObjective;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTermTextSet)
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

#endif // !defined(AFX_TTERMTEXTSET_H__C5E69900_5836_4E60_A141_1F2F05758D83__INCLUDED_)
