#pragma once
#include "TMonSpawnInspectorHUGEMAP.h"

class CTMonSpawnInspectorDlg : public CDialog
{
	// 생성
public:
	CTMonSpawnInspectorDlg(CWnd* pParent = NULL);	// 표준 생성자
	virtual ~CTMonSpawnInspectorDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_TMSI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


	// 구현
public:
	HICON					m_hIcon;
	CButton					m_btnClientPath;
	CButton					m_btnDestPath;
	CString					m_strCLIENTPATH;
	CString					m_strDESTPATH;
	CComboBox			m_cmbInputMethod;
	CStatic					m_stcWorkingText;
	CProgressCtrl			m_ctrlWorkingProgress;

protected:
	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonClient();
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonInspection();
	afx_msg void OnBnClickedWorkingStop();

public:
	BOOL					m_bStopWorking;			// 작업 중간에 중단시키기 위한 플래그.

	VTSPAWNDATA		m_vTSPAWNDATA;			// 데이타 소스.
	VTRESULT				m_vTRESULT;					// 검사 결과.
	VTSORTEDMAP		m_vTSORTEDMAP;			// 데이타 소스를 정렬한 데이타.

public:
	BOOL AddSpawnData(
		LPCTSTR szLine);

	VOID SetControlWorkingMode(
		BOOL bWorkingMode);

	BOOL OnInputFromFile();

	BOOL OnInputFromEditBox();

	INT ProcSortDataByMapID();

	INT ProcInspectData();

	INT ProcWriteReport();
	
	INT ProcWriteReviseHeightReport();

	VOID InsertSpawnByMapID(
		WORD wMapID,
		TSPAWNDATA& data);

	LTSPAWNDATA& FindSpawnArrayByUnitIndex(
		VTSORTEDUNIT& vUnits,
		INT nUnitIndex,
		INT nUnitX,
		INT nUnitZ );
	BOOL m_bReviseHeight;
};