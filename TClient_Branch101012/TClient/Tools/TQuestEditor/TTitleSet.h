#if !defined(AFX_TTITLESET_H__83535E0C_E8E3_4319_B937_D54F24D99BE2__INCLUDED_)
#define AFX_TTITLESET_H__83535E0C_E8E3_4319_B937_D54F24D99BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TTitleSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTTitleSet recordset

class CTTitleSet : public CRecordset
{
public:
	CTTitleSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTTitleSet)

// Field/Param Data
	//{{AFX_FIELD(CTTitleSet, CRecordset)
	long	m_dwQuestID;
	long	m_dwClassID;
	CString	m_szTitle;
	CString	m_szMessage;
	CString	m_szComplete;
	CString	m_szAccept;
	CString	m_szReject;
	CString m_szSummary;
	CString	m_szNPCName;
	CString	m_szReply;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTTitleSet)
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

#endif // !defined(AFX_TTITLESET_H__83535E0C_E8E3_4319_B937_D54F24D99BE2__INCLUDED_)
