#include "stdafx.h"
#include "TCinematicDlg.h"


CTCinematicDlg::CTCinematicDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc)
{
	m_pTSKIP = (TButton *) FindKid(ID_CTRLINST_BTN_SKIP);
	m_pMLIST = (TList *) FindKid(ID_CTRLINST_MSG);

	m_pNPCNAME = FindKid(ID_CTRLINST_NPC_NAME);
	m_pBOTTOM = FindKid(ID_CTRLINST_BOTTOM);
	m_pTOP = FindKid(ID_CTRLINST_TOP);

	DefaultPosition( NULL, FALSE);
	m_bTSKIP = TRUE;

	m_pMLIST->SetTextClr(
		TSCENE_TEXT_COLOR,
		TSCENE_OUTLINE_COLOR,
		TRUE);
}

CTCinematicDlg::~CTCinematicDlg()
{
}

void CTCinematicDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if(!m_bTSKIP)
		m_pTSKIP->ShowComponent(FALSE);

	TCOMP_LIST::iterator it = m_pMLIST->GetFirstKidsFinder();
	while(!m_pMLIST->EndOfKids(it))
	{
		TComponent *pKid = m_pMLIST->GetNextKid(it);

		if( pKid && pKid->IsTypeOf(TCML_TYPE_SCROLL))
			pKid->ShowComponent(FALSE);
		else
			pKid->ShowComponent(bVisible);
	}
}

void CTCinematicDlg::DefaultPosition( CPoint *pBASIS, BOOL bRestore)
{
	static DWORD vTCOMP[5][2] = {
		{ ID_CTRLINST_NPC_NAME, TBASISPOINT_CENTER_TOP},
		{ ID_CTRLINST_MSG, TBASISPOINT_CENTER_BOTTOM},
		{ ID_CTRLINST_BOTTOM, TBASISPOINT_LEFT_BOTTOM},
		{ ID_CTRLINST_TOP, TBASISPOINT_LEFT_TOP},
		{ ID_CTRLINST_BTN_SKIP, TBASISPOINT_RIGHT_BOTTOM}};

	for( int i=0; i<5; i++)
	{
		TComponent *pTCOMP = FindKid(vTCOMP[i][0]);

		if(pTCOMP)
		{
			if(!bRestore)
			{
				CPoint point;

				pTCOMP->GetComponentPos(&point);
				point -= CTClientUIBase::m_vDefaultBASIS[vTCOMP[i][1]];

				m_vTCOMP_OFFSET[i] = point;
			}
			else
				pTCOMP->MoveComponent(CTClientUIBase::m_vBasis[vTCOMP[i][1]] + m_vTCOMP_OFFSET[i]);

			switch(vTCOMP[i][0])
			{
			case ID_CTRLINST_BOTTOM	:
			case ID_CTRLINST_TOP	:
				if(CTachyonRes::m_pDEVICE)
					pTCOMP->SetScale( FLOAT(CTachyonRes::m_pDEVICE->m_option.m_dwScreenX) / FLOAT(TBASE_SCREEN_X), 1.0f, FALSE);

				break;
			}
		}
	}
}

void CTCinematicDlg::ResetPosition()
{
	DefaultPosition( NULL, TRUE);
}

void CTCinematicDlg::ResetTEXT( CString *pTEXT)
{
	m_pMLIST->RemoveAll();

	if(pTEXT)
	{
		CSize szITEM;
		int nPOS = 0;

		pTEXT->Replace( _T("\\n"), _T("\n"));
		m_pMLIST->GetItemSize(&szITEM);
		szITEM.cx -= 2 * TINFOTEXT_MARGINE;

		CString strLINE = pTEXT->Tokenize( "\n", nPOS);
		while(!strLINE.IsEmpty())
		{
			while(!strLINE.IsEmpty())
			{
				CString strMBCS = BuildMBCSInfo(strLINE);
				CString strFORMAT = strLINE;
				CSize szTEXT;

				m_pMLIST->GetTextExtentPoint( strLINE, szTEXT);
				while( szTEXT.cx > szITEM.cx )
				{
					int nLength = strFORMAT.GetLength() - 1;

					strFORMAT.Delete(nLength);
					nLength--;

					if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
						strFORMAT.Delete(nLength);

					m_pMLIST->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
				}

				int nCount = strLINE.GetLength() - strFORMAT.GetLength();
				if( nCount > 0 )
					strLINE = strLINE.Right(nCount);
				else
					strLINE.Empty();

				m_pMLIST->AddItem( strFORMAT, 0);
			}

			strLINE = pTEXT->Tokenize( "\n", nPOS);
		}

		m_pMLIST->SetCurSelItem(0);
	}
}

void CTCinematicDlg::ShowTSKIP( BYTE bShow)
{
	if(m_bVisible)
		m_pTSKIP->ShowComponent(bShow);

	m_bTSKIP = bShow;
}
