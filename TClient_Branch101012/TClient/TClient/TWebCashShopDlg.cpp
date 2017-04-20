#include "Stdafx.h"
#include "TWebCashShopDlg.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"

#ifdef ADD_WEB_CASHSHOP

CTWebCashShopDlg::CTWebCashShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent,pDesc),
m_strURL(""),
m_bWebShop(FALSE),
m_bNavigate(TRUE),
m_pHTTP(NULL)
{
	TComponent* pWEB = FindKid( ID_CTRLINST_WEB);

	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
		CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL )
	{
		m_pHTTP = new THttpCtrl( this, this->m_pDESC );
		m_pHTTP->m_id = pWEB->m_id;
	}
	else
	{
		m_pHTTP = new THttpCtrl( this, pWEB->m_pDESC );
		m_pHTTP->m_id = pWEB->m_id;
	}
	
	RemoveKid( pWEB );
	delete pWEB;
	AddKid( m_pHTTP );

	InitWebCashShop();
}

CTWebCashShopDlg::~CTWebCashShopDlg()
{
}

void CTWebCashShopDlg::ShowComponent(BOOL bVisible /* = TRUE */)
{
	if ( bVisible && m_bNavigate )
	{
		if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
			CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
			CTNationOption::GREECE || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
			CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
			CTNationOption::PORTUGAL )
		{
			CTClientGame* pGame = CTClientGame::GetInstance();

			if ( (m_pHTTP->m_rc.right != pGame->GetDevice()->m_option.m_dwScreenX) && (m_pHTTP->m_rc.bottom != pGame->GetDevice()->m_option.m_dwScreenY) )
			{
				m_pHTTP->SetComponentSize( CSize(pGame->GetDevice()->m_option.m_dwScreenX, pGame->GetDevice()->m_option.m_dwScreenY));
				this->MoveComponent(CTClientUIBase::m_vBasis[ TBASISPOINT_LEFT_TOP ] );
			}

		}
		Navigate();
	}
	
	CTClientUIBase::ShowComponent( bVisible );
}

void CTWebCashShopDlg::InitWebCashShop()
{
	CStdioFile file;
	if ( file.Open(LPCSTR(_T(".\\Tcd\\TCashShop.tcd")), CFile::modeRead|CFile::typeText ) )
	{
		file.ReadString( m_strURL );
		file.Close();

		m_bWebShop = TRUE;
	}
}

void CTWebCashShopDlg::OnMouseMove(UINT nFlags, CPoint pt)
{
	if( m_blDragging )
	{
		CTClientUIBase::OnMouseMove(nFlags, pt);
		if( m_pHTTP )
		{
			m_pHTTP->MoveWndHTTP();

			if( TFRAME_WEB_CASHSHOP != CTClientGame::GetInstance()->GetTopFrame() )
				m_pHTTP->Redraw();
		}
	}
}

void CTWebCashShopDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( pGame->m_bLButtonDown || pGame->m_bRButtonDown )
		ShowComponent( FALSE );

	if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
		CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE || CTNationOption::ROMANIA || CTNationOption::EGYPT ||
		CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL )
	{
		if( nChar == VK_ESCAPE && m_bWebShop )
		{
			ShowComponent(FALSE);
		}
		else
			CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
	}
	else
	{
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
	}
}

