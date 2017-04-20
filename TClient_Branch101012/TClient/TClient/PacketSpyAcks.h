#pragma once

class CPacketSpyAcks
{
public:
	static void RegAllAcks();

	static CString GET_CS_INVALIDCHAR_ACK(CPacket* pPacket);
	static CString GET_CS_ADDCONNECT_ACK(CPacket* pPacket);
	static CString GET_CS_SHUTDOWN_ACK(CPacket* pPacket);
	static CString GET_CS_CONNECT_ACK(CPacket* pPacket);

	static CString GET_CS_CHARSTATINFO_ACK(CPacket* pPacket);
	static CString GET_CS_CHARINFO_ACK(CPacket* pPacket);
	static CString GET_CS_ENTER_ACK(CPacket* pPacket);

	static CString GET_CS_MOVE_ACK(CPacket* pPacket);
	static CString GET_CS_DEFEND_ACK(CPacket* pPacket);
	static CString GET_CS_SKILLUSE_ACK(CPacket* pPacket);
	static CString GET_CS_ACTION_ACK(CPacket* pPacket);

	static CString GET_CS_STOREOPEN_ACK(CPacket* pPacket);
	static CString GET_CS_STORECLOSE_ACK(CPacket* pPacket);
	static CString GET_CS_STOREITEMLIST_ACK(CPacket* pPacket);
	static CString GET_CS_STOREITEMSELL_ACK(CPacket* pPacket);
};