#include "Stdafx.h"
#include "TChatCommander.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "Resource.h"


// ====================================================================
const DWORD CTChatCommander::CMD_STRING[] = 
{
	TSTR_CMD_CHAT_BAN,			///< Ã¤ÆÃ ¹ê
		
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
	CString strParam1(""), strParam2("");
	if( !vPARAM.empty() )
	{
		strParam1 = vPARAM[0];

		if( vPARAM.size() > 1 )
			strParam2 = vPARAM[1];
	}

	GetInstance()->DoMessengerCommand((CmdEnum)dwUSER, strParam1, strParam2);
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
void CTChatCommander::DoMessengerCommand(CmdEnum eCmd, const CString& strParam1, const CString& strParam2)
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
			INT nBanMin = atoi(strParam2);

			if ( nBanMin > CHAT_BAN_MAX_TIME )
			{
				ChatSysMSG( CTChart::LoadString(TSTR_CMD_CHAT_BAN_MAXMIN) );
			}
			else
			{
                pSESSION->SendCS_CHATBAN_REQ( strParam1, (WORD) nBanMin );
			}
		}
		break;
	}
}