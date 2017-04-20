#include "Stdafx.h"
#include "TChatCommander.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "Resource.h"


// ====================================================================
const DWORD CTChatCommander::CMD_STRING[] = 
{
	TSTR_CMD_CHAT_BAN,			///< 채팅 밴
	TSTR_CMD_TELEPORT_POS,		///< Teleport Pos
	TSTR_CMD_TELEPORT_TARGET,	///< Teleport Target
	TSTR_CMD_GIFT,				///< GM이 선물주기.
};
// ====================================================================

// ====================================================================
CTChatCommander::CTChatCommander(void)
{
}
// --------------------------------------------------------------------
CTChatCommander* CTChatCommander::GetInstance()
{
	static CTChatCommander mcmd;
	return &mcmd;
}
// ====================================================================
// --------------------------------------------------------------------
void CTChatCommander::ChatSysMSG( const CString& strMSG)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrm = pGame->GetChatFrame();
	CTClientChar* pMainChar = pGame->GetMainChar();

	CString strType;
	strType = CTChart::LoadString( TSTR_INFO_TITLE);

	pChatFrm->ChatSysMSG( strType, pMainChar->GetName(), strMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
}
// ====================================================================
// ====================================================================
CString CTChatCommander::OnMessengerCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	GetInstance()->DoMessengerCommand((CmdEnum)dwUSER, vPARAM);
	return CString("");
}
// --------------------------------------------------------------------
void CTChatCommander::RegChatCmds()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTChatFrame* pChatFrame = pGame->GetChatFrame();
	if( !pChatFrame )
		return;

	CString strCMD;
	for(DWORD i=0; i<CMD_MSG_COUNT; ++i)
	{
		strCMD = CTChart::LoadString( (TSTRING) CMD_STRING[i] );
		pChatFrame->AddChatCmdProc(strCMD, OnMessengerCmdByChatMsg, i);
	}
}
// ====================================================================

// ====================================================================
void CTChatCommander::DoMessengerCommand(CmdEnum eCmd, const VECTORSTRING& vPARAM)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	CTClientSession* pSESSION = pGame->GetSession();

	switch(eCmd)
	{
	case CMD_OPERATOR_CHAT_BAN: 
		{
			if ( vPARAM.size() == 2)
			{
				INT nBanMin = atoi(vPARAM[1]);

				if ( nBanMin > CHAT_BAN_MAX_TIME )
				{
					ChatSysMSG( CTChart::LoadString(TSTR_CMD_CHAT_BAN_MAXMIN) );
				}
				else
				{
					pSESSION->SendCS_CHATBAN_REQ( vPARAM[0], (WORD) nBanMin );
				}
			}
		}
		break;
	case CMD_OPERATOR_TELEPORT_POS:
		{
			if ( vPARAM.size() == 3 )
			{
				float fPos[3];

				for ( int i = 0; i < 3; ++i )
					fPos[i] = atof(vPARAM[i]);

				pSESSION->SendCS_CMMOVE_REQ( pGame->GetMainWnd()->m_bChannel,  pGame->GetClientMap()->m_wMapID, fPos[0], fPos[1], fPos[2] );
			}
		}
		break;

	case CMD_OPERATOR_TELEPORT_TARGET:
		{
			if ( vPARAM.size() == 1 )
			{
				pSESSION->SendCS_CMMOVETOUSER_REQ( vPARAM[0] );
			}
		}
		break;

	case CMD_OPERATOR_GIFT :
		{
			if( vPARAM.size() == 1 )
			{	// 아이디만 쓸 경우. 서버에서 자동으로 캐릭터 레벨별로 선물전달.
				pSESSION->SendCS_CMGIFT_REQ( vPARAM[0], 0 );
			}
			else if( vPARAM.size() == 2 )
			{	// Gift ID를 직접입력.
				WORD wGiftID = (WORD)atoi( vPARAM[1] );
				pSESSION->SendCS_CMGIFT_REQ( vPARAM[0], wGiftID );
			}
		}
		break;
	}
}