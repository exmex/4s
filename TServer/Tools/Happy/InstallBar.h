#pragma once

// CInstallBar 대화 상자입니다.

class CInstallBar : public CDialogBar
{
	DECLARE_DYNAMIC(CInstallBar)

public:
	CInstallBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInstallBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INSTALLBAR };

	void OnUpdateUpdateEnable(CCmdUI *pCmdUI){pCmdUI->Enable(m_bEnableUpdate);}
	void OnUpdateOpenEnable(CCmdUI *pCmdUI)	{pCmdUI->Enable(m_bEnableOpen);}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BYTE	m_bEnableUpdate;
	BYTE	m_bEnableOpen;
};
