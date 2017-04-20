// TTranslatorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTTranslatorDlg dialog
class CTTranslatorDlg : public CDialog
{
// Construction
public:
	CTTranslatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TTRANSLATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	void SetCANCEL( BYTE bCANCEL);
	BYTE GetCANCEL();

	void ReleaseTID();
	BYTE LoadTID();

	void BuildParam();

public:
	CRITICAL_SECTION m_cs;
	BYTE m_bCANCEL;

public:
	MAPTRANSLATION m_mapTRANSLATION;
	MAPTRANSLATION m_mapTINFOTEXT;
	MAPTRANSLATION m_mapTMONSTER;
	MAPTRANSLATION m_mapTBATTLE;
	MAPTRANSLATION m_mapSVRMSG;
	MAPTRANSLATION m_mapTOURNAMENT;
	MAPTRANSLATION m_mapTOURNAMENTSCHEDULE;	

	MAPTSTRINGDICBUFF m_mapTEVENTQUATER;
	MAPTSTRINGDICBUFF m_mapTINTERFACE;
	MAPTSTRINGDICBUFF m_mapTHELP;

	MAPTSTRINGDICBUFF m_mapTQOBJECTIVE;
	MAPTDWORDDICBUFF m_mapTQCATEGORY;
	MAPTDWORDDICBUFF m_mapTQTITLE;

	MAPTDWORDDICBUFF m_mapTCOUNTRY;
	MAPTDWORDDICBUFF m_mapTCLIENT;
	MAPTDWORDDICBUFF m_mapTCLASS;
	MAPTDWORDDICBUFF m_mapTDHELP;
	MAPTDWORDDICBUFF m_mapTRACE;
	MAPTDWORDDICBUFF m_mapTINFO;

	VSTRING m_vTQCATEGORY_SQL;
	VSTRING m_vTQTITLE_SQL;
	VSTRING m_vTQTERM_SQL;

	VSTRING m_vTMONSTER_SQL;
	VSTRING m_vTSKILL_SQL;
	VSTRING m_vTITEM_SQL;
	VSTRING m_vTNPC_SQL;

	CString m_strSOURCE;
	CString m_strTARGET;
	CString m_strTEXT;

	int m_nTOTAL;
	int m_nTSTEP;

	int m_nFROM;
	int m_nTO;

	CComboBox m_cLANG_FROM;
	CComboBox m_cLANG_TO;

	CButton m_cSOURCE;
	CButton m_cTARGET;
	CButton m_cTEXT;

protected:
	VSTRING m_vTLANGUAGE[TLANG_CODE_COUNT];
	VDWORD m_vCODEPAGE;

	VSTRING m_vTINFOSTRING[TINFO_TEXT_COUNT];
	MAPNAME m_mapNPCNAME;

	MAPDWORD m_mapMISSION;
	MAPDWORD m_mapNPC;

	MAPTID m_mapTSTRING;
	MAPTID m_mapTERM;
	MAPTID m_mapTID;

protected:
	static DWORD __Build( DWORD_PTR lParam);

	static CString m_vTDBNAME[TACCESSID_COUNT];
	static CString m_vTDOCKIND[TDOC_COUNT];

protected:
	void Build( LPTBDPARAM lParam);

	void BuildTPORTALREGIONCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTMONSTERSHOPCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTITEMMAGICCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTMONTHRANKCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTGODTOWERCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTGODBALLCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTAUCTIONCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTMONSTERCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTPORTALCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTSKILLCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTITEMCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTNPCCHART_TCD( CFile *pLOG, LPTBDPARAM lParam);

