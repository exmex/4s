#pragma once

#include <map>
using namespace std;

typedef map< BYTE, CView* > MAPVIEW;

// CTabCtrlST

class CTabCtrlST : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlST)

public:
	CTabCtrlST();
	virtual ~CTabCtrlST();

protected:
	DECLARE_MESSAGE_MAP()

public:
	// View Add
	BOOL	AddView(int nPageID, CRuntimeClass  *pViewClass, CCreateContext *pContext);
	void	SetActiveView(int nPageID); // Active View Set
	CView*	GetView(int nPageID); // View Get
	CView*	GetActiveView(); // Active View Get
	void	Resize(int cx, int cy);

	CView*	m_pActiveView;
	MAPVIEW	m_mapView;

	void TabSelchange(); // Tab Sel Change
	
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};