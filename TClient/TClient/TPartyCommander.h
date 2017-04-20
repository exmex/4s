#pragma once

class CTPartyCommander
{
public:
	/// 명령 열거
	enum CmdEnum
	{
		CMD_PARTY_INVITE,				///< 파티 초대
		CMD_PARTY_KICK,					///< 파티원 추방
		CMD_PARTY_OUT,					///< 파티 나가기
		CMD_CORPS_INVITE,				///< 전대 초대
		CMD_ASSISTANT_INVITE,			///< 부관 초대
		CMD_COUNT
	};

	/// 명령에 따른 문자열 배열
	static const DWORD CMD_STRING[CMD_COUNT];

public:
	/// 파티 커맨드 인스턴스를 얻는다.
	static CTPartyCommander* GetInstance();

	/// 채팅 메시지에 의한 파티 명령 이벤트를 처리한다.
	static CString OnPartyCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER);
	/// 채팅 메시지 커맨드를 등록한다.
	static void RegChatCmds();

protected:


public:
	/// 파티 관련 명령을 내린다.
	void DoPartyCommand(CmdEnum eCmd, const CString& strParam="");

protected:
	CTPartyCommander();
};




