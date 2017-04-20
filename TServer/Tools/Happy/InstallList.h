#pragma once

// CInstallList 폼 뷰입니다.

class CInstallList : public CFormView
{
	DECLARE_DYNCREATE(CInstallList)

protected:
	CInstallList();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInstallList();

public:
	enum { IDD = IDD_INSTALLLIST };
	enum { OLD_LIST = 1, NEW_LIST = 2};

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CListCtrl m_listCtrl;
	CListCtrl m_listCtrl2;

	virtual void OnInitialUpdate();
	BYTE UpdateList(BYTE bListNum, LPQINSTALL pFile);
};


