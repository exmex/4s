#if !defined(AFX_TREWARDSET_H__A7A74F37_6D9A_4040_9B97_3C41A11CE1E2__INCLUDED_)
#define AFX_TREWARDSET_H__A7A74F37_6D9A_4040_9B97_3C41A11CE1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TRewardSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTRewardSet recordset

class CTRewardSet : public CRecordset
{
public:
	CTRewardSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CTRewardSet)

// Field/Param Data
	//{{AFX_FIELD(CTRewardSet, CRecordset)
	BYTE	m_bCount;
	BYTE	m_bRewardType;
	long	m_dwID;
	long	m_dwQuestID;
	long	m_dwRewardID;
	BYTE	m_bTakeData;
	BYTE	m_bTakeMethod;
	long m_dwQuestMob;
	long m_dwQuestTime;
	long m_dwQuestPathMob;
	long m_dwTicketID;
	BYTE m_bSendQ;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTRewardSet)
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

#endif // !defined(AFX_TREWARDSET_H__A7A74F37_6D9A_4040_9B97_3C41A11CE1E2__INCLUDED_)
