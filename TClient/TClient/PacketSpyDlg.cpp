// PacketSpyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TClient.h"
#include "PacketSpyDlg.h"
#include ".\packetspydlg.h"


const CString CPacketSpyDlg::PACKTYPE_STRS[PACKTYPE_CNT] = 
{
	"ALL",
	"SND",
	"RCV",
};

const CString CPacketSpyDlg::COL_STRS[COL_CNT] =
{
	"시각",
	"패킷"
};

const FLOAT CPacketSpyDlg::SIZE_RATES[COL_CNT] =
{
	0.20f,
	0.79f
};

// CPacketSpyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPacketSpyDlg, CDialog)
CPacketSpyDlg::CPacketSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketSpyDlg::IDD, pParent)
{
	Create(CPacketSpyDlg::IDD, pParent);
	ShowWindow(SW_HIDE);
}

CPacketSpyDlg::~CPacketSpyDlg()
{
	ClearPacket();
}

void CPacketSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PACKET, m_TargetList);
	DDX_Control(pDX, IDC_LIST_CAPTURED, m_SpyList);
	DDX_Control(pDX, IDC_COMBO1, m_TypeCmb);
	DDX_Control(pDX, IDC_COMBO2, m_PacketCmb);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_OutputEdit);
}


BEGIN_MESSAGE_MAP(CPacketSpyDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CAPTURED, OnLvnItemchangedListCaptured)
END_MESSAGE_MAP()

BOOL CPacketSpyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	m_SpyList.GetWindowRect(&rc);
	LONG w = rc.Width();

	INT i;
	for(i=0; i<COL_CNT; ++i)
		m_SpyList.InsertColumn(i, COL_STRS[i], LVCFMT_LEFT, int(w*SIZE_RATES[i]));

	m_SpyList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

	for(i=0; i<PACKTYPE_CNT; ++i)
		m_TypeCmb.AddString(PACKTYPE_STRS[i]);

	m_TypeCmb.SelectString(PACKTYPE_ALL, PACKTYPE_STRS[PACKTYPE_ALL]);

	m_bNeedUpdate = FALSE;
	m_nTimer = SetTimer(1, 50, 0);

	return TRUE;
}

void CPacketSpyDlg::OnTimer(UINT nIDEvent)
{
	UpdateControls();
	CDialog::OnTimer(nIDEvent);
}


void CPacketSpyDlg::OnBnClickedButtonAdd()
{
	int nSel = m_PacketCmb.GetCurSel();
	if( nSel < 0 )
		return;

	DWORD dwID = (DWORD) m_PacketCmb.GetItemData(nSel);
	if( m_mapSndPacket.find(dwID) != m_mapSndPacket.end() )
		AddTarget(TRUE, dwID);
	else
		AddTarget(FALSE, dwID);
}

void CPacketSpyDlg::OnLvnItemchangedListCaptured(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_SpyList.GetFirstSelectedItemPosition();
	if( pos != NULL )
	{
		int nItem = m_SpyList.GetNextSelectedItem(pos);
		SetDlgItemText(IDC_EDIT_OUTPUT, m_vCaptured[nItem]->m_Output);
	}

	*pResult = 0;
}

void CPacketSpyDlg::UpdateControls()
{
	if( !m_bNeedUpdate )
		return;

	m_bNeedUpdate = FALSE;

	int nCur = m_TypeCmb.GetCurSel();
	if( nCur < 0 )
		nCur = 0;

	BOOL bAddSnd = FALSE;
	BOOL bAddRcv = FALSE;

	switch( nCur )
	{
	case PACKTYPE_ALL:
		bAddSnd = TRUE;
		bAddRcv = TRUE;
		break;

	case PACKTYPE_SND:
		bAddSnd = TRUE;
		break;

	case PACKTYPE_RCV:
		bAddRcv = TRUE;
		break;
	}

	m_PacketCmb.ResetContent();
	m_TargetList.ResetContent();

	PACKETINFO_MAP::iterator itrINFO,endINFO;
	PACKETID_MAP::iterator itrID,endID;

	if( bAddSnd )
	{
		itrINFO = m_mapSndPacket.begin();
		endINFO = m_mapSndPacket.end();
		for( ;itrINFO!=endINFO; ++itrINFO)
		{
			INT n = m_PacketCmb.AddString(itrINFO->second.m_strID);
			m_PacketCmb.SetItemData(n, (DWORD_PTR)itrINFO->first);
		}

		itrID = m_mapSndTarget.begin();
		endID = m_mapSndTarget.end();
		for( ;itrID!=endID; ++itrID)
		{
			INT n = m_TargetList.AddString(itrID->second);
			m_TargetList.SetItemData(n, (DWORD_PTR)itrID->first);
		}
	}

	if( bAddRcv )
	{
		itrINFO = m_mapRcvPacket.begin();
		endINFO = m_mapRcvPacket.end();
		for( ;itrINFO!=endINFO; ++itrINFO)
		{
			INT n = m_PacketCmb.AddString(itrINFO->second.m_strID);
			m_PacketCmb.SetItemData(n, (DWORD_PTR)itrINFO->first);
		}

		itrID = m_mapRcvTarget.begin();
		endID = m_mapRcvTarget.end();
		for( ;itrID!=endID; ++itrID)
		{
			INT n = m_TargetList.AddString(itrID->second);
			m_TargetList.SetItemData(n, (DWORD_PTR)itrID->first);
		}
	}
}

