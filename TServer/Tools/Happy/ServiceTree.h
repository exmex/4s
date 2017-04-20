#pragma once
#include "afxcmn.h"

typedef enum TREE_IMAGE
{
	IMAGE_SERVICE_STOP=0,
	IMAGE_SERVCIE_RUNNING,
	IMAGE_SERVICE_PENDING,
	IMAGE_SERVICE_NOTCONTROL,
	IMAGE_GROUP_NOMAL,
	IMAGE_GROUP_SELECTED,
	IMAGE_WORLD_NOMAL,
	IMAGE_WORLD_SELECTED
} *LPTREE_IMAGE;

// CServiceTree 폼 뷰입니다.
class CHappyDoc;
class CServiceTree : public CFormView
{
	DECLARE_DYNCREATE(CServiceTree)

protected:
	CServiceTree();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CServiceTree();

public:
	void InsertInitData();
	int SelectServiceImage(DWORD dwStatus);
	HTREEITEM FindService(DWORD dwID, HTREEITEM hStart);
	void UpdateServiceTree(DWORD dwID);

public:
	enum { IDD = IDD_SERVICETREE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTreeCtrl m_treeCtrl;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeService(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindragTreeService(NMHDR *pNMHDR, LRESULT *pResult);
public:
	/////////////////////////////////////////////////////
	// 현승룡 Graph
	HTREEITEM  m_hDragItem; 
	int m_DragFlag;
    HCURSOR m_Cursor;
	CRect treeRect;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	/////////////////////////////////////////////////////
public:
	BYTE m_bIsGroup;

	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	
public:
	DWORD m_dwSelectedMapSvrID;
	void MapSvrConnect();
	void MapSvrDisConnect();
};


