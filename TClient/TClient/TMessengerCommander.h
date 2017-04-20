#pragma once

class CTMessengerCommander
{
public:
	/// 명령 열거
	enum CmdEnum
	{
		CMD_MSG_ADD_FRIEND,			///< 친구 등록
		CMD_MSG_DEL_FRIEND,			///< 친구 제거
		CMD_MSG_MAKE_GROUP,			///< 친구 그룹 생성
		CMD_MSG_DEL_GROUP,			///< 친구 그룹 제거
		CMD_MSG_CHANGE_GROUP,		///< 친구 그룹을 변경한다.
		CMD_MSG_NAME_GROUP,			///< 친구 그룹 이름을 변경한다.
		CMD_MSG_ADD_BLOCK,			///< 유저를 차단목록에 등록한다.
		CMD_MSG_DEL_BLOCK,			///< 유저를 차단목록에서 제거한다.
		CMD_MSG_COUNT
	};

public:
	struct FriendGroup
	{
		BYTE bGroupID;
		CString strName;
	};
	
	struct FriendMember
	{
		DWORD dwCharID;
		CString strName;
		BYTE bLevel;
		BYTE bGroup;
		BYTE bClass;
		BYTE bConnected;
		DWORD dwRegion;
	};

	typedef std::vector<CTMessengerCommander::FriendMember*> VTFRIENDMEMBER;
	typedef std::map< BYTE, VTFRIENDMEMBER > TMAPLINKGROUPMEMBER;

public:
	static const DWORD CMD_STRING[];

protected:
	CString m_strReqTarget;

public:
	/// 메신저 커맨드 인스턴스를 얻는다.
	static CTMessengerCommander* GetInstance();

	/// 채팅 메시지에 의한 메신저 명령 이벤트를 처리한다.
	static CString OnMessengerCmdByChatMsg(const CString& strCMD, const VECTORSTRING& vPARAM, DWORD dwUSER);
	/// 채팅 메시지 커맨드를 등록한다.
	static void RegChatCmds();

public:
	/// 메신저 관련 명령을 내린다.
	void DoMessengerCommand(CmdEnum eCmd, const CString& strParam1="", const CString& strParam2="");

public:
	/// 친구 목록을 받는다.
	void OnFriendlist(
		BYTE bGroupCount,
		FriendGroup* pGroups,
		BYTE bFriendCount,
		FriendMember* pFriends,
		TMAPLINKGROUPMEMBER& );

	/// 친구 등록을 요청한다.
	void DoAddFriend(const CString& strFriend);
	/// 친구 등록 요청에 대한 응답을 받아 처리한다.
	void OnFriendInvite(const CString& strInviter);
	/// 친구 요청에 대한 답을 보낸다.
	void DoFriendInviteAnswer(BYTE bReply);
	/// 친구 등록 요청을 처리한다.
	void OnAddFriend(BYTE bResult, FriendMember* pFriend);

	/// 친구 제거를 요청한다.
	void DoEraseFriend(DWORD dwCharID);
	/// 친구 제거 요청을 처리한다.
	void OnEraseFriend(DWORD dwCharID);

	/// 친구의 접속 여부를 처리한다.
	void OnConnectFriend(BYTE bResult, const CString& strName, DWORD dwRegion);
	/// 친구의 지역 변경을 처리한다.
	void OnChangeFriendRegion(const CString& strName, DWORD dwRegion);

	/// 친구 그룹 생성을 요청한다.
	void DoMakeFriendGroup(const CString& strName);
	/// 친구 그룹 생성에 대한 요청을 처리한다.
	void OnMakeFriendGroup(BYTE bResult, BYTE bGroupID, const CString& strName);

	/// 친구 그룹 제거를 요청한다.
	void DoEraseFriendGroup(BYTE bGroupID);
	/// 친구 그룹 제거에 대한 요청을 처리한다.
	void OnEraseFriendGroup(BYTE bResult, BYTE bGroupID);

	/// 친구 그룹 변경을 요청한다.
	void DoChangeFriendGroup(DWORD dwCharID, BYTE bWantedGroup);
	/// 친구 그룹 변경에 대한 요청을 처리한다.
	void OnChangeFriendGroup(BYTE bResult, BYTE bGroup, DWORD dwCharID);

	/// 친구 그룹명 변경을 요청한다.
	void DoFriendGroupName(BYTE bGroup, const CString& strName);
	/// 친구 그룹명 변경에 대한 요청을 처리한다.
	void OnFriendGroupName(BYTE bResult, BYTE bGroup, const CString& strName);

	/// 그룹 전체에 대한 초대를 요청한다.
	void DoInviteChatGroup(DWORD dwTMS, BYTE bGroup);
	/// 타겟의 초대를 요청한다.
	void DoInviteChatTarget(DWORD dwTMS, const VECTORDWORD& vTARGET);
	/// 채팅 초대에 대한 요청을 처리한다.
	void OnInviteChatTarget(DWORD dwTMS, const CString& strInviter, LPMCTARGET_ARRAY pTargets);

	/// 메신저 채트 메시지를 보낸다.
	void DoChat(DWORD dwTMS, const CString& strMessage);
	/// 메신저 채트 메시지를 수신한다.
	void OnChat(DWORD dwTMS, const CString& strSender, const CString& strMessage);

	/// 메신저 채트 종료를 요청한다.
	void DoChatOut(CTMessengerChat* pChat);
	/// 메신저 채트 종료를 처리한다.
	void OnChatOut(DWORD dwTMS, const CString& strTarget);

	/// 주어진 유저를 차단한다.
	void DoAddBlock(const CString& strTarget);
	/// 주어진 유저의 차단을 해제한다.
	void DoDeleteBlock(const CString& strTarget);

protected:
	void ChatSysMSG( const CString& strMSG);

protected:
	CTMessengerCommander();
};















