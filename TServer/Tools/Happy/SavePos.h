#pragma once
#include "afxwin.h"


// CSavePos 대화 상자입니다.

class CSavePos : public CDialog
{
	DECLARE_DYNAMIC(CSavePos)

public:
	CSavePos(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSavePos();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVEPOS };

protected:
	DECLARE_MESSAGE_MAP()
public:

	CString* m_pstrSaveName;
	afx_msg void OnBnClickedOk();
};
