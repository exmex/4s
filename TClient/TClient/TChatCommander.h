#pragma once

class CTChatCommander
{
public:
	/// 명령 열거
	enum CmdEnum
	{
		CMD_OPERATOR_CHAT_BAN,			///< 채팅 차단
		CMD_OPERATOR_TELEPORT_POS,		///< Teleport
		CMD_OPERATOR_TELEPORT_TARGET,
		CMD_OPERATOR_GIFT,				///< GM이 선물주기.
		CMD_MSG_COUNT
	};

	enum
	{
		CHAT_BAN_MAX_TIME = 30000,
	};

public:
	static const DWORD CMD_STRING[];

public:
	/// 채팅 커맨드 인스턴스를 얻는다.
	static CTChatCommander* GetInstance();

	/// 채팅 메시지에 의한 메신저 명령 이벤트를 처리한다.
	static CString OnMessengerCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER);
	/// 채팅 메시지 커맨드를 등록한다.
	static void RegChatCmds();

public:
	/// 채팅 관련 명령을 내린다.
	void DoMessengerCommand(CmdEnum eCmd, const VECTORSTRING& vPARAM);
	void ChatSysMSG( const CString& strMSG);

protected:
	

protected:
	CTChatCommander(void);
};
