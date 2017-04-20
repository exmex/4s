#include "Stdafx.h"
#include "TFameRank.h"
#include "TFameBaseDlg.h"
#include "TClientGame.h"
#include "TFameListDlg.h"
#include "TClientChar.h"

CTFameListDlg::CTFameListDlg( TComponent *pParent, LP_FRAMEDESC pDesc )
: CTClientUIBase( pParent, pDesc )
{
	CPoint ptRankOffset, ptGuildOffset, ptNameOffset, ptCountryOffset, ptMonthfameOffset, ptButtonOffset;
	m_pTITLE = FindKid( ID_CTRLINST_TITLE );
	m_pLIST = (TList*) FindKid( ID_CTRLINST_LIST );
	m_pLIST->LastItemToFront();
}

CTFameListDlg::~CTFameListDlg()
{
	ReleaseData();
}

BOOL CTFameRankSort( CTFameRank* pLeft, CTFameRank* pRight )
{
	if ( pLeft->dwMonthPoint > pRight->dwMonthPoint )
	{
		return TRUE;
	}
	else if ( pLeft->dwMonthPoint == pRight->dwMonthPoint )
	{
		if ( pLeft->wWin > pRight->wWin )
		{
			return TRUE;
		}
		else if ( pLeft->wWin == pRight->wWin )
		{
			if( (pLeft->wWin+pLeft->wLose) > (pRight->wWin+pRight->wLose) )
			{
				return TRUE;
			}
			else if ( (pLeft->wWin+pLeft->wLose) == (pRight->wWin+pRight->wLose) )
			{
				return ( pLeft->strName < pRight->strName );
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

void CTFameListDlg::ReleaseData()
{
	VTFAMERANK::iterator itor;

	for( itor = m_vtFAMERANK.begin(); itor != m_vtFAMERANK.end(); itor++ )
		delete (*itor);

	m_vtFAMERANK.clear();
}

void CTFameListDlg::ResetList()
{
	std::sort( m_vtFAMERANK.begin(), m_vtFAMERANK.end(), CTFameRankSort );
	m_pLIST->RemoveAll();

	VTFAMERANK::iterator it, end;
	it = m_vtFAMERANK.begin();
	end = m_vtFAMERANK.end();
	for(; it != end ; ++it )
	{
		int nLine = m_pLIST->AddString(_T(""), 0);
		m_pLIST->SetItemString(nLine, 1, (*it)->strName );

		m_pLIST->SetItemString(nLine, 2, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[(*it)->bCountry] ) );
		m_pLIST->SetColumnAlign(2, ALIGN_CENTER);

		m_pLIST->SetItemString(nLine, 3, (*it)->strGuild );
		m_pLIST->SetColumnAlign(3, ALIGN_CENTER);

		CString strFMT;
		strFMT.Format("%d", (*it)->dwMonthPoint);
		m_pLIST->SetItemString(nLine, 4, strFMT );
		m_pLIST->SetColumnAlign(4, ALIGN_CENTER);
	}
}