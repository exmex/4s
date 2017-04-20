#include "Stdafx.h"
#include "TCommunityDlg.h"
#include "TGuildNormalFrame.h"
#include "TClientGame.h"
#include "Resource.h"

// ======================================================================
CTGuildNormalFrame::CTGuildNormalFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent, pDesc, TCOMMUNITY_FRAME_GUILDNORMAL)
{
	static const DWORD dwID[] = 
	{
		ID_CTRLINST_T_GN,	///< 길드명
		ID_CTRLINST_T_GD,	///< 길드 개설 일
		ID_CTRLINST_T_GM,	///< 길드장
		ID_CTRLINST_T_GML,	///< 길드장 작위
		ID_CTRLINST_T_GSM1,	///< 부 길드장
		ID_CTRLINST_T_GSM2,	///< 부 길드장
		ID_CTRLINST_T_GNB,	///< 길드원의 수
		ID_CTRLINST_T_GL,	///< 길드 레벨
		ID_CTRLINST_T_GE,	///< 길드 경험치
		ID_CTRLINST_T_GK,	///< 길드 종류
		ID_CTRLINST_T_MD,	///< 나의 직위
		ID_CTRLINST_T_ML,	///< 나의 작위
		ID_CTRLINST_T_MR,	///< 나의 길드 공헌도
		ID_CTRLINST_RUNE,	///< 길드 자금
		ID_CTRLINST_LUNA,	///< 길드 자금
		ID_CTRLINST_CRON,	///< 길드 자금
		ID_CTRLINST_GN_TEXT,	///< 길드 공지사항
		ID_CTRLINST_T_GR,
		ID_CTRLINST_T_GF,
		ID_CTRLINST_T_GP,
	};
	
	for(INT i=0; i<INFO_COUNT; ++i)
	{
		m_vInfoCtrl[i] = FindKid(dwID[i]);
		m_vInfoCtrl[i]->m_strText = "";
	}

	m_pMarkImgset = static_cast<TImageList*>( FindKid(ID_CTRLINST_MARK) );
	m_pBackImgset = static_cast<TImageList*>( FindKid(ID_CTRLINST_MARK_BACK) );
	m_pBackBase = FindKid(ID_CTRLINST_MARK_BASE);

	m_pMarkImgset->EnableUserColor(TRUE);
	m_pBackImgset->EnableUserColor(TRUE);

	m_pGuildMarkButton = (TButton*) FindKid( ID_CTRLINST_BTN_GUILDMARK );
	m_pGuildExitButton = (TButton*) FindKid( ID_CTRLINST_BTN_OUT );
}
// ----------------------------------------------------------------------
CTGuildNormalFrame::~CTGuildNormalFrame()
{
}
// ======================================================================

