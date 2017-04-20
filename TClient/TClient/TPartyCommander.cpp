#include "Stdafx.h"
#include "TPartyCommander.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"


// ====================================================================
const DWORD CTPartyCommander::CMD_STRING[CTPartyCommander::CMD_COUNT] = 
{
	TSTR_CMD_PARTY_INVITE,				///< 파티 초대
	TSTR_CMD_PARTY_KICK,					///< 파티원 추방
	TSTR_CMD_PARTY_OUT,					///< 파티 나가기
	TSTR_MENU_CORPS_REQ,					///< 전대 초대
	TSTR_CMD_ASSISTANT_INVITE			///< 부관 임명
};
// ====================================================================

// ====================================================================
CTPartyCommander::CTPartyCommander()
{
}
// --------------------------------------------------------------------
CTPartyCommander* CTPartyCommander::GetInstance()
{
	static CTPartyCommander party;
	return &party;
}
// ====================================================================

// ====================================================================
CString CTPartyCommander::OnPartyCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CString strParam("");
	if( !vPARAM.empty() )
		strParam = vPARAM.front();

	GetInstance()->DoPartyCommand((CmdEnum)dwUSER, strParam);
	return CString("");
}
// --------------------------------------------------------------------
void CTPartyCommander::RegChatCmds()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrame = pGame->GetChatFrame();
	if( !pChatFrame )
		return;
	
	CString strCMD;
	for(DWORD i=0; i<CMD_COUNT; ++i)
	{
		strCMD = CTChart::LoadString( (TSTRING) CMD_STRING[i] );
		pChatFrame->AddChatCmdProc(strCMD, OnPartyCmdByChatMsg, i);
	}
}
// ====================================================================

// ====================================================================
void CTPartyCommander::DoPartyCommand(CmdEnum eCmd, const CString& strParam)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;
	
	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	switch(eCmd)
	{
	case CMD_PARTY_INVITE:
		{
			if( !strParam.IsEmpty() )
			{
				BYTE bObtainType;
				if( pGame->GetPartyID(pMainChar) )
					bObtainType = pGame->GetPartyItemType();
				else
					bObtainType = PT_ORDER;

				pSession->SendCS_PARTYADD_REQ(strParam,bObtainType);
			}
		}
		break;

	case CMD_PARTY_KICK:
		{
			if( !strParam.IsEmpty() && strParam != pMainChar->m_strNAME )
			{
				for(int i=0; i<pGame->GetPartyCnt(); ++i)
				{
					LPTPARTY pParty = pGame->GetParty(i);
					if( pParty->m_strNAME == strParam )
					{
						pSession->SendCS_PARTYDEL_REQ(pParty->m_dwCharID);
						break;
					}
				}
			}
		}
		break;

	case CMD_PARTY_OUT:
		{
			pSession->SendCS_PARTYDEL_REQ(pMainChar->m_dwID);
		}
		break;

	case CMD_CORPS_INVITE:
		{
			if( !strParam.IsEmpty() )
				pSession->SendCS_CORPSASK_REQ(strParam);
		}
		break;
#ifdef ADD_SKYGARDEN
	case CMD_ASSISTANT_INVITE:
		{
			if( !strParam.IsEmpty() )
				pSession->SendCS_ASSISTANT_REQ(strParam);
		}
		break;
#endif
	}
}
// ====================================================================
