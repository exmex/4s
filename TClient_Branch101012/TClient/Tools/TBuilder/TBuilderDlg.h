// TBuilderDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CTBuilderDlg 대화 상자
class CTBuilderDlg : public CDialog
{
// 생성
public:
	CTBuilderDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_TBUILDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

public:
	CString m_strHISTORYFOLDER;
	CString m_strLISTFOLDER;
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;
	CString m_strGROUP;

	CString m_strHISTORY;
	CString m_strLIST;
	CString m_strDEST;
	CString m_strSRC;

	CButton m_cHISTORY;
	CButton m_cLIST;
	CButton m_cDEST;
	CButton m_cSRC;

	CProgressCtrl m_Progress;
	CString		  m_strStatus;

	CStatic m_cHISTORYTITLE;
	CEdit m_cHISTORYEDIT;

	BOOL m_bSaveIMGBUF;
	BOOL m_bUsePrevID;

	int m_nIMGCOUNT;
	DWORD m_dwTIS;

public:
	VECTORGSFFILE m_vGSFFILE;
	VECTORTPROJECT m_vTANIMATION;
	VECTORTPROJECT m_vTOBJECT;
	VECTORTPROJECT m_vTEXTURE;
	VECTORTPROJECT m_vTIMAGE;
	VECTORTPROJECT m_vTMEDIA;
	VECTORTPROJECT m_vTFRAME;
	VECTORTPROJECT m_vTMESH;
	VECTORTPROJECT m_vTCOMP;
	VECTORTPROJECT m_vTSFX;
	VECTORTPROJECT m_vTMAP;

public:
	BYTE LoadData(
		LPVECTORTPROJECT pTPROJECT,
		LPTGSFFILE,
		int nCount );

	BYTE UseHISTORY();

	D3DFORMAT GetD3DFormat( BYTE bFormat);

	void LoadRoot(
		 LPVECTORTPROJECT pTPROJECT,
		 LPTGSFFILE );

	void LoadGlobal( CFile *pFILE);
	void LoadHISTORY();

	void LoadGSF(
		LPTGSFFILE pGSFFILE );

	void ReportDATA();
	void BuildDATA();

	void AddItem(
		LPVECTORTPROJECT pTPROJECT,
		CItemBase *pItem);

	void ClearItem( LPVECTORTPROJECT pTPROJECT);
	void ClearGSFFILE();
	void Clear();

	void ReleaseDevice();
	void InitDevice();
	void ResetCtrl();

	void GenerateMipmap(
		LPBYTE pData, 
		DWORD dwSize,
		DWORD dwSizeDiv,
		LPBYTE* ppGenData,
		DWORD* pGenSize,
		DWORD* pOrgSize,
		BYTE bFormat=DXT3);

	void FillRect(
		LPLISTCRECT pBLANK,
		LPLISTCRECT pRES,
		CRect *pDEST,
		CRect *pSRC,
		CRect *pRESULT);

protected:
	void BuildHISTORY();
	void BuildHEADER();
	void BuildTSC();

	void BuildTIS( LPVECTORSTRING pTFILE);
	void BuildINDEX(
		CString strINDEX,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildTEX(
		CTextureItem *pItem,
		CFile *pTFILE,
		TEXTURE_DETAIL_LEVEL tex_det);

	void BuildTEX(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE,
		TEXTURE_DETAIL_LEVEL eTexDet);

	void BuildTEX(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX,
		TEXTURE_DETAIL_LEVEL eTexDet);

	void BuildIMG(
		LPMAPTIMGBUF pTIMGBUF,
		CImageItem *pItem,
		CFile *pTFILE,
		CFile *pTIS);

	void BuildIMG(
		LPMAPTIMGBUF pTIMGBUF,
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE,
		CFile *pTIS);

	void BuildIMG(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTIS,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildMEDIA(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem);

	void BuildMEDIA(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildANI(
		CAnimationItem *pItem,
		CFile *pTFILE);

	void BuildANI(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE);

	void BuildANI(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildMESH(
		CMeshItem *pItem,
		CFile *pTFILE);

	void BuildMESH(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE);

	void BuildMESH(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildOBJ(
		COBJItem *pItem,
		CFile *pTFILE);

	void BuildOBJ(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE);

	void BuildOBJ(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildSFX(
		CSFXItem *pItem,
		CFile *pTFILE);

	void BuildSFX(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE);

	void BuildSFX(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

	void BuildEXTMAP(
		CEXTMAPItem *pItem,
		CFile *pTFILE);

	void BuildBSPMAP(
		CBSPMAPItem *pItem,
		CFile *pTFILE);

	void BuildMAP(
		LPMAPRESINDEX pTINDEX,
		CItemBase *pItem,
		DWORD dwFileID,
		CFile *pTFILE);

	void BuildMAP(
		LPTPROJECT pTPROJECT,
		LPVECTORSTRING pTFILE,
		LPMAPRESINDEX pTINDEX);

protected:
	void PrintText(
		CStdioFile& file,
		CString strText);

	void PrintText(
		CStdioFile& file,
		CString strTag,
		CString strText);

	void PrintInt(
		CStdioFile& file,
		int nData);

	void PrintInt(
		CStdioFile& file,
		CString strTag,
		int nData);

	void PrintDWORD(
		CStdioFile& file,
		CString strTag,
		DWORD dwData);

	void PrintOpenTag(
		CStdioFile& file,
		CString strTag);

	void PrintCloseTag(
		CStdioFile& file,
		CString strTag);

	void PrintNewLine(
		CStdioFile& file,
		int nCount = 1);

	void PrintTab(
		CStdioFile& file,
		int nCount = 1);

	void PrintTime( CStdioFile& file);
	CString GetAlign( BYTE bAlign);

	void PrintID(
		CStdioFile& file,
		CString strID,
		DWORD dwID);

	void PrintCTRL(
		CStdioFile& file,
		LPCTRL pCTRL,
		LPCOMP pCOMP,
		BOOL bFRAME,
		int nTab);

	void PrintFrame(
		CStdioFile& file,
		CFRAMEItem *pFRAME);

	void PrintFontType(
		CStdioFile& file,
		LPGSFONT pFONT);

// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCheckUseprevid();
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonGsflist();
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedBuild();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonHistory();
	BOOL m_bImageCheck;
	BOOL m_bMediaCheck;
	BOOL m_bAniCheck;
	BOOL m_bMeshCheck;
	BOOL m_bObjCheck;
	BOOL m_bSfxCheck;
	BOOL m_bMapCheck;
	afx_msg void OnBnClickedImageCheck();
	afx_msg void OnBnClickedMediaCheck();
	afx_msg void OnBnClickedAniCheck();
	afx_msg void OnBnClickedMeshCheck();
	afx_msg void OnBnClickedObjectCheck();
	afx_msg void OnBnClickedSfxCheck();
	afx_msg void OnBnClickedMapCheck();
	BOOL m_bAllCheck;
	afx_msg void OnBnClickedAllCheck();
	CButton m_cImage;
	CButton m_cMedia;
	CButton m_cAni;
	CButton m_cMesh;
	CButton m_cObj;
	CButton m_cSfx;
	CButton m_cMap;
	BOOL m_bTexCheck;
	CButton m_cTex;
	afx_msg void OnBnClickedTexCheck();
	afx_msg void OnBnClickedImgOut();
};