int CPacketSpyDlg::GetCurSel(CListCtrl* pList)
{
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if( pos )
		return pList->GetNextSelectedItem(pos);

	return -1;
}

void CPacketSpyDlg::SetCurSel(CListCtrl* pList, int nIndex)
{
	POSITION pos = pList->GetFirstSelectedItemPosition();
	BOOL bUpdate = nIndex != -1;

	while(pos)
	{
		int nSel = pList->GetNextSelectedItem(pos);

		if( nSel != nIndex )
		{
			pList->SetItem(
				nSel, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);

			pList->Update(nSel);
		}
		else
			bUpdate = FALSE;
	}

	if(bUpdate)
	{
		pList->SetItem(
			nIndex, 0,
			LVIF_STATE,
			NULL, 0,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			NULL);

		pList->Update(nIndex);
	}
}

void CPacketSpyDlg::RegPacket(BOOL bSndType, DWORD dwID, const CString& strID, GetOutputStrFn fp)
{
	PacketInfo info;
	info.m_dwID = dwID;
	info.m_strID = strID;
	info.m_fpGetOutput = fp;

	if( bSndType )
		m_mapSndPacket.insert( make_pair(dwID, info) );
	else
		m_mapRcvPacket.insert( make_pair(dwID, info) );

	m_bNeedUpdate = TRUE;
}

void CPacketSpyDlg::AddTarget(BOOL bSndType, DWORD dwID)
{
	PACKETINFO_MAP *pPacketMap;
	PACKETID_MAP *pTarget;
	if( bSndType )
	{
		pPacketMap = &m_mapSndPacket;
		pTarget = &m_mapSndTarget;
	}
	else
	{
		pPacketMap = &m_mapRcvPacket;
		pTarget = &m_mapRcvTarget;
	}

	PACKETID_MAP::iterator itrID = pTarget->find(dwID);
	if( itrID != pTarget->end() )
		return;

	PACKETINFO_MAP::iterator itrINFO = pPacketMap->find(dwID);
	if( itrINFO == pPacketMap->end() )
		return;

	pTarget->insert( make_pair(itrINFO->first, itrINFO->second.m_strID) );
	m_bNeedUpdate = TRUE;
}
void CPacketSpyDlg::RemoveTarget(BOOL bSndType, DWORD dwID)
{
	PACKETID_MAP *pTarget;
	if( bSndType )
		pTarget = &m_mapSndTarget;
	else
		pTarget = &m_mapRcvTarget;

	PACKETID_MAP::iterator itr = pTarget->find(dwID);
	if( itr == pTarget->end() )
		return;

	pTarget->erase(itr);
	m_bNeedUpdate = TRUE;
}

void CPacketSpyDlg::CapturePacket(BOOL bSndType, DWORD dwID, CPacket* pPacket)
{
	PACKETINFO_MAP *pPacketMap;
	PACKETID_MAP *pTarget;
	if( bSndType )
	{
		pPacketMap = &m_mapSndPacket;
		pTarget = &m_mapSndTarget;
	}
	else
	{
		pPacketMap = &m_mapRcvPacket;
		pTarget = &m_mapRcvTarget;
	}

	PACKETID_MAP::iterator itrID = pTarget->find(dwID);
	if( itrID == pTarget->end() )
		return;

	PACKETINFO_MAP::iterator itrINFO = pPacketMap->find(dwID);
	if( itrINFO == pPacketMap->end() )
		return;

	CapturedPacket* pCapPack = new CapturedPacket;
	pCapPack->m_bSndType = bSndType;
	pCapPack->m_dwID = dwID;
	pCapPack->m_Time = CTime::GetCurrentTime();

	if( itrINFO->second.m_fpGetOutput )
		pCapPack->m_Output = (*itrINFO->second.m_fpGetOutput)(pPacket);

	m_vCaptured.push_back(pCapPack);

	int nCnt = m_SpyList.GetItemCount();
	
	CString strTime;
	strTime.Format("%d:%d:%d",
		pCapPack->m_Time.GetHour(),
		pCapPack->m_Time.GetMinute(),
		pCapPack->m_Time.GetSecond());

	m_SpyList.InsertItem(nCnt, strTime);
	m_SpyList.SetItemText(nCnt, COL_PACKET, itrID->second);

	if( !nCnt || GetCurSel(&m_SpyList) == nCnt-1 )
	{
		SetCurSel(&m_SpyList, nCnt);
		
		CSize sz(0,m_SpyList.GetCountPerPage());
		m_SpyList.Scroll(sz);
	}
}

void CPacketSpyDlg::ClearPacket()
{
	if(m_SpyList.GetSafeHwnd())
		m_SpyList.DeleteAllItems();

	for(INT i=0; i<INT(m_vCaptured.size()); ++i)
		delete m_vCaptured[i];
	m_vCaptured.clear();
}