void CTWebCashShopDlg::Navigate()
{
	if ( m_pHTTP && !m_strURL.IsEmpty() )
	{
		CTClientGame* pGame = CTClientGame::GetInstance();
		CTClientChar* pMainChar = pGame->GetMainChar();

		TFrame *pFRAME = pGame->GetMainWnd()->m_TNet.FindFrame(ID_FRAME_LOGIN);
		TEdit* pEditID = (TEdit*) pFRAME->FindKid(ID_CTRLINST_ID);
		TEdit* pEditPW = (TEdit*) pFRAME->FindKid(ID_CTRLINST_PW);

		CString strURL = m_strURL;
		CString strID = pEditID->m_strText;
		CString strPW = pEditPW->m_strText;

		CString strCharID;
		strCharID.Format( "%d", pGame->GetMainChar()->m_dwID );
		CString strCharName = pGame->GetMainChar()->m_strNAME;

		if ( CTNationOption::KOR || CTNationOption::ENGLISH )
		{
			strID.MakeUpper();

			static LPCTSTR vREPLACE[][1] = 
			{
				{ "<__HASH_CODE__>" },
			};

			//////////////////////////////////////////////////////////////////////////
			// 0 to 2 :  (0 : MD5(USER ID) 1 : MD5(USER PW) 2 : CHAR SEQ // 3 : USER SEQ)
			// 3 : ( 0 : USER SEQ, 1 : CHAR SEQ )
			//////////////////////////////////////////////////////////////////////////
			static int nAEARSORT[WEBCASHSHOP_AREAR_COUNT_KOREA][4] =
			{
				{ 0,1,2,1 },
				{ 1,2,0,1 },
				{ 2,0,1,1 },
				{ 0,2,1,1 },
				{ 2,1,0,1 },
				{ 1,0,2,1 },
			};

			BYTE bAreaID = rand() % WEBCASHSHOP_AREAR_COUNT_KOREA;

			CString strString[WEBCASHSHOP_STRING_COUNT];
			CString strArearID, strFlag;
			CString strHashCode;

			strHashCode.Empty();
			for ( int i = 0; i < WEBCASHSHOP_STRING_COUNT; ++i )
			{
				switch ( nAEARSORT[bAreaID][i] )
				{
				case 0:
					strString[i].Format("%s", CTClientGame::GetMD5String( strID ) );
					break;
				case 1:
					strString[i].Format("%s", CTClientGame::GetMD5String( strPW ) );
					break;
				case 2:
					strString[i].Format("%d", pMainChar->m_dwID );
					break;
				}
			}

			for ( int i=0; i < WEBCASHSHOP_STRING_COUNT; ++i )
				strHashCode += strString[i];

			strArearID.Format("%d", bAreaID+1);
			strFlag.Format("%d", nAEARSORT[bAreaID][3]);

			strHashCode += strArearID + strFlag;

			strURL.Replace( vREPLACE[0][0], strHashCode );
		}
		else if( CTNationOption::POLAND ||
			CTNationOption::GERMANY ||
			// Arab Ver.
			CTNationOption::EGYPT ||
			CTNationOption::GREECE ||
			CTNationOption::FRANCE ||
			CTNationOption::CZECH || 
			CTNationOption::ITALY || 
			CTNationOption::SPAIN ||
			CTNationOption::UNITEDKINGDOM ||
			CTNationOption::ROMANIA ||
			CTNationOption::UNITEDSTATE ||
			CTNationOption::HUNGARY ||
			CTNationOption::TURKEY ||
			CTNationOption::PORTUGAL )
		{
			enum
			{
				USER_ID,
				USER_PASSWORD,
				SERVER_INDEX,
				CHAR_ID,
				CHAR_NAME,
				MD5_SECURITY,
				R_COUNT,
			};
			static LPCTSTR vREPLACE[ R_COUNT ] = 
			{
				{ "<__USER_ID__>" },
				{ "<__PASSWD__>" },
				{ "<__SERVER_INDEX__>" },
				{ "<__CHAR_ID__>" },
				{ "<__CHAR_NAME__>" },
				{ "<__MD5_SECURITY__>" },
			};

			CTClientWnd* pWnd = CTClientWnd::GetInstance();

			CString strServerIndex;
			strServerIndex.Format( "%d",  pWnd->m_TNet.m_bSelServerGropNum );
			strURL.Replace( vREPLACE[ SERVER_INDEX ], strServerIndex );
			strURL.Replace( vREPLACE[ USER_ID ], pEditID->m_strText );
			strURL.Replace( vREPLACE[ CHAR_ID ], strCharID );
			strURL.Replace( vREPLACE[ CHAR_NAME ], strCharName );

			CString Hash_Code = "oYYq0MDJ85HoB";
			CString strMake = strServerIndex + strCharID + pEditID->m_strText + strCharName + Hash_Code;
			CString strMD5 = CTClientGame::GetMD5String( strMake );
			strMD5.MakeLower();

			strURL.Replace( vREPLACE[ MD5_SECURITY ], strMD5 );
		}
		else
		{
			static LPCTSTR vREPLACE[][1] = 
			{
				{ "<__USER_ID__>" },
				{ "<__PASSWD__>" },
			};

			CString strMD5PW = CTClientGame::GetMD5String( strPW );

			strURL.Replace( vREPLACE[0][0], strID );
			strURL.Replace( vREPLACE[1][0], strMD5PW );
		}

		m_pHTTP->Navigate( (LPCTSTR)strURL );
		//////////////////////////////////////////////////////////////////////////
	}

}

BOOL CTWebCashShopDlg::IsWebShop()
{
	return m_bWebShop;
}

void CTWebCashShopDlg::OnSetFocus( TComponent* pLoseFocus )
{
	TFrame::OnSetFocus( pLoseFocus );

	if( m_pHTTP )
		m_pHTTP->Redraw();
}

#endif