// ======================================================================
void CTGuildNormalFrame::RequestInfo()
{
	CTGuildCommander::GetInstance()
		->RequestGuildDetInfo();
}
// ----------------------------------------------------------------------
void CTGuildNormalFrame::ResetInfo()
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	CTGuildCommander* pGuildCmd = CTGuildCommander::GetInstance();
	const CTGuildCommander::GuildDetInfo& info = pGuildCmd->GetGuildDetInfo();

	m_vInfoCtrl[INFO_GNAME]->m_strText = info.m_strGName;

	CTime t( info.m_ldwGDate );
	CString strFMT;
	strFMT.Format( "%d/%d/%d", t.GetYear(), t.GetMonth(), t.GetDay() );
	m_vInfoCtrl[INFO_GDATE]->m_strText = strFMT;

	m_vInfoCtrl[INFO_GMASTER]->m_strText = info.m_strGMaster;
	m_vInfoCtrl[INFO_GMASTER_PEER]->m_strText = pMainChar->m_dwTacticsID ?
		CTChart::LoadString( TSTR_GUILD_PEER_TACTICS ) : CTGuildCommander::GetPeerageStr(info.m_bGMasterPeer);

	m_vInfoCtrl[INFO_GSUBMASTER1]->m_strText = info.m_strGSubMaster1;
	m_vInfoCtrl[INFO_GSUBMASTER2]->m_strText = info.m_strGSubMaster2;
	m_vInfoCtrl[INFO_GKIND]->m_strText = info.m_strGKind;
	m_vInfoCtrl[INFO_MYDUTY]->m_strText = CTGuildCommander::GetDutyStr(info.m_bMyDuty);
	m_vInfoCtrl[INFO_MYPEER]->m_strText = CTGuildCommander::GetPeerageStr(info.m_bMyPeerage);

	if( info.m_nGMbMax == 0 )
		m_vInfoCtrl[INFO_GMBCNT]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, info.m_nGMbCnt);
	else
		m_vInfoCtrl[INFO_GMBCNT]->m_strText = CTChart::Format( TSTR_FMT_NUMPERNUM, info.m_nGMbCnt, info.m_nGMbMax );

	m_vInfoCtrl[INFO_GLEV]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, info.m_nGLev );
	m_vInfoCtrl[INFO_GEXP]->m_strText = CTChart::Format( TSTR_FMT_NUMPERNUM, info.m_dwGExpCur, info.m_dwGExpMax );
	m_vInfoCtrl[INFO_MYCONT]->m_strText = CTChart::Format( TSTR_FMT_NUMPERCENT, info.m_bMyService);
	m_vInfoCtrl[INFO_GMONEY_RUNE]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, info.m_nGRune);
	m_vInfoCtrl[INFO_GMONEY_LUNA]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, info.m_nGLuna);
	m_vInfoCtrl[INFO_GMONEY_CRON]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, info.m_nGCron);

	m_vInfoCtrl[INFO_NOTIFY]->ResetTextSetting();
	m_vInfoCtrl[INFO_NOTIFY]->m_strText = pTextLinker->MakeNetToSimpleLinkText(
		m_vInfoCtrl[INFO_NOTIFY],
		info.m_strNotify);

	m_vInfoCtrl[ INFO_GUILD_RANK ]->m_strText.Empty();

	m_vInfoCtrl[ INFO_GUILD_POINT ]->m_strText.Format( "%d",
		info.m_dwPvPTotalPoint );

	m_vInfoCtrl[ INFO_GUILD_USEABLE_POINT ]->m_strText.Format( "%d",
		info.m_dwPvPUseablePoint );

	if( info.m_bShowMark )
	{
		m_pMarkImgset->ShowComponent(TRUE);
		m_pBackImgset->ShowComponent(TRUE);
		m_pBackBase->ShowComponent(TRUE);

		m_pMarkImgset->SetCurImage( info.m_bMark );
		m_pBackImgset->SetCurImage( info.m_bMarkBack );

		m_pBackBase->m_dwColor = CTClientGuildMark::m_dwCOLOR[info.m_bMarkBackColor1];

		m_pMarkImgset->SetUserColor(CTClientGuildMark::m_dwCOLOR[info.m_bMarkColor]);
		m_pBackImgset->SetUserColor(CTClientGuildMark::m_dwCOLOR[info.m_bMarkBackColor2]);
	}
	else
	{
		m_pMarkImgset->ShowComponent(FALSE);
		m_pBackImgset->ShowComponent(FALSE);
		m_pBackBase->ShowComponent(FALSE);
	}

	if( CTClientGame::GetInstance()->GetMainChar()->m_dwTacticsID != 0 )
		HideComponentWhenTactics();
}
// ======================================================================
void CTGuildNormalFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess()) 
		return;

	if( m_vInfoCtrl[INFO_NOTIFY]->HitTest(pt))
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();
		DWORD dwInfo;

		if( m_vInfoCtrl[INFO_NOTIFY]->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					pTGAME->ShowChatDetailInfo(pItem);

				return;
			}
		}

		SwitchFocus( m_vInfoCtrl[INFO_NOTIFY]);
	}

	ITInnerFrame::OnLButtonDown( nFlags, pt);
}

void CTGuildNormalFrame::HideComponentWhenTactics()
{
	DWORD dwCOMP[] =
	{
		ID_CTRLINST_16,
		ID_CTRLINST_17,
		ID_CTRLINST_18,
		ID_CTRLINST_15,
		ID_CTRLINST_11,
		ID_CTRLINST_12,
		ID_CTRLINST_13,
		ID_CTRLINST_14,
		ID_CTRLINST_8,
		ID_CTRLINST_10,
		ID_CTRLINST_9,
		ID_CTRLINST_RUNE,
		ID_CTRLINST_LUNA,
		ID_CTRLINST_CRON,
		ID_CTRLINST_GOLDBASE,
		ID_CTRLINST_T_GE,
		ID_CTRLINST_B_GG,
		ID_CTRLINST_B_GE,
		ID_CTRLINST_T_MD,
		ID_CTRLINST_T_ML,
		ID_CTRLINST_T_MR,
		ID_CTRLINST_T_GR,
		ID_CTRLINST_T_GF,
		ID_CTRLINST_T_GP,
		ID_CTRLINST_T_GNB,
		ID_CTRLINST_T_GL,
		ID_CTRLINST_T_GK,
		ID_CTRLINST_BTN_GUILDMARK
	};

	INT nCount = sizeof(dwCOMP) / sizeof(DWORD);

	for( INT i=0 ; i < nCount ; ++i )
		FindKid( dwCOMP[ i ] )->ShowComponent( FALSE );
}

void CTGuildNormalFrame::ShowComponent( BOOL bVisible )
{
	ITInnerFrame::ShowComponent( bVisible );

	CTGuildCommander* pGuildCmd = CTGuildCommander::GetInstance();
	const CTGuildCommander::GuildDetInfo& info = pGuildCmd->GetGuildDetInfo();
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();

	m_pGuildMarkButton->ShowComponent( bVisible && pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF );
	m_pGuildExitButton->ShowComponent( bVisible && pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF );
	m_pMarkImgset->ShowComponent(info.m_bShowMark);
	m_pBackImgset->ShowComponent(info.m_bShowMark);
	m_pBackBase->ShowComponent(info.m_bShowMark);

	if( pMainChar->m_dwTacticsID != 0 )
		HideComponentWhenTactics();
}