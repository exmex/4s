// TMFConvertorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "TMeshTemp.h"

// CTMFConvertorDlg dialog
class CTMFConvertorDlg : public CDialog
{
// Construction
public:
	CTMFConvertorDlg(CWnd* pParent = NULL);	// standard constructor
	~CTMFConvertorDlg();

// Dialog Data
	enum { IDD = IDD_TMFConvertor_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	BYTE IsMeshFile( CString strFileName);

	WORD FindMESH(LPVECTORPTR pMESH, LPWMESHVERTEX pPOINT);
	WORD FindMESH(LPVECTORPTR pMESH, LPMESHVERTEX pPOINT);

	void Convert( CString strSRC, CString strDEST);
	void Convert( CFile *pSRC, CFile *pDEST );

	BOOL Load(LPBYTE pDATA, CTMeshTemp * pMesh);
	void Save(CFile *pDEST, CTMeshTemp * pMesh);

	void ConvertMESH(LPVECTORPTR pMESH, LPIBDATA pINDEX, LPVBDATA pDATA);

public:
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;

	CString m_strDEST;
	CString m_strSRC;

	CButton m_cSRCFOLDER;
	CButton m_cDESTFOLDER;

	CTMeshTemp * m_pMesh;
protected:
	HICON m_hIcon;
protected:
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonDestfloder();
	afx_msg void OnBnClickedConvert();
	afx_msg void OnBnClickedButtonSrcfolder();
	CD3DDevice m_pDevice;
	void ReleaseMesh(CTMeshTemp *pMesh);
};
