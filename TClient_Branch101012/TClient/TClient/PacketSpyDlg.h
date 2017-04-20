#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CPacketSpyDlg 대화 상자입니다.

class CPacketSpyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPacketSpyDlg)

public:
	CPacketSpyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPacketSpyDlg();

public:
	enum { IDD = ID_PACKET_DIALOG };
	
	enum
	{
		PACKTYPE_ALL,
		PACKTYPE_SND,
		PACKTYPE_RCV,
		PACKTYPE_CNT,
	};

	enum
	{
		COL_TIME,
		COL_PACKET,
		COL_CNT,
	};

	typedef CString (*GetOutputStrFn)(CPacket* pPacket);

	struct PacketInfo
	{
		DWORD			m_dwID;
		CString			m_strID;
		GetOutputStrFn	m_fpGetOutput;
	};

	struct CapturedPacket
	{
		BOOL			m_bSndType;
		DWORD			m_dwID;
		CTime			m_Time;
		CString			m_Output;
	};

	typedef std::map<DWORD, PacketInfo>		PACKETINFO_MAP;
	typedef std::map<DWORD, CString>		PACKETID_MAP;
	typedef std::vector<CapturedPacket*>	CAPACKET_ARR;

public:
	static const CString PACKTYPE_STRS[PACKTYPE_CNT];
	static const CString COL_STRS[COL_CNT];
	static const FLOAT SIZE_RATES[COL_CNT];

protected:
	CListBox		m_TargetList;
	CListCtrl		m_SpyList;
	CRichEditCtrl	m_OutputEdit;
	CComboBox		m_TypeCmb;
	CComboBox		m_PacketCmb;

	PACKETINFO_MAP	m_mapSndPacket;
	PACKETINFO_MAP	m_mapRcvPacket;

	PACKETID_MAP	m_mapSndTarget;
	PACKETID_MAP	m_mapRcvTarget;

	CAPACKET_ARR	m_vCaptured;
    
	INT				m_nTimer;
	BOOL			m_bNeedUpdate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnLvnItemchangedListCaptured(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	void UpdateControls();
	
	int GetCurSel(CListCtrl* pList);
	void SetCurSel(CListCtrl* pList, int nIndex);

public:
	void RegPacket(BOOL bSndType, DWORD dwID, const CString& strID, GetOutputStrFn fp);
	
	void AddTarget(BOOL bSndType, DWORD dwID);
	void RemoveTarget(BOOL bSndType, DWORD dwID);
	
	void CapturePacket(BOOL bSndType, DWORD dwID, CPacket* pPacket);
	void ClearPacket();
	
};

