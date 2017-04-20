// TChartProtectDlg.h : header file
//

#pragma once
#include "afxwin.h"

typedef struct tagTNPC			TNPC, *LPTNPC;
typedef vector<LPTNPC>			VTNPC, *LPVTNPC;


// CTChartProtectDlg dialog
class CTChartProtectDlg : public CDialog
{
// Construction
public:
	CTChartProtectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TCHARTPROTECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	void ConvertActionDefChart();
	void ConvertMonsterChart();
	void ConvertActionChart();
	void ConvertSwitchChart();
	void ConvertSkillChart();
	void ConvertGateChart();
	void ConvertItemChart();
	void ConvertItemVisualChar();
	void ConvertItemMagicSfxChar();
	void ConvertItemGradeSfxChar();
	void ConvertItemGradeVisualChart();
	void ConvertRaceChart();
	void ConvertHeadChart();
	void ConvertNudeChart();
	void ConvertSkillTreeChart();
	void ConvertSkillFunctionChart();
	void ConvertRegionChart();
	void ConvertInfoChart();
	void ConvertMinimapChart();
	void ConvertNPCOutwardChart();
	void ConvertNPCChart();
	void ConvertSFXChart();
	void ConvertSTEPChart();
	void ConvertMAPChart();
	void ConvertNODEChart();
	void ConvertBGMChart();
	void ConvertENVChart();
	void ConvertLIGHTChart();
	void ConvertMenuItemChart();
	void ConvertPopupMenuChart();
	void ConvertSkyBoxChart();
	void ConvertFogChart();
	void ConvertLevelChart();
	void ConvertDynamicHelpChart();
	void ConvertArrowDIRChart();
	void ConvertItemMagicChart();
	void ConvertPetChart();
	void ConvertPortalChart();
	void ConvertItemAttrChart();
	void ConvertItemGradeChart();
	void ConvertQuestItemChart();
	void ConvertSkillPointChart();
	void ConvertFormulaChart();
	void ConvertJointChart();
	void ConvertHelpChart();
	void ConvertHelpLinkChart();
	void ConvertGodTowerChart();
	void ConvertGodBallChart();
	void ConvertMonsterShop();
	void ConvertPortalRegion();
	void ConvertMantleTexture();
	void ConvertStringChart();
	void ConvertAuctionTreeChart();
	void ConvertFameRankChart();
	void ConvertEquipCreateChar();
	void ConvertRpsChart();
	void ConvertArenaChart();
	void ConvertDestChart();
	void ConvertUnitLinkChart();

	void WriteTNPC(
		CArchive *pAR,
		LPVTNPC pVTNPC);

	CString Tokenize(
		CString strTEXT,
		CString strTOK,
		int& nPOS);

	bool LoopLine(
		const CString& strSrc, 
		int& nPos, 
		CString& outLine);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;
	CString m_strLISTFOLDER;

	CString m_strDEST;
	CString m_strSRC;
	CString m_strLIST;

	CButton m_cDEST;
	CButton m_cSRC;
	CButton m_cLIST;

	CComboBox m_ComboBox;
	int m_nType;

	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnCbnSelchangeComboChartType();
	afx_msg void OnBnClickedButtonList();
	afx_msg void OnBnClickedButtonListConvert();
};