	void BuildTAction_Define_Chart_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTRegionChart_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTCountry_Info_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTInfo_Chart_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTClass_Info_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTMenu_Item_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTRace_Info_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTString_TCD( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTHelp_TCD( CFile *pLOG, LPTBDPARAM lParam);

	void BuildTEVENTQUARTERCHART_SQL( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTQUEST_SQL( CFile *pLOG, LPTBDPARAM lParam);
	void BuildTGAME_SQL( CFile *pLOG, LPTBDPARAM lParam);

	void BuildTIF( CFile *pLOG, LPTBDPARAM lParam);
	void BuildMPQ( CFile *pLOG, LPTBDPARAM lParam);

	void LoadDB( CFile *pLOG, LPTBDPARAM lParam);
	void ReleaseDB();

protected:
	CString GetTPREVSKILLFromInfoString( CString strTEXT);
	CString GetTITLEFromInfoString( CString strTEXT);
	CString GetSimpleFormat( CString strTEXT);
	CString GetFormat( CString strTEXT);
	CString ReadStringFromIDH( CFile* pFILE);

	CString Translate(
		LPMAPTRANSLATION pTDIC,
		CFile *pLOG,
		CString strTEXT,
		LPDWORD pCOUNTER);

	CString Translate(
		LPTDICDATA pTDATA,
		CFile *pLOG,
		CString strTEXT,
		LPDWORD pCOUNTER);

	void BuildTINFODIC(
		LPLISTTDICDATA pTDIC,
		CString strTEXT,
		DWORD dwID,
		CFile *pLOG,
		LPDWORD pCOUNTER);

	void TranslateTQUEST(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFROM,
		CFile *pTO,
		LPDWORD pCOUNTER);

	void TranslateTQTERM(
		CFile *pLOG,
		CFile *pFROM,
		CFile *pTO,
		DWORD dwQuestID,
		LPDWORD pCOUNTER);

	void TranslateFRAME(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFROM,
		CFile *pTO,
		DWORD dwFrameID,
		LPDWORD pCOUNTER);

	void TranslateINFO(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CArchive *pFROM,
		CArchive *pTO,
		LPDWORD pCOUNTER);

	void TranslateMON(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CArchive *pFROM,
		CArchive *pTO,
		LPDWORD pCOUNTER);

	void LoadDB(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CString& strFILE);

	void LoadDIC(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

	void LoadTBATTLEZONE(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

	void LoadTDWORDDICBUFF(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTDWORDDICBUFF pTDWORDDIC);

	void LoadTCLIENT(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTDWORDDICBUFF pTDWORDDIC);

	void LoadTPAIRSTRDICBUFF(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTSTRINGDICBUFF pTSTRINGDIC);

	void LoadTINTERFACE(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTSTRINGDICBUFF pTSTRINGDIC);

	void LoadTEVENTQUATER(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTSTRINGDICBUFF pTSTRINGDIC);

	void LoadTMONSTER(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

	void LoadSVRMSG(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

	void LoadTOURNAMENT(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

	void LoadTOURNAMENTSCHEDULE(
		LPTBDPARAM lParam,
		CFile *pLOG,
		CFile *pFILE,
		LPMAPTRANSLATION pTDIC);

protected:
	CString ReadTEXT(
		CFile *pFILE,
		DWORD dwCP);

	BYTE ReadWString(
		CFile *pFILE,
		CString& strTEXT,
		DWORD dwCP);

	void OpenUnicode( CFile *pFILE);
	void WriteWString(
		CFile *pFILE,
		CString strTEXT,
		DWORD dwCP);

	LPTDICBUFF FindTDICBUF(
		CFile *pLOG,
		CString strID,
		LPMAPTSTRINGDICBUFF pTSTRINGDIC);

	LPTDICBUFF FindTDICBUF(
		CFile *pLOG,
		DWORD dwID,
		LPMAPTDWORDDICBUFF pTDWORDDIC);

	CString GetNPCName(
		CFile *pLOG,
		DWORD dwQuestID,
		CString strTEXT,
		LPTDICDATA pTDATA,
		LPDWORD pCOUNTER);

	DWORD GetMissionID(
		CFile *pLOG,
		DWORD dwID);

	DWORD GetTermID(
		CFile *pLOG,
		DWORD dwQuestID,
		BYTE bTermType,
		DWORD dwTermID);

	DWORD GetTSTRING(
		CFile *pLOG,
		CString strID);

	DWORD GetTID(
		CFile *pLOG,
		CString strID);

	void WriteTEXT(
		CFile *pFILE,
		CString strTEXT);

	void LogString(
		CFile *pLOG,
		DWORD dwFMT,
		DWORD dwDATA,
		DWORD dwCP);

	void LogString(
		CFile *pLOG,
		DWORD dwFMT,
		CString strDATA,
		DWORD dwCP);

	void LogString(
		CFile *pLOG,
		DWORD dwID,
		DWORD dwCP);

	void LogString(
		CFile *pLOG,
		CString strLOG,
		DWORD dwCP);

	void Replace_TempInfo( CString* strTEXT );
	void Replace_OrigInfo( CString* strTEXT );

protected:
	void CopyFLOAT(
		CArchive *pFROM,
		CArchive *pTO,
		BYTE bCount);

	void CopyDWORD(
		CArchive *pFROM,
		CArchive *pTO,
		BYTE bCount);

	void CopyWORD(
		CArchive *pFROM,
		CArchive *pTO,
		BYTE bCount);

	void CopyBYTE(
		CArchive *pFROM,
		CArchive *pTO,
		BYTE bCount);

	void CopyINT(
		CArchive *pFROM,
		CArchive *pTO,
		BYTE bCount);

protected:
	int GetTotalStep();

	void UpdateProgressBar(
		LPTBDPARAM lParam,
		int nTSTEP,
		DWORD dwTotal,
		DWORD dwCurrent);

	void UpdateProgressBar(
		LPTBDPARAM lParam,
		int nTSTEP,
		CFile *pFILE);

	void UpdateProgressBar(
		LPTBDPARAM lParam,
		int nTSTEP,
		CArchive *pFILE);

	void SetActionText(
		LPTBDPARAM lParam,
		CString strACTION);

	void SetActionText(
		LPTBDPARAM lParam,
		DWORD dwID);

	void SetActionText(
		LPTBDPARAM lParam,
		DWORD dwID,
		CString strTEXT);

	void SetActionText(
		LPTBDPARAM lParam,
		DWORD dwID,
		CString strTEXT,
		CString strSUB);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonTextFolder();
	afx_msg void OnBnClickedButtonSource();
	afx_msg void OnBnClickedButtonTarget();
	afx_msg void OnBnClickedButtonBuild();
	DECLARE_MESSAGE_MAP()
};
